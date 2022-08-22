#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

class MetroBoySPU2 {
public:

  static int b1(int x, int o) { return (x >> o) & ((1 << 1) - 1); }
  static int b2(int x, int o) { return (x >> o) & ((1 << 2) - 1); }
  static int b3(int x, int o) { return (x >> o) & ((1 << 3) - 1); }
  static int b4(int x, int o) { return (x >> o) & ((1 << 4) - 1); }
  static int b5(int x, int o) { return (x >> o) & ((1 << 5) - 1); }
  static int b6(int x, int o) { return (x >> o) & ((1 << 6) - 1); }
  static int b7(int x, int o) { return (x >> o) & ((1 << 7) - 1); }
  static int b8(int x, int o) { return (x >> o) & ((1 << 8) - 1); }

  uint16_t out_r;
  uint16_t out_l;

  uint8_t ack_data;
  bool ack_valid;

  //----------------------------------------

  void tock_out() {
    uint8_t s3_volume_shift = 0;
    switch (b2(nr32, 5)) {
      case 0: s3_volume_shift = 4; break;
      case 1: s3_volume_shift = 0; break;
      case 2: s3_volume_shift = 1; break;
      case 3: s3_volume_shift = 2; break;
    }

    //----------
    // output

    int s1_out = 0;
    int s2_out = 0;
    int s3_out = 0;
    int s4_out = 0;

    auto s1_duty = b2(nr11, 6);
    auto s2_duty = b2(nr21, 6);

    auto s3_power = b1(nr30, 7);
    auto s3_vol   = b2(nr32, 5);

    if (s1_running) {
      s1_duty = s1_duty ? s1_duty * 2 : 1;
      s1_out = (s1_phase < s1_duty) * s1_env_vol;
    }

    if (s2_running) {
      s2_duty = s2_duty ? s2_duty * 2 : 1;
      s2_out = (s2_phase < s2_duty) * s2_env_vol;
    }

    if (s3_running && s3_power) {
      uint8_t s3_sample = s3_wave[s3_phase >> 1];
      s3_out = (s3_phase & 1) ? b4(s3_sample, 0) : b4(s3_sample, 4);

      switch (s3_vol) {
      case 0: s3_out >>= 4;  break;
      case 1: s3_out >>= 0; break;
      case 2: s3_out >>= 1;  break;
      case 3: s3_out >>= 2;  break;
      }
    }

    if (s4_running) {
      s4_out = b1(s4_lfsr, 15) * s4_env_vol;
    }

    //----------
    // mixer & master volume

    out_r = 0;
    out_l = 0;

    if (bit(nr51, 0)) out_r = out_r + s1_out;
    if (bit(nr51, 1)) out_r = out_r + s2_out;
    if (bit(nr51, 2)) out_r = out_r + s3_out;
    if (bit(nr51, 3)) out_r = out_r + s4_out;
    if (bit(nr51, 4)) out_l = out_l + s1_out;
    if (bit(nr51, 5)) out_l = out_l + s2_out;
    if (bit(nr51, 6)) out_l = out_l + s3_out;
    if (bit(nr51, 7)) out_l = out_l + s4_out;

    uint8_t volume_r = b3(nr50, 0);
    uint8_t volume_l = b3(nr50, 4);

    out_r = out_r * volume_r;
    out_l = out_l * volume_l;
  }

  //----------------------------------------

  void tick_reset() {
    nr10 = 0x80;
    nr11 = 0xBF;
    nr12 = 0xF3;
    nr13 = 0xFF;
    nr14 = 0xBF;

    nr20 = 0x00;
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

    for (int i = 0; i < 16; i++) {
      s3_wave[i] = 0;
    }

    s4_lfsr = 0x7FFF;

    spu_clock = 0;

    s1_running = 1;
    s2_running = 0;
    s3_running = 0;
    s4_running = 0;

    s1_sweep_timer = 0;
    s1_sweep_freq = 0;

    s1_len_timer = 0;
    s2_len_timer = 0;
    s3_len_timer = 0;
    s4_len_timer = 0;

    s1_env_timer = 0;
    s2_env_timer = 0;
    s4_env_timer = 0;

    s1_env_vol = 0;
    s2_env_vol = 0;
    s4_env_vol = 0;

    s1_freq_timer = 0;
    s2_freq_timer = 0;
    s3_freq_timer = 0;
    s4_freq_timer = 0;

    s1_phase = 0;
    s2_phase = 0;
    s3_phase = 0;
  }

  //----------------------------------------

  void tick(bool reset, uint16_t addr, uint8_t data, bool we) {
    if (reset) {
      tick_reset();
      return;
    }

    auto spu_clock_old = spu_clock;
    auto spu_clock_new = (spu_clock + 1) & 0x3FFF;
    
    bool sweep_tick =  (spu_clock_new & 0b01111111111111) == 0b01000000000000;
    bool length_tick = (spu_clock_new & 0b00111111111111) == 0b00000000000000;
    bool env_tick =    (spu_clock_new & 0b11111111111111) == 0b11100000000000;

    auto s1_trig = b1(nr14, 7);
    auto s2_trig = b1(nr24, 7);
    auto s3_trig = b1(nr34, 7);
    auto s4_trig = b1(nr44, 7);

    auto s1_len_timer_init = b6(nr11, 0);
    auto s2_len_timer_init = b6(nr21, 0);
    auto s3_len_timer_init = b8(nr31, 0);
    auto s4_len_timer_init = b6(nr41, 0);

    auto s1_len_en = b1(nr14, 6);
    auto s2_len_en = b1(nr24, 6);
    auto s3_len_en = b1(nr34, 6);
    auto s4_len_en = b1(nr44, 6);

    auto s1_env_timer_init = b3(nr12, 0);
    auto s2_env_timer_init = b3(nr22, 0);
    auto s4_env_timer_init = b3(nr42, 0);

    auto s1_env_add = b1(nr12, 3);
    auto s2_env_add = b1(nr22, 3);
    auto s4_env_add = b1(nr42, 3);

    auto s1_env_vol_init = b4(nr12, 4);
    auto s2_env_vol_init = b4(nr22, 4);
    auto s4_env_vol_init = b4(nr42, 4);

    auto s1_sweep_shift  = b3(nr10, 0);
    auto s1_sweep_dir    = b1(nr10, 3);
    auto s1_sweep_timer_init = b3(nr10, 4);
    
    auto s1_freq_timer_init = (b3(nr14, 0) << 8) | nr13;
    auto s2_freq_timer_init = (b3(nr24, 0) << 8) | nr23;
    auto s3_freq_timer_init = (b3(nr34, 0) << 8) | nr33;
    auto s4_freq_timer_init = b3(nr43, 0);

    auto s4_mode  = b1(nr43, 3);
    auto s4_shift = b4(nr43, 4);

    //----------
    // s1 clock

    s1_freq_timer = (s1_freq_timer + 1) & 0x7FF;
    if (s1_freq_timer == 0) {
      s1_phase = (s1_phase + 1) & 7;
      s1_freq_timer = s1_sweep_timer_init ? s1_sweep_freq : s1_freq_timer_init;
    }

    s2_freq_timer = (s2_freq_timer + 1) & 0x7FF;
    if (s2_freq_timer == 0) {
      s2_phase = (s2_phase + 1) & 7;
      s2_freq_timer = s2_freq_timer_init;
    }

    for (int i = 0; i < 2; i++) {
      // s3 clock - we run this twice because this is ticking at 1 mhz
      s3_freq_timer = (s3_freq_timer + 1) & 0x7FF;
      if (s3_freq_timer == 0) {
        s3_phase = (s3_phase + 1) & 31;
        s3_freq_timer = s3_freq_timer_init;
      }
    }

    if (posedge(bit(spu_clock_old, s4_shift), bit(spu_clock_new, s4_shift))) {
      if (s4_freq_timer) {
        s4_freq_timer--;
      }
      else {
        auto lfsr_bit = bit(s4_lfsr, 14) ^ bit(s4_lfsr, 15);
        if (s4_mode) s4_lfsr = (s4_lfsr & 0xFEFF) | (lfsr_bit << 8);
        s4_lfsr = (s4_lfsr << 1) | lfsr_bit;
        s4_freq_timer = s4_freq_timer_init;
      }
    }

    //----------
    // sweep

    if (sweep_tick && s1_sweep_timer_init && s1_sweep_shift) {
      if (s1_sweep_timer) {
        s1_sweep_timer = s1_sweep_timer - 1;
      }
      else {
        uint16_t delta = s1_sweep_freq >> s1_sweep_shift;
        s1_sweep_freq = s1_sweep_freq + (s1_sweep_dir ? -delta : +delta);
        if (s1_sweep_freq > 2047) s1_running = 0;
        s1_sweep_timer = s1_sweep_timer_init;
      }
    }

    //----------
    // length

    if (length_tick) {
      if (s1_running && s1_len_en) {
        s1_len_timer++;
        if (s1_len_timer == 64) {
          s1_len_timer = 0;
          s1_running = 0;
        }
      }
      if (s2_running && s2_len_en) {
        s2_len_timer++;
        if (s2_len_timer == 64) {
          s2_len_timer = 0;
          s2_running = 0;
        }
      }
      if (s3_running && s3_len_en) {
        s3_len_timer++;
        if (s3_len_timer == 256) {
          s3_len_timer = 0;
          s3_running = 0;
        }
      }
      if (s4_running && s4_len_en) {
        s4_len_timer++;
        if (s4_len_timer == 64) {
          s4_len_timer = 0;
          s4_running = 0;
        }
      }
    }

    //----------
    // env

    if (env_tick) {
      if (s1_env_timer_init) {
        if (s1_env_timer) {
          s1_env_timer = s1_env_timer - 1;
        }
        else {
          s1_env_timer = s1_env_timer_init;
          if (s1_env_add) { if (s1_env_vol < 15) s1_env_vol++; }
          else            { if (s1_env_vol >  0) s1_env_vol--; }
        }
      }

      if (s2_env_timer_init) {
        if (s2_env_timer) {
          s2_env_timer = s2_env_timer - 1;
        }
        else {
          s2_env_timer = s2_env_timer_init;
          if (s2_env_add) { if (s2_env_vol < 15) s2_env_vol++; }
          else            { if (s2_env_vol >  0) s2_env_vol--; }
        }
      }

      if (s4_env_timer_init) {
        if (s4_env_timer) {
          s4_env_timer = s4_env_timer - 1;
        }
        else {
          s4_env_timer = s4_env_timer_init;
          if (s4_env_add) { if (s4_env_vol < 15) s4_env_vol++; }
          else            { if (s4_env_vol >  0) s4_env_vol--; }
        }
      }
    }

    spu_clock = spu_clock_new;

    //----------
    // Wavetable writes

    if (we && addr >= 0xFF30 && addr <= 0xFF3F) {
      s3_wave[addr & 0xF] = data;
    }

    //----------
    // Triggers

    bool s1_trigger = we && addr == 0xFF14 && (data & 0x80);
    bool s2_trigger = we && addr == 0xFF19 && (data & 0x80);
    bool s3_trigger = we && addr == 0xFF1E && (data & 0x80);
    bool s4_trigger = we && addr == 0xFF23 && (data & 0x80);

    if (s1_trigger && s1_env_vol_init) {
      s1_running     = 1;
      s1_len_timer   = s1_len_timer_init;
      s1_sweep_timer = s1_sweep_timer_init;
      s1_sweep_freq  = s1_freq_timer_init;
      s1_env_vol     = s1_env_vol_init;
      s1_env_timer   = s1_env_timer_init;
      s1_freq_timer  = s1_freq_timer_init;
      s1_phase       = 0;
    }

    if (s2_trigger && s2_env_vol_init) {
      s2_running    = 1;
      s2_len_timer  = s2_len_timer_init;
      s2_env_vol    = s2_env_vol_init;
      s2_env_timer  = s2_env_timer_init;
      s2_freq_timer = s2_freq_timer_init;
      s2_phase      = 0;
    }

    if (s3_trigger) {
      s3_running    = 1;
      s3_len_timer  = s3_len_timer_init;
      s3_freq_timer = s3_freq_timer_init;
      s3_phase      = 0;
    }

    if (s4_trigger && s4_env_vol_init) {
      s4_running    = 1;
      s4_len_timer  = s4_len_timer_init;
      s4_env_vol    = s4_env_vol_init;
      s4_env_timer  = s4_env_timer_init;
      s4_freq_timer = s4_freq_timer_init;
      s4_lfsr       = 0x7FFF;
    }

    //----------
    // Register writes

    if (we && addr >= 0xFF10 && addr <= 0xFF26) {
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

        case 0xFF20: {
          nr41 = data | 0b11000000;
          printf("nr41 0x%02x len %d\n", nr41, 64 - b6(nr41, 0));
          break;
        }
        case 0xFF21: {
          nr42 = data | 0b00000000;
          printf("nr42 0x%02x env period %d add %d init %d\n", nr42, b3(nr42, 0), b1(nr42, 3), b4(nr42, 4));
          break;
        }
        case 0xFF22: {
          nr43 = data | 0b00000000;
          printf("nr43 0x%02x lfsr delay %d width %d clock mux %d\n", nr43, b3(nr43, 0), b1(nr43, 3), b4(nr43, 4));
          break;
        }
        case 0xFF23: {
          nr44 = data | 0b00111111;
          printf("nr44 0x%02x len en %d trig %d\n", nr44, b1(nr44, 6), b1(nr44, 7));
          break;
        }

        case 0xFF24: nr50 = data | 0b00000000; break;
        case 0xFF25: nr51 = data | 0b00000000; break;
        case 0xFF26: nr52 = data | 0b01110000; break;
      }
    }
  }

  //-----------------------------------------------------------------------------

  void dump(Dumper& d) const {
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

    uint8_t s3_env_vol = 0;
    switch (b2(nr32, 5)) {
    case 0: s3_env_vol = 0; break;
    case 1: s3_env_vol = 15; break;
    case 2: s3_env_vol = 7; break;
    case 3: s3_env_vol = 3; break;
    }

    d("s1 running %d\n", s1_running);
    d("s2 running %d\n", s2_running);
    d("s3 running %d\n", s3_running);
    d("s4 running %d\n", s4_running);

    d("s1 len %d\n", s1_len_timer);
    d("s2 len %d\n", s2_len_timer);
    d("s3 len %d\n", s3_len_timer);
    d("s4 len %d\n", s4_len_timer);

    d("s1 env timer %d\n", s1_env_timer);
    d("s2 env timer %d\n", s2_env_timer);
    d("s4 env timer %d\n", s4_env_timer);

    d("s1 vol %2d %s\n", s1_env_vol, bar + 15 - s1_env_vol);
    d("s2 vol %2d %s\n", s2_env_vol, bar + 15 - s2_env_vol);
    d("s3 vol %2d %s\n", s3_env_vol, bar + 15 - s3_env_vol);
    d("s4 vol %2d %s\n", s4_env_vol, bar + 15 - s4_env_vol);

    d("s1 sweep timer %2d\n", s1_sweep_timer);
    d("s1 sweep freq  %2d\n", s1_sweep_freq);

    d("s1 freq timer %d\n", s1_freq_timer);
    d("s2 freq timer %d\n", s2_freq_timer);
    d("s3 freq timer %d\n", s3_freq_timer);
    d("s4 freq timer %d\n", s4_freq_timer);

    d("s1 phase %d\n", s1_phase);
    d("s2 phase %d\n", s2_phase);
    d("s3 phase %d\n", s3_phase);

    d("s4 lfsr 0x%04x\n", s4_lfsr);

    char buf[33];
    for (int i = 0; i < 16; i++) {
      uint8_t a = (s3_wave[i] & 0x0F) >> 0;
      uint8_t b = (s3_wave[i] & 0xF0) >> 4;

      buf[2 * i + 0] = a > 9 ? 'A' + a - 10 : '0' + a;
      buf[2 * i + 1] = b > 9 ? 'B' + b - 10 : '0' + b;
    }

    buf[32] = 0;
    d("[%s]\n", buf);
  }

private:

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
  uint8_t nr52;

  uint16_t spu_clock;

  bool s1_running;
  bool s2_running;
  bool s3_running;
  bool s4_running;

  uint8_t  s1_len_timer;
  uint8_t  s2_len_timer;
  uint16_t s3_len_timer;
  uint8_t  s4_len_timer;

  uint8_t  s1_env_timer;
  uint8_t  s2_env_timer;
  uint8_t  s4_env_timer;

  uint8_t  s1_env_vol;
  uint8_t  s2_env_vol;
  uint8_t  s4_env_vol;

  uint8_t  s1_sweep_timer;
  uint16_t s1_sweep_freq;

  uint16_t s1_freq_timer;
  uint16_t s2_freq_timer;
  uint16_t s3_freq_timer;
  uint16_t s4_freq_timer;

  uint8_t  s1_phase;
  uint8_t  s2_phase;
  uint8_t  s3_phase;

  uint8_t  s3_wave[16];
  uint16_t s4_lfsr;
};
