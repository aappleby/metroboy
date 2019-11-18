#include "P36_Palettes.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P36_Palettes::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // BGP
  c.p36.VUSO = and(b.sys.CPU_RD2, b.p22.FF47);
  c.p36.VELY = and(b.sys.CPU_WR2, b.p22.FF47);
  c.p36.TEPY = not(b.p36.VUSO);
  c.p36.TEPO = not(b.p36.VELY);

  c.p36.MENA = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.MENA, b.D7);
  c.p36.MORU = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.MORU, b.D5);
  c.p36.MAXY = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.MAXY, b.D3);
  c.p36.NUSY = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.NUSY, b.D1);
  c.p36.MOGY = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.MOGY, b.D6);
  c.p36.MUKE = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.MUKE, b.D4);
  c.p36.PYLU = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.PYLU, b.D2);
  c.p36.PAVO = tock_pos(a.p36.TEPO, b.p36.TEPO, 0, b.p36.PAVO, b.D0);

  c.p36.LARY = not(!b.p36.MENA);
  c.p36.LYKA = not(!b.p36.MORU);
  c.p36.LOBE = not(!b.p36.MAXY);
  c.p36.PABA = not(!b.p36.NUSY);
  c.p36.LODY = not(!b.p36.MOGY);
  c.p36.LACE = not(!b.p36.MUKE);
  c.p36.REDO = not(!b.p36.PYLU);
  c.p36.RARO = not(!b.p36.PAVO);

  if (b.p36.TEPY) {
    c.D7 = b.p36.LARY;
    c.D5 = b.p36.LYKA;
    c.D3 = b.p36.LOBE;
    c.D1 = b.p36.PABA;
    c.D6 = b.p36.LODY;
    c.D4 = b.p36.LACE;
    c.D2 = b.p36.REDO;
    c.D0 = b.p36.RARO;
  }

  // OBP0
  c.p36.XUFY = and(b.sys.CPU_RD2, b.p22.FF48);
  c.p36.XOMA = and(b.sys.CPU_WR2, b.p22.FF48);
  c.p36.XOZY = not(b.p36.XUFY);
  c.p36.XELO = not(b.p36.XOMA);

  c.p36.XANA = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XANA, b.D7);
  c.p36.XYZE = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XYZE, b.D5);
  c.p36.XALO = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XALO, b.D3);
  c.p36.XUKY = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XUKY, b.D1);
  c.p36.XUPO = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XUPO, b.D6);
  c.p36.XERU = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XERU, b.D4);
  c.p36.XOVA = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XOVA, b.D2);
  c.p36.XUFU = tock_pos(a.p36.XELO, b.p36.XELO, 0, b.p36.XUFU, b.D0);

  c.p36.XAWO = not(!b.p36.XANA);
  c.p36.XOBO = not(!b.p36.XYZE);
  c.p36.XUBY = not(!b.p36.XALO);
  c.p36.XOKE = not(!b.p36.XUKY);
  c.p36.XAXA = not(!b.p36.XUPO);
  c.p36.XAJU = not(!b.p36.XERU);
  c.p36.XUNO = not(!b.p36.XOVA);
  c.p36.XARY = not(!b.p36.XUFU);

  if (b.p36.XOZY) {
    c.D7 = b.p36.XAWO;
    c.D5 = b.p36.XOBO;
    c.D3 = b.p36.XUBY;
    c.D1 = b.p36.XOKE;
    c.D6 = b.p36.XAXA;
    c.D4 = b.p36.XAJU;
    c.D2 = b.p36.XUNO;
    c.D0 = b.p36.XARY;
  }

  // OBP1
  c.p36.MUMY = and(b.sys.CPU_RD2, b.p22.FF49);
  c.p36.MYXE = and(b.sys.CPU_WR2, b.p22.FF49);
  c.p36.LOTE = not(b.p36.MUMY);
  c.p36.LEHO = not(b.p36.MYXE);

  c.p36.LUXO = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.LUXO, b.D7);
  c.p36.LUGU = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.LUGU, b.D5);
  c.p36.LOSE = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.LOSE, b.D3);
  c.p36.LAWO = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.LAWO, b.D1);
  c.p36.LEPU = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.LEPU, b.D6);
  c.p36.LUNE = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.LUNE, b.D4);
  c.p36.MOSA = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.MOSA, b.D2);
  c.p36.MOXY = tock_pos(a.p36.LEHO, b.p36.LEHO, 0, b.p36.MOXY, b.D0);

  c.p36.LELU = not(!b.p36.LUXO);
  c.p36.LUGA = not(!b.p36.LUGU);
  c.p36.LYZA = not(!b.p36.LOSE);
  c.p36.LEPA = not(!b.p36.LAWO);
  c.p36.LEBA = not(!b.p36.LEPU);
  c.p36.LUKY = not(!b.p36.LUNE);
  c.p36.LODE = not(!b.p36.MOSA);
  c.p36.LAJU = not(!b.p36.MOXY);

  if (b.p36.LOTE) {
    c.D7 = b.p36.LELU;
    c.D5 = b.p36.LUGA;
    c.D3 = b.p36.LYZA;
    c.D1 = b.p36.LEPA;
    c.D6 = b.p36.LEBA;
    c.D4 = b.p36.LUKY;
    c.D2 = b.p36.LODE;
    c.D0 = b.p36.LAJU;
  }
}