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
#include "metron_tools.h"

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


  void init() {
  }

  void tick(bool rst_n, logic<N> req_i, logic<DW> data_i[N], logic<1> ready_i) {
  }

  // align to powers of 2 for simplicity
  // a full binary tree with N levels has 2**N + 2**N-1 nodes
  logic<pow2(IdxW + 1) - 1> req_tree;
  logic<pow2(IdxW + 1) - 1> gnt_tree;
  logic<pow2(IdxW + 1) - 1> idx_tree[IdxW];
  logic<pow2(IdxW + 1) - 1> data_tree[DW];

  void tock(bool rst_n, logic<N> req_i, logic<DW> data_i[N], logic<1> ready_i) {
    /*
    for (int level = 0; level < IdxW + 1; level++) {
      //
      // level+1   C0   C1   <- "Base1" points to the first node on "level+1",
      //            \  /         these nodes are the children of the nodes one level below
      // level       Pa      <- "Base0", points to the first node on "level",
      //                         these nodes are the parents of the nodes one level above
      //
      // hence we have the following indices for the Pa, C0, C1 nodes:
      // Pa = 2**level     - 1 + offset       = Base0 + offset
      // C0 = 2**(level+1) - 1 + 2*offset     = Base1 + 2*offset
      // C1 = 2**(level+1) - 1 + 2*offset + 1 = Base1 + 2*offset + 1
      //

      static const int Base0 = pow2(level) - 1;
      static const int Base1 = pow2(level + 1) - 1;

      for (int offset = 0; offset < pow2(level); offset++) {
      }
    }

    // the results can be found at the tree root
    if (EnDataPort) {
      data_o = data_tree[0];
    } else {
      logic<DW> unused_data;
      unused_data = data_tree[0];
      data_o = 1;
    }

    idx_o       = idx_tree[0];
    valid_o     = req_tree[0];

    // this propagates a grant back to the input
    gnt_tree[0] = valid_o & ready_i;
    */
  }

};