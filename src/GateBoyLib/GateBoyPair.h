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
    if (r1.is_err()) { LOG_R("gb1 result.is_err()\n"); return r1; }
    if (r2.is_err()) { LOG_R("gb2 result.is_err()\n"); return r2; }
    return GBResult(r1.unwrap() & r2.unwrap());
  }

  GBResult load_raw_dump(BlobStream& dump_in) override {
    auto old_cursor = dump_in.cursor;
    GBResult r1 = gb1->load_raw_dump(dump_in);
    dump_in.cursor = old_cursor;
    GBResult r2 = gb2->load_raw_dump(dump_in);
    return check_results(r1, r2);
  }

  GBResult save_raw_dump(BlobStream& dump_out) const override {
    if (!check_sync()) return Error::MISMATCH;
    return gb1->save_raw_dump(dump_out);
  }

  GBResult reset_to_poweron(const blob& cart_blob) override {
    GBResult r1 = gb1->reset_to_poweron(cart_blob);
    GBResult r2 = gb2->reset_to_poweron(cart_blob);
    return check_results(r1, r2);
  }

  GBResult run_poweron_reset(const blob& cart_blob, bool fastboot) override {
    GBResult r1 = gb1->run_poweron_reset(cart_blob, fastboot);
    GBResult r2 = gb2->run_poweron_reset(cart_blob, fastboot);
    return check_results(r1, r2);
  }

  GBResult reset_to_bootrom(const blob& cart_blob) override {
    GBResult r1 = gb1->reset_to_bootrom(cart_blob);
    GBResult r2 = gb2->reset_to_bootrom(cart_blob);
    return check_results(r1, r2);
  }

  GBResult reset_to_cart   (const blob& cart_blob) override {
    GBResult r1 = gb1->reset_to_cart(cart_blob);
    GBResult r2 = gb2->reset_to_cart(cart_blob);
    return check_results(r1, r2);
  }

  GBResult peek(int addr) const override {
    GBResult r1 = gb1->peek(addr);
    GBResult r2 = gb2->peek(addr);
    return check_results(r1, r2);
  }

  GBResult poke(int addr, uint8_t data_in) override {
    GBResult r1 = gb1->poke(addr, data_in);
    GBResult r2 = gb2->poke(addr, data_in);
    return check_results(r1, r2);
  }

  GBResult dbg_req(uint16_t addr, uint8_t data, bool write) override {
    GBResult r1 = gb1->dbg_req(addr, data, write);
    GBResult r2 = gb2->dbg_req(addr, data, write);
    return check_results(r1, r2);
  }

  GBResult dbg_read(const blob& cart_blob, int addr) override {
    GBResult r1 = gb1->dbg_read(cart_blob, addr);
    GBResult r2 = gb2->dbg_read(cart_blob, addr);
    return check_results(r1, r2);
  }

  GBResult dbg_write (const blob& cart_blob, int addr, uint8_t data_in) override {
    GBResult r1 = gb1->dbg_write(cart_blob, addr, data_in);
    GBResult r2 = gb2->dbg_write(cart_blob, addr, data_in);
    return check_results(r1, r2);
  }

  GBResult dbg_flip() {
    select_ab = !select_ab;
    return GBResult::ok();
  };

  GBResult run_phases(const blob& cart_blob, int phase_count) override {
    GBResult r1 = gb1->run_phases(cart_blob, phase_count);
    GBResult r2 = gb2->run_phases(cart_blob, phase_count);
    return check_results(r1, r2);
  }

  GBResult next_phase(const blob& cart_blob) override {
    GBResult r1 = gb1->next_phase(cart_blob);
    GBResult r2 = gb2->next_phase(cart_blob);
    return check_results(r1, r2);
  }

  GBResult set_buttons(uint8_t buttons) override {
    GBResult r1 = gb1->set_buttons(buttons);
    GBResult r2 = gb2->set_buttons(buttons);
    // if we check sync after buttons, we spam mismatch messages once a single one happens
    //return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
    return GBResult::ok();
  }

  GBResult set_cpu_en(bool enabled) override {
    GBResult r1 = gb1->set_cpu_en(enabled);
    GBResult r2 = gb2->set_cpu_en(enabled);
    return check_results(r1, r2);
  };

  const GateBoyCpu&   get_cpu() const override    { return select_ab ? gb1->get_cpu()    : gb2->get_cpu();    }
  const GateBoyMem&   get_mem() const override    { return select_ab ? gb1->get_mem()    : gb2->get_mem();    }
  const GateBoyState& get_state() const override  { return select_ab ? gb1->get_state()  : gb2->get_state();  }
  const GateBoySys&   get_sys() const override    { return select_ab ? gb1->get_sys()    : gb2->get_sys();    }
  const GateBoyPins&  get_pins() const override   { return select_ab ? gb1->get_pins()   : gb2->get_pins();   }
  const Probes&       get_probes() const override { return select_ab ? gb1->get_probes() : gb2->get_probes(); }

  IGateBoy* gb1;
  IGateBoy* gb2;
  bool select_ab = true;

  GBResult check_results(GBResult r1, GBResult r2) const {
    if (r1.is_err()) { LOG_R("gb1 result.is_err()\n"); return r1; }
    if (r2.is_err()) { LOG_R("gb2 result.is_err()\n"); return r2; }

    if (!(r1 == r2)) {
      LOG_R("gb1 result != gb2 result\n"); return Error::MISMATCH;
    }
    if (!check_sync()) return Error::MISMATCH;
    return r1;
  }

  bool check_sync() const {
    const auto& state1 = gb1->get_state();
    const auto& state2 = gb2->get_state();

    if (state1.diff(state2, 0x01)) {
      LOG_R("Regression test state mismatch @ phase %lld!\n", gb1->get_sys().gb_phase_total);
      //__debugbreak();
      return false;
    }

    const auto& pins1 = gb1->get_pins();
    const auto& pins2 = gb2->get_pins();
    
    //if (memcmp(&pins1, &pins2, sizeof(GateBoyPins))) {
    if (!bit_cmp(pins1, pins2, 0x01)) {
      LOG_R("Regression test pins mismatch @ phase %lld!\n", gb1->get_sys().gb_phase_total);
      return false;
    }

    return true;
  }
};

//---------------------------------------------------------------------------------------------------------------------
