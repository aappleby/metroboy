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

  stat_int = 0;
  stat_int2 = 0;
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

  //-----------------------------------
  // Update counter/line/frame

  ppu.counterP2++;
  if (ppu.counterP2 == TCYCLES_LINE) {
    ppu.counterP2 = 0;
    ppu.lineP2++;
    if (ppu.lineP2 == 154) {
      ppu.lineP2 = 0;
      ppu.frame_count++;
    }
  }

  //-----------------------------------

  const int oam_start = 0;
  const int oam_end = 80;
  const int render_start = 82;
  const int render_start_l0 = 84;

  bool weird_line = ppu.frame_count == 0 && ppu.lineP2 == 0;

  if (tphase == 0) {

    //-----------------------------------

    bool vblank = ppu.lineP2 > 143;
    ppu.frame_start = (ppu.counterP2 == 0) && (ppu.lineP2 == 0);
    ppu.frame_done = (ppu.counterP2 == 0) && (ppu.lineP2 == 144);

    //-----------------------------------
    // lyc_match

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

    //----------------------------------------
    // Update state machiney stuff

    int old_hblank_delay = ppu.hblank_delay;

    if (!vblank && ppu.counterP2 == 0) {
      ppu.oam_phase = !weird_line && !(ppu.lineP2 > 143);
      ppu.hblank_phase = false;
      ppu.sprite_index = -1;
      ppu.sprite_count = 0;
    }

    if (!vblank && ppu.counterP2 == 4) {
      if (!weird_line) ppu.state = PPU_STATE_OAM;
      ppu.pix_count = 0;
      ppu.hblank_delay = HBLANK_DELAY_START;
    }

    if (!vblank && ppu.counterP2 == 84) {
      ppu.hblank_phase = false;
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

    if (!vblank && ppu.hblank_delay < 7) {
      ppu.render_phase = false;
      ppu.hblank_phase = true;
      ppu.state = PPU_STATE_HBLANK;

      ppu.vram_addr = 0;
      ppu.fetch_state = PPU::FETCH_IDLE;
    }

    ppu.stat &= 0b11111000;
    ppu.stat |= ppu.state;
    ppu.stat |= (compare_line == ppu.lyc) << 2;

    //----------------------------------------
    // locking

    if (!weird_line && ppu.counterP2 == oam_start) ppu.oam_lock = true;
    if (!weird_line && ppu.counterP2 == oam_end)   ppu.oam_lock = false;

    if (weird_line && ppu.counterP2 == render_start_l0) ppu.oam_lock = true;
    if (weird_line && ppu.counterP2 == render_start_l0) ppu.vram_lock = true;

    if (ppu.hblank_delay == 6 || vblank) {
      ppu.oam_lock = false;
      ppu.vram_lock = false;
    }

    //----------------------------------------

    stat_int &= ~EI_VBLANK;
    if (ppu.lineP2 == 144 && ppu.counterP2 == 4) stat_int |= (ppu.stat & EI_VBLANK);

    stat_int &= ~EI_HBLANK;
    if (ppu.hblank_delay < 7) stat_int |= (ppu.stat & EI_HBLANK);

    if (ppu.lcdc & FLAG_LCD_ON) {
      stat_int &= ~EI_LYC;
      if (compare_line == ppu.lyc) stat_int |= (ppu.stat & EI_LYC);
    }

    stat_int2 = stat_int;

    stat_int &= ~EI_OAM;
    if (!weird_line && ppu.lineP2 <= 143 && ppu.counterP2 == 0) stat_int |= (ppu.stat & EI_OAM);

    // stat int glitch

    bool stat_int_glitch = false;

    if (z80.mem_write_ && z80.mem_addr_ == ADDR_STAT) {
      stat_int_glitch |= old_hblank_delay < 6;
      stat_int_glitch |= vblank;
      stat_int_glitch |= (compare_line == ppu.lyc);
    }

    stat_int &= ~0x80;
    stat_int |= stat_int_glitch ? 0x80 : 0;
  }
  else if (tphase == 1) {
    // FIXME this is weird
    if (ppu.counterP2 == 85) ppu.tile_latched = true;
  }
  else if (tphase == 2) {
    if (!weird_line && ppu.counterP2 == render_start) {
      ppu.oam_lock = true;
      ppu.vram_lock = true;
    }

    if (ppu.lcdc & FLAG_LCD_ON) {
      stat_int &= ~EI_LYC;
      if (compare_line == ppu.lyc) stat_int |= (ppu.stat & EI_LYC);
    }

    stat_int &= ~EI_HBLANK;
    if (ppu.hblank_delay < 7) stat_int |= (ppu.stat & EI_HBLANK);
  }


  if (ppu.pix_count == 160 && ppu.hblank_delay) {
    ppu.hblank_delay--;
  }

  //----------------------------------------
  // tick z80

  if (tphase == PHASE_CPU_TICK) {

    uint8_t bus_out_ = bus_out;
    uint8_t bus_oe_ = bus_oe;

    if (tphase == PHASE_CPU_TICK) {
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
    }

    if (imask & 0x01) z80.unhalt |= (ppu.lineP2 == 144 && ppu.counterP2 == 4) ? true : false;
    if (imask & 0x02) z80.unhalt |= stat_int2 != 0;
    if (imask & 0x04) z80.unhalt |= (timer.overflow) ? true : false;
    if (imask & 0x10) z80.unhalt |= (buttons.val != 0xFF) ? true : false;

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

  if (tphase == PHASE_CPU_READ) {
    if (cpu_read_) {
      bus_out = 0x00;
      bus_oe = false;
      if (cpu_addr_ == ADDR_IF) { bus_out = intf; bus_oe = true; }
      if (cpu_addr_ == ADDR_IE) { bus_out = imask; bus_oe = true; }
    }
  }

  if (ppu.lineP2 == 144 && ppu.counterP2 == 4) intf |= INT_VBLANK;
  if (stat_int && !old_stat_int) intf |= INT_STAT;
  if (timer.overflow)      intf |= INT_TIMER;
  if (buttons.val != 0xFF) intf |= INT_JOYPAD;
  old_stat_int = stat_int;

  bool lcd_on = (ppu.lcdc & FLAG_LCD_ON) != 0;

  //-----------------------------------

  if (!lcd_on) {
    ppu.tock_lcdoff(tphase, cpu_addr_, cpu_data_, cpu_read_, cpu_write_, vram.bus_out, oam.bus_out);
  }
  else {
    ppu.tock(tphase, cpu_addr_, cpu_data_, cpu_read_, cpu_write_, vram.bus_out, oam.bus_out);
  }

  //-----------------------------------
  // DMA state machine

  if (tphase == PHASE_DMA_READ) {
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

  bool ce_oam = (cpu_addr_ & 0xFF00) == 0xFE00;
  bool ce_zram = (cpu_addr_ & 0xFF00) == 0xFF00;

  int page = cpu_addr_ >> 13;

  bool ce_rom = page <= 3;
  bool ce_vram = page == 4;
  bool ce_cram = page == 5;
  bool ce_iram = page == 6;
  bool ce_echo = page == 7 && !ce_oam && !ce_zram;

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