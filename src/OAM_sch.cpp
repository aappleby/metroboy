// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool MATU_Q;
extern bool MOPA_PHI;
extern bool P10_B;
extern bool VRAM_TO_OAM;
extern bool OAM_ADDR_DMA;

extern bool SARO;
extern bool TUVO;
extern bool XYMU;
extern bool CATY;
extern bool CATU;
extern bool AVAP;
extern bool XOCE;

//----------
// outputs

bool AMAB;

//----------
// mystery signals

extern bool XUPY;
extern bool CLK3;
extern bool OAM_CLK;
extern bool RESET_VIDEO;
extern bool RESET_VIDEO2N;
extern bool ABEZ;
extern bool CPU_WR2;
extern bool TYFO_QN;
extern bool CPU_RD2;
extern bool WARU;
extern bool RESET7N;
extern bool XARE;
extern bool WYJA;

extern bool OAM_ADDR_RENDER;
extern bool OAM_ADDR_PARSE;
extern bool OAM_ADDR_CPU;

extern bool OAM_A_CS;
extern bool OAM_B_CS;

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
static bool YFEL;
static bool WEWY;
static bool GOSO;
static bool ELYN;
static bool FAHA;
static bool FONY;
static bool YFEL_CLK = 0;

static bool ANEL;
static bool ANEL_CLK;

static bool XECY;
static bool XECY_CLK;

static bool XUVA;
static bool XUVA_CLK;

//-----------------------------------------------------------------------------
// 28_OAM.png

void tick_oam() {
  bool FETO = and(YFEL, WEWY, FONY, GOSO);
  bool GAVA = or(FETO, XUPY);

  bool WEFE = not(P10_B);
  bool YVAL = not(CLK3);
  bool YRYV = not(YVAL);
  bool ZODO = not(YRYV);

  OAM_CLK = ZODO;

  bool ATAR = not(RESET_VIDEO);
  RESET_VIDEO2N = ATAR;

  bool AWOH = not(XUPY);
  bool ABAF = not(CATU);

  bool BYHA = unk3(ANEL, ABAF, ABEZ);
  bool ATEJ = not(BYHA);
  bool ANOM = nor(RESET_VIDEO2N, ATEJ);
  bool AZYB = not(ATEJ);
  bool AMYG = not(RESET_VIDEO);
  bool ABAK = or(ATEJ, AMYG);
  bool BYVA = not(ABAK);

  bool ASEN = or(RESET_VIDEO2N, AVAP);
  bool BOGE = not(MATU_Q);
  bool BESU = unk2(CATU, ASEN);
  bool AJON = and(XYMU, BOGE);
  bool ACYL = and(BOGE, BESU);
  bool BETE = not(AJON);
  OAM_ADDR_RENDER = BETE;
  bool APAR = not(ACYL);
  OAM_ADDR_PARSE = APAR;
  bool AJUJ = nor(MATU_Q, ACYL, AJON);
  bool ASAM = or(ACYL, XYMU, MATU_Q);
  
  bool XYNY = not(MOPA_PHI);
  bool XUTO = and(SARO, CPU_WR2);
  AMAB = and(SARO, AJUJ);
  bool ADAH = not(SARO);

  bool WUJE = unk2(XYNY, XUTO);
  bool XUPA = not(WUJE);

  bool AJEP = and(ACYL, XOCE);
  bool WEFY = and(TUVO, TYFO_QN);
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

  bool GOBY = not(FONY);
  bool GAMA = not(FAHA);
  bool FAKU = not(ELYN);
  bool FUTO = not(GOSO);
  bool GEMA = not(WEWY);
  bool GUSE = not(YFEL);
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
  bool LEKO = not(MYNU);
  bool WAFO = not(GEKA);
  bool GUKO = and(WAFO, AMAB, LEKO);
  bool WUKU = and(LEKO, AMAB, GEKA);
  bool YLYC = and(WYJA, GEKA);
  bool YNYC = and(WYJA, WAFO);

  bool WUME = not(GUKO);
  bool WEWU = not(WUKU);
  bool ZONE = not(YLYC);
  bool ZOFE = not(YNYC);

  OAM_A_CS = ZONE;
  OAM_B_CS = ZOFE;

  //---------------------------------------------------------------------------

  bool YFEL_CLK_ = GAVA;
  if (YFEL_CLK && !YFEL_CLK_) {
    // FIXME increment YFEL:WEWY:GOSO:ELYN:FAHA:FONY
  }
  YFEL_CLK = YFEL_CLK_;
  
  bool ANEL_ = ANEL;
  bool ANEL_CLK_ = AWOH;
  if (ANEL_CLK && !ANEL_CLK_) {
    ANEL_ = CATU;
  }
  ANEL_CLK = ANEL_CLK_;
  if (!ABEZ) {
    ANEL_ = 0;
  }

  bool XECY_ = XECY;
  bool XECY_CLK_ = XUCA;
  if (XECY_CLK && !XECY_CLK_) {
    XECY_ = RESET7N;
  }
  XECY_CLK = XECY_CLK_;

  bool XUVA_ = XUVA;
  bool XUVA_CLK_ = XYNY;
  if (XUVA_CLK && !XUVA_CLK_) {
    XUVA = XECY;
  }
  if (!XARE) {
    XUVA = 0;
  }

  if (ANOM) {
    YFEL = 0;
    WEWY = 0;
    GOSO = 0;
    ELYN = 0;
    FAHA = 0;
    FONY = 0;
  }

  ANEL = ANEL_; ANEL_CLK = ANEL_CLK_;
  XECY = XECY_; XECY_CLK = XECY_CLK_;
  XUVA = XUVA_; XUVA_CLK = XUVA_CLK_;

  //---------------------------------------------------------------------------
  // unsunk signals

  (void)ASAM;
  (void)BYVA;
  (void)AZYB;
  (void)WUME;
  (void)WEWU;
}

//-----------------------------------------------------------------------------
