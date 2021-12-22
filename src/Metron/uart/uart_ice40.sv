`ifndef UART_BENCH_SV
`define UART_BENCH_SV
`default_nettype none
`timescale 1 ns / 1 ps

`include "uart_top.sv"

//==============================================================================

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

module pll(
        input  clock_in,
        output clock_out,
        output locked
        );

SB_PLL40_CORE #(
                .FEEDBACK_PATH("SIMPLE"),
                .DIVR(4'b0000),         // DIVR =  0
                .DIVF(7'b0111111),      // DIVF = 63
                .DIVQ(3'b101),          // DIVQ =  5
                .FILTER_RANGE(3'b001)   // FILTER_RANGE = 1
        ) uut (
                .LOCK(locked),
                .RESETB(1'b1),
                .BYPASS(1'b0),
                .REFERENCECLK(clock_in),
                .PLLOUTCORE(clock_out)
                );

endmodule

//==============================================================================

module uart_ice40(
  input logic CLK,

  // Serial port to host
  output logic SER_DCDn,
  output logic SER_DSRn,
  input  logic SER_DTRn,
  input  logic SER_RTSn,
  output logic SER_CTSn,
  output logic SER_TX,
  input  logic SER_RX,

  // On-board LEDs
  output logic [7:0] LEDS,

  // Top pin rown connection to logic analyser
  output logic LOGIC0
);

  logic pll_clk;
  logic rst_n;
  pll pll(CLK, pll_clk, rst_n);

  localparam pll_clk_rate    = 24000000;
  localparam ser_clk_rate_4x = 300 * 4;
  localparam ser_clk_rate_1x =     300;

  logic       ser_clk_4x;
  logic[31:0] ser_clk_4x_div;

  always_ff @(posedge pll_clk) begin
    if (ser_clk_4x_div > pll_clk_rate - (ser_clk_rate_4x*2)) begin
      ser_clk_4x_div <= ser_clk_4x_div + (ser_clk_rate_4x*2) - pll_clk_rate;
      ser_clk_4x <= !ser_clk_4x;
    end else begin
      ser_clk_4x_div <= ser_clk_4x_div + (ser_clk_rate_4x*2);
    end
  end

  logic       ser_clk_1x;
  logic[31:0] ser_clk_1x_div;

  always_ff @(posedge pll_clk) begin
    if (ser_clk_1x_div > pll_clk_rate - (ser_clk_rate_1x*2)) begin
      ser_clk_1x_div <= ser_clk_1x_div + (ser_clk_rate_1x*2) - pll_clk_rate;
      ser_clk_1x <= !ser_clk_1x;
    end else begin
      ser_clk_1x_div <= ser_clk_1x_div + (ser_clk_rate_1x*2);
    end
  end

  /*
  uart_top #(ser_clk_rate_4x, clocks_per_bit) dut(ser_clk_4x, rst_n, SER_TX, LEDS);
  */

  //============================================================================

  uart_top dut(ser_clk_4x, ser_clk_1x, rst_n, SER_TX, LEDS);
  assign LOGIC0 = SER_TX;

endmodule

//==============================================================================

`endif
