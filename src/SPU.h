#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct SPU {
  void reset();

  void tock(int tphase, ubit16_t addr, ubit8_t data, bool read, bool write);

  char* dump(char* buf);

  //----------
  // output registers

  ubit9_t out_r;
  ubit9_t out_l;
  ubit8_t bus_out;
  bool    bus_oe;

private:

  void bus_read(ubit16_t addr);
  void bus_write(ubit16_t addr, ubit8_t data);

  //----------
  // internal stuff

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
  ubit8_t nr52;

  ubit14_t spu_clock;

  bool sound_on;
  ubit4_t volume_r;
  ubit4_t volume_l;
  ubit4_t s1l, s1r, s2l, s2r, s3l, s3r, s4l, s4r;

  //----------

  bool s1_enable;
  bool s2_enable;
  bool s3_enable;
  bool s4_enable;

  ubit4_t s1_volume;
  ubit4_t s2_volume;
  ubit4_t s3_volume;
  ubit4_t s4_volume;

  ubit3_t s1_sweep_period;
  bool    s1_sweep_dir;
  ubit3_t s1_sweep_shift;
  ubit3_t s1_sweep_clock;
  ubit11_t s1_freq;

  bool s1_length_enable;
  bool s2_length_enable;
  bool s3_length_enable;
  bool s4_length_enable;

  ubit7_t s1_length;
  ubit7_t s2_length;
  ubit9_t s3_length;
  ubit7_t s4_length;

  ubit4_t s1_env_volume;
  ubit4_t s2_env_volume;
  ubit4_t s4_env_volume;

  bool s1_env_dir;
  bool s2_env_dir;
  bool s4_env_dir;

  ubit3_t s1_env_period;
  ubit3_t s2_env_period;
  ubit3_t s4_env_period;

  ubit3_t s1_env_clock;
  ubit3_t s2_env_clock;
  ubit3_t s4_env_clock;

  ubit11_t s1_phase_clock;
  ubit11_t s2_phase_clock;
  ubit11_t s3_phase_clock;
  ubit11_t s4_phase_clock;

  ubit3_t s1_phase;
  ubit3_t s2_phase;
  ubit5_t s3_phase;
  ubit15_t s4_lfsr;

  ubit11_t s2_freq;
  ubit11_t s3_freq;

  ubit3_t s1_duty;
  ubit3_t s2_duty;
  bool s3_power;
  ubit3_t s3_volume_shift;
  ubit4_t s4_phase_period;
  ubit4_t s4_clock_shift;
  uint8_t s4_lfsr_mode;

  ubit4_t s1_out;
  ubit4_t s2_out;
  ubit4_t s3_out;
  ubit4_t s4_out;

  uint8_t  s3_wave[16];
};

//-----------------------------------------------------------------------------
