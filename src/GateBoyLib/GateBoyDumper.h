#pragma once
#include "CoreLib/Types.h"

struct GateBoy;

class GateBoyDumper {
public:
  void dump_sys(const GateBoy& gb, Dumper& d);
  void dump_cpu_bus(const GateBoy& gb, Dumper& d);
  void dump_ext_bus(const GateBoy& gb, Dumper& d);
  void dump_vram_bus(const GateBoy& gb, Dumper& d);
  void dump_oam_bus(const GateBoy& gb, Dumper& d);
  void dump_sprite_scanner(const GateBoy& gb, Dumper& d);
  void dump_tile_fetcher(const GateBoy& gb, Dumper& d);
  void dump_sprite_fetcher(const GateBoy& gb, Dumper& d);
  void dump_resets(const GateBoy& gb, Dumper& d);
  void dump_timer(const GateBoy& gb, Dumper& d);
  void dump_clocks(const GateBoy& gb, Dumper& d);
  void dump_interrupts(const GateBoy& gb, Dumper& d);
  void dump_joypad(const GateBoy& gb, Dumper& d);
  void dump_lcd(const GateBoy& gb, Dumper& d);
  void dump_sprite_store(const GateBoy& gb, Dumper& d);
  void dump_mbc1(const GateBoy& gb, Dumper& d);
  void dump_dma(const GateBoy& gb, Dumper& d);
  void dump_serial(const GateBoy& gb, Dumper& d);
  void dump_ppu(const GateBoy& gb, Dumper& d);
  void dump_spu(const GateBoy& gb, Dumper& d);
};