#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P29_SpriteControl {
  struct Input {
    bool CLK1;
    bool CLK2;
    bool RESET_VIDEO;
    bool RESET_VIDEO2n;
    bool XYVO;
    bool ANOM;
    bool SELA;
    bool BESU;
    bool FETO;
    bool SPR_MATCH;
    bool XYMU;
    bool WUME; // P28
    bool DYTY;
    bool TAVA;
    bool BAXO;
    bool TEXY;
    bool VYPO;
    bool SECA;
    bool FF40_D1;
    bool P10_B;
    bool BYVA;
    bool CLK3;
    bool COTA;
    bool FF40_D2;
    bool YZOS;
    bool WENU;
    bool CUCU;
    bool CEGA;
    bool AZYB; // P28

    bool YNAZ, YKOK;
    bool ZURE, YWOS;
    bool EKES, CEHU;
    bool EWAM, CYVY;
    bool ZAKO, XEBA;
    bool YLEV, YTUB;
    bool FEHA, DAMA;
    bool CYCO, DAJE;
    bool YDOT, YWAP;
    bool FYMA, COGY;

    bool V0,V1,V2,V3,V4,V5,V6,V7;

    bool OAM_B_D0,OAM_B_D1,OAM_B_D2,OAM_B_D3,OAM_B_D4,OAM_B_D5,OAM_B_D6,OAM_B_D7;
  };

  struct Output {
    // sprite select signals
    bool CACU, BUZY, FUKE;
    bool ZAPE, WUSE, ZURU;
    bool FEFO, GECY, WABE;
    bool FEKA, XYHA, YFAG;
    bool CEXU, AKOL, BYMY;
    bool FUXU, ENOB, GENY;
    bool WEME, WUFA, FAKA;
    bool CYLA, DYMO, BUCY;
    bool WOFO, WYLU, EWOT;
    bool ASYS, AHOF, BYVY;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool MA_OE;
    bool MA0,MA1,MA2,MA3,MA4,MA5,MA6,MA7,MA8,MA9,MA10,MA11,MA12;
  };

  reg WUVU,WOSU,CENO,CATU,BYBA,DOBA;
  reg TYFO,SEBA,TOXE,TULY,TESE;
  reg FONO,EXUQ,WAPO,WOMY,WAFY,XUDY,GOTA,EGAV,CEDY,EBOJ;

  reg YCEB,ZUCA,WONE,ZAXE,XAFU,YSES,ZECA,YDYV;
  reg XEGU,YJEX,XYJU,YBOG,WYSO,XOTE,YZAB,XUSO;
  reg TOBU,VONU;

  reg DEZY,BESE,CUXY,BEGO,DYBE;

  void tick(const Input& in, Output& out) {

    //----------
    // Sprite y comparator

    wire YCEB_Q = YCEB.latch(in.CLK3, in.OAM_B_D1);
    wire ZUCA_Q = ZUCA.latch(in.CLK3, in.OAM_B_D2);
    wire WONE_Q = WONE.latch(in.CLK3, in.OAM_B_D3);
    wire ZAXE_Q = ZAXE.latch(in.CLK3, in.OAM_B_D4);
    wire XAFU_Q = XAFU.latch(in.CLK3, in.OAM_B_D5);
    wire YSES_Q = YSES.latch(in.CLK3, in.OAM_B_D6);
    wire ZECA_Q = ZECA.latch(in.CLK3, in.OAM_B_D7);
    wire YDYV_Q = YDYV.latch(in.CLK3, in.OAM_B_D0);

    wire XELE = not(YCEB_Q);
    wire YPON = not(ZUCA_Q);
    wire XUVO = not(WONE_Q);
    wire ZYSA = not(ZAXE_Q);
    wire YWEG = not(XAFU_Q);
    wire XABU = not(YSES_Q);
    wire YTUX = not(ZECA_Q);
    wire YFAP = not(YDYV_Q);

    wire YWOK = not(in.COTA);

    wire XEGU_Q = XEGU.tock(YWOK, 0, YCEB_Q);
    wire YJEX_Q = YJEX.tock(YWOK, 0, ZUCA_Q);
    wire XYJU_Q = XYJU.tock(YWOK, 0, WONE_Q);
    wire YBOG_Q = YBOG.tock(YWOK, 0, ZAXE_Q);
    wire WYSO_Q = WYSO.tock(YWOK, 0, XAFU_Q);
    wire XOTE_Q = XOTE.tock(YWOK, 0, YSES_Q);
    wire YZAB_Q = YZAB.tock(YWOK, 0, ZECA_Q);
    wire XUSO_Q = XUSO.tock(YWOK, 0, YDYV_Q);

    wire ABON = not(in.TEXY);

    wire FUGY = not(!XEGU_Q);
    wire GAVO = not(!YJEX_Q);
    wire WYGA = not(!XYJU_Q);
    wire WUNE = not(!YBOG_Q);
    wire GOTU = not(!WYSO_Q);
    wire GEGU = not(!XOTE_Q);
    wire XEHE = not(!YZAB_Q);

    wire EBOS = not(in.V0);
    wire DASA = not(in.V1);
    wire FUKY = not(in.V2);
    wire FUVE = not(in.V3);
    wire FEPU = not(in.V4);
    wire FOFA = not(in.V5);
    wire FEMO = not(in.V6);
    wire GUSU = not(in.V7);

    wire ERUC_S = add_s(EBOS, !XUSO_Q, in.P10_B);
    wire ERUC_C = add_c(EBOS, !XUSO_Q, in.P10_B);
    wire ENEF_C = add_c(DASA, !XEGU_Q, ERUC_C);
    wire ENEF_S = add_s(DASA, !XEGU_Q, ERUC_C);
    wire FECO_C = add_c(FUKY, !YJEX_Q, ENEF_C);
    wire FECO_S = add_s(FUKY, !YJEX_Q, ENEF_C);
    wire GYKY_C = add_c(FUVE, !XYJU_Q, FECO_C);
    wire GYKY_S = add_s(FUVE, !XYJU_Q, FECO_C);
    wire GOPU_C = add_c(FEPU, !YBOG_Q, GYKY_C);
    wire GOPU_S = add_s(FEPU, !YBOG_Q, GYKY_C);
    wire FUWA_C = add_c(FOFA, !WYSO_Q, GOPU_C);
    wire FUWA_S = add_s(FOFA, !WYSO_Q, GOPU_C);
    wire GOJU_C = add_c(FEMO, !XOTE_Q, FUWA_C);
    wire GOJU_S = add_s(FEMO, !XOTE_Q, FUWA_C);
    wire WUHU_C = add_c(GUSU, !YZAB_Q, GOJU_C);
    wire WUHU_S = add_s(GUSU, !YZAB_Q, GOJU_C);

    wire DEGE = not(ERUC_S);
    wire DABY = not(ENEF_S);
    wire DABU = not(FECO_S);
    wire GYSA = not(GYKY_S);
    wire GACE = not(GOPU_S);
    wire GUVU = not(FUWA_S);
    wire GYDA = not(GOJU_S);
    wire GEWY = not(WUHU_S);

    wire GOVU = or(GYKY_S, in.FF40_D2);
    wire WOTA = nand(GACE, GUVU, GYDA, GEWY, WUHU_C, GOVU);
    wire GESE = not(WOTA);
    wire SPR_MATCH = GESE;

    if (in.WUME) {
      out.D_OE = true;
      out.D0 = YFAP;
      out.D1 = XELE;
      out.D2 = YPON;
      out.D3 = XUVO;
      out.D4 = ZYSA;
      out.D5 = YWEG;
      out.D6 = XABU;
      out.D7 = YTUX;
    }

    if (ABON) {
      out.MA_OE = true;
      out.MA5 = FUGY;
      out.MA6 = GAVO;
      out.MA7 = WYGA;
      out.MA8 = WUNE;
      out.MA9 = GOTU;
      out.MA10 = GEGU;
      out.MA11 = XEHE;
    }

    //----------
    // bottom
    
    wire XYVA = not(in.CLK1);
    wire XOTA = not(XYVA);
    wire XYFY = not(XOTA);
    wire WUVU_Q = WUVU.flip(XOTA, in.RESET_VIDEO);
    wire ALES = not(in.XYVO);
    wire ABOV = and(in.SELA, ALES);
    wire WOSU_Q = WOSU.tock(XYFY, in.RESET_VIDEO, !WUVU_Q);
    wire XUPY = not(!WUVU_Q);
    wire ABEZ = not(in.RESET_VIDEO2n);
    wire WOJO = nor(!WUVU_Q, !WOSU_Q);
    wire XYSO = not(WOJO);

    wire XOCE = not(WOSU_Q);
    wire CENO_Q = CENO.tock(XUPY, ABEZ, in.BESU);
    wire CEHA = not(!CENO_Q);
    wire BUZA = and(!CENO_Q, in.XYMU);
    wire CARE = or(XOCE, CEHA, in.SPR_MATCH);
    wire DYTY = not(CARE);

    wire CATU_Q = CATU.tock(XUPY, ABEZ, ABOV);

    wire BALU = not(in.ANOM);
    wire BAGY = not(BALU);
    wire BYBA_Q = BYBA.tock(XUPY, BAGY, in.FETO);
    wire DOBA_Q = DOBA.tock(in.CLK2, BAGY, BYBA_Q);
    wire BEBU = or(DOBA_Q, BALU, !BYBA_Q);
    wire AVAP = not(BEBU);

    //----------
    // center thing

    wire TYFO_Q = TYFO.q();
    wire SEBA_Q = SEBA.q();
    wire TOXE_Q = TOXE.q();
    wire TULY_Q = TULY.q();
    wire TESE_Q = TESE.q();

    wire TOBU_Q = TOBU.tock(in.TAVA, in.XYMU, TULY_Q);
    wire VONU_Q = VONU.tock(in.TAVA, in.XYMU, !TOBU_Q);

    wire LAPE = not(in.CLK2);
    wire TEPA = not(in.XYMU);

    wire TYNO = nand(TOXE_Q, SEBA_Q, VONU_Q);
    wire VUSA = or(!TYFO_Q, TYNO);
    wire WUTY = not(VUSA);
    wire XEFY = not(WUTY);

    wire SAKY = nor(TULY_Q, VONU_Q);
    wire TYSO = or(SAKY, TEPA);
    wire TEXY = not(TYSO);
    wire XONO = and(!in.BAXO, in.TEXY);

    wire TUVO = or(TEPA, TULY_Q, TESE_Q);
    wire TAME = nand(TESE_Q, TOXE_Q);
    wire TOMA = nand(LAPE, TAME);

    wire TYTU = not(TAME);
    wire SYCU = nor(TYTU, TEPA, TYFO_Q);
    wire TACU = nand(TYTU, TYFO_Q);
    wire TOPU = and(TULY_Q, SYCU);
    wire RACA = and(VONU_Q, SYCU);
    wire VYWA = not(TOPU);
    wire WENY = not(VYWA);
    wire XADO = not(WENY);
    wire PEBY = not(RACA);
    wire NYBE = not(PEBY);
    wire PUCO = not(NYBE);

    TYFO.tock(LAPE, in.VYPO, TAME);
    SEBA.tock(LAPE, in.XYMU, VONU_Q);
    TOXE.flip(TOMA,    in.SECA);
    TULY.flip(!TOXE_Q, in.SECA);
    TESE.flip(!TULY_Q, in.SECA);

    //----------
    // Sprite priority

    wire BYJO = not(CEHA);
    wire AZEM = and(BYJO, in.XYMU);
    wire AROR = and(AZEM, in.FF40_D1);

    wire XAGE = nand(AROR, in.YNAZ, in.YKOK);
    wire YLOZ = nand(AROR, in.ZURE, in.YWOS);
    wire DEGO = nand(AROR, in.EKES, in.CEHU);
    wire DYDU = nand(AROR, in.EWAM, in.CYVY);
    wire YDUG = nand(AROR, in.ZAKO, in.XEBA);

    wire YGEM = nand(AROR, in.YLEV, in.YTUB);
    wire EFYL = nand(AROR, in.FEHA, in.DAMA);
    wire DYKA = nand(AROR, in.CYCO, in.DAJE);
    wire YBEZ = nand(AROR, in.YDOT, in.YWAP);
    wire EGOM = nand(AROR, in.FYMA, in.COGY);

    wire FEFY = nand(XAGE, YLOZ, DEGO, DYDU, YDUG);
    wire FOVE = nand(YGEM, EFYL, DYKA, YBEZ, EGOM);
    wire FEPO = or(FEFY, FOVE);

    wire WEFU = not(YDUG);
    wire GEZE = or(WEFU, in.P10_B);
    wire GUVA = nor(YDUG, in.P10_B);

    wire GAJA = not(DYDU);
    wire FUMA = or(GAJA, GEZE);
    wire ENUT = nor(DYDU, GEZE);

    wire GUPO = not(DEGO);
    wire GEDE = or(GUPO, FUMA);
    wire EMOL = nor(DEGO, FUMA);

    wire WEBO = not(YLOZ);
    wire WUTO = or(WEBO, GEDE);
    wire GYFY = nor(YLOZ, GEDE);

    wire WUNA = not(XAGE);
    wire XYLA = or(WUNA, WUTO);
    wire GONO = nor(XAGE, WUTO);

    wire GABA = not(EGOM);
    wire WEJA = or(GABA, XYLA);
    wire GEGA = nor(EGOM, XYLA);

    wire WASE = not(YBEZ);
    wire WYLA = or(WASE, WEJA);
    wire XOJA = nor(YBEZ, WEJA);

    wire GYTE = not(DYKA);
    wire FAVO = or(GYTE, WYLA);
    wire GUTU = nor(DYKA, WYLA);

    wire GEKE = not(EFYL);
    wire GYGA = or(GEKE, FAVO);
    wire FOXA = nor(EFYL, FAVO);

    wire GUZE = nor(YGEM, GYGA);

    wire FADO = not(GUZE);
    wire DENY = not(FOXA);
    wire GUGY = not(GUTU);
    wire XYME = not(XOJA);
    wire GYGY = not(GEGA);
    wire GOWO = not(GONO);
    wire GYMA = not(GYFY);
    wire FAME = not(EMOL);
    wire DYDO = not(ENUT);
    wire FURO = not(GUVA);

    wire FONO_Q = FONO.tock(WUTY, in.BYVA, GUZE);
    wire EXUQ_Q = EXUQ.tock(WUTY, in.BYVA, FOXA);
    wire WAPO_Q = WAPO.tock(WUTY, in.BYVA, GUTU);
    wire WOMY_Q = WOMY.tock(WUTY, in.BYVA, XOJA);
    wire WAFY_Q = WAFY.tock(WUTY, in.BYVA, GEGA);
    wire XUDY_Q = XUDY.tock(WUTY, in.BYVA, GONO);
    wire GOTA_Q = GOTA.tock(WUTY, in.BYVA, GYFY);
    wire EGAV_Q = EGAV.tock(WUTY, in.BYVA, EMOL);
    wire CEDY_Q = CEDY.tock(WUTY, in.BYVA, ENUT);
    wire EBOJ_Q = EBOJ.tock(WUTY, in.BYVA, GUVA);

    wire DYBA = not(in.BYVA);

    wire DUBU = or(DYBA, FONO_Q);
    wire GORO = or(DYBA, EXUQ_Q);
    wire GUKY = or(DYBA, WAPO_Q);
    wire WACY = or(DYBA, WOMY_Q);
    wire FEVE = or(DYBA, WAFY_Q);
    wire WOHU = or(DYBA, XUDY_Q);
    wire GAKE = or(DYBA, GOTA_Q);
    wire FOKO = or(DYBA, EGAV_Q);
    wire EFEV = or(DYBA, CEDY_Q);
    wire DYWE = or(DYBA, EBOJ_Q);

    wire DOSY = not(DUBU);
    wire WUZO = not(GORO);
    wire GAFY = not(GUKY);
    wire XAHO = not(WACY);
    wire EJAD = not(FEVE);
    wire WUNU = not(WOHU);
    wire WUPA = not(GAKE);
    wire GAMY = not(FOKO);
    wire DOKU = not(EFEV);
    wire DYNA = not(DYWE);

    //----------
    // Y flip flag

    wire WUKY = not(in.YZOS);
    wire WAGO = xor(WUKY, in.WENU);
    wire CYVU = xor(WUKY, in.CUCU);
    wire BORE = xor(WUKY, in.WENU);
    wire BUVY = xor(WUKY, in.CEGA);
    wire XUQU = not(!VONU_Q);

    wire BAXE = not(CYVU);
    wire ARAS = not(BORE);
    wire AGAG = not(BUVY);
    wire ABEM = not(XUQU);
    wire DYSO = not(in.P10_B);

    wire FUFO = not(in.FF40_D2);
    wire GEJY = amux2(!XUSO_Q, FUFO, in.FF40_D2, WAGO);
    wire FAMU = not(GEJY);

    if (ABON) {
      out.MA_OE = true;
      out.MA1 = BAXE;
      out.MA2 = ARAS;
      out.MA3 = AGAG;
      out.MA0 = ABEM;
      out.MA12 = DYSO;
    }

    //----------
    // Decoder, right side

    wire DEZY_Q = DEZY.q();
    wire BESE_Q = BESE.q();
    wire CUXY_Q = CUXY.q();
    wire BEGO_Q = BEGO.q();
    wire DYBE_Q = DYBE.q();

    wire BAKY = and(CUXY_Q, DYBE_Q);
    wire CAKE = or(BAKY, DEZY_Q);
    wire EDEN = not(BESE_Q);
    wire FYCU = not(EDEN);
    wire CYPY = not(CUXY_Q);
    wire FONE = not(CYPY);
    wire CAPE = not(BEGO_Q);
    wire EKUD = not(CAPE);
    wire CAXU = not(DYBE_Q);
    wire ELYG = not(CAXU);

    // CAXU ELYG CAPE EKUD CYPY FYCU EDEN FONE

    wire GEBU = nand(EDEN, FONE, CAPE, CAXU);
    wire WOMU = nand(EDEN, FONE, EKUD, CAXU);
    wire GUNA = nand(FYCU, FONE, EKUD, CAXU);
    wire FOCO = nand(FYCU, FONE, CAPE, CAXU);
    wire DEWY = nand(EDEN, CYPY, CAPE, ELYG);
    wire DEZO = nand(EDEN, CYPY, CAPE, CAXU);
    wire DOGU = nand(FYCU, CYPY, CAPE, ELYG);
    wire CUGU = nand(FYCU, CYPY, EKUD, CAPE); // this one is weird... schematic probably wrong, these all decode to numbers...
    wire CUPE = nand(EDEN, CYPY, EKUD, CAXU);
    wire CUVA = nand(FYCU, CYPY, CAPE, CAXU); // also bit weird? schematic says 0?01 but it seems to be a normal decode...

    wire WYXO = or(in.DYTY, GEBU);
    wire XUJO = or(in.DYTY, WOMU);
    wire GAPE = or(in.DYTY, GUNA);
    wire GUVE = or(in.DYTY, FOCO);
    wire CAHO = or(in.DYTY, DEWY);
    wire CEMY = or(in.DYTY, DEZO);
    wire CATO = or(in.DYTY, DOGU);
    wire CADO = or(in.DYTY, CUGU);
    wire CECU = or(in.DYTY, CUPE);
    wire BYBY = or(in.DYTY, CUVA);

    // why so many signals?
    wire GYFO = not(WYXO);
    wire WEKA = not(XUJO);
    wire GYVO = not(GAPE);
    wire GUSA = not(GUVE);
    wire BUKA = not(CAHO);
    wire DYHU = not(CEMY);
    wire DECU = not(CATO);
    wire BEDE = not(CADO);
    wire DUKE = not(CECU);
    wire BUCO = not(BYBY);

    out.CACU = not(GYFO); out.BUZY = not(GYFO); out.FUKE = not(GYFO);
    out.ZAPE = not(WEKA); out.WUSE = not(WEKA); out.ZURU = not(WEKA);
    out.FEFO = not(GYVO); out.GECY = not(GYVO); out.WABE = not(GYVO);
    out.FEKA = not(GUSA); out.XYHA = not(GUSA); out.YFAG = not(GUSA);
    out.CEXU = not(BUKA); out.AKOL = not(BUKA); out.BYMY = not(BUKA);
    out.FUXU = not(DYHU); out.ENOB = not(DYHU); out.GENY = not(DYHU);
    out.WEME = not(DECU); out.WUFA = not(DECU); out.FAKA = not(DECU);
    out.CYLA = not(BEDE); out.DYMO = not(BEDE); out.BUCY = not(BEDE);
    out.WOFO = not(DUKE); out.WYLU = not(DUKE); out.EWOT = not(DUKE);
    out.ASYS = not(BUCO); out.AHOF = not(BUCO); out.BYVY = not(BUCO);

    DEZY.tock(in.CLK1, in.RESET_VIDEO, in.DYTY);
    BESE.flip(CAKE,    in.AZYB);
    CUXY.flip(!BESE_Q, in.AZYB);
    BEGO.flip(!CUXY_Q, in.AZYB);
    DYBE.flip(!BEGO_Q, in.AZYB);
  }
};