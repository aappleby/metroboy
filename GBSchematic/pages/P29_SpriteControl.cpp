#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P29_SpriteControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // Sprite y comparator

  /*p29.YDYV*/ c.p29.SPRITE_Y_LATCH0 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH0, b.OAM_B_D0);
  /*p29.YCEB*/ c.p29.SPRITE_Y_LATCH1 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH1, b.OAM_B_D1);
  /*p29.ZUCA*/ c.p29.SPRITE_Y_LATCH2 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH2, b.OAM_B_D2);
  /*p29.WONE*/ c.p29.SPRITE_Y_LATCH3 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH3, b.OAM_B_D3);
  /*p29.ZAXE*/ c.p29.SPRITE_Y_LATCH4 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH4, b.OAM_B_D4);
  /*p29.XAFU*/ c.p29.SPRITE_Y_LATCH5 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH5, b.OAM_B_D5);
  /*p29.YSES*/ c.p29.SPRITE_Y_LATCH6 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH6, b.OAM_B_D6);
  /*p29.ZECA*/ c.p29.SPRITE_Y_LATCH7 = latch_pos(b.p28.CLK3, b.p29.SPRITE_Y_LATCH7, b.OAM_B_D7);

  /*p29.YWOK*/ c.p29.YWOK = not(b.p25.COTA);

  /*p29.XUSO*/ c.p29.SPRITE_Y0 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y0, b.p29.SPRITE_Y_LATCH0);
  /*p29.XEGU*/ c.p29.SPRITE_Y1 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y1, b.p29.SPRITE_Y_LATCH1);
  /*p29.YJEX*/ c.p29.SPRITE_Y2 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y2, b.p29.SPRITE_Y_LATCH2);
  /*p29.XYJU*/ c.p29.SPRITE_Y3 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y3, b.p29.SPRITE_Y_LATCH3);
  /*p29.YBOG*/ c.p29.SPRITE_Y4 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y4, b.p29.SPRITE_Y_LATCH4);
  /*p29.WYSO*/ c.p29.SPRITE_Y5 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y5, b.p29.SPRITE_Y_LATCH5);
  /*p29.XOTE*/ c.p29.SPRITE_Y6 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y6, b.p29.SPRITE_Y_LATCH6);
  /*p29.YZAB*/ c.p29.SPRITE_Y7 = tock_pos(a.p29.YWOK, b.p29.YWOK, 0, b.p29.SPRITE_Y7, b.p29.SPRITE_Y_LATCH7);


  /*p29.EBOS*/ c.p29.V0n = not(b.p21.V0);
  /*p29.DASA*/ c.p29.V1n = not(b.p21.V1);
  /*p29.FUKY*/ c.p29.V2n = not(b.p21.V2);
  /*p29.FUVE*/ c.p29.V3n = not(b.p21.V3);
  /*p29.FEPU*/ c.p29.V4n = not(b.p21.V4);
  /*p29.FOFA*/ c.p29.V5n = not(b.p21.V5);
  /*p29.FEMO*/ c.p29.V6n = not(b.p21.V6);
  /*p29.GUSU*/ c.p29.V7n = not(b.p21.V7);

  c.p29.ERUC_S = add_c(b.p29.V0n, !b.p29.SPRITE_Y0, b.chip.P10_B);
  c.p29.ERUC_C = add_s(b.p29.V0n, !b.p29.SPRITE_Y0, b.chip.P10_B);
  c.p29.ENEF_S = add_s(b.p29.V1n, !b.p29.SPRITE_Y1, b.p29.ERUC_C);
  c.p29.ENEF_C = add_c(b.p29.V1n, !b.p29.SPRITE_Y1, b.p29.ERUC_C);
  c.p29.FECO_S = add_s(b.p29.V2n, !b.p29.SPRITE_Y2, b.p29.ENEF_C);
  c.p29.FECO_C = add_c(b.p29.V2n, !b.p29.SPRITE_Y2, b.p29.ENEF_C);
  c.p29.GYKY_S = add_s(b.p29.V3n, !b.p29.SPRITE_Y3, b.p29.FECO_C);
  c.p29.GYKY_C = add_c(b.p29.V3n, !b.p29.SPRITE_Y3, b.p29.FECO_C);
  c.p29.GOPU_S = add_s(b.p29.V4n, !b.p29.SPRITE_Y4, b.p29.GYKY_C);
  c.p29.GOPU_C = add_c(b.p29.V4n, !b.p29.SPRITE_Y4, b.p29.GYKY_C);
  c.p29.FUWA_S = add_s(b.p29.V5n, !b.p29.SPRITE_Y5, b.p29.GOPU_C);
  c.p29.FUWA_C = add_c(b.p29.V5n, !b.p29.SPRITE_Y5, b.p29.GOPU_C);
  c.p29.GOJU_S = add_s(b.p29.V6n, !b.p29.SPRITE_Y6, b.p29.FUWA_C);
  c.p29.GOJU_C = add_c(b.p29.V6n, !b.p29.SPRITE_Y6, b.p29.FUWA_C);
  c.p29.WUHU_S = add_s(b.p29.V7n, !b.p29.SPRITE_Y7, b.p29.GOJU_C);
  c.p29.WUHU_C = add_c(b.p29.V7n, !b.p29.SPRITE_Y7, b.p29.GOJU_C);

  c.p29.DEGE = not(b.p29.ERUC_S);
  c.p29.DABY = not(b.p29.ENEF_S);
  c.p29.DABU = not(b.p29.FECO_S);
  c.p29.GYSA = not(b.p29.GYKY_S);
  c.p29.GACE = not(b.p29.GOPU_S);
  c.p29.GUVU = not(b.p29.FUWA_S);
  c.p29.GYDA = not(b.p29.GOJU_S);
  c.p29.GEWY = not(b.p29.WUHU_S);

  c.p29.GOVU = or(b.p29.GYKY_S, b.p23.SPRITE_SIZE);
  c.p29.WOTA = nand(b.p29.GACE, b.p29.GUVU, b.p29.GYDA, b.p29.GEWY, b.p29.WUHU_C, b.p29.GOVU);
  c.p29.SPR_MATCH = not(b.p29.WOTA);

  /*p29.YFAP*/ if (b.p28.WUME) c.D0 = b.p29.SPRITE_Y_LATCH0;
  /*p29.XELE*/ if (b.p28.WUME) c.D1 = b.p29.SPRITE_Y_LATCH1;
  /*p29.YPON*/ if (b.p28.WUME) c.D2 = b.p29.SPRITE_Y_LATCH2;
  /*p29.XUVO*/ if (b.p28.WUME) c.D3 = b.p29.SPRITE_Y_LATCH3;
  /*p29.ZYSA*/ if (b.p28.WUME) c.D4 = b.p29.SPRITE_Y_LATCH4;
  /*p29.YWEG*/ if (b.p28.WUME) c.D5 = b.p29.SPRITE_Y_LATCH5;
  /*p29.XABU*/ if (b.p28.WUME) c.D6 = b.p29.SPRITE_Y_LATCH6;
  /*p29.YTUX*/ if (b.p28.WUME) c.D7 = b.p29.SPRITE_Y_LATCH7;

  /*p29.ABON*/ c.p29.ABON = not(b.p29.TEXY);
  /*p29.FUGY*/ if (b.p29.ABON) c.MA05 = b.p29.SPRITE_Y1;
  /*p29.GAVO*/ if (b.p29.ABON) c.MA06 = b.p29.SPRITE_Y2;
  /*p29.WYGA*/ if (b.p29.ABON) c.MA07 = b.p29.SPRITE_Y3;
  /*p29.WUNE*/ if (b.p29.ABON) c.MA08 = b.p29.SPRITE_Y4;
  /*p29.GOTU*/ if (b.p29.ABON) c.MA09 = b.p29.SPRITE_Y5;
  /*p29.GEGU*/ if (b.p29.ABON) c.MA10 = b.p29.SPRITE_Y6;
  /*p29.XEHE*/ if (b.p29.ABON) c.MA11 = b.p29.SPRITE_Y7;

  //----------
  // bottom
    
  c.p29.LINE_144n = not(b.p21.LINE_144);
  c.p29.ABOV = and(b.p21.LINE_DONEo, b.p29.LINE_144n);

  c.p29.WOSU = tock_pos(a.p21.CLK_AxCxExGxc, b.p21.CLK_AxCxExGxc, b.sys.VID_RESETn1, b.p29.WOSU, !b.p21.CLK_2Ma);
  c.p29.WOJO = nor(!b.p21.CLK_2Ma, !b.p29.WOSU);
  c.p29.XYSO = not(b.p29.WOJO);
  c.p29.XOCE = not(b.p29.WOSU);

  c.p29.CENO = tock_pos(a.p21.CLK_2Mb, b.p21.CLK_2Mb, b.sys.VID_RESETn3, b.p29.CENO, b.p28.BESU);
  c.p29.CEHA = not(!b.p29.CENO);
  c.p29.BUZA = and(!b.p29.CENO, b.p21.XYMU);
  c.p29.CARE = or(b.p29.XOCE, b.p29.CEHA, b.p29.SPR_MATCH);
  c.p29.DYTY = not(b.p29.CARE);
  c.p29.CATU = tock_pos(a.p21.CLK_2Mb, b.p21.CLK_2Mb, b.sys.VID_RESETn3, b.p29.CATU, b.p29.ABOV);
  c.p29.BALU = not(b.p28.SCAN_RSTn);
  c.p29.BAGY = not(b.p29.BALU);
  c.p29.BYBA = tock_pos(a.p21.CLK_2Mb,    b.p21.CLK_2Mb,    b.p29.BAGY, b.p29.BYBA, b.p28.SCAN_DONE);
  c.p29.DOBA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p29.BAGY, b.p29.DOBA, b.p29.BYBA);
  c.p29.BEBU = or(b.p29.DOBA, b.p29.BALU, !b.p29.BYBA);
  c.p29.AVAP = not(b.p29.BEBU);

  //----------
  // center thing

  c.p29.TOBU = tock_pos(a.sys.CLK_AxCxExGx10, b.sys.CLK_AxCxExGx10, b.p21.XYMU, b.p29.TOBU, b.p29.TULY);
  c.p29.VONU = tock_pos(a.sys.CLK_AxCxExGx10, b.sys.CLK_AxCxExGx10, b.p21.XYMU, b.p29.VONU, !b.p29.TOBU);
  c.p29.XUQU = not(!b.p29.VONU);

  c.p29.TEPA = not(b.p21.XYMU);

  c.p29.TYNO = nand(b.p29.TOXE, b.p29.SEBA, b.p29.VONU);
  c.p29.VUSA = or(!b.p29.TYFO, b.p29.TYNO);
  c.p29.WUTY = not(b.p29.VUSA);
  c.p29.XEFY = not(b.p29.WUTY);

  c.p29.SAKY = nor(b.p29.TULY, b.p29.VONU);
  c.p29.TYSO = or(b.p29.SAKY, b.p29.TEPA);
  c.p29.TEXY = not(b.p29.TYSO);
  c.p29.FLIP_X = and(!b.p31.BAXO, b.p29.TEXY);

  c.p29.TUVO = or(b.p29.TEPA, b.p29.TULY, b.p29.TESE);
  c.p29.TAME = nand(b.p29.TESE, b.p29.TOXE);
  c.p29.TOMA = nand(b.p21.CLK_xBxDxFxHd, b.p29.TAME);

  c.p29.TYTU = not(b.p29.TAME);
  c.p29.SYCU = nor(b.p29.TYTU, b.p29.TEPA, b.p29.TYFO);
  c.p29.TACU = nand(b.p29.TYTU, b.p29.TYFO);
  c.p29.TOPU = and(b.p29.TULY, b.p29.SYCU);
  c.p29.RACA = and(b.p29.VONU, b.p29.SYCU);
  c.p29.VYWA = not(b.p29.TOPU);
  c.p29.WENY = not(b.p29.VYWA);
  c.p29.XADO = not(b.p29.WENY);
  c.p29.PEBY = not(b.p29.RACA);
  c.p29.NYBE = not(b.p29.PEBY);
  c.p29.PUCO = not(b.p29.NYBE);

  c.p29.TYFO = tock_pos( a.p21.CLK_xBxDxFxHd,  b.p21.CLK_xBxDxFxHd, b.p27.P10_Bn, b.p29.TYFO, b.p29.TAME);
  c.p29.SEBA = tock_pos( a.p21.CLK_xBxDxFxHd,  b.p21.CLK_xBxDxFxHd, b.p21.XYMU, b.p29.SEBA, b.p29.VONU);
  c.p29.TOXE = tock_pos( a.p29.TOMA,  b.p29.TOMA, b.p27.SECA, b.p29.TOXE, !b.p29.TOXE);
  c.p29.TULY = tock_pos(!a.p29.TOXE, !b.p29.TOXE, b.p27.SECA, b.p29.TULY, !b.p29.TULY);
  c.p29.TESE = tock_pos(!a.p29.TULY, !b.p29.TULY, b.p27.SECA, b.p29.TESE, !b.p29.TESE);

  //----------
  // Sprite priority

  c.p29.BYJO = not(b.p29.CEHA);
  c.p29.AZEM = and(b.p29.BYJO, b.p21.XYMU);
  c.p29.AROR = and(b.p29.AZEM, b.p23.SPRITE_EN);

  c.p29.XAGE = nand(b.p29.AROR, b.p31.YNAZ, b.p31.YKOK);
  c.p29.YLOZ = nand(b.p29.AROR, b.p31.ZURE, b.p31.YWOS);
  c.p29.DEGO = nand(b.p29.AROR, b.p31.EKES, b.p31.CEHU);
  c.p29.DYDU = nand(b.p29.AROR, b.p31.EWAM, b.p31.CYVY);
  c.p29.YDUG = nand(b.p29.AROR, b.p31.ZAKO, b.p31.XEBA);
  c.p29.YGEM = nand(b.p29.AROR, b.p31.YLEV, b.p31.YTUB);
  c.p29.EFYL = nand(b.p29.AROR, b.p31.FEHA, b.p31.DAMA);
  c.p29.DYKA = nand(b.p29.AROR, b.p31.CYCO, b.p31.DAJE);
  c.p29.YBEZ = nand(b.p29.AROR, b.p31.YDOT, b.p31.YWAP);
  c.p29.EGOM = nand(b.p29.AROR, b.p31.FYMA, b.p31.COGY);

  c.p29.FEFY = nand(b.p29.XAGE, b.p29.YLOZ, b.p29.DEGO, b.p29.DYDU, b.p29.YDUG);
  c.p29.FOVE = nand(b.p29.YGEM, b.p29.EFYL, b.p29.DYKA, b.p29.YBEZ, b.p29.EGOM);
  c.p29.FEPO = or(b.p29.FEFY, b.p29.FOVE);

  c.p29.WEFU = not(b.p29.YDUG);
  c.p29.GEZE = or(b.p29.WEFU, b.chip.P10_B);
  c.p29.GUVA = nor(b.p29.YDUG, b.chip.P10_B);

  c.p29.GAJA = not(b.p29.DYDU);
  c.p29.FUMA = or(b.p29.GAJA, b.p29.GEZE);
  c.p29.ENUT = nor(b.p29.DYDU, b.p29.GEZE);

  c.p29.GUPO = not(b.p29.DEGO);
  c.p29.GEDE = or(b.p29.GUPO, b.p29.FUMA);
  c.p29.EMOL = nor(b.p29.DEGO, b.p29.FUMA);

  c.p29.WEBO = not(b.p29.YLOZ);
  c.p29.WUTO = or(b.p29.WEBO, b.p29.GEDE);
  c.p29.GYFY = nor(b.p29.YLOZ, b.p29.GEDE);

  c.p29.WUNA = not(b.p29.XAGE);
  c.p29.XYLA = or(b.p29.WUNA, b.p29.WUTO);
  c.p29.GONO = nor(b.p29.XAGE, b.p29.WUTO);

  c.p29.GABA = not(b.p29.EGOM);
  c.p29.WEJA = or(b.p29.GABA, b.p29.XYLA);
  c.p29.GEGA = nor(b.p29.EGOM, b.p29.XYLA);

  c.p29.WASE = not(b.p29.YBEZ);
  c.p29.WYLA = or(b.p29.WASE, b.p29.WEJA);
  c.p29.XOJA = nor(b.p29.YBEZ, b.p29.WEJA);

  c.p29.GYTE = not(b.p29.DYKA);
  c.p29.FAVO = or(b.p29.GYTE, b.p29.WYLA);
  c.p29.GUTU = nor(b.p29.DYKA, b.p29.WYLA);

  c.p29.GEKE = not(b.p29.EFYL);
  c.p29.GYGA = or(b.p29.GEKE, b.p29.FAVO);
  c.p29.FOXA = nor(b.p29.EFYL, b.p29.FAVO);

  c.p29.GUZE = nor(b.p29.YGEM, b.p29.GYGA);

  c.p29.FADO = not(b.p29.GUZE);
  c.p29.DENY = not(b.p29.FOXA);
  c.p29.GUGY = not(b.p29.GUTU);
  c.p29.XYME = not(b.p29.XOJA);
  c.p29.GYGY = not(b.p29.GEGA);
  c.p29.GOWO = not(b.p29.GONO);
  c.p29.GYMA = not(b.p29.GYFY);
  c.p29.FAME = not(b.p29.EMOL);
  c.p29.DYDO = not(b.p29.ENUT);
  c.p29.FURO = not(b.p29.GUVA);

  /*p28.ABAK*/ c.p28.ABAK = or(b.p28.ATEJ, b.sys.VID_RESET7);
  /*p28.BYVA*/ c.p28.BYVA = not(b.p28.ABAK);

  c.p29.FONO = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.FONO, b.p29.GUZE);
  c.p29.EXUQ = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.EXUQ, b.p29.FOXA);
  c.p29.WAPO = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.WAPO, b.p29.GUTU);
  c.p29.WOMY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.WOMY, b.p29.XOJA);
  c.p29.WAFY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.WAFY, b.p29.GEGA);
  c.p29.XUDY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.XUDY, b.p29.GONO);
  c.p29.GOTA = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.GOTA, b.p29.GYFY);
  c.p29.EGAV = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.EGAV, b.p29.EMOL);
  c.p29.CEDY = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.CEDY, b.p29.ENUT);
  c.p29.EBOJ = tock_pos(a.p29.WUTY, b.p29.WUTY, b.p28.BYVA, b.p29.EBOJ, b.p29.GUVA);

  c.p29.DYBA = not(b.p28.BYVA);

  c.p29.DUBU = or(b.p29.DYBA, b.p29.FONO);
  c.p29.GORO = or(b.p29.DYBA, b.p29.EXUQ);
  c.p29.GUKY = or(b.p29.DYBA, b.p29.WAPO);
  c.p29.WACY = or(b.p29.DYBA, b.p29.WOMY);
  c.p29.FEVE = or(b.p29.DYBA, b.p29.WAFY);
  c.p29.WOHU = or(b.p29.DYBA, b.p29.XUDY);
  c.p29.GAKE = or(b.p29.DYBA, b.p29.GOTA);
  c.p29.FOKO = or(b.p29.DYBA, b.p29.EGAV);
  c.p29.EFEV = or(b.p29.DYBA, b.p29.CEDY);
  c.p29.DYWE = or(b.p29.DYBA, b.p29.EBOJ);

  c.p29.DOSY = not(b.p29.DUBU);
  c.p29.WUZO = not(b.p29.GORO);
  c.p29.GAFY = not(b.p29.GUKY);
  c.p29.XAHO = not(b.p29.WACY);
  c.p29.EJAD = not(b.p29.FEVE);
  c.p29.WUNU = not(b.p29.WOHU);
  c.p29.WUPA = not(b.p29.GAKE);
  c.p29.GAMY = not(b.p29.FOKO);
  c.p29.DOKU = not(b.p29.EFEV);
  c.p29.DYNA = not(b.p29.DYWE);

  //----------
  // Y flip flag

  c.p29.FLIP_Y = not(b.p31.FLIP_Yn);
  c.p29.WAGO = xor(b.p29.FLIP_Y, b.p30.LINE_0);
  c.p29.CYVU = xor(b.p29.FLIP_Y, b.p30.LINE_1);
  c.p29.BORE = xor(b.p29.FLIP_Y, b.p30.LINE_2);
  c.p29.BUVY = xor(b.p29.FLIP_Y, b.p30.LINE_3);


  c.p29.SPRITE_SIZEn = not(b.p23.SPRITE_SIZE);
  c.p29.GEJY = amux2(!b.p29.SPRITE_Y0, b.p29.SPRITE_SIZEn, b.p23.SPRITE_SIZE, b.p29.WAGO);
  c.p29.FAMU = not(b.p29.GEJY);

  if (b.p29.ABON) c.MA01 = b.p29.CYVU;
  if (b.p29.ABON) c.MA02 = b.p29.BORE;
  if (b.p29.ABON) c.MA03 = b.p29.BUVY;
  if (b.p29.ABON) c.MA00 = b.p29.XUQU;
  if (b.p29.ABON) c.MA12 = b.chip.P10_B;

  //----------
  // Decoder, right side

  c.p29.BAKY = and(b.p29.CUXY, b.p29.DYBE);
  c.p29.CAKE = or(b.p29.BAKY, b.p29.DEZY);
  c.p29.EDEN = not(b.p29.BESE);
  c.p29.FYCU = not(b.p29.EDEN);
  c.p29.CYPY = not(b.p29.CUXY);
  c.p29.FONE = not(b.p29.CYPY);
  c.p29.CAPE = not(b.p29.BEGO);
  c.p29.EKUD = not(b.p29.CAPE);
  c.p29.CAXU = not(b.p29.DYBE);
  c.p29.ELYG = not(b.p29.CAXU);

  // CAXU ELYG CAPE EKUD CYPY FYCU EDEN FONE

  c.p29.GEBU = nand(b.p29.EDEN, b.p29.FONE, b.p29.CAPE, b.p29.CAXU);
  c.p29.WOMU = nand(b.p29.EDEN, b.p29.FONE, b.p29.EKUD, b.p29.CAXU);
  c.p29.GUNA = nand(b.p29.FYCU, b.p29.FONE, b.p29.EKUD, b.p29.CAXU);
  c.p29.FOCO = nand(b.p29.FYCU, b.p29.FONE, b.p29.CAPE, b.p29.CAXU);
  c.p29.DEWY = nand(b.p29.EDEN, b.p29.CYPY, b.p29.CAPE, b.p29.ELYG);
  c.p29.DEZO = nand(b.p29.EDEN, b.p29.CYPY, b.p29.CAPE, b.p29.CAXU);
  c.p29.DOGU = nand(b.p29.FYCU, b.p29.CYPY, b.p29.CAPE, b.p29.ELYG);
  c.p29.CUGU = nand(b.p29.FYCU, b.p29.CYPY, b.p29.EKUD, b.p29.CAPE); // this one is weird... schematic probably wrong, these all decode to numbers...
  c.p29.CUPE = nand(b.p29.EDEN, b.p29.CYPY, b.p29.EKUD, b.p29.CAXU);
  c.p29.CUVA = nand(b.p29.FYCU, b.p29.CYPY, b.p29.CAPE, b.p29.CAXU); // also bit weird? schematic says 0?01 but it seems to be a normal decode...

  c.p29.WYXO = or(b.p29.DYTY, b.p29.GEBU);
  c.p29.XUJO = or(b.p29.DYTY, b.p29.WOMU);
  c.p29.GAPE = or(b.p29.DYTY, b.p29.GUNA);
  c.p29.GUVE = or(b.p29.DYTY, b.p29.FOCO);
  c.p29.CAHO = or(b.p29.DYTY, b.p29.DEWY);
  c.p29.CEMY = or(b.p29.DYTY, b.p29.DEZO);
  c.p29.CATO = or(b.p29.DYTY, b.p29.DOGU);
  c.p29.CADO = or(b.p29.DYTY, b.p29.CUGU);
  c.p29.CECU = or(b.p29.DYTY, b.p29.CUPE);
  c.p29.BYBY = or(b.p29.DYTY, b.p29.CUVA);

  // why so many signals?
  c.p29.GYFO = not(b.p29.WYXO);
  c.p29.WEKA = not(b.p29.XUJO);
  c.p29.GYVO = not(b.p29.GAPE);
  c.p29.GUSA = not(b.p29.GUVE);
  c.p29.BUKA = not(b.p29.CAHO);
  c.p29.DYHU = not(b.p29.CEMY);
  c.p29.DECU = not(b.p29.CATO);
  c.p29.BEDE = not(b.p29.CADO);
  c.p29.DUKE = not(b.p29.CECU);
  c.p29.BUCO = not(b.p29.BYBY);

  c.p29.CACU = not(b.p29.GYFO);
  c.p29.ZAPE = not(b.p29.WEKA);
  c.p29.FEFO = not(b.p29.GYVO);
  c.p29.FEKA = not(b.p29.GUSA);
  c.p29.CEXU = not(b.p29.BUKA);
  c.p29.FUXU = not(b.p29.DYHU);
  c.p29.WEME = not(b.p29.DECU);
  c.p29.CYLA = not(b.p29.BEDE);
  c.p29.WOFO = not(b.p29.DUKE);
  c.p29.ASYS = not(b.p29.BUCO);

  c.p29.BUZY = not(b.p29.GYFO);
  c.p29.WUSE = not(b.p29.WEKA);
  c.p29.GECY = not(b.p29.GYVO);
  c.p29.XYHA = not(b.p29.GUSA);
  c.p29.AKOL = not(b.p29.BUKA);
  c.p29.ENOB = not(b.p29.DYHU);
  c.p29.WUFA = not(b.p29.DECU);
  c.p29.DYMO = not(b.p29.BEDE);
  c.p29.WYLU = not(b.p29.DUKE);
  c.p29.AHOF = not(b.p29.BUCO);

  c.p29.FUKE = not(b.p29.GYFO);
  c.p29.ZURU = not(b.p29.WEKA);
  c.p29.WABE = not(b.p29.GYVO);
  c.p29.YFAG = not(b.p29.GUSA);
  c.p29.BYMY = not(b.p29.BUKA);
  c.p29.GENY = not(b.p29.DYHU);
  c.p29.FAKA = not(b.p29.DECU);
  c.p29.BUCY = not(b.p29.BEDE);
  c.p29.EWOT = not(b.p29.DUKE);
  c.p29.BYVY = not(b.p29.BUCO);

  c.p29.DEZY = tock_pos(a.sys.CLK_xBxDxFxH2, b.sys.CLK_xBxDxFxH2, b.sys.VID_RESETn1, b.p29.DEZY, b.p29.DYTY);
  c.p29.BESE = tock_pos(a.p29.CAKE,    b.p29.CAKE,    b.p28.AZYB,        b.p29.BESE, !b.p29.BESE);
  c.p29.CUXY = tock_pos(!a.p29.BESE,   !b.p29.BESE,   b.p28.AZYB,        b.p29.CUXY, !b.p29.CUXY);
  c.p29.BEGO = tock_pos(!a.p29.CUXY,   !b.p29.CUXY,   b.p28.AZYB,        b.p29.BEGO, !b.p29.BEGO);
  c.p29.DYBE = tock_pos(!a.p29.BEGO,   !b.p29.BEGO,   b.p28.AZYB,        b.p29.DYBE, !b.p29.DYBE);
}