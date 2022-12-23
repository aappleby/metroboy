#pragma once
#include "AudioLib/Audio.h"

struct SpuChannel2;
sample_t ch2_audio_out(const SpuChannel2& ch2);

//==============================================================================
//             CHANNEL 2
//==============================================================================

struct SpuChannel2 {
  void reset() {
    ERYC_NR21_LEN0.state = 0x1a;
    CERA_NR21_LEN1.state = 0x18;
    CONU_NR21_LEN2.state = 0x18;
    CAME_NR21_LEN3.state = 0x18;
    BUVA_NR21_LEN4.state = 0x18;
    AKYD_NR21_LEN5.state = 0x18;
    BERA_NR21_DUTY0.state = 0x1a;
    BAMY_NR21_DUTY1.state = 0x1a;
    HYFU_NR22_P0p.state = 0x1a;
    HORE_NR22_P1p.state = 0x1a;
    HAVA_NR22_P2p.state = 0x1a;
    FORE_NR22_ADDp.state = 0x1a;
    GATA_NR22_V0p.state = 0x1a;
    GUFE_NR22_V1p.state = 0x1a;
    GURA_NR22_V2p.state = 0x1a;
    GAGE_NR22_V3p.state = 0x1a;
    FOFE_NR23_FREQ00p.state = 0x1a;
    FOVA_NR23_FREQ01p.state = 0x1a;
    FEDY_NR23_FREQ02p.state = 0x1a;
    FOME_NR23_FREQ03p.state = 0x1a;
    FORA_NR23_FREQ04p.state = 0x1a;
    GODA_NR23_FREQ05p.state = 0x1a;
    GUMY_NR23_FREQ06p.state = 0x1a;
    GUPU_NR23_FREQ07p.state = 0x1a;
    JEFU_NR24_FREQ08p.state = 0x1a;
    JANY_NR24_FREQ09p.state = 0x1a;
    JUPY_NR24_FREQ10p.state = 0x1a;
    DANE_CH2_ACTIVEp.state = 0x18;
    JORE_ENV_DELAY0p.state = 0x18;
    JONA_ENV_DELAY1p.state = 0x19;
    JEVY_ENV_DELAY2p.state = 0x1a;
    JOPA_ENV_TICK.state = 0x1a;
    DONE_COUNT00.state = 0x1a;
    DYNU_COUNT01.state = 0x18;
    EZOF_COUNT02.state = 0x18;
    CYVO_COUNT03.state = 0x18;
    FUXO_COUNT04.state = 0x18;
    GANO_COUNT05.state = 0x18;
    GOCA_COUNT06.state = 0x18;
    GANE_COUNT07.state = 0x18;
    HEVY_COUNT08.state = 0x18;
    HEPU_COUNT09.state = 0x18;
    HERO_COUNT10.state = 0x18;
    DOME_CH2_OUTp.state = 0x18;
    ELOX_TRIGp_d01.state = 0x18;
    CAZA_TRIGp_d17.state = 0x18;
    EMER_NR24_LENENp.state = 0x1a;
    DOPE_TRIGp_d00.state = 0x18;
    DALA_TRIGp_d00.state = 0x19;
    ETAP_NR24_TRIGp.state = 0x1a;
    DORY_TRIGp_d09.state = 0x18;
    GYKO_SAMPLE_CLKp.state = 0x1a;
    CANO_PHASE0.state = 0x1a;
    CAGY_PHASE1.state = 0x1a;
    DYVE_PHASE2.state = 0x1a;
    BUTA_FREQ_GATEp.state = 0x19;
    FENA_ENV_VOL3.state = 0x1a;
    FOMY_ENV_VOL2.state = 0x1a;
    FETE_ENV_VOL1.state = 0x1a;
    FENO_ENV_VOL0.state = 0x1a;
    HEPO_ENV_MAXp.state = 0x18;
    JEME_ENV_DONEp.state = 0x18;
    JYNA_ENV_CLK64.state = 0x18;
    CYRE_LEN_DONEp.state = 0x1a;
  }

  //----------
  // FF16

  /*_p15.ERYC*/ DFF20 ERYC_NR21_LEN0;
  /*_p15.CERA*/ DFF20 CERA_NR21_LEN1;
  /*_p15.CONU*/ DFF20 CONU_NR21_LEN2;
  /*_p15.CAME*/ DFF20 CAME_NR21_LEN3;
  /*_p15.BUVA*/ DFF20 BUVA_NR21_LEN4;
  /*_p15.AKYD*/ DFF20 AKYD_NR21_LEN5;
  /*_p14.BERA*/ DFF9 BERA_NR21_DUTY0;
  /*_p14.BAMY*/ DFF9 BAMY_NR21_DUTY1;

  //----------
  // FF17 NR22 - channel 2 vol/env/period

  /*_p14.HYFU*/ DFF9 HYFU_NR22_P0p;
  /*_p14.HORE*/ DFF9 HORE_NR22_P1p;
  /*_p14.HAVA*/ DFF9 HAVA_NR22_P2p;
  /*_p14.FORE*/ DFF9 FORE_NR22_ADDp;
  /*_p14.GATA*/ DFF9 GATA_NR22_V0p;
  /*_p14.GUFE*/ DFF9 GUFE_NR22_V1p;
  /*_p14.GURA*/ DFF9 GURA_NR22_V2p;
  /*_p14.GAGE*/ DFF9 GAGE_NR22_V3p;

  //----------
  // FF18

  DFF9 FOFE_NR23_FREQ00p;
  DFF9 FOVA_NR23_FREQ01p;
  DFF9 FEDY_NR23_FREQ02p;
  DFF9 FOME_NR23_FREQ03p;
  DFF9 FORA_NR23_FREQ04p;
  DFF9 GODA_NR23_FREQ05p;
  DFF9 GUMY_NR23_FREQ06p;
  DFF9 GUPU_NR23_FREQ07p;

  //----------
  // FF19

  DFF9 JEFU_NR24_FREQ08p;
  DFF9 JANY_NR24_FREQ09p;
  DFF9 JUPY_NR24_FREQ10p;
  DFF9 EMER_NR24_LENENp;
  DFF9 ETAP_NR24_TRIGp;

  //----------

  DFF20 DONE_COUNT00;
  DFF20 DYNU_COUNT01; //o30
  DFF20 EZOF_COUNT02;
  DFF20 CYVO_COUNT03;
  DFF20 FUXO_COUNT04;
  DFF20 GANO_COUNT05;
  DFF20 GOCA_COUNT06;
  DFF20 GANE_COUNT07;
  DFF20 HEVY_COUNT08;
  DFF20 HEPU_COUNT09;
  DFF20 HERO_COUNT10;

  /*#p15.ELOX*/ DFF17     ELOX_TRIGp_d01; // o40
  /*#p15.CAZA*/ DFF17     CAZA_TRIGp_d17;
  /*#p15.DOPE*/ DFF17     DOPE_TRIGp_d00;
  /*#p15.DALA*/ NorLatch  DALA_TRIGp_d00;
  /*#p15.DORY*/ DFF17     DORY_TRIGp_d09;

  /*_p15.CYRE*/ DFF17     CYRE_LEN_DONEp;

  /*#p15.JYNA*/ DFF17     JYNA_ENV_CLK64;
  /*#p15.JOPA*/ DFF17     JOPA_ENV_TICK;
  /*#p15.HEPO*/ DFF17     HEPO_ENV_MAXp;
  /*#p15.JEME*/ NorLatch  JEME_ENV_DONEp;

  DFF20 JORE_ENV_DELAY0p; // o50
  DFF20 JONA_ENV_DELAY1p;
  DFF20 JEVY_ENV_DELAY2p;

  DFF20 FENO_ENV_VOL0;
  DFF20 FETE_ENV_VOL1;
  DFF20 FOMY_ENV_VOL2;
  DFF20 FENA_ENV_VOL3;

  /*#p15.CANO*/ DFF17     CANO_PHASE0;
  /*#p15.CAGY*/ DFF13     CAGY_PHASE1;
  /*#p15.DYVE*/ DFF13     DYVE_PHASE2;

  /*_p15.DANE*/ NorLatch  DANE_CH2_ACTIVEp;
  /*_p15.BUTA*/ NandLatch BUTA_FREQ_GATEp;
  /*#p15.DOME*/ DFF17     DOME_CH2_OUTp;
  /*#p15.GYKO*/ DFF17     GYKO_SAMPLE_CLKp;
};
