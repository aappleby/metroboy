#pragma once

#include "GateBoyLib/IGateBoy.h"
#include "CoreLib/MetroBoyCPU.h"
#include "CoreLib/Constants.h"
#include "GateBoyLib/LogicBoyState.h"
#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct LogicBoyCpu {
  void reset_to_bootrom() {
    core.reset_to_bootrom();
    bus_req_new.addr = 0x0000;
    bus_req_new.data = 0;
    bus_req_new.read = 0;
    bus_req_new.write = 0;
    cpu_data_latch = 49;
    imask_latch = 0;
    intf_latch = 0;
    intf_latch_delay = 0;
    intf_halt_latch = 0;
  }

  void reset_to_cart() {
    core.reset_to_cart();
    bus_req_new.addr = 0xFF50;
    bus_req_new.data = 1;
    bus_req_new.read = 0;
    bus_req_new.write = 1;
    cpu_data_latch = 1;
    imask_latch = 0;
    intf_latch = 1;
    intf_latch_delay = 0;
    intf_halt_latch = 0;
  }

  MetroBoyCPU core;
  Req      bus_req_new = {0};
  uint8_t  cpu_data_latch = 0;
  uint8_t  imask_latch = 0;
  uint8_t  intf_latch = 0;
  uint8_t  intf_latch_delay = 0;
  uint8_t  intf_halt_latch = 0;
};
#pragma pack(pop)

static_assert(sizeof(GateBoyCpu) == sizeof(LogicBoyCpu));

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct LogicBoySys {
  void reset_to_bootrom() {
    rst = false;
    t1 = false;
    t2 = false;
    clk_en = true;
    clk_good = true;
    clk_req = true;
    cpu_en = true;
    fastboot = true;
    buttons = 0;
    phase_total = 80;
  }

  void reset_to_cart() {
    rst = false;
    t1 = false;
    t2 = false;
    clk_en = true;
    clk_good = true;
    clk_req = true;
    cpu_en = true;
    fastboot = true;
    buttons = 0;
    phase_total = 46880720;
  }


  uint8_t rst = 0;
  uint8_t t1 = 0;
  uint8_t t2 = 0;
  uint8_t clk_en = 0;
  uint8_t clk_good = 0;
  uint8_t clk_req = 0;
  uint8_t cpu_en = 0;
  uint8_t fastboot = 0;
  uint8_t buttons = 0;
  uint64_t phase_total = 0;
};
#pragma pack(pop)

static_assert(sizeof(GateBoySys) == sizeof(LogicBoySys));

//-----------------------------------------------------------------------------

struct LogicBoy : public IGateBoy {
  virtual ~LogicBoy() {}

  IGateBoy* clone() override {
    LogicBoy* result = new LogicBoy();
    result->lb_state = lb_state;
    result->cpu = cpu;
    result->mem = mem;
    result->sys = sys;
    result->probes = probes;
    return result;
  }

  int size_bytes() override { return sizeof(LogicBoy); }

  bool load_raw_dump(BlobStream& dump_in) override        { return false; }
  bool save_raw_dump(BlobStream& dump_out) const override { return false; }

  void reset_to_poweron(const blob& cart_blob) override { CHECK_P(false); }
  void run_poweron_reset(const blob& cart_blob, bool fastboot) override { CHECK_P(false); }
  void reset_to_bootrom(const blob& cart_blob) override;
  void reset_to_cart   (const blob& cart_blob) override;

  Result<uint8_t, Error> peek(const blob& cart_blob, int addr) const override;
  Result<uint8_t, Error> poke(blob& cart_blob, int addr, uint8_t data_in) override;

  Result<uint8_t, Error> dbg_read(const blob& cart_blob, int addr) override;
  Result<uint8_t, Error> dbg_write (const blob& cart_blob, int addr, uint8_t data) override;

  bool run_phases(const blob& cart_blob, int phase_count) override;
  bool next_phase(const blob& cart_blob) override;

  void set_buttons(uint8_t buttons) override { sys.buttons = buttons; }

  const GateBoyCpu&   get_cpu() const override    { return *(GateBoyCpu*)&cpu; }
  const GateBoyMem&   get_mem() const override    { return mem; }
  const GateBoyState& get_state() const override  { lb_state.to_gb_state(const_cast<GateBoyState&>(gb_state), sys.phase_total); return gb_state; }
  const GateBoySys&   get_sys() const override    { return *(GateBoySys*)&sys; }
  const Probes&       get_probes() const override { return probes; }

  //----------------------------------------

  /*
  void from_blob(const blob& b) {
    CHECK_P(b.size() >= sizeof(LogicBoy));
    memcpy(this, b.data(), sizeof(LogicBoy));
  }

  void to_blob(blob& b) {
    uint8_t* bytes = (uint8_t*)this;
    b.insert(b.end(), bytes, bytes + sizeof(*this));
  }
  */

  //----------------------------------------

  void set_boot_bit(const blob& cart_blob) {
    dbg_write(cart_blob, 0xFF50, 0xFF);
  }

  //-----------------------------------------------------------------------------


  void tock_cpu();
  void tock_logic(const blob& cart_blob, int64_t phase_total);
  void update_framebuffer();

  //void wipe() {
  //  lb_state.wipe();
  //}

  //-----------------------------------------------------------------------------

  void tock_spu_logic();
  void tock_serial_logic(bool cpu_wr_old, bool cpu_wr_new, uint16_t cpu_addr_old, uint16_t cpu_addr_new, uint16_t div_old, uint16_t div_new);

  //-----------------------------------------------------------------------------

  LogicBoyState lb_state;
  LogicBoyCpu cpu;
  GateBoyMem  mem;
  LogicBoySys sys;
  Probes      probes;

  GateBoyState gb_state;
};

//-----------------------------------------------------------------------------
