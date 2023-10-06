#pragma once
#include "metron/metron_tools.h"

//------------------------------------------------------------------------------

class MetroBoySPU2 {
public:

  //----------------------------------------

  void tock_out() {
    logic<9> l;
    logic<9> r;

    l = 0;
    r = 0;

    if (s1_running && s1_env_vol) {
      logic<1> s1_out = 0;
      switch(s1_duty) {
        case 0: s1_out = s1_phase < 1; break;
        case 1: s1_out = s1_phase < 2; break;
        case 2: s1_out = s1_phase < 4; break;
        case 3: s1_out = s1_phase < 6; break;
      }
      if (mix_l1 && s1_out) l = l + s1_env_vol;
      if (mix_r1 && s1_out) r = r + s1_env_vol;
    }

    if (s2_running && s2_env_vol) {
      logic<1> s2_out = 0;
      switch(s2_duty) {
        case 0: s2_out = s2_phase < 1; break;
        case 1: s2_out = s2_phase < 2; break;
        case 2: s2_out = s2_phase < 4; break;
        case 3: s2_out = s2_phase < 6; break;
      }
      if (mix_l2 && s2_out) l = l + s2_env_vol;
      if (mix_r2 && s2_out) r = r + s2_env_vol;
    }

    if (s3_running && s3_power) {
      logic<8> s3_sample = s3_wave[s3_phase >> 1];
      logic<4> s3_out = (s3_phase & 1) ? b4(s3_sample, 0) : b4(s3_sample, 4);
      s3_out = s3_out >> s3_volume_shift;
      if (mix_l3) l = l + s3_out;
      if (mix_r3) r = r + s3_out;
    }

    if (s4_running && s4_env_vol) {
      logic<1> s4_out = b1(s4_lfsr, 15);
      if (mix_l4 && s4_out) l = l + s4_env_vol;
      if (mix_r4 && s4_out) r = r + s4_env_vol;
    }

    l = l * volume_l;
    r = r * volume_r;

    out_l = l;
    out_r = r;
  }

  //----------------------------------------

  void tick(logic<1> reset, logic<16> addr, logic<8> data_in, logic<1> read, logic<1> write) {
    if (reset) {
      spu_clock_old = 0;
      data_out = 0;

      s1_sweep_shift = 0;
      s1_sweep_dir = 0;
      s1_sweep_timer_init = 0;
      s1_len_timer_init = 0;
      s1_duty = 0;
      s1_env_timer_init = 0;
      s1_env_add = 0;
      s1_env_vol_init = 0;
      s1_freq_timer_init = 0;
      s1_len_en = 0;
      s1_trig = 0;
      s1_running = 0;
      s1_sweep_timer = 0;
      s1_sweep_freq = 0;
      s1_len_timer = 0;
      s1_env_vol = 0;
      s1_env_timer = 0;
      s1_freq_timer = 0;
      s1_phase = 0;

      s2_len_timer_init = 0;
      s2_duty = 0;
      s2_env_timer_init = 0;
      s2_env_add = 0;
      s2_env_vol_init = 0;
      s2_freq_timer_init = 0;
      s2_len_en = 0;
      s2_trig = 0;

      s2_len_timer = 0;
      s2_running = 0;
      s2_env_timer = 0;
      s2_env_vol = 0;
      s2_freq_timer = 0;
      s2_phase = 0;

      s3_power = 0;
      s3_len_timer_init = 0;
      s3_volume_shift = 0;
      s3_freq_timer_init = 0;
      s3_len_en = 0;
      s3_trig = 0;

      s3_running = 0;
      s3_len_timer = 0;
      s3_freq_timer = 0;
      s3_phase = 0;

      for (int i = 0; i < 16; i++) {
        s3_wave[i] = 0;
      }

      s4_len_timer_init = 0;
      s4_env_timer_init = 0;
      s4_env_add = 0;
      s4_env_vol_init = 0;
      s4_freq_timer_init = 0;
      s4_mode = 0;
      s4_shift = 0;
      s4_len_en = 0;
      s4_trig = 0;

      s4_running = 0;
      s4_len_timer = 0;
      s4_env_timer = 0;
      s4_env_vol = 0;
      s4_freq_timer = 0;
      s4_lfsr = 0;

      volume_l = 0;
      volume_r = 0;

      mix_r1 = 0;
      mix_r2 = 0;
      mix_r3 = 0;
      mix_r4 = 0;
      mix_l1 = 0;
      mix_l2 = 0;
      mix_l3 = 0;
      mix_l4 = 0;

      spu_power = 0;

    }
    else {
      logic<16> spu_clock_new = spu_clock_old + 1;
      logic<16> spu_tick = (~spu_clock_old) & (spu_clock_new);

      logic<1> sweep_tick  = b1(spu_tick, 12);
      logic<1> length_tick = b1(spu_tick, 11);
      logic<1> env_tick    = b1(spu_tick, 13);

      //----------------------------------------
      // Reg read

      if (read) {
        switch (addr) {
          case 0xFF10: data_out = cat(b1(1), s1_sweep_timer_init, s1_sweep_dir, s1_sweep_shift); break;
          case 0xFF11: data_out = cat(s1_duty, s1_len_timer_init); break;
          case 0xFF12: data_out = cat(s1_env_vol_init, s1_env_add, s1_env_timer_init); break;
          case 0xFF13: data_out = b8(s1_freq_timer_init, 0); break;
          case 0xFF14: data_out = cat(s1_trig, s1_len_en, b3(0b111), b3(s1_freq_timer_init, 8)); break;

            //----------

          case 0xFF16: data_out = cat(s2_duty, s2_len_timer_init); break;
          case 0xFF17: data_out = cat(s2_env_vol_init, s2_env_add, s2_env_timer_init); break;
          case 0xFF18: data_out = b8(s2_freq_timer_init, 0); break;
          case 0xFF19: data_out = cat(s2_trig, s2_len_en, b3(0b111), b3(s2_freq_timer_init, 8)); break;

            //----------

          case 0xFF1A: data_out = cat(s3_power, b7(0b1111111)); break;
          case 0xFF1B: data_out = s3_len_timer_init; break;

          // metron didn't like the block without {}
          case 0xFF1C: {
            switch (s3_volume_shift) {
              case 0: data_out = 0b01000000; break;
              case 1: data_out = 0b10000000; break;
              case 2: data_out = 0b11000000; break;
              case 4: data_out = 0b00000000; break;
            }
            break;
          }
          case 0xFF1D: data_out = b8(s3_freq_timer_init, 0); break;
          case 0xFF1E: data_out = cat(s3_trig, s3_len_en, b3(0b111), b3(s3_freq_timer_init, 8)); break;

            //----------

          case 0xFF20: data_out = cat(b2(0b11), s4_len_timer_init); break;
          case 0xFF21: data_out = cat(s4_env_vol_init, s4_env_add, s4_env_timer_init); break;
          case 0xFF22: data_out = cat(s4_shift, s4_mode, s4_freq_timer_init); break;
          case 0xFF23: data_out = cat(s4_trig, s4_len_en, b6(0b111111)); break;

            //----------

          case 0xFF24: data_out = cat(b1(0), volume_l, b1(0), volume_r); break;
          case 0xFF25: data_out = cat(mix_l4, mix_l3, mix_l2, mix_l1, mix_r4, mix_r3, mix_r2, mix_r1); break;
          case 0xFF26: data_out = cat(spu_power, b7(0)); break;

          // "default: break didn't work?"
          default: { break; }
        }
      }

      //----------
      // s1 clock

      if (s1_freq_timer == 0b11111111111) {
        s1_phase = s1_phase + 1;
        s1_freq_timer = s1_sweep_timer_init ? s1_sweep_freq : s1_freq_timer_init;
      }
      else {
        s1_freq_timer = s1_freq_timer + 1;
      }

      //----------
      // s1 length

      if (length_tick && s1_running && s1_len_en) {
        if (s1_len_timer == 0b111111) {
          s1_len_timer = 0;
          s1_running = 0;
        }
        else {
          s1_len_timer = s1_len_timer + 1;
        }
      }

      //----------
      // s1 sweep

      if (sweep_tick && s1_sweep_timer_init && s1_sweep_shift) {
        if (s1_sweep_timer) {
          s1_sweep_timer = s1_sweep_timer - 1;
        }
        else {
          logic<11> delta = s1_sweep_freq >> s1_sweep_shift;
          logic<12> next_freq = s1_sweep_freq + (s1_sweep_dir ? -delta : +delta);
          if (next_freq > 2047) s1_running = 0;
          s1_sweep_timer = s1_sweep_timer_init;
          s1_sweep_freq = next_freq;
        }
      }

      //----------
      // s1 env

      if (env_tick && s1_env_timer_init) {
        if (s1_env_timer) {
          s1_env_timer = s1_env_timer - 1;
        }
        else {
          s1_env_timer = s1_env_timer_init;
          if (s1_env_add) { if (s1_env_vol < 15) s1_env_vol = s1_env_vol + 1; }
          else            { if (s1_env_vol >  0) s1_env_vol = s1_env_vol - 1; }
        }
      }

      //----------
      // s2 clock

      if (s2_freq_timer == 0x7FF) {
        s2_phase = s2_phase + 1;
        s2_freq_timer = s2_freq_timer_init;
      }
      else {
        s2_freq_timer = s2_freq_timer + 1;
      }

      //----------
      // s2 length

      if (length_tick && s2_running && s2_len_en) {
        if (s2_len_timer == 0x3F) {
          s2_len_timer = 0;
          s2_running = 0;
        }
        else {
          s2_len_timer = s2_len_timer + 1;
        }
      }

      //----------
      // s2 env

      if (env_tick && s2_env_timer_init) {
        if (s2_env_timer) {
          s2_env_timer = s2_env_timer - 1;
        }
        else {
          s2_env_timer = s2_env_timer_init;
          if (s2_env_add) { if (s2_env_vol < 15) s2_env_vol = s2_env_vol + 1; }
          else            { if (s2_env_vol >  0) s2_env_vol = s2_env_vol - 1; }
        }
      }

      //----------
      // s3 clock - we run this twice because s3's timer ticks at 2 mhz

      {
        logic<5> next_phase = s3_phase;
        logic<11> next_timer = s3_freq_timer;

        if (next_timer == 0x7FF) {
          next_phase = next_phase + 1;
          next_timer = s3_freq_timer_init;
        }
        else {
          next_timer = next_timer + 1;
        }

        if (next_timer == 0x7FF) {
          next_phase = next_phase + 1;
          next_timer = s3_freq_timer_init;
        }
        else {
          next_timer = next_timer + 1;
        }

        s3_phase = next_phase;
        s3_freq_timer = next_timer;
      }

      //----------
      // s3 length

      if (length_tick && s3_running && s3_len_en) {
        if (s3_len_timer == 0xFF) {
          s3_len_timer = 0;
          s3_running = 0;
        }
        else {
          s3_len_timer = s3_len_timer + 1;
        }
      }

      //----------
      // s4 lfsr

      logic<1> lfsr_clock_old = spu_clock_old[s4_shift + 1];
      logic<1> lfsr_clock_new = spu_clock_new[s4_shift + 1];

      /*
      logic<1> lfsr_clock_old = 0;
      logic<1> lfsr_clock_new = 0;

      switch(s4_shift) {
        case 0 : { lfsr_clock_old = spu_clock_old[1 ]; lfsr_clock_new = spu_clock_new[1 ]; break; }
        case 1 : { lfsr_clock_old = spu_clock_old[2 ]; lfsr_clock_new = spu_clock_new[2 ]; break; }
        case 2 : { lfsr_clock_old = spu_clock_old[3 ]; lfsr_clock_new = spu_clock_new[3 ]; break; }
        case 3 : { lfsr_clock_old = spu_clock_old[4 ]; lfsr_clock_new = spu_clock_new[4 ]; break; }
        case 4 : { lfsr_clock_old = spu_clock_old[5 ]; lfsr_clock_new = spu_clock_new[5 ]; break; }
        case 5 : { lfsr_clock_old = spu_clock_old[6 ]; lfsr_clock_new = spu_clock_new[6 ]; break; }
        case 6 : { lfsr_clock_old = spu_clock_old[7 ]; lfsr_clock_new = spu_clock_new[7 ]; break; }
        case 7 : { lfsr_clock_old = spu_clock_old[8 ]; lfsr_clock_new = spu_clock_new[8 ]; break; }
        case 8 : { lfsr_clock_old = spu_clock_old[9 ]; lfsr_clock_new = spu_clock_new[9 ]; break; }
        case 9 : { lfsr_clock_old = spu_clock_old[10]; lfsr_clock_new = spu_clock_new[10]; break; }
        case 10: { lfsr_clock_old = spu_clock_old[11]; lfsr_clock_new = spu_clock_new[11]; break; }
        case 11: { lfsr_clock_old = spu_clock_old[12]; lfsr_clock_new = spu_clock_new[12]; break; }
        case 12: { lfsr_clock_old = spu_clock_old[13]; lfsr_clock_new = spu_clock_new[13]; break; }
        case 13: { lfsr_clock_old = spu_clock_old[14]; lfsr_clock_new = spu_clock_new[14]; break; }
        case 14: { lfsr_clock_old = spu_clock_old[15]; lfsr_clock_new = spu_clock_new[15]; break; }
        case 15: { lfsr_clock_old = 0;                 lfsr_clock_new = 0;                 break; }
      }
      */

      if ((lfsr_clock_old == 0) && (lfsr_clock_new == 1)) {
        if (s4_freq_timer) {
          s4_freq_timer = s4_freq_timer - 1;
        }
        else {
          logic<1> new_bit = s4_lfsr[15] ^ s4_lfsr[14] ^ 1;
          s4_lfsr = cat(
            b6(s4_lfsr, 9),
            s4_mode ? new_bit : s4_lfsr[8],
            b8(s4_lfsr, 0),
            new_bit);
          s4_freq_timer = s4_freq_timer_init;
        }
      }

      //----------
      // s4 length

      if (length_tick && s4_running && s4_len_en) {
        if (s4_len_timer == 0x3F) {
          s4_len_timer = 0;
          s4_running = 0;
        }
        else {
          s4_len_timer = s4_len_timer + 1;
        }
      }

      //----------
      // s4 env

      if (env_tick && s4_env_timer_init) {
        if (s4_env_timer) {
          s4_env_timer = s4_env_timer - 1;
        }
        else {
          s4_env_timer = s4_env_timer_init;
          if (s4_env_add) { if (s4_env_vol < 15) s4_env_vol = s4_env_vol + 1; }
          else            { if (s4_env_vol >  0) s4_env_vol = s4_env_vol - 1; }
        }
      }

      //----------
      // Triggers

      if (s1_trig && (s1_env_vol_init || s1_env_add)) {
        s1_running     = 1;
        s1_len_timer   = s1_len_timer_init;
        s1_sweep_timer = s1_sweep_timer_init;
        s1_sweep_freq  = s1_freq_timer_init;
        s1_env_vol     = s1_env_vol_init;
        s1_env_timer   = s1_env_timer_init;
        s1_freq_timer  = s1_freq_timer_init;
        s1_phase       = 0;
        s1_trig        = 0;
      }

      if (s2_trig && (s2_env_vol_init || s2_env_add)) {
        s2_running    = 1;
        s2_len_timer  = s2_len_timer_init;
        s2_env_vol    = s2_env_vol_init;
        s2_env_timer  = s2_env_timer_init;
        s2_freq_timer = s2_freq_timer_init;
        s2_phase      = 0;
        s2_trig       = 0;
      }

      if (s3_trig) {
        s3_running    = 1;
        s3_len_timer  = s3_len_timer_init;
        s3_freq_timer = s3_freq_timer_init;
        s3_phase      = 0;
        s3_trig       = 0;
      }

      if (s4_trig && (s4_env_vol_init || s4_env_add)) {
        s4_running    = 1;
        s4_len_timer  = s4_len_timer_init;
        s4_env_vol    = s4_env_vol_init;
        s4_env_timer  = s4_env_timer_init;
        s4_freq_timer = s4_freq_timer_init;
        s4_lfsr       = 0;
        s4_trig       = 0;
      }

      //----------
      // Register writes

      if (write && addr >= 0xFF10 && addr <= 0xFF26) {
        switch (addr) {
          case 0xFF10: {
            s1_sweep_shift      = b3(data_in, 0);
            s1_sweep_dir        = b1(data_in, 3);
            s1_sweep_timer_init = b3(data_in, 4);
            break;
          }
          case 0xFF11: {
            s1_len_timer_init = b6(data_in, 0);
            s1_duty           = b2(data_in, 6);
            break;
          }
          case 0xFF12: {
            s1_env_timer_init = b3(data_in, 0);
            s1_env_add        = b1(data_in, 3);
            s1_env_vol_init   = b4(data_in, 4);
            break;
          }
          case 0xFF13: {
            s1_freq_timer_init = cat(b3(s1_freq_timer_init, 8), data_in);
            break;
          }
          case 0xFF14: {
            s1_freq_timer_init = cat(b3(data_in, 0), b8(s1_freq_timer_init, 0));
            s1_len_en          = b1(data_in, 6);
            s1_trig            = b1(data_in, 7);
            break;
          }

          //----------

          case 0xFF16: {
            s2_len_timer_init = b6(data_in, 0);
            s2_duty           = b2(data_in, 6);
            break;
          }
          case 0xFF17: {
            s2_env_timer_init = b3(data_in, 0);
            s2_env_add        = b1(data_in, 3);
            s2_env_vol_init   = b4(data_in, 4);
            break;
          }
          case 0xFF18: {
            s2_freq_timer_init = cat(b3(s2_freq_timer_init, 8), data_in);
            break;
          }
          case 0xFF19: {
            s2_freq_timer_init = cat(b3(data_in, 0), b8(s2_freq_timer_init, 0));
            s2_len_en          = b1(data_in, 6);
            s2_trig            = b1(data_in, 7);
            break;
          }

          //----------

          case 0xFF1A: {
            s3_power = b1(data_in, 7);
            break;
          }
          case 0xFF1B: {
            s3_len_timer_init = b8(data_in, 0);
            break;
          }
          case 0xFF1C: {
            switch (b2(data_in, 5)) {
              case 0: s3_volume_shift = 4; break;
              case 1: s3_volume_shift = 0; break;
              case 2: s3_volume_shift = 1; break;
              case 3: s3_volume_shift = 2; break;
            }
            break;
          }
          case 0xFF1D: {
            s3_freq_timer_init = cat(b3(s3_freq_timer_init, 8), data_in);
            break;
          }
          case 0xFF1E: {
            s3_freq_timer_init = cat(b3(data_in, 0), b8(s3_freq_timer_init, 0));
            s3_len_en          = b1(data_in, 6);
            s3_trig            = b1(data_in, 7);
            break;
          }

          //----------

          case 0xFF20: {
            s4_len_timer_init = b6(data_in, 0);
            break;
          }
          case 0xFF21: {
            s4_env_timer_init = b3(data_in, 0);
            s4_env_add        = b1(data_in, 3);
            s4_env_vol_init   = b4(data_in, 4);
            break;
          }
          case 0xFF22: {
            s4_freq_timer_init = b3(data_in, 0);
            s4_mode            = b1(data_in, 3);
            s4_shift           = b4(data_in, 4);
            break;
          }
          case 0xFF23: {
            s4_len_en = b1(data_in, 6);
            s4_trig   = b1(data_in, 7);
            break;
          }

          //----------

          case 0xFF24: {
            volume_r = b3(data_in, 0) + 1;
            volume_l = b3(data_in, 4) + 1;
            break;
          }
          case 0xFF25: {
            mix_r1 = b1(data_in, 0);
            mix_r2 = b1(data_in, 1);
            mix_r3 = b1(data_in, 2);
            mix_r4 = b1(data_in, 3);
            mix_l1 = b1(data_in, 4);
            mix_l2 = b1(data_in, 5);
            mix_l3 = b1(data_in, 6);
            mix_l4 = b1(data_in, 7);
            break;
          }
          case 0xFF26: {
            spu_power = b1(data_in, 7);
            break;
          }
          default: {
            break;
          }
        }
      }

      //----------
      // Wavetable writes

      if (write && addr >= 0xFF30 && addr <= 0xFF3F) {
        s3_wave[addr & 0xF] = data_in;
      }

      spu_clock_old = spu_clock_new;
    }
  }

  //----------------------------------------

  void dump(Dumper& d) const {
    d("\002--------------SPU--------------\001\n");

    const char* bar = "===============";

    logic<4> s3_env_vol = 0;
    switch (s3_volume_shift) {
    case 0: s3_env_vol = 15; break;
    case 1: s3_env_vol = 7; break;
    case 2: s3_env_vol = 3; break;
    case 4: s3_env_vol = 0; break;
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

  logic<9>  out_r; // signals
  logic<9>  out_l; // signals

  //----------------------------------------

private:

  logic<16> spu_clock_old;
  logic<8>  data_out;

  // Channel 1

  /*NR10*/ logic<3>  s1_sweep_shift;
  /*NR10*/ logic<1>  s1_sweep_dir;
  /*NR10*/ logic<3>  s1_sweep_timer_init;
  /*NR11*/ logic<6>  s1_len_timer_init;
  /*NR11*/ logic<2>  s1_duty;
  /*NR12*/ logic<3>  s1_env_timer_init;
  /*NR12*/ logic<1>  s1_env_add;
  /*NR12*/ logic<4>  s1_env_vol_init;
  /*NR13*/ logic<11> s1_freq_timer_init;
  /*NR14*/ logic<1>  s1_len_en;
  /*NR14*/ logic<1>  s1_trig;

  logic<1>  s1_running;
  logic<3>  s1_sweep_timer;
  logic<11> s1_sweep_freq;
  logic<6>  s1_len_timer;
  logic<4>  s1_env_vol;
  logic<3>  s1_env_timer;
  logic<11> s1_freq_timer;
  logic<3>  s1_phase;

  // Channel 2

  /*NR21*/ logic<6>  s2_len_timer_init;
  /*NR21*/ logic<2>  s2_duty;
  /*NR22*/ logic<3>  s2_env_timer_init;
  /*NR22*/ logic<1>  s2_env_add;
  /*NR22*/ logic<4>  s2_env_vol_init;
  /*NR23*/ logic<11> s2_freq_timer_init;
  /*NR24*/ logic<1>  s2_len_en;
  /*NR24*/ logic<1>  s2_trig;

  logic<6>  s2_len_timer;
  logic<1>  s2_running;
  logic<3>  s2_env_timer;
  logic<4>  s2_env_vol;
  logic<11> s2_freq_timer;
  logic<3>  s2_phase;

  // Channel 3

  /*NR30*/ logic<1>  s3_power;
  /*NR31*/ logic<8>  s3_len_timer_init;
  /*NR32*/ logic<3>  s3_volume_shift;
  /*NR33*/ logic<11> s3_freq_timer_init;
  /*NR34*/ logic<1>  s3_len_en;
  /*NR34*/ logic<1>  s3_trig;

  logic<1>  s3_running;
  logic<8>  s3_len_timer;
  logic<11> s3_freq_timer;
  logic<5>  s3_phase;
  logic<8>  s3_wave[16];

  // Channel 4

  /*NR41*/ logic<6>  s4_len_timer_init;
  /*NR42*/ logic<3>  s4_env_timer_init;
  /*NR42*/ logic<1>  s4_env_add;
  /*NR42*/ logic<4>  s4_env_vol_init;
  /*NR43*/ logic<3>  s4_freq_timer_init;
  /*NR43*/ logic<1>  s4_mode;
  /*NR43*/ logic<4>  s4_shift;
  /*NR44*/ logic<1>  s4_len_en;
  /*NR44*/ logic<1>  s4_trig;

  logic<1>  s4_running;
  logic<6>  s4_len_timer;
  logic<3>  s4_env_timer;
  logic<4>  s4_env_vol;
  logic<3>  s4_freq_timer;
  logic<16> s4_lfsr;

  // SPU Control Registers

  /*NR50*/ logic<4>  volume_l;
  /*NR50*/ logic<4>  volume_r;

  /*NR51*/ logic<1>  mix_r1;
  /*NR51*/ logic<1>  mix_r2;
  /*NR51*/ logic<1>  mix_r3;
  /*NR51*/ logic<1>  mix_r4;
  /*NR51*/ logic<1>  mix_l1;
  /*NR51*/ logic<1>  mix_l2;
  /*NR51*/ logic<1>  mix_l3;
  /*NR51*/ logic<1>  mix_l4;

  /*NR52*/ logic<1>  spu_power;

};
