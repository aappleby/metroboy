/* verilator lint_off WIDTH */
`default_nettype none

`include "uart_rx.sv"
`include "uart_tx.sv"
`include "uart_hello.sv"



//==============================================================================

module uart_top
#(parameter cycles_per_bit = 3) (
  input logic clk,
  input logic rst_n,

  output logic       o_serial,
  output logic[7:0]  o_data,
  output logic       o_valid,
  output logic       o_done,
  output logic[31:0] o_sum
);
  //----------------------------------------

  /*verilator public_module*/

  logic       hello_i_cts;
  logic       hello_i_idle;
  logic[7:0]  hello_o_data;
  logic       hello_o_req;
  logic       hello_o_done;
  uart_hello hello(clk, rst_n, hello_i_cts, hello_i_idle, hello_o_data, hello_o_req, hello_o_done);

  logic[7:0]  tx_i_data;
  logic       tx_i_req;
  logic       tx_o_serial;
  logic       tx_o_cts;
  logic       tx_o_idle;
  uart_tx #(cycles_per_bit) tx(clk, rst_n, tx_i_data, tx_i_req, tx_o_serial, tx_o_cts, tx_o_idle);

  logic       rx_i_serial;
  logic[7:0]  rx_o_data;
  logic       rx_o_valid;
  logic[31:0] rx_o_sum;
  uart_rx #(cycles_per_bit) rx(clk, rst_n, rx_i_serial, rx_o_data, rx_o_valid, rx_o_sum);

  //----------------------------------------

  always_comb begin : tick
    //hello.tick(rst_n);
    //tx.tick(rst_n);
    //rx.tick(rst_n);

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

  always_ff @(posedge clk, negedge rst_n) begin : tock
    //hello.tock(rst_n);
    //tx.tock(rst_n);
    //rx.tock(rst_n);

    if (!rst_n) begin
    end else begin
    end
  end

  //----------------------------------------
/*
  void dump_header() {
    printf("[top data valid done sum ] ");
    hello.dump_header();
    tx.dump_header();
    rx.dump_header();
  }

  void dump() {
    printf("[    %04x %-5d %-4d %04x] ", o_data, o_valid, o_done, o_sum);
    hello.dump();
    tx.dump();
    rx.dump();
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$var wire 8  top_o_data  o_data $end\n");
    fprintf(d.file, "$var wire 1  top_o_valid o_valid $end\n");
    fprintf(d.file, "$var wire 1  top_o_done  o_done $end\n");
    fprintf(d.file, "$var wire 32 top_o_sum   o_sum $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("top_o_data",  o_data, 8);
    d.set_value("top_o_valid", o_valid, 1);
    d.set_value("top_o_done",  o_done, 1);
    d.set_value("top_o_sum",   o_sum, 32);
    hello.dump_value(d);
    tx.dump_value(d);
    rx.dump_value(d);
  }
*/
endmodule

//==============================================================================
