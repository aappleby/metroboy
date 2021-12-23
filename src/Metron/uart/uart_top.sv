`ifndef UART_TOP_SV
`define UART_TOP_SV
`default_nettype none

`include "uart_rx.sv"
`include "uart_tx.sv"
`include "uart_hello.sv"

//==============================================================================

module uart_top
(
  input logic ser_clk_4x,
  input logic ser_clk_1x,
  input logic rst_n,
  output logic SER_TX,
  output logic [7:0] LEDS
);

  /*
  wire tx_en = 1;
  wire [7:0] tx_data = 35;
  wire tx_busy;

  uart_tx #(1) serial_tx(clk, rst_n, tx_data, tx_en, out_tx, tx_busy);
  */

  /*
  wire [7:0] tx_data;
  wire [7:0] rx_data;
  wire rx_valid;

  wire loopback;
  wire tx_en;
  wire tx_busy;
  wire message_sent;

  uart_tx #(clocks_per_bit) serial_tx(CLK, RST_N, tx_data, tx_en, loopback, tx_busy);
  uart_rx #(clocks_per_bit) serial_rx(CLK, RST_N, loopback, rx_data, rx_valid);

  uart_hello hello(CLK, RST_N, tx_busy, tx_data, tx_en, message_sent);

  assign LEDS = tx_data;
  */

  localparam clocks_per_bit = 4;

  //----------------------------------------

  logic ser_cts;
  logic ser_idle;
  logic[4:0] ser_count;
  logic[7:0] ser_buf;

  logic[7:0] ser_tx_data;
  logic ser_tx_en;

  always @(posedge ser_clk_1x, negedge rst_n) begin
    if (!rst_n) begin
      ser_cts   <= 1;
      ser_idle  <= 1;
      ser_count <= 0;
      ser_buf   <= 8'hFF;
      SER_TX    <= 1;
    end
    else if (ser_cts && ser_tx_en) begin
      ser_cts   <= 0;
      ser_idle  <= 0;
      ser_count <= 16;
      ser_buf   <= ser_tx_data;
      SER_TX    <= 0;
    end
    else if (ser_count) begin
      if (ser_count == 9) ser_cts <= 1;
      if (ser_count == 1) ser_idle <= 1;
      ser_count <= ser_count - 1;
      ser_buf   <= 8'h80 | (ser_buf >> 1);
      SER_TX    <= ser_buf[0];
    end
  end

  //----------------------------------------

  localparam message_len = 8;
  logic[7:0] message[0:7];
  logic[3:0] message_cursor = 0;

  initial begin
    message[0] = 8'h48;
    message[1] = 8'h65;
    message[2] = 8'h6c;
    message[3] = 8'h6c;
    message[4] = 8'h6f;
    message[5] = 8'h21;
    message[6] = 8'h21;
    message[7] = 8'h0A;
  end

  // FIXME this doesn't work at 4x
  always_ff @(posedge ser_clk_1x, negedge rst_n) begin
    if (!rst_n) begin
      ser_tx_en <= 0;
      message_cursor <= message_len;
    end else begin

      if (ser_idle) begin
        ser_tx_en <= 1;
        message_cursor <= 0;
      end else begin
        if (ser_cts && !ser_tx_en) begin
          if (message_cursor < message_len - 1) begin
            ser_tx_en <= 1;
            message_cursor <= message_cursor + 1;
          end else begin
            ser_tx_en <= 0;
            if (message_cursor < message_len) message_cursor <= message_cursor + 1;
          end
        end else if (ser_tx_en && !ser_cts) begin
          ser_tx_en <= 0;
        end
      end
    end
  end

  always_comb begin
    if (message_cursor < message_len) ser_tx_data = message[message_cursor];
    else ser_tx_data = 0;
  end

  //----------------------------------------

  logic [1:0] rx_cycle_count;
  logic [3:0] rx_bit_count;
  logic[7:0] out_data;
  logic out_valid;

  always @(posedge ser_clk_4x, negedge rst_n) begin
    if (!rst_n) begin
      rx_cycle_count <= clocks_per_bit - 1;
      rx_bit_count <= 9;
      out_data <= 8'hFF;
      out_valid <= 0;
    end
    else begin
      out_valid <= 0;

      if (rx_cycle_count != clocks_per_bit - 1) begin
        rx_cycle_count <= rx_cycle_count + 1;
      end
      else if (rx_bit_count != 9) begin
        out_data <= {SER_TX, out_data[7:1]};
        if (rx_bit_count == 7) out_valid <= 1;
        rx_cycle_count <= 0;
        rx_bit_count <= rx_bit_count + 1;
      end
      else if (SER_TX == 0) begin
        out_data <= {SER_TX, out_data[7:1]};
        rx_cycle_count <= 0;
        rx_bit_count <= 0;
      end
    end
  end

  assign LEDS   = out_valid ? out_data : 0;

endmodule

//==============================================================================

`endif
