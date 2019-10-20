#include "Sprites.h"

#include "AddressDecoder.h"
#include "Clocks.h"
#include "CpuBus.h"
#include "DmaBus.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "OAM.h"
#include "PPU.h"
#include "System.h"
#include "VramBus.h"
#include "Window.h"

Sprites spr;

//-----------------------------------------------------------------------------

void Sprites::tick(OAM& oam, const Resets& rst) {
  //----------
  // sprite scan counter

  wire YFEL_Q = YFEL.q();
  wire WEWY_Q = WEWY.q();
  wire GOSO_Q = GOSO.q();
  wire ELYN_Q = ELYN.q();
  wire FAHA_Q = FAHA.q();
  wire FONY_Q = FONY.q();

  FETO = and(YFEL_Q, WEWY_Q, FONY_Q, GOSO_Q);
  wire GAVA = or(FETO, XUPY);

  YFEL.flip(GAVA,    ANOM);
  WEWY.flip(!YFEL_Q, ANOM);
  GOSO.flip(!WEWY_Q, ANOM);
  ELYN.flip(!GOSO_Q, ANOM);
  FAHA.flip(!ELYN_Q, ANOM);
  FONY.flip(!FAHA_Q, ANOM);

  //----------

  wire CATU_Q = CATU.q();
  wire ANEL_Q = ANEL.q();
  wire MATU_Q = dma.MATU.q();

  WEFE = not(ext.P10_B);
  wire YVAL = not(clk.CLK3);
  wire YRYV = not(YVAL);
  wire ZODO = not(YRYV);

  wire AWOH = not(XUPY);
  wire ABAF = not(CATU_Q);

  wire BYHA = unk3(ANEL_Q, ABAF, ABEZ);
  ATEJ = not(BYHA);
  ANOM = nor(rst.RESET_VIDEO2n, ATEJ);
  AZYB = not(ATEJ);
  wire AMYG = not(rst.RESET_VIDEO);
  wire ABAK = or(ATEJ, AMYG);
  BYVA = not(ABAK);

  wire ASEN = or(rst.RESET_VIDEO2n, AVAP);
  wire BOGE = not(MATU_Q);
  BESU = unk2(CATU_Q, ASEN);
  wire AJON = and(ppu.XYMU, BOGE);
  ACYL = and(BOGE, BESU);
  wire BETE = not(AJON);
  wire APAR = not(ACYL);
  wire AJUJ = nor(MATU_Q, ACYL, AJON);
  wire ASAM = or(ACYL, ppu.XYMU, MATU_Q);

  wire XYNY = not(dma.MOPA_PHI);
  wire XUTO = and(sys.SARO, cpu.CPU_WR2);
  AMAB = and(sys.SARO, AJUJ);
  wire ADAH = not(sys.SARO);

  wire WUJE = unk2(XYNY, XUTO);
  wire XUPA = not(WUJE);

  wire AJEP = and(ACYL, XOCE);
  wire WEFY = and(TUVO, !TYFO.q());
  wire XUJA = not(WEFY);
  wire BOFE = not(dma.CATY);
  wire BOTA = nor(BOFE, sys.SARO, cpu.CPU_RD2);
  wire ASYT = and(AJEP, XUJA, BOTA);
  wire BODE = not(ASYT);
  clk.CLK3 = BODE;

  wire WARU = and(dec.FF40, cpu.CPU_WR2);
  wire XUCA = not(WARU);
  wire APAG = amux2(XUPA, AMAB, AJUJ, ADAH);

  oam.OAM_CLK = ZODO;
  oam.OAM_ADDR_RENDER = BETE;
  oam.OAM_ADDR_PARSE = APAR;
  oam.OAM_ADDR_CPU = ASAM;

  wire AZUL = not(APAG);
  if (AZUL) {
    wire ZAXA = not(mem.D0);
    wire ZAMY = not(mem.D0);
    wire ZAKY = not(mem.D1);
    wire ZOPU = not(mem.D1);
    wire WULE = not(mem.D2);
    wire WYKY = not(mem.D2);
    wire ZOZO = not(mem.D3);
    wire ZAJA = not(mem.D3);

    wire ZUFO = not(mem.D4);
    wire ZUGA = not(mem.D4);
    wire ZATO = not(mem.D5);
    wire ZUMO = not(mem.D5);
    wire YVUC = not(mem.D6);
    wire XYTO = not(mem.D6);
    wire ZUFE = not(mem.D7);
    wire ZYFA = not(mem.D7);

    oam.OAM_A_D0 = ZAXA;
    oam.OAM_B_D0 = ZAMY;
    oam.OAM_A_D1 = ZAKY;
    oam.OAM_B_D1 = ZOPU;
    oam.OAM_A_D2 = WULE;
    oam.OAM_B_D2 = WYKY;
    oam.OAM_A_D3 = ZOZO;
    oam.OAM_B_D3 = ZAJA;

    oam.OAM_A_D4 = ZUFO;
    oam.OAM_B_D4 = ZUGA;
    oam.OAM_A_D5 = ZATO;
    oam.OAM_B_D5 = ZUMO;
    oam.OAM_A_D6 = YVUC;
    oam.OAM_B_D6 = XYTO;
    oam.OAM_A_D7 = ZUFE;
    oam.OAM_B_D7 = ZYFA;
  }

  wire AZAR = not(dma.VRAM_TO_OAM);
  if (AZAR) {
    wire WUZU = not(vram.MD0);
    wire WOWA = not(vram.MD0);
    wire AXER = not(vram.MD1);
    wire AVEB = not(vram.MD1);
    wire ASOX = not(vram.MD2);
    wire AMUH = not(vram.MD2);
    wire CETU = not(vram.MD3);
    wire COFO = not(vram.MD3);

    wire ARYN = not(vram.MD4);
    wire AZOZ = not(vram.MD4);
    wire ACOT = not(vram.MD5);
    wire AGYK = not(vram.MD5);
    wire CUJE = not(vram.MD6);
    wire BUSE = not(vram.MD6);
    wire ATER = not(vram.MD7);
    wire ANUM = not(vram.MD7);

    oam.OAM_A_D0 = WUZU;
    oam.OAM_B_D0 = WOWA;
    oam.OAM_A_D1 = AXER;
    oam.OAM_B_D1 = AVEB;
    oam.OAM_A_D2 = ASOX;
    oam.OAM_B_D2 = AMUH;
    oam.OAM_A_D3 = CETU;
    oam.OAM_B_D3 = COFO;

    oam.OAM_A_D4 = ARYN;
    oam.OAM_B_D4 = AZOZ;
    oam.OAM_A_D5 = ACOT;
    oam.OAM_B_D5 = AGYK;
    oam.OAM_A_D6 = CUJE;
    oam.OAM_B_D6 = BUSE;
    oam.OAM_A_D7 = ATER;
    oam.OAM_B_D7 = ANUM;
  }

  //----------

  wire FOBY = not(mem.A7);
  wire WAXA = not(mem.A6);
  wire GERA = not(mem.A5);
  wire FEVU = not(mem.A4);
  wire WAPE = not(mem.A3);
  wire GOSE = not(mem.A2);
  wire WACU = not(mem.A1);
  wire GARO = not(mem.A0);

  wire FYKE = not(WEZA);
  wire FUGU = not(WUCO);
  wire FACO = not(WYDA);
  wire FABY = not(ZYSU);
  wire GYKA = not(WYSE);
  wire GYBU = not(WUZY);
  wire WYDU = not(WEFE);
  wire GECA = not(WEFE);

  wire GOBY = not(FONY_Q);
  wire GAMA = not(FAHA_Q);
  wire FAKU = not(ELYN_Q);
  wire FUTO = not(GOSO_Q);
  wire GEMA = not(WEWY_Q);
  wire GUSE = not(YFEL_Q);
  wire WUWE = not(ext.P10_B);
  wire GEFY = not(ext.P10_B);

  wire FETU = not(dma.DMA_A7);
  wire FYDU = not(dma.DMA_A6);
  wire EDOL = not(dma.DMA_A5);
  wire ELUG = not(dma.DMA_A4);
  wire FYKY = not(dma.DMA_A3);
  wire FAGO = not(dma.DMA_A2);
  wire FESA = not(dma.DMA_A1);
  wire FODO = not(dma.DMA_A0);

  wire YZET = not((FOBY & oam.OAM_ADDR_CPU) | (FYKE & oam.OAM_ADDR_RENDER) | (GOBY & oam.OAM_ADDR_PARSE) | (FETU & oam.OAM_ADDR_DMA));
  wire XEMU = not((WAXA & oam.OAM_ADDR_CPU) | (FUGU & oam.OAM_ADDR_RENDER) | (GAMA & oam.OAM_ADDR_PARSE) | (FYDU & oam.OAM_ADDR_DMA));
  wire YMEV = not((GERA & oam.OAM_ADDR_CPU) | (FACO & oam.OAM_ADDR_RENDER) | (FAKU & oam.OAM_ADDR_PARSE) | (EDOL & oam.OAM_ADDR_DMA));
  wire YVOM = not((FEVU & oam.OAM_ADDR_CPU) | (FABY & oam.OAM_ADDR_RENDER) | (FUTO & oam.OAM_ADDR_PARSE) | (ELUG & oam.OAM_ADDR_DMA));
  wire YFOC = not((WAPE & oam.OAM_ADDR_CPU) | (GYKA & oam.OAM_ADDR_RENDER) | (GEMA & oam.OAM_ADDR_PARSE) | (FYKY & oam.OAM_ADDR_DMA));
  wire YFOT = not((GOSE & oam.OAM_ADDR_CPU) | (GYBU & oam.OAM_ADDR_RENDER) | (GUSE & oam.OAM_ADDR_PARSE) | (FAGO & oam.OAM_ADDR_DMA));
  wire ZYFO = not((WACU & oam.OAM_ADDR_CPU) | (WYDU & oam.OAM_ADDR_RENDER) | (WUWE & oam.OAM_ADDR_PARSE) | (FESA & oam.OAM_ADDR_DMA));
  wire GEKA = not((GARO & oam.OAM_ADDR_CPU) | (GECA & oam.OAM_ADDR_RENDER) | (GEFY & oam.OAM_ADDR_PARSE) | (FODO & oam.OAM_ADDR_DMA));

  wire MYNU = nand(cpu.CPU_RD2, dma.CATY);
  LEKO = not(MYNU);
  wire WAFO = not(GEKA);
  wire GUKO = and(WAFO, AMAB, LEKO);
  wire WUKU = and(LEKO, AMAB, GEKA);
  wire YLYC = and(dma.WYJA, GEKA);
  wire YNYC = and(dma.WYJA, WAFO);

  WUME = not(GUKO);
  WEWU = not(WUKU);
  wire ZONE = not(YLYC);
  wire ZOFE = not(YNYC);

  ANEL.tock(AWOH, ABEZ, CATU_Q);
  wire XECY_Q = XECY.tock(XUCA, 0, rst.RESET7n); // ? weird
  XUVA.tock(XYNY, rst.XARE, XECY_Q);

  oam.OAM_A_CS = ZONE;
  oam.OAM_B_CS = ZOFE;
  oam.OAM_A7 = YZET;
  oam.OAM_A6 = XEMU;
  oam.OAM_A5 = YMEV;
  oam.OAM_A4 = YVOM;
  oam.OAM_A3 = YFOC;
  oam.OAM_A2 = YFOT;
  oam.OAM_A1 = ZYFO;
}

//-----------------------------------------------------------------------------

void Sprites::tick_control(const OAM & oam, const Resets& rst) {

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
  wire ALES = not(ppu.XYVO);
  wire ABOV = and(ppu.SELA, ALES);
  spr.XUPY = not(!WUVU_Q);
  spr.ABEZ = not(rst.RESET_VIDEO2n);
  wire BALU = not(spr.ANOM);
  wire BAGY = not(BALU);
  wire WOJO = nor(!WUVU_Q, !WOSU_Q);
  spr.XOCE = not(WOSU_Q);
  spr.XYSO = not(WOJO);
  spr.CEHA = not(!CENO_Q);
  spr.BUZA = and(!CENO_Q, ppu.XYMU);
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
  wire TEPA = not(ppu.XYMU);

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
  SEBA.tock(spr.LAPE, ppu.XYMU, VONU_Q);
  TOXE.flip(TOMA,    win.SECA);
  TULY.flip(!TOXE_Q, win.SECA);
  TESE.flip(!TULY_Q, win.SECA);

  //----------
  // Sprite priority

  wire BYJO = not(spr.CEHA);
  wire AZEM = and(BYJO, ppu.XYMU);
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

  TOBU.tock(win.TAVA, ppu.XYMU, TULY_Q);
  VONU.tock(win.TAVA, ppu.XYMU, !TOBU_Q);

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

//-----------------------------------------------------------------------------

void Sprites::tick_matcher(const OAM& oam) {
  bool WYNO_Q = WYNO.latch(clk.CLK3, oam.OAM_A_D4);
  bool CYRA_Q = CYRA.latch(clk.CLK3, oam.OAM_A_D5);
  bool ZUVE_Q = ZUVE.latch(clk.CLK3, oam.OAM_A_D6);
  bool ECED_Q = ECED.latch(clk.CLK3, oam.OAM_A_D7);
  bool XYKY_Q = XYKY.latch(clk.CLK3, oam.OAM_A_D0);
  bool YRUM_Q = YRUM.latch(clk.CLK3, oam.OAM_A_D1);
  bool YSEX_Q = YSEX.latch(clk.CLK3, oam.OAM_A_D2);
  bool YVEL_Q = YVEL.latch(clk.CLK3, oam.OAM_A_D3);

  if (spr.WEWU) {
    mem.D0 = XYKY_Q;
    mem.D1 = YRUM_Q;
    mem.D2 = YSEX_Q;
    mem.D3 = YVEL_Q;
    mem.D4 = WYNO_Q;
    mem.D5 = CYRA_Q;
    mem.D6 = ZUVE_Q;
    mem.D7 = ECED_Q;
  }

  bool XEGA = not(vram.COTA);
  bool GOMO_Q = GOMO.tock(XEGA, 0, WYNO_Q);
  bool BAXO_Q = BAXO.tock(XEGA, 0, CYRA_Q);
  bool YZOS_Q = YZOS.tock(XEGA, 0, ZUVE_Q);
  bool DEPO_Q = DEPO.tock(XEGA, 0, ECED_Q);
  bool YLOR_Q = YLOR.tock(XEGA, 0, XYKY_Q);
  bool ZYTY_Q = ZYTY.tock(XEGA, 0, YRUM_Q);
  bool ZYVE_Q = ZYVE.tock(XEGA, 0, YSEX_Q);
  bool ZEZY_Q = ZEZY.tock(XEGA, 0, YVEL_Q);

  bool COSE = not(!GOMO_Q);
  bool AROP = not(!BAXO_Q);
  bool XATU = not(!YZOS_Q);
  bool BADY = not(!DEPO_Q);
  bool ZAGO = not(!YLOR_Q);
  bool ZOCY = not(!ZYTY_Q);
  bool YPUR = not(!ZYVE_Q);
  bool YVOK = not(!ZEZY_Q);

  // CHECK CLK/RESET WIRES

  // matcher 1
  bool WELO_Q = WELO.tock(spr.FUXU, spr.DYNA, COSE);
  bool XUNY_Q = XUNY.tock(spr.FUXU, spr.DYNA, AROP);
  bool WOTE_Q = WOTE.tock(spr.FUXU, spr.DYNA, XATU);
  bool XAKO_Q = XAKO.tock(spr.FUXU, spr.DYNA, BADY);
  bool XEPE_Q = XEPE.tock(spr.FUXU, spr.DYNA, ZAGO);
  bool YLAH_Q = YLAH.tock(spr.FUXU, spr.DYNA, ZOCY);
  bool ZOLA_Q = ZOLA.tock(spr.FUXU, spr.DYNA, YPUR);
  bool ZULU_Q = ZULU.tock(spr.FUXU, spr.DYNA, YVOK);

  bool WOJU = xor(WELO_Q, ppu.APUX);
  bool YFUN = xor(XUNY_Q, ppu.ABEF);
  bool WYZA = xor(WOTE_Q, ppu.ADAZ);
  bool YPUK = xor(XAKO_Q, ppu.ASAH);
  bool ZOGY = xor(XEPE_Q, ppu.ACAM);
  bool ZEBA = xor(YLAH_Q, ppu.AZUB);
  bool ZAHA = xor(ZOLA_Q, ppu.AMEL);
  bool ZOKY = xor(ZULU_Q, ppu.AHAL);
  XEBA = nor(WOJU, YFUN, WYZA, YPUK);
  ZAKO = nor(ZOGY, ZEBA, ZAHA, ZOKY);

  // matcher 2
  bool XOMY_Q = XOMY.tock(spr.YFAG, spr.WUPA, COSE);
  bool WUHA_Q = WUHA.tock(spr.YFAG, spr.WUPA, AROP);
  bool WYNA_Q = WYNA.tock(spr.YFAG, spr.WUPA, XATU);
  bool WECO_Q = WECO.tock(spr.YFAG, spr.WUPA, BADY);
  bool XOLY_Q = XOLY.tock(spr.YFAG, spr.WUPA, ZAGO);
  bool XYBA_Q = XYBA.tock(spr.YFAG, spr.WUPA, ZOCY);
  bool XABE_Q = XABE.tock(spr.YFAG, spr.WUPA, YPUR);
  bool XEKA_Q = XEKA.tock(spr.YFAG, spr.WUPA, YVOK);

  bool YVAP = xor(XOMY_Q, ppu.APUX);
  bool XENY = xor(WUHA_Q, ppu.ABEF);
  bool XAVU = xor(WYNA_Q, ppu.ADAZ);
  bool XEVA = xor(WECO_Q, ppu.ASAH);
  bool YHOK = xor(XOLY_Q, ppu.ACAM);
  bool YCAH = xor(XYBA_Q, ppu.AZUB);
  bool YDAJ = xor(XABE_Q, ppu.AMEL);
  bool YVUZ = xor(XEKA_Q, ppu.AHAL);
  YWOS = nor(YVAP, XENY, XAVU, XEVA);
  ZURE = nor(YHOK, YCAH, YDAJ, YVUZ);

  // matcher 3
  bool FAZU_Q = FAZU.tock(spr.GECY, spr.GAFY, COSE);
  bool FAXE_Q = FAXE.tock(spr.GECY, spr.GAFY, AROP);
  bool EXUK_Q = EXUK.tock(spr.GECY, spr.GAFY, XATU);
  bool FEDE_Q = FEDE.tock(spr.GECY, spr.GAFY, BADY);
  bool ERAZ_Q = ERAZ.tock(spr.GECY, spr.GAFY, ZAGO);
  bool EPUM_Q = EPUM.tock(spr.GECY, spr.GAFY, ZOCY);
  bool EROL_Q = EROL.tock(spr.GECY, spr.GAFY, YPUR);
  bool EHYN_Q = EHYN.tock(spr.GECY, spr.GAFY, YVOK);

  bool EJOT = xor(FAZU_Q, ppu.APUX);
  bool ESAJ = xor(FAXE_Q, ppu.ABEF);
  bool DUCU = xor(EXUK_Q, ppu.ADAZ);
  bool EWUD = xor(FEDE_Q, ppu.ASAH);
  bool DUSE = xor(ERAZ_Q, ppu.ACAM);
  bool DAGU = xor(EPUM_Q, ppu.AZUB);
  bool DYZE = xor(EROL_Q, ppu.AMEL);
  bool DESO = xor(EHYN_Q, ppu.AHAL);
  DAJE = nor(EJOT, ESAJ, DUCU, EWUD);
  CYCO = nor(DUSE, DAGU, DYZE, DESO);

  // matcher 4
  bool DAKE_Q = DAKE.tock(spr.ASYS, spr.DOKU, COSE);
  bool CESO_Q = CESO.tock(spr.ASYS, spr.DOKU, AROP);
  bool DYFU_Q = DYFU.tock(spr.ASYS, spr.DOKU, XATU);
  bool CUSY_Q = CUSY.tock(spr.ASYS, spr.DOKU, BADY);
  bool DANY_Q = DANY.tock(spr.ASYS, spr.DOKU, ZAGO);
  bool DUKO_Q = DUKO.tock(spr.ASYS, spr.DOKU, ZOCY);
  bool DESU_Q = DESU.tock(spr.ASYS, spr.DOKU, YPUR);
  bool DAZO_Q = DAZO.tock(spr.ASYS, spr.DOKU, YVOK);

  bool COLA = xor(DAKE_Q, ppu.APUX);
  bool BOBA = xor(CESO_Q, ppu.ABEF);
  bool COLU = xor(DYFU_Q, ppu.ADAZ);
  bool BAHU = xor(CUSY_Q, ppu.ASAH);
  bool EDYM = xor(DANY_Q, ppu.ACAM);
  bool EMYB = xor(DUKO_Q, ppu.AZUB);
  bool EBEF = xor(DESU_Q, ppu.AMEL);
  bool EWOK = xor(DAZO_Q, ppu.AHAL);
  CYVY = nor(COLA, BOBA, COLU, BAHU);
  EWAM = nor(EDYM, EMYB, EBEF, EWOK);

  // matcher 5
  bool ZOLY_Q = ZOLY.tock(spr.ZAPE, spr.XAHO, COSE);
  bool ZOGO_Q = ZOGO.tock(spr.ZAPE, spr.XAHO, AROP);
  bool ZECU_Q = ZECU.tock(spr.ZAPE, spr.XAHO, XATU);
  bool ZESA_Q = ZESA.tock(spr.ZAPE, spr.XAHO, BADY);
  bool YCOL_Q = YCOL.tock(spr.ZAPE, spr.XAHO, ZAGO);
  bool YRAC_Q = YRAC.tock(spr.ZAPE, spr.XAHO, ZOCY);
  bool YMEM_Q = YMEM.tock(spr.ZAPE, spr.XAHO, YPUR);
  bool YVAG_Q = YVAG.tock(spr.ZAPE, spr.XAHO, YVOK);

  bool ZARE = xor(ZOLY_Q, ppu.APUX);
  bool ZEMU = xor(ZOGO_Q, ppu.ABEF);
  bool ZYGO = xor(ZECU_Q, ppu.ADAZ);
  bool ZUZY = xor(ZESA_Q, ppu.ASAH);
  bool XOSU = xor(YCOL_Q, ppu.ACAM);
  bool ZUVU = xor(YRAC_Q, ppu.AZUB);
  bool XUCO = xor(YMEM_Q, ppu.AMEL);
  bool ZULO = xor(YVAG_Q, ppu.AHAL);
  YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
  YDOT = nor(XOSU, ZUVU, XUCO, ZULO);

  // matcher 6
  bool YBED_Q = YBED.tock(spr.WUNU, spr.WOFO, COSE);
  bool ZALA_Q = ZALA.tock(spr.WUNU, spr.WOFO, AROP);
  bool WYDE_Q = WYDE.tock(spr.WUNU, spr.WOFO, XATU);
  bool XEPA_Q = XEPA.tock(spr.WUNU, spr.WOFO, BADY);
  bool WEDU_Q = WEDU.tock(spr.WUNU, spr.WOFO, ZAGO);
  bool YGAJ_Q = YGAJ.tock(spr.WUNU, spr.WOFO, ZOCY);
  bool ZYJO_Q = ZYJO.tock(spr.WUNU, spr.WOFO, YPUR);
  bool XURY_Q = XURY.tock(spr.WUNU, spr.WOFO, YVOK);

  bool ZYKU = xor(YBED_Q, ppu.APUX);
  bool ZYPU = xor(ZALA_Q, ppu.ABEF);
  bool XAHA = xor(WYDE_Q, ppu.ADAZ);
  bool ZEFE = xor(XEPA_Q, ppu.ASAH);
  bool XEJU = xor(WEDU_Q, ppu.ACAM);
  bool ZATE = xor(YGAJ_Q, ppu.AZUB);
  bool ZAKU = xor(ZYJO_Q, ppu.AMEL);
  bool YBOX = xor(XURY_Q, ppu.AHAL);
  YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
  YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);

  // matcher 7
  bool EZUF_Q = EZUF.tock(spr.CEXU, spr.WUZO, COSE);
  bool ENAD_Q = ENAD.tock(spr.CEXU, spr.WUZO, AROP);
  bool EBOW_Q = EBOW.tock(spr.CEXU, spr.WUZO, XATU);
  bool FYCA_Q = FYCA.tock(spr.CEXU, spr.WUZO, BADY);
  bool GAVY_Q = GAVY.tock(spr.CEXU, spr.WUZO, ZAGO);
  bool GYPU_Q = GYPU.tock(spr.CEXU, spr.WUZO, ZOCY);
  bool GADY_Q = GADY.tock(spr.CEXU, spr.WUZO, YPUR);
  bool GAZA_Q = GAZA.tock(spr.CEXU, spr.WUZO, YVOK);

  bool DUZE = xor(EZUF_Q, ppu.APUX);
  bool DAGA = xor(ENAD_Q, ppu.ABEF);
  bool DAWU = xor(EBOW_Q, ppu.ADAZ);
  bool EJAW = xor(FYCA_Q, ppu.ASAH);
  bool GOHO = xor(GAVY_Q, ppu.ACAM);
  bool GASU = xor(GYPU_Q, ppu.AZUB);
  bool GABU = xor(GADY_Q, ppu.AMEL);
  bool GAFE = xor(GAZA_Q, ppu.AHAL);
  DAMA = nor(DUZE, DAGA, DAWU, EJAW);
  FEHA = nor(GOHO, GASU, GABU, GAFE);

  // matcher 8
  bool YPOD_Q = YPOD.tock(spr.WEME, spr.DOSY, COSE);
  bool YROP_Q = YROP.tock(spr.WEME, spr.DOSY, AROP);
  bool YNEP_Q = YNEP.tock(spr.WEME, spr.DOSY, XATU);
  bool YZOF_Q = YZOF.tock(spr.WEME, spr.DOSY, BADY);
  bool XUVY_Q = XUVY.tock(spr.WEME, spr.DOSY, ZAGO);
  bool XERE_Q = XERE.tock(spr.WEME, spr.DOSY, ZOCY);
  bool XUZO_Q = XUZO.tock(spr.WEME, spr.DOSY, YPUR);
  bool XEXA_Q = XEXA.tock(spr.WEME, spr.DOSY, YVOK);

  bool ZYWU = xor(YPOD_Q, ppu.APUX);
  bool ZUZA = xor(YROP_Q, ppu.ABEF);
  bool ZEJO = xor(YNEP_Q, ppu.ADAZ);
  bool ZEDA = xor(YZOF_Q, ppu.ASAH);
  bool YMAM = xor(XUVY_Q, ppu.ACAM);
  bool YTYP = xor(XERE_Q, ppu.AZUB);
  bool YFOP = xor(XUZO_Q, ppu.AMEL);
  bool YVAC = xor(XEXA_Q, ppu.AHAL);
  YTUB = nor(ZYWU, ZUZA, ZEJO, ZEDA);
  YLEV = nor(YMAM, YTYP, YFOP, YVAC);

  // matcher 9
  bool CYWE_Q = CYWE.tock(spr.CYLA, spr.EJAD, COSE);
  bool DYBY_Q = DYBY.tock(spr.CYLA, spr.EJAD, AROP);
  bool DURY_Q = DURY.tock(spr.CYLA, spr.EJAD, XATU);
  bool CUVY_Q = CUVY.tock(spr.CYLA, spr.EJAD, BADY);
  bool FUSA_Q = FUSA.tock(spr.CYLA, spr.EJAD, ZAGO);
  bool FAXA_Q = FAXA.tock(spr.CYLA, spr.EJAD, ZOCY);
  bool FOZY_Q = FOZY.tock(spr.CYLA, spr.EJAD, YPUR);
  bool FESY_Q = FESY.tock(spr.CYLA, spr.EJAD, YVOK);

  bool BAZY = xor(CYWE_Q, ppu.APUX);
  bool CYLE = xor(DYBY_Q, ppu.ABEF);
  bool CEVA = xor(DURY_Q, ppu.ADAZ);
  bool BUMY = xor(CUVY_Q, ppu.ASAH);
  bool GUZO = xor(FUSA_Q, ppu.ACAM);
  bool GOLA = xor(FAXA_Q, ppu.AZUB);
  bool GEVE = xor(FOZY_Q, ppu.AMEL);
  bool GUDE = xor(FESY_Q, ppu.AHAL);
  COGY = nor(BAZY, CYLE, CEVA, BUMY);
  FYMA = nor(GUZO, GOLA, GEVE, GUDE);

  // matcher 10
  bool DUHY_Q = DUHY.tock(spr.CACU, spr.GAMY, COSE);
  bool EJUF_Q = EJUF.tock(spr.CACU, spr.GAMY, AROP);
  bool ENOR_Q = ENOR.tock(spr.CACU, spr.GAMY, XATU);
  bool DEPY_Q = DEPY.tock(spr.CACU, spr.GAMY, BADY);
  bool FOKA_Q = FOKA.tock(spr.CACU, spr.GAMY, ZAGO);
  bool FYTY_Q = FYTY.tock(spr.CACU, spr.GAMY, ZOCY);
  bool FUBY_Q = FUBY.tock(spr.CACU, spr.GAMY, YPUR);
  bool GOXU_Q = GOXU.tock(spr.CACU, spr.GAMY, YVOK);

  bool CEKO = xor(DUHY_Q, ppu.APUX);
  bool DETY = xor(EJUF_Q, ppu.ABEF);
  bool DOZO = xor(ENOR_Q, ppu.ADAZ);
  bool CONY = xor(DEPY_Q, ppu.ASAH);
  bool FUZU = xor(FOKA_Q, ppu.ACAM);
  bool FESO = xor(FYTY_Q, ppu.AZUB);
  bool FOKY = xor(FUBY_Q, ppu.AMEL);
  bool FYVA = xor(GOXU_Q, ppu.AHAL);
  CEHU = nor(CEKO, DETY, DOZO, CONY);
  EKES = nor(FUZU, FESO, FOKY, FYVA);
}