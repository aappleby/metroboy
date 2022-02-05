//--------------------------------------------------------------------------------
// MODULE:       uart_tx
// MODULEPARAMS: cycles_per_bit, 
// INPUTS:       i_data, i_req, 
// OUTPUTS:      o_serial, o_cts, o_idle, 
// LOCALPARAMS:  extra_stop_bits, cycle_bits, cycle_max, cursor_bits, cursor_max, 
// FIELDS:       cycle, cursor, buffer, 
// SUBMODULES:   
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none

`include "metron.h.sv"

//==============================================================================


module uart_tx
#(parameter int cycles_per_bit = 4)
(clk, rst_n, i_data, i_req, o_serial, o_cts, o_idle);
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n;
  input logic[7:0] i_data;
  input logic i_req; 

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  localparam int extra_stop_bits = 7;

  localparam int cycle_bits = $clog2(cycles_per_bit);
  localparam int cycle_max = cycles_per_bit - 1;

  localparam int cursor_bits = $clog2(10 + extra_stop_bits);
  localparam int cursor_max = 10 + extra_stop_bits - 1;

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[8:0] buffer;

  output logic o_serial;
  output logic o_cts;
  output logic o_idle;

  //----------------------------------------

  initial begin : INIT
    o_serial = 0;
    o_cts = 0;
    o_idle = 0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : TICK
    if (!rst_n) begin
      cycle <= 0;
      cursor <= 0;
      buffer <= 'h1FF;
    end else begin
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
        buffer <= (buffer >> 1) | 'h100;
      end
    end
  end

  //----------------------------------------

  always_comb begin : TOCK
    o_serial = buffer & 1;
    o_cts = ((cursor == extra_stop_bits) && (cycle == 0)) || (cursor < extra_stop_bits);
    o_idle = (cursor == 0) && (cycle == 0);
  end

endmodule

//==============================================================================
