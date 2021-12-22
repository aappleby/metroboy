`ifndef UART_BENCH_SV
`define UART_BENCH_SV
`default_nettype none
`timescale 1 ns / 1 ps

`include "uart_top.sv"

// 1250 clocks per bit gives 9600 bps
//localparam clock_rate = 12000000;
//localparam clocks_per_bit = 4;

//==============================================================================

module uart_bench;

  // 12 mhz clock
  logic clk = 0;
  always #1 clk = ~clk;

  logic[2:0] clk_div;
  logic[3:0] rst_counter = 15;
  always @(posedge clk) begin
    clk_div <= clk_div + 1;
    if (rst_counter) rst_counter <= rst_counter - 1;
  end
  wire rst_n = rst_counter == 0;

  wire ser_tx;
  logic[7:0] leds;

  uart_top dut
  (
    clk,
    clk_div[1],
    rst_n,
    ser_tx,
    leds
  );

  //uart_top #(clock_rate, clocks_per_bit) dut(clk, rst_n, leds);

  initial begin
    clk_div = 0;
    //counter = 0;
    $dumpfile("uart_bench.vcd");
    $dumpvars(0, uart_bench);

    //wait(counter == 6);
    #2000;

    $finish;
  end

  /*
  logic[31:0] counter;
  always @(posedge clk) begin
    if (dut.rx_valid) begin
      //counter <= counter + 1;
      $write("%d %c\n", counter, dut.rx_data);
      $fflush();
    end
  end
  */

endmodule

//==============================================================================

`endif
