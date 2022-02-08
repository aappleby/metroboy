//--------------------------------------------------------------------------------
// MODULE:       uart_hello
// MODULEPARAMS: 
// INPUTS:       i_cts, i_idle, 
// OUTPUTS:      o_data, o_req, o_done, 
// LOCALPARAMS:  message_len, cursor_bits, 
// FIELDS:       state, cursor, memory, data, 
// SUBMODULES:   
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none

`include "metron.h.sv"

//==============================================================================

module uart_hello
(clk, rst_n, i_cts, i_idle, o_data, o_req, o_done);
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n;
  input bool i_cts;
  input bool i_idle; 

  localparam int message_len = 'd512;
  localparam int cursor_bits = $clog2(message_len);

  typedef enum logic[1:0] { WAIT, SEND, DONE } e_state;

  e_state state;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] memory['d512];
  logic[7:0] data;

  output logic[7:0] o_data;
  output logic o_req;
  output logic o_done;

  //----------------------------------------

  initial begin : INIT
    $readmemh("obj/message.hex", memory, 'd0, 'd511);
    o_data = 8'd0;
    o_req = 1'd0;
    o_done = 1'd0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : TICK
    if (!rst_n) begin
      state <= 2'(WAIT);
      cursor <= cursor_bits'('d0);
    end
    else begin
      data <= memory[cursor];
      if (state == WAIT && i_idle) begin
        state <= 2'(SEND);
      end
      else if (state == SEND && i_cts) begin
        if (cursor == (message_len - 'd1)) state <= 2'(DONE);
        cursor <= cursor_bits'(cursor + 'd1);
      end
      else if (state == DONE) begin
        //state = WAIT;
        cursor <= cursor_bits'('d0);
      end
    end
  end

  //----------------------------------------

  always_comb begin : TOCK
    o_data = data;
    o_req = state == SEND;
    o_done = state == DONE;
  end

endmodule

//==============================================================================
