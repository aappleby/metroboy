#include "Gameboy.h"

#include "Assembler.h"
#include "Constants.h"

#include <assert.h>
#include <imgui.h>

extern uint8_t rom_buf[];
extern const uint8_t DMG_ROM_bin[];

//-----------------------------------------------------------------------------

void Gameboy::reset(size_t new_rom_size, uint16_t new_pc) {
  *this = {};

  z80.reset(new_pc);
  cart.reset(new_rom_size);
  ppu.reset(new_pc == 0);
  oam.reset();
  spu.reset();
  timer.reset();
  vram.reset();
  iram.reset();
  joypad.reset();
  serial.reset();
  zram.reset();
  boot.reset(new_pc);

  tcycle_old = -2;
  tcycle_new = -1;
  trace_val = 0;

  intf = 0xE1;
  imask = 0x00;
  ack = {0};

  sentinel = 0xDEADBEEF;
}

void Gameboy::reset(uint16_t new_pc) {
  reset(cart.get_rom_size(), new_pc);
}

//-----------------------------------------------------------------------------

//#pragma warning(push)
#pragma warning(disable:4458)

void Gameboy::ibus_req2(Req ibus_req) {
  ack = {0};
  bool hit = (ibus_req.addr == ADDR_IF) || (ibus_req.addr == ADDR_IE);

  if (!hit) {
    return;
  }
  else if (ibus_req.write) {
    if (ibus_req.addr == ADDR_IF) intf  = (uint8_t)ibus_req.data | 0b11100000;
    if (ibus_req.addr == ADDR_IE) imask = (uint8_t)ibus_req.data;
    ack = {
      .addr  = ibus_req.addr,
      .data  = ibus_req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else if (ibus_req.read) {
    uint8_t data = 0;
    if (ibus_req.addr == ADDR_IF) data = 0b11100000 | intf;
    if (ibus_req.addr == ADDR_IE) data = imask;
    ack = {
      .addr = ibus_req.addr,
      .data  = data,
      .read  = 1,
      .write = 0,
    };
  }
  else {
    assert(false);
  }
}

void Gameboy::ibus_ack2(Ack& ibus_ack) const {
  ibus_ack.addr  += ack.addr;
  ibus_ack.data  += ack.data;
  ibus_ack.read  += ack.read;
  ibus_ack.write += ack.write;
}

//#pragma warning(pop)

//-----------------------------------------------------------------------------

/*
void print_ack(std::string& d, const char* name, const Ack& ack) {
  sprintf(d, "%-8s %d 0x%04x 0x%02x %c %c\n",
          name,
          ack.phase,
          ack.addr,
          ack.read ? 'R' : '-',
          ack.write ? 'W' : '-');
}
*/

//-----------------------------------------------------------------------------
// 4 mhz tick/tock

#pragma warning(disable:4189)

void Gameboy::tock() {
  tcycle_old = tcycle_new;
  tcycle_new = tcycle_new + 1;
  tphase_old = tcycle_old & 3;
  tphase_new = tcycle_new & 3;

  //-----------------------------------
  // Internal bus mux

  /*
  if ((tphase_new == 0) && cpu_has_ibus_req) {
    ibus_req = cpu_req;
  }

  if (dma.has_ebus_req()) {
    ebus_req = dma.get_ebus_req();
  }
  else if (cpu_has_ebus_req) {
    ebus_req = cpu_req;
  }

  if (dma.has_vbus_req()) {
    vbus_req = dma.get_vbus_req();
  }
  else if (ppu.has_vbus_req()) {
    vbus_req = ppu.get_vbus_req();
  }
  else if (cpu_has_vbus_req) {
    vbus_req = cpu_req;
  }

  if (dma.has_obus_req()) {
    obus_req = dma.get_obus_req();
  }
  else if (ppu.has_obus_req()) {
    obus_req = ppu.get_obus_req();
  }
  else if (cpu_has_obus_req) {
    obus_req = cpu_req;
  }
  */

  //-----------------------------------

  if (tphase_old == 0) {
    uint8_t intf_ = intf;
    uint8_t imask_ = imask;

    ibus_ack = {0};
    ebus_ack = {0};
    vbus_ack = {0};
    obus_ack = {0};

    if (ibus_req.read || ibus_req.write) {
      this->ibus_ack2(ibus_ack);
      ppu.   ibus_ack(ibus_ack);
      serial.ibus_ack(ibus_ack);
      joypad.ibus_ack(ibus_ack);
      zram.  ibus_ack(ibus_ack);
      timer. ibus_ack(ibus_ack);
      spu.   ibus_ack(ibus_ack);
      dma.   ibus_ack(ibus_ack);
      boot.  ibus_ack(ibus_ack);
    }

    if (ebus_req.read || ebus_req.write) {
      cart.ebus_ack(ebus_ack);
      iram.ebus_ack(ebus_ack);
    }

    if (vbus_req.read || vbus_req.write) {
      vram.vbus_ack(vbus_ack);
    }

    if (obus_req.read || obus_req.write) {
      oam.obus_ack(obus_ack);
    }

    //-----------------------------------

    if (ibus_ack.read  > 1) __debugbreak();
    if (ibus_ack.write > 1) __debugbreak();
    if (ebus_ack.read  > 1) __debugbreak();
    if (ebus_ack.write > 1) __debugbreak();
    if (vbus_ack.read  > 1) __debugbreak();
    if (vbus_ack.write > 1) __debugbreak();
    if (obus_ack.read  > 1) __debugbreak();
    if (obus_ack.write > 1) __debugbreak();

    int region = cpu_req.addr >> 13;
    bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
    bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

    if (cpu_has_ibus_req) cpu_ack = ibus_ack;
    if (cpu_has_ebus_req) cpu_ack = ebus_ack;
    if (cpu_has_vbus_req) cpu_ack = vbus_ack;
    if (cpu_has_obus_req) cpu_ack = obus_ack;

    bool fire_int_timer1   = timer.get_interrupt();
    bool fire_int_buttons1 = joypad.get() != 0xFF;

    if (imask & 0x01) z80.unhalt |= ppu.vblank1;
    if (imask & 0x02) z80.unhalt |= ppu.stat2;
    if (imask & 0x04) z80.unhalt |= fire_int_timer1;
    if (imask & 0x10) z80.unhalt |= fire_int_buttons1;

    if (ppu.vblank1)       intf_ |= INT_VBLANK_MASK;
    if (ppu.stat1)         intf_ |= INT_STAT_MASK;
    if (fire_int_timer1)   intf_ |= INT_TIMER_MASK;
    if (fire_int_buttons1) intf_ |= INT_JOYPAD_MASK;

    z80.on_bus_ack(cpu_ack);
    z80.tock_t01(imask_, intf_);
    timer.tock(int(tcycle_new));
    //ppu  .tock(int(tcycle_new));
    //spu  .tock(int(tcycle_new));
    //dma  .tock(int(tcycle_new));

    intf = intf_;
    imask = imask_;
  }

  //-----------------------------------

  if (tphase_old == 1) {
    uint8_t intf_ = intf;
    uint8_t imask_ = imask;

    z80.tock_t12(imask_, intf_);
    intf_ &= ~z80.get_int_ack();
    timer.tock(int(tcycle_new));
    //ppu  .tock(int(tcycle_new));
    //spu  .tock(int(tcycle_new));
    //dma  .tock(int(tcycle_new));

    intf = intf_;
    imask = imask_;
  }

  //-----------------------------------

  if (tphase_old == 2) {
    uint8_t intf_ = intf;
    uint8_t imask_ = imask;

    cpu_req = z80.get_bus_req();
    int region = cpu_req.addr >> 13;
    bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
    bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

    ibus_req = {};
    ebus_req = {};
    vbus_req = {};
    obus_req = {};

    if (cpu_has_ibus_req) ibus_req = cpu_req;
    if (cpu_has_ebus_req) ebus_req = cpu_req;
    if (cpu_has_vbus_req) vbus_req = cpu_req;
    if (cpu_has_obus_req) obus_req = cpu_req;

    if (ibus_req.read || ibus_req.write) {
      this->ibus_req2(ibus_req);
      intf_ = intf;
      imask_ = imask;

      ppu.   ibus_req(ibus_req);
      serial.ibus_req(ibus_req);
      joypad.ibus_req(ibus_req);
      zram.  ibus_req(ibus_req);
      timer. ibus_req(ibus_req);
      spu.   ibus_req(ibus_req);
      dma.   ibus_req(ibus_req);
      boot.  ibus_req(ibus_req);
    }

    if (ebus_req.read || ebus_req.write) {
      cart.ebus_req(ebus_req);
      iram.ebus_req(ebus_req);
    }

    if (vbus_req.read || vbus_req.write) {
      vram.vbus_req(vbus_req);
    }

    if (obus_req.read || obus_req.write) {
      oam.obus_req(obus_req);
    }

    z80.tock_t23(imask_, intf_);
    timer.tock(int(tcycle_new));
    //ppu  .tock(int(tcycle_new));
    //spu  .tock(int(tcycle_new));
    //dma  .tock(int(tcycle_new));

    intf = intf_;
    imask = imask_;

  }

  //-----------------------------------

  if (tphase_old == 3) {
    uint8_t intf_ = intf;
    uint8_t imask_ = imask;

    z80.tock_t30(imask_, intf_);
    timer.tock(int(tcycle_new));
    //ppu  .tock(int(tcycle_new));
    //spu  .tock(int(tcycle_new));
    //dma  .tock(int(tcycle_new));

    intf = intf_;
    imask = imask_;
  }

  //-----------------------------------

  /*
  if (cpu_has_ibus_req) {
    cpu_ack = ibus_ack;
  }

  if (dma.has_ebus_req()) {
    dma.on_ebus_ack(ebus_ack);
  }
  else if (cpu_has_ebus_req) {
    cpu_ack = ebus_ack;
  }

  if (dma.has_vbus_req()) {
    dma.on_vbus_ack(vbus_ack);
  }
  else if (ppu.has_vbus_req()) {
    ppu.on_vbus_ack(vbus_ack);
  }
  else if (cpu_has_vbus_req) {
    cpu_ack = vbus_ack;
  }

  if (dma.has_obus_req()) {
    dma.on_obus_ack(obus_ack);
  }
  else if (ppu.has_obus_req()) {
    ppu.on_obus_ack(obus_ack);
  }
  else if (cpu_has_obus_req) {
    cpu_ack = obus_ack;
  }
  */

  //-----------------------------------
  // Z80 bus mux & tock

  //-----------------------------------
  // Peripheral bus mux & tocks

  //----------

  /*
  gb_to_host.x       = ppu.pix_count;
  gb_to_host.y       = ppu.line;
  gb_to_host.counter = ppu.counter;
  gb_to_host.pix     = ppu.pix_out;
  gb_to_host.pix_oe  = ppu.pix_oe;
  gb_to_host.out_r   = spu.get_r();
  gb_to_host.out_l   = spu.get_l();
  gb_to_host.trace   = vbus_req.addr;
  gb_to_host.trace   = 0;
  */
}

//-----------------------------------------------------------------------------

void Gameboy::dump_cpu(std::string& d) {
  sprintf(d, "\002------------- CPU --------------\001\n");
  z80.dump(d);
}

void Gameboy::dump_bus(std::string& d) {
  sprintf(d, "\002------------- BUS --------------\001\n");
  sprintf(d, "tcycle_old %lld\n", tcycle_old);
  sprintf(d, "tcycle_new %lld\n", tcycle_new);
  sprintf(d, "tphase_old %d\n", int(tcycle_old & 3));
  sprintf(d, "tphase_new %d\n", int(tcycle_new & 3));
  sprintf(d, "bgb cycle      0x%08x\n", (tcycle_new * 8) + 0x00B2D5E6);
  sprintf(d, "imask  %s\n", byte_to_bits(imask));
  sprintf(d, "intf   %s\n", byte_to_bits(intf));
  sprintf(d, "boot   %d\n", boot.disable_bootrom);
  sprintf(d, "\n");


  sprintf(d,   "---CPU:\n");
  print_req(d, "cpu_req  ", cpu_req);
  print_ack(d, "cpu_ack  ", cpu_ack);

  sprintf(d,   "---IBUS:\n");
  print_req(d, "ibus_req ", ibus_req);
  print_ack(d, "ibus_ack ", ibus_ack);

  sprintf(d,   "---EBUS:\n");
  print_req(d, "ebus_req ", ebus_req);
  print_ack(d, "ebus_ack ", ebus_ack);

  sprintf(d,   "---VBUS:\n");
  print_req(d, "vbus_req ", vbus_req);
  print_ack(d, "vbus_ack ", vbus_ack);

  sprintf(d,   "---OBUS:\n");
  print_req(d, "obus_req ", obus_req);
  print_ack(d, "obus_ack ", obus_ack);
  sprintf(d,   "\n");
}

void Gameboy::dump_zram(std::string& d) {
  zram.dump(d);
}

void Gameboy::dump_timer(std::string& d) {
  timer.dump(d);
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