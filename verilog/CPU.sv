`ifndef CPU_SV
`define CPU_SV
`include "types.sv"

import types::*;

`define JP_HL (op_ == 8'hE9)

//-----------------------------------------------------------------------------

module CPU(input bool clk,
           input bool rst);

  int model = 0;
  uint8_t int_ack_;
  uint8_t bus_data_;
  uint8_t imask_;
  uint8_t intf_;
  bool unhalt;

  uint16_t mem_addr_;
  bool mem_read_;
  bool mem_write_;
  uint8_t mem_out_;

  typedef enum {
    TAG_OPCODE,
    TAG_OPCODE_CB,
    TAG_ARG0,
    TAG_ARG1,

    TAG_DATA0,
    TAG_DATA1,

    TAG_NONE
  } MemTag;

  MemTag bus_tag, bus_tag_;
  uint16_t mem_addr;

  typedef enum {
    Z80_STATE_DECODE,
    Z80_STATE_HALT,

    Z80_STATE_DELAY_A,

    Z80_STATE_MEM_READ1,
    Z80_STATE_MEM_READ2,
    Z80_STATE_MEM_READ3,

    Z80_STATE_DELAY_D,

    Z80_STATE_MEM_WRITE1,
    Z80_STATE_MEM_WRITE2,

    Z80_STATE_DECODE_CB,
    Z80_STATE_MEM_READ_CB,
    Z80_STATE_MEM_WRITE_CB,

    Z80_STATE_DELAY_B,
    Z80_STATE_DELAY_C
  } Z80State;

  Z80State state, state_;

  uint16_t bc;
  uint16_t de;
  uint16_t hl;
  uint16_t af;
  uint16_t sp;
  uint16_t pc, pc_;
  uint8_t f_;

  uint8_t data_lo;
  uint8_t data_hi;

  uint8_t data_lo_;
  uint8_t data_hi_;

  uint16_t reg_in, reg_in_;

  uint8_t op_;
  uint8_t quad_;
  uint8_t row_;
  uint8_t col_;
  bool odd_row_;

  uint8_t cb_quad_;
  uint8_t cb_row_;
  uint8_t cb_col_;

  bool get_hl_;
  bool put_hl_;
  bool pop_d16_;
  bool push_d16_;
  bool fetch_d8_;
  bool fetch_d16_;
  bool any_read_;
  bool any_write_;
  bool take_branch_;

  // Interrupt stuff

  bool interrupt2;
  bool ime, ime_;
  bool ime_delay;

  uint8_t imask_latch;

  // Wires
  uint16_t alu_out_;

  // Misc

  uint32_t opcount;
  int cycle;

  //---------------------------------------------------------------------------

  task setup_decode();
    if (cycle == 0) begin
      pc_ <= pc;
    end

  if (interrupt2) begin
    // Someone could've changed the interrupt mask or flags while we were
    // handling the interrupt, so we have to compute the new PC at the very
    // last second.

    uint16_t actual_interrupt = 16'hFFFF;
    uint8_t interrupts = imask_latch & intf_;
    if ((interrupts & INT_JOYPAD) != 0) actual_interrupt = 4; // joypad
    if ((interrupts & INT_SERIAL) != 0) actual_interrupt = 3; // serial
    if ((interrupts & INT_TIMER) != 0)  actual_interrupt = 2; // timer
    if ((interrupts & INT_STAT) != 0)   actual_interrupt = 1; // lcd stat
    if ((interrupts & INT_VBLANK) != 0) actual_interrupt = 0; // vblank

    if (actual_interrupt != 16'hFFFF) begin
      //if (actual_interrupt == 0) printf("vblank interrupt!\n");
      //if (actual_interrupt == 1) printf("stat interrupt!\n");

      pc_ = uint16_t'(16'h0040 + (actual_interrupt << 3));
      int_ack_ = 1 << actual_interrupt;
    end
    else begin
      pc_ = 16'h0000;
    end
  end else if (take_branch_) begin
    if (`JP_HL) begin
      pc_ <= hl;
    end
  end


  endtask;

  //---------------------------------------------------------------------------

  task setup_halt();
    state_ <= Z80_STATE_HALT;
    bus_tag_ <= TAG_OPCODE;
    mem_addr_ <= pc;
    mem_read_ <= true;
    mem_write_ <= false;
    unhalt <= 0;
  endtask;

  task tock_halt();
  endtask;

  task tick_halt();
  endtask;

  //---------------------------------------------------------------------------

  task setup_decode_cb();
    state_ <= Z80_STATE_DECODE_CB;
    bus_tag_ <= TAG_OPCODE_CB;
    mem_addr_ <= pc + 1;
    mem_read_ <= true;
    mem_write_ <= false;
  endtask;

  task tock_decode_cb();
  endtask;

  task tick_decode_cb();
    uint8_t cb_opcode_ = bus_data_;

    cb_quad_ <= (cb_opcode_ >> 6) & 3;
    cb_row_ <= (cb_opcode_ >> 3) & 7;
    cb_col_ <= (cb_opcode_ >> 0) & 7;

    if (cb_col_ == 6) begin
      //setup_mem_read_cb();
    end else begin
      //tick_exec_cb();
      //setup_decode();
    end
  endtask;

  //-----------------------------------

  task setup_mem_read_cb();
    state_ <= Z80_STATE_MEM_READ_CB;
    bus_tag_ <= TAG_ARG1;
    mem_addr_ <= hl;
    mem_read_ <= true;
    mem_write_ <= false;
  endtask;

  task tock_mem_read_cb();
  endtask;

  task tick_mem_read_cb();
    //tick_exec_cb();
    if (cb_col_ == 6 && cb_quad_ != 1) begin
      //setup_mem_write_cb();
    end else begin
      //setup_decode();
    end;
  endtask;

  //---------------------------------------------------------------------------

  task reset();
  endtask;

  task tock();
  endtask;

  always @(posedge clk) begin
    if (rst) reset();
    else tock();
  end

endmodule;

//-----------------------------------------------------------------------------

`endif // CPU_SV