#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct GateBoyVramBus {

  void reset_to_cart() {
    BUS_VRAM_A00n.reset(1);
    BUS_VRAM_A01n.reset(1);
    BUS_VRAM_A02n.reset(1);
    BUS_VRAM_A03n.reset(1);
    BUS_VRAM_A04n.reset(0);
    BUS_VRAM_A05n.reset(1);
    BUS_VRAM_A06n.reset(0);
    BUS_VRAM_A07n.reset(1);
    BUS_VRAM_A08n.reset(1);
    BUS_VRAM_A09n.reset(1);
    BUS_VRAM_A10n.reset(1);
    BUS_VRAM_A11n.reset(1);
    BUS_VRAM_A12n.reset(1);

    PIN_34_VRAM_A00.reset(1);
    PIN_35_VRAM_A01.reset(1);
    PIN_36_VRAM_A02.reset(1);
    PIN_37_VRAM_A03.reset(1);
    PIN_38_VRAM_A04.reset(0);
    PIN_39_VRAM_A05.reset(1);
    PIN_40_VRAM_A06.reset(0);
    PIN_41_VRAM_A07.reset(1);
    PIN_48_VRAM_A08.reset(1);
    PIN_47_VRAM_A09.reset(1);
    PIN_44_VRAM_A10.reset(1);
    PIN_46_VRAM_A11.reset(1);
    PIN_42_VRAM_A12.reset(1);
  }

  void reset_buses() {
    BUS_VRAM_A00n.reset_for_pass();
    BUS_VRAM_A01n.reset_for_pass();
    BUS_VRAM_A02n.reset_for_pass();
    BUS_VRAM_A03n.reset_for_pass();
    BUS_VRAM_A04n.reset_for_pass();
    BUS_VRAM_A05n.reset_for_pass();
    BUS_VRAM_A06n.reset_for_pass();
    BUS_VRAM_A07n.reset_for_pass();
    BUS_VRAM_A08n.reset_for_pass();
    BUS_VRAM_A09n.reset_for_pass();
    BUS_VRAM_A10n.reset_for_pass();
    BUS_VRAM_A11n.reset_for_pass();
    BUS_VRAM_A12n.reset_for_pass();

    BUS_VRAM_D00p.reset_for_pass();
    BUS_VRAM_D01p.reset_for_pass();
    BUS_VRAM_D02p.reset_for_pass();
    BUS_VRAM_D03p.reset_for_pass();
    BUS_VRAM_D04p.reset_for_pass();
    BUS_VRAM_D05p.reset_for_pass();
    BUS_VRAM_D06p.reset_for_pass();
    BUS_VRAM_D07p.reset_for_pass();

    PIN_43_VRAM_CSn.reset_for_pass();
    PIN_45_VRAM_OEn.reset_for_pass();
    PIN_49_VRAM_WRn.reset_for_pass();

    PIN_34_VRAM_A00.reset_for_pass();
    PIN_35_VRAM_A01.reset_for_pass();
    PIN_36_VRAM_A02.reset_for_pass();
    PIN_37_VRAM_A03.reset_for_pass();
    PIN_38_VRAM_A04.reset_for_pass();
    PIN_39_VRAM_A05.reset_for_pass();
    PIN_40_VRAM_A06.reset_for_pass();
    PIN_41_VRAM_A07.reset_for_pass();
    PIN_48_VRAM_A08.reset_for_pass();
    PIN_47_VRAM_A09.reset_for_pass();
    PIN_44_VRAM_A10.reset_for_pass();
    PIN_46_VRAM_A11.reset_for_pass();
    PIN_42_VRAM_A12.reset_for_pass();

    PIN_33_VRAM_D00.reset_for_pass();
    PIN_31_VRAM_D01.reset_for_pass();
    PIN_30_VRAM_D02.reset_for_pass();
    PIN_29_VRAM_D03.reset_for_pass();
    PIN_28_VRAM_D04.reset_for_pass();
    PIN_27_VRAM_D05.reset_for_pass();
    PIN_26_VRAM_D06.reset_for_pass();
    PIN_25_VRAM_D07.reset_for_pass();
  }

  void cpu_addr_to_vram_addr(const GateBoyCpuBus& cpu_bus, NorLatch XYMU_RENDERINGn, wire LUFA_DMA_VRAMp);
  void dma_addr_to_vram_addr(const GateBoyDMA& dma);
  void scroll_to_addr(BGScrollX scroll_x, BGScrollY scroll_y, wire POTU_BGW_MAP_READp, wire AXAD_WIN_MODEn, DFF9 XAFO_LCDC_BGMAPn);
  void win_to_addr(const WinMapX& win_map_x, const WinLineY& win_line_y, wire POTU_BGW_MAP_READp, wire PORE_WIN_MODEp, DFF9 WOKY_LCDC_WINMAPn);
  void tile_to_addr(const BGScrollY scroll_y, const WinLineY win_line_y, const TileTempB tile_temp_b, wire NETA_BGW_TILE_READp, wire XUHA_FETCH_HILOp, DFF9 WEXU_LCDC_BGTILEn, wire PORE_WIN_MODEp, wire AXAD_WIN_MODEn);
  void sprite_to_addr(const SpriteStore& sprite_store, const OamTempA& oam_temp_a, const OamTempB& oam_temp_b, wire XUQU_SPRITE_AB, wire SAKY_SFETCHn, NorLatch XYMU_RENDERINGp, DFF9 XYMO_LCDC_SPSIZEn);
  void addr_to_pins();
  void cpu_data_to_bus_data(const GateBoyCpuBus& cpu_bus, wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);
  void bus_data_to_pins(wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);

  void set_pin_cs(wire TUTO_VRAM_DBGp, wire SERE_CPU_VRAM_RDp, wire LUFA_DMA_VRAMp, wire LENA_BFETCHINGp, wire TEXY_SFETCHINGp);
  void set_pin_wr(wire TUTO_VRAM_DBGp, wire SERE_CPU_VRAM_RDp, wire TUJA_CPU_VRAM_WRp);
  void set_pin_oe(wire TUTO_VRAM_DBGp, wire SALE_CPU_VRAM_WRn, wire LUFA_DMA_VRAMp, NorLatch XYMU_RENDERINGn, NandLatch LONY_BFETCHINGp, wire SOHO_SPR_VRAM_RDp);

  void read_vram(const uint8_t* vid_ram);
  void write_vram(uint8_t* vid_ram);

  void pins_to_data_bus(wire SERE_CPU_VRAM_RDp, wire SALE_CPU_VRAM_WRn);
  void data_bus_to_cpu_bus(GateBoyCpuBus& cpu_bus, wire SERE_CPU_VRAM_RDp);

  void dump(Dumper& d) {
    d.dump_bitp   ("PIN_43_VRAM_CSn  : ", PIN_43_VRAM_CSn.state);
    d.dump_bitp   ("PIN_45_VRAM_OEn  : ", PIN_45_VRAM_OEn.state);
    d.dump_bitp   ("PIN_49_VRAM_WRn  : ", PIN_49_VRAM_WRn.state);
    d.dump_slice2p("PIN_34_VRAM_ADDR : ", &PIN_34_VRAM_A00, 13);
    d.dump_slice2p("PIN_25_VRAM_DATA : ", &PIN_33_VRAM_D00, 8);
    d.dump_slice2n("BUS_VRAM_An     : ", &BUS_VRAM_A00n, 13);
    d.dump_slice2p("BUS_VRAM_Dp     : ", &BUS_VRAM_D00p, 8);
  }

  /*BUS_VRAM_A00n*/ Bus BUS_VRAM_A00n;
  /*BUS_VRAM_A01n*/ Bus BUS_VRAM_A01n;
  /*BUS_VRAM_A02n*/ Bus BUS_VRAM_A02n;
  /*BUS_VRAM_A03n*/ Bus BUS_VRAM_A03n;
  /*BUS_VRAM_A04n*/ Bus BUS_VRAM_A04n;
  /*BUS_VRAM_A05n*/ Bus BUS_VRAM_A05n;
  /*BUS_VRAM_A06n*/ Bus BUS_VRAM_A06n;
  /*BUS_VRAM_A07n*/ Bus BUS_VRAM_A07n;
  /*BUS_VRAM_A08n*/ Bus BUS_VRAM_A08n;
  /*BUS_VRAM_A09n*/ Bus BUS_VRAM_A09n;
  /*BUS_VRAM_A10n*/ Bus BUS_VRAM_A10n;
  /*BUS_VRAM_A11n*/ Bus BUS_VRAM_A11n;
  /*BUS_VRAM_A12n*/ Bus BUS_VRAM_A12n;

  /*BUS_VRAM_D00p*/ Bus BUS_VRAM_D00p;
  /*BUS_VRAM_D01p*/ Bus BUS_VRAM_D01p;
  /*BUS_VRAM_D02p*/ Bus BUS_VRAM_D02p;
  /*BUS_VRAM_D03p*/ Bus BUS_VRAM_D03p;
  /*BUS_VRAM_D04p*/ Bus BUS_VRAM_D04p;
  /*BUS_VRAM_D05p*/ Bus BUS_VRAM_D05p;
  /*BUS_VRAM_D06p*/ Bus BUS_VRAM_D06p;
  /*BUS_VRAM_D07p*/ Bus BUS_VRAM_D07p;

  /*PIN_43*/ PinOut PIN_43_VRAM_CSn;
  /*PIN_45*/ PinOut PIN_45_VRAM_OEn;
  /*PIN_49*/ PinOut PIN_49_VRAM_WRn;

  /*PIN_34*/ PinOut PIN_34_VRAM_A00;
  /*PIN_35*/ PinOut PIN_35_VRAM_A01;
  /*PIN_36*/ PinOut PIN_36_VRAM_A02;
  /*PIN_37*/ PinOut PIN_37_VRAM_A03;
  /*PIN_38*/ PinOut PIN_38_VRAM_A04;
  /*PIN_39*/ PinOut PIN_39_VRAM_A05;
  /*PIN_40*/ PinOut PIN_40_VRAM_A06;
  /*PIN_41*/ PinOut PIN_41_VRAM_A07;
  /*PIN_48*/ PinOut PIN_48_VRAM_A08;
  /*PIN_47*/ PinOut PIN_47_VRAM_A09;
  /*PIN_44*/ PinOut PIN_44_VRAM_A10;
  /*PIN_46*/ PinOut PIN_46_VRAM_A11;
  /*PIN_42*/ PinOut PIN_42_VRAM_A12;

  /*PIN_33*/ PinIO  PIN_33_VRAM_D00;
  /*PIN_31*/ PinIO  PIN_31_VRAM_D01;
  /*PIN_30*/ PinIO  PIN_30_VRAM_D02;
  /*PIN_29*/ PinIO  PIN_29_VRAM_D03;
  /*PIN_28*/ PinIO  PIN_28_VRAM_D04;
  /*PIN_27*/ PinIO  PIN_27_VRAM_D05;
  /*PIN_26*/ PinIO  PIN_26_VRAM_D06;
  /*PIN_25*/ PinIO  PIN_25_VRAM_D07;
};

//-----------------------------------------------------------------------------
