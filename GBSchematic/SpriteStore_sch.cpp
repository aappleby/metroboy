// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool XUPY;
extern bool WEFE;
extern bool BUZA;

extern bool OAM_A_A7;
extern bool OAM_A_A6;
extern bool OAM_A_A5;
extern bool OAM_A_A4;
extern bool OAM_A_A3;
extern bool OAM_A_A2;

extern bool FEPO;
extern bool DABY;
extern bool DABU;
extern bool DEGE;
extern bool GYSA;

extern bool DENY;
extern bool AKOL;
extern bool BYMY;

extern bool XYME;
extern bool WUSE;
extern bool ZURU;

extern bool GOWO;
extern bool WYLU;
extern bool EWOT;

extern bool GUGY;
extern bool FEFO;
extern bool WABE;

extern bool DYDO;
extern bool BYVY;
extern bool AHOF;

extern bool GYGY;
extern bool DYMO;
extern bool BUCY;

extern bool GYMA;
extern bool FEKA;
extern bool XYHA;

extern bool FAME;
extern bool BUZY;
extern bool FUKE;

extern bool FADO;
extern bool WUFA;
extern bool FAKA;

extern bool FURO;
extern bool GENY;
extern bool ENOB;

//----------
// outputs

bool WEZA;
bool WUCO;
bool WYDA;
bool ZYSU;
bool WYSE;
bool WUZY;

bool CUCA;
bool CEGA;
bool CUCU;
bool WENU;

//----------
// registers

reg XECU;
reg YDUF;
reg XOBE;
reg ZUZE;
reg XEDY;
reg XADU;

//-----------------------------------------------------------------------------

void tick_spritestore() {
  bool CYKE = not(XUPY);
  bool WUDA = not(CYKE);

  bool XECU_Q = XECU.tock(WUDA, WEFE, OAM_A_A7);
  bool YDUF_Q = YDUF.tock(WUDA, WEFE, OAM_A_A6);
  bool XOBE_Q = XOBE.tock(WUDA, WEFE, OAM_A_A5);
  bool ZUZE_Q = ZUZE.tock(WUDA, WEFE, OAM_A_A4);
  bool XEDY_Q = XEDY.tock(WUDA, WEFE, OAM_A_A3);
  bool XADU_Q = XADU.tock(WUDA, WEFE, OAM_A_A2);

  // ok, the schematic has the net the same name as the cell, which is annoying.
  bool WEZA2 = not(!XECU_Q);
  bool WUCO2 = not(!YDUF_Q);
  bool WYDA2 = not(!XOBE_Q);
  bool ZYSU2 = not(!ZUZE_Q);
  bool WYSE2 = not(!XEDY_Q);
  bool WUZY2 = not(!XADU_Q);

  bool CUCA2 = not(DABY);
  bool CEGA2 = not(DABU);
  bool CUCU2 = not(DEGE);
  bool WENU2 = not(GYSA);

  if (BUZA) {
    WEZA = WEZA2;
    WUCO = WUCO2;
    WYDA = WYDA2;
    ZYSU = ZYSU2;
    WYSE = WYSE2;
    WUZY = WUZY2;
  }

  if (FEPO) {
    CUCA = CUCA2;
    CEGA = CEGA2;
    CUCU = CUCU2;
    WENU = WENU2;
  }

  // 1 of 10

  reg AXUV;
  bool AXUV_Q = AXUV.q();
  bool ADYB = not(!AXUV_Q);
  AXUV.tock(AKOL, 0, ADYB);
}