#include "Sch_CPU.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

CpuSignals CpuRegisters::sig(const TestGB& gb) const {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_pins);

  auto& cpu_pins = gb.cpu_pins;

  // UJYV01 << UNOR04
  // UJYV02 << (mux 1) RD_C
  // UJYV03 << (mux 0) CPU_RAW_RD
  // UJYV04 nc
  // UJYV05 >> TEDO01

  // UBAL1 << UNOR04
  // UBAL2 << (mux1) << WR_C
  // UBAL3 << (mux0) << APOV04
  // UBAL4 nc
  // UBAL5 >>

  /*p07.UJYV*/ wire UJYV_BUS_RD_MUX = mux2_n(gb.ext_pins.RD_C, gb.cpu_pins.CPU_RAW_RD, dbg_sig.UNOR_MODE_DBG2p);
  /*p07.TEDO*/ wire TEDO_CPU_RD = not(UJYV_BUS_RD_MUX);
  /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
  /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(gb.cpu_pins.CPU_RAW_WR, clk_sig.AFAS_xxxxxFGH);
  /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);
  /*p07.UBAL*/ wire UBAL_CPU_WR_ABCDExxx = mux2_n(gb.ext_pins.WR_C, APOV_CPU_WR_xxxxxFGH, dbg_sig.UNOR_MODE_DBG2p);
  /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(UBAL_CPU_WR_ABCDExxx);
  /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
  /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);

  // i do not trust this block, seems weird.

  // LAGU 5-rung
  // LAGU01

  /*p08.LAGU*/ wire _LAGU = or(and(cpu_pins.CPU_RAW_RD, adr_sig.LEVO_8000_9FFFp), cpu_pins.CPU_RAW_WR);
  /*p08.LYWE*/ wire _LYWE = not(_LAGU);
  /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor(adr_sig.TEXO_8000_9FFFn, dbg_sig.UMUT_MODE_DBG1);
  /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or(_MOCA_DBG_EXT_RD, _LYWE);

  /*p04.DECY*/ wire _DECY_FROM_CPU5n = not(cpu_pins.FROM_CPU5);

  return {
    .TEDO_CPU_RD = TEDO_CPU_RD,
    .ASOT_CPU_RD = ASOT_CPU_RD,
    .MOCA_DBG_EXT_RD = _MOCA_DBG_EXT_RD,
    .MOTY_CPU_EXT_RD = _MOTY_CPU_EXT_RD,
    .TAPU_CPU_WR_xxxxxFGH = TAPU_CPU_WR_xxxxxFGH,
    .CUPA_CPU_WR_xxxxxFGH = CUPA_CPU_WR_xxxxxFGH,
    .APOV_CPU_WR_xxxxxFGH = APOV_CPU_WR_xxxxxFGH,
    .DECY_FROM_CPU5n = _DECY_FROM_CPU5n,
  };
}

//-----------------------------------------------------------------------------

void CpuRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_pins);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto tim_sig = gb.tim_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  auto& ppu_reg = gb.ppu_reg;
  auto& bus_reg = gb.bus_reg;
  auto& cpu_pins = gb.cpu_pins;
  auto& tim_reg = gb.tim_reg;
  auto& int_reg = gb.int_reg;
  auto& cfg_reg = gb.cfg_reg;
  auto& lcd_reg = gb.lcd_reg;
  auto& joy_pin = gb.joy_pin;
  auto& joy_reg = gb.joy_reg;
  auto& ser_reg = gb.ser_reg;

  // FF0F INT
  {
    /*p07.SEMY*/ wire ADDR_XX0X = nor(cpu_pins.A07, cpu_pins.A06, cpu_pins.A05, cpu_pins.A04);
    /*p07.SAPA*/ wire ADDR_XXXF = and(cpu_pins.A00, cpu_pins.A01, cpu_pins.A02, cpu_pins.A03);
    /*p07.ROLO*/ wire FF0F_RDn = nand(ADDR_XX0X, ADDR_XXXF, adr_sig.SYKE_FF00_FFFFp, cpu_sig.TEDO_CPU_RD); // schematic wrong, is NAND
    /*p02.POLA*/ wire FF0F_RDa = not(FF0F_RDn);

    // pass gates? does look like a transparent latch here...
    /*p02.MATY*/ int_reg.FF0F_L0.tp_latch(FF0F_RDn, int_reg.FF0F_0);
    /*p02.NEJY*/ int_reg.FF0F_L1.tp_latch(FF0F_RDn, int_reg.FF0F_1);
    /*p02.NUTY*/ int_reg.FF0F_L2.tp_latch(FF0F_RDn, int_reg.FF0F_2);
    /*p02.MOPO*/ int_reg.FF0F_L3.tp_latch(FF0F_RDn, int_reg.FF0F_3);
    /*p02.PAVY*/ int_reg.FF0F_L4.tp_latch(FF0F_RDn, int_reg.FF0F_4);

    /*p02.NELA*/ cpu_pins.D0.set_tribuf(FF0F_RDa, int_reg.FF0F_L0); // small tri
    /*p02.NABO*/ cpu_pins.D1.set_tribuf(FF0F_RDa, int_reg.FF0F_L1); // small tri
    /*p02.ROVA*/ cpu_pins.D2.set_tribuf(FF0F_RDa, int_reg.FF0F_L2);
    /*p02.PADO*/ cpu_pins.D3.set_tribuf(FF0F_RDa, int_reg.FF0F_L3);
    /*p02.PEGY*/ cpu_pins.D4.set_tribuf(FF0F_RDa, int_reg.FF0F_L4);
  }

  // FF40 LCDC
  {
    /*p22.WORU*/ wire FF40n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.VOCA*/ wire FF40 = not(FF40n);
    /*p23.VYRE*/ wire FF40_RD = and(FF40, cpu_sig.ASOT_CPU_RD);
    /*p23.WYCE*/ wire FF40_RDn = not(FF40_RD);

    /*p23.WYPO*/ cpu_pins.D0.set_tribuf(!FF40_RDn, cfg_reg.LCDC_BGEN.q());
    /*p23.XERO*/ cpu_pins.D1.set_tribuf(!FF40_RDn, cfg_reg.LCDC_SPEN.q());
    /*p23.WYJU*/ cpu_pins.D2.set_tribuf(!FF40_RDn, cfg_reg.LCDC_SPSIZE.q());
    /*p23.WUKA*/ cpu_pins.D3.set_tribuf(!FF40_RDn, cfg_reg.LCDC_BGMAP.q());
    /*p23.VOKE*/ cpu_pins.D4.set_tribuf(!FF40_RDn, cfg_reg.LCDC_BGTILE.q());
    /*p23.VATO*/ cpu_pins.D5.set_tribuf(!FF40_RDn, cfg_reg.LCDC_WINEN.q());
    /*p23.VAHA*/ cpu_pins.D6.set_tribuf(!FF40_RDn, cfg_reg.LCDC_WINMAP.q());
    /*p23.XEBU*/ cpu_pins.D7.set_tribuf(!FF40_RDn, cfg_reg.XONA_LCDC_EN.q());

    /*p23.WARU*/ wire _WARU_FF40_WR = and (FF40, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p23.XUBO*/ wire _XUBO_FF40_WRn = not(_WARU_FF40_WR);

    /*p01.XARE*/ wire _XARE_RESET = not(rst_sig.XORE_RSTp);
    /*p23.VYXE*/ cfg_reg.LCDC_BGEN.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D0);
    /*p23.XYLO*/ cfg_reg.LCDC_SPEN.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D1);
    /*p23.XYMO*/ cfg_reg.LCDC_SPSIZE.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D2);
    /*p23.XAFO*/ cfg_reg.LCDC_BGMAP.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D3);
    /*p23.WEXU*/ cfg_reg.LCDC_BGTILE.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D4);
    /*p23.WYMO*/ cfg_reg.LCDC_WINEN.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D5);
    /*p23.WOKY*/ cfg_reg.LCDC_WINMAP.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D6);
    /*p23.XONA*/ cfg_reg.XONA_LCDC_EN.set(_XUBO_FF40_WRn, _XARE_RESET, cpu_pins.D7);
  }


  // FF41
  {
    /*p22.WOFA*/ wire FF41n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.VARY*/ wire FF41 = not(FF41n);
    /*p21.TOBE*/ wire FF41_RDa = and(cpu_sig.ASOT_CPU_RD, FF41); // die AND
    /*p21.VAVE*/ wire FF41_RDb = not(FF41_RDa); // die INV

  // stat read
  // hblank  = stat 0 = 00
  // vblank  = stat 1 = 01
  // oamscan = stat 2 = 10
  // render  = stat 3 = 11

    /*p21.PARU*/ wire PARU_IN_VBLANK = not(!lcd_reg.POPU_IN_VBLANK_d4);
    /*p21.XATY*/ wire XATY_STAT_MODE1n = nor(ppu_reg.XYMU_RENDERINGp, ppu_sig.ACYL_PPU_USE_OAM1p); // die NOR
    /*p21.SADU*/ wire SADU_STAT_MODE0n = nor(ppu_reg.XYMU_RENDERINGp, PARU_IN_VBLANK); // die NOR

    // OK, these tribufs are _slightly_ different - compare SEGO and SASY, second rung.
    /*p21.TEBY*/ cpu_pins.D0.set_tribuf(FF41_RDa, not(SADU_STAT_MODE0n));
    /*p21.WUGA*/ cpu_pins.D1.set_tribuf(FF41_RDa, not(XATY_STAT_MODE1n));
    /*p21.SEGO*/ cpu_pins.D2.set_tribuf(FF41_RDa, not(lcd_reg.LYC_MATCH_LATCHn.q()));
    /*p21.PUZO*/ cpu_pins.D3.set_tribuf(not(FF41_RDb), ppu_reg.ROXE_INT_HBL_EN.q());
    /*p21.POFO*/ cpu_pins.D4.set_tribuf(not(FF41_RDb), ppu_reg.RUFO_INT_VBL_EN.q());
    /*p21.SASY*/ cpu_pins.D5.set_tribuf(not(FF41_RDb), ppu_reg.REFE_INT_OAM_EN.q());
    /*p21.POTE*/ cpu_pins.D6.set_tribuf(not(FF41_RDb), ppu_reg.RUGU_INT_LYC_EN.q());
  }

  // FF42 SCY
  {
    /*p22.WEBU*/ wire FF42n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.XARO*/ wire FF42 = not(FF42n);
    /*p23.ANYP*/ wire FF42_RD = and(FF42, cpu_sig.ASOT_CPU_RD);
    /*p23.BUWY*/ wire FF42_RDn = not(FF42_RD);

    /*p23.WARE*/ cpu_pins.D0.set_tribuf(!FF42_RDn, cfg_reg.SCY0.q());
    /*p23.GOBA*/ cpu_pins.D1.set_tribuf(!FF42_RDn, cfg_reg.SCY1.q());
    /*p23.GONU*/ cpu_pins.D2.set_tribuf(!FF42_RDn, cfg_reg.SCY2.q());
    /*p23.GODO*/ cpu_pins.D3.set_tribuf(!FF42_RDn, cfg_reg.SCY3.q());
    /*p23.CUSA*/ cpu_pins.D4.set_tribuf(!FF42_RDn, cfg_reg.SCY4.q());
    /*p23.GYZO*/ cpu_pins.D5.set_tribuf(!FF42_RDn, cfg_reg.SCY5.q());
    /*p23.GUNE*/ cpu_pins.D6.set_tribuf(!FF42_RDn, cfg_reg.SCY6.q());
    /*p23.GYZA*/ cpu_pins.D7.set_tribuf(!FF42_RDn, cfg_reg.SCY7.q());
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire FF43n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.XAVY*/ wire FF43 = not(FF43n);
    /*p23.AVOG*/ wire FF43_RD = and(FF43, cpu_sig.ASOT_CPU_RD);
    /*p23.BEBA*/ wire FF43_RDn = not(FF43_RD);

    /*p23.EDOS*/ cpu_pins.D0.set_tribuf(!FF43_RDn, cfg_reg.SCX0.q());
    /*p23.EKOB*/ cpu_pins.D1.set_tribuf(!FF43_RDn, cfg_reg.SCX1.q());
    /*p23.CUGA*/ cpu_pins.D2.set_tribuf(!FF43_RDn, cfg_reg.SCX2.q());
    /*p23.WONY*/ cpu_pins.D3.set_tribuf(!FF43_RDn, cfg_reg.SCX3.q());
    /*p23.CEDU*/ cpu_pins.D4.set_tribuf(!FF43_RDn, cfg_reg.SCX4.q());
    /*p23.CATA*/ cpu_pins.D5.set_tribuf(!FF43_RDn, cfg_reg.SCX5.q());
    /*p23.DOXE*/ cpu_pins.D6.set_tribuf(!FF43_RDn, cfg_reg.SCX6.q());
    /*p23.CASY*/ cpu_pins.D7.set_tribuf(!FF43_RDn, cfg_reg.SCX7.q());
  }

  // FF44 LY
  {
    /*p22.WYLE*/ wire FF44n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.XENO_A01n, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.XOGY*/ wire FF44 = not(FF44n);
    /*p23.WAFU*/ wire FF44_RD = and(cpu_sig.ASOT_CPU_RD, FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(lcd_reg.MUWY_Y0.q());
    /*p23.XEPO*/ wire LY1n = not(lcd_reg.MYRO_Y1.q());
    /*p23.MYFA*/ wire LY2n = not(lcd_reg.LEXA_Y2.q());
    /*p23.XUHY*/ wire LY3n = not(lcd_reg.LYDO_Y3.q());
    /*p23.WATA*/ wire LY4n = not(lcd_reg.LOVU_Y4.q());
    /*p23.XAGA*/ wire LY5n = not(lcd_reg.LEMA_Y5.q());
    /*p23.XUCE*/ wire LY6n = not(lcd_reg.MATO_Y6.q());
    /*p23.XOWO*/ wire LY7n = not(lcd_reg.LAFO_Y7.q());

    /*p23.VEGA*/ cpu_pins.D0.set_tribuf(!FF44_RDn, not(LY0n));
    /*p23.WUVA*/ cpu_pins.D1.set_tribuf(!FF44_RDn, not(LY1n));
    /*p23.LYCO*/ cpu_pins.D2.set_tribuf(!FF44_RDn, not(LY2n));
    /*p23.WOJY*/ cpu_pins.D3.set_tribuf(!FF44_RDn, not(LY3n));
    /*p23.VYNE*/ cpu_pins.D4.set_tribuf(!FF44_RDn, not(LY4n));
    /*p23.WAMA*/ cpu_pins.D5.set_tribuf(!FF44_RDn, not(LY5n));
    /*p23.WAVO*/ cpu_pins.D6.set_tribuf(!FF44_RDn, not(LY6n));
    /*p23.WEZE*/ cpu_pins.D7.set_tribuf(!FF44_RDn, not(LY7n));
  }

  // FF45
  {
    /*p22.WETY*/ wire FF45n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.XAYU*/ wire FF45 = not(FF45n);
    /*p23.XYLY*/ wire FF45_RD = and(cpu_sig.ASOT_CPU_RD, FF45);
    /*p23.WEKU*/ wire FF45_RDn = not(FF45_RD);

    /*p23.RETU*/ cpu_pins.D0.set_tribuf(!FF45_RDn, lcd_reg.SYRY_LYC0.q());
    /*p23.VOJO*/ cpu_pins.D1.set_tribuf(!FF45_RDn, lcd_reg.VUCE_LYC1.q());
    /*p23.RAZU*/ cpu_pins.D2.set_tribuf(!FF45_RDn, lcd_reg.SEDY_LYC2.q());
    /*p23.REDY*/ cpu_pins.D3.set_tribuf(!FF45_RDn, lcd_reg.SALO_LYC3.q());
    /*p23.RACE*/ cpu_pins.D4.set_tribuf(!FF45_RDn, lcd_reg.SOTA_LYC4.q());
    /*p23.VAZU*/ cpu_pins.D5.set_tribuf(!FF45_RDn, lcd_reg.VAFA_LYC5.q());
    /*p23.VAFE*/ cpu_pins.D6.set_tribuf(!FF45_RDn, lcd_reg.VEVO_LYC6.q());
    /*p23.PUFY*/ cpu_pins.D7.set_tribuf(!FF45_RDn, lcd_reg.RAHA_LYC7.q());
  }

  // FF47
  {
    /*p22.WYBO*/ wire FF47n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.WESA_A01, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.WERA*/ wire FF47 = not(FF47n);
    /*p36.VUSO*/ wire FF47_RD = and(cpu_sig.ASOT_CPU_RD, FF47);
    /*p36.TEPY*/ wire FF47_RDn = not(FF47_RD);

    /*p36.RARO*/ cpu_pins.D0.set_tribuf(!FF47_RDn, cfg_reg.BGP0.q());
    /*p36.PABA*/ cpu_pins.D1.set_tribuf(!FF47_RDn, cfg_reg.BGP1.q());
    /*p36.REDO*/ cpu_pins.D2.set_tribuf(!FF47_RDn, cfg_reg.BGP2.q());
    /*p36.LOBE*/ cpu_pins.D3.set_tribuf(!FF47_RDn, cfg_reg.BGP3.q());
    /*p36.LACE*/ cpu_pins.D4.set_tribuf(!FF47_RDn, cfg_reg.BGP4.q());
    /*p36.LYKA*/ cpu_pins.D5.set_tribuf(!FF47_RDn, cfg_reg.BGP5.q());
    /*p36.LODY*/ cpu_pins.D6.set_tribuf(!FF47_RDn, cfg_reg.BGP6.q());
    /*p36.LARY*/ cpu_pins.D7.set_tribuf(!FF47_RDn, cfg_reg.BGP7.q());
  }

  // FF48
  {
    /*p22.WETA*/ wire FF48n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.XAYO*/ wire FF48 = not(FF48n);
    /*p36.XUFY*/ wire FF48_RD = and(cpu_sig.ASOT_CPU_RD, FF48);
    /*p36.XOZY*/ wire FF48_RDn = not(FF48_RD);

    /*p36.XARY*/ cpu_pins.D0.set_tribuf(!FF48_RDn, cfg_reg.OBP00.q());
    /*p36.XOKE*/ cpu_pins.D1.set_tribuf(!FF48_RDn, cfg_reg.OBP01.q());
    /*p36.XUNO*/ cpu_pins.D2.set_tribuf(!FF48_RDn, cfg_reg.OBP02.q());
    /*p36.XUBY*/ cpu_pins.D3.set_tribuf(!FF48_RDn, cfg_reg.OBP03.q());
    /*p36.XAJU*/ cpu_pins.D4.set_tribuf(!FF48_RDn, cfg_reg.OBP04.q());
    /*p36.XOBO*/ cpu_pins.D5.set_tribuf(!FF48_RDn, cfg_reg.OBP05.q());
    /*p36.XAXA*/ cpu_pins.D6.set_tribuf(!FF48_RDn, cfg_reg.OBP06.q());
    /*p36.XAWO*/ cpu_pins.D7.set_tribuf(!FF48_RDn, cfg_reg.OBP07.q());
  }

  // FF49
  {
    /*p22.VAMA*/ wire FF49n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.TEGO*/ wire FF49 = not(FF49n);
    /*p36.MUMY*/ wire FF49_RD = and(cpu_sig.ASOT_CPU_RD, FF49);
    /*p36.LOTE*/ wire FF49_RDn = not(FF49_RD); // where does this go?

    /*p36.LAJU*/ cpu_pins.D0.set_tribuf(!FF49_RDn, cfg_reg.OBP10.q());
    /*p36.LEPA*/ cpu_pins.D1.set_tribuf(!FF49_RDn, cfg_reg.OBP11.q());
    /*p36.LODE*/ cpu_pins.D2.set_tribuf(!FF49_RDn, cfg_reg.OBP12.q());
    /*p36.LYZA*/ cpu_pins.D3.set_tribuf(!FF49_RDn, cfg_reg.OBP13.q());
    /*p36.LUKY*/ cpu_pins.D4.set_tribuf(!FF49_RDn, cfg_reg.OBP14.q());
    /*p36.LUGA*/ cpu_pins.D5.set_tribuf(!FF49_RDn, cfg_reg.OBP15.q());
    /*p36.LEBA*/ cpu_pins.D6.set_tribuf(!FF49_RDn, cfg_reg.OBP16.q());
    /*p36.LELU*/ cpu_pins.D7.set_tribuf(!FF49_RDn, cfg_reg.OBP17.q());
  }

  // FF4A
  {
    /*p22.WYVO*/ wire FF4An = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.VYGA*/ wire FF4A = not(FF4An);
    /*p23.WAXU*/ wire FF4A_RD = and(cpu_sig.ASOT_CPU_RD, FF4A);
    /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);

    /*p23.PUNU*/ cpu_pins.D0.set_tribuf(!FF4A_RDn, cfg_reg.WY0.q());
    /*p23.PODA*/ cpu_pins.D1.set_tribuf(!FF4A_RDn, cfg_reg.WY1.q());
    /*p23.PYGU*/ cpu_pins.D2.set_tribuf(!FF4A_RDn, cfg_reg.WY2.q());
    /*p23.LOKA*/ cpu_pins.D3.set_tribuf(!FF4A_RDn, cfg_reg.WY3.q());
    /*p23.MEGA*/ cpu_pins.D4.set_tribuf(!FF4A_RDn, cfg_reg.WY4.q());
    /*p23.PELA*/ cpu_pins.D5.set_tribuf(!FF4A_RDn, cfg_reg.WY5.q());
    /*p23.POLO*/ cpu_pins.D6.set_tribuf(!FF4A_RDn, cfg_reg.WY6.q());
    /*p23.MERA*/ cpu_pins.D7.set_tribuf(!FF4A_RDn, cfg_reg.WY7.q());
  }

  // FF4B
  {
    /*p22.WAGE*/ wire FF4Bn = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.VUMY*/ wire FF4B = not(FF4Bn);
    /*p23.WYZE*/ wire FF4B_RD = and(cpu_sig.ASOT_CPU_RD, FF4B);
    /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);

    /*p23.LOVA*/ cpu_pins.D0.set_tribuf(!FF4B_RDn, cfg_reg.WX0.q());
    /*p23.MUKA*/ cpu_pins.D1.set_tribuf(!FF4B_RDn, cfg_reg.WX1.q());
    /*p23.MOKO*/ cpu_pins.D2.set_tribuf(!FF4B_RDn, cfg_reg.WX2.q());
    /*p23.LOLE*/ cpu_pins.D3.set_tribuf(!FF4B_RDn, cfg_reg.WX3.q());
    /*p23.MELE*/ cpu_pins.D4.set_tribuf(!FF4B_RDn, cfg_reg.WX4.q());
    /*p23.MUFE*/ cpu_pins.D5.set_tribuf(!FF4B_RDn, cfg_reg.WX5.q());
    /*p23.MULY*/ cpu_pins.D6.set_tribuf(!FF4B_RDn, cfg_reg.WX6.q());
    /*p23.MARA*/ cpu_pins.D7.set_tribuf(!FF4B_RDn, cfg_reg.WX7.q());
  }

  // FF50
  {
    /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(cpu_pins.A07, cpu_pins.A05, cpu_pins.A03, cpu_pins.A02, cpu_pins.A01, cpu_pins.A00);
    /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(cpu_pins.A04, cpu_pins.A06);
    /*p07.TEXE*/ wire FF50_RD = and(cpu_sig.TEDO_CPU_RD, adr_sig.SYKE_FF00_FFFFp, ADDR_0x0x0000, ADDR_x1x1xxxx);
    /*p07.SYPU*/ cpu_pins.D0.set_tribuf(FF50_RD, bus_reg.BOOT_BITn.q()); // does the rung of the tribuf control polarity?
  }

#if 0
  // Debug stuff I disabled

  /*p07.APET*/ wire APET_MODE_DBG = or(dbg_sig.UMUT_MODE_DBG1, UNOR_MODE_DBG2p); // suggests UMUTp
  /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, cpu_pins.A05, cpu_pins.A06, TAPU_CPUWR, ADDR_111111110xx00000);

  //----------
  // weird debug things, probably not right

  /*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk_sig.BEDO_xBxxxxxx);
  /*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
  /*p05.APYS*/ wire APYS_xBCDEFGH = nor(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
  /*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
  /*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk_sig.BEDO_xBxxxxxx, sys_sig.MODE_DBG2);

  if (AFOP_Axxxxxxx) cpu_pins.set_data(
    /*p05.ANOC*/ not(sys_sig.PIN_P10_B),
    /*p05.ATAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.AJEC*/ not(sys_sig.PIN_P10_B),
    /*p05.ASUZ*/ not(sys_sig.PIN_P10_B),
    /*p05.BENU*/ not(sys_sig.PIN_P10_B),
    /*p05.AKAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.ARAR*/ not(sys_sig.PIN_P10_B),
    /*p05.BEDA*/ not(sys_sig.PIN_P10_B)
  );

  if (LECO_xBCDEFGH) cpu_pins.set_data(
    /*p07.ROMY*/ sys_sig.PIN_P10_B,
    /*p07.RYNE*/ sys_sig.PIN_P10_B,
    /*p07.REJY*/ sys_sig.PIN_P10_B,
    /*p07.RASE*/ sys_sig.PIN_P10_B,
    /*p07.REKA*/ sys_sig.PIN_P10_B,
    /*p07.ROWE*/ sys_sig.PIN_P10_B,
    /*p07.RYKE*/ sys_sig.PIN_P10_B,
    /*p07.RARU*/ sys_sig.PIN_P10_B
  );



  //----------
  // more debug stuff

  /*p25.TUSO*/ wire TUSO = nor(MODE_DBG2, clk.BOGA_AxCDEFGH);
  /*p25.SOLE*/ wire SOLE = not(TUSO);

  if (dbg_sig.VYPO_P10_Bn) bus_out.set_data(
    /*p25.TOVU*/ SOLE,
    /*p25.SOSA*/ SOLE,
    /*p25.SEDU*/ SOLE,
    /*p25.TAXO*/ SOLE,
    /*p25.TAHY*/ SOLE,
    /*p25.TESU*/ SOLE,
    /*p25.TAZU*/ SOLE,
    /*p25.TEWA*/ SOLE
  );

}

// FIXME
  ///*p05.KORE*/ wire P05_NC0 = nand(DBG_FF00_D7, FF60_0);
  ///*p05.KYWE*/ wire P05_NC1 = nor (DBG_FF00_D7, FF60_0o);

  /*p08.LYRA*/ wire DBG_D_RDn = nand(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
  /*p08.TUTY*/ if (!DBG_D_RDn) cpu_pins.D0 = not(/*p08.TOVO*/ not(pins.D0_C));
  /*p08.SYWA*/ if (!DBG_D_RDn) cpu_pins.D1 = not(/*p08.RUZY*/ not(pins.D1_C));
  /*p08.SUGU*/ if (!DBG_D_RDn) cpu_pins.D2 = not(/*p08.ROME*/ not(pins.D2_C));
  /*p08.TAWO*/ if (!DBG_D_RDn) cpu_pins.D3 = not(/*p08.SAZA*/ not(pins.D3_C));
  /*p08.TUTE*/ if (!DBG_D_RDn) cpu_pins.D4 = not(/*p08.TEHE*/ not(pins.D4_C));
  /*p08.SAJO*/ if (!DBG_D_RDn) cpu_pins.D5 = not(/*p08.RATU*/ not(pins.D5_C));
  /*p08.TEMY*/ if (!DBG_D_RDn) cpu_pins.D6 = not(/*p08.SOCA*/ not(pins.D6_C));
  /*p08.ROPA*/ if (!DBG_D_RDn) cpu_pins.D7 = not(/*p08.RYBA*/ not(pins.D7_C));
#endif

  /*p01.WESY*/ wire WESY_RSTn = not(rst_sig.XORE_RSTp);
  /*p01.WALU*/ wire WALU_RESET = not(rst_sig.XORE_RSTp);

  // FF41
  {
    /*p22.WOFA*/ wire FF41n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.VARY*/ wire FF41 = not(FF41n);
    /*p21.SEPA*/ wire FF41_WR = and(cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF41);
    /*p21.RYVE*/ wire RYVE_FF41_WRn = not(FF41_WR);

    /*p21.ROXE*/ ppu_reg.ROXE_INT_HBL_EN.set(RYVE_FF41_WRn, WESY_RSTn, cpu_pins.D0);
    /*p21.RUFO*/ ppu_reg.RUFO_INT_VBL_EN.set(RYVE_FF41_WRn, WESY_RSTn, cpu_pins.D1);
    /*p21.REFE*/ ppu_reg.REFE_INT_OAM_EN.set(RYVE_FF41_WRn, WESY_RSTn, cpu_pins.D2);
    /*p21.RUGU*/ ppu_reg.RUGU_INT_LYC_EN.set(RYVE_FF41_WRn, WESY_RSTn, cpu_pins.D3);
  }

  // FF42 SCY
  {
    /*p22.WEBU*/ wire FF42n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.XARO*/ wire FF42 = not(FF42n);
    /*p23.BEDY*/ wire FF42_WR_ABCDxxxH = and(FF42, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p23.CAVO*/ wire FF42_WRn_xxxxEFGx = not(FF42_WR_ABCDxxxH);

    /*p23.GAVE*/ cfg_reg.SCY0.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D0);
    /*p23.FYMO*/ cfg_reg.SCY1.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D1);
    /*p23.FEZU*/ cfg_reg.SCY2.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D2);
    /*p23.FUJO*/ cfg_reg.SCY3.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D3);
    /*p23.DEDE*/ cfg_reg.SCY4.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D4);
    /*p23.FOTY*/ cfg_reg.SCY5.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D5);
    /*p23.FOHA*/ cfg_reg.SCY6.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D6);
    /*p23.FUNY*/ cfg_reg.SCY7.set(FF42_WRn_xxxxEFGx, rst_sig.CUNU_RSTn, cpu_pins.D7);
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire FF43n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.XERA_A03n);
    /*p22.XAVY*/ wire FF43 = not(FF43n);
    /*p23.ARUR*/ wire FF43_WR = and(FF43, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p23.AMUN*/ wire FF43_WRn = not(FF43_WR);

    /*p23.DATY*/ cfg_reg.SCX0.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D0);
    /*p23.DUZU*/ cfg_reg.SCX1.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D1);
    /*p23.CYXU*/ cfg_reg.SCX2.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D2);
    /*p23.GUBO*/ cfg_reg.SCX3.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D3);
    /*p23.BEMY*/ cfg_reg.SCX4.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D4);
    /*p23.CUZY*/ cfg_reg.SCX5.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D5);
    /*p23.CABU*/ cfg_reg.SCX6.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D6);
    /*p23.BAKE*/ cfg_reg.SCX7.set(FF43_WRn, rst_sig.CUNU_RSTn, cpu_pins.D7);
  }

  // FF45
  {
    /*p22.WETY*/ wire FF45n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.XAYU*/ wire FF45 = not(FF45n);
    /*p23.XUFA*/ wire FF45_WR = and(cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF45);
    /*p23.WANE*/ wire FF45_WRn = not(FF45_WR);

    /*p23.SYRY*/ lcd_reg.SYRY_LYC0.set(FF45_WRn, WESY_RSTn, cpu_pins.D0);
    /*p23.VUCE*/ lcd_reg.VUCE_LYC1.set(FF45_WRn, WESY_RSTn, cpu_pins.D1);
    /*p23.SEDY*/ lcd_reg.SEDY_LYC2.set(FF45_WRn, WESY_RSTn, cpu_pins.D2);
    /*p23.SALO*/ lcd_reg.SALO_LYC3.set(FF45_WRn, WESY_RSTn, cpu_pins.D3);
    /*p23.SOTA*/ lcd_reg.SOTA_LYC4.set(FF45_WRn, WESY_RSTn, cpu_pins.D4);
    /*p23.VAFA*/ lcd_reg.VAFA_LYC5.set(FF45_WRn, WESY_RSTn, cpu_pins.D5);
    /*p23.VEVO*/ lcd_reg.VEVO_LYC6.set(FF45_WRn, WESY_RSTn, cpu_pins.D6);
    /*p23.RAHA*/ lcd_reg.RAHA_LYC7.set(FF45_WRn, WESY_RSTn, cpu_pins.D7);
  }

  // FF47
  {
    /*p22.WYBO*/ wire FF47n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.WESA_A01, adr_sig.WALO_A02, adr_sig.XERA_A03n);
    /*p22.WERA*/ wire FF47 = not(FF47n);
    /*p36.VELY*/ wire FF47_WR = and(cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF47);
    /*p36.TEPO*/ wire FF47_WRn = not(FF47_WR);

    /*p36.PAVO*/ cfg_reg.BGP0.set(FF47_WRn, 1, cpu_pins.D0);
    /*p36.NUSY*/ cfg_reg.BGP1.set(FF47_WRn, 1, cpu_pins.D1);
    /*p36.PYLU*/ cfg_reg.BGP2.set(FF47_WRn, 1, cpu_pins.D2);
    /*p36.MAXY*/ cfg_reg.BGP3.set(FF47_WRn, 1, cpu_pins.D3);
    /*p36.MUKE*/ cfg_reg.BGP4.set(FF47_WRn, 1, cpu_pins.D4);
    /*p36.MORU*/ cfg_reg.BGP5.set(FF47_WRn, 1, cpu_pins.D5);
    /*p36.MOGY*/ cfg_reg.BGP6.set(FF47_WRn, 1, cpu_pins.D6);
    /*p36.MENA*/ cfg_reg.BGP7.set(FF47_WRn, 1, cpu_pins.D7);
  }

  // FF48
  {
    /*p22.WETA*/ wire FF48n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.XAYO*/ wire FF48 = not(FF48n);
    /*p36.XOMA*/ wire FF48_WR = and(cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF48);
    /*p36.XELO*/ wire FF48_WRn = not(FF48_WR);

    /*p36.XUFU*/ cfg_reg.OBP00.set(FF48_WRn, 1, cpu_pins.D0);
    /*p36.XUKY*/ cfg_reg.OBP01.set(FF48_WRn, 1, cpu_pins.D1);
    /*p36.XOVA*/ cfg_reg.OBP02.set(FF48_WRn, 1, cpu_pins.D2);
    /*p36.XALO*/ cfg_reg.OBP03.set(FF48_WRn, 1, cpu_pins.D3);
    /*p36.XERU*/ cfg_reg.OBP04.set(FF48_WRn, 1, cpu_pins.D4);
    /*p36.XYZE*/ cfg_reg.OBP05.set(FF48_WRn, 1, cpu_pins.D5);
    /*p36.XUPO*/ cfg_reg.OBP06.set(FF48_WRn, 1, cpu_pins.D6);
    /*p36.XANA*/ cfg_reg.OBP07.set(FF48_WRn, 1, cpu_pins.D7);
  }

  // FF49
  {
    /*p22.VAMA*/ wire FF49n = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.XENO_A01n, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.TEGO*/ wire FF49 = not(FF49n);
    /*p36.MYXE*/ wire FF49_WR = and(cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF49);
    /*p36.LEHO*/ wire FF49_WRn = not(FF49_WR);

    /*p36.MOXY*/ cfg_reg.OBP10.set(FF49_WRn, 1, cpu_pins.D0);
    /*p36.LAWO*/ cfg_reg.OBP11.set(FF49_WRn, 1, cpu_pins.D1);
    /*p36.MOSA*/ cfg_reg.OBP12.set(FF49_WRn, 1, cpu_pins.D2);
    /*p36.LOSE*/ cfg_reg.OBP13.set(FF49_WRn, 1, cpu_pins.D3);
    /*p36.LUNE*/ cfg_reg.OBP14.set(FF49_WRn, 1, cpu_pins.D4);
    /*p36.LUGU*/ cfg_reg.OBP15.set(FF49_WRn, 1, cpu_pins.D5);
    /*p36.LEPU*/ cfg_reg.OBP16.set(FF49_WRn, 1, cpu_pins.D6);
    /*p36.LUXO*/ cfg_reg.OBP17.set(FF49_WRn, 1, cpu_pins.D7);
  }

  // FF4A
  {
    /*p22.WYVO*/ wire FF4An = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.XOLA_A00n, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.VYGA*/ wire FF4A = not(FF4An);
    /*p23.WEKO*/ wire FF4A_WR = and(cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF4A);
    /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);

    /*p23.NESO*/ cfg_reg.WY0.set(FF4A_WRn, WALU_RESET, cpu_pins.D0);
    /*p23.NYRO*/ cfg_reg.WY1.set(FF4A_WRn, WALU_RESET, cpu_pins.D1);
    /*p23.NAGA*/ cfg_reg.WY2.set(FF4A_WRn, WALU_RESET, cpu_pins.D2);
    /*p23.MELA*/ cfg_reg.WY3.set(FF4A_WRn, WALU_RESET, cpu_pins.D3);
    /*p23.NULO*/ cfg_reg.WY4.set(FF4A_WRn, WALU_RESET, cpu_pins.D4);
    /*p23.NENE*/ cfg_reg.WY5.set(FF4A_WRn, WALU_RESET, cpu_pins.D5);
    /*p23.NUKA*/ cfg_reg.WY6.set(FF4A_WRn, WALU_RESET, cpu_pins.D6);
    /*p23.NAFU*/ cfg_reg.WY7.set(FF4A_WRn, WALU_RESET, cpu_pins.D7);
  }

  // FF4B
  {
    /*p22.WAGE*/ wire FF4Bn = nand(adr_sig.WERO_FF40_FF4Fp, adr_sig.WADO_A00, adr_sig.WESA_A01, adr_sig.XUSY_A02n, adr_sig.WEPO_A03);
    /*p22.VUMY*/ wire FF4B = not(FF4Bn);
    /*p23.WUZA*/ wire FF4B_WR = and(cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF4B);
    /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

    /*p23.MYPA*/ cfg_reg.WX0.set(FF4B_WRn, WALU_RESET, cpu_pins.D0);
    /*p23.NOFE*/ cfg_reg.WX1.set(FF4B_WRn, WALU_RESET, cpu_pins.D1);
    /*p23.NOKE*/ cfg_reg.WX2.set(FF4B_WRn, WALU_RESET, cpu_pins.D2);
    /*p23.MEBY*/ cfg_reg.WX3.set(FF4B_WRn, WALU_RESET, cpu_pins.D3);
    /*p23.MYPU*/ cfg_reg.WX4.set(FF4B_WRn, WALU_RESET, cpu_pins.D4);
    /*p23.MYCE*/ cfg_reg.WX5.set(FF4B_WRn, WALU_RESET, cpu_pins.D5);
    /*p23.MUVO*/ cfg_reg.WX6.set(FF4B_WRn, WALU_RESET, cpu_pins.D6);
    /*p23.NUKU*/ cfg_reg.WX7.set(FF4B_WRn, WALU_RESET, cpu_pins.D7);
  }

  // FF50 BOOT_BIT
  {
    /*p07.TYRO*/ wire ADDR_0x0x0000 = nor(cpu_pins.A07, cpu_pins.A05, cpu_pins.A03, cpu_pins.A02, cpu_pins.A01, cpu_pins.A00);
    /*p07.TUFA*/ wire ADDR_x1x1xxxx = and(cpu_pins.A04, cpu_pins.A06);
    /*p07.TUGE*/ wire FF50_WRn_xxxxxFGH = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.SYKE_FF00_FFFFp, ADDR_0x0x0000, ADDR_x1x1xxxx);
    /*p07.SATO*/ wire BOOT_BIT_IN = or(cpu_pins.D0, bus_reg.BOOT_BITn.q());

    /*p07.TEPU*/ bus_reg.BOOT_BITn.set(FF50_WRn_xxxxxFGH, rst_sig.ALUR_RSTn, BOOT_BIT_IN);
  }

  // hack, not correct
#if 0
  {
    // FF60 debug reg
    /*p07.APET*/ wire APET_MODE_DBG = or(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
    /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, cpu_pins.A05, cpu_pins.A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

    /*p05.KURA*/ wire FF60_0n = not(FF60_0);
    /*p05.JEVA*/ wire FF60_0o = not(FF60_0);
    /*p07.BURO*/ FF60_0.set(FF60_WRn, rst_sig.SYS_RESETn, cpu_pins.D0);
    /*p07.AMUT*/ FF60_1.set(FF60_WRn, rst_sig.SYS_RESETn, cpu_pins.D1);

    ///*p05.KURA*/ wire FF60_0n = not(FF60);
    ///*p05.JEVA*/ wire FF60_0o = not(FF60);
    /*p07.BURO*/ dbg_reg.FF60_0.set(1, rst_sig.ALUR_RSTn, cpu_pins.D0);
    /*p07.AMUT*/ dbg_reg.FF60_1.set(1, rst_sig.ALUR_RSTn, cpu_pins.D1);
  }
#endif

  {
    cpu_pins.SYRO.set(adr_sig.SYRO_FE00_FFFFp);
  }

  {
  }

  /*p07.SEMY*/ wire ADDR_XX0X = nor(cpu_pins.A07, cpu_pins.A06, cpu_pins.A05, cpu_pins.A04);
  /*p07.SAPA*/ wire ADDR_XXXF = and(cpu_pins.A00, cpu_pins.A01, cpu_pins.A02, cpu_pins.A03);

  /*p07.REFA*/ wire FF0F_WRn_xxxxxFGH = nand(ADDR_XX0X, ADDR_XXXF, adr_sig.SYKE_FF00_FFFFp, cpu_sig.TAPU_CPU_WR_xxxxxFGH); // schematic wrong, is NAND
  /*p02.ROTU*/ wire FF0F_WRa_xxxxxFGH = not(FF0F_WRn_xxxxxFGH);

  {
    /*p21.PARU*/ wire PARU_IN_VBLANK = not(!lcd_reg.POPU_IN_VBLANK_d4);
    /*p21.TOLU*/ wire INT_VBLn = not(PARU_IN_VBLANK);
    /*p02.LETY*/ wire INT_VBL_ACK = not(cpu_pins.ACK_VBLANK);
    /*p21.VYPU*/ wire VYPU_INT_VBL = not(INT_VBLn);
    /*p02.MUXE*/ wire MUXE = or(cpu_pins.D0, FF0F_WRn_xxxxxFGH);
    /*p02.MYZU*/ wire FF0F_SET0 = nand(FF0F_WRa_xxxxxFGH, INT_VBL_ACK, cpu_pins.D0);
    /*p02.LYTA*/ wire FF0F_RST0 = and(MUXE, INT_VBL_ACK, rst_sig.ALUR_RSTn);
    /*p02.PESU*/ wire FF0F_IN = not(joy_pin.P10_B);
    /*p02.LOPE*/ int_reg.FF0F_0.set(VYPU_INT_VBL, FF0F_SET0, FF0F_RST0, FF0F_IN);
  }

  {
    /*p02.LEJA*/ wire INT_SER_ACK = not(cpu_pins.ACK_STAT);
    /*p02.NABE*/ wire NABE = or(cpu_pins.D1, FF0F_WRn_xxxxxFGH);
    /*p02.MODY*/ wire FF0F_SET1 = nand(FF0F_WRa_xxxxxFGH, INT_SER_ACK, cpu_pins.D1);
    /*p02.MOVU*/ wire FF0F_RST1 = and(NABE, INT_SER_ACK, rst_sig.ALUR_RSTn);
    /*p02.PESU*/ wire FF0F_IN = not(joy_pin.P10_B);
    /*p02.UBUL*/ int_reg.FF0F_1.set(ser_reg.SER_CNT3, FF0F_SET1, FF0F_RST1, FF0F_IN);
  }

  {
    // this and timer int are swapped?
    /*p02.LESA*/ wire INT_JOY_ACK = not(cpu_pins.ACK_TIMER);
    /*p02.RAKE*/ wire RAKE = or(cpu_pins.D2, FF0F_WRn_xxxxxFGH);
    /*p02.PYHU*/ wire FF0F_SET2 = nand(FF0F_WRa_xxxxxFGH, INT_JOY_ACK, cpu_pins.D2);
    /*p02.PYGA*/ wire FF0F_RST2 = and(RAKE, INT_JOY_ACK, rst_sig.ALUR_RSTn);
    /*p02.ASOK*/ wire INT_JP = and(joy_reg.JP_GLITCH3, joy_reg.JP_GLITCH0);
    /*p02.PESU*/ wire FF0F_IN = not(joy_pin.P10_B);
    /*p02.ULAK*/ int_reg.FF0F_2.set(INT_JP, FF0F_SET2, FF0F_RST2, FF0F_IN);
  }

  {
    // uhhhh probably not ack_serial here either? wtf did i do?
    /*p21.PARU*/ wire PARU_IN_VBLANK = not(!lcd_reg.POPU_IN_VBLANK_d4);
    /*p21.TOLU*/ wire INT_VBLn = not(PARU_IN_VBLANK);
    /*p02.LUFE*/ wire INT_STAT_ACK = not(cpu_pins.ACK_SERIAL);
    /*p02.SULO*/ wire SULO = or(cpu_pins.D3, FF0F_WRn_xxxxxFGH);
    /*p02.TOME*/ wire FF0F_SET3 = nand(FF0F_WRa_xxxxxFGH, INT_STAT_ACK, cpu_pins.D3);
    /*p02.TUNY*/ wire FF0F_RST3 = and(SULO, INT_STAT_ACK, rst_sig.ALUR_RSTn);
    /*p21.TAPA*/ wire TAPA_INT_OAM = and(INT_VBLn, lcd_sig.SELA_NEW_LINE_d0p);
    /*p21.TARU*/ wire TARU_INT_HBL = and(INT_VBLn, ppu_sig.WODU_RENDER_DONE);
    /*p21.SUKO*/ wire INT_STATb = amux4(ppu_reg.RUGU_INT_LYC_EN, lcd_reg.ROPO_LY_MATCH_SYNC,
      ppu_reg.REFE_INT_OAM_EN, TAPA_INT_OAM,
      ppu_reg.RUFO_INT_VBL_EN, PARU_IN_VBLANK, // polarity?
      ppu_reg.ROXE_INT_HBL_EN, TARU_INT_HBL);
    /*p21.TUVA*/ wire TUVA_INT_STATn = not(INT_STATb);
    /*p21.VOTY*/ wire VOTY_INT_STAT = not(TUVA_INT_STATn);
    /*p02.PESU*/ wire FF0F_IN = not(joy_pin.P10_B);
    /*p02.LALU*/ int_reg.FF0F_3.set(VOTY_INT_STAT, FF0F_SET3, FF0F_RST3, FF0F_IN);
  }

  {
    /*p02.LAMO*/ wire INT_TIM_ACK = not(cpu_pins.ACK_JOYPAD);
    /*p02.SEME*/ wire SEME = or(cpu_pins.D4, FF0F_WRn_xxxxxFGH);
    /*p02.TOGA*/ wire FF0F_SET4 = nand(FF0F_WRa_xxxxxFGH, INT_TIM_ACK, cpu_pins.D4);
    /*p02.TYME*/ wire FF0F_RST4 = and(SEME, INT_TIM_ACK, rst_sig.ALUR_RSTn);
    /*p02.PESU*/ wire FF0F_IN = not(joy_pin.P10_B);
    /*p02.NYBO*/ int_reg.FF0F_4.set(tim_reg.INT_TIMER, FF0F_SET4, FF0F_RST4, FF0F_IN);
  }

  cpu_pins.INT_VBLANK.set(int_reg.FF0F_0);
  cpu_pins.INT_SERIAL.set(int_reg.FF0F_1);
  cpu_pins.INT_JOYPAD.set(int_reg.FF0F_2);
  cpu_pins.INT_STAT.set(int_reg.FF0F_3);
  cpu_pins.INT_TIMER.set(int_reg.FF0F_4);
}

//-----------------------------------------------------------------------------

#if 0
  // so the address bus is technically a tribuf, but we're going to ignore
  // this debug circuit for now.
  {
  // If we're in debug mode 2, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  /*p08.KOVA*/ wire A00_Cn = not(ext_pins.A00_C);
  /*p08.CAMU*/ wire A01_Cn = not(ext_pins.A01_C);
  /*p08.BUXU*/ wire A02_Cn = not(ext_pins.A02_C);
  /*p08.BASE*/ wire A03_Cn = not(ext_pins.A03_C);
  /*p08.AFEC*/ wire A04_Cn = not(ext_pins.A04_C);
  /*p08.ABUP*/ wire A05_Cn = not(ext_pins.A05_C);
  /*p08.CYGU*/ wire A06_Cn = not(ext_pins.A06_C);
  /*p08.COGO*/ wire A07_Cn = not(ext_pins.A07_C);
  /*p08.MUJY*/ wire A08_Cn = not(ext_pins.A08_C);
  /*p08.NENA*/ wire A09_Cn = not(ext_pins.A09_C);
  /*p08.SURA*/ wire A10_Cn = not(ext_pins.A10_C);
  /*p08.MADY*/ wire A11_Cn = not(ext_pins.A11_C);
  /*p08.LAHE*/ wire A12_Cn = not(ext_pins.A12_C);
  /*p08.LURA*/ wire A13_Cn = not(ext_pins.A13_C);
  /*p08.PEVO*/ wire A14_Cn = not(ext_pins.A14_C);
  /*p08.RAZA*/ wire A15_Cn = not(ext_pins.A15_C);

  /*p08.KEJO*/ cpu_pins.A00.set(!TOVA_MODE_DBG2n, not(A00_Cn));
  /*p08.BYXE*/ cpu_pins.A01.set(!TOVA_MODE_DBG2n, not(A01_Cn));
  /*p08.AKAN*/ cpu_pins.A02.set(!TOVA_MODE_DBG2n, not(A02_Cn));
  /*p08.ANAR*/ cpu_pins.A03.set(!TOVA_MODE_DBG2n, not(A03_Cn));
  /*p08.AZUV*/ cpu_pins.A04.set(!TOVA_MODE_DBG2n, not(A04_Cn));
  /*p08.AJOV*/ cpu_pins.A05.set(!TOVA_MODE_DBG2n, not(A05_Cn));
  /*p08.BYNE*/ cpu_pins.A06.set(!TOVA_MODE_DBG2n, not(A06_Cn));
  /*p08.BYNA*/ cpu_pins.A07.set(!TOVA_MODE_DBG2n, not(A07_Cn));
  /*p08.LOFA*/ cpu_pins.A08.set(!TOVA_MODE_DBG2n, not(A08_Cn));
  /*p08.MAPU*/ cpu_pins.A09.set(!TOVA_MODE_DBG2n, not(A09_Cn));
  /*p08.RALA*/ cpu_pins.A10.set(!TOVA_MODE_DBG2n, not(A10_Cn));
  /*p08.LORA*/ cpu_pins.A11.set(!TOVA_MODE_DBG2n, not(A11_Cn));
  /*p08.LYNA*/ cpu_pins.A12.set(!TOVA_MODE_DBG2n, not(A12_Cn));
  /*p08.LEFY*/ cpu_pins.A13.set(!TOVA_MODE_DBG2n, not(A13_Cn));
  /*p08.NEFE*/ cpu_pins.A14.set(!TOVA_MODE_DBG2n, not(A14_Cn));
  /*p08.SYZU*/ cpu_pins.A15.set(!TOVA_MODE_DBG2n, not(A15_Cn));
  }
#endif

