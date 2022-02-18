/* verilator lint_off UNUSED */
/* verilator lint_off UNDRIVEN */
(* blackbox *)
module SB_PLL40_CORE (
  output  LOCK,
  input   RESETB,
  input   BYPASS,
  input   REFERENCECLK,
  output  PLLOUTCORE
);
  parameter FEEDBACK_PATH = "SIMPLE";
  parameter DELAY_ADJUSTMENT_MODE_FEEDBACK = "FIXED";
  parameter DELAY_ADJUSTMENT_MODE_RELATIVE = "FIXED";
  parameter SHIFTREG_DIV_MODE = 1'b0;
  parameter FDA_FEEDBACK = 4'b0000;
  parameter FDA_RELATIVE = 4'b0000;
  parameter PLLOUT_SELECT = "GENCLK";
  parameter DIVR = 4'b0000;
  parameter DIVF = 7'b0000000;
  parameter DIVQ = 3'b000;
  parameter FILTER_RANGE = 3'b000;
  parameter ENABLE_ICEGATE = 1'b0;
  parameter TEST_MODE = 1'b0;
  parameter EXTERNAL_DIVIDE_FACTOR = 1;
endmodule
/* verilator lint_on UNUSED */
/* verilator lint_on UNDRIVEN */
