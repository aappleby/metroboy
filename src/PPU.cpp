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
// interrupt glitch - oam stat fires on vblank
// interrupt glitch - writing to stat during hblank/vblank triggers stat interrupt

#if 0
PPU::Out PPU::tick(const int tcycle_) const {
  (void)tcycle_;

  //-----------------------------------
  // OAM/VRAM address generator

  /*
  struct Out {
  Bus to_cpu;
  Bus to_vram;
  Bus to_oam;

  int x;
  int y;
  int counter;
  uint8_t pix_out;
  bool pix_oe;

  bool stat1;
  bool stat2;
  bool vblank1;
  bool vblank2;
  };
  */

  Out out = {};

  out.ppu_to_bus  = ppu_to_bus;
  out.ppu_to_vram = ppu_to_vram;
  out.ppu_to_oam  = ppu_to_oam;
  out.x       = pix_count;
  out.y       = line;
  out.counter = counter;
  out.pix_out = pix_out;
  out.pix_oe  = pix_oe;

  if (!(lcdc & FLAG_LCD_ON)) {
    return out;
  }
  
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

  //if (frame_count_ == 0 && line_ == 0 && counter_ < 84) {
  //  oam_addr_ = 0;
  //}
  //else

  //----------------------------------------
  // locking

  const int oam_start = 0;
  const int oam_end = 80;
  const int render_start = 82;
  const int render_start_l0 = 84;

  bool in_blank = (hblank_delay2 < 8) || (line >= 144);

  out.ppu_to_oam.lock = false;
  out.ppu_to_vram.lock = false;

  if (frame_count_ == 0 && line_ == 0) {
    out.ppu_to_oam.lock  = (counter_ >= render_start_l0);
    out.ppu_to_vram.lock = (counter_ >= render_start_l0);
  }
  else {
    out.ppu_to_oam.lock  |= (oam_start <= counter_) && (counter_ < oam_end);
    out.ppu_to_oam.lock  |= (render_start <= counter_);
    out.ppu_to_vram.lock |= (render_start <= counter_);
  }

  if (in_blank) {
    out.ppu_to_oam.lock = false;
    out.ppu_to_vram.lock = false;
  }

  return out;
}
#endif

//-----------------------------------------------------------------------------

bool PPU::has_vbus_req(int /*tcycle*/) {
  return fetch_type != FETCH_NONE;
}

Req PPU::get_vbus_req(int /*tcycle*/) {
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

  return {
    .addr  = fetch_addr,
    .data  = 0,
    .read  = fetch_addr != 0,
    .write = 0,
  };
}

//----------------------------------------

bool PPU::has_obus_req(int /*tcycle*/) {
  return (counter < 80) || (fetch_type == FETCH_SPRITE);
}

Req PPU::get_obus_req(int /*tcycle*/) {
  uint16_t fetch_addr = 0;

  // must have 80 cycles for oam read otherwise we lose an eye in oh.gb
  if (counter < 80 && ((counter & 1) == 0)) {
    return {
      .addr  = uint16_t(ADDR_OAM_BEGIN + ((counter << 1) & 0b11111100)),
      .data  = 0,
      .read  = 1,
      .write = 0,
    };
  }

  if (fetch_type == FETCH_SPRITE && fetch_state == FETCH_SPRITE_MAP) {
    fetch_addr = ADDR_OAM_BEGIN + (sprite_index << 2) + 2;
  }

  if (pix_count + pix_discard_pad == 168) {
    fetch_addr = 0;
  }

  return {
    .addr  = fetch_addr,
    .data  = 0,
    .read  = fetch_addr != 0,
    .write = 0,
  };
}

//-----------------------------------------------------------------------------

bool PPU::on_ibus_req(int tcycle, Req ibus_req, Ack& ibus_ack) {
  int tphase = tcycle & 3;

  if (ibus_req.addr == ADDR_DMA) return false;
  bool hit = (ADDR_GPU_BEGIN <= ibus_req.addr && ibus_req.addr <= ADDR_GPU_END);
  if (!hit) return false;

  assert(!ibus_ack.read && !ibus_ack.write);

  uint8_t data = (uint8_t)ibus_req.data;

  if (hit && ibus_req.read) {
    switch (ibus_req.addr) {
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

    ibus_ack = {
      .addr  = ibus_req.addr,
      .data  = data,
      .read  = 1,
      .write = 0,
    };
    return true;
  }

  if (hit && ibus_req.write && tphase == 0) {
    switch (ibus_req.addr) {
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

    ibus_ack = {
      .addr  = ibus_req.addr,
      .data  = data,
      .read  = 0,
      .write = 1,
    };
    return true;
  }

  return false;
}

//-----------------------------------------------------------------------------

void PPU::on_vbus_ack(Ack ack) {
  uint8_t data = (uint8_t)ack.data;

  if (ack.read) {
    if (fetch_type == FETCH_BACKGROUND || fetch_type == FETCH_WINDOW) {
      if (fetch_state == FETCH_TILE_MAP) tile_map = data;
      if (fetch_state == FETCH_TILE_LO)  tile_lo = data;
      if (fetch_state == FETCH_TILE_HI)  tile_hi = data;
    }
    else if (fetch_type == FETCH_SPRITE) {
      if (fetch_state == FETCH_SPRITE_LO) sprite_lo = data;
      if (fetch_state == FETCH_SPRITE_HI) sprite_hi = data;
    }
  }
}

//----------------------------------------

void PPU::on_obus_ack(Ack ack) {
  uint8_t lo = uint8_t(ack.data >> 0);
  uint8_t hi = uint8_t(ack.data >> 8);

  if (ack.read) {
    if (ack.addr & 2) {
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
      int si = (ack.addr - ADDR_OAM_BEGIN) >> 2;
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

void PPU::tock(const int tcycle) {
  //from_int(tcycle, ibus);

  if ((lcdc & FLAG_LCD_ON) == 0) {
    tock_lcdoff();
    return;
  }

  //----------------------------------------
  // Update state machiney stuff

  const int tphase = tcycle & 3;

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

  /*
  ppu_to_vram.addr = 0;
  ppu_to_oam.addr = 0;

  uint8_t new_map_x = (map_x + (scx >> 3)) & 31;
  uint8_t map_y = ((scy + line) >> 3) & 31;

  if (fetch_type == FETCH_BACKGROUND) {
    if      (fetch_state == FETCH_TILE_MAP) ppu_to_vram.addr = tile_map_address(lcdc, new_map_x, map_y);
    else if (fetch_state == FETCH_TILE_LO)  ppu_to_vram.addr = tile_base_address(lcdc, scy, line, tile_map) + 0;
    else if (fetch_state == FETCH_TILE_HI)  ppu_to_vram.addr = tile_base_address(lcdc, scy, line, tile_map) + 1;
  }
  else if (fetch_type == FETCH_WINDOW) {
    if      (fetch_state == FETCH_TILE_MAP) ppu_to_vram.addr = win_map_address(lcdc, map_x, win_y_latch);
    else if (fetch_state == FETCH_TILE_LO)  ppu_to_vram.addr = win_base_address(lcdc, win_y_latch, tile_map) + 0;
    else if (fetch_state == FETCH_TILE_HI)  ppu_to_vram.addr = win_base_address(lcdc, win_y_latch, tile_map) + 1;
  }
  else if (fetch_type == FETCH_SPRITE) {
    if      (fetch_state == FETCH_SPRITE_MAP) ppu_to_oam.addr = ADDR_OAM_BEGIN + (sprite_index << 2) + 2;
    else if (fetch_state == FETCH_SPRITE_LO)  ppu_to_vram.addr = sprite_base_address(lcdc, line, spriteY, spriteP, spriteF) + 0;
    else if (fetch_state == FETCH_SPRITE_HI)  ppu_to_vram.addr = sprite_base_address(lcdc, line, spriteY, spriteP, spriteF) + 1;
  }

  if (pix_count + pix_discard_pad == 168) {
    ppu_to_oam.addr = 0;
    ppu_to_vram.addr = 0;
  }

  if (ppu_to_vram.addr) ppu_to_vram.read = true;
  if (ppu_to_oam.addr) ppu_to_oam.read = true;
  */

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

  // move to tick()?

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

#if 0
void PPU::bus_write_early(uint16_t addr, uint8_t data) {
  if (ADDR_GPU_BEGIN <= addr && addr <= ADDR_GPU_END) {
    switch (addr) {
    case ADDR_LCDC: {
      lcdc  = lcdc & 0b10000011;
      lcdc |= data & 0b01111100;

      // dmg glitch hack
      if (pix_count == 0) {
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
    case ADDR_BGP:  bgp |= data; break;
    case ADDR_OBP0: obp0 = data; break;
    case ADDR_OBP1: obp1 = data; break;
    };
    update_palettes();
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
        in_window_early = false;
        win_retrig_old = false;
        win_retrig_new = false;
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
      break;
    }
    case ADDR_WY:   wy = data;   break;
    case ADDR_WX:   wx = data;   break;
    };
    update_palettes();
  }
}
#endif

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


void PPU::dump(std::string& d) const {
  sprintf(d, "\002--------------PPU--------------\001\n");

  sprintf(d, "LCDC      %s\n", byte_to_bits(lcdc));
  sprintf(d, "STAT      %s\n", byte_to_bits(stat));
  sprintf(d, "SCY       %d\n", scy);
  sprintf(d, "SCX       %d\n", scx);
  sprintf(d, "LY        %d\n", ly);
  sprintf(d, "LYC       %d\n", lyc);
  //sprintf(d, "DMA       %d\n", dma);
  sprintf(d, "BGP       0x%02x\n", bgp);
  sprintf(d, "OBP0      0x%02x\n", obp0);
  sprintf(d, "OBP1      0x%02x\n", obp1);
  sprintf(d, "WY        %d\n", wy);
  sprintf(d, "WX        %d\n", wx);
  sprintf(d, "wyc       %d\n", win_y_counter);
  sprintf(d, "\n");

  sprintf(d, "frame     %d\n", frame_count);
  sprintf(d, "line      %d\n", line);
  sprintf(d, "state     %s\n", state_names[state]);
  sprintf(d, "counter   %d\n", counter);
  sprintf(d, "hdelay    %d\n", hblank_delay2);
  sprintf(d, "\n");

  sprintf(d, "compare_line  %d\n", compare_line);
  sprintf(d, "stat_int1     %d\n", stat_int1);
  sprintf(d, "stat_int2     %d\n", stat_int2);
  sprintf(d, "old_stat_int1 %d\n", old_stat_int1);
  sprintf(d, "old_stat_int2 %d\n", old_stat_int2);
  sprintf(d, "scx_latch     %d\n", scx_latch);
  sprintf(d, "win_y_latch   %d\n", win_y_latch);
  sprintf(d, "win_y_counter %d\n", win_y_counter);
  sprintf(d, "\n");

  //----------
  // Sprites

  sprintf(d, "sprite_count %d\n",     sprite_count);
  sprintf(d, "sprite_index %d\n",     sprite_index);
  sprintf(d, "sprite_hit   %d\n",     sprite_hit);
  sprintf(d, "spriteY      %d\n",     spriteY);
  sprintf(d, "spriteX      %d\n",     spriteX);
  sprintf(d, "spriteP      %d\n",     spriteP);
  sprintf(d, "spriteF      %d\n",     spriteF);
  sprintf(d, "sprite_lo    0x%02x\n", sprite_lo);
  sprintf(d, "sprite_hi    0x%02x\n", sprite_hi);

  for (int i = 0; i < 10; i++) {
    sprintf(d, "sprite %d %d %d\n", sprite_i[i], sprite_x[i], sprite_y[i]);
  }

  sprintf(d, "\n");

  //----------
  // Vram Fetcher

  sprintf(d, "fetch_type      %s\n", fetch_names1[fetch_type]);
  sprintf(d, "fetch_state     %s\n", fetch_names2[fetch_state]);
  sprintf(d, "fetch_delay     %d\n", fetch_delay);
  sprintf(d, "in_window_old   %d\n", in_window_old);
  sprintf(d, "in_window_new   %d\n", in_window_new);
  sprintf(d, "in_window_early %d\n", in_window_early);
  sprintf(d, "in_window_late  %d\n", in_window_late);
  sprintf(d, "win_retrig_old  %d\n", win_retrig_old);
  sprintf(d, "win_retrig_new  %d\n", win_retrig_new);
  sprintf(d, "\n");

  //----------
  // Pixel pipe

  sprintf(d, "map_x           %d\n", map_x);

  sprintf(d, "tile_map        %d\n", tile_map);
  sprintf(d, "tile_lo         %d\n", tile_lo);
  sprintf(d, "tile_hi         %d\n", tile_hi);
  sprintf(d, "tile_latched    %d\n", tile_latched);

  sprintf(d, "pix_count       %d\n", pix_count);
  sprintf(d, "pix_discard_scx %d\n", pix_discard_scx);
  sprintf(d, "pix_discard_pad %d\n", pix_discard_pad);
  sprintf(d, "pipe_count      %d\n", pipe_count);
  sprintf(d, "\n");

  sprintf(d, "bg_pix_lo       %s\n", byte_to_bits(bg_pix_lo));
  sprintf(d, "bg_pix_hi       %s\n", byte_to_bits(bg_pix_hi));
  sprintf(d, "bg_pal_lo       %s\n", byte_to_bits(bg_pal_lo));
  sprintf(d, "bg_pal_hi       %s\n", byte_to_bits(bg_pal_hi));

  {
    /*
    auto out = tick(tcycle);

    pribus(d, "bus_to_ppu",  bus_to_ppu);
    pribus(d, "ppu_to_bus",  out.ppu_to_bus);
    sprintf(d, "\n");

    pribus(d, "ppu_to_vram", out.ppu_to_vram);
    pribus(d, "vram_to_ppu", vram_to_ppu);
    sprintf(d, "\n");

    pribus(d, "ppu_to_oam",  out.ppu_to_oam);
    pribus(d, "oam_to_ppu",  oam_to_ppu);
    sprintf(d, "\n");
    */

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
