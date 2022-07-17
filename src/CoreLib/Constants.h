#pragma once
#include "CoreLib/Types.h"

extern uint8_t DMG_ROM_bin[];

extern const char* op_strings[256];
extern const char* op_strings2[256];

constexpr uint16_t BOOT_DIV = 0xEAF3;
constexpr uint16_t VRAM_ADDR_MASK = 0b1111111111111;

//-----------------------------------------------------------------------------
// Audio config

constexpr int64_t input_hz = 154 * 114 * 60;
constexpr int64_t output_hz = 48000;
constexpr int samples_per_frame = output_hz / 60;

//-----------------------------------------------------------------------------
// Timing config

constexpr double TCYCLES_PER_SECOND = 4194304.0;
constexpr double PHASES_PER_SECOND = TCYCLES_PER_SECOND * 2.0;

#define TCYCLES_LINE      456
#define TCYCLES_OAM       80
#define TCYCLES_VRAM      172
#define TCYCLES_HBLANK    204

#define HBLANK_DELAY_START  8

#define MCYCLES_PER_FRAME (114 * 154)

//-----------------------------------------------------------------------------
// Memory map

#define ADDR_BOOT_ROM_BEGIN  0x0000
#define ADDR_BOOT_ROM_END    0x00FF

#define ADDR_CART_ROM_BEGIN  0x0000
#define ADDR_CART_ROM_END    0x7FFF

#define ADDR_VRAM_BEGIN      0x8000
#define ADDR_VRAM_END        0x9FFF

#define ADDR_CART_RAM_BEGIN  0xA000
#define ADDR_CART_RAM_END    0xBFFF

#define ADDR_MAIN_RAM_BEGIN  0xC000
#define ADDR_MAIN_RAM_END    0xDFFF

#define ADDR_ECHO_RAM_BEGIN  0xE000
#define ADDR_ECHO_RAM_END    0xFDFF

#define ADDR_OAM_BEGIN       0xFE00
#define ADDR_OAM_END         0xFE9F

#define ADDR_IOBUS_BEGIN     0xFF00
#define ADDR_IOBUS_END       0xFFFF

//----------

#define ADDR_TILE0       0x8000
#define ADDR_TILE1       0x8800
#define ADDR_MAP0        0x9800
#define ADDR_MAP1        0x9C00

//----------

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

#define ADDR_NR10        0xFF10
#define ADDR_NR11        0xFF11
#define ADDR_NR12        0xFF12
#define ADDR_NR13        0xFF13
#define ADDR_NR14        0xFF14

#define ADDR_NR21        0xFF16
#define ADDR_NR22        0xFF17
#define ADDR_NR23        0xFF18
#define ADDR_NR24        0xFF19

#define ADDR_NR30        0xFF1A
#define ADDR_NR31        0xFF1B
#define ADDR_NR32        0xFF1C
#define ADDR_NR33        0xFF1D
#define ADDR_NR34        0xFF1E

#define ADDR_NR41        0xFF20
#define ADDR_NR42        0xFF21
#define ADDR_NR43        0xFF22
#define ADDR_NR44        0xFF23

#define ADDR_NR50        0xFF24
#define ADDR_NR51        0xFF25
#define ADDR_NR52        0xFF26

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

#define ADDR_DISABLE_BOOTROM 0xFF50

#define ADDR_UNUSABLE_BEGIN  0xFF4C
#define ADDR_UNUSABLE_END    0xFF47

#define ADDR_ZEROPAGE_BEGIN  0xFF80
#define ADDR_ZEROPAGE_END    0xFFFE

#define ADDR_IE     0xFFFF

constexpr bool in_range(uint16_t addr, uint16_t begin, uint16_t end) {
  return (begin <= addr) && (addr <= end);
}

//-----------------------------------------------------------------------------
// Flags

#define BIT_VBLANK  0
#define BIT_STAT    1
#define BIT_TIMER   2
#define BIT_SERIAL  3
#define BIT_JOYPAD  4

#define INT_VBLANK_MASK  0b00000001
#define INT_STAT_MASK    0b00000010
#define INT_TIMER_MASK   0b00000100
#define INT_SERIAL_MASK  0b00001000
#define INT_JOYPAD_MASK  0b00010000

#define EI_GLITCH    0b10000000
#define EI_LYC       0b01000000
#define EI_OAM       0b00100000
#define EI_VBLANK    0b00010000
#define EI_HBLANK    0b00001000

#define STAT_LYC     0b00000100

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

static const uint8_t flag_mask[256] = {
     0,    0,    0,    0, 0xE0, 0xE0,    0, 0xF0,
     0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0xF0,
     0,    0,    0,    0, 0xE0, 0xE0,    0, 0xF0,
     0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0xF0,
     0,    0,    0,    0, 0xE0, 0xE0,    0, 0xB0,
     0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0x60,
     0,    0,    0,    0, 0xE0, 0xE0,    0, 0x70,
     0, 0x70,    0,    0, 0xE0, 0xE0,    0, 0x70,

     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,
     0,    0,    0,    0,    0,    0,    0,    0,

  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,

     0,    0,    0,    0,    0,    0, 0xF0,    0,
     0,    0,    0,    0,    0,    0, 0xF0,    0,
     0,    0,    0,    0,    0,    0, 0xF0,    0,
     0,    0,    0,    0,    0,    0, 0xF0,    0,
     0,    0,    0,    0,    0,    0, 0xF0,    0,
  0xF0,    0,    0,    0,    0,    0, 0xF0,    0,
     0, 0xF0,    0,    0,    0,    0, 0xF0,    0,
  0xF0,    0,    0,    0,    0,    0, 0xF0,    0,
};

static const uint8_t cb_flag_mask[4] = { 0xF0, 0xE0,    0, 0x0 };

inline const char* byte_to_bits(uint8_t x) {
  static const char* tab[256] = {
    "0000:0000","0000:0001","0000:0010","0000:0011","0000:0100","0000:0101","0000:0110","0000:0111",
    "0000:1000","0000:1001","0000:1010","0000:1011","0000:1100","0000:1101","0000:1110","0000:1111",
    "0001:0000","0001:0001","0001:0010","0001:0011","0001:0100","0001:0101","0001:0110","0001:0111",
    "0001:1000","0001:1001","0001:1010","0001:1011","0001:1100","0001:1101","0001:1110","0001:1111",
    "0010:0000","0010:0001","0010:0010","0010:0011","0010:0100","0010:0101","0010:0110","0010:0111",
    "0010:1000","0010:1001","0010:1010","0010:1011","0010:1100","0010:1101","0010:1110","0010:1111",
    "0011:0000","0011:0001","0011:0010","0011:0011","0011:0100","0011:0101","0011:0110","0011:0111",
    "0011:1000","0011:1001","0011:1010","0011:1011","0011:1100","0011:1101","0011:1110","0011:1111",
    "0100:0000","0100:0001","0100:0010","0100:0011","0100:0100","0100:0101","0100:0110","0100:0111",
    "0100:1000","0100:1001","0100:1010","0100:1011","0100:1100","0100:1101","0100:1110","0100:1111",
    "0101:0000","0101:0001","0101:0010","0101:0011","0101:0100","0101:0101","0101:0110","0101:0111",
    "0101:1000","0101:1001","0101:1010","0101:1011","0101:1100","0101:1101","0101:1110","0101:1111",
    "0110:0000","0110:0001","0110:0010","0110:0011","0110:0100","0110:0101","0110:0110","0110:0111",
    "0110:1000","0110:1001","0110:1010","0110:1011","0110:1100","0110:1101","0110:1110","0110:1111",
    "0111:0000","0111:0001","0111:0010","0111:0011","0111:0100","0111:0101","0111:0110","0111:0111",
    "0111:1000","0111:1001","0111:1010","0111:1011","0111:1100","0111:1101","0111:1110","0111:1111",
    "1000:0000","1000:0001","1000:0010","1000:0011","1000:0100","1000:0101","1000:0110","1000:0111",
    "1000:1000","1000:1001","1000:1010","1000:1011","1000:1100","1000:1101","1000:1110","1000:1111",
    "1001:0000","1001:0001","1001:0010","1001:0011","1001:0100","1001:0101","1001:0110","1001:0111",
    "1001:1000","1001:1001","1001:1010","1001:1011","1001:1100","1001:1101","1001:1110","1001:1111",
    "1010:0000","1010:0001","1010:0010","1010:0011","1010:0100","1010:0101","1010:0110","1010:0111",
    "1010:1000","1010:1001","1010:1010","1010:1011","1010:1100","1010:1101","1010:1110","1010:1111",
    "1011:0000","1011:0001","1011:0010","1011:0011","1011:0100","1011:0101","1011:0110","1011:0111",
    "1011:1000","1011:1001","1011:1010","1011:1011","1011:1100","1011:1101","1011:1110","1011:1111",
    "1100:0000","1100:0001","1100:0010","1100:0011","1100:0100","1100:0101","1100:0110","1100:0111",
    "1100:1000","1100:1001","1100:1010","1100:1011","1100:1100","1100:1101","1100:1110","1100:1111",
    "1101:0000","1101:0001","1101:0010","1101:0011","1101:0100","1101:0101","1101:0110","1101:0111",
    "1101:1000","1101:1001","1101:1010","1101:1011","1101:1100","1101:1101","1101:1110","1101:1111",
    "1110:0000","1110:0001","1110:0010","1110:0011","1110:0100","1110:0101","1110:0110","1110:0111",
    "1110:1000","1110:1001","1110:1010","1110:1011","1110:1100","1110:1101","1110:1110","1110:1111",
    "1111:0000","1111:0001","1111:0010","1111:0011","1111:0100","1111:0101","1111:0110","1111:0111",
    "1111:1000","1111:1001","1111:1010","1111:1011","1111:1100","1111:1101","1111:1110","1111:1111"
  };
  return tab[x];
}

extern const uint8_t vram_boot[8192];
extern const uint8_t framebuffer_boot[160*144];

#define MB_DELTA_AB   ((phase_total & 7) == 0)
#define MB_DELTA_BC   ((phase_total & 7) == 1)
#define MB_DELTA_CD   ((phase_total & 7) == 2)
#define MB_DELTA_DE   ((phase_total & 7) == 3)
#define MB_DELTA_EF   ((phase_total & 7) == 4)
#define MB_DELTA_FG   ((phase_total & 7) == 5)
#define MB_DELTA_GH   ((phase_total & 7) == 6)
#define MB_DELTA_HA   ((phase_total & 7) == 7)
