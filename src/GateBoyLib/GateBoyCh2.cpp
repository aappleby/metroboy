#include "GateBoySPU.h"

#include "GateBoyLib/GateBoyState.h"

void tick_ch2(const GateBoyState& reg_old, GateBoyState& reg_new) {

  /*#p15.DOPE*/ reg_new.ch2.DOPE_CH2_TRIGp.dff17(reg_new.sys_clk.DOVA_ABCDxxxx(), reg_new.CYWU_APU_RSTn_new(), reg_old.ch2.ETAP_NR24_TRIGp.qn_old());

  {
    /*_p14.BACU*/ wire BACU_FF16_WRp = and2(reg_new.cpu_abus.COVY_ADDR_FF16p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*_p14.BUDU*/ wire BUDU_FF16_WRn = not1(BACU_FF16_WRp);
    /*_p14.BERA*/ reg_new.ch2.BERA_NR21_D6.dff9(BUDU_FF16_WRn, reg_new.AFAT_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p14.BAMY*/ reg_new.ch2.BAMY_NR21_D7.dff9(BUDU_FF16_WRn, reg_new.AFAT_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    // FIXME wtf with the different clock polarities? i guess it doesn't matter much.
    /*#p14.ENUF*/ wire ENUF_FF17_WRp = and2(reg_new.cpu_abus.DUTU_ADDR_FF17p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*#p14.GERE*/ wire GERE_FF17_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DUTU_ADDR_FF17p());
    /*#p14.JEDE*/ wire JEDE_FF17_WRn = not1(GERE_FF17_WRp);

    /*_p14.HYFU*/ reg_new.ch2.HYFU_NR22_P0n.dff9(JEDE_FF17_WRn, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p14.HORE*/ reg_new.ch2.HORE_NR22_P1n.dff9(JEDE_FF17_WRn, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p14.HAVA*/ reg_new.ch2.HAVA_NR22_P2n.dff9(JEDE_FF17_WRn, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p14.FORE*/ reg_new.ch2.FORE_NR22_ADD.dff9(ENUF_FF17_WRp, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p14.GATA*/ reg_new.ch2.GATA_NR22_V0 .dff9(ENUF_FF17_WRp, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p14.GUFE*/ reg_new.ch2.GUFE_NR22_V1 .dff9(ENUF_FF17_WRp, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p14.GURA*/ reg_new.ch2.GURA_NR22_V2 .dff9(ENUF_FF17_WRp, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p14.GAGE*/ reg_new.ch2.GAGE_NR22_V3 .dff9(ENUF_FF17_WRp, reg_new.JYBU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*_p14.DOSA*/ wire DOSA_NR23_WR1 = and2(reg_new.cpu_abus.DARA_ADDR_FF18p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*_p14.EXUC*/ wire EXUC_NR23_WR2 = and2(reg_new.cpu_abus.DARA_ADDR_FF18p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*_p14.ESUR*/ wire ESUR_NR23_WRn1 = not1(DOSA_NR23_WR1);
    /*_p14.FYXO*/ wire FYXO_NR23_WRn2 = not1(EXUC_NR23_WR2);
    /*_p14.FOFE*/ reg_new.ch2.FOFE_NR23_FREQ_00.dff9(ESUR_NR23_WRn1, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p14.FOVA*/ reg_new.ch2.FOVA_NR23_FREQ_01.dff9(ESUR_NR23_WRn1, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p14.FEDY*/ reg_new.ch2.FEDY_NR23_FREQ_02.dff9(ESUR_NR23_WRn1, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p14.FOME*/ reg_new.ch2.FOME_NR23_FREQ_03.dff9(ESUR_NR23_WRn1, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p14.FORA*/ reg_new.ch2.FORA_NR23_FREQ_04.dff9(ESUR_NR23_WRn1, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p14.GODA*/ reg_new.ch2.GODA_NR23_FREQ_05.dff9(FYXO_NR23_WRn2, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p14.GUMY*/ reg_new.ch2.GUMY_NR23_FREQ_06.dff9(FYXO_NR23_WRn2, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p14.GUPU*/ reg_new.ch2.GUPU_NR23_FREQ_07.dff9(FYXO_NR23_WRn2, reg_new.HUDE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*_pXX.JENU*/ wire JENU_NR24_WRp = and2(reg_new.cpu_abus.DOZA_ADDR_FF19p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*_pXX.KYSA*/ wire KYSA_NR24_WRn = not1(JENU_NR24_WRp);
    /*_pXX.JEFU*/ reg_new.ch2.JEFU_NR24_FREQ_08.dff9(KYSA_NR24_WRn, reg_new.KYPU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_pXX.JANY*/ reg_new.ch2.JANY_NR24_FREQ_09.dff9(KYSA_NR24_WRn, reg_new.KYPU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_pXX.JUPY*/ reg_new.ch2.JUPY_NR24_FREQ_10.dff9(KYSA_NR24_WRn, reg_new.KYPU_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());

    /*#p??.DETA*/ wire DETA_NR24_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DOZA_ADDR_FF19p());
    /*#p15.DERA*/ wire DERA_TRIG_RSTn = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch2.DOPE_CH2_TRIGp.qp_new());
    /*#p??.ETAP*/ reg_new.ch2.ETAP_NR24_TRIGp.dff9(DETA_NR24_WRn, DERA_TRIG_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

    /*#pXX.EVYF*/ wire EVYF = nand2(reg_new.ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.DOZA_ADDR_FF19p());
    /*#pXX.EMER*/ reg_new.ch2.EMER_NR24_LENENp.dff9(EVYF, reg_new.FAZO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  }

  //----------
  // Trigger

  {
    /*#p15.DORY*/ reg_new.ch2.DORY_CH2_TRIGp.dff17(reg_new.spu.CEMO_CLK_1M.qp_new(), reg_new.CEXE_APU_RSTn_new(), reg_old.ch2.ELOX_CH2_TRIGp.qp_old());
    /*#p15.CAZA*/ reg_new.ch2.CAZA_CH2_TRIGp.dff17(reg_new.spu.CEMO_CLK_1M.qp_new(), reg_new.CEXE_APU_RSTn_new(), reg_old.ch2.DORY_CH2_TRIGp.qp_old());

    /*#p15.DOXA*/ wire DOXA_TRIG_RSTn_new = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch2.DORY_CH2_TRIGp.qp_new());
    /*#p15.ELOX*/ reg_new.ch2.ELOX_CH2_TRIGp.dff17(reg_new.spu.CEMO_CLK_1M.qp_new(), DOXA_TRIG_RSTn_new, reg_old.ch2.DALA_CH2_TRIGp.qn_old());

    /*#p15.CELO*/ wire CELO_CH2_TRIGp_new = not1(DOXA_TRIG_RSTn_new);   
    /*#p15.DALA*/ reg_new.ch2.DALA_CH2_TRIGp.nor_latch(CELO_CH2_TRIGp_new, reg_new.ch2.DOPE_CH2_TRIGp.qp_new());
  }

  //----------
  // Clocks

  {
    /*#p15.HOTA*/ wire HOTA_CLK_128 = not1(reg_new.spu.BYFE_CLK_128n());
    /*#p15.JYNA*/ reg_new.ch2.JYNA_CLK_64n.dff17(HOTA_CLK_128, reg_new.KATY_APU_RSTn_new(), reg_old.ch2.JYNA_CLK_64n.qn_old());
  }

  //----------------------------------------
  // Freq counter

  {
    /*#p15.ARES*/ wire ARES = nor2(reg_new.ch2.FUTE_CH2_AMP_ENn_new(), reg_new.KEBA_APU_RSTp_new()); // looks like nor on the die?
    /*#p15.BODO*/ wire BODO = not1(reg_new.ch2.CAZA_CH2_TRIGp.qp_new());
    /*_p15.BUTA*/ reg_new.ch2.BUTA_FREQ_GATEp.nand_latch(ARES, BODO);

    /*#p15.CAMA*/ wire CAMA_FREQ_TICKp = nor2(reg_new.spu.CEMO_CLK_1M.qp_new(), reg_new.ch2.BUTA_FREQ_GATEp.qp_new());
    /*_p15.DOCA*/ wire DOCA_FREQ_TICKn = not1(CAMA_FREQ_TICKp);

    /*#p15.DUJU*/ wire DUJU_LOADn_old = nor2(reg_old.ch2.DAVU_SAMPLE_CLKp_old(), reg_old.ch2.ELOX_CH2_TRIGp.qp_old());
    /*#p15.COGU*/ wire COGU_LOADp_old = not1(DUJU_LOADn_old);
    /*#p15.EROG*/ wire EROG_LOADp_old = not1(DUJU_LOADn_old);
    /*#p15.GYPA*/ wire GYPA_LOADp_old = not1(DUJU_LOADn_old);

    /*_p14.DONE*/ reg_new.ch2.DONE_CH2_FREQ_00.dff20(DOCA_FREQ_TICKn,                        COGU_LOADp_old, reg_old.ch2.FOFE_NR23_FREQ_00.qp_old());
    /*_p14.DYNU*/ reg_new.ch2.DYNU_CH2_FREQ_01.dff20(reg_new.ch2.DONE_CH2_FREQ_00.qp_new(),  COGU_LOADp_old, reg_old.ch2.FOVA_NR23_FREQ_01.qp_old());
    /*_p14.EZOF*/ reg_new.ch2.EZOF_CH2_FREQ_02.dff20(reg_new.ch2.DYNU_CH2_FREQ_01.qp_new(),  COGU_LOADp_old, reg_old.ch2.FEDY_NR23_FREQ_02.qp_old());
    /*_p14.CYVO*/ reg_new.ch2.CYVO_CH2_FREQ_03.dff20(reg_new.ch2.EZOF_CH2_FREQ_02.qp_new(),  COGU_LOADp_old, reg_old.ch2.FOME_NR23_FREQ_03.qp_old());

    /*#p14.EDEP*/ wire EDEP_CH2_FREQ_03 = not1(reg_new.ch2.CYVO_CH2_FREQ_03.qn_new());
    /*_p14.FUXO*/ reg_new.ch2.FUXO_CH2_FREQ_04.dff20(EDEP_CH2_FREQ_03,                       EROG_LOADp_old, reg_old.ch2.FORA_NR23_FREQ_04.qp_old());
    /*_p14.GANO*/ reg_new.ch2.GANO_CH2_FREQ_05.dff20(reg_new.ch2.FUXO_CH2_FREQ_04.qp_new(),  EROG_LOADp_old, reg_old.ch2.GODA_NR23_FREQ_05.qp_old());
    /*_p14.GOCA*/ reg_new.ch2.GOCA_CH2_FREQ_06.dff20(reg_new.ch2.GANO_CH2_FREQ_05.qp_new(),  EROG_LOADp_old, reg_old.ch2.GUMY_NR23_FREQ_06.qp_old());
    /*_p14.GANE*/ reg_new.ch2.GANE_CH2_FREQ_07.dff20(reg_new.ch2.GOCA_CH2_FREQ_06.qp_new(),  EROG_LOADp_old, reg_old.ch2.GUPU_NR23_FREQ_07.qp_old());

    /*#p14.GALA*/ wire GALA_CH2_FREQ_07 = not1(reg_new.ch2.GANE_CH2_FREQ_07.qn_new());
    /*_p14.HEVY*/ reg_new.ch2.HEVY_CH2_FREQ_08.dff20(GALA_CH2_FREQ_07,                       GYPA_LOADp_old, reg_old.ch2.JEFU_NR24_FREQ_08.qp_old());
    /*_p14.HEPU*/ reg_new.ch2.HEPU_CH2_FREQ_09.dff20(reg_new.ch2.HEVY_CH2_FREQ_08.qp_new(),  GYPA_LOADp_old, reg_old.ch2.JANY_NR24_FREQ_09.qp_old());
    /*_p14.HERO*/ reg_new.ch2.HERO_CH2_FREQ_10.dff20(reg_new.ch2.HEPU_CH2_FREQ_09.qp_new(),  GYPA_LOADp_old, reg_old.ch2.JUPY_NR24_FREQ_10.qp_old());
  }

  {
    /*#p15.GALU*/ wire GALU_CH2_FREQ_OVERFLOWp = not1(reg_new.ch2.HERO_CH2_FREQ_10.qp_new());
    /*#p15.FUJY*/ wire FUJY_old = and2(reg_old.ch2.GYKO_SAMPLE_CLKp.qp_old(), reg_old.spu.CEMO_CLK_1M.qp_old());
    /*#p15.GYRE*/ wire GYRE_RSTn_old = nor3(reg_new.KEBA_APU_RSTp_new(), reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), FUJY_old);
    /*#p15.GYKO*/ reg_new.ch2.GYKO_SAMPLE_CLKp.dff17(GALU_CH2_FREQ_OVERFLOWp, GYRE_RSTn_old, reg_old.ch2.GYKO_SAMPLE_CLKp.qn_old());
  }

  {
    /*#p15.GALU*/ wire GALU_CH2_FREQ_OVERFLOWp = not1(reg_new.ch2.HERO_CH2_FREQ_10.qp_new());
    /*#p15.FUJY*/ wire FUJY_new = and2(reg_new.ch2.GYKO_SAMPLE_CLKp.qp_new(), reg_new.spu.CEMO_CLK_1M.qp_new());
    /*#p15.GYRE*/ wire GYRE_RSTn_new = nor3(reg_new.KEBA_APU_RSTp_new(), reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), FUJY_new);
    /*#p15.GYKO*/ reg_new.ch2.GYKO_SAMPLE_CLKp.dff17_any(GALU_CH2_FREQ_OVERFLOWp, GYRE_RSTn_new, reg_old.ch2.GYKO_SAMPLE_CLKp.qn_old());
  }

  {
    /*#p15.CAMA*/ wire CAMA_FREQ_TICKp = nor2(reg_new.spu.CEMO_CLK_1M.qp_new(), reg_new.ch2.BUTA_FREQ_GATEp.qp_new());
    /*_p15.DOCA*/ wire DOCA_FREQ_TICKn = not1(CAMA_FREQ_TICKp);

    /*#p15.DUJU*/ wire DUJU_LOADn_new = nor2(reg_new.ch2.DAVU_SAMPLE_CLKp_new(), reg_new.ch2.ELOX_CH2_TRIGp.qp_new());
    /*#p15.COGU*/ wire COGU_LOADp_new = not1(DUJU_LOADn_new);
    /*#p15.EROG*/ wire EROG_LOADp_new = not1(DUJU_LOADn_new);
    /*#p15.GYPA*/ wire GYPA_LOADp_new = not1(DUJU_LOADn_new);

    /*_p14.DONE*/ reg_new.ch2.DONE_CH2_FREQ_00.dff20_any(DOCA_FREQ_TICKn,                        COGU_LOADp_new, reg_old.ch2.FOFE_NR23_FREQ_00.qp_old());
    /*_p14.DYNU*/ reg_new.ch2.DYNU_CH2_FREQ_01.dff20_any(reg_new.ch2.DONE_CH2_FREQ_00.qp_new(),  COGU_LOADp_new, reg_old.ch2.FOVA_NR23_FREQ_01.qp_old());
    /*_p14.EZOF*/ reg_new.ch2.EZOF_CH2_FREQ_02.dff20_any(reg_new.ch2.DYNU_CH2_FREQ_01.qp_new(),  COGU_LOADp_new, reg_old.ch2.FEDY_NR23_FREQ_02.qp_old());
    /*_p14.CYVO*/ reg_new.ch2.CYVO_CH2_FREQ_03.dff20_any(reg_new.ch2.EZOF_CH2_FREQ_02.qp_new(),  COGU_LOADp_new, reg_old.ch2.FOME_NR23_FREQ_03.qp_old());

    /*#p14.EDEP*/ wire EDEP_CH2_FREQ_03 = not1(reg_new.ch2.CYVO_CH2_FREQ_03.qn_new());
    /*_p14.FUXO*/ reg_new.ch2.FUXO_CH2_FREQ_04.dff20_any(EDEP_CH2_FREQ_03,                       EROG_LOADp_new, reg_old.ch2.FORA_NR23_FREQ_04.qp_old());
    /*_p14.GANO*/ reg_new.ch2.GANO_CH2_FREQ_05.dff20_any(reg_new.ch2.FUXO_CH2_FREQ_04.qp_new(),  EROG_LOADp_new, reg_old.ch2.GODA_NR23_FREQ_05.qp_old());
    /*_p14.GOCA*/ reg_new.ch2.GOCA_CH2_FREQ_06.dff20_any(reg_new.ch2.GANO_CH2_FREQ_05.qp_new(),  EROG_LOADp_new, reg_old.ch2.GUMY_NR23_FREQ_06.qp_old());
    /*_p14.GANE*/ reg_new.ch2.GANE_CH2_FREQ_07.dff20_any(reg_new.ch2.GOCA_CH2_FREQ_06.qp_new(),  EROG_LOADp_new, reg_old.ch2.GUPU_NR23_FREQ_07.qp_old());

    /*#p14.GALA*/ wire GALA_CH2_FREQ_07 = not1(reg_new.ch2.GANE_CH2_FREQ_07.qn_new());
    /*_p14.HEVY*/ reg_new.ch2.HEVY_CH2_FREQ_08.dff20_any(GALA_CH2_FREQ_07,                       GYPA_LOADp_new, reg_old.ch2.JEFU_NR24_FREQ_08.qp_old());
    /*_p14.HEPU*/ reg_new.ch2.HEPU_CH2_FREQ_09.dff20_any(reg_new.ch2.HEVY_CH2_FREQ_08.qp_new(),  GYPA_LOADp_new, reg_old.ch2.JANY_NR24_FREQ_09.qp_old());
    /*_p14.HERO*/ reg_new.ch2.HERO_CH2_FREQ_10.dff20_any(reg_new.ch2.HEPU_CH2_FREQ_09.qp_new(),  GYPA_LOADp_new, reg_old.ch2.JUPY_NR24_FREQ_10.qp_old());
  }

  //----------------------------------------
  // Env tick

  {
    /*#p15.GADE*/ wire GADE_old = not1(reg_old.ch2.JOPA_CH2_ENV_TICK.qp_old());
    /*#p15.HOLY*/ wire HOLY_CLKp_old = nor2(reg_old.spu.HORU_CLK_512_old(), GADE_old);
    /*#p15.HAFE*/ wire HAFE_RSTn_old = nor4(HOLY_CLKp_old, reg_old.ch2.JUPU_ENV_OFFp_old(), reg_old.ch2.ELOX_CH2_TRIGp.qp_old(), reg_old.KEBA_APU_RSTp_old()); // schematic wrong this is nor4
   
    /*#p15.KYVO*/ wire KYVO_CH2_ENV_TICK_old = and3(reg_old.ch2.JEVY_CH2_ENV_DELAYp.qp_old(), reg_old.ch2.JONA_CH2_ENV_DELAYp.qp_old(), reg_old.ch2.JORE_CH2_ENV_DELAYp.qp_old());
   
    /*#p15.JOPA*/ reg_new.ch2.JOPA_CH2_ENV_TICK.dff17(reg_new.spu.HORU_CLK_512_new(), HAFE_RSTn_old, KYVO_CH2_ENV_TICK_old);

    /*#p15.GADE*/ wire GADE_new = not1(reg_new.ch2.JOPA_CH2_ENV_TICK.qp_new());
    /*#p15.HOLY*/ wire HOLY_CLKp_new = nor2(reg_new.spu.HORU_CLK_512_new(), GADE_new);
    /*#p15.HAFE*/ wire HAFE_RSTn_new = nor4(HOLY_CLKp_new, reg_new.ch2.JUPU_ENV_OFFp_new(), reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), reg_new.KEBA_APU_RSTp_new()); // schematic wrong this is nor4
   
    /*#p15.JOPA*/ reg_new.ch2.JOPA_CH2_ENV_TICK.dff17_any(reg_new.spu.HORU_CLK_512_new(), HAFE_RSTn_new, KYVO_CH2_ENV_TICK_old);
  }

  //----------------------------------------
  // Env delay

  {
    /*#p15.KYLO*/ wire KYLO_CLK_64p_new = not1(reg_new.ch2.JYNA_CLK_64n.qp_new());
    /*#p15.KENE*/ wire KENE_CLK_64n_new = not1(KYLO_CLK_64p_new);
    
    /*_p15.HYLY*/ wire HYLY_ENV_LOADn_new = nor2(reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), reg_new.ch2.JOPA_CH2_ENV_TICK.qp_new());
    /*_p15.JAKE*/ wire JAKE_ENV_LOADp_new = not1(HYLY_ENV_LOADn_new);
    
    /*#p15.JORE*/ reg_new.ch2.JORE_CH2_ENV_DELAYp.dff20(KENE_CLK_64n_new,                         JAKE_ENV_LOADp_new, reg_old.ch2.HYFU_NR22_P0n.qp_old());
    /*#p15.JONA*/ reg_new.ch2.JONA_CH2_ENV_DELAYp.dff20(reg_new.ch2.JORE_CH2_ENV_DELAYp.qp_new(), JAKE_ENV_LOADp_new, reg_old.ch2.HORE_NR22_P1n.qp_old());
    /*#p15.JEVY*/ reg_new.ch2.JEVY_CH2_ENV_DELAYp.dff20(reg_new.ch2.JONA_CH2_ENV_DELAYp.qp_new(), JAKE_ENV_LOADp_new, reg_old.ch2.HAVA_NR22_P2n.qp_old());
  }

  //----------------------------------------
  // Env stop

  {
    /*_p15.HYPA*/ wire HYPA_ENV_MAX_RSTn_new = nor2(reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), reg_new.KEBA_APU_RSTp_new());

    /*#p15.EMYR*/ wire EMYR_ENV_BOTp_old = nor5 (reg_old.ch2.FORE_NR22_ADD.qn_old(), reg_old.ch2.FENO_CH2_ENV0.qp_old(), reg_old.ch2.FETE_CH2_ENV1.qp_old(), reg_old.ch2.FOMY_CH2_ENV2.qp_old(), reg_old.ch2.FENA_CH2_ENV3.qp_old());
    /*#p15.ERAT*/ wire ERAT_ENV_TOPn_old = nand5(reg_old.ch2.FORE_NR22_ADD.qn_old(), reg_old.ch2.FENO_CH2_ENV0.qp_old(), reg_old.ch2.FETE_CH2_ENV1.qp_old(), reg_old.ch2.FOMY_CH2_ENV2.qp_old(), reg_old.ch2.FENA_CH2_ENV3.qp_old());
    /*#p15.FYRE*/ wire FYRE_ENV_TOPp_old = not1(ERAT_ENV_TOPn_old);
    /*#p15.GUFY*/ wire GUFY_ENV_MAXp_old = or2(EMYR_ENV_BOTp_old, FYRE_ENV_TOPp_old);

    /*#p15.HEPO*/ reg_new.ch2.HEPO_ENV_MAXp.dff17(reg_new.ch2.JOPA_CH2_ENV_TICK.qp_new(), HYPA_ENV_MAX_RSTn_new, GUFY_ENV_MAXp_old);
  }

  //----------------------------------------
  // Env increment


  {
    /*_p15.HYLE*/ wire HYLE_ENV_RSTp = or2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch2.ELOX_CH2_TRIGp.qp_new());
    /*#p15.JEME*/ reg_new.ch2.JEME_ENV_DONEp.nor_latch(reg_new.ch2.HEPO_ENV_MAXp.qp_new(), HYLE_ENV_RSTp);
    /*_p15.HOFO*/ wire HOFO_ENV_CLK = or3(reg_new.ch2.JOPA_CH2_ENV_TICK.qp_new(), reg_new.ch2.JUPU_ENV_OFFp_new(), reg_new.ch2.JEME_ENV_DONEp.qp_new());

    /*#p15.GAFA*/ wire GAFA_ENV_CLK = amux2(reg_new.ch2.FORE_NR22_ADD.qn_new(), HOFO_ENV_CLK,                       HOFO_ENV_CLK,                       reg_new.ch2.FORE_NR22_ADD.qp_new());
    /*#p15.FENO*/ reg_new.ch2.FENO_CH2_ENV0.dff20(GAFA_ENV_CLK, reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), reg_old.ch2.GATA_NR22_V0.qn_old());

    /*#p15.FARU*/ wire FARU_ENV_CLK = amux2(reg_new.ch2.FORE_NR22_ADD.qn_new(), reg_new.ch2.FENO_CH2_ENV0.qp_new(), reg_new.ch2.FENO_CH2_ENV0.qn_new(), reg_new.ch2.FORE_NR22_ADD.qp_new());
    /*#p15.FETE*/ reg_new.ch2.FETE_CH2_ENV1.dff20(FARU_ENV_CLK, reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), reg_old.ch2.GUFE_NR22_V1.qn_old());

    /*#p15.ETUP*/ wire ETUP_ENV_CLK = amux2(reg_new.ch2.FORE_NR22_ADD.qn_new(), reg_new.ch2.FETE_CH2_ENV1.qp_new(), reg_new.ch2.FETE_CH2_ENV1.qn_new(), reg_new.ch2.FORE_NR22_ADD.qp_new());
    /*#p15.FOMY*/ reg_new.ch2.FOMY_CH2_ENV2.dff20(ETUP_ENV_CLK, reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), reg_old.ch2.GURA_NR22_V2.qn_old());

    /*#p15.FOPY*/ wire FOPY_ENV_CLK = amux2(reg_new.ch2.FORE_NR22_ADD.qn_new(), reg_new.ch2.FOMY_CH2_ENV2.qp_new(), reg_new.ch2.FOMY_CH2_ENV2.qn_new(), reg_new.ch2.FORE_NR22_ADD.qp_new());
    /*#p15.FENA*/ reg_new.ch2.FENA_CH2_ENV3.dff20(FOPY_ENV_CLK, reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), reg_old.ch2.GAGE_NR22_V3.qn_old());
  }

  //----------------------------------------
  // Len tick

  {
    // using CYRE_old to break the loop

    /*_p15.DEME*/ wire DEME_LEN_CLKn_new = nor3(reg_old.ch2.CYRE_CH2_LEN_DONEp.qp_old(), reg_new.spu.BUFY_CLK_256n(), reg_new.ch2.EMER_NR24_LENENp.qp_new()); // why was this or2?
    /*_p15.DYRO*/ wire DYRO_LEN_CLKp_new = not1(DEME_LEN_CLKn_new);

    /*_p14.AGYN*/ wire AGYN_FF16_WRn_new = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.COVY_ADDR_FF16p());
    /*_p15.BYMO*/ wire BYMO_FF16_WRp_new = not1(AGYN_FF16_WRn_new);
    /*_p15.AGET*/ wire AGET_FF16_WRp_new = not1(AGYN_FF16_WRn_new);
    /*_p14.ASYP*/ wire ASYP_FF16_WRp_new = not1(AGYN_FF16_WRn_new);

    /*_p15.ERYC*/ reg_new.ch2.ERYC_NR21_L0.dff20(DYRO_LEN_CLKp_new,                 BYMO_FF16_WRp_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p15.CERA*/ reg_new.ch2.CERA_NR21_L1.dff20(reg_new.ch2.ERYC_NR21_L0.qp_new(), BYMO_FF16_WRp_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p15.CONU*/ reg_new.ch2.CONU_NR21_L2.dff20(reg_new.ch2.CERA_NR21_L1.qp_new(), BYMO_FF16_WRp_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p15.CAME*/ reg_new.ch2.CAME_NR21_L3.dff20(reg_new.ch2.CONU_NR21_L2.qp_new(), BYMO_FF16_WRp_new, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());

    /*#p15.BUKO*/ wire BUKO_CH2_LEN_CLK = not1(reg_new.ch2.CAME_NR21_L3.qn_new());
    /*_p15.BUVA*/ reg_new.ch2.BUVA_NR21_L4.dff20(BUKO_CH2_LEN_CLK,                  AGET_FF16_WRp_new, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p15.AKYD*/ reg_new.ch2.AKYD_NR21_L5.dff20(reg_new.ch2.BUVA_NR21_L4.qp_new(), AGET_FF16_WRp_new, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());

    /*_p14.BENY*/ wire BENY_RSTn = nor3(ASYP_FF16_WRp_new, reg_new.KEBA_APU_RSTp_new(), reg_new.ch2.ELOX_CH2_TRIGp.qp_new());
    /*_p15.CYRE*/ reg_new.ch2.CYRE_CH2_LEN_DONEp.dff17(reg_new.ch2.AKYD_NR21_L5.qn_new(), BENY_RSTn, reg_old.ch2.CYRE_CH2_LEN_DONEp.qn_old());
  }

  //----------------------------------------
  // CH2 duty cycle & output to DAC

  {
    /*#p15.CANO*/ reg_new.ch2.CANO_00.dff17(reg_new.ch2.CULE_SAMPLE_CLKn_new(), reg_new.AFAT_APU_RSTn_new(), reg_old.ch2.CANO_00.qn_old());
    /*#p15.CAGY*/ reg_new.ch2.CAGY_01.dff13(reg_new.ch2.CANO_00.qn_new(),       reg_new.AFAT_APU_RSTn_new(), reg_old.ch2.CAGY_01.qn_old());
    /*#p15.DYVE*/ reg_new.ch2.DYVE_02.dff13(reg_new.ch2.CAGY_01.qn_new(),       reg_new.AFAT_APU_RSTn_new(), reg_old.ch2.DYVE_02.qn_old());

    /*#p15.DOJU*/ wire DOJU = nor2(reg_new.ch2.BERA_NR21_D6.qn_new(), reg_new.ch2.BAMY_NR21_D7.qp_new());
    /*#p15.DYTA*/ wire DYTA = nor2(reg_new.ch2.BERA_NR21_D6.qp_new(), reg_new.ch2.BAMY_NR21_D7.qn_new());
    /*#p15.DOMO*/ wire DOMO = nor2(reg_new.ch2.BERA_NR21_D6.qn_new(), reg_new.ch2.BAMY_NR21_D7.qn_new());
    /*#p15.DOVE*/ wire DOVE = nor2(reg_new.ch2.BERA_NR21_D6.qp_new(), reg_new.ch2.BAMY_NR21_D7.qp_new());
    /*#p15.DUGE*/ wire DUGE = not1(reg_new.ch2.CANO_00.qp_new());
    /*#p15.DYMU*/ wire DYMU = and2(reg_new.ch2.DYVE_02.qp_new(), reg_new.ch2.CAGY_01.qp_new());
    /*#p15.EGOG*/ wire EGOG = and2(DYMU, DUGE);
    /*#p15.DARE*/ wire DARE = not1(DYMU);
    /*#p15.EXES*/ wire EXES_WAVE = amux4(EGOG, DOMO, DYMU, DYTA, reg_new.ch2.DYVE_02.qp_new(), DOJU, DARE, DOVE);
    /*#p15.DOME*/ reg_new.ch2.DOME_CH2_OUTp.dff17(reg_new.ch2.DAVU_SAMPLE_CLKp_new(), reg_new.AFAT_APU_RSTn_new(), EXES_WAVE);

    // dac 2 inputs ANAN ANYV ASOG AMOV
    /*_p15.DORA*/ wire DORA_CH2_STOPp_new = and2(reg_new.ch2.CYRE_CH2_LEN_DONEp.qp_new(), reg_new.ch2.EMER_NR24_LENENp.qn_new());
    /*_p15.ESYK*/ wire ESYK_CH2_STOPp_new = or3(reg_new.KEBA_APU_RSTp_new(), DORA_CH2_STOPp_new, reg_new.ch2.FUTE_CH2_AMP_ENn_new());
    /*_p15.DANE*/ reg_new.ch2.DANE_CH2_ACTIVEp.nor_latch(reg_new.ch2.ELOX_CH2_TRIGp.qp_new(), ESYK_CH2_STOPp_new);
    /*#p15.CYSE*/ wire CYSE_CH2_OUTp = and2(reg_new.ch2.DANE_CH2_ACTIVEp.qp_new(), reg_new.ch2.DOME_CH2_OUTp.qp_new());
    /*#p15.BONU*/ wire BONU_CH2_OUTp = or2(CYSE_CH2_OUTp, reg_new.EDEK_NR52_DBG_APUp());
    /*#p15.ANAN*/ wire ANAN_CH2_BIT0 = and2(reg_new.ch2.FENO_CH2_ENV0.qp_new(), BONU_CH2_OUTp);
    /*#p15.ANYV*/ wire ANYV_CH2_BIT1 = and2(reg_new.ch2.FETE_CH2_ENV1.qp_new(), BONU_CH2_OUTp);
    /*#p15.ASOG*/ wire ASOG_CH2_BIT2 = and2(reg_new.ch2.FOMY_CH2_ENV2.qp_new(), BONU_CH2_OUTp);
    /*#p15.AMOV*/ wire AMOV_CH2_BIT3 = and2(reg_new.ch2.FENA_CH2_ENV3.qp_new(), BONU_CH2_OUTp);
  }



  //----------------------------------------
  // NR21 read

  {
    /*_p14.CORO*/ wire CORO_NR21_RDn = nand2(reg_new.cpu_abus.COVY_ADDR_FF16p(), reg_new.cpu_signals.BYGO_CPU_RDp());
    /*_p14.CECY*/ triwire CECY_D6 = tri6_nn(CORO_NR21_RDn, reg_new.ch2.BERA_NR21_D6.qp_new()); // polarity?
    /*_p14.CEKA*/ triwire CEKA_D7 = tri6_nn(CORO_NR21_RDn, reg_new.ch2.BAMY_NR21_D7.qp_new());
    
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(CECY_D6);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(CEKA_D7);
  }

  //----------------------------------------
  // NR22 read

  {
    /*_p14.GURE*/ wire GURE = not1(reg_new.cpu_abus.DUTU_ADDR_FF17p());
    /*_p14.FYRY*/ wire FYRY = not1(reg_new.cpu_abus.DUTU_ADDR_FF17p());
    /*_p14.GEXA*/ wire GEXA = or2(GURE, reg_new.cpu_signals.AGUZ_CPU_RDn());
    /*_p14.GURU*/ wire GURU = or2(FYRY, reg_new.cpu_signals.AGUZ_CPU_RDn());

    /*_p14.HUVU*/ triwire HUVU_D0 = tri6_nn(GEXA, reg_new.ch2.HYFU_NR22_P0n.qp_new());
    /*_p14.HYRE*/ triwire HYRE_D1 = tri6_nn(GEXA, reg_new.ch2.HORE_NR22_P1n.qp_new());
    /*_p14.HAVU*/ triwire HAVU_D2 = tri6_nn(GEXA, reg_new.ch2.HYFU_NR22_P0n.qp_new());
    /*_p14.GENE*/ triwire GENE_D3 = tri6_nn(GURU, reg_new.ch2.FORE_NR22_ADD.qp_new());
    /*_p14.HUPE*/ triwire HUPE_D4 = tri6_nn(GURU, reg_new.ch2.GATA_NR22_V0.qp_new());
    /*_p14.HERE*/ triwire HERE_D5 = tri6_nn(GURU, reg_new.ch2.GUFE_NR22_V1.qp_new());
    /*_p14.HORO*/ triwire HORO_D6 = tri6_nn(GURU, reg_new.ch2.GURA_NR22_V2.qp_new());
    /*_p14.HYRY*/ triwire HYRY_D7 = tri6_nn(GURU, reg_new.ch2.GAGE_NR22_V3.qp_new());

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(HUVU_D0);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(HYRE_D1);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(HAVU_D2);
    reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(GENE_D3);
    reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(HUPE_D4);
    reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(HERE_D5);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HORO_D6);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(HYRY_D7);
  }

  //----------------------------------------
  // NR23 read? this can't be right...

  {
    /*#p14.FERY*/ wire FERY_A03n = not1(reg_new.cpu_abus.DUCE_A03p());
    /*#p14.GUZA*/ wire GUZA = nor2(FERY_A03n, reg_new.FAPE_CPU_RDp_DBGn());
    /*#p14.FUTY*/ wire FUTY = not1(GUZA);

    /*#p14.FAVA*/ triwire FAVA_D0 = tri6_nn(FUTY, reg_new.ch2.DONE_CH2_FREQ_00.qn_new());
    /*#p14.FAJY*/ triwire FAJY_D1 = tri6_nn(FUTY, reg_new.ch2.DYNU_CH2_FREQ_01.qn_new());
    /*#p14.FEGU*/ triwire FEGU_D2 = tri6_nn(FUTY, reg_new.ch2.EZOF_CH2_FREQ_02.qn_new());
    /*#p14.FOSE*/ triwire FOSE_D3 = tri6_nn(FUTY, reg_new.ch2.CYVO_CH2_FREQ_03.qn_new());
    /*#p14.GERO*/ triwire GERO_D4 = tri6_nn(FUTY, reg_new.ch2.FUXO_CH2_FREQ_04.qn_new()); // d4 and d5 are switched on the schematic
    /*#p14.GAKY*/ triwire GAKY_D5 = tri6_nn(FUTY, reg_new.ch2.GANO_CH2_FREQ_05.qn_new());
    /*#p14.GADU*/ triwire GADU_D6 = tri6_nn(FUTY, reg_new.ch2.GOCA_CH2_FREQ_06.qn_new());
    /*#p14.GAZO*/ triwire GAZO_D7 = tri6_nn(FUTY, reg_new.ch2.GANE_CH2_FREQ_07.qn_new());

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(FAVA_D0);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(FAJY_D1);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(FEGU_D2);
    reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(FOSE_D3);
    reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(GERO_D4);
    reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(GAKY_D5);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GADU_D6);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(GAZO_D7);
  }

  //----------------------------------------
  // NR24 read

  {
    /*#p14.GOTE*/ wire GOTE_ADDR_FF19n = not1(reg_new.cpu_abus.DOZA_ADDR_FF19p());
    /*#p14.HYPO*/ wire HYPO_NR24_RDn = or2(GOTE_ADDR_FF19n, reg_new.FAPE_CPU_RDp_DBGn());
    /*#pXX.HUMA*/ wire HUMA_NR24_RDn = nand2(reg_new.cpu_abus.DOZA_ADDR_FF19p(), reg_new.cpu_signals.GADO_CPU_RDp());

    /*#p14.HUNA*/ triwire HUNA_D0 = tri6_nn(HYPO_NR24_RDn, reg_new.ch2.HEVY_CH2_FREQ_08.qn_new());
    /*#p14.JARO*/ triwire JARO_D1 = tri6_nn(HYPO_NR24_RDn, reg_new.ch2.HEPU_CH2_FREQ_09.qn_new());
    /*#p14.JEKE*/ triwire JEKE_D2 = tri6_nn(HYPO_NR24_RDn, reg_new.ch2.HERO_CH2_FREQ_10.qn_new());
    /*#pXX.GOJY*/ triwire GOJY_D6 = tri6_nn(HUMA_NR24_RDn, reg_new.ch2.EMER_NR24_LENENp.qp_new());

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(HUNA_D0);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(JARO_D1);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(JEKE_D2);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GOJY_D6);
  }
}
















