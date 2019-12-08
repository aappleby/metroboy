
#pragma once

namespace Schematics {

//-----------------------------------------------------------------------------

struct Video {
  /*p27.MOXE*/ bool CLK_xBxDxFxHa;
  /*p27.MEHE*/ bool CLK_xBxDxFxHb;
  /*p27.MYVO*/ bool CLK_xBxDxFxHc;
  /*p29.LAPE*/ bool CLK_xBxDxFxHd;
  /*p29.XOTA*/ bool CLK_xBxDxFxHe;

  /*p29.XYVA*/ bool CLK_AxCxExGxb;
  /*p29.XYFY*/ bool CLK_AxCxExGxc;
  /*p27.TAVA*/ bool CLK_AxCxExGxa; // tag wrong?

  /*p29.WUVU*/ bool CLK_AxxDExxHa;
  /*p29.XUPY*/ bool CLK_AxxDExxHb;
  /*p21.VENA*/ bool CLK_AxxxxFGHa;
  /*p21.TALU*/ bool CLK_AxxxxFGHb;
  /*p21.SONO*/ bool CLK_xBCDExxxa;

  /*p21.XYMU*/ bool RENDERING;
  /*p21.NYPE*/ bool LINE_DONE_SYNCb;

  /*p21.WODU*/ bool RENDER_DONE;
  /*p21.VOGA*/ bool RENDER_DONE_SYNC;

  /*p21.RUTU*/ bool LINE_DONE_SYNCa;
  /*p21.NAPO*/ bool FRAME_EVEN;




  /*p21.VYPU*/ bool INT_VBL_BUF;
  /*p21.VOTY*/ bool INT_STAT;

  /*p21.MUWY*/ bool V0; // y counter
  /*p21.MYRO*/ bool V1;
  /*p21.LEXA*/ bool V2;
  /*p21.LYDO*/ bool V3;
  /*p21.LOVU*/ bool V4;
  /*p21.LEMA*/ bool V5;
  /*p21.MATO*/ bool V6;
  /*p21.LAFO*/ bool V7;

  // x counter
  /*p21.XEHO*/ bool X0;
  /*p21.SAVY*/ bool X1;
  /*p21.XODU*/ bool X2;
  /*p21.XYDO*/ bool X3;
  /*p21.TUHU*/ bool X4;
  /*p21.TUKY*/ bool X5;
  /*p21.TAKO*/ bool X6;
  /*p21.SYBE*/ bool X7;

  /*p21.ACAM*/ bool X0n;
  /*p21.AZUB*/ bool X1n;
  /*p21.AMEL*/ bool X2n;
  /*p21.AHAL*/ bool X3n;
  /*p21.APUX*/ bool X4n;
  /*p21.ABEF*/ bool X5n;
  /*p21.ADAZ*/ bool X6n;
  /*p21.ASAH*/ bool X7n;

  /*p21.TADY*/ bool X_RST;
  /*p21.XUKE*/ bool XUKE;
  /*p21.XYLE*/ bool XYLE;
  /*p21.RYBO*/ bool RYBO;
  /*p21.XEGY*/ bool XEGY;
  /*p21.XORA*/ bool XORA;
  /*p21.TOCA*/ bool TOCA;
  /*p21.TYBA*/ bool TYBA;
  /*p21.SURY*/ bool SURY;
  /*p21.SAKE*/ bool SAKE;
  /*p21.TYGE*/ bool TYGE;
  /*p21.ROKU*/ bool ROKU;

  /*p21.SAXO*/ bool CNT_0;
  /*p21.TYPO*/ bool CNT_1;
  /*p21.VYZO*/ bool CNT_2;
  /*p21.TELU*/ bool CNT_3;
  /*p21.SUDE*/ bool CNT_4;
  /*p21.TAHA*/ bool CNT_5;
  /*p21.TYRY*/ bool CNT_6;

  /*p21.SYGU*/ bool LINE_STROBE;

  /*p21.POPU*/ bool REG_VBLANK;
  /*p21.XUGU*/ bool X_167n;

  /*p21.XANO*/ bool X_167;



  /*p21.RYVE*/ bool CLK_STAT;
  /*p21.ROPO*/ bool LYC_MATCH;

  /*p21.RUGU*/ bool INT_LYC_EN;
  /*p21.REFE*/ bool INT_OAM_EN;
  /*p21.RUFO*/ bool INT_VBL_EN;
  /*p21.ROXE*/ bool INT_HBL_EN;


  /*p21.MYTA*/ bool LINE_147_SYNC;
  

  /*p22.XALY*/ bool ADDR_0x00xxxx;
  /*p22.WUTU*/ bool FF4Xn;
  /*p22.WERO*/ bool FF4X;

  /*p22.XOLA*/ bool A00n;
  /*p22.XENO*/ bool A01n;
  /*p22.XUSY*/ bool A02n;
  /*p22.XERA*/ bool A03n;

  /*p22.WADO*/ bool A00;
  /*p22.WESA*/ bool A01;
  /*p22.WALO*/ bool A02;
  /*p22.WEPO*/ bool A03;

  /*p22.WORU*/ bool FF40n;
  /*p22.WEBU*/ bool FF42n;
  /*p22.WAVU*/ bool FF43n;


  /*p22.WYVO*/ bool FF4An;
  /*p22.WAGE*/ bool FF4Bn;

  /*p22.VOCA*/ bool FF40;
  /*p22.XARO*/ bool FF42;
  /*p22.XAVY*/ bool FF43;


  /*p22.VYGA*/ bool FF4A;
  /*p22.VUMY*/ bool FF4B;

  // LCDC
  /*p23.VYRE*/ bool FF40_RD;
  /*p23.WARU*/ bool FF40_WR;
  /*p23.WYCE*/ bool FF40_RDn;
  /*p23.XUBO*/ bool FF40_WRn;

  /*p23.VYXE*/ bool LCDC_BGEN;
  /*p23.XYLO*/ bool LCDC_SPEN;
  /*p23.XYMO*/ bool LCDC_SPSIZE;
  /*p23.XAFO*/ bool LCDC_BGMAP;
  /*p23.WEXU*/ bool BG_TILE_SEL;
  /*p23.WYMO*/ bool LCDC_WINEN;
  /*p23.WOKY*/ bool WIN_MAP_SEL;
  /*p23.XONA*/ bool LCDC_EN;

  // SCY
  /*p23.ANYP*/ bool ANYP;
  /*p23.BEDY*/ bool BEDY;
  /*p23.CAVO*/ bool CAVO;
  /*p23.BUWY*/ bool BUWY;

  /*p23.GAVE*/ bool SCY0;
  /*p23.FYMO*/ bool SCY1;
  /*p23.FEZU*/ bool SCY2;
  /*p23.FUJO*/ bool SCY3;
  /*p23.DEDE*/ bool SCY4;
  /*p23.FOTY*/ bool SCY5;
  /*p23.FOHA*/ bool SCY6;
  /*p23.FUNY*/ bool SCY7;

  // SCX
  /*p23.AVOG*/ bool AVOG;
  /*p23.ARUR*/ bool ARUR;
  /*p23.AMUN*/ bool AMUN;
  /*p23.BEBA*/ bool BEBA;

  /*p23.DATY*/ bool SCX0;
  /*p23.DUZU*/ bool SCX1;
  /*p23.CYXU*/ bool SCX2;
  /*p23.GUBO*/ bool SCX3;
  /*p23.BEMY*/ bool SCX4;
  /*p23.CUZY*/ bool SCX5;
  /*p23.CABU*/ bool SCX6;
  /*p23.BAKE*/ bool SCX7;

  // LYC
  /*p23.WANE*/ bool CLK_LYC;

  /*p23.SYRY*/ bool LYC0;
  /*p23.VUCE*/ bool LYC1;
  /*p23.SEDY*/ bool LYC2;
  /*p23.SALO*/ bool LYC3;
  /*p23.SOTA*/ bool LYC4;
  /*p23.VAFA*/ bool LYC5;
  /*p23.VEVO*/ bool LYC6;
  /*p23.RAHA*/ bool LYC7;

  // WY
  /*p23.WAXU*/ bool WAXU;
  /*p23.WEKO*/ bool WEKO;
  /*p23.VEFU*/ bool VEFU;
  /*p23.VOMY*/ bool VOMY;

  /*p23.NESO*/ bool WY0;
  /*p23.NYRO*/ bool WY1;
  /*p23.NAGA*/ bool WY2;
  /*p23.MELA*/ bool WY3;
  /*p23.NULO*/ bool WY4;
  /*p23.NENE*/ bool WY5;
  /*p23.NUKA*/ bool WY6;
  /*p23.NAFU*/ bool WY7;

  // WX
  /*p23.WYZE*/ bool WYZE;
  /*p23.WUZA*/ bool WUZA;
  /*p23.VYCU*/ bool VYCU;
  /*p23.VOXU*/ bool VOXU;

  /*p23.MYPA*/ bool WX0;
  /*p23.NOFE*/ bool WX1;
  /*p23.NOKE*/ bool WX2;
  /*p23.MEBY*/ bool WX3;
  /*p23.MYPU*/ bool WX4;
  /*p23.MYCE*/ bool WX5;
  /*p23.MUVO*/ bool WX6;
  /*p23.NUKU*/ bool WX7;


  /*p24.SACU*/ bool CLKPIPE;
  /*p24.ROXO*/ bool FINE_MATCH_CLK;

  /*p24.NYKA*/ bool REG_SEQ5a;
  /*p24.PORY*/ bool REG_SEQ5b;
  /*p24.PYGO*/ bool REG_SEQ5c;

  /*p24.SEGU*/ bool SEGU_4M;
  /*p24.LOBY*/ bool RENDERINGn;




  /*p24.PAHO*/ bool PAHO;



  /*p24.MEDA*/ bool LINE_000_SYNCn;
  /*p24.LOFU*/ bool CLK_LINE_EVEN;
  /*p24.LUCA*/ bool LINE_EVEN;
  /*p24.LEBE*/ bool LEBE;



  /*p25.SYRO*/ bool ADDR_FE00_FFFF;

  /*p25.COTA*/ bool OAM_IN_CLKn;






  /*p25.TEFA*/ bool TEFA;
  /*p25.SOSE*/ bool ADDR_VRAM;


  /*p25.SYCY*/ bool CLK_SOTO;
  /*p25.SOTO*/ bool DBG_SOTO;








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

  /*p27.SARY*/ bool WY_MATCH_SYNC;


  /*p27.NUKO*/ bool WIN_MATCH;






  /*p27.NOCU*/ bool TILE_Y_CLK;

  // combi loop
  /*p27.RYDY*/ bool RYDY;


  /*p27.TEVO*/ bool MAP_X_CLK_STOPn;

  /*p27.VETU*/ bool MAP_X_CLK;
  /*p27.ROCO*/ bool ROCO_4M;



  /*p27.NOPA*/ bool REG_NOPA;
  /*p27.PYCO*/ bool WIN_MATCH_SYNC1;
  /*p27.NUNU*/ bool WIN_MATCH_SYNC2;

  /*p27.PECU*/ bool FINE_CLK;
  /*p27.RYKU*/ bool FINE_CNT0;
  /*p27.ROGA*/ bool FINE_CNT1;
  /*p27.RUBU*/ bool FINE_CNT2;

  /*p27.NYZE*/ bool FINE_MATCH_SYNC2;
  /*p27.PUXA*/ bool FINE_MATCH_SYNC1;
  /*p27.RYFA*/ bool WIN_MATCH_ONSCREEN_SYNC1;
  /*p27.RENE*/ bool WIN_MATCH_ONSCREEN_SYNC2;
  /*p27.SOVY*/ bool REG_SOVY;

  /*p27.LEBO*/ bool BG_SEQ_CLK;
  /*p27.NOGU*/ bool BG_SEQ_xx234567;


  /*p27.LYZU*/ bool BG_SEQ_x1x3x5x7_DELAY;

  /*p27.LAXU*/ bool BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ bool BG_SEQ_xx23xx67;
  /*p27.NYVA*/ bool BG_SEQ_xxxx4567;


  /*p27.LOVY*/ bool BG_SEQ5_SYNC;

  /*p27.WYKA*/ bool MAP_X0;
  /*p27.WODY*/ bool MAP_X1;
  /*p27.WOBO*/ bool MAP_X2;
  /*p27.WYKO*/ bool MAP_X3;
  /*p27.XOLO*/ bool MAP_X4;

  /*p27.VYNO*/ bool TILE_Y0;
  /*p27.VUJO*/ bool TILE_Y1;
  /*p27.VYMU*/ bool TILE_Y2;

  /*p27.TUFU*/ bool MAP_Y0;
  /*p27.TAXA*/ bool MAP_Y1;
  /*p27.TOZO*/ bool MAP_Y2;
  /*p27.TATE*/ bool MAP_Y3;
  /*p27.TEKE*/ bool MAP_Y4;

  /*p27.VEVY*/ bool MA10;
  /*p27.VEZA*/ bool MA11;
  /*p27.VOGU*/ bool MA12;

  /*p27.RYCE*/ bool TEKY_TRIG;
  /*p27.VEKU*/ bool VEKU;
  /*p27.TAKA*/ bool TAKA;
  /*p27.TUKU*/ bool TUKU;
  /*p27.SOWO*/ bool SOWO;
  /*p27.TEKY*/ bool TEKY;
  /*p27.SOBU*/ bool TEKY_SYNC1;
  /*p27.SUDA*/ bool TEKY_SYNC2;
};

//-----------------------------------------------------------------------------

};