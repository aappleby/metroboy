`timescale 1 ns / 1 ps
`include "uart_top.sv"

//==============================================================================

module uart_bench;

  logic clk;
  always #41.6666667 clk = ~clk; // 12 mhz clock

  wire[4:0] leds;

  localparam clocks_per_bit = 4;
  uart_top #(clocks_per_bit) dut(clk, leds[0], leds[1], leds[2], leds[3], leds[4]);

  logic[31:0] timer = 0;
  always @(posedge clk) begin
    timer <= timer + 1;
    if (dut.rx_valid) begin
      $write("%d %c\n", timer, dut.rx_data);
      $fflush();
    end
  end

  initial begin
    clk = 0;
    $dumpfile("uart_bench.vcd");
    $dumpvars();

    $write("timer %d\n", timer);
    wait(dut.message_sent);
    $write("timer %d\n", timer);

    #10000;
    $finish;
  end

endmodule
