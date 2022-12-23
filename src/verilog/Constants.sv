`ifndef CONSTANTS_SV
`define CONSTANTS_SV

`include "types.sv"

package constants;

import types::*;

//-----------------------------------------------------------------------------
// Model config

parameter uint16_t MODEL_DMG = 0;
parameter uint16_t MODEL_AGS = 1;

//-----------------------------------------------------------------------------
// Audio config

parameter input_hz = 154 * 114 * 60;
parameter output_hz = 48000;

//-----------------------------------------------------------------------------
// Timing config

parameter uint16_t TCYCLES_LINE      =456;
parameter uint16_t TCYCLES_OAM       =80;
parameter uint16_t TCYCLES_VRAM      =172;
parameter uint16_t TCYCLES_HBLANK    =204;

parameter uint8_t HBLANK_DELAY_START  = 8;

//-----------------------------------------------------------------------------
// Memory map

parameter uint16_t ADDR_BOOTROM_BEGIN = 16'h0000;
parameter uint16_t ADDR_BOOTROM_END   = 16'h00FF;

parameter uint16_t ADDR_ROM_BEGIN = 16'h0000;
parameter uint16_t ADDR_ROM_END   = 16'h7FFF;

parameter uint16_t ADDR_VRAM_BEGIN = 16'h8000;

parameter uint16_t ADDR_TILE0      = 16'h8000;
parameter uint16_t ADDR_TILE1      = 16'h8800;
parameter uint16_t ADDR_MAP0       = 16'h9800;
parameter uint16_t ADDR_MAP1       = 16'h9C00;

parameter uint16_t ADDR_VRAM_END   = 16'h9FFF;

parameter uint16_t ADDR_CRAM_BEGIN = 16'hA000;
parameter uint16_t ADDR_CRAM_END   = 16'hBFFF;

parameter uint16_t ADDR_IRAM_BEGIN = 16'hC000;
parameter uint16_t ADDR_IRAM_END   = 16'hDFFF;

parameter uint16_t ADDR_ECHO_BEGIN  = 16'hE000;
parameter uint16_t ADDR_ECHO_END    = 16'hFDFF;

parameter uint16_t ADDR_OAM_BEGIN   = 16'hFE00;
parameter uint16_t ADDR_OAM_END     = 16'hFE9F;

parameter uint16_t ADDR_IOBUS_BEGIN = 16'hFF00;
;
parameter uint16_t ADDR_IOREG_BEGIN = 16'hFF00;
parameter uint16_t ADDR_P1          = 16'hFF00;
parameter uint16_t ADDR_SB          = 16'hFF01;
parameter uint16_t ADDR_SC          = 16'hFF02;

parameter uint16_t ADDR_TIMER_BEGIN = 16'hFF04;
parameter uint16_t ADDR_DIV         = 16'hFF04;
parameter uint16_t ADDR_TIMA        = 16'hFF05;
parameter uint16_t ADDR_TMA         = 16'hFF06;
parameter uint16_t ADDR_TAC         = 16'hFF07;
parameter uint16_t ADDR_TIMER_END   = 16'hFF07;

parameter uint16_t ADDR_IF          = 16'hFF0F;
parameter uint16_t ADDR_IOREG_END   = 16'hFF0F;

parameter uint16_t ADDR_SPU_BEGIN   = 16'hFF10;
parameter uint16_t ADDR_SPU_END     = 16'hFF3F;

parameter uint16_t ADDR_GPU_BEGIN   = 16'hFF40;
parameter uint16_t ADDR_LCDC        = 16'hFF40;
parameter uint16_t ADDR_STAT        = 16'hFF41;
parameter uint16_t ADDR_SCY         = 16'hFF42;
parameter uint16_t ADDR_SCX         = 16'hFF43;
parameter uint16_t ADDR_LY          = 16'hFF44;
parameter uint16_t ADDR_LYC         = 16'hFF45;
parameter uint16_t ADDR_DMA         = 16'hFF46;
parameter uint16_t ADDR_BGP         = 16'hFF47;
parameter uint16_t ADDR_OBP0        = 16'hFF48;
parameter uint16_t ADDR_OBP1        = 16'hFF49;
parameter uint16_t ADDR_WY          = 16'hFF4A;
parameter uint16_t ADDR_WX          = 16'hFF4B;
parameter uint16_t ADDR_GPU_END     = 16'hFF4B;

parameter uint16_t ADDR_DISABLE_BOOTROM     = 16'hFF50;

parameter uint16_t ADDR_UNUSABLE_BEGIN = 16'hFF4C;
parameter uint16_t ADDR_UNUSABLE_END   = 16'hFF47;

parameter uint16_t ADDR_ZEROPAGE_BEGIN = 16'hFF80;
parameter uint16_t ADDR_ZEROPAGE_END   = 16'hFFFE;

parameter uint16_t ADDR_IE     = 16'hFFFF;
parameter uint16_t ADDR_IOBUS_END = 16'hFFFF;

//-----------------------------------------------------------------------------
// Flags

parameter uint8_t INT_VBLANK  = 8'b00000001;
parameter uint8_t INT_STAT    = 8'b00000010;
parameter uint8_t INT_TIMER   = 8'b00000100;
parameter uint8_t INT_SERIAL  = 8'b00001000;
parameter uint8_t INT_JOYPAD  = 8'b00010000;

parameter uint8_t EI_GLITCH    = 8'b10000000;
parameter uint8_t EI_LYC       = 8'b01000000;
parameter uint8_t EI_OAM       = 8'b00100000;
parameter uint8_t EI_VBLANK    = 8'b00010000;
parameter uint8_t EI_HBLANK    = 8'b00001000;

parameter uint8_t STAT_LYC     = 8'b00000100;

parameter uint8_t PPU_STATE_HBLANK = 0;
parameter uint8_t PPU_STATE_VBLANK = 1;
parameter uint8_t PPU_STATE_OAM    = 2;
parameter uint8_t PPU_STATE_VRAM   = 3;

parameter uint8_t FLAG_BG_ON        = 8'h01;
parameter uint8_t FLAG_OBJ_ON       = 8'h02;
parameter uint8_t FLAG_TALL_SPRITES = 8'h04;
parameter uint8_t FLAG_BG_MAP_1     = 8'h08;
parameter uint8_t FLAG_TILE_0       = 8'h10;
parameter uint8_t FLAG_WIN_ON       = 8'h20;
parameter uint8_t FLAG_WIN_MAP_1    = 8'h40;
parameter uint8_t FLAG_LCD_ON       = 8'h80;

parameter uint8_t SPRITE_PRI    = 8'h80;
parameter uint8_t SPRITE_FLIP_Y = 8'h40;
parameter uint8_t SPRITE_FLIP_X = 8'h20;
parameter uint8_t SPRITE_PAL    = 8'h10;

parameter uint8_t TAC_RUN = 8'h04;

//-----------------------------------------------------------------------------

endpackage;

`endif // CONSTANTS_SV
