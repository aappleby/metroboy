#include "P23_VideoRegs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P23_VideoRegs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // FF40 LCDC
  c.p23.VYRE = and(b.p22.FF40, b.p07.CPU_RD2);
  c.p23.WARU = and(b.p22.FF40, b.p07.CPU_WR2);
  c.p23.WYCE = not(b.p23.VYRE);
  c.p23.XUBO = not(b.p23.WARU);

  c.p23.VYXE = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.VYXE, b.D0);
  c.p23.XYLO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.XYLO, b.D1);
  c.p23.XYMO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.XYMO, b.D2);
  c.p23.XAFO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.XAFO, b.D3);
  c.p23.WEXU = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.WEXU, b.D4);
  c.p23.WYMO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.WYMO, b.D5);
  c.p23.WOKY = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.WOKY, b.D6);
  c.p23.XONA = tock_pos(a.p23.XUBO, b.p23.XUBO, b.p01.SYS_RESETn7, b.p23.XONA, b.D7);

  if (b.p23.WYCE) {
    c.D0 = b.p23.VYXE;
    c.D1 = b.p23.XYLO;
    c.D2 = b.p23.XYMO;
    c.D3 = b.p23.XAFO;
    c.D4 = b.p23.WEXU;
    c.D5 = b.p23.WYMO;
    c.D6 = b.p23.WOKY;
    c.D7 = b.p23.XONA;
  }

  // FF4B WX
  c.p23.WYZE = and(b.p07.CPU_RD2, b.p22.FF4B);
  c.p23.WUZA = and(b.p07.CPU_WR2, b.p22.FF4B);
  c.p23.VYCU = not(b.p23.WYZE);
  c.p23.VOXU = not(b.p23.WUZA);
  
  // reset polarity?
  c.p23.MYPA = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.MYPA, b.D0);
  c.p23.NOFE = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.NOFE, b.D1);
  c.p23.NOKE = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.NOKE, b.D2);
  c.p23.MEBY = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.MEBY, b.D3);
  c.p23.MYPU = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.MYPU, b.D4);
  c.p23.MYCE = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.MYCE, b.D5); 
  c.p23.MUVO = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.MUVO, b.D6);
  c.p23.NUKU = tock_pos(a.p23.VOXU, b.p23.VOXU, b.p01.SYS_RESETn5, a.p23.NUKU, b.D7);

  c.p23.LOVA = not(!b.p23.MYPA);
  c.p23.MUKA = not(!b.p23.NOFE);
  c.p23.MOKO = not(!b.p23.NOKE);
  c.p23.LOLE = not(!b.p23.MEBY);
  c.p23.MELE = not(!b.p23.MYPU);
  c.p23.MUFE = not(!b.p23.MYCE);
  c.p23.MULY = not(!b.p23.MUVO);
  c.p23.MARA = not(!b.p23.NUKU);

  if (b.p23.VYCU) {
    c.D0 = b.p23.LOVA;
    c.D1 = b.p23.MUKA;
    c.D2 = b.p23.MOKO;
    c.D3 = b.p23.LOLE;
    c.D4 = b.p23.MELE;
    c.D5 = b.p23.MUFE;
    c.D6 = b.p23.MULY;
    c.D7 = b.p23.MARA;
  }

  // FF4A WY
  c.p23.WAXU = and(b.p07.CPU_RD2, b.p22.FF4A);
  c.p23.WEKO = and(b.p07.CPU_WR2, b.p22.FF4A);
  c.p23.VEFU = not(b.p23.WEKO);
  c.p23.VOMY = not(b.p23.WAXU);

  c.p23.NESO = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.NESO, b.D0);
  c.p23.NYRO = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.NYRO, b.D1);
  c.p23.NAGA = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.NAGA, b.D2);
  c.p23.MELA = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.MELA, b.D3);
  c.p23.NULO = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.NULO, b.D4);
  c.p23.NENE = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.NENE, b.D5);
  c.p23.NUKA = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.NUKA, b.D6);
  c.p23.NAFU = tock_pos(a.p23.VEFU, b.p23.VEFU, b.p01.SYS_RESETn5, b.p23.NAFU, b.D7);

  c.p23.PUNU = not(!b.p23.NESO);
  c.p23.PODA = not(!b.p23.NYRO);
  c.p23.PYGU = not(!b.p23.NAGA);
  c.p23.LOKA = not(!b.p23.MELA);
  c.p23.MEGA = not(!b.p23.NULO);
  c.p23.PELA = not(!b.p23.NENE);
  c.p23.POLO = not(!b.p23.NUKA);
  c.p23.MERA = not(!b.p23.NAFU);

  if (b.p23.VOMY) {
    c.D0 = b.p23.PUNU;
    c.D1 = b.p23.PODA;
    c.D2 = b.p23.PYGU;
    c.D3 = b.p23.LOKA;
    c.D4 = b.p23.MEGA;
    c.D5 = b.p23.PELA;
    c.D6 = b.p23.POLO;
    c.D7 = b.p23.MERA;
  }

  // FF43 SCX
  c.p23.AVOG = and(b.p22.FF43, b.p07.CPU_RD2);
  c.p23.ARUR = and(b.p22.FF43, b.p07.CPU_WR2);
  c.p23.AMUN = not(b.p23.ARUR);
  c.p23.BEBA = not(b.p23.AVOG);

  c.p23.DATY = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.DATY, b.D0);
  c.p23.DUZU = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.DUZU, b.D1);
  c.p23.CYXU = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.CYXU, b.D2);
  c.p23.GUBO = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.GUBO, b.D3);
  c.p23.BEMY = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.BEMY, b.D4);
  c.p23.CUZY = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.CUZY, b.D5);
  c.p23.CABU = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.CABU, b.D6);
  c.p23.BAKE = tock_pos(a.p23.AMUN, b.p23.AMUN, b.p01.SYS_RESETn2, a.p23.BAKE, b.D7);

  c.p23.EDOS = not(!b.p23.DATY);
  c.p23.EKOB = not(!b.p23.DUZU);
  c.p23.CUGA = not(!b.p23.CYXU);
  c.p23.WONY = not(!b.p23.GUBO);
  c.p23.CEDU = not(!b.p23.BEMY);
  c.p23.CATA = not(!b.p23.CUZY);
  c.p23.DOXE = not(!b.p23.CABU);
  c.p23.CASY = not(!b.p23.BAKE);

  if (b.p23.BEBA) {
    c.D0 = b.p23.EDOS;
    c.D1 = b.p23.EKOB;
    c.D2 = b.p23.CUGA;
    c.D3 = b.p23.WONY;
    c.D4 = b.p23.CEDU;
    c.D5 = b.p23.CATA;
    c.D6 = b.p23.DOXE;
    c.D7 = b.p23.CASY;
  }

  // FF42 SCY
  c.p23.ANYP = and(b.p07.CPU_RD2, b.p22.FF42);
  c.p23.BEDY = and(b.p07.CPU_WR2, b.p22.FF42);
  c.p23.CAVO = not(b.p23.BEDY);
  c.p23.BUWY = not(b.p23.ANYP);

  c.p23.GAVE = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.GAVE, b.D0);
  c.p23.FYMO = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.FYMO, b.D1);
  c.p23.FEZU = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.FEZU, b.D2);
  c.p23.FUJO = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.FUJO, b.D3);
  c.p23.DEDE = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.DEDE, b.D4);
  c.p23.FOTY = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.FOTY, b.D5);
  c.p23.FOHA = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.FOHA, b.D6);
  c.p23.FUNY = tock_pos(a.p23.CAVO, b.p23.CAVO, b.p01.SYS_RESETn2, b.p23.FUNY, b.D7);

  c.p23.WARE = not(!b.p23.GAVE);
  c.p23.GOBA = not(!b.p23.FYMO);
  c.p23.GONU = not(!b.p23.FEZU);
  c.p23.GODO = not(!b.p23.FUJO);
  c.p23.CUSA = not(!b.p23.DEDE);
  c.p23.GYZO = not(!b.p23.FOTY);
  c.p23.GUNE = not(!b.p23.FOHA);
  c.p23.GYZA = not(!b.p23.FUNY);

  if (b.p23.BUWY) {
    c.D0 = b.p23.WARE;
    c.D1 = b.p23.GOBA;
    c.D2 = b.p23.GONU;
    c.D3 = b.p23.GODO;
    c.D4 = b.p23.CUSA;
    c.D5 = b.p23.GYZO;
    c.D6 = b.p23.GUNE;
    c.D7 = b.p23.GYZA;
  }

  // FF45 LYC
  c.p23.XYLY = and(b.p07.CPU_RD2, b.p22.FF45);
  c.p23.XUFA = and(b.p07.CPU_WR2, b.p22.FF45);
  c.p23.WANE = not(b.p23.XUFA);
  c.p23.WEKU = not(b.p23.XYLY);

  c.p23.SYRY = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.SYRY, b.D0);
  c.p23.VUCE = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.VUCE, b.D1);
  c.p23.SEDY = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.SEDY, b.D2);
  c.p23.SALO = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.SALO, b.D3);
  c.p23.SOTA = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.SOTA, b.D4);
  c.p23.VAFA = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.VAFA, b.D5);
  c.p23.VEVO = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.VEVO, b.D6);
  c.p23.RAHA = tock_pos(a.p23.WANE, b.p23.WANE, b.p01.SYS_RESETn6, a.p23.RAHA, b.D7);

  c.p23.RETU = not(!b.p23.SYRY);
  c.p23.VOJO = not(!b.p23.VUCE);
  c.p23.RAZU = not(!b.p23.SEDY);
  c.p23.REDY = not(!b.p23.SALO);
  c.p23.RACE = not(!b.p23.SOTA);
  c.p23.VAZU = not(!b.p23.VAFA);
  c.p23.VAFE = not(!b.p23.VEVO);
  c.p23.PUFY = not(!b.p23.RAHA);

  if (b.p23.WEKU) {
    c.D0 = b.p23.RETU;
    c.D1 = b.p23.VOJO;
    c.D2 = b.p23.RAZU;
    c.D3 = b.p23.REDY;
    c.D4 = b.p23.RACE;
    c.D5 = b.p23.VAZU;
    c.D6 = b.p23.VAFE;
    c.D7 = b.p23.PUFY;
  }

  // FF44 LY
  c.p23.WAFU = and(b.p07.CPU_RD2, b.p22.FF44);
  c.p23.VARO = not(b.p23.WAFU);

  c.p23.WURY = not(b.p21.V0);
  c.p23.XEPO = not(b.p21.V1);
  c.p23.MYFA = not(b.p21.V2);
  c.p23.XUHY = not(b.p21.V3);
  c.p23.WATA = not(b.p21.V4);
  c.p23.XAGA = not(b.p21.V5);
  c.p23.XUCE = not(b.p21.V6);
  c.p23.XOWO = not(b.p21.V7);

  c.p23.VEGA = not(b.p23.WURY);
  c.p23.WUVA = not(b.p23.XEPO);
  c.p23.LYCO = not(b.p23.MYFA);
  c.p23.WOJY = not(b.p23.XUHY);
  c.p23.VYNE = not(b.p23.WATA);
  c.p23.WAMA = not(b.p23.XAGA);
  c.p23.WAVO = not(b.p23.XUCE);
  c.p23.WEZE = not(b.p23.XOWO);

  if (b.p23.VARO) {
    c.D0 = b.p23.VEGA;
    c.D1 = b.p23.WUVA;
    c.D2 = b.p23.LYCO;
    c.D3 = b.p23.WOJY;
    c.D4 = b.p23.VYNE;
    c.D5 = b.p23.WAMA;
    c.D6 = b.p23.WAVO;
    c.D7 = b.p23.WEZE;
  }
}
