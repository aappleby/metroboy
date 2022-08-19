#include "GateBoySPU.h"

#include "GateBoy.h"

#ifdef SIM_AUDIO

sample_t ch4_audio_out_fast(const SpuChannel4& ch4) {

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
  /*_p20.DATO*/ wire DATO_CH4_RAW_BIT = or2(EZUL_CH4_BIT_MUX, COTE_DBG_CH4_MUTE);

  /*#p20.AKOF*/ wire AKOF_CH4_DAC0 = and2(ch4.FEKO_CH4_VOL0.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.BYZY*/ wire BYZY_CH4_DAC1 = and2(ch4.FATY_CH4_VOL1.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.APYR*/ wire APYR_CH4_DAC2 = and2(ch4.FERU_CH4_VOL2.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.BOZA*/ wire BOZA_CH4_DAC3 = and2(ch4.FYRO_CH4_VOL3.qp_old(), DATO_CH4_RAW_BIT);

  return ((AKOF_CH4_DAC0 & 1) << 0) |
    ((BYZY_CH4_DAC1 & 1) << 1) |
    ((APYR_CH4_DAC2 & 1) << 2) |
    ((BOZA_CH4_DAC3 & 1) << 3);
}

//------------------------------------------------------------------------------

void tick_ch4_fast(
  const GateBoyCpuDBus& cpu_dbus_old,
  const GateBoySPU& spu_old,
  const SpuChannel4& ch4_old,
  const GateBoyWaveBus& wave_dbus_old,
  const GateBoyCpuABus& cpu_abus_new,

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
  auto addr = bit_pack(cpu_abus_new);

  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = and2(AFUR_ABCDxxxx_qn, SIG_CPU_CLKREQ);
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx = not1(BUDE_xxxxEFGH);

  /*_p09.KEBA*/ wire KEBA_APU_RSTp = or3(AFER_SYS_RSTp, ASOL_POR_DONEn, spu_new.HADA_NR52_ALL_SOUND_ON.qn_any());

  /*_p10.DYTE*/ wire DYTE_A00n = not1(cpu_abus_new.BUS_CPU_A00p.out_any());
  /*_p10.AFOB*/ wire AFOB_A01n = not1(cpu_abus_new.BUS_CPU_A01p.out_any());
  /*_p10.ABUB*/ wire ABUB_A02n = not1(cpu_abus_new.BUS_CPU_A02p.out_any());
  /*_p10.ACOL*/ wire ACOL_A03n = not1(cpu_abus_new.BUS_CPU_A03p.out_any());
  /*#p10.BOXY*/ wire BOXY_A05n = not1(cpu_abus_new.BUS_CPU_A05p.out_any());
  /*_p10.DOSO*/ wire DOSO_A00p = not1(DYTE_A00n);
  /*_p10.DUPA*/ wire DUPA_A01p = not1(AFOB_A01n);

  /*_p10.AWET*/ wire AWET_ADDR_XX2Xn = or4(cpu_abus_new.BUS_CPU_A07p.out_any(), cpu_abus_new.BUS_CPU_A06p.out_any(), BOXY_A05n, cpu_abus_new.BUS_CPU_A04p.out_any());
  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn = not1(cpu_abus_new.SYKE_ADDR_HIp_any());
  /*_p10.BEZY*/ wire BEZY_ADDR_FF2Xn = or2(AWET_ADDR_XX2Xn, BAKO_ADDR_FFXXn);
  /*_p10.CONA*/ wire CONA_ADDR_FF2Xp = not1(BEZY_ADDR_FF2Xn);

  /*#p10.DONA*/ wire DONA_ADDR_0000n = nand4(DYTE_A00n, AFOB_A01n, ABUB_A02n, ACOL_A03n);
  /*#p10.DEWA*/ wire DEWA_ADDR_0001n = nand4(DOSO_A00p, AFOB_A01n, ABUB_A02n, ACOL_A03n);
  /*#p10.DOFA*/ wire DOFA_ADDR_0010p = and4(ACOL_A03n, ABUB_A02n, DUPA_A01p, DYTE_A00n);
  /*#p10.DUFE*/ wire DUFE_ADDR_0011n = nand4(DOSO_A00p, DUPA_A01p, ABUB_A02n, ACOL_A03n);

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(TEDO_CPU_RDp);
  /*_p10.BAFU*/ wire BAFU_CPU_WRn = not1(TAPU_CPU_WRp);
  /*_p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn);

  /*#p01.HAMA*/ wire HAMA_CLK_512K = not1(spu_new.JESO_CLK_512K.qp_any());
  /*_p01.BURE*/ wire BURE_CLK_512 = not1(spu_new.BARA_CLK_512.qp_any());
  /*#p01.FYNE*/ wire FYNE_CLK_512 = not1(BURE_CLK_512);
  /*_p01.GALE*/ wire GALE_CLK_512 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_any(), HAMA_CLK_512K, FYNE_CLK_512);
  /*_p01.GEXY*/ wire GEXY_CLK_512 = not1(GALE_CLK_512);
  /*_p01.HORU*/ wire HORU_CLK_512 = not1(GEXY_CLK_512);

  /*_p01.CULO*/ wire CULO_CLK_256 = not1(spu_new.CARU_CLK_256.qp_any());
  /*_p01.BEZE*/ wire BEZE_CLK_256 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_any(), HAMA_CLK_512K, CULO_CLK_256);
  /*_p01.COFU*/ wire COFU_CLK_256 = not1(BEZE_CLK_256);
  /*_p01.BUFY*/ wire BUFY_CLK_256 = not1(COFU_CLK_256);

  /*_p01.APEF*/ wire APEF_CLK_128 = not1(spu_new.BYLU_CLK_128.qp_any());
  /*_p01.BULE*/ wire BULE_CLK_128 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_any(), HAMA_CLK_512K, APEF_CLK_128);
  /*_p01.BARU*/ wire BARU_CLK_128 = not1(BULE_CLK_128);
  /*_p01.BYFE*/ wire BYFE_CLK_128 = not1(BARU_CLK_128);

  {
    /*_p19.EMOK*/ ch4_new.EMOK_NR42_ENV_TIMER0p.dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D00p.qp_any());
    /*_p19.ETYJ*/ ch4_new.ETYJ_NR42_ENV_TIMER1p.dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D01p.qp_any());
    /*_p19.EZYK*/ ch4_new.EZYK_NR42_ENV_TIMER2p.dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D02p.qp_any());
    /*_p19.GEKY*/ ch4_new.GEKY_NR42_ENV_DIRp   .dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D03p.qp_any());
    /*_p19.GARU*/ ch4_new.GARU_NR42_ENV0p      .dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D04p.qp_any());
    /*_p19.GOKY*/ ch4_new.GOKY_NR42_ENV1p      .dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D05p.qp_any());
    /*_p19.GOZO*/ ch4_new.GOZO_NR42_ENV2p      .dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D06p.qp_any());
    /*_p19.GEDU*/ ch4_new.GEDU_NR42_ENV3p      .dff9(!(addr == 0xFF21 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D07p.qp_any());
  }

  {
    /*#p19.JARE*/ ch4_new.JARE_NR43_DIV0p .dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D00p.qp_any());
    /*#p19.JERO*/ ch4_new.JERO_NR43_DIV1p .dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D01p.qp_any());
    /*#p19.JAKY*/ ch4_new.JAKY_NR43_DIV2p .dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D02p.qp_any());
    /*#p19.JAMY*/ ch4_new.JAMY_NR43_MODEp .dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D03p.qp_any());
    /*#p19.FETA*/ ch4_new.FETA_NR43_FREQ0p.dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D04p.qp_any());
    /*#p19.FYTO*/ ch4_new.FYTO_NR43_FREQ1p.dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D05p.qp_any());
    /*_p19.GOGO*/ ch4_new.GOGO_NR43_FREQ2p.dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D06p.qp_any());
    /*_p19.GAFO*/ ch4_new.GAFO_NR43_FREQ3p.dff9(!(addr == 0xFF22 && TAPU_CPU_WRp), ~KEBA_APU_RSTp, cpu_dbus_old.BUS_CPU_D07p.qp_any());
  }


  /*#p20.GYSU*/ ch4_new.GYSU_CH4_TRIG.dff17(DOVA_ABCDxxxx, ~KEBA_APU_RSTp, ch4_old.HOGA_NR44_TRIGp.qp_any());

  /*#p20.GONE*/ ch4_new.GONE_CH4_TRIGp.dff17_clk(HAMA_CLK_512K, ch4_old.HAZO_CH4_TRIGn.qn_any());
  /*#p20.GORA*/ ch4_new.GORA_CH4_TRIGp.dff17(HAMA_CLK_512K, ~KEBA_APU_RSTp, ch4_old.GONE_CH4_TRIGp.qp_any());
  /*#p20.GATY*/ ch4_new.GATY_CH4_TRIGp.dff17(HAMA_CLK_512K, ~KEBA_APU_RSTp, ch4_old.GORA_CH4_TRIGp.qp_any());

  /*#p20.FALE*/ wire FALE_RESET_LATCHn = nor2(KEBA_APU_RSTp, ch4_new.GORA_CH4_TRIGp.qp_any());
  /*#p20.GONE*/ ch4_new.GONE_CH4_TRIGp.dff17_rst(FALE_RESET_LATCHn);

  /*#p20.HELU*/ wire HELU_RESET_LATCHp = not1(FALE_RESET_LATCHn);
  /*#p20.HAZO*/ ch4_new.HAZO_CH4_TRIGn.nor_latch(HELU_RESET_LATCHp, ch4_new.GYSU_CH4_TRIG.qp_any());


  // This goes to all the CHN_LENENp registers...why?
  /*#p10.CUGE*/ wire CUGE_ADDR_FF23p = nor2(DUFE_ADDR_0011n, BEZY_ADDR_FF2Xn);
  /*_p16.ANUJ*/ wire ANUJ_CPU_WR_WEIRD = and2(SIG_IN_CPU_DBUS_FREE, BOGY_CPU_WRp);
  /*#p19.DULU*/ wire DULU_FF23_WRn = nand2(ANUJ_CPU_WR_WEIRD, CUGE_ADDR_FF23p);
  /*#p19.FOXE*/ wire FOXE_FF23_WRn = nand2(BOGY_CPU_WRp, CUGE_ADDR_FF23p);
  /*#p20.GUZY*/ wire GUZY_NR44_TRIG_RST = nor2(KEBA_APU_RSTp, ch4_new.GYSU_CH4_TRIG.qp_any());

  /*#p19.CABE*/ wire CABE_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p19.CUNY*/ ch4_new.CUNY_NR44_LEN_ENp.dff9(!(addr == 0xFF23 && TAPU_CPU_WRp), CABE_APU_RSTn_new, cpu_dbus_old.BUS_CPU_D06p.qp_any());
  /*#p19.HOGA*/ ch4_new.HOGA_NR44_TRIGp  .dff9(!(addr == 0xFF23 && TAPU_CPU_WRp), GUZY_NR44_TRIG_RST, cpu_dbus_old.BUS_CPU_D07p.qp_any());

  /*#p20.GUNY*/ wire GUNY_FREQ_GATE_RSTn_new = nor2(KEBA_APU_RSTp, ch4_new.GONE_CH4_TRIGp.qp_any());

  /*#p20.HYNO*/ wire HYNO_DIV_MAX_old = and3(ch4_old.JYFU_DIV2.qp_any(), ch4_old.JYRE_DIV1.qp_any(), ch4_old.JYCO_DIV0.qp_any()); // Die may be wrong, this has to be an AND

  /*_p01.BAVU*/ wire BAVU_CLK_1M = not1(spu_new.AVOK_xBCDExxx.qp_any());
  /*#p20.GYBA*/ wire GYBA_CLK_1M = not1(BAVU_CLK_1M);
  /*#p20.GARY*/ ch4_new.GARY_FREQ_GATEp.dff17(GYBA_CLK_1M, GUNY_FREQ_GATE_RSTn_new, HYNO_DIV_MAX_old);




  {

    /*#p20.CARY*/ wire CARY_FREQ_CLK = and2(BAVU_CLK_1M, ch4_new.GARY_FREQ_GATEp.qp_any());

    /*#p20.CEXO*/ ch4_new.CEXO_FREQ_00.dff17(CARY_FREQ_CLK,                 ~KEBA_APU_RSTp, ch4_old.CEXO_FREQ_00.qn_any());
    /*_p20.DEKO*/ ch4_new.DEKO_FREQ_01.dff17(ch4_new.CEXO_FREQ_00.qn_any(), ~KEBA_APU_RSTp, ch4_old.DEKO_FREQ_01.qn_any());
    /*_p20.EZEF*/ ch4_new.EZEF_FREQ_02.dff17(ch4_new.DEKO_FREQ_01.qn_any(), ~KEBA_APU_RSTp, ch4_old.EZEF_FREQ_02.qn_any());
    /*_p20.EPOR*/ ch4_new.EPOR_FREQ_03.dff17(ch4_new.EZEF_FREQ_02.qn_any(), ~KEBA_APU_RSTp, ch4_old.EPOR_FREQ_03.qn_any());
    /*_p20.DURE*/ ch4_new.DURE_FREQ_04.dff17(ch4_new.EPOR_FREQ_03.qn_any(), ~KEBA_APU_RSTp, ch4_old.DURE_FREQ_04.qn_any());
    /*_p20.DALE*/ ch4_new.DALE_FREQ_05.dff17(ch4_new.DURE_FREQ_04.qn_any(), ~KEBA_APU_RSTp, ch4_old.DALE_FREQ_05.qn_any());
    /*_p20.DOKE*/ ch4_new.DOKE_FREQ_06.dff17(ch4_new.DALE_FREQ_05.qn_any(), ~KEBA_APU_RSTp, ch4_old.DOKE_FREQ_06.qn_any());
    /*_p20.DEMO*/ ch4_new.DEMO_FREQ_07.dff17(ch4_new.DOKE_FREQ_06.qn_any(), ~KEBA_APU_RSTp, ch4_old.DEMO_FREQ_07.qn_any());
    /*_p20.DOSE*/ ch4_new.DOSE_FREQ_08.dff17(ch4_new.DEMO_FREQ_07.qn_any(), ~KEBA_APU_RSTp, ch4_old.DOSE_FREQ_08.qn_any());
    /*_p20.DETE*/ ch4_new.DETE_FREQ_09.dff17(ch4_new.DOSE_FREQ_08.qn_any(), ~KEBA_APU_RSTp, ch4_old.DETE_FREQ_09.qn_any());
    /*_p20.ERUT*/ ch4_new.ERUT_FREQ_10.dff17(ch4_new.DETE_FREQ_09.qn_any(), ~KEBA_APU_RSTp, ch4_old.ERUT_FREQ_10.qn_any());
    /*_p20.DOTA*/ ch4_new.DOTA_FREQ_11.dff17(ch4_new.ERUT_FREQ_10.qn_any(), ~KEBA_APU_RSTp, ch4_old.DOTA_FREQ_11.qn_any());
    /*_p20.DERE*/ ch4_new.DERE_FREQ_12.dff17(ch4_new.DOTA_FREQ_11.qn_any(), ~KEBA_APU_RSTp, ch4_old.DERE_FREQ_12.qn_any());
    /*_p20.ESEP*/ ch4_new.ESEP_FREQ_13.dff17(ch4_new.DERE_FREQ_12.qn_any(), ~KEBA_APU_RSTp, ch4_old.ESEP_FREQ_13.qn_any());
  }

  /*#p20.EMOF*/ wire EMOF_LFSR_CLK_MUX_7 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_any(), ch4_new.FYTO_NR43_FREQ1p.qn_any(), ch4_new.FETA_NR43_FREQ0p.qn_any());
  /*#p20.ELAR*/ wire ELAR_LFSR_CLK_MUX_6 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_any(), ch4_new.FYTO_NR43_FREQ1p.qn_any(), ch4_new.FETA_NR43_FREQ0p.qp_any());
  /*#p20.DUDU*/ wire DUDU_LFSR_CLK_MUX_5 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_any(), ch4_new.FYTO_NR43_FREQ1p.qp_any(), ch4_new.FETA_NR43_FREQ0p.qn_any());
  /*#p20.ETAT*/ wire ETAT_LFSR_CLK_MUX_4 = nor3(ch4_new.GOGO_NR43_FREQ2p.qn_any(), ch4_new.FYTO_NR43_FREQ1p.qp_any(), ch4_new.FETA_NR43_FREQ0p.qp_any());
  /*#p20.FURA*/ wire FURA_LFSR_CLK_MUX_3 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_any(), ch4_new.FYTO_NR43_FREQ1p.qn_any(), ch4_new.FETA_NR43_FREQ0p.qn_any());
  /*#p20.ETAR*/ wire ETAR_LFSR_CLK_MUX_2 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_any(), ch4_new.FYTO_NR43_FREQ1p.qn_any(), ch4_new.FETA_NR43_FREQ0p.qp_any());
  /*#p20.EVER*/ wire EVER_LFSR_CLK_MUX_1 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_any(), ch4_new.FYTO_NR43_FREQ1p.qp_any(), ch4_new.FETA_NR43_FREQ0p.qn_any());
  /*#p20.ETOV*/ wire ETOV_LFSR_CLK_MUX_0 = nor3(ch4_new.GOGO_NR43_FREQ2p.qp_any(), ch4_new.FYTO_NR43_FREQ1p.qp_any(), ch4_new.FETA_NR43_FREQ0p.qp_any());

  /*_p20.ETYR*/ wire ETYR_LFSR_CLK_MUX_A = amux6(
    ch4_new.ESEP_FREQ_13.qp_any(), DUDU_LFSR_CLK_MUX_5,
    ch4_new.DERE_FREQ_12.qp_any(), ETAT_LFSR_CLK_MUX_4,
    ch4_new.DOTA_FREQ_11.qp_any(), FURA_LFSR_CLK_MUX_3,
    ch4_new.ERUT_FREQ_10.qp_any(), ETAR_LFSR_CLK_MUX_2,
    ch4_new.DETE_FREQ_09.qp_any(), EVER_LFSR_CLK_MUX_1,
    ch4_new.DOSE_FREQ_08.qp_any(), ETOV_LFSR_CLK_MUX_0);

  /*_p20.ELYX*/ wire ELYX_LFSR_CLK_MUX_B = amux4(
    ch4_new.DEMO_FREQ_07.qp_any(), EMOF_LFSR_CLK_MUX_7,
    ch4_new.DOKE_FREQ_06.qp_any(), ELAR_LFSR_CLK_MUX_6,
    ch4_new.DALE_FREQ_05.qp_any(), DUDU_LFSR_CLK_MUX_5,
    ch4_new.DURE_FREQ_04.qp_any(), ETAT_LFSR_CLK_MUX_4);

  /*_p20.DARY*/ wire DARY_LFSR_CLK_MUX_C = amux4(
    ch4_new.EPOR_FREQ_03.qp_any(), FURA_LFSR_CLK_MUX_3,
    ch4_new.EZEF_FREQ_02.qp_any(), ETAR_LFSR_CLK_MUX_2,
    ch4_new.DEKO_FREQ_01.qp_any(), EVER_LFSR_CLK_MUX_1,
    ch4_new.CEXO_FREQ_00.qp_any(), ETOV_LFSR_CLK_MUX_0);

  /*_p20.ERYF*/ wire ERYF_LFSR_CLK_MUX_D = or2(ELYX_LFSR_CLK_MUX_B, DARY_LFSR_CLK_MUX_C);


  {
    /*#p10.DANU*/ wire DANU_ADDR_FF20p = nor2(DONA_ADDR_0000n, BEZY_ADDR_FF2Xn);
    /*#p19.CAZE*/ wire CAZE_FF20_WRn = nand2(BOGY_CPU_WRp, DANU_ADDR_FF20p);
    /*#p19.FURU*/ wire FURU_FF20_WRa = not1(CAZE_FF20_WRn);
    /*#p19.DOTU*/ wire DOTU_FF20_WRb = not1(CAZE_FF20_WRn);
    /*#p19.EPEK*/ wire EPEK_FF20_WRc = not1(CAZE_FF20_WRn);

    // using FUGO_old to break the loop

    /*#p19.DODA*/ wire DODA_LEN_CLKn = nor3(ch4_old.FUGO_CH4_LEN_DONEp.qp_any(), BUFY_CLK_256, ch4_new.CUNY_NR44_LEN_ENp.qn_any());
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa = not1(DODA_LEN_CLKn);

    /*#p19.DANO*/ ch4_new.DANO_NR41_LEN0p.dff20(CUWA_LEN_CLKa,                    DOTU_FF20_WRb, cpu_dbus_new.BUS_CPU_D00p.qp_any());
    /*#p19.FAVY*/ ch4_new.FAVY_NR41_LEN1p.dff20(ch4_new.DANO_NR41_LEN0p.qp_any(), DOTU_FF20_WRb, cpu_dbus_new.BUS_CPU_D01p.qp_any());
    /*#p19.DENA*/ ch4_new.DENA_NR41_LEN2p.dff20(ch4_new.FAVY_NR41_LEN1p.qp_any(), DOTU_FF20_WRb, cpu_dbus_new.BUS_CPU_D02p.qp_any());
    /*#p19.CEDO*/ ch4_new.CEDO_NR41_LEN3p.dff20(ch4_new.DENA_NR41_LEN2p.qp_any(), DOTU_FF20_WRb, cpu_dbus_new.BUS_CPU_D03p.qp_any());

    /*#p19.DOPU*/ wire DOPU_NR41_LEN3p = not1(ch4_new.CEDO_NR41_LEN3p.qn_any());
    /*#p19.FYLO*/ ch4_new.FYLO_NR41_LEN4p.dff20(DOPU_NR41_LEN3p, EPEK_FF20_WRc, cpu_dbus_new.BUS_CPU_D04p.qp_any());
    /*#p19.EDOP*/ ch4_new.EDOP_NR41_LEN5p.dff20(ch4_new.FYLO_NR41_LEN4p.qp_any(), EPEK_FF20_WRc, cpu_dbus_new.BUS_CPU_D05p.qp_any());

    /*#p19.GAPY*/ wire GAPY_LEN_DONE_RSTn = nor3(FURU_FF20_WRa, KEBA_APU_RSTp, ch4_new.GONE_CH4_TRIGp.qp_any());
    /*#p19.FUGO*/ ch4_new.FUGO_CH4_LEN_DONEp.dff17(ch4_new.EDOP_NR41_LEN5p.qn_any(), GAPY_LEN_DONE_RSTn, ch4_old.FUGO_CH4_LEN_DONEp.qn_any());

    /*#p19.DODA*/ wire DODA_LEN_CLKn2 = nor3(ch4_new.FUGO_CH4_LEN_DONEp.qp_any(), BUFY_CLK_256, ch4_new.CUNY_NR44_LEN_ENp.qn_any());
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa2 = not1(DODA_LEN_CLKn2);

    /*#p19.DANO*/ ch4_new.DANO_NR41_LEN0p.dff20_any(CUWA_LEN_CLKa2, DOTU_FF20_WRb, cpu_dbus_new.BUS_CPU_D00p.qp_any());
  }

  /*#p20.GEVY*/ wire GEVY_CH4_AMP_ENn = nor5(
    ch4_new.GEKY_NR42_ENV_DIRp.qp_any(),
    ch4_new.GARU_NR42_ENV0p.qp_any(),
    ch4_new.GOKY_NR42_ENV1p.qp_any(),
    ch4_new.GOZO_NR42_ENV2p.qp_any(),
    ch4_new.GEDU_NR42_ENV3p.qp_any());


  {
    /*#p20.EFOT*/ wire EFOT_CH4_STOPp = and2(ch4_new.CUNY_NR44_LEN_ENp.qp_any(), ch4_new.FUGO_CH4_LEN_DONEp.qp_any());
    /*#p20.FEGY*/ wire FEGY_CH4_STOPp = or3(KEBA_APU_RSTp, EFOT_CH4_STOPp, GEVY_CH4_AMP_ENn);
    /*#p20.GENA*/ ch4_new.GENA_CH4_ACTIVEp.nor_latch(ch4_new.GONE_CH4_TRIGp.qp_any(), FEGY_CH4_STOPp);
  }

  {
    /*#p20.JERY*/ ch4_new.JERY_DIV_GATE.nand_latch(nor2(GEVY_CH4_AMP_ENn, KEBA_APU_RSTp), not1(ch4_new.GATY_CH4_TRIGp.qp_any()));
  }

  {
    /*#p20.KANU*/ wire KANU_DIV_CLKa = or2(ch4_new.JERY_DIV_GATE.qp_any(), spu_new.JESO_CLK_512K.qp_any());

    /*#p20.HUCE*/ wire HUCE_DIV_LOADp = or2(ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GARY_FREQ_GATEp.qp_any());

    /*#p20.JYCO*/ ch4_new.JYCO_DIV0.dff20(KANU_DIV_CLKa,              HUCE_DIV_LOADp, ch4_new.JARE_NR43_DIV0p.qn_any());
    /*#p20.JYRE*/ ch4_new.JYRE_DIV1.dff20(ch4_new.JYCO_DIV0.qp_any(), HUCE_DIV_LOADp, ch4_new.JERO_NR43_DIV1p.qn_any());
    /*_p20.JYFU*/ ch4_new.JYFU_DIV2.dff20(ch4_new.JYRE_DIV1.qp_any(), HUCE_DIV_LOADp, ch4_new.JAKY_NR43_DIV2p.qn_any());
  }


  //----------
  // The actual LFSR

  {
    /*#p20.FEME*/ wire FEME_LFSR_CLKp_new = mux2p(ch4_new.GAFO_NR43_FREQ3p.qp_any(), ETYR_LFSR_CLK_MUX_A, ERYF_LFSR_CLK_MUX_D);
    /*#p20.HURA*/ wire HURA_LFSR_IN_old = xnor2(ch4_old.HEZU_LFSR_15.qp_any(), ch4_old.HYRO_LFSR_14.qp_any());

    if (or2(ch4_new.GONE_CH4_TRIGp.qp_any(), KEBA_APU_RSTp) & 1) {
      /*#p20.JOTO*/ ch4_new.JOTO_LFSR_00.dff17(not1(FEME_LFSR_CLKp_new), 0, HURA_LFSR_IN_old);
      /*#p20.KOMU*/ ch4_new.KOMU_LFSR_01.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.JOTO_LFSR_00.qp_any());
      /*#p20.KETU*/ ch4_new.KETU_LFSR_02.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.KOMU_LFSR_01.qp_any());
      /*#p20.KUTA*/ ch4_new.KUTA_LFSR_03.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.KETU_LFSR_02.qp_any());
      /*#p20.KUZY*/ ch4_new.KUZY_LFSR_04.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.KUTA_LFSR_03.qp_any());
      /*#p20.KYWY*/ ch4_new.KYWY_LFSR_05.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.KUZY_LFSR_04.qp_any());
      /*#p20.JAJU*/ ch4_new.JAJU_LFSR_06.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.KYWY_LFSR_05.qp_any());
      /*#p20.HAPE*/ ch4_new.HAPE_LFSR_07.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.JAJU_LFSR_06.qp_any());
      /*#p20.JUXE*/ ch4_new.JUXE_LFSR_08.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.HAPE_LFSR_07.qp_any());

      /*#p20.KAVU*/ wire KAVU_LFSR_FB_old = amux2(ch4_old.JOTO_LFSR_00.qp_any(), ch4_old.JAMY_NR43_MODEp.qp_any(), ch4_old.JAMY_NR43_MODEp.qn_any(), ch4_old.JUXE_LFSR_08.qp_any());
      /*#p20.JEPE*/ ch4_new.JEPE_LFSR_09.dff17(FEME_LFSR_CLKp_new,       0, KAVU_LFSR_FB_old);
      /*#p20.JAVO*/ ch4_new.JAVO_LFSR_10.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.JEPE_LFSR_09.qp_any());
      /*#p20.HEPA*/ ch4_new.HEPA_LFSR_11.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.JAVO_LFSR_10.qp_any());
      /*#p20.HORY*/ ch4_new.HORY_LFSR_12.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.HEPA_LFSR_11.qp_any());
      /*#p20.HENO*/ ch4_new.HENO_LFSR_13.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.HORY_LFSR_12.qp_any());
      /*#p20.HYRO*/ ch4_new.HYRO_LFSR_14.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.HENO_LFSR_13.qp_any());
      /*#p20.HEZU*/ ch4_new.HEZU_LFSR_15.dff17(FEME_LFSR_CLKp_new,       0, ch4_old.HYRO_LFSR_14.qp_any());
    } else {

      /*#p20.JOTO*/ ch4_new.JOTO_LFSR_00.dff17(not1(FEME_LFSR_CLKp_new), 1, HURA_LFSR_IN_old);
      /*#p20.KOMU*/ ch4_new.KOMU_LFSR_01.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.JOTO_LFSR_00.qp_any());
      /*#p20.KETU*/ ch4_new.KETU_LFSR_02.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.KOMU_LFSR_01.qp_any());
      /*#p20.KUTA*/ ch4_new.KUTA_LFSR_03.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.KETU_LFSR_02.qp_any());
      /*#p20.KUZY*/ ch4_new.KUZY_LFSR_04.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.KUTA_LFSR_03.qp_any());
      /*#p20.KYWY*/ ch4_new.KYWY_LFSR_05.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.KUZY_LFSR_04.qp_any());
      /*#p20.JAJU*/ ch4_new.JAJU_LFSR_06.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.KYWY_LFSR_05.qp_any());
      /*#p20.HAPE*/ ch4_new.HAPE_LFSR_07.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.JAJU_LFSR_06.qp_any());
      /*#p20.JUXE*/ ch4_new.JUXE_LFSR_08.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.HAPE_LFSR_07.qp_any());

      /*#p20.KAVU*/ wire KAVU_LFSR_FB_old = amux2(ch4_old.JOTO_LFSR_00.qp_any(), ch4_old.JAMY_NR43_MODEp.qp_any(), ch4_old.JAMY_NR43_MODEp.qn_any(), ch4_old.JUXE_LFSR_08.qp_any());
      /*#p20.JEPE*/ ch4_new.JEPE_LFSR_09.dff17(FEME_LFSR_CLKp_new,       1, KAVU_LFSR_FB_old);
      /*#p20.JAVO*/ ch4_new.JAVO_LFSR_10.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.JEPE_LFSR_09.qp_any());
      /*#p20.HEPA*/ ch4_new.HEPA_LFSR_11.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.JAVO_LFSR_10.qp_any());
      /*#p20.HORY*/ ch4_new.HORY_LFSR_12.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.HEPA_LFSR_11.qp_any());
      /*#p20.HENO*/ ch4_new.HENO_LFSR_13.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.HORY_LFSR_12.qp_any());
      /*#p20.HYRO*/ ch4_new.HYRO_LFSR_14.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.HENO_LFSR_13.qp_any());
      /*#p20.HEZU*/ ch4_new.HEZU_LFSR_15.dff17(FEME_LFSR_CLKp_new,       1, ch4_old.HYRO_LFSR_14.qp_any());
    }

  }

  //----------
  // Env

  {
    // Generates a 1 usec pulse when the env timer hits 111

    /*#p20.EJEX*/ wire EJEX_ENV_TIMER_MAX_old = or3(ch4_old.DOGO_ENV_DELAY2n.qp_any(), ch4_old.COFE_ENV_DELAY1n.qp_any(), ch4_old.CUNA_ENV_DELAY0n.qp_any());
    /*#p20.FOSY*/ ch4_new.FOSY_ENV_CLKp.dff17_clk(HORU_CLK_512, EJEX_ENV_TIMER_MAX_old);

    /*#p20.GEXE*/ wire GEXE_ENV_PULSEn_new = not1(ch4_new.FOSY_ENV_CLKp.qp_any());
    /*#p20.HURY*/ wire HURY_ENV_PULSE_RSTp_new = nor2(HORU_CLK_512, GEXE_ENV_PULSEn_new);
    /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new = nor3(ch4_new.EMOK_NR42_ENV_TIMER0p.qp_any(), ch4_new.ETYJ_NR42_ENV_TIMER1p.qp_any(), ch4_new.EZYK_NR42_ENV_TIMER2p.qp_any());
    /*#p20.GOPA*/ wire GOPA_ENV_PULSE_RSTn_new = nor4(HURY_ENV_PULSE_RSTp_new, FOWA_ENV_OFFp_new, ch4_new.GONE_CH4_TRIGp.qp_any(), KEBA_APU_RSTp);
    /*#p20.FOSY*/ ch4_new.FOSY_ENV_CLKp.dff17_rst(GOPA_ENV_PULSE_RSTn_new);
  }

  {
    /*#p20.ALOP*/ wire ALOP_CLK_128 = not1(BYFE_CLK_128);
    /*#p20.BOKY*/ wire BOKY_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p20.ABEL*/ ch4_new.ABEL_CLK_64.dff17(ALOP_CLK_128, BOKY_APU_RSTn_new, ch4_old.ABEL_CLK_64.qn_any());
    /*#p20.BAWA*/ wire BAWA_CLK_64_new = not1(ch4_new.ABEL_CLK_64.qp_any());
    /*#p20.BUXO*/ wire BUXO_CLK_64_new = not1(BAWA_CLK_64_new);

    /*#p20.ENEC*/ wire ENEC_ENV_TIMER_LOADn_new = nor2(ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.FOSY_ENV_CLKp.qp_any());
    /*#p20.DAPY*/ wire DAPY_ENV_TIMER_LOADp_new = not1(ENEC_ENV_TIMER_LOADn_new);

    /*#p20.CUNA*/ ch4_new.CUNA_ENV_DELAY0n.dff20(BUXO_CLK_64_new, DAPY_ENV_TIMER_LOADp_new, ch4_new.EMOK_NR42_ENV_TIMER0p.qn_any());
    /*#p20.COFE*/ ch4_new.COFE_ENV_DELAY1n.dff20(ch4_new.CUNA_ENV_DELAY0n.qp_any(), DAPY_ENV_TIMER_LOADp_new, ch4_new.ETYJ_NR42_ENV_TIMER1p.qn_any());
    /*#p20.DOGO*/ ch4_new.DOGO_ENV_DELAY2n.dff20(ch4_new.COFE_ENV_DELAY1n.qp_any(), DAPY_ENV_TIMER_LOADp_new, ch4_new.EZYK_NR42_ENV_TIMER2p.qn_any());
  }


  {
    /*#p20.EMET*/ wire EMET_ENV_STOP_RSTn = nor2(ch4_new.GONE_CH4_TRIGp.qp_any(), KEBA_APU_RSTp);

    /*#p20.DARO*/ wire DARO_ENV_BOTp_old = nor5(ch4_old.GEKY_NR42_ENV_DIRp.qp_any(), ch4_old.FEKO_CH4_VOL0.qp_any(), ch4_old.FATY_CH4_VOL1.qp_any(), ch4_old.FERU_CH4_VOL2.qp_any(), ch4_old.FYRO_CH4_VOL3.qp_any());
    /*#p20.CUTY*/ wire CUTY_ENV_TOPn_old = nand5(ch4_old.GEKY_NR42_ENV_DIRp.qp_any(), ch4_old.FEKO_CH4_VOL0.qp_any(), ch4_old.FATY_CH4_VOL1.qp_any(), ch4_old.FERU_CH4_VOL2.qp_any(), ch4_old.FYRO_CH4_VOL3.qp_any());
    /*#p20.DUBO*/ wire DUBO_ENV_TOPp_old = not1(CUTY_ENV_TOPn_old);
    /*#p20.EVUR*/ wire EVUR_ENV_MAXp_old = or2(DARO_ENV_BOTp_old, DUBO_ENV_TOPp_old);

    /*#p20.FYNO*/ ch4_new.FYNO_ENV_MAXp.dff17(ch4_new.FOSY_ENV_CLKp.qp_any(), EMET_ENV_STOP_RSTn, EVUR_ENV_MAXp_old);
  }





  {
    /*#p20.ENUR*/ wire ENUR_ENV_STARTp = or2(KEBA_APU_RSTp, ch4_new.GONE_CH4_TRIGp.qp_any());
    /*#p20.EROX*/ ch4_new.EROX_ENV_RUNNINGn.nor_latch(ch4_new.FYNO_ENV_MAXp.qp_any(), ENUR_ENV_STARTp);

    /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new = nor3(ch4_new.EMOK_NR42_ENV_TIMER0p.qp_any(), ch4_new.ETYJ_NR42_ENV_TIMER1p.qp_any(), ch4_new.EZYK_NR42_ENV_TIMER2p.qp_any());
    /*#p20.FELO*/ wire FELO_ENV_CLK = or3(ch4_new.FOSY_ENV_CLKp.qp_any(), FOWA_ENV_OFFp_new, ch4_new.EROX_ENV_RUNNINGn.qp_any());
    /*#p20.FOLE*/ wire FOLE_VOL_CLK0n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_any(), FELO_ENV_CLK, FELO_ENV_CLK, ch4_new.GEKY_NR42_ENV_DIRp.qn_any());
    /*#p20.FEKO*/ ch4_new.FEKO_CH4_VOL0.dff20(FOLE_VOL_CLK0n, ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GARU_NR42_ENV0p.qp_any());

    /*#p20.ETEF*/ wire ETEF_VOL_CLK1n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_any(), ch4_new.FEKO_CH4_VOL0.qp_any(), ch4_new.FEKO_CH4_VOL0.qn_any(), ch4_new.GEKY_NR42_ENV_DIRp.qn_any());
    /*#p20.FATY*/ ch4_new.FATY_CH4_VOL1.dff20(ETEF_VOL_CLK1n, ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GOKY_NR42_ENV1p.qp_any());

    /*#p20.EDYF*/ wire EDYF_VOL_CLK2n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_any(), ch4_new.FATY_CH4_VOL1.qp_any(), ch4_new.FATY_CH4_VOL1.qn_any(), ch4_new.GEKY_NR42_ENV_DIRp.qn_any());
    /*#p20.FERU*/ ch4_new.FERU_CH4_VOL2.dff20(EDYF_VOL_CLK2n, ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GOZO_NR42_ENV2p.qp_any());

    /*#p20.ELAF*/ wire ELAF_VOL_CLK3n = amux2(ch4_new.GEKY_NR42_ENV_DIRp.qp_any(), ch4_new.FERU_CH4_VOL2.qp_any(), ch4_new.FERU_CH4_VOL2.qn_any(), ch4_new.GEKY_NR42_ENV_DIRp.qn_any());
    /*#p20.FYRO*/ ch4_new.FYRO_CH4_VOL3.dff20(ELAF_VOL_CLK3n, ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GEDU_NR42_ENV3p.qp_any());
  }


#if 0
  /*#p20.COSA*/ wire COSA_CPU_RDp = not1(reg_new.cpu_signals.AGUZ_CPU_RDn());
  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  /*#p20.DYRY*/ wire DYRY_DBG = and2(ch4_new.CUNY_NR44_LEN_ENp.qn_any(), EDEK_NR52_DBG_APUp);
  /*_p20.COMO*/ wire COMO_DBG = and2(DYRY_DBG, COSA_CPU_RDp);
  /*_p20.BAGU*/ wire BAGU_DBG = nand2(CUGE_ADDR_FF23p, COMO_DBG);
  /*_p20.BEFA*/ wire BEFA_DBG = not1(CARY_FREQ_CLK);
  /*_p20.ATEL*/ triwire ATEL = tri6_nn(BAGU_DBG, BEFA_DBG); // goes to data bus D0
#endif

  if (addr == 0xFF21 && TEDO_CPU_RDp) bit_unpack(cpu_dbus_new, bit_pack(&ch4_new.EMOK_NR42_ENV_TIMER0p, 8));
  if (addr == 0xFF22 && TEDO_CPU_RDp) bit_unpack(cpu_dbus_new, bit_pack(&ch4_new.JARE_NR43_DIV0p, 8));

  {
    /*#p19.BYLO*/ wire BYLO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p19.BARE*/ wire BARE_FF23_RDn = nand2(CUGE_ADDR_FF23p, BYLO_CPU_RDp);
    /*#p19.CURY*/ triwire CURY = tri6_nn(BARE_FF23_RDn, ch4_new.CUNY_NR44_LEN_ENp.qn_any());
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(CURY);
  }

}

#endif
