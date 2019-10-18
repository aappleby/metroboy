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
reg FUZO, GESY, FYSU, FEFA, GYNO, GULE, XYGO, XYNA, XAKU, YGUM;
reg CAJY, CUZA, COMA, CUFA, CEBO, CADU, ABUG, AMES, ABOP, AROF;
reg EKAP, ETAV, EBEX, GORU, ETYM, EKOP, ANED, ACEP, ABUX, ABEG;
reg DAFU, DEBA, DUHA, DUNY, DESE, DEVY, ZURY, ZURO, ZENE, ZYLU;
reg BOXA, BUNA, BULU, BECA, BYHU, BUHE, YKUK, YLOV, XAZY, XOSY;
reg XUFO, XUTE, XOTU, XYFE, YZOR, YBER, DEWU, CANA, DYSY, FOFO;
reg YGUS, YSOK, YZEP, WYTE, ZONY, YWAK, FYHY, GYHO, BOZU, CUFO;

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

  // Sprite store 4 of 10
  bool FUZO_Q = FUZO.q(); bool EVYT = not(!FUZO_Q); FUZO.tock(FEFO, 0, WEZA);
  bool GESY_Q = GESY.q(); bool WABA = not(!GESY_Q); GESY.tock(FEFO, 0, WUCO);
  bool FYSU_Q = FYSU.q(); bool ETAD = not(!FYSU_Q); FYSU.tock(FEFO, 0, WYDA);
  bool FEFA_Q = FEFA.q(); bool ELEP = not(!FEFA_Q); FEFA.tock(FEFO, 0, ZYSU);
  bool GYNO_Q = GYNO.q(); bool WYGO = not(!GYNO_Q); GYNO.tock(FEFO, 0, WYSE);
  bool GULE_Q = GULE.q(); bool WAKO = not(!GULE_Q); GULE.tock(FEFO, 0, WUZY);
  bool XYGO_Q = XYGO.q(); bool WANA = not(!XYGO_Q); XYGO.tock(WABE, 0, CUCA);
  bool XYNA_Q = XYNA.q(); bool WAXE = not(!XYNA_Q); XYNA.tock(WABE, 0, CEGA);
  bool XAKU_Q = XAKU.q(); bool WABU = not(!XAKU_Q); XAKU.tock(WABE, 0, CUCU);
  bool YGUM_Q = YGUM.q(); bool YPOZ = not(!YGUM_Q); YGUM.tock(WABE, 0, WENU);

  if (GUGY) {
    WEZA = EVYT;
    WUCO = WABA;
    WYDA = ETAD;
    ZYSU = ELEP;
    WYSE = WYGO;
    WUZY = WAKO;
    CUCA = WANA;
    CEGA = WAXE;
    CUCU = WABU;
    WENU = YPOZ;
  }

  // Sprite store 5 of 10
  bool CAJY_Q = CAJY.q(); bool BEMO = not(!CAJY_Q); CAJY.tock(BYVY, 0, WEZA);
  bool CUZA_Q = CUZA.q(); bool CYBY = not(!CUZA_Q); CUZA.tock(BYVY, 0, WUCO);
  bool COMA_Q = COMA.q(); bool BETY = not(!COMA_Q); COMA.tock(BYVY, 0, WYDA);
  bool CUFA_Q = CUFA.q(); bool CEGY = not(!CUFA_Q); CUFA.tock(BYVY, 0, ZYSU);
  bool CEBO_Q = CEBO.q(); bool CELU = not(!CEBO_Q); CEBO.tock(BYVY, 0, WYSE);
  bool CADU_Q = CADU.q(); bool CUBO = not(!CADU_Q); CADU.tock(BYVY, 0, WUZY);
  bool ABUG_Q = ABUG.q(); bool BEFE = not(!ABUG_Q); ABUG.tock(AHOF, 0, CUCA);
  bool AMES_Q = AMES.q(); bool BYRO = not(!AMES_Q); AMES.tock(AHOF, 0, CEGA);
  bool ABOP_Q = ABOP.q(); bool BACO = not(!ABOP_Q); ABOP.tock(AHOF, 0, CUCU);
  bool AROF_Q = AROF.q(); bool AHUM = not(!AROF_Q); AROF.tock(AHOF, 0, WENU);

  if (DYDO) {
    WEZA = BEMO;
    WUCO = CYBY;
    WYDA = BETY;
    ZYSU = CEGY;
    WYSE = CELU;
    WUZY = CUBO;
    CUCA = BEFE;
    CEGA = BYRO;
    CUCU = BACO;
    WENU = AHUM;
  }

  // Sprite store 6 of 10
  bool EKAP_Q = EKAP.q(); bool DALO = not(!EKAP_Q); EKAP.tock(DYMO, 0, WEZA);
  bool ETAV_Q = ETAV.q(); bool DALY = not(!ETAV_Q); ETAV.tock(DYMO, 0, WUCO);
  bool EBEX_Q = EBEX.q(); bool DUZA = not(!EBEX_Q); EBEX.tock(DYMO, 0, WYDA);
  bool GORU_Q = GORU.q(); bool WAGA = not(!GORU_Q); GORU.tock(DYMO, 0, ZYSU);
  bool ETYM_Q = ETYM.q(); bool DYNY = not(!ETYM_Q); ETYM.tock(DYMO, 0, WYSE);
  bool EKOP_Q = EKOP.q(); bool DOBO = not(!EKOP_Q); EKOP.tock(DYMO, 0, WUZY);
  bool ANED_Q = ANED.q(); bool AWAT = not(!ANED_Q); ANED.tock(BUCY, 0, CUCA);
  bool ACEP_Q = ACEP.q(); bool BACE = not(!ACEP_Q); ACEP.tock(BUCY, 0, CEGA);
  bool ABUX_Q = ABUX.q(); bool BODU = not(!ABUX_Q); ABUX.tock(BUCY, 0, CUCU);
  bool ABEG_Q = ABEG.q(); bool BUJA = not(!ABEG_Q); ABEG.tock(BUCY, 0, WENU);

  if (GYGY) {
    WEZA = DALO;
    WUCO = DALY;
    WYDA = DUZA;
    ZYSU = WAGA;
    WYSE = DYNY;
    WUZY = DOBO;
    CUCA = AWAT;
    CEGA = BACE;
    CUCU = BODU;
    WENU = BUJA;
  }

  // Sprite store 7 of 10
  bool DAFU_Q = DAFU.q(); bool DEZU = not(!DAFU_Q); DAFU.tock(FEKA, 0, WEZA);
  bool DEBA_Q = DEBA.q(); bool EFUD = not(!DEBA_Q); DEBA.tock(FEKA, 0, WUCO);
  bool DUHA_Q = DUHA.q(); bool DONY = not(!DUHA_Q); DUHA.tock(FEKA, 0, WYDA);
  bool DUNY_Q = DUNY.q(); bool DOWA = not(!DUNY_Q); DUNY.tock(FEKA, 0, ZYSU);
  bool DESE_Q = DESE.q(); bool DYGO = not(!DESE_Q); DESE.tock(FEKA, 0, WYSE);
  bool DEVY_Q = DEVY.q(); bool ENAP = not(!DEVY_Q); DEVY.tock(FEKA, 0, WUZY);
  bool ZURY_Q = ZURY.q(); bool ZYPO = not(!ZURY_Q); ZURY.tock(XYHA, 0, CUCA);
  bool ZURO_Q = ZURO.q(); bool ZEXE = not(!ZURO_Q); ZURO.tock(XYHA, 0, CEGA);
  bool ZENE_Q = ZENE.q(); bool YJEM = not(!ZENE_Q); ZENE.tock(XYHA, 0, CUCU);
  bool ZYLU_Q = ZYLU.q(); bool YWAV = not(!ZYLU_Q); ZYLU.tock(XYHA, 0, WENU);

  if (GYMA) {
    WEZA = DEZU;
    WUCO = EFUD;
    WYDA = DONY;
    ZYSU = DOWA;
    WYSE = DYGO;
    WUZY = ENAP;
    CUCA = ZYPO;
    CEGA = ZEXE;
    CUCU = YJEM;
    WENU = YWAV;
  }

  // Sprite store 8 of 10
  bool BOXA_Q = BOXA.q(); bool AXEC = not(!BOXA_Q); BOXA.tock(BUZY, 0, WEZA);
  bool BUNA_Q = BUNA.q(); bool CYRO = not(!BUNA_Q); BUNA.tock(BUZY, 0, WUCO);
  bool BULU_Q = BULU.q(); bool CUVU = not(!BULU_Q); BULU.tock(BUZY, 0, WYDA);
  bool BECA_Q = BECA.q(); bool APON = not(!BECA_Q); BECA.tock(BUZY, 0, ZYSU);
  bool BYHU_Q = BYHU.q(); bool AFOZ = not(!BYHU_Q); BYHU.tock(BUZY, 0, WYSE);
  bool BUHE_Q = BUHE.q(); bool CUBE = not(!BUHE_Q); BUHE.tock(BUZY, 0, WUZY);
  bool YKUK_Q = YKUK.q(); bool ZABY = not(!YKUK_Q); YKUK.tock(FUKE, 0, CUCA);
  bool YLOV_Q = YLOV.q(); bool ZUKE = not(!YLOV_Q); YLOV.tock(FUKE, 0, CEGA);
  bool XAZY_Q = XAZY.q(); bool WUXE = not(!XAZY_Q); XAZY.tock(FUKE, 0, CUCU);
  bool XOSY_Q = XOSY.q(); bool WERE = not(!XOSY_Q); XOSY.tock(FUKE, 0, WENU);

  if (FAME) {
    WEZA = AXEC;
    WUCO = CYRO;
    WYDA = CUVU;
    ZYSU = APON;
    WYSE = AFOZ;
    WUZY = CUBE;
    CUCA = ZABY;
    CEGA = ZUKE;
    CUCU = WUXE;
    WENU = WERE;
  }

  // Sprite store 9 of 10
  bool XUFO_Q = XUFO.q(); bool YHAL = not(!XUFO_Q); XUFO.tock(WUFA, 0, WEZA);
  bool XUTE_Q = XUTE.q(); bool YRAD = not(!XUTE_Q); XUTE.tock(WUFA, 0, WUCO);
  bool XOTU_Q = XOTU.q(); bool XYRA = not(!XOTU_Q); XOTU.tock(WUFA, 0, WYDA);
  bool XYFE_Q = XYFE.q(); bool YNEV = not(!XYFE_Q); XYFE.tock(WUFA, 0, ZYSU);
  bool YZOR_Q = YZOR.q(); bool ZOJY = not(!YZOR_Q); YZOR.tock(WUFA, 0, WYSE);
  bool YBER_Q = YBER.q(); bool ZARO = not(!YBER_Q); YBER.tock(WUFA, 0, WUZY);
  bool DEWU_Q = DEWU.q(); bool CAWO = not(!DEWU_Q); DEWU.tock(FAKA, 0, CUCA);
  bool CANA_Q = CANA.q(); bool BYME = not(!CANA_Q); CANA.tock(FAKA, 0, CEGA);
  bool DYSY_Q = DYSY.q(); bool COHO = not(!DYSY_Q); DYSY.tock(FAKA, 0, CUCU);
  bool FOFO_Q = FOFO.q(); bool GATE = not(!FOFO_Q); FOFO.tock(FAKA, 0, WENU);

  if (FADO) {
    WEZA = YHAL;
    WUCO = YRAD;
    WYDA = XYRA;
    ZYSU = YNEV;
    WYSE = ZOJY;
    WUZY = ZARO;
    CUCA = CAWO;
    CEGA = BYME;
    CUCU = COHO;
    WENU = GATE;
  }

  // Sprite store 10 of 10
  bool YGUS_Q = YGUS.q(); bool ZETU = not(!YGUS_Q); YGUS.tock(GENY, 0, WEZA);
  bool YSOK_Q = YSOK.q(); bool ZECE = not(!YSOK_Q); YSOK.tock(GENY, 0, WUCO);
  bool YZEP_Q = YZEP.q(); bool ZAVE = not(!YZEP_Q); YZEP.tock(GENY, 0, WYDA);
  bool WYTE_Q = WYTE.q(); bool WOKO = not(!WYTE_Q); WYTE.tock(GENY, 0, ZYSU);
  bool ZONY_Q = ZONY.q(); bool ZUMU = not(!ZONY_Q); ZONY.tock(GENY, 0, WYSE);
  bool YWAK_Q = YWAK.q(); bool ZEDY = not(!YWAK_Q); YWAK.tock(GENY, 0, WUZY);
  bool FYHY_Q = FYHY.q(); bool GOFO = not(!FYHY_Q); FYHY.tock(ENOB, 0, CUCA);
  bool GYHO_Q = GYHO.q(); bool WEHE = not(!GYHO_Q); GYHO.tock(ENOB, 0, CEGA);
  bool BOZU_Q = BOZU.q(); bool AJAL = not(!BOZU_Q); BOZU.tock(ENOB, 0, CUCU);
  bool CUFO_Q = CUFO.q(); bool BUKY = not(!CUFO_Q); CUFO.tock(ENOB, 0, WENU);

  if (FURO) {
    WEZA = ZETU;
    WUCO = ZECE;
    WYDA = ZAVE;
    ZYSU = WOKO;
    WYSE = ZUMU;
    WUZY = ZEDY;
    CUCA = GOFO;
    CEGA = WEHE;
    CUCU = AJAL;
    WENU = BUKY;
  }
}