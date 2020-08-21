#include "PPU.h"
#include "Constants.h"
#include <assert.h>

extern const uint32_t gb_colors[];

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

void PPU::reset(bool run_bootrom) {
  *this = {};

  //----------
  // Registers

  stat = 0x80;

  //----------
  // Timers and states

  counter = 4;
  counter_delay1 = 3;
  counter_delay2 = 2;
  counter_delay3 = 1;

  hblank_delay2 = HBLANK_DELAY_START;

  //----------
  // Sprites

  sprite_hit = 15;
  sprite_index = -1;

  for (int i = 0; i < 10; i++) sprite_x[i] = 0xFF;
  for (int i = 0; i < 10; i++) sprite_y[i] = 0xFF;
  for (int i = 0; i < 10; i++) sprite_i[i] = 0xFF;

  //----------
  // Fixup if we're not running the bootrom

  if (!run_bootrom) {
    //dma = 0xFF;
    bgp = 0xFC;
    obp0 = 0xFF;
    obp1 = 0xFF;

    line = 153;
    line_delay1 = 153;
    line_delay2 = 153;
    line_delay3 = 153;

    counter = 401;
    counter_delay1 = 400;
    counter_delay2 = 399;
    counter_delay3 = 398;

    lcdc = 0x91;
    pix_count = 160;
    pix_discard_scx = 0;
    pix_discard_pad = 8;

    update_palettes();
  }
}

//-----------------------------------------------------------------------------

void PPU::get_vbus_req(Req& r) const {
  if ((lcdc & FLAG_LCD_ON) == 0) return;

  uint8_t new_map_x = (map_x + (scx >> 3)) & 31;
  uint8_t map_y = ((scy + line) >> 3) & 31;

  uint16_t fetch_addr = 0;

  if (fetch_type == FETCH_BACKGROUND) {
    if      (fetch_state == FETCH_TILE_MAP)  fetch_addr = tile_map_address(lcdc, new_map_x, map_y);
    else if (fetch_state == FETCH_TILE_LO)   fetch_addr = tile_base_address(lcdc, scy, line, tile_map) + 0;
    else if (fetch_state == FETCH_TILE_HI)   fetch_addr = tile_base_address(lcdc, scy, line, tile_map) + 1;
  }
  else if (fetch_type == FETCH_WINDOW) {
    if      (fetch_state == FETCH_TILE_MAP)  fetch_addr = win_map_address(lcdc, map_x, win_y_latch);
    else if (fetch_state == FETCH_TILE_LO)   fetch_addr = win_base_address(lcdc, win_y_latch, tile_map) + 0;
    else if (fetch_state == FETCH_TILE_HI)   fetch_addr = win_base_address(lcdc, win_y_latch, tile_map) + 1;
  }
  else if (fetch_type == FETCH_SPRITE) {
    if      (fetch_state == FETCH_SPRITE_LO) fetch_addr = sprite_base_address(lcdc, line, spriteY, spriteP, spriteF) + 0;
    else if (fetch_state == FETCH_SPRITE_HI) fetch_addr = sprite_base_address(lcdc, line, spriteY, spriteP, spriteF) + 1;
  }

  if (pix_count + pix_discard_pad == 168) {
    fetch_addr = 0;
  }

  if (fetch_type != FETCH_NONE) {
    r.addr    = fetch_addr;
    r.data_lo = 0;
    r.read    = 1;
    r.write   = 0;
  }
}

//----------------------------------------

void PPU::get_obus_req(Req& r) const {
  if ((lcdc & FLAG_LCD_ON) == 0) return;

  uint16_t fetch_addr = 0;

  // must have 80 cycles for oam read otherwise we lose an eye in oh.gb
  if (counter < 80) {
    r.addr  = uint16_t(ADDR_OAM_BEGIN + ((counter << 1) & 0b11111100));
    r.data  = 0;
    r.read  = 1;
    r.write = 0;
    return;
  }

  if (fetch_type == FETCH_SPRITE && fetch_state == FETCH_SPRITE_MAP) {
    fetch_addr = ADDR_OAM_BEGIN + (sprite_index << 2) + 2;
  }

  if (pix_count + pix_discard_pad == 168) {
    fetch_addr = 0;
  }

  if (fetch_addr != 0) {
    r.addr  = fetch_addr;
    r.data  = 0;
    r.read  = 1;
    r.write = 0;
  }
}

//-----------------------------------------------------------------------------

void PPU::on_vbus_ack(const Ack& vbus_ack) {
  uint8_t data = (uint8_t)vbus_ack.data_lo;

  if (vbus_ack.read) {
    if (fetch_type == FETCH_BACKGROUND || fetch_type == FETCH_WINDOW) {
      //printf("fetch background 0x%02x\n", data);

      if (fetch_state == FETCH_TILE_MAP) tile_map = data;
      if (fetch_state == FETCH_TILE_LO)  tile_lo = data;
      if (fetch_state == FETCH_TILE_HI)  tile_hi = data;
    }
    else if (fetch_type == FETCH_SPRITE) {
      //printf("fetch sprite 0x%02x\n", data);

      if (fetch_state == FETCH_SPRITE_LO) sprite_lo = data;
      if (fetch_state == FETCH_SPRITE_HI) sprite_hi = data;
    }
  }
}

//----------------------------------------
// this is probably gonna break if cpu tries to read obus during rendering...

void PPU::on_obus_ack(const Ack& obus_ack) {
  uint8_t lo = obus_ack.data_lo;
  uint8_t hi = obus_ack.data_hi;

  if (obus_ack.read) {
    //printf("fetch oam 0x%04x\n", obus_ack.data);

    if (obus_ack.addr & 2) {
      this->spriteP = lo;
      this->spriteF = hi;
    }
    else {
      this->spriteY = lo;
      this->spriteX = hi;
    }

    //-----------------------------------
    // Build sprite table

    if (counter < 86 && sprite_count < 10) {
      int si = (obus_ack.addr - ADDR_OAM_BEGIN) >> 2;
      int sy = spriteY - 16;
      int sx = spriteX;

      uint8_t sprite_height = (lcdc & FLAG_TALL_SPRITES) ? 16 : 8;
      if ((sx < 168) && (sy <= line) && (line < sy + sprite_height)) {
        sprite_x[sprite_count] = spriteX;
        sprite_y[sprite_count] = spriteY;
        sprite_i[sprite_count] = (uint8_t)si;
        sprite_count++;
      }
    }
  }
}

//-----------------------------------------------------------------------------

void PPU::tick(const Req& req, Ack& ack) const {
  if (req.addr == ADDR_LY) return;

  if (req.read && (ADDR_GPU_BEGIN <= req.addr) && (req.addr <= ADDR_GPU_END) && (req.addr != ADDR_DMA)) {
    uint8_t data = (uint8_t)req.data_lo;
    switch (req.addr) {
    case ADDR_LCDC: data = lcdc; break;
    case ADDR_STAT: data = stat; break;
    case ADDR_SCY:  data = scy; break;
    case ADDR_SCX:  data = scx; break;
    case ADDR_LY:   data = ly; break;
    case ADDR_LYC:  data = lyc; break;
      //case ADDR_DMA:  data = dma; break;
    case ADDR_BGP:  data = bgp; break;
    case ADDR_OBP0: data = obp0; break;
    case ADDR_OBP1: data = obp1; break;
    case ADDR_WY:   data = wy; break;
    case ADDR_WX:   data = wx; break;
    default:        data = 0; break;
    }

    ack.addr = req.addr;
    ack.data_lo = data;
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void PPU::tock(int old_phase, int /*new_phase*/, const Req& req) {
  // interrupt glitch - oam stat fires on vblank
  // interrupt glitch - writing to stat during hblank/vblank triggers stat interrupt

  if (req.write && (ADDR_GPU_BEGIN <= req.addr) && (req.addr <= ADDR_GPU_END) && (req.addr != ADDR_DMA)) {
    uint8_t data = (uint8_t)req.data_lo;
    switch (req.addr) {
    case ADDR_LCDC: lcdc = data; break;
    case ADDR_STAT: stat = (stat & 0x87) | (data & 0x78); break;
    case ADDR_SCY:  scy = data;  break;
    case ADDR_SCX:  scx = data;  break;
    case ADDR_LY:   ly = data;   break;
    case ADDR_LYC:  lyc = data;  break;
      //case ADDR_DMA:  dma = data;  break;
    case ADDR_BGP:  bgp = data;  break;
    case ADDR_OBP0: obp0 = data; break;
    case ADDR_OBP1: obp1 = data; break;
    case ADDR_WY:   wy = data;   break;
    case ADDR_WX:   wx = data;   break;
    };

    update_palettes();
  }

  if ((lcdc & FLAG_LCD_ON) == 0) {
    tock_lcdoff();
    return;
  }

  //----------------------------------------
  // Update state machiney stuff

  const int tphase = (old_phase >> 1);

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

  if (counter == 0) {
    state = PPU_STATE_HBLANK;
    in_window_old = false;
    in_window_new = false;
    in_window_early = false;
    win_retrig_old = false;
    win_retrig_new = false;
    pipe_count = 0;
    sprite_index = -1;
    sprite_count = 0;
    pix_count = 0;
    pix_discard_scx = 0;
    pix_discard_pad = 0;
    hblank_delay2 = HBLANK_DELAY_START;
    frame_start = (line == 0);
    frame_done = (line == 144);
  }

  if (counter == 4 && (frame_count != 0 || line != 0)) {
    this->state = PPU_STATE_OAM;
  }

  if (counter == 84) {
    state = PPU_STATE_VRAM;
  }

  if (counter > 84 && (pix_count + pix_discard_pad == 168)) {
    state = PPU_STATE_HBLANK;
    fetch_delay = false;
    fetch_state = PPU::FETCH_IDLE;
  }

  if ((line == 144 && counter >= 4) || (line >= 145)) {
    state = PPU_STATE_VBLANK;
  }

  //-----------------------------------
  // Handle OAM/VRAM reads

  if (!fetch_delay && (fetch_state == FETCH_SPRITE_HI))  {
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

  //-----------------------------------
  // Window stuff

  win_retrig_old = win_retrig_new;
  win_retrig_new = in_window_old && in_window_early;

  // check window hit
  in_window_new = in_window_early | in_window_late;
  in_window_early =
    (lcdc & FLAG_WIN_ON) &&
    (line >= wy) &&
    (pix_count + pix_discard_pad == wx);

  /*
  // what was this about? writes to FF40 do something weird?
  in_window_late = false;
  if (bus_to_ppu.write && bus_to_ppu.addr == 0xFF40 && (bus_to_ppu.data & FLAG_WIN_ON)) {
    in_window_late =
      (lcdc & FLAG_WIN_ON) &&
      (line >= wy) &&
      (pix_count + pix_discard_pad == wx + 1);
  }
  */

  if (!in_window_old && in_window_new) {
    win_y_latch = win_y_counter;
    win_y_counter++;
    map_x = 0;

    fetch_state = FETCH_IDLE;
    fetch_delay = false;
    pipe_count = 0;
    tile_latched = false;

    bg_pix_lo = 0;
    bg_pix_hi = 0;
    bg_pal_lo = 0;
    bg_pal_hi = 0;
  }

  if (!win_retrig_old && win_retrig_new) {
    //win_y_latch = win_y_counter;
    //win_y_counter++;
    //map_x = 0;

    //fetch_state = FETCH_IDLE;
    //fetch_delay = false;
    //if (in_window_early) {
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

  //-----------------------------------
  // Actual rendering

  int next_pix = pix_count + pix_discard_pad;

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

#if SPRITE_AT_ZERO_GLITCH
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
#endif
      }

  if ((sprite_index == -1) && sprite_hit != 15) {
    sprite_index = sprite_i[sprite_hit];
    spriteX = sprite_x[sprite_hit];
    spriteY = sprite_y[sprite_hit];

    sprite_i[sprite_hit] = 255;
    sprite_x[sprite_hit] = 255;
    sprite_y[sprite_hit] = 255;
  }

  if (counter == 86) {

    tile_latched = true;

    if (line == 0) {
      win_y_counter = 0;
      win_y_latch = 0;
    }
    map_x = 0;
    scx_latch = scx;
  }

  // if this isn't 87 stuff breaks :/
  bool rendering = counter >= 87 && (pix_count + pix_discard_pad != 168) && line < 144;

  if (rendering) {
    emit_pixel(tphase);

    if (pipe_count == 0 && tile_latched) {
      bg_pix_lo = tile_lo;
      bg_pix_hi = tile_hi;
      pipe_count = 8;
      tile_latched = 0;
    }
  }

  if ((pix_count + pix_discard_pad == 168) && hblank_delay2) {
    hblank_delay2--;
  }

  //-----------------------------------
  // Fetcher state machine

  if (rendering) {
    if (fetch_delay) {
      fetch_delay = false;
    }
    else {
      if (fetch_state == FETCH_TILE_MAP) map_x++;
      if (fetch_state == FETCH_TILE_HI) tile_latched = 1;

      if      (fetch_state == FETCH_TILE_MAP)   fetch_state = FETCH_TILE_LO;
      else if (fetch_state == FETCH_TILE_LO)    fetch_state = FETCH_TILE_HI;
      else if (fetch_state == FETCH_TILE_HI)    fetch_state = FETCH_IDLE;
      else if (fetch_state == FETCH_SPRITE_MAP) fetch_state = FETCH_SPRITE_LO;
      else if (fetch_state == FETCH_SPRITE_LO)  fetch_state = FETCH_SPRITE_HI;
      else if (fetch_state == FETCH_SPRITE_HI)  fetch_state = FETCH_IDLE;

      if (fetch_state == FETCH_IDLE) {
        if (!tile_latched) {
          if (in_window_old && (lcdc & FLAG_WIN_ON)) {
            fetch_type = FETCH_WINDOW;
            fetch_state = FETCH_TILE_MAP;
          }
          else {
            fetch_type = FETCH_BACKGROUND;
            fetch_state = FETCH_TILE_MAP;
          }
        }
        else if (sprite_index != -1) {
          fetch_type = FETCH_SPRITE;
          fetch_state = FETCH_SPRITE_MAP;
        }
      }

      if (fetch_state != FETCH_IDLE) {
        fetch_delay = true;
      }
      else {
        fetch_type = FETCH_NONE;
      }
    }
  }

  //-----------------------------------
  // Turn fetcher off once line is done

  if (pix_count + pix_discard_pad == 168) {
    fetch_type = FETCH_NONE;
    fetch_state = FETCH_IDLE;
    fetch_delay = false;
  }

  //-----------------------------------

  // this needs to go somewhere else

  if (tphase == 0 || tphase == 2) {
    stat &= 0b11111100;
    stat |= state;
  }

  //-----------------------------------
  // lyc_match

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

  //-----------------------------------
  // interrupt generation

  if (tphase == 1 || tphase == 3) {
    bool fire_stat_hblank1 = hblank_delay2 <= 6;
    bool fire_stat_vblank1 = (line == 144 && counter >= 4) || (line >= 145);
    bool fire_stat_lyc1    = compare_line == lyc;
    //bool fire_stat_glitch1 = ibus_req.write && ibus_req.addr == ADDR_STAT && stat_int1 != 0;
    bool fire_stat_oam1    = (line > 0) && (line < 144) && (counter == 0);

    bool fire_stat_hblank2 = hblank_delay2 <= 6;
    bool fire_stat_vblank2 = (line == 144 && counter >= 4) || (line >= 145);
    bool fire_stat_lyc2    = compare_line == lyc;
    //bool fire_stat_glitch2 = ibus_req.write && ibus_req.addr == ADDR_STAT && stat_int2 != 0;
    bool fire_stat_oam2   = ((line == 0 && counter == 4) || (line > 0 && line <= 144 && counter == 4));

    uint8_t stat_ = stat;
    uint8_t stat_int1_ = stat_int1;
    uint8_t stat_int2_ = stat_int2;

    if (tphase == 3) {
      stat_ &= ~STAT_LYC;
      stat_int1_ = 0;
      stat_int2_ = 0;
    }

    if (fire_stat_lyc1)    stat_ |= STAT_LYC;

    if (fire_stat_hblank1) stat_int1_ |= EI_HBLANK;
    if (fire_stat_vblank1) stat_int1_ |= EI_VBLANK;
    if (fire_stat_lyc1)    stat_int1_ |= EI_LYC;
    //if (fire_stat_glitch1) stat_int1_ |= EI_GLITCH;
    if (fire_stat_oam1)    stat_int1_ |= EI_OAM;

    if (fire_stat_hblank2) stat_int2_ |= EI_HBLANK;
    if (fire_stat_vblank2) stat_int2_ |= EI_VBLANK;
    if (fire_stat_lyc2)    stat_int2_ |= EI_LYC;
    //if (fire_stat_glitch2) stat_int2_ |= EI_GLITCH;
    if (fire_stat_oam2)    stat_int2_ |= EI_OAM;

    vblank1 = line == 144 && counter == 4;
    stat1   = ((stat_ & stat_int1_) && !old_stat_int1);

    vblank2 = line == 144 && counter == 4;
    stat2   = ((stat_ & stat_int2_) && !old_stat_int2);

    stat = stat_;
    stat_int1 = stat_int1_;
    stat_int2 = stat_int2_;
    old_stat_int1 = (stat_ & stat_int1_);
    old_stat_int2 = (stat_ & stat_int2_);
  }

} // PPU::tock

//-----------------------------------------------------------------------------

void PPU::tock_lcdoff() {
  counter = 4;
  counter_delay1 = 3;
  counter_delay2 = 2;
  counter_delay3 = 1;

  line = 0;
  line_delay1 = 0;
  line_delay2 = 0;
  line_delay3 = 0;

  ly = 0;
  frame_count = 0;
  frame_done = false;
  frame_start = false;

  hblank_delay2 = HBLANK_DELAY_START;
  fetch_state = FETCH_IDLE;

  pix_count = 0;
  pix_discard_scx = 0;
  pix_discard_pad = 0;
  sprite_count = 0;
  sprite_index = -1;
  tile_latched = false;

  compare_line = 0;

  state = PPU_STATE_HBLANK;
  stat &= 0b11111100;

  stat1 = 0;
  stat2 = 0;
  vblank1 = 0;
  vblank2 = 0;
}

//-----------------------------------------------------------------------------

uint16_t pack_tile_addr(uint16_t base, uint8_t tile, uint8_t ty) {
  return base + (tile << 4) + (ty << 1);
}

uint16_t tile_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y) {
  uint16_t base = (lcdc & FLAG_BG_MAP_1) ? ADDR_MAP1 : ADDR_MAP0;
  return base + (map_y << 5) + map_x;
}

uint16_t tile_base_address(uint8_t lcdc, uint8_t scy, uint8_t line, uint8_t map) {
  uint16_t base = (lcdc & FLAG_TILE_0) ? ADDR_TILE0 : ADDR_TILE1;
  map = (lcdc & FLAG_TILE_0) ? map : map ^ 0x80;
  uint8_t ty = (scy + line) & 7;
  return pack_tile_addr(base, map, ty);
}

uint16_t win_map_address(uint8_t lcdc, uint8_t map_x, int wy_counter) {
  uint16_t base = (lcdc & FLAG_WIN_MAP_1) ? ADDR_MAP1 : ADDR_MAP0;
  uint8_t win_y = uint8_t(wy_counter >> 3);
  return base + (win_y << 5) + map_x;
}

uint16_t win_base_address(uint8_t lcdc, int wy_counter, uint8_t map) {
  uint16_t base = (lcdc & FLAG_TILE_0) ? ADDR_TILE0 : ADDR_TILE1;
  map = (lcdc & FLAG_TILE_0) ? map : map ^ 0x80;
  return pack_tile_addr(base, map, wy_counter & 7);
}

uint16_t sprite_base_address(uint8_t /*lcdc*/, uint8_t line, uint8_t sprite_y, uint8_t map, uint8_t flags) {
  uint8_t sprite_dy = line + 16 - sprite_y;
  if (flags & SPRITE_FLIP_Y) {
    uint8_t sprite_height = 7;
    sprite_dy ^= sprite_height;
  }

  return pack_tile_addr(ADDR_TILE0, map, sprite_dy);
}

//-----------------------------------------------------------------------------
// Emit pixel if we have some in the pipe and we're not stalled.

void PPU::emit_pixel(int /*tphase*/) {
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

  pix_out = 0;
  pix_oe = false;

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
  else if (pix_count + pix_discard_pad == 168) {
    pix_oe = false;
    pix_out = 0;
  }
  else {
    pix_oe = true;
    pix_out = (palettes[pal] >> (pix << 1)) & 3;
    pix_count++;
  }
}

//-----------------------------------------------------------------------------

const char* state_names[] = {
  "HBLANK",
  "VBLANK",
  "OAM",
  "VRAM"
};

const char* fetch_names1[] = {
  "FETCH_NONE",
  "FETCH_BACKGROUND",
  "FETCH_WINDOW",
  "FETCH_SPRITE",
};

const char* fetch_names2[] = {
  "FETCH_IDLE",
  "FETCH_TILE_MAP",
  "FETCH_TILE_LO",
  "FETCH_TILE_HI",
  "FETCH_SPRITE_MAP",
  "FETCH_SPRITE_LO",
  "FETCH_SPRITE_HI",
};


void PPU::dump(Dumper& d) const {
  d("\002--------------PPU--------------\001\n");

  d("LCDC      %s\n", byte_to_bits(lcdc));
  d("STAT      %s\n", byte_to_bits(stat));
  d("SCY       %d\n", scy);
  d("SCX       %d\n", scx);
  d("LY        %d\n", ly);
  d("LYC       %d\n", lyc);
  //d("DMA       %d\n", dma);
  d("BGP       0x%02x\n", bgp);
  d("OBP0      0x%02x\n", obp0);
  d("OBP1      0x%02x\n", obp1);
  d("WY        %d\n", wy);
  d("WX        %d\n", wx);
  d("wyc       %d\n", win_y_counter);
  d("\n");

  d("frame     %d\n", frame_count);
  d("line      %d\n", line);
  d("state     %s\n", state_names[state]);
  d("counter   %d\n", counter);
  d("hdelay    %d\n", hblank_delay2);
  d("\n");

  d("compare_line  %d\n", compare_line);
  d("stat_int1     %d\n", stat_int1);
  d("stat_int2     %d\n", stat_int2);
  d("old_stat_int1 %d\n", old_stat_int1);
  d("old_stat_int2 %d\n", old_stat_int2);
  d("scx_latch     %d\n", scx_latch);
  d("win_y_latch   %d\n", win_y_latch);
  d("win_y_counter %d\n", win_y_counter);
  d("\n");

  //----------
  // Sprites

  d("sprite_count %d\n",     sprite_count);
  d("sprite_index %d\n",     sprite_index);
  d("sprite_hit   %d\n",     sprite_hit);
  d("spriteY      %d\n",     spriteY);
  d("spriteX      %d\n",     spriteX);
  d("spriteP      %d\n",     spriteP);
  d("spriteF      %d\n",     spriteF);
  d("sprite_lo    0x%02x\n", sprite_lo);
  d("sprite_hi    0x%02x\n", sprite_hi);

  for (int i = 0; i < 10; i++) {
    d("sprite %d %d %d\n", sprite_i[i], sprite_x[i], sprite_y[i]);
  }

  d("\n");

  //----------
  // Vram Fetcher

  d("fetch_type      %s\n", fetch_names1[fetch_type]);
  d("fetch_state     %s\n", fetch_names2[fetch_state]);
  d("fetch_delay     %d\n", fetch_delay);
  d("in_window_old   %d\n", in_window_old);
  d("in_window_new   %d\n", in_window_new);
  d("in_window_early %d\n", in_window_early);
  d("in_window_late  %d\n", in_window_late);
  d("win_retrig_old  %d\n", win_retrig_old);
  d("win_retrig_new  %d\n", win_retrig_new);
  d("\n");

  //----------
  // Pixel pipe

  d("map_x           %d\n", map_x);

  d("tile_map        %d\n", tile_map);
  d("tile_lo         %d\n", tile_lo);
  d("tile_hi         %d\n", tile_hi);
  d("tile_latched    %d\n", tile_latched);

  d("pix_count       %d\n", pix_count);
  d("pix_discard_scx %d\n", pix_discard_scx);
  d("pix_discard_pad %d\n", pix_discard_pad);
  d("pipe_count      %d\n", pipe_count);
  d("\n");

  d("bg_pix_lo       %s\n", byte_to_bits(bg_pix_lo));
  d("bg_pix_hi       %s\n", byte_to_bits(bg_pix_hi));
  d("bg_pal_lo       %s\n", byte_to_bits(bg_pal_lo));
  d("bg_pal_hi       %s\n", byte_to_bits(bg_pal_hi));

  {
    /*
    auto out = tick(tcycle);

    pribus(d, "bus_to_ppu",  bus_to_ppu);
    pribus(d, "ppu_to_bus",  out.ppu_to_bus);
    d("\n");

    pribus(d, "ppu_to_vram", out.ppu_to_vram);
    pribus(d, "vram_to_ppu", vram_to_ppu);
    d("\n");

    pribus(d, "ppu_to_oam",  out.ppu_to_oam);
    pribus(d, "oam_to_ppu",  oam_to_ppu);
    d("\n");
    */

#define dumpit(a, b) d("%-14s " b "\n", #a, a);

    dumpit(pix_count ,"%d");
    dumpit(line      ,"%d");
    dumpit(counter   ,"%d");
    dumpit(pix_out   ,"0x%02x");
    dumpit(pix_oe    ,"%d");
    dumpit(stat1     ,"%d");
    dumpit(stat2     ,"%d");
    dumpit(vblank1   ,"%d");
    dumpit(vblank2   ,"%d");
  }
}

//-----------------------------------------------------------------------------
