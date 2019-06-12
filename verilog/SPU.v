/* verilator lint_off WIDTH */
/* verilator lint_off UNUSED */

`include "types.v"

module SPU(
  input bool rst,
  input bool clk,
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

ubit14_t spu_clock;

bool sound_on;
ubit8_t volume_r;
ubit8_t volume_l;
ubit8_t s1l, s1r, s2l, s2r, s3l, s3r, s4l, s4r;

bool s1_enable;
bool s2_enable;
bool s3_enable;
bool s4_enable;

ubit4_t s1_volume;
ubit4_t s2_volume;
ubit4_t s3_volume;
ubit4_t s4_volume;

ubit3_t s1_sweep_period;
bool    s1_sweep_dir;
ubit3_t s1_sweep_shift;
ubit3_t s1_sweep_clock;

bool s1_length_enable;
bool s2_length_enable;
bool s3_length_enable;
bool s4_length_enable;

ubit7_t s1_length;
ubit7_t s2_length;
ubit9_t s3_length;
ubit7_t s4_length;

ubit4_t s1_env_volume;
ubit4_t s2_env_volume;
ubit4_t s4_env_volume;

bool s1_env_dir;
bool s2_env_dir;
bool s4_env_dir;

ubit3_t s1_env_period;
ubit3_t s2_env_period;
ubit3_t s4_env_period;

ubit3_t s1_env_clock;
ubit3_t s2_env_clock;
ubit3_t s4_env_clock;

ubit11_t s1_phase_clock;
ubit11_t s2_phase_clock;
ubit11_t s3_phase_clock;
ubit11_t s4_phase_clock;

ubit3_t s1_phase;
ubit3_t s2_phase;
ubit5_t s3_phase;

ubit16_t s1_freq;
ubit16_t s2_freq;
ubit16_t s3_freq;

ubit3_t  s1_duty;
ubit3_t  s2_duty;
bool     s3_power;
ubit3_t  s3_volume_shift;
ubit4_t  s4_phase_period;
ubit4_t  s4_clock_shift;
ubit16_t s4_lfsr;
bool     s4_lfsr_mode;

ubit8_t s3_wave[16];

ubit8_t s1_out;
ubit8_t s2_out;
ubit8_t s3_out;
ubit8_t s4_out;

//-----------------------------------------------------------------------------
// RESET BEGINS HERE
// RESET BEGINS HERE
// RESET BEGINS HERE
// RESET BEGINS HERE
// RESET BEGINS HERE

task reset;
begin
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
  nr30 <= 8'h00;
  nr31 <= 8'h00;
  nr32 <= 8'h00;
  nr33 <= 8'h00;
  nr34 <= 8'h00;
  nr40 <= 8'h80;
  nr41 <= 8'hBF;
  nr42 <= 8'hF3;
  nr43 <= 8'hFF;
  nr44 <= 8'hBF;
  nr50 <= 8'h00;
  nr51 <= 8'h00;
  nr52 <= 8'h00;

  spu_clock <= 0;

  sound_on <= 0;
  volume_r <= 0;
  volume_l <= 0;
  s1l <= 0;
  s1r <= 0;
  s2l <= 0;
  s2r <= 0;
  s3l <= 0;
  s3r <= 0;
  s4l <= 0;
  s4r <= 0;

  s1_enable <= 0;
  s2_enable <= 0;
  s3_enable <= 0;
  s4_enable <= 0;

  s1_volume <= 0;
  s2_volume <= 0;
  s3_volume <= 0;
  s4_volume <= 0;

  s1_sweep_period <= 0;
  s1_sweep_dir <= 0;
  s1_sweep_shift <= 0;
  s1_sweep_clock <= 0;

  s1_length_enable <= 0;
  s2_length_enable <= 0;
  s3_length_enable <= 0;
  s4_length_enable <= 0;

  s1_length <= 0;
  s2_length <= 0;
  s3_length <= 0;
  s4_length <= 0;

  s1_env_volume <= 0;
  s2_env_volume <= 0;
  s4_env_volume <= 0;

  s1_env_dir <= 0;
  s2_env_dir <= 0;
  s4_env_dir <= 0;

  s1_env_period <= 0;
  s2_env_period <= 0;
  s4_env_period <= 0;

  s1_env_clock <= 0;
  s2_env_clock <= 0;
  s4_env_clock <= 0;

  s1_phase_clock <= 2047;
  s2_phase_clock <= 2047;
  s3_phase_clock <= 2047;
  s4_phase_clock <= 2047;

  s1_phase <= 0;
  s2_phase <= 0;
  s3_phase <= 0;

  s1_freq <= 0;
  s2_freq <= 0;
  s3_freq <= 0;

  s1_duty <= 0;
  s2_duty <= 0;
  s3_power <= 0;
  s3_volume_shift <= 0;
  s4_phase_period <= 0;
  s4_clock_shift <= 0;
  s4_lfsr <= 16'h7FFF;
  s4_lfsr_mode <= 0;

  for (i = 0; i < 16; i++) s3_wave[i] <= 0;
end
endtask;

//-----------------------------------------------------------------------------

task tick;
begin
end
endtask

//-----------------------------------------------------------------------------
// TOCK BEGINS HERE
// TOCK BEGINS HERE
// TOCK BEGINS HERE
// TOCK BEGINS HERE
// TOCK BEGINS HERE

task tock;
begin
  ubit14_t spu_clock_ = spu_clock + 1;
  ubit14_t clock_flip = (~spu_clock) & spu_clock_;
  ubit9_t out_r_ = out_r;
  ubit9_t out_l_ = out_l;
  ubit4_t s1_out_, s2_out_, s3_out_, s4_out_;

  bool s1_enable_ = s1_enable;
  bool s2_enable_ = s2_enable;
  bool s3_enable_ = s3_enable;
  bool s4_enable_ = s4_enable;

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

  bool sweep_tick;
  bool length_tick;
  bool env_tick;

  bool s1_trigger_ = write && addr == 16'hFF14 && (data >> 7);
  bool s2_trigger_ = write && addr == 16'hFF19 && (data >> 7);
  bool s3_trigger_ = write && addr == 16'hFF1E && (data >> 7);
  bool s4_trigger_ = write && addr == 16'hFF23 && (data >> 7);

  //----------
  // sweep

  sweep_tick = (spu_clock & 14'b01111111111111) == 14'b01000000000000;

  if (s1_trigger_) begin
    s1_sweep_clock_ = s1_sweep_period;
  end
  else begin
    if (sweep_tick && s1_sweep_period && s1_sweep_shift) begin
      if (s1_sweep_clock) begin
        s1_sweep_clock_ = s1_sweep_clock_ - 1;
      end
      else begin
        ubit11_t delta = s1_freq_ >> s1_sweep_shift;
        ubit11_t new_freq = s1_freq_ + (s1_sweep_dir ? -delta : +delta);
        s1_sweep_clock_ = s1_sweep_period;
        if (new_freq > 2047) begin
          s1_enable_ = false;
        end
        else begin
          s1_freq_ = new_freq;
        end
      end
    end
  end

  //----------
  // length

  if (s1_trigger_) s1_enable_ = true;
  if (s2_trigger_) s2_enable_ = true;
  if (s3_trigger_) s3_enable_ = true;
  if (s4_trigger_) s4_enable_ = true;

  length_tick = (spu_clock & 14'b00111111111111) == 14'b00000000000000;
  if (length_tick) begin
    if (s1_length_enable && s1_length_) s1_length_ = s1_length_ - 1;
    if (s2_length_enable && s2_length_) s2_length_ = s2_length_ - 1;
    if (s3_length_enable && s3_length_) s3_length_ = s3_length_ - 1;
    if (s4_length_enable && s4_length_) s4_length_ = s4_length_ - 1;

    if (s1_length_ == 0) s1_enable_ = false;
    if (s2_length_ == 0) s2_enable_ = false;
    if (s3_length_ == 0) s3_enable_ = false;
    if (s4_length_ == 0) s4_enable_ = false;

  end

  //----------
  // env

  env_tick = (spu_clock & 14'b11111111111111) == 14'b11100000000000;

  if (s1_trigger_) begin
    s1_volume_ = s1_env_volume;
    s1_env_clock_ = s1_env_period;
  end
  else if (env_tick && s1_env_period) begin
    if (s1_env_clock_) begin
      s1_env_clock_--;
    end
    else begin
      s1_env_clock_ = s1_env_period;
      if (s1_volume_ < 15 && s1_env_dir) s1_volume_++;
      if (s1_volume_ > 0 && !s1_env_dir) s1_volume_--;
    end
  end

  if (s2_trigger_) begin
    s2_volume_ = s2_env_volume;
    s2_env_clock_ = s2_env_period;
  end else if (env_tick && s2_env_period) begin
    if (s2_env_clock_) begin
      s2_env_clock_--;
    end
    else begin
      s2_env_clock_ = s2_env_period;
      if (s2_volume_ < 15 && s2_env_dir) s2_volume_++;
      if (s2_volume_ > 0 && !s2_env_dir) s2_volume_--;
    end
  end

  if (s4_trigger_) begin
    s4_volume_ = s4_env_volume;
    s4_env_clock_ = s4_env_period;
  end
  else if (env_tick && s4_env_period) begin
    if (s4_env_clock_) begin
      s4_env_clock_--;
    end
    else begin
      s4_env_clock_ = s4_env_period;
      if (s4_volume_ < 15 && s4_env_dir) s4_volume_++;
      if (s4_volume_ > 0 && !s4_env_dir) s4_volume_--;
    end
  end

  //----------
  // phase

  if (s1_trigger_) begin
    s1_phase_clock_ = 2047 ^ s1_freq;
    s1_phase_ = 0;
  end
  else if (!s1_phase_clock_) begin
    s1_phase_clock_ = 2047 ^ s1_freq;
    s1_phase_ = (s1_phase_ + 1) & 7;
  end
  else begin
    s1_phase_clock_--;
  end

  if (s2_trigger_) begin
    s2_phase_clock_ = 2047 ^ s2_freq;
    s2_phase_ = 0;
  end
  else if (!s2_phase_clock_) begin
    s2_phase_clock_ = 2047 ^ s2_freq;
    s2_phase_ = (s2_phase_ + 1) & 7;
  end
  else begin
    s2_phase_clock_--;
  end

  if (s3_trigger_) begin
    s3_phase_clock_ = 2047 ^ s3_freq;
    s3_phase_ = 0;
  end
  else begin
    // we run this twice because this is ticking at 1 mhz
    if (!s3_phase_clock_) begin
      s3_phase_clock_ = 2047 ^ s3_freq;
      s3_phase_ = (s3_phase_ + 1) & 31;
    end
    else begin
      s3_phase_clock_--;
    end

    if (!s3_phase_clock_) begin
      s3_phase_clock_ = 2047 ^ s3_freq;
      s3_phase_ = (s3_phase_ + 1) & 31;
    end
    else begin
      s3_phase_clock_--;
    end
  end

  if (s4_trigger_) begin
    s4_phase_clock_ = s4_phase_period;
    s4_lfsr_ = 16'hFFFF;
  end
  else if (!s4_phase_clock_) begin
    bool lfsr_bit;
    s4_phase_clock_ = s4_phase_period;
    lfsr_bit = (s4_lfsr_ ^ (s4_lfsr_ >> 1)) & 1;
    s4_lfsr_ >>= 1;
    s4_lfsr_ |= (lfsr_bit << 14);
    if (s4_lfsr_mode) begin
      s4_lfsr_ &= ~(1 << 5);
      s4_lfsr_ |= (lfsr_bit << 5);
    end
  end
  else begin
    s4_phase_clock_ -= (clock_flip >> s4_clock_shift) & 1;
  end

  //----------
  // output

  if (sound_on) begin
    ubit4_t s3_sample_;
    if (s3_phase & 1) begin
      s3_sample_ = (s3_wave[s3_phase >> 1] >> 0) & 8'h0F;
    end
    else begin
      s3_sample_ = (s3_wave[s3_phase >> 1] >> 4) & 8'h0F;
    end
    s3_sample_ >>= s3_volume_shift;

    s1_out_ = s1_enable ? (s1_phase < s1_duty ? s1_volume : 0) : 0;
    s2_out_ = s2_enable ? (s2_phase < s2_duty ? s2_volume : 0) : 0;
    s3_out_ = s3_enable ? (s3_power ? s3_sample_ : 0) : 0;
    s4_out_ = s4_enable ? (s4_lfsr & 1 ? s4_volume : 0) : 0;

    out_r_ = (s1_out_ & s1r) + (s2_out_ & s2r) + (s3_out_ & s3r) + (s4_out_ & s4r);
    out_l_ = (s1_out_ & s1l) + (s2_out_ & s2l) + (s3_out_ & s3l) + (s4_out_ & s4l);

    out_r_ *= volume_r;
    out_l_ *= volume_l;
  end
  else begin
    out_r_ = 0;
    out_l_ = 0;
  end

  //----------
  // commit

  if (read) bus_read(addr);

  spu_clock <= spu_clock_;

  s1_enable <= s1_enable_;
  s2_enable <= s2_enable_;
  s3_enable <= s3_enable_;
  s4_enable <= s4_enable_;

  s1_volume <= s1_volume_;
  s2_volume <= s2_volume_;
  s4_volume <= s4_volume_;

  s1_sweep_clock <= s1_sweep_clock_;
  s1_freq <= s1_freq_;

  s1_length <= s1_length_;
  s2_length <= s2_length_;
  s3_length <= s3_length_;
  s4_length <= s4_length_;

  s1_env_clock <= s1_env_clock_;
  s2_env_clock <= s2_env_clock_;
  s4_env_clock <= s4_env_clock_;

  s1_phase_clock <= s1_phase_clock_;
  s2_phase_clock <= s2_phase_clock_;
  s3_phase_clock <= s3_phase_clock_;
  s4_phase_clock <= s4_phase_clock_;

  s1_phase <= s1_phase_;
  s2_phase <= s2_phase_;
  s3_phase <= s3_phase_;
  s4_lfsr <= s4_lfsr_;

  s1_out <= s1_out_;
  s2_out <= s2_out_;
  s3_out <= s3_out_;
  s4_out <= s4_out_;

  out_r <= out_r_;
  out_l <= out_l_;

  if (write) bus_write(addr, data);
end
endtask;

//-----------------------------------------------------------------------------
// BUS READ
// BUS READ
// BUS READ
// BUS READ
// BUS READ

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
    bus_out <= s3_wave[addr & 4'hF];
  end
endtask

//-----------------------------------------------------------------------------
// BUS WRITE
// BUS WRITE
// BUS WRITE
// BUS WRITE
// BUS WRITE

task bus_write(input ubit16_t addr, input ubit8_t data);
  case(addr)
  
  //----------
  // nrx0 - sweep and wave power

  16'hFF10: begin
    nr10 <= data | 8'b10000000;
    s1_sweep_period <= (data & 8'b01110000) >> 4;
    s1_sweep_dir    <= (data & 8'b00001000) >> 3;
    s1_sweep_shift  <= (data & 8'b00000111) >> 0;
  end
  16'hFF1A: begin
    nr30 <= data;
    s3_power <= (data & 8'b10000000) >> 7;
  end

  //----------
  // nrx1 - duty and length

  16'hFF11: begin
    case((data & 8'b11000000) >> 6)
    0: s1_duty <= 1;
    1: s1_duty <= 2;
    2: s1_duty <= 4;
    3: s1_duty <= 6;
    endcase

    s1_length <= 64;
    if (data & 8'b00111111) s1_length <= 63 ^ (data & 8'b00111111);

    nr11 <= data;
  end

  16'hFF16: begin
    case ((data & 8'b11000000) >> 6)
    0: s2_duty <= 1;
    1: s2_duty <= 2;
    2: s2_duty <= 4;
    3: s2_duty <= 6;
    endcase

    s2_length <= 64;
    if (data & 8'b00111111) s2_length <= 63 ^ (data & 8'b00111111);

    nr21 <= data;
  end

  16'hFF1B: begin
    s3_length <= 256;
    if (data) s3_length <= 255 ^ data;

    nr31 <= data;
  end

  16'hFF20: begin
    if ((data & 8'b00111111) == 0) begin
      s4_length <= 64;
    end else begin
      s4_length <= 63 ^ (data & 8'b00111111);
    end

    nr41 <= data;
  end

  //----------
  // nrx2 - volume and envelope

  16'hFF12: begin
    s1_enable <= (data != 0);
    s1_env_volume <= (data & 8'b11110000) >> 4;
    s1_env_dir    <= (data & 8'b00001000) >> 3;
    s1_env_period <= (data & 8'b00000111) >> 0;

    nr12 <= data;
  end

  16'hFF17: begin
    s2_enable <= (data != 0);
    s2_env_volume <= (data & 8'b11110000) >> 4;
    s2_env_dir    <= (data & 8'b00001000) >> 3;
    s2_env_period <= (data & 8'b00000111) >> 0;

    nr22 <= data;
  end

  16'hFF1C: begin
    s3_enable <= (data != 0);
    case ((data & 8'b01100000) >> 5)
    0: begin s3_volume_shift <= 4; s3_volume <= 0; end
    1: begin s3_volume_shift <= 0; s3_volume <= 15; end
    2: begin s3_volume_shift <= 1; s3_volume <= 7; end
    3: begin s3_volume_shift <= 2; s3_volume <= 3; end
    endcase

    nr32 <= data;
  end

  16'hFF21: begin
    s4_enable <= (data != 0);
    s4_env_volume <= (data & 8'b11110000) >> 4;
    s4_env_dir    <= (data & 8'b00001000) >> 3;
    s4_env_period <= (data & 8'b00000111) >> 0;

    nr42 <= data;
  end

  //----------
  // nrx3 - frequency lsb, lfsr clock/mode/shift

  16'hFF13: begin
    s1_freq <= (s1_freq & 16'hFF00) | data;
    nr13 <= data;
  end

  16'hFF18: begin
    s2_freq <= (s2_freq & 16'hFF00) | data;
    nr23 <= data;
  end

  16'hFF1D: begin
    s3_freq <= (s2_freq & 16'hFF00) | data;
    nr33 <= data;
  end

  16'hFF22: begin
    // period is actually [1, 2, 4, 6, 8, 10, 12, 14]
    if (data & 8'b00000111) begin
      s4_phase_period <= (data & 8'b00000111) * 2;
    end else begin
      s4_phase_period <= 1;
    end
    s4_lfsr_mode <= (data & 8'b00001000) >> 3;
    s4_clock_shift <= (data & 8'b11110000) >> 4;

    nr43 <= data;
  end
  
  //----------
  // nrx4 - frequency msb, length_enable, trigger

  16'hFF14: begin
    s1_freq <= (s1_freq & 16'h00FF) | ((data & 8'b00000111) << 8);
    s1_length_enable <= (data & 8'b01000000) >> 6;

    nr14 <= data;
  end

  16'hFF19: begin
    s2_freq <= (s2_freq & 16'h00FF) | ((data & 8'b00000111) << 8);
    s2_length_enable <= (data & 8'b01000000) >> 6;

    nr24 <= data;
  end

  16'hFF1E: begin
    s3_freq <= (s3_freq & 16'h00FF) | ((data & 8'b00000111) << 8);
    s3_length_enable <= (data & 8'b01000000) >> 6;

    nr34 <= data;
  end

  16'hFF23: begin
    s4_length_enable <= (data & 8'b01000000) >> 6;

    nr44 <= data;
  end

  //----------
  // global spu stuff

  16'hFF24: begin
    nr50     <= data;
    volume_r <= ((data & 8'b00000111) >> 0) + 1;
    volume_l <= ((data & 8'b01110000) >> 4) + 1;
  end
  16'hFF25: begin
    nr51 <= data;
    s1r  <= (data & 8'b00000001) ? 8'hFF : 8'h00;
    s2r  <= (data & 8'b00000010) ? 8'hFF : 8'h00;
    s3r  <= (data & 8'b00000100) ? 8'hFF : 8'h00;
    s4r  <= (data & 8'b00001000) ? 8'hFF : 8'h00;
    s1l  <= (data & 8'b00010000) ? 8'hFF : 8'h00;
    s2l  <= (data & 8'b00100000) ? 8'hFF : 8'h00;
    s3l  <= (data & 8'b01000000) ? 8'hFF : 8'h00;
    s4l  <= (data & 8'b10000000) ? 8'hFF : 8'h00;
  end
  16'hFF26: begin
    nr52 <= data;
    sound_on <= (data >> 7) & 1;
  end
  endcase

  //----------
  // wavetable

  if (addr >= 16'hFF30 && addr <= 16'hFF3F) begin
    s3_wave[addr & 4'hF] <= data;
  end

  //----------
  // triggers

  begin
    logic s1_trigger_ = (addr == 16'hFF14) && (data >> 7);
    logic s2_trigger_ = (addr == 16'hFF19) && (data >> 7);
    logic s3_trigger_ = (addr == 16'hFF1E) && (data >> 7);
    logic s4_trigger_ = (addr == 16'hFF23) && (data >> 7);

    if (s1_trigger_) begin
      s1_enable <= true;
      s1_sweep_clock <= s1_sweep_period;
      s1_volume <= s1_env_volume;
      s1_env_clock <= s1_env_period;
      s1_phase_clock <= 2047 ^ s1_freq;
      s1_phase <= 0;
    end

    if (s2_trigger_) begin
      s2_enable <= true;
      s2_volume <= s2_env_volume;
      s2_env_clock <= s2_env_period;
      s2_phase_clock <= 2047 ^ s2_freq;
      s2_phase <= 0;
    end

    if (s3_trigger_) begin
      s3_enable <= true;
      s3_phase_clock <= 2047 ^ s3_freq;
      s3_phase <= 0;
    end

    if (s4_trigger_) begin
      s4_enable <= true;
      s4_volume <= s4_env_volume;
      s4_env_clock <= s4_env_period;
      s4_phase_clock <= s4_phase_period;
      s4_lfsr <= 16'h7FFF;
    end
  end

endtask

//-----------------------------------------------------------------------------

always @* tick();
always @(posedge clk) begin
  if (rst) reset();
  else tock();
end

endmodule

//-----------------------------------------------------------------------------

