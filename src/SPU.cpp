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

  nr41 = 0xFF;
  nr42 = 0x00;
  nr43 = 0x00;
  nr44 = 0xBF;

  nr50 = 0x77;
  nr51 = 0xF3;
  nr52 = 0xF1;

  s1_enable = false;
  s2_enable = false;
  s3_enable = false;
  s4_enable = false;

  s1_env_volume = 0;
  s2_env_volume = 0;
  s4_env_volume = 0;

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
  const bool sound_on = (nr52 & 0x80);

  if (read)  bus_read(addr);
  if (write) {
    if (sound_on || addr == 0xFF26) {
      bus_write(addr, data2);
    }
  }
  if (tphase != 2) return;

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

  ubit14_t spu_clock_ = (spu_clock + 1) & 0x3FFF;
  ubit14_t clock_flip = (~spu_clock) & spu_clock_;
  spu_clock = spu_clock_;

  //----------
  // sweep

  bool sweep_tick = (spu_clock & 0b01111111111111) == 0b01000000000000;
  if (sweep_tick) {
    const ubit3_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    const ubit3_t s1_sweep_shift = (nr10 & 0b00000111) >> 0;
    const bool s1_sweep_dir = (nr10 & 0b00001000) >> 3;
    const ubit11_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;

    if (s1_sweep_period && s1_sweep_shift) {
      if (s1_sweep_clock) {
        s1_sweep_clock = s1_sweep_clock - 1;
      }
      else {
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

  //----------
  // env

  bool env_tick = (spu_clock & 0b11111111111111) == 0b11100000000000;
  if (env_tick) {

    const bool s1_env_dir = (nr12 & 0b00001000) >> 3;
    const bool s2_env_dir = (nr22 & 0b00001000) >> 3;
    const bool s4_env_dir = (nr42 & 0b00001000) >> 3;

    const ubit3_t s1_env_period = (nr12 & 0b00000111) >> 0;
    const ubit3_t s2_env_period = (nr22 & 0b00000111) >> 0;
    const ubit3_t s4_env_period = (nr42 & 0b00000111) >> 0;

    if (s1_env_period) {
      if (s1_env_clock) {
        s1_env_clock--;
      }
      else {
        s1_env_clock = s1_env_period;
        if (s1_env_volume < 15) s1_env_volume++;
      }
    }

    if (s2_env_period) {
      if (s2_env_clock) {
        s2_env_clock--;
      }
      else {
        s2_env_clock = s2_env_period;
        if (s2_env_volume < 15) s2_env_volume++;
      }
    }

    if (s4_env_period) {
      if (s4_env_clock) {
        s4_env_clock--;
      }
      else {
        s4_env_clock = s4_env_period;
        if (s4_env_volume < 15) s4_env_volume++;
      }
    }
  }

  //----------
  // phase

  {
    const ubit11_t s2_freq = ((nr24 << 8) | nr23) & 0x07FF;
    const ubit11_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;
    const ubit4_t s4_phase_period = (nr43 & 0b00000111) ? (nr43 & 0b00000111) * 2 : 1;
    const bool s4_lfsr_mode = (nr43 & 0b00001000) >> 3;
    const ubit4_t s4_clock_shift = (nr43 & 0b11110000) >> 4;

    if (!s1_phase_clock) {
      const ubit3_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
      const ubit11_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;
      s1_phase_clock = 2047 ^ (s1_sweep_period ? s1_sweep_freq : s1_freq);
      s1_phase = (s1_phase + 1) & 7;
    }
    else {
      s1_phase_clock--;
    }

    if (!s2_phase_clock) {
      s2_phase_clock = 2047 ^ s2_freq;
      s2_phase = (s2_phase + 1) & 7;
    }
    else {
      s2_phase_clock--;
    }

    // we run this twice because this is ticking at 1 mhz
    if (!s3_phase_clock) {
      s3_phase_clock = 2047 ^ s3_freq;
      s3_phase = (s3_phase + 1) & 31;
    }
    else {
      s3_phase_clock--;
    }

    if (!s3_phase_clock) {
      s3_phase_clock = 2047 ^ s3_freq;
      s3_phase = (s3_phase + 1) & 31;
    }
    else {
      s3_phase_clock--;
    }

    if (!s4_phase_clock) {
      s4_phase_clock = s4_phase_period;
      bool lfsr_bit = (s4_lfsr ^ (s4_lfsr >> 1)) & 1;
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

  ubit4_t s3_sample;
  s3_sample = s3_wave[s3_phase >> 1];
  s3_sample = (s3_phase & 1) ? (s3_sample & 0xF) : (s3_sample >> 4);

  ubit3_t s3_volume_shift = 0;
  switch ((nr32 & 0b01100000) >> 5) {
  case 0: s3_volume_shift = 4; break;
  case 1: s3_volume_shift = 0; break;
  case 2: s3_volume_shift = 1; break;
  case 3: s3_volume_shift = 2; break;
  }

  ubit4_t s1_volume = (nr12 & 0x08) ? s1_env_volume : 15 ^ s1_env_volume;
  ubit4_t s2_volume = (nr22 & 0x08) ? s2_env_volume : 15 ^ s2_env_volume;
  ubit4_t s3_volume = s3_sample >> s3_volume_shift;
  ubit4_t s4_volume = (nr42 & 0x08) ? s4_env_volume : 15 ^ s4_env_volume;

  ubit3_t s1_duty = (nr11 & 0b11000000) >> 6;
  s1_duty = s1_duty ? s1_duty * 2 : 1;

  ubit3_t s2_duty = (nr21 & 0b11000000) >> 6;
  s2_duty = s2_duty ? s2_duty * 2 : 1;

  const bool s3_power = (nr30 & 0b10000000);

  s1_out = s1_enable && (s1_phase < s1_duty) ? s1_volume : 0;
  s2_out = s2_enable && (s2_phase < s2_duty) ? s2_volume : 0;
  s3_out = s3_enable && s3_power             ? s3_volume : 0;
  s4_out = s4_enable && (s4_lfsr & 1)        ? s4_volume : 0;

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

  const ubit4_t volume_r = ((nr50 & 0b00000111) >> 0) + 1;
  const ubit4_t volume_l = ((nr50 & 0b01110000) >> 4) + 1;

  out_r *= volume_r;
  out_l *= volume_l;
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

  case 0xFF24: bus_out = nr50; break;
  case 0xFF25: bus_out = nr51; break;

  case 0xFF26: {
    bus_out = nr52;
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
  if (addr >= 0xFF10 && addr <= 0xFF14) {
    //printf("0x%04x 0x%02x\n", addr, data);
  }

  switch (addr) {
  case 0xFF10: nr10 = data | 0b10000000; break;
  case 0xFF11: nr11 = data | 0b00000000; break;
  case 0xFF12: {
    if ((data & 0xF8) == 0) s1_enable = false;
    if ((nr12 & 0x08) && s1_enable) s1_env_volume = (s1_env_volume + 1) & 15;
    nr12 = data;
    break;
  }
  case 0xFF13: nr13 = data | 0b00000000; break;
  case 0xFF14: nr14 = data | 0b00111000; break;

  case 0xFF16: nr21 = data | 0b00000000; break;
  case 0xFF17: {
    if ((data & 0xF8) == 0) s2_enable = false;
    if ((nr22 & 0x08) && s2_enable) s2_env_volume = (s2_env_volume + 1) & 15;
    nr22 = data;
    break;
  }
  case 0xFF18: nr23 = data | 0b00000000; break;
  case 0xFF19: nr24 = data | 0b00111000; break;

  case 0xFF1A: nr30 = data | 0b01111111; break;
  case 0xFF1B: nr31 = data | 0b00000000; break;
  case 0xFF1C: nr32 = data | 0b10011111; break;
  case 0xFF1D: nr33 = data | 0b00000000; break;
  case 0xFF1E: nr34 = data | 0b00111000; break;

  case 0xFF20: nr41 = data | 0b11000000; break;
  case 0xFF21: {
    if ((data & 0xF8) == 0) s4_enable = false;
    if ((nr42 & 0x08) && s4_enable) s4_env_volume = (s4_env_volume + 1) & 15;
    nr42 = data;
    break;
  }
  case 0xFF22: nr43 = data | 0b00000000; break;
  case 0xFF23: nr44 = data | 0b00111111; break;

  case 0xFF24: nr50 = data | 0b00000000; break;
  case 0xFF25: nr51 = data | 0b00000000; break;
  case 0xFF26: {
    nr52 = data | 0b01110000;
    if (!(nr52 & 0x80)) reset();
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

  bool s1_trigger_ = addr == 0xFF14 && (data & 0x80);
  bool s2_trigger_ = addr == 0xFF19 && (data & 0x80);
  bool s3_trigger_ = addr == 0xFF1E && (data & 0x80);
  bool s4_trigger_ = addr == 0xFF23 && (data & 0x80);

  if (s1_trigger_) {
    const ubit3_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    const ubit7_t s1_length = 64 - (nr11 & 0b00111111);
    const ubit4_t s1_start_volume = (nr12 & 0b11110000) >> 4;
    const ubit3_t s1_env_period = (nr12 & 0b00000111) >> 0;
    const ubit11_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;

    s1_enable = (nr12 & 0xF8) != 0;
    s1_duration = s1_length;
    s1_sweep_clock = s1_sweep_period;
    s1_sweep_freq = s1_freq;

    s1_env_volume = (nr12 & 0x08) ? s1_start_volume : 15 ^ s1_start_volume;
    s1_env_clock = s1_env_period;

    s1_phase_clock = 2047 ^ s1_freq;
    s1_phase = 0;
  }

  if (s2_trigger_) {
    const ubit7_t s2_length = 64 - (nr21 & 0b00111111);
    const ubit4_t s2_start_volume = (nr22 & 0b11110000) >> 4;
    const ubit3_t s2_env_period = (nr22 & 0b00000111) >> 0;
    const ubit11_t s2_freq = ((nr24 << 8) | nr23) & 0x07FF;

    s2_enable = (nr22 & 0xF8) != 0;
    s2_duration = s2_length;

    s2_env_volume = (nr22 & 0x08) ? s2_start_volume : 15 ^ s2_start_volume;
    s2_env_clock = s2_env_period;

    s2_phase_clock = 2047 ^ s2_freq;
    s2_phase = 0;
  }

  if (s3_trigger_) {
    const ubit9_t s3_length = 256 - nr31;
    const ubit11_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;

    s3_enable = (nr32 != 0);
    s3_duration = s3_length;
    s3_phase_clock = 2047 ^ s3_freq;
    s3_phase = 0;
  }

  if (s4_trigger_) {
    const ubit7_t s4_length = 64 - (nr41 & 0b00111111);
    const ubit4_t s4_start_volume = (nr42 & 0b11110000) >> 4;
    const ubit3_t s4_env_period = (nr42 & 0b00000111) >> 0;
    const ubit4_t s4_phase_period = (nr43 & 0b00000111) ? (nr43 & 0b00000111) * 2 : 1;

    s4_enable = (nr42 & 0xF8) != 0;
    s4_duration = s4_length;

    s4_env_volume = (nr42 & 0x08) ? s4_start_volume : 15 ^ s4_start_volume;
    s4_env_clock = s4_env_period;

    s4_phase_clock = s4_phase_period;
    s4_lfsr = 0x7FFF;
  }
}

//-----------------------------------------------------------------------------

void SPU::dump(std::string& out) {
  sprintf(out, "nr10 %s\n", to_binary(nr10));
  sprintf(out, "nr11 %s\n", to_binary(nr11));
  sprintf(out, "nr12 %s\n", to_binary(nr12));
  sprintf(out, "nr13 %s\n", to_binary(nr13));
  sprintf(out, "nr14 %s\n", to_binary(nr14));
  sprintf(out, "\n");

  sprintf(out, "nr20 %s\n", to_binary(nr20));
  sprintf(out, "nr21 %s\n", to_binary(nr21));
  sprintf(out, "nr22 %s\n", to_binary(nr22));
  sprintf(out, "nr23 %s\n", to_binary(nr23));
  sprintf(out, "nr24 %s\n", to_binary(nr24));
  sprintf(out, "\n");

  sprintf(out, "nr30 %s\n", to_binary(nr30));
  sprintf(out, "nr31 %s\n", to_binary(nr31));
  sprintf(out, "nr32 %s\n", to_binary(nr32));
  sprintf(out, "nr33 %s\n", to_binary(nr33));
  sprintf(out, "nr34 %s\n", to_binary(nr34));
  sprintf(out, "\n");

  sprintf(out, "nr40 %s\n", to_binary(nr40));
  sprintf(out, "nr41 %s\n", to_binary(nr41));
  sprintf(out, "nr42 %s\n", to_binary(nr42));
  sprintf(out, "nr43 %s\n", to_binary(nr43));
  sprintf(out, "nr44 %s\n", to_binary(nr44));
  sprintf(out, "\n");

  sprintf(out, "nr50 %s\n", to_binary(nr50));
  sprintf(out, "nr51 %s\n", to_binary(nr51));
  sprintf(out, "nr52 %s\n", to_binary(nr52));
  sprintf(out, "\n");

  const char* bar = "===============";

  ubit4_t s1_volume = (nr12 & 0x08) ? s1_env_volume : 15 ^ s1_env_volume;
  sprintf(out, "s1 vol %s\n", bar + (15 - s1_volume));
  sprintf(out, "s2 vol %s\n", bar + (15 - s2_env_volume));

  ubit3_t s3_volume = 0;
  switch ((nr32 & 0b01100000) >> 5) {
  case 0: s3_volume = 0; break;
  case 1: s3_volume = 15; break;
  case 2: s3_volume = 7; break;
  case 3: s3_volume = 3; break;
  }
  sprintf(out, "s3 vol %s\n", bar + (15 - s3_volume));
  sprintf(out, "s4 vol %s\n", bar + (15 - s4_env_volume));
}

//-----------------------------------------------------------------------------
