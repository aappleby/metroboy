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

#include "Sch_CpuBus.h"
#include "Sch_BusMux.h"
#include "Sch_Interrupts.h"
#include "Sch_Bootrom.h"
#include "Sch_SpriteFetcher.h"
#include "Sch_TileFetcher.h"
#include "Sch_SpriteScanner.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SchematicTop {

  void tick_everything();

  void tick_vram_pins();
  void tick_top_regs();

  SignalHash commit_everything();

  SignalHash commit_ibus();
  SignalHash commit_vbus();
  SignalHash commit_ebus();
  SignalHash commit_obus();

  SignalHash commit_sys_pins();
  SignalHash commit_lcd_pins();
  SignalHash commit_joy_pins();

  SignalHash commit_top_regs();

  //-----------------------------------------------------------------------------
  // Debug signals

  wire UMUT_MODE_DBG1p() const;
  wire UNOR_MODE_DBG2p() const;
  wire UPOJ_MODE_PROD() const;
  wire TOVA_MODE_DBG2n() const;
  wire TUTO_DBG_VRAMp() const;
  wire ABUZ() const;
  void preset_t1t2(bool t1, bool t2);

  //-----------------------------------------------------------------------------
  // Clock signals

  wire AFUR_xBCDExxx() const;
  wire ALEF_xxCDEFxx() const;
  wire APUK_xxxDEFGx() const;
  wire ADYK_xxxxEFGH() const;

  wire WUVU_AxxDExxH() const;
  wire VENA_xBCDExxx() const;
  wire WOSU_xxCDxxGH() const;

  wire BYJU_AxCDEFGH() const;

  //-----------------------------------------------------------------------------
  // Timer signals

  wire UVYN_DIV_05n() const;
  wire UMEK_DIV_06n() const;
  wire UREK_DIV_07n() const;
  wire UPOF_DIV_15() const;
  wire MOBA_INT_TIMERp() const;

  //-----------------------------------------------------------------------------
  // Reset signals

  wire AVOR_RSTp() const;
  wire XAPO_VID_RSTn() const;

  //-----------------------------------------------------------------------------
  // CPU signals

  wire UJYV_CPU_RD() const;
  wire UBAL_CPU_WRp_ABCDExxx() const;

  int get_addr() const;
  void preset_addr(bool oe, uint16_t addr);
  int get_data() const;
  void set_data(bool oe, uint8_t data);
  void preset_rd(bool rd);
  void preset_wr(bool wr);
  void preset_addr_valid(bool valid);

  //-----------------------------------------------------------------------------
  // Bus mux signals

  /*p28.GEKA*/ wire GEKA_OAM_A0p() const;
  /*p28.ZYFO*/ wire ZYFO_OAM_A1p() const;
  /*p28.YFOT*/ wire YFOT_OAM_A2p() const;
  /*p28.YFOC*/ wire YFOC_OAM_A3p() const;
  /*p28.YVOM*/ wire YVOM_OAM_A4p() const;
  /*p28.YMEV*/ wire YMEV_OAM_A5p() const;
  /*p28.XEMU*/ wire XEMU_OAM_A6p() const;
  /*p28.YZET*/ wire YZET_OAM_A7p() const;

  /*p31.YLOR*/ wire YLOR_SPRITE_X0() const;
  /*p31.ZYTY*/ wire ZYTY_SPRITE_X1() const;
  /*p31.ZYVE*/ wire ZYVE_SPRITE_X2() const;
  /*p31.ZEZY*/ wire ZEZY_SPRITE_X3() const;
  /*p31.GOMO*/ wire GOMO_SPRITE_X4() const;
  /*p31.BAXO*/ wire BAXO_SPRITE_X5() const;
  /*p31.YZOS*/ wire YZOS_SPRITE_X6() const;
  /*p31.DEPO*/ wire DEPO_SPRITE_X7() const;

  /*p29.XUSO*/ wire XUSO_SPRITE_Y0() const;
  /*p29.XEGU*/ wire XEGU_SPRITE_Y1() const;
  /*p29.YJEX*/ wire YJEX_SPRITE_Y2() const;
  /*p29.XYJU*/ wire XYJU_SPRITE_Y3() const;
  /*p29.YBOG*/ wire YBOG_SPRITE_Y4() const;
  /*p29.WYSO*/ wire WYSO_SPRITE_Y5() const;
  /*p29.XOTE*/ wire XOTE_SPRITE_Y6() const;
  /*p29.YZAB*/ wire YZAB_SPRITE_Y7() const;

  //-----------------------------------------------------------------------------
  // DMA signals

  /*p04.MATU*/ wire MATU_DMA_RUNNINGp() const;
  /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp() const;

  /*p04.NAKY*/ wire DMA_A00() const;
  /*p04.PYRO*/ wire DMA_A01() const;
  /*p04.NEFY*/ wire DMA_A02() const;
  /*p04.MUTY*/ wire DMA_A03() const;
  /*p04.NYKO*/ wire DMA_A04() const;
  /*p04.PYLO*/ wire DMA_A05() const;
  /*p04.NUTO*/ wire DMA_A06() const;
  /*p04.MUGU*/ wire DMA_A07() const;
  /*p04.NAFA*/ wire DMA_A08() const;
  /*p04.PYNE*/ wire DMA_A09() const;
  /*p04.PARA*/ wire DMA_A10() const;
  /*p04.NYDO*/ wire DMA_A11() const;
  /*p04.NYGY*/ wire DMA_A12() const;
  /*p04.PULA*/ wire DMA_A13() const;
  /*p04.POKU*/ wire DMA_A14() const;
  /*p04.MARU*/ wire DMA_A15() const;

  //-----------------------------------------------------------------------------
  // LCD signals

  wire BYHA_VID_LINE_TRIG_d4n() const;

  wire RUTU_NEW_LINE_d0() const;
  wire CATU_VID_LINE_d4() const;
  wire POPU_VBLANK_d4() const;
  wire ROPO_LY_MATCH_SYNCp() const;
  wire MUWY_Y0() const;
  wire MYRO_Y1() const;
  wire LEXA_Y2() const;
  wire LYDO_Y3() const;
  wire LOVU_Y4() const;
  wire LEMA_Y5() const;
  wire MATO_Y6() const;
  wire LAFO_Y7() const;

  //-----------------------------------------------------------------------------
  // Address decoder signals

  wire WERO_FF40_FF4Fp() const;

  //-----------------------------------------------------------------------------
  // PPU signals

  wire XYMU_RENDERINGp() const;
  wire WODU_RENDER_DONEp() const;
  wire TEVO_FINE_RSTp() const;
  wire SERE_VRAM_RD() const;
  wire NYXU_TILE_FETCHER_RSTn() const;
  wire ACYL_SCANNINGp() const;
  wire LYRY_BFETCH_DONEp() const;
  wire LOZE_PIPE_A_LOAD() const;
  wire LUXA_PIPE_B_LOAD() const;
  wire SEGU_CLKPIPEn() const;
  wire SACU_CLKPIPEp() const;
  wire ROZE_FINE_COUNT_STOPn() const;
  wire VOTY_INT_STATp() const;
  wire TEKY_SPRITE_FETCH() const;
  wire VEKU_SFETCH_RUNNING_RSTn() const;

  wire XEHO_X0() const;
  wire SAVY_X1() const;
  wire XODU_X2() const;
  wire XYDO_X3() const;
  wire TUHU_X4() const;
  wire TUKY_X5() const;
  wire TAKO_X6() const;
  wire SYBE_X7() const;

  //-----------------------------------------------------------------------------
  // Tile fetcher signals

  wire POKY_PORCH_DONEp() const;
  wire PORY_FETCH_DONE_Bp() const;
  wire LONY_BG_READ_VRAM_LATCHp() const;
  wire TAVE_PORCH_DONE_TRIGp() const;

  wire DATY_SCX0() const;
  wire DUZU_SCX1() const;
  wire CYXU_SCX2() const;

  //-----------------------------------------------------------------------------
  // Sprite scanner signals

  /*p29.CEHA*/ wire CEHA_SCANNINGp() const { return not(sprite_scanner.CENO_SCANNINGp.qn()); }
  /*p28.BESU*/ wire BESU_SCANNINGp() const { return sprite_scanner.BESU_SCANNINGp; }
  /*p29.CENO*/ wire CENO_SCANNINGp() const { return sprite_scanner.CENO_SCANNINGp; }

  wire AVAP_SCAN_DONE_TRIGp() const {
    /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn());
    /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n());
    /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4p, ATAR_VID_RSTp);
    /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
    /*p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or(BALU_SCAN_RST, sprite_scanner.SCAN_DONE_TRIG_B.q(), !sprite_scanner.SCAN_DONE_TRIG_A.q());
    /*p29.AVAP*/ wire AVAP_SCAN_DONE_TRIGp = not(BEBU_SCAN_DONE_TRIGn);
    return AVAP_SCAN_DONE_TRIGp;
  }

  wire DEGE_SPRITE_DELTA0() const { return sprite_scanner.DEGE_SPRITE_DELTA0; }
  wire DABY_SPRITE_DELTA1() const { return sprite_scanner.DABY_SPRITE_DELTA1; }
  wire DABU_SPRITE_DELTA2() const { return sprite_scanner.DABU_SPRITE_DELTA2; }
  wire GYSA_SPRITE_DELTA3() const { return sprite_scanner.GYSA_SPRITE_DELTA3; }
  wire CARE_STORE_ENp_ABxxEFxx() const { return sprite_scanner.CARE_STORE_ENp_ABxxEFxx; }

  /*p28.YFEL*/ wire YFEL_SCAN0() const { return sprite_scanner.YFEL_SCAN0; }
  /*p28.WEWY*/ wire WEWY_SCAN1() const { return sprite_scanner.WEWY_SCAN1; }
  /*p28.GOSO*/ wire GOSO_SCAN2() const { return sprite_scanner.GOSO_SCAN2; }
  /*p28.ELYN*/ wire ELYN_SCAN3() const { return sprite_scanner.ELYN_SCAN3; }
  /*p28.FAHA*/ wire FAHA_SCAN4() const { return sprite_scanner.FAHA_SCAN4; }
  /*p28.FONY*/ wire FONY_SCAN5() const { return sprite_scanner.FONY_SCAN5; }

  //-----------------------------------------------------------------------------
  // Sprite store signals

  /*p29.FEPO*/ wire FEPO_STORE_MATCHp() const { return sst_reg.FEPO_STORE_MATCHp; }

  //-----------------------------------------------------------------------------
  // Sprite fetcher signals

  wire WEFY_SPR_READp() const {
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1, sprite_fetcher.TESE_SFETCH_S2);
    /*p28.WEFY*/ wire WEFY_SPR_READp = and(TUVO_PPU_OAM_RDp, sprite_fetcher.TYFO_SFETCH_S0_D1);
    return WEFY_SPR_READp;
  }

  wire VAPE_FETCH_OAM_CLK() const {
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1, sprite_fetcher.TESE_SFETCH_S2);
    /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(sprite_fetcher.TOXE_SFETCH_S0_D0);
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(sprite_fetcher.TYFO_SFETCH_S0_D1, TYTU_SFETCH_S0_D0n);
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and (TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
    return VAPE_FETCH_OAM_CLK;
  }

  wire VUSA_PIPE_LOAD_SPRITEn() const {
    // TYNO_01 << TOXE_17
    // TYNO_02 << SEBA_17
    // TYNO_03 << VONU_17
    // TYNO_04 >> VUSA_02

    // VUSA_01 << TYFO_16
    // VUSA_02 << TYNO_04
    // VUSA_03 nc
    // VUSA_04 >>

    /*p29.TYNO*/ wire TYNO = nand(sprite_fetcher.TOXE_SFETCH_S0_D0.q(), sprite_fetcher.SEBA_SFETCH_S1_D5.q(), sprite_fetcher.VONU_SFETCH_S1_D4.q());
    /*p29.VUSA*/ wire VUSA_PIPE_LOAD_SPRITEn = or(sprite_fetcher.TYFO_SFETCH_S0_D1.qn(), TYNO);
    return VUSA_PIPE_LOAD_SPRITEn;
  }

  wire SOWO_SFETCH_RUNNINGn() const {
    /*p27.SOWO*/ return not(sprite_fetcher.TAKA_SFETCH_RUNNINGp);
  }

  wire TEXY_SPRITE_READp() const {
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
    /*p29.SAKY*/ wire SAKY = nor(sprite_fetcher.TULY_SFETCH_S1.q(), sprite_fetcher.VONU_SFETCH_S1_D4.q());
    /*p29.TYSO*/ wire TYSO_SPRITE_READn = or(SAKY, TEPA_RENDERINGn);
    /*p29.TEXY*/ wire TEXY_SPRITE_READp = not(TYSO_SPRITE_READn);
    return TEXY_SPRITE_READp;
  }

  wire SOHO_SPR_VRAM_RDp() const {
    /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(sprite_fetcher.TOXE_SFETCH_S0_D0.q());
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(sprite_fetcher.TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0_D0n);
    /*p29.ABON*/ wire ABON_SPR_VRAM_RDp1 = not(TEXY_SPRITE_READp());
    /*p25.SOHO*/ wire SOHO_SPR_VRAM_RDp = and (TACU_SPR_SEQ_5_TRIG, ABON_SPR_VRAM_RDp1);
    return SOHO_SPR_VRAM_RDp;
  }


  /*p33.PEFO*/ wire SPR_PIX_A0() const { return sprite_fetcher.SPR_PIX_A0; }
  /*p33.ROKA*/ wire SPR_PIX_A1() const { return sprite_fetcher.SPR_PIX_A1; }
  /*p33.MYTU*/ wire SPR_PIX_A2() const { return sprite_fetcher.SPR_PIX_A2; }
  /*p33.RAMU*/ wire SPR_PIX_A3() const { return sprite_fetcher.SPR_PIX_A3; }
  /*p33.SELE*/ wire SPR_PIX_A4() const { return sprite_fetcher.SPR_PIX_A4; }
  /*p33.SUTO*/ wire SPR_PIX_A5() const { return sprite_fetcher.SPR_PIX_A5; }
  /*p33.RAMA*/ wire SPR_PIX_A6() const { return sprite_fetcher.SPR_PIX_A6; }
  /*p33.RYDU*/ wire SPR_PIX_A7() const { return sprite_fetcher.SPR_PIX_A7; }

  /*p33.REWO*/ wire SPR_PIX_B0() const { return sprite_fetcher.SPR_PIX_B0; }
  /*p33.PEBA*/ wire SPR_PIX_B1() const { return sprite_fetcher.SPR_PIX_B1; }
  /*p33.MOFO*/ wire SPR_PIX_B2() const { return sprite_fetcher.SPR_PIX_B2; }
  /*p33.PUDU*/ wire SPR_PIX_B3() const { return sprite_fetcher.SPR_PIX_B3; }
  /*p33.SAJA*/ wire SPR_PIX_B4() const { return sprite_fetcher.SPR_PIX_B4; }
  /*p33.SUNY*/ wire SPR_PIX_B5() const { return sprite_fetcher.SPR_PIX_B5; }
  /*p33.SEMO*/ wire SPR_PIX_B6() const { return sprite_fetcher.SPR_PIX_B6; }
  /*p33.SEGA*/ wire SPR_PIX_B7() const { return sprite_fetcher.SPR_PIX_B7; }


  //-----------------------------------------------------------------------------
  // Window signals

  /*p27.NOCU*/ wire NOCU_WIN_MODEn       () const { return not(win_reg.PYNU_WIN_MODE_TRIGA.q()); }
  /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp  () const { return and (win_reg.PYNU_WIN_MODE_TRIGA, !win_reg.NOPA_WIN_MODE_TRIGB); }
  /*p27.SEKO*/ wire SEKO_WIN_MATCH_TRIGp () const { return nor(!win_reg.RYFA_WIN_MATCH_TRIGA, win_reg.RENE_WIN_MATCH_TRIGB); }

  /*p27.SYLO*/ wire SYLO_WIN_HITn        () const { return not(win_reg.RYDY_WIN_HIT_LATCHp); }

  // FIXME this needs a name
  wire SUZU() const {
    /*p27.TUXY*/ wire TUXY = nand(win_reg.SOVY_WIN_HIT_SYNC, SYLO_WIN_HITn());
    /*p27.SUZU*/ wire SUZU = not(TUXY);
    return SUZU;
  }

  wire WIN_X3() const { return win_reg.WIN_X3; }
  wire WIN_X4() const { return win_reg.WIN_X4; }
  wire WIN_X5() const { return win_reg.WIN_X5; }
  wire WIN_X6() const { return win_reg.WIN_X6; }
  wire WIN_X7() const { return win_reg.WIN_X7; }

  wire WIN_Y0() const { return win_reg.WIN_Y0; }
  wire WIN_Y1() const { return win_reg.WIN_Y1; }
  wire WIN_Y2() const { return win_reg.WIN_Y2; }
  wire WIN_Y3() const { return win_reg.WIN_Y3; }
  wire WIN_Y4() const { return win_reg.WIN_Y4; }
  wire WIN_Y5() const { return win_reg.WIN_Y5; }
  wire WIN_Y6() const { return win_reg.WIN_Y6; }
  wire WIN_Y7() const { return win_reg.WIN_Y7; }

  //-----------------------------------------------------------------------------
  // Misc signals

  wire BOOT_BITn() const;    // Bootrom
  wire ASOK_INT_JOYPADp() const; // Joypad
  wire CALY_INT_SERIALp() const { return ser_reg.CALY_INT_SERIALp; }

  //-----------------------------------------------------------------------------

  int phase_counter = -32;

  // FF40 - LCDC (in top because it's used everywhere)
  /*p23.VYXE*/ Reg VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg XONA_LCDC_EN;

  /*p32.LEGU*/ Reg8 LEGU_BG_PIX_A0; // 8-rung
  /*p32.NUDU*/ Reg8 NUDU_BG_PIX_A1;
  /*p32.MUKU*/ Reg8 MUKU_BG_PIX_A2;
  /*p32.LUZO*/ Reg8 LUZO_BG_PIX_A3;
  /*p32.MEGU*/ Reg8 MEGU_BG_PIX_A4;
  /*p32.MYJY*/ Reg8 MYJY_BG_PIX_A5;
  /*p32.NASA*/ Reg8 NASA_BG_PIX_A6;
  /*p32.NEFO*/ Reg8 NEFO_BG_PIX_A7; // color wrong on die

  /*p32.RAWU*/ Reg RAWU_BG_PIX_B0; // 11-rung, also holds tile index during fetch
  /*p32.POZO*/ Reg POZO_BG_PIX_B1; // 11-rung
  /*p32.PYZO*/ Reg PYZO_BG_PIX_B2; 
  /*p32.POXA*/ Reg POXA_BG_PIX_B3; 
  /*p32.PULO*/ Reg PULO_BG_PIX_B4; 
  /*p32.POJU*/ Reg POJU_BG_PIX_B5; 
  /*p32.POWY*/ Reg POWY_BG_PIX_B6; 
  /*p32.PYJU*/ Reg PYJU_BG_PIX_B7;

  //----------

  PinOut CPU_PIN_WAKE;          // top right wire by itself <- P02.AWOB

  PinIn  CPU_PIN_RD;            // top right port PORTA_00: ->
  PinIn  CPU_PIN_WR;            // top right port PORTA_01: ->
  PinOut CPU_PIN_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut CPU_PIN_SYRO;          // top right port PORTA_03: <- P25.SYRO
  PinOut CPU_PIN_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut CPU_PIN_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  PinIn  CPU_PIN_ADDR_VALID;    // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.

  PinIn  CPU_PIN_ACK_VBLANK;    // bottom right port PORTB_01: ->        P02.LETY, vblank int ack
  PinOut CPU_PIN_INT_VBLANK;    // bottom right port PORTB_03: <-        P02.LOPE, vblank int
  PinIn  CPU_PIN_ACK_STAT;      // bottom right port PORTB_05: ->        P02.LEJA, stat int ack
  PinOut CPU_PIN_INT_STAT;      // bottom right port PORTB_07: <-        P02.LALU, stat int
  PinIn  CPU_PIN_ACK_TIMER;     // bottom right port PORTB_09: ->        P02.LESA, timer int ack
  PinOut CPU_PIN_INT_TIMER;     // bottom right port PORTB_11: <-        P02.NYBO, timer int
  PinIn  CPU_PIN_ACK_SERIAL;    // bottom right port PORTB_13: ->        P02.LUFE, serial int ack
  PinOut CPU_PIN_INT_SERIAL;    // bottom right port PORTB_15: <-        P02.UBUL, serial int
  PinIn  CPU_PIN_ACK_JOYPAD;    // bottom right port PORTB_17: ->        P02.LAMO, joypad int ack
  PinOut CPU_PIN_INT_JOYPAD;    // bottom right port PORTB_19: <-        P02.ULAK, joypad int

  PinIn  CPU_PIN_CLKREQ;        // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  PinOut CPU_PIN_AFER_RSTp;     // top center port PORTC_01: <- P01.AFER , reset related reg
  PinOut CPU_PIN_EXT_RESET;     // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut CPU_PIN_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut CPU_PIN_TABA_RSTp;     // top center port PORTC_04: <- P01.CPU_RESET

  PinIn  CPU_PIN6;              // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  PinOut CPU_PIN_BOWA_AxCDEFGH; // top left port PORTD_01: <- BOWA_AxCDEFGH // Blue clock - decoders, alu, some reset stuff
  PinOut CPU_PIN_BEDO_xBxxxxxx; // top left port PORTD_02: <- BEDO_xBxxxxxx
  PinOut CPU_PIN_BEKO_xBCDExxx; // top left port PORTD_03: <- BEKO_ABCDxxxx + BAVY connection not indicated on P01 - test pad 1
  PinOut CPU_PIN_BUDE_AxxxxFGH; // top left port PORTD_04: <- BUDE_AxxxxFGH + BEVA
  PinOut CPU_PIN_BOLO_xBCDEFGx; // top left port PORTD_05: <- BOLO_ABCDEFxx + BYDA? - test pad 2
  PinIn  CPU_PIN5;              // top left port PORTD_06: -> ANUJ (FROM_CPU5). Maybe this means "latch the bus"?
  PinOut CPU_PIN_BUKE_ABxxxxxH; // top left port PORTD_07: <- BUKE_ABxxxxxH
  PinOut CPU_PIN_BOMA_xBxxxxxx; // top left port PORTD_08: <- BOMA_xBxxxxxx (RESET_CLK)
  PinOut CPU_PIN_BOGA_AxCDEFGH; // top left port PORTD_09: <- BOGA_AxCDEFGH - test pad 3

  PinIn  CPU_PIN_A00;           // bottom right port PORTB_00: -> A00
  PinIn  CPU_PIN_A01;           // bottom right port PORTB_04: -> A01
  PinIn  CPU_PIN_A02;           // bottom right port PORTB_08: -> A02
  PinIn  CPU_PIN_A03;           // bottom right port PORTB_12: -> A03
  PinIn  CPU_PIN_A04;           // bottom right port PORTB_16: -> A04
  PinIn  CPU_PIN_A05;           // bottom right port PORTB_20: -> A05
  PinIn  CPU_PIN_A06;           // bottom right port PORTB_24: -> A06
  PinIn  CPU_PIN_A07;           // bottom right port PORTB_28: -> A07
  PinIn  CPU_PIN_A08;           // bottom right port PORTB_02: -> A08
  PinIn  CPU_PIN_A09;           // bottom right port PORTB_06: -> A09
  PinIn  CPU_PIN_A10;           // bottom right port PORTB_10: -> A10
  PinIn  CPU_PIN_A11;           // bottom right port PORTB_14: -> A11
  PinIn  CPU_PIN_A12;           // bottom right port PORTB_18: -> A12
  PinIn  CPU_PIN_A13;           // bottom right port PORTB_22: -> A13
  PinIn  CPU_PIN_A14;           // bottom right port PORTB_26: -> A14
  PinIn  CPU_PIN_A15;           // bottom right port PORTB_30: -> A15
                                
  Tribuf CPU_TRI_D0;            // bottom left port: <>
  Tribuf CPU_TRI_D1;            // bottom left port: <>
  Tribuf CPU_TRI_D2;            // bottom left port: <>
  Tribuf CPU_TRI_D3;            // bottom left port: <>
  Tribuf CPU_TRI_D4;            // bottom left port: <>
  Tribuf CPU_TRI_D5;            // bottom left port: <>
  Tribuf CPU_TRI_D6;            // bottom left port: <>
  Tribuf CPU_TRI_D7;            // bottom left port: <>

  //----------
  // OAM pins

  PinOut OAM_PIN_CLK;
  PinOut OAM_PIN_OE;
  PinOut OAM_PIN_WR_A;
  PinOut OAM_PIN_WR_B;

  PinOut OAM_PIN_A0;
  PinOut OAM_PIN_A1;
  PinOut OAM_PIN_A2;
  PinOut OAM_PIN_A3;
  PinOut OAM_PIN_A4;
  PinOut OAM_PIN_A5;
  PinOut OAM_PIN_A6;
  PinOut OAM_PIN_A7;

  Tribuf OAM_PIN_DA0;
  Tribuf OAM_PIN_DA1;
  Tribuf OAM_PIN_DA2;
  Tribuf OAM_PIN_DA3;
  Tribuf OAM_PIN_DA4;
  Tribuf OAM_PIN_DA5;
  Tribuf OAM_PIN_DA6;
  Tribuf OAM_PIN_DA7;

  Tribuf OAM_PIN_DB0;
  Tribuf OAM_PIN_DB1;
  Tribuf OAM_PIN_DB2;
  Tribuf OAM_PIN_DB3;
  Tribuf OAM_PIN_DB4;
  Tribuf OAM_PIN_DB5;
  Tribuf OAM_PIN_DB6;
  Tribuf OAM_PIN_DB7;

  //----------
  // LCD pins

  /* PIN_50 */ PinOut LCD_PIN_LD1;
  /* PIN_51 */ PinOut LCD_PIN_LD0;
  /* PIN_52 */ PinOut LCD_PIN_CPG;
  /* PIN_53 */ PinOut LCD_PIN_CP;
  /* PIN_54 */ PinOut LCD_PIN_ST;
  /* PIN_55 */ PinOut LCD_PIN_CPL;
  /* PIN_56 */ PinOut LCD_PIN_FR;
  /* PIN_57 */ PinOut LCD_PIN_S;

  //----------
  // Sys pins

  /* PIN_71 */ PinIn  SYS_PIN_RST;
  /* PIN_74 */ PinIn  SYS_PIN_CLK_xBxDxFxH;
  /* PIN_74 */ PinIn  SYS_PIN_CLK_GOOD;
  /* PIN_76 */ PinIn  SYS_PIN_T2;
  /* PIN_77 */ PinIn  SYS_PIN_T1;

  //----------
  // Joypad pins

  // The B connections on the joypad pins are werid.
  // They seem to be used as an input, or at least I can't find the driver
  // PESU
  // RARU ROWE RYKE RYNE RASE REJY REKA ROMY
  // RUNY VYPO TOMY? SEZU? RAWU? PUTE? MYDE RUGO? NYLU WYMO?
  // WEFE WUWE GEFY WYGA? FABY ECAB? DYSO ERUC GEZE GUVA 
  // ARAR ATAJ ASUZ AJEC AKAJ ANOC BENU BEDA
  // BEKU

  /* PIN_67 */ PinOut JOY_PIN_P10_A;   // <- P05.KOLE
  /* PIN_67 */ PinOut JOY_PIN_P10_B;   // -> BENU BEDA ATAJ ASUZ AJEC AKAJ ANOC ARAR
  /* PIN_67 */ PinIn  JOY_PIN_P10_C;   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ PinOut JOY_PIN_P10_D;   // <- P05.KYBU

  /* PIN_66 */ PinOut JOY_PIN_P11_A;   // <- P05.KYTO
  /* PIN_66 */ PinOut JOY_PIN_P11_B;   
  /* PIN_66 */ PinIn  JOY_PIN_P11_C;   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ PinOut JOY_PIN_P11_D;   // <- P05.KABU

  /* PIN_65 */ PinOut JOY_PIN_P12_A;   // <- P05.KYHU
  /* PIN_65 */ PinOut JOY_PIN_P12_B;   
  /* PIN_65 */ PinIn  JOY_PIN_P12_C;   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ PinOut JOY_PIN_P12_D;   // <- P05.KASY

  /* PIN_64 */ PinOut JOY_PIN_P13_A;   // <- P05.KORY
  /* PIN_64 */ PinOut JOY_PIN_P13_B;   
  /* PIN_64 */ PinIn  JOY_PIN_P13_C;   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ PinOut JOY_PIN_P13_D;   // <- P05.KALE

  /* PIN_63 */ PinOut JOY_PIN_P14_A;   // <- p05.KARU
  /* PIN_63 */ PinOut JOY_PIN_P14_D;   // <- p05.KELY

  /* PIN_62 */ PinOut JOY_PIN_P15_A;   // <- p05.CELA
  /* PIN_62 */ PinOut JOY_PIN_P15_D;   // <- p05.COFY

  //----------
  // Ext bus

  /* PIN_75 */ PinOut EXT_PIN_CLK;     // <- P01.BUDE/BEVA

  /* PIN_78 */ PinOut EXT_PIN_WRn_A;   // <- P08.UVER
  /* PIN_78 */ PinIn  EXT_PIN_WRn_C;   // -> P07.UBAL
  /* PIN_78 */ PinOut EXT_PIN_WRn_D;   // <- P08.USUF

  /* PIN_79 */ PinOut EXT_PIN_RDn_A;   // <- P08.UGAC
  /* PIN_79 */ PinIn  EXT_PIN_RDn_C;   // -> P07.UJYV
  /* PIN_79 */ PinOut EXT_PIN_RDn_D;   // <- P08.URUN

  /* PIN_80 */ PinOut EXT_PIN_CSn_A;   // <- P08.TYHO

  /* PIN_01 */ PinOut EXT_PIN_A00_A;   // <- P08.KUPO
  /* PIN_01 */ PinIn  EXT_PIN_A00_C;   // -> P08.KOVA
  /* PIN_01 */ PinOut EXT_PIN_A00_D;   // <- P08.KOTY

  /* PIN_02 */ PinOut EXT_PIN_A01_A;   // <- P08.CABA
  /* PIN_02 */ PinIn  EXT_PIN_A01_C;   // -> P08.CAMU
  /* PIN_02 */ PinOut EXT_PIN_A01_D;   // <- P08.COTU

  /* PIN_03 */ PinOut EXT_PIN_A02_A;   // <- P08.BOKU
  /* PIN_03 */ PinIn  EXT_PIN_A02_C;   // -> P08.BUXU
  /* PIN_03 */ PinOut EXT_PIN_A02_D;   // <- P08.BAJO

  /* PIN_04 */ PinOut EXT_PIN_A03_A;   // <- P08.BOTY
  /* PIN_04 */ PinIn  EXT_PIN_A03_C;   // -> P08.BASE
  /* PIN_04 */ PinOut EXT_PIN_A03_D;   // <- P08.BOLA

  /* PIN_05 */ PinOut EXT_PIN_A04_A;   // <- P08.BYLA
  /* PIN_05 */ PinIn  EXT_PIN_A04_C;   // -> P08.AFEC
  /* PIN_05 */ PinOut EXT_PIN_A04_D;   // <- P08.BEVO

  /* PIN_06 */ PinOut EXT_PIN_A05_A;   // <- P08.BADU
  /* PIN_06 */ PinIn  EXT_PIN_A05_C;   // -> P08.ABUP
  /* PIN_06 */ PinOut EXT_PIN_A05_D;   // <- P08.AJAV

  /* PIN_07 */ PinOut EXT_PIN_A06_A;   // <- P08.CEPU
  /* PIN_07 */ PinIn  EXT_PIN_A06_C;   // -> P08.CYGU
  /* PIN_07 */ PinOut EXT_PIN_A06_D;   // <- P08.CYKA

  /* PIN_08 */ PinOut EXT_PIN_A07_A;   // <- P08.DEFY
  /* PIN_08 */ PinIn  EXT_PIN_A07_C;   // -> P08.COGO
  /* PIN_08 */ PinOut EXT_PIN_A07_D;   // <- P08.COLO

  /* PIN_09 */ PinOut EXT_PIN_A08_A;   // <- P08.MYNY
  /* PIN_09 */ PinIn  EXT_PIN_A08_C;   // -> P08.MUJY
  /* PIN_09 */ PinOut EXT_PIN_A08_D;   // <- P08.MEGO

  /* PIN_10 */ PinOut EXT_PIN_A09_A;   // <- P08.MUNE
  /* PIN_10 */ PinIn  EXT_PIN_A09_C;   // -> P08.NENA
  /* PIN_10 */ PinOut EXT_PIN_A09_D;   // <- P08.MENY

  /* PIN_11 */ PinOut EXT_PIN_A10_A;   // <- P08.ROXU
  /* PIN_11 */ PinIn  EXT_PIN_A10_C;   // -> P08.SURA
  /* PIN_11 */ PinOut EXT_PIN_A10_D;   // <- P08.RORE

  /* PIN_12 */ PinOut EXT_PIN_A11_A;   // <- P08.LEPY
  /* PIN_12 */ PinIn  EXT_PIN_A11_C;   // -> P08.MADY
  /* PIN_12 */ PinOut EXT_PIN_A11_D;   // <- P08.LYNY

  /* PIN_13 */ PinOut EXT_PIN_A12_A;   // <- P08.LUCE
  /* PIN_13 */ PinIn  EXT_PIN_A12_C;   // -> P08.LAHE
  /* PIN_13 */ PinOut EXT_PIN_A12_D;   // <- P08.LOSO

  /* PIN_14 */ PinOut EXT_PIN_A13_A;   // <- P08.LABE
  /* PIN_14 */ PinIn  EXT_PIN_A13_C;   // -> P08.LURA
  /* PIN_14 */ PinOut EXT_PIN_A13_D;   // <- P08.LEVA

  /* PIN_15 */ PinOut EXT_PIN_A14_A;   // <- P08.PUHE
  /* PIN_15 */ PinIn  EXT_PIN_A14_C;   // -> P08.PEVO
  /* PIN_15 */ PinOut EXT_PIN_A14_D;   // <- P08.PAHY

  /* PIN_16 */ PinOut EXT_PIN_A15_A;   // <- P08.SUZE
  /* PIN_16 */ PinIn  EXT_PIN_A15_C;   // -> P08.RAZA
  /* PIN_16 */ PinOut EXT_PIN_A15_D;   // <- P08.RULO
                      
  /* PIN_17 */ PinOut EXT_PIN_D0_A;    // <- P08.RUXA
  /* PIN_17 */ PinOut EXT_PIN_D0_B;    // <- P08.LULA
  /* PIN_17 */ PinIn  EXT_PIN_D0_C;    // -> P08.TOVO,SOMA
  /* PIN_17 */ PinOut EXT_PIN_D0_D;    // <- P08.RUNE

  /* PIN_18 */ PinOut EXT_PIN_D1_A;    // <- P08.RUJA
  /* PIN_18 */ PinOut EXT_PIN_D1_B;    // <- P08.LULA
  /* PIN_18 */ PinIn  EXT_PIN_D1_C;    // -> P08.RUZY,RONY
  /* PIN_18 */ PinOut EXT_PIN_D1_D;    // <- P08.RYPU

  /* PIN_19 */ PinOut EXT_PIN_D2_A;    // <- P08.RABY
  /* PIN_19 */ PinOut EXT_PIN_D2_B;    // <- P08.LULA
  /* PIN_19 */ PinIn  EXT_PIN_D2_C;    // -> P08.ROME,RAXY
  /* PIN_19 */ PinOut EXT_PIN_D2_D;    // <- P08.SULY

  /* PIN_20 */ PinOut EXT_PIN_D3_A;    // <- P08.RERA
  /* PIN_20 */ PinOut EXT_PIN_D3_B;    // <- P08.LULA
  /* PIN_20 */ PinIn  EXT_PIN_D3_C;    // -> P08.SAZA,SELO
  /* PIN_20 */ PinOut EXT_PIN_D3_D;    // <- P08.SEZE

  /* PIN_21 */ PinOut EXT_PIN_D4_A;    // <- P08.RORY
  /* PIN_21 */ PinOut EXT_PIN_D4_B;    // <- P08.LULA
  /* PIN_21 */ PinIn  EXT_PIN_D4_C;    // -> P08.TEHE,SODY
  /* PIN_21 */ PinOut EXT_PIN_D4_D;    // <- P08.RESY

  /* PIN_22 */ PinOut EXT_PIN_D5_A;    // <- P08.RYVO
  /* PIN_22 */ PinOut EXT_PIN_D5_B;    // <- P08.LULA
  /* PIN_22 */ PinIn  EXT_PIN_D5_C;    // -> P08.RATU,SAGO
  /* PIN_22 */ PinOut EXT_PIN_D5_D;    // <- P08.TAMU

  /* PIN_23 */ PinOut EXT_PIN_D6_A;    // <- P08.RAFY
  /* PIN_23 */ PinOut EXT_PIN_D6_B;    // <- P08.LULA
  /* PIN_23 */ PinIn  EXT_PIN_D6_C;    // -> P08.SOCA,RUPA
  /* PIN_23 */ PinOut EXT_PIN_D6_D;    // <- P08.ROGY

  /* PIN_24 */ PinOut EXT_PIN_D7_A;    // <- P08.RAVU
  /* PIN_24 */ PinOut EXT_PIN_D7_B;    // <- P08.LULA
  /* PIN_24 */ PinIn  EXT_PIN_D7_C;    // -> P08.RYBA,SAZY
  /* PIN_24 */ PinOut EXT_PIN_D7_D;    // <- P08.RYDA

  //----------
  // VRAM bus

  /* PIN_43 */ PinOut VRAM_PIN_MCSn_A;   // <- P25.SOKY
  /* PIN_43 */ PinIn  VRAM_PIN_MCSn_C;   // -> P25.TEFY
  /* PIN_43 */ PinOut VRAM_PIN_MCSn_D;   // <- P25.SETY

  /* PIN_45 */ PinOut VRAM_PIN_MOEn_A;   // <- P25.REFO
  /* PIN_45 */ PinIn  VRAM_PIN_MOEn_C;   // -> P25.TAVY
  /* PIN_45 */ PinOut VRAM_PIN_MOEn_D;   // <- P25.SAHA

  /* PIN_49 */ PinOut VRAM_PIN_MWRn_A;   // <- P25.SYSY
  /* PIN_49 */ PinIn  VRAM_PIN_MWRn_C;   // -> P25.SUDO
  /* PIN_49 */ PinOut VRAM_PIN_MWRn_D;   // <- P25.RAGU

  /* PIN_34 */ PinOut VRAM_PIN_MA00_AD;  // <- P04.ECAL
  /* PIN_35 */ PinOut VRAM_PIN_MA01_AD;  // <- P04.EGEZ
  /* PIN_36 */ PinOut VRAM_PIN_MA02_AD;  // <- P04.FUHE
  /* PIN_37 */ PinOut VRAM_PIN_MA03_AD;  // <- P04.FYZY
  /* PIN_38 */ PinOut VRAM_PIN_MA04_AD;  // <- P04.DAMU
  /* PIN_39 */ PinOut VRAM_PIN_MA05_AD;  // <- P04.DAVA
  /* PIN_40 */ PinOut VRAM_PIN_MA06_AD;  // <- P04.ETEG
  /* PIN_41 */ PinOut VRAM_PIN_MA07_AD;  // <- P04.EREW
  /* PIN_48 */ PinOut VRAM_PIN_MA08_AD;  // <- P04.EVAX
  /* PIN_47 */ PinOut VRAM_PIN_MA09_AD;  // <- P04.DUVE
  /* PIN_44 */ PinOut VRAM_PIN_MA10_AD;  // <- P04.ERAF
  /* PIN_46 */ PinOut VRAM_PIN_MA11_AD;  // <- P04.FUSY
  /* PIN_42 */ PinOut VRAM_PIN_MA12_AD;  // <- P04.EXYF

  /* PIN_33 */ PinOut VRAM_PIN_MD0_A;    // <- P25.REGE
  /* PIN_31 */ PinOut VRAM_PIN_MD1_A;    // <- P25.RYKY
  /* PIN_30 */ PinOut VRAM_PIN_MD2_A;    // <- P25.RAZO
  /* PIN_29 */ PinOut VRAM_PIN_MD3_A;    // <- P25.RADA
  /* PIN_28 */ PinOut VRAM_PIN_MD4_A;    // <- P25.RYRO
  /* PIN_27 */ PinOut VRAM_PIN_MD5_A;    // <- P25.REVU
  /* PIN_26 */ PinOut VRAM_PIN_MD6_A;    // <- P25.REKU
  /* PIN_25 */ PinOut VRAM_PIN_MD7_A;    // <- P25.RYZE

  /* PIN_33 */ PinOut VRAM_PIN_MD0_B;    // <- P25.ROFA
  /* PIN_31 */ PinOut VRAM_PIN_MD1_B;    // <- P25.ROFA
  /* PIN_30 */ PinOut VRAM_PIN_MD2_B;    // <- P25.ROFA
  /* PIN_29 */ PinOut VRAM_PIN_MD3_B;    // <- P25.ROFA
  /* PIN_28 */ PinOut VRAM_PIN_MD4_B;    // <- P25.ROFA
  /* PIN_27 */ PinOut VRAM_PIN_MD5_B;    // <- P25.ROFA
  /* PIN_26 */ PinOut VRAM_PIN_MD6_B;    // <- P25.ROFA
  /* PIN_25 */ PinOut VRAM_PIN_MD7_B;    // <- P25.ROFA

  /* PIN_33 */ PinIn  VRAM_PIN_MD0_C;    // -> P25.RODY
  /* PIN_31 */ PinIn  VRAM_PIN_MD1_C;    // -> P25.REBA
  /* PIN_30 */ PinIn  VRAM_PIN_MD2_C;    // -> P25.RYDO
  /* PIN_29 */ PinIn  VRAM_PIN_MD3_C;    // -> P25.REMO
  /* PIN_28 */ PinIn  VRAM_PIN_MD4_C;    // -> P25.ROCE
  /* PIN_27 */ PinIn  VRAM_PIN_MD5_C;    // -> P25.ROPU
  /* PIN_26 */ PinIn  VRAM_PIN_MD6_C;    // -> P25.RETA
  /* PIN_25 */ PinIn  VRAM_PIN_MD7_C;    // -> P25.RAKU

  /* PIN_33 */ PinOut VRAM_PIN_MD0_D;    // <- P25.RURA
  /* PIN_31 */ PinOut VRAM_PIN_MD1_D;    // <- P25.RULY
  /* PIN_30 */ PinOut VRAM_PIN_MD2_D;    // <- P25.RARE
  /* PIN_29 */ PinOut VRAM_PIN_MD3_D;    // <- P25.RODU
  /* PIN_28 */ PinOut VRAM_PIN_MD4_D;    // <- P25.RUBE
  /* PIN_27 */ PinOut VRAM_PIN_MD5_D;    // <- P25.RUMU
  /* PIN_26 */ PinOut VRAM_PIN_MD6_D;    // <- P25.RYTY
  /* PIN_25 */ PinOut VRAM_PIN_MD7_D;    // <- P25.RADY

  Tribuf VRM_TRI_A00;
  Tribuf VRM_TRI_A01;
  Tribuf VRM_TRI_A02;
  Tribuf VRM_TRI_A03;
  Tribuf VRM_TRI_A04;
  Tribuf VRM_TRI_A05;
  Tribuf VRM_TRI_A06;
  Tribuf VRM_TRI_A07;
  Tribuf VRM_TRI_A08;
  Tribuf VRM_TRI_A09;
  Tribuf VRM_TRI_A10;
  Tribuf VRM_TRI_A11;
  Tribuf VRM_TRI_A12;

  Tribuf VRM_TRI_D0;
  Tribuf VRM_TRI_D1;
  Tribuf VRM_TRI_D2;
  Tribuf VRM_TRI_D3;
  Tribuf VRM_TRI_D4;
  Tribuf VRM_TRI_D5;
  Tribuf VRM_TRI_D6;
  Tribuf VRM_TRI_D7;

  //----------
  // Sprite store tribufs

  Tribuf SPR_TRI_IDX_0;
  Tribuf SPR_TRI_IDX_1;
  Tribuf SPR_TRI_IDX_2;
  Tribuf SPR_TRI_IDX_3;
  Tribuf SPR_TRI_IDX_4;
  Tribuf SPR_TRI_IDX_5;

  Tribuf SPR_TRI_LINE_0;
  Tribuf SPR_TRI_LINE_1;
  Tribuf SPR_TRI_LINE_2;
  Tribuf SPR_TRI_LINE_3;

  //----------

private:

  ClockRegisters clk_reg;
  DebugRegisters dbg_reg;
  DmaRegisters dma_reg;
  InterruptRegisters int_reg;
  JoypadRegisters joy_reg;
  LcdRegisters lcd_reg;
  PixelPipeRegisters pxp_reg;
  ResetRegisters rst_reg;
  SerialRegisters ser_reg;
  SpriteStoreRegisters sst_reg;
  TimerRegisters tim_reg;
  PpuRegisters ppu_reg;
  WindowRegisters win_reg;
  CpuBus  cpu_bus;
  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;
  BusMux bus_mux;
  Bootrom bootrom;
};

//-----------------------------------------------------------------------------

};