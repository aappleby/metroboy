#include "P23_VideoRegs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P23_VideoRegs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // FF40 LCDC
  c.p23.VYRE = and(b.p22.FF40, b.sys.CPU_RD2);
  c.p23.WARU = and(b.p22.FF40, b.sys.CPU_WR2);
  c.p23.WYCE = not(b.p23.VYRE);
  c.p23.XUBO = not(b.p23.WARU);

  c.p23.VYXE = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.VYXE, b.D0);
  c.p23.XYLO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.XYLO, b.D1);
  c.p23.XYMO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.XYMO, b.D2);
  c.p23.XAFO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.XAFO, b.D3);
  c.p23.WEXU = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.WEXU, b.D4);
  c.p23.WYMO = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.WYMO, b.D5);
  c.p23.WOKY = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.WOKY, b.D6);
  c.p23.XONA = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.XONA, b.D7);

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
  c.p23.WYZE = and(b.sys.CPU_RD2, b.p22.FF4B);
  c.p23.WUZA = and(b.sys.CPU_WR2, b.p22.FF4B);
  c.p23.VYCU = not(b.p23.WYZE);
  c.p23.VOXU = not(b.p23.WUZA);
  
  // reset polarity?
  c.p23.WX0 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX0, b.D0);
  c.p23.WX1 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX1, b.D1);
  c.p23.WX2 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX2, b.D2);
  c.p23.WX3 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX3, b.D3);
  c.p23.WX4 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX4, b.D4);
  c.p23.WX5 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX5, b.D5); 
  c.p23.WX6 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX6, b.D6);
  c.p23.WX7 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX7, b.D7);

  /*p23.LOVA*/ if (b.p23.VYCU) c.D0 = b.p23.WX0;
  /*p23.MUKA*/ if (b.p23.VYCU) c.D1 = b.p23.WX1;
  /*p23.MOKO*/ if (b.p23.VYCU) c.D2 = b.p23.WX2;
  /*p23.LOLE*/ if (b.p23.VYCU) c.D3 = b.p23.WX3;
  /*p23.MELE*/ if (b.p23.VYCU) c.D4 = b.p23.WX4;
  /*p23.MUFE*/ if (b.p23.VYCU) c.D5 = b.p23.WX5;
  /*p23.MULY*/ if (b.p23.VYCU) c.D6 = b.p23.WX6;
  /*p23.MARA*/ if (b.p23.VYCU) c.D7 = b.p23.WX7;

  // FF4A WY
  c.p23.WAXU = and(b.sys.CPU_RD2, b.p22.FF4A);
  c.p23.WEKO = and(b.sys.CPU_WR2, b.p22.FF4A);
  c.p23.VEFU = not(b.p23.WEKO);
  c.p23.VOMY = not(b.p23.WAXU);

  c.p23.WY0 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY0, b.D0);
  c.p23.WY1 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY1, b.D1);
  c.p23.WY2 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY2, b.D2);
  c.p23.WY3 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY3, b.D3);
  c.p23.WY4 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY4, b.D4);
  c.p23.WY5 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY5, b.D5);
  c.p23.WY6 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY6, b.D6);
  c.p23.WY7 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY7, b.D7);

  /*p23.PUNU*/ if (b.p23.VOMY) c.D0 = b.p23.WY0;
  /*p23.PODA*/ if (b.p23.VOMY) c.D1 = b.p23.WY1;
  /*p23.PYGU*/ if (b.p23.VOMY) c.D2 = b.p23.WY2;
  /*p23.LOKA*/ if (b.p23.VOMY) c.D3 = b.p23.WY3;
  /*p23.MEGA*/ if (b.p23.VOMY) c.D4 = b.p23.WY4;
  /*p23.PELA*/ if (b.p23.VOMY) c.D5 = b.p23.WY5;
  /*p23.POLO*/ if (b.p23.VOMY) c.D6 = b.p23.WY6;
  /*p23.MERA*/ if (b.p23.VOMY) c.D7 = b.p23.WY7;

  // FF43 SCX
  c.p23.AVOG = and(b.p22.FF43, b.sys.CPU_RD2);
  c.p23.ARUR = and(b.p22.FF43, b.sys.CPU_WR2);
  c.p23.AMUN = not(b.p23.ARUR);
  c.p23.BEBA = not(b.p23.AVOG);

  c.p23.FF43_D0 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D0, b.D0);
  c.p23.FF43_D1 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D1, b.D1);
  c.p23.FF43_D2 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D2, b.D2);
  c.p23.FF43_D3 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D3, b.D3);
  c.p23.FF43_D4 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D4, b.D4);
  c.p23.FF43_D5 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D5, b.D5);
  c.p23.FF43_D6 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D6, b.D6);
  c.p23.FF43_D7 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.FF43_D7, b.D7);

  /*p23.EDOS*/ if (b.p23.BEBA) c.D0 = b.p23.FF43_D0;
  /*p23.EKOB*/ if (b.p23.BEBA) c.D1 = b.p23.FF43_D1;
  /*p23.CUGA*/ if (b.p23.BEBA) c.D2 = b.p23.FF43_D2;
  /*p23.WONY*/ if (b.p23.BEBA) c.D3 = b.p23.FF43_D3;
  /*p23.CEDU*/ if (b.p23.BEBA) c.D4 = b.p23.FF43_D4;
  /*p23.CATA*/ if (b.p23.BEBA) c.D5 = b.p23.FF43_D5;
  /*p23.DOXE*/ if (b.p23.BEBA) c.D6 = b.p23.FF43_D6;
  /*p23.CASY*/ if (b.p23.BEBA) c.D7 = b.p23.FF43_D7;

  // FF42 SCY
  c.p23.ANYP = and(b.sys.CPU_RD2, b.p22.FF42);
  c.p23.BEDY = and(b.sys.CPU_WR2, b.p22.FF42);
  c.p23.CAVO = not(b.p23.BEDY);
  c.p23.BUWY = not(b.p23.ANYP);

  c.p23.SCY0 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY0, b.D0);
  c.p23.SCY1 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY1, b.D1);
  c.p23.SCY2 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY2, b.D2);
  c.p23.SCY3 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY3, b.D3);
  c.p23.SCY4 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY4, b.D4);
  c.p23.SCY5 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY5, b.D5);
  c.p23.SCY6 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY6, b.D6);
  c.p23.SCY7 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY7, b.D7);

  /*p23.WARE*/ if (b.p23.BUWY) c.D0 = b.p23.SCY0;
  /*p23.GOBA*/ if (b.p23.BUWY) c.D1 = b.p23.SCY1;
  /*p23.GONU*/ if (b.p23.BUWY) c.D2 = b.p23.SCY2;
  /*p23.GODO*/ if (b.p23.BUWY) c.D3 = b.p23.SCY3;
  /*p23.CUSA*/ if (b.p23.BUWY) c.D4 = b.p23.SCY4;
  /*p23.GYZO*/ if (b.p23.BUWY) c.D5 = b.p23.SCY5;
  /*p23.GUNE*/ if (b.p23.BUWY) c.D6 = b.p23.SCY6;
  /*p23.GYZA*/ if (b.p23.BUWY) c.D7 = b.p23.SCY7;

  // FF45 LYC
  c.p23.XYLY = and(b.sys.CPU_RD2, b.p22.FF45);
  c.p23.XUFA = and(b.sys.CPU_WR2, b.p22.FF45);
  c.p23.WANE = not(b.p23.XUFA);
  c.p23.WEKU = not(b.p23.XYLY);

  c.p23.LYC0 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC0, b.D0);
  c.p23.LYC1 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC1, b.D1);
  c.p23.LYC2 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC2, b.D2);
  c.p23.LYC3 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC3, b.D3);
  c.p23.LYC4 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC4, b.D4);
  c.p23.LYC5 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC5, b.D5);
  c.p23.LYC6 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC6, b.D6);
  c.p23.LYC7 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC7, b.D7);

  /*p23.RETU*/ if (b.p23.WEKU) c.D0 = b.p23.LYC0;
  /*p23.VOJO*/ if (b.p23.WEKU) c.D1 = b.p23.LYC1;
  /*p23.RAZU*/ if (b.p23.WEKU) c.D2 = b.p23.LYC2;
  /*p23.REDY*/ if (b.p23.WEKU) c.D3 = b.p23.LYC3;
  /*p23.RACE*/ if (b.p23.WEKU) c.D4 = b.p23.LYC4;
  /*p23.VAZU*/ if (b.p23.WEKU) c.D5 = b.p23.LYC5;
  /*p23.VAFE*/ if (b.p23.WEKU) c.D6 = b.p23.LYC6;
  /*p23.PUFY*/ if (b.p23.WEKU) c.D7 = b.p23.LYC7;

  // FF44 LY
  c.p23.WAFU = and(b.sys.CPU_RD2, b.p22.FF44);
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
