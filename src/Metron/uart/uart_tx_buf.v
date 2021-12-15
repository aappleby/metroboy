`timescale 1 ns / 1 ps

//==============================================================================

module rs232_tx(clk, resetn, tx_data_, tx_en_, out_tx, out_tx_busy);
  parameter clocks_per_bit = 4;
  localparam cycle_reg_size = $clog2(clocks_per_bit);

  input clk;
  input resetn;
  input [7:0] tx_data_;
  input tx_en_;
  output reg out_tx;
  output reg out_tx_busy;

  reg[7:0] buffer[0:255];
  reg[7:0] cursorA, cursorA_;
  reg[7:0] cursorB, cursorB_;

  reg[7:0] tx_data;
  reg tx_en;

  reg[8:0] latch, latch_;
  reg data_latched, data_latched_;
  reg [cycle_reg_size-1:0] cycle_count, cycle_count_;
  reg [3:0] bit_count, bit_count_;
  reg transmit_done, transmit_done_;
  reg bit_end;

  assign out_tx = latch[0] | (bit_count == 9);
  assign out_tx_busy = data_latched;

  always @* begin
    latch_ = latch;
    data_latched_ = data_latched;
    transmit_done_ = transmit_done;

    bit_end = (cycle_count == clocks_per_bit - 1);
    cycle_count_ = bit_end ? 0 : cycle_count + 1;
    bit_count_ = bit_count + {3'b000, bit_end};

    if (bit_end) begin
      if (bit_count == 8) begin
        data_latched_ = 0;
      end

      if (bit_count == 9) begin
        transmit_done_ = 1;
      end else begin
        latch_ = { 1'b1, latch[8:1] };
      end
    end

    if (!data_latched_ && tx_en) begin
      latch_ = { tx_data, 1'b0 };
      data_latched_ = 1;
    end

    if (transmit_done_ && data_latched_) begin
      cycle_count_ = 0;
      bit_count_ = 0;
      transmit_done_ = 0;
    end

  end

  always @(posedge clk) begin
    if (!resetn) begin
      latch <= 9'b111111111;
      data_latched <= 0;
      cycle_count <= 0;
      bit_count <= 0;
      transmit_done <= 1;
      tx_data <= 0;
      tx_en <= 0;
      cursorA <= 0;
      cursorB <= 0;
    end else begin
      latch <= latch_;
      data_latched <= data_latched_;
      cycle_count <= cycle_count_;
      bit_count <= bit_count_;
      transmit_done <= transmit_done_;
      tx_data <= tx_data_;
      tx_en <= tx_en_;

      if (tx_en) begin
        buffer[cursorB + 1] <= tx_data;
        cursorB <= cursorB + 1;
      end

      if (transmit_done_) begin
        latch <= buffer[cursorA + 1];
        cursorA <= cursorA + 1;
      end
    end

  end
  
endmodule

//==============================================================================
