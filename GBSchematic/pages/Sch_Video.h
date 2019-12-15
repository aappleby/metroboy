#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Video {
  /*p21.XYMU*/ bool RENDERING;

  /*p21.WODU*/ bool RENDER_DONE;
  /*p21.VOGA*/ Reg RENDER_DONE_SYNC;

  /*p21.NAPO*/ bool FRAME_EVEN;


  /*p21.VOTY*/ bool INT_STAT;

  // x counter
  /*p21.XEHO*/ Reg X0;
  /*p21.SAVY*/ Reg X1;
  /*p21.XODU*/ Reg X2;
  /*p21.XYDO*/ Reg X3;
  /*p21.TUHU*/ Reg X4;
  /*p21.TUKY*/ Reg X5;
  /*p21.TAKO*/ Reg X6;
  /*p21.SYBE*/ Reg X7;

#if 0
  /*p21.ACAM*/ bool X0n;
  /*p21.AZUB*/ bool X1n;
  /*p21.AMEL*/ bool X2n;
  /*p21.AHAL*/ bool X3n;
  /*p21.APUX*/ bool X4n;
  /*p21.ABEF*/ bool X5n;
  /*p21.ADAZ*/ bool X6n;
  /*p21.ASAH*/ bool X7n;
#endif

  /*p21.TADY*/ bool X_RST;

  /*p21.SYGU*/ bool LINE_STROBE;

  /*p21.XUGU*/ bool X_167n;

  /*p21.XANO*/ bool X_167;




  /*p21.ROPO*/ Reg LYC_MATCH;

  /*p21.RUGU*/ Reg INT_LYC_EN;
  /*p21.REFE*/ Reg INT_OAM_EN;
  /*p21.RUFO*/ Reg INT_VBL_EN;
  /*p21.ROXE*/ Reg INT_HBL_EN;



  /*p22.XALY*/ bool ADDR_0x00xxxx;
  /*p22.WUTU*/ bool FF4Xn;
  /*p22.WERO*/ bool FF4X;

  /*p22.XOLA*/ bool A00n;
  /*p22.XENO*/ bool A01n;
  /*p22.XUSY*/ bool A02n;
  /*p22.XERA*/ bool A03n;

  /*p22.WADO*/ bool WADO_A00;
  /*p22.WESA*/ bool WESA_A01;
  /*p22.WALO*/ bool WALO_A02;
  /*p22.WEPO*/ bool WEPO_A03;

  /*p22.WORU*/ bool FF40n;
  /*p22.WEBU*/ bool FF42n;
  /*p22.WAVU*/ bool FF43n;

  /*p22.VOCA*/ bool FF40;
  /*p22.XARO*/ bool FF42;
  /*p22.XAVY*/ bool FF43;


  /*p22.VYGA*/ bool FF4A;
  /*p22.VUMY*/ bool FF4B;


  /*p24.SACU*/ bool CLKPIPE;
  /*p24.SEGU*/ bool SEGU_4M;
  /*p24.ROXO*/ bool ROXO_4M;

  /*p27.NOGU*/ bool BG_SEQ_xx234567;
  /*p27.LYZU*/ Reg BG_SEQ_x1x3x5x7_DELAY;
  
  /*p27.LAXU*/ Reg BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ Reg BG_SEQ_xx23xx67;
  /*p27.NYVA*/ Reg BG_SEQ_xxxx4567;
  
  /*p27.LOVY*/ Reg BG_SEQ5_SYNC;
  /*p24.NYKA*/ Reg BG_SEQ_6;
  /*p24.PORY*/ Reg BG_SEQ_7;
  /*p24.PYGO*/ Reg BG_SEQ_8;

  /*p24.LOBY*/ bool RENDERINGn;


  /*p24.PAHO*/ Reg PAHO; // something related to hsync



  /*p24.MEDA*/ bool VSYNC_OUTn;
  /*p24.LOFU*/ bool CLK_LINE_EVEN;
  /*p24.LUCA*/ bool LINE_EVEN;



  /*p25.SYRO*/ bool ADDR_FE00_FFFF;

  /*p25.COTA*/ bool OAM_IN_CLKn;







  /*p25.SOSE*/ bool ADDR_VRAM;



  /*p26.VEZO*/ bool MASK_PIPE_0;
  /*p26.WURU*/ bool MASK_PIPE_1;
  /*p26.VOSA*/ bool MASK_PIPE_2;
  /*p26.WYFU*/ bool MASK_PIPE_3;
  /*p26.XETE*/ bool MASK_PIPE_4;
  /*p26.WODA*/ bool MASK_PIPE_5;
  /*p26.VUMO*/ bool MASK_PIPE_6;
  /*p26.VAVA*/ bool MASK_PIPE_7;

  /*p27.LENA*/ bool LONYb;
  

  /*p27.LYRY*/ bool BG_SEQ_5;

  /*p27.POVA*/ bool FINE_MATCH_TRIG;

  /*p27.NYXU*/ bool BG_SEQ_RSTn;

  /*p27.SARY*/ Reg WY_MATCH_SYNC;


  /*p27.NUKO*/ bool WIN_MATCH;




  // NOR SR latch
  /*p27.PUKU*/ bool PUKU;
  /*p27.RYDY*/ bool RYDY;


  /*p27.TEVO*/ bool MAP_X_CLK_STOPn;

  /*p27.ROCO*/ bool ROCO_4M;

  /*p27.PYNU*/ bool WIN_RST;
  /*p27.NOPA*/ Reg WIN_RST_SYNC;
  /*p27.NUNY*/ bool WIN_RST_TRIG;



  /*p27.PYCO*/ Reg WIN_MATCH_SYNC1;
  /*p27.NUNU*/ Reg WIN_MATCH_SYNC2;

  /*p27.RYKU*/ Reg FINE_CNT0;
  /*p27.ROGA*/ Reg FINE_CNT1;
  /*p27.RUBU*/ Reg FINE_CNT2;

  /*p27.PUXA*/ Reg FINE_MATCH_SYNC1;
  /*p27.NYZE*/ Reg FINE_MATCH_SYNC2;
  /*p27.RYFA*/ Reg WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ Reg WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.SEKO*/ bool WIN_TRIGGER;
  /*p27.SOVY*/ Reg REG_SOVY;


  /*p27.WYKA*/ Reg MAP_X0;
  /*p27.WODY*/ Reg MAP_X1;
  /*p27.WOBO*/ Reg MAP_X2;
  /*p27.WYKO*/ Reg MAP_X3;
  /*p27.XOLO*/ Reg MAP_X4;

  /*p27.VYNO*/ Reg TILE_Y0;
  /*p27.VUJO*/ Reg TILE_Y1;
  /*p27.VYMU*/ Reg TILE_Y2;

  /*p27.TUFU*/ Reg MAP_Y0;
  /*p27.TAXA*/ Reg MAP_Y1;
  /*p27.TOZO*/ Reg MAP_Y2;
  /*p27.TATE*/ Reg MAP_Y3;
  /*p27.TEKE*/ Reg MAP_Y4;

  /*p27.VEVY*/ bool MA10;
  /*p27.VEZA*/ bool MA11;
  /*p27.VOGU*/ bool MA12;

  ///*p27.TAVA*/ bool TAVA_AxCxExGx;
  /*p27.SOBU*/ Reg TEKY_SYNC1;
  /*p27.SUDA*/ Reg TEKY_SYNC2;
};

//-----------------------------------------------------------------------------

};