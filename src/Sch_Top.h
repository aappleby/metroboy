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

namespace Schematics {

//-----------------------------------------------------------------------------

struct SchematicTop {

  void set_cpu(uint16_t addr, uint8_t data, bool read, bool write);
  void set_ext();
  void set_joy(uint8_t buttons);
  void set_vram(wire OE, uint8_t data);
  void set_oam();

  SignalHash tick();

  //-----------------------------------------------------------------------------

  void tick_vram_pins();
  void tick_top_regs();
  void tick_cpu_bus();

  //-----------------------------------------------------------------------------

  SignalHash commit_io();
  SignalHash commit_ibus();
  SignalHash commit_vbus();
  SignalHash commit_ebus();
  SignalHash commit_obus();

  SignalHash commit_lcd_pins();
  SignalHash commit_joy_pins();

  SignalHash commit_top_regs();

  //-----------------------------------------------------------------------------
  // Debug signals

  /*p07.UBET*/ wire UBET_T1p() const { return not(SYS_PIN_T1n); }
  /*p07.UVAR*/ wire UVAR_T2p() const { return not(SYS_PIN_T2n); }

  /*p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and(SYS_PIN_T1n, UVAR_T2p()); }
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and(SYS_PIN_T2n, UBET_T1p()); }
  /*p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand(UBET_T1p(), UVAR_T2p(), SYS_PIN_RSTp); }
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not(UNOR_MODE_DBG2p()); }

  //-----------------------------------------------------------------------------
  // Clock signals

  //----------------------------------------
  // ATAL 4 mhz chain

  // ignoring the deglitcher here
  /*p01.ATAL*/ wire ATAL_xBxDxFxH() const { return SYS_PIN_CLK_B; }
  /*p01.AZOF*/ wire AZOF_AxCxExGx() const { return not(ATAL_xBxDxFxH()); }
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH() const { return not(AZOF_AxCxExGx()); }
  /*p01.ZEME*/ wire ZEME_AxCxExGx() const { return not(ZAXY_xBxDxFxH()); }
  /*p01.ALET*/ wire ALET_xBxDxFxH() const { return not(ZEME_AxCxExGx()); }
  /*p27.MOXE*/ wire MOXE_AxCxExGx() const { return not(ALET_xBxDxFxH()); }
  /*p27.MEHE*/ wire MEHE_AxCxExGx() const { return not(ALET_xBxDxFxH()); }
  /*p27.MYVO*/ wire MYVO_AxCxExGx() const { return not(ALET_xBxDxFxH()); }

  //----------------------------------------
  // Root chains

  /*p01.AFUR*/ wire AFUR_ABCDxxxx() const { return clk_reg.AFUR_ABCDxxxx; }
  /*p01.ALEF*/ wire ALEF_xBCDExxx() const { return clk_reg.ALEF_xBCDExxx; }
  /*p01.APUK*/ wire APUK_xxCDEFxx() const { return clk_reg.APUK_xxCDEFxx; }
  /*p01.ADYK*/ wire ADYK_xxxDEFGx() const { return clk_reg.ADYK_xxxDEFGx; }

  /*p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not(!AFUR_ABCDxxxx()); }
  /*p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not( ALEF_xBCDExxx()); }
  /*p01.AROV*/ wire AROV_xxCDEFxx() const { return not(!APUK_xxCDEFxx()); }
  /*p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not( ADYK_xxxDEFGx()); }

  //----------------------------------------
  // Video chains

  /*p29.WUVU*/ wire WUVU_xxCDxxGH() const { return clk_reg.WUVU_xxCDxxGH; }
  /*p21.VENA*/ wire VENA_xxxxEFGH() const { return clk_reg.VENA_xxxxEFGH; }
  /*p29.WOSU*/ wire WOSU_xBCxxFGx() const { return clk_reg.WOSU_xBCxxFGx; }

  /*p29.XOCE*/ wire XOCE_AxxDExxH() const { return not(WOSU_xBCxxFGx()); }
  /*p29.XUPY*/ wire XUPY_ABxxEFxx() const { return not(WUVU_xxCDxxGH()); }
  /*p28.AWOH*/ wire AWOH_xxCDxxGH() const { return not(XUPY_ABxxEFxx()); }
  /*p21.TALU*/ wire TALU_ABCDxxxx() const { return not(VENA_xxxxEFGH()); }
  /*p21.SONO*/ wire SONO_xxxxEFGH() const { return not(TALU_ABCDxxxx()); }

  //----------------------------------------
  // NULE chain
  wire NULE_xxxxEFGH() const {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_READYp);
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(clk_reg.AFUR_ABCDxxxx.qn());
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(ABOL_CLKREQn,  ATYP_ABCDxxxx);
    return NULE_xxxxEFGH;
  }
  /*p01.BYRY*/ wire BYRY_ABCDxxxx() const { return not(NULE_xxxxEFGH()); }
  /*p01.BUDE*/ wire BUDE_xxxxEFGH() const { return not(BYRY_ABCDxxxx()); }
  /*p01.UVYT*/ wire UVYT_ABCDxxxx() const { return not(BUDE_xxxxEFGH()); }
  /*p01.BEKO*/ wire BEKO_ABCDxxxx() const { return not(BUDE_xxxxEFGH()); }
  /*p04.MOPA*/ wire MOPA_xxxxEFGH() const { return not(UVYT_ABCDxxxx()); }
  /*p28.XYNY*/ wire XYNY_ABCDxxxx() const { return not(MOPA_xxxxEFGH()); }

  //----------------------------------------
  // BAPY chain
  wire BAPY_xxxxxxGH() const {
    /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_READYp);
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(clk_reg.AFUR_ABCDxxxx.qn());
    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(ABOL_CLKREQn, AROV_xxCDEFxx(), ATYP_ABCDxxxx);
    return BAPY_xxxxxxGH;
  }
  /*p01.BERU*/ wire BERU_ABCDEFxx() const { return not(BAPY_xxxxxxGH()); }
  /*p01.BUFA*/ wire BUFA_xxxxxxGH() const { return not(BERU_ABCDEFxx()); }
  /*p01.BOLO*/ wire BOLO_ABCDEFxx() const { return not(BUFA_xxxxxxGH()); }

  //----------------------------------------
  // BEJA chain

  wire BEJA_xxxxEFGH() const {
    /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx(), BEKO_ABCDxxxx());
    return BEJA_xxxxEFGH;
  }
  /*p01.BANE*/ wire BANE_ABCDxxxx() const { return not(BEJA_xxxxEFGH()); }
  /*p01.BELO*/ wire BELO_xxxxEFGH() const { return not(BANE_ABCDxxxx()); }
  /*p01.BAZE*/ wire BAZE_ABCDxxxx() const { return not(BELO_xxxxEFGH()); }

  //----------------------------------------
  // BUTO chain

  wire BUTO_xBCDEFGH() const {
    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH(), ATYP_ABCDxxxx(), BAZE_ABCDxxxx());
    return BUTO_xBCDEFGH;
  }
  /*p01.BELE*/ wire BELE_Axxxxxxx() const { return not(BUTO_xBCDEFGH()); }

  //----------------------------------------
  // BYJU chain

  wire BYJU_xBCDEFGH() const {
    /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(SYS_PIN_CLK_A);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx(), ATEZ_CLKBAD);
    return BYJU_xBCDEFGH;
  }
  /*p01.BALY*/ wire BALY_Axxxxxxx() const { return not(BYJU_xBCDEFGH()); }
  /*p01.BOGA*/ wire BOGA_xBCDEFGH() const { return not(BALY_Axxxxxxx()); }
  /*p01.BOMA*/ wire BOMA_Axxxxxxx() const { return not(BOGA_xBCDEFGH()); }

  //----------------------------------------
  // AFAS chain

  wire AFAS_xxxxEFGx() const {
    /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor(ADAR_ABCxxxxH(), ATYP_ABCDxxxx());
    return AFAS_xxxxEFGx;
  }

  //-----------------------------------------------------------------------------
  // Timer signals

  wire UVYN_DIV_05n() const;
  wire UMEK_DIV_06n() const;
  wire UREK_DIV_07n() const;
  wire UPOF_DIV_15() const;
  wire MOBA_INT_TIMERp() const;

  //-----------------------------------------------------------------------------
  // Reset signals

  //----------------------------------------
  // AVOR sys reset chain

  /*p01.AVOR*/ wire AVOR_SYS_RSTp() const;
  /*p01.ALUR*/ wire ALUR_SYS_RSTn() const { return not(AVOR_SYS_RSTp()); }
  /*p01.DULA*/ wire DULA_SYS_RSTp() const { return not(ALUR_SYS_RSTn()); }
  /*p01.CUNU*/ wire CUNU_SYS_RSTn() const { return not(DULA_SYS_RSTp()); }
  /*p01.XORE*/ wire XORE_SYS_RSTp() const { return not(CUNU_SYS_RSTn()); }
  /*p01.WALU*/ wire WALU_SYS_RSTn() const { return not(XORE_SYS_RSTp()); }
  /*p01.WESY*/ wire WESY_SYS_RSTn() const { return not(XORE_SYS_RSTp()); }

  /*p01.XAPO*/ wire XAPO_VID_RSTn() const;
  /*p01.TOFU*/ wire TOFU_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.LYHA*/ wire LYHA_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.ATAR*/ wire ATAR_VID_RSTp() const { return not(XAPO_VID_RSTn()); }
  /*p01.LYFE*/ wire LYFE_VID_RSTn() const { return not(LYHA_VID_RSTp()); }
  /*p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not(ATAR_VID_RSTp()); }

  //-----------------------------------------------------------------------------
  // CPU signals

  /*p07.UJYV*/ wire UJYV_CPU_RDn() const;
  /*p07.TEDO*/ wire TEDO_CPU_RDp() const { return not(UJYV_CPU_RDn()); }
  /*p07.AJAS*/ wire AJAS_CPU_RDn() const { return not(TEDO_CPU_RDp()); }
  /*p07.ASOT*/ wire ASOT_CPU_RDp() const { return not(AJAS_CPU_RDn()); }

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH() const;
  /*p07.UBAL*/ wire UBAL_CPU_WRp_ABCDxxxH() const;
  /*p07.TAPU*/ wire TAPU_CPU_WRn_xxxxEFGx() const { return not(UBAL_CPU_WRp_ABCDxxxH()); }
  /*p07.DYKY*/ wire DYKY_CPU_WRp_ABCDxxxH() const { return not(TAPU_CPU_WRn_xxxxEFGx()); }
  /*p07.CUPA*/ wire CUPA_CPU_WRn_xxxxEFGx() const { return not(DYKY_CPU_WRp_ABCDxxxH()); }

  int get_addr() const;
  int get_data() const;
  void set_data(bool oe, uint8_t data);

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

  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4() const;

  /*p21.RUTU*/ wire RUTU_LINE_END()       const { return lcd_reg.RUTU_LINE_END; }
  /*p29.CATU*/ wire CATU_LINE_END()       const { return lcd_reg.CATU_LINE_END; }
  /*p21.ROPO*/ wire ROPO_LY_MATCH_SYNCp() const { return lcd_reg.ROPO_LY_MATCH_SYNCp; }
  /*p21.POPU*/ wire POPU_VBLANKp_d4()     const { return lcd_reg.POPU_VBLANKp_d4; }
  /*p21.PARU*/ wire PARU_VBLANKp_d4()     const { return not(lcd_reg.POPU_VBLANKp_d4.qn()); }

  /*p21.MUWY*/ wire MUWY_Y0() const { return lcd_reg.MUWY_Y0; }
  /*p21.MYRO*/ wire MYRO_Y1() const { return lcd_reg.MYRO_Y1; }
  /*p21.LEXA*/ wire LEXA_Y2() const { return lcd_reg.LEXA_Y2; }
  /*p21.LYDO*/ wire LYDO_Y3() const { return lcd_reg.LYDO_Y3; }
  /*p21.LOVU*/ wire LOVU_Y4() const { return lcd_reg.LOVU_Y4; }
  /*p21.LEMA*/ wire LEMA_Y5() const { return lcd_reg.LEMA_Y5; }
  /*p21.MATO*/ wire MATO_Y6() const { return lcd_reg.MATO_Y6; }
  /*p21.LAFO*/ wire LAFO_Y7() const { return lcd_reg.LAFO_Y7; }

  //-----------------------------------------------------------------------------
  // Address decoder signals

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

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const;
  /*p22.WERO*/ wire WERO_FF4Xp() const;

  /*p07.SYKE*/ wire SYKE_FF00_FFFFp() const { return nor(TUNA_0000_FDFFp(), TONA_A08n()); }
  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor(CPU_PIN_A07, CPU_PIN_A06, CPU_PIN_A05, CPU_PIN_A04, CPU_PIN_A03); }

  //-----------------------------------------------------------------------------
  // PPU signals

  wire XYMU_RENDERINGp() const;
  wire TEVO_FINE_RSTp() const;
  wire NYXU_TILE_FETCHER_RSTn() const;
  wire ACYL_SCANNINGp() const;
  wire LYRY_BFETCH_DONEp() const;
  wire SEGU_CLKPIPEn() const;
  wire SACU_CLKPIPEp() const;
  wire ROZE_FINE_COUNT_7n() const;
  wire VOTY_INT_STATp() const;
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

  wire PORY_TILE_FETCH_DONE_Bp() const;
  wire LONY_BG_READ_VRAM_LATCHp() const;

  wire DATY_SCX0() const;
  wire DUZU_SCX1() const;
  wire CYXU_SCX2() const;

  //-----------------------------------------------------------------------------
  // Sprite scanner signals

  wire CEHA_SCANNINGp() const;
  wire BESU_SCANNINGp() const;
  wire CENO_SCANNINGp() const;

  wire AVAP_RENDER_START_RST() const;
  wire CARE_STORE_ENp_ABxxEFxx() const;

  wire DEGE_SPRITE_DELTA0() const;
  wire DABY_SPRITE_DELTA1() const;
  wire DABU_SPRITE_DELTA2() const;
  wire GYSA_SPRITE_DELTA3() const;

  wire YFEL_SCAN0() const;
  wire WEWY_SCAN1() const;
  wire GOSO_SCAN2() const;
  wire ELYN_SCAN3() const;
  wire FAHA_SCAN4() const;
  wire FONY_SCAN5() const;

  //-----------------------------------------------------------------------------
  // Sprite store signals

  wire FEPO_STORE_MATCHp() const;

  //-----------------------------------------------------------------------------
  // Sprite fetcher signals

  wire WEFY_SPR_READp() const;
  wire VAPE_FETCH_OAM_CLK() const;
  wire VUSA_SPRITE_DONEn() const;
  wire TEXY_SPR_READ_VRAMp() const;
  wire SOHO_SPR_VRAM_RDp() const;

  wire SPR_PIX_A0() const;
  wire SPR_PIX_A1() const;
  wire SPR_PIX_A2() const;
  wire SPR_PIX_A3() const;
  wire SPR_PIX_A4() const;
  wire SPR_PIX_A5() const;
  wire SPR_PIX_A6() const;
  wire SPR_PIX_A7() const;

  wire SPR_PIX_B0() const;
  wire SPR_PIX_B1() const;
  wire SPR_PIX_B2() const;
  wire SPR_PIX_B3() const;
  wire SPR_PIX_B4() const;
  wire SPR_PIX_B5() const;
  wire SPR_PIX_B6() const;
  wire SPR_PIX_B7() const;

  //-----------------------------------------------------------------------------
  // Window signals

  wire RYDY_WIN_FIRST_TILE_A() const;
  wire NOCU_WIN_MODEn() const;
  wire NUNY_WX_MATCHpe() const;

  wire WYKA_WIN_X3() const;
  wire WODY_WIN_X4() const;
  wire WOBO_WIN_X5() const;
  wire WYKO_WIN_X6() const;
  wire XOLO_WIN_X7() const;

  wire VYNO_WIN_Y0() const;
  wire VUJO_WIN_Y1() const;
  wire VYMU_WIN_Y2() const;
  wire TUFU_WIN_Y3() const;
  wire TAXA_WIN_Y4() const;
  wire TOZO_WIN_Y5() const;
  wire TATE_WIN_Y6() const;
  wire TEKE_WIN_Y7() const;

  //-----------------------------------------------------------------------------
  // Misc signals

  wire BOOT_BITn() const;
  wire ASOK_INT_JOYPADp() const;
  wire CALY_INT_SERIALp() const;

  //-----------------------------------------------------------------------------
  // Internal state for debugging

  int phase_counter = -16;

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

  /*p32.LEGU*/ Reg8 LEGU_BG_PIX_A0;
  /*p32.NUDU*/ Reg8 NUDU_BG_PIX_A1;
  /*p32.MUKU*/ Reg8 MUKU_BG_PIX_A2;
  /*p32.LUZO*/ Reg8 LUZO_BG_PIX_A3;
  /*p32.MEGU*/ Reg8 MEGU_BG_PIX_A4;
  /*p32.MYJY*/ Reg8 MYJY_BG_PIX_A5;
  /*p32.NASA*/ Reg8 NASA_BG_PIX_A6;
  /*p32.NEFO*/ Reg8 NEFO_BG_PIX_A7; // color wrong on die

  /*p32.RAWU*/ Reg11 RAWU_BG_PIX_B0; // also holds tile index during fetch
  /*p32.POZO*/ Reg11 POZO_BG_PIX_B1;
  /*p32.PYZO*/ Reg11 PYZO_BG_PIX_B2; 
  /*p32.POXA*/ Reg11 POXA_BG_PIX_B3; 
  /*p32.PULO*/ Reg11 PULO_BG_PIX_B4; 
  /*p32.POJU*/ Reg11 POJU_BG_PIX_B5; 
  /*p32.POWY*/ Reg11 POWY_BG_PIX_B6; 
  /*p32.PYJU*/ Reg11 PYJU_BG_PIX_B7;

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

  PinIn  SYS_PIN_RSTp;   // PIN_71 -> UPOJ, UPYF, AFAR, ASOL, UFOL
  PinIn  SYS_PIN_T2n;    // PIN_76, tied to 0 on board - but there's probably an implicit inverter
  PinIn  SYS_PIN_T1n;    // PIN_77, tied to 0 on board - but there's probably an implicit inverter

  // In run mode:
  // if SYS_PIN_CLK_A was tied low
  // then UCOB would be tied high
  // then UFOL would be tied low
  // then TAPE_FF04_WR would not affect DIV
  // Therefore SYS_PIN_CLK_A = 1 in run mode

  PinIn  SYS_PIN_CLK_A; // PIN_74 -> ATEZ, UCOB

  // SYS_PIN_CLK_A -> ATEZ -> BYJU
  // SYS_PIN_CLK_A -> UCOB -> UFOL, UPYF

  PinIn  SYS_PIN_CLK_B; // PIN_74 

  // Ground ties - VYPO, RUNY, WEFE, unlabeled cell between BONE and BUFY.

  wire GND = 0;
  wire VYPO_GND = 0;
  wire RUNY_GND = 0;
  wire WEFE_GND = 0;
  wire PESU_GND = 0;

  //-----------------------------------------------------------------------------
  // Internal bus to CPU

  PinOut CPU_PIN_WAKE;          // top right wire by itself <- P02.AWOB

  PinIn  CPU_PIN_RD;            // top right port PORTA_00: ->
  PinIn  CPU_PIN_WRp;            // top right port PORTA_01: ->
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

  // In run mode, MATU (a Reg17) must _not_ be stuck.
  // If CPU_PIN_CLKREQ was 0 in run mode
  // then ABOL = 1
  // then NULE = 0, BYRY = 1, BUDE = 0, UVYT = 1, and MATU would be stuck.
  // Therefore CPU_PIN_CLKREQ must be 1 in run mode

  PinIn  CPU_PIN_READYp;        // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  // At boot,
  // CPU_PIN_DBG = T1 ^ T2
  // CPU_PIN_INT_RST = !T1 && !T2 && RSTn;

  PinOut CPU_PIN_SYS_RSTp;          // top center port PORTC_01: <- P01.AFER , reset related reg
  PinOut CPU_PIN_EXT_RST;     // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut CPU_PIN_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut CPU_PIN_STARTp;           // top center port PORTC_04: <- P01.CPU_RESET

  PinOut CPU_PIN_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  PinOut CPU_PIN_ADDR_HI;       // top right port PORTA_03: <- P25.SYRO
  PinOut CPU_PIN_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut CPU_PIN_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1






  PinIn  CPU_PIN6;              // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  PinOut CPU_PIN_BOWA_AxCDEFGH; // top left port PORTD_01: <- BOWA_AxCDEFGH // Blue clock - decoders, alu, some reset stuff
  PinOut CPU_PIN_BEDO_xBxxxxxx; // top left port PORTD_02: <- BEDO_xBxxxxxx
  PinOut CPU_PIN_BEKO_xBCDExxx; // top left port PORTD_03: <- BEKO_ABCDxxxx + BAVY connection not indicated on P01 - test pad 1
  PinOut CPU_PIN_BUDE_AxxxxFGH; // top left port PORTD_04: <- BUDE_AxxxxFGH + BEVA
  PinOut CPU_PIN_BOLO_xBCDEFGx; // top left port PORTD_05: <- BOLO_ABCDEFxx + BYDA? - test pad 2
  PinIn  CPU_PIN5;              // top left port PORTD_06: -> ANUJ (FROM_CPU5). Probably "DATA_VALIDn"
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

  //-----------------------------------------------------------------------------
  // OAM bus

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

  //-----------------------------------------------------------------------------
  // LCD pins

  PinOut LCD_PIN_LD1;  // PIN_50 
  PinOut LCD_PIN_LD0;  // PIN_51 
  PinOut LCD_PIN_CPG;  // PIN_52 
  PinOut LCD_PIN_CP;   // PIN_53 
  PinOut LCD_PIN_ST;   // PIN_54 
  PinOut LCD_PIN_CPL;  // PIN_55 
  PinOut LCD_PIN_FR;   // PIN_56 
  PinOut LCD_PIN_S;    // PIN_57 

  //-----------------------------------------------------------------------------
  // Joypad pins

  PinOut JOY_PIN_P10_A;   // PIN_67 <- P05.KOLE
  PinOut JOY_PIN_P10_B;   // PIN_67 <- tied low between BONE and BUFY
  PinIn  JOY_PIN_P10_C;   // PIN_67 -> P02.KERY, P05.KEVU
  PinOut JOY_PIN_P10_D;   // PIN_67 <- P05.KYBU

  PinOut JOY_PIN_P11_A;   // PIN_66 <- P05.KYTO
  PinOut JOY_PIN_P11_B;   // PIN_66 <- tied low between BONE and BUFY
  PinIn  JOY_PIN_P11_C;   // PIN_66 -> P02.KERY, P05.KAPA
  PinOut JOY_PIN_P11_D;   // PIN_66 <- P05.KABU

  PinOut JOY_PIN_P12_A;   // PIN_65 <- P05.KYHU
  PinOut JOY_PIN_P12_B;   // PIN_65 <- tied low between BONE and BUFY
  PinIn  JOY_PIN_P12_C;   // PIN_65 -> P02.KERY, P05.KEJA
  PinOut JOY_PIN_P12_D;   // PIN_65 <- P05.KASY

  PinOut JOY_PIN_P13_A;   // PIN_64 <- P05.KORY
  PinOut JOY_PIN_P13_B;   // PIN_64 <- tied low between BONE and BUFY
  PinIn  JOY_PIN_P13_C;   // PIN_64 -> P02.KERY, P05.KOLO
  PinOut JOY_PIN_P13_D;   // PIN_64 <- P05.KALE

  PinOut JOY_PIN_P14_A;   // PIN_63 <- p05.KARU
  PinOut JOY_PIN_P14_D;   // PIN_63 <- p05.KELY

  PinOut JOY_PIN_P15_A;   // PIN_62 <- p05.CELA
  PinOut JOY_PIN_P15_D;   // PIN_62 <- p05.COFY

  //-----------------------------------------------------------------------------
  // Ext bus

  PinOut EXT_PIN_CLK;     // PIN_75 <- P01.BUDE/BEVA

  PinOut EXT_PIN_WRn_A;   // PIN_78 <- P08.UVER
  PinIn  EXT_PIN_WRp_C;   // PIN_78 -> P07.UBAL
  PinOut EXT_PIN_WRn_D;   // PIN_78 <- P08.USUF

  PinOut EXT_PIN_RDn_A;   // PIN_79 <- P08.UGAC
  PinIn  EXT_PIN_RDn_C;   // PIN_79 -> P07.UJYV
  PinOut EXT_PIN_RDn_D;   // PIN_79 <- P08.URUN

  PinOut EXT_PIN_CSn_A;   // PIN_80 <- P08.TYHO

  PinOut EXT_PIN_A00_A;   // PIN_01 <- P08.KUPO
  PinIn  EXT_PIN_A00_C;   // PIN_01 -> P08.KOVA
  PinOut EXT_PIN_A00_D;   // PIN_01 <- P08.KOTY

  PinOut EXT_PIN_A01_A;   // PIN_02 <- P08.CABA
  PinIn  EXT_PIN_A01_C;   // PIN_02 -> P08.CAMU
  PinOut EXT_PIN_A01_D;   // PIN_02 <- P08.COTU

  PinOut EXT_PIN_A02_A;   // PIN_03 <- P08.BOKU
  PinIn  EXT_PIN_A02_C;   // PIN_03 -> P08.BUXU
  PinOut EXT_PIN_A02_D;   // PIN_03 <- P08.BAJO

  PinOut EXT_PIN_A03_A;   // PIN_04 <- P08.BOTY
  PinIn  EXT_PIN_A03_C;   // PIN_04 -> P08.BASE
  PinOut EXT_PIN_A03_D;   // PIN_04 <- P08.BOLA

  PinOut EXT_PIN_A04_A;   // PIN_05 <- P08.BYLA
  PinIn  EXT_PIN_A04_C;   // PIN_05 -> P08.AFEC
  PinOut EXT_PIN_A04_D;   // PIN_05 <- P08.BEVO

  PinOut EXT_PIN_A05_A;   // PIN_06 <- P08.BADU
  PinIn  EXT_PIN_A05_C;   // PIN_06 -> P08.ABUP
  PinOut EXT_PIN_A05_D;   // PIN_06 <- P08.AJAV

  PinOut EXT_PIN_A06_A;   // PIN_07 <- P08.CEPU
  PinIn  EXT_PIN_A06_C;   // PIN_07 -> P08.CYGU
  PinOut EXT_PIN_A06_D;   // PIN_07 <- P08.CYKA

  PinOut EXT_PIN_A07_A;   // PIN_08 <- P08.DEFY
  PinIn  EXT_PIN_A07_C;   // PIN_08 -> P08.COGO
  PinOut EXT_PIN_A07_D;   // PIN_08 <- P08.COLO

  PinOut EXT_PIN_A08_A;   // PIN_09 <- P08.MYNY
  PinIn  EXT_PIN_A08_C;   // PIN_09 -> P08.MUJY
  PinOut EXT_PIN_A08_D;   // PIN_09 <- P08.MEGO

  PinOut EXT_PIN_A09_A;   // PIN_10 <- P08.MUNE
  PinIn  EXT_PIN_A09_C;   // PIN_10 -> P08.NENA
  PinOut EXT_PIN_A09_D;   // PIN_10 <- P08.MENY

  PinOut EXT_PIN_A10_A;   // PIN_11 <- P08.ROXU
  PinIn  EXT_PIN_A10_C;   // PIN_11 -> P08.SURA
  PinOut EXT_PIN_A10_D;   // PIN_11 <- P08.RORE

  PinOut EXT_PIN_A11_A;   // PIN_12 <- P08.LEPY
  PinIn  EXT_PIN_A11_C;   // PIN_12 -> P08.MADY
  PinOut EXT_PIN_A11_D;   // PIN_12 <- P08.LYNY

  PinOut EXT_PIN_A12_A;   // PIN_13 <- P08.LUCE
  PinIn  EXT_PIN_A12_C;   // PIN_13 -> P08.LAHE
  PinOut EXT_PIN_A12_D;   // PIN_13 <- P08.LOSO

  PinOut EXT_PIN_A13_A;   // PIN_14 <- P08.LABE
  PinIn  EXT_PIN_A13_C;   // PIN_14 -> P08.LURA
  PinOut EXT_PIN_A13_D;   // PIN_14 <- P08.LEVA

  PinOut EXT_PIN_A14_A;   // PIN_15 <- P08.PUHE
  PinIn  EXT_PIN_A14_C;   // PIN_15 -> P08.PEVO
  PinOut EXT_PIN_A14_D;   // PIN_15 <- P08.PAHY

  PinOut EXT_PIN_A15_A;   // PIN_16 <- P08.SUZE
  PinIn  EXT_PIN_A15_C;   // PIN_16 -> P08.RAZA
  PinOut EXT_PIN_A15_D;   // PIN_16 <- P08.RULO

  PinOut EXT_PIN_D0_A;    // PIN_17 <- P08.RUXA
  PinOut EXT_PIN_D0_B;    // PIN_17 <- P08.LULA
  PinIn  EXT_PIN_D0_C;    // PIN_17 -> P08.TOVO,SOMA
  PinOut EXT_PIN_D0_D;    // PIN_17 <- P08.RUNE

  PinOut EXT_PIN_D1_A;    // PIN_18 <- P08.RUJA
  PinOut EXT_PIN_D1_B;    // PIN_18 <- P08.LULA
  PinIn  EXT_PIN_D1_C;    // PIN_18 -> P08.RUZY,RONY
  PinOut EXT_PIN_D1_D;    // PIN_18 <- P08.RYPU

  PinOut EXT_PIN_D2_A;    // PIN_19 <- P08.RABY
  PinOut EXT_PIN_D2_B;    // PIN_19 <- P08.LULA
  PinIn  EXT_PIN_D2_C;    // PIN_19 -> P08.ROME,RAXY
  PinOut EXT_PIN_D2_D;    // PIN_19 <- P08.SULY

  PinOut EXT_PIN_D3_A;    // PIN_20 <- P08.RERA
  PinOut EXT_PIN_D3_B;    // PIN_20 <- P08.LULA
  PinIn  EXT_PIN_D3_C;    // PIN_20 -> P08.SAZA,SELO
  PinOut EXT_PIN_D3_D;    // PIN_20 <- P08.SEZE

  PinOut EXT_PIN_D4_A;    // PIN_21 <- P08.RORY
  PinOut EXT_PIN_D4_B;    // PIN_21 <- P08.LULA
  PinIn  EXT_PIN_D4_C;    // PIN_21 -> P08.TEHE,SODY
  PinOut EXT_PIN_D4_D;    // PIN_21 <- P08.RESY

  PinOut EXT_PIN_D5_A;    // PIN_22 <- P08.RYVO
  PinOut EXT_PIN_D5_B;    // PIN_22 <- P08.LULA
  PinIn  EXT_PIN_D5_C;    // PIN_22 -> P08.RATU,SAGO
  PinOut EXT_PIN_D5_D;    // PIN_22 <- P08.TAMU

  PinOut EXT_PIN_D6_A;    // PIN_23 <- P08.RAFY
  PinOut EXT_PIN_D6_B;    // PIN_23 <- P08.LULA
  PinIn  EXT_PIN_D6_C;    // PIN_23 -> P08.SOCA,RUPA
  PinOut EXT_PIN_D6_D;    // PIN_23 <- P08.ROGY

  PinOut EXT_PIN_D7_A;    // PIN_24 <- P08.RAVU
  PinOut EXT_PIN_D7_B;    // PIN_24 <- P08.LULA
  PinIn  EXT_PIN_D7_C;    // PIN_24 -> P08.RYBA,SAZY
  PinOut EXT_PIN_D7_D;    // PIN_24 <- P08.RYDA

  //-----------------------------------------------------------------------------
  // VRAM bus

  PinOut VRAM_PIN_MCSn_A;   // PIN_43 <- P25.SOKY
  PinIn  VRAM_PIN_MCSn_C;   // PIN_43 -> P25.TEFY
  PinOut VRAM_PIN_MCSn_D;   // PIN_43 <- P25.SETY

  PinOut VRAM_PIN_MOEn_A;   // PIN_45 <- P25.REFO
  PinIn  VRAM_PIN_MOEn_C;   // PIN_45 -> P25.TAVY
  PinOut VRAM_PIN_MOEn_D;   // PIN_45 <- P25.SAHA

  PinOut VRAM_PIN_MWRn_A;   // PIN_49 <- P25.SYSY
  PinIn  VRAM_PIN_MWRn_C;   // PIN_49 -> P25.SUDO
  PinOut VRAM_PIN_MWRn_D;   // PIN_49 <- P25.RAGU

  PinOut VRAM_PIN_MA00_AD;  // PIN_34 <- P04.ECAL
  PinOut VRAM_PIN_MA01_AD;  // PIN_35 <- P04.EGEZ
  PinOut VRAM_PIN_MA02_AD;  // PIN_36 <- P04.FUHE
  PinOut VRAM_PIN_MA03_AD;  // PIN_37 <- P04.FYZY
  PinOut VRAM_PIN_MA04_AD;  // PIN_38 <- P04.DAMU
  PinOut VRAM_PIN_MA05_AD;  // PIN_39 <- P04.DAVA
  PinOut VRAM_PIN_MA06_AD;  // PIN_40 <- P04.ETEG
  PinOut VRAM_PIN_MA07_AD;  // PIN_41 <- P04.EREW
  PinOut VRAM_PIN_MA08_AD;  // PIN_48 <- P04.EVAX
  PinOut VRAM_PIN_MA09_AD;  // PIN_47 <- P04.DUVE
  PinOut VRAM_PIN_MA10_AD;  // PIN_44 <- P04.ERAF
  PinOut VRAM_PIN_MA11_AD;  // PIN_46 <- P04.FUSY
  PinOut VRAM_PIN_MA12_AD;  // PIN_42 <- P04.EXYF

  PinOut VRAM_PIN_MD0_A;    // PIN_33 <- P25.REGE
  PinOut VRAM_PIN_MD1_A;    // PIN_31 <- P25.RYKY
  PinOut VRAM_PIN_MD2_A;    // PIN_30 <- P25.RAZO
  PinOut VRAM_PIN_MD3_A;    // PIN_29 <- P25.RADA
  PinOut VRAM_PIN_MD4_A;    // PIN_28 <- P25.RYRO
  PinOut VRAM_PIN_MD5_A;    // PIN_27 <- P25.REVU
  PinOut VRAM_PIN_MD6_A;    // PIN_26 <- P25.REKU
  PinOut VRAM_PIN_MD7_A;    // PIN_25 <- P25.RYZE

  PinOut VRAM_PIN_MD0_B;    // PIN_33 <- P25.ROFA
  PinOut VRAM_PIN_MD1_B;    // PIN_31 <- P25.ROFA
  PinOut VRAM_PIN_MD2_B;    // PIN_30 <- P25.ROFA
  PinOut VRAM_PIN_MD3_B;    // PIN_29 <- P25.ROFA
  PinOut VRAM_PIN_MD4_B;    // PIN_28 <- P25.ROFA
  PinOut VRAM_PIN_MD5_B;    // PIN_27 <- P25.ROFA
  PinOut VRAM_PIN_MD6_B;    // PIN_26 <- P25.ROFA
  PinOut VRAM_PIN_MD7_B;    // PIN_25 <- P25.ROFA

  PinIn  VRAM_PIN_MD0_C;    // PIN_33 -> P25.RODY
  PinIn  VRAM_PIN_MD1_C;    // PIN_31 -> P25.REBA
  PinIn  VRAM_PIN_MD2_C;    // PIN_30 -> P25.RYDO
  PinIn  VRAM_PIN_MD3_C;    // PIN_29 -> P25.REMO
  PinIn  VRAM_PIN_MD4_C;    // PIN_28 -> P25.ROCE
  PinIn  VRAM_PIN_MD5_C;    // PIN_27 -> P25.ROPU
  PinIn  VRAM_PIN_MD6_C;    // PIN_26 -> P25.RETA
  PinIn  VRAM_PIN_MD7_C;    // PIN_25 -> P25.RAKU

  PinOut VRAM_PIN_MD0_D;    // PIN_33 <- P25.RURA
  PinOut VRAM_PIN_MD1_D;    // PIN_31 <- P25.RULY
  PinOut VRAM_PIN_MD2_D;    // PIN_30 <- P25.RARE
  PinOut VRAM_PIN_MD3_D;    // PIN_29 <- P25.RODU
  PinOut VRAM_PIN_MD4_D;    // PIN_28 <- P25.RUBE
  PinOut VRAM_PIN_MD5_D;    // PIN_27 <- P25.RUMU
  PinOut VRAM_PIN_MD6_D;    // PIN_26 <- P25.RYTY
  PinOut VRAM_PIN_MD7_D;    // PIN_25 <- P25.RADY

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

  //-----------------------------------------------------------------------------
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

  //-----------------------------------------------------------------------------
  // Sub-modules

//private:

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
  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;
  BusMux bus_mux;
  Bootrom bootrom;
};

//-----------------------------------------------------------------------------

};