`ifndef UART_TOP_SV
`define UART_TOP_SV
`default_nettype none

`include "uart_rx.sv"
`include "uart_tx.sv"
`include "uart_hello.sv"


//==============================================================================

module uart_top
#(parameter clocks_per_bit = 3)
(
  input logic clk,
  input logic rst_n,

  output logic ser_tx,
  output logic[7:0] out_data,
  output logic out_valid
);

  logic[7:0]  tx_data;
  logic       tx_req;
  logic       tx_cts;
  logic       tx_idle;

  uart_hello hello(clk, rst_n, tx_cts, tx_idle, tx_data, tx_req);

  uart_tx    #(.clocks_per_bit(clocks_per_bit)) tx   (clk, rst_n, tx_data, tx_req, ser_tx, tx_cts, tx_idle);
  uart_rx    #(.clocks_per_bit(clocks_per_bit)) rx   (clk, rst_n, ser_tx, out_data, out_valid);

endmodule

//==============================================================================

`endif
