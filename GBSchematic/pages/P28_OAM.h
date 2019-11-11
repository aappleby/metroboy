#pragma once

struct P28_OAM {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool BODE; bool CLK3; };
  union { bool ZODO; bool OAM_CLK; };
  union { bool ATAR; bool RESET_VIDEO2n; };

  union { bool ZODO; bool OAM_CLK; };
  union { bool BETE; bool OAM_ADDR_RENDER; };
  union { bool APAR; bool OAM_ADDR_PARSE; };
  union { bool ASAM; bool OAM_ADDR_CPU; };

  union { bool ZONE; bool OAM_A_CS; };
  union { bool ZOFE; bool OAM_B_CS; };
  union { bool YZET; bool OAM_A7; };
  union { bool XEMU; bool OAM_A6; };
  union { bool YMEV; bool OAM_A5; };
  union { bool YVOM; bool OAM_A4; };
  union { bool YFOC; bool OAM_A3; };
  union { bool YFOT; bool OAM_A2; };
  union { bool ZYFO; bool OAM_A1; };

  union { bool AMAB; };
  union { bool LEKO; };
  union { bool ATEJ; };
  union { bool ACYL; };
  union { bool WEFE; };
  union { bool WEWU; };

private:

  bool YVAL,YRYV;
  bool AWOH,ABAF,ANEL,BYHA,AMYG,ANOM,AZYB,ABAK,BYVA;
  bool FETO,GAVA;
  bool YFEL,WEWY,GOSO,ELYN,FAHA,FONY;
  bool ASEN,BOGE,BESU,AJON,AJUJ,XYNY,XUTO,ADAH,WUJE,XUPA,APAG;
  bool WEFY,BOFE,AJEP,XUJA,BOTA,ASYT;
  bool WARU,XUCA,AZUL,AZAR;
  bool ZAXA,ZAMY,ZAKY,ZOPU,WULE,WYKY,ZOZO,ZAJA;
  bool ZUFO,ZUGA,ZATO,ZUMO,YVUC,XYTO,ZUFE,ZYFA;
  bool WUZU,WOWA,AXER,AVEB,ASOX,AMUH,CETU,COFO;
  bool ARYN,AZOZ,ACOT,AGYK,CUJE,BUSE,ATER,ANUM;
  bool FOBY,WAXA,GERA,FEVU,WAPE,GOSE,WACU,GARO;
  bool FYKE,FUGU,FACO,FABY,GYKA,GYBU,WYDU,GECA;
  bool GOBY,GAMA,FAKU,FUTO,GEMA,GUSE,WUWE,GEFY;
  bool FETU,FYDU,EDOL,ELUG,FYKY,FAGO,FESA,FODO;
  bool GEKA;
  bool MYNU,WAFO,GUKO,WUKU,YLYC,YNYC;
  bool WUME;
  bool XECY,XUVA;

};
