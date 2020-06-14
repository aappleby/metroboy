#include "Gameboy.h"

#include "Assembler.h"
#include "Constants.h"

#include <assert.h>
#include <imgui.h>

#pragma warning(disable:4189)
#pragma warning(disable:4244)

//-----------------------------------------------------------------------------

void Gameboy::set_rom(uint8_t* new_rom, size_t new_rom_size) {
  cart.set_rom(new_rom, new_rom_size);

  for (int i = 0; i < 160*144; i++) fb[i] = rand() & 3;
}

void Gameboy::reset(uint16_t new_pc) {
  uint8_t* old_rom = cart.rom;
  size_t old_rom_size = cart.rom_size;

  z80.reset(new_pc);
  cart.set_rom(old_rom, old_rom_size);
  cart.reset();
  ppu.reset(new_pc == 0);
  oam.reset();
  spu.reset();
  timer2.reset();
  vram.reset();
  iram.reset();
  joypad.reset();
  serial.reset();
  zram.reset();
  boot.reset(new_pc);

  phase = 0;
  trace_val = 0;

  intf = 0xE1;
  imask = 0x00;

  ebus_req = {.addr = new_pc, .data = 0x00, .read = 1, .write = 0 };
  ebus_ack = {.addr = new_pc, .data = 0x00, .read = 1 };

  sentinel = 0xDEADBEEF;
}

//-----------------------------------------------------------------------------

void Gameboy::tock(int phase_, const Req& req) {
  if (PHASE_F && req.write) {
    if (req.addr == ADDR_IF) intf  = (uint8_t)req.data | 0b11100000;
    if (req.addr == ADDR_IE) imask = (uint8_t)req.data;
  }
}

void Gameboy::tick(int phase_, const Req& req, Ack& ack) const {
  if (req.read && ((req.addr == ADDR_IF) || (req.addr == ADDR_IE))) {
    uint8_t data = 0;
    if (req.addr == ADDR_IF) data = 0b11100000 | intf;
    if (req.addr == ADDR_IE) data = imask;

    ack.addr = req.addr;
    ack.data = data;
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void Gameboy::tick_gb() {
  auto& self = *this;

  int64_t tphase2 = (phase & 7);

  //-----------------------------------
  // interrupts are partially asynchronous

  bool timer_int = timer2.get_interrupt();

  intf &= ~z80.get_int_ack();
  if (imask & 0x01) z80.unhalt |= ppu.vblank1;
  if (imask & 0x02) z80.unhalt |= ppu.stat2;
  if (imask & 0x04) z80.unhalt |= timer_int;
  if (imask & 0x10) z80.unhalt |= joypad.get() != 0xFF;

  if (ppu.vblank1)          intf |= INT_VBLANK_MASK;
  if (ppu.stat1)            intf |= INT_STAT_MASK;
  if (timer_int)            intf |= INT_TIMER_MASK;
  if (joypad.get() != 0xFF) intf |= INT_JOYPAD_MASK;

  //-----------------------------------
  // gather acks

  if (ibus_req.read) {
    ibus_ack = { 0 };
    ppu.tick(phase, ibus_req, ibus_ack);
    serial.tick(phase, ibus_req, ibus_ack);
    joypad.tick(phase, ibus_req, ibus_ack);
    zram.tick(phase, ibus_req, ibus_ack);
    spu.tick(phase, ibus_req, ibus_ack);
    boot.tick(phase, ibus_req, ibus_ack);
    self.tick(phase, ibus_req, ibus_ack);
    timer2.tick(phase, ibus_req, ibus_ack);
  }

  {
    ebus_ack = { 0 };
    cart.tick(phase, ebus_req, ebus_ack);
    iram.tick(phase, ebus_req, ebus_ack);
  }

  {
    vbus_ack = { 0 };
    vram.tick(phase, vbus_req, vbus_ack);
    ppu.on_vbus_ack(vbus_ack);
  }

  {
    obus_ack = { 0 };
    oam.tick(phase, obus_req, obus_ack);
    ppu.on_obus_ack(obus_ack);
  }

  //-----------------------------------
  // prioritize reqs

  if (PHASE_C) {
    z80.get_bus_req(ibus_req);
  }

  bool cpu_has_ibus_req = ibus_req.addr >= ADDR_IOBUS_BEGIN;
  bool cpu_has_vbus_req = ibus_req.addr >= ADDR_VRAM_BEGIN && ibus_req.addr <= ADDR_VRAM_END;
  bool cpu_has_obus_req = ibus_req.addr >= ADDR_OAM_BEGIN && ibus_req.addr <= ADDR_OAM_END;
  bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

  ebus_req = ibus_req;

  vbus_req = ibus_req;
  ppu.get_vbus_req(vbus_req);

  obus_req = ibus_req;
  ppu.get_obus_req(obus_req);

  bool dma_src_vram = (dma2.addr >= ADDR_VRAM_BEGIN) && (dma2.addr <= ADDR_VRAM_END);

  if (dma2.DMA_RUN_READ && dma_src_vram) {
    vbus_req = {
      .addr = dma2.addr,
      .data = 0,
      .read = 1,
      .write = 0,
    };
  }

  if (dma2.DMA_RUN_READ && !dma_src_vram) {
    ebus_req = {
      .addr = dma2.addr,
      .data = 0,
      .read = 1,
      .write = 0,
    };
  }

  if (dma2.DMA_RUN_WRITE) {
    uint8_t dma_src_data = dma_src_vram ? vbus_ack.data : ebus_ack.data;
    obus_req = {
      .addr = uint16_t(0xFE00 | (dma2.addr & 0xFF)),
      .data = 0,
      .read = 0,
      .write = 1,
    };
  }

  /*
  if (ibus_ack.read > 1) __debugbreak();
  if (ebus_ack.read > 1) __debugbreak();
  if (vbus_ack.read > 1) __debugbreak();
  if (obus_ack.read > 1) __debugbreak();
  */
}

//-----------------------------------------------------------------------------

void Gameboy::tock_gb() {
  auto& self = *this;

  phase++;

  if (PHASE_A || PHASE_B) {
    bool cpu_has_ibus_req = ibus_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_vbus_req = ibus_req.addr >= ADDR_VRAM_BEGIN && ibus_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = ibus_req.addr >= ADDR_OAM_BEGIN  && ibus_req.addr <= ADDR_OAM_END;
    bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

    if (cpu_has_ibus_req) {
      cpu_ack = ibus_ack;
    }
    else if (cpu_has_ebus_req) {
      cpu_ack = ebus_ack;
    }
    else if (cpu_has_vbus_req) {
      cpu_ack = vbus_ack;
    }
    else if (cpu_has_obus_req) {
      cpu_ack = obus_ack;
    }
    else {
      cpu_ack = { 0 };
    }

    if (cpu_ack.read == 0) {
      cpu_ack.data = 0xFF;
    }
  }

  if (PHASE_A) z80.tock_a(imask, intf, cpu_ack);
  if (PHASE_B) z80.tock_b(imask, intf, cpu_ack);
  //if (PHASE_C) z80.tock_c(imask, intf, cpu_ack);
  //if (PHASE_D) z80.tock_d(imask, intf, cpu_ack);
  //if (PHASE_E) z80.tock_e(imask, intf, cpu_ack);
  //if (PHASE_F) z80.tock_f(imask, intf, cpu_ack);
  //if (PHASE_G) z80.tock_g(imask, intf, cpu_ack);
  //if (PHASE_H) z80.tock_h(imask, intf, cpu_ack);

  timer2.tock(phase, ibus_req);

  if (PHASE_F) {
    self.  tock(phase, ibus_req);
    serial.tock(phase, ibus_req);
    joypad.tock(phase, ibus_req);
    boot.  tock(phase, ibus_req);
  }

  if (PHASE_HI) {
    zram.  tock(phase, ibus_req);
    spu.   tock(phase, ibus_req);
    ppu.   tock(phase, ibus_req);
    dma2.  tock(phase, ibus_req);
    cart.  tock(phase, ebus_req);
    iram.  tock(phase, ebus_req);
    vram.  tock(phase, vbus_req);


    oam.   tock(phase, obus_req);

    //----------

    gb_to_host.x       = ppu.pix_count;
    gb_to_host.y       = ppu.line;
    gb_to_host.counter = ppu.counter;
    gb_to_host.pix     = ppu.pix_out;
    gb_to_host.pix_oe  = ppu.pix_oe;
    gb_to_host.out_r   = spu.get_r();
    gb_to_host.out_l   = spu.get_l();
    gb_to_host.trace   = ebus_req.addr;

    int pix_x = ppu.pix_count;
    int pix_y = ppu.line;

    if (pix_x >= 0 && pix_x < 160 && pix_y >= 0 && pix_y < 144) {
      fb[gb_to_host.x + gb_to_host.y * 160] = ppu.pix_out;
    }
  }
}

//-----------------------------------------------------------------------------

void Gameboy::dump_cpu(std::string& d) {
  sprintf(d, "\002------------- CPU --------------\001\n");
  z80.dump(d, cpu_ack);
}

void Gameboy::dump_bus(std::string& d) {
  sprintf(d, "\002------------- BUS --------------\001\n");

  const char* phases[] = {
    "\002A_______\001",
    "\003_B______\001",
    "\002__C_____\001",
    "\003___D____\001",
    "\002____E___\001",
    "\003_____F__\001",
    "\002______G_\001",
    "\003_______H\001",
  };

  sprintf(d, "phase %s\n", phases[phase & 7]);
  sprintf(d, "tphase %lld\n", phase);
  sprintf(d, "tcycle %lld\n", phase >> 1);
  sprintf(d, "mcycle %lld\n", phase >> 3);
  sprintf(d, "bgb cycle      0x%08x\n", ((phase / 2) * 8) + 0x00B2D5E6);
  sprintf(d, "imask  %s\n", byte_to_bits(imask));
  sprintf(d, "intf   %s\n", byte_to_bits(intf));
  sprintf(d, "boot   %d\n", boot.disable_bootrom);
  sprintf(d, "\n");

  sprintf(d, "---IBUS req:    "); print_req(d, ibus_req);
  sprintf(d, "---IBUS ack:    "); print_ack(d, ibus_ack);
  sprintf(d, "---EBUS req:    "); print_req(d, ebus_req);
  sprintf(d, "---EBUS ack:    "); print_ack(d, ebus_ack);
  sprintf(d, "---VBUS req:    "); print_req(d, vbus_req);
  sprintf(d, "---VBUS ack:    "); print_ack(d, vbus_ack);
  sprintf(d, "---OBUS req:    "); print_req(d, obus_req);
  sprintf(d, "---OBUS ack:    "); print_ack(d, obus_ack);
  sprintf(d, "---CPU  ack:    "); print_ack(d, cpu_ack);
}

void Gameboy::dump_zram(std::string& d) {
  zram.dump(d);
}

void Gameboy::dump_timer(std::string& d) {
  timer2.dump(d);
}

void Gameboy::dump_cart(std::string& d) {
  cart.dump(d);
}

void Gameboy::dump_oam(std::string& d) {
  oam.dump(d);
}

void Gameboy::dump_joypad(std::string& d) {
  joypad.dump(d);
}

void Gameboy::dump_serial(std::string& d) {
  serial.dump(d);
}

void Gameboy::dump_disasm(std::string& d) {
  sprintf(d, "\002--------------DISASM-----------\001\n");

  uint16_t pc = z80.get_op_addr();
  const uint8_t* segment;

  if (ADDR_IRAM_BEGIN <= pc && pc <= ADDR_IRAM_END) {
    segment = iram.get() + (pc - ADDR_IRAM_BEGIN);
  }
  else if (ADDR_ZEROPAGE_BEGIN <= pc && pc <= ADDR_ZEROPAGE_END) {
    segment = zram.get() + (pc - ADDR_ZEROPAGE_BEGIN);
  }
  else if (ADDR_OAM_BEGIN <= pc && pc <= ADDR_OAM_END) {
    segment = oam.get() + (pc - ADDR_OAM_BEGIN);
  }
  else {
    segment = cart.get_flat_ptr(pc);
  }

  if (segment) {
    Assembler a;
    a.disassemble(segment, 65536, pc, 30, d, false);
    sprintf(d, "\n");
  }
  else {
    sprintf(d, "(bad pc)\n");
  }
}

//-----------------------------------------------------------------------------