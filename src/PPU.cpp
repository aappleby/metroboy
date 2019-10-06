#include "PPU.h"

#include "Constants.h"

extern const uint32_t gb_colors[];

const char* to_binary(uint8_t b);

uint16_t pack_map_addr(uint16_t base, uint8_t map_x, uint8_t map_y);
uint16_t pack_tile_addr(uint16_t base, uint8_t tile, uint8_t ty);

uint16_t tile_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y);
uint16_t tile_base_address(uint8_t lcdc, uint8_t scy, uint8_t line, uint8_t map);

uint16_t win_map_address(uint8_t lcdc, uint8_t map_x, int wy_counter);
uint16_t win_base_address(uint8_t lcdc, int wy_counter, uint8_t map);

uint16_t sprite_base_address(uint8_t lcdc, uint8_t line, uint8_t sprite_y, uint8_t map, uint8_t flags);

uint8_t flip2(uint8_t b) {
  return uint8_t(((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32);
}

//-----------------------------------------------------------------------------

void PPU::reset(bool run_bootrom, int new_model) {
  *this = {};
  model = new_model;

  //----------
  // Registers

  stat = 0x80;

  //----------
  // Timers and states

  state = PPU_STATE_HBLANK;

  counter = 4;
  counter_delay1 = 3;
  counter_delay2 = 2;
  counter_delay3 = 1;

  hblank_delay2 = HBLANK_DELAY_START;

  //----------
  // Sprites

  sprite_index = -1;
  for (int i = 0; i < 10; i++) sprite_x[i] = 0xFF;
  for (int i = 0; i < 10; i++) sprite_y[i] = 0xFF;
  for (int i = 0; i < 10; i++) sprite_i[i] = 0xFF;

  //----------
  // Pixel pipe

  fetch_type = FETCH_NONE;
  fetch_state = FETCH_IDLE;
  fetch_delay = false;
  sprite_hit = 15;

  //----------
  // Fixup if we're not running the bootrom

  if (!run_bootrom) {
    dma = 0xFF;
    bgp = 0xFC;
    bgp_early = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;

    line = 153;
    line_delay1 = 153;
    line_delay2 = 153;
    line_delay3 = 153;

    counter = 404;
    counter_delay1 = 403;
    counter_delay2 = 402;
    counter_delay3 = 401;

    lcdc = 0x91;
    palettes[0] = 0xfc;
    pix_count2 = 160;
    pix_discard_scx = 0;
    pix_discard_pad = 8;
  }
}

//-----------------------------------------------------------------------------
// interrupt glitch - oam stat fires on vblank
// interrupt glitch - writing to stat during hblank/vblank triggers stat interrupt

PpuOut PPU::tick(int /*tphase*/) const {
  auto out2 = out;

  uint16_t counter_ = counter;
  uint8_t line_ = line;
  int frame_count_ = frame_count;

  counter_++;
  if (counter_ == TCYCLES_LINE) {
    counter_ = 0;
    line_++;
    if (line_ == 154) {
      line_ = 0;
      frame_count_++;
    }
  }

  if (lcdc & FLAG_LCD_ON) {

    uint16_t oam_addr_ = out.oam_addr;
    bool oam_read_ = out.oam_read;

    if (frame_count_ == 0 && line_ == 0 && counter_ < 84) {
      oam_addr_ = 0;
      oam_read_ = false;
    }
    else if (counter_ < 80 && ((counter_ & 1) == 0)) {
      // must have 80 cycles for oam read otherwise we lose an eye in oh.gb
      oam_addr_ = ADDR_OAM_BEGIN + ((counter_ << 1) & 0b11111100);
      oam_read_ = true;
    }

    out2.oam_addr = oam_addr_;
    out2.oam_read = oam_read_;
  }

  return out2;
}

//-----------------------------------------------------------------------------

void PPU::tock(int tphase, CpuBus bus, VRAM::Out vram_out, OAM::Out oam_out) {
  uint16_t counter_ = counter;
  uint8_t line_ = line;
  int frame_count_ = frame_count;

  counter_++;
  if (counter_ == TCYCLES_LINE) {
    counter_ = 0;
    line_++;
    if (line_ == 154) {
      line_ = 0;
      frame_count_++;
    }
  }

  if (tphase == 0 || tphase == 2) {
    if (lcdc & FLAG_LCD_ON) {
      //----------------------------------------
      // Update state machiney stuff

      if (counter == 0)                                          this->state = PPU_STATE_HBLANK;
      if (counter == 4 && (frame_count != 0 || line != 0))       this->state = PPU_STATE_OAM;
      if (counter == 84)                                         this->state = PPU_STATE_VRAM;
      if (counter > 84 && (pix_count2 + pix_discard_pad == 168)) this->state = PPU_STATE_HBLANK;
      if ((line == 144 && counter >= 4) || (line >= 145))        this->state = PPU_STATE_VBLANK;
    }
  }

  if ((lcdc & FLAG_LCD_ON) == 0) {
    this->tock_lcdoff(tphase, bus, vram_out, oam_out);
    return;
  }

  if (counter > 84 && (pix_count2 + pix_discard_pad == 168)) {
    this->vram_addr = 0;
    this->fetch_delay = false;
    this->fetch_state = PPU::FETCH_IDLE;
  }

  if (counter == 0) {
    this->in_window_old = false;
    this->in_window_new = false;
    this->in_window_new_early = false;
    this->window_retrigger_old = false;
    this->window_retrigger_new = false;
    this->pipe_count = 0;
    this->sprite_index = -1;
    this->sprite_count = 0;
    this->pix_count2 = 0;
    this->pix_discard_scx = 0;
    this->pix_discard_pad = 0;
  }

  if (tphase == 0 || tphase == 2) {
    this->stat &= 0b11111100;
    this->stat |= state;
  }

  //-----------------------------------
  // Bus write

  if (bus.read)  bus_read_early(bus.addr);
  if (bus.write) bus_write_early(bus.addr, bus.data);

  //-----------------------------------
  // Handle OAM reads from the previous cycle

  if (oam_out.oe) {
    if (oam_out.addr & 2) {
      this->spriteP = uint8_t(oam_out.data16 >> 0);
      this->spriteF = uint8_t(oam_out.data16 >> 8);
    }
    else {
      this->spriteY = uint8_t(oam_out.data16 >> 0);
      this->spriteX = uint8_t(oam_out.data16 >> 8);
    }
  }

  if (fetch_type == FETCH_BACKGROUND || fetch_type == FETCH_WINDOW) {
    if (fetch_state == FETCH_MAP) tile_map = vram_out.data;
    if (fetch_state == FETCH_LO)  tile_lo = vram_out.data;
    if (fetch_state == FETCH_HI)  tile_hi = vram_out.data;
  }
  else if (fetch_type == FETCH_SPRITE) {
    if (fetch_state == FETCH_LO) sprite_lo = vram_out.data;
    if (fetch_state == FETCH_HI) sprite_hi = vram_out.data;
  }

  //-----------------------------------
  // Build sprite table

  if (oam_out.oe && counter < 86 && sprite_count < 10) {
    int si = (oam_out.addr - ADDR_OAM_BEGIN) >> 2;
    int sy = spriteY - 16;
    int sx = spriteX;

    uint8_t sprite_height = (lcdc & FLAG_TALL_SPRITES) ? 16 : 8;
    if ((sx < 168) && (sy <= line_) && (line_ < sy + sprite_height)) {
      this->sprite_x[sprite_count] = spriteX;
      this->sprite_y[sprite_count] = spriteY;
      this->sprite_i[sprite_count] = (uint8_t)si;
      this->sprite_count++;
    }
  }

  //-----------------------------------
  // Render phase

  if (counter == 86) {

    tile_latched = true;

    if (line == 0) {
      win_y_counter = 0;
      win_y_latch = 0;
    }
    map_x = 0;
    scx_latch = scx;
  }

  window_retrigger_old = window_retrigger_new;
  window_retrigger_new = in_window_old && in_window_new_early;

  // check window hit
  in_window_new = in_window_new_early | in_window_late;
  in_window_new_early =
    (lcdc & FLAG_WIN_ON) &&
    (line >= wy) &&
    (pix_count2 + pix_discard_pad == wx);

  in_window_late = false;
  if (bus.write && bus.addr == 0xFF40 && (bus.data & FLAG_WIN_ON)) {
    in_window_late =
      (lcdc & FLAG_WIN_ON) &&
      (line >= wy) &&
      (pix_count2 + pix_discard_pad == wx + 1);
  }

  if (!in_window_old && in_window_new) {
    win_y_latch = win_y_counter;
    win_y_counter++;
    map_x = 0;

    fetch_state = FETCH_IDLE;
    fetch_delay = false;
    pipe_count = 0;
    tile_latched = false;
    vram_addr = 0;

    bg_pix_lo = 0;
    bg_pix_hi = 0;
    bg_pal_lo = 0;
    bg_pal_hi = 0;
  }

  if (!window_retrigger_old && window_retrigger_new) {
    //win_y_latch = win_y_counter;
    //win_y_counter++;
    //map_x = 0;

    //fetch_state = FETCH_IDLE;
    //fetch_delay = false;
    //if (in_window_new_early) {
    //pipe_count++;
    //}
    //tile_latched = false;
    //vram_addr = 0;

    //bg_pix_lo = 0;
    //bg_pix_hi = 0;
    //bg_pal_lo = 0;
    //bg_pal_hi = 0;
  }

  in_window_old |= in_window_new;

  uint16_t oam_addr_ = 0;
  bool oam_read_ = false;
  vram_addr = 0;

  /*
  #define ADDR_VRAM_BEGIN 0x8000
  #define ADDR_TILE0      0x8000
  #define ADDR_TILE1      0x8800
  #define ADDR_MAP0       0x9800
  #define ADDR_MAP1       0x9C00
  */

  if (vram_out.oe && !fetch_delay) {
    if (fetch_type == FETCH_SPRITE) {
      if (vram_out.addr & 1) {
        sprite_hi = vram_out.data;
      }
      else {
        sprite_lo = vram_out.data;
      }
    }
    else {
      if (vram_out.addr >= ADDR_MAP0) {
        tile_map = vram_out.data;
      }
      else if (vram_out.addr & 1) {
        if (fetch_type != FETCH_BACKGROUND && fetch_type != FETCH_WINDOW && fetch_state != FETCH_LO) {
          //printf("x");
        }
        //tile_hi = vram_out.data;
      }
      else {
        //tile_lo = vram_out.data;
      }
    }
  }

  bool sprite_latched = false;

  if (!fetch_delay) {
    if (fetch_type == FETCH_BACKGROUND || fetch_type == FETCH_WINDOW) {
      if (fetch_state == FETCH_MAP) map_x++;
      if (fetch_state == FETCH_HI) tile_latched = 1;
    }
    else if (fetch_type == FETCH_SPRITE) {
      if (fetch_state == FETCH_HI) sprite_latched = true;
    }
  }

  // if this isn't 86 stuff breaks :/
  if (counter >= 86 && (pix_count2 + pix_discard_pad != 168) && line < 144) {
    if (sprite_latched) {
      if (spriteF & SPRITE_FLIP_X) {
        sprite_lo = flip2(sprite_lo);
        sprite_hi = flip2(sprite_hi);
      }

      uint8_t sprite_pal_lo = spriteF & SPRITE_PAL ? 0b11111111 : 0b00000000;
      uint8_t sprite_pal_hi = 0b11111111;

      // sprites don't draw where we already drew sprites
      uint8_t mask = ob_pix_lo | ob_pix_hi;

      ob_pix_lo |= (sprite_lo & ~mask);
      ob_pix_hi |= (sprite_hi & ~mask);
      ob_pal_lo |= (sprite_pal_lo & ~mask);
      ob_pal_hi |= (sprite_pal_hi & ~mask);
      sprite_index = -1;
    }

    if (sprite_index == -1) {
      if (sprite_hit != 15) {
        sprite_index = sprite_i[sprite_hit];
        spriteX = sprite_x[sprite_hit];
        spriteY = sprite_y[sprite_hit];

        sprite_i[sprite_hit] = 255;
        sprite_x[sprite_hit] = 255;
        sprite_y[sprite_hit] = 255;
      }
    }

    emit_pixel(tphase);

    merge_tile(tphase);

    if (pix_count2 + pix_discard_pad == 168) {
      fetch_type = FETCH_NONE;
      fetch_state = FETCH_IDLE;
      vram_addr = 0;
      fetch_delay = false;
    }
    else {
      if (fetch_delay) {
        fetch_delay = false;
      }
      else {
        if (fetch_state == FETCH_MAP) {
          fetch_state = FETCH_LO;
          fetch_delay = true;
        }
        else if (fetch_state == FETCH_LO) {
          fetch_state = FETCH_HI;
          fetch_delay = true;
        }
        else if (fetch_state == FETCH_HI) {
          fetch_state = FETCH_IDLE;
          fetch_type = FETCH_NONE;
        }

        if (fetch_state == FETCH_IDLE) {
          if (!tile_latched) {
            if (in_window_old && (lcdc & FLAG_WIN_ON)) {
              fetch_type = FETCH_WINDOW;
              fetch_state = FETCH_MAP;
              fetch_delay = true;
            }
            else {
              fetch_type = FETCH_BACKGROUND;
              fetch_state = FETCH_MAP;
              fetch_delay = true;
            }
          }
          else if (sprite_index != -1) {
            fetch_type = FETCH_SPRITE;
            fetch_state = FETCH_MAP;
            fetch_delay = true;
          }
        }
      }

      if (fetch_type == FETCH_BACKGROUND) {
        if (fetch_state == FETCH_MAP) {
          uint8_t new_map_x = (map_x + (scx >> 3)) & 31;
          map_y = ((scy + line) >> 3) & 31;
          vram_addr = tile_map_address(lcdc, new_map_x, map_y);
        }
        else if (fetch_state == FETCH_LO) {
          vram_addr = tile_base_address(lcdc, scy, line, tile_map) + 0;
        }
        else if (fetch_state == FETCH_HI) {
          vram_addr = tile_base_address(lcdc, scy, line, tile_map) + 1;
        }
      }
      else if (fetch_type == FETCH_WINDOW) {
        if (fetch_state == FETCH_MAP) {
          vram_addr = win_map_address(lcdc, map_x, win_y_latch);
        }
        else if (fetch_state == FETCH_LO) {
          vram_addr = win_base_address(lcdc, win_y_latch, tile_map) + 0;
        }
        else if (fetch_state == FETCH_HI) {
          vram_addr = win_base_address(lcdc, win_y_latch, tile_map) + 1;
        }
      }
      else if (fetch_type == FETCH_SPRITE) {
        if (fetch_state == FETCH_MAP) {
          oam_addr_ = (sprite_index << 2) + 2;
          oam_addr_ += ADDR_OAM_BEGIN;
          oam_read_ = true;
        }
        else if (fetch_state == FETCH_LO) {
          vram_addr = sprite_base_address(lcdc, line, spriteY, spriteP, spriteF) + 0;
        }
        else if (fetch_state == FETCH_HI) {
          vram_addr = sprite_base_address(lcdc, line, spriteY, spriteP, spriteF) + 1;
        }
      }
    }
  }

  if ((pix_count2 + pix_discard_pad == 168) && hblank_delay2) {
    hblank_delay2--;
  }

  //-----------------------------------

  int next_pix = pix_count2 + pix_discard_pad;

  sprite_hit = 15;
  if (lcdc & FLAG_OBJ_ON) {
    if (next_pix == sprite_x[9]) sprite_hit = 9;
    if (next_pix == sprite_x[8]) sprite_hit = 8;
    if (next_pix == sprite_x[7]) sprite_hit = 7;
    if (next_pix == sprite_x[6]) sprite_hit = 6;
    if (next_pix == sprite_x[5]) sprite_hit = 5;
    if (next_pix == sprite_x[4]) sprite_hit = 4;
    if (next_pix == sprite_x[3]) sprite_hit = 3;
    if (next_pix == sprite_x[2]) sprite_hit = 2;
    if (next_pix == sprite_x[1]) sprite_hit = 1;
    if (next_pix == sprite_x[0]) sprite_hit = 0;

    if (sprite_hit == 15) {
      if (sprite_x[9] == 0) sprite_hit = 9;
      if (sprite_x[8] == 0) sprite_hit = 8;
      if (sprite_x[7] == 0) sprite_hit = 7;
      if (sprite_x[6] == 0) sprite_hit = 6;
      if (sprite_x[5] == 0) sprite_hit = 5;
      if (sprite_x[4] == 0) sprite_hit = 4;
      if (sprite_x[3] == 0) sprite_hit = 3;
      if (sprite_x[2] == 0) sprite_hit = 2;
      if (sprite_x[1] == 0) sprite_hit = 1;
      if (sprite_x[0] == 0) sprite_hit = 0;
    }
  }

  //-----------------------------------

  if (bus.read)  bus_read_late(bus.addr);
  if (bus.write) bus_write_late(bus.addr, bus.data);

  counter_delay3 = counter_delay2;
  counter_delay2 = counter_delay1;
  counter_delay1 = counter;

  line_delay3 = line_delay2;
  line_delay2 = line_delay1;
  line_delay1 = line;

  counter++;
  if (counter == TCYCLES_LINE) {
    counter = 0;
    line++;
    if (line == 154) {
      line = 0;
      frame_count++;
    }
  }

  if (tphase == 1 || tphase == 3) {
    if (lcdc & FLAG_LCD_ON) {
      frame_start = (counter == 0) && (line == 0);
      frame_done = (counter == 0) && (line == 144);

      if (counter == 0) {
        hblank_delay2 = HBLANK_DELAY_START;
      }
    }
  }

  //----------------------------------------
  // locking

  if (tphase == 1 || tphase == 3) {
    if (lcdc & FLAG_LCD_ON) {

      const int oam_start = 0;
      const int oam_end = 80;
      const int render_start = 82;
      const int render_start_l0 = 84;

      if (frame_count == 0 && line == 0) {
        if (counter == render_start_l0) {
          oam_lock = true;
          vram_lock = true;
        }
      }
      else {
        if (counter == oam_start) {
          oam_lock = true;
        }
        else if (counter == oam_end) {
          oam_lock = false;
        }
        else if (counter == render_start) {
          oam_lock = true;
          vram_lock = true;
        }
      }

      if (hblank_delay2 < 8 || line >= 144) {
        oam_lock = false;
        vram_lock = false;
      }
    }
  }

  //-----------------------------------
  // lyc_match

  if (tphase == 1 || tphase == 3) {
    if (lcdc & FLAG_LCD_ON) {

      if (line == 0) {
        if (counter == 0) compare_line = 0;
        if (counter == 0) ly = line;

        if (counter == 4) compare_line = ly;
        if (counter == 4) ly = line;
      }
      else if (line < 153) {
        if (counter == 0) compare_line = -1;
        if (counter == 0) ly = line;

        if (counter == 4) compare_line = ly;
        if (counter == 4) ly = line;
      }
      else if (line == 153) {
        if (counter == 0) compare_line = -1;
        if (counter == 0) ly = line;

        if (counter == 4) compare_line = ly;
        if (counter == 4) ly = 0;

        if (counter == 8) compare_line = -1;
        if (counter == 8) ly = 0;

        if (counter == 12) compare_line = 0;
        if (counter == 12) ly = 0;
      }
    }
  }

  if (tphase == 1 || tphase == 3) {
    bool fire_stat_oam1 =
      (line > 0) &&
      (line < 144) &&
      (counter == 0);

    bool fire_stat_hblank1 = hblank_delay2 <= 6;
    bool fire_stat_vblank1 = (line == 144 && counter >= 4) || (line >= 145);
    bool fire_stat_lyc1 = compare_line == lyc;
    bool fire_stat_glitch1 = bus.write && bus.addr == ADDR_STAT && stat_int1 != 0;

    bool fire_stat_oam2 =
      ((line == 0 && counter == 4) || (line > 0 && line <= 144 && counter == 4));

    //bool fire_stat_oam2 = (line > 0) && (line < 144) && (counter == 0);

    bool fire_stat_hblank2 = hblank_delay2 <= 6;
    bool fire_stat_vblank2 = (line == 144 && counter >= 4) || (line >= 145);
    bool fire_stat_lyc2 = compare_line == lyc;
    bool fire_stat_glitch2 = bus.write && bus.addr == ADDR_STAT && stat_int2 != 0;

    uint8_t stat_ = stat;
    uint8_t stat_int1_ = stat_int1;
    uint8_t stat_int2_ = stat_int2;

    if (lcdc & FLAG_LCD_ON) {
      if (tphase == 3) {
        stat_ &= ~STAT_LYC;
        stat_int1_ = 0;
        stat_int2_ = 0;
      }
    }

    if (!(lcdc & FLAG_LCD_ON)) {
      fire_stat_oam1 = false;
      fire_stat_hblank1 = false;
      fire_stat_vblank1 = false;
      fire_stat_lyc1 = false;
      fire_stat_glitch1 = false;
    }

    if (fire_stat_lyc1)    stat_ |= STAT_LYC;

    if (fire_stat_hblank1) stat_int1_ |= EI_HBLANK;
    if (fire_stat_vblank1) stat_int1_ |= EI_VBLANK;
    if (fire_stat_lyc1)    stat_int1_ |= EI_LYC;
    if (fire_stat_glitch1) stat_int1_ |= EI_GLITCH;
    if (fire_stat_oam1)    stat_int1_ |= EI_OAM;

    if (fire_stat_hblank2) stat_int2_ |= EI_HBLANK;
    if (fire_stat_vblank2) stat_int2_ |= EI_VBLANK;
    if (fire_stat_lyc2)    stat_int2_ |= EI_LYC;
    if (fire_stat_glitch2) stat_int2_ |= EI_GLITCH;
    if (fire_stat_oam2)    stat_int2_ |= EI_OAM;

    fire_int_vblank1 = line == 144 && counter == 4;
    fire_int_stat1    = ((stat_ & stat_int1_) && !old_stat_int1);

    fire_int_vblank2 = line == 144 && counter == 4;
    fire_int_stat2 = ((stat_ & stat_int2_) && !old_stat_int2);

    stat = stat_;
    stat_int1 = stat_int1_;
    stat_int2 = stat_int2_;
    old_stat_int1 = (stat_ & stat_int1_);
    old_stat_int2 = (stat_ & stat_int2_);
  }

  out = {
    bus_out,
    bus_oe,

    vram_lock,
    vram_addr,
    vram_addr != 0,

    oam_lock,
    oam_addr_,
    oam_read_,

    pix_count2,
    line,
    counter,
    pix_out,
    pix_oe,

    fire_int_stat1,
    fire_int_stat2,
    fire_int_vblank1,
    fire_int_vblank2,
  };

  if (vram_addr != 0) {
    if (fetch_type == FETCH_NONE || fetch_state == FETCH_IDLE) {
      printf("x");
    }
  }
} // PPU::tock

//-----------------------------------------------------------------------------

void PPU::tock_lcdoff(int tphase, CpuBus bus, VRAM::Out /*vram_out*/, OAM::Out /*oam_out*/) {
  counter = 4;
  counter_delay1 = 3;
  counter_delay2 = 2;
  counter_delay3 = 1;

  line = 0;
  line_delay1 = 0;
  line_delay2 = 0;
  line_delay3 = 0;

  if (bus.read)  bus_read_early(bus.addr);
  if (bus.write) bus_write_early(bus.addr, bus.data);

  ly = 0;
  frame_count = 0;
  frame_done = false;
  frame_start = false;

  hblank_delay2 = HBLANK_DELAY_START;
  fetch_state = FETCH_IDLE;

  pix_count2 = 0;
  pix_discard_scx = 0;
  pix_discard_pad = 0;
  pix_out = 0;
  pix_oe = false;
  sprite_count = 0;
  sprite_index = -1;
  tile_latched = false;

  compare_line = 0;

  vram_addr = 0;
  uint16_t oam_addr_ = 0;
  bool oam_read_ = false;

  state = PPU_STATE_HBLANK;
  stat &= 0b11111100;

  vram_lock = false;
  oam_lock = false;

  bus_oe = 0;
  bus_out = 0;

  if (bus.read)  bus_read_late(bus.addr);
  if (bus.write) bus_write_late(bus.addr, bus.data);

  counter_delay3 = counter_delay2;
  counter_delay2 = counter_delay1;
  counter_delay1 = counter;

  line_delay3 = line_delay2;
  line_delay2 = line_delay1;
  line_delay1 = line;

  out = {
    bus_out,
    bus_oe,

    vram_lock,
    vram_addr,
    vram_addr != 0,

    oam_lock,
    oam_addr_,
    oam_read_,

    pix_count2,
    line,
    counter,
    pix_out,
    pix_oe
  };

  if (tphase == 1 || tphase == 3) {
    if (lcdc & FLAG_LCD_ON) {
      frame_start = (counter == 0) && (line == 0);
      frame_done = (counter == 0) && (line == 144);

      if (counter == 0) {
        hblank_delay2 = HBLANK_DELAY_START;
      }
    }
  }

  //----------------------------------------
  // locking

  if (tphase == 1 || tphase == 3) {
    if (lcdc & FLAG_LCD_ON) {

      const int oam_start = 0;
      const int oam_end = 80;
      const int render_start = 82;
      const int render_start_l0 = 84;

      if (frame_count == 0 && line == 0) {
        if (counter == render_start_l0) {
          oam_lock = true;
          vram_lock = true;
        }
      }
      else {
        if (counter == oam_start) {
          oam_lock = true;
        }
        else if (counter == oam_end) {
          oam_lock = false;
        }
        else if (counter == render_start) {
          oam_lock = true;
          vram_lock = true;
        }
      }

      if (hblank_delay2 < 8 || line >= 144) {
        oam_lock = false;
        vram_lock = false;
      }
    }
  }

  //-----------------------------------
  // lyc_match

  if (tphase == 1 || tphase == 3) {
    if (lcdc & FLAG_LCD_ON) {

      if (line == 0) {
        if (counter == 0) compare_line = 0;
        if (counter == 0) ly = line;

        if (counter == 4) compare_line = ly;
        if (counter == 4) ly = line;
      }
      else if (line < 153) {
        if (counter == 0) compare_line = -1;
        if (counter == 0) ly = line;

        if (counter == 4) compare_line = ly;
        if (counter == 4) ly = line;
      }
      else if (line == 153) {
        if (counter == 0) compare_line = -1;
        if (counter == 0) ly = line;

        if (counter == 4) compare_line = ly;
        if (counter == 4) ly = 0;

        if (counter == 8) compare_line = -1;
        if (counter == 8) ly = 0;

        if (counter == 12) compare_line = 0;
        if (counter == 12) ly = 0;
      }
    }
  }
}

//-----------------------------------------------------------------------------

uint16_t pack_map_addr(uint16_t base, uint8_t map_x, uint8_t map_y) {
  return base + (map_y << 5) + map_x;
}

uint16_t pack_tile_addr(uint16_t base, uint8_t tile, uint8_t ty) {
  return base + (tile << 4) + (ty << 1);
}

uint16_t tile_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y) {
  uint16_t base = (lcdc & FLAG_BG_MAP_1) ? ADDR_MAP1 : ADDR_MAP0;
  return pack_map_addr(base, map_x, map_y);
}

uint16_t tile_base_address(uint8_t lcdc, uint8_t scy, uint8_t line, uint8_t map) {
  uint16_t base = (lcdc & FLAG_TILE_0) ? ADDR_TILE0 : ADDR_TILE1;
  map = (lcdc & FLAG_TILE_0) ? map : map ^ 0x80;
  uint8_t ty = (scy + line) & 7;
  return pack_tile_addr(base, map, ty);
}

uint16_t win_map_address(uint8_t lcdc, uint8_t map_x, int wy_counter) {
  uint16_t base = (lcdc & FLAG_WIN_MAP_1) ? ADDR_MAP1 : ADDR_MAP0;
  return pack_map_addr(base, map_x, uint8_t(wy_counter >> 3));
}

uint16_t win_base_address(uint8_t lcdc, int wy_counter, uint8_t map) {
  uint16_t base = (lcdc & FLAG_TILE_0) ? ADDR_TILE0 : ADDR_TILE1;
  map = (lcdc & FLAG_TILE_0) ? map : map ^ 0x80;
  return pack_tile_addr(base, map, wy_counter & 7);
}

uint16_t sprite_base_address(uint8_t /*lcdc*/, uint8_t line, uint8_t sprite_y, uint8_t map, uint8_t flags) {
  uint8_t sprite_dy = line + 16 - sprite_y;
  if (flags & SPRITE_FLIP_Y) {
    //if (lcdc & FLAG_TALL_SPRITES) map &= 0xFE;
    //uint8_t sprite_height = lcdc & FLAG_TALL_SPRITES ? 15 : 7;
    uint8_t sprite_height = 7;
    sprite_dy ^= sprite_height;
  }

  return pack_tile_addr(ADDR_TILE0, map, sprite_dy);
}

//-----------------------------------------------------------------------------
// Emit pixel if we have some in the pipe and we're not stalled.

void PPU::emit_pixel(int /*tphase*/) {
  pix_oe = false;
  pix_out = 0;

  if (pipe_count == 0) {
    return;
  }
  if (sprite_index != -1) {
    return;
  }

  int bg_pix = ((bg_pix_hi >> 6) & 2) | ((bg_pix_lo >> 7) & 1);
  int bg_pal = ((bg_pal_hi >> 6) & 2) | ((bg_pal_lo >> 7) & 1);
  int ob_pix = ((ob_pix_hi >> 6) & 2) | ((ob_pix_lo >> 7) & 1);
  int ob_pal = ((ob_pal_hi >> 6) & 2) | ((ob_pal_lo >> 7) & 1);

  if (!(lcdc & FLAG_OBJ_ON)) {
    ob_pix = 0;
    ob_pal = 0;
  }

  if (!(lcdc & FLAG_BG_ON)) {
    bg_pix = 0;
    bg_pal = 0;
  }

  bg_pix_lo <<= 1;
  bg_pix_hi <<= 1;
  bg_pal_lo <<= 1;
  bg_pal_hi <<= 1;
  ob_pix_lo <<= 1;
  ob_pix_hi <<= 1;
  ob_pal_lo <<= 1;
  ob_pal_hi <<= 1;

  int pal = ob_pix ? ob_pal : bg_pal;
  int pix = ob_pix ? ob_pix : bg_pix;

  if (spriteF & SPRITE_PRI) {
    if (bg_pix) {
      pal = bg_pal;
      pix = bg_pix;
    }
  }

  pipe_count--;

  if (pix_discard_scx < (scx_latch & 7)) {
    pix_oe = false;
    pix_out = 0;
    pix_discard_scx++;
  }
  else if (pix_discard_pad < 8) {
    pix_oe = false;
    pix_out = 0;
    pix_discard_pad++;
  }
  else if (pix_count2 + pix_discard_pad == 168) {
    pix_oe = false;
    pix_out = 0;
  }
  else {
    pix_oe = true;
    pix_out = (palettes[pal] >> (pix << 1)) & 3;
    pix_count2++;
  }
}

//-----------------------------------------------------------------------------

void PPU::merge_tile(int /*tphase*/) {
  if (pipe_count) return;
  if (!tile_latched) return;

  bg_pix_lo = tile_lo;
  bg_pix_hi = tile_hi;

  pipe_count = 8;
  tile_latched = 0;
}

//-----------------------------------------------------------------------------

void PPU::bus_read_early(uint16_t addr) {
  bus_oe = 0;
  bus_out = 0;

  if (ADDR_GPU_BEGIN <= addr && addr <= ADDR_GPU_END) {
    switch (addr) {
    case ADDR_LCDC: bus_oe = 1; bus_out = lcdc; break;
    case ADDR_STAT: bus_oe = 1; bus_out = stat; break;
    case ADDR_SCY:  bus_oe = 1; bus_out = scy; break;
    case ADDR_SCX:  bus_oe = 1; bus_out = scx; break;
    case ADDR_LY:   bus_oe = 1; bus_out = ly; break;
    case ADDR_LYC:  bus_oe = 1; bus_out = lyc; break;
    case ADDR_DMA:  bus_oe = 1; bus_out = dma; break;
    case ADDR_BGP:  bus_oe = 1; bus_out = bgp; break;
    case ADDR_OBP0: bus_oe = 1; bus_out = obp0; break;
    case ADDR_OBP1: bus_oe = 1; bus_out = obp1; break;
    case ADDR_WY:   bus_oe = 1; bus_out = wy; break;
    case ADDR_WX:   bus_oe = 1; bus_out = wx; break;
    }
  }
}

//----------------------------------------

void PPU::bus_read_late(uint16_t addr) {

  if (ADDR_GPU_BEGIN <= addr && addr <= ADDR_GPU_END) {
    switch (addr) {
    case ADDR_LCDC: bus_oe = 1; bus_out = lcdc; break;
    case ADDR_STAT: bus_oe = 1; bus_out = stat; break;
    case ADDR_SCY:  bus_oe = 1; bus_out = scy; break;
    case ADDR_SCX:  bus_oe = 1; bus_out = scx; break;
    case ADDR_LY:   bus_oe = 1; bus_out = ly; break;
    case ADDR_LYC:  bus_oe = 1; bus_out = lyc; break;
    case ADDR_DMA:  bus_oe = 1; bus_out = dma; break;
    case ADDR_BGP:  bus_oe = 1; bus_out = bgp; break;
    case ADDR_OBP0: bus_oe = 1; bus_out = obp0; break;
    case ADDR_OBP1: bus_oe = 1; bus_out = obp1; break;
    case ADDR_WY:   bus_oe = 1; bus_out = wy; break;
    case ADDR_WX:   bus_oe = 1; bus_out = wx; break;
    }
  }
}

//----------------------------------------

void PPU::bus_write_early(uint16_t addr, uint8_t data) {
  if (ADDR_GPU_BEGIN <= addr && addr <= ADDR_GPU_END) {
    switch (addr) {
    case ADDR_LCDC: {
      lcdc  = lcdc & 0b10000011;
      lcdc |= data & 0b01111100;

      // dmg glitch hack
      if (pix_count2 == 0) {
        if ((data & 2) == 0) {
          lcdc &= ~2;
        }
      }

      break;
    }
    case ADDR_STAT: stat = (stat & 0b10000111) | (data & 0b01111000); break;
    case ADDR_SCY:  scy = data;  break;
    case ADDR_LY:   ly = data;   break;
    case ADDR_LYC:  lyc = data;  break;
    case ADDR_DMA:  dma = data;  break;
    case ADDR_BGP: {
      bgp |= data;
      palettes[0] |= data;
      break;
    }
    case ADDR_OBP0: obp0 = palettes[2] = data; break;
    case ADDR_OBP1: obp1 = palettes[3] = data; break;
    };
  }
}

//----------------------------------------

void PPU::bus_write_late(uint16_t addr, uint8_t data) {
  if (ADDR_GPU_BEGIN <= addr && addr <= ADDR_GPU_END) {
    switch (addr) {
    case ADDR_LCDC: {
      // obj_en _must_ be late
      // tile_sel should probably be early?
      lcdc  = lcdc & 0b01111100;
      lcdc |= data & 0b10000011;

      if (!(lcdc & FLAG_WIN_ON)) {
        in_window_old = false;
        in_window_new = false;
        in_window_new_early = false;
        window_retrigger_old = false;
        window_retrigger_new = false;
      }
      break;
    };
    //case ADDR_STAT: stat = (stat & 0b10000111) | (data & 0b01111000); break;
    //case ADDR_SCY:  scy = data;  break;

    // scx write must be late
    case ADDR_SCX: {
      if (counter == 88) {
        scx = data;
      }
      else if (counter == 92) {
        scx = data;
      }
      else {
        scx = data;
      }
      break;
    }

    //case ADDR_LY:   ly = data;   break;
    //case ADDR_LYC:  lyc = data;  break;
    //case ADDR_DMA:  dma = data;  break;
    case ADDR_BGP: {
      bgp = data;
      palettes[0] = data;
      break;
    }
    case ADDR_WY:   wy = data;   break;
    case ADDR_WX:   wx = data;   break;
    };
  }
}

//-----------------------------------------------------------------------------

void PPU::dump(std::string& d) {
  //sprintf(out, "old stat int1 %d\n", ppu.get_old_stat_int1());
  //sprintf(out, "old stat int2 %d\n", ppu.get_old_stat_int2());

  /*
  sprintf(d, "LCDC %s\n", to_binary(lcdc));
  sprintf(d, "STAT %s\n", to_binary(stat));
  sprintf(d, "SCY  %d\n", scy);
  sprintf(d, "SCX  %d\n", scx);
  sprintf(d, "LY   %d\n", ly);
  sprintf(d, "LYC  %d\n", lyc);
  sprintf(d, "DMA  %d\n", dma);
  sprintf(d, "BGP  0x%02x\n", palettes[0]);
  sprintf(d, "OBP0 0x%02x\n", palettes[2]);
  sprintf(d, "OBP1 0x%02x\n", palettes[3]);
  sprintf(d, "WY   %d\n", wy);
  sprintf(d, "WX   %d\n", wx);
  sprintf(d, "wyc   %d\n", win_y_counter);
  sprintf(d, "\n");
  */

  const char* fetch_names1[] = {
    "FETCH_BACKGROUND",
    "FETCH_WINDOW",
    "FETCH_SPRITE",
    "FETCH_NONE",
  };

  const char* fetch_names2[] = {
    "FETCH_MAP",
    "FETCH_LO",
    "FETCH_HI",
    "FETCH_IDLE",
  };

  sprintf(d, "frame   %d\n", frame_count);
  sprintf(d, "state   %d\n", state);

  /*
  sprintf(d, "%s %s %s %s\n",
    oam_phase    ? "OAM" : "   ",
    render_phase ? "VRM" : "   ",
    hblank_phase ? "HBK" : "   ",
    vblank_phase ? "VBK" : "   ");
  */

  /*
  if (stat_int) {
    sprintf(d, "%s %s %s %s %s\n",
      stat_int_lyc ? "#LYC" : "    ",
      stat_int_oam ? "#OAM" : "    ",
      stat_int_hblank ? "#HBK" : "    ",
      stat_int_vblank ? "#VBK" : "    ",
      stat_int_glitch ? "#GLT" : "    ");
  }
  else {
    sprintf(d, "%s %s %s %s %s\n",
      stat_int_lyc ? "-LYC" : "    ",
      stat_int_oam ? "-OAM" : "    ",
      stat_int_hblank ? "-HBK" : "    ",
      stat_int_vblank ? "-VBK" : "    ",
      stat_int_glitch ? "-GLT" : "    ");
  }
  */

  sprintf(d, "clockP2 %3d:%3d\n", line, counter);

  sprintf(d, "hbdly   %d\n", hblank_delay2);
  //sprintf(d, "vblank int %d\n", vblank_int);
  //sprintf(d, "stat int %d\n", stat_int);
  sprintf(d, "\n");
  
  /*
  sprintf(d, "%s\n", in_window_old ? "in_window_old" : "");
  sprintf(d, "%s\n", in_window_new ? "in_window_new" : "");
  sprintf(d, "%s\n", in_window_new_early ? "in_window_new_early" : "");
  sprintf(d, "%s\n", in_window_late ? "in_window_late" : "");
  sprintf(d, "%s\n", window_retrigger_old ? "window_retrigger_old" : "");
  sprintf(d, "%s\n", window_retrigger_new ? "window_retrigger_new" : "");
  sprintf(d, "map x   %d\n", map_x);
  sprintf(d, "map y   %d\n", map_y);
  */

  /*
  sprintf(d, "scx disc %d\n", pix_discard_scx);
  sprintf(d, "pad disc %d\n", pix_discard_pad);
  sprintf(d, "pix      %d\n", pix_count2);
  sprintf(d, "pipe     %d\n", pipe_count);
  sprintf(d, "fetch    %s\n", fetch_names1[fetch_type]);
  sprintf(d, "         %s\n", fetch_names2[fetch_state]);
  sprintf(d, "latched  %d\n", tile_latched);
  sprintf(d, "\n");
  */

  sprintf(d, "sprite idx %d\n", sprite_index);
  sprintf(d, "oam addr  %04x\n", out.oam_addr);
  sprintf(d, "oam read  %04x\n", out.oam_read);
  sprintf(d, "vram addr  %04x\n", vram_addr);
  sprintf(d, "\n");

  /*
  sprintf(d, "stat_int_lyc    %d %d\n", stat_int_lyc, bool(stat & EI_LYC));
  sprintf(d, "stat_int_oam    %d %d\n", stat_int_oam, bool(stat & EI_OAM));
  sprintf(d, "stat_int_vblank %d %d\n", stat_int_vblank, bool(stat & EI_VBLANK));
  sprintf(d, "stat_int_hblank %d %d\n", stat_int_vblank, bool(stat & EI_HBLANK));
  */

  sprintf(d, "spriteX %d\n", spriteX);
  sprintf(d, "spriteY %d\n", spriteY);
  sprintf(d, "spriteP %d\n", spriteP);
  sprintf(d, "spriteF %d\n", spriteF);
  sprintf(d, "\n");

  uint8_t* si = sprite_i;
  uint8_t* sx = sprite_x;
  uint8_t* sy = sprite_y;

  sprintf(d, "sprite_i [%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d]\n", si[0], si[1], si[2], si[3], si[4], si[5], si[6], si[7], si[8], si[9]);
  sprintf(d, "sprite_x [%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d]\n", sx[0], sx[1], sx[2], sx[3], sx[4], sx[5], sx[6], sx[7], sx[8], sx[9]);
  sprintf(d, "sprite_y [%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d]\n", sy[0], sy[1], sy[2], sy[3], sy[4], sy[5], sy[6], sy[7], sy[8], sy[9]);
}

//-----------------------------------------------------------------------------

void draw_tile(uint32_t* framebuffer, int stride, int x, int y, int scale,
  const uint8_t* tile, uint32_t pal, bool flip_x, bool flip_y) {
  for (int ty = 0; ty < 8; ty++) {
    int y2 = flip_y ? 7 - ty : ty;
    uint8_t lo = tile[y2 * 2 + 0];
    uint8_t hi = tile[y2 * 2 + 1];

    if (flip_x) {
      lo = flip2(lo);
      hi = flip2(hi);
    }

    for (int tx = 0; tx < 8; tx++) {
      int plo = (lo >> 7) & 1;
      int phi = (hi >> 7) & 1;
      int pix = (phi << 1) | plo;

      if (pix != 0) {
        int c = gb_colors[(pal >> (pix * 2)) & 3];

        for (int sy = 0; sy < scale; sy++) {
          for (int sx = 0; sx < scale; sx++) {
            framebuffer[(x + tx * scale + sx) + (y + ty * scale + sy) * stride] = c;
          }
        }
      }

      lo <<= 1;
      hi <<= 1;
    }
  }
}

//-------------------------------------

void PPU::dump_tiles(uint32_t* framebuffer, int stride, int x, int y, int /*scale*/,
  const uint8_t* tiles) const {

  for (int sy = 0; sy < 192; sy++) {
    uint32_t* lineA = &framebuffer[(y + sy * 2 + 0) * stride + x];
    uint32_t* lineB = &framebuffer[(y + sy * 2 + 1) * stride + x];
    for (int sx = 0; sx < 128; sx++) {
      int tileX = sx >> 3;
      int tileY = sy >> 3;
      int tileIndex = int(tileY * 16 + tileX);
      const uint8_t* tile = &tiles[tileIndex * 16];
      int spriteX2 = (sx & 7) ^ 7;
      int spriteY2 = (sy & 7);

      uint8_t lo = tile[spriteY2 * 2 + 0];
      uint8_t hi = tile[spriteY2 * 2 + 1];
      uint8_t c = 2 * ((hi >> spriteX2) & 1) + ((lo >> spriteX2) & 1);

      uint32_t color = gb_colors[c];

      if (spriteX2 == 0 && spriteY2 == 0) color = 0xFF00FF00;

      *lineA++ = color; *lineA++ = color;
      *lineB++ = color; *lineB++ = color;
    }
  }
}

//-------------------------------------

void PPU::draw_sprite(OAM& oam, uint32_t* framebuffer, int stride, int sx, int sy, int scale,
  const uint8_t* vram2, int sprite_index2) const {
  const Sprite& s = ((Sprite*)oam.get())[sprite_index2];
  if (s.x == 0) return;
  const uint8_t* sprite_base = vram2 + (16 * s.p);

  uint32_t pal = palettes[2 + s.flags.bits.pal];

  if (lcdc & FLAG_TALL_SPRITES) {
    if (s.flags.bits.flipY) {
      draw_tile(framebuffer, stride, sx, sy, scale, sprite_base + 16, pal, s.flags.bits.flipX, true);
      draw_tile(framebuffer, stride, sx, sy + 8 * scale, scale, sprite_base, pal, s.flags.bits.flipX, true);
    }
    else {
      draw_tile(framebuffer, stride, sx, sy, scale, sprite_base, pal, s.flags.bits.flipX, false);
      draw_tile(framebuffer, stride, sx, sy + 8 * scale, scale, sprite_base + 16, pal, s.flags.bits.flipX, false);
    }
  }
  else {
    draw_tile(framebuffer, stride, sx, sy, scale, sprite_base, pal, s.flags.bits.flipX, s.flags.bits.flipY);
  }
}

//-------------------------------------

void PPU::draw_map(uint32_t* framebuffer, int stride, int sx, int sy, int /*scale*/,
  const uint8_t* map, const uint8_t* tiles, uint8_t map_flip) const {
  for (int my = 0; my < 32; my++) {
    for (int mx = 0; mx < 32; mx++) {
      int m = map[mx + my * 32] ^ map_flip;

      for (int ty = 0; ty < 8; ty++) {
        int lo = tiles[m * 16 + ty * 2 + 0];
        int hi = tiles[m * 16 + ty * 2 + 1];

        for (int tx = 0; tx < 8; tx++) {
          int a = (lo >> (7 - tx)) & 1;
          int b = (hi >> (7 - tx)) & 1;
          int c = gb_colors[b * 2 + a];

          framebuffer[(sx + mx * 8 + tx) + (sy + my * 8 + ty) * stride] = c;
        }
      }
    }
  }
}

//-------------------------------------

void PPU::draw_bg_map(uint32_t* framebuffer, int stride, int sx, int sy, int scale, const uint8_t* vram2) const {
  int map_offset = (lcdc & FLAG_BG_MAP_1) ? 0x1C00 : 0x1800;
  int tile_offset = (lcdc & FLAG_TILE_0) ? 0x0000 : 0x0800;
  uint8_t map_flip = (lcdc & FLAG_TILE_0) ? 0x00 : 0x80;

  draw_map(framebuffer, stride, sx, sy, scale,
    vram2 + map_offset, vram2 + tile_offset, map_flip);
}

void PPU::draw_wm_map(uint32_t* framebuffer, int stride, int sx, int sy, int scale, const uint8_t* vram2) const {
  int map_offset = (lcdc & FLAG_WIN_MAP_1) ? 0x1C00 : 0x1800;
  int tile_offset = (lcdc & FLAG_TILE_0) ? 0x0000 : 0x0800;
  uint8_t map_flip = (lcdc & FLAG_TILE_0) ? 0x00 : 0x80;
  draw_map(framebuffer, stride, sx, sy, scale,
    vram2 + map_offset, vram2 + tile_offset, map_flip);
}

//-----------------------------------------------------------------------------
