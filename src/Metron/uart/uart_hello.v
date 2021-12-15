`timescale 1 ns / 1 ps

//==============================================================================
// synthesizable "Hello World!" transmitter

module rs232_hello(
  input clk,
  input resetn,
  input tx_busy,
  output [7:0] out_tx_data,
  output out_tx_en,
  output out_message_sent
);

  //----------

  reg [7:0] message[0:13];
  initial $readmemh("message", message);

  reg [2:0] state, state_;
  reg [3:0] sent_count, sent_count_;
  reg [7:0] next_char;
  reg [7:0] tx_data, tx_data_;
  reg tx_en, tx_en_;
  reg message_sent, message_sent_;

  assign out_tx_data = tx_data;
  assign out_tx_en = tx_en;
  assign out_message_sent = message_sent;

  //------------------------------------------

  always @* begin
    state_ = state;
    sent_count_ = sent_count;
    message_sent_ = message_sent;

    tx_data_ = tx_data;
    tx_en_ = tx_en;

    case(state)
    0: begin
      // waiting for not busy
      if (!tx_busy) begin
        state_ = (sent_count == 14) ? 2 : 1;
      end

    end

    1: begin
      // waiting for busy
      tx_data_ = next_char;
      tx_en_ = 1;
      if (tx_busy) begin
        sent_count_ = sent_count + 1;
        tx_en_ = 0;
        state_ = 0;
      end
    end

    2: message_sent_ = 1;
    endcase
  end

  //------------------------------------------

  always @(posedge clk) begin
    if (!resetn) begin
      state <= 0;
      sent_count <= 0;
      next_char <= message[0];
      tx_data <= 0;
      tx_en <= 0;
      message_sent <= 0;
    end else begin
      state <= state_;
      sent_count <= sent_count_;
      next_char <= message[sent_count];
      tx_data <= tx_data_;
      tx_en <= tx_en_;
      message_sent <= message_sent_;
      /*
      if (state_ == 0) begin
        repeat ($urandom % 9) @(posedge clk);
      end
      */
    end
  end

endmodule