#include "GateBoyLib/GateBoyTimer.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"
#include "GateBoyLib/GateBoyCpuBus.h"

//------------------------------------------------------------------------------------------------------------------------
// this is hacked up because we're ignoring the debug reg for the moment
///* p01.ULUR*/ wire2 _ULUR_DIV_06_clknew = /*mux2p(FF60_1, _BOGA_Axxxxxxx,*/ TAMA_DIV05p.qn_new() /*)*/;
///* p01.UGOT*/ UGOT_DIV06p.dff17_clk(_ULUR_DIV_06_clknew,  UGOT_DIV06p.qn_old());

void GateBoyDiv::tock(GateBoyResetDebug& rst, GateBoyClock& clk, GateBoyCpuBus& cpu_bus)
{
  /* p01.TAPE*/ wire2 _TAPE_FF04_WRp = and4(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.RYFO_FF04_FF07p(), cpu_bus.TOLA_A01n(), cpu_bus.TOVY_A00n());
  /* p01.UFOL*/ wire2 _UFOL_DIV_RSTn = nor3b(clk.UCOB_CLKBADp(), rst.PIN71_RST.int_qp_new(), _TAPE_FF04_WRp);

  /* p01.UKUP*/ UKUP_DIV00p.dff17(clk.BOGA_Axxxxxxx(),  _UFOL_DIV_RSTn, UKUP_DIV00p.qn_old());
  /* p01.UFOR*/ UFOR_DIV01p.dff17(UKUP_DIV00p.qn_any2(), _UFOL_DIV_RSTn, UFOR_DIV01p.qn_old());
  /* p01.UNER*/ UNER_DIV02p.dff17(UFOR_DIV01p.qn_any2(), _UFOL_DIV_RSTn, UNER_DIV02p.qn_old());
  /*#p01.TERO*/ TERO_DIV03p.dff17(UNER_DIV02p.qn_any2(), _UFOL_DIV_RSTn, TERO_DIV03p.qn_old());
  /* p01.UNYK*/ UNYK_DIV04p.dff17(TERO_DIV03p.qn_any2(), _UFOL_DIV_RSTn, UNYK_DIV04p.qn_old());
  /* p01.TAMA*/ TAMA_DIV05p.dff17(UNYK_DIV04p.qn_any2(), _UFOL_DIV_RSTn, TAMA_DIV05p.qn_old());
  /* p01.UGOT*/ UGOT_DIV06p.dff17(TAMA_DIV05p.qn_any2(), _UFOL_DIV_RSTn, UGOT_DIV06p.qn_old());
  /* p01.TULU*/ TULU_DIV07p.dff17(UGOT_DIV06p.qn_any2(), _UFOL_DIV_RSTn, TULU_DIV07p.qn_old());
  /* p01.TUGO*/ TUGO_DIV08p.dff17(TULU_DIV07p.qn_any2(), _UFOL_DIV_RSTn, TUGO_DIV08p.qn_old());
  /* p01.TOFE*/ TOFE_DIV09p.dff17(TUGO_DIV08p.qn_any2(), _UFOL_DIV_RSTn, TOFE_DIV09p.qn_old());
  /* p01.TERU*/ TERU_DIV10p.dff17(TOFE_DIV09p.qn_any2(), _UFOL_DIV_RSTn, TERU_DIV10p.qn_old());
  /* p01.SOLA*/ SOLA_DIV11p.dff17(TERU_DIV10p.qn_any2(), _UFOL_DIV_RSTn, SOLA_DIV11p.qn_old());
  /* p01.SUBU*/ SUBU_DIV12p.dff17(SOLA_DIV11p.qn_any2(), _UFOL_DIV_RSTn, SUBU_DIV12p.qn_old());
  /* p01.TEKA*/ TEKA_DIV13p.dff17(SUBU_DIV12p.qn_any2(), _UFOL_DIV_RSTn, TEKA_DIV13p.qn_old());
  /* p01.UKET*/ UKET_DIV14p.dff17(TEKA_DIV13p.qn_any2(), _UFOL_DIV_RSTn, UKET_DIV14p.qn_old());
  /* p01.UPOF*/ UPOF_DIV15p.dff17(UKET_DIV14p.qn_any2(), _UFOL_DIV_RSTn, UPOF_DIV15p.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyDiv::read(GateBoyCpuBus& cpu_bus)
{
#if 0
  /* p01.UMEK*/ auto _UMEK_DIV06n_evn = not1b(UGOT_DIV06p);
  /* p01.UREK*/ auto _UREK_DIV07n_evn = not1b(TULU_DIV07p);
  /* p01.UTOK*/ auto _UTOK_DIV08n_evn = not1b(TUGO_DIV08p);
  /* p01.SAPY*/ auto _SAPY_DIV09n_evn = not1b(TOFE_DIV09p);
  /* p01.UMER*/ auto _UMER_DIV10n_evn = not1b(TERU_DIV10p);
  /* p01.RAVE*/ auto _RAVE_DIV11n_evn = not1b(SOLA_DIV11p);
  /* p01.RYSO*/ auto _RYSO_DIV12n_evn = not1b(SUBU_DIV12p);
  /* p01.UDOR*/ auto _UDOR_DIV13n_evn = not1b(TEKA_DIV13p);

  /* p01.TAGY*/ auto _TAGY_FF04_RDp_ext = and4(cpu_bus.TEDO_CPU_RDp, BitBase(cpu_bus.RYFO_FF04_FF07p()), BitBase(cpu_bus.TOLA_A01n()), BitBase(cpu_bus.TOVY_A00n()));

  /* p01.TAWU*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_TAGY_FF04_RDp_ext, _UMEK_DIV06n_evn);
  /* p01.TAKU*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_TAGY_FF04_RDp_ext, _UREK_DIV07n_evn);
  /* p01.TEMU*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_TAGY_FF04_RDp_ext, _UTOK_DIV08n_evn);
  /* p01.TUSE*/ cpu_bus.BUS_CPU_D[3].tri6_pn(_TAGY_FF04_RDp_ext, _SAPY_DIV09n_evn);
  /* p01.UPUG*/ cpu_bus.BUS_CPU_D[4].tri6_pn(_TAGY_FF04_RDp_ext, _UMER_DIV10n_evn); // Schematic wrong, UPUG/SEPU driving D5/D4
  /* p01.SEPU*/ cpu_bus.BUS_CPU_D[5].tri6_pn(_TAGY_FF04_RDp_ext, _RAVE_DIV11n_evn);
  /* p01.SAWA*/ cpu_bus.BUS_CPU_D[6].tri6_pn(_TAGY_FF04_RDp_ext, _RYSO_DIV12n_evn);
  /* p01.TATU*/ cpu_bus.BUS_CPU_D[7].tri6_pn(_TAGY_FF04_RDp_ext, _UDOR_DIV13n_evn);
#else
  /* p01.UMEK*/ wire2 _UMEK_DIV06n_evn = not1b(UGOT_DIV06p.qp_new());
  /* p01.UREK*/ wire2 _UREK_DIV07n_evn = not1b(TULU_DIV07p.qp_new());
  /* p01.UTOK*/ wire2 _UTOK_DIV08n_evn = not1b(TUGO_DIV08p.qp_new());
  /* p01.SAPY*/ wire2 _SAPY_DIV09n_evn = not1b(TOFE_DIV09p.qp_new());
  /* p01.UMER*/ wire2 _UMER_DIV10n_evn = not1b(TERU_DIV10p.qp_new());
  /* p01.RAVE*/ wire2 _RAVE_DIV11n_evn = not1b(SOLA_DIV11p.qp_new());
  /* p01.RYSO*/ wire2 _RYSO_DIV12n_evn = not1b(SUBU_DIV12p.qp_new());
  /* p01.UDOR*/ wire2 _UDOR_DIV13n_evn = not1b(TEKA_DIV13p.qp_new());

  /* p01.TAGY*/ wire2 _TAGY_FF04_RDp_ext    = and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.RYFO_FF04_FF07p(), cpu_bus.TOLA_A01n(), cpu_bus.TOVY_A00n());
  /* p01.TAWU*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_TAGY_FF04_RDp_ext, _UMEK_DIV06n_evn);
  /* p01.TAKU*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_TAGY_FF04_RDp_ext, _UREK_DIV07n_evn);
  /* p01.TEMU*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_TAGY_FF04_RDp_ext, _UTOK_DIV08n_evn);
  /* p01.TUSE*/ cpu_bus.BUS_CPU_D[3].tri6_pn(_TAGY_FF04_RDp_ext, _SAPY_DIV09n_evn);
  /* p01.UPUG*/ cpu_bus.BUS_CPU_D[4].tri6_pn(_TAGY_FF04_RDp_ext, _UMER_DIV10n_evn); // Schematic wrong, UPUG/SEPU driving D5/D4
  /* p01.SEPU*/ cpu_bus.BUS_CPU_D[5].tri6_pn(_TAGY_FF04_RDp_ext, _RAVE_DIV11n_evn);
  /* p01.SAWA*/ cpu_bus.BUS_CPU_D[6].tri6_pn(_TAGY_FF04_RDp_ext, _RYSO_DIV12n_evn);
  /* p01.TATU*/ cpu_bus.BUS_CPU_D[7].tri6_pn(_TAGY_FF04_RDp_ext, _UDOR_DIV13n_evn);
#endif

}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyTimer::read_tima(GateBoyCpuBus& cpu_bus) {
  /*#p03.TEDA*/ wire2 _TEDA_FF05_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.TOLA_A01n(), cpu_bus.BUS_CPU_A[ 0].qp_new());
  /*#p03.SOKU*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_TEDA_FF05_RDp, REGA_TIMA0p.qn_new());
  /*#p03.RACY*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_TEDA_FF05_RDp, POVY_TIMA1p.qn_new());
  /*#p03.RAVY*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_TEDA_FF05_RDp, PERU_TIMA2p.qn_new());
  /*#p03.SOSY*/ cpu_bus.BUS_CPU_D[3].tri6_pn(_TEDA_FF05_RDp, RATE_TIMA3p.qn_new());
  /*#p03.SOMU*/ cpu_bus.BUS_CPU_D[4].tri6_pn(_TEDA_FF05_RDp, RUBY_TIMA4p.qn_new());
  /*#p03.SURO*/ cpu_bus.BUS_CPU_D[5].tri6_pn(_TEDA_FF05_RDp, RAGE_TIMA5p.qn_new());
  /*#p03.ROWU*/ cpu_bus.BUS_CPU_D[6].tri6_pn(_TEDA_FF05_RDp, PEDA_TIMA6p.qn_new());
  /*#p03.PUSO*/ cpu_bus.BUS_CPU_D[7].tri6_pn(_TEDA_FF05_RDp, NUGA_TIMA7p.qn_new());

}

void GateBoyTimer::write_tima_async(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  (void)rst;
  (void)cpu_bus;
}

void GateBoyTimer::read_tma(GateBoyCpuBus& cpu_bus) {
  /* p03.TUBY*/ wire2 _TUBY_FF06_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.BUS_CPU_A[ 1].qp_new(), cpu_bus.TOVY_A00n());
  /*#p03.SETE*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_TUBY_FF06_RDp, SABU_TMA0p.qn_new());
  /*#p03.PYRE*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_TUBY_FF06_RDp, NYKE_TMA1p.qn_new());
  /*#p03.NOLA*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_TUBY_FF06_RDp, MURU_TMA2p.qn_new());
  /*#p03.SALU*/ cpu_bus.BUS_CPU_D[3].tri6_pn(_TUBY_FF06_RDp, TYVA_TMA3p.qn_new());
  /*#p03.SUPO*/ cpu_bus.BUS_CPU_D[4].tri6_pn(_TUBY_FF06_RDp, TYRU_TMA4p.qn_new());
  /*#p03.SOTU*/ cpu_bus.BUS_CPU_D[5].tri6_pn(_TUBY_FF06_RDp, SUFY_TMA5p.qn_new());
  /*#p03.REVA*/ cpu_bus.BUS_CPU_D[6].tri6_pn(_TUBY_FF06_RDp, PETO_TMA6p.qn_new());
  /*#p03.SAPU*/ cpu_bus.BUS_CPU_D[7].tri6_pn(_TUBY_FF06_RDp, SETA_TMA7p.qn_new());
}

void GateBoyTimer::write_tma_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  /* p03.TYJU*/ wire2 _TYJU_FF06_WRn = nand4b(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.BUS_CPU_A[ 1].qp_new(), cpu_bus.TOVY_A00n());
  /* p03.SABU*/ SABU_TMA0p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p03.NYKE*/ NYKE_TMA1p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p03.MURU*/ MURU_TMA2p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());
  /* p03.TYVA*/ TYVA_TMA3p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[3].qp_old());
  /* p03.TYRU*/ TYRU_TMA4p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[4].qp_old());
  /* p03.SUFY*/ SUFY_TMA5p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[5].qp_old());
  /* p03.PETO*/ PETO_TMA6p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[6].qp_old());
  /* p03.SETA*/ SETA_TMA7p.dff17(_TYJU_FF06_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[7].qp_old());

}

void GateBoyTimer::read_tac(GateBoyCpuBus& cpu_bus) {
  /* p03.SORA*/ wire2 _SORA_FF07_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.BUS_CPU_A[ 1].qp_new(), cpu_bus.BUS_CPU_A[ 0].qp_new());
  /*#p03.RYLA*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_SORA_FF07_RDp, SOPU_TAC0p.qn_new());
  /*#p03.ROTE*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_SORA_FF07_RDp, SAMY_TAC1p.qn_new());
  /*#p03.SUPE*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_SORA_FF07_RDp, SABO_TAC2p.qn_new());
}

void GateBoyTimer::write_tac_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  /* p03.SARA*/ wire2 _SARA_FF07_WRn = nand4b(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.BUS_CPU_A[ 1].qp_new(), cpu_bus.BUS_CPU_A[ 0].qp_new());
  /* p03.SOPU*/ SOPU_TAC0p.dff17(_SARA_FF07_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[0].qp_old());
  /* p03.SAMY*/ SAMY_TAC1p.dff17(_SARA_FF07_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[1].qp_old());
  /* p03.SABO*/ SABO_TAC2p.dff17(_SARA_FF07_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D[2].qp_old());

}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyTimer::tock2(
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  GateBoyCpuBus& cpu_bus,
  const GateBoyDiv& div)
{

  /*#p03.MERY*/ wire2 _MERY_TIMER_OVERFLOWp_old = nor2b(NUGA_TIMA7p.qp_old(), NYDU_TIMA7p_DELAY.qn_old());
  /*#p03.MOBA*/ MOBA_TIMER_OVERFLOWp.dff17(clk.BOGA_Axxxxxxx(), rst.ALUR_SYS_RSTn(), _MERY_TIMER_OVERFLOWp_old);

  /*#p03.TOPE*/ wire2 _TOPE_FF05_WRn = nand4b(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.TOLA_A01n(), cpu_bus.BUS_CPU_A[ 0].qp_new());
  /*#p03.MUZU*/ wire2 _MUZU_CPU_LOAD_TIMAn  = or2(cpu_bus.SIG_CPU_LATCH_EXT.qp_new(), _TOPE_FF05_WRn);
  /*#p03.MEKE*/ wire2 _MEKE_TIMER_OVERFLOWn = not1b(MOBA_TIMER_OVERFLOWp.qp_new());
  /*#p03.MEXU*/ wire2 _MEXU_TIMA_LOADp      = nand3b(_MUZU_CPU_LOAD_TIMAn, rst.ALUR_SYS_RSTn(), _MEKE_TIMER_OVERFLOWn);
  /*#p03.MUGY*/ wire2 _MUGY_TIMA_MAX_RSTn   = not1b(_MEXU_TIMA_LOADp);
  /*#p03.NYDU*/ NYDU_TIMA7p_DELAY.dff17(clk.BOGA_Axxxxxxx(), _MUGY_TIMA_MAX_RSTn, NUGA_TIMA7p.qp_old());

  /*#p03.UBOT*/ wire2 _UBOT_DIV01n = not1b(div.UFOR_DIV01p.qp_new());
  /*#p03.UVYR*/ wire2 _UVYR_DIV03n = not1b(div.TERO_DIV03p.qp_new());
  /* p01.UVYN*/ wire2 _UVYN_DIV05n = not1b(div.TAMA_DIV05p.qp_new());
  /* p01.UREK*/ wire2 _UREK_DIV07n = not1b(div.TULU_DIV07p.qp_new());

  /*#p03.UKAP*/ wire2 _UKAP_CLK_MUXa  = mux2nb(SOPU_TAC0p.qp_new(), _UVYN_DIV05n, _UVYR_DIV03n);
  /*#p03.TEKO*/ wire2 _TEKO_CLK_MUXb  = mux2nb(SOPU_TAC0p.qp_new(), _UBOT_DIV01n, _UREK_DIV07n);
  /*#p03.TECY*/ wire2 _TECY_CLK_MUXc  = mux2nb(SAMY_TAC1p.qp_new(), _UKAP_CLK_MUXa, _TEKO_CLK_MUXb);
  /*#p03.SOGU*/ wire2 _SOGU_TIMA_CLKn = nor2b(_TECY_CLK_MUXc, SABO_TAC2p.qn_new());

  ///*#p03.TOPE*/ wire2 _TOPE_FF05_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.TOLA_A01n(), cpu_bus.BUS_CPU_A[ 0].qp_new());

  /*#p03.ROKE*/ wire2 _ROKE_TIMA_D0 = mux2nb(_TOPE_FF05_WRn, SABU_TMA0p.qp_new(), cpu_bus.BUS_CPU_D[0].qp_new());
  /*#p03.PETU*/ wire2 _PETU_TIMA_D1 = mux2nb(_TOPE_FF05_WRn, NYKE_TMA1p.qp_new(), cpu_bus.BUS_CPU_D[1].qp_new());
  /*#p03.NYKU*/ wire2 _NYKU_TIMA_D2 = mux2nb(_TOPE_FF05_WRn, MURU_TMA2p.qp_new(), cpu_bus.BUS_CPU_D[2].qp_new());
  /*#p03.SOCE*/ wire2 _SOCE_TIMA_D3 = mux2nb(_TOPE_FF05_WRn, TYVA_TMA3p.qp_new(), cpu_bus.BUS_CPU_D[3].qp_new());
  /*#p03.SALA*/ wire2 _SALA_TIMA_D4 = mux2nb(_TOPE_FF05_WRn, TYRU_TMA4p.qp_new(), cpu_bus.BUS_CPU_D[4].qp_new());
  /*#p03.SYRU*/ wire2 _SYRU_TIMA_D5 = mux2nb(_TOPE_FF05_WRn, SUFY_TMA5p.qp_new(), cpu_bus.BUS_CPU_D[5].qp_new());
  /*#p03.REFU*/ wire2 _REFU_TIMA_D6 = mux2nb(_TOPE_FF05_WRn, PETO_TMA6p.qp_new(), cpu_bus.BUS_CPU_D[6].qp_new());
  /*#p03.RATO*/ wire2 _RATO_TIMA_D7 = mux2nb(_TOPE_FF05_WRn, SETA_TMA7p.qp_new(), cpu_bus.BUS_CPU_D[7].qp_new());

  /*#p03.PUXY*/ wire2 _PUXY_TIMA_D0 = nor2b(rst.MULO_SYS_RSTn(), _ROKE_TIMA_D0);
  /*#p03.NERO*/ wire2 _NERO_TIMA_D1 = nor2b(rst.MULO_SYS_RSTn(), _PETU_TIMA_D1);
  /*#p03.NADA*/ wire2 _NADA_TIMA_D2 = nor2b(rst.MULO_SYS_RSTn(), _NYKU_TIMA_D2);
  /*#p03.REPA*/ wire2 _REPA_TIMA_D3 = nor2b(rst.MULO_SYS_RSTn(), _SOCE_TIMA_D3);
  /*#p03.ROLU*/ wire2 _ROLU_TIMA_D4 = nor2b(rst.MULO_SYS_RSTn(), _SALA_TIMA_D4);
  /*#p03.RUGY*/ wire2 _RUGY_TIMA_D5 = nor2b(rst.MULO_SYS_RSTn(), _SYRU_TIMA_D5);
  /*#p03.PYMA*/ wire2 _PYMA_TIMA_D6 = nor2b(rst.MULO_SYS_RSTn(), _REFU_TIMA_D6);
  /*#p03.PAGU*/ wire2 _PAGU_TIMA_D7 = nor2b(rst.MULO_SYS_RSTn(), _RATO_TIMA_D7);

  ///*#p03.MUZU*/ wire2 _MUZU_CPU_LOAD_TIMAn  = or2(cpu_bus.SIG_CPU_LATCH_EXT.qp_new(), _TOPE_FF05_WRn);
  ///*#p03.MEKE*/ wire2 _MEKE_TIMER_OVERFLOWn = not1(MOBA_TIMER_OVERFLOWp.qp_new());
  ///*#p03.MEXU*/ wire2 _MEXU_TIMA_LOADp      = nand3(_MUZU_CPU_LOAD_TIMAn, rst.ALUR_SYS_RSTn(), _MEKE_TIMER_OVERFLOWn);

#if 0
  /*#p03.REGA*/ REGA_TIMA0p.dff20_load(_MEXU_TIMA_LOADp, _PUXY_TIMA_D0);
  /*#p03.POVY*/ POVY_TIMA1p.dff20_load(_MEXU_TIMA_LOADp, _NERO_TIMA_D1);
  /*#p03.PERU*/ PERU_TIMA2p.dff20_load(_MEXU_TIMA_LOADp, _NADA_TIMA_D2);
  /*#p03.RATE*/ RATE_TIMA3p.dff20_load(_MEXU_TIMA_LOADp, _REPA_TIMA_D3);
  /*#p03.RUBY*/ RUBY_TIMA4p.dff20_load(_MEXU_TIMA_LOADp, _ROLU_TIMA_D4);
  /*#p03.RAGE*/ RAGE_TIMA5p.dff20_load(_MEXU_TIMA_LOADp, _RUGY_TIMA_D5);
  /*#p03.PEDA*/ PEDA_TIMA6p.dff20_load(_MEXU_TIMA_LOADp, _PYMA_TIMA_D6);
  /*#p03.NUGA*/ NUGA_TIMA7p.dff20_load(_MEXU_TIMA_LOADp, _PAGU_TIMA_D7);

  /*#p03.REGA*/ REGA_TIMA0p.dff20_clk(_SOGU_TIMA_CLKn);
  /*#p03.POVY*/ POVY_TIMA1p.dff20_clk(REGA_TIMA0p.qp_any());
  /*#p03.PERU*/ PERU_TIMA2p.dff20_clk(POVY_TIMA1p.qp_any());
  /*#p03.RATE*/ RATE_TIMA3p.dff20_clk(PERU_TIMA2p.qp_any());
  /*#p03.RUBY*/ RUBY_TIMA4p.dff20_clk(RATE_TIMA3p.qp_any());
  /*#p03.RAGE*/ RAGE_TIMA5p.dff20_clk(RUBY_TIMA4p.qp_any());
  /*#p03.PEDA*/ PEDA_TIMA6p.dff20_clk(RAGE_TIMA5p.qp_any());
  /*#p03.NUGA*/ NUGA_TIMA7p.dff20_clk(PEDA_TIMA6p.qp_any());
#endif

  /*#p03.REGA*/ REGA_TIMA0p.dff20(_SOGU_TIMA_CLKn,       _MEXU_TIMA_LOADp, _PUXY_TIMA_D0);
  /*#p03.POVY*/ POVY_TIMA1p.dff20(REGA_TIMA0p.qp_any2(), _MEXU_TIMA_LOADp, _NERO_TIMA_D1);
  /*#p03.PERU*/ PERU_TIMA2p.dff20(POVY_TIMA1p.qp_any2(), _MEXU_TIMA_LOADp, _NADA_TIMA_D2);
  /*#p03.RATE*/ RATE_TIMA3p.dff20(PERU_TIMA2p.qp_any2(), _MEXU_TIMA_LOADp, _REPA_TIMA_D3);
  /*#p03.RUBY*/ RUBY_TIMA4p.dff20(RATE_TIMA3p.qp_any2(), _MEXU_TIMA_LOADp, _ROLU_TIMA_D4);
  /*#p03.RAGE*/ RAGE_TIMA5p.dff20(RUBY_TIMA4p.qp_any2(), _MEXU_TIMA_LOADp, _RUGY_TIMA_D5);
  /*#p03.PEDA*/ PEDA_TIMA6p.dff20(RAGE_TIMA5p.qp_any2(), _MEXU_TIMA_LOADp, _PYMA_TIMA_D6);
  /*#p03.NUGA*/ NUGA_TIMA7p.dff20(PEDA_TIMA6p.qp_any2(), _MEXU_TIMA_LOADp, _PAGU_TIMA_D7);
}

//------------------------------------------------------------------------------------------------------------------------