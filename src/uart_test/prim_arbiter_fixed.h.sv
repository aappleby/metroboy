//--------------------------------------------------------------------------------
// MODULE:       prim_arbiter_fixed
// MODULEPARAMS: N, DW, 
// INPUTS:       
// OUTPUTS:      
// LOCALPARAMS:  
// FIELDS:       
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
#(parameter int N = 'd8, parameter int DW = 'd32)
(clk, rst_n);
/*verilator public_module*/

input logic clk;
input logic rst_n; 

endmodule