`include "types.v"

module SPU(
  input bool rst,
  input bool clk,
  ubit2_t tphase,
  input ubit16_t addr,
  input ubit8_t data,
  input bool read,
  input bool write,
  output ubit8_t bus_out,
  output bool bus_oe,
  output ubit9_t out_r,
  output ubit9_t out_l
);

parameter true=1;
parameter false=0;

ubit8_t nr10;
ubit8_t nr11;
ubit8_t nr12;
ubit8_t nr13;
ubit8_t nr14;
ubit8_t nr20;
ubit8_t nr21;
ubit8_t nr22;
ubit8_t nr23;
ubit8_t nr24;
ubit8_t nr30;
ubit8_t nr31;
ubit8_t nr32;
ubit8_t nr33;
ubit8_t nr34;
ubit8_t nr40;
ubit8_t nr41;
ubit8_t nr42;
ubit8_t nr43;
ubit8_t nr44;
ubit8_t nr50;
ubit8_t nr51;
ubit8_t nr52;

ubit8_t s3_wave[16];

ubit14_t spu_clock;

bool s1_enable;
bool s2_enable;
bool s3_enable;
bool s4_enable;

ubit7_t s1_duration;
ubit7_t s2_duration;
ubit9_t s3_duration;
ubit7_t s4_duration;

ubit3_t s1_sweep_clock;
ubit11_t s1_sweep_freq;

ubit4_t s1_env_volume;
ubit4_t s2_env_volume;
ubit4_t s4_env_volume;
ubit3_t s1_env_clock;
ubit3_t s2_env_clock;
ubit3_t s4_env_clock;

ubit11_t s1_phase_clock;
ubit11_t s2_phase_clock;
ubit11_t s3_phase_clock;
ubit4_t s4_phase_clock;
ubit3_t s1_phase;
ubit3_t s2_phase;
ubit5_t s3_phase;
ubit15_t s4_lfsr;

ubit4_t s1_out;
ubit4_t s2_out;
ubit4_t s3_out;
ubit4_t s4_out;

/*
function bit_set(logic[31:0] a, int b);
  return a[b];
endfunction
*/

//let bit_set(a, b) = a[b];

//-----------------------------------------------------------------------------

task reset;
  integer i;

  bus_out <= 0;
  bus_oe <= 0;
  out_r <= 0;
  out_l <= 0;

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

  for (i = 0; i < 16; i++) s3_wave[i] <= 0;
  s4_lfsr <= 15'h7FFF;

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
  `define bit_set(a,b) a[b]
  
  logic sound_on = `bit_set(nr52, 7);
  ubit14_t spu_clock_ = spu_clock + 14'b1;
  ubit14_t clock_flip = (~spu_clock) & spu_clock_;
  logic sweep_tick, length_tick, env_tick;
  ubit4_t s1_out_;
  ubit4_t s2_out_;
  ubit4_t s3_out_;
  ubit4_t s4_out_;
  ubit9_t out_r_ = 0;
  ubit9_t out_l_ = 0;

  //----------

  if (read) bus_read(addr);
  if (write && (sound_on || addr == 16'hFF26)) bus_write(addr, data);
  if (tphase != 2) return;

  if (!sound_on) begin
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
    ubit3_t s1_sweep_period = nr10[6:4];
    ubit3_t s1_sweep_shift = nr10[2:0];
    bool s1_sweep_dir = nr10[3];
    ubit11_t s1_freq = {nr14[2:0], nr13};

    if ((s1_sweep_period != 0) && (s1_sweep_shift != 0)) begin
      if (s1_sweep_clock != 0) begin
        s1_sweep_clock <= s1_sweep_clock - 1;
      end
      else begin
        ubit11_t delta = s1_sweep_freq >> s1_sweep_shift;
        ubit11_t new_freq = s1_sweep_freq + (s1_sweep_dir ? -delta : +delta);
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
    bool s1_length_enable = nr14[6];
    bool s2_length_enable = nr24[6];
    bool s3_length_enable = nr34[6];
    bool s4_length_enable = nr44[6];
    ubit7_t s1_duration_ = s1_duration;
    ubit7_t s2_duration_ = s2_duration;
    ubit9_t s3_duration_ = s3_duration;
    ubit7_t s4_duration_ = s4_duration;

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
    bool s1_env_dir = nr12[3];
    bool s2_env_dir = nr22[3];
    bool s4_env_dir = nr42[3];

    ubit3_t s1_env_period = nr12[2:0];
    ubit3_t s2_env_period = nr22[2:0];
    ubit3_t s4_env_period = nr42[2:0];

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
    ubit3_t s1_sweep_period = nr10[6:4];
    ubit11_t s1_freq = {nr14[2:0], nr13};
      
    s1_phase_clock <= 2047 ^ (s1_sweep_period != 0 ? s1_sweep_freq : s1_freq);
    s1_phase <= (s1_phase + 1) & 7;
  end
  else begin
    s1_phase_clock <= s1_phase_clock - 1;
  end

  if (s2_phase_clock == 0) begin
    ubit11_t s2_freq = {nr24[2:0], nr23};
      
    s2_phase_clock <= 2047 ^ s2_freq;
    s2_phase <= (s2_phase + 1) & 7;
  end
  else begin
    s2_phase_clock <= s2_phase_clock - 1;
  end

  // we run this twice because this is ticking at 1 mhz
  begin
    ubit11_t s3_freq = {nr34[2:0], nr33};
    ubit11_t s3_phase_clock_ = s3_phase_clock;
    ubit5_t s3_phase_ = s3_phase;
    
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
    bool s4_lfsr_mode = nr43[3];
    ubit4_t s4_phase_period = nr43[2:0] != 0 ? {nr43[2:0], 1'b0} : 1;
    bool lfsr_bit = s4_lfsr[0] ^ s4_lfsr[1];

    s4_phase_clock <= s4_phase_period;
    if (s4_lfsr_mode) begin
      s4_lfsr <= {10'b0, lfsr_bit, s4_lfsr[4:1]};
    end else begin
      s4_lfsr <= {lfsr_bit, s4_lfsr[14:1]};
    end
  end
  else begin
    ubit4_t s4_clock_shift = nr43[7:4];
    s4_phase_clock <= s4_phase_clock - {3'b0, clock_flip[s4_clock_shift]};
  end

  //----------
  // output

  s1_out_ = 0;
  s2_out_ = 0;
  s3_out_ = 0;
  s4_out_ = 0;

  if (s1_enable) begin
    ubit4_t s1_sample;
    ubit4_t s1_volume = nr12[3] ? s1_env_volume : 15 ^ s1_env_volume;
    ubit3_t s1_duty = nr11[7:6] == 0 ? 1 : {nr11[7:6], 1'b0};
    
    s1_sample = (s1_phase < s1_duty) ? s1_volume : 0;
    s1_out_ = s1_sample;
  end

  if (s2_enable) begin
    ubit4_t s2_sample;
    ubit4_t s2_volume = nr22[3] ? s2_env_volume : 15 ^ s2_env_volume;
    ubit3_t s2_duty = nr21[7:6] == 0 ? 1 : {nr11[7:6], 1'b0};

    s2_sample = (s2_phase < s2_duty) ? s2_volume : 0;
    s2_out_ = s2_sample;
  end

  if (s3_enable && nr30[7]) begin
    ubit8_t s3_byte = s3_wave[s3_phase[4:1]];
    ubit4_t s3_sample = s3_phase[0] ? s3_byte[3:0] : s3_byte[7:4];
    ubit3_t s3_volume_shift = 0;

    case ((nr32 & 8'b01100000) >> 5) 
    0: s3_volume_shift = 4;
    1: s3_volume_shift = 0;
    2: s3_volume_shift = 1;
    3: s3_volume_shift = 2;
    endcase

    s3_out_ = s3_sample >> s3_volume_shift;
  end

  if (s4_enable) begin
    ubit4_t s4_volume = nr42[3] ? s4_env_volume : 15 ^ s4_env_volume;
    ubit4_t s4_sample = s4_lfsr[0] ? s4_volume : 0;
    s4_out_ = s4_sample;
  end

  //----------
  // mixer & master volume

  begin
    ubit4_t volume_r = nr50[2:0] + 1;
    ubit4_t volume_l = nr50[6:4] + 1;

    if (`bit_set(nr51, 0)) out_r_ += {5'b0, s1_out_};
    if (`bit_set(nr51, 1)) out_r_ += {5'b0, s2_out_};
    if (`bit_set(nr51, 2)) out_r_ += {5'b0, s3_out_};
    if (`bit_set(nr51, 3)) out_r_ += {5'b0, s4_out_};
    if (`bit_set(nr51, 4)) out_l_ += {5'b0, s1_out_};
    if (`bit_set(nr51, 5)) out_l_ += {5'b0, s2_out_};
    if (`bit_set(nr51, 6)) out_l_ += {5'b0, s3_out_};
    if (`bit_set(nr51, 7)) out_l_ += {5'b0, s4_out_};

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

task bus_read(input ubit16_t addr);
  bus_oe <= 1;
  case(addr)
  16'hFF10: bus_out <= nr10 | 8'h80;
  16'hFF11: bus_out <= nr11 | 8'h3F;
  16'hFF12: bus_out <= nr12 | 8'h00;
  16'hFF13: bus_out <= nr13 | 8'hFF;
  16'hFF14: bus_out <= nr14 | 8'hBF;
  
  16'hFF15: bus_out <= nr20 | 8'hFF;
  16'hFF16: bus_out <= nr21 | 8'h3F;
  16'hFF17: bus_out <= nr22 | 8'h00;
  16'hFF18: bus_out <= nr23 | 8'hFF;
  16'hFF19: bus_out <= nr24 | 8'hBF;
  
  16'hFF1A: bus_out <= nr30 | 8'h7F;
  16'hFF1B: bus_out <= nr31 | 8'hFF;
  16'hFF1C: bus_out <= nr32 | 8'h9F;
  16'hFF1D: bus_out <= nr33 | 8'hFF;
  16'hFF1E: bus_out <= nr34 | 8'hBF;
  
  16'hFF1F: bus_out <= nr40 | 8'hFF;
  16'hFF20: bus_out <= nr41 | 8'hFF;
  16'hFF21: bus_out <= nr42 | 8'h00;
  16'hFF22: bus_out <= nr43 | 8'h00;
  16'hFF23: bus_out <= nr44 | 8'hBF;
  
  16'hFF24: bus_out <= nr50 | 8'h00;
  16'hFF25: bus_out <= nr51 | 8'h00;

  16'hFF26: begin
    ubit8_t bus_out_ = (nr52 & 8'h80) | 8'h70;
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

task bus_write(input ubit16_t addr, input ubit8_t data);
  //----------
  // glitches n stuff

  if (addr == 16'hFF12) begin
    if (nr12[3] && s1_enable) s1_env_volume <= (s1_env_volume + 1) & 4'hF;
    if (data[7:3] == 0) s1_enable <= false;
  end

  if (addr == 16'hFF17) begin
    if (nr22[3] && s2_enable) s2_env_volume <= (s2_env_volume + 1) & 4'hF;
    if (data[7:3] == 0) s2_enable <= false;
  end

  if (addr == 16'hFF21) begin
    if (nr42[3] && s4_enable) s4_env_volume <= (s4_env_volume + 1) & 4'hF;
    if (data[7:3] == 0) s4_enable <= false;
  end

  if (addr == 16'hFF26) begin
    if (data[7] == 0) reset();
  end

  //----------
  // registers

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
      ubit3_t s1_sweep_period = nr10[6:4];
      ubit7_t s1_length = 64 - nr11[5:0];
      ubit4_t s1_start_volume = nr12[7:4];
      ubit3_t s1_env_period = nr12[2:0];
      ubit11_t s1_freq = {nr14[2:0], nr13};

      s1_enable <= (nr12 & 8'hF8) != 0;
      s1_duration <= s1_length;
      s1_sweep_clock <= s1_sweep_period;
      s1_sweep_freq <= s1_freq;

      s1_env_volume <= nr12[3] ? s1_start_volume : 15 ^ s1_start_volume;
      s1_env_clock <= s1_env_period;

      s1_phase_clock <= 2047 ^ s1_freq;
      s1_phase <= 0;
    end

    if (s2_trigger_) begin
      ubit7_t s2_length = 64 - nr21[5:0];
      ubit4_t s2_start_volume = nr22[7:4];
      ubit3_t s2_env_period = nr22[2:0];
      ubit11_t s2_freq = {nr24[2:0], nr23};

      s2_enable <= (nr22 & 8'hF8) != 0;
      s2_duration <= s2_length;

      s2_env_volume <= nr22[3] ? s2_start_volume : 15 ^ s2_start_volume;
      s2_env_clock <= s2_env_period;

      s2_phase_clock <= 2047 ^ s2_freq;
      s2_phase <= 0;
    end

    if (s3_trigger_) begin
      ubit9_t s3_length = 256 - nr31;
      ubit11_t s3_freq = {nr34[2:0], nr33};

      s3_enable <= (nr32 != 0);
      s3_duration <= s3_length;
      s3_phase_clock <= 2047 ^ s3_freq;
      s3_phase <= 0;
    end

    if (s4_trigger_) begin
      ubit7_t s4_length = 64 - nr41[5:0];
      ubit4_t s4_start_volume = nr42[7:4];
      ubit3_t s4_env_period = nr42[2:0];
      ubit4_t s4_phase_period = (nr43[2:0] == 0) ? 1 : {nr43[2:0], 1'b0};

      s4_enable <= (nr42 & 8'hF8) != 0;
      s4_duration <= s4_length;

      s4_env_volume <= nr42[3] ? s4_start_volume : 15 ^ s4_start_volume;
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

endmodule

//-----------------------------------------------------------------------------

