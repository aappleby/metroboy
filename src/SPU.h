#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct SPU {
  void reset();

  void ibus_req(Req ibus_req);
  void ibus_ack(Ack& ibus_ack);

  sample_t get_l() const { return out_l; }
  sample_t get_r() const { return out_r; }
  void tock(const int tcycle_);

  void  dump(std::string& out) const;
  const uint8_t* get_wave() const { return s3_wave; }
  
private:

  int tcycle;

  void bus_read (Req ibus_req);
  void bus_write(Req ibus_req);
  Ack ack;

  //----------

  uint8_t nr10;
  uint8_t nr11;
  uint8_t nr12;
  uint8_t nr13;
  uint8_t nr14;
  uint8_t nr20;
  uint8_t nr21;
  uint8_t nr22;
  uint8_t nr23;
  uint8_t nr24;
  uint8_t nr30;
  uint8_t nr31;
  uint8_t nr32;
  uint8_t nr33;
  uint8_t nr34;
  uint8_t nr40;
  uint8_t nr41;
  uint8_t nr42;
  uint8_t nr43;
  uint8_t nr44;
  uint8_t nr50;
  uint8_t nr51;
  uint8_t nr52; // FF26
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
  uint16_t s4_lfsr;

  uint16_t spu_clock;

  bool s1_enable;
  bool s2_enable;
  bool s3_enable;
  bool s4_enable;

  uint8_t s1_sweep_clock;
  uint16_t s1_sweep_freq;

  uint8_t s1_duration;
  uint8_t s2_duration;
  uint16_t s3_duration;
  uint8_t s4_duration;

  uint8_t s1_env_clock;
  uint8_t s2_env_clock;
  uint8_t s4_env_clock;
  uint8_t s1_env_volume;
  uint8_t s2_env_volume;
  uint8_t s4_env_volume;

  uint16_t s1_phase_clock;
  uint16_t s2_phase_clock;
  uint16_t s3_phase_clock;
  uint16_t s4_phase_clock;
  uint8_t s1_phase;
  uint8_t s2_phase;
  uint8_t s3_phase;

  uint8_t s1_out;
  uint8_t s2_out;
  uint8_t s3_out;
  uint8_t s4_out;

  sample_t out_r;
  sample_t out_l;
};

//-----------------------------------------------------------------------------
