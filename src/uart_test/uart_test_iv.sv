`ifndef UART_TEST_SV
`define UART_TEST_SV
`default_nettype none
`timescale 1 ns / 1 ns

`include "uart_top.h.sv"

//==============================================================================

module uart_test;

  /*verilator public_module*/

  // 12 mhz clock
  logic clk;
  logic clken;
  logic rst_n;
  logic[63:0] timestamp = 0;

  always #5 clk = ~clk & clken;

  always @(posedge clk) begin
    timestamp <= timestamp + 1;
  end

  wire ser_tx;
  logic[7:0] out_data;
  logic out_valid;
  logic out_done;
  logic[31:0] out_sum;
  logic[7:0] out_onehot;

  uart_top #(.cycles_per_bit(3)) top
  (
    clk,
    rst_n,
    ser_tx,
    out_data,
    out_valid,
    out_done,
    out_sum,
    out_onehot
  );

  always begin
    wait (!out_valid);
    wait (out_valid);
    $write("%c", out_data);
  end

  initial begin
    $write("Icarus simulation:\n");
    $write("================================================================================\n");

    $dumpfile("uart_test_iv.vcd");
    $dumpvars(0, uart_test);

    clken = 0;
    clk = 0;
    rst_n = 1;
    #5;
    rst_n = 0;
    #5;
    clken = 1;
    clk = 1;
    #5;
    rst_n = 1;

    #200;
    wait (top.tx.o_idle);
    #5

    $write("\n");
    $write("================================================================================\n");
    if (out_sum == 32'h0000b764) $write("All tests pass.\n");
    $finish();
  end

endmodule

//==============================================================================

`endif
