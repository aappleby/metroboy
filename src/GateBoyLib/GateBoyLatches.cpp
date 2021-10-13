#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

// This code is not for running, it's for trying to figure out what might trigger glitches in GateBoy's latches.


void GateBoy::tock_latches() {
{
   // TAKA

  /*#p21.XYVO*/ wire XYVO_y144p_old = and2(gb_state.reg_ly.LOVU_LY4p_odd.qp_old(), gb_state.reg_ly.LAFO_LY7p_odd.qp_old()); // 128 + 16 = 144
  /*#p21.PURE*/ wire PURE_x113n_old = not1(gb_state.lcd.RUTU_LINE_ENDp_odd.qp_old());
  /*#p29.ALES*/ wire ALES_y144n_old = not1(XYVO_y144p_old);
  /*#p21.SELA*/ wire SELA_x113p_old = not1(PURE_x113n_old);
  /*#p29.ABOV*/ wire ABOV_x113p_old = and2(SELA_x113p_old, ALES_y144n_old);
  /*#p29.CATU*/ gb_state.lcd.CATU_LINE_ENDp_odd.dff17(gb_state.sys_clk.XUPY_ABxxEFxx_new(), ABEZ_VID_RSTn(), ABOV_x113p_old);
  /*#p28.ANEL*/ gb_state.lcd.ANEL_LINE_ENDp_odd.dff17(gb_state.sys_clk.AWOH_xxCDxxGH_new(), ABEZ_VID_RSTn(), gb_state.lcd.CATU_LINE_ENDp_odd.qp_old());

  /*_p28.ABAF*/ wire ABAF_x113n_odd = not1(gb_state.lcd.CATU_LINE_ENDp_odd.qp_new());
  /*_p28.BYHA*/ wire BYHA_LINE_RST_TRIGn_odd = or_and3(gb_state.lcd.ANEL_LINE_ENDp_odd.qp_new(), ABAF_x113n_odd, ABEZ_VID_RSTn()); // so if this is or_and, BYHA should go low on 910 and 911

  /*_p28.ATEJ*/ gb_state.ATEJ_LINE_RST_TRIGp_odd <<= not1(BYHA_LINE_RST_TRIGn_odd);
  /*_p27.ROMO*/ wire ROMO_PRELOAD_DONEn_evn = not1(gb_state.tfetch_control.POKY_PRELOAD_LATCHp_evn.qp_new());
  /*_p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(gb_state.XYMU_RENDERING_LATCHn.qn_new(), ROMO_PRELOAD_DONEn_evn, gb_state.tfetch_control.NYKA_FETCH_DONEp_evn.qp_new(), gb_state.tfetch_control.PORY_FETCH_DONEp_odd.qp_new());
  /*_p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);
  /*_p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(gb_state.sfetch_control.SOBU_SFETCH_REQp_evn.qp_new(), gb_state.sfetch_control.SUDA_SFETCH_REQp_odd.qn_new());
  /*#p27.SECA*/ wire SECA_SFETCH_STARTn_evn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), gb_state.ATEJ_LINE_RST_TRIGp_odd.out_new());
  /*_p27.VEKU*/ wire VEKU_SFETCH_ENDn = nor2(gb_state.sfetch_control.WUTY_SFETCH_DONE_TRIGp_odd.out_new(), TAVE_PRELOAD_DONE_TRIGp);
  /*_p27.TAKA*/ gb_state.sfetch_control.TAKA_SFETCH_RUNNINGp_evn.nand_latch(SECA_SFETCH_STARTn_evn, VEKU_SFETCH_ENDn);
}

}






#if 0


// ALOR external address latch. We know this one is glitchy.
  /*_p08.ALOR*/ gb_state.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.tp_latchn(MATE_HOLDn, gb_state.cpu_abus.BUS_CPU_A00p.out_new());
void latch1() {
  ALOR_EXT_ADDR_LATCH_00p.tp_latchn(
    // if this glitches high, latch fires
    SIG_IN_CPU_EXT_BUSp && or3(BUS_CPU_A13p, BUS_CPU_A14p, !BUS_CPU_A15p),
    BUS_CPU_A00p);
}






  /*_p27.LONY*/ NandLatch LONY_TFETCHINGp;       // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*_p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp_evn;  // xBxDxFxG

  /*#p04.LYXE*/ NorLatch LYXE_DMA_LATCHp; // xxxxExxx
  /*_p21.RUPO*/ NorLatch RUPO_LYC_MATCHn;
  /*#p21.WUSA*/ NorLatch WUSA_LCD_CLOCK_GATE; // xBxDxFGH High on G at beginning of line, low on H at end of line. Not sure what's up with the others. Scroll/sprite count?
  /*_p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx
  /*_p27.PYNU*/ NorLatch PYNU_WIN_MODE_Ap_odd;      // AxxxxxGx
  /*_p27.REJO*/ NorLatch REJO_WY_MATCH_LATCHp_odd;  // xxCxxxxx
  /*#p27.ROXY*/ NorLatch ROXY_FINE_SCROLL_DONEn_evn;    // xBxDxFxH
  /*_p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping SIG_CPU_INT_RESETp when UPOF_DIV_15 changed
  /*_p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*#p28.BESU*/ NorLatch BESU_SCAN_DONEn_odd;
  /*#p21.XYMU*/ NorLatch XYMU_RENDERINGn;


  /*#p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_D0n; // AxCxExxx
  /*_p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_D1n; // AxCxExxx
  /*_p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_D2n; // AxCxExxx
  /*_p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_D3n; // AxCxExxx
  /*_p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_D4n; // AxCxExxx
  /*_p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_D5n; // AxCxExxx
  /*_p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_D6n; // AxCxExxx
  /*_p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_D7n; // AxCxExxx

  /*_p08.ALOR*/ TpLatch ALOR_EXT_ADDR_LATCH_00p; // xBxxxxxx
  /*_p08.APUR*/ TpLatch APUR_EXT_ADDR_LATCH_01p; // xBxxxxxx
  /*_p08.ALYR*/ TpLatch ALYR_EXT_ADDR_LATCH_02p; // xBxxxxxx
  /*_p08.ARET*/ TpLatch ARET_EXT_ADDR_LATCH_03p; // xBxxxxxx
  /*_p08.AVYS*/ TpLatch AVYS_EXT_ADDR_LATCH_04p; // xBxxxxxx
  /*_p08.ATEV*/ TpLatch ATEV_EXT_ADDR_LATCH_05p; // xBxxxxxx
  /*_p08.AROS*/ TpLatch AROS_EXT_ADDR_LATCH_06p; // xBxxxxxx
  /*_p08.ARYM*/ TpLatch ARYM_EXT_ADDR_LATCH_07p; // xBxxxxxx
  /*_p08.LUNO*/ TpLatch LUNO_EXT_ADDR_LATCH_08p; // ABxxxxxx
  /*_p08.LYSA*/ TpLatch LYSA_EXT_ADDR_LATCH_09p; // ABxxxxxx
  /*_p08.PATE*/ TpLatch PATE_EXT_ADDR_LATCH_10p; // ABxxxxxx
  /*_p08.LUMY*/ TpLatch LUMY_EXT_ADDR_LATCH_11p; // ABxxxxxx
  /*_p08.LOBU*/ TpLatch LOBU_EXT_ADDR_LATCH_12p; // ABxxxxxx
  /*_p08.LONU*/ TpLatch LONU_EXT_ADDR_LATCH_13p; // ABxxxxxx
  /*_p08.NYRE*/ TpLatch NYRE_EXT_ADDR_LATCH_14p; // ABxxxxxx

  /*_p02.MATY*/ TpLatch MATY_FF0F_L0p;
  /*_p02.MOPO*/ TpLatch MOPO_FF0F_L1p;
  /*_p02.PAVY*/ TpLatch PAVY_FF0F_L2p;
  /*_p02.NEJY*/ TpLatch NEJY_FF0F_L3p;
  /*_p02.NUTY*/ TpLatch NUTY_FF0F_L4p;
  /*_p02.AWOB*/ TpLatch AWOB_WAKE_CPU;

  /*#p05.KEVU*/ TpLatch KEVU_JOYP_L0n;
  /*#p05.KAPA*/ TpLatch KAPA_JOYP_L1n;
  /*#p05.KEJA*/ TpLatch KEJA_JOYP_L2n;
  /*#p05.KOLO*/ TpLatch KOLO_JOYP_L3n;

  /*#p29.YDYV*/ TpLatch YDYV_OAM_LATCH_DA0n; // xBxDxFxx // Proooobably all odd clocks?
  /*_p29.YCEB*/ TpLatch YCEB_OAM_LATCH_DA1n; // xBxDxFxx
  /*_p29.ZUCA*/ TpLatch ZUCA_OAM_LATCH_DA2n; // xBxDxFxx
  /*_p29.WONE*/ TpLatch WONE_OAM_LATCH_DA3n; // xBxDxFxx
  /*_p29.ZAXE*/ TpLatch ZAXE_OAM_LATCH_DA4n; // xBxDxFxx
  /*_p29.XAFU*/ TpLatch XAFU_OAM_LATCH_DA5n; // xBxDxFxx
  /*_p29.YSES*/ TpLatch YSES_OAM_LATCH_DA6n; // xBxDxFxx
  /*_p29.ZECA*/ TpLatch ZECA_OAM_LATCH_DA7n; // xBxDxFxx

  /*#p31.XYKY*/ TpLatch XYKY_OAM_LATCH_DB0n; // xBxxxFxx // Proooobably all odd clocks?
  /*_p31.YRUM*/ TpLatch YRUM_OAM_LATCH_DB1n; // xBxxxFxx
  /*_p31.YSEX*/ TpLatch YSEX_OAM_LATCH_DB2n; // xBxxxFxx
  /*_p31.YVEL*/ TpLatch YVEL_OAM_LATCH_DB3n; // xBxxxFxx
  /*_p31.WYNO*/ TpLatch WYNO_OAM_LATCH_DB4n; // xBxxxFxx
  /*_p31.CYRA*/ TpLatch CYRA_OAM_LATCH_DB5n; // xBxxxFxx
  /*_p31.ZUVE*/ TpLatch ZUVE_OAM_LATCH_DB6n; // xBxxxFxx
  /*_p31.ECED*/ TpLatch ECED_OAM_LATCH_DB7n; // xBxxxFxx

#endif
