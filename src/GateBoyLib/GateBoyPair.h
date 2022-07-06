#pragma once
#include "GateBoyLib/IGateBoy.h"

struct GateBoy;
struct LogicBoy;

void print_field_at(int offset);

//---------------------------------------------------------------------------------------------------------------------

struct GateBoyPair : public IGateBoy {
  GateBoyPair();
  GateBoyPair(GateBoy* gb1, LogicBoy* gb2);

  IGateBoy* clone() const override;
  int size_bytes() override;
  GBResult get_flags() const override;
  GBResult load_raw_dump(BlobStream& dump_in) override;
  GBResult save_raw_dump(BlobStream& dump_out) const override;
  GBResult reset_to_poweron() override;
  GBResult reset_to_bootrom(const blob& cart_blob) override;
  GBResult reset_to_cart   (const blob& cart_blob) override;
  GBResult peek(int addr) const override;
  GBResult poke(int addr, uint8_t data_in) override;
  GBResult dbg_req(uint16_t addr, uint8_t data, bool write) override;
  GBResult dbg_read(const blob& cart_blob, int addr) override;
  GBResult dbg_write (const blob& cart_blob, int addr, uint8_t data_in) override;
  GBResult dbg_flip() override;

  GBResult run_phases(const blob& cart_blob, int phase_count) override;
  GBResult next_phase(const blob& cart_blob) override;
  GBResult run_to(const blob& cart_blob, int phase) override;

  GBResult set_buttons(uint8_t buttons) override;
  GBResult set_cpu_en(bool enabled) override;

  const GateBoyCpu&   get_cpu() const override;
  const GateBoyMem&   get_mem() const override;
  const GateBoyState& get_state() const override;
  const GateBoySys&   get_sys() const override;
  const GateBoyPins&  get_pins() const override;
  const Probes&       get_probes() const override;

  GateBoy* gb;
  LogicBoy* lb;
  bool select_ab = true;

  GBResult check_results(GBResult r1, GBResult r2) const;
  bool check_sync() const;
};

//---------------------------------------------------------------------------------------------------------------------
