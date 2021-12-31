`ifndef UART_BENCH_SV
`define UART_BENCH_SV
`default_nettype none
`timescale 1 ns / 1 ns

`include "uart_top.sv"

//==============================================================================

module uart_bench;

  // 12 mhz clock
  logic clk;
  always #100 clk = ~clk;

  logic[63:0] timestamp = 0;

  logic[3:0] rst_counter;
  always @(posedge clk) begin
    timestamp <= timestamp + 1;
    if (rst_counter) rst_counter <= rst_counter - 1;
  end
  wire rst_n = rst_counter == 0;

  wire ser_tx;
  logic[7:0] leds;

  uart_top #(.clocks_per_bit(3)) dut
  (
    clk,
    rst_n,
    ser_tx,
    leds
  );

  initial begin
    $dumpfile("uart_bench.vcd");
    $dumpvars(0, uart_bench);

    clk = 0;
    rst_counter = 15;
  end

  int reps = 0;

  always begin
    wait (!dut.out_valid);
    wait (dut.out_valid);

    if (dut.out_data == 8'h0A) begin
      $write("%8d \\n\n", timestamp);
    end else begin
      $write("%8d %c\n", timestamp, dut.out_data);
    end


    if (dut.out_data == 8'h0A) begin
      reps++;
      if (reps == 2) begin
        $write("\n");
        $finish();
      end
    end
  end

endmodule

//==============================================================================

`endif
