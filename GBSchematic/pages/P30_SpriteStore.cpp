#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P30_SpriteStore {
  struct Input {
    bool XUPY;
    bool WEFE;
    bool FEPO;
    bool BUZA;

    // which sprite line to render
    bool DABY,DABU,DEGE,GYSA;

    // sprite store read selection lines
    bool DENY,XYME,GOWO,GUGY,DYDO,GYGY,GYMA,FAME,FADO,FURO;

    // sprite write clock lines
    bool AKOL,WUSE,WYLU,FEFO,BYVY,DYMO,FEKA,BUZY,WUFA,GENY;
    bool BYMY,ZURU,EWOT,WABE,AHOF,BUCY,XYHA,FUKE,FAKA,ENOB;
      
    bool OAM_A_A0,OAM_A_A1,OAM_A_A2,OAM_A_A3,OAM_A_A4,OAM_A_A5,OAM_A_A6,OAM_A_A7;
  };

  struct Output {
    // Sprite index
    bool WEZA,WUCO,WYDA,ZYSU,WYSE,WUZY;

    // Sprite Y line
    bool CUCA,CEGA,CUCU,WENU;
  };

  // Sprite index register
  reg XADU,XEDY,ZUZE,XOBE,YDUF,XECU;

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

  void tick(const Input& in, Output& out) {
    bool CYKE = not(in.XUPY);
    bool WUDA = not(CYKE);

    bool XECU_Q = XECU.tock(WUDA, in.WEFE, in.OAM_A_A7);
    bool YDUF_Q = YDUF.tock(WUDA, in.WEFE, in.OAM_A_A6);
    bool XOBE_Q = XOBE.tock(WUDA, in.WEFE, in.OAM_A_A5);
    bool ZUZE_Q = ZUZE.tock(WUDA, in.WEFE, in.OAM_A_A4);
    bool XEDY_Q = XEDY.tock(WUDA, in.WEFE, in.OAM_A_A3);
    bool XADU_Q = XADU.tock(WUDA, in.WEFE, in.OAM_A_A2);

    // ok, the schematic has the net the same name as the cell, which is annoying.
    bool WEZA = not(!XECU_Q);
    bool WUCO = not(!YDUF_Q);
    bool WYDA = not(!XOBE_Q);
    bool ZYSU = not(!ZUZE_Q);
    bool WYSE = not(!XEDY_Q);
    bool WUZY = not(!XADU_Q);

    bool CUCA = not(in.DABY);
    bool CEGA = not(in.DABU);
    bool CUCU = not(in.DEGE);
    bool WENU = not(in.GYSA);

    // so this is a bit weird as we have a tri-state bus inside one schematic...
    bool WEZA2,WUCO2,WYDA2,ZYSU2,WYSE2,WUZY2,CUCA2,CEGA2,CUCU2,WENU2;

    if (in.BUZA) {
      WEZA2 = WEZA;
      WUCO2 = WUCO;
      WYDA2 = WYDA;
      ZYSU2 = ZYSU;
      WYSE2 = WYSE;
      WUZY2 = WUZY;
    }

    if (in.FEPO) {
      CUCA2 = CUCA;
      CEGA2 = CEGA;
      CUCU2 = CUCU;
      WENU2 = WENU;
    }

    // Sprite store 1 of 10
    {
      bool AXUV_Q = AXUV.q(); bool ADYB = not(!AXUV_Q); if (in.DENY) WEZA2 = ADYB; AXUV.tock(in.AKOL, 0, WEZA2);
      bool BADA_Q = BADA.q(); bool APOB = not(!BADA_Q); if (in.DENY) WUCO2 = APOB; BADA.tock(in.AKOL, 0, WUCO2);
      bool APEV_Q = APEV.q(); bool APYV = not(!APEV_Q); if (in.DENY) WYDA2 = APYV; APEV.tock(in.AKOL, 0, WYDA2);
      bool BADO_Q = BADO.q(); bool AFEN = not(!BADO_Q); if (in.DENY) ZYSU2 = AFEN; BADO.tock(in.AKOL, 0, ZYSU2);
      bool BEXY_Q = BEXY.q(); bool AKYH = not(!BEXY_Q); if (in.DENY) WYSE2 = AKYH; BEXY.tock(in.AKOL, 0, WYSE2);
      bool BYHE_Q = BYHE.q(); bool APOC = not(!BYHE_Q); if (in.DENY) WUZY2 = APOC; BYHE.tock(in.AKOL, 0, WUZY2);
      bool AFYM_Q = AFYM.q(); bool BUJY = not(!AFYM_Q); if (in.DENY) CUCA2 = BUJY; AFYM.tock(in.BYMY, 0, CUCA2);
      bool AZAP_Q = AZAP.q(); bool BOSO = not(!AZAP_Q); if (in.DENY) CEGA2 = BOSO; AZAP.tock(in.BYMY, 0, CEGA2);
      bool AFUT_Q = AFUT.q(); bool AHAC = not(!AFUT_Q); if (in.DENY) CUCU2 = AHAC; AFUT.tock(in.BYMY, 0, CUCU2);
      bool AFYX_Q = AFYX.q(); bool BAZU = not(!AFYX_Q); if (in.DENY) WENU2 = BAZU; AFYX.tock(in.BYMY, 0, WENU2);
    }

    // Sprite store 2 of 10
    {
      bool GECU_Q = GECU.q(); bool WOCY = not(!GECU_Q); if (in.XYME) WEZA2 = WOCY; GECU.tock(in.WUSE, 0, WEZA2);
      bool FOXY_Q = FOXY.q(); bool ELYC = not(!FOXY_Q); if (in.XYME) WUCO2 = ELYC; FOXY.tock(in.WUSE, 0, WUCO2);
      bool GOHU_Q = GOHU.q(); bool WABO = not(!GOHU_Q); if (in.XYME) WYDA2 = WABO; GOHU.tock(in.WUSE, 0, WYDA2);
      bool FOGO_Q = FOGO.q(); bool EZOC = not(!FOGO_Q); if (in.XYME) ZYSU2 = EZOC; FOGO.tock(in.WUSE, 0, ZYSU2);
      bool GACY_Q = GACY.q(); bool WYWY = not(!GACY_Q); if (in.XYME) WYSE2 = WYWY; GACY.tock(in.WUSE, 0, WYSE2);
      bool GABO_Q = GABO.q(); bool WATO = not(!GABO_Q); if (in.XYME) WUZY2 = WATO; GABO.tock(in.WUSE, 0, WUZY2);
      bool ZUBE_Q = ZUBE.q(); bool ZUDO = not(!ZUBE_Q); if (in.XYME) CUCA2 = ZUDO; ZUBE.tock(in.ZURU, 0, CUCA2);
      bool ZUMY_Q = ZUMY.q(); bool YBUK = not(!ZUMY_Q); if (in.XYME) CEGA2 = YBUK; ZUMY.tock(in.ZURU, 0, CEGA2);
      bool ZEXO_Q = ZEXO.q(); bool ZYTO = not(!ZEXO_Q); if (in.XYME) CUCU2 = ZYTO; ZEXO.tock(in.ZURU, 0, CUCU2);
      bool ZAFU_Q = ZAFU.q(); bool YKOZ = not(!ZAFU_Q); if (in.XYME) WENU2 = YKOZ; ZAFU.tock(in.ZURU, 0, WENU2);
    }

    // Sprite store 3 of 10
    {
      bool XYNU_Q = XYNU.q(); bool WAJA = not(!XYNU_Q); if (in.GOWO) WEZA2 = WAJA; XYNU.tock(in.WYLU, 0, WEZA2);
      bool XEGE_Q = XEGE.q(); bool WOXY = not(!XEGE_Q); if (in.GOWO) WUCO2 = WOXY; XEGE.tock(in.WYLU, 0, WUCO2);
      bool XABO_Q = XABO.q(); bool XYRE = not(!XABO_Q); if (in.GOWO) WYDA2 = XYRE; XABO.tock(in.WYLU, 0, WYDA2);
      bool WANU_Q = WANU.q(); bool WERU = not(!WANU_Q); if (in.GOWO) ZYSU2 = WERU; WANU.tock(in.WYLU, 0, ZYSU2);
      bool XEFE_Q = XEFE.q(); bool WEPY = not(!XEFE_Q); if (in.GOWO) WYSE2 = WEPY; XEFE.tock(in.WYLU, 0, WYSE2);
      bool XAVE_Q = XAVE.q(); bool WUXU = not(!XAVE_Q); if (in.GOWO) WUZY2 = WUXU; XAVE.tock(in.WYLU, 0, WUZY2);
      bool CUMU_Q = CUMU.q(); bool BYDO = not(!CUMU_Q); if (in.GOWO) CUCA2 = BYDO; CUMU.tock(in.EWOT, 0, CUCA2);
      bool CAPO_Q = CAPO.q(); bool BUCE = not(!CAPO_Q); if (in.GOWO) CEGA2 = BUCE; CAPO.tock(in.EWOT, 0, CEGA2);
      bool CONO_Q = CONO.q(); bool BOVE = not(!CONO_Q); if (in.GOWO) CUCU2 = BOVE; CONO.tock(in.EWOT, 0, CUCU2);
      bool CAJU_Q = CAJU.q(); bool BEVY = not(!CAJU_Q); if (in.GOWO) WENU2 = BEVY; CAJU.tock(in.EWOT, 0, WENU2);
    }

    // Sprite store 4 of 10
    {
      bool FUZO_Q = FUZO.q(); bool EVYT = not(!FUZO_Q); if (in.GUGY) WEZA2 = EVYT; FUZO.tock(in.FEFO, 0, WEZA2);
      bool GESY_Q = GESY.q(); bool WABA = not(!GESY_Q); if (in.GUGY) WUCO2 = WABA; GESY.tock(in.FEFO, 0, WUCO2);
      bool FYSU_Q = FYSU.q(); bool ETAD = not(!FYSU_Q); if (in.GUGY) WYDA2 = ETAD; FYSU.tock(in.FEFO, 0, WYDA2);
      bool FEFA_Q = FEFA.q(); bool ELEP = not(!FEFA_Q); if (in.GUGY) ZYSU2 = ELEP; FEFA.tock(in.FEFO, 0, ZYSU2);
      bool GYNO_Q = GYNO.q(); bool WYGO = not(!GYNO_Q); if (in.GUGY) WYSE2 = WYGO; GYNO.tock(in.FEFO, 0, WYSE2);
      bool GULE_Q = GULE.q(); bool WAKO = not(!GULE_Q); if (in.GUGY) WUZY2 = WAKO; GULE.tock(in.FEFO, 0, WUZY2);
      bool XYGO_Q = XYGO.q(); bool WANA = not(!XYGO_Q); if (in.GUGY) CUCA2 = WANA; XYGO.tock(in.WABE, 0, CUCA2);
      bool XYNA_Q = XYNA.q(); bool WAXE = not(!XYNA_Q); if (in.GUGY) CEGA2 = WAXE; XYNA.tock(in.WABE, 0, CEGA2);
      bool XAKU_Q = XAKU.q(); bool WABU = not(!XAKU_Q); if (in.GUGY) CUCU2 = WABU; XAKU.tock(in.WABE, 0, CUCU2);
      bool YGUM_Q = YGUM.q(); bool YPOZ = not(!YGUM_Q); if (in.GUGY) WENU2 = YPOZ; YGUM.tock(in.WABE, 0, WENU2);
    }

    // Sprite store 5 of 10
    {
      bool CAJY_Q = CAJY.q(); bool BEMO = not(!CAJY_Q); if (in.DYDO) WEZA2 = BEMO; CAJY.tock(in.BYVY, 0, WEZA2);
      bool CUZA_Q = CUZA.q(); bool CYBY = not(!CUZA_Q); if (in.DYDO) WUCO2 = CYBY; CUZA.tock(in.BYVY, 0, WUCO2);
      bool COMA_Q = COMA.q(); bool BETY = not(!COMA_Q); if (in.DYDO) WYDA2 = BETY; COMA.tock(in.BYVY, 0, WYDA2);
      bool CUFA_Q = CUFA.q(); bool CEGY = not(!CUFA_Q); if (in.DYDO) ZYSU2 = CEGY; CUFA.tock(in.BYVY, 0, ZYSU2);
      bool CEBO_Q = CEBO.q(); bool CELU = not(!CEBO_Q); if (in.DYDO) WYSE2 = CELU; CEBO.tock(in.BYVY, 0, WYSE2);
      bool CADU_Q = CADU.q(); bool CUBO = not(!CADU_Q); if (in.DYDO) WUZY2 = CUBO; CADU.tock(in.BYVY, 0, WUZY2);
      bool ABUG_Q = ABUG.q(); bool BEFE = not(!ABUG_Q); if (in.DYDO) CUCA2 = BEFE; ABUG.tock(in.AHOF, 0, CUCA2);
      bool AMES_Q = AMES.q(); bool BYRO = not(!AMES_Q); if (in.DYDO) CEGA2 = BYRO; AMES.tock(in.AHOF, 0, CEGA2);
      bool ABOP_Q = ABOP.q(); bool BACO = not(!ABOP_Q); if (in.DYDO) CUCU2 = BACO; ABOP.tock(in.AHOF, 0, CUCU2);
      bool AROF_Q = AROF.q(); bool AHUM = not(!AROF_Q); if (in.DYDO) WENU2 = AHUM; AROF.tock(in.AHOF, 0, WENU2);
    }

    // Sprite store 6 of 10
    {
      bool EKAP_Q = EKAP.q(); bool DALO = not(!EKAP_Q); if (in.GYGY) WEZA2 = DALO; EKAP.tock(in.DYMO, 0, WEZA2);
      bool ETAV_Q = ETAV.q(); bool DALY = not(!ETAV_Q); if (in.GYGY) WUCO2 = DALY; ETAV.tock(in.DYMO, 0, WUCO2);
      bool EBEX_Q = EBEX.q(); bool DUZA = not(!EBEX_Q); if (in.GYGY) WYDA2 = DUZA; EBEX.tock(in.DYMO, 0, WYDA2);
      bool GORU_Q = GORU.q(); bool WAGA = not(!GORU_Q); if (in.GYGY) ZYSU2 = WAGA; GORU.tock(in.DYMO, 0, ZYSU2);
      bool ETYM_Q = ETYM.q(); bool DYNY = not(!ETYM_Q); if (in.GYGY) WYSE2 = DYNY; ETYM.tock(in.DYMO, 0, WYSE2);
      bool EKOP_Q = EKOP.q(); bool DOBO = not(!EKOP_Q); if (in.GYGY) WUZY2 = DOBO; EKOP.tock(in.DYMO, 0, WUZY2);
      bool ANED_Q = ANED.q(); bool AWAT = not(!ANED_Q); if (in.GYGY) CUCA2 = AWAT; ANED.tock(in.BUCY, 0, CUCA2);
      bool ACEP_Q = ACEP.q(); bool BACE = not(!ACEP_Q); if (in.GYGY) CEGA2 = BACE; ACEP.tock(in.BUCY, 0, CEGA2);
      bool ABUX_Q = ABUX.q(); bool BODU = not(!ABUX_Q); if (in.GYGY) CUCU2 = BODU; ABUX.tock(in.BUCY, 0, CUCU2);
      bool ABEG_Q = ABEG.q(); bool BUJA = not(!ABEG_Q); if (in.GYGY) WENU2 = BUJA; ABEG.tock(in.BUCY, 0, WENU2);
    }

    // Sprite store 7 of 10
    {
      bool DAFU_Q = DAFU.q(); bool DEZU = not(!DAFU_Q); if (in.GYMA) WEZA2 = DEZU; DAFU.tock(in.FEKA, 0, WEZA2);
      bool DEBA_Q = DEBA.q(); bool EFUD = not(!DEBA_Q); if (in.GYMA) WUCO2 = EFUD; DEBA.tock(in.FEKA, 0, WUCO2);
      bool DUHA_Q = DUHA.q(); bool DONY = not(!DUHA_Q); if (in.GYMA) WYDA2 = DONY; DUHA.tock(in.FEKA, 0, WYDA2);
      bool DUNY_Q = DUNY.q(); bool DOWA = not(!DUNY_Q); if (in.GYMA) ZYSU2 = DOWA; DUNY.tock(in.FEKA, 0, ZYSU2);
      bool DESE_Q = DESE.q(); bool DYGO = not(!DESE_Q); if (in.GYMA) WYSE2 = DYGO; DESE.tock(in.FEKA, 0, WYSE2);
      bool DEVY_Q = DEVY.q(); bool ENAP = not(!DEVY_Q); if (in.GYMA) WUZY2 = ENAP; DEVY.tock(in.FEKA, 0, WUZY2);
      bool ZURY_Q = ZURY.q(); bool ZYPO = not(!ZURY_Q); if (in.GYMA) CUCA2 = ZYPO; ZURY.tock(in.XYHA, 0, CUCA2);
      bool ZURO_Q = ZURO.q(); bool ZEXE = not(!ZURO_Q); if (in.GYMA) CEGA2 = ZEXE; ZURO.tock(in.XYHA, 0, CEGA2);
      bool ZENE_Q = ZENE.q(); bool YJEM = not(!ZENE_Q); if (in.GYMA) CUCU2 = YJEM; ZENE.tock(in.XYHA, 0, CUCU2);
      bool ZYLU_Q = ZYLU.q(); bool YWAV = not(!ZYLU_Q); if (in.GYMA) WENU2 = YWAV; ZYLU.tock(in.XYHA, 0, WENU2);
    }

    // Sprite store 8 of 10
    {
      bool BOXA_Q = BOXA.q(); bool AXEC = not(!BOXA_Q); if (in.FAME) WEZA2 = AXEC; BOXA.tock(in.BUZY, 0, WEZA2);
      bool BUNA_Q = BUNA.q(); bool CYRO = not(!BUNA_Q); if (in.FAME) WUCO2 = CYRO; BUNA.tock(in.BUZY, 0, WUCO2);
      bool BULU_Q = BULU.q(); bool CUVU = not(!BULU_Q); if (in.FAME) WYDA2 = CUVU; BULU.tock(in.BUZY, 0, WYDA2);
      bool BECA_Q = BECA.q(); bool APON = not(!BECA_Q); if (in.FAME) ZYSU2 = APON; BECA.tock(in.BUZY, 0, ZYSU2);
      bool BYHU_Q = BYHU.q(); bool AFOZ = not(!BYHU_Q); if (in.FAME) WYSE2 = AFOZ; BYHU.tock(in.BUZY, 0, WYSE2);
      bool BUHE_Q = BUHE.q(); bool CUBE = not(!BUHE_Q); if (in.FAME) WUZY2 = CUBE; BUHE.tock(in.BUZY, 0, WUZY2);
      bool YKUK_Q = YKUK.q(); bool ZABY = not(!YKUK_Q); if (in.FAME) CUCA2 = ZABY; YKUK.tock(in.FUKE, 0, CUCA2);
      bool YLOV_Q = YLOV.q(); bool ZUKE = not(!YLOV_Q); if (in.FAME) CEGA2 = ZUKE; YLOV.tock(in.FUKE, 0, CEGA2);
      bool XAZY_Q = XAZY.q(); bool WUXE = not(!XAZY_Q); if (in.FAME) CUCU2 = WUXE; XAZY.tock(in.FUKE, 0, CUCU2);
      bool XOSY_Q = XOSY.q(); bool WERE = not(!XOSY_Q); if (in.FAME) WENU2 = WERE; XOSY.tock(in.FUKE, 0, WENU2);
    }

    // Sprite store 9 of 10
    {
      bool XUFO_Q = XUFO.q(); bool YHAL = not(!XUFO_Q); if (in.FADO) WEZA2 = YHAL; XUFO.tock(in.WUFA, 0, WEZA2);
      bool XUTE_Q = XUTE.q(); bool YRAD = not(!XUTE_Q); if (in.FADO) WUCO2 = YRAD; XUTE.tock(in.WUFA, 0, WUCO2);
      bool XOTU_Q = XOTU.q(); bool XYRA = not(!XOTU_Q); if (in.FADO) WYDA2 = XYRA; XOTU.tock(in.WUFA, 0, WYDA2);
      bool XYFE_Q = XYFE.q(); bool YNEV = not(!XYFE_Q); if (in.FADO) ZYSU2 = YNEV; XYFE.tock(in.WUFA, 0, ZYSU2);
      bool YZOR_Q = YZOR.q(); bool ZOJY = not(!YZOR_Q); if (in.FADO) WYSE2 = ZOJY; YZOR.tock(in.WUFA, 0, WYSE2);
      bool YBER_Q = YBER.q(); bool ZARO = not(!YBER_Q); if (in.FADO) WUZY2 = ZARO; YBER.tock(in.WUFA, 0, WUZY2);
      bool DEWU_Q = DEWU.q(); bool CAWO = not(!DEWU_Q); if (in.FADO) CUCA2 = CAWO; DEWU.tock(in.FAKA, 0, CUCA2);
      bool CANA_Q = CANA.q(); bool BYME = not(!CANA_Q); if (in.FADO) CEGA2 = BYME; CANA.tock(in.FAKA, 0, CEGA2);
      bool DYSY_Q = DYSY.q(); bool COHO = not(!DYSY_Q); if (in.FADO) CUCU2 = COHO; DYSY.tock(in.FAKA, 0, CUCU2);
      bool FOFO_Q = FOFO.q(); bool GATE = not(!FOFO_Q); if (in.FADO) WENU2 = GATE; FOFO.tock(in.FAKA, 0, WENU2);
    }

    // Sprite store 10 of 10
    {
      bool YGUS_Q = YGUS.q(); bool ZETU = not(!YGUS_Q); if (in.FURO) WEZA2 = ZETU; YGUS.tock(in.GENY, 0, WEZA2);
      bool YSOK_Q = YSOK.q(); bool ZECE = not(!YSOK_Q); if (in.FURO) WUCO2 = ZECE; YSOK.tock(in.GENY, 0, WUCO2);
      bool YZEP_Q = YZEP.q(); bool ZAVE = not(!YZEP_Q); if (in.FURO) WYDA2 = ZAVE; YZEP.tock(in.GENY, 0, WYDA2);
      bool WYTE_Q = WYTE.q(); bool WOKO = not(!WYTE_Q); if (in.FURO) ZYSU2 = WOKO; WYTE.tock(in.GENY, 0, ZYSU2);
      bool ZONY_Q = ZONY.q(); bool ZUMU = not(!ZONY_Q); if (in.FURO) WYSE2 = ZUMU; ZONY.tock(in.GENY, 0, WYSE2);
      bool YWAK_Q = YWAK.q(); bool ZEDY = not(!YWAK_Q); if (in.FURO) WUZY2 = ZEDY; YWAK.tock(in.GENY, 0, WUZY2);
      bool FYHY_Q = FYHY.q(); bool GOFO = not(!FYHY_Q); if (in.FURO) CUCA2 = GOFO; FYHY.tock(in.ENOB, 0, CUCA2);
      bool GYHO_Q = GYHO.q(); bool WEHE = not(!GYHO_Q); if (in.FURO) CEGA2 = WEHE; GYHO.tock(in.ENOB, 0, CEGA2);
      bool BOZU_Q = BOZU.q(); bool AJAL = not(!BOZU_Q); if (in.FURO) CUCU2 = AJAL; BOZU.tock(in.ENOB, 0, CUCU2);
      bool CUFO_Q = CUFO.q(); bool BUKY = not(!CUFO_Q); if (in.FURO) WENU2 = BUKY; CUFO.tock(in.ENOB, 0, WENU2);
    }

    out.WEZA = WEZA2;
    out.WUCO = WUCO2;
    out.WYDA = WYDA2;
    out.ZYSU = ZYSU2;
    out.WYSE = WYSE2;
    out.WUZY = WUZY2;
    out.CUCA = CUCA2;
    out.CEGA = CEGA2;
    out.CUCU = CUCU2;
    out.WENU = WENU2;
  }
};