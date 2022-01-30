//--------------------------------------------------------------------------------
// MODULE:       uart_rx
// MODULEPARAMS: cycles_per_bit, 
// INPUTS:       i_serial, 
// OUTPUTS:      o_data, o_valid, o_sum, 
// LOCALPARAMS:  cycle_bits, cycle_max, cursor_max, cursor_bits, 
// FIELDS:       cycle, cursor, buffer, sum, 
// SUBMODULES:   
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none
`include "metron.h.sv"

//==============================================================================

module uart_rx
#(parameter int cycles_per_bit = 4)
(clk, rst_n, i_serial, o_data, o_valid, o_sum); 
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n;
  input logic i_serial;

  localparam int cycle_bits = $clog2(cycles_per_bit);
  localparam int cycle_max = cycles_per_bit - 1;
  localparam int cursor_max = 9;
  localparam int cursor_bits = $clog2(cursor_max);

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] buffer;
  logic[31:0] sum;

  output logic[7:0] o_data;
  output logic o_valid;
  output logic[31:0] o_sum;

  //----------------------------------------

  initial begin : INIT
    o_data = 0;
    o_valid = 0;
    o_sum = 0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : TICK
    if (!rst_n) begin
      cycle <= 0;
      cursor <= 0;
      buffer <= 0;
      sum <= 0;
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

  always_comb begin : TOCK
    o_data = buffer;
    o_valid = cursor == 1;
    o_sum = sum;
  end

endmodule

//==============================================================================