`ifndef UART_BENCH_SV
`define UART_BENCH_SV
`default_nettype none

`include "uart_top.sv"
`include "SB_PLL40_CORE.v"

//==============================================================================

module uart_ice40(
  input logic CLK,

  // Serial port to host
  //output logic SER_DCDn,
  //output logic SER_DSRn,
  //input  logic SER_DTRn,
  //input  logic SER_RTSn,
  //output logic SER_CTSn,
  output logic SER_TX,
  //input  logic SER_RX,

  // On-board LEDs
  output logic [7:0] LEDS,

  // Top pin row connection to logic analyser
  output logic LOGIC7
);

  logic pll_clk;
  logic rst_n;

  /**
   * PLL configuration
   *
   * This Verilog module was generated automatically
   * using the icepll tool from the IceStorm project.
   * Use at your own risk.
   *
   * Given input frequency:        12.000 MHz
   * Requested output frequency:   24.000 MHz
   * Achieved output frequency:    24.000 MHz
   */
  SB_PLL40_CORE #(
                  .FEEDBACK_PATH("SIMPLE"),
                  .DIVR(4'b0000),         // DIVR =  0
                  .DIVF(7'b0111111),      // DIVF = 63
                  .DIVQ(3'b101),          // DIVQ =  5
                  .FILTER_RANGE(3'b001)   // FILTER_RANGE = 1
          ) uut (
                  .LOCK(rst_n),
                  .RESETB(1'b1),
                  .BYPASS(1'b0),
                  .REFERENCECLK(CLK),
                  .PLLOUTCORE(pll_clk)
                  );


  localparam pll_clk_rate = 24000000;
  localparam ser_clk_rate =     1200;
  localparam clocks_per_bit = pll_clk_rate / ser_clk_rate;

  logic ser_tx;
  logic[7:0] out_data;
  logic out_valid;

  uart_top #(.clocks_per_bit(clocks_per_bit)) dut(pll_clk, rst_n, ser_tx, out_data, out_valid);

  always_comb begin
    SER_TX = ser_tx;
    LOGIC7 = ser_tx;
    LEDS = out_valid ? out_data : '0;
  end

endmodule

//==============================================================================

`endif
