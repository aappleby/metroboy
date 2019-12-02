#pragma once

namespace Schematics {

//-----------------------------------------------------------------------------

struct Video {
  /*p27.MOXE*/ bool CLK_xBxDxFxHa;
  /*p27.MEHE*/ bool CLK_xBxDxFxHb;
  /*p27.MYVO*/ bool CLK_xBxDxFxHc;
  /*p29.LAPE*/ bool CLK_xBxDxFxHd;
  /*p29.XOTA*/ bool CLK_xBxDxFxHe;

  /*p27.TAVA*/ bool CLK_AxCxExGxa;
  /*p29.XYVA*/ bool CLK_AxCxExGxb;
  /*p29.XYFY*/ bool CLK_AxCxExGxc;

  /*p29.WUVU*/ bool CLK_2Ma;
  /*p29.XUPY*/ bool CLK_2Mb;

  /*p21.VENA*/ bool CLK_1Ma;
  /*p21.TALU*/ bool CLK_1Mb;

  /*p21.XYMU*/ bool RENDERING;
  /*p21.NYPE*/ bool LINE_DONE_DELAYn;
  /*p21.WODU*/ bool RENDER_DONEn;
  /*p21.RUTU*/ bool LINE_DONEn;
  /*p21.NAPO*/ bool NAPO;
  /*p21.PURE*/ bool LINE_DONEa;

  /*p21.PARU*/ bool INT_VBL;
  /*p21.TAPA*/ bool INT_OAM;
  /*p21.TARU*/ bool INT_HBL;
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

  /*p21.SELA*/ bool LINE_DONEo;
  /*p21.XYVO*/ bool LINE_144;

  /*p21.ACAM*/ bool X0n;
  /*p21.AZUB*/ bool X1n;
  /*p21.AMEL*/ bool X2n;
  /*p21.AHAL*/ bool X3n;
  /*p21.APUX*/ bool X4n;
  /*p21.ABEF*/ bool X5n;
  /*p21.ADAZ*/ bool X6n;
  /*p21.ASAH*/ bool X7n;

  /*p21.TADY*/ bool TADY;
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

  /*p21.SYFU*/ bool LY_MATCH7;
  /*p21.TERY*/ bool LY_MATCH6;
  /*p21.TUCY*/ bool LY_MATCH5;
  /*p21.TYKU*/ bool LY_MATCH4;
  /*p21.RASY*/ bool LY_MATCH3;
  /*p21.REDA*/ bool LY_MATCH2;
  /*p21.TYDE*/ bool LY_MATCH1;
  /*p21.RYME*/ bool LY_MATCH0;
  /*p21.SOVU*/ bool LY_MATCHA;
  /*p21.SUBO*/ bool LY_MATCHB;
  /*p21.RAPE*/ bool LY_MATCHn;
  /*p21.PALY*/ bool LY_MATCH;

  /*p21.MUDE*/ bool CNT_RSTn;

  /*p21.SAXO*/ bool CNT_0;
  /*p21.TYPO*/ bool CNT_1;
  /*p21.VYZO*/ bool CNT_2;
  /*p21.TELU*/ bool CNT_3;
  /*p21.SUDE*/ bool CNT_4;
  /*p21.TAHA*/ bool CNT_5;
  /*p21.TYRY*/ bool CNT_6;

  /*p21.TOCU*/ bool CNT_0n;
  /*p21.VEPE*/ bool CNT_1n;
  /*p21.VUTY*/ bool CNT_2n;
  /*p21.TUJU*/ bool CNT_6n;
  /*p21.TAFY*/ bool CNT_5n;
  /*p21.TUDA*/ bool CNT_4n;
  /*p21.VATE*/ bool CNT_3n;

  /*p21.VOKU*/ bool CNT_000n;
  /*p21.TOZU*/ bool CNT_007n;
  /*p21.TECE*/ bool CNT_045n;
  /*p21.TEBO*/ bool CNT_083n;
  /*p21.TEGY*/ bool TEGY;
  /*p21.SANU*/ bool CNT_113n;
  /*p21.SONO*/ bool SONO_CLK;
  /*p21.SYGU*/ bool SYGU;
  /*p21.RYNO*/ bool RYNO;

  /*p21.POPU*/ bool LINE_144_SYNC;
  /*p21.XUGU*/ bool X_167n;
  /*p21.XENA*/ bool OAM_SCANn;
  /*p21.XANO*/ bool X_167;
  /*p21.TOLU*/ bool INT_VBLn;

  /*p21.SEPA*/ bool FF41_WR;
  /*p21.VOGA*/ bool VOGA;
  /*p21.WEGO*/ bool WEGO;
  /*p21.XAJO*/ bool X_009;
  /*p21.WUSA*/ bool WUSA;
  /*p21.TOBA*/ bool TOBA;
  /*p21.SADU*/ bool STAT_MODE0n;
  /*p21.XATY*/ bool STAT_MODE1n;
  /*p21.SEMU*/ bool CPn;
  /*p21.RYJU*/ bool FF41_WRn;
  /*p21.PAGO*/ bool STAT_LYC_MATCH1;
  /*p21.RYVE*/ bool FF41_WRo;
  /*p21.RUGU*/ bool INT_LYC_EN;
  /*p21.REFE*/ bool INT_OAM_EN;
  /*p21.ROPO*/ bool INT_LYC;
  /*p21.RUFO*/ bool INT_VBL_EN;
  /*p21.ROXE*/ bool INT_HBL_EN;

  /*p21.SUKO*/ bool INT_STATb;
  /*p21.TUVA*/ bool INT_STATn;
  /*p21.RUPO*/ bool STAT_LYC_MATCH2;

  /*p21.TOBE*/ bool FF41_RDa;
  /*p21.VAVE*/ bool FF41_RDb;

  /*p21.NOKO*/ bool LINE_147;
  /*p21.MYTA*/ bool LINE_147_SYNC;
  /*p21.LAMA*/ bool Y_RSTn;

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
  /*p22.WOFA*/ bool FF41n;
  /*p22.WEBU*/ bool FF42n;
  /*p22.WAVU*/ bool FF43n;
  /*p22.WYLE*/ bool FF44n;
  /*p22.WETY*/ bool FF45n;
  /*p22.WATE*/ bool FF46n;
  /*p22.WYBO*/ bool FF47n;
  /*p22.WETA*/ bool FF48n;
  /*p22.VAMA*/ bool FF49n;
  /*p22.WYVO*/ bool FF4An;
  /*p22.WAGE*/ bool FF4Bn;

  /*p22.VOCA*/ bool FF40;
  /*p22.VARY*/ bool FF41;
  /*p22.XARO*/ bool FF42;
  /*p22.XAVY*/ bool FF43;
  /*p22.XOGY*/ bool FF44;
  /*p22.XAYU*/ bool FF45;
  /*p22.XEDA*/ bool FF46;
  /*p22.WERA*/ bool FF47;
  /*p22.XAYO*/ bool FF48;
  /*p22.TEGO*/ bool FF49;
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
  /*p23.XYLY*/ bool XYLY;
  /*p23.XUFA*/ bool XUFA;
  /*p23.WANE*/ bool WANE;
  /*p23.WEKU*/ bool WEKU;

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

  // LY
  /*p23.WAFU*/ bool FF44_RD;
  /*p23.VARO*/ bool FF44_RDn;

  /*p23.WURY*/ bool LY0n;
  /*p23.XEPO*/ bool LY1n;
  /*p23.MYFA*/ bool LY2n;
  /*p23.XUHY*/ bool LY3n;
  /*p23.WATA*/ bool LY4n;
  /*p23.XAGA*/ bool LY5n;
  /*p23.XUCE*/ bool LY6n;
  /*p23.XOWO*/ bool LY7n;

  /*p24.SACU*/ bool CLKPIPE;
  /*p24.ROXO*/ bool FINE_MATCH_CLK;
  /*p24.POKY*/ bool POKY;
  /*p24.PORY*/ bool PORY;
  /*p24.TOMU*/ bool TOMU;
  /*p24.NYKA*/ bool NYKA;
  /*p24.SEGU*/ bool SEGU_4M;
  /*p24.LOBY*/ bool RENDERINGn;

  /*p24.NAFY*/ bool NAFY;
  /*p24.PYGO*/ bool PYGO;
  /*p24.SOCY*/ bool SOCY;
  /*p24.VYBO*/ bool VYBO;
  /*p24.TYFA*/ bool TYFA;
  /*p24.PAHO*/ bool PAHO;
  /*p24.POME*/ bool POME;
  /*p24.RUJU*/ bool RUJU;
  /*p24.POFY*/ bool POFY;
  /*p24.NERU*/ bool LINE_000n;
  /*p24.MEDA*/ bool LINE_000_SYNCn;
  /*p24.LOFU*/ bool LINE_DONE;
  /*p24.LUCA*/ bool LUCA;
  /*p24.MAGU*/ bool MAGU;
  /*p24.LEBE*/ bool LEBE;
  /*p24.MECO*/ bool MECO;
  /*p24.KEBO*/ bool KEBO;
  /*p24.KASA*/ bool KASA;
  /*p24.UMOB*/ bool UMOB;
  /*p24.USEC*/ bool USEC;
  /*p24.KAHE*/ bool CPLn;
  /*p24.KUPA*/ bool FRn;


  /*p25.SYRO*/ bool ADDR_FE00_FFFF;

  /*p25.COTA*/ bool OAM_IN_CLKn;

  // guess
  /*p25.CUFE*/ bool OAM_WR;

  /*p25.VAPE*/ bool VAPE;
  /*p25.AVER*/ bool AVER;
  /*p25.XUJY*/ bool XUJY;
  /*p25.BYCU*/ bool OAM_IN_CLK;
  /*p25.TEFA*/ bool TEFA;
  /*p25.SOSE*/ bool ADDR_VRAM;
  /*p25.TUCA*/ bool CPU_VRAM_RD;
  /*p25.TUJA*/ bool CPU_VRAM_WR;
  /*p25.TEGU*/ bool CPU_VRAM_CLK;
  /*p25.TAVY*/ bool MOE_Cn;
  /*p25.SYCY*/ bool MODE_DBG2n;
  /*p25.SOTO*/ bool DBG_SOTO;
  /*p25.TUTO*/ bool DBG_TUTO;
  /*p25.SUDO*/ bool MWR_Cn;
  /*p25.TEFY*/ bool MCS_Cn;

  /*p25.TOLE*/ bool CPU_VRAM_RD2;
  /*p25.TYJY*/ bool CPU_VRAM_WR2;
  /*p25.SALE*/ bool CPU_VRAM_CLK2;

  /*p25.RENA*/ bool MD_IN;
  /*p25.RYJE*/ bool MD_INb;
  /*p25.RAHU*/ bool D_TO_MD;

  /*p25.ROFA*/ bool MD_OUT;
  /*p25.RELA*/ bool MD_OUTb;
  /*p25.REVO*/ bool MD_OUTc;
  /*p25.SAZO*/ bool MD_OUTd;
  /*p25.ROCY*/ bool MD_OUTe;

  /*p25.ROPY*/ bool RENDERINGo;
  /*p25.SERE*/ bool SERE;
  /*p25.RUVY*/ bool CPU_VRAM_CLK2n;
  /*p25.SOHY*/ bool MWR;

  /*p25.RYLU*/ bool RYLU;
  /*p25.SOHO*/ bool SOHO;
  /*p25.RAWA*/ bool RAWA;
  /*p25.APAM*/ bool VRAM_TO_OAM;
  /*p25.SUTU*/ bool MCS;
  /*p25.RACU*/ bool MOE;
  /*p25.RACO*/ bool DBG_TUTOn;
  /*p25.RUTE*/ bool MOE_Dn;
  /*p25.SEWO*/ bool MCS_Dn;
  /*p25.TODE*/ bool MCS_An;
  /*p25.SEMA*/ bool MOE_An;
  /*p25.TAXY*/ bool MWR_An;
  /*p25.SOFY*/ bool MWR_Dn;

  // something debug
  /*p25.TUSO*/ bool TUSO;
  /*p25.SOLE*/ bool SOLE;

  /*p25.ROVE*/ bool D_TO_MDn;

  /*p25.CEDE*/ bool DO_DMAn;

  /*p25.XANE*/ bool A_TO_MAn;
  /*p25.XEDU*/ bool A_TO_MA;

  /*p25.TYVY*/ bool MD_TO_Dn;
  /*p25.SEBY*/ bool MD_TO_D;

  /*p25.VUZA*/ bool WIN_TILE_BANK;

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

  /*p27.LENA*/ bool LONYb;
  /*p27.MYMA*/ bool MYMA;
  /*p27.MOSU*/ bool MOSU;
  /*p27.LYRY*/ bool BG_SEQ_5;
  /*p27.SYLO*/ bool SYLO;
  /*p27.ROXY*/ bool FINE_MATCH_TRIG_OR_NOT_RENDERING;
  /*p27.POVA*/ bool FINE_MATCH_TRIG;
  /*p27.POTU*/ bool POTU;
  /*p27.PORE*/ bool TILE_Y_CLKn;
  /*p27.VYPO*/ bool P10_Bn;

  /*p27.MOFU*/ bool MOFU;
  /*p27.NYDY*/ bool BG_SEQ_TRIG_3n;
  /*p27.NYXU*/ bool BG_SEQ_RSTn;

  /*p27.SECA*/ bool SECA_RST;

  /*p27.SARY*/ bool WY_MATCH_SYNC;
  /*p27.REPU*/ bool IN_FRAME_Y;

  /*p27.NUKO*/ bool WIN_MATCH;

  /*p27.ROZE*/ bool FINE_COUNT_STOPn;


  /*p27.SEKO*/ bool WIN_TRIGGER;
  /*p27.ROMO*/ bool ROMO;
  /*p27.SUVU*/ bool SUVU;
  /*p27.TAVE*/ bool TAVE;
  /*p27.XAHY*/ bool NEW_LINEn;
  /*p27.XOFO*/ bool MAP_X_RST;
  /*p27.XACO*/ bool MAP_X_RSTn;
  /*p27.PYNU*/ bool TILE_ABb;
  /*p27.NUNY*/ bool NUNY;
  /*p27.NOCU*/ bool TILE_Y_CLK;
  /*p27.PUKU*/ bool PUKU;
  /*p27.RYDY*/ bool RYDY;
  /*p27.TUXY*/ bool TUXY;
  /*p27.SUZU*/ bool SUZU;
  /*p27.TEVO*/ bool MAP_X_CLK_STOPn;

  /*p27.VETU*/ bool MAP_X_CLK;
  /*p27.ROCO*/ bool ROCO_4M;
  /*p27.NYFO*/ bool NYFO;
  /*p27.WAZY*/ bool TILE_ABo;
  /*p27.SYNY*/ bool IN_FRAME_Yn;
  /*p27.NOPA*/ bool NOPA;
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
  /*p27.SOVY*/ bool SOVY;
  /*p27.MOCE*/ bool BG_SEQ_5n;
  /*p27.LEBO*/ bool BG_SEQ_CLK;
  /*p27.LAXE*/ bool BG_SEQ0n;
  /*p27.MYSO*/ bool BG_SEQ_TRIG_1357;
  /*p27.NOGU*/ bool BG_SEQ_xx234567;
  /*p27.NENY*/ bool BG_SEQ_01xxxxxx;
  /*p27.LURY*/ bool LURY;
  /*p27.LONY*/ bool LONY;
  /*p27.LUSU*/ bool LONYn;
  /*p27.LYZU*/ bool BG_SEQ_x1x3x5x7_DELAY;

  /*p27.LAXU*/ bool BG_SEQ_x1x3x5x7;
  /*p27.MESU*/ bool BG_SEQ_xx23xx67;
  /*p27.NYVA*/ bool BG_SEQ_xxxx4567;
  /*p27.XUHA*/ bool BG_SEQ_xxxx4567b;

  /*p27.NAKO*/ bool BG_SEQ_01xx45xx;
  /*p27.NOFU*/ bool BG_SEQ_0123xxxx;


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