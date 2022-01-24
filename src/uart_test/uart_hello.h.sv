//--------------------------------------------------------------------------------
// MODULE:       uart_hello
// MODULEPARAMS: 
// INPUTS:       i_cts, i_idle, 
// OUTPUTS:      o_data, o_req, o_done, 
// LOCALPARAMS:  message_len, cursor_bits, 
// FIELDS:       state, cursor, memory, data, 
// SUBMODULES:   

/* verilator lint_off WIDTH */
`default_nettype none
`include "metron.h.sv"

//==============================================================================

module uart_hello
(clk, rst_n, i_cts, i_idle, o_data, o_req, o_done);
  input logic clk;
  input logic rst_n;

  input logic i_cts;
  input logic i_idle;
  
  output logic[7:0] o_data;
  output logic o_req;
  output logic o_done;

  //----------------------------------------
  /*verilator public_module*/

  localparam /*const*/ int message_len = 512;
  localparam /*const*/ int cursor_bits = $clog2(message_len);

  typedef enum { WAIT, SEND, DONE } e_state;
  logic[1:0] state;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] memory[512];
  logic[7:0] data;

  //----------------------------------------

  /*void*/ initial begin
    $readmemh("obj/message.hex", memory, 0, 511);
  end

  //----------------------------------------

  /*void*/ always_comb begin
    o_data = data;
    o_req = state == SEND;
    o_done = state == DONE;
  end

  //----------------------------------------

  /*void*/ always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      state <= WAIT;
      cursor <= 0;
    end else begin
      data <= memory[cursor];
      if (state == WAIT && i_idle) begin
        state <= SEND;
      end else if (state == SEND && i_cts) begin
        if (cursor == (message_len - 1)) state <= DONE;
        cursor <= cursor + 1;
      end else if (state == DONE) begin
        state <= WAIT;
        cursor <= 0;
      end
    end
  end
endmodule
