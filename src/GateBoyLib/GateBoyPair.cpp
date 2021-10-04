#include "GateBoyLib/GateBoyPair.h"

#include "GateBoyLib/GateBoyState.h"
#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/Utils.h"


void print_field_at(int offset);

//---------------------------------------------------------------------------------------------------------------------

GateBoyPair::GateBoyPair() : gb(new GateBoy()), lb(new LogicBoy()) {}

GateBoyPair::GateBoyPair(GateBoy* gb1, LogicBoy* gb2) : gb(gb1), lb(gb2) {}

IGateBoy* GateBoyPair::clone() const {
  return new GateBoyPair(gb->clone(), lb->clone());
}

int GateBoyPair::size_bytes() {
  return gb->size_bytes() + lb->size_bytes();
}

GBResult GateBoyPair::get_flags() const {
  GBResult r1 = gb->get_flags();
  GBResult r2 = lb->get_flags();
  if (r1.is_err()) { LOG_R("gb1 result.is_err()\n"); return r1; }
  if (r2.is_err()) { LOG_R("gb2 result.is_err()\n"); return r2; }
  return GBResult(r1.unwrap() & r2.unwrap());
}

GBResult GateBoyPair::load_raw_dump(BlobStream& dump_in) {
  GBResult r1 = gb->load_raw_dump(dump_in);
  GBResult r2 = lb->load_raw_dump(dump_in);
  return check_results(r1, r2);
}

GBResult GateBoyPair::save_raw_dump(BlobStream& dump_out) const {
  if (!check_sync()) return Error::MISMATCH;
  GBResult r1 = gb->save_raw_dump(dump_out);
  GBResult r2 = lb->save_raw_dump(dump_out);
  return check_results(r1, r2);
}

GBResult GateBoyPair::reset_to_poweron(const blob& cart_blob) {
  GBResult r1 = gb->reset_to_poweron(cart_blob);
  GBResult r2 = lb->reset_to_poweron(cart_blob);
  return check_results(r1, r2);
}

GBResult GateBoyPair::run_poweron_reset(const blob& cart_blob, bool fastboot) {
  GBResult r1 = gb->run_poweron_reset(cart_blob, fastboot);
  GBResult r2 = lb->run_poweron_reset(cart_blob, fastboot);
  return check_results(r1, r2);
}

GBResult GateBoyPair::reset_to_bootrom(const blob& cart_blob) {
  GBResult r1 = gb->reset_to_bootrom(cart_blob);
  GBResult r2 = lb->reset_to_bootrom(cart_blob);
  return check_results(r1, r2);
}

GBResult GateBoyPair::reset_to_cart   (const blob& cart_blob) {
  GBResult r1 = gb->reset_to_cart(cart_blob);
  GBResult r2 = lb->reset_to_cart(cart_blob);
  return check_results(r1, r2);
}

GBResult GateBoyPair::peek(int addr) const {
  GBResult r1 = gb->peek(addr);
  GBResult r2 = lb->peek(addr);
  return check_results(r1, r2);
}

GBResult GateBoyPair::poke(int addr, uint8_t data_in) {
  GBResult r1 = gb->poke(addr, data_in);
  GBResult r2 = lb->poke(addr, data_in);
  return check_results(r1, r2);
}

GBResult GateBoyPair::dbg_req(uint16_t addr, uint8_t data, bool write) {
  GBResult r1 = gb->dbg_req(addr, data, write);
  GBResult r2 = lb->dbg_req(addr, data, write);
  return check_results(r1, r2);
}

GBResult GateBoyPair::dbg_read(const blob& cart_blob, int addr) {
  GBResult r1 = gb->dbg_read(cart_blob, addr);
  GBResult r2 = lb->dbg_read(cart_blob, addr);
  return check_results(r1, r2);
}

GBResult GateBoyPair::dbg_write (const blob& cart_blob, int addr, uint8_t data_in) {
  GBResult r1 = gb->dbg_write(cart_blob, addr, data_in);
  GBResult r2 = lb->dbg_write(cart_blob, addr, data_in);
  return check_results(r1, r2);
}

GBResult GateBoyPair::dbg_flip() {
  select_ab = !select_ab;
  return GBResult::ok();
};

GBResult GateBoyPair::run_phases(const blob& cart_blob, int phase_count) {
  GBResult r1 = gb->run_phases(cart_blob, phase_count);
  GBResult r2 = lb->run_phases(cart_blob, phase_count);
  return check_results(r1, r2);
}

GBResult GateBoyPair::next_phase(const blob& cart_blob) {
  GBResult r1 = gb->next_phase(cart_blob);
  GBResult r2 = lb->next_phase(cart_blob);
  return check_results(r1, r2);
}

GBResult GateBoyPair::set_buttons(uint8_t buttons) {
  GBResult r1 = gb->set_buttons(buttons);
  GBResult r2 = lb->set_buttons(buttons);
  // if we check sync after buttons, we spam mismatch messages once a single one happens
  //return r1 == r2  && check_sync() ? r1 : Error::MISMATCH;
  return GBResult::ok();
}

GBResult GateBoyPair::set_cpu_en(bool enabled) {
  GBResult r1 = gb->set_cpu_en(enabled);
  GBResult r2 = lb->set_cpu_en(enabled);
  return check_results(r1, r2);
};

const GateBoyCpu&   GateBoyPair::get_cpu() const    { return select_ab ? gb->get_cpu()    : lb->get_cpu();    }
const GateBoyMem&   GateBoyPair::get_mem() const    { return select_ab ? gb->get_mem()    : lb->get_mem();    }
const GateBoyState& GateBoyPair::get_state() const  { return select_ab ? gb->get_state()  : lb->get_state();  }
const GateBoySys&   GateBoyPair::get_sys() const    { return select_ab ? gb->get_sys()    : lb->get_sys();    }
const GateBoyPins&  GateBoyPair::get_pins() const   { return select_ab ? gb->get_pins()   : lb->get_pins();   }
const Probes&       GateBoyPair::get_probes() const { return select_ab ? gb->get_probes() : lb->get_probes(); }

//-----------------------------------------------------------------------------

GBResult GateBoyPair::check_results(GBResult r1, GBResult r2) const {
  if (r1.is_err()) { LOG_R("gb1 result.is_err()\n"); return r1; }
  if (r2.is_err()) { LOG_R("gb2 result.is_err()\n"); return r2; }

  if (!(r1 == r2)) {
    LOG_R("gb1 result != gb2 result\n"); return Error::MISMATCH;
  }
  if (!check_sync()) return Error::MISMATCH;
  return r1;
}

//-----------------------------------------------------------------------------

bool GateBoyPair::check_sync() const {
  /*
  const auto& state1 = gb->get_state();
  const auto& state2 = lb->get_state();

  if (state1.diff(state2, 0x01)) {
    LOG_R("Regression test state mismatch @ phase %lld!\n", gb->get_sys().gb_phase_total);
    return false;
  }

  const auto& pins1 = gb->get_pins();
  const auto& pins2 = lb->get_pins();
    
  if (!bit_cmp(pins1, pins2, 0x01)) {
    LOG_R("Regression test pins mismatch @ phase %lld!\n", gb->get_sys().gb_phase_total);
    return false;
  }
  */

  return true;
}

//-----------------------------------------------------------------------------