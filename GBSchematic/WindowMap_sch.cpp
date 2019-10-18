// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "PpuRegs.h"
#include "VramBus.h"

//----------
// inputs

extern reg NYKA;
extern reg PORY;

extern bool ROXO;
extern bool WUKO;
extern bool LOBY;
extern bool POKY;

extern bool XYMU;
extern bool AVAP;
extern bool CUBA1;
extern bool TALU;
extern bool WUTY;
extern bool TOMU;
extern bool FEPO;
extern bool LAPE;
extern bool ATEJ;
extern bool SEGU;
extern bool CLK2;

extern bool INT_VBL;

// pixel x
extern reg XEHO;
extern reg SAVY;
extern reg XODU;
extern reg XYDO;
extern reg TUHU;
extern reg TUKY;
extern reg TAKO;
extern reg SYBE;

//----------
// outputs

bool RESET_VIDEOn;
bool MOFU;
bool NYDY;
bool NYXU;
bool VYPO;
bool ROXY;
bool LYRY;
bool SECA;
bool POVA;
bool MYVO;
bool SYLO;
bool LENA;
bool MOSU;
bool MYMA;
bool NETA;
bool PORE;
bool POTU;
bool TAVA;
bool XUHA;

//----------
// registers

reg SOBU;
reg SUDA;
reg SARY;
reg NOPA;
reg PYCO;
reg NUNU;

reg RYKU;
reg ROGA;
reg RUBU;
reg NYZE;
reg PUXA;

reg RYFA;
reg SOVY;
reg RENE;
reg LYZU;
reg LAXU;
reg MESU;
reg NYVA;
reg LOVY;

// big address bus, bottom right

reg VYNO;
reg VUJO;
reg VYMU;
reg TUFU;
reg TAXA;
reg TOZO;
reg TATE;
reg TEKE;
reg WYKA;
reg WODY;
reg WOBO;
reg WYKO;
reg XOLO;

//-----------------------------------------------------------------------------

void tick_windowmap() {
  //----------
  // Window Y match

  wire NOJO = xor(FF4A_D4, V4);
  wire PAGA = xor(FF4A_D5, V5);
  wire PEZO = xor(FF4A_D6, V6);
  wire NUPA = xor(FF4A_D7, V7);

  wire NAZE = xor(FF4A_D0, V0);
  wire PEBO = xor(FF4A_D1, V1);
  wire POMO = xor(FF4A_D2, V2);
  wire NEVU = xor(FF4A_D3, V3);

  wire PALO = nand(FF40_D5, NOJO, PAGA, PEZO, NUPA);
  wire NELE = not(PALO);
  wire PAFU = nand(NELE, NAZE, PEBO, POMO, NEVU);
  wire ROGE = not(PAFU);

  VYPO = not(CUBA1);

  wire SARY_Q = SARY.tock(TALU, RESET_VIDEO, ROGE);

  //----------
  // Window X match

  wire PYRY = not(RESET_VIDEO);
  RESET_VIDEOn = PYRY;
  wire REPU = or(INT_VBL, PYRY);
  wire REJO = unk2(SARY_Q, REPU);
  
  bool XEHO_Q = XEHO.q();
  bool SAVY_Q = SAVY.q();
  bool XODU_Q = XODU.q();
  bool XYDO_Q = XYDO.q();
  bool TUHU_Q = TUHU.q();
  bool TUKY_Q = TUKY.q();
  bool TAKO_Q = TAKO.q();
  bool SYBE_Q = SYBE.q();

  wire NEZO = xor(TUHU_Q, FF4B_D4);
  wire NORY = xor(TUKY_Q, FF4B_D5);
  wire NONO = xor(TAKO_Q, FF4B_D6);
  wire PASE = xor(SYBE_Q, FF4B_D7);
  wire MYLO = xor(XEHO_Q, FF4B_D0);
  wire PUWU = xor(SAVY_Q, FF4B_D1);
  wire PUHO = xor(XODU_Q, FF4B_D2);
  wire NYTU = xor(XYDO_Q, FF4B_D3);

  wire PUKY = nand(REJO, NEZO, NORY, NONO, PASE);
  wire NUFA = not(PUKY);
  wire NOGY = nand(NUFA, MYLO, PUWU, PUHO, NYTU);
  wire NUKO = not(NOGY);

  //----------
  // something to do with fine x scroll?

  bool NOPA_Q = NOPA.q();
  bool PYCO_Q = PYCO.q();
  bool NUNU_Q = NUNU.q();
  bool RYFA_Q = RYFA.q();
  bool SOVY_Q = SOVY.q();
  bool RENE_Q = RENE.q();

  bool RYKU_Q = RYKU.q();
  bool ROGA_Q = ROGA.q();
  bool RUBU_Q = RUBU.q();
  bool NYZE_Q = NYZE.q();
  bool PUXA_Q = PUXA.q();

  wire ROZE = nand(RYKU_Q, ROGA_Q, RUBU_Q);
  POVA = and(!NYZE_Q, PUXA_Q);
  wire PAHA = not(XYMU);
  ROXY = unk2(PAHA, POVA);
  wire SUHA = xor(FF43_D0, RYKU_Q);
  wire SYBY = xor(FF43_D1, ROGA_Q);
  wire SOZU = xor(FF43_D2, RUBU_Q);
  wire PECU = nand(ROXO, ROZE);
  wire RONE = nand(ROXY, SUHA, SYBY, SOZU);
  wire POHU = not(RONE);
  wire MOXE = not(CLK2);
  wire PANY = nor(NUKO, ROZE);
  wire SEKO = nor(RENE_Q, !RYFA_Q);
  wire ROMO = not(POKY);
  wire SUVU = nand(XYMU, ROMO, NYKA.q(), PORY.q());
  wire TAVE = not(SUVU);
  wire XAHY = not(ATEJ);
  wire XOFO = nand(FF40_D5, XAHY, RESET_VIDEO);
  wire XACO = not(XOFO);
  wire PYNU = unk2(NUNU_Q, XOFO);
  wire NUNY = and(!NOPA_Q, PYNU);
  wire NOCU = not(PYNU);
  PORE = not(NOCU);

  // glitch filter loop, FIXME double check if this logic is correct
  // bool PUKU = nor(NUNY, RYDY);
  // bool RYDY = nor(PUKU, RESET_VIDEOn, PORY);
  wire RYDY = NUNY && !(RESET_VIDEOn || PORY.q());

  SYLO = not(RYDY);
  wire TUXY = nand(SOVY_Q, SYLO);
  wire SUZU = not(TUXY);
  wire TEVO = nor(SEKO, SUZU, TAVE);
  wire PASO = nor(TEVO, PAHA);
  wire VETU = and(TEVO, PORE);
  wire ROCO = not(SEGU);
  wire MEHE = not(CLK2);
  wire NYFO = not(NUNY);
  MOSU = not(NYFO);
  NYXU = nor(AVAP, MOSU, TEVO);
  wire WAZY = not(PORE);
  wire SYNY = not(REPU);

  NOPA.tock(CLK2, RESET_VIDEO, PYNU);
  PYCO.tock(ROCO, RESET_VIDEO, NUKO);
  NUNU.tock(MEHE, RESET_VIDEO, PYCO_Q);

  //----------
  // dunno, top right

  bool LYZU_Q = LYZU.q();
  bool LAXU_Q = LAXU.q();
  bool MESU_Q = MESU.q();
  bool NYVA_Q = NYVA.q();
  bool LOVY_Q = LOVY.q();

  wire MOCE = nand(LAXU_Q, NYVA_Q, NYXU);
  wire LEBO = nand(CLK2, MOCE);
  MYVO = not(CLK2);
  LYRY = not(MOCE);
  wire LAXE = not(LAXU_Q);
  wire MYSO = nor(LOBY, LAXE, LYZU_Q);
  wire NAKO = not(MESU_Q);
  wire NOFU = not(NYVA_Q);
  MOFU = and(MYSO, NAKO);
  wire NOGU = nand(NAKO, NOFU);
  NYDY = nand(MYSO, MESU_Q, NOFU);
  XUHA = not(NOFU);
  wire NENY = not(NOGU);
  wire LURY = and(!LOVY_Q, XYMU);
  wire LONY = unk2(LURY, !LOVY_Q);
  MYMA = not(LONY);
  wire LUSU = not(LONY);
  LENA = not(LUSU);
  POTU = and(LENA, NENY);
  NETA = and(LENA, NOGU);

  LYZU.tock(CLK2,    XYMU, LAXU_Q);
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

  wire VEVY = not(FF40_D6);
  wire VEZA = not(VYPO);
  wire VOGU = not(VYPO);

  if (WUKO) {
    MA0 = XEJA;
    MA1 = XAMO;
    MA2 = XAHE;
    MA3 = XULO;
    MA4 = WUJU;
    MA5 = VYTO;
    MA6 = VEHA;
    MA7 = VACE;
    MA8 = VOVO;
    MA9 = VULO;
    MA10 = VEVY;
    MA11 = VEZA;
    MA12 = VOGU;
  }
  
  //----------
  // computes SECA and TAVA

  wire SOBU_Q = SOBU.q();
  wire SUDA_Q = SUDA.q();

  wire RYCE = and(SOBU_Q, !SUDA_Q);
  wire ROSY = not(RESET_VIDEO);
  SECA = nor(RYCE, ROSY, ATEJ);
  wire VEKU = nor(WUTY, TAVE);
  wire TAKA = unk2(VEKU, SECA);
  wire TUKU = not(TOMU);
  wire SOWO = not(TAKA);
  wire TEKY = and(FEPO, TUKU, LYRY, SOWO);
  TAVA = not(LAPE);

  //----------
  // registers

  SOBU.tock(TAVA, VYPO, TEKY);
  SUDA.tock(LAPE, VYPO, SOBU_Q);

  RYKU.flip(PECU, PASO);
  ROGA.flip(!RYKU_Q, PASO);
  RUBU.flip(!ROGA_Q, PASO);

  NYZE.tock(MOXE, XYMU, PUXA_Q);
  PUXA.tock(ROXO, XYMU, POHU);

  RYFA.tock(SEGU, XYMU, PANY);
  RENE.tock(CLK2, XYMU, RYFA_Q);
  SOVY.tock(CLK2, RESET_VIDEO, RYDY);
}