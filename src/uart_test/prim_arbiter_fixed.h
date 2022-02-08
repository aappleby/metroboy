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

#pragma once
#include "metron.h"

template<
  int N = 8,
  int DW = 32,

  // Configurations
  // EnDataPort: {0, 1}, if 0, input data will be ignored
  int EnDataPort = 1,

  // Derived parameters
  int IdxW = clog2(N)
>
struct prim_arbiter_fixed {

  logic<N>    gnt_o;
  logic<IdxW> idx_o;

  logic<1>    valid_o;
  logic<DW>   data_o;


  // align to powers of 2 for simplicity
  // a full binary tree with N levels has 2**N + 2**N-1 nodes
  logic<pow2(IdxW + 1) - 1> req_tree;
  logic<pow2(IdxW + 1) - 1> gnt_tree;
  logic<pow2(IdxW + 1) - 1> idx_tree[IdxW];
  logic<pow2(IdxW + 1) - 1> data_tree[DW];

  void init() {
  }

  void tick(bool rst_n, logic<N> req_i, logic<DW> data_i[N], logic<1> ready_i) {
  }

  

  void tock(bool rst_n, logic<N> req_i, logic<DW> data_i[N], logic<1> ready_i) {
    for (int level = 0; level < IdxW + 1; level++) {
    }
  }

};