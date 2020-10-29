#include "GateBoyLib/Sch_Top.h"
#include <memory.h>
#include <stdio.h>

#include "GateBoyLib/Probe.h"

using namespace Schematics;

#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void SchematicTop::tick_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  auto& top = *this;
  wire GND = 0;

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

  /*#p01.AVOR*/ wire _AVOR_SYS_RSTp = or2(top.clk_reg.AFER_SYS_RSTp.qp13(), top.clk_reg.ASOL_POR_DONEn.qp04());
  /*#p01.ALUR*/ top.clk_reg.ALUR_SYS_RSTn = not1(_AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
  /*#p01.CUNU*/ top.clk_reg.CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
  /*#p01.XORE*/ top.clk_reg.XORE_SYS_RSTp = not1(top.clk_reg.CUNU_SYS_RSTn);
  /* p01.WESY*/ top.clk_reg.WESY_SYS_RSTn = not1(top.clk_reg.XORE_SYS_RSTp);

  /* p01.XEBE*/ wire _XEBE_SYS_RSTn = not1(top.clk_reg.XORE_SYS_RSTp);
  /* p01.XODO*/ wire _XODO_VID_RSTp = nand2(_XEBE_SYS_RSTn, top.pix_pipe.XONA_LCDC_LCDENn.qn08());
  /* p01.XAPO*/ top.clk_reg.XAPO_VID_RSTn = not1(_XODO_VID_RSTp);
  /*#p01.ATAR*/ top.clk_reg.ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /*#p01.ABEZ*/ top.clk_reg.ABEZ_VID_RSTn = not1(top.clk_reg.ATAR_VID_RSTp);


  /* p01.ATEZ*/ wire _ATEZ_CLKBAD = not1(CLKGOOD);
  /* p01.UCOB*/ top.clk_reg.UCOB_CLKBADp = not1(CLKGOOD);
  /* p01.ABOL*/ wire _ABOL_CLKREQn = not1(CPUREADY);
  /*#p01.BUTY*/ wire _BUTY_CLKREQ = not1(_ABOL_CLKREQn);

  // ignoring the deglitcher here
  /*p01.ATAL*/ top.clk_reg.ATAL_xBxDxFxH = CLK;
  /*p01.AZOF*/ wire _AZOF_AxCxExGx = not1(top.clk_reg.ATAL_xBxDxFxH);
  /*p01.ZAXY*/ top.clk_reg.ZAXY_xBxDxFxH = not1(_AZOF_AxCxExGx);

  /*#p01.AROV*/ top.clk_reg.AROV_xxCDEFxx = not1(top.clk_reg.APUK_ABxxxxGH.qp09());
  /*#p01.AFEP*/ wire _AFEP_AxxxxFGH = not1(top.clk_reg.ALEF_AxxxxFGH.qn08());
  /*#p01.ATYP*/ top.clk_reg.ATYP_ABCDxxxx = not1(top.clk_reg.AFUR_xxxxEFGH.qp09());
  /*#p01.AJAX*/ top.clk_reg.AJAX_xxxxEFGH = not1(top.clk_reg.ATYP_ABCDxxxx.qp());

  /*#p01.BAPY*/ wire _BAPY_xxxxxxGH = nor3(_ABOL_CLKREQn, top.clk_reg.AROV_xxCDEFxx, top.clk_reg.ATYP_ABCDxxxx);

  /*#p01.BELU*/ wire _BELU_xxxxEFGH = nor2(top.clk_reg.ATYP_ABCDxxxx, _ABOL_CLKREQn);
  /*#p01.BYRY*/ wire _BYRY_ABCDxxxx = not1(_BELU_xxxxEFGH);
  /*#p01.BUDE*/ top.clk_reg.BUDE_xxxxEFGH = not1(_BYRY_ABCDxxxx);

  /*#p01.BERU*/ wire _BERU_ABCDEFxx = not1(_BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire _BUFA_xxxxxxGH = not1(_BERU_ABCDEFxx);
  /*#p01.BOLO*/ top.clk_reg.BOLO_ABCDEFxx = not1(_BUFA_xxxxxxGH);

  /*#p01.BEKO*/ top.clk_reg.BEKO_ABCDxxxx = not1(top.clk_reg.BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire _BEJA_xxxxEFGH = nand4(top.clk_reg.BOLO_ABCDEFxx, top.clk_reg.BOLO_ABCDEFxx, top.clk_reg.BEKO_ABCDxxxx, top.clk_reg.BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire _BANE_ABCDxxxx = not1(_BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire _BELO_xxxxEFGH = not1(_BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire _BAZE_ABCDxxxx = not1(_BELO_xxxxEFGH);

  /*#p01.BUTO*/ wire _BUTO_xBCDEFGH = nand3(_AFEP_AxxxxFGH, top.clk_reg.ATYP_ABCDxxxx, _BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire _BELE_Axxxxxxx = not1(_BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire _BYJU_Axxxxxxx = or2(_BELE_Axxxxxxx, _ATEZ_CLKBAD);

  /*#p01.BALY*/ top.clk_reg.BALY_xBCDEFGH = not1(_BYJU_Axxxxxxx);
  /* p01.BOGA*/ top.clk_reg.BOGA_Axxxxxxx = not1(top.clk_reg.BALY_xBCDEFGH);

  /*#p01.ADAR*/ top.clk_reg.ADAR_ABCxxxxH = not1(top.clk_reg.ADYK_ABCxxxxH.qn08());
  /*#p29.XUPY*/ top.clk_reg.XUPY_ABxxEFxx = not1(top.clk_reg.WUVU_ABxxEFxx.qn16());
  /*#p21.TALU*/ top.clk_reg.TALU_xxCDEFxx = not1(top.clk_reg.VENA_xxCDEFxx.qn16());
  /*#p29.XOCE*/ top.clk_reg.XOCE_xBCxxFGx = not1(top.clk_reg.WOSU_AxxDExxH.qp17());
  /*#p29.WOJO*/ top.clk_reg.WOJO_AxxxExxx = nor2(top.clk_reg.WOSU_AxxDExxH.qn16(), top.clk_reg.WUVU_ABxxEFxx.qn16());
  /*#p01.AFAS*/ top.clk_reg.AFAS_xxxxEFGx = nor2(top.clk_reg.ADAR_ABCxxxxH, top.clk_reg.ATYP_ABCDxxxx);

  /*#p01.ZEME*/ top.clk_reg.ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /* p01.UVYT*/ top.clk_reg.UVYT_ABCDxxxx = not1(top.clk_reg.BUDE_xxxxEFGH);
  /* p04.MOPA*/ top.clk_reg.MOPA_xxxxEFGH = not1(top.clk_reg.UVYT_ABCDxxxx);
  /*#p01.ALET*/ top.clk_reg.ALET_xBxDxFxH = not1(top.clk_reg.ZEME_AxCxExGx);
  /*#p27.MYVO*/ top.clk_reg.MYVO_AxCxExGx = not1(top.clk_reg.ALET_xBxDxFxH);

  /*#p01.BUGO*/ wire _BUGO_xBCDExxx = not1(_AFEP_AxxxxFGH);
  /*#p01.BATE*/ wire _BATE_AxxxxxGH = nor3(_BUGO_xBCDExxx, top.clk_reg.AROV_xxCDEFxx, _ABOL_CLKREQn);
  /*#p01.BASU*/ wire _BASU_xBCDEFxx = not1(_BATE_AxxxxxGH);

  /*#p01.BUKE*/ top.clk_reg.BUKE_AxxxxxGH = not1(_BASU_xBCDEFxx);
  /*#p01.BOMA*/ top.clk_reg.BOMA_xBCDEFGH = not1(top.clk_reg.BOGA_Axxxxxxx);

  /*#p01.BUVU*/ wire _BUVU_Axxxxxxx = and2(top.clk_reg.BALY_xBCDEFGH, _BUTY_CLKREQ);
  /*#p01.BYXO*/ wire _BYXO_xBCDEFGH = not1(_BUVU_Axxxxxxx);
  /*#p01.BEDO*/ top.clk_reg.BEDO_Axxxxxxx = not1(_BYXO_xBCDEFGH);
  /*#p01.BOWA*/ top.clk_reg.BOWA_xBCDEFGH = not1(top.clk_reg.BEDO_Axxxxxxx);


  //------------------------------------------------------------------------------
  // lcd_reg.tick(top);

  // fires on P910 and P911
  /* p28.ABAF*/ wire _ABAF_LINE_P910n = not1(top.lcd_reg.CATU_LINE_P000.qp17());

  // so if this is or_and, BYHA should go low on 910 and 911
  /* p28.BYHA*/ top.lcd_reg.BYHA_VID_LINE_END_TRIGn = or_and3(top.lcd_reg.ANEL_LINE_P002.qp17(), _ABAF_LINE_P910n, top.clk_reg.ABEZ_VID_RSTn);

  // fires on P910 and P911
  /* p28.ATEJ*/ top.lcd_reg.ATEJ_LINE_TRIGp = not1(top.lcd_reg.BYHA_VID_LINE_END_TRIGn);

  // -> interrupts, ppu
  /*#p21.PARU*/ top.lcd_reg.PARU_VBLANKp_d4 = not1(top.lcd_reg.POPU_IN_VBLANKp.qn16());

  /*#p21.TOLU*/ top.lcd_reg.TOLU_VBLANKn = not1(top.lcd_reg.PARU_VBLANKp_d4);
  /*#p21.VYPU*/ top.lcd_reg.VYPU_INT_VBLANKp = not1(top.lcd_reg.TOLU_VBLANKn);

  /*#p21.PURE*/ top.lcd_reg.PURE_LINE_P908n = not1(top.lcd_reg.RUTU_LINE_P910.qp17());
  /*#p21.SELA*/ top.lcd_reg.SELA_LINE_P908p = not1(top.lcd_reg.PURE_LINE_P908n);

  //------------------------------------------------------------------------------
  // sprite_scanner.tick()

  // this is using an adder as a subtracter by inverting the first input.

  /*#p29.EBOS*/ wire _EBOS_Y0n = not1(top.lcd_reg.MUWY_Y0p.qp17());
  /* p29.DASA*/ wire _DASA_Y1n = not1(top.lcd_reg.MYRO_Y1p.qp17());
  /* p29.FUKY*/ wire _FUKY_Y2n = not1(top.lcd_reg.LEXA_Y2p.qp17());
  /* p29.FUVE*/ wire _FUVE_Y3n = not1(top.lcd_reg.LYDO_Y3p.qp17());
  /* p29.FEPU*/ wire _FEPU_Y4n = not1(top.lcd_reg.LOVU_Y4p.qp17());
  /* p29.FOFA*/ wire _FOFA_Y5n = not1(top.lcd_reg.LEMA_Y5p.qp17());
  /* p29.FEMO*/ wire _FEMO_Y6n = not1(top.lcd_reg.MATO_Y6p.qp17());
  /* p29.GUSU*/ wire _GUSU_Y7n = not1(top.lcd_reg.LAFO_Y7p.qp17());

  /*p29.ERUC*/ wire _ERUC_YDIFF_S0 = add_s(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0p.qp08(), GND);
  /*p29.ERUC*/ wire _ERUC_YDIFF_C0 = add_c(_EBOS_Y0n, top.oam_bus.XUSO_OAM_DA0p.qp08(), GND);
  /*p29.ENEF*/ wire _ENEF_YDIFF_S1 = add_s(_DASA_Y1n, top.oam_bus.XEGU_OAM_DA1p.qp08(), _ERUC_YDIFF_C0);
  /*p29.ENEF*/ wire _ENEF_YDIFF_C1 = add_c(_DASA_Y1n, top.oam_bus.XEGU_OAM_DA1p.qp08(), _ERUC_YDIFF_C0);
  /*p29.FECO*/ wire _FECO_YDIFF_S2 = add_s(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2p.qp08(), _ENEF_YDIFF_C1);
  /*p29.FECO*/ wire _FECO_YDIFF_C2 = add_c(_FUKY_Y2n, top.oam_bus.YJEX_OAM_DA2p.qp08(), _ENEF_YDIFF_C1);
  /*p29.GYKY*/ wire _GYKY_YDIFF_S3 = add_s(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3p.qp08(), _FECO_YDIFF_C2);
  /*p29.GYKY*/ wire _GYKY_YDIFF_C3 = add_c(_FUVE_Y3n, top.oam_bus.XYJU_OAM_DA3p.qp08(), _FECO_YDIFF_C2);
  /*p29.GOPU*/ wire _GOPU_YDIFF_S4 = add_s(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4p.qp08(), _GYKY_YDIFF_C3);
  /*p29.GOPU*/ wire _GOPU_YDIFF_C4 = add_c(_FEPU_Y4n, top.oam_bus.YBOG_OAM_DA4p.qp08(), _GYKY_YDIFF_C3);
  /*p29.FUWA*/ wire _FUWA_YDIFF_S5 = add_s(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5p.qp08(), _GOPU_YDIFF_C4);
  /*p29.FUWA*/ wire _FUWA_YDIFF_C5 = add_c(_FOFA_Y5n, top.oam_bus.WYSO_OAM_DA5p.qp08(), _GOPU_YDIFF_C4);
  /*p29.GOJU*/ wire _GOJU_YDIFF_S6 = add_s(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6p.qp08(), _FUWA_YDIFF_C5);
  /*p29.GOJU*/ wire _GOJU_YDIFF_C6 = add_c(_FEMO_Y6n, top.oam_bus.XOTE_OAM_DA6p.qp08(), _FUWA_YDIFF_C5);
  /*p29.WUHU*/ wire _WUHU_YDIFF_S7 = add_s(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7p.qp08(), _GOJU_YDIFF_C6);
  /*p29.WUHU*/ wire _WUHU_YDIFF_C7 = add_c(_GUSU_Y7n, top.oam_bus.YZAB_OAM_DA7p.qp08(), _GOJU_YDIFF_C6);

  /* p29.DEGE*/ top.sprite_scanner.DEGE_SPRITE_DELTA0 = not1(_ERUC_YDIFF_S0);
  /* p29.DABY*/ top.sprite_scanner.DABY_SPRITE_DELTA1 = not1(_ENEF_YDIFF_S1);
  /* p29.DABU*/ top.sprite_scanner.DABU_SPRITE_DELTA2 = not1(_FECO_YDIFF_S2);
  /* p29.GYSA*/ top.sprite_scanner.GYSA_SPRITE_DELTA3 = not1(_GYKY_YDIFF_S3);
  /* p29.GACE*/ top.sprite_scanner.GACE_SPRITE_DELTA4 = not1(_GOPU_YDIFF_S4);
  /* p29.GUVU*/ top.sprite_scanner.GUVU_SPRITE_DELTA5 = not1(_FUWA_YDIFF_S5);
  /* p29.GYDA*/ top.sprite_scanner.GYDA_SPRITE_DELTA6 = not1(_GOJU_YDIFF_S6);
  /* p29.GEWY*/ top.sprite_scanner.GEWY_SPRITE_DELTA7 = not1(_WUHU_YDIFF_S7);

  /*#p29.GOVU*/ wire _GOVU_SPSIZE_MATCH  = or2(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn08(), _GYKY_YDIFF_S3);
  /* p29.WOTA*/ wire _WOTA_SCAN_MATCH_Yn = nand6(top.sprite_scanner.GACE_SPRITE_DELTA4, top.sprite_scanner.GUVU_SPRITE_DELTA5, top.sprite_scanner.GYDA_SPRITE_DELTA6, top.sprite_scanner.GEWY_SPRITE_DELTA7, _WUHU_YDIFF_C7, _GOVU_SPSIZE_MATCH);
  /* p29.GESE*/ wire _GESE_SCAN_MATCH_Y  = not1(_WOTA_SCAN_MATCH_Yn);
  /* p29.CEHA*/ top.sprite_scanner.CEHA_SCANNINGp     = not1(top.sprite_scanner.CENO_SCANNINGp.qn16());

  /* p29.CARE*/ top.sprite_scanner.CARE_STORE_ENp_ABxxEFxx = and3(top.clk_reg.XOCE_xBCxxFGx, top.sprite_scanner.CEHA_SCANNINGp, _GESE_SCAN_MATCH_Y); // Dots on VCC, this is AND. Die shot and schematic wrong.

  /*#p28.ACYL*/ top.sprite_scanner.ACYL_SCANNINGp = and2(top.dma_reg.BOGE_DMA_RUNNINGn, top.sprite_scanner.BESU_SCANNINGp.qp04());

  /*#p28.ANOM*/ top.sprite_scanner.ANOM_LINE_RSTn = nor2(top.lcd_reg.ATEJ_LINE_TRIGp, top.clk_reg.ATAR_VID_RSTp);
  /*#p29.BALU*/ wire _BALU_LINE_RSTp = not1(top.sprite_scanner.ANOM_LINE_RSTn);
  /*#p29.BAGY*/ top.sprite_scanner.BAGY_LINE_RSTn = not1(_BALU_LINE_RSTp);

  /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or3(top.sprite_scanner.DOBA_SCAN_DONE_B.qp17(), _BALU_LINE_RSTp, top.sprite_scanner.BYBA_SCAN_DONE_A.qn16());
  /*#p29.AVAP*/ top.sprite_scanner.AVAP_RENDER_START_TRIGp = not1(_BEBU_SCAN_DONE_TRIGn);

  //----------------------------------------

  /*#p21.ACAM*/ wire _ACAM_X0n = not1(top.pix_pipe.XEHO_X0p.qp17());
  /* p21.AZUB*/ wire _AZUB_X1n = not1(top.pix_pipe.SAVY_X1p.qp17());
  /* p21.AMEL*/ wire _AMEL_X2n = not1(top.pix_pipe.XODU_X2p.qp17());
  /* p21.AHAL*/ wire _AHAL_X3n = not1(top.pix_pipe.XYDO_X3p.qp17());
  /* p21.APUX*/ wire _APUX_X4n = not1(top.pix_pipe.TUHU_X4p.qp17());
  /* p21.ABEF*/ wire _ABEF_X5n = not1(top.pix_pipe.TUKY_X5p.qp17());
  /* p21.ADAZ*/ wire _ADAZ_X6n = not1(top.pix_pipe.TAKO_X6p.qp17());
  /* p21.ASAH*/ wire _ASAH_X7n = not1(top.pix_pipe.SYBE_X7p.qp17());

  /*#p29.BYJO*/ wire _BYJO_SCANNINGn  = not1(top.sprite_scanner.CEHA_SCANNINGp);
  /*#p29.AZEM*/ wire _AZEM_RENDERINGp = and2(_BYJO_SCANNINGn, top.pix_pipe.XYMU_RENDERINGn.qn03());
  /*#p29.AROR*/ wire _AROR_MATCH_ENp  = and2(_AZEM_RENDERINGp, top.pix_pipe.XYLO_LCDC_SPENn.qn08());

  {
    /*#p29.BUZA*/ wire _BUZA_STORE_SPRITE_INDXn = and2(top.sprite_scanner.CENO_SCANNINGp.qn16(), top.pix_pipe.XYMU_RENDERINGn.qn03());
    /*#p30.WUZY*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, top.sprite_scanner.XADU_SPRITE_IDX0p.qn12());
    /* p30.WYSE*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, top.sprite_scanner.XEDY_SPRITE_IDX1p.qn12());
    /* p30.ZYSU*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, top.sprite_scanner.ZUZE_SPRITE_IDX2p.qn12());
    /* p30.WYDA*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, top.sprite_scanner.XOBE_SPRITE_IDX3p.qn12());
    /* p30.WUCO*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, top.sprite_scanner.YDUF_SPRITE_IDX4p.qn12());
    /* p30.WEZA*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(_BUZA_STORE_SPRITE_INDXn, top.sprite_scanner.XECU_SPRITE_IDX5p.qn12());
  }

  //----------------------------------------

  /*#p31.ZOGY*/ wire ZOGY_STORE0_MATCH0n = xor2(top.sprite_store.XEPE_STORE0_X0p.qn08(), _ACAM_X0n);
  /* p31.ZEBA*/ wire ZEBA_STORE0_MATCH1n = xor2(top.sprite_store.YLAH_STORE0_X1p.qn08(), _AZUB_X1n);
  /* p31.ZAHA*/ wire ZAHA_STORE0_MATCH2n = xor2(top.sprite_store.ZOLA_STORE0_X2p.qn08(), _AMEL_X2n);
  /* p31.ZOKY*/ wire ZOKY_STORE0_MATCH3n = xor2(top.sprite_store.ZULU_STORE0_X3p.qn08(), _AHAL_X3n);
  /* p31.WOJU*/ wire WOJU_STORE0_MATCH4n = xor2(top.sprite_store.WELO_STORE0_X4p.qn08(), _APUX_X4n);
  /* p31.YFUN*/ wire YFUN_STORE0_MATCH5n = xor2(top.sprite_store.XUNY_STORE0_X5p.qn08(), _ABEF_X5n);
  /* p31.WYZA*/ wire WYZA_STORE0_MATCH6n = xor2(top.sprite_store.WOTE_STORE0_X6p.qn08(), _ADAZ_X6n);
  /* p31.YPUK*/ wire YPUK_STORE0_MATCH7n = xor2(top.sprite_store.XAKO_STORE0_X7p.qn08(), _ASAH_X7n);

  /* p31.EDYM*/ wire EDYM_STORE1_MATCH0n = xor2(top.sprite_store.DANY_STORE1_X0p.qn08(), _ACAM_X0n);
  /* p31.EMYB*/ wire EMYB_STORE1_MATCH1n = xor2(top.sprite_store.DUKO_STORE1_X1p.qn08(), _AZUB_X1n);
  /* p31.EBEF*/ wire EBEF_STORE1_MATCH2n = xor2(top.sprite_store.DESU_STORE1_X2p.qn08(), _AMEL_X2n);
  /* p31.EWOK*/ wire EWOK_STORE1_MATCH3n = xor2(top.sprite_store.DAZO_STORE1_X3p.qn08(), _AHAL_X3n);
  /* p31.COLA*/ wire COLA_STORE1_MATCH4n = xor2(top.sprite_store.DAKE_STORE1_X4p.qn08(), _APUX_X4n);
  /* p31.BOBA*/ wire BOBA_STORE1_MATCH5n = xor2(top.sprite_store.CESO_STORE1_X5p.qn08(), _ABEF_X5n);
  /* p31.COLU*/ wire COLU_STORE1_MATCH6n = xor2(top.sprite_store.DYFU_STORE1_X6p.qn08(), _ADAZ_X6n);
  /* p31.BAHU*/ wire BAHU_STORE1_MATCH7n = xor2(top.sprite_store.CUSY_STORE1_X7p.qn08(), _ASAH_X7n);

  /* p31.FUZU*/ wire FUZU_STORE2_MATCH0n = xor2(top.sprite_store.FOKA_STORE2_X0p.qn08(), _ACAM_X0n);
  /* p31.FESO*/ wire FESO_STORE2_MATCH1n = xor2(top.sprite_store.FYTY_STORE2_X1p.qn08(), _AZUB_X1n);
  /* p31.FOKY*/ wire FOKY_STORE2_MATCH2n = xor2(top.sprite_store.FUBY_STORE2_X2p.qn08(), _AMEL_X2n);
  /* p31.FYVA*/ wire FYVA_STORE2_MATCH3n = xor2(top.sprite_store.GOXU_STORE2_X3p.qn08(), _AHAL_X3n);
  /* p31.CEKO*/ wire CEKO_STORE2_MATCH4n = xor2(top.sprite_store.DUHY_STORE2_X4p.qn08(), _APUX_X4n);
  /* p31.DETY*/ wire DETY_STORE2_MATCH5n = xor2(top.sprite_store.EJUF_STORE2_X5p.qn08(), _ABEF_X5n);
  /* p31.DOZO*/ wire DOZO_STORE2_MATCH6n = xor2(top.sprite_store.ENOR_STORE2_X6p.qn08(), _ADAZ_X6n);
  /* p31.CONY*/ wire CONY_STORE2_MATCH7n = xor2(top.sprite_store.DEPY_STORE2_X7p.qn08(), _ASAH_X7n);

  /* p31.YHOK*/ wire YHOK_STORE3_MATCH0n = xor2(top.sprite_store.XOLY_STORE3_X0p.qn08(), _ACAM_X0n);
  /* p31.YCAH*/ wire YCAH_STORE3_MATCH1n = xor2(top.sprite_store.XYBA_STORE3_X1p.qn08(), _AZUB_X1n);
  /* p31.YDAJ*/ wire YDAJ_STORE3_MATCH2n = xor2(top.sprite_store.XABE_STORE3_X2p.qn08(), _AMEL_X2n);
  /* p31.YVUZ*/ wire YVUZ_STORE3_MATCH3n = xor2(top.sprite_store.XEKA_STORE3_X3p.qn08(), _AHAL_X3n);
  /* p31.YVAP*/ wire YVAP_STORE3_MATCH4n = xor2(top.sprite_store.XOMY_STORE3_X4p.qn08(), _APUX_X4n);
  /* p31.XENY*/ wire XENY_STORE3_MATCH5n = xor2(top.sprite_store.WUHA_STORE3_X5p.qn08(), _ABEF_X5n);
  /* p31.XAVU*/ wire XAVU_STORE3_MATCH6n = xor2(top.sprite_store.WYNA_STORE3_X6p.qn08(), _ADAZ_X6n);
  /* p31.XEVA*/ wire XEVA_STORE3_MATCH7n = xor2(top.sprite_store.WECO_STORE3_X7p.qn08(), _ASAH_X7n);

  /* p31.XEJU*/ wire XEJU_STORE4_MATCH0n = xor2(top.sprite_store.WEDU_STORE4_X0p.qn08(), _ACAM_X0n);
  /* p31.ZATE*/ wire ZATE_STORE4_MATCH1n = xor2(top.sprite_store.YGAJ_STORE4_X1p.qn08(), _AZUB_X1n);
  /* p31.ZAKU*/ wire ZAKU_STORE4_MATCH2n = xor2(top.sprite_store.ZYJO_STORE4_X2p.qn08(), _AMEL_X2n);
  /* p31.YBOX*/ wire YBOX_STORE4_MATCH3n = xor2(top.sprite_store.XURY_STORE4_X3p.qn08(), _AHAL_X3n);
  /* p31.ZYKU*/ wire ZYKU_STORE4_MATCH4n = xor2(top.sprite_store.YBED_STORE4_X4p.qn08(), _APUX_X4n);
  /* p31.ZYPU*/ wire ZYPU_STORE4_MATCH5n = xor2(top.sprite_store.ZALA_STORE4_X5p.qn08(), _ABEF_X5n);
  /* p31.XAHA*/ wire XAHA_STORE4_MATCH6n = xor2(top.sprite_store.WYDE_STORE4_X6p.qn08(), _ADAZ_X6n);
  /* p31.ZEFE*/ wire ZEFE_STORE4_MATCH7n = xor2(top.sprite_store.XEPA_STORE4_X7p.qn08(), _ASAH_X7n);

  /* p31.GUZO*/ wire GUZO_STORE5_MATCH0n = xor2(top.sprite_store.FUSA_STORE5_X0p.qn08(), _ACAM_X0n);
  /* p31.GOLA*/ wire GOLA_STORE5_MATCH1n = xor2(top.sprite_store.FAXA_STORE5_X1p.qn08(), _AZUB_X1n);
  /* p31.GEVE*/ wire GEVE_STORE5_MATCH2n = xor2(top.sprite_store.FOZY_STORE5_X2p.qn08(), _AMEL_X2n);
  /* p31.GUDE*/ wire GUDE_STORE5_MATCH3n = xor2(top.sprite_store.FESY_STORE5_X3p.qn08(), _AHAL_X3n);
  /* p31.BAZY*/ wire BAZY_STORE5_MATCH4n = xor2(top.sprite_store.CYWE_STORE5_X4p.qn08(), _APUX_X4n);
  /* p31.CYLE*/ wire CYLE_STORE5_MATCH5n = xor2(top.sprite_store.DYBY_STORE5_X5p.qn08(), _ABEF_X5n);
  /* p31.CEVA*/ wire CEVA_STORE5_MATCH6n = xor2(top.sprite_store.DURY_STORE5_X6p.qn08(), _ADAZ_X6n);
  /* p31.BUMY*/ wire BUMY_STORE5_MATCH7n = xor2(top.sprite_store.CUVY_STORE5_X7p.qn08(), _ASAH_X7n);

  /* p31.XOSU*/ wire XOSU_STORE6_MATCH0n = xor2(top.sprite_store.YCOL_STORE6_X0p.qn08(), _ACAM_X0n);
  /* p31.ZUVU*/ wire ZUVU_STORE6_MATCH1n = xor2(top.sprite_store.YRAC_STORE6_X1p.qn08(), _AZUB_X1n);
  /* p31.XUCO*/ wire XUCO_STORE6_MATCH2n = xor2(top.sprite_store.YMEM_STORE6_X2p.qn08(), _AMEL_X2n);
  /* p31.ZULO*/ wire ZULO_STORE6_MATCH3n = xor2(top.sprite_store.YVAG_STORE6_X3p.qn08(), _AHAL_X3n);
  /* p31.ZARE*/ wire ZARE_STORE6_MATCH4n = xor2(top.sprite_store.ZOLY_STORE6_X4p.qn08(), _APUX_X4n);
  /* p31.ZEMU*/ wire ZEMU_STORE6_MATCH5n = xor2(top.sprite_store.ZOGO_STORE6_X5p.qn08(), _ABEF_X5n);
  /* p31.ZYGO*/ wire ZYGO_STORE6_MATCH6n = xor2(top.sprite_store.ZECU_STORE6_X6p.qn08(), _ADAZ_X6n);
  /* p31.ZUZY*/ wire ZUZY_STORE6_MATCH7n = xor2(top.sprite_store.ZESA_STORE6_X7p.qn08(), _ASAH_X7n);

  /* p31.DUSE*/ wire DUSE_STORE7_MATCH0n = xor2(top.sprite_store.ERAZ_STORE7_X0p.qn08(), _ACAM_X0n);
  /* p31.DAGU*/ wire DAGU_STORE7_MATCH1n = xor2(top.sprite_store.EPUM_STORE7_X1p.qn08(), _AZUB_X1n);
  /* p31.DYZE*/ wire DYZE_STORE7_MATCH2n = xor2(top.sprite_store.EROL_STORE7_X2p.qn08(), _AMEL_X2n);
  /* p31.DESO*/ wire DESO_STORE7_MATCH3n = xor2(top.sprite_store.EHYN_STORE7_X3p.qn08(), _AHAL_X3n);
  /* p31.EJOT*/ wire EJOT_STORE7_MATCH4n = xor2(top.sprite_store.FAZU_STORE7_X4p.qn08(), _APUX_X4n);
  /* p31.ESAJ*/ wire ESAJ_STORE7_MATCH5n = xor2(top.sprite_store.FAXE_STORE7_X5p.qn08(), _ABEF_X5n);
  /* p31.DUCU*/ wire DUCU_STORE7_MATCH6n = xor2(top.sprite_store.EXUK_STORE7_X6p.qn08(), _ADAZ_X6n);
  /* p31.EWUD*/ wire EWUD_STORE7_MATCH7n = xor2(top.sprite_store.FEDE_STORE7_X7p.qn08(), _ASAH_X7n);

  /* p31.DUZE*/ wire DUZE_STORE8_MATCH0n = xor2(top.sprite_store.EZUF_STORE8_X0p.qn08(), _APUX_X4n);
  /* p31.DAGA*/ wire DAGA_STORE8_MATCH1n = xor2(top.sprite_store.ENAD_STORE8_X1p.qn08(), _ABEF_X5n);
  /* p31.DAWU*/ wire DAWU_STORE8_MATCH2n = xor2(top.sprite_store.EBOW_STORE8_X2p.qn08(), _ADAZ_X6n);
  /* p31.EJAW*/ wire EJAW_STORE8_MATCH3n = xor2(top.sprite_store.FYCA_STORE8_X3p.qn08(), _ASAH_X7n);
  /* p31.GOHO*/ wire GOHO_STORE8_MATCH4n = xor2(top.sprite_store.GAVY_STORE8_X4p.qn08(), _ACAM_X0n);
  /* p31.GASU*/ wire GASU_STORE8_MATCH5n = xor2(top.sprite_store.GYPU_STORE8_X5p.qn08(), _AZUB_X1n);
  /* p31.GABU*/ wire GABU_STORE8_MATCH6n = xor2(top.sprite_store.GADY_STORE8_X6p.qn08(), _AMEL_X2n);
  /* p31.GAFE*/ wire GAFE_STORE8_MATCH7n = xor2(top.sprite_store.GAZA_STORE8_X7p.qn08(), _AHAL_X3n);

  /* p31.YMAM*/ wire YMAM_STORE9_MATCH0n = xor2(top.sprite_store.XUVY_STORE9_X0p.qn08(), _ACAM_X0n);
  /* p31.YTYP*/ wire YTYP_STORE9_MATCH1n = xor2(top.sprite_store.XERE_STORE9_X1p.qn08(), _AZUB_X1n);
  /* p31.YFOP*/ wire YFOP_STORE9_MATCH2n = xor2(top.sprite_store.XUZO_STORE9_X2p.qn08(), _AMEL_X2n);
  /* p31.YVAC*/ wire YVAC_STORE9_MATCH3n = xor2(top.sprite_store.XEXA_STORE9_X3p.qn08(), _AHAL_X3n);
  /* p31.ZYWU*/ wire ZYWU_STORE9_MATCH4n = xor2(top.sprite_store.YPOD_STORE9_X4p.qn08(), _APUX_X4n);
  /* p31.ZUZA*/ wire ZUZA_STORE9_MATCH5n = xor2(top.sprite_store.YROP_STORE9_X5p.qn08(), _ABEF_X5n);
  /* p31.ZEJO*/ wire ZEJO_STORE9_MATCH6n = xor2(top.sprite_store.YNEP_STORE9_X6p.qn08(), _ADAZ_X6n);
  /* p31.ZEDA*/ wire ZEDA_STORE9_MATCH7n = xor2(top.sprite_store.YZOF_STORE9_X7p.qn08(), _ASAH_X7n);

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

  /* p29.YDUG*/ top.sprite_store.YDUG_STORE0_MATCHn = nand3(_AROR_MATCH_ENp, ZAKO_STORE0_MATCHAp, XEBA_STORE0_MATCHBp);
  /* p29.DYDU*/ top.sprite_store.DYDU_STORE1_MATCHn = nand3(_AROR_MATCH_ENp, EWAM_STORE1_MATCHAp, CYVY_STORE1_MATCHBp);
  /* p29.DEGO*/ top.sprite_store.DEGO_STORE2_MATCHn = nand3(_AROR_MATCH_ENp, CEHU_STORE2_MATCHAp, EKES_STORE2_MATCHBp);
  /* p29.YLOZ*/ top.sprite_store.YLOZ_STORE3_MATCHn = nand3(_AROR_MATCH_ENp, ZURE_STORE3_MATCHAp, YWOS_STORE3_MATCHBp);
  /* p29.XAGE*/ top.sprite_store.XAGE_STORE4_MATCHn = nand3(_AROR_MATCH_ENp, YKOK_STORE4_MATCHAp, YNAZ_STORE4_MATCHBp);
  /* p29.EGOM*/ top.sprite_store.EGOM_STORE5_MATCHn = nand3(_AROR_MATCH_ENp, COGY_STORE5_MATCHAp, FYMA_STORE5_MATCHBp);
  /* p29.YBEZ*/ top.sprite_store.YBEZ_STORE6_MATCHn = nand3(_AROR_MATCH_ENp, YWAP_STORE6_MATCHAp, YDOT_STORE6_MATCHBp);
  /* p29.DYKA*/ top.sprite_store.DYKA_STORE7_MATCHn = nand3(_AROR_MATCH_ENp, CYCO_STORE7_MATCHAp, DAJE_STORE7_MATCHBp);
  /* p29.EFYL*/ top.sprite_store.EFYL_STORE8_MATCHn = nand3(_AROR_MATCH_ENp, DAMA_STORE8_MATCHAp, FEHA_STORE8_MATCHBp);
  /* p29.YGEM*/ top.sprite_store.YGEM_STORE9_MATCHn = nand3(_AROR_MATCH_ENp, YLEV_STORE9_MATCHAp, YTUB_STORE9_MATCHBp);

  /* p29.FEFY*/ wire _FEFY_STORE_MATCHp = nand5(top.sprite_store.XAGE_STORE4_MATCHn, top.sprite_store.YLOZ_STORE3_MATCHn, top.sprite_store.DEGO_STORE2_MATCHn, top.sprite_store.DYDU_STORE1_MATCHn, top.sprite_store.YDUG_STORE0_MATCHn);
  /* p29.FOVE*/ wire _FOVE_STORE_MATCHp = nand5(top.sprite_store.YGEM_STORE9_MATCHn, top.sprite_store.EFYL_STORE8_MATCHn, top.sprite_store.DYKA_STORE7_MATCHn, top.sprite_store.YBEZ_STORE6_MATCHn, top.sprite_store.EGOM_STORE5_MATCHn);
  /* p29.FEPO*/ top.sprite_store.FEPO_STORE_MATCHp = or2(_FEFY_STORE_MATCHp, _FOVE_STORE_MATCHp);

  //------------------------------------------------------------------------------

  // 128 + 32 + 4 + 2 + 1 = 167
  /*#p21.XUGU*/ wire _XUGU_X_167n = nand5(top.pix_pipe.XEHO_X0p.qp17(), top.pix_pipe.SAVY_X1p.qp17(), top.pix_pipe.XODU_X2p.qp17(), top.pix_pipe.TUKY_X5p.qp17(), top.pix_pipe.SYBE_X7p.qp17());
  /*#p21.XANO*/ wire _XANO_X_167p = not1(_XUGU_X_167n);
  /*#p21.XENA*/ wire _XENA_STORE_MATCHn = not1(top.sprite_store.FEPO_STORE_MATCHp);
  /*#p21.WODU*/ top.pix_pipe.WODU_HBLANKp = and2(_XENA_STORE_MATCHn, _XANO_X_167p);

  /*#p21.TAPA*/ wire _TAPA_INT_OAM = and2(top.lcd_reg.TOLU_VBLANKn, top.lcd_reg.SELA_LINE_P908p);
  /*#p21.TARU*/ wire _TARU_INT_HBL = and2(top.pix_pipe.WODU_HBLANKp, top.lcd_reg.TOLU_VBLANKn);
  /*#p21.SUKO*/ wire _SUKO_INT_STATp = amux4(top.pix_pipe.RUGU_STAT_LYI_ENn.qn08(), top.lcd_reg.ROPO_LY_MATCH_SYNCp.qp17(),
                                             top.pix_pipe.REFE_STAT_OAI_ENn.qn08(), _TAPA_INT_OAM,
                                             top.pix_pipe.RUFO_STAT_VBI_ENn.qn08(), top.lcd_reg.PARU_VBLANKp_d4, // polarity?
                                             top.pix_pipe.ROXE_STAT_HBI_ENn.qn08(), _TARU_INT_HBL);

  /*#p21.TUVA*/ wire _TUVA_INT_STATn = not1(_SUKO_INT_STATp);
  /*#p21.VOTY*/ top.pix_pipe.VOTY_INT_STATp = not1(_TUVA_INT_STATn);

  /*#p27.NOCU*/ top.pix_pipe.NOCU_WIN_MODEn = not1(top.pix_pipe.PYNU_WIN_MODE_A.qp04());
  /* p27.PORE*/ top.pix_pipe.PORE_WIN_MODEp = not1(top.pix_pipe.NOCU_WIN_MODEn);
  /*#p27.SYLO*/ top.pix_pipe.SYLO_WIN_HITn = not1(top.pix_pipe.RYDY);
  /*#p24.TOMU*/ top.pix_pipe.TOMU_WIN_HITp = not1(top.pix_pipe.SYLO_WIN_HITn);
  /* p27.TUXY*/ wire _TUXY_WIN_FIRST_TILE_NE = nand2(top.pix_pipe.SYLO_WIN_HITn, top.pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17());
  /* p27.SUZU*/ top.pix_pipe.SUZU_WIN_FIRST_TILEne = not1(_TUXY_WIN_FIRST_TILE_NE);

  /* p24.LOBY*/ top.pix_pipe.LOBY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGn.qn03());

  //----------

  /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn = not1(top.tile_fetcher.POKY_PRELOAD_LATCHp.qp04());
  /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn = nand4(top.pix_pipe.XYMU_RENDERINGn.qn03(),
                                                      _ROMO_PRELOAD_DONEn,
                                                      top.tile_fetcher.NYKA_FETCH_DONE_P11.qp17(),
                                                      top.tile_fetcher.PORY_FETCH_DONE_P12.qp17());


  /* p27.SEKO*/ top.pix_pipe.SEKO_FETCH_TRIGp = nor2(top.pix_pipe.RYFA_FETCHn_A.qn16(), top.pix_pipe.RENE_FETCHn_B.qp17());
  /* p27.TAVE*/ top.pix_pipe.TAVE_PRELOAD_DONE_TRIGp = not1(_SUVU_PRELOAD_DONE_TRIGn);
  /* p27.TEVO*/ top.pix_pipe.TEVO_FETCH_TRIGp = or3(top.pix_pipe.SEKO_FETCH_TRIGp, top.pix_pipe.SUZU_WIN_FIRST_TILEne, top.pix_pipe.TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  /*#p27.NUNY*/ top.pix_pipe.NUNY_WX_MATCH_TRIGp = and2(top.pix_pipe.PYNU_WIN_MODE_A.qp04(), top.pix_pipe.NOPA_WIN_MODE_B.qn16());
  /* p27.NYFO*/ wire _NYFO_WIN_FETCH_TRIGn = not1(top.pix_pipe.NUNY_WX_MATCH_TRIGp);
  /* p27.MOSU*/ top.pix_pipe.MOSU_WIN_FETCH_TRIGp = not1(_NYFO_WIN_FETCH_TRIGn);

  /* p27.NYXU*/ top.pix_pipe.NYXU_FETCH_TRIGn = nor3(top.sprite_scanner.AVAP_RENDER_START_TRIGp, top.pix_pipe.MOSU_WIN_FETCH_TRIGp, top.pix_pipe.TEVO_FETCH_TRIGp);

  /* p21.SADU*/ top.pix_pipe.SADU_STAT_MODE0n = nor2(top.pix_pipe.XYMU_RENDERINGn.qn03(), top.lcd_reg.PARU_VBLANKp_d4); // die NOR
  /* p21.XATY*/ top.pix_pipe.XATY_STAT_MODE1n = nor2(top.sprite_scanner.ACYL_SCANNINGp, top.pix_pipe.XYMU_RENDERINGn.qn03()); // die NOR

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

  /* p01.UVYN*/ top.tim_reg.UVYN_DIV_05n = not1(top.tim_reg.TAMA_DIV_05.qp17());
  /* p01.UMEK*/ top.tim_reg.UMEK_DIV_06n = not1(top.tim_reg.UGOT_DIV_06.qp17());
  /* p01.UREK*/ top.tim_reg.UREK_DIV_07n = not1(top.tim_reg.TULU_DIV_07.qp17());

  //----------------------------------------

  /*p02.ASOK*/ top.joypad.ASOK_INT_JOYp = and2(top.joypad.APUG_JP_GLITCH3.qp17(), top.joypad.BATU_JP_GLITCH0.qp17());

  //------------------------------------------------------------------------------

  /* p26.AXAD*/ wire _AXAD_WIN_MODEn = not1(top.pix_pipe.PORE_WIN_MODEp);

  /*#p27.LAXE*/ wire _LAXE_BFETCH_S0n = not1(top.tile_fetcher.LAXU_BFETCH_S0.qp17());
  /*#p27.NAKO*/ wire _NAKO_BFETCH_S1n = not1(top.tile_fetcher.MESU_BFETCH_S1.qp17());
  /*#p27.NOFU*/ wire _NOFU_BFETCH_S2n = not1(top.tile_fetcher.NYVA_BFETCH_S2.qp17());

  /* p27.NOGU*/ wire _NOGU_FETCH_01p = nand2(_NAKO_BFETCH_S1n, _NOFU_BFETCH_S2n);
  /* p27.NENY*/ wire _NENY_FETCH_01n = not1(_NOGU_FETCH_01p);

  /* p27.LUSU*/ wire _LUSU_BGW_VRAM_RDn = not1(top.tile_fetcher.LONY_BG_FETCH_RUNNINGp.qp03());
  /* p27.LENA*/ top.tile_fetcher.LENA_BGW_VRM_RDp = not1(_LUSU_BGW_VRAM_RDn);

  /* p27.POTU*/ wire _POTU_BG_MAP_READp  = and2(top.tile_fetcher.LENA_BGW_VRM_RDp, _NENY_FETCH_01n);
  /*#p25.XEZE*/ wire _XEZE_WIN_MAP_READp = and2(_POTU_BG_MAP_READp, top.pix_pipe.PORE_WIN_MODEp);
  /* p26.ACEN*/ wire _ACEN_BG_MAP_READp  = and2(_POTU_BG_MAP_READp, _AXAD_WIN_MODEn);

  /* p27.NETA*/ top.tile_fetcher.NETA_TILE_READp = and2(top.tile_fetcher.LENA_BGW_VRM_RDp, _NOGU_FETCH_01p);
  /* p26.ASUL*/ wire _ASUL_BG_TILE_READp  = and2(top.tile_fetcher.NETA_TILE_READp, _AXAD_WIN_MODEn);
  /* p25.XUCY*/ top.tile_fetcher.XUCY_WIN_TILE_READn = nand2(top.tile_fetcher.NETA_TILE_READp, top.pix_pipe.PORE_WIN_MODEp);
  /*#p25.WUKO*/ top.tile_fetcher.WUKO_WIN_MAP_READn  = not1(_XEZE_WIN_MAP_READp);
  /* p26.BAFY*/ top.tile_fetcher.BAFY_BG_MAP_READn   = not1(_ACEN_BG_MAP_READp);
  /* p26.BEJE*/ top.tile_fetcher.BEJE_BG_TILE_READn = not1(_ASUL_BG_TILE_READp);

  /* p27.MOCE*/ top.tile_fetcher.MOCE_BFETCH_DONEn = nand3(top.tile_fetcher.LAXU_BFETCH_S0.qp17(), top.tile_fetcher.NYVA_BFETCH_S2.qp17(), top.pix_pipe.NYXU_FETCH_TRIGn);

  /*#p27.XUHA*/ top.tile_fetcher.XUHA_FETCH_S2p  = not1(_NOFU_BFETCH_S2n);

  // MYSO fires on fetch phase 2, 6, 10
  /*#p27.MYSO*/ wire _MYSO_BG_TRIGp   = nor3(top.pix_pipe.LOBY_RENDERINGn, _LAXE_BFETCH_S0n, top.tile_fetcher.LYZU_BFETCH_S0_D1.qp17());

  // NYDY on fetch phase 6
  /*#p27.NYDY*/ wire _NYDY_LATCH_TILE_DAn = nand3(_MYSO_BG_TRIGp, top.tile_fetcher.MESU_BFETCH_S1.qp17(), _NOFU_BFETCH_S2n);
  /*#p32.METE*/ top.tile_fetcher.METE_LATCH_TILE_DAp   = not1(_NYDY_LATCH_TILE_DAn);

  // MOFU fires on fetch phase 2 and 10
  /*p27.MOFU*/ wire _MOFU_LATCH_TILE_DBp = and2(_MYSO_BG_TRIGp, _NAKO_BFETCH_S1n);
  /*p32.LESO*/ top.tile_fetcher.LESO_LATCH_TILE_DBn   = not1(_MOFU_LATCH_TILE_DBp);

  /*p27.LYRY*/ top.tile_fetcher.LYRY_BFETCH_DONEp = not1(top.tile_fetcher.MOCE_BFETCH_DONEn);

  //------------------------------------------------------------------------------
  // FIXME this is kind of a mess

  /* p29.TEPA*/ wire _TEPA_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGn.qn03());
  /* p29.TYNO*/ wire _TYNO = nand3(top.sprite_fetcher.TOXE_SFETCH_S0.qp17(), top.sprite_fetcher.SEBA_SFETCH_S1_D5.qp17(), top.sprite_fetcher.VONU_SFETCH_S1_D4.qp17());
  /* p29.VUSA*/ wire _VUSA_SPRITE_DONEn  = or2(top.sprite_fetcher.TYFO_SFETCH_S0_D1.qn16(), _TYNO);
  /* p29.WUTY*/ top.sprite_fetcher.WUTY_SPRITE_DONEp = not1(_VUSA_SPRITE_DONEn);
  /* p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor3(_TEPA_RENDERINGn, top.sprite_fetcher.TULY_SFETCH_S1.qp17(), top.sprite_fetcher.TESE_SFETCH_S2.qp17());
  /* p28.WEFY*/ top.sprite_fetcher.WEFY_SPR_READp = and2(_TUVO_PPU_OAM_RDp, top.sprite_fetcher.TYFO_SFETCH_S0_D1.qp17());
  /* p29.TYTU*/ wire _TYTU_SFETCH_S0n = not1(top.sprite_fetcher.TOXE_SFETCH_S0.qp17());
  /* p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand2(top.sprite_fetcher.TYFO_SFETCH_S0_D1.qp17(), _TYTU_SFETCH_S0n);
  /* p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY = nor2(top.sprite_fetcher.TULY_SFETCH_S1.qp17(), top.sprite_fetcher.VONU_SFETCH_S1_D4.qp17());
  /* p29.TYSO*/ wire _TYSO_SPRITE_READn = or2(_SAKY_SFETCH_MYSTERY, _TEPA_RENDERINGn); // def or
  /* p29.TEXY*/ top.sprite_fetcher.TEXY_SPR_READ_VRAMp = not1(_TYSO_SPRITE_READn);
  /* p29.ABON*/ top.sprite_fetcher.ABON_SPR_VRM_RDn = not1(top.sprite_fetcher.TEXY_SPR_READ_VRAMp);
  /* p25.SOHO*/ top.sprite_fetcher.SOHO_SPR_VRAM_RDp = and2(_TACU_SPR_SEQ_5_TRIG, top.sprite_fetcher.TEXY_SPR_READ_VRAMp);
  /* p25.VAPE*/ top.sprite_fetcher.VAPE_FETCH_OAM_CLK = and2(_TUVO_PPU_OAM_RDp, _TACU_SPR_SEQ_5_TRIG);
  /*#p29.XUQU*/ top.sprite_fetcher.XUQU_SPRITE_AB = not1(top.sprite_fetcher.VONU_SFETCH_S1_D4.qn16());
  /* p29.SYCU*/ wire _SYCU_SFETCH_S0pe = nor3(_TYTU_SFETCH_S0n, top.pix_pipe.LOBY_RENDERINGn, top.sprite_fetcher.TYFO_SFETCH_S0_D1.qp17());
  /*#p29.RACA*/ top.sprite_fetcher.RACA_LATCH_SPPIXB = and2(top.sprite_fetcher.VONU_SFETCH_S1_D4.qp17(), _SYCU_SFETCH_S0pe);
  /*#p29.TOPU*/ top.sprite_fetcher.TOPU_LATCH_SPPIXA = and2(top.sprite_fetcher.TULY_SFETCH_S1.qp17(), _SYCU_SFETCH_S0pe);

  //----------------------------------------

  RegBase::tick_running = false;
}





//==============================================================================
//==============================================================================
//==============================================================================
//==============================================================================






void SchematicTop::tock_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  auto& top = *this;
  wire GND = 0;
  wire WEFE_VCC = 1;

  {
    /*#p25.SYCY*/ wire SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p);
    /*#p25.SOTO*/ SOTO_DBG_VRAM.dff17(SYCY_DBG_CLOCKn, clk_reg.CUNU_SYS_RSTn, SOTO_DBG_VRAM.qn16());
  }

  clk_reg.tock_clk_slow(top, CLKGOOD);
  clk_reg.tock_rst_slow(top, RST, CLKGOOD, CPUREADY);
  clk_reg.tock_dbg_slow(top);
  clk_reg.tock_vid_slow(top, CLK);
  tim_reg.tock(RST, top, cpu_bus);
  bootrom.tock(top, cpu_bus);

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
  /*p06.URYS*/ wire _URYS_FF01_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());
  /*p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /*p06.UFEG*/ wire _UFEG_FF01_RDp = and4(top.TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.BUS_CPU_A00.qp(), cpu_bus.TOLA_A01n());

  /*p06.COBA*/ wire _COBA_SER_CNT3n = not1(top.ser_reg.CALY_INT_SERp.qp17());
  /*p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, top.clk_reg.ALUR_SYS_RSTn);

  /*p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ETAF*/ top.ser_reg.ETAF_XFER_START.dff17(_UWAM_FF02_WRn, _CABY_XFER_RESET, cpu_bus.BUS_CPU_D7p.qp());
  /*p06.CULY*/ top.ser_reg.CULY_XFER_DIR  .dff17(_UWAM_FF02_WRn, top.clk_reg.ALUR_SYS_RSTn, cpu_bus.BUS_CPU_D0p.qp());

  /*p06.UCOM*/ wire _UCOM_FF02_RD = and4(top.TEDO_CPU_RDp, _SANO_ADDR_FF00_FF03, cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01.qp());
  /*p06.ELUV*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UCOM_FF02_RD, top.ser_reg.ETAF_XFER_START.qn16());
  /*p06.CORE*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UCOM_FF02_RD, top.ser_reg.CULY_XFER_DIR.qn16());

  /*p06.COTY*/ top.ser_reg.COTY_SER_CLK.dff17(top.tim_reg.UVYN_DIV_05n, _UWAM_FF02_WRn, top.ser_reg.COTY_SER_CLK.qn16());

  /* p06.CAVE*/ wire _CAVE_SER_CLK_MUXn = mux2n(top.ser_reg.CULY_XFER_DIR.qp17(), top.ser_reg.COTY_SER_CLK.qp17(), top.ser_reg.PIN_SCK.qn());

  /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK_MUXn, top.ser_reg.ETAF_XFER_START.qn16()); // this must stop the clock or something when the transmit's done
  /* p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  /* p06.CARO*/ wire _CARO_SER_RST = and2(_UWAM_FF02_WRn, top.clk_reg.ALUR_SYS_RSTn);
  /* p06.CAFA*/ top.ser_reg.CAFA_SER_CNT0.dff17(_DAWA_SER_CLK,                    _CARO_SER_RST, top.ser_reg.CAFA_SER_CNT0.qn16());
  /* p06.CYLO*/ top.ser_reg.CYLO_SER_CNT1.dff17(top.ser_reg.CAFA_SER_CNT0.qn16(), _CARO_SER_RST, top.ser_reg.CYLO_SER_CNT1.qn16());
  /* p06.CYDE*/ top.ser_reg.CYDE_SER_CNT2.dff17(top.ser_reg.CYLO_SER_CNT1.qn16(), _CARO_SER_RST, top.ser_reg.CYDE_SER_CNT2.qn16());
  /* p06.CALY*/ top.ser_reg.CALY_INT_SERp.dff17(top.ser_reg.CYDE_SER_CNT2.qn16(), _CARO_SER_RST, top.ser_reg.CALY_INT_SERp.qn16());

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

  /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D0p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D1p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D2p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D3p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D4p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D5p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D6p.qp(), top.clk_reg.ALUR_SYS_RSTn);
  /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D7p.qp(), top.clk_reg.ALUR_SYS_RSTn);

  /* p06.CAGE*/ wire _CAGE_SIN_Cp = not1(top.ser_reg.PIN_SIN.qn());
  /* p06.CUBA*/ top.ser_reg.CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SIN_Cp);
  /* p06.DEGU*/ top.ser_reg.DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, top.ser_reg.CUBA_SER_DATA0.qp16());
  /* p06.DYRA*/ top.ser_reg.DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, top.ser_reg.DEGU_SER_DATA1.qp16());
  /* p06.DOJO*/ top.ser_reg.DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, top.ser_reg.DYRA_SER_DATA2.qp16());
  /* p06.DOVU*/ top.ser_reg.DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, top.ser_reg.DOJO_SER_DATA3.qp16());
  /* p06.EJAB*/ top.ser_reg.EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, top.ser_reg.DOVU_SER_DATA4.qp16());
  /* p06.EROD*/ top.ser_reg.EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, top.ser_reg.EJAB_SER_DATA5.qp16());
  /* p06.EDER*/ top.ser_reg.EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEL_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, top.ser_reg.EROD_SER_DATA6.qp16());

  /* p06.ELYS*/ top.ser_reg.ELYS_SER_OUT  .dff17(_EDYL_SER_CLK, top.clk_reg.ALUR_SYS_RSTn, top.ser_reg.EDER_SER_DATA7.qp16());

  // FIXME hacking out debug stuff
  ///*p05.KENA*/ ser_reg.SOUT  = mux2n(KUKO_DBG_FF00_D6, ser_reg.SER_OUT, FF60_0);

  /* p06.KEXU*/ wire _KEXU = nand2(top.ser_reg.COTY_SER_CLK.qp17(), top.ser_reg.CULY_XFER_DIR.qp17());
  /* p06.KUJO*/ wire _KUJO = nor2 (top.ser_reg.COTY_SER_CLK.qp17(), /*p06.JAGO*/ not1(top.ser_reg.CULY_XFER_DIR.qp17()));

  /* p06.KEXU*/ top.ser_reg.PIN_SCK.io_pin(_KEXU, _KUJO, top.ser_reg.CULY_XFER_DIR.qp17());
  /* p05.KENA*/ top.ser_reg.PIN_SOUT.io_pin(top.ser_reg.ELYS_SER_OUT.qp17(), top.ser_reg.ELYS_SER_OUT.qp17());

  /*#p06.CUGY*/ cpu_bus.BUS_CPU_D0p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.CUBA_SER_DATA0.qn15());
  /* p06.DUDE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DEGU_SER_DATA1.qn15());
  /* p06.DETU*/ cpu_bus.BUS_CPU_D2p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DYRA_SER_DATA2.qn15());
  /* p06.DASO*/ cpu_bus.BUS_CPU_D3p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DOJO_SER_DATA3.qn15());
  /* p06.DAME*/ cpu_bus.BUS_CPU_D4p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.DOVU_SER_DATA4.qn15());
  /* p06.EVOK*/ cpu_bus.BUS_CPU_D5p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.EJAB_SER_DATA5.qn15());
  /* p06.EFAB*/ cpu_bus.BUS_CPU_D6p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.EROD_SER_DATA6.qn15());
  /*#p06.ETAK*/ cpu_bus.BUS_CPU_D7p.tri_6pn(_UFEG_FF01_RDp, top.ser_reg.EDER_SER_DATA7.qn15());


  //------------------------------------------------------------------------------

  /*p10.AMUS*/ wire _AMUS_XX_0xx00000 = nor6(top.cpu_bus.BUS_CPU_A00.qp(), top.cpu_bus.BUS_CPU_A01.qp(), top.cpu_bus.BUS_CPU_A02.qp(), top.cpu_bus.BUS_CPU_A03.qp(), top.cpu_bus.BUS_CPU_A04.qp(), top.cpu_bus.BUS_CPU_A07.qp());
  /*p10.ANAP*/ wire _ANAP_FF_0xx00000 = and2(_AMUS_XX_0xx00000, top.cpu_bus.SYKE_FF00_FFFFp());
  /*p10.AKUG*/ wire _AKUG_A06n = not1(top.cpu_bus.BUS_CPU_A06.qp());
  /*p10.BYKO*/ wire _BYKO_A05n = not1(top.cpu_bus.BUS_CPU_A05.qp());

  /*p02.KERY*/ wire _KERY_ANY_BUTTONp = or4(top.joypad.PIN_JOY_P13.qn(), top.joypad.PIN_JOY_P12.qn(), top.joypad.PIN_JOY_P11.qn(), top.joypad.PIN_JOY_P10.qn());

  {
    /*p02.AWOB*/ top.joypad.AWOB_WAKE_CPU.tp_latch(top.clk_reg.BOGA_Axxxxxxx, _KERY_ANY_BUTTONp);
    top.joypad.PIN_CPU_WAKE.set(top.joypad.AWOB_WAKE_CPU.qp08());
  }

  {
    /*p02.BATU*/ top.joypad.BATU_JP_GLITCH0.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, _KERY_ANY_BUTTONp);
    /*p02.ACEF*/ top.joypad.ACEF_JP_GLITCH1.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, top.joypad.BATU_JP_GLITCH0.qp17());
    /*p02.AGEM*/ top.joypad.AGEM_JP_GLITCH2.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, top.joypad.ACEF_JP_GLITCH1.qp17());
    /*p02.APUG*/ top.joypad.APUG_JP_GLITCH3.dff17(top.clk_reg.BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, top.joypad.AGEM_JP_GLITCH2.qp17());
  }

  {
    /*p10.ATOZ*/ wire _ATOZ_FF00_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /*p05.JUTE*/ top.joypad.JUTE_JOYP_RA    .dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p05.KECY*/ top.joypad.KECY_JOYP_LB    .dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p05.JALE*/ top.joypad.JALE_JOYP_UC    .dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p05.KYME*/ top.joypad.KYME_JOYP_DS    .dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p05.KELY*/ top.joypad.KELY_JOYP_UDLR  .dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p05.COFY*/ top.joypad.COFY_JOYP_ABCS  .dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p05.KUKO*/ top.joypad.KUKO_DBG_FF00_D6.dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p05.KERU*/ top.joypad.KERU_DBG_FF00_D7.dff17(_ATOZ_FF00_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D7p.qp());

    /* p10.ACAT*/ wire _ACAT_FF00_RDp = and4(top.TEDO_CPU_RDp, _ANAP_FF_0xx00000, _AKUG_A06n, _BYKO_A05n);
    /* p05.BYZO*/ wire _BYZO_FF00_RDn = not1(_ACAT_FF00_RDp);
    /* p05.KEVU*/ top.joypad.KEVU_JOYP_L0.tp_latch(_BYZO_FF00_RDn, top.joypad.PIN_JOY_P10.qn());
    /* p05.KAPA*/ top.joypad.KAPA_JOYP_L1.tp_latch(_BYZO_FF00_RDn, top.joypad.PIN_JOY_P11.qn());
    /* p05.KEJA*/ top.joypad.KEJA_JOYP_L2.tp_latch(_BYZO_FF00_RDn, top.joypad.PIN_JOY_P12.qn());
    /* p05.KOLO*/ top.joypad.KOLO_JOYP_L3.tp_latch(_BYZO_FF00_RDn, top.joypad.PIN_JOY_P13.qn());

    // JOYP should read as 0xCF at reset? So the RegQPs reset to 1 and the RegQNs reset to 0?
    // That also means that _both_ P14 and P15 are selected at reset :/

    /* p05.KEMA*/ top.cpu_bus.BUS_CPU_D0p.tri_6nn(_BYZO_FF00_RDn, top.joypad.KEVU_JOYP_L0.qp08());
    /* p05.KURO*/ top.cpu_bus.BUS_CPU_D1p.tri_6nn(_BYZO_FF00_RDn, top.joypad.KAPA_JOYP_L1.qp08());
    /* p05.KUVE*/ top.cpu_bus.BUS_CPU_D2p.tri_6nn(_BYZO_FF00_RDn, top.joypad.KEJA_JOYP_L2.qp08());
    /* p05.JEKU*/ top.cpu_bus.BUS_CPU_D3p.tri_6nn(_BYZO_FF00_RDn, top.joypad.KOLO_JOYP_L3.qp08());
    /* p05.KOCE*/ top.cpu_bus.BUS_CPU_D4p.tri_6nn(_BYZO_FF00_RDn, top.joypad.KELY_JOYP_UDLR.qn16());
    /* p05.CUDY*/ top.cpu_bus.BUS_CPU_D5p.tri_6nn(_BYZO_FF00_RDn, top.joypad.COFY_JOYP_ABCS.qn16());
    /* p??.????*/ top.cpu_bus.BUS_CPU_D6p.tri_6nn(_BYZO_FF00_RDn, top.joypad.KUKO_DBG_FF00_D6.qp17());
    /* p??.????*/ top.cpu_bus.BUS_CPU_D7p.tri_6nn(_BYZO_FF00_RDn, top.joypad.KERU_DBG_FF00_D7.qp17());
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

    /*p05.KOLE*/ wire _KOLE = nand2(top.joypad.JUTE_JOYP_RA.qp17(), BURO_FF60_0p);
    /*p05.KYBU*/ wire _KYBU = nor2 (top.joypad.JUTE_JOYP_RA.qp17(), _KURA);
    /*p05.KYTO*/ wire _KYTO = nand2(top.joypad.KECY_JOYP_LB.qp17(), BURO_FF60_0p);
    /*p05.KABU*/ wire _KABU = nor2 (top.joypad.KECY_JOYP_LB.qp17(), _KURA);
    /*p05.KYHU*/ wire _KYHU = nand2(top.joypad.JALE_JOYP_UC.qp17(), BURO_FF60_0p);
    /*p05.KASY*/ wire _KASY = nor2 (top.joypad.JALE_JOYP_UC.qp17(), _KURA);
    /*p05.KORY*/ wire _KORY = nand2(top.joypad.KYME_JOYP_DS.qp17(), BURO_FF60_0p);
    /*p05.KALE*/ wire _KALE = nor2 (top.joypad.KYME_JOYP_DS.qp17(), _KURA);

    top.joypad.PIN_JOY_P10.io_pin(_KOLE, _KYBU);
    top.joypad.PIN_JOY_P11.io_pin(_KYTO, _KABU);
    top.joypad.PIN_JOY_P12.io_pin(_KYHU, _KASY);
    top.joypad.PIN_JOY_P13.io_pin(_KORY, _KALE);

    /*p05.KARU*/ wire _KARU = or2(top.joypad.KELY_JOYP_UDLR.qn16(), _KURA);
    /*p05.CELA*/ wire _CELA = or2(top.joypad.COFY_JOYP_ABCS.qn16(), _KURA);

    /*
    // lcd ribbon voltages after bootrom
    04 5 left & b
    05 0 diodes 1&2
    06 5 down & start
    07 5 up & select
    08 5 right & a
    09 0 diodes 3 & 4
    */

    top.joypad.PIN_JOY_P14.io_pin(_KARU, top.joypad.KELY_JOYP_UDLR.qn16());
    top.joypad.PIN_JOY_P15.io_pin(_CELA, top.joypad.COFY_JOYP_ABCS.qn16());
  }

  //------------------------------------------------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  // 32 + 4 + 2 + 1 = 39
  /*#p28.FETO*/ wire _FETO_SCAN_DONE_d0 = and4(top.sprite_scanner.YFEL_SCAN0.qp17(), top.sprite_scanner.WEWY_SCAN1.qp17(), top.sprite_scanner.GOSO_SCAN2.qp17(), top.sprite_scanner.FONY_SCAN5.qp17());

  {
    /*#p29.BYBA*/ top.sprite_scanner.BYBA_SCAN_DONE_A.dff17(top.clk_reg.XUPY_ABxxEFxx, top.sprite_scanner.BAGY_LINE_RSTn, _FETO_SCAN_DONE_d0);
    /*#p29.DOBA*/ top.sprite_scanner.DOBA_SCAN_DONE_B.dff17(top.clk_reg.ALET_xBxDxFxH, top.sprite_scanner.BAGY_LINE_RSTn, top.sprite_scanner.BYBA_SCAN_DONE_A.qp17());

    /*#p28.ASEN*/ wire _ASEN_SCAN_DONE_PE = or2(top.clk_reg.ATAR_VID_RSTp, top.sprite_scanner.AVAP_RENDER_START_TRIGp);
    /*#p28.BESU*/ top.sprite_scanner.BESU_SCANNINGp.nor_latch(top.lcd_reg.CATU_LINE_P000.qp17(), _ASEN_SCAN_DONE_PE);
    /*#p29.CENO*/ top.sprite_scanner.CENO_SCANNINGp.dff17(top.clk_reg.XUPY_ABxxEFxx, top.clk_reg.ABEZ_VID_RSTn, top.sprite_scanner.BESU_SCANNINGp.qp04());
  }

  {
    /*#p30.CYKE*/ wire _CYKE_ABxxEFxx = not1(top.clk_reg.XUPY_ABxxEFxx);
    /*#p30.WUDA*/ wire _WUDA_xxCDxxGH = not1(_CYKE_ABxxEFxx);

    /* p28.YFOT*/ wire _YFOT_OAM_A2p = not1(top.oam_bus.BUS_OAM_A2n.qp());
    /* p28.YFOC*/ wire _YFOC_OAM_A3p = not1(top.oam_bus.BUS_OAM_A3n.qp());
    /* p28.YVOM*/ wire _YVOM_OAM_A4p = not1(top.oam_bus.BUS_OAM_A4n.qp());
    /* p28.YMEV*/ wire _YMEV_OAM_A5p = not1(top.oam_bus.BUS_OAM_A5n.qp());
    /* p28.XEMU*/ wire _XEMU_OAM_A6p = not1(top.oam_bus.BUS_OAM_A6n.qp());
    /* p28.YZET*/ wire _YZET_OAM_A7p = not1(top.oam_bus.BUS_OAM_A7n.qp());

    /*p30.XADU*/ top.sprite_scanner.XADU_SPRITE_IDX0p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YFOT_OAM_A2p);
    /*p30.XEDY*/ top.sprite_scanner.XEDY_SPRITE_IDX1p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YFOC_OAM_A3p);
    /*p30.ZUZE*/ top.sprite_scanner.ZUZE_SPRITE_IDX2p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YVOM_OAM_A4p);
    /*p30.XOBE*/ top.sprite_scanner.XOBE_SPRITE_IDX3p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YMEV_OAM_A5p);
    /*p30.YDUF*/ top.sprite_scanner.YDUF_SPRITE_IDX4p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _XEMU_OAM_A6p);
    /*p30.XECU*/ top.sprite_scanner.XECU_SPRITE_IDX5p.dff13(_WUDA_xxCDxxGH, WEFE_VCC, _YZET_OAM_A7p);
  }

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  {
    /*p28.GAVA*/ wire _GAVA_SCAN_CLK = or2(_FETO_SCAN_DONE_d0,   top.clk_reg.XUPY_ABxxEFxx);

    /*p28.YFEL*/ top.sprite_scanner.YFEL_SCAN0.dff17(_GAVA_SCAN_CLK,                       top.sprite_scanner.ANOM_LINE_RSTn, top.sprite_scanner.YFEL_SCAN0.qn16());
    /*p28.WEWY*/ top.sprite_scanner.WEWY_SCAN1.dff17(top.sprite_scanner.YFEL_SCAN0.qn16(), top.sprite_scanner.ANOM_LINE_RSTn, top.sprite_scanner.WEWY_SCAN1.qn16());
    /*p28.GOSO*/ top.sprite_scanner.GOSO_SCAN2.dff17(top.sprite_scanner.WEWY_SCAN1.qn16(), top.sprite_scanner.ANOM_LINE_RSTn, top.sprite_scanner.GOSO_SCAN2.qn16());
    /*p28.ELYN*/ top.sprite_scanner.ELYN_SCAN3.dff17(top.sprite_scanner.GOSO_SCAN2.qn16(), top.sprite_scanner.ANOM_LINE_RSTn, top.sprite_scanner.ELYN_SCAN3.qn16());
    /*p28.FAHA*/ top.sprite_scanner.FAHA_SCAN4.dff17(top.sprite_scanner.ELYN_SCAN3.qn16(), top.sprite_scanner.ANOM_LINE_RSTn, top.sprite_scanner.FAHA_SCAN4.qn16());
    /*p28.FONY*/ top.sprite_scanner.FONY_SCAN5.dff17(top.sprite_scanner.FAHA_SCAN4.qn16(), top.sprite_scanner.ANOM_LINE_RSTn, top.sprite_scanner.FONY_SCAN5.qn16());
  }

  //------------------------------------------------------------------------------
  // lcd_reg.tock();

  /* p01.LYHA*/ wire _LYHA_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);

  /*#p28.AWOH*/ wire _AWOH_xxCDxxGH = not1(top.clk_reg.XUPY_ABxxEFxx);
  /*#p21.SONO*/ wire _SONO_ABxxxxGH = not1(top.clk_reg.TALU_xxCDEFxx);

  /*#p21.XYVO*/ wire _XYVO_IN_VBLANKp = and2(top.lcd_reg.LOVU_Y4p.qp17(), top.lcd_reg.LAFO_Y7p.qp17()); // 128 + 16 = 144
  /*#p29.ALES*/ wire _ALES_IN_VBLANKn = not1(_XYVO_IN_VBLANKp);

  /* p01.LYFE*/ wire _LYFE_LCD_RSTn = not1(_LYHA_VID_RSTp);
  /*#p21.POPU*/ top.lcd_reg.POPU_IN_VBLANKp.dff17(top.lcd_reg.NYPE_LINE_P002.qp17(), _LYFE_LCD_RSTn, _XYVO_IN_VBLANKp);

  /*#p24.KEDY*/ wire _KEDY_LCDC_ENn = not1(top.pix_pipe.XONA_LCDC_LCDENn.qn08());

  {
    // LCD main timer, 912 phases per line

    /*#p21.MUDE*/ wire _MUDE_X_RSTn = nor2(top.lcd_reg.RUTU_LINE_P910.qp17(), _LYHA_VID_RSTp);
    /*#p21.SAXO*/ top.lcd_reg.SAXO_X0p.dff17(top.clk_reg.TALU_xxCDEFxx,   _MUDE_X_RSTn, top.lcd_reg.SAXO_X0p.qn16());
    /*#p21.TYPO*/ top.lcd_reg.TYPO_X1p.dff17(top.lcd_reg.SAXO_X0p.qn16(), _MUDE_X_RSTn, top.lcd_reg.TYPO_X1p.qn16());
    /*#p21.VYZO*/ top.lcd_reg.VYZO_X2p.dff17(top.lcd_reg.TYPO_X1p.qn16(), _MUDE_X_RSTn, top.lcd_reg.VYZO_X2p.qn16());
    /*#p21.TELU*/ top.lcd_reg.TELU_X3p.dff17(top.lcd_reg.VYZO_X2p.qn16(), _MUDE_X_RSTn, top.lcd_reg.TELU_X3p.qn16());
    /*#p21.SUDE*/ top.lcd_reg.SUDE_X4p.dff17(top.lcd_reg.TELU_X3p.qn16(), _MUDE_X_RSTn, top.lcd_reg.SUDE_X4p.qn16());
    /*#p21.TAHA*/ top.lcd_reg.TAHA_X5p.dff17(top.lcd_reg.SUDE_X4p.qn16(), _MUDE_X_RSTn, top.lcd_reg.TAHA_X5p.qn16());
    /*#p21.TYRY*/ top.lcd_reg.TYRY_X6p.dff17(top.lcd_reg.TAHA_X5p.qn16(), _MUDE_X_RSTn, top.lcd_reg.TYRY_X6p.qn16());

    /*#p21.LAMA*/ wire _LAMA_FRAME_RSTn = nor2(top.lcd_reg.MYTA_LINE_153p.qp17(), _LYHA_VID_RSTp);
    /*#p21.MUWY*/ top.lcd_reg.MUWY_Y0p.dff17(top.lcd_reg.RUTU_LINE_P910.qp17(), _LAMA_FRAME_RSTn, top.lcd_reg.MUWY_Y0p.qn16());
    /*#p21.MYRO*/ top.lcd_reg.MYRO_Y1p.dff17(top.lcd_reg.MUWY_Y0p.qn16(),       _LAMA_FRAME_RSTn, top.lcd_reg.MYRO_Y1p.qn16());
    /*#p21.LEXA*/ top.lcd_reg.LEXA_Y2p.dff17(top.lcd_reg.MYRO_Y1p.qn16(),       _LAMA_FRAME_RSTn, top.lcd_reg.LEXA_Y2p.qn16());
    /*#p21.LYDO*/ top.lcd_reg.LYDO_Y3p.dff17(top.lcd_reg.LEXA_Y2p.qn16(),       _LAMA_FRAME_RSTn, top.lcd_reg.LYDO_Y3p.qn16());
    /*#p21.LOVU*/ top.lcd_reg.LOVU_Y4p.dff17(top.lcd_reg.LYDO_Y3p.qn16(),       _LAMA_FRAME_RSTn, top.lcd_reg.LOVU_Y4p.qn16());
    /*#p21.LEMA*/ top.lcd_reg.LEMA_Y5p.dff17(top.lcd_reg.LOVU_Y4p.qn16(),       _LAMA_FRAME_RSTn, top.lcd_reg.LEMA_Y5p.qn16());
    /*#p21.MATO*/ top.lcd_reg.MATO_Y6p.dff17(top.lcd_reg.LEMA_Y5p.qn16(),       _LAMA_FRAME_RSTn, top.lcd_reg.MATO_Y6p.qn16());
    /*#p21.LAFO*/ top.lcd_reg.LAFO_Y7p.dff17(top.lcd_reg.MATO_Y6p.qn16(),       _LAMA_FRAME_RSTn, top.lcd_reg.LAFO_Y7p.qn16());
  }

  {
    /*#p21.SANU*/ wire _SANU_x113p = and4(top.lcd_reg.TYRY_X6p.qp17(), top.lcd_reg.TAHA_X5p.qp17(), top.lcd_reg.SUDE_X4p.qp17(), top.lcd_reg.SAXO_X0p.qp17()); // 113 = 64 + 32 + 16 + 1, schematic is wrong

    /*#p21.RUTU*/ top.lcd_reg.RUTU_LINE_P910.dff17(_SONO_ABxxxxGH, _LYFE_LCD_RSTn, _SANU_x113p);
    /*#p21.NYPE*/ top.lcd_reg.NYPE_LINE_P002.dff17(top.clk_reg.TALU_xxCDEFxx, _LYFE_LCD_RSTn, top.lcd_reg.RUTU_LINE_P910.qp17());

    /*#p29.ABOV*/ wire _ABOV_VID_LINE_P908p = and2(top.lcd_reg.SELA_LINE_P908p, _ALES_IN_VBLANKn);

    /*#p29.CATU*/ top.lcd_reg.CATU_LINE_P000.dff17(top.clk_reg.XUPY_ABxxEFxx, top.clk_reg.ABEZ_VID_RSTn, _ABOV_VID_LINE_P908p);
    /*#p28.ANEL*/ top.lcd_reg.ANEL_LINE_P002.dff17(_AWOH_xxCDxxGH, top.clk_reg.ABEZ_VID_RSTn, top.lcd_reg.CATU_LINE_P000.qp17());

    /*#p21.NOKO*/ wire _NOKO_LINE_153 = and4(top.lcd_reg.LAFO_Y7p.qp17(), top.lcd_reg.LOVU_Y4p.qp17(), top.lcd_reg.LYDO_Y3p.qp17(), top.lcd_reg.MUWY_Y0p.qp17()); // Schematic wrong: NOKO = and2(V7, V4, V3, V0) = 128 + 16 + 8 + 1 = 153
    /*#p21.MYTA*/ top.lcd_reg.MYTA_LINE_153p.dff17(top.lcd_reg.NYPE_LINE_P002.qn16(), _LYFE_LCD_RSTn, _NOKO_LINE_153);
  }

  {
    // if LCDC_ENn, LCD_PIN_ALTSG = 4k div clock. Otherwise LCD_PIN_FR = xor(LINE_EVEN,FRAME_EVEN)

    /*#p24.LOFU*/ wire _LOFU_LINE_ENDn = not1(top.lcd_reg.RUTU_LINE_P910.qp17());
    /*#p24.LUCA*/ top.lcd_reg.LUCA_LINE_EVEN .dff17(_LOFU_LINE_ENDn,                    _LYFE_LCD_RSTn, top.lcd_reg.LUCA_LINE_EVEN.qn16());
    /*#p21.NAPO*/ top.lcd_reg.NAPO_FRAME_EVEN.dff17(top.lcd_reg.POPU_IN_VBLANKp.qp17(), _LYFE_LCD_RSTn, top.lcd_reg.NAPO_FRAME_EVEN.qn16());

    /*#p24.MAGU*/ wire _MAGU = xor2(top.lcd_reg.NAPO_FRAME_EVEN.qp17(), top.lcd_reg.LUCA_LINE_EVEN.qn16());
    /*#p24.MECO*/ wire _MECO = not1(_MAGU);
    /*#p24.KEBO*/ wire _KEBO = not1(_MECO);

    /*#p24.USEC*/ wire _USEC_DIV_07p = not1(top.tim_reg.UREK_DIV_07n);
    /*#p24.KUPA*/ wire _KUPA = amux2(top.pix_pipe.XONA_LCDC_LCDENn.qn08(), _KEBO, _KEDY_LCDC_ENn, _USEC_DIV_07p);
    /*#p24.KOFO*/ wire _KOFO = not1(_KUPA);

    top.PIN_LCD_FLIPS.io_pin(_KOFO, _KOFO);
  }

  {
    /*#p21.TOCU*/ wire _TOCU_C0n = not1(top.lcd_reg.SAXO_X0p.qp17());
    /*#p21.VEPE*/ wire _VEPE_C1n = not1(top.lcd_reg.TYPO_X1p.qp17());
    /* p21.VUTY*/ wire _VUTY_C2n = not1(top.lcd_reg.VYZO_X2p.qp17());
    /* p21.VATE*/ wire _VATE_C3n = not1(top.lcd_reg.TELU_X3p.qp17());
    /* p21.TUDA*/ wire _TUDA_C4n = not1(top.lcd_reg.SUDE_X4p.qp17());
    /* p21.TAFY*/ wire _TAFY_C5n = not1(top.lcd_reg.TAHA_X5p.qp17());
    /* p21.TUJU*/ wire _TUJU_C6n = not1(top.lcd_reg.TYRY_X6p.qp17());

    /* p21.VOKU*/ wire _VOKU_000n = nand7(_TUJU_C6n,       _TAFY_C5n,       _TUDA_C4n,       _VATE_C3n,       _VUTY_C2n,       _VEPE_C1n,       _TOCU_C0n);       // 0000000 == 0
    /* p21.TOZU*/ wire _TOZU_007n = nand7(_TUJU_C6n,       _TAFY_C5n,       _TUDA_C4n,       _VATE_C3n,       top.lcd_reg.VYZO_X2p.qp17(), top.lcd_reg.TYPO_X1p.qp17(), top.lcd_reg.SAXO_X0p.qp17()); // 0000111 == 7
    /* p21.TECE*/ wire _TECE_045n = nand7(_TUJU_C6n,       top.lcd_reg.TAHA_X5p.qp17(), _TUDA_C4n,       top.lcd_reg.TELU_X3p.qp17(), top.lcd_reg.VYZO_X2p.qp17(), _VEPE_C1n,       top.lcd_reg.SAXO_X0p.qp17()); // 0101101 == 45
    /*#p21.TEBO*/ wire _TEBO_083n = nand7(top.lcd_reg.TYRY_X6p.qp17(), _TAFY_C5n,       top.lcd_reg.SUDE_X4p.qp17(), _VATE_C3n,       _VUTY_C2n,       top.lcd_reg.TYPO_X1p.qp17(), top.lcd_reg.SAXO_X0p.qp17()); // 1010011 == 83

    /*#p21.TEGY*/ wire _TEGY_LINE_STROBE = nand4(_VOKU_000n, _TOZU_007n, _TECE_045n, _TEBO_083n);
    /*#p21.SYGU*/ top.lcd_reg.SYGU_LINE_STROBE.dff17(_SONO_ABxxxxGH, _LYFE_LCD_RSTn, _TEGY_LINE_STROBE);

    /*#p21.RYNO*/ wire _RYNO = or2(top.lcd_reg.SYGU_LINE_STROBE.qp17(), top.lcd_reg.RUTU_LINE_P910.qp17());
    /*#p21.POGU*/ wire _POGU = not1(_RYNO);

    top.PIN_LCD_CNTRL.io_pin(_POGU, _POGU);
  }

  {

    /*#p24.KASA*/ wire _KASA_LINE_ENDp = not1(top.lcd_reg.PURE_LINE_P908n);

    /*#p24.UMOB*/ wire UMOB_DIV_06p = not1(top.tim_reg.UMEK_DIV_06n);
    /*#p24.KAHE*/ wire _KAHE_LINE_ENDp = amux2(top.pix_pipe.XONA_LCDC_LCDENn.qn08(), _KASA_LINE_ENDp, _KEDY_LCDC_ENn, UMOB_DIV_06p);
    /*#p24.KYMO*/ wire _KYMO_LINE_ENDn = not1(_KAHE_LINE_ENDp);
    top.PIN_LCD_LATCH.io_pin(_KYMO_LINE_ENDn, _KYMO_LINE_ENDn);
  }

  // LCD vertical sync pin
  {
    // NERU looks a little odd, not 100% positive it's a big nor but it does make sense as one
    /*#p24.NERU*/ wire _NERU_LINE_000p = nor8(top.lcd_reg.LAFO_Y7p.qp17(), top.lcd_reg.LOVU_Y4p.qp17(), top.lcd_reg.LYDO_Y3p.qp17(), top.lcd_reg.MUWY_Y0p.qp17(),
                                              top.lcd_reg.MYRO_Y1p.qp17(), top.lcd_reg.LEXA_Y2p.qp17(), top.lcd_reg.LEMA_Y5p.qp17(), top.lcd_reg.MATO_Y6p.qp17());

    /*#p24.MEDA*/ top.lcd_reg.MEDA_VSYNC_OUTn.dff17(top.lcd_reg.NYPE_LINE_P002.qn16(), _LYFE_LCD_RSTn, _NERU_LINE_000p);
    /*#p24.MURE*/ wire _MURE_VSYNC = not1(top.lcd_reg.MEDA_VSYNC_OUTn.qp17());

    /*#*/ top.PIN_LCD_VSYNC.io_pin(_MURE_VSYNC, _MURE_VSYNC);
  }

  // ly match
  {
    /* p21.RYME*/ wire _RYME_LY_MATCH0n = xor2(top.lcd_reg.MUWY_Y0p.qp17(), top.lcd_reg.SYRY_LYC0n.qn08());
    /* p21.TYDE*/ wire _TYDE_LY_MATCH1n = xor2(top.lcd_reg.MYRO_Y1p.qp17(), top.lcd_reg.VUCE_LYC1n.qn08());
    /* p21.REDA*/ wire _REDA_LY_MATCH2n = xor2(top.lcd_reg.LEXA_Y2p.qp17(), top.lcd_reg.SEDY_LYC2n.qn08());
    /* p21.RASY*/ wire _RASY_LY_MATCH3n = xor2(top.lcd_reg.LYDO_Y3p.qp17(), top.lcd_reg.SALO_LYC3n.qn08());
    /* p21.TYKU*/ wire _TYKU_LY_MATCH4n = xor2(top.lcd_reg.LOVU_Y4p.qp17(), top.lcd_reg.SOTA_LYC4n.qn08());
    /* p21.TUCY*/ wire _TUCY_LY_MATCH5n = xor2(top.lcd_reg.LEMA_Y5p.qp17(), top.lcd_reg.VAFA_LYC5n.qn08());
    /* p21.TERY*/ wire _TERY_LY_MATCH6n = xor2(top.lcd_reg.MATO_Y6p.qp17(), top.lcd_reg.VEVO_LYC6n.qn08());
    /* p21.SYFU*/ wire _SYFU_LY_MATCH7n = xor2(top.lcd_reg.LAFO_Y7p.qp17(), top.lcd_reg.RAHA_LYC7n.qn08());

    /* p21.SOVU*/ wire _SOVU_LY_MATCHA = nor4 (_SYFU_LY_MATCH7n, _TERY_LY_MATCH6n, _TUCY_LY_MATCH5n, _TYKU_LY_MATCH4n); // def nor4
    /* p21.SUBO*/ wire _SUBO_LY_MATCHB = nor4 (_RASY_LY_MATCH3n, _REDA_LY_MATCH2n, _TYDE_LY_MATCH1n, _RYME_LY_MATCH0n); // def nor4
    /* p21.RAPE*/ wire _RAPE_LY_MATCHn = nand2(_SOVU_LY_MATCHA,  _SUBO_LY_MATCHB); // def nand2
    /* p21.PALY*/ wire _PALY_LY_MATCHa = not1(_RAPE_LY_MATCHn); // def not

    //probe(4, "PALY_LY_MATCHA", _PALY_LY_MATCHa);

    /*#p21.ROPO*/ top.lcd_reg.ROPO_LY_MATCH_SYNCp.dff17(top.clk_reg.TALU_xxCDEFxx, top.clk_reg.WESY_SYS_RSTn, _PALY_LY_MATCHa);
  }

  // FF44 LY
  {
    /* p22.WYLE*/ wire _WYLE_FF44n    = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.XOLA_A00n(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /* p22.XOGY*/ wire _XOGY_FF44p    = not1(_WYLE_FF44n);
    /* p23.WAFU*/ wire _WAFU_FF44_RDp = and2(top.ASOT_CPU_RDp, _XOGY_FF44p);
    /* p23.VARO*/ wire _VARO_FF44_RDn = not1(_WAFU_FF44_RDp);

    /*#p23.WURY*/ wire _WURY_LY0n = not1(top.lcd_reg.MUWY_Y0p.qp17());
    /* p23.XEPO*/ wire _XEPO_LY1n = not1(top.lcd_reg.MYRO_Y1p.qp17());
    /* p23.MYFA*/ wire _MYFA_LY2n = not1(top.lcd_reg.LEXA_Y2p.qp17());
    /* p23.XUHY*/ wire _XUHY_LY3n = not1(top.lcd_reg.LYDO_Y3p.qp17());
    /* p23.WATA*/ wire _WATA_LY4n = not1(top.lcd_reg.LOVU_Y4p.qp17());
    /* p23.XAGA*/ wire _XAGA_LY5n = not1(top.lcd_reg.LEMA_Y5p.qp17());
    /* p23.XUCE*/ wire _XUCE_LY6n = not1(top.lcd_reg.MATO_Y6p.qp17());
    /* p23.XOWO*/ wire _XOWO_LY7n = not1(top.lcd_reg.LAFO_Y7p.qp17());

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
    /* p22.WETY*/ wire _WETY_FF45n = nand5(top.cpu_bus.WERO_FF4Xp(), top.cpu_bus.WADO_A00p(), top.cpu_bus.XENO_A01n(), top.cpu_bus.WALO_A02p(), top.cpu_bus.XERA_A03n());
    /* p22.XAYU*/ wire _XAYU_FF45p = not1(_WETY_FF45n);

    /* p23.XYLY*/ wire _XYLY_FF45_RDp = and2(top.ASOT_CPU_RDp, _XAYU_FF45p);
    /* p23.WEKU*/ wire _WEKU_FF45_RDn = not1(_XYLY_FF45_RDp);

    /* p23.XUFA*/ wire _XUFA_FF45_WRn = and2(top.CUPA_CPU_WRp_xxxxEFGx, _XAYU_FF45p);
    /* p23.WANE*/ wire _WANE_FF45_WRp = not1(_XUFA_FF45_WRn);

    /* p23.SYRY*/ top.lcd_reg.SYRY_LYC0n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D0p.qp());
    /* p23.VUCE*/ top.lcd_reg.VUCE_LYC1n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D1p.qp());
    /* p23.SEDY*/ top.lcd_reg.SEDY_LYC2n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p23.SALO*/ top.lcd_reg.SALO_LYC3n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p23.SOTA*/ top.lcd_reg.SOTA_LYC4n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p23.VAFA*/ top.lcd_reg.VAFA_LYC5n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p23.VEVO*/ top.lcd_reg.VEVO_LYC6n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p23.RAHA*/ top.lcd_reg.RAHA_LYC7n.dff9(_WANE_FF45_WRp, top.clk_reg.WESY_SYS_RSTn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p23.RETU*/ cpu_bus.BUS_CPU_D0p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.SYRY_LYC0n.qp09());
    /* p23.VOJO*/ cpu_bus.BUS_CPU_D1p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.VUCE_LYC1n.qp09());
    /* p23.RAZU*/ cpu_bus.BUS_CPU_D2p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.SEDY_LYC2n.qp09());
    /* p23.REDY*/ cpu_bus.BUS_CPU_D3p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.SALO_LYC3n.qp09());
    /* p23.RACE*/ cpu_bus.BUS_CPU_D4p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.SOTA_LYC4n.qp09());
    /*#p23.VAZU*/ cpu_bus.BUS_CPU_D5p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.VAFA_LYC5n.qp09());
    /* p23.VAFE*/ cpu_bus.BUS_CPU_D6p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.VEVO_LYC6n.qp09());
    /* p23.PUFY*/ cpu_bus.BUS_CPU_D7p.tri_6nn(_WEKU_FF45_RDn, top.lcd_reg.RAHA_LYC7n.qp09());
  }

  //------------------------------------------------------------------------------

  /* p01.AMYG*/ wire _AMYG_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);

  /* p29.DYTY*/ wire _DYTY_STORE_ENn_xxCDxxGH = not1(top.sprite_scanner.CARE_STORE_ENp_ABxxEFxx);

  /*#p28.AZYB*/ wire _AZYB_VID_LINE_TRIGn = not1(top.lcd_reg.ATEJ_LINE_TRIGp);
  /* p28.ABAK*/ wire _ABAK_VID_LINE_TRIGp = or2(top.lcd_reg.ATEJ_LINE_TRIGp, _AMYG_VID_RSTp);
  /* p28.BYVA*/ wire _BYVA_VID_LINE_TRIGn = not1(_ABAK_VID_LINE_TRIGp);
  /* p29.DYBA*/ wire _DYBA_VID_LINE_TRIGp = not1(_BYVA_VID_LINE_TRIGn);

  /*#p31.ZAGO*/ wire _ZAGO_X0n = not1(top.oam_bus.YLOR_OAM_DB0p.qp08());
  /* p31.ZOCY*/ wire _ZOCY_X1n = not1(top.oam_bus.ZYTY_OAM_DB1p.qp08());
  /* p31.YPUR*/ wire _YPUR_X2n = not1(top.oam_bus.ZYVE_OAM_DB2p.qp08());
  /* p31.YVOK*/ wire _YVOK_X3n = not1(top.oam_bus.ZEZY_OAM_DB3p.qp08());
  /* p31.COSE*/ wire _COSE_X4n = not1(top.oam_bus.GOMO_OAM_DB4p.qp08());
  /* p31.AROP*/ wire _AROP_X5n = not1(top.oam_bus.BAXO_OAM_DB5p.qp08());
  /* p31.XATU*/ wire _XATU_X6n = not1(top.oam_bus.YZOS_OAM_DB6p.qp08());
  /* p31.BADY*/ wire _BADY_X7n = not1(top.oam_bus.DEPO_OAM_DB7p.qp08());

  //----------------------------------------
  // Sprite scan Y matcher

  {
    // Sprite store counter.
    // The sprite count clock stops ticking once we have 10 sprites.

    /*#p29.BAKY*/ wire _BAKY_SPRITES_FULL = and2(top.sprite_store.CUXY_SPRITE_COUNT1.qp17(), top.sprite_store.DYBE_SPRITE_COUNT3.qp17());
    /*#p29.CAKE*/ wire _CAKE_CLKp = or2(_BAKY_SPRITES_FULL, top.sprite_store.DEZY_STORE_ENn.qp17());

    /*p29.BESE*/ top.sprite_store.BESE_SPRITE_COUNT0.dff17(_CAKE_CLKp,                                 _AZYB_VID_LINE_TRIGn, top.sprite_store.BESE_SPRITE_COUNT0.qn16());
    /*p29.CUXY*/ top.sprite_store.CUXY_SPRITE_COUNT1.dff17(top.sprite_store.BESE_SPRITE_COUNT0.qn16(), _AZYB_VID_LINE_TRIGn, top.sprite_store.CUXY_SPRITE_COUNT1.qn16());
    /*p29.BEGO*/ top.sprite_store.BEGO_SPRITE_COUNT2.dff17(top.sprite_store.CUXY_SPRITE_COUNT1.qn16(), _AZYB_VID_LINE_TRIGn, top.sprite_store.BEGO_SPRITE_COUNT2.qn16());
    /*p29.DYBE*/ top.sprite_store.DYBE_SPRITE_COUNT3.dff17(top.sprite_store.BEGO_SPRITE_COUNT2.qn16(), _AZYB_VID_LINE_TRIGn, top.sprite_store.DYBE_SPRITE_COUNT3.qn16());
  }

  //----------------------------------------

  {
    // FEPO_STORE_MATCHp here is weird, I guess it's just an easy signal to use to mux the bus?

    /*#p30.CUCU*/ top.sprite_store.SPR_TRI_L0.tri_6nn(top.sprite_store.FEPO_STORE_MATCHp, top.sprite_scanner.DEGE_SPRITE_DELTA0);
    /*#p30.CUCA*/ top.sprite_store.SPR_TRI_L1.tri_6nn(top.sprite_store.FEPO_STORE_MATCHp, top.sprite_scanner.DABY_SPRITE_DELTA1);
    /*#p30.CEGA*/ top.sprite_store.SPR_TRI_L2.tri_6nn(top.sprite_store.FEPO_STORE_MATCHp, top.sprite_scanner.DABU_SPRITE_DELTA2);
    /*#p30.WENU*/ top.sprite_store.SPR_TRI_L3.tri_6nn(top.sprite_store.FEPO_STORE_MATCHp, top.sprite_scanner.GYSA_SPRITE_DELTA3);

    /* p29.DEZY*/ top.sprite_store.DEZY_STORE_ENn.dff17(top.clk_reg.ZEME_AxCxExGx, top.clk_reg.XAPO_VID_RSTn, _DYTY_STORE_ENn_xxCDxxGH);
  }

  //----------------------------------------
  // Sprite store getter

  {
    /*p29.WEFU*/ wire WEFU_STORE0_MATCH = not1(top.sprite_store.YDUG_STORE0_MATCHn);
    /*p29.GAJA*/ wire GAJA_STORE1_MATCH = not1(top.sprite_store.DYDU_STORE1_MATCHn);
    /*p29.GUPO*/ wire GUPO_STORE2_MATCH = not1(top.sprite_store.DEGO_STORE2_MATCHn);
    /*p29.WEBO*/ wire WEBO_STORE3_MATCH = not1(top.sprite_store.YLOZ_STORE3_MATCHn);
    /*p29.WUNA*/ wire WUNA_STORE4_MATCH = not1(top.sprite_store.XAGE_STORE4_MATCHn);
    /*p29.GABA*/ wire GABA_STORE5_MATCH = not1(top.sprite_store.EGOM_STORE5_MATCHn);
    /*p29.WASE*/ wire WASE_STORE6_MATCH = not1(top.sprite_store.YBEZ_STORE6_MATCHn);
    /*p29.GYTE*/ wire GYTE_STORE7_MATCH = not1(top.sprite_store.DYKA_STORE7_MATCHn);
    /*p29.GEKE*/ wire GEKE_STORE8_MATCH = not1(top.sprite_store.EFYL_STORE8_MATCHn);

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

    /*p29.GUVA*/ wire GUVA_SPRITE0_GETp = nor2(top.sprite_store.YDUG_STORE0_MATCHn, GND);
    /*p29.ENUT*/ wire ENUT_SPRITE1_GETp = nor2(top.sprite_store.DYDU_STORE1_MATCHn, GEZE_STORE0_MATCH_OUT);
    /*p29.EMOL*/ wire EMOL_SPRITE2_GETp = nor2(top.sprite_store.DEGO_STORE2_MATCHn, FUMA_STORE1_MATCH_OUT);
    /*p29.GYFY*/ wire GYFY_SPRITE3_GETp = nor2(top.sprite_store.YLOZ_STORE3_MATCHn, GEDE_STORE2_MATCH_OUT);
    /*p29.GONO*/ wire GONO_SPRITE4_GETp = nor2(top.sprite_store.XAGE_STORE4_MATCHn, WUTO_STORE3_MATCH_OUT);
    /*p29.GEGA*/ wire GEGA_SPRITE5_GETp = nor2(top.sprite_store.EGOM_STORE5_MATCHn, XYLA_STORE4_MATCH_OUT);
    /*p29.XOJA*/ wire XOJA_SPRITE6_GETp = nor2(top.sprite_store.YBEZ_STORE6_MATCHn, WEJA_STORE5_MATCH_OUT);
    /*p29.GUTU*/ wire GUTU_SPRITE7_GETp = nor2(top.sprite_store.DYKA_STORE7_MATCHn, WYLA_STORE6_MATCH_OUT);
    /*p29.FOXA*/ wire FOXA_SPRITE8_GETp = nor2(top.sprite_store.EFYL_STORE8_MATCHn, FAVO_STORE7_MATCH_OUT);
    /*p29.GUZE*/ wire GUZE_SPRITE9_GETp = nor2(top.sprite_store.YGEM_STORE9_MATCHn, GYGA_STORE8_MATCH_OUT);

    // Delayed reset signal for the selected store once sprite fetch is done.

    /*p29.EBOJ*/ top.sprite_store.EBOJ_STORE0_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GUVA_SPRITE0_GETp);
    /*p29.CEDY*/ top.sprite_store.CEDY_STORE1_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, ENUT_SPRITE1_GETp);
    /*p29.EGAV*/ top.sprite_store.EGAV_STORE2_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, EMOL_SPRITE2_GETp);
    /*p29.GOTA*/ top.sprite_store.GOTA_STORE3_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GYFY_SPRITE3_GETp);
    /*p29.XUDY*/ top.sprite_store.XUDY_STORE4_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GONO_SPRITE4_GETp);
    /*p29.WAFY*/ top.sprite_store.WAFY_STORE5_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GEGA_SPRITE5_GETp);
    /*p29.WOMY*/ top.sprite_store.WOMY_STORE6_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, XOJA_SPRITE6_GETp);
    /*p29.WAPO*/ top.sprite_store.WAPO_STORE7_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GUTU_SPRITE7_GETp);
    /*p29.EXUQ*/ top.sprite_store.EXUQ_STORE8_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, FOXA_SPRITE8_GETp);
    /*p29.FONO*/ top.sprite_store.FONO_STORE9_RSTp.dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp, _BYVA_VID_LINE_TRIGn, GUZE_SPRITE9_GETp);

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

    /* p30.ZETU*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.YGUS_STORE0_I0n.qp08());
    /* p30.ZECE*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.YSOK_STORE0_I1n.qp08());
    /* p30.ZAVE*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.YZEP_STORE0_I2n.qp08());
    /* p30.WOKO*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.WYTE_STORE0_I3n.qp08());
    /* p30.ZUMU*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.ZONY_STORE0_I4n.qp08());
    /*#p30.ZEDY*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.YWAK_STORE0_I5n.qp08());

    /*#p30.CUBO*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.CADU_STORE1_I0n.qp08());
    /* p30.CELU*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.CEBO_STORE1_I1n.qp08());
    /* p30.CEGY*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.CUFA_STORE1_I2n.qp08());
    /* p30.BETY*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.COMA_STORE1_I3n.qp08());
    /* p30.CYBY*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.CUZA_STORE1_I4n.qp08());
    /* p30.BEMO*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.CAJY_STORE1_I5n.qp08());

    /* p30.CUBE*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.BUHE_STORE2_I0n.qp08());
    /* p30.AFOZ*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.BYHU_STORE2_I1n.qp08());
    /* p30.APON*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.BECA_STORE2_I2n.qp08());
    /* p30.CUVU*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.BULU_STORE2_I3n.qp08());
    /* p30.CYRO*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.BUNA_STORE2_I4n.qp08());
    /* p30.AXEC*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.BOXA_STORE2_I5n.qp08());

    /* p30.ENAP*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.DEVY_STORE3_I0n.qp08());
    /* p30.DYGO*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.DESE_STORE3_I1n.qp08());
    /* p30.DOWA*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.DUNY_STORE3_I2n.qp08());
    /* p30.DONY*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.DUHA_STORE3_I3n.qp08());
    /* p30.EFUD*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.DEBA_STORE3_I4n.qp08());
    /* p30.DEZU*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.DAFU_STORE3_I5n.qp08());

    /* p30.WUXU*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.XAVE_STORE4_I0n.qp08());
    /* p30.WEPY*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.XEFE_STORE4_I1n.qp08());
    /* p30.WERU*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.WANU_STORE4_I2n.qp08());
    /* p30.XYRE*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.XABO_STORE4_I3n.qp08());
    /* p30.WOXY*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.XEGE_STORE4_I4n.qp08());
    /* p30.WAJA*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.XYNU_STORE4_I5n.qp08());

    /* p30.DOBO*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.EKOP_STORE5_I0n.qp08());
    /* p30.DYNY*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.ETYM_STORE5_I1n.qp08());
    /* p30.WAGA*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.GORU_STORE5_I2n.qp08());
    /* p30.DUZA*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.EBEX_STORE5_I3n.qp08());
    /* p30.DALY*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.ETAV_STORE5_I4n.qp08());
    /* p30.DALO*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.EKAP_STORE5_I5n.qp08());

    /* p30.WATO*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.GABO_STORE6_I0n.qp08());
    /* p30.WYWY*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.GACY_STORE6_I1n.qp08());
    /* p30.EZOC*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.FOGO_STORE6_I2n.qp08());
    /* p30.WABO*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.GOHU_STORE6_I3n.qp08());
    /* p30.ELYC*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.FOXY_STORE6_I4n.qp08());
    /* p30.WOCY*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.GECU_STORE6_I5n.qp08());

    /* p30.WAKO*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.GULE_STORE7_I0n.qp08());
    /* p30.WYGO*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.GYNO_STORE7_I1n.qp08());
    /* p30.ELEP*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.FEFA_STORE7_I2n.qp08());
    /* p30.ETAD*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.FYSU_STORE7_I3n.qp08());
    /* p30.WABA*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.GESY_STORE7_I4n.qp08());
    /* p30.EVYT*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.FUZO_STORE7_I5n.qp08());

    /* p30.APOC*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.AXUV_STORE8_I0n.qp08());
    /* p30.AKYH*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.BADA_STORE8_I1n.qp08());
    /* p30.AFEN*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.APEV_STORE8_I2n.qp08());
    /* p30.APYV*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.BADO_STORE8_I3n.qp08());
    /* p30.APOB*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.BEXY_STORE8_I4n.qp08());
    /* p30.ADYB*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.BYHE_STORE8_I5n.qp08());

    /* p30.ZARO*/ top.sprite_store.SPR_TRI_I0p.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.YBER_STORE9_I0n.qp08());
    /* p30.ZOJY*/ top.sprite_store.SPR_TRI_I1p.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.YZOR_STORE9_I1n.qp08());
    /* p30.YNEV*/ top.sprite_store.SPR_TRI_I2p.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.XYFE_STORE9_I2n.qp08());
    /* p30.XYRA*/ top.sprite_store.SPR_TRI_I3p.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.XOTU_STORE9_I3n.qp08());
    /* p30.YRAD*/ top.sprite_store.SPR_TRI_I4p.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.XUTE_STORE9_I4n.qp08());
    /* p30.YHAL*/ top.sprite_store.SPR_TRI_I5p.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.XUFO_STORE9_I5n.qp08());

    /* p30.WEHE*/ top.sprite_store.SPR_TRI_L0.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.GYHO_STORE0_L0n.qp08());
    /* p30.BUKY*/ top.sprite_store.SPR_TRI_L1.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.CUFO_STORE0_L1n.qp08());
    /* p30.AJAL*/ top.sprite_store.SPR_TRI_L2.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.BOZU_STORE0_L2n.qp08());
    /* p30.GOFO*/ top.sprite_store.SPR_TRI_L3.tri_6nn(FURO_SPRITE0_GETn, top.sprite_store.FYHY_STORE0_L3n.qp08());

    /* p30.BYRO*/ top.sprite_store.SPR_TRI_L0.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.AMES_STORE1_L0n.qp08());
    /* p30.AHUM*/ top.sprite_store.SPR_TRI_L1.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.AROF_STORE1_L1n.qp08());
    /* p30.BACO*/ top.sprite_store.SPR_TRI_L2.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.ABOP_STORE1_L2n.qp08());
    /* p30.BEFE*/ top.sprite_store.SPR_TRI_L3.tri_6nn(DYDO_SPRITE1_GETn, top.sprite_store.ABUG_STORE1_L3n.qp08());

    /* p30.ZUKE*/ top.sprite_store.SPR_TRI_L0.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.YLOV_STORE2_L0n.qp08());
    /* p30.WERE*/ top.sprite_store.SPR_TRI_L1.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.XOSY_STORE2_L1n.qp08());
    /* p30.WUXE*/ top.sprite_store.SPR_TRI_L2.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.XAZY_STORE2_L2n.qp08());
    /* p30.ZABY*/ top.sprite_store.SPR_TRI_L3.tri_6nn(FAME_SPRITE2_GETn, top.sprite_store.YKUK_STORE2_L3n.qp08());

    /* p30.ZEXE*/ top.sprite_store.SPR_TRI_L0.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.ZURO_STORE3_L0n.qp08());
    /* p30.YWAV*/ top.sprite_store.SPR_TRI_L1.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.ZYLU_STORE3_L1n.qp08());
    /* p30.YJEM*/ top.sprite_store.SPR_TRI_L2.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.ZENE_STORE3_L2n.qp08());
    /* p30.ZYPO*/ top.sprite_store.SPR_TRI_L3.tri_6nn(GYMA_SPRITE3_GETn, top.sprite_store.ZURY_STORE3_L3n.qp08());

    /* p30.BUCE*/ top.sprite_store.SPR_TRI_L0.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.CAPO_STORE4_L0n.qp08());
    /* p30.BEVY*/ top.sprite_store.SPR_TRI_L1.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.CAJU_STORE4_L1n.qp08());
    /* p30.BOVE*/ top.sprite_store.SPR_TRI_L2.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.CONO_STORE4_L2n.qp08());
    /* p30.BYDO*/ top.sprite_store.SPR_TRI_L3.tri_6nn(GOWO_SPRITE4_GETn, top.sprite_store.CUMU_STORE4_L3n.qp08());

    /* p30.BACE*/ top.sprite_store.SPR_TRI_L0.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.ACEP_STORE5_L0n.qp08());
    /* p30.BUJA*/ top.sprite_store.SPR_TRI_L1.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.ABEG_STORE5_L1n.qp08());
    /* p30.BODU*/ top.sprite_store.SPR_TRI_L2.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.ABUX_STORE5_L2n.qp08());
    /* p30.AWAT*/ top.sprite_store.SPR_TRI_L3.tri_6nn(GYGY_SPRITE5_GETn, top.sprite_store.ANED_STORE5_L3n.qp08());

    /* p30.YBUK*/ top.sprite_store.SPR_TRI_L0.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.ZUMY_STORE6_L0n.qp08());
    /* p30.YKOZ*/ top.sprite_store.SPR_TRI_L1.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.ZAFU_STORE6_L1n.qp08());
    /* p30.ZYTO*/ top.sprite_store.SPR_TRI_L2.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.ZEXO_STORE6_L2n.qp08());
    /* p30.ZUDO*/ top.sprite_store.SPR_TRI_L3.tri_6nn(XYME_SPRITE6_GETn, top.sprite_store.ZUBE_STORE6_L3n.qp08());

    /* p30.WAXE*/ top.sprite_store.SPR_TRI_L0.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.XYNA_STORE7_L0n.qp08());
    /* p30.YPOZ*/ top.sprite_store.SPR_TRI_L1.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.YGUM_STORE7_L1n.qp08());
    /* p30.WABU*/ top.sprite_store.SPR_TRI_L2.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.XAKU_STORE7_L2n.qp08());
    /* p30.WANA*/ top.sprite_store.SPR_TRI_L3.tri_6nn(GUGY_SPRITE7_GETn, top.sprite_store.XYGO_STORE7_L3n.qp08());

    /* p30.BOSO*/ top.sprite_store.SPR_TRI_L0.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.AZAP_STORE8_L0n.qp08());
    /* p30.BAZU*/ top.sprite_store.SPR_TRI_L1.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.AFYX_STORE8_L1n.qp08());
    /* p30.AHAC*/ top.sprite_store.SPR_TRI_L2.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.AFUT_STORE8_L2n.qp08());
    /* p30.BUJY*/ top.sprite_store.SPR_TRI_L3.tri_6nn(DENY_SPRITE8_GETn, top.sprite_store.AFYM_STORE8_L3n.qp08());

    /* p30.BYME*/ top.sprite_store.SPR_TRI_L0.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.CANA_STORE9_L0n.qp08());
    /* p30.GATE*/ top.sprite_store.SPR_TRI_L1.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.FOFO_STORE9_L1n.qp08());
    /* p30.COHO*/ top.sprite_store.SPR_TRI_L2.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.DYSY_STORE9_L2n.qp08());
    /* p30.CAWO*/ top.sprite_store.SPR_TRI_L3.tri_6nn(FADO_SPRITE9_GETn, top.sprite_store.DEWU_STORE9_L3n.qp08());
  }

  //----------------------------------------
  // Sprite store setter

  {
    /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.EBOJ_STORE0_RSTp.qp17());
    /*p29.EFEV*/ wire EFEV_STORE1_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.CEDY_STORE1_RSTp.qp17());
    /*p29.FOKO*/ wire FOKO_STORE2_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.EGAV_STORE2_RSTp.qp17());
    /*p29.GAKE*/ wire GAKE_STORE3_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.GOTA_STORE3_RSTp.qp17());
    /*p29.WOHU*/ wire WOHU_STORE4_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.XUDY_STORE4_RSTp.qp17());
    /*p29.FEVE*/ wire FEVE_STORE5_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.WAFY_STORE5_RSTp.qp17());
    /*p29.WACY*/ wire WACY_STORE6_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.WOMY_STORE6_RSTp.qp17());
    /*p29.GUKY*/ wire GUKY_STORE7_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.WAPO_STORE7_RSTp.qp17());
    /*p29.GORO*/ wire GORO_STORE8_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.EXUQ_STORE8_RSTp.qp17());
    /*p29.DUBU*/ wire DUBU_STORE9_RSTp = or2(_DYBA_VID_LINE_TRIGp, top.sprite_store.FONO_STORE9_RSTp.qp17());

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

    /*p29.EDEN*/ wire EDEN_SPRITE_COUNT0n = not1(top.sprite_store.BESE_SPRITE_COUNT0.qp17());
    /*p29.CYPY*/ wire CYPY_SPRITE_COUNT1n = not1(top.sprite_store.CUXY_SPRITE_COUNT1.qp17());
    /*p29.CAPE*/ wire CAPE_SPRITE_COUNT2n = not1(top.sprite_store.BEGO_SPRITE_COUNT2.qp17());
    /*p29.CAXU*/ wire CAXU_SPRITE_COUNT3n = not1(top.sprite_store.DYBE_SPRITE_COUNT3.qp17());

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

      /*p30.YGUS*/ top.sprite_store.YGUS_STORE0_I0n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, top.sprite_store.SPR_TRI_I0p.qp());
      /*p30.YSOK*/ top.sprite_store.YSOK_STORE0_I1n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, top.sprite_store.SPR_TRI_I1p.qp());
      /*p30.YZEP*/ top.sprite_store.YZEP_STORE0_I2n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, top.sprite_store.SPR_TRI_I2p.qp());
      /*p30.WYTE*/ top.sprite_store.WYTE_STORE0_I3n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, top.sprite_store.SPR_TRI_I3p.qp());
      /*p30.ZONY*/ top.sprite_store.ZONY_STORE0_I4n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, top.sprite_store.SPR_TRI_I4p.qp());
      /*p30.YWAK*/ top.sprite_store.YWAK_STORE0_I5n.dff8n(GENY_STORE0_CLKp, GENY_STORE0_CLKn, top.sprite_store.SPR_TRI_I5p.qp());

      /*p29.ENOB*/ wire ENOB_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
      /*p29.ENOB*/ wire ENOB_STORE0_CLKn = not1(ENOB_STORE0_CLKp);

      /*p30.GYHO*/ top.sprite_store.GYHO_STORE0_L0n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, top.sprite_store.SPR_TRI_L0.qp());
      /*p30.CUFO*/ top.sprite_store.CUFO_STORE0_L1n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, top.sprite_store.SPR_TRI_L1.qp());
      /*p30.BOZU*/ top.sprite_store.BOZU_STORE0_L2n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, top.sprite_store.SPR_TRI_L2.qp());
      /*p30.FYHY*/ top.sprite_store.FYHY_STORE0_L3n.dff8n(ENOB_STORE0_CLKp, ENOB_STORE0_CLKn, top.sprite_store.SPR_TRI_L3.qp());

      /*p29.FUXU*/ wire FUXU_STORE0_CLKp = not1(DYHU_STORE0_CLKn);
      /*p29.FUXU*/ wire FUXU_STORE0_CLKn = not1(FUXU_STORE0_CLKp);

      /*#p31.XEPE*/ top.sprite_store.XEPE_STORE0_X0p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _ZAGO_X0n);
      /* p31.YLAH*/ top.sprite_store.YLAH_STORE0_X1p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _ZOCY_X1n);
      /* p31.ZOLA*/ top.sprite_store.ZOLA_STORE0_X2p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _YPUR_X2n);
      /* p31.ZULU*/ top.sprite_store.ZULU_STORE0_X3p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _YVOK_X3n);
      /* p31.WELO*/ top.sprite_store.WELO_STORE0_X4p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _COSE_X4n);
      /* p31.XUNY*/ top.sprite_store.XUNY_STORE0_X5p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _AROP_X5n);
      /* p31.WOTE*/ top.sprite_store.WOTE_STORE0_X6p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _XATU_X6n);
      /* p31.XAKO*/ top.sprite_store.XAKO_STORE0_X7p.dff9(FUXU_STORE0_CLKp, FUXU_STORE0_CLKn, DYNA_STORE0_RSTn, _BADY_X7n);
    }

    {
      /*p29.BYVY*/ wire BYVY_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /*p29.BYVY*/ wire BYVY_STORE1_CLKn = not1(BYVY_STORE1_CLKp);

      /*p30.CADU*/ top.sprite_store.CADU_STORE1_I0n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, top.sprite_store.SPR_TRI_I0p.qp());
      /*p30.CEBO*/ top.sprite_store.CEBO_STORE1_I1n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, top.sprite_store.SPR_TRI_I1p.qp());
      /*p30.CUFA*/ top.sprite_store.CUFA_STORE1_I2n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, top.sprite_store.SPR_TRI_I2p.qp());
      /*p30.COMA*/ top.sprite_store.COMA_STORE1_I3n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, top.sprite_store.SPR_TRI_I3p.qp());
      /*p30.CUZA*/ top.sprite_store.CUZA_STORE1_I4n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, top.sprite_store.SPR_TRI_I4p.qp());
      /*p30.CAJY*/ top.sprite_store.CAJY_STORE1_I5n.dff8n(BYVY_STORE1_CLKp, BYVY_STORE1_CLKn, top.sprite_store.SPR_TRI_I5p.qp());

      /*p29.AHOF*/ wire AHOF_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /*p29.AHOF*/ wire AHOF_STORE1_CLKn = not1(AHOF_STORE1_CLKp);

      /*p30.AMES*/ top.sprite_store.AMES_STORE1_L0n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, top.sprite_store.SPR_TRI_L0.qp());
      /*p30.AROF*/ top.sprite_store.AROF_STORE1_L1n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, top.sprite_store.SPR_TRI_L1.qp());
      /*p30.ABOP*/ top.sprite_store.ABOP_STORE1_L2n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, top.sprite_store.SPR_TRI_L2.qp());
      /*p30.ABUG*/ top.sprite_store.ABUG_STORE1_L3n.dff8n(AHOF_STORE1_CLKp, AHOF_STORE1_CLKn, top.sprite_store.SPR_TRI_L3.qp());

      /*p29.ASYS*/ wire ASYS_STORE1_CLKp = not1(BUCO_STORE1_CLKn);
      /*p29.ASYS*/ wire ASYS_STORE1_CLKn = not1(ASYS_STORE1_CLKp);

      /*p31.DANY*/ top.sprite_store.DANY_STORE1_X0p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _ZAGO_X0n);
      /*p31.DUKO*/ top.sprite_store.DUKO_STORE1_X1p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _ZOCY_X1n);
      /*p31.DESU*/ top.sprite_store.DESU_STORE1_X2p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _YPUR_X2n);
      /*p31.DAZO*/ top.sprite_store.DAZO_STORE1_X3p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _YVOK_X3n);
      /*p31.DAKE*/ top.sprite_store.DAKE_STORE1_X4p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _COSE_X4n);
      /*p31.CESO*/ top.sprite_store.CESO_STORE1_X5p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _AROP_X5n);
      /*p31.DYFU*/ top.sprite_store.DYFU_STORE1_X6p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _XATU_X6n);
      /*p31.CUSY*/ top.sprite_store.CUSY_STORE1_X7p.dff9(ASYS_STORE1_CLKp, ASYS_STORE1_CLKn, DOKU_STORE1_RSTn, _BADY_X7n);
    }

    /*p29.BUZY*/ wire BUZY_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.FUKE*/ wire FUKE_STORE2_CLKp = not1(GYFO_STORE2_CLKn);
    /*p29.CACU*/ wire CACU_STORE2_CLKp = not1(GYFO_STORE2_CLKn);

    /*p30.BUHE*/ top.sprite_store.BUHE_STORE2_I0n.dff8n(BUZY_STORE2_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.BYHU*/ top.sprite_store.BYHU_STORE2_I1n.dff8n(BUZY_STORE2_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.BECA*/ top.sprite_store.BECA_STORE2_I2n.dff8n(BUZY_STORE2_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.BULU*/ top.sprite_store.BULU_STORE2_I3n.dff8n(BUZY_STORE2_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.BUNA*/ top.sprite_store.BUNA_STORE2_I4n.dff8n(BUZY_STORE2_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.BOXA*/ top.sprite_store.BOXA_STORE2_I5n.dff8n(BUZY_STORE2_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.YLOV*/ top.sprite_store.YLOV_STORE2_L0n.dff8n(FUKE_STORE2_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.XOSY*/ top.sprite_store.XOSY_STORE2_L1n.dff8n(FUKE_STORE2_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.XAZY*/ top.sprite_store.XAZY_STORE2_L2n.dff8n(FUKE_STORE2_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.YKUK*/ top.sprite_store.YKUK_STORE2_L3n.dff8n(FUKE_STORE2_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.FOKA*/ top.sprite_store.FOKA_STORE2_X0p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _ZAGO_X0n);
    /*p31.FYTY*/ top.sprite_store.FYTY_STORE2_X1p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _ZOCY_X1n);
    /*p31.FUBY*/ top.sprite_store.FUBY_STORE2_X2p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _YPUR_X2n);
    /*p31.GOXU*/ top.sprite_store.GOXU_STORE2_X3p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _YVOK_X3n);
    /*p31.DUHY*/ top.sprite_store.DUHY_STORE2_X4p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _COSE_X4n);
    /*p31.EJUF*/ top.sprite_store.EJUF_STORE2_X5p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _AROP_X5n);
    /*p31.ENOR*/ top.sprite_store.ENOR_STORE2_X6p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _XATU_X6n);
    /*p31.DEPY*/ top.sprite_store.DEPY_STORE2_X7p.dff9(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, _BADY_X7n);

    /*p29.FEKA*/ wire FEKA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
    /*p29.XYHA*/ wire XYHA_STORE3_CLKp = not1(GUSA_STORE3_CLKn);
    /*p29.YFAG*/ wire YFAG_STORE3_CLKp = not1(GUSA_STORE3_CLKn);

    /*p30.DEVY*/ top.sprite_store.DEVY_STORE3_I0n.dff8n(FEKA_STORE3_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.DESE*/ top.sprite_store.DESE_STORE3_I1n.dff8n(FEKA_STORE3_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.DUNY*/ top.sprite_store.DUNY_STORE3_I2n.dff8n(FEKA_STORE3_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.DUHA*/ top.sprite_store.DUHA_STORE3_I3n.dff8n(FEKA_STORE3_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.DEBA*/ top.sprite_store.DEBA_STORE3_I4n.dff8n(FEKA_STORE3_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.DAFU*/ top.sprite_store.DAFU_STORE3_I5n.dff8n(FEKA_STORE3_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.ZURO*/ top.sprite_store.ZURO_STORE3_L0n.dff8n(XYHA_STORE3_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.ZYLU*/ top.sprite_store.ZYLU_STORE3_L1n.dff8n(XYHA_STORE3_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.ZENE*/ top.sprite_store.ZENE_STORE3_L2n.dff8n(XYHA_STORE3_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.ZURY*/ top.sprite_store.ZURY_STORE3_L3n.dff8n(XYHA_STORE3_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.XOLY*/ top.sprite_store.XOLY_STORE3_X0p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _ZAGO_X0n);
    /*p31.XYBA*/ top.sprite_store.XYBA_STORE3_X1p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _ZOCY_X1n);
    /*p31.XABE*/ top.sprite_store.XABE_STORE3_X2p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _YPUR_X2n);
    /*p31.XEKA*/ top.sprite_store.XEKA_STORE3_X3p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _YVOK_X3n);
    /*p31.XOMY*/ top.sprite_store.XOMY_STORE3_X4p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _COSE_X4n);
    /*p31.WUHA*/ top.sprite_store.WUHA_STORE3_X5p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _AROP_X5n);
    /*p31.WYNA*/ top.sprite_store.WYNA_STORE3_X6p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _XATU_X6n);
    /*p31.WECO*/ top.sprite_store.WECO_STORE3_X7p.dff9(YFAG_STORE3_CLKp, WUPA_STORE3_RSTn, _BADY_X7n);

    /*p29.WOFO*/ wire WOFO_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
    /*p29.WYLU*/ wire WYLU_STORE4_CLKp = not1(DUKE_STORE4_CLKn);
    /*p29.EWOT*/ wire EWOT_STORE4_CLKp = not1(DUKE_STORE4_CLKn);

    /*p30.XAVE*/ top.sprite_store.XAVE_STORE4_I0n.dff8n(WYLU_STORE4_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.XEFE*/ top.sprite_store.XEFE_STORE4_I1n.dff8n(WYLU_STORE4_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.WANU*/ top.sprite_store.WANU_STORE4_I2n.dff8n(WYLU_STORE4_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.XABO*/ top.sprite_store.XABO_STORE4_I3n.dff8n(WYLU_STORE4_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.XEGE*/ top.sprite_store.XEGE_STORE4_I4n.dff8n(WYLU_STORE4_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.XYNU*/ top.sprite_store.XYNU_STORE4_I5n.dff8n(WYLU_STORE4_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.CAPO*/ top.sprite_store.CAPO_STORE4_L0n.dff8n(EWOT_STORE4_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.CAJU*/ top.sprite_store.CAJU_STORE4_L1n.dff8n(EWOT_STORE4_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.CONO*/ top.sprite_store.CONO_STORE4_L2n.dff8n(EWOT_STORE4_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.CUMU*/ top.sprite_store.CUMU_STORE4_L3n.dff8n(EWOT_STORE4_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.WEDU*/ top.sprite_store.WEDU_STORE4_X0p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _ZAGO_X0n);
    /*p31.YGAJ*/ top.sprite_store.YGAJ_STORE4_X1p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _ZOCY_X1n);
    /*p31.ZYJO*/ top.sprite_store.ZYJO_STORE4_X2p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _YPUR_X2n);
    /*p31.XURY*/ top.sprite_store.XURY_STORE4_X3p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _YVOK_X3n);
    /*p31.YBED*/ top.sprite_store.YBED_STORE4_X4p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _COSE_X4n);
    /*p31.ZALA*/ top.sprite_store.ZALA_STORE4_X5p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _AROP_X5n);
    /*p31.WYDE*/ top.sprite_store.WYDE_STORE4_X6p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _XATU_X6n);
    /*p31.XEPA*/ top.sprite_store.XEPA_STORE4_X7p.dff9(WOFO_STORE4_CLKp, WUNU_STORE4_RSTn, _BADY_X7n);

    /*p29.CYLA*/ wire CYLA_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
    /*p29.DYMO*/ wire DYMO_STORE5_CLKp = not1(BEDE_STORE5_CLKn);
    /*p29.BUCY*/ wire BUCY_STORE5_CLKp = not1(BEDE_STORE5_CLKn);

    /*p30.EKOP*/ top.sprite_store.EKOP_STORE5_I0n.dff8n(DYMO_STORE5_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.ETYM*/ top.sprite_store.ETYM_STORE5_I1n.dff8n(DYMO_STORE5_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.GORU*/ top.sprite_store.GORU_STORE5_I2n.dff8n(DYMO_STORE5_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.EBEX*/ top.sprite_store.EBEX_STORE5_I3n.dff8n(DYMO_STORE5_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.ETAV*/ top.sprite_store.ETAV_STORE5_I4n.dff8n(DYMO_STORE5_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.EKAP*/ top.sprite_store.EKAP_STORE5_I5n.dff8n(DYMO_STORE5_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.ACEP*/ top.sprite_store.ACEP_STORE5_L0n.dff8n(BUCY_STORE5_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.ABEG*/ top.sprite_store.ABEG_STORE5_L1n.dff8n(BUCY_STORE5_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.ABUX*/ top.sprite_store.ABUX_STORE5_L2n.dff8n(BUCY_STORE5_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.ANED*/ top.sprite_store.ANED_STORE5_L3n.dff8n(BUCY_STORE5_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.FUSA*/ top.sprite_store.FUSA_STORE5_X0p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _ZAGO_X0n);
    /*p31.FAXA*/ top.sprite_store.FAXA_STORE5_X1p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _ZOCY_X1n);
    /*p31.FOZY*/ top.sprite_store.FOZY_STORE5_X2p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _YPUR_X2n);
    /*p31.FESY*/ top.sprite_store.FESY_STORE5_X3p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _YVOK_X3n);
    /*p31.CYWE*/ top.sprite_store.CYWE_STORE5_X4p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _COSE_X4n);
    /*p31.DYBY*/ top.sprite_store.DYBY_STORE5_X5p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _AROP_X5n);
    /*p31.DURY*/ top.sprite_store.DURY_STORE5_X6p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _XATU_X6n);
    /*p31.CUVY*/ top.sprite_store.CUVY_STORE5_X7p.dff9(CYLA_STORE5_CLKp, EJAD_STORE5_RSTn, _BADY_X7n);

    /*p29.ZAPE*/ wire ZAPE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
    /*p29.WUSE*/ wire WUSE_STORE6_CLKp = not1(WEKA_STORE6_CLKn);
    /*p29.ZURU*/ wire ZURU_STORE6_CLKp = not1(WEKA_STORE6_CLKn);

    /*p30.GABO*/ top.sprite_store.GABO_STORE6_I0n.dff8n(WUSE_STORE6_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.GACY*/ top.sprite_store.GACY_STORE6_I1n.dff8n(WUSE_STORE6_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.FOGO*/ top.sprite_store.FOGO_STORE6_I2n.dff8n(WUSE_STORE6_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.GOHU*/ top.sprite_store.GOHU_STORE6_I3n.dff8n(WUSE_STORE6_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.FOXY*/ top.sprite_store.FOXY_STORE6_I4n.dff8n(WUSE_STORE6_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.GECU*/ top.sprite_store.GECU_STORE6_I5n.dff8n(WUSE_STORE6_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.ZUMY*/ top.sprite_store.ZUMY_STORE6_L0n.dff8n(ZURU_STORE6_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.ZAFU*/ top.sprite_store.ZAFU_STORE6_L1n.dff8n(ZURU_STORE6_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.ZEXO*/ top.sprite_store.ZEXO_STORE6_L2n.dff8n(ZURU_STORE6_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.ZUBE*/ top.sprite_store.ZUBE_STORE6_L3n.dff8n(ZURU_STORE6_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.YCOL*/ top.sprite_store.YCOL_STORE6_X0p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _ZAGO_X0n);
    /*p31.YRAC*/ top.sprite_store.YRAC_STORE6_X1p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _ZOCY_X1n);
    /*p31.YMEM*/ top.sprite_store.YMEM_STORE6_X2p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _YPUR_X2n);
    /*p31.YVAG*/ top.sprite_store.YVAG_STORE6_X3p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _YVOK_X3n);
    /*p31.ZOLY*/ top.sprite_store.ZOLY_STORE6_X4p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _COSE_X4n);
    /*p31.ZOGO*/ top.sprite_store.ZOGO_STORE6_X5p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _AROP_X5n);
    /*p31.ZECU*/ top.sprite_store.ZECU_STORE6_X6p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _XATU_X6n);
    /*p31.ZESA*/ top.sprite_store.ZESA_STORE6_X7p.dff9(ZAPE_STORE6_CLKp, XAHO_STORE6_RSTn, _BADY_X7n);

    /*p29.GECY*/ wire GECY_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
    /*p29.FEFO*/ wire FEFO_STORE7_CLKp = not1(GYVO_STORE7_CLKn);
    /*p29.WABE*/ wire WABE_STORE7_CLKp = not1(GYVO_STORE7_CLKn);

    /*p30.GULE*/ top.sprite_store.GULE_STORE7_I0n.dff8n(FEFO_STORE7_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.GYNO*/ top.sprite_store.GYNO_STORE7_I1n.dff8n(FEFO_STORE7_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.FEFA*/ top.sprite_store.FEFA_STORE7_I2n.dff8n(FEFO_STORE7_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.FYSU*/ top.sprite_store.FYSU_STORE7_I3n.dff8n(FEFO_STORE7_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.GESY*/ top.sprite_store.GESY_STORE7_I4n.dff8n(FEFO_STORE7_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.FUZO*/ top.sprite_store.FUZO_STORE7_I5n.dff8n(FEFO_STORE7_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.XYNA*/ top.sprite_store.XYNA_STORE7_L0n.dff8n(WABE_STORE7_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.YGUM*/ top.sprite_store.YGUM_STORE7_L1n.dff8n(WABE_STORE7_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.XAKU*/ top.sprite_store.XAKU_STORE7_L2n.dff8n(WABE_STORE7_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.XYGO*/ top.sprite_store.XYGO_STORE7_L3n.dff8n(WABE_STORE7_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.ERAZ*/ top.sprite_store.ERAZ_STORE7_X0p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _ZAGO_X0n);
    /*p31.EPUM*/ top.sprite_store.EPUM_STORE7_X1p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _ZOCY_X1n);
    /*p31.EROL*/ top.sprite_store.EROL_STORE7_X2p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _YPUR_X2n);
    /*p31.EHYN*/ top.sprite_store.EHYN_STORE7_X3p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _YVOK_X3n);
    /*p31.FAZU*/ top.sprite_store.FAZU_STORE7_X4p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _COSE_X4n);
    /*p31.FAXE*/ top.sprite_store.FAXE_STORE7_X5p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _AROP_X5n);
    /*p31.EXUK*/ top.sprite_store.EXUK_STORE7_X6p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _XATU_X6n);
    /*p31.FEDE*/ top.sprite_store.FEDE_STORE7_X7p.dff9(GECY_STORE7_CLKp, GAFY_STORE7_RSTn, _BADY_X7n);

    /*p29.CEXU*/ wire CEXU_STORE8_CLKp = not1(BUKA_STORE8_CLKn);
    /*p29.AKOL*/ wire AKOL_STORE8_CLKp = not1(BUKA_STORE8_CLKn);
    /*p29.BYMY*/ wire BYMY_STORE8_CLKp = not1(BUKA_STORE8_CLKn);

    /*p30.AXUV*/ top.sprite_store.AXUV_STORE8_I0n.dff8n(AKOL_STORE8_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.BADA*/ top.sprite_store.BADA_STORE8_I1n.dff8n(AKOL_STORE8_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.APEV*/ top.sprite_store.APEV_STORE8_I2n.dff8n(AKOL_STORE8_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.BADO*/ top.sprite_store.BADO_STORE8_I3n.dff8n(AKOL_STORE8_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.BEXY*/ top.sprite_store.BEXY_STORE8_I4n.dff8n(AKOL_STORE8_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.BYHE*/ top.sprite_store.BYHE_STORE8_I5n.dff8n(AKOL_STORE8_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.AZAP*/ top.sprite_store.AZAP_STORE8_L0n.dff8n(BYMY_STORE8_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.AFYX*/ top.sprite_store.AFYX_STORE8_L1n.dff8n(BYMY_STORE8_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.AFUT*/ top.sprite_store.AFUT_STORE8_L2n.dff8n(BYMY_STORE8_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.AFYM*/ top.sprite_store.AFYM_STORE8_L3n.dff8n(BYMY_STORE8_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.EZUF*/ top.sprite_store.EZUF_STORE8_X0p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _COSE_X4n);
    /*p31.ENAD*/ top.sprite_store.ENAD_STORE8_X1p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _AROP_X5n);
    /*p31.EBOW*/ top.sprite_store.EBOW_STORE8_X2p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _XATU_X6n);
    /*p31.FYCA*/ top.sprite_store.FYCA_STORE8_X3p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _BADY_X7n);
    /*p31.GAVY*/ top.sprite_store.GAVY_STORE8_X4p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _ZAGO_X0n);
    /*p31.GYPU*/ top.sprite_store.GYPU_STORE8_X5p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _ZOCY_X1n);
    /*p31.GADY*/ top.sprite_store.GADY_STORE8_X6p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _YPUR_X2n);
    /*p31.GAZA*/ top.sprite_store.GAZA_STORE8_X7p.dff9(CEXU_STORE8_CLKp, WUZO_STORE8_RSTn, _YVOK_X3n);

    /*p29.WEME*/ wire WEME_STORE9_CLKp = not1(DECU_STORE9_CLKn);
    /*p29.WUFA*/ wire WUFA_STORE9_CLKp = not1(DECU_STORE9_CLKn);
    /*p29.FAKA*/ wire FAKA_STORE9_CLKp = not1(DECU_STORE9_CLKn);

    /*p30.YBER*/ top.sprite_store.YBER_STORE9_I0n.dff8n(WUFA_STORE9_CLKp, top.sprite_store.SPR_TRI_I0p.qp());
    /*p30.YZOR*/ top.sprite_store.YZOR_STORE9_I1n.dff8n(WUFA_STORE9_CLKp, top.sprite_store.SPR_TRI_I1p.qp());
    /*p30.XYFE*/ top.sprite_store.XYFE_STORE9_I2n.dff8n(WUFA_STORE9_CLKp, top.sprite_store.SPR_TRI_I2p.qp());
    /*p30.XOTU*/ top.sprite_store.XOTU_STORE9_I3n.dff8n(WUFA_STORE9_CLKp, top.sprite_store.SPR_TRI_I3p.qp());
    /*p30.XUTE*/ top.sprite_store.XUTE_STORE9_I4n.dff8n(WUFA_STORE9_CLKp, top.sprite_store.SPR_TRI_I4p.qp());
    /*p30.XUFO*/ top.sprite_store.XUFO_STORE9_I5n.dff8n(WUFA_STORE9_CLKp, top.sprite_store.SPR_TRI_I5p.qp());

    /*p30.CANA*/ top.sprite_store.CANA_STORE9_L0n.dff8n(FAKA_STORE9_CLKp, top.sprite_store.SPR_TRI_L0.qp());
    /*p30.FOFO*/ top.sprite_store.FOFO_STORE9_L1n.dff8n(FAKA_STORE9_CLKp, top.sprite_store.SPR_TRI_L1.qp());
    /*p30.DYSY*/ top.sprite_store.DYSY_STORE9_L2n.dff8n(FAKA_STORE9_CLKp, top.sprite_store.SPR_TRI_L2.qp());
    /*p30.DEWU*/ top.sprite_store.DEWU_STORE9_L3n.dff8n(FAKA_STORE9_CLKp, top.sprite_store.SPR_TRI_L3.qp());

    /*p31.XUVY*/ top.sprite_store.XUVY_STORE9_X0p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _ZAGO_X0n);
    /*p31.XERE*/ top.sprite_store.XERE_STORE9_X1p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _ZOCY_X1n);
    /*p31.XUZO*/ top.sprite_store.XUZO_STORE9_X2p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _YPUR_X2n);
    /*p31.XEXA*/ top.sprite_store.XEXA_STORE9_X3p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _YVOK_X3n);
    /*p31.YPOD*/ top.sprite_store.YPOD_STORE9_X4p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _COSE_X4n);
    /*p31.YROP*/ top.sprite_store.YROP_STORE9_X5p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _AROP_X5n);
    /*p31.YNEP*/ top.sprite_store.YNEP_STORE9_X6p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _XATU_X6n);
    /*p31.YZOF*/ top.sprite_store.YZOF_STORE9_X7p.dff9(WEME_STORE9_CLKp, DOSY_STORE9_RSTn, _BADY_X7n);
  }

  //------------------------------------------------------------------------------

  pix_pipe.tock(top, cpu_bus);

  //------------------------------------------------------------------------------

  sprite_fetcher.tock(top);

  //------------------------------------------------------------------------------

  tile_fetcher.tock(top);

  //------------------------------------------------------------------------------

  int_reg.tock(top, cpu_bus);

  //------------------------------------------------------------------------------

  cpu_bus.PIN_CPU_ADDR_HIp.set(cpu_bus.SYRO_FE00_FFFFp());
  cpu_bus.PIN_CPU_BOOTp.set(TUTU_ADDR_BOOTp);

  //------------------------------------------------------------------------------
  // ext_bus.tock(top);


  /*p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(top.TEXO_ADDR_EXT_AND_NOT_VRAM, top.UMUT_MODE_DBG1p);
  /*p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM = not1(top.TEXO_ADDR_EXT_AND_NOT_VRAM);
  /*p08.LAGU*/ wire _LAGU = and_or3(top.cpu_bus.PIN_CPU_RDp.qp(), _LEVO_ADDR_INT_OR_ADDR_VRAM, top.cpu_bus.PIN_CPU_WRp.qp());
  /*p08.LYWE*/ wire _LYWE = not1(_LAGU);
  /*p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);

  /*p08.TOVA*/ wire _TOVA_MODE_DBG2n = not1(top.UNOR_MODE_DBG2p);

  //----------------------------------------
  // Ext pins

  {
    /*p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(top.dma_reg.LUMA_DMA_CARTp, _MOTY_CPU_EXT_RD);
    /*p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, _TOVA_MODE_DBG2n);
    /*p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, top.UNOR_MODE_DBG2p);
    top.ext_bus.PIN_EXT_RDn.io_pin(_UGAC_RD_A, _URUN_RD_D);
  }

  {

    /*p08.MEXO*/ wire _MEXO_CPU_WRn_ABCDxxxH = not1(top.APOV_CPU_WRp_xxxxEFGx);
    /*p08.NEVY*/ wire _NEVY = or2(_MEXO_CPU_WRn_ABCDxxxH, _MOCA_DBG_EXT_RD);
    /*p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, top.dma_reg.LUMA_DMA_CARTp);
    /*p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, _TOVA_MODE_DBG2n);
    /*p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, top.UNOR_MODE_DBG2p);
    top.ext_bus.PIN_EXT_WRn.io_pin(_UVER_WR_A, _USUF_WR_D);
  }


  {
    /*p08.SOGY*/ wire _SOGY_A14n = not1(top.cpu_bus.BUS_CPU_A14.qp());
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and3(top.cpu_bus.BUS_CPU_A13.qp(), _SOGY_A14n, top.cpu_bus.BUS_CPU_A15.qp());
    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = and_or3(top.cpu_bus.BUS_CPU_A15.qp(), top.cpu_bus.BUS_CPU_A14.qp(), _TUMA_CART_RAM);

    /*p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH = and3(top.ABUZ_xxCDEFGH, _TYNU_ADDR_RAM, top.cpu_bus.TUNA_0000_FDFFp());
    /*p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.MARU_DMA_A15n.qn07(), _TOZA_PIN_EXT_CS_A_xxCDEFGH);
    top.ext_bus.PIN_EXT_CSn.io_pin(_TYHO_PIN_EXT_CS_A_xxCDEFGH, _TYHO_PIN_EXT_CS_A_xxCDEFGH);
  }


  //----------------------------------------
  // Ext address

  // DMA address / CPU address latch -> ext addr pins
  {
    /*p08.MULE*/ wire _MULE_MODE_DBG1n  = not1(top.UMUT_MODE_DBG1p);
    /*p08.LOXO*/ wire _LOXO_HOLDn = and_or3(_MULE_MODE_DBG1n, top.TEXO_ADDR_EXT_AND_NOT_VRAM, top.UMUT_MODE_DBG1p);
    /*p08.LASY*/ wire _LASY_HOLDp = not1(_LOXO_HOLDn);
    /*p08.MATE*/ wire _MATE_HOLDn = not1(_LASY_HOLDp);

    /* p08.ALOR*/ top.ext_bus.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A00.qp());
    /* p08.APUR*/ top.ext_bus.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A01.qp());
    /* p08.ALYR*/ top.ext_bus.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A02.qp());
    /* p08.ARET*/ top.ext_bus.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A03.qp());
    /* p08.AVYS*/ top.ext_bus.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A04.qp());
    /* p08.ATEV*/ top.ext_bus.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A05.qp());
    /* p08.AROS*/ top.ext_bus.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A06.qp());
    /* p08.ARYM*/ top.ext_bus.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A07.qp());
    /* p08.LUNO*/ top.ext_bus.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A08.qp());
    /* p08.LYSA*/ top.ext_bus.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A09.qp());
    /* p08.PATE*/ top.ext_bus.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A10.qp());
    /* p08.LUMY*/ top.ext_bus.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A11.qp());
    /* p08.LOBU*/ top.ext_bus.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A12.qp());
    /* p08.LONU*/ top.ext_bus.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A13.qp());
    /* p08.NYRE*/ top.ext_bus.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn, top.cpu_bus.BUS_CPU_A14.qp());


    /* p08.AMET*/ wire _AMET_A00p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.NAKY_DMA_A00p.qp17(), top.ext_bus.ALOR_EXT_ADDR_LATCH_00p.qp08());
    /* p08.ATOL*/ wire _ATOL_A01p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.PYRO_DMA_A01p.qp17(), top.ext_bus.APUR_EXT_ADDR_LATCH_01p.qp08());
    /* p08.APOK*/ wire _APOK_A02p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.NEFY_DMA_A02p.qp17(), top.ext_bus.ALYR_EXT_ADDR_LATCH_02p.qp08());
    /* p08.AMER*/ wire _AMER_A03p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.MUTY_DMA_A03p.qp17(), top.ext_bus.ARET_EXT_ADDR_LATCH_03p.qp08());
    /* p08.ATEM*/ wire _ATEM_A04p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.NYKO_DMA_A04p.qp17(), top.ext_bus.AVYS_EXT_ADDR_LATCH_04p.qp08());
    /* p08.ATOV*/ wire _ATOV_A05p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.PYLO_DMA_A05p.qp17(), top.ext_bus.ATEV_EXT_ADDR_LATCH_05p.qp08());
    /* p08.ATYR*/ wire _ATYR_A06p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.NUTO_DMA_A06p.qp17(), top.ext_bus.AROS_EXT_ADDR_LATCH_06p.qp08());
    /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.MUGU_DMA_A07p.qp17(), top.ext_bus.ARYM_EXT_ADDR_LATCH_07p.qp08());
    /*#p08.MANO*/ wire _MANO_A08p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.NAFA_DMA_A08n.qn07(), top.ext_bus.LUNO_EXT_ADDR_LATCH_08p.qp08());
    /* p08.MASU*/ wire _MASU_A09p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.PYNE_DMA_A09n.qn07(), top.ext_bus.LYSA_EXT_ADDR_LATCH_09p.qp08());
    /* p08.PAMY*/ wire _PAMY_A10p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.PARA_DMA_A10n.qn07(), top.ext_bus.PATE_EXT_ADDR_LATCH_10p.qp08());
    /* p08.MALE*/ wire _MALE_A11p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.NYDO_DMA_A11n.qn07(), top.ext_bus.LUMY_EXT_ADDR_LATCH_11p.qp08());
    /* p08.MOJY*/ wire _MOJY_A12p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.NYGY_DMA_A12n.qn07(), top.ext_bus.LOBU_EXT_ADDR_LATCH_12p.qp08());
    /* p08.MUCE*/ wire _MUCE_A13p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.PULA_DMA_A13n.qn07(), top.ext_bus.LONU_EXT_ADDR_LATCH_13p.qp08());
    /* p08.PEGE*/ wire _PEGE_A14p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.POKU_DMA_A14n.qn07(), top.ext_bus.NYRE_EXT_ADDR_LATCH_14p.qp08());

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

    /*p08.KOTY*/ wire _KOTY = nor2 (_AMET_A00p, top.UNOR_MODE_DBG2p);
    /*p08.COTU*/ wire _COTU = nor2 (_ATOL_A01p, top.UNOR_MODE_DBG2p);
    /*p08.BAJO*/ wire _BAJO = nor2 (_APOK_A02p, top.UNOR_MODE_DBG2p);
    /*p08.BOLA*/ wire _BOLA = nor2 (_AMER_A03p, top.UNOR_MODE_DBG2p);
    /*p08.BEVO*/ wire _BEVO = nor2 (_ATEM_A04p, top.UNOR_MODE_DBG2p);
    /*p08.AJAV*/ wire _AJAV = nor2 (_ATOV_A05p, top.UNOR_MODE_DBG2p);
    /*p08.CYKA*/ wire _CYKA = nor2 (_ATYR_A06p, top.UNOR_MODE_DBG2p);
    /*p08.COLO*/ wire _COLO = nor2 (_ASUR_A07p, top.UNOR_MODE_DBG2p);
    /*p08.MEGO*/ wire _MEGO = nor2 (_MANO_A08p, top.UNOR_MODE_DBG2p);
    /*p08.MENY*/ wire _MENY = nor2 (_MASU_A09p, top.UNOR_MODE_DBG2p);
    /*p08.RORE*/ wire _RORE = nor2 (_PAMY_A10p, top.UNOR_MODE_DBG2p);
    /*p08.LYNY*/ wire _LYNY = nor2 (_MALE_A11p, top.UNOR_MODE_DBG2p);
    /*p08.LOSO*/ wire _LOSO = nor2 (_MOJY_A12p, top.UNOR_MODE_DBG2p);
    /*p08.LEVA*/ wire _LEVA = nor2 (_MUCE_A13p, top.UNOR_MODE_DBG2p);
    /*p08.PAHY*/ wire _PAHY = nor2 (_PEGE_A14p, top.UNOR_MODE_DBG2p);

    top.ext_bus.PIN_EXT_A00p.io_pin(_KUPO, _KOTY);
    top.ext_bus.PIN_EXT_A01p.io_pin(_CABA, _COTU);
    top.ext_bus.PIN_EXT_A02p.io_pin(_BOKU, _BAJO);
    top.ext_bus.PIN_EXT_A03p.io_pin(_BOTY, _BOLA);
    top.ext_bus.PIN_EXT_A04p.io_pin(_BYLA, _BEVO);
    top.ext_bus.PIN_EXT_A05p.io_pin(_BADU, _AJAV);
    top.ext_bus.PIN_EXT_A06p.io_pin(_CEPU, _CYKA);
    top.ext_bus.PIN_EXT_A07p.io_pin(_DEFY, _COLO);
    top.ext_bus.PIN_EXT_A08p.io_pin(_MYNY, _MEGO);
    top.ext_bus.PIN_EXT_A09p.io_pin(_MUNE, _MENY);
    top.ext_bus.PIN_EXT_A10p.io_pin(_ROXU, _RORE);
    top.ext_bus.PIN_EXT_A11p.io_pin(_LEPY, _LYNY);
    top.ext_bus.PIN_EXT_A12p.io_pin(_LUCE, _LOSO);
    top.ext_bus.PIN_EXT_A13p.io_pin(_LABE, _LEVA);
    top.ext_bus.PIN_EXT_A14p.io_pin(_PUHE, _PAHY);
  }


  {
    // A15 is "special"

    /* p08.RYCA*/ wire _RYCA_MODE_DBG2n = not1(top.UNOR_MODE_DBG2p);
    /* p08.SOBY*/ wire _SOBY_A15n = nor2(top.cpu_bus.BUS_CPU_A15.qp(), top.TUTU_ADDR_BOOTp);
    /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx = nand2(top.ABUZ_xxCDEFGH, _SOBY_A15n);
    /* p08.TAZY*/ wire _TAZY_A15p = mux2p(top.dma_reg.LUMA_DMA_CARTp, top.dma_reg.MARU_DMA_A15n.qn07(), _SEPY_A15p_ABxxxxxx);
    /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n = nand2(_TAZY_A15p, _RYCA_MODE_DBG2n);
    /* p08.RULO*/ wire _RULO_PIN_EXT_A15n = nor2 (_TAZY_A15p, top.UNOR_MODE_DBG2p);

    top.ext_bus.PIN_EXT_A15p.io_pin(_SUZE_PIN_EXT_A15n, _RULO_PIN_EXT_A15n);
  }

  //----------------------------------------
  // Ext data out


  // CPU data bus -> external data bus
  {
    // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.REDU*/ wire _REDU_CPU_RDn = not1(top.TEDO_CPU_RDp);
    /*p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(top.UNOR_MODE_DBG2p, _REDU_CPU_RDn, _MOTY_CPU_EXT_RD);
    /*p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

    /*p25.RUXA*/ wire _RUXA = nand2(top.cpu_bus.BUS_CPU_D0p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RUJA*/ wire _RUJA = nand2(top.cpu_bus.BUS_CPU_D1p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RABY*/ wire _RABY = nand2(top.cpu_bus.BUS_CPU_D2p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RERA*/ wire _RERA = nand2(top.cpu_bus.BUS_CPU_D3p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RORY*/ wire _RORY = nand2(top.cpu_bus.BUS_CPU_D4p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RYVO*/ wire _RYVO = nand2(top.cpu_bus.BUS_CPU_D5p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAFY*/ wire _RAFY = nand2(top.cpu_bus.BUS_CPU_D6p.qp(), _LULA_CBD_TO_EPDp);
    /*p25.RAVU*/ wire _RAVU = nand2(top.cpu_bus.BUS_CPU_D7p.qp(), _LULA_CBD_TO_EPDp);

    /*p08.RUNE*/ wire _RUNE = nor2 (top.cpu_bus.BUS_CPU_D0p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYPU*/ wire _RYPU = nor2 (top.cpu_bus.BUS_CPU_D1p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SULY*/ wire _SULY = nor2 (top.cpu_bus.BUS_CPU_D2p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.SEZE*/ wire _SEZE = nor2 (top.cpu_bus.BUS_CPU_D3p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RESY*/ wire _RESY = nor2 (top.cpu_bus.BUS_CPU_D4p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.TAMU*/ wire _TAMU = nor2 (top.cpu_bus.BUS_CPU_D5p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.ROGY*/ wire _ROGY = nor2 (top.cpu_bus.BUS_CPU_D6p.qp(), _RORU_CBD_TO_EPDn);
    /*p08.RYDA*/ wire _RYDA = nor2 (top.cpu_bus.BUS_CPU_D7p.qp(), _RORU_CBD_TO_EPDn);

    top.ext_bus.PIN_EXT_D00p.io_pin(_RUXA, _RUNE, _LULA_CBD_TO_EPDp);
    top.ext_bus.PIN_EXT_D01p.io_pin(_RUJA, _RYPU, _LULA_CBD_TO_EPDp);
    top.ext_bus.PIN_EXT_D02p.io_pin(_RABY, _SULY, _LULA_CBD_TO_EPDp);
    top.ext_bus.PIN_EXT_D03p.io_pin(_RERA, _SEZE, _LULA_CBD_TO_EPDp);
    top.ext_bus.PIN_EXT_D04p.io_pin(_RORY, _RESY, _LULA_CBD_TO_EPDp);
    top.ext_bus.PIN_EXT_D05p.io_pin(_RYVO, _TAMU, _LULA_CBD_TO_EPDp);
    top.ext_bus.PIN_EXT_D06p.io_pin(_RAFY, _ROGY, _LULA_CBD_TO_EPDp);
    top.ext_bus.PIN_EXT_D07p.io_pin(_RAVU, _RYDA, _LULA_CBD_TO_EPDp);
  }

  //----------------------------------------
  // Ext data in
  {
    // -> ext bus
    /*p08.LAVO*/ wire _LAVO_HOLDn = nand3(top.cpu_bus.PIN_CPU_RDp.qp(), top.TEXO_ADDR_EXT_AND_NOT_VRAM, top.cpu_bus.PIN_CPU_LATCH_EXT.qp());

    // Ext pin -> Ext latch
    /*#p08.SOMA*/ top.ext_bus.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D00p.qn());
    /* p08.RONY*/ top.ext_bus.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D01p.qn());
    /* p08.RAXY*/ top.ext_bus.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D02p.qn());
    /* p08.SELO*/ top.ext_bus.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D03p.qn());
    /* p08.SODY*/ top.ext_bus.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D04p.qn());
    /* p08.SAGO*/ top.ext_bus.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D05p.qn());
    /* p08.RUPA*/ top.ext_bus.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D06p.qn());
    /* p08.SAZY*/ top.ext_bus.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn, top.ext_bus.PIN_EXT_D07p.qn());

    // Ext latch -> int bus
    /*#p08.RYMA*/ top.cpu_bus.BUS_CPU_D0p.tri_6nn(_LAVO_HOLDn, top.ext_bus.SOMA_EXT_DATA_LATCH_D0n.qp08());
    /* p08.RUVO*/ top.cpu_bus.BUS_CPU_D1p.tri_6nn(_LAVO_HOLDn, top.ext_bus.RONY_EXT_DATA_LATCH_D1n.qp08());
    /* p08.RYKO*/ top.cpu_bus.BUS_CPU_D2p.tri_6nn(_LAVO_HOLDn, top.ext_bus.RAXY_EXT_DATA_LATCH_D2n.qp08());
    /* p08.TAVO*/ top.cpu_bus.BUS_CPU_D3p.tri_6nn(_LAVO_HOLDn, top.ext_bus.SELO_EXT_DATA_LATCH_D3n.qp08());
    /* p08.TEPE*/ top.cpu_bus.BUS_CPU_D4p.tri_6nn(_LAVO_HOLDn, top.ext_bus.SODY_EXT_DATA_LATCH_D4n.qp08());
    /* p08.SAFO*/ top.cpu_bus.BUS_CPU_D5p.tri_6nn(_LAVO_HOLDn, top.ext_bus.SAGO_EXT_DATA_LATCH_D5n.qp08());
    /* p08.SEVU*/ top.cpu_bus.BUS_CPU_D6p.tri_6nn(_LAVO_HOLDn, top.ext_bus.RUPA_EXT_DATA_LATCH_D6n.qp08());
    /* p08.TAJU*/ top.cpu_bus.BUS_CPU_D7p.tri_6nn(_LAVO_HOLDn, top.ext_bus.SAZY_EXT_DATA_LATCH_D7n.qp08());
  }

  //------------------------------------------------------------------------------
  // OAM signals

  /* p28.XYNY*/ wire _XYNY_ABCDxxxx = not1(top.clk_reg.MOPA_xxxxEFGH);

  /*#p28.GEKA*/ wire _GEKA_OAM_A0p = not1(top.oam_bus.BUS_OAM_A0n.qp()); // -> WAFO, YLYC, WUKU
  /* p28.WAFO*/ wire _WAFO_OAM_A0n = not1(_GEKA_OAM_A0p);

  /* p28.AJON*/ wire _AJON_PPU_OAM_ENp = and2(top.dma_reg.BOGE_DMA_RUNNINGn, top.pix_pipe.XYMU_RENDERINGn.qn03()); // def AND. ppu can read oam when there's rendering but no dma

  /*#p28.AJEP*/ wire _AJEP_SCAN_OAM_LATCHn = nand2(top.sprite_scanner.ACYL_SCANNINGp, top.clk_reg.XOCE_xBCxxFGx); // schematic wrong, is def nand2
  /*#p28.XUJA*/ wire _XUJA_SPR_OAM_LATCHn  = not1(top.sprite_fetcher.WEFY_SPR_READp);
  /*#p28.BOFE*/ wire _BOFE_LATCH_EXTn = not1(top.CATY_LATCH_EXTp);
  /*#p28.BOTA*/ wire _BOTA_CPU_OAM_OEn  = nand3(_BOFE_LATCH_EXTn, top.cpu_bus.SARO_FE00_FEFFp(), top.ASOT_CPU_RDp); // Schematic wrong, this is NAND

  //probe(16, "PIN_CPU_LATCH_EXT", top.cpu_bus.PIN_CPU_LATCH_EXT.qp());
  //probe(17, "SARO_FE00_FEFFp",   top.cpu_bus.SARO_FE00_FEFFp());
  //probe(18, "ASOT_CPU_RDp",      top.ASOT_CPU_RDp);
  //probe(15, "BOTA_CPU_OAM_OEn",  _BOTA_CPU_OAM_OEn);

  /*#p28.ASYT*/ wire _ASYT_OAM_OEn = and3(_AJEP_SCAN_OAM_LATCHn, _XUJA_SPR_OAM_LATCHn, _BOTA_CPU_OAM_OEn); // def and
  /*#p28.BODE*/ wire _BODE_OAM_OEp = not1(_ASYT_OAM_OEn);

  /* p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor3(top.dma_reg.MATU_DMA_RUNNINGp.qp17(), top.sprite_scanner.ACYL_SCANNINGp, _AJON_PPU_OAM_ENp); // def nor
  /* p28.AMAB*/ wire _AMAB_CPU_OAM_ENp = and2(top.cpu_bus.SARO_FE00_FEFFp(), _AJUJ_OAM_BUSYn); // def and

  // CUFE looks like BYHA minus an inverter

  bool _COTA_OAM_CLKn;
  {
    /* p29.XYSO*/ wire _XYSO_xBCDxFGH = not1(top.clk_reg.WOJO_AxxxExxx);
    /* p25.AVER*/ wire _AVER_SCAN_OAM_CLK = nand2(top.sprite_scanner.ACYL_SCANNINGp, _XYSO_xBCDxFGH);
    /* p25.XUJY*/ wire _XUJY_PPU_OAM_CLK  = not1(top.sprite_fetcher.VAPE_FETCH_OAM_CLK);
    /* p25.CUFE*/ wire _CUFE_OAM_CLK  = not1(or_and3(top.cpu_bus.SARO_FE00_FEFFp(), top.dma_reg.MATU_DMA_RUNNINGp.qp17(), top.clk_reg.MOPA_xxxxEFGH));

    /* p25.BYCU*/ wire _BYCU_OAM_CLKp = nand3(_AVER_SCAN_OAM_CLK, _XUJY_PPU_OAM_CLK, _CUFE_OAM_CLK);
    /* p25.COTA*/ _COTA_OAM_CLKn = not1(_BYCU_OAM_CLKp);
  }

  //----------------------------------------
  // OAM pins
  // PIN_CPU_LATCH_EXT _blocks_ DMA from writing to OAM? wat?

  {
    top.oam_bus.PIN_OAM_CLK.set(_COTA_OAM_CLKn);

    /*#p28.YVAL*/ wire _YVAL_OAM_OEn = not1(_BODE_OAM_OEp);
    /*#p28.YRYV*/ wire _YRYU_OAM_OEp = not1(_YVAL_OAM_OEn);
    /*#p28.ZODO*/ wire _ZODO_OAM_OEn = not1(_YRYU_OAM_OEp);
    top.oam_bus.PIN_OAM_OE.set(_ZODO_OAM_OEn);

    /*p04.MAKA*/ top.oam_bus.MAKA_HOLD_MEMp.dff17(top.clk_reg.ZEME_AxCxExGx, top.clk_reg.CUNU_SYS_RSTn, top.CATY_LATCH_EXTp);

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WRp = nor2(top.clk_reg.UVYT_ABCDxxxx, top.oam_bus.MAKA_HOLD_MEMp.qp17()); // def nor2
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp  = and2(top.dma_reg.MATU_DMA_RUNNINGp.qp17(), _NAXY_DMA_OAM_WRp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp      = and_or3(_AMAB_CPU_OAM_ENp, top.CUPA_CPU_WRp_xxxxEFGx, _POWU_DMA_OAM_WRp);
    /*p28.YNYC*/ wire _YNYC_OAM_A_WRp = and2(_WYJA_OAM_WRp, _WAFO_OAM_A0n);
    /*p28.YLYC*/ wire _YLYC_OAM_B_WRp = and2(_WYJA_OAM_WRp, _GEKA_OAM_A0p);
    /*p28.ZOFE*/ wire _ZOFE_OAM_A_WRn = not1(_YNYC_OAM_A_WRp);
    /*p28.ZONE*/ wire _ZONE_OAM_B_WRn = not1(_YLYC_OAM_B_WRp);
    top.oam_bus.PIN_OAM_WR_A.set(_ZOFE_OAM_A_WRn);
    top.oam_bus.PIN_OAM_WR_B.set(_ZONE_OAM_B_WRn);
  }

  //----------------------------------------
  // OAM address mux

  {
    /*p28.APAR*/ wire _APAR_SCAN_OAM_RDn = not1(top.sprite_scanner.ACYL_SCANNINGp);
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn  = not1(top.dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn  = or3(top.sprite_scanner.ACYL_SCANNINGp, top.pix_pipe.XYMU_RENDERINGn.qn03(), top.dma_reg.MATU_DMA_RUNNINGp.qp17());
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn  = not1(_AJON_PPU_OAM_ENp);

    //probe(20, "_APAR_SCAN_OAM_RDn", _APAR_SCAN_OAM_RDn);
    //probe(21, "_DUGA_DMA_OAM_RDn ", _DUGA_DMA_OAM_RDn );
    //probe(22, "_ASAM_CPU_OAM_RDn ", _ASAM_CPU_OAM_RDn );
    //probe(23, "_BETE_PPU_OAM_RDn ", _BETE_PPU_OAM_RDn );

    // Scanner addr -> OAM addr
    /*p28.GEFY*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.WUWE*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_APAR_SCAN_OAM_RDn, GND);
    /*p28.GUSE*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.YFEL_SCAN0.qp17());
    /*p28.GEMA*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.WEWY_SCAN1.qp17());
    /*p28.FUTO*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.GOSO_SCAN2.qp17());
    /*p28.FAKU*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.ELYN_SCAN3.qp17());
    /*p28.GAMA*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FAHA_SCAN4.qp17());
    /*p28.GOBY*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_APAR_SCAN_OAM_RDn, top.sprite_scanner.FONY_SCAN5.qp17());

    // DMA addr -> OAM addr
    /*p28.FODO*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp17());
    /*p28.FESA*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp17());
    /*p28.FAGO*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp17());
    /*p28.FYKY*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp17());
    /*p28.ELUG*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp17());
    /*p28.EDOL*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp17());
    /*p28.FYDU*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp17());
    /*p28.FETU*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_DUGA_DMA_OAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp17());

    // CPU addr -> OAM addr
    /*p28.GARO*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A00.qp());
    /*p28.WACU*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A01.qp());
    /*p28.GOSE*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A02.qp());
    /*p28.WAPE*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A03.qp());
    /*p28.FEVU*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A04.qp());
    /*p28.GERA*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A05.qp());
    /*p28.WAXA*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A06.qp());
    /*p28.FOBY*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_ASAM_CPU_OAM_RDn, top.cpu_bus.BUS_CPU_A07.qp());

    // PPU addr -> OAM addr
    /*p28.GECA*/ top.oam_bus.BUS_OAM_A0n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.WYDU*/ top.oam_bus.BUS_OAM_A1n.tri_6nn(_BETE_PPU_OAM_RDn, WEFE_VCC);
    /*p28.GYBU*/ top.oam_bus.BUS_OAM_A2n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I0p.qp());
    /*p28.GYKA*/ top.oam_bus.BUS_OAM_A3n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I1p.qp());
    /*p28.FABY*/ top.oam_bus.BUS_OAM_A4n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I2p.qp());
    /*p28.FACO*/ top.oam_bus.BUS_OAM_A5n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I3p.qp());
    /*p28.FUGU*/ top.oam_bus.BUS_OAM_A6n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I4p.qp());
    /*p28.FYKE*/ top.oam_bus.BUS_OAM_A7n.tri_6nn(_BETE_PPU_OAM_RDn, top.sprite_store.SPR_TRI_I5p.qp());
  }

  //----------------------------------------
  // OAM data out

  // CBD -> OBD
  {
    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WRp = and2(top.cpu_bus.SARO_FE00_FEFFp(), top.CUPA_CPU_WRp_xxxxEFGx);
    /*p28.WUJE*/ top.oam_bus.WUJE_CPU_OAM_WRn.nor_latch(_XYNY_ABCDxxxx, _XUTO_CPU_OAM_WRp);

    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WRp = not1(top.oam_bus.WUJE_CPU_OAM_WRn.qp04());
    /*p28.APAG*/ wire _APAG_CBD_TO_OBDp = amux2(_XUPA_CPU_OAM_WRp, _AMAB_CPU_OAM_ENp, _AJUJ_OAM_BUSYn, top.cpu_bus.ADAH_FE00_FEFFn());
    /*p28.AZUL*/ wire _AZUL_CBD_TO_OBDn = not1(_APAG_CBD_TO_OBDp);

    /*p28.ZAXA*/ top.oam_bus.BUS_OAM_DA0n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZAKY*/ top.oam_bus.BUS_OAM_DA1n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WULE*/ top.oam_bus.BUS_OAM_DA2n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZOZO*/ top.oam_bus.BUS_OAM_DA3n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUFO*/ top.oam_bus.BUS_OAM_DA4n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZATO*/ top.oam_bus.BUS_OAM_DA5n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.YVUC*/ top.oam_bus.BUS_OAM_DA6n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZUFE*/ top.oam_bus.BUS_OAM_DA7n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*p28.ZAMY*/ top.oam_bus.BUS_OAM_DB0n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p28.ZOPU*/ top.oam_bus.BUS_OAM_DB1n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p28.WYKY*/ top.oam_bus.BUS_OAM_DB2n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p28.ZAJA*/ top.oam_bus.BUS_OAM_DB3n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p28.ZUGA*/ top.oam_bus.BUS_OAM_DB4n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p28.ZUMO*/ top.oam_bus.BUS_OAM_DB5n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p28.XYTO*/ top.oam_bus.BUS_OAM_DB6n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p28.ZYFA*/ top.oam_bus.BUS_OAM_DB7n.tri_6nn(_AZUL_CBD_TO_OBDn, top.cpu_bus.BUS_CPU_D7p.qp());
  }

  // EBD -> OPD
  {
    /*p25.RALO*/ wire _RALO_EXT_D0p = not1(top.ext_bus.PIN_EXT_D00p.qn());
    /*p25.TUNE*/ wire _TUNE_EXT_D1p = not1(top.ext_bus.PIN_EXT_D01p.qn());
    /*p25.SERA*/ wire _SERA_EXT_D2p = not1(top.ext_bus.PIN_EXT_D02p.qn());
    /*p25.TENU*/ wire _TENU_EXT_D3p = not1(top.ext_bus.PIN_EXT_D03p.qn());
    /*p25.SYSA*/ wire _SYSA_EXT_D4p = not1(top.ext_bus.PIN_EXT_D04p.qn());
    /*p25.SUGY*/ wire _SUGY_EXT_D5p = not1(top.ext_bus.PIN_EXT_D05p.qn());
    /*p25.TUBE*/ wire _TUBE_EXT_D6p = not1(top.ext_bus.PIN_EXT_D06p.qn());
    /*p25.SYZO*/ wire _SYZO_EXT_D7p = not1(top.ext_bus.PIN_EXT_D07p.qn());

    /*p25.CEDE*/ wire _CEDE_EBD_TO_OBDn = not1(top.dma_reg.LUMA_DMA_CARTp);
    /*p25.WASA*/ top.oam_bus.BUS_OAM_DA0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BOMO*/ top.oam_bus.BUS_OAM_DA1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BASA*/ top.oam_bus.BUS_OAM_DA2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CAKO*/ top.oam_bus.BUS_OAM_DA3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BUMA*/ top.oam_bus.BUS_OAM_DA4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUPY*/ top.oam_bus.BUS_OAM_DA5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BASY*/ top.oam_bus.BUS_OAM_DA6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BAPE*/ top.oam_bus.BUS_OAM_DA7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);

    /*p25.WEJO*/ top.oam_bus.BUS_OAM_DB0n.tri_6nn(_CEDE_EBD_TO_OBDn, _RALO_EXT_D0p);
    /*p25.BUBO*/ top.oam_bus.BUS_OAM_DB1n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUNE_EXT_D1p);
    /*p25.BETU*/ top.oam_bus.BUS_OAM_DB2n.tri_6nn(_CEDE_EBD_TO_OBDn, _SERA_EXT_D2p);
    /*p25.CYME*/ top.oam_bus.BUS_OAM_DB3n.tri_6nn(_CEDE_EBD_TO_OBDn, _TENU_EXT_D3p);
    /*p25.BAXU*/ top.oam_bus.BUS_OAM_DB4n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYSA_EXT_D4p);
    /*p25.BUHU*/ top.oam_bus.BUS_OAM_DB5n.tri_6nn(_CEDE_EBD_TO_OBDn, _SUGY_EXT_D5p);
    /*p25.BYNY*/ top.oam_bus.BUS_OAM_DB6n.tri_6nn(_CEDE_EBD_TO_OBDn, _TUBE_EXT_D6p);
    /*p25.BYPY*/ top.oam_bus.BUS_OAM_DB7n.tri_6nn(_CEDE_EBD_TO_OBDn, _SYZO_EXT_D7p);
  }

  // VBD -> OPD
  {
    /*p28.AZAR*/ wire _AZAR_VBD_TO_OBDn = not1(top.dma_reg.LUFA_DMA_VRAMp);
    /*p28.WUZU*/ top.oam_bus.BUS_OAM_DA0n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AXER*/ top.oam_bus.BUS_OAM_DA1n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.ASOX*/ top.oam_bus.BUS_OAM_DA2n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.CETU*/ top.oam_bus.BUS_OAM_DA3n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.ARYN*/ top.oam_bus.BUS_OAM_DA4n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.ACOT*/ top.oam_bus.BUS_OAM_DA5n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.CUJE*/ top.oam_bus.BUS_OAM_DA6n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ATER*/ top.oam_bus.BUS_OAM_DA7n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());

    /*p28.WOWA*/ top.oam_bus.BUS_OAM_DB0n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p28.AVEB*/ top.oam_bus.BUS_OAM_DB1n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p28.AMUH*/ top.oam_bus.BUS_OAM_DB2n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p28.COFO*/ top.oam_bus.BUS_OAM_DB3n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p28.AZOZ*/ top.oam_bus.BUS_OAM_DB4n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p28.AGYK*/ top.oam_bus.BUS_OAM_DB5n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p28.BUSE*/ top.oam_bus.BUS_OAM_DB6n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p28.ANUM*/ top.oam_bus.BUS_OAM_DB7n.tri_6nn(_AZAR_VBD_TO_OBDn, top.vram_bus.BUS_VRAM_D7p.qp());
  }

  //----------------------------------------
  // OAM data in

  // OPD -> OBL
  {
    /* p29.YDYV*/ top.oam_bus.YDYV_OAM_LATCH_DA0n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA0n.qp());
    /* p29.YCEB*/ top.oam_bus.YCEB_OAM_LATCH_DA1n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA1n.qp());
    /* p29.ZUCA*/ top.oam_bus.ZUCA_OAM_LATCH_DA2n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA2n.qp());
    /* p29.WONE*/ top.oam_bus.WONE_OAM_LATCH_DA3n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA3n.qp());
    /* p29.ZAXE*/ top.oam_bus.ZAXE_OAM_LATCH_DA4n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA4n.qp());
    /* p29.XAFU*/ top.oam_bus.XAFU_OAM_LATCH_DA5n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA5n.qp());
    /* p29.YSES*/ top.oam_bus.YSES_OAM_LATCH_DA6n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA6n.qp());
    /* p29.ZECA*/ top.oam_bus.ZECA_OAM_LATCH_DA7n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DA7n.qp());

    /*#p31.XYKY*/ top.oam_bus.XYKY_OAM_LATCH_DB0n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB0n.qp());
    /* p31.YRUM*/ top.oam_bus.YRUM_OAM_LATCH_DB1n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB1n.qp());
    /* p31.YSEX*/ top.oam_bus.YSEX_OAM_LATCH_DB2n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB2n.qp());
    /* p31.YVEL*/ top.oam_bus.YVEL_OAM_LATCH_DB3n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB3n.qp());
    /* p31.WYNO*/ top.oam_bus.WYNO_OAM_LATCH_DB4n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB4n.qp());
    /* p31.CYRA*/ top.oam_bus.CYRA_OAM_LATCH_DB5n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB5n.qp());
    /* p31.ZUVE*/ top.oam_bus.ZUVE_OAM_LATCH_DB6n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB6n.qp());
    /* p31.ECED*/ top.oam_bus.ECED_OAM_LATCH_DB7n.tp_latch(_BODE_OAM_OEp, top.oam_bus.BUS_OAM_DB7n.qp());
  }

  // OBL -> temp
  {
    /*p29.YWOK*/ wire _YWOK_OAM_CLKn = not1(_COTA_OAM_CLKn);
    /*p29.YWOK*/ wire _YWOK_OAM_CLKp = not1(_YWOK_OAM_CLKn);

    /*#p29.XUSO*/ top.oam_bus.XUSO_OAM_DA0p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.YDYV_OAM_LATCH_DA0n.qp08());
    /* p29.XEGU*/ top.oam_bus.XEGU_OAM_DA1p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.YCEB_OAM_LATCH_DA1n.qp08());
    /* p29.YJEX*/ top.oam_bus.YJEX_OAM_DA2p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.ZUCA_OAM_LATCH_DA2n.qp08());
    /* p29.XYJU*/ top.oam_bus.XYJU_OAM_DA3p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.WONE_OAM_LATCH_DA3n.qp08());
    /* p29.YBOG*/ top.oam_bus.YBOG_OAM_DA4p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.ZAXE_OAM_LATCH_DA4n.qp08());
    /* p29.WYSO*/ top.oam_bus.WYSO_OAM_DA5p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.XAFU_OAM_LATCH_DA5n.qp08());
    /* p29.XOTE*/ top.oam_bus.XOTE_OAM_DA6p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.YSES_OAM_LATCH_DA6n.qp08());
    /* p29.YZAB*/ top.oam_bus.YZAB_OAM_DA7p.dff8n(_YWOK_OAM_CLKn, _YWOK_OAM_CLKp, top.oam_bus.ZECA_OAM_LATCH_DA7n.qp08());

    /* p31.XEGA*/ wire _XEGA_OAM_CLKp = not1(_COTA_OAM_CLKn);
    /* p31.XEGA*/ wire _XEGA_OAM_CLKn = not1(_XEGA_OAM_CLKp);

    /* p31.YLOR*/ top.oam_bus.YLOR_OAM_DB0p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.XYKY_OAM_LATCH_DB0n.qp08());
    /* p31.ZYTY*/ top.oam_bus.ZYTY_OAM_DB1p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.YRUM_OAM_LATCH_DB1n.qp08());
    /* p31.ZYVE*/ top.oam_bus.ZYVE_OAM_DB2p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.YSEX_OAM_LATCH_DB2n.qp08());
    /* p31.ZEZY*/ top.oam_bus.ZEZY_OAM_DB3p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.YVEL_OAM_LATCH_DB3n.qp08());
    /* p31.GOMO*/ top.oam_bus.GOMO_OAM_DB4p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.WYNO_OAM_LATCH_DB4n.qp08());
    /* p31.BAXO*/ top.oam_bus.BAXO_OAM_DB5p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.CYRA_OAM_LATCH_DB5n.qp08());
    /* p31.YZOS*/ top.oam_bus.YZOS_OAM_DB6p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.ZUVE_OAM_LATCH_DB6n.qp08());
    /* p31.DEPO*/ top.oam_bus.DEPO_OAM_DB7p.dff8n(_XEGA_OAM_CLKp, _XEGA_OAM_CLKn, top.oam_bus.ECED_OAM_LATCH_DB7n.qp08());
  }

  // OBL -> CBD
  {
    /* p28.GUKO*/ wire _GUKO_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _WAFO_OAM_A0n);
    /* p28.WUKU*/ wire _WUKU_OBL_TO_CBDp = and3(top.LEKO_CPU_RDp, _AMAB_CPU_OAM_ENp, _GEKA_OAM_A0p);

    /* p28.WUME*/ wire _WUME_OBL_TO_CBDn = not1(_GUKO_OBL_TO_CBDp);
    /* p29.YFAP*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.YDYV_OAM_LATCH_DA0n.qn10());
    /* p29.XELE*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.YCEB_OAM_LATCH_DA1n.qn10());
    /* p29.YPON*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.ZUCA_OAM_LATCH_DA2n.qn10());
    /* p29.XUVO*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.WONE_OAM_LATCH_DA3n.qn10());
    /* p29.ZYSA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.ZAXE_OAM_LATCH_DA4n.qn10());
    /* p29.YWEG*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.XAFU_OAM_LATCH_DA5n.qn10());
    /* p29.XABU*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.YSES_OAM_LATCH_DA6n.qn10());
    /* p29.YTUX*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(_WUME_OBL_TO_CBDn, top.oam_bus.ZECA_OAM_LATCH_DA7n.qn10());

    /* p28.WEWU*/ wire _WEWU_OBL_TO_CBDn = not1(_WUKU_OBL_TO_CBDp);
    /* p31.XACA*/ top.cpu_bus.BUS_CPU_D0p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.XYKY_OAM_LATCH_DB0n.qn10());
    /* p31.XAGU*/ top.cpu_bus.BUS_CPU_D1p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.YRUM_OAM_LATCH_DB1n.qn10());
    /* p31.XEPU*/ top.cpu_bus.BUS_CPU_D2p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.YSEX_OAM_LATCH_DB2n.qn10());
    /* p31.XYGU*/ top.cpu_bus.BUS_CPU_D3p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.YVEL_OAM_LATCH_DB3n.qn10());
    /* p31.XUNA*/ top.cpu_bus.BUS_CPU_D4p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.WYNO_OAM_LATCH_DB4n.qn10());
    /* p31.DEVE*/ top.cpu_bus.BUS_CPU_D5p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.CYRA_OAM_LATCH_DB5n.qn10());
    /* p31.ZEHA*/ top.cpu_bus.BUS_CPU_D6p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.ZUVE_OAM_LATCH_DB6n.qn10());
    /* p31.FYRA*/ top.cpu_bus.BUS_CPU_D7p.tri10_np(_WEWU_OBL_TO_CBDn, top.oam_bus.ECED_OAM_LATCH_DB7n.qn10());
  }

  //------------------------------------------------------------------------------

  wire VYPO = 1;

  /*#p25.TEFA*/ wire _TEFA_8000_9FFFp = nor2(top.cpu_bus.SYRO_FE00_FFFFp(), top.TEXO_ADDR_EXT_AND_NOT_VRAM);
  /*#p25.SOSE*/ wire _SOSE_8000_9FFFp = and2(top.cpu_bus.BUS_CPU_A15.qp(), _TEFA_8000_9FFFp);

  /*#p25.ROPY*/ wire _ROPY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGn.qn03());

  /*#p25.TEFY*/ wire _TEFY_VRAM_MCSp = not1(top.vram_bus.PIN_VRAM_CSn.qn());
  /*#p25.TUCA*/ wire _TUCA_CPU_VRAM_RDp = and2(_SOSE_8000_9FFFp, top.ABUZ_xxCDEFGH);

  /*#p25.TOLE*/ wire _TOLE_VRAM_RDp     = mux2p(top.TUTO_DBG_VRAMp, _TEFY_VRAM_MCSp, _TUCA_CPU_VRAM_RDp);
  /*#p25.SERE*/ wire _SERE_CPU_VRM_RDp = and2(_TOLE_VRAM_RDp, _ROPY_RENDERINGn);

  /*#p25.TAVY*/ wire _TAVY_MOEp = not1(top.vram_bus.PIN_VRAM_OEn.qn());
  /*#p25.TEGU*/ wire _TEGU_CPU_VRAM_WRn = nand2(_SOSE_8000_9FFFp, top.cpu_bus.PIN_CPU_WRp.qp());  // Schematic wrong, second input is PIN_CPU_WRp
  /*#p25.SALE*/ wire _SALE_CPU_VRAM_WRn = mux2p(top.TUTO_DBG_VRAMp, _TAVY_MOEp, _TEGU_CPU_VRAM_WRn);
  /*#p25.RUVY*/ wire _RUVY_VRAM_WRp    = not1(_SALE_CPU_VRAM_WRn);
  /*#p25.SAZO*/ wire _SAZO_CBD_TO_VPDp = and2(_SERE_CPU_VRM_RDp, _RUVY_VRAM_WRp);
  /*#p25.RYJE*/ wire _RYJE_CBD_TO_VPDn = not1(_SAZO_CBD_TO_VPDp);
  /*#p25.REVO*/ wire _REVO_CBD_TO_VPDp = not1(_RYJE_CBD_TO_VPDn);
  /*#p25.RELA*/ wire _RELA_CBD_TO_VPDp = or2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
  /*#p25.RENA*/ wire _RENA_CBD_TO_VPDn = not1(_RELA_CBD_TO_VPDp);

  /*#p25.XANE*/ wire _XANE_VRAM_LOCKn = nor2(top.dma_reg.LUFA_DMA_VRAMp, top.pix_pipe.XYMU_RENDERINGn.qn03());

  /*#p25.RACO*/ wire _RACO_DBG_VRAMn = not1(top.TUTO_DBG_VRAMp);
  (void)_RACO_DBG_VRAMn;

  //----------------------------------------
  // VRAM pins

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WRp = and2(_SOSE_8000_9FFFp, top.APOV_CPU_WRp_xxxxEFGx);
    /*p25.SUDO*/ wire _SUDO_MWRp_C = not1(top.vram_bus.PIN_VRAM_WRn.qn());
    /*p25.TYJY*/ wire _TYJY_VRAM_WRp = mux2p(top.TUTO_DBG_VRAMp, _SUDO_MWRp_C, _TUJA_CPU_VRAM_WRp);

    /*p25.SOHY*/ wire _SOHY_MWRn = nand2(_TYJY_VRAM_WRp, _SERE_CPU_VRM_RDp);

    /*p25.TAXY*/ wire _TAXY_MWRn_A = and2(_SOHY_MWRn, _RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire _SOFY_MWRn_D = or2(_SOHY_MWRn, top.TUTO_DBG_VRAMp);
    /*p25.SYSY*/ wire _SYSY_MWRp_A = not1(_TAXY_MWRn_A);
    /*p25.RAGU*/ wire _RAGU_MWRp_D = not1(_SOFY_MWRn_D);

    top.vram_bus.PIN_VRAM_WRn.io_pin(_SYSY_MWRp_A, _RAGU_MWRp_D);
  }

  {
    /*p25.RYLU*/ wire _RYLU_CPU_VRAM_RDn = nand2(_SALE_CPU_VRAM_WRn, _XANE_VRAM_LOCKn);

    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn = not1(top.sprite_fetcher.SOHO_SPR_VRAM_RDp);
    /*p25.APAM*/ wire _APAM_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_VRAMp);
    /*p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn = not1(top.tile_fetcher.LONY_BG_FETCH_RUNNINGp.qp03());

    /*p25.RACU*/ wire _RACU_MOEn = and4(_RYLU_CPU_VRAM_RDn, _RAWA_SPR_VRAM_RDn, _MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAM_RDn); // def and

    /*p25.SEMA*/ wire _SEMA_MOEn_A = and2(_RACU_MOEn, _RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire _RUTE_MOEn_D = or2(_RACU_MOEn, top.TUTO_DBG_VRAMp); // schematic wrong, second input is RACU
    /*p25.REFO*/ wire _REFO_MOEn_A = not1(_SEMA_MOEn_A);
    /*p25.SAHA*/ wire _SAHA_MOEn_D = not1(_RUTE_MOEn_D);

    top.vram_bus.PIN_VRAM_OEn.io_pin(_REFO_MOEn_A, _SAHA_MOEn_D);
  }

  {
    /*#p25.SUTU*/ wire _SUTU_MCSn = nor4(top.tile_fetcher.LENA_BGW_VRM_RDp,
                                         top.dma_reg.LUFA_DMA_VRAMp,
                                         top.sprite_fetcher.TEXY_SPR_READ_VRAMp,
                                         _SERE_CPU_VRM_RDp);

    /*p25.TODE*/ wire _TODE_MCSn_A = and2(_SUTU_MCSn, _RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire _SEWO_MCSn_D = or2(_SUTU_MCSn, top.TUTO_DBG_VRAMp);
    /*p25.SOKY*/ wire _SOKY_MCSp_A = not1(_TODE_MCSn_A);
    /*p25.SETY*/ wire _SETY_MCSp_D = not1(_SEWO_MCSn_D);

    top.vram_bus.PIN_VRAM_CSn.io_pin(_SOKY_MCSp_A, _SETY_MCSp_D);
  }

  //----------------------------------------
  // VRAM address

  // CPU address -> vram address
  {
    /* p25.XEDU*/ wire _XEDU_CPU_VRAM_RDn = not1(_XANE_VRAM_LOCKn);
    /* p25.XAKY*/ top.vram_bus.BUS_VRAM_A00n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A00.qp());
    /* p25.XUXU*/ top.vram_bus.BUS_VRAM_A01n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A01.qp());
    /* p25.XYNE*/ top.vram_bus.BUS_VRAM_A02n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A02.qp());
    /* p25.XODY*/ top.vram_bus.BUS_VRAM_A03n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A03.qp());
    /* p25.XECA*/ top.vram_bus.BUS_VRAM_A04n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A04.qp());
    /* p25.XOBA*/ top.vram_bus.BUS_VRAM_A05n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A05.qp());
    /* p25.XOPO*/ top.vram_bus.BUS_VRAM_A06n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A06.qp());
    /* p25.XYBO*/ top.vram_bus.BUS_VRAM_A07n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A07.qp());
    /* p25.RYSU*/ top.vram_bus.BUS_VRAM_A08n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A08.qp());
    /* p25.RESE*/ top.vram_bus.BUS_VRAM_A09n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A09.qp());
    /* p25.RUSE*/ top.vram_bus.BUS_VRAM_A10n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A10.qp());
    /* p25.RYNA*/ top.vram_bus.BUS_VRAM_A11n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A11.qp());
    /* p25.RUMO*/ top.vram_bus.BUS_VRAM_A12n.tri_6nn(_XEDU_CPU_VRAM_RDn, top.cpu_bus.BUS_CPU_A12.qp());
  }

  // DMA address -> vram address
  {
    /* p04.AHOC*/ wire _AHOC_DMA_VRAM_RDn = not1(top.dma_reg.LUFA_DMA_VRAMp);
    /* p04.ECAL*/ top.vram_bus.BUS_VRAM_A00n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NAKY_DMA_A00p.qp17());
    /* p04.EGEZ*/ top.vram_bus.BUS_VRAM_A01n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PYRO_DMA_A01p.qp17());
    /* p04.FUHE*/ top.vram_bus.BUS_VRAM_A02n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NEFY_DMA_A02p.qp17());
    /* p04.FYZY*/ top.vram_bus.BUS_VRAM_A03n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.MUTY_DMA_A03p.qp17());
    /* p04.DAMU*/ top.vram_bus.BUS_VRAM_A04n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NYKO_DMA_A04p.qp17());
    /* p04.DAVA*/ top.vram_bus.BUS_VRAM_A05n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PYLO_DMA_A05p.qp17());
    /* p04.ETEG*/ top.vram_bus.BUS_VRAM_A06n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NUTO_DMA_A06p.qp17());
    /*#p04.EREW*/ top.vram_bus.BUS_VRAM_A07n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.MUGU_DMA_A07p.qp17());
    /*#p04.EVAX*/ top.vram_bus.BUS_VRAM_A08n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NAFA_DMA_A08n.qn07());
    /* p04.DUVE*/ top.vram_bus.BUS_VRAM_A09n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PYNE_DMA_A09n.qn07());
    /* p04.ERAF*/ top.vram_bus.BUS_VRAM_A10n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.PARA_DMA_A10n.qn07());
    /* p04.FUSY*/ top.vram_bus.BUS_VRAM_A11n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NYDO_DMA_A11n.qn07());
    /* p04.EXYF*/ top.vram_bus.BUS_VRAM_A12n.tri_6nn(_AHOC_DMA_VRAM_RDn, top.dma_reg.NYGY_DMA_A12n.qn07());
  }

  // Sprite fetcher read
  {
    /*#p29.WUKY*/ wire _WUKY_FLIP_Yp = not1(top.oam_bus.YZOS_OAM_DB6p.qp08());
    /*#p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not1(top.pix_pipe.XYMO_LCDC_SPSIZEn.qn08());

    /*#p29.CYVU*/ wire _CYVU_L0 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L0.qp());
    /*#p29.BORE*/ wire _BORE_L1 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L1.qp());
    /*#p29.BUVY*/ wire _BUVY_L2 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L2.qp());
    /*#p29.WAGO*/ wire _WAGO_L3 = xor2(_WUKY_FLIP_Yp, top.sprite_store.SPR_TRI_L3.qp());
    /*#p29.GEJY*/ wire _GEJY_L3 = amux2(top.oam_bus.XUSO_OAM_DA0p.qp08(), _FUFO_LCDC_SPSIZEn,
                                        top.pix_pipe.XYMO_LCDC_SPSIZEn.qn08(), _WAGO_L3);

    /* p29.ABEM*/ top.vram_bus.BUS_VRAM_A00n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.sprite_fetcher.XUQU_SPRITE_AB);
    /* p29.BAXE*/ top.vram_bus.BUS_VRAM_A01n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _CYVU_L0);
    /* p29.ARAS*/ top.vram_bus.BUS_VRAM_A02n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _BORE_L1);
    /* p29.AGAG*/ top.vram_bus.BUS_VRAM_A03n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _BUVY_L2);
    /* p29.FAMU*/ top.vram_bus.BUS_VRAM_A04n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, _GEJY_L3);
    /*#p29.FUGY*/ top.vram_bus.BUS_VRAM_A05n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XEGU_OAM_DA1p.qp08());
    /* p29.GAVO*/ top.vram_bus.BUS_VRAM_A06n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YJEX_OAM_DA2p.qp08());
    /* p29.WYGA*/ top.vram_bus.BUS_VRAM_A07n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XYJU_OAM_DA3p.qp08());
    /* p29.WUNE*/ top.vram_bus.BUS_VRAM_A08n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YBOG_OAM_DA4p.qp08());
    /* p29.GOTU*/ top.vram_bus.BUS_VRAM_A09n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.WYSO_OAM_DA5p.qp08());
    /* p29.GEGU*/ top.vram_bus.BUS_VRAM_A10n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.XOTE_OAM_DA6p.qp08());
    /* p29.XEHE*/ top.vram_bus.BUS_VRAM_A11n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, top.oam_bus.YZAB_OAM_DA7p.qp08());
    /* p29.DYSO*/ top.vram_bus.BUS_VRAM_A12n.tri_6nn(top.sprite_fetcher.ABON_SPR_VRM_RDn, 0);   // sprites always in low half of tile store
  }

  {
    // Map scroll adder
#pragma warning(push)
#pragma warning(disable:4189)
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0S = add_s(top.lcd_reg.MUWY_Y0p.qp17(), top.pix_pipe.GAVE_SCY0n.qn08(), 0);
    /*#p26.FAFO*/ wire _FAFO_TILE_Y0C = add_c(top.lcd_reg.MUWY_Y0p.qp17(), top.pix_pipe.GAVE_SCY0n.qn08(), 0);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1S = add_s(top.lcd_reg.MYRO_Y1p.qp17(), top.pix_pipe.FYMO_SCY1n.qn08(), _FAFO_TILE_Y0C);
    /* p26.EMUX*/ wire _EMUX_TILE_Y1C = add_c(top.lcd_reg.MYRO_Y1p.qp17(), top.pix_pipe.FYMO_SCY1n.qn08(), _FAFO_TILE_Y0C);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2S = add_s(top.lcd_reg.LEXA_Y2p.qp17(), top.pix_pipe.FEZU_SCY2n.qn08(), _EMUX_TILE_Y1C);
    /* p26.ECAB*/ wire _ECAB_TILE_Y2C = add_c(top.lcd_reg.LEXA_Y2p.qp17(), top.pix_pipe.FEZU_SCY2n.qn08(), _EMUX_TILE_Y1C);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0S  = add_s(top.lcd_reg.LYDO_Y3p.qp17(), top.pix_pipe.FUJO_SCY3n.qn08(), _ECAB_TILE_Y2C);
    /* p26.ETAM*/ wire _ETAM_MAP_Y0C  = add_c(top.lcd_reg.LYDO_Y3p.qp17(), top.pix_pipe.FUJO_SCY3n.qn08(), _ECAB_TILE_Y2C);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1S  = add_s(top.lcd_reg.LOVU_Y4p.qp17(), top.pix_pipe.DEDE_SCY4n.qn08(), _ETAM_MAP_Y0C);
    /* p26.DOTO*/ wire _DOTO_MAP_Y1C  = add_c(top.lcd_reg.LOVU_Y4p.qp17(), top.pix_pipe.DEDE_SCY4n.qn08(), _ETAM_MAP_Y0C);
    /* p26.DABA*/ wire _DABA_MAP_Y2S  = add_s(top.lcd_reg.LEMA_Y5p.qp17(), top.pix_pipe.FOTY_SCY5n.qn08(), _DOTO_MAP_Y1C);
    /* p26.DABA*/ wire _DABA_MAP_Y2C  = add_c(top.lcd_reg.LEMA_Y5p.qp17(), top.pix_pipe.FOTY_SCY5n.qn08(), _DOTO_MAP_Y1C);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3S  = add_s(top.lcd_reg.MATO_Y6p.qp17(), top.pix_pipe.FOHA_SCY6n.qn08(), _DABA_MAP_Y2C);
    /* p26.EFYK*/ wire _EFYK_MAP_Y3C  = add_c(top.lcd_reg.MATO_Y6p.qp17(), top.pix_pipe.FOHA_SCY6n.qn08(), _DABA_MAP_Y2C);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4S  = add_s(top.lcd_reg.LAFO_Y7p.qp17(), top.pix_pipe.FUNY_SCY7n.qn08(), _EFYK_MAP_Y3C);
    /* p26.EJOK*/ wire _EJOK_MAP_Y4C  = add_c(top.lcd_reg.LAFO_Y7p.qp17(), top.pix_pipe.FUNY_SCY7n.qn08(), _EFYK_MAP_Y3C);

    /*#p26.ATAD*/ wire _ATAD_TILE_X0S = add_s(top.pix_pipe.XEHO_X0p.qp17(), top.pix_pipe.DATY_SCX0n.qn08(), 0);
    /*#p26.ATAD*/ wire _ATAD_TILE_X0C = add_c(top.pix_pipe.XEHO_X0p.qp17(), top.pix_pipe.DATY_SCX0n.qn08(), 0);
    /* p26.BEHU*/ wire _BEHU_TILE_X1S = add_s(top.pix_pipe.SAVY_X1p.qp17(), top.pix_pipe.DUZU_SCX1n.qn08(), _ATAD_TILE_X0C);
    /* p26.BEHU*/ wire _BEHU_TILE_X1C = add_c(top.pix_pipe.SAVY_X1p.qp17(), top.pix_pipe.DUZU_SCX1n.qn08(), _ATAD_TILE_X0C);
    /* p26.APYH*/ wire _APYH_TILE_X2S = add_s(top.pix_pipe.XODU_X2p.qp17(), top.pix_pipe.CYXU_SCX2n.qn08(), _BEHU_TILE_X1C);
    /* p26.APYH*/ wire _APYH_TILE_X2C = add_c(top.pix_pipe.XODU_X2p.qp17(), top.pix_pipe.CYXU_SCX2n.qn08(), _BEHU_TILE_X1C);
    /* p26.BABE*/ wire _BABE_MAP_X0S  = add_s(top.pix_pipe.XYDO_X3p.qp17(), top.pix_pipe.GUBO_SCX3n.qn08(), _APYH_TILE_X2C);
    /* p26.BABE*/ wire _BABE_MAP_X0C  = add_c(top.pix_pipe.XYDO_X3p.qp17(), top.pix_pipe.GUBO_SCX3n.qn08(), _APYH_TILE_X2C);
    /* p26.ABOD*/ wire _ABOD_MAP_X1S  = add_s(top.pix_pipe.TUHU_X4p.qp17(), top.pix_pipe.BEMY_SCX4n.qn08(), _BABE_MAP_X0C);
    /* p26.ABOD*/ wire _ABOD_MAP_X1C  = add_c(top.pix_pipe.TUHU_X4p.qp17(), top.pix_pipe.BEMY_SCX4n.qn08(), _BABE_MAP_X0C);
    /* p26.BEWY*/ wire _BEWY_MAP_X2S  = add_s(top.pix_pipe.TUKY_X5p.qp17(), top.pix_pipe.CUZY_SCX5n.qn08(), _ABOD_MAP_X1C);
    /* p26.BEWY*/ wire _BEWY_MAP_X2C  = add_c(top.pix_pipe.TUKY_X5p.qp17(), top.pix_pipe.CUZY_SCX5n.qn08(), _ABOD_MAP_X1C);
    /* p26.BYCA*/ wire _BYCA_MAP_X3S  = add_s(top.pix_pipe.TAKO_X6p.qp17(), top.pix_pipe.CABU_SCX6n.qn08(), _BEWY_MAP_X2C);
    /* p26.BYCA*/ wire _BYCA_MAP_X3C  = add_c(top.pix_pipe.TAKO_X6p.qp17(), top.pix_pipe.CABU_SCX6n.qn08(), _BEWY_MAP_X2C);
    /* p26.ACUL*/ wire _ACUL_MAP_X4S  = add_s(top.pix_pipe.SYBE_X7p.qp17(), top.pix_pipe.BAKE_SCX7n.qn08(), _BYCA_MAP_X3C);
    /* p26.ACUL*/ wire _ACUL_MAP_X4C  = add_c(top.pix_pipe.SYBE_X7p.qp17(), top.pix_pipe.BAKE_SCX7n.qn08(), _BYCA_MAP_X3C);
#pragma warning(pop)

    // Background map read
    {
      /* p26.AXEP*/ top.vram_bus.BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _BABE_MAP_X0S);
      /* p26.AFEB*/ top.vram_bus.BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _ABOD_MAP_X1S);
      /* p26.ALEL*/ top.vram_bus.BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _BEWY_MAP_X2S);
      /* p26.COLY*/ top.vram_bus.BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _BYCA_MAP_X3S);
      /* p26.AJAN*/ top.vram_bus.BUS_VRAM_A04n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _ACUL_MAP_X4S);
      /* p26.DUHO*/ top.vram_bus.BUS_VRAM_A05n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _ETAM_MAP_Y0S);
      /* p26.CASE*/ top.vram_bus.BUS_VRAM_A06n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _DOTO_MAP_Y1S);
      /* p26.CYPO*/ top.vram_bus.BUS_VRAM_A07n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _DABA_MAP_Y2S);
      /* p26.CETA*/ top.vram_bus.BUS_VRAM_A08n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _EFYK_MAP_Y3S);
      /* p26.DAFE*/ top.vram_bus.BUS_VRAM_A09n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, _EJOK_MAP_Y4S);
      /*#p26.AMUV*/ top.vram_bus.BUS_VRAM_A10n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, top.pix_pipe.XAFO_LCDC_BGMAPn.qn08());
      /* p26.COVE*/ top.vram_bus.BUS_VRAM_A11n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
      /* p26.COXO*/ top.vram_bus.BUS_VRAM_A12n.tri_6nn(top.tile_fetcher.BAFY_BG_MAP_READn, 1);
    }

    // Window map read
    {
      /*#p27.XEJA*/ top.vram_bus.BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKA_WIN_X3.qp17());
      /* p27.XAMO*/ top.vram_bus.BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WODY_WIN_X4.qp17());
      /* p27.XAHE*/ top.vram_bus.BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOBO_WIN_X5.qp17());
      /* p27.XULO*/ top.vram_bus.BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WYKO_WIN_X6.qp17());
      /* p27.WUJU*/ top.vram_bus.BUS_VRAM_A04n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.XOLO_WIN_X7.qp17());
      /*#p27.VYTO*/ top.vram_bus.BUS_VRAM_A05n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TUFU_WIN_Y3.qp17());
      /* p27.VEHA*/ top.vram_bus.BUS_VRAM_A06n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TAXA_WIN_Y4.qp17());
      /* p27.VACE*/ top.vram_bus.BUS_VRAM_A07n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TOZO_WIN_Y5.qp17());
      /* p27.VOVO*/ top.vram_bus.BUS_VRAM_A08n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TATE_WIN_Y6.qp17());
      /* p27.VULO*/ top.vram_bus.BUS_VRAM_A09n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.TEKE_WIN_Y7.qp17());
      /*#p27.VEVY*/ top.vram_bus.BUS_VRAM_A10n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, top.pix_pipe.WOKY_LCDC_WINMAPn.qn08());
      /* p27.VEZA*/ top.vram_bus.BUS_VRAM_A11n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
      /* p27.VOGU*/ top.vram_bus.BUS_VRAM_A12n.tri_6nn(top.tile_fetcher.WUKO_WIN_MAP_READn, 1);
    }

    // Background/window tile read
    {
      /*#p26.ASUM*/ top.vram_bus.BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  top.tile_fetcher.XUHA_FETCH_S2p);
      /* p26.EVAD*/ top.vram_bus.BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  _FAFO_TILE_Y0S);
      /* p26.DAHU*/ top.vram_bus.BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  _EMUX_TILE_Y1S);
      /* p26.DODE*/ top.vram_bus.BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.BEJE_BG_TILE_READn,  _ECAB_TILE_Y2S); // check outputs of ECAB

      /*#p25.XONU*/ top.vram_bus.BUS_VRAM_A00n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.tile_fetcher.XUHA_FETCH_S2p);
      /*#p25.WUDO*/ top.vram_bus.BUS_VRAM_A01n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYNO_WIN_Y0.qp17());
      /*#p25.WAWE*/ top.vram_bus.BUS_VRAM_A02n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VUJO_WIN_Y1.qp17());
      /*#p25.WOLU*/ top.vram_bus.BUS_VRAM_A03n.tri_6nn(top.tile_fetcher.XUCY_WIN_TILE_READn, top.pix_pipe.VYMU_WIN_Y2.qp17());

      /*#p25.VUZA*/ wire _VUZA_TILE_BANKp = nor2(top.vram_bus.PYJU_TILE_DB7p.q11p(), top.pix_pipe.WEXU_LCDC_BGTILEn.qn08());

      /*#p25.VAPY*/ top.vram_bus.BUS_VRAM_A04n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.RAWU_TILE_DB0p.q11p());
      /*#p25.SEZU*/ top.vram_bus.BUS_VRAM_A05n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.POZO_TILE_DB1p.q11p());
      /*#p25.VEJY*/ top.vram_bus.BUS_VRAM_A06n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.PYZO_TILE_DB2p.q11p());
      /*#p25.RUSA*/ top.vram_bus.BUS_VRAM_A07n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.POXA_TILE_DB3p.q11p());
      /*#p25.ROHA*/ top.vram_bus.BUS_VRAM_A08n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.PULO_TILE_DB4p.q11p());
      /*#p25.RESO*/ top.vram_bus.BUS_VRAM_A09n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.POJU_TILE_DB5p.q11p());
      /*#p25.SUVO*/ top.vram_bus.BUS_VRAM_A10n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.POWY_TILE_DB6p.q11p());
      /*#p25.TOBO*/ top.vram_bus.BUS_VRAM_A11n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     top.vram_bus.PYJU_TILE_DB7p.q11p());
      /*#p25.VURY*/ top.vram_bus.BUS_VRAM_A12n.tri_6pn(top.tile_fetcher.NETA_TILE_READp,     _VUZA_TILE_BANKp);
    }
  }

  // VRAM addr bus -> VRAM addr pin
  {
    /*p25.MYFU*/ wire _MYFUp = not1(top.vram_bus.BUS_VRAM_A00n.qp());
    /*p25.MASA*/ wire _MASAp = not1(top.vram_bus.BUS_VRAM_A01n.qp());
    /*p25.MYRE*/ wire _MYREp = not1(top.vram_bus.BUS_VRAM_A02n.qp());
    /*p25.MAVU*/ wire _MAVUp = not1(top.vram_bus.BUS_VRAM_A03n.qp());
    /*p25.MEPA*/ wire _MEPAp = not1(top.vram_bus.BUS_VRAM_A04n.qp());
    /*p25.MYSA*/ wire _MYSAp = not1(top.vram_bus.BUS_VRAM_A05n.qp());
    /*p25.MEWY*/ wire _MEWYp = not1(top.vram_bus.BUS_VRAM_A06n.qp());
    /*p25.MUME*/ wire _MUMEp = not1(top.vram_bus.BUS_VRAM_A07n.qp());
    /*p25.VOVA*/ wire _VOVAp = not1(top.vram_bus.BUS_VRAM_A08n.qp());
    /*p25.VODE*/ wire _VODEp = not1(top.vram_bus.BUS_VRAM_A09n.qp());
    /*p25.RUKY*/ wire _RUKYp = not1(top.vram_bus.BUS_VRAM_A10n.qp());
    /*p25.RUMA*/ wire _RUMAp = not1(top.vram_bus.BUS_VRAM_A11n.qp());
    /*p25.REHO*/ wire _REHOp = not1(top.vram_bus.BUS_VRAM_A12n.qp());

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

    top.vram_bus.PIN_VRAM_A00p.io_pin(_LEXEn, _LEXEn);
    top.vram_bus.PIN_VRAM_A01p.io_pin(_LOZUn, _LOZUn);
    top.vram_bus.PIN_VRAM_A02p.io_pin(_LACAn, _LACAn);
    top.vram_bus.PIN_VRAM_A03p.io_pin(_LUVOn, _LUVOn);
    top.vram_bus.PIN_VRAM_A04p.io_pin(_LOLYn, _LOLYn);
    top.vram_bus.PIN_VRAM_A05p.io_pin(_LALOn, _LALOn);
    top.vram_bus.PIN_VRAM_A06p.io_pin(_LEFAn, _LEFAn);
    top.vram_bus.PIN_VRAM_A07p.io_pin(_LUBYn, _LUBYn);
    top.vram_bus.PIN_VRAM_A08p.io_pin(_TUJYn, _TUJYn);
    top.vram_bus.PIN_VRAM_A09p.io_pin(_TAGOn, _TAGOn);
    top.vram_bus.PIN_VRAM_A10p.io_pin(_NUVAn, _NUVAn);
    top.vram_bus.PIN_VRAM_A11p.io_pin(_PEDUn, _PEDUn);
    top.vram_bus.PIN_VRAM_A12p.io_pin(_PONYn, _PONYn);
  }

  //----------------------------------------
  // VRAM data out

  // CBD->VBD->VPD
  {
    /*p25.ROCY*/ wire _ROCY_CBD_TO_VPDp = and2(_REVO_CBD_TO_VPDp, _SAZO_CBD_TO_VPDp);
    /*p25.RAHU*/ wire _RAHU_CBD_TO_VPDn = not1(_ROCY_CBD_TO_VPDp);

    /* p25.TEME*/ top.vram_bus.BUS_VRAM_D0p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D0p.qp());
    /* p25.TEWU*/ top.vram_bus.BUS_VRAM_D1p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*#p25.TYGO*/ top.vram_bus.BUS_VRAM_D2p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D2p.qp());
    /* p25.SOTE*/ top.vram_bus.BUS_VRAM_D3p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D3p.qp());
    /* p25.SEKE*/ top.vram_bus.BUS_VRAM_D4p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D4p.qp());
    /* p25.RUJO*/ top.vram_bus.BUS_VRAM_D5p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D5p.qp());
    /* p25.TOFA*/ top.vram_bus.BUS_VRAM_D6p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D6p.qp());
    /* p25.SUZA*/ top.vram_bus.BUS_VRAM_D7p.tri10_np(_RAHU_CBD_TO_VPDn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p25.ROVE*/ wire _ROVE_CBD_TO_VPDp = not1(_RAHU_CBD_TO_VPDn);
    /*#p25.SEFA*/ wire _SEFA_D0p = and2(top.vram_bus.BUS_VRAM_D0p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SOGO*/ wire _SOGO_D1p = and2(top.vram_bus.BUS_VRAM_D1p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SEFU*/ wire _SEFU_D2p = and2(top.vram_bus.BUS_VRAM_D2p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUNA*/ wire _SUNA_D3p = and2(top.vram_bus.BUS_VRAM_D3p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUMO*/ wire _SUMO_D4p = and2(top.vram_bus.BUS_VRAM_D4p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SAZU*/ wire _SAZU_D5p = and2(top.vram_bus.BUS_VRAM_D5p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SAMO*/ wire _SAMO_D6p = and2(top.vram_bus.BUS_VRAM_D6p.qp(), _ROVE_CBD_TO_VPDp);
    /* p25.SUKE*/ wire _SUKE_D7p = and2(top.vram_bus.BUS_VRAM_D7p.qp(), _ROVE_CBD_TO_VPDp);

    /*#p25.REGE*/ wire _REGE_D0n = not1(_SEFA_D0p);
    /* p25.RYKY*/ wire _RYKY_D1n = not1(_SOGO_D1p);
    /* p25.RAZO*/ wire _RAZO_D2n = not1(_SEFU_D2p);
    /* p25.RADA*/ wire _RADA_D3n = not1(_SUNA_D3p);
    /* p25.RYRO*/ wire _RYRO_D4n = not1(_SUMO_D4p);
    /* p25.REVU*/ wire _REVU_D5n = not1(_SAZU_D5p);
    /* p25.REKU*/ wire _REKU_D6n = not1(_SAMO_D6p);
    /* p25.RYZE*/ wire _RYZE_D7n = not1(_SUKE_D7p);

    /*#p25.SYNU*/ wire _SYNU_D0p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D0p.qp());
    /* p25.SYMA*/ wire _SYMA_D1p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D1p.qp());
    /* p25.ROKO*/ wire _ROKO_D2p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D2p.qp());
    /* p25.SYBU*/ wire _SYBU_D3p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D3p.qp());
    /* p25.SAKO*/ wire _SAKO_D4p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D4p.qp());
    /* p25.SEJY*/ wire _SEJY_D5p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D5p.qp());
    /* p25.SEDO*/ wire _SEDO_D6p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D6p.qp());
    /* p25.SAWU*/ wire _SAWU_D7p = or2(_RAHU_CBD_TO_VPDn, top.vram_bus.BUS_VRAM_D7p.qp());

    /*#p25.RURA*/ wire _RURA_D0n = not1(_SYNU_D0p);
    /* p25.RULY*/ wire _RULY_D1n = not1(_SYMA_D1p);
    /* p25.RARE*/ wire _RARE_D2n = not1(_ROKO_D2p);
    /* p25.RODU*/ wire _RODU_D3n = not1(_SYBU_D3p);
    /* p25.RUBE*/ wire _RUBE_D4n = not1(_SAKO_D4p);
    /* p25.RUMU*/ wire _RUMU_D5n = not1(_SEJY_D5p);
    /* p25.RYTY*/ wire _RYTY_D6n = not1(_SEDO_D6p);
    /* p25.RADY*/ wire _RADY_D7n = not1(_SAWU_D7p);

    /*#p25.ROFA*/ wire _ROFA_CBD_TO_VPDp = not1(_RENA_CBD_TO_VPDn);

    /*#*/ top.vram_bus.PIN_VRAM_D00p.io_pin(_REGE_D0n, _RURA_D0n, _ROFA_CBD_TO_VPDp);
    /* */ top.vram_bus.PIN_VRAM_D01p.io_pin(_RYKY_D1n, _RULY_D1n, _ROFA_CBD_TO_VPDp);
    /* */ top.vram_bus.PIN_VRAM_D02p.io_pin(_RAZO_D2n, _RARE_D2n, _ROFA_CBD_TO_VPDp);
    /* */ top.vram_bus.PIN_VRAM_D03p.io_pin(_RADA_D3n, _RODU_D3n, _ROFA_CBD_TO_VPDp);
    /* */ top.vram_bus.PIN_VRAM_D04p.io_pin(_RYRO_D4n, _RUBE_D4n, _ROFA_CBD_TO_VPDp);
    /* */ top.vram_bus.PIN_VRAM_D05p.io_pin(_REVU_D5n, _RUMU_D5n, _ROFA_CBD_TO_VPDp);
    /* */ top.vram_bus.PIN_VRAM_D06p.io_pin(_REKU_D6n, _RYTY_D6n, _ROFA_CBD_TO_VPDp);
    /* */ top.vram_bus.PIN_VRAM_D07p.io_pin(_RYZE_D7n, _RADY_D7n, _ROFA_CBD_TO_VPDp);
  }

  //----------------------------------------
  // VRAM data in

  // VPD -> VBD
  {
    /*p25.RODY*/ top.vram_bus.BUS_VRAM_D0p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D00p.qn());
    /*p25.REBA*/ top.vram_bus.BUS_VRAM_D1p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D01p.qn());
    /*p25.RYDO*/ top.vram_bus.BUS_VRAM_D2p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D02p.qn());
    /*p25.REMO*/ top.vram_bus.BUS_VRAM_D3p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D03p.qn());
    /*p25.ROCE*/ top.vram_bus.BUS_VRAM_D4p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D04p.qn());
    /*p25.ROPU*/ top.vram_bus.BUS_VRAM_D5p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D05p.qn());
    /*p25.RETA*/ top.vram_bus.BUS_VRAM_D6p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D06p.qn());
    /*p25.RAKU*/ top.vram_bus.BUS_VRAM_D7p.tri_6pn(_RENA_CBD_TO_VPDn, top.vram_bus.PIN_VRAM_D07p.qn());
  }

  // VBD -> CBD
  {
    /* p25.TYVY*/ wire _TYVY_VBD_TO_CBDn = nand2(_SERE_CPU_VRM_RDp, top.LEKO_CPU_RDp);
    /* p25.SEBY*/ wire _SEBY_VBD_TO_CBDp = not1(_TYVY_VBD_TO_CBDn);

    /* p25.RERY*/ wire _RERY_VBUS_D0n = not1(top.vram_bus.BUS_VRAM_D0p.qp());
    /* p25.RUNA*/ wire _RUNA_VBUS_D1n = not1(top.vram_bus.BUS_VRAM_D1p.qp());
    /* p25.RONA*/ wire _RONA_VBUS_D2n = not1(top.vram_bus.BUS_VRAM_D2p.qp());
    /* p25.RUNO*/ wire _RUNO_VBUS_D3n = not1(top.vram_bus.BUS_VRAM_D3p.qp());
    /* p25.SANA*/ wire _SANA_VBUS_D4n = not1(top.vram_bus.BUS_VRAM_D4p.qp());
    /* p25.RORO*/ wire _RORO_VBUS_D5n = not1(top.vram_bus.BUS_VRAM_D5p.qp());
    /* p25.RABO*/ wire _RABO_VBUS_D6n = not1(top.vram_bus.BUS_VRAM_D6p.qp());
    /* p25.SAME*/ wire _SAME_VBUS_D7n = not1(top.vram_bus.BUS_VRAM_D7p.qp());

    /*#p25.RUGA*/ top.cpu_bus.BUS_CPU_D0p.tri_6pn(_SEBY_VBD_TO_CBDp, _RERY_VBUS_D0n);
    /* p25.ROTA*/ top.cpu_bus.BUS_CPU_D1p.tri_6pn(_SEBY_VBD_TO_CBDp, _RUNA_VBUS_D1n);
    /* p25.RYBU*/ top.cpu_bus.BUS_CPU_D2p.tri_6pn(_SEBY_VBD_TO_CBDp, _RONA_VBUS_D2n);
    /* p25.RAJU*/ top.cpu_bus.BUS_CPU_D3p.tri_6pn(_SEBY_VBD_TO_CBDp, _RUNO_VBUS_D3n);
    /* p25.TYJA*/ top.cpu_bus.BUS_CPU_D4p.tri_6pn(_SEBY_VBD_TO_CBDp, _SANA_VBUS_D4n);
    /* p25.REXU*/ top.cpu_bus.BUS_CPU_D5p.tri_6pn(_SEBY_VBD_TO_CBDp, _RORO_VBUS_D5n);
    /* p25.RUPY*/ top.cpu_bus.BUS_CPU_D6p.tri_6pn(_SEBY_VBD_TO_CBDp, _RABO_VBUS_D6n);
    /* p25.TOKU*/ top.cpu_bus.BUS_CPU_D7p.tri_6pn(_SEBY_VBD_TO_CBDp, _SAME_VBUS_D7n);
  }

  // VBD -> tile pix temp
  {
    /*#p32.LOMA*/ wire _LOMA_LATCH_TILE_DAn = not1(top.tile_fetcher.METE_LATCH_TILE_DAp);

    /*p32.LEGU*/ top.vram_bus.LEGU_TILE_DA0n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p32.NUDU*/ top.vram_bus.NUDU_TILE_DA1n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p32.MUKU*/ top.vram_bus.MUKU_TILE_DA2n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p32.LUZO*/ top.vram_bus.LUZO_TILE_DA3n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p32.MEGU*/ top.vram_bus.MEGU_TILE_DA4n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p32.MYJY*/ top.vram_bus.MYJY_TILE_DA5n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p32.NASA*/ top.vram_bus.NASA_TILE_DA6n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p32.NEFO*/ top.vram_bus.NEFO_TILE_DA7n.dff8p(_LOMA_LATCH_TILE_DAn, top.vram_bus.BUS_VRAM_D7p.qp());

    // This is the only block of "dff11" on the chip. Not sure about clock polarity, it seems to work either way.
    /*p??.LUVE*/ wire _LUVE_MATCH_TILE_DBp = not1(top.tile_fetcher.LESO_LATCH_TILE_DBn); // Schematic wrong, was labeled AJAR
    /*p32.LABU*/ wire _LABU_LATCH_TILE_DBn = not1(_LUVE_MATCH_TILE_DBp);

    /*p32.RAWU*/ top.vram_bus.RAWU_TILE_DB0p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D0p.qp());
    /*p32.POZO*/ top.vram_bus.POZO_TILE_DB1p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D1p.qp());
    /*p32.PYZO*/ top.vram_bus.PYZO_TILE_DB2p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D2p.qp());
    /*p32.POXA*/ top.vram_bus.POXA_TILE_DB3p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D3p.qp());
    /*p32.PULO*/ top.vram_bus.PULO_TILE_DB4p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D4p.qp());
    /*p32.POJU*/ top.vram_bus.POJU_TILE_DB5p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D5p.qp());
    /*p32.POWY*/ top.vram_bus.POWY_TILE_DB6p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D6p.qp());
    /*p32.PYJU*/ top.vram_bus.PYJU_TILE_DB7p.dff11(_LABU_LATCH_TILE_DBn, VYPO, top.vram_bus.BUS_VRAM_D7p.qp());
  }

  // VRAM tri -> sprite pix temp + x flip
  {
    /*#p29.XONO*/ wire _XONO_FLIP_X = and2(top.oam_bus.BAXO_OAM_DB5p.qp08(), top.sprite_fetcher.TEXY_SPR_READ_VRAMp);

    /*p33.PUTE*/ wire _PUTE_FLIP0p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D7p.qp(), top.vram_bus.BUS_VRAM_D0p.qp());
    /*p33.PELO*/ wire _PELO_FLIP1p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D6p.qp(), top.vram_bus.BUS_VRAM_D1p.qp());
    /*p33.PONO*/ wire _PONO_FLIP2p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D5p.qp(), top.vram_bus.BUS_VRAM_D2p.qp());
    /*p33.POBE*/ wire _POBE_FLIP3p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D4p.qp(), top.vram_bus.BUS_VRAM_D3p.qp());
    /*p33.PACY*/ wire _PACY_FLIP4p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D3p.qp(), top.vram_bus.BUS_VRAM_D4p.qp());
    /*p33.PUGU*/ wire _PUGU_FLIP5p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D2p.qp(), top.vram_bus.BUS_VRAM_D5p.qp());
    /*p33.PAWE*/ wire _PAWE_FLIP6p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D1p.qp(), top.vram_bus.BUS_VRAM_D6p.qp());
    /*p33.PULY*/ wire _PULY_FLIP7p = mux2p(_XONO_FLIP_X, top.vram_bus.BUS_VRAM_D0p.qp(), top.vram_bus.BUS_VRAM_D7p.qp());

    /*#p29.PEBY*/ wire _PEBY_CLKp = not1(top.sprite_fetcher.RACA_LATCH_SPPIXB);
    /*#p29.NYBE*/ wire _NYBE_CLKn = not1(_PEBY_CLKp);
    /*#p29.PUCO*/ wire _PUCO_CLKp = not1(_NYBE_CLKn);

    top.vram_bus.latch_sprite_a_delay.set(_PUCO_CLKp);

    /*p33.PEFO*/ top.vram_bus.PEFO_SPRITE_DB0n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _PUTE_FLIP0p);
    /*p33.ROKA*/ top.vram_bus.ROKA_SPRITE_DB1n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _PELO_FLIP1p);
    /*p33.MYTU*/ top.vram_bus.MYTU_SPRITE_DB2n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _PONO_FLIP2p);
    /*p33.RAMU*/ top.vram_bus.RAMU_SPRITE_DB3n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _POBE_FLIP3p);
    /*p33.SELE*/ top.vram_bus.SELE_SPRITE_DB4n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _PACY_FLIP4p);
    /*p33.SUTO*/ top.vram_bus.SUTO_SPRITE_DB5n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _PUGU_FLIP5p);
    /*p33.RAMA*/ top.vram_bus.RAMA_SPRITE_DB6n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _PAWE_FLIP6p);
    /*p33.RYDU*/ top.vram_bus.RYDU_SPRITE_DB7n.dff8n(top.vram_bus.latch_sprite_a_delay.q6(), _PULY_FLIP7p);

    /*#p29.VYWA*/ wire _VYWA_CLKp = not1(top.sprite_fetcher.TOPU_LATCH_SPPIXA);
    /*#p29.WENY*/ wire _WENY_CLKn = not1(_VYWA_CLKp);
    /*#p29.XADO*/ wire _XADO_CLKp = not1(_WENY_CLKn);

    top.vram_bus.latch_sprite_b_delay.set(_XADO_CLKp);

    /*p33.REWO*/ top.vram_bus.REWO_SPRITE_DA0n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _PUTE_FLIP0p);
    /*p33.PEBA*/ top.vram_bus.PEBA_SPRITE_DA1n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _PELO_FLIP1p);
    /*p33.MOFO*/ top.vram_bus.MOFO_SPRITE_DA2n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _PONO_FLIP2p);
    /*p33.PUDU*/ top.vram_bus.PUDU_SPRITE_DA3n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _POBE_FLIP3p);
    /*p33.SAJA*/ top.vram_bus.SAJA_SPRITE_DA4n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _PACY_FLIP4p);
    /*p33.SUNY*/ top.vram_bus.SUNY_SPRITE_DA5n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _PUGU_FLIP5p);
    /*p33.SEMO*/ top.vram_bus.SEMO_SPRITE_DA6n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _PAWE_FLIP6p);
    /*p33.SEGA*/ top.vram_bus.SEGA_SPRITE_DA7n.dff8n(top.vram_bus.latch_sprite_b_delay.q6(), _PULY_FLIP7p);
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
