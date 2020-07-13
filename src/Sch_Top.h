#pragma once

#include "Sch_Timer.h"
#include "Sch_LCD.h"
#include "Sch_DMA.h"
#include "Sch_SpriteStore.h"
#include "Sch_PPU.h"
#include "Sch_Window.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Debug.h"
#include "Sch_PixPipe.h"
#include "Sch_Joypad.h"
#include "Sch_Serial.h"
#include "Sch_BusMux.h"
#include "Sch_Interrupts.h"
#include "Sch_Bootrom.h"
#include "Sch_SpriteFetcher.h"
#include "Sch_TileFetcher.h"
#include "Sch_SpriteScanner.h"

#include "Sch_OamBus.h"
#include "Sch_ExtBus.h"
#include "Sch_IntBus.h"
#include "Sch_VramBus.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SchematicTop {

  SchematicTop();

  SchematicTop& top = *this;

  void set_buttons(uint8_t buttons);

  SignalHash tick();

  /*p25.COTA*/ wire COTA_OAM_CLKn() const {
    /*p25.AVER*/ wire _AVER_SCAN_OAM_CLK = nand(top.ACYL_SCANNINGp(), top.clk_reg.XYSO_ABCxDEFx()); 
    /*p25.CUFE*/ wire _CUFE_DMA_OAM_CLK  = and (or (top.int_bus.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp()), top.clk_reg.MOPA_xxxxEFGH());
    /*p25.BYCU*/ wire _BYCU_OAM_CLK      = nand(_AVER_SCAN_OAM_CLK, top.sprite_fetcher.XUJY_PPU_OAM_CLK(), _CUFE_DMA_OAM_CLK);
    return not(_BYCU_OAM_CLK);
  }

  /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn() const {
     return nor(top.dma_reg.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), top.AJON_OAM_BUSY()); // def nor
  }

  /*p28.AMAB*/ wire AMAB_OAM_LOCKn() const {
    return and (top.int_bus.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn()); // def and
  }

  /*p04.WYJA*/ wire WYJA_OAM_WRp() const { // -> oam_bus
    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp = nor(top.clk_reg.UVYT_ABCDxxxx(), top.int_bus.MAKA_FROM_CPU5_SYNC.q()); // def nor
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp = and (top.dma_reg.MATU_DMA_RUNNINGp(), _NAXY_DMA_OAM_WENp); // def and
    return or (and (AMAB_OAM_LOCKn(), top.CUPA_CPU_WRp_xxxxEFGx()), _POWU_DMA_OAM_WRp);
  }

  /*p28.BODE*/ wire BODE_OAM_LATCHp() const { // -> oam_bus
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand(top.DECY_FROM_CPU5n(), top.int_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCHn      = and(_AJEP_SCAN_OAM_LATCHn, top.sprite_fetcher.XUJA_SPR_OAM_LATCHn(), _BOTA_CPU_OAM_LATCHn); // def and
    return not(_ASYT_OAM_LATCHn);
  }

  /*p28.WEWU*/ wire WEWU_OAM_A_CPU_RDn() const {
    /*p28.WUKU*/ wire _WUKU_OAM_A_CPU_RD  = and (top.LEKO_CPU_RDp(), top.AMAB_OAM_LOCKn(), top.oam_bus.GEKA_OAM_A0p());
    return not(_WUKU_OAM_A_CPU_RD);
  }

  /*p28.WUME*/ wire WUME_OAM_B_CPU_RDn() const {
    /*p28.GUKO*/ wire _GUKO_OAM_B_CPU_RD  = and (top.LEKO_CPU_RDp(), top.AMAB_OAM_LOCKn(), top.oam_bus.WAFO_OAM_A0n());
    return not(_GUKO_OAM_B_CPU_RD);
  }

  /*p08.MATE*/ wire MATE_LATCH_CPU_ADDRp() const {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n = not(top.UMUT_MODE_DBG1p());
    /*p08.LOXO*/ wire _LOXO_LATCH_CPU_ADDRp = or (and (_MULE_MODE_DBG1n, top.int_bus.TEXO_8000_9FFFn()), top.UMUT_MODE_DBG1p());
    /*p08.LASY*/ wire _LASY_LATCH_CPU_ADDRn = not(_LOXO_LATCH_CPU_ADDRp);
    return not(_LASY_LATCH_CPU_ADDRn);
  }

  /*p08.MOCA*/ wire MOCA_DBG_EXT_RD() const { return nor(top.int_bus.TEXO_8000_9FFFn(), top.UMUT_MODE_DBG1p()); }

  /*p08.MOTY*/ wire MOTY_CPU_EXT_RD() const {
    /*p08.LAGU*/ wire _LAGU = or(and(top.int_bus.CPU_PIN_RDp, top.int_bus.LEVO_8000_9FFFp()), top.int_bus.CPU_PIN_WRp);
    /*p08.LYWE*/ wire _LYWE = not(_LAGU);
    return or(MOCA_DBG_EXT_RD(), _LYWE);
  }
  
  /*p08.RORU*/ wire RORU_IBUS_TO_EBUSn() const {
    /*p08.REDU*/ wire _REDU_CPU_RDn = not(top.TEDO_CPU_RDp());
    return mux2_p(_REDU_CPU_RDn, MOTY_CPU_EXT_RD(), top.UNOR_MODE_DBG2p());
  }

  /*p08.TYMU*/ wire TYMU_RD_OUTn() const { return nor(top.dma_reg.LUMA_DMA_READ_CARTp(), MOTY_CPU_EXT_RD()); }

  /*p08.PUVA*/ wire PUVA_EXT_WRn() const {
    /*p08.NEVY*/ wire _NEVY = or(top.MEXO_CPU_WRn_ABCDxxxH(), MOCA_DBG_EXT_RD());
    return or(_NEVY, top.dma_reg.LUMA_DMA_READ_CARTp());
  }

  /*p08.TAZY*/ wire TAZY_A15p() const {
    // This whole section is screwy
    // Where did AGUT go?
    // Where did AJAX go?

    // AJAX = not(ATYP)
    // AGUT = nor(AROV, AJAX)
    // AGUT looks unused

    /*p08.SOBY*/ wire _SOBY_A15n = nor(top.int_bus.CPU_PIN_A15, top.TUTU_ADDR_BOOTp());
    /*p08.SEPY*/ wire _SEPY_A15p = nand(top.ABUZ_CPU_ADDR_VALIDp(), _SOBY_A15n);
    return mux2_p(top.dma_reg.DMA_A15.q(), _SEPY_A15p, top.dma_reg.LUMA_DMA_READ_CARTp());
  }

  /*p25.SEBY*/ wire SEBY_VRM_TO_CPUp() const {
    /*p25.TYVY*/ wire _TYVY_VRM_TO_CPUn = nand(SERE_CPU_VRM_RDp(), LEKO_CPU_RDp());
    return not(_TYVY_VRM_TO_CPUn);
  }

  /*p25.TUTO*/ wire TUTO_DBG_VRAMp() const { return and (top.UNOR_MODE_DBG2p(), dbg_reg.SOTO_DBG.qn()); }
  /*p25.RACO*/ wire RACO_DBG_VRAMn() const { return not(TUTO_DBG_VRAMp()); }

  /*p25.SERE*/ wire SERE_CPU_VRM_RDp() const {
    /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and (top.int_bus.SOSE_8000_9FFFp(), top.ABUZ_CPU_ADDR_VALIDp());
    /*p25.TEFY*/ wire _TEFY_VRAM_MCSp    = not(vram_bus.VRAM_PIN_MCSn_C());
    /*p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2_p(_TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp, TUTO_DBG_VRAMp());
    return and (_TOLE_VRAM_RDp, ppu_reg.ROPY_RENDERINGn());
  }

  /*p25.SOHY*/ wire SOHY_MWRn() const {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and(top.int_bus.SOSE_8000_9FFFp(), APOV_CPU_WRp_xxxxEFGx());
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not(vram_bus.VRAM_PIN_MWRn_C());
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WRp = mux2_p(_SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp, TUTO_DBG_VRAMp());
    return nand(_TYJY_DBG_VRAM_WRp, SERE_CPU_VRM_RDp());
  }

  /*p25.SALE*/ wire SALE_VRAM_WRn() const {
    /*p25.TAVY*/ wire _TAVY_MOEp = not(vram_bus.VRAM_PIN_MOEn_C());
    return mux2_p(_TAVY_MOEp, top.int_bus.TEGU_CPU_VRAM_WRn(), TUTO_DBG_VRAMp());
  }

  /*p25.RUVY*/ wire RUVY_VRAM_WR() const { return not(SALE_VRAM_WRn()); }
  /*p25.SAZO*/ wire SAZO_VRAM_RD() const { return and (RUVY_VRAM_WR(), SERE_CPU_VRM_RDp()); }
  /*p25.RYJE*/ wire RYJE_VRAM_RDn() const { return not(SAZO_VRAM_RD()); }
  /*p25.REVO*/ wire REVO_VRAM_RDp() const { return not(RYJE_VRAM_RDn()); }
  /*p25.RELA*/ wire RELA_VRM_TO_CPUn() const { return or (REVO_VRAM_RDp(), SAZO_VRAM_RD()); }
  /*p25.RENA*/ wire RENA_VRM_TO_CPUp() const { return not(RELA_VRM_TO_CPUn()); }
  /*p25.ROCY*/ wire ROCY_CPU_TO_VRMp() const { return and (REVO_VRAM_RDp(), SAZO_VRAM_RD()); }
  /*p25.RAHU*/ wire RAHU_CPU_TO_VRMn() const { return not(ROCY_CPU_TO_VRMp()); }

  /*p25.RACU*/ wire RACU_MOEn() const {
    /*p25.RYLU*/ wire _RYLU_DBG_VRAM_RDn = nand(top.SALE_VRAM_WRn(), XANE_VRAM_LOCKn());
    /*p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp = and(sprite_fetcher.TACU_SPR_SEQ_5_TRIG(), ABON_SPR_VRM_RDn());
    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not(_SOHO_SPR_VRAM_RDp);
    return and (_RYLU_DBG_VRAM_RDn, _RAWA_SPR_VRAM_RDn, top.tile_fetcher.MYMA_BGW_VRAM_RDn(), top.dma_reg.APAM_DMA_VRAM_RDn()); // def and
  }

  /*p25.SUTU*/ wire SUTU_MCSn() const {
    // Polarity issues here, ABON should be P
    // ABON = not(TEXY)
    // SUTU = nor(LENA, LUFA, ABON, SERE);
    return nor(top.tile_fetcher.LENA_BGW_VRM_RDp(),
               top.dma_reg.LUFA_DMA_VRAM_RDp(),
               ABON_SPR_VRM_RDn(),
               SERE_CPU_VRM_RDp());
  }

  //-----------------------------------------------------------------------------

  /*p08.TOZA*/ wire TOZA_CPU_RAM_RDp() const { 
    return and(top.ABUZ_CPU_ADDR_VALIDp(), top.int_bus.TYNU_ADDR_RAM(), top.int_bus.TUNA_0000_FDFFp()); // suggests ABUZp
  }

  /*p01.ABUZ*/ wire ABUZ_CPU_ADDR_VALIDp() const {
    /*p??.APAP*/ wire APAP_CPU_ADDR_VALIDp = not(int_bus.CPU_PIN_ADDR_VALID); // Missing from schematic
    /*p01.AWOD*/ wire AWOD_CPU_ADDR_VALIDn = nor(UNOR_MODE_DBG2p(), APAP_CPU_ADDR_VALIDp);
    return not(AWOD_CPU_ADDR_VALIDn);
  }

  /*p07.TUTU*/ wire TUTU_ADDR_BOOTp() const {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not(bootrom.BOOT_BITn());
    return and(TERA_BOOT_BITp, top.int_bus.TULO_ADDR_00XXp());
  }

  /*p25.XANE*/ wire XANE_VRAM_LOCKn() const { return nor(dma_reg.LUFA_DMA_VRAM_RDp(), ppu_reg.XYMU_RENDERINGp()); } // def nor

  /*p28.AJON*/ wire AJON_OAM_BUSY() const {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_reg.MATU_DMA_RUNNINGp());
    return and(BOGE_DMA_RUNNINGn, ppu_reg.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
  }

  /*p27.NYXU*/ wire NYXU_TILE_FETCHER_RSTn() const { return nor(top.sprite_scanner.AVAP_RENDER_START_RST(), win_reg.MOSU_WIN_MODE_TRIGp(), win_reg.TEVO_FINE_RSTp()); }
  /*p29.ABON*/ wire ABON_SPR_VRM_RDn()       const { return not(TEXY_SPR_READ_VRAMp()); }

  //-----------------------------------------------------------------------------
  // Debug signals

  /*p07.UBET*/ wire UBET_T1p()        const { return not(SYS_PIN_T1n); }
  /*p07.UVAR*/ wire UVAR_T2p()        const { return not(SYS_PIN_T2n); }
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and(SYS_PIN_T1n, UVAR_T2p()); }
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and(SYS_PIN_T2n, UBET_T1p()); }
  /*p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand(UBET_T1p(), UVAR_T2p(), SYS_PIN_RSTp); }
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not(UNOR_MODE_DBG2p()); }

  //-----------------------------------------------------------------------------

  /*p07.TEDO*/ wire TEDO_CPU_RDp() const {
      /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(ext_bus.EXT_PIN_RDp_C(), int_bus.CPU_PIN_RDp, UNOR_MODE_DBG2p());
      return not(UJYV_CPU_RDn);
  }

  /*p07.AJAS*/ wire AJAS_CPU_RDn() const { return not(TEDO_CPU_RDp()); }
  /*p07.ASOT*/ wire ASOT_CPU_RDp() const { return not(AJAS_CPU_RDn()); }

  /*p04.DECY*/ wire DECY_FROM_CPU5n() const { return not(int_bus.CPU_PIN5); }

  /*p28.LEKO*/ wire LEKO_CPU_RDp() const {
    /*p04.CATY*/ wire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n());
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RDp(), CATY_FROM_CPU5p);
    return not(MYNU_CPU_RDn);
  }

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH() const { return nand(int_bus.CPU_PIN_WRp, clk_reg.AFAS_xxxxEFGx()); }
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx() const { return not(AREV_CPU_WRn_ABCDxxxH()); }
  /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH() const { return mux2_n(ext_bus.EXT_PIN_WRp_C(), APOV_CPU_WRp_xxxxEFGx(), UNOR_MODE_DBG2p()); }
  /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx() const { return not(UBAL_CPU_WRn_ABCDxxxH()); }
  /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH() const { return not(TAPU_CPU_WRp_xxxxEFGx()); }
  /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx() const { return not(DYKY_CPU_WRn_ABCDxxxH()); }
  /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH() const { return not(APOV_CPU_WRp_xxxxEFGx()); }

  //-----------------------------------------------------------------------------
  // PPU signals

  /*p28.ACYL*/ wire ACYL_SCANNINGp() const { return and(dma_reg.BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp()); } // so dma stops oam scan?

  /*p24.SEGU*/ wire SEGU_CLKPIPEn() const {
    /*p24.VYBO*/ wire _VYBO_PIX_CLK_xBxDxFxH = nor(sprite_store.FEPO_STORE_MATCHp, ppu_reg.WODU_RENDER_DONEp(), clk_reg.MYVO_AxCxExGx());
    /*p24.SOCY*/ wire _SOCY_WIN_HITn = not(win_reg.TOMU_WIN_HITp());
    /*p24.TYFA*/ wire _TYFA_CLKPIPEp_xBxDxFxH = and (_SOCY_WIN_HITn, tile_fetcher.POKY_PORCH_DONEp(), _VYBO_PIX_CLK_xBxDxFxH);
    return not(_TYFA_CLKPIPEp_xBxDxFxH);
  }

  /*p24.SACU*/ wire SACU_CLKPIPEp() const { return nor(SEGU_CLKPIPEn(), ppu_reg.ROXY_FINE_MATCH_LATCHn.q()); }

  /*p21.VOTY*/ wire VOTY_INT_STATp() const {
    /*p21.TARU*/ wire TARU_INT_HBL = and (lcd_reg.TOLU_VBLANKn(), ppu_reg.WODU_RENDER_DONEp());
    /*p21.SUKO*/ wire SUKO_INT_STATb = amux4(ppu_reg.RUGU_INT_LYC_EN.q(), lcd_reg.ROPO_LY_MATCH_SYNCp(),
                                             ppu_reg.REFE_INT_OAM_EN.q(), lcd_reg.TAPA_INT_OAM(),
                                             ppu_reg.RUFO_INT_VBL_EN.q(), lcd_reg.PARU_VBLANKp_d4(), // polarity?
                                             ppu_reg.ROXE_INT_HBL_EN.q(), TARU_INT_HBL);
    /*p21.TUVA*/ wire TUVA_INT_STATn = not(SUKO_INT_STATb);
    return not(TUVA_INT_STATn);
  }

  // And this is the topmost "reset sprite fetcher" signal
  /*p27.VEKU*/ wire VEKU_SFETCH_RUNNING_RSTn() const {
    return nor(sprite_fetcher.WUTY_SPRITE_DONEp(), tile_fetcher.TAVE_PORCH_DONE_TRIGp()); // def nor
  }

  /*p29.TEXY*/ wire TEXY_SPR_READ_VRAMp() const {
    /*p29.TYSO*/ wire TYSO_SPRITE_READn = or(sprite_fetcher.SAKY(), ppu_reg.TEPA_RENDERINGn());
    return not(TYSO_SPRITE_READn);
  }

  //-----------------------------------------------------------------------------
  // Internal state for debugging

  int phase_counter = -17;

  //-----------------------------------------------------------------------------
  // Sys pins

#if 0
  if (RST) {
    // This can't be run mode, TAPE doesn't affect UKUP
    ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, or(T1, T2), APUK_xxCDEFxx.q());
    UKUP_DIV_00.set(BOGA_xBCDEFGH, 0, UKUP_DIV_00.qn());
  }
  else {
    if (CLKIN_A) {
      // So this must be run mode, and ADYK must have _negative_ reset
      // RST = 0
      // CLKIN_A = 1
      ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, 1, APUK_xxCDEFxx.q());
      UKUP_DIV_00.set(BOGA_xBCDEFGH, !TAPE, UKUP_DIV_00.qn());  
    }
    else {
      // This also can't be run mode
      ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, 1, APUK_xxCDEFxx.q());
      UKUP_DIV_00.set(BOGA_xBCDEFGH, 0, UKUP_DIV_00.qn());  
    }
  }
#endif

  ExtPinIn  SYS_PIN_RSTp;   // PIN_71 -> UPOJ, UPYF, AFAR, ASOL, UFOL
  ExtPinIn  SYS_PIN_T2n;    // PIN_76, tied to 0 on board - but there's probably an implicit inverter
  ExtPinIn  SYS_PIN_T1n;    // PIN_77, tied to 0 on board - but there's probably an implicit inverter

  // Ground/vcc ties - VYPO, RUNY, WEFE, unlabeled cell between BONE and BUFY.

  wire GND = 0;
  wire VYPO_GND = 0;
  wire RUNY_GND = 0;
  wire PESU_GND = 0;

  wire WEFE_VCC = 1;

  //-----------------------------------------------------------------------------
  // LCD pins

  ExtPinOut LCD_PIN_LD1;  // PIN_50 
  ExtPinOut LCD_PIN_LD0;  // PIN_51 
  ExtPinOut LCD_PIN_CPG;  // PIN_52 
  ExtPinOut LCD_PIN_CP;   // PIN_53 
  ExtPinOut LCD_PIN_ST;   // PIN_54 
  ExtPinOut LCD_PIN_CPL;  // PIN_55 
  ExtPinOut LCD_PIN_FR;   // PIN_56 
  ExtPinOut LCD_PIN_S;    // PIN_57 

  //-----------------------------------------------------------------------------
  // Joypad pins

  ExtPinOut JOY_PIN_P10_A;   // PIN_67 <- P05.KOLE
  ExtPinOut JOY_PIN_P10_B;   // PIN_67 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P10_C;   // PIN_67 -> P02.KERY, P05.KEVU
  ExtPinOut JOY_PIN_P10_D;   // PIN_67 <- P05.KYBU

  ExtPinOut JOY_PIN_P11_A;   // PIN_66 <- P05.KYTO
  ExtPinOut JOY_PIN_P11_B;   // PIN_66 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P11_C;   // PIN_66 -> P02.KERY, P05.KAPA
  ExtPinOut JOY_PIN_P11_D;   // PIN_66 <- P05.KABU

  ExtPinOut JOY_PIN_P12_A;   // PIN_65 <- P05.KYHU
  ExtPinOut JOY_PIN_P12_B;   // PIN_65 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P12_C;   // PIN_65 -> P02.KERY, P05.KEJA
  ExtPinOut JOY_PIN_P12_D;   // PIN_65 <- P05.KASY

  ExtPinOut JOY_PIN_P13_A;   // PIN_64 <- P05.KORY
  ExtPinOut JOY_PIN_P13_B;   // PIN_64 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P13_C;   // PIN_64 -> P02.KERY, P05.KOLO
  ExtPinOut JOY_PIN_P13_D;   // PIN_64 <- P05.KALE

  ExtPinOut JOY_PIN_P14_A;   // PIN_63 <- p05.KARU
  ExtPinOut JOY_PIN_P14_D;   // PIN_63 <- p05.KELY

  ExtPinOut JOY_PIN_P15_A;   // PIN_62 <- p05.CELA
  ExtPinOut JOY_PIN_P15_D;   // PIN_62 <- p05.COFY

  //-----------------------------------------------------------------------------
  // Top level registers

  // FF40 - LCDC (in top because it's used everywhere)
  /*p23.VYXE*/ Reg9 VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg9 XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg9 XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg9 XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg9 WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg9 WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg9 WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg9 XONA_LCDC_EN;

  //-----------------------------------------------------------------------------
  // Sub-modules

//private:

  OamBus oam_bus;
  ExtBus ext_bus;
  IntBus int_bus;
  VramBus vram_bus;

  ClockRegisters clk_reg;
  DebugRegisters dbg_reg;
  DmaRegisters dma_reg;
  InterruptRegisters int_reg;
  Joypad joypad;
  LcdRegisters lcd_reg;
  PixelPipe pix_pipe;
  ResetRegisters rst_reg;
  SerialRegisters ser_reg;
  SpriteStore sprite_store;
  Timer tim_reg;
  PpuRegisters ppu_reg;
  WindowRegisters win_reg;
  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;
  BusMux bus_mux;
  Bootrom bootrom;
};

//-----------------------------------------------------------------------------

};