/// plait_noparse

#pragma once

//-----------------------------------------------------------------------------

struct Channel3 {

  // resets

  /*_p16.KUHA*/ bool APU_RESETq;

  //----------
  // FF1A NR30

  /*_p16.GUXE*/ bool CH3_AMP_ENna;

  //----------
  // FF1B NR31

  //----------
  // FF1C NR32

  /*_p16.HODY*/ bool NR32_VOL0;
  /*_p16.HUKY*/ bool NR32_VOL1;

  //----------
  // FF1D NR33

  /*_p16.KYHO*/ bool FF1D_WRa;
  /*_p16.KULY*/ bool FF1D_WRb;

  /*_p16.KOGA*/ bool NR33_FREQ0;
  /*_p16.JOVY*/ bool NR33_FREQ1;
  /*_p16.JAXA*/ bool NR33_FREQ2;
  /*_p16.JEFE*/ bool NR33_FREQ3;
  /*_p16.JYPO*/ bool NR33_FREQ4;
  /*_p16.JOVE*/ bool NR33_FREQ5;
  /*_p16.KANA*/ bool NR33_FREQ6;
  /*_p16.KOGU*/ bool NR33_FREQ7;

  //----------
  // FF1E NR34

  /*_p16.JEMO*/ bool NR34_FREQ8;
  /*_p16.JETY*/ bool NR34_FREQ9;
  /*_p16.JACY*/ bool NR34_FREQ10;
  /*_p16.HOTO*/ bool NR34_STOP;

  //----------


  /*_p16.GUCY*/ bool FF1A_WRn;




  /*_p16.FABO*/ bool CLK_xxCDxxGH;
  /*_p16.GAVU*/ bool NR34_START;

  /*_p16.GARA*/ bool GARA;
  /*_p16.GYTA*/ bool GYTA;
  /*_p16.GYRA*/ bool GYRA;

  /*_p16.FOBA*/ bool CH3_RESTART_SYNC;





  /*_p16.HACA*/ bool HACA;
  /*_p16.EPYX*/ bool FF1E_WRp;
  /*_p16.FOVO*/ bool FF1E_WRo;
  /*_p16.JUZO*/ bool FF1E_WR;



  /*_p16.GUZU*/ bool FF1C_WRn;
  /*_p16.HUCO*/ bool HUCO;
  /*_p16.HAMU*/ bool HAMU;





  /*_p17.ARUC*/ bool ARUC_CLK;
  /*_p17.COZY*/ bool COZY_CLK;

  /*_p17.BUSA*/ bool BUSA;
  /*_p17.BANO*/ bool BANO;
  /*_p17.AZUS*/ bool SAMPLE_CLKn;
  /*_p17.AZET*/ bool AZET;


  /*_p17.BEKA*/ bool SAMPLE_CLKa;
  /*_p17.COJU*/ bool SAMPLE_CLKb;
  /*_p17.BAJA*/ bool SAMPLE_CLKc;
  /*_p17.BUFE*/ bool SAMPLE_CLKd;

  /*_p17.DUGU*/ bool WAVE_D0;
  /*_p17.DESY*/ bool WAVE_D1;
  /*_p17.BATY*/ bool WAVE_D2;
  /*_p17.BADE*/ bool WAVE_D3;
  /*_p17.BUNE*/ bool WAVE_D4;
  /*_p17.BAVA*/ bool WAVE_D5;
  /*_p17.DESA*/ bool WAVE_D6;
  /*_p17.BEZU*/ bool WAVE_D7;


  /*_p17.CYFO*/ bool SAMPLE_0;
  /*_p17.CESY*/ bool SAMPLE_1;
  /*_p17.BUDY*/ bool SAMPLE_2;
  /*_p17.BEGU*/ bool SAMPLE_3;
  /*_p17.CUVO*/ bool SAMPLE_4;
  /*_p17.CEVO*/ bool SAMPLE_5;
  /*_p17.BORA*/ bool SAMPLE_6;
  /*_p17.BEPA*/ bool SAMPLE_7;

  /*_p18.ERED*/ bool CH3_ACTIVEn;





  /*_p18.DERO*/ bool WAVE_CLK;


  /*_p18.EFAR*/ bool WAVE_IDX0;
  /*_p18.ERUS*/ bool WAVE_IDX1;
  /*_p18.EFUZ*/ bool WAVE_IDX2;
  /*_p18.EXEL*/ bool WAVE_IDX3;
  /*_p18.EFAL*/ bool WAVE_IDX4;

  /*_p18.FETY*/ bool WAVE_LOOP;


  /*_p18.JYFO*/ bool JYFO_CLK;
  /*_p18.HUNO*/ bool WAVE_CLKo;


  /*_p18.JUTY*/ bool FREQ_CLKa;


  /*_p18.KYRU*/ bool FREQ_CLKb;
  /*_p18.KESE*/ bool FREQ_CLKc;

  /*_p18.KUTU*/ bool FREQ_00;
  /*_p18.KUPE*/ bool FREQ_01;
  /*_p18.KUNU*/ bool FREQ_02;
  /*_p18.KEMU*/ bool FREQ_03;
  /*_p18.KYGU*/ bool FREQ_04;
  /*_p18.KEPA*/ bool FREQ_05;
  /*_p18.KAFO*/ bool FREQ_06;
  /*_p18.KENO*/ bool FREQ_07;
  /*_p18.KEJU*/ bool FREQ_08;
  /*_p18.KEZA*/ bool FREQ_09;
  /*_p18.JAPU*/ bool FREQ_10;



  /*_p18.GEDO*/ bool GEDO;
  /*_p18.FYGO*/ bool FYGO;
  /*_p18.FOZU*/ bool FOZU;
  /*_p18.EZAS*/ bool EZAS;
  /*_p18.DORU*/ bool DORU;
  /*_p18.DAVO*/ bool CH3_ACTIVEo;

  /*_p18.GENU*/ bool GENU_CLK;
  /*_p18.FALU*/ bool FALU_CLK;
  /*_p18.GEVO*/ bool CH3_LEN0;
  /*_p18.FORY*/ bool CH3_LEN1;
  /*_p18.GATU*/ bool CH3_LEN2;
  /*_p18.GAPO*/ bool CH3_LEN3;
  /*_p18.GEMO*/ bool CH3_LEN4;
  /*_p18.FORO*/ bool CH3_LEN5;
  /*_p18.FAVE*/ bool CH3_LEN6;
  /*_p18.FYRU*/ bool CH3_LEN7;
  /*_p18.FEXU*/ bool LEN_DONE;

  //----------
  // wave ram bus

  bool WAVE_RAM_CTRL1;
  bool WAVE_RAM_CTRL3;
  bool WAVE_WRn;

  /*_p18.BOLE*/ bool WAVE_A0;
  /*_p18.AGYL*/ bool WAVE_A1;
  /*_p18.AFUM*/ bool WAVE_A2;
  /*_p18.AXOL*/ bool WAVE_A3;

  /*_p18.BARY*/ bool WAVE_DAC0;
  /*_p18.BYKA*/ bool WAVE_DAC1;
  /*_p18.BOPA*/ bool WAVE_DAC2;
  /*_p18.BELY*/ bool WAVE_DAC3;
};

//-----------------------------------------------------------------------------
