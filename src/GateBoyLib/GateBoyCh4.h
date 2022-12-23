#pragma once
#include "AudioLib/Audio.h"

struct SpuChannel4;
sample_t ch4_audio_out(const SpuChannel4& ch4);

//==============================================================================
//             CHANNEL 4
//==============================================================================

struct SpuChannel4 {
  void reset() {
    EMOK_NR42_ENV_DELAY0p.state = 0x1a;
    ETYJ_NR42_ENV_DELAY1p.state = 0x1a;
    EZYK_NR42_ENV_DELAY2p.state = 0x1a;
    GEKY_NR42_ENV_DIRp.state = 0x1a;
    GARU_NR42_VOL0p.state = 0x1a;
    GOKY_NR42_VOL1p.state = 0x1a;
    GOZO_NR42_VOL2p.state = 0x1a;
    GEDU_NR42_VOL3p.state = 0x1a;
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
    JERY_FREQ_GATE.state = 0x19;
    JYCO_FREQ_DELAY0.state = 0x1a;
    JYRE_FREQ_DELAY1.state = 0x18;
    JYFU_FREQ_DELAY2.state = 0x18;
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
    CUNA_ENV_TIMER0n.state = 0x18;
    COFE_ENV_TIMER1n.state = 0x19;
    DOGO_ENV_TIMER2n.state = 0x1a;
    FOSY_ENV_CLKp.state = 0x1a;
    FYNO_VOL_MAXp.state = 0x18;
    FEKO_CH4_VOL0.state = 0x1a;
    FATY_CH4_VOL1.state = 0x1a;
    FERU_CH4_VOL2.state = 0x1a;
    FYRO_CH4_VOL3.state = 0x1a;
    EROX_ENV_RUNNINGn.state = 0x18;
  }


  /*#p19.DANO*/ DFF20 DANO_NR41_LEN0p;
  /*#p19.FAVY*/ DFF20 FAVY_NR41_LEN1p;
  /*#p19.DENA*/ DFF20 DENA_NR41_LEN2p;
  /*#p19.CEDO*/ DFF20 CEDO_NR41_LEN3p;
  /*#p19.FYLO*/ DFF20 FYLO_NR41_LEN4p;
  /*_p19.EDOP*/ DFF20 EDOP_NR41_LEN5p;

  /*_p19.EMOK*/ DFF9 EMOK_NR42_ENV_DELAY0p;
  /*_p19.ETYJ*/ DFF9 ETYJ_NR42_ENV_DELAY1p;
  /*_p19.EZYK*/ DFF9 EZYK_NR42_ENV_DELAY2p;
  /*_p19.GEKY*/ DFF9 GEKY_NR42_ENV_DIRp;
  /*_p19.GARU*/ DFF9 GARU_NR42_VOL0p; // 10
  /*_p19.GOKY*/ DFF9 GOKY_NR42_VOL1p;
  /*_p19.GOZO*/ DFF9 GOZO_NR42_VOL2p;
  /*_p19.GEDU*/ DFF9 GEDU_NR42_VOL3p;

  /*_p19.JARE*/ DFF9 JARE_NR43_DIV0p;
  /*_p19.JERO*/ DFF9 JERO_NR43_DIV1p;
  /*_p19.JAKY*/ DFF9 JAKY_NR43_DIV2p;
  /*_p19.JAMY*/ DFF9 JAMY_NR43_MODEp;
  /*_p19.FETA*/ DFF9 FETA_NR43_FREQ0p;
  /*_p19.FYTO*/ DFF9 FYTO_NR43_FREQ1p;
  /*_p19.GOGO*/ DFF9 GOGO_NR43_FREQ2p; // 20
  /*_p19.GAFO*/ DFF9 GAFO_NR43_FREQ3p;

  /*#p19.CUNY*/ DFF9 CUNY_NR44_LEN_ENp;
  /*#p19.HOGA*/ DFF9 HOGA_NR44_TRIGp;

  //----------

  /*_p20.CEXO*/ DFF17 CEXO_FREQ_00;
  /*_p20.DEKO*/ DFF17 DEKO_FREQ_01;
  /*_p20.EZEF*/ DFF17 EZEF_FREQ_02;
  /*_p20.EPOR*/ DFF17 EPOR_FREQ_03;
  /*_p20.DURE*/ DFF17 DURE_FREQ_04;
  /*_p20.DALE*/ DFF17 DALE_FREQ_05;
  /*_p20.DOKE*/ DFF17 DOKE_FREQ_06; // 30
  /*_p20.DEMO*/ DFF17 DEMO_FREQ_07;
  /*_p20.DOSE*/ DFF17 DOSE_FREQ_08;
  /*_p20.DETE*/ DFF17 DETE_FREQ_09;
  /*_p20.ERUT*/ DFF17 ERUT_FREQ_10;
  /*_p20.DOTA*/ DFF17 DOTA_FREQ_11;
  /*_p20.DERE*/ DFF17 DERE_FREQ_12;
  /*_p20.ESEP*/ DFF17 ESEP_FREQ_13;

  /*_p20.JOTO*/ DFF17 JOTO_LFSR_00;
  /*_p20.KOMU*/ DFF17 KOMU_LFSR_01;
  /*_p20.KETU*/ DFF17 KETU_LFSR_02; // 40
  /*_p20.KUTA*/ DFF17 KUTA_LFSR_03;
  /*_p20.KUZY*/ DFF17 KUZY_LFSR_04;
  /*_p20.KYWY*/ DFF17 KYWY_LFSR_05;
  /*_p20.JAJU*/ DFF17 JAJU_LFSR_06;
  /*_p20.HAPE*/ DFF17 HAPE_LFSR_07;
  /*_p20.JUXE*/ DFF17 JUXE_LFSR_08;
  /*_p20.JEPE*/ DFF17 JEPE_LFSR_09;
  /*_p20.JAVO*/ DFF17 JAVO_LFSR_10;
  /*_p20.HEPA*/ DFF17 HEPA_LFSR_11;
  /*_p20.HORY*/ DFF17 HORY_LFSR_12; // 50
  /*_p20.HENO*/ DFF17 HENO_LFSR_13;
  /*_p20.HYRO*/ DFF17 HYRO_LFSR_14;
  /*_p20.HEZU*/ DFF17 HEZU_LFSR_15;

  /*_p20.GENA*/ NorLatch GENA_CH4_ACTIVEp;

  /*_p20.GONE*/ DFF17 GONE_CH4_TRIGp;
  /*_p20.GORA*/ DFF17 GORA_CH4_TRIGp;
  /*_p20.GATY*/ DFF17 GATY_CH4_TRIGp;
  /*_p19.FUGO*/ DFF17 FUGO_CH4_LEN_DONEp;

  /*#p20.GYSU*/ DFF17 GYSU_CH4_TRIG;

  /*#p20.HAZO*/ NorLatch HAZO_CH4_TRIGn; // 60
  /*#p20.JERY*/ NandLatch JERY_FREQ_GATE;
  /*#p20.JYCO*/ DFF20 JYCO_FREQ_DELAY0;
  /*#p20.JYRE*/ DFF20 JYRE_FREQ_DELAY1;
  /*#p20.JYFU*/ DFF20 JYFU_FREQ_DELAY2;
  /*#p20.GARY*/ DFF17 GARY_FREQ_GATEp;

  /*#p20.ABEL*/ DFF17 ABEL_CLK_64;
  /*#p20.CUNA*/ DFF20 CUNA_ENV_TIMER0n;
  /*#p20.COFE*/ DFF20 COFE_ENV_TIMER1n;
  /*#p20.DOGO*/ DFF20 DOGO_ENV_TIMER2n;
  /*#p20.FOSY*/ DFF17 FOSY_ENV_CLKp;
  /*#p20.FYNO*/ DFF17 FYNO_VOL_MAXp;
  /*#p20.FEKO*/ DFF20 FEKO_CH4_VOL0;
  /*#p20.FATY*/ DFF20 FATY_CH4_VOL1;
  /*#p20.FERU*/ DFF20 FERU_CH4_VOL2;
  /*#p20.FYRO*/ DFF20 FYRO_CH4_VOL3;

  /*#p20.EROX*/ NorLatch EROX_ENV_RUNNINGn;
};
