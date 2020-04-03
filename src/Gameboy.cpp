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

  sentinel = 0xDEADBEEF;
}

void Gameboy::reset(uint16_t new_pc) {
  reset(cart.get_rom_size(), new_pc);
}

//-----------------------------------------------------------------------------

//#pragma warning(push)
#pragma warning(disable:4458)

bool Gameboy::on_ibus_req(Req ibus_req, Ack& ibus_ack) {
  bool hit = (ibus_req.addr == ADDR_IF) || (ibus_req.addr == ADDR_IE);
  if (!hit) return {};

  if (ibus_req.write) {
    if (ibus_req.addr == ADDR_IF) intf  = (uint8_t)ibus_req.data | 0b11100000;
    if (ibus_req.addr == ADDR_IE) imask = (uint8_t)ibus_req.data;
    ibus_ack = {
      .addr  = ibus_req.addr,
      .data  = ibus_req.data,
      .read  = 0,
      .write = 1,
    };
    return true;
  }
  else if (ibus_req.read) {
    uint8_t data = 0;
    if (ibus_req.addr == ADDR_IF) data = 0b11100000 | intf;
    if (ibus_req.addr == ADDR_IE) data = imask;
    ibus_ack = {
      .addr = ibus_req.addr,
      .data  = data,
      .read  = 1,
      .write = 0,
    };
    return true;
  }
  else {
    assert(false);
    return false;
  }
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


  cpu_req = {};
  cpu_ack = {};
  ebus_req = {};
  ebus_ack = {};
  vbus_req = {};
  vbus_ack = {};
  vbus_ack = {};
  obus_req = {};
  obus_ack = {};
  ibus_req = {};
  ibus_ack = {};

  if (tphase_new == 0) {
    cpu_req = z80.get_bus_req();
  }
  int region = cpu_req.addr >> 13;

  bool cpu_has_req = cpu_req.read || cpu_req.write;
  bool cpu_has_vbus_req = cpu_has_req && cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
  bool cpu_has_obus_req = cpu_has_req && cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
  bool cpu_has_ibus_req = cpu_has_req && cpu_req.addr >= ADDR_IOBUS_BEGIN;
  bool cpu_has_ebus_req = cpu_has_req && !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

  if (cpu_has_ibus_req) {
    Req ibus_req = cpu_req;
    Ack ibus_ack = {};
    this-> on_ibus_req(                 ibus_req, ibus_ack);
    timer. on_ibus_req(                 ibus_req, ibus_ack);
    zram.  on_ibus_req(                 ibus_req, ibus_ack);
    joypad.on_ibus_req(                 ibus_req, ibus_ack);
    serial.on_ibus_req(                 ibus_req, ibus_ack);
    ppu.   on_ibus_req(int(tcycle_new), ibus_req, ibus_ack);
    spu.   on_ibus_req(                 ibus_req, ibus_ack);
    dma.   on_ibus_req(int(tcycle_new), ibus_req, ibus_ack);
    boot.  on_ibus_req(                 ibus_req, ibus_ack);

    cpu_ack = ibus_ack;
    z80.on_bus_ack(ibus_ack);
  }

  //-----------------------------------
  // External bus mux

  if (dma.has_ebus_req(int(tcycle_new))) {
    ebus_req = dma.get_ebus_req(int(tcycle_new));
    ebus_ack = {};
    cart.on_ebus_req(ebus_req, ebus_ack);
    iram.on_ebus_req(ebus_req, ebus_ack);
    dma.on_ebus_ack(ebus_ack);
  }
  else if (cpu_has_ebus_req) {
    ebus_req = cpu_req;
    ebus_ack = {};
    cart.on_ebus_req(ebus_req, ebus_ack);
    iram.on_ebus_req(ebus_req, ebus_ack);

    cpu_ack = ebus_ack;
    z80.on_bus_ack(ebus_ack);
    // cpu_ack = ebus_ack;
  }

  //-----------------------------------
  // Vram bus mux

  if (dma.has_vbus_req(int(tcycle_new))) {
    vbus_req = dma.get_vbus_req(int(tcycle_new));
    vbus_ack = {};
    vram.on_vbus_req(vbus_req, vbus_ack);
    dma.on_vbus_ack(vbus_ack);
  }
  else if (ppu.has_vbus_req(int(tcycle_new))) {
    vbus_req = ppu.get_vbus_req(int(tcycle_new));
    vbus_ack = {};
    vram.on_vbus_req(vbus_req, vbus_ack);
    ppu.on_vbus_ack(vbus_ack);
  }
  else if (cpu_has_vbus_req) {
    vbus_req = cpu_req;
    vbus_ack = {};
    vram.on_vbus_req(vbus_req, vbus_ack);

    cpu_ack = vbus_ack;
    z80.on_bus_ack(vbus_ack);
    // cpu_ack = vbus_ack;
  }

  //-----------------------------------
  // OAM bus mux

  if (dma.has_obus_req(int(tcycle_new))) {
    obus_req = dma.get_obus_req(int(tcycle_new));
    obus_ack = {};
    oam.on_obus_req(obus_req, obus_ack);
    dma.on_obus_ack(obus_ack);
  }
  else if (ppu.has_obus_req(int(tcycle_new))) {
    obus_req = ppu.get_obus_req(int(tcycle_new));
    obus_ack = {};
    oam.on_obus_req(obus_req, obus_ack);
    ppu.on_obus_ack(obus_ack);
  }
  else if (cpu_has_obus_req) {
    obus_req = cpu_req;
    obus_ack = {};
    oam.on_obus_req(obus_req, obus_ack);

    cpu_ack = obus_ack;
    z80.on_bus_ack(obus_ack);
    //cpu_ack = obus_ack;
  }

  //-----------------------------------
  // interrupt stuff

  uint8_t intf_ = intf;
  uint8_t imask_ = imask;

  if (tphase_new == 0 || tphase_new == 2) {
    bool fire_int_timer1   = timer.get_interrupt();;
    bool fire_int_buttons1 = joypad.get() != 0xFF;
    //bool fire_int_timer2 = timer_to_bus.overflow;
    //bool fire_int_buttons2 = joy_to_bus.val != 0xFF;

    if (imask & 0x01) z80.unhalt |= ppu.vblank1;
    if (imask & 0x02) z80.unhalt |= ppu.stat2;
    if (imask & 0x04) z80.unhalt |= fire_int_timer1;
    if (imask & 0x10) z80.unhalt |= fire_int_buttons1;

    if (ppu.vblank1)       intf_ |= INT_VBLANK_MASK;
    if (ppu.stat1)         intf_ |= INT_STAT_MASK;
    if (fire_int_timer1)   intf_ |= INT_TIMER_MASK;
    if (fire_int_buttons1) intf_ |= INT_JOYPAD_MASK;
  }

  //-----------------------------------
  // Z80 bus mux & tock

  intf_ &= ~z80.get_int_ack();

  switch (tphase_new) {
    case 0:
      z80.tock_t30(imask_, intf_);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
  };
  

  //-----------------------------------
  // Peripheral bus mux & tocks

  timer.tock(int(tcycle_new));
  ppu  .tock(int(tcycle_new));
  spu  .tock(int(tcycle_new));
  dma  .tock(int(tcycle_new));

  intf = intf_;
  imask = imask_;

  //----------

  gb_to_host.x       = ppu.pix_count;
  gb_to_host.y       = ppu.line;
  gb_to_host.counter = ppu.counter;
  gb_to_host.pix     = ppu.pix_out;
  gb_to_host.pix_oe  = ppu.pix_oe;
  gb_to_host.out_r   = spu.get_r();
  gb_to_host.out_l   = spu.get_l();
  gb_to_host.trace   = 0; //vbus_req.addr;
  gb_to_host.trace   = 0;
}

//-----------------------------------------------------------------------------

#if 0
{
//return z80.get_state() << 4;

//return z80.get_op();

//return z80.op_ == 0x76 ? 0xFFFFFFFF : 0; // moderately interesting
//return z80.op_ == 0x00 ? 0xFFFFFFFF : 0; // moderately interesting
//return z80.op_ == 0xcb ? 0xFFFFFFFF : 0; // moderately interesting
//return (z80.get_op() & 0b11000000) == 0b10000000 ? 0xFFFFFFFF : 0; // moderately interesting

//return (z80.mem_addr >= ADDR_SPU_BEGIN && z80.mem_addr < ADDR_SPU_END) ? -1 : 0; // sparse

//return ppu.sprite_index << 4; // also pretty cool

//return cpu_to_bus.addr;
trace_val = ppu_out.vram_addr; // this one's pretty cool
                                //return cpu_to_bus.write ? 0xFFFFFFFF : 0x00000000;
}
#endif

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