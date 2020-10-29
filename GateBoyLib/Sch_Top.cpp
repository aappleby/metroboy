#include "GateBoyLib/Sch_Top.h"
#include <memory.h>
#include <stdio.h>

#include "GateBoyLib/Probe.h"

using namespace Schematics;
extern const uint8_t DMG_ROM_bin[];

//#define FAST_BOOT

#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void SchematicTop::tock_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  wire GND = 0;
  wire WEFE_VCC = 1;
  wire VYPO = 1;

  RegBase::tick_running = true;

  //----------------------------------------
  // dma_reg.tick();

  /*#p28.BOGE*/ dma_reg.BOGE_DMA_RUNNINGn = not1(dma_reg.MATU_DMA_RUNNINGp.qp17());

  /*#p04.LEBU*/ wire _LEBU_DMA_A15n    = not1(dma_reg.MARU_DMA_A15n.qn07());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp   = nor3(dma_reg.PULA_DMA_A13n.qn07(), dma_reg.POKU_DMA_A14n.qn07(), _LEBU_DMA_A15n);
  /* p04.LOGO*/ wire _LOGO_DMA_VRAMn   = not1(_MUDA_DMA_VRAMp);
  /* p04.MORY*/ wire _MORY_DMA_CARTn   = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17(), _LOGO_DMA_VRAMn);
  /* p04.LUMA*/ dma_reg.LUMA_DMA_CARTp = not1(_MORY_DMA_CARTn);

  /* p04.MUHO*/ wire _MUHO_DMA_VRAMp   = nand2(dma_reg.MATU_DMA_RUNNINGp.qp17(), _MUDA_DMA_VRAMp);
  /* p04.LUFA*/ dma_reg.LUFA_DMA_VRAMp = not1(_MUHO_DMA_VRAMp);

  //----------------------------------------

  {
    /*p07.UBET*/ wire _UBET_T1p  = not1(T1n);
    /*p07.UVAR*/ wire _UVAR_T2p  = not1(T2n);
    /*p07.UMUT*/ UMUT_MODE_DBG1p = and2(T1n, _UVAR_T2p);
    /*p07.UNOR*/ UNOR_MODE_DBG2p = and2(T2n, _UBET_T1p);
    /*p07.UPOJ*/ UPOJ_MODE_PRODn = nand3(_UBET_T1p, _UVAR_T2p, RST);
    /*p25.TUTO*/ TUTO_DBG_VRAMp  = and2(UNOR_MODE_DBG2p, SOTO_DBG_VRAM.qn16());
  }

  /* p07.UJYV*/ wire _UJYV_CPU_RDn = mux2n(UNOR_MODE_DBG2p, ext_bus.PIN_EXT_RDn.qn(), cpu_bus.PIN_CPU_RDp.qp());
  /* p07.TEDO*/ TEDO_CPU_RDp = not1(_UJYV_CPU_RDn);
  /* p07.AJAS*/ AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
  /* p07.ASOT*/ ASOT_CPU_RDp = not1(AJAS_CPU_RDn);



  {
    /*#p01.AGUT*/ wire _AGUT_xxCDEFGH = or_and3(clk_reg.AROV_xxCDEFxx.qp(), clk_reg.AJAX_xxxxEFGH.qp(), cpu_bus.PIN_CPU_EXT_BUSp.qp());
    /*#p01.AWOD*/ wire _AWOD_ABxxxxxx = nor2(UNOR_MODE_DBG2p, _AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ ABUZ_xxCDEFGH = not1(_AWOD_ABxxxxxx);

    /*#p08.SORE*/ wire _SORE_0000_7FFFp = not1(cpu_bus.BUS_CPU_A15.qp());
    /*#p08.TEVY*/ wire _TEVY_8000_9FFFn = or3(cpu_bus.BUS_CPU_A13.qp(), cpu_bus.BUS_CPU_A14.qp(), _SORE_0000_7FFFp);
    /*#p08.TEXO*/ TEXO_ADDR_EXT_AND_NOT_VRAM = and2(cpu_bus.PIN_CPU_EXT_BUSp.qp(), _TEVY_8000_9FFFn);
  }

  {
    /* p04.DECY*/ wire _DECY_LATCH_EXTn = not1(cpu_bus.PIN_CPU_LATCH_EXT.qp());
    /* p04.CATY*/ CATY_LATCH_EXTp = not1(_DECY_LATCH_EXTn);

    /* p28.MYNU*/ wire _MYNU_CPU_RDn = nand2(ASOT_CPU_RDp, CATY_LATCH_EXTp);
    /* p28.LEKO*/ LEKO_CPU_RDp = not1(_MYNU_CPU_RDn);
  }

  //----------------------------------------
  //clk_reg.tick_slow(CLK, CLKGOOD, CPUREADY, top);

  /*#p01.AVOR*/ wire _AVOR_SYS_RSTp = or2(clk_reg.AFER_SYS_RSTp.qp13(), clk_reg.ASOL_POR_DONEn.qp04());
  /*#p01.ALUR*/ clk_reg.ALUR_SYS_RSTn = not1(_AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(clk_reg.ALUR_SYS_RSTn);
  /*#p01.CUNU*/ clk_reg.CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*#p01.XORE*/ clk_reg.XORE_SYS_RSTp = not1(clk_reg.CUNU_SYS_RSTn);
  /* p01.WESY*/ clk_reg.WESY_SYS_RSTn = not1(clk_reg.XORE_SYS_RSTp);

  /* p01.XEBE*/ wire _XEBE_SYS_RSTn = not1(clk_reg.XORE_SYS_RSTp);
  /* p01.XODO*/ wire _XODO_VID_RSTp = nand2(_XEBE_SYS_RSTn, pix_pipe.XONA_LCDC_LCDENn.qn08());
  /* p01.XAPO*/ clk_reg.XAPO_VID_RSTn = not1(_XODO_VID_RSTp);
  /*#p01.ATAR*/ clk_reg.ATAR_VID_RSTp = not1(clk_reg.XAPO_VID_RSTn);
  /*#p01.ABEZ*/ clk_reg.ABEZ_VID_RSTn = not1(clk_reg.ATAR_VID_RSTp);


  /* p01.ATEZ*/ wire _ATEZ_CLKBAD = not1(CLKGOOD);
  /* p01.UCOB*/ clk_reg.UCOB_CLKBADp = not1(CLKGOOD);
  /* p01.ABOL*/ wire _ABOL_CLKREQn = not1(CPUREADY);
  /*#p01.BUTY*/ wire _BUTY_CLKREQ = not1(_ABOL_CLKREQn);

  // ignoring the deglitcher here
  /*p01.ATAL*/ clk_reg.ATAL_xBxDxFxH = CLK;
  /*p01.AZOF*/ wire _AZOF_AxCxExGx = not1(clk_reg.ATAL_xBxDxFxH);
  /*p01.ZAXY*/ clk_reg.ZAXY_xBxDxFxH = not1(_AZOF_AxCxExGx);

  /*#p01.AROV*/ clk_reg.AROV_xxCDEFxx = not1(clk_reg.APUK_ABxxxxGH.qp09());
  /*#p01.AFEP*/ wire _AFEP_AxxxxFGH = not1(clk_reg.ALEF_AxxxxFGH.qn08());
  /*#p01.ATYP*/ clk_reg.ATYP_ABCDxxxx = not1(clk_reg.AFUR_xxxxEFGH.qp09());
  /*#p01.AJAX*/ clk_reg.AJAX_xxxxEFGH = not1(clk_reg.ATYP_ABCDxxxx.qp());

  /*#p01.BAPY*/ wire _BAPY_xxxxxxGH = nor3(_ABOL_CLKREQn, clk_reg.AROV_xxCDEFxx, clk_reg.ATYP_ABCDxxxx);

  /*#p01.BELU*/ wire _BELU_xxxxEFGH = nor2(clk_reg.ATYP_ABCDxxxx, _ABOL_CLKREQn);
  /*#p01.BYRY*/ wire _BYRY_ABCDxxxx = not1(_BELU_xxxxEFGH);
  /*#p01.BUDE*/ clk_reg.BUDE_xxxxEFGH = not1(_BYRY_ABCDxxxx);

  /*#p01.BERU*/ wire _BERU_ABCDEFxx = not1(_BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire _BUFA_xxxxxxGH = not1(_BERU_ABCDEFxx);
  /*#p01.BOLO*/ clk_reg.BOLO_ABCDEFxx = not1(_BUFA_xxxxxxGH);

  /*#p01.BEKO*/ clk_reg.BEKO_ABCDxxxx = not1(clk_reg.BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire _BEJA_xxxxEFGH = nand4(clk_reg.BOLO_ABCDEFxx, clk_reg.BOLO_ABCDEFxx, clk_reg.BEKO_ABCDxxxx, clk_reg.BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire _BANE_ABCDxxxx = not1(_BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire _BELO_xxxxEFGH = not1(_BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire _BAZE_ABCDxxxx = not1(_BELO_xxxxEFGH);

  /*#p01.BUTO*/ wire _BUTO_xBCDEFGH = nand3(_AFEP_AxxxxFGH, clk_reg.ATYP_ABCDxxxx, _BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire _BELE_Axxxxxxx = not1(_BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire _BYJU_Axxxxxxx = or2(_BELE_Axxxxxxx, _ATEZ_CLKBAD);

  /*#p01.BALY*/ clk_reg.BALY_xBCDEFGH = not1(_BYJU_Axxxxxxx);
  /* p01.BOGA*/ clk_reg.BOGA_Axxxxxxx = not1(clk_reg.BALY_xBCDEFGH);

  /*#p01.ADAR*/ clk_reg.ADAR_ABCxxxxH = not1(clk_reg.ADYK_ABCxxxxH.qn08());
  /*#p29.XUPY*/ clk_reg.XUPY_ABxxEFxx = not1(clk_reg.WUVU_ABxxEFxx.qn16());
  /*#p21.TALU*/ clk_reg.TALU_xxCDEFxx = not1(clk_reg.VENA_xxCDEFxx.qn16());
  /*#p29.XOCE*/ clk_reg.XOCE_xBCxxFGx = not1(clk_reg.WOSU_AxxDExxH.qp17());
  /*#p29.WOJO*/ clk_reg.WOJO_AxxxExxx = nor2(clk_reg.WOSU_AxxDExxH.qn16(), clk_reg.WUVU_ABxxEFxx.qn16());
  /*#p01.AFAS*/ clk_reg.AFAS_xxxxEFGx = nor2(clk_reg.ADAR_ABCxxxxH, clk_reg.ATYP_ABCDxxxx);

  /*#p01.ZEME*/ clk_reg.ZEME_AxCxExGx = not1(clk_reg.ZAXY_xBxDxFxH);
  /* p01.UVYT*/ clk_reg.UVYT_ABCDxxxx = not1(clk_reg.BUDE_xxxxEFGH);
  /* p04.MOPA*/ clk_reg.MOPA_xxxxEFGH = not1(clk_reg.UVYT_ABCDxxxx);
  /*#p01.ALET*/ clk_reg.ALET_xBxDxFxH = not1(clk_reg.ZEME_AxCxExGx);
  /*#p27.MYVO*/ clk_reg.MYVO_AxCxExGx = not1(clk_reg.ALET_xBxDxFxH);

  /*#p01.BUGO*/ wire _BUGO_xBCDExxx = not1(_AFEP_AxxxxFGH);
  /*#p01.BATE*/ wire _BATE_AxxxxxGH = nor3(_BUGO_xBCDExxx, clk_reg.AROV_xxCDEFxx, _ABOL_CLKREQn);
  /*#p01.BASU*/ wire _BASU_xBCDEFxx = not1(_BATE_AxxxxxGH);

  /*#p01.BUKE*/ clk_reg.BUKE_AxxxxxGH = not1(_BASU_xBCDEFxx);
  /*#p01.BOMA*/ clk_reg.BOMA_xBCDEFGH = not1(clk_reg.BOGA_Axxxxxxx);

  /*#p01.BUVU*/ wire _BUVU_Axxxxxxx = and2(clk_reg.BALY_xBCDEFGH, _BUTY_CLKREQ);
  /*#p01.BYXO*/ wire _BYXO_xBCDEFGH = not1(_BUVU_Axxxxxxx);
  /*#p01.BEDO*/ clk_reg.BEDO_Axxxxxxx = not1(_BYXO_xBCDEFGH);
  /*#p01.BOWA*/ clk_reg.BOWA_xBCDEFGH = not1(clk_reg.BEDO_Axxxxxxx);


  //------------------------------------------------------------------------------
  // lcd_reg.tick(top);

  // fires on P910 and P911
  /* p28.ABAF*/ wire _ABAF_LINE_P910n = not1(lcd_reg.CATU_LINE_P000.qp17());

  // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.BYHA*/ lcd_reg.BYHA_VID_LINE_END_TRIGn = or_and3(lcd_reg.ANEL_LINE_P002.qp17(), _ABAF_LINE_P910n, clk_reg.ABEZ_VID_RSTn);

  // fires on P910 and P911
  /* p28.ATEJ*/ lcd_reg.ATEJ_LINE_TRIGp = not1(lcd_reg.BYHA_VID_LINE_END_TRIGn);

  // -> interrupts, ppu
  /*#p21.PARU*/ lcd_reg.PARU_VBLANKp_d4 = not1(lcd_reg.POPU_IN_VBLANKp.qn16());

  /*#p21.TOLU*/ lcd_reg.TOLU_VBLANKn = not1(lcd_reg.PARU_VBLANKp_d4);
  /*#p21.VYPU*/ lcd_reg.VYPU_INT_VBLANKp = not1(lcd_reg.TOLU_VBLANKn);

  /*#p21.PURE*/ lcd_reg.PURE_LINE_P908n = not1(lcd_reg.RUTU_LINE_P910.qp17());
  /*#p21.SELA*/ lcd_reg.SELA_LINE_P908p = not1(lcd_reg.PURE_LINE_P908n);

  //------------------------------------------------------------------------------
  // sprite_scanner.tick()

  // this is using an adder as a subtracter by inverting the first input.

  /*#p29.EBOS*/ wire _EBOS_Y0n = not1(lcd_reg.MUWY_Y0p.qp17());
  /* p29.DASA*/ wire _DASA_Y1n = not1(lcd_reg.MYRO_Y1p.qp17());
  /* p29.FUKY*/ wire _FUKY_Y2n = not1(lcd_reg.LEXA_Y2p.qp17());
  /* p29.FUVE*/ wire _FUVE_Y3n = not1(lcd_reg.LYDO_Y3p.qp17());
  /* p29.FEPU*/ wire _FEPU_Y4n = not1(lcd_reg.LOVU_Y4p.qp17());
  /* p29.FOFA*/ wire _FOFA_Y5n = not1(lcd_reg.LEMA_Y5p.qp17());
  /* p29.FEMO*/ wire _FEMO_Y6n = not1(lcd_reg.MATO_Y6p.qp17());
  /* p29.GUSU*/ wire _GUSU_Y7n = not1(lcd_reg.LAFO_Y7p.qp17());

  /*p29.ERUC*/ wire _ERUC_YDIFF_S0 = add_s(_EBOS_Y0n, oam_bus.XUSO_OAM_DA0p.qp08(), GND);
  /*p29.ERUC*/ wire _ERUC_YDIFF_C0 = add_c(_EBOS_Y0n, oam_bus.XUSO_OAM_DA0p.qp08(), GND);
  /*p29.ENEF*/ wire _ENEF_YDIFF_S1 = add_s(_DASA_Y1n, oam_bus.XEGU_OAM_DA1p.qp08(), _ERUC_YDIFF_C0);
  /*p29.ENEF*/ wire _ENEF_YDIFF_C1 = add_c(_DASA_Y1n, oam_bus.XEGU_OAM_DA1p.qp08(), _ERUC_YDIFF_C0);
  /*p29.FECO*/ wire _FECO_YDIFF_S2 = add_s(_FUKY_Y2n, oam_bus.YJEX_OAM_DA2p.qp08(), _ENEF_YDIFF_C1);
  /*p29.FECO*/ wire _FECO_YDIFF_C2 = add_c(_FUKY_Y2n, oam_bus.YJEX_OAM_DA2p.qp08(), _ENEF_YDIFF_C1);
  /*p29.GYKY*/ wire _GYKY_YDIFF_S3 = add_s(_FUVE_Y3n, oam_bus.XYJU_OAM_DA3p.qp08(), _FECO_YDIFF_C2);
  /*p29.GYKY*/ wire _GYKY_YDIFF_C3 = add_c(_FUVE_Y3n, oam_bus.XYJU_OAM_DA3p.qp08(), _FECO_YDIFF_C2);
  /*p29.GOPU*/ wire _GOPU_YDIFF_S4 = add_s(_FEPU_Y4n, oam_bus.YBOG_OAM_DA4p.qp08(), _GYKY_YDIFF_C3);
  /*p29.GOPU*/ wire _GOPU_YDIFF_C4 = add_c(_FEPU_Y4n, oam_bus.YBOG_OAM_DA4p.qp08(), _GYKY_YDIFF_C3);
  /*p29.FUWA*/ wire _FUWA_YDIFF_S5 = add_s(_FOFA_Y5n, oam_bus.WYSO_OAM_DA5p.qp08(), _GOPU_YDIFF_C4);
  /*p29.FUWA*/ wire _FUWA_YDIFF_C5 = add_c(_FOFA_Y5n, oam_bus.WYSO_OAM_DA5p.qp08(), _GOPU_YDIFF_C4);
  /*p29.GOJU*/ wire _GOJU_YDIFF_S6 = add_s(_FEMO_Y6n, oam_bus.XOTE_OAM_DA6p.qp08(), _FUWA_YDIFF_C5);
  /*p29.GOJU*/ wire _GOJU_YDIFF_C6 = add_c(_FEMO_Y6n, oam_bus.XOTE_OAM_DA6p.qp08(), _FUWA_YDIFF_C5);
  /*p29.WUHU*/ wire _WUHU_YDIFF_S7 = add_s(_GUSU_Y7n, oam_bus.YZAB_OAM_DA7p.qp08(), _GOJU_YDIFF_C6);
  /*p29.WUHU*/ wire _WUHU_YDIFF_C7 = add_c(_GUSU_Y7n, oam_bus.YZAB_OAM_DA7p.qp08(), _GOJU_YDIFF_C6);

  /* p29.DEGE*/ sprite_scanner.DEGE_SPRITE_DELTA0 = not1(_ERUC_YDIFF_S0);
  /* p29.DABY*/ sprite_scanner.DABY_SPRITE_DELTA1 = not1(_ENEF_YDIFF_S1);
  /* p29.DABU*/ sprite_scanner.DABU_SPRITE_DELTA2 = not1(_FECO_YDIFF_S2);
  /* p29.GYSA*/ sprite_scanner.GYSA_SPRITE_DELTA3 = not1(_GYKY_YDIFF_S3);
  /* p29.GACE*/ sprite_scanner.GACE_SPRITE_DELTA4 = not1(_GOPU_YDIFF_S4);
  /* p29.GUVU*/ sprite_scanner.GUVU_SPRITE_DELTA5 = not1(_FUWA_YDIFF_S5);
  /* p29.GYDA*/ sprite_scanner.GYDA_SPRITE_DELTA6 = not1(_GOJU_YDIFF_S6);
  /* p29.GEWY*/ sprite_scanner.GEWY_SPRITE_DELTA7 = not1(_WUHU_YDIFF_S7);

  /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH  = or2(pix_pipe.XYMO_LCDC_SPSIZEn.qn08(), _GYKY_YDIFF_S3);
  /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn = nand6(sprite_scanner.GACE_SPRITE_DELTA4, sprite_scanner.GUVU_SPRITE_DELTA5, sprite_scanner.GYDA_SPRITE_DELTA6, sprite_scanner.GEWY_SPRITE_DELTA7, _WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH);
  /* p29.GESE*/ wire _GESE_SCAN_MATCH_Y  = not1(_WOTA_SCAN_MATCH_Yn);
  /* p29.CEHA*/ sprite_scanner.CEHA_SCANNINGp     = not1(sprite_scanner.CENO_SCANNINGp.qn16());

  /* p29.CARE*/ sprite_scanner.CARE_STORE_ENp_ABxxEFxx = and3(clk_reg.XOCE_xBCxxFGx, sprite_scanner.CEHA_SCANNINGp, _GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.

  /*#p28.ACYL*/ sprite_scanner.ACYL_SCANNINGp = and2(dma_reg.BOGE_DMA_RUNNINGn, sprite_scanner.BESU_SCANNINGp.qp04());

  /*#p28.ANOM*/ sprite_scanner.ANOM_LINE_RSTn = nor2(lcd_reg.ATEJ_LINE_TRIGp, clk_reg.ATAR_VID_RSTp);
  /*#p29.BALU*/ wire _BALU_LINE_RSTp = not1(sprite_scanner.ANOM_LINE_RSTn);
  /*#p29.BAGY*/ sprite_scanner.BAGY_LINE_RSTn = not1(_BALU_LINE_RSTp);

  /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_B.qp17(), _BALU_LINE_RSTp, sprite_scanner.BYBA_SCAN_DONE_A.qn16());
  /*#p29.AVAP*/ sprite_scanner.AVAP_RENDER_START_TRIGp = not1(_BEBU_SCAN_DONE_TRIGn);

  //----------------------------------------

  /*#p21.ACAM*/ wire _ACAM_X0n = not1(pix_pipe.XEHO_X0p.qp17());
  /* p21.AZUB*/ wire _AZUB_X1n = not1(pix_pipe.SAVY_X1p.qp17());
  /* p21.AMEL*/ wire _AMEL_X2n = not1(pix_pipe.XODU_X2p.qp17());
  /* p21.AHAL*/ wire _AHAL_X3n = not1(pix_pipe.XYDO_X3p.qp17());
  /* p21.APUX*/ wire _APUX_X4n = not1(pix_pipe.TUHU_X4p.qp17());
  /* p21.ABEF*/ wire _ABEF_X5n = not1(pix_pipe.TUKY_X5p.qp17());
  /* p21.ADAZ*/ wire _ADAZ_X6n = not1(pix_pipe.TAKO_X6p.qp17());
  /* p21.ASAH*/ wire _ASAH_X7n = not1(pix_pipe.SYBE_X7p.qp17());

  /*#p29.BYJO*/ wire _BYJO_SCANNINGn  = not1(sprite_scanner.CEHA_SCANNINGp);
  /*#p29.AZEM*/ wire _AZEM_RENDERINGp = and2(_BYJO_SCANNINGn, pix_pipe.XYMU_RENDERINGn.qn03());
  /*#p29.AROR*/ wire _AROR_MATCH_ENp  = and2(_AZEM_RENDERINGp, pix_pipe.XYLO_LCDC_SPENn.qn08());

  {
    /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn = and2(sprite_scanner.CENO_SCANNINGp.qn16(), pix_pipe.XYMU_RENDERINGn.qn03());
    /*#p30.WUZY*/ sprite_store.SPR_TRI_I0p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XADU_SPRITE_IDX0p.qn12());
    /* p30.WYSE*/ sprite_store.SPR_TRI_I1p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XEDY_SPRITE_IDX1p.qn12());
    /* p30.ZYSU*/ sprite_store.SPR_TRI_I2p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.ZUZE_SPRITE_IDX2p.qn12());
    /* p30.WYDA*/ sprite_store.SPR_TRI_I3p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XOBE_SPRITE_IDX3p.qn12());
    /* p30.WUCO*/ sprite_store.SPR_TRI_I4p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.YDUF_SPRITE_IDX4p.qn12());
    /* p30.WEZA*/ sprite_store.SPR_TRI_I5p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, sprite_scanner.XECU_SPRITE_IDX5p.qn12());
  }

  //----------------------------------------

  /*#p31.ZOGY*/ wire ZOGY_STORE0_MATCH0n = xor2(sprite_store.XEPE_STORE0_X0p.qn08(), _ACAM_X0n);
  /* p31.ZEBA*/ wire ZEBA_STORE0_MATCH1n = xor2(sprite_store.YLAH_STORE0_X1p.qn08(), _AZUB_X1n);
  /* p31.ZAHA*/ wire ZAHA_STORE0_MATCH2n = xor2(sprite_store.ZOLA_STORE0_X2p.qn08(), _AMEL_X2n);
  /* p31.ZOKY*/ wire ZOKY_STORE0_MATCH3n = xor2(sprite_store.ZULU_STORE0_X3p.qn08(), _AHAL_X3n);
  /* p31.WOJU*/ wire WOJU_STORE0_MATCH4n = xor2(sprite_store.WELO_STORE0_X4p.qn08(), _APUX_X4n);
  /* p31.YFUN*/ wire YFUN_STORE0_MATCH5n = xor2(sprite_store.XUNY_STORE0_X5p.qn08(), _ABEF_X5n);
  /* p31.WYZA*/ wire WYZA_STORE0_MATCH6n = xor2(sprite_store.WOTE_STORE0_X6p.qn08(), _ADAZ_X6n);
  /* p31.YPUK*/ wire YPUK_STORE0_MATCH7n = xor2(sprite_store.XAKO_STORE0_X7p.qn08(), _ASAH_X7n);

  /* p31.EDYM*/ wire EDYM_STORE1_MATCH0n = xor2(sprite_store.DANY_STORE1_X0p.qn08(), _ACAM_X0n);
  /* p31.EMYB*/ wire EMYB_STORE1_MATCH1n = xor2(sprite_store.DUKO_STORE1_X1p.qn08(), _AZUB_X1n);
  /* p31.EBEF*/ wire EBEF_STORE1_MATCH2n = xor2(sprite_store.DESU_STORE1_X2p.qn08(), _AMEL_X2n);
  /* p31.EWOK*/ wire EWOK_STORE1_MATCH3n = xor2(sprite_store.DAZO_STORE1_X3p.qn08(), _AHAL_X3n);
  /* p31.COLA*/ wire COLA_STORE1_MATCH4n = xor2(sprite_store.DAKE_STORE1_X4p.qn08(), _APUX_X4n);
  /* p31.BOBA*/ wire BOBA_STORE1_MATCH5n = xor2(sprite_store.CESO_STORE1_X5p.qn08(), _ABEF_X5n);
  /* p31.COLU*/ wire COLU_STORE1_MATCH6n = xor2(sprite_store.DYFU_STORE1_X6p.qn08(), _ADAZ_X6n);
  /* p31.BAHU*/ wire BAHU_STORE1_MATCH7n = xor2(sprite_store.CUSY_STORE1_X7p.qn08(), _ASAH_X7n);

  /* p31.FUZU*/ wire FUZU_STORE2_MATCH0n = xor2(sprite_store.FOKA_STORE2_X0p.qn08(), _ACAM_X0n);
  /* p31.FESO*/ wire FESO_STORE2_MATCH1n = xor2(sprite_store.FYTY_STORE2_X1p.qn08(), _AZUB_X1n);
  /* p31.FOKY*/ wire FOKY_STORE2_MATCH2n = xor2(sprite_store.FUBY_STORE2_X2p.qn08(), _AMEL_X2n);
  /* p31.FYVA*/ wire FYVA_STORE2_MATCH3n = xor2(sprite_store.GOXU_STORE2_X3p.qn08(), _AHAL_X3n);
  /* p31.CEKO*/ wire CEKO_STORE2_MATCH4n = xor2(sprite_store.DUHY_STORE2_X4p.qn08(), _APUX_X4n);
  /* p31.DETY*/ wire DETY_STORE2_MATCH5n = xor2(sprite_store.EJUF_STORE2_X5p.qn08(), _ABEF_X5n);
  /* p31.DOZO*/ wire DOZO_STORE2_MATCH6n = xor2(sprite_store.ENOR_STORE2_X6p.qn08(), _ADAZ_X6n);
  /* p31.CONY*/ wire CONY_STORE2_MATCH7n = xor2(sprite_store.DEPY_STORE2_X7p.qn08(), _ASAH_X7n);

  /* p31.YHOK*/ wire YHOK_STORE3_MATCH0n = xor2(sprite_store.XOLY_STORE3_X0p.qn08(), _ACAM_X0n);
  /* p31.YCAH*/ wire YCAH_STORE3_MATCH1n = xor2(sprite_store.XYBA_STORE3_X1p.qn08(), _AZUB_X1n);
  /* p31.YDAJ*/ wire YDAJ_STORE3_MATCH2n = xor2(sprite_store.XABE_STORE3_X2p.qn08(), _AMEL_X2n);
  /* p31.YVUZ*/ wire YVUZ_STORE3_MATCH3n = xor2(sprite_store.XEKA_STORE3_X3p.qn08(), _AHAL_X3n);
  /* p31.YVAP*/ wire YVAP_STORE3_MATCH4n = xor2(sprite_store.XOMY_STORE3_X4p.qn08(), _APUX_X4n);
  /* p31.XENY*/ wire XENY_STORE3_MATCH5n = xor2(sprite_store.WUHA_STORE3_X5p.qn08(), _ABEF_X5n);
  /* p31.XAVU*/ wire XAVU_STORE3_MATCH6n = xor2(sprite_store.WYNA_STORE3_X6p.qn08(), _ADAZ_X6n);
  /* p31.XEVA*/ wire XEVA_STORE3_MATCH7n = xor2(sprite_store.WECO_STORE3_X7p.qn08(), _ASAH_X7n);

  /* p31.XEJU*/ wire XEJU_STORE4_MATCH0n = xor2(sprite_store.WEDU_STORE4_X0p.qn08(), _ACAM_X0n);
  /* p31.ZATE*/ wire ZATE_STORE4_MATCH1n = xor2(sprite_store.YGAJ_STORE4_X1p.qn08(), _AZUB_X1n);
  /* p31.ZAKU*/ wire ZAKU_STORE4_MATCH2n = xor2(sprite_store.ZYJO_STORE4_X2p.qn08(), _AMEL_X2n);
  /* p31.YBOX*/ wire YBOX_STORE4_MATCH3n = xor2(sprite_store.XURY_STORE4_X3p.qn08(), _AHAL_X3n);
  /* p31.ZYKU*/ wire ZYKU_STORE4_MATCH4n = xor2(sprite_store.YBED_STORE4_X4p.qn08(), _APUX_X4n);
  /* p31.ZYPU*/ wire ZYPU_STORE4_MATCH5n = xor2(sprite_store.ZALA_STORE4_X5p.qn08(), _ABEF_X5n);
  /* p31.XAHA*/ wire XAHA_STORE4_MATCH6n = xor2(sprite_store.WYDE_STORE4_X6p.qn08(), _ADAZ_X6n);
  /* p31.ZEFE*/ wire ZEFE_STORE4_MATCH7n = xor2(sprite_store.XEPA_STORE4_X7p.qn08(), _ASAH_X7n);

  /* p31.GUZO*/ wire GUZO_STORE5_MATCH0n = xor2(sprite_store.FUSA_STORE5_X0p.qn08(), _ACAM_X0n);
  /* p31.GOLA*/ wire GOLA_STORE5_MATCH1n = xor2(sprite_store.FAXA_STORE5_X1p.qn08(), _AZUB_X1n);
  /* p31.GEVE*/ wire GEVE_STORE5_MATCH2n = xor2(sprite_store.FOZY_STORE5_X2p.qn08(), _AMEL_X2n);
  /* p31.GUDE*/ wire GUDE_STORE5_MATCH3n = xor2(sprite_store.FESY_STORE5_X3p.qn08(), _AHAL_X3n);
  /* p31.BAZY*/ wire BAZY_STORE5_MATCH4n = xor2(sprite_store.CYWE_STORE5_X4p.qn08(), _APUX_X4n);
  /* p31.CYLE*/ wire CYLE_STORE5_MATCH5n = xor2(sprite_store.DYBY_STORE5_X5p.qn08(), _ABEF_X5n);
  /* p31.CEVA*/ wire CEVA_STORE5_MATCH6n = xor2(sprite_store.DURY_STORE5_X6p.qn08(), _ADAZ_X6n);
  /* p31.BUMY*/ wire BUMY_STORE5_MATCH7n = xor2(sprite_store.CUVY_STORE5_X7p.qn08(), _ASAH_X7n);

  /* p31.XOSU*/ wire XOSU_STORE6_MATCH0n = xor2(sprite_store.YCOL_STORE6_X0p.qn08(), _ACAM_X0n);
  /* p31.ZUVU*/ wire ZUVU_STORE6_MATCH1n = xor2(sprite_store.YRAC_STORE6_X1p.qn08(), _AZUB_X1n);
  /* p31.XUCO*/ wire XUCO_STORE6_MATCH2n = xor2(sprite_store.YMEM_STORE6_X2p.qn08(), _AMEL_X2n);
  /* p31.ZULO*/ wire ZULO_STORE6_MATCH3n = xor2(sprite_store.YVAG_STORE6_X3p.qn08(), _AHAL_X3n);
  /* p31.ZARE*/ wire ZARE_STORE6_MATCH4n = xor2(sprite_store.ZOLY_STORE6_X4p.qn08(), _APUX_X4n);
  /* p31.ZEMU*/ wire ZEMU_STORE6_MATCH5n = xor2(sprite_store.ZOGO_STORE6_X5p.qn08(), _ABEF_X5n);
  /* p31.ZYGO*/ wire ZYGO_STORE6_MATCH6n = xor2(sprite_store.ZECU_STORE6_X6p.qn08(), _ADAZ_X6n);
  /* p31.ZUZY*/ wire ZUZY_STORE6_MATCH7n = xor2(sprite_store.ZESA_STORE6_X7p.qn08(), _ASAH_X7n);

  /* p31.DUSE*/ wire DUSE_STORE7_MATCH0n = xor2(sprite_store.ERAZ_STORE7_X0p.qn08(), _ACAM_X0n);
  /* p31.DAGU*/ wire DAGU_STORE7_MATCH1n = xor2(sprite_store.EPUM_STORE7_X1p.qn08(), _AZUB_X1n);
  /* p31.DYZE*/ wire DYZE_STORE7_MATCH2n = xor2(sprite_store.EROL_STORE7_X2p.qn08(), _AMEL_X2n);
  /* p31.DESO*/ wire DESO_STORE7_MATCH3n = xor2(sprite_store.EHYN_STORE7_X3p.qn08(), _AHAL_X3n);
  /* p31.EJOT*/ wire EJOT_STORE7_MATCH4n = xor2(sprite_store.FAZU_STORE7_X4p.qn08(), _APUX_X4n);
  /* p31.ESAJ*/ wire ESAJ_STORE7_MATCH5n = xor2(sprite_store.FAXE_STORE7_X5p.qn08(), _ABEF_X5n);
  /* p31.DUCU*/ wire DUCU_STORE7_MATCH6n = xor2(sprite_store.EXUK_STORE7_X6p.qn08(), _ADAZ_X6n);
  /* p31.EWUD*/ wire EWUD_STORE7_MATCH7n = xor2(sprite_store.FEDE_STORE7_X7p.qn08(), _ASAH_X7n);

  /* p31.DUZE*/ wire DUZE_STORE8_MATCH0n = xor2(sprite_store.EZUF_STORE8_X0p.qn08(), _APUX_X4n);
  /* p31.DAGA*/ wire DAGA_STORE8_MATCH1n = xor2(sprite_store.ENAD_STORE8_X1p.qn08(), _ABEF_X5n);
  /* p31.DAWU*/ wire DAWU_STORE8_MATCH2n = xor2(sprite_store.EBOW_STORE8_X2p.qn08(), _ADAZ_X6n);
  /* p31.EJAW*/ wire EJAW_STORE8_MATCH3n = xor2(sprite_store.FYCA_STORE8_X3p.qn08(), _ASAH_X7n);
  /* p31.GOHO*/ wire GOHO_STORE8_MATCH4n = xor2(sprite_store.GAVY_STORE8_X4p.qn08(), _ACAM_X0n);
  /* p31.GASU*/ wire GASU_STORE8_MATCH5n = xor2(sprite_store.GYPU_STORE8_X5p.qn08(), _AZUB_X1n);
  /* p31.GABU*/ wire GABU_STORE8_MATCH6n = xor2(sprite_store.GADY_STORE8_X6p.qn08(), _AMEL_X2n);
  /* p31.GAFE*/ wire GAFE_STORE8_MATCH7n = xor2(sprite_store.GAZA_STORE8_X7p.qn08(), _AHAL_X3n);

  /* p31.YMAM*/ wire YMAM_STORE9_MATCH0n = xor2(sprite_store.XUVY_STORE9_X0p.qn08(), _ACAM_X0n);
  /* p31.YTYP*/ wire YTYP_STORE9_MATCH1n = xor2(sprite_store.XERE_STORE9_X1p.qn08(), _AZUB_X1n);
  /* p31.YFOP*/ wire YFOP_STORE9_MATCH2n = xor2(sprite_store.XUZO_STORE9_X2p.qn08(), _AMEL_X2n);
  /* p31.YVAC*/ wire YVAC_STORE9_MATCH3n = xor2(sprite_store.XEXA_STORE9_X3p.qn08(), _AHAL_X3n);
  /* p31.ZYWU*/ wire ZYWU_STORE9_MATCH4n = xor2(sprite_store.YPOD_STORE9_X4p.qn08(), _APUX_X4n);
  /* p31.ZUZA*/ wire ZUZA_STORE9_MATCH5n = xor2(sprite_store.YROP_STORE9_X5p.qn08(), _ABEF_X5n);
  /* p31.ZEJO*/ wire ZEJO_STORE9_MATCH6n = xor2(sprite_store.YNEP_STORE9_X6p.qn08(), _ADAZ_X6n);
  /* p31.ZEDA*/ wire ZEDA_STORE9_MATCH7n = xor2(sprite_store.YZOF_STORE9_X7p.qn08(), _ASAH_X7n);

  /* p31.ZAKO*/ wire ZAKO_STORE0_MATCHAp = nor4(ZOGY_STORE0_MATCH0n, ZEBA_STORE0_MATCH1n, ZAHA_STORE0_MATCH2n, ZOKY_STORE0_MATCH3n);
  /* p31.XEBA*/ wire XEBA_STORE0_MATCHBp = nor4(WOJU_STORE0_MATCH4n, YFUN_STORE0_MATCH5n, WYZA_STORE0_MATCH6n, YPUK_STORE0_MATCH7n);
  /* p31.CYVY*/ wire CYVY_STORE1_MATCHBp = nor4(COLA_STORE1_MATCH4n, BOBA_STORE1_MATCH5n, COLU_STORE1_MATCH6n, BAHU_STORE1_MATCH7n);
  /* p31.EWAM*/ wire EWAM_STORE1_MATCHAp = nor4(EDYM_STORE1_MATCH0n, EMYB_STORE1_MATCH1n, EBEF_STORE1_MATCH2n, EWOK_STORE1_MATCH3n);
  /* p31.CEHU*/ wire CEHU_STORE2_MATCHAp = nor4(CEKO_STORE2_MATCH4n, DETY_STORE2_MATCH5n, DOZO_STORE2_MATCH6n, CONY_STORE2_MATCH7n);
  /* p31.EKES*/ wire EKES_STORE2_MATCHBp = nor4(FUZU_STORE2_MATCH0n, FESO_STORE2_MATCH1n, FOKY_STORE2_MATCH2n, FYVA_STORE2_MATCH3n);
  /* p31.ZURE*/ wire ZURE_STORE3_MATCHAp = nor4(YHOK_STORE3_MATCH0n, YCAH_STORE3_MATCH1n, YDAJ_STORE3_MATCH2n, YVUZ_STORE3_MATCH3n);
  /* p31.YWOS*/ wire YWOS_STORE3_MATCHBp = nor4(YVAP_STORE3_MATCH4n, XENY_STORE3_MATCH5n, XAVU_STORE3_MATCH6n, XEVA_STORE3_MATCH7n);
  /* p31.YKOK*/ wire YKOK_STORE4_MATCHAp = nor4(XEJU_STORE4_MATCH0n, ZATE_STORE4_MATCH1n, ZAKU_STORE4_MATCH2n, YBOX_STORE4_MATCH3n);
  /* p31.YNAZ*/ wire YNAZ_STORE4_MATCHBp = nor4(ZYKU_STORE4_MATCH4n, ZYPU_STORE4_MATCH5n, XAHA_STORE4_MATCH6n, ZEFE_STORE4_MATCH7n);
  /* p31.COGY*/ wire COGY_STORE5_MATCHAp = nor4(BAZY_STORE5_MATCH4n, CYLE_STORE5_MATCH5n, CEVA_STORE5_MATCH6n, BUMY_STORE5_MATCH7n);
  /* p31.FYMA*/ wire FYMA_STORE5_MATCHBp = nor4(GUZO_STORE5_MATCH0n, GOLA_STORE5_MATCH1n, GEVE_STORE5_MATCH2n, GUDE_STORE5_MATCH3n);
  /* p31.YWAP*/ wire YWAP_STORE6_MATCHAp = nor4(ZARE_STORE6_MATCH4n, ZEMU_STORE6_MATCH5n, ZYGO_STORE6_MATCH6n, ZUZY_STORE6_MATCH7n);
  /* p31.YDOT*/ wire YDOT_STORE6_MATCHBp = nor4(XOSU_STORE6_MATCH0n, ZUVU_STORE6_MATCH1n, XUCO_STORE6_MATCH2n, ZULO_STORE6_MATCH3n);
  /* p31.CYCO*/ wire CYCO_STORE7_MATCHAp = nor4(DUSE_STORE7_MATCH0n, DAGU_STORE7_MATCH1n, DYZE_STORE7_MATCH2n, DESO_STORE7_MATCH3n);
  /* p31.DAJE*/ wire DAJE_STORE7_MATCHBp = nor4(EJOT_STORE7_MATCH4n, ESAJ_STORE7_MATCH5n, DUCU_STORE7_MATCH6n, EWUD_STORE7_MATCH7n);
  /* p31.DAMA*/ wire DAMA_STORE8_MATCHAp = nor4(DUZE_STORE8_MATCH0n, DAGA_STORE8_MATCH1n, DAWU_STORE8_MATCH2n, EJAW_STORE8_MATCH3n);
  /* p31.FEHA*/ wire FEHA_STORE8_MATCHBp = nor4(GOHO_STORE8_MATCH4n, GASU_STORE8_MATCH5n, GABU_STORE8_MATCH6n, GAFE_STORE8_MATCH7n);
  /* p31.YLEV*/ wire YLEV_STORE9_MATCHAp = nor4(YMAM_STORE9_MATCH0n, YTYP_STORE9_MATCH1n, YFOP_STORE9_MATCH2n, YVAC_STORE9_MATCH3n);
  /* p31.YTUB*/ wire YTUB_STORE9_MATCHBp = nor4(ZYWU_STORE9_MATCH4n, ZUZA_STORE9_MATCH5n, ZEJO_STORE9_MATCH6n, ZEDA_STORE9_MATCH7n);

  /* p29.YDUG*/ sprite_store.YDUG_STORE0_MATCHn = nand3(_AROR_MATCH_ENp, ZAKO_STORE0_MATCHAp, XEBA_STORE0_MATCHBp);
  /* p29.DYDU*/ sprite_store.DYDU_STORE1_MATCHn = nand3(_AROR_MATCH_ENp, EWAM_STORE1_MATCHAp, CYVY_STORE1_MATCHBp);
  /* p29.DEGO*/ sprite_store.DEGO_STORE2_MATCHn = nand3(_AROR_MATCH_ENp, CEHU_STORE2_MATCHAp, EKES_STORE2_MATCHBp);
  /* p29.YLOZ*/ sprite_store.YLOZ_STORE3_MATCHn = nand3(_AROR_MATCH_ENp, ZURE_STORE3_MATCHAp, YWOS_STORE3_MATCHBp);
  /* p29.XAGE*/ sprite_store.XAGE_STORE4_MATCHn = nand3(_AROR_MATCH_ENp, YKOK_STORE4_MATCHAp, YNAZ_STORE4_MATCHBp);
  /* p29.EGOM*/ sprite_store.EGOM_STORE5_MATCHn = nand3(_AROR_MATCH_ENp, COGY_STORE5_MATCHAp, FYMA_STORE5_MATCHBp);
  /* p29.YBEZ*/ sprite_store.YBEZ_STORE6_MATCHn = nand3(_AROR_MATCH_ENp, YWAP_STORE6_MATCHAp, YDOT_STORE6_MATCHBp);
  /* p29.DYKA*/ sprite_store.DYKA_STORE7_MATCHn = nand3(_AROR_MATCH_ENp, CYCO_STORE7_MATCHAp, DAJE_STORE7_MATCHBp);
  /* p29.EFYL*/ sprite_store.EFYL_STORE8_MATCHn = nand3(_AROR_MATCH_ENp, DAMA_STORE8_MATCHAp, FEHA_STORE8_MATCHBp);
  /* p29.YGEM*/ sprite_store.YGEM_STORE9_MATCHn = nand3(_AROR_MATCH_ENp, YLEV_STORE9_MATCHAp, YTUB_STORE9_MATCHBp);

  /* p29.FEFY*/ wire _FEFY_STORE_MATCHp = nand5(sprite_store.XAGE_STORE4_MATCHn, sprite_store.YLOZ_STORE3_MATCHn, sprite_store.DEGO_STORE2_MATCHn, sprite_store.DYDU_STORE1_MATCHn, sprite_store.YDUG_STORE0_MATCHn);
  /* p29.FOVE*/ wire _FOVE_STORE_MATCHp = nand5(sprite_store.YGEM_STORE9_MATCHn, sprite_store.EFYL_STORE8_MATCHn, sprite_store.DYKA_STORE7_MATCHn, sprite_store.YBEZ_STORE6_MATCHn, sprite_store.EGOM_STORE5_MATCHn);
  /* p29.FEPO*/ sprite_store.FEPO_STORE_MATCHp = or2(_FEFY_STORE_MATCHp, _FOVE_STORE_MATCHp);

  //------------------------------------------------------------------------------

  // 128 + 32 + 4 + 2 + 1 = 167
  /*#p21.XUGU*/ wire _XUGU_X_167n = nand5(pix_pipe.XEHO_X0p.qp17(), pix_pipe.SAVY_X1p.qp17(), pix_pipe.XODU_X2p.qp17(), pix_pipe.TUKY_X5p.qp17(), pix_pipe.SYBE_X7p.qp17());
  /*#p21.XANO*/ wire _XANO_X_167p = not1(_XUGU_X_167n);
  /*#p21.XENA*/ wire _XENA_STORE_MATCHn = not1(sprite_store.FEPO_STORE_MATCHp);
  /*#p21.WODU*/ pix_pipe.WODU_HBLANKp = and2(_XENA_STORE_MATCHn, _XANO_X_167p);

  /*#p21.TAPA*/ wire _TAPA_INT_OAM = and2(lcd_reg.TOLU_VBLANKn, lcd_reg.SELA_LINE_P908p);
  /*#p21.TARU*/ wire _TARU_INT_HBL = and2(pix_pipe.WODU_HBLANKp, lcd_reg.TOLU_VBLANKn);
  /*#p21.SUKO*/ wire _SUKO_INT_STATp = amux4(pix_pipe.RUGU_STAT_LYI_ENn.qn08(), lcd_reg.ROPO_LY_MATCH_SYNCp.qp17(),
                                             pix_pipe.REFE_STAT_OAI_ENn.qn08(), _TAPA_INT_OAM,
                                             pix_pipe.RUFO_STAT_VBI_ENn.qn08(), lcd_reg.PARU_VBLANKp_d4, // polarity?
                                             pix_pipe.ROXE_STAT_HBI_ENn.qn08(), _TARU_INT_HBL);

  /*#p21.TUVA*/ wire _TUVA_INT_STATn = not1(_SUKO_INT_STATp);
  /*#p21.VOTY*/ pix_pipe.VOTY_INT_STATp = not1(_TUVA_INT_STATn);

  /*#p27.NOCU*/ pix_pipe.NOCU_WIN_MODEn = not1(pix_pipe.PYNU_WIN_MODE_A.qp04());
  /* p27.PORE*/ pix_pipe.PORE_WIN_MODEp = not1(pix_pipe.NOCU_WIN_MODEn);
  /*#p27.SYLO*/ pix_pipe.SYLO_WIN_HITn = not1(pix_pipe.RYDY);
  /*#p24.TOMU*/ pix_pipe.TOMU_WIN_HITp = not1(pix_pipe.SYLO_WIN_HITn);
  /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILE_NE = nand2(pix_pipe.SYLO_WIN_HITn, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17());
  /* p27.SUZU*/ pix_pipe.SUZU_WIN_FIRST_TILEne = not1(_TUXY_WIN_FIRST_TILE_NE);

  /* p24.LOBY*/ pix_pipe.LOBY_RENDERINGn = not1(pix_pipe.XYMU_RENDERINGn.qn03());

  //----------

  /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp04());
  /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn = nand4(pix_pipe.XYMU_RENDERINGn.qn03(),
                                                      _ROMO_PRELOAD_DONEn,
                                                      tile_fetcher.NYKA_FETCH_DONE_P11.qp17(),
                                                      tile_fetcher.PORY_FETCH_DONE_P12.qp17());


  /* p27.SEKO*/ pix_pipe.SEKO_FETCH_TRIGp = nor2(pix_pipe.RYFA_FETCHn_A.qn16(), pix_pipe.RENE_FETCHn_B.qp17());
  /* p27.TAVE*/ pix_pipe.TAVE_PRELOAD_DONE_TRIGp = not1(_SUVU_PRELOAD_DONE_TRIGn);
  /* p27.TEVO*/ pix_pipe.TEVO_FETCH_TRIGp = or3(pix_pipe.SEKO_FETCH_TRIGp, pix_pipe.SUZU_WIN_FIRST_TILEne, pix_pipe.TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /*#p27.NUNY*/ pix_pipe.NUNY_WX_MATCH_TRIGp = and2(pix_pipe.PYNU_WIN_MODE_A.qp04(), pix_pipe.NOPA_WIN_MODE_B.qn16());
  /* p27.NYFO*/ wire _NYFO_WIN_FETCH_TRIGn = not1(pix_pipe.NUNY_WX_MATCH_TRIGp);
  /* p27.MOSU*/ pix_pipe.MOSU_WIN_FETCH_TRIGp = not1(_NYFO_WIN_FETCH_TRIGn);

  /* p27.NYXU*/ pix_pipe.NYXU_FETCH_TRIGn = nor3(sprite_scanner.AVAP_RENDER_START_TRIGp, pix_pipe.MOSU_WIN_FETCH_TRIGp, pix_pipe.TEVO_FETCH_TRIGp);

  /* p21.SADU*/ pix_pipe.SADU_STAT_MODE0n = nor2(pix_pipe.XYMU_RENDERINGn.qn03(), lcd_reg.PARU_VBLANKp_d4); // die NOR
  /* p21.XATY*/ pix_pipe.XATY_STAT_MODE1n = nor2(sprite_scanner.ACYL_SCANNINGp, pix_pipe.XYMU_RENDERINGn.qn03()); // die NOR

  //----------------------------------------

  {
    /*p01.AREV*/ wire _AREV_CPU_WRn_ABCDxxxH = nand2(cpu_bus.PIN_CPU_WRp.qp(), clk_reg.AFAS_xxxxEFGx);
    /*p01.APOV*/ APOV_CPU_WRp_xxxxEFGx      = not1(_AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire _UBAL_CPU_WRn_ABCDxxxH = mux2n(UNOR_MODE_DBG2p, ext_bus.PIN_EXT_WRn.qn(), APOV_CPU_WRp_xxxxEFGx);
    /*p07.TAPU*/ TAPU_CPU_WRp_xxxxEFGx      = not1(_UBAL_CPU_WRn_ABCDxxxH);
    /*p07.DYKY*/ wire _DYKY_CPU_WRn_ABCDxxxH = not1(TAPU_CPU_WRp_xxxxEFGx);
    /*p07.CUPA*/ CUPA_CPU_WRp_xxxxEFGx      = not1(_DYKY_CPU_WRn_ABCDxxxH);
  }

  {
    /*p07.TERA*/ wire _TERA_BOOT_BITp  = not1(bootrom.BOOT_BITn.qp17());
    /*p07.TUTU*/ TUTU_ADDR_BOOTp = and2(_TERA_BOOT_BITp, cpu_bus.TULO_ADDR_00XXp());
  }

  //----------------------------------------
  // tim_reg.tick()

  /* p01.UVYN*/ tim_reg.UVYN_DIV_05n = not1(tim_reg.TAMA_DIV_05.qp17());
  /* p01.UMEK*/ tim_reg.UMEK_DIV_06n = not1(tim_reg.UGOT_DIV_06.qp17());
  /* p01.UREK*/ tim_reg.UREK_DIV_07n = not1(tim_reg.TULU_DIV_07.qp17());

  //----------------------------------------

  /*p02.ASOK*/ joypad.ASOK_INT_JOYp = and2(joypad.APUG_JP_GLITCH3.qp17(), joypad.BATU_JP_GLITCH0.qp17());

  //------------------------------------------------------------------------------

  /* p26.AXAD*/ wire _AXAD_WIN_MODEn = not1(pix_pipe.PORE_WIN_MODEp);

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(tile_fetcher.LAXU_BFETCH_S0.qp17());
  /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n = not1(tile_fetcher.MESU_BFETCH_S1.qp17());
  /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n = not1(tile_fetcher.NYVA_BFETCH_S2.qp17());

  /* p27.NOGU*/ wire _NOGU_FETCH_01p = nand2(_NAKO_BFETCH_S1n, _NOFU_BFETCH_S2n);
  /* p27.NENY*/ wire _NENY_FETCH_01n = not1(_NOGU_FETCH_01p);

  /* p27.LUSU*/ wire _LUSU_BGW_VRAM_RDn = not1(tile_fetcher.LONY_BG_FETCH_RUNNINGp.qp03());
  /* p27.LENA*/ tile_fetcher.LENA_BGW_VRM_RDp = not1(_LUSU_BGW_VRAM_RDn);

  /* p27.POTU*/ wire _POTU_BG_MAP_READp  = and2(tile_fetcher.LENA_BGW_VRM_RDp, _NENY_FETCH_01n);
  /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp = and2(_POTU_BG_MAP_READp, pix_pipe.PORE_WIN_MODEp);
  /* p26.ACEN*/ wire _ACEN_BG_MAP_READp  = and2(_POTU_BG_MAP_READp, _AXAD_WIN_MODEn);

  /* p27.NETA*/ tile_fetcher.NETA_TILE_READp = and2(tile_fetcher.LENA_BGW_VRM_RDp, _NOGU_FETCH_01p);
  /* p26.ASUL*/ wire _ASUL_BG_TILE_READp  = and2(tile_fetcher.NETA_TILE_READp, _AXAD_WIN_MODEn);
  /* p25.XUCY*/ tile_fetcher.XUCY_WIN_TILE_READn = nand2(tile_fetcher.NETA_TILE_READp, pix_pipe.PORE_WIN_MODEp);
  /*#p25.WUKO*/ tile_fetcher.WUKO_WIN_MAP_READn  = not1(_XEZE_WIN_MAP_READp);
  /* p26.BAFY*/ tile_fetcher.BAFY_BG_MAP_READn   = not1(_ACEN_BG_MAP_READp);
  /* p26.BEJE*/ tile_fetcher.BEJE_BG_TILE_READn = not1(_ASUL_BG_TILE_READp);

  /* p27.MOCE*/ tile_fetcher.MOCE_BFETCH_DONEn = nand3(tile_fetcher.LAXU_BFETCH_S0.qp17(), tile_fetcher.NYVA_BFETCH_S2.qp17(), pix_pipe.NYXU_FETCH_TRIGn);

  /*#p27.XUHA*/ tile_fetcher.XUHA_FETCH_S2p  = not1(_NOFU_BFETCH_S2n);

  // MYSO fires on fetch phase 2, 6, 10
  /*#p27.MYSO*/ wire _MYSO_BG_TRIGp   = nor3(pix_pipe.LOBY_RENDERINGn, _LAXE_BFETCH_S0n, tile_fetcher.LYZU_BFETCH_S0_D1.qp17());

  // NYDY on fetch phase 6
  /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn = nand3(_MYSO_BG_TRIGp, tile_fetcher.MESU_BFETCH_S1.qp17(), _NOFU_BFETCH_S2n);
  /*#p32.METE*/ tile_fetcher.METE_LATCH_TILE_DAp   = not1(_NYDY_LATCH_TILE_DAn);

  // MOFU fires on fetch phase 2 and 10
  /*p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp = and2(_MYSO_BG_TRIGp, _NAKO_BFETCH_S1n);
  /*p32.LESO*/ tile_fetcher.LESO_LATCH_TILE_DBn   = not1(_MOFU_LATCH_TILE_DBp);

  /*p27.LYRY*/ tile_fetcher.LYRY_BFETCH_DONEp = not1(tile_fetcher.MOCE_BFETCH_DONEn);

  //------------------------------------------------------------------------------
  // FIXME this is kind of a mess

  /* p29.TEPA*/ wire _TEPA_RENDERINGn = not1(pix_pipe.XYMU_RENDERINGn.qn03());
  /* p29.TYNO*/ wire _TYNO = nand3(sprite_fetcher.TOXE_SFETCH_S0.qp17(), sprite_fetcher.SEBA_SFETCH_S1_D5.qp17(), sprite_fetcher.VONU_SFETCH_S1_D4.qp17());
  /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn  = or2(sprite_fetcher.TYFO_SFETCH_S0_D1.qn16(), _TYNO);
  /* p29.WUTY*/ sprite_fetcher.WUTY_SPRITE_DONEp = not1(_VUSA_SPRITE_DONEn);
  /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor3(_TEPA_RENDERINGn, sprite_fetcher.TULY_SFETCH_S1.qp17(), sprite_fetcher.TESE_SFETCH_S2.qp17());
  /* p28.WEFY*/ sprite_fetcher.WEFY_SPR_READp = and2(_TUVO_PPU_OAM_RDp, sprite_fetcher.TYFO_SFETCH_S0_D1.qp17());
  /* p29.TYTU*/ wire _TYTU_SFETCH_S0n = not1(sprite_fetcher.TOXE_SFETCH_S0.qp17());
  /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand2(sprite_fetcher.TYFO_SFETCH_S0_D1.qp17(), _TYTU_SFETCH_S0n);
  /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY = nor2(sprite_fetcher.TULY_SFETCH_S1.qp17(), sprite_fetcher.VONU_SFETCH_S1_D4.qp17());
  /* p29.TYSO*/ wire _TYSO_SPRITE_READn = or2(_SAKY_SFETCH_MYSTERY, _TEPA_RENDERINGn); // def or
  /* p29.TEXY*/ sprite_fetcher.TEXY_SPR_READ_VRAMp = not1(_TYSO_SPRITE_READn);
  /* p29.ABON*/ sprite_fetcher.ABON_SPR_VRM_RDn = not1(sprite_fetcher.TEXY_SPR_READ_VRAMp);
  /* p25.SOHO*/ sprite_fetcher.SOHO_SPR_VRAM_RDp = and2(_TACU_SPR_SEQ_5_TRIG, sprite_fetcher.TEXY_SPR_READ_VRAMp);
  /* p25.VAPE*/ sprite_fetcher.VAPE_FETCH_OAM_CLK = and2(_TUVO_PPU_OAM_RDp, _TACU_SPR_SEQ_5_TRIG);
  /*#p29.XUQU*/ sprite_fetcher.XUQU_SPRITE_AB = not1(sprite_fetcher.VONU_SFETCH_S1_D4.qn16());
  /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe = nor3(_TYTU_SFETCH_S0n, pix_pipe.LOBY_RENDERINGn, sprite_fetcher.TYFO_SFETCH_S0_D1.qp17());
  /*#p29.RACA*/ sprite_fetcher.RACA_LATCH_SPPIXB = and2(sprite_fetcher.VONU_SFETCH_S1_D4.qp17(), _SYCU_SFETCH_S0pe);
  /*#p29.TOPU*/ sprite_fetcher.TOPU_LATCH_SPPIXA = and2(sprite_fetcher.TULY_SFETCH_S1.qp17(), _SYCU_SFETCH_S0pe);

  //----------------------------------------

  RegBase::tick_running = false;





  //==============================================================================
  //==============================================================================
  //==============================================================================
  //==============================================================================





  RegBase::tock_running = true;

  {
    /*#p25.SYCY*/ wire SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p);
    /*#p25.SOTO*/ SOTO_DBG_VRAM.dff17(SYCY_DBG_CLOCKn, clk_reg.CUNU_SYS_RSTn, SOTO_DBG_VRAM.qn16());
  }

  //------------------------------------------------------------------------------

  // the comp clock is unmarked on the die trace but it's directly to the left of ATAL

  /*p01.AFUR*/ clk_reg.AFUR_xxxxEFGH.dff9(!clk_reg.ATAL_xBxDxFxH, UPOJ_MODE_PRODn, clk_reg.ADYK_ABCxxxxH.qp09());
  /*p01.ALEF*/ clk_reg.ALEF_AxxxxFGH.dff9( clk_reg.ATAL_xBxDxFxH, UPOJ_MODE_PRODn, clk_reg.AFUR_xxxxEFGH.qn08());
  /*p01.APUK*/ clk_reg.APUK_ABxxxxGH.dff9(!clk_reg.ATAL_xBxDxFxH, UPOJ_MODE_PRODn, clk_reg.ALEF_AxxxxFGH.qn08());
  /*p01.ADYK*/ clk_reg.ADYK_ABCxxxxH.dff9( clk_reg.ATAL_xBxDxFxH, UPOJ_MODE_PRODn, clk_reg.APUK_ABxxxxGH.qn08());

  cpu_bus.PIN_CPU_EXT_CLKGOOD.set(CLKGOOD);

  cpu_bus.PIN_CPU_BOWA_Axxxxxxx.set(clk_reg.BOWA_xBCDEFGH);
  cpu_bus.PIN_CPU_BEDO_xBCDEFGH.set(clk_reg.BEDO_Axxxxxxx);

  cpu_bus.PIN_CPU_BEKO_ABCDxxxx.set(clk_reg.BEKO_ABCDxxxx);
  cpu_bus.PIN_CPU_BUDE_xxxxEFGH.set(clk_reg.BUDE_xxxxEFGH);

  cpu_bus.PIN_CPU_BOLO_ABCDEFxx.set(clk_reg.BOLO_ABCDEFxx);
  cpu_bus.PIN_CPU_BUKE_AxxxxxGH.set(clk_reg.BUKE_AxxxxxGH);

  cpu_bus.PIN_CPU_BOMA_xBCDEFGH.set(clk_reg.BOMA_xBCDEFGH);
  cpu_bus.PIN_CPU_BOGA_Axxxxxxx.set(clk_reg.BOGA_Axxxxxxx);

  ext_bus.PIN_EXT_CLK.io_pin(clk_reg.BUDE_xxxxEFGH, clk_reg.BUDE_xxxxEFGH);

  /*p01.UPYF*/ wire _UPYF = or2(RST, clk_reg.UCOB_CLKBADp);
  /*p01.TUBO*/ clk_reg.TUBO_WAITINGp.nor_latch(_UPYF, CPUREADY);

#ifdef FAST_BOOT
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(clk_reg.TUBO_WAITINGp.qp(), tim_reg.TERO_DIV_03.qp());
#else
  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(clk_reg.TUBO_WAITINGp.qp04(), tim_reg.UPOF_DIV_15.qp17());
#endif

  /*p01.TABA*/ wire _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p, UMUT_MODE_DBG1p, _UNUT_POR_TRIGn);
  cpu_bus.PIN_CPU_STARTp.set(_TABA_POR_TRIGn);

  /*#p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire _AFAR_RSTp  = nor2(RST, _ALYP_RSTn);
  /* p01.ASOL*/ clk_reg.ASOL_POR_DONEn.nor_latch(RST, _AFAR_RSTp); // Schematic wrong, this is a latch.

  /* p01.AFER*/ clk_reg.AFER_SYS_RSTp.dff13(clk_reg.BOGA_Axxxxxxx, UPOJ_MODE_PRODn, clk_reg.ASOL_POR_DONEn.qp04());

  cpu_bus.PIN_CPU_SYS_RSTp.set(clk_reg.AFER_SYS_RSTp.qp13());
  cpu_bus.PIN_CPU_EXT_RST.set(RST);

  cpu_bus.PIN_CPU_UNOR_DBG.set(UNOR_MODE_DBG2p);
  cpu_bus.PIN_CPU_UMUT_DBG.set(UMUT_MODE_DBG1p);

  /*p29.XYVA*/ wire _XYVA_xBxDxFxH = not1(clk_reg.ZEME_AxCxExGx);
  /*p29.XOTA*/ wire _XOTA_AxCxExGx = not1(_XYVA_xBxDxFxH);
  /*p29.XYFY*/ wire _XYFY_xBxDxFxH = not1(_XOTA_AxCxExGx);

  /*p29.WUVU*/ clk_reg.WUVU_ABxxEFxx.dff17(_XOTA_AxCxExGx,                   clk_reg.XAPO_VID_RSTn, clk_reg.WUVU_ABxxEFxx.qn16());
  /*p21.VENA*/ clk_reg.VENA_xxCDEFxx.dff17(clk_reg.WUVU_ABxxEFxx.qn16(), clk_reg.XAPO_VID_RSTn, clk_reg.VENA_xxCDEFxx.qn16()); // inverting the clock to this reg doesn't seem to break anything, which is really weird
  /*p29.WOSU*/ clk_reg.WOSU_AxxDExxH.dff17(_XYFY_xBxDxFxH,                   clk_reg.XAPO_VID_RSTn, clk_reg.WUVU_ABxxEFxx.qn16());

  //------------------------------------------------------------------------------

  /*p03.RYFO*/ wire _RYFO_FF04_FF07p = and3(cpu_bus.BUS_CPU_A02.qp(), cpu_bus.SARE_XX00_XX07p(), cpu_bus.SYKE_FF00_FFFFp());

  // FF04 DIV
  {
    /*p01.TAGY*/ wire _TAGY_FF04_RDp = and4(TEDO_CPU_RDp, _RYFO_FF04_FF07p, cpu_bus.TOLA_A01n(), cpu_bus.TOVY_A00n());
    /*p01.TAPE*/ wire _TAPE_FF04_WRp = and4(TAPU_CPU_WRp_xxxxEFGx, _RYFO_FF04_FF07p, cpu_bus.TOLA_A01n(), cpu_bus.TOVY_A00n());
    /*p01.UFOL*/ wire _UFOL_DIV_RSTn = nor3(clk_reg.UCOB_CLKBADp, RST, _TAPE_FF04_WRp);

    /* p01.UKUP*/ tim_reg.UKUP_DIV_00.dff17(clk_reg.BOGA_Axxxxxxx,      _UFOL_DIV_RSTn, tim_reg.UKUP_DIV_00.qn16());
    /* p01.UFOR*/ tim_reg.UFOR_DIV_01.dff17(tim_reg.UKUP_DIV_00.qn16(), _UFOL_DIV_RSTn, tim_reg.UFOR_DIV_01.qn16());
    /* p01.UNER*/ tim_reg.UNER_DIV_02.dff17(tim_reg.UFOR_DIV_01.qn16(), _UFOL_DIV_RSTn, tim_reg.UNER_DIV_02.qn16());
    /*#p01.TERO*/ tim_reg.TERO_DIV_03.dff17(tim_reg.UNER_DIV_02.qn16(), _UFOL_DIV_RSTn, tim_reg.TERO_DIV_03.qn16());
    /* p01.UNYK*/ tim_reg.UNYK_DIV_04.dff17(tim_reg.TERO_DIV_03.qn16(), _UFOL_DIV_RSTn, tim_reg.UNYK_DIV_04.qn16());
    /* p01.TAMA*/ tim_reg.TAMA_DIV_05.dff17(tim_reg.UNYK_DIV_04.qn16(), _UFOL_DIV_RSTn, tim_reg.TAMA_DIV_05.qn16());

    // this is hacked up because we're ignoring the debug reg for the moment
    ///*p01.ULUR*/ wire ULUR_DIV_06_CLK = mux2p(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /* p01.ULUR*/ wire _ULUR_DIV_06_CLK = tim_reg.TAMA_DIV_05.qn16();

    /* p01.UGOT*/ tim_reg.UGOT_DIV_06.dff17(_ULUR_DIV_06_CLK,               _UFOL_DIV_RSTn, tim_reg.UGOT_DIV_06.qn16());
    /* p01.TULU*/ tim_reg.TULU_DIV_07.dff17(tim_reg.UGOT_DIV_06.qn16(), _UFOL_DIV_RSTn, tim_reg.TULU_DIV_07.qn16());
    /* p01.TUGO*/ tim_reg.TUGO_DIV_08.dff17(tim_reg.TULU_DIV_07.qn16(), _UFOL_DIV_RSTn, tim_reg.TUGO_DIV_08.qn16());
    /* p01.TOFE*/ tim_reg.TOFE_DIV_09.dff17(tim_reg.TUGO_DIV_08.qn16(), _UFOL_DIV_RSTn, tim_reg.TOFE_DIV_09.qn16());
    /* p01.TERU*/ tim_reg.TERU_DIV_10.dff17(tim_reg.TOFE_DIV_09.qn16(), _UFOL_DIV_RSTn, tim_reg.TERU_DIV_10.qn16());
    /* p01.SOLA*/ tim_reg.SOLA_DIV_11.dff17(tim_reg.TERU_DIV_10.qn16(), _UFOL_DIV_RSTn, tim_reg.SOLA_DIV_11.qn16());
    /* p01.SUBU*/ tim_reg.SUBU_DIV_12.dff17(tim_reg.SOLA_DIV_11.qn16(), _UFOL_DIV_RSTn, tim_reg.SUBU_DIV_12.qn16());
    /* p01.TEKA*/ tim_reg.TEKA_DIV_13.dff17(tim_reg.SUBU_DIV_12.qn16(), _UFOL_DIV_RSTn, tim_reg.TEKA_DIV_13.qn16());
    /* p01.UKET*/ tim_reg.UKET_DIV_14.dff17(tim_reg.TEKA_DIV_13.qn16(), _UFOL_DIV_RSTn, tim_reg.UKET_DIV_14.qn16());
    /* p01.UPOF*/ tim_reg.UPOF_DIV_15.dff17(tim_reg.UKET_DIV_14.qn16(), _UFOL_DIV_RSTn, tim_reg.UPOF_DIV_15.qn16());

    /* p01.UTOK*/ wire _UTOK_DIV_08n = not1(tim_reg.TUGO_DIV_08.qp17());
    /* p01.SAPY*/ wire _SAPY_DIV_09n = not1(tim_reg.TOFE_DIV_09.qp17());
    /* p01.UMER*/ wire _UMER_DIV_10n = not1(tim_reg.TERU_DIV_10.qp17());
    /* p01.RAVE*/ wire _RAVE_DIV_11n = not1(tim_reg.SOLA_DIV_11.qp17());
    /* p01.RYSO*/ wire _RYSO_DIV_12n = not1(tim_reg.SUBU_DIV_12.qp17());
    /* p01.UDOR*/ wire _UDOR_DIV_13n = not1(tim_reg.TEKA_DIV_13.qp17());

    /* p01.TAWU*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_TAGY_FF04_RDp, tim_reg.UMEK_DIV_06n);
    /* p01.TAKU*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_TAGY_FF04_RDp, tim_reg.UREK_DIV_07n);
    /* p01.TEMU*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_TAGY_FF04_RDp, _UTOK_DIV_08n);
    /* p01.TUSE*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_TAGY_FF04_RDp, _SAPY_DIV_09n);
    /* p01.UPUG*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_TAGY_FF04_RDp, _UMER_DIV_10n); // Schematic wrong, UPUG/SEPU driving D5/D4
    /* p01.SEPU*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_TAGY_FF04_RDp, _RAVE_DIV_11n);
    /* p01.SAWA*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_TAGY_FF04_RDp, _RYSO_DIV_12n);
    /* p01.TATU*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_TAGY_FF04_RDp, _UDOR_DIV_13n);
  }

  // FF05 TIMA
  {
    /*#p03.TEDA*/ wire _TEDA_FF05_RDp = and4(_RYFO_FF04_FF07p, TEDO_CPU_RDp, cpu_bus.TOLA_A01n(), cpu_bus.BUS_CPU_A00.qp());
    /*#p03.TOPE*/ wire _TOPE_FF05_WRn = nand4(cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n(), TAPU_CPU_WRp_xxxxEFGx, _RYFO_FF04_FF07p);

    /*#p03.UBOT*/ wire _UBOT_DIV_01n = not1(tim_reg.UFOR_DIV_01.qp17());
    /*#p03.UVYR*/ wire _UVYR_DIV_03n = not1(tim_reg.TERO_DIV_03.qp17());

    /*#p03.UKAP*/ wire _UKAP_CLK_MUXa = mux2n(tim_reg.SOPU_TAC_D0.qp17(), tim_reg.UVYN_DIV_05n, _UVYR_DIV_03n);
    /*#p03.TEKO*/ wire _TEKO_CLK_MUXb = mux2n(tim_reg.SOPU_TAC_D0.qp17(), _UBOT_DIV_01n, tim_reg.UREK_DIV_07n);
    /*#p03.TECY*/ wire _TECY_CLK_MUXc = mux2n(tim_reg.SAMY_TAC_D1.qp17(), _UKAP_CLK_MUXa,  _TEKO_CLK_MUXb);

    /*#p03.ROKE*/ wire _ROKE_TIMA_D0 = mux2n(_TOPE_FF05_WRn, tim_reg.SABU_TMA_D0.qp17(), cpu_bus.BUS_CPU_D0p.qp());
    /*#p03.PETU*/ wire _PETU_TIMA_D1 = mux2n(_TOPE_FF05_WRn, tim_reg.NYKE_TMA_D1.qp17(), cpu_bus.BUS_CPU_D1p.qp());
    /*#p03.NYKU*/ wire _NYKU_TIMA_D2 = mux2n(_TOPE_FF05_WRn, tim_reg.MURU_TMA_D2.qp17(), cpu_bus.BUS_CPU_D2p.qp());
    /*#p03.SOCE*/ wire _SOCE_TIMA_D3 = mux2n(_TOPE_FF05_WRn, tim_reg.TYVA_TMA_D3.qp17(), cpu_bus.BUS_CPU_D3p.qp());
    /*#p03.SALA*/ wire _SALA_TIMA_D4 = mux2n(_TOPE_FF05_WRn, tim_reg.TYRU_TMA_D4.qp17(), cpu_bus.BUS_CPU_D4p.qp());
    /*#p03.SYRU*/ wire _SYRU_TIMA_D5 = mux2n(_TOPE_FF05_WRn, tim_reg.SUFY_TMA_D5.qp17(), cpu_bus.BUS_CPU_D5p.qp());
    /*#p03.REFU*/ wire _REFU_TIMA_D6 = mux2n(_TOPE_FF05_WRn, tim_reg.PETO_TMA_D6.qp17(), cpu_bus.BUS_CPU_D6p.qp());
    /*#p03.RATO*/ wire _RATO_TIMA_D7 = mux2n(_TOPE_FF05_WRn, tim_reg.SETA_TMA_D7.qp17(), cpu_bus.BUS_CPU_D7p.qp());

    /* p03.MULO*/ wire _MULO_SYS_RSTn = not1(clk_reg.ALUR_SYS_RSTn);
    /*#p03.PUXY*/ wire _PUXY_TIMA_D0 = nor2(_MULO_SYS_RSTn, _ROKE_TIMA_D0);
    /*#p03.NERO*/ wire _NERO_TIMA_D1 = nor2(_MULO_SYS_RSTn, _PETU_TIMA_D1);
    /*#p03.NADA*/ wire _NADA_TIMA_D2 = nor2(_MULO_SYS_RSTn, _NYKU_TIMA_D2);
    /*#p03.REPA*/ wire _REPA_TIMA_D3 = nor2(_MULO_SYS_RSTn, _SOCE_TIMA_D3);
    /*#p03.ROLU*/ wire _ROLU_TIMA_D4 = nor2(_MULO_SYS_RSTn, _SALA_TIMA_D4);
    /*#p03.RUGY*/ wire _RUGY_TIMA_D5 = nor2(_MULO_SYS_RSTn, _SYRU_TIMA_D5);
    /*#p03.PYMA*/ wire _PYMA_TIMA_D6 = nor2(_MULO_SYS_RSTn, _REFU_TIMA_D6);
    /*#p03.PAGU*/ wire _PAGU_TIMA_D7 = nor2(_MULO_SYS_RSTn, _RATO_TIMA_D7);

    /*#p03.MEKE*/ wire _MEKE_TIMER_OVERFLOWn = not1(tim_reg.MOBA_TIMER_OVERFLOWp.qp17());
    /*#p03.MUZU*/ wire _MUZU_CPU_LOAD_TIMAn  = or2(cpu_bus.PIN_CPU_LATCH_EXT.qp(), _TOPE_FF05_WRn);
    /*#p03.MEXU*/ wire _MEXU_TIMA_LOADp      = nand3(_MUZU_CPU_LOAD_TIMAn, clk_reg.ALUR_SYS_RSTn, _MEKE_TIMER_OVERFLOWn);

    /*#p03.SOGU*/ wire _SOGU_TIMA_CLK = nor2(_TECY_CLK_MUXc, tim_reg.SABO_TAC_D2.qn16());
    /*#p03.REGA*/ tim_reg.REGA_TIMA_D0.dff20(_SOGU_TIMA_CLK,      _MEXU_TIMA_LOADp, _PUXY_TIMA_D0);
    /*#p03.POVY*/ tim_reg.POVY_TIMA_D1.dff20(tim_reg.REGA_TIMA_D0.qp01(), _MEXU_TIMA_LOADp, _NERO_TIMA_D1);
    /*#p03.PERU*/ tim_reg.PERU_TIMA_D2.dff20(tim_reg.POVY_TIMA_D1.qp01(), _MEXU_TIMA_LOADp, _NADA_TIMA_D2);
    /*#p03.RATE*/ tim_reg.RATE_TIMA_D3.dff20(tim_reg.PERU_TIMA_D2.qp01(), _MEXU_TIMA_LOADp, _REPA_TIMA_D3);
    /*#p03.RUBY*/ tim_reg.RUBY_TIMA_D4.dff20(tim_reg.RATE_TIMA_D3.qp01(), _MEXU_TIMA_LOADp, _ROLU_TIMA_D4);
    /*#p03.RAGE*/ tim_reg.RAGE_TIMA_D5.dff20(tim_reg.RUBY_TIMA_D4.qp01(), _MEXU_TIMA_LOADp, _RUGY_TIMA_D5);
    /*#p03.PEDA*/ tim_reg.PEDA_TIMA_D6.dff20(tim_reg.RAGE_TIMA_D5.qp01(), _MEXU_TIMA_LOADp, _PYMA_TIMA_D6);
    /*#p03.NUGA*/ tim_reg.NUGA_TIMA_D7.dff20(tim_reg.PEDA_TIMA_D6.qp01(), _MEXU_TIMA_LOADp, _PAGU_TIMA_D7);

    /*#p03.SOKU*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_TEDA_FF05_RDp, tim_reg.REGA_TIMA_D0.qn17());
    /*#p03.RACY*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_TEDA_FF05_RDp, tim_reg.POVY_TIMA_D1.qn17());
    /*#p03.RAVY*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_TEDA_FF05_RDp, tim_reg.PERU_TIMA_D2.qn17());
    /*#p03.SOSY*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_TEDA_FF05_RDp, tim_reg.RATE_TIMA_D3.qn17());
    /*#p03.SOMU*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_TEDA_FF05_RDp, tim_reg.RUBY_TIMA_D4.qn17());
    /*#p03.SURO*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_TEDA_FF05_RDp, tim_reg.RAGE_TIMA_D5.qn17());
    /*#p03.ROWU*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_TEDA_FF05_RDp, tim_reg.PEDA_TIMA_D6.qn17());
    /*#p03.PUSO*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_TEDA_FF05_RDp, tim_reg.NUGA_TIMA_D7.qn17());

    /*#p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn = not1(_MEXU_TIMA_LOADp);
    /*#p03.NYDU*/ tim_reg.NYDU_TIMA_D7_DELAY.dff17(clk_reg.BOGA_Axxxxxxx, _MUGY_TIMA_MAX_RSTn, tim_reg.NUGA_TIMA_D7.qp01());

    /*#p03.MERY*/ wire _MERY_TIMER_OVERFLOWp = nor2(tim_reg.NUGA_TIMA_D7.qp01(), tim_reg.NYDU_TIMA_D7_DELAY.qn16());
    /*#p03.MOBA*/ tim_reg.MOBA_TIMER_OVERFLOWp.dff17(clk_reg.BOGA_Axxxxxxx, clk_reg.ALUR_SYS_RSTn, _MERY_TIMER_OVERFLOWp);
  }

  // FF06 TMA
  {
    /*p03.TUBY*/ wire _TUBY_FF06_RDp = and4(TEDO_CPU_RDp, _RYFO_FF04_FF07p, cpu_bus.BUS_CPU_A01.qp(), cpu_bus.TOVY_A00n());
    /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand4(TAPU_CPU_WRp_xxxxEFGx, _RYFO_FF04_FF07p, cpu_bus.BUS_CPU_A01.qp(), cpu_bus.TOVY_A00n());

    /*p03.SABU*/ tim_reg.SABU_TMA_D0.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /*p03.NYKE*/ tim_reg.NYKE_TMA_D1.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /*p03.MURU*/ tim_reg.MURU_TMA_D2.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /*p03.TYVA*/ tim_reg.TYVA_TMA_D3.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /*p03.TYRU*/ tim_reg.TYRU_TMA_D4.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /*p03.SUFY*/ tim_reg.SUFY_TMA_D5.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /*p03.PETO*/ tim_reg.PETO_TMA_D6.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /*p03.SETA*/ tim_reg.SETA_TMA_D7.dff17(_TYJU_FF06_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /*#p03.SETE*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_TUBY_FF06_RDp, tim_reg.SABU_TMA_D0.qn16());
    /*#p03.PYRE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_TUBY_FF06_RDp, tim_reg.NYKE_TMA_D1.qn16());
    /*#p03.NOLA*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_TUBY_FF06_RDp, tim_reg.MURU_TMA_D2.qn16());
    /*#p03.SALU*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_TUBY_FF06_RDp, tim_reg.TYVA_TMA_D3.qn16());
    /*#p03.SUPO*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_TUBY_FF06_RDp, tim_reg.TYRU_TMA_D4.qn16());
    /*#p03.SOTU*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_TUBY_FF06_RDp, tim_reg.SUFY_TMA_D5.qn16());
    /*#p03.REVA*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_TUBY_FF06_RDp, tim_reg.PETO_TMA_D6.qn16());
    /*#p03.SAPU*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_TUBY_FF06_RDp, tim_reg.SETA_TMA_D7.qn16());
  }

  // FF07 TAC
  {
    /*p03.SORA*/ wire _SORA_FF07_RDp = and4(TEDO_CPU_RDp, _RYFO_FF04_FF07p, cpu_bus.BUS_CPU_A01.qp(), cpu_bus.BUS_CPU_A00.qp());
    /*p03.SARA*/ wire _SARA_FF07_WRn = nand4(TAPU_CPU_WRp_xxxxEFGx, _RYFO_FF04_FF07p, cpu_bus.BUS_CPU_A01.qp(), cpu_bus.BUS_CPU_A00.qp());

    /*p03.SOPU*/ tim_reg.SOPU_TAC_D0.dff17(_SARA_FF07_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /*p03.SAMY*/ tim_reg.SAMY_TAC_D1.dff17(_SARA_FF07_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /*p03.SABO*/ tim_reg.SABO_TAC_D2.dff17(_SARA_FF07_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());

    /*#p03.RYLA*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_SORA_FF07_RDp, tim_reg.SOPU_TAC_D0.qn16());
    /*#p03.ROTE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_SORA_FF07_RDp, tim_reg.SAMY_TAC_D1.qn16());
    /*#p03.SUPE*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_SORA_FF07_RDp, tim_reg.SABO_TAC_D2.qn16());
  }

  //------------------------------------------------------------------------------

  // FF50 - disable bootrom bit
  {
    /*p07.TYRO*/ wire _TYFO_ADDR_0x0x0000p = nor6(cpu_bus.BUS_CPU_A07.qp(), cpu_bus.BUS_CPU_A05.qp(), cpu_bus.BUS_CPU_A03.qp(),
                                                  cpu_bus.BUS_CPU_A02.qp(), cpu_bus.BUS_CPU_A01.qp(), cpu_bus.BUS_CPU_A00.qp());
    /*p07.TUFA*/ wire _TUFA_ADDR_x1x1xxxxp = and2(cpu_bus.BUS_CPU_A04.qp(), cpu_bus.BUS_CPU_A06.qp());

    /*p07.TEXE*/ wire _TEXE_FF50_RDp = and4(TEDO_CPU_RDp, cpu_bus.SYKE_FF00_FFFFp(), _TYFO_ADDR_0x0x0000p, _TUFA_ADDR_x1x1xxxxp);
    /*p07.SYPU*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_TEXE_FF50_RDp, bootrom.BOOT_BITn.qp17());

    /*p07.TUGE*/ wire _TUGE_FF50_WRn = nand4(TAPU_CPU_WRp_xxxxEFGx, cpu_bus.SYKE_FF00_FFFFp(), _TYFO_ADDR_0x0x0000p, _TUFA_ADDR_x1x1xxxxp);
    /*p07.SATO*/ wire _SATO_BOOT_BIT_IN = or2(cpu_bus.BUS_CPU_D0p.qp(), bootrom.BOOT_BITn.qp17());

    /*p07.TEPU*/ bootrom.BOOT_BITn.dff17(_TUGE_FF50_WRn, clk_reg.ALUR_SYS_RSTn, _SATO_BOOT_BIT_IN);
  }

  {
    // Bootrom -> CPU

    /*p07.YAZA*/ wire _YAZA_MODE_DBG1n = not1(UMUT_MODE_DBG1p);
    /*p07.YULA*/ wire _YULA_BOOT_RDp   = and3(TEDO_CPU_RDp, _YAZA_MODE_DBG1n, TUTU_ADDR_BOOTp); // def AND
    /*p07.ZADO*/ wire _ZADO_BOOT_CSn   = nand2(_YULA_BOOT_RDp, cpu_bus.ZUFA_ADDR_00XX());
    /*p07.ZERY*/ wire _ZERY_BOOT_CSp   = not1(_ZADO_BOOT_CSn);

#if 0
    /*p07.ZYBA*/ wire ZYBA_ADDR_00n = not1(cpu_bus.BUS_CPU_A00);
    /*p07.ZUVY*/ wire ZUVY_ADDR_01n = not1(cpu_bus.BUS_CPU_A01);
    /*p07.ZUFY*/ wire ZUFY_ADDR_04n = not1(cpu_bus.BUS_CPU_A04);
    /*p07.ZERA*/ wire ZERA_ADDR_05n = not1(cpu_bus.BUS_CPU_A05);
    /*p07.ZOLE*/ wire ZOLE_ADDR_00  = and2(ZUVY_ADDR_01n, ZYBA_ADDR_00n);
    /*p07.ZAJE*/ wire ZAJE_ADDR_01  = and2(ZUVY_ADDR_01n, cpu_bus.BUS_CPU_A00);
    /*p07.ZUBU*/ wire ZUBU_ADDR_10  = and2(cpu_bus.BUS_CPU_A01, ZYBA_ADDR_00n);
    /*p07.ZAPY*/ wire ZAPY_ADDR_11  = and2(cpu_bus.BUS_CPU_A01, cpu_bus.BUS_CPU_A00);

    /*p07.ZETE*/ wire BOOTROM_A1nA0n = not1(ZOLE_ADDR_00);
    /*p07.ZEFU*/ wire BOOTROM_A1nA0p  = not1(ZAJE_ADDR_01);
    /*p07.ZYRO*/ wire BOOTROM_A1pA0n  = not1(ZUBU_ADDR_10);
    /*p07.ZAPA*/ wire BOOTROM_A1pA0p   = not1(ZAPY_ADDR_11);
    /*p07.ZYGA*/ wire BOOTROM_A2n    = not1(cpu_bus.BUS_CPU_A02);
    /*p07.ZYKY*/ wire BOOTROM_A3n    = not1(cpu_bus.BUS_CPU_A03);
    /*p07.ZYKY*/ wire BOOTROM_A5nA4n = and2(ZERA_ADDR_05n, ZUFY_ADDR_04n);
    /*p07.ZYGA*/ wire BOOTROM_A5nA4p  = and2(ZERA_ADDR_05n, cpu_bus.BUS_CPU_A04);
    /*p07.ZOVY*/ wire BOOTROM_A5pA4n  = and2(cpu_bus.BUS_CPU_A05, ZUFY_ADDR_04n);
    /*p07.ZUKO*/ wire BOOTROM_A5pA4p   = and2(cpu_bus.BUS_CPU_A05, cpu_bus.BUS_CPU_A04);
    /*p07.ZAGE*/ wire BOOTROM_A6n    = not1(cpu_bus.BUS_CPU_A06);
    /*p07.ZYRA*/ wire BOOTROM_A7n    = not1(cpu_bus.BUS_CPU_A07);
#endif

#if 1
    // this is kind of a hack
    uint16_t addr = (uint16_t)cpu_bus.get_bus_addr();
    uint8_t data = DMG_ROM_bin[addr & 0xFF];

    cpu_bus.BUS_CPU_D0p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x01));
    cpu_bus.BUS_CPU_D1p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x02));
    cpu_bus.BUS_CPU_D2p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x04));
    cpu_bus.BUS_CPU_D3p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x08));
    cpu_bus.BUS_CPU_D4p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x10));
    cpu_bus.BUS_CPU_D5p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x20));
    cpu_bus.BUS_CPU_D6p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x40));
    cpu_bus.BUS_CPU_D7p.tri_6pn(_ZERY_BOOT_CSp, !bool(data & 0x80));
#endif
  }

  //------------------------------------------------------------------------------
  // dma_reg.tock(top, cpu_bus);

  /*#p22.WATE*/ wire _WATE_FF46n    = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.WESA_A01p(), cpu_bus.WALO_A02p(), cpu_bus.XERA_A03n());
  /*#p22.XEDA*/ wire _XEDA_FF46p    = not1(_WATE_FF46n);
  /*#p04.LAVY*/ wire _LAVY_FF46_WRp = and2(CUPA_CPU_WRp_xxxxEFGx, _XEDA_FF46p);

  /*#p04.LOKO*/ wire _LOKO_DMA_RSTp = nand2(dma_reg.LENE_DMA_TRIG_d4.qn16(), clk_reg.CUNU_SYS_RSTn);
  /*#p04.LAPA*/ wire _LAPA_DMA_RSTn = not1(_LOKO_DMA_RSTp);

  {
    /*#p04.LYXE*/ dma_reg.LYXE_DMA_LATCHp.nor_latch(_LAVY_FF46_WRp, _LOKO_DMA_RSTp);

    /*#p04.LUPA*/ wire _LUPA_DMA_TRIG = nor2(_LAVY_FF46_WRp, dma_reg.LYXE_DMA_LATCHp.qn03());
    /*#p04.LUVY*/ dma_reg.LUVY_DMA_TRIG_d0  .dff17(clk_reg.UVYT_ABCDxxxx, clk_reg.CUNU_SYS_RSTn, _LUPA_DMA_TRIG);
    /*#p04.LENE*/ dma_reg.LENE_DMA_TRIG_d4  .dff17(clk_reg.MOPA_xxxxEFGH, clk_reg.CUNU_SYS_RSTn, dma_reg.LUVY_DMA_TRIG_d0.qp17());

    /*#p04.LOKY*/ dma_reg.LOKY_DMA_LATCHp.nand_latch(dma_reg.LENE_DMA_TRIG_d4.qn16(), and2(dma_reg.MYTE_DMA_DONE.qn16(), clk_reg.CUNU_SYS_RSTn));
    /*#p04.MATU*/ dma_reg.MATU_DMA_RUNNINGp .dff17(clk_reg.UVYT_ABCDxxxx, clk_reg.CUNU_SYS_RSTn, dma_reg.LOKY_DMA_LATCHp.qp03());
  }

  {
    // 128+16+8+4+2+1 = 159, this must be "dma done"

    /*#p04.NAVO*/ wire _NAVO_DMA_DONEn = nand6(dma_reg.NAKY_DMA_A00p.qp17(), dma_reg.PYRO_DMA_A01p.qp17(),
                                               dma_reg.NEFY_DMA_A02p.qp17(), dma_reg.MUTY_DMA_A03p.qp17(),
                                               dma_reg.NYKO_DMA_A04p.qp17(), dma_reg.MUGU_DMA_A07p.qp17());
    /*#p04.NOLO*/ wire _NOLO_DMA_DONEp = not1(_NAVO_DMA_DONEn);
    /*#p04.MYTE*/ dma_reg.MYTE_DMA_DONE.dff17(clk_reg.MOPA_xxxxEFGH, _LAPA_DMA_RSTn, _NOLO_DMA_DONEp);
  }

  {
    /*#p04.META*/ wire _META_DMA_CLKp = and2(clk_reg.UVYT_ABCDxxxx, dma_reg.LOKY_DMA_LATCHp.qp03());

    /*#p04.NAKY*/ dma_reg.NAKY_DMA_A00p.dff17(_META_DMA_CLKp,                   _LAPA_DMA_RSTn, dma_reg.NAKY_DMA_A00p.qn16());
    /*#p04.PYRO*/ dma_reg.PYRO_DMA_A01p.dff17(dma_reg.NAKY_DMA_A00p.qn16(), _LAPA_DMA_RSTn, dma_reg.PYRO_DMA_A01p.qn16());
    /* p04.NEFY*/ dma_reg.NEFY_DMA_A02p.dff17(dma_reg.PYRO_DMA_A01p.qn16(), _LAPA_DMA_RSTn, dma_reg.NEFY_DMA_A02p.qn16());
    /* p04.MUTY*/ dma_reg.MUTY_DMA_A03p.dff17(dma_reg.NEFY_DMA_A02p.qn16(), _LAPA_DMA_RSTn, dma_reg.MUTY_DMA_A03p.qn16());
    /* p04.NYKO*/ dma_reg.NYKO_DMA_A04p.dff17(dma_reg.MUTY_DMA_A03p.qn16(), _LAPA_DMA_RSTn, dma_reg.NYKO_DMA_A04p.qn16());
    /* p04.PYLO*/ dma_reg.PYLO_DMA_A05p.dff17(dma_reg.NYKO_DMA_A04p.qn16(), _LAPA_DMA_RSTn, dma_reg.PYLO_DMA_A05p.qn16());
    /* p04.NUTO*/ dma_reg.NUTO_DMA_A06p.dff17(dma_reg.PYLO_DMA_A05p.qn16(), _LAPA_DMA_RSTn, dma_reg.NUTO_DMA_A06p.qn16());
    /* p04.MUGU*/ dma_reg.MUGU_DMA_A07p.dff17(dma_reg.NUTO_DMA_A06p.qn16(), _LAPA_DMA_RSTn, dma_reg.MUGU_DMA_A07p.qn16());
  }

  // FF46 DMA
  {
    /*#p04.LORU*/ wire _LORU_FF46_WRn = not1(_LAVY_FF46_WRp);
    ///*#p??.PYSU*/ wire _PYSU_FF46_WRp = not1(LORU_FF46_WRn);

    /*#p04.NAFA*/ dma_reg.NAFA_DMA_A08n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D0p.qp()); // BOTH OUTPUTS USED
    /* p04.PYNE*/ dma_reg.PYNE_DMA_A09n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D1p.qp());
    /* p04.PARA*/ dma_reg.PARA_DMA_A10n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D2p.qp());
    /* p04.NYDO*/ dma_reg.NYDO_DMA_A11n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D3p.qp());
    /* p04.NYGY*/ dma_reg.NYGY_DMA_A12n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D4p.qp());
    /* p04.PULA*/ dma_reg.PULA_DMA_A13n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D5p.qp());
    /* p04.POKU*/ dma_reg.POKU_DMA_A14n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D6p.qp());
    /* p04.MARU*/ dma_reg.MARU_DMA_A15n.dff8p(_LORU_FF46_WRn, cpu_bus.BUS_CPU_D7p.qp());

    /*#p04.MOLU*/ wire _MOLU_FF46_RDp = and2(_XEDA_FF46p, ASOT_CPU_RDp);
    /*#p04.NYGO*/ wire _NYGO_FF46_RDn = not1(_MOLU_FF46_RDp);
    /*#p04.PUSY*/ wire _PUSY_FF46_RDp = not1(_NYGO_FF46_RDn);

    /*#p04.POLY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_PUSY_FF46_RDp, dma_reg.NAFA_DMA_A08n.qp08());
    /* p04.ROFO*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_PUSY_FF46_RDp, dma_reg.PYNE_DMA_A09n.qp08());
    /* p04.REMA*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_PUSY_FF46_RDp, dma_reg.PARA_DMA_A10n.qp08());
    /* p04.PANE*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_PUSY_FF46_RDp, dma_reg.NYDO_DMA_A11n.qp08());
    /* p04.PARE*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_PUSY_FF46_RDp, dma_reg.NYGY_DMA_A12n.qp08());
    /* p04.RALY*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_PUSY_FF46_RDp, dma_reg.PULA_DMA_A13n.qp08());
    /* p04.RESU*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_PUSY_FF46_RDp, dma_reg.POKU_DMA_A14n.qp08());
    /* p04.NUVY*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_PUSY_FF46_RDp, dma_reg.MARU_DMA_A15n.qp08());
  }

  //------------------------------------------------------------------------------
  // ser_reg.tock(top, cpu_bus);

  /*p06.SANO*/ wire _SANO_ADDR_FF00_FF03 = and3(cpu_bus.SARE_XX00_XX07p(), cpu_bus.SEFY_A02n(), cpu_bus.SYKE_FF00_FFFFp());
  /*p06.URYS*/ wire _URYS_FF01_WRn = nand4(TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());
  /*p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /*p06.UFEG*/ wire _UFEG_FF01_RDp = and4(TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());

  /*p06.COBA*/ wire _COBA_SER_CNT3n = not1(ser_reg.CALY_INT_SERp.qp17());
  /*p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, clk_reg.ALUR_SYS_RSTn);

  /*p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ETAF*/ ser_reg.ETAF_XFER_START.dff17(_UWAM_FF02_WRn, _CABY_XFER_RESET, cpu_bus.BUS_CPU_D7p.qp());
  /*p06.CULY*/ ser_reg.CULY_XFER_DIR  .dff17(_UWAM_FF02_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());

  /*p06.UCOM*/ wire _UCOM_FF02_RD = and4(TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ELUV*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UCOM_FF02_RD, ser_reg.ETAF_XFER_START.qn16());
  /*p06.CORE*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UCOM_FF02_RD, ser_reg.CULY_XFER_DIR.qn16());

  /*p06.COTY*/ ser_reg.COTY_SER_CLK.dff17(tim_reg.UVYN_DIV_05n, _UWAM_FF02_WRn, ser_reg.COTY_SER_CLK.qn16());

  /* p06.CAVE*/ wire _CAVE_SER_CLK_MUXn = mux2n(ser_reg.CULY_XFER_DIR.qp17(), ser_reg.COTY_SER_CLK.qp17(), ser_reg.PIN_SCK.qn());

  /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK_MUXn, ser_reg.ETAF_XFER_START.qn16()); // this must stop the clock or something when the transmit's done
  /* p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  /* p06.CARO*/ wire _CARO_SER_RST = and2(_UWAM_FF02_WRn, clk_reg.ALUR_SYS_RSTn);
  /* p06.CAFA*/ ser_reg.CAFA_SER_CNT0.dff17(_DAWA_SER_CLK,                    _CARO_SER_RST, ser_reg.CAFA_SER_CNT0.qn16());
  /* p06.CYLO*/ ser_reg.CYLO_SER_CNT1.dff17(ser_reg.CAFA_SER_CNT0.qn16(), _CARO_SER_RST, ser_reg.CYLO_SER_CNT1.qn16());
  /* p06.CYDE*/ ser_reg.CYDE_SER_CNT2.dff17(ser_reg.CYLO_SER_CNT1.qn16(), _CARO_SER_RST, ser_reg.CYDE_SER_CNT2.qn16());
  /* p06.CALY*/ ser_reg.CALY_INT_SERp.dff17(ser_reg.CYDE_SER_CNT2.qn16(), _CARO_SER_RST, ser_reg.CALY_INT_SERp.qn16());

  /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(cpu_bus.BUS_CPU_D0p.qp(), _DAKU_FF01_WRp);
  /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(cpu_bus.BUS_CPU_D1p.qp(), _DAKU_FF01_WRp);
  /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(cpu_bus.BUS_CPU_D2p.qp(), _DAKU_FF01_WRp);
  /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(cpu_bus.BUS_CPU_D3p.qp(), _DAKU_FF01_WRp);
  /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(cpu_bus.BUS_CPU_D4p.qp(), _DAKU_FF01_WRp);
  /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(cpu_bus.BUS_CPU_D5p.qp(), _DAKU_FF01_WRp);
  /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(cpu_bus.BUS_CPU_D6p.qp(), _DAKU_FF01_WRp);
  /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn = nand2(cpu_bus.BUS_CPU_D7p.qp(), _DAKU_FF01_WRp);

  // COHY matches BYHA, and BYHA's C input _must_ be and, so this is (A | B) & C
  // and dff22's SET and RST _must_ be SETn/RSTn

  /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D0p.qp(), clk_reg.ALUR_SYS_RSTn);
  /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D1p.qp(), clk_reg.ALUR_SYS_RSTn);
  /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D2p.qp(), clk_reg.ALUR_SYS_RSTn);
  /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D3p.qp(), clk_reg.ALUR_SYS_RSTn);
  /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D4p.qp(), clk_reg.ALUR_SYS_RSTn);
  /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D5p.qp(), clk_reg.ALUR_SYS_RSTn);
  /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D6p.qp(), clk_reg.ALUR_SYS_RSTn);
  /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D7p.qp(), clk_reg.ALUR_SYS_RSTn);

  /* p06.CAGE*/ wire _CAGE_SIN_Cp = not1(ser_reg.PIN_SIN.qn());
  /* p06.CUBA*/ ser_reg.CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SIN_Cp);
  /* p06.DEGU*/ ser_reg.DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, ser_reg.CUBA_SER_DATA0.qp16());
  /* p06.DYRA*/ ser_reg.DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, ser_reg.DEGU_SER_DATA1.qp16());
  /* p06.DOJO*/ ser_reg.DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, ser_reg.DYRA_SER_DATA2.qp16());
  /* p06.DOVU*/ ser_reg.DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, ser_reg.DOJO_SER_DATA3.qp16());
  /* p06.EJAB*/ ser_reg.EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, ser_reg.DOVU_SER_DATA4.qp16());
  /* p06.EROD*/ ser_reg.EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, ser_reg.EJAB_SER_DATA5.qp16());
  /* p06.EDER*/ ser_reg.EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEL_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, ser_reg.EROD_SER_DATA6.qp16());

  /* p06.ELYS*/ ser_reg.ELYS_SER_OUT  .dff17(_EDYL_SER_CLK, clk_reg.ALUR_SYS_RSTn, ser_reg.EDER_SER_DATA7.qp16());

  // FIXME hacking out debug stuff
  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);

  /* p06.KEXU*/ wire _KEXU = nand2(ser_reg.COTY_SER_CLK.qp17(), ser_reg.CULY_XFER_DIR.qp17());
  /* p06.KUJO*/ wire _KUJO = nor2 (ser_reg.COTY_SER_CLK.qp17(), /*p06.JAGO*/ not1(ser_reg.CULY_XFER_DIR.qp17()));

  /* p06.KEXU*/ ser_reg.PIN_SCK.io_pin(_KEXU, _KUJO, ser_reg.CULY_XFER_DIR.qp17());
  /* p05.KENA*/ ser_reg.PIN_SOUT.io_pin(ser_reg.ELYS_SER_OUT.qp17(), ser_reg.ELYS_SER_OUT.qp17());

  /*#p06.CUGY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UFEG_FF01_RDp, ser_reg.CUBA_SER_DATA0.qn15());
  /* p06.DUDE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_UFEG_FF01_RDp, ser_reg.DEGU_SER_DATA1.qn15());
  /* p06.DETU*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_UFEG_FF01_RDp, ser_reg.DYRA_SER_DATA2.qn15());
  /* p06.DASO*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_UFEG_FF01_RDp, ser_reg.DOJO_SER_DATA3.qn15());
  /* p06.DAME*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_UFEG_FF01_RDp, ser_reg.DOVU_SER_DATA4.qn15());
  /* p06.EVOK*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_UFEG_FF01_RDp, ser_reg.EJAB_SER_DATA5.qn15());
  /* p06.EFAB*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_UFEG_FF01_RDp, ser_reg.EROD_SER_DATA6.qn15());
  /*#p06.ETAK*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UFEG_FF01_RDp, ser_reg.EDER_SER_DATA7.qn15());


  //------------------------------------------------------------------------------

  /*p10.AMUS*/ wire _AMUS_XX_0xx00000 = nor6(cpu_bus.BUS_CPU_A00.qp(), cpu_bus.BUS_CPU_A01.qp(), cpu_bus.BUS_CPU_A02.qp(), cpu_bus.BUS_CPU_A03.qp(), cpu_bus.BUS_CPU_A04.qp(), cpu_bus.BUS_CPU_A07.qp());
  /*p10.ANAP*/ wire _ANAP_FF_0xx00000 = and2(_AMUS_XX_0xx00000, cpu_bus.SYKE_FF00_FFFFp());
  /*p10.AKUG*/ wire _AKUG_A06n = not1(cpu_bus.BUS_CPU_A06.qp());
  /*p10.BYKO*/ wire _BYKO_A05n = not1(cpu_bus.BUS_CPU_A05.qp());

  /*p02.KERY*/ wire _KERY_ANY_BUTTONp = or4(joypad.PIN_JOY_P13.qn(), joypad.PIN_JOY_P12.qn(), joypad.PIN_JOY_P11.qn(), joypad.PIN_JOY_P10.qn());

  {
    /*p02.AWOB*/ joypad.AWOB_WAKE_CPU.tp_latch(clk_reg.BOGA_Axxxxxxx, _KERY_ANY_BUTTONp);
    joypad.PIN_CPU_WAKE.set(joypad.AWOB_WAKE_CPU.qp08());
  }

  {
    /*p02.BATU*/ joypad.BATU_JP_GLITCH0.dff17(clk_reg.BOGA_Axxxxxxx, clk_reg.ALUR_SYS_RSTn, _KERY_ANY_BUTTONp);
    /*p02.ACEF*/ joypad.ACEF_JP_GLITCH1.dff17(clk_reg.BOGA_Axxxxxxx, clk_reg.ALUR_SYS_RSTn, joypad.BATU_JP_GLITCH0.qp17());
    /*p02.AGEM*/ joypad.AGEM_JP_GLITCH2.dff17(clk_reg.BOGA_Axxxxxxx, clk_reg.ALUR_SYS_RSTn, joypad.ACEF_JP_GLITCH1.qp17());
    /*p02.APUG*/ joypad.APUG_JP_GLITCH3.dff17(clk_reg.BOGA_Axxxxxxx, clk_reg.ALUR_SYS_RSTn, joypad.AGEM_JP_GLITCH2.qp17());
  }

  {
    /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand4(TAPU_CPU_WRp_xxxxEFGx, _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /*p05.JUTE*/ joypad.JUTE_JOYP_RA    .dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /*p05.KECY*/ joypad.KECY_JOYP_LB    .dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /*p05.JALE*/ joypad.JALE_JOYP_UC    .dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /*p05.KYME*/ joypad.KYME_JOYP_DS    .dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /*p05.KELY*/ joypad.KELY_JOYP_UDLR  .dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /*p05.COFY*/ joypad.COFY_JOYP_ABCS  .dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /*p05.KUKO*/ joypad.KUKO_DBG_FF00_D6.dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /*p05.KERU*/ joypad.KERU_DBG_FF00_D7.dff17(_ATOZ_FF00_WRn, clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /* p10.ACAT*/ wire _ACAT_FF00_RDp = and4(TEDO_CPU_RDp, _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /* p05.BYZO*/ wire _BYZO_FF00_RDn = not1(_ACAT_FF00_RDp);
    /* p05.KEVU*/ joypad.KEVU_JOYP_L0.tp_latch(_BYZO_FF00_RDn, joypad.PIN_JOY_P10.qn());
    /* p05.KAPA*/ joypad.KAPA_JOYP_L1.tp_latch(_BYZO_FF00_RDn, joypad.PIN_JOY_P11.qn());
    /* p05.KEJA*/ joypad.KEJA_JOYP_L2.tp_latch(_BYZO_FF00_RDn, joypad.PIN_JOY_P12.qn());
    /* p05.KOLO*/ joypad.KOLO_JOYP_L3.tp_latch(_BYZO_FF00_RDn, joypad.PIN_JOY_P13.qn());

    // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
    // That also means that _both_ P14 and P15 are selected at reset :/

    /* p05.KEMA*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_BYZO_FF00_RDn, joypad.KEVU_JOYP_L0.qp08());
    /* p05.KURO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_BYZO_FF00_RDn, joypad.KAPA_JOYP_L1.qp08());
    /* p05.KUVE*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_BYZO_FF00_RDn, joypad.KEJA_JOYP_L2.qp08());
    /* p05.JEKU*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_BYZO_FF00_RDn, joypad.KOLO_JOYP_L3.qp08());
    /* p05.KOCE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_BYZO_FF00_RDn, joypad.KELY_JOYP_UDLR.qn16());
    /* p05.CUDY*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_BYZO_FF00_RDn, joypad.COFY_JOYP_ABCS.qn16());
    /* p??.????*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_BYZO_FF00_RDn, joypad.KUKO_DBG_FF00_D6.qp17());
    /* p??.????*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_BYZO_FF00_RDn, joypad.KERU_DBG_FF00_D7.qp17());
  }

  {

#if 0
    // non-debug-mode

    PIN_JOY_P10.io_pin(1, 0); // hi z
    PIN_JOY_P11.io_pin(1, 0); // hi z
    PIN_JOY_P12.io_pin(1, 0); // hi z
    PIN_JOY_P13.io_pin(1, 0); // hi z

    PIN_JOY_P14.io_pin(1, KELY_JOYP_UDLR.qn()); // open drain
    PIN_JOY_P15.io_pin(1, COFY_JOYP_ABCS.qn()); // open drain
#endif

    wire BURO_FF60_0p = 0; // FIXME hacking out debug stuff
    /*p05.KURA*/ wire _KURA = not1(BURO_FF60_0p);

    /*p05.KOLE*/ wire _KOLE = nand2(joypad.JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
    /*p05.KYBU*/ wire _KYBU = nor2 (joypad.JUTE_JOYP_RA.qp17(), _KURA);
    /*p05.KYTO*/ wire _KYTO = nand2(joypad.KECY_JOYP_LB.qp17(), BURO_FF60_0p);
    /*p05.KABU*/ wire _KABU = nor2 (joypad.KECY_JOYP_LB.qp17(), _KURA);
    /*p05.KYHU*/ wire _KYHU = nand2(joypad.JALE_JOYP_UC.qp17(), BURO_FF60_0p);
    /*p05.KASY*/ wire _KASY = nor2 (joypad.JALE_JOYP_UC.qp17(), _KURA);
    /*p05.KORY*/ wire _KORY = nand2(joypad.KYME_JOYP_DS.qp17(), BURO_FF60_0p);
    /*p05.KALE*/ wire _KALE = nor2 (joypad.KYME_JOYP_DS.qp17(), _KURA);

    joypad.PIN_JOY_P10.io_pin(_KOLE, _KYBU);
    joypad.PIN_JOY_P11.io_pin(_KYTO, _KABU);
    joypad.PIN_JOY_P12.io_pin(_KYHU, _KASY);
    joypad.PIN_JOY_P13.io_pin(_KORY, _KALE);

    /*p05.KARU*/ wire _KARU = or2(joypad.KELY_JOYP_UDLR.qn16(), _KURA);
    /*p05.CELA*/ wire _CELA = or2(joypad.COFY_JOYP_ABCS.qn16(), _KURA);

    /*
    // lcd ribbon voltages after bootrom
    04 5 left & b
    05 0 diodes 1&2
    06 5 down & start
    07 5 up & select
    08 5 right & a
    09 0 diodes 3 & 4
    */

    joypad.PIN_JOY_P14.io_pin(_KARU, joypad.KELY_JOYP_UDLR.qn16());
    joypad.PIN_JOY_P15.io_pin(_CELA, joypad.COFY_JOYP_ABCS.qn16());
  }

  //------------------------------------------------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  // 32 + 4 + 2 + 1 = 39
  /*#p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and4(sprite_scanner.YFEL_SCAN0.qp17(), sprite_scanner.WEWY_SCAN1.qp17(), sprite_scanner.GOSO_SCAN2.qp17(), sprite_scanner.FONY_SCAN5.qp17());

  {
    /*#p29.BYBA*/ sprite_scanner.BYBA_SCAN_DONE_A.dff17(clk_reg.XUPY_ABxxEFxx, sprite_scanner.BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*#p29.DOBA*/ sprite_scanner.DOBA_SCAN_DONE_B.dff17(clk_reg.ALET_xBxDxFxH, sprite_scanner.BAGY_LINE_RSTn, sprite_scanner.BYBA_SCAN_DONE_A.qp17());

    /*#p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or2(clk_reg.ATAR_VID_RSTp, sprite_scanner.AVAP_RENDER_START_TRIGp);
    /*#p28.BESU*/ sprite_scanner.BESU_SCANNINGp.nor_latch(lcd_reg.CATU_LINE_P000.qp17(), _ASEN_SCAN_DONE_PE);
    /*#p29.CENO*/ sprite_scanner.CENO_SCANNINGp.dff17(clk_reg.XUPY_ABxxEFxx, clk_reg.ABEZ_VID_RSTn, sprite_scanner.BESU_SCANNINGp.qp04());
  }

  {
    /*#p30.CYKE*/ wire _CYKE_ABxxEFxx = not1(clk_reg.XUPY_ABxxEFxx);
    /*#p30.WUDA*/ wire _WUDA_xxCDxxGH = not1(_CYKE_ABxxEFxx);

    /* p28.YFOT*/ wire _YFOT_OAM_A2p = not1(oam_bus.BUS_OAM_A2n.qp());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p = not1(oam_bus.BUS_OAM_A3n.qp());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p = not1(oam_bus.BUS_OAM_A4n.qp());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p = not1(oam_bus.BUS_OAM_A5n.qp());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p = not1(oam_bus.BUS_OAM_A6n.qp());
    /* p28.YZET*/ wire _YZET_OAM_A7p = not1(oam_bus.BUS_OAM_A7n.qp());

    /*p30.XADU*/ sprite_scanner.XADU_SPRITE_IDX0p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YFOT_OAM_A2p);
    /*p30.XEDY*/ sprite_scanner.XEDY_SPRITE_IDX1p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YFOC_OAM_A3p);
    /*p30.ZUZE*/ sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YVOM_OAM_A4p);
    /*p30.XOBE*/ sprite_scanner.XOBE_SPRITE_IDX3p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YMEV_OAM_A5p);
    /*p30.YDUF*/ sprite_scanner.YDUF_SPRITE_IDX4p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _XEMU_OAM_A6p);
    /*p30.XECU*/ sprite_scanner.XECU_SPRITE_IDX5p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YZET_OAM_A7p);
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or2(_FETO_SCAN_DONE_d0,   clk_reg.XUPY_ABxxEFxx);

    /*p28.YFEL*/ sprite_scanner.YFEL_SCAN0.dff17(_GAVA_SCAN_CLK,                       sprite_scanner.ANOM_LINE_RSTn, sprite_scanner.YFEL_SCAN0.qn16());
    /*p28.WEWY*/ sprite_scanner.WEWY_SCAN1.dff17(sprite_scanner.YFEL_SCAN0.qn16(), sprite_scanner.ANOM_LINE_RSTn, sprite_scanner.WEWY_SCAN1.qn16());
    /*p28.GOSO*/ sprite_scanner.GOSO_SCAN2.dff17(sprite_scanner.WEWY_SCAN1.qn16(), sprite_scanner.ANOM_LINE_RSTn, sprite_scanner.GOSO_SCAN2.qn16());
    /*p28.ELYN*/ sprite_scanner.ELYN_SCAN3.dff17(sprite_scanner.GOSO_SCAN2.qn16(), sprite_scanner.ANOM_LINE_RSTn, sprite_scanner.ELYN_SCAN3.qn16());
    /*p28.FAHA*/ sprite_scanner.FAHA_SCAN4.dff17(sprite_scanner.ELYN_SCAN3.qn16(), sprite_scanner.ANOM_LINE_RSTn, sprite_scanner.FAHA_SCAN4.qn16());
    /*p28.FONY*/ sprite_scanner.FONY_SCAN5.dff17(sprite_scanner.FAHA_SCAN4.qn16(), sprite_scanner.ANOM_LINE_RSTn, sprite_scanner.FONY_SCAN5.qn16());
  }

  //------------------------------------------------------------------------------
  // lcd_reg.tock();

  /* p01.LYHA*/ wire _LYHA_VID_RSTp = not1(clk_reg.XAPO_VID_RSTn);

  /*#p28.AWOH*/ wire _AWOH_xxCDxxGH = not1(clk_reg.XUPY_ABxxEFxx);
  /*#p21.SONO*/ wire _SONO_ABxxxxGH = not1(clk_reg.TALU_xxCDEFxx);

  /*#p21.XYVO*/ wire _XYVO_IN_VBLANKp = and2(lcd_reg.LOVU_Y4p.qp17(), lcd_reg.LAFO_Y7p.qp17()); // 128 + 16 = 144
  /*#p29.ALES*/ wire _ALES_IN_VBLANKn = not1(_XYVO_IN_VBLANKp);

  /* p01.LYFE*/ wire _LYFE_LCD_RSTn = not1(_LYHA_VID_RSTp);
  /*#p21.POPU*/ lcd_reg.POPU_IN_VBLANKp.dff17(lcd_reg.NYPE_LINE_P002.qp17(), _LYFE_LCD_RSTn, _XYVO_IN_VBLANKp);

  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(pix_pipe.XONA_LCDC_LCDENn.qn08());

  {
    // LCD main timer, 912 phases per line

    /*#p21.MUDE*/ wire _MUDE_X_RSTn = nor2(lcd_reg.RUTU_LINE_P910.qp17(), _LYHA_VID_RSTp);
    /*#p21.SAXO*/ lcd_reg.SAXO_X0p.dff17(clk_reg.TALU_xxCDEFxx,   _MUDE_X_RSTn, lcd_reg.SAXO_X0p.qn16());
    /*#p21.TYPO*/ lcd_reg.TYPO_X1p.dff17(lcd_reg.SAXO_X0p.qn16(), _MUDE_X_RSTn, lcd_reg.TYPO_X1p.qn16());
    /*#p21.VYZO*/ lcd_reg.VYZO_X2p.dff17(lcd_reg.TYPO_X1p.qn16(), _MUDE_X_RSTn, lcd_reg.VYZO_X2p.qn16());
    /*#p21.TELU*/ lcd_reg.TELU_X3p.dff17(lcd_reg.VYZO_X2p.qn16(), _MUDE_X_RSTn, lcd_reg.TELU_X3p.qn16());
    /*#p21.SUDE*/ lcd_reg.SUDE_X4p.dff17(lcd_reg.TELU_X3p.qn16(), _MUDE_X_RSTn, lcd_reg.SUDE_X4p.qn16());
    /*#p21.TAHA*/ lcd_reg.TAHA_X5p.dff17(lcd_reg.SUDE_X4p.qn16(), _MUDE_X_RSTn, lcd_reg.TAHA_X5p.qn16());
    /*#p21.TYRY*/ lcd_reg.TYRY_X6p.dff17(lcd_reg.TAHA_X5p.qn16(), _MUDE_X_RSTn, lcd_reg.TYRY_X6p.qn16());

    /*#p21.LAMA*/ wire _LAMA_FRAME_RSTn = nor2(lcd_reg.MYTA_LINE_153p.qp17(), _LYHA_VID_RSTp);
    /*#p21.MUWY*/ lcd_reg.MUWY_Y0p.dff17(lcd_reg.RUTU_LINE_P910.qp17(), _LAMA_FRAME_RSTn, lcd_reg.MUWY_Y0p.qn16());
    /*#p21.MYRO*/ lcd_reg.MYRO_Y1p.dff17(lcd_reg.MUWY_Y0p.qn16(),       _LAMA_FRAME_RSTn, lcd_reg.MYRO_Y1p.qn16());
    /*#p21.LEXA*/ lcd_reg.LEXA_Y2p.dff17(lcd_reg.MYRO_Y1p.qn16(),       _LAMA_FRAME_RSTn, lcd_reg.LEXA_Y2p.qn16());
    /*#p21.LYDO*/ lcd_reg.LYDO_Y3p.dff17(lcd_reg.LEXA_Y2p.qn16(),       _LAMA_FRAME_RSTn, lcd_reg.LYDO_Y3p.qn16());
    /*#p21.LOVU*/ lcd_reg.LOVU_Y4p.dff17(lcd_reg.LYDO_Y3p.qn16(),       _LAMA_FRAME_RSTn, lcd_reg.LOVU_Y4p.qn16());
    /*#p21.LEMA*/ lcd_reg.LEMA_Y5p.dff17(lcd_reg.LOVU_Y4p.qn16(),       _LAMA_FRAME_RSTn, lcd_reg.LEMA_Y5p.qn16());
    /*#p21.MATO*/ lcd_reg.MATO_Y6p.dff17(lcd_reg.LEMA_Y5p.qn16(),       _LAMA_FRAME_RSTn, lcd_reg.MATO_Y6p.qn16());
    /*#p21.LAFO*/ lcd_reg.LAFO_Y7p.dff17(lcd_reg.MATO_Y6p.qn16(),       _LAMA_FRAME_RSTn, lcd_reg.LAFO_Y7p.qn16());
  }

  {
    /*#p21.SANU*/ wire _SANU_x113p = and4(lcd_reg.TYRY_X6p.qp17(), lcd_reg.TAHA_X5p.qp17(), lcd_reg.SUDE_X4p.qp17(), lcd_reg.SAXO_X0p.qp17()); // 113 = 64 + 32 + 16 + 1, schematic is wrong

    /*#p21.RUTU*/ lcd_reg.RUTU_LINE_P910.dff17(_SONO_ABxxxxGH, _LYFE_LCD_RSTn, _SANU_x113p);
    /*#p21.NYPE*/ lcd_reg.NYPE_LINE_P002.dff17(clk_reg.TALU_xxCDEFxx, _LYFE_LCD_RSTn, lcd_reg.RUTU_LINE_P910.qp17());

    /*#p29.ABOV*/ wire _ABOV_VID_LINE_P908p = and2(lcd_reg.SELA_LINE_P908p, _ALES_IN_VBLANKn);

    /*#p29.CATU*/ lcd_reg.CATU_LINE_P000.dff17(clk_reg.XUPY_ABxxEFxx, clk_reg.ABEZ_VID_RSTn, _ABOV_VID_LINE_P908p);
    /*#p28.ANEL*/ lcd_reg.ANEL_LINE_P002.dff17(_AWOH_xxCDxxGH, clk_reg.ABEZ_VID_RSTn, lcd_reg.CATU_LINE_P000.qp17());

    /*#p21.NOKO*/ wire _NOKO_LINE_153 = and4(lcd_reg.LAFO_Y7p.qp17(), lcd_reg.LOVU_Y4p.qp17(), lcd_reg.LYDO_Y3p.qp17(), lcd_reg.MUWY_Y0p.qp17()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.MYTA*/ lcd_reg.MYTA_LINE_153p.dff17(lcd_reg.NYPE_LINE_P002.qn16(), _LYFE_LCD_RSTn, _NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)

    /*#p24.LOFU*/ wire _LOFU_LINE_ENDn = not1(lcd_reg.RUTU_LINE_P910.qp17());
    /*#p24.LUCA*/ lcd_reg.LUCA_LINE_EVEN .dff17(_LOFU_LINE_ENDn,                    _LYFE_LCD_RSTn, lcd_reg.LUCA_LINE_EVEN.qn16());
    /*#p21.NAPO*/ lcd_reg.NAPO_FRAME_EVEN.dff17(lcd_reg.POPU_IN_VBLANKp.qp17(), _LYFE_LCD_RSTn, lcd_reg.NAPO_FRAME_EVEN.qn16());

    /*#p24.MAGU*/ wire _MAGU = xor2(lcd_reg.NAPO_FRAME_EVEN.qp17(), lcd_reg.LUCA_LINE_EVEN.qn16());
    /*#p24.MECO*/ wire _MECO = not1(_MAGU);
    /*#p24.KEBO*/ wire _KEBO = not1(_MECO);

    /*#p24.USEC*/ wire _USEC_DIV_07p = not1(tim_reg.UREK_DIV_07n);
    /*#p24.KUPA*/ wire _KUPA = amux2(pix_pipe.XONA_LCDC_LCDENn.qn08(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV_07p);
    /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);

    PIN_LCD_FLIPS.io_pin(_KOFO, _KOFO);
  }

  {
    /*#p21.TOCU*/ wire _TOCU_C0n = not1(lcd_reg.SAXO_X0p.qp17());
    /*#p21.VEPE*/ wire _VEPE_C1n = not1(lcd_reg.TYPO_X1p.qp17());
    /* p21.VUTY*/ wire _VUTY_C2n = not1(lcd_reg.VYZO_X2p.qp17());
    /* p21.VATE*/ wire _VATE_C3n = not1(lcd_reg.TELU_X3p.qp17());
    /* p21.TUDA*/ wire _TUDA_C4n = not1(lcd_reg.SUDE_X4p.qp17());
    /* p21.TAFY*/ wire _TAFY_C5n = not1(lcd_reg.TAHA_X5p.qp17());
    /* p21.TUJU*/ wire _TUJU_C6n = not1(lcd_reg.TYRY_X6p.qp17());

    /* p21.VOKU*/ wire _VOKU_000n = nand7(_TUJU_C6n,       _TAFY_C5n,       _TUDA_C4n,       _VATE_C3n,       _VUTY_C2n,       _VEPE_C1n,       _TOCU_C0n);       // 0000000 == 0
    /* p21.TOZU*/ wire _TOZU_007n = nand7(_TUJU_C6n,       _TAFY_C5n,       _TUDA_C4n,       _VATE_C3n,       lcd_reg.VYZO_X2p.qp17(), lcd_reg.TYPO_X1p.qp17(), lcd_reg.SAXO_X0p.qp17()); // 0000111 == 7
    /* p21.TECE*/ wire _TECE_045n = nand7(_TUJU_C6n,       lcd_reg.TAHA_X5p.qp17(), _TUDA_C4n,       lcd_reg.TELU_X3p.qp17(), lcd_reg.VYZO_X2p.qp17(), _VEPE_C1n,       lcd_reg.SAXO_X0p.qp17()); // 0101101 == 45
    /*#p21.TEBO*/ wire _TEBO_083n = nand7(lcd_reg.TYRY_X6p.qp17(), _TAFY_C5n,       lcd_reg.SUDE_X4p.qp17(), _VATE_C3n,       _VUTY_C2n,       lcd_reg.TYPO_X1p.qp17(), lcd_reg.SAXO_X0p.qp17()); // 1010011 == 83

    /*#p21.TEGY*/ wire _TEGY_LINE_STROBE = nand4(_VOKU_000n, _TOZU_007n, _TECE_045n, _TEBO_083n);
    /*#p21.SYGU*/ lcd_reg.SYGU_LINE_STROBE.dff17(_SONO_ABxxxxGH, _LYFE_LCD_RSTn, _TEGY_LINE_STROBE);

    /*#p21.RYNO*/ wire _RYNO = or2(lcd_reg.SYGU_LINE_STROBE.qp17(), lcd_reg.RUTU_LINE_P910.qp17());
    /*#p21.POGU*/ wire _POGU = not1(_RYNO);

    PIN_LCD_CNTRL.io_pin(_POGU, _POGU);
  }

  {

    /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(lcd_reg.PURE_LINE_P908n);

    /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(tim_reg.UMEK_DIV_06n);
    /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(pix_pipe.XONA_LCDC_LCDENn.qn08(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, UMOB_DIV_06p);
    /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
    PIN_LCD_LATCH.io_pin(_KYMO_LINE_ENDn, _KYMO_LINE_ENDn);
  }

  // LCD vertical sync pin
  {
    // NERU looks a little odd, not 100% positive it's a big nor but it does make sense as one
    /*#p24.NERU*/ wire _NERU_LINE_000p = nor8(lcd_reg.LAFO_Y7p.qp17(), lcd_reg.LOVU_Y4p.qp17(), lcd_reg.LYDO_Y3p.qp17(), lcd_reg.MUWY_Y0p.qp17(),
                                              lcd_reg.MYRO_Y1p.qp17(), lcd_reg.LEXA_Y2p.qp17(), lcd_reg.LEMA_Y5p.qp17(), lcd_reg.MATO_Y6p.qp17());

    /*#p24.MEDA*/ lcd_reg.MEDA_VSYNC_OUTn.dff17(lcd_reg.NYPE_LINE_P002.qn16(), _LYFE_LCD_RSTn, _NERU_LINE_000p);
    /*#p24.MURE*/ wire _MURE_VSYNC = not1(lcd_reg.MEDA_VSYNC_OUTn.qp17());

    /*#*/ PIN_LCD_VSYNC.io_pin(_MURE_VSYNC, _MURE_VSYNC);
  }

  // ly match
  {
    /* p21.RYME*/ wire _RYME_LY_MATCH0n = xor2(lcd_reg.MUWY_Y0p.qp17(), lcd_reg.SYRY_LYC0n.qn08());
    /* p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor2(lcd_reg.MYRO_Y1p.qp17(), lcd_reg.VUCE_LYC1n.qn08());
    /* p21.REDA*/ wire _REDA_LY_MATCH2n = xor2(lcd_reg.LEXA_Y2p.qp17(), lcd_reg.SEDY_LYC2n.qn08());
    /* p21.RASY*/ wire _RASY_LY_MATCH3n = xor2(lcd_reg.LYDO_Y3p.qp17(), lcd_reg.SALO_LYC3n.qn08());
    /* p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor2(lcd_reg.LOVU_Y4p.qp17(), lcd_reg.SOTA_LYC4n.qn08());
    /* p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor2(lcd_reg.LEMA_Y5p.qp17(), lcd_reg.VAFA_LYC5n.qn08());
    /* p21.TERY*/ wire _TERY_LY_MATCH6n = xor2(lcd_reg.MATO_Y6p.qp17(), lcd_reg.VEVO_LYC6n.qn08());
    /* p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor2(lcd_reg.LAFO_Y7p.qp17(), lcd_reg.RAHA_LYC7n.qn08());

    /* p21.SOVU*/ wire _SOVU_LY_MATCHA = nor4 (_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor4
    /* p21.SUBO*/ wire _SUBO_LY_MATCHB = nor4 (_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor4
    /* p21.RAPE*/ wire _RAPE_LY_MATCHn = nand2(_SOVU_LY_MATCHA,  _SUBO_LY_MATCHB); // def nand2
    /* p21.PALY*/ wire _PALY_LY_MATCHa = not1(_RAPE_LY_MATCHn); // def not

    //probe(4, "PALY_LY_MATCHA", _PALY_LY_MATCHa);

    /*#p21.ROPO*/ lcd_reg.ROPO_LY_MATCH_SYNCp.dff17(clk_reg.TALU_xxCDEFxx, clk_reg.WESY_SYS_RSTn, _PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /* p22.WYLE*/ wire _WYLE_FF44n    = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.XENO_A01n(), cpu_bus.WALO_A02p(), cpu_bus.XERA_A03n());
    /* p22.XOGY*/ wire _XOGY_FF44p    = not1(_WYLE_FF44n);
    /* p23.WAFU*/ wire _WAFU_FF44_RDp = and2(ASOT_CPU_RDp, _XOGY_FF44p);
    /* p23.VARO*/ wire _VARO_FF44_RDn = not1(_WAFU_FF44_RDp);

    /*#p23.WURY*/ wire _WURY_LY0n = not1(lcd_reg.MUWY_Y0p.qp17());
    /* p23.XEPO*/ wire _XEPO_LY1n = not1(lcd_reg.MYRO_Y1p.qp17());
    /* p23.MYFA*/ wire _MYFA_LY2n = not1(lcd_reg.LEXA_Y2p.qp17());
    /* p23.XUHY*/ wire _XUHY_LY3n = not1(lcd_reg.LYDO_Y3p.qp17());
    /* p23.WATA*/ wire _WATA_LY4n = not1(lcd_reg.LOVU_Y4p.qp17());
    /* p23.XAGA*/ wire _XAGA_LY5n = not1(lcd_reg.LEMA_Y5p.qp17());
    /* p23.XUCE*/ wire _XUCE_LY6n = not1(lcd_reg.MATO_Y6p.qp17());
    /* p23.XOWO*/ wire _XOWO_LY7n = not1(lcd_reg.LAFO_Y7p.qp17());

    /* p23.VEGA*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_VARO_FF44_RDn, _WURY_LY0n);
    /* p23.WUVA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_VARO_FF44_RDn, _XEPO_LY1n);
    /* p23.LYCO*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_VARO_FF44_RDn, _MYFA_LY2n);
    /* p23.WOJY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_VARO_FF44_RDn, _XUHY_LY3n);
    /* p23.VYNE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_VARO_FF44_RDn, _WATA_LY4n);
    /* p23.WAMA*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_VARO_FF44_RDn, _XAGA_LY5n);
    /* p23.WAVO*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_VARO_FF44_RDn, _XUCE_LY6n);
    /* p23.WEZE*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_VARO_FF44_RDn, _XOWO_LY7n);
  }

  // FF45 LYC
  {
    /* p22.WETY*/ wire _WETY_FF45n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.XENO_A01n(), cpu_bus.WALO_A02p(), cpu_bus.XERA_A03n());
    /* p22.XAYU*/ wire _XAYU_FF45p = not1(_WETY_FF45n);

    /* p23.XYLY*/ wire _XYLY_FF45_RDp = and2(ASOT_CPU_RDp, _XAYU_FF45p);
    /* p23.WEKU*/ wire _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);

    /* p23.XUFA*/ wire _XUFA_FF45_WRn = and2(CUPA_CPU_WRp_xxxxEFGx, _XAYU_FF45p);
    /* p23.WANE*/ wire _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);

    /* p23.SYRY*/ lcd_reg.SYRY_LYC0n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /* p23.VUCE*/ lcd_reg.VUCE_LYC1n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /* p23.SEDY*/ lcd_reg.SEDY_LYC2n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /* p23.SALO*/ lcd_reg.SALO_LYC3n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /* p23.SOTA*/ lcd_reg.SOTA_LYC4n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /* p23.VAFA*/ lcd_reg.VAFA_LYC5n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /* p23.VEVO*/ lcd_reg.VEVO_LYC6n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /* p23.RAHA*/ lcd_reg.RAHA_LYC7n.dff9(_WANE_FF45_WRp, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /*#p23.RETU*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.SYRY_LYC0n.qp09());
    /* p23.VOJO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.VUCE_LYC1n.qp09());
    /* p23.RAZU*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.SEDY_LYC2n.qp09());
    /* p23.REDY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.SALO_LYC3n.qp09());
    /* p23.RACE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.SOTA_LYC4n.qp09());
    /*#p23.VAZU*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.VAFA_LYC5n.qp09());
    /* p23.VAFE*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.VEVO_LYC6n.qp09());
    /* p23.PUFY*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_WEKU_FF45_RDn, lcd_reg.RAHA_LYC7n.qp09());
  }

  //------------------------------------------------------------------------------

  /* p01.AMYG*/ wire _AMYG_VID_RSTp = not1(clk_reg.XAPO_VID_RSTn);

  /* p29.DYTY*/ wire _DYTY_STORE_ENn_xxCDxxGH = not1(sprite_scanner.CARE_STORE_ENp_ABxxEFxx);

  /*#p28.AZYB*/ wire _AZYB_VID_LINE_TRIGn = not1(lcd_reg.ATEJ_LINE_TRIGp);
  /* p28.ABAK*/ wire _ABAK_VID_LINE_TRIGp = or2(lcd_reg.ATEJ_LINE_TRIGp, _AMYG_VID_RSTp);
  /* p28.BYVA*/ wire _BYVA_VID_LINE_TRIGn = not1(_ABAK_VID_LINE_TRIGp);
  /* p29.DYBA*/ wire _DYBA_VID_LINE_TRIGp = not1(_BYVA_VID_LINE_TRIGn);

  /*#p31.ZAGO*/ wire _ZAGO_X0n = not1(oam_bus.YLOR_OAM_DB0p.qp08());
  /* p31.ZOCY*/ wire _ZOCY_X1n = not1(oam_bus.ZYTY_OAM_DB1p.qp08());
  /* p31.YPUR*/ wire _YPUR_X2n = not1(oam_bus.ZYVE_OAM_DB2p.qp08());
  /* p31.YVOK*/ wire _YVOK_X3n = not1(oam_bus.ZEZY_OAM_DB3p.qp08());
  /* p31.COSE*/ wire _COSE_X4n = not1(oam_bus.GOMO_OAM_DB4p.qp08());
  /* p31.AROP*/ wire _AROP_X5n = not1(oam_bus.BAXO_OAM_DB5p.qp08());
  /* p31.XATU*/ wire _XATU_X6n = not1(oam_bus.YZOS_OAM_DB6p.qp08());
  /* p31.BADY*/ wire _BADY_X7n = not1(oam_bus.DEPO_OAM_DB7p.qp08());

  //----------------------------------------
  // Sprite scan Y matcher

  {
    // Sprite store counter.
    // The sprite count clock stops ticking once we have 10 sprites.

    /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL = and2(sprite_store.CUXY_SPRITE_COUNT1.qp17(), sprite_store.DYBE_SPRITE_COUNT3.qp17());
    /*#p29.CAKE*/ wire _CAKE_CLKp = or2(_BAKY_SPRITES_FULL, sprite_store.DEZY_STORE_ENn.qp17());

    /*p29.BESE*/ sprite_store.BESE_SPRITE_COUNT0.dff17(_CAKE_CLKp,                                 _AZYB_VID_LINE_TRIGn, sprite_store.BESE_SPRITE_COUNT0.qn16());
    /*p29.CUXY*/ sprite_store.CUXY_SPRITE_COUNT1.dff17(sprite_store.BESE_SPRITE_COUNT0.qn16(), _AZYB_VID_LINE_TRIGn, sprite_store.CUXY_SPRITE_COUNT1.qn16());
    /*p29.BEGO*/ sprite_store.BEGO_SPRITE_COUNT2.dff17(sprite_store.CUXY_SPRITE_COUNT1.qn16(), _AZYB_VID_LINE_TRIGn, sprite_store.BEGO_SPRITE_COUNT2.qn16());
    /*p29.DYBE*/ sprite_store.DYBE_SPRITE_COUNT3.dff17(sprite_store.BEGO_SPRITE_COUNT2.qn16(), _AZYB_VID_LINE_TRIGn, sprite_store.DYBE_SPRITE_COUNT3.qn16());
  }

  //----------------------------------------

  {
    // FEPO_STORE_MATCHp here is weird, I guess it's just an easy signal to use to mux the bus?

    /*#p30.CUCU*/ sprite_store.SPR_TRI_L0.tri_6nn(sprite_store.FEPO_STORE_MATCHp, sprite_scanner.DEGE_SPRITE_DELTA0);
    /*#p30.CUCA*/ sprite_store.SPR_TRI_L1.tri_6nn(sprite_store.FEPO_STORE_MATCHp, sprite_scanner.DABY_SPRITE_DELTA1);
    /*#p30.CEGA*/ sprite_store.SPR_TRI_L2.tri_6nn(sprite_store.FEPO_STORE_MATCHp, sprite_scanner.DABU_SPRITE_DELTA2);
    /*#p30.WENU*/ sprite_store.SPR_TRI_L3.tri_6nn(sprite_store.FEPO_STORE_MATCHp, sprite_scanner.GYSA_SPRITE_DELTA3);

    /* p29.DEZY*/ sprite_store.DEZY_STORE_ENn.dff17(clk_reg.ZEME_AxCxExGx, clk_reg.XAPO_VID_RSTn, _DYTY_STORE_ENn_xxCDxxGH);
  }

  //----------------------------------------
  // Sprite store getter

  {
    /*p29.WEFU*/ wire WEFU_STORE0_MATCH = not1(sprite_store.YDUG_STORE0_MATCHn);
    /*p29.GAJA*/ wire GAJA_STORE1_MATCH = not1(sprite_store.DYDU_STORE1_MATCHn);
    /*p29.GUPO*/ wire GUPO_STORE2_MATCH = not1(sprite_store.DEGO_STORE2_MATCHn);
    /*p29.WEBO*/ wire WEBO_STORE3_MATCH = not1(sprite_store.YLOZ_STORE3_MATCHn);
    /*p29.WUNA*/ wire WUNA_STORE4_MATCH = not1(sprite_store.XAGE_STORE4_MATCHn);
    /*p29.GABA*/ wire GABA_STORE5_MATCH = not1(sprite_store.EGOM_STORE5_MATCHn);
    /*p29.WASE*/ wire WASE_STORE6_MATCH = not1(sprite_store.YBEZ_STORE6_MATCHn);
    /*p29.GYTE*/ wire GYTE_STORE7_MATCH = not1(sprite_store.DYKA_STORE7_MATCHn);
    /*p29.GEKE*/ wire GEKE_STORE8_MATCH = not1(sprite_store.EFYL_STORE8_MATCHn);

    // Priority encoder so we fetch the first sprite that matches

    /*p29.GEZE*/ wire GEZE_STORE0_MATCH_OUT = or2(WEFU_STORE0_MATCH, GND);
    /*p29.FUMA*/ wire FUMA_STORE1_MATCH_OUT = or2(GAJA_STORE1_MATCH, GEZE_STORE0_MATCH_OUT);
    /*p29.GEDE*/ wire GEDE_STORE2_MATCH_OUT = or2(GUPO_STORE2_MATCH, FUMA_STORE1_MATCH_OUT);
    /*p29.WUTO*/ wire WUTO_STORE3_MATCH_OUT = or2(WEBO_STORE3_MATCH, GEDE_STORE2_MATCH_OUT);
    /*p29.XYLA*/ wire XYLA_STORE4_MATCH_OUT = or2(WUNA_STORE4_MATCH, WUTO_STORE3_MATCH_OUT);
    /*p29.WEJA*/ wire WEJA_STORE5_MATCH_OUT = or2(GABA_STORE5_MATCH, XYLA_STORE4_MATCH_OUT);
    /*p29.WYLA*/ wire WYLA_STORE6_MATCH_OUT = or2(WASE_STORE6_MATCH, WEJA_STORE5_MATCH_OUT);
    /*p29.FAVO*/ wire FAVO_STORE7_MATCH_OUT = or2(GYTE_STORE7_MATCH, WYLA_STORE6_MATCH_OUT);
    /*p29.GYGA*/ wire GYGA_STORE8_MATCH_OUT = or2(GEKE_STORE8_MATCH, FAVO_STORE7_MATCH_OUT);

    /*p29.GUVA*/ wire GUVA_SPRITE0_GETp = nor2(sprite_store.YDUG_STORE0_MATCHn, GND);
    /*p29.ENUT*/ wire ENUT_SPRITE1_GETp = nor2(sprite_store.DYDU_STORE1_MATCHn, GEZE_STORE0_MATCH_OUT);
    /*p29.EMOL*/ wire EMOL_SPRITE2_GETp = nor2(sprite_store.DEGO_STORE2_MATCHn, FUMA_STORE1_MATCH_OUT);
    /*p29.GYFY*/ wire GYFY_SPRITE3_GETp = nor2(sprite_store.YLOZ_STORE3_MATCHn, GEDE_STORE2_MATCH_OUT);
    /*p29.GONO*/ wire GONO_SPRITE4_GETp = nor2(sprite_store.XAGE_STORE4_MATCHn, WUTO_STORE3_MATCH_OUT);
    /*p29.GEGA*/ wire GEGA_SPRITE5_GETp = nor2(sprite_store.EGOM_STORE5_MATCHn, XYLA_STORE4_MATCH_OUT);
    /*p29.XOJA*/ wire XOJA_SPRITE6_GETp = nor2(sprite_store.YBEZ_STORE6_MATCHn, WEJA_STORE5_MATCH_OUT);
    /*p29.GUTU*/ wire GUTU_SPRITE7_GETp = nor2(sprite_store.DYKA_STORE7_MATCHn, WYLA_STORE6_MATCH_OUT);
    /*p29.FOXA*/ wire FOXA_SPRITE8_GETp = nor2(sprite_store.EFYL_STORE8_MATCHn, FAVO_STORE7_MATCH_OUT);
    /*p29.GUZE*/ wire GUZE_SPRITE9_GETp = nor2(sprite_store.YGEM_STORE9_MATCHn, GYGA_STORE8_MATCH_OUT);

    // Delayed reset signal for the selected store once sprite fetch is done.

    /*p29.EBOJ*/ sprite_store.EBOJ_STORE0_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GUVA_SPRITE0_GETp);
    /*p29.CEDY*/ sprite_store.CEDY_STORE1_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, ENUT_SPRITE1_GETp);
    /*p29.EGAV*/ sprite_store.EGAV_STORE2_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, EMOL_SPRITE2_GETp);
    /*p29.GOTA*/ sprite_store.GOTA_STORE3_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GYFY_SPRITE3_GETp);
    /*p29.XUDY*/ sprite_store.XUDY_STORE4_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GONO_SPRITE4_GETp);
    /*p29.WAFY*/ sprite_store.WAFY_STORE5_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GEGA_SPRITE5_GETp);
    /*p29.WOMY*/ sprite_store.WOMY_STORE6_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, XOJA_SPRITE6_GETp);
    /*p29.WAPO*/ sprite_store.WAPO_STORE7_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GUTU_SPRITE7_GETp);
    /*p29.EXUQ*/ sprite_store.EXUQ_STORE8_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, FOXA_SPRITE8_GETp);
    /*p29.FONO*/ sprite_store.FONO_STORE9_RSTp.dff17(sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GUZE_SPRITE9_GETp);

    // Tribus drivers for all 10 sprite stores

    /*p29.FURO*/ wire FURO_SPRITE0_GETn = not1(GUVA_SPRITE0_GETp);
    /*p29.DYDO*/ wire DYDO_SPRITE1_GETn = not1(ENUT_SPRITE1_GETp);
    /*p29.FAME*/ wire FAME_SPRITE2_GETn = not1(EMOL_SPRITE2_GETp);
    /*p29.GYMA*/ wire GYMA_SPRITE3_GETn = not1(GYFY_SPRITE3_GETp);
    /*p29.GOWO*/ wire GOWO_SPRITE4_GETn = not1(GONO_SPRITE4_GETp);
    /*p29.GYGY*/ wire GYGY_SPRITE5_GETn = not1(GEGA_SPRITE5_GETp);
    /*p29.XYME*/ wire XYME_SPRITE6_GETn = not1(XOJA_SPRITE6_GETp);
    /*p29.GUGY*/ wire GUGY_SPRITE7_GETn = not1(GUTU_SPRITE7_GETp);
    /*p29.DENY*/ wire DENY_SPRITE8_GETn = not1(FOXA_SPRITE8_GETp);
    /*p29.FADO*/ wire FADO_SPRITE9_GETn = not1(GUZE_SPRITE9_GETp);

    /* p30.ZETU*/ sprite_store.SPR_TRI_I0p.tri_6nn(FURO_SPRITE0_GETn, sprite_store.YGUS_STORE0_I0n.qp08());
    /* p30.ZECE*/ sprite_store.SPR_TRI_I1p.tri_6nn(FURO_SPRITE0_GETn, sprite_store.YSOK_STORE0_I1n.qp08());
    /* p30.ZAVE*/ sprite_store.SPR_TRI_I2p.tri_6nn(FURO_SPRITE0_GETn, sprite_store.YZEP_STORE0_I2n.qp08());
    /* p30.WOKO*/ sprite_store.SPR_TRI_I3p.tri_6nn(FURO_SPRITE0_GETn, sprite_store.WYTE_STORE0_I3n.qp08());
    /* p30.ZUMU*/ sprite_store.SPR_TRI_I4p.tri_6nn(FURO_SPRITE0_GETn, sprite_store.ZONY_STORE0_I4n.qp08());
    /*#p30.ZEDY*/ sprite_store.SPR_TRI_I5p.tri_6nn(FURO_SPRITE0_GETn, sprite_store.YWAK_STORE0_I5n.qp08());

    /*#p30.CUBO*/ sprite_store.SPR_TRI_I0p.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.CADU_STORE1_I0n.qp08());
    /* p30.CELU*/ sprite_store.SPR_TRI_I1p.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.CEBO_STORE1_I1n.qp08());
    /* p30.CEGY*/ sprite_store.SPR_TRI_I2p.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.CUFA_STORE1_I2n.qp08());
    /* p30.BETY*/ sprite_store.SPR_TRI_I3p.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.COMA_STORE1_I3n.qp08());
    /* p30.CYBY*/ sprite_store.SPR_TRI_I4p.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.CUZA_STORE1_I4n.qp08());
    /* p30.BEMO*/ sprite_store.SPR_TRI_I5p.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.CAJY_STORE1_I5n.qp08());

    /* p30.CUBE*/ sprite_store.SPR_TRI_I0p.tri_6nn(FAME_SPRITE2_GETn, sprite_store.BUHE_STORE2_I0n.qp08());
    /* p30.AFOZ*/ sprite_store.SPR_TRI_I1p.tri_6nn(FAME_SPRITE2_GETn, sprite_store.BYHU_STORE2_I1n.qp08());
    /* p30.APON*/ sprite_store.SPR_TRI_I2p.tri_6nn(FAME_SPRITE2_GETn, sprite_store.BECA_STORE2_I2n.qp08());
    /* p30.CUVU*/ sprite_store.SPR_TRI_I3p.tri_6nn(FAME_SPRITE2_GETn, sprite_store.BULU_STORE2_I3n.qp08());
    /* p30.CYRO*/ sprite_store.SPR_TRI_I4p.tri_6nn(FAME_SPRITE2_GETn, sprite_store.BUNA_STORE2_I4n.qp08());
    /* p30.AXEC*/ sprite_store.SPR_TRI_I5p.tri_6nn(FAME_SPRITE2_GETn, sprite_store.BOXA_STORE2_I5n.qp08());

    /* p30.ENAP*/ sprite_store.SPR_TRI_I0p.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.DEVY_STORE3_I0n.qp08());
    /* p30.DYGO*/ sprite_store.SPR_TRI_I1p.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.DESE_STORE3_I1n.qp08());
    /* p30.DOWA*/ sprite_store.SPR_TRI_I2p.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.DUNY_STORE3_I2n.qp08());
    /* p30.DONY*/ sprite_store.SPR_TRI_I3p.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.DUHA_STORE3_I3n.qp08());
    /* p30.EFUD*/ sprite_store.SPR_TRI_I4p.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.DEBA_STORE3_I4n.qp08());
    /* p30.DEZU*/ sprite_store.SPR_TRI_I5p.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.DAFU_STORE3_I5n.qp08());

    /* p30.WUXU*/ sprite_store.SPR_TRI_I0p.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.XAVE_STORE4_I0n.qp08());
    /* p30.WEPY*/ sprite_store.SPR_TRI_I1p.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.XEFE_STORE4_I1n.qp08());
    /* p30.WERU*/ sprite_store.SPR_TRI_I2p.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.WANU_STORE4_I2n.qp08());
    /* p30.XYRE*/ sprite_store.SPR_TRI_I3p.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.XABO_STORE4_I3n.qp08());
    /* p30.WOXY*/ sprite_store.SPR_TRI_I4p.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.XEGE_STORE4_I4n.qp08());
    /* p30.WAJA*/ sprite_store.SPR_TRI_I5p.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.XYNU_STORE4_I5n.qp08());

    /* p30.DOBO*/ sprite_store.SPR_TRI_I0p.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.EKOP_STORE5_I0n.qp08());
    /* p30.DYNY*/ sprite_store.SPR_TRI_I1p.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.ETYM_STORE5_I1n.qp08());
    /* p30.WAGA*/ sprite_store.SPR_TRI_I2p.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.GORU_STORE5_I2n.qp08());
    /* p30.DUZA*/ sprite_store.SPR_TRI_I3p.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.EBEX_STORE5_I3n.qp08());
    /* p30.DALY*/ sprite_store.SPR_TRI_I4p.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.ETAV_STORE5_I4n.qp08());
    /* p30.DALO*/ sprite_store.SPR_TRI_I5p.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.EKAP_STORE5_I5n.qp08());

    /* p30.WATO*/ sprite_store.SPR_TRI_I0p.tri_6nn(XYME_SPRITE6_GETn, sprite_store.GABO_STORE6_I0n.qp08());
    /* p30.WYWY*/ sprite_store.SPR_TRI_I1p.tri_6nn(XYME_SPRITE6_GETn, sprite_store.GACY_STORE6_I1n.qp08());
    /* p30.EZOC*/ sprite_store.SPR_TRI_I2p.tri_6nn(XYME_SPRITE6_GETn, sprite_store.FOGO_STORE6_I2n.qp08());
    /* p30.WABO*/ sprite_store.SPR_TRI_I3p.tri_6nn(XYME_SPRITE6_GETn, sprite_store.GOHU_STORE6_I3n.qp08());
    /* p30.ELYC*/ sprite_store.SPR_TRI_I4p.tri_6nn(XYME_SPRITE6_GETn, sprite_store.FOXY_STORE6_I4n.qp08());
    /* p30.WOCY*/ sprite_store.SPR_TRI_I5p.tri_6nn(XYME_SPRITE6_GETn, sprite_store.GECU_STORE6_I5n.qp08());

    /* p30.WAKO*/ sprite_store.SPR_TRI_I0p.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.GULE_STORE7_I0n.qp08());
    /* p30.WYGO*/ sprite_store.SPR_TRI_I1p.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.GYNO_STORE7_I1n.qp08());
    /* p30.ELEP*/ sprite_store.SPR_TRI_I2p.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.FEFA_STORE7_I2n.qp08());
    /* p30.ETAD*/ sprite_store.SPR_TRI_I3p.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.FYSU_STORE7_I3n.qp08());
    /* p30.WABA*/ sprite_store.SPR_TRI_I4p.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.GESY_STORE7_I4n.qp08());
    /* p30.EVYT*/ sprite_store.SPR_TRI_I5p.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.FUZO_STORE7_I5n.qp08());

    /* p30.APOC*/ sprite_store.SPR_TRI_I0p.tri_6nn(DENY_SPRITE8_GETn, sprite_store.AXUV_STORE8_I0n.qp08());
    /* p30.AKYH*/ sprite_store.SPR_TRI_I1p.tri_6nn(DENY_SPRITE8_GETn, sprite_store.BADA_STORE8_I1n.qp08());
    /* p30.AFEN*/ sprite_store.SPR_TRI_I2p.tri_6nn(DENY_SPRITE8_GETn, sprite_store.APEV_STORE8_I2n.qp08());
    /* p30.APYV*/ sprite_store.SPR_TRI_I3p.tri_6nn(DENY_SPRITE8_GETn, sprite_store.BADO_STORE8_I3n.qp08());
    /* p30.APOB*/ sprite_store.SPR_TRI_I4p.tri_6nn(DENY_SPRITE8_GETn, sprite_store.BEXY_STORE8_I4n.qp08());
    /* p30.ADYB*/ sprite_store.SPR_TRI_I5p.tri_6nn(DENY_SPRITE8_GETn, sprite_store.BYHE_STORE8_I5n.qp08());

    /* p30.ZARO*/ sprite_store.SPR_TRI_I0p.tri_6nn(FADO_SPRITE9_GETn, sprite_store.YBER_STORE9_I0n.qp08());
    /* p30.ZOJY*/ sprite_store.SPR_TRI_I1p.tri_6nn(FADO_SPRITE9_GETn, sprite_store.YZOR_STORE9_I1n.qp08());
    /* p30.YNEV*/ sprite_store.SPR_TRI_I2p.tri_6nn(FADO_SPRITE9_GETn, sprite_store.XYFE_STORE9_I2n.qp08());
    /* p30.XYRA*/ sprite_store.SPR_TRI_I3p.tri_6nn(FADO_SPRITE9_GETn, sprite_store.XOTU_STORE9_I3n.qp08());
    /* p30.YRAD*/ sprite_store.SPR_TRI_I4p.tri_6nn(FADO_SPRITE9_GETn, sprite_store.XUTE_STORE9_I4n.qp08());
    /* p30.YHAL*/ sprite_store.SPR_TRI_I5p.tri_6nn(FADO_SPRITE9_GETn, sprite_store.XUFO_STORE9_I5n.qp08());

    /* p30.WEHE*/ sprite_store.SPR_TRI_L0.tri_6nn(FURO_SPRITE0_GETn, sprite_store.GYHO_STORE0_L0n.qp08());
    /* p30.BUKY*/ sprite_store.SPR_TRI_L1.tri_6nn(FURO_SPRITE0_GETn, sprite_store.CUFO_STORE0_L1n.qp08());
    /* p30.AJAL*/ sprite_store.SPR_TRI_L2.tri_6nn(FURO_SPRITE0_GETn, sprite_store.BOZU_STORE0_L2n.qp08());
    /* p30.GOFO*/ sprite_store.SPR_TRI_L3.tri_6nn(FURO_SPRITE0_GETn, sprite_store.FYHY_STORE0_L3n.qp08());

    /* p30.BYRO*/ sprite_store.SPR_TRI_L0.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.AMES_STORE1_L0n.qp08());
    /* p30.AHUM*/ sprite_store.SPR_TRI_L1.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.AROF_STORE1_L1n.qp08());
    /* p30.BACO*/ sprite_store.SPR_TRI_L2.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.ABOP_STORE1_L2n.qp08());
    /* p30.BEFE*/ sprite_store.SPR_TRI_L3.tri_6nn(DYDO_SPRITE1_GETn, sprite_store.ABUG_STORE1_L3n.qp08());

    /* p30.ZUKE*/ sprite_store.SPR_TRI_L0.tri_6nn(FAME_SPRITE2_GETn, sprite_store.YLOV_STORE2_L0n.qp08());
    /* p30.WERE*/ sprite_store.SPR_TRI_L1.tri_6nn(FAME_SPRITE2_GETn, sprite_store.XOSY_STORE2_L1n.qp08());
    /* p30.WUXE*/ sprite_store.SPR_TRI_L2.tri_6nn(FAME_SPRITE2_GETn, sprite_store.XAZY_STORE2_L2n.qp08());
    /* p30.ZABY*/ sprite_store.SPR_TRI_L3.tri_6nn(FAME_SPRITE2_GETn, sprite_store.YKUK_STORE2_L3n.qp08());

    /* p30.ZEXE*/ sprite_store.SPR_TRI_L0.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.ZURO_STORE3_L0n.qp08());
    /* p30.YWAV*/ sprite_store.SPR_TRI_L1.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.ZYLU_STORE3_L1n.qp08());
    /* p30.YJEM*/ sprite_store.SPR_TRI_L2.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.ZENE_STORE3_L2n.qp08());
    /* p30.ZYPO*/ sprite_store.SPR_TRI_L3.tri_6nn(GYMA_SPRITE3_GETn, sprite_store.ZURY_STORE3_L3n.qp08());

    /* p30.BUCE*/ sprite_store.SPR_TRI_L0.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.CAPO_STORE4_L0n.qp08());
    /* p30.BEVY*/ sprite_store.SPR_TRI_L1.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.CAJU_STORE4_L1n.qp08());
    /* p30.BOVE*/ sprite_store.SPR_TRI_L2.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.CONO_STORE4_L2n.qp08());
    /* p30.BYDO*/ sprite_store.SPR_TRI_L3.tri_6nn(GOWO_SPRITE4_GETn, sprite_store.CUMU_STORE4_L3n.qp08());

    /* p30.BACE*/ sprite_store.SPR_TRI_L0.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.ACEP_STORE5_L0n.qp08());
    /* p30.BUJA*/ sprite_store.SPR_TRI_L1.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.ABEG_STORE5_L1n.qp08());
    /* p30.BODU*/ sprite_store.SPR_TRI_L2.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.ABUX_STORE5_L2n.qp08());
    /* p30.AWAT*/ sprite_store.SPR_TRI_L3.tri_6nn(GYGY_SPRITE5_GETn, sprite_store.ANED_STORE5_L3n.qp08());

    /* p30.YBUK*/ sprite_store.SPR_TRI_L0.tri_6nn(XYME_SPRITE6_GETn, sprite_store.ZUMY_STORE6_L0n.qp08());
    /* p30.YKOZ*/ sprite_store.SPR_TRI_L1.tri_6nn(XYME_SPRITE6_GETn, sprite_store.ZAFU_STORE6_L1n.qp08());
    /* p30.ZYTO*/ sprite_store.SPR_TRI_L2.tri_6nn(XYME_SPRITE6_GETn, sprite_store.ZEXO_STORE6_L2n.qp08());
    /* p30.ZUDO*/ sprite_store.SPR_TRI_L3.tri_6nn(XYME_SPRITE6_GETn, sprite_store.ZUBE_STORE6_L3n.qp08());

    /* p30.WAXE*/ sprite_store.SPR_TRI_L0.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.XYNA_STORE7_L0n.qp08());
    /* p30.YPOZ*/ sprite_store.SPR_TRI_L1.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.YGUM_STORE7_L1n.qp08());
    /* p30.WABU*/ sprite_store.SPR_TRI_L2.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.XAKU_STORE7_L2n.qp08());
    /* p30.WANA*/ sprite_store.SPR_TRI_L3.tri_6nn(GUGY_SPRITE7_GETn, sprite_store.XYGO_STORE7_L3n.qp08());

    /* p30.BOSO*/ sprite_store.SPR_TRI_L0.tri_6nn(DENY_SPRITE8_GETn, sprite_store.AZAP_STORE8_L0n.qp08());
    /* p30.BAZU*/ sprite_store.SPR_TRI_L1.tri_6nn(DENY_SPRITE8_GETn, sprite_store.AFYX_STORE8_L1n.qp08());
    /* p30.AHAC*/ sprite_store.SPR_TRI_L2.tri_6nn(DENY_SPRITE8_GETn, sprite_store.AFUT_STORE8_L2n.qp08());
    /* p30.BUJY*/ sprite_store.SPR_TRI_L3.tri_6nn(DENY_SPRITE8_GETn, sprite_store.AFYM_STORE8_L3n.qp08());

    /* p30.BYME*/ sprite_store.SPR_TRI_L0.tri_6nn(FADO_SPRITE9_GETn, sprite_store.CANA_STORE9_L0n.qp08());
    /* p30.GATE*/ sprite_store.SPR_TRI_L1.tri_6nn(FADO_SPRITE9_GETn, sprite_store.FOFO_STORE9_L1n.qp08());
    /* p30.COHO*/ sprite_store.SPR_TRI_L2.tri_6nn(FADO_SPRITE9_GETn, sprite_store.DYSY_STORE9_L2n.qp08());
    /* p30.CAWO*/ sprite_store.SPR_TRI_L3.tri_6nn(FADO_SPRITE9_GETn, sprite_store.DEWU_STORE9_L3n.qp08());
  }

  //----------------------------------------
  // Sprite store setter

  {
    /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.EBOJ_STORE0_RSTp.qp17());
    /*p29.EFEV*/ wire EFEV_STORE1_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.CEDY_STORE1_RSTp.qp17());
    /*p29.FOKO*/ wire FOKO_STORE2_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.EGAV_STORE2_RSTp.qp17());
    /*p29.GAKE*/ wire GAKE_STORE3_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.GOTA_STORE3_RSTp.qp17());
    /*p29.WOHU*/ wire WOHU_STORE4_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.XUDY_STORE4_RSTp.qp17());
    /*p29.FEVE*/ wire FEVE_STORE5_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.WAFY_STORE5_RSTp.qp17());
    /*p29.WACY*/ wire WACY_STORE6_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.WOMY_STORE6_RSTp.qp17());
    /*p29.GUKY*/ wire GUKY_STORE7_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.WAPO_STORE7_RSTp.qp17());
    /*p29.GORO*/ wire GORO_STORE8_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.EXUQ_STORE8_RSTp.qp17());
    /*p29.DUBU*/ wire DUBU_STORE9_RSTp = or2(_DYBA_VID_LINE_TRIGp, sprite_store.FONO_STORE9_RSTp.qp17());

    /*p29.DYNA*/ wire DYNA_STORE0_RSTn = not1(DYWE_STORE0_RSTp);
    /*p29.DOKU*/ wire DOKU_STORE1_RSTn = not1(EFEV_STORE1_RSTp);
    /*p29.GAMY*/ wire GAMY_STORE2_RSTn = not1(FOKO_STORE2_RSTp);
    /*p29.WUPA*/ wire WUPA_STORE3_RSTn = not1(GAKE_STORE3_RSTp);
    /*p29.WUNU*/ wire WUNU_STORE4_RSTn = not1(WOHU_STORE4_RSTp);
    /*p29.EJAD*/ wire EJAD_STORE5_RSTn = not1(FEVE_STORE5_RSTp);
    /*p29.XAHO*/ wire XAHO_STORE6_RSTn = not1(WACY_STORE6_RSTp);
    /*p29.GAFY*/ wire GAFY_STORE7_RSTn = not1(GUKY_STORE7_RSTp);
    /*p29.WUZO*/ wire WUZO_STORE8_RSTn = not1(GORO_STORE8_RSTp);
    /*p29.DOSY*/ wire DOSY_STORE9_RSTn = not1(DUBU_STORE9_RSTp);

    /*p29.EDEN*/ wire EDEN_SPRITE_COUNT0n = not1(sprite_store.BESE_SPRITE_COUNT0.qp17());
    /*p29.CYPY*/ wire CYPY_SPRITE_COUNT1n = not1(sprite_store.CUXY_SPRITE_COUNT1.qp17());
    /*p29.CAPE*/ wire CAPE_SPRITE_COUNT2n = not1(sprite_store.BEGO_SPRITE_COUNT2.qp17());
    /*p29.CAXU*/ wire CAXU_SPRITE_COUNT3n = not1(sprite_store.DYBE_SPRITE_COUNT3.qp17());

    /*p29.FYCU*/ wire FYCU_SPRITE_COUNT0p = not1(EDEN_SPRITE_COUNT0n);
    /*p29.FONE*/ wire FONE_SPRITE_COUNT1p = not1(CYPY_SPRITE_COUNT1n);
    /*p29.EKUD*/ wire EKUD_SPRITE_COUNT2p = not1(CAPE_SPRITE_COUNT2n);
    /*p29.ELYG*/ wire ELYG_SPRITE_COUNT3p = not1(CAXU_SPRITE_COUNT3n);

    /*p29.DEZO*/ wire DEZO_STORE0_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.CUVA*/ wire CUVA_STORE1_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.GEBU*/ wire GEBU_STORE2_SELn = nand4(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.FOCO*/ wire FOCO_STORE3_SELn = nand4(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.CUPE*/ wire CUPE_STORE4_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.CUGU*/ wire CUGU_STORE5_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.WOMU*/ wire WOMU_STORE6_SELn = nand4(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.GUNA*/ wire GUNA_STORE7_SELn = nand4(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.DEWY*/ wire DEWY_STORE8_SELn = nand4(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);
    /*p29.DOGU*/ wire DOGU_STORE9_SELn = nand4(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);

    // Sprite stores latch their input when their SELn signal goes _high_
    /*p29.CEMY*/ wire CEMY_STORE0_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, DEZO_STORE0_SELn);
    /*p29.BYBY*/ wire BYBY_STORE1_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, CUVA_STORE1_SELn);
    /*p29.WYXO*/ wire WYXO_STORE2_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, GEBU_STORE2_SELn);
    /*p29.GUVE*/ wire GUVE_STORE3_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, FOCO_STORE3_SELn);
    /*p29.CECU*/ wire CECU_STORE4_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, CUPE_STORE4_SELn);
    /*p29.CADO*/ wire CADO_STORE5_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, CUGU_STORE5_SELn);
    /*p29.XUJO*/ wire XUJO_STORE6_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, WOMU_STORE6_SELn);
    /*p29.GAPE*/ wire GAPE_STORE7_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, GUNA_STORE7_SELn);
    /*p29.CAHO*/ wire CAHO_STORE8_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, DEWY_STORE8_SELn);
    /*p29.CATO*/ wire CATO_STORE9_CLKp = or2(_DYTY_STORE_ENn_xxCDxxGH, DOGU_STORE9_SELn);

    /*p29.DYHU*/ wire DYHU_STORE0_CLKn = not1(CEMY_STORE0_CLKp);
    /*p29.BUCO*/ wire BUCO_STORE1_CLKn = not1(BYBY_STORE1_CLKp);
    /*p29.GYFO*/ wire GYFO_STORE2_CLKn = not1(WYXO_STORE2_CLKp);
    /*p29.GUSA*/ wire GUSA_STORE3_CLKn = not1(GUVE_STORE3_CLKp);
    /*p29.DUKE*/ wire DUKE_STORE4_CLKn = not1(CECU_STORE4_CLKp);
    /*p29.BEDE*/ wire BEDE_STORE5_CLKn = not1(CADO_STORE5_CLKp);
    /*p29.WEKA*/ wire WEKA_STORE6_CLKn = not1(XUJO_STORE6_CLKp);
    /*p29.GYVO*/ wire GYVO_STORE7_CLKn = not1(GAPE_STORE7_CLKp);
    /*p29.BUKA*/ wire BUKA_STORE8_CLKn = not1(CAHO_STORE8_CLKp);
    /*p29.DECU*/ wire DECU_STORE9_CLKn = not1(CATO_STORE9_CLKp);

    // 10 sprite stores
    // Resetting the store X coords to 0 doesn't make sense, as they'd fire during a line even if we never stored any sprites.
    // I guess it must store inverted X, so that when reset X = 0xFF?

    {
      /*p29.GENY*/ wire GENY_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
      /*p29.GENY*/ wire GENY_STORE0_CLKn = not1(GENY_STORE0_CLKp);

      /*p30.YGUS*/ sprite_store.YGUS_STORE0_I0n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, sprite_store.SPR_TRI_I0p.qp());
      /*p30.YSOK*/ sprite_store.YSOK_STORE0_I1n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, sprite_store.SPR_TRI_I1p.qp());
      /*p30.YZEP*/ sprite_store.YZEP_STORE0_I2n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, sprite_store.SPR_TRI_I2p.qp());
      /*p30.WYTE*/ sprite_store.WYTE_STORE0_I3n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, sprite_store.SPR_TRI_I3p.qp());
      /*p30.ZONY*/ sprite_store.ZONY_STORE0_I4n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, sprite_store.SPR_TRI_I4p.qp());
      /*p30.YWAK*/ sprite_store.YWAK_STORE0_I5n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, sprite_store.SPR_TRI_I5p.qp());

      /*p29.ENOB*/ wire ENOB_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
      /*p29.ENOB*/ wire ENOB_STORE0_CLKn = not1(ENOB_STORE0_CLKp);

      /*p30.GYHO*/ sprite_store.GYHO_STORE0_L0n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, sprite_store.SPR_TRI_L0.qp());
      /*p30.CUFO*/ sprite_store.CUFO_STORE0_L1n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, sprite_store.SPR_TRI_L1.qp());
      /*p30.BOZU*/ sprite_store.BOZU_STORE0_L2n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, sprite_store.SPR_TRI_L2.qp());
      /*p30.FYHY*/ sprite_store.FYHY_STORE0_L3n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, sprite_store.SPR_TRI_L3.qp());

      /*p29.FUXU*/ wire FUXU_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
      /*p29.FUXU*/ wire FUXU_STORE0_CLKn = not1(FUXU_STORE0_CLKp);

      /*#p31.XEPE*/ sprite_store.XEPE_STORE0_X0p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _ZAGO_X0n);
      /* p31.YLAH*/ sprite_store.YLAH_STORE0_X1p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _ZOCY_X1n);
      /* p31.ZOLA*/ sprite_store.ZOLA_STORE0_X2p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _YPUR_X2n);
      /* p31.ZULU*/ sprite_store.ZULU_STORE0_X3p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _YVOK_X3n);
      /* p31.WELO*/ sprite_store.WELO_STORE0_X4p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _COSE_X4n);
      /* p31.XUNY*/ sprite_store.XUNY_STORE0_X5p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _AROP_X5n);
      /* p31.WOTE*/ sprite_store.WOTE_STORE0_X6p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _XATU_X6n);
      /* p31.XAKO*/ sprite_store.XAKO_STORE0_X7p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _BADY_X7n);
    }

    {
      /*p29.BYVY*/ wire BYVY_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /*p29.BYVY*/ wire BYVY_STORE1_CLKn = not1(BYVY_STORE1_CLKp);

      /*p30.CADU*/ sprite_store.CADU_STORE1_I0n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, sprite_store.SPR_TRI_I0p.qp());
      /*p30.CEBO*/ sprite_store.CEBO_STORE1_I1n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, sprite_store.SPR_TRI_I1p.qp());
      /*p30.CUFA*/ sprite_store.CUFA_STORE1_I2n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, sprite_store.SPR_TRI_I2p.qp());
      /*p30.COMA*/ sprite_store.COMA_STORE1_I3n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, sprite_store.SPR_TRI_I3p.qp());
      /*p30.CUZA*/ sprite_store.CUZA_STORE1_I4n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, sprite_store.SPR_TRI_I4p.qp());
      /*p30.CAJY*/ sprite_store.CAJY_STORE1_I5n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, sprite_store.SPR_TRI_I5p.qp());

      /*p29.AHOF*/ wire AHOF_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /*p29.AHOF*/ wire AHOF_STORE1_CLKn = not1(AHOF_STORE1_CLKp);

      /*p30.AMES*/ sprite_store.AMES_STORE1_L0n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, sprite_store.SPR_TRI_L0.qp());
      /*p30.AROF*/ sprite_store.AROF_STORE1_L1n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, sprite_store.SPR_TRI_L1.qp());
      /*p30.ABOP*/ sprite_store.ABOP_STORE1_L2n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, sprite_store.SPR_TRI_L2.qp());
      /*p30.ABUG*/ sprite_store.ABUG_STORE1_L3n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, sprite_store.SPR_TRI_L3.qp());

      /*p29.ASYS*/ wire ASYS_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /*p29.ASYS*/ wire ASYS_STORE1_CLKn = not1(ASYS_STORE1_CLKp);

      /*p31.DANY*/ sprite_store.DANY_STORE1_X0p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _ZAGO_X0n);
      /*p31.DUKO*/ sprite_store.DUKO_STORE1_X1p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _ZOCY_X1n);
      /*p31.DESU*/ sprite_store.DESU_STORE1_X2p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _YPUR_X2n);
      /*p31.DAZO*/ sprite_store.DAZO_STORE1_X3p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _YVOK_X3n);
      /*p31.DAKE*/ sprite_store.DAKE_STORE1_X4p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _COSE_X4n);
      /*p31.CESO*/ sprite_store.CESO_STORE1_X5p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _AROP_X5n);
      /*p31.DYFU*/ sprite_store.DYFU_STORE1_X6p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _XATU_X6n);
      /*p31.CUSY*/ sprite_store.CUSY_STORE1_X7p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _BADY_X7n);
    }

    /*p29.BUZY*/ wire BUZY_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.FUKE*/ wire FUKE_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.CACU*/ wire CACU_STORE2_CLKp = not1(GYFO_STORE2_CLKn);

    /*p30.BUHE*/ sprite_store.BUHE_STORE2_I0n.dff8n(BUZY_STORE2_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.BYHU*/ sprite_store.BYHU_STORE2_I1n.dff8n(BUZY_STORE2_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.BECA*/ sprite_store.BECA_STORE2_I2n.dff8n(BUZY_STORE2_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.BULU*/ sprite_store.BULU_STORE2_I3n.dff8n(BUZY_STORE2_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.BUNA*/ sprite_store.BUNA_STORE2_I4n.dff8n(BUZY_STORE2_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.BOXA*/ sprite_store.BOXA_STORE2_I5n.dff8n(BUZY_STORE2_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.YLOV*/ sprite_store.YLOV_STORE2_L0n.dff8n(FUKE_STORE2_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.XOSY*/ sprite_store.XOSY_STORE2_L1n.dff8n(FUKE_STORE2_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.XAZY*/ sprite_store.XAZY_STORE2_L2n.dff8n(FUKE_STORE2_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.YKUK*/ sprite_store.YKUK_STORE2_L3n.dff8n(FUKE_STORE2_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.FOKA*/ sprite_store.FOKA_STORE2_X0p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _ZAGO_X0n);
    /*p31.FYTY*/ sprite_store.FYTY_STORE2_X1p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _ZOCY_X1n);
    /*p31.FUBY*/ sprite_store.FUBY_STORE2_X2p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _YPUR_X2n);
    /*p31.GOXU*/ sprite_store.GOXU_STORE2_X3p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _YVOK_X3n);
    /*p31.DUHY*/ sprite_store.DUHY_STORE2_X4p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _COSE_X4n);
    /*p31.EJUF*/ sprite_store.EJUF_STORE2_X5p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _AROP_X5n);
    /*p31.ENOR*/ sprite_store.ENOR_STORE2_X6p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _XATU_X6n);
    /*p31.DEPY*/ sprite_store.DEPY_STORE2_X7p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _BADY_X7n);

    /*p29.FEKA*/ wire FEKA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
    /*p29.XYHA*/ wire XYHA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
    /*p29.YFAG*/ wire YFAG_STORE3_CLKp = not1(GUSA_STORE3_CLKn);

    /*p30.DEVY*/ sprite_store.DEVY_STORE3_I0n.dff8n(FEKA_STORE3_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.DESE*/ sprite_store.DESE_STORE3_I1n.dff8n(FEKA_STORE3_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.DUNY*/ sprite_store.DUNY_STORE3_I2n.dff8n(FEKA_STORE3_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.DUHA*/ sprite_store.DUHA_STORE3_I3n.dff8n(FEKA_STORE3_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.DEBA*/ sprite_store.DEBA_STORE3_I4n.dff8n(FEKA_STORE3_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.DAFU*/ sprite_store.DAFU_STORE3_I5n.dff8n(FEKA_STORE3_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.ZURO*/ sprite_store.ZURO_STORE3_L0n.dff8n(XYHA_STORE3_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.ZYLU*/ sprite_store.ZYLU_STORE3_L1n.dff8n(XYHA_STORE3_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.ZENE*/ sprite_store.ZENE_STORE3_L2n.dff8n(XYHA_STORE3_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.ZURY*/ sprite_store.ZURY_STORE3_L3n.dff8n(XYHA_STORE3_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.XOLY*/ sprite_store.XOLY_STORE3_X0p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _ZAGO_X0n);
    /*p31.XYBA*/ sprite_store.XYBA_STORE3_X1p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _ZOCY_X1n);
    /*p31.XABE*/ sprite_store.XABE_STORE3_X2p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _YPUR_X2n);
    /*p31.XEKA*/ sprite_store.XEKA_STORE3_X3p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _YVOK_X3n);
    /*p31.XOMY*/ sprite_store.XOMY_STORE3_X4p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _COSE_X4n);
    /*p31.WUHA*/ sprite_store.WUHA_STORE3_X5p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _AROP_X5n);
    /*p31.WYNA*/ sprite_store.WYNA_STORE3_X6p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _XATU_X6n);
    /*p31.WECO*/ sprite_store.WECO_STORE3_X7p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _BADY_X7n);

    /*p29.WOFO*/ wire WOFO_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
    /*p29.WYLU*/ wire WYLU_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
    /*p29.EWOT*/ wire EWOT_STORE4_CLKp = not1(DUKE_STORE4_CLKn);

    /*p30.XAVE*/ sprite_store.XAVE_STORE4_I0n.dff8n(WYLU_STORE4_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.XEFE*/ sprite_store.XEFE_STORE4_I1n.dff8n(WYLU_STORE4_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.WANU*/ sprite_store.WANU_STORE4_I2n.dff8n(WYLU_STORE4_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.XABO*/ sprite_store.XABO_STORE4_I3n.dff8n(WYLU_STORE4_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.XEGE*/ sprite_store.XEGE_STORE4_I4n.dff8n(WYLU_STORE4_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.XYNU*/ sprite_store.XYNU_STORE4_I5n.dff8n(WYLU_STORE4_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.CAPO*/ sprite_store.CAPO_STORE4_L0n.dff8n(EWOT_STORE4_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.CAJU*/ sprite_store.CAJU_STORE4_L1n.dff8n(EWOT_STORE4_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.CONO*/ sprite_store.CONO_STORE4_L2n.dff8n(EWOT_STORE4_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.CUMU*/ sprite_store.CUMU_STORE4_L3n.dff8n(EWOT_STORE4_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.WEDU*/ sprite_store.WEDU_STORE4_X0p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _ZAGO_X0n);
    /*p31.YGAJ*/ sprite_store.YGAJ_STORE4_X1p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _ZOCY_X1n);
    /*p31.ZYJO*/ sprite_store.ZYJO_STORE4_X2p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _YPUR_X2n);
    /*p31.XURY*/ sprite_store.XURY_STORE4_X3p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _YVOK_X3n);
    /*p31.YBED*/ sprite_store.YBED_STORE4_X4p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _COSE_X4n);
    /*p31.ZALA*/ sprite_store.ZALA_STORE4_X5p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _AROP_X5n);
    /*p31.WYDE*/ sprite_store.WYDE_STORE4_X6p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _XATU_X6n);
    /*p31.XEPA*/ sprite_store.XEPA_STORE4_X7p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _BADY_X7n);

    /*p29.CYLA*/ wire CYLA_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
    /*p29.DYMO*/ wire DYMO_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
    /*p29.BUCY*/ wire BUCY_STORE5_CLKp = not1(BEDE_STORE5_CLKn);

    /*p30.EKOP*/ sprite_store.EKOP_STORE5_I0n.dff8n(DYMO_STORE5_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.ETYM*/ sprite_store.ETYM_STORE5_I1n.dff8n(DYMO_STORE5_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.GORU*/ sprite_store.GORU_STORE5_I2n.dff8n(DYMO_STORE5_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.EBEX*/ sprite_store.EBEX_STORE5_I3n.dff8n(DYMO_STORE5_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.ETAV*/ sprite_store.ETAV_STORE5_I4n.dff8n(DYMO_STORE5_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.EKAP*/ sprite_store.EKAP_STORE5_I5n.dff8n(DYMO_STORE5_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.ACEP*/ sprite_store.ACEP_STORE5_L0n.dff8n(BUCY_STORE5_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.ABEG*/ sprite_store.ABEG_STORE5_L1n.dff8n(BUCY_STORE5_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.ABUX*/ sprite_store.ABUX_STORE5_L2n.dff8n(BUCY_STORE5_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.ANED*/ sprite_store.ANED_STORE5_L3n.dff8n(BUCY_STORE5_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.FUSA*/ sprite_store.FUSA_STORE5_X0p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _ZAGO_X0n);
    /*p31.FAXA*/ sprite_store.FAXA_STORE5_X1p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _ZOCY_X1n);
    /*p31.FOZY*/ sprite_store.FOZY_STORE5_X2p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _YPUR_X2n);
    /*p31.FESY*/ sprite_store.FESY_STORE5_X3p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _YVOK_X3n);
    /*p31.CYWE*/ sprite_store.CYWE_STORE5_X4p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _COSE_X4n);
    /*p31.DYBY*/ sprite_store.DYBY_STORE5_X5p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _AROP_X5n);
    /*p31.DURY*/ sprite_store.DURY_STORE5_X6p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _XATU_X6n);
    /*p31.CUVY*/ sprite_store.CUVY_STORE5_X7p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _BADY_X7n);

    /*p29.ZAPE*/ wire ZAPE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
    /*p29.WUSE*/ wire WUSE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
    /*p29.ZURU*/ wire ZURU_STORE6_CLKp = not1(WEKA_STORE6_CLKn);

    /*p30.GABO*/ sprite_store.GABO_STORE6_I0n.dff8n(WUSE_STORE6_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.GACY*/ sprite_store.GACY_STORE6_I1n.dff8n(WUSE_STORE6_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.FOGO*/ sprite_store.FOGO_STORE6_I2n.dff8n(WUSE_STORE6_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.GOHU*/ sprite_store.GOHU_STORE6_I3n.dff8n(WUSE_STORE6_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.FOXY*/ sprite_store.FOXY_STORE6_I4n.dff8n(WUSE_STORE6_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.GECU*/ sprite_store.GECU_STORE6_I5n.dff8n(WUSE_STORE6_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.ZUMY*/ sprite_store.ZUMY_STORE6_L0n.dff8n(ZURU_STORE6_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.ZAFU*/ sprite_store.ZAFU_STORE6_L1n.dff8n(ZURU_STORE6_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.ZEXO*/ sprite_store.ZEXO_STORE6_L2n.dff8n(ZURU_STORE6_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.ZUBE*/ sprite_store.ZUBE_STORE6_L3n.dff8n(ZURU_STORE6_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.YCOL*/ sprite_store.YCOL_STORE6_X0p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _ZAGO_X0n);
    /*p31.YRAC*/ sprite_store.YRAC_STORE6_X1p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _ZOCY_X1n);
    /*p31.YMEM*/ sprite_store.YMEM_STORE6_X2p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _YPUR_X2n);
    /*p31.YVAG*/ sprite_store.YVAG_STORE6_X3p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _YVOK_X3n);
    /*p31.ZOLY*/ sprite_store.ZOLY_STORE6_X4p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _COSE_X4n);
    /*p31.ZOGO*/ sprite_store.ZOGO_STORE6_X5p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _AROP_X5n);
    /*p31.ZECU*/ sprite_store.ZECU_STORE6_X6p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _XATU_X6n);
    /*p31.ZESA*/ sprite_store.ZESA_STORE6_X7p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _BADY_X7n);

    /*p29.GECY*/ wire GECY_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
    /*p29.FEFO*/ wire FEFO_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
    /*p29.WABE*/ wire WABE_STORE7_CLKp = not1(GYVO_STORE7_CLKn);

    /*p30.GULE*/ sprite_store.GULE_STORE7_I0n.dff8n(FEFO_STORE7_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.GYNO*/ sprite_store.GYNO_STORE7_I1n.dff8n(FEFO_STORE7_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.FEFA*/ sprite_store.FEFA_STORE7_I2n.dff8n(FEFO_STORE7_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.FYSU*/ sprite_store.FYSU_STORE7_I3n.dff8n(FEFO_STORE7_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.GESY*/ sprite_store.GESY_STORE7_I4n.dff8n(FEFO_STORE7_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.FUZO*/ sprite_store.FUZO_STORE7_I5n.dff8n(FEFO_STORE7_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.XYNA*/ sprite_store.XYNA_STORE7_L0n.dff8n(WABE_STORE7_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.YGUM*/ sprite_store.YGUM_STORE7_L1n.dff8n(WABE_STORE7_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.XAKU*/ sprite_store.XAKU_STORE7_L2n.dff8n(WABE_STORE7_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.XYGO*/ sprite_store.XYGO_STORE7_L3n.dff8n(WABE_STORE7_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.ERAZ*/ sprite_store.ERAZ_STORE7_X0p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _ZAGO_X0n);
    /*p31.EPUM*/ sprite_store.EPUM_STORE7_X1p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _ZOCY_X1n);
    /*p31.EROL*/ sprite_store.EROL_STORE7_X2p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _YPUR_X2n);
    /*p31.EHYN*/ sprite_store.EHYN_STORE7_X3p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _YVOK_X3n);
    /*p31.FAZU*/ sprite_store.FAZU_STORE7_X4p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _COSE_X4n);
    /*p31.FAXE*/ sprite_store.FAXE_STORE7_X5p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _AROP_X5n);
    /*p31.EXUK*/ sprite_store.EXUK_STORE7_X6p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _XATU_X6n);
    /*p31.FEDE*/ sprite_store.FEDE_STORE7_X7p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _BADY_X7n);

    /*p29.CEXU*/ wire CEXU_STORE8_CLKp = not1(BUKA_STORE8_CLKn);
    /*p29.AKOL*/ wire AKOL_STORE8_CLKp = not1(BUKA_STORE8_CLKn);
    /*p29.BYMY*/ wire BYMY_STORE8_CLKp = not1(BUKA_STORE8_CLKn);

    /*p30.AXUV*/ sprite_store.AXUV_STORE8_I0n.dff8n(AKOL_STORE8_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.BADA*/ sprite_store.BADA_STORE8_I1n.dff8n(AKOL_STORE8_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.APEV*/ sprite_store.APEV_STORE8_I2n.dff8n(AKOL_STORE8_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.BADO*/ sprite_store.BADO_STORE8_I3n.dff8n(AKOL_STORE8_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.BEXY*/ sprite_store.BEXY_STORE8_I4n.dff8n(AKOL_STORE8_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.BYHE*/ sprite_store.BYHE_STORE8_I5n.dff8n(AKOL_STORE8_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.AZAP*/ sprite_store.AZAP_STORE8_L0n.dff8n(BYMY_STORE8_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.AFYX*/ sprite_store.AFYX_STORE8_L1n.dff8n(BYMY_STORE8_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.AFUT*/ sprite_store.AFUT_STORE8_L2n.dff8n(BYMY_STORE8_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.AFYM*/ sprite_store.AFYM_STORE8_L3n.dff8n(BYMY_STORE8_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.EZUF*/ sprite_store.EZUF_STORE8_X0p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _COSE_X4n);
    /*p31.ENAD*/ sprite_store.ENAD_STORE8_X1p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _AROP_X5n);
    /*p31.EBOW*/ sprite_store.EBOW_STORE8_X2p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _XATU_X6n);
    /*p31.FYCA*/ sprite_store.FYCA_STORE8_X3p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _BADY_X7n);
    /*p31.GAVY*/ sprite_store.GAVY_STORE8_X4p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _ZAGO_X0n);
    /*p31.GYPU*/ sprite_store.GYPU_STORE8_X5p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _ZOCY_X1n);
    /*p31.GADY*/ sprite_store.GADY_STORE8_X6p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _YPUR_X2n);
    /*p31.GAZA*/ sprite_store.GAZA_STORE8_X7p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _YVOK_X3n);

    /*p29.WEME*/ wire WEME_STORE9_CLKp = not1(DECU_STORE9_CLKn);
    /*p29.WUFA*/ wire WUFA_STORE9_CLKp = not1(DECU_STORE9_CLKn);
    /*p29.FAKA*/ wire FAKA_STORE9_CLKp = not1(DECU_STORE9_CLKn);

    /*p30.YBER*/ sprite_store.YBER_STORE9_I0n.dff8n(WUFA_STORE9_CLKp, sprite_store.SPR_TRI_I0p.qp());
    /*p30.YZOR*/ sprite_store.YZOR_STORE9_I1n.dff8n(WUFA_STORE9_CLKp, sprite_store.SPR_TRI_I1p.qp());
    /*p30.XYFE*/ sprite_store.XYFE_STORE9_I2n.dff8n(WUFA_STORE9_CLKp, sprite_store.SPR_TRI_I2p.qp());
    /*p30.XOTU*/ sprite_store.XOTU_STORE9_I3n.dff8n(WUFA_STORE9_CLKp, sprite_store.SPR_TRI_I3p.qp());
    /*p30.XUTE*/ sprite_store.XUTE_STORE9_I4n.dff8n(WUFA_STORE9_CLKp, sprite_store.SPR_TRI_I4p.qp());
    /*p30.XUFO*/ sprite_store.XUFO_STORE9_I5n.dff8n(WUFA_STORE9_CLKp, sprite_store.SPR_TRI_I5p.qp());

    /*p30.CANA*/ sprite_store.CANA_STORE9_L0n.dff8n(FAKA_STORE9_CLKp, sprite_store.SPR_TRI_L0.qp());
    /*p30.FOFO*/ sprite_store.FOFO_STORE9_L1n.dff8n(FAKA_STORE9_CLKp, sprite_store.SPR_TRI_L1.qp());
    /*p30.DYSY*/ sprite_store.DYSY_STORE9_L2n.dff8n(FAKA_STORE9_CLKp, sprite_store.SPR_TRI_L2.qp());
    /*p30.DEWU*/ sprite_store.DEWU_STORE9_L3n.dff8n(FAKA_STORE9_CLKp, sprite_store.SPR_TRI_L3.qp());

    /*p31.XUVY*/ sprite_store.XUVY_STORE9_X0p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _ZAGO_X0n);
    /*p31.XERE*/ sprite_store.XERE_STORE9_X1p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _ZOCY_X1n);
    /*p31.XUZO*/ sprite_store.XUZO_STORE9_X2p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _YPUR_X2n);
    /*p31.XEXA*/ sprite_store.XEXA_STORE9_X3p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _YVOK_X3n);
    /*p31.YPOD*/ sprite_store.YPOD_STORE9_X4p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _COSE_X4n);
    /*p31.YROP*/ sprite_store.YROP_STORE9_X5p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _AROP_X5n);
    /*p31.YNEP*/ sprite_store.YNEP_STORE9_X6p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _XATU_X6n);
    /*p31.YZOF*/ sprite_store.YZOF_STORE9_X7p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _BADY_X7n);
  }

  //------------------------------------------------------------------------------

  /*#p01.WALU*/ wire _WALU_SYS_RSTn = not1(clk_reg.XORE_SYS_RSTp);
  /* p01.TOFU*/ wire _TOFU_VID_RSTp = not1(clk_reg.XAPO_VID_RSTn);
  /* p01.PYRY*/ wire _PYRY_VID_RSTp = not1(clk_reg.XAPO_VID_RSTn);

  /*#p27.MEHE*/ wire _MEHE_AxCxExGx = not1(clk_reg.ALET_xBxDxFxH);

  /*#p24.SOCY*/ wire _SOCY_WIN_HITn = not1(pix_pipe.TOMU_WIN_HITp);

  /*#p24.VYBO*/ wire _VYBO_CLKPIPEp = nor3(sprite_store.FEPO_STORE_MATCHp, pix_pipe.WODU_HBLANKp, clk_reg.MYVO_AxCxExGx);
  /*#p24.TYFA*/ wire _TYFA_CLKPIPEp = and3(_SOCY_WIN_HITn, tile_fetcher.POKY_PRELOAD_LATCHp.qp04(), _VYBO_CLKPIPEp);
  /*#p24.SEGU*/ wire _SEGU_CLKPIPEn = not1(_TYFA_CLKPIPEp);
  /*#p24.ROXO*/ wire _ROXO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
  /*#p24.SACU*/ wire _SACU_CLKPIPEp = or2(_SEGU_CLKPIPEn, pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp04()); // Schematic wrong, this is OR

  /* p27.REPU*/ wire _REPU_VBLANK_RSTp = or2(lcd_reg.PARU_VBLANKp_d4, _PYRY_VID_RSTp);

  /*#p21.WEGO*/ wire _WEGO_HBLANKp = or2(_TOFU_VID_RSTp, pix_pipe.VOGA_HBLANKp.qp17());

  /* p21.TADY*/ wire _TADY_LINE_START_RSTn = nor2(lcd_reg.ATEJ_LINE_TRIGp, _TOFU_VID_RSTp);

  //----------------------------------------
  // XYMU is the main "we're rendering" flag

  /*#p21.VOGA*/ pix_pipe.VOGA_HBLANKp.dff17(clk_reg.ALET_xBxDxFxH, _TADY_LINE_START_RSTn, pix_pipe.WODU_HBLANKp);

  /*#p21.XYMU*/ pix_pipe.XYMU_RENDERINGn.nor_latch(_WEGO_HBLANKp, sprite_scanner.AVAP_RENDER_START_TRIGp);

  //----------------------------------------
  // Pixel counter, has carry lookahead because this can increment every tcycle

  {
    /*p21.RYBO*/ wire _RYBO = xor2(pix_pipe.XEHO_X0p.qp17(), pix_pipe.SAVY_X1p.qp17()); // XOR layout 1, feet facing gnd, this should def be regular xor
    /*p21.XUKE*/ wire _XUKE = and2(pix_pipe.XEHO_X0p.qp17(), pix_pipe.SAVY_X1p.qp17());

    /*p21.XYLE*/ wire _XYLE = and2(pix_pipe.XODU_X2p.qp17(), _XUKE);
    /*p21.XEGY*/ wire _XEGY = xor2(pix_pipe.XODU_X2p.qp17(), _XUKE); // feet facing gnd
    /*p21.XORA*/ wire _XORA = xor2(pix_pipe.XYDO_X3p.qp17(), _XYLE); // feet facing gnd

    /*p21.XEHO*/ pix_pipe.XEHO_X0p.dff17(_SACU_CLKPIPEp, _TADY_LINE_START_RSTn, pix_pipe.XEHO_X0p.qn16());
    /*p21.SAVY*/ pix_pipe.SAVY_X1p.dff17(_SACU_CLKPIPEp, _TADY_LINE_START_RSTn, _RYBO);
    /*p21.XODU*/ pix_pipe.XODU_X2p.dff17(_SACU_CLKPIPEp, _TADY_LINE_START_RSTn, _XEGY);
    /*p21.XYDO*/ pix_pipe.XYDO_X3p.dff17(_SACU_CLKPIPEp, _TADY_LINE_START_RSTn, _XORA);

    /*p21.SAKE*/ wire _SAKE = xor2(pix_pipe.TUHU_X4p.qp17(), pix_pipe.TUKY_X5p.qp17());
    /*p21.TYBA*/ wire _TYBA = and2(pix_pipe.TUHU_X4p.qp17(), pix_pipe.TUKY_X5p.qp17());
    /*p21.SURY*/ wire _SURY = and2(pix_pipe.TAKO_X6p.qp17(), _TYBA);
    /*p21.TYGE*/ wire _TYGE = xor2(pix_pipe.TAKO_X6p.qp17(), _TYBA);
    /*p21.ROKU*/ wire _ROKU = xor2(pix_pipe.SYBE_X7p.qp17(), _SURY);

    /*p24.TOCA*/ wire _TOCA_CLKPIPE_HI = not1(pix_pipe.XYDO_X3p.qp17());
    /*p21.TUHU*/ pix_pipe.TUHU_X4p.dff17(_TOCA_CLKPIPE_HI, _TADY_LINE_START_RSTn, pix_pipe.TUHU_X4p.qn16());
    /*p21.TUKY*/ pix_pipe.TUKY_X5p.dff17(_TOCA_CLKPIPE_HI, _TADY_LINE_START_RSTn, _SAKE);
    /*p21.TAKO*/ pix_pipe.TAKO_X6p.dff17(_TOCA_CLKPIPE_HI, _TADY_LINE_START_RSTn, _TYGE);
    /*p21.SYBE*/ pix_pipe.SYBE_X7p.dff17(_TOCA_CLKPIPE_HI, _TADY_LINE_START_RSTn, _ROKU);
  }

  //----------------------------------------
  // LCD pins that are controlled by the pixel counter

  /*#p27.POVA*/ wire _POVA_FINE_MATCHpe = and2(pix_pipe.NYZE_SCX_FINE_MATCH_B.qn16(), pix_pipe.PUXA_SCX_FINE_MATCH_A.qp17());

  {
    /*#p21.XAJO*/ wire _XAJO_X_009p = and2(pix_pipe.XEHO_X0p.qp17(), pix_pipe.XYDO_X3p.qp17());
    /*#p21.WUSA*/ pix_pipe.WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009p, _WEGO_HBLANKp);
    /*#p21.TOBA*/ wire _TOBA_LCD_CLOCK = and2(pix_pipe.WUSA_LCD_CLOCK_GATE.qp04(), _SACU_CLKPIPEp);
    /*#p21.SEMU*/ wire _SEMU_LCD_CLOCK = or2(_TOBA_LCD_CLOCK, _POVA_FINE_MATCHpe);
    /*#p21.RYPO*/ wire _RYPO_LCD_CLOCK = not1(_SEMU_LCD_CLOCK);

    PIN_LCD_CLOCK.io_pin(_RYPO_LCD_CLOCK, _RYPO_LCD_CLOCK);
  }

  {
    // LCD horizontal sync pin latch

    // FIXME inversion
    // I don't know why ROXO has to be inverted here but it extends HSYNC by one phase, which
    // seems to be correct and makes it match the trace. With that change, HSYNC is 30 phases.
    // Is it possible that it should be 29 phases and it only looks like 30 phases because of gate delay?
    // That would be a loooot of gate delay.
    // Could we possibly be incrementing X3p one phase early?

    /* p24.PAHO*/ pix_pipe.PAHO_X_8_SYNC.dff17(_ROXO_CLKPIPEp, pix_pipe.XYMU_RENDERINGn.qn03(), pix_pipe.XYDO_X3p.qp17());

    /*#p24.POME*/ pix_pipe.POME = nor2(sprite_scanner.AVAP_RENDER_START_TRIGp, pix_pipe.POFY);
    /*#p24.RUJU*/ pix_pipe.RUJU = or3(pix_pipe.PAHO_X_8_SYNC.qp17(), _TOFU_VID_RSTp, pix_pipe.POME);
    /*#p24.POFY*/ pix_pipe.POFY = not1(pix_pipe.RUJU);

    /*#p24.RUZE*/ wire _RUZE_HSYNCn = not1(pix_pipe.POFY);
    PIN_LCD_HSYNC.io_pin(_RUZE_HSYNCn, _RUZE_HSYNCn);
  }

  //----------------------------------------
  // Fine counter.

  /*#p27.ROZE*/ wire _ROZE_FINE_COUNT_7n = nand3(pix_pipe.RUBU_FINE_CNT2.qp17(), pix_pipe.ROGA_FINE_CNT1.qp17(), pix_pipe.RYKU_FINE_CNT0.qp17());
  /*#p27.PAHA*/ wire _PAHA_RENDERINGn = not1(pix_pipe.XYMU_RENDERINGn.qn03());

  {
    /*#p27.PECU*/ wire _PECU_FINE_CLK = nand2(_ROXO_CLKPIPEp, _ROZE_FINE_COUNT_7n);
    /*#p27.PASO*/ wire _PASO_FINE_RST = nor2(_PAHA_RENDERINGn, pix_pipe.TEVO_FETCH_TRIGp);

    /*#p27.RYKU*/ pix_pipe.RYKU_FINE_CNT0.dff17(_PECU_FINE_CLK,                     _PASO_FINE_RST, pix_pipe.RYKU_FINE_CNT0.qn16());
    /*#p27.ROGA*/ pix_pipe.ROGA_FINE_CNT1.dff17(pix_pipe.RYKU_FINE_CNT0.qn16(), _PASO_FINE_RST, pix_pipe.ROGA_FINE_CNT1.qn16());
    /*#p27.RUBU*/ pix_pipe.RUBU_FINE_CNT2.dff17(pix_pipe.ROGA_FINE_CNT1.qn16(), _PASO_FINE_RST, pix_pipe.RUBU_FINE_CNT2.qn16());
  }

  //----------------------------------------

  {
    /* p27.SUHA*/ wire _SUHA_SCX_FINE_MATCHp = xnor2(pix_pipe.DATY_SCX0n.qn08(), pix_pipe.RYKU_FINE_CNT0.qp17());
    /* p27.SYBY*/ wire _SYBY_SCX_FINE_MATCHp = xnor2(pix_pipe.DUZU_SCX1n.qn08(), pix_pipe.ROGA_FINE_CNT1.qp17());
    /* p27.SOZU*/ wire _SOZU_SCX_FINE_MATCHp = xnor2(pix_pipe.CYXU_SCX2n.qn08(), pix_pipe.RUBU_FINE_CNT2.qp17());

    /*#p27.RONE*/ wire _RONE_SCX_FINE_MATCHn = nand4(pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.qp04(), _SUHA_SCX_FINE_MATCHp, _SYBY_SCX_FINE_MATCHp, _SOZU_SCX_FINE_MATCHp);
    /*#p27.POHU*/ wire _POHU_SCX_FINE_MATCHp = not1(_RONE_SCX_FINE_MATCHn);

    /* p27.MOXE*/ wire _MOXE_AxCxExGx = not1(clk_reg.ALET_xBxDxFxH);
    /*#p27.PUXA*/ pix_pipe.PUXA_SCX_FINE_MATCH_A.dff17(_ROXO_CLKPIPEp, pix_pipe.XYMU_RENDERINGn.qn03(), _POHU_SCX_FINE_MATCHp);
    /*#p27.NYZE*/ pix_pipe.NYZE_SCX_FINE_MATCH_B.dff17(_MOXE_AxCxExGx, pix_pipe.XYMU_RENDERINGn.qn03(), pix_pipe.PUXA_SCX_FINE_MATCH_A.qp17());

    /*#p27.ROXY*/ pix_pipe.ROXY_SCX_FINE_MATCH_LATCHn.nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCHpe);
  }

  //----------------------------------------
  // Window matcher

  /*#p27.NAZE*/ wire _NAZE_WY_MATCH0 = xnor2(pix_pipe.NESO_WY0n.qn08(), lcd_reg.MUWY_Y0p.qp17());
  /* p27.PEBO*/ wire _PEBO_WY_MATCH1 = xnor2(pix_pipe.NYRO_WY1n.qn08(), lcd_reg.MYRO_Y1p.qp17());
  /* p27.POMO*/ wire _POMO_WY_MATCH2 = xnor2(pix_pipe.NAGA_WY2n.qn08(), lcd_reg.LEXA_Y2p.qp17());
  /* p27.NEVU*/ wire _NEVU_WY_MATCH3 = xnor2(pix_pipe.MELA_WY3n.qn08(), lcd_reg.LYDO_Y3p.qp17());
  /* p27.NOJO*/ wire _NOJO_WY_MATCH4 = xnor2(pix_pipe.NULO_WY4n.qn08(), lcd_reg.LOVU_Y4p.qp17());
  /* p27.PAGA*/ wire _PAGA_WY_MATCH5 = xnor2(pix_pipe.NENE_WY5n.qn08(), lcd_reg.LEMA_Y5p.qp17());
  /* p27.PEZO*/ wire _PEZO_WY_MATCH6 = xnor2(pix_pipe.NUKA_WY6n.qn08(), lcd_reg.MATO_Y6p.qp17());
  /* p27.NUPA*/ wire _NUPA_WY_MATCH7 = xnor2(pix_pipe.NAFU_WY7n.qn08(), lcd_reg.LAFO_Y7p.qp17());

  /*#p27.PALO*/ wire _PALO_WY_MATCH_HIn  = nand5(pix_pipe.WYMO_LCDC_WINENn.qn08(), _NOJO_WY_MATCH4, _PAGA_WY_MATCH5, _PEZO_WY_MATCH6, _NUPA_WY_MATCH7);
  /* p27.NELE*/ wire _NELE_WY_MATCH_HI   = not1(_PALO_WY_MATCH_HIn);
  /* p27.PAFU*/ wire _PAFU_WY_MATCHn     = nand5(_NELE_WY_MATCH_HI, _NAZE_WY_MATCH0, _PEBO_WY_MATCH1, _POMO_WY_MATCH2, _NEVU_WY_MATCH3);
  /* p27.ROGE*/ wire _ROGE_WY_MATCHp = not1(_PAFU_WY_MATCHn);
  /* p27.SARY*/ pix_pipe.SARY_WY_MATCH.dff17(clk_reg.TALU_xxCDEFxx, clk_reg.XAPO_VID_RSTn, _ROGE_WY_MATCHp);
  /* p27.REJO*/ pix_pipe.REJO_WY_MATCH_LATCH.nor_latch(pix_pipe.SARY_WY_MATCH.qp17(), _REPU_VBLANK_RSTp);

  /*#p27.MYLO*/ wire _MYLO_WX_MATCH0 = xnor2(pix_pipe.XEHO_X0p.qp17(), pix_pipe.MYPA_WX0n.qn08());
  /* p27.PUWU*/ wire _PUWU_WX_MATCH1 = xnor2(pix_pipe.SAVY_X1p.qp17(), pix_pipe.NOFE_WX1n.qn08());
  /* p27.PUHO*/ wire _PUHO_WX_MATCH2 = xnor2(pix_pipe.XODU_X2p.qp17(), pix_pipe.NOKE_WX2n.qn08());
  /* p27.NYTU*/ wire _NYTU_WX_MATCH3 = xnor2(pix_pipe.XYDO_X3p.qp17(), pix_pipe.MEBY_WX3n.qn08());
  /* p27.NEZO*/ wire _NEZO_WX_MATCH4 = xnor2(pix_pipe.TUHU_X4p.qp17(), pix_pipe.MYPU_WX4n.qn08());
  /* p27.NORY*/ wire _NORY_WX_MATCH5 = xnor2(pix_pipe.TUKY_X5p.qp17(), pix_pipe.MYCE_WX5n.qn08());
  /* p27.NONO*/ wire _NONO_WX_MATCH6 = xnor2(pix_pipe.TAKO_X6p.qp17(), pix_pipe.MUVO_WX6n.qn08());
  /* p27.PASE*/ wire _PASE_WX_MATCH7 = xnor2(pix_pipe.SYBE_X7p.qp17(), pix_pipe.NUKU_WX7n.qn08());

  /*p27.PUKY*/ wire _PUKY_WX_MATCH_HIn  = nand5(pix_pipe.REJO_WY_MATCH_LATCH.qp04(), _NEZO_WX_MATCH4, _NORY_WX_MATCH5, _NONO_WX_MATCH6, _PASE_WX_MATCH7);
  /*p27.NUFA*/ wire _NUFA_WX_MATCH_HI   = not1(_PUKY_WX_MATCH_HIn);
  /*p27.NOGY*/ wire _NOGY_WX_MATCHn     = nand5(_NUFA_WX_MATCH_HI, _MYLO_WX_MATCH0, _PUWU_WX_MATCH1, _PUHO_WX_MATCH2, _NYTU_WX_MATCH3);
  /*p27.NUKO*/ wire _NUKO_WX_MATCHp = not1(_NOGY_WX_MATCHn);

  // This trigger fires on the pixel _at_ WX
  /*p27.ROCO*/ wire _ROCO_CLKPIPEp = not1(_SEGU_CLKPIPEn);
  /*p27.PYCO*/ pix_pipe.PYCO_WX_MATCH_A.dff17(_ROCO_CLKPIPEp, clk_reg.XAPO_VID_RSTn, _NUKO_WX_MATCHp);
  /*p27.NUNU*/ pix_pipe.NUNU_WX_MATCH_B.dff17(_MEHE_AxCxExGx, clk_reg.XAPO_VID_RSTn, pix_pipe.PYCO_WX_MATCH_A.qp17());

  /*p27.PANY*/ wire _PANY_FETCHn = nor2(_NUKO_WX_MATCHp, _ROZE_FINE_COUNT_7n);

  /*p27.RYFA*/ pix_pipe.RYFA_FETCHn_A.dff17(_SEGU_CLKPIPEn, pix_pipe.XYMU_RENDERINGn.qn03(), _PANY_FETCHn);
  /*p27.RENE*/ pix_pipe.RENE_FETCHn_B.dff17(clk_reg.ALET_xBxDxFxH, pix_pipe.XYMU_RENDERINGn.qn03(), pix_pipe.RYFA_FETCHn_A.qp17());

  //----------------------------------------
  // Window sequencer

  /* p27.XAHY*/ wire _XAHY_VID_LINE_TRIG_d4n = not1(lcd_reg.ATEJ_LINE_TRIGp);
  /*#p27.XOFO*/ wire _XOFO_WIN_RSTp = nand3(pix_pipe.WYMO_LCDC_WINENn.qn08(), _XAHY_VID_LINE_TRIG_d4n, clk_reg.XAPO_VID_RSTn);

  {
    /* p27.PYNU*/ pix_pipe.PYNU_WIN_MODE_A.nor_latch(pix_pipe.NUNU_WX_MATCH_B.qp17(), _XOFO_WIN_RSTp);
    /* p27.NOPA*/ pix_pipe.NOPA_WIN_MODE_B.dff17(clk_reg.ALET_xBxDxFxH, clk_reg.XAPO_VID_RSTn, pix_pipe.PYNU_WIN_MODE_A.qp04());

    /* p27.PUKU*/ pix_pipe.PUKU = nor2(pix_pipe.NUNY_WX_MATCH_TRIGp, pix_pipe.RYDY);
    /* p27.RYDY*/ pix_pipe.RYDY = nor3(pix_pipe.PUKU, tile_fetcher.PORY_FETCH_DONE_P12.qp17(), _PYRY_VID_RSTp);

    /* p27.SOVY*/ pix_pipe.SOVY_WIN_FIRST_TILE_B.dff17(clk_reg.ALET_xBxDxFxH, clk_reg.XAPO_VID_RSTn, pix_pipe.RYDY);
  }

  //----------------------------------------
  // Window x coordinate

  {
    /*p27.VETU*/ wire _VETU_WIN_MAP_CLK = and2(pix_pipe.TEVO_FETCH_TRIGp, pix_pipe.PORE_WIN_MODEp);
    /*p27.XACO*/ wire _XACO_WIN_RSTn = not1(_XOFO_WIN_RSTp);

    /*p27.WYKA*/ pix_pipe.WYKA_WIN_X3.dff17(_VETU_WIN_MAP_CLK,                _XACO_WIN_RSTn, pix_pipe.WYKA_WIN_X3.qn16());
    /*p27.WODY*/ pix_pipe.WODY_WIN_X4.dff17(pix_pipe.WYKA_WIN_X3.qn16(),  _XACO_WIN_RSTn, pix_pipe.WODY_WIN_X4.qn16());
    /*p27.WOBO*/ pix_pipe.WOBO_WIN_X5.dff17(pix_pipe.WODY_WIN_X4.qn16(),  _XACO_WIN_RSTn, pix_pipe.WOBO_WIN_X5.qn16());
    /*p27.WYKO*/ pix_pipe.WYKO_WIN_X6.dff17(pix_pipe.WOBO_WIN_X5.qn16(),  _XACO_WIN_RSTn, pix_pipe.WYKO_WIN_X6.qn16());
    /*p27.XOLO*/ pix_pipe.XOLO_WIN_X7.dff17(pix_pipe.WYKO_WIN_X6.qn16(),  _XACO_WIN_RSTn, pix_pipe.XOLO_WIN_X7.qn16());
  }

  //----------------------------------------
  // Window y coordinate
  {
    /*p27.WAZY*/ wire _WAZY_WIN_Y_CLKp = not1(pix_pipe.PORE_WIN_MODEp);
    /*p27.SYNY*/ wire _SYNY_WIN_Y_RSTn = not1(_REPU_VBLANK_RSTp);

    // Every time we leave win mode we increment win_y
    /*p27.VYNO*/ pix_pipe.VYNO_WIN_Y0.dff17(_WAZY_WIN_Y_CLKp,                _SYNY_WIN_Y_RSTn, pix_pipe.VYNO_WIN_Y0.qn16());
    /*p27.VUJO*/ pix_pipe.VUJO_WIN_Y1.dff17(pix_pipe.VYNO_WIN_Y0.qn16(), _SYNY_WIN_Y_RSTn, pix_pipe.VUJO_WIN_Y1.qn16());
    /*p27.VYMU*/ pix_pipe.VYMU_WIN_Y2.dff17(pix_pipe.VUJO_WIN_Y1.qn16(), _SYNY_WIN_Y_RSTn, pix_pipe.VYMU_WIN_Y2.qn16());
    /*p27.TUFU*/ pix_pipe.TUFU_WIN_Y3.dff17(pix_pipe.VYMU_WIN_Y2.qn16(), _SYNY_WIN_Y_RSTn, pix_pipe.TUFU_WIN_Y3.qn16());
    /*p27.TAXA*/ pix_pipe.TAXA_WIN_Y4.dff17(pix_pipe.TUFU_WIN_Y3.qn16(), _SYNY_WIN_Y_RSTn, pix_pipe.TAXA_WIN_Y4.qn16());
    /*p27.TOZO*/ pix_pipe.TOZO_WIN_Y5.dff17(pix_pipe.TAXA_WIN_Y4.qn16(), _SYNY_WIN_Y_RSTn, pix_pipe.TOZO_WIN_Y5.qn16());
    /*p27.TATE*/ pix_pipe.TATE_WIN_Y6.dff17(pix_pipe.TOZO_WIN_Y5.qn16(), _SYNY_WIN_Y_RSTn, pix_pipe.TATE_WIN_Y6.qn16());
    /*p27.TEKE*/ pix_pipe.TEKE_WIN_Y7.dff17(pix_pipe.TATE_WIN_Y6.qn16(), _SYNY_WIN_Y_RSTn, pix_pipe.TEKE_WIN_Y7.qn16());
  }

  //----------------------------------------
  // Background pipes

  // Background pipes are loaded at phase 16 of tile fetch

  {
    /*p32.LUHE*/ wire BG_PIX_A0n = not1(vram_bus.LEGU_TILE_DA0n.qn07());
    /*p32.NOLY*/ wire BG_PIX_A1n = not1(vram_bus.NUDU_TILE_DA1n.qn07());
    /*p32.LEKE*/ wire BG_PIX_A2n = not1(vram_bus.MUKU_TILE_DA2n.qn07());
    /*p32.LOMY*/ wire BG_PIX_A3n = not1(vram_bus.LUZO_TILE_DA3n.qn07());
    /*p32.LALA*/ wire BG_PIX_A4n = not1(vram_bus.MEGU_TILE_DA4n.qn07());
    /*p32.LOXA*/ wire BG_PIX_A5n = not1(vram_bus.MYJY_TILE_DA5n.qn07());
    /*p32.NEZE*/ wire BG_PIX_A6n = not1(vram_bus.NASA_TILE_DA6n.qn07());
    /*p32.NOBO*/ wire BG_PIX_A7n = not1(vram_bus.NEFO_TILE_DA7n.qn07());

    /*p32.LOZE*/ wire _LOZE_PIPE_A_LOADp = not1(pix_pipe.NYXU_FETCH_TRIGn);
    /*p32.LAKY*/ wire BG_PIPE_A_SET0 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.LEGU_TILE_DA0n.qn07());
    /*p32.NYXO*/ wire BG_PIPE_A_SET1 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NUDU_TILE_DA1n.qn07());
    /*p32.LOTO*/ wire BG_PIPE_A_SET2 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MUKU_TILE_DA2n.qn07());
    /*p32.LYDU*/ wire BG_PIPE_A_SET3 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.LUZO_TILE_DA3n.qn07());
    /*p32.MYVY*/ wire BG_PIPE_A_SET4 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MEGU_TILE_DA4n.qn07());
    /*p32.LODO*/ wire BG_PIPE_A_SET5 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.MYJY_TILE_DA5n.qn07());
    /*p32.NUTE*/ wire BG_PIPE_A_SET6 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NASA_TILE_DA6n.qn07());
    /*p32.NAJA*/ wire BG_PIPE_A_SET7 = nand2(_LOZE_PIPE_A_LOADp, vram_bus.NEFO_TILE_DA7n.qn07());

    /*p32.LOTY*/ wire BG_PIPE_A_RST0 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A0n);
    /*p32.NEXA*/ wire BG_PIPE_A_RST1 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A1n);
    /*p32.LUTU*/ wire BG_PIPE_A_RST2 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A2n);
    /*p32.LUJA*/ wire BG_PIPE_A_RST3 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A3n);
    /*p32.MOSY*/ wire BG_PIPE_A_RST4 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A4n);
    /*p32.LERU*/ wire BG_PIPE_A_RST5 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A5n);
    /*p32.NYHA*/ wire BG_PIPE_A_RST6 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A6n);
    /*p32.NADY*/ wire BG_PIPE_A_RST7 = nand2(_LOZE_PIPE_A_LOADp, BG_PIX_A7n);

    /*p32.MYDE*/ pix_pipe.MYDE_BG_PIPE_A0.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET0, BG_PIPE_A_RST0, GND);
    /*p32.NOZO*/ pix_pipe.NOZO_BG_PIPE_A1.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET1, BG_PIPE_A_RST1, pix_pipe.MYDE_BG_PIPE_A0.qp16());
    /*p32.MOJU*/ pix_pipe.MOJU_BG_PIPE_A2.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET2, BG_PIPE_A_RST2, pix_pipe.NOZO_BG_PIPE_A1.qp16());
    /*p32.MACU*/ pix_pipe.MACU_BG_PIPE_A3.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET3, BG_PIPE_A_RST3, pix_pipe.MOJU_BG_PIPE_A2.qp16());
    /*p32.NEPO*/ pix_pipe.NEPO_BG_PIPE_A4.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET4, BG_PIPE_A_RST4, pix_pipe.MACU_BG_PIPE_A3.qp16());
    /*p32.MODU*/ pix_pipe.MODU_BG_PIPE_A5.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET5, BG_PIPE_A_RST5, pix_pipe.NEPO_BG_PIPE_A4.qp16());
    /*p32.NEDA*/ pix_pipe.NEDA_BG_PIPE_A6.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET6, BG_PIPE_A_RST6, pix_pipe.MODU_BG_PIPE_A5.qp16());
    /*p32.PYBO*/ pix_pipe.PYBO_BG_PIPE_A7.dff22(_SACU_CLKPIPEp, BG_PIPE_A_SET7, BG_PIPE_A_RST7, pix_pipe.NEDA_BG_PIPE_A6.qp16());
  }

  {
    /*p32.TOSA*/ wire BG_PIX_B0n = not1(vram_bus.RAWU_TILE_DB0p.q11p());
    /*p32.RUCO*/ wire BG_PIX_B1n = not1(vram_bus.POZO_TILE_DB1p.q11p());
    /*p32.TYCE*/ wire BG_PIX_B2n = not1(vram_bus.PYZO_TILE_DB2p.q11p());
    /*p32.REVY*/ wire BG_PIX_B3n = not1(vram_bus.POXA_TILE_DB3p.q11p());
    /*p32.RYGA*/ wire BG_PIX_B4n = not1(vram_bus.PULO_TILE_DB4p.q11p());
    /*p32.RYLE*/ wire BG_PIX_B5n = not1(vram_bus.POJU_TILE_DB5p.q11p());
    /*p32.RAPU*/ wire BG_PIX_B6n = not1(vram_bus.POWY_TILE_DB6p.q11p());
    /*p32.SOJA*/ wire BG_PIX_B7n = not1(vram_bus.PYJU_TILE_DB7p.q11p());

    /*p32.LUXA*/ wire LUXA_PIPE_B_LOADp = not1(pix_pipe.NYXU_FETCH_TRIGn);
    /*p32.TUXE*/ wire BG_PIPE_B_SET0 = nand2(LUXA_PIPE_B_LOADp, vram_bus.RAWU_TILE_DB0p.q11p());
    /*p32.SOLY*/ wire BG_PIPE_B_SET1 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POZO_TILE_DB1p.q11p());
    /*p32.RUCE*/ wire BG_PIPE_B_SET2 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PYZO_TILE_DB2p.q11p());
    /*p32.RYJA*/ wire BG_PIPE_B_SET3 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POXA_TILE_DB3p.q11p());
    /*p32.RUTO*/ wire BG_PIPE_B_SET4 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PULO_TILE_DB4p.q11p());
    /*p32.RAJA*/ wire BG_PIPE_B_SET5 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POJU_TILE_DB5p.q11p());
    /*p32.RAJO*/ wire BG_PIPE_B_SET6 = nand2(LUXA_PIPE_B_LOADp, vram_bus.POWY_TILE_DB6p.q11p());
    /*p32.RAGA*/ wire BG_PIPE_B_SET7 = nand2(LUXA_PIPE_B_LOADp, vram_bus.PYJU_TILE_DB7p.q11p());

    /*p32.SEJA*/ wire BG_PIPE_B_RST0 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B0n);
    /*p32.SENO*/ wire BG_PIPE_B_RST1 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B1n);
    /*p32.SURE*/ wire BG_PIPE_B_RST2 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B2n);
    /*p32.SEBO*/ wire BG_PIPE_B_RST3 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B3n);
    /*p32.SUCA*/ wire BG_PIPE_B_RST4 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B4n);
    /*p32.SYWE*/ wire BG_PIPE_B_RST5 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B5n);
    /*p32.SUPU*/ wire BG_PIPE_B_RST6 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B6n);
    /*p32.RYJY*/ wire BG_PIPE_B_RST7 = nand2(LUXA_PIPE_B_LOADp, BG_PIX_B7n);

    /*p32.TOMY*/ pix_pipe.TOMY_BG_PIPE_B0.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET0, BG_PIPE_B_RST0, GND);
    /*p32.TACA*/ pix_pipe.TACA_BG_PIPE_B1.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET1, BG_PIPE_B_RST1, pix_pipe.TOMY_BG_PIPE_B0.qp16());
    /*p32.SADY*/ pix_pipe.SADY_BG_PIPE_B2.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET2, BG_PIPE_B_RST2, pix_pipe.TACA_BG_PIPE_B1.qp16());
    /*p32.RYSA*/ pix_pipe.RYSA_BG_PIPE_B3.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET3, BG_PIPE_B_RST3, pix_pipe.SADY_BG_PIPE_B2.qp16());
    /*p32.SOBO*/ pix_pipe.SOBO_BG_PIPE_B4.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET4, BG_PIPE_B_RST4, pix_pipe.RYSA_BG_PIPE_B3.qp16());
    /*p32.SETU*/ pix_pipe.SETU_BG_PIPE_B5.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET5, BG_PIPE_B_RST5, pix_pipe.SOBO_BG_PIPE_B4.qp16());
    /*p32.RALU*/ pix_pipe.RALU_BG_PIPE_B6.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET6, BG_PIPE_B_RST6, pix_pipe.SETU_BG_PIPE_B5.qp16());
    /*p32.SOHU*/ pix_pipe.SOHU_BG_PIPE_B7.dff22(_SACU_CLKPIPEp, BG_PIPE_B_SET7, BG_PIPE_B_RST7, pix_pipe.RALU_BG_PIPE_B6.qp16());
  }

  //----------------------------------------
  // Sprite pipes

  {
    /*p29.XEFY*/ wire XEPY_SPRITE_DONEn  = not1(sprite_fetcher.WUTY_SPRITE_DONEp);

    /*p34.MEFU*/ wire MEFU_SPRITE_MASK0n = or3(XEPY_SPRITE_DONEn, pix_pipe.NYLU_SPR_PIPE_B0.qp16(), pix_pipe.NURO_SPR_PIPE_A0.qp16()); // def or
    /*p34.MEVE*/ wire MEVE_SPRITE_MASK1n = or3(XEPY_SPRITE_DONEn, pix_pipe.PEFU_SPR_PIPE_B1.qp16(), pix_pipe.MASO_SPR_PIPE_A1.qp16());
    /*p34.MYZO*/ wire MYZO_SPRITE_MASK2n = or3(XEPY_SPRITE_DONEn, pix_pipe.NATY_SPR_PIPE_B2.qp16(), pix_pipe.LEFE_SPR_PIPE_A2.qp16());
    /*p34.RUDA*/ wire RUDA_SPRITE_MASK3n = or3(XEPY_SPRITE_DONEn, pix_pipe.PYJO_SPR_PIPE_B3.qp16(), pix_pipe.LESU_SPR_PIPE_A3.qp16());
    /*p34.VOTO*/ wire VOTO_SPRITE_MASK4n = or3(XEPY_SPRITE_DONEn, pix_pipe.VARE_SPR_PIPE_B4.qp16(), pix_pipe.WYHO_SPR_PIPE_A4.qp16());
    /*p34.VYSA*/ wire VYSA_SPRITE_MASK5n = or3(XEPY_SPRITE_DONEn, pix_pipe.WEBA_SPR_PIPE_B5.qp16(), pix_pipe.WORA_SPR_PIPE_A5.qp16());
    /*p34.TORY*/ wire TORY_SPRITE_MASK6n = or3(XEPY_SPRITE_DONEn, pix_pipe.VANU_SPR_PIPE_B6.qp16(), pix_pipe.VAFO_SPR_PIPE_A6.qp16());
    /*p34.WOPE*/ wire WOPE_SPRITE_MASK7n = or3(XEPY_SPRITE_DONEn, pix_pipe.VUPY_SPR_PIPE_B7.qp16(), pix_pipe.WUFY_SPR_PIPE_A7.qp16());

    // The mask is 1 where there are _no_ sprite pixels
    /*p34.LESY*/ wire LESY_SPRITE_MASK0p = not1(MEFU_SPRITE_MASK0n);
    /*p34.LOTA*/ wire LOTA_SPRITE_MASK1p = not1(MEVE_SPRITE_MASK1n);
    /*p34.LYKU*/ wire LYKU_SPRITE_MASK2p = not1(MYZO_SPRITE_MASK2n);
    /*p34.ROBY*/ wire ROBY_SPRITE_MASK3p = not1(RUDA_SPRITE_MASK3n);
    /*p34.TYTA*/ wire TYTA_SPRITE_MASK4p = not1(VOTO_SPRITE_MASK4n);
    /*p34.TYCO*/ wire TYCO_SPRITE_MASK5p = not1(VYSA_SPRITE_MASK5n);
    /*p34.SOKA*/ wire SOKA_SPRITE_MASK6p = not1(TORY_SPRITE_MASK6n);
    /*p34.XOVU*/ wire XOVU_SPRITE_MASK7p = not1(WOPE_SPRITE_MASK7n);

    // Sprite pipe A
    {
      /*p33.RATA*/ wire RATA_SPR_PIX_DA0n = not1(vram_bus.REWO_SPRITE_DA0n.qn07());
      /*p33.NUCA*/ wire NUCA_SPR_PIX_DA1n = not1(vram_bus.PEBA_SPRITE_DA1n.qn07());
      /*p33.LASE*/ wire LASE_SPR_PIX_DA2n = not1(vram_bus.MOFO_SPRITE_DA2n.qn07());
      /*p33.LUBO*/ wire LUBO_SPR_PIX_DA3n = not1(vram_bus.PUDU_SPRITE_DA3n.qn07());
      /*p33.WERY*/ wire WERY_SPR_PIX_DA4n = not1(vram_bus.SAJA_SPRITE_DA4n.qn07());
      /*p33.WURA*/ wire WURA_SPR_PIX_DA5n = not1(vram_bus.SUNY_SPRITE_DA5n.qn07());
      /*p33.SULU*/ wire SULU_SPR_PIX_DA6n = not1(vram_bus.SEMO_SPRITE_DA6n.qn07());
      /*p33.WAMY*/ wire WAMY_SPR_PIX_DA7n = not1(vram_bus.SEGA_SPRITE_DA7n.qn07());

      /*p33.PABE*/ wire PABE_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, vram_bus.REWO_SPRITE_DA0n.qn07());
      /*p33.MYTO*/ wire MYTO_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, vram_bus.PEBA_SPRITE_DA1n.qn07());
      /*p33.LELA*/ wire LELA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, vram_bus.MOFO_SPRITE_DA2n.qn07());
      /*p33.MAME*/ wire MAME_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, vram_bus.PUDU_SPRITE_DA3n.qn07());
      /*p33.VEXU*/ wire VEXU_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, vram_bus.SAJA_SPRITE_DA4n.qn07());
      /*p33.VABY*/ wire VABY_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, vram_bus.SUNY_SPRITE_DA5n.qn07());
      /*p33.TUXA*/ wire TUXA_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, vram_bus.SEMO_SPRITE_DA6n.qn07());
      /*p33.VUNE*/ wire VUNE_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, vram_bus.SEGA_SPRITE_DA7n.qn07());

      /*p33.PYZU*/ wire PYZU_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, RATA_SPR_PIX_DA0n);
      /*p33.MADA*/ wire MADA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, NUCA_SPR_PIX_DA1n);
      /*p33.LYDE*/ wire LYDE_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LASE_SPR_PIX_DA2n);
      /*p33.LUFY*/ wire LUFY_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, LUBO_SPR_PIX_DA3n);
      /*p33.XATO*/ wire XATO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, WERY_SPR_PIX_DA4n);
      /*p33.XEXU*/ wire XEXU_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WURA_SPR_PIX_DA5n);
      /*p33.TUPE*/ wire TUPE_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SULU_SPR_PIX_DA6n);
      /*p33.XYVE*/ wire XYVE_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, WAMY_SPR_PIX_DA7n);

      /*p33.NURO*/ pix_pipe.NURO_SPR_PIPE_A0.dff22(_SACU_CLKPIPEp, PABE_SPR_PIX_SET0, PYZU_SPR_PIX_RST0, GND);
      /*p33.MASO*/ pix_pipe.MASO_SPR_PIPE_A1.dff22(_SACU_CLKPIPEp, MYTO_SPR_PIX_SET1, MADA_SPR_PIX_RST1, pix_pipe.NURO_SPR_PIPE_A0.qp16());
      /*p33.LEFE*/ pix_pipe.LEFE_SPR_PIPE_A2.dff22(_SACU_CLKPIPEp, LELA_SPR_PIX_SET2, LYDE_SPR_PIX_RST2, pix_pipe.MASO_SPR_PIPE_A1.qp16());
      /*p33.LESU*/ pix_pipe.LESU_SPR_PIPE_A3.dff22(_SACU_CLKPIPEp, MAME_SPR_PIX_SET3, LUFY_SPR_PIX_RST3, pix_pipe.LEFE_SPR_PIPE_A2.qp16());
      /*p33.WYHO*/ pix_pipe.WYHO_SPR_PIPE_A4.dff22(_SACU_CLKPIPEp, VEXU_SPR_PIX_SET4, XATO_SPR_PIX_RST4, pix_pipe.LESU_SPR_PIPE_A3.qp16());
      /*p33.WORA*/ pix_pipe.WORA_SPR_PIPE_A5.dff22(_SACU_CLKPIPEp, VABY_SPR_PIX_SET5, XEXU_SPR_PIX_RST5, pix_pipe.WYHO_SPR_PIPE_A4.qp16());
      /*p33.VAFO*/ pix_pipe.VAFO_SPR_PIPE_A6.dff22(_SACU_CLKPIPEp, TUXA_SPR_PIX_SET6, TUPE_SPR_PIX_RST6, pix_pipe.WORA_SPR_PIPE_A5.qp16());
      /*p33.WUFY*/ pix_pipe.WUFY_SPR_PIPE_A7.dff22(_SACU_CLKPIPEp, VUNE_SPR_PIX_SET7, XYVE_SPR_PIX_RST7, pix_pipe.VAFO_SPR_PIPE_A6.qp16());
    }

    // Sprite pipe B
    {
      /*p33.LOZA*/ wire LOZA_SPR_PIX_DB0n = not1(vram_bus.PEFO_SPRITE_DB0n.qn07());
      /*p33.SYBO*/ wire SYBO_SPR_PIX_DB1n = not1(vram_bus.ROKA_SPRITE_DB1n.qn07());
      /*p33.LUMO*/ wire LUMO_SPR_PIX_DB2n = not1(vram_bus.MYTU_SPRITE_DB2n.qn07());
      /*p33.SOLO*/ wire SOLO_SPR_PIX_DB3n = not1(vram_bus.RAMU_SPRITE_DB3n.qn07());
      /*p33.VOBY*/ wire VOBY_SPR_PIX_DB4n = not1(vram_bus.SELE_SPRITE_DB4n.qn07());
      /*p33.WYCO*/ wire WYCO_SPR_PIX_DB5n = not1(vram_bus.SUTO_SPRITE_DB5n.qn07());
      /*p33.SERY*/ wire SERY_SPR_PIX_DB6n = not1(vram_bus.RAMA_SPRITE_DB6n.qn07());
      /*p33.SELU*/ wire SELU_SPR_PIX_DB7n = not1(vram_bus.RYDU_SPRITE_DB7n.qn07());

      /*p33.MEZU*/ wire MEZU_SPR_PIX_SET0 = nand2(LESY_SPRITE_MASK0p, vram_bus.PEFO_SPRITE_DB0n.qn07());
      /*p33.RUSY*/ wire RUSY_SPR_PIX_SET1 = nand2(LOTA_SPRITE_MASK1p, vram_bus.ROKA_SPRITE_DB1n.qn07());
      /*p33.MYXA*/ wire MYXA_SPR_PIX_SET2 = nand2(LYKU_SPRITE_MASK2p, vram_bus.MYTU_SPRITE_DB2n.qn07());
      /*p33.RANO*/ wire RANO_SPR_PIX_SET3 = nand2(ROBY_SPRITE_MASK3p, vram_bus.RAMU_SPRITE_DB3n.qn07());
      /*p33.TYGA*/ wire TYGA_SPR_PIX_SET4 = nand2(TYTA_SPRITE_MASK4p, vram_bus.SELE_SPRITE_DB4n.qn07());
      /*p33.VUME*/ wire VUME_SPR_PIX_SET5 = nand2(TYCO_SPRITE_MASK5p, vram_bus.SUTO_SPRITE_DB5n.qn07());
      /*p33.TAPO*/ wire TAPO_SPR_PIX_SET6 = nand2(SOKA_SPRITE_MASK6p, vram_bus.RAMA_SPRITE_DB6n.qn07());
      /*p33.TESO*/ wire TESO_SPR_PIX_SET7 = nand2(XOVU_SPRITE_MASK7p, vram_bus.RYDU_SPRITE_DB7n.qn07());

      /*p33.MOFY*/ wire MOFY_SPR_PIX_RST0 = nand2(LESY_SPRITE_MASK0p, LOZA_SPR_PIX_DB0n);
      /*p33.RUCA*/ wire RUCA_SPR_PIX_RST1 = nand2(LOTA_SPRITE_MASK1p, SYBO_SPR_PIX_DB1n);
      /*p33.MAJO*/ wire MAJO_SPR_PIX_RST2 = nand2(LYKU_SPRITE_MASK2p, LUMO_SPR_PIX_DB2n);
      /*p33.REHU*/ wire REHU_SPR_PIX_RST3 = nand2(ROBY_SPRITE_MASK3p, SOLO_SPR_PIX_DB3n);
      /*p33.WAXO*/ wire WAXO_SPR_PIX_RST4 = nand2(TYTA_SPRITE_MASK4p, VOBY_SPR_PIX_DB4n);
      /*p33.XOLE*/ wire XOLE_SPR_PIX_RST5 = nand2(TYCO_SPRITE_MASK5p, WYCO_SPR_PIX_DB5n);
      /*p33.TABY*/ wire TABY_SPR_PIX_RST6 = nand2(SOKA_SPRITE_MASK6p, SERY_SPR_PIX_DB6n);
      /*p33.TULA*/ wire TULA_SPR_PIX_RST7 = nand2(XOVU_SPRITE_MASK7p, SELU_SPR_PIX_DB7n);

      /*p33.NYLU*/ pix_pipe.NYLU_SPR_PIPE_B0.dff22(_SACU_CLKPIPEp, MEZU_SPR_PIX_SET0, MOFY_SPR_PIX_RST0, GND);
      /*p33.PEFU*/ pix_pipe.PEFU_SPR_PIPE_B1.dff22(_SACU_CLKPIPEp, RUSY_SPR_PIX_SET1, RUCA_SPR_PIX_RST1, pix_pipe.NYLU_SPR_PIPE_B0.qp16());
      /*p33.NATY*/ pix_pipe.NATY_SPR_PIPE_B2.dff22(_SACU_CLKPIPEp, MYXA_SPR_PIX_SET2, MAJO_SPR_PIX_RST2, pix_pipe.PEFU_SPR_PIPE_B1.qp16());
      /*p33.PYJO*/ pix_pipe.PYJO_SPR_PIPE_B3.dff22(_SACU_CLKPIPEp, RANO_SPR_PIX_SET3, REHU_SPR_PIX_RST3, pix_pipe.NATY_SPR_PIPE_B2.qp16());
      /*p33.VARE*/ pix_pipe.VARE_SPR_PIPE_B4.dff22(_SACU_CLKPIPEp, TYGA_SPR_PIX_SET4, WAXO_SPR_PIX_RST4, pix_pipe.PYJO_SPR_PIPE_B3.qp16());
      /*p33.WEBA*/ pix_pipe.WEBA_SPR_PIPE_B5.dff22(_SACU_CLKPIPEp, VUME_SPR_PIX_SET5, XOLE_SPR_PIX_RST5, pix_pipe.VARE_SPR_PIPE_B4.qp16());
      /*p33.VANU*/ pix_pipe.VANU_SPR_PIPE_B6.dff22(_SACU_CLKPIPEp, TAPO_SPR_PIX_SET6, TABY_SPR_PIX_RST6, pix_pipe.WEBA_SPR_PIPE_B5.qp16());
      /*p33.VUPY*/ pix_pipe.VUPY_SPR_PIPE_B7.dff22(_SACU_CLKPIPEp, TESO_SPR_PIX_SET7, TULA_SPR_PIX_RST7, pix_pipe.VANU_SPR_PIPE_B6.qp16());
    }

    // Palette pipe
    {
      /*p34.SYPY*/ wire SYPY = not1(oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.TOTU*/ wire TOTU = not1(oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.NARO*/ wire NARO = not1(oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.WEXY*/ wire WEXY = not1(oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.RYZY*/ wire RYZY = not1(oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.RYFE*/ wire RYFE = not1(oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.LADY*/ wire LADY = not1(oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.LAFY*/ wire LAFY = not1(oam_bus.GOMO_OAM_DB4p.qp08());

      /*p34.PUME*/ wire PUME_PAL_PIPE_SET0n = nand2(LESY_SPRITE_MASK0p, oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.SORO*/ wire SORO_PAL_PIPE_SET1n = nand2(LOTA_SPRITE_MASK1p, oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.PAMO*/ wire PAMO_PAL_PIPE_SET2n = nand2(LYKU_SPRITE_MASK2p, oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.SUKY*/ wire SUKY_PAL_PIPE_SET3n = nand2(ROBY_SPRITE_MASK3p, oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.RORA*/ wire RORA_PAL_PIPE_SET4n = nand2(TYTA_SPRITE_MASK4p, oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.MENE*/ wire MENE_PAL_PIPE_SET5n = nand2(TYCO_SPRITE_MASK5p, oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.LUKE*/ wire LUKE_PAL_PIPE_SET6n = nand2(SOKA_SPRITE_MASK6p, oam_bus.GOMO_OAM_DB4p.qp08());
      /*p34.LAMY*/ wire LAMY_PAL_PIPE_SET7n = nand2(XOVU_SPRITE_MASK7p, oam_bus.GOMO_OAM_DB4p.qp08());

      /*p34.SUCO*/ wire SUCO_PAL_PIPE_RST0n = nand2(LESY_SPRITE_MASK0p, SYPY);
      /*p34.TAFA*/ wire TAFA_PAL_PIPE_RST1n = nand2(LOTA_SPRITE_MASK1p, TOTU);
      /*p34.PYZY*/ wire PYZY_PAL_PIPE_RST2n = nand2(LYKU_SPRITE_MASK2p, NARO);
      /*p34.TOWA*/ wire TOWA_PAL_PIPE_RST3n = nand2(ROBY_SPRITE_MASK3p, WEXY);
      /*p34.RUDU*/ wire RUDU_PAL_PIPE_RST4n = nand2(TYTA_SPRITE_MASK4p, RYZY);
      /*p34.PAZO*/ wire PAZO_PAL_PIPE_RST5n = nand2(TYCO_SPRITE_MASK5p, RYFE);
      /*p34.LOWA*/ wire LOWA_PAL_PIPE_RST6n = nand2(SOKA_SPRITE_MASK6p, LADY);
      /*p34.LUNU*/ wire LUNU_PAL_PIPE_RST7n = nand2(XOVU_SPRITE_MASK7p, LAFY);

      /*p34.RUGO*/ pix_pipe.RUGO_PAL_PIPE_0.dff22(_SACU_CLKPIPEp, PUME_PAL_PIPE_SET0n, SUCO_PAL_PIPE_RST0n, GND);
      /*p34.SATA*/ pix_pipe.SATA_PAL_PIPE_1.dff22(_SACU_CLKPIPEp, SORO_PAL_PIPE_SET1n, TAFA_PAL_PIPE_RST1n, pix_pipe.RUGO_PAL_PIPE_0.qp16());
      /*p34.ROSA*/ pix_pipe.ROSA_PAL_PIPE_2.dff22(_SACU_CLKPIPEp, PAMO_PAL_PIPE_SET2n, PYZY_PAL_PIPE_RST2n, pix_pipe.SATA_PAL_PIPE_1.qp16());
      /*p34.SOMY*/ pix_pipe.SOMY_PAL_PIPE_3.dff22(_SACU_CLKPIPEp, SUKY_PAL_PIPE_SET3n, TOWA_PAL_PIPE_RST3n, pix_pipe.ROSA_PAL_PIPE_2.qp16());
      /*p34.PALU*/ pix_pipe.PALU_PAL_PIPE_4.dff22(_SACU_CLKPIPEp, RORA_PAL_PIPE_SET4n, RUDU_PAL_PIPE_RST4n, pix_pipe.SOMY_PAL_PIPE_3.qp16());
      /*p34.NUKE*/ pix_pipe.NUKE_PAL_PIPE_5.dff22(_SACU_CLKPIPEp, MENE_PAL_PIPE_SET5n, PAZO_PAL_PIPE_RST5n, pix_pipe.PALU_PAL_PIPE_4.qp16());
      /*p34.MODA*/ pix_pipe.MODA_PAL_PIPE_6.dff22(_SACU_CLKPIPEp, LUKE_PAL_PIPE_SET6n, LOWA_PAL_PIPE_RST6n, pix_pipe.NUKE_PAL_PIPE_5.qp16());
      /*p34.LYME*/ pix_pipe.LYME_PAL_PIPE_7.dff22(_SACU_CLKPIPEp, LAMY_PAL_PIPE_SET7n, LUNU_PAL_PIPE_RST7n, pix_pipe.MODA_PAL_PIPE_6.qp16());
    }

    // Background mask pipe
    {
      /*p26.XOGA*/ wire XOGA = not1(oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XURA*/ wire XURA = not1(oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.TAJO*/ wire TAJO = not1(oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XENU*/ wire XENU = not1(oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XYKE*/ wire XYKE = not1(oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XABA*/ wire XABA = not1(oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.TAFU*/ wire TAFU = not1(oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XUHO*/ wire XUHO = not1(oam_bus.DEPO_OAM_DB7p.qp08());

      /*p26.TEDE*/ wire TEDE_MASK_PIPE_SET0 = nand2(LESY_SPRITE_MASK0p, oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XALA*/ wire XALA_MASK_PIPE_SET1 = nand2(LOTA_SPRITE_MASK1p, oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.TYRA*/ wire TYRA_MASK_PIPE_SET2 = nand2(LYKU_SPRITE_MASK2p, oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XYRU*/ wire XYRU_MASK_PIPE_SET3 = nand2(ROBY_SPRITE_MASK3p, oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XUKU*/ wire XUKU_MASK_PIPE_SET4 = nand2(TYTA_SPRITE_MASK4p, oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.XELY*/ wire XELY_MASK_PIPE_SET5 = nand2(TYCO_SPRITE_MASK5p, oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.TYKO*/ wire TYKO_MASK_PIPE_SET6 = nand2(SOKA_SPRITE_MASK6p, oam_bus.DEPO_OAM_DB7p.qp08());
      /*p26.TUWU*/ wire TUWU_MASK_PIPE_SET7 = nand2(XOVU_SPRITE_MASK7p, oam_bus.DEPO_OAM_DB7p.qp08());

      /*p26.WOKA*/ wire WOKA_MASK_PIPE_RST0 = nand2(LESY_SPRITE_MASK0p, XOGA);
      /*p26.WEDE*/ wire WEDE_MASK_PIPE_RST1 = nand2(LOTA_SPRITE_MASK1p, XURA);
      /*p26.TUFO*/ wire TUFO_MASK_PIPE_RST2 = nand2(LYKU_SPRITE_MASK2p, TAJO);
      /*p26.WEVO*/ wire WEVO_MASK_PIPE_RST3 = nand2(ROBY_SPRITE_MASK3p, XENU);
      /*p26.WEDY*/ wire WEDY_MASK_PIPE_RST4 = nand2(TYTA_SPRITE_MASK4p, XYKE);
      /*p26.WUJA*/ wire WUJA_MASK_PIPE_RST5 = nand2(TYCO_SPRITE_MASK5p, XABA);
      /*p26.TENA*/ wire TENA_MASK_PIPE_RST6 = nand2(SOKA_SPRITE_MASK6p, TAFU);
      /*p26.WUBU*/ wire WUBU_MASK_PIPE_RST7 = nand2(XOVU_SPRITE_MASK7p, XUHO);

      /*p26.VEZO*/ pix_pipe.VEZO_MASK_PIPE_0.dff22(_SACU_CLKPIPEp, TEDE_MASK_PIPE_SET0, WOKA_MASK_PIPE_RST0, VYPO);
      /*p26.WURU*/ pix_pipe.WURU_MASK_PIPE_1.dff22(_SACU_CLKPIPEp, XALA_MASK_PIPE_SET1, WEDE_MASK_PIPE_RST1, pix_pipe.VEZO_MASK_PIPE_0.qp16());
      /*p26.VOSA*/ pix_pipe.VOSA_MASK_PIPE_2.dff22(_SACU_CLKPIPEp, TYRA_MASK_PIPE_SET2, TUFO_MASK_PIPE_RST2, pix_pipe.WURU_MASK_PIPE_1.qp16());
      /*p26.WYFU*/ pix_pipe.WYFU_MASK_PIPE_3.dff22(_SACU_CLKPIPEp, XYRU_MASK_PIPE_SET3, WEVO_MASK_PIPE_RST3, pix_pipe.VOSA_MASK_PIPE_2.qp16());
      /*p26.XETE*/ pix_pipe.XETE_MASK_PIPE_4.dff22(_SACU_CLKPIPEp, XUKU_MASK_PIPE_SET4, WEDY_MASK_PIPE_RST4, pix_pipe.WYFU_MASK_PIPE_3.qp16());
      /*p26.WODA*/ pix_pipe.WODA_MASK_PIPE_5.dff22(_SACU_CLKPIPEp, XELY_MASK_PIPE_SET5, WUJA_MASK_PIPE_RST5, pix_pipe.XETE_MASK_PIPE_4.qp16());
      /*p26.VUMO*/ pix_pipe.VUMO_MASK_PIPE_6.dff22(_SACU_CLKPIPEp, TYKO_MASK_PIPE_SET6, TENA_MASK_PIPE_RST6, pix_pipe.WODA_MASK_PIPE_5.qp16());
      /*p26.VAVA*/ pix_pipe.VAVA_MASK_PIPE_7.dff22(_SACU_CLKPIPEp, TUWU_MASK_PIPE_SET7, WUBU_MASK_PIPE_RST7, pix_pipe.VUMO_MASK_PIPE_6.qp16());
    }
  }

  //----------------------------------------
  // Pixel merge + emit

  {
    /*#p35.RAJY*/ wire RAJY_PIX_BG_LOp  = and2(pix_pipe.PYBO_BG_PIPE_A7.qp16(), pix_pipe.VYXE_LCDC_BGENn.qn08());
    /*#p35.TADE*/ wire TADE_PIX_BG_HIp  = and2(pix_pipe.SOHU_BG_PIPE_B7.qp16(), pix_pipe.VYXE_LCDC_BGENn.qn08());
    /*#p35.XULA*/ wire XULA_PIX_SP_LOp  = and2(pix_pipe.XYLO_LCDC_SPENn.qn08(), pix_pipe.WUFY_SPR_PIPE_A7.qp16());
    /*#p35.WOXA*/ wire WOXA_PIX_SP_HIp  = and2(pix_pipe.XYLO_LCDC_SPENn.qn08(), pix_pipe.VUPY_SPR_PIPE_B7.qp16());

    /*#p35.NULY*/ wire NULY_PIX_SP_MASKn = nor2(WOXA_PIX_SP_HIp, XULA_PIX_SP_LOp);

    /*#p35.RYFU*/ wire RYFU_MASK_BG0 = and2(RAJY_PIX_BG_LOp, pix_pipe.VAVA_MASK_PIPE_7.qp16());
    /*#p35.RUTA*/ wire RUTA_MASK_BG1 = and2(TADE_PIX_BG_HIp, pix_pipe.VAVA_MASK_PIPE_7.qp16());
    /*#p35.POKA*/ wire POKA_BGPIXELn = nor3(NULY_PIX_SP_MASKn, RUTA_MASK_BG1, RYFU_MASK_BG0);

    /*#p34.LOME*/ wire LOME_PAL_PIPE_7n = not1(pix_pipe.LYME_PAL_PIPE_7.qp16());
    /*#p34.LAFU*/ wire LAFU_OBP0PIXELn = nand2(LOME_PAL_PIPE_7n, POKA_BGPIXELn);
    /*#p34.LEKA*/ wire LEKA_OBP1PIXELn = nand2(pix_pipe.LYME_PAL_PIPE_7.qp16(), POKA_BGPIXELn);

    //----------
    // Sprite palette 0 lookup

    /*#p35.WELE*/ wire WELE_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    /*#p35.WOLO*/ wire WOLO_PIX_SP_LOp = not1(WELE_PIX_SP_LOn);
    /*#p35.VUMU*/ wire VUMU_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    /*#p35.WYRU*/ wire WYRU_PIX_SP_HIp = not1(VUMU_PIX_SP_HIn);

    /*#p35.LAVA*/ wire LAVA_MASK_OPB0  = not1(LAFU_OBP0PIXELn);

    /*#p35.VUGO*/ wire VUGO_PAL_OBP0A = and3(VUMU_PIX_SP_HIn, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VOLO*/ wire VOLO_PAL_OBP0B = and3(VUMU_PIX_SP_HIn, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VATA*/ wire VATA_PAL_OBP0C = and3(WYRU_PIX_SP_HIp, WELE_PIX_SP_LOn, LAVA_MASK_OPB0); // does not have vcc arm
    /*#p35.VYRO*/ wire VYRO_PAL_OBP0D = and3(WYRU_PIX_SP_HIp, WOLO_PIX_SP_LOp, LAVA_MASK_OPB0); // does not have vcc arm

    /*#p35.WUFU*/ wire WUFU_COL_OBP0_HI = amux4(pix_pipe.XANA_OBP0_D7n.qn07(), VYRO_PAL_OBP0D,
                                                pix_pipe.XYZE_OBP0_D5n.qn07(), VATA_PAL_OBP0C,
                                                pix_pipe.XALO_OBP0_D3n.qn07(), VOLO_PAL_OBP0B,
                                                pix_pipe.XUKY_OBP0_D1n.qn07(), VUGO_PAL_OBP0A);

    /*#p35.WALY*/ wire WALY_COL_OBP0_LO = amux4(pix_pipe.XUPO_OBP0_D6n.qn07(), VYRO_PAL_OBP0D,
                                                pix_pipe.XERU_OBP0_D4n.qn07(), VATA_PAL_OBP0C,
                                                pix_pipe.XOVA_OBP0_D2n.qn07(), VOLO_PAL_OBP0B,
                                                pix_pipe.XUFU_OBP0_D0n.qn07(), VUGO_PAL_OBP0A);

    //----------
    // Sprite palette 1 lookup

    /*#p35.MABY*/ wire MABY_PIX_SP_LOn = not1(XULA_PIX_SP_LOp);
    /*#p35.LYLE*/ wire LYLE_PIX_SP_LOp = not1(MABY_PIX_SP_LOn);
    /*#p35.MEXA*/ wire MEXA_PIX_SP_HIn = not1(WOXA_PIX_SP_HIp);
    /*#p35.LOZO*/ wire LOZO_PIX_SP_HIp = not1(MEXA_PIX_SP_HIn);

    /*#p35.LUKU*/ wire LUKU_MASK_OBP1  = not1(LEKA_OBP1PIXELn);

    /*p#35.LOPU*/ wire LOPU_PAL_OBP1A = and3(MEXA_PIX_SP_HIn, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /*p#35.LYKY*/ wire LYKY_PAL_OBP1B = and3(MEXA_PIX_SP_HIn, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm
    /*p#35.LARU*/ wire LARU_PAL_OBP1C = and3(LOZO_PIX_SP_HIp, MABY_PIX_SP_LOn, LUKU_MASK_OBP1); // does not have vcc arm
    /*p#35.LEDO*/ wire LEDO_PAL_OBP1D = and3(LOZO_PIX_SP_HIp, LYLE_PIX_SP_LOp, LUKU_MASK_OBP1); // does not have vcc arm

    /*#p35.MOKA*/ wire MOKA_COL_OBP1_HI = amux4(pix_pipe.LUXO_OBP1_D7n.qn07(), LEDO_PAL_OBP1D,
                                                pix_pipe.LUGU_OBP1_D5n.qn07(), LARU_PAL_OBP1C,
                                                pix_pipe.LOSE_OBP1_D3n.qn07(), LYKY_PAL_OBP1B,
                                                pix_pipe.LAWO_OBP1_D1n.qn07(), LOPU_PAL_OBP1A);

    /*#p35.MUFA*/ wire MUFA_COL_OBP1_LO = amux4(LEDO_PAL_OBP1D, pix_pipe.LEPU_OBP1_D6n.qn07(),
                                                LARU_PAL_OBP1C, pix_pipe.LUNE_OBP1_D4n.qn07(),
                                                LYKY_PAL_OBP1B, pix_pipe.MOSA_OBP1_D2n.qn07(),
                                                LOPU_PAL_OBP1A, pix_pipe.MOXY_OBP1_D0n.qn07());

    //----------
    // Background/window palette lookup

    /*p35.SOBA*/ wire SOBA_PIX_BG_LOn = not1(RAJY_PIX_BG_LOp);
    /*p35.NUPO*/ wire NUPO_PIX_BG_LOp = not1(SOBA_PIX_BG_LOn);
    /*p35.VYCO*/ wire VYCO_PIX_BG_HIn = not1(TADE_PIX_BG_HIp);
    /*p35.NALE*/ wire NALE_PIX_BG_HIp = not1(VYCO_PIX_BG_HIn);

    /*p35.MUVE*/ wire MUVE_MASK_BGP = not1(POKA_BGPIXELn);

    /*p35.POBU*/ wire POBU_PAL_BGPA = and3(VYCO_PIX_BG_HIn, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    /*p35.NUXO*/ wire NUXO_PAL_BGPB = and3(VYCO_PIX_BG_HIn, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm
    /*p35.NUMA*/ wire NUMA_PAL_BGPC = and3(NALE_PIX_BG_HIp, SOBA_PIX_BG_LOn, MUVE_MASK_BGP); // does not have vcc arm
    /*p35.NYPO*/ wire NYPO_PAL_BGPD = and3(NALE_PIX_BG_HIp, NUPO_PIX_BG_LOp, MUVE_MASK_BGP); // does not have vcc arm

    /*#p35.NELO*/ wire NELO_COL_BG_LO = amux4(NYPO_PAL_BGPD, pix_pipe.MOGY_BGP_D6n.qn07(),
                                              NUMA_PAL_BGPC, pix_pipe.MUKE_BGP_D4n.qn07(),
                                              NUXO_PAL_BGPB, pix_pipe.PYLU_BGP_D2n.qn07(),
                                              POBU_PAL_BGPA, pix_pipe.PAVO_BGP_D0n.qn07());

    /*#p35.NURA*/ wire NURA_COL_BG_HI = amux4(pix_pipe.MENA_BGP_D7n.qn07(), NYPO_PAL_BGPD,
                                              pix_pipe.MORU_BGP_D5n.qn07(), NUMA_PAL_BGPC,
                                              pix_pipe.MAXY_BGP_D3n.qn07(), NUXO_PAL_BGPB,
                                              pix_pipe.NUSY_BGP_D1n.qn07(), POBU_PAL_BGPA);

    //----------
    // Pixel merge and send

    /*#p35.PERO*/ wire _PERO_COL_LO = or3(NELO_COL_BG_LO, WALY_COL_OBP0_LO, MUFA_COL_OBP1_LO);
    /*#p35.PATY*/ wire _PATY_COL_HI = or3(NURA_COL_BG_HI, WUFU_COL_OBP0_HI, MOKA_COL_OBP1_HI);

    /*#p35.REMY*/ wire _REMY_LD0n = not1(_PERO_COL_LO);
    /*#p35.RAVO*/ wire _RAVO_LD1n = not1(_PATY_COL_HI);

    lcd_data0_delay.set(_REMY_LD0n);
    lcd_data1_delay.set(_RAVO_LD1n);

    // so q1 works but q2 has tiny errors? wat?
    PIN_LCD_DATA0.io_pin(lcd_data0_delay.q1(), lcd_data0_delay.q1());
    PIN_LCD_DATA1.io_pin(lcd_data1_delay.q1(), lcd_data1_delay.q1());
  }

  //----------------------------------------
  // FF40 LCDC

  {
    /*p22.WORU*/ wire _WORU_FF40n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VOCA*/ wire _VOCA_FF40p = not1(_WORU_FF40n);

    /*p23.WARU*/ wire _WARU_FF40_WRp = and2(_VOCA_FF40p, CUPA_CPU_WRp_xxxxEFGx);
    /*p23.XUBO*/ wire _XUBO_FF40_WRn = not1(_WARU_FF40_WRp);

    /*p01.XARE*/ wire _XARE_RSTn = not1(clk_reg.XORE_SYS_RSTp);
    /*#p23.VYXE*/ pix_pipe.VYXE_LCDC_BGENn   .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /* p23.XYLO*/ pix_pipe.XYLO_LCDC_SPENn   .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /* p23.XYMO*/ pix_pipe.XYMO_LCDC_SPSIZEn .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /* p23.XAFO*/ pix_pipe.XAFO_LCDC_BGMAPn  .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /* p23.WEXU*/ pix_pipe.WEXU_LCDC_BGTILEn .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /* p23.WYMO*/ pix_pipe.WYMO_LCDC_WINENn  .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /* p23.WOKY*/ pix_pipe.WOKY_LCDC_WINMAPn .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /* p23.XONA*/ pix_pipe.XONA_LCDC_LCDENn  .dff9(_XUBO_FF40_WRn, _XARE_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /*p23.VYRE*/ wire _VYRE_FF40_RDp = and2(_VOCA_FF40p, ASOT_CPU_RDp);
    /*p23.WYCE*/ wire _WYCE_FF40_RDn = not1(_VYRE_FF40_RDp);

    /*#p23.WYPO*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.VYXE_LCDC_BGENn.qp09());
    /*#p23.XERO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.XYLO_LCDC_SPENn.qp09());
    /* p23.WYJU*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.XYMO_LCDC_SPSIZEn.qp09());
    /* p23.WUKA*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.XAFO_LCDC_BGMAPn.qp09());
    /* p23.VOKE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.WEXU_LCDC_BGTILEn.qp09());
    /* p23.VATO*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.WYMO_LCDC_WINENn.qp09());
    /*#p23.VAHA*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.WOKY_LCDC_WINMAPn.qp09());
    /*#p23.XEBU*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_WYCE_FF40_RDn, pix_pipe.XONA_LCDC_LCDENn.qp09());
  }

  //----------------------------------------
  // FF41 STAT

  {
    /*p22.WOFA*/ wire _WOFA_FF41n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.VARY*/ wire _VARY_FF41p = not1(_WOFA_FF41n);

    /*p21.SEPA*/ wire _SEPA_FF41_WRp = and2(_VARY_FF41p, CUPA_CPU_WRp_xxxxEFGx);
    /*p21.RYVE*/ wire _RYVE_FF41_WRn = not1(_SEPA_FF41_WRp);

    /*p21.ROXE*/ pix_pipe.ROXE_STAT_HBI_ENn.dff9(_RYVE_FF41_WRn, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /*p21.RUFO*/ pix_pipe.RUFO_STAT_VBI_ENn.dff9(_RYVE_FF41_WRn, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /*p21.REFE*/ pix_pipe.REFE_STAT_OAI_ENn.dff9(_RYVE_FF41_WRn, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /*p21.RUGU*/ pix_pipe.RUGU_STAT_LYI_ENn.dff9(_RYVE_FF41_WRn, clk_reg.WESY_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());

    /*
    if (lcd_reg.PARU_VBLANKp_d4 && WODU_HBLANKp && SEPA_FF41_WRp) {
      printf("Stat write during hblank/vblank\n");
    }
    */

    /*p21.RYJU*/ wire _RYJU_FF41_WRn = not1(_SEPA_FF41_WRp);
    /*p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = or2(clk_reg.WESY_SYS_RSTn, _RYJU_FF41_WRn);

    // This latch isn't actually used as a latch, it's effectively an inverter.
    /*p21.RUPO*/ pix_pipe.RUPO_LYC_MATCH_LATCHn.nor_latch(_PAGO_LYC_MATCH_RST, lcd_reg.ROPO_LY_MATCH_SYNCp.qp17());

    /*p21.TOBE*/ wire _TOBE_FF41_RDp = and2(_VARY_FF41p, ASOT_CPU_RDp); // die AND
    /*p21.VAVE*/ wire _VAVE_FF41_RDn = not1(_TOBE_FF41_RDp); // die INV

    /*#p21.TEBY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_TOBE_FF41_RDp, pix_pipe.SADU_STAT_MODE0n);
    /*#p21.WUGA*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_TOBE_FF41_RDp, pix_pipe.XATY_STAT_MODE1n);
    /*#p21.SEGO*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_TOBE_FF41_RDp, pix_pipe.RUPO_LYC_MATCH_LATCHn.qp04());
    /* p21.PUZO*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_VAVE_FF41_RDn, pix_pipe.ROXE_STAT_HBI_ENn.qp09());
    /* p21.POFO*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_VAVE_FF41_RDn, pix_pipe.RUFO_STAT_VBI_ENn.qp09());
    /* p21.SASY*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_VAVE_FF41_RDn, pix_pipe.REFE_STAT_OAI_ENn.qp09());
    /* p21.POTE*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_VAVE_FF41_RDn, pix_pipe.RUGU_STAT_LYI_ENn.qp09());
  }

  //----------------------------------------
  // FF42 SCY
  {
    /*p22.WEBU*/ wire WEBU_FF42n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.WESA_A01p(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.XARO*/ wire XARO_FF42p = not1(WEBU_FF42n);

    /*p23.BEDY*/ wire BEDY_FF42_WRp = and2(XARO_FF42p, CUPA_CPU_WRp_xxxxEFGx);
    /*p23.CAVO*/ wire CAVO_FF42_WRn = not1(BEDY_FF42_WRp);

    /*p23.GAVE*/ pix_pipe.GAVE_SCY0n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /*p23.FYMO*/ pix_pipe.FYMO_SCY1n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /*p23.FEZU*/ pix_pipe.FEZU_SCY2n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /*p23.FUJO*/ pix_pipe.FUJO_SCY3n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /*p23.DEDE*/ pix_pipe.DEDE_SCY4n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /*p23.FOTY*/ pix_pipe.FOTY_SCY5n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /*p23.FOHA*/ pix_pipe.FOHA_SCY6n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /*p23.FUNY*/ pix_pipe.FUNY_SCY7n.dff9(CAVO_FF42_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /*p23.ANYP*/ wire ANYP_FF42_RDp = and2(XARO_FF42p, ASOT_CPU_RDp);
    /*p23.BUWY*/ wire BUWY_FF42_RDn = not1(ANYP_FF42_RDp);

    /*#p23.WARE*/ cpu_bus.BUS_CPU_D0p.tri_6nn(BUWY_FF42_RDn, pix_pipe.GAVE_SCY0n.qp09());
    /* p23.GOBA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(BUWY_FF42_RDn, pix_pipe.FYMO_SCY1n.qp09());
    /* p23.GONU*/ cpu_bus.BUS_CPU_D2p.tri_6nn(BUWY_FF42_RDn, pix_pipe.FEZU_SCY2n.qp09());
    /* p23.GODO*/ cpu_bus.BUS_CPU_D3p.tri_6nn(BUWY_FF42_RDn, pix_pipe.FUJO_SCY3n.qp09());
    /* p23.CUSA*/ cpu_bus.BUS_CPU_D4p.tri_6nn(BUWY_FF42_RDn, pix_pipe.DEDE_SCY4n.qp09());
    /* p23.GYZO*/ cpu_bus.BUS_CPU_D5p.tri_6nn(BUWY_FF42_RDn, pix_pipe.FOTY_SCY5n.qp09());
    /* p23.GUNE*/ cpu_bus.BUS_CPU_D6p.tri_6nn(BUWY_FF42_RDn, pix_pipe.FOHA_SCY6n.qp09());
    /* p23.GYZA*/ cpu_bus.BUS_CPU_D7p.tri_6nn(BUWY_FF42_RDn, pix_pipe.FUNY_SCY7n.qp09());
  }

  //----------------------------------------
  // FF43 SCX
  {
    /*p22.WAVU*/ wire WAVU_FF43n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.WESA_A01p(), cpu_bus.XUSY_A02n(), cpu_bus.XERA_A03n());
    /*p22.XAVY*/ wire XAVY_FF43p = not1(WAVU_FF43n);

    /*p23.ARUR*/ wire ARUR_FF43_WRp = and2(XAVY_FF43p, CUPA_CPU_WRp_xxxxEFGx);
    /*p23.AMUN*/ wire AMUN_FF43_WRn = not1(ARUR_FF43_WRp);

    /*p23.DATY*/ pix_pipe.DATY_SCX0n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /*p23.DUZU*/ pix_pipe.DUZU_SCX1n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /*p23.CYXU*/ pix_pipe.CYXU_SCX2n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /*p23.GUBO*/ pix_pipe.GUBO_SCX3n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /*p23.BEMY*/ pix_pipe.BEMY_SCX4n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /*p23.CUZY*/ pix_pipe.CUZY_SCX5n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /*p23.CABU*/ pix_pipe.CABU_SCX6n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /*p23.BAKE*/ pix_pipe.BAKE_SCX7n.dff9(AMUN_FF43_WRn, clk_reg.CUNU_SYS_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /*p23.AVOG*/ wire AVOG_FF43_RDp = and2(XAVY_FF43p, ASOT_CPU_RDp);
    /*p23.BEBA*/ wire BEBA_FF43_RDn = not1(AVOG_FF43_RDp);

    /*#p23.EDOS*/ cpu_bus.BUS_CPU_D0p.tri_6nn(BEBA_FF43_RDn, pix_pipe.DATY_SCX0n.qp09());
    /* p23.EKOB*/ cpu_bus.BUS_CPU_D1p.tri_6nn(BEBA_FF43_RDn, pix_pipe.DUZU_SCX1n.qp09());
    /* p23.CUGA*/ cpu_bus.BUS_CPU_D2p.tri_6nn(BEBA_FF43_RDn, pix_pipe.CYXU_SCX2n.qp09());
    /* p23.WONY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(BEBA_FF43_RDn, pix_pipe.GUBO_SCX3n.qp09());
    /* p23.CEDU*/ cpu_bus.BUS_CPU_D4p.tri_6nn(BEBA_FF43_RDn, pix_pipe.BEMY_SCX4n.qp09());
    /* p23.CATA*/ cpu_bus.BUS_CPU_D5p.tri_6nn(BEBA_FF43_RDn, pix_pipe.CUZY_SCX5n.qp09());
    /* p23.DOXE*/ cpu_bus.BUS_CPU_D6p.tri_6nn(BEBA_FF43_RDn, pix_pipe.CABU_SCX6n.qp09());
    /* p23.CASY*/ cpu_bus.BUS_CPU_D7p.tri_6nn(BEBA_FF43_RDn, pix_pipe.BAKE_SCX7n.qp09());
  }

  //----------------------------------------
  // FF47 BGP

  {
    /*p22.WYBO*/ wire WYBO_FF47n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.WESA_A01p(), cpu_bus.WALO_A02p(), cpu_bus.XERA_A03n());
    /*p22.WERA*/ wire WERA_FF47 = not1(WYBO_FF47n);

    /*p36.VELY*/ wire VELY_FF47_WR  = and2(CUPA_CPU_WRp_xxxxEFGx, WERA_FF47);
    /*p36.TEPO*/ wire TEPO_FF47_WRp = not1(VELY_FF47_WR);

    // This has to latch on the _rising_ edge of the clock, otherwise the timing glitches in m3_bgp_change are way off.

    /*#p36.PAVO*/ pix_pipe.PAVO_BGP_D0n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D0p.qp());
    /* p36.NUSY*/ pix_pipe.NUSY_BGP_D1n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D1p.qp());
    /* p36.PYLU*/ pix_pipe.PYLU_BGP_D2n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D2p.qp());
    /* p36.MAXY*/ pix_pipe.MAXY_BGP_D3n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D3p.qp());
    /* p36.MUKE*/ pix_pipe.MUKE_BGP_D4n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D4p.qp());
    /* p36.MORU*/ pix_pipe.MORU_BGP_D5n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D5p.qp());
    /* p36.MOGY*/ pix_pipe.MOGY_BGP_D6n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D6p.qp());
    /* p36.MENA*/ pix_pipe.MENA_BGP_D7n.dff8p(TEPO_FF47_WRp, cpu_bus.BUS_CPU_D7p.qp());

    /*p36.VUSO*/ wire VUSO_FF47_RD = and2(ASOT_CPU_RDp, WERA_FF47);
    /*p36.TEPY*/ wire TEPY_FF47_RDn = not1(VUSO_FF47_RD);

    /*#p36.RARO*/ cpu_bus.BUS_CPU_D0p.tri_6nn(TEPY_FF47_RDn, pix_pipe.PAVO_BGP_D0n.qp08());
    /* p36.PABA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(TEPY_FF47_RDn, pix_pipe.NUSY_BGP_D1n.qp08());
    /* p36.REDO*/ cpu_bus.BUS_CPU_D2p.tri_6nn(TEPY_FF47_RDn, pix_pipe.PYLU_BGP_D2n.qp08());
    /* p36.LOBE*/ cpu_bus.BUS_CPU_D3p.tri_6nn(TEPY_FF47_RDn, pix_pipe.MAXY_BGP_D3n.qp08());
    /* p36.LACE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(TEPY_FF47_RDn, pix_pipe.MUKE_BGP_D4n.qp08());
    /* p36.LYKA*/ cpu_bus.BUS_CPU_D5p.tri_6nn(TEPY_FF47_RDn, pix_pipe.MORU_BGP_D5n.qp08());
    /* p36.LODY*/ cpu_bus.BUS_CPU_D6p.tri_6nn(TEPY_FF47_RDn, pix_pipe.MOGY_BGP_D6n.qp08());
    /* p36.LARY*/ cpu_bus.BUS_CPU_D7p.tri_6nn(TEPY_FF47_RDn, pix_pipe.MENA_BGP_D7n.qp08());
  }

  //----------------------------------------
  // FF48 OBP0

  {
    /*p22.WETA*/ wire WETA_FF48n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.WEPO_A03p());
    /*p22.XAYO*/ wire XAYO_FF48 = not1(WETA_FF48n);

    /*p36.XOMA*/ wire XOMA_FF48_WR = and2(CUPA_CPU_WRp_xxxxEFGx, XAYO_FF48);
    /*p36.XELO*/ wire XELO_FF48_WRn = not1(XOMA_FF48_WR);

    /*p36.XUFU*/ pix_pipe.XUFU_OBP0_D0n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D0p.qp());
    /*p36.XUKY*/ pix_pipe.XUKY_OBP0_D1n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D1p.qp());
    /*p36.XOVA*/ pix_pipe.XOVA_OBP0_D2n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D2p.qp());
    /*p36.XALO*/ pix_pipe.XALO_OBP0_D3n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D3p.qp());
    /*p36.XERU*/ pix_pipe.XERU_OBP0_D4n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D4p.qp());
    /*p36.XYZE*/ pix_pipe.XYZE_OBP0_D5n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D5p.qp());
    /*p36.XUPO*/ pix_pipe.XUPO_OBP0_D6n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D6p.qp());
    /*p36.XANA*/ pix_pipe.XANA_OBP0_D7n.dff8p(XELO_FF48_WRn, cpu_bus.BUS_CPU_D7p.qp());

    /*p36.XUFY*/ wire XUFY_FF48_RD = and2(ASOT_CPU_RDp, XAYO_FF48);
    /*p36.XOZY*/ wire XOZY_FF48_RDn = not1(XUFY_FF48_RD);

    /*#p36.XARY*/ cpu_bus.BUS_CPU_D0p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XUFU_OBP0_D0n.qp08());
    /* p36.XOKE*/ cpu_bus.BUS_CPU_D1p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XUKY_OBP0_D1n.qp08());
    /* p36.XUNO*/ cpu_bus.BUS_CPU_D2p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XOVA_OBP0_D2n.qp08());
    /* p36.XUBY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XALO_OBP0_D3n.qp08());
    /* p36.XAJU*/ cpu_bus.BUS_CPU_D4p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XERU_OBP0_D4n.qp08());
    /* p36.XOBO*/ cpu_bus.BUS_CPU_D5p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XYZE_OBP0_D5n.qp08());
    /* p36.XAXA*/ cpu_bus.BUS_CPU_D6p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XUPO_OBP0_D6n.qp08());
    /* p36.XAWO*/ cpu_bus.BUS_CPU_D7p.tri_6nn(XOZY_FF48_RDn, pix_pipe.XANA_OBP0_D7n.qp08());
  }

  //----------------------------------------
  // FF49 OBP1

  {
    /*p22.VAMA*/ wire VAMA_FF49n = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.XENO_A01n(), cpu_bus.XUSY_A02n(), cpu_bus.WEPO_A03p());
    /*p22.TEGO*/ wire TEGO_FF49 = not1(VAMA_FF49n);

    /*p36.MYXE*/ wire MYXE_FF49_WR = and2(CUPA_CPU_WRp_xxxxEFGx, TEGO_FF49);
    /*p36.LEHO*/ wire LEHO_FF49_WRn = not1(MYXE_FF49_WR);

    /*p36.MOXY*/ pix_pipe.MOXY_OBP1_D0n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D0p.qp());
    /*p36.LAWO*/ pix_pipe.LAWO_OBP1_D1n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D1p.qp());
    /*p36.MOSA*/ pix_pipe.MOSA_OBP1_D2n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D2p.qp());
    /*p36.LOSE*/ pix_pipe.LOSE_OBP1_D3n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D3p.qp());
    /*p36.LUNE*/ pix_pipe.LUNE_OBP1_D4n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D4p.qp());
    /*p36.LUGU*/ pix_pipe.LUGU_OBP1_D5n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D5p.qp());
    /*p36.LEPU*/ pix_pipe.LEPU_OBP1_D6n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D6p.qp());
    /*p36.LUXO*/ pix_pipe.LUXO_OBP1_D7n.dff8p(LEHO_FF49_WRn, cpu_bus.BUS_CPU_D7p.qp());

    /*p36.MUMY*/ wire MUMY_FF49_RD = and2(ASOT_CPU_RDp, TEGO_FF49);
    /*p36.LOTE*/ wire LOTE_FF49_RDn = not1(MUMY_FF49_RD);

    /*#p36.LAJU*/ cpu_bus.BUS_CPU_D0p.tri_6nn(LOTE_FF49_RDn, pix_pipe.MOXY_OBP1_D0n.qp08());
    /* p36.LEPA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(LOTE_FF49_RDn, pix_pipe.LAWO_OBP1_D1n.qp08());
    /* p36.LODE*/ cpu_bus.BUS_CPU_D2p.tri_6nn(LOTE_FF49_RDn, pix_pipe.MOSA_OBP1_D2n.qp08());
    /* p36.LYZA*/ cpu_bus.BUS_CPU_D3p.tri_6nn(LOTE_FF49_RDn, pix_pipe.LOSE_OBP1_D3n.qp08());
    /* p36.LUKY*/ cpu_bus.BUS_CPU_D4p.tri_6nn(LOTE_FF49_RDn, pix_pipe.LUNE_OBP1_D4n.qp08());
    /* p36.LUGA*/ cpu_bus.BUS_CPU_D5p.tri_6nn(LOTE_FF49_RDn, pix_pipe.LUGU_OBP1_D5n.qp08());
    /* p36.LEBA*/ cpu_bus.BUS_CPU_D6p.tri_6nn(LOTE_FF49_RDn, pix_pipe.LEPU_OBP1_D6n.qp08());
    /* p36.LELU*/ cpu_bus.BUS_CPU_D7p.tri_6nn(LOTE_FF49_RDn, pix_pipe.LUXO_OBP1_D7n.qp08());
  }

  //----------------------------------------
  // FF4A WY

  {
    /*p22.WYVO*/ wire WYVO_FF4An = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.XOLA_A00n(), cpu_bus.WESA_A01p(), cpu_bus.XUSY_A02n(), cpu_bus.WEPO_A03p());
    /*p22.VYGA*/ wire VYGA_FF4Ap = not1(WYVO_FF4An);

    /*p23.WEKO*/ wire WEKO_FF4A_WRp = and2(VYGA_FF4Ap, CUPA_CPU_WRp_xxxxEFGx);
    /*p23.VEFU*/ wire VEFU_FF4A_WRn = not1(WEKO_FF4A_WRp);

    /*p23.NESO*/ pix_pipe.NESO_WY0n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /*p23.NYRO*/ pix_pipe.NYRO_WY1n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /*p23.NAGA*/ pix_pipe.NAGA_WY2n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /*p23.MELA*/ pix_pipe.MELA_WY3n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /*p23.NULO*/ pix_pipe.NULO_WY4n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /*p23.NENE*/ pix_pipe.NENE_WY5n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /*p23.NUKA*/ pix_pipe.NUKA_WY6n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /*p23.NAFU*/ pix_pipe.NAFU_WY7n.dff9(VEFU_FF4A_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /*p23.WAXU*/ wire WAXU_FF4A_RDp = and2(VYGA_FF4Ap, ASOT_CPU_RDp);
    /*p23.VOMY*/ wire VOMY_FF4A_RDn = not1(WAXU_FF4A_RDp);

    /*#p23.PUNU*/ cpu_bus.BUS_CPU_D0p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.NESO_WY0n.qp09());
    /* p23.PODA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.NYRO_WY1n.qp09());
    /* p23.PYGU*/ cpu_bus.BUS_CPU_D2p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.NAGA_WY2n.qp09());
    /* p23.LOKA*/ cpu_bus.BUS_CPU_D3p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.MELA_WY3n.qp09());
    /* p23.MEGA*/ cpu_bus.BUS_CPU_D4p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.NULO_WY4n.qp09());
    /* p23.PELA*/ cpu_bus.BUS_CPU_D5p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.NENE_WY5n.qp09());
    /* p23.POLO*/ cpu_bus.BUS_CPU_D6p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.NUKA_WY6n.qp09());
    /* p23.MERA*/ cpu_bus.BUS_CPU_D7p.tri_6nn(VOMY_FF4A_RDn, pix_pipe.NAFU_WY7n.qp09());
  }

  //----------------------------------------
  // FF4B WX

  {
    /*p22.WAGE*/ wire WAGE_FF4Bn = nand5(cpu_bus.WERO_FF4Xp(), cpu_bus.WADO_A00p(), cpu_bus.WESA_A01p(), cpu_bus.XUSY_A02n(), cpu_bus.WEPO_A03p());
    /*p22.VUMY*/ wire VUMY_FF4Bp = not1(WAGE_FF4Bn);

    /*p23.WUZA*/ wire WUZA_FF4B_WRp = and2(VUMY_FF4Bp, CUPA_CPU_WRp_xxxxEFGx);
    /*p23.VOXU*/ wire VOXU_FF4B_WRn = not1(WUZA_FF4B_WRp);

    /*p23.MYPA*/ pix_pipe.MYPA_WX0n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());
    /*p23.NOFE*/ pix_pipe.NOFE_WX1n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D1p.qp());
    /*p23.NOKE*/ pix_pipe.NOKE_WX2n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D2p.qp());
    /*p23.MEBY*/ pix_pipe.MEBY_WX3n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D3p.qp());
    /*p23.MYPU*/ pix_pipe.MYPU_WX4n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D4p.qp());
    /*p23.MYCE*/ pix_pipe.MYCE_WX5n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D5p.qp());
    /*p23.MUVO*/ pix_pipe.MUVO_WX6n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D6p.qp());
    /*p23.NUKU*/ pix_pipe.NUKU_WX7n.dff9(VOXU_FF4B_WRn, _WALU_SYS_RSTn, cpu_bus.BUS_CPU_D7p.qp());

    /*p23.WYZE*/ wire WYZE_FF4B_RDp = and2(VUMY_FF4Bp, ASOT_CPU_RDp);
    /*p23.VYCU*/ wire VYCU_FF4B_RDn = not1(WYZE_FF4B_RDp);

    /*#p23.LOVA*/ cpu_bus.BUS_CPU_D0p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.MYPA_WX0n.qp09());
    /* p23.MUKA*/ cpu_bus.BUS_CPU_D1p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.NOFE_WX1n.qp09());
    /* p23.MOKO*/ cpu_bus.BUS_CPU_D2p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.NOKE_WX2n.qp09());
    /* p23.LOLE*/ cpu_bus.BUS_CPU_D3p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.MEBY_WX3n.qp09());
    /* p23.MELE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.MYPU_WX4n.qp09());
    /* p23.MUFE*/ cpu_bus.BUS_CPU_D5p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.MYCE_WX5n.qp09());
    /* p23.MULY*/ cpu_bus.BUS_CPU_D6p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.MUVO_WX6n.qp09());
    /* p23.MARA*/ cpu_bus.BUS_CPU_D7p.tri_6nn(VYCU_FF4B_RDn, pix_pipe.NUKU_WX7n.qp09());
  }

  //------------------------------------------------------------------------------

  /* p01.ROSY*/ wire _ROSY_VID_RSTp = not1(clk_reg.XAPO_VID_RSTn);
  /* p01.LAPE*/ wire _LAPE_AxCxExGx = not1(clk_reg.ALET_xBxDxFxH);
  /* p27.TAVA*/ wire _TAVA_xBxDxFxH = not1(_LAPE_AxCxExGx);

  //----------------------------------------

  /* p27.TUKU*/ wire _TUKU_WIN_HITn = not1(pix_pipe.TOMU_WIN_HITp);

  /* p27.SOWO*/ wire _SOWO_SFETCH_RUNNINGn = not1(sprite_fetcher.TAKA_SFETCH_RUNNINGp.qp03());

  /* p27.TEKY*/ wire _TEKY_SFETCH_REQp = and4(sprite_store.FEPO_STORE_MATCHp, _TUKU_WIN_HITn, tile_fetcher.LYRY_BFETCH_DONEp, _SOWO_SFETCH_RUNNINGn);

  /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(_TAVA_xBxDxFxH, VYPO, _TEKY_SFETCH_REQp);
  /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(_LAPE_AxCxExGx, VYPO, sprite_fetcher.SOBU_SFETCH_REQp.qp17());

  /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp17(),  sprite_fetcher.SUDA_SFETCH_REQp.qn16());

  /*#p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn = nor3(_RYCE_SFETCH_TRIGp, _ROSY_VID_RSTp, lcd_reg.ATEJ_LINE_TRIGp);
  /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(sprite_fetcher.WUTY_SPRITE_DONEp, pix_pipe.TAVE_PRELOAD_DONE_TRIGp); // def nor
  /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RUNNING_SETn, _VEKU_SFETCH_RUNNING_RSTn);

  /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(sprite_fetcher.TESE_SFETCH_S2.qp17(), sprite_fetcher.TOXE_SFETCH_S0.qp17());
  /*#p29.TOMA*/ wire _TOMA_SFETCH_CLK_xBxDxFxH = nand2(_LAPE_AxCxExGx, _TAME_SFETCH_CLK_GATE);

  /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0   .dff17(_TOMA_SFETCH_CLK_xBxDxFxH,                   _SECA_SFETCH_RUNNING_SETn,            sprite_fetcher.TOXE_SFETCH_S0.qn16());
  /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0_D1.dff17(_LAPE_AxCxExGx,                              VYPO,                                 sprite_fetcher.TOXE_SFETCH_S0.qp17());
  /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1   .dff17(sprite_fetcher.TOXE_SFETCH_S0.qn16(),    _SECA_SFETCH_RUNNING_SETn,            sprite_fetcher.TULY_SFETCH_S1.qn16());
  /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1_D2.dff17(_TAVA_xBxDxFxH,                              pix_pipe.XYMU_RENDERINGn.qn03(),  sprite_fetcher.TULY_SFETCH_S1.qp17());
  /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1_D4.dff17(_TAVA_xBxDxFxH,                              pix_pipe.XYMU_RENDERINGn.qn03(),  sprite_fetcher.TOBU_SFETCH_S1_D2.qp17());
  /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1_D5.dff17(_LAPE_AxCxExGx,                              pix_pipe.XYMU_RENDERINGn.qn03(),  sprite_fetcher.VONU_SFETCH_S1_D4.qp17());
  /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2   .dff17(sprite_fetcher.TULY_SFETCH_S1.qn16(),    _SECA_SFETCH_RUNNING_SETn,            sprite_fetcher.TESE_SFETCH_S2.qn16());

  //------------------------------------------------------------------------------

  /*p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(clk_reg.ALET_xBxDxFxH, tile_fetcher.MOCE_BFETCH_DONEn);

  /*p27.LAXU*/ tile_fetcher.LAXU_BFETCH_S0   .dff17(_LEBO_AxCxExGx,                             pix_pipe.NYXU_FETCH_TRIGn,       tile_fetcher.LAXU_BFETCH_S0.qn16());
  /*p27.MESU*/ tile_fetcher.MESU_BFETCH_S1   .dff17(tile_fetcher.LAXU_BFETCH_S0.qn16(),     pix_pipe.NYXU_FETCH_TRIGn,       tile_fetcher.MESU_BFETCH_S1.qn16());
  /*p27.NYVA*/ tile_fetcher.NYVA_BFETCH_S2   .dff17(tile_fetcher.MESU_BFETCH_S1.qn16(),     pix_pipe.NYXU_FETCH_TRIGn,       tile_fetcher.NYVA_BFETCH_S2.qn16());
  /*p27.LYZU*/ tile_fetcher.LYZU_BFETCH_S0_D1.dff17(clk_reg.ALET_xBxDxFxH,                  pix_pipe.XYMU_RENDERINGn.qn03(), tile_fetcher.LAXU_BFETCH_S0.qp17());

  /*p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG = nor2(pix_pipe.MOSU_WIN_FETCH_TRIGp, pix_pipe.LOBY_RENDERINGn);

  /*p24.NYKA*/ tile_fetcher.NYKA_FETCH_DONE_P11.dff17(clk_reg.ALET_xBxDxFxH, _NAFY_RENDERING_AND_NOT_WIN_TRIG,      tile_fetcher.LYRY_BFETCH_DONEp);
  /*p24.PORY*/ tile_fetcher.PORY_FETCH_DONE_P12.dff17(clk_reg.MYVO_AxCxExGx, _NAFY_RENDERING_AND_NOT_WIN_TRIG,      tile_fetcher.NYKA_FETCH_DONE_P11.qp17());
  /*p24.PYGO*/ tile_fetcher.PYGO_FETCH_DONE_P13.dff17(clk_reg.ALET_xBxDxFxH, pix_pipe.XYMU_RENDERINGn.qn03(),   tile_fetcher.PORY_FETCH_DONE_P12.qp17());

  /*p24.POKY*/ tile_fetcher.POKY_PRELOAD_LATCHp.nor_latch(tile_fetcher.PYGO_FETCH_DONE_P13.qp17(), pix_pipe.LOBY_RENDERINGn);

  /*p27.LOVY*/ tile_fetcher.LOVY_BG_FETCH_DONEp  .dff17(clk_reg.MYVO_AxCxExGx, pix_pipe.NYXU_FETCH_TRIGn,     tile_fetcher.LYRY_BFETCH_DONEp);
  /*p27.LURY*/ wire _LURY_BG_FETCH_DONEn = and2(tile_fetcher.LOVY_BG_FETCH_DONEp.qn16(), pix_pipe.XYMU_RENDERINGn.qn03());
  /*p27.LONY*/ tile_fetcher.LONY_BG_FETCH_RUNNINGp.nand_latch(pix_pipe.NYXU_FETCH_TRIGn, _LURY_BG_FETCH_DONEn);

  //------------------------------------------------------------------------------

  wire _PESU_VCC = 1;

  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  /* p07.SEMY*/ wire _SEMY_ADDR_XX0X = nor4(cpu_bus.BUS_CPU_A07.qp(), cpu_bus.BUS_CPU_A06.qp(), cpu_bus.BUS_CPU_A05.qp(), cpu_bus.BUS_CPU_A04.qp());
  /* p07.SAPA*/ wire _SAPA_ADDR_XXXF = and4(cpu_bus.BUS_CPU_A00.qp(), cpu_bus.BUS_CPU_A01.qp(), cpu_bus.BUS_CPU_A02.qp(), cpu_bus.BUS_CPU_A03.qp());
  /* p07.REFA*/ wire _REFA_FF0F_WRn  = nand4(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, cpu_bus.SYKE_FF00_FFFFp(), TAPU_CPU_WRp_xxxxEFGx); // schematic wrong, is NAND
  /* p02.ROTU*/ wire _ROTU_FF0F_WRp  = not1(_REFA_FF0F_WRn);

  /* p02.LETY*/ wire _LETY_INT_VBL_ACKn  = not1(int_reg.PIN_CPU_ACK_VBLANK.qp());
  /* p02.LEJA*/ wire _LEJA_INT_STAT_ACKn = not1(int_reg.PIN_CPU_ACK_STAT.qp());
  /* p02.LESA*/ wire _LESA_INT_TIM_ACKn  = not1(int_reg.PIN_CPU_ACK_TIMER.qp());
  /* p02.LUFE*/ wire _LUFE_INT_SER_ACKn  = not1(int_reg.PIN_CPU_ACK_SERIAL.qp());
  /* p02.LAMO*/ wire _LAMO_INT_JOY_ACKn  = not1(int_reg.PIN_CPU_ACK_JOYPAD.qp());

  /* p02.MYZU*/ wire _MYZU_FF0F_SET0n = nand3(_ROTU_FF0F_WRp, _LETY_INT_VBL_ACKn,  cpu_bus.BUS_CPU_D0p.qp());
  /* p02.MODY*/ wire _MODY_FF0F_SET1n = nand3(_ROTU_FF0F_WRp, _LEJA_INT_STAT_ACKn, cpu_bus.BUS_CPU_D1p.qp());
  /* p02.PYHU*/ wire _PYHU_FF0F_SET2n = nand3(_ROTU_FF0F_WRp, _LESA_INT_TIM_ACKn,  cpu_bus.BUS_CPU_D2p.qp());
  /* p02.TOME*/ wire _TOME_FF0F_SET3n = nand3(_ROTU_FF0F_WRp, _LUFE_INT_SER_ACKn,  cpu_bus.BUS_CPU_D3p.qp());
  /* p02.TOGA*/ wire _TOGA_FF0F_SET4n = nand3(_ROTU_FF0F_WRp, _LAMO_INT_JOY_ACKn,  cpu_bus.BUS_CPU_D4p.qp());

  /*#p02.MUXE*/ wire _MUXE_INT0_WRn = or2(cpu_bus.BUS_CPU_D0p.qp(), _REFA_FF0F_WRn);
  /* p02.NABE*/ wire _NABE_INT1_WRn = or2(cpu_bus.BUS_CPU_D1p.qp(), _REFA_FF0F_WRn);
  /* p02.RAKE*/ wire _RAKE_INT2_WRn = or2(cpu_bus.BUS_CPU_D2p.qp(), _REFA_FF0F_WRn);
  /* p02.SULO*/ wire _SULO_INT3_WRn = or2(cpu_bus.BUS_CPU_D3p.qp(), _REFA_FF0F_WRn);
  /* p02.SEME*/ wire _SEME_INT4_WRn = or2(cpu_bus.BUS_CPU_D4p.qp(), _REFA_FF0F_WRn);

  /*#p02.LYTA*/ wire _LYTA_FF0F_RST0n = and3(_MUXE_INT0_WRn, _LETY_INT_VBL_ACKn,  clk_reg.ALUR_SYS_RSTn);
  /* p02.MOVU*/ wire _MOVU_FF0F_RST1n = and3(_NABE_INT1_WRn, _LEJA_INT_STAT_ACKn, clk_reg.ALUR_SYS_RSTn);
  /* p02.PYGA*/ wire _PYGA_FF0F_RST2n = and3(_RAKE_INT2_WRn, _LESA_INT_TIM_ACKn,  clk_reg.ALUR_SYS_RSTn);
  /* p02.TUNY*/ wire _TUNY_FF0F_RST3n = and3(_SULO_INT3_WRn, _LUFE_INT_SER_ACKn,  clk_reg.ALUR_SYS_RSTn);
  /* p02.TYME*/ wire _TYME_FF0F_RST4n = and3(_SEME_INT4_WRn, _LAMO_INT_JOY_ACKn,  clk_reg.ALUR_SYS_RSTn);

  /* p02.LOPE*/ int_reg.LOPE_FF0F_D0p.dff22(lcd_reg.VYPU_INT_VBLANKp,            _MYZU_FF0F_SET0n, _LYTA_FF0F_RST0n, _PESU_VCC);
  /* p02.LALU*/ int_reg.LALU_FF0F_D1p.dff22(pix_pipe.VOTY_INT_STATp,             _MODY_FF0F_SET1n, _MOVU_FF0F_RST1n, _PESU_VCC);
  /* p02.NYBO*/ int_reg.NYBO_FF0F_D2p.dff22(tim_reg.MOBA_TIMER_OVERFLOWp.qp17(), _PYHU_FF0F_SET2n, _PYGA_FF0F_RST2n, _PESU_VCC);
  /* p02.UBUL*/ int_reg.UBUL_FF0F_D3p.dff22(ser_reg.CALY_INT_SERp.qp17(),        _TOME_FF0F_SET3n, _TUNY_FF0F_RST3n, _PESU_VCC);
  /* p02.ULAK*/ int_reg.ULAK_FF0F_D4p.dff22(joypad.ASOK_INT_JOYp,                _TOGA_FF0F_SET4n, _TYME_FF0F_RST4n, _PESU_VCC);

  int_reg.PIN_CPU_INT_VBLANK.set(int_reg.LOPE_FF0F_D0p.qp16());
  int_reg.PIN_CPU_INT_STAT  .set(int_reg.LALU_FF0F_D1p.qp16());
  int_reg.PIN_CPU_INT_TIMER .set(int_reg.NYBO_FF0F_D2p.qp16());
  int_reg.PIN_CPU_INT_SERIAL.set(int_reg.UBUL_FF0F_D3p.qp16());
  int_reg.PIN_CPU_INT_JOYPAD.set(int_reg.ULAK_FF0F_D4p.qp16());

  // pass gates? does look like a transparent latch here...

  /* p07.ROLO*/ wire _ROLO_HOLDn = nand4(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, cpu_bus.SYKE_FF00_FFFFp(), TEDO_CPU_RDp); // schematic wrong, is NAND

  // FIXME this inversion fixes a bunch of tests...
  // MATY is one of those big yellow latchy things

  /* p02.MATY*/ int_reg.MATY_FF0F_L0p.tp_latch(!_ROLO_HOLDn, int_reg.LOPE_FF0F_D0p.qp16()); // OUTPUT ON RUNG 10
  /* p02.MOPO*/ int_reg.MOPO_FF0F_L1p.tp_latch(!_ROLO_HOLDn, int_reg.LALU_FF0F_D1p.qp16()); // OUTPUT ON RUNG 10
  /* p02.PAVY*/ int_reg.PAVY_FF0F_L2p.tp_latch(!_ROLO_HOLDn, int_reg.NYBO_FF0F_D2p.qp16()); // OUTPUT ON RUNG 10
  /* p02.NEJY*/ int_reg.NEJY_FF0F_L3p.tp_latch(!_ROLO_HOLDn, int_reg.UBUL_FF0F_D3p.qp16()); // OUTPUT ON RUNG 10
  /* p02.NUTY*/ int_reg.NUTY_FF0F_L4p.tp_latch(!_ROLO_HOLDn, int_reg.ULAK_FF0F_D4p.qp16()); // OUTPUT ON RUNG 10

  /* p02.POLA*/ wire _POLA_FF0F_RD  = not1(_ROLO_HOLDn);
  /*#p02.NELA*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_POLA_FF0F_RD, int_reg.MATY_FF0F_L0p.qn10());
  /*#p02.NABO*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_POLA_FF0F_RD, int_reg.MOPO_FF0F_L1p.qn10());
  /*#p02.ROVA*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_POLA_FF0F_RD, int_reg.PAVY_FF0F_L2p.qn10());
  /*#p02.PADO*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_POLA_FF0F_RD, int_reg.NEJY_FF0F_L3p.qn10());
  /*#p02.PEGY*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_POLA_FF0F_RD, int_reg.NUTY_FF0F_L4p.qn10());

  //------------------------------------------------------------------------------

  cpu_bus.PIN_CPU_ADDR_HIp.set(cpu_bus.SYRO_FE00_FFFFp());
  cpu_bus.PIN_CPU_BOOTp.set(TUTU_ADDR_BOOTp);

  //------------------------------------------------------------------------------
  // ext_bus.tock(top);


  /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_ADDR_EXT_AND_NOT_VRAM, UMUT_MODE_DBG1p);
  /*p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM = not1(TEXO_ADDR_EXT_AND_NOT_VRAM);
  /*p08.LAGU*/ wire _LAGU = and_or3(cpu_bus.PIN_CPU_RDp.qp(), _LEVO_ADDR_INT_OR_ADDR_VRAM, cpu_bus.PIN_CPU_WRp.qp());
  /*p08.LYWE*/ wire _LYWE = not1(_LAGU);
  /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);

  /*p08.TOVA*/ wire _TOVA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);

  //----------------------------------------
  // Ext pins

  {
    /*p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(dma_reg.LUMA_DMA_CARTp, _MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, _TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, UNOR_MODE_DBG2p);
    ext_bus.PIN_EXT_RDn.io_pin(_UGAC_RD_A, _URUN_RD_D);
  }

  {

    /*p08.MEXO*/ wire _MEXO_CPU_WRn_ABCDxxxH = not1(APOV_CPU_WRp_xxxxEFGx);
    /*p08.NEVY*/ wire _NEVY = or2(_MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, dma_reg.LUMA_DMA_CARTp);
    /*p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, _TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, UNOR_MODE_DBG2p);
    ext_bus.PIN_EXT_WRn.io_pin(_UVER_WR_A, _USUF_WR_D);
  }


  {
    /*p08.SOGY*/ wire _SOGY_A14n = not1(cpu_bus.BUS_CPU_A14.qp());
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and3(cpu_bus.BUS_CPU_A13.qp(), _SOGY_A14n, cpu_bus.BUS_CPU_A15.qp());
    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = and_or3(cpu_bus.BUS_CPU_A15.qp(), cpu_bus.BUS_CPU_A14.qp(), _TUMA_CART_RAM);

    /*p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH = and3(ABUZ_xxCDEFGH, _TYNU_ADDR_RAM, cpu_bus.TUNA_0000_FDFFp());
    /*p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.MARU_DMA_A15n.qn07(), _TOZA_PIN_EXT_CS_A_xxCDEFGH);
    ext_bus.PIN_EXT_CSn.io_pin(_TYHO_PIN_EXT_CS_A_xxCDEFGH, _TYHO_PIN_EXT_CS_A_xxCDEFGH);
  }


  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n  = not1(UMUT_MODE_DBG1p);
    /*p08.LOXO*/ wire _LOXO_HOLDn = and_or3(_MULE_MODE_DBG1n, TEXO_ADDR_EXT_AND_NOT_VRAM, UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire _LASY_HOLDp = not1(_LOXO_HOLDn);
    /*p08.MATE*/ wire _MATE_HOLDn = not1(_LASY_HOLDp);

    /* p08.ALOR*/ ext_bus.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A00.qp());
    /* p08.APUR*/ ext_bus.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A01.qp());
    /* p08.ALYR*/ ext_bus.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A02.qp());
    /* p08.ARET*/ ext_bus.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A03.qp());
    /* p08.AVYS*/ ext_bus.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A04.qp());
    /* p08.ATEV*/ ext_bus.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A05.qp());
    /* p08.AROS*/ ext_bus.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A06.qp());
    /* p08.ARYM*/ ext_bus.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A07.qp());
    /* p08.LUNO*/ ext_bus.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A08.qp());
    /* p08.LYSA*/ ext_bus.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A09.qp());
    /* p08.PATE*/ ext_bus.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A10.qp());
    /* p08.LUMY*/ ext_bus.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A11.qp());
    /* p08.LOBU*/ ext_bus.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A12.qp());
    /* p08.LONU*/ ext_bus.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A13.qp());
    /* p08.NYRE*/ ext_bus.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A14.qp());


    /* p08.AMET*/ wire _AMET_A00p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.NAKY_DMA_A00p.qp17(), ext_bus.ALOR_EXT_ADDR_LATCH_00p.qp08());
    /* p08.ATOL*/ wire _ATOL_A01p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.PYRO_DMA_A01p.qp17(), ext_bus.APUR_EXT_ADDR_LATCH_01p.qp08());
    /* p08.APOK*/ wire _APOK_A02p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.NEFY_DMA_A02p.qp17(), ext_bus.ALYR_EXT_ADDR_LATCH_02p.qp08());
    /* p08.AMER*/ wire _AMER_A03p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.MUTY_DMA_A03p.qp17(), ext_bus.ARET_EXT_ADDR_LATCH_03p.qp08());
    /* p08.ATEM*/ wire _ATEM_A04p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.NYKO_DMA_A04p.qp17(), ext_bus.AVYS_EXT_ADDR_LATCH_04p.qp08());
    /* p08.ATOV*/ wire _ATOV_A05p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.PYLO_DMA_A05p.qp17(), ext_bus.ATEV_EXT_ADDR_LATCH_05p.qp08());
    /* p08.ATYR*/ wire _ATYR_A06p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.NUTO_DMA_A06p.qp17(), ext_bus.AROS_EXT_ADDR_LATCH_06p.qp08());
    /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.MUGU_DMA_A07p.qp17(), ext_bus.ARYM_EXT_ADDR_LATCH_07p.qp08());
    /*#p08.MANO*/ wire _MANO_A08p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.NAFA_DMA_A08n.qn07(), ext_bus.LUNO_EXT_ADDR_LATCH_08p.qp08());
    /* p08.MASU*/ wire _MASU_A09p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.PYNE_DMA_A09n.qn07(), ext_bus.LYSA_EXT_ADDR_LATCH_09p.qp08());
    /* p08.PAMY*/ wire _PAMY_A10p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.PARA_DMA_A10n.qn07(), ext_bus.PATE_EXT_ADDR_LATCH_10p.qp08());
    /* p08.MALE*/ wire _MALE_A11p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.NYDO_DMA_A11n.qn07(), ext_bus.LUMY_EXT_ADDR_LATCH_11p.qp08());
    /* p08.MOJY*/ wire _MOJY_A12p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.NYGY_DMA_A12n.qn07(), ext_bus.LOBU_EXT_ADDR_LATCH_12p.qp08());
    /* p08.MUCE*/ wire _MUCE_A13p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.PULA_DMA_A13n.qn07(), ext_bus.LONU_EXT_ADDR_LATCH_13p.qp08());
    /* p08.PEGE*/ wire _PEGE_A14p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.POKU_DMA_A14n.qn07(), ext_bus.NYRE_EXT_ADDR_LATCH_14p.qp08());

    /*p08.KUPO*/ wire _KUPO = nand2(_AMET_A00p, _TOVA_MODE_DBG2n);
    /*p08.CABA*/ wire _CABA = nand2(_ATOL_A01p, _TOVA_MODE_DBG2n);
    /*p08.BOKU*/ wire _BOKU = nand2(_APOK_A02p, _TOVA_MODE_DBG2n);
    /*p08.BOTY*/ wire _BOTY = nand2(_AMER_A03p, _TOVA_MODE_DBG2n);
    /*p08.BYLA*/ wire _BYLA = nand2(_ATEM_A04p, _TOVA_MODE_DBG2n);
    /*p08.BADU*/ wire _BADU = nand2(_ATOV_A05p, _TOVA_MODE_DBG2n);
    /*p08.CEPU*/ wire _CEPU = nand2(_ATYR_A06p, _TOVA_MODE_DBG2n);
    /*p08.DEFY*/ wire _DEFY = nand2(_ASUR_A07p, _TOVA_MODE_DBG2n);
    /*p08.MYNY*/ wire _MYNY = nand2(_MANO_A08p, _TOVA_MODE_DBG2n);
    /*p08.MUNE*/ wire _MUNE = nand2(_MASU_A09p, _TOVA_MODE_DBG2n);
    /*p08.ROXU*/ wire _ROXU = nand2(_PAMY_A10p, _TOVA_MODE_DBG2n);
    /*p08.LEPY*/ wire _LEPY = nand2(_MALE_A11p, _TOVA_MODE_DBG2n);
    /*p08.LUCE*/ wire _LUCE = nand2(_MOJY_A12p, _TOVA_MODE_DBG2n);
    /*p08.LABE*/ wire _LABE = nand2(_MUCE_A13p, _TOVA_MODE_DBG2n);
    /*p08.PUHE*/ wire _PUHE = nand2(_PEGE_A14p, _TOVA_MODE_DBG2n);

    /*p08.KOTY*/ wire _KOTY = nor2 (_AMET_A00p, UNOR_MODE_DBG2p);
    /*p08.COTU*/ wire _COTU = nor2 (_ATOL_A01p, UNOR_MODE_DBG2p);
    /*p08.BAJO*/ wire _BAJO = nor2 (_APOK_A02p, UNOR_MODE_DBG2p);
    /*p08.BOLA*/ wire _BOLA = nor2 (_AMER_A03p, UNOR_MODE_DBG2p);
    /*p08.BEVO*/ wire _BEVO = nor2 (_ATEM_A04p, UNOR_MODE_DBG2p);
    /*p08.AJAV*/ wire _AJAV = nor2 (_ATOV_A05p, UNOR_MODE_DBG2p);
    /*p08.CYKA*/ wire _CYKA = nor2 (_ATYR_A06p, UNOR_MODE_DBG2p);
    /*p08.COLO*/ wire _COLO = nor2 (_ASUR_A07p, UNOR_MODE_DBG2p);
    /*p08.MEGO*/ wire _MEGO = nor2 (_MANO_A08p, UNOR_MODE_DBG2p);
    /*p08.MENY*/ wire _MENY = nor2 (_MASU_A09p, UNOR_MODE_DBG2p);
    /*p08.RORE*/ wire _RORE = nor2 (_PAMY_A10p, UNOR_MODE_DBG2p);
    /*p08.LYNY*/ wire _LYNY = nor2 (_MALE_A11p, UNOR_MODE_DBG2p);
    /*p08.LOSO*/ wire _LOSO = nor2 (_MOJY_A12p, UNOR_MODE_DBG2p);
    /*p08.LEVA*/ wire _LEVA = nor2 (_MUCE_A13p, UNOR_MODE_DBG2p);
    /*p08.PAHY*/ wire _PAHY = nor2 (_PEGE_A14p, UNOR_MODE_DBG2p);

    ext_bus.PIN_EXT_A00p.io_pin(_KUPO, _KOTY);
    ext_bus.PIN_EXT_A01p.io_pin(_CABA, _COTU);
    ext_bus.PIN_EXT_A02p.io_pin(_BOKU, _BAJO);
    ext_bus.PIN_EXT_A03p.io_pin(_BOTY, _BOLA);
    ext_bus.PIN_EXT_A04p.io_pin(_BYLA, _BEVO);
    ext_bus.PIN_EXT_A05p.io_pin(_BADU, _AJAV);
    ext_bus.PIN_EXT_A06p.io_pin(_CEPU, _CYKA);
    ext_bus.PIN_EXT_A07p.io_pin(_DEFY, _COLO);
    ext_bus.PIN_EXT_A08p.io_pin(_MYNY, _MEGO);
    ext_bus.PIN_EXT_A09p.io_pin(_MUNE, _MENY);
    ext_bus.PIN_EXT_A10p.io_pin(_ROXU, _RORE);
    ext_bus.PIN_EXT_A11p.io_pin(_LEPY, _LYNY);
    ext_bus.PIN_EXT_A12p.io_pin(_LUCE, _LOSO);
    ext_bus.PIN_EXT_A13p.io_pin(_LABE, _LEVA);
    ext_bus.PIN_EXT_A14p.io_pin(_PUHE, _PAHY);
  }


  {
    // A15 is "special"

    /* p08.RYCA*/ wire _RYCA_MODE_DBG2n = not1(UNOR_MODE_DBG2p);
    /* p08.SOBY*/ wire _SOBY_A15n = nor2(cpu_bus.BUS_CPU_A15.qp(), TUTU_ADDR_BOOTp);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx = nand2(ABUZ_xxCDEFGH, _SOBY_A15n);
    /* p08.TAZY*/ wire _TAZY_A15p = mux2p(dma_reg.LUMA_DMA_CARTp, dma_reg.MARU_DMA_A15n.qn07(), _SEPY_A15p_ABxxxxxx);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n = nand2(_TAZY_A15p, _RYCA_MODE_DBG2n);
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n = nor2 (_TAZY_A15p, UNOR_MODE_DBG2p);

    ext_bus.PIN_EXT_A15p.io_pin(_SUZE_PIN_EXT_A15n, _RULO_PIN_EXT_A15n);
  }

  //----------------------------------------
  // Ext data out


  // CPU data bus -> external data bus
  {
    // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.REDU*/ wire _REDU_CPU_RDn = not1(TEDO_CPU_RDp);
    /*p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(UNOR_MODE_DBG2p, _REDU_CPU_RDn, _MOTY_CPU_EXT_RD);
    /*p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

    /*p25.RUXA*/ wire _RUXA = nand2(cpu_bus.BUS_CPU_D0p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RUJA*/ wire _RUJA = nand2(cpu_bus.BUS_CPU_D1p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RABY*/ wire _RABY = nand2(cpu_bus.BUS_CPU_D2p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RERA*/ wire _RERA = nand2(cpu_bus.BUS_CPU_D3p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RORY*/ wire _RORY = nand2(cpu_bus.BUS_CPU_D4p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RYVO*/ wire _RYVO = nand2(cpu_bus.BUS_CPU_D5p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAFY*/ wire _RAFY = nand2(cpu_bus.BUS_CPU_D6p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAVU*/ wire _RAVU = nand2(cpu_bus.BUS_CPU_D7p.qp(), _LULA_CBD_TO_EPDp);

    /*p08.RUNE*/ wire _RUNE = nor2 (cpu_bus.BUS_CPU_D0p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYPU*/ wire _RYPU = nor2 (cpu_bus.BUS_CPU_D1p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SULY*/ wire _SULY = nor2 (cpu_bus.BUS_CPU_D2p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SEZE*/ wire _SEZE = nor2 (cpu_bus.BUS_CPU_D3p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RESY*/ wire _RESY = nor2 (cpu_bus.BUS_CPU_D4p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.TAMU*/ wire _TAMU = nor2 (cpu_bus.BUS_CPU_D5p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.ROGY*/ wire _ROGY = nor2 (cpu_bus.BUS_CPU_D6p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYDA*/ wire _RYDA = nor2 (cpu_bus.BUS_CPU_D7p.qp(), _RORU_CBD_TO_EPDn);

    ext_bus.PIN_EXT_D00p.io_pin(_RUXA, _RUNE, _LULA_CBD_TO_EPDp);
    ext_bus.PIN_EXT_D01p.io_pin(_RUJA, _RYPU, _LULA_CBD_TO_EPDp);
    ext_bus.PIN_EXT_D02p.io_pin(_RABY, _SULY, _LULA_CBD_TO_EPDp);
    ext_bus.PIN_EXT_D03p.io_pin(_RERA, _SEZE, _LULA_CBD_TO_EPDp);
    ext_bus.PIN_EXT_D04p.io_pin(_RORY, _RESY, _LULA_CBD_TO_EPDp);
    ext_bus.PIN_EXT_D05p.io_pin(_RYVO, _TAMU, _LULA_CBD_TO_EPDp);
    ext_bus.PIN_EXT_D06p.io_pin(_RAFY, _ROGY, _LULA_CBD_TO_EPDp);
    ext_bus.PIN_EXT_D07p.io_pin(_RAVU, _RYDA, _LULA_CBD_TO_EPDp);
  }

  //----------------------------------------
  // Ext data in
  {
    // -> ext bus
    /*p08.LAVO*/ wire _LAVO_HOLDn = nand3(cpu_bus.PIN_CPU_RDp.qp(), TEXO_ADDR_EXT_AND_NOT_VRAM, cpu_bus.PIN_CPU_LATCH_EXT.qp());

    // Ext pin -> Ext latch
    /*#p08.SOMA*/ ext_bus.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D00p.qn());
    /* p08.RONY*/ ext_bus.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D01p.qn());
    /* p08.RAXY*/ ext_bus.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D02p.qn());
    /* p08.SELO*/ ext_bus.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D03p.qn());
    /* p08.SODY*/ ext_bus.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D04p.qn());
    /* p08.SAGO*/ ext_bus.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D05p.qn());
    /* p08.RUPA*/ ext_bus.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D06p.qn());
    /* p08.SAZY*/ ext_bus.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn, ext_bus.PIN_EXT_D07p.qn());

    // Ext latch -> int bus
    /*#p08.RYMA*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_LAVO_HOLDn, ext_bus.SOMA_EXT_DATA_LATCH_D0n.qp08());
    /* p08.RUVO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_LAVO_HOLDn, ext_bus.RONY_EXT_DATA_LATCH_D1n.qp08());
    /* p08.RYKO*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_LAVO_HOLDn, ext_bus.RAXY_EXT_DATA_LATCH_D2n.qp08());
    /* p08.TAVO*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_LAVO_HOLDn, ext_bus.SELO_EXT_DATA_LATCH_D3n.qp08());
    /* p08.TEPE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_LAVO_HOLDn, ext_bus.SODY_EXT_DATA_LATCH_D4n.qp08());
    /* p08.SAFO*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_LAVO_HOLDn, ext_bus.SAGO_EXT_DATA_LATCH_D5n.qp08());
    /* p08.SEVU*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_LAVO_HOLDn, ext_bus.RUPA_EXT_DATA_LATCH_D6n.qp08());
    /* p08.TAJU*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_LAVO_HOLDn, ext_bus.SAZY_EXT_DATA_LATCH_D7n.qp08());
  }

  //------------------------------------------------------------------------------
  // OAM signals

  /* p28.XYNY*/ wire _XYNY_ABCDxxxx = not1(clk_reg.MOPA_xxxxEFGH);

  /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(oam_bus.BUS_OAM_A0n.qp()); // -> WAFO, YLYC, WUKU
  /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);

  /* p28.AJON*/ wire _AJON_PPU_OAM_ENp = and2(dma_reg.BOGE_DMA_RUNNINGn, pix_pipe.XYMU_RENDERINGn.qn03()); // def AND. ppu can read oam when there's rendering but no dma

  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(sprite_scanner.ACYL_SCANNINGp, clk_reg.XOCE_xBCxxFGx); // schematic wrong, is def nand2
  /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(sprite_fetcher.WEFY_SPR_READp);
  /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn = not1(CATY_LATCH_EXTp);
  /*#p28.BOTA*/ wire _BOTA_CPU_OAM_OEn  = nand3(_BOFE_LATCH_EXTn, cpu_bus.SARO_FE00_FEFFp(), ASOT_CPU_RDp); // Schematic wrong, this is NAND

  //probe(16, "PIN_CPU_LATCH_EXT", cpu_bus.PIN_CPU_LATCH_EXT.qp());
  //probe(17, "SARO_FE00_FEFFp",   cpu_bus.SARO_FE00_FEFFp());
  //probe(18, "ASOT_CPU_RDp",      ASOT_CPU_RDp);
  //probe(15, "BOTA_CPU_OAM_OEn",  _BOTA_CPU_OAM_OEn);

  /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_OEn); // def and
  /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);

  /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor3(dma_reg.MATU_DMA_RUNNINGp.qp17(), sprite_scanner.ACYL_SCANNINGp, _AJON_PPU_OAM_ENp); // def nor
  /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(cpu_bus.SARO_FE00_FEFFp(), _AJUJ_OAM_BUSYn); // def and

  // CUFE looks like BYHA minus an inverter

  bool _COTA_OAM_CLKn;
  {
    /* p29.XYSO*/ wire _XYSO_xBCDxFGH = not1(clk_reg.WOJO_AxxxExxx);
    /* p25.AVER*/ wire _AVER_SCAN_OAM_CLK = nand2(sprite_scanner.ACYL_SCANNINGp, _XYSO_xBCDxFGH);
    /* p25.XUJY*/ wire _XUJY_PPU_OAM_CLK  = not1(sprite_fetcher.VAPE_FETCH_OAM_CLK);
    /* p25.CUFE*/ wire _CUFE_OAM_CLK  = not1(or_and3(cpu_bus.SARO_FE00_FEFFp(), dma_reg.MATU_DMA_RUNNINGp.qp17(), clk_reg.MOPA_xxxxEFGH));

    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_SCAN_OAM_CLK, _XUJY_PPU_OAM_CLK, _CUFE_OAM_CLK);
    /* p25.COTA*/ _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
  }

  //----------------------------------------
  // OAM pins
  // PIN_CPU_LATCH_EXT _blocks_ DMA from writing to OAM? wat?

  {
    oam_bus.PIN_OAM_CLK.set(_COTA_OAM_CLKn);

    /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);
    oam_bus.PIN_OAM_OE.set(_ZODO_OAM_OEn);

    /*p04.MAKA*/ oam_bus.MAKA_HOLD_MEMp.dff17(clk_reg.ZEME_AxCxExGx, clk_reg.CUNU_SYS_RSTn, CATY_LATCH_EXTp);

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WRp = nor2(clk_reg.UVYT_ABCDxxxx, oam_bus.MAKA_HOLD_MEMp.qp17()); // def nor2
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp  = and2(dma_reg.MATU_DMA_RUNNINGp.qp17(), _NAXY_DMA_OAM_WRp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp      = and_or3(_AMAB_CPU_OAM_ENp, CUPA_CPU_WRp_xxxxEFGx, _POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire _YNYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, _WAFO_OAM_A0n);
    /*p28.YLYC*/ wire _YLYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, _GEKA_OAM_A0p);
    /*p28.ZOFE*/ wire _ZOFE_OAM_A_WRn = not1(_YNYC_OAM_A_WRp);
    /*p28.ZONE*/ wire _ZONE_OAM_B_WRn = not1(_YLYC_OAM_B_WRp);
    oam_bus.PIN_OAM_WR_A.set(_ZOFE_OAM_A_WRn);
    oam_bus.PIN_OAM_WR_B.set(_ZONE_OAM_B_WRn);
  }

  //----------------------------------------
  // OAM address mux

  {
    /*p28.APAR*/ wire _APAR_SCAN_OAM_RDn = not1(sprite_scanner.ACYL_SCANNINGp);
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not1(dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or3(sprite_scanner.ACYL_SCANNINGp, pix_pipe.XYMU_RENDERINGn.qn03(), dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not1(_AJON_PPU_OAM_ENp);

    //probe(20, "_APAR_SCAN_OAM_RDn", _APAR_SCAN_OAM_RDn);
    //probe(21, "_DUGA_DMA_OAM_RDn ", _DUGA_DMA_OAM_RDn );
    //probe(22, "_ASAM_CPU_OAM_RDn ", _ASAM_CPU_OAM_RDn );
    //probe(23, "_BETE_PPU_OAM_RDn ", _BETE_PPU_OAM_RDn );

    // Scanner addr -> OAM addr
    /*p28.GEFY*/ oam_bus.BUS_OAM_A0n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ oam_bus.BUS_OAM_A1n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ oam_bus.BUS_OAM_A2n.tri_6nn(_APAR_SCAN_OAM_RDn, sprite_scanner.YFEL_SCAN0.qp17());
    /*p28.GEMA*/ oam_bus.BUS_OAM_A3n.tri_6nn(_APAR_SCAN_OAM_RDn, sprite_scanner.WEWY_SCAN1.qp17());
    /*p28.FUTO*/ oam_bus.BUS_OAM_A4n.tri_6nn(_APAR_SCAN_OAM_RDn, sprite_scanner.GOSO_SCAN2.qp17());
    /*p28.FAKU*/ oam_bus.BUS_OAM_A5n.tri_6nn(_APAR_SCAN_OAM_RDn, sprite_scanner.ELYN_SCAN3.qp17());
    /*p28.GAMA*/ oam_bus.BUS_OAM_A6n.tri_6nn(_APAR_SCAN_OAM_RDn, sprite_scanner.FAHA_SCAN4.qp17());
    /*p28.GOBY*/ oam_bus.BUS_OAM_A7n.tri_6nn(_APAR_SCAN_OAM_RDn, sprite_scanner.FONY_SCAN5.qp17());

    // DMA addr -> OAM addr
    /*p28.FODO*/ oam_bus.BUS_OAM_A0n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.NAKY_DMA_A00p.qp17());
    /*p28.FESA*/ oam_bus.BUS_OAM_A1n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.PYRO_DMA_A01p.qp17());
    /*p28.FAGO*/ oam_bus.BUS_OAM_A2n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.NEFY_DMA_A02p.qp17());
    /*p28.FYKY*/ oam_bus.BUS_OAM_A3n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.MUTY_DMA_A03p.qp17());
    /*p28.ELUG*/ oam_bus.BUS_OAM_A4n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.NYKO_DMA_A04p.qp17());
    /*p28.EDOL*/ oam_bus.BUS_OAM_A5n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.PYLO_DMA_A05p.qp17());
    /*p28.FYDU*/ oam_bus.BUS_OAM_A6n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.NUTO_DMA_A06p.qp17());
    /*p28.FETU*/ oam_bus.BUS_OAM_A7n.tri_6nn(_DUGA_DMA_OAM_RDn, dma_reg.MUGU_DMA_A07p.qp17());

    // CPU addr -> OAM addr
    /*p28.GARO*/ oam_bus.BUS_OAM_A0n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A00.qp());
    /*p28.WACU*/ oam_bus.BUS_OAM_A1n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A01.qp());
    /*p28.GOSE*/ oam_bus.BUS_OAM_A2n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A02.qp());
    /*p28.WAPE*/ oam_bus.BUS_OAM_A3n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A03.qp());
    /*p28.FEVU*/ oam_bus.BUS_OAM_A4n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A04.qp());
    /*p28.GERA*/ oam_bus.BUS_OAM_A5n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A05.qp());
    /*p28.WAXA*/ oam_bus.BUS_OAM_A6n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A06.qp());
    /*p28.FOBY*/ oam_bus.BUS_OAM_A7n.tri_6nn(_ASAM_CPU_OAM_RDn, cpu_bus.BUS_CPU_A07.qp());

    // PPU addr -> OAM addr
    /*p28.GECA*/ oam_bus.BUS_OAM_A0n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ oam_bus.BUS_OAM_A1n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ oam_bus.BUS_OAM_A2n.tri_6nn(_BETE_PPU_OAM_RDn, sprite_store.SPR_TRI_I0p.qp());
    /*p28.GYKA*/ oam_bus.BUS_OAM_A3n.tri_6nn(_BETE_PPU_OAM_RDn, sprite_store.SPR_TRI_I1p.qp());
    /*p28.FABY*/ oam_bus.BUS_OAM_A4n.tri_6nn(_BETE_PPU_OAM_RDn, sprite_store.SPR_TRI_I2p.qp());
    /*p28.FACO*/ oam_bus.BUS_OAM_A5n.tri_6nn(_BETE_PPU_OAM_RDn, sprite_store.SPR_TRI_I3p.qp());
    /*p28.FUGU*/ oam_bus.BUS_OAM_A6n.tri_6nn(_BETE_PPU_OAM_RDn, sprite_store.SPR_TRI_I4p.qp());
    /*p28.FYKE*/ oam_bus.BUS_OAM_A7n.tri_6nn(_BETE_PPU_OAM_RDn, sprite_store.SPR_TRI_I5p.qp());
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and2(cpu_bus.SARO_FE00_FEFFp(), CUPA_CPU_WRp_xxxxEFGx);
    /*p28.WUJE*/ oam_bus.WUJE_CPU_OAM_WRn.nor_latch(_XYNY_ABCDxxxx, _XUTO_CPU_OAM_WRp);

    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WRp = not1(oam_bus.WUJE_CPU_OAM_WRn.qp04());
    /*p28.APAG*/ wire _APAG_CBD_TO_OBDp = amux2(_XUPA_CPU_OAM_WRp, _AMAB_CPU_OAM_ENp, _AJUJ_OAM_BUSYn, cpu_bus.ADAH_FE00_FEFFn());
    /*p28.AZUL*/ wire _AZUL_CBD_TO_OBDn = not1(_APAG_CBD_TO_OBDp);

    /*p28.ZAXA*/ oam_bus.BUS_OAM_DA0n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZAKY*/ oam_bus.BUS_OAM_DA1n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WULE*/ oam_bus.BUS_OAM_DA2n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZOZO*/ oam_bus.BUS_OAM_DA3n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUFO*/ oam_bus.BUS_OAM_DA4n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZATO*/ oam_bus.BUS_OAM_DA5n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D5p.qp());
    /*p28.YVUC*/ oam_bus.BUS_OAM_DA6n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZUFE*/ oam_bus.BUS_OAM_DA7n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D7p.qp());

    /*p28.ZAMY*/ oam_bus.BUS_OAM_DB0n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZOPU*/ oam_bus.BUS_OAM_DB1n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WYKY*/ oam_bus.BUS_OAM_DB2n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZAJA*/ oam_bus.BUS_OAM_DB3n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUGA*/ oam_bus.BUS_OAM_DB4n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZUMO*/ oam_bus.BUS_OAM_DB5n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D5p.qp());
    /*p28.XYTO*/ oam_bus.BUS_OAM_DB6n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZYFA*/ oam_bus.BUS_OAM_DB7n.tri_6nn(_AZUL_CBD_TO_OBDn, cpu_bus.BUS_CPU_D7p.qp());
  }

  // EBD -> OPD
  {
    /*p25.RALO*/ wire _RALO_EXT_D0p = not1(ext_bus.PIN_EXT_D00p.qn());
    /*p25.TUNE*/ wire _TUNE_EXT_D1p = not1(ext_bus.PIN_EXT_D01p.qn());
    /*p25.SERA*/ wire _SERA_EXT_D2p = not1(ext_bus.PIN_EXT_D02p.qn());
    /*p25.TENU*/ wire _TENU_EXT_D3p = not1(ext_bus.PIN_EXT_D03p.qn());
    /*p25.SYSA*/ wire _SYSA_EXT_D4p = not1(ext_bus.PIN_EXT_D04p.qn());
    /*p25.SUGY*/ wire _SUGY_EXT_D5p = not1(ext_bus.PIN_EXT_D05p.qn());
    /*p25.TUBE*/ wire _TUBE_EXT_D6p = not1(ext_bus.PIN_EXT_D06p.qn());
    /*p25.SYZO*/ wire _SYZO_EXT_D7p = not1(ext_bus.PIN_EXT_D07p.qn());

    /*p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(dma_reg.LUMA_DMA_CARTp);
    /*p25.WASA*/ oam_bus.BUS_OAM_DA0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BOMO*/ oam_bus.BUS_OAM_DA1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BASA*/ oam_bus.BUS_OAM_DA2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CAKO*/ oam_bus.BUS_OAM_DA3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BUMA*/ oam_bus.BUS_OAM_DA4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUPY*/ oam_bus.BUS_OAM_DA5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BASY*/ oam_bus.BUS_OAM_DA6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BAPE*/ oam_bus.BUS_OAM_DA7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);

    /*p25.WEJO*/ oam_bus.BUS_OAM_DB0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BUBO*/ oam_bus.BUS_OAM_DB1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BETU*/ oam_bus.BUS_OAM_DB2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CYME*/ oam_bus.BUS_OAM_DB3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BAXU*/ oam_bus.BUS_OAM_DB4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUHU*/ oam_bus.BUS_OAM_DB5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BYNY*/ oam_bus.BUS_OAM_DB6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BYPY*/ oam_bus.BUS_OAM_DB7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);
  }

  // VBD -> OPD
  {
    /*p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(dma_reg.LUFA_DMA_VRAMp);
    /*p28.WUZU*/ oam_bus.BUS_OAM_DA0n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AXER*/ oam_bus.BUS_OAM_DA1n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D1p.qp());
    /*p28.ASOX*/ oam_bus.BUS_OAM_DA2n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D2p.qp());
    /*p28.CETU*/ oam_bus.BUS_OAM_DA3n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D3p.qp());
    /*p28.ARYN*/ oam_bus.BUS_OAM_DA4n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D4p.qp());
    /*p28.ACOT*/ oam_bus.BUS_OAM_DA5n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D5p.qp());
    /*p28.CUJE*/ oam_bus.BUS_OAM_DA6n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ATER*/ oam_bus.BUS_OAM_DA7n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D7p.qp());

    /*p28.WOWA*/ oam_bus.BUS_OAM_DB0n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AVEB*/ oam_bus.BUS_OAM_DB1n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D1p.qp());
    /*p28.AMUH*/ oam_bus.BUS_OAM_DB2n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D2p.qp());
    /*p28.COFO*/ oam_bus.BUS_OAM_DB3n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D3p.qp());
    /*p28.AZOZ*/ oam_bus.BUS_OAM_DB4n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D4p.qp());
    /*p28.AGYK*/ oam_bus.BUS_OAM_DB5n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D5p.qp());
    /*p28.BUSE*/ oam_bus.BUS_OAM_DB6n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ANUM*/ oam_bus.BUS_OAM_DB7n.tri_6nn(_AZAR_VBD_TO_OBDn, vram_bus.BUS_VRAM_D7p.qp());
  }

  //----------------------------------------
  // OAM data in

  // OPD -> OBL
  {
    /* p29.YDYV*/ oam_bus.YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA0n.qp());
    /* p29.YCEB*/ oam_bus.YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA1n.qp());
    /* p29.ZUCA*/ oam_bus.ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA2n.qp());
    /* p29.WONE*/ oam_bus.WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA3n.qp());
    /* p29.ZAXE*/ oam_bus.ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA4n.qp());
    /* p29.XAFU*/ oam_bus.XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA5n.qp());
    /* p29.YSES*/ oam_bus.YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA6n.qp());
    /* p29.ZECA*/ oam_bus.ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DA7n.qp());

    /*#p31.XYKY*/ oam_bus.XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB0n.qp());
    /* p31.YRUM*/ oam_bus.YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB1n.qp());
    /* p31.YSEX*/ oam_bus.YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB2n.qp());
    /* p31.YVEL*/ oam_bus.YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB3n.qp());
    /* p31.WYNO*/ oam_bus.WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB4n.qp());
    /* p31.CYRA*/ oam_bus.CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB5n.qp());
    /* p31.ZUVE*/ oam_bus.ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB6n.qp());
    /* p31.ECED*/ oam_bus.ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp, oam_bus.BUS_OAM_DB7n.qp());
  }

  // OBL -> temp
  {
    /*p29.YWOK*/ wire _YWOK_OAM_CLKn = not1(_COTA_OAM_CLKn);
    /*p29.YWOK*/ wire _YWOK_OAM_CLKp = not1(_YWOK_OAM_CLKn);

    /*#p29.XUSO*/ oam_bus.XUSO_OAM_DA0p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.YDYV_OAM_LATCH_DA0n.qp08());
    /* p29.XEGU*/ oam_bus.XEGU_OAM_DA1p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.YCEB_OAM_LATCH_DA1n.qp08());
    /* p29.YJEX*/ oam_bus.YJEX_OAM_DA2p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.ZUCA_OAM_LATCH_DA2n.qp08());
    /* p29.XYJU*/ oam_bus.XYJU_OAM_DA3p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.WONE_OAM_LATCH_DA3n.qp08());
    /* p29.YBOG*/ oam_bus.YBOG_OAM_DA4p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.ZAXE_OAM_LATCH_DA4n.qp08());
    /* p29.WYSO*/ oam_bus.WYSO_OAM_DA5p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.XAFU_OAM_LATCH_DA5n.qp08());
    /* p29.XOTE*/ oam_bus.XOTE_OAM_DA6p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.YSES_OAM_LATCH_DA6n.qp08());
    /* p29.YZAB*/ oam_bus.YZAB_OAM_DA7p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, oam_bus.ZECA_OAM_LATCH_DA7n.qp08());

    /* p31.XEGA*/ wire _XEGA_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /* p31.XEGA*/ wire _XEGA_OAM_CLKn = not1(_XEGA_OAM_CLKp);

    /* p31.YLOR*/ oam_bus.YLOR_OAM_DB0p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.XYKY_OAM_LATCH_DB0n.qp08());
    /* p31.ZYTY*/ oam_bus.ZYTY_OAM_DB1p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.YRUM_OAM_LATCH_DB1n.qp08());
    /* p31.ZYVE*/ oam_bus.ZYVE_OAM_DB2p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.YSEX_OAM_LATCH_DB2n.qp08());
    /* p31.ZEZY*/ oam_bus.ZEZY_OAM_DB3p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.YVEL_OAM_LATCH_DB3n.qp08());
    /* p31.GOMO*/ oam_bus.GOMO_OAM_DB4p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.WYNO_OAM_LATCH_DB4n.qp08());
    /* p31.BAXO*/ oam_bus.BAXO_OAM_DB5p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.CYRA_OAM_LATCH_DB5n.qp08());
    /* p31.YZOS*/ oam_bus.YZOS_OAM_DB6p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.ZUVE_OAM_LATCH_DB6n.qp08());
    /* p31.DEPO*/ oam_bus.DEPO_OAM_DB7p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, oam_bus.ECED_OAM_LATCH_DB7n.qp08());
  }

  // OBL -> CBD
  {
    /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
    /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);

    /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
    /* p29.YFAP*/ cpu_bus.BUS_CPU_D0p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YDYV_OAM_LATCH_DA0n.qn10());
    /* p29.XELE*/ cpu_bus.BUS_CPU_D1p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YCEB_OAM_LATCH_DA1n.qn10());
    /* p29.YPON*/ cpu_bus.BUS_CPU_D2p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZUCA_OAM_LATCH_DA2n.qn10());
    /* p29.XUVO*/ cpu_bus.BUS_CPU_D3p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.WONE_OAM_LATCH_DA3n.qn10());
    /* p29.ZYSA*/ cpu_bus.BUS_CPU_D4p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZAXE_OAM_LATCH_DA4n.qn10());
    /* p29.YWEG*/ cpu_bus.BUS_CPU_D5p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.XAFU_OAM_LATCH_DA5n.qn10());
    /* p29.XABU*/ cpu_bus.BUS_CPU_D6p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.YSES_OAM_LATCH_DA6n.qn10());
    /* p29.YTUX*/ cpu_bus.BUS_CPU_D7p.tri10_np(_WUME_OBL_TO_CBDn, oam_bus.ZECA_OAM_LATCH_DA7n.qn10());

    /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
    /* p31.XACA*/ cpu_bus.BUS_CPU_D0p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.XYKY_OAM_LATCH_DB0n.qn10());
    /* p31.XAGU*/ cpu_bus.BUS_CPU_D1p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YRUM_OAM_LATCH_DB1n.qn10());
    /* p31.XEPU*/ cpu_bus.BUS_CPU_D2p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YSEX_OAM_LATCH_DB2n.qn10());
    /* p31.XYGU*/ cpu_bus.BUS_CPU_D3p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.YVEL_OAM_LATCH_DB3n.qn10());
    /* p31.XUNA*/ cpu_bus.BUS_CPU_D4p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.WYNO_OAM_LATCH_DB4n.qn10());
    /* p31.DEVE*/ cpu_bus.BUS_CPU_D5p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.CYRA_OAM_LATCH_DB5n.qn10());
    /* p31.ZEHA*/ cpu_bus.BUS_CPU_D6p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.ZUVE_OAM_LATCH_DB6n.qn10());
    /* p31.FYRA*/ cpu_bus.BUS_CPU_D7p.tri10_np(_WEWU_OBL_TO_CBDn, oam_bus.ECED_OAM_LATCH_DB7n.qn10());
  }

  //------------------------------------------------------------------------------

  /*#p25.TEFA*/ wire _TEFA_8000_9FFFp = nor2(cpu_bus.SYRO_FE00_FFFFp(), TEXO_ADDR_EXT_AND_NOT_VRAM);
  /*#p25.SOSE*/ wire _SOSE_8000_9FFFp = and2(cpu_bus.BUS_CPU_A15.qp(), _TEFA_8000_9FFFp);

  /*#p25.ROPY*/ wire _ROPY_RENDERINGn = not1(pix_pipe.XYMU_RENDERINGn.qn03());

  /*#p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(vram_bus.PIN_VRAM_CSn.qn());
  /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(_SOSE_8000_9FFFp, ABUZ_xxCDEFGH);

  /*#p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2p(TUTO_DBG_VRAMp, _TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp);
  /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp = and2(_TOLE_VRAM_RDp, _ROPY_RENDERINGn);

  /*#p25.TAVY*/ wire _TAVY_MOEp = not1(vram_bus.PIN_VRAM_OEn.qn());
  /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2(_SOSE_8000_9FFFp, cpu_bus.PIN_CPU_WRp.qp());  // Schematic wrong, second input is PIN_CPU_WRp
  /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2p(TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);
  /*#p25.RUVY*/ wire _RUVY_VRAM_WRp    = not1(_SALE_CPU_VRAM_WRn);
  /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp = and2(_SERE_CPU_VRM_RDp, _RUVY_VRAM_WRp);
  /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn = not1(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp = not1(_RYJE_CBD_TO_VPDn);
  /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp = or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn = not1(_RELA_CBD_TO_VPDp);

  /*#p25.XANE*/ wire _XANE_VRAM_LOCKn = nor2(dma_reg.LUFA_DMA_VRAMp, pix_pipe.XYMU_RENDERINGn.qn03());

  /*#p25.RACO*/ wire _RACO_DBG_VRAMn = not1(TUTO_DBG_VRAMp);
  (void)_RACO_DBG_VRAMn;

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and2(_SOSE_8000_9FFFp, APOV_CPU_WRp_xxxxEFGx);
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not1(vram_bus.PIN_VRAM_WRn.qn());
    /*p25.TYJY*/ wire _TYJY_VRAM_WRp = mux2p(TUTO_DBG_VRAMp, _SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp);

    /*p25.SOHY*/ wire _SOHY_MWRn = nand2(_TYJY_VRAM_WRp, _SERE_CPU_VRM_RDp);

    /*p25.TAXY*/ wire _TAXY_MWRn_A = and2(_SOHY_MWRn, _RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire _SOFY_MWRn_D = or2(_SOHY_MWRn, TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire _SYSY_MWRp_A = not1(_TAXY_MWRn_A);
    /*p25.RAGU*/ wire _RAGU_MWRp_D = not1(_SOFY_MWRn_D);

    vram_bus.PIN_VRAM_WRn.io_pin(_SYSY_MWRp_A, _RAGU_MWRp_D);
  }

  {
    /*p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn = nand2(_SALE_CPU_VRAM_WRn, _XANE_VRAM_LOCKn);

    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not1(sprite_fetcher.SOHO_SPR_VRAM_RDp);
    /*p25.APAM*/ wire _APAM_DMA_VRAM_RDn = not1(dma_reg.LUFA_DMA_VRAMp);
    /*p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn = not1(tile_fetcher.LONY_BG_FETCH_RUNNINGp.qp03());

    /*p25.RACU*/ wire _RACU_MOEn = and4(_RYLU_CPU_VRAM_RDn, _RAWA_SPR_VRAM_RDn, _MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAM_RDn); // def and

    /*p25.SEMA*/ wire _SEMA_MOEn_A = and2(_RACU_MOEn, _RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire _RUTE_MOEn_D = or2(_RACU_MOEn, TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire _REFO_MOEn_A = not1(_SEMA_MOEn_A);
    /*p25.SAHA*/ wire _SAHA_MOEn_D = not1(_RUTE_MOEn_D);

    vram_bus.PIN_VRAM_OEn.io_pin(_REFO_MOEn_A, _SAHA_MOEn_D);
  }

  {
    /*#p25.SUTU*/ wire _SUTU_MCSn = nor4(tile_fetcher.LENA_BGW_VRM_RDp,
                                         dma_reg.LUFA_DMA_VRAMp,
                                         sprite_fetcher.TEXY_SPR_READ_VRAMp,
                                         _SERE_CPU_VRM_RDp);

    /*p25.TODE*/ wire _TODE_MCSn_A = and2(_SUTU_MCSn, _RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire _SEWO_MCSn_D = or2(_SUTU_MCSn, TUTO_DBG_VRAMp);
    /*p25.SOKY*/ wire _SOKY_MCSp_A = not1(_TODE_MCSn_A);
    /*p25.SETY*/ wire _SETY_MCSp_D = not1(_SEWO_MCSn_D);

    vram_bus.PIN_VRAM_CSn.io_pin(_SOKY_MCSp_A, _SETY_MCSp_D);
  }

  //----------------------------------------
  // VRAM address

  // CPU address -> vram address
  {
    /* p25.XEDU*/ wire _XEDU_CPU_VRAM_RDn = not1(_XANE_VRAM_LOCKn);
    /* p25.XAKY*/ vram_bus.BUS_VRAM_A00n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A00.qp());
    /* p25.XUXU*/ vram_bus.BUS_VRAM_A01n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A01.qp());
    /* p25.XYNE*/ vram_bus.BUS_VRAM_A02n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A02.qp());
    /* p25.XODY*/ vram_bus.BUS_VRAM_A03n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A03.qp());
    /* p25.XECA*/ vram_bus.BUS_VRAM_A04n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A04.qp());
    /* p25.XOBA*/ vram_bus.BUS_VRAM_A05n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A05.qp());
    /* p25.XOPO*/ vram_bus.BUS_VRAM_A06n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A06.qp());
    /* p25.XYBO*/ vram_bus.BUS_VRAM_A07n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A07.qp());
    /* p25.RYSU*/ vram_bus.BUS_VRAM_A08n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A08.qp());
    /* p25.RESE*/ vram_bus.BUS_VRAM_A09n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A09.qp());
    /* p25.RUSE*/ vram_bus.BUS_VRAM_A10n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A10.qp());
    /* p25.RYNA*/ vram_bus.BUS_VRAM_A11n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A11.qp());
    /* p25.RUMO*/ vram_bus.BUS_VRAM_A12n.tri_6nn(_XEDU_CPU_VRAM_RDn, cpu_bus.BUS_CPU_A12.qp());
  }

  // DMA address -> vram address
  {
    /* p04.AHOC*/ wire _AHOC_DMA_VRAM_RDn = not1(dma_reg.LUFA_DMA_VRAMp);
    /* p04.ECAL*/ vram_bus.BUS_VRAM_A00n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.NAKY_DMA_A00p.qp17());
    /* p04.EGEZ*/ vram_bus.BUS_VRAM_A01n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.PYRO_DMA_A01p.qp17());
    /* p04.FUHE*/ vram_bus.BUS_VRAM_A02n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.NEFY_DMA_A02p.qp17());
    /* p04.FYZY*/ vram_bus.BUS_VRAM_A03n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.MUTY_DMA_A03p.qp17());
    /* p04.DAMU*/ vram_bus.BUS_VRAM_A04n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.NYKO_DMA_A04p.qp17());
    /* p04.DAVA*/ vram_bus.BUS_VRAM_A05n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.PYLO_DMA_A05p.qp17());
    /* p04.ETEG*/ vram_bus.BUS_VRAM_A06n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.NUTO_DMA_A06p.qp17());
    /*#p04.EREW*/ vram_bus.BUS_VRAM_A07n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.MUGU_DMA_A07p.qp17());
    /*#p04.EVAX*/ vram_bus.BUS_VRAM_A08n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.NAFA_DMA_A08n.qn07());
    /* p04.DUVE*/ vram_bus.BUS_VRAM_A09n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.PYNE_DMA_A09n.qn07());
    /* p04.ERAF*/ vram_bus.BUS_VRAM_A10n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.PARA_DMA_A10n.qn07());
    /* p04.FUSY*/ vram_bus.BUS_VRAM_A11n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.NYDO_DMA_A11n.qn07());
    /* p04.EXYF*/ vram_bus.BUS_VRAM_A12n.tri_6nn(_AHOC_DMA_VRAM_RDn, dma_reg.NYGY_DMA_A12n.qn07());
  }

  // Sprite fetcher read
  {
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp = not1(oam_bus.YZOS_OAM_DB6p.qp08());
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not1(pix_pipe.XYMO_LCDC_SPSIZEn.qn08());

    /*#p29.CYVU*/ wire _CYVU_L0 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L0.qp());
    /*#p29.BORE*/ wire _BORE_L1 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L1.qp());
    /*#p29.BUVY*/ wire _BUVY_L2 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L2.qp());
    /*#p29.WAGO*/ wire _WAGO_L3 = xor2(_WUKY_FLIP_Yp, sprite_store.SPR_TRI_L3.qp());
    /*#p29.GEJY*/ wire _GEJY_L3 = amux2(oam_bus.XUSO_OAM_DA0p.qp08(), _FUFO_LCDC_SPSIZEn,
                                        pix_pipe.XYMO_LCDC_SPSIZEn.qn08(), _WAGO_L3);

    /* p29.ABEM*/ vram_bus.BUS_VRAM_A00n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, sprite_fetcher.XUQU_SPRITE_AB);
    /* p29.BAXE*/ vram_bus.BUS_VRAM_A01n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, _CYVU_L0);
    /* p29.ARAS*/ vram_bus.BUS_VRAM_A02n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, _BORE_L1);
    /* p29.AGAG*/ vram_bus.BUS_VRAM_A03n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, _BUVY_L2);
    /* p29.FAMU*/ vram_bus.BUS_VRAM_A04n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, _GEJY_L3);
    /*#p29.FUGY*/ vram_bus.BUS_VRAM_A05n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, oam_bus.XEGU_OAM_DA1p.qp08());
    /* p29.GAVO*/ vram_bus.BUS_VRAM_A06n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, oam_bus.YJEX_OAM_DA2p.qp08());
    /* p29.WYGA*/ vram_bus.BUS_VRAM_A07n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, oam_bus.XYJU_OAM_DA3p.qp08());
    /* p29.WUNE*/ vram_bus.BUS_VRAM_A08n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, oam_bus.YBOG_OAM_DA4p.qp08());
    /* p29.GOTU*/ vram_bus.BUS_VRAM_A09n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, oam_bus.WYSO_OAM_DA5p.qp08());
    /* p29.GEGU*/ vram_bus.BUS_VRAM_A10n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, oam_bus.XOTE_OAM_DA6p.qp08());
    /* p29.XEHE*/ vram_bus.BUS_VRAM_A11n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, oam_bus.YZAB_OAM_DA7p.qp08());
    /* p29.DYSO*/ vram_bus.BUS_VRAM_A12n.tri_6nn(sprite_fetcher.ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
  }

  {
    // Map scroll adder
#pragma warning(push)
#pragma warning(disable:4189)
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0S = add_s(lcd_reg.MUWY_Y0p.qp17(), pix_pipe.GAVE_SCY0n.qn08(), 0);
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0C = add_c(lcd_reg.MUWY_Y0p.qp17(), pix_pipe.GAVE_SCY0n.qn08(), 0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1S = add_s(lcd_reg.MYRO_Y1p.qp17(), pix_pipe.FYMO_SCY1n.qn08(), _FAFO_TILE_Y0C);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1C = add_c(lcd_reg.MYRO_Y1p.qp17(), pix_pipe.FYMO_SCY1n.qn08(), _FAFO_TILE_Y0C);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2S = add_s(lcd_reg.LEXA_Y2p.qp17(), pix_pipe.FEZU_SCY2n.qn08(), _EMUX_TILE_Y1C);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2C = add_c(lcd_reg.LEXA_Y2p.qp17(), pix_pipe.FEZU_SCY2n.qn08(), _EMUX_TILE_Y1C);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0S  = add_s(lcd_reg.LYDO_Y3p.qp17(), pix_pipe.FUJO_SCY3n.qn08(), _ECAB_TILE_Y2C);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0C  = add_c(lcd_reg.LYDO_Y3p.qp17(), pix_pipe.FUJO_SCY3n.qn08(), _ECAB_TILE_Y2C);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1S  = add_s(lcd_reg.LOVU_Y4p.qp17(), pix_pipe.DEDE_SCY4n.qn08(), _ETAM_MAP_Y0C);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1C  = add_c(lcd_reg.LOVU_Y4p.qp17(), pix_pipe.DEDE_SCY4n.qn08(), _ETAM_MAP_Y0C);
    /* p26.DABA*/ wire _DABA_MAP_Y2S  = add_s(lcd_reg.LEMA_Y5p.qp17(), pix_pipe.FOTY_SCY5n.qn08(), _DOTO_MAP_Y1C);
    /* p26.DABA*/ wire _DABA_MAP_Y2C  = add_c(lcd_reg.LEMA_Y5p.qp17(), pix_pipe.FOTY_SCY5n.qn08(), _DOTO_MAP_Y1C);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3S  = add_s(lcd_reg.MATO_Y6p.qp17(), pix_pipe.FOHA_SCY6n.qn08(), _DABA_MAP_Y2C);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3C  = add_c(lcd_reg.MATO_Y6p.qp17(), pix_pipe.FOHA_SCY6n.qn08(), _DABA_MAP_Y2C);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4S  = add_s(lcd_reg.LAFO_Y7p.qp17(), pix_pipe.FUNY_SCY7n.qn08(), _EFYK_MAP_Y3C);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4C  = add_c(lcd_reg.LAFO_Y7p.qp17(), pix_pipe.FUNY_SCY7n.qn08(), _EFYK_MAP_Y3C);

    /*#p26.ATAD*/ wire _ATAD_TILE_X0S = add_s(pix_pipe.XEHO_X0p.qp17(), pix_pipe.DATY_SCX0n.qn08(), 0);
    /*#p26.ATAD*/ wire _ATAD_TILE_X0C = add_c(pix_pipe.XEHO_X0p.qp17(), pix_pipe.DATY_SCX0n.qn08(), 0);
    /* p26.BEHU*/ wire _BEHU_TILE_X1S = add_s(pix_pipe.SAVY_X1p.qp17(), pix_pipe.DUZU_SCX1n.qn08(), _ATAD_TILE_X0C);
    /* p26.BEHU*/ wire _BEHU_TILE_X1C = add_c(pix_pipe.SAVY_X1p.qp17(), pix_pipe.DUZU_SCX1n.qn08(), _ATAD_TILE_X0C);
    /* p26.APYH*/ wire _APYH_TILE_X2S = add_s(pix_pipe.XODU_X2p.qp17(), pix_pipe.CYXU_SCX2n.qn08(), _BEHU_TILE_X1C);
    /* p26.APYH*/ wire _APYH_TILE_X2C = add_c(pix_pipe.XODU_X2p.qp17(), pix_pipe.CYXU_SCX2n.qn08(), _BEHU_TILE_X1C);
    /* p26.BABE*/ wire _BABE_MAP_X0S  = add_s(pix_pipe.XYDO_X3p.qp17(), pix_pipe.GUBO_SCX3n.qn08(), _APYH_TILE_X2C);
    /* p26.BABE*/ wire _BABE_MAP_X0C  = add_c(pix_pipe.XYDO_X3p.qp17(), pix_pipe.GUBO_SCX3n.qn08(), _APYH_TILE_X2C);
    /* p26.ABOD*/ wire _ABOD_MAP_X1S  = add_s(pix_pipe.TUHU_X4p.qp17(), pix_pipe.BEMY_SCX4n.qn08(), _BABE_MAP_X0C);
    /* p26.ABOD*/ wire _ABOD_MAP_X1C  = add_c(pix_pipe.TUHU_X4p.qp17(), pix_pipe.BEMY_SCX4n.qn08(), _BABE_MAP_X0C);
    /* p26.BEWY*/ wire _BEWY_MAP_X2S  = add_s(pix_pipe.TUKY_X5p.qp17(), pix_pipe.CUZY_SCX5n.qn08(), _ABOD_MAP_X1C);
    /* p26.BEWY*/ wire _BEWY_MAP_X2C  = add_c(pix_pipe.TUKY_X5p.qp17(), pix_pipe.CUZY_SCX5n.qn08(), _ABOD_MAP_X1C);
    /* p26.BYCA*/ wire _BYCA_MAP_X3S  = add_s(pix_pipe.TAKO_X6p.qp17(), pix_pipe.CABU_SCX6n.qn08(), _BEWY_MAP_X2C);
    /* p26.BYCA*/ wire _BYCA_MAP_X3C  = add_c(pix_pipe.TAKO_X6p.qp17(), pix_pipe.CABU_SCX6n.qn08(), _BEWY_MAP_X2C);
    /* p26.ACUL*/ wire _ACUL_MAP_X4S  = add_s(pix_pipe.SYBE_X7p.qp17(), pix_pipe.BAKE_SCX7n.qn08(), _BYCA_MAP_X3C);
    /* p26.ACUL*/ wire _ACUL_MAP_X4C  = add_c(pix_pipe.SYBE_X7p.qp17(), pix_pipe.BAKE_SCX7n.qn08(), _BYCA_MAP_X3C);
#pragma warning(pop)

    // Background map read
    {
      /* p26.AXEP*/ vram_bus.BUS_VRAM_A00n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _BABE_MAP_X0S);
      /* p26.AFEB*/ vram_bus.BUS_VRAM_A01n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _ABOD_MAP_X1S);
      /* p26.ALEL*/ vram_bus.BUS_VRAM_A02n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _BEWY_MAP_X2S);
      /* p26.COLY*/ vram_bus.BUS_VRAM_A03n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _BYCA_MAP_X3S);
      /* p26.AJAN*/ vram_bus.BUS_VRAM_A04n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _ACUL_MAP_X4S);
      /* p26.DUHO*/ vram_bus.BUS_VRAM_A05n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _ETAM_MAP_Y0S);
      /* p26.CASE*/ vram_bus.BUS_VRAM_A06n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _DOTO_MAP_Y1S);
      /* p26.CYPO*/ vram_bus.BUS_VRAM_A07n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _DABA_MAP_Y2S);
      /* p26.CETA*/ vram_bus.BUS_VRAM_A08n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _EFYK_MAP_Y3S);
      /* p26.DAFE*/ vram_bus.BUS_VRAM_A09n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, _EJOK_MAP_Y4S);
      /*#p26.AMUV*/ vram_bus.BUS_VRAM_A10n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, pix_pipe.XAFO_LCDC_BGMAPn.qn08());
      /* p26.COVE*/ vram_bus.BUS_VRAM_A11n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, 1);
      /* p26.COXO*/ vram_bus.BUS_VRAM_A12n.tri_6nn(tile_fetcher.BAFY_BG_MAP_READn, 1);
    }

    // Window map read
    {
      /*#p27.XEJA*/ vram_bus.BUS_VRAM_A00n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.WYKA_WIN_X3.qp17());
      /* p27.XAMO*/ vram_bus.BUS_VRAM_A01n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.WODY_WIN_X4.qp17());
      /* p27.XAHE*/ vram_bus.BUS_VRAM_A02n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.WOBO_WIN_X5.qp17());
      /* p27.XULO*/ vram_bus.BUS_VRAM_A03n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.WYKO_WIN_X6.qp17());
      /* p27.WUJU*/ vram_bus.BUS_VRAM_A04n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.XOLO_WIN_X7.qp17());
      /*#p27.VYTO*/ vram_bus.BUS_VRAM_A05n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.TUFU_WIN_Y3.qp17());
      /* p27.VEHA*/ vram_bus.BUS_VRAM_A06n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.TAXA_WIN_Y4.qp17());
      /* p27.VACE*/ vram_bus.BUS_VRAM_A07n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.TOZO_WIN_Y5.qp17());
      /* p27.VOVO*/ vram_bus.BUS_VRAM_A08n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.TATE_WIN_Y6.qp17());
      /* p27.VULO*/ vram_bus.BUS_VRAM_A09n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.TEKE_WIN_Y7.qp17());
      /*#p27.VEVY*/ vram_bus.BUS_VRAM_A10n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, pix_pipe.WOKY_LCDC_WINMAPn.qn08());
      /* p27.VEZA*/ vram_bus.BUS_VRAM_A11n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, 1);
      /* p27.VOGU*/ vram_bus.BUS_VRAM_A12n.tri_6nn(tile_fetcher.WUKO_WIN_MAP_READn, 1);
    }

    // Background/window tile read
    {
      /*#p26.ASUM*/ vram_bus.BUS_VRAM_A00n.tri_6nn(tile_fetcher.BEJE_BG_TILE_READn,  tile_fetcher.XUHA_FETCH_S2p);
      /* p26.EVAD*/ vram_bus.BUS_VRAM_A01n.tri_6nn(tile_fetcher.BEJE_BG_TILE_READn,  _FAFO_TILE_Y0S);
      /* p26.DAHU*/ vram_bus.BUS_VRAM_A02n.tri_6nn(tile_fetcher.BEJE_BG_TILE_READn,  _EMUX_TILE_Y1S);
      /* p26.DODE*/ vram_bus.BUS_VRAM_A03n.tri_6nn(tile_fetcher.BEJE_BG_TILE_READn,  _ECAB_TILE_Y2S); // check outputs of ECAB

      /*#p25.XONU*/ vram_bus.BUS_VRAM_A00n.tri_6nn(tile_fetcher.XUCY_WIN_TILE_READn, tile_fetcher.XUHA_FETCH_S2p);
      /*#p25.WUDO*/ vram_bus.BUS_VRAM_A01n.tri_6nn(tile_fetcher.XUCY_WIN_TILE_READn, pix_pipe.VYNO_WIN_Y0.qp17());
      /*#p25.WAWE*/ vram_bus.BUS_VRAM_A02n.tri_6nn(tile_fetcher.XUCY_WIN_TILE_READn, pix_pipe.VUJO_WIN_Y1.qp17());
      /*#p25.WOLU*/ vram_bus.BUS_VRAM_A03n.tri_6nn(tile_fetcher.XUCY_WIN_TILE_READn, pix_pipe.VYMU_WIN_Y2.qp17());

      /*#p25.VUZA*/ wire _VUZA_TILE_BANKp = nor2(vram_bus.PYJU_TILE_DB7p.q11p(), pix_pipe.WEXU_LCDC_BGTILEn.qn08());

      /*#p25.VAPY*/ vram_bus.BUS_VRAM_A04n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.RAWU_TILE_DB0p.q11p());
      /*#p25.SEZU*/ vram_bus.BUS_VRAM_A05n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.POZO_TILE_DB1p.q11p());
      /*#p25.VEJY*/ vram_bus.BUS_VRAM_A06n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.PYZO_TILE_DB2p.q11p());
      /*#p25.RUSA*/ vram_bus.BUS_VRAM_A07n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.POXA_TILE_DB3p.q11p());
      /*#p25.ROHA*/ vram_bus.BUS_VRAM_A08n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.PULO_TILE_DB4p.q11p());
      /*#p25.RESO*/ vram_bus.BUS_VRAM_A09n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.POJU_TILE_DB5p.q11p());
      /*#p25.SUVO*/ vram_bus.BUS_VRAM_A10n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.POWY_TILE_DB6p.q11p());
      /*#p25.TOBO*/ vram_bus.BUS_VRAM_A11n.tri_6pn(tile_fetcher.NETA_TILE_READp,     vram_bus.PYJU_TILE_DB7p.q11p());
      /*#p25.VURY*/ vram_bus.BUS_VRAM_A12n.tri_6pn(tile_fetcher.NETA_TILE_READp,     _VUZA_TILE_BANKp);
    }
  }

  // VRAM addr bus -> VRAM addr pin
  {
    /*p25.MYFU*/ wire _MYFUp = not1(vram_bus.BUS_VRAM_A00n.qp());
    /*p25.MASA*/ wire _MASAp = not1(vram_bus.BUS_VRAM_A01n.qp());
    /*p25.MYRE*/ wire _MYREp = not1(vram_bus.BUS_VRAM_A02n.qp());
    /*p25.MAVU*/ wire _MAVUp = not1(vram_bus.BUS_VRAM_A03n.qp());
    /*p25.MEPA*/ wire _MEPAp = not1(vram_bus.BUS_VRAM_A04n.qp());
    /*p25.MYSA*/ wire _MYSAp = not1(vram_bus.BUS_VRAM_A05n.qp());
    /*p25.MEWY*/ wire _MEWYp = not1(vram_bus.BUS_VRAM_A06n.qp());
    /*p25.MUME*/ wire _MUMEp = not1(vram_bus.BUS_VRAM_A07n.qp());
    /*p25.VOVA*/ wire _VOVAp = not1(vram_bus.BUS_VRAM_A08n.qp());
    /*p25.VODE*/ wire _VODEp = not1(vram_bus.BUS_VRAM_A09n.qp());
    /*p25.RUKY*/ wire _RUKYp = not1(vram_bus.BUS_VRAM_A10n.qp());
    /*p25.RUMA*/ wire _RUMAp = not1(vram_bus.BUS_VRAM_A11n.qp());
    /*p25.REHO*/ wire _REHOp = not1(vram_bus.BUS_VRAM_A12n.qp());

    /*p25.LEXE*/ wire _LEXEn = not1(_MYFUp);
    /*p25.LOZU*/ wire _LOZUn = not1(_MASAp);
    /*p25.LACA*/ wire _LACAn = not1(_MYREp);
    /*p25.LUVO*/ wire _LUVOn = not1(_MAVUp);
    /*p25.LOLY*/ wire _LOLYn = not1(_MEPAp);
    /*p25.LALO*/ wire _LALOn = not1(_MYSAp);
    /*p25.LEFA*/ wire _LEFAn = not1(_MEWYp);
    /*p25.LUBY*/ wire _LUBYn = not1(_MUMEp);
    /*p25.TUJY*/ wire _TUJYn = not1(_VOVAp);
    /*p25.TAGO*/ wire _TAGOn = not1(_VODEp);
    /*p25.NUVA*/ wire _NUVAn = not1(_RUKYp);
    /*p25.PEDU*/ wire _PEDUn = not1(_RUMAp);
    /*p25.PONY*/ wire _PONYn = not1(_REHOp);

    vram_bus.PIN_VRAM_A00p.io_pin(_LEXEn, _LEXEn);
    vram_bus.PIN_VRAM_A01p.io_pin(_LOZUn, _LOZUn);
    vram_bus.PIN_VRAM_A02p.io_pin(_LACAn, _LACAn);
    vram_bus.PIN_VRAM_A03p.io_pin(_LUVOn, _LUVOn);
    vram_bus.PIN_VRAM_A04p.io_pin(_LOLYn, _LOLYn);
    vram_bus.PIN_VRAM_A05p.io_pin(_LALOn, _LALOn);
    vram_bus.PIN_VRAM_A06p.io_pin(_LEFAn, _LEFAn);
    vram_bus.PIN_VRAM_A07p.io_pin(_LUBYn, _LUBYn);
    vram_bus.PIN_VRAM_A08p.io_pin(_TUJYn, _TUJYn);
    vram_bus.PIN_VRAM_A09p.io_pin(_TAGOn, _TAGOn);
    vram_bus.PIN_VRAM_A10p.io_pin(_NUVAn, _NUVAn);
    vram_bus.PIN_VRAM_A11p.io_pin(_PEDUn, _PEDUn);
    vram_bus.PIN_VRAM_A12p.io_pin(_PONYn, _PONYn);
  }

  //----------------------------------------
  // VRAM data out

  // CBD->VBD->VPD
  {
    /*p25.ROCY*/ wire _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
    /*p25.RAHU*/ wire _RAHU_CBD_TO_VPDn = not1(_ROCY_CBD_TO_VPDp);

    /* p25.TEME*/ vram_bus.BUS_VRAM_D0p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D0p.qp());
    /* p25.TEWU*/ vram_bus.BUS_VRAM_D1p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D1p.qp());
    /*#p25.TYGO*/ vram_bus.BUS_VRAM_D2p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D2p.qp());
    /* p25.SOTE*/ vram_bus.BUS_VRAM_D3p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D3p.qp());
    /* p25.SEKE*/ vram_bus.BUS_VRAM_D4p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D4p.qp());
    /* p25.RUJO*/ vram_bus.BUS_VRAM_D5p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D5p.qp());
    /* p25.TOFA*/ vram_bus.BUS_VRAM_D6p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D6p.qp());
    /* p25.SUZA*/ vram_bus.BUS_VRAM_D7p.tri10_np(_RAHU_CBD_TO_VPDn, cpu_bus.BUS_CPU_D7p.qp());

    /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp = not1(_RAHU_CBD_TO_VPDn);
    /*#p25.SEFA*/ wire _SEFA_D0p = and2(vram_bus.BUS_VRAM_D0p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SOGO*/ wire _SOGO_D1p = and2(vram_bus.BUS_VRAM_D1p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SEFU*/ wire _SEFU_D2p = and2(vram_bus.BUS_VRAM_D2p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUNA*/ wire _SUNA_D3p = and2(vram_bus.BUS_VRAM_D3p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUMO*/ wire _SUMO_D4p = and2(vram_bus.BUS_VRAM_D4p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SAZU*/ wire _SAZU_D5p = and2(vram_bus.BUS_VRAM_D5p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SAMO*/ wire _SAMO_D6p = and2(vram_bus.BUS_VRAM_D6p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUKE*/ wire _SUKE_D7p = and2(vram_bus.BUS_VRAM_D7p.qp(), _ROVE_CBD_TO_VPDp);

    /*#p25.REGE*/ wire _REGE_D0n = not1(_SEFA_D0p);
    /* p25.RYKY*/ wire _RYKY_D1n = not1(_SOGO_D1p);
    /* p25.RAZO*/ wire _RAZO_D2n = not1(_SEFU_D2p);
    /* p25.RADA*/ wire _RADA_D3n = not1(_SUNA_D3p);
    /* p25.RYRO*/ wire _RYRO_D4n = not1(_SUMO_D4p);
    /* p25.REVU*/ wire _REVU_D5n = not1(_SAZU_D5p);
    /* p25.REKU*/ wire _REKU_D6n = not1(_SAMO_D6p);
    /* p25.RYZE*/ wire _RYZE_D7n = not1(_SUKE_D7p);

    /*#p25.SYNU*/ wire _SYNU_D0p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D0p.qp());
    /* p25.SYMA*/ wire _SYMA_D1p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D1p.qp());
    /* p25.ROKO*/ wire _ROKO_D2p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D2p.qp());
    /* p25.SYBU*/ wire _SYBU_D3p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D3p.qp());
    /* p25.SAKO*/ wire _SAKO_D4p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D4p.qp());
    /* p25.SEJY*/ wire _SEJY_D5p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D5p.qp());
    /* p25.SEDO*/ wire _SEDO_D6p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D6p.qp());
    /* p25.SAWU*/ wire _SAWU_D7p = or2(_RAHU_CBD_TO_VPDn, vram_bus.BUS_VRAM_D7p.qp());

    /*#p25.RURA*/ wire _RURA_D0n = not1(_SYNU_D0p);
    /* p25.RULY*/ wire _RULY_D1n = not1(_SYMA_D1p);
    /* p25.RARE*/ wire _RARE_D2n = not1(_ROKO_D2p);
    /* p25.RODU*/ wire _RODU_D3n = not1(_SYBU_D3p);
    /* p25.RUBE*/ wire _RUBE_D4n = not1(_SAKO_D4p);
    /* p25.RUMU*/ wire _RUMU_D5n = not1(_SEJY_D5p);
    /* p25.RYTY*/ wire _RYTY_D6n = not1(_SEDO_D6p);
    /* p25.RADY*/ wire _RADY_D7n = not1(_SAWU_D7p);

    /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp = not1(_RENA_CBD_TO_VPDn);

    /*#*/ vram_bus.PIN_VRAM_D00p.io_pin(_REGE_D0n, _RURA_D0n, _ROFA_CBD_TO_VPDp);
    /* */ vram_bus.PIN_VRAM_D01p.io_pin(_RYKY_D1n, _RULY_D1n, _ROFA_CBD_TO_VPDp);
    /* */ vram_bus.PIN_VRAM_D02p.io_pin(_RAZO_D2n, _RARE_D2n, _ROFA_CBD_TO_VPDp);
    /* */ vram_bus.PIN_VRAM_D03p.io_pin(_RADA_D3n, _RODU_D3n, _ROFA_CBD_TO_VPDp);
    /* */ vram_bus.PIN_VRAM_D04p.io_pin(_RYRO_D4n, _RUBE_D4n, _ROFA_CBD_TO_VPDp);
    /* */ vram_bus.PIN_VRAM_D05p.io_pin(_REVU_D5n, _RUMU_D5n, _ROFA_CBD_TO_VPDp);
    /* */ vram_bus.PIN_VRAM_D06p.io_pin(_REKU_D6n, _RYTY_D6n, _ROFA_CBD_TO_VPDp);
    /* */ vram_bus.PIN_VRAM_D07p.io_pin(_RYZE_D7n, _RADY_D7n, _ROFA_CBD_TO_VPDp);
  }

  //----------------------------------------
  // VRAM data in

  // VPD -> VBD
  {
    /*p25.RODY*/ vram_bus.BUS_VRAM_D0p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D00p.qn());
    /*p25.REBA*/ vram_bus.BUS_VRAM_D1p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D01p.qn());
    /*p25.RYDO*/ vram_bus.BUS_VRAM_D2p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D02p.qn());
    /*p25.REMO*/ vram_bus.BUS_VRAM_D3p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D03p.qn());
    /*p25.ROCE*/ vram_bus.BUS_VRAM_D4p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D04p.qn());
    /*p25.ROPU*/ vram_bus.BUS_VRAM_D5p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D05p.qn());
    /*p25.RETA*/ vram_bus.BUS_VRAM_D6p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D06p.qn());
    /*p25.RAKU*/ vram_bus.BUS_VRAM_D7p.tri_6pn(_RENA_CBD_TO_VPDn, vram_bus.PIN_VRAM_D07p.qn());
  }

  // VBD -> CBD
  {
    /* p25.TYVY*/ wire _TYVY_VBD_TO_CBDn = nand2(_SERE_CPU_VRM_RDp, LEKO_CPU_RDp);
    /* p25.SEBY*/ wire _SEBY_VBD_TO_CBDp = not1(_TYVY_VBD_TO_CBDn);

    /* p25.RERY*/ wire _RERY_VBUS_D0n = not1(vram_bus.BUS_VRAM_D0p.qp());
    /* p25.RUNA*/ wire _RUNA_VBUS_D1n = not1(vram_bus.BUS_VRAM_D1p.qp());
    /* p25.RONA*/ wire _RONA_VBUS_D2n = not1(vram_bus.BUS_VRAM_D2p.qp());
    /* p25.RUNO*/ wire _RUNO_VBUS_D3n = not1(vram_bus.BUS_VRAM_D3p.qp());
    /* p25.SANA*/ wire _SANA_VBUS_D4n = not1(vram_bus.BUS_VRAM_D4p.qp());
    /* p25.RORO*/ wire _RORO_VBUS_D5n = not1(vram_bus.BUS_VRAM_D5p.qp());
    /* p25.RABO*/ wire _RABO_VBUS_D6n = not1(vram_bus.BUS_VRAM_D6p.qp());
    /* p25.SAME*/ wire _SAME_VBUS_D7n = not1(vram_bus.BUS_VRAM_D7p.qp());

    /*#p25.RUGA*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_SEBY_VBD_TO_CBDp, _RERY_VBUS_D0n);
    /* p25.ROTA*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_SEBY_VBD_TO_CBDp, _RUNA_VBUS_D1n);
    /* p25.RYBU*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_SEBY_VBD_TO_CBDp, _RONA_VBUS_D2n);
    /* p25.RAJU*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_SEBY_VBD_TO_CBDp, _RUNO_VBUS_D3n);
    /* p25.TYJA*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_SEBY_VBD_TO_CBDp, _SANA_VBUS_D4n);
    /* p25.REXU*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_SEBY_VBD_TO_CBDp, _RORO_VBUS_D5n);
    /* p25.RUPY*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_SEBY_VBD_TO_CBDp, _RABO_VBUS_D6n);
    /* p25.TOKU*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_SEBY_VBD_TO_CBDp, _SAME_VBUS_D7n);
  }

  // VBD -> tile pix temp
  {
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(tile_fetcher.METE_LATCH_TILE_DAp);

    /*p32.LEGU*/ vram_bus.LEGU_TILE_DA0n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D0p.qp());
    /*p32.NUDU*/ vram_bus.NUDU_TILE_DA1n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D1p.qp());
    /*p32.MUKU*/ vram_bus.MUKU_TILE_DA2n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D2p.qp());
    /*p32.LUZO*/ vram_bus.LUZO_TILE_DA3n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D3p.qp());
    /*p32.MEGU*/ vram_bus.MEGU_TILE_DA4n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D4p.qp());
    /*p32.MYJY*/ vram_bus.MYJY_TILE_DA5n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D5p.qp());
    /*p32.NASA*/ vram_bus.NASA_TILE_DA6n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D6p.qp());
    /*p32.NEFO*/ vram_bus.NEFO_TILE_DA7n.dff8p(_LOMA_LATCH_TILE_DAn, vram_bus.BUS_VRAM_D7p.qp());

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*p??.LUVE*/ wire _LUVE_MATCH_TILE_DBp = not1(tile_fetcher.LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_MATCH_TILE_DBp);

    /*p32.RAWU*/ vram_bus.RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D0p.qp());
    /*p32.POZO*/ vram_bus.POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D1p.qp());
    /*p32.PYZO*/ vram_bus.PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D2p.qp());
    /*p32.POXA*/ vram_bus.POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D3p.qp());
    /*p32.PULO*/ vram_bus.PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D4p.qp());
    /*p32.POJU*/ vram_bus.POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D5p.qp());
    /*p32.POWY*/ vram_bus.POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D6p.qp());
    /*p32.PYJU*/ vram_bus.PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn, VYPO, vram_bus.BUS_VRAM_D7p.qp());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*#p29.XONO*/ wire _XONO_FLIP_X = and2(oam_bus.BAXO_OAM_DB5p.qp08(), sprite_fetcher.TEXY_SPR_READ_VRAMp);

    /*p33.PUTE*/ wire _PUTE_FLIP0p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D7p.qp(), vram_bus.BUS_VRAM_D0p.qp());
    /*p33.PELO*/ wire _PELO_FLIP1p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D6p.qp(), vram_bus.BUS_VRAM_D1p.qp());
    /*p33.PONO*/ wire _PONO_FLIP2p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D5p.qp(), vram_bus.BUS_VRAM_D2p.qp());
    /*p33.POBE*/ wire _POBE_FLIP3p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D4p.qp(), vram_bus.BUS_VRAM_D3p.qp());
    /*p33.PACY*/ wire _PACY_FLIP4p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D3p.qp(), vram_bus.BUS_VRAM_D4p.qp());
    /*p33.PUGU*/ wire _PUGU_FLIP5p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D2p.qp(), vram_bus.BUS_VRAM_D5p.qp());
    /*p33.PAWE*/ wire _PAWE_FLIP6p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D1p.qp(), vram_bus.BUS_VRAM_D6p.qp());
    /*p33.PULY*/ wire _PULY_FLIP7p = mux2p(_XONO_FLIP_X, vram_bus.BUS_VRAM_D0p.qp(), vram_bus.BUS_VRAM_D7p.qp());

    /*#p29.PEBY*/ wire _PEBY_CLKp = not1(sprite_fetcher.RACA_LATCH_SPPIXB);
    /*#p29.NYBE*/ wire _NYBE_CLKn = not1(_PEBY_CLKp);
    /*#p29.PUCO*/ wire _PUCO_CLKp = not1(_NYBE_CLKn);

    vram_bus.latch_sprite_a_delay.set(_PUCO_CLKp);

    /*p33.PEFO*/ vram_bus.PEFO_SPRITE_DB0n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _PUTE_FLIP0p);
    /*p33.ROKA*/ vram_bus.ROKA_SPRITE_DB1n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _PELO_FLIP1p);
    /*p33.MYTU*/ vram_bus.MYTU_SPRITE_DB2n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _PONO_FLIP2p);
    /*p33.RAMU*/ vram_bus.RAMU_SPRITE_DB3n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _POBE_FLIP3p);
    /*p33.SELE*/ vram_bus.SELE_SPRITE_DB4n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _PACY_FLIP4p);
    /*p33.SUTO*/ vram_bus.SUTO_SPRITE_DB5n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _PUGU_FLIP5p);
    /*p33.RAMA*/ vram_bus.RAMA_SPRITE_DB6n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _PAWE_FLIP6p);
    /*p33.RYDU*/ vram_bus.RYDU_SPRITE_DB7n.dff8n(vram_bus.latch_sprite_a_delay.q6(), _PULY_FLIP7p);

    /*#p29.VYWA*/ wire _VYWA_CLKp = not1(sprite_fetcher.TOPU_LATCH_SPPIXA);
    /*#p29.WENY*/ wire _WENY_CLKn = not1(_VYWA_CLKp);
    /*#p29.XADO*/ wire _XADO_CLKp = not1(_WENY_CLKn);

    vram_bus.latch_sprite_b_delay.set(_XADO_CLKp);

    /*p33.REWO*/ vram_bus.REWO_SPRITE_DA0n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _PUTE_FLIP0p);
    /*p33.PEBA*/ vram_bus.PEBA_SPRITE_DA1n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _PELO_FLIP1p);
    /*p33.MOFO*/ vram_bus.MOFO_SPRITE_DA2n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _PONO_FLIP2p);
    /*p33.PUDU*/ vram_bus.PUDU_SPRITE_DA3n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _POBE_FLIP3p);
    /*p33.SAJA*/ vram_bus.SAJA_SPRITE_DA4n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _PACY_FLIP4p);
    /*p33.SUNY*/ vram_bus.SUNY_SPRITE_DA5n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _PUGU_FLIP5p);
    /*p33.SEMO*/ vram_bus.SEMO_SPRITE_DA6n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _PAWE_FLIP6p);
    /*p33.SEGA*/ vram_bus.SEGA_SPRITE_DA7n.dff8n(vram_bus.latch_sprite_b_delay.q6(), _PULY_FLIP7p);
  }

  //------------------------------------------------------------------------------
  // Interrupts

  {
    // IE is technically in the CPU, but we're going to implement it here for now.
    wire FFFF_HIT = cpu_bus.get_bus_addr() == 0xFFFF;
    wire FFFF_WRn = nand2(TAPU_CPU_WRp_xxxxEFGx, FFFF_HIT);

    IE_D0.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D0p.qp());
    IE_D1.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D1p.qp());
    IE_D2.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D2p.qp());
    IE_D3.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D3p.qp());
    IE_D4.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D4p.qp());

    wire FFFF_RDn = nand2(TEDO_CPU_RDp, FFFF_HIT);

    cpu_bus.BUS_CPU_D0p.tri_6nn(FFFF_RDn, IE_D0.qn());
    cpu_bus.BUS_CPU_D1p.tri_6nn(FFFF_RDn, IE_D1.qn());
    cpu_bus.BUS_CPU_D2p.tri_6nn(FFFF_RDn, IE_D2.qn());
    cpu_bus.BUS_CPU_D3p.tri_6nn(FFFF_RDn, IE_D3.qn());
    cpu_bus.BUS_CPU_D4p.tri_6nn(FFFF_RDn, IE_D4.qn());
  }

  //------------------------------------------------------------------------------
  // LCD pixel pipe

  lcd_pix_lo.nor_latch(PIN_LCD_DATA0.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());
  lcd_pix_hi.nor_latch(PIN_LCD_DATA1.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());

  for (int i = 0; i < 159; i++) {
    lcd_pipe_lo[i].dff(PIN_LCD_CLOCK.qp(), lcd_pipe_lo[i + 1].qp());
    lcd_pipe_hi[i].dff(PIN_LCD_CLOCK.qp(), lcd_pipe_hi[i + 1].qp());
  }

  lcd_pipe_lo[159].dff(PIN_LCD_CLOCK.qp(), lcd_pix_lo.qp04());
  lcd_pipe_hi[159].dff(PIN_LCD_CLOCK.qp(), lcd_pix_hi.qp04());
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_ext_bus(wire RST, const uint8_t* cart_rom, uint8_t* cart_ram, uint8_t* ext_ram) {
  if (RST) return;

  uint16_t ext_addr = ext_bus.get_pin_addr();

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = ext_bus.PIN_EXT_RDn.qp();
    wire CEn = ext_bus.PIN_EXT_A15p.qp();

    if (!CEn && !OEn) {
      if (cart_rom) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
      else {
        printf("no cart loaded!\n");
      }
    }
  }

  // Ext RAM read/write (also echo RAM)
  {
    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = ext_bus.PIN_EXT_WRn.qp();
    wire CE1n = ext_bus.PIN_EXT_CSn.qp();
    wire CE2  = ext_bus.PIN_EXT_A14p.qp();
    wire OEn  = ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CE1n && CE2 && !WRn) {
      ext_ram[ram_addr] = ext_bus.get_pin_data();
    }

    // Read
    if (!CE1n && CE2 && WRn && !OEn) {
      ext_bus.set_pin_data(ext_ram[ram_addr]);
    }
  }

  // Cart RAM read/write
  {
    // A000-BFFF
    // 0b101xxxxxxxxxxxxx

    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = ext_bus.PIN_EXT_WRn.qp();
    wire CS1n = ext_bus.PIN_EXT_CSn.qp();
    wire CS2  = ext_bus.PIN_EXT_A13p.qp() && !ext_bus.PIN_EXT_A14p.qp() && ext_bus.PIN_EXT_A15p.qp();
    wire OEn = ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CS1n && CS2 && !WRn) {
      cart_ram[ram_addr] = ext_bus.get_pin_data();
    }

    // Read
    if (!CS1n && CS2 && !OEn) {
      ext_bus.set_pin_data(cart_ram[ram_addr]);
    }
  }

  // FIXME - implement MBC1

  // 0000-3FFF - ROM Bank 00 (Read Only) This area always contains the first 16KBytes of the cartridge ROM.
  // 4000-7FFF - ROM Bank 01-7F (Read Only) This area may contain any of the further 16KByte banks of the ROM, allowing to address up to 125 ROM Banks (almost 2MByte). As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
  // A000-BFFF - RAM Bank 00-03, if any (Read/Write) This area is used to address external RAM in the cartridge (if any). External RAM is often battery buffered, allowing to store game positions or high score tables, even if the gameboy is turned off, or if the cartridge is removed from the gameboy. Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).

  // 0000-1FFF - RAM Enable (Write Only)   00h  Disable RAM (default)   ?Ah  Enable RAM Practically any value with 0Ah in the lower 4 bits enables RAM, and any other value disables RAM.
  // 2000-3FFF - ROM Bank Number (Write Only) Writing to this address space selects the lower 5 bits of the ROM Bank Number (in range 01-1Fh). When 00h is written, the MBC translates that to bank 01h also. That doesn't harm so far, because ROM Bank 00h can be always directly accessed by reading from 0000-3FFF.
  // But (when using the register below to specify the upper ROM Bank bits), the same happens for Bank 20h, 40h, and 60h. Any attempt to address these ROM Banks will select Bank 21h, 41h, and 61h instead.
  // 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only) This 2bit register can be used to select a RAM Bank in range from 00-03h, or to specify the upper two bits (Bit 5-6) of the ROM Bank number, depending on the current ROM/RAM Mode. (See below.)
  // 6000-7FFF - ROM/RAM Mode Select (Write Only)  00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)   01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)

  // MBC1_RAM_EN

  // MBC1_BANK_D0
  // MBC1_BANK_D1
  // MBC1_BANK_D2
  // MBC1_BANK_D3
  // MBC1_BANK_D4
  // MBC1_BANK_D5
  // MBC1_BANK_D6

  // MBC1_MODE

  /*
  {

    bool bank_0 = nor(MBC1_BANK_D0, MBC1_BANK_D1, MBC1_BANK_D2, MBC1_BANK_D3, MBC1_BANK_D4);

    wire cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp();
    wire cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp();
    wire cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp();
    wire cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp();
    wire cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp();
    wire cart_rom_a19 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp();
    wire cart_rom_a20 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp();

    wire cart_ram_a13 = MBC1_MODE.qp() ? MBC1_BANK_D5.qp() : 0;
    wire cart_ram_a14 = MBC1_MODE.qp() ? MBC1_BANK_D6.qp() : 0;

    // ROM read
    {
      uint16_t rom_addr = ext_addr & 0x7FFF;
      wire OEn = ext_bus.PIN_EXT_RDn.qp();
      wire CEn = ext_bus.PIN_EXT_A15p.qp();

      if (!CEn && !OEn) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
    }
  }
  */
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_vram_bus(wire RST, uint8_t* vid_ram) {
  if (RST) return;

  int vram_addr = vram_bus.get_pin_addr();
  uint8_t& vram_data = vid_ram[vram_addr];

  // We're getting a fake write on the first phase because PIN_VRAM_WRn resets to 0...
  // ignore it if we're in reset

  if (!RST) {
    if (!vram_bus.PIN_VRAM_WRn.qp()) {
      vram_data = (uint8_t)vram_bus.get_pin_data();
    }

    if (!vram_bus.PIN_VRAM_OEn.qp()) {
      vram_bus.set_pin_data_in(vram_data);
    }
    else {
      vram_bus.set_pin_data_z();
    }
  }
}

//-----------------------------------------------------------------------------
// FIXME This should be using PIN_OAM_CLK (which might actually be PIN_OAM_CSn?)

void SchematicTop::tock_oam_bus(wire RST, uint8_t* oam_ram) {
  if (RST) return;

  uint16_t oam_addr = oam_bus.get_oam_pin_addr();
  uint8_t& oam_data_a = oam_ram[(oam_addr << 1) + 0];
  uint8_t& oam_data_b = oam_ram[(oam_addr << 1) + 1];

  if (!oam_bus.PIN_OAM_WR_A.qp()) oam_data_a = oam_bus.get_oam_pin_data_a();
  if (!oam_bus.PIN_OAM_WR_B.qp()) oam_data_b = oam_bus.get_oam_pin_data_b();

  if (!oam_bus.PIN_OAM_OE.qp()) oam_bus.set_pin_data_a(oam_data_a);
  if (!oam_bus.PIN_OAM_OE.qp()) oam_bus.set_pin_data_b(oam_data_b);
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_zram_bus(wire RST, uint8_t* zero_ram) {
  if (RST) return;

  // ZRAM control signals are

  // clk_reg.PIN_CPU_BUKE_AxxxxxGH
  // TEDO_CPU_RDp();
  // TAPU_CPU_WRp_xxxxEFGx()
  // cpu_bus.SYKE_FF00_FFFFp()

  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  int addr = cpu_bus.get_bus_addr();
  bool hit_zram = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (hit_zram) {
    uint8_t& data = zero_ram[addr & 0x007F];
    if (TAPU_CPU_WRp_xxxxEFGx) data = cpu_bus.get_bus_data();
    if (TEDO_CPU_RDp) cpu_bus.set_data(data);
  }
}

//------------------------------------------------------------------------------

#if 0
// Debug stuff I disabled

/*p07.APET*/ wire APET_MODE_DBG = or2(clk_reg.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A05, BUS_CPU_A06, TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/*p05.AXYN*/ wire AXYN_xBCDEFGH = not1(clk_reg.BEDO_Axxxxxxx);
/*p05.ADYR*/ wire ADYR_Axxxxxxx = not1(AXYN_xBCDEFGH);
/*p05.APYS*/ wire APYS_xBCDEFGH = nor4(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/*p05.AFOP*/ wire AFOP_Axxxxxxx = not1(APYS_xBCDEFGH);
/*p07.LECO*/ wire LECO_xBCDEFGH = nor4(clk_reg.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

if (AFOP_Axxxxxxx) set_data(
  /*p05.ANOC*/ not1(GND),
  /*p05.ATAJ*/ not1(GND),
  /*p05.AJEC*/ not1(GND),
  /*p05.ASUZ*/ not1(GND),
  /*p05.BENU*/ not1(GND),
  /*p05.AKAJ*/ not1(GND),
  /*p05.ARAR*/ not1(GND),
  /*p05.BEDA*/ not1(GND)
);

if (LECO_xBCDEFGH) set_data(
  /*p07.ROMY*/ GND,
  /*p07.RYNE*/ GND,
  /*p07.REJY*/ GND,
  /*p07.RASE*/ GND,
  /*p07.REKA*/ GND,
  /*p07.ROWE*/ GND,
  /*p07.RYKE*/ GND,
  /*p07.RARU*/ GND
);



//----------
// more debug stuff

/*p25.TUSO*/ wire TUSO = nor4(MODE_DBG2, dff20.PIN_CPU_BOGA_xBCDEFGH);
/*p25.SOLE*/ wire SOLE = not1(TUSO);

if (VYPO_GND) bus_out.set_data(
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
/*p08.TUTY*/ if (DBG_D_RDn) BUS_CPU_D0p = not1(/*p08.TOVO*/ not1(pins.PIN_D0_C));
/*p08.SYWA*/ if (DBG_D_RDn) BUS_CPU_D1p = not1(/*p08.RUZY*/ not1(pins.PIN_D1_C));
/*p08.SUGU*/ if (DBG_D_RDn) BUS_CPU_D2p = not1(/*p08.ROME*/ not1(pins.PIN_D2_C));
/*p08.TAWO*/ if (DBG_D_RDn) BUS_CPU_D3p = not1(/*p08.SAZA*/ not1(pins.PIN_D3_C));
/*p08.TUTE*/ if (DBG_D_RDn) BUS_CPU_D4p = not1(/*p08.TEHE*/ not1(pins.PIN_D4_C));
/*p08.SAJO*/ if (DBG_D_RDn) BUS_CPU_D5p = not1(/*p08.RATU*/ not1(pins.PIN_D5_C));
/*p08.TEMY*/ if (DBG_D_RDn) BUS_CPU_D6p = not1(/*p08.SOCA*/ not1(pins.PIN_D6_C));
/*p08.ROPA*/ if (DBG_D_RDn) BUS_CPU_D7p = not1(/*p08.RYBA*/ not1(pins.PIN_D7_C));
#endif

// hack, not correct
#if 0
{
  // FF60 debug state
  /*p07.APET*/ wire APET_MODE_DBG = or2(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A05, BUS_CPU_A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not1(BURO_FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not1(BURO_FF60_0);
  /*p07.BURO*/ BURO_FF60_0 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D0p);
  /*p07.AMUT*/ AMUT_FF60_1 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D1p);

  ///*p05.KURA*/ wire FF60_0n = not1(FF60);
  ///*p05.JEVA*/ wire FF60_0o = not1(FF60);
}
#endif


#if 0
// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
  // If we're in debug mode 2, drive external address bus onto internal address

  /*#p08.KOVA*/ wire KOVA_A00p = not1(PIN_EXT_A00p.qn());
  /* p08.CAMU*/ wire CAMU_A01p = not1(PIN_EXT_A01p.qn());
  /* p08.BUXU*/ wire BUXU_A02p = not1(PIN_EXT_A02p.qn());
  /* p08.BASE*/ wire BASE_A03p = not1(PIN_EXT_A03p.qn());
  /* p08.AFEC*/ wire AFEC_A04p = not1(PIN_EXT_A04p.qn());
  /* p08.ABUP*/ wire ABUP_A05p = not1(PIN_EXT_A05p.qn());
  /* p08.CYGU*/ wire CYGU_A06p = not1(PIN_EXT_A06p.qn());
  /* p08.COGO*/ wire COGO_A07p = not1(PIN_EXT_A07p.qn());
  /* p08.MUJY*/ wire MUJY_A08p = not1(PIN_EXT_A08p.qn());
  /* p08.NENA*/ wire NENA_A09p = not1(PIN_EXT_A09p.qn());
  /* p08.SURA*/ wire SURA_A10p = not1(PIN_EXT_A10p.qn());
  /* p08.MADY*/ wire MADY_A11p = not1(PIN_EXT_A11p.qn());
  /* p08.LAHE*/ wire LAHE_A12p = not1(PIN_EXT_A12p.qn());
  /* p08.LURA*/ wire LURA_A13p = not1(PIN_EXT_A13p.qn());
  /* p08.PEVO*/ wire PEVO_A14p = not1(PIN_EXT_A14p.qn());
  /* p08.RAZA*/ wire RAZA_A15p = not1(PIN_EXT_A15p.qn());

  // KEJO_01 << KOVA_02
  // KEJO_02
  // KEJO_03
  // KEJO_04 << TOVA_02
  // KEJO_05 << KOVA_02
  // KEJO_06
  // KEJO_07
  // KEJO_08
  // KEJO_09 >> BUS_CPU_A00p
  // KEJO_10

  /* p08.KEJO*/ cpu_bus.BUS_CPU_A00 = tribuf_10np(TOVA_MODE_DBG2n, KOVA_A00p);
  /* p08.BYXE*/ cpu_bus.BUS_CPU_A01 = tribuf_10np(TOVA_MODE_DBG2n, CAMU_A01p);
  /* p08.AKAN*/ cpu_bus.BUS_CPU_A02 = tribuf_10np(TOVA_MODE_DBG2n, BUXU_A02p);
  /* p08.ANAR*/ cpu_bus.BUS_CPU_A03 = tribuf_10np(TOVA_MODE_DBG2n, BASE_A03p);
  /* p08.AZUV*/ cpu_bus.BUS_CPU_A04 = tribuf_10np(TOVA_MODE_DBG2n, AFEC_A04p);
  /* p08.AJOV*/ cpu_bus.BUS_CPU_A05 = tribuf_10np(TOVA_MODE_DBG2n, ABUP_A05p);
  /* p08.BYNE*/ cpu_bus.BUS_CPU_A06 = tribuf_10np(TOVA_MODE_DBG2n, CYGU_A06p);
  /* p08.BYNA*/ cpu_bus.BUS_CPU_A07 = tribuf_10np(TOVA_MODE_DBG2n, COGO_A07p);
  /* p08.LOFA*/ cpu_bus.BUS_CPU_A08 = tribuf_10np(TOVA_MODE_DBG2n, MUJY_A08p);
  /* p08.MAPU*/ cpu_bus.BUS_CPU_A09 = tribuf_10np(TOVA_MODE_DBG2n, NENA_A09p);
  /* p08.RALA*/ cpu_bus.BUS_CPU_A10 = tribuf_10np(TOVA_MODE_DBG2n, SURA_A10p);
  /* p08.LORA*/ cpu_bus.BUS_CPU_A11 = tribuf_10np(TOVA_MODE_DBG2n, MADY_A11p);
  /* p08.LYNA*/ cpu_bus.BUS_CPU_A12 = tribuf_10np(TOVA_MODE_DBG2n, LAHE_A12p);
  /* p08.LEFY*/ cpu_bus.BUS_CPU_A13 = tribuf_10np(TOVA_MODE_DBG2n, LURA_A13p);
  /* p08.NEFE*/ cpu_bus.BUS_CPU_A14 = tribuf_10np(TOVA_MODE_DBG2n, PEVO_A14p);
  /* p08.SYZU*/ cpu_bus.BUS_CPU_A15 = tribuf_10np(TOVA_MODE_DBG2n, RAZA_A15p);
}
#endif

//-----------------------------------------------------------------------------
