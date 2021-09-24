#pragma once

#include "GateBoyLib/IGateBoy.h"
#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "GateBoyLib/LogicBoyState.h"
#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

struct LogicBoy : public IGateBoy {
  virtual ~LogicBoy() {}

  IGateBoy* clone() const override {
    LogicBoy* result = new LogicBoy();
    result->lb_state = lb_state;
    result->gb_state = gb_state;
    result->cpu = cpu;
    result->mem = mem;
    result->sys = sys;
    result->pins = pins;
    result->probes = probes;
    return result;
  }

  int size_bytes() override { return sizeof(LogicBoy); }

  GBResult get_flags() const override {
    return GBResult(BIT_DATA);
  }

  GBResult load_raw_dump(BlobStream& bs) override        {
    bool load_ok = true;
    load_ok &= bs.read(gb_state);
    load_ok &= bs.read(cpu);
    load_ok &= bs.read(mem);
    load_ok &= bs.read(sys);
    load_ok &= bs.read(pins);
    load_ok &= bs.read(probes);
    lb_state.from_gb_state(gb_state, sys.gb_phase_total);
    return load_ok ? GBResult::ok() : Error::CORRUPT;
  }

  GBResult save_raw_dump(BlobStream& dump_out) const override { return Error::CORRUPT; }

  GBResult reset_to_poweron(const blob& cart_blob) override { return Error::CORRUPT; }
  GBResult run_poweron_reset(const blob& cart_blob, bool fastboot) override { return Error::CORRUPT; }
  GBResult reset_to_bootrom(const blob& cart_blob) override;
  GBResult reset_to_cart   (const blob& cart_blob) override;

  GBResult peek(int addr) const override;
  GBResult poke(int addr, uint8_t data_in) override;

  GBResult dbg_req  (uint16_t addr, uint8_t data, bool write) override;
  GBResult dbg_read (const blob& cart_blob, int addr) override;
  GBResult dbg_write(const blob& cart_blob, int addr, uint8_t data) override;

  GBResult run_phases(const blob& cart_blob, int phase_count) override;
  GBResult next_phase(const blob& cart_blob) override;

  GBResult set_buttons(uint8_t buttons) override { sys.buttons = buttons; return GBResult::ok(); }

  GBResult set_cpu_en(bool enabled) override {
    sys.cpu_en = enabled;
    return GBResult::ok();
  };

  const GateBoyCpu&   get_cpu() const override    { return *(GateBoyCpu*)&cpu; }
  const GateBoyMem&   get_mem() const override    { return mem; }
  const GateBoyState& get_state() const override  { lb_state.to_gb_state(const_cast<GateBoyState&>(gb_state), sys.gb_phase_total); return gb_state; }
  const GateBoySys&   get_sys() const override    { return *(GateBoySys*)&sys; }
  const GateBoyPins&  get_pins() const override   { return pins; }
  const Probes&       get_probes() const override { return probes; }

  //----------------------------------------

  void set_boot_bit(const blob& cart_blob) {
    dbg_write(cart_blob, 0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------

  void tock_cpu();
  void tock_logic(const blob& cart_blob);

  //-----------------------------------------------------------------------------

  void tock_spu_logic();
  void tock_serial_logic(bool cpu_wr_old, bool cpu_wr_new, uint16_t cpu_addr_old, uint16_t cpu_addr_new, uint16_t div_old, uint16_t div_new);

  //-----------------------------------------------------------------------------

  LogicBoyState lb_state;
  GateBoyState  gb_state;
  GateBoyCpu    cpu;
  GateBoyMem    mem;
  GateBoySys    sys;
  GateBoyPins   pins;
  Probes        probes;

  static FieldInfo fields[];
};

//-----------------------------------------------------------------------------
