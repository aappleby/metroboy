#pragma once
#include "CoreLib/Types.h"

class IGateBoy {
public:
  virtual ~IGateBoy() {}

  virtual void reset_to_bootrom(const blob& cart_blob, bool fastboot) = 0;
  virtual void reset_to_cart   (const blob& cart_blob) = 0;

  virtual Result<uint8_t, Error> peek(const blob& cart_blob, int addr) const = 0;
  virtual Result<uint8_t, Error> poke(blob& cart_blob, int addr, uint8_t data_in) = 0;

  virtual Result<uint8_t, Error> dbg_read(const blob& cart_blob, int addr) = 0;
  virtual Result<uint8_t, Error> dbg_write (const blob& cart_blob, int addr, uint8_t data) = 0;

  virtual void run_phases(const blob& cart_blob, int phase_count) = 0;
  virtual bool next_phase(const blob& cart_blob) = 0;

  virtual void set_buttons(uint8_t buttons) = 0;

  virtual const uint8_t* get_framebuffer() const = 0;
  virtual const uint8_t* get_vram() const = 0;
  virtual void get_state(blob& state_out) const = 0;

  virtual void load_raw_dump(const blob& dump_in) = 0;
  virtual void save_raw_dump(blob& dump_out) const = 0;

  virtual const FieldInfo* get_field_info() const = 0;
};