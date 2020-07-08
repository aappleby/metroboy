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
  uint8_t* old_rom = cart.cart_rom;
  size_t old_rom_size = cart.cart_size;

  z80.reset(new_pc);
  cart.set_rom(old_rom, old_rom_size);
  cart.reset();
  ppu.reset(new_pc == 0);
  oam.reset();
  spu.reset();
  timer2.reset();
  vram.reset();
  joypad.reset();
  serial.reset();
  zram.reset();
  boot.reset(new_pc);
  lcd.reset();

  phase = 0;
  trace_val = 0;

  intf = 0xE1;
  imask = 0x00;

  ebus_req = {.addr = new_pc, .data = 0x00, .read = 1, .write = 0 };
  ebus_ack = {.addr = new_pc, .data = 0x00, .read = 1 };

  sentinel = 0xDEADBEEF;
}

//-----------------------------------------------------------------------------

void Gameboy::tock(int /*phase_*/, const Req& req) {
  if (PHASE_F && req.write) {
    if (req.addr == ADDR_IF) intf  = (uint8_t)req.data | 0b11100000;
    if (req.addr == ADDR_IE) imask = (uint8_t)req.data;
  }
}

void Gameboy::tick(const Req& req, Ack& ack) const {
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
  phase++;

  //if (z80.pc == 0xFF80) printf("running from hiram at phase %lld\n", phase);

  auto& self = *this;

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
    ppu.tick(ibus_req, ibus_ack);
    serial.tick(ibus_req, ibus_ack);
    joypad.tick(ibus_req, ibus_ack);
    zram.tick(ibus_req, ibus_ack);
    spu.tick(ibus_req, ibus_ack);
    boot.tick(ibus_req, ibus_ack);
    self.tick(ibus_req, ibus_ack);
    timer2.tick(ibus_req, ibus_ack);
    dma2.tick(ibus_req, ibus_ack);
    lcd.tick(ibus_req, ibus_ack);
  }

  ebus_ack = { 0 };
  vbus_ack = { 0 };
  obus_ack = { 0 };

  cart.tick(ebus_req, ebus_ack);
  vram.tick(vbus_req, vbus_ack);
  oam .tick(obus_req, obus_ack);
}

//-----------------------------------------------------------------------------

void Gameboy::tock_gb() {
  auto& self = *this;

  bool dma_src_vbus = dma2.DMA_RUN_READ && (dma2.addr >= ADDR_VRAM_BEGIN) && (dma2.addr <= ADDR_VRAM_END);
  bool dma_src_ebus = dma2.DMA_RUN_READ && !dma_src_vbus;

  if (dma_src_vbus) dma_data_latch = vbus_ack.data;
  if (dma_src_ebus) dma_data_latch = ebus_ack.data;

  if (ibus_ack.read > 1) __debugbreak();
  if (ebus_ack.read > 1) __debugbreak();
  if (vbus_ack.read > 1) __debugbreak();
  if (obus_ack.read > 1) __debugbreak();

  ppu.on_vbus_ack(vbus_ack);
  ppu.on_obus_ack(obus_ack);

  if (PHASE_A || PHASE_B) {
    bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
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
      cpu_ack.addr = cpu_req.addr;
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

  bool XONA_LCDC_EN = ppu.lcdc & FLAG_LCD_ON;

  if (PHASE_HI) {
    zram.  tock(ibus_req);
    spu.   tock(phase, ibus_req);
    ppu.   tock(phase, ibus_req);
    dma2.  tock(phase, ibus_req);
    cart.  tock(ebus_req);
    vram.  tock(vbus_req);
    oam.   tock(obus_req);
    lcd.tock(phase, ibus_req, XONA_LCDC_EN);

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

  //-----------------------------------
  // prioritize reqs

  if (PHASE_B) {
    ibus_req = {0};
    ebus_req = {0};
    vbus_req = {0};
    obus_req = {0};

    z80.get_bus_req(cpu_req);

    if (cpu_req.addr >= 0xFF00 && cpu_req.addr <= 0xFFFF) {
      ibus_req = cpu_req;
    }
    else if (cpu_req.addr >= 0xFE00 && cpu_req.addr <= 0xFEFF) {
      obus_req = cpu_req;
    }
    else if (cpu_req.addr >= 0x8000 && cpu_req.addr <= 0x9FFF) {
      vbus_req = cpu_req;
    }
    else {
      ebus_req = cpu_req;
    }
  }

  ppu.get_vbus_req(vbus_req);
  ppu.get_obus_req(obus_req);

  dma_src_vbus = dma2.DMA_RUN_READ && (dma2.addr >= ADDR_VRAM_BEGIN) && (dma2.addr <= ADDR_VRAM_END);
  dma_src_ebus = dma2.DMA_RUN_READ && !dma_src_vbus;

  if (dma_src_vbus) {
    vbus_req = {
      .addr = dma2.addr,
      .data = 0,
      .read = 1,
      .write = 0,
    };
  }

  if (dma_src_ebus) {
    ebus_req = {
      .addr = dma2.addr,
      .data = 0,
      .read = 1,
      .write = 0,
    };
  }

  if (PHASE_E && dma2.DMA_RUN_WRITE) {
    obus_req = {
      .addr = uint16_t(0xFE00 | (dma2.addr & 0xFF)),
      .data = dma_data_latch,
      .read = 0,
      .write = 1,
    };
  }
}

//-----------------------------------------------------------------------------

void Gameboy::dump_cpu(Dumper& d) {
  d("\002------------- CPU --------------\001\n");
  z80.dump(d, cpu_ack);
}

void Gameboy::dump_bus(Dumper& d) {
  d("\002------------- BUS --------------\001\n");

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

  d("phase %s\n", phases[phase & 7]);
  d("tphase %lld\n", phase);
  d("tcycle %lld\n", phase >> 1);
  d("mcycle %lld\n", phase >> 3);
  d("bgb cycle      0x%08x\n", ((phase / 2) * 8) + 0x00B2D5E6);
  d("imask  %s\n", byte_to_bits(imask));
  d("intf   %s\n", byte_to_bits(intf));
  d("boot   %d\n", boot.disable_bootrom);
  d("\n");

  d("---IBUS req:    "); dump_req(d, ibus_req);
  d("---IBUS ack:    "); dump_ack(d, ibus_ack);
  d("---EBUS req:    "); dump_req(d, ebus_req);
  d("---EBUS ack:    "); dump_ack(d, ebus_ack);
  d("---VBUS req:    "); dump_req(d, vbus_req);
  d("---VBUS ack:    "); dump_ack(d, vbus_ack);
  d("---OBUS req:    "); dump_req(d, obus_req);
  d("---OBUS ack:    "); dump_ack(d, obus_ack);
  d("---CPU  ack:    "); dump_ack(d, cpu_ack);
}

void Gameboy::dump_zram(Dumper& d) {
  zram.dump(d);
}

void Gameboy::dump_timer(Dumper& d) {
  timer2.dump(d);
}

void Gameboy::dump_cart(Dumper& d) {
  cart.dump(d);
}

void Gameboy::dump_oam(Dumper& d) {
  oam.dump(d);
}

void Gameboy::dump_joypad(Dumper& d) {
  joypad.dump(d);
}

void Gameboy::dump_serial(Dumper& d) {
  serial.dump(d);
}

void Gameboy::dump_disasm(Dumper& d) {
  d("\002--------------DISASM-----------\001\n");

  uint16_t pc = z80.get_op_addr();
  const uint8_t* segment;

  if (ADDR_MAIN_RAM_BEGIN <= pc && pc <= ADDR_MAIN_RAM_END) {
    segment = cart.get_main_ram() + (pc - ADDR_MAIN_RAM_BEGIN);
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
    d("\n");
  }
  else {
    d("(bad pc)\n");
  }
}

//-----------------------------------------------------------------------------