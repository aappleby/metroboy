#pragma once

struct Gameboy;

struct P17_WaveRam {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool BYZA; bool WAVE_RAM_WR; };
  union { bool CAZU; bool WAVE_RAM_RDn; };
  union { bool AMYT; bool WAVE_RAM_WRn; };
  union { bool CYBO; bool CYBO_4M; };

  union { bool BUTU; bool BUTU_512K; };       // this net name is wrong, this is not a divided clock
  union { bool ALER; bool WAVE_RAM_CTRL1; };
  union { bool DATE; bool WAVE_PLAY_D0; };
  union { bool DAZY; bool WAVE_PLAY_D1; };
  union { bool CUZO; bool WAVE_PLAY_D2; };
  union { bool COPO; bool WAVE_PLAY_D3; };

private:

  bool BOKE,BENA;
  bool BAMA,ARUC,COZY;
  bool ABUR,BORY,BETA,AZOR,BUKU;
  bool BUSA,BANO,AZUS,AZET;
  bool BOXO,BORU;
  bool ATUR;
  bool BEKA,COJU,BAJA,BUFE;
  bool AKAF,CUTO,BERO,BACA,ADOK,ATEC,CEGU,CUGO;
  bool BEZU,DESA,BAVA,BUNE,BADE,BATY,DESY,DUGU;
  bool ACOR;
  bool BEPA,BORA,CEVO,CUVO,BEGU,BUDY,CESY,CYFO;
};
