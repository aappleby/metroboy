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

  void set_cpu_bus(Req req);
  void set_buttons(uint8_t buttons);
  void set_vram_bus(uint8_t data);

  int get_addr() const {
    return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03,
                CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
                CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11,
                CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
  }

  SignalHash tick();

  /*p25.COTA*/ wire COTA_OAM_CLKn() const {
    /*p25.AVER*/ wire _AVER_SCAN_OAM_CLK = nand(top.ACYL_SCANNINGp(), top.clk_reg.XYSO_ABCxDEFx()); 
    /*p25.CUFE*/ wire _CUFE_DMA_OAM_CLK  = and (or (top.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp()), top.clk_reg.MOPA_xxxxEFGH());
    /*p25.BYCU*/ wire _BYCU_OAM_CLK      = nand(_AVER_SCAN_OAM_CLK, top.sprite_fetcher.XUJY_PPU_OAM_CLK(), _CUFE_DMA_OAM_CLK);
    return not(_BYCU_OAM_CLK);
  }

  /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn() const {
     return nor(top.dma_reg.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), top.AJON_OAM_BUSY()); // def nor
  }

  /*p28.AMAB*/ wire AMAB_OAM_LOCKn() const {
    return and (top.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn()); // def and
  }

  /*p04.WYJA*/ wire WYJA_OAM_WRp() const { // -> oam_bus
    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp = nor(top.clk_reg.UVYT_ABCDxxxx(), MAKA_FROM_CPU5_SYNC.q()); // def nor
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp = and (top.dma_reg.MATU_DMA_RUNNINGp(), _NAXY_DMA_OAM_WENp); // def and
    return or (and (AMAB_OAM_LOCKn(), top.CUPA_CPU_WRp_xxxxEFGx()), _POWU_DMA_OAM_WRp);
  }

  /*p28.BODE*/ wire BODE_OAM_LATCHp() const { // -> oam_bus
    /*p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), top.clk_reg.XOCE_AxxDExxH()); // schematic wrong, is def nand
    /*p28.BOTA*/ wire _BOTA_CPU_OAM_LATCHn  = nand(top.DECY_FROM_CPU5n(), top.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND
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

  /*p08.LAVO*/ wire LAVO_LATCH_CPU_DATAn() const { return nand(top.CPU_PIN_RDp, top.TEXO_8000_9FFFn(), top.CPU_PIN5); }

  /*p08.MATE*/ wire MATE_LATCH_CPU_ADDRp() const {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n = not(top.UMUT_MODE_DBG1p());
    /*p08.LOXO*/ wire _LOXO_LATCH_CPU_ADDRp = or (and (_MULE_MODE_DBG1n, top.TEXO_8000_9FFFn()), top.UMUT_MODE_DBG1p());
    /*p08.LASY*/ wire _LASY_LATCH_CPU_ADDRn = not(_LOXO_LATCH_CPU_ADDRp);
    return not(_LASY_LATCH_CPU_ADDRn);
  }

  /*p08.MOCA*/ wire MOCA_DBG_EXT_RD() const { return nor(top.TEXO_8000_9FFFn(), top.UMUT_MODE_DBG1p()); }

  /*p08.MOTY*/ wire MOTY_CPU_EXT_RD() const {
    /*p08.LAGU*/ wire _LAGU = or(and(top.CPU_PIN_RDp, top.LEVO_8000_9FFFp()), top.CPU_PIN_WRp);
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

    /*p08.SOBY*/ wire _SOBY_A15n = nor(top.CPU_PIN_A15, top.TUTU_ADDR_BOOTp());
    /*p08.SEPY*/ wire _SEPY_A15p = nand(top.ABUZ_CPU_ADDR_VALIDp(), _SOBY_A15n);
    return mux2_p(top.dma_reg.DMA_A15.q(), _SEPY_A15p, top.dma_reg.LUMA_DMA_READ_CARTp());
  }

  /*p08.TOZA*/ wire TOZA_CPU_RAM_RDp() const { 
    /*p08.SOGY*/ wire _SOGY_A14n = not(top.CPU_PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(top.CPU_PIN_A13, _SOGY_A14n, top.CPU_PIN_A15);
    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = or(and(top.CPU_PIN_A15, top.CPU_PIN_A14), _TUMA_CART_RAM);
    return and(top.ABUZ_CPU_ADDR_VALIDp(), _TYNU_ADDR_RAM, top.TUNA_0000_FDFFp()); // suggests ABUZp
  }


  //-----------------------------------------------------------------------------

  void tick_vram_pins();

  /*p01.ABUZ*/ wire ABUZ_CPU_ADDR_VALIDp() const {
    /*p??.APAP*/ wire APAP_CPU_ADDR_VALIDp = not(CPU_PIN_ADDR_VALID); // Missing from schematic
    /*p01.AWOD*/ wire AWOD_CPU_ADDR_VALIDn = nor(UNOR_MODE_DBG2p(), APAP_CPU_ADDR_VALIDp);
    return not(AWOD_CPU_ADDR_VALIDn);
  }

  /*p07.TUTU*/ wire TUTU_ADDR_BOOTp() const {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not(bootrom.BOOT_BITn());
    /*p07.TULO*/ wire TULO_ADDR_00XXp = nor(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09, CPU_PIN_A08);
    return and(TERA_BOOT_BITp, TULO_ADDR_00XXp);
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
      /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(ext_bus.EXT_PIN_RDp_C(), CPU_PIN_RDp, UNOR_MODE_DBG2p());
      return not(UJYV_CPU_RDn);
  }

  /*p07.AJAS*/ wire AJAS_CPU_RDn() const { return not(TEDO_CPU_RDp()); }
  /*p07.ASOT*/ wire ASOT_CPU_RDp() const { return not(AJAS_CPU_RDn()); }

  /*p04.DECY*/ wire DECY_FROM_CPU5n() const { return not(CPU_PIN5); }

  /*p28.LEKO*/ wire LEKO_CPU_RDp() const {
    /*p04.CATY*/ wire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n());
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RDp(), CATY_FROM_CPU5p);
    return not(MYNU_CPU_RDn);
  }

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH() const { return nand(CPU_PIN_WRp, clk_reg.AFAS_xxxxEFGx()); }
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx() const { return not(AREV_CPU_WRn_ABCDxxxH()); }
  /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH() const { return mux2_n(ext_bus.EXT_PIN_WRp_C(), APOV_CPU_WRp_xxxxEFGx(), UNOR_MODE_DBG2p()); }
  /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx() const { return not(UBAL_CPU_WRn_ABCDxxxH()); }
  /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH() const { return not(TAPU_CPU_WRp_xxxxEFGx()); }
  /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx() const { return not(DYKY_CPU_WRn_ABCDxxxH()); }
  /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH() const { return not(APOV_CPU_WRp_xxxxEFGx()); }


  //-----------------------------------------------------------------------------
  // Address decoder signals

  /*p25.SYRO*/ wire SYRO_FE00_FFFFp() const { return not(TUNA_0000_FDFFp()); }
  /*p07.RYCU*/ wire RYCU_0000_FDFFn() const { return not(TUNA_0000_FDFFp()); }
  /*p07.SOHA*/ wire SOHA_FF00_FFFFn() const { return not(SYKE_FF00_FFFFp()); }
  /*p07.ROPE*/ wire ROPE_FE00_FEFFn() const { return nand(RYCU_0000_FDFFn(), SOHA_FF00_FFFFn()); }
  /*p07.SARO*/ wire SARO_FE00_FEFFp() const { return not(ROPE_FE00_FEFFn()); }
  /*p28.ADAH*/ wire ADAH_FE00_FEFFn() const { return not(SARO_FE00_FEFFp()); }
  /*p08.SORE*/ wire SORE_0000_7FFFp() const { return not(CPU_PIN_A15); }
  /*p08.TEVY*/ wire TEVY_8000_9FFFn() const { return or(CPU_PIN_A13, CPU_PIN_A14, SORE_0000_7FFFp()); }
  /*p08.TEXO*/ wire TEXO_8000_9FFFn() const { return and(CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn()); }
  /*p08.LEVO*/ wire LEVO_8000_9FFFp() const { return not(TEXO_8000_9FFFn()); }

  /*p22.XOLA*/ wire XOLA_A00n() const { return not(CPU_PIN_A00); }
  /*p22.XENO*/ wire XENO_A01n() const { return not(CPU_PIN_A01); }
  /*p22.XUSY*/ wire XUSY_A02n() const { return not(CPU_PIN_A02); }
  /*p22.XERA*/ wire XERA_A03n() const { return not(CPU_PIN_A03); }
  /*p07.TONA*/ wire TONA_A08n() const { return not(CPU_PIN_A08); }

  /*p22.WADO*/ wire WADO_A00p() const { return not(XOLA_A00n()); }
  /*p22.WESA*/ wire WESA_A01p() const { return not(XENO_A01n()); }
  /*p22.WALO*/ wire WALO_A02p() const { return not(XUSY_A02n()); }
  /*p22.WEPO*/ wire WEPO_A03p() const { return not(XERA_A03n()); }

  /*p03.TOVY*/ wire TOVY_A00n() const { return not(CPU_PIN_A00); }
  /*p08.TOLA*/ wire TOLA_A01n() const { return not(CPU_PIN_A01); }
  /*p06.SEFY*/ wire SEFY_A02n() const { return not(CPU_PIN_A02); }

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const { return nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09); }

  /*p22.WERO*/ wire WERO_FF4Xp() const {
    /*p22.XALY*/ wire XALY_0x00xxxxp  = nor(CPU_PIN_A07, CPU_PIN_A05, CPU_PIN_A04);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp(), TONA_A08n());
    /*p22.WUTU*/ wire WUTU_FF4Xn = nand(SYKE_FF00_FFFFp, CPU_PIN_A06, XALY_0x00xxxxp);
    return not(WUTU_FF4Xn);
  }

  /*p07.SYKE*/ wire SYKE_FF00_FFFFp() const { return nor(TUNA_0000_FDFFp(), TONA_A08n()); }
  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor(CPU_PIN_A07, CPU_PIN_A06, CPU_PIN_A05, CPU_PIN_A04, CPU_PIN_A03); }

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
  // SOC-to-CPU control signals

  CpuPinOut CPU_PIN_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related reg
  CpuPinOut CPU_PIN_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  CpuPinOut CPU_PIN_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  CpuPinOut CPU_PIN_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  CpuPinOut CPU_PIN_ADDR_HI;       // top right port PORTA_03: <- P25.SYRO // Not really sure why this is here

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  CpuPinIn  CPU_PIN6;              // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  CpuPinIn  CPU_PIN5;              // top left port PORTD_06: -> ANUJ (FROM_CPU5). Probably "DATA_VALIDn"

  /*p04.MAKA*/ Reg17 MAKA_FROM_CPU5_SYNC;

  // Main bus
  CpuPinIn  CPU_PIN_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  CpuPinIn  CPU_PIN_WRp;           // top right port PORTA_01: ->
  CpuPinIn  CPU_PIN_ADDR_VALID;    // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.

  CpuPinIn  CPU_PIN_A00;           // bottom right port PORTB_00: -> A00
  CpuPinIn  CPU_PIN_A01;           // bottom right port PORTB_04: -> A01
  CpuPinIn  CPU_PIN_A02;           // bottom right port PORTB_08: -> A02
  CpuPinIn  CPU_PIN_A03;           // bottom right port PORTB_12: -> A03
  CpuPinIn  CPU_PIN_A04;           // bottom right port PORTB_16: -> A04
  CpuPinIn  CPU_PIN_A05;           // bottom right port PORTB_20: -> A05
  CpuPinIn  CPU_PIN_A06;           // bottom right port PORTB_24: -> A06
  CpuPinIn  CPU_PIN_A07;           // bottom right port PORTB_28: -> A07
  CpuPinIn  CPU_PIN_A08;           // bottom right port PORTB_02: -> A08
  CpuPinIn  CPU_PIN_A09;           // bottom right port PORTB_06: -> A09
  CpuPinIn  CPU_PIN_A10;           // bottom right port PORTB_10: -> A10
  CpuPinIn  CPU_PIN_A11;           // bottom right port PORTB_14: -> A11
  CpuPinIn  CPU_PIN_A12;           // bottom right port PORTB_18: -> A12
  CpuPinIn  CPU_PIN_A13;           // bottom right port PORTB_22: -> A13
  CpuPinIn  CPU_PIN_A14;           // bottom right port PORTB_26: -> A14
  CpuPinIn  CPU_PIN_A15;           // bottom right port PORTB_30: -> A15
                                
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
  // VRAM bus

  ExtPinOut VRAM_PIN_MCSn_A;   // PIN_43 <- P25.SOKY
  ExtPinOut VRAM_PIN_MCSn_D;   // PIN_43 <- P25.SETY
  ExtPinOut VRAM_PIN_MOEn_A;   // PIN_45 <- P25.REFO
  ExtPinOut VRAM_PIN_MOEn_D;   // PIN_45 <- P25.SAHA
  ExtPinOut VRAM_PIN_MWRn_A;   // PIN_49 <- P25.SYSY
  ExtPinOut VRAM_PIN_MWRn_D;   // PIN_49 <- P25.RAGU

  ExtPinOut VRAM_PIN_MA00_AD;  // PIN_34 <- P04.ECAL
  ExtPinOut VRAM_PIN_MA01_AD;  // PIN_35 <- P04.EGEZ
  ExtPinOut VRAM_PIN_MA02_AD;  // PIN_36 <- P04.FUHE
  ExtPinOut VRAM_PIN_MA03_AD;  // PIN_37 <- P04.FYZY
  ExtPinOut VRAM_PIN_MA04_AD;  // PIN_38 <- P04.DAMU
  ExtPinOut VRAM_PIN_MA05_AD;  // PIN_39 <- P04.DAVA
  ExtPinOut VRAM_PIN_MA06_AD;  // PIN_40 <- P04.ETEG
  ExtPinOut VRAM_PIN_MA07_AD;  // PIN_41 <- P04.EREW
  ExtPinOut VRAM_PIN_MA08_AD;  // PIN_48 <- P04.EVAX
  ExtPinOut VRAM_PIN_MA09_AD;  // PIN_47 <- P04.DUVE
  ExtPinOut VRAM_PIN_MA10_AD;  // PIN_44 <- P04.ERAF
  ExtPinOut VRAM_PIN_MA11_AD;  // PIN_46 <- P04.FUSY
  ExtPinOut VRAM_PIN_MA12_AD;  // PIN_42 <- P04.EXYF

  ExtPinOut VRAM_PIN_MD0_A;    // PIN_33 <- P25.REGE
  ExtPinOut VRAM_PIN_MD1_A;    // PIN_31 <- P25.RYKY
  ExtPinOut VRAM_PIN_MD2_A;    // PIN_30 <- P25.RAZO
  ExtPinOut VRAM_PIN_MD3_A;    // PIN_29 <- P25.RADA
  ExtPinOut VRAM_PIN_MD4_A;    // PIN_28 <- P25.RYRO
  ExtPinOut VRAM_PIN_MD5_A;    // PIN_27 <- P25.REVU
  ExtPinOut VRAM_PIN_MD6_A;    // PIN_26 <- P25.REKU
  ExtPinOut VRAM_PIN_MD7_A;    // PIN_25 <- P25.RYZE

  ExtPinOut VRAM_PIN_MD0_B;    // PIN_33 <- P25.ROFA
  ExtPinOut VRAM_PIN_MD1_B;    // PIN_31 <- P25.ROFA
  ExtPinOut VRAM_PIN_MD2_B;    // PIN_30 <- P25.ROFA
  ExtPinOut VRAM_PIN_MD3_B;    // PIN_29 <- P25.ROFA
  ExtPinOut VRAM_PIN_MD4_B;    // PIN_28 <- P25.ROFA
  ExtPinOut VRAM_PIN_MD5_B;    // PIN_27 <- P25.ROFA
  ExtPinOut VRAM_PIN_MD6_B;    // PIN_26 <- P25.ROFA
  ExtPinOut VRAM_PIN_MD7_B;    // PIN_25 <- P25.ROFA

  ExtPinOut VRAM_PIN_MD0_D;    // PIN_33 <- P25.RURA
  ExtPinOut VRAM_PIN_MD1_D;    // PIN_31 <- P25.RULY
  ExtPinOut VRAM_PIN_MD2_D;    // PIN_30 <- P25.RARE
  ExtPinOut VRAM_PIN_MD3_D;    // PIN_29 <- P25.RODU
  ExtPinOut VRAM_PIN_MD4_D;    // PIN_28 <- P25.RUBE
  ExtPinOut VRAM_PIN_MD5_D;    // PIN_27 <- P25.RUMU
  ExtPinOut VRAM_PIN_MD6_D;    // PIN_26 <- P25.RYTY
  ExtPinOut VRAM_PIN_MD7_D;    // PIN_25 <- P25.RADY

  ExtPinIn  VRAM_PIN_MCSn_C;   // PIN_43 -> P25.TEFY
  ExtPinIn  VRAM_PIN_MOEn_C;   // PIN_45 -> P25.TAVY
  ExtPinIn  VRAM_PIN_MWRn_C;   // PIN_49 -> P25.SUDO

  ExtPinIn  VRAM_PIN_MD0_C;    // PIN_33 -> P25.RODY
  ExtPinIn  VRAM_PIN_MD1_C;    // PIN_31 -> P25.REBA
  ExtPinIn  VRAM_PIN_MD2_C;    // PIN_30 -> P25.RYDO
  ExtPinIn  VRAM_PIN_MD3_C;    // PIN_29 -> P25.REMO
  ExtPinIn  VRAM_PIN_MD4_C;    // PIN_28 -> P25.ROCE
  ExtPinIn  VRAM_PIN_MD5_C;    // PIN_27 -> P25.ROPU
  ExtPinIn  VRAM_PIN_MD6_C;    // PIN_26 -> P25.RETA
  ExtPinIn  VRAM_PIN_MD7_C;    // PIN_25 -> P25.RAKU

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