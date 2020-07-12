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

  SchematicTop();

  void set_cpu_bus(Req req);
  void set_buttons(uint8_t buttons);
  void set_vram_bus(uint8_t data);
  void set_oam_bus(wire OE, uint16_t data);
  void set_ext_bus(uint16_t data);

  int get_addr() const {
    return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03,
                CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
                CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11,
                CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
  }

  int get_data() const {
    return pack(CPU_TRI_D0, CPU_TRI_D1, CPU_TRI_D2, CPU_TRI_D3,
                CPU_TRI_D4, CPU_TRI_D5, CPU_TRI_D6, CPU_TRI_D7);
  }

  /*
  void set_data(bool oe, uint8_t data) {
    CPU_TRI_D0.set_tribuf(oe, data & 0x01);
    CPU_TRI_D1.set_tribuf(oe, data & 0x02);
    CPU_TRI_D2.set_tribuf(oe, data & 0x04);
    CPU_TRI_D3.set_tribuf(oe, data & 0x08);
    CPU_TRI_D4.set_tribuf(oe, data & 0x10);
    CPU_TRI_D5.set_tribuf(oe, data & 0x20);
    CPU_TRI_D6.set_tribuf(oe, data & 0x40);
    CPU_TRI_D7.set_tribuf(oe, data & 0x80);
  }
  */

  SignalHash tick();

  //-----------------------------------------------------------------------------

  void tick_vram_pins();

  wire ABUZ_CPU_ADDR_VALIDp() const {
    /*p??.APAP*/ wire APAP_CPU_ADDR_VALIDp = not(CPU_PIN_ADDR_VALID); // Missing from schematic
    /*p01.AWOD*/ wire AWOD_CPU_ADDR_VALIDn = nor(UNOR_MODE_DBG2p(), APAP_CPU_ADDR_VALIDp);
    /*p01.ABUZ*/ wire ABUZ_CPU_ADDR_VALIDp = not(AWOD_CPU_ADDR_VALIDn);
    return ABUZ_CPU_ADDR_VALIDp;
  }

  wire TUTU_ADDR_BOOTp() const {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not(BOOT_BITn());
    /*p07.TULO*/ wire TULO_ADDR_00XXp = nor(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09, CPU_PIN_A08);
    /*p07.TUTU*/ wire TUTU_ADDR_BOOTp = and (TERA_BOOT_BITp, TULO_ADDR_00XXp);
    return TUTU_ADDR_BOOTp;
  }

  wire LUFA_DMA_VRAM_RDp() const {
    /*p04.MUHO*/ wire MUHO_DMA_VRAM_RDn   = nand(MATU_DMA_RUNNINGp(), MUDA_DMA_SRC_VRAMp());
    /*p04.LUFA*/ wire LUFA_DMA_VRAM_RDp = not(MUHO_DMA_VRAM_RDn);
    return LUFA_DMA_VRAM_RDp;
  }

  wire XANE_VRAM_LOCKn() const {
    /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor(LUFA_DMA_VRAM_RDp(), XYMU_RENDERINGp()); // def nor
    return XANE_VRAM_LOCKn;
  }

  wire AJON_OAM_BUSY() const {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
    return AJON_OAM_BUSY;
  }


  wire LENA_BGW_VRM_RDp() const {
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp());
    /*p27.LENA*/ wire LENA_BGW_VRM_RDp = not(LUSU_BGW_VRAM_RDn);
    return LENA_BGW_VRM_RDp;
  }

  wire MOSU_WIN_MODE_TRIGp() const {
    /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WX_MATCHpe());
    /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not(NYFO_WIN_MODE_TRIGn);
    return MOSU_WIN_MODE_TRIGp;
  }

  /*p27.NYXU*/ wire NYXU_TILE_FETCHER_RSTn() const { return nor(AVAP_RENDER_START_RST(), MOSU_WIN_MODE_TRIGp(), TEVO_FINE_RSTp()); }

  /*p21.PURE*/ wire PURE_NEW_LINE_d0n() const { return not(RUTU_LINE_END()); }

  /*p21.SELA*/ wire SELA_LINE_END_Fp() const { return not(PURE_NEW_LINE_d0n()); }

  /*p01.UCOB*/ wire UCOB_CLKBADp() const { return not(SYS_PIN_CLK_A); }

  /*p29.TYTU*/ wire TYTU_SFETCH_S0n() const { return not(sprite_fetcher.TOXE_SFETCH_S0.q()); }

  /*p29.ABON*/ wire ABON_SPR_VRM_RDn() const { return not(TEXY_SPR_READ_VRAMp()); }

  /*p29.CEHA*/ wire CEHA_SCANNINGp() const { return not(sprite_scanner.CENO_SCANNINGp.qn()); }
  /*p28.BESU*/ wire BESU_SCANNINGp() const { return sprite_scanner.BESU_SCANNINGp.q(); }
  /*p29.CENO*/ wire CENO_SCANNINGqn() const { return sprite_scanner.CENO_SCANNINGp.q(); }

  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p() const { return not(BYHA_VID_LINE_TRIG_d4()); }

  //-----------------------------------------------------------------------------
  // Debug signals

  /*p07.UBET*/ wire UBET_T1p() const { return not(SYS_PIN_T1n); }
  /*p07.UVAR*/ wire UVAR_T2p() const { return not(SYS_PIN_T2n); }

  /*p07.UMUT*/ wire UMUT_MODE_DBG1p() const {
    return and(SYS_PIN_T1n, UVAR_T2p());
  }
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p() const {
    return and(SYS_PIN_T2n, UBET_T1p());
  }
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

  /*p01.AFUR*/ wire AFUR_ABCDxxxx() const { return clk_reg.AFUR_ABCDxxxx.q(); }
  /*p01.ALEF*/ wire ALEF_xBCDExxx() const { return clk_reg.ALEF_xBCDExxx.q(); }
  /*p01.APUK*/ wire APUK_xxCDEFxx() const { return clk_reg.APUK_xxCDEFxx.q(); }
  /*p01.ADYK*/ wire ADYK_xxxDEFGx() const { return clk_reg.ADYK_xxxDEFGx.q(); }

  /*p01.ATYP*/ wire ATYP_ABCDxxxx() const { return not(!AFUR_ABCDxxxx()); }
  /*p01.AFEP*/ wire AFEP_AxxxxFGH() const { return not( ALEF_xBCDExxx()); }
  /*p01.AROV*/ wire AROV_xxCDEFxx() const { return not(!APUK_xxCDEFxx()); }
  /*p01.ADAR*/ wire ADAR_ABCxxxxH() const { return not( ADYK_xxxDEFGx()); }

  //----------------------------------------
  // Video chains

  /*p29.WUVU*/ wire WUVU_xxCDxxGH() const {
    return clk_reg.WUVU_xxCDxxGH.q();
  }
  /*p21.VENA*/ wire VENA_xxxxEFGH() const { return clk_reg.VENA_xxxxEFGH.q(); }
  /*p29.WOSU*/ wire WOSU_xBCxxFGx() const { return clk_reg.WOSU_xBCxxFGx.q(); }

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

  wire UVYN_DIV_05n() const    { return not(tim_reg.TAMA_DIV_05.q()); }
  wire UMEK_DIV_06n() const    { return not(tim_reg.UGOT_DIV_06.q()); }
  wire UREK_DIV_07n() const    { return not(tim_reg.TULU_DIV_07.q()); }

  wire UKUP_DIV_00() const { return tim_reg.UKUP_DIV_00.q(); }
  wire UFOR_DIV_01() const { return tim_reg.UFOR_DIV_01.q(); }
  wire UNER_DIV_02() const { return tim_reg.UNER_DIV_02.q(); }
  wire TERO_DIV_03() const { return tim_reg.TERO_DIV_03.q(); }
  wire UNYK_DIV_04() const { return tim_reg.UNYK_DIV_04.q(); }
  wire TERU_DIV_10() const { return tim_reg.TERU_DIV_10.q(); }
  wire UPOF_DIV_15() const { return tim_reg.UPOF_DIV_15.q(); }

  wire MOBA_INT_TIMERp() const { return tim_reg.MOBA_INT_TIMERp.q(); }

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

  /*p04.DECY*/ wire DECY_FROM_CPU5n() const { return not(CPU_PIN5); }

  wire LEKO_CPU_RDp() const {
    /*p04.CATY*/ wire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n());
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RDp(), CATY_FROM_CPU5p);
    /*p28.LEKO*/ wire LEKO_CPU_RDp = not(MYNU_CPU_RDn);
    return LEKO_CPU_RDp;
  }

  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH() const;
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx() const { return not(AREV_CPU_WRn_ABCDxxxH()); }

  /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH() const;
  /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx() const { return not(UBAL_CPU_WRn_ABCDxxxH()); }
  /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCDxxxH() const { return not(TAPU_CPU_WRp_xxxxEFGx()); }
  /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx() const { return not(DYKY_CPU_WRn_ABCDxxxH()); }

  //-----------------------------------------------------------------------------
  // Bus mux signals

  /*p31.YLOR*/ wire YLOR_SPRITE_X0() const { return bus_mux.YLOR_SPRITE_X0.q(); }
  /*p31.ZYTY*/ wire ZYTY_SPRITE_X1() const { return bus_mux.ZYTY_SPRITE_X1.q(); }
  /*p31.ZYVE*/ wire ZYVE_SPRITE_X2() const { return bus_mux.ZYVE_SPRITE_X2.q(); }
  /*p31.ZEZY*/ wire ZEZY_SPRITE_X3() const { return bus_mux.ZEZY_SPRITE_X3.q(); }
  /*p31.GOMO*/ wire GOMO_SPRITE_X4() const { return bus_mux.GOMO_SPRITE_X4.q(); }
  /*p31.BAXO*/ wire BAXO_SPRITE_X5() const { return bus_mux.BAXO_SPRITE_X5.q(); }
  /*p31.YZOS*/ wire YZOS_SPRITE_X6() const { return bus_mux.YZOS_SPRITE_X6.q(); }
  /*p31.DEPO*/ wire DEPO_SPRITE_X7() const { return bus_mux.DEPO_SPRITE_X7.q(); }

  /*p29.XUSO*/ wire XUSO_SPRITE_Y0() const { return bus_mux.XUSO_SPRITE_Y0.q(); }
  /*p29.XEGU*/ wire XEGU_SPRITE_Y1() const { return bus_mux.XEGU_SPRITE_Y1.q(); }
  /*p29.YJEX*/ wire YJEX_SPRITE_Y2() const { return bus_mux.YJEX_SPRITE_Y2.q(); }
  /*p29.XYJU*/ wire XYJU_SPRITE_Y3() const { return bus_mux.XYJU_SPRITE_Y3.q(); }
  /*p29.YBOG*/ wire YBOG_SPRITE_Y4() const { return bus_mux.YBOG_SPRITE_Y4.q(); }
  /*p29.WYSO*/ wire WYSO_SPRITE_Y5() const { return bus_mux.WYSO_SPRITE_Y5.q(); }
  /*p29.XOTE*/ wire XOTE_SPRITE_Y6() const { return bus_mux.XOTE_SPRITE_Y6.q(); }
  /*p29.YZAB*/ wire YZAB_SPRITE_Y7() const { return bus_mux.YZAB_SPRITE_Y7.q(); }

  //-----------------------------------------------------------------------------
  // DMA signals

  /*p04.MATU*/ wire MATU_DMA_RUNNINGp() const;
  /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp() const;

  /*p04.NAKY*/ wire DMA_A00() const { return dma_reg.DMA_A00.q(); }
  /*p04.PYRO*/ wire DMA_A01() const { return dma_reg.DMA_A01.q(); }
  /*p04.NEFY*/ wire DMA_A02() const { return dma_reg.DMA_A02.q(); }
  /*p04.MUTY*/ wire DMA_A03() const { return dma_reg.DMA_A03.q(); }
  /*p04.NYKO*/ wire DMA_A04() const { return dma_reg.DMA_A04.q(); }
  /*p04.PYLO*/ wire DMA_A05() const { return dma_reg.DMA_A05.q(); }
  /*p04.NUTO*/ wire DMA_A06() const { return dma_reg.DMA_A06.q(); }
  /*p04.MUGU*/ wire DMA_A07() const { return dma_reg.DMA_A07.q(); }
  /*p04.NAFA*/ wire DMA_A08() const { return dma_reg.DMA_A08.q(); }
  /*p04.PYNE*/ wire DMA_A09() const { return dma_reg.DMA_A09.q(); }
  /*p04.PARA*/ wire DMA_A10() const { return dma_reg.DMA_A10.q(); }
  /*p04.NYDO*/ wire DMA_A11() const { return dma_reg.DMA_A11.q(); }
  /*p04.NYGY*/ wire DMA_A12() const { return dma_reg.DMA_A12.q(); }
  /*p04.PULA*/ wire DMA_A13() const { return dma_reg.DMA_A13.q(); }
  /*p04.POKU*/ wire DMA_A14() const { return dma_reg.DMA_A14.q(); }
  /*p04.MARU*/ wire DMA_A15() const { return dma_reg.DMA_A15.q(); }

  //-----------------------------------------------------------------------------
  // LCD signals

  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4() const;

  /*p21.RUTU*/ wire RUTU_LINE_END()       const { return lcd_reg.RUTU_LINE_END.q(); }
  /*p29.CATU*/ wire CATU_LINE_END()       const { return lcd_reg.CATU_LINE_END.q(); }
  /*p21.ROPO*/ wire ROPO_LY_MATCH_SYNCp() const { return lcd_reg.ROPO_LY_MATCH_SYNCp.q(); }
  /*p21.POPU*/ wire POPU_VBLANKp_d4()     const { return lcd_reg.POPU_VBLANKp_d4.q(); }
  /*p21.PARU*/ wire PARU_VBLANKp_d4()     const { return not(lcd_reg.POPU_VBLANKp_d4.qn()); }
  /*p21.TOLU*/ wire TOLU_VBLANKn()        const { return not(PARU_VBLANKp_d4()); }


  /*p21.MUWY*/ wire MUWY_Y0() const { return lcd_reg.MUWY_Y0.q(); }
  /*p21.MYRO*/ wire MYRO_Y1() const { return lcd_reg.MYRO_Y1.q(); }
  /*p21.LEXA*/ wire LEXA_Y2() const { return lcd_reg.LEXA_Y2.q(); }
  /*p21.LYDO*/ wire LYDO_Y3() const { return lcd_reg.LYDO_Y3.q(); }
  /*p21.LOVU*/ wire LOVU_Y4() const { return lcd_reg.LOVU_Y4.q(); }
  /*p21.LEMA*/ wire LEMA_Y5() const { return lcd_reg.LEMA_Y5.q(); }
  /*p21.MATO*/ wire MATO_Y6() const { return lcd_reg.MATO_Y6.q(); }
  /*p21.LAFO*/ wire LAFO_Y7() const { return lcd_reg.LAFO_Y7.q(); }

  //-----------------------------------------------------------------------------
  // Address decoder signals

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

  /*p07.TUNA*/ wire TUNA_0000_FDFFp() const;
  /*p22.WERO*/ wire WERO_FF4Xp() const;

  /*p07.SYKE*/ wire SYKE_FF00_FFFFp() const { return nor(TUNA_0000_FDFFp(), TONA_A08n()); }
  /*p06.SARE*/ wire SARE_XX00_XX07p() const { return nor(CPU_PIN_A07, CPU_PIN_A06, CPU_PIN_A05, CPU_PIN_A04, CPU_PIN_A03); }

  //-----------------------------------------------------------------------------
  // PPU signals

  wire XYMU_RENDERINGp() const;
  /*p24.LOBY*/ wire LOBY_RENDERINGn() const { return not(XYMU_RENDERINGp()); }
  /*p25.ROPY*/ wire ROPY_RENDERINGn() const { return not(XYMU_RENDERINGp()); }


  wire WODU_RENDER_DONEp() const;
  wire TEVO_FINE_RSTp() const;
  wire ACYL_SCANNINGp() const;
  wire SEGU_CLKPIPEn() const;
  wire SACU_CLKPIPEp() const;
  wire ROZE_FINE_COUNT_7n() const;
  wire VOTY_INT_STATp() const;
  wire VEKU_SFETCH_RUNNING_RSTn() const;

  /*p21.XEHO*/ wire XEHO_X0() const { return ppu_reg.XEHO_X0.q(); }
  /*p21.SAVY*/ wire SAVY_X1() const { return ppu_reg.SAVY_X1.q(); }
  /*p21.XODU*/ wire XODU_X2() const { return ppu_reg.XODU_X2.q(); }
  /*p21.XYDO*/ wire XYDO_X3() const { return ppu_reg.XYDO_X3.q(); }
  /*p21.TUHU*/ wire TUHU_X4() const { return ppu_reg.TUHU_X4.q(); }
  /*p21.TUKY*/ wire TUKY_X5() const { return ppu_reg.TUKY_X5.q(); }
  /*p21.TAKO*/ wire TAKO_X6() const { return ppu_reg.TAKO_X6.q(); }
  /*p21.SYBE*/ wire SYBE_X7() const { return ppu_reg.SYBE_X7.q(); }

  //-----------------------------------------------------------------------------
  // Tile fetcher signals

  wire PORY_TILE_FETCH_DONE_Bp() const;
  wire LONY_BG_READ_VRAM_LATCHp() const;

  wire DATY_SCX0() const;
  wire DUZU_SCX1() const;
  wire CYXU_SCX2() const;

  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn() const { return nand(tile_fetcher.LAXU_BFETCH_S0.q(), tile_fetcher.NYVA_BFETCH_S2.q(), NYXU_TILE_FETCHER_RSTn()); }
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp() const { return not(MOCE_BFETCH_DONEn()); }

  //-----------------------------------------------------------------------------
  // Sprite scanner signals

  /*p28.ANOM*/ wire ANOM_LINE_RSTn() const;
  /*p29.BALU*/ wire BALU_LINE_RSTp() const;

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

  /*p29.WUTY*/ wire WUTY_SPRITE_DONEp() const { return not(VUSA_SPRITE_DONEn()); }


  /*p33.PEFO*/ wire SPR_PIX_A0() const { return sprite_fetcher.SPR_PIX_A0.q(); }
  /*p33.ROKA*/ wire SPR_PIX_A1() const { return sprite_fetcher.SPR_PIX_A1.q(); }
  /*p33.MYTU*/ wire SPR_PIX_A2() const { return sprite_fetcher.SPR_PIX_A2.q(); }
  /*p33.RAMU*/ wire SPR_PIX_A3() const { return sprite_fetcher.SPR_PIX_A3.q(); }
  /*p33.SELE*/ wire SPR_PIX_A4() const { return sprite_fetcher.SPR_PIX_A4.q(); }
  /*p33.SUTO*/ wire SPR_PIX_A5() const { return sprite_fetcher.SPR_PIX_A5.q(); }
  /*p33.RAMA*/ wire SPR_PIX_A6() const { return sprite_fetcher.SPR_PIX_A6.q(); }
  /*p33.RYDU*/ wire SPR_PIX_A7() const { return sprite_fetcher.SPR_PIX_A7.q(); }

  /*p33.REWO*/ wire SPR_PIX_B0() const { return sprite_fetcher.SPR_PIX_B0.q(); }
  /*p33.PEBA*/ wire SPR_PIX_B1() const { return sprite_fetcher.SPR_PIX_B1.q(); }
  /*p33.MOFO*/ wire SPR_PIX_B2() const { return sprite_fetcher.SPR_PIX_B2.q(); }
  /*p33.PUDU*/ wire SPR_PIX_B3() const { return sprite_fetcher.SPR_PIX_B3.q(); }
  /*p33.SAJA*/ wire SPR_PIX_B4() const { return sprite_fetcher.SPR_PIX_B4.q(); }
  /*p33.SUNY*/ wire SPR_PIX_B5() const { return sprite_fetcher.SPR_PIX_B5.q(); }
  /*p33.SEMO*/ wire SPR_PIX_B6() const { return sprite_fetcher.SPR_PIX_B6.q(); }
  /*p33.SEGA*/ wire SPR_PIX_B7() const { return sprite_fetcher.SPR_PIX_B7.q(); }

  //-----------------------------------------------------------------------------
  // Window signals

  wire RYDY_WIN_FIRST_TILE_A() const;
  wire NOCU_WIN_MODEn() const;
  wire NUNY_WX_MATCHpe() const;

  wire PORE_WIN_MODEp() const {
    /*p27.PORE*/ wire PORE_WIN_MODEp = not(NOCU_WIN_MODEn());
    return PORE_WIN_MODEp;
  }

  /*p27.SYLO*/ wire SYLO_WIN_HITn() const { return not(win_reg.RYDY_WIN_FIRST_TILE_A.q()); }
  /*p24.TOMU*/ wire TOMU_WIN_HITp() const { return not(SYLO_WIN_HITn()); }

  /*p27.WYKA*/ wire WYKA_WIN_X3() const { return win_reg.WYKA_WIN_X3.q(); }
  /*p27.WODY*/ wire WODY_WIN_X4() const { return win_reg.WODY_WIN_X4.q(); }
  /*p27.WOBO*/ wire WOBO_WIN_X5() const { return win_reg.WOBO_WIN_X5.q(); }
  /*p27.WYKO*/ wire WYKO_WIN_X6() const { return win_reg.WYKO_WIN_X6.q(); }
  /*p27.XOLO*/ wire XOLO_WIN_X7() const { return win_reg.XOLO_WIN_X7.q(); }

  /*p27.VYNO*/ wire VYNO_WIN_Y0() const { return win_reg.VYNO_WIN_Y0.q(); }
  /*p27.VUJO*/ wire VUJO_WIN_Y1() const { return win_reg.VUJO_WIN_Y1.q(); }
  /*p27.VYMU*/ wire VYMU_WIN_Y2() const { return win_reg.VYMU_WIN_Y2.q(); }
  /*p27.TUFU*/ wire TUFU_WIN_Y3() const { return win_reg.TUFU_WIN_Y3.q(); }
  /*p27.TAXA*/ wire TAXA_WIN_Y4() const { return win_reg.TAXA_WIN_Y4.q(); }
  /*p27.TOZO*/ wire TOZO_WIN_Y5() const { return win_reg.TOZO_WIN_Y5.q(); }
  /*p27.TATE*/ wire TATE_WIN_Y6() const { return win_reg.TATE_WIN_Y6.q(); }
  /*p27.TEKE*/ wire TEKE_WIN_Y7() const { return win_reg.TEKE_WIN_Y7.q(); }

  //-----------------------------------------------------------------------------
  // Misc signals

  wire BOOT_BITn() const;
  wire ASOK_INT_JOYPADp() const;
  wire CALY_INT_SERIALp() const;

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
  ExtPinIn  SYS_PIN_CLK_A; // PIN_74 -> ATEZ, UCOB. Basically "clock good".
  ExtPinIn  SYS_PIN_CLK_B; // PIN_74 
  ExtPinIn  SYS_PIN_T2n;    // PIN_76, tied to 0 on board - but there's probably an implicit inverter
  ExtPinIn  SYS_PIN_T1n;    // PIN_77, tied to 0 on board - but there's probably an implicit inverter

  // Ground ties - VYPO, RUNY, WEFE, unlabeled cell between BONE and BUFY.

  wire GND = 0;
  wire VYPO_GND = 0;
  wire RUNY_GND = 0;
  wire WEFE_GND = 0;
  wire PESU_GND = 0;

  //-----------------------------------------------------------------------------
  // SOC-to-CPU control signals

  CpuPinOut CPU_PIN_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related reg
  CpuPinOut CPU_PIN_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  CpuPinOut CPU_PIN_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  CpuPinOut CPU_PIN_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  CpuPinOut CPU_PIN_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  CpuPinOut CPU_PIN_ADDR_HI;       // top right port PORTA_03: <- P25.SYRO // Not really sure why this is here

  // Clocks
  CpuPinOut CPU_PIN_BOWA_AxCDEFGH; // top left port PORTD_01: <- BOWA_AxCDEFGH // Blue clock - decoders, alu, some reset stuff
  CpuPinOut CPU_PIN_BEDO_xBxxxxxx; // top left port PORTD_02: <- BEDO_xBxxxxxx
  CpuPinOut CPU_PIN_BEKO_xBCDExxx; // top left port PORTD_03: <- BEKO_ABCDxxxx + BAVY connection not indicated on P01 - test pad 1
  CpuPinOut CPU_PIN_BUDE_AxxxxFGH; // top left port PORTD_04: <- BUDE_AxxxxFGH + BEVA
  CpuPinOut CPU_PIN_BOLO_xBCDEFGx; // top left port PORTD_05: <- BOLO_ABCDEFxx + BYDA? - test pad 2
  CpuPinOut CPU_PIN_BUKE_ABxxxxxH; // top left port PORTD_07: <- BUKE_ABxxxxxH
  CpuPinOut CPU_PIN_BOMA_xBxxxxxx; // top left port PORTD_08: <- BOMA_xBxxxxxx (RESET_CLK)
  CpuPinOut CPU_PIN_BOGA_AxCDEFGH; // top left port PORTD_09: <- BOGA_AxCDEFGH - test pad 3

  //-----------------------------------------------------------------------------
  // CPU-to-SOC control signals

  CpuPinIn  CPU_PIN_READYp;        // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  CpuPinIn  CPU_PIN6;              // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  CpuPinIn  CPU_PIN5;              // top left port PORTD_06: -> ANUJ (FROM_CPU5). Probably "DATA_VALIDn"

  // Main bus
  CpuPinIn  CPU_PIN_RDp;           // top right port PORTA_00: ->
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
                                
  Tribuf CPU_TRI_D0;            // bottom left port: <>
  Tribuf CPU_TRI_D1;            // bottom left port: <>
  Tribuf CPU_TRI_D2;            // bottom left port: <>
  Tribuf CPU_TRI_D3;            // bottom left port: <>
  Tribuf CPU_TRI_D4;            // bottom left port: <>
  Tribuf CPU_TRI_D5;            // bottom left port: <>
  Tribuf CPU_TRI_D6;            // bottom left port: <>
  Tribuf CPU_TRI_D7;            // bottom left port: <>

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
  // External bus

  ExtPinOut EXT_PIN_CLK;     // PIN_75 <- P01.BUDE/BEVA
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

  Tribuf SPR_TRI_INDX_0;
  Tribuf SPR_TRI_INDX_1;
  Tribuf SPR_TRI_INDX_2;
  Tribuf SPR_TRI_INDX_3;
  Tribuf SPR_TRI_INDX_4;
  Tribuf SPR_TRI_INDX_5;

  Tribuf SPR_TRI_LINE_0;
  Tribuf SPR_TRI_LINE_1;
  Tribuf SPR_TRI_LINE_2;
  Tribuf SPR_TRI_LINE_3;

  //-----------------------------------------------------------------------------
  // Top level registers

  /*p30.XADU*/ Reg13 XADU_SPRITE_INDX0;
  /*p30.XEDY*/ Reg13 XEDY_SPRITE_INDX1;
  /*p30.ZUZE*/ Reg13 ZUZE_SPRITE_INDX2;
  /*p30.XOBE*/ Reg13 XOBE_SPRITE_INDX3;
  /*p30.YDUF*/ Reg13 YDUF_SPRITE_INDX4;
  /*p30.XECU*/ Reg13 XECU_SPRITE_INDX5;

  // FF40 - LCDC (in top because it's used everywhere)
  /*p23.VYXE*/ Reg9 VYXE_LCDC_BGEN;
  /*p23.XYLO*/ Reg9 XYLO_LCDC_SPEN;
  /*p23.XYMO*/ Reg9 XYMO_LCDC_SPSIZE;
  /*p23.XAFO*/ Reg9 XAFO_LCDC_BGMAP;
  /*p23.WEXU*/ Reg9 WEXU_LCDC_BGTILE;
  /*p23.WYMO*/ Reg9 WYMO_LCDC_WINEN;
  /*p23.WOKY*/ Reg9 WOKY_LCDC_WINMAP;
  /*p23.XONA*/ Reg9 XONA_LCDC_EN;

  // Temp pix storage
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
  SpriteStoreRegisters sprite_store;
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