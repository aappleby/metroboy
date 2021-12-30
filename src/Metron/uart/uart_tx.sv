`ifndef UART_TX_SV
`define UART_TX_SV
`default_nettype none

//==============================================================================

module uart_tx
#(parameter clocks_per_bit = 4)
(
  input logic ser_clk,
  input logic rst_n,

  input logic[7:0] tx_data,
  input logic tx_req,

  output logic SER_TX,
  output logic tx_cts,
  output logic tx_idle
);

  localparam timer_bits = $clog2(clocks_per_bit);

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  localparam extra_stop_bits = 7;

  logic[timer_bits-1:0] tx_cycle_;
  logic[$clog2(10 + extra_stop_bits)-1:0]  tx_bit_;
  logic[8:0]  tx_buf_;

  //----------------------------------------------------------------------------

  always @(posedge ser_clk, negedge rst_n) begin
    logic[timer_bits-1:0] _tx_cycle;
    logic[$clog2(10 + extra_stop_bits)-1:0] _tx_bit;
    logic[8:0] _tx_buf;

    if (!rst_n) begin
      _tx_cycle = '0;
      _tx_bit   = '0;
      _tx_buf   = '1;
    end else begin
      if (tx_bit_ <= extra_stop_bits && tx_cycle_ <= 1 && tx_req) begin
        // Transmit start
        _tx_cycle = timer_bits'(clocks_per_bit) - 1;
        _tx_bit   = 10 + extra_stop_bits - 1;
        _tx_buf   = { tx_data, 1'b0 };
      end else if (|tx_cycle_) begin
        // Bit delay
        _tx_cycle = tx_cycle_ - 1;
        _tx_bit   = tx_bit_;
        _tx_buf   = tx_buf_;
      end else if (|tx_bit_) begin
        // Bit delay done, switch to next bit.
        _tx_cycle = timer_bits'(clocks_per_bit) - 1'b1;
        _tx_bit   = tx_bit_ - 1;
        _tx_buf   = (tx_buf_ >> 1) | 9'h100;
      end
    end

    tx_cycle_ <= _tx_cycle;
    tx_bit_   <= _tx_bit;
    tx_buf_   <= _tx_buf;
  end

  always_comb begin
    SER_TX  = tx_buf_[0];
    tx_cts  = ((tx_bit_ <= extra_stop_bits) && (tx_cycle_ <= 1)) || (tx_bit_ < extra_stop_bits);
    tx_idle = (tx_bit_ == 0) && (tx_cycle_ <= 1);
  end
  
endmodule

//==============================================================================

`endif
