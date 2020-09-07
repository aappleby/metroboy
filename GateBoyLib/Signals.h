#pragma once
#include "CoreLib/Types.h"

#pragma warning(disable : 5054) // or'ing different enums deprecated

//-----------------------------------------------------------------------------

enum RegState : uint8_t {
  REG_D0C0 = 0b0000, // 00: state 0 + clock 0
  REG_D1C0 = 0b0001, // 01: state 1 + clock 0
  REG_D0C1 = 0b0010, // 02: state 0 + clock 1
  REG_D1C1 = 0b0011, // 03: state 1 + clock 1
  SIG_0000 = 0b0100, // 04: signal driven low
  SIG_1111 = 0b0101, // 05: signal driven high

  TRI_D0PD = 0b0110, // 06: pin driven 0 + pull down
  TRI_D1PD = 0b0111, // 07: pin driven 1 + pull down
  TRI_D0PU = 0b1000, // 08: pin driven 0 + pull up
  TRI_D1PU = 0b1001, // 09: pin driven 1 + pull up
  TRI_D0NP = 0b1010, // 10: pin driven 0 + no pull
  TRI_D1NP = 0b1011, // 11: pin driven 1 + no pull

  TRI_HZPD = 0b1100, // 12: pin driven Z + pull down
  TRI_HZPU = 0b1101, // 13: pin driven Z + pull up
  TRI_HZNP = 0b1110, // 14: pin driven Z + no pull

  ERR_XXXX = 0b1111, // 15: combined error state
};

//-----------------------------------------------------------------------------

enum RegDelta : uint8_t {
  DELTA_NONE = 0b0000, // 00: delta not set yet
  DELTA_XXXX = 0b0001, // 01: error
  DELTA_HOLD = 0b0010, // 02: do not change tri when committed, used for latches and config bits
  DELTA_LOCK = 0b0011, // 03: do not change tri when committed, sticky. used for buses.
  DELTA_TRIZ = 0b0100, // 04: 
  DELTA_TRI1 = 0b0101, // 05: 
  DELTA_TRI0 = 0b0110, // 06: 
  DELTA_TRIX = 0b0111, // 07: 
  DELTA_D0C0 = 0b1000, // 08: data 0    + clock 0
  DELTA_D1C0 = 0b1001, // 09: data 1    + clock 0
  DELTA_D0C1 = 0b1010, // 10: data 0    + clock 1
  DELTA_D1C1 = 0b1011, // 11: data 1    + clock 1
  DELTA_A0C0 = 0b1100, // 12: async rst + clock 0
  DELTA_A1C0 = 0b1101, // 13: async set + clock 0
  DELTA_A0C1 = 0b1110, // 14: async rst + clock 1
  DELTA_A1C1 = 0b1111, // 15: async set + clock 1
};

struct RegQNIn { RegDelta d; };
struct RegQPNIn { RegDelta d; };
struct RegQPIn { RegDelta d; };

//-----------------------------------------------------------------------------

struct Lut8 {
  inline uint8_t operator[](int x) const { return tab[x]; }
  uint8_t tab[256];
};

extern const Lut8 logic_lut1;

//-----------------------------------------------------------------------------

void combine_hash(uint64_t& a, uint64_t b);
void commit_and_hash(void* blob, int size, uint64_t& hash_regs);

template<typename T>
inline void commit_and_hash(T& obj, uint64_t& hash_regs) {
  commit_and_hash(&obj, sizeof(T), hash_regs);
}

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201)

struct RegBase {
  RegBase() = delete;
  RegBase& operator=(const RegBase&) = delete;
  RegBase(RegState s) : state(s), delta(DELTA_NONE) {}

  static bool sim_running;
  static bool tick_running;
  static bool tock_running;
  static bool bus_collision;
  static bool bus_floating;

  char c() const {
    switch(state) {
      case REG_D0C0: return '0';
      case REG_D1C0: return '1';
      case REG_D0C1: return '0';
      case REG_D1C1: return '1';
      case SIG_0000: return '0';
      case SIG_1111: return '1';
      case TRI_D0PD: return '0';
      case TRI_D1PD: return '1';
      case TRI_D0PU: return '0';
      case TRI_D1PU: return '1';
      case TRI_D0NP: return '0';
      case TRI_D1NP: return '1';
      case TRI_HZPD: return 'v';
      case TRI_HZPU: return '^';
      case TRI_HZNP: return 'Z';
      default:       return 'E';
    }
  }

  char cn() const {
    switch(state) {
      case REG_D0C0: return '1';
      case REG_D1C0: return '0';
      case REG_D0C1: return '1';
      case REG_D1C1: return '0';
      case SIG_0000: return '1';
      case SIG_1111: return '0';
      case TRI_D0PD: return '1';
      case TRI_D1PD: return '0';
      case TRI_D0PU: return '1';
      case TRI_D1PU: return '0';
      case TRI_D0NP: return '1';
      case TRI_D1NP: return '0';
      case TRI_HZPD: return '^';
      case TRI_HZPU: return 'v';
      case TRI_HZNP: return 'Z';
      default:       return 'E';
    }
  }

  inline bool is_reg()    const { return (state >= REG_D0C0) && (state <= REG_D1C1); }
  inline bool is_sig()    const { return (state >= SIG_0000) && (state <= SIG_1111); }
  inline bool is_tri()    const { return (state >= TRI_D0PD) && (state <= TRI_HZNP); }
  inline bool has_delta() const { return delta != DELTA_NONE; }
  inline wire as_wire()   const { /*CHECKn(has_delta());*/ return wire(state & 1); }

  union {
    struct {
      RegState state : 4;
      RegDelta delta : 4;
    };
    uint8_t value;
  };
};

#pragma warning(pop)

//-----------------------------------------------------------------------------

struct Reg : private RegBase {
  Reg(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire qp()  const { return  as_wire(); }
  inline wire qn() const { return !as_wire(); }
  inline wire clk() const { return wire(state & 0x02); }

  inline wire posedge() const {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    return !(old_v & 1) && (new_v & 1);
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_reg()); // must be state state
    CHECK_N(d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = d;
  }
};

//-----------------------------------------------------------------------------
// 8-rung register with no reset, inverting input, and dual outputs. Used by
// sprite store, bg pix a, spr pix a/b, dma hi, bus mux sprite temp

// REG8_01 |o------O | << CLKn
// REG8_02 |====O====| << Dn
// REG8_03 |  -----  |
// REG8_04 |O-------o| << CLKp
// REG8_05 |  -----  |
// REG8_06 |==     ==|
// REG8_07 |xxx-O-xxx| >> Qn
// REG8_08 |xxx-O-xxx| >> Q  or this rung can be empty

struct DFF8 : private RegBase {
  DFF8() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire q07() const { return !as_wire(); }
  inline wire q08() const { return  as_wire(); }

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKn, bool Dn) {
    CHECK_P(is_reg() && !has_delta());
    delta = RegDelta(DELTA_D0C0 | ((!CLKn) << 1) | ((!Dn) << 0));
  }

  inline void tock(wire CLKn, wire CLKp, bool Dn) {
    (void)CLKn;
    CHECK_P(is_reg() && !has_delta());
    delta = RegDelta(DELTA_D0C0 | ((CLKp) << 1) | ((!Dn) << 0));
  }
};

inline RegQPIn dff8_B_inv(wire CLKp, bool D) {
  return {RegDelta(DELTA_D0C0 | ((!CLKp) << 1) | ((!D) << 0))};
}

//-----------------------------------------------------------------------------
// 9-rung register with reset, inverting input, and dual outputs. Looks like
// Reg8 with a hat and a belt. Used by clock phase (CHECK), LYC, BGP, OBP0,
// OBP1, stat int enable, sprite store, SCY, SCX, LCDC, WX, WY.

// REG9_01 | O===--o | 
// REG9_02 |==--O====| << CLKp
// REG9_03 | ------- | << D
// REG9_04 |o-------O| << CLKn
// REG9_05 |  -----  | 
// REG9_06 |--xxOxx--| << RSTn
// REG9_07 |o-------o| 
// REG9_08 |xxx-O-xxx| >> Qn
// REG9_09 |xxx-O-xxx| >> Q

struct DFF9 : private RegBase {
  DFF9() : RegBase(REG_D0C0) {}

  using RegBase::c;

  inline wire q08() const { return !as_wire(); }
  inline wire q09() const { return  as_wire(); }

  inline wire qn() const { return !as_wire(); }
  inline wire qp() const { return  as_wire(); }

  inline void tock(wire CLKp, wire RSTn, bool D) {
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A1C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0));
    }
  }

  inline void tock(wire CLKp, wire CLKn, wire RSTn, bool D) {
    (void)CLKn;
    CHECK_P(is_reg() && !has_delta());
    if (!RSTn) {
      delta = RegDelta(DELTA_A1C0 | (CLKp << 1));
    }
    else {
      delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0));
    }
  }
};

inline RegDelta dff9_inv(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return RegDelta(DELTA_A1C0 | (CLKp << 1));
  }
  else {
    return RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0));
  }
}

inline RegQNIn dff9_A_inv(wire CLKp, wire RSTn, wire D) {
  if (!RSTn) {
    return {RegDelta(DELTA_A1C0 | (CLKp << 1))};
  }
  else {
    return {RegDelta(DELTA_D0C0 | (CLKp << 1) | ((!D) << 0))};
  }
}

//-----------------------------------------------------------------------------

struct RegQP : private RegBase {
  RegQP(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire qp()  const { return  as_wire(); }
  inline wire clk() const { return wire(state & 0x02); }

  inline wire posedge() const {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    return !(old_v & 1) && (new_v & 1);
  }

  inline void operator = (RegQPIn in) {
    CHECK_P(is_reg()); // must be state state
    CHECK_N(in.d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = in.d;
  }
};

//-----------------------------------------------------------------------------

struct RegQN : private RegBase {
  RegQN(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire qn() const { return !as_wire(); }
  inline wire clk() const { return wire(state & 0x02); }

  inline wire posedge() const {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    return !(old_v & 1) && (new_v & 1);
  }

  inline void operator = (RegQNIn in) {
    CHECK_P(is_reg()); // must be state state
    CHECK_N(in.d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = in.d;
  }
};

//-----------------------------------------------------------------------------

struct RegQPN : private RegBase {
  RegQPN(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire qp() const { return  as_wire(); }
  inline wire qn() const { return !as_wire(); }
  inline wire clk() const { return wire(state & 0x02); }

  inline wire posedge() const {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    return !(old_v & 1) && (new_v & 1);
  }

  inline void operator = (RegQPNIn in) {
    CHECK_P(is_reg()); // must be state state
    CHECK_N(in.d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = in.d;
  }
};

//-----------------------------------------------------------------------------

struct Sig : private RegBase {
  Sig() : RegBase(SIG_0000) {}

  using RegBase::c;

  inline wire qp() const { return  as_wire(); }

  inline operator wire() const { return as_wire(); }

  inline bool as_wire() const {
    CHECK_P(is_sig());

    // FIXME doing this check in release mode for sanity, but probably need to remove it later
    //CHECK_P(has_delta() == sim_running);
    if (has_delta() != sim_running) __debugbreak();

    return wire(state & 1);
  }

  inline void operator = (wire s) {
    CHECK_P(is_sig());
    CHECK_N(has_delta());

    // FIXME doing this check in release mode for sanity, but probably need to remove it later
    if (!tick_running) __debugbreak();
    
    state = RegState(SIG_0000 | int(s));
    delta = s ? DELTA_TRI1 : DELTA_TRI0;
  }
};

//-----------------------------------------------------------------------------

struct Tri : private RegBase {
  Tri(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire tp()  const {
    //CHECK_N(state == TRI_HZNP);
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return  as_wire();
  }
  //inline wire qn() const { return !as_wire(); }

  inline void operator = (wire w)  { (*this) = w ? DELTA_TRI1 : DELTA_TRI0; }

  inline void lock(wire w) {
    CHECK_P(delta == DELTA_NONE || delta == DELTA_LOCK);
    delta = w ? DELTA_TRI1 : DELTA_TRI0;
    value = logic_lut1[value];
    delta = DELTA_LOCK;
  }

  inline void preset(RegDelta d) {
    CHECK_P(delta == DELTA_NONE);
    delta = d;
    value = logic_lut1[value];
    delta = d;
  }

  inline void preset(wire d) {
    preset(d ? DELTA_TRI1 : DELTA_TRI0);
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------

struct Bus : private RegBase {
  Bus(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;
  using RegBase::cn;

  inline wire tp()  const {
    //CHECK_N(state == TRI_HZNP);
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return  as_wire();
  }
  //inline wire qn() const { return !as_wire(); }

  inline void operator = (wire w)  {
    (*this) = w ? DELTA_TRI1 : DELTA_TRI0;
  }

  inline void lock(wire w) {
    CHECK_P(delta == DELTA_NONE || delta == DELTA_LOCK);
    delta = w ? DELTA_TRI1 : DELTA_TRI0;
    value = logic_lut1[value];
    delta = DELTA_LOCK;
  }

  inline void preset(RegDelta d) {
    CHECK_P(delta == DELTA_NONE);
    delta = d;
    value = logic_lut1[value];
    delta = d;
  }

  inline void preset(wire d) {
    preset(d ? DELTA_TRI1 : DELTA_TRI0);
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------

struct Pin : private RegBase {
  Pin(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;

  inline wire qp()  const {
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return as_wire();
  }

  inline wire qn()  const {
    if (state == TRI_HZNP) {
      bus_floating = true;
    }
    return !as_wire();
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      __debugbreak();
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------

struct Latch : private RegBase {
  Latch(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;
  using RegBase::cn;

  // adding Q/Qn here because latches can have both inverting and
  // non-inverting outputs
  inline wire qp()  const { return  as_wire(); }
  inline wire qn() const { return !as_wire(); }

  inline void operator = (wire w)  { (*this) = w ? DELTA_TRI1 : DELTA_TRI0; }

  inline void operator = (RegDelta d) {
    CHECK_P(is_tri());

    if (delta == DELTA_NONE) {
      delta = d;
    }
    else if (delta == DELTA_HOLD) {
      CHECK_P(d == DELTA_TRIZ);
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(d == DELTA_TRIZ || d == DELTA_TRI0 || d == DELTA_TRI1);
      delta = d;
    }
    else {
      //CHECK_P(d == DELTA_TRIZ);
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
  }
};

//-----------------------------------------------------------------------------

static_assert(sizeof(Reg) == 1, "Reg size != 1");
static_assert(sizeof(Sig) == 1, "Sig size != 1");
static_assert(sizeof(Tri) == 1, "Tri size != 1");

//-----------------------------------------------------------------------------
