#include "PPU.h"

#include "Clocks.h"
#include "CpuBus.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "AddressDecoder.h"
#include "Sprites.h"
#include "Window.h"
#include "LCD.h"
#include "Background.h"

struct PPU ppu;

//-----------------------------------------------------------------------------

void PPU::tick_videocontrol(const Resets& rst, const AddressDecoder& dec, const Window& win, const LCD& lcd) {

  LYHA = not(rst.RESET_VIDEO);
  LYFE = not(LYHA);

  //----------
  // LY compare

  bool SYFU = xor(V7, LYC_D7);
  bool TERY = xor(V6, LYC_D6);
  bool TUCY = xor(V5, LYC_D5);
  bool TYKU = xor(V4, LYC_D4);
  bool RASY = xor(V3, LYC_D3);
  bool REDA = xor(V2, LYC_D2);
  bool TYDE = xor(V1, LYC_D1);
  bool RYME = xor(V0, LYC_D0);

  bool SOVU = nor(SYFU, TERY, TUCY, TYKU);
  bool SUBO = nor(RASY, REDA, TYDE, RYME);
  bool RAPE = nand(SOVU, SUBO);
  PALY = not(RAPE);

  //----------
  // top center

  bool WUVU_Q = spr.WUVU.q();
  bool VENA_Q = VENA.flip(!WUVU_Q, rst.RESET_VIDEO);
  bool RUTU_Q = RUTU.q();
  bool SYGU_Q = SYGU.q();

  bool MUDE = nor(RUTU_Q, LYHA); // schematic says RUTU_OUT, but I think this is just RUTU_Q?
  TALU = not(!VENA_Q);

  bool SAXO_Q = SAXO.flip(TALU,    MUDE);
  bool TYPO_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool VYZO_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TELU_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool SUDE_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TAHA_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TYRY_Q = TYPO.flip(!SAXO_Q, MUDE);


  bool TOCU = not(SAXO_Q);
  bool VEPE = not(TYPO_Q);
  bool VUTY = not(VYZO_Q);
  bool TUJU = not(TELU_Q);
  bool TAFY = not(SUDE_Q);
  bool TUDA = not(TAHA_Q);
  bool VATE = not(TYRY_Q);

  bool VOKU = nand(TUJU,   TAFY,   TUDA,   VATE,   VUTY,   VEPE,   TOCU);
  bool TOZU = nand(TUJU,   TAFY,   TUDA,   VATE,   VYZO_Q, TYPO_Q, SAXO_Q);
  bool TECE = nand(TUJU,   TAHA_Q, TUDA,   TELU_Q, VYZO_Q, VEPE,   SAXO_Q);
  bool TEBO = nand(TYRY_Q, TAFY,   SUDE_Q, VATE,   VUTY,   TYPO_Q, SAXO_Q);

  bool TEGY = nand(VOKU, TOZU, TECE, TEBO);
  bool SANU = nand(TYRY_Q, TAHA_Q, SUDE_Q, SAXO_Q);

  bool SONO = not(TALU);

  bool RYNO = or(SYGU_Q, RUTU_Q);
  bool POGU = not(RYNO);
  ext.PIN_CPG = POGU;

  RUTU.tock(SONO, LYFE, SANU);
  SYGU.tock(SONO, LYFE, TEGY);

  //----------
  // x counter

  bool TADY = nor(spr.ATEJ, lcd.TOFU);

  bool X_Q0 = X_R0.q();
  bool X_Q1 = X_R1.q();
  bool X_Q2 = X_R2.q();
  bool X_Q3 = X_R3.q();
  bool X_Q4 = X_R4.q();
  bool X_Q5 = X_R5.q();
  bool X_Q6 = X_R6.q();
  bool X_Q7 = X_R7.q();

  X_R0.tock(clk.CLKPIPE, TADY, !X_Q0);
  X_R1.tock(clk.CLKPIPE, TADY, xor(X_Q1, X_Q0));
  X_R2.tock(clk.CLKPIPE, TADY, xor(X_Q2, and(X_Q0, X_Q1)));
  X_R3.tock(clk.CLKPIPE, TADY, xor(X_Q3, and(X_Q2, and(X_Q0, X_Q1))));

  bool TOCA = not(X_Q3);

  X_R4.tock(TOCA, TADY, !X_Q4);
  X_R5.tock(TOCA, TADY, xor(X_Q5, X_Q4));
  X_R6.tock(TOCA, TADY, xor(X_Q6, and(X_Q5, X_Q4)));
  X_R7.tock(TOCA, TADY, xor(X_Q7, and(X_Q6, and(X_Q5, X_Q4))));

  X0 = not(X_Q0);
  X1 = not(X_Q1);
  X2 = not(X_Q2);
  X3 = not(X_Q3);
  X4 = not(X_Q4);
  X5 = not(X_Q5);
  X6 = not(X_Q6);
  X7 = not(X_Q7);

  //----------
  // FF41 STAT

  XYVO = and(V4, V7);

  bool NYPE_Q = NYPE.tock(TALU,   LYFE, RUTU_Q); // RUTU_OUT on the schematic?
  bool POPU_Q = POPU.tock(NYPE_Q, LYFE, XYVO);
  NAPO.flip(POPU_Q, LYFE);

  bool XUGU = nand(X_Q0, X_Q1, X_Q2, X_Q5, X_Q7);
  bool XENA = not(spr.FEPO);
  bool XANO = not(XUGU);
  bool PARU = not(!POPU_Q);
  INT_VBL = PARU;
  WODU = and(XENA, XANO);

  PURE = not(RUTU_Q);
  SELA = not(PURE);
  bool TOLU = not(PARU);
  bool TAPA = and(SELA, TOLU);
  INT_OAM = TAPA;
  bool TARU = and(TOLU, WODU);
  INT_HBL = TARU;
  bool VYPU = not(TOLU);
  cpu.INT_VBL_BUF = VYPU;

  //---

  bool SEPA = and(cpu.CPU_WR2, dec.FF41);

  bool VOGA_Q = VOGA.tock(clk.CLK2, TADY, WODU);
  bool WEGO = or(lcd.TOFU, VOGA_Q);
  bool XAJO = and(X_Q0, X_Q3);
  XYMU = unk2(WEGO, spr.AVAP);
  bool SADU = nor(XYMU, PARU);
  bool XATY = nor(spr.ACYL, XYMU);
  bool PAGO = or(rst.WESY, not(SEPA));

  //---

  bool RYVE = not(SEPA);
  bool RUGU_Q = RUGU.tock(RYVE, rst.WESY, mem.D6);
  bool REFE_Q = REFE.tock(RYVE, rst.WESY, mem.D5);
  bool ROPO_Q = ROPO.tock(TALU, rst.WESY, PALY); // this seems odd
  bool RUFO_Q = RUFO.tock(RYVE, rst.WESY, mem.D4);
  bool ROXE_Q = ROXE.tock(RYVE, rst.WESY, mem.D3);

  bool PUZO = not(!ROXE_Q);
  bool SASY = not(!REFE_Q);
  bool POFO = not(!RUFO_Q);
  bool POTE = not(!RUGU_Q);
  bool TEBY = not(SADU); // these two are the STAT mode signal
  bool WUGA = not(XATY);

  bool SUKO = amux4(RUGU_Q, ROPO_Q, REFE_Q, INT_OAM, RUFO_Q, INT_VBL, ROXE_Q, INT_HBL);
  bool TUVA = not(SUKO);
  bool VOTY = not(TUVA);
  bool RUPO = unk2(ROPO_Q, PAGO);
  bool SEGO = not(RUPO);

  // LCD clock pin output
  bool WUSA = unk2(XAJO, WEGO);
  bool TOBA = and(clk.CLKPIPE, WUSA);
  ext.CP = nor(TOBA, win.POVA);


  cpu.INT_STAT = VOTY;

  bool TOBE = and(cpu.CPU_RD2, dec.FF41);
  bool VAVE = TOBE; // buffer, not inverter
  if (TOBE) {
    mem.D0 = TEBY;
    mem.D1 = WUGA;
    mem.D2 = SEGO;
  }
  if (VAVE) {
    mem.D3 = PUZO;
    mem.D4 = POFO;
    mem.D5 = SASY;
    mem.D6 = POTE;
  }

  //----------
  // y counter

  bool NOKO = and(V7, V4, V0, V1);
  bool MYTA_Q = MYTA.tock(NYPE_Q, LYFE, NOKO);
  bool LAMA = nor(MYTA_Q, LYHA);

  V0 = Y_R0.flip(RUTU_Q, LAMA);;
  V1 = Y_R1.flip(!V0,    LAMA);;
  V2 = Y_R2.flip(!V1,    LAMA);;
  V3 = Y_R3.flip(!V2,    LAMA);;
  V4 = Y_R4.flip(!V3,    LAMA);;
  V5 = Y_R5.flip(!V4,    LAMA);;
  V6 = Y_R6.flip(!V5,    LAMA);;
  V7 = Y_R7.flip(!V6,    LAMA);;
}

//-----------------------------------------------------------------------------

void PPU::tick_videoregs(const Resets& rst, const AddressDecoder& dec) {
  // FF40 LCDC
  bool WARU = and(dec.FF40, cpu.CPU_WR2);
  bool XUBO = not(WARU);
  LCDC_BGEN    = BGEN   .tock(XUBO, rst.XARE, mem.D0);
  LCDC_SPREN   = SPREN  .tock(XUBO, rst.XARE, mem.D1);
  LCDC_SPRSIZE = SPRSIZE.tock(XUBO, rst.XARE, mem.D2);
  LCDC_BGMAP   = BGMAP  .tock(XUBO, rst.XARE, mem.D3);
  LCDC_BGTILE  = BGTILE .tock(XUBO, rst.XARE, mem.D4);
  LCDC_WINEN   = WINEN  .tock(XUBO, rst.XARE, mem.D5);
  LCDC_WINMAP  = WINMAP .tock(XUBO, rst.XARE, mem.D6);
  LCDC_LCDEN   = LCDEN  .tock(XUBO, rst.XARE, mem.D7);

  if (and(dec.FF40, cpu.CPU_RD2)) {
    mem.D0 = LCDC_BGEN;
    mem.D1 = LCDC_SPREN;
    mem.D2 = LCDC_SPRSIZE;
    mem.D3 = LCDC_BGMAP;
    mem.D4 = LCDC_BGTILE;
    mem.D5 = LCDC_WINEN;
    mem.D6 = LCDC_WINMAP;
    mem.D7 = LCDC_LCDEN;
  }

  // FF4B WX
  bool WUZA = and(cpu.CPU_WR2, dec.FF4B);
  bool VOXU = not(WUZA);
  WX_D0 = WX_R0.tock(VOXU, rst.RESET8, mem.D0);
  WX_D1 = WX_R1.tock(VOXU, rst.RESET8, mem.D1);
  WX_D2 = WX_R2.tock(VOXU, rst.RESET8, mem.D2);
  WX_D3 = WX_R3.tock(VOXU, rst.RESET8, mem.D3);
  WX_D4 = WX_R4.tock(VOXU, rst.RESET8, mem.D4);
  WX_D5 = WX_R5.tock(VOXU, rst.RESET8, mem.D5);
  WX_D6 = WX_R6.tock(VOXU, rst.RESET8, mem.D6);
  WX_D7 = WX_R7.tock(VOXU, rst.RESET8, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF4B)) {
    mem.D0 = WX_D0;
    mem.D1 = WX_D1;
    mem.D2 = WX_D2;
    mem.D3 = WX_D3;
    mem.D4 = WX_D4;
    mem.D5 = WX_D5;
    mem.D6 = WX_D6;
    mem.D7 = WX_D7;
  }

  // FF4A WY
  bool WEKO = and(cpu.CPU_WR2, dec.FF4A);
  bool VEFU = not(WEKO);
  WY_D0 = NESO.tock(VEFU, rst.RESET8, mem.D0);
  WY_D1 = NYRO.tock(VEFU, rst.RESET8, mem.D1);
  WY_D2 = NAGA.tock(VEFU, rst.RESET8, mem.D2);
  WY_D3 = MELA.tock(VEFU, rst.RESET8, mem.D3);
  WY_D4 = NULO.tock(VEFU, rst.RESET8, mem.D4);
  WY_D5 = NENE.tock(VEFU, rst.RESET8, mem.D5);
  WY_D6 = NUKA.tock(VEFU, rst.RESET8, mem.D6);
  WY_D7 = NAFU.tock(VEFU, rst.RESET8, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF4A)) {
    mem.D0 = WY_D0;
    mem.D1 = WY_D1;
    mem.D2 = WY_D2;
    mem.D3 = WY_D3;
    mem.D4 = WY_D4;
    mem.D5 = WY_D5;
    mem.D6 = WY_D6;
    mem.D7 = WY_D7;
  }

  // FF43 SCX
  bool ARUR = and(dec.FF43, cpu.CPU_WR2);
  bool AMUN = not(ARUR);

  SCX_D0 = DATY.tock(AMUN, rst.RESET6, mem.D0);
  SCX_D1 = DUZU.tock(AMUN, rst.RESET6, mem.D1);
  SCX_D2 = CYXU.tock(AMUN, rst.RESET6, mem.D2);
  SCX_D3 = GUBO.tock(AMUN, rst.RESET6, mem.D3);
  SCX_D4 = BEMY.tock(AMUN, rst.RESET6, mem.D4);
  SCX_D5 = CUZY.tock(AMUN, rst.RESET6, mem.D5);
  SCX_D6 = CABU.tock(AMUN, rst.RESET6, mem.D6);
  SCX_D7 = BAKE.tock(AMUN, rst.RESET6, mem.D7);

  if (and(dec.FF43, cpu.CPU_RD2)) {
    mem.D0 = SCX_D0;
    mem.D1 = SCX_D1;
    mem.D2 = SCX_D2;
    mem.D3 = SCX_D3;
    mem.D4 = SCX_D4;
    mem.D5 = SCX_D5;
    mem.D6 = SCX_D6;
    mem.D7 = SCX_D7;
  }

  // FF42 SCY
  bool BEDY = and(cpu.CPU_WR2, dec.FF42);
  bool CAVO = not(BEDY);

  SCY_D0 = SCY_R0.tock(CAVO, rst.RESET6, mem.D0);
  SCY_D1 = SCY_R1.tock(CAVO, rst.RESET6, mem.D1);
  SCY_D2 = SCY_R2.tock(CAVO, rst.RESET6, mem.D2);
  SCY_D3 = SCY_R3.tock(CAVO, rst.RESET6, mem.D3);
  SCY_D4 = SCY_R4.tock(CAVO, rst.RESET6, mem.D4);
  SCY_D5 = SCY_R5.tock(CAVO, rst.RESET6, mem.D5);
  SCY_D6 = SCY_R6.tock(CAVO, rst.RESET6, mem.D6);
  SCY_D7 = SCY_R7.tock(CAVO, rst.RESET6, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF42)) {
    mem.D0 = SCY_D0;
    mem.D1 = SCY_D1;
    mem.D2 = SCY_D2;
    mem.D3 = SCY_D3;
    mem.D4 = SCY_D4;
    mem.D5 = SCY_D5;
    mem.D6 = SCY_D6;
    mem.D7 = SCY_D7;
  }

  // FF45 LYC
  bool XUFA = and(cpu.CPU_WR2, dec.FF45);
  bool WANE = not(XUFA);

  LYC_D0 = SYRY.tock(WANE, rst.RESET9, mem.D0);
  LYC_D1 = VUCE.tock(WANE, rst.RESET9, mem.D1);
  LYC_D2 = SEDY.tock(WANE, rst.RESET9, mem.D2);
  LYC_D3 = SALO.tock(WANE, rst.RESET9, mem.D3);
  LYC_D4 = SOTA.tock(WANE, rst.RESET9, mem.D4);
  LYC_D5 = VAFA.tock(WANE, rst.RESET9, mem.D5);
  LYC_D6 = VEVO.tock(WANE, rst.RESET9, mem.D6);
  LYC_D7 = RAHA.tock(WANE, rst.RESET9, mem.D7);

  if (and(cpu.CPU_RD2, dec.FF45)) {
    mem.D0 = LYC_D0;
    mem.D1 = LYC_D1;
    mem.D2 = LYC_D2;
    mem.D3 = LYC_D3;
    mem.D4 = LYC_D4;
    mem.D5 = LYC_D5;
    mem.D6 = LYC_D6;
    mem.D7 = LYC_D7;
  }

  // FF44 LY
  if (and(cpu.CPU_RD2, dec.FF44)) {
    mem.D0 = V0;
    mem.D1 = V1;
    mem.D2 = V2;
    mem.D3 = V3;
    mem.D4 = V4;
    mem.D5 = V5;
    mem.D6 = V6;
    mem.D7 = V7;
  }
}

//-----------------------------------------------------------------------------

void PPU::tick_palettes(const AddressDecoder& dec) {
  // BGP
  bool VUSO = and(cpu.CPU_RD2, dec.FF47);
  bool VELY = and(cpu.CPU_WR2, dec.FF47);
  bool TEPY = not(VUSO);
  bool TEPO = not(VELY);

  bool MENA_Q = MENA.tock(TEPO, 0, mem.D7);
  bool MORU_Q = MORU.tock(TEPO, 0, mem.D5);
  bool MAXY_Q = MAXY.tock(TEPO, 0, mem.D3);
  bool NUSY_Q = NUSY.tock(TEPO, 0, mem.D1);
  bool MOGY_Q = MOGY.tock(TEPO, 0, mem.D6);
  bool MUKE_Q = MUKE.tock(TEPO, 0, mem.D4);
  bool PYLU_Q = PYLU.tock(TEPO, 0, mem.D2);
  bool PAVO_Q = PAVO.tock(TEPO, 0, mem.D0);

  BGP_D7 = MENA_Q;
  BGP_D5 = MORU_Q;
  BGP_D3 = MAXY_Q;
  BGP_D1 = NUSY_Q;
  BGP_D6 = MOGY_Q;
  BGP_D4 = MUKE_Q;
  BGP_D2 = PYLU_Q;
  BGP_D0 = PAVO_Q;

  bool LARY = not(!MENA_Q);
  bool LYKA = not(!MORU_Q);
  bool LOBE = not(!MAXY_Q);
  bool PABA = not(!NUSY_Q);
  bool LODY = not(!MOGY_Q);
  bool LACE = not(!MUKE_Q);
  bool REDO = not(!PYLU_Q);
  bool RARO = not(!PAVO_Q);

  if (TEPY) {
    mem.D7 = LARY;
    mem.D5 = LYKA;
    mem.D3 = LOBE;
    mem.D1 = PABA;
    mem.D6 = LODY;
    mem.D4 = LACE;
    mem.D2 = REDO;
    mem.D0 = RARO;
  }

  // OBP0
  bool XUFY = and(cpu.CPU_RD2, dec.FF48);
  bool XOMA = and(cpu.CPU_WR2, dec.FF48);
  bool XOZY = not(XUFY);
  bool XELO = not(XOMA);

  bool XUFU_Q = OBP0_R0.tock(XELO, 0, mem.D0);
  bool XUKY_Q = OBP0_R1.tock(XELO, 0, mem.D1);
  bool XOVA_Q = OBP0_R2.tock(XELO, 0, mem.D2);
  bool XALO_Q = OBP0_R3.tock(XELO, 0, mem.D3);
  bool XERU_Q = OBP0_R4.tock(XELO, 0, mem.D4);
  bool XYZE_Q = OBP0_R5.tock(XELO, 0, mem.D5);
  bool XUPO_Q = OBP0_R6.tock(XELO, 0, mem.D6);
  bool XANA_Q = OBP0_R7.tock(XELO, 0, mem.D7);

  OBP0_D7 = XANA_Q;
  OBP0_D5 = XYZE_Q;
  OBP0_D3 = XALO_Q;
  OBP0_D1 = XUKY_Q;
  OBP0_D6 = XUPO_Q;
  OBP0_D4 = XERU_Q;
  OBP0_D2 = XOVA_Q;
  OBP0_D0 = XUFU_Q;

  bool XAWO = not(!XANA_Q);
  bool XOBO = not(!XYZE_Q);
  bool XUBY = not(!XALO_Q);
  bool XOKE = not(!XUKY_Q);
  bool XAXA = not(!XUPO_Q);
  bool XAJU = not(!XERU_Q);
  bool XUNO = not(!XOVA_Q);
  bool XARY = not(!XUFU_Q);

  if (XOZY) {
    mem.D7 = XAWO;
    mem.D5 = XOBO;
    mem.D3 = XUBY;
    mem.D1 = XOKE;
    mem.D6 = XAXA;
    mem.D4 = XAJU;
    mem.D2 = XUNO;
    mem.D0 = XARY;
  }

  // OBP1
  bool MUMY = and(cpu.CPU_RD2, dec.FF49);
  bool MYXE = and(cpu.CPU_WR2, dec.FF49);
  bool LOTE = not(MUMY);
  bool LEHO = not(MYXE);

  bool LUXO_Q = LUXO.tock(LEHO, 0, mem.D7);
  bool LUGU_Q = LUGU.tock(LEHO, 0, mem.D5);
  bool LOSE_Q = LOSE.tock(LEHO, 0, mem.D3);
  bool LAWO_Q = LAWO.tock(LEHO, 0, mem.D1);
  bool LEPU_Q = LEPU.tock(LEHO, 0, mem.D6);
  bool LUNE_Q = LUNE.tock(LEHO, 0, mem.D4);
  bool MOSA_Q = MOSA.tock(LEHO, 0, mem.D2);
  bool MOXY_Q = MOXY.tock(LEHO, 0, mem.D0);

  OBP1_D7 = LUXO_Q;
  OBP1_D5 = LUGU_Q;
  OBP1_D3 = LOSE_Q;
  OBP1_D1 = LAWO_Q;
  OBP1_D6 = LEPU_Q;
  OBP1_D4 = LUNE_Q;
  OBP1_D2 = MOSA_Q;
  OBP1_D0 = MOXY_Q;

  bool LELU = not(!LUXO_Q);
  bool LUGA = not(!LUGU_Q);
  bool LYZA = not(!LOSE_Q);
  bool LEPA = not(!LAWO_Q);
  bool LEBA = not(!LEPU_Q);
  bool LUKY = not(!LUNE_Q);
  bool LODE = not(!MOSA_Q);
  bool LAJU = not(!MOXY_Q);

  if (LOTE) {
    mem.D7 = LELU;
    mem.D5 = LUGA;
    mem.D3 = LYZA;
    mem.D1 = LEPA;
    mem.D6 = LEBA;
    mem.D4 = LUKY;
    mem.D2 = LODE;
    mem.D0 = LAJU;
  }
}

//-----------------------------------------------------------------------------

void PPU::tick_pixelmux(const Background& bg) {
  bool WOXA = and(LCDC_SPREN, SPR_PIX_A_7);
  bool XULA = and(LCDC_SPREN, SPR_PIX_B_7);
  bool TADE = and(LCDC_BGEN, BG_PIX_B_7);
  bool RAJY = and(LCDC_BGEN, BG_PIX_A_7);

  bool WELE = not(XULA);
  bool VUMU = not(WOXA);
  bool NULY = nor(WOXA, XULA);
  bool RUTA = and(TADE, bg.VAVA3);
  bool RYFU = and(RAJY, bg.VAVA3);
  bool POKA = nor(NULY, RUTA, RYFU);
  BGPIXELn = POKA;

  bool LAVA = not(OBP0PIXELn);
  bool WOLO = not(WELE);
  bool WYRU = not(VUMU);

  bool VYRO = and(WYRU, WOLO, LAVA);
  bool VATA = and(WYRU, WELE, LAVA);
  bool VOLO = and(VUMU, WOLO, LAVA);
  bool VUGO = and(VUMU, WELE, LAVA);

  bool WUFU = amux4(OBP0_D7, VYRO, OBP0_D5, VATA, OBP0_D3, VOLO, OBP0_D1, VUGO);
  bool WALY = amux4(VYRO, OBP0_D6, VATA, OBP0_D4, VOLO, OBP0_D2, VUGO, OBP0_D0);

  bool MABY = not(XULA);
  bool LUKU = not(OBP1PIXELn);
  bool MEXA = not(WOXA);
  bool LOZO = not(MEXA);
  bool LYLE = not(MABY);
  bool LEDO = and(LOZO, LYLE, LUKU);
  bool LYKY = and(MEXA, LYLE, LUKU);
  bool LOPU = and(MEXA, MABY, LUKU);
  bool LARU = and(LOZO, MABY, LUKU);

  bool MOKA = amux4(OBP1_D7, LEDO, OBP1_D5, LYKY, OBP1_D3, LOPU, OBP1_D1, LARU);
  bool MUFA = amux4(LEDO, OBP1_D6, LYKY, OBP1_D4, LOPU, OBP1_D2, LARU, OBP1_D0);

  bool SOBA = not(RAJY);
  bool VYCO = not(TADE);

  bool MUVE = not(BGPIXELn);
  bool NUPO = not(SOBA);
  bool NALE = not(VYCO);
  bool NYPO = and(NALE, NUPO, MUVE);
  bool NUXO = and(VYCO, NUPO, MUVE);
  bool NUMA = and(NALE, SOBA, MUVE);
  bool POBU = and(VYCO, SOBA, MUVE);
  bool NURA = amux4(BGP_D7, NYPO, BGP_D5, NUMA, BGP_D3, NUXO, BGP_D1, POBU);
  bool NELO = amux4(NYPO, BGP_D6, NUMA, BGP_D4, NUXO, BGP_D2, POBU, BGP_D0);

  bool PATY = or(NURA, WUFU, MOKA);
  bool PERO = or(NELO, WALY, MUFA);

  bool RAVO = not(PATY);
  bool REMY = not(PERO);

  ext.LD1 = RAVO;
  ext.LD0 = REMY;
}