`ifndef METRON_H_SV
`define METRON_H_SV

/*
`ifndef IVERILOG
typedef logic bool;
`endif

typedef logic[7:0]  uint8_t;
typedef logic[15:0] uint16_t;
typedef logic[31:0] uint32_t;
typedef logic[63:0] uint64_t;
*/

/*verilator lint_off width*/
/* verilator lint_off IGNOREDRETURN */

`default_nettype none

/*
`ifdef IVERILOG
`define IV_TEST(A,B) initial #1 if (A != B) $display("FAIL", A, B);
`else
`define IV_TEST(A,B)
`endif
*/

`endif
