`timescale 1 ns / 1 ps

//==============================================================================

module uart_tx
#(parameter clocks_per_bit = 4)
(
  input logic clk,
  input logic rst_n,
  input logic[7:0] tx_data,
  input logic tx_en,
  output logic out_tx,
  output logic out_tx_busy
);

  logic[8:0] latch;
  logic[$clog2(clocks_per_bit)-1:0] cycle_count;
  logic[3:0] bit_count;

  always_comb begin
    out_tx = latch[0];
  end

  task reset();
    latch <= 9'b111111111;
    cycle_count <= 0;
    bit_count <= 0;
  endtask

  task tock();
    if (cycle_count != 0) begin
      cycle_count <= cycle_count - 1;
    end
    else if (bit_count != 0) begin
      latch <= (latch >> 1) | 9'b100000000;
      bit_count <= bit_count - 1;
      cycle_count <= clocks_per_bit - 1;
    end
    else if (tx_en) begin
      latch <= { tx_data, 1'b0 };
      bit_count <= 9;
      cycle_count <= clocks_per_bit - 1;
      out_tx_busy <= 1;
    end
    else begin
      out_tx_busy <= 0;
    end
  endtask

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) reset();
    else tock();
  end
  
endmodule

//==============================================================================
