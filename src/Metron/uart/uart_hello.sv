`ifndef UART_HELLO_SV
`define UART_HELLO_SV
`default_nettype none

//==============================================================================

module uart_hello
(
  input logic clk,
  input logic rst_n,
  input logic in_tx_busy,
  output logic [7:0] out_tx_data,
  output logic out_tx_en,
  output logic out_message_sent
);

  localparam message_len = 8;

  logic[7:0] message[0:7];

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

  logic [3:0] cursor;
  logic in_tx_busy_old;

  logic busy;
  logic done;

  always_comb begin
    out_tx_data = busy ? message[cursor] : 0;
    out_tx_en = busy;
    out_message_sent = done;
  end

  always @(posedge clk) begin
    if (!rst_n) begin
      busy <= 0;
      done <= 0;
      cursor <= 0;
      in_tx_busy_old <= 0;
    end else begin
      in_tx_busy_old <= in_tx_busy;

      if (!done && !busy) begin
        // Start state
        if (!in_tx_busy) begin
          busy <= 1;
          done <= 0;
          cursor <= 0;
        end
      end else if (!done && busy) begin
        // Working state
        if (in_tx_busy_old && !in_tx_busy) begin
          if (cursor == message_len - 1) begin
            busy <= 0;
            done <= 1;
            cursor <= 0;
          end else begin
            busy <= 1;
            done <= 0;
            cursor <= cursor + 1;
          end
        end
      end else if (done && !busy) begin
        // End state
      end else if (done && busy) begin
        // Invalid state
      end
    end
  end

endmodule

//==============================================================================

`endif
