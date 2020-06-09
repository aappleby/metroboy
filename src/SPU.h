#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct SPU {
  void reset();

  void tock_req(const Req& req);
  void tick_ack(Ack& ibus_ack);

  sample_t get_l() const { return out_l; }
  sample_t get_r() const { return out_r; }
  void tock(const int phase);

  void  dump(std::string& out) const;
  const uint8_t* get_wave() const { return s3_wave; }
  
private:

  int tcycle = 0;

  void bus_read (const Req& ibus_req);
  void bus_write(const Req& ibus_req);
  Ack ack = {0};

  //----------

  uint8_t nr10 = 0;
  uint8_t nr11 = 0;
  uint8_t nr12 = 0;
  uint8_t nr13 = 0;

  uint8_t nr14 = 0;
  uint8_t nr20 = 0;
  uint8_t nr21 = 0;
  uint8_t nr22 = 0;
  
  uint8_t nr23 = 0;
  uint8_t nr24 = 0;
  uint8_t nr30 = 0;
  uint8_t nr31 = 0;
  
  uint8_t nr32 = 0;
  uint8_t nr33 = 0;
  uint8_t nr34 = 0;
  uint8_t nr40 = 0;
  
  uint8_t nr41 = 0;
  uint8_t nr42 = 0;
  uint8_t nr43 = 0;
  uint8_t nr44 = 0;
  
  uint8_t nr50 = 0;
  uint8_t nr51 = 0;
  uint8_t nr52 = 0;
  uint8_t pad2 = 0;

  uint8_t  s3_wave[16] = {0}; // FF30
  uint16_t s4_lfsr = 0;

  uint16_t spu_clock = 0;

  bool s1_enable = 0;
  bool s2_enable = 0;
  bool s3_enable = 0;
  bool s4_enable = 0;

  uint8_t s1_sweep_clock = 0;
  uint8_t pad1 = 0;
  uint16_t s1_sweep_freq = 0;

  uint8_t s1_duration = 0;
  uint8_t s2_duration = 0;
  uint16_t s3_duration = 0;

  uint8_t s4_duration = 0;
  uint8_t s1_env_clock = 0;
  uint8_t s2_env_clock = 0;
  uint8_t s4_env_clock = 0;
  
  uint8_t s1_env_volume = 0;
  uint8_t s2_env_volume = 0;
  uint8_t s4_env_volume = 0;
  uint8_t pad3 = 0;

  uint16_t s1_phase_clock = 0;
  uint16_t s2_phase_clock = 0;
  uint16_t s3_phase_clock = 0;
  uint16_t s4_phase_clock = 0;

  uint8_t s1_phase = 0;
  uint8_t s2_phase = 0;
  uint8_t s3_phase = 0;
  uint8_t s1_out = 0;

  uint8_t s2_out = 0;
  uint8_t s3_out = 0;
  uint8_t s4_out = 0;
  uint8_t pad4 = 0;

  sample_t out_r = 0;
  sample_t out_l = 0;
};

//-----------------------------------------------------------------------------
