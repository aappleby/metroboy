#include "P23_VideoRegs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P23_VideoRegs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // FF40 LCDC
  /*p23.VYRE*/ c.p23.VYRE = and(b.p22.FF40, b.sys.CPU_RD2);
  /*p23.WARU*/ c.p23.WARU = and(b.p22.FF40, b.sys.CPU_WR2);
  /*p23.WYCE*/ c.p23.WYCE = not(b.p23.VYRE);
  /*p23.XUBO*/ c.p23.XUBO = not(b.p23.WARU);

  /*p23.VYXE*/ c.p23.BG_EN        = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.BG_EN      , b.D0);
  /*p23.XYLO*/ c.p23.SPRITE_EN    = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.SPRITE_EN  , b.D1);
  /*p23.XYMO*/ c.p23.SPRITE_SIZE  = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.SPRITE_SIZE, b.D2);
  /*p23.XAFO*/ c.p23.BG_MAP_SEL   = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.BG_MAP_SEL , b.D3);
  /*p23.WEXU*/ c.p23.BG_TILE_SEL  = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.BG_TILE_SEL, b.D4);
  /*p23.WYMO*/ c.p23.WIN_EN       = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.WIN_EN     , b.D5);
  /*p23.WOKY*/ c.p23.WIN_MAP_SEL  = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.WIN_MAP_SEL, b.D6);
  /*p23.XONA*/ c.p23.LCD_ON       = tock_pos(a.p23.XUBO, b.p23.XUBO, b.sys.SYS_RESETn7, b.p23.LCD_ON     , b.D7);

  /*p23.WYPO*/ if (b.p23.WYCE) c.D0 = b.p23.BG_EN      ;
  /*p23.XERO*/ if (b.p23.WYCE) c.D1 = b.p23.SPRITE_EN  ;
  /*p23.WYJU*/ if (b.p23.WYCE) c.D2 = b.p23.SPRITE_SIZE;
  /*p23.WUKA*/ if (b.p23.WYCE) c.D3 = b.p23.BG_MAP_SEL ;
  /*p23.VOKE*/ if (b.p23.WYCE) c.D4 = b.p23.BG_TILE_SEL;
  /*p23.VATO*/ if (b.p23.WYCE) c.D5 = b.p23.WIN_EN     ;
  /*p23.VAHA*/ if (b.p23.WYCE) c.D6 = b.p23.WIN_MAP_SEL;
  /*p23.XEBU*/ if (b.p23.WYCE) c.D7 = b.p23.LCD_ON     ;

  // FF4B WX
  /*p23.WYZE*/ c.p23.WYZE = and(b.sys.CPU_RD2, b.p22.FF4B);
  /*p23.WUZA*/ c.p23.WUZA = and(b.sys.CPU_WR2, b.p22.FF4B);
  /*p23.VYCU*/ c.p23.VYCU = not(b.p23.WYZE);
  /*p23.VOXU*/ c.p23.VOXU = not(b.p23.WUZA);
  
  /*p23.MYPA*/ c.p23.WX0 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX0, b.D0);
  /*p23.NOFE*/ c.p23.WX1 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX1, b.D1);
  /*p23.NOKE*/ c.p23.WX2 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX2, b.D2);
  /*p23.MEBY*/ c.p23.WX3 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX3, b.D3);
  /*p23.MYPU*/ c.p23.WX4 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX4, b.D4);
  /*p23.MYCE*/ c.p23.WX5 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX5, b.D5); 
  /*p23.MUVO*/ c.p23.WX6 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX6, b.D6);
  /*p23.NUKU*/ c.p23.WX7 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX7, b.D7);

  /*p23.LOVA*/ if (b.p23.VYCU) c.D0 = b.p23.WX0;
  /*p23.MUKA*/ if (b.p23.VYCU) c.D1 = b.p23.WX1;
  /*p23.MOKO*/ if (b.p23.VYCU) c.D2 = b.p23.WX2;
  /*p23.LOLE*/ if (b.p23.VYCU) c.D3 = b.p23.WX3;
  /*p23.MELE*/ if (b.p23.VYCU) c.D4 = b.p23.WX4;
  /*p23.MUFE*/ if (b.p23.VYCU) c.D5 = b.p23.WX5;
  /*p23.MULY*/ if (b.p23.VYCU) c.D6 = b.p23.WX6;
  /*p23.MARA*/ if (b.p23.VYCU) c.D7 = b.p23.WX7;

  // FF4A WY
  /*p23.WAXU*/ c.p23.WAXU = and(b.sys.CPU_RD2, b.p22.FF4A);
  /*p23.WEKO*/ c.p23.WEKO = and(b.sys.CPU_WR2, b.p22.FF4A);
  /*p23.VEFU*/ c.p23.VEFU = not(b.p23.WEKO);
  /*p23.VOMY*/ c.p23.VOMY = not(b.p23.WAXU);

  /*p23.NESO*/ c.p23.WY0 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY0, b.D0);
  /*p23.NYRO*/ c.p23.WY1 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY1, b.D1);
  /*p23.NAGA*/ c.p23.WY2 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY2, b.D2);
  /*p23.MELA*/ c.p23.WY3 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY3, b.D3);
  /*p23.NULO*/ c.p23.WY4 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY4, b.D4);
  /*p23.NENE*/ c.p23.WY5 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY5, b.D5);
  /*p23.NUKA*/ c.p23.WY6 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY6, b.D6);
  /*p23.NAFU*/ c.p23.WY7 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY7, b.D7);

  /*p23.PUNU*/ if (b.p23.VOMY) c.D0 = b.p23.WY0;
  /*p23.PODA*/ if (b.p23.VOMY) c.D1 = b.p23.WY1;
  /*p23.PYGU*/ if (b.p23.VOMY) c.D2 = b.p23.WY2;
  /*p23.LOKA*/ if (b.p23.VOMY) c.D3 = b.p23.WY3;
  /*p23.MEGA*/ if (b.p23.VOMY) c.D4 = b.p23.WY4;
  /*p23.PELA*/ if (b.p23.VOMY) c.D5 = b.p23.WY5;
  /*p23.POLO*/ if (b.p23.VOMY) c.D6 = b.p23.WY6;
  /*p23.MERA*/ if (b.p23.VOMY) c.D7 = b.p23.WY7;

  // FF43 SCX
  /*p23.AVOG*/ c.p23.AVOG = and(b.p22.FF43, b.sys.CPU_RD2);
  /*p23.ARUR*/ c.p23.ARUR = and(b.p22.FF43, b.sys.CPU_WR2);
  /*p23.AMUN*/ c.p23.AMUN = not(b.p23.ARUR);
  /*p23.BEBA*/ c.p23.BEBA = not(b.p23.AVOG);

  /*p23.DATY*/ c.p23.SCX0 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX0, b.D0);
  /*p23.DUZU*/ c.p23.SCX1 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX1, b.D1);
  /*p23.CYXU*/ c.p23.SCX2 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX2, b.D2);
  /*p23.GUBO*/ c.p23.SCX3 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX3, b.D3);
  /*p23.BEMY*/ c.p23.SCX4 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX4, b.D4);
  /*p23.CUZY*/ c.p23.SCX5 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX5, b.D5);
  /*p23.CABU*/ c.p23.SCX6 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX6, b.D6);
  /*p23.BAKE*/ c.p23.SCX7 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX7, b.D7);

  /*p23.EDOS*/ if (b.p23.BEBA) c.D0 = b.p23.SCX0;
  /*p23.EKOB*/ if (b.p23.BEBA) c.D1 = b.p23.SCX1;
  /*p23.CUGA*/ if (b.p23.BEBA) c.D2 = b.p23.SCX2;
  /*p23.WONY*/ if (b.p23.BEBA) c.D3 = b.p23.SCX3;
  /*p23.CEDU*/ if (b.p23.BEBA) c.D4 = b.p23.SCX4;
  /*p23.CATA*/ if (b.p23.BEBA) c.D5 = b.p23.SCX5;
  /*p23.DOXE*/ if (b.p23.BEBA) c.D6 = b.p23.SCX6;
  /*p23.CASY*/ if (b.p23.BEBA) c.D7 = b.p23.SCX7;

  // FF42 SCY
  /*p23.ANYP*/ c.p23.ANYP = and(b.sys.CPU_RD2, b.p22.FF42);
  /*p23.BEDY*/ c.p23.BEDY = and(b.sys.CPU_WR2, b.p22.FF42);
  /*p23.CAVO*/ c.p23.CAVO = not(b.p23.BEDY);
  /*p23.BUWY*/ c.p23.BUWY = not(b.p23.ANYP);

  /*p23.GAVE*/ c.p23.SCY0 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY0, b.D0);
  /*p23.FYMO*/ c.p23.SCY1 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY1, b.D1);
  /*p23.FEZU*/ c.p23.SCY2 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY2, b.D2);
  /*p23.FUJO*/ c.p23.SCY3 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY3, b.D3);
  /*p23.DEDE*/ c.p23.SCY4 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY4, b.D4);
  /*p23.FOTY*/ c.p23.SCY5 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY5, b.D5);
  /*p23.FOHA*/ c.p23.SCY6 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY6, b.D6);
  /*p23.FUNY*/ c.p23.SCY7 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY7, b.D7);

  /*p23.WARE*/ if (b.p23.BUWY) c.D0 = b.p23.SCY0;
  /*p23.GOBA*/ if (b.p23.BUWY) c.D1 = b.p23.SCY1;
  /*p23.GONU*/ if (b.p23.BUWY) c.D2 = b.p23.SCY2;
  /*p23.GODO*/ if (b.p23.BUWY) c.D3 = b.p23.SCY3;
  /*p23.CUSA*/ if (b.p23.BUWY) c.D4 = b.p23.SCY4;
  /*p23.GYZO*/ if (b.p23.BUWY) c.D5 = b.p23.SCY5;
  /*p23.GUNE*/ if (b.p23.BUWY) c.D6 = b.p23.SCY6;
  /*p23.GYZA*/ if (b.p23.BUWY) c.D7 = b.p23.SCY7;

  // FF45 LYC
  /*p23.XYLY*/ c.p23.XYLY = and(b.sys.CPU_RD2, b.p22.FF45);
  /*p23.XUFA*/ c.p23.XUFA = and(b.sys.CPU_WR2, b.p22.FF45);
  /*p23.WANE*/ c.p23.WANE = not(b.p23.XUFA);
  /*p23.WEKU*/ c.p23.WEKU = not(b.p23.XYLY);

  /*p23.SYRY*/ c.p23.LYC0 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC0, b.D0);
  /*p23.VUCE*/ c.p23.LYC1 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC1, b.D1);
  /*p23.SEDY*/ c.p23.LYC2 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC2, b.D2);
  /*p23.SALO*/ c.p23.LYC3 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC3, b.D3);
  /*p23.SOTA*/ c.p23.LYC4 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC4, b.D4);
  /*p23.VAFA*/ c.p23.LYC5 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC5, b.D5);
  /*p23.VEVO*/ c.p23.LYC6 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC6, b.D6);
  /*p23.RAHA*/ c.p23.LYC7 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC7, b.D7);

  /*p23.RETU*/ if (b.p23.WEKU) c.D0 = b.p23.LYC0;
  /*p23.VOJO*/ if (b.p23.WEKU) c.D1 = b.p23.LYC1;
  /*p23.RAZU*/ if (b.p23.WEKU) c.D2 = b.p23.LYC2;
  /*p23.REDY*/ if (b.p23.WEKU) c.D3 = b.p23.LYC3;
  /*p23.RACE*/ if (b.p23.WEKU) c.D4 = b.p23.LYC4;
  /*p23.VAZU*/ if (b.p23.WEKU) c.D5 = b.p23.LYC5;
  /*p23.VAFE*/ if (b.p23.WEKU) c.D6 = b.p23.LYC6;
  /*p23.PUFY*/ if (b.p23.WEKU) c.D7 = b.p23.LYC7;

  // FF44 LY
  /*p23.WAFU*/ c.p23.FF44_RD = and(b.sys.CPU_RD2, b.p22.FF44);
  /*p23.VARO*/ c.p23.FF44_RDn = not(b.p23.FF44_RD);

  /*p23.WURY*/ c.p23.LY0n = not(b.p21.V0);
  /*p23.XEPO*/ c.p23.LY1n = not(b.p21.V1);
  /*p23.MYFA*/ c.p23.LY2n = not(b.p21.V2);
  /*p23.XUHY*/ c.p23.LY3n = not(b.p21.V3);
  /*p23.WATA*/ c.p23.LY4n = not(b.p21.V4);
  /*p23.XAGA*/ c.p23.LY5n = not(b.p21.V5);
  /*p23.XUCE*/ c.p23.LY6n = not(b.p21.V6);
  /*p23.XOWO*/ c.p23.LY7n = not(b.p21.V7);

  /*p23.VEGA*/ if (b.p23.FF44_RDn) c.D0 = not(b.p23.LY0n);
  /*p23.WUVA*/ if (b.p23.FF44_RDn) c.D1 = not(b.p23.LY1n);
  /*p23.LYCO*/ if (b.p23.FF44_RDn) c.D2 = not(b.p23.LY2n);
  /*p23.WOJY*/ if (b.p23.FF44_RDn) c.D3 = not(b.p23.LY3n);
  /*p23.VYNE*/ if (b.p23.FF44_RDn) c.D4 = not(b.p23.LY4n);
  /*p23.WAMA*/ if (b.p23.FF44_RDn) c.D5 = not(b.p23.LY5n);
  /*p23.WAVO*/ if (b.p23.FF44_RDn) c.D6 = not(b.p23.LY6n);
  /*p23.WEZE*/ if (b.p23.FF44_RDn) c.D7 = not(b.p23.LY7n);
}
