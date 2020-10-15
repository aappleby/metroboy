#include "GateBoyLib/Sch_Timer.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

void Timer::dump(Dumper& d) const {
  d("\002===== Timer =====\001\n");
  d("DIV    : 0x%04x %d\n", get_div(), get_div());
  d("TIMA A : 0x%02x %d\n", get_tima_a(), get_tima_a());
  d("TIMA B : 0x%02x %d\n", get_tima_b(), get_tima_b());
  d("TMA    : 0x%02x %d\n", get_tma(), get_tma());
  d("TAC    : 0x%02x %d\n", get_tac(), get_tac());
  d("NYDU_TIMA_D7_DELAY   %c\n", NYDU_TIMA_D7_DELAY.c());
  d("MOBA_TIMER_OVERFLOWp %c\n", MOBA_TIMER_OVERFLOWp.c());
  d("\n");
}


//------------------------------------------------------------------------------

void Timer::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void Timer::tock(wire RST, const SchematicTop& top, CpuBus& cpu_bus) {
  /*p03.MULO*/ wire MULO_SYS_RSTn = not1(top.clk_reg.ALUR_SYS_RSTn);
  /*p01.BOGA*/ wire BOGA_Axxxxxxx = not1(top.clk_reg.BALY_xBCDEFGH);
  /*p03.RYFO*/ wire RYFO_FF04_FF07p = and3(top.cpu_bus.BUS_CPU_A02.qp(), top.cpu_bus.SARE_XX00_XX07p(), top.cpu_bus.SYKE_FF00_FFFFp());

  /* p01.UTOK*/ wire UMEK_DIV_06n = not1(UGOT_DIV_06.qp17());
  /*#p01.UREK*/ wire UREK_DIV_07n = not1(TULU_DIV_07.qp17());
  /* p01.UTOK*/ wire UTOK_DIV_08n = not1(TUGO_DIV_08.qp17());
  /* p01.SAPY*/ wire SAPY_DIV_09n = not1(TOFE_DIV_09.qp17());
  /* p01.UMER*/ wire UMER_DIV_10n = not1(TERU_DIV_10.qp17());
  /* p01.RAVE*/ wire RAVE_DIV_11n = not1(SOLA_DIV_11.qp17());
  /* p01.RYSO*/ wire RYSO_DIV_12n = not1(SUBU_DIV_12.qp17());
  /* p01.UDOR*/ wire UDOR_DIV_13n = not1(TEKA_DIV_13.qp17());

  // FF04 DIV
  {
    /*p01.TAGY*/ wire TAGY_FF04_RDp = and4(top.TEDO_CPU_RDp, RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());
    /*p01.TAPE*/ wire TAPE_FF04_WRp = and4(top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());
    /*p01.UFOL*/ wire UFOL_DIV_RSTn = nor3(top.clk_reg.UCOB_CLKBADp, RST, TAPE_FF04_WRp);

    /* p01.UKUP*/ UKUP_DIV_00.dff17(BOGA_Axxxxxxx,    UFOL_DIV_RSTn, UKUP_DIV_00.qn16());
    /* p01.UFOR*/ UFOR_DIV_01.dff17(UKUP_DIV_00.qn16(), UFOL_DIV_RSTn, UFOR_DIV_01.qn16());
    /* p01.UNER*/ UNER_DIV_02.dff17(UFOR_DIV_01.qn16(), UFOL_DIV_RSTn, UNER_DIV_02.qn16());
    /*#p01.TERO*/ TERO_DIV_03.dff17(UNER_DIV_02.qn16(), UFOL_DIV_RSTn, TERO_DIV_03.qn16());
    /* p01.UNYK*/ UNYK_DIV_04.dff17(TERO_DIV_03.qn16(), UFOL_DIV_RSTn, UNYK_DIV_04.qn16());
    /* p01.TAMA*/ TAMA_DIV_05.dff17(UNYK_DIV_04.qn16(), UFOL_DIV_RSTn, TAMA_DIV_05.qn16());

    ///*p01.ULUR*/ wire ULUR_DIV_06_CLK = mux2p(BOGA_AxCDEFGH, DIV_05, FF60_1);
    ///
    /* p01.ULUR*/ wire ULUR_DIV_06_CLK = TAMA_DIV_05.qn16();
    /* p01.UGOT*/ UGOT_DIV_06.dff17(ULUR_DIV_06_CLK,  UFOL_DIV_RSTn, UGOT_DIV_06.qn16());
    /* p01.TULU*/ TULU_DIV_07.dff17(UGOT_DIV_06.qn16(), UFOL_DIV_RSTn, TULU_DIV_07.qn16());
    /* p01.TUGO*/ TUGO_DIV_08.dff17(TULU_DIV_07.qn16(), UFOL_DIV_RSTn, TUGO_DIV_08.qn16());
    /* p01.TOFE*/ TOFE_DIV_09.dff17(TUGO_DIV_08.qn16(), UFOL_DIV_RSTn, TOFE_DIV_09.qn16());
    /* p01.TERU*/ TERU_DIV_10.dff17(TOFE_DIV_09.qn16(), UFOL_DIV_RSTn, TERU_DIV_10.qn16());
    /* p01.SOLA*/ SOLA_DIV_11.dff17(TERU_DIV_10.qn16(), UFOL_DIV_RSTn, SOLA_DIV_11.qn16());
    /* p01.SUBU*/ SUBU_DIV_12.dff17(SOLA_DIV_11.qn16(), UFOL_DIV_RSTn, SUBU_DIV_12.qn16());
    /* p01.TEKA*/ TEKA_DIV_13.dff17(SUBU_DIV_12.qn16(), UFOL_DIV_RSTn, TEKA_DIV_13.qn16());
    /* p01.UKET*/ UKET_DIV_14.dff17(TEKA_DIV_13.qn16(), UFOL_DIV_RSTn, UKET_DIV_14.qn16());
    /* p01.UPOF*/ UPOF_DIV_15.dff17(UKET_DIV_14.qn16(), UFOL_DIV_RSTn, UPOF_DIV_15.qn16());

    /* p01.TAWU*/ cpu_bus.BUS_CPU_D0p.tri_6pn(TAGY_FF04_RDp, UMEK_DIV_06n);
    /* p01.TAKU*/ cpu_bus.BUS_CPU_D1p.tri_6pn(TAGY_FF04_RDp, UREK_DIV_07n);
    /* p01.TEMU*/ cpu_bus.BUS_CPU_D2p.tri_6pn(TAGY_FF04_RDp, UTOK_DIV_08n);
    /* p01.TUSE*/ cpu_bus.BUS_CPU_D3p.tri_6pn(TAGY_FF04_RDp, SAPY_DIV_09n);
    /* p01.UPUG*/ cpu_bus.BUS_CPU_D4p.tri_6pn(TAGY_FF04_RDp, UMER_DIV_10n); // Schematic wrong, UPUG/SEPU driving D5/D4
    /* p01.SEPU*/ cpu_bus.BUS_CPU_D5p.tri_6pn(TAGY_FF04_RDp, RAVE_DIV_11n);
    /* p01.SAWA*/ cpu_bus.BUS_CPU_D6p.tri_6pn(TAGY_FF04_RDp, RYSO_DIV_12n);
    /* p01.TATU*/ cpu_bus.BUS_CPU_D7p.tri_6pn(TAGY_FF04_RDp, UDOR_DIV_13n);
  }

  // FF05 TIMA
  {
    /*#p03.TEDA*/ wire TEDA_FF05_RDp = and4(RYFO_FF04_FF07p, top.TEDO_CPU_RDp, top.cpu_bus.TOLA_A01n(), top.cpu_bus.BUS_CPU_A00.qp());
    /*#p03.TOPE*/ wire TOPE_FF05_WRn = nand4(top.cpu_bus.BUS_CPU_A00.qp(), top.cpu_bus.TOLA_A01n(), top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p);

    /*#p03.UBOT*/ wire UBOT_CLK_256Kn = not1(UFOR_DIV_01.qp17());
    /*#p03.UVYR*/ wire UVYR_CLK_64Kn = not1(TERO_DIV_03.qp17());

    /* p01.UVYN*/ wire UVYN_DIV_05n = not1(TAMA_DIV_05.qp17());
    /*#p03.UKAP*/ wire UKAP_CLK_MUXa = mux2n(SOPU_TAC_D0.qp17(), UVYN_DIV_05n,   UVYR_CLK_64Kn);
    /*#p03.TEKO*/ wire TEKO_CLK_MUXb = mux2n(SOPU_TAC_D0.qp17(), UBOT_CLK_256Kn, UREK_DIV_07n);
    /*#p03.TECY*/ wire TECY_CLK_MUXc = mux2n(SAMY_TAC_D1.qp17(), UKAP_CLK_MUXa,  TEKO_CLK_MUXb);

    /*#p03.ROKE*/ wire ROKE_TIMA_D0 = mux2n(TOPE_FF05_WRn, SABU_TMA_D0.qp17(), top.cpu_bus.BUS_CPU_D0p.qp());
    /*#p03.PETU*/ wire PETU_TIMA_D1 = mux2n(TOPE_FF05_WRn, NYKE_TMA_D1.qp17(), top.cpu_bus.BUS_CPU_D1p.qp());
    /*#p03.NYKU*/ wire NYKU_TIMA_D2 = mux2n(TOPE_FF05_WRn, MURU_TMA_D2.qp17(), top.cpu_bus.BUS_CPU_D2p.qp());
    /*#p03.SOCE*/ wire SOCE_TIMA_D3 = mux2n(TOPE_FF05_WRn, TYVA_TMA_D3.qp17(), top.cpu_bus.BUS_CPU_D3p.qp());
    /*#p03.SALA*/ wire SALA_TIMA_D4 = mux2n(TOPE_FF05_WRn, TYRU_TMA_D4.qp17(), top.cpu_bus.BUS_CPU_D4p.qp());
    /*#p03.SYRU*/ wire SYRU_TIMA_D5 = mux2n(TOPE_FF05_WRn, SUFY_TMA_D5.qp17(), top.cpu_bus.BUS_CPU_D5p.qp());
    /*#p03.REFU*/ wire REFU_TIMA_D6 = mux2n(TOPE_FF05_WRn, PETO_TMA_D6.qp17(), top.cpu_bus.BUS_CPU_D6p.qp());
    /*#p03.RATO*/ wire RATO_TIMA_D7 = mux2n(TOPE_FF05_WRn, SETA_TMA_D7.qp17(), top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p03.PUXY*/ wire PUXY_TIMA_D0 = nor2(MULO_SYS_RSTn, ROKE_TIMA_D0);
    /*#p03.NERO*/ wire NERO_TIMA_D1 = nor2(MULO_SYS_RSTn, PETU_TIMA_D1);
    /*#p03.NADA*/ wire NADA_TIMA_D2 = nor2(MULO_SYS_RSTn, NYKU_TIMA_D2);
    /*#p03.REPA*/ wire REPA_TIMA_D3 = nor2(MULO_SYS_RSTn, SOCE_TIMA_D3);
    /*#p03.ROLU*/ wire ROLU_TIMA_D4 = nor2(MULO_SYS_RSTn, SALA_TIMA_D4);
    /*#p03.RUGY*/ wire RUGY_TIMA_D5 = nor2(MULO_SYS_RSTn, SYRU_TIMA_D5);
    /*#p03.PYMA*/ wire PYMA_TIMA_D6 = nor2(MULO_SYS_RSTn, REFU_TIMA_D6);
    /*#p03.PAGU*/ wire PAGU_TIMA_D7 = nor2(MULO_SYS_RSTn, RATO_TIMA_D7);

    /*#p03.MEKE*/ wire MEKE_TIMER_OVERFLOWn = not1(MOBA_TIMER_OVERFLOWp.qp17());
    /*#p03.MUZU*/ wire MUZU_CPU_LOAD_TIMAn  = or2(top.cpu_bus.PIN_CPU_LATCH_EXT.qp(), TOPE_FF05_WRn);
    /*#p03.MEXU*/ wire MEXU_TIMA_LOADp      = nand3(MUZU_CPU_LOAD_TIMAn, top.clk_reg.ALUR_SYS_RSTn, MEKE_TIMER_OVERFLOWn);

    /*#p03.SOGU*/ wire SOGU_TIMA_CLK = nor2(TECY_CLK_MUXc, SABO_TAC_D2.qn16());
    /*#p03.REGA*/ REGA_TIMA_D0.dff20(SOGU_TIMA_CLK,     MEXU_TIMA_LOADp, PUXY_TIMA_D0);
    /*#p03.POVY*/ POVY_TIMA_D1.dff20(REGA_TIMA_D0.qp01(), MEXU_TIMA_LOADp, NERO_TIMA_D1);
    /*#p03.PERU*/ PERU_TIMA_D2.dff20(POVY_TIMA_D1.qp01(), MEXU_TIMA_LOADp, NADA_TIMA_D2);
    /*#p03.RATE*/ RATE_TIMA_D3.dff20(PERU_TIMA_D2.qp01(), MEXU_TIMA_LOADp, REPA_TIMA_D3);
    /*#p03.RUBY*/ RUBY_TIMA_D4.dff20(RATE_TIMA_D3.qp01(), MEXU_TIMA_LOADp, ROLU_TIMA_D4);
    /*#p03.RAGE*/ RAGE_TIMA_D5.dff20(RUBY_TIMA_D4.qp01(), MEXU_TIMA_LOADp, RUGY_TIMA_D5);
    /*#p03.PEDA*/ PEDA_TIMA_D6.dff20(RAGE_TIMA_D5.qp01(), MEXU_TIMA_LOADp, PYMA_TIMA_D6);
    /*#p03.NUGA*/ NUGA_TIMA_D7.dff20(PEDA_TIMA_D6.qp01(), MEXU_TIMA_LOADp, PAGU_TIMA_D7);

    /*#p03.SOKU*/ cpu_bus.BUS_CPU_D0p.tri_6pn(TEDA_FF05_RDp, REGA_TIMA_D0.qn17());
    /*#p03.RACY*/ cpu_bus.BUS_CPU_D1p.tri_6pn(TEDA_FF05_RDp, POVY_TIMA_D1.qn17());
    /*#p03.RAVY*/ cpu_bus.BUS_CPU_D2p.tri_6pn(TEDA_FF05_RDp, PERU_TIMA_D2.qn17());
    /*#p03.SOSY*/ cpu_bus.BUS_CPU_D3p.tri_6pn(TEDA_FF05_RDp, RATE_TIMA_D3.qn17());
    /*#p03.SOMU*/ cpu_bus.BUS_CPU_D4p.tri_6pn(TEDA_FF05_RDp, RUBY_TIMA_D4.qn17());
    /*#p03.SURO*/ cpu_bus.BUS_CPU_D5p.tri_6pn(TEDA_FF05_RDp, RAGE_TIMA_D5.qn17());
    /*#p03.ROWU*/ cpu_bus.BUS_CPU_D6p.tri_6pn(TEDA_FF05_RDp, PEDA_TIMA_D6.qn17());
    /*#p03.PUSO*/ cpu_bus.BUS_CPU_D7p.tri_6pn(TEDA_FF05_RDp, NUGA_TIMA_D7.qn17());

    /*#p03.MUGY*/ wire MUGY_TIMA_MAX_RSTn = not1(MEXU_TIMA_LOADp);
    /*#p03.NYDU*/ NYDU_TIMA_D7_DELAY.dff17(BOGA_Axxxxxxx, MUGY_TIMA_MAX_RSTn, NUGA_TIMA_D7.qp01());

    /*#p03.MERY*/ wire MERY_TIMER_OVERFLOWp = nor2(NUGA_TIMA_D7.qp01(), NYDU_TIMA_D7_DELAY.qn16());
    /*#p03.MOBA*/ MOBA_TIMER_OVERFLOWp.dff17(BOGA_Axxxxxxx, top.clk_reg.ALUR_SYS_RSTn, MERY_TIMER_OVERFLOWp);
  }

  // FF06 TMA
  {
    /*p03.TUBY*/ wire TUBY_FF06_RDp = and4(top.TEDO_CPU_RDp, RYFO_FF04_FF07p, top.cpu_bus.BUS_CPU_A01.qp(), top.cpu_bus.TOVY_A00n());
    /*p03.TYJU*/ wire TYJU_FF06_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p, top.cpu_bus.BUS_CPU_A01.qp(), top.cpu_bus.TOVY_A00n());

    /*p03.SABU*/ SABU_TMA_D0.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p03.NYKE*/ NYKE_TMA_D1.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p03.MURU*/ MURU_TMA_D2.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D2p.qp());
    /*p03.TYVA*/ TYVA_TMA_D3.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D3p.qp());
    /*p03.TYRU*/ TYRU_TMA_D4.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D4p.qp());
    /*p03.SUFY*/ SUFY_TMA_D5.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D5p.qp());
    /*p03.PETO*/ PETO_TMA_D6.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D6p.qp());
    /*p03.SETA*/ SETA_TMA_D7.dff17(TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D7p.qp());

    /*#p03.SETE*/ cpu_bus.BUS_CPU_D0p.tri_6pn(TUBY_FF06_RDp, SABU_TMA_D0.qn16());
    /*#p03.PYRE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(TUBY_FF06_RDp, NYKE_TMA_D1.qn16());
    /*#p03.NOLA*/ cpu_bus.BUS_CPU_D2p.tri_6pn(TUBY_FF06_RDp, MURU_TMA_D2.qn16());
    /*#p03.SALU*/ cpu_bus.BUS_CPU_D3p.tri_6pn(TUBY_FF06_RDp, TYVA_TMA_D3.qn16());
    /*#p03.SUPO*/ cpu_bus.BUS_CPU_D4p.tri_6pn(TUBY_FF06_RDp, TYRU_TMA_D4.qn16());
    /*#p03.SOTU*/ cpu_bus.BUS_CPU_D5p.tri_6pn(TUBY_FF06_RDp, SUFY_TMA_D5.qn16());
    /*#p03.REVA*/ cpu_bus.BUS_CPU_D6p.tri_6pn(TUBY_FF06_RDp, PETO_TMA_D6.qn16());
    /*#p03.SAPU*/ cpu_bus.BUS_CPU_D7p.tri_6pn(TUBY_FF06_RDp, SETA_TMA_D7.qn16());
  }

  // FF07 TAC
  {
    /*p03.SORA*/ wire SORA_FF07_RDp = and4(top.TEDO_CPU_RDp, RYFO_FF04_FF07p, top.cpu_bus.BUS_CPU_A01.qp(), top.cpu_bus.BUS_CPU_A00.qp());
    /*p03.SARA*/ wire SARA_FF07_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p, top.cpu_bus.BUS_CPU_A01.qp(), top.cpu_bus.BUS_CPU_A00.qp());

    /*p03.SOPU*/ SOPU_TAC_D0.dff17(SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D0p.qp());
    /*p03.SAMY*/ SAMY_TAC_D1.dff17(SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D1p.qp());
    /*p03.SABO*/ SABO_TAC_D2.dff17(SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.BUS_CPU_D2p.qp());

    /*#p03.RYLA*/ cpu_bus.BUS_CPU_D0p.tri_6pn(SORA_FF07_RDp, SOPU_TAC_D0.qn16());
    /*#p03.ROTE*/ cpu_bus.BUS_CPU_D1p.tri_6pn(SORA_FF07_RDp, SAMY_TAC_D1.qn16());
    /*#p03.SUPE*/ cpu_bus.BUS_CPU_D2p.tri_6pn(SORA_FF07_RDp, SABO_TAC_D2.qn16());
  }
}

//------------------------------------------------------------------------------
