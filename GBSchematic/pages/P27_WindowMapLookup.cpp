#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P27_WindowMapLookup::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p27.PYRY = not(b.p01.RESET_VIDEO);

  //----------
  // Window Y match

  c.p27.NAZE = xor(b.p23.FF4A_D0, b.p21.V0);
  c.p27.PEBO = xor(b.p23.FF4A_D1, b.p21.V1);
  c.p27.POMO = xor(b.p23.FF4A_D2, b.p21.V2);
  c.p27.NEVU = xor(b.p23.FF4A_D3, b.p21.V3);
  c.p27.NOJO = xor(b.p23.FF4A_D4, b.p21.V4);
  c.p27.PAGA = xor(b.p23.FF4A_D5, b.p21.V5);
  c.p27.PEZO = xor(b.p23.FF4A_D6, b.p21.V6);
  c.p27.NUPA = xor(b.p23.FF4A_D7, b.p21.V7);

  c.p27.PALO = nand(b.p23.WIN_EN, b.p27.NOJO, b.p27.PAGA, b.p27.PEZO, b.p27.NUPA);
  c.p27.NELE = not(b.p27.PALO);
  c.p27.PAFU = nand(b.p27.NELE, b.p27.NAZE, b.p27.PEBO, b.p27.POMO, b.p27.NEVU);
  c.p27.ROGE = not(b.p27.PAFU);

  // this is not right. where is VYPO/CUBA in the schematic?
  //c.p27.VYPO = not(in.CUBA1);

  // this is what the die looks like
  c.p27.VYPO = not(b.chip.P10_B);

  c.p27.SARY = tock_pos(a.p21.TALU, b.p21.TALU, b.p01.RESET_VIDEO, b.p27.SARY, b.p27.ROGE);

  //----------
  // Window X match

  c.p27.REPU = or(b.p21.INT_VBL, b.p27.PYRY);
  c.p27.REJO = or(b.p27.SARY, b.p27.REPU);

  c.p27.NEZO = xor(b.p21.TUHU, b.p23.FF4B_D4);
  c.p27.NORY = xor(b.p21.TUKY, b.p23.FF4B_D5);
  c.p27.NONO = xor(b.p21.TAKO, b.p23.FF4B_D6);
  c.p27.PASE = xor(b.p21.SYBE, b.p23.FF4B_D7);
  c.p27.MYLO = xor(b.p21.XEHO, b.p23.FF4B_D0);
  c.p27.PUWU = xor(b.p21.SAVY, b.p23.FF4B_D1);
  c.p27.PUHO = xor(b.p21.XODU, b.p23.FF4B_D2);
  c.p27.NYTU = xor(b.p21.XYDO, b.p23.FF4B_D3);

  c.p27.PUKY = nand(b.p27.REJO, b.p27.NEZO, b.p27.NORY, b.p27.NONO, b.p27.PASE);
  c.p27.NUFA = not(b.p27.PUKY);
  c.p27.NOGY = nand(b.p27.NUFA, b.p27.MYLO, b.p27.PUWU, b.p27.PUHO, b.p27.NYTU);
  c.p27.NUKO = not(b.p27.NOGY);

  //----------

  c.p27.ROZE = nand(b.p27.RYKU, b.p27.ROGA, b.p27.RUBU);
  c.p27.POVA = and(!b.p27.NYZE, b.p27.PUXA);
  c.p27.PAHA = not(b.p21.XYMU);
  c.p27.ROXY = unk2(b.p27.PAHA, b.p27.POVA);
  c.p27.SUHA = xor(b.p23.FF43_D0, b.p27.RYKU);
  c.p27.SYBY = xor(b.p23.FF43_D1, b.p27.ROGA);
  c.p27.SOZU = xor(b.p23.FF43_D2, b.p27.RUBU);
  c.p27.PECU = nand(b.p24.ROXO, b.p27.ROZE);
  c.p27.RONE = nand(b.p27.ROXY, b.p27.SUHA, b.p27.SYBY, b.p27.SOZU);
  c.p27.POHU = not(b.p27.RONE);
  c.p27.MOXE = not(b.p01.CLK_AxCxExGx4);
  c.p27.PANY = nor(b.p27.NUKO, b.p27.ROZE);
  c.p27.SEKO = nor(b.p27.RENE, !b.p27.RYFA);
  c.p27.ROMO = not(b.p24.POKY);
  c.p27.SUVU = nand(b.p21.XYMU, b.p27.ROMO, b.p24.NYKA, b.p24.PORY);
  c.p27.TAVE = not(b.p27.SUVU);
  c.p27.XAHY = not(b.p28.ATEJ);
  c.p27.XOFO = nand(b.p23.WIN_EN, b.p27.XAHY, b.p01.RESET_VIDEO);
  c.p27.XACO = not(b.p27.XOFO);
  c.p27.PYNU = or(b.p27.NUNU, b.p27.XOFO);
  c.p27.NUNY = and(!b.p27.NOPA, b.p27.PYNU);
  c.p27.NOCU = not(b.p27.PYNU);
  c.p27.PORE = not(b.p27.NOCU);

  c.p27.PUKU = nor(b.p27.NUNY, b.p27.RYDY);
  c.p27.RYDY = nor(b.p27.PUKU, b.p27.RESET_VIDEOn, b.p24.PORY);

  c.p27.SYLO = not(b.p27.RYDY);
  c.p27.TUXY = nand(b.p27.SOVY, b.p27.SYLO);
  c.p27.SUZU = not(b.p27.TUXY);
  c.p27.TEVO = nor(b.p27.SEKO, b.p27.SUZU, b.p27.TAVE);
  c.p27.PASO = nor(b.p27.TEVO, b.p27.PAHA);
  c.p27.VETU = and(b.p27.TEVO, b.p27.PORE);
  c.p27.ROCO = not(b.p24.SEGU);
  c.p27.MEHE = not(b.p01.CLK_AxCxExGx4);
  c.p27.NYFO = not(b.p27.NUNY);
  c.p27.MOSU = not(b.p27.NYFO);
  c.p27.NYXU = nor(b.p29.AVAP, b.p27.MOSU, b.p27.TEVO);
  c.p27.WAZY = not(b.p27.PORE);
  c.p27.SYNY = not(b.p27.REPU);

  c.p27.NOPA = tock_pos(a.p01.CLK_AxCxExGx4, b.p01.CLK_AxCxExGx4, b.p01.RESET_VIDEO, b.p27.NOPA, b.p27.PYNU);
  c.p27.PYCO = tock_pos(a.p27.ROCO, b.p27.ROCO, b.p01.RESET_VIDEO, b.p27.PYCO, b.p27.NUKO);
  c.p27.NUNU = tock_pos(a.p27.MEHE, b.p27.MEHE, b.p01.RESET_VIDEO, b.p27.NUNU, b.p27.PYCO);

  c.p27.RYKU = tock_pos(a.p27.PECU,  b.p27.PECU,  b.p27.PASO, b.p27.RYKU, !b.p27.RYKU);
  c.p27.ROGA = tock_pos(!a.p27.RYKU, !b.p27.RYKU, b.p27.PASO, b.p27.ROGA, !b.p27.ROGA);
  c.p27.RUBU = tock_pos(!a.p27.ROGA, !b.p27.ROGA, b.p27.PASO, b.p27.RUBU, !b.p27.RUBU);

  c.p27.NYZE = tock_pos(a.p27.MOXE, b.p27.MOXE, b.p21.XYMU, b.p27.NYZE, b.p27.PUXA);
  c.p27.PUXA = tock_pos(a.p24.ROXO, b.p24.ROXO, b.p21.XYMU, b.p27.PUXA, b.p27.POHU);

  c.p27.RYFA = tock_pos(a.p24.SEGU, b.p24.SEGU, b.p21.XYMU,        b.p27.RYFA, b.p27.PANY);
  c.p27.RENE = tock_pos(a.p01.CLK_AxCxExGx4, b.p01.CLK_AxCxExGx4, b.p21.XYMU,        b.p27.RENE, b.p27.RYFA);
  c.p27.SOVY = tock_pos(a.p01.CLK_AxCxExGx4, b.p01.CLK_AxCxExGx4, b.p01.RESET_VIDEO, b.p27.SOVY, b.p27.RYDY);

  //----------
  // dunno, top right

  c.p27.MOCE = nand(b.p27.LAXU, b.p27.NYVA, b.p27.NYXU);
  c.p27.LEBO = nand(b.p01.CLK_AxCxExGx4, b.p27.MOCE);
  c.p27.MYVO = not(b.p01.CLK_AxCxExGx4);
  c.p27.LYRY = not(b.p27.MOCE);
  c.p27.LAXE = not(b.p27.LAXU);
  c.p27.MYSO = nor(b.p24.LOBY, b.p27.LAXE, b.p27.LYZU);
  c.p27.NAKO = not(b.p27.MESU);
  c.p27.NOFU = not(b.p27.NYVA);
  c.p27.MOFU = and(b.p27.MYSO, b.p27.NAKO);
  c.p27.NOGU = nand(b.p27.NAKO, b.p27.NOFU);
  c.p27.NYDY = nand(b.p27.MYSO, b.p27.MESU, b.p27.NOFU);
  c.p27.XUHA = not(b.p27.NOFU);
  c.p27.NENY = not(b.p27.NOGU);
  c.p27.LURY = and(!b.p27.LOVY, b.p21.XYMU);
  c.p27.LONY = unk2(b.p27.LURY, !b.p27.LOVY);
  c.p27.MYMA = not(b.p27.LONY);
  c.p27.LUSU = not(b.p27.LONY);
  c.p27.LENA = not(b.p27.LUSU);
  c.p27.POTU = and(b.p27.LENA, b.p27.NENY);
  c.p27.NETA = and(b.p27.LENA, b.p27.NOGU);

  c.p27.LYZU = tock_pos( a.p01.CLK_AxCxExGx4,  b.p01.CLK_AxCxExGx4, b.p21.XYMU, b.p27.LYZU,  b.p27.LAXU);
  c.p27.LAXU = tock_pos( a.p27.LEBO,  b.p27.LEBO, b.p27.NYXU, b.p27.LAXU, !b.p27.LAXU);
  c.p27.MESU = tock_pos(!a.p27.LAXU, !b.p27.LAXU, b.p27.NYXU, b.p27.MESU, !b.p27.MESU);
  c.p27.NYVA = tock_pos(!a.p27.MESU, !b.p27.MESU, b.p27.NYXU, b.p27.NYVA, !b.p27.NYVA);
  c.p27.LOVY = tock_pos( a.p27.MYVO,  b.p27.MYVO, b.p27.NYXU, b.p27.LOVY,  b.p27.LYRY);

  //----------
  // address output bus

  c.p27.WYKA = tock_pos( a.p27.VETU,  b.p27.VETU, b.p27.XACO, b.p27.WYKA, !b.p27.WYKA);
  c.p27.WODY = tock_pos(!a.p27.WYKA, !b.p27.WYKA, b.p27.XACO, b.p27.WODY, !b.p27.WODY);
  c.p27.WOBO = tock_pos(!a.p27.WODY, !b.p27.WODY, b.p27.XACO, b.p27.WOBO, !b.p27.WOBO);
  c.p27.WYKO = tock_pos(!a.p27.WOBO, !b.p27.WOBO, b.p27.XACO, b.p27.WYKO, !b.p27.WYKO);
  c.p27.XOLO = tock_pos(!a.p27.WYKO, !b.p27.WYKO, b.p27.XACO, b.p27.XOLO, !b.p27.XOLO);

  c.p27.VYNO = tock_pos( a.p27.WAZY,  b.p27.WAZY, b.p27.SYNY, b.p27.VYNO, !b.p27.VYNO);
  c.p27.VUJO = tock_pos(!a.p27.VYNO, !b.p27.VYNO, b.p27.SYNY, b.p27.VUJO, !b.p27.VUJO);
  c.p27.VYMU = tock_pos(!a.p27.VUJO, !b.p27.VUJO, b.p27.SYNY, b.p27.VYMU, !b.p27.VYMU);

  c.p27.TUFU = tock_pos(!a.p27.VYMU, !b.p27.VYMU, b.p27.SYNY, b.p27.TUFU, !b.p27.TUFU);
  c.p27.TAXA = tock_pos(!a.p27.TUFU, !b.p27.TUFU, b.p27.SYNY, b.p27.TAXA, !b.p27.TAXA);
  c.p27.TOZO = tock_pos(!a.p27.TAXA, !b.p27.TAXA, b.p27.SYNY, b.p27.TOZO, !b.p27.TOZO);
  c.p27.TATE = tock_pos(!a.p27.TOZO, !b.p27.TOZO, b.p27.SYNY, b.p27.TATE, !b.p27.TATE);
  c.p27.TEKE = tock_pos(!a.p27.TATE, !b.p27.TATE, b.p27.SYNY, b.p27.TEKE, !b.p27.TEKE);

  c.p27.XEJA = not(b.p27.WYKA);
  c.p27.XAMO = not(b.p27.WODY);
  c.p27.XAHE = not(b.p27.WOBO);
  c.p27.XULO = not(b.p27.WYKO);
  c.p27.WUJU = not(b.p27.XOLO);

  c.p27.VYTO = not(b.p27.TUFU);
  c.p27.VEHA = not(b.p27.TAXA);
  c.p27.VACE = not(b.p27.TOZO);
  c.p27.VOVO = not(b.p27.TATE);
  c.p27.VULO = not(b.p27.TEKE);

  c.p27.VEVY = not(b.p23.WIN_MAP_SEL);
  c.p27.VEZA = not(b.p27.VYPO);
  c.p27.VOGU = not(b.p27.VYPO);

  if (b.p25.WUKO) {
    c.MA00 = b.p27.XEJA;
    c.MA01 = b.p27.XAMO;
    c.MA02 = b.p27.XAHE;
    c.MA03 = b.p27.XULO;
    c.MA04 = b.p27.WUJU;
    c.MA05 = b.p27.VYTO;
    c.MA06 = b.p27.VEHA;
    c.MA07 = b.p27.VACE;
    c.MA08 = b.p27.VOVO;
    c.MA09 = b.p27.VULO;
    c.MA10 = b.p27.VEVY;
    c.MA11 = b.p27.VEZA;
    c.MA12 = b.p27.VOGU;
  }

  //----------
  // computes SECA and TAVA

  c.p27.RYCE = and(b.p27.SOBU, !b.p27.SUDA);
  c.p27.ROSY = not(b.p01.RESET_VIDEO);
  c.p27.SECA = nor(b.p27.RYCE, b.p27.ROSY, b.p28.ATEJ);
  c.p27.VEKU = nor(b.p29.WUTY, b.p27.TAVE);
  c.p27.TAKA = unk2(b.p27.VEKU, b.p27.SECA);
  c.p27.TUKU = not(b.p24.TOMU);
  c.p27.SOWO = not(b.p27.TAKA);
  c.p27.TEKY = and(b.p29.FEPO, b.p27.TUKU, b.p27.LYRY, b.p27.SOWO);
  c.p27.TAVA = not(b.p01.LAPE);

  c.p27.SOBU = tock_pos(a.p27.TAVA, b.p27.TAVA, b.p27.VYPO, b.p27.SOBU, b.p27.TEKY);
  c.p27.SUDA = tock_pos(a.p01.LAPE, b.p01.LAPE, b.p27.VYPO, b.p27.SUDA, b.p27.SOBU);
}
