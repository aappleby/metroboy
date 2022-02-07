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
#(parameter int cycles_per_bit = 'd4)
(clk, rst_n, i_data, i_req, o_serial, o_cts, o_idle);
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n;
  input logic[7:0] i_data;
  input logic i_req; 

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  localparam int extra_stop_bits = 'd7;

  localparam int cycle_bits = $clog2(cycles_per_bit);
  localparam int cycle_max = cycles_per_bit - 'd1;

  localparam int cursor_bits = $clog2('d10 + extra_stop_bits);
  localparam int cursor_max = 'd10 + extra_stop_bits - 'd1;

  logic[cycle_bits-1:0] cycle;
  logic[cursor_bits-1:0] cursor;
  logic[8:0] buffer;

  output logic o_serial;
  output logic o_cts;
  output logic o_idle;

  //----------------------------------------

  initial begin : INIT
    o_serial = 'd0;
    o_cts = 'd0;
    o_idle = 'd0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : TICK
    if (!rst_n) begin
      cycle <= 'd0;
      cursor <= 'd0;
      buffer <= 'h1FF;
    end else begin
      if (cursor <= extra_stop_bits && cycle == 'd0 && i_req) begin
        // Transmit start
        cycle <= cycle_max;
        cursor <= cursor_max;
        buffer <= i_data << 'd1;
      end else if (cycle != 'd0) begin
        // Bit delay
        cycle <= cycle - 'd1;
        cursor <= cursor;
        buffer <= buffer;
      end else if (cursor != 'd0) begin
        // Bit delay done, switch to next bit.
        cycle <= cycle_max;
        cursor <= cursor - 'd1;
        buffer <= (buffer >> 'd1) | 'h100;
      end
    end
  end

  //----------------------------------------

  always_comb begin : TOCK
    o_serial = buffer & 'd1;
    o_cts = ((cursor == extra_stop_bits) && (cycle == 'd0)) || (cursor < extra_stop_bits);
    o_idle = (cursor == 'd0) && (cycle == 'd0);
  end

endmodule

//==============================================================================
