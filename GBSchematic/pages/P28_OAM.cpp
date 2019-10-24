#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P28_OAM {
  
  struct Input {
    bool XUPY;
    bool ABEZ;
    bool P10_B;
    bool CLK3;
    bool RESET_VIDEO;
    bool CPU_RD2;
    bool CPU_WR2;
    bool FF40;
    bool MOPA_PHI;
    bool SARO;
    bool CATY;
    bool XYMU;
    bool AVAP;
    bool CATU;
    bool MATU_Q;
    bool TYFO;
    bool TUVO;
    bool XOCE;
    bool VRAM_TO_OAM;
    bool WYJA;
    bool RESET7n;
    bool XARE;

    bool OAM_ADDR_DMA; // P24

    bool WEZA,WUCO,WYDA,ZYSU,WYSE,WUZY;

    bool A0,A1,A2,A3,A4,A5,A6,A7;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;

    bool DMA_A0,DMA_A1,DMA_A2,DMA_A3,DMA_A4,DMA_A5,DMA_A6,DMA_A7;
  };

  struct Output {
    bool CLK3;
    bool OAM_CLK;
    bool RESET_VIDEO2n;
    bool OAM_ADDR_RENDER;
    bool OAM_ADDR_PARSE;
    bool OAM_ADDR_CPU;

    bool OAM_A_CS;
    bool OAM_B_CS;
    bool OAM_A1,OAM_A2,OAM_A3,OAM_A4,OAM_A5,OAM_A6,OAM_A7;

    // actual pins to oam
    bool OAM_D_OE;
    bool OAM_A_D0,OAM_A_D1,OAM_A_D2,OAM_A_D3,OAM_A_D4,OAM_A_D5,OAM_A_D6,OAM_A_D7;
    bool OAM_B_D0,OAM_B_D1,OAM_B_D2,OAM_B_D3,OAM_B_D4,OAM_B_D5,OAM_B_D6,OAM_B_D7;
  };

  reg YFEL,WEWY,GOSO,ELYN,FAHA,FONY;
  reg ANEL;
  reg XECY,XUVA;

  void tick(const Input& in, Output& out) {

    //----------

    wire WEFE = not(in.P10_B);
    wire YVAL = not(in.CLK3);
    wire YRYV = not(YVAL);
    wire ZODO = not(YRYV);
    wire ATAR = not(in.RESET_VIDEO);

    out.OAM_CLK = ZODO;
    out.RESET_VIDEO2n = ATAR;

    //----------

    wire AWOH = not(in.XUPY);
    wire ABAF = not(in.CATU);
    wire ANEL_Q = ANEL.tock(AWOH, in.ABEZ, in.CATU);
    wire BYHA = unk3(ANEL_Q, ABAF, in.ABEZ);
    wire ATEJ = not(BYHA);
    wire AMYG = not(in.RESET_VIDEO);
    wire ANOM = nor(out.RESET_VIDEO2n, ATEJ);
    wire AZYB = not(ATEJ);
    wire ABAK = or(ATEJ, AMYG);
    wire BYVA = not(ABAK);

    //----------
    // sprite scan counter

    wire YFEL_Q = YFEL.q();
    wire WEWY_Q = WEWY.q();
    wire GOSO_Q = GOSO.q();
    wire ELYN_Q = ELYN.q();
    wire FAHA_Q = FAHA.q();
    wire FONY_Q = FONY.q();

    wire FETO = and(YFEL_Q, WEWY_Q, FONY_Q, GOSO_Q);
    wire GAVA = or(FETO, in.XUPY);

    YFEL.flip(GAVA,    ANOM);
    WEWY.flip(!YFEL_Q, ANOM);
    GOSO.flip(!WEWY_Q, ANOM);
    ELYN.flip(!GOSO_Q, ANOM);
    FAHA.flip(!ELYN_Q, ANOM);
    FONY.flip(!FAHA_Q, ANOM);

    //----------

    wire ASEN = or(out.RESET_VIDEO2n, in.AVAP);
    wire BOGE = not(in.MATU_Q);
    wire BESU = unk2(in.CATU, ASEN);
    wire AJON = and(in.XYMU, BOGE);
    wire ACYL = and(BOGE, BESU);
    wire BETE = not(AJON);
    wire APAR = not(ACYL);
    wire AJUJ = nor(in.MATU_Q, ACYL, AJON);
    wire ASAM = or(ACYL, in.XYMU, in.MATU_Q);
    wire XYNY = not(in.MOPA_PHI);
    wire XUTO = and(in.SARO, in.CPU_WR2);
    wire AMAB = and(in.SARO, AJUJ);
    wire ADAH = not(in.SARO);
    wire WUJE = unk2(XYNY, XUTO);
    wire XUPA = not(WUJE);
    wire APAG = amux2(XUPA, AMAB, AJUJ, ADAH);

    wire WEFY = and(in.TUVO, in.TYFO);
    wire BOFE = not(in.CATY);
    wire AJEP = and(ACYL, in.XOCE);
    wire XUJA = not(WEFY);
    wire BOTA = nor(BOFE, in.SARO, in.CPU_RD2);
    wire ASYT = and(AJEP, XUJA, BOTA);
    wire BODE = not(ASYT);

    out.CLK3 = BODE;

    //----------

    wire WARU = and(in.FF40, in.CPU_WR2);
    wire XUCA = not(WARU);

    out.OAM_CLK = ZODO;
    out.OAM_ADDR_RENDER = BETE;
    out.OAM_ADDR_PARSE = APAR;
    out.OAM_ADDR_CPU = ASAM;

    wire ZAXA = not(in.D0);
    wire ZAMY = not(in.D0);
    wire ZAKY = not(in.D1);
    wire ZOPU = not(in.D1);
    wire WULE = not(in.D2);
    wire WYKY = not(in.D2);
    wire ZOZO = not(in.D3);
    wire ZAJA = not(in.D3);

    wire ZUFO = not(in.D4);
    wire ZUGA = not(in.D4);
    wire ZATO = not(in.D5);
    wire ZUMO = not(in.D5);
    wire YVUC = not(in.D6);
    wire XYTO = not(in.D6);
    wire ZUFE = not(in.D7);
    wire ZYFA = not(in.D7);

    wire AZUL = not(APAG);
    if (AZUL) {
      out.OAM_A_D0 = ZAXA;
      out.OAM_B_D0 = ZAMY;
      out.OAM_A_D1 = ZAKY;
      out.OAM_B_D1 = ZOPU;
      out.OAM_A_D2 = WULE;
      out.OAM_B_D2 = WYKY;
      out.OAM_A_D3 = ZOZO;
      out.OAM_B_D3 = ZAJA;

      out.OAM_A_D4 = ZUFO;
      out.OAM_B_D4 = ZUGA;
      out.OAM_A_D5 = ZATO;
      out.OAM_B_D5 = ZUMO;
      out.OAM_A_D6 = YVUC;
      out.OAM_B_D6 = XYTO;
      out.OAM_A_D7 = ZUFE;
      out.OAM_B_D7 = ZYFA;
    }

    wire WUZU = not(in.MD0);
    wire WOWA = not(in.MD0);
    wire AXER = not(in.MD1);
    wire AVEB = not(in.MD1);
    wire ASOX = not(in.MD2);
    wire AMUH = not(in.MD2);
    wire CETU = not(in.MD3);
    wire COFO = not(in.MD3);

    wire ARYN = not(in.MD4);
    wire AZOZ = not(in.MD4);
    wire ACOT = not(in.MD5);
    wire AGYK = not(in.MD5);
    wire CUJE = not(in.MD6);
    wire BUSE = not(in.MD6);
    wire ATER = not(in.MD7);
    wire ANUM = not(in.MD7);

    wire AZAR = not(in.VRAM_TO_OAM);
    if (AZAR) {

      out.OAM_A_D0 = WUZU;
      out.OAM_B_D0 = WOWA;
      out.OAM_A_D1 = AXER;
      out.OAM_B_D1 = AVEB;
      out.OAM_A_D2 = ASOX;
      out.OAM_B_D2 = AMUH;
      out.OAM_A_D3 = CETU;
      out.OAM_B_D3 = COFO;

      out.OAM_A_D4 = ARYN;
      out.OAM_B_D4 = AZOZ;
      out.OAM_A_D5 = ACOT;
      out.OAM_B_D5 = AGYK;
      out.OAM_A_D6 = CUJE;
      out.OAM_B_D6 = BUSE;
      out.OAM_A_D7 = ATER;
      out.OAM_B_D7 = ANUM;
    }

    //----------
    // OAM address generator

    wire FOBY = not(in.A7);
    wire WAXA = not(in.A6);
    wire GERA = not(in.A5);
    wire FEVU = not(in.A4);
    wire WAPE = not(in.A3);
    wire GOSE = not(in.A2);
    wire WACU = not(in.A1);
    wire GARO = not(in.A0);

    wire FYKE = not(in.WEZA);
    wire FUGU = not(in.WUCO);
    wire FACO = not(in.WYDA);
    wire FABY = not(in.ZYSU);
    wire GYKA = not(in.WYSE);
    wire GYBU = not(in.WUZY);
    wire WYDU = not(WEFE);
    wire GECA = not(WEFE);

    wire GOBY = not(FONY_Q);
    wire GAMA = not(FAHA_Q);
    wire FAKU = not(ELYN_Q);
    wire FUTO = not(GOSO_Q);
    wire GEMA = not(WEWY_Q);
    wire GUSE = not(YFEL_Q);
    wire WUWE = not(in.P10_B);
    wire GEFY = not(in.P10_B);

    wire FETU = not(in.DMA_A7);
    wire FYDU = not(in.DMA_A6);
    wire EDOL = not(in.DMA_A5);
    wire ELUG = not(in.DMA_A4);
    wire FYKY = not(in.DMA_A3);
    wire FAGO = not(in.DMA_A2);
    wire FESA = not(in.DMA_A1);
    wire FODO = not(in.DMA_A0);

    wire YZET = not((FOBY & out.OAM_ADDR_CPU) | (FYKE & out.OAM_ADDR_RENDER) | (GOBY & out.OAM_ADDR_PARSE) | (FETU & in.OAM_ADDR_DMA));
    wire XEMU = not((WAXA & out.OAM_ADDR_CPU) | (FUGU & out.OAM_ADDR_RENDER) | (GAMA & out.OAM_ADDR_PARSE) | (FYDU & in.OAM_ADDR_DMA));
    wire YMEV = not((GERA & out.OAM_ADDR_CPU) | (FACO & out.OAM_ADDR_RENDER) | (FAKU & out.OAM_ADDR_PARSE) | (EDOL & in.OAM_ADDR_DMA));
    wire YVOM = not((FEVU & out.OAM_ADDR_CPU) | (FABY & out.OAM_ADDR_RENDER) | (FUTO & out.OAM_ADDR_PARSE) | (ELUG & in.OAM_ADDR_DMA));
    wire YFOC = not((WAPE & out.OAM_ADDR_CPU) | (GYKA & out.OAM_ADDR_RENDER) | (GEMA & out.OAM_ADDR_PARSE) | (FYKY & in.OAM_ADDR_DMA));
    wire YFOT = not((GOSE & out.OAM_ADDR_CPU) | (GYBU & out.OAM_ADDR_RENDER) | (GUSE & out.OAM_ADDR_PARSE) | (FAGO & in.OAM_ADDR_DMA));
    wire ZYFO = not((WACU & out.OAM_ADDR_CPU) | (WYDU & out.OAM_ADDR_RENDER) | (WUWE & out.OAM_ADDR_PARSE) | (FESA & in.OAM_ADDR_DMA));
    wire GEKA = not((GARO & out.OAM_ADDR_CPU) | (GECA & out.OAM_ADDR_RENDER) | (GEFY & out.OAM_ADDR_PARSE) | (FODO & in.OAM_ADDR_DMA));

    wire MYNU = nand(in.CPU_RD2, in.CATY);
    wire LEKO = not(MYNU);
    wire WAFO = not(GEKA);
    wire GUKO = and(WAFO, AMAB, LEKO);
    wire WUKU = and(LEKO, AMAB, GEKA);
    wire YLYC = and(in.WYJA, GEKA);
    wire YNYC = and(in.WYJA, WAFO);

    wire WUME = not(GUKO);
    wire WEWU = not(WUKU);
    wire ZONE = not(YLYC);
    wire ZOFE = not(YNYC);

    wire XECY_Q = XECY.tock(XUCA, 0, in.RESET7n); // ? weird
    XUVA.tock(XYNY, in.XARE, XECY_Q);

    out.OAM_A_CS = ZONE;
    out.OAM_B_CS = ZOFE;
    out.OAM_A7 = YZET;
    out.OAM_A6 = XEMU;
    out.OAM_A5 = YMEV;
    out.OAM_A4 = YVOM;
    out.OAM_A3 = YFOC;
    out.OAM_A2 = YFOT;
    out.OAM_A1 = ZYFO;
  }
};