/* verilator lint_off WIDTH */
`default_nettype none

//==============================================================================

module uart_rx
#(parameter cycles_per_bit = 4)
(
  input logic clk,
  input logic rst_n,

  //----------------------------------------

  input  logic       i_serial,

  output logic[7:0]  o_data,
  output logic       o_valid,
  output logic[31:0] o_sum
);
  //----------------------------------------
  /*verilator public_module*/

  localparam cycle_bits  = $clog2(cycles_per_bit);
  localparam cycle_max   = cycles_per_bit - 1;
  localparam cursor_max  = 9;
  localparam cursor_bits = $clog2(cursor_max);

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] buffer;
  logic[31:0] sum;

  //----------------------------------------

  always_comb begin : tick
    o_data  = buffer;
    o_valid = cursor == 1;
    o_sum   = sum;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tock
    if (!rst_n) begin
      cycle  <= 0;
      cursor <= 0;
      buffer <= 0;
      sum    <= 0;
    end else begin
      if (cycle != 0) begin
        cycle <= cycle - 1;
      end  else if (cursor != 0) begin
        logic [7:0] temp;
  
        temp = (i_serial << 7) | (buffer >> 1);
        if (cursor - 1 == 1) sum <= sum + temp;
  
        cycle  <= cycle_max;
        cursor <= cursor - 1;
        buffer <= temp;
      end
      else if (i_serial == 0) begin
        cycle  <= cycle_max;
        cursor <= cursor_max;
      end
    end
  end

  //----------------------------------------

  /*
  void dump_header() {
    printf("[rx cyc cursor o_data o_valid o_sum] ");
  }

  void dump() {
    printf("[   %-3d %-3d %-6x %-7d %05x] ", cycle, cursor, o_data, o_valid, o_sum);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module rx $end\n");
    fprintf(d.file, "$var wire %d  rx_cycle   cycle   $end\n",  cycle_bits);
    fprintf(d.file, "$var wire %d  rx_cursor  cursor  $end\n", cursor_bits);
    fprintf(d.file, "$var wire 8   rx_data    data    $end\n");
    fprintf(d.file, "$var wire 32  rx_sum     sum     $end\n");
    fprintf(d.file, "$var wire 8   rx_o_data  o_data  $end\n");
    fprintf(d.file, "$var wire 1   rx_o_valid o_valid $end\n");
    fprintf(d.file, "$var wire 32  rx_o_sum   o_sum   $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("rx_cycle",   cycle,   cycle_bits);
    d.set_value("rx_cursor",  cursor,  cursor_bits);
    d.set_value("rx_data",    buffer,    8);
    d.set_value("rx_sum",     sum,     32);
    d.set_value("rx_o_data",  o_data,  8);
    d.set_value("rx_o_valid", o_valid, 1);
    d.set_value("rx_o_sum",   o_sum,   32);
  }
  */

endmodule

//==============================================================================
