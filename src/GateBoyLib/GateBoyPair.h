#pragma once
#include "GateBoyLib/GateBoy.h"

void print_field_at(int offset);

//---------------------------------------------------------------------------------------------------------------------

struct GateBoyPair : public IGateBoy {
  GateBoyPair(IGateBoy* gb1, IGateBoy* gb2) : gb1(gb1), gb2(gb2) {}

  IGateBoy* clone() const override {
    auto c1 = gb1->clone();
    auto c2 = gb2->clone();
    return new GateBoyPair(c1, c2);
  }

  int size_bytes() override {
    return gb1->size_bytes() + gb2->size_bytes();
  }

  GBResult get_flags() const override {
    GBResult r1 = gb1->get_flags();
    GBResult r2 = gb2->get_flags();
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult load_raw_dump(BlobStream& dump_in) override {
    auto old_cursor = dump_in.cursor;
    GBResult r1 = gb1->load_raw_dump(dump_in);
    dump_in.cursor = old_cursor;
    GBResult r2 = gb2->load_raw_dump(dump_in);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult save_raw_dump(BlobStream& dump_out) const override {
    if (!check_sync()) return Error::MISMATCH;
    return gb1->save_raw_dump(dump_out);
  }

  GBResult reset_to_poweron(const blob& cart_blob) override {
    GBResult r1 = gb1->reset_to_poweron(cart_blob);
    GBResult r2 = gb2->reset_to_poweron(cart_blob);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult run_poweron_reset(const blob& cart_blob, bool fastboot) override {
    GBResult r1 = gb1->run_poweron_reset(cart_blob, fastboot);
    GBResult r2 = gb2->run_poweron_reset(cart_blob, fastboot);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult reset_to_bootrom(const blob& cart_blob) override {
    GBResult r1 = gb1->reset_to_bootrom(cart_blob);
    GBResult r2 = gb2->reset_to_bootrom(cart_blob);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult reset_to_cart   (const blob& cart_blob) override {
    GBResult r1 = gb1->reset_to_cart(cart_blob);
    GBResult r2 = gb2->reset_to_cart(cart_blob);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult peek(int addr) const override {
    GBResult r1 = gb1->peek(addr);
    GBResult r2 = gb2->peek(addr);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult poke(int addr, uint8_t data_in) override {
    GBResult r1 = gb1->poke(addr, data_in);
    GBResult r2 = gb2->poke(addr, data_in);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult dbg_read(const blob& cart_blob, int addr) override {
    GBResult r1 = gb1->dbg_read(cart_blob, addr);
    GBResult r2 = gb2->dbg_read(cart_blob, addr);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult dbg_write (const blob& cart_blob, int addr, uint8_t data_in) override {
    GBResult r1 = gb1->dbg_write(cart_blob, addr, data_in);
    GBResult r2 = gb2->dbg_write(cart_blob, addr, data_in);
    //return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;

    auto res = r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
    //if (res.is_err()) debugbreak();
    return res;
  }

  GBResult run_phases(const blob& cart_blob, int phase_count) override {
    GBResult r1 = gb1->run_phases(cart_blob, phase_count);
    GBResult r2 = gb2->run_phases(cart_blob, phase_count);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult next_phase(const blob& cart_blob) override {
    GBResult r1 = gb1->next_phase(cart_blob);
    GBResult r2 = gb2->next_phase(cart_blob);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  GBResult set_buttons(uint8_t buttons) override {
    GBResult r1 = gb1->set_buttons(buttons);
    GBResult r2 = gb2->set_buttons(buttons);
    return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  }

  const GateBoyCpu&   get_cpu() const override    { return gb1->get_cpu(); }
  const GateBoyMem&   get_mem() const override    { return gb1->get_mem(); }
  const GateBoyState& get_state() const override  { return gb1->get_state(); }
  const GateBoySys&   get_sys() const override    { return gb1->get_sys(); }
  const GateBoyPins&  get_pins() const override   { return gb1->get_pins(); }
  const Probes&       get_probes() const override { return gb1->get_probes(); }

  IGateBoy* gb1;
  IGateBoy* gb2;

  bool check_sync() const {
    const auto& state1 = gb1->get_state();
    const auto& state2 = gb2->get_state();

    if (state1.diff(state2, 0x01)) {
      LOG_R("Regression test mismatch @ phase %lld!\n", gb1->get_sys().phase_total);
      //__debugbreak();
      return false;
    }
    return true;
  }
};

//---------------------------------------------------------------------------------------------------------------------
