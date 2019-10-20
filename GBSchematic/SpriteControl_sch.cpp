// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "OamBus.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "VramBus.h"
#include "PPU.h"
#include "Clocks.h"
#include "Sprites.h"
#include "Window.h"

//----------
// inputs

extern reg BAXO;
extern reg YZOS;

extern bool XYVO;
extern bool SELA;
extern bool XYMU;

extern bool XYMU;
extern bool WENU;
extern bool CUCU;
extern bool CEGA;

//-----------------------------------------------------------------------------

void Sprites::tick_control() {

  //----------
  // bottom

  wire WUVU_Q = WUVU.q();
  wire WOSU_Q = WOSU.q();
  wire CENO_Q = CENO.q();
  wire BYBA_Q = BYBA.q();
  wire DOBA_Q = DOBA.q(); 

  wire XYVA = not(clk.CLK1);
  wire XOTA = not(XYVA);
  wire XYFY = not(XOTA);
  wire ALES = not(XYVO);
  wire ABOV = and(SELA, ALES);
  spr.XUPY = not(!WUVU_Q);
  spr.ABEZ = not(rst.RESET_VIDEO2n);
  wire BALU = not(spr.ANOM);
  wire BAGY = not(BALU);
  wire WOJO = nor(!WUVU_Q, !WOSU_Q);
  spr.XOCE = not(WOSU_Q);
  spr.XYSO = not(WOJO);
  spr.CEHA = not(!CENO_Q);
  spr.BUZA = and(!CENO_Q, XYMU);
  wire CARE = or(spr.XOCE, spr.CEHA, spr.SPR_MATCH);
  spr.DYTY = not(CARE);
  wire BEBU = or(DOBA_Q, BALU, !BYBA_Q);
  spr.AVAP = not(BEBU);

  WUVU.flip(XOTA, rst.RESET_VIDEO);
  WOSU.tock(XYFY, rst.RESET_VIDEO, !WUVU_Q);
  CENO.tock(spr.XUPY, spr.ABEZ, spr.BESU);
  CATU.tock(spr.XUPY, spr.ABEZ, ABOV);
  BYBA.tock(spr.XUPY, BAGY, spr.FETO);
  DOBA.tock(clk.CLK2, BAGY, BYBA_Q);

  //----------
  // center thing

  wire TOBU_Q = TOBU.q();
  wire VONU_Q = VONU.q();

  wire TYFO_Q = TYFO.q();
  wire SEBA_Q = SEBA.q();
  wire TOXE_Q = TOXE.q();
  wire TULY_Q = TULY.q();
  wire TESE_Q = TESE.q();

  spr.LAPE = not(clk.CLK2);
  wire TEPA = not(XYMU);

  wire TYNO = nand(TOXE_Q, SEBA_Q, VONU_Q);
  wire VUSA = or(!TYFO_Q, TYNO);
  spr.WUTY = not(VUSA);
  spr.XEFY = not(spr.WUTY);

  wire SAKY = nor(TULY_Q, VONU_Q);
  wire TYSO = or(SAKY, TEPA);
  spr.TEXY = not(TYSO);
  wire BAXO_Q = BAXO.q();
  spr.XONO = and(!BAXO_Q, spr.TEXY);

  spr.TUVO = or(TEPA, TULY_Q, TESE_Q);
  wire TAME = nand(TESE_Q, TOXE_Q);
  wire TOMA = nand(spr.LAPE, TAME);

  wire TYTU = not(TAME);
  wire SYCU = nor(TYTU, TEPA, TYFO_Q);
  spr.TACU = nand(TYTU, TYFO_Q);
  wire TOPU = and(TULY_Q, SYCU);
  wire RACA = and(VONU_Q, SYCU);
  wire VYWA = not(TOPU);
  wire WENY = not(VYWA);
  spr.XADO = not(WENY);
  wire PEBY = not(RACA);
  wire NYBE = not(PEBY);
  spr.PUCO = not(NYBE);

  TYFO.tock(spr.LAPE, win.VYPO, TAME);
  SEBA.tock(spr.LAPE, XYMU, VONU_Q);
  TOXE.flip(TOMA,    win.SECA);
  TULY.flip(!TOXE_Q, win.SECA);
  TESE.flip(!TULY_Q, win.SECA);

  //----------
  // Sprite priority

  wire BYJO = not(spr.CEHA);
  wire AZEM = and(BYJO, XYMU);
  wire AROR = and(AZEM, ppu.FF40_D1);

  wire XAGE = nand(AROR, spr.YNAZ, spr.YKOK);
  wire YLOZ = nand(AROR, spr.ZURE, spr.YWOS);
  wire DEGO = nand(AROR, spr.EKES, spr.CEHU);
  wire DYDU = nand(AROR, spr.EWAM, spr.CYVY);
  wire YDUG = nand(AROR, spr.ZAKO, spr.XEBA);

  wire YGEM = nand(AROR, spr.YLEV, spr.YTUB);
  wire EFYL = nand(AROR, spr.FEHA, spr.DAMA);
  wire DYKA = nand(AROR, spr.CYCO, spr.DAJE);
  wire YBEZ = nand(AROR, spr.YDOT, spr.YWAP);
  wire EGOM = nand(AROR, spr.FYMA, spr.COGY);

  wire FEFY = nand(XAGE, YLOZ, DEGO, DYDU, YDUG);
  wire FOVE = nand(YGEM, EFYL, DYKA, YBEZ, EGOM);
  spr.FEPO = or(FEFY, FOVE);

  wire WEFU = not(YDUG);
  wire GEZE = or(WEFU, ext.P10_B);
  wire GUVA = nor(YDUG, ext.P10_B);

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

  spr.FADO = not(GUZE);
  spr.DENY = not(FOXA);
  spr.GUGY = not(GUTU);
  spr.XYME = not(XOJA);
  spr.GYGY = not(GEGA);
  spr.GOWO = not(GONO);
  spr.GYMA = not(GYFY);
  spr.FAME = not(EMOL);
  spr.DYDO = not(ENUT);
  spr.FURO = not(GUVA);

  wire FONO_Q = FONO.tock(spr.WUTY, spr.BYVA, GUZE);
  wire EXUQ_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.DENY);
  wire WAPO_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.GUGY);
  wire WOMY_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.XYME);
  wire WAFY_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.GYGY);
  wire XUDY_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.GOWO);
  wire GOTA_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.GYMA);
  wire EGAV_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.FAME);
  wire CEDY_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.DYDO);
  wire EBOJ_Q = FONO.tock(spr.WUTY, spr.BYVA, spr.FURO);

  wire DYBA = not(spr.BYVA);

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

  spr.DOSY = not(DUBU);
  spr.WUZO = not(GORO);
  spr.GAFY = not(GUKY);
  spr.XAHO = not(WACY);
  spr.EJAD = not(FEVE);
  spr.WUNU = not(WOHU);
  spr.WUPA = not(GAKE);
  spr.GAMY = not(FOKO);
  spr.DOKU = not(EFEV);
  spr.DYNA = not(DYWE);

  //----------
  // Sprite y comparator

  wire YCEB_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D1); // order is weird? does it matter?
  wire ZUCA_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D2);
  wire WONE_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D3);
  wire ZAXE_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D4);
  wire XAFU_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D5);
  wire YSES_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D6);
  wire ZECA_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D7);
  wire YDYV_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D0);

  wire XELE = not(YCEB_Q);
  wire YPON = not(ZUCA_Q);
  wire XUVO = not(WONE_Q);
  wire ZYSA = not(ZAXE_Q);
  wire YWEG = not(XAFU_Q);
  wire XABU = not(YSES_Q);
  wire YTUX = not(ZECA_Q);
  wire YFAP = not(YDYV_Q);

  wire YWOK = not(vram.COTA);

  wire XEGU_Q = XEGU.tock(YWOK, 0, YCEB_Q);
  wire YJEX_Q = YJEX.tock(YWOK, 0, ZUCA_Q);
  wire XYJU_Q = XYJU.tock(YWOK, 0, WONE_Q);
  wire YBOG_Q = YBOG.tock(YWOK, 0, ZAXE_Q);
  wire WYSO_Q = WYSO.tock(YWOK, 0, XAFU_Q);
  wire XOTE_Q = XOTE.tock(YWOK, 0, YSES_Q);
  wire YZAB_Q = YZAB.tock(YWOK, 0, ZECA_Q);
  wire XUSO_Q = XUSO.tock(YWOK, 0, YDYV_Q);

  wire FUGY = not(!XEGU_Q); // only 7 of these?
  wire GAVO = not(!YJEX_Q);
  wire WYGA = not(!XYJU_Q);
  wire WUNE = not(!YBOG_Q);
  wire GOTU = not(!WYSO_Q);
  wire GEGU = not(!XOTE_Q);
  wire XEHE = not(!YZAB_Q);

  wire EBOS = not(ppu.V0);
  wire DASA = not(ppu.V1);
  wire FUKY = not(ppu.V2);
  wire FUVE = not(ppu.V3);
  wire FEPU = not(ppu.V4);
  wire FOFA = not(ppu.V5);
  wire FEMO = not(ppu.V6);
  wire GUSU = not(ppu.V7);

  wire ERUC_S = add_s(EBOS, !XUSO_Q, ext.P10_B);
  wire ERUC_C = add_c(EBOS, !XUSO_Q, ext.P10_B);
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

  spr.DEGE = not(ERUC_S);
  spr.DABY = not(ENEF_S);
  spr.DABU = not(FECO_S);
  spr.GYSA = not(GYKY_S);
  wire GACE = not(GOPU_S);
  wire GUVU = not(FUWA_S);
  wire GYDA = not(GOJU_S);
  wire GEWY = not(WUHU_S);

  wire YZOS_Q = YZOS.q();
  wire WUKY = not(!YZOS_Q);
  wire WAGO = xor(WUKY, WENU);
  wire CYVU = xor(WUKY, CUCU);
  wire BORE = xor(WUKY, WENU);
  wire BUVY = xor(WUKY, CEGA);
  wire XUQU = not(!VONU_Q);

  wire BAXE = not(CYVU);
  wire ARAS = not(BORE);
  wire AGAG = not(BUVY);
  wire ABEM = not(XUQU);
  wire DYSO = not(ext.P10_B);

  wire FUFO = not(ppu.FF40_D2);
  wire GEJY = amux2(!XUSO_Q, FUFO, ppu.FF40_D2, WAGO);
  wire FAMU = not(GEJY);
  wire GOVU = or(GYKY_S, ppu.FF40_D2);
  wire WOTA = nand(GACE, GUVU, GYDA, GEWY, WUHU_C, GOVU);
  wire GESE = not(WOTA);
  spr.SPR_MATCH = GESE;

  if (spr.WUME) {
    mem.D0 = YFAP;
    mem.D1 = XELE;
    mem.D2 = YPON;
    mem.D3 = XUVO;
    mem.D4 = ZYSA;
    mem.D5 = YWEG;
    mem.D6 = XABU;
    mem.D7 = YTUX;
  }

  wire ABON = not(spr.TEXY);
  if (ABON) {
    vram.MA0 = ABEM;
    vram.MA1 = BAXE;
    vram.MA2 = ARAS;
    vram.MA3 = AGAG;
    vram.MA4 = FAMU;
    vram.MA5 = FUGY;
    vram.MA6 = GAVO;
    vram.MA7 = WYGA;
    vram.MA8 = WUNE;
    vram.MA9 = GOTU;
    vram.MA10 = GEGU;
    vram.MA11 = XEHE;
    vram.MA12 = DYSO;
  }

  TOBU.tock(win.TAVA, XYMU, TULY_Q);
  VONU.tock(win.TAVA, XYMU, !TOBU_Q);

  //----------

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

  wire WYXO = or(spr.DYTY, GEBU);
  wire XUJO = or(spr.DYTY, WOMU);
  wire GAPE = or(spr.DYTY, GUNA);
  wire GUVE = or(spr.DYTY, FOCO);
  wire CAHO = or(spr.DYTY, DEWY);
  wire CEMY = or(spr.DYTY, DEZO);
  wire CATO = or(spr.DYTY, DOGU);
  wire CADO = or(spr.DYTY, CUGU);
  wire CECU = or(spr.DYTY, CUPE);
  wire BYBY = or(spr.DYTY, CUVA);

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

  spr.CACU = not(GYFO); spr.BUZY = not(GYFO); spr.FUKE = not(GYFO);
  spr.ZAPE = not(WEKA); spr.WUSE = not(WEKA); spr.ZURU = not(WEKA);
  spr.FEFO = not(GYVO); spr.GECY = not(GYVO); spr.WABE = not(GYVO);
  spr.FEKA = not(GUSA); spr.XYHA = not(GUSA); spr.YFAG = not(GUSA);
  spr.CEXU = not(BUKA); spr.AKOL = not(BUKA); spr.BYMY = not(BUKA);
  spr.FUXU = not(DYHU); spr.ENOB = not(DYHU); spr.GENY = not(DYHU);
  spr.WEME = not(DECU); spr.WUFA = not(DECU); spr.FAKA = not(DECU);
  spr.CYLA = not(BEDE); spr.DYMO = not(BEDE); spr.BUCY = not(BEDE);
  spr.WOFO = not(DUKE); spr.WYLU = not(DUKE); spr.EWOT = not(DUKE);
  spr.ASYS = not(BUCO); spr.AHOF = not(BUCO); spr.BYVY = not(BUCO);

  DEZY.tock(clk.CLK1, rst.RESET_VIDEO, spr.DYTY);
  BESE.flip(CAKE,    spr.AZYB);
  CUXY.flip(!BESE_Q, spr.AZYB);
  BEGO.flip(!CUXY_Q, spr.AZYB);
  DYBE.flip(!BEGO_Q, spr.AZYB);
}