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

  output logic       top_o_sio,
  output logic[7:0]  top_o_data,
  output logic       top_o_valid,
  output logic       top_o_done,
  output logic[31:0] top_o_sum
);
  /*verilator public_module*/

  logic       hello_i_cts;
  logic       hello_i_idle;
  logic[7:0]  hello_o_data;
  logic       hello_o_req;
  logic       hello_o_done;
  uart_hello hello(clk, rst_n, hello_i_cts, hello_i_idle, hello_o_data, hello_o_req, hello_o_done);

  logic[7:0]  tx_i_data;
  logic       tx_i_req;
  logic       tx_o_sio;
  logic       tx_o_cts;
  logic       tx_o_idle;
  uart_tx #(.clocks_per_bit(clocks_per_bit)) tx   (clk, rst_n, tx_i_data, tx_i_req, tx_o_sio, tx_o_cts, tx_o_idle);

  logic       rx_i_sio;
  logic[7:0]  rx_o_data;
  logic       rx_o_valid;
  logic[31:0] rx_o_sum;
  uart_rx    #(.clocks_per_bit(clocks_per_bit)) rx   (clk, rst_n, rx_i_sio, rx_o_data, rx_o_valid, rx_o_sum);

  always_comb begin
    hello_i_cts  = tx_o_cts;
    hello_i_idle = tx_o_idle;

    tx_i_data = hello_o_data;
    tx_i_req  = hello_o_req;

    rx_i_sio = tx_o_sio;

    top_o_sio   = tx_o_sio;
    top_o_data  = rx_o_data;
    top_o_valid = rx_o_valid;
    top_o_done  = hello_o_done;
    top_o_sum   = rx_o_sum;
  end

endmodule

//==============================================================================

`endif
