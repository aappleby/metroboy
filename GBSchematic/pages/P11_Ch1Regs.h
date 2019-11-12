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

  bool CEPO;

  union { bool JUSA; bool FF12_D0; };
  union { bool JUZY; bool FF12_D1; };
  union { bool JOMA; bool FF12_D2; };
  union { bool JAFY; bool FF12_D3; bool CH1_ENV_DIR; };
  union { bool JATY; bool FF12_D4; };
  union { bool JAXO; bool FF12_D5; };
  union { bool JENA; bool FF12_D6; };
  union { bool JOPU; bool FF12_D7; };

  // FF13 NR13 - ch1 freq
  union { bool GAXE; bool CH1_FREQ_00; };
  union { bool HYFE; bool CH1_FREQ_01; };
  union { bool JYTY; bool CH1_FREQ_02; };
  union { bool KYNA; bool CH1_FREQ_03; };
  union { bool JEMA; bool CH1_FREQ_04; };
  union { bool HYKE; bool CH1_FREQ_05; };
  union { bool FEVA; bool CH1_FREQ_06; };
  union { bool EKOV; bool CH1_FREQ_07; };
  union { bool EMUS; bool CH1_FREQ_08; };
  union { bool EVAK; bool CH1_FREQ_09; };
  union { bool COPU; bool CH1_FREQ_10; };

private:

  //----------
  // regs


  //----------
  // cells

  bool CENU,BUZE,ATYN,ASOP;
  bool AMYD,AZYP,AWOS,AFOX,ATAX,AVEK,AKUX;
  bool COVU,DAFO,BUWA,BEXU,BOWO,CUDA;

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



