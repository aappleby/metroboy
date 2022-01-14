/* verilator lint_off WIDTH */
`default_nettype none

//==============================================================================

module uart_tx
#(parameter cycles_per_bit = 4)
(
  input logic clk,
  input logic rst_n,

  //----------------------------------------

  input logic[7:0] i_data,
  input logic i_req,

  output logic o_serial,
  output logic o_cts,
  output logic o_idle
);
  /*verilator public_module*/

  //----------------------------------------
  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages

  //localparam cycles_per_bit = 3;
  localparam extra_stop_bits = 7;

  localparam cycle_bits = $clog2(cycles_per_bit);
  localparam cycle_max  = cycle_bits'(cycles_per_bit) - 1'b1;

  localparam cursor_bits = $clog2(10 + extra_stop_bits);
  localparam cursor_max = 10 + extra_stop_bits - 1;

  logic[cycle_bits-1:0]  cycle;
  logic[cursor_bits-1:0] cursor;
  logic[8:0] buffer;

  //----------------------------------------

  always_comb begin : tick
    o_serial = buffer & 1;
    o_cts    = ((cursor == extra_stop_bits) && (cycle == 0)) || (cursor < extra_stop_bits);
    o_idle   = (cursor == 0) && (cycle == 0);
  end

  //----------------------------------------
  
  always_ff @(posedge clk, negedge rst_n) begin : tock
    if (!rst_n) begin
      cycle  = '0;
      cursor = '0;
      buffer = 9'h1FF;
    end else begin
      if (cursor <= extra_stop_bits && cycle == 0 && i_req) begin
        // Transmit start
        cycle  = cycle_max;
        cursor = cursor_max;
        buffer = i_data << 1;
      end else if (cycle != 0) begin
        // Bit delay
        cycle  = cycle - 1;
        cursor = cursor;
        buffer = buffer;
      end else if (cursor != 0) begin
        // Bit delay done, switch to next bit.
        cycle  = cycle_max;
        cursor = cursor - 1;
        buffer = (buffer >> 1) | 9'h100;
      end
    end
  end

  //----------------------------------------

  /*
  void dump_header() {
    printf("[tx cyc bit buf serial cts idle] ");
  }

  void dump() {
    printf("[   %-3d %-3d %03x %-6d %-3d %-4d] ", cycle, cursor, buffer, o_serial, o_cts, o_idle);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module tx $end\n");
    fprintf(d.file, "$var wire 5  tx_cycle    cycle    $end\n");
    fprintf(d.file, "$var wire 5  tx_cursor   cursor   $end\n");
    fprintf(d.file, "$var wire 9  tx_buf      buf      $end\n");
    fprintf(d.file, "$var wire 1  tx_o_cts    o_cts    $end\n");
    fprintf(d.file, "$var wire 1  tx_o_idle   o_idle   $end\n");
    fprintf(d.file, "$var wire 1  tx_o_serial o_serial $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("tx_cycle",    cycle, 5);
    d.set_value("tx_cursor",   cursor, 5);
    d.set_value("tx_buf",      buffer, 9);

    d.set_value("tx_o_serial", o_serial, 1);
    d.set_value("tx_o_cts",    o_cts, 1);
    d.set_value("tx_o_idle",   o_idle, 1);
  }
  */
endmodule

//==============================================================================
