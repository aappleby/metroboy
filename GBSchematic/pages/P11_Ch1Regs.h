#pragma once

struct Gameboy;

struct P11_Ch1Regs {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool BANY; bool FF10_D0n; };
  union { bool ARAX; bool FF10_D1n; };
  union { bool ANAZ; bool FF10_D2n; };
  union { bool AVAF; bool FF10_D3n; };
  union { bool ADEK; bool FF10_D4n; };
  union { bool BANA; bool FF10_D5n; };
  union { bool BOTU; bool FF10_D6n; };
  union { bool CENA; bool FF11_D6; };
  union { bool DYCA; bool FF11_D7; };
  union { bool BOKO; bool FF14_D6; };

private:

  //----------
  // regs

  bool JOPU,JENA,JAXO,JATY,JAFY,JUSA,JUZY,JOMA; // FF12 NR12
  bool GAXE,HYFE,JYTY,KYNA,JEMA,HYKE,FEVA,EKOV,EMUS,EVAK,COPU; // FF13 NR13

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



