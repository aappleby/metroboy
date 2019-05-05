#include "Platform.h"
#include "Gameboy.h"

#include "Assembler.h"
#include "Common.h"
#include "Constants.h"

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

  compare_line = 0;

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

  old_stat_int = false;

  intf = 0xE1;
  imask = 0x00;
}

void Gameboy::reset(uint16_t new_pc) {
  reset(model, mmu.rom_size, new_pc);
}

//-----------------------------------------------------------------------------
// 4 mhz tick/tock

void Gameboy::tick() {
  tcycle++;
  int tphase = tcycle & 3;

  uint16_t cpu_addr_ = z80.mem_addr_;
  bool     cpu_write_ = z80.mem_write_ && (tphase == PHASE_CPU_WRITE);
  int old_hblank_delay = ppu.hblank_delay;

  //-----------------------------------
  // Tick the CPU using the updated interrupts and bus mux

  uint8_t bus_out_ = bus_out;
  uint8_t bus_oe_ = bus_oe;

  bus_out_ |= cpu_read_cart ? mmu.bus_out : 0x00;
  bus_out_ |= cpu_read_vram ? vram.bus_out : 0x00;
  bus_out_ |= cpu_read_iram ? iram.bus_out : 0x00;
  bus_out_ |= cpu_read_oam ? oam.bus_out : 0x00;

  bus_out_ |= ppu.bus_out;
  bus_out_ |= buttons.bus_out;
  bus_out_ |= serial.bus_out;
  bus_out_ |= spu.bus_out;
  bus_out_ |= timer.bus_out;
  bus_out_ |= zram.bus_out;

  bus_oe_ += cpu_read_cart;
  bus_oe_ += cpu_read_vram;
  bus_oe_ += cpu_read_iram;
  bus_oe_ += cpu_read_oam;

  bus_oe_ += ppu.bus_oe;
  bus_oe_ += buttons.bus_oe;
  bus_oe_ += serial.bus_oe;
  bus_oe_ += spu.bus_oe;
  bus_oe_ += timer.bus_oe;
  bus_oe_ += zram.bus_oe;

  assert(bus_oe_ <= 1);
  if (!bus_oe_) bus_out_ = 0xFF;

  if (tcycle == 0) {
    if (z80.pc == 0) {
      bus_out_ = DMG_ROM_bin[0];
      bus_oe_ = 1;
    }
    else {
      bus_out_ = rom_buf[z80.pc];
      bus_oe_ = 1;
    }
  }

  //-----------------------------------
  // Update counter/line/frame

  ppu.counterM2++;
  if (ppu.counterM2 == TCYCLES_LINE) {
    ppu.counterM2 = 0;
    ppu.lineM2++;
    if (ppu.lineM2 == 154) {
      ppu.lineM2 = 0;
    }
  }

  ppu.counter0++;
  if (ppu.counter0 == TCYCLES_LINE) {
    ppu.counter0 = 0;
    ppu.line0++;
    if (ppu.line0 == 154) {
      ppu.line0 = 0;
      ppu.frame_count++;
    }
  }

  ppu.counterP2++;
  if (ppu.counterP2 == TCYCLES_LINE) {
    ppu.counterP2 = 0;
    ppu.lineP2++;
    if (ppu.lineP2 == 154) {
      ppu.lineP2 = 0;
    }
  }

  ppu.frame_start = (ppu.counterM2 == 0) && (ppu.lineM2 == 0);
  ppu.frame_done = (ppu.counterM2 == 0) && (ppu.lineM2 == 144);

  bool vblank0 = ppu.lineP2 >= 144;

  bool vblank_int = false;
  bool stat_int_lyc = false;
  bool stat_int_oam = false;
  bool stat_int_vblank = false;
  bool stat_int_hblank = false;

  ppu.vblank_int = false;
  ppu.stat_int_lyc = false;
  ppu.stat_int_oam = false;
  ppu.stat_int_vblank = false;
  ppu.stat_int_hblank = false;

  //-----------------------------------
  // lyc_match

  if (tphase == 0) {
    compare_line = ppu.ly;

    if (ppu.lineP2 > 0 && ppu.counterP2 == 0) {
      ppu.ly = ppu.lineP2;
      compare_line = -1;
    }

    if (ppu.lineP2 == 153 && ppu.counterP2 == 4) {
      ppu.ly = 0;
    }

    if (ppu.lineP2 == 153 && ppu.counterP2 == 8) {
      compare_line = -1;
    }
  }

  if (ppu.lcdc & FLAG_LCD_ON) ppu.lyc_match = (compare_line == ppu.lyc);

  //----------------------------------------
  // Update state machiney stuff

  if (ppu.counterP2 == 0) {
    ppu.oam_phase = (ppu.frame_count != 0 || ppu.lineP2 != 0) && !vblank0;
    ppu.hblank_phase = false;
    ppu.sprite_index = -1;
    ppu.sprite_count = 0;
  }

  if (!vblank0 && ppu.counterP2 == 4) {
    if (ppu.frame_count != 0 || ppu.lineP2 != 0) ppu.state = PPU_STATE_OAM;
    ppu.pix_count = 0;
    ppu.hblank_delay = HBLANK_DELAY_START;
  }

  if (!vblank0 && ppu.counterP2 == 80) {
  }

  if (!vblank0 && ppu.counterP2 == 82) {
  }

  if (!vblank0 && ppu.counterP2 == 84) {
    ppu.oam_phase = false;
    ppu.render_phase = true;
    ppu.state = PPU_STATE_VRAM;

    ppu.sprite_index = -1;
    ppu.window_hit = false;
    ppu.map_x = (ppu.scx >> 3) & 31;
    ppu.pix_discard = (ppu.scx & 7) + 8;
    ppu.sprite_latched = false;
    ppu.tile_latched = false;
    ppu.window_hit = false;
    ppu.pipe_count = 0;
  }

  if (!vblank0 && ppu.counterP2 == 85) {
    ppu.tile_latched = true; // we always start w/ a "garbage" tile
  }

  if (!vblank0 && ppu.pix_count == 160 && ppu.hblank_delay) {
    ppu.hblank_delay--;
  }

  if (!vblank0 && ppu.hblank_delay == 6) {
    ppu.vram_lock = false;
  }

  if (!vblank0 && ppu.hblank_delay == 4) {
    ppu.render_phase = false;
    ppu.hblank_phase = true;
    ppu.state = PPU_STATE_HBLANK;

    ppu.vram_addr = 0;
    ppu.fetch_state = PPU::FETCH_IDLE;
  }

  //----------------------------------------

  if (ppu.frame_count == 0 && ppu.lineP2 == 0) {
    ppu.vram_lock = (ppu.counterP2 >= 84);
  }

  if (ppu.frame_count != 0 || ppu.line0 != 0) {
    ppu.vram_lock = (ppu.counterP2 >= 82);
  }

  if (ppu.hblank_delay < 6) {
    ppu.vram_lock = false;
  }

  ppu.oam_lock = (ppu.counterP2 >= 82);

  if (ppu.hblank_delay < 6) ppu.oam_lock = false;

  if (ppu.frame_count == 0 && ppu.lineP2 == 0 && ppu.counterP2 < 84) {
    ppu.oam_lock = false;
  }

  if (ppu.counterP2 < 80) {
    ppu.oam_lock = true;
  }

  ppu.stat = ubit8_t(0x80 | (ppu.stat & 0b01111000) | (ppu.lyc_match << 2) | ppu.state);

  //----------------------------------------

  ppu.vblank_int      |= (ppu.lineP2 == 144) && ppu.counterP2 == 4;
  ppu.stat_int_lyc    |= ppu.lyc_match;
  ppu.stat_int_oam    |= ((ppu.lineP2 <= 143) && (ppu.counterP2 == 0)) | vblank_int;
  ppu.stat_int_vblank |= (ppu.lineM2 >= 144);
  ppu.stat_int_hblank |= (old_hblank_delay < 6);

  vblank_int          |= ppu.lineP2 == 144 && ppu.counterP2 == 2;
  stat_int_lyc        |= ppu.lyc_match;
  stat_int_oam        |= ((ppu.lineP2 <= 143) && (ppu.counterP2 == 2)) | vblank_int;
  stat_int_vblank     |= (ppu.lineP2 >= 144);
  stat_int_hblank     |= (old_hblank_delay == 6);

  //----------

  bool stat_int_glitch = false;

  if (cpu_write_ && cpu_addr_ == ADDR_STAT) {
    stat_int_glitch |= ppu.stat_int_hblank;
    stat_int_glitch |= ppu.lineM2 >= 144;
    stat_int_glitch |= ppu.lyc_match;
  }

  ppu.stat_int_glitch = stat_int_glitch;

  //----------------------------------------
  // tick z80

  stat_int_lyc &= (ppu.stat & EI_LYC) != 0;
  stat_int_oam &= (ppu.stat & EI_OAM) != 0;
  stat_int_vblank &= (ppu.stat & EI_VBLANK) != 0;
  stat_int_hblank &= (ppu.stat & EI_HBLANK) != 0;

  if (imask & 0x01) {
    z80.unhalt |= vblank_int;
  }

  if (imask & 0x02) {
    z80.unhalt |= stat_int_lyc;
    z80.unhalt |= stat_int_oam;
    z80.unhalt |= stat_int_hblank;
    z80.unhalt |= stat_int_vblank;
    z80.unhalt |= stat_int_glitch;
  }

  if (imask & 0x04) {
    z80.unhalt |= timer.overflow;
  }

  if (imask & 0x10) {
    z80.unhalt |= buttons.val != 0xFF;
  }

  // TICK IS HERE
  if (tphase == PHASE_CPU_TICK) {
    z80.tick_t0(imask, intf, bus_out_);
  }
}

//-----------------------------------------------------------------------------

void Gameboy::tock() {
  int tphase = tcycle & 3;

  uint16_t cpu_addr_ = z80.mem_addr_;
  uint8_t  cpu_data_ = z80.mem_out_;
  bool     cpu_read_ = z80.mem_read_ && (tphase == PHASE_CPU_READ);
  bool     cpu_write_ = z80.mem_write_ && (tphase == PHASE_CPU_WRITE);

  bool lcd_on = (ppu.lcdc & FLAG_LCD_ON) != 0;
  bool vblank0 = ppu.line0 >= 144;

  //-----------------------------------

  if (!lcd_on) {
    ppu.handle_lcd_off();
  }

  if (vblank0) {

    ppu.hblank_phase = false;
    ppu.hblank_delay = HBLANK_DELAY_START;

    ppu.oam_lock = false;
    ppu.oam_addr = 0;
    ppu.oam_read = false;

    ppu.vram_lock = false;
    ppu.vram_addr = 0;

    ppu.stat = ubit8_t(0x80 | (ppu.stat & 0b01111000) | (ppu.lyc_match << 2) | PPU_STATE_VBLANK);
  }

  //-----------------------------------

  ppu.stat_int_lyc &= (ppu.stat & EI_LYC) != 0;
  ppu.stat_int_oam &= (ppu.stat & EI_OAM) != 0;
  ppu.stat_int_vblank &= (ppu.stat & EI_VBLANK) != 0;
  ppu.stat_int_hblank &= (ppu.stat & EI_HBLANK) != 0;

  ppu.stat_int = ppu.stat_int_lyc | ppu.stat_int_oam | ppu.stat_int_hblank | ppu.stat_int_vblank | ppu.stat_int_glitch;

  if (!lcd_on || vblank0) {
    ppu.vram_lock = false;
    ppu.oam_lock = false;
    if (cpu_write_) ppu.bus_write(cpu_addr_, cpu_data_);
    ppu.stat = ubit8_t(0x80 | (ppu.stat & 0b01111000) | (ppu.lyc_match << 2) | 1);
  }
  else {
    ppu.tock(cpu_addr_, cpu_data_, cpu_read_, cpu_write_, vram.bus_out, oam.bus_out);
  }

  bool new_stat_int = ppu.stat_int && !old_stat_int;
  old_stat_int = ppu.stat_int;

  if (ppu.vblank_int)      intf |= INT_VBLANK;
  if (new_stat_int)        intf |= INT_STAT;
  if (timer.overflow)      intf |= INT_TIMER;
  if (buttons.val != 0xFF) intf |= INT_JOYPAD;

  if (cpu_read_) {
    bus_out = 0x00;
    bus_oe = false;
    if (cpu_addr_ == ADDR_IF) { bus_out = intf; bus_oe = true; }
    if (cpu_addr_ == ADDR_IE) { bus_out = imask; bus_oe = true; }
  }

  bool ce_oam = (cpu_addr_ & 0xFF00) == 0xFE00;
  bool ce_zram = (cpu_addr_ & 0xFF00) == 0xFF00;

  int page = cpu_addr_ >> 13;

  bool ce_rom  = page <= 3;
  bool ce_vram = page == 4;
  bool ce_cram = page == 5;
  bool ce_iram = page == 6;
  bool ce_echo = page == 7 && !ce_oam && !ce_zram;

  //-----------------------------------
  // DMA state machine - tock can't be on t3, can't be after cpu_write_

  if (tphase == PHASE_DMA_TOCK) {
    dma_mode_b = dma_mode_a;
    dma_count_b = dma_count_a;
    if (dma_mode_a == DMA_CART) dma_data_b = mmu.bus_out;
    if (dma_mode_a == DMA_VRAM) dma_data_b = vram.bus_out;
    if (dma_mode_a == DMA_IRAM) dma_data_b = iram.bus_out;

    dma_mode_a = dma_mode_x;
    dma_count_a = dma_count_x;
    dma_source_a = dma_source_x;

    if (dma_count_x == 159) {
      dma_mode_x = DMA_NONE;
    }
    else {
      dma_count_x = dma_count_x + 1;
    }
  }

  uint16_t dma_read_addr = (dma_source_a << 8) | dma_count_a;
  bool     dma_write = dma_mode_b != DMA_NONE;
  uint16_t dma_write_addr = ADDR_OAM_BEGIN + dma_count_b;
  uint8_t  dma_write_data = dma_data_b;

  //-----------------------------------
  // oam bus mux

  if (dma_write) {
    cpu_read_oam = false;
    oam.tock(dma_write_addr, dma_write_data, false, true);
  }
  else if (ppu.oam_lock) {
    cpu_read_oam = false;
    oam.tock(ppu.oam_addr, ppu.oam_data, ppu.oam_read, false);
  }
  else {
    cpu_read_oam = cpu_read_ && ce_oam;
    oam.tock(cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  }

  //-----------------------------------
  // vram bus mux

  if (dma_mode_a == DMA_VRAM) {
    cpu_read_vram = false;
    vram.tock(dma_read_addr, 0, true, false);
  }
  else if (ppu.vram_lock) {
    cpu_read_vram = false;
    vram.tock(ppu.vram_addr, 0, ppu.vram_addr != 0, false);
  }
  else {
    cpu_read_vram = cpu_read_ && ce_vram;
    vram.tock(cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  }

  //-----------------------------------
  // iram bus mux

  if (dma_mode_a == DMA_IRAM) {
    cpu_read_iram = false;
    iram.tock_t3(dma_read_addr, 0, true, false);
  }
  else {
    cpu_read_iram = cpu_read_ && (ce_iram || ce_echo);
    iram.tock_t3(cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  }

  //-----------------------------------
  // cart bus mux

  if (dma_mode_a == DMA_CART) {
    cpu_read_cart = false;
    mmu.tock_t3(dma_read_addr, 0, true, false);
  }
  else {
    cpu_read_cart = cpu_read_ && (ce_rom || ce_cram);
    mmu.tock_t3(cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  }

  //-----------------------------------
  // everything else

  buttons.tock(cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  serial.tock(cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  timer.tock(tphase, cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  zram.tock(cpu_addr_, cpu_data_, cpu_read_, cpu_write_);
  spu.tock(tphase, cpu_addr_, cpu_data_, cpu_read_, cpu_write_);

  //-----------------------------------
  // Z80

  if (tphase == PHASE_CPU_TOCK) {
    z80.tock_t3();
    intf &= ~z80.int_ack_;
  }

  //-----------------------------------
  // bus write

  if (cpu_write_) {
    if (cpu_addr_ == ADDR_DMA) {
      if (cpu_data_ <= 0x7F) dma_mode_x = DMA_CART;
      if (0x80 <= cpu_data_ && cpu_data_ <= 0x9F) dma_mode_x = DMA_VRAM;
      if (0xA0 <= cpu_data_ && cpu_data_ <= 0xBF) dma_mode_x = DMA_CART;
      if (0xC0 <= cpu_data_ && cpu_data_ <= 0xFD) dma_mode_x = DMA_IRAM;
      dma_count_x = 0;
      dma_source_x = cpu_data_;
    }

    if (cpu_addr_ == ADDR_IF) {
      intf = cpu_data_ | 0b11100000;
    }
    if (cpu_addr_ == ADDR_IE) {
      imask = cpu_data_;
    }
  }

  ppu.bus_oe = 0;
  ppu.bus_out = 0;
  if (cpu_read_) ppu.bus_read(cpu_addr_);
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

char* Gameboy::dump(char* cursor) {
  //uint16_t pc = z80.get_pc();

  cursor = buttons.dump(cursor);

  cursor = z80.dump(cursor);
  cursor += sprintf(cursor, "imask %s\n", to_binary(imask));
  cursor += sprintf(cursor, "intf  %s\n", to_binary(intf));
  cursor += sprintf(cursor, "old stat int %d\n", old_stat_int);
  cursor += sprintf(cursor, "\n");

  cursor += sprintf(cursor, "tcycle %zd\n", tcycle);

  cursor += sprintf(cursor, "z80 mem addr 0x%04x\n", z80.mem_addr_);
  cursor += sprintf(cursor, "z80 mem data 0x%02x\n", z80.mem_out_);
  cursor += sprintf(cursor, "z80 mem read %d\n", z80.mem_read_);
  cursor += sprintf(cursor, "z80 mem write %d\n", z80.mem_write_);

  //cursor += sprintf(cursor, "bus out2 %02x\n", bus_out2);
  //cursor += sprintf(cursor, "bus oe2 %d\n", bus_oe2);
  cursor += sprintf(cursor, "\n");

  cursor += sprintf(cursor, "dma mode a %d\n", dma_mode_a);
  cursor += sprintf(cursor, "dma count a %d\n", dma_count_a);
  cursor += sprintf(cursor, "dma source a 0x%04x\n", dma_source_a);
  cursor += sprintf(cursor, "\n");

  cursor += sprintf(cursor, "dma mode b %d\n", dma_mode_b);
  cursor += sprintf(cursor, "dma count b %d\n", dma_count_b);
  cursor += sprintf(cursor, "dma data b %d\n", dma_data_b);

  cursor += sprintf(cursor, "\n");

  cursor = timer.dump(cursor);
  cursor += sprintf(cursor, "\n");

  cursor = ppu.dump(cursor);
  cursor += sprintf(cursor, "\n");

  /*
  cursor = spu.dump(cursor);
  cursor += sprintf(cursor, "\n");

  {
    uint8_t* ram = ppu.get_oam_ram();
    for (int i = 0; i < 16; i++) {
      cursor += sprintf(cursor, "%02x ", ram[i]);
    }
    cursor += sprintf(cursor, "\n");
  }

  {
    uint8_t* ram = vram.ram;
    for (int i = 0; i < 16; i++) {
      cursor += sprintf(cursor, "%02x ", ram[i]);
    }
    cursor += sprintf(cursor, "\n");
  }
  */

  return cursor;
}

//-----------------------------------------------------------------------------

char* Gameboy::dump_disasm(char* cursor) {
  uint16_t pc = z80.get_pc();

  cursor += sprintf(cursor, "---disasm---\n");
  uint8_t* flat = mmu.get_flat_ptr(pc);
  if (flat != nullptr) {
    cursor += disassemble(flat, pc, 0, 30, cursor);
  }
  else if (ADDR_IRAM_BEGIN <= pc && pc <= ADDR_IRAM_END) {
    cursor += disassemble(iram.get(), ADDR_IRAM_BEGIN, pc - ADDR_IRAM_BEGIN, 30, cursor);
  }
  else if (ADDR_ZEROPAGE_BEGIN <= pc && pc <= ADDR_ZEROPAGE_END) {
    cursor += disassemble(zram.get(), ADDR_ZEROPAGE_BEGIN, pc - ADDR_ZEROPAGE_BEGIN, 30, cursor);
  }
  else if (ADDR_OAM_BEGIN <= pc && pc <= ADDR_OAM_END) {
    cursor += disassemble(oam.ram, ADDR_OAM_BEGIN, pc - ADDR_OAM_BEGIN, 30, cursor);
  }
  else {
    cursor += sprintf(cursor, "(bad pc)\n");
  }
  cursor += sprintf(cursor, "\n");

  return cursor;
}

//-----------------------------------------------------------------------------