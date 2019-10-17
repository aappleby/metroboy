// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool P10_B;
extern bool CLKPIPE;

extern bool XONO;
extern bool XADO;
extern bool PUCO;

extern bool LESY, LOTA, LYKU, ROBY, TYTA, TYCO, SOKA, XOVU;

//----------
// outputs

bool SPR_PIX_A_0;
bool SPR_PIX_A_1;
bool SPR_PIX_A_2;
bool SPR_PIX_A_3;
bool SPR_PIX_A_4;
bool SPR_PIX_A_5;
bool SPR_PIX_A_6;
bool SPR_PIX_A_7;

bool SPR_PIX_B_0;
bool SPR_PIX_B_1;
bool SPR_PIX_B_2;
bool SPR_PIX_B_3;
bool SPR_PIX_B_4;
bool SPR_PIX_B_5;
bool SPR_PIX_B_6;
bool SPR_PIX_B_7;

//----------
// registers

static reg PUDU;
static reg RAMU;
static reg SELE;
static reg SAJA;
static reg MYTU;
static reg MOFO;
static reg SUNY;
static reg SUTO;
static reg PEFO;
static reg REWO;
static reg RYDU;
static reg SEGA;
static reg PEBA;
static reg ROKA;
static reg RAMA;
static reg SEMO;

static reg NURO;
static reg MASO;
static reg LEFE;
static reg LESU;
static reg WYHO;
static reg WORA;
static reg VAFO;
static reg WUFY;

static reg NYLU;
static reg PEFU;
static reg NATY;
static reg PYJO;
static reg VARE;
static reg WEBA;
static reg VANU;
static reg VUPY;

//-----------------------------------------------------------------------------

void tick_spritepixelshifter() {
  // sprite x flip
  bool POBE = mux2(MD4, MD3, XONO);
  bool PACY = mux2(MD3, MD4, XONO);
  bool PONO = mux2(MD5, MD2, XONO);
  bool PUGU = mux2(MD2, MD5, XONO);
  bool PUTE = mux2(MD7, MD0, XONO);
  bool PULY = mux2(MD0, MD7, XONO);
  bool PELO = mux2(MD6, MD1, XONO);
  bool PAWE = mux2(MD1, MD6, XONO);

  // the schematic layout is slightly inconsistent, might have some of these swapped

  bool PUDU_Q = PUDU.latch(XADO, POBE);
  bool RAMU_Q = RAMU.latch(PUCO, POBE);
  bool SELE_Q = SELE.latch(PUCO, PACY);
  bool SAJA_Q = SAJA.latch(XADO, PACY);
  bool MYTU_Q = MYTU.latch(XADO, PONO);
  bool MOFO_Q = MOFO.latch(PUCO, PONO);
  bool SUNY_Q = SUNY.latch(XADO, PUGU);
  bool SUTO_Q = SUTO.latch(PUCO, PUGU);
  bool PEFO_Q = PEFO.latch(XADO, PUTE);
  bool REWO_Q = REWO.latch(PUCO, PUTE);
  bool RYDU_Q = RYDU.latch(PUCO, PULY);
  bool SEGA_Q = RYDU.latch(XADO, PULY);
  bool PEBA_Q = PEBA.latch(XADO, PELO);
  bool ROKA_Q = ROKA.latch(PUCO, PELO);
  bool RAMA_Q = RAMA.latch(PUCO, PAWE);
  bool SEMO_Q = SEMO.latch(XADO, PAWE);

  bool LUBO = not(PUDU_Q);
  bool SOLO = not(RAMU_Q);
  bool VOBY = not(RAMU_Q);
  bool WERY = not(RAMU_Q);
  bool LUMO = not(RAMU_Q);
  bool LASE = not(RAMU_Q);
  bool WURA = not(RAMU_Q);
  bool WYCO = not(RAMU_Q);
  bool LOZA = not(RAMU_Q);
  bool RATA = not(RAMU_Q);
  bool SELU = not(RAMU_Q);
  bool WAMY = not(RAMU_Q);
  bool NUCA = not(RAMU_Q);
  bool SYBO = not(RAMU_Q);
  bool SERY = not(RAMU_Q);
  bool SULU = not(RAMU_Q);

  bool LUFY = nand(LUBO,   ROBY);
  bool MAME = nand(PUDU_Q, ROBY);
  bool REHU = nand(SOLO,   ROBY);
  bool RANO = nand(RAMU_Q, ROBY);

  bool WAXO = nand(VOBY,   TYTA);
  bool TYGA = nand(SELE_Q, TYTA);
  bool XATO = nand(WERY,   TYTA);
  bool VEXU = nand(SAJA_Q, TYTA);

  bool MAJO = nand(LUMO,   LYKU);
  bool MYXA = nand(MYTU_Q, LYKU);
  bool LYDE = nand(LASE,   LYKU);
  bool LELA = nand(MOFO_Q, LYKU);

  bool XEXU = nand(WURA,   TYCO);
  bool VABY = nand(SUNY_Q, TYCO);
  bool XOLE = nand(WYCO,   TYCO);
  bool VUME = nand(SUTO_Q, TYCO);

  bool MOFY = nand(LOZA,   LESY);
  bool MEZU = nand(PEFO_Q, LESY);
  bool PYZU = nand(RATA,   LESY);
  bool PABE = nand(REWO_Q, LESY);

  bool TULA = nand(SELU,   XOVU);
  bool TESO = nand(RYDU_Q, XOVU);
  bool XYVE = nand(WAMY,   XOVU);
  bool VUNE = nand(SEGA_Q, XOVU);

  bool MADA = nand(NUCA,   LOTA);
  bool MYTO = nand(PEBA_Q, LOTA);
  bool RUCA = nand(SYBO,   LOTA);
  bool RUSY = nand(ROKA_Q, LOTA);

  bool TABY = nand(SERY,   SOKA);
  bool TAPO = nand(RAMA_Q, SOKA);
  bool TUPE = nand(SULU,   SOKA);
  bool TUXA = nand(SEMO_Q, SOKA);

  bool NURO_Q = NURO.srtock(CLKPIPE, PABE, PYZU, P10_B);
  bool MASO_Q = MASO.srtock(CLKPIPE, MYTO, MADA, NURO_Q);
  bool LEFE_Q = LEFE.srtock(CLKPIPE, LELA, LYDE, MASO_Q);
  bool LESU_Q = LESU.srtock(CLKPIPE, MAME, LUFY, LEFE_Q);
  bool WYHO_Q = WYHO.srtock(CLKPIPE, VEXU, XATO, LESU_Q);
  bool WORA_Q = WORA.srtock(CLKPIPE, VABY, XEXU, WYHO_Q);
  bool VAFO_Q = VAFO.srtock(CLKPIPE, TUXA, TUPE, WORA_Q);
  bool WUFY_Q = WUFY.srtock(CLKPIPE, VUNE, XYVE, VAFO_Q);

  bool NYLU_Q = NYLU.srtock(CLKPIPE, MEZU, MOFY, P10_B);
  bool PEFU_Q = PEFU.srtock(CLKPIPE, RUSY, RUCA, NYLU_Q);
  bool NATY_Q = NATY.srtock(CLKPIPE, MYXA, MAJO, PEFU_Q);
  bool PYJO_Q = PYJO.srtock(CLKPIPE, RANO, REHU, NATY_Q);
  bool VARE_Q = VARE.srtock(CLKPIPE, TYGA, WAXO, PYJO_Q);
  bool WEBA_Q = WEBA.srtock(CLKPIPE, VUME, XOLE, VARE_Q);
  bool VANU_Q = VANU.srtock(CLKPIPE, TAPO, TABY, WEBA_Q);
  bool VUPY_Q = VUPY.srtock(CLKPIPE, TESO, TULA, VANU_Q);

  SPR_PIX_B_0 = NURO_Q;
  SPR_PIX_B_1 = MASO_Q;
  SPR_PIX_B_2 = LEFE_Q;
  SPR_PIX_B_3 = LESU_Q;
  SPR_PIX_B_4 = WYHO_Q;
  SPR_PIX_B_5 = WORA_Q;
  SPR_PIX_B_6 = VAFO_Q;
  SPR_PIX_B_7 = WUFY_Q;

  SPR_PIX_A_0 = NYLU_Q;
  SPR_PIX_A_1 = PEFU_Q;
  SPR_PIX_A_2 = NATY_Q;
  SPR_PIX_A_3 = PYJO_Q;
  SPR_PIX_A_4 = VARE_Q;
  SPR_PIX_A_5 = WEBA_Q;
  SPR_PIX_A_6 = VANU_Q;
  SPR_PIX_A_7 = VUPY_Q;
}