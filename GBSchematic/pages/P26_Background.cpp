#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P26_Background::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p26.AXAD*/ c.p26.AXAD = not(b.p27.PORE);

  //----------
  // y + scy;

  /*p26.FAFO*/ c.p26.TILE_Y0C = add_c(b.p21.V0, b.p23.SCY0, 0);
  /*p26.EMUX*/ c.p26.TILE_Y1C = add_c(b.p21.V1, b.p23.SCY1, b.p26.TILE_Y0C);
  /*p26.ECAB*/ c.p26.TILE_Y2C = add_c(b.p21.V2, b.p23.SCY2, b.p26.TILE_Y1C);
  /*p26.ETAM*/ c.p26.MAP_Y0C  = add_c(b.p21.V3, b.p23.SCY3, b.p26.TILE_Y2C);
  /*p26.DOTO*/ c.p26.MAP_Y1C  = add_c(b.p21.V4, b.p23.SCY4, b.p26.MAP_Y0C);
  /*p26.DABA*/ c.p26.MAP_Y2C  = add_c(b.p21.V5, b.p23.SCY5, b.p26.MAP_Y1C);
  /*p26.EFYK*/ c.p26.MAP_Y3C  = add_c(b.p21.V6, b.p23.SCY6, b.p26.MAP_Y2C);
  /*p26.EJOK*/ c.p26.MAP_Y4C  = add_c(b.p21.V7, b.p23.SCY7, b.p26.MAP_Y3C);

  /*p26.FAFO*/ c.p26.TILE_Y0 = add_s(b.p21.V0, b.p23.SCY0, 0);
  /*p26.EMUX*/ c.p26.TILE_Y1 = add_s(b.p21.V1, b.p23.SCY1, b.p26.TILE_Y0C);
  /*p26.ECAB*/ c.p26.TILE_Y2 = add_s(b.p21.V2, b.p23.SCY2, b.p26.TILE_Y1C);
  /*p26.ETAM*/ c.p26.MAP_Y0  = add_s(b.p21.V3, b.p23.SCY3, b.p26.TILE_Y2C);
  /*p26.DOTO*/ c.p26.MAP_Y1  = add_s(b.p21.V4, b.p23.SCY4, b.p26.MAP_Y0C);
  /*p26.DABA*/ c.p26.MAP_Y2  = add_s(b.p21.V5, b.p23.SCY5, b.p26.MAP_Y1C);
  /*p26.EFYK*/ c.p26.MAP_Y3  = add_s(b.p21.V6, b.p23.SCY6, b.p26.MAP_Y2C);
  /*p26.EJOK*/ c.p26.MAP_Y4  = add_s(b.p21.V7, b.p23.SCY7, b.p26.MAP_Y3C);

  /*p26.ASUL*/ c.p26.ASUL = and(b.p26.AXAD, b.p27.NETA);
  /*p26.BEJE*/ c.p26.BEJE = not(b.p26.ASUL);

  /*p26.ASUM*/ if (b.p26.BEJE) c.MA00 = b.p27.MA00_2;
  /*p26.EVAD*/ if (b.p26.BEJE) c.MA01 = b.p26.TILE_Y0;
  /*p26.DAHU*/ if (b.p26.BEJE) c.MA02 = b.p26.TILE_Y1;
  /*p26.DODE*/ if (b.p26.BEJE) c.MA03 = b.p26.TILE_Y2;

  //----------
  // x + scx

  /*p26.ATAD*/ c.p26.TILE_X0C = add_c(b.p21.X0, b.p23.SCX0, 0);
  /*p26.BEHU*/ c.p26.TILE_X1C = add_c(b.p21.X1, b.p23.SCX1, b.p26.TILE_X0C);
  /*p26.APYH*/ c.p26.TILE_X2C = add_c(b.p21.X2, b.p23.SCX2, b.p26.TILE_X1C);
  /*p26.BABE*/ c.p26.MAP_X0C  = add_c(b.p21.X3, b.p23.SCX3, b.p26.TILE_X2C);
  /*p26.ABOD*/ c.p26.MAP_X1C  = add_c(b.p21.X4, b.p23.SCX4, b.p26.MAP_X0C);
  /*p26.BEWY*/ c.p26.MAP_X2C  = add_c(b.p21.X5, b.p23.SCX5, b.p26.MAP_X1C);
  /*p26.BYCA*/ c.p26.MAP_X3C  = add_c(b.p21.X6, b.p23.SCX6, b.p26.MAP_X2C);
  /*p26.ACUL*/ c.p26.MAP_X4C  = add_c(b.p21.X7, b.p23.SCX7, b.p26.MAP_X3C);

  /*p26.ATAD*/ c.p26.TILE_X0  = add_s(b.p21.X0, b.p23.SCX0, 0);
  /*p26.BEHU*/ c.p26.TILE_X1  = add_s(b.p21.X1, b.p23.SCX1, b.p26.TILE_X0C);
  /*p26.APYH*/ c.p26.TILE_X2  = add_s(b.p21.X2, b.p23.SCX2, b.p26.TILE_X1C);
  /*p26.BABE*/ c.p26.MAP_X0   = add_s(b.p21.X3, b.p23.SCX3, b.p26.TILE_X2C);
  /*p26.ABOD*/ c.p26.MAP_X1   = add_s(b.p21.X4, b.p23.SCX4, b.p26.MAP_X0C);
  /*p26.BEWY*/ c.p26.MAP_X2   = add_s(b.p21.X5, b.p23.SCX5, b.p26.MAP_X1C);
  /*p26.BYCA*/ c.p26.MAP_X3   = add_s(b.p21.X6, b.p23.SCX6, b.p26.MAP_X2C);
  /*p26.ACUL*/ c.p26.MAP_X4   = add_s(b.p21.X7, b.p23.SCX7, b.p26.MAP_X3C);

  //----------
  // map fetch

  /*p26.ACEN*/ c.p26.FETCH_MAPn = and(b.p26.AXAD, b.p27.POTU);
  /*p26.BAFY*/ c.p26.FETCH_MAP  = not(b.p26.FETCH_MAPn);

  /*p26.AXEP*/ if (b.p26.FETCH_MAP) c.MA00 = b.p26.MAP_X0;
  /*p26.AFEB*/ if (b.p26.FETCH_MAP) c.MA01 = b.p26.MAP_X1;
  /*p26.ALEL*/ if (b.p26.FETCH_MAP) c.MA02 = b.p26.MAP_X2;
  /*p26.COLY*/ if (b.p26.FETCH_MAP) c.MA03 = b.p26.MAP_X3;
  /*p26.AJAN*/ if (b.p26.FETCH_MAP) c.MA04 = b.p26.MAP_X4;

  /*p26.DUHO*/ if (b.p26.FETCH_MAP) c.MA05 = b.p26.MAP_Y0;
  /*p26.CASE*/ if (b.p26.FETCH_MAP) c.MA06 = b.p26.MAP_Y1;
  /*p26.CYPO*/ if (b.p26.FETCH_MAP) c.MA07 = b.p26.MAP_Y2;
  /*p26.CETA*/ if (b.p26.FETCH_MAP) c.MA08 = b.p26.MAP_Y3;
  /*p26.DAFE*/ if (b.p26.FETCH_MAP) c.MA09 = b.p26.MAP_Y4;

  /*p26.AMUV*/ if (b.p26.FETCH_MAP) c.MA10 = b.p23.LCDC_BGMAP;
  /*p26.COVE*/ if (b.p26.FETCH_MAP) c.MA11 = b.p28.P10_Bn;
  /*p26.COXO*/ if (b.p26.FETCH_MAP) c.MA12 = b.p28.P10_Bn;

  //----------
  // background shift register connected to VAVA3

  /*p26.XOGA*/ c.p26.OAM_A_D7n = not(b.p31.OAM_A_D7);
  /*p26.XURA*/ c.p26.OAM_A_D7o = not(b.p31.OAM_A_D7);
  /*p26.TAJO*/ c.p26.OAM_A_D7p = not(b.p31.OAM_A_D7);
  /*p26.XENU*/ c.p26.OAM_A_D7q = not(b.p31.OAM_A_D7);
  /*p26.XYKE*/ c.p26.OAM_A_D7r = not(b.p31.OAM_A_D7);
  /*p26.XABA*/ c.p26.OAM_A_D7s = not(b.p31.OAM_A_D7);
  /*p26.TAFU*/ c.p26.OAM_A_D7t = not(b.p31.OAM_A_D7);
  /*p26.XUHO*/ c.p26.OAM_A_D7u = not(b.p31.OAM_A_D7);

  /*p26.TEDE*/ c.p26.MASK_PIPE_SET0 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK0);
  /*p26.XALA*/ c.p26.MASK_PIPE_SET1 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK1);
  /*p26.TYRA*/ c.p26.MASK_PIPE_SET2 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK2);
  /*p26.XYRU*/ c.p26.MASK_PIPE_SET3 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK3);
  /*p26.XUKU*/ c.p26.MASK_PIPE_SET4 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK4);
  /*p26.XELY*/ c.p26.MASK_PIPE_SET5 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK5);
  /*p26.TYKO*/ c.p26.MASK_PIPE_SET6 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK6);
  /*p26.TUWU*/ c.p26.MASK_PIPE_SET7 = nand(b.p31.OAM_A_D7,  b.p34.SPRITE_MASK7);

  /*p26.WOKA*/ c.p26.MASK_PIPE_RST0 = nand(b.p26.OAM_A_D7n, b.p34.SPRITE_MASK0);
  /*p26.WEDE*/ c.p26.MASK_PIPE_RST1 = nand(b.p26.OAM_A_D7o, b.p34.SPRITE_MASK1);
  /*p26.TUFO*/ c.p26.MASK_PIPE_RST2 = nand(b.p26.OAM_A_D7p, b.p34.SPRITE_MASK2);
  /*p26.WEVO*/ c.p26.MASK_PIPE_RST3 = nand(b.p26.OAM_A_D7q, b.p34.SPRITE_MASK3);
  /*p26.WEDY*/ c.p26.MASK_PIPE_RST4 = nand(b.p26.OAM_A_D7r, b.p34.SPRITE_MASK4);
  /*p26.WUJA*/ c.p26.MASK_PIPE_RST5 = nand(b.p26.OAM_A_D7s, b.p34.SPRITE_MASK5);
  /*p26.TENA*/ c.p26.MASK_PIPE_RST6 = nand(b.p26.OAM_A_D7t, b.p34.SPRITE_MASK6);
  /*p26.WUBU*/ c.p26.MASK_PIPE_RST7 = nand(b.p26.OAM_A_D7u, b.p34.SPRITE_MASK7);

  /*p26.VEZO*/ c.p26.MASK_PIPE_0 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET0, b.p26.MASK_PIPE_RST0, b.p26.MASK_PIPE_0, 0);
  /*p26.WURU*/ c.p26.MASK_PIPE_1 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET1, b.p26.MASK_PIPE_RST1, b.p26.MASK_PIPE_1, b.p26.MASK_PIPE_0);
  /*p26.VOSA*/ c.p26.MASK_PIPE_2 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET2, b.p26.MASK_PIPE_RST2, b.p26.MASK_PIPE_2, b.p26.MASK_PIPE_1);
  /*p26.WYFU*/ c.p26.MASK_PIPE_3 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET3, b.p26.MASK_PIPE_RST3, b.p26.MASK_PIPE_3, b.p26.MASK_PIPE_2);
  /*p26.XETE*/ c.p26.MASK_PIPE_4 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET4, b.p26.MASK_PIPE_RST4, b.p26.MASK_PIPE_4, b.p26.MASK_PIPE_3);
  /*p26.WODA*/ c.p26.MASK_PIPE_5 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET5, b.p26.MASK_PIPE_RST5, b.p26.MASK_PIPE_5, b.p26.MASK_PIPE_4);
  /*p26.VUMO*/ c.p26.MASK_PIPE_6 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET6, b.p26.MASK_PIPE_RST6, b.p26.MASK_PIPE_6, b.p26.MASK_PIPE_5);
  /*p26.VAVA*/ c.p26.MASK_PIPE_7 = srtock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p26.MASK_PIPE_SET7, b.p26.MASK_PIPE_RST7, b.p26.MASK_PIPE_7, b.p26.MASK_PIPE_6);
}