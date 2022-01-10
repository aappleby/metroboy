`default_nettype none

//==============================================================================

module uart_tx
#(parameter cycles_per_bit = 4)
(
  input logic clk,
  input logic rst_n,

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

  localparam extra_stop_bits = 7;

  localparam cycle_bits = $clog2(cycles_per_bit);
  localparam cycle_max  = cycle_bits'(cycles_per_bit) - 1'b1;

  localparam cursor_bits = $clog2(10 + extra_stop_bits);
  localparam cursor_max = 10 + extra_stop_bits - 1;

  logic[cycle_bits-1:0]    cycle;
  logic[cursor_bits-1:0] cursor;
  logic[8:0] buffer;

  //----------------------------------------

  task automatic reset();
    cycle  <= '0;
    cursor <= '0;
    buffer <= 9'h1FF;
  endtask

  //----------------------------------------

  // having combi logic in tick doesn't work in icarus
  always_comb begin
    o_serial = buffer[0];
    o_cts    = ((cursor == extra_stop_bits) && (cycle == 0)) || (cursor < extra_stop_bits);
    o_idle   = (cursor == 0) && (cycle == 0);
  end

  //----------------------------------------
  
  task automatic tock();
    if (cursor <= extra_stop_bits && cycle == 0 && i_req) begin
      // Transmit start
      cycle  <= cycle_max;
      cursor <= cursor_max;
      buffer <= { i_data, 1'b0 };
    end else if (cycle != 0) begin
      // Bit delay
      cycle  <= cycle - 1;
      cursor <= cursor;
      buffer <= buffer;
    end else if (cursor != 0) begin
      // Bit delay done, switch to next bit.
      cycle  <= cycle_max;
      cursor <= cursor - 1;
      buffer <= (buffer >> 1) | 9'h100;
    end

    // having combi logic as blocking assignment here doesn't work in verilator

  endtask

  //----------------------------------------

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      reset();
    end else begin
      tock();
    end
  end

endmodule

//==============================================================================
