#pragma once
#include "Types.h"
#include <stdlib.h>
#include <stdio.h>

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
  DELTA_TTTT = 0b0001, // 01: unused slot
  DELTA_HOLD = 0b0010, // 02: do not change tri when committed, used for latches
  DELTA_SSSS = 0b0011, // 03: meaningless, free slot
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

//-----------------------------------------------------------------------------

struct Lut8 {
  inline uint8_t operator[](int x) const { return tab[x]; }
  uint8_t tab[256];
};

extern const Lut8 logic_lut1;

//-----------------------------------------------------------------------------

inline uint64_t mix(uint64_t h) {
  return _byteswap_uint64(h * 0xff51afd7ed558ccd);
}

inline uint64_t mix3(uint64_t h) {
  return mix(mix(mix(h)));
}

inline void combine_hash(uint64_t& a, uint64_t b) {
  a = mix(a ^ b);
}

//-----------------------------------------------------------------------------

constexpr uint64_t HASH_INIT = 0x12345678;

inline uint8_t commit(uint8_t s1) {
  if ((s1 & 0x0F) == ERR_XXXX) {
    __debugbreak();
  }

  uint8_t s2 = logic_lut1[s1];

  if ((s2 & 0x0F) == ERR_XXXX) {
    __debugbreak();
  }
  return s2;
}

inline void commit_and_hash(void* blob, int size, uint64_t& hash_regs) {
  uint64_t h = hash_regs;

  uint8_t* base = (uint8_t*)blob;

  for (int i = 0; i < size; i++) {
    uint8_t s2 = commit(base[i]);
    combine_hash(h, s2 & 0x0F);
    base[i] = s2;
  }

  hash_regs = h;
}

template<typename T>
inline void commit_and_hash(T& obj, uint64_t& hash_regs) {
  commit_and_hash(&obj, sizeof(T), hash_regs);
}

template<typename T>
inline uint64_t hash_obj(T const& obj) {
  uint64_t hash = HASH_INIT;
  const uint8_t* base = (const uint8_t*)(&obj);
  for (int i = 0; i < sizeof(T); i++) {
    uint8_t s2 = base[i];
    combine_hash(hash, s2);
  }
  return hash;
}

//-----------------------------------------------------------------------------

#pragma warning(push)
#pragma warning(disable:4201)

struct RegBase {
  RegBase() = delete;
  RegBase& operator=(const RegBase&) = delete;
  RegBase(RegState s) : state(s), delta(DELTA_NONE) {}

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

  inline wire posedge() const {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    return !(old_v & 1) && (new_v & 1);
  }

  inline wire negedge() const {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    return (old_v & 1) && !(new_v & 1);
  }

  void dump_edge(const char* name) {
    uint8_t old_v = value;
    uint8_t new_v = logic_lut1[value];
    CHECK_N(old_v == ERR_XXXX);
    CHECK_N(new_v == ERR_XXXX);
    if (!(old_v & 1) && (new_v & 1)) printf("%s ^^^\n", name);
    if ((old_v & 1) && !(new_v & 1)) printf("%s vvv\n", name);
  }

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
  using RegBase::posedge;
  using RegBase::negedge;
  using RegBase::dump_edge;

  inline wire q()  const { return  as_wire(); }
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

struct RegQPIn { RegDelta d; };

struct RegQP : private RegBase {
  RegQP(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;
  using RegBase::posedge;
  using RegBase::negedge;
  using RegBase::dump_edge;

  inline wire q()  const { return  as_wire(); }
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
    CHECK_N(d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = in.d;
  }
};

struct RegQNIn { RegDelta d; };

struct RegQN : private RegBase {
  RegQN(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;
  using RegBase::posedge;
  using RegBase::negedge;
  using RegBase::dump_edge;

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
    CHECK_N(d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = in.d;
  }
};

struct RegQPNIn { RegDelta d; };

struct RegQPN : private RegBase {
  RegQPN(RegState s) : RegBase(s) { CHECK_P(is_reg()); }

  using RegBase::c;
  using RegBase::cn;
  using RegBase::posedge;
  using RegBase::negedge;
  using RegBase::dump_edge;

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
    CHECK_N(d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // state must not already be driven
    delta = in.d;
  }
};


//-----------------------------------------------------------------------------

struct Sig : private RegBase {
  Sig() : RegBase(SIG_0000) {}

  using RegBase::c;
  //using RegBase::as_wire;

  inline wire q()  const { return  as_wire(); }
  inline wire qn() const { return !as_wire(); }

  static bool sim_running;

  inline operator wire() const { return as_wire(); }
  inline bool as_wire() const {
    CHECK_P(is_sig());
    CHECK_P(has_delta() == sim_running);
    return wire(state & 1);
  }

  inline void operator = (wire s) {
    CHECK_P(is_sig());
    CHECK_N(has_delta());

    state = RegState(SIG_0000 | int(s));
    delta = s ? DELTA_TRI1 : DELTA_TRI0;
  }
};

//-----------------------------------------------------------------------------

extern bool bus_collision;

struct Tri : private RegBase {
  Tri(RegState r) : RegBase(r) { CHECK_P(is_tri()); }

  using RegBase::c;
  using RegBase::cn;
  using RegBase::posedge;
  using RegBase::negedge;
  using RegBase::dump_edge;

  // adding Q/Qn here because latches can have both inverting and
  // non-inverting outputs
  inline wire q()  const { return  as_wire(); }
  inline wire qn() const { return !as_wire(); }

  //inline operator wire()  const { return as_wire(); }
  inline void operator = (wire w)  { (*this) = w ? DELTA_TRI1 : DELTA_TRI0; }


  inline void preset(RegDelta new_delta) {
    if (delta == DELTA_NONE) {
      CHECK_P(is_tri());
      delta = new_delta;
      value = logic_lut1[value];
      delta = new_delta;
      CHECK_P(is_tri());
    }
    else if (delta == DELTA_TRIZ) {
      CHECK_P(is_tri());
      delta = new_delta;
      value = logic_lut1[value];
      delta = new_delta;
      CHECK_P(is_tri());
    }
    else {
      CHECK_P(new_delta == DELTA_TRIZ);
    }
  }

  inline void preset(wire d) {
    preset(d ? DELTA_TRI1 : DELTA_TRI0);
  }

  inline void glitchy_assign(RegDelta d) {
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
      if (d != DELTA_TRIZ) {
        bus_collision = true;
      }
    }
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
      CHECK_P(d == DELTA_TRIZ);
      /*
      if (d != DELTA_TRIZ) {
        //printf("Bus collision!\n");
        bus_collision = true;
      }
      */
    }
  }
};

//-----------------------------------------------------------------------------

static_assert(sizeof(Reg) == 1, "Reg size != 1");
static_assert(sizeof(Sig) == 1, "Sig size != 1");
static_assert(sizeof(Tri) == 1, "Tri size != 1");

//-----------------------------------------------------------------------------
