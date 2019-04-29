#pragma once

//-----------------------------------------------------------------------------
// Model config

#define MODEL_DMG 0
#define MODEL_AGS 1

//-----------------------------------------------------------------------------
// Audio config

const int64_t input_hz = 154 * 114 * 60;
const int64_t output_hz = 48000;
const int samples_per_frame = output_hz / 60;

//-----------------------------------------------------------------------------
// Timing config

#define TCYCLES_LINE      456
#define TCYCLES_OAM       80
#define TCYCLES_VRAM      172
#define TCYCLES_HBLANK    204

#define HBLANK_DELAY_START  8

#define HBLANK_DELAY_PHASE  7
#define HBLANK_DELAY_LOCK   5
#define HBLANK_DELAY_STATE  5
#define HBLANK_DELAY_INT    6

#define PHASE_CPU_TICK  0
#define PHASE_DMA_TOCK  0
#define PHASE_CPU_WRITE 1
#define PHASE_SPU_TOCK  2
#define PHASE_CPU_TOCK  3
#define PHASE_CPU_READ  3

//-----------------------------------------------------------------------------
// Memory map

#define ADDR_BOOTROM_BEGIN 0x0000
#define ADDR_BOOTROM_END   0x00FF

#define ADDR_ROM_BEGIN 0x0000
#define ADDR_ROM_END   0x7FFF

#define ADDR_VRAM_BEGIN 0x8000

#define ADDR_TILE0      0x8000
#define ADDR_TILE1      0x8800
#define ADDR_MAP0       0x9800
#define ADDR_MAP1       0x9C00

#define ADDR_VRAM_END   0x9FFF

#define ADDR_CRAM_BEGIN 0xA000
#define ADDR_CRAM_END   0xBFFF

#define ADDR_IRAM_BEGIN 0xC000
#define ADDR_IRAM_END   0xDFFF

#define ADDR_ECHO_BEGIN  0xE000
#define ADDR_ECHO_END    0xFDFF

#define ADDR_OAM_BEGIN   0xFE00
#define ADDR_OAM_END     0xFE9F

#define ADDR_IOBUS_BEGIN 0xFF00

#define ADDR_IOREG_BEGIN 0xFF00
#define ADDR_P1          0xFF00
#define ADDR_SB          0xFF01
#define ADDR_SC          0xFF02

#define ADDR_TIMER_BEGIN 0xFF04
#define ADDR_DIV         0xFF04
#define ADDR_TIMA        0xFF05
#define ADDR_TMA         0xFF06
#define ADDR_TAC         0xFF07
#define ADDR_TIMER_END   0xFF07

#define ADDR_IF          0xFF0F
#define ADDR_IOREG_END   0xFF0F

#define ADDR_SPU_BEGIN   0xFF10
#define ADDR_SPU_END     0xFF3F

#define ADDR_GPU_BEGIN   0xFF40
#define ADDR_LCDC        0xFF40
#define ADDR_STAT        0xFF41
#define ADDR_SCY         0xFF42
#define ADDR_SCX         0xFF43
#define ADDR_LY          0xFF44
#define ADDR_LYC         0xFF45
#define ADDR_DMA         0xFF46
#define ADDR_BGP         0xFF47
#define ADDR_OBP0        0xFF48
#define ADDR_OBP1        0xFF49
#define ADDR_WY          0xFF4A
#define ADDR_WX          0xFF4B
#define ADDR_GPU_END     0xFF4B

#define ADDR_DISABLE_BOOTROM     0xFF50

#define ADDR_UNUSABLE_BEGIN 0xFF4C
#define ADDR_UNUSABLE_END   0xFF47

#define ADDR_ZEROPAGE_BEGIN 0xFF80
#define ADDR_ZEROPAGE_END   0xFFFE

#define ADDR_IE     0xFFFF
#define ADDR_IOBUS_END 0xFFFF

//-----------------------------------------------------------------------------
// Flags

#define INT_VBLANK  0b00000001
#define INT_STAT    0b00000010
#define INT_TIMER   0b00000100
#define INT_SERIAL  0b00001000
#define INT_JOYPAD  0b00010000

#define EI_LYC       0b01000000
#define EI_OAM       0b00100000
#define EI_VBLANK    0b00010000
#define EI_HBLANK    0b00001000

#define PPU_STATE_HBLANK 0
#define PPU_STATE_VBLANK 1
#define PPU_STATE_OAM    2
#define PPU_STATE_VRAM   3

#define FLAG_BG_ON        0x01
#define FLAG_OBJ_ON       0x02
#define FLAG_TALL_SPRITES 0x04
#define FLAG_BG_MAP_1     0x08
#define FLAG_TILE_0       0x10
#define FLAG_WIN_ON       0x20
#define FLAG_WIN_MAP_1    0x40
#define FLAG_LCD_ON       0x80

#define SPRITE_PRI    0x80
#define SPRITE_FLIP_Y 0x40
#define SPRITE_FLIP_X 0x20
#define SPRITE_PAL    0x10

#define TAC_RUN 0x04

//-----------------------------------------------------------------------------
