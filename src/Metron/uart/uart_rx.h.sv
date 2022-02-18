`default_nettype none


`include "../metron_tools.h.sv"

//==============================================================================
/* verilator lint_off WIDTH */


module uart_rx
#(parameter int cycles_per_bit = 4)
(
input logic clk,
input logic rst_n,

input logic i_serial,
output logic[7:0] o_data,
output logic o_valid,
output logic[31:0] o_sum
); 

  localparam int cycle_bits = $clog2(cycles_per_bit);
  localparam int cycle_max = cycles_per_bit - 1;
  localparam int cursor_max = 9;
  localparam int cursor_bits = $clog2(cursor_max);

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] buffer;
  logic[31:0] sum;
  logic[7:0] temp;

  
  
  

  //----------------------------------------

  initial begin : init
    o_data = 0;
    o_valid = 0;
    o_sum = 0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tick
    if (!rst_n) begin
      cycle <= 0;
      cursor <= 0;
      buffer <= 0;
      sum <= 0;
      temp <= 0;
    end else begin
      if (cycle != 0) begin
        cycle <= cycle - 1;
      end else if (cursor != 0) begin
        logic[7:0] temp;
        temp = (i_serial << 7) | (buffer >> 1);
        if (cursor - 1 == 1) sum <= sum + temp;
        cycle <= cycle_max;
        cursor <= cursor - 1;
        buffer <= temp;
      end
      else if (i_serial == 0) begin
        cycle <= cycle_max;
        cursor <= cursor_max;
      end

    end
  end

  //----------------------------------------

  always_comb begin : tock
    o_data = buffer;
    o_valid = cursor == 1;
    o_sum = sum;
  end

endmodule

//==============================================================================