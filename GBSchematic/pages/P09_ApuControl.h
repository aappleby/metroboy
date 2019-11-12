#pragma once

struct Gameboy;

struct P09_ApuControl {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool AJER; bool AJER_2M; };
  union { bool DYFA; bool DYFA_1M; };
  union { bool AGUR; bool APU_RESETn; };
  union { bool AFAT; bool APU_RESET2n; };
  union { bool ATYV; bool APU_RESET3n; };
  union { bool DAPA; bool APU_RESET4n; };
  union { bool KAME; bool APU_RESET5n; };
  union { bool KEBA; bool APU_RESET; };
  union { bool AGUZ; bool CPU_RDn; };
  union { bool EDEK; bool NET03; };
  union { bool FERO; bool FERO_Q; };

private:

  // FF24 NR50
  bool APEG,BYGA,AGER,APOS,BYRE,BUMO,COZU,BEDU;

  // FF25 NR51
  bool ANEV,BOGU,BAFO,ATUF,BUME,BOFA,BEFO,BEPU;

  //----------

  bool BATA,CALO;
  bool HAWU,BOPY,HAPO,GUFO,HADA,JYRO,KEPY,KUBY;
  bool ETUC,EFOP,FOKU,BOWY,BAZA,CELY,CONE,CATE,KYDU,JURE,HOPE;
  bool BYMA,BEFU,ADAK;
  bool BOSU,BAXY,BUBU,BOWE,ATAF;
  bool AKOD,AWED,AVUD,AXEM,AMAD,ARUX,BOCY,ATUM;
  bool BUPO,BONO,BYFA,GEPA,HEFA,GUMU;
  bool CAPU,CAGA,BOCA,BUZU,CERE,CADA,CAVU,CUDU;

  bool CETO,KAZO,CURU,GAXO;
  bool DOLE,KAMU,DURU,FEWA;
  bool COTO,KOGE,EFUS,FATE;
};

