#include "Window.h"

#include "Clocks.h"
#include "PPU.h"
#include "VramBus.h"
#include "Sprites.h"
#include "Window.h"
#include "LCD.h"

//-----------------------------------------------------------------------------

void Window::tick(const Resets& rst, const LCD& lcd, Vram& vram, const Clocks& clk) {
  //----------
  // Window Y match

  wire NAZE = xor(ppu.WY_D0, ppu.V0);
  wire PEBO = xor(ppu.WY_D1, ppu.V1);
  wire POMO = xor(ppu.WY_D2, ppu.V2);
  wire NEVU = xor(ppu.WY_D3, ppu.V3);
  wire NOJO = xor(ppu.WY_D4, ppu.V4);
  wire PAGA = xor(ppu.WY_D5, ppu.V5);
  wire PEZO = xor(ppu.WY_D6, ppu.V6);
  wire NUPA = xor(ppu.WY_D7, ppu.V7);

  // Either the schematic should have a nor instead of a nand, or i've got the polarity of the y counter flipped

  /*
  wire PALO = nand(ppu.LCDC_WINEN, NOJO, PAGA, PEZO, NUPA);
  wire NELE = not(PALO);
  wire PAFU = nand(NELE, NAZE, PEBO, POMO, NEVU);
  wire ROGE = not(PAFU);
  */

  wire match = and(ppu.LCDC_WINEN, nor(NAZE, PEBO, POMO, NEVU, NOJO, PAGA, PEZO, NUPA));

  wire WIN_Y_MATCHQ = WIN_Y_MATCH.tock(ppu.TALU, rst.RESET_VIDEO, match);

  //----------
  // Window X match

  wire MYLO = xor(ppu.WX_D0, ppu.X_R0.q());
  wire PUWU = xor(ppu.WX_D1, ppu.X_R1.q());
  wire PUHO = xor(ppu.WX_D2, ppu.X_R2.q());
  wire NYTU = xor(ppu.WX_D3, ppu.X_R3.q());
  wire NEZO = xor(ppu.WX_D4, ppu.X_R4.q());
  wire NORY = xor(ppu.WX_D5, ppu.X_R5.q());
  wire NONO = xor(ppu.WX_D6, ppu.X_R6.q());
  wire PASE = xor(ppu.WX_D7, ppu.X_R7.q());

  // unk2 is probably A && !B?
  wire REJO = unk2(WIN_Y_MATCHQ, or(ppu.INT_VBL, not(rst.RESET_VIDEO)));
  
  // polarity looks wrong
  wire WIN_X_MATCH_NEW = and(MYLO, PUWU, PUHO, NYTU, NEZO, NORY, NONO, PASE, REJO); 

  //----------
  // something to do with fine x scroll?

  bool NOPA_Q = NOPA.q();
  bool WIN_X_MATCHQ = WIN_X_MATCH.q();
  bool WIN_X_MATCH_DELAYQ = WIN_X_MATCH_DELAY.q();
  bool RYFA_Q = RYFA.q();
  bool SOVY_Q = SOVY.q();
  bool RENE_Q = RENE.q();

  bool RYKU_Q = RYKU.q();
  bool ROGA_Q = ROGA.q();
  bool RUBU_Q = RUBU.q();

  bool NYZE_Q = NYZE.q();
  bool PUXA_Q = PUXA.q();

  POVA = and(!NYZE_Q, PUXA_Q);
  ROXY = unk2(not(ppu.XYMU), POVA);

  // maybe this triggers pixel emitting when fine x matches?
  wire SUHA = xor(ppu.SCX_D0, RYKU_Q);
  wire SYBY = xor(ppu.SCX_D1, ROGA_Q);
  wire SOZU = xor(ppu.SCX_D2, RUBU_Q);
  wire POHU = and(ROXY, SUHA, SYBY, SOZU);

  wire ROZE = nand(RYKU_Q, ROGA_Q, RUBU_Q);
  wire PECU = nand(lcd.ROXO, ROZE);
  wire MOXE = not(clk.CLK_0246);
  wire PANY = nor(WIN_X_MATCH_NEW, ROZE);
  wire SEKO = nor(RENE_Q, !RYFA_Q);
  wire ROMO = not(lcd.POKY);
  wire TAVE = and(ppu.XYMU, ROMO, lcd.NYKA.q(), lcd.PORY.q());
  wire XOFO = nand(ppu.LCDC_WINEN, not(spr.ATEJ), rst.RESET_VIDEO);
  wire XACO = not(XOFO);
  wire PYNU = unk2(WIN_X_MATCH_DELAYQ, XOFO);
  wire NUNY = and(!NOPA_Q, PYNU);
  PORE = PYNU;

  // glitch filter loop, FIXME double check if this logic is correct
  // bool PUKU = nor(NUNY, RYDY);
  // bool RYDY = nor(PUKU, not(rst.RESET_VIDEO), PORY);
  wire RYDY = NUNY && !(not(rst.RESET_VIDEO) || lcd.PORY.q());

  SYLO = not(RYDY);

  wire TEVO = nor(SEKO, not(nand(SOVY_Q, not(RYDY))), TAVE);
  wire PASO = nor(TEVO, not(ppu.XYMU));
  MOSU = NUNY;
  NYXU = nor(spr.AVAP, NUNY, TEVO);

  NOPA.tock(clk.CLK_0246, rst.RESET_VIDEO, PYNU);
  WIN_X_MATCH.tock(not(lcd.SEGU), rst.RESET_VIDEO, WIN_X_MATCH_NEW);
  WIN_X_MATCH_DELAY.tock(not(clk.CLK_0246), rst.RESET_VIDEO, WIN_X_MATCHQ);

  //----------
  // dunno, top right

  bool LYZU_Q = LYZU.q();
  bool LOVY_Q = LOVY.q();

  bool LAXU_Q = LAXU.q();
  bool MESU_Q = MESU.q();
  bool NYVA_Q = NYVA.q();

  MYVO = not(clk.CLK_0246);
  LYRY = not(nand(LAXU_Q, NYVA_Q, NYXU));
  wire MYSO = nor(lcd.LOBY, not(LAXU_Q), LYZU_Q);
  MOFU = and(MYSO, not(MESU_Q));
  NYDY = nand(MYSO, MESU_Q, not(NYVA_Q));
  XUHA = NYVA_Q;
  
  wire LURY = and(!LOVY_Q, ppu.XYMU);
  wire LONY = unk2(LURY, !LOVY_Q);
  
  MYMA = not(LONY);
  LENA = LONY;

  NETA = and(LONY, nand(not(MESU_Q), not(NYVA_Q)));
  POTU = and(LONY, not(nand(not(MESU_Q), not(NYVA_Q))));

  LAXU.flip(nand(clk.CLK_0246, nand(LAXU_Q, NYVA_Q, NYXU)),    NYXU);
  MESU.flip(!LAXU_Q, NYXU);
  NYVA.flip(!MESU_Q, NYXU);

  LYZU.tock(clk.CLK_0246, ppu.XYMU, LAXU_Q);

  LOVY.tock(MYVO,  NYXU, LYRY);

  //----------
  // address output bus

  wire VETU = and(TEVO, PORE);
  wire WYKA_Q = WYKA.flip(   VETU, XACO);
  wire WODY_Q = WODY.flip(!WYKA_Q, XACO);
  wire WOBO_Q = WOBO.flip(!WODY_Q, XACO);
  wire WYKO_Q = WYKO.flip(!WOBO_Q, XACO);
  wire XOLO_Q = XOLO.flip(!WYKO_Q, XACO);

  wire SYNY = nor(ppu.INT_VBL, not(rst.RESET_VIDEO));

  wire VYNO_Q = VYNO.flip(not(PORE), SYNY);
  wire VUJO_Q = VUJO.flip(!VYNO_Q,   SYNY);
  wire VYMU_Q = VYMU.flip(!VUJO_Q,   SYNY);
  wire TUFU_Q = TUFU.flip(!VYMU_Q,   SYNY);
  wire TAXA_Q = TAXA.flip(!TUFU_Q,   SYNY);
  wire TOZO_Q = TOZO.flip(!TAXA_Q,   SYNY);
  wire TATE_Q = TATE.flip(!TOZO_Q,   SYNY);
  wire TEKE_Q = TEKE.flip(!TATE_Q,   SYNY);

  VYPO = not(CUBA1);

  bool XEZE = nand(POTU, PORE);
  bool WUKO = not(XEZE);
  if (WUKO) {
    vram.MA0 = WYKA_Q;
    vram.MA1 = WODY_Q;
    vram.MA2 = WOBO_Q;
    vram.MA3 = WYKO_Q;
    vram.MA4 = XOLO_Q;
    vram.MA5 = TUFU_Q;
    vram.MA6 = TAXA_Q;
    vram.MA7 = TOZO_Q;
    vram.MA8 = TATE_Q;
    vram.MA9 = TEKE_Q;
    vram.MA10 = not(ppu.LCDC_WINMAP);
    vram.MA11 = CUBA1;
    vram.MA12 = CUBA1;
  }

  //----------
  // computes SECA and TAVA

  wire SOBU_Q = SOBU.q();
  wire SUDA_Q = SUDA.q();

  SECA = nor(and(SOBU_Q, !SUDA_Q), not(rst.RESET_VIDEO), spr.ATEJ);

  //----------
  // registers

  wire TAKA = unk2(nor(spr.WUTY, TAVE), nor(and(SOBU_Q, !SUDA_Q), not(rst.RESET_VIDEO), spr.ATEJ));
  
  SOBU.tock(clk.CLK_0246,      VYPO, and(spr.FEPO, not(lcd.TOMU), not(nand(LAXU_Q, NYVA_Q, NYXU)), not(TAKA)));
  SUDA.tock(not(clk.CLK_0246), VYPO, SOBU_Q);

  RYKU.flip(PECU, PASO);
  ROGA.flip(!RYKU_Q, PASO);
  RUBU.flip(!ROGA_Q, PASO);

  NYZE.tock(MOXE, ppu.XYMU, PUXA_Q);
  PUXA.tock(lcd.ROXO, ppu.XYMU, POHU);

  RYFA.tock(lcd.SEGU, ppu.XYMU, PANY);
  RENE.tock(clk.CLK_0246, ppu.XYMU, RYFA_Q);
  SOVY.tock(clk.CLK_0246, rst.RESET_VIDEO, RYDY);
}