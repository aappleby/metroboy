`ifndef UART_TEST_SV
`define UART_TEST_SV
`default_nettype none
`timescale 1 ns / 1 ns

`include "uart_top.sv"

//==============================================================================

module uart_test;

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
  logic[7:0] out_data;
  logic out_valid;

  uart_top #(.clocks_per_bit(3)) dut
  (
    clk,
    rst_n,
    ser_tx,
    out_data,
    out_valid
  );

  initial begin
    $dumpfile("uart_test.vcd");
    $dumpvars(0, uart_test);

    $write("Icarus simulation:\n");
    clk = 0;
    rst_counter = 15;

    //#220000

    //$write("stuck!\n");
    //$finish();
  end

  int reps = 0;

  always begin
    wait (!out_valid);
    wait (out_valid);

    /*
    if (out_data == 8'h0A) begin
      $write("%8d \\n\n", timestamp);
    end else begin
      $write("%8d %c\n", timestamp, out_data);
    end
    */
    $write("%c", out_data);

    if (out_data == 8'h0A) begin
      reps++;
      if (reps == 2) begin
        $finish();
      end
    end
  end

endmodule

//==============================================================================

`endif
