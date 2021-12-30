`ifndef UART_TOP_SV
`define UART_TOP_SV
`default_nettype none

`include "uart_rx.sv"
`include "uart_tx.sv"
`include "uart_hello.sv"

//==============================================================================

module uart_top
#(parameter clocks_per_bit = 4)
(
  input logic clk,
  input logic rst_n,
  output logic SER_TX,
  output logic [7:0] LEDS
);

  logic[7:0]  tx_data;
  logic       tx_req;
  logic       tx_cts;
  logic       tx_idle;
  logic[7:0]  out_data;
  logic       out_valid;

  uart_hello hello(clk, rst_n, tx_cts, tx_idle, tx_data, tx_req);

  uart_tx    #(.clocks_per_bit(clocks_per_bit)) tx   (clk, rst_n, tx_data, tx_req, SER_TX, tx_cts, tx_idle);
  uart_rx    #(.clocks_per_bit(clocks_per_bit)) rx   (clk, rst_n, SER_TX, out_data, out_valid);

  always_comb begin
    LEDS = out_valid ? out_data : 0;
  end

endmodule

//==============================================================================

`endif
