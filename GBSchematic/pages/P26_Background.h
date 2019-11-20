#pragma once

struct Gameboy;

struct P26_Background {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);


  /*p26.AXAD*/ bool AXAD;
  /*p26.ASUL*/ bool ASUL;
  /*p26.ACEN*/ bool FETCH_MAPn;
  /*p26.BEJE*/ bool BEJE;
  /*p26.BAFY*/ bool FETCH_MAP;

  /*p26.FAFO*/ bool TILE_Y0;
  /*p26.EMUX*/ bool TILE_Y1;
  /*p26.ECAB*/ bool TILE_Y2;

  /*p26.ETAM*/ bool MAP_Y0;
  /*p26.DOTO*/ bool MAP_Y1;
  /*p26.DABA*/ bool MAP_Y2;
  /*p26.EFYK*/ bool MAP_Y3;
  /*p26.EJOK*/ bool MAP_Y4;

  /*p26.FAFO*/ bool TILE_Y0C;
  /*p26.EMUX*/ bool TILE_Y1C;
  /*p26.ECAB*/ bool TILE_Y2C;
  /*p26.ETAM*/ bool MAP_Y0C;
  /*p26.DOTO*/ bool MAP_Y1C;
  /*p26.DABA*/ bool MAP_Y2C;
  /*p26.EFYK*/ bool MAP_Y3C;
  /*p26.EJOK*/ bool MAP_Y4C;

  /*p26.ATAD*/ bool TILE_X0;
  /*p26.BEHU*/ bool TILE_X1;
  /*p26.APYH*/ bool TILE_X2;
  /*p26.BABE*/ bool MAP_X0;
  /*p26.ABOD*/ bool MAP_X1;
  /*p26.BEWY*/ bool MAP_X2;
  /*p26.BYCA*/ bool MAP_X3;
  /*p26.ACUL*/ bool MAP_X4;

  /*p26.ATAD*/ bool TILE_X0C;
  /*p26.BEHU*/ bool TILE_X1C;
  /*p26.APYH*/ bool TILE_X2C;
  /*p26.BABE*/ bool MAP_X0C;
  /*p26.ABOD*/ bool MAP_X1C;
  /*p26.BEWY*/ bool MAP_X2C;
  /*p26.BYCA*/ bool MAP_X3C;
  /*p26.ACUL*/ bool MAP_X4C;

  /*p26.XOGA*/ bool OAM_A_D7n;
  /*p26.XURA*/ bool OAM_A_D7o;
  /*p26.TAJO*/ bool OAM_A_D7p;
  /*p26.XENU*/ bool OAM_A_D7q;
  /*p26.XYKE*/ bool OAM_A_D7r;
  /*p26.XABA*/ bool OAM_A_D7s;
  /*p26.TAFU*/ bool OAM_A_D7t;
  /*p26.XUHO*/ bool OAM_A_D7u;

  /*p26.TEDE*/ bool MASK_PIPE_SET0;
  /*p26.XALA*/ bool MASK_PIPE_SET1;
  /*p26.TYRA*/ bool MASK_PIPE_SET2;
  /*p26.XYRU*/ bool MASK_PIPE_SET3;
  /*p26.XUKU*/ bool MASK_PIPE_SET4;
  /*p26.XELY*/ bool MASK_PIPE_SET5;
  /*p26.TYKO*/ bool MASK_PIPE_SET6;
  /*p26.TUWU*/ bool MASK_PIPE_SET7;

  /*p26.WOKA*/ bool MASK_PIPE_RST0;
  /*p26.WEDE*/ bool MASK_PIPE_RST1;
  /*p26.TUFO*/ bool MASK_PIPE_RST2;
  /*p26.WEVO*/ bool MASK_PIPE_RST3;
  /*p26.WEDY*/ bool MASK_PIPE_RST4;
  /*p26.WUJA*/ bool MASK_PIPE_RST5;
  /*p26.TENA*/ bool MASK_PIPE_RST6;
  /*p26.WUBU*/ bool MASK_PIPE_RST7;

  /*p26.VEZO*/ bool MASK_PIPE_0;
  /*p26.WURU*/ bool MASK_PIPE_1;
  /*p26.VOSA*/ bool MASK_PIPE_2;
  /*p26.WYFU*/ bool MASK_PIPE_3;
  /*p26.XETE*/ bool MASK_PIPE_4;
  /*p26.WODA*/ bool MASK_PIPE_5;
  /*p26.VUMO*/ bool MASK_PIPE_6;
  /*p26.VAVA*/ bool MASK_PIPE_7;
};



