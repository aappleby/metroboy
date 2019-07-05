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
  ppu.reset(new_pc == 0, new_model);
  oam.reset();
  mmu.reset(new_rom_size, new_pc);
  timer.reset();
  vram.reset();
  iram.reset();
  buttons.reset();
  serial.reset();
  zram.reset();
  spu.reset();

  model = new_model;
  tcycle = -1;

  cpu_read_oam = false;
  cpu_read_vram = false;
  cpu_read_iram = false;
  cpu_read_cart = true;

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

  memset(framebuffer, 0, 160 * 144);
}

void Gameboy::reset(uint16_t new_pc) {
  reset(model, mmu.rom_size, new_pc);
}

//-----------------------------------------------------------------------------
// 4 mhz tick/tock

void Gameboy::tick() {
  tcycle++;
  int tphase = tcycle & 3;

  ppu.tick(tphase, cpu_bus2);

  //----------------------------------------
  // tick z80

  if (tphase == 3) {
    uint8_t bus_out_ = bus_out;
    uint8_t bus_oe_ = bus_oe;

    bus_out_ |= cpu_read_cart ? mmu_out.data : 0x00;
    bus_out_ |= cpu_read_vram ? vram_out.data : 0x00;
    bus_out_ |= cpu_read_iram ? iram_out.data : 0x00;
    bus_out_ |= cpu_read_oam ? oam_out.data : 0x00;

    bus_out_ |= ppu.bus_out;
    bus_out_ |= buttons_out.data;
    bus_out_ |= serial_out.data;
    bus_out_ |= spu_out.data;
    bus_out_ |= timer_out.data;
    bus_out_ |= zram_out.data;

    bus_oe_ += cpu_read_cart;
    bus_oe_ += cpu_read_vram;
    bus_oe_ += cpu_read_iram;
    bus_oe_ += cpu_read_oam;

    bus_oe_ += ppu.bus_oe;
    bus_oe_ += buttons_out.oe;
    bus_oe_ += serial_out.oe;
    bus_oe_ += spu_out.data;
    bus_oe_ += timer_out.oe;
    bus_oe_ += zram_out.oe;

    assert(bus_oe_ <= 1);
    if (!bus_oe_) bus_out_ = 0xFF;

    bus_in = bus_out_;
  }

  if (tphase == 0) {
    if (imask & 0x01) z80.unhalt |= (ppu.get_line() == 144 && ppu.get_counter() == 4);
    if (imask & 0x02) z80.unhalt |= ppu.new_stat_int != 0;
    if (imask & 0x04) z80.unhalt |= (timer.overflow) ? true : false;
    if (imask & 0x10) z80.unhalt |= (buttons.get() != 0xFF) ? true : false;

    if (tcycle == 0) {
      if (z80.get_pc() == 0) {
        bus_in = DMG_ROM_bin[0];
      }
      else {
        bus_in = rom_buf[z80.get_pc()];
      }
    }

    cpu_bus2 = z80.tick_t0(imask, intf, bus_in);
  }
}

//-----------------------------------------------------------------------------

void Gameboy::tock() {
  int tphase = tcycle & 3;

  CpuBus cpu_bus = {
    cpu_bus2.addr,
    cpu_bus2.data,
    cpu_bus2.read && (tphase == 2),
    cpu_bus2.write && (tphase == 0),
  };

  if ((ppu.stat & ppu.stat_int) && !ppu.old_stat_int) intf |= INT_STAT;
  if (tphase == 0) ppu.old_stat_int = (ppu.stat & ppu.stat_int);


  if (ppu.get_line() == 144 && ppu.get_counter() == 4) intf |= INT_VBLANK;
  if (timer.overflow)      intf |= INT_TIMER;
  if (buttons.get() != 0xFF) intf |= INT_JOYPAD;

  if (cpu_bus.read) {
    bus_out = 0x00;
    bus_oe = false;
    if (cpu_bus.addr == ADDR_IF) { bus_out = 0b11100000 | intf; bus_oe = true; }
    if (cpu_bus.addr == ADDR_IE) { bus_out = imask; bus_oe = true; }
  }

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

  uint8_t dma_data = 0;
  if (dma_mode_b == DMA_CART) dma_data = mmu_out.data;
  if (dma_mode_b == DMA_VRAM) dma_data = vram_out.data;
  if (dma_mode_b == DMA_IRAM) dma_data = iram_out.data;

  //-----------------------------------

  bool ce_oam = (cpu_bus.addr & 0xFF00) == 0xFE00;
  bool ce_zram = (cpu_bus.addr & 0xFF00) == 0xFF00;

  int page = cpu_bus.addr >> 13;

  bool ce_rom = page <= 3;
  bool ce_vram = page == 4;
  bool ce_cram = page == 5;
  bool ce_iram = page == 6;
  bool ce_echo = page == 7 && !ce_oam && !ce_zram;

  CpuBus dma_bus = { dma_read_addr, 0, true, false };

  //-----------------------------------
  // oam bus mux

  if (dma_mode_b != DMA_NONE) {
    cpu_read_oam = false;
    if (tphase == 0) {
      oam_out = oam.tock({ dma_write_addr, dma_data, false, true });
    }
    else {
      oam_out = oam.tock({ 0, 0, false, false });
    }
  }
  else {
    // Dirty hack - on tcycle 0 of a line, cpu write takes precendence over ppu read.
    if (ppu.get_counter() == 0) {
      if (cpu_bus.write && (cpu_bus.addr & 0xFF00) == 0xFE00) {
        cpu_read_oam = cpu_bus.read && ce_oam;
        oam_out = oam.tock(cpu_bus);
      }
      else {
        cpu_read_oam = false;
        oam_out = oam.tock({ ppu.oam_addr, ppu.oam_data, ppu.oam_read, false });
      }
    }
    else {
      if (ppu.oam_lock) {
        cpu_read_oam = false;
        oam_out = oam.tock({ ppu.oam_addr, ppu.oam_data, ppu.oam_read, false });
      }
      else {
        cpu_read_oam = cpu_bus.read && ce_oam;
        oam_out = oam.tock(cpu_bus);
      }
    }
  }

  //-----------------------------------
  // vram bus mux

  cpu_read_vram = (dma_mode_a != DMA_VRAM) && !ppu.vram_lock && cpu_bus.read && ce_vram;
  cpu_read_iram = (dma_mode_a != DMA_IRAM) && cpu_bus.read && (ce_iram || ce_echo);
  cpu_read_cart = (dma_mode_a != DMA_CART) && cpu_bus.read && (ce_rom || ce_cram);

  {
    CpuBus ppu_bus = { ppu.vram_addr, 0, ppu.vram_addr != 0, false };
    vram_out = vram.tock(dma_mode_a == DMA_VRAM ? dma_bus : ppu.vram_lock ? ppu_bus : cpu_bus);
  }
  iram_out = iram.tock_t2(dma_mode_a == DMA_IRAM ? dma_bus : cpu_bus);
  mmu_out = mmu.tock_t2(dma_mode_a == DMA_CART ? dma_bus : cpu_bus);

  buttons_out = buttons.tock(cpu_bus);
  serial_out = serial.tock(cpu_bus);
  zram_out = zram.tock(cpu_bus);
  spu_out = spu.tock(tphase, cpu_bus);
  timer_out = timer.tock(tphase, cpu_bus);

  //-----------------------------------
  // Z80

  if (tphase == 2) {
    z80.tock_t2();
    intf &= ~z80.int_ack_;
  }

  //-----------------------------------
  // bus write

  if (cpu_bus.write) {
    if (cpu_bus.addr == ADDR_DMA) {
      //printf("%8lld: Starting dma from 0x%02x00\n", tcycle, cpu_bus.data);

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

  //-----------------------------------

  bool lcd_on = (ppu.get_lcdc() & FLAG_LCD_ON) != 0;

  // FIXME should not be tocking w/ vram out here, it just got tocked

  if (!lcd_on) {
    ppu.tock_lcdoff(tphase, cpu_bus, vram_out, oam_out);
  }
  else {
    ppu.tock(tphase, cpu_bus, vram_out, oam_out);
  }
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

  return ppu.vram_addr; // this one's pretty cool
}

//-----------------------------------------------------------------------------

void Gameboy::dump(std::string& out) {
  //uint16_t pc = z80.get_pc();

  buttons.dump(out);

  z80.dump(out);
  sprintf(out, "imask %s\n", to_binary(imask));
  sprintf(out, "intf  %s\n", to_binary(intf));
  sprintf(out, "old stat int %d\n", ppu.old_stat_int);
  sprintf(out, "\n");

  sprintf(out, "tcycle %zd\n", tcycle);

  sprintf(out, "z80 mem addr 0x%04x\n", cpu_bus2.addr);
  sprintf(out, "z80 mem data 0x%02x\n", cpu_bus2.data);
  sprintf(out, "z80 mem read %d\n", cpu_bus2.read);
  sprintf(out, "z80 mem write %d\n", cpu_bus2.write);

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

  ppu.dump(out);
  sprintf(out, "\n");

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
    segment = zram.ram + (pc - ADDR_ZEROPAGE_BEGIN);
  }
  else if (ADDR_OAM_BEGIN <= pc && pc <= ADDR_OAM_END) {
    segment = oam.ram + (pc - ADDR_OAM_BEGIN);
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