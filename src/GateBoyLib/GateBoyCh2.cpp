#include "GateBoySPU.h"

#include "GateBoyLib/GateBoyState.h"

void GBSound::tick_ch2(const GateBoyState& reg_old, GateBoyState& reg_new) {
  {
    // this is freq overflow or something
    /*#p15.GALU*/ wire GALU_CH2_FREQ_OVERFLOWp = not1(ch2.HERO_CH2_FREQ_10.qp_new());
    /*#p15.FUJY*/ wire FUJY = and2(ch2.GYKO_SAMPLE_CLKp.qp_new(), CEMO_CLK_1M.qp_new());
    /*#p15.GYRE*/ wire GYRE_RSTn = nor3(KEBA_APU_RSTp(), ch2.ELOX_CH2_TRIGp.qp_new(), FUJY);
    /*#p15.GYKO*/ ch2.GYKO_SAMPLE_CLKp.dff17(GALU_CH2_FREQ_OVERFLOWp, GYRE_RSTn, ch2.GYKO_SAMPLE_CLKp.qn_old());
  }

  // Channel 2 frequency counter
  {
    /*#p15.ARES*/ wire ARES = nor2(ch2.FUTE_CH2_AMP_ENn(), KEBA_APU_RSTp()); // looks like nor on the die?
    /*#p15.BODO*/ wire BODO = not1(ch2.CAZA_CH2_TRIGp.qp_new());
    /*_p15.BUTA*/ ch2.BUTA_FREQ_GATEp.nand_latch(ARES, BODO);
    /*#p15.CAMA*/ wire CAMA_FREQ_TICKp = nor2(CEMO_CLK_1M.qp_new(), ch2.BUTA_FREQ_GATEp.qp_new()); // no idea of buta polarity
    /*_p15.DOCA*/ wire DOCA_FREQ_TICKn = not1(CAMA_FREQ_TICKp);
    /*#p15.DUJU*/ wire DUJU_LOADn = nor2(ch2.DAVU_CLK(), ch2.ELOX_CH2_TRIGp.qp_new());

    /*#p15.COGU*/ wire COGU_LOADp = not1(DUJU_LOADn);
    /*_p14.DONE*/ ch2.DONE_CH2_FREQ_00.dff20(DOCA_FREQ_TICKn,                COGU_LOADp, ch2.FOFE_NR23_FREQ_00.qp_new());
    /*_p14.DYNU*/ ch2.DYNU_CH2_FREQ_01.dff20(ch2.DONE_CH2_FREQ_00.qp_new(),  COGU_LOADp, ch2.FOVA_NR23_FREQ_01.qp_new());
    /*_p14.EZOF*/ ch2.EZOF_CH2_FREQ_02.dff20(ch2.DYNU_CH2_FREQ_01.qp_new(),  COGU_LOADp, ch2.FEDY_NR23_FREQ_02.qp_new());
    /*_p14.CYVO*/ ch2.CYVO_CH2_FREQ_03.dff20(ch2.EZOF_CH2_FREQ_02.qp_new(),  COGU_LOADp, ch2.FOME_NR23_FREQ_03.qp_new());

    /*#p14.EDEP*/ wire EDEP_CH2_FREQ_03 = not1(ch2.CYVO_CH2_FREQ_03.qn_new());
    /*#p15.EROG*/ wire EROG_LOADp = not1(DUJU_LOADn);
    /*_p14.FUXO*/ ch2.FUXO_CH2_FREQ_04.dff20(EDEP_CH2_FREQ_03,               EROG_LOADp, ch2.FORA_NR23_FREQ_04.qp_new());
    /*_p14.GANO*/ ch2.GANO_CH2_FREQ_05.dff20(ch2.FUXO_CH2_FREQ_04.qp_new(),  EROG_LOADp, ch2.GODA_NR23_FREQ_05.qp_new());
    /*_p14.GOCA*/ ch2.GOCA_CH2_FREQ_06.dff20(ch2.GANO_CH2_FREQ_05.qp_new(),  EROG_LOADp, ch2.GUMY_NR23_FREQ_06.qp_new());
    /*_p14.GANE*/ ch2.GANE_CH2_FREQ_07.dff20(ch2.GOCA_CH2_FREQ_06.qp_new(),  EROG_LOADp, ch2.GUPU_NR23_FREQ_07.qp_new());

    /*#p14.GALA*/ wire GALA_CH2_FREQ_07 = not1(ch2.GANE_CH2_FREQ_07.qn_new());
    /*#p15.GYPA*/ wire GYPA_LOADp = not1(DUJU_LOADn);
    /*_p14.HEVY*/ ch2.HEVY_CH2_FREQ_08.dff20(GALA_CH2_FREQ_07,               GYPA_LOADp, ch2.JEFU_NR24_FREQ_08.qp_new());
    /*_p14.HEPU*/ ch2.HEPU_CH2_FREQ_09.dff20(ch2.HEVY_CH2_FREQ_08.qp_new(),  GYPA_LOADp, ch2.JANY_NR24_FREQ_09.qp_new());
    /*_p14.HERO*/ ch2.HERO_CH2_FREQ_10.dff20(ch2.HEPU_CH2_FREQ_09.qp_new(),  GYPA_LOADp, ch2.JUPY_NR24_FREQ_10.qp_new());
  }

  {
    /*#p15.HOTA*/ wire HOTA_CLK_128 = not1(BYFE_CLK_128n());
    /*#p15.JYNA*/ ch2.JYNA_CLK_64n.dff17(HOTA_CLK_128, KATY_APU_RSTn(), ch2.JYNA_CLK_64n.qn_old());

    /*#p15.KYLO*/ wire KYLO_CLK_64p = not1(ch2.JYNA_CLK_64n.qp_new());
    /*#p15.KENE*/ wire KENE_CLK_64n = not1(KYLO_CLK_64p);
    
    /*_p15.HYLY*/ wire HYLY_ENV_LOADn = nor2(ch2.ELOX_CH2_TRIGp.qp_new(), ch2.JOPA_CH2_ENV_TICK.qp_new());
    /*_p15.JAKE*/ wire JAKE_ENV_LOADp = not1(HYLY_ENV_LOADn);
    
    /*#p15.JORE*/ ch2.JORE_CH2_ENV_DELAYp.dff20(KENE_CLK_64n,                     JAKE_ENV_LOADp, ch2.HYFU_NR22_P0n.qp_new());
    /*#p15.JONA*/ ch2.JONA_CH2_ENV_DELAYp.dff20(ch2.JORE_CH2_ENV_DELAYp.qp_new(), JAKE_ENV_LOADp, ch2.HORE_NR22_P1n.qp_new());
    /*#p15.JEVY*/ ch2.JEVY_CH2_ENV_DELAYp.dff20(ch2.JONA_CH2_ENV_DELAYp.qp_new(), JAKE_ENV_LOADp, ch2.HAVA_NR22_P2n.qp_new());

    /*#p15.GADE*/ wire GADE = not1(ch2.JOPA_CH2_ENV_TICK.qp_new());
    /*#p15.HOLY*/ wire HOLY_CLKp = nor2(HORU_CLK_512p(), GADE);
    /*#p15.HAFE*/ wire HAFE_RSTn = nor4(HOLY_CLKp, ch2.JUPU_ENV_OFFp(), ch2.ELOX_CH2_TRIGp.qp_new(), KEBA_APU_RSTp()); // schematic wrong this is nor4
    /*#p15.KYVO*/ wire KYVO_CH2_ENV_TICK = and3(ch2.JEVY_CH2_ENV_DELAYp.qp_new(), ch2.JONA_CH2_ENV_DELAYp.qp_new(), ch2.JORE_CH2_ENV_DELAYp.qp_new());
    /*#p15.JOPA*/ ch2.JOPA_CH2_ENV_TICK.dff17(HORU_CLK_512p(), HAFE_RSTn, KYVO_CH2_ENV_TICK);
  }

  {
    /*_p15.HYPA*/ wire HYPA = nor2(ch2.ELOX_CH2_TRIGp.qp_new(), KEBA_APU_RSTp());
    /*#p15.EMYR*/ wire EMYR = nor5 (ch2.FORE_NR22_ADD.qn_new(), ch2.FENO_CH2_ENV0.qp_new(), ch2.FETE_CH2_ENV1.qp_new(), ch2.FOMY_CH2_ENV2.qp_new(), ch2.FENA_CH2_ENV3.qp_new());
    /*#p15.ERAT*/ wire ERAT = nand5(ch2.FORE_NR22_ADD.qn_new(), ch2.FENO_CH2_ENV0.qp_new(), ch2.FETE_CH2_ENV1.qp_new(), ch2.FOMY_CH2_ENV2.qp_new(), ch2.FENA_CH2_ENV3.qp_new());
    /*#p15.FYRE*/ wire FYRE = not1(ERAT);
    /*#p15.GUFY*/ wire GUFY = or2(EMYR, FYRE);
    /*#p15.HEPO*/ ch2.HEPO.dff17(ch2.JOPA_CH2_ENV_TICK.qp_new(), HYPA, GUFY);
    /*_p15.HYLE*/ wire HYLE = or2(KEBA_APU_RSTp(), ch2.ELOX_CH2_TRIGp.qp_new());
    /*#p15.JEME*/ ch2.JEME.nor_latch(ch2.HEPO.qp_new(), HYLE);
  }
  
  {
    /*_p15.DORA*/ wire DORA_CH2_STOPp = and2(ch2.CYRE_CH2_LEN_DONEp.qp_new(), ch2.EMER_NR24_LENENp.qn_new());
    /*_p15.ESYK*/ wire ESYK_CH2_STOPp = or3(KEBA_APU_RSTp(), DORA_CH2_STOPp, ch2.FUTE_CH2_AMP_ENn());
    /*_p15.DANE*/ ch2.DANE_CH2_ACTIVEp.nor_latch(ch2.ELOX_CH2_TRIGp.qp_new(), ESYK_CH2_STOPp);
  }

  {
    /*_p15.HOFO*/ wire HOFO_ENV_CLK = or3(ch2.JOPA_CH2_ENV_TICK.qp_new(), ch2.JUPU_ENV_OFFp(), ch2.JEME.qp_new());
    /*#p15.GAFA*/ wire GAFA_ENV_CLK = amux2(ch2.FORE_NR22_ADD.qn_new(), HOFO_ENV_CLK,               HOFO_ENV_CLK,               ch2.FORE_NR22_ADD.qp_new());
    /*#p15.FARU*/ wire FARU_ENV_CLK = amux2(ch2.FORE_NR22_ADD.qn_new(), ch2.FENO_CH2_ENV0.qp_new(), ch2.FENO_CH2_ENV0.qn_new(), ch2.FORE_NR22_ADD.qp_new());
    /*#p15.ETUP*/ wire ETUP_ENV_CLK = amux2(ch2.FORE_NR22_ADD.qn_new(), ch2.FETE_CH2_ENV1.qp_new(), ch2.FETE_CH2_ENV1.qn_new(), ch2.FORE_NR22_ADD.qp_new());
    /*#p15.FOPY*/ wire FOPY_ENV_CLK = amux2(ch2.FORE_NR22_ADD.qn_new(), ch2.FOMY_CH2_ENV2.qp_new(), ch2.FOMY_CH2_ENV2.qn_new(), ch2.FORE_NR22_ADD.qp_new());

    /*#p15.FENO*/ ch2.FENO_CH2_ENV0.dff20(GAFA_ENV_CLK, ch2.ELOX_CH2_TRIGp.qp_new(), ch2.GATA_NR22_V0.qn_old());
    /*#p15.FETE*/ ch2.FETE_CH2_ENV1.dff20(FARU_ENV_CLK, ch2.ELOX_CH2_TRIGp.qp_new(), ch2.GUFE_NR22_V1.qn_old());
    /*#p15.FOMY*/ ch2.FOMY_CH2_ENV2.dff20(ETUP_ENV_CLK, ch2.ELOX_CH2_TRIGp.qp_new(), ch2.GURA_NR22_V2.qn_old());
    /*#p15.FENA*/ ch2.FENA_CH2_ENV3.dff20(FOPY_ENV_CLK, ch2.ELOX_CH2_TRIGp.qp_new(), ch2.GAGE_NR22_V3.qn_old());
  }

  {
    // CH2 duty cycle generator

    /*#p15.CANO*/ ch2.CANO_00.dff17(ch2.CULE_CLK(),       AFAT_APU_RSTn(), ch2.CANO_00.qn_old());
    /*#p15.CAGY*/ ch2.CAGY_01.dff13(ch2.CANO_00.qn_new(), AFAT_APU_RSTn(), ch2.CAGY_01.qn_old());
    /*#p15.DYVE*/ ch2.DYVE_02.dff13(ch2.CAGY_01.qn_new(), AFAT_APU_RSTn(), ch2.DYVE_02.qn_old());

    /*#p15.DOJU*/ wire DOJU = nor2(ch2.BERA_NR21_D6.qn_new(), ch2.BAMY_NR21_D7.qp_new());
    /*#p15.DYTA*/ wire DYTA = nor2(ch2.BERA_NR21_D6.qp_new(), ch2.BAMY_NR21_D7.qn_new());
    /*#p15.DOMO*/ wire DOMO = nor2(ch2.BERA_NR21_D6.qn_new(), ch2.BAMY_NR21_D7.qn_new());
    /*#p15.DOVE*/ wire DOVE = nor2(ch2.BERA_NR21_D6.qp_new(), ch2.BAMY_NR21_D7.qp_new());
    /*#p15.DUGE*/ wire DUGE = not1(ch2.CANO_00.qp_new());
    /*#p15.DYMU*/ wire DYMU = and2(ch2.DYVE_02.qp_new(), ch2.CAGY_01.qp_new());
    /*#p15.EGOG*/ wire EGOG = and2(DYMU, DUGE);
    /*#p15.DARE*/ wire DARE = not1(DYMU);
    /*#p15.EXES*/ wire EXES_WAVE = amux4(EGOG, DOMO, DYMU, DYTA, ch2.DYVE_02.qp_new(), DOJU, DARE, DOVE);
    /*#p15.DOME*/ ch2.DOME_CH2_OUTp.dff17(ch2.DAVU_CLK(), AFAT_APU_RSTn(), EXES_WAVE);
  }

  {
    // CH2 output to DAC
    // dac 2 inputs ANAN ANYV ASOG AMOV
    /*#p15.CYSE*/ wire CYSE_CH2_OUTp = and2(ch2.DANE_CH2_ACTIVEp.qp_new(), ch2.DOME_CH2_OUTp.qp_new());
    /*#p15.BONU*/ wire BONU_CH2_OUTp = or2(CYSE_CH2_OUTp, EDEK_NR52_DBG_APUp());
    /*#p15.ANAN*/ wire ANAN_CH2_BIT0 = and2(ch2.FENO_CH2_ENV0.qp_new(), BONU_CH2_OUTp);
    /*#p15.ANYV*/ wire ANYV_CH2_BIT1 = and2(ch2.FETE_CH2_ENV1.qp_new(), BONU_CH2_OUTp);
    /*#p15.ASOG*/ wire ASOG_CH2_BIT2 = and2(ch2.FOMY_CH2_ENV2.qp_new(), BONU_CH2_OUTp);
    /*#p15.AMOV*/ wire AMOV_CH2_BIT3 = and2(ch2.FENA_CH2_ENV3.qp_new(), BONU_CH2_OUTp);
  }
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr21(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*_p15.DEME*/ wire DEME = nor3(ch2.CYRE_CH2_LEN_DONEp.qp_new(), BUFY_CLK_256n(), ch2.EMER_NR24_LENENp.qp_new()); // why was this or2?
  /*_p15.DYRO*/ wire DYRO_CH2_LEN_CLK = not1(DEME);

  /*_p14.AGYN*/ wire AGYN_FF16_WRn = nand2(BOGY_CPU_WRp(), reg_new.cpu_abus.COVY_ADDR_FF16p());
  /*_p15.BYMO*/ wire BYMO_FF16_WRp = not1(AGYN_FF16_WRn);
  /*_p15.AGET*/ wire AGET_FF16_WRp = not1(AGYN_FF16_WRn);
  /*_p14.ASYP*/ wire ASYP_FF16_WRp = not1(AGYN_FF16_WRn);
  /*_p14.BACU*/ wire BACU_FF16_WRp = and2(reg_new.cpu_abus.COVY_ADDR_FF16p(), BOGY_CPU_WRp());
  /*_p14.BUDU*/ wire BUDU_FF16_WRn = not1(BACU_FF16_WRp);

  /*_p15.ERYC*/ ch2.ERYC_NR21_L0.dff20(DYRO_CH2_LEN_CLK,          BYMO_FF16_WRp, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p15.CERA*/ ch2.CERA_NR21_L1.dff20(ch2.ERYC_NR21_L0.qp_new(), BYMO_FF16_WRp, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p15.CONU*/ ch2.CONU_NR21_L2.dff20(ch2.CERA_NR21_L1.qp_new(), BYMO_FF16_WRp, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p15.CAME*/ ch2.CAME_NR21_L3.dff20(ch2.CONU_NR21_L2.qp_new(), BYMO_FF16_WRp, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());

  /*#p15.BUKO*/ wire BUKO_CH2_LEN_CLK = not1(ch2.CAME_NR21_L3.qn_new());
  /*_p15.BUVA*/ ch2.BUVA_NR21_L4.dff20(BUKO_CH2_LEN_CLK,      AGET_FF16_WRp, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p15.AKYD*/ ch2.AKYD_NR21_L5.dff20(ch2.BUVA_NR21_L4.qp_new(), AGET_FF16_WRp, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());

  /*_p14.BERA*/ ch2.BERA_NR21_D6.dff9(BUDU_FF16_WRn, AFAT_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p14.BAMY*/ ch2.BAMY_NR21_D7.dff9(BUDU_FF16_WRn, AFAT_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  // CH2 length expire flag?
  /*_p14.BENY*/ wire BENY_RSTn = nor3(ASYP_FF16_WRp, KEBA_APU_RSTp(), ch2.ELOX_CH2_TRIGp.qp_new());
  /*_p15.CYRE*/ ch2.CYRE_CH2_LEN_DONEp.dff17(ch2.AKYD_NR21_L5.qn_new(), BENY_RSTn, ch2.CYRE_CH2_LEN_DONEp.qn_old());

  /*_p14.CORO*/ wire CORO_NR21_RDn = nand2(reg_new.cpu_abus.COVY_ADDR_FF16p(), BYGO_CPU_RDp());

  /*_p14.CECY*/ triwire CECY_D6 = tri6_nn(CORO_NR21_RDn, ch2.BERA_NR21_D6.qp_new()); // polarity?
  /*_p14.CEKA*/ triwire CEKA_D7 = tri6_nn(CORO_NR21_RDn, ch2.BAMY_NR21_D7.qp_new());
  
  // No read circuit for length?

  reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(CECY_D6);
  reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(CEKA_D7);
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr22(const GateBoyState& reg_old, GateBoyState& reg_new) {

  // FIXME wtf with the different clock polarities? i guess it doesn't matter much.
  /*#p14.ENUF*/ wire ENUF = and2(reg_new.cpu_abus.DUTU_ADDR_FF17p(), BOGY_CPU_WRp());
  /*#p14.GERE*/ wire GERE = and2(BOGY_CPU_WRp(), reg_new.cpu_abus.DUTU_ADDR_FF17p());
  /*#p14.JEDE*/ wire JEDE = not1(GERE);

  /*_p14.HYFU*/ ch2.HYFU_NR22_P0n.dff9(JEDE, JYBU_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p14.HORE*/ ch2.HORE_NR22_P1n.dff9(JEDE, JYBU_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p14.HAVA*/ ch2.HAVA_NR22_P2n.dff9(JEDE, JYBU_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p14.FORE*/ ch2.FORE_NR22_ADD.dff9(ENUF, JYBU_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p14.GATA*/ ch2.GATA_NR22_V0.dff9(ENUF, JYBU_APU_RSTn(),  reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p14.GUFE*/ ch2.GUFE_NR22_V1.dff9(ENUF, JYBU_APU_RSTn(),  reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p14.GURA*/ ch2.GURA_NR22_V2.dff9(ENUF, JYBU_APU_RSTn(),  reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p14.GAGE*/ ch2.GAGE_NR22_V3.dff9(ENUF, JYBU_APU_RSTn(),  reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p14.GURE*/ wire GURE = not1(reg_new.cpu_abus.DUTU_ADDR_FF17p());
  /*_p14.FYRY*/ wire FYRY = not1(reg_new.cpu_abus.DUTU_ADDR_FF17p());
  /*_p14.GEXA*/ wire GEXA = or2(GURE, AGUZ_CPU_RDn());
  /*_p14.GURU*/ wire GURU = or2(FYRY, AGUZ_CPU_RDn());

  /*_p14.HUVU*/ triwire HUVU_D0 = tri6_nn(GEXA, ch2.HYFU_NR22_P0n.qp_new());
  /*_p14.HYRE*/ triwire HYRE_D1 = tri6_nn(GEXA, ch2.HORE_NR22_P1n.qp_new());
  /*_p14.HAVU*/ triwire HAVU_D2 = tri6_nn(GEXA, ch2.HYFU_NR22_P0n.qp_new());
  /*_p14.GENE*/ triwire GENE_D3 = tri6_nn(GURU, ch2.FORE_NR22_ADD.qp_new());
  /*_p14.HUPE*/ triwire HUPE_D4 = tri6_nn(GURU, ch2.GATA_NR22_V0.qp_new());
  /*_p14.HERE*/ triwire HERE_D5 = tri6_nn(GURU, ch2.GUFE_NR22_V1.qp_new());
  /*_p14.HORO*/ triwire HORO_D6 = tri6_nn(GURU, ch2.GURA_NR22_V2.qp_new());
  /*_p14.HYRY*/ triwire HYRY_D7 = tri6_nn(GURU, ch2.GAGE_NR22_V3.qp_new());

  reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(HUVU_D0);
  reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(HYRE_D1);
  reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(HAVU_D2);
  reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(GENE_D3);
  reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(HUPE_D4);
  reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(HERE_D5);
  reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HORO_D6);
  reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(HYRY_D7);
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr23(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*_p14.DOSA*/ wire DOSA_NR23_WR1 = and2(reg_new.cpu_abus.DARA_ADDR_FF18p(), BOGY_CPU_WRp());
  /*_p14.EXUC*/ wire EXUC_NR23_WR2 = and2(reg_new.cpu_abus.DARA_ADDR_FF18p(), BOGY_CPU_WRp());
  /*_p14.ESUR*/ wire ESUR_NR23_WRn1 = not1(DOSA_NR23_WR1);
  /*_p14.FYXO*/ wire FYXO_NR23_WRn2 = not1(EXUC_NR23_WR2);
  /*_p14.FOFE*/ ch2.FOFE_NR23_FREQ_00.dff9(ESUR_NR23_WRn1, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p14.FOVA*/ ch2.FOVA_NR23_FREQ_01.dff9(ESUR_NR23_WRn1, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p14.FEDY*/ ch2.FEDY_NR23_FREQ_02.dff9(ESUR_NR23_WRn1, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p14.FOME*/ ch2.FOME_NR23_FREQ_03.dff9(ESUR_NR23_WRn1, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p14.FORA*/ ch2.FORA_NR23_FREQ_04.dff9(ESUR_NR23_WRn1, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p14.GODA*/ ch2.GODA_NR23_FREQ_05.dff9(FYXO_NR23_WRn2, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p14.GUMY*/ ch2.GUMY_NR23_FREQ_06.dff9(FYXO_NR23_WRn2, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p14.GUPU*/ ch2.GUPU_NR23_FREQ_07.dff9(FYXO_NR23_WRn2, HUDE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  // this can't be right...

  /*_p14.FERY*/ wire FERY = not1(reg_new.cpu_abus.DUCE_A03p());
  /*_p14.GUZA*/ wire GUZA = nor2(FERY, FAPE_CPU_RDp_DBGp());
  /*_p14.FUTY*/ wire FUTY = not1(GUZA);
  /*#p14.FAVA*/ triwire FAVA_D0 = tri6_nn(FUTY, ch2.DONE_CH2_FREQ_00.qn_new());
  /*#p14.FAJY*/ triwire FAJY_D1 = tri6_nn(FUTY, ch2.DYNU_CH2_FREQ_01.qn_new());
  /*#p14.FEGU*/ triwire FEGU_D2 = tri6_nn(FUTY, ch2.EZOF_CH2_FREQ_02.qn_new());
  /*#p14.FOSE*/ triwire FOSE_D3 = tri6_nn(FUTY, ch2.CYVO_CH2_FREQ_03.qn_new());
  /*#p14.GERO*/ triwire GERO_D4 = tri6_nn(FUTY, ch2.FUXO_CH2_FREQ_04.qn_new()); // d4 and d5 are switched on the schematic
  /*#p14.GAKY*/ triwire GAKY_D5 = tri6_nn(FUTY, ch2.GANO_CH2_FREQ_05.qn_new());
  /*#p14.GADU*/ triwire GADU_D6 = tri6_nn(FUTY, ch2.GOCA_CH2_FREQ_06.qn_new());
  /*#p14.GAZO*/ triwire GAZO_D7 = tri6_nn(FUTY, ch2.GANE_CH2_FREQ_07.qn_new());

  reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(FAVA_D0);
  reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(FAJY_D1);
  reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(FEGU_D2);
  reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(FOSE_D3);
  reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(GERO_D4);
  reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(GAKY_D5);
  reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GADU_D6);
  reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(GAZO_D7);
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr24(const GateBoyState& reg_old, GateBoyState& reg_new) {
  // NR24 frequency bits
  {
    /*_pXX.JENU*/ wire JENU_NR24_WRp = and2(reg_new.cpu_abus.DOZA_ADDR_FF19p(), BOGY_CPU_WRp());
    /*_pXX.KYSA*/ wire KYSA_NR24_WRn = not1(JENU_NR24_WRp);
    /*_pXX.JEFU*/ ch2.JEFU_NR24_FREQ_08.dff9(KYSA_NR24_WRn, KYPU_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_pXX.JANY*/ ch2.JANY_NR24_FREQ_09.dff9(KYSA_NR24_WRn, KYPU_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_pXX.JUPY*/ ch2.JUPY_NR24_FREQ_10.dff9(KYSA_NR24_WRn, KYPU_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());

    /*#p14.GOTE*/ wire GOTE_ADDR_FF19n = not1(reg_new.cpu_abus.DOZA_ADDR_FF19p());
    /*#p14.HYPO*/ wire HYPO_NR24_RDn = or2(GOTE_ADDR_FF19n, FAPE_CPU_RDp_DBGp());
    /*#p14.HUNA*/ triwire HUNA_D0 = tri6_nn(HYPO_NR24_RDn, ch2.HEVY_CH2_FREQ_08.qn_new());
    /*#p14.JARO*/ triwire JARO_D1 = tri6_nn(HYPO_NR24_RDn, ch2.HEPU_CH2_FREQ_09.qn_new());
    /*#p14.JEKE*/ triwire JEKE_D2 = tri6_nn(HYPO_NR24_RDn, ch2.HERO_CH2_FREQ_10.qn_new());

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(HUNA_D0);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(JARO_D1);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(JEKE_D2);
  }

  // NR24 length enable
  {
    /*#pXX.EVYF*/ wire EVYF = nand2(ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.DOZA_ADDR_FF19p());
    /*#pXX.EMER*/ ch2.EMER_NR24_LENENp.dff9(EVYF, FAZO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());

    /*#pXX.HUMA*/ wire HUMA_NR24_RDn = nand2(reg_new.cpu_abus.DOZA_ADDR_FF19p(), GADO_CPU_RDp());
    /*#pXX.GOJY*/ triwire GOJY_D6 = tri6_nn(HUMA_NR24_RDn, ch2.EMER_NR24_LENENp.qp_new());
    
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GOJY_D6);
  }

  // NR24 CH2 trigger / sequencer
  {
    /*#p??.DETA*/ wire DETA_NR24_WRn = nand2(BOGY_CPU_WRp(), reg_new.cpu_abus.DOZA_ADDR_FF19p());
    /*#p15.DERA*/ wire DERA_TRIG_RSTn = nor2(KEBA_APU_RSTp(), ch2.DOPE_CH2_TRIGp.qp_new());
    /*#p??.ETAP*/ ch2.ETAP_NR24_TRIGp.dff9(DETA_NR24_WRn, DERA_TRIG_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
    /*#p15.DOPE*/ ch2.DOPE_CH2_TRIGp.dff17(DOVA_ABCDxxxx(), CYWU_APU_RSTn(), ch2.ETAP_NR24_TRIGp.qn_old());

    /*#p15.DOXA*/ wire DOXA_TRIG_RSTn = nor2(KEBA_APU_RSTp(), ch2.DORY_CH2_TRIGp.qp_new());
    /*#p15.CELO*/ wire CELO_CH2_TRIGp = not1(DOXA_TRIG_RSTn);
    
    /*#p15.DALA*/ ch2.DALA_CH2_TRIGp.nor_latch(CELO_CH2_TRIGp, ch2.DOPE_CH2_TRIGp.qp_new());

    /*#p15.ELOX*/ ch2.ELOX_CH2_TRIGp.dff17(CEMO_CLK_1M.qp_new(), DOXA_TRIG_RSTn,  ch2.DALA_CH2_TRIGp.qn_new());
    /*#p15.DORY*/ ch2.DORY_CH2_TRIGp.dff17(CEMO_CLK_1M.qp_new(), CEXE_APU_RSTn(), ch2.ELOX_CH2_TRIGp.qp_old());
    /*#p15.CAZA*/ ch2.CAZA_CH2_TRIGp.dff17(CEMO_CLK_1M.qp_new(), CEXE_APU_RSTn(), ch2.DORY_CH2_TRIGp.qp_old());
  }
}
















