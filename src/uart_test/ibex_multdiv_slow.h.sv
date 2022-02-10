//--------------------------------------------------------------------------------
// MODULE:       ibex_multdiv_slow
// MODULEPARAMS: 
// INPUTS:       mult_en_i, div_en_i, mult_sel_i, div_sel_i, operator_i, signed_mode_i, op_a_i, op_b_i, alu_adder_ext_i, alu_adder_i, equal_to_zero_i, data_ind_timing_i, imd_val_q_i, multdiv_ready_id_i, mult_en_i, div_en_i, mult_sel_i, div_sel_i, operator_i, signed_mode_i, op_a_i, op_b_i, alu_adder_ext_i, alu_adder_i, equal_to_zero_i, data_ind_timing_i, imd_val_q_i, multdiv_ready_id_i, mult_en_i, div_en_i, mult_sel_i, div_sel_i, operator_i, signed_mode_i, op_a_i, op_b_i, alu_adder_ext_i, alu_adder_i, equal_to_zero_i, data_ind_timing_i, imd_val_q_i, multdiv_ready_id_i, operator_i, div_en_i, 
// OUTPUTS:      alu_operand_a_o, alu_operand_b_o, imd_val_we_o, multdiv_result_o, valid_o, 
// LOCALPARAMS:  
// FIELDS:       imd_val_d_o, md_op_e, md_fsm_e, accum_window_q, unused_imd_val0, unused_imd_val1, res_adder_l, res_adder_h, multdiv_count_q, op_b_shift_q, op_a_shift_q, op_a_ext, one_shift, op_a_bw_pp, b_0, sign_a, next_quotient, next_remainder, op_numerator_q, is_greater_equal, div_change_sign, div_by_zero_d, multdiv_hold, multdiv_en, 
// SUBMODULES:   md_state_q, 
// TASKS:        tock1, tock2, tock3, 
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none
// Copyright lowRISC contributors.
// Copyright 2018 ETH Zurich and University of Bologna, see also CREDITS.md.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

/**
 * Slow Multiplier and Division
 *
 * Baugh-Wooley multiplier and Long Division
 */


`include "metron.h.sv"

module ibex_multdiv_slow
(
input logic clk,
input logic rst_n,

input logic  mult_en_i,
input logic  div_en_i,
input logic  mult_sel_i,
input logic  div_sel_i,
input md_op_e   operator_i,
input logic[1:0]  signed_mode_i,
input logic[31:0] op_a_i,
input logic[31:0] op_b_i,
input logic[33:0] alu_adder_ext_i,
input logic[31:0] alu_adder_i,
input logic  equal_to_zero_i,
input logic  data_ind_timing_i,
input logic[33:0] imd_val_q_i['d2],
input logic  multdiv_ready_id_i,

output logic[32:0] alu_operand_a_o,
output logic[32:0] alu_operand_b_o,
output logic[33:0] imd_val_d_o['d2],
output logic[1:0]  imd_val_we_o,
output logic[31:0] multdiv_result_o,
output logic  valid_o
);

  /*verilator public_module*/
  
  typedef enum logic[2:0] {
    // Multiplier/divider
    MD_OP_MULL,
    MD_OP_MULH,
    MD_OP_DIV,
    MD_OP_REM
  } md_op_e;

  typedef enum logic[3:0] {
    MD_IDLE, MD_ABS_A, MD_ABS_B, MD_COMP, MD_LAST, MD_CHANGE_SIGN, MD_FINISH
  } md_fsm_e;
  md_fsm_e md_state_q, md_state_d;

  logic[32:0] accum_window_q, accum_window_d;
  logic  unused_imd_val0;
  logic[1:0]  unused_imd_val1;

  logic[32:0] res_adder_l;
  logic[32:0] res_adder_h;

  logic[4:0]  multdiv_count_q, multdiv_count_d;
  logic[32:0] op_b_shift_q, op_b_shift_d;
  logic[32:0] op_a_shift_q, op_a_shift_d;
  logic[32:0] op_a_ext, op_b_ext;
  logic[32:0] one_shift;
  logic[32:0] op_a_bw_pp, op_a_bw_last_pp;
  logic[31:0] b_0;
  logic  sign_a, sign_b;
  logic[32:0] next_quotient;
  logic[31:0] next_remainder;
  logic[31:0] op_numerator_q, op_numerator_d;
  logic  is_greater_equal;
  logic  div_change_sign, rem_change_sign;
  logic  div_by_zero_d, div_by_zero_q;
  logic  multdiv_hold;
  logic  multdiv_en;

  always_ff @(posedge clk, negedge rst_n)
  begin : tick
  end
  

  /////////////////////
  // ALU Operand MUX //
  /////////////////////

  always_comb begin : tock1

     // (accum_window_q + op_a_shift_q)
    res_adder_l = 33'(alu_adder_ext_i);
     // (accum_window_q + op_a_shift_q)>>1
    res_adder_h = alu_adder_ext_i[33:1];

    //----------
    // Intermediate value register shared with ALU
    imd_val_d_o['d0]  = {1'd0, accum_window_d};
    imd_val_we_o['d0] = ~multdiv_hold;
    accum_window_q  = 33'(imd_val_q_i['d0]);
    unused_imd_val0 = (imd_val_q_i['d0])['d33];

    imd_val_d_o['d1]  = {2'd0, op_numerator_d};
    imd_val_we_o['d1] = multdiv_en;
    op_numerator_q  = 32'(imd_val_q_i['d1]);
    unused_imd_val1 = imd_val_q_i[1][33:32];

    //----------

    multdiv_en = (mult_en_i | div_en_i) & ~multdiv_hold;

    //----------

    alu_operand_a_o = accum_window_q;

    case (operator_i) 
      MD_OP_MULL: begin
        alu_operand_b_o = op_a_bw_pp;
        /*break;*/
      end

      MD_OP_MULH: begin
        alu_operand_b_o = (md_state_q == md_fsm_e::MD_LAST) ? op_a_bw_last_pp : op_a_bw_pp;
        /*break;*/
      end

      MD_OP_DIV,
      MD_OP_REM: begin
        case (md_state_q) 
          MD_IDLE: begin
            // 0 - B = 0 iff B == 0
            alu_operand_a_o = {32'd0, 1'd1};
            alu_operand_b_o = {~op_b_i, 1'd1};
            /*break;*/
          end
          MD_ABS_A: begin
            // ABS(A) = 0 - A
            alu_operand_a_o = {32'd0, 1'd1};
            alu_operand_b_o = {~op_a_i, 1'd1};
            /*break;*/
          end
          MD_ABS_B: begin
            // ABS(B) = 0 - B
            alu_operand_a_o = {32'd0, 1'd1};
            alu_operand_b_o = {~op_b_i, 1'd1};
            /*break;*/
          end
          MD_CHANGE_SIGN: begin
            // ABS(Quotient) = 0 - Quotient (or Reminder)
            alu_operand_a_o = {32'd0, 1'd1};
            alu_operand_b_o = {~32'(accum_window_q), 1'd1};
            /*break;*/
          end
          default: begin
            // Division
            alu_operand_a_o = {32'(accum_window_q), 1'd1}; // it contains the remainder
            alu_operand_b_o = {~32'(op_b_shift_q), 1'd1};     // -denominator two's compliment
            /*break;*/
         end
        endcase
        /*break;*/
      end
      default: begin
        alu_operand_a_o = accum_window_q;
        alu_operand_b_o = {~32'(op_b_shift_q), 1'd1};
        /*break;*/
      end
    endcase

    // Multiplier partial product calculation
    b_0 = {32 {1'(op_b_shift_q)}};
    
    op_a_bw_pp = {
      ~1'(op_a_shift_q[32] & op_b_shift_q[0]),
      32'(op_a_shift_q & b_0)
    };

    op_a_bw_last_pp = {1'(op_a_shift_q[32] & op_b_shift_q[0]), 32'(~(32'(op_a_shift_q) & b_0)) };

    // Sign extend the input operands
    sign_a = op_a_i[31] & signed_mode_i[0];
    sign_b = op_b_i[31] & signed_mode_i[1];

    op_a_ext = {sign_a, op_a_i};
    op_b_ext = {sign_b, op_b_i};

    // Divider calculations

    // The adder in the ALU computes Remainder - Divisor. If Remainder - Divisor >= 0,
    // is_greater_equal is true, the next Remainder is the subtraction result and the Quotient
    // multdiv_count_q-th bit is set to 1.
    is_greater_equal = (b1(accum_window_q, 31) == b1(op_b_shift_q, 31)) ?
      ~b1(res_adder_h, 31) : b1(accum_window_q, 31);

    one_shift = 33'({ 32'd0, 1'd1 } << multdiv_count_q);

    next_remainder = is_greater_equal ? b32(res_adder_h) : b32(accum_window_q);
    next_quotient = is_greater_equal ? b33(op_a_shift_q | one_shift) : b33(op_a_shift_q);

    div_change_sign = (sign_a ^ sign_b) & ~div_by_zero_q;
    rem_change_sign = sign_a;
  end

  always_comb begin : tock2

    multdiv_count_d  = multdiv_count_q;
    accum_window_d   = accum_window_q;
    op_b_shift_d     = op_b_shift_q;
    op_a_shift_d     = op_a_shift_q;
    op_numerator_d   = op_numerator_q;
    md_state_d       = md_state_q;
    multdiv_hold     = 1'd0;
    div_by_zero_d    = div_by_zero_q;

    if (mult_sel_i || div_sel_i) begin
      case (md_state_q) 
        MD_IDLE : begin
          case (operator_i) 
            MD_OP_MULL : begin
              op_a_shift_d   = op_a_ext << 'd1;
              accum_window_d = {
                1'(~(op_a_ext['d32] & op_b_i['d0])),
                32'(op_a_ext & {32 {op_b_i['d0]}})
              };
              op_b_shift_d = op_b_ext >> 'd1;
              // Proceed with multiplication by 0/1 in data-independent time mode
              md_state_d = (!data_ind_timing_i && ((op_b_ext >> 1) == 0)) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              /*break;*/
            end
            MD_OP_MULH: begin
              //op_a_shift_d   = op_a_ext;
              //accum_window_d = { 1'b1, ~(op_a_ext[32]   &     op_b_i[0]),
              //                           op_a_ext[31:1] & {31{op_b_i[0]}}  };
              //op_b_shift_d   = op_b_ext >> 1;
              //md_state_d     = MD_COMP;

              op_a_shift_d   = op_a_ext;
              accum_window_d = { 1'd1, ~(op_a_ext['d32] & op_b_i['d0]), op_a_ext[31:1] & {31 {op_b_i['d0]}}  };
              op_b_shift_d   = op_b_ext >> 'd1;
              md_state_d     = MD_COMP;
              /*break;*/
            end
            MD_OP_DIV : begin
              // Check if the denominator is 0
              // quotient for division by 0 is specified to be -1
              // Note with data-independent time option, the full divide operation will proceed as
              // normal and will naturally return -1
              accum_window_d = {33 {1'd1}};
              md_state_d     = (!data_ind_timing_i && equal_to_zero_i) ? md_fsm_e::MD_FINISH : md_fsm_e::MD_ABS_A;
              // Record that this is a div by zero to stop the sign change at the end of the
              // division (in data_ind_timing mode).
              div_by_zero_d  = equal_to_zero_i;
              /*break;*/
            end
            MD_OP_REM : begin
              // Check if the denominator is 0
              // remainder for division by 0 is specified to be the numerator (operand a)
              // Note with data-independent time option, the full divide operation will proceed as
              // normal and will naturally return operand a
              accum_window_d = op_a_ext;
              md_state_d     = (!data_ind_timing_i && equal_to_zero_i) ? md_fsm_e::MD_FINISH : md_fsm_e::MD_ABS_A;
              /*break;*/
            end
            default: /*break;*/
          endcase
          multdiv_count_d   = 5'd31;
          /*break;*/
        end

        MD_ABS_A: begin
          // quotient
          op_a_shift_d = 33'd0;
          // A abs value
          op_numerator_d = sign_a ? alu_adder_i : op_a_i;
          md_state_d     = MD_ABS_B;
          /*break;*/
        end

        MD_ABS_B: begin
          // remainder
          accum_window_d = {32'd0, op_numerator_q[31]};
          // B abs value
          op_b_shift_d   = sign_b ? cat(b1(0), alu_adder_i) : cat(b1(0), op_b_i);
          md_state_d     = MD_COMP;
          /*break;*/
        end

        MD_COMP: begin
          multdiv_count_d = multdiv_count_q - 5'd1;
          case (operator_i) 
            MD_OP_MULL: begin
              accum_window_d = res_adder_l;
              op_a_shift_d   = op_a_shift_q << 'd1;
              op_b_shift_d   = op_b_shift_q >> 'd1;
              // Multiplication is complete once op_b is zero, unless in data_ind_timing mode where
              // the maximum possible shift-add operations will be completed regardless of op_b
              md_state_d     = ((!data_ind_timing_i && (op_b_shift_d == 0)) ||
                                (multdiv_count_q == b5(1))) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              /*break;*/
            end
            MD_OP_MULH: begin
              accum_window_d = res_adder_h;
              op_a_shift_d   = op_a_shift_q;
              op_b_shift_d   = op_b_shift_q >> 'd1;
              md_state_d     = (multdiv_count_q == 1) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              /*break;*/
            end
            MD_OP_DIV,
            MD_OP_REM: begin
              accum_window_d = {32'(next_remainder), op_numerator_q[multdiv_count_d]};
              op_a_shift_d   = next_quotient;
              md_state_d     = (multdiv_count_q == 1) ? md_fsm_e::MD_LAST : md_fsm_e::MD_COMP;
              /*break;*/
            end
            default: /*break;*/
          endcase
          /*break;*/
        end

        MD_LAST: begin
          case (operator_i) 
            MD_OP_MULL: begin
              accum_window_d = res_adder_l;

              // Note no state transition will occur if multdiv_hold is set
              md_state_d   = MD_IDLE;
              multdiv_hold = ~multdiv_ready_id_i;
              /*break;*/
            end
            MD_OP_MULH: begin
              accum_window_d = res_adder_l;
              md_state_d     = MD_IDLE;

              // Note no state transition will occur if multdiv_hold is set
              md_state_d   = MD_IDLE;
              multdiv_hold = ~multdiv_ready_id_i;
              /*break;*/
            end
            MD_OP_DIV: begin
              // this time we save the quotient in accum_window_q since we do not need anymore the
              // remainder
              accum_window_d = next_quotient;
              md_state_d     = MD_CHANGE_SIGN;
              /*break;*/
            end
            MD_OP_REM: begin
              // this time we do not save the quotient anymore since we need only the remainder
              accum_window_d = {1'd0, 32'(next_remainder)};
              md_state_d     = MD_CHANGE_SIGN;
              /*break;*/
            end
            default: /*break;*/
          endcase
          /*break;*/
        end

        MD_CHANGE_SIGN: begin
          md_state_d = MD_FINISH;
          case (operator_i) 
            MD_OP_DIV: begin
              accum_window_d = div_change_sign ? cat(b1(0), alu_adder_i) : accum_window_q;
              /*break;*/
            end
            MD_OP_REM: begin
              accum_window_d = rem_change_sign ? cat(b1(0), alu_adder_i) : accum_window_q;
              /*break;*/
            end
            default: /*break;*/;
          endcase
          /*break;*/
        end

        MD_FINISH : begin
          // Note no state transition will occur if multdiv_hold is set
          md_state_d   = MD_IDLE;
          multdiv_hold = ~multdiv_ready_id_i;
          /*break;*/
        end

        default: begin
          md_state_d = MD_IDLE;
          /*break;*/
        end
      endcase // md_state_q
    end // (mult_sel_i || div_sel_i)
  end

  //////////////////////////////////////////
  // Mutliplier / Divider state registers //
  //////////////////////////////////////////

  always_ff @(posedge clk, negedge rst_n) begin : tick
    if (!rst_n) begin
      multdiv_count_q  <= 5'd0;
      op_b_shift_q     <= 33'd0;
      op_a_shift_q     <= 33'd0;
      md_state_q       = MD_IDLE;
      div_by_zero_q    = 1'd0;
    end else if (multdiv_en) begin
      multdiv_count_q  <= multdiv_count_d;
      op_b_shift_q     <= op_b_shift_d;
      op_a_shift_q     <= op_a_shift_d;
      md_state_q       = md_state_d;
      div_by_zero_q    = div_by_zero_d;
    end
  end
  

  /////////////
  // Outputs //
  /////////////

  always_comb begin : tock3
    valid_o = 1'((md_state_q == MD_FINISH) ||
      (md_state_q == MD_LAST && (operator_i == MD_OP_MULL || operator_i == MD_OP_MULH)));

    multdiv_result_o = div_en_i ? b32(accum_window_q) : b32(res_adder_l);
  end


endmodule