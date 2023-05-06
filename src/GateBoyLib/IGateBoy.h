#pragma once
#include "metrolib/core/Blobs.h"
#include "metrolib/core/Result.h"

struct GateBoyCpu;
struct GateBoyMem;
struct GateBoyState;
struct GateBoySys;
struct GateBoyPins;
struct Probes;
struct BlobStream;

class IGateBoy {
public:
  virtual ~IGateBoy() {}

  virtual IGateBoy* get_a() = 0;
  virtual IGateBoy* get_b() = 0;

  virtual const char* get_id() const = 0;

  virtual GBResult poweron(bool fastboot) = 0;
  virtual GBResult reset() = 0;

  virtual IGateBoy* clone() const = 0;
  virtual int size_bytes() = 0;

  virtual GBResult get_flags() const = 0;

  virtual GBResult load_bootrom(const uint8_t* data, int size) = 0;
  virtual GBResult load_raw_dump(BlobStream& dump_in) = 0;
  virtual GBResult save_raw_dump(BlobStream& dump_out) const = 0;

  virtual GBResult peek(int addr) const = 0;
  virtual GBResult poke(int addr, uint8_t data_in) = 0;

  virtual GBResult dbg_req  (uint16_t addr, uint8_t data, bool write) = 0;
  virtual GBResult dbg_read (const blob& cart_blob, int addr) = 0;
  virtual GBResult dbg_write(const blob& cart_blob, int addr, uint8_t data) = 0;

  virtual GBResult run_phases(const blob& cart_blob, int phase_count) = 0;
  virtual GBResult next_phase(const blob& cart_blob) = 0;
  virtual GBResult run_to(const blob& cart_blob, int phase) = 0;

  virtual GBResult set_buttons(uint8_t buttons) = 0;
  virtual GBResult set_cpu_en(bool enabled) { return GBResult::ok(); };

  virtual const GateBoyCpu&   get_cpu() const = 0;
  virtual const GateBoyMem&   get_mem() const = 0;
  virtual const GateBoyState& get_state() const = 0;
  virtual const GateBoySys&   get_sys() const = 0;
  virtual const GateBoyPins&  get_pins() const = 0;
  virtual const Probes&       get_probes() const = 0;

  virtual void get_flat_blob(const blob& cart_blob, int addr, int size, uint8_t* out) const = 0;
};
