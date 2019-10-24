#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P27_WindowMapLookup {
  struct Input {
    bool CLK2;
    bool RESET_VIDEO;
    bool INT_VBL;

    bool ATEJ;
    bool ROXO;
    bool PECU;
    bool POKY;
    bool WUKO;
    bool XYMU;
    bool NYKA;
    bool TALU;
    bool PORY;
    bool TOMU;
    bool SEGU;
    bool AVAP;
    bool LOBY;
    bool WUTY;
    bool FEPO;
    bool LAPE;

    bool CUBA1;

    bool NEZO,NORY,NONO,PASE,MYLO,PUWU,PUHO,NYTU;

    bool V0,V1,V2,V3,V4,V5,V6,V7;

    bool FF40_D0,FF40_D1,FF40_D2,FF40_D3,FF40_D4,FF40_D5,FF40_D6,FF40_D7; // FF40 LCDC
    bool FF43_D0,FF43_D1,FF43_D2,FF43_D3,FF43_D4,FF43_D5,FF43_D6,FF43_D7; // FF43 SCX
    bool FF4A_D0,FF4A_D1,FF4A_D2,FF4A_D3,FF4A_D4,FF4A_D5,FF4A_D6,FF4A_D7; // FF4A WY
    bool FF4B_D0,FF4B_D1,FF4B_D2,FF4B_D3,FF4B_D4,FF4B_D5,FF4B_D6,FF4B_D7; // FF4B WX

    bool XEHO,SAVY,XODU,XYDO,TUHU,TUKY,TAKO,SYBE; // X counter regs on P21
  };

  struct Output {
    bool RESET_VIDEOn;

    bool MA_OE;
    bool MA0,MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12;
  };

  // 3-bit counter
  reg RYKU,ROGA,RUBU;

  // misc
  reg NYZE,PUXA,RYFA,SOVY,RENE,NOPA;
  reg SOBU,SUDA;
  reg LYZU,LAXU,MESU,NYVA,LOVY;
  reg VYNO,VUJO,VYMU,TUFU,TAXA,TOZO,TATE,TEKE;
  reg WYKA,WODY,WOBO,WYKO,XOLO;
  reg PYCO,NUNU;
  reg SARY;

  void tick(const Input& in, Output& out) {
    wire PYRY = not(in.RESET_VIDEO);
    out.RESET_VIDEOn = PYRY;

    //----------
    // Window Y match

    wire NOJO = xor(in.FF4A_D4, in.V4);
    wire PAGA = xor(in.FF4A_D5, in.V5);
    wire PEZO = xor(in.FF4A_D6, in.V6);
    wire NUPA = xor(in.FF4A_D7, in.V7);
    wire NAZE = xor(in.FF4A_D0, in.V0);
    wire PEBO = xor(in.FF4A_D1, in.V1);
    wire POMO = xor(in.FF4A_D2, in.V2);
    wire NEVU = xor(in.FF4A_D3, in.V3);

    wire PALO = nand(in.FF40_D5, NOJO, PAGA, PEZO, NUPA);
    wire NELE = not(PALO);
    wire PAFU = nand(NELE, NAZE, PEBO, POMO, NEVU);
    wire ROGE = not(PAFU);

    wire VYPO = not(in.CUBA1);

    wire SARY_Q = SARY.tock(in.TALU, in.RESET_VIDEO, ROGE);

    //----------
    // Window X match

    wire REPU = or(in.INT_VBL, PYRY);
    wire REJO = unk2(SARY_Q, REPU);

    wire XEHO_Q = in.XEHO;
    wire SAVY_Q = in.SAVY;
    wire XODU_Q = in.XODU;
    wire XYDO_Q = in.XYDO;
    wire TUHU_Q = in.TUHU;
    wire TUKY_Q = in.TUKY;
    wire TAKO_Q = in.TAKO;
    wire SYBE_Q = in.SYBE;

    wire NEZO = xor(TUHU_Q, in.FF4B_D4);
    wire NORY = xor(TUKY_Q, in.FF4B_D5);
    wire NONO = xor(TAKO_Q, in.FF4B_D6);
    wire PASE = xor(SYBE_Q, in.FF4B_D7);
    wire MYLO = xor(XEHO_Q, in.FF4B_D0);
    wire PUWU = xor(SAVY_Q, in.FF4B_D1);
    wire PUHO = xor(XODU_Q, in.FF4B_D2);
    wire NYTU = xor(XYDO_Q, in.FF4B_D3);

    wire PUKY = nand(REJO, NEZO, NORY, NONO, PASE);
    wire NUFA = not(PUKY);
    wire NOGY = nand(NUFA, MYLO, PUWU, PUHO, NYTU);
    wire NUKO = not(NOGY);

    //----------
    // something to do with fine x scroll?

    wire NOPA_Q = NOPA.q();
    wire PYCO_Q = PYCO.q();
    wire NUNU_Q = NUNU.q();
    wire RYFA_Q = RYFA.q();
    wire SOVY_Q = SOVY.q();
    wire RENE_Q = RENE.q();

    wire RYKU_Q = RYKU.q();
    wire ROGA_Q = ROGA.q();
    wire RUBU_Q = RUBU.q();
    wire NYZE_Q = NYZE.q();
    wire PUXA_Q = PUXA.q();

    wire ROZE = nand(RYKU_Q, ROGA_Q, RUBU_Q);
    wire POVA = and(!NYZE_Q, PUXA_Q);
    wire PAHA = not(in.XYMU);
    wire ROXY = unk2(PAHA, POVA);
    wire SUHA = xor(in.FF43_D0, RYKU_Q);
    wire SYBY = xor(in.FF43_D1, ROGA_Q);
    wire SOZU = xor(in.FF43_D2, RUBU_Q);
    wire PECU = nand(in.ROXO, ROZE);
    wire RONE = nand(ROXY, SUHA, SYBY, SOZU);
    wire POHU = not(RONE);
    wire MOXE = not(in.CLK2);
    wire PANY = nor(NUKO, ROZE);
    wire SEKO = nor(RENE_Q, !RYFA_Q);
    wire ROMO = not(in.POKY);
    wire SUVU = nand(in.XYMU, ROMO, in.NYKA, in.PORY);
    wire TAVE = not(SUVU);
    wire XAHY = not(in.ATEJ);
    wire XOFO = nand(in.FF40_D5, XAHY, in.RESET_VIDEO);
    wire XACO = not(XOFO);
    wire PYNU = unk2(NUNU_Q, XOFO);
    wire NUNY = and(!NOPA_Q, PYNU);
    wire NOCU = not(PYNU);
    wire PORE = not(NOCU);

    // glitch filter loop, FIXME double check if this logic is correct
    // wire PUKU = nor(NUNY, RYDY);
    // wire RYDY = nor(PUKU, RESET_VIDEOn, PORY);
    wire RYDY = NUNY && !(out.RESET_VIDEOn || in.PORY);

    wire SYLO = not(RYDY);
    wire TUXY = nand(SOVY_Q, SYLO);
    wire SUZU = not(TUXY);
    wire TEVO = nor(SEKO, SUZU, TAVE);
    wire PASO = nor(TEVO, PAHA);
    wire VETU = and(TEVO, PORE);
    wire ROCO = not(in.SEGU);
    wire MEHE = not(in.CLK2);
    wire NYFO = not(NUNY);
    wire MOSU = not(NYFO);
    wire NYXU = nor(in.AVAP, MOSU, TEVO);
    wire WAZY = not(PORE);
    wire SYNY = not(REPU);

    NOPA.tock(in.CLK2, in.RESET_VIDEO, PYNU);
    PYCO.tock(ROCO, in.RESET_VIDEO, NUKO);
    NUNU.tock(MEHE, in.RESET_VIDEO, PYCO_Q);

    RYKU.flip(PECU, PASO);
    ROGA.flip(!RYKU_Q, PASO);
    RUBU.flip(!ROGA_Q, PASO);

    NYZE.tock(MOXE, in.XYMU, PUXA_Q);
    PUXA.tock(in.ROXO, in.XYMU, POHU);

    RYFA.tock(in.SEGU, in.XYMU, PANY);
    RENE.tock(in.CLK2, in.XYMU, RYFA_Q);
    SOVY.tock(in.CLK2, in.RESET_VIDEO, RYDY);

    //----------
    // dunno, top right

    wire LYZU_Q = LYZU.q();
    wire LAXU_Q = LAXU.q();
    wire MESU_Q = MESU.q();
    wire NYVA_Q = NYVA.q();
    wire LOVY_Q = LOVY.q();

    wire MOCE = nand(LAXU_Q, NYVA_Q, NYXU);
    wire LEBO = nand(in.CLK2, MOCE);
    wire MYVO = not(in.CLK2);
    wire LYRY = not(MOCE);
    wire LAXE = not(LAXU_Q);
    wire MYSO = nor(in.LOBY, LAXE, LYZU_Q);
    wire NAKO = not(MESU_Q);
    wire NOFU = not(NYVA_Q);
    wire MOFU = and(MYSO, NAKO);
    wire NOGU = nand(NAKO, NOFU);
    wire NYDY = nand(MYSO, MESU_Q, NOFU);
    wire XUHA = not(NOFU);
    wire NENY = not(NOGU);
    wire LURY = and(!LOVY_Q, in.XYMU);
    wire LONY = unk2(LURY, !LOVY_Q);
    wire MYMA = not(LONY);
    wire LUSU = not(LONY);
    wire LENA = not(LUSU);
    wire POTU = and(LENA, NENY);
    wire NETA = and(LENA, NOGU);

    LYZU.tock(in.CLK2,    in.XYMU, LAXU_Q);
    LAXU.flip(LEBO,    NYXU);
    MESU.flip(!LAXU_Q, NYXU);
    NYVA.flip(!MESU_Q, NYXU);
    LOVY.tock(MYVO,    NYXU, LYRY);

    //----------
    // address output bus

    wire WYKA_Q = WYKA.flip(   VETU, XACO);
    wire WODY_Q = WODY.flip(!WYKA_Q, XACO);
    wire WOBO_Q = WOBO.flip(!WODY_Q, XACO);
    wire WYKO_Q = WYKO.flip(!WOBO_Q, XACO);
    wire XOLO_Q = XOLO.flip(!WYKO_Q, XACO);

    wire VYNO_Q = VYNO.flip(   WAZY, SYNY);
    wire VUJO_Q = VUJO.flip(!VYNO_Q, SYNY);
    wire VYMU_Q = VYMU.flip(!VUJO_Q, SYNY);

    wire TUFU_Q = TUFU.flip(!VYMU_Q, SYNY);
    wire TAXA_Q = TAXA.flip(!TUFU_Q, SYNY);
    wire TOZO_Q = TOZO.flip(!TAXA_Q, SYNY);
    wire TATE_Q = TATE.flip(!TOZO_Q, SYNY);
    wire TEKE_Q = TEKE.flip(!TATE_Q, SYNY);

    wire XEJA = not(WYKA_Q);
    wire XAMO = not(WODY_Q);
    wire XAHE = not(WOBO_Q);
    wire XULO = not(WYKO_Q);
    wire WUJU = not(XOLO_Q);

    wire VYTO = not(TUFU_Q);
    wire VEHA = not(TAXA_Q);
    wire VACE = not(TOZO_Q);
    wire VOVO = not(TATE_Q);
    wire VULO = not(TEKE_Q);

    wire VEVY = not(in.FF40_D6);
    wire VEZA = not(VYPO);
    wire VOGU = not(VYPO);

    if (in.WUKO) {
      out.MA_OE;
      out.MA0 = XEJA;
      out.MA1 = XAMO;
      out.MA2 = XAHE;
      out.MA3 = XULO;
      out.MA4 = WUJU;
      out.MA5 = VYTO;
      out.MA6 = VEHA;
      out.MA7 = VACE;
      out.MA8 = VOVO;
      out.MA9 = VULO;
      out.MA10 = VEVY;
      out.MA11 = VEZA;
      out.MA12 = VOGU;
    }

    //----------
    // computes SECA and TAVA

    wire SOBU_Q = SOBU.q();
    wire SUDA_Q = SUDA.q();

    wire RYCE = and(SOBU_Q, !SUDA_Q);
    wire ROSY = not(in.RESET_VIDEO);
    wire SECA = nor(RYCE, ROSY, in.ATEJ);
    wire VEKU = nor(in.WUTY, TAVE);
    wire TAKA = unk2(VEKU, SECA);
    wire TUKU = not(in.TOMU);
    wire SOWO = not(TAKA);
    wire TEKY = and(in.FEPO, TUKU, LYRY, SOWO);
    wire TAVA = not(in.LAPE);
    SOBU.tock(TAVA, VYPO, TEKY);
    SUDA.tock(in.LAPE, VYPO, SOBU_Q);
  }
};