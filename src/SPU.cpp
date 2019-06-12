#include "Platform.h"
#include "SPU.h"

#include "Common.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

void SPU::reset() {
  int i;

  out_r = 0;
  out_l = 0;
  bus_out = 0;
  bus_oe = 0;

  nr10 = 0x80;

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

void SPU::tock(int tphase, ubit16_t addr, ubit8_t data2, bool read, bool write) {
  if (read)  bus_read(addr);
  if (write) bus_write(addr, data2);
  if (tphase != 2) return;

  //----------

  ubit14_t spu_clock_ = (spu_clock + 1) & 0x3FFF;
  ubit14_t clock_flip = (~spu_clock) & spu_clock_;
  spu_clock = spu_clock_;

  //----------
  // sweep

  {
    bool sweep_tick = (spu_clock & 0b01111111111111) == 0b01000000000000;
    const ubit3_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    const ubit3_t s1_sweep_shift = (nr10 & 0b00000111) >> 0;
    const ubit11_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;

    if (sweep_tick && s1_sweep_period && s1_sweep_shift) {
      if (s1_sweep_clock) {
        s1_sweep_clock = s1_sweep_clock - 1;
      }
      else {
        const bool    s1_sweep_dir = (nr10 & 0b00001000) >> 3;
        ubit11_t delta = s1_sweep_freq >> s1_sweep_shift;
        ubit11_t new_freq = s1_sweep_freq + (s1_sweep_dir ? -delta : +delta);
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

  {
    bool length_tick = (spu_clock & 0b00111111111111) == 0b00000000000000;
    if (length_tick) {
      const bool s1_length_enable = (nr14 & 0b01000000) >> 6;
      const bool s2_length_enable = (nr24 & 0b01000000) >> 6;
      const bool s3_length_enable = (nr34 & 0b01000000) >> 6;
      const bool s4_length_enable = (nr44 & 0b01000000) >> 6;

      if (s1_length_enable && s1_duration) s1_duration = s1_duration - 1;
      if (s2_length_enable && s2_duration) s2_duration = s2_duration - 1;
      if (s3_length_enable && s3_duration) s3_duration = s3_duration - 1;
      if (s4_length_enable && s4_duration) s4_duration = s4_duration - 1;

      if (s1_length_enable && s1_duration == 0) s1_enable = false;
      if (s2_length_enable && s2_duration == 0) s2_enable = false;
      if (s3_length_enable && s3_duration == 0) s3_enable = false;
      if (s4_length_enable && s4_duration == 0) s4_enable = false;
    }
  }

  //----------
  // env

  {
    bool env_tick = (spu_clock & 0b11111111111111) == 0b11100000000000;

    if (env_tick && s1_env_period) {
      if (s1_env_clock) {
        s1_env_clock--;
      }
      else {
        s1_env_clock = s1_env_period;
        if (s1_volume < 15 && s1_env_dir) s1_volume++;
        if (s1_volume > 0 && !s1_env_dir) s1_volume--;
      }
    }

    if (env_tick && s2_env_period) {
      if (s2_env_clock) {
        s2_env_clock--;
      }
      else {
        s2_env_clock = s2_env_period;
        if (s2_volume < 15 && s2_env_dir) s2_volume++;
        if (s2_volume > 0 && !s2_env_dir) s2_volume--;
      }
    }

    if (env_tick && s4_env_period) {
      if (s4_env_clock) {
        s4_env_clock--;
      }
      else {
        s4_env_clock = s4_env_period;
        if (s4_volume < 15 && s4_env_dir) s4_volume++;
        if (s4_volume > 0 && !s4_env_dir) s4_volume--;
      }
    }
  }

  //----------
  // phase

  {
    if (!s1_phase_clock) {
      s1_phase_clock = 2047 ^ s1_sweep_freq;
      s1_phase = (s1_phase + 1) & 7;
    }
    else {
      s1_phase_clock--;
    }

    if (!s2_phase_clock) {
      const ubit11_t s2_freq = ((nr24 << 8) | nr23) & 0x07FF;
      s2_phase_clock = 2047 ^ s2_freq;
      s2_phase = (s2_phase + 1) & 7;
    }
    else {
      s2_phase_clock--;
    }

    // we run this twice because this is ticking at 1 mhz
    if (!s3_phase_clock) {
      const ubit11_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;
      s3_phase_clock = 2047 ^ s3_freq;
      s3_phase = (s3_phase + 1) & 31;
    }
    else {
      s3_phase_clock--;
    }

    if (!s3_phase_clock) {
      const ubit11_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;
      s3_phase_clock = 2047 ^ s3_freq;
      s3_phase = (s3_phase + 1) & 31;
    }
    else {
      s3_phase_clock--;
    }

    if (!s4_phase_clock) {
      bool lfsr_bit;
      s4_phase_clock = s4_phase_period;
      lfsr_bit = (s4_lfsr ^ (s4_lfsr >> 1)) & 1;
      s4_lfsr >>= 1;
      s4_lfsr |= (lfsr_bit << 14);
      if (s4_lfsr_mode) {
        s4_lfsr &= ~(1 << 5);
        s4_lfsr |= (lfsr_bit << 5);
      }
    }
    else {
      s4_phase_clock -= (clock_flip >> s4_clock_shift) & 1;
    }
  }

  //----------
  // output

  if (sound_on) {
    ubit4_t s3_sample;
    s3_sample = s3_wave[s3_phase >> 1];
    s3_sample = (s3_phase & 1) ? (s3_sample & 0xF) : (s3_sample >> 4);
    s3_sample >>= s3_volume_shift;

    ubit3_t s1_duty = 0;
    switch ((nr11 & 0b11000000) >> 6) {
    case 0: s1_duty = 1; break;
    case 1: s1_duty = 2; break;
    case 2: s1_duty = 4; break;
    case 3: s1_duty = 6; break;
    }

    const bool s3_power = (nr30 & 0b10000000);

    s1_out = s1_enable ? (s1_phase < s1_duty ? s1_volume : 0) : 0;
    s2_out = s2_enable ? (s2_phase < s2_duty ? s2_volume : 0) : 0;
    s3_out = s3_enable ? (s3_power ? s3_sample : 0) : 0;
    s4_out = s4_enable ? (s4_lfsr & 1 ? s4_volume : 0) : 0;

    const bool s1r = (nr51 & 0b00000001);
    const bool s2r = (nr51 & 0b00000010);
    const bool s3r = (nr51 & 0b00000100);
    const bool s4r = (nr51 & 0b00001000);
    const bool s1l = (nr51 & 0b00010000);
    const bool s2l = (nr51 & 0b00100000);
    const bool s3l = (nr51 & 0b01000000);
    const bool s4l = (nr51 & 0b10000000);

    out_r = (s1_out * s1r) + (s2_out * s2r) + (s3_out * s3r) + (s4_out * s4r);
    out_l = (s1_out * s1l) + (s2_out * s2l) + (s3_out * s3l) + (s4_out * s4l);

    out_r *= volume_r;
    out_l *= volume_l;
  }
  else {
    s1_out = 0;
    s2_out = 0;
    s3_out = 0;
    s4_out = 0;

    out_r = 0;
    out_l = 0;
  }
}

//-----------------------------------------------------------------------------

void SPU::bus_read(ubit16_t addr) {
  bus_oe = 1;
  switch (addr) {
  case 0xFF10: bus_out = nr10 | 0x80; break;
  case 0xFF11: bus_out = nr11 | 0x3F; break;
  case 0xFF12: bus_out = nr12 | 0x00; break;
  case 0xFF13: bus_out = nr13 | 0xFF; break;
  case 0xFF14: bus_out = nr14 | 0xBF; break;
  case 0xFF15: bus_out = nr20 | 0xFF; break;
  case 0xFF16: bus_out = nr21 | 0x3F; break;
  case 0xFF17: bus_out = nr22 | 0x00; break;
  case 0xFF18: bus_out = nr23 | 0xFF; break;
  case 0xFF19: bus_out = nr24 | 0xBF; break;
  case 0xFF1A: bus_out = nr30 | 0x7F; break;
  case 0xFF1B: bus_out = nr31 | 0xFF; break;
  case 0xFF1C: bus_out = nr32 | 0x9F; break;
  case 0xFF1D: bus_out = nr33 | 0xFF; break;
  case 0xFF1E: bus_out = nr34 | 0xBF; break;
  case 0xFF1F: bus_out = nr40 | 0xFF; break;
  case 0xFF20: bus_out = nr41 | 0xFF; break;
  case 0xFF21: bus_out = nr42 | 0x00; break;
  case 0xFF22: bus_out = nr43 | 0x00; break;
  case 0xFF23: bus_out = nr44 | 0xBF; break;
  case 0xFF24: bus_out = nr50 | 0x00; break;
  case 0xFF25: bus_out = nr51 | 0x00; break;

  case 0xFF26: {
    bus_out = (nr52 & 0x80) | 0x70;
    if (s1_enable) bus_out |= 0b00000001;
    if (s2_enable) bus_out |= 0b00000010;
    if (s3_enable) bus_out |= 0b00000100;
    if (s4_enable) bus_out |= 0b00001000;
    break;
  }

  default: {
    bus_out = 0;
    bus_oe = 0;
    break;
  }
  }

  //----------
  // wavetable

  if (addr >= 0xFF30 && addr <= 0xFF3F) {
    bus_oe = 1;
    bus_out = s3_wave[addr & 0xF];
  }
}

//-----------------------------------------------------------------------------

void SPU::bus_write(ubit16_t addr, ubit8_t data) {
  switch (addr) {

  case 0xFF10: nr10 = data | 0b10000000; break;
  case 0xFF11: nr11 = data; break;
  case 0xFF1A: nr30 = data; break;

  case 0xFF16: {
    nr21 = data;

    switch ((nr21 & 0b11000000) >> 6) {
    case 0: s2_duty = 1; break;
    case 1: s2_duty = 2; break;
    case 2: s2_duty = 4; break;
    case 3: s2_duty = 6; break;
    }

    break;
  }

  case 0xFF1B: nr31 = data;  break;

  case 0xFF20: {
    nr41 = data;

    break;
  }

  //----------
  // nrx2 - volume and envelope

  case 0xFF12: {
    s1_enable = (data != 0);
    s1_env_volume = (data & 0b11110000) >> 4;
    s1_env_dir    = (data & 0b00001000) >> 3;
    s1_env_period = (data & 0b00000111) >> 0;

    nr12 = data;
    break;
  }

  case 0xFF17: {
    s2_enable = (data != 0);
    s2_env_volume = (data & 0b11110000) >> 4;
    s2_env_dir    = (data & 0b00001000) >> 3;
    s2_env_period = (data & 0b00000111) >> 0;

    nr22 = data;
    break;
  }

  case 0xFF1C: {
    s3_enable = (data != 0);
    switch ((data & 0b01100000) >> 5) {
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
    s4_env_volume = (data & 0b11110000) >> 4;
    s4_env_dir    = (data & 0b00001000) >> 3;
    s4_env_period = (data & 0b00000111) >> 0;

    nr42 = data;
    break;
  }

  //----------
  // nrx3 - frequency lsb, lfsr clock/mode/shift

  case 0xFF13: nr13 = data; break;
  case 0xFF18: nr23 = data; break;
  case 0xFF1D: nr33 = data; break;

  case 0xFF22: {
    nr43 = data;

    // period is actually [1, 2, 4, 6, 8, 10, 12, 14]
    if (nr43 & 0b00000111) {
      s4_phase_period = (nr43 & 0b00000111) * 2;
    } else {
      s4_phase_period = 1;
    }
    s4_lfsr_mode = (nr43 & 0b00001000) >> 3;
    s4_clock_shift = (nr43 & 0b11110000) >> 4;

    break;
  }

  //----------
  // nrx4 - frequency msb, length_enable, trigger

  case 0xFF14: nr14 = data; break;
  case 0xFF19: nr24 = data; break;
  case 0xFF1E: nr34 = data; break;
  case 0xFF23: nr44 = data; break;

  //----------
  // global spu stuff

  case 0xFF24: {
    nr50 = data;
    volume_r = ((data & 0b00000111) >> 0) + 1;
    volume_l = ((data & 0b01110000) >> 4) + 1;
    break;
  }
  case 0xFF25: {
    nr51 = data;
    break;
  }
  case 0xFF26: {
    nr52 = data;
    sound_on = (data >> 7) & 1;
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
    const ubit7_t s1_length = (nr11 & 0b00111111) ? 63 ^ (nr11 & 0b00111111) : 64;
    const ubit3_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    const ubit11_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;

    s1_enable = true;
    s1_duration = s1_length;
    s1_sweep_clock = s1_sweep_period;
    s1_sweep_freq = s1_freq;
    s1_volume = s1_env_volume;
    s1_env_clock = s1_env_period;
    s1_phase_clock = 2047 ^ s1_freq;
    s1_phase = 0;
  }

  if (s2_trigger_) {
    const ubit7_t s2_length = (nr21 & 0b00111111) ? 63 ^ (nr21 & 0b00111111) : 64;
    const ubit11_t s2_freq = ((nr24 << 8) | nr23) & 0x07FF;

    s2_enable = true;
    s2_duration = s2_length;
    s2_volume = s2_env_volume;
    s2_env_clock = s2_env_period;
    s2_phase_clock = 2047 ^ s2_freq;
    s2_phase = 0;
  }

  if (s3_trigger_) {
    const ubit9_t s3_length = (nr31 & 0b11111111) ? 255 ^ (nr31 & 0b11111111) : 256;
    const ubit11_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;

    s3_enable = true;
    s3_duration = s3_length;
    s3_phase_clock = 2047 ^ s3_freq;
    s3_phase = 0;
  }

  if (s4_trigger_) {
    const ubit7_t s4_length = (nr41 & 0b00111111) ? 63 ^ (nr41 & 0b00111111) : 64;

    s4_enable = true;
    s4_duration = s4_length;
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
