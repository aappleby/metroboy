  #include "GateBoySPU.h"

#include "GateBoy.h"

#ifdef SIM_AUDIO

sample_t ch4_audio_out(const SpuChannel4& ch4) {

  ///*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  ///*_p20.DYRY*/ wire DYRY_DBG = and2(ch4.CUNY_NR44_LEN_ENp.qp_old(), EDEK_NR52_DBG_APUp);
  /*_p20.DYRY*/ wire DYRY_DBG = 0;

  /*_p20.GAME*/ wire GAME_LFSR_OUT = and2(ch4.GENA_CH4_ACTIVEp.qp_old(), ch4.HEZU_LFSR_15.qp_old());

  // ignoring debug thing on the audio out here
  wire FEME_LFSR_CLKp_new = 0;

  /*_p20.EZUL*/ wire EZUL_CH4_BIT_MUX = mux2p(DYRY_DBG, FEME_LFSR_CLKp_new, GAME_LFSR_OUT);

  ///*_p20.CEPY*/ wire CEPY_NR44_STOPn = not1(ch4.CUNY_NR44_LEN_ENp.qp_old());
  ///*_p20.COTE*/ wire COTE_DBG_CH4_MUTE = and2(CEPY_NR44_STOPn, EDEK_NR52_DBG_APUp);
  /*_p20.COTE*/ wire COTE_DBG_CH4_MUTE = 0;
  /*_p20.DATO*/ wire DATO_CH4_RAW_BIT  = or2(EZUL_CH4_BIT_MUX, COTE_DBG_CH4_MUTE);

  /*#p20.AKOF*/ wire AKOF_CH4_DAC0 = and2(ch4.FEKO_CH4_VOL0.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.BYZY*/ wire BYZY_CH4_DAC1 = and2(ch4.FATY_CH4_VOL1.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.APYR*/ wire APYR_CH4_DAC2 = and2(ch4.FERU_CH4_VOL2.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.BOZA*/ wire BOZA_CH4_DAC3 = and2(ch4.FYRO_CH4_VOL3.qp_old(), DATO_CH4_RAW_BIT);

  return ((AKOF_CH4_DAC0 & 1) << 0) |
         ((BYZY_CH4_DAC1 & 1) << 1) |
         ((APYR_CH4_DAC2 & 1) << 2) |
         ((BOZA_CH4_DAC3 & 1) << 3);
}

void SpuChannel4::reset() {
EMOK_NR42_ENV_TIMER0p.state = 0x1a;
ETYJ_NR42_ENV_TIMER1p.state = 0x1a;
EZYK_NR42_ENV_TIMER2p.state = 0x1a;
GEKY_NR42_ENV_DIRp.state = 0x1a;
GARU_NR42_ENV0p.state = 0x1a;
GOKY_NR42_ENV1p.state = 0x1a;
GOZO_NR42_ENV2p.state = 0x1a;
GEDU_NR42_ENV3p.state = 0x1a;
JARE_NR43_DIV0p.state = 0x1a;
JERO_NR43_DIV1p.state = 0x1a;
JAKY_NR43_DIV2p.state = 0x1a;
JAMY_NR43_MODEp.state = 0x1a;
FETA_NR43_FREQ0p.state = 0x1a;
FYTO_NR43_FREQ1p.state = 0x1a;
GOGO_NR43_FREQ2p.state = 0x1a;
GAFO_NR43_FREQ3p.state = 0x1a;
GENA_CH4_ACTIVEp.state = 0x18;
GONE_CH4_TRIGp.state = 0x1a;
GORA_CH4_TRIGp.state = 0x1a;
GATY_CH4_TRIGp.state = 0x1a;
FUGO_CH4_LEN_DONEp.state = 0x1a;
DANO_NR41_LEN0p.state = 0x1a;
FAVY_NR41_LEN1p.state = 0x18;
DENA_NR41_LEN2p.state = 0x18;
CEDO_NR41_LEN3p.state = 0x18;
FYLO_NR41_LEN4p.state = 0x18;
EDOP_NR41_LEN5p.state = 0x18;
CUNY_NR44_LEN_ENp.state = 0x1a;
HOGA_NR44_TRIGp.state = 0x1a;
GYSU_CH4_TRIG.state = 0x18;
HAZO_CH4_TRIGn.state = 0x19;
JERY_DIV_GATE.state = 0x19;
JYCO_DIV0.state = 0x1a;
JYRE_DIV1.state = 0x18;
JYFU_DIV2.state = 0x18;
GARY_FREQ_GATEp.state = 0x18;
CEXO_FREQ_00.state = 0x18;
DEKO_FREQ_01.state = 0x1a;
EZEF_FREQ_02.state = 0x1a;
EPOR_FREQ_03.state = 0x1a;
DURE_FREQ_04.state = 0x1a;
DALE_FREQ_05.state = 0x1a;
DOKE_FREQ_06.state = 0x1a;
DEMO_FREQ_07.state = 0x1a;
DOSE_FREQ_08.state = 0x1a;
DETE_FREQ_09.state = 0x1a;
ERUT_FREQ_10.state = 0x1a;
DOTA_FREQ_11.state = 0x1a;
DERE_FREQ_12.state = 0x1a;
ESEP_FREQ_13.state = 0x1a;
JOTO_LFSR_00.state = 0x1a;
KOMU_LFSR_01.state = 0x18;
KETU_LFSR_02.state = 0x18;
KUTA_LFSR_03.state = 0x18;
KUZY_LFSR_04.state = 0x18;
KYWY_LFSR_05.state = 0x18;
JAJU_LFSR_06.state = 0x18;
HAPE_LFSR_07.state = 0x18;
JUXE_LFSR_08.state = 0x18;
JEPE_LFSR_09.state = 0x18;
JAVO_LFSR_10.state = 0x18;
HEPA_LFSR_11.state = 0x18;
HORY_LFSR_12.state = 0x18;
HENO_LFSR_13.state = 0x18;
HYRO_LFSR_14.state = 0x18;
HEZU_LFSR_15.state = 0x18;
ABEL_CLK_64.state = 0x18;
CUNA_ENV_DELAY0n.state = 0x18;
COFE_ENV_DELAY1n.state = 0x19;
DOGO_ENV_DELAY2n.state = 0x1a;
FOSY_ENV_CLKp.state = 0x1a;
FYNO_ENV_MAXp.state = 0x18;
FEKO_CH4_VOL0.state = 0x1a;
FATY_CH4_VOL1.state = 0x1a;
FERU_CH4_VOL2.state = 0x1a;
FYRO_CH4_VOL3.state = 0x1a;
EROX_ENV_RUNNINGn.state = 0x18;
}

void tick_ch4(const GateBoyState& reg_old, GateBoyState& reg_new) {
  auto& abus_new = reg_new.cpu_abus;

  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_new());
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx = not1(BUDE_xxxxEFGH);

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*_p09.HAPO*/ wire HAPO_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*_p09.JYRO*/ wire JYRO_APU_RSTp = or2(HAPO_SYS_RSTp, reg_new.spu.HADA_NR52_ALL_SOUND_ON.qn_new());
  /*_p09.KUBY*/ wire KUBY_APU_RSTn = not1(JYRO_APU_RSTp);
  /*_p09.KEBA*/ wire KEBA_APU_RSTp = not1(KUBY_APU_RSTn);

  /*_p10.DYTE*/ wire DYTE_A00n = not1(abus_new.BUS_CPU_A00p.out_new());
  /*_p10.AFOB*/ wire AFOB_A01n = not1(abus_new.BUS_CPU_A01p.out_new());
  /*_p10.ABUB*/ wire ABUB_A02n = not1(abus_new.BUS_CPU_A02p.out_new());
  /*_p10.ACOL*/ wire ACOL_A03n = not1(abus_new.BUS_CPU_A03p.out_new());
  /*#p10.BOXY*/ wire BOXY_A05n = not1(abus_new.BUS_CPU_A05p.out_new());
  /*_p10.DOSO*/ wire DOSO_A00p = not1(DYTE_A00n);
  /*_p10.DUPA*/ wire DUPA_A01p = not1(AFOB_A01n);

  /*_p10.AWET*/ wire AWET_ADDR_XX2Xn = or4(abus_new.BUS_CPU_A07p.out_new(), abus_new.BUS_CPU_A06p.out_new(), BOXY_A05n, abus_new.BUS_CPU_A04p.out_new());
  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn = not1(abus_new.SYKE_ADDR_HIp_new());
  /*_p10.BEZY*/ wire BEZY_ADDR_FF2Xn = or2(AWET_ADDR_XX2Xn, BAKO_ADDR_FFXXn);
  /*_p10.CONA*/ wire CONA_ADDR_FF2Xp = not1(BEZY_ADDR_FF2Xn);

  /*#p10.DONA*/ wire DONA_ADDR_0000n = nand4(DYTE_A00n, AFOB_A01n, ABUB_A02n, ACOL_A03n);
  /*#p10.DEWA*/ wire DEWA_ADDR_0001n = nand4(DOSO_A00p, AFOB_A01n, ABUB_A02n, ACOL_A03n);
  /*#p10.DOFA*/ wire DOFA_ADDR_0010p =  and4(ACOL_A03n, ABUB_A02n, DUPA_A01p, DYTE_A00n);
  /*#p10.DUFE*/ wire DUFE_ADDR_0011n = nand4(DOSO_A00p, DUPA_A01p, ABUB_A02n, ACOL_A03n);

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(reg_new.cpu_signals.TEDO_CPU_RDp.qp_new());
  /*_p10.BAFU*/ wire BAFU_CPU_WRn = not1(reg_new.cpu_signals.TAPU_CPU_WRp.qp_new());
  /*_p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn);

  /*#p01.HAMA*/ wire HAMA_CLK_512K = not1(reg_new.spu.JESO_CLK_512K.qp_new());
  /*_p01.BURE*/ wire BURE_CLK_512 = not1(reg_new.spu.BARA_CLK_512.qp_new());
  /*#p01.FYNE*/ wire FYNE_CLK_512 = not1(BURE_CLK_512);
  /*_p01.GALE*/ wire GALE_CLK_512 = mux2p(reg_new.spu.FERO_NR52_DBG_APUp.qp_new(), HAMA_CLK_512K, FYNE_CLK_512);
  /*_p01.GEXY*/ wire GEXY_CLK_512 = not1(GALE_CLK_512);
  /*_p01.HORU*/ wire HORU_CLK_512 = not1(GEXY_CLK_512);

  /*_p01.CULO*/ wire CULO_CLK_256 = not1(reg_new.spu.CARU_CLK_256.qp_new());
  /*_p01.BEZE*/ wire BEZE_CLK_256 = mux2p(reg_new.spu.FERO_NR52_DBG_APUp.qp_new(), HAMA_CLK_512K, CULO_CLK_256);
  /*_p01.COFU*/ wire COFU_CLK_256 = not1(BEZE_CLK_256);
  /*_p01.BUFY*/ wire BUFY_CLK_256 = not1(COFU_CLK_256);

  /*_p01.APEF*/ wire APEF_CLK_128 = not1(reg_new.spu.BYLU_CLK_128.qp_new());
  /*_p01.BULE*/ wire BULE_CLK_128 = mux2p(reg_new.spu.FERO_NR52_DBG_APUp.qp_new(), HAMA_CLK_512K, APEF_CLK_128);
  /*_p01.BARU*/ wire BARU_CLK_128 = not1(BULE_CLK_128);
  /*_p01.BYFE*/ wire BYFE_CLK_128 = not1(BARU_CLK_128);

  {
    /*#p10.COVO*/ wire COVO_ADDR_FF21p = nor2(DEWA_ADDR_0001n, BEZY_ADDR_FF2Xn);
    /*#p19.DACO*/ wire DACO_FF21_WRp = and2(BOGY_CPU_WRp, COVO_ADDR_FF21p);
    /*#p19.GOKO*/ wire GOKO_FF21_WRp = and2(COVO_ADDR_FF21p, BOGY_CPU_WRp);
    /*#p19.DYKE*/ wire DYKE_FF21_WRn = not1(DACO_FF21_WRp);
    /*#p19.FUPA*/ wire FUPA_FF21_WRn = not1(GOKO_FF21_WRp);
    
    /*#p19.FEXO*/ wire FEXO_APU_RSTn_new = not1(KEBA_APU_RSTp);

    /*_p19.EMOK*/ reg_new.ch4.EMOK_NR42_ENV_TIMER0p.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*_p19.ETYJ*/ reg_new.ch4.ETYJ_NR42_ENV_TIMER1p.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*_p19.EZYK*/ reg_new.ch4.EZYK_NR42_ENV_TIMER2p.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*_p19.GEKY*/ reg_new.ch4.GEKY_NR42_ENV_DIRp   .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*_p19.GARU*/ reg_new.ch4.GARU_NR42_ENV0p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*_p19.GOKY*/ reg_new.ch4.GOKY_NR42_ENV1p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*_p19.GOZO*/ reg_new.ch4.GOZO_NR42_ENV2p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*_p19.GEDU*/ reg_new.ch4.GEDU_NR42_ENV3p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
  }

  {
    /*#p10.EKEZ*/ wire EKEZ_ADDR_FF22p = and2(CONA_ADDR_FF2Xp, DOFA_ADDR_0010p);
    /*#p19.HUMO*/ wire HUMO_FF22_WRp =  and2(BOGY_CPU_WRp, EKEZ_ADDR_FF22p);
    /*#p19.GETU*/ wire GETU_FF22_WRp =  and2(BOGY_CPU_WRp, EKEZ_ADDR_FF22p);
    /*#p19.HOVA*/ wire HOVA_FF22_WRn = not1(HUMO_FF22_WRp);
    /*#p19.HOSO*/ wire HOSO_FF22_WRn = nand2(EKEZ_ADDR_FF22p, BOGY_CPU_WRp);
    /*#p19.EFUG*/ wire EFUG_FF22_WRn = not1(GETU_FF22_WRp);

    /*_p09.KAME*/ wire KAME_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p19.HYNE*/ wire HYNE_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p09.DAPA*/ wire DAPA_APU_RSTn_new = not1(KEBA_APU_RSTp);

    /*#p19.JARE*/ reg_new.ch4.JARE_NR43_DIV0p .dff9(HOVA_FF22_WRn, KAME_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p19.JERO*/ reg_new.ch4.JERO_NR43_DIV1p .dff9(HOVA_FF22_WRn, KAME_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p19.JAKY*/ reg_new.ch4.JAKY_NR43_DIV2p .dff9(HOVA_FF22_WRn, KAME_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*#p19.JAMY*/ reg_new.ch4.JAMY_NR43_MODEp .dff9(HOSO_FF22_WRn, HYNE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*#p19.FETA*/ reg_new.ch4.FETA_NR43_FREQ0p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*#p19.FYTO*/ reg_new.ch4.FYTO_NR43_FREQ1p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*_p19.GOGO*/ reg_new.ch4.GOGO_NR43_FREQ2p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*_p19.GAFO*/ reg_new.ch4.GAFO_NR43_FREQ3p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
  }


  /*#p20.GASO*/ wire GASO_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p20.GYSU*/ reg_new.ch4.GYSU_CH4_TRIG.dff17(DOVA_ABCDxxxx, GASO_APU_RSTn_new, reg_old.ch4.HOGA_NR44_TRIGp.qp_old());

  /*#p20.FEBY*/ wire FEBY_APU_RSTn_new = not1(KEBA_APU_RSTp);

  /*#p20.GONE*/ reg_new.ch4.GONE_CH4_TRIGp.dff17_clk(HAMA_CLK_512K, reg_old.ch4.HAZO_CH4_TRIGn.qn_old());
  /*#p20.GORA*/ reg_new.ch4.GORA_CH4_TRIGp.dff17(HAMA_CLK_512K, FEBY_APU_RSTn_new, reg_old.ch4.GONE_CH4_TRIGp.qp_old());
  /*#p20.GATY*/ reg_new.ch4.GATY_CH4_TRIGp.dff17(HAMA_CLK_512K, FEBY_APU_RSTn_new, reg_old.ch4.GORA_CH4_TRIGp.qp_old());

  /*#p20.FALE*/ wire FALE_RESET_LATCHn = nor2(KEBA_APU_RSTp, reg_new.ch4.GORA_CH4_TRIGp.qp_new());
  /*#p20.GONE*/ reg_new.ch4.GONE_CH4_TRIGp.dff17_rst(FALE_RESET_LATCHn);

  /*#p20.HELU*/ wire HELU_RESET_LATCHp  = not1(FALE_RESET_LATCHn);
  /*#p20.HAZO*/ reg_new.ch4.HAZO_CH4_TRIGn.nor_latch(HELU_RESET_LATCHp, reg_new.ch4.GYSU_CH4_TRIG.qp_new());


  // This goes to all the CHN_LENENp registers...why?
  /*#p10.CUGE*/ wire CUGE_ADDR_FF23p = nor2(DUFE_ADDR_0011n, BEZY_ADDR_FF2Xn);
  /*_p16.ANUJ*/ wire ANUJ_CPU_WR_WEIRD = and2(reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.qp_new(), BOGY_CPU_WRp);
  /*#p19.DULU*/ wire DULU_FF23_WRn = nand2(ANUJ_CPU_WR_WEIRD, CUGE_ADDR_FF23p);
  /*#p19.FOXE*/ wire FOXE_FF23_WRn = nand2(BOGY_CPU_WRp, CUGE_ADDR_FF23p);
  /*#p20.GUZY*/ wire GUZY_NR44_TRIG_RST = nor2(KEBA_APU_RSTp, reg_new.ch4.GYSU_CH4_TRIG.qp_new());

  /*#p19.CABE*/ wire CABE_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p19.CUNY*/ reg_new.ch4.CUNY_NR44_LEN_ENp.dff9(DULU_FF23_WRn, CABE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
  /*#p19.HOGA*/ reg_new.ch4.HOGA_NR44_TRIGp.dff9(FOXE_FF23_WRn, GUZY_NR44_TRIG_RST, reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());

  /*#p20.GUNY*/ wire GUNY_FREQ_GATE_RSTn_new = nor2(KEBA_APU_RSTp, reg_new.ch4.GONE_CH4_TRIGp.qp_new());
  
  /*#p20.HYNO*/ wire HYNO_DIV_MAX_old = and3(reg_old.ch4.JYFU_DIV2.qp_old(), reg_old.ch4.JYRE_DIV1.qp_old(), reg_old.ch4.JYCO_DIV0.qp_old()); // Die may be wrong, this has to be an AND
  
  /*_p01.BAVU*/ wire BAVU_CLK_1M = not1(reg_new.spu.AVOK_xBCDExxx.qp_new());
  /*#p20.GYBA*/ wire GYBA_CLK_1M = not1(BAVU_CLK_1M);
  /*#p20.GARY*/ reg_new.ch4.GARY_FREQ_GATEp.dff17(GYBA_CLK_1M, GUNY_FREQ_GATE_RSTn_new, HYNO_DIV_MAX_old);




  {

    /*#p20.CARY*/ wire CARY_FREQ_CLK = and2(BAVU_CLK_1M, reg_new.ch4.GARY_FREQ_GATEp.qp_new());
    /*#p09.DAPA*/ wire DAPA_APU_RSTn_new = not1(KEBA_APU_RSTp);
    
    /*#p20.CEXO*/ reg_new.ch4.CEXO_FREQ_00.dff17(CARY_FREQ_CLK,                     DAPA_APU_RSTn_new, reg_old.ch4.CEXO_FREQ_00.qn_old());
    /*_p20.DEKO*/ reg_new.ch4.DEKO_FREQ_01.dff17(reg_new.ch4.CEXO_FREQ_00.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DEKO_FREQ_01.qn_old());
    /*_p20.EZEF*/ reg_new.ch4.EZEF_FREQ_02.dff17(reg_new.ch4.DEKO_FREQ_01.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.EZEF_FREQ_02.qn_old());
    /*_p20.EPOR*/ reg_new.ch4.EPOR_FREQ_03.dff17(reg_new.ch4.EZEF_FREQ_02.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.EPOR_FREQ_03.qn_old());
    /*_p20.DURE*/ reg_new.ch4.DURE_FREQ_04.dff17(reg_new.ch4.EPOR_FREQ_03.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DURE_FREQ_04.qn_old());
    /*_p20.DALE*/ reg_new.ch4.DALE_FREQ_05.dff17(reg_new.ch4.DURE_FREQ_04.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DALE_FREQ_05.qn_old());
    /*_p20.DOKE*/ reg_new.ch4.DOKE_FREQ_06.dff17(reg_new.ch4.DALE_FREQ_05.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DOKE_FREQ_06.qn_old());
    /*_p20.DEMO*/ reg_new.ch4.DEMO_FREQ_07.dff17(reg_new.ch4.DOKE_FREQ_06.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DEMO_FREQ_07.qn_old());
    /*_p20.DOSE*/ reg_new.ch4.DOSE_FREQ_08.dff17(reg_new.ch4.DEMO_FREQ_07.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DOSE_FREQ_08.qn_old());
    /*_p20.DETE*/ reg_new.ch4.DETE_FREQ_09.dff17(reg_new.ch4.DOSE_FREQ_08.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DETE_FREQ_09.qn_old());
    /*_p20.ERUT*/ reg_new.ch4.ERUT_FREQ_10.dff17(reg_new.ch4.DETE_FREQ_09.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.ERUT_FREQ_10.qn_old());
    /*_p20.DOTA*/ reg_new.ch4.DOTA_FREQ_11.dff17(reg_new.ch4.ERUT_FREQ_10.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DOTA_FREQ_11.qn_old());
    /*_p20.DERE*/ reg_new.ch4.DERE_FREQ_12.dff17(reg_new.ch4.DOTA_FREQ_11.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.DERE_FREQ_12.qn_old());
    /*_p20.ESEP*/ reg_new.ch4.ESEP_FREQ_13.dff17(reg_new.ch4.DERE_FREQ_12.qn_new(), DAPA_APU_RSTn_new, reg_old.ch4.ESEP_FREQ_13.qn_old());
  }

  // 14-to-1 mux to select LFSR clock

  // some of these are wrong... let's see if inverting the freqNp bits works

  // emof = nor(gogo_qn, elar)

  /*#p20.EMOF*/ wire EMOF_LFSR_CLK_MUX_7 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qn_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qn_new(), reg_new.ch4.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ELAR*/ wire ELAR_LFSR_CLK_MUX_6 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qn_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qn_new(), reg_new.ch4.FETA_NR43_FREQ0p.qp_new());
  /*#p20.DUDU*/ wire DUDU_LFSR_CLK_MUX_5 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qn_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qp_new(), reg_new.ch4.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ETAT*/ wire ETAT_LFSR_CLK_MUX_4 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qn_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qp_new(), reg_new.ch4.FETA_NR43_FREQ0p.qp_new());
  /*#p20.FURA*/ wire FURA_LFSR_CLK_MUX_3 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qp_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qn_new(), reg_new.ch4.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ETAR*/ wire ETAR_LFSR_CLK_MUX_2 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qp_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qn_new(), reg_new.ch4.FETA_NR43_FREQ0p.qp_new());
  /*#p20.EVER*/ wire EVER_LFSR_CLK_MUX_1 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qp_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qp_new(), reg_new.ch4.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ETOV*/ wire ETOV_LFSR_CLK_MUX_0 = nor3(reg_new.ch4.GOGO_NR43_FREQ2p.qp_new(), reg_new.ch4.FYTO_NR43_FREQ1p.qp_new(), reg_new.ch4.FETA_NR43_FREQ0p.qp_new());

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

  /*#p20.FEME*/ wire FEME_LFSR_CLKp_new = mux2p(reg_new.ch4.GAFO_NR43_FREQ3p.qp_new(), ETYR_LFSR_CLK_MUX_A, ERYF_LFSR_CLK_MUX_D);




  {
    /*#p10.DANU*/ wire DANU_ADDR_FF20p = nor2(DONA_ADDR_0000n, BEZY_ADDR_FF2Xn);
    /*#p19.CAZE*/ wire CAZE_FF20_WRn = nand2(BOGY_CPU_WRp, DANU_ADDR_FF20p);
    /*#p19.FURU*/ wire FURU_FF20_WRa = not1(CAZE_FF20_WRn);
    /*#p19.DOTU*/ wire DOTU_FF20_WRb = not1(CAZE_FF20_WRn);
    /*#p19.EPEK*/ wire EPEK_FF20_WRc = not1(CAZE_FF20_WRn);

    // using FUGO_old to break the loop

    /*#p19.DODA*/ wire DODA_LEN_CLKn = nor3(reg_old.ch4.FUGO_CH4_LEN_DONEp.qp_old(), BUFY_CLK_256, reg_new.ch4.CUNY_NR44_LEN_ENp.qn_new());
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa = not1(DODA_LEN_CLKn);

    /*#p19.DANO*/ reg_new.ch4.DANO_NR41_LEN0p.dff20(CUWA_LEN_CLKa,                        DOTU_FF20_WRb, reg_new.cpu_dbus.BUS_CPU_D00p.qp_new());
    /*#p19.FAVY*/ reg_new.ch4.FAVY_NR41_LEN1p.dff20(reg_new.ch4.DANO_NR41_LEN0p.qp_new(), DOTU_FF20_WRb, reg_new.cpu_dbus.BUS_CPU_D01p.qp_new());
    /*#p19.DENA*/ reg_new.ch4.DENA_NR41_LEN2p.dff20(reg_new.ch4.FAVY_NR41_LEN1p.qp_new(), DOTU_FF20_WRb, reg_new.cpu_dbus.BUS_CPU_D02p.qp_new());
    /*#p19.CEDO*/ reg_new.ch4.CEDO_NR41_LEN3p.dff20(reg_new.ch4.DENA_NR41_LEN2p.qp_new(), DOTU_FF20_WRb, reg_new.cpu_dbus.BUS_CPU_D03p.qp_new());

    /*#p19.DOPU*/ wire DOPU_NR41_LEN3p = not1(reg_new.ch4.CEDO_NR41_LEN3p.qn_new());
    /*#p19.FYLO*/ reg_new.ch4.FYLO_NR41_LEN4p.dff20(DOPU_NR41_LEN3p,                      EPEK_FF20_WRc, reg_new.cpu_dbus.BUS_CPU_D04p.qp_new());
    /*#p19.EDOP*/ reg_new.ch4.EDOP_NR41_LEN5p.dff20(reg_new.ch4.FYLO_NR41_LEN4p.qp_new(), EPEK_FF20_WRc, reg_new.cpu_dbus.BUS_CPU_D05p.qp_new());

    /*#p19.GAPY*/ wire GAPY_LEN_DONE_RSTn = nor3(FURU_FF20_WRa, KEBA_APU_RSTp, reg_new.ch4.GONE_CH4_TRIGp.qp_new());
    /*#p19.FUGO*/ reg_new.ch4.FUGO_CH4_LEN_DONEp.dff17(reg_new.ch4.EDOP_NR41_LEN5p.qn_new(), GAPY_LEN_DONE_RSTn, reg_old.ch4.FUGO_CH4_LEN_DONEp.qn_old());

    /*#p19.DODA*/ wire DODA_LEN_CLKn2 = nor3(reg_new.ch4.FUGO_CH4_LEN_DONEp.qp_new(), BUFY_CLK_256, reg_new.ch4.CUNY_NR44_LEN_ENp.qn_new());
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa2 = not1(DODA_LEN_CLKn2);

    /*#p19.DANO*/ reg_new.ch4.DANO_NR41_LEN0p.dff20_any(CUWA_LEN_CLKa2,                        DOTU_FF20_WRb, reg_new.cpu_dbus.BUS_CPU_D00p.qp_new());
  }





  /*#p20.GEVY*/ wire GEVY_CH4_AMP_ENn = nor5(reg_new.ch4.GEKY_NR42_ENV_DIRp.qp_new(),
                                             reg_new.ch4.GARU_NR42_ENV0p.qp_new(),
                                             reg_new.ch4.GOKY_NR42_ENV1p.qp_new(),
                                             reg_new.ch4.GOZO_NR42_ENV2p.qp_new(),
                                             reg_new.ch4.GEDU_NR42_ENV3p.qp_new());


  {
    /*#p20.EFOT*/ wire EFOT_CH4_STOPp = and2(reg_new.ch4.CUNY_NR44_LEN_ENp.qp_new(), reg_new.ch4.FUGO_CH4_LEN_DONEp.qp_new());
    /*#p20.FEGY*/ wire FEGY_CH4_STOPp = or3(KEBA_APU_RSTp, EFOT_CH4_STOPp, GEVY_CH4_AMP_ENn);
    /*#p20.GENA*/ reg_new.ch4.GENA_CH4_ACTIVEp.nor_latch(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), FEGY_CH4_STOPp);
  }

  {
    /*#p20.HERY*/ wire HERY_DIV_GATE1n = nor2(GEVY_CH4_AMP_ENn, KEBA_APU_RSTp);
    /*#p20.HAPU*/ wire HAPU_CH4_TRIGn = not1(reg_new.ch4.GATY_CH4_TRIGp.qp_new());
    /*#p20.JERY*/ reg_new.ch4.JERY_DIV_GATE.nand_latch(HERY_DIV_GATE1n, HAPU_CH4_TRIGn);
  }

  {
    /*#p20.KYKU*/ wire KYKU_DIV_CLKb  = or2(reg_new.ch4.JERY_DIV_GATE.qp_new(), reg_new.spu.JESO_CLK_512K.qp_new());
    /*#p20.KONY*/ wire KONY_DIV_CLKn  = not1(KYKU_DIV_CLKb);
    /*#p20.KANU*/ wire KANU_DIV_CLKa  = not1(KONY_DIV_CLKn);

    /*#p20.GOFU*/ wire GOFU_DIV_LOADn = nor2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.GARY_FREQ_GATEp.qp_new());
    /*#p20.HUCE*/ wire HUCE_DIV_LOADp  = not1(GOFU_DIV_LOADn);

    /*#p20.JYCO*/ reg_new.ch4.JYCO_DIV0.dff20(KANU_DIV_CLKa,                  HUCE_DIV_LOADp, reg_new.ch4.JARE_NR43_DIV0p.qn_new());
    /*#p20.JYRE*/ reg_new.ch4.JYRE_DIV1.dff20(reg_new.ch4.JYCO_DIV0.qp_new(), HUCE_DIV_LOADp, reg_new.ch4.JERO_NR43_DIV1p.qn_new());
    /*_p20.JYFU*/ reg_new.ch4.JYFU_DIV2.dff20(reg_new.ch4.JYRE_DIV1.qp_new(), HUCE_DIV_LOADp, reg_new.ch4.JAKY_NR43_DIV2p.qn_new());
  }


  //----------
  // The actual LFSR



  {
    /*#p20.JYJA*/ wire JYJA_LFSR_CLKn_new = not1(FEME_LFSR_CLKp_new);
    /*#p20.GUFA*/ wire GUFA_LFSR_CLKn_new = not1(FEME_LFSR_CLKp_new);
    /*#p20.GYVE*/ wire GYVE_LFSR_CLKp_new = not1(GUFA_LFSR_CLKn_new);
    /*#p20.KARA*/ wire KARA_LFSR_CLKn_new = not1(GYVE_LFSR_CLKp_new);
    /*#p20.KOPA*/ wire KOPA_LFSR_CLKp_new = not1(KARA_LFSR_CLKn_new);

    /*#p20.GEPO*/ wire GEPO_LFSR_RSTa = or2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp);
    /*#p20.GOGE*/ wire GOGE_LFSR_RSTn = not1(GEPO_LFSR_RSTa);

    // naze is xnor2
    /*#p20.HURA*/ wire HURA_LFSR_IN_old = xnor2(reg_old.ch4.HEZU_LFSR_15.qp_old(), reg_old.ch4.HYRO_LFSR_14.qp_old());

    /*#p20.JOTO*/ reg_new.ch4.JOTO_LFSR_00.dff17(JYJA_LFSR_CLKn_new, GOGE_LFSR_RSTn, HURA_LFSR_IN_old);
    /*#p20.KOMU*/ reg_new.ch4.KOMU_LFSR_01.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.JOTO_LFSR_00.qp_old());
    /*#p20.KETU*/ reg_new.ch4.KETU_LFSR_02.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KOMU_LFSR_01.qp_old());
    /*#p20.KUTA*/ reg_new.ch4.KUTA_LFSR_03.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KETU_LFSR_02.qp_old());
    /*#p20.KUZY*/ reg_new.ch4.KUZY_LFSR_04.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KUTA_LFSR_03.qp_old());
    /*#p20.KYWY*/ reg_new.ch4.KYWY_LFSR_05.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KUZY_LFSR_04.qp_old());
    /*#p20.JAJU*/ reg_new.ch4.JAJU_LFSR_06.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.KYWY_LFSR_05.qp_old());
    /*#p20.HAPE*/ reg_new.ch4.HAPE_LFSR_07.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.JAJU_LFSR_06.qp_old());
    /*#p20.JUXE*/ reg_new.ch4.JUXE_LFSR_08.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, reg_old.ch4.HAPE_LFSR_07.qp_old());

    /*#p20.KAVU*/ wire KAVU_LFSR_FB_old = amux2(reg_old.ch4.JOTO_LFSR_00.qp_old(), reg_old.ch4.JAMY_NR43_MODEp.qp_old(), reg_old.ch4.JAMY_NR43_MODEp.qn_old(), reg_old.ch4.JUXE_LFSR_08.qp_old());
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
    /*#p20.FOSY*/ reg_new.ch4.FOSY_ENV_CLKp.dff17_clk(HORU_CLK_512, EJEX_ENV_TIMER_MAX_old);

    /*#p20.GEXE*/ wire GEXE_ENV_PULSEn_new  = not1(reg_new.ch4.FOSY_ENV_CLKp.qp_new());
    /*#p20.HURY*/ wire HURY_ENV_PULSE_RSTp_new = nor2(HORU_CLK_512, GEXE_ENV_PULSEn_new);
    /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new = nor3(reg_new.ch4.EMOK_NR42_ENV_TIMER0p.qp_new(), reg_new.ch4.ETYJ_NR42_ENV_TIMER1p.qp_new(), reg_new.ch4.EZYK_NR42_ENV_TIMER2p.qp_new());
    /*#p20.GOPA*/ wire GOPA_ENV_PULSE_RSTn_new = nor4(HURY_ENV_PULSE_RSTp_new, FOWA_ENV_OFFp_new, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp);
    /*#p20.FOSY*/ reg_new.ch4.FOSY_ENV_CLKp.dff17_rst(GOPA_ENV_PULSE_RSTn_new);
  }

  {
    /*#p20.ALOP*/ wire ALOP_CLK_128 = not1(BYFE_CLK_128);
    /*#p20.BOKY*/ wire BOKY_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p20.ABEL*/ reg_new.ch4.ABEL_CLK_64.dff17(ALOP_CLK_128, BOKY_APU_RSTn_new, reg_old.ch4.ABEL_CLK_64.qn_old());
    /*#p20.BAWA*/ wire BAWA_CLK_64_new  = not1(reg_new.ch4.ABEL_CLK_64.qp_new());
    /*#p20.BUXO*/ wire BUXO_CLK_64_new  = not1(BAWA_CLK_64_new);

    /*#p20.ENEC*/ wire ENEC_ENV_TIMER_LOADn_new = nor2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.FOSY_ENV_CLKp.qp_new());
    /*#p20.DAPY*/ wire DAPY_ENV_TIMER_LOADp_new  = not1(ENEC_ENV_TIMER_LOADn_new);

    /*#p20.CUNA*/ reg_new.ch4.CUNA_ENV_DELAY0n.dff20(BUXO_CLK_64_new,                       DAPY_ENV_TIMER_LOADp_new, reg_new.ch4.EMOK_NR42_ENV_TIMER0p.qn_new());
    /*#p20.COFE*/ reg_new.ch4.COFE_ENV_DELAY1n.dff20(reg_new.ch4.CUNA_ENV_DELAY0n.qp_new(), DAPY_ENV_TIMER_LOADp_new, reg_new.ch4.ETYJ_NR42_ENV_TIMER1p.qn_new());
    /*#p20.DOGO*/ reg_new.ch4.DOGO_ENV_DELAY2n.dff20(reg_new.ch4.COFE_ENV_DELAY1n.qp_new(), DAPY_ENV_TIMER_LOADp_new, reg_new.ch4.EZYK_NR42_ENV_TIMER2p.qn_new());
  }


  {
    /*#p20.EMET*/ wire EMET_ENV_STOP_RSTn = nor2(reg_new.ch4.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp);
    
    /*#p20.DARO*/ wire DARO_ENV_BOTp_old  = nor5(reg_old.ch4.GEKY_NR42_ENV_DIRp.qp_old(), reg_old.ch4.FEKO_CH4_VOL0.qp_old(), reg_old.ch4.FATY_CH4_VOL1.qp_old(), reg_old.ch4.FERU_CH4_VOL2.qp_old(), reg_old.ch4.FYRO_CH4_VOL3.qp_old());
    /*#p20.CUTY*/ wire CUTY_ENV_TOPn_old = nand5(reg_old.ch4.GEKY_NR42_ENV_DIRp.qp_old(), reg_old.ch4.FEKO_CH4_VOL0.qp_old(), reg_old.ch4.FATY_CH4_VOL1.qp_old(), reg_old.ch4.FERU_CH4_VOL2.qp_old(), reg_old.ch4.FYRO_CH4_VOL3.qp_old());
    /*#p20.DUBO*/ wire DUBO_ENV_TOPp_old = not1(CUTY_ENV_TOPn_old);
    /*#p20.EVUR*/ wire EVUR_ENV_MAXp_old  = or2(DARO_ENV_BOTp_old, DUBO_ENV_TOPp_old);

    /*#p20.FYNO*/ reg_new.ch4.FYNO_ENV_MAXp.dff17(reg_new.ch4.FOSY_ENV_CLKp.qp_new(), EMET_ENV_STOP_RSTn, EVUR_ENV_MAXp_old);
  }





  {
    /*#p20.ENUR*/ wire ENUR_ENV_STARTp = or2(KEBA_APU_RSTp, reg_new.ch4.GONE_CH4_TRIGp.qp_new());
    /*#p20.EROX*/ reg_new.ch4.EROX_ENV_RUNNINGn.nor_latch(reg_new.ch4.FYNO_ENV_MAXp.qp_new(), ENUR_ENV_STARTp);

    /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new = nor3(reg_new.ch4.EMOK_NR42_ENV_TIMER0p.qp_new(), reg_new.ch4.ETYJ_NR42_ENV_TIMER1p.qp_new(), reg_new.ch4.EZYK_NR42_ENV_TIMER2p.qp_new());
    /*#p20.FELO*/ wire FELO_ENV_CLK  = or3(reg_new.ch4.FOSY_ENV_CLKp.qp_new(), FOWA_ENV_OFFp_new, reg_new.ch4.EROX_ENV_RUNNINGn.qp_new());
    /*#p20.FOLE*/ wire FOLE_VOL_CLK0n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIRp.qp_new(), FELO_ENV_CLK,               FELO_ENV_CLK,               reg_new.ch4.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FEKO*/ reg_new.ch4.FEKO_CH4_VOL0.dff20(FOLE_VOL_CLK0n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.GARU_NR42_ENV0p.qp_new());

    /*#p20.ETEF*/ wire ETEF_VOL_CLK1n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIRp.qp_new(), reg_new.ch4.FEKO_CH4_VOL0.qp_new(), reg_new.ch4.FEKO_CH4_VOL0.qn_new(), reg_new.ch4.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FATY*/ reg_new.ch4.FATY_CH4_VOL1.dff20(ETEF_VOL_CLK1n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.GOKY_NR42_ENV1p.qp_new());

    /*#p20.EDYF*/ wire EDYF_VOL_CLK2n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIRp.qp_new(), reg_new.ch4.FATY_CH4_VOL1.qp_new(), reg_new.ch4.FATY_CH4_VOL1.qn_new(), reg_new.ch4.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FERU*/ reg_new.ch4.FERU_CH4_VOL2.dff20(EDYF_VOL_CLK2n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.GOZO_NR42_ENV2p.qp_new());

    /*#p20.ELAF*/ wire ELAF_VOL_CLK3n = amux2(reg_new.ch4.GEKY_NR42_ENV_DIRp.qp_new(), reg_new.ch4.FERU_CH4_VOL2.qp_new(), reg_new.ch4.FERU_CH4_VOL2.qn_new(), reg_new.ch4.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FYRO*/ reg_new.ch4.FYRO_CH4_VOL3.dff20(ELAF_VOL_CLK3n, reg_new.ch4.GONE_CH4_TRIGp.qp_new(), reg_new.ch4.GEDU_NR42_ENV3p.qp_new());
  }


#if 0
  /*#p20.COSA*/ wire COSA_CPU_RDp = not1(reg_new.cpu_signals.AGUZ_CPU_RDn());
  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  /*#p20.DYRY*/ wire DYRY_DBG = and2(reg_new.ch4.CUNY_NR44_LEN_ENp.qn_new(), EDEK_NR52_DBG_APUp);
  /*_p20.COMO*/ wire COMO_DBG = and2(DYRY_DBG, COSA_CPU_RDp);
  /*_p20.BAGU*/ wire BAGU_DBG = nand2(CUGE_ADDR_FF23p, COMO_DBG);
  /*_p20.BEFA*/ wire BEFA_DBG = not1(CARY_FREQ_CLK);
  /*_p20.ATEL*/ triwire ATEL = tri6_nn(BAGU_DBG, BEFA_DBG); // goes to data bus D0
#endif

  {
    /*#p10.COVO*/ wire COVO_ADDR_FF21p = nor2(DEWA_ADDR_0001n, BEZY_ADDR_FF2Xn);
    /*#p19.BOFY*/ wire BOFY_FF21n    = not1(COVO_ADDR_FF21p);
    /*#p19.GONY*/ wire GONY_FF21n    = not1(COVO_ADDR_FF21p);
    /*#p19.BOXE*/ wire BOXE_FF21_RDn = or2(BOFY_FF21n, AGUZ_CPU_RDn);
    /*#p19.HASU*/ wire HASU_FF21_RDn = or2(GONY_FF21n, AGUZ_CPU_RDn);

    /*#p19.DEMY*/ triwire DEMY = tri6_nn(BOXE_FF21_RDn, reg_new.ch4.EMOK_NR42_ENV_TIMER0p.qn_new());
    /*#p19.COCE*/ triwire COCE = tri6_nn(BOXE_FF21_RDn, reg_new.ch4.ETYJ_NR42_ENV_TIMER1p.qn_new());
    /*#p19.CUZU*/ triwire CUZU = tri6_nn(BOXE_FF21_RDn, reg_new.ch4.EZYK_NR42_ENV_TIMER2p.qn_new());
    /*#p19.GOME*/ triwire GOME = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GEKY_NR42_ENV_DIRp   .qn_new());
    /*#p19.HEDA*/ triwire HEDA = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GARU_NR42_ENV0p      .qn_new());
    /*#p19.GODU*/ triwire GODU = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GOKY_NR42_ENV1p      .qn_new());
    /*#p19.HOGE*/ triwire HOGE = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GOZO_NR42_ENV2p      .qn_new());
    /*#p19.HACU*/ triwire HACU = tri6_nn(HASU_FF21_RDn, reg_new.ch4.GEDU_NR42_ENV3p      .qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(DEMY);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(COCE);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(CUZU);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(GOME);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(HEDA);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(GODU);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HOGE);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(HACU);
  }

  {
    /*#p19.KAGE*/ wire KAGE_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p10.EKEZ*/ wire EKEZ_ADDR_FF22p = and2(CONA_ADDR_FF2Xp, DOFA_ADDR_0010p);

    /*#p19.KOKU*/ wire KOKU_ADDR_FF22n = not1(EKEZ_ADDR_FF22p);
    /*#p19.GUGO*/ wire GUGO_ADDR_FF22n = not1(EKEZ_ADDR_FF22p);
    /*#p19.KEKA*/ wire KEKA_FF22_RDn =   or2(KOKU_ADDR_FF22n, AGUZ_CPU_RDn);
    /*#p19.JORA*/ wire JORA_FF22_RDn = nand2(KAGE_CPU_RDp, EKEZ_ADDR_FF22p);
    /*#p19.HEZE*/ wire HEZE_FF22_RDn =   or2(GUGO_ADDR_FF22n, AGUZ_CPU_RDn);

    /*#p19.KAMO*/ triwire KAMO = tri6_nn(KEKA_FF22_RDn, reg_new.ch4.JARE_NR43_DIV0p .qn_new());
    /*#p19.KAKU*/ triwire KAKU = tri6_nn(KEKA_FF22_RDn, reg_new.ch4.JERO_NR43_DIV1p .qn_new());
    /*_p19.KYRO*/ triwire KYRO = tri6_nn(KEKA_FF22_RDn, reg_new.ch4.JAKY_NR43_DIV2p .qn_new());
    /*_p19.KETA*/ triwire KETA = tri6_nn(JORA_FF22_RDn, reg_new.ch4.JAMY_NR43_MODEp .qn_new());
    /*_p19.GEDA*/ triwire GEDA = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.FETA_NR43_FREQ0p.qn_new());
    /*_p19.GYPE*/ triwire GYPE = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.FYTO_NR43_FREQ1p.qn_new());
    /*_p19.GAKA*/ triwire GAKA = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.GOGO_NR43_FREQ2p.qn_new());
    /*_p19.HAPY*/ triwire HAPY = tri6_nn(HEZE_FF22_RDn, reg_new.ch4.GAFO_NR43_FREQ3p.qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(KAMO);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(KAKU);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(KYRO);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(KETA);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(GEDA);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(GYPE);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GAKA);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(HAPY);
  }

  {
    /*#p19.BYLO*/ wire BYLO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p19.BARE*/ wire BARE_FF23_RDn = nand2(CUGE_ADDR_FF23p, BYLO_CPU_RDp);
    /*#p19.CURY*/ triwire CURY = tri6_nn(BARE_FF23_RDn, reg_new.ch4.CUNY_NR44_LEN_ENp.qn_new());
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(CURY);
  }

}









































































void tick_ch4_fast(
  const GateBoyCpuDBus& cpu_dbus_old,
  const GateBoySPU& spu_old,
  const SpuChannel4& ch4_old,
  const GateBoyWaveBus& wave_dbus_old,

  GateBoyCpuABus& cpu_abus_new,
  GateBoyCpuDBus& cpu_dbus_new,
  GateBoySPU& spu_new,
  SpuChannel4& ch4_new,
  GateBoyWaveBus& wave_dbus_new,

  wire AFER_SYS_RSTp,
  wire ASOL_POR_DONEn,
  wire SIG_CPU_CLKREQ,
  wire AVET_AxCxExGx,
  wire AFUR_ABCDxxxx_qn,
  wire APUK_xxCDEFxx_qn,
  wire ALEF_xBCDExxx,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,
  wire SIG_IN_CPU_DBUS_FREE,

  uint8_t* wave_ram
)
{
  /*_p01.ABOL*/ wire ABOL_CLKREQn = not1(SIG_CPU_CLKREQ);
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(AFUR_ABCDxxxx_qn);
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx = not1(BUDE_xxxxEFGH);

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp = or2(AFER_SYS_RSTp, ASOL_POR_DONEn);
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*_p09.HAPO*/ wire HAPO_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*_p09.JYRO*/ wire JYRO_APU_RSTp = or2(HAPO_SYS_RSTp, spu_new.HADA_NR52_ALL_SOUND_ON.qn_new());
  /*_p09.KUBY*/ wire KUBY_APU_RSTn = not1(JYRO_APU_RSTp);
  /*_p09.KEBA*/ wire KEBA_APU_RSTp = not1(KUBY_APU_RSTn);

  /*_p10.DYTE*/ wire DYTE_A00n = not1(cpu_abus_new.BUS_CPU_A00p.out_new());
  /*_p10.AFOB*/ wire AFOB_A01n = not1(cpu_abus_new.BUS_CPU_A01p.out_new());
  /*_p10.ABUB*/ wire ABUB_A02n = not1(cpu_abus_new.BUS_CPU_A02p.out_new());
  /*_p10.ACOL*/ wire ACOL_A03n = not1(cpu_abus_new.BUS_CPU_A03p.out_new());
  /*#p10.BOXY*/ wire BOXY_A05n = not1(cpu_abus_new.BUS_CPU_A05p.out_new());
  /*_p10.DOSO*/ wire DOSO_A00p = not1(DYTE_A00n);
  /*_p10.DUPA*/ wire DUPA_A01p = not1(AFOB_A01n);

  /*_p10.AWET*/ wire AWET_ADDR_XX2Xn = or4(cpu_abus_new.BUS_CPU_A07p.out_new(), cpu_abus_new.BUS_CPU_A06p.out_new(), BOXY_A05n, cpu_abus_new.BUS_CPU_A04p.out_new());
  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn = not1(cpu_abus_new.SYKE_ADDR_HIp_new());
  /*_p10.BEZY*/ wire BEZY_ADDR_FF2Xn = or2(AWET_ADDR_XX2Xn, BAKO_ADDR_FFXXn);
  /*_p10.CONA*/ wire CONA_ADDR_FF2Xp = not1(BEZY_ADDR_FF2Xn);

  /*#p10.DONA*/ wire DONA_ADDR_0000n = nand4(DYTE_A00n, AFOB_A01n, ABUB_A02n, ACOL_A03n);
  /*#p10.DEWA*/ wire DEWA_ADDR_0001n = nand4(DOSO_A00p, AFOB_A01n, ABUB_A02n, ACOL_A03n);
  /*#p10.DOFA*/ wire DOFA_ADDR_0010p = and4(ACOL_A03n, ABUB_A02n, DUPA_A01p, DYTE_A00n);
  /*#p10.DUFE*/ wire DUFE_ADDR_0011n = nand4(DOSO_A00p, DUPA_A01p, ABUB_A02n, ACOL_A03n);

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(TEDO_CPU_RDp);
  /*_p10.BAFU*/ wire BAFU_CPU_WRn = not1(TAPU_CPU_WRp);
  /*_p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn);

  /*#p01.HAMA*/ wire HAMA_CLK_512K = not1(spu_new.JESO_CLK_512K.qp_new());
  /*_p01.BURE*/ wire BURE_CLK_512 = not1(spu_new.BARA_CLK_512.qp_new());
  /*#p01.FYNE*/ wire FYNE_CLK_512 = not1(BURE_CLK_512);
  /*_p01.GALE*/ wire GALE_CLK_512 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_new(), HAMA_CLK_512K, FYNE_CLK_512);
  /*_p01.GEXY*/ wire GEXY_CLK_512 = not1(GALE_CLK_512);
  /*_p01.HORU*/ wire HORU_CLK_512 = not1(GEXY_CLK_512);

  /*_p01.CULO*/ wire CULO_CLK_256 = not1(spu_new.CARU_CLK_256.qp_new());
  /*_p01.BEZE*/ wire BEZE_CLK_256 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_new(), HAMA_CLK_512K, CULO_CLK_256);
  /*_p01.COFU*/ wire COFU_CLK_256 = not1(BEZE_CLK_256);
  /*_p01.BUFY*/ wire BUFY_CLK_256 = not1(COFU_CLK_256);

  /*_p01.APEF*/ wire APEF_CLK_128 = not1(spu_new.BYLU_CLK_128.qp_new());
  /*_p01.BULE*/ wire BULE_CLK_128 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_new(), HAMA_CLK_512K, APEF_CLK_128);
  /*_p01.BARU*/ wire BARU_CLK_128 = not1(BULE_CLK_128);
  /*_p01.BYFE*/ wire BYFE_CLK_128 = not1(BARU_CLK_128);

  {
    /*#p10.COVO*/ wire COVO_ADDR_FF21p = nor2(DEWA_ADDR_0001n, BEZY_ADDR_FF2Xn);
    /*#p19.DACO*/ wire DACO_FF21_WRp = and2(BOGY_CPU_WRp, COVO_ADDR_FF21p);
    /*#p19.GOKO*/ wire GOKO_FF21_WRp = and2(COVO_ADDR_FF21p, BOGY_CPU_WRp);
    /*#p19.DYKE*/ wire DYKE_FF21_WRn = not1(DACO_FF21_WRp);
    /*#p19.FUPA*/ wire FUPA_FF21_WRn = not1(GOKO_FF21_WRp);

    /*#p19.FEXO*/ wire FEXO_APU_RSTn_new = not1(KEBA_APU_RSTp);

    /*_p19.EMOK*/ ch4_new.EMOK_NR42_ENV_TIMER0p.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D00p.qp_old());
    /*_p19.ETYJ*/ ch4_new.ETYJ_NR42_ENV_TIMER1p.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D01p.qp_old());
    /*_p19.EZYK*/ ch4_new.EZYK_NR42_ENV_TIMER2p.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D02p.qp_old());
    /*_p19.GEKY*/ ch4_new.GEKY_NR42_ENV_DIRp   .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D03p.qp_old());
    /*_p19.GARU*/ ch4_new.GARU_NR42_ENV0p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D04p.qp_old());
    /*_p19.GOKY*/ ch4_new.GOKY_NR42_ENV1p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D05p.qp_old());
    /*_p19.GOZO*/ ch4_new.GOZO_NR42_ENV2p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D06p.qp_old());
    /*_p19.GEDU*/ ch4_new.GEDU_NR42_ENV3p      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D07p.qp_old());
  }

  {
    /*#p10.EKEZ*/ wire EKEZ_ADDR_FF22p = and2(CONA_ADDR_FF2Xp, DOFA_ADDR_0010p);
    /*#p19.HUMO*/ wire HUMO_FF22_WRp = and2(BOGY_CPU_WRp, EKEZ_ADDR_FF22p);
    /*#p19.GETU*/ wire GETU_FF22_WRp = and2(BOGY_CPU_WRp, EKEZ_ADDR_FF22p);
    /*#p19.HOVA*/ wire HOVA_FF22_WRn = not1(HUMO_FF22_WRp);
    /*#p19.HOSO*/ wire HOSO_FF22_WRn = nand2(EKEZ_ADDR_FF22p, BOGY_CPU_WRp);
    /*#p19.EFUG*/ wire EFUG_FF22_WRn = not1(GETU_FF22_WRp);

    /*_p09.KAME*/ wire KAME_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p19.HYNE*/ wire HYNE_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p09.DAPA*/ wire DAPA_APU_RSTn_new = not1(KEBA_APU_RSTp);

    /*#p19.JARE*/ ch4_new.JARE_NR43_DIV0p .dff9(HOVA_FF22_WRn, KAME_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D00p.qp_old());
    /*#p19.JERO*/ ch4_new.JERO_NR43_DIV1p .dff9(HOVA_FF22_WRn, KAME_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D01p.qp_old());
    /*#p19.JAKY*/ ch4_new.JAKY_NR43_DIV2p .dff9(HOVA_FF22_WRn, KAME_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D02p.qp_old());
    /*#p19.JAMY*/ ch4_new.JAMY_NR43_MODEp .dff9(HOSO_FF22_WRn, HYNE_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D03p.qp_old());
    /*#p19.FETA*/ ch4_new.FETA_NR43_FREQ0p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D04p.qp_old());
    /*#p19.FYTO*/ ch4_new.FYTO_NR43_FREQ1p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D05p.qp_old());
    /*_p19.GOGO*/ ch4_new.GOGO_NR43_FREQ2p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D06p.qp_old());
    /*_p19.GAFO*/ ch4_new.GAFO_NR43_FREQ3p.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D07p.qp_old());
  }


  /*#p20.GASO*/ wire GASO_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p20.GYSU*/ ch4_new.GYSU_CH4_TRIG.dff17(DOVA_ABCDxxxx, GASO_APU_RSTn_new, ch4_old.HOGA_NR44_TRIGp.qp_old());

  /*#p20.FEBY*/ wire FEBY_APU_RSTn_new = not1(KEBA_APU_RSTp);

  /*#p20.GONE*/ ch4_new.GONE_CH4_TRIGp.dff17_clk(HAMA_CLK_512K,                    ch4_old.HAZO_CH4_TRIGn.qn_old());
  /*#p20.GORA*/ ch4_new.GORA_CH4_TRIGp.dff17    (HAMA_CLK_512K, FEBY_APU_RSTn_new, ch4_old.GONE_CH4_TRIGp.qp_old());
  /*#p20.GATY*/ ch4_new.GATY_CH4_TRIGp.dff17    (HAMA_CLK_512K, FEBY_APU_RSTn_new, ch4_old.GORA_CH4_TRIGp.qp_old());

  /*#p20.FALE*/ wire FALE_RESET_LATCHn = nor2(KEBA_APU_RSTp, ch4_new.GORA_CH4_TRIGp.qp_new());
  /*#p20.GONE*/ ch4_new.GONE_CH4_TRIGp.dff17_rst(FALE_RESET_LATCHn);

  /*#p20.HELU*/ wire HELU_RESET_LATCHp = not1(FALE_RESET_LATCHn);
  /*#p20.HAZO*/ ch4_new.HAZO_CH4_TRIGn.nor_latch(HELU_RESET_LATCHp, ch4_new.GYSU_CH4_TRIG.qp_new());


  // This goes to all the CHN_LENENp registers...why?
  /*#p10.CUGE*/ wire CUGE_ADDR_FF23p = nor2(DUFE_ADDR_0011n, BEZY_ADDR_FF2Xn);
  /*_p16.ANUJ*/ wire ANUJ_CPU_WR_WEIRD = and2(SIG_IN_CPU_DBUS_FREE, BOGY_CPU_WRp);
  /*#p19.DULU*/ wire DULU_FF23_WRn = nand2(ANUJ_CPU_WR_WEIRD, CUGE_ADDR_FF23p);
  /*#p19.FOXE*/ wire FOXE_FF23_WRn = nand2(BOGY_CPU_WRp, CUGE_ADDR_FF23p);
  /*#p20.GUZY*/ wire GUZY_NR44_TRIG_RST = nor2(KEBA_APU_RSTp, ch4_new.GYSU_CH4_TRIG.qp_new());

  /*#p19.CABE*/ wire CABE_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p19.CUNY*/ ch4_new.CUNY_NR44_LEN_ENp.dff9(DULU_FF23_WRn, CABE_APU_RSTn_new,  cpu_dbus_old.BUS_CPU_D06p.qp_old());
  /*#p19.HOGA*/ ch4_new.HOGA_NR44_TRIGp  .dff9(FOXE_FF23_WRn, GUZY_NR44_TRIG_RST, cpu_dbus_old.BUS_CPU_D07p.qp_old());

  /*#p20.GUNY*/ wire GUNY_FREQ_GATE_RSTn_new = nor2(KEBA_APU_RSTp, ch4_new.GONE_CH4_TRIGp.qp_new());

  /*#p20.HYNO*/ wire HYNO_DIV_MAX_old = and3(ch4_old.JYFU_DIV2.qp_old(), ch4_old.JYRE_DIV1.qp_old(), ch4_old.JYCO_DIV0.qp_old()); // Die may be wrong, this has to be an AND

  /*_p01.BAVU*/ wire BAVU_CLK_1M = not1(spu_new.AVOK_xBCDExxx.qp_new());
  /*#p20.GYBA*/ wire GYBA_CLK_1M = not1(BAVU_CLK_1M);
  /*#p20.GARY*/ ch4_new.GARY_FREQ_GATEp.dff17(GYBA_CLK_1M, GUNY_FREQ_GATE_RSTn_new, HYNO_DIV_MAX_old);




  {

    /*#p20.CARY*/ wire CARY_FREQ_CLK = and2(BAVU_CLK_1M, ch4_new.GARY_FREQ_GATEp.qp_new());
    /*#p09.DAPA*/ wire DAPA_APU_RSTn_new = not1(KEBA_APU_RSTp);

    /*#p20.CEXO*/ ch4_new.CEXO_FREQ_00.dff17(                CARY_FREQ_CLK, DAPA_APU_RSTn_new, ch4_old.CEXO_FREQ_00.qn_old());
    /*_p20.DEKO*/ ch4_new.DEKO_FREQ_01.dff17(ch4_new.CEXO_FREQ_00.qn_new(), DAPA_APU_RSTn_new, ch4_old.DEKO_FREQ_01.qn_old());
    /*_p20.EZEF*/ ch4_new.EZEF_FREQ_02.dff17(ch4_new.DEKO_FREQ_01.qn_new(), DAPA_APU_RSTn_new, ch4_old.EZEF_FREQ_02.qn_old());
    /*_p20.EPOR*/ ch4_new.EPOR_FREQ_03.dff17(ch4_new.EZEF_FREQ_02.qn_new(), DAPA_APU_RSTn_new, ch4_old.EPOR_FREQ_03.qn_old());
    /*_p20.DURE*/ ch4_new.DURE_FREQ_04.dff17(ch4_new.EPOR_FREQ_03.qn_new(), DAPA_APU_RSTn_new, ch4_old.DURE_FREQ_04.qn_old());
    /*_p20.DALE*/ ch4_new.DALE_FREQ_05.dff17(ch4_new.DURE_FREQ_04.qn_new(), DAPA_APU_RSTn_new, ch4_old.DALE_FREQ_05.qn_old());
    /*_p20.DOKE*/ ch4_new.DOKE_FREQ_06.dff17(ch4_new.DALE_FREQ_05.qn_new(), DAPA_APU_RSTn_new, ch4_old.DOKE_FREQ_06.qn_old());
    /*_p20.DEMO*/ ch4_new.DEMO_FREQ_07.dff17(ch4_new.DOKE_FREQ_06.qn_new(), DAPA_APU_RSTn_new, ch4_old.DEMO_FREQ_07.qn_old());
    /*_p20.DOSE*/ ch4_new.DOSE_FREQ_08.dff17(ch4_new.DEMO_FREQ_07.qn_new(), DAPA_APU_RSTn_new, ch4_old.DOSE_FREQ_08.qn_old());
    /*_p20.DETE*/ ch4_new.DETE_FREQ_09.dff17(ch4_new.DOSE_FREQ_08.qn_new(), DAPA_APU_RSTn_new, ch4_old.DETE_FREQ_09.qn_old());
    /*_p20.ERUT*/ ch4_new.ERUT_FREQ_10.dff17(ch4_new.DETE_FREQ_09.qn_new(), DAPA_APU_RSTn_new, ch4_old.ERUT_FREQ_10.qn_old());
    /*_p20.DOTA*/ ch4_new.DOTA_FREQ_11.dff17(ch4_new.ERUT_FREQ_10.qn_new(), DAPA_APU_RSTn_new, ch4_old.DOTA_FREQ_11.qn_old());
    /*_p20.DERE*/ ch4_new.DERE_FREQ_12.dff17(ch4_new.DOTA_FREQ_11.qn_new(), DAPA_APU_RSTn_new, ch4_old.DERE_FREQ_12.qn_old());
    /*_p20.ESEP*/ ch4_new.ESEP_FREQ_13.dff17(ch4_new.DERE_FREQ_12.qn_new(), DAPA_APU_RSTn_new, ch4_old.ESEP_FREQ_13.qn_old());
  }

  // 14-to-1 mux to select LFSR clock

  // some of these are wrong... let's see if inverting the freqNp bits works

  // emof = nor(gogo_qn, elar)

  /*#p20.EMOF*/ wire EMOF_LFSR_CLK_MUX_7 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_new(), ch4_new.FYTO_NR43_FREQ1p.qn_new(), ch4_new.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ELAR*/ wire ELAR_LFSR_CLK_MUX_6 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_new(), ch4_new.FYTO_NR43_FREQ1p.qn_new(), ch4_new.FETA_NR43_FREQ0p.qp_new());
  /*#p20.DUDU*/ wire DUDU_LFSR_CLK_MUX_5 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_new(), ch4_new.FYTO_NR43_FREQ1p.qp_new(), ch4_new.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ETAT*/ wire ETAT_LFSR_CLK_MUX_4 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_new(), ch4_new.FYTO_NR43_FREQ1p.qp_new(), ch4_new.FETA_NR43_FREQ0p.qp_new());
  /*#p20.FURA*/ wire FURA_LFSR_CLK_MUX_3 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_new(), ch4_new.FYTO_NR43_FREQ1p.qn_new(), ch4_new.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ETAR*/ wire ETAR_LFSR_CLK_MUX_2 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_new(), ch4_new.FYTO_NR43_FREQ1p.qn_new(), ch4_new.FETA_NR43_FREQ0p.qp_new());
  /*#p20.EVER*/ wire EVER_LFSR_CLK_MUX_1 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_new(), ch4_new.FYTO_NR43_FREQ1p.qp_new(), ch4_new.FETA_NR43_FREQ0p.qn_new());
  /*#p20.ETOV*/ wire ETOV_LFSR_CLK_MUX_0 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_new(), ch4_new.FYTO_NR43_FREQ1p.qp_new(), ch4_new.FETA_NR43_FREQ0p.qp_new());

  /*_p20.ETYR*/ wire ETYR_LFSR_CLK_MUX_A = amux6(
    ch4_new.ESEP_FREQ_13.qp_new(), DUDU_LFSR_CLK_MUX_5,
    ch4_new.DERE_FREQ_12.qp_new(), ETAT_LFSR_CLK_MUX_4,
    ch4_new.DOTA_FREQ_11.qp_new(), FURA_LFSR_CLK_MUX_3,
    ch4_new.ERUT_FREQ_10.qp_new(), ETAR_LFSR_CLK_MUX_2,
    ch4_new.DETE_FREQ_09.qp_new(), EVER_LFSR_CLK_MUX_1,
    ch4_new.DOSE_FREQ_08.qp_new(), ETOV_LFSR_CLK_MUX_0);

  /*_p20.ELYX*/ wire ELYX_LFSR_CLK_MUX_B = amux4(
    ch4_new.DEMO_FREQ_07.qp_new(), EMOF_LFSR_CLK_MUX_7,
    ch4_new.DOKE_FREQ_06.qp_new(), ELAR_LFSR_CLK_MUX_6,
    ch4_new.DALE_FREQ_05.qp_new(), DUDU_LFSR_CLK_MUX_5,
    ch4_new.DURE_FREQ_04.qp_new(), ETAT_LFSR_CLK_MUX_4);

  /*_p20.DARY*/ wire DARY_LFSR_CLK_MUX_C = amux4(
    ch4_new.EPOR_FREQ_03.qp_new(), FURA_LFSR_CLK_MUX_3,
    ch4_new.EZEF_FREQ_02.qp_new(), ETAR_LFSR_CLK_MUX_2,
    ch4_new.DEKO_FREQ_01.qp_new(), EVER_LFSR_CLK_MUX_1,
    ch4_new.CEXO_FREQ_00.qp_new(), ETOV_LFSR_CLK_MUX_0);

  /*_p20.ERYF*/ wire ERYF_LFSR_CLK_MUX_D = or2(ELYX_LFSR_CLK_MUX_B, DARY_LFSR_CLK_MUX_C);

  /*#p20.FEME*/ wire FEME_LFSR_CLKp_new = mux2p(ch4_new.GAFO_NR43_FREQ3p.qp_new(), ETYR_LFSR_CLK_MUX_A, ERYF_LFSR_CLK_MUX_D);




  {
    /*#p10.DANU*/ wire DANU_ADDR_FF20p = nor2(DONA_ADDR_0000n, BEZY_ADDR_FF2Xn);
    /*#p19.CAZE*/ wire CAZE_FF20_WRn = nand2(BOGY_CPU_WRp, DANU_ADDR_FF20p);
    /*#p19.FURU*/ wire FURU_FF20_WRa = not1(CAZE_FF20_WRn);
    /*#p19.DOTU*/ wire DOTU_FF20_WRb = not1(CAZE_FF20_WRn);
    /*#p19.EPEK*/ wire EPEK_FF20_WRc = not1(CAZE_FF20_WRn);

    // using FUGO_old to break the loop

    /*#p19.DODA*/ wire DODA_LEN_CLKn = nor3(ch4_old.FUGO_CH4_LEN_DONEp.qp_old(), BUFY_CLK_256, ch4_new.CUNY_NR44_LEN_ENp.qn_new());
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa = not1(DODA_LEN_CLKn);

    /*#p19.DANO*/ ch4_new.DANO_NR41_LEN0p.dff20(                   CUWA_LEN_CLKa, DOTU_FF20_WRb, cpu_dbus_old.BUS_CPU_D00p.qp_old());
    /*#p19.FAVY*/ ch4_new.FAVY_NR41_LEN1p.dff20(ch4_new.DANO_NR41_LEN0p.qp_new(), DOTU_FF20_WRb, cpu_dbus_old.BUS_CPU_D01p.qp_old());
    /*#p19.DENA*/ ch4_new.DENA_NR41_LEN2p.dff20(ch4_new.FAVY_NR41_LEN1p.qp_new(), DOTU_FF20_WRb, cpu_dbus_old.BUS_CPU_D02p.qp_old());
    /*#p19.CEDO*/ ch4_new.CEDO_NR41_LEN3p.dff20(ch4_new.DENA_NR41_LEN2p.qp_new(), DOTU_FF20_WRb, cpu_dbus_old.BUS_CPU_D03p.qp_old());

    /*#p19.DOPU*/ wire DOPU_NR41_LEN3p = not1(ch4_new.CEDO_NR41_LEN3p.qn_new());
    /*#p19.FYLO*/ ch4_new.FYLO_NR41_LEN4p.dff20(DOPU_NR41_LEN3p,                  EPEK_FF20_WRc, cpu_dbus_old.BUS_CPU_D04p.qp_old());
    /*#p19.EDOP*/ ch4_new.EDOP_NR41_LEN5p.dff20(ch4_new.FYLO_NR41_LEN4p.qp_new(), EPEK_FF20_WRc, cpu_dbus_old.BUS_CPU_D05p.qp_old());

    /*#p19.GAPY*/ wire GAPY_LEN_DONE_RSTn = nor3(FURU_FF20_WRa, KEBA_APU_RSTp, ch4_new.GONE_CH4_TRIGp.qp_new());
    /*#p19.FUGO*/ ch4_new.FUGO_CH4_LEN_DONEp.dff17(ch4_new.EDOP_NR41_LEN5p.qn_new(), GAPY_LEN_DONE_RSTn, ch4_old.FUGO_CH4_LEN_DONEp.qn_old());

    /*#p19.DODA*/ wire DODA_LEN_CLKn2 = nor3(ch4_new.FUGO_CH4_LEN_DONEp.qp_new(), BUFY_CLK_256, ch4_new.CUNY_NR44_LEN_ENp.qn_new());
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa2 = not1(DODA_LEN_CLKn2);

    /*#p19.DANO*/ ch4_new.DANO_NR41_LEN0p.dff20_any(CUWA_LEN_CLKa2, DOTU_FF20_WRb, cpu_dbus_old.BUS_CPU_D00p.qp_old());
  }





  /*#p20.GEVY*/ wire GEVY_CH4_AMP_ENn = nor5(
    ch4_new.GEKY_NR42_ENV_DIRp.qp_new(),
    ch4_new.GARU_NR42_ENV0p.qp_new(),
    ch4_new.GOKY_NR42_ENV1p.qp_new(),
    ch4_new.GOZO_NR42_ENV2p.qp_new(),
    ch4_new.GEDU_NR42_ENV3p.qp_new());


  {
    /*#p20.EFOT*/ wire EFOT_CH4_STOPp = and2(ch4_new.CUNY_NR44_LEN_ENp.qp_new(), ch4_new.FUGO_CH4_LEN_DONEp.qp_new());
    /*#p20.FEGY*/ wire FEGY_CH4_STOPp = or3(KEBA_APU_RSTp, EFOT_CH4_STOPp, GEVY_CH4_AMP_ENn);
    /*#p20.GENA*/ ch4_new.GENA_CH4_ACTIVEp.nor_latch(ch4_new.GONE_CH4_TRIGp.qp_new(), FEGY_CH4_STOPp);
  }

  {
    /*#p20.HERY*/ wire HERY_DIV_GATE1n = nor2(GEVY_CH4_AMP_ENn, KEBA_APU_RSTp);
    /*#p20.HAPU*/ wire HAPU_CH4_TRIGn = not1(ch4_new.GATY_CH4_TRIGp.qp_new());
    /*#p20.JERY*/ ch4_new.JERY_DIV_GATE.nand_latch(HERY_DIV_GATE1n, HAPU_CH4_TRIGn);
  }

  {
    /*#p20.KYKU*/ wire KYKU_DIV_CLKb = or2(ch4_new.JERY_DIV_GATE.qp_new(), spu_new.JESO_CLK_512K.qp_new());
    /*#p20.KONY*/ wire KONY_DIV_CLKn = not1(KYKU_DIV_CLKb);
    /*#p20.KANU*/ wire KANU_DIV_CLKa = not1(KONY_DIV_CLKn);

    /*#p20.GOFU*/ wire GOFU_DIV_LOADn = nor2(ch4_new.GONE_CH4_TRIGp.qp_new(), ch4_new.GARY_FREQ_GATEp.qp_new());
    /*#p20.HUCE*/ wire HUCE_DIV_LOADp = not1(GOFU_DIV_LOADn);

    /*#p20.JYCO*/ ch4_new.JYCO_DIV0.dff20(             KANU_DIV_CLKa, HUCE_DIV_LOADp, ch4_old.JARE_NR43_DIV0p.qn_new());
    /*#p20.JYRE*/ ch4_new.JYRE_DIV1.dff20(ch4_new.JYCO_DIV0.qp_new(), HUCE_DIV_LOADp, ch4_old.JERO_NR43_DIV1p.qn_new());
    /*_p20.JYFU*/ ch4_new.JYFU_DIV2.dff20(ch4_new.JYRE_DIV1.qp_new(), HUCE_DIV_LOADp, ch4_old.JAKY_NR43_DIV2p.qn_new());
  }


  //----------
  // The actual LFSR



  {
    /*#p20.JYJA*/ wire JYJA_LFSR_CLKn_new = not1(FEME_LFSR_CLKp_new);
    /*#p20.GUFA*/ wire GUFA_LFSR_CLKn_new = not1(FEME_LFSR_CLKp_new);
    /*#p20.GYVE*/ wire GYVE_LFSR_CLKp_new = not1(GUFA_LFSR_CLKn_new);
    /*#p20.KARA*/ wire KARA_LFSR_CLKn_new = not1(GYVE_LFSR_CLKp_new);
    /*#p20.KOPA*/ wire KOPA_LFSR_CLKp_new = not1(KARA_LFSR_CLKn_new);

    /*#p20.GEPO*/ wire GEPO_LFSR_RSTa = or2(ch4_new.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp);
    /*#p20.GOGE*/ wire GOGE_LFSR_RSTn = not1(GEPO_LFSR_RSTa);

    // naze is xnor2
    /*#p20.HURA*/ wire HURA_LFSR_IN_old = xnor2(ch4_old.HEZU_LFSR_15.qp_old(), ch4_old.HYRO_LFSR_14.qp_old());

    /*#p20.JOTO*/ ch4_new.JOTO_LFSR_00.dff17(JYJA_LFSR_CLKn_new, GOGE_LFSR_RSTn, HURA_LFSR_IN_old);
    /*#p20.KOMU*/ ch4_new.KOMU_LFSR_01.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.JOTO_LFSR_00.qp_old());
    /*#p20.KETU*/ ch4_new.KETU_LFSR_02.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.KOMU_LFSR_01.qp_old());
    /*#p20.KUTA*/ ch4_new.KUTA_LFSR_03.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.KETU_LFSR_02.qp_old());
    /*#p20.KUZY*/ ch4_new.KUZY_LFSR_04.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.KUTA_LFSR_03.qp_old());
    /*#p20.KYWY*/ ch4_new.KYWY_LFSR_05.dff17(KOPA_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.KUZY_LFSR_04.qp_old());
    /*#p20.JAJU*/ ch4_new.JAJU_LFSR_06.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.KYWY_LFSR_05.qp_old());
    /*#p20.HAPE*/ ch4_new.HAPE_LFSR_07.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.JAJU_LFSR_06.qp_old());
    /*#p20.JUXE*/ ch4_new.JUXE_LFSR_08.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.HAPE_LFSR_07.qp_old());

    /*#p20.KAVU*/ wire KAVU_LFSR_FB_old = amux2(ch4_old.JOTO_LFSR_00.qp_old(), ch4_old.JAMY_NR43_MODEp.qp_old(), ch4_old.JAMY_NR43_MODEp.qn_old(), ch4_old.JUXE_LFSR_08.qp_old());
    /*#p20.JEPE*/ ch4_new.JEPE_LFSR_09.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, KAVU_LFSR_FB_old);
    /*#p20.JAVO*/ ch4_new.JAVO_LFSR_10.dff17(GYVE_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.JEPE_LFSR_09.qp_old());
    /*#p20.HEPA*/ ch4_new.HEPA_LFSR_11.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.JAVO_LFSR_10.qp_old());
    /*#p20.HORY*/ ch4_new.HORY_LFSR_12.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.HEPA_LFSR_11.qp_old());
    /*#p20.HENO*/ ch4_new.HENO_LFSR_13.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.HORY_LFSR_12.qp_old());
    /*#p20.HYRO*/ ch4_new.HYRO_LFSR_14.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.HENO_LFSR_13.qp_old());
    /*#p20.HEZU*/ ch4_new.HEZU_LFSR_15.dff17(FEME_LFSR_CLKp_new, GOGE_LFSR_RSTn, ch4_old.HYRO_LFSR_14.qp_old());
  }

  //----------
  // Env

  {
    // Generates a 1 usec pulse when the env timer hits 111

    /*#p20.EJEX*/ wire EJEX_ENV_TIMER_MAX_old = or3(ch4_old.DOGO_ENV_DELAY2n.qp_old(), ch4_old.COFE_ENV_DELAY1n.qp_old(), ch4_old.CUNA_ENV_DELAY0n.qp_old());
    /*#p20.FOSY*/ ch4_new.FOSY_ENV_CLKp.dff17_clk(HORU_CLK_512, EJEX_ENV_TIMER_MAX_old);

    /*#p20.GEXE*/ wire GEXE_ENV_PULSEn_new = not1(ch4_new.FOSY_ENV_CLKp.qp_new());
    /*#p20.HURY*/ wire HURY_ENV_PULSE_RSTp_new = nor2(HORU_CLK_512, GEXE_ENV_PULSEn_new);
    /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new = nor3(ch4_new.EMOK_NR42_ENV_TIMER0p.qp_new(), ch4_new.ETYJ_NR42_ENV_TIMER1p.qp_new(), ch4_new.EZYK_NR42_ENV_TIMER2p.qp_new());
    /*#p20.GOPA*/ wire GOPA_ENV_PULSE_RSTn_new = nor4(HURY_ENV_PULSE_RSTp_new, FOWA_ENV_OFFp_new, ch4_new.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp);
    /*#p20.FOSY*/ ch4_new.FOSY_ENV_CLKp.dff17_rst(GOPA_ENV_PULSE_RSTn_new);
  }

  {
    /*#p20.ALOP*/ wire ALOP_CLK_128 = not1(BYFE_CLK_128);
    /*#p20.BOKY*/ wire BOKY_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p20.ABEL*/ ch4_new.ABEL_CLK_64.dff17(ALOP_CLK_128, BOKY_APU_RSTn_new, ch4_old.ABEL_CLK_64.qn_old());
    /*#p20.BAWA*/ wire BAWA_CLK_64_new = not1(ch4_new.ABEL_CLK_64.qp_new());
    /*#p20.BUXO*/ wire BUXO_CLK_64_new = not1(BAWA_CLK_64_new);

    /*#p20.ENEC*/ wire ENEC_ENV_TIMER_LOADn_new = nor2(ch4_new.GONE_CH4_TRIGp.qp_new(), ch4_new.FOSY_ENV_CLKp.qp_new());
    /*#p20.DAPY*/ wire DAPY_ENV_TIMER_LOADp_new = not1(ENEC_ENV_TIMER_LOADn_new);

    /*#p20.CUNA*/ ch4_new.CUNA_ENV_DELAY0n.dff20(                  BUXO_CLK_64_new, DAPY_ENV_TIMER_LOADp_new, ch4_old.EMOK_NR42_ENV_TIMER0p.qn_old());
    /*#p20.COFE*/ ch4_new.COFE_ENV_DELAY1n.dff20(ch4_new.CUNA_ENV_DELAY0n.qp_new(), DAPY_ENV_TIMER_LOADp_new, ch4_old.ETYJ_NR42_ENV_TIMER1p.qn_old());
    /*#p20.DOGO*/ ch4_new.DOGO_ENV_DELAY2n.dff20(ch4_new.COFE_ENV_DELAY1n.qp_new(), DAPY_ENV_TIMER_LOADp_new, ch4_old.EZYK_NR42_ENV_TIMER2p.qn_old());
  }


  {
    /*#p20.EMET*/ wire EMET_ENV_STOP_RSTn = nor2(ch4_new.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp);

    /*#p20.DARO*/ wire DARO_ENV_BOTp_old = nor5 (ch4_old.GEKY_NR42_ENV_DIRp.qp_old(), ch4_old.FEKO_CH4_VOL0.qp_old(), ch4_old.FATY_CH4_VOL1.qp_old(), ch4_old.FERU_CH4_VOL2.qp_old(), ch4_old.FYRO_CH4_VOL3.qp_old());
    /*#p20.CUTY*/ wire CUTY_ENV_TOPn_old = nand5(ch4_old.GEKY_NR42_ENV_DIRp.qp_old(), ch4_old.FEKO_CH4_VOL0.qp_old(), ch4_old.FATY_CH4_VOL1.qp_old(), ch4_old.FERU_CH4_VOL2.qp_old(), ch4_old.FYRO_CH4_VOL3.qp_old());
    /*#p20.DUBO*/ wire DUBO_ENV_TOPp_old = not1(CUTY_ENV_TOPn_old);
    /*#p20.EVUR*/ wire EVUR_ENV_MAXp_old = or2(DARO_ENV_BOTp_old, DUBO_ENV_TOPp_old);

    /*#p20.FYNO*/ ch4_new.FYNO_ENV_MAXp.dff17(ch4_new.FOSY_ENV_CLKp.qp_new(), EMET_ENV_STOP_RSTn, EVUR_ENV_MAXp_old);
  }





  {
    /*#p20.ENUR*/ wire ENUR_ENV_STARTp = or2(KEBA_APU_RSTp, ch4_new.GONE_CH4_TRIGp.qp_new());
    /*#p20.EROX*/ ch4_new.EROX_ENV_RUNNINGn.nor_latch(ch4_new.FYNO_ENV_MAXp.qp_new(), ENUR_ENV_STARTp);

    /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new = nor3(ch4_new.EMOK_NR42_ENV_TIMER0p.qp_new(), ch4_new.ETYJ_NR42_ENV_TIMER1p.qp_new(), ch4_new.EZYK_NR42_ENV_TIMER2p.qp_new());
    /*#p20.FELO*/ wire FELO_ENV_CLK = or3(ch4_new.FOSY_ENV_CLKp.qp_new(), FOWA_ENV_OFFp_new, ch4_new.EROX_ENV_RUNNINGn.qp_new());
    /*#p20.FOLE*/ wire FOLE_VOL_CLK0n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_new(), FELO_ENV_CLK, FELO_ENV_CLK, ch4_new.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FEKO*/ ch4_new.FEKO_CH4_VOL0.dff20(FOLE_VOL_CLK0n, ch4_new.GONE_CH4_TRIGp.qp_new(), ch4_new.GARU_NR42_ENV0p.qp_new());

    /*#p20.ETEF*/ wire ETEF_VOL_CLK1n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_new(), ch4_new.FEKO_CH4_VOL0.qp_new(), ch4_new.FEKO_CH4_VOL0.qn_new(), ch4_new.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FATY*/ ch4_new.FATY_CH4_VOL1.dff20(ETEF_VOL_CLK1n, ch4_new.GONE_CH4_TRIGp.qp_new(), ch4_new.GOKY_NR42_ENV1p.qp_new());

    /*#p20.EDYF*/ wire EDYF_VOL_CLK2n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_new(), ch4_new.FATY_CH4_VOL1.qp_new(), ch4_new.FATY_CH4_VOL1.qn_new(), ch4_new.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FERU*/ ch4_new.FERU_CH4_VOL2.dff20(EDYF_VOL_CLK2n, ch4_new.GONE_CH4_TRIGp.qp_new(), ch4_new.GOZO_NR42_ENV2p.qp_new());

    /*#p20.ELAF*/ wire ELAF_VOL_CLK3n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_new(), ch4_new.FERU_CH4_VOL2.qp_new(), ch4_new.FERU_CH4_VOL2.qn_new(), ch4_new.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p20.FYRO*/ ch4_new.FYRO_CH4_VOL3.dff20(ELAF_VOL_CLK3n, ch4_new.GONE_CH4_TRIGp.qp_new(), ch4_new.GEDU_NR42_ENV3p.qp_new());
  }


#if 0
  /*#p20.COSA*/ wire COSA_CPU_RDp = not1(reg_new.cpu_signals.AGUZ_CPU_RDn());
  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  /*#p20.DYRY*/ wire DYRY_DBG = and2(ch4_new.CUNY_NR44_LEN_ENp.qn_new(), EDEK_NR52_DBG_APUp);
  /*_p20.COMO*/ wire COMO_DBG = and2(DYRY_DBG, COSA_CPU_RDp);
  /*_p20.BAGU*/ wire BAGU_DBG = nand2(CUGE_ADDR_FF23p, COMO_DBG);
  /*_p20.BEFA*/ wire BEFA_DBG = not1(CARY_FREQ_CLK);
  /*_p20.ATEL*/ triwire ATEL = tri6_nn(BAGU_DBG, BEFA_DBG); // goes to data bus D0
#endif

  {
    /*#p10.COVO*/ wire COVO_ADDR_FF21p = nor2(DEWA_ADDR_0001n, BEZY_ADDR_FF2Xn);
    /*#p19.BOFY*/ wire BOFY_FF21n = not1(COVO_ADDR_FF21p);
    /*#p19.GONY*/ wire GONY_FF21n = not1(COVO_ADDR_FF21p);
    /*#p19.BOXE*/ wire BOXE_FF21_RDn = or2(BOFY_FF21n, AGUZ_CPU_RDn);
    /*#p19.HASU*/ wire HASU_FF21_RDn = or2(GONY_FF21n, AGUZ_CPU_RDn);

    /*#p19.DEMY*/ triwire DEMY = tri6_nn(BOXE_FF21_RDn, ch4_new.EMOK_NR42_ENV_TIMER0p.qn_new());
    /*#p19.COCE*/ triwire COCE = tri6_nn(BOXE_FF21_RDn, ch4_new.ETYJ_NR42_ENV_TIMER1p.qn_new());
    /*#p19.CUZU*/ triwire CUZU = tri6_nn(BOXE_FF21_RDn, ch4_new.EZYK_NR42_ENV_TIMER2p.qn_new());
    /*#p19.GOME*/ triwire GOME = tri6_nn(HASU_FF21_RDn, ch4_new.GEKY_NR42_ENV_DIRp.qn_new());
    /*#p19.HEDA*/ triwire HEDA = tri6_nn(HASU_FF21_RDn, ch4_new.GARU_NR42_ENV0p.qn_new());
    /*#p19.GODU*/ triwire GODU = tri6_nn(HASU_FF21_RDn, ch4_new.GOKY_NR42_ENV1p.qn_new());
    /*#p19.HOGE*/ triwire HOGE = tri6_nn(HASU_FF21_RDn, ch4_new.GOZO_NR42_ENV2p.qn_new());
    /*#p19.HACU*/ triwire HACU = tri6_nn(HASU_FF21_RDn, ch4_new.GEDU_NR42_ENV3p.qn_new());

    /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(DEMY);
    /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(COCE);
    /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(CUZU);
    /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(GOME);
    /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(HEDA);
    /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(GODU);
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(HOGE);
    /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(HACU);
  }

  {
    /*#p19.KAGE*/ wire KAGE_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p10.EKEZ*/ wire EKEZ_ADDR_FF22p = and2(CONA_ADDR_FF2Xp, DOFA_ADDR_0010p);

    /*#p19.KOKU*/ wire KOKU_ADDR_FF22n = not1(EKEZ_ADDR_FF22p);
    /*#p19.GUGO*/ wire GUGO_ADDR_FF22n = not1(EKEZ_ADDR_FF22p);
    /*#p19.KEKA*/ wire KEKA_FF22_RDn = or2(KOKU_ADDR_FF22n, AGUZ_CPU_RDn);
    /*#p19.JORA*/ wire JORA_FF22_RDn = nand2(KAGE_CPU_RDp, EKEZ_ADDR_FF22p);
    /*#p19.HEZE*/ wire HEZE_FF22_RDn = or2(GUGO_ADDR_FF22n, AGUZ_CPU_RDn);

    /*#p19.KAMO*/ triwire KAMO = tri6_nn(KEKA_FF22_RDn, ch4_new.JARE_NR43_DIV0p.qn_new());
    /*#p19.KAKU*/ triwire KAKU = tri6_nn(KEKA_FF22_RDn, ch4_new.JERO_NR43_DIV1p.qn_new());
    /*_p19.KYRO*/ triwire KYRO = tri6_nn(KEKA_FF22_RDn, ch4_new.JAKY_NR43_DIV2p.qn_new());
    /*_p19.KETA*/ triwire KETA = tri6_nn(JORA_FF22_RDn, ch4_new.JAMY_NR43_MODEp.qn_new());
    /*_p19.GEDA*/ triwire GEDA = tri6_nn(HEZE_FF22_RDn, ch4_new.FETA_NR43_FREQ0p.qn_new());
    /*_p19.GYPE*/ triwire GYPE = tri6_nn(HEZE_FF22_RDn, ch4_new.FYTO_NR43_FREQ1p.qn_new());
    /*_p19.GAKA*/ triwire GAKA = tri6_nn(HEZE_FF22_RDn, ch4_new.GOGO_NR43_FREQ2p.qn_new());
    /*_p19.HAPY*/ triwire HAPY = tri6_nn(HEZE_FF22_RDn, ch4_new.GAFO_NR43_FREQ3p.qn_new());

    /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(KAMO);
    /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(KAKU);
    /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(KYRO);
    /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(KETA);
    /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(GEDA);
    /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(GYPE);
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(GAKA);
    /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(HAPY);
  }

  {
    /*#p19.BYLO*/ wire BYLO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p19.BARE*/ wire BARE_FF23_RDn = nand2(CUGE_ADDR_FF23p, BYLO_CPU_RDp);
    /*#p19.CURY*/ triwire CURY = tri6_nn(BARE_FF23_RDn, ch4_new.CUNY_NR44_LEN_ENp.qn_new());
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(CURY);
  }

}

#endif
