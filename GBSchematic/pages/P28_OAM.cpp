#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P28_OAM::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p28.WEFE = not(b.chip.P10_B);
  c.p28.YVAL = not(b.p28.CLK3);
  c.p28.YRYV = not(b.p28.YVAL);
  c.p28.ZODO = not(b.p28.YRYV);

  //----------

  c.p28.AWOH = not(b.p29.XUPY);
  c.p28.ABAF = not(b.p29.CATU);
  c.p28.ANEL = tock_pos(a.p28.AWOH, b.p28.AWOH, b.p01.VID_RESETn3, b.p28.ANEL, b.p29.CATU);
  c.p28.BYHA = or(b.p28.ANEL, b.p28.ABAF, b.p01.VID_RESETn3);
  c.p28.ATEJ = not(b.p28.BYHA);
  c.p28.ANOM = nor(b.p01.VID_RESET6, b.p28.ATEJ);
  c.p28.AZYB = not(b.p28.ATEJ);
  c.p28.ABAK = or(b.p28.ATEJ, b.p01.VID_RESET7);
  c.p28.BYVA = not(b.p28.ABAK);

  //----------
  // sprite scan counter

  c.p28.FETO = and(b.p28.YFEL, b.p28.WEWY, b.p28.FONY, b.p28.GOSO);
  c.p28.GAVA = or(b.p28.FETO, b.p29.XUPY);

  c.p28.YFEL = tock_pos( a.p28.GAVA,  b.p28.GAVA, b.p28.ANOM, b.p28.YFEL, !b.p28.YFEL);
  c.p28.WEWY = tock_pos(!a.p28.YFEL, !b.p28.YFEL, b.p28.ANOM, b.p28.WEWY, !b.p28.WEWY);
  c.p28.GOSO = tock_pos(!a.p28.WEWY, !b.p28.WEWY, b.p28.ANOM, b.p28.GOSO, !b.p28.GOSO);
  c.p28.ELYN = tock_pos(!a.p28.GOSO, !b.p28.GOSO, b.p28.ANOM, b.p28.ELYN, !b.p28.ELYN);
  c.p28.FAHA = tock_pos(!a.p28.ELYN, !b.p28.ELYN, b.p28.ANOM, b.p28.FAHA, !b.p28.FAHA);
  c.p28.FONY = tock_pos(!a.p28.FAHA, !b.p28.FAHA, b.p28.ANOM, b.p28.FONY, !b.p28.FONY);

  //----------

  c.p28.ASEN = or(b.p01.VID_RESET6, b.p29.AVAP);
  c.p28.BOGE = not(b.p04.MATU);
  c.p28.BESU = unk2(b.p29.CATU, b.p28.ASEN);
  c.p28.AJON = and(b.p21.XYMU, b.p28.BOGE);
  c.p28.ACYL = and(b.p28.BOGE, b.p28.BESU);
  c.p28.BETE = not(b.p28.AJON);
  c.p28.APAR = not(b.p28.ACYL);
  c.p28.AJUJ = nor(b.p04.MATU, b.p28.ACYL, b.p28.AJON);
  c.p28.ASAM = or(b.p28.ACYL, b.p21.XYMU, b.p04.MATU);
  c.p28.XYNY = not(b.p04.PHI_OUT);
  c.p28.XUTO = and(b.p07.ADDR_OAM, b.p07.CPU_WR2);
  c.p28.AMAB = and(b.p07.ADDR_OAM, b.p28.AJUJ);
  c.p28.ADAH = not(b.p07.ADDR_OAM);
  c.p28.WUJE = unk2(b.p28.XYNY, b.p28.XUTO);
  c.p28.XUPA = not(b.p28.WUJE);
  c.p28.APAG = amux2(b.p28.XUPA, b.p28.AMAB, b.p28.AJUJ, b.p28.ADAH);

  c.p28.WEFY = and(b.p29.TUVO, b.p29.TYFO);
  c.p28.BOFE = not(b.p04.FROM_CPU5);
  c.p28.AJEP = and(b.p28.ACYL, b.p29.XOCE);
  c.p28.XUJA = not(b.p28.WEFY);
  c.p28.BOTA = nor(b.p28.BOFE, b.p07.ADDR_OAM, b.p07.CPU_RD2);
  c.p28.ASYT = and(b.p28.AJEP, b.p28.XUJA, b.p28.BOTA);
  c.p28.BODE = not(b.p28.ASYT);

  //----------

  c.p28.WARU = and(b.p22.FF40, b.p07.CPU_WR2);
  c.p28.AZUL = not(b.p28.APAG);
  c.p28.AZAR = not(b.p04.VRAM_TO_OAM);

  c.p28.ZAXA = not(b.D0);
  c.p28.ZAMY = not(b.D0);
  c.p28.ZAKY = not(b.D1);
  c.p28.ZOPU = not(b.D1);
  c.p28.WULE = not(b.D2);
  c.p28.WYKY = not(b.D2);
  c.p28.ZOZO = not(b.D3);
  c.p28.ZAJA = not(b.D3);

  c.p28.ZUFO = not(b.D4);
  c.p28.ZUGA = not(b.D4);
  c.p28.ZATO = not(b.D5);
  c.p28.ZUMO = not(b.D5);
  c.p28.YVUC = not(b.D6);
  c.p28.XYTO = not(b.D6);
  c.p28.ZUFE = not(b.D7);
  c.p28.ZYFA = not(b.D7);

  c.p28.WUZU = not(b.MD0);
  c.p28.WOWA = not(b.MD0);
  c.p28.AXER = not(b.MD1);
  c.p28.AVEB = not(b.MD1);
  c.p28.ASOX = not(b.MD2);
  c.p28.AMUH = not(b.MD2);
  c.p28.CETU = not(b.MD3);
  c.p28.COFO = not(b.MD3);

  c.p28.ARYN = not(b.MD4);
  c.p28.AZOZ = not(b.MD4);
  c.p28.ACOT = not(b.MD5);
  c.p28.AGYK = not(b.MD5);
  c.p28.CUJE = not(b.MD6);
  c.p28.BUSE = not(b.MD6);
  c.p28.ATER = not(b.MD7);
  c.p28.ANUM = not(b.MD7);

  if (b.p28.AZUL) {
    c.OAM_A_D0 = b.p28.ZAXA;
    c.OAM_B_D0 = b.p28.ZAMY;
    c.OAM_A_D1 = b.p28.ZAKY;
    c.OAM_B_D1 = b.p28.ZOPU;
    c.OAM_A_D2 = b.p28.WULE;
    c.OAM_B_D2 = b.p28.WYKY;
    c.OAM_A_D3 = b.p28.ZOZO;
    c.OAM_B_D3 = b.p28.ZAJA;

    c.OAM_A_D4 = b.p28.ZUFO;
    c.OAM_B_D4 = b.p28.ZUGA;
    c.OAM_A_D5 = b.p28.ZATO;
    c.OAM_B_D5 = b.p28.ZUMO;
    c.OAM_A_D6 = b.p28.YVUC;
    c.OAM_B_D6 = b.p28.XYTO;
    c.OAM_A_D7 = b.p28.ZUFE;
    c.OAM_B_D7 = b.p28.ZYFA;
  }

  if (b.p28.AZAR) {
    c.OAM_A_D0 = b.p28.WUZU;
    c.OAM_B_D0 = b.p28.WOWA;
    c.OAM_A_D1 = b.p28.AXER;
    c.OAM_B_D1 = b.p28.AVEB;
    c.OAM_A_D2 = b.p28.ASOX;
    c.OAM_B_D2 = b.p28.AMUH;
    c.OAM_A_D3 = b.p28.CETU;
    c.OAM_B_D3 = b.p28.COFO;

    c.OAM_A_D4 = b.p28.ARYN;
    c.OAM_B_D4 = b.p28.AZOZ;
    c.OAM_A_D5 = b.p28.ACOT;
    c.OAM_B_D5 = b.p28.AGYK;
    c.OAM_A_D6 = b.p28.CUJE;
    c.OAM_B_D6 = b.p28.BUSE;
    c.OAM_A_D7 = b.p28.ATER;
    c.OAM_B_D7 = b.p28.ANUM;
  }

  //----------
  // OAM address generator

  c.p28.FOBY = not(b.A07);
  c.p28.WAXA = not(b.A06);
  c.p28.GERA = not(b.A05);
  c.p28.FEVU = not(b.A04);
  c.p28.WAPE = not(b.A03);
  c.p28.GOSE = not(b.A02);
  c.p28.WACU = not(b.A01);
  c.p28.GARO = not(b.A00);

  c.p28.FYKE = not(b.p30.WEZA);
  c.p28.FUGU = not(b.p30.WUCO);
  c.p28.FACO = not(b.p30.WYDA);
  c.p28.FABY = not(b.p30.ZYSU);
  c.p28.GYKA = not(b.p30.WYSE);
  c.p28.GYBU = not(b.p30.WUZY);
  c.p28.WYDU = not(b.p28.WEFE);
  c.p28.GECA = not(b.p28.WEFE);

  c.p28.GOBY = not(b.p28.FONY);
  c.p28.GAMA = not(b.p28.FAHA);
  c.p28.FAKU = not(b.p28.ELYN);
  c.p28.FUTO = not(b.p28.GOSO);
  c.p28.GEMA = not(b.p28.WEWY);
  c.p28.GUSE = not(b.p28.YFEL);
  c.p28.WUWE = not(b.chip.P10_B);
  c.p28.GEFY = not(b.chip.P10_B);

  c.p28.FETU = not(b.p04.DMA_A07);
  c.p28.FYDU = not(b.p04.DMA_A06);
  c.p28.EDOL = not(b.p04.DMA_A05);
  c.p28.ELUG = not(b.p04.DMA_A04);
  c.p28.FYKY = not(b.p04.DMA_A03);
  c.p28.FAGO = not(b.p04.DMA_A02);
  c.p28.FESA = not(b.p04.DMA_A01);
  c.p28.FODO = not(b.p04.DMA_A00);

  c.p28.YZET = not((b.p28.FOBY & b.p28.OAM_ADDR_CPU) | (b.p28.FYKE & b.p28.OAM_ADDR_RENDER) | (b.p28.GOBY & b.p28.OAM_ADDR_PARSE) | (b.p28.FETU & b.p04.OAM_ADDR_DMA));
  c.p28.XEMU = not((b.p28.WAXA & b.p28.OAM_ADDR_CPU) | (b.p28.FUGU & b.p28.OAM_ADDR_RENDER) | (b.p28.GAMA & b.p28.OAM_ADDR_PARSE) | (b.p28.FYDU & b.p04.OAM_ADDR_DMA));
  c.p28.YMEV = not((b.p28.GERA & b.p28.OAM_ADDR_CPU) | (b.p28.FACO & b.p28.OAM_ADDR_RENDER) | (b.p28.FAKU & b.p28.OAM_ADDR_PARSE) | (b.p28.EDOL & b.p04.OAM_ADDR_DMA));
  c.p28.YVOM = not((b.p28.FEVU & b.p28.OAM_ADDR_CPU) | (b.p28.FABY & b.p28.OAM_ADDR_RENDER) | (b.p28.FUTO & b.p28.OAM_ADDR_PARSE) | (b.p28.ELUG & b.p04.OAM_ADDR_DMA));
  c.p28.YFOC = not((b.p28.WAPE & b.p28.OAM_ADDR_CPU) | (b.p28.GYKA & b.p28.OAM_ADDR_RENDER) | (b.p28.GEMA & b.p28.OAM_ADDR_PARSE) | (b.p28.FYKY & b.p04.OAM_ADDR_DMA));
  c.p28.YFOT = not((b.p28.GOSE & b.p28.OAM_ADDR_CPU) | (b.p28.GYBU & b.p28.OAM_ADDR_RENDER) | (b.p28.GUSE & b.p28.OAM_ADDR_PARSE) | (b.p28.FAGO & b.p04.OAM_ADDR_DMA));
  c.p28.ZYFO = not((b.p28.WACU & b.p28.OAM_ADDR_CPU) | (b.p28.WYDU & b.p28.OAM_ADDR_RENDER) | (b.p28.WUWE & b.p28.OAM_ADDR_PARSE) | (b.p28.FESA & b.p04.OAM_ADDR_DMA));
  c.p28.GEKA = not((b.p28.GARO & b.p28.OAM_ADDR_CPU) | (b.p28.GECA & b.p28.OAM_ADDR_RENDER) | (b.p28.GEFY & b.p28.OAM_ADDR_PARSE) | (b.p28.FODO & b.p04.OAM_ADDR_DMA));

  c.p28.MYNU = nand(b.p07.CPU_RD2, b.p04.FROM_CPU5);
  c.p28.LEKO = not(b.p28.MYNU);
  c.p28.WAFO = not(b.p28.GEKA);
  c.p28.GUKO = and(b.p28.WAFO, b.p28.AMAB, b.p28.LEKO);
  c.p28.WUKU = and(b.p28.LEKO, b.p28.AMAB, b.p28.GEKA);
  c.p28.YLYC = and(b.p04.WYJA, b.p28.GEKA);
  c.p28.YNYC = and(b.p04.WYJA, b.p28.WAFO);

  c.p28.WUME = not(b.p28.GUKO);
  c.p28.WEWU = not(b.p28.WUKU);
  c.p28.ZONE = not(b.p28.YLYC);
  c.p28.ZOFE = not(b.p28.YNYC);

  // these are unused
  /*
  c.p28.XUCA = not(b.p28.WARU);
  c.p28.XECY = tock_pos(a.p28.XUCA, b.p28.XUCA, 0,          b.p28.XECY, b.p01.SYS_RESETn4); // ? weird
  c.p28.XUVA = tock_pos(a.p28.XYNY, b.p28.XYNY, b.p01.XARE, b.p28.XUVA, b.p28.XECY);
  */
}
