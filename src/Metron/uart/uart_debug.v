//`define DEBUG_TX
//`define DEBUG_RX

  //============================================================================
  // reset generator

  /*
  reg[2:0] resetn_counter = 0;
  always @(posedge clk) resetn_counter <= resetn_counter + (&resetn_counter ? 0 : 1);
  wire resetn = &resetn_counter;
  */

  //============================================================================
  // transmitter

  /*
`ifdef DEBUG_TX

  initial begin
    tx <= 1;
    tx_busy <= 0;
  end

  always @(posedge clk) begin
    if (!resetn) begin
      tx <= 1;
      tx_busy <= 0;
    end else begin
      integer i;
      
      tx <= 1;
      tx_busy <= 0;

      if (tx_en) begin
        reg [7:0] data;

        data = tx_data;
        tx_busy <= 1;
        tx <= 0;
        repeat (clocks_per_bit) @(posedge clk);
        for (i = 0; i < 8; i = i + 1) begin
          tx <= data[i];
          repeat (clocks_per_bit) @(posedge clk);
        end
        tx <= 1;
        repeat (clocks_per_bit) @(posedge clk);
        tx_busy <= 0;
      end
    end
  end

`else

  rs232_tx #(clocks_per_bit) dut_tx(clk, resetn, tx_data, tx_en, tx, tx_busy);

`endif
  */

  //============================================================================
  // receiver

  /*
`ifdef DEBUG_RX

  always @(posedge clk) begin
    if (!resetn) begin
      rx_valid <= 0;
      rx_data <= 0;
    end else begin
      integer i;
      reg b;
      rx_valid <= 0;
      wait(!rx);
      repeat (clocks_per_bit) @(posedge clk);
      for (i = 0; i < 8; i++) begin
        repeat (1) @(posedge clk);
        rx_data = { rx, rx_data[7:1] };
        repeat (clocks_per_bit - 1) @(posedge clk);
      end
      repeat (clocks_per_bit) @(posedge clk);
      rx_valid <= 1;
    end
  end

`else

  rs232_rx #(clocks_per_bit) dut_rx(clk, resetn, rx, rx_data, rx_valid);

`endif
  */

