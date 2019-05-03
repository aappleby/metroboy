#include "Platform.h"
#include "PPU.h"

#include "Common.h"
#include "Constants.h"

uint16_t pack_map_addr(uint16_t base, uint8_t map_x, uint8_t map_y);
uint16_t pack_tile_addr(uint16_t base, uint8_t tile, uint8_t ty);
uint16_t tile_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y);
uint16_t win_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y);
uint16_t tile_base_address(uint8_t lcdc, uint8_t scy, uint8_t line, uint8_t map);
uint16_t win_base_address(uint8_t lcdc, uint8_t wy, uint8_t line, uint8_t map);
uint16_t sprite_base_address(uint8_t lcdc, uint8_t line, uint8_t sprite_y, uint8_t map, uint8_t flags);

//-----------------------------------------------------------------------------

void PPU::reset(int new_model) {
  model = new_model;

  vblank_int = 0;
  stat_int = 0;
  stat_int_lyc = 0;
  stat_int_oam = 0;
  stat_int_hblank = 0;
  stat_int_vblank = 0;
  stat_int_glitch = 0;

  bus_out = 0;
  bus_oe = 0;

  pix_out = 0;
  pix_oe = 0;

  vram_addr = 0;
  vram_delay = false;

  oam_lock = false;
  vram_lock = false;

  //----------
  // Registers

  lcdc = 0;
  stat = 0;
  scy = 0;
  scx = 0;
  ly = 0;
  lyc = 0;
  dma = 0xFF;
  bgp = 0xFC;
  obp0 = 0xFF;
  obp1 = 0xFF;
  wy = 0;
  wx = 0;

  palettes[0] = 0;
  palettes[1] = 0;
  palettes[2] = 0;
  palettes[3] = 0;

  scy_latch = 0;

  lyc_match = 0;

  //----------
  // Timers and states

  counter2 = 4;
  line2 = 0;

  frame_start = 0;
  frame_done = 0;
  frame_count = 0;

  oam_phase = false;
  render_phase = false;
  hblank_phase = false;

  hblank_delay = HBLANK_DELAY_START;

  //----------
  // Sprites

  oam_addr = 0;
  oam_data = 0;
  oam_read = false;

  sprite_count = 0;
  sprite_index = -1;
  sprite_latched = 0;
  for (int i = 0; i < 10; i++) sprite_x[i] = 0xFF;
  for (int i = 0; i < 10; i++) sprite_y[i] = 0xFF;
  for (int i = 0; i < 10; i++) sprite_i[i] = 0xFF;

  spriteY = 0;
  spriteX = 0;
  spriteP = 0;
  spriteF = 0;

  sprite_lo = 0;
  sprite_hi = 0;

  //----------
  // Pixel pipe

  fetch_state = FETCH_IDLE;
  window_hit = 0;

  tile_map = 0;
  tile_lo = 0;
  tile_hi = 0;
  tile_latched = 0;

  pix_count = 0;
  pipe_count = 0;
  pix_discard = 0;

  bg_pix_lo = 0;
  bg_pix_hi = 0;
  bg_pal_lo = 0;
  bg_pal_hi = 0;

  ob_pix_lo = 0;
  ob_pix_hi = 0;
  ob_pal_lo = 0;
  ob_pal_hi = 0;

  //----------
  // Fixup if we're not running the bootrom

  line2 = 153;
  counter2 = 399;

  line1 = 153;
  counter1 = 400;

  line0 = 153;
  counter0 = 401;

  lcdc = 0x91;
  palettes[0] = 0xfc;
  oam_phase = false;
  render_phase = false;
  hblank_phase = false;
  pix_count = 160;
}


//-----------------------------------------------------------------------------
// interrupt glitch - oam stat fires on vblank
// interrupt glitch - writing to stat during hblank/vblank triggers stat interrupt

void PPU::tick(ubit16_t /*cpu_addr*/, ubit8_t /*cpu_data*/, bool /*cpu_read*/, bool /*cpu_write*/,
               uint8_t /*vram_in*/, uint8_t /*oam_in*/) {
}

//-----------------------------------------------------------------------------

void PPU::tock(ubit16_t cpu_addr, ubit8_t cpu_data, bool cpu_read, bool cpu_write,
               uint8_t vram_in, uint8_t oam_in) {

  bus_oe = 0;
  bus_out = 0;
  if (cpu_read) bus_read(cpu_addr);

  bool lcd_on = (lcdc & FLAG_LCD_ON) != 0;

  if (!lcd_on) {
    handle_lcd_off(cpu_addr, cpu_data, cpu_read, cpu_write);
    return;
  }

  // FIXME why 1 here? something to do with video out
  frame_start = (counter2 < 4) && (line2 == 0);
  frame_done = (counter2 < 4) && (line2 == 144);
  bool line0_weirdness = (frame_count == 0 && line2 == 0 && counter2 < 80);

  //-----------------------------------
  // vblank early-out

  if (line2 >= 144) {
    oam_phase = false;
    render_phase = false;
    hblank_phase = false;
    hblank_delay = HBLANK_DELAY_START;

    oam_lock = false;
    vram_lock = false;

    int state = PPU_STATE_VBLANK;
    if (line2 == 153 && counter2 == 454) {
      state = PPU_STATE_HBLANK;

      bool lyc_match_2 = lyc == 0;
      stat = ubit8_t(0x80 | (stat & 0b01111000) | (lyc_match_2 << 2) | state);
    }
    else {
      stat = ubit8_t(0x80 | (stat & 0b01111000) | (lyc_match << 2) | state);
    }

    oam_addr = 0;
    oam_read = false;
    vram_addr = 0;
    if (cpu_write) bus_write(cpu_addr, cpu_data);
    return;
  }

  //-----------------------------------
  // Update state machiney stuff

  if (counter2 < 77) {
    oam_phase = true;
    render_phase = false;
    hblank_phase = false;

    pix_count = 0;
    hblank_delay = HBLANK_DELAY_START;

    vram_lock = false;
  }
  else if (counter2 == 77) {
    vram_lock = false;
  }
  else if (counter2 == 78) {
    vram_lock = false;
  }
  else if (counter2 == 79) {
    vram_lock = false;
  }
  else if (counter2 == 80) {
    vram_lock = true;
  }
  else if (counter2 == 81) {
    vram_lock = true;

    oam_phase = false;
    render_phase = true;
    hblank_phase = false;

    sprite_index = -1;
    window_hit = false;
    map_x = (scx >> 3) & 31;
    pix_discard = (scx & 7) + 8;
    tile_latched = true; // we always start w/ a "garbage" tile
  }
  else if (pix_count < 160) {
    vram_lock = true;
  }
  else if (hblank_delay) {
    fetch_state = FETCH_IDLE;
    sprite_latched = false;
    tile_latched = false;
    window_hit = false;
    pipe_count = 0;
    vram_addr = 0;

    hblank_delay--;

    if (hblank_delay < HBLANK_DELAY_PHASE) {
      oam_phase = false;
      render_phase = false;
      hblank_phase = true;
    }

    vram_lock = !(hblank_delay < 8);
  }
  else if (counter2 >= TCYCLES_LINE - 2) { // 0 1 2
    vram_lock = false;
    sprite_index = -1;
    sprite_count = 0;
  }
  else {
    vram_lock = false;
  }

  //-----------------------------------

  int                  state = PPU_STATE_HBLANK;
  if (render_phase)    state = PPU_STATE_VRAM;
  if (hblank_delay < HBLANK_DELAY_STATE)   state = PPU_STATE_HBLANK;
  if (oam_phase)       state = PPU_STATE_OAM;
  if (line0_weirdness) state = PPU_STATE_HBLANK;

  stat = ubit8_t(0x80 | (stat & 0b01111000) | (lyc_match << 2) | state);

  //-----------------------------------
  // Handle OAM reads from the previous cycle

  if (oam_read && (oam_addr & 3) == 0) spriteY = oam_in;
  if (oam_read && (oam_addr & 3) == 1) spriteX = oam_in;
  if (oam_read && (oam_addr & 3) == 2) spriteP = oam_in;
  if (oam_read && (oam_addr & 3) == 3) spriteF = oam_in;

  if (oam_read && (counter0 <= 80) && (oam_addr & 3) == 1 && sprite_count < 10) {
    int si = (counter0 - 1) >> 1;
    int sy = spriteY - 16;
    int sx = spriteX;

    ubit4_t sprite_height = lcdc & FLAG_TALL_SPRITES ? 15 : 7;
    if ((sx < 168) && (sy <= line0) && (line0 <= sy + sprite_height)) {
      sprite_x[sprite_count] = spriteX;
      sprite_y[sprite_count] = spriteY;
      sprite_i[sprite_count] = (uint8_t)si;
      sprite_count++;
    }
  }

  // If we don't do this early, the right twirler in gejmboj is broken
  // (but it could also be a timing issue with the lyc int?)
  if (cpu_write) bus_write(cpu_addr, cpu_data);

  oam_addr = 0;
  oam_read = false;

  // FIXME
  if (frame_count == 0 && line0 == 0 && counter0 < 82) {
    oam_addr = 0;
    oam_read = false;
    oam_lock = false;
  }
  else if (line0 < 144 && counter0 < 82) {
    oam_addr = ((counter0 << 1) & 0b11111100) | (counter0 & 1);
    oam_addr += ADDR_OAM_BEGIN;
    oam_read = true;
    oam_lock = true;
  }
  else {
    // 5 6 7 8
    oam_lock = (line0 < 144) && (hblank_delay >= 3);
  }

  //-----------------------------------
  // Render phase

  if (render_phase) {
    if (vram_delay) {
      vram_delay = 0;
    }
    else if (vram_addr) {
      if (fetch_state == FETCH_TILE_MAP)  tile_map = vram_in;
      if (fetch_state == FETCH_TILE_LO)   tile_lo = vram_in;
      if (fetch_state == FETCH_TILE_HI) { tile_hi = vram_in; tile_latched = 1; }
      if (fetch_state == FETCH_SPRITE_LO) sprite_lo = vram_in;
      if (fetch_state == FETCH_SPRITE_HI) { sprite_hi = vram_in; sprite_latched = 1; }
      vram_addr = 0;
    }

    merge_sprite();
    check_sprite_hit();
    emit_pixel();
    merge_tile();

    // Slightly broken
    if ((lcdc & FLAG_WIN_ON) && !window_hit && (line2 >= wy)) {
      if (pix_count + pix_discard == wx + 1) {
        window_hit = true;
        fetch_state = FETCH_IDLE;
        pipe_count = 0;
        tile_latched = false;
        map_x = 0;
        map_y = (line2 - wy) >> 3;
        vram_addr = 0;
        vram_delay = 0;

        bg_pix_lo = 0;
        bg_pix_hi = 0;
        bg_pal_lo = 0;
        bg_pal_hi = 0;
      }
    }

    // Still feels too complicated
    if (vram_addr == 0) {
      if      (fetch_state == FETCH_TILE_MAP)   fetch_state = FETCH_TILE_LO;
      else if (fetch_state == FETCH_TILE_LO)    fetch_state = FETCH_TILE_HI;
      else if (fetch_state == FETCH_TILE_HI)    fetch_state = FETCH_IDLE;
      else if (fetch_state == FETCH_SPRITE_MAP) fetch_state = FETCH_SPRITE_LO;
      else if (fetch_state == FETCH_SPRITE_LO)  fetch_state = FETCH_SPRITE_HI;
      else if (fetch_state == FETCH_SPRITE_HI)  fetch_state = FETCH_IDLE;

      if (fetch_state == FETCH_IDLE) {
        if (sprite_index != -1) fetch_state = FETCH_SPRITE_MAP;
        if (!tile_latched) fetch_state = FETCH_TILE_MAP;
      }

      if (fetch_state == FETCH_TILE_MAP) {
        if (window_hit) {
          map_y = ((line2 - wy) >> 3) & 31;
          vram_addr = win_map_address(lcdc, map_x, map_y);
        }
        else {
          map_y = ((scy + line2) >> 3) & 31;
          vram_addr = tile_map_address(lcdc, map_x, map_y);
        }
        map_x = (map_x + 1) & 31;
      }
      else if (fetch_state == FETCH_TILE_LO) {
        if (window_hit) {
          vram_addr = win_base_address(lcdc, wy, line2, tile_map) + 0;
        }
        else {
          vram_addr = tile_base_address(lcdc, scy, line2, tile_map) + 0;
        }
      }
      else if (fetch_state == FETCH_TILE_HI) {
        if (window_hit) {
          vram_addr = win_base_address(lcdc, wy, line2, tile_map) + 1;
        }
        else {
          vram_addr = tile_base_address(lcdc, scy, line2, tile_map) + 1;
        }
      }
      else if (fetch_state == FETCH_SPRITE_MAP) {
        // bogus address just to keep the state machine running
        vram_addr = ADDR_VRAM_BEGIN;
      }
      else if (fetch_state == FETCH_SPRITE_LO) {
        vram_addr = sprite_base_address(lcdc, line2, spriteY, spriteP, spriteF) + 0;
      }
      else if (fetch_state == FETCH_SPRITE_HI) {
        vram_addr = sprite_base_address(lcdc, line2, spriteY, spriteP, spriteF) + 1;
      }
      else {
        vram_addr = 0;
      }

      if (vram_addr != 0) {
        vram_delay = true;
      }
    }

    if (fetch_state == FETCH_SPRITE_MAP) {
      oam_addr = (sprite_index << 2) + (counter2 & 1) + 2;
      oam_addr += ADDR_OAM_BEGIN;
      oam_read = true;
    }
  }

  //-----------------------------------

}

//-----------------------------------------------------------------------------

void PPU::handle_lcd_off(ubit16_t cpu_addr, ubit8_t cpu_data, bool cpu_read, bool cpu_write) {
  if (cpu_read) bus_read(cpu_addr);

  counter2 &= 3;
  counter1 &= 3;
  counter0 &= 3;

  line2 = 0;
  line1 = 0;
  line0 = 0;

  ly = 0;
  frame_count = 0;
  frame_done = false;
  frame_start = false;

  oam_phase = false;
  render_phase = false;
  hblank_phase = true;
  hblank_delay = HBLANK_DELAY_START;
  fetch_state = FETCH_IDLE;


  pix_count = 0;
  pix_oe = false;
  sprite_count = 0;
  sprite_index = -1;
  tile_latched = false;

  vram_addr = 0;
  oam_addr = 0;
  oam_data = 0;
  oam_read = false;

  //stat = ubit8_t(0x80 | (stat & 0b11111000) | (lyc_match << 2));
  stat = ubit8_t(0x80 | (stat & 0b11111100));

  if (cpu_write) bus_write(cpu_addr, cpu_data);
}

//-----------------------------------------------------------------------------

uint16_t pack_map_addr(uint16_t base, uint8_t map_x, uint8_t map_y) {
  return base + (map_y << 5) + map_x;
}

uint16_t pack_tile_addr(uint16_t base, uint8_t tile, uint8_t ty) {
  return base + (tile << 4) + (ty << 1);
}

uint16_t tile_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y) {
  ubit16_t base = (lcdc & FLAG_BG_MAP_1) ? ADDR_MAP1 : ADDR_MAP0;
  return pack_map_addr(base, map_x, map_y);
}

uint16_t win_map_address(uint8_t lcdc, uint8_t map_x, uint8_t map_y) {
  ubit16_t base = (lcdc & FLAG_WIN_MAP_1) ? ADDR_MAP1 : ADDR_MAP0;
  return pack_map_addr(base, map_x, map_y);
}

uint16_t tile_base_address(uint8_t lcdc, uint8_t scy, uint8_t line, uint8_t map) {
  ubit16_t base = (lcdc & FLAG_TILE_0) ? ADDR_TILE0 : ADDR_TILE1;
  map = (lcdc & FLAG_TILE_0) ? map : map ^ 0x80;
  uint8_t ty = (scy + line) & 7;

  return pack_tile_addr(base, map, ty);
}

uint16_t win_base_address(uint8_t lcdc, uint8_t wy, uint8_t line, uint8_t map) {
  ubit16_t base = (lcdc & FLAG_TILE_0) ? ADDR_TILE0 : ADDR_TILE1;
  map = (lcdc & FLAG_TILE_0) ? map : map ^ 0x80;
  return pack_tile_addr(base, map, (line - wy) & 7);
}

uint16_t sprite_base_address(uint8_t lcdc, uint8_t line, uint8_t sprite_y, uint8_t map, uint8_t flags) {
  if (lcdc & FLAG_TALL_SPRITES) map &= 0xFE;
  ubit4_t sprite_height = lcdc & FLAG_TALL_SPRITES ? 15 : 7;
  uint8_t sprite_dy = line + 16 - sprite_y;
  if (flags & SPRITE_FLIP_Y) sprite_dy ^= sprite_height;

  return pack_tile_addr(ADDR_TILE0, map, sprite_dy);
}


//-----------------------------------------------------------------------------

void PPU::merge_sprite() {
  if (!sprite_latched) return;

  if (spriteF & SPRITE_FLIP_X) {
    sprite_lo = flip2(sprite_lo);
    sprite_hi = flip2(sprite_hi);
  }

  ubit8_t sprite_pal_lo = spriteF & SPRITE_PAL ? 0b11111111 : 0b00000000;
  ubit8_t sprite_pal_hi = 0b11111111;

  // sprites don't draw where we already drew sprites
  ubit8_t mask = ob_pix_lo | ob_pix_hi;

  ob_pix_lo |= (sprite_lo & ~mask);
  ob_pix_hi |= (sprite_hi & ~mask);
  ob_pal_lo |= (sprite_pal_lo & ~mask);
  ob_pal_hi |= (sprite_pal_hi & ~mask);

  sprite_latched = false;
  sprite_index = -1;
}

//-----------------------------------------------------------------------------

void PPU::check_sprite_hit() {
  if (sprite_index != -1) return;
  if (!(lcdc & FLAG_OBJ_ON)) return;

  ubit4_t hit = 15;
  int next_pix = pix_count + 8 - pix_discard;

  if (next_pix == sprite_x[9]) hit = 9;
  if (next_pix == sprite_x[8]) hit = 8;
  if (next_pix == sprite_x[7]) hit = 7;
  if (next_pix == sprite_x[6]) hit = 6;
  if (next_pix == sprite_x[5]) hit = 5;
  if (next_pix == sprite_x[4]) hit = 4;
  if (next_pix == sprite_x[3]) hit = 3;
  if (next_pix == sprite_x[2]) hit = 2;
  if (next_pix == sprite_x[1]) hit = 1;
  if (next_pix == sprite_x[0]) hit = 0;

  if (hit != 15) {
    sprite_index = sprite_i[hit];
    spriteX = sprite_x[hit];
    spriteY = sprite_y[hit];

    sprite_i[hit] = 255;
    sprite_x[hit] = 255;
    sprite_y[hit] = 255;
  }
}

//-----------------------------------------------------------------------------
// Emit pixel if we have some in the pipe and we're not stalled.

void PPU::emit_pixel() {
  pix_oe = false;
  pix_out = 0;

  if (pipe_count == 0) return;
  if (sprite_index != -1) return;

  int bg_pix = ((bg_pix_hi >> 6) & 2) | ((bg_pix_lo >> 7) & 1);
  int bg_pal = ((bg_pal_hi >> 6) & 2) | ((bg_pal_lo >> 7) & 1);
  int ob_pix = ((ob_pix_hi >> 6) & 2) | ((ob_pix_lo >> 7) & 1);
  int ob_pal = ((ob_pal_hi >> 6) & 2) | ((ob_pal_lo >> 7) & 1);

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

  if (pix_discard || pix_count == 160) {
    pix_oe = false;
    pix_out = 0;
    if (pix_discard) pix_discard--;
  }
  else {
    pix_oe = true;
    pix_out = (palettes[pal] >> (pix << 1)) & 3;
    pix_count = pix_count + 1;
  }
}

//-----------------------------------------------------------------------------

void PPU::merge_tile() {
  if (pipe_count) return;
  if (!tile_latched) return;

  bg_pix_lo = tile_lo;
  bg_pix_hi = tile_hi;
  pipe_count = 8;
  tile_latched = 0;
}

//-----------------------------------------------------------------------------

void PPU::bus_read(uint16_t addr) {
  bus_oe = 0;
  bus_out = 0;

  if (ADDR_GPU_BEGIN <= addr && addr <= ADDR_GPU_END) {
    bus_oe = 1;
    switch (addr) {
    case ADDR_LCDC: bus_out = lcdc; break;
    case ADDR_STAT: bus_out = stat; break;
    case ADDR_SCY:  bus_out = scy; break;
    case ADDR_SCX:  bus_out = scx; break;
    case ADDR_LY:   bus_out = ly; break;
    case ADDR_LYC:  bus_out = lyc; break;
    case ADDR_DMA:  bus_out = dma; break;
    case ADDR_BGP:  bus_out = bgp; break;
    case ADDR_OBP0: bus_out = obp0; break;
    case ADDR_OBP1: bus_out = obp1; break;
    case ADDR_WY:   bus_out = wy; break;
    case ADDR_WX:   bus_out = wx; break;
    }
  }
}

void PPU::bus_write(uint16_t addr, uint8_t data) {
  if (ADDR_GPU_BEGIN <= addr && addr <= ADDR_GPU_END) {
    switch (addr) {
    case ADDR_LCDC: lcdc = data; break;
    case ADDR_STAT: {
      stat = (stat & 0b10000111) | (data & 0b01111000);
      break;
    }
    case ADDR_SCY:  scy = data;  break;
    case ADDR_SCX:  scx = data;  break;
    case ADDR_LY:   ly = data;   break;
    case ADDR_LYC:  lyc = data;  break;
    case ADDR_DMA:  dma = data;  break;
    case ADDR_BGP:  bgp = palettes[0] = data; break;
    case ADDR_OBP0: obp0 = palettes[2] = data; break;
    case ADDR_OBP1: obp1 = palettes[3] = data; break;
    case ADDR_WY:   wy = data;   break;
    case ADDR_WX:   wx = data;   break;
    };
  }
}

//-----------------------------------------------------------------------------

char* PPU::dump(char* cursor) {
  cursor += sprintf(cursor, "LCDC %s\n", to_binary(lcdc));
  cursor += sprintf(cursor, "STAT %s\n", to_binary(stat));
  cursor += sprintf(cursor, "SCY  %d\n", scy);
  cursor += sprintf(cursor, "SCY+ %d\n", scy_latch);
  cursor += sprintf(cursor, "SCX  %d\n", scx);
  cursor += sprintf(cursor, "LY   %d\n", ly);
  cursor += sprintf(cursor, "LYC  %d\n", lyc);
  cursor += sprintf(cursor, "lyc match %d\n", lyc_match);
  cursor += sprintf(cursor, "DMA  %d\n", dma);
  cursor += sprintf(cursor, "BGP  0x%02x\n", palettes[0]);
  cursor += sprintf(cursor, "OBP0 0x%02x\n", palettes[2]);
  cursor += sprintf(cursor, "OBP1 0x%02x\n", palettes[3]);
  cursor += sprintf(cursor, "WY   %d\n", wy);
  cursor += sprintf(cursor, "WX   %d\n", wx);
  cursor += sprintf(cursor, "\n");

  const char* bus_names[] = {
    "FETCH_TILE_MAP",
    "FETCH_TILE_LO",
    "FETCH_TILE_HI",
    "FETCH_SPRITE_MAP",
    "FETCH_SPRITE_LO",
    "FETCH_SPRITE_HI",
    "FETCH_IDLE",
    "FETCH_DONE",
  };

  cursor += sprintf(cursor, "frame   %d\n", frame_count);
  cursor += sprintf(cursor, "%s %s %s %s\n",
    oam_phase ? "OAM" : "   ",
    render_phase ? "VRM" : "   ",
    hblank_phase ? "HBK" : "   ",
    line2 >= 144 ? "VBK" : "   ");
  if (stat_int) {
    cursor += sprintf(cursor, "%s %s %s %s %s\n",
      stat_int_lyc ? "#LYC" : "    ",
      stat_int_oam ? "#OAM" : "    ",
      stat_int_hblank ? "#HBK" : "    ",
      stat_int_vblank ? "#VBK" : "    ",
      stat_int_glitch ? "#GLT" : "    ");
  }
  else {
    cursor += sprintf(cursor, "%s %s %s %s %s\n",
      stat_int_lyc ? "-LYC" : "    ",
      stat_int_oam ? "-OAM" : "    ",
      stat_int_hblank ? "-HBK" : "    ",
      stat_int_vblank ? "-VBK" : "    ",
      stat_int_glitch ? "-GLT" : "    ");
  }

  cursor += sprintf(cursor, "clock2  %3d:%3d\n", line2, counter2);
  cursor += sprintf(cursor, "clock1  %3d:%3d\n", line1, counter1);
  cursor += sprintf(cursor, "clock0  %3d:%3d\n", line0, counter0);

  cursor += sprintf(cursor, "hbdly   %d\n", hblank_delay);
  cursor += sprintf(cursor, "vblank int %d\n", vblank_int);
  cursor += sprintf(cursor, "stat int %d\n", stat_int);
  cursor += sprintf(cursor, "\n");

  cursor += sprintf(cursor, "map x   %d\n", map_x);
  cursor += sprintf(cursor, "map y   %d\n", map_y);

  cursor += sprintf(cursor, "discard %d\n", pix_discard);
  cursor += sprintf(cursor, "pix     %d\n", pix_count);
  cursor += sprintf(cursor, "pipe    %d\n", pipe_count);
  cursor += sprintf(cursor, "fetch   %s\n", bus_names[fetch_state]);
  cursor += sprintf(cursor, "latched %d\n", tile_latched);
  cursor += sprintf(cursor, "\n");

  cursor += sprintf(cursor, "sprite idx %d\n", sprite_index);
  cursor += sprintf(cursor, "oam addr  %04x\n", oam_addr);
  cursor += sprintf(cursor, "oam read  %04x\n", oam_read);
  cursor += sprintf(cursor, "vram addr  %04x\n", vram_addr);
  cursor += sprintf(cursor, "vram delay %d\n", vram_delay);
  cursor += sprintf(cursor, "\n");

  cursor += sprintf(cursor, "stat_int_lyc    %d %d\n", stat_int_lyc, bool(stat & EI_LYC));
  cursor += sprintf(cursor, "stat_int_oam    %d %d\n", stat_int_oam, bool(stat & EI_OAM));
  cursor += sprintf(cursor, "stat_int_vblank %d %d\n", stat_int_vblank, bool(stat & EI_VBLANK));
  cursor += sprintf(cursor, "stat_int_hblank %d %d\n", stat_int_vblank, bool(stat & EI_HBLANK));

  /*
  cursor += sprintf(cursor, "spriteX %d\n", spriteX);
  cursor += sprintf(cursor, "spriteY %d\n", spriteY);
  cursor += sprintf(cursor, "spriteP %d\n", spriteP);
  cursor += sprintf(cursor, "spriteF %d\n", spriteF);
  cursor += sprintf(cursor, "\n");
  */

  uint8_t* si = sprite_i;
  uint8_t* sx = sprite_x;
  uint8_t* sy = sprite_y;

  cursor += sprintf(cursor, "sprite_i [%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d]\n", si[0], si[1], si[2], si[3], si[4], si[5], si[6], si[7], si[8], si[9]);
  cursor += sprintf(cursor, "sprite_x [%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d]\n", sx[0], sx[1], sx[2], sx[3], sx[4], sx[5], sx[6], sx[7], sx[8], sx[9]);
  cursor += sprintf(cursor, "sprite_y [%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d]\n", sy[0], sy[1], sy[2], sy[3], sy[4], sy[5], sy[6], sy[7], sy[8], sy[9]);

  cursor += sprintf(cursor, "\n");

  return cursor;
}

//-----------------------------------------------------------------------------

void draw_tile(uint32_t* framebuffer, int stride, int x, int y, int scale,
  const uint8_t* tile, uint32_t pal, bool flip_x, bool flip_y) {
  for (int ty = 0; ty < 8; ty++) {
    int y2 = flip_y ? 7 - ty : ty;
    uint8_t lo = tile[y2 * 2 + 0];
    uint8_t hi = tile[y2 * 2 + 1];

    if (flip_x) {
      lo = flip(lo);
      hi = flip(hi);
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

      *lineA++ = color; *lineA++ = color;
      *lineB++ = color; *lineB++ = color;
    }
  }
}

//-------------------------------------

void PPU::draw_sprite(OAM& oam, uint32_t* framebuffer, int stride, int sx, int sy, int scale,
  const uint8_t* vram2, int sprite_index2) const {
  const Sprite& s = oam.get_sprite(sprite_index2);
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
