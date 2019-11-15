#pragma once

struct Gameboy;

struct P17_WaveRam {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { /*bool BYZA;*/ bool WAVE_RAM_WR; };
  union { /*bool CAZU;*/ bool WAVE_RAM_RDn; };

  union { /*bool BUTU;*/ bool BUTU; };

  union { /*bool ALER;*/ bool WAVE_RAM_CTRL1; };
  union { /*bool AMYT;*/ bool WAVE_RAM_WRn; };

  union { /*bool DATE;*/ bool WAVE_PLAY_D0; };
  union { /*bool DAZY;*/ bool WAVE_PLAY_D1; };
  union { /*bool COPO;*/ bool WAVE_PLAY_D3; };
  union { /*bool CUZO;*/ bool WAVE_PLAY_D2; };

  /*p17.BOKE*/ bool BOKE;
  /*p17.BENA*/ bool BENA;
  /*p17.BAMA*/ bool BAMA;
  /*p17.ARUC*/ bool ARUC;
  /*p17.COZY*/ bool COZY;
  /*p17.ABUR*/ bool ABUR;
  /*p17.BORY*/ bool BORY;
  /*p17.BETA*/ bool BETA;
  /*p17.AZOR*/ bool AZOR;
  /*p17.BUKU*/ bool BUKU;
  /*p17.BUSA*/ bool BUSA;
  /*p17.BANO*/ bool BANO;
  /*p17.AZUS*/ bool AZUS;
  /*p17.AZET*/ bool AZET;
  /*p17.BOXO*/ bool BOXO;
  /*p17.BORU*/ bool BORU;
  /*p17.ATUR*/ bool ATUR;
  /*p17.BEKA*/ bool BEKA;
  /*p17.COJU*/ bool COJU;
  /*p17.BAJA*/ bool BAJA;
  /*p17.BUFE*/ bool BUFE;

  /*p17.CUGO*/ bool WAVE_D0n;
  /*p17.CEGU*/ bool WAVE_D1n;
  /*p17.ATEC*/ bool WAVE_D2n;
  /*p17.ADOK*/ bool WAVE_D3n;
  /*p17.BACA*/ bool WAVE_D4n;
  /*p17.BERO*/ bool WAVE_D5n;
  /*p17.CUTO*/ bool WAVE_D6n;
  /*p17.AKAF*/ bool WAVE_D7n;

  /*p17.DUGU*/ bool WAVE_D0;
  /*p17.DESY*/ bool WAVE_D1;
  /*p17.BATY*/ bool WAVE_D2;
  /*p17.BADE*/ bool WAVE_D3;
  /*p17.BUNE*/ bool WAVE_D4;
  /*p17.BAVA*/ bool WAVE_D5;
  /*p17.DESA*/ bool WAVE_D6;
  /*p17.BEZU*/ bool WAVE_D7;

  /*p17.ACOR*/ bool ACOR;

  /*p17.CYFO*/ bool SAMPLE_0;
  /*p17.CESY*/ bool SAMPLE_1;
  /*p17.BUDY*/ bool SAMPLE_2;
  /*p17.BEGU*/ bool SAMPLE_3;
  /*p17.CUVO*/ bool SAMPLE_4;
  /*p17.CEVO*/ bool SAMPLE_5;
  /*p17.BORA*/ bool SAMPLE_6;
  /*p17.BEPA*/ bool SAMPLE_7;
};
