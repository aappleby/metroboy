#pragma once

struct Gameboy;

struct P11_Ch1Regs {
  static void tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc);

  //----------
  // inputs

  bool HYKA; // p12
  bool JYKA; // p12
  bool HAVO; // p12
  bool EDUL; // p12
  bool FELY; // p12
  bool HOLU; // p12
  bool HYXU; // p12
  bool HOPO; // p12
  bool DYGY; // p12
  bool EVAB; // p12
  bool AXAN; // p12

  bool GEXU; // p13
  bool FEKU; // p13
  bool COPE; // p13

             //----------
             // outputs

  bool FF10_D0n,FF10_D1n,FF10_D2n,FF10_D3n,FF10_D4n,FF10_D5n,FF10_D6n;

  bool FF11_D6;
  bool FF11_D6n;
  bool FF11_D7;
  bool FF11_D7n;

  bool FF14_D6;
  bool FF14_D6n;

  bool FF10,FF11,FF12,FF13,FF14;

  //----------
  // regs

  bool BANY,ANAZ,BOTU,AVAF,ARAX,ADEK,BANA; // FF10 NR10
  bool CENA,DYCA; // FF11 NR11
  bool JOPU,JENA,JAXO,JATY,JAFY,JUSA,JUZY,JOMA; // FF12 NR12
  bool GAXE,HYFE,JYTY,KYNA,JEMA,HYKE,FEVA,EKOV,EMUS,EVAK,COPU; // FF13 NR13
  bool BOKO; // FF14 NR14

             //----------
             // cells

  bool CENU,BUZE,ATYN,ASOP;
  bool AMYD,AZYP,AWOS,AFOX,ATAX,AVEK,AKUX;
  bool COVU,CEPO,DAFO,BUWA,BEXU,BOWO,CUDA;

  bool GAXU,HATO,GAGO,KAGY,HOCU;
  bool JYSE,HEVE,HEWA,HOWU,HONO;
  bool HAXE,HAFU,HAMY,KYGY;
  bool JYNE,JACA,JOKU;

  bool FULO,EPYK,GEKU,FUME;
  bool DEGA,KYPE,DAKO,DERU;
  bool FORU,GEFU,KYVU,KUMO,KARY,GODE,GOJE,FOZE;
  bool CEGE,DAXA,CACA,DYPU,EVAJ;
  bool CURE,DUPY,DOPA,DEMU,DEXO;

  bool BAGE,CAMY,BUDA,BALE,BYTU;
};



