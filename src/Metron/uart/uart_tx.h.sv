`default_nettype none


`include "../metron_tools.h.sv"

//==============================================================================
/* verilator lint_off WIDTH */


module uart_tx
#(parameter int cycles_per_bit = 4)
(
input logic clk,
input logic rst_n,

input logic[7:0] i_data,
input logic i_req,
output logic o_serial,
output logic o_cts,
output logic o_idle
); 

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  localparam int extra_stop_bits = 7;
  localparam int cycle_bits = $clog2(cycles_per_bit);
  localparam int cursor_bits = $clog2(10 + extra_stop_bits);

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[8:0] buffer;

  
  
  

  //----------------------------------------

  initial begin : init
    o_serial = 0;
    o_cts = 0;
    o_idle = 0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tick
    if (!rst_n) begin
      cycle <= 0;
      cursor <= 0;
      buffer <= 12'h1FF;
    end else begin
      logic[cycle_bits-1:0] cycle_max;
      logic[cursor_bits-1:0] cursor_max;
      cycle_max = cycle_bits'(cycles_per_bit - 1);
      cursor_max = cursor_bits'(10 + extra_stop_bits - 1);

      if (cursor <= extra_stop_bits && cycle == 0 && i_req) begin
        // Transmit start
        cycle <= cycle_max;
        cursor <= cursor_max;
        buffer <= i_data << 1;
      end else if (cycle != 0) begin
        // Bit delay
        cycle <= cycle - 1;
        cursor <= cursor;
        buffer <= buffer;
      end else if (cursor != 0) begin
        // Bit delay done, switch to next bit.
        cycle <= cycle_max;
        cursor <= cursor - 1;
        buffer <= (buffer >> 1) | 12'h100;
      end
    end
  end

  //----------------------------------------

  always_comb begin : tock
    o_serial = buffer & 1;
    o_cts = ((cursor == extra_stop_bits) && (cycle == 0)) || (cursor < extra_stop_bits);
    o_idle = (cursor == 0) && (cycle == 0);
  end

endmodule

//==============================================================================
