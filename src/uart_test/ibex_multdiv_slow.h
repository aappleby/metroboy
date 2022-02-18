// Copyright lowRISC contributors.
// Copyright 2018 ETH Zurich and University of Bologna, see also CREDITS.md.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/**
 * Slow Multiplier and Division
 *
 * Baugh-Wooley multiplier and Long Division
 */

#pragma once
#include "metron_tools.h"
#include "ibex_pkg.h"

struct ibex_multdiv_slow {

  logic<33> alu_operand_a_o;
  logic<33> alu_operand_b_o;
  logic<34> imd_val_d_o[2];
  logic<2>  imd_val_we_o;
  logic<32> multdiv_result_o;
  logic<1>  valid_o;

  enum class md_fsm_e : logic<3>::BASE {
    MD_IDLE, MD_ABS_A, MD_ABS_B, MD_COMP, MD_LAST, MD_CHANGE_SIGN, MD_FINISH
  };
  md_fsm_e md_state_q, md_state_d;

  logic<33> accum_window_q, accum_window_d;
  logic<1>  unused_imd_val0;
  logic<2>  unused_imd_val1;

  logic<33> res_adder_l;
  logic<33> res_adder_h;

  logic<5>  multdiv_count_q, multdiv_count_d;
  logic<33> op_b_shift_q, op_b_shift_d;
  logic<33> op_a_shift_q, op_a_shift_d;
  logic<33> op_a_ext, op_b_ext;
  logic<33> one_shift;
  logic<33> op_a_bw_pp, op_a_bw_last_pp;
  logic<32> b_0;
  logic<1>  sign_a, sign_b;
  logic<33> next_quotient;
  logic<32> next_remainder;
  logic<32> op_numerator_q, op_numerator_d;
  logic<1>  is_greater_equal;
  logic<1>  div_change_sign, rem_change_sign;
  logic<1>  div_by_zero_d, div_by_zero_q;
  logic<1>  multdiv_hold;
  logic<1>  multdiv_en;

  void tock4(logic<34> alu_adder_ext_i) {
     // (accum_window_q + op_a_shift_q)
    res_adder_l = b33(alu_adder_ext_i);
     // (accum_window_q + op_a_shift_q)>>1
    res_adder_h = b33(alu_adder_ext_i, 1);
  }

  /////////////////////
  // ALU Operand MUX //
  /////////////////////

  void tock7(logic<34> imd_val_q_i[2]) {
    // Intermediate value register shared with ALU
    imd_val_d_o[0]  = cat(b1(0b0), accum_window_d);
    imd_val_we_o[0] = ~multdiv_hold;
    accum_window_q  = b33(imd_val_q_i[0]);
    unused_imd_val0 = imd_val_q_i[0][33];

    imd_val_d_o[1]  = cat(b2(0b00), op_numerator_d);
    imd_val_we_o[1] = multdiv_en;
    op_numerator_q  = b32(imd_val_q_i[1]);
    unused_imd_val1 = b2(imd_val_q_i[1], 32);
  }

  void tock1(
    logic<1>  mult_en_i,
    logic<1>  div_en_i,
    logic<1>  mult_sel_i,
    logic<1>  div_sel_i,
    ibex_pkg::md_op_e   operator_i,
    logic<2>  signed_mode_i,
    logic<32> op_a_i,
    logic<32> op_b_i,
    logic<34> alu_adder_ext_i,
    logic<32> alu_adder_i,
    logic<1>  equal_to_zero_i,
    logic<1>  data_ind_timing_i,
    logic<34> imd_val_q_i[2],
    logic<1>  multdiv_ready_id_i) {
    using namespace ibex_pkg;
    alu_operand_a_o = accum_window_q;

    switch (operator_i) {
      case md_op_e::MD_OP_MULL: {
        alu_operand_b_o = op_a_bw_pp;
        break;
      }

      case md_op_e::MD_OP_MULH: {
        alu_operand_b_o = (md_state_q == md_fsm_e::MD_LAST) ? op_a_bw_last_pp : op_a_bw_pp;
        break;
      }

      case md_op_e::MD_OP_DIV:
      case md_op_e::MD_OP_REM: {
        switch (md_state_q) {
          case md_fsm_e::MD_IDLE: {
            // 0 - B = 0 iff B == 0
            alu_operand_a_o = cat(b32(0x0), b1(0b1));
            alu_operand_b_o = cat(~op_b_i, b1(0b1));
            break;
          }
          case md_fsm_e::MD_ABS_A: {
            // ABS(A) = 0 - A
            alu_operand_a_o = cat(b32(0x0), b1(0b1));
            alu_operand_b_o = cat(~op_a_i, b1(0b1));
            break;
          }
          case md_fsm_e::MD_ABS_B: {
            // ABS(B) = 0 - B
            alu_operand_a_o = cat(b32(0x0), b1(0b1));
            alu_operand_b_o = cat(~op_b_i, b1(0b1));
            break;
          }
          case md_fsm_e::MD_CHANGE_SIGN: {
            // ABS(Quotient) = 0 - Quotient (or Reminder)
            alu_operand_a_o = cat(b32(0x0), b1(0b1));
            alu_operand_b_o = cat(~b32(accum_window_q), b1(0b1));
            break;
          }
          default: {
            // Division
            alu_operand_a_o = cat(b32(accum_window_q), b1(0b1)); // it contains the remainder
            alu_operand_b_o = cat(~b32(op_b_shift_q), b1(0b1));     // -denominator two's compliment
            break;
         }
        }
        break;
      }
      default: {
        alu_operand_a_o = accum_window_q;
        alu_operand_b_o = cat(~b32(op_b_shift_q), b1(0b1));
        break;
      }
    }
  }

  void tock6(logic<32> op_a_i, logic<32> op_b_i, logic<2>  signed_mode_i) {
    // Multiplier partial product calculation
    b_0             = dup<32>(b1(op_b_shift_q));
    op_a_bw_pp      = cat( ~b1(op_a_shift_q, 32) & b1(op_b_shift_q),  (b32(op_a_shift_q) & b_0) );
    op_a_bw_last_pp = cat(  b1(op_a_shift_q, 32) & b1(op_b_shift_q), ~(b32(op_a_shift_q) & b_0) );

    // Sign extend the input operands
    sign_a   = b1(op_a_i, 31) & b1(signed_mode_i, 0);
    sign_b   = b1(op_b_i, 31) & b1(signed_mode_i, 1);

    op_a_ext = cat(sign_a, op_a_i);
    op_b_ext = cat(sign_b, op_b_i);

    // Divider calculations

    // The adder in the ALU computes Remainder - Divisor. If Remainder - Divisor >= 0,
    // is_greater_equal is true, the next Remainder is the subtraction result and the Quotient
    // multdiv_count_q-th bit is set to 1.
    is_greater_equal = (b1(accum_window_q, 31) == b1(op_b_shift_q, 31)) ?
      ~b1(res_adder_h, 31) : b1(accum_window_q, 31);

    one_shift      = cat( b32(0b0), b1(0b1) ) << multdiv_count_q;

    next_remainder = is_greater_equal ? b32(res_adder_h)         : b32(accum_window_q);
    next_quotient  = is_greater_equal ? op_a_shift_q | one_shift : op_a_shift_q;

    div_change_sign = (sign_a ^ sign_b) & ~div_by_zero_q;
    rem_change_sign = sign_a;
  }

  void tock2(logic<1>  mult_en_i,
    logic<1>  div_en_i,
    logic<1>  mult_sel_i,
    logic<1>  div_sel_i,
    ibex_pkg::md_op_e   operator_i,
    logic<2>  signed_mode_i,
    logic<32> op_a_i,
    logic<32> op_b_i,
    logic<34> alu_adder_ext_i,
    logic<32> alu_adder_i,
    logic<1>  equal_to_zero_i,
    logic<1>  data_ind_timing_i,
    logic<34> imd_val_q_i[2],
    logic<1>  multdiv_ready_id_i) {
    using namespace ibex_pkg;
    multdiv_count_d  = multdiv_count_q;
    accum_window_d   = accum_window_q;
    op_b_shift_d     = op_b_shift_q;
    op_a_shift_d     = op_a_shift_q;
    op_numerator_d   = op_numerator_q;
    md_state_d       = md_state_q;
    multdiv_hold     = b1(0b0);
    div_by_zero_d    = div_by_zero_q;
    if (mult_sel_i || div_sel_i) {
      switch (md_state_q) {
        case md_fsm_e::MD_IDLE : {
          switch (operator_i) {
            case md_op_e::MD_OP_MULL : {
              op_a_shift_d   = op_a_ext << 1;
              accum_window_d = cat(~(op_a_ext[32] & op_b_i[0]),
                                   b32(op_a_ext) & dup<32>(op_b_i[0]));
              op_b_shift_d   = op_b_ext >> 1;
              // Proceed with multiplication by 0/1 in data-independent time mode
              md_state_d     = (!data_ind_timing_i && ((op_b_ext >> 1) == 0)) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              break;
            }
            case md_op_e::MD_OP_MULH: {
              op_a_shift_d   = op_a_ext;
              accum_window_d = cat( b1(0b1), ~(op_a_ext[32] & op_b_i[0]),
                                   b31(op_a_ext, 1) & dup<31>(op_b_i[0])  );
              op_b_shift_d   = op_b_ext >> 1;
              md_state_d     = md_fsm_e::MD_COMP;
              break;
            }
            case md_op_e::MD_OP_DIV : {
              // Check if the denominator is 0
              // quotient for division by 0 is specified to be -1
              // Note with data-independent time option, the full divide operation will proceed as
              // normal and will naturally return -1
              accum_window_d = dup<33>(b1(0b1));
              md_state_d     = (!data_ind_timing_i && equal_to_zero_i) ? md_fsm_e::MD_FINISH : md_fsm_e::MD_ABS_A;
              // Record that this is a div by zero to stop the sign change at the end of the
              // division (in data_ind_timing mode).
              div_by_zero_d  = equal_to_zero_i;
              break;
            }
            case md_op_e::MD_OP_REM : {
              // Check if the denominator is 0
              // remainder for division by 0 is specified to be the numerator (operand a)
              // Note with data-independent time option, the full divide operation will proceed as
              // normal and will naturally return operand a
              accum_window_d = op_a_ext;
              md_state_d     = (!data_ind_timing_i && equal_to_zero_i) ? md_fsm_e::MD_FINISH : md_fsm_e::MD_ABS_A;
              break;
            }
            default: break;
          }
          multdiv_count_d   = b5(31);
          break;
        }

        case md_fsm_e::MD_ABS_A: {
          // quotient
          op_a_shift_d = 0;
          // A abs value
          op_numerator_d = sign_a ? alu_adder_i : op_a_i;
          md_state_d     = md_fsm_e::MD_ABS_B;
          break;
        }

        case md_fsm_e::MD_ABS_B: {
          // remainder
          accum_window_d = cat(b32(0x0), b1(op_numerator_q, 31));
          // B abs value
          op_b_shift_d   = sign_b ? cat(b1(0b0), alu_adder_i) : cat(b1(0b0), op_b_i);
          md_state_d     = md_fsm_e::MD_COMP;
          break;
        }

        case md_fsm_e::MD_COMP: {
          multdiv_count_d = multdiv_count_q - b5(0x1);
          switch (operator_i) {
            case md_op_e::MD_OP_MULL: {
              accum_window_d = res_adder_l;
              op_a_shift_d   = op_a_shift_q << 1;
              op_b_shift_d   = op_b_shift_q >> 1;
              // Multiplication is complete once op_b is zero, unless in data_ind_timing mode where
              // the maximum possible shift-add operations will be completed regardless of op_b
              md_state_d     = ((!data_ind_timing_i && (op_b_shift_d == 0)) ||
                                (multdiv_count_q == b5(1))) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              break;
            }
            case md_op_e::MD_OP_MULH: {
              accum_window_d = res_adder_h;
              op_a_shift_d   = op_a_shift_q;
              op_b_shift_d   = op_b_shift_q >> 1;
              md_state_d     = (multdiv_count_q == b5(1)) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              break;
            }
            case md_op_e::MD_OP_DIV:
            case md_op_e::MD_OP_REM: {
              accum_window_d = cat(b32(next_remainder), op_numerator_q[multdiv_count_d]);
              op_a_shift_d   = next_quotient;
              md_state_d     = (multdiv_count_q == b5(1)) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              break;
            }
            default: break;
          }
          break;
        }

        case md_fsm_e::MD_LAST: {
          switch (operator_i) {
            case md_op_e::MD_OP_MULL: {
              accum_window_d = res_adder_l;

              // Note no state transition will occur if multdiv_hold is set
              md_state_d   = md_fsm_e::MD_IDLE;
              multdiv_hold = ~multdiv_ready_id_i;
              break;
            }
            case md_op_e::MD_OP_MULH: {
              accum_window_d = res_adder_l;
              md_state_d     = md_fsm_e::MD_IDLE;

              // Note no state transition will occur if multdiv_hold is set
              md_state_d   = md_fsm_e::MD_IDLE;
              multdiv_hold = ~multdiv_ready_id_i;
              break;
            }
            case md_op_e::MD_OP_DIV: {
              // this time we save the quotient in accum_window_q since we do not need anymore the
              // remainder
              accum_window_d = next_quotient;
              md_state_d     = md_fsm_e::MD_CHANGE_SIGN;
              break;
            }
            case md_op_e::MD_OP_REM: {
              // this time we do not save the quotient anymore since we need only the remainder
              accum_window_d = cat(b1(0b0), b32(next_remainder));
              md_state_d     = md_fsm_e::MD_CHANGE_SIGN;
              break;
            }
            default: break;
          }
          break;
        }

        case md_fsm_e::MD_CHANGE_SIGN: {
          md_state_d = md_fsm_e::MD_FINISH;
          switch (operator_i) {
            case md_op_e::MD_OP_DIV:
              accum_window_d = div_change_sign ? cat(b1(0b0), alu_adder_i) : accum_window_q;
              break;
            case md_op_e::MD_OP_REM:
              accum_window_d = rem_change_sign ? cat(b1(0b0), alu_adder_i) : accum_window_q;
              break;
            default: break;
          }
          break;
        }

        case md_fsm_e::MD_FINISH : {
          // Note no state transition will occur if multdiv_hold is set
          md_state_d   = md_fsm_e::MD_IDLE;
          multdiv_hold = ~multdiv_ready_id_i;
          break;
        }

        default: {
          md_state_d = md_fsm_e::MD_IDLE;
          break;
        }
      } // md_state_q
    } // (mult_sel_i || div_sel_i)
  }

  //////////////////////////////////////////
  // Mutliplier / Divider state registers //
  //////////////////////////////////////////

  void tock5(logic<1>  mult_en_i, logic<1>  div_en_i) {
    multdiv_en = (mult_en_i | div_en_i) & ~multdiv_hold;
  }

  void tick(bool rst_n) {
    if (!rst_n) {
      multdiv_count_q  = b5(0x0);
      op_b_shift_q     = b33(0x0);
      op_a_shift_q     = b33(0x0);
      md_state_q       = md_fsm_e::MD_IDLE;
      div_by_zero_q    = b1(0b0);
    } else if (multdiv_en) {
      multdiv_count_q  = multdiv_count_d;
      op_b_shift_q     = op_b_shift_d;
      op_a_shift_q     = op_a_shift_d;
      md_state_q       = md_state_d;
      div_by_zero_q    = div_by_zero_d;
    }
  }

  /////////////
  // Outputs //
  /////////////

  void tock3(bool rst_n, ibex_pkg::md_op_e operator_i, logic<1> div_en_i) {
    using namespace ibex_pkg;
    valid_o = (md_state_q == md_fsm_e::MD_FINISH) |
              ((md_state_q == md_fsm_e::MD_LAST) &
              ((operator_i == md_op_e::MD_OP_MULL) |
               (operator_i == md_op_e::MD_OP_MULH)));

    multdiv_result_o = div_en_i ? b32(accum_window_q) : b32(res_adder_l);
  }

/*
  ////////////////
  // Assertions //
  ////////////////

  // State must be valid.
  `ASSERT(IbexMultDivStateValid, md_state_q inside {
      MD_IDLE, MD_ABS_A, MD_ABS_B, MD_COMP, MD_LAST, MD_CHANGE_SIGN, MD_FINISH
      }, clk_i, !rst_ni)

`ifdef FORMAL
  `ifdef YOSYS
    `include "formal_tb_frag.svh"
  `endif
`endif
 */
};