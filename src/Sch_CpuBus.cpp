#include "Sch_CpuBus.h"

#include "TestGB.h"

using namespace Schematics;

//------------------------------------------------------------------------------

CpuSignals CpuBus::sig(const TestGB& gb) const {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);

  auto& cpu_bus = gb.cpu_bus;

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

  /*p07.UJYV*/ wire UJYV_BUS_RD_MUX = mux2_n(gb.ext_bus.PIN_RD_C, gb.cpu_bus.PIN_CPU_RAW_RD, dbg_sig.UNOR_MODE_DBG2p);
  /*p07.TEDO*/ wire TEDO_CPU_RD = not(UJYV_BUS_RD_MUX);
  /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
  /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(gb.cpu_bus.PIN_CPU_RAW_WR, clk_sig.AFAS_xxxxxFGH);
  /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);
  /*p07.UBAL*/ wire UBAL_CPU_WR_ABCDExxx = mux2_n(gb.ext_bus.PIN_WR_C, APOV_CPU_WR_xxxxxFGH, dbg_sig.UNOR_MODE_DBG2p);
  /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(UBAL_CPU_WR_ABCDExxx);
  /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
  /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);

  // i do not trust this block, seems weird.

  // LAGU 5-rung
  // LAGU01

  /*p08.LAGU*/ wire _LAGU = or(and(cpu_bus.PIN_CPU_RAW_RD, adr_sig.LEVO_8000_9FFFp), cpu_bus.PIN_CPU_RAW_WR);
  /*p08.LYWE*/ wire _LYWE = not(_LAGU);
  /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor(adr_sig.TEXO_8000_9FFFn, dbg_sig.UMUT_MODE_DBG1p);
  /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or(_MOCA_DBG_EXT_RD, _LYWE);

  /*p04.DECY*/ wire _DECY_FROM_CPU5n = not(cpu_bus.PIN_FROM_CPU5p);

  /*p04.CATY*/ wire _CATY_FROM_CPU5p = not(_DECY_FROM_CPU5n);
  /*p28.MYNU*/ wire _MYNU_CPU_RDn = nand(ASOT_CPU_RD, _CATY_FROM_CPU5p);
  /*p28.LEKO*/ wire _LEKO_CPU_RDp = not(_MYNU_CPU_RDn);

  /*p08.MULE*/ wire _MULE_MODE_DBG1n = not(dbg_sig.UMUT_MODE_DBG1p);
  /*p08.LOXO*/ wire _LOXO_LATCH_CPU_ADDRp = or (and (_MULE_MODE_DBG1n, adr_sig.TEXO_8000_9FFFn), dbg_sig.UMUT_MODE_DBG1p);
  /*p08.LASY*/ wire _LASY_LATCH_CPU_ADDRn = not(_LOXO_LATCH_CPU_ADDRp);
  /*p08.MATE*/ wire _MATE_LATCH_CPU_ADDRp = not(_LASY_LATCH_CPU_ADDRn);
  /*p08.LAVO*/ wire _LAVO_LATCH_CPU_DATAp = nand(cpu_bus.PIN_CPU_RAW_RD, adr_sig.TEXO_8000_9FFFn, cpu_bus.PIN_FROM_CPU5p);

  /*p08.REDU*/ wire _REDU_CPU_RD = not(TEDO_CPU_RD);
  /*p08.RORU*/ wire _RORU_IBUS_TO_EBUSn = mux2_p(_REDU_CPU_RD, _MOTY_CPU_EXT_RD, dbg_sig.UNOR_MODE_DBG2p);

  return {
    .TEDO_CPU_RD = TEDO_CPU_RD,
    .ASOT_CPU_RD = ASOT_CPU_RD,
    .MOCA_DBG_EXT_RD = _MOCA_DBG_EXT_RD,
    .MOTY_CPU_EXT_RD = _MOTY_CPU_EXT_RD,
    .TAPU_CPU_WR_xxxxxFGH = TAPU_CPU_WR_xxxxxFGH,
    .CUPA_CPU_WR_xxxxxFGH = CUPA_CPU_WR_xxxxxFGH,
    .APOV_CPU_WR_xxxxxFGH = APOV_CPU_WR_xxxxxFGH,
    .MAKA_FROM_CPU5_SYNC = MAKA_FROM_CPU5_SYNC,
    .DECY_FROM_CPU5n = _DECY_FROM_CPU5n,
    .LEKO_CPU_RDp = _LEKO_CPU_RDp,
    .MATE_LATCH_CPU_ADDRp = _MATE_LATCH_CPU_ADDRp,
    .LAVO_LATCH_CPU_DATAp = _LAVO_LATCH_CPU_DATAp,
    .RORU_IBUS_TO_EBUSn = _RORU_IBUS_TO_EBUSn,
  };
}

//------------------------------------------------------------------------------

void CpuBus::tick(TestGB& gb) {
  auto& cpu_bus = gb.cpu_bus;

  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);

  {
    /*p04.MAKA*/ MAKA_FROM_CPU5_SYNC.set(clk_sig.ZEME_AxCxExGx, rst_sig.CUNU_RSTn, PIN_FROM_CPU5p);
  }

#if 0
  // Debug stuff I disabled

  /*p07.APET*/ wire APET_MODE_DBG = or(dbg_sig.UMUT_MODE_DBG1p, UNOR_MODE_DBG2p); // suggests UMUTp
  /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, cpu_bus.PIN_A05, cpu_bus.PIN_A06, TAPU_CPUWR, ADDR_111111110xx00000);

  //----------
  // weird debug things, probably not right

  /*p05.AXYN*/ wire AXYN_xBCDEFGH = not(clk_sig.PIN_BEDO_xBxxxxxx);
  /*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
  /*p05.APYS*/ wire APYS_xBCDEFGH = nor(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
  /*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
  /*p07.LECO*/ wire LECO_xBCDEFGH = nor(clk_sig.PIN_BEDO_xBxxxxxx, sys_sig.MODE_DBG2);

  if (AFOP_Axxxxxxx) cpu_bus.set_data(
    /*p05.ANOC*/ not(sys_sig.PIN_P10_B),
    /*p05.ATAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.AJEC*/ not(sys_sig.PIN_P10_B),
    /*p05.ASUZ*/ not(sys_sig.PIN_P10_B),
    /*p05.BENU*/ not(sys_sig.PIN_P10_B),
    /*p05.AKAJ*/ not(sys_sig.PIN_P10_B),
    /*p05.ARAR*/ not(sys_sig.PIN_P10_B),
    /*p05.BEDA*/ not(sys_sig.PIN_P10_B)
  );

  if (LECO_xBCDEFGH) cpu_bus.set_data(
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

  /*p25.TUSO*/ wire TUSO = nor(MODE_DBG2, clk.PIN_BOGA_AxCDEFGH);
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

  // FIXME
  ///*p05.KORE*/ wire P05_NC0 = nand(DBG_FF00_D7, FF60_0);
  ///*p05.KYWE*/ wire P05_NC1 = nor (DBG_FF00_D7, FF60_0o);

  /*p08.LYRA*/ wire DBG_D_RDn = nand(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
  /*p08.TUTY*/ if (!DBG_D_RDn) cpu_bus.TS_D0 = not(/*p08.TOVO*/ not(pins.PIN_D0_C));
  /*p08.SYWA*/ if (!DBG_D_RDn) cpu_bus.TS_D1 = not(/*p08.RUZY*/ not(pins.PIN_D1_C));
  /*p08.SUGU*/ if (!DBG_D_RDn) cpu_bus.TS_D2 = not(/*p08.ROME*/ not(pins.PIN_D2_C));
  /*p08.TAWO*/ if (!DBG_D_RDn) cpu_bus.TS_D3 = not(/*p08.SAZA*/ not(pins.PIN_D3_C));
  /*p08.TUTE*/ if (!DBG_D_RDn) cpu_bus.TS_D4 = not(/*p08.TEHE*/ not(pins.PIN_D4_C));
  /*p08.SAJO*/ if (!DBG_D_RDn) cpu_bus.TS_D5 = not(/*p08.RATU*/ not(pins.PIN_D5_C));
  /*p08.TEMY*/ if (!DBG_D_RDn) cpu_bus.TS_D6 = not(/*p08.SOCA*/ not(pins.PIN_D6_C));
  /*p08.ROPA*/ if (!DBG_D_RDn) cpu_bus.TS_D7 = not(/*p08.RYBA*/ not(pins.PIN_D7_C));
#endif

  // hack, not correct
#if 0
  {
    // FF60 debug reg
    /*p07.APET*/ wire APET_MODE_DBG = or(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
    /*p07.APER*/ wire FF60_WRn = nand(APET_MODE_DBG, cpu_bus.PIN_A05, cpu_bus.PIN_A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

    /*p05.KURA*/ wire FF60_0n = not(BURO_FF60_0);
    /*p05.JEVA*/ wire FF60_0o = not(BURO_FF60_0);
    /*p07.BURO*/ BURO_FF60_0.set(FF60_WRn, rst_sig.SYS_RESETn, cpu_bus.TS_D0);
    /*p07.AMUT*/ AMUT_FF60_1.set(FF60_WRn, rst_sig.SYS_RESETn, cpu_bus.TS_D1);

    ///*p05.KURA*/ wire FF60_0n = not(FF60);
    ///*p05.JEVA*/ wire FF60_0o = not(FF60);
    /*p07.BURO*/ dbg_reg.BURO_FF60_0.set(1, rst_sig.ALUR_RSTn, cpu_bus.TS_D0);
    /*p07.AMUT*/ dbg_reg.AMUT_FF60_1.set(1, rst_sig.ALUR_RSTn, cpu_bus.TS_D1);
  }
#endif

  {
    cpu_bus.PIN_SYRO.set(adr_sig.SYRO_FE00_FFFFp);
  }
}

//------------------------------------------------------------------------------

bool CpuBus::commit() {
  bool changed = false;

  /*p04.MAKA*/ changed |= MAKA_FROM_CPU5_SYNC.commit_reg();
  ///*p07.TEPU*/ changed |= BOOT_BITn.commit_reg();

  changed |= PIN_TO_CPU2.commit_pinout();       // <- P02.AWOB
  changed |= PIN_CPU_RAW_RD.clear_preset();     // PORTA_00: -> UJYV, LAGU, LAVO
  changed |= PIN_CPU_RAW_WR.clear_preset();     // PORTA_01: -> AREV, LAGU.
  changed |= PIN_READ_BOOTROM.commit_pinout();  // PORTA_04: <- TUTU
  changed |= PIN_ADDR_VALID.clear_preset();     // PORTA_06: -> APAP, TEXO
  changed |= PIN_CLKREQ.clear_preset();         // PORTC_00: -> ABOL
  changed |= PIN_AFER.commit_pinout();          // PORTC_01: <- AFER
  changed |= PIN_RESET.commit_pinout();         // PORTC_02: <- PIN_RESET directly connected to the pad
  changed |= PIN_CLK_GOOD.commit_pinout();      // PORTC_03: <- CLKIN_A
  changed |= PIN_CPU_RESET.commit_pinout();     // PORTC_04: <- TABA
  changed |= PIN_FROM_CPU6.clear_preset();      // PORTD_00: -> LEXY, doesn't do anything

  changed |= PIN_BOWA_AxCDEFGH.commit_pinout(); // PORTD_01: <- BOWA
  changed |= PIN_BEDO_xBxxxxxx.commit_pinout(); // PORTD_02: <- BEDO _____fgh

  changed |= PIN_BEKO_xBCDExxx.commit_pinout(); // PORTD_03: <- BEKO ____efgh connection not indicated on P01
  changed |= PIN_BUDE_AxxxxFGH.commit_pinout(); // PORTD_04: <- BUDE abcd____

  changed |= PIN_BOLO_xBCDEFGx.commit_pinout(); // PORTD_05: <- BOLO
  changed |= PIN_FROM_CPU5p.clear_preset();      // PORTD_05: -> FROM_CPU5
  changed |= PIN_BUKE_ABxxxxxH.commit_pinout(); // PORTD_06: <- BUKE _____f__

  changed |= PIN_BOMA_xBxxxxxx.commit_pinout(); // PORTD_07: <- BOMA _____fgh
  changed |= PIN_BOGA_AxCDEFGH.commit_pinout(); // PORTD_08: <- BOGA abcde___

  changed |= PIN_A00.clear_preset();
  changed |= PIN_A01.clear_preset();
  changed |= PIN_A02.clear_preset();
  changed |= PIN_A03.clear_preset();
  changed |= PIN_A04.clear_preset();
  changed |= PIN_A05.clear_preset();
  changed |= PIN_A06.clear_preset();
  changed |= PIN_A07.clear_preset();
  changed |= PIN_A08.clear_preset();
  changed |= PIN_A09.clear_preset();
  changed |= PIN_A10.clear_preset();
  changed |= PIN_A11.clear_preset();
  changed |= PIN_A12.clear_preset();
  changed |= PIN_A13.clear_preset();
  changed |= PIN_A14.clear_preset();
  changed |= PIN_A15.clear_preset();
  changed |= TS_D0.commit_tribuf();
  changed |= TS_D1.commit_tribuf();
  changed |= TS_D2.commit_tribuf();
  changed |= TS_D3.commit_tribuf();
  changed |= TS_D4.commit_tribuf();
  changed |= TS_D5.commit_tribuf();
  changed |= TS_D6.commit_tribuf();
  changed |= TS_D7.commit_tribuf();
  return changed;
}

//------------------------------------------------------------------------------