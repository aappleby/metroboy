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

// Sprite store
reg AXUV, BADA, APEV, BADO, BEXY, BYHE, AFYM, AZAP, AFUT, AFYX;
reg GECU, FOXY, GOHU, FOGO, GACY, GABO, ZUBE, ZUMY, ZEXO, ZAFU;
reg XYNU, XEGE, XABO, WANU, XEFE, XAVE, CUMU, CAPO, CONO, CAJU;

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

  // Sprite store 1 of 10
  bool AXUV_Q = AXUV.q(); bool ADYB = not(!AXUV_Q); AXUV.tock(AKOL, 0, WEZA);
  bool BADA_Q = BADA.q(); bool APOB = not(!BADA_Q); BADA.tock(AKOL, 0, WUCO);
  bool APEV_Q = APEV.q(); bool APYV = not(!APEV_Q); APEV.tock(AKOL, 0, WYDA);
  bool BADO_Q = BADO.q(); bool AFEN = not(!BADO_Q); BADO.tock(AKOL, 0, ZYSU);
  bool BEXY_Q = BEXY.q(); bool AKYH = not(!BEXY_Q); BEXY.tock(AKOL, 0, WYSE);
  bool BYHE_Q = BYHE.q(); bool APOC = not(!BYHE_Q); BYHE.tock(AKOL, 0, WUZY);
  bool AFYM_Q = AFYM.q(); bool BUJY = not(!AFYM_Q); AFYM.tock(BYMY, 0, CUCA);
  bool AZAP_Q = AZAP.q(); bool BOSO = not(!AZAP_Q); AZAP.tock(BYMY, 0, CEGA);
  bool AFUT_Q = AFUT.q(); bool AHAC = not(!AFUT_Q); AFUT.tock(BYMY, 0, CUCU);
  bool AFYX_Q = AFYX.q(); bool BAZU = not(!AFYX_Q); AFYX.tock(BYMY, 0, WENU);

  if (DENY) {
    WEZA = ADYB; WUCO = APOB; WYDA = APYV; ZYSU = AFEN; WYSE = AKYH; WUZY = APOC;
    CUCA = BUJY; CEGA = BOSO; CUCU = AHAC; WENU = BAZU;
  }

  // Sprite store 2 of 10
  bool GECU_Q = GECU.q(); bool WOCY = not(!GECU_Q); GECU.tock(WUSE, 0, WEZA);
  bool FOXY_Q = FOXY.q(); bool ELYC = not(!FOXY_Q); FOXY.tock(WUSE, 0, WUCO);
  bool GOHU_Q = GOHU.q(); bool WABO = not(!GOHU_Q); GOHU.tock(WUSE, 0, WYDA);
  bool FOGO_Q = FOGO.q(); bool EZOC = not(!FOGO_Q); FOGO.tock(WUSE, 0, ZYSU);
  bool GACY_Q = GACY.q(); bool WYWY = not(!GACY_Q); GACY.tock(WUSE, 0, WYSE);
  bool GABO_Q = GABO.q(); bool WATO = not(!GABO_Q); GABO.tock(WUSE, 0, WUZY);
  bool ZUBE_Q = ZUBE.q(); bool ZUDO = not(!ZUBE_Q); ZUBE.tock(ZURU, 0, CUCA);
  bool ZUMY_Q = ZUMY.q(); bool YBUK = not(!ZUMY_Q); ZUMY.tock(ZURU, 0, CEGA);
  bool ZEXO_Q = ZEXO.q(); bool ZYTO = not(!ZEXO_Q); ZEXO.tock(ZURU, 0, CUCU);
  bool ZAFU_Q = ZAFU.q(); bool YKOZ = not(!ZAFU_Q); ZAFU.tock(ZURU, 0, WENU);

  if (XYME) {
    WEZA = WOCY;
    WUCO = ELYC;
    WYDA = WABO;
    ZYSU = EZOC;
    WYSE = WYWY;
    WUZY = WATO;
    CUCA = ZUDO;
    CEGA = YBUK;
    CUCU = ZYTO;
    WENU = YKOZ;
  }

  // Sprite store 3 of 10
  bool XYNU_Q = XYNU.q(); bool WAJA = not(!XYNU_Q); XYNU.tock(WYLU, 0, WEZA);
  bool XEGE_Q = XEGE.q(); bool WOXY = not(!XEGE_Q); XEGE.tock(WYLU, 0, WUCO);
  bool XABO_Q = XABO.q(); bool XYRE = not(!XABO_Q); XABO.tock(WYLU, 0, WYDA);
  bool WANU_Q = WANU.q(); bool WERU = not(!WANU_Q); WANU.tock(WYLU, 0, ZYSU);
  bool XEFE_Q = XEFE.q(); bool WEPY = not(!XEFE_Q); XEFE.tock(WYLU, 0, WYSE);
  bool XAVE_Q = XAVE.q(); bool WUXU = not(!XAVE_Q); XAVE.tock(WYLU, 0, WUZY);
  bool CUMU_Q = CUMU.q(); bool BYDO = not(!CUMU_Q); CUMU.tock(EWOT, 0, CUCA);
  bool CAPO_Q = CAPO.q(); bool BUCE = not(!CAPO_Q); CAPO.tock(EWOT, 0, CEGA);
  bool CONO_Q = CONO.q(); bool BOVE = not(!CONO_Q); CONO.tock(EWOT, 0, CUCU);
  bool CAJU_Q = CAJU.q(); bool BEVY = not(!CAJU_Q); CAJU.tock(EWOT, 0, WENU);

  if (GOWO) {
    WEZA = WAJA;
    WUCO = WOXY;
    WYDA = XYRE;
    ZYSU = WERU;
    WYSE = WEPY;
    WUZY = WUXU;
    CUCA = BYDO;
    CEGA = BUCE;
    CUCU = BOVE;
    WENU = BEVY;
  }

  /*
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(ZZZZ, 0, WEZA);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(ZZZZ, 0, WUCO);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(ZZZZ, 0, WYDA);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(ZZZZ, 0, ZYSU);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(ZZZZ, 0, WYSE);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(ZZZZ, 0, WUZY);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(YYYY, 0, CUCA);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(YYYY, 0, CEGA);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(YYYY, 0, CUCU);
  bool ZZZZ_Q = ZZZZ.q(); bool ZZZZ = not(!ZZZZ_Q); ZZZZ.tock(YYYY, 0, WENU);

  if (DENY) {
    WEZA = ZZZZ;
    WUCO = ZZZZ;
    WYDA = ZZZZ;
    ZYSU = ZZZZ;
    WYSE = ZZZZ;
    WUZY = ZZZZ;
    CUCA = ZZZZ;
    CEGA = ZZZZ;
    CUCU = ZZZZ;
    WENU = ZZZZ;
  }
  */
}