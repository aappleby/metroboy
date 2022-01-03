`ifndef UART_RX_SV
`define UART_RX_SV
`default_nettype none

//==============================================================================

module uart_rx
#(parameter clocks_per_bit = 4)
(
  input logic clk,
  input logic rst_n,
  input logic SER_TX,

  output logic[7:0] out_data,
  output logic out_valid,
  output logic[31:0] out_sum
);
  /*verilator public_module*/
  localparam timer_bits = $clog2(clocks_per_bit);
  logic[timer_bits-1:0] rx_cycle;
  logic [3:0]  rx_bit;
  logic [31:0] sum;

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      rx_cycle <= 0;
      rx_bit <= 0;
      out_data <= 0;
      sum <= 0;
    end else begin
      if (rx_cycle != 0) begin
        rx_cycle <= rx_cycle - 1;
      end
      else if (rx_bit != 0) begin
        rx_cycle <= timer_bits'(clocks_per_bit - 1);
        rx_bit <= rx_bit - 1;
        out_data <= {SER_TX, out_data[7:1]};
        if (rx_bit == 2) sum <= sum + {24'b0, SER_TX, out_data[7:1]};
      end
      else if (SER_TX == 0) begin
        rx_cycle <= timer_bits'(clocks_per_bit - 1);
        rx_bit <= 9;
      end
    end
  end

  always_comb begin
    out_valid = rx_bit == 1;
    out_sum = sum;
  end

endmodule

//==============================================================================

`endif
