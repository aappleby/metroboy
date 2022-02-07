//--------------------------------------------------------------------------------
// MODULE:       prim_arbiter_fixed
// MODULEPARAMS: N, DW, EnDataPort, IdxW, 
// INPUTS:       
// OUTPUTS:      gnt_o, idx_o, valid_o, data_o, 
// LOCALPARAMS:  
// FIELDS:       req_tree, gnt_tree, idx_tree, data_tree, 
// SUBMODULES:   
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none
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


`include "metron.h.sv"


module prim_arbiter_fixed
#(
  parameter int N = 'd8,
  parameter int DW = 'd32,

  // Configurations
  // EnDataPort: {0, 1}, if 0, input data will be ignored
  parameter int EnDataPort = 'd1,

  // Derived parameters
  parameter int IdxW = $clog2(N)
)
(clk, rst_n, gnt_o, idx_o, valid_o, data_o);
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n; 

  output logic[N-1:0]    gnt_o;
  output logic[IdxW-1:0] idx_o;

  output logic    valid_o;
  output logic[DW-1:0]   data_o;


  // align to powers of 2 for simplicity
  // a full binary tree with N levels has 2**N + 2**N-1 nodes
  logic[2**(IdxW + 'd1) - 'd1-1:0] req_tree;
  logic[2**(IdxW + 'd1) - 'd1-1:0] gnt_tree;
  logic[2**(IdxW + 'd1) - 'd1-1:0] idx_tree[IdxW];
  logic[2**(IdxW + 'd1) - 'd1-1:0] data_tree[DW];

  initial begin : INIT
  end

  always_ff @(posedge clk, negedge rst_n) begin : TICK
  end

  

  always_comb begin : TOCK

  end

endmodule