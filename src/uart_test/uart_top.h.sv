//--------------------------------------------------------------------------------
// MODULE:       uart_top
// MODULEPARAMS: cycles_per_bit, 
// INPUTS:       
// OUTPUTS:      o_serial, o_data, o_valid, o_done, o_sum, o_onehot, 
// LOCALPARAMS:  
// FIELDS:       temp, opcode_e, 
// SUBMODULES:   hello, tx, rx, cdec, prim_arbiter, 
// TASKS:        
// FUNCTIONS:    
/* verilator lint_off WIDTH */
`default_nettype none

`include "metron.h.sv"
`include "uart_rx.h.sv"
`include "uart_tx.h.sv"
`include "uart_hello.h.sv"
`include "ibex_compressed_decoder.h.sv"
`include "prim_arbiter_fixed.h.sv"

//==============================================================================


module uart_top
#(parameter int cycles_per_bit = 'd3)
(clk, rst_n, o_serial, o_data, o_valid, o_done, o_sum, o_onehot);
  /*verilator public_module*/
  
  input logic clk;
  input logic rst_n; 

  bool hello_i_cts;
  bool hello_i_idle;
  logic[7:0] hello_o_data;
  logic hello_o_req;
  logic hello_o_done;
  uart_hello hello(clk, rst_n, hello_i_cts, hello_i_idle, hello_o_data, hello_o_req, hello_o_done);
  
  logic[7:0] tx_i_data;
  logic tx_i_req;
  logic tx_o_serial;
  logic tx_o_cts;
  logic tx_o_idle;
  uart_tx #(cycles_per_bit) tx(clk, rst_n, tx_i_data, tx_i_req, tx_o_serial, tx_o_cts, tx_o_idle);
  
  logic rx_i_serial;
  logic[7:0] rx_o_data;
  logic rx_o_valid;
  logic[31:0] rx_o_sum;
  uart_rx #(cycles_per_bit) rx(clk, rst_n, rx_i_serial, rx_o_data, rx_o_valid, rx_o_sum);
  

  logic cdec_valid_i;
  logic[31:0] cdec_instr_i;
  logic  cdec_is_compressed_o;
  logic  cdec_illegal_instr_o;
  logic[31:0] cdec_instr_o;
  ibex_compressed_decoder cdec(clk, rst_n, cdec_valid_i, cdec_instr_i, cdec_is_compressed_o, cdec_illegal_instr_o, cdec_instr_o);
  

  logic[8-1:0]    prim_arbiter_gnt_o;
  logic[3-1:0] prim_arbiter_idx_o;
  logic    prim_arbiter_valid_o;
  logic[32-1:0]   prim_arbiter_data_o;
  prim_arbiter_fixed #('d8, 'd32, 'd1, 'd3) prim_arbiter(clk, rst_n, prim_arbiter_gnt_o, prim_arbiter_idx_o, prim_arbiter_valid_o, prim_arbiter_data_o);
  

  output logic o_serial;
  output logic[7:0] o_data;
  output logic o_valid;
  output logic o_done;
  output logic[31:0] o_sum;
  output logic[7:0] o_onehot;

  logic[7:0] temp;

  typedef enum {
    OPCODE_LOAD = 'h03,
    OPCODE_MISC_MEM = 'h0f,
    OPCODE_OP_IMM = 'h13,
    OPCODE_AUIPC = 'h17,
    OPCODE_STORE = 'h23,
    OPCODE_OP = 'h33,
    OPCODE_LUI = 'h37,
    OPCODE_BRANCH = 'h63,
    OPCODE_JALR = 'h67,
    OPCODE_JAL = 'h6f,
    OPCODE_SYSTEM = 'h73
  } opcode_e;

  //----------------------------------------

  initial begin : INIT
    $write("uart_top.init()\n");

    /*hello.init()*/;
    /*tx.init()*/;
    /*rx.init()*/;

    o_serial = 'd0;
    o_data = 'd0;
    o_valid = 'd0;
    o_done = 'd0;
    o_sum = 'd0;
    o_onehot = 'd0;

    temp = 'd0;
  end

  //----------------------------------------

  always_ff @(posedge clk, negedge rst_n) begin : TICK
    /*hello.tick(rst_n, tx.o_cts, tx.o_idle)*/;
    /*tx.tick(rst_n, hello.o_data, hello.o_req)*/;
    /*rx.tick(rst_n, tx.o_serial)*/;

    if (!rst_n) begin
    end
    else begin
      logic[31:0] instr_i;
      logic[31:0] instr_o;
      logic[31:0] blep;
      instr_i = 'h12345678;
      

      /*
      instr_o = cat(
        bx<2>(0b00),
        bx<4>(instr_i, 10, 7),
        bx<2>(instr_i, 12, 11),
        bx<1>(instr_i, 5),
        bx<1>(instr_i, 6),
        bx<2>(0b00),
        bx<5>(0x02),
        bx<3>(0b000),
        bx<2>(0b01),
        bx<3>(instr_i, 4, 2),
        bx<7>(OPCODE_OP_IMM)
      );
      */

      instr_o = {
        {6 {instr_i[12]}},
        instr_i[12],
        instr_i[6:2],
        instr_i[11:7],
        3'd0,
        instr_i[11:7],
        OPCODE_OP_IMM[6:0]
      };

      

      if (instr_i[6:2] != 5'd0) begin
        blep = 'd1;
      end
      else begin
        blep = 'd0;
      end


      case (o_data & 'b111) 
      'd0:  begin temp <= 'b00000001; /*break;*/ end
      'd1:  begin temp <= 'b00000010; /*break;*/ end
      'd2:  begin temp <= 'b00000100; /*break;*/ end
      'd3:  begin temp <= 'b00001000; /*break;*/ end
      'd4:  begin temp <= 'b00010000; /*break;*/ end
      'd5:  begin temp <= 'b00100000; /*break;*/ end
      'd6:  begin temp <= 'b01000000; /*break;*/ end
      'd7:  begin temp <= 'b10000000; /*break;*/ end
      default: begin temp <= 'b00000000; /*break;*/ end
      endcase
    end
  end
  
  assign hello_i_cts = tx_o_cts;
  assign hello_i_idle = tx_o_idle;
  assign tx_i_data = hello_o_data;
  assign tx_i_req = hello_o_req;
  assign rx_i_serial = tx_o_serial;

  //----------------------------------------

  always_comb begin : TOCK
    logic blah;
    /*hello.tock(rst_n)*/;
    /*tx.tock(rst_n)*/;
    /*rx.tock(rst_n)*/;

    o_serial = tx_o_serial;
    o_data = rx_o_data;
    o_valid = rx_o_valid;

    

    blah = hello_o_done && tx_o_idle;

    o_done = blah;
    o_sum = rx_o_sum;

    case (o_data & 'b111) 
    'd0:  begin o_onehot = 'b00000001; /*break;*/ end
    'd1:  begin o_onehot = 'b00000010; /*break;*/ end
    'd2:  begin o_onehot = 'b00000100; /*break;*/ end
    'd3:  begin o_onehot = 'b00001000; /*break;*/ end
    'd4:  begin o_onehot = 'b00010000; /*break;*/ end
    'd5:  begin o_onehot = 'b00100000; /*break;*/ end
    'd6:  begin o_onehot = 'b01000000; /*break;*/ end
    'd7:  begin o_onehot = 'b10000000; /*break;*/ end
    default: begin o_onehot = 'b00000000; /*break;*/ end
    endcase
  end

endmodule

//==============================================================================
