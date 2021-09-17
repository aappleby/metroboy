#pragma once
#include "CoreLib/Types.h"

struct GateBoyCpu;
struct GateBoyMem;
struct GateBoyState;
struct GateBoySys;
struct Probes;

class IGateBoy {
public:
  virtual ~IGateBoy() {}

  virtual IGateBoy* clone() const = 0;
  virtual int size_bytes() = 0;

  virtual uint8_t get_flags() const = 0;

  virtual bool load_raw_dump(BlobStream& dump_in) = 0;
  virtual bool save_raw_dump(BlobStream& dump_out) const = 0;

  virtual void reset_to_poweron(const blob& cart_blob) = 0;
  virtual void run_poweron_reset(const blob& cart_blob, bool fastboot) = 0;
  virtual void reset_to_bootrom(const blob& cart_blob) = 0;
  virtual void reset_to_cart   (const blob& cart_blob) = 0;

  virtual Result<uint8_t, Error> peek(int addr) const = 0;
  virtual Result<uint8_t, Error> poke(int addr, uint8_t data_in) = 0;

  virtual Result<uint8_t, Error> dbg_read(const blob& cart_blob, int addr) = 0;
  virtual Result<uint8_t, Error> dbg_write (const blob& cart_blob, int addr, uint8_t data) = 0;

  virtual bool run_phases(const blob& cart_blob, int phase_count) = 0;
  virtual bool next_phase(const blob& cart_blob) = 0;

  virtual void set_buttons(uint8_t buttons) = 0;

  virtual const GateBoyCpu&   get_cpu() const = 0;
  virtual const GateBoyMem&   get_mem() const = 0;
  virtual const GateBoyState& get_state() const = 0;
  virtual const GateBoySys&   get_sys() const = 0;
  virtual const Probes&       get_probes() const = 0;
};