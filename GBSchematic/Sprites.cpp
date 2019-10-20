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

void Sprites::tick(OAM& oam, const Resets& rst, const AddressDecoder& dec, const Vram& vram, const MemBus& mem) {
  //----------
  // sprite scan counter

  wire YFEL_Q = YFEL.q();
  wire WEWY_Q = WEWY.q();
  wire GOSO_Q = GOSO.q();
  wire ELYN_Q = ELYN.q();
  wire FAHA_Q = FAHA.q();
  wire FONY_Q = FONY.q();

  SPRITE_SCAN_DONE = and(YFEL_Q, WEWY_Q, GOSO_Q, FONY_Q);
  wire GAVA = or(SPRITE_SCAN_DONE, SPR_CLK_2M.q());

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

  ATEJ = not(unk3(ANEL_Q, not(CATU_Q), rst.RESET_VIDEO));

  ANOM = nor(not(rst.RESET_VIDEO), ATEJ);

  AZYB = not(ATEJ);
  wire AMYG = not(rst.RESET_VIDEO);
  wire ABAK = or(ATEJ, AMYG);
  BYVA = not(ABAK);

  wire BOGE = not(MATU_Q);
  BESU = unk2(CATU_Q, or(not(rst.RESET_VIDEO), AVAP));
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

  oam.OAM_CLK = not(clk.CLK3);
  oam.OAM_ADDR_RENDER = BETE;
  oam.OAM_ADDR_PARSE = APAR;
  oam.OAM_ADDR_CPU = ASAM;

  if (not(APAG)) {
    oam.OAM_A_D0 = mem.D0;
    oam.OAM_A_D1 = mem.D1;
    oam.OAM_A_D2 = mem.D2;
    oam.OAM_A_D3 = mem.D3;
    oam.OAM_A_D4 = mem.D4;
    oam.OAM_A_D5 = mem.D5;
    oam.OAM_A_D6 = mem.D6;
    oam.OAM_A_D7 = mem.D7;

    oam.OAM_B_D0 = mem.D0;
    oam.OAM_B_D1 = mem.D1;
    oam.OAM_B_D2 = mem.D2;
    oam.OAM_B_D3 = mem.D3;
    oam.OAM_B_D4 = mem.D4;
    oam.OAM_B_D5 = mem.D5;
    oam.OAM_B_D6 = mem.D6;
    oam.OAM_B_D7 = mem.D7;
  }

  if (not(dma.VRAM_TO_OAM)) {
    oam.OAM_A_D0 = vram.MD0;
    oam.OAM_A_D1 = vram.MD1;
    oam.OAM_A_D2 = vram.MD2;
    oam.OAM_A_D3 = vram.MD3;
    oam.OAM_A_D4 = vram.MD4;
    oam.OAM_A_D5 = vram.MD5;
    oam.OAM_A_D6 = vram.MD6;
    oam.OAM_A_D7 = vram.MD7;

    oam.OAM_B_D0 = vram.MD0;
    oam.OAM_B_D1 = vram.MD1;
    oam.OAM_B_D2 = vram.MD2;
    oam.OAM_B_D3 = vram.MD3;
    oam.OAM_B_D4 = vram.MD4;
    oam.OAM_B_D5 = vram.MD5;
    oam.OAM_B_D6 = vram.MD6;
    oam.OAM_B_D7 = vram.MD7;
  }

  //----------


  wire GEKA = not((mem.A0 & oam.OAM_ADDR_CPU) | (WEFE & oam.OAM_ADDR_RENDER) | (ext.P10_B & oam.OAM_ADDR_PARSE) | (dma.DMA_A0 & oam.OAM_ADDR_DMA));
  oam.OAM_A_CS = nand(dma.WYJA, GEKA);
  oam.OAM_B_CS = nand(dma.WYJA, not(GEKA));

  WUME = not(and(not(GEKA), AMAB, not(nand(cpu.CPU_RD2, dma.CATY))));
  WEWU = not(and(not(nand(cpu.CPU_RD2, dma.CATY)), AMAB, GEKA));

  ANEL.tock(not(SPR_CLK_2M.q()), rst.RESET_VIDEO, CATU_Q);

  wire XECY_Q = XECY.tock(XUCA, 0, rst.RESET2);
  XUVA.tock(XYNY, rst.RESET2, XECY_Q);


  oam.OAM_A7 = not((mem.A7 & oam.OAM_ADDR_CPU) | (WEZA & oam.OAM_ADDR_RENDER) | (FONY_Q & oam.OAM_ADDR_PARSE)    | (dma.DMA_A7 & oam.OAM_ADDR_DMA));
  oam.OAM_A6 = not((mem.A6 & oam.OAM_ADDR_CPU) | (WUCO & oam.OAM_ADDR_RENDER) | (FAHA_Q & oam.OAM_ADDR_PARSE)    | (dma.DMA_A6 & oam.OAM_ADDR_DMA));
  oam.OAM_A5 = not((mem.A5 & oam.OAM_ADDR_CPU) | (WYDA & oam.OAM_ADDR_RENDER) | (ELYN_Q & oam.OAM_ADDR_PARSE)    | (dma.DMA_A5 & oam.OAM_ADDR_DMA));
  oam.OAM_A4 = not((mem.A4 & oam.OAM_ADDR_CPU) | (ZYSU & oam.OAM_ADDR_RENDER) | (GOSO_Q & oam.OAM_ADDR_PARSE)    | (dma.DMA_A4 & oam.OAM_ADDR_DMA));
  oam.OAM_A3 = not((mem.A3 & oam.OAM_ADDR_CPU) | (WYSE & oam.OAM_ADDR_RENDER) | (WEWY_Q & oam.OAM_ADDR_PARSE)    | (dma.DMA_A3 & oam.OAM_ADDR_DMA));
  oam.OAM_A2 = not((mem.A2 & oam.OAM_ADDR_CPU) | (WUZY & oam.OAM_ADDR_RENDER) | (YFEL_Q & oam.OAM_ADDR_PARSE)    | (dma.DMA_A2 & oam.OAM_ADDR_DMA));
  oam.OAM_A1 = not((mem.A1 & oam.OAM_ADDR_CPU) | (WEFE & oam.OAM_ADDR_RENDER) | (ext.P10_B & oam.OAM_ADDR_PARSE) | (dma.DMA_A1 & oam.OAM_ADDR_DMA));
}

//-----------------------------------------------------------------------------

void Sprites::tick_control(const OAM & oam, const Resets& rst, const Window& win, Vram& vram, MemBus& mem) {

  //----------
  // bottom

  wire SPR_CLK_2MQ = SPR_CLK_2M.q();
  wire WOSU_Q = WOSU.q();
  wire CENO_Q = CENO.q();
  wire BYBA_Q = BYBA.q();
  wire DOBA_Q = DOBA.q(); 

  XUPY = SPR_CLK_2M.q();
  XOCE = not(WOSU_Q);
  XYSO = or(!SPR_CLK_2MQ, !WOSU_Q);
  CEHA = not(!CENO_Q);
  BUZA = and(!CENO_Q, ppu.XYMU);
  DYTY = nor(XOCE, CEHA, SPR_MATCH);
  AVAP = nor(DOBA_Q, not(ANOM), !BYBA_Q);

  SPR_CLK_2M.flip(clk.CLK_4M_A, rst.RESET_VIDEO);

  WOSU.tock(not(clk.CLK_4M_A), rst.RESET_VIDEO, !SPR_CLK_2MQ);

  CENO.tock(SPR_CLK_2MQ, rst.RESET_VIDEO, BESU);

  CATU.tock(SPR_CLK_2MQ, rst.RESET_VIDEO, and(ppu.SELA, not(ppu.XYVO)));

  BYBA.tock(SPR_CLK_2MQ, ANOM, SPRITE_SCAN_DONE);
  DOBA.tock(clk.CLK_4M_B, ANOM, BYBA_Q);

  //----------
  // center thing

  wire TOBU_Q = TOBU.q();
  wire FETCH_LOHI_Q = FETCH_LOHI.q();

  wire TYFO_Q = TYFO.q();
  wire SEBA_Q = SEBA.q();
  wire TOXE_Q = TOXE.q();
  wire TULY_Q = TULY.q();
  wire TESE_Q = TESE.q();

  LAPE = not(clk.CLK_4M_B);

  WUTY = nor(!TYFO_Q, nand(TOXE_Q, SEBA_Q, FETCH_LOHI_Q));
  XEFY = not(WUTY);

  TEXY = nor(nor(TULY_Q, FETCH_LOHI_Q), not(ppu.XYMU));

  wire BAXO_Q = BAXO.q();
  XONO = and(!BAXO_Q, TEXY);

  wire TYTU = not(nand(TESE_Q, TOXE_Q));
  XADO = not(and(TULY_Q, nor(TYTU, not(ppu.XYMU), TYFO_Q)));
  PUCO = not(and(FETCH_LOHI_Q, nor(TYTU, not(ppu.XYMU), TYFO_Q)));

  TYFO.tock(not(clk.CLK_4M_B), win.VYPO, nand(TESE_Q, TOXE_Q));
  SEBA.tock(not(clk.CLK_4M_B), ppu.XYMU, FETCH_LOHI_Q);
  TOXE.flip(nand(not(clk.CLK_4M_B), nand(TESE_Q, TOXE_Q)),    win.SECA);
  TULY.flip(!TOXE_Q, win.SECA);
  TESE.flip(!TULY_Q, win.SECA);

  //----------
  // Sprite priority

  wire AROR = and(not(CENO_Q), ppu.XYMU, ppu.LCDC_SPREN); // check polarity of clock

  wire XAGE = nand(AROR, YNAZ, YKOK);
  wire YLOZ = nand(AROR, ZURE, YWOS);
  wire DEGO = nand(AROR, EKES, CEHU);
  wire DYDU = nand(AROR, EWAM, CYVY);
  wire YDUG = nand(AROR, ZAKO, XEBA);

  wire YGEM = nand(AROR, YLEV, YTUB);
  wire EFYL = nand(AROR, FEHA, DAMA);
  wire DYKA = nand(AROR, CYCO, DAJE);
  wire YBEZ = nand(AROR, YDOT, YWAP);
  wire EGOM = nand(AROR, FYMA, COGY);

  wire FEFY = nand(XAGE, YLOZ, DEGO, DYDU, YDUG);
  wire FOVE = nand(YGEM, EFYL, DYKA, YBEZ, EGOM);
  FEPO = or(FEFY, FOVE);

  wire GEZE = or(not(YDUG), ext.P10_B);
  wire GUVA = nor(YDUG, ext.P10_B);

  wire FUMA = or(not(DYDU), GEZE);
  wire ENUT = nor(DYDU, GEZE);

  wire GEDE = or(not(DEGO), FUMA);
  wire EMOL = nor(DEGO, FUMA);

  wire WUTO = or(not(YLOZ), GEDE);
  wire GYFY = nor(YLOZ, GEDE);

  wire XYLA = or(not(XAGE), WUTO);
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

  wire FONO_Q = FONO.tock(WUTY, BYVA, GUZE);
  wire EXUQ_Q = EXUQ.tock(WUTY, BYVA, DENY);
  wire WAPO_Q = WAPO.tock(WUTY, BYVA, GUGY);
  wire WOMY_Q = WOMY.tock(WUTY, BYVA, XYME);
  wire WAFY_Q = WAFY.tock(WUTY, BYVA, GYGY);
  wire XUDY_Q = XUDY.tock(WUTY, BYVA, GOWO);
  wire GOTA_Q = GOTA.tock(WUTY, BYVA, GYMA);
  wire EGAV_Q = EGAV.tock(WUTY, BYVA, FAME);
  wire CEDY_Q = CEDY.tock(WUTY, BYVA, DYDO);
  wire EBOJ_Q = EBOJ.tock(WUTY, BYVA, FURO);

  wire DYBA = not(BYVA);

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

  wire YDYV_Q = YDYV.latch(clk.CLK3, oam.OAM_B_D0);
  wire YCEB_Q = YCEB.latch(clk.CLK3, oam.OAM_B_D1);
  wire ZUCA_Q = ZUCA.latch(clk.CLK3, oam.OAM_B_D2);
  wire WONE_Q = WONE.latch(clk.CLK3, oam.OAM_B_D3);
  wire ZAXE_Q = ZAXE.latch(clk.CLK3, oam.OAM_B_D4);
  wire XAFU_Q = XAFU.latch(clk.CLK3, oam.OAM_B_D5);
  wire YSES_Q = YSES.latch(clk.CLK3, oam.OAM_B_D6);
  wire ZECA_Q = ZECA.latch(clk.CLK3, oam.OAM_B_D7);

  bool MATU_Q = dma.MATU.q();
  bool CUFE = unk3(sys.SARO, MATU_Q, dma.MOPA_PHI);

  TACU = nand(TYTU, TYFO_Q);
  TUVO = or(not(ppu.XYMU), TULY_Q, TESE_Q);
  COTA = not(nor(CUFE, not(and(TACU, TUVO)), and(ACYL, XYSO)));

  wire YWOK = not(COTA);

  wire XUSO_Q = XUSO.tock(YWOK, 0, YDYV_Q);
  wire XEGU_Q = XEGU.tock(YWOK, 0, YCEB_Q);
  wire YJEX_Q = YJEX.tock(YWOK, 0, ZUCA_Q);
  wire XYJU_Q = XYJU.tock(YWOK, 0, WONE_Q);
  wire YBOG_Q = YBOG.tock(YWOK, 0, ZAXE_Q);
  wire WYSO_Q = WYSO.tock(YWOK, 0, XAFU_Q);
  wire XOTE_Q = XOTE.tock(YWOK, 0, YSES_Q);
  wire YZAB_Q = YZAB.tock(YWOK, 0, ZECA_Q);

  wire EBOS = not(ppu.V0);
  wire DASA = not(ppu.V1);
  wire FUKY = not(ppu.V2);
  wire FUVE = not(ppu.V3);
  wire FEPU = not(ppu.V4);
  wire FOFA = not(ppu.V5);
  wire FEMO = not(ppu.V6);
  wire GUSU = not(ppu.V7);

  // this is adding the sprite y to the screen y and getting the corresponding line of the sprite to render
  // maybe the "adder" is expecting its inputs to be negated?

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

  DEGE = not(ERUC_S);
  DABY = not(ENEF_S);
  DABU = not(FECO_S);
  GYSA = not(GYKY_S);
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


  wire FUFO = not(ppu.LCDC_SPRSIZE);
  wire GOVU = or(GYKY_S, ppu.LCDC_SPRSIZE);
  wire WOTA = nand(GACE, GUVU, GYDA, GEWY, WUHU_C, GOVU);
  wire GESE = not(WOTA);
  SPR_MATCH = GESE;

  if (WUME) {
    mem.D0 = YDYV_Q;
    mem.D1 = YCEB_Q;
    mem.D2 = ZUCA_Q;
    mem.D3 = WONE_Q;
    mem.D4 = ZAXE_Q;
    mem.D5 = XAFU_Q;
    mem.D6 = YSES_Q;
    mem.D7 = ZECA_Q;
  }

  if (not(TEXY)) {
    vram.MA0 = not(FETCH_LOHI_Q); // so this is selecting lo/hi sprite byte

    wire GEJY = amux2(!XUSO_Q, FUFO, ppu.LCDC_SPRSIZE, WAGO);

    wire BAXE = not(CYVU);
    wire ARAS = not(BORE);
    wire AGAG = not(BUVY);
    wire FAMU = not(GEJY);

    vram.MA1 = BAXE;
    vram.MA2 = ARAS;
    vram.MA3 = AGAG;
    vram.MA4 = FAMU;

    // only 7 of these?
    vram.MA5  = XEGU_Q;
    vram.MA6  = YJEX_Q;
    vram.MA7  = XYJU_Q;
    vram.MA8  = YBOG_Q;
    vram.MA9  = WYSO_Q;
    vram.MA10 = XOTE_Q;
    vram.MA11 = YZAB_Q;

    // wat?
    vram.MA12 = not(ext.P10_B);
  }

  TOBU.tock(win.TAVA, ppu.XYMU, TULY_Q);
  FETCH_LOHI.tock(win.TAVA, ppu.XYMU, !TOBU_Q);

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

  wire WYXO = or(DYTY, GEBU);
  wire XUJO = or(DYTY, WOMU);
  wire GAPE = or(DYTY, GUNA);
  wire GUVE = or(DYTY, FOCO);
  wire CAHO = or(DYTY, DEWY);
  wire CEMY = or(DYTY, DEZO);
  wire CATO = or(DYTY, DOGU);
  wire CADO = or(DYTY, CUGU);
  wire CECU = or(DYTY, CUPE);
  wire BYBY = or(DYTY, CUVA);

  // why so many signals?

  CACU = WYXO; BUZY = WYXO; FUKE = WYXO;
  ZAPE = XUJO; WUSE = XUJO; ZURU = XUJO;
  FEFO = GAPE; GECY = GAPE; WABE = GAPE;
  FEKA = GUVE; XYHA = GUVE; YFAG = GUVE;
  CEXU = CAHO; AKOL = CAHO; BYMY = CAHO;
  FUXU = CEMY; ENOB = CEMY; GENY = CEMY;
  WEME = CATO; WUFA = CATO; FAKA = CATO;
  CYLA = CADO; DYMO = CADO; BUCY = CADO;
  WOFO = CECU; WYLU = CECU; EWOT = CECU;
  ASYS = BYBY; AHOF = BYBY; BYVY = BYBY;

  DEZY.tock(clk.CLK_4M_A, rst.RESET_VIDEO, DYTY);
  BESE.flip(CAKE,    AZYB);
  CUXY.flip(!BESE_Q, AZYB);
  BEGO.flip(!CUXY_Q, AZYB);
  DYBE.flip(!BEGO_Q, AZYB);

  bool XYKY_Q = XYKY.latch(clk.CLK3, oam.OAM_A_D0);
  bool YRUM_Q = YRUM.latch(clk.CLK3, oam.OAM_A_D1);
  bool YSEX_Q = YSEX.latch(clk.CLK3, oam.OAM_A_D2);
  bool YVEL_Q = YVEL.latch(clk.CLK3, oam.OAM_A_D3);
  bool WYNO_Q = WYNO.latch(clk.CLK3, oam.OAM_A_D4);
  bool CYRA_Q = CYRA.latch(clk.CLK3, oam.OAM_A_D5);
  bool ZUVE_Q = ZUVE.latch(clk.CLK3, oam.OAM_A_D6);
  bool ECED_Q = ECED.latch(clk.CLK3, oam.OAM_A_D7);

  // this seems out of place...
  if (WEWU) {
    mem.D0 = XYKY_Q;
    mem.D1 = YRUM_Q;
    mem.D2 = YSEX_Q;
    mem.D3 = YVEL_Q;
    mem.D4 = WYNO_Q;
    mem.D5 = CYRA_Q;
    mem.D6 = ZUVE_Q;
    mem.D7 = ECED_Q;
  }

  bool XEGA = not(COTA); // what is this thing?
  bool YLOR_Q = YLOR.tock(XEGA, 0, XYKY_Q);
  bool ZYTY_Q = ZYTY.tock(XEGA, 0, YRUM_Q);
  bool ZYVE_Q = ZYVE.tock(XEGA, 0, YSEX_Q);
  bool ZEZY_Q = ZEZY.tock(XEGA, 0, YVEL_Q);
  bool GOMO_Q = GOMO.tock(XEGA, 0, WYNO_Q);
  /*bool BAXO_Q =*/ BAXO.tock(XEGA, 0, CYRA_Q);
  /*bool YZOS_Q =*/ YZOS.tock(XEGA, 0, ZUVE_Q);
  bool DEPO_Q = DEPO.tock(XEGA, 0, ECED_Q);

  // CHECK CLK/RESET WIRES

  // matcher 1
  bool XEPE_Q = XEPE.tock(FUXU, DYNA, YLOR_Q);
  bool YLAH_Q = YLAH.tock(FUXU, DYNA, ZYTY_Q);
  bool ZOLA_Q = ZOLA.tock(FUXU, DYNA, ZYVE_Q);
  bool ZULU_Q = ZULU.tock(FUXU, DYNA, ZEZY_Q);
  bool WELO_Q = WELO.tock(FUXU, DYNA, GOMO_Q);
  bool XUNY_Q = XUNY.tock(FUXU, DYNA, BAXO_Q);
  bool WOTE_Q = WOTE.tock(FUXU, DYNA, YZOS_Q);
  bool XAKO_Q = XAKO.tock(FUXU, DYNA, DEPO_Q);

  bool ZOGY = xor(XEPE_Q, ppu.X0);
  bool ZEBA = xor(YLAH_Q, ppu.X1);
  bool ZAHA = xor(ZOLA_Q, ppu.X2);
  bool ZOKY = xor(ZULU_Q, ppu.X3);
  bool WOJU = xor(WELO_Q, ppu.X4);
  bool YFUN = xor(XUNY_Q, ppu.X5);
  bool WYZA = xor(WOTE_Q, ppu.X6);
  bool YPUK = xor(XAKO_Q, ppu.X7);

  ZAKO = nor(ZOGY, ZEBA, ZAHA, ZOKY);
  XEBA = nor(WOJU, YFUN, WYZA, YPUK);

  // matcher 2
  bool XOLY_Q = XOLY.tock(YFAG, WUPA, YLOR_Q);
  bool XYBA_Q = XYBA.tock(YFAG, WUPA, ZYTY_Q);
  bool XABE_Q = XABE.tock(YFAG, WUPA, ZYVE_Q);
  bool XEKA_Q = XEKA.tock(YFAG, WUPA, ZEZY_Q);
  bool XOMY_Q = XOMY.tock(YFAG, WUPA, GOMO_Q);
  bool WUHA_Q = WUHA.tock(YFAG, WUPA, BAXO_Q);
  bool WYNA_Q = WYNA.tock(YFAG, WUPA, YZOS_Q);
  bool WECO_Q = WECO.tock(YFAG, WUPA, DEPO_Q);

  bool YHOK = xor(XOLY_Q, ppu.X0);
  bool YCAH = xor(XYBA_Q, ppu.X1);
  bool YDAJ = xor(XABE_Q, ppu.X2);
  bool YVUZ = xor(XEKA_Q, ppu.X3);
  bool YVAP = xor(XOMY_Q, ppu.X4);
  bool XENY = xor(WUHA_Q, ppu.X5);
  bool XAVU = xor(WYNA_Q, ppu.X6);
  bool XEVA = xor(WECO_Q, ppu.X7);
  YWOS = nor(YVAP, XENY, XAVU, XEVA);
  ZURE = nor(YHOK, YCAH, YDAJ, YVUZ);

  // matcher 3
  bool ERAZ_Q = ERAZ.tock(GECY, GAFY, YLOR_Q);
  bool EPUM_Q = EPUM.tock(GECY, GAFY, ZYTY_Q);
  bool EROL_Q = EROL.tock(GECY, GAFY, ZYVE_Q);
  bool EHYN_Q = EHYN.tock(GECY, GAFY, ZEZY_Q);
  bool FAZU_Q = FAZU.tock(GECY, GAFY, GOMO_Q);
  bool FAXE_Q = FAXE.tock(GECY, GAFY, BAXO_Q);
  bool EXUK_Q = EXUK.tock(GECY, GAFY, YZOS_Q);
  bool FEDE_Q = FEDE.tock(GECY, GAFY, DEPO_Q);

  bool DUSE = xor(ERAZ_Q, ppu.X0);
  bool DAGU = xor(EPUM_Q, ppu.X1);
  bool DYZE = xor(EROL_Q, ppu.X2);
  bool DESO = xor(EHYN_Q, ppu.X3);
  bool EJOT = xor(FAZU_Q, ppu.X4);
  bool ESAJ = xor(FAXE_Q, ppu.X5);
  bool DUCU = xor(EXUK_Q, ppu.X6);
  bool EWUD = xor(FEDE_Q, ppu.X7);
  DAJE = nor(EJOT, ESAJ, DUCU, EWUD);
  CYCO = nor(DUSE, DAGU, DYZE, DESO);

  // matcher 4
  bool DANY_Q = DANY.tock(ASYS, DOKU, YLOR_Q);
  bool DUKO_Q = DUKO.tock(ASYS, DOKU, ZYTY_Q);
  bool DESU_Q = DESU.tock(ASYS, DOKU, ZYVE_Q);
  bool DAZO_Q = DAZO.tock(ASYS, DOKU, ZEZY_Q);
  bool DAKE_Q = DAKE.tock(ASYS, DOKU, GOMO_Q);
  bool CESO_Q = CESO.tock(ASYS, DOKU, BAXO_Q);
  bool DYFU_Q = DYFU.tock(ASYS, DOKU, YZOS_Q);
  bool CUSY_Q = CUSY.tock(ASYS, DOKU, DEPO_Q);

  bool EDYM = xor(DANY_Q, ppu.X0);
  bool EMYB = xor(DUKO_Q, ppu.X1);
  bool EBEF = xor(DESU_Q, ppu.X2);
  bool EWOK = xor(DAZO_Q, ppu.X3);
  bool COLA = xor(DAKE_Q, ppu.X4);
  bool BOBA = xor(CESO_Q, ppu.X5);
  bool COLU = xor(DYFU_Q, ppu.X6);
  bool BAHU = xor(CUSY_Q, ppu.X7);
  CYVY = nor(COLA, BOBA, COLU, BAHU);
  EWAM = nor(EDYM, EMYB, EBEF, EWOK);

  // matcher 5
  bool YCOL_Q = YCOL.tock(ZAPE, XAHO, YLOR_Q);
  bool YRAC_Q = YRAC.tock(ZAPE, XAHO, ZYTY_Q);
  bool YMEM_Q = YMEM.tock(ZAPE, XAHO, ZYVE_Q);
  bool YVAG_Q = YVAG.tock(ZAPE, XAHO, ZEZY_Q);
  bool ZOLY_Q = ZOLY.tock(ZAPE, XAHO, GOMO_Q);
  bool ZOGO_Q = ZOGO.tock(ZAPE, XAHO, BAXO_Q);
  bool ZECU_Q = ZECU.tock(ZAPE, XAHO, YZOS_Q);
  bool ZESA_Q = ZESA.tock(ZAPE, XAHO, DEPO_Q);

  bool XOSU = xor(YCOL_Q, ppu.X0);
  bool ZUVU = xor(YRAC_Q, ppu.X1);
  bool XUCO = xor(YMEM_Q, ppu.X2);
  bool ZULO = xor(YVAG_Q, ppu.X3);
  bool ZARE = xor(ZOLY_Q, ppu.X4);
  bool ZEMU = xor(ZOGO_Q, ppu.X5);
  bool ZYGO = xor(ZECU_Q, ppu.X6);
  bool ZUZY = xor(ZESA_Q, ppu.X7);
  YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
  YDOT = nor(XOSU, ZUVU, XUCO, ZULO);

  // matcher 6
  bool WEDU_Q = WEDU.tock(WUNU, WOFO, YLOR_Q);
  bool YGAJ_Q = YGAJ.tock(WUNU, WOFO, ZYTY_Q);
  bool ZYJO_Q = ZYJO.tock(WUNU, WOFO, ZYVE_Q);
  bool XURY_Q = XURY.tock(WUNU, WOFO, ZEZY_Q);
  bool YBED_Q = YBED.tock(WUNU, WOFO, GOMO_Q);
  bool ZALA_Q = ZALA.tock(WUNU, WOFO, BAXO_Q);
  bool WYDE_Q = WYDE.tock(WUNU, WOFO, YZOS_Q);
  bool XEPA_Q = XEPA.tock(WUNU, WOFO, DEPO_Q);

  bool XEJU = xor(WEDU_Q, ppu.X0);
  bool ZATE = xor(YGAJ_Q, ppu.X1);
  bool ZAKU = xor(ZYJO_Q, ppu.X2);
  bool YBOX = xor(XURY_Q, ppu.X3);
  bool ZYKU = xor(YBED_Q, ppu.X4);
  bool ZYPU = xor(ZALA_Q, ppu.X5);
  bool XAHA = xor(WYDE_Q, ppu.X6);
  bool ZEFE = xor(XEPA_Q, ppu.X7);
  YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
  YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);

  // matcher 7
  bool GAVY_Q = GAVY.tock(CEXU, WUZO, YLOR_Q);
  bool GYPU_Q = GYPU.tock(CEXU, WUZO, ZYTY_Q);
  bool GADY_Q = GADY.tock(CEXU, WUZO, ZYVE_Q);
  bool GAZA_Q = GAZA.tock(CEXU, WUZO, ZEZY_Q);
  bool EZUF_Q = EZUF.tock(CEXU, WUZO, GOMO_Q);
  bool ENAD_Q = ENAD.tock(CEXU, WUZO, BAXO_Q);
  bool EBOW_Q = EBOW.tock(CEXU, WUZO, YZOS_Q);
  bool FYCA_Q = FYCA.tock(CEXU, WUZO, DEPO_Q);

  bool GOHO = xor(GAVY_Q, ppu.X0);
  bool GASU = xor(GYPU_Q, ppu.X1);
  bool GABU = xor(GADY_Q, ppu.X2);
  bool GAFE = xor(GAZA_Q, ppu.X3);
  bool DUZE = xor(EZUF_Q, ppu.X4);
  bool DAGA = xor(ENAD_Q, ppu.X5);
  bool DAWU = xor(EBOW_Q, ppu.X6);
  bool EJAW = xor(FYCA_Q, ppu.X7);
  DAMA = nor(DUZE, DAGA, DAWU, EJAW);
  FEHA = nor(GOHO, GASU, GABU, GAFE);

  // matcher 8
  bool XUVY_Q = XUVY.tock(WEME, DOSY, YLOR_Q);
  bool XERE_Q = XERE.tock(WEME, DOSY, ZYTY_Q);
  bool XUZO_Q = XUZO.tock(WEME, DOSY, ZYVE_Q);
  bool XEXA_Q = XEXA.tock(WEME, DOSY, ZEZY_Q);
  bool YPOD_Q = YPOD.tock(WEME, DOSY, GOMO_Q);
  bool YROP_Q = YROP.tock(WEME, DOSY, BAXO_Q);
  bool YNEP_Q = YNEP.tock(WEME, DOSY, YZOS_Q);
  bool YZOF_Q = YZOF.tock(WEME, DOSY, DEPO_Q);

  bool YMAM = xor(XUVY_Q, ppu.X0);
  bool YTYP = xor(XERE_Q, ppu.X1);
  bool YFOP = xor(XUZO_Q, ppu.X2);
  bool YVAC = xor(XEXA_Q, ppu.X3);
  bool ZYWU = xor(YPOD_Q, ppu.X4);
  bool ZUZA = xor(YROP_Q, ppu.X5);
  bool ZEJO = xor(YNEP_Q, ppu.X6);
  bool ZEDA = xor(YZOF_Q, ppu.X7);
  YTUB = nor(ZYWU, ZUZA, ZEJO, ZEDA);
  YLEV = nor(YMAM, YTYP, YFOP, YVAC);

  // matcher 9
  bool FUSA_Q = FUSA.tock(CYLA, EJAD, YLOR_Q);
  bool FAXA_Q = FAXA.tock(CYLA, EJAD, ZYTY_Q);
  bool FOZY_Q = FOZY.tock(CYLA, EJAD, ZYVE_Q);
  bool FESY_Q = FESY.tock(CYLA, EJAD, ZEZY_Q);
  bool CYWE_Q = CYWE.tock(CYLA, EJAD, GOMO_Q);
  bool DYBY_Q = DYBY.tock(CYLA, EJAD, BAXO_Q);
  bool DURY_Q = DURY.tock(CYLA, EJAD, YZOS_Q);
  bool CUVY_Q = CUVY.tock(CYLA, EJAD, DEPO_Q);

  bool GUZO = xor(FUSA_Q, ppu.X0);
  bool GOLA = xor(FAXA_Q, ppu.X1);
  bool GEVE = xor(FOZY_Q, ppu.X2);
  bool GUDE = xor(FESY_Q, ppu.X3);
  bool BAZY = xor(CYWE_Q, ppu.X4);
  bool CYLE = xor(DYBY_Q, ppu.X5);
  bool CEVA = xor(DURY_Q, ppu.X6);
  bool BUMY = xor(CUVY_Q, ppu.X7);
  COGY = nor(BAZY, CYLE, CEVA, BUMY);
  FYMA = nor(GUZO, GOLA, GEVE, GUDE);

  // matcher 10
  bool FOKA_Q = FOKA.tock(CACU, GAMY, YLOR_Q);
  bool FYTY_Q = FYTY.tock(CACU, GAMY, ZYTY_Q);
  bool FUBY_Q = FUBY.tock(CACU, GAMY, ZYVE_Q);
  bool GOXU_Q = GOXU.tock(CACU, GAMY, ZEZY_Q);
  bool DUHY_Q = DUHY.tock(CACU, GAMY, GOMO_Q);
  bool EJUF_Q = EJUF.tock(CACU, GAMY, BAXO_Q);
  bool ENOR_Q = ENOR.tock(CACU, GAMY, YZOS_Q);
  bool DEPY_Q = DEPY.tock(CACU, GAMY, DEPO_Q);

  bool FUZU = xor(FOKA_Q, ppu.X0);
  bool FESO = xor(FYTY_Q, ppu.X1);
  bool FOKY = xor(FUBY_Q, ppu.X2);
  bool FYVA = xor(GOXU_Q, ppu.X3);
  bool CEKO = xor(DUHY_Q, ppu.X4);
  bool DETY = xor(EJUF_Q, ppu.X5);
  bool DOZO = xor(ENOR_Q, ppu.X6);
  bool CONY = xor(DEPY_Q, ppu.X7);
  CEHU = nor(CEKO, DETY, DOZO, CONY);
  EKES = nor(FUZU, FESO, FOKY, FYVA);
}

//-----------------------------------------------------------------------------

void Sprites::tick_pixelshifter(const Vram& vram) {
  // sprite x flip
  bool POBE = mux2(vram.MD4, vram.MD3, XONO);
  bool PACY = mux2(vram.MD3, vram.MD4, XONO);
  bool PONO = mux2(vram.MD5, vram.MD2, XONO);
  bool PUGU = mux2(vram.MD2, vram.MD5, XONO);
  bool PUTE = mux2(vram.MD7, vram.MD0, XONO);
  bool PULY = mux2(vram.MD0, vram.MD7, XONO);
  bool PELO = mux2(vram.MD6, vram.MD1, XONO);
  bool PAWE = mux2(vram.MD1, vram.MD6, XONO);

  // the schematic layout is slightly inconsistent, might have some of these swapped

  bool PUDU_Q = PUDU.latch(XADO, POBE);
  bool RAMU_Q = RAMU.latch(PUCO, POBE);
  bool SELE_Q = SELE.latch(PUCO, PACY);
  bool SAJA_Q = SAJA.latch(XADO, PACY);
  bool MYTU_Q = MYTU.latch(XADO, PONO);
  bool MOFO_Q = MOFO.latch(PUCO, PONO);
  bool SUNY_Q = SUNY.latch(XADO, PUGU);
  bool SUTO_Q = SUTO.latch(PUCO, PUGU);
  bool PEFO_Q = PEFO.latch(XADO, PUTE);
  bool REWO_Q = REWO.latch(PUCO, PUTE);
  bool RYDU_Q = RYDU.latch(PUCO, PULY);
  bool SEGA_Q = RYDU.latch(XADO, PULY);
  bool PEBA_Q = PEBA.latch(XADO, PELO);
  bool ROKA_Q = ROKA.latch(PUCO, PELO);
  bool RAMA_Q = RAMA.latch(PUCO, PAWE);
  bool SEMO_Q = SEMO.latch(XADO, PAWE);

  bool LUBO = not(PUDU_Q);
  bool SOLO = not(RAMU_Q);
  bool VOBY = not(RAMU_Q);
  bool WERY = not(RAMU_Q);
  bool LUMO = not(RAMU_Q);
  bool LASE = not(RAMU_Q);
  bool WURA = not(RAMU_Q);
  bool WYCO = not(RAMU_Q);
  bool LOZA = not(RAMU_Q);
  bool RATA = not(RAMU_Q);
  bool SELU = not(RAMU_Q);
  bool WAMY = not(RAMU_Q);
  bool NUCA = not(RAMU_Q);
  bool SYBO = not(RAMU_Q);
  bool SERY = not(RAMU_Q);
  bool SULU = not(RAMU_Q);

  bool LUFY = nand(LUBO,   ppu.ROBY);
  bool MAME = nand(PUDU_Q, ppu.ROBY);
  bool REHU = nand(SOLO,   ppu.ROBY);
  bool RANO = nand(RAMU_Q, ppu.ROBY);

  bool WAXO = nand(VOBY,   ppu.TYTA);
  bool TYGA = nand(SELE_Q, ppu.TYTA);
  bool XATO = nand(WERY,   ppu.TYTA);
  bool VEXU = nand(SAJA_Q, ppu.TYTA);

  bool MAJO = nand(LUMO,   ppu.LYKU);
  bool MYXA = nand(MYTU_Q, ppu.LYKU);
  bool LYDE = nand(LASE,   ppu.LYKU);
  bool LELA = nand(MOFO_Q, ppu.LYKU);

  bool XEXU = nand(WURA,   ppu.TYCO);
  bool VABY = nand(SUNY_Q, ppu.TYCO);
  bool XOLE = nand(WYCO,   ppu.TYCO);
  bool VUME = nand(SUTO_Q, ppu.TYCO);

  bool MOFY = nand(LOZA,   ppu.LESY);
  bool MEZU = nand(PEFO_Q, ppu.LESY);
  bool PYZU = nand(RATA,   ppu.LESY);
  bool PABE = nand(REWO_Q, ppu.LESY);

  bool TULA = nand(SELU,   ppu.XOVU);
  bool TESO = nand(RYDU_Q, ppu.XOVU);
  bool XYVE = nand(WAMY,   ppu.XOVU);
  bool VUNE = nand(SEGA_Q, ppu.XOVU);

  bool MADA = nand(NUCA,   ppu.LOTA);
  bool MYTO = nand(PEBA_Q, ppu.LOTA);
  bool RUCA = nand(SYBO,   ppu.LOTA);
  bool RUSY = nand(ROKA_Q, ppu.LOTA);

  bool TABY = nand(SERY,   ppu.SOKA);
  bool TAPO = nand(RAMA_Q, ppu.SOKA);
  bool TUPE = nand(SULU,   ppu.SOKA);
  bool TUXA = nand(SEMO_Q, ppu.SOKA);

  bool NURO_Q = NURO.srtock(clk.CLKPIPE, PABE, PYZU, ext.P10_B);
  bool MASO_Q = MASO.srtock(clk.CLKPIPE, MYTO, MADA, NURO_Q);
  bool LEFE_Q = LEFE.srtock(clk.CLKPIPE, LELA, LYDE, MASO_Q);
  bool LESU_Q = LESU.srtock(clk.CLKPIPE, MAME, LUFY, LEFE_Q);
  bool WYHO_Q = WYHO.srtock(clk.CLKPIPE, VEXU, XATO, LESU_Q);
  bool WORA_Q = WORA.srtock(clk.CLKPIPE, VABY, XEXU, WYHO_Q);
  bool VAFO_Q = VAFO.srtock(clk.CLKPIPE, TUXA, TUPE, WORA_Q);
  bool WUFY_Q = WUFY.srtock(clk.CLKPIPE, VUNE, XYVE, VAFO_Q);

  bool NYLU_Q = NYLU.srtock(clk.CLKPIPE, MEZU, MOFY, ext.P10_B);
  bool PEFU_Q = PEFU.srtock(clk.CLKPIPE, RUSY, RUCA, NYLU_Q);
  bool NATY_Q = NATY.srtock(clk.CLKPIPE, MYXA, MAJO, PEFU_Q);
  bool PYJO_Q = PYJO.srtock(clk.CLKPIPE, RANO, REHU, NATY_Q);
  bool VARE_Q = VARE.srtock(clk.CLKPIPE, TYGA, WAXO, PYJO_Q);
  bool WEBA_Q = WEBA.srtock(clk.CLKPIPE, VUME, XOLE, VARE_Q);
  bool VANU_Q = VANU.srtock(clk.CLKPIPE, TAPO, TABY, WEBA_Q);
  bool VUPY_Q = VUPY.srtock(clk.CLKPIPE, TESO, TULA, VANU_Q);

  ppu.SPR_PIX_B_0 = NURO_Q;
  ppu.SPR_PIX_B_1 = MASO_Q;
  ppu.SPR_PIX_B_2 = LEFE_Q;
  ppu.SPR_PIX_B_3 = LESU_Q;
  ppu.SPR_PIX_B_4 = WYHO_Q;
  ppu.SPR_PIX_B_5 = WORA_Q;
  ppu.SPR_PIX_B_6 = VAFO_Q;
  ppu.SPR_PIX_B_7 = WUFY_Q;

  ppu.SPR_PIX_A_0 = NYLU_Q;
  ppu.SPR_PIX_A_1 = PEFU_Q;
  ppu.SPR_PIX_A_2 = NATY_Q;
  ppu.SPR_PIX_A_3 = PYJO_Q;
  ppu.SPR_PIX_A_4 = VARE_Q;
  ppu.SPR_PIX_A_5 = WEBA_Q;
  ppu.SPR_PIX_A_6 = VANU_Q;
  ppu.SPR_PIX_A_7 = VUPY_Q;
}

//-----------------------------------------------------------------------------

void Sprites::tick_spritestore() {
  bool SPR_CLK_2MQ = SPR_CLK_2M.q();
  bool CYKE = not(SPR_CLK_2MQ);
  bool WUDA = not(CYKE);

  bool XECU_Q = XECU.tock(WUDA, spr.WEFE, OAM_A_A7);
  bool YDUF_Q = YDUF.tock(WUDA, spr.WEFE, OAM_A_A6);
  bool XOBE_Q = XOBE.tock(WUDA, spr.WEFE, OAM_A_A5);
  bool ZUZE_Q = ZUZE.tock(WUDA, spr.WEFE, OAM_A_A4);
  bool XEDY_Q = XEDY.tock(WUDA, spr.WEFE, OAM_A_A3);
  bool XADU_Q = XADU.tock(WUDA, spr.WEFE, OAM_A_A2);

  if (spr.BUZA) {
    WEZA = XECU_Q;
    WUCO = YDUF_Q;
    WYDA = XOBE_Q;
    ZYSU = ZUZE_Q;
    WYSE = XEDY_Q;
    WUZY = XADU_Q;
  }

  if (spr.FEPO) {
    CUCA = spr.DABY;
    CEGA = spr.DABU;
    CUCU = spr.DEGE;
    WENU = spr.GYSA;
  }

  // Sprite store 1 of 10
  bool AXUV_Q = AXUV.q(); bool ADYB = not(!AXUV_Q); AXUV.tock(spr.AKOL, 0, WEZA);
  bool BADA_Q = BADA.q(); bool APOB = not(!BADA_Q); BADA.tock(spr.AKOL, 0, WUCO);
  bool APEV_Q = APEV.q(); bool APYV = not(!APEV_Q); APEV.tock(spr.AKOL, 0, WYDA);
  bool BADO_Q = BADO.q(); bool AFEN = not(!BADO_Q); BADO.tock(spr.AKOL, 0, ZYSU);
  bool BEXY_Q = BEXY.q(); bool AKYH = not(!BEXY_Q); BEXY.tock(spr.AKOL, 0, WYSE);
  bool BYHE_Q = BYHE.q(); bool APOC = not(!BYHE_Q); BYHE.tock(spr.AKOL, 0, WUZY);
  bool AFYM_Q = AFYM.q(); bool BUJY = not(!AFYM_Q); AFYM.tock(spr.BYMY, 0, CUCA);
  bool AZAP_Q = AZAP.q(); bool BOSO = not(!AZAP_Q); AZAP.tock(spr.BYMY, 0, CEGA);
  bool AFUT_Q = AFUT.q(); bool AHAC = not(!AFUT_Q); AFUT.tock(spr.BYMY, 0, CUCU);
  bool AFYX_Q = AFYX.q(); bool BAZU = not(!AFYX_Q); AFYX.tock(spr.BYMY, 0, WENU);

  if (spr.DENY) {
    WEZA = ADYB; WUCO = APOB; WYDA = APYV; ZYSU = AFEN; WYSE = AKYH; WUZY = APOC;
    CUCA = BUJY; CEGA = BOSO; CUCU = AHAC; WENU = BAZU;
  }

  // Sprite store 2 of 10
  bool GECU_Q = GECU.q(); bool WOCY = not(!GECU_Q); GECU.tock(spr.WUSE, 0, WEZA);
  bool FOXY_Q = FOXY.q(); bool ELYC = not(!FOXY_Q); FOXY.tock(spr.WUSE, 0, WUCO);
  bool GOHU_Q = GOHU.q(); bool WABO = not(!GOHU_Q); GOHU.tock(spr.WUSE, 0, WYDA);
  bool FOGO_Q = FOGO.q(); bool EZOC = not(!FOGO_Q); FOGO.tock(spr.WUSE, 0, ZYSU);
  bool GACY_Q = GACY.q(); bool WYWY = not(!GACY_Q); GACY.tock(spr.WUSE, 0, WYSE);
  bool GABO_Q = GABO.q(); bool WATO = not(!GABO_Q); GABO.tock(spr.WUSE, 0, WUZY);
  bool ZUBE_Q = ZUBE.q(); bool ZUDO = not(!ZUBE_Q); ZUBE.tock(spr.ZURU, 0, CUCA);
  bool ZUMY_Q = ZUMY.q(); bool YBUK = not(!ZUMY_Q); ZUMY.tock(spr.ZURU, 0, CEGA);
  bool ZEXO_Q = ZEXO.q(); bool ZYTO = not(!ZEXO_Q); ZEXO.tock(spr.ZURU, 0, CUCU);
  bool ZAFU_Q = ZAFU.q(); bool YKOZ = not(!ZAFU_Q); ZAFU.tock(spr.ZURU, 0, WENU);

  if (spr.XYME) {
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
  bool XYNU_Q = XYNU.q(); bool WAJA = not(!XYNU_Q); XYNU.tock(spr.WYLU, 0, WEZA);
  bool XEGE_Q = XEGE.q(); bool WOXY = not(!XEGE_Q); XEGE.tock(spr.WYLU, 0, WUCO);
  bool XABO_Q = XABO.q(); bool XYRE = not(!XABO_Q); XABO.tock(spr.WYLU, 0, WYDA);
  bool WANU_Q = WANU.q(); bool WERU = not(!WANU_Q); WANU.tock(spr.WYLU, 0, ZYSU);
  bool XEFE_Q = XEFE.q(); bool WEPY = not(!XEFE_Q); XEFE.tock(spr.WYLU, 0, WYSE);
  bool XAVE_Q = XAVE.q(); bool WUXU = not(!XAVE_Q); XAVE.tock(spr.WYLU, 0, WUZY);
  bool CUMU_Q = CUMU.q(); bool BYDO = not(!CUMU_Q); CUMU.tock(spr.EWOT, 0, CUCA);
  bool CAPO_Q = CAPO.q(); bool BUCE = not(!CAPO_Q); CAPO.tock(spr.EWOT, 0, CEGA);
  bool CONO_Q = CONO.q(); bool BOVE = not(!CONO_Q); CONO.tock(spr.EWOT, 0, CUCU);
  bool CAJU_Q = CAJU.q(); bool BEVY = not(!CAJU_Q); CAJU.tock(spr.EWOT, 0, WENU);

  if (spr.GOWO) {
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
  bool FUZO_Q = FUZO.q(); bool EVYT = not(!FUZO_Q); FUZO.tock(spr.FEFO, 0, WEZA);
  bool GESY_Q = GESY.q(); bool WABA = not(!GESY_Q); GESY.tock(spr.FEFO, 0, WUCO);
  bool FYSU_Q = FYSU.q(); bool ETAD = not(!FYSU_Q); FYSU.tock(spr.FEFO, 0, WYDA);
  bool FEFA_Q = FEFA.q(); bool ELEP = not(!FEFA_Q); FEFA.tock(spr.FEFO, 0, ZYSU);
  bool GYNO_Q = GYNO.q(); bool WYGO = not(!GYNO_Q); GYNO.tock(spr.FEFO, 0, WYSE);
  bool GULE_Q = GULE.q(); bool WAKO = not(!GULE_Q); GULE.tock(spr.FEFO, 0, WUZY);
  bool XYGO_Q = XYGO.q(); bool WANA = not(!XYGO_Q); XYGO.tock(spr.WABE, 0, CUCA);
  bool XYNA_Q = XYNA.q(); bool WAXE = not(!XYNA_Q); XYNA.tock(spr.WABE, 0, CEGA);
  bool XAKU_Q = XAKU.q(); bool WABU = not(!XAKU_Q); XAKU.tock(spr.WABE, 0, CUCU);
  bool YGUM_Q = YGUM.q(); bool YPOZ = not(!YGUM_Q); YGUM.tock(spr.WABE, 0, WENU);

  if (spr.GUGY) {
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
  bool CAJY_Q = CAJY.q(); bool BEMO = not(!CAJY_Q); CAJY.tock(spr.BYVY, 0, WEZA);
  bool CUZA_Q = CUZA.q(); bool CYBY = not(!CUZA_Q); CUZA.tock(spr.BYVY, 0, WUCO);
  bool COMA_Q = COMA.q(); bool BETY = not(!COMA_Q); COMA.tock(spr.BYVY, 0, WYDA);
  bool CUFA_Q = CUFA.q(); bool CEGY = not(!CUFA_Q); CUFA.tock(spr.BYVY, 0, ZYSU);
  bool CEBO_Q = CEBO.q(); bool CELU = not(!CEBO_Q); CEBO.tock(spr.BYVY, 0, WYSE);
  bool CADU_Q = CADU.q(); bool CUBO = not(!CADU_Q); CADU.tock(spr.BYVY, 0, WUZY);
  bool ABUG_Q = ABUG.q(); bool BEFE = not(!ABUG_Q); ABUG.tock(spr.AHOF, 0, CUCA);
  bool AMES_Q = AMES.q(); bool BYRO = not(!AMES_Q); AMES.tock(spr.AHOF, 0, CEGA);
  bool ABOP_Q = ABOP.q(); bool BACO = not(!ABOP_Q); ABOP.tock(spr.AHOF, 0, CUCU);
  bool AROF_Q = AROF.q(); bool AHUM = not(!AROF_Q); AROF.tock(spr.AHOF, 0, WENU);

  if (spr.DYDO) {
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
  bool EKAP_Q = EKAP.q(); bool DALO = not(!EKAP_Q); EKAP.tock(spr.DYMO, 0, WEZA);
  bool ETAV_Q = ETAV.q(); bool DALY = not(!ETAV_Q); ETAV.tock(spr.DYMO, 0, WUCO);
  bool EBEX_Q = EBEX.q(); bool DUZA = not(!EBEX_Q); EBEX.tock(spr.DYMO, 0, WYDA);
  bool GORU_Q = GORU.q(); bool WAGA = not(!GORU_Q); GORU.tock(spr.DYMO, 0, ZYSU);
  bool ETYM_Q = ETYM.q(); bool DYNY = not(!ETYM_Q); ETYM.tock(spr.DYMO, 0, WYSE);
  bool EKOP_Q = EKOP.q(); bool DOBO = not(!EKOP_Q); EKOP.tock(spr.DYMO, 0, WUZY);
  bool ANED_Q = ANED.q(); bool AWAT = not(!ANED_Q); ANED.tock(spr.BUCY, 0, CUCA);
  bool ACEP_Q = ACEP.q(); bool BACE = not(!ACEP_Q); ACEP.tock(spr.BUCY, 0, CEGA);
  bool ABUX_Q = ABUX.q(); bool BODU = not(!ABUX_Q); ABUX.tock(spr.BUCY, 0, CUCU);
  bool ABEG_Q = ABEG.q(); bool BUJA = not(!ABEG_Q); ABEG.tock(spr.BUCY, 0, WENU);

  if (spr.GYGY) {
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
  bool DAFU_Q = DAFU.q(); bool DEZU = not(!DAFU_Q); DAFU.tock(spr.FEKA, 0, WEZA);
  bool DEBA_Q = DEBA.q(); bool EFUD = not(!DEBA_Q); DEBA.tock(spr.FEKA, 0, WUCO);
  bool DUHA_Q = DUHA.q(); bool DONY = not(!DUHA_Q); DUHA.tock(spr.FEKA, 0, WYDA);
  bool DUNY_Q = DUNY.q(); bool DOWA = not(!DUNY_Q); DUNY.tock(spr.FEKA, 0, ZYSU);
  bool DESE_Q = DESE.q(); bool DYGO = not(!DESE_Q); DESE.tock(spr.FEKA, 0, WYSE);
  bool DEVY_Q = DEVY.q(); bool ENAP = not(!DEVY_Q); DEVY.tock(spr.FEKA, 0, WUZY);
  bool ZURY_Q = ZURY.q(); bool ZYPO = not(!ZURY_Q); ZURY.tock(spr.XYHA, 0, CUCA);
  bool ZURO_Q = ZURO.q(); bool ZEXE = not(!ZURO_Q); ZURO.tock(spr.XYHA, 0, CEGA);
  bool ZENE_Q = ZENE.q(); bool YJEM = not(!ZENE_Q); ZENE.tock(spr.XYHA, 0, CUCU);
  bool ZYLU_Q = ZYLU.q(); bool YWAV = not(!ZYLU_Q); ZYLU.tock(spr.XYHA, 0, WENU);

  if (spr.GYMA) {
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
  bool BOXA_Q = BOXA.q(); bool AXEC = not(!BOXA_Q); BOXA.tock(spr.BUZY, 0, WEZA);
  bool BUNA_Q = BUNA.q(); bool CYRO = not(!BUNA_Q); BUNA.tock(spr.BUZY, 0, WUCO);
  bool BULU_Q = BULU.q(); bool CUVU = not(!BULU_Q); BULU.tock(spr.BUZY, 0, WYDA);
  bool BECA_Q = BECA.q(); bool APON = not(!BECA_Q); BECA.tock(spr.BUZY, 0, ZYSU);
  bool BYHU_Q = BYHU.q(); bool AFOZ = not(!BYHU_Q); BYHU.tock(spr.BUZY, 0, WYSE);
  bool BUHE_Q = BUHE.q(); bool CUBE = not(!BUHE_Q); BUHE.tock(spr.BUZY, 0, WUZY);
  bool YKUK_Q = YKUK.q(); bool ZABY = not(!YKUK_Q); YKUK.tock(spr.FUKE, 0, CUCA);
  bool YLOV_Q = YLOV.q(); bool ZUKE = not(!YLOV_Q); YLOV.tock(spr.FUKE, 0, CEGA);
  bool XAZY_Q = XAZY.q(); bool WUXE = not(!XAZY_Q); XAZY.tock(spr.FUKE, 0, CUCU);
  bool XOSY_Q = XOSY.q(); bool WERE = not(!XOSY_Q); XOSY.tock(spr.FUKE, 0, WENU);

  if (spr.FAME) {
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
  bool XUFO_Q = XUFO.q(); bool YHAL = not(!XUFO_Q); XUFO.tock(spr.WUFA, 0, WEZA);
  bool XUTE_Q = XUTE.q(); bool YRAD = not(!XUTE_Q); XUTE.tock(spr.WUFA, 0, WUCO);
  bool XOTU_Q = XOTU.q(); bool XYRA = not(!XOTU_Q); XOTU.tock(spr.WUFA, 0, WYDA);
  bool XYFE_Q = XYFE.q(); bool YNEV = not(!XYFE_Q); XYFE.tock(spr.WUFA, 0, ZYSU);
  bool YZOR_Q = YZOR.q(); bool ZOJY = not(!YZOR_Q); YZOR.tock(spr.WUFA, 0, WYSE);
  bool YBER_Q = YBER.q(); bool ZARO = not(!YBER_Q); YBER.tock(spr.WUFA, 0, WUZY);
  bool DEWU_Q = DEWU.q(); bool CAWO = not(!DEWU_Q); DEWU.tock(spr.FAKA, 0, CUCA);
  bool CANA_Q = CANA.q(); bool BYME = not(!CANA_Q); CANA.tock(spr.FAKA, 0, CEGA);
  bool DYSY_Q = DYSY.q(); bool COHO = not(!DYSY_Q); DYSY.tock(spr.FAKA, 0, CUCU);
  bool FOFO_Q = FOFO.q(); bool GATE = not(!FOFO_Q); FOFO.tock(spr.FAKA, 0, WENU);

  if (spr.FADO) {
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
  bool YGUS_Q = YGUS.q(); bool ZETU = not(!YGUS_Q); YGUS.tock(spr.GENY, 0, WEZA);
  bool YSOK_Q = YSOK.q(); bool ZECE = not(!YSOK_Q); YSOK.tock(spr.GENY, 0, WUCO);
  bool YZEP_Q = YZEP.q(); bool ZAVE = not(!YZEP_Q); YZEP.tock(spr.GENY, 0, WYDA);
  bool WYTE_Q = WYTE.q(); bool WOKO = not(!WYTE_Q); WYTE.tock(spr.GENY, 0, ZYSU);
  bool ZONY_Q = ZONY.q(); bool ZUMU = not(!ZONY_Q); ZONY.tock(spr.GENY, 0, WYSE);
  bool YWAK_Q = YWAK.q(); bool ZEDY = not(!YWAK_Q); YWAK.tock(spr.GENY, 0, WUZY);
  bool FYHY_Q = FYHY.q(); bool GOFO = not(!FYHY_Q); FYHY.tock(spr.ENOB, 0, CUCA);
  bool GYHO_Q = GYHO.q(); bool WEHE = not(!GYHO_Q); GYHO.tock(spr.ENOB, 0, CEGA);
  bool BOZU_Q = BOZU.q(); bool AJAL = not(!BOZU_Q); BOZU.tock(spr.ENOB, 0, CUCU);
  bool CUFO_Q = CUFO.q(); bool BUKY = not(!CUFO_Q); CUFO.tock(spr.ENOB, 0, WENU);

  if (spr.FURO) {
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