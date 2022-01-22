//--------------------------------------------------------------------------------
// MODULE:       uart_top
// MODULEPARAMS: cycles_per_bit, 
// INPUTS:       
// OUTPUTS:      o_serial, o_data, o_valid, o_done, o_sum, 
// LOCALPARAMS:  
// FIELDS:       
// SUBMODULES:   hello, tx, rx, 

/* verilator lint_off WIDTH */
`default_nettype none
`include "metron.sv"
`include "uart_rx.sv"
`include "uart_tx.sv"
`include "uart_hello.sv"

//==============================================================================


module uart_top
#(parameter int cycles_per_bit = 3)
(clk, rst_n, o_serial, o_data, o_valid, o_done, o_sum);
  input logic clk;
  input logic rst_n;

  output logic  o_serial;
  output logic[7:0]  o_data;
  output logic  o_valid;
  output logic  o_done;
  output logic[31:0] o_sum;

  //----------------------------------------
  /*verilator public_module*/

  logic hello_i_cts;
  logic hello_i_idle;
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

  initial begin
    /* hello.init() */;
    /* tx.init() */;
    /* rx.init() */;
  end

  //----------------------------------------

  always_comb begin
    /* hello.tick(rst_n) */;
    /* tx.tick(rst_n) */;
    /* rx.tick(rst_n) */;

    o_serial = tx_o_serial;
    o_data   = rx_o_data;
    o_valid  = rx_o_valid;
    o_done   = hello_o_done;
    o_sum    = rx_o_sum;

    hello_i_cts  = tx_o_cts;
    hello_i_idle = tx_o_idle;
    tx_i_data    = hello_o_data;
    tx_i_req     = hello_o_req;
    rx_i_serial  = tx_o_serial;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin
    /* hello.tock(rst_n) */;
    /* tx.tock(rst_n) */;
    /* rx.tock(rst_n) */;

    if (!rst_n) begin
    end else begin
    end
  end
endmodule
