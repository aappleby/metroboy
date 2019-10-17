// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool RESET_VIDEO;
extern bool RESET_VIDEO2n;

extern bool FF40_D1;
extern bool FF40_D2;
extern bool P10_B;

extern bool CLK1;
extern bool CLK2;
extern bool CLK3;

extern bool VYPO;
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
extern bool SECA;
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
bool ABEZ;
bool CEHA;
bool XYSO;
bool XOCE;
bool DYTY;
bool BUZA;
bool AVAP;
bool DEGE;
bool DABY;
bool DABU;
bool GYSA;
bool TACU;
bool TEXY;
bool TUVO;
bool WUTY;
bool XUPY;
bool LAPE;

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
static reg FONO;
static reg EXUQ;
static reg WAPO;
static reg WOMY;
static reg WAFY;
static reg XUDY;
static reg GOTA;
static reg EGAV;
static reg CEDY;
static reg EBOJ;

// sprite y comparator reg?


reg WUVU;
static reg WOSU;
static reg CENO;
reg CATU;
static reg BYBA;
static reg DOBA;


reg TYFO;
static reg SEBA;
static reg TOXE;
static reg TULY;
static reg TESE;

// Sprite Y Comparator

static reg YCEB; // oam_b latch
static reg ZUCA;
static reg WONE;
static reg ZAXE;
static reg XAFU;
static reg YSES;
static reg ZECA;
static reg YDYV;

static reg XEGU; // oam_b temp reg?
static reg YJEX;
static reg XYJU;
static reg YBOG;
static reg WYSO;
static reg XOTE;
static reg YZAB;
static reg XUSO;

static reg TOBU;
static reg VONU;

static reg DEZY;
static reg BESE; // "Count from 0 to A"
static reg CUXY;
static reg BEGO;
static reg DYBE;

//-----------------------------------------------------------------------------

void tick_spritecontrol() {

  //----------
  // bottom

  bool WUVU_Q = WUVU.q();
  bool WOSU_Q = WOSU.q();
  bool CENO_Q = CENO.q();
  bool BYBA_Q = BYBA.q();
  bool DOBA_Q = DOBA.q(); 

  bool XYVA = not(CLK1);
  bool XOTA = not(XYVA);
  bool XYFY = not(XOTA);
  bool ALES = not(XYVO);
  bool ABOV = and(SELA, ALES);
  XUPY = not(!WUVU_Q);
  ABEZ = not(RESET_VIDEO2n);
  bool BALU = not(ANOM);
  bool BAGY = not(BALU);
  bool WOJO = nor(!WUVU_Q, !WOSU_Q);
  XOCE = not(WOSU_Q);
  XYSO = not(WOJO);
  CEHA = not(!CENO_Q);
  BUZA = and(!CENO_Q, XYMU);
  bool CARE = or(XOCE, CEHA, SPR_MATCH);
  DYTY = not(CARE);
  bool BEBU = or(DOBA_Q, BALU, !BYBA_Q);
  AVAP = not(BEBU);

  WUVU.flip(XOTA, RESET_VIDEO);
  WOSU.tock(XYFY, RESET_VIDEO, !WUVU_Q);
  CENO.tock(XUPY, ABEZ, BESU);
  CATU.tock(XUPY, ABEZ, ABOV);
  BYBA.tock(XUPY, BAGY, FETO);
  DOBA.tock(CLK2, BAGY, BYBA_Q);

  //----------
  // center thing

  bool TOBU_Q = TOBU.q();
  bool VONU_Q = VONU.q();

  bool TYFO_Q = TYFO.q();
  bool SEBA_Q = SEBA.q();
  bool TOXE_Q = TOXE.q();
  bool TULY_Q = TULY.q();
  bool TESE_Q = TESE.q();

  LAPE = not(CLK2);
  bool TEPA = not(XYMU);

  bool TYNO = nand(TOXE_Q, SEBA_Q, VONU_Q);
  bool VUSA = or(!TYFO_Q, TYNO);
  WUTY = not(VUSA);
  bool XEFY = not(WUTY);

  bool SAKY = nor(TULY_Q, VONU_Q);
  bool TYSO = or(SAKY, TEPA);
  TEXY = not(TYSO);
  bool XONO = and(BAXO, TEXY);

  TUVO = or(TEPA, TULY_Q, TESE_Q);
  bool TAME = nand(TESE_Q, TOXE_Q);
  bool TOMA = nand(LAPE, TAME);

  bool TYTU = not(TAME);
  bool SYCU = nor(TYTU, TEPA, TYFO_Q);
  TACU = nand(TYTU, TYFO_Q);
  bool TOPU = and(TULY_Q, SYCU);
  bool RACA = and(VONU_Q, SYCU);
  bool VYWA = not(TOPU);
  bool WENY = not(VYWA);
  bool XADO = not(WENY);
  bool PEBY = not(RACA);
  bool NYBE = not(PEBY);
  bool PUCO = not(NYBE);

  TYFO.tock(LAPE, VYPO, TAME);
  SEBA.tock(LAPE, XYMU, VONU_Q);
  TOXE.flip(TOMA,    SECA);
  TULY.flip(!TOXE_Q, SECA);
  TESE.flip(!TULY_Q, SECA);

  //----------
  // Sprite priority

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

  bool FONO_Q = FONO.tock(WUTY, BYVA, GUZE);
  bool EXUQ_Q = FONO.tock(WUTY, BYVA, DENY);
  bool WAPO_Q = FONO.tock(WUTY, BYVA, GUGY);
  bool WOMY_Q = FONO.tock(WUTY, BYVA, XYME);
  bool WAFY_Q = FONO.tock(WUTY, BYVA, GYGY);
  bool XUDY_Q = FONO.tock(WUTY, BYVA, GOWO);
  bool GOTA_Q = FONO.tock(WUTY, BYVA, GYMA);
  bool EGAV_Q = FONO.tock(WUTY, BYVA, FAME);
  bool CEDY_Q = FONO.tock(WUTY, BYVA, DYDO);
  bool EBOJ_Q = FONO.tock(WUTY, BYVA, FURO);

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

  //----------
  // Sprite y comparator

  bool YCEB_Q = YCEB.latch(CLK3, OAM_B_D1); // order is weird? does it matter?
  bool ZUCA_Q = YCEB.latch(CLK3, OAM_B_D2);
  bool WONE_Q = YCEB.latch(CLK3, OAM_B_D3);
  bool ZAXE_Q = YCEB.latch(CLK3, OAM_B_D4);
  bool XAFU_Q = YCEB.latch(CLK3, OAM_B_D5);
  bool YSES_Q = YCEB.latch(CLK3, OAM_B_D6);
  bool ZECA_Q = YCEB.latch(CLK3, OAM_B_D7);
  bool YDYV_Q = YCEB.latch(CLK3, OAM_B_D0);

  bool XELE = not(YCEB_Q);
  bool YPON = not(ZUCA_Q);
  bool XUVO = not(WONE_Q);
  bool ZYSA = not(ZAXE_Q);
  bool YWEG = not(XAFU_Q);
  bool XABU = not(YSES_Q);
  bool YTUX = not(ZECA_Q);
  bool YFAP = not(YDYV_Q);

  bool YWOK = not(COTA);

  bool XEGU_Q = XEGU.tock(YWOK, 0, YCEB_Q);
  bool YJEX_Q = YJEX.tock(YWOK, 0, ZUCA_Q);
  bool XYJU_Q = XYJU.tock(YWOK, 0, WONE_Q);
  bool YBOG_Q = YBOG.tock(YWOK, 0, ZAXE_Q);
  bool WYSO_Q = WYSO.tock(YWOK, 0, XAFU_Q);
  bool XOTE_Q = XOTE.tock(YWOK, 0, YSES_Q);
  bool YZAB_Q = YZAB.tock(YWOK, 0, ZECA_Q);
  bool XUSO_Q = XUSO.tock(YWOK, 0, YDYV_Q);

  bool FUGY = not(!XEGU_Q); // only 7 of these?
  bool GAVO = not(!YJEX_Q);
  bool WYGA = not(!XYJU_Q);
  bool WUNE = not(!YBOG_Q);
  bool GOTU = not(!WYSO_Q);
  bool GEGU = not(!XOTE_Q);
  bool XEHE = not(!YZAB_Q);

  bool EBOS = not(V0);
  bool DASA = not(V1);
  bool FUKY = not(V2);
  bool FUVE = not(V3);
  bool FEPU = not(V4);
  bool FOFA = not(V5);
  bool FEMO = not(V6);
  bool GUSU = not(V7);

  bool ERUC_S = add_s(EBOS, !XUSO_Q, P10_B);
  bool ERUC_C = add_c(EBOS, !XUSO_Q, P10_B);
  bool ENEF_C = add_c(DASA, !XEGU_Q, ERUC_C);
  bool ENEF_S = add_s(DASA, !XEGU_Q, ERUC_C);
  bool FECO_C = add_c(FUKY, !YJEX_Q, ENEF_C);
  bool FECO_S = add_s(FUKY, !YJEX_Q, ENEF_C);
  bool GYKY_C = add_c(FUVE, !XYJU_Q, FECO_C);
  bool GYKY_S = add_s(FUVE, !XYJU_Q, FECO_C);
  bool GOPU_C = add_c(FEPU, !YBOG_Q, GYKY_C);
  bool GOPU_S = add_s(FEPU, !YBOG_Q, GYKY_C);
  bool FUWA_C = add_c(FOFA, !WYSO_Q, GOPU_C);
  bool FUWA_S = add_s(FOFA, !WYSO_Q, GOPU_C);
  bool GOJU_C = add_c(FEMO, !XOTE_Q, FUWA_C);
  bool GOJU_S = add_s(FEMO, !XOTE_Q, FUWA_C);
  bool WUHU_C = add_c(GUSU, !YZAB_Q, GOJU_C);
  bool WUHU_S = add_s(GUSU, !YZAB_Q, GOJU_C);

  DEGE = not(ERUC_S);
  DABY = not(ENEF_S);
  DABU = not(FECO_S);
  GYSA = not(GYKY_S);
  bool GACE = not(GOPU_S);
  bool GUVU = not(FUWA_S);
  bool GYDA = not(GOJU_S);
  bool GEWY = not(WUHU_S);

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

  bool FUFO = not(FF40_D2);
  bool GEJY = amux2(!XUSO_Q, FUFO, FF40_D2, WAGO);
  bool FAMU = not(GEJY);
  bool GOVU = or(GYKY_S, FF40_D2);
  bool WOTA = nand(GACE, GUVU, GYDA, GEWY, WUHU_C, GOVU);
  bool GESE = not(WOTA);
  SPR_MATCH = GESE;

  if (WUME) {
    D0 = YFAP;
    D1 = XELE;
    D2 = YPON;
    D3 = XUVO;
    D4 = ZYSA;
    D5 = YWEG;
    D6 = XABU;
    D7 = YTUX;
  }

  bool ABON = not(TEXY);
  if (ABON) {
    MA0 = ABEM;
    MA1 = BAXE;
    MA2 = ARAS;
    MA3 = AGAG;
    MA4 = FAMU;
    MA5 = FUGY;
    MA6 = GAVO;
    MA7 = WYGA;
    MA8 = WUNE;
    MA9 = GOTU;
    MA10 = GEGU;
    MA11 = XEHE;
    MA12 = DYSO;
  }

  TOBU.tock(TAVA, XYMU, TULY_Q);
  VONU.tock(TAVA, XYMU, !TOBU_Q);

  //----------

  bool DEZY_Q = DEZY.q();
  bool BESE_Q = BESE.q();
  bool CUXY_Q = CUXY.q();
  bool BEGO_Q = BEGO.q();
  bool DYBE_Q = DYBE.q();

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

  DEZY.tock(CLK1, RESET_VIDEO, DYTY);
  BESE.flip(CAKE,    AZYB);
  CUXY.flip(!BESE_Q, AZYB);
  BEGO.flip(!CUXY_Q, AZYB);
  DYBE.flip(!BEGO_Q, AZYB);

  //----------
  // unsunk signals

  (void)XADO;
  (void)TACU;
  (void)XONO;
  (void)TUVO;
  (void)XEFY;
  (void)PUCO;
}