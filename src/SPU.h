#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct SpuOut {
  ubit8_t bus_out;
  bool    bus_oe;
  ubit9_t out_r;
  ubit9_t out_l;
};

struct SPU {
  void reset();

  SpuOut tock(int tphase, CpuBus bus);

  void  dump(std::string& out);

  const uint8_t* get_wave() { return s3_wave; }

  //----------
  // output registers


private:

  void bus_read(ubit16_t addr, SpuOut& ret);
  void bus_write(ubit16_t addr, ubit8_t data);

  //----------

  ubit8_t nr10;
  ubit8_t nr11;
  ubit8_t nr12;
  ubit8_t nr13;
  ubit8_t nr14;
  ubit8_t nr20;
  ubit8_t nr21;
  ubit8_t nr22;
  ubit8_t nr23;
  ubit8_t nr24;
  ubit8_t nr30;
  ubit8_t nr31;
  ubit8_t nr32;
  ubit8_t nr33;
  ubit8_t nr34;
  ubit8_t nr40;
  ubit8_t nr41;
  ubit8_t nr42;
  ubit8_t nr43;
  ubit8_t nr44;
  ubit8_t nr50;
  ubit8_t nr51;
  ubit8_t nr52; // FF26
  // FF27
  // FF28
  // FF29
  // FF2A
  // FF2B
  // FF2C
  // FF2D
  // FF2E
  // FF2F

  uint8_t  s3_wave[16]; // FF30
  ubit15_t s4_lfsr;

  ubit14_t spu_clock;

  bool s1_enable;
  bool s2_enable;
  bool s3_enable;
  bool s4_enable;

  ubit3_t s1_sweep_clock;
  ubit11_t s1_sweep_freq;

  ubit7_t s1_duration;
  ubit7_t s2_duration;
  ubit9_t s3_duration;
  ubit7_t s4_duration;

  ubit3_t s1_env_clock;
  ubit3_t s2_env_clock;
  ubit3_t s4_env_clock;
  ubit4_t s1_env_volume;
  ubit4_t s2_env_volume;
  ubit4_t s4_env_volume;

  ubit11_t s1_phase_clock;
  ubit11_t s2_phase_clock;
  ubit11_t s3_phase_clock;
  ubit11_t s4_phase_clock;
  ubit3_t s1_phase;
  ubit3_t s2_phase;
  ubit5_t s3_phase;

  ubit4_t s1_out;
  ubit4_t s2_out;
  ubit4_t s3_out;
  ubit4_t s4_out;
};

//-----------------------------------------------------------------------------
