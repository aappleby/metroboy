#pragma once

struct Dumper;
struct GateBoySys;
struct GateBoyState;
struct GateBoyPins;

class GateBoyDumper {
public:
  void dump_pins(const GateBoyPins& p, Dumper& d);
  void dump_sys(const GateBoySys& s, Dumper& d);
  void dump_cpu_bus(const GateBoyState& s, Dumper& d);
  void dump_ext_bus(const GateBoyState& s, Dumper& d);
  void dump_vram_bus(const GateBoyState& s, Dumper& d);
  void dump_oam_bus(const GateBoyState& s, Dumper& d);
  void dump_sprite_scanner(const GateBoyState& s, Dumper& d);
  void dump_tile_fetcher(const GateBoyState& s, Dumper& d);
  void dump_sprite_fetcher(const GateBoyState& s, Dumper& d);
  void dump_resets(const GateBoyState& s, Dumper& d);
  void dump_timer(const GateBoyState& s, Dumper& d);
  void dump_clocks(const GateBoyState& s, Dumper& d);
  void dump_interrupts(const GateBoyState& s, Dumper& d);
  void dump_joypad(const GateBoyState& s, Dumper& d);
  void dump_lcd(const GateBoyState& s, Dumper& d);
  void dump_sprite_store(const GateBoyState& s, Dumper& d);
  void dump_mbc1(const GateBoyState& s, Dumper& d);
  void dump_dma(const GateBoyState& s, Dumper& d);
  void dump_serial(const GateBoyState& s, Dumper& d);
  void dump_ppu(const GateBoyState& s, Dumper& d);
  void dump_spu(const GateBoyState& s, Dumper& d);
};
