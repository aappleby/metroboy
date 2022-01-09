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

  typedef enum { WAIT, SEND, DONE } e_state;
  e_state state;
  logic[cursor_bits-1:0] cursor;

  //----------------------------------------

  logic[8:0] mem_i_addr;
  logic[7:0] mem_o_data;
  blockram_512x8 mem(clk, rst_n, mem_i_addr, mem_o_data);

  //----------------------------------------

  task automatic reset();
    state <= WAIT;
    cursor <= 0;
  endtask

  //----------------------------------------
  // this can't be factored out into tick() or icarus sim fails

  always_comb begin : tick
    o_data = mem_o_data;
    o_req  = state == SEND;
    o_done = state == DONE;
    mem_i_addr = 9'(cursor);
  end

  //----------------------------------------

  task automatic tock();
    if (state == WAIT && i_idle) begin
      state <= SEND;
    end else if (state == SEND && i_cts) begin
      if (cursor == (cursor_bits)'(message_len - 1)) state <= DONE;
      cursor <= cursor + 1;
    end else if (state == DONE) begin
      state <= WAIT;
      cursor <= 0;
    end
  endtask

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else        tock();
  end

endmodule

//==============================================================================
