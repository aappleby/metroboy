typedef logic        bool;
typedef logic[7:0]   uint8_t;
typedef logic[15:0]  uint16_t;
typedef logic[31:0]  uint32_t;

module SPU(
  input bool rst,
  input bool clk,
  input uint8_t tphase,
  input uint16_t addr,
  input uint8_t data,
  input bool read,
  input bool write,
  output uint8_t bus_out,
  output bool bus_oe,
  output uint16_t out_r,
  output uint16_t out_l
);

parameter true=1;
parameter false=0;

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
  bool sound_on = (nr52 & 8'h80) != 0;
  uint16_t spu_clock_ = (spu_clock + 1) & 16'h3FFF;
  uint16_t clock_flip = (~spu_clock) & spu_clock_;
  bool sweep_tick, length_tick, env_tick;
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

  sweep_tick =  (spu_clock_ & 16'b01111111111111) == 16'b01000000000000;
  length_tick = (spu_clock_ & 16'b00111111111111) == 16'b00000000000000;
  env_tick =    (spu_clock_ & 16'b11111111111111) == 16'b11100000000000;

  if (sweep_tick) begin
    uint8_t s1_sweep_period = (nr10 & 8'b01110000) >> 4;
    uint8_t s1_sweep_shift = (nr10 & 8'b00000111) >> 0;
    /* verilator lint_off WIDTH */
    bool s1_sweep_dir = (nr10 & 8'b00001000) >> 3;
    /* verilator lint_on WIDTH */

    if (s1_sweep_period && s1_sweep_shift) begin
      if (s1_sweep_clock) begin
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
    /* verilator lint_off WIDTH */
    bool s1_length_enable = (nr14 & 8'b01000000) >> 6;
    bool s2_length_enable = (nr24 & 8'b01000000) >> 6;
    bool s3_length_enable = (nr34 & 8'b01000000) >> 6;
    bool s4_length_enable = (nr44 & 8'b01000000) >> 6;
    /* verilator lint_on WIDTH */
    uint8_t s1_duration_ = s1_duration;
    uint8_t s2_duration_ = s2_duration;
    uint16_t s3_duration_ = s3_duration;
    uint8_t s4_duration_ = s4_duration;

    if (s1_length_enable && s1_duration_)) s1_duration_ = s1_duration_ - 1;
    if (s2_length_enable && s2_duration_)) s2_duration_ = s2_duration_ - 1;
    if (s3_length_enable && s3_duration_)) s3_duration_ = s3_duration_ - 1;
    if (s4_length_enable && s4_duration_)) s4_duration_ = s4_duration_ - 1;

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
    /* verilator lint_off WIDTH */
    bool s1_env_dir = (nr12 & 8'b00001000) >> 3;
    bool s2_env_dir = (nr22 & 8'b00001000) >> 3;
    bool s4_env_dir = (nr42 & 8'b00001000) >> 3;
    /* verilator lint_on WIDTH */

    uint8_t s1_env_period = (nr12 & 8'b00000111) >> 0;
    uint8_t s2_env_period = (nr22 & 8'b00000111) >> 0;
    uint8_t s4_env_period = (nr42 & 8'b00000111) >> 0;

    if (s1_env_period) begin
      if (s1_env_clock) begin
        s1_env_clock <= s1_env_clock - 1;
      end
      else begin
        s1_env_clock <= s1_env_period;
        if (s1_env_volume < 15) s1_env_volume <= s1_env_volume + 1;
      end
    end

    if (s2_env_period) begin
      if (s2_env_clock) begin
        s2_env_clock <= s2_env_clock - 1;
      end
      else  begin
        s2_env_clock <= s2_env_period;
        if (s2_env_volume < 15) s2_env_volume <= s2_env_volume + 1;
      end
    end

    if (s4_env_period) begin
      if (s4_env_clock) begin
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

  if (!s1_phase_clock) begin
    uint8_t s1_sweep_period = (nr10 & 8'b01110000) >> 4;
    /* verilator lint_off WIDTH */    
    uint16_t s1_freq = ((nr14 << 8) | nr13) & 16'h07FF;
    /* verilator lint_on WIDTH */
      
    s1_phase_clock <= 2047 ^ (s1_sweep_period ? s1_sweep_freq : s1_freq);
    s1_phase <= (s1_phase + 1) & 7;
  end
  else begin
    s1_phase_clock <= s1_phase_clock - 1;
  end

  if (!s2_phase_clock) begin
    /* verilator lint_off WIDTH */    
    uint16_t s2_freq = ((nr24 << 8) | nr23) & 16'h07FF;
    /* verilator lint_on WIDTH */
      
    s2_phase_clock <= 2047 ^ s2_freq;
    s2_phase <= (s2_phase + 1) & 7;
  end
  else begin
    s2_phase_clock <= s2_phase_clock - 1;
  end

  // we run this twice because this is ticking at 1 mhz
  begin
    /* verilator lint_off WIDTH */    
    uint16_t s3_freq = ((nr34 << 8) | nr33) & 16'h07FF;
    /* verilator lint_on WIDTH */    
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

  if (!s4_phase_clock) begin
    /* verilator lint_off WIDTH */    
    bool s4_lfsr_mode = (nr43 & 8'b00001000) >> 3;
    uint8_t s4_phase_period = (nr43 & 8'b00000111) ? (nr43 & 8'b00000111) * 2 : 1;
    bool lfsr_bit = (s4_lfsr ^ (s4_lfsr >> 1)) & 1;
    /* verilator lint_on WIDTH */    

    s4_phase_clock <= s4_phase_period;
    if (s4_lfsr_mode) begin
      /* verilator lint_off WIDTH */    
      s4_lfsr <= ((s4_lfsr >> 1) & 16'b0111111111011111) | (lfsr_bit << 5);
      /* verilator lint_on WIDTH */    
    end else begin
      /* verilator lint_off WIDTH */    
      s4_lfsr <= ((s4_lfsr >> 1) & 16'b0011111111111111) | (lfsr_bit << 14);
      /* verilator lint_on WIDTH */    
    end
  end
  else begin
    uint8_t s4_clock_shift = (nr43 & 8'b11110000) >> 4;
    /* verilator lint_off WIDTH */    
    s4_phase_clock <= s4_phase_clock - ((clock_flip >> s4_clock_shift) & 1);
    /* verilator lint_on WIDTH */    
  end

  //----------
  // output

  s1_out_ = 0;
  s2_out_ = 0;
  s3_out_ = 0;
  s4_out_ = 0;

  if (s1_enable) begin
    /* verilator lint_off WIDTH */    
    uint8_t s1_volume = (nr12 & 8'h08) ? s1_env_volume : 15 ^ s1_env_volume;
    uint8_t s1_duty = (nr11 & 8'b11000000) ? ((nr11 & 8'b11000000) >> 6) * 2 : 1;
    /* verilator lint_on WIDTH */    
    uint8_t s1_sample = (s1_phase < s1_duty) ? s1_volume : 0;
    s1_out <= s1_sample;
  end

  if (s2_enable) begin
    /* verilator lint_off WIDTH */    
    uint8_t s2_volume = (nr22 & 8'h08) ? s2_env_volume : 15 ^ s2_env_volume;
    uint8_t s2_duty = (nr21 & 8'b11000000) ? ((nr21 & 8'b11000000) >> 6) * 2 : 1;
    /* verilator lint_on WIDTH */    
    uint8_t s2_sample = (s2_phase < s2_duty) ? s2_volume : 0;
    s2_out_ = s2_sample;
  end

  /* verilator lint_off WIDTH */    
  if (s3_enable && (nr30 & 8'b10000000)) begin
    uint8_t s3_byte = s3_wave[s3_phase >> 1];
    uint8_t s3_sample = (s3_phase & 1) ? (s3_byte & 8'h0F) : (s3_byte >> 4);
    /* verilator lint_on WIDTH */    
    uint8_t s3_volume_shift = 0;

    case ((nr32 & 8'b01100000) >> 5) 
    0: s3_volume_shift = 4;
    1: s3_volume_shift = 0;
    2: s3_volume_shift = 1;
    3: s3_volume_shift = 2;
    endcase

    s3_out_ = s3_sample >> s3_volume_shift;
  end

  if (s4_enable) begin
    /* verilator lint_off WIDTH */    
    uint8_t s4_volume = (nr42 & 8'h08) ? s4_env_volume : 15 ^ s4_env_volume;
    uint8_t s4_sample = (s4_lfsr & 1) ? s4_volume : 0;
    /* verilator lint_on WIDTH */    
    s4_out_ = s4_sample;
  end

  //----------
  // mixer & master volume

  out_r_ = 0;
  out_l_ = 0;

  /* verilator lint_off WIDTH */    
  if (nr51 & 8'b00000001) out_r_ += s1_out;
  if (nr51 & 8'b00000010) out_r_ += s2_out;
  if (nr51 & 8'b00000100) out_r_ += s3_out;
  if (nr51 & 8'b00001000) out_r_ += s4_out;
  if (nr51 & 8'b00010000) out_l_ += s1_out;
  if (nr51 & 8'b00100000) out_l_ += s2_out;
  if (nr51 & 8'b01000000) out_l_ += s3_out;
  if (nr51 & 8'b10000000) out_l_ += s4_out;
  /* verilator lint_on WIDTH */    

  begin
    uint8_t volume_r = ((nr50 & 8'b00000111) >> 0) + 1;
    uint8_t volume_l = ((nr50 & 8'b01110000) >> 4) + 1;

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
    uint8_t bus_out_ = (nr52 & 8'h80) | 8'h70;
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
    if ((nr12 & 8'h08) && s1_enable) s1_env_volume <= (s1_env_volume + 1) & 15;
    if (!(data & 8'hF8) s1_enable <= false;
  end

  if (addr == 16'hFF17) begin
    if ((nr22 & 8'h08) && s2_enable) s2_env_volume <= (s2_env_volume + 1) & 15;
    if (!(data & 8'hF8) s2_enable <= false;
  end

  if (addr == 16'hFF21) begin
    if ((nr42 & 8'h08) && s4_enable) s4_env_volume <= (s4_env_volume + 1) & 15;
    if (!(data & 8'hF8) s4_enable <= false;
  end

  if (addr == 16'hFF26) begin
    if (!(data & 8'h80) reset();
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
      uint8_t s1_sweep_period = (nr10 & 8'b01110000) >> 4;
      uint8_t s1_length = 64 - (nr11 & 8'b00111111);
      uint8_t s1_start_volume = (nr12 & 8'b11110000) >> 4;
      uint8_t s1_env_period = (nr12 & 8'b00000111) >> 0;
      /* verilator lint_off WIDTH */
      uint16_t s1_freq = ((nr14 << 8) | nr13) & 16'h07FF;
      /* verilator lint_on WIDTH */

      s1_enable <= (nr12 & 8'hF8) != 0;
      s1_duration <= s1_length;
      s1_sweep_clock <= s1_sweep_period;
      s1_sweep_freq <= s1_freq;

      s1_env_volume <= (nr12 & 8'h08) ? s1_start_volume : 15 ^ s1_start_volume;
      s1_env_clock <= s1_env_period;

      s1_phase_clock <= 2047 ^ s1_freq;
      s1_phase <= 0;
    end

    if (s2_trigger_) begin
      uint8_t s2_length = 64 - (nr21 & 8'b00111111);
      uint8_t s2_start_volume = (nr22 & 8'b11110000) >> 4;
      uint8_t s2_env_period = (nr22 & 8'b00000111) >> 0;
      /* verilator lint_off WIDTH */
      uint16_t s2_freq = ((nr24 << 8) | nr23) & 16'h07FF;
      /* verilator lint_on WIDTH */

      s2_enable <= (nr22 & 8'hF8) != 0;
      s2_duration <= s2_length;

      s2_env_volume <= nr22[3] ? s2_start_volume : 15 ^ s2_start_volume;
      s2_env_clock <= s2_env_period;

      s2_phase_clock <= 2047 ^ s2_freq;
      s2_phase <= 0;
    end

    if (s3_trigger_) begin
      /* verilator lint_off WIDTH */
      uint16_t s3_length = 256 - nr31;
      uint16_t s3_freq = ((nr34 << 8) | nr33) & 16'h07FF;
      /* verilator lint_on WIDTH */

      s3_enable <= (nr32 != 0);
      s3_duration <= s3_length;
      s3_phase_clock <= 2047 ^ s3_freq;
      s3_phase <= 0;
    end

    if (s4_trigger_) begin
      uint8_t s4_length = 64 - (nr41 & 8'b00111111);
      uint8_t s4_start_volume = (nr42 & 8'b11110000) >> 4;
      uint8_t s4_env_period = (nr42 & 8'b00000111) >> 0;
      /* verilator lint_off WIDTH */
      uint8_t s4_phase_period = (nr43 & 8'b00000111) ? (nr43 & 8'b00000111) * 2 : 1;
      /* verilator lint_on WIDTH */

      s4_enable <= (nr42 & 8'hF8) != 0;
      s4_duration <= s4_length;

      s4_env_volume <= nr42[3] ? s4_start_volume : 15 ^ s4_start_volume;
      s4_env_clock <= s4_env_period;

      s4_phase_clock <= s4_phase_period;
      s4_lfsr <= 16'h7FFF;
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

