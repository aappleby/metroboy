#include "Gameboy.h"

#include "Assembler.h"
#include "Constants.h"

#include <assert.h>

extern uint8_t rom_buf[];
extern const uint8_t DMG_ROM_bin[];
const char* to_binary(uint8_t b);

//-----------------------------------------------------------------------------

Gameboy::Gameboy()
: z80(),
  mmu(),
  ppu(),
  oam(),
  spu(),
  timer(),
  vram(),
  iram(),
  buttons(),
  serial(),
  zram()
{
  reset(MODEL_DMG, 0, 0);
}

void Gameboy::reset(int new_model, size_t new_rom_size, uint16_t new_pc) {
  z80.reset(new_model, new_pc);
  ppu_out = ppu.reset(new_pc == 0, new_model);
  
  oam_out = oam.reset();
  mmu_out = mmu.reset(new_rom_size, new_pc);
  timer_out = timer.reset();
  vram_out = vram.reset();
  iram_out = iram.reset();
  buttons_out = buttons.reset();
  serial_out = serial.reset();
  zram_out = zram.reset();
  spu_out = spu.reset();

  model = new_model;
  tcycle = -1;

  cpu_read_oam = false;

  dma_mode_x = DMA_NONE;
  dma_count_x = 0;
  dma_source_x = 0x000;

  dma_mode_a = DMA_NONE;
  dma_count_a = 0;
  dma_source_a = 0x000;

  dma_mode_b = DMA_NONE;
  dma_count_b = 0;
  dma_data_b = 0x00;

  bus_out = 0;
  bus_oe = false;

  intf = 0xE1;
  imask = 0x00;

  cpu_bus = {0};

  memset(framebuffer, 0, 160 * 144);
}

void Gameboy::reset(uint16_t new_pc) {
  reset(model, mmu.get_rom_size(), new_pc);
}

//-----------------------------------------------------------------------------
// 4 mhz tick/tock

void Gameboy::tick() {
  tcycle++;
  int tphase = tcycle & 3;

  //----------------------------------------

  PpuOut ppu_tick_out = ppu.tick(tphase, cpu_bus);

  
  if ((tphase == 0) || (tphase == 2)) {
    bool fire_stat_oam1 =
      (ppu.line > 0) &&
      (ppu.line < 144) &&
      (ppu.counter == 0);

    bool fire_stat_hblank1 = ppu.hblank_delay2 <= 6;
    bool fire_stat_vblank1 = (ppu.line == 144 && ppu.counter >= 4) || (ppu.line >= 145);
    bool fire_stat_lyc1 = ppu.compare_line == ppu.lyc;
    bool fire_stat_glitch1 = cpu_bus.write && cpu_bus.addr == ADDR_STAT && ppu.stat_int1 != 0;

    bool fire_stat_oam2 =
      ((ppu.line == 0 && ppu.counter == 4) || (ppu.line > 0 && ppu.line <= 144 && ppu.counter == 4));

    //bool fire_stat_oam2 = (ppu.line > 0) && (ppu.line < 144) && (ppu.counter == 0);

    bool fire_stat_hblank2 = ppu.hblank_delay2 <= 6;
    bool fire_stat_vblank2 = (ppu.line == 144 && ppu.counter >= 4) || (ppu.line >= 145);
    bool fire_stat_lyc2 = ppu.compare_line == ppu.lyc;
    bool fire_stat_glitch2 = cpu_bus.write && cpu_bus.addr == ADDR_STAT && ppu.stat_int2 != 0;

    if (ppu.lcdc & FLAG_LCD_ON) {
      if (tphase == 0) {
        ppu.stat &= ~STAT_LYC;
        ppu.stat_int1 = 0;
        ppu.stat_int2 = 0;
      }
    }
    else {
      fire_stat_oam1 = false;
      fire_stat_hblank1 = false;
      fire_stat_vblank1 = false;
      fire_stat_lyc1 = false;
      fire_stat_glitch1 = false;
    }

    if (tphase == 0 || tphase == 2) {
      if (fire_stat_lyc1)    ppu.stat |= STAT_LYC;

      if (fire_stat_hblank1) ppu.stat_int1 |= EI_HBLANK;
      if (fire_stat_vblank1) ppu.stat_int1 |= EI_VBLANK;
      if (fire_stat_lyc1)    ppu.stat_int1 |= EI_LYC;
      if (fire_stat_glitch1) ppu.stat_int1 |= EI_GLITCH;
      if (fire_stat_oam1)    ppu.stat_int1 |= EI_OAM;

      if (fire_stat_hblank2) ppu.stat_int2 |= EI_HBLANK;
      if (fire_stat_vblank2) ppu.stat_int2 |= EI_VBLANK;
      if (fire_stat_lyc2)    ppu.stat_int2 |= EI_LYC;
      if (fire_stat_glitch2) ppu.stat_int2 |= EI_GLITCH;
      if (fire_stat_oam2)    ppu.stat_int2 |= EI_OAM;
    }

    bool fire_int_vblank1 = ppu.line == 144 && ppu.counter == 4;
    bool fire_int_stat1    = ((ppu.stat & ppu.stat_int1) && !old_stat_int1);
    bool fire_int_timer1   = timer_out.interrupt;
    bool fire_int_buttons1 = buttons_out.val != 0xFF;

    //bool fire_int_vblank2 = ppu.line == 144 && ppu.counter == 4;
    bool fire_int_stat2 = ((ppu.stat & ppu.stat_int2) && !old_stat_int2);
    //bool fire_int_timer2 = timer_out.overflow;
    //bool fire_int_buttons2 = buttons_out.val != 0xFF;

    if (tphase == 0 || tphase == 2) {
      if (imask & 0x01) z80.unhalt |= fire_int_vblank1;
      if (imask & 0x02) z80.unhalt |= fire_int_stat2;
      if (imask & 0x04) z80.unhalt |= fire_int_timer1;
      if (imask & 0x10) z80.unhalt |= fire_int_buttons1;
    }

    if (fire_int_timer1)   intf |= INT_TIMER;
    if (fire_int_stat1)    intf |= INT_STAT;
    if (fire_int_vblank1)  intf |= INT_VBLANK;
    if (fire_int_buttons1) intf |= INT_JOYPAD;

    //----------------------------------------
    // tick z80

    if (tphase == 0) {
      cpu_bus = z80.tick(imask, intf, bus_in);
      intf &= ~z80.int_ack_;
    }

    //----------------------------------------

    old_stat_int1 = (ppu.stat & ppu.stat_int1);
    old_stat_int2 = (ppu.stat & ppu.stat_int2);
  }
}

//-----------------------------------------------------------------------------

GameboyOut Gameboy::tock() {
  int tphase = tcycle & 3;

  ppu_out = ppu.tock(tphase, cpu_bus, vram_out, oam_out);

  CpuBus ppu_bus = { ppu_out.vram_addr, 0, ppu_out.vram_read, false };

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
  }

  uint16_t dma_read_addr = (dma_source_a << 8) | dma_count_a;
  uint16_t dma_write_addr = ADDR_OAM_BEGIN + dma_count_b;

  //-----------------------------------

  int page = cpu_bus.addr >> 13;

  bool ce_rom  = page <= 3;
  bool ce_vram = page == 4;
  bool ce_cram = page == 5;
  bool ce_iram = page == 6;
  bool ce_echo = page == 7 && (cpu_bus.addr < 0xFE00);
  bool ce_oam  = (cpu_bus.addr & 0xFF00) == 0xFE00;

  //-----------------------------------
  // oam bus mux

  CpuBus oam_bus = { ppu_out.oam_addr, 0, ppu_out.oam_read, false };
  cpu_read_oam = false;

  if (dma_mode_b != DMA_NONE) {
    if (tphase == 0) {
      uint8_t dma_data = 0;
      if (dma_mode_b == DMA_CART) dma_data = mmu_out.data;
      if (dma_mode_b == DMA_VRAM) dma_data = vram_out.data;
      if (dma_mode_b == DMA_IRAM) dma_data = iram_out.data;
      oam_bus = { dma_write_addr, dma_data, false, true };
    }
    else {
      oam_bus = { 0, 0, false, false };
    }
  }
  else {
    // Dirty hack - on tcycle 0 of a line, cpu write takes precendence over ppu read.
    if (ppu_out.counter == 0) {
      if (cpu_bus.write && ce_oam) {
        cpu_read_oam = cpu_bus.read && ce_oam;
        oam_bus = cpu_bus;
      }
    }
    else {
      if (!ppu_out.oam_lock) {
        cpu_read_oam = cpu_bus.read && ce_oam;
        oam_bus = cpu_bus;
      }
    }
  }

  oam_out = oam.tock(oam_bus);

  //-----------------------------------
  // vram bus mux

  CpuBus dma_bus = { dma_read_addr, 0, true, false };

  vram_out    = vram.tock(dma_mode_a == DMA_VRAM ? dma_bus : ppu_out.vram_lock ? ppu_bus : cpu_bus);
  iram_out    = iram.tock_t2(dma_mode_a == DMA_IRAM ? dma_bus : cpu_bus);
  mmu_out     = mmu.tock_t2(dma_mode_a == DMA_CART ? dma_bus : cpu_bus);
  buttons_out = buttons.tock(cpu_bus);
  serial_out  = serial.tock(cpu_bus);
  zram_out    = zram.tock(cpu_bus);
  spu_out     = spu.tock(tphase, cpu_bus);
  timer_out   = timer.tock(tphase, cpu_bus);

  //-----------------------------------
  // writes happen on t0

  if (tphase == 0) {
    if (cpu_bus.write) {
      if (cpu_bus.addr == ADDR_DMA) {
        if (cpu_bus.data <= 0x7F) dma_mode_x = DMA_CART;
        if (0x80 <= cpu_bus.data && cpu_bus.data <= 0x9F) dma_mode_x = DMA_VRAM;
        if (0xA0 <= cpu_bus.data && cpu_bus.data <= 0xBF) dma_mode_x = DMA_CART;
        if (0xC0 <= cpu_bus.data && cpu_bus.data <= 0xFD) dma_mode_x = DMA_IRAM;
        dma_count_x = 0;
        dma_source_x = cpu_bus.data;
      }

      if (cpu_bus.addr == ADDR_IF) {
        intf = cpu_bus.data | 0b11100000;
      }
      if (cpu_bus.addr == ADDR_IE) {
        imask = cpu_bus.data;
      }
    }
  }

  //-----------------------------------
  // reads happen on t2

  if (tphase == 2) {
    if (cpu_bus.read) {
      bus_out = 0x00;
      bus_oe = false;
      if (cpu_bus.addr == ADDR_IF) { 
        bus_out = 0b11100000 | intf; 
        bus_oe = true;
      }
      if (cpu_bus.addr == ADDR_IE) {
        bus_out = imask;
        bus_oe = true;
      }
    }

    uint8_t bus_out_ = bus_out;
    uint8_t bus_oe_ = bus_oe;

    bool cpu_read_vram = (dma_mode_a != DMA_VRAM) && !ppu_out.vram_lock && cpu_bus.read && ce_vram;
    bool cpu_read_iram = (dma_mode_a != DMA_IRAM) && cpu_bus.read && (ce_iram || ce_echo);
    bool cpu_read_cart = (dma_mode_a != DMA_CART) && cpu_bus.read && (ce_rom || ce_cram);

    bus_out_ |= cpu_read_cart ? mmu_out.data : 0x00;
    bus_out_ |= cpu_read_vram ? vram_out.data : 0x00;
    bus_out_ |= cpu_read_iram ? iram_out.data : 0x00;
    bus_out_ |= cpu_read_oam ? oam_out.data : 0x00;

    bus_out_ |= ppu_out.data;
    bus_out_ |= buttons_out.data;
    bus_out_ |= serial_out.data;
    bus_out_ |= spu_out.data;
    bus_out_ |= timer_out.data;
    bus_out_ |= zram_out.data;

    bus_oe_ += cpu_read_cart;
    bus_oe_ += cpu_read_vram;
    bus_oe_ += cpu_read_iram;
    bus_oe_ += cpu_read_oam;

    bus_oe_ += ppu_out.oe;
    bus_oe_ += buttons_out.oe;
    bus_oe_ += serial_out.oe;
    bus_oe_ += spu_out.oe;
    bus_oe_ += timer_out.oe;
    bus_oe_ += zram_out.oe;

    assert(bus_oe_ <= 1);
    if (!bus_oe_) bus_out_ = 0xFF;

    bus_in = bus_out_;
  }

  //-----------------------------------

  return {
    ppu_out.x,
    ppu_out.y,
    ppu_out.counter,
    ppu_out.pix_out,
    ppu_out.pix_oe,
    spu_out.out_r,
    spu_out.out_l,
  };
}

//-----------------------------------------------------------------------------

uint32_t Gameboy::trace() {

  //return z80.state << 4;

  //return z80.op_ == 0x76 ? 0xFFFFFFFF : 0; // moderately interesting
  //return z80.op_ == 0x00 ? 0xFFFFFFFF : 0; // moderately interesting
  //return z80.op_ == 0xcb ? 0xFFFFFFFF : 0; // moderately interesting
  //return (z80.op_ & 0b11000000) == 0b10000000 ? 0xFFFFFFFF : 0; // moderately interesting

  //return (z80.mem_addr >= ADDR_SPU_BEGIN && z80.mem_addr < ADDR_SPU_END) ? -1 : 0; // sparse

  //return ppu.sprite_index << 4; // also pretty cool

  return ppu_out.vram_addr; // this one's pretty cool
}

//-----------------------------------------------------------------------------

void Gameboy::dump(std::string& out) {
  //uint16_t pc = z80.get_pc();

  buttons.dump(out);

  z80.dump(out);
  sprintf(out, "imask %s\n", to_binary(imask));
  sprintf(out, "intf  %s\n", to_binary(intf));
  sprintf(out, "old stat int1 %d\n", old_stat_int1);
  sprintf(out, "old stat int2 %d\n", old_stat_int2);
  sprintf(out, "\n");

  sprintf(out, "tcycle %zd\n", tcycle);

  sprintf(out, "z80 mem addr 0x%04x\n", cpu_bus.addr);
  sprintf(out, "z80 mem data 0x%02x\n", cpu_bus.data);
  sprintf(out, "z80 mem read %d\n", cpu_bus.read);
  sprintf(out, "z80 mem write %d\n", cpu_bus.write);

  //sprintf(out, "bus out2 %02x\n", bus_out2);
  //sprintf(out, "bus oe2 %d\n", bus_oe2);
  sprintf(out, "\n");

  sprintf(out, "dma mode a %d\n", dma_mode_a);
  sprintf(out, "dma count a %d\n", dma_count_a);
  sprintf(out, "dma source a 0x%04x\n", dma_source_a);
  sprintf(out, "\n");

  sprintf(out, "dma mode b %d\n", dma_mode_b);
  sprintf(out, "dma count b %d\n", dma_count_b);
  sprintf(out, "dma data b %d\n", dma_data_b);

  sprintf(out, "\n");

  timer.dump(out);
  sprintf(out, "\n");

  //ppu.dump(out);
  //sprintf(out, "\n");

  /*
  {
    uint8_t* ram = ppu.get_oam_ram();
    for (int i = 0; i < 16; i++) {
      sprintf(out, "%02x ", ram[i]);
    }
    sprintf(out, "\n");
  }

  {
    uint8_t* ram = vram.ram;
    for (int i = 0; i < 16; i++) {
      sprintf(out, "%02x ", ram[i]);
    }
    sprintf(out, "\n");
  }
  */
}

//-----------------------------------------------------------------------------

void Gameboy::dump_disasm(std::string& out) {
  sprintf(out, "---disasm---\n");

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