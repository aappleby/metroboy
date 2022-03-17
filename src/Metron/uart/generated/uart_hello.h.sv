
`include "metron_tools.h.sv"

//==============================================================================

module uart_hello
(
  input logic clock,
  input logic i_rstn,
  input logic i_cts,
  input logic i_idle,
  output logic[7:0] o_data,
  output logic o_req,
  output logic o_done
);
 /*public:*/
  /*verilator public_module*/

  localparam /*const*/ int message_len = 512;
  localparam /*const*/ int cursor_bits = $clog2(message_len);

  typedef enum logic[1:0] {WAIT, SEND, DONE} state;

  state s;
  logic[cursor_bits-1:0] cursor;
  logic[7:0] memory[512];
  logic[7:0] data;

  /*logic<8> o_data;*/
  /*logic<1> o_req;*/
  /*logic<1> o_done;*/

  //----------------------------------------

  initial begin : init $readmemh("message.hex", memory, 0, 511); end

  //----------------------------------------

  always_ff @(posedge clock) begin : tick
    if (!i_rstn) begin
      s <= WAIT;
      cursor <= 0;
    end else begin
      data <= memory[cursor];
      if (s == WAIT && i_idle) begin
        s <= SEND;
      end else if (s == SEND && i_cts) begin
        if (cursor == 9'(message_len - 1)) begin
          s <= DONE;
        end else begin
          cursor <= cursor + 1;
        end
      end else if (s == DONE) begin
        // s = state::WAIT;
        cursor <= 0;
      end
    end
  end

  //----------------------------------------

  always_comb begin : tock
    o_data = data;
    o_req = (s == SEND);
    o_done = (s == DONE);
  end
endmodule

//==============================================================================

