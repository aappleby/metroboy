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

  cpu_req  = {.addr = new_pc, .data = 0x00, .read = 1, .write = 0 };
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

  ibus_ack = {0};
  ebus_ack = {0};
  vbus_ack = {0};
  obus_ack = {0};

  ppu.   tick(phase, ibus_req, ibus_ack);
  serial.tick(phase, ibus_req, ibus_ack);
  joypad.tick(phase, ibus_req, ibus_ack);
  zram.  tick(phase, ibus_req, ibus_ack);
  spu.   tick(phase, ibus_req, ibus_ack);
  boot.  tick(phase, ibus_req, ibus_ack);
  self.  tick(phase, ibus_req, ibus_ack);
  dma1.  tick(phase, ibus_req, ibus_ack);
  timer2.tick(phase, ibus_req, ibus_ack);
  cart.  tick(phase, ebus_req, ebus_ack);
  iram.  tick(phase, ebus_req, ebus_ack);
  vram.  tick(phase, vbus_req, vbus_ack);
  oam.   tick(phase, obus_req, obus_ack);

  /*
  if (ibus_ack.read  > 1) __debugbreak();
  if (ibus_ack.write > 1) __debugbreak();
  if (ebus_ack.read  > 1) __debugbreak();
  if (ebus_ack.write > 1) __debugbreak();
  if (vbus_ack.read  > 1) __debugbreak();
  if (vbus_ack.write > 1) __debugbreak();
  if (obus_ack.read  > 1) __debugbreak();
  if (obus_ack.write > 1) __debugbreak();
  */

  bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
  bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
  bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
  bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

  cpu_ack = {};

  if (cpu_has_ibus_req) {
    cpu_ack = ibus_ack;
  }

  if (dma_ebus_req) {
    dma1.on_ebus_ack(ebus_ack);
  }
  else if (cpu_has_ebus_req) {
    cpu_ack = ebus_ack;
  }

  if (dma_vbus_req) {
    dma1.on_vbus_ack(vbus_ack);
  }
  else if (ppu_vbus_req.read) {
    ppu.on_vbus_ack(vbus_ack);
  }
  else if (cpu_has_vbus_req) {
    cpu_ack = vbus_ack;
  }

  if (dma_obus_req) {
    dma1.on_obus_ack(obus_ack);
  }
  else if (ppu_obus_req) {
    ppu.on_obus_ack(obus_ack);
  }
  else if (cpu_has_obus_req) {
    cpu_ack = obus_ack;
  }

  if (cpu_ack.read == 0) {
    cpu_ack.data = 0xFF;
  }
}

//-----------------------------------------------------------------------------

void Gameboy::tock_gb() {
  auto& self = *this;

  phase++;

  if (PHASE_A) z80.tock_a(imask, intf, cpu_ack);
  if (PHASE_B) z80.tock_b(imask, intf, cpu_ack);
  if (PHASE_C) z80.tock_c(imask, intf, cpu_ack);
  if (PHASE_D) z80.tock_d(imask, intf, cpu_ack);
  if (PHASE_E) z80.tock_e(imask, intf, cpu_ack);
  if (PHASE_F) z80.tock_f(imask, intf, cpu_ack);
  if (PHASE_G) z80.tock_g(imask, intf, cpu_ack);
  if (PHASE_H) z80.tock_h(imask, intf, cpu_ack);

  timer2.tock(phase, ibus_req);

  if (PHASE_B || PHASE_D || PHASE_F || PHASE_H) {
    serial.tock(phase, ibus_req);
    joypad.tock(phase, ibus_req);
    zram.  tock(phase, ibus_req);
    spu.   tock(phase, ibus_req);
    boot.  tock(phase, ibus_req);
    cart.  tock(phase, ebus_req);
    iram.  tock(phase, ebus_req);
    vram.  tock(phase, vbus_req);
    oam.   tock(phase, obus_req);
    ppu.   tock(phase, ibus_req);
    self.  tock(phase, ibus_req);
    dma1.  tock(phase, ibus_req);
    dma2.  tock(phase, ibus_req);

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

    //-----------------------------------
    // gather reqs

    z80.get_bus_req(cpu_req);

    dma1.get_ebus_req(dma_ebus_req);
    dma1.get_vbus_req(dma_vbus_req);
    dma1.get_obus_req(dma_obus_req);

    ppu.get_vbus_req(ppu_vbus_req);
    ppu.get_obus_req(ppu_obus_req);

    //-----------------------------------
    // prioritize reqs

    bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
    bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

    bool dma_has_vbus_req = (PHASE_B || PHASE_D) && dma2.DMA_RUN && (dma2.addr >= ADDR_VRAM_BEGIN && dma2.addr <= ADDR_VRAM_END);
    bool dma_has_ebus_req = (PHASE_B || PHASE_D) && dma2.DMA_RUN && (dma2.addr >= ADDR_ROM_BEGIN  && dma2.addr <= ADDR_ROM_END);
    bool dma_has_obus_req = (PHASE_F || PHASE_H) && dma2.DMA_RUN;

    // ibus
    if (cpu_has_ibus_req) {
      ibus_req = cpu_req;
    }
    else {
      ibus_req = {0};
    }

    // ebus
    if (dma_has_ebus_req) {
      ebus_req = dma_ebus_req;
    }
    else if (cpu_has_ebus_req) {
      ebus_req = cpu_req;
    }
    else {
      ebus_req = {0};
    }

    // vbus
    if (dma_has_vbus_req) {
      vbus_req = dma_vbus_req;
    }
    else if (ppu_vbus_req) {
      vbus_req = ppu_vbus_req;
    }
    else if (cpu_has_vbus_req) {
      vbus_req = cpu_req;
    }
    else {
      vbus_req = {0};
    }

    // obus
    if (dma_has_obus_req) {
      obus_req = dma_obus_req;
    }
    else if (ppu_obus_req) {
      obus_req = ppu_obus_req;
    }
    else if (cpu_has_obus_req) {
      obus_req = cpu_req;
    }
    else {
      obus_req = {0};
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

  {
    bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
    bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

    char cpu_dir = cpu_req.write ? 'W' : cpu_req.read ? 'R' : '?';

    int cpu_to_ibus = cpu_has_ibus_req ? cpu_dir : ' ';
    int cpu_to_ebus = cpu_has_ebus_req ? cpu_dir : ' ';
    int cpu_to_vbus = cpu_has_vbus_req ? cpu_dir : ' ';
    int cpu_to_obus = cpu_has_obus_req ? cpu_dir : ' ';

    int dma_to_ebus = dma_ebus_req.read  ? 'R' : ' ';
    int dma_to_vbus = dma_vbus_req.read  ? 'R' : ' ';
    int dma_to_obus = dma_obus_req.write ? 'W' : ' ';

    int ppu_to_vbus = ppu_vbus_req.read ? 'R' : ' ';
    int ppu_to_obus = ppu_obus_req.read ? 'O' : ' ';

    if (dma_ebus_req.read) {
      cpu_to_ebus = tolower(cpu_to_ebus);
    }

    sprintf(d, "     |cpu|dma|ppu|\n");
    sprintf(d, " ibus|  %c|  %c|  %c|\n", cpu_to_ibus, ' ', ' ');
    sprintf(d, " ebus|  %c|  %c|  %c|\n", cpu_to_ebus, dma_to_ebus, ' ');
    sprintf(d, " vbus|  %c|  %c|  %c|\n", cpu_to_vbus, dma_to_vbus, ppu_to_vbus);
    sprintf(d, " obus|  %c|  %c|  %c|\n", cpu_to_obus, dma_to_obus, ppu_to_obus);
    sprintf(d, "\n");
  }

  /*
  sprintf(d, "---CPU:         "); print_req(d, cpu_req);
  sprintf(d, "---DMA to EBUS: "); print_req(d, dma_ebus_req);
  sprintf(d, "---DMA to VBUS: "); print_req(d, dma_vbus_req);
  sprintf(d, "---DMA to OBUS: "); print_req(d, dma_obus_req);
  sprintf(d, "---PPU to VBUS: "); print_req(d, ppu_vbus_req);
  sprintf(d, "---PPU to OBUS: "); print_req(d, ppu_obus_req);
  sprintf(d, "\n");

  sprintf(d, "---IBUS req:    "); print_req(d, ibus_req);
  sprintf(d, "---IBUS ack:    "); print_ack(d, ibus_ack);
  sprintf(d, "---EBUS req:    "); print_req(d, ebus_req);
  sprintf(d, "---EBUS ack:    "); print_ack(d, ebus_ack);
  sprintf(d, "---VBUS req:    "); print_req(d, vbus_req);
  sprintf(d, "---VBUS ack:    "); print_ack(d, vbus_ack);
  sprintf(d, "---OBUS req:    "); print_req(d, obus_req);
  sprintf(d, "---OBUS ack:    "); print_ack(d, obus_ack);
  */
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

/*
void MetroBoy::cycle() {
  current_gameboy->tick();
  current_gameboy->tock();

  if (trace) {
  tracebuffer[current_gameboy->ppu.line * 456 + current_gameboy->ppu.counter] = current_gameboy->trace();
  }

  if (current_gameboy->get_pix_oe()) {
  int x = current_gameboy->ppu.pix_count2 - 1;
  int y = current_gameboy->ppu.line;

  if (x >= 0 && x < 160 && y >= 0 && y < 144) {
  current_gameboy->framebuffer[x + y * 160] = current_gameboy->get_pix_out();
  }
  }

  current_gameboy->check_sentinel();
  cycles++;
}
*/

//-----------------------------------------------------------------------------