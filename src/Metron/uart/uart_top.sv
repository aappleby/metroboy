`ifndef UART_TOP_SV
`define UART_TOP_SV
`default_nettype none

`include "uart_rx.sv"
`include "uart_tx.sv"
`include "uart_hello.sv"

//==============================================================================

module uart_top
#(parameter clocks_per_bit = 4)
(
  input logic ser_clk,
  input logic rst_n,
  output logic SER_TX,
  output logic [7:0] LEDS
);


  localparam timer_bits = $clog2(clocks_per_bit);

  typedef logic[timer_bits-1:0] timer;

  /*
  wire tx_req = 1;
  wire [7:0] tx_data = 35;
  wire tx_busy;

  uart_tx #(1) serial_tx(clk, rst_n, tx_data, tx_req, out_tx, tx_busy);
  */

  /*
  wire [7:0] tx_data;
  wire [7:0] rx_data;
  wire rx_valid;

  wire loopback;
  wire tx_req;
  wire tx_busy;
  wire message_sent;

  uart_tx #(clocks_per_bit) serial_tx(CLK, RST_N, tx_data, tx_req, loopback, tx_busy);
  uart_rx #(clocks_per_bit) serial_rx(CLK, RST_N, loopback, rx_data, rx_valid);

  uart_hello hello(CLK, RST_N, tx_busy, tx_data, tx_req, message_sent);

  assign LEDS = tx_data;
  */

  //----------------------------------------

  logic[7:0]  tx_data;
  logic       tx_req;

  logic       tx_cts;
  logic       tx_idle;
  timer       tx_cycle;
  logic[4:0]  tx_bit;
  logic[8:0]  tx_buf;

  always @(posedge ser_clk, negedge rst_n) begin
    if (!rst_n) begin
      tx_cycle <= 0;
      tx_bit  <= 0;
      tx_buf   <= '1;
    end
    else begin
      if (tx_cts && tx_req) begin
        tx_cycle <= clocks_per_bit - 1;
        tx_bit <= 16;
        tx_buf <= tx_data << 1;
      end else begin
        if (tx_bit && !tx_cycle) begin
          tx_bit <= tx_bit - 1;
          tx_cycle <= clocks_per_bit - 1;
          tx_buf <= (tx_buf >> 1) | 9'h100;
        end else begin
          if (tx_cycle) tx_cycle <= tx_cycle - 1;
        end
      end
    end
  end

  always_comb begin
    SER_TX  = tx_buf[0];
    tx_cts  = (tx_bit <= 7) && (tx_cycle <= 1);
    tx_idle = (tx_bit == 0) && (tx_cycle <= 1);
  end

  //----------------------------------------

  localparam message_len = 8;
  logic[7:0] message[0:7];
  logic[3:0] message_cursor;
  logic      message_done;

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

  always_ff @(posedge ser_clk, negedge rst_n) begin
    if (!rst_n) begin
      tx_req <= 0;
      message_cursor <= message_len;
      message_done <= 0;
      tx_data <= 8'hFF;
    end else begin
      if (tx_idle && !message_done) begin
        message_cursor <= 0;
        tx_req <= 1;
        tx_data <= message[0];
      end else begin
        if (tx_cts && !tx_req && message_cursor < message_len) begin
          message_cursor <= message_cursor + 1;
          if (message_cursor + 1 < message_len) begin
            tx_data <= message[message_cursor + 1];
            tx_req <= 1;
          end else begin
            //message_done <= 1;
            tx_data <= 8'hFF;
            tx_req <= 0;
          end
        end

        if (tx_req && !tx_cts) begin
          tx_req <= 0;
        end
      end
    end
  end

  //----------------------------------------

  timer        rx_cycle;
  logic [3:0]  rx_bit;
  logic [7:0]  out_data;
  logic        out_valid;

  always_ff @(posedge ser_clk, negedge rst_n) begin
    if (!rst_n) begin
      rx_cycle <= 0;
      rx_bit <= 0;
      out_data <= 0;
    end else begin
      if (rx_cycle) begin
        rx_cycle <= rx_cycle - 1;
      end
      else if (rx_bit) begin
        rx_cycle <= clocks_per_bit - 1;
        rx_bit <= rx_bit - 1;
        out_data <= {SER_TX, out_data[7:1]};
      end
      else if (!SER_TX) begin
        rx_bit <= 9;
        rx_cycle <= clocks_per_bit - 1;
      end
    end
  end

  always_comb begin
    out_valid = rx_bit == 1;
    LEDS = out_valid ? out_data : 0;
  end

endmodule

//==============================================================================

`endif
