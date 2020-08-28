#include "GateBoyLib/Sch_Top.h"
#include <memory.h>

using namespace Schematics;

#pragma warning(disable:4100)

//-----------------------------------------------------------------------------
// optimizer still fscking this up somehow

#pragma optimize("", off)
void SchematicTop::tick_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {

  dma_reg.tick();

  {
    /*p07.UBET*/ wire UBET_T1p = not1(T1n);
    /*p07.UVAR*/ wire UVAR_T2p = not1(T2n);
    /*p07.UMUT*/ UMUT_MODE_DBG1p = and2(T1n, UVAR_T2p);
    /*p07.UNOR*/ UNOR_MODE_DBG2p = and2(T2n, UBET_T1p);
    /*p07.UPOJ*/ UPOJ_MODE_PRODn = nand3(UBET_T1p, UVAR_T2p, RST);

    /*p25.TUTO*/ TUTO_DBG_VRAMp = and2(UNOR_MODE_DBG2p, SOTO_DBG_VRAM.qn());
  }

  {
    /*p??.APAP*/ wire APAP_AVn = not1(cpu_bus.CPU_PIN_ADDR_EXTp.tp()); // Missing from schematic
    /*p01.AWOD*/ wire AWOD_AVp = nor2(UNOR_MODE_DBG2p, APAP_AVn);
    /*p01.ABUZ*/ ABUZ_AVn = not1(AWOD_AVp);
  }

  {
    /* p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(!ext_bus.EXT_PIN_RDn.qp(), cpu_bus.CPU_PIN_RDp.tp(), UNOR_MODE_DBG2p);
    /* p07.TEDO*/ TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /* p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /*p04.DECY*/ DECY_LATCH_EXTn = not1(cpu_bus.CPU_PIN_LATCH_EXT.tp());
    /*p04.CATY*/ wire CATY_LATCH_EXTp = not1(DECY_LATCH_EXTn);
    /* p28.MYNU*/ wire MYNU_CPU_RDn = nand2(ASOT_CPU_RDp, CATY_LATCH_EXTp);
    /* p28.LEKO*/ LEKO_CPU_RDp = not1(MYNU_CPU_RDn);
  }

  {
    /*p27.ROMO*/ wire ROMO_PRELOAD_DONEn = not1(tile_fetcher._POKY_PRELOAD_LATCHp.qp());
    /*p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(pix_pipe.XYMU_RENDERINGp(),
                                                      ROMO_PRELOAD_DONEn,
                                                      tile_fetcher._NYKA_FETCH_DONE_P11.qp(),
                                                      tile_fetcher._PORY_FETCH_DONE_P12.qp());
    // -> sprite fetcher, top.TEVO
    /*p27.TAVE*/ TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);

    /*p27.SYLO*/ wire SYLO_WIN_HITn = not1(pix_pipe.RYDY_WIN_FIRST_TILE_A.tp());
    /*p27.TUXY*/ wire TUXY_WIN_FIRST_TILE_NE = nand2(SYLO_WIN_HITn, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp());
    /*p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILE_NE);

    // -> ppu.PASO, window.VETU, top.NYXU_TILE_FETCHER_RSTn
    /*p27.TEVO*/ TEVO_FETCH_TRIGp = or3(pix_pipe.SEKO_WIN_TILE_TRIG(),
                                        SUZU_WIN_FIRST_TILEne,
                                        TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  }

  {
    // int.asam, oam.aver/ajep, ppu.xaty, top.apar/.ajuj
    // so dma stops oam scan?
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma_reg.MATU_DMA_RUNNINGp);
    /*p28.ACYL*/ ACYL_SCANNINGp = and2(BOGE_DMA_RUNNINGn, sprite_scanner.BESU_SCANNINGp.tp());
  }

  clk_reg.tick_slow(CLK, CLKGOOD, CPUREADY, *this);
  lcd_reg.tick(*this);

  {
    /*p01.ATAR*/ wire ATAR_VID_RSTp = not1(clk_reg.XAPO_VID_RSTn);
    /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(lcd_reg.ATEJ_VID_LINE_END_TRIGp, ATAR_VID_RSTp);
    /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_B.qp(), BALU_LINE_RSTp, sprite_scanner.BYBA_SCAN_DONE_A.qn());
    /*#p29.AVAP*/ AVAP_RENDER_START_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  }

  {
    /*p27.NYFO*/ wire NYFO_WIN_FETCH_TRIGn = not1(pix_pipe.NUNY_WX_MATCH_TRIGp());
    /*p27.MOSU*/ wire MOSU_WIN_FETCH_TRIGp = not1(NYFO_WIN_FETCH_TRIGn);

    // pxp.loze, pxp.luxa, tile.lony/lovy/laxu/mesu/nyva/moce
    // low on phase 0 of bg tile fetch
    /*p27.NYXU*/ NYXU_FETCH_TRIGn = nor3(AVAP_RENDER_START_TRIGp, MOSU_WIN_FETCH_TRIGp, TEVO_FETCH_TRIGp);
  }

  {
    /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(clk_reg.ADAR_ABCxxxxH, clk_reg.ATYP_ABCDxxxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand2(cpu_bus.CPU_PIN_WRp.tp(), AFAS_xxxxEFGx);
    /*p01.APOV*/ APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH = mux2_n(!ext_bus.EXT_PIN_WRn.qp(), APOV_CPU_WRp_xxxxEFGx, UNOR_MODE_DBG2p);
    /*p07.TAPU*/ TAPU_CPU_WRp_xxxxEFGx = not1(UBAL_CPU_WRn_ABCDxxxH); // boot.TUGE, int.REFA, joy.ATOZ, ser.URYS/UWAM, timer.TAPE/TOPE/TYJU/SARA, top.DYKY
  }

  {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not1(bootrom.BOOT_BITn.qp());
    /*p07.TUTU*/ TUTU_ADDR_BOOTp = and2(TERA_BOOT_BITp, cpu_bus.TULO_ADDR_00XXp());
  }

  {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma_reg.MATU_DMA_RUNNINGp);
    /*p28.AJON*/ AJON_OAM_BUSY = and2(BOGE_DMA_RUNNINGn, pix_pipe.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ AJUJ_OAM_BUSYn = nor3(dma_reg.MATU_DMA_RUNNINGp, ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor4
  }

  sprite_scanner.tick(*this);
  sprite_store.tick(*this);
  pix_pipe.tick(*this);
  tim_reg.tick(*this);
  ser_reg.tick(*this);
  joypad.tick(*this);
  sprite_fetcher.tick(*this);
  int_reg.tick(*this);
}
#pragma optimize("", on)

//------------------------------------------------------------------------------

#pragma optimize("", off)
void SchematicTop::tock_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  {
    /*p25.SYCY*/ wire SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p);
    /*p01.DULA*/ wire DULA_SYS_RSTp = not1(clk_reg.ALUR_SYS_RSTn);
    /*p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
    /*p25.SOTO*/ SOTO_DBG_VRAM = dff17_A(SYCY_DBG_CLOCKn, CUNU_SYS_RSTn, SOTO_DBG_VRAM.qn());
  }

  clk_reg.tock_clk_slow(RST, CLK, CLKGOOD, CPUREADY, *this);
  clk_reg.tock_rst_slow(RST, CLKGOOD, CPUREADY, *this);
  clk_reg.tock_dbg_slow(*this);
  clk_reg.tock_vid_slow(CLK, *this);
  
  tim_reg.tock(RST, *this, cpu_bus);
  bootrom.tock(*this, cpu_bus);
  dma_reg.tock(*this, cpu_bus);
  ser_reg.tock(*this, cpu_bus);
  joypad.tock(*this, cpu_bus);
  sprite_scanner.tock(*this);
  lcd_reg.tock(*this, cpu_bus);
  sprite_store.tock(*this);

  pix_pipe.tock(*this, cpu_bus);
  sprite_fetcher.tock(*this);
  tile_fetcher.tock(*this);
  int_reg.tock(*this, cpu_bus);

  cpu_bus.CPU_PIN_ADDR_HI = cpu_bus.SYRO_FE00_FFFFp();
  cpu_bus.CPU_PIN_BOOTp = TUTU_ADDR_BOOTp;

  ext_bus.tock(*this);
  oam_bus.tock(*this);
  vram_bus.tock(*this);
}
#pragma optimize("", on)

//------------------------------------------------------------------------------

#if 0
// Debug stuff I disabled

/*p07.APET*/ wire APET_MODE_DBG = or2(top.clk_reg.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, CPU_BUS_A05, CPU_BUS_A06, TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/*p05.AXYN*/ wire AXYN_xBCDEFGH = not1(clk_reg.BEDO_Axxxxxxx);
/*p05.ADYR*/ wire ADYR_Axxxxxxx = not1(AXYN_xBCDEFGH);
/*p05.APYS*/ wire APYS_xBCDEFGH = nor4(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/*p05.AFOP*/ wire AFOP_Axxxxxxx = not1(APYS_xBCDEFGH);
/*p07.LECO*/ wire LECO_xBCDEFGH = nor4(clk_reg.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

if (AFOP_Axxxxxxx) set_data(
  /*p05.ANOC*/ not1(top.GND),
  /*p05.ATAJ*/ not1(top.GND),
  /*p05.AJEC*/ not1(top.GND),
  /*p05.ASUZ*/ not1(top.GND),
  /*p05.BENU*/ not1(top.GND),
  /*p05.AKAJ*/ not1(top.GND),
  /*p05.ARAR*/ not1(top.GND),
  /*p05.BEDA*/ not1(top.GND)
);

if (LECO_xBCDEFGH) set_data(
  /*p07.ROMY*/ top.GND,
  /*p07.RYNE*/ top.GND,
  /*p07.REJY*/ top.GND,
  /*p07.RASE*/ top.GND,
  /*p07.REKA*/ top.GND,
  /*p07.ROWE*/ top.GND,
  /*p07.RYKE*/ top.GND,
  /*p07.RARU*/ top.GND
);



//----------
// more debug stuff

/*p25.TUSO*/ wire TUSO = nor4(MODE_DBG2, dff20.CPU_PIN_BOGA_xBCDEFGH);
/*p25.SOLE*/ wire SOLE = not1(TUSO);

if (top.VYPO_GND) bus_out.set_data(
  /*p25.TOVU*/ SOLE,
  /*p25.SOSA*/ SOLE,
  /*p25.SEDU*/ SOLE,
  /*p25.TAXO*/ SOLE,
  /*p25.TAHY*/ SOLE,
  /*p25.TESU*/ SOLE,
  /*p25.TAZU*/ SOLE,
  /*p25.TEWA*/ SOLE
);

///*p05.KORE*/ wire P05_NC0 = nand2(KERU_DBG_FF00_D7, FF60_0);
///*p05.KYWE*/ wire P05_NC1 = nor4 (KERU_DBG_FF00_D7, FF60_0o);

/*p08.LYRA*/ wire DBG_D_RDn = nand2(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
/*p08.TUTY*/ if (!DBG_D_RDn) CPU_BUS_D0p = not1(/*p08.TOVO*/ not1(pins.PIN_D0_C));
/*p08.SYWA*/ if (!DBG_D_RDn) CPU_BUS_D1p = not1(/*p08.RUZY*/ not1(pins.PIN_D1_C));
/*p08.SUGU*/ if (!DBG_D_RDn) CPU_BUS_D2p = not1(/*p08.ROME*/ not1(pins.PIN_D2_C));
/*p08.TAWO*/ if (!DBG_D_RDn) CPU_BUS_D3p = not1(/*p08.SAZA*/ not1(pins.PIN_D3_C));
/*p08.TUTE*/ if (!DBG_D_RDn) CPU_BUS_D4p = not1(/*p08.TEHE*/ not1(pins.PIN_D4_C));
/*p08.SAJO*/ if (!DBG_D_RDn) CPU_BUS_D5p = not1(/*p08.RATU*/ not1(pins.PIN_D5_C));
/*p08.TEMY*/ if (!DBG_D_RDn) CPU_BUS_D6p = not1(/*p08.SOCA*/ not1(pins.PIN_D6_C));
/*p08.ROPA*/ if (!DBG_D_RDn) CPU_BUS_D7p = not1(/*p08.RYBA*/ not1(pins.PIN_D7_C));
#endif

// hack, not correct
#if 0
{
  // FF60 debug state
  /*p07.APET*/ wire APET_MODE_DBG = or2(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, CPU_BUS_A05, CPU_BUS_A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not1(BURO_FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not1(BURO_FF60_0);
  /*p07.BURO*/ BURO_FF60_0 = ff9(FF60_WRn, rst_sig.SYS_RESETn, CPU_BUS_D0p);
  /*p07.AMUT*/ AMUT_FF60_1 = ff9(FF60_WRn, rst_sig.SYS_RESETn, CPU_BUS_D1p);

  ///*p05.KURA*/ wire FF60_0n = not1(FF60);
  ///*p05.JEVA*/ wire FF60_0o = not1(FF60);
}
#endif


#if 0
// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
// If we're in debug mode 2, drive external address bus onto internal address
// these may be backwards, probably don't want to drive external address onto bus normally...

/*p08.KOVA*/ wire A00_Cn = not1(EXT_PIN_A00_C);
/*p08.CAMU*/ wire A01_Cn = not1(EXT_PIN_A01_C);
/*p08.BUXU*/ wire A02_Cn = not1(EXT_PIN_A02_C);
/*p08.BASE*/ wire A03_Cn = not1(EXT_PIN_A03_C);
/*p08.AFEC*/ wire A04_Cn = not1(EXT_PIN_A04_C);
/*p08.ABUP*/ wire A05_Cn = not1(EXT_PIN_A05_C);
/*p08.CYGU*/ wire A06_Cn = not1(EXT_PIN_A06_C);
/*p08.COGO*/ wire A07_Cn = not1(EXT_PIN_A07_C);
/*p08.MUJY*/ wire A08_Cn = not1(EXT_PIN_A08_C);
/*p08.NENA*/ wire A09_Cn = not1(EXT_PIN_A09_C);
/*p08.SURA*/ wire A10_Cn = not1(EXT_PIN_A10_C);
/*p08.MADY*/ wire A11_Cn = not1(EXT_PIN_A11_C);
/*p08.LAHE*/ wire A12_Cn = not1(EXT_PIN_A12_C);
/*p08.LURA*/ wire A13_Cn = not1(EXT_PIN_A13_C);
/*p08.PEVO*/ wire A14_Cn = not1(EXT_PIN_A14_C);
/*p08.RAZA*/ wire A15_Cn = not1(EXT_PIN_A15_C);

/*p08.KEJO*/ top.cpu_bus.CPU_BUS_A00 = tribuf_10nn(TOVA_MODE_DBG2n, A00_Cn);
/*p08.BYXE*/ top.cpu_bus.CPU_BUS_A01 = tribuf_10nn(TOVA_MODE_DBG2n, A01_Cn);
/*p08.AKAN*/ top.cpu_bus.CPU_BUS_A02 = tribuf_10nn(TOVA_MODE_DBG2n, A02_Cn);
/*p08.ANAR*/ top.cpu_bus.CPU_BUS_A03 = tribuf_10nn(TOVA_MODE_DBG2n, A03_Cn);
/*p08.AZUV*/ top.cpu_bus.CPU_BUS_A04 = tribuf_10nn(TOVA_MODE_DBG2n, A04_Cn);
/*p08.AJOV*/ top.cpu_bus.CPU_BUS_A05 = tribuf_10nn(TOVA_MODE_DBG2n, A05_Cn);
/*p08.BYNE*/ top.cpu_bus.CPU_BUS_A06 = tribuf_10nn(TOVA_MODE_DBG2n, A06_Cn);
/*p08.BYNA*/ top.cpu_bus.CPU_BUS_A07 = tribuf_10nn(TOVA_MODE_DBG2n, A07_Cn);
/*p08.LOFA*/ top.cpu_bus.CPU_BUS_A08 = tribuf_10nn(TOVA_MODE_DBG2n, A08_Cn);
/*p08.MAPU*/ top.cpu_bus.CPU_BUS_A09 = tribuf_10nn(TOVA_MODE_DBG2n, A09_Cn);
/*p08.RALA*/ top.cpu_bus.CPU_BUS_A10 = tribuf_10nn(TOVA_MODE_DBG2n, A10_Cn);
/*p08.LORA*/ top.cpu_bus.CPU_BUS_A11 = tribuf_10nn(TOVA_MODE_DBG2n, A11_Cn);
/*p08.LYNA*/ top.cpu_bus.CPU_BUS_A12 = tribuf_10nn(TOVA_MODE_DBG2n, A12_Cn);
/*p08.LEFY*/ top.cpu_bus.CPU_BUS_A13 = tribuf_10nn(TOVA_MODE_DBG2n, A13_Cn);
/*p08.NEFE*/ top.cpu_bus.CPU_BUS_A14 = tribuf_10nn(TOVA_MODE_DBG2n, A14_Cn);
/*p08.SYZU*/ top.cpu_bus.CPU_BUS_A15 = tribuf_10nn(TOVA_MODE_DBG2n, A15_Cn);
}
#endif

//-----------------------------------------------------------------------------
