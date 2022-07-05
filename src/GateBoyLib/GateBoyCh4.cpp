#include "GateBoySPU.h"

#include "GateBoyState.h"

#ifdef SIM_AUDIO

void tick_ch4(const GateBoyState& reg_old, GateBoyState& reg_new) {

  {
    /*#p19.DACO*/ wire DACO_FF21_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.COVO_ADDR_FF21p());
    /*#p19.GOKO*/ wire GOKO_FF21_WRp = and2(reg_new.cpu_abus.COVO_ADDR_FF21p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*#p19.DYKE*/ wire DYKE_FF21_WRn = not1(DACO_FF21_WRp);
    /*#p19.FUPA*/ wire FUPA_FF21_WRn = not1(GOKO_FF21_WRp);
    
    /*_p19.EMOK*/ reg_new.ch4.EMOK_NR42_ENV_TIMER0.dff9b(DYKE_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*_p19.ETYJ*/ reg_new.ch4.ETYJ_NR42_ENV_TIMER1.dff9b(DYKE_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*_p19.EZYK*/ reg_new.ch4.EZYK_NR42_ENV_TIMER2.dff9b(DYKE_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*_p19.GEKY*/ reg_new.ch4.GEKY_NR42_ENV_DIR   .dff9b(FUPA_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*_p19.GARU*/ reg_new.ch4.GARU_NR42_ENV0      .dff9b(FUPA_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*_p19.GOKY*/ reg_new.ch4.GOKY_NR42_ENV1      .dff9b(FUPA_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*_p19.GOZO*/ reg_new.ch4.GOZO_NR42_ENV2      .dff9b(FUPA_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*_p19.GEDU*/ reg_new.ch4.GEDU_NR42_ENV3      .dff9b(FUPA_FF21_WRn, reg_new.FEXO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
  }

  {
    /*#p19.HUMO*/ wire HUMO_FF22_WRp =  and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.EKEZ_ADDR_FF22p());
    /*#p19.GETU*/ wire GETU_FF22_WRp =  and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.EKEZ_ADDR_FF22p());
    /*#p19.HOVA*/ wire HOVA_FF22_WRn = not1(HUMO_FF22_WRp);
    /*#p19.HOSO*/ wire HOSO_FF22_WRn = nand2(reg_new.cpu_abus.EKEZ_ADDR_FF22p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*#p19.EFUG*/ wire EFUG_FF22_WRn = not1(GETU_FF22_WRp);

    /*#p19.JARE*/ reg_new.ch4.JARE_NR43_DIV0 .dff9b(HOVA_FF22_WRn, reg_new.KAME_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p19.JERO*/ reg_new.ch4.JERO_NR43_DIV1 .dff9b(HOVA_FF22_WRn, reg_new.KAME_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p19.JAKY*/ reg_new.ch4.JAKY_NR43_DIV2 .dff9b(HOVA_FF22_WRn, reg_new.KAME_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*#p19.JAMY*/ reg_new.ch4.JAMY_NR43_MODE .dff9b(HOSO_FF22_WRn, reg_new.HYNE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*#p19.FETA*/ reg_new.ch4.FETA_NR43_FREQ0.dff9b(EFUG_FF22_WRn, reg_new.DAPA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*#p19.FYTO*/ reg_new.ch4.FYTO_NR43_FREQ1.dff9b(EFUG_FF22_WRn, reg_new.DAPA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*_p19.GOGO*/ reg_new.ch4.GOGO_NR43_FREQ2.dff9b(EFUG_FF22_WRn, reg_new.DAPA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*_p19.GAFO*/ reg_new.ch4.GAFO_NR43_FREQ3.dff9b(EFUG_FF22_WRn, reg_new.DAPA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
  }


  /*#p20.GYSU*/ reg_new.ch4.GYSU_CH4_TRIG.dff17(reg_new.sys_clk.DOVA_ABCDxxxx(), reg_new.GASO_APU_RSTn_new(), reg_old.ch4.HOGA_NR44_TRIG.qp_oldB());

  /*#p20.FALE*/ wire FALE_RESET_LATCHp_old = nor2(reg_old.KEBA_APU_RSTp_old(), reg_old.ch4.GORA_CH4_TRIGp.qp_old());

  /*#p20.GONE*/ reg_new.ch4.GONE_CH4_TRIGp.dff17(reg_new.spu.HAMA_CLK_512K_new(), FALE_RESET_LATCHp_old,         reg_old.ch4.HAZO_CH4_TRIGn.qn_old());
  /*#p20.GORA*/ reg_new.ch4.GORA_CH4_TRIGp.dff17(reg_new.spu.HAMA_CLK_512K_new(), reg_new.FEBY_APU_RSTn_new(),   reg_old.ch4.GONE_CH4_TRIGp.qp_old());
  /*#p20.GATY*/ reg_new.ch4.GATY_CH4_TRIGp.dff17(reg_new.spu.HAMA_CLK_512K_new(), reg_new.FEBY_APU_RSTn_new(),   reg_old.ch4.GORA_CH4_TRIGp.qp_old());

  /*#p20.FALE*/ wire FALE_RESET_LATCHp_new = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch4.GORA_CH4_TRIGp.qp_new());
  /*#p20.HELU*/ wire HELU_RESET_LATCHp  = not1(FALE_RESET_LATCHp_new);
  /*#p20.HAZO*/ reg_new.ch4.HAZO_CH4_TRIGn.nor_latch(HELU_RESET_LATCHp, reg_new.ch4.GYSU_CH4_TRIG.qp_new());

  /*#p20.GONE*/ reg_new.ch4.GONE_CH4_TRIGp.dff17_any(reg_new.spu.HAMA_CLK_512K_new(), FALE_RESET_LATCHp_new,         reg_old.ch4.HAZO_CH4_TRIGn.qn_old());
  /*#p20.GORA*/ reg_new.ch4.GORA_CH4_TRIGp.dff17_any(reg_new.spu.HAMA_CLK_512K_new(), reg_new.FEBY_APU_RSTn_new(),   reg_old.ch4.GONE_CH4_TRIGp.qp_old());
  /*#p20.GATY*/ reg_new.ch4.GATY_CH4_TRIGp.dff17_any(reg_new.spu.HAMA_CLK_512K_new(), reg_new.FEBY_APU_RSTn_new(),   reg_old.ch4.GORA_CH4_TRIGp.qp_old());


  /*#p19.DULU*/ wire DULU_FF23_WRn = nand2(reg_new.ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.CUGE_ADDR_FF23p());
  /*#p19.FOXE*/ wire FOXE_FF23_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.CUGE_ADDR_FF23p());
  /*#p20.GUZY*/ wire GUZY_NR44_TRIG_RST = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch4.GYSU_CH4_TRIG.qp_new());

  /*#p19.CUNY*/ reg_new.ch4.CUNY_NR44_LEN_ENp.dff9b(DULU_FF23_WRn, reg_new.CABE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
  /*#p19.HOGA*/ reg_new.ch4.HOGA_NR44_TRIG.dff9b(FOXE_FF23_WRn, GUZY_NR44_TRIG_RST, reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());

  /*#p20.GUNY*/ wire GUNY_FREQ_GATE_RSTn_new = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch4.GONE_CH4_TRIGp.qp_new());
  /*#p20.HYNO*/ wire HYNO_DIV_MAX_old = or3(reg_old.ch4.JYFU_DIV2.qp_old(), reg_old.ch4.JYRE_DIV1.qp_old(), reg_old.ch4.JYCO_DIV0.qp_old());
  /*#p20.GARY*/ reg_new.ch4.GARY_FREQ_GATEp.dff17(reg_new.spu.GYBA_CLK_1M(), GUNY_FREQ_GATE_RSTn_new, HYNO_DIV_MAX_old);






  {

    /*#p20.CARY*/ wire CARY_FREQ_CLK = and2(reg_new.spu.BAVU_CLK_1M(), reg_new.ch4.GARY_FREQ_GATEp.qp_new());
    
    /*#p20.CEXO*/ reg_new.ch4.CEXO_FREQ_00.dff17(CARY_FREQ_CLK,                     reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.CEXO_FREQ_00.qn_old());
    /*_p20.DEKO*/ reg_new.ch4.DEKO_FREQ_01.dff17(reg_new.ch4.CEXO_FREQ_00.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DEKO_FREQ_01.qn_old());
    /*_p20.EZEF*/ reg_new.ch4.EZEF_FREQ_02.dff17(reg_new.ch4.DEKO_FREQ_01.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.EZEF_FREQ_02.qn_old());
    /*_p20.EPOR*/ reg_new.ch4.EPOR_FREQ_03.dff17(reg_new.ch4.EZEF_FREQ_02.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.EPOR_FREQ_03.qn_old());
    /*_p20.DURE*/ reg_new.ch4.DURE_FREQ_04.dff17(reg_new.ch4.EPOR_FREQ_03.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DURE_FREQ_04.qn_old());
    /*_p20.DALE*/ reg_new.ch4.DALE_FREQ_05.dff17(reg_new.ch4.DURE_FREQ_04.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DALE_FREQ_05.qn_old());
    /*_p20.DOKE*/ reg_new.ch4.DOKE_FREQ_06.dff17(reg_new.ch4.DALE_FREQ_05.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DOKE_FREQ_06.qn_old());
    /*_p20.DEMO*/ reg_new.ch4.DEMO_FREQ_07.dff17(reg_new.ch4.DOKE_FREQ_06.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DEMO_FREQ_07.qn_old());
    /*_p20.DOSE*/ reg_new.ch4.DOSE_FREQ_08.dff17(reg_new.ch4.DEMO_FREQ_07.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DOSE_FREQ_08.qn_old());
    /*_p20.DETE*/ reg_new.ch4.DETE_FREQ_09.dff17(reg_new.ch4.DOSE_FREQ_08.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DETE_FREQ_09.qn_old());
    /*_p20.ERUT*/ reg_new.ch4.ERUT_FREQ_10.dff17(reg_new.ch4.DETE_FREQ_09.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.ERUT_FREQ_10.qn_old());
    /*_p20.DOTA*/ reg_new.ch4.DOTA_FREQ_11.dff17(reg_new.ch4.ERUT_FREQ_10.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DOTA_FREQ_11.qn_old());
    /*_p20.DERE*/ reg_new.ch4.DERE_FREQ_12.dff17(reg_new.ch4.DOTA_FREQ_11.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.DERE_FREQ_12.qn_old());
    /*_p20.ESEP*/ reg_new.ch4.ESEP_FREQ_13.dff17(reg_new.ch4.DERE_FREQ_12.qn_new(), reg_new.DAPA_APU_RSTn_new(), reg_old.ch4.ESEP_FREQ_13.qn_old());
  }

  // 14-to-1 mux to select LFSR clock

  /*_p20.EMOF*/ wire EMOF_LFSR_CLK_MUX_7 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qp_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qp_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qp_newB());
  /*_p20.ELAR*/ wire ELAR_LFSR_CLK_MUX_6 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qp_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qp_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qn_newB());
  /*_p20.DUDU*/ wire DUDU_LFSR_CLK_MUX_5 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qp_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qn_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qp_newB());
  /*_p20.ETAT*/ wire ETAT_LFSR_CLK_MUX_4 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qp_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qn_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qn_newB());
  /*_p20.FURA*/ wire FURA_LFSR_CLK_MUX_3 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qn_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qp_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qp_newB());
  /*_p20.ETAR*/ wire ETAR_LFSR_CLK_MUX_2 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qn_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qp_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qn_newB());
  /*_p20.EVER*/ wire EVER_LFSR_CLK_MUX_1 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qn_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qn_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qp_newB());
  /*_p20.ETOV*/ wire ETOV_LFSR_CLK_MUX_0 = nor3(reg_new.ch4.FETA_NR43_FREQ0.qn_newB(), reg_new.ch4.FYTO_NR43_FREQ1.qn_newB(), reg_new.ch4.GOGO_NR43_FREQ2.qn_newB());

  /*_p20.ETYR*/ wire ETYR_LFSR_CLK_MUX_A = amux6(reg_new.ch4.ESEP_FREQ_13.qp_new(), DUDU_LFSR_CLK_MUX_5,
                                                 reg_new.ch4.DERE_FREQ_12.qp_new(), ETAT_LFSR_CLK_MUX_4,
                                                 reg_new.ch4.DOTA_FREQ_11.qp_new(), FURA_LFSR_CLK_MUX_3,
                                                 reg_new.ch4.ERUT_FREQ_10.qp_new(), ETAR_LFSR_CLK_MUX_2,
                                                 reg_new.ch4.DETE_FREQ_09.qp_new(), EVER_LFSR_CLK_MUX_1,
                                                 reg_new.ch4.DOSE_FREQ_08.qp_new(), ETOV_LFSR_CLK_MUX_0);

  /*_p20.ELYX*/ wire ELYX_LFSR_CLK_MUX_B = amux4(reg_new.ch4.DEMO_FREQ_07.qp_new(), EMOF_LFSR_CLK_MUX_7,
                                                 reg_new.ch4.DOKE_FREQ_06.qp_new(), ELAR_LFSR_CLK_MUX_6,
                                                 reg_new.ch4.DALE_FREQ_05.qp_new(), DUDU_LFSR_CLK_MUX_5,
                                                 reg_new.ch4.DURE_FREQ_04.qp_new(), ETAT_LFSR_CLK_MUX_4);

  /*_p20.DARY*/ wire DARY_LFSR_CLK_MUX_C = amux4(reg_new.ch4.EPOR_FREQ_03.qp_new(), FURA_LFSR_CLK_MUX_3,
                                                 reg_new.ch4.EZEF_FREQ_02.qp_new(), ETAR_LFSR_CLK_MUX_2,
                                                 reg_new.ch4.DEKO_FREQ_01.qp_new(), EVER_LFSR_CLK_MUX_1,
                                                 reg_new.ch4.CEXO_FREQ_00.qp_new(), ETOV_LFSR_CLK_MUX_0);

  /*_p20.ERYF*/ wire ERYF_LFSR_CLK_MUX_D = or2(ELYX_LFSR_CLK_MUX_B, DARY_LFSR_CLK_MUX_C);

  /*#p20.FEME*/ wire FEME_LFSR_CLKp_new = mux2p(reg_new.ch4.GAFO_NR43_FREQ3.qp_newB(), ETYR_LFSR_CLK_MUX_A, ERYF_LFSR_CLK_MUX_D);






  {
    /*#p19.CAZE*/ wire CAZE_FF20_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DANU_ADDR_FF20p());
    /*#p19.FURU*/ wire FURU_FF20_WRa = not1(CAZE_FF20_WRn);
    /*#p19.DOTU*/ wire DOTU_FF20_WRb = not1(CAZE_FF20_WRn);
    /*#p19.EPEK*/ wire EPEK_FF20_WRc = not1(CAZE_FF20_WRn);

    // using FUGO_old to break the loop

    /*#p19.DODA*/ wire DODA_LEN_CLKn = nor3(reg_old.ch4.FUGO_CH4_LEN_DONEp.qp_old(), reg_new.spu.BUFY_CLK_256n(), reg_new.ch4.CUNY_NR44_LEN_ENp.qn_newB());
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa = not1(DODA_LEN_CLKn);

    /*#p19.DANO*/ reg_new.ch4.DANO_NR41_LEN0.dff20(CUWA_LEN_CLKa,                         DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p19.FAVY*/ reg_new.ch4.FAVY_NR41_LEN1.dff20(reg_new.ch4.DANO_NR41_LEN0.qp_new(),   DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p19.DENA*/ reg_new.ch4.DENA_NR41_LEN2.dff20(reg_new.ch4.FAVY_NR41_LEN1.qp_new(),   DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*#p19.CEDO*/ reg_new.ch4.CEDO_NR41_LEN3.dff20(reg_new.ch4.DENA_NR41_LEN2.qp_new(),   DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());

    /*#p19.DOPU*/ wire DOPU_NR41_LEN3p = not1(reg_new.ch4.CEDO_NR41_LEN3.qn_new());
    /*#p19.FYLO*/ reg_new.ch4.FYLO_NR41_LEN4.dff20(DOPU_NR41_LEN3p,                       EPEK_FF20_WRc, reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*#p19.EDOP*/ reg_new.ch4.EDOP_NR41_LEN5.dff20(reg_new.ch4.FYLO_NR41_LEN4.qp_new(),   EPEK_FF20_WRc, reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());

    /*#p19.GAPY*/ wire GAPY_LEN_DONE_RSTn = nor3(FURU_FF20_WRa, reg_new.KEBA_APU_RSTp_new(), reg_new.ch4.GONE_CH4_TRIGp.qp_new());
    /*#p19.FUGO*/ reg_new.ch4.FUGO_CH4_LEN_DONEp.dff17(reg_new.ch4.EDOP_NR41_LEN5.qn_new(), GAPY_LEN_DONE_RSTn, reg_old.ch4.FUGO_CH4_LEN_DONEp.qn_old());
  }







  {
    /*#p20.EFOT*/ wire EFOT_CH4_STOPp = and2(reg_new.ch4.CUNY_NR44_LEN_ENp.qp_newB(), reg_new.ch4.FUGO_CH4_LEN_DONEp.qp_new());
    /*#p20.FEGY*/ wire FEGY_CH4_STOPp = or3(reg_new.KEBA_APU_RSTp_new(), EFOT_CH4_STOPp, reg_new.ch4.GEVY_CH4_AMP_ENn());
    /*#p20.GENA*/ reg_new.ch4.GENA_CH4_ACTIVEp.nor_latch(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), FEGY_CH4_STOPp);
  }

  {
    /*#p20.HERY*/ wire HERY_DIV_GATE1n = nor2(reg_new.ch4.GEVY_CH4_AMP_ENn(), reg_new.KEBA_APU_RSTp_new());
    /*#p20.HAPU*/ wire HAPU_CH4_TRIGn = not1(reg_new.ch4.GATY_CH4_TRIGp.qp_new());
    /*#p20.JERY*/ reg_new.ch4.JERY_DIV_GATE.nand_latch(HERY_DIV_GATE1n, HAPU_CH4_TRIGn);
  }

  {
    /*#p20.KYKU*/ wire KYKU_DIV_CLKb  = or2(reg_new.ch4.JERY_DIV_GATE.qp_new(), reg_new.spu.JESO_CLK_512K.qp_new());
    /*#p20.KONY*/ wire KONY_DIV_CLKn  = not1(KYKU_DIV_CLKb);
    /*#p20.KANU*/ wire KANU_DIV_CLKa  = not1(KONY_DIV_CLKn);

    /*#p20.GOFU*/ wire GOFU_DIV_LOADn = nor2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.GARY_FREQ_GATEp.qp_new());
    /*#p20.HUCE*/ wire HUCE_DIV_LOAD  = not1(GOFU_DIV_LOADn);

    /*#p20.JYCO*/ reg_new.ch4.JYCO_DIV0.dff20(KANU_DIV_CLKa,                  HUCE_DIV_LOAD, reg_old.ch4.JARE_NR43_DIV0.qn_oldB());
    /*#p20.JYRE*/ reg_new.ch4.JYRE_DIV1.dff20(reg_new.ch4.JYCO_DIV0.qp_new(), HUCE_DIV_LOAD, reg_old.ch4.JERO_NR43_DIV1.qn_oldB());
    /*_p20.JYFU*/ reg_new.ch4.JYFU_DIV2.dff20(reg_new.ch4.JYRE_DIV1.qp_new(), HUCE_DIV_LOAD, reg_old.ch4.JAKY_NR43_DIV2.qn_oldB());
  }


  //----------
  // The actual LFSR



  {
    /*#p20.JYJA*/ wire JYJA_LFSR_CLKn_new = not1(FEME_LFSR_CLKp_new);
    /*#p20.GUFA*/ wire GUFA_LFSR_CLKn_new = not1(FEME_LFSR_CLKp_new);
    /*#p20.GYVE*/ wire GYVE_LFSR_CLKp_new = not1(GUFA_LFSR_CLKn_new);
    /*#p20.KARA*/ wire KARA_LFSR_CLKn_new = not1(GYVE_LFSR_CLKp_new);
    /*#p20.KOPA*/ wire KOPA_LFSR_CLKp_new = not1(KARA_LFSR_CLKn_new);

    /*#p20.GEPO*/ wire GEPO_LFSR_RSTa = or2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.KEBA_APU_RSTp_new());
    /*#p20.GOGE*/ wire GOGE_LFSR_RSTn = not1(GEPO_LFSR_RSTa);

    /*#p20.HURA*/ wire HURA_LFSR_IN_old = xor2(reg_old.ch4.HEZU_LFSR_15.qp_old(), reg_old.ch4.HYRO_LFSR_14.qp_old());

    /*#p20.JOTO*/ reg_new.ch4.JOTO_LFSR_00.dff17(JYJA_LFSR_CLKn_new, GOGE_LFSR_RSTn, HURA_LFSR_IN_old);
    /*#p20.KOMU*/ reg_new.ch4.KOMU_LFSR_01.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.JOTO_LFSR_00.qp_old());
    /*#p20.KETU*/ reg_new.ch4.KETU_LFSR_02.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KOMU_LFSR_01.qp_old());
    /*#p20.KUTA*/ reg_new.ch4.KUTA_LFSR_03.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KETU_LFSR_02.qp_old());
    /*#p20.KUZY*/ reg_new.ch4.KUZY_LFSR_04.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KUTA_LFSR_03.qp_old());
    /*#p20.KYWY*/ reg_new.ch4.KYWY_LFSR_05.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KUZY_LFSR_04.qp_old());
    /*#p20.JAJU*/ reg_new.ch4.JAJU_LFSR_06.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KYWY_LFSR_05.qp_old());
    /*#p20.HAPE*/ reg_new.ch4.HAPE_LFSR_07.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.JAJU_LFSR_06.qp_old());
    /*#p20.JUXE*/ reg_new.ch4.JUXE_LFSR_08.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.HAPE_LFSR_07.qp_old());

    /*#p20.KAVU*/ wire KAVU_LFSR_FB_old = amux2(reg_old.ch4.JOTO_LFSR_00.qp_old(), reg_old.ch4.JAMY_NR43_MODE.qp_oldB(), reg_old.ch4.JAMY_NR43_MODE.qn_oldB(), reg_old.ch4.JUXE_LFSR_08.qp_old());
    /*#p20.JEPE*/ reg_new.ch4.JEPE_LFSR_09.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, KAVU_LFSR_FB_old);
    /*#p20.JAVO*/ reg_new.ch4.JAVO_LFSR_10.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.JEPE_LFSR_09.qp_old());
    /*#p20.HEPA*/ reg_new.ch4.HEPA_LFSR_11.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.JAVO_LFSR_10.qp_old());
    /*#p20.HORY*/ reg_new.ch4.HORY_LFSR_12.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.HEPA_LFSR_11.qp_old());
    /*#p20.HENO*/ reg_new.ch4.HENO_LFSR_13.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.HORY_LFSR_12.qp_old());
    /*#p20.HYRO*/ reg_new.ch4.HYRO_LFSR_14.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.HENO_LFSR_13.qp_old());
    /*#p20.HEZU*/ reg_new.ch4.HEZU_LFSR_15.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.HYRO_LFSR_14.qp_old());
  }

  //----------
  // Env

  {
    // Generates a 1 usec pulse when the env timer hits 111

    /*#p20.EJEX*/ wire EJEX_ENV_TIMER_MAX_old = or3(reg_old.ch4.DOGO_ENV_DELAY2n.qp_old(), reg_old.ch4.COFE_ENV_DELAY1n.qp_old(), reg_old.ch4.CUNA_ENV_DELAY0n.qp_old());

    /*#p20.GEXE*/ wire GEXE_ENV_PULSEn_old  = not1(reg_old.ch4.FOSY_ENV_CLKp.qp_old());
    /*#p20.HURY*/ wire HURY_ENV_PULSE_RSTp_old = nor2(reg_old.spu.HORU_CLK_512_old(), GEXE_ENV_PULSEn_old);
    /*#p20.GOPA*/ wire GOPA_ENV_PULSE_RSTn_old = nor4(HURY_ENV_PULSE_RSTp_old, reg_old.ch4.FOWA_ENV_OFFp_old(), reg_old.ch4.GONE_CH4_TRIGp.qp_old(), reg_old.KEBA_APU_RSTp_old());

    /*#p20.FOSY*/ reg_new.ch4.FOSY_ENV_CLKp.dff17(reg_new.spu.HORU_CLK_512_new(), GOPA_ENV_PULSE_RSTn_old, EJEX_ENV_TIMER_MAX_old);

    /*#p20.GEXE*/ wire GEXE_ENV_PULSEn_new  = not1(reg_new.ch4.FOSY_ENV_CLKp.qp_new());
    /*#p20.HURY*/ wire HURY_ENV_PULSE_RSTp_new = nor2(reg_new.spu.HORU_CLK_512_new(), GEXE_ENV_PULSEn_new);
    /*#p20.GOPA*/ wire GOPA_ENV_PULSE_RSTn_new = nor4(HURY_ENV_PULSE_RSTp_new, reg_new.ch4.FOWA_ENV_OFFp_new(), reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.KEBA_APU_RSTp_new());

    /*#p20.FOSY*/ reg_new.ch4.FOSY_ENV_CLKp.dff17_any(reg_new.spu.HORU_CLK_512_new(), GOPA_ENV_PULSE_RSTn_new, EJEX_ENV_TIMER_MAX_old);
  }

  {
    /*#p20.ABEL*/ reg_new.ch4.ABEL_CLK_64.dff17(reg_new.spu.ALOP_CLK_128(), reg_new.BOKY_APU_RSTn_new(), reg_old.ch4.ABEL_CLK_64.qn_old());
    /*#p20.BAWA*/ wire BAWA_CLK_64_new  = not1(reg_new.ch4.ABEL_CLK_64.qp_new());
    /*#p20.BUXO*/ wire BUXO_CLK_64_new  = not1(BAWA_CLK_64_new);

    /*#p20.ENEC*/ wire ENEC_ENV_TIMER_LOADn_new = nor2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.FOSY_ENV_CLKp.qp_new());
    /*#p20.DAPY*/ wire DAPY_ENV_TIMER_LOADp_new  = not1(ENEC_ENV_TIMER_LOADn_new);

    /*#p20.CUNA*/ reg_new.ch4.CUNA_ENV_DELAY0n.dff20(BUXO_CLK_64_new,                       DAPY_ENV_TIMER_LOADp_new, reg_old.ch4.EMOK_NR42_ENV_TIMER0.qn_oldB());
    /*#p20.COFE*/ reg_new.ch4.COFE_ENV_DELAY1n.dff20(reg_new.ch4.CUNA_ENV_DELAY0n.qp_new(), DAPY_ENV_TIMER_LOADp_new, reg_old.ch4.ETYJ_NR42_ENV_TIMER1.qn_oldB());
    /*#p20.DOGO*/ reg_new.ch4.DOGO_ENV_DELAY2n.dff20(reg_new.ch4.COFE_ENV_DELAY1n.qp_new(), DAPY_ENV_TIMER_LOADp_new, reg_old.ch4.EZYK_NR42_ENV_TIMER2.qn_oldB());
  }


  {
    /*#p20.EMET*/ wire EMET_ENV_STOP_RSTn = nor2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.KEBA_APU_RSTp_new());
    
    /*#p20.DARO*/ wire DARO_ENV_BOTp_old  = nor5(reg_old.ch4.GEKY_NR42_ENV_DIR.qp_oldB(), reg_old.ch4.FEKO_CH4_VOL0.qp_old(), reg_old.ch4.FATY_CH4_VOL1.qp_old(), reg_old.ch4.FERU_CH4_VOL2.qp_old(), reg_old.ch4.FYRO_CH4_VOL3.qp_old());
    /*#p20.CUTY*/ wire CUTY_ENV_TOPn_old = nand5(reg_old.ch4.GEKY_NR42_ENV_DIR.qp_oldB(), reg_old.ch4.FEKO_CH4_VOL0.qp_old(), reg_old.ch4.FATY_CH4_VOL1.qp_old(), reg_old.ch4.FERU_CH4_VOL2.qp_old(), reg_old.ch4.FYRO_CH4_VOL3.qp_old());
    /*#p20.DUBO*/ wire DUBO_ENV_TOPp_old = not1(CUTY_ENV_TOPn_old);
    /*#p20.EVUR*/ wire EVUR_ENV_MAXp_old  = or2(DARO_ENV_BOTp_old, DUBO_ENV_TOPp_old);

    /*#p20.FYNO*/ reg_new.ch4.FYNO_ENV_MAXp.dff17(reg_new.ch4.FOSY_ENV_CLKp.qp_new(), EMET_ENV_STOP_RSTn, EVUR_ENV_MAXp_old);
  }





  {
    /*#p20.ENUR*/ wire ENUR_ENV_STARTp = or2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch4.GONE_CH4_TRIGp.qp_new());
    /*#p20.EROX*/ reg_new.ch4.EROX_ENV_RUNNINGn.nor_latch(reg_new.ch4.FYNO_ENV_MAXp.qp_new(), ENUR_ENV_STARTp);

    /*#p20.FELO*/ wire FELO_ENV_CLK  = or3(reg_new.ch4.FOSY_ENV_CLKp.qp_new(), reg_new.ch4.FOWA_ENV_OFFp_new(), reg_new.ch4.EROX_ENV_RUNNINGn.qp_new());
    /*#p20.FOLE*/ wire FOLE_VOL_CLK0n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIR.qp_newB(), FELO_ENV_CLK,               FELO_ENV_CLK,               reg_new.ch4.GEKY_NR42_ENV_DIR.qn_newB());
    /*#p20.FEKO*/ reg_new.ch4.FEKO_CH4_VOL0.dff20(FOLE_VOL_CLK0n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_old.ch4.GARU_NR42_ENV0.qp_oldB());

    /*#p20.ETEF*/ wire ETEF_VOL_CLK1n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIR.qp_newB(), reg_new.ch4.FEKO_CH4_VOL0.qp_new(), reg_new.ch4.FEKO_CH4_VOL0.qn_new(), reg_new.ch4.GEKY_NR42_ENV_DIR.qn_newB());
    /*#p20.FATY*/ reg_new.ch4.FATY_CH4_VOL1.dff20(ETEF_VOL_CLK1n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_old.ch4.GOKY_NR42_ENV1.qp_oldB());

    /*#p20.EDYF*/ wire EDYF_VOL_CLK2n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIR.qp_newB(), reg_new.ch4.FATY_CH4_VOL1.qp_new(), reg_new.ch4.FATY_CH4_VOL1.qn_new(), reg_new.ch4.GEKY_NR42_ENV_DIR.qn_newB());
    /*#p20.FERU*/ reg_new.ch4.FERU_CH4_VOL2.dff20(EDYF_VOL_CLK2n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_old.ch4.GOZO_NR42_ENV2.qp_oldB());

    /*#p20.ELAF*/ wire ELAF_VOL_CLK3n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIR.qp_newB(), reg_new.ch4.FERU_CH4_VOL2.qp_new(), reg_new.ch4.FERU_CH4_VOL2.qn_new(), reg_new.ch4.GEKY_NR42_ENV_DIR.qn_newB());
    /*#p20.FYRO*/ reg_new.ch4.FYRO_CH4_VOL3.dff20(ELAF_VOL_CLK3n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_old.ch4.GEDU_NR42_ENV3.qp_oldB());
  }

  {
    //----------
    // Output

    /*#p20.DYRY*/ wire DYRY_DBG = and2(reg_new.ch4.CUNY_NR44_LEN_ENp.qp_newB(), reg_new.EDEK_NR52_DBG_APUp());
    /*#p20.GAME*/ wire GAME_LFSR_OUT = and2(reg_new.ch4.GENA_CH4_ACTIVEp.qp_new(), reg_new.ch4.HEZU_LFSR_15.qp_new());
    /*#p20.EZUL*/ wire EZUL_CH4_BIT_MUX = mux2p(DYRY_DBG, FEME_LFSR_CLKp_new, GAME_LFSR_OUT);
    /*#p20.CEPY*/ wire CEPY_NR44_STOPn = not1(reg_new.ch4.CUNY_NR44_LEN_ENp.qp_newB());
    /*#p20.COTE*/ wire COTE_DBG_CH4_MUTE = and2(CEPY_NR44_STOPn, reg_new.EDEK_NR52_DBG_APUp());
    /*#p20.DATO*/ wire DATO_CH4_RAW_BIT  = or2(EZUL_CH4_BIT_MUX, COTE_DBG_CH4_MUTE);

    /*#p20.AKOF*/ wire AKOF_CH4_DAC0 = and2(reg_new.ch4.FEKO_CH4_VOL0.qp_new(), DATO_CH4_RAW_BIT);
    /*#p20.BYZY*/ wire BYZY_CH4_DAC1 = and2(reg_new.ch4.FATY_CH4_VOL1.qp_new(), DATO_CH4_RAW_BIT);
    /*#p20.APYR*/ wire APYR_CH4_DAC2 = and2(reg_new.ch4.FERU_CH4_VOL2.qp_new(), DATO_CH4_RAW_BIT);
    /*#p20.BOZA*/ wire BOZA_CH4_DAC3 = and2(reg_new.ch4.FYRO_CH4_VOL3.qp_new(), DATO_CH4_RAW_BIT);
  }





#if 0
  /*#p20.DYRY*/ wire DYRY_DBG = and2(reg_new.ch4.CUNY_NR44_LEN_ENp.qn_new(), reg_new.EDEK_NR52_DBG_APUp());
  /*_p20.COMO*/ wire COMO_DBG = and2(DYRY_DBG, COSA_CPU_RDp());
  /*_p20.BAGU*/ wire BAGU_DBG = nand2(CUGE_ADDR_FF23p(), COMO_DBG);
  /*_p20.BEFA*/ wire BEFA_DBG = not1(CARY_FREQ_CLK);
  /*_p20.ATEL*/ triwire ATEL = tri6_nn(BAGU_DBG, BEFA_DBG); // goes to data bus D0
#endif




























  {
    /*#p19.BOFY*/ wire BOFY_FF21n    = not1(reg_new.cpu_abus.COVO_ADDR_FF21p());
    /*#p19.GONY*/ wire GONY_FF21n    = not1(reg_new.cpu_abus.COVO_ADDR_FF21p());
    /*#p19.BOXE*/ wire BOXE_FF21_RDn = or2(BOFY_FF21n, reg_new.cpu_signals.AGUZ_CPU_RDn());
    /*#p19.HASU*/ wire HASU_FF21_RDn = or2(GONY_FF21n, reg_new.cpu_signals.AGUZ_CPU_RDn());

    /*#p19.DEMY*/ triwire DEMY = tri6_nn(BOXE_FF21_RDn, reg_new.ch4.EMOK_NR42_ENV_TIMER0.qn_newB());
    /*#p19.COCE*/ triwire COCE = tri6_nn(BOXE_FF21_RDn, reg_new.ch4.ETYJ_NR42_ENV_TIMER1.qn_newB());
    /*#p19.CUZU*/ triwire CUZU = tri6_nn(BOXE_FF21_RDn, reg_new.ch4.EZYK_NR42_ENV_TIMER2.qn_newB());
    /*#p19.GOME*/ triwire GOME = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GEKY_NR42_ENV_DIR   .qn_newB());
    /*#p19.HEDA*/ triwire HEDA = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GARU_NR42_ENV0      .qn_newB());
    /*#p19.GODU*/ triwire GODU = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GOKY_NR42_ENV1      .qn_newB());
    /*#p19.HOGE*/ triwire HOGE = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GOZO_NR42_ENV2      .qn_newB());
    /*#p19.HACU*/ triwire HACU = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GEDU_NR42_ENV3      .qn_newB());
  }

  {
    /*#p19.KOKU*/ wire KOKU_ADDR_FF22n = not1(reg_new.cpu_abus.EKEZ_ADDR_FF22p());
    /*#p19.GUGO*/ wire GUGO_ADDR_FF22n = not1(reg_new.cpu_abus.EKEZ_ADDR_FF22p());
    /*#p19.KEKA*/ wire KEKA_FF22_RDn =   or2(KOKU_ADDR_FF22n, reg_new.cpu_signals.AGUZ_CPU_RDn());
    /*#p19.JORA*/ wire JORA_FF22_RDn = nand2(reg_new.cpu_signals.KAGE_CPU_RDp(), reg_new.cpu_abus.EKEZ_ADDR_FF22p());
    /*#p19.HEZE*/ wire HEZE_FF22_RDn =   or2(GUGO_ADDR_FF22n, reg_new.cpu_signals.AGUZ_CPU_RDn());

    /*#p19.KAMO*/ triwire KAMO = tri6_nn(KEKA_FF22_RDn, reg_new.ch4.JARE_NR43_DIV0 .qn_newB());
    /*#p19.KAKU*/ triwire KAKU = tri6_nn(KEKA_FF22_RDn, reg_new.ch4.JERO_NR43_DIV1 .qn_newB());
    /*_p19.KYRO*/ triwire KYRO = tri6_nn(KEKA_FF22_RDn, reg_new.ch4.JAKY_NR43_DIV2 .qn_newB());
    /*_p19.KETA*/ triwire KETA = tri6_nn(JORA_FF22_RDn, reg_new.ch4.JAMY_NR43_MODE .qn_newB());
    /*_p19.GEDA*/ triwire GEDA = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.FETA_NR43_FREQ0.qn_newB());
    /*_p19.GYPE*/ triwire GYPE = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.FYTO_NR43_FREQ1.qn_newB());
    /*_p19.GAKA*/ triwire GAKA = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.GOGO_NR43_FREQ2.qn_newB());
    /*_p19.HAPY*/ triwire HAPY = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.GAFO_NR43_FREQ3.qn_newB());
  }

  {
    /*#p19.BARE*/ wire BARE_FF23_RDn = nand2(reg_new.cpu_abus.CUGE_ADDR_FF23p(), reg_new.cpu_signals.BYLO_CPU_RDp());
    /*#p19.CURY*/ triwire CURY = tri6_nn(BARE_FF23_RDn, reg_new.ch4.CUNY_NR44_LEN_ENp.qn_newB());
  }

}

#endif