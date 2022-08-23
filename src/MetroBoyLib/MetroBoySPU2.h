#pragma once
#include "CoreLib/Types.h"

#include "CoreLib/metron_tools.h"

//-----------------------------------------------------------------------------

class MetroBoySPU2 {
public:

  logic<9> out_r;
  logic<9> out_l;

  logic<8> ack_data;
  logic<1> ack_valid;

  logic<1> posedge_bit(int t_old, int t_new, int b) {
    return posedge(b1(t_old, b), b1(t_new, b));
  }

  //----------------------------------------

  void tock_out() {
    logic<3> s3_volume_shift = 0;
    switch (b2(nr32, 5)) {
      case 0: s3_volume_shift = 4; break;
      case 1: s3_volume_shift = 0; break;
      case 2: s3_volume_shift = 1; break;
      case 3: s3_volume_shift = 2; break;
    }

    //----------
    // output

    logic<4> s1_out = 0;
    logic<4> s2_out = 0;
    logic<4> s3_out = 0;
    logic<4> s4_out = 0;

    logic<2> s1_duty = b2(nr11, 6);
    logic<2> s2_duty = b2(nr21, 6);

    logic<1> s3_power = b1(nr30, 7);
    logic<2> s3_vol   = b2(nr32, 5);

    if (s1_running) {
      switch(s1_duty) {
        case 0: s1_out = b1(0b10000000, s1_phase); break;
        case 1: s1_out = b1(0b10000001, s1_phase); break;
        case 2: s1_out = b1(0b11100001, s1_phase); break;
        case 3: s1_out = b1(0b01111110, s1_phase); break;
      }
      s1_out = s1_out * s1_env_vol;
    }

    if (s2_running) {
      switch(s2_duty) {
        case 0: s2_out = b1(0b10000000, s2_phase); break;
        case 1: s2_out = b1(0b10000001, s2_phase); break;
        case 2: s2_out = b1(0b11100001, s2_phase); break;
        case 3: s2_out = b1(0b01111110, s2_phase); break;
      }
      s2_out = s2_out * s2_env_vol;
    }

    if (s3_running && s3_power) {
      logic<8> s3_sample = s3_wave[s3_phase >> 1];
      s3_out = (s3_phase & 1) ? b4(s3_sample, 0) : b4(s3_sample, 4);

      switch (s3_vol) {
      case 0: s3_out = s3_out >> 4;  break;
      case 1: s3_out = s3_out >> 0; break;
      case 2: s3_out = s3_out >> 1;  break;
      case 3: s3_out = s3_out >> 2;  break;
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

    logic<4> volume_r = b3(nr50, 0) + 1;
    logic<4> volume_l = b3(nr50, 4) + 1;

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

    s1_running = 0;
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

  void tick(logic<1> reset, logic<16> addr, logic<8> data, logic<1> we) {
    if (reset) {
      tick_reset();
      return;
    }

    logic<16> spu_clock_old = spu_clock;
    logic<16> spu_clock_new = (spu_clock + 1) & 0x3FFF;
    
    logic<1> sweep_tick =  (spu_clock_new & 0b01111111111111) == 0b01000000000000;
    logic<1> length_tick = (spu_clock_new & 0b00111111111111) == 0b00000000000000;
    logic<1> env_tick =    (spu_clock_new & 0b11111111111111) == 0b11100000000000;

    logic<1> s1_trig = b1(nr14, 7);
    logic<1> s2_trig = b1(nr24, 7);
    logic<1> s3_trig = b1(nr34, 7);
    logic<1> s4_trig = b1(nr44, 7);

    logic<6> s1_len_timer_init = b6(nr11, 0);
    logic<6> s2_len_timer_init = b6(nr21, 0);
    logic<8> s3_len_timer_init = b8(nr31, 0);
    logic<6> s4_len_timer_init = b6(nr41, 0);

    logic<1> s1_len_en = b1(nr14, 6);
    logic<1> s2_len_en = b1(nr24, 6);
    logic<1> s3_len_en = b1(nr34, 6);
    logic<1> s4_len_en = b1(nr44, 6);

    logic<3> s1_env_timer_init = b3(nr12, 0);
    logic<3> s2_env_timer_init = b3(nr22, 0);
    logic<3> s4_env_timer_init = b3(nr42, 0);

    logic<1> s1_env_add = b1(nr12, 3);
    logic<1> s2_env_add = b1(nr22, 3);
    logic<1> s4_env_add = b1(nr42, 3);

    logic<4> s1_env_vol_init = b4(nr12, 4);
    logic<4> s2_env_vol_init = b4(nr22, 4);
    logic<4> s4_env_vol_init = b4(nr42, 4);

    logic<3> s1_sweep_shift  = b3(nr10, 0);
    logic<1> s1_sweep_dir    = b1(nr10, 3);
    logic<3> s1_sweep_timer_init = b3(nr10, 4);
    
    logic<11> s1_freq_timer_init = (b3(nr14, 0) << 8) | nr13;
    logic<11> s2_freq_timer_init = (b3(nr24, 0) << 8) | nr23;
    logic<11> s3_freq_timer_init = (b3(nr34, 0) << 8) | nr33;
    logic<3>  s4_freq_timer_init = b3(nr43, 0);

    logic<1> s4_mode  = b1(nr43, 3);
    logic<4> s4_shift = b4(nr43, 4);

    //----------
    // s1 clock

    s1_freq_timer = (s1_freq_timer + 1) & 0x7FF;
    if (s1_freq_timer == 0) {
      s1_phase = (s1_phase + 1) & 7;
      s1_freq_timer = s1_sweep_timer_init ? s1_sweep_freq : s1_freq_timer_init;
    }

    //----------
    // s2 clock

    s2_freq_timer = (s2_freq_timer + 1) & 0x7FF;
    if (s2_freq_timer == 0) {
      s2_phase = (s2_phase + 1) & 7;
      s2_freq_timer = s2_freq_timer_init;
    }

    //----------
    // s3 clock - we run this twice because this is ticking at 1 mhz

    for (int i = 0; i < 2; i++) {
      s3_freq_timer = (s3_freq_timer + 1) & 0x7FF;
      if (s3_freq_timer == 0) {
        s3_phase = (s3_phase + 1) & 31;
        s3_freq_timer = s3_freq_timer_init;
      }
    }

    //----------
    // lfsr

    if (posedge_bit(spu_clock_old, spu_clock_new, s4_shift + 1)) {
      if (s4_freq_timer) {
        s4_freq_timer = s4_freq_timer - 1;
      }
      else {
        logic<1> new_bit = b1(s4_lfsr, 15) ^ b1(s4_lfsr, 14);
        s4_lfsr = s4_lfsr << 1;

        logic<16> mask = s4_mode ? 0b0000001000000010: 0b0000000000000010;

        s4_lfsr = s4_lfsr & ~mask;
        s4_lfsr = s4_lfsr | (new_bit ? mask : b16(0));
        
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
        logic<16> delta = s1_sweep_freq >> s1_sweep_shift;
        s1_sweep_freq = s1_sweep_freq + (s1_sweep_dir ? -delta : +delta);
        if (s1_sweep_freq > 2047) s1_running = 0;
        s1_sweep_timer = s1_sweep_timer_init;
      }
    }

    //----------
    // length

    if (length_tick) {
      if (s1_running && s1_len_en) {
        if (s1_len_timer == 63) {
          s1_len_timer = 0;
          s1_running = 0;
          
        }
        else {
          s1_len_timer = s1_len_timer + 1;
        }
      }
      if (s2_running && s2_len_en) {
        if (s2_len_timer == 63) {
          s2_len_timer = 0;
          s2_running = 0;
        }
        else {
          s2_len_timer = s2_len_timer + 1;
        }
      }
      if (s3_running && s3_len_en) {
        if (s3_len_timer == 255) {
          s3_len_timer = 0;
          s3_running = 0;
        }
        else {
          s3_len_timer = s3_len_timer + 1;
        }
      }
      if (s4_running && s4_len_en) {
        if (s4_len_timer == 63) {
          s4_len_timer = 0;
          s4_running = 0;
        }
        else {
          s4_len_timer = s4_len_timer + 1;
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
          if (s1_env_add) { if (s1_env_vol < 15) s1_env_vol = s1_env_vol + 1; }
          else            { if (s1_env_vol >  0) s1_env_vol = s1_env_vol - 1; }
        }
      }

      if (s2_env_timer_init) {
        if (s2_env_timer) {
          s2_env_timer = s2_env_timer - 1;
        }
        else {
          s2_env_timer = s2_env_timer_init;
          if (s2_env_add) { if (s2_env_vol < 15) s2_env_vol = s2_env_vol + 1; }
          else            { if (s2_env_vol >  0) s2_env_vol = s2_env_vol - 1; }
        }
      }

      if (s4_env_timer_init) {
        if (s4_env_timer) {
          s4_env_timer = s4_env_timer - 1;
        }
        else {
          s4_env_timer = s4_env_timer_init;
          if (s4_env_add) { if (s4_env_vol < 15) s4_env_vol = s4_env_vol + 1; }
          else            { if (s4_env_vol >  0) s4_env_vol = s4_env_vol - 1; }
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

    logic<1> s1_trigger = we && addr == 0xFF14 && (data & 0x80);
    logic<1> s2_trigger = we && addr == 0xFF19 && (data & 0x80);
    logic<1> s3_trigger = we && addr == 0xFF1E && (data & 0x80);
    logic<1> s4_trigger = we && addr == 0xFF23 && (data & 0x80);

    if (s1_trigger && (s1_env_vol_init || s1_env_add)) {
      s1_running     = 1;
      s1_len_timer   = s1_len_timer_init;
      s1_sweep_timer = s1_sweep_timer_init;
      s1_sweep_freq  = s1_freq_timer_init;
      s1_env_vol     = s1_env_vol_init;
      s1_env_timer   = s1_env_timer_init;
      s1_freq_timer  = s1_freq_timer_init;
      s1_phase       = 0;
    }

    if (s2_trigger && (s2_env_vol_init || s2_env_add)) {
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

    if (s4_trigger && (s4_env_vol_init || s4_env_add)) {
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
          //printf("nr41 0x%02x len 64 - %d\n", nr41, b6(nr41, 0));
          break;
        }
        case 0xFF21: {
          nr42 = data | 0b00000000;
          //printf("nr42 0x%02x env period %d add %d init %d\n", nr42, b3(nr42, 0), b1(nr42, 3), b4(nr42, 4));
          break;
        }
        case 0xFF22: {
          nr43 = data | 0b00000000;
          //printf("nr43 0x%02x lfsr delay %d width %d clock mux %d\n", nr43, b3(nr43, 0), b1(nr43, 3), b4(nr43, 4));
          break;
        }
        case 0xFF23: {
          nr44 = data | 0b00111111;
          //printf("nr44 0x%02x len en %d trig %d\n", nr44, b1(nr44, 6), b1(nr44, 7));
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

    logic<4> s3_env_vol = 0;
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
      logic<4> a = b4(s3_wave[i], 4);
      logic<4> b = b4(s3_wave[i], 0);

      buf[2 * i + 0] = a > 9 ? 'A' + a - 10 : '0' + a;
      buf[2 * i + 1] = b > 9 ? 'B' + b - 10 : '0' + b;
    }

    buf[32] = 0;
    d("[%s]\n", buf);
  }

private:

  logic<8> nr10;
  logic<8> nr11;
  logic<8> nr12;
  logic<8> nr13;
  logic<8> nr14;

  logic<8> nr20;
  logic<8> nr21;
  logic<8> nr22;
  logic<8> nr23;
  logic<8> nr24;

  logic<8> nr30;
  logic<8> nr31;
  logic<8> nr32;
  logic<8> nr33;
  logic<8> nr34;

  logic<8> nr40;
  logic<8> nr41;
  logic<8> nr42;
  logic<8> nr43;
  logic<8> nr44;

  logic<8> nr50;
  logic<8> nr51;
  logic<8> nr52;

  logic<16> spu_clock;

  logic<1> s1_running;
  logic<1> s2_running;
  logic<1> s3_running;
  logic<1> s4_running;

  logic<6> s1_len_timer;
  logic<6> s2_len_timer;
  logic<8> s3_len_timer;
  logic<6> s4_len_timer;

  logic<3> s1_env_timer;
  logic<3> s2_env_timer;
  logic<3> s4_env_timer;

  logic<4> s1_env_vol;
  logic<4> s2_env_vol;
  logic<4> s4_env_vol;

  logic<3>  s1_sweep_timer;
  logic<11> s1_sweep_freq;

  logic<11> s1_freq_timer;
  logic<11> s2_freq_timer;
  logic<11> s3_freq_timer;
  logic<3>  s4_freq_timer;

  logic<3>  s1_phase;
  logic<3>  s2_phase;
  logic<5>  s3_phase;

  logic<8>  s3_wave[16];
  logic<16> s4_lfsr;
};
