#include "MetroBoyLib/SPU.h"
#include "CoreLib/Constants.h"
#include <assert.h>

const char* byte_to_bits(uint8_t b);

//-----------------------------------------------------------------------------

void SPU::reset() {
  *this = {0};

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

void SPU::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;
  if (req.read) bus_read(req, ack);
}

//-----------------------------------------------------------------------------

void SPU::tock(int phase_total, const Req& req) {
  if (DELTA_GH && req.write) bus_write(req);

  if (!DELTA_HA) return;

  bool sound_on = (nr52 & 0x80);
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

  bool sweep_tick =  (spu_clock_ & 0b01111111111111) == 0b01000000000000;
  bool length_tick = (spu_clock_ & 0b00111111111111) == 0b00000000000000;
  bool env_tick =    (spu_clock_ & 0b11111111111111) == 0b11100000000000;

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
}

//-----------------------------------------------------------------------------

void SPU::bus_read(const Req& req, Ack& ack) const {
  if (req.addr < 0xFF10) return;
  if (req.addr > 0xFF26 && req.addr < 0xFF30) return;
  if (req.addr > 0xFF3F) return;

  ack.addr = req.addr;
  ack.data_lo = 0;
  ack.read++;

  //----------
  // wavetable

  if (req.addr >= 0xFF30 && req.addr <= 0xFF3F) {
    ack.data_lo = s3_wave[req.addr & 0xF];
    return;
  }

  switch (req.addr) {
  case 0xFF10: ack.data_lo = nr10 | 0x80; break;
  case 0xFF11: ack.data_lo = nr11 | 0x3F; break;
  case 0xFF12: ack.data_lo = nr12 | 0x00; break;
  case 0xFF13: ack.data_lo = nr13 | 0xFF; break;
  case 0xFF14: ack.data_lo = nr14 | 0xBF; break;

  case 0xFF15: ack.data_lo = nr20 | 0xFF; break;
  case 0xFF16: ack.data_lo = nr21 | 0x3F; break;
  case 0xFF17: ack.data_lo = nr22 | 0x00; break;
  case 0xFF18: ack.data_lo = nr23 | 0xFF; break;
  case 0xFF19: ack.data_lo = nr24 | 0xBF; break;

  case 0xFF1A: ack.data_lo = nr30 | 0x7F; break;
  case 0xFF1B: ack.data_lo = nr31 | 0xFF; break;
  case 0xFF1C: ack.data_lo = nr32 | 0x9F; break;
  case 0xFF1D: ack.data_lo = nr33 | 0xFF; break;
  case 0xFF1E: ack.data_lo = nr34 | 0xBF; break;

  case 0xFF1F: ack.data_lo = nr40 | 0xFF; break;
  case 0xFF20: ack.data_lo = nr41 | 0xFF; break;
  case 0xFF21: ack.data_lo = nr42 | 0x00; break;
  case 0xFF22: ack.data_lo = nr43 | 0x00; break;
  case 0xFF23: ack.data_lo = nr44 | 0xBF; break;

  case 0xFF24: ack.data_lo = nr50 | 0x00; break;
  case 0xFF25: ack.data_lo = nr51 | 0x00; break;

  case 0xFF26: {
    uint8_t bus_out_ = (nr52 & 0x80) | 0x70;
    if (s1_enable) bus_out_ |= 0b00000001;
    if (s2_enable) bus_out_ |= 0b00000010;
    if (s3_enable) bus_out_ |= 0b00000100;
    if (s4_enable) bus_out_ |= 0b00001000;
    ack.data_lo = bus_out_;
    break;
  }
  }
}

//-----------------------------------------------------------------------------

void SPU::bus_write(const Req& req) {
  bool sound_on = (nr52 & 0x80);

  if (!sound_on) {
    if (req.addr == 0xFF26) {
      nr52 = (uint8_t)req.data_lo | 0b01110000;
      if (nr52 & 0x80) reset();
    }
    return;
  }

  //----------
  // wavetable

  if (req.addr >= 0xFF30 && req.addr <= 0xFF3F) {
    s3_wave[req.addr & 0xF] = (uint8_t)req.data_lo;
    return;
  }

  //----------
  // glitches n stuff

  if (req.addr == 0xFF12) {
    if ((nr12 & 0x08) && s1_enable) s1_env_volume = (s1_env_volume + 1) & 15;
    if ((req.data_lo & 0xF8) == 0) s1_enable = false;
  }

  if (req.addr == 0xFF17) {
    if ((nr22 & 0x08) && s2_enable) s2_env_volume = (s2_env_volume + 1) & 15;
    if ((req.data_lo & 0xF8) == 0) s2_enable = false;
  }

  if (req.addr == 0xFF21) {
    if ((nr42 & 0x08) && s4_enable) s4_env_volume = (s4_env_volume + 1) & 15;
    if ((req.data_lo & 0xF8) == 0) s4_enable = false;
  }

  //----------
  // registers

  switch (req.addr) {
  case 0xFF10: nr10 = (uint8_t)req.data_lo | 0b10000000; break;
  case 0xFF11: nr11 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF12: nr12 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF13: nr13 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF14: nr14 = (uint8_t)req.data_lo | 0b00111000; break;
  case 0xFF16: nr21 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF17: nr22 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF18: nr23 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF19: nr24 = (uint8_t)req.data_lo | 0b00111000; break;
  case 0xFF1A: nr30 = (uint8_t)req.data_lo | 0b01111111; break;
  case 0xFF1B: nr31 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF1C: nr32 = (uint8_t)req.data_lo | 0b10011111; break;
  case 0xFF1D: nr33 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF1E: nr34 = (uint8_t)req.data_lo | 0b00111000; break;
  case 0xFF20: nr41 = (uint8_t)req.data_lo | 0b11000000; break;
  case 0xFF21: nr42 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF22: nr43 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF23: nr44 = (uint8_t)req.data_lo | 0b00111111; break;
  case 0xFF24: nr50 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF25: nr51 = (uint8_t)req.data_lo | 0b00000000; break;
  case 0xFF26: nr52 = (uint8_t)req.data_lo | 0b01110000; break;
  default: return;
  }

  //----------
  // triggers

  {
    bool s1_trigger_ = req.addr == 0xFF14 && (req.data_lo & 0x80);
    bool s2_trigger_ = req.addr == 0xFF19 && (req.data_lo & 0x80);
    bool s3_trigger_ = req.addr == 0xFF1E && (req.data_lo & 0x80);
    bool s4_trigger_ = req.addr == 0xFF23 && (req.data_lo & 0x80);

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

void SPU::dump(Dumper& d) const {
  d("\002--------------SPU--------------\001\n");

  d("NR10 %s\n", byte_to_bits(nr10));
  d("NR11 %s\n", byte_to_bits(nr11));
  d("NR12 %s\n", byte_to_bits(nr12));
  d("NR13 %s\n", byte_to_bits(nr13));
  d("NR14 %s\n", byte_to_bits(nr14));
  d("NR20 %s\n", byte_to_bits(nr20));
  d("NR21 %s\n", byte_to_bits(nr21));
  d("NR22 %s\n", byte_to_bits(nr22));
  d("NR23 %s\n", byte_to_bits(nr23));
  d("NR24 %s\n", byte_to_bits(nr24));
  d("NR30 %s\n", byte_to_bits(nr30));
  d("NR31 %s\n", byte_to_bits(nr31));
  d("NR32 %s\n", byte_to_bits(nr32));
  d("NR33 %s\n", byte_to_bits(nr33));
  d("NR34 %s\n", byte_to_bits(nr34));
  d("NR40 %s\n", byte_to_bits(nr40));
  d("NR41 %s\n", byte_to_bits(nr41));
  d("NR42 %s\n", byte_to_bits(nr42));
  d("NR43 %s\n", byte_to_bits(nr43));
  d("NR44 %s\n", byte_to_bits(nr44));
  d("NR50 %s\n", byte_to_bits(nr50));
  d("NR51 %s\n", byte_to_bits(nr51));
  d("NR52 %s\n", byte_to_bits(nr52));

  const char* bar = "===============";

  uint8_t s1_volume = (nr12 & 0x08) ? s1_env_volume : 15 ^ s1_env_volume;
  d("s1 vol %s\n", bar + (15 - s1_volume));

  uint8_t s2_volume = (nr22 & 0x08) ? s2_env_volume : 15 ^ s2_env_volume;
  d("s2 vol %s\n", bar + (15 - s2_volume));

  uint8_t s3_volume = 0;
  switch ((nr32 & 0b01100000) >> 5) {
  case 0: s3_volume = 0; break;
  case 1: s3_volume = 15; break;
  case 2: s3_volume = 7; break;
  case 3: s3_volume = 3; break;
  }
  d("s3 vol %s\n", bar + (15 - s3_volume));

  uint8_t s4_volume = (nr42 & 0x08) ? s4_env_volume : 15 ^ s4_env_volume;
  d("s4 vol %s\n", bar + (15 - s4_volume));

  /*
  char buf[33];
  for (int i = 0; i < 16; i++) {
    uint8_t a = (s3_wave[i] & 0x0F) >> 0;
    uint8_t b = (s3_wave[i] & 0xF0) >> 4;

    buf[2 * i + 0] = 'A' + a;
    buf[2 * i + 1] = 'A' + b;
  }

  buf[32] = 0;
  d("[%s]\n", buf);
  */
}

//-----------------------------------------------------------------------------
