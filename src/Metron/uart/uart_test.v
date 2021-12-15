`timescale 1 ns / 1 ps
`include "rs232_rx.v"
`include "rs232_tx.v"
`include "rs232_hello.v"

//==============================================================================

module rs232_test (
  input  clk,
  input  RS232_Rx,
  output reg RS232_Tx,
  output reg LED0,
  output reg LED1,
  output reg LED2,
  output reg LED3,
  output reg LED4,
  output reg LED5,
  output reg LED6,
  output reg LED7
);
  parameter clocks_per_bit = 12;

  reg resetn;
  reg[2:0] resetn_counter = 0;
  assign resetn = &resetn_counter;
  always @(posedge clk) begin
    if (!resetn) begin
      resetn_counter <= resetn_counter + 1;
    end
  end

  reg [7:0] rx_data;
  reg rx_valid;

  rs232_rx #(clocks_per_bit) serial_in(clk, resetn, RS232_Rx, rx_data, rx_valid);

  always @(posedge clk) begin
    if (rx_valid) begin
      LED0 <= rx_data[0];
      LED1 <= rx_data[1];
      LED2 <= rx_data[2];
      LED3 <= rx_data[3];
      LED4 <= rx_data[4];
      LED5 <= rx_data[5];
      LED6 <= rx_data[6];
      LED7 <= rx_data[7];
    end
  end

  reg [7:0] tx_data;
  reg tx_en;
  reg tx_busy;

  rs232_tx #(clocks_per_bit) serial_out(clk, resetn, tx_data, tx_en, RS232_Tx, tx_busy);

  reg message_sent;
  rs232_hello hello(clk, resetn, tx_busy, tx_data, tx_en, message_sent);

endmodule