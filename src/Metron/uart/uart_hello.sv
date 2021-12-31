`ifndef UART_HELLO_SV
`define UART_HELLO_SV
`default_nettype none

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

  localparam message_len = 14;
  logic[7:0] message[message_len];
  logic[3:0] message_cursor;
  logic      message_done;

  initial begin
    $readmemh("message", message, 0, message_len - 1);
  end

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      message_cursor <= message_len;
      message_done <= 0;
      tx_req <= 0;
    end else begin
      if (tx_idle && !message_done) begin
        message_cursor <= 0;
        tx_req <= 1;
      end else if (tx_cts && !tx_req && message_cursor < message_len) begin
        message_cursor <= message_cursor + 1;
        tx_req <= message_cursor < message_len - 1;
      end
      if (tx_req && !tx_cts) tx_req <= 0;
    end
  end

  always_comb begin
    tx_data = message_cursor < message_len ? message[message_cursor] : 8'hFF;
  end

endmodule

//==============================================================================

`endif
