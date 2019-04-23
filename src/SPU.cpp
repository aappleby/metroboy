#include "SPU.h"
#include "constants.h"

#include <math.h>
#include <stdarg.h>

#pragma warning(disable : 4996)

const char* to_binary(uint8_t b);

uint32_t pext(uint32_t a, uint32_t b) {
  return _pext_u32(a, b);
}

//-----------------------------------------------------------------------------

void SPU::reset() {
  int i;

  out_r = 0;
  out_l = 0;
  bus_out = 0;
  bus_oe = 0;

  bus_write(0xFF10, 0x80); // nr10
  bus_write(0xFF11, 0xBF); // nr11
  bus_write(0xFF12, 0xF3); // nr12
  bus_write(0xFF13, 0xFF); // nr13
  bus_write(0xFF14, 0xBF); // nr14

  bus_write(0xFF16, 0x3F); // nr21
  bus_write(0xFF17, 0x00); // nr22
  bus_write(0xFF18, 0xFF); // nr23
  bus_write(0xFF19, 0xBF); // nr24

  bus_write(0xFF1A, 0x7F); // nr30
  bus_write(0xFF1B, 0xFF); // nr31
  bus_write(0xFF1C, 0x9F); // nr32
  bus_write(0xFF1D, 0xFF); // nr33
  bus_write(0xFF1E, 0xBF); // nr34

  bus_write(0xFF20, 0xFF); // nr41
  bus_write(0xFF21, 0x00); // nr42
  bus_write(0xFF22, 0x00); // nr43
  bus_write(0xFF23, 0xBF); // nr44

  bus_write(0xFF24, 0x77); // nr50
  bus_write(0xFF25, 0xF3); // nr51
  bus_write(0xFF26, 0xF1); // nr52

  s1_enable = true;
  s2_enable = false;
  s3_enable = false;
  s4_enable = false;

  s1_volume = 0;
  s2_volume = 0;
  s3_volume = 0;
  s4_volume = 0;

  spu_clock = 0;

  s1_sweep_clock = 0;

  s1_env_clock = 0;
  s2_env_clock = 0;
  s4_env_clock = 0;

  s1_phase_clock = 2047;
  s2_phase_clock = 2047;
  s3_phase_clock = 2047;
  s4_phase_clock = 2047;

  s1_phase = 0;
  s2_phase = 0;
  s3_phase = 0;

  s4_lfsr = 0x7FFF;

  for (i = 0; i < 16; i++) s3_wave[i] = 0;
}

//-----------------------------------------------------------------------------

void SPU::tock(int tphase, ubit16_t addr, ubit8_t data, bool read, bool write) {
  if (read) {
    bus_read(addr);
    return;
  }
  else if (write) {
    bus_write(addr, data);
    return;
  }
  else if (tphase != PHASE_SPU_TOCK) {
    return;
  }

  //----------

  ubit14_t spu_clock_ = (spu_clock + 1) & 0x3FFF;
  ubit14_t clock_flip = (~spu_clock) & spu_clock_;
  spu_clock = spu_clock_;

  ubit9_t _out_r = out_r;
  ubit9_t _out_l = out_l;
  ubit4_t s1_out_, s2_out_, s3_out_, s4_out_;

  ubit4_t s1_volume_ = s1_volume;
  ubit4_t s2_volume_ = s2_volume;
  ubit4_t s4_volume_ = s4_volume;

  ubit3_t s1_sweep_clock_ = s1_sweep_clock;
  ubit11_t s1_freq_ = s1_freq;

  ubit7_t s1_length_ = s1_length;
  ubit7_t s2_length_ = s2_length;
  ubit9_t s3_length_ = s3_length;
  ubit7_t s4_length_ = s4_length;

  ubit3_t s1_env_clock_ = s1_env_clock;
  ubit3_t s2_env_clock_ = s2_env_clock;
  ubit3_t s4_env_clock_ = s4_env_clock;

  ubit11_t s1_phase_clock_ = s1_phase_clock;
  ubit11_t s2_phase_clock_ = s2_phase_clock;
  ubit11_t s3_phase_clock_ = s3_phase_clock;
  ubit11_t s4_phase_clock_ = s4_phase_clock;

  ubit3_t s1_phase_ = s1_phase;
  ubit3_t s2_phase_ = s2_phase;
  ubit5_t s3_phase_ = s3_phase;
  ubit15_t s4_lfsr_ = s4_lfsr;

  //----------
  // sweep

  bool sweep_tick = (spu_clock & 0b01111111111111) == 0b01000000000000;

  if (sweep_tick && s1_sweep_period && s1_sweep_shift) {
    if (s1_sweep_clock_) {
      s1_sweep_clock_ = s1_sweep_clock_ - 1;
    }
    else {
      ubit11_t delta = s1_freq_ >> s1_sweep_shift;
      ubit11_t new_freq = s1_freq_ + (s1_sweep_dir ? -delta : +delta);
      s1_sweep_clock_ = s1_sweep_period;
      if (new_freq > 2047) {
        s1_enable = false;
      }
      else {
        s1_freq_ = new_freq;
      }
    }
  }

  //----------
  // length

  bool length_tick = (spu_clock & 0b00111111111111) == 0b00000000000000;
  if (length_tick) {
    if (s1_length_enable && s1_length_) s1_length_ = s1_length_ - 1;
    if (s2_length_enable && s2_length_) s2_length_ = s2_length_ - 1;
    if (s3_length_enable && s3_length_) s3_length_ = s3_length_ - 1;
    if (s4_length_enable && s4_length_) s4_length_ = s4_length_ - 1;

    if (s1_length_enable && s1_length_ == 0) s1_enable = false;
    if (s2_length_enable && s2_length_ == 0) s2_enable = false;
    if (s3_length_enable && s3_length_ == 0) s3_enable = false;
    if (s4_length_enable && s4_length_ == 0) s4_enable = false;
  }

  //----------
  // env

  bool env_tick = (spu_clock & 0b11111111111111) == 0b11100000000000;

  if (env_tick && s1_env_period) {
    if (s1_env_clock_) {
      s1_env_clock_--;
    }
    else {
      s1_env_clock_ = s1_env_period;
      if (s1_volume_ < 15 && s1_env_dir) s1_volume_++;
      if (s1_volume_ > 0 && !s1_env_dir) s1_volume_--;
    }
  }

  if (env_tick && s2_env_period) {
    if (s2_env_clock_) {
      s2_env_clock_--;
    }
    else {
      s2_env_clock_ = s2_env_period;
      if (s2_volume_ < 15 && s2_env_dir) s2_volume_++;
      if (s2_volume_ > 0 && !s2_env_dir) s2_volume_--;
    }
  }

  if (env_tick && s4_env_period) {
    if (s4_env_clock_) {
      s4_env_clock_--;
    }
    else {
      s4_env_clock_ = s4_env_period;
      if (s4_volume_ < 15 && s4_env_dir) s4_volume_++;
      if (s4_volume_ > 0 && !s4_env_dir) s4_volume_--;
    }
  }

  //----------
  // phase

  if (!s1_phase_clock_) {
    s1_phase_clock_ = 2047 ^ s1_freq;
    s1_phase_ = (s1_phase_ + 1) & 7;
  }
  else {
    s1_phase_clock_--;
  }

  if (!s2_phase_clock_) {
    s2_phase_clock_ = 2047 ^ s2_freq;
    s2_phase_ = (s2_phase_ + 1) & 7;
  }
  else {
    s2_phase_clock_--;
  }


  // we run this twice because this is ticking at 1 mhz
  if (!s3_phase_clock_) {
    s3_phase_clock_ = 2047 ^ s3_freq;
    s3_phase_ = (s3_phase_ + 1) & 31;
  }
  else {
    s3_phase_clock_--;
  }

  if (!s3_phase_clock_) {
    s3_phase_clock_ = 2047 ^ s3_freq;
    s3_phase_ = (s3_phase_ + 1) & 31;
  }
  else {
    s3_phase_clock_--;
  }

  if (!s4_phase_clock_) {
    bool lfsr_bit;
    s4_phase_clock_ = s4_phase_period;
    lfsr_bit = (s4_lfsr_ ^ (s4_lfsr_ >> 1)) & 1;
    s4_lfsr_ >>= 1;
    s4_lfsr_ |= (lfsr_bit << 14);
    if (s4_lfsr_mode) {
      s4_lfsr_ &= ~(1 << 5);
      s4_lfsr_ |= (lfsr_bit << 5);
    }
  }
  else {
    s4_phase_clock_ -= (clock_flip >> s4_clock_shift) & 1;
  }

  //----------
  // output

  if (sound_on) {
    ubit4_t s3_sample_;
    s3_sample_ = pext(s3_wave[s3_phase >> 1], (s3_phase & 1) ? 0b00001111 : 0b11110000);
    s3_sample_ >>= s3_volume_shift;

    s1_out_ = s1_enable ? (s1_phase < s1_duty ? s1_volume : 0) : 0;
    s2_out_ = s2_enable ? (s2_phase < s2_duty ? s2_volume : 0) : 0;
    s3_out_ = s3_enable ? (s3_power ? s3_sample_ : 0) : 0;
    s4_out_ = s4_enable ? (s4_lfsr & 1 ? s4_volume : 0) : 0;

    _out_r = (s1_out_ & s1r) + (s2_out_ & s2r) + (s3_out_ & s3r) + (s4_out_ & s4r);
    _out_l = (s1_out_ & s1l) + (s2_out_ & s2l) + (s3_out_ & s3l) + (s4_out_ & s4l);

    _out_r *= volume_r;
    _out_l *= volume_l;
  }
  else {
    s1_out_ = 0;
    s2_out_ = 0;
    s3_out_ = 0;
    s4_out_ = 0;

    _out_r = 0;
    _out_l = 0;
  }

  //----------
  // commit

  s1_volume = s1_volume_;
  s2_volume = s2_volume_;
  s4_volume = s4_volume_;

  s1_sweep_clock = s1_sweep_clock_;
  s1_freq = s1_freq_;

  s1_length = s1_length_;
  s2_length = s2_length_;
  s3_length = s3_length_;
  s4_length = s4_length_;

  s1_env_clock = s1_env_clock_;
  s2_env_clock = s2_env_clock_;
  s4_env_clock = s4_env_clock_;

  s1_phase_clock = s1_phase_clock_;
  s2_phase_clock = s2_phase_clock_;
  s3_phase_clock = s3_phase_clock_;
  s4_phase_clock = s4_phase_clock_;

  s1_phase = s1_phase_;
  s2_phase = s2_phase_;
  s3_phase = s3_phase_;
  s4_lfsr = s4_lfsr_;

  s1_out = s1_out_;
  s2_out = s2_out_;
  s3_out = s3_out_;
  s4_out = s4_out_;

  out_r = _out_r;
  out_l = _out_l;
}

//-----------------------------------------------------------------------------

void SPU::bus_read(ubit16_t addr) {
  ubit8_t _bus_out = bus_out;
  bool _bus_oe = bus_oe;

  _bus_oe = 1;
  switch (addr) {
  case 0xFF10: _bus_out = nr10 | 0x80; break;
  case 0xFF11: _bus_out = nr11 | 0x3F; break;
  case 0xFF12: _bus_out = nr12 | 0x00; break;
  case 0xFF13: _bus_out = nr13 | 0xFF; break;
  case 0xFF14: _bus_out = nr14 | 0xBF; break;
  case 0xFF15: _bus_out = nr20 | 0xFF; break;
  case 0xFF16: _bus_out = nr21 | 0x3F; break;
  case 0xFF17: _bus_out = nr22 | 0x00; break;
  case 0xFF18: _bus_out = nr23 | 0xFF; break;
  case 0xFF19: _bus_out = nr24 | 0xBF; break;
  case 0xFF1A: _bus_out = nr30 | 0x7F; break;
  case 0xFF1B: _bus_out = nr31 | 0xFF; break;
  case 0xFF1C: _bus_out = nr32 | 0x9F; break;
  case 0xFF1D: _bus_out = nr33 | 0xFF; break;
  case 0xFF1E: _bus_out = nr34 | 0xBF; break;
  case 0xFF1F: _bus_out = nr40 | 0xFF; break;
  case 0xFF20: _bus_out = nr41 | 0xFF; break;
  case 0xFF21: _bus_out = nr42 | 0x00; break;
  case 0xFF22: _bus_out = nr43 | 0x00; break;
  case 0xFF23: _bus_out = nr44 | 0xBF; break;
  case 0xFF24: _bus_out = nr50 | 0x00; break;
  case 0xFF25: _bus_out = nr51 | 0x00; break;

  case 0xFF26: {
    _bus_out = (nr52 & 0x80) | 0x70;
    if (s1_enable) _bus_out |= 0b00000001;
    if (s2_enable) _bus_out |= 0b00000010;
    if (s3_enable) _bus_out |= 0b00000100;
    if (s4_enable) _bus_out |= 0b00001000;
    break;
  }

  default: {
    _bus_out = 0;
    _bus_oe = 0;
    break;
  }
  }

  //----------
  // wavetable

  if (addr >= 0xFF30 && addr <= 0xFF3F) {
    _bus_oe = 1;
    _bus_out = s3_wave[addr & 0xF];
  }

  bus_out = _bus_out;
  bus_oe = _bus_oe;
}

//-----------------------------------------------------------------------------

void SPU::bus_write(ubit16_t addr, ubit8_t data) {
  switch (addr) {

  //----------
  // nrx0 - sweep and wave power

  case 0xFF10: {
    nr10 = data | 0b10000000;
    s1_sweep_period = pext(data, 0b01110000);
    s1_sweep_dir    = pext(data, 0b00001000);
    s1_sweep_shift  = pext(data, 0b00000111);
    break;
  }
  case 0xFF1A: {
    nr30 = data;
    s3_power = pext(data, 0b10000000);
    break;
  }

  //----------
  // nrx1 - duty and length

  case 0xFF11: {
    switch (pext(data, 0b11000000)) {
    case 0: s1_duty = 1; break;
    case 1: s1_duty = 2; break;
    case 2: s1_duty = 4; break;
    case 3: s1_duty = 6; break;
    }

    s1_length = 63 ^ pext(data, 0b00111111);
    if (s1_length == 0) s1_length = 64;

    nr11 = data;
    break;
  }

  case 0xFF16: {
    switch ((data & 0b11000000) >> 6) {
    case 0: s2_duty = 1; break;
    case 1: s2_duty = 2; break;
    case 2: s2_duty = 4; break;
    case 3: s2_duty = 6; break;
    }

    s2_length = 63 ^ (data & 0b00111111);
    if (s2_length == 0) s2_length = 64;

    nr21 = data;
    break;
  }

  case 0xFF1B: {
    s3_length = 255 ^ data;
    if (s3_length == 0) s3_length = 256;

    nr31 = data;
    break;
  }

  case 0xFF20: {
    s4_length = 63 ^ (data & 0b00111111);
    if (s4_length == 0) s4_length = 64;

    nr41 = data;
    break;
  }

  //----------
  // nrx2 - volume and envelope

  case 0xFF12: {
    if (data == 0) s1_enable = false;
    s1_env_volume = pext(data, 0b11110000);
    s1_env_dir    = pext(data, 0b00001000);
    s1_env_period = pext(data, 0b00000111);

    nr12 = data;
    break;
  }

  case 0xFF17: {
    if (data == 0) s2_enable = false;
    s2_env_volume = pext(data, 0b11110000);
    s2_env_dir    = pext(data, 0b00001000);
    s2_env_period = pext(data, 0b00000111);

    nr22 = data;
    break;
  }

  case 0xFF1C: {
    switch (pext(data, 0b01100000)) {
    case 0: s3_volume_shift = 4; s3_volume = 0;  break;
    case 1: s3_volume_shift = 0; s3_volume = 15; break;
    case 2: s3_volume_shift = 1; s3_volume = 7;  break;
    case 3: s3_volume_shift = 2; s3_volume = 3;  break;
    }

    nr32 = data;
    break;
  }

  case 0xFF21: {
    s4_enable = (data != 0);
    s4_env_volume = pext(data, 0b11110000);
    s4_env_dir =    pext(data, 0b00001000);
    s4_env_period = pext(data, 0b00000111);

    nr42 = data;
    break;
  }

  //----------
  // nrx3 - frequency lsb, lfsr clock/mode/shift

  case 0xFF13: {
    s1_freq = (s1_freq & 0xFF00) | data;
    nr13 = data;
    break;
  }

  case 0xFF18: {
    s2_freq = (s2_freq & 0xFF00) | data;
    nr23 = data;
    break;
  }

  case 0xFF1D: {
    s3_freq = (s2_freq & 0xFF00) | data;
    nr33 = data;
    break;
  }

  case 0xFF22: {
    // period is actually [1, 2, 4, 6, 8, 10, 12, 14]
    s4_phase_period = pext(data, 0b00000111) * 2;
    if (s4_phase_period == 0) s4_phase_period = 1;
    s4_lfsr_mode = pext(data, 0b00001000);
    s4_clock_shift = pext(data, 0b11110000);

    nr43 = data;
    break;
  }

  //----------
  // nrx4 - frequency msb, length_enable, trigger

  case 0xFF14: {
    s1_freq = (s1_freq & 0x00FF) | (pext(data, 0b00000111) << 8);
    s1_length_enable = pext(data, 0b01000000);

    nr14 = data;
    break;
  }

  case 0xFF19: {
    s2_freq = (s2_freq & 0x00FF) | (pext(data, 0b00000111) << 8);
    s2_length_enable = pext(data, 0b01000000);

    nr24 = data;
    break;
  }

  case 0xFF1E: {
    s3_freq = (s3_freq & 0x00FF) | (pext(data, 0b00000111) << 8);
    s3_length_enable = pext(data, 0b01000000);

    nr34 = data;
    break;
  }

  case 0xFF23: {
    s4_length_enable = pext(data, 0b01000000);

    nr44 = data;
    break;
  }

  //----------
  // global spu stuff

  case 0xFF24: {
    nr50 = data;

    volume_r = pext(data, 0b00000111) + 1;
    volume_l = pext(data, 0b01110000) + 1;

    break;
  }
  case 0xFF25: {
    nr51 = data;
    s1r = pext(data, 0b00000001) * 0xF;
    s2r = pext(data, 0b00000010) * 0xF;
    s3r = pext(data, 0b00000100) * 0xF;
    s4r = pext(data, 0b00001000) * 0xF;
    s1l = pext(data, 0b00010000) * 0xF;
    s2l = pext(data, 0b00100000) * 0xF;
    s3l = pext(data, 0b01000000) * 0xF;
    s4l = pext(data, 0b10000000) * 0xF;
    break;
  }
  case 0xFF26: {
    nr52 = data;
    sound_on = pext(data, 0b10000000);
    break;
  }
  }

  //----------
  // wavetable

  if (addr >= 0xFF30 && addr <= 0xFF3F) {
    s3_wave[addr & 0xF] = data;
  }

  //----------
  // triggers

  bool s1_trigger_ = addr == 0xFF14 && (data >> 7);
  bool s2_trigger_ = addr == 0xFF19 && (data >> 7);
  bool s3_trigger_ = addr == 0xFF1E && (data >> 7);
  bool s4_trigger_ = addr == 0xFF23 && (data >> 7);

  if (s1_trigger_) {
    s1_enable = true;
    s1_sweep_clock = s1_sweep_period;
    s1_volume = s1_env_volume;
    s1_env_clock = s1_env_period;
    s1_phase_clock = 2047 ^ s1_freq;
    s1_phase = 0;
  }

  if (s2_trigger_) {
    s2_enable = true;
    s2_volume = s2_env_volume;
    s2_env_clock = s2_env_period;
    s2_phase_clock = 2047 ^ s2_freq;
    s2_phase = 0;
  }

  if (s3_trigger_) {
    s3_enable = true;
    s3_phase_clock = 2047 ^ s3_freq;
    s3_phase = 0;
  }

  if (s4_trigger_) {
    s4_enable = true;
    s4_volume = s4_env_volume;
    s4_env_clock = s4_env_period;
    s4_phase_clock = s4_phase_period;
    s4_lfsr = 0x7FFF;
  }
}

//-----------------------------------------------------------------------------

char* SPU::dump(char* cursor) {
  cursor += sprintf(cursor, "nr10 %s\n", to_binary(nr10));
  cursor += sprintf(cursor, "nr11 %s\n", to_binary(nr11));
  cursor += sprintf(cursor, "nr12 %s\n", to_binary(nr12));
  cursor += sprintf(cursor, "nr13 %s\n", to_binary(nr13));
  cursor += sprintf(cursor, "nr14 %s\n", to_binary(nr14));

  const char* bar = "===============";
  cursor += sprintf(cursor, "vol %s\n", bar + (15 - s1_volume));

  return cursor;
}

//-----------------------------------------------------------------------------
