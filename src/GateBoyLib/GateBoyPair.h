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

  uint8_t get_flags() const override {
    return gb1->get_flags() & gb2->get_flags();
  }

  bool load_raw_dump(BlobStream& dump_in) override {
    auto old_cursor = dump_in.cursor;
    bool result = true;
    result &= gb1->load_raw_dump(dump_in);
    dump_in.cursor = old_cursor;
    result &= gb2->load_raw_dump(dump_in);
    return result & check_sync();
  }

  bool save_raw_dump(BlobStream& dump_out) const override {
    check_sync();
    return gb1->save_raw_dump(dump_out);
  }

  void reset_to_poweron(const blob& cart_blob) override {
    gb1->reset_to_poweron(cart_blob);
    gb2->reset_to_poweron(cart_blob);
    check_sync();
  }

  void run_poweron_reset(const blob& cart_blob, bool fastboot) override {
    gb1->run_poweron_reset(cart_blob, fastboot);
    gb2->run_poweron_reset(cart_blob, fastboot);
    check_sync();
  }

  void reset_to_bootrom(const blob& cart_blob) override {
    gb1->reset_to_bootrom(cart_blob);
    gb2->reset_to_bootrom(cart_blob);
    check_sync();
  }

  void reset_to_cart   (const blob& cart_blob) override {
    gb1->reset_to_cart(cart_blob);
    gb2->reset_to_cart(cart_blob);
    check_sync();
  }

  Result<uint8_t, Error> peek(int addr) const override {
    auto result1 = gb1->peek(addr);
    auto result2 = gb2->peek(addr);
    CHECK_P(result1 == result2);
    return result1.unwrap() && check_sync();
  }

  Result<uint8_t, Error> poke(int addr, uint8_t data_in) override {
    auto result1 = gb1->poke(addr, data_in);
    auto result2 = gb2->poke(addr, data_in);
    CHECK_P(result1 == result2);
    return result1.unwrap() && check_sync();
  }

  Result<uint8_t, Error> dbg_read(const blob& cart_blob, int addr) override {
    auto result1 = gb1->dbg_read(cart_blob, addr);
    auto result2 = gb2->dbg_read(cart_blob, addr);
    CHECK_P(result1 == result2);
    return result1.unwrap() && check_sync();
  }

  Result<uint8_t, Error> dbg_write (const blob& cart_blob, int addr, uint8_t data_in) override {
    auto result1 = gb1->dbg_write(cart_blob, addr, data_in);
    auto result2 = gb2->dbg_write(cart_blob, addr, data_in);
    CHECK_P(result1 == result2);
    return result1.unwrap() && check_sync();
  }

  bool run_phases(const blob& cart_blob, int phase_count) override {
    bool result = true;
    result &= gb1->run_phases(cart_blob, phase_count);
    result &= gb2->run_phases(cart_blob, phase_count);
    result &= check_sync();
    return result;
  }

  bool next_phase(const blob& cart_blob) override {
    bool result = true;
    result &= gb1->next_phase(cart_blob);
    result &= gb2->next_phase(cart_blob);
    result &= check_sync();
    return result;
  }

  void set_buttons(uint8_t buttons) override {
    gb1->set_buttons(buttons);
    gb2->set_buttons(buttons);
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
