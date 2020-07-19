#pragma once
#include "Types.h"
#include <stdlib.h>

#pragma warning(disable : 5054) // or'ing different enums deprecated

//-----------------------------------------------------------------------------

enum RegMode : uint8_t {
  REG_D0C0 = 0b0000, // 00: reg 0 + clock 0
  REG_D1C0 = 0b0001, // 01: reg 1 + clock 0
  REG_D0C1 = 0b0010, // 02: reg 0 + clock 1
  REG_D1C1 = 0b0011, // 03: reg 1 + clock 1
  SIG_0000 = 0b0100, // 04: high signal
  SIG_1111 = 0b0101, // 05: low signal
  PIN_D0PD = 0b0110, // 06: pin driven 0 + pull down
  PIN_D1PD = 0b0111, // 07: pin driven 1 + pull down
  PIN_D0PU = 0b1000, // 08: pin driven 0 + pull up
  PIN_D1PU = 0b1001, // 09: pin driven 1 + pull up
  PIN_D0NP = 0b1010, // 10: pin driven 0 + no pull
  PIN_D1NP = 0b1011, // 11: pin driven 1 + no pull
  PIN_HZPD = 0b1100, // 12: pin pulled down
  PIN_HZPU = 0b1101, // 13: pin pulled up
  PIN_HZNP = 0b1110, // 14: pin floating
  ERR_XXXX = 0b1111, // 15: unused
};

//-----------------------------------------------------------------------------

enum Delta : uint8_t {
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

inline void combine_hash(uint64_t& a, uint64_t b) {
  a ^= b;
  a *= 0xff51afd7ed558ccd;
  a = _byteswap_uint64(a);
}

inline uint64_t commit_and_hash(void* blob, int size) {
  uint64_t h = 0x12345678;

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

    combine_hash(h, s2);

    base[i] = s2;
  }

  return h;
}

//-----------------------------------------------------------------------------

struct RegBase2 {
  RegBase2() = delete;
  RegBase2& operator=(const RegBase2&) = delete;
  RegBase2(RegMode r, Delta d) : reg(r), delta(d) {}

  inline bool is_reg()    const {
    return (reg >= REG_D0C0) && (reg <= REG_D1C1);
  }
  inline bool is_sig()    const { return (reg >= SIG_0000) && (reg <= SIG_1111); }
  inline bool is_pin()    const { return (reg >= PIN_D0PD) && (reg <= PIN_HZNP); }
  inline bool is_held()   const { return delta == DELTA_HOLD; }
  inline bool is_driven() const { return delta & 0b1000; }
  inline bool has_delta() const { return delta != DELTA_NONE; }

  RegMode reg : 4;
  Delta delta : 4;
};

//-----------------------------------------------------------------------------

struct Reg2 : public RegBase2 {
  Reg2(RegMode r, Delta d) : RegBase2(r, d) {
    CHECKp(is_reg());
    CHECKp(delta == DELTA_NONE);
  }

  static const Reg2 D0C0;
  static const Reg2 D1C0;
  static const Reg2 D0C1;
  static const Reg2 D1C1;

  inline void preset_a(bool a) {
    reg = RegMode((reg & 0b1110) | uint8_t(a));
  }

  inline wire q() const {
    CHECKp(is_reg());
    //CHECKn(has_delta()); // read-after-write
    return wire(reg & 1);
  }

  inline wire qn() const {
    CHECKp(is_reg());
    //CHECKn(has_delta()); // read-after-write
    return !wire(reg & 1);
  }

  inline wire as_wire() const  {
    CHECKp(is_reg());
    //CHECKn(has_delta()); // read-after-write
    return wire(reg & 1);
  }

  inline wire as_clock() const {
    CHECKp(is_reg());
    //CHECKn(has_delta()); // read-after-write
    return wire(reg & 2);
  }

  inline void operator = (Delta d) {
    CHECKp(is_reg()); // must be reg state
    CHECKn(d == DELTA_NONE); // must not be invalid sig
    CHECKn(has_delta());     // reg must not already be driven
    delta = d;
  }
};


//-----------------------------------------------------------------------------

struct Pin2 : public RegBase2 {
  Pin2(RegMode r, Delta d) : RegBase2(r, d) {
    CHECKp(is_pin());
    CHECKp((d == DELTA_NONE) || (d == DELTA_HOLD));
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
    CHECKp(is_pin());
    CHECKp(is_held());
    reg = RegMode((reg & 0b1110) | int(D));
  }

  inline void hold_z() {
    CHECKp(is_pin());
    CHECKp(is_held());
    reg = PIN_HZNP;
  }

  inline wire as_wire() const  {
    CHECKp(is_pin());
    return wire(reg & 1);
  }

  inline void operator = (wire w) {
    CHECKp(is_pin());
    CHECKp(!is_held());
    CHECKp(!is_driven());
    delta = w ? DELTA_SIG1 : DELTA_SIG0;
  }

  inline void operator = (Delta D) {
    CHECKp(is_pin());
    CHECKp(!is_held());
    CHECKp(!is_driven() || (D == DELTA_SIGZ));
    CHECKp(D == DELTA_SIGZ || D == DELTA_SIG0 || D == DELTA_SIG1 || D == DELTA_PASS);
    delta = Delta(delta | D);
  }
};

//-----------------------------------------------------------------------------

struct Sig2 : public RegBase2 {
  Sig2() : RegBase2(SIG_0000, DELTA_NONE) {}
  
  inline operator wire() const {
    CHECKp(is_sig());
    return wire(reg & 1);
  }

  inline void operator = (wire s) {
    CHECKp(is_sig());
    CHECKn(has_delta());

    reg = RegMode(SIG_0000 | int(s));
    delta = s ? DELTA_SIG1 : DELTA_SIG0;
  }
};

//-----------------------------------------------------------------------------

static_assert(sizeof(Reg2) == 1, "Reg2 size != 1");
static_assert(sizeof(Pin2) == 1, "Pin2 size != 1");
static_assert(sizeof(Sig2) == 1, "Sig2 size != 1");

//-----------------------------------------------------------------------------
