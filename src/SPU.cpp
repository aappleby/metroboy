#include "SPU.h"

#include "Constants.h"

const char* to_binary(uint8_t b);

//-----------------------------------------------------------------------------

SpuOut SPU::reset() {
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

  memset(s3_wave, 0, 16);
  s4_lfsr = 0x7FFF;

  spu_clock = 0;

  s1_enable = true;
  s2_enable = false;
  s3_enable = false;
  s4_enable = false;

  s1_sweep_clock = 0;
  s1_sweep_freq = 0;

  s1_duration = 0;
  s2_duration = 0;
  s3_duration = 0;
  s4_duration = 0;

  s1_env_clock = 0;
  s2_env_clock = 0;
  s4_env_clock = 0;
  s1_env_volume = 0;
  s2_env_volume = 0;
  s4_env_volume = 0;

  s1_phase_clock = 2047;
  s2_phase_clock = 2047;
  s3_phase_clock = 2047;
  s4_phase_clock = 2047;
  s1_phase = 0;
  s2_phase = 0;
  s3_phase = 0;

  return {0};
}

//-----------------------------------------------------------------------------

SpuOut SPU::tick() const {
  return out;
}

void SPU::tock(int tphase, CpuBus bus) {
  bool sound_on = (nr52 & 0x80);
  uint16_t spu_clock_ = (spu_clock + 1) & 0x3FFF;
  uint16_t clock_flip = (~spu_clock) & spu_clock_;

  if (bus.read)  bus_read(bus.addr, out);
  if (bus.write && (sound_on || bus.addr == 0xFF26)) bus_write(bus.addr, bus.data);
  if (tphase != 0) return;

  if (!sound_on) {
    s1_out = 0;
    s2_out = 0;
    s3_out = 0;
    s4_out = 0;

    out.out_r = 0;
    out.out_l = 0;
    return;
  }

  //----------
  // sweep

  bool sweep_tick = (spu_clock_ & 0b01111111111111) == 0b01000000000000;
  if (sweep_tick) {
    uint8_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    uint8_t s1_sweep_shift = (nr10 & 0b00000111) >> 0;
    bool s1_sweep_dir = (nr10 & 0b00001000) >> 3;

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

  bool length_tick = (spu_clock_ & 0b00111111111111) == 0b00000000000000;
  if (length_tick) {
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

  bool env_tick = (spu_clock_ & 0b11111111111111) == 0b11100000000000;
  if (env_tick) {

    const bool s1_env_dir = (nr12 & 0b00001000) >> 3;
    const bool s2_env_dir = (nr22 & 0b00001000) >> 3;
    const bool s4_env_dir = (nr42 & 0b00001000) >> 3;

    const uint8_t s1_env_period = (nr12 & 0b00000111) >> 0;
    const uint8_t s2_env_period = (nr22 & 0b00000111) >> 0;
    const uint8_t s4_env_period = (nr42 & 0b00000111) >> 0;

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

  if (!s1_phase_clock) {
    const uint8_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
    const uint16_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;
      
    s1_phase_clock = 2047 ^ (s1_sweep_period ? s1_sweep_freq : s1_freq);
    s1_phase = (s1_phase + 1) & 7;
  }
  else {
    s1_phase_clock--;
  }

  if (!s2_phase_clock) {
    const uint16_t s2_freq = ((nr24 << 8) | nr23) & 0x07FF;
      
    s2_phase_clock = 2047 ^ s2_freq;
    s2_phase = (s2_phase + 1) & 7;
  }
  else {
    s2_phase_clock--;
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
    s4_lfsr = (s4_lfsr >> 1) | (lfsr_bit << 14);
    if (s4_lfsr_mode) {
      s4_lfsr &= ~(1 << 5);
      s4_lfsr |= (lfsr_bit << 5);
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

  out.out_r = 0;
  out.out_l = 0;

  if (nr51 & 0b00000001) out.out_r += s1_out;
  if (nr51 & 0b00000010) out.out_r += s2_out;
  if (nr51 & 0b00000100) out.out_r += s3_out;
  if (nr51 & 0b00001000) out.out_r += s4_out;
  if (nr51 & 0b00010000) out.out_l += s1_out;
  if (nr51 & 0b00100000) out.out_l += s2_out;
  if (nr51 & 0b01000000) out.out_l += s3_out;
  if (nr51 & 0b10000000) out.out_l += s4_out;

  const uint8_t volume_r = ((nr50 & 0b00000111) >> 0) + 1;
  const uint8_t volume_l = ((nr50 & 0b01110000) >> 4) + 1;

  out.out_r *= volume_r;
  out.out_l *= volume_l;

  spu_clock = spu_clock_;
}

//-----------------------------------------------------------------------------

void SPU::bus_read(uint16_t addr, SpuOut& ret) {
  ret.oe = 1;
  switch (addr) {
  case 0xFF10: ret.data = nr10 | 0x80; break;
  case 0xFF11: ret.data = nr11 | 0x3F; break;
  case 0xFF12: ret.data = nr12 | 0x00; break;
  case 0xFF13: ret.data = nr13 | 0xFF; break;
  case 0xFF14: ret.data = nr14 | 0xBF; break;

  case 0xFF15: ret.data = nr20 | 0xFF; break;
  case 0xFF16: ret.data = nr21 | 0x3F; break;
  case 0xFF17: ret.data = nr22 | 0x00; break;
  case 0xFF18: ret.data = nr23 | 0xFF; break;
  case 0xFF19: ret.data = nr24 | 0xBF; break;
  
  case 0xFF1A: ret.data = nr30 | 0x7F; break;
  case 0xFF1B: ret.data = nr31 | 0xFF; break;
  case 0xFF1C: ret.data = nr32 | 0x9F; break;
  case 0xFF1D: ret.data = nr33 | 0xFF; break;
  case 0xFF1E: ret.data = nr34 | 0xBF; break;
  
  case 0xFF1F: ret.data = nr40 | 0xFF; break;
  case 0xFF20: ret.data = nr41 | 0xFF; break;
  case 0xFF21: ret.data = nr42 | 0x00; break;
  case 0xFF22: ret.data = nr43 | 0x00; break;
  case 0xFF23: ret.data = nr44 | 0xBF; break;

  case 0xFF24: ret.data = nr50 | 0x00; break;
  case 0xFF25: ret.data = nr51 | 0x00; break;

  case 0xFF26: {
    uint8_t bus_out_ = (nr52 & 0x80) | 0x70;
    if (s1_enable) bus_out_ |= 0b00000001;
    if (s2_enable) bus_out_ |= 0b00000010;
    if (s3_enable) bus_out_ |= 0b00000100;
    if (s4_enable) bus_out_ |= 0b00001000;
    ret.data = bus_out_;
    break;
  }

  default: {
    ret.data = 0;
    ret.oe = 0;
    break;
  }
  }

  //----------
  // wavetable

  if (addr >= 0xFF30 && addr <= 0xFF3F) {
    ret.oe = 1;
    ret.data = s3_wave[addr & 0xF];
  }
}

//-----------------------------------------------------------------------------

void SPU::bus_write(uint16_t addr, uint8_t data) {
  //----------
  // glitches n stuff

  if (addr == 0xFF12) {
    if ((nr12 & 0x08) && s1_enable) s1_env_volume = (s1_env_volume + 1) & 15;
    if ((data & 0xF8) == 0) s1_enable = false;
  }

  if (addr == 0xFF17) {
    if ((nr22 & 0x08) && s2_enable) s2_env_volume = (s2_env_volume + 1) & 15;
    if ((data & 0xF8) == 0) s2_enable = false;
  }

  if (addr == 0xFF21) {
    if ((nr42 & 0x08) && s4_enable) s4_env_volume = (s4_env_volume + 1) & 15;
    if ((data & 0xF8) == 0) s4_enable = false;
  }

  if (addr == 0xFF26) {
    if (!(data & 0x80)) reset();
  }

  //----------
  // registers

  switch (addr) {
  case 0xFF10: nr10 = data | 0b10000000; break;
  case 0xFF11: nr11 = data | 0b00000000; break;
  case 0xFF12: nr12 = data | 0b00000000; break;
  case 0xFF13: nr13 = data | 0b00000000; break;
  case 0xFF14: nr14 = data | 0b00111000; break;
  case 0xFF16: nr21 = data | 0b00000000; break;
  case 0xFF17: nr22 = data | 0b00000000; break;
  case 0xFF18: nr23 = data | 0b00000000; break;
  case 0xFF19: nr24 = data | 0b00111000; break;
  case 0xFF1A: nr30 = data | 0b01111111; break;
  case 0xFF1B: nr31 = data | 0b00000000; break;
  case 0xFF1C: nr32 = data | 0b10011111; break;
  case 0xFF1D: nr33 = data | 0b00000000; break;
  case 0xFF1E: nr34 = data | 0b00111000; break;
  case 0xFF20: nr41 = data | 0b11000000; break;
  case 0xFF21: nr42 = data | 0b00000000; break;
  case 0xFF22: nr43 = data | 0b00000000; break;
  case 0xFF23: nr44 = data | 0b00111111; break;
  case 0xFF24: nr50 = data | 0b00000000; break;
  case 0xFF25: nr51 = data | 0b00000000; break;
  case 0xFF26: nr52 = data | 0b01110000; break;
  }

  //----------
  // wavetable

  if (addr >= 0xFF30 && addr <= 0xFF3F) {
    s3_wave[addr & 0xF] = data;
  }

  //----------
  // triggers

  {
    bool s1_trigger_ = addr == 0xFF14 && (data & 0x80);
    bool s2_trigger_ = addr == 0xFF19 && (data & 0x80);
    bool s3_trigger_ = addr == 0xFF1E && (data & 0x80);
    bool s4_trigger_ = addr == 0xFF23 && (data & 0x80);

    if (s1_trigger_) {
      uint8_t s1_sweep_period = (nr10 & 0b01110000) >> 4;
      uint8_t s1_length = 64 - (nr11 & 0b00111111);
      uint8_t s1_start_volume = (nr12 & 0b11110000) >> 4;
      uint8_t s1_env_period = (nr12 & 0b00000111) >> 0;
      uint16_t s1_freq = ((nr14 << 8) | nr13) & 0x07FF;

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
      uint8_t s2_length = 64 - (nr21 & 0b00111111);
      uint8_t s2_start_volume = (nr22 & 0b11110000) >> 4;
      uint8_t s2_env_period = (nr22 & 0b00000111) >> 0;
      uint16_t s2_freq = ((nr24 << 8) | nr23) & 0x07FF;

      s2_enable = (nr22 & 0xF8) != 0;
      s2_duration = s2_length;

      s2_env_volume = (nr22 & 0x08) ? s2_start_volume : 15 ^ s2_start_volume;
      s2_env_clock = s2_env_period;

      s2_phase_clock = 2047 ^ s2_freq;
      s2_phase = 0;
    }

    if (s3_trigger_) {
      uint16_t s3_length = 256 - nr31;
      uint16_t s3_freq = ((nr34 << 8) | nr33) & 0x07FF;

      s3_enable = (nr32 != 0);
      s3_duration = s3_length;
      s3_phase_clock = 2047 ^ s3_freq;
      s3_phase = 0;
    }

    if (s4_trigger_) {
      uint8_t s4_length = 64 - (nr41 & 0b00111111);
      uint8_t s4_start_volume = (nr42 & 0b11110000) >> 4;
      uint8_t s4_env_period = (nr42 & 0b00000111) >> 0;
      uint8_t s4_phase_period = (nr43 & 0b00000111) ? (nr43 & 0b00000111) * 2 : 1;

      s4_enable = (nr42 & 0xF8) != 0;
      s4_duration = s4_length;

      s4_env_volume = (nr42 & 0x08) ? s4_start_volume : 15 ^ s4_start_volume;
      s4_env_clock = s4_env_period;

      s4_phase_clock = s4_phase_period;
      s4_lfsr = 0x7FFF;
    }
  }
}

//-----------------------------------------------------------------------------

void SPU::dump(std::string& d) const {
  sprintf(d, "nr10 %s\n", to_binary(nr10));
  sprintf(d, "nr11 %s\n", to_binary(nr11));
  sprintf(d, "nr12 %s\n", to_binary(nr12));
  sprintf(d, "nr13 %s\n", to_binary(nr13));
  sprintf(d, "nr14 %s\n", to_binary(nr14));

  sprintf(d, "nr20 %s\n", to_binary(nr20));
  sprintf(d, "nr21 %s\n", to_binary(nr21));
  sprintf(d, "nr22 %s\n", to_binary(nr22));
  sprintf(d, "nr23 %s\n", to_binary(nr23));
  sprintf(d, "nr24 %s\n", to_binary(nr24));

  sprintf(d, "nr30 %s\n", to_binary(nr30));
  sprintf(d, "nr31 %s\n", to_binary(nr31));
  sprintf(d, "nr32 %s\n", to_binary(nr32));
  sprintf(d, "nr33 %s\n", to_binary(nr33));
  sprintf(d, "nr34 %s\n", to_binary(nr34));

  sprintf(d, "nr40 %s\n", to_binary(nr40));
  sprintf(d, "nr41 %s\n", to_binary(nr41));
  sprintf(d, "nr42 %s\n", to_binary(nr42));
  sprintf(d, "nr43 %s\n", to_binary(nr43));
  sprintf(d, "nr44 %s\n", to_binary(nr44));

  sprintf(d, "nr50 %s\n", to_binary(nr50));
  sprintf(d, "nr51 %s\n", to_binary(nr51));
  sprintf(d, "nr52 %s\n", to_binary(nr52));

  const char* bar = "===============";

  uint8_t s1_volume = (nr12 & 0x08) ? s1_env_volume : 15 ^ s1_env_volume;
  sprintf(d, "s1 vol %s\n", bar + (15 - s1_volume));

  uint8_t s2_volume = (nr22 & 0x08) ? s2_env_volume : 15 ^ s2_env_volume;
  sprintf(d, "s2 vol %s\n", bar + (15 - s2_volume));

  uint8_t s3_volume = 0;
  switch ((nr32 & 0b01100000) >> 5) {
  case 0: s3_volume = 0; break;
  case 1: s3_volume = 15; break;
  case 2: s3_volume = 7; break;
  case 3: s3_volume = 3; break;
  }
  sprintf(d, "s3 vol %s\n", bar + (15 - s3_volume));

  uint8_t s4_volume = (nr42 & 0x08) ? s4_env_volume : 15 ^ s4_env_volume;
  sprintf(d, "s4 vol %s\n", bar + (15 - s4_volume));

  /*
  char buf[33];
  for (int i = 0; i < 16; i++) {
    uint8_t a = (s3_wave[i] & 0x0F) >> 0;
    uint8_t b = (s3_wave[i] & 0xF0) >> 4;

    buf[2 * i + 0] = 'A' + a;
    buf[2 * i + 1] = 'A' + b;
  }

  buf[32] = 0;
  sprintf(d, "[%s]\n", buf);
  */
}

//-----------------------------------------------------------------------------
