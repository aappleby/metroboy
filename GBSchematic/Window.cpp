#include "Window.h"

#include "Clocks.h"
#include "PPU.h"
#include "VramBus.h"
#include "Sprites.h"
#include "Window.h"
#include "LCD.h"

//-----------------------------------------------------------------------------

void Window::tick(const Resets& rst, const LCD& lcd, Vram& vram) {
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

  wire SARY_Q = SARY.tock(ppu.TALU, rst.RESET_VIDEO, match);

  //----------
  // Window X match

  wire MYLO = xor(ppu.X_R0.q(), ppu.WX_D0);
  wire PUWU = xor(ppu.X_R1.q(), ppu.WX_D1);
  wire PUHO = xor(ppu.X_R2.q(), ppu.WX_D2);
  wire NYTU = xor(ppu.X_R3.q(), ppu.WX_D3);
  wire NEZO = xor(ppu.X_R4.q(), ppu.WX_D4);
  wire NORY = xor(ppu.X_R5.q(), ppu.WX_D5);
  wire NONO = xor(ppu.X_R6.q(), ppu.WX_D6);
  wire PASE = xor(ppu.X_R7.q(), ppu.WX_D7);

  wire REPU = or(ppu.INT_VBL, not(rst.RESET_VIDEO));
  wire REJO = unk2(SARY_Q, REPU);
  wire PUKY = nand(REJO, NEZO, NORY, NONO, PASE);
  wire NUFA = not(PUKY);
  wire NOGY = nand(NUFA, MYLO, PUWU, PUHO, NYTU);
  wire NUKO = not(NOGY);

  //----------
  // something to do with fine x scroll?

  bool NOPA_Q = NOPA.q();
  bool PYCO_Q = PYCO.q();
  bool NUNU_Q = NUNU.q();
  bool RYFA_Q = RYFA.q();
  bool SOVY_Q = SOVY.q();
  bool RENE_Q = RENE.q();

  bool RYKU_Q = RYKU.q();
  bool ROGA_Q = ROGA.q();
  bool RUBU_Q = RUBU.q();
  bool NYZE_Q = NYZE.q();
  bool PUXA_Q = PUXA.q();

  wire ROZE = nand(RYKU_Q, ROGA_Q, RUBU_Q);
  POVA = and(!NYZE_Q, PUXA_Q);
  wire PAHA = not(ppu.XYMU);
  ROXY = unk2(PAHA, POVA);

  wire SUHA = xor(ppu.SCX_D0, RYKU_Q);
  wire SYBY = xor(ppu.SCX_D1, ROGA_Q);
  wire SOZU = xor(ppu.SCX_D2, RUBU_Q);
  wire PECU = nand(lcd.ROXO, ROZE);
  wire RONE = nand(ROXY, SUHA, SYBY, SOZU);
  wire POHU = not(RONE);
  wire MOXE = not(clk.CLK_4M_B);
  wire PANY = nor(NUKO, ROZE);
  wire SEKO = nor(RENE_Q, !RYFA_Q);
  wire ROMO = not(lcd.POKY);
  wire SUVU = nand(ppu.XYMU, ROMO, lcd.NYKA.q(), lcd.PORY.q());
  wire TAVE = not(SUVU);
  wire XAHY = not(spr.ATEJ);
  wire XOFO = nand(ppu.LCDC_WINEN, XAHY, rst.RESET_VIDEO);
  wire XACO = not(XOFO);
  wire PYNU = unk2(NUNU_Q, XOFO);
  wire NUNY = and(!NOPA_Q, PYNU);
  wire NOCU = not(PYNU);
  PORE = not(NOCU);

  // glitch filter loop, FIXME double check if this logic is correct
  // bool PUKU = nor(NUNY, RYDY);
  // bool RYDY = nor(PUKU, RESET_VIDEOn, PORY);
  wire RYDY = NUNY && !(rst.RESET_VIDEOn || lcd.PORY.q());

  SYLO = not(RYDY);
  wire TUXY = nand(SOVY_Q, SYLO);
  wire SUZU = not(TUXY);
  wire TEVO = nor(SEKO, SUZU, TAVE);
  wire PASO = nor(TEVO, PAHA);
  wire VETU = and(TEVO, PORE);
  wire ROCO = not(lcd.SEGU);
  wire MEHE = not(clk.CLK_4M_B);
  wire NYFO = not(NUNY);
  MOSU = not(NYFO);
  NYXU = nor(spr.AVAP, MOSU, TEVO);
  wire WAZY = not(PORE);
  wire SYNY = not(REPU);

  NOPA.tock(clk.CLK_4M_B, rst.RESET_VIDEO, PYNU);
  PYCO.tock(ROCO, rst.RESET_VIDEO, NUKO);
  NUNU.tock(MEHE, rst.RESET_VIDEO, PYCO_Q);

  //----------
  // dunno, top right

  bool LYZU_Q = LYZU.q();
  bool LAXU_Q = LAXU.q();
  bool MESU_Q = MESU.q();
  bool NYVA_Q = NYVA.q();
  bool LOVY_Q = LOVY.q();

  wire MOCE = nand(LAXU_Q, NYVA_Q, NYXU);
  wire LEBO = nand(clk.CLK_4M_B, MOCE);
  MYVO = not(clk.CLK_4M_B);
  LYRY = not(MOCE);
  wire LAXE = not(LAXU_Q);
  wire MYSO = nor(lcd.LOBY, LAXE, LYZU_Q);
  wire NAKO = not(MESU_Q);
  wire NOFU = not(NYVA_Q);
  MOFU = and(MYSO, NAKO);
  wire NOGU = nand(NAKO, NOFU);
  NYDY = nand(MYSO, MESU_Q, NOFU);
  XUHA = not(NOFU);
  wire NENY = not(NOGU);
  wire LURY = and(!LOVY_Q, ppu.XYMU);
  wire LONY = unk2(LURY, !LOVY_Q);
  MYMA = not(LONY);
  wire LUSU = not(LONY);
  LENA = not(LUSU);
  POTU = and(LENA, NENY);
  NETA = and(LENA, NOGU);

  LYZU.tock(clk.CLK_4M_B,    ppu.XYMU, LAXU_Q);
  LAXU.flip(LEBO,    NYXU);
  MESU.flip(!LAXU_Q, NYXU);
  NYVA.flip(!MESU_Q, NYXU);
  LOVY.tock(MYVO,    NYXU, LYRY);

  //----------
  // address output bus

  wire WYKA_Q = WYKA.flip(   VETU, XACO);
  wire WODY_Q = WODY.flip(!WYKA_Q, XACO);
  wire WOBO_Q = WOBO.flip(!WODY_Q, XACO);
  wire WYKO_Q = WYKO.flip(!WOBO_Q, XACO);
  wire XOLO_Q = XOLO.flip(!WYKO_Q, XACO);

  wire VYNO_Q = VYNO.flip(   WAZY, SYNY);
  wire VUJO_Q = VUJO.flip(!VYNO_Q, SYNY);
  wire VYMU_Q = VYMU.flip(!VUJO_Q, SYNY);

  wire TUFU_Q = TUFU.flip(!VYMU_Q, SYNY);
  wire TAXA_Q = TAXA.flip(!TUFU_Q, SYNY);
  wire TOZO_Q = TOZO.flip(!TAXA_Q, SYNY);
  wire TATE_Q = TATE.flip(!TOZO_Q, SYNY);
  wire TEKE_Q = TEKE.flip(!TATE_Q, SYNY);

  wire XEJA = not(WYKA_Q);
  wire XAMO = not(WODY_Q);
  wire XAHE = not(WOBO_Q);
  wire XULO = not(WYKO_Q);
  wire WUJU = not(XOLO_Q);

  wire VYTO = not(TUFU_Q);
  wire VEHA = not(TAXA_Q);
  wire VACE = not(TOZO_Q);
  wire VOVO = not(TATE_Q);
  wire VULO = not(TEKE_Q);

  wire VEVY = not(ppu.LCDC_WINMAP);

  VYPO = not(CUBA1);
  wire VEZA = not(VYPO);
  wire VOGU = not(VYPO);

  bool XEZE = nand(POTU, PORE);
  bool WUKO = not(XEZE);
  if (WUKO) {
    vram.MA0 = XEJA;
    vram.MA1 = XAMO;
    vram.MA2 = XAHE;
    vram.MA3 = XULO;
    vram.MA4 = WUJU;
    vram.MA5 = VYTO;
    vram.MA6 = VEHA;
    vram.MA7 = VACE;
    vram.MA8 = VOVO;
    vram.MA9 = VULO;
    vram.MA10 = VEVY;
    vram.MA11 = VEZA;
    vram.MA12 = VOGU;
  }

  //----------
  // computes SECA and TAVA

  wire SOBU_Q = SOBU.q();
  wire SUDA_Q = SUDA.q();

  wire RYCE = and(SOBU_Q, !SUDA_Q);
  wire ROSY = not(rst.RESET_VIDEO);
  SECA = nor(RYCE, ROSY, spr.ATEJ);
  wire VEKU = nor(spr.WUTY, TAVE);
  wire TAKA = unk2(VEKU, SECA);
  wire TUKU = not(lcd.TOMU);
  wire TEKY = and(spr.FEPO, TUKU, LYRY, not(TAKA));
  TAVA = not(spr.LAPE);

  //----------
  // registers

  SOBU.tock(TAVA, VYPO, TEKY);
  SUDA.tock(spr.LAPE, VYPO, SOBU_Q);

  RYKU.flip(PECU, PASO);
  ROGA.flip(!RYKU_Q, PASO);
  RUBU.flip(!ROGA_Q, PASO);

  NYZE.tock(MOXE, ppu.XYMU, PUXA_Q);
  PUXA.tock(lcd.ROXO, ppu.XYMU, POHU);

  RYFA.tock(lcd.SEGU, ppu.XYMU, PANY);
  RENE.tock(clk.CLK_4M_B, ppu.XYMU, RYFA_Q);
  SOVY.tock(clk.CLK_4M_B, rst.RESET_VIDEO, RYDY);
}