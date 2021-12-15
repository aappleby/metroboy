3
`timescale 1 ns / 1 ps

//==============================================================================

module rs232_rx(clk, resetn, in_serial, out_result, out_result_valid);
  parameter clocks_per_bit = 4;
  localparam cycle_reg_size = $clog2(clocks_per_bit);

  input clk;
  input resetn;
  input in_serial;
  output reg [7:0] out_result;
  output reg out_result_valid;

  reg [3:0] window, window_;
  reg [cycle_reg_size - 1:0] cycle_count, cycle_count_;
  reg [3:0] bit_count, bit_count_;
  reg [7:0] result, result_;
  reg result_valid, result_valid_;

  assign out_result = result_;
  assign out_result_valid = result_valid_;

  always @* begin
    window_ = {window[2:0], in_serial};
    cycle_count_ = cycle_count + 1;
    bit_count_ = bit_count;
    result_ = result;
    result_valid_ = 0;

    if (bit_count == 9) begin
      if (window_ == 0) begin
        cycle_count_ = 0;
        bit_count_ = 0;
      end
    end else begin
      if (cycle_count == (clocks_per_bit - 1)) begin
        cycle_count_ = 0;
        bit_count_ = bit_count + 1;
        result_ = {window_[2], result[7:1]};
        result_valid_ = bit_count == 7;
      end
    end
  end

  always @(posedge clk) begin
    if (!resetn) begin
      window <= 4'b1111;
      cycle_count <= 0;
      bit_count <= 9;
      result <= 0;
      result_valid <= 0;
    end else begin
      window <= window_;
      cycle_count <= cycle_count_;
      bit_count <= bit_count_;
      result <= result_;
      result_valid <= result_valid_;
    end
  end

endmodule

//==============================================================================
