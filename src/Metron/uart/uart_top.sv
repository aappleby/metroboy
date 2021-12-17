`include "uart_rx.sv"
`include "uart_tx.sv"
//`include "uart_tx_buf.v"
`include "uart_hello.sv"

//==============================================================================

module uart_top (
  input  CLK,
  output wire D1,
  output wire D2,
  output wire D3,
  output wire D4,
  output wire D5
);
  parameter clocks_per_bit = 12;

  reg[2:0] reset_counter = 3'b111;
  wire resetn = !(|reset_counter);

  wire [7:0] tx_data;
  wire [7:0] rx_data;

  logic[7:0] leds;

  wire uart_tx;
  wire uart_rx;
  wire rx_valid;
  wire tx_en;
  wire tx_busy;
  wire message_sent;

  assign uart_rx = uart_tx;

  uart_rx #(clocks_per_bit) serial_in (CLK, resetn, uart_rx, rx_data, rx_valid);
  uart_tx #(clocks_per_bit) serial_out(CLK, resetn, tx_data, tx_en, uart_tx, tx_busy);

  uart_hello hello(CLK, resetn, tx_busy, tx_data, tx_en, message_sent);

  always @(posedge CLK) begin
    if (reset_counter != 0) reset_counter <= reset_counter - 1;
    if (rx_valid) leds <= rx_data;
  end

  assign D1 = leds[0];
  assign D2 = leds[1];
  assign D3 = leds[2];
  assign D4 = leds[3];
  assign D5 = leds[4];

endmodule

