// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// N:1 fixed priority arbiter module (index 0 has highest prio)
//
// Verilog parameter
//   N:           Number of request ports
//   DW:          Data width
//   DataPort:    Set to 1 to enable the data port. Otherwise that port will be ignored.
//
// See also: prim_arbiter_ppc, prim_arbiter_tree

/* verilator lint_off ALWCOMBORDER */

module prim_arbiter_fixed #(
  parameter int N   = 8,
  parameter int DW  = 32,

  // Configurations
  // EnDataPort: {0, 1}, if 0, input data will be ignored
  parameter bit EnDataPort = 1,

  // Derived parameters
  localparam int IdxW = $clog2(N)
) (
  // used for assertions only
  input clk_i,
  input rst_ni,

  input        [ N-1:0]    req_i,
  input        [DW-1:0]    data_i [N],
  input                    ready_i,

  output logic [ N-1:0]    gnt_o,
  output logic [IdxW-1:0]  idx_o,
  output logic             valid_o,
  output logic [DW-1:0]    data_o
);

endmodule : prim_arbiter_fixed