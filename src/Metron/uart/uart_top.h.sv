`default_nettype none


`include "../metron_tools.h.sv"
`include "uart_rx.h.sv"
`include "uart_tx.h.sv"
`include "uart_hello.h.sv"

//==============================================================================
/* verilator lint_off WIDTH */


module uart_top
#(parameter int cycles_per_bit = 3)
(
input logic clk,
input logic rst_n,

output logic o_serial,
output logic[7:0] o_data,
output logic o_valid,
output logic o_done,
output logic[31:0] o_sum
); 

  bool hello_i_cts;
  bool hello_i_idle;
  logic[7:0] hello_o_data;
  logic hello_o_req;
  logic hello_o_done;
  uart_hello hello(clk, rst_n, hello_i_cts, hello_i_idle, hello_o_data, hello_o_req, hello_o_done);
  
  logic[7:0] tx_i_data;
  logic tx_i_req;
  logic tx_o_serial;
  logic tx_o_cts;
  logic tx_o_idle;
  uart_tx #(cycles_per_bit) tx(clk, rst_n, tx_i_data, tx_i_req, tx_o_serial, tx_o_cts, tx_o_idle);
  
  logic rx_i_serial;
  logic[7:0] rx_o_data;
  logic rx_o_valid;
  logic[31:0] rx_o_sum;
  uart_rx #(cycles_per_bit) rx(clk, rst_n, rx_i_serial, rx_o_data, rx_o_valid, rx_o_sum);
  

  
  
  
  
  

  //----------------------------------------

  initial begin : init
    $write("uart_top.init()\n");

    /*hello.init()*/;
    /*tx.init()*/;
    /*rx.init()*/;

    o_serial = 0;
    o_data = 0;
    o_valid = 0;
    o_done = 0;
    o_sum = 0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : tick
    /*hello.tick(rst_n, tx.o_cts, tx.o_idle)*/;
    /*tx.tick(rst_n, hello.o_data, hello.o_req)*/;
    /*rx.tick(rst_n, tx.o_serial)*/;
  end

  //----------------------------------------

  always_comb begin : tock
    /*hello.tock(rst_n)*/;
    /*tx.tock(rst_n)*/;
    /*rx.tock(rst_n)*/;

    o_serial = tx_o_serial;
    o_data = rx_o_data;
    o_valid = rx_o_valid;
    o_done = hello_o_done && tx_o_idle;
    o_sum = rx_o_sum;
  end
endmodule

//==============================================================================
