#pragma once

struct P21_VideoControl {
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  bool INT_VBL_BUF;
  bool INT_STAT;

  bool XYMU;
  bool NYPE;
  bool LYFE;
  bool WODU;
  bool RUTU;
  bool NAPO;
  bool PURE;

  union { bool POGU; bool PIN_CPG; };
  union { bool RYPO; bool CP; };

  union { bool PARU; bool INT_VBL; };
  union { bool TAPA; bool INT_OAM; };
  union { bool TARU; bool INT_HBL; };
  union { bool VYPU; bool INT_VBL_BUF; };
  union { bool VOTY; bool INT_STAT; };

  // y counter
  union { bool MUWY; bool V0; };
  union { bool MYRO; bool V1; };
  union { bool LEXA; bool V2; };
  union { bool LYDO; bool V3; };
  union { bool LOVU; bool V4; };
  union { bool LEMA; bool V5; };
  union { bool MATO; bool V6; };
  union { bool LAFO; bool V7; };

  // x counter
  bool XEHO;
  bool SAVY;
  bool XODU;
  bool XYDO;
  bool TUHU;
  bool TUKY;
  bool TAKO;
  bool SYBE;

  bool TALU;

  // x coordinate? used by sprite matcher
  bool ACAM,AZUB,AMEL,AHAL,APUX,ABEF,ADAZ,ASAH;

private:

  bool LYHA;

  bool TADY;
  bool XUKE,XYLE,RYBO,XEGY,XORA;
  bool TOCA;
  bool TYBA,SURY,SAKE,TYGE,ROKU;

  bool SYFU,TERY,TUCY,TYKU,RASY,REDA,TYDE,RYME;
  bool SOVU,SUBO,RAPE,PALY;

  bool VENA,MUDE;
  bool SAXO,TYPO,VYZO,TELU,SUDE,TAHA,TYRY;
  bool TOCU,VEPE,VUTY,TUJU,TAFY,TUDA,VATE;
  bool VOKU,TOZU,TECE,TEBO,TEGY;
  bool SANU,SONO,SYGU,RYNO;

  bool XYVO;
  bool POPU;
  bool XUGU,XENA,XANO;
  bool SELA,TOLU;

  bool SEPA;
  bool VOGA,WEGO,XAJO,WUSA,TOBA,SADU,XATY,SEMU,RYJU,PAGO;
  bool RYVE,RUGU,REFE,ROPO,RUFO,ROXE;
  bool PUZO,SASY,POFO,POTE,TEBY,WUGA;
  bool SUKO,TUVA,RUPO,SEGO;
  bool TOBE,VAVE;

  bool NOKO,MYTA,LAMA;
};
