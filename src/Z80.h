#pragma once

//-----------------------------------------------------------------------------

struct Z80 {
  void reset(uint16_t new_pc);
  void tick_t0(uint8_t imask, uint8_t intf, uint8_t mem_out);
  void tock_t3();
  uint16_t get_pc() { return pc; }
  uint8_t get_a() { return a; }
  uint8_t get_op() { return op_; }
  char* dump(char* text_buf);

  uint16_t mem_addr_;
  bool mem_read_;
  bool mem_write_;
  uint8_t mem_out_;
  uint8_t int_ack_;
  uint8_t bus_data_;
  uint8_t imask_;
  uint8_t intf_;

//private:

  // What's on the bus?
  enum MemTag {
    TAG_OPCODE,
    TAG_OPCODE_CB,
    TAG_ARG0,
    TAG_ARG1,

    TAG_DATA0,
    TAG_DATA1,

    TAG_NONE,
  };

  MemTag bus_tag, bus_tag_;
  uint16_t mem_addr;

  //----------

  enum Z80State {
    Z80_STATE_DECODE = 0,
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
    Z80_STATE_DELAY_C,
  };

  Z80State state, state_;

  // Registers
  union { uint16_t bc; struct { uint8_t c; uint8_t b; }; };
  union { uint16_t de; struct { uint8_t e; uint8_t d; }; };
  union { uint16_t hl; struct { uint8_t l; uint8_t h; }; };
  union { uint16_t af; struct { uint8_t f; uint8_t a; }; };
  union { uint16_t sp; struct { uint8_t p; uint8_t s; }; };
  uint16_t pc, pc_;
  uint8_t f_;

  union {
    struct {
      uint8_t data_lo;
      uint8_t data_hi;
    };
    uint16_t data16;
  };

  union {
    struct {
      uint8_t data_lo_;
      uint8_t data_hi_;
    };
    uint16_t data16_;
  };

  // Signals

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

  //----------

private:

  void tick_halt();
  void tick_decode();
  void tick_delayA();
  void tick_mem_read1();
  void tick_mem_read2();
  void tick_mem_read3();
  void tick_delayD();
  void tick_mem_write1();
  void tick_mem_write2();
  void tick_decode_cb();
  void tick_mem_read_cb();
  void tick_mem_write_cb();
  void tick_delayB();
  void tick_delayC();

  void tock_halt();
  void tock_decode();
  void tock_delayA();
  void tock_mem_read1();
  void tock_mem_read2();
  void tock_mem_read3();
  void tock_delayD();
  void tock_mem_write1();
  void tock_mem_write2();
  void tock_decode_cb();
  void tock_mem_read_cb();
  void tock_mem_write_cb();
  void tock_delayB();
  void tock_delayC();

  void setup_halt();
  void setup_decode();
  void setup_delayA();
  void setup_mem_read1();
  void setup_mem_read2();
  void setup_mem_read3();
  void setup_delayD();
  void setup_mem_write1();
  void setup_mem_write2();
  void setup_decode_cb();
  void setup_mem_read_cb();
  void setup_mem_write_cb();
  void setup_delayB();
  void setup_delayC();

  void tick_exec();
  void tick_exec_cb();

  uint8_t add2(uint8_t x, uint8_t z, uint8_t f);
  uint8_t adc2(uint8_t x, uint8_t z, uint8_t f);
  uint8_t sub2(uint8_t z);
  uint8_t sbc2(uint8_t z);
  uint8_t and2(uint8_t z);
  uint8_t xor2(uint8_t z);
  uint8_t or2(uint8_t z);
  uint8_t cp2(uint8_t z);
  uint8_t inc2(uint8_t z);
  uint8_t dec2(uint8_t z);

  uint8_t rlc(uint8_t x);
  uint8_t rrc(uint8_t x);
  uint8_t rl(uint8_t x);
  uint8_t rr(uint8_t x);
  uint8_t daa(uint8_t a);
  uint8_t cpl(uint8_t x);
  uint8_t scf(uint8_t x);
  uint8_t ccf(uint8_t x);

  uint8_t sla(uint8_t x);
  uint8_t sra(uint8_t x);
  uint8_t swap(uint8_t x);
  uint8_t srl(uint8_t x);

  uint8_t bit(uint8_t x, uint8_t y);
  uint8_t res(uint8_t x, uint8_t y);
  uint8_t set(uint8_t x, uint8_t y);
};

//-----------------------------------------------------------------------------
