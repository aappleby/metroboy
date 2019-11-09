#pragma once

struct Gameboy;

struct P09_ApuControl {
  // FF24 NR50
  bool APEG,BYGA,AGER,APOS,BYRE,BUMO,COZU,BEDU;

  // FF25 NR51
  bool ANEV,BOGU,BAFO,ATUF,BUME,BOFA,BEFO,BEPU;

  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  //----------

  bool AGUR,AFAT,ATYV,DAPA,KAME;
  bool BATA,CALO,DYFA,AJER;
  bool HAWU,BOPY,HAPO,GUFO,HADA,JYRO,KEPY,KUBY,KEBA;
  bool ETUC,EFOP,FOKU,FERO,EDEK,BOWY,BAZA,CELY,CONE,CATE,AGUZ,KYDU,JURE,HOPE;
  bool BYMA,BEFU,ADAK;
  bool BOSU,BAXY,BUBU,BOWE,ATAF;
  bool AKOD,AWED,AVUD,AXEM,AMAD,ARUX,BOCY,ATUM;
  bool BUPO,BONO,BYFA,GEPA,HEFA,GUMU;
  bool CAPU,CAGA,BOCA,BUZU,CERE,CADA,CAVU,CUDU;

  bool CETO,KAZO,CURU,GAXO;
  bool DOLE,KAMU,DURU,FEWA;
  bool COTO,KOGE,EFUS,FATE;

  //----------

  bool FF24;
  bool FF25;
  bool FF26;
};

