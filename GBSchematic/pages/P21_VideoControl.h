#pragma once

struct P21_VideoControl {

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

  /*p21.XYMU*/ bool XYMU;
  /*p21.NYPE*/ bool LINE_DONE_DELAYn;
  /*p21.WODU*/ bool WODU;
  /*p21.RUTU*/ bool LINE_DONEn;
  /*p21.NAPO*/ bool NAPO;
  /*p21.PURE*/ bool LINE_DONEa;

  /*p21.POGU*/ bool PIN_CPG;
  /*p21.RYPO*/ bool CP;
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
  /*p21.XENA*/ bool XENA;
  /*p21.XANO*/ bool X_167;
  /*p21.TOLU*/ bool INT_VBLn;

  /*p21.SEPA*/ bool FF41_WR;
  /*p21.VOGA*/ bool VOGA;
  /*p21.WEGO*/ bool WEGO;
  /*p21.XAJO*/ bool XAJO;
  /*p21.WUSA*/ bool WUSA;
  /*p21.TOBA*/ bool TOBA;
  /*p21.SADU*/ bool STAT_MODE0;
  /*p21.XATY*/ bool STAT_MODE1;
  /*p21.SEMU*/ bool SEMU;
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
};
