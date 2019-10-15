// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool RESET_VIDEO;
extern bool RESET_VIDEO2N;

extern bool FF40_D1;
extern bool FF40_D2;
extern bool P10_B;

extern bool CLK1;
extern bool CLK2;
extern bool CLK3;

extern bool AZYB;
extern bool BAXO;
extern bool BESU;
extern bool FETO;
extern bool ANOM;
extern bool XYVO;
extern bool SELA;
extern bool XYMU;
extern bool BYVA;
extern bool WUME;
extern bool COTA;
extern bool TAVA;
extern bool TULY;
extern bool XYMU;
extern bool YZOS;
extern bool WENU;
extern bool CUCU;
extern bool CEGA;
extern bool XEBA;
extern bool ZAKO;
extern bool YWOS;
extern bool ZURE;
extern bool DAJE;
extern bool CYCO;
extern bool CYVY;
extern bool EWAM;
extern bool YWAP;
extern bool YDOT;
extern bool YKOK;
extern bool YNAZ;
extern bool DAMA;
extern bool FEHA;
extern bool YTUB;
extern bool YLEV;
extern bool COGY;
extern bool FYMA;
extern bool CEHU;
extern bool EKES;

//----------
// outputs

bool SPR_MATCH;
bool FEPO;

bool XYSO;
bool XOCE;
bool DYTY;
bool BUZA;
bool CATU;
bool AVAP;

bool DEGE;
bool DABY;
bool DABU;
bool GYSA;

// sprite hit flags
bool FADO;
bool DENY;
bool GUGY;
bool XYME;
bool GYGY;
bool GOWO;
bool GYMA;
bool FAME;
bool DYDO;
bool FURO;

// sprite hit flags 2
bool DOSY;
bool WUZO;
bool GAFY;
bool XAHO;
bool EJAD;
bool WUNU;
bool WUPA;
bool GAMY;
bool DOKU;
bool DYNA;

// sprite hit flags or something
bool CACU; bool BUZY; bool FUKE;
bool ZAPE; bool WUSE; bool ZURU;
bool FEFO; bool GECY; bool WABE;
bool FEKA; bool XYHA; bool YFAG;
bool CEXU; bool AKOL; bool BYMY;
bool FUXU; bool ENOB; bool GENY;
bool WEME; bool WUFA; bool FAKA;
bool CYLA; bool DYMO; bool BUCY;
bool WOFO; bool WYLU; bool EWOT;
bool ASYS; bool AHOF; bool BYVY;

//----------
// mystery signals

//----------
// registers

// sprite hit regs
static bool FONO_Q;
static bool EXUQ_Q;
static bool WAPO_Q;
static bool WOMY_Q;
static bool WAFY_Q;
static bool XUDY_Q;
static bool GOTA_Q;
static bool EGAV_Q;
static bool CEDY_Q;
static bool EBOJ_Q;
static bool WUTY_CLK;

// oam_b latch
static bool YCEB_L;
static bool ZUCA_L;
static bool WONE_L;
static bool ZAXE_L;
static bool XAFU_L;
static bool YSES_L;
static bool ZECA_L;
static bool YDYV_L;

// sprite y comparator reg?
static bool XEGU_Q;
static bool YJEX_Q;
static bool XYJU_Q;
static bool YBOG_Q;
static bool WYSO_Q;
static bool XOTE_Q;
static bool YZAB_Q;
static bool XUSO_Q;
static bool YWOK_CLK;

static bool TOBU_Q;
static bool VONU_Q;
static bool TAVA_CLK;

static bool WUVU_Q;
static bool XOTA_CLK;

static bool WOSU_Q;
static bool XYFY_CLK;

static bool CENO_Q;
static bool CATU_Q;
static bool BYBA_Q;
static bool XUPY_CLK;

static bool DOBA_Q;
static bool CLK2_CLK;

static bool TYFO_Q;
static bool SEBA_Q;
static bool TOXE_Q;
static bool TULY_Q;
static bool TESE_Q;

static bool LAPE_CLK;
static bool TOMA_CLK;

static bool DEZY_Q;
static bool BESE_Q;
static bool CUXY_Q;
static bool BEGO_Q;
static bool DYBE_Q;
static bool CLK1_CLK;
static bool CAKE_CLK;

//-----------------------------------------------------------------------------

void tick_spritecontrol() {
  bool CENO_N = not(CENO_Q);
  bool CEHA = not(CENO_N);

  bool BYJO = not(CEHA);
  bool AZEM = and(BYJO, XYMU);
  bool AROR = and(AZEM, FF40_D1);

  bool XAGE = nand(AROR, YNAZ, YKOK);
  bool YLOZ = nand(AROR, ZURE, YWOS);
  bool DEGO = nand(AROR, EKES, CEHU);
  bool DYDU = nand(AROR, EWAM, CYVY);
  bool YDUG = nand(AROR, ZAKO, XEBA);

  bool YGEM = nand(AROR, YLEV, YTUB);
  bool EFYL = nand(AROR, FEHA, DAMA);
  bool DYKA = nand(AROR, CYCO, DAJE);
  bool YBEZ = nand(AROR, YDOT, YWAP);
  bool EGOM = nand(AROR, FYMA, COGY);

  bool FEFY = nand(XAGE, YLOZ, DEGO, DYDU, YDUG);
  bool FOVE = nand(YGEM, EFYL, DYKA, YBEZ, EGOM);

  FEPO = or(FEFY, FOVE);

  bool WEFU = not(YDUG);
  bool GEZE = or(WEFU, P10_B);
  bool GUVA = nor(YDUG, P10_B);

  bool GAJA = not(DYDU);
  bool FUMA = or(GAJA, GEZE);
  bool ENUT = nor(DYDU, GEZE);

  bool GUPO = not(DEGO);
  bool GEDE = or(GUPO, FUMA);
  bool EMOL = nor(DEGO, FUMA);

  bool WEBO = not(YLOZ);
  bool WUTO = or(WEBO, GEDE);
  bool GYFY = nor(YLOZ, GEDE);

  bool WUNA = not(XAGE);
  bool XYLA = or(WUNA, WUTO);
  bool GONO = nor(XAGE, WUTO);


  bool GABA = not(EGOM);
  bool WEJA = or(GABA, XYLA);
  bool GEGA = nor(EGOM, XYLA);

  bool WASE = not(YBEZ);
  bool WYLA = or(WASE, WEJA);
  bool XOJA = nor(YBEZ, WEJA);

  bool GYTE = not(DYKA);
  bool FAVO = or(GYTE, WYLA);
  bool GUTU = nor(DYKA, WYLA);

  bool GEKE = not(EFYL);
  bool GYGA = or(GEKE, FAVO);
  bool FOXA = nor(EFYL, FAVO);

  bool GUZE = nor(YGEM, GYGA);

  FADO = not(GUZE);
  DENY = not(FOXA);
  GUGY = not(GUTU);
  XYME = not(XOJA);
  GYGY = not(GEGA);
  GOWO = not(GONO);
  GYMA = not(GYFY);
  FAME = not(EMOL);
  DYDO = not(ENUT);
  FURO = not(GUVA);

  bool DYBA = not(BYVA);

  bool DUBU = or(DYBA, FONO_Q);
  bool GORO = or(DYBA, EXUQ_Q);
  bool GUKY = or(DYBA, WAPO_Q);
  bool WACY = or(DYBA, WOMY_Q);
  bool FEVE = or(DYBA, WAFY_Q);
  bool WOHU = or(DYBA, XUDY_Q);
  bool GAKE = or(DYBA, GOTA_Q);
  bool FOKO = or(DYBA, EGAV_Q);
  bool EFEV = or(DYBA, CEDY_Q);
  bool DYWE = or(DYBA, EBOJ_Q);

  DOSY = not(DUBU);
  WUZO = not(GORO);
  GAFY = not(GUKY);
  XAHO = not(WACY);
  EJAD = not(FEVE);
  WUNU = not(WOHU);
  WUPA = not(GAKE);
  GAMY = not(FOKO);
  DOKU = not(EFEV);
  DYNA = not(DYWE);

  // sprite y comparator

  // active low latch?
  if (CLK3) {
    YCEB_L = OAM_B_D1;
    ZUCA_L = OAM_B_D2;
    WONE_L = OAM_B_D3;
    ZAXE_L = OAM_B_D4;
    XAFU_L = OAM_B_D5;
    YSES_L = OAM_B_D6;
    ZECA_L = OAM_B_D7;
    YDYV_L = OAM_B_D0;
  }

  bool XELE = YCEB_L;
  bool YPON = ZUCA_L;
  bool XUVO = WONE_L;
  bool ZYSA = ZAXE_L;
  bool YWEG = XAFU_L;
  bool XABU = YSES_L;
  bool YTUX = ZECA_L;
  bool YFAP = YDYV_L;

  if (WUME) {
    D1 = XELE;
    D2 = YPON;
    D3 = XUVO;
    D4 = ZYSA;
    D5 = YWEG;
    D6 = XABU;
    D7 = YTUX;
    D0 = YFAP;
  }

  bool TEPA = not(XYMU);
  bool SAKY = nor(TULY_Q, VONU_Q);
  bool TYSO = or(SAKY, TEPA);
  bool TEXY = not(TYSO);
  bool XONO = and(BAXO, TEXY);

  bool YWOK = not(COTA);
  bool ABON = not(TEXY);

  // only 7 of these?
  bool FUGY = not(!XEGU_Q);
  bool GAVO = not(!YJEX_Q);
  bool WYGA = not(!XYJU_Q);
  bool WUNE = not(!YBOG_Q);
  bool GOTU = not(!WYSO_Q);
  bool GEGU = not(!XOTE_Q);
  bool XEHE = not(!YZAB_Q);

  if (ABON) {
    MA5 = FUGY;
    MA6 = GAVO;
    MA7 = WYGA;
    MA8 = WUNE;
    MA9 = GOTU;
    MA10 = GEGU;
    MA11 = XEHE;
  }

  bool WUKY = not(YZOS);
  bool WAGO = xor(WUKY, WENU);
  bool CYVU = xor(WUKY, CUCU);
  bool BORE = xor(WUKY, WENU);
  bool BUVY = xor(WUKY, CEGA);

  bool XUQU = not(!VONU_Q);
  bool BAXE = not(CYVU);
  bool ARAS = not(BORE);
  bool AGAG = not(BUVY);
  bool ABEM = not(XUQU);
  bool DYSO = not(P10_B);

  if (ABON) {
    MA1 = BAXE;
    MA2 = ARAS;
    MA3 = AGAG;
    MA0 = ABEM;
    MA12 = DYSO;
  }

  bool EBOS = not(V0);
  bool DASA = not(V1);
  bool FUKY = not(V2);
  bool FUVE = not(V3);
  bool FEPU = not(V4);
  bool FOFA = not(V5);
  bool FEMO = not(V6);
  bool GUSU = not(V7);

  // FIXME totally wrong, probably a counter...
  bool ERUC = unk1(/*CIN=*/P10_B, /*COUT=*/0, EBOS, !XUSO_Q);
  bool ENEF = unk1(/*CIN=*/0,     /*COUT=*/0, DASA, !XEGU_Q);
  bool FECO = unk1(/*CIN=*/0,     /*COUT=*/0, FUKY, !YJEX_Q);
  bool GYKY = unk1(/*CIN=*/0,     /*COUT=*/0, FUVE, !XYJU_Q);
  bool GOPU = unk1(/*CIN=*/0,     /*COUT=*/0, FEPU, !YBOG_Q);
  bool FUWA = unk1(/*CIN=*/0,     /*COUT=*/0, FOFA, !WYSO_Q);
  bool GOJU = unk1(/*CIN=*/0,     /*COUT=*/0, FEMO, !XOTE_Q);
  bool WUHU = unk1(/*CIN=*/0,     /*COUT=*/0, GUSU, !YZAB_Q);

  DEGE = not(ERUC);
  DABY = not(ENEF);
  DABU = not(FECO);
  GYSA = not(GYKY);
  bool GACE = not(GOPU);
  bool GUVU = not(FUWA);
  bool GYDA = not(GOJU);
  bool GEWY = not(WUHU);
  
  bool GOVU = or(GYKY, FF40_D2);
  bool FUFO = not(FF40_D2);
  bool GEJY = amux2(!XUSO_Q, FUFO, FF40_D2, WAGO);
  bool FAMU = not(GEJY);

  if (ABON) {
    MA4 = FAMU;
  }

  // FIXME
  bool WOTA = nand(GACE, GUVU, GYDA, GEWY, /*WUHU_COUT*/0, GOVU);
  bool GESE = not(WOTA);
  SPR_MATCH = GESE;

  bool WUVU_N = not(WUVU_Q);
  bool WOSU_N = not(WOSU_Q);
  bool BYBA_N = not(BYBA_Q);

  bool XYVA = not(CLK1);
  bool XOTA = not(XYVA);
  bool XYFY = not(XOTA);
  bool XUPY = not(WUVU_N);
  bool ABEZ = not(RESET_VIDEO2N);
  bool BALU = not(ANOM);
  bool BAGY = not(BALU);
  bool WOJO = nor(WUVU_N, WOSU_N);
  XOCE = not(WOSU_Q);
  XYSO = not(WOJO);
  bool ALES = not(XYVO);
  bool ABOV = and(SELA, ALES);
  BUZA = and(CENO_N, XYMU);
  bool CARE = or(XOCE, CEHA, SPR_MATCH);
  DYTY = not(CARE);

  bool BEBU = or(DOBA_Q, BALU, BYBA_N);
  AVAP = not(BEBU);

  bool LAPE = not(CLK2);
  bool TYNO = nand(TOXE_Q, SEBA_Q, VONU_Q);
  bool VUSA = or(!TYFO_Q, TYNO);
  bool WUTY = not(VUSA);
  bool XEFY = not(WUTY);

  bool TUVO = or(TEPA, TULY_Q, TESE_Q);
  bool TAME = nand(TESE_Q, TOXE_Q);
  bool TOMA = nand(LAPE, TAME);

  bool TYTU = not(TAME);
  bool SYCU = nor(TYTU, TEPA, TYFO_Q);
  bool TACU = nand(TYTU, TYFO_Q);
  bool TOPU = and(TULY, SYCU);
  bool RACA = and(VONU_Q, SYCU);
  bool VYWA = not(TOPU);
  bool WENY = not(VYWA);
  bool XADO = not(WENY);
  bool PEBY = not(RACA);
  bool NYBE = not(PEBY);
  bool PUCO = not(NYBE);

  bool BAKY = and(CUXY_Q, DYBE_Q);
  bool CAKE = or(BAKY, DEZY_Q);
  bool EDEN = not(BESE_Q);
  bool FYCU = not(EDEN);
  bool CYPY = not(CUXY_Q);
  bool FONE = not(CYPY);
  bool CAPE = not(BEGO_Q);
  bool EKUD = not(CAPE);
  bool CAXU = not(DYBE_Q);
  bool ELYG = not(CAXU);

  // CAXU ELYG CAPE EKUD CYPY FYCU EDEN FONE

  bool GEBU = nand(EDEN, FONE, CAPE, CAXU);
  bool WOMU = nand(EDEN, FONE, EKUD, CAXU);
  bool GUNA = nand(FYCU, FONE, EKUD, CAXU);
  bool FOCO = nand(FYCU, FONE, CAPE, CAXU);
  bool DEWY = nand(EDEN, CYPY, CAPE, ELYG);
  bool DEZO = nand(EDEN, CYPY, CAPE, CAXU);
  bool DOGU = nand(FYCU, CYPY, CAPE, ELYG);
  bool CUGU = nand(FYCU, CYPY, EKUD, CAPE); // this one is weird... schematic probably wrong, these all decode to numbers...
  bool CUPE = nand(EDEN, CYPY, EKUD, CAXU);
  bool CUVA = nand(FYCU, CYPY, CAPE, CAXU); // also bit weird? schematic says 0?01 but it seems to be a normal decode...

  bool WYXO = or(DYTY, GEBU);
  bool XUJO = or(DYTY, WOMU);
  bool GAPE = or(DYTY, GUNA);
  bool GUVE = or(DYTY, FOCO);
  bool CAHO = or(DYTY, DEWY);
  bool CEMY = or(DYTY, DEZO);
  bool CATO = or(DYTY, DOGU);
  bool CADO = or(DYTY, CUGU);
  bool CECU = or(DYTY, CUPE);
  bool BYBY = or(DYTY, CUVA);

  // why so many signals?
  bool GYFO = not(WYXO);
  bool WEKA = not(XUJO);
  bool GYVO = not(GAPE);
  bool GUSA = not(GUVE);
  bool BUKA = not(CAHO);
  bool DYHU = not(CEMY);
  bool DECU = not(CATO);
  bool BEDE = not(CADO);
  bool DUKE = not(CECU);
  bool BUCO = not(BYBY);

  CACU = not(GYFO); BUZY = not(GYFO); FUKE = not(GYFO);
  ZAPE = not(WEKA); WUSE = not(WEKA); ZURU = not(WEKA);
  FEFO = not(GYVO); GECY = not(GYVO); WABE = not(GYVO);
  FEKA = not(GUSA); XYHA = not(GUSA); YFAG = not(GUSA);
  CEXU = not(BUKA); AKOL = not(BUKA); BYMY = not(BUKA);
  FUXU = not(DYHU); ENOB = not(DYHU); GENY = not(DYHU);
  WEME = not(DECU); WUFA = not(DECU); FAKA = not(DECU);
  CYLA = not(BEDE); DYMO = not(BEDE); BUCY = not(BEDE);
  WOFO = not(DUKE); WYLU = not(DUKE); EWOT = not(DUKE);
  ASYS = not(BUCO); AHOF = not(BUCO); BYVY = not(BUCO);

  //----------
  // registers

  bool WUTY_CLK_ = WUTY;
  if (WUTY_CLK && !WUTY_CLK_) {
    FONO_Q = GUZE;
    EXUQ_Q = DENY;
    WAPO_Q = GUGY;
    WOMY_Q = XYME;
    WAFY_Q = GYGY;
    XUDY_Q = GOWO;
    GOTA_Q = GYMA;
    EGAV_Q = FAME;
    CEDY_Q = DYDO;
    EBOJ_Q = FURO;
  }
  if (!BYVA) {
    FONO_Q = 0;
    EXUQ_Q = 0;
    WAPO_Q = 0;
    WOMY_Q = 0;
    WAFY_Q = 0;
    XUDY_Q = 0;
    GOTA_Q = 0;
    EGAV_Q = 0;
    CEDY_Q = 0;
    EBOJ_Q = 0;
  }
  WUTY_CLK = WUTY_CLK_;

  bool YWOK_CLK_ = YWOK;
  if (YWOK_CLK && !YWOK_CLK_) {
    XEGU_Q = YCEB_L;
    YJEX_Q = ZUCA_L;
    XYJU_Q = WONE_L;
    YBOG_Q = ZAXE_L;
    WYSO_Q = XAFU_L;
    XOTE_Q = YSES_L;
    YZAB_Q = ZECA_L;
    XUSO_Q = YDYV_L;
  }
  YWOK_CLK = YWOK_CLK_;

  bool TOBU_Q_ = TOBU_Q;
  bool VONU_Q_ = VONU_Q;
  bool TAVA_CLK_ = TAVA;
  if (TAVA_CLK && !TAVA_CLK_) {
    TOBU_Q_ = TULY;
    VONU_Q_ = !TOBU_Q;
  }
  if (!XYMU) {
    TOBU_Q_ = 0;
    VONU_Q_ = 0;
  }
  TOBU_Q = TOBU_Q_;
  VONU_Q = VONU_Q_;
  TAVA_CLK = TAVA_CLK_;

  //----------
  // more registers

  bool WUVU_Q_ = WUVU_Q;
  bool WOSU_Q_ = WOSU_Q;
  bool CENO_Q_ = CENO_Q;
  bool CATU_Q_ = CATU_Q;
  bool BYBA_Q_ = BYBA_Q;
  bool DOBA_Q_ = DOBA_Q;

  bool XOTA_CLK_ = XOTA;
  bool XYFY_CLK_ = XYFY;
  bool XUPY_CLK_ = XUPY;
  bool CLK2_CLK_ = CLK2;

  if (XOTA_CLK && !XOTA_CLK_) WUVU_Q_ = WUVU_N;
  if (XYFY_CLK && !XYFY_CLK_) WOSU_Q_ = WUVU_N;
  if (XUPY_CLK && !XUPY_CLK_) CENO_Q_ = BESU;
  if (XUPY_CLK && !XUPY_CLK_) CATU_Q_ = ABOV;
  if (XUPY_CLK && !XUPY_CLK_) BYBA_Q_ = FETO;
  if (CLK2_CLK && !CLK2_CLK_) DOBA_Q_ = BYBA_Q;

  if (!RESET_VIDEO) WUVU_Q_ = 0;
  if (!RESET_VIDEO) WOSU_Q_ = 0;
  if (!ABEZ) CENO_Q_ = 0;
  if (!ABEZ) CATU_Q_ = 0;
  if (!BAGY) BYBA_Q_ = 0;
  if (!BAGY) DOBA_Q_ = 0;

  WUVU_Q = WUVU_Q_;
  WOSU_Q = WOSU_Q_;
  CENO_Q = CENO_Q_;
  CATU_Q = CATU_Q_;
  BYBA_Q = BYBA_Q_;
  DOBA_Q = DOBA_Q_;

  XOTA_CLK = XOTA_CLK_;
  XYFY_CLK = XYFY_CLK_;
  XUPY_CLK = XUPY_CLK_;
  CLK2_CLK = CLK2_CLK_;

  //----------
  // more registers

  bool TYFO_Q_ = TYFO_Q;
  bool SEBA_Q_ = SEBA_Q;
  bool TOXE_Q_ = TOXE_Q;
  bool TULY_Q_ = TULY_Q;
  bool TESE_Q_ = TESE_Q;

  if (LAPE_CLK && !LAPE)  TYFO_Q_ = TAME;
  if (LAPE_CLK && !LAPE)  SEBA_Q_ = VONU_Q;
  if (TOMA_CLK && !TOMA)  TOXE_Q_ = !TOXE_Q;
  if (!TOXE_Q && TOXE_Q_) TULY_Q_ = !TULY_Q;
  if (!TULY_Q && TULY_Q_) TESE_Q_ = !TESE_Q;

  TYFO_Q = TYFO_Q_;
  SEBA_Q = SEBA_Q_;
  TOXE_Q = TOXE_Q_;
  TULY_Q = TULY_Q_;
  TESE_Q = TESE_Q_;

  LAPE_CLK = LAPE;
  TOMA_CLK = TOMA;

  //----------
  // more registers

  bool DEZY_Q_ = DEZY_Q;
  bool BESE_Q_ = BESE_Q;
  bool CUXY_Q_ = CUXY_Q;
  bool BEGO_Q_ = BEGO_Q;
  bool DYBE_Q_ = DYBE_Q;

    // not sure if this will tick right...
  if (CLK1_CLK && !CLK1) DEZY_Q_ = DYTY;
  if (CAKE_CLK && !CAKE) BESE_Q_ = !BESE_Q;
  if (!BESE_Q && BESE_Q_) CUXY_Q_ = !CUXY_Q;
  if (!CUXY_Q && CUXY_Q_) BEGO_Q_ = !BEGO_Q;
  if (!BEGO_Q && BEGO_Q_) DYBE_Q_ = !DYBE_Q;

  if (!RESET_VIDEO) DEZY_Q_ = 0;
  if (!AZYB) BESE_Q_ = 0;
  if (!AZYB) CUXY_Q_ = 0;
  if (!AZYB) BEGO_Q_ = 0;
  if (!AZYB) DYBE_Q_ = 0;

  DEZY_Q = DEZY_Q_;
  BESE_Q = BESE_Q_;
  CUXY_Q = CUXY_Q_;
  BEGO_Q = BEGO_Q_;
  DYBE_Q = DYBE_Q_;

  CLK1_CLK = CLK1;
  CAKE_CLK = CAKE;

  //----------
  // unsunk signals

  (void)XADO;
  (void)TACU;
  (void)XONO;
  (void)TUVO;
  (void)XEFY;
  (void)PUCO;
}