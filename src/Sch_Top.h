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

#include "Sch_VramPins.h"
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
  SignalHash commit_everything();

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
  // Bootrom signals

  wire BOOT_BITn() const;
  wire TUTU_BOOTp() const;

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

  //------------------------------------------------------------------------------
  // Joypad signals

  wire ASOK_INT_JPp() const;

  //-----------------------------------------------------------------------------
  // LCD signals


  wire BYHA_VID_LINE_TRIG_d4n() const {
    /*p28.ABAF*/ wire ABAF_VID_LINE_d4n = not(lcd_reg.CATU_VID_LINE_d4.q());
    /*p01.ATAR*/ wire ATAR_VID_RSTp = not(XAPO_VID_RSTn());
    /*p01.ABEZ*/ wire ABEZ_VID_RSTn = not(ATAR_VID_RSTp);
    /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (lcd_reg.ANEL_VID_LINE_d6.q(), ABAF_VID_LINE_d4n), ABEZ_VID_RSTn);
    return BYHA_VID_LINE_TRIG_d4n;
  }

  wire RUTU_NEW_LINE_d0() const { return lcd_reg.RUTU_NEW_LINE_d0; }
  wire CATU_VID_LINE_d4() const { return lcd_reg.CATU_VID_LINE_d4; }
  wire POPU_VBLANK_d4() const { return lcd_reg.POPU_VBLANK_d4; }
  wire ROPO_LY_MATCH_SYNCp() const { return lcd_reg.ROPO_LY_MATCH_SYNCp; }
  wire MUWY_Y0() const { return lcd_reg.MUWY_Y0; }
  wire MYRO_Y1() const { return lcd_reg.MYRO_Y1; }
  wire LEXA_Y2() const { return lcd_reg.LEXA_Y2; }
  wire LYDO_Y3() const { return lcd_reg.LYDO_Y3; }
  wire LOVU_Y4() const { return lcd_reg.LOVU_Y4; }
  wire LEMA_Y5() const { return lcd_reg.LEMA_Y5; }
  wire MATO_Y6() const { return lcd_reg.MATO_Y6; }
  wire LAFO_Y7() const { return lcd_reg.LAFO_Y7; }

  //-----------------------------------------------------------------------------
  // Address decoder signals

  wire WERO_FF40_FF4Fp() const {
    /*p07.TONA*/ wire TONA_A08n = not(CPU_PIN_A08);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
    /*p22.XALY*/ wire XALY_0x00xxxxp = nor(CPU_PIN_A07, CPU_PIN_A05, CPU_PIN_A04);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p22.WUTU*/ wire WUTU_FF40_FF4Fn = nand(SYKE_FF00_FFFFp, CPU_PIN_A06, XALY_0x00xxxxp);
    /*p22.WERO*/ wire WERO_FF40_FF4Fp = not(WUTU_FF40_FF4Fn);
    return WERO_FF40_FF4Fp;
  }

  //-----------------------------------------------------------------------------
  // PPU signals

  /*p21.XYMU*/ wire XYMU_RENDERINGp() const { return ppu_reg.XYMU_RENDERINGp; }

  /*p21.XEHO*/ wire XEHO_X0() const { return ppu_reg.XEHO_X0; }
  /*p21.SAVY*/ wire SAVY_X1() const { return ppu_reg.SAVY_X1; }
  /*p21.XODU*/ wire XODU_X2() const { return ppu_reg.XODU_X2; }
  /*p21.XYDO*/ wire XYDO_X3() const { return ppu_reg.XYDO_X3; }
  /*p21.TUHU*/ wire TUHU_X4() const { return ppu_reg.TUHU_X4; }
  /*p21.TUKY*/ wire TUKY_X5() const { return ppu_reg.TUKY_X5; }
  /*p21.TAKO*/ wire TAKO_X6() const { return ppu_reg.TAKO_X6; }
  /*p21.SYBE*/ wire SYBE_X7() const { return ppu_reg.SYBE_X7; }

  wire SERE_VRAM_RD() const {
    // the logic here is kinda weird, still seems to select vram.
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09);
    /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp);
    /*p08.SORE*/ wire SORE_0000_7FFFp = not(CPU_PIN_A15);
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(CPU_PIN_A13, CPU_PIN_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
    /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor(SYRO_FE00_FFFFp, TEXO_8000_9FFFn);
    /*p25.SOSE*/ wire SOSE_8000_9FFFp = and (CPU_PIN_A15, TEFA_8000_9FFFp);
    /*p25.TUCA*/ wire TUCA_CPU_VRAM_RD = and (SOSE_8000_9FFFp, ABUZ());
    /*p25.TEFY*/ wire TEFY_MCSn_Cn = not(EXT_PIN_MCSn_C);
    /*p25.TOLE*/ wire TOLE_VRAM_RD = mux2_p(TEFY_MCSn_Cn, TUCA_CPU_VRAM_RD, TUTO_DBG_VRAMp());
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(XYMU_RENDERINGp());
    /*p25.SERE*/ wire SERE_VRAM_RD = and (TOLE_VRAM_RD, ROPY_RENDERINGn);
    return SERE_VRAM_RD;
  }

  //-----------------------------------------------------------------------------
  // Tile fetcher signals

  /*p23.DATY*/ wire DATY_SCX0() const { return tile_fetcher.DATY_SCX0; }
  /*p23.DUZU*/ wire DUZU_SCX1() const { return tile_fetcher.DUZU_SCX1; }
  /*p23.CYXU*/ wire CYXU_SCX2() const { return tile_fetcher.CYXU_SCX2; }

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

  /*p28.GUSE*/ wire GUSE_SCAN0n() const { return not(sprite_scanner.SCAN0.q()); }
  /*p28.GEMA*/ wire GEMA_SCAN1n() const { return not(sprite_scanner.SCAN1.q()); }
  /*p28.FUTO*/ wire FUTO_SCAN2n() const { return not(sprite_scanner.SCAN2.q()); }
  /*p28.FAKU*/ wire FAKU_SCAN3n() const { return not(sprite_scanner.SCAN3.q()); }
  /*p28.GAMA*/ wire GAMA_SCAN4n() const { return not(sprite_scanner.SCAN4.q()); }
  /*p28.GOBY*/ wire GOBY_SCAN5n() const { return not(sprite_scanner.SCAN5.q()); }

  //-----------------------------------------------------------------------------
  // Sprite fetcher signals

  wire WEFY_SPR_READp() const {
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp());
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1, sprite_fetcher.TESE_SFETCH_S2);
    /*p28.WEFY*/ wire WEFY_SPR_READp = and(TUVO_PPU_OAM_RDp, sprite_fetcher.TYFO_SFETCH_S0_D1);
    return WEFY_SPR_READp;
  }

  wire VAPE_FETCH_OAM_CLK() const {
#if 0
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(TYFO_SFETCH_S0_D1, !TOXE_SFETCH_S0_D0);
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and (RENDERING, !TULY_SFETCH_S1, !TESE_SFETCH_S2, TACU_SPR_SEQ_5_TRIG);
#endif
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


  int phase_counter = -32;

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */

  // FF40 - LCDC
  /*p23.VYXE*/ Reg VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg XONA_LCDC_EN;

  //----------

  PinIn  CPU_PIN_RD;         // top right port PORTA_00
  PinIn  CPU_PIN_WR;         // top right port PORTA_01
  PinIn  CPU_PIN_ADDR_VALID; // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.
  PinIn  CPU_PIN5;           // top left port PORTD_06: -> ANUJ (FROM_CPU5). Maybe this means "latch the bus"?
  PinIn  CPU_PIN6;           // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  PinIn  CPU_PIN_CLKREQ;     // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  PinIn  CPU_PIN_A00; // bottom right port PORTB_00: -> A00
  PinIn  CPU_PIN_A01; // bottom right port PORTB_04: -> A01
  PinIn  CPU_PIN_A02; // bottom right port PORTB_08: -> A02
  PinIn  CPU_PIN_A03; // bottom right port PORTB_12: -> A03
  PinIn  CPU_PIN_A04; // bottom right port PORTB_16: -> A04
  PinIn  CPU_PIN_A05; // bottom right port PORTB_20: -> A05
  PinIn  CPU_PIN_A06; // bottom right port PORTB_24: -> A06
  PinIn  CPU_PIN_A07; // bottom right port PORTB_28: -> A07
  PinIn  CPU_PIN_A08; // bottom right port PORTB_02: -> A08
  PinIn  CPU_PIN_A09; // bottom right port PORTB_06: -> A09
  PinIn  CPU_PIN_A10; // bottom right port PORTB_10: -> A10
  PinIn  CPU_PIN_A11; // bottom right port PORTB_14: -> A11
  PinIn  CPU_PIN_A12; // bottom right port PORTB_18: -> A12
  PinIn  CPU_PIN_A13; // bottom right port PORTB_22: -> A13
  PinIn  CPU_PIN_A14; // bottom right port PORTB_26: -> A14
  PinIn  CPU_PIN_A15; // bottom right port PORTB_30: -> A15

  Tribuf CPU_TRI_D0;
  Tribuf CPU_TRI_D1;
  Tribuf CPU_TRI_D2;
  Tribuf CPU_TRI_D3;
  Tribuf CPU_TRI_D4;
  Tribuf CPU_TRI_D5;
  Tribuf CPU_TRI_D6;
  Tribuf CPU_TRI_D7;

  PinOut CPU_PIN_BOOTp;    // top right port PORTA_04: <- P07.READ_BOOTROM tutu?

  //----------
  // ext bus

  /* PIN_01 */ PinIn EXT_PIN_A00_C;   // -> P08.KOVA
  /* PIN_02 */ PinIn EXT_PIN_A01_C;   // -> P08.CAMU
  /* PIN_03 */ PinIn EXT_PIN_A02_C;   // -> P08.BUXU
  /* PIN_04 */ PinIn EXT_PIN_A03_C;   // -> P08.BASE
  /* PIN_05 */ PinIn EXT_PIN_A04_C;   // -> P08.AFEC
  /* PIN_06 */ PinIn EXT_PIN_A05_C;   // -> P08.ABUP
  /* PIN_07 */ PinIn EXT_PIN_A06_C;   // -> P08.CYGU
  /* PIN_08 */ PinIn EXT_PIN_A07_C;   // -> P08.COGO
  /* PIN_09 */ PinIn EXT_PIN_A08_C;   // -> P08.MUJY
  /* PIN_10 */ PinIn EXT_PIN_A09_C;   // -> P08.NENA
  /* PIN_11 */ PinIn EXT_PIN_A10_C;   // -> P08.SURA
  /* PIN_12 */ PinIn EXT_PIN_A11_C;   // -> P08.MADY
  /* PIN_13 */ PinIn EXT_PIN_A12_C;   // -> P08.LAHE
  /* PIN_14 */ PinIn EXT_PIN_A13_C;   // -> P08.LURA
  /* PIN_15 */ PinIn EXT_PIN_A14_C;   // -> P08.PEVO
  /* PIN_16 */ PinIn EXT_PIN_A15_C;   // -> P08.RAZA

  /* PIN_45 */ PinIn  EXT_PIN_MOEn_C;  // -> P25.TAVY
  /* PIN_43 */ PinIn  EXT_PIN_MCSn_C;  // -> P25.TEFY

  /* PIN_49 */ PinIn  EXT_PIN_MWRn_C;  // -> P25.SUDO

  /* PIN_33 */ PinIn  EXT_PIN_MD0_C;   // -> P25.RODY
  /* PIN_31 */ PinIn  EXT_PIN_MD1_C;   // -> P25.REBA
  /* PIN_30 */ PinIn  EXT_PIN_MD2_C;   // -> P25.RYDO
  /* PIN_29 */ PinIn  EXT_PIN_MD3_C;   // -> P25.REMO
  /* PIN_28 */ PinIn  EXT_PIN_MD4_C;   // -> P25.ROCE
  /* PIN_27 */ PinIn  EXT_PIN_MD5_C;   // -> P25.ROPU
  /* PIN_26 */ PinIn  EXT_PIN_MD6_C;   // -> P25.RETA
  /* PIN_25 */ PinIn  EXT_PIN_MD7_C;   // -> P25.RAKU

  //----------
  // VRAM control pins

  /* PIN_43 */ PinOut EXT_PIN_MCSn_A;   // <- P25.SOKY
  /* PIN_43 */ PinOut EXT_PIN_MCSn_D;   // <- P25.SETY

  /* PIN_45 */ PinOut EXT_PIN_MOEn_A;   // <- P25.REFO
  /* PIN_45 */ PinOut EXT_PIN_MOEn_D;   // <- P25.SAHA

  /* PIN_49 */ PinOut EXT_PIN_MWRn_A;   // <- P25.SYSY
  /* PIN_49 */ PinOut EXT_PIN_MWRn_D;   // <- P25.RAGU

  //----------
  // VRAM address pins

  /* PIN_34 */ PinOut EXT_PIN_MA00_AD;  // <- P04.ECAL
  /* PIN_35 */ PinOut EXT_PIN_MA01_AD;  // <- P04.EGEZ
  /* PIN_36 */ PinOut EXT_PIN_MA02_AD;  // <- P04.FUHE
  /* PIN_37 */ PinOut EXT_PIN_MA03_AD;  // <- P04.FYZY
  /* PIN_38 */ PinOut EXT_PIN_MA04_AD;  // <- P04.DAMU
  /* PIN_39 */ PinOut EXT_PIN_MA05_AD;  // <- P04.DAVA
  /* PIN_40 */ PinOut EXT_PIN_MA06_AD;  // <- P04.ETEG
  /* PIN_41 */ PinOut EXT_PIN_MA07_AD;  // <- P04.EREW
  /* PIN_48 */ PinOut EXT_PIN_MA08_AD;  // <- P04.EVAX
  /* PIN_47 */ PinOut EXT_PIN_MA09_AD;  // <- P04.DUVE
  /* PIN_44 */ PinOut EXT_PIN_MA10_AD;  // <- P04.ERAF
  /* PIN_46 */ PinOut EXT_PIN_MA11_AD;  // <- P04.FUSY
  /* PIN_42 */ PinOut EXT_PIN_MA12_AD;  // <- P04.EXYF

  //----------
  // VRAM data pins

  /* PIN_33 */ PinOut EXT_PIN_MD0_A;   // <- P25.REGE
  /* PIN_31 */ PinOut EXT_PIN_MD1_A;   // <- P25.RYKY
  /* PIN_30 */ PinOut EXT_PIN_MD2_A;   // <- P25.RAZO
  /* PIN_29 */ PinOut EXT_PIN_MD3_A;   // <- P25.RADA
  /* PIN_28 */ PinOut EXT_PIN_MD4_A;   // <- P25.RYRO
  /* PIN_27 */ PinOut EXT_PIN_MD5_A;   // <- P25.REVU
  /* PIN_26 */ PinOut EXT_PIN_MD6_A;   // <- P25.REKU
  /* PIN_25 */ PinOut EXT_PIN_MD7_A;   // <- P25.RYZE

  /* PIN_33 */ PinOut EXT_PIN_MD0_B;   // <- P25.ROFA
  /* PIN_31 */ PinOut EXT_PIN_MD1_B;   // <- P25.ROFA
  /* PIN_30 */ PinOut EXT_PIN_MD2_B;   // <- P25.ROFA
  /* PIN_29 */ PinOut EXT_PIN_MD3_B;   // <- P25.ROFA
  /* PIN_28 */ PinOut EXT_PIN_MD4_B;   // <- P25.ROFA
  /* PIN_27 */ PinOut EXT_PIN_MD5_B;   // <- P25.ROFA
  /* PIN_26 */ PinOut EXT_PIN_MD6_B;   // <- P25.ROFA
  /* PIN_25 */ PinOut EXT_PIN_MD7_B;   // <- P25.ROFA

  /* PIN_33 */ PinOut EXT_PIN_MD0_D;   // <- P25.RURA
  /* PIN_31 */ PinOut EXT_PIN_MD1_D;   // <- P25.RULY
  /* PIN_30 */ PinOut EXT_PIN_MD2_D;   // <- P25.RARE
  /* PIN_29 */ PinOut EXT_PIN_MD3_D;   // <- P25.RODU
  /* PIN_28 */ PinOut EXT_PIN_MD4_D;   // <- P25.RUBE
  /* PIN_27 */ PinOut EXT_PIN_MD5_D;   // <- P25.RUMU
  /* PIN_26 */ PinOut EXT_PIN_MD6_D;   // <- P25.RYTY
  /* PIN_25 */ PinOut EXT_PIN_MD7_D;   // <- P25.RADY

  /* PIN_74 */ PinIn PIN_CLK_IN_xBxDxFxH;
  /* PIN_76 */ PinIn EXT_PIN_T2;
  /* PIN_77 */ PinIn EXT_PIN_T1;
  /* PIN_78 */ PinIn EXT_PIN_WRn_C;   // -> P07.UBAL
  /* PIN_79 */ PinIn EXT_PIN_RD_C;   // -> P07.UJYV

  //----------
  // VRAM bus

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

  VramPins vram_pins;

  CpuBus  cpu_bus;
  CpuPinsOut cpu_pins_out;

  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;

  BusMux bus_mux;

  Bootrom bootrom;

  /* PIN_71 */ PinIn EXT_PIN_RST;
  /* PIN_74 */ PinIn EXT_PIN_CLK_GOOD;

  /* PIN_17 */ PinIn EXT_PIN_D0_C;    // -> P08.TOVO,SOMA
  /* PIN_18 */ PinIn EXT_PIN_D1_C;    // -> P08.RUZY,RONY
  /* PIN_19 */ PinIn EXT_PIN_D2_C;    // -> P08.ROME,RAXY
  /* PIN_20 */ PinIn EXT_PIN_D3_C;    // -> P08.SAZA,SELO
  /* PIN_21 */ PinIn EXT_PIN_D4_C;    // -> P08.TEHE,SODY
  /* PIN_22 */ PinIn EXT_PIN_D5_C;    // -> P08.RATU,SAGO
  /* PIN_23 */ PinIn EXT_PIN_D6_C;    // -> P08.SOCA,RUPA
  /* PIN_24 */ PinIn EXT_PIN_D7_C;    // -> P08.RYBA,SAZY

private:

};

//-----------------------------------------------------------------------------

};