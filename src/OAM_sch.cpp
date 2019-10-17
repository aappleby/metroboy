// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern reg MATU;
extern bool MOPA_PHI;
extern bool P10_B;
extern bool VRAM_TO_OAM;
extern bool OAM_ADDR_DMA;

extern bool SARO;
extern bool TUVO;
extern bool XYMU;
extern bool CATY;
extern bool AVAP;
extern bool XOCE;

extern reg CATU;

extern reg TYFO;
extern bool XUPY;
extern bool CLK3;
extern bool RESET_VIDEO;
extern bool RESET_VIDEO2n;
extern bool ABEZ;
extern bool CPU_WR2;
extern bool CPU_RD2;
extern bool WARU;
extern bool RESET7n;
extern bool XARE;
extern bool WYJA;

//----------
// outputs

bool OAM_CLK;
bool OAM_ADDR_RENDER;
bool OAM_ADDR_PARSE;
bool OAM_ADDR_CPU;
bool OAM_A_CS;
bool OAM_B_CS;

bool AMAB;
bool ATEJ;
bool WEFE;
bool ACYL;
bool ANOM;
bool AZYB;
bool BESU;
bool BYVA;
bool WUME;
bool WEWU;
bool FETO;
bool LEKO;

//----------
// sprite index bus

extern bool WEZA;
extern bool WUCO;
extern bool WYDA;
extern bool ZYSU;
extern bool WYSE;
extern bool WUZY;

//----------
// registers

// sprite scan counter
static reg YFEL;
static reg WEWY;
static reg GOSO;
static reg ELYN;
static reg FAHA;
static reg FONY;

static reg ANEL;
static reg XECY;
static reg XUVA;

//-----------------------------------------------------------------------------
// 28_OAM.png

void tick_oam() {
  //----------
  // sprite scan counter

  bool YFEL_Q = YFEL.q();
  bool WEWY_Q = WEWY.q();
  bool GOSO_Q = GOSO.q();
  bool ELYN_Q = ELYN.q();
  bool FAHA_Q = FAHA.q();
  bool FONY_Q = FONY.q();

  FETO = and(YFEL_Q, WEWY_Q, FONY_Q, GOSO_Q);
  bool GAVA = or(FETO, XUPY);

  YFEL.flip(GAVA,    ANOM);
  WEWY.flip(!YFEL_Q, ANOM);
  GOSO.flip(!WEWY_Q, ANOM);
  ELYN.flip(!GOSO_Q, ANOM);
  FAHA.flip(!ELYN_Q, ANOM);
  FONY.flip(!FAHA_Q, ANOM);

  //----------

  bool CATU_Q = CATU.q();

  WEFE = not(P10_B);
  bool YVAL = not(CLK3);
  bool YRYV = not(YVAL);
  bool ZODO = not(YRYV);

  OAM_CLK = ZODO;

  bool ATAR = not(RESET_VIDEO);
  RESET_VIDEO2n = ATAR;

  bool AWOH = not(XUPY);
  bool ABAF = not(CATU_Q);

  bool ANEL_Q = ANEL.q();
  bool BYHA = unk3(ANEL_Q, ABAF, ABEZ);
  ATEJ = not(BYHA);
  ANOM = nor(RESET_VIDEO2n, ATEJ);
  AZYB = not(ATEJ);
  bool AMYG = not(RESET_VIDEO);
  bool ABAK = or(ATEJ, AMYG);
  BYVA = not(ABAK);

  bool ASEN = or(RESET_VIDEO2n, AVAP);
  bool MATU_Q = MATU.q();
  bool BOGE = not(MATU_Q);
  BESU = unk2(CATU_Q, ASEN);
  bool AJON = and(XYMU, BOGE);
  ACYL = and(BOGE, BESU);
  bool BETE = not(AJON);
  OAM_ADDR_RENDER = BETE;
  bool APAR = not(ACYL);
  OAM_ADDR_PARSE = APAR;
  bool AJUJ = nor(MATU_Q, ACYL, AJON);
  bool ASAM = or(ACYL, XYMU, MATU_Q);
  OAM_ADDR_CPU = ASAM;
  
  bool XYNY = not(MOPA_PHI);
  bool XUTO = and(SARO, CPU_WR2);
  AMAB = and(SARO, AJUJ);
  bool ADAH = not(SARO);

  bool WUJE = unk2(XYNY, XUTO);
  bool XUPA = not(WUJE);

  bool AJEP = and(ACYL, XOCE);
  bool WEFY = and(TUVO, !TYFO.q());
  bool XUJA = not(WEFY);
  bool BOFE = not(CATY);
  bool BOTA = nor(BOFE, SARO, CPU_RD2);
  bool ASYT = and(AJEP, XUJA, BOTA);
  bool BODE = not(ASYT);
  CLK3 = BODE;

  bool XUCA = not(WARU);
  bool APAG = amux2(XUPA, AMAB, AJUJ, ADAH);
  bool AZUL = not(APAG);

  bool ZAXA = not(D0);
  bool ZAMY = not(D0);
  bool ZAKY = not(D1);
  bool ZOPU = not(D1);
  bool WULE = not(D2);
  bool WYKY = not(D2);
  bool ZOZO = not(D3);
  bool ZAJA = not(D3);

  bool ZUFO = not(D4);
  bool ZUGA = not(D4);
  bool ZATO = not(D5);
  bool ZUMO = not(D5);
  bool YVUC = not(D6);
  bool XYTO = not(D6);
  bool ZUFE = not(D7);
  bool ZYFA = not(D7);

  if (AZUL) {
    OAM_A_D0 = ZAXA;
    OAM_B_D0 = ZAMY;
    OAM_A_D1 = ZAKY;
    OAM_B_D1 = ZOPU;
    OAM_A_D2 = WULE;
    OAM_B_D2 = WYKY;
    OAM_A_D3 = ZOZO;
    OAM_B_D3 = ZAJA;

    OAM_A_D4 = ZUFO;
    OAM_B_D4 = ZUGA;
    OAM_A_D5 = ZATO;
    OAM_B_D5 = ZUMO;
    OAM_A_D6 = YVUC;
    OAM_B_D6 = XYTO;
    OAM_A_D7 = ZUFE;
    OAM_B_D7 = ZYFA;
  }

  bool AZAR = not(VRAM_TO_OAM);

  bool WUZU = not(MD0);
  bool WOWA = not(MD0);
  bool AXER = not(MD1);
  bool AVEB = not(MD1);
  bool ASOX = not(MD2);
  bool AMUH = not(MD2);
  bool CETU = not(MD3);
  bool COFO = not(MD3);

  bool ARYN = not(MD4);
  bool AZOZ = not(MD4);
  bool ACOT = not(MD5);
  bool AGYK = not(MD5);
  bool CUJE = not(MD6);
  bool BUSE = not(MD6);
  bool ATER = not(MD7);
  bool ANUM = not(MD7);

  if (AZAR) {
    OAM_A_D0 = WUZU;
    OAM_B_D0 = WOWA;
    OAM_A_D1 = AXER;
    OAM_B_D1 = AVEB;
    OAM_A_D2 = ASOX;
    OAM_B_D2 = AMUH;
    OAM_A_D3 = CETU;
    OAM_B_D3 = COFO;

    OAM_A_D4 = ARYN;
    OAM_B_D4 = AZOZ;
    OAM_A_D5 = ACOT;
    OAM_B_D5 = AGYK;
    OAM_A_D6 = CUJE;
    OAM_B_D6 = BUSE;
    OAM_A_D7 = ATER;
    OAM_B_D7 = ANUM;
  }

  //----------

  bool FOBY = not(A7);
  bool WAXA = not(A6);
  bool GERA = not(A5);
  bool FEVU = not(A4);
  bool WAPE = not(A3);
  bool GOSE = not(A2);
  bool WACU = not(A1);
  bool GARO = not(A0);

  bool FYKE = not(WEZA);
  bool FUGU = not(WUCO);
  bool FACO = not(WYDA);
  bool FABY = not(ZYSU);
  bool GYKA = not(WYSE);
  bool GYBU = not(WUZY);
  bool WYDU = not(WEFE);
  bool GECA = not(WEFE);

  bool GOBY = not(FONY_Q);
  bool GAMA = not(FAHA_Q);
  bool FAKU = not(ELYN_Q);
  bool FUTO = not(GOSO_Q);
  bool GEMA = not(WEWY_Q);
  bool GUSE = not(YFEL_Q);
  bool WUWE = not(P10_B);
  bool GEFY = not(P10_B);

  bool FETU = not(DMA_A7);
  bool FYDU = not(DMA_A6);
  bool EDOL = not(DMA_A5);
  bool ELUG = not(DMA_A4);
  bool FYKY = not(DMA_A3);
  bool FAGO = not(DMA_A2);
  bool FESA = not(DMA_A1);
  bool FODO = not(DMA_A0);

  bool YZET = not((FOBY & OAM_ADDR_CPU) | (FYKE & OAM_ADDR_RENDER) | (GOBY & OAM_ADDR_PARSE) | (FETU & OAM_ADDR_DMA));
  bool XEMU = not((WAXA & OAM_ADDR_CPU) | (FUGU & OAM_ADDR_RENDER) | (GAMA & OAM_ADDR_PARSE) | (FYDU & OAM_ADDR_DMA));
  bool YMEV = not((GERA & OAM_ADDR_CPU) | (FACO & OAM_ADDR_RENDER) | (FAKU & OAM_ADDR_PARSE) | (EDOL & OAM_ADDR_DMA));
  bool YVOM = not((FEVU & OAM_ADDR_CPU) | (FABY & OAM_ADDR_RENDER) | (FUTO & OAM_ADDR_PARSE) | (ELUG & OAM_ADDR_DMA));
  bool YFOC = not((WAPE & OAM_ADDR_CPU) | (GYKA & OAM_ADDR_RENDER) | (GEMA & OAM_ADDR_PARSE) | (FYKY & OAM_ADDR_DMA));
  bool YFOT = not((GOSE & OAM_ADDR_CPU) | (GYBU & OAM_ADDR_RENDER) | (GUSE & OAM_ADDR_PARSE) | (FAGO & OAM_ADDR_DMA));
  bool ZYFO = not((WACU & OAM_ADDR_CPU) | (WYDU & OAM_ADDR_RENDER) | (WUWE & OAM_ADDR_PARSE) | (FESA & OAM_ADDR_DMA));
  bool GEKA = not((GARO & OAM_ADDR_CPU) | (GECA & OAM_ADDR_RENDER) | (GEFY & OAM_ADDR_PARSE) | (FODO & OAM_ADDR_DMA));

  OAM_A7 = YZET;
  OAM_A6 = XEMU;
  OAM_A5 = YMEV;
  OAM_A4 = YVOM;
  OAM_A3 = YFOC;
  OAM_A2 = YFOT;
  OAM_A1 = ZYFO;

  bool MYNU = nand(CPU_RD2, CATY);
  LEKO = not(MYNU);
  bool WAFO = not(GEKA);
  bool GUKO = and(WAFO, AMAB, LEKO);
  bool WUKU = and(LEKO, AMAB, GEKA);
  bool YLYC = and(WYJA, GEKA);
  bool YNYC = and(WYJA, WAFO);

  WUME = not(GUKO);
  WEWU = not(WUKU);
  bool ZONE = not(YLYC);
  bool ZOFE = not(YNYC);

  OAM_A_CS = ZONE;
  OAM_B_CS = ZOFE;

  ANEL.tock(AWOH, ABEZ, CATU_Q);
  bool XECY_Q = XECY.tock(XUCA, 0, RESET7n); // ? weird
  XUVA.tock(XYNY, XARE, XECY_Q);
}

//-----------------------------------------------------------------------------
