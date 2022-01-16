/* verilator lint_off WIDTH */
`default_nettype none

//==============================================================================

module uart_hello (
  input logic clk,
  input logic rst_n,

  input logic i_cts,
  input logic i_idle,

  output logic [7:0] o_data,
  output logic       o_req,
  output logic       o_done
);
  //----------------------------------------
  /*verilator public_module*/

  //localparam message_len = 7;
  localparam message_len = 512;
  localparam cursor_bits = $clog2(message_len);

  typedef enum { WAIT, SEND, DONE } e_state;
  e_state state;
  logic[cursor_bits-1:0] cursor;

  //----------------------------------------

  logic[7:0] memory[512];
  logic[7:0] data;

  initial begin
    $readmemh("obj/message.hex", memory, 0, 511);
  end

  //----------------------------------------

  always_comb begin : tick
    o_data = data;
    o_req  = state == SEND;
    o_done = state == DONE;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tock
    if (!rst_n) begin
      state <= WAIT;
      cursor <= 0;
    end else begin
      data <= memory[cursor];
      if (state == WAIT && i_idle) begin
        state <= SEND;
      end else if (state == SEND && i_cts) begin
        if (cursor == (cursor_bits)'(message_len - 1)) state <= DONE;
        cursor <= cursor + 1;
      end else if (state == DONE) begin
        state <= WAIT;
        cursor <= 0;
      end
    end
  end

  //----------------------------------------

endmodule

//==============================================================================
