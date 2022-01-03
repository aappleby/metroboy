`default_nettype none
`include "blockram_512x8.sv"

//==============================================================================

module uart_hello
(
  input logic clk,
  input logic rst_n,

  input logic i_cts,
  input logic i_idle,

  output logic [7:0] o_data,
  output logic       o_req,
  output logic       o_done
);
  /*verilator public_module*/
  //----------------------------------------

  localparam message_len = 512;
  localparam cursor_bits = $clog2(message_len);

  enum { WAIT, SEND, DONE } state;
  logic[cursor_bits-1:0] cursor;

  logic      mem_i_clk;
  logic[8:0] mem_i_cursor;
  logic[7:0] mem_o_data;

  blockram_512x8 #(.INIT_FILE("obj/message2.hex"))
  mem(mem_i_clk, mem_i_cursor, mem_o_data);

  //----------------------------------------

  task automatic reset();
    state <= WAIT;
    cursor <= 0;
  endtask

  //----------------------------------------
  // this can't be factored out into tick() or icarus sim fails

  always_comb begin
    mem_i_clk    = clk;
    mem_i_cursor = cursor;

    o_data = mem_o_data;
    o_req  = state == SEND;
    o_done = state == DONE;
  end

  //----------------------------------------

  task automatic tock();
    if (state == WAIT && i_idle) begin
      state <= SEND;
    end else if (state == SEND && i_cts) begin
      cursor <= cursor + 1;
      if (cursor == (cursor_bits)'(message_len - 1)) state <= DONE;
    end else if (state == DONE) begin
      state <= WAIT;
      cursor <= 0;
    end
  endtask

  always_ff @(posedge clk, negedge rst_n) if (!rst_n) reset(); else tock();

endmodule

//==============================================================================
