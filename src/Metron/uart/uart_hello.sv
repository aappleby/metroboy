`ifndef UART_HELLO_SV
`define UART_HELLO_SV
`default_nettype none

`include "blockram_512x8.sv"

//==============================================================================

module uart_hello
(
  input logic clk,
  input logic rst_n,

  input logic tx_cts,
  input logic tx_idle,

  output logic [7:0] tx_data,
  output logic tx_req
);

  enum { WAIT_FOR_IDLE, WAIT_FOR_CTS, WAIT_FOR_ACK } state;

  localparam message_len = 512;
  logic[8:0] message_cursor;
  logic      message_done;

  task reset();
    message_cursor <= 0;
    message_done <= 0;
    tx_req <= 0;
    state <= WAIT_FOR_IDLE;
  endtask

  task wait_for_idle();
    message_cursor <= 0;
    message_done <= 0;
    tx_req <= 1;
    state <= WAIT_FOR_ACK;
  endtask

  task wait_for_cts();
    tx_req <= 1;
    state <= WAIT_FOR_ACK;
  endtask

  task wait_for_ack();
    if (message_cursor == 9'(message_len - 1)) begin
      message_cursor <= 0;
      message_done <= 1;
      tx_req <= 0;
      state <= WAIT_FOR_IDLE;
    end else begin
      message_cursor <= message_cursor + 1;
      tx_req <= 0;
      state <= WAIT_FOR_CTS;
    end
  endtask

  task tock();
    case(state)
      WAIT_FOR_IDLE: if (tx_idle) wait_for_idle();
      WAIT_FOR_CTS:  if (tx_cts)  wait_for_cts();
      WAIT_FOR_ACK:  if (!tx_cts) wait_for_ack();
    endcase
  endtask

  always_ff @(posedge clk, negedge rst_n) if (!rst_n) reset(); else tock();

  logic       mem_read_en;
  logic[8:0]  mem_read_addr;
  logic[7:0]  mem_read_data;

  logic       mem_write_en   = '0;
  logic[8:0]  mem_write_addr = '0;
  logic[7:0]  mem_write_data = '0;

  blockram_512x8 #(.INIT_FILE("obj/message2.hex"))
  mem
  (
    clk,
    mem_read_en,
    mem_read_addr,
    mem_read_data,
    mem_write_en,
    mem_write_addr,
    mem_write_data
  );

  always_comb begin
    mem_read_en = 1;
    mem_read_addr = message_cursor;
    tx_data = mem_read_data;
  end

endmodule

//==============================================================================

`endif
