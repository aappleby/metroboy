// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "DmaBus.h"
#include "VramBus.h"
#include "OamBus.h"
#include "PpuRegs.h"
#include "Clocks.h"
#include "CpuBus.h"

//----------
// inputs

extern bool MOPA_PHI;
extern bool VRAM_TO_OAM;

extern bool SARO;
extern bool TUVO;
extern bool XYMU;
extern bool CATY;
extern bool AVAP;
extern bool XOCE;
extern bool XUPY;
extern bool ABEZ;
extern bool WARU;
extern bool XARE;
extern bool WYJA;

extern reg MATU;
extern reg CATU;
extern reg TYFO;

// sprite index bus

extern bool WEZA;
extern bool WUCO;
extern bool WYDA;
extern bool ZYSU;
extern bool WYSE;
extern bool WUZY;

//----------
// outputs

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
// registers

// sprite scan counter
reg YFEL;
reg WEWY;
reg GOSO;
reg ELYN;
reg FAHA;
reg FONY;

reg ANEL;
reg XECY;
reg XUVA;

//-----------------------------------------------------------------------------
// 28_OAM.png

void tick_oam() {
  //----------
  // sprite scan counter

  wire YFEL_Q = YFEL.q();
  wire WEWY_Q = WEWY.q();
  wire GOSO_Q = GOSO.q();
  wire ELYN_Q = ELYN.q();
  wire FAHA_Q = FAHA.q();
  wire FONY_Q = FONY.q();

  FETO = and(YFEL_Q, WEWY_Q, FONY_Q, GOSO_Q);
  wire GAVA = or(FETO, XUPY);

  YFEL.flip(GAVA,    ANOM);
  WEWY.flip(!YFEL_Q, ANOM);
  GOSO.flip(!WEWY_Q, ANOM);
  ELYN.flip(!GOSO_Q, ANOM);
  FAHA.flip(!ELYN_Q, ANOM);
  FONY.flip(!FAHA_Q, ANOM);

  //----------

  wire CATU_Q = CATU.q();
  wire ANEL_Q = ANEL.q();
  wire MATU_Q = MATU.q();

  WEFE = not(P10_B);
  wire YVAL = not(CLK3);
  wire YRYV = not(YVAL);
  wire ZODO = not(YRYV);


  wire ATAR = not(RESET_VIDEO);

  wire AWOH = not(XUPY);
  wire ABAF = not(CATU_Q);

  wire BYHA = unk3(ANEL_Q, ABAF, ABEZ);
  ATEJ = not(BYHA);
  ANOM = nor(RESET_VIDEO2n, ATEJ);
  AZYB = not(ATEJ);
  wire AMYG = not(RESET_VIDEO);
  wire ABAK = or(ATEJ, AMYG);
  BYVA = not(ABAK);

  wire ASEN = or(RESET_VIDEO2n, AVAP);
  wire BOGE = not(MATU_Q);
  BESU = unk2(CATU_Q, ASEN);
  wire AJON = and(XYMU, BOGE);
  ACYL = and(BOGE, BESU);
  wire BETE = not(AJON);
  wire APAR = not(ACYL);
  wire AJUJ = nor(MATU_Q, ACYL, AJON);
  wire ASAM = or(ACYL, XYMU, MATU_Q);

  wire XYNY = not(MOPA_PHI);
  wire XUTO = and(SARO, CPU_WR2);
  AMAB = and(SARO, AJUJ);
  wire ADAH = not(SARO);

  wire WUJE = unk2(XYNY, XUTO);
  wire XUPA = not(WUJE);

  wire AJEP = and(ACYL, XOCE);
  wire WEFY = and(TUVO, !TYFO.q());
  wire XUJA = not(WEFY);
  wire BOFE = not(CATY);
  wire BOTA = nor(BOFE, SARO, CPU_RD2);
  wire ASYT = and(AJEP, XUJA, BOTA);
  wire BODE = not(ASYT);

  wire XUCA = not(WARU);
  wire APAG = amux2(XUPA, AMAB, AJUJ, ADAH);

  wire ZAXA = not(mem.D0);
  wire ZAMY = not(mem.D0);
  wire ZAKY = not(mem.D1);
  wire ZOPU = not(mem.D1);
  wire WULE = not(mem.D2);
  wire WYKY = not(mem.D2);
  wire ZOZO = not(mem.D3);
  wire ZAJA = not(mem.D3);

  wire ZUFO = not(mem.D4);
  wire ZUGA = not(mem.D4);
  wire ZATO = not(mem.D5);
  wire ZUMO = not(mem.D5);
  wire YVUC = not(mem.D6);
  wire XYTO = not(mem.D6);
  wire ZUFE = not(mem.D7);
  wire ZYFA = not(mem.D7);

  wire WUZU = not(MD0);
  wire WOWA = not(MD0);
  wire AXER = not(MD1);
  wire AVEB = not(MD1);
  wire ASOX = not(MD2);
  wire AMUH = not(MD2);
  wire CETU = not(MD3);
  wire COFO = not(MD3);

  wire ARYN = not(MD4);
  wire AZOZ = not(MD4);
  wire ACOT = not(MD5);
  wire AGYK = not(MD5);
  wire CUJE = not(MD6);
  wire BUSE = not(MD6);
  wire ATER = not(MD7);
  wire ANUM = not(MD7);

  OAM_CLK = ZODO;
  OAM_ADDR_RENDER = BETE;
  OAM_ADDR_PARSE = APAR;
  RESET_VIDEO2n = ATAR;
  OAM_ADDR_CPU = ASAM;
  CLK3 = BODE;

  wire AZUL = not(APAG);
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

  wire AZAR = not(VRAM_TO_OAM);
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

  wire FOBY = not(mem.A7);
  wire WAXA = not(mem.A6);
  wire GERA = not(mem.A5);
  wire FEVU = not(mem.A4);
  wire WAPE = not(mem.A3);
  wire GOSE = not(mem.A2);
  wire WACU = not(mem.A1);
  wire GARO = not(mem.A0);

  wire FYKE = not(WEZA);
  wire FUGU = not(WUCO);
  wire FACO = not(WYDA);
  wire FABY = not(ZYSU);
  wire GYKA = not(WYSE);
  wire GYBU = not(WUZY);
  wire WYDU = not(WEFE);
  wire GECA = not(WEFE);

  wire GOBY = not(FONY_Q);
  wire GAMA = not(FAHA_Q);
  wire FAKU = not(ELYN_Q);
  wire FUTO = not(GOSO_Q);
  wire GEMA = not(WEWY_Q);
  wire GUSE = not(YFEL_Q);
  wire WUWE = not(P10_B);
  wire GEFY = not(P10_B);

  wire FETU = not(DMA_A7);
  wire FYDU = not(DMA_A6);
  wire EDOL = not(DMA_A5);
  wire ELUG = not(DMA_A4);
  wire FYKY = not(DMA_A3);
  wire FAGO = not(DMA_A2);
  wire FESA = not(DMA_A1);
  wire FODO = not(DMA_A0);

  wire YZET = not((FOBY & OAM_ADDR_CPU) | (FYKE & OAM_ADDR_RENDER) | (GOBY & OAM_ADDR_PARSE) | (FETU & OAM_ADDR_DMA));
  wire XEMU = not((WAXA & OAM_ADDR_CPU) | (FUGU & OAM_ADDR_RENDER) | (GAMA & OAM_ADDR_PARSE) | (FYDU & OAM_ADDR_DMA));
  wire YMEV = not((GERA & OAM_ADDR_CPU) | (FACO & OAM_ADDR_RENDER) | (FAKU & OAM_ADDR_PARSE) | (EDOL & OAM_ADDR_DMA));
  wire YVOM = not((FEVU & OAM_ADDR_CPU) | (FABY & OAM_ADDR_RENDER) | (FUTO & OAM_ADDR_PARSE) | (ELUG & OAM_ADDR_DMA));
  wire YFOC = not((WAPE & OAM_ADDR_CPU) | (GYKA & OAM_ADDR_RENDER) | (GEMA & OAM_ADDR_PARSE) | (FYKY & OAM_ADDR_DMA));
  wire YFOT = not((GOSE & OAM_ADDR_CPU) | (GYBU & OAM_ADDR_RENDER) | (GUSE & OAM_ADDR_PARSE) | (FAGO & OAM_ADDR_DMA));
  wire ZYFO = not((WACU & OAM_ADDR_CPU) | (WYDU & OAM_ADDR_RENDER) | (WUWE & OAM_ADDR_PARSE) | (FESA & OAM_ADDR_DMA));
  wire GEKA = not((GARO & OAM_ADDR_CPU) | (GECA & OAM_ADDR_RENDER) | (GEFY & OAM_ADDR_PARSE) | (FODO & OAM_ADDR_DMA));

  wire MYNU = nand(CPU_RD2, CATY);
  LEKO = not(MYNU);
  wire WAFO = not(GEKA);
  wire GUKO = and(WAFO, AMAB, LEKO);
  wire WUKU = and(LEKO, AMAB, GEKA);
  wire YLYC = and(WYJA, GEKA);
  wire YNYC = and(WYJA, WAFO);

  WUME = not(GUKO);
  WEWU = not(WUKU);
  wire ZONE = not(YLYC);
  wire ZOFE = not(YNYC);

  ANEL.tock(AWOH, ABEZ, CATU_Q);
  wire XECY_Q = XECY.tock(XUCA, 0, RESET7n); // ? weird
  XUVA.tock(XYNY, XARE, XECY_Q);

  OAM_A_CS = ZONE;
  OAM_B_CS = ZOFE;
  OAM_A7 = YZET;
  OAM_A6 = XEMU;
  OAM_A5 = YMEV;
  OAM_A4 = YVOM;
  OAM_A3 = YFOC;
  OAM_A2 = YFOT;
  OAM_A1 = ZYFO;
}

//-----------------------------------------------------------------------------
