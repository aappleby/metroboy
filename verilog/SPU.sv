`include "types.sv"

import types::*;

/* verilator lint_off WIDTH */

//-----------------------------------------------------------------------------

module sound_regs;
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

  task reset();
    nr10 <= 8'h80;
    nr11 <= 8'hBF;
    nr12 <= 8'hF3;
    nr13 <= 8'hFF;
    nr14 <= 8'hBF;
    nr21 <= 8'h3F;
    nr22 <= 8'h00;
    nr23 <= 8'hFF;
    nr24 <= 8'hBF;
    nr30 <= 8'h7F;
    nr31 <= 8'hFF;
    nr32 <= 8'h9F;
    nr33 <= 8'hFF;
    nr34 <= 8'hBF;
    nr40 <= 8'h80;
    nr41 <= 8'hFF;
    nr42 <= 8'h00;
    nr43 <= 8'h00;
    nr44 <= 8'hBF;
    nr50 <= 8'h77;
    nr51 <= 8'hF3;
    nr52 <= 8'hF1;
  endtask;

  task bus_write(input uint16_t addr, input uint8_t data);
    case(addr)
    16'hFF10: nr10 <= data | 8'b10000000;
    16'hFF11: nr11 <= data | 8'b00000000;
    16'hFF12: nr12 <= data | 8'b00000000;
    16'hFF13: nr13 <= data | 8'b00000000;
    16'hFF14: nr14 <= data | 8'b00111000;
    16'hFF16: nr21 <= data | 8'b00000000;
    16'hFF17: nr22 <= data | 8'b00000000;
    16'hFF18: nr23 <= data | 8'b00000000;
    16'hFF19: nr24 <= data | 8'b00111000;
    16'hFF1A: nr30 <= data | 8'b01111111;
    16'hFF1B: nr31 <= data | 8'b00000000;
    16'hFF1C: nr32 <= data | 8'b10011111;
    16'hFF1D: nr33 <= data | 8'b00000000;
    16'hFF1E: nr34 <= data | 8'b00111000;
    16'hFF20: nr41 <= data | 8'b11000000;
    16'hFF21: nr42 <= data | 8'b00000000;
    16'hFF22: nr43 <= data | 8'b00000000;
    16'hFF23: nr44 <= data | 8'b00111111;
    16'hFF24: nr50 <= data | 8'b00000000;
    16'hFF25: nr51 <= data | 8'b00000000;
    16'hFF26: nr52 <= data | 8'b01110000;
    endcase
  endtask;

  task bus_read(input uint16_t addr, output uint8_t out, output bool oe);
    oe <= 1;
    case(addr)
    16'hFF10: out <= regs.nr10 | 8'h80;
    16'hFF11: out <= regs.nr11 | 8'h3F;
    16'hFF12: out <= regs.nr12 | 8'h00;
    16'hFF13: out <= regs.nr13 | 8'hFF;
    16'hFF14: out <= regs.nr14 | 8'hBF;
    16'hFF15: out <= regs.nr20 | 8'hFF;
    16'hFF16: out <= regs.nr21 | 8'h3F;
    16'hFF17: out <= regs.nr22 | 8'h00;
    16'hFF18: out <= regs.nr23 | 8'hFF;
    16'hFF19: out <= regs.nr24 | 8'hBF;
    16'hFF1A: out <= regs.nr30 | 8'h7F;
    16'hFF1B: out <= regs.nr31 | 8'hFF;
    16'hFF1C: out <= regs.nr32 | 8'h9F;
    16'hFF1D: out <= regs.nr33 | 8'hFF;
    16'hFF1E: out <= regs.nr34 | 8'hBF;
    16'hFF1F: out <= regs.nr40 | 8'hFF;
    16'hFF20: out <= regs.nr41 | 8'hFF;
    16'hFF21: out <= regs.nr42 | 8'h00;
    16'hFF22: out <= regs.nr43 | 8'h00;
    16'hFF23: out <= regs.nr44 | 8'hBF;
    16'hFF24: out <= regs.nr50 | 8'h00;
    16'hFF25: out <= regs.nr51 | 8'h00;

    default: begin
      out <= 0;
      oe <= 0;
    end
    endcase
  endtask;

  function uint8_t s1_sweep_period(); return uint8_t'(regs.nr10[6:4]); endfunction;

  function bool sound_on(); return nr52[7]; endfunction;

endmodule;

//-----------------------------------------------------------------------------

module SPU(
  input bool rst,
  input bool clk,
  uint8_t tphase,
  input uint16_t addr,
  input uint8_t data,
  input bool read,
  input bool write,
  output uint8_t bus_out,
  output bool bus_oe,
  output sample_t out_r,
  output sample_t out_l
);

sound_regs regs;

uint8_t s3_wave[16];

uint16_t spu_clock;

bool s1_enable;
bool s2_enable;
bool s3_enable;
bool s4_enable;

uint8_t s1_duration;
uint8_t s2_duration;
uint16_t s3_duration;
uint8_t s4_duration;

uint8_t s1_sweep_clock;
uint16_t s1_sweep_freq;

uint8_t s1_env_volume;
uint8_t s2_env_volume;
uint8_t s4_env_volume;
uint8_t s1_env_clock;
uint8_t s2_env_clock;
uint8_t s4_env_clock;

uint16_t s1_phase_clock;
uint16_t s2_phase_clock;
uint16_t s3_phase_clock;
uint8_t s4_phase_clock;
uint8_t s1_phase;
uint8_t s2_phase;
uint8_t s3_phase;
uint16_t s4_lfsr;

uint8_t s1_out;
uint8_t s2_out;
uint8_t s3_out;
uint8_t s4_out;

//-----------------------------------------------------------------------------

task reset;
  integer i;

  bus_out <= 0;
  bus_oe <= 0;
  out_r <= 0;
  out_l <= 0;

  regs.reset();

  for (i = 0; i < 16; i++) s3_wave[i] <= 0;
  s4_lfsr <= 16'h7FFF;

  spu_clock <= 0;

  s1_enable <= false;
  s2_enable <= false;
  s3_enable <= false;
  s4_enable <= false;

  s1_sweep_clock <= 0;
  s1_sweep_freq <= 0;

  s1_duration <= 0;
  s2_duration <= 0;
  s3_duration <= 0;
  s4_duration <= 0;

  s1_env_clock <= 0;
  s2_env_clock <= 0;
  s4_env_clock <= 0;
  s1_env_volume <= 0;
  s2_env_volume <= 0;
  s4_env_volume <= 0;

  s1_phase_clock <= 2047;
  s2_phase_clock <= 2047;
  s3_phase_clock <= 2047;
  s4_phase_clock <= 15;
  s1_phase <= 0;
  s2_phase <= 0;
  s3_phase <= 0;
endtask;

//-----------------------------------------------------------------------------

task tock;
  logic sound_on = `bit_set(regs.nr52, 7);
  uint16_t spu_clock_ = spu_clock + 14'b1;
  uint16_t clock_flip = (~spu_clock) & spu_clock_;
  logic sweep_tick, length_tick, env_tick;
  uint8_t s1_out_;
  uint8_t s2_out_;
  uint8_t s3_out_;
  uint8_t s4_out_;
  uint16_t out_r_ = 0;
  uint16_t out_l_ = 0;

  //----------

  if (read) bus_read(addr);
  if (write && (sound_on || addr == 16'hFF26)) bus_write(addr, data);
  if (tphase != 2) return;

  if (!regs.sound_on()) begin
    s1_out <= 0;
    s2_out <= 0;
    s3_out <= 0;
    s4_out <= 0;

    out_r <= 0;
    out_l <= 0;
    return;
  end

  //----------
  // sweep

  sweep_tick =  (spu_clock_ & 14'b01111111111111) == 14'b01000000000000;
  length_tick = (spu_clock_ & 14'b00111111111111) == 14'b00000000000000;
  env_tick =    (spu_clock_ & 14'b11111111111111) == 14'b11100000000000;

  if (sweep_tick) begin
    uint8_t s1_sweep_period = regs.nr10[6:4];
    uint8_t s1_sweep_shift = regs.nr10[2:0];
    bool s1_sweep_dir = regs.nr10[3];
    uint16_t s1_freq = {regs.nr14[2:0], regs.nr13};

    if ((s1_sweep_period != 0) && (s1_sweep_shift != 0)) begin
      if (s1_sweep_clock != 0) begin
        s1_sweep_clock <= s1_sweep_clock - 1;
      end
      else begin
        uint16_t delta = s1_sweep_freq >> s1_sweep_shift;
        uint16_t new_freq = s1_sweep_freq + (s1_sweep_dir ? -delta : +delta);
        s1_sweep_clock <= s1_sweep_period;
        if (new_freq > 2047) begin
          s1_enable <= false;
        end
        else begin
          s1_sweep_freq <= new_freq;
        end
      end
    end
  end

  //----------
  // length

  if (length_tick) begin
    bool s1_length_enable = regs.nr14[6];
    bool s2_length_enable = regs.nr24[6];
    bool s3_length_enable = regs.nr34[6];
    bool s4_length_enable = regs.nr44[6];
    uint8_t s1_duration_ = s1_duration;
    uint8_t s2_duration_ = s2_duration;
    uint16_t s3_duration_ = s3_duration;
    uint8_t s4_duration_ = s4_duration;

    if (s1_length_enable && (s1_duration_ != 0)) s1_duration_ = s1_duration_ - 1;
    if (s2_length_enable && (s2_duration_ != 0)) s2_duration_ = s2_duration_ - 1;
    if (s3_length_enable && (s3_duration_ != 0)) s3_duration_ = s3_duration_ - 1;
    if (s4_length_enable && (s4_duration_ != 0)) s4_duration_ = s4_duration_ - 1;

    if (s1_length_enable && s1_duration_ == 0) s1_enable <= false;
    if (s2_length_enable && s2_duration_ == 0) s2_enable <= false;
    if (s3_length_enable && s3_duration_ == 0) s3_enable <= false;
    if (s4_length_enable && s4_duration_ == 0) s4_enable <= false;

    s1_duration <= s1_duration_;
    s2_duration <= s2_duration_;
    s3_duration <= s3_duration_;
    s4_duration <= s4_duration_;
  end

  //----------
  // env

  if (env_tick) begin
    bool s1_env_dir = regs.nr12[3];
    bool s2_env_dir = regs.nr22[3];
    bool s4_env_dir = regs.nr42[3];

    uint8_t s1_env_period = regs.nr12[2:0];
    uint8_t s2_env_period = regs.nr22[2:0];
    uint8_t s4_env_period = regs.nr42[2:0];

    if (s1_env_period != 0) begin
      if (s1_env_clock != 0) begin
        s1_env_clock <= s1_env_clock - 1;
      end
      else begin
        s1_env_clock <= s1_env_period;
        if (s1_env_volume < 15) s1_env_volume <= s1_env_volume + 1;
      end
    end

    if (s2_env_period != 0) begin
      if (s2_env_clock != 0) begin
        s2_env_clock <= s2_env_clock - 1;
      end
      else  begin
        s2_env_clock <= s2_env_period;
        if (s2_env_volume < 15) s2_env_volume <= s2_env_volume + 1;
      end
    end

    if (s4_env_period != 0) begin
      if (s4_env_clock != 0) begin
        s4_env_clock <= s4_env_clock - 1;
      end
      else begin
        s4_env_clock <= s4_env_period;
        if (s4_env_volume < 15) s4_env_volume <= s4_env_volume + 1;
      end
    end
  end

  //----------
  // phase

  if (s1_phase_clock == 0) begin
    uint8_t s1_sweep_period = regs.nr10[6:4];
    uint16_t s1_freq = {regs.nr14[2:0], regs.nr13};
      
    s1_phase_clock <= 2047 ^ (s1_sweep_period != 0 ? s1_sweep_freq : s1_freq);
    s1_phase <= (s1_phase + 1) & 7;
  end
  else begin
    s1_phase_clock <= s1_phase_clock - 1;
  end

  if (s2_phase_clock == 0) begin
    uint16_t s2_freq = {regs.nr24[2:0], regs.nr23};
      
    s2_phase_clock <= 2047 ^ s2_freq;
    s2_phase <= (s2_phase + 1) & 7;
  end
  else begin
    s2_phase_clock <= s2_phase_clock - 1;
  end

  // we run this twice because this is ticking at 1 mhz
  begin
    uint16_t s3_freq = {regs.nr34[2:0], regs.nr33};
    uint16_t s3_phase_clock_ = s3_phase_clock;
    uint8_t s3_phase_ = s3_phase;
    
    if (s3_phase_clock_ == 0) begin
      s3_phase_clock_ = 2047 ^ s3_freq;
      s3_phase_ = (s3_phase + 1) & 31;
    end
    else begin
      s3_phase_clock_ = s3_phase_clock_ - 1;
    end

    if (s3_phase_clock_ == 0) begin
      s3_phase_clock_ = 2047 ^ s3_freq;
      s3_phase_ = (s3_phase + 1) & 31;
    end
    else begin
      s3_phase_clock_ = s3_phase_clock_ - 1;
    end

    s3_phase_clock <= s3_phase_clock_;
    s3_phase <= s3_phase_;
  end

  if (s4_phase_clock == 0) begin
    bool s4_lfsr_mode = regs.nr43[3];
    uint8_t s4_phase_period = regs.nr43[2:0] != 0 ? {regs.nr43[2:0], 1'b0} : 1;
    bool lfsr_bit = s4_lfsr[0] ^ s4_lfsr[1];

    s4_phase_clock <= s4_phase_period;
    if (s4_lfsr_mode) begin
      s4_lfsr <= {10'b0, lfsr_bit, s4_lfsr[4:1]};
    end else begin
      s4_lfsr <= {lfsr_bit, s4_lfsr[14:1]};
    end
  end
  else begin
    uint8_t s4_clock_shift = regs.nr43[7:4];
    s4_phase_clock <= s4_phase_clock - {3'b0, clock_flip[s4_clock_shift]};
  end

  //----------
  // output

  s1_out_ = 0;
  s2_out_ = 0;
  s3_out_ = 0;
  s4_out_ = 0;

  if (s1_enable) begin
    uint8_t s1_sample;
    uint8_t s1_volume = regs.nr12[3] ? s1_env_volume : 15 ^ s1_env_volume;
    uint8_t s1_duty = regs.nr11[7:6] == 0 ? 1 : {regs.nr11[7:6], 1'b0};
    
    s1_sample = (s1_phase < s1_duty) ? s1_volume : 0;
    s1_out_ = s1_sample;
  end

  if (s2_enable) begin
    uint8_t s2_sample;
    uint8_t s2_volume = regs.nr22[3] ? s2_env_volume : 15 ^ s2_env_volume;
    uint8_t s2_duty = regs.nr21[7:6] == 0 ? 1 : {regs.nr11[7:6], 1'b0};

    s2_sample = (s2_phase < s2_duty) ? s2_volume : 0;
    s2_out_ = s2_sample;
  end

  if (s3_enable && regs.nr30[7]) begin
    uint8_t s3_byte = s3_wave[s3_phase[4:1]];
    uint8_t s3_sample = s3_phase[0] ? s3_byte[3:0] : s3_byte[7:4];
    uint8_t s3_volume_shift = 0;

    case ((regs.nr32 & 8'b01100000) >> 5) 
    0: s3_volume_shift = 4;
    1: s3_volume_shift = 0;
    2: s3_volume_shift = 1;
    3: s3_volume_shift = 2;
    endcase

    s3_out_ = s3_sample >> s3_volume_shift;
  end

  if (s4_enable) begin
    uint8_t s4_volume = regs.nr42[3] ? s4_env_volume : 15 ^ s4_env_volume;
    uint8_t s4_sample = s4_lfsr[0] ? s4_volume : 0;
    s4_out_ = s4_sample;
  end

  //----------
  // mixer & master volume

  begin
    uint8_t volume_r = uint8_t'(regs.nr50[2:0]) + 1;
    uint8_t volume_l = uint8_t'(regs.nr50[6:4]) + 1;

    if (`bit_set(regs.nr51, 0)) out_r_ += {7'b0, s1_out_};
    if (`bit_set(regs.nr51, 1)) out_r_ += {7'b0, s2_out_};
    if (`bit_set(regs.nr51, 2)) out_r_ += {7'b0, s3_out_};
    if (`bit_set(regs.nr51, 3)) out_r_ += {7'b0, s4_out_};
    if (`bit_set(regs.nr51, 4)) out_l_ += {7'b0, s1_out_};
    if (`bit_set(regs.nr51, 5)) out_l_ += {7'b0, s2_out_};
    if (`bit_set(regs.nr51, 6)) out_l_ += {7'b0, s3_out_};
    if (`bit_set(regs.nr51, 7)) out_l_ += {7'b0, s4_out_};

    out_r_ *= volume_r;
    out_l_ *= volume_l;
  end

  //----------
  // commit

  s1_out <= s1_out_;
  s2_out <= s2_out_;
  s3_out <= s3_out_;
  s4_out <= s4_out_;

  out_r <= out_r_;
  out_l <= out_l_;
  spu_clock <= spu_clock_;
endtask;

//-----------------------------------------------------------------------------

task bus_read(input uint16_t addr);
  bus_oe <= 1;
  case(addr)
  16'hFF10: bus_out <= regs.nr10 | 8'h80;
  16'hFF11: bus_out <= regs.nr11 | 8'h3F;
  16'hFF12: bus_out <= regs.nr12 | 8'h00;
  16'hFF13: bus_out <= regs.nr13 | 8'hFF;
  16'hFF14: bus_out <= regs.nr14 | 8'hBF;
  
  16'hFF15: bus_out <= regs.nr20 | 8'hFF;
  16'hFF16: bus_out <= regs.nr21 | 8'h3F;
  16'hFF17: bus_out <= regs.nr22 | 8'h00;
  16'hFF18: bus_out <= regs.nr23 | 8'hFF;
  16'hFF19: bus_out <= regs.nr24 | 8'hBF;
  
  16'hFF1A: bus_out <= regs.nr30 | 8'h7F;
  16'hFF1B: bus_out <= regs.nr31 | 8'hFF;
  16'hFF1C: bus_out <= regs.nr32 | 8'h9F;
  16'hFF1D: bus_out <= regs.nr33 | 8'hFF;
  16'hFF1E: bus_out <= regs.nr34 | 8'hBF;
  
  16'hFF1F: bus_out <= regs.nr40 | 8'hFF;
  16'hFF20: bus_out <= regs.nr41 | 8'hFF;
  16'hFF21: bus_out <= regs.nr42 | 8'h00;
  16'hFF22: bus_out <= regs.nr43 | 8'h00;
  16'hFF23: bus_out <= regs.nr44 | 8'hBF;
  
  16'hFF24: bus_out <= regs.nr50 | 8'h00;
  16'hFF25: bus_out <= regs.nr51 | 8'h00;

  16'hFF26: begin
    uint8_t bus_out_ = (regs.nr52 & 8'h80) | 8'h70;
    if (s1_enable) bus_out_ |= 8'b00000001;
    if (s2_enable) bus_out_ |= 8'b00000010;
    if (s3_enable) bus_out_ |= 8'b00000100;
    if (s4_enable) bus_out_ |= 8'b00001000;
    bus_out <= bus_out_;
  end

  default: begin
    bus_out <= 0;
    bus_oe <= 0;
  end
  endcase

  //----------
  // wavetable

  if (addr >= 16'hFF30 && addr <= 16'hFF3F) begin
    bus_oe <= 1;
    bus_out <= s3_wave[addr[3:0]];
  end
endtask

//-----------------------------------------------------------------------------

task bus_write(input uint16_t addr, input uint8_t data);
  //----------
  // glitches n stuff

  if (addr == 16'hFF12) begin
    if (regs.nr12[3] && s1_enable) s1_env_volume <= (s1_env_volume + 1) & 4'hF;
    if (data[7:3] == 0) s1_enable <= false;
  end

  if (addr == 16'hFF17) begin
    if (regs.nr22[3] && s2_enable) s2_env_volume <= (s2_env_volume + 1) & 4'hF;
    if (data[7:3] == 0) s2_enable <= false;
  end

  if (addr == 16'hFF21) begin
    if (regs.nr42[3] && s4_enable) s4_env_volume <= (s4_env_volume + 1) & 4'hF;
    if (data[7:3] == 0) s4_enable <= false;
  end

  if (addr == 16'hFF26) begin
    if (data[7] == 0) reset();
  end

  //----------
  // registers

  regs.bus_write(addr, data);
  
  //----------
  // wavetable

  if (addr >= 16'hFF30 && addr <= 16'hFF3F) begin
    s3_wave[addr[3:0]] <= data;
  end

  //----------
  // triggers

  begin
    logic s1_trigger_ = addr == 16'hFF14 && data[7];
    logic s2_trigger_ = addr == 16'hFF19 && data[7];
    logic s3_trigger_ = addr == 16'hFF1E && data[7];
    logic s4_trigger_ = addr == 16'hFF23 && data[7];

    if (s1_trigger_) begin
      uint8_t s1_sweep_period = regs.nr10[6:4];
      uint8_t s1_length = 64 - regs.nr11[5:0];
      uint8_t s1_start_volume = regs.nr12[7:4];
      uint8_t s1_env_period = regs.nr12[2:0];
      uint16_t s1_freq = {regs.nr14[2:0], regs.nr13};

      s1_enable <= (regs.nr12 & 8'hF8) != 0;
      s1_duration <= s1_length;
      s1_sweep_clock <= s1_sweep_period;
      s1_sweep_freq <= s1_freq;

      s1_env_volume <= regs.nr12[3] ? s1_start_volume : 15 ^ s1_start_volume;
      s1_env_clock <= s1_env_period;

      s1_phase_clock <= 2047 ^ s1_freq;
      s1_phase <= 0;
    end

    if (s2_trigger_) begin
      uint8_t s2_length = 64 - regs.nr21[5:0];
      uint8_t s2_start_volume = regs.nr22[7:4];
      uint8_t s2_env_period = regs.nr22[2:0];
      uint16_t s2_freq = {regs.nr24[2:0], regs.nr23};

      s2_enable <= (regs.nr22 & 8'hF8) != 0;
      s2_duration <= s2_length;

      s2_env_volume <= regs.nr22[3] ? s2_start_volume : 15 ^ s2_start_volume;
      s2_env_clock <= s2_env_period;

      s2_phase_clock <= 2047 ^ s2_freq;
      s2_phase <= 0;
    end

    if (s3_trigger_) begin
      uint16_t s3_length = 256 - regs.nr31;
      uint16_t s3_freq = {regs.nr34[2:0], regs.nr33};

      s3_enable <= (regs.nr32 != 0);
      s3_duration <= s3_length;
      s3_phase_clock <= 2047 ^ s3_freq;
      s3_phase <= 0;
    end

    if (s4_trigger_) begin
      uint8_t s4_length = 64 - regs.nr41[5:0];
      uint8_t s4_start_volume = regs.nr42[7:4];
      uint8_t s4_env_period = regs.nr42[2:0];
      uint8_t s4_phase_period = (regs.nr43[2:0] == 0) ? 1 : {regs.nr43[2:0], 1'b0};

      s4_enable <= (regs.nr42 & 8'hF8) != 0;
      s4_duration <= s4_length;

      s4_env_volume <= regs.nr42[3] ? s4_start_volume : 15 ^ s4_start_volume;
      s4_env_clock <= s4_env_period;

      s4_phase_clock <= s4_phase_period;
      s4_lfsr <= 15'h7FFF;
    end
  end

endtask

//-----------------------------------------------------------------------------

always @(posedge clk) begin
  if (rst) reset();
  else tock();
end

//-----------------------------------------------------------------------------

endmodule