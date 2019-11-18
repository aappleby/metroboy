#pragma once

struct P21_VideoControl {

  /*p27.MOXE*/ bool MOXE;
  /*p27.MEHE*/ bool MEHE;
  /*p27.MYVO*/ bool MYVO;
  /*p27.TAVA*/ bool TAVA;

  /*p21.XYMU*/ bool XYMU;
  /*p21.NYPE*/ bool NYPE;
  /*p21.WODU*/ bool WODU;
  /*p21.RUTU*/ bool RUTU;
  /*p21.NAPO*/ bool NAPO;
  /*p21.PURE*/ bool PURE;

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
  /*p21.XEHO*/ bool XEHO;
  /*p21.SAVY*/ bool SAVY;
  /*p21.XODU*/ bool XODU;
  /*p21.XYDO*/ bool XYDO;
  /*p21.TUHU*/ bool TUHU;
  /*p21.TUKY*/ bool TUKY;
  /*p21.TAKO*/ bool TAKO;
  /*p21.SYBE*/ bool SYBE;

  /*p21.TALU*/ bool TALU; // this is a clock
  /*p21.SELA*/ bool SELA;
  /*p21.XYVO*/ bool LINE_144;

  /*p21.ACAM*/ bool ACAM; // x coordinate? used by sprite matcher
  /*p21.AZUB*/ bool AZUB;
  /*p21.AMEL*/ bool AMEL;
  /*p21.AHAL*/ bool AHAL;
  /*p21.APUX*/ bool APUX;
  /*p21.ABEF*/ bool ABEF;
  /*p21.ADAZ*/ bool ADAZ;
  /*p21.ASAH*/ bool ASAH;

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

  /*p21.SYFU*/ bool SYFU;
  /*p21.TERY*/ bool TERY;
  /*p21.TUCY*/ bool TUCY;
  /*p21.TYKU*/ bool TYKU;
  /*p21.RASY*/ bool RASY;
  /*p21.REDA*/ bool REDA;
  /*p21.TYDE*/ bool TYDE;
  /*p21.RYME*/ bool RYME;
  /*p21.SOVU*/ bool SOVU;
  /*p21.SUBO*/ bool SUBO;
  /*p21.RAPE*/ bool RAPE;
  /*p21.PALY*/ bool PALY;

  /*p21.VENA*/ bool VENA;
  /*p21.MUDE*/ bool MUDE;
  /*p21.SAXO*/ bool SAXO;
  /*p21.TYPO*/ bool TYPO;
  /*p21.VYZO*/ bool VYZO;
  /*p21.TELU*/ bool TELU;
  /*p21.SUDE*/ bool SUDE;
  /*p21.TAHA*/ bool TAHA;
  /*p21.TYRY*/ bool TYRY;
  /*p21.TOCU*/ bool TOCU;
  /*p21.VEPE*/ bool VEPE;
  /*p21.VUTY*/ bool VUTY;
  /*p21.TUJU*/ bool TUJU;
  /*p21.TAFY*/ bool TAFY;
  /*p21.TUDA*/ bool TUDA;
  /*p21.VATE*/ bool VATE;
  /*p21.VOKU*/ bool VOKU;
  /*p21.TOZU*/ bool TOZU;
  /*p21.TECE*/ bool TECE;
  /*p21.TEBO*/ bool TEBO;
  /*p21.TEGY*/ bool TEGY;
  /*p21.SANU*/ bool SANU;
  /*p21.SONO*/ bool SONO;
  /*p21.SYGU*/ bool SYGU;
  /*p21.RYNO*/ bool RYNO;

  /*p21.POPU*/ bool LINE_144_SYNC;
  /*p21.XUGU*/ bool XUGU;
  /*p21.XENA*/ bool XENA;
  /*p21.XANO*/ bool XANO;
  /*p21.TOLU*/ bool TOLU;

  /*p21.SEPA*/ bool SEPA;
  /*p21.VOGA*/ bool VOGA;
  /*p21.WEGO*/ bool WEGO;
  /*p21.XAJO*/ bool XAJO;
  /*p21.WUSA*/ bool WUSA;
  /*p21.TOBA*/ bool TOBA;
  /*p21.SADU*/ bool STAT_MODE0;
  /*p21.XATY*/ bool STAT_MODE1;
  /*p21.SEMU*/ bool SEMU;
  /*p21.RYJU*/ bool RYJU;
  /*p21.PAGO*/ bool PAGO;
  /*p21.RYVE*/ bool RYVE;
  /*p21.RUGU*/ bool INT_LYC_EN;
  /*p21.REFE*/ bool INT_OAM_EN;
  /*p21.ROPO*/ bool INT_LYC;
  /*p21.RUFO*/ bool INT_VBL_EN;
  /*p21.ROXE*/ bool INT_HBL_EN;

  /*p21.SUKO*/ bool INT_STATb;
  /*p21.TUVA*/ bool INT_STATn;
  /*p21.RUPO*/ bool STAT_LYC_MATCH;

  /*p21.TOBE*/ bool FF41_RDa;
  /*p21.VAVE*/ bool FF41_RDb;

  /*p21.NOKO*/ bool NOKO;
  /*p21.MYTA*/ bool MYTA;
  /*p21.LAMA*/ bool LAMA;
};
