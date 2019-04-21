
#include "PPU.h"
#include "constants.h"

#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <io.h>

bool posedge(bool& a, bool b) {
  bool result = !a && b;
  a = b;
  return result;
}

//-----------------------------------------------------------------------------
// interrupt glitch - oam stat fires on vblank
// interrupt glitch - writing to stat during hblank/vblank triggers stat interrupt

void PPU::tick(ubit16_t cpu_addr, ubit8_t cpu_data, bool cpu_read, bool cpu_write,
               uint8_t vram_in, uint8_t oam_in) {
  //-----------------------------------
  // Update counter/line/frame

  counter2++;
  if (counter2 == TCYCLES_LINE) {
    counter2 = 0;
    line2++;
  }
  if (line2 == 154) {
    line2 = 0;
    frame_count++;
  }

  //----------

  vblank_int = false;
  vblank_int |= (line2 == 144) && (counter2 == 1);

  //----------
  // DO THE SAME INTERRUPT TESTS FOR LYC_MATCH

  stat_int_lyc = false;
  stat_int_lyc |= lyc_match;
  stat_int_lyc &= (stat & EI_LYC) != 0;

  //----------

  stat_int_oam = false;
  stat_int_oam |= (line2 == 153) && (counter2 == 455);
  stat_int_oam |= (line2  < 143) && (counter2 == 455);
  stat_int_oam |= vblank_int; // glitch
  stat_int_oam &= (stat & EI_OAM) != 0;

  //----------

  stat_int_vblank = false;
  stat_int_vblank |= (line2 == 144) && (counter2 >= 1);
  stat_int_vblank |= (line2 > 144);

  if ((line2 == 153) && (counter2 >= 452)) {
    stat_int_vblank = false;
  }

  stat_int_vblank &= (stat & EI_VBLANK) != 0;

  //----------

  stat_int_hblank = false;
  stat_int_hblank |= (counter2 > 80) && (hblank_delay < HBLANK_DELAY_INT);
 
  if (counter2 >= 452) stat_int_hblank = false;

  stat_int_hblank &= (stat & EI_HBLANK) != 0;

  //----------

  //stat_int_glitch = cpu_write && cpu_addr == ADDR_STAT && (hblank_phase || vblank);
  stat_int_glitch = 0;

  //----------

  stat_int = stat_int_lyc | stat_int_oam | stat_int_hblank | stat_int_vblank | stat_int_glitch;
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

  // FIXME ly handling is still a bit weird, ly changes "early"

  lyc_match = (line2 == lyc);

  if (counter2 == (TCYCLES_LINE - 4)) {
    ly = (line2 == 153) ? 0 : line2 + 1;
    lyc_match = 0;
  }

  // FIXME why 1 here? something to do with video out
  frame_start = (counter2 < 4) && (line2 == 0);
  frame_done = (counter2 < 4) && (line2 == 144);
  bool line0_weirdness = (frame_count == 0 && line2 == 0 && counter2 < 81);

  //-----------------------------------
  // vblank early-out

  if (line2 >= 144) {
    oam_phase = false;
    render_phase = false;
    hblank_phase = false;

    oam_lock = false;
    vram_lock = false;
    stat = ubit8_t(0x80 | (stat & 0b01111000) | (lyc_match << 2) | PPU_STATE_VBLANK);
    oam_addr = 0;
    oam_read = false;
    vram_addr = 0;
    if (cpu_write) bus_write(cpu_addr, cpu_data);
    return;
  }

  //-----------------------------------
  // Update state machiney stuff

  if (counter2 == 0) {
    oam_phase = true;
    render_phase = false;
    hblank_phase = false;

    pix_count = 0;
    sprite_index = -1;
    hblank_delay = HBLANK_DELAY_START;
    sprite_count = 0;

    oam_lock = !line0_weirdness;
    vram_lock = false;
  }
  else if (counter2 < 81) {
    oam_lock = counter2 != 78 && !line0_weirdness;
    vram_lock = counter2 == 80 && !line0_weirdness;
  }
  else if (counter2 == 81) {
    oam_phase = false;
    render_phase = true;
    hblank_phase = false;

    window_hit = false;
    map_x = (scx >> 3) & 31;
    map_y = ((scy + line2) >> 3) & 31;
    pix_discard = (scx & 7) + 8;
    scy_latch = scy;
    tile_latched = true; // we always start w/ a "garbage" tile
  }
  else if (pix_count < 160) {
    oam_lock = true;
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

    oam_lock = !(hblank_delay < HBLANK_DELAY_LOCK);
    vram_lock = !(hblank_delay < HBLANK_DELAY_LOCK);
  }
  else {
    oam_lock = false;
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

  if (oam_phase && (oam_addr & 3) == 1 && sprite_count < 10) {
    int si = (counter2 - 1) >> 1;
    int sy = spriteY - 16;
    int sx = spriteX;

    ubit4_t sprite_height = lcdc & FLAG_TALL_SPRITES ? 15 : 7;
    if ((sx < 168) && (sy <= line2) && (line2 <= sy + sprite_height)) {
      sprite_x[sprite_count] = spriteX;
      sprite_y[sprite_count] = spriteY;
      sprite_i[sprite_count] = (uint8_t)si;
      sprite_count++;
    }
  }

  //-----------------------------------
  // Render phase

  oam_addr = 0;
  oam_read = false;

  if (oam_phase) {
    oam_addr = _pdep_u32(counter2, 0b11111101);
    oam_addr += ADDR_OAM_BEGIN;
    oam_read = true;
  }


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
          vram_addr = win_map_address(lcdc, map_x, map_y);
        }
        else {
          vram_addr = tile_map_address(lcdc, map_x, map_y);
        }
        map_x = (map_x + 1) & 31;
      }
      else if (fetch_state == FETCH_TILE_LO) {
        if (window_hit) {
          vram_addr = win_base_address(lcdc, wy, line2, tile_map) + 0;
        }
        else {
          vram_addr = tile_base_address(lcdc, scy_latch, line2, tile_map) + 0;
        }
      }
      else if (fetch_state == FETCH_TILE_HI) {
        if (window_hit) {
          vram_addr = win_base_address(lcdc, wy, line2, tile_map) + 1;
        }
        else {
          vram_addr = tile_base_address(lcdc, scy_latch, line2, tile_map) + 1;
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

  if (cpu_write) bus_write(cpu_addr, cpu_data);
}

//-----------------------------------------------------------------------------
