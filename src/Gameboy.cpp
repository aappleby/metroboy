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

  tcycle = -1;
  trace_val = 0;

  intf = 0xE1;
  imask = 0x00;

  sentinel = 0xDEADBEEF;
}

void Gameboy::reset(uint16_t new_pc) {
  reset(cart.get_rom_size(), new_pc);
}

//-----------------------------------------------------------------------------

Ack Gameboy::on_ibus_req(Req req) {
  bool hit = (req.addr == ADDR_IF) || (req.addr == ADDR_IE);
  if (!hit) return {};

  if (req.write) {
    if (req.addr == ADDR_IF) intf = (uint8_t)req.data | 0b11100000;
    if (req.addr == ADDR_IE) imask = (uint8_t)req.data;
    return {
      .phase = req.phase,
      .addr  = req.addr,
      .data  = req.data,
      .read  = 0,
      .write = 1,
    };
  }
  else if (req.read) {
    uint8_t data = 0;
    if (req.addr == ADDR_IF) data = 0b11100000 | intf;
    if (req.addr == ADDR_IE) data = imask;
    return {
      .phase = req.phase,
      .addr  = req.addr,
      .data  = data,
      .read  = 1,
      .write = 0,
    };
  }
  else {
    assert(false);
    return {};
  }
}

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

Ack merge(Ack a, Ack b) {
  assert(a.phase == b.phase);
  assert(a.addr == 0 || b.addr == 0);
  assert(a.data == 0 || b.data == 0);
  assert(a.read + a.write + b.read + b.write <= 1);
  return {
    int     (a.phase | b.phase),
    uint16_t(a.addr  | b.addr),
    uint16_t(a.data  | b.data),
    uint8_t (a.read  | b.read),
    uint8_t (a.write | b.write)
  };
}

template<typename... Args>
Ack merge (Ack a, Args... args) { return merge(a, merge(args...)); }

void Gameboy::tock() {
  tcycle++;
  const int tphase = tcycle & 3;
  
  bus_req_cpu = z80.get_bus_req();
  bool cpu_addr_is_boot = bus_req_cpu.addr <= ADDR_BOOT_END && !boot.disable_bootrom;
  bool cpu_addr_is_vram = bus_req_cpu.addr >= ADDR_VRAM_BEGIN && bus_req_cpu.addr <= ADDR_VRAM_END;
  bool cpu_addr_is_high = bus_req_cpu.addr > ADDR_ECHO_END;
  bool cpu_addr_is_oam  = bus_req_cpu.addr >= ADDR_OAM_BEGIN  && bus_req_cpu.addr <= ADDR_OAM_END;
  bool cpu_addr_is_ext  = !cpu_addr_is_boot && !cpu_addr_is_vram && !cpu_addr_is_high;

  {
    ebus_req_cpu = cpu_addr_is_ext ? bus_req_cpu : Req{0};
    ebus_req_dma = dma.get_ebus_req(tcycle);

    Req ebus_req = ebus_req_cpu;
    if (ebus_req_dma.read) ebus_req = ebus_req_dma;

    Ack ebus_ack_cart = cart.on_ebus_req(ebus_req);
    Ack ebus_ack_iram = iram.on_ebus_req(ebus_req);
    Ack ebus_ack = merge(ebus_ack_cart, ebus_ack_iram);

    if (ebus_req_dma.read) {
      ebus_ack_cpu = {};
      ebus_ack_dma = ebus_ack;
    }
    else {
      ebus_ack_cpu = ebus_ack;
      ebus_ack_dma = {};
    }
  }

  {
    vbus_req_cpu = cpu_addr_is_vram ? bus_req_cpu : Req{0};
    vbus_req_ppu = ppu.get_vbus_req(tcycle);
    vbus_req_dma = dma.get_vbus_req(tcycle);

    Req vbus_req = vbus_req_cpu;
    if (vbus_req_ppu.read) vbus_req = vbus_req_ppu;
    if (vbus_req_dma.read) vbus_req = vbus_req_dma;

    Ack vbus_ack = vram.on_vbus_req(vbus_req);

    if (vbus_req_dma.read) {
      vbus_ack_cpu = {};
      vbus_ack_ppu = {};
      vbus_ack_dma = vbus_ack;
    }
    else if (vbus_req_ppu.read) {
      vbus_ack_cpu = {};
      vbus_ack_ppu = vbus_ack;
      vbus_ack_dma = {};
    }
    else {
      vbus_ack_cpu = vbus_ack;
      vbus_ack_ppu = {};
      vbus_ack_dma = {};
    }
  }

  {
    obus_req_cpu = cpu_addr_is_oam  ? bus_req_cpu : Req{0};
    obus_req_ppu = ppu.get_obus_req(tcycle);
    obus_req_dma = dma.get_obus_req(tcycle);

    Req obus_req = obus_req_cpu;
    if (obus_req_ppu.read) obus_req = obus_req_ppu;
    if (obus_req_dma.write) obus_req = obus_req_dma;

    Ack obus_ack = oam.on_obus_req(obus_req);

    if (obus_req_dma.write) {
      obus_ack_cpu = {};
      obus_ack_ppu = {};
      obus_ack_dma = obus_ack;
    }
    else if (obus_req_ppu.read) {
      obus_ack_cpu = {};
      obus_ack_ppu = obus_ack;
      obus_ack_dma = {};
    }
    else {
      obus_ack_cpu = obus_ack;
      obus_ack_ppu = {};
      obus_ack_dma = {};
    }
  }

  ibus_ack_gb   = this-> on_ibus_req(bus_req_cpu);
  ibus_ack_tim  = timer .on_ibus_req(bus_req_cpu);
  ibus_ack_zram = zram  .on_ibus_req(bus_req_cpu);
  ibus_ack_joy  = joypad.on_ibus_req(bus_req_cpu);
  ibus_ack_ser  = serial.on_ibus_req(bus_req_cpu);
  ibus_ack_ppu  = ppu   .on_ibus_req(bus_req_cpu);
  ibus_ack_spu  = spu   .on_ibus_req(bus_req_cpu);
  ibus_ack_dma  = dma   .on_ibus_req(bus_req_cpu);
  ibus_ack_boot = boot  .on_ibus_req(bus_req_cpu);

  ibus_ack_cpu = merge(ibus_ack_gb,
                       ibus_ack_tim,
                       ibus_ack_zram,
                       ibus_ack_joy,
                       ibus_ack_ser,
                       ibus_ack_ppu,
                       ibus_ack_spu,
                       ibus_ack_dma,
                       ibus_ack_boot);

  bus_ack_cpu = merge(ibus_ack_cpu, ebus_ack_cpu, obus_ack_cpu, vbus_ack_cpu);

  z80.on_bus_ack(bus_ack_cpu);

  dma.on_ebus_ack(ebus_ack_dma);
  dma.on_vbus_ack(vbus_ack_dma);
  dma.on_obus_ack(obus_ack_dma);

  ppu.on_vbus_ack(vbus_ack_ppu);
  ppu.on_obus_ack(obus_ack_ppu);

  //-----------------------------------
  // interrupt stuff

  uint8_t intf_ = intf;
  uint8_t imask_ = imask;

  if (tphase == 0 || tphase == 2) {
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
  z80.tock(tcycle, imask_, intf_);

  //-----------------------------------
  // Peripheral bus mux & tocks

  timer.tock(tcycle);
  ppu  .tock(tcycle);
  spu  .tock(tcycle);

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
  sprintf(d, "tcycle %d\n", tcycle);
  sprintf(d, "imask  %s\n", byte_to_bits(imask));
  sprintf(d, "intf   %s\n", byte_to_bits(intf));
  sprintf(d, "boot   %d\n", boot.disable_bootrom);
  sprintf(d, "\n");
  

  sprintf(d,   "---CPU:\n");
  print_req(d, "bus_req_cpu ",  bus_req_cpu);
  print_ack(d, "bus_ack_cpu ",  bus_ack_cpu);

  sprintf(d,   "---IBUS:\n");
  print_req(d, "ibus_req_cpu",  ibus_req_cpu);
  print_ack(d, "ibus_ack_gb",   ibus_ack_gb);
  print_ack(d, "ibus_ack_tim",  ibus_ack_tim);
  print_ack(d, "ibus_ack_zram", ibus_ack_zram);
  print_ack(d, "ibus_ack_joy",  ibus_ack_joy);
  print_ack(d, "ibus_ack_ser",  ibus_ack_ser);
  print_ack(d, "ibus_ack_ppu",  ibus_ack_ppu);
  print_ack(d, "ibus_ack_spu",  ibus_ack_spu);
  print_ack(d, "ibus_ack_dma",  ibus_ack_dma);
  print_ack(d, "ibus_ack_boot", ibus_ack_boot);
  print_ack(d, "ibus_ack_cpu",  ibus_ack_cpu);

  sprintf(d,   "---EBUS:\n");
  print_req(d, "ebus_req_cpu",  ebus_req_cpu);
  print_req(d, "ebus_req_dma",  ebus_req_dma);
  print_ack(d, "ebus_ack_cpu",  ebus_ack_cpu);
  print_ack(d, "ebus_ack_dma",  ebus_ack_dma);

  sprintf(d,   "---VBUS:\n");
  print_req(d, "vbus_req_cpu",  vbus_req_cpu);
  print_req(d, "vbus_req_ppu",  vbus_req_ppu);
  print_req(d, "vbus_req_dma",  vbus_req_dma);
  print_ack(d, "vbus_ack_cpu",  vbus_ack_cpu);
  print_ack(d, "vbus_ack_ppu",  vbus_ack_ppu);
  print_ack(d, "vbus_ack_dma",  vbus_ack_dma);

  sprintf(d,   "---OBUS:\n");
  print_req(d, "obus_req_cpu",  obus_req_cpu);
  print_req(d, "obus_req_ppu",  obus_req_ppu);
  print_req(d, "obus_req_dma",  obus_req_dma);
  print_ack(d, "obus_ack_ppu",  obus_ack_ppu);
  print_ack(d, "ebus_ack_dma",  ebus_ack_dma);
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

void Gameboy::dump_vram(std::string& d) {
  vram.dump(d);
}

void Gameboy::dump_iram(std::string& d) {
  iram.dump(d);
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