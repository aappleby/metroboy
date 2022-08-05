#include "GateBoyLib/GateBoyPair.h"

#include "GateBoyLib/GateBoyState.h"
#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/LogicBoy.h"
#include "GateBoyLib/Utils.h"


void print_field_at(int offset);

//---------------------------------------------------------------------------------------------------------------------

GateBoyPair::GateBoyPair() : gb(new GateBoy()), lb(new LogicBoy()) {}

GateBoyPair::GateBoyPair(GateBoy* gb1, LogicBoy* gb2) : gb(gb1), lb(gb2) {}

IGateBoy* GateBoyPair::get_a() { return gb; }
IGateBoy* GateBoyPair::get_b() { return lb; }

const char* GateBoyPair::get_id() const { return "GateBoyPair"; }


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

GBResult GateBoyPair::load_bootrom(const uint8_t* data, int size) {
  gb->load_bootrom(data, size);
  lb->load_bootrom(data, size);
  return GBResult::ok();
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

GBResult GateBoyPair::poweron(bool fastboot) {
  GBResult r1 = gb->poweron(fastboot);
  GBResult r2 = lb->poweron(fastboot);
  return check_results(r1, r2);
}

GBResult GateBoyPair::reset() {
  GBResult r1 = gb->reset();
  GBResult r2 = lb->reset();
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

GBResult GateBoyPair::run_to(const blob& cart_blob, int phase) {
  while(get_sys().gb_phase_total_old < phase) {
    next_phase(cart_blob);
  }
  return GBResult::ok();
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

const GateBoyCpu&   GateBoyPair::get_cpu() const    { return gb->get_cpu();    }
const GateBoyMem&   GateBoyPair::get_mem() const    { return gb->get_mem();    }
const GateBoyState& GateBoyPair::get_state() const  { return gb->get_state();  }
const GateBoySys&   GateBoyPair::get_sys() const    { return gb->get_sys();    }
const GateBoyPins&  GateBoyPair::get_pins() const   { return gb->get_pins();   }
const Probes&       GateBoyPair::get_probes() const { return gb->get_probes(); }

void GateBoyPair::get_flat_blob(const blob& cart_blob, int addr, int size, blob& out) const {
  return gb->get_flat_blob(cart_blob, addr, size, out);
}

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
  //printf("%d %d %d %d %d\n", sizeof(GateBoyCpu), sizeof(GateBoyMem), sizeof(GateBoyState), sizeof(GateBoySys), sizeof(GateBoyPins));

  /*
  if (!diff_blobs(&gb->get_cpu(), &lb->get_cpu(), sizeof(GateBoyCpu), 0xFF, nullptr)) {
    LOG_R("Regression test cpu mismatch @ phase old %lld!\n", gb->get_sys().gb_phase_total_old);
    return false;
  }
  */

  // probably don't need to diff mem, but eh - we're already slow in regression test mode
  /*
  if (!diff_blobs(&gb->get_mem(), &lb->get_mem(), sizeof(GateBoyMem), 0xFF, nullptr)) {
    LOG_R("Regression test mem mismatch @ phase old %lld!\n", gb->get_sys().gb_phase_total_old);
    return false;
  }
  */

  if (gb->get_state().diff(lb->get_state(), 0x01)) {
    LOG_R("Regression test state mismatch @ phase old %lld!\n", gb->get_sys().gb_phase_total_old);
    return false;
  }

  /*
  if (!diff_blobs(&gb->get_sys(), &lb->get_sys(), sizeof(GateBoySys), 0xFF, nullptr)) {
    LOG_R("Regression test cpu mismatch @ phase old %lld!\n", gb->get_sys().gb_phase_total_old);
    return false;
  }
  */

  /*
  if (!bit_cmp(gb->get_pins(), lb->get_pins(), 0x01)) {
    LOG_R("Regression test pins mismatch @ phase_old %lld!\n", gb->get_sys().gb_phase_total_old);
    return false;
  }
  */

  return true;
}

//-----------------------------------------------------------------------------