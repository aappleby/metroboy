#pragma once

namespace Schematics {

//-----------------------------------------------------------------------------

struct Channel3 {

  // resets

  /*p16.KUHA*/ bool APU_RESETq;

  //----------
  // FF1A NR30

  /*p16.GUXE*/ bool CH3_AMP_ENna;

  //----------
  // FF1B NR31

  //----------
  // FF1C NR32

  /*p16.HODY*/ bool NR32_VOL0;
  /*p16.HUKY*/ bool NR32_VOL1;

  //----------
  // FF1D NR33

  /*p16.KYHO*/ bool FF1D_WRa;
  /*p16.KULY*/ bool FF1D_WRb;

  /*p16.KOGA*/ bool NR33_FREQ0;
  /*p16.JOVY*/ bool NR33_FREQ1;
  /*p16.JAXA*/ bool NR33_FREQ2;
  /*p16.JEFE*/ bool NR33_FREQ3;
  /*p16.JYPO*/ bool NR33_FREQ4;
  /*p16.JOVE*/ bool NR33_FREQ5;
  /*p16.KANA*/ bool NR33_FREQ6;
  /*p16.KOGU*/ bool NR33_FREQ7;

  //----------
  // FF1E NR34

  /*p16.JEMO*/ bool NR34_FREQ8;
  /*p16.JETY*/ bool NR34_FREQ9;
  /*p16.JACY*/ bool NR34_FREQ10;
  /*p16.HOTO*/ bool NR34_STOP;

  //----------


  /*p16.GUCY*/ bool FF1A_WRn;
  



  /*p16.FABO*/ bool CLK_xxCDxxGH;
  /*p16.GAVU*/ bool NR34_START;

  /*p16.GARA*/ bool GARA;
  /*p16.GYTA*/ bool GYTA;
  /*p16.GYRA*/ bool GYRA;

  /*p16.FOBA*/ bool CH3_RESTART_SYNC;





  /*p16.HACA*/ bool HACA;
  /*p16.EPYX*/ bool FF1E_WRp;
  /*p16.FOVO*/ bool FF1E_WRo;
  /*p16.JUZO*/ bool FF1E_WR;



  /*p16.GUZU*/ bool FF1C_WRn;
  /*p16.HUCO*/ bool HUCO;
  /*p16.HAMU*/ bool HAMU;





  /*p17.ARUC*/ bool ARUC_CLK;
  /*p17.COZY*/ bool COZY_CLK;

  /*p17.BUSA*/ bool BUSA;
  /*p17.BANO*/ bool BANO;
  /*p17.AZUS*/ bool SAMPLE_CLKn;
  /*p17.AZET*/ bool AZET;


  /*p17.BEKA*/ bool SAMPLE_CLKa;
  /*p17.COJU*/ bool SAMPLE_CLKb;
  /*p17.BAJA*/ bool SAMPLE_CLKc;
  /*p17.BUFE*/ bool SAMPLE_CLKd;

  /*p17.DUGU*/ bool WAVE_D0;
  /*p17.DESY*/ bool WAVE_D1;
  /*p17.BATY*/ bool WAVE_D2;
  /*p17.BADE*/ bool WAVE_D3;
  /*p17.BUNE*/ bool WAVE_D4;
  /*p17.BAVA*/ bool WAVE_D5;
  /*p17.DESA*/ bool WAVE_D6;
  /*p17.BEZU*/ bool WAVE_D7;


  /*p17.CYFO*/ bool SAMPLE_0;
  /*p17.CESY*/ bool SAMPLE_1;
  /*p17.BUDY*/ bool SAMPLE_2;
  /*p17.BEGU*/ bool SAMPLE_3;
  /*p17.CUVO*/ bool SAMPLE_4;
  /*p17.CEVO*/ bool SAMPLE_5;
  /*p17.BORA*/ bool SAMPLE_6;
  /*p17.BEPA*/ bool SAMPLE_7;

  /*p18.ERED*/ bool CH3_ACTIVEn;





  /*p18.DERO*/ bool WAVE_CLK;


  /*p18.EFAR*/ bool WAVE_IDX0;
  /*p18.ERUS*/ bool WAVE_IDX1;
  /*p18.EFUZ*/ bool WAVE_IDX2;
  /*p18.EXEL*/ bool WAVE_IDX3;
  /*p18.EFAL*/ bool WAVE_IDX4;
  
  /*p18.FETY*/ bool WAVE_LOOP;


  /*p18.JYFO*/ bool JYFO_CLK;
  /*p18.HUNO*/ bool WAVE_CLKo;


  /*p18.JUTY*/ bool FREQ_CLKa;


  /*p18.KYRU*/ bool FREQ_CLKb;
  /*p18.KESE*/ bool FREQ_CLKc;

  /*p18.KUTU*/ bool FREQ_00;
  /*p18.KUPE*/ bool FREQ_01;
  /*p18.KUNU*/ bool FREQ_02;
  /*p18.KEMU*/ bool FREQ_03;
  /*p18.KYGU*/ bool FREQ_04;
  /*p18.KEPA*/ bool FREQ_05;
  /*p18.KAFO*/ bool FREQ_06;
  /*p18.KENO*/ bool FREQ_07;
  /*p18.KEJU*/ bool FREQ_08;
  /*p18.KEZA*/ bool FREQ_09;
  /*p18.JAPU*/ bool FREQ_10;



  /*p18.GEDO*/ bool GEDO;
  /*p18.FYGO*/ bool FYGO;
  /*p18.FOZU*/ bool FOZU;
  /*p18.EZAS*/ bool EZAS;
  /*p18.DORU*/ bool DORU;
  /*p18.DAVO*/ bool CH3_ACTIVEo;

  /*p18.GENU*/ bool GENU_CLK;
  /*p18.FALU*/ bool FALU_CLK;
  /*p18.GEVO*/ bool CH3_LEN0;
  /*p18.FORY*/ bool CH3_LEN1;
  /*p18.GATU*/ bool CH3_LEN2;
  /*p18.GAPO*/ bool CH3_LEN3;
  /*p18.GEMO*/ bool CH3_LEN4;
  /*p18.FORO*/ bool CH3_LEN5;
  /*p18.FAVE*/ bool CH3_LEN6;
  /*p18.FYRU*/ bool CH3_LEN7;
  /*p18.FEXU*/ bool LEN_DONE;
};

//-----------------------------------------------------------------------------

};