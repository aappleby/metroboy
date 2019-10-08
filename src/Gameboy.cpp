#include "Gameboy.h"

#include "Assembler.h"
#include "Constants.h"

#include <assert.h>

extern uint8_t rom_buf[];
extern const uint8_t DMG_ROM_bin[];

//-----------------------------------------------------------------------------

Gameboy::Gameboy() {
  reset(MODEL_DMG, 0, 0);
}

void Gameboy::reset(int new_model, size_t new_rom_size, uint16_t new_pc) {
  gb_out = {};
  host_out = {};
  cpu_in = {};

  z80.reset(new_model, new_pc);
  mmu.reset(new_rom_size, new_pc);
  ppu.reset(new_pc == 0, new_model);
  oam.reset();
  spu.reset();
  timer.reset();
  vram.reset();
  iram.reset();
  buttons.reset();
  serial.reset();
  zram.reset();

  ppu_out = {};
  oam_out = {};

  model = new_model;
  tcycle = -1;
  trace_val = 0;

  dma_mode_x = DMA_NONE;
  dma_count_x = 0;
  dma_source_x = 0x000;

  dma_mode_a = DMA_NONE;
  dma_count_a = 0;
  dma_source_a = 0x000;

  dma_mode_b = DMA_NONE;
  dma_count_b = 0;
  dma_data_b = 0x00;

  intf = 0xE1;
  imask = 0x00;

  sentinel = 0xDEADBEEF;
}

void Gameboy::reset(uint16_t new_pc) {
  reset(model, mmu.get_rom_size(), new_pc);
}

//-----------------------------------------------------------------------------
// 4 mhz tick/tock

Gameboy::Out Gameboy::tick() const {
  return gb_out;
}

//-----------------------------------------------------------------------------

Gameboy::HostOut Gameboy::tock() {
  tcycle++;
  int tphase = tcycle & 3;

  auto cpu_bus = z80.tick();

  const auto iram_out    = iram.tick();
  const auto mmu_out     = mmu.tick();
  const auto buttons_out = buttons.tick();
  const auto serial_out  = serial.tick();;
  const auto zram_out    = zram.tick();
  const auto spu_out     = spu.tick();
  const auto timer_out   = timer.tick();
  const auto vram_out    = vram.tick();
  
  oam_out = oam.tick();
  ppu_out = ppu.tick(tphase);

  //-----------------------------------
  // DMA state machine

  if (tphase == 0) {
    dma_mode_b = dma_mode_a;
    dma_count_b = dma_count_a;

    dma_mode_a = dma_mode_x;
    dma_count_a = dma_count_x;
    dma_source_a = dma_source_x;

    if (dma_mode_x != DMA_NONE) dma_count_x++;
    if (dma_count_x == 160) dma_mode_x = DMA_NONE;

    if (cpu_bus.write) {
      if (cpu_bus.addr == ADDR_DMA) {
        if (cpu_bus.data <= 0x7F) dma_mode_x = DMA_CART;
        if (0x80 <= cpu_bus.data && cpu_bus.data <= 0x9F) dma_mode_x = DMA_VRAM;
        if (0xA0 <= cpu_bus.data && cpu_bus.data <= 0xBF) dma_mode_x = DMA_CART;
        if (0xC0 <= cpu_bus.data && cpu_bus.data <= 0xFD) dma_mode_x = DMA_IRAM;
        dma_count_x = 0;
        dma_source_x = cpu_bus.data;
      }
    }
  }

  //-----------------------------------
  // oam bus mux

  CpuBus oam_bus = { ppu_out.oam_addr, 0, ppu_out.oam_addr != 0, false };
  bool cpu_read_oam = false;

  if (dma_mode_b != DMA_NONE) {
    if (tphase == 0) {
      uint8_t dma_data = 0;
      if (dma_mode_b == DMA_CART) dma_data = mmu_out.data;
      if (dma_mode_b == DMA_VRAM) dma_data = vram_out.data;
      if (dma_mode_b == DMA_IRAM) dma_data = iram_out.data;

      uint16_t dma_write_addr = ADDR_OAM_BEGIN + dma_count_b;
      oam_bus = { dma_write_addr, dma_data, false, true };
    }
    else {
      oam_bus = {};
    }
  }
  else {
    if (ppu_out.oam_addr == 0) {
      bool ce_oam  = (cpu_bus.addr & 0xFF00) == 0xFE00;
      cpu_read_oam = cpu_bus.read && ce_oam;
      oam_bus = cpu_bus;
    }
  }

  //-----------------------------------
  // interrupt stuff

  uint8_t intf_ = intf;
  uint8_t imask_ = imask;

  if (tphase == 0 || tphase == 2) {
    bool fire_int_timer1   = timer_out.interrupt;
    bool fire_int_buttons1 = buttons_out.val != 0xFF;
    //bool fire_int_timer2 = timer_out.overflow;
    //bool fire_int_buttons2 = buttons_out.val != 0xFF;

    if (imask & 0x01) z80.unhalt |= ppu_out.vblank1;
    if (imask & 0x02) z80.unhalt |= ppu_out.stat2;
    if (imask & 0x04) z80.unhalt |= fire_int_timer1;
    if (imask & 0x10) z80.unhalt |= fire_int_buttons1;

    if (ppu_out.vblank1)   intf_ |= INT_VBLANK;
    if (ppu_out.stat1)     intf_ |= INT_STAT;
    if (fire_int_timer1)   intf_ |= INT_TIMER;
    if (fire_int_buttons1) intf_ |= INT_JOYPAD;

    if (tphase == 0) intf_ &= ~cpu_bus.int_ack;
  }

  //-----------------------------------
  // reads happen on t2

  if (tphase == 0 && cpu_bus.read) {
    gb_out = {};

    if (cpu_bus.addr == ADDR_IF) { 
      // FIXME intf or intf_?
      gb_out.addr = cpu_bus.addr;
      gb_out.data = 0b11100000 | intf_;
      gb_out.oe = 1;;
    }
    if (cpu_bus.addr == ADDR_IE) {
      // FIXME imask or imask_?
      gb_out.addr = cpu_bus.addr;
      gb_out.data = imask_;
      gb_out.oe = 1;
    }
  }

  if (tphase == 2 && cpu_bus.write) {
    if (cpu_bus.addr == ADDR_IF) {
      intf_ = cpu_bus.data | 0b11100000;
    }
    if (cpu_bus.addr == ADDR_IE) {
      imask_ = cpu_bus.data;
    }
  }

  //-----------------------------------
  // z80 tocks last

  if (tphase == 2) {
    cpu_in = {};

    int page = cpu_bus.addr >> 13;

    bool ce_rom  = page <= 3;
    bool ce_vram = page == 4;
    bool ce_cram = page == 5;
    bool ce_iram = page == 6;
    bool ce_echo = page == 7 && (cpu_bus.addr < 0xFE00);

    bool cpu_read_vram = (dma_mode_a != DMA_VRAM) && (ppu_out.vram_addr == 0) && ce_vram;
    bool cpu_read_iram = (dma_mode_a != DMA_IRAM) && (ce_iram || ce_echo);
    bool cpu_read_cart = (dma_mode_a != DMA_CART) && (ce_rom || ce_cram);

    cpu_in.addr |= cpu_read_cart ? mmu_out.addr : 0x0000;
    cpu_in.addr |= cpu_read_vram ? vram_out.addr : 0x0000;
    cpu_in.addr |= cpu_read_iram ? iram_out.addr : 0x0000;
    cpu_in.addr |= cpu_read_oam  ? oam_out.addr : 0x0000;

    cpu_in.addr |= ppu_out.addr;
    cpu_in.addr |= buttons_out.addr;
    cpu_in.addr |= serial_out.addr;
    cpu_in.addr |= spu_out.addr;
    cpu_in.addr |= timer_out.addr;
    cpu_in.addr |= zram_out.addr;
    cpu_in.addr |= gb_out.addr;

    cpu_in.data |= cpu_read_cart ? mmu_out.data : 0x00;
    cpu_in.data |= cpu_read_vram ? vram_out.data : 0x00;
    cpu_in.data |= cpu_read_iram ? iram_out.data : 0x00;
    cpu_in.data |= cpu_read_oam  ? oam_out.data : 0x00;

    cpu_in.data |= ppu_out.data;
    cpu_in.data |= buttons_out.data;
    cpu_in.data |= serial_out.data;
    cpu_in.data |= spu_out.data;
    cpu_in.data |= timer_out.data;
    cpu_in.data |= zram_out.data;
    cpu_in.data |= gb_out.data;

    cpu_in.oe += cpu_read_cart;
    cpu_in.oe += cpu_read_vram;
    cpu_in.oe += cpu_read_iram;
    cpu_in.oe += cpu_read_oam;

    cpu_in.oe += ppu_out.oe;
    cpu_in.oe += buttons_out.oe;
    cpu_in.oe += serial_out.oe;
    cpu_in.oe += spu_out.oe;
    cpu_in.oe += timer_out.oe;
    cpu_in.oe += zram_out.oe;
    cpu_in.oe += gb_out.oe;

    if (cpu_in.oe > 1) {
      printf("BUS COLLISION\n");
    }
    else if (cpu_in.oe == 0) {
      cpu_in.data = 0xFF;
    }

    cpu_in.imask = imask_;
    cpu_in.intf = intf_;

    z80.tock(cpu_in);
  }

  //-----------------------------------

  uint16_t dma_read_addr = (dma_source_a << 8) | dma_count_a;

  CpuBus dma_bus = { dma_read_addr, 0, true, false };
  CpuBus ppu_bus = { ppu_out.vram_addr, 0, ppu_out.vram_addr != 0, false };

  vram.tock   (tphase, dma_mode_a == DMA_VRAM ? dma_bus : ppu_out.vram_addr ? ppu_bus : cpu_bus);
  iram.tock   (tphase, dma_mode_a == DMA_IRAM ? dma_bus : cpu_bus);
  mmu.tock    (tphase, dma_mode_a == DMA_CART ? dma_bus : cpu_bus);
  buttons.tock(tphase, cpu_bus);
  serial.tock (tphase, cpu_bus);
  zram.tock   (tphase, cpu_bus);
  spu.tock    (tphase, cpu_bus);
  timer.tock  (tphase, cpu_bus);
  oam.tock    (tphase, oam_bus);
  ppu.tock    (tphase, cpu_bus, vram_out, oam_out);

  intf = intf_;
  imask = imask_;

  //----------

  HostOut out2;
  out2.x       = ppu_out.x;
  out2.y       = ppu_out.y;
  out2.counter = ppu_out.counter;
  out2.pix     = ppu_out.pix_out;
  out2.pix_oe  = ppu_out.pix_oe;
  out2.out_r   = spu_out.out_r;
  out2.out_l   = spu_out.out_l;
  out2.trace   = ppu_out.vram_addr;

  return out2;
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

//return cpu_bus.addr;
trace_val = ppu_out.vram_addr; // this one's pretty cool
                                //return cpu_bus.write ? 0xFFFFFFFF : 0x00000000;
}
#endif

//-----------------------------------------------------------------------------

void Gameboy::dump(std::string& d) {

  sprintf(d, "--------------TOP--------------\n");
  sprintf(d, "model          %d\n", model);
  sprintf(d, "tcycle         %d\n", tcycle);
  sprintf(d, "dma_mode_a     %d\n", dma_mode_a);
  sprintf(d, "dma_count_a    %d\n", dma_count_a);
  sprintf(d, "dma_source_a   0x%04x\n", dma_source_a);
  sprintf(d, "dma_mode_b     %d\n", dma_mode_b);
  sprintf(d, "dma_count_b    %d\n", dma_count_b);
  sprintf(d, "dma_data_b     %d\n", dma_data_b);
  sprintf(d, "imask          %s\n", byte_to_bits(imask));
  sprintf(d, "intf           %s\n", byte_to_bits(intf));
  sprintf(d, "\n");

  dumpit(gb_out.addr,       "0x%04x");
  dumpit(gb_out.data,       "0x%02x");
  dumpit(gb_out.oe,         "%d");
  sprintf(d, "\n");

  dumpit(cpu_in.addr,       "0x%04x");
  dumpit(cpu_in.data,       "0x%02x");
  dumpit(cpu_in.oe,         "%d");
  sprintf(d, "cpu_in.imask   %s\n", byte_to_bits(cpu_in.imask));
  sprintf(d, "cpu_in.intf    %s\n", byte_to_bits(cpu_in.intf));
  sprintf(d, "\n");


  sprintf(d, "--------------CPU--------------\n");
  z80.dump(d);
  sprintf(d, "\n");

  sprintf(d, "--------------TIMER------------\n");
  timer.dump(d);
  sprintf(d, "\n");

  sprintf(d, "--------------MMU--------------\n");
  mmu.dump(d);
  sprintf(d, "\n");

  sprintf(d, "--------------OAM--------------\n");
  oam.dump(d);
  sprintf(d, "\n");

  sprintf(d, "--------------VRAM-------------\n");
  vram.dump(d);
  sprintf(d, "\n");

  sprintf(d, "--------------IRAM-------------\n");
  iram.dump(d);
  sprintf(d, "\n");

  sprintf(d, "--------------ZRAM-------------\n");
  zram.dump(d);
  sprintf(d, "\n");

  /*
  sprintf(d, "--------------BUTTONS----------\n");
  buttons.dump(d);
  sprintf(d, "\n");
  */

  /*
  sprintf(d, "--------------SERIAL-----------\n");
  serial.dump(d);
  */
}

//-----------------------------------------------------------------------------

void Gameboy::dump_disasm(std::string& out) {
  sprintf(out, "--------------DISASM-----------\n");

  uint16_t pc = z80.get_pc();
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
    segment = mmu.get_flat_ptr(pc);
  }

  if (segment) {
    Assembler a;
    a.disassemble(segment, 65536, pc, 30, out, false);
    sprintf(out, "\n");
  }
  else {
    sprintf(out, "(bad pc)\n");
  }
}

//-----------------------------------------------------------------------------