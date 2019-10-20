// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "DmaBus.h"
#include "VramBus.h"
#include "OamBus.h"
#include "PPU.h"
#include "Clocks.h"
#include "CpuBus.h"
#include "AddressDecoder.h"
#include "System.h"
#include "Sprites.h"

//----------
// inputs

extern bool XYMU;

extern reg CATU;
extern reg TYFO;

// sprite index bus

extern bool WEZA;
extern bool WUCO;
extern bool WYDA;
extern bool ZYSU;
extern bool WYSE;
extern bool WUZY;

//-----------------------------------------------------------------------------
// 28_OAM.png

void Sprites::tick() {
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
  wire MATU_Q = dma.MATU.q();

  WEFE = not(ext.P10_B);
  wire YVAL = not(clk.CLK3);
  wire YRYV = not(YVAL);
  wire ZODO = not(YRYV);

  wire AWOH = not(XUPY);
  wire ABAF = not(CATU_Q);

  wire BYHA = unk3(ANEL_Q, ABAF, ABEZ);
  ATEJ = not(BYHA);
  ANOM = nor(rst.RESET_VIDEO2n, ATEJ);
  AZYB = not(ATEJ);
  wire AMYG = not(rst.RESET_VIDEO);
  wire ABAK = or(ATEJ, AMYG);
  BYVA = not(ABAK);

  wire ASEN = or(rst.RESET_VIDEO2n, AVAP);
  wire BOGE = not(MATU_Q);
  BESU = unk2(CATU_Q, ASEN);
  wire AJON = and(XYMU, BOGE);
  ACYL = and(BOGE, BESU);
  wire BETE = not(AJON);
  wire APAR = not(ACYL);
  wire AJUJ = nor(MATU_Q, ACYL, AJON);
  wire ASAM = or(ACYL, XYMU, MATU_Q);

  wire XYNY = not(dma.MOPA_PHI);
  wire XUTO = and(sys.SARO, cpu.CPU_WR2);
  AMAB = and(sys.SARO, AJUJ);
  wire ADAH = not(sys.SARO);

  wire WUJE = unk2(XYNY, XUTO);
  wire XUPA = not(WUJE);

  wire AJEP = and(ACYL, XOCE);
  wire WEFY = and(TUVO, !TYFO.q());
  wire XUJA = not(WEFY);
  wire BOFE = not(dma.CATY);
  wire BOTA = nor(BOFE, sys.SARO, cpu.CPU_RD2);
  wire ASYT = and(AJEP, XUJA, BOTA);
  wire BODE = not(ASYT);
  clk.CLK3 = BODE;

  wire WARU = and(dec.FF40, cpu.CPU_WR2);
  wire XUCA = not(WARU);
  wire APAG = amux2(XUPA, AMAB, AJUJ, ADAH);

  oam.OAM_CLK = ZODO;
  oam.OAM_ADDR_RENDER = BETE;
  oam.OAM_ADDR_PARSE = APAR;
  oam.OAM_ADDR_CPU = ASAM;

  wire AZUL = not(APAG);
  if (AZUL) {
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

    oam.OAM_A_D0 = ZAXA;
    oam.OAM_B_D0 = ZAMY;
    oam.OAM_A_D1 = ZAKY;
    oam.OAM_B_D1 = ZOPU;
    oam.OAM_A_D2 = WULE;
    oam.OAM_B_D2 = WYKY;
    oam.OAM_A_D3 = ZOZO;
    oam.OAM_B_D3 = ZAJA;

    oam.OAM_A_D4 = ZUFO;
    oam.OAM_B_D4 = ZUGA;
    oam.OAM_A_D5 = ZATO;
    oam.OAM_B_D5 = ZUMO;
    oam.OAM_A_D6 = YVUC;
    oam.OAM_B_D6 = XYTO;
    oam.OAM_A_D7 = ZUFE;
    oam.OAM_B_D7 = ZYFA;
  }

  wire AZAR = not(dma.VRAM_TO_OAM);
  if (AZAR) {
    wire WUZU = not(vram.MD0);
    wire WOWA = not(vram.MD0);
    wire AXER = not(vram.MD1);
    wire AVEB = not(vram.MD1);
    wire ASOX = not(vram.MD2);
    wire AMUH = not(vram.MD2);
    wire CETU = not(vram.MD3);
    wire COFO = not(vram.MD3);

    wire ARYN = not(vram.MD4);
    wire AZOZ = not(vram.MD4);
    wire ACOT = not(vram.MD5);
    wire AGYK = not(vram.MD5);
    wire CUJE = not(vram.MD6);
    wire BUSE = not(vram.MD6);
    wire ATER = not(vram.MD7);
    wire ANUM = not(vram.MD7);

    oam.OAM_A_D0 = WUZU;
    oam.OAM_B_D0 = WOWA;
    oam.OAM_A_D1 = AXER;
    oam.OAM_B_D1 = AVEB;
    oam.OAM_A_D2 = ASOX;
    oam.OAM_B_D2 = AMUH;
    oam.OAM_A_D3 = CETU;
    oam.OAM_B_D3 = COFO;

    oam.OAM_A_D4 = ARYN;
    oam.OAM_B_D4 = AZOZ;
    oam.OAM_A_D5 = ACOT;
    oam.OAM_B_D5 = AGYK;
    oam.OAM_A_D6 = CUJE;
    oam.OAM_B_D6 = BUSE;
    oam.OAM_A_D7 = ATER;
    oam.OAM_B_D7 = ANUM;
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
  wire WUWE = not(ext.P10_B);
  wire GEFY = not(ext.P10_B);

  wire FETU = not(dma.DMA_A7);
  wire FYDU = not(dma.DMA_A6);
  wire EDOL = not(dma.DMA_A5);
  wire ELUG = not(dma.DMA_A4);
  wire FYKY = not(dma.DMA_A3);
  wire FAGO = not(dma.DMA_A2);
  wire FESA = not(dma.DMA_A1);
  wire FODO = not(dma.DMA_A0);

  wire YZET = not((FOBY & oam.OAM_ADDR_CPU) | (FYKE & oam.OAM_ADDR_RENDER) | (GOBY & oam.OAM_ADDR_PARSE) | (FETU & oam.OAM_ADDR_DMA));
  wire XEMU = not((WAXA & oam.OAM_ADDR_CPU) | (FUGU & oam.OAM_ADDR_RENDER) | (GAMA & oam.OAM_ADDR_PARSE) | (FYDU & oam.OAM_ADDR_DMA));
  wire YMEV = not((GERA & oam.OAM_ADDR_CPU) | (FACO & oam.OAM_ADDR_RENDER) | (FAKU & oam.OAM_ADDR_PARSE) | (EDOL & oam.OAM_ADDR_DMA));
  wire YVOM = not((FEVU & oam.OAM_ADDR_CPU) | (FABY & oam.OAM_ADDR_RENDER) | (FUTO & oam.OAM_ADDR_PARSE) | (ELUG & oam.OAM_ADDR_DMA));
  wire YFOC = not((WAPE & oam.OAM_ADDR_CPU) | (GYKA & oam.OAM_ADDR_RENDER) | (GEMA & oam.OAM_ADDR_PARSE) | (FYKY & oam.OAM_ADDR_DMA));
  wire YFOT = not((GOSE & oam.OAM_ADDR_CPU) | (GYBU & oam.OAM_ADDR_RENDER) | (GUSE & oam.OAM_ADDR_PARSE) | (FAGO & oam.OAM_ADDR_DMA));
  wire ZYFO = not((WACU & oam.OAM_ADDR_CPU) | (WYDU & oam.OAM_ADDR_RENDER) | (WUWE & oam.OAM_ADDR_PARSE) | (FESA & oam.OAM_ADDR_DMA));
  wire GEKA = not((GARO & oam.OAM_ADDR_CPU) | (GECA & oam.OAM_ADDR_RENDER) | (GEFY & oam.OAM_ADDR_PARSE) | (FODO & oam.OAM_ADDR_DMA));

  wire MYNU = nand(cpu.CPU_RD2, dma.CATY);
  LEKO = not(MYNU);
  wire WAFO = not(GEKA);
  wire GUKO = and(WAFO, AMAB, LEKO);
  wire WUKU = and(LEKO, AMAB, GEKA);
  wire YLYC = and(dma.WYJA, GEKA);
  wire YNYC = and(dma.WYJA, WAFO);

  WUME = not(GUKO);
  WEWU = not(WUKU);
  wire ZONE = not(YLYC);
  wire ZOFE = not(YNYC);

  ANEL.tock(AWOH, ABEZ, CATU_Q);
  wire XECY_Q = XECY.tock(XUCA, 0, rst.RESET7n); // ? weird
  XUVA.tock(XYNY, rst.XARE, XECY_Q);

  oam.OAM_A_CS = ZONE;
  oam.OAM_B_CS = ZOFE;
  oam.OAM_A7 = YZET;
  oam.OAM_A6 = XEMU;
  oam.OAM_A5 = YMEV;
  oam.OAM_A4 = YVOM;
  oam.OAM_A3 = YFOC;
  oam.OAM_A2 = YFOT;
  oam.OAM_A1 = ZYFO;
}

//-----------------------------------------------------------------------------
