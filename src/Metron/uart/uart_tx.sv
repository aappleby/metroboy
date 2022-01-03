`ifndef UART_TX_SV
`define UART_TX_SV
`default_nettype none

//==============================================================================

module uart_tx
#(parameter clocks_per_bit = 4)
(
  input logic clk,
  input logic rst_n,

  input logic[7:0] tx_data,
  input logic tx_req,

  output logic SER_TX,
  output logic tx_cts,
  output logic tx_idle
);
  /*verilator public_module*/

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  localparam extra_stop_bits = 7;

  localparam timer_bits = $clog2(clocks_per_bit);
  localparam timer_max  = timer_bits'(clocks_per_bit) - 1'b1;

  localparam bitcount_bits = $clog2(10 + extra_stop_bits);
  localparam bitcount_max = 10 + extra_stop_bits - 1;

  logic[timer_bits-1:0]    tx_cycle;
  logic[bitcount_bits-1:0] tx_bit;
  logic[8:0] tx_buf;

  //----------------------------------------------------------------------------

  task automatic reset();
    tx_cycle <= '0;
    tx_bit   <= '0;
    tx_buf   <= '1;
  endtask

  task automatic tock();
    if (tx_bit <= extra_stop_bits && tx_cycle == 0 && tx_req) begin
      // Transmit start
      tx_cycle <= timer_max;
      tx_bit   <= bitcount_max;
      tx_buf   <= { tx_data, 1'b0 };
    end else if (tx_cycle != 0) begin
      // Bit delay
      tx_cycle <= tx_cycle - 1;
      tx_bit   <= tx_bit;
      tx_buf   <= tx_buf;
    end else if (tx_bit != 0) begin
      // Bit delay done, switch to next bit.
      tx_cycle <= timer_max;
      tx_bit   <= tx_bit - 1;
      tx_buf   <= (tx_buf >> 1) | 9'h100;
    end

    // having combi logic as blocking assignment here doesn't work in verilator

  endtask

  // having combi logic in tick doesn't work in icarus
  always_comb begin
    SER_TX  = tx_buf[0];
    tx_cts  = ((tx_bit == extra_stop_bits) && (tx_cycle == 0)) || (tx_bit < extra_stop_bits);
    tx_idle = (tx_bit == 0) && (tx_cycle == 0);

    if (!rst_n) begin
      tx_cts = 0;
      tx_idle = 0;
    end
  end
  
  always @(posedge clk, negedge rst_n) if (!rst_n) reset(); else tock();

endmodule

//==============================================================================

`endif
