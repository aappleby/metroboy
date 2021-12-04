#include "metron_top.h"

//-----------------------------------------------------------------------------

class MetroBoySPU {
public:
  void reset();
  void tock();

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

  uint8_t  s3_wave[16] = { 0 }; // FF30
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

  uint16_t out_r = 0;
  uint16_t out_l = 0;
};


//-----------------------------------------------------------------------------

void MetroBoySPU::reset() {
  //*this = { 0 };

  nr10 = 0x80;
  nr11 = 0xBF;
  nr12 = 0xF3;
  nr13 = 0xFF;
  nr14 = 0xBF;
  nr21 = 0x3F;
  nr22 = 0x00;
  nr23 = 0xFF;
  nr24 = 0xBF;
  nr30 = 0x7F;
  nr31 = 0xFF;
  nr32 = 0x9F;
  nr33 = 0xFF;
  nr34 = 0xBF;
  nr40 = 0x80;
  nr41 = 0xFF;
  nr42 = 0x00;
  nr43 = 0x00;
  nr44 = 0xBF;
  nr50 = 0x77;
  nr51 = 0xF3;
  nr52 = 0xF1;

  s4_lfsr = 0x7FFF;

  s1_enable = true;
}

//-----------------------------------------------------------------------------

void MetroBoySPU::tock() {
  bool sound_on = (nr52 & 0x80);

#if 0
  uint16_t spu_clock_ = (spu_clock + 1) & 0x3FFF;
  uint16_t clock_flip = (~spu_clock) & spu_clock_;

  if (!sound_on) {
    s1_out = 0;
    s2_out = 0;
    s3_out = 0;
    s4_out = 0;

    out_r = 0;
    out_l = 0;
    return;
  }

  //----------
  // sweep

  bool sweep_tick = (spu_clock_ & 0b01111111111111) == 0b01000000000000;
  bool length_tick = (spu_clock_ & 0b00111111111111) == 0b00000000000000;
  bool env_tick = (spu_clock_ & 0b11111111111111) == 0b11100000000000;

  if (sweep_tick) {
    uint8_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    uint8_t s1_sweep_shift = (nr10 & 0b00000111) >> 0;
    bool s1_sweep_dir = (nr10 & 0b00001000) >> 3;
    /* verilator lint_on WIDTH */

    if (s1_sweep_period && s1_sweep_shift) {
      if (s1_sweep_clock) {
        s1_sweep_clock = s1_sweep_clock - 1;
      }
      else {
        uint16_t delta = s1_sweep_freq >> s1_sweep_shift;
        uint16_t new_freq = s1_sweep_freq + (s1_sweep_dir ? -delta : +delta);
        s1_sweep_clock = s1_sweep_period;
        if (new_freq > 2047) {
          s1_enable = false;
        }
        else {
          s1_sweep_freq = new_freq;
        }
      }
    }
  }

  //----------
  // length

  if (length_tick) {
    /* verilator lint_off WIDTH */
    bool s1_length_enable = (nr14 & 0b01000000) >> 6;
    bool s2_length_enable = (nr24 & 0b01000000) >> 6;
    bool s3_length_enable = (nr34 & 0b01000000) >> 6;
    bool s4_length_enable = (nr44 & 0b01000000) >> 6;
    uint8_t s1_duration_ = s1_duration;
    uint8_t s2_duration_ = s2_duration;
    uint16_t s3_duration_ = s3_duration;
    uint8_t s4_duration_ = s4_duration;

    if (s1_length_enable && s1_duration_) s1_duration_ = s1_duration_ - 1;
    if (s2_length_enable && s2_duration_) s2_duration_ = s2_duration_ - 1;
    if (s3_length_enable && s3_duration_) s3_duration_ = s3_duration_ - 1;
    if (s4_length_enable && s4_duration_) s4_duration_ = s4_duration_ - 1;

    if (s1_length_enable && s1_duration_ == 0) s1_enable = false;
    if (s2_length_enable && s2_duration_ == 0) s2_enable = false;
    if (s3_length_enable && s3_duration_ == 0) s3_enable = false;
    if (s4_length_enable && s4_duration_ == 0) s4_enable = false;

    s1_duration = s1_duration_;
    s2_duration = s2_duration_;
    s3_duration = s3_duration_;
    s4_duration = s4_duration_;
  }

  //----------
  // env

  if (env_tick) {
    /* verilator lint_off WIDTH */
    const bool s1_env_dir = (nr12 & 0b00001000) >> 3;
    const bool s2_env_dir = (nr22 & 0b00001000) >> 3;
    const bool s4_env_dir = (nr42 & 0b00001000) >> 3;
    /* verilator lint_on WIDTH */

    const uint8_t s1_env_period = (nr12 & 0b00000111) >> 0;
    const uint8_t s2_env_period = (nr22 & 0b00000111) >> 0;
    const uint8_t s4_env_period = (nr42 & 0b00000111) >> 0;

    if (s1_env_period) {
      if (s1_env_clock) {
        s1_env_clock = s1_env_clock - 1;
      }
      else {
        s1_env_clock = s1_env_period;
        if (s1_env_volume < 15) s1_env_volume = s1_env_volume + 1;
      }
    }

    if (s2_env_period) {
      if (s2_env_clock) {
        s2_env_clock = s2_env_clock - 1;
      }
      else {
        s2_env_clock = s2_env_period;
        if (s2_env_volume < 15) s2_env_volume = s2_env_volume + 1;
      }
    }

    if (s4_env_period) {
      if (s4_env_clock) {
        s4_env_clock = s4_env_clock - 1;
      }
      else {
        s4_env_clock = s4_env_period;
        if (s4_env_volume < 15) s4_env_volume = s4_env_volume + 1;
      }
    }
  }

  //----------
  // phase

  if (!s1_phase_clock) {
    const uint8_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    const uint16_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;

    s1_phase_clock = 2047 ^ (s1_sweep_period ? s1_sweep_freq : s1_freq);
    s1_phase = (s1_phase + 1) & 7;
  }
  else {
    s1_phase_clock = s1_phase_clock - 1;
  }

  if (!s2_phase_clock) {
    const uint16_t s2_freq = ((nr24 << 8) | nr23) & 0x07FF;

    s2_phase_clock = 2047 ^ s2_freq;
    s2_phase = (s2_phase + 1) & 7;
  }
  else {
    s2_phase_clock = s2_phase_clock - 1;
  }

  // we run this twice because this is ticking at 1 mhz
  if (!s3_phase_clock) {
    const uint16_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;

    s3_phase_clock = 2047 ^ s3_freq;
    s3_phase = (s3_phase + 1) & 31;
  }
  else {
    s3_phase_clock--;
  }

  if (!s3_phase_clock) {
    const uint16_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;

    s3_phase_clock = 2047 ^ s3_freq;
    s3_phase = (s3_phase + 1) & 31;
  }
  else {
    s3_phase_clock--;
  }

  if (!s4_phase_clock) {
    const bool s4_lfsr_mode = (nr43 & 0b00001000) >> 3;
    const uint8_t s4_phase_period = (nr43 & 0b00000111) ? (nr43 & 0b00000111) * 2 : 1;

    s4_phase_clock = s4_phase_period;
    bool lfsr_bit = (s4_lfsr ^ (s4_lfsr >> 1)) & 1;
    /* verilator lint_on WIDTH */
    s4_phase_clock = s4_phase_period;
    if (s4_lfsr_mode) {
      /* verilator lint_off WIDTH */
      s4_lfsr = ((s4_lfsr >> 1) & 0b0111111111011111) | (lfsr_bit << 5);
      /* verilator lint_on WIDTH */
    }
    else {
      /* verilator lint_off WIDTH */
      s4_lfsr = ((s4_lfsr >> 1) & 0b0011111111111111) | (lfsr_bit << 14);
      /* verilator lint_on WIDTH */
    }
  }
  else {
    const uint8_t s4_clock_shift = (nr43 & 0b11110000) >> 4;
    s4_phase_clock -= (clock_flip >> s4_clock_shift) & 1;
  }

  //----------
  // output

  s1_out = 0;
  s2_out = 0;
  s3_out = 0;
  s4_out = 0;

  if (s1_enable) {
    uint8_t s1_volume = (nr12 & 0x08) ? s1_env_volume : 15 ^ s1_env_volume;
    uint8_t s1_duty = (nr11 & 0b11000000) >> 6;
    s1_duty = s1_duty ? s1_duty * 2 : 1;
    uint8_t s1_sample = (s1_phase < s1_duty) ? s1_volume : 0;
    s1_out = s1_sample;
  }

  if (s2_enable) {
    uint8_t s2_volume = (nr22 & 0x08) ? s2_env_volume : 15 ^ s2_env_volume;
    uint8_t s2_duty = (nr21 & 0b11000000) >> 6;
    s2_duty = s2_duty ? s2_duty * 2 : 1;
    uint8_t s2_sample = (s2_phase < s2_duty) ? s2_volume : 0;
    s2_out = s2_sample;
  }

  const bool s3_power = (nr30 & 0b10000000);
  if (s3_enable && s3_power) {
    uint8_t s3_volume_shift = 0;
    switch ((nr32 & 0b01100000) >> 5) {
    case 0: s3_volume_shift = 4; break;
    case 1: s3_volume_shift = 0; break;
    case 2: s3_volume_shift = 1; break;
    case 3: s3_volume_shift = 2; break;
    }
    uint8_t s3_sample = s3_wave[s3_phase >> 1];
    s3_sample = (s3_phase & 1) ? (s3_sample & 0xF) : (s3_sample >> 4);
    s3_sample >>= s3_volume_shift;
    s3_out = s3_sample;
  }

  if (s4_enable) {
    uint8_t s4_volume = (nr42 & 0x08) ? s4_env_volume : 15 ^ s4_env_volume;
    uint8_t s4_sample = (s4_lfsr & 1) ? s4_volume : 0;
    s4_out = s4_sample;
  }

  //----------
  // mixer & master volume

  out_r = 0;
  out_l = 0;

  /* verilator lint_off WIDTH */
  if (nr51 & 0b00000001) out_r += s1_out;
  if (nr51 & 0b00000010) out_r += s2_out;
  if (nr51 & 0b00000100) out_r += s3_out;
  if (nr51 & 0b00001000) out_r += s4_out;
  if (nr51 & 0b00010000) out_l += s1_out;
  if (nr51 & 0b00100000) out_l += s2_out;
  if (nr51 & 0b01000000) out_l += s3_out;
  if (nr51 & 0b10000000) out_l += s4_out;
  /* verilator lint_on WIDTH */

  const uint8_t volume_r = ((nr50 & 0b00000111) >> 0) + 1;
  const uint8_t volume_l = ((nr50 & 0b01110000) >> 4) + 1;

  spu_clock = spu_clock_;
#endif
}

//-----------------------------------------------------------------------------
