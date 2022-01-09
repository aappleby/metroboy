`ifndef UART_TEST_SV
`define UART_TEST_SV
`default_nettype none
`timescale 1 ns / 1 ns

`include "uart_top.sv"

//==============================================================================

module uart_test;

  // 12 mhz clock
  logic clk;
  logic rst_n;
  logic[63:0] timestamp = 0;

  always #5 clk = ~clk & rst_n;

  always @(posedge clk) begin
    timestamp <= timestamp + 1;
  end

  wire ser_tx;
  logic[7:0] out_data;
  logic out_valid;
  logic out_done;
  logic[31:0] out_sum;

  uart_top #(.cycles_per_bit(3)) top
  (
    clk,
    rst_n,
    ser_tx,
    out_data,
    out_valid,
    out_done,
    out_sum
  );

  initial begin
    $readmemh("obj/message.hex", top.hello.mem.memory);
    $dumpfile("uart_test.vcd");
    $dumpvars(0, uart_test);
  end

  always begin
    wait (!out_valid);
    wait (out_valid);
    $write("%c", out_data);
  end

  always begin
    $write("\n");
    $write("Icarus simulation:\n");
    $write("================================================================================\n");
    clk = 0;
    rst_n = 0;
    #40;
    rst_n = 1;

    #200;
    wait (top.tx.o_idle);

    $write("\n");
    $write("================================================================================\n");
    $write("cycle = %-8d\n", timestamp);
    $write("checksum = %08x %s\n", out_sum, out_sum == 32'h0000b989 ? "ok" : "fail" );
    $write("\n");
    $finish();
  end

endmodule

//==============================================================================

`endif
