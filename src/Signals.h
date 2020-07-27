#pragma once
#include "Types.h"
#include <stdlib.h>

#pragma warning(disable : 5054) // or'ing different enums deprecated

//-----------------------------------------------------------------------------

enum RegState : uint8_t {
  REG_D0C0 = 0b0000, // 00: reg 0 + clock 0
  REG_D1C0 = 0b0001, // 01: reg 1 + clock 0
  REG_D0C1 = 0b0010, // 02: reg 0 + clock 1
  REG_D1C1 = 0b0011, // 03: reg 1 + clock 1
  SIG_0000 = 0b0100, // 04: signal driven low
  SIG_1111 = 0b0101, // 05: signal driven high
  PIN_D0PD = 0b0110, // 06: pin driven 0 + pull down
  PIN_D1PD = 0b0111, // 07: pin driven 1 + pull down
  PIN_D0PU = 0b1000, // 08: pin driven 0 + pull up
  PIN_D1PU = 0b1001, // 09: pin driven 1 + pull up
  PIN_D0NP = 0b1010, // 10: pin driven 0 + no pull
  PIN_D1NP = 0b1011, // 11: pin driven 1 + no pull
  PIN_HZPD = 0b1100, // 12: pin driven Z + pulled down
  PIN_HZPU = 0b1101, // 13: pin driven Z + pulled up
  PIN_HZNP = 0b1110, // 14: pin driven Z + floating
  ERR_XXXX = 0b1111, // 15: combined error state
};

//-----------------------------------------------------------------------------

enum RegDelta : uint8_t {
  DELTA_NONE = 0b0000, // 00: delta not set yet
  DELTA_HOLD = 0b0001, // 01: delta hold, do not change register when committed, sticky
  DELTA_PASS = 0b0010, // 02: delta pass, do not change register when committed, not sticky
  DELTA_SSSS = 0b0011, // 03: meaningless, free slot
  DELTA_SIGZ = 0b0100, // 04: 
  DELTA_SIG1 = 0b0101, // 05: 
  DELTA_SIG0 = 0b0110, // 06: 
  DELTA_SIGX = 0b0111, // 07: 
  DELTA_D0C0 = 0b1000, // 08: delta 0 + clock 0
  DELTA_D1C0 = 0b1001, // 09: delta 1 + clock 0
  DELTA_D0C1 = 0b1010, // 10: delta 0 + clock 1
  DELTA_D1C1 = 0b1011, // 11: delta 1 + clock 1
  DELTA_A0C0 = 0b1100, // 12: async reset + clock 0
  DELTA_A1C0 = 0b1101, // 13: async set + clock 0
  DELTA_A0C1 = 0b1110, // 14: async reset + clock 1
  DELTA_A1C1 = 0b1111, // 15: async set + clock 1
};

//-----------------------------------------------------------------------------

struct Lut8 {
  inline uint8_t operator[](int x) const { return tab[x]; }
  uint8_t tab[256];
};

extern const Lut8 logic_lut1;

//-----------------------------------------------------------------------------

inline void mix(uint64_t& h) {
  h *= 0xff51afd7ed558ccd;
  h = _byteswap_uint64(h);
}

inline void combine_hash(uint64_t& a, uint64_t b) {
  a ^= b;
  mix(a);
}

//-----------------------------------------------------------------------------

constexpr uint64_t HASH_INIT = 0x12345678;

inline void commit_and_hash(void* blob, int size, uint64_t& hash_regs) {
  uint64_t h = hash_regs;

  uint8_t* base = (uint8_t*)blob;

  for (int i = 0; i < size; i++) {
    uint8_t s1 = base[i];

    if ((s1 & 0x0F) == ERR_XXXX) {
      __debugbreak();
    }

    uint8_t s2 = logic_lut1[s1];

    if ((s2 & 0x0F) == ERR_XXXX) {
      __debugbreak();
    }

    if ((s1 & 0x0F) != (s2 & 0x0F)) {
      int x = 1;
      x++;
    }

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

struct RegBase2 {
  RegBase2() = delete;
  RegBase2& operator=(const RegBase2&) = delete;
  RegBase2(RegState r, RegDelta d) : reg(r), delta(d) {}

  char c() const {
    switch(reg) {
      case REG_D0C0: return '0';
      case REG_D1C0: return '1';
      case REG_D0C1: return '0';
      case REG_D1C1: return '1';
      case SIG_0000: return '0';
      case SIG_1111: return '1';
      case PIN_D0PD: return '0';
      case PIN_D1PD: return '1';
      case PIN_D0PU: return '0';
      case PIN_D1PU: return '1';
      case PIN_D0NP: return '0';
      case PIN_D1NP: return '1';
      case PIN_HZPD: return 'v';
      case PIN_HZPU: return '^';
      case PIN_HZNP: return 'Z';
      default:       return 'E';
    }
  }

  char cn() const {
    switch(reg) {
      case REG_D0C0: return '1';
      case REG_D1C0: return '0';
      case REG_D0C1: return '1';
      case REG_D1C1: return '0';
      case SIG_0000: return '1';
      case SIG_1111: return '0';
      case PIN_D0PD: return '1';
      case PIN_D1PD: return '0';
      case PIN_D0PU: return '1';
      case PIN_D1PU: return '0';
      case PIN_D0NP: return '1';
      case PIN_D1NP: return '0';
      case PIN_HZPD: return '^';
      case PIN_HZPU: return 'v';
      case PIN_HZNP: return 'Z';
      default:       return 'E';
    }
  }

  inline bool is_reg()    const { return (reg >= REG_D0C0) && (reg <= REG_D1C1); }
  inline bool is_sig()    const { return (reg >= SIG_0000) && (reg <= SIG_1111); }
  inline bool is_pin()    const { return (reg >= PIN_D0PD) && (reg <= PIN_HZNP); }
  inline bool is_held()   const { return delta == DELTA_HOLD; }
  inline bool has_delta() const { return delta != DELTA_NONE; }

  uint8_t reg : 4;
  uint8_t delta : 4;
};

//-----------------------------------------------------------------------------

struct Reg2 : public RegBase2 {
  Reg2(RegState r, RegDelta d) : RegBase2(r, d) {
    CHECK_P(is_reg());
    CHECK_P(delta == DELTA_NONE);
  }

  static const Reg2 D0C0;
  static const Reg2 D1C0;
  static const Reg2 D0C1;
  static const Reg2 D1C1;

  inline wire clk() {
    return wire(reg & 2);
  }

  inline void set_delta(bool CLKp, bool D) {
    CHECK_N(has_delta());
    delta = RegDelta(DELTA_D0C0 | (CLKp << 1) | (D << 0));
  }

  inline void hold(bool CLK, bool D) {
    CHECK_P(is_reg());

    reg = RegState(REG_D0C0 | (CLK << 1) | (D << 0));
    delta = DELTA_HOLD;
  }

  inline void hold_d(bool D) {
    CHECK_P(is_reg());

    reg = RegState((reg & 0b1110) | int(D));
    delta = DELTA_HOLD;
  }

  inline void hold_clk(bool clk) {
    CHECK_P(is_reg());

    reg = RegState((reg & 0b1101) | (clk << 1));
    delta = DELTA_HOLD;
  }

  inline void release() {
    CHECK_P(is_reg());
    if (delta == DELTA_HOLD) delta = DELTA_NONE;
  }

  inline void preset_a(bool a) {
    reg = RegState((reg & 0b1110) | uint8_t(a));
  }

  inline wire q() const {
    CHECK_P(is_reg());
    //CHECKn(has_delta()); // read-after-write
    return wire(reg & 1);
  }

  inline wire qn() const {
    CHECK_P(is_reg());
    //CHECKn(has_delta()); // read-after-write
    return !wire(reg & 1);
  }

  inline wire as_wire() const  {
    CHECK_P(is_reg());
    //CHECKn(has_delta()); // read-after-write
    return wire(reg & 1);
  }

  inline void operator = (RegDelta d) {
    CHECK_P(is_reg()); // must be reg state
    CHECK_N(d == DELTA_NONE); // must not be invalid sig
    CHECK_N(has_delta());     // reg must not already be driven
    delta = d;
  }
};

//-----------------------------------------------------------------------------

struct Pin2 : public RegBase2 {
  Pin2(RegState r, RegDelta d) : RegBase2(r, d) {
    CHECK_P(is_pin());
    CHECK_P((d == DELTA_NONE) || (d == DELTA_HOLD));
  }
  
  static const Pin2 HIZ_NP;
  static const Pin2 HIZ_PU;
  static const Pin2 HIZ_PD;

  static const Pin2 HOLD_0;
  static const Pin2 HOLD_1;
  static const Pin2 HOLD_Z;

  static const Pin2 LATCH_0;
  static const Pin2 LATCH_1;

  inline operator wire()  const { return as_wire(); }
  inline wire q()         const { return as_wire(); }
  inline wire qn()        const { return !as_wire(); }

  inline void hold(bool D) {
    CHECK_P(is_pin());
    CHECK_P(is_held() || !has_delta());

    if ((reg == PIN_D0PD) || (reg == PIN_D1PD) || (reg == PIN_HZPD)) reg = D ? PIN_D1PD : PIN_D0PD;
    if ((reg == PIN_D0PU) || (reg == PIN_D1PU) || (reg == PIN_HZPU)) reg = D ? PIN_D1PU : PIN_D0PU;
    if ((reg == PIN_D0NP) || (reg == PIN_D1NP) || (reg == PIN_HZNP)) reg = D ? PIN_D1NP : PIN_D0NP;
    delta = DELTA_HOLD;
  }

  inline void hold_z() {
    CHECK_P(is_pin());
    CHECK_P(is_held() || !has_delta());

    if ((reg == PIN_D0PD) || (reg == PIN_D1PD) || (reg == PIN_HZPD)) reg = PIN_HZPD;
    if ((reg == PIN_D0PU) || (reg == PIN_D1PU) || (reg == PIN_HZPU)) reg = PIN_HZPU;
    if ((reg == PIN_D0NP) || (reg == PIN_D1NP) || (reg == PIN_HZNP)) reg = PIN_HZNP;
    delta = DELTA_HOLD;
  }

  inline void release() {
    CHECK_P(is_pin());
    //CHECK_P(is_held());
    delta = DELTA_NONE;
  }

  inline void pass() {
    CHECK_P(is_pin());
    //CHECK_P(is_held());
    delta = DELTA_PASS;
  }

  inline wire as_wire() const  {
    CHECK_P(is_pin());
    return wire(reg & 1);
  }

  inline void operator = (wire w) {
    CHECK_P(is_pin());
    CHECK_P(!has_delta());
    delta = w ? DELTA_SIG1 : DELTA_SIG0;
  }

  inline void operator = (Pin2& p) {
    (*this) = (wire)p;
  }

  inline void operator = (RegDelta D) {
    CHECK_P(is_pin());
    CHECK_P((delta == DELTA_NONE) || (delta == DELTA_SIGZ) || (D == DELTA_SIGZ));
    CHECK_P(D == DELTA_SIGZ || D == DELTA_SIG0 || D == DELTA_SIG1 || D == DELTA_PASS);
    delta = RegDelta(delta | D);
  }
};

//-----------------------------------------------------------------------------

struct Sig2 : public RegBase2 {
  Sig2() : RegBase2(SIG_0000, DELTA_NONE) {}
  
  inline operator wire() const {
    CHECK_P(is_sig());
    CHECK_P(has_delta());
    return wire(reg & 1);
  }

  inline void operator = (wire s) {
    CHECK_P(is_sig());
    CHECK_N(has_delta());

    reg = RegState(SIG_0000 | int(s));
    delta = s ? DELTA_SIG1 : DELTA_SIG0;
  }
};

//-----------------------------------------------------------------------------

static_assert(sizeof(Reg2) == 1, "Reg2 size != 1");
static_assert(sizeof(Pin2) == 1, "Pin2 size != 1");
static_assert(sizeof(Sig2) == 1, "Sig2 size != 1");

//-----------------------------------------------------------------------------
