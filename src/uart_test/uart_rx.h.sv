//--------------------------------------------------------------------------------
// MODULE:       uart_rx
// MODULEPARAMS: cycles_per_bit, 
// INPUTS:       i_serial, 
// OUTPUTS:      o_data, o_valid, o_sum, 
// LOCALPARAMS:  cycle_bits, cycle_max, cursor_max, cursor_bits, 
// FIELDS:       cycle, cursor, buffer, sum, temp, 
// SUBMODULES:   
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none

`include "metron.h.sv"

//==============================================================================


module uart_rx
#(parameter int cycles_per_bit = 'd4)
(clk, rst_n, i_serial, o_data, o_valid, o_sum);
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n;
  input logic i_serial; 

  localparam int cycle_bits = $clog2(cycles_per_bit);
  localparam int cycle_max = cycles_per_bit - 'd1;
  localparam int cursor_max = 'd9;
  localparam int cursor_bits = $clog2(cursor_max);

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] buffer;
  logic[31:0] sum;
  logic[7:0] temp;

  output logic[7:0] o_data;
  output logic o_valid;
  output logic[31:0] o_sum;

  //----------------------------------------

  initial begin : INIT
    o_data = 'd0;
    o_valid = 'd0;
    o_sum = 'd0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : TICK
    if (!rst_n) begin
      cycle <= 'd0;
      cursor <= 'd0;
      buffer <= 'd0;
      sum <= 'd0;
      temp <= 'd0;
    end else begin
      if (cycle != 'd0) begin
        cycle <= cycle - 'd1;
      end else if (cursor != 'd0) begin
        logic[7:0] temp;
        temp = (i_serial << 'd7) | (buffer >> 'd1);
        if (cursor - 'd1 == 'd1) sum <= sum + temp;
        cycle <= cycle_max;
        cursor <= cursor - 'd1;
        buffer <= temp;
      end
      else if (i_serial == 'd0) begin
        cycle <= cycle_max;
        cursor <= cursor_max;
      end

    end
  end

  //----------------------------------------

  always_comb begin : TOCK
    o_data = buffer;
    o_valid = cursor == 'd1;
    o_sum = sum;
  end

endmodule

//==============================================================================