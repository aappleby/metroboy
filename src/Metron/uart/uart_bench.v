`timescale 1 ns / 1 ps
`include "rs232_test.v"

//==============================================================================

module rs232_bench;

  localparam clocks_per_bit = 12;

  reg clk = 1;
  reg tx;
  reg rx;
  reg led0, led1, led2, led3, led4, led5, led6, led7;

  rs232_test #(clocks_per_bit) dut(clk, rx, tx,
      led0, led1, led2, led3, led4, led5, led6, led7);

  //============================================================================

  // 12 mhz clock
  always #41.6666667 clk = ~clk;

  // loopback rx/tx
  assign rx = tx;

  initial begin
    $dumpfile("rs232_bench.vcd");
    $dumpvars();

    wait(dut.message_sent);
    repeat (100) @(posedge clk);

    $finish;
  end

  always @(posedge clk) begin
    if (dut.rx_valid) begin
      $write("%c", dut.rx_data);
      $fflush();
    end
  end


endmodule
