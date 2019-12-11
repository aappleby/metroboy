#include "Sch_Video.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P21_VideoControl_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)cpu_in;
  (void)chip_in;

  /*p08.SORE*/ wire SORE = not(b.A15);
  /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(b.A13, b.A14, SORE);
  /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(cpu_in.ADDR_VALID, ADDR_NOT_VRAM);
  /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
  /*p25.SYRO*/ c.vid.ADDR_FE00_FFFF = not(ADDR_0000_FE00);

  {
    /*p25.TEFA*/ wire TEFA = nor(b.vid.ADDR_FE00_FFFF, ADDR_VALID_AND_NOT_VRAM);
    /*p25.SOSE*/ c.vid.ADDR_VRAM = and(b.A15, TEFA);
  }

  //----------


  //----------

  {
    /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(b.apu.CLK_xBxDxFxH4);
    /*p01.ZEME*/ c.sys.CLK_xBxDxFxH2  = not(ZAXY_AxCxExGx);
  }

  /*p01.ALET*/ c.sys.CLK_AxCxExGx4  = not(b.sys.CLK_xBxDxFxH2);

  /*p27.MYVO*/ c.vid.CLK_xBxDxFxHc = not(b.sys.CLK_AxCxExGx4);
  /*p29.LAPE*/ c.vid.CLK_xBxDxFxHd = not(b.sys.CLK_AxCxExGx4);

  {
    /*p01.ATAL*/ wire ATAL_AxCxExGx  = not(b.sys.ROOTCLK_xBxDxFxH);
    /*p01.AZOF*/ wire AZOF_xBxDxFxH  = not(ATAL_AxCxExGx);
    /*p01.ZAXY*/ wire ZAXY_AxCxExGx  = not(AZOF_xBxDxFxH);
    /*p01.ZEME*/ wire ZEME_xBxDxFxH  = not(ZAXY_AxCxExGx);
    /*p29.XYVA*/ wire XYVA_AxCxExGx  = not(ZEME_xBxDxFxH);
    /*p29.XOTA*/ c.vid.CLK_xBxDxFxHe = not(XYVA_AxCxExGx);
    /*p29.WUVU*/ c.vid.CLK_AxxDExxHa = tock_pos( a.vid.CLK_xBxDxFxHe,  b.vid.CLK_xBxDxFxHe, b.sys.VID_RESETn, b.vid.CLK_AxxDExxHa, !b.vid.CLK_AxxDExxHa);
    /*p21.VENA*/ c.vid.CLK_AxxxxFGHa = tock_pos(!a.vid.CLK_AxxDExxHa, !b.vid.CLK_AxxDExxHa, b.sys.VID_RESETn, b.vid.CLK_AxxxxFGHa, !b.vid.CLK_AxxxxFGHa);
  }

  //----------
  // x counter. this is a little weird, presumably because it can tick at 4 mhz but not always?

  {
    /*p01.TOFU*/ wire VID_RESET3  = not(b.sys.VID_RESETn);
    /*p21.TADY*/ c.vid.X_RST = nor(b.vid.NEW_LINE1, VID_RESET3);

    /*p21.RYBO*/ c.vid.RYBO = xor(b.vid.X0, b.vid.X1);
    /*p21.XUKE*/ c.vid.XUKE = and(b.vid.X0, b.vid.X1);
    /*p21.XYLE*/ c.vid.XYLE = and(b.vid.X2, b.vid.XUKE);
    /*p21.XEGY*/ c.vid.XEGY = xor(b.vid.X2, b.vid.XUKE);
    /*p21.XORA*/ c.vid.XORA = xor(b.vid.X3, b.vid.XYLE);

    /*p21.XEHO*/ c.vid.X0 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.X_RST, b.vid.X0, !b.vid.X0);
    /*p21.SAVY*/ c.vid.X1 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.X_RST, b.vid.X1, b.vid.RYBO);
    /*p21.XODU*/ c.vid.X2 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.X_RST, b.vid.X2, b.vid.XEGY);
    /*p21.XYDO*/ c.vid.X3 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.X_RST, b.vid.X3, b.vid.XORA);

    /*p21.TOCA*/ c.vid.TOCA = not(b.vid.X3);
    /*p21.SAKE*/ c.vid.SAKE = xor(b.vid.X4, b.vid.X5);
    /*p21.TYBA*/ c.vid.TYBA = and(b.vid.X4, b.vid.X5);
    /*p21.SURY*/ c.vid.SURY = and(b.vid.X6, b.vid.TYBA);
    /*p21.TYGE*/ c.vid.TYGE = xor(b.vid.X6, b.vid.TYBA);
    /*p21.ROKU*/ c.vid.ROKU = xor(b.vid.X7, b.vid.SURY);

    /*p21.TUHU*/ c.vid.X4 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.X_RST, b.vid.X4, !b.vid.X4);
    /*p21.TUKY*/ c.vid.X5 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.X_RST, b.vid.X5, b.vid.SAKE);
    /*p21.TAKO*/ c.vid.X6 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.X_RST, b.vid.X6, b.vid.TYGE);
    /*p21.SYBE*/ c.vid.X7 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.X_RST, b.vid.X7, b.vid.ROKU);

    /*p21.XUGU*/ c.vid.X_167n = nand(b.vid.X0, b.vid.X1, b.vid.X2, b.vid.X5, b.vid.X7); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ c.vid.X_167 = not(b.vid.X_167n);
  }

  //----------
  // LY compare

  {
    /*p21.SYFU*/ wire LY_MATCH7 = xor(b.vid.V7, b.vid.LYC7);
    /*p21.TERY*/ wire LY_MATCH6 = xor(b.vid.V6, b.vid.LYC6);
    /*p21.TUCY*/ wire LY_MATCH5 = xor(b.vid.V5, b.vid.LYC5);
    /*p21.TYKU*/ wire LY_MATCH4 = xor(b.vid.V4, b.vid.LYC4);
    /*p21.RASY*/ wire LY_MATCH3 = xor(b.vid.V3, b.vid.LYC3);
    /*p21.REDA*/ wire LY_MATCH2 = xor(b.vid.V2, b.vid.LYC2);
    /*p21.TYDE*/ wire LY_MATCH1 = xor(b.vid.V1, b.vid.LYC1);
    /*p21.RYME*/ wire LY_MATCH0 = xor(b.vid.V0, b.vid.LYC0);

    /*p21.SOVU*/ wire LY_MATCHA = nor(LY_MATCH7, LY_MATCH6, LY_MATCH5, LY_MATCH4);
    /*p21.SUBO*/ wire LY_MATCHB = nor(LY_MATCH3, LY_MATCH2, LY_MATCH1, LY_MATCH0);

    /*p21.RAPE*/ wire LY_MATCHn = nand(LY_MATCHA, LY_MATCHB);
    /*p21.PALY*/ wire LY_MATCHa = not(LY_MATCHn);

    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.WESY*/ wire SYS_RESETn6 = not(XORE_RESET); // video

    /*p21.TALU*/ c.vid.CLK_AxxxxFGHb = not(!b.vid.CLK_AxxxxFGHa);
    /*p21.ROPO*/ c.vid.LYC_MATCH = tock_pos(a.vid.CLK_AxxxxFGHb, b.vid.CLK_AxxxxFGHb, SYS_RESETn6, b.vid.LYC_MATCH, LY_MATCHa);
  }

  //----------
  // Frame timer

  {
    /*p01.ATAR*/ wire VID_RESET6  = not(b.sys.VID_RESETn);
    /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);
    /*p01.AMYG*/ wire VID_RESET7  = not(b.sys.VID_RESETn);

    /*p01.LYHA*/ wire VID_RESET2  = not(b.sys.VID_RESETn);
    /*p21.MUDE*/ wire CNT_RSTn = nor(b.vid.LINE_DONE_SYNCa, VID_RESET2);
    /*p21.SAXO*/ c.vid.CNT_0 = tock_pos( a.vid.CLK_AxxxxFGHb, b.vid.CLK_AxxxxFGHb, CNT_RSTn, c.vid.CNT_0, !c.vid.CNT_0);
    /*p21.TYPO*/ c.vid.CNT_1 = tock_pos(!a.vid.CNT_0,         !b.vid.CNT_0,        CNT_RSTn, c.vid.CNT_1, !c.vid.CNT_1);
    /*p21.VYZO*/ c.vid.CNT_2 = tock_pos(!a.vid.CNT_1,         !b.vid.CNT_1,        CNT_RSTn, c.vid.CNT_2, !c.vid.CNT_2);
    /*p21.TELU*/ c.vid.CNT_3 = tock_pos(!a.vid.CNT_2,         !b.vid.CNT_2,        CNT_RSTn, c.vid.CNT_3, !c.vid.CNT_3);
    /*p21.SUDE*/ c.vid.CNT_4 = tock_pos(!a.vid.CNT_3,         !b.vid.CNT_3,        CNT_RSTn, c.vid.CNT_4, !c.vid.CNT_4);
    /*p21.TAHA*/ c.vid.CNT_5 = tock_pos(!a.vid.CNT_4,         !b.vid.CNT_4,        CNT_RSTn, c.vid.CNT_5, !c.vid.CNT_5);
    /*p21.TYRY*/ c.vid.CNT_6 = tock_pos(!a.vid.CNT_5,         !b.vid.CNT_5,        CNT_RSTn, c.vid.CNT_6, !c.vid.CNT_6);

    /*p21.SANU*/ wire CNT_113n = nand(b.vid.CNT_6, b.vid.CNT_5, b.vid.CNT_4, b.vid.CNT_0);

    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
    /*p21.SONO*/ c.vid.CLK_xBCDExxxa = not(b.vid.CLK_AxxxxFGHb);
    /*p21.RUTU*/ c.vid.LINE_DONE_SYNCa = tock_pos(a.vid.CLK_xBCDExxxa, b.vid.CLK_xBCDExxxa, VID_RESETn2, b.vid.LINE_DONE_SYNCa, CNT_113n);
    /*p21.NYPE*/ c.vid.LINE_DONE_SYNCb = tock_pos(a.vid.CLK_AxxxxFGHb, b.vid.CLK_AxxxxFGHb, VID_RESETn2, b.vid.LINE_DONE_SYNCb, b.vid.LINE_DONE_SYNCa);

    /*p21.PURE*/ wire LINE_DONEa     = not(b.vid.LINE_DONE_SYNCa);
    /*p21.SELA*/ wire LINE_DONEo     = not(LINE_DONEa);
    /*p21.XYVO*/ wire LINE_144       = and(b.vid.V4, b.vid.V7); // 128 + 16 = 144
    /*p29.ALES*/ wire LINE_144n      = not(LINE_144);
    /*p29.ABOV*/ wire IN_LINEb       = and(LINE_DONEo, LINE_144n);

    /*p29.XUPY*/ c.vid.CLK_AxxDExxHb = not(!b.vid.CLK_AxxDExxHa);
    /*p29.CATU*/ c.spr.IN_LINE_SYNCa = tock_pos(a.vid.CLK_AxxDExxHb, b.vid.CLK_AxxDExxHb, VID_RESETn3, b.spr.IN_LINE_SYNCa, IN_LINEb);
    /*p28.ANEL*/ c.spr.IN_LINE_SYNCb = tock_pos(a.spr.CLK_xBCxxFGx,  b.spr.CLK_xBCxxFGx,  VID_RESETn3, b.spr.IN_LINE_SYNCb, b.spr.IN_LINE_SYNCa);

    /*p28.ABAF*/ wire IN_LINEn  = not(b.spr.IN_LINE_SYNCa);
    /*p28.BYHA*/ wire NEW_LINEn = or(b.spr.IN_LINE_SYNCb, IN_LINEn, VID_RESETn3);

    /*p28.ATEJ*/ c.vid.NEW_LINE1 = not(NEW_LINEn); // this drives sprite count reset, it must be new line.
    /*p28.ABAK*/ c.vid.NEW_LINE2 = or (b.vid.NEW_LINE1, VID_RESET7);
    /*p28.BYVA*/ c.vid.NEW_LINE3 = not(b.vid.NEW_LINE2);
    /*p29.DYBA*/ c.vid.NEW_LINE4 = not(b.vid.NEW_LINE3);

    // 128+16+1+2
    /*p21.NOKO*/ wire LINE_147 = and(b.vid.V7, b.vid.V4, b.vid.V0, b.vid.V1);
    /*p21.MYTA*/ c.vid.LINE_147_SYNC = tock_pos(a.vid.LINE_DONE_SYNCb, b.vid.LINE_DONE_SYNCb, VID_RESETn2, b.vid.LINE_147_SYNC, LINE_147);
    /*p21.LAMA*/ wire Y_RSTn = nor(b.vid.LINE_147_SYNC, VID_RESET2);

    /*p21.MUWY*/ c.vid.V0 = tock_pos( a.vid.LINE_DONE_SYNCa,  b.vid.LINE_DONE_SYNCa, Y_RSTn, c.vid.V0, !c.vid.V0);
    /*p21.MYRO*/ c.vid.V1 = tock_pos(!a.vid.V0,              !b.vid.V0,              Y_RSTn, c.vid.V1, !c.vid.V1);
    /*p21.LEXA*/ c.vid.V2 = tock_pos(!a.vid.V1,              !b.vid.V1,              Y_RSTn, c.vid.V2, !c.vid.V2);
    /*p21.LYDO*/ c.vid.V3 = tock_pos(!a.vid.V2,              !b.vid.V2,              Y_RSTn, c.vid.V3, !c.vid.V3);
    /*p21.LOVU*/ c.vid.V4 = tock_pos(!a.vid.V3,              !b.vid.V3,              Y_RSTn, c.vid.V4, !c.vid.V4);
    /*p21.LEMA*/ c.vid.V5 = tock_pos(!a.vid.V4,              !b.vid.V4,              Y_RSTn, c.vid.V5, !c.vid.V5);
    /*p21.MATO*/ c.vid.V6 = tock_pos(!a.vid.V5,              !b.vid.V5,              Y_RSTn, c.vid.V6, !c.vid.V6);
    /*p21.LAFO*/ c.vid.V7 = tock_pos(!a.vid.V6,              !b.vid.V6,              Y_RSTn, c.vid.V7, !c.vid.V7);

    /*p21.POPU*/ c.vid.REG_VBLANK = tock_pos(a.vid.LINE_DONE_SYNCb, b.vid.LINE_DONE_SYNCb, VID_RESETn2, b.vid.REG_VBLANK, LINE_144);
  }


  {

    /*p27.XAHY*/ wire NEW_LINEn = not(b.vid.NEW_LINE1);
    /*p27.XOFO*/ wire MAP_X_RST = nand(b.vid.LCDC_WINEN, NEW_LINEn, b.sys.VID_RESETn);
    /*p27.PYNU*/ c.vid.WIN_RST = or(b.vid.WIN_MATCH_SYNC2, MAP_X_RST);
    /*p27.NOPA*/ c.vid.WIN_RST_SYNC = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn, b.vid.WIN_RST_SYNC, b.vid.WIN_RST);

    /*p27.NUNY*/ c.vid.WIN_RST_TRIG = and(b.vid.WIN_RST, !b.vid.WIN_RST_SYNC);

    /*p01.PYRY*/ wire VID_RESET4  = not(b.sys.VID_RESETn);

    // PUKU/RYDY form a NOR latch. WIN_RST_TRIG is SET, (VID_REST | BG_SEQ_7) is RESET.
    /*p27.PUKU*/ c.vid.PUKU = nor(b.vid.RYDY, b.vid.WIN_RST_TRIG);
    /*p27.RYDY*/ c.vid.RYDY = nor(b.vid.PUKU, VID_RESET4, b.vid.BG_SEQ_7);
    /*p27.SOVY*/ c.vid.REG_SOVY = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn, b.vid.REG_SOVY, b.vid.RYDY);

    /*p27.NOCU*/ wire NOCU = not(c.vid.WIN_RST);
    /*p27.PORE*/ wire PORE = not(NOCU);
    /*p27.WAZY*/ c.vid.WAZY= not(PORE);
  }

  {
    /*p24.POKY*/ wire POKY = or(b.vid.BG_SEQ_8, b.vid.RENDERINGn);
    /*p27.ROMO*/ wire ROMO = not(POKY);
    /*p27.SYLO*/ wire SYLO = not(b.vid.RYDY);

    /*p27.SUVU*/   wire SUVU = nand(b.vid.RENDERING, ROMO, b.vid.BG_SEQ_6, b.vid.BG_SEQ_7);
    /*p27.TAVE*/ wire TAVE = not(SUVU);

    /*p27.TUXY*/     wire TUXY = nand(b.vid.REG_SOVY, SYLO);
    /*p27.SUZU*/   wire SUZU = not(TUXY);
    /*p27.TEVO*/ c.vid.MAP_X_CLK_STOPn = nor(b.vid.WIN_TRIGGER, SUZU, TAVE);

    /*p01.ROSY*/ wire VID_RESET5  = not(b.sys.VID_RESETn);

    /*p24.TOMU*/     wire TOMU = not(SYLO);
    /*p27.TUKU*/   wire TUKU = not(TOMU);
    /*p27.VEKU*/       wire VEKU = nor(b.spr.SPRITE_DONE, TAVE);
    /*p27.RYCE*/         wire TEKY_TRIG = and(b.vid.TEKY_SYNC1, !b.vid.TEKY_SYNC2);
    /*p27.SECA*/       wire SPR_SEQ_RST = nor(TEKY_TRIG, VID_RESET5, b.vid.NEW_LINE1);
    /*p27.TAKA*/     wire TAKA = unk2(VEKU, SPR_SEQ_RST);
    /*p27.SOWO*/   wire SOWO = not(TAKA);
    /*p27.TEKY*/ wire TEKY = and(b.spr.STORE_MATCH, TUKU, b.vid.BG_SEQ_5, SOWO);

    /*p27.VYPO*/ wire VYPO = not(b.joy.P10_B);

    /*p01.ATAL*/ wire ATAL_AxCxExGx = not(b.sys.ROOTCLK_xBxDxFxH);
    /*p01.AZOF*/ wire AZOF_xBxDxFxH = not(ATAL_AxCxExGx);
    /*p01.ZAXY*/ wire ZAXY_AxCxExGx = not(AZOF_xBxDxFxH);
    /*p01.ZEME*/ wire ZEME_xBxDxFxH = not(ZAXY_AxCxExGx);
    /*p01.ALET*/ wire ALET_AxCxExGx = not(ZEME_xBxDxFxH);

    /*p01.LAPE*/ c.vid.LAPE_xBxDxFxH = not(ALET_AxCxExGx);
    /*p27.TAVA*/ c.vid.TAVA_AxCxExGx = not(b.vid.LAPE_xBxDxFxH);
    /*p27.SOBU*/ c.vid.TEKY_SYNC1 = tock_pos(a.vid.TAVA_AxCxExGx, b.vid.TAVA_AxCxExGx, VYPO, b.vid.TEKY_SYNC1, TEKY);
    /*p27.SUDA*/ c.vid.TEKY_SYNC2 = tock_pos(a.vid.LAPE_xBxDxFxH, b.vid.LAPE_xBxDxFxH, VYPO, b.vid.TEKY_SYNC2, b.vid.TEKY_SYNC1);

    /*p24.SOCY*/     wire SOCY = not(TOMU);
    /*p24.VYBO*/     wire VYBO = nor(b.spr.STORE_MATCH, b.vid.RENDER_DONE, b.vid.CLK_xBxDxFxHc);
    /*p24.TYFA*/   wire TYFA = and(SOCY, POKY, VYBO);
    /*p24.SEGU*/ c.vid.SEGU_4M = not(TYFA);
  }

  {
    /*p27.PAHA*/ wire PAHA = not(b.vid.RENDERING);
    /*p27.ROXY*/ wire STOP_THE_PIPE = or(PAHA, b.vid.FINE_MATCH_TRIG);
    /*p24.SACU*/ c.vid.CLKPIPE = nor(b.vid.SEGU_4M, STOP_THE_PIPE);
  }

  {
    // Horizontal sync
    // ST = or(PAHO, VID_RESET3, nor(SCAN_DONE_TRIG, POFY));
    // Combi loop, please fix.
    ///*p24.POME*/ wire POME = nor(b.spr.SCAN_DONE_TRIG, POFY);
    ///*p24.RUJU*/ wire RUJU = or(b.vid.PAHO, b.sys.VID_RESET3, POME);
    ///*p24.POFY*/ wire POFY = not(RUJU);

    /*p24.PAHO*/ c.vid.PAHO = tock_pos(a.vid.FINE_MATCH_CLK, b.vid.FINE_MATCH_CLK, b.vid.RENDERING, b.vid.PAHO, b.vid.X3);
    /*p01.TOFU*/ wire VID_RESET3  = not(b.sys.VID_RESETn);
    /*p24.RUJU*/ wire RUJU = or(b.vid.PAHO, VID_RESET3, b.spr.SCAN_DONE_TRIG);
    /*p24.POFY*/ wire POFY = not(RUJU);

    /*p24.RUZE*/ c.lcd.ST = not(POFY);
  }

  {
    /*p24.KEDY*/ wire LCDC_ENn = not(b.vid.LCDC_EN);
    /*p21.PURE*/ wire LINE_DONEn = not(b.vid.LINE_DONE_SYNCa);
    /*p24.KASA*/ wire LINE_DONEb = not(LINE_DONEn);
    /*p24.UMOB*/ wire DIV_06 = not(b.tim.DIV_06n);
    /*p24.KAHE*/ wire CPLn = amux2(b.vid.LCDC_EN, LINE_DONEb, LCDC_ENn, DIV_06);
    /*p24.KYMO*/ c.lcd.CPL = not(CPLn);
  }

  {
    // if LCDC_ENn, FR = 4k div clock. Otherwise FR = xor(LINE_EVEN,FRAME_EVEN)

    /*p01.LYHA*/ wire VID_RESET2  = not(b.sys.VID_RESETn);
    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);

    /*p24.LOFU*/ c.vid.CLK_LINE_EVEN = not(b.vid.LINE_DONE_SYNCa);
    /*p24.LUCA*/ c.vid.LINE_EVEN = tock_pos(a.vid.CLK_LINE_EVEN, b.vid.CLK_LINE_EVEN, VID_RESETn2, b.vid.LINE_EVEN, !b.vid.LINE_EVEN);
    /*p21.NAPO*/ c.vid.FRAME_EVEN = tock_pos(a.vid.REG_VBLANK, b.vid.REG_VBLANK, VID_RESETn2, b.vid.FRAME_EVEN, !b.vid.FRAME_EVEN);

    /*p24.KEDY*/ wire LCDC_ENn = not(b.vid.LCDC_EN);
    /*p24.MAGU*/ wire MAGU = xor(b.vid.FRAME_EVEN, b.vid.LINE_EVEN);
    /*p24.MECO*/ wire MECO = not(MAGU);
    /*p24.KEBO*/ wire KEBO = not(MECO);
    /*p24.USEC*/ wire USEC = not(b.tim.DIV_07n);
    /*p24.KUPA*/ wire FRn  = amux2(b.vid.LCDC_EN, KEBO, LCDC_ENn, USEC);
    /*p24.KOFO*/ c.lcd.FR  = not(FRn);
  }

  {
    // Vertical sync
    /*p01.LYHA*/ wire VID_RESET2  = not(b.sys.VID_RESETn);
    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);
    /*p24.NERU*/ wire LINE_000n = nor(b.vid.V0, b.vid.V1, b.vid.V2, b.vid.V3, b.vid.V4, b.vid.V5, b.vid.V6, b.vid.V7);
    /*p24.MEDA*/ c.vid.VSYNC_OUTn = tock_pos(a.vid.LINE_DONE_SYNCb, b.vid.LINE_DONE_SYNCb, VID_RESETn2, b.vid.VSYNC_OUTn, LINE_000n);
    /*p24.MURE*/ c.lcd.S = not(b.vid.VSYNC_OUTn);

  }



  {
    wire C0 = b.vid.CNT_0;
    wire C1 = b.vid.CNT_1;
    wire C2 = b.vid.CNT_2;
    wire C3 = b.vid.CNT_3;
    wire C4 = b.vid.CNT_4;
    wire C5 = b.vid.CNT_5;
    wire C6 = b.vid.CNT_6;

    /*p21.TOCU*/ wire C0n = not(C0);
    /*p21.VEPE*/ wire C1n = not(C1);
    /*p21.VUTY*/ wire C2n = not(C2);
    /*p21.VATE*/ wire C3n = not(C3);
    /*p21.TUDA*/ wire C4n = not(C4);
    /*p21.TAFY*/ wire C5n = not(C5);
    /*p21.TUJU*/ wire C6n = not(C6);

    // so this is like a strobe that fires 4x per line
    /*p21.VOKU*/ wire CNT_000n = nand(C6n, C5n, C4n, C3n, C2n, C1n, C0n); // 0000000 == 0
    /*p21.TOZU*/ wire CNT_007n = nand(C6n, C5n, C4n, C3n, C2,  C1,  C0 ); // 0000111 == 7
    /*p21.TECE*/ wire CNT_045n = nand(C6n, C5,  C4n, C3,  C2,  C1n, C0 ); // 0101101 == 45
    /*p21.TEBO*/ wire CNT_083n = nand(C6,  C5n, C4,  C3n, C2n, C1,  C0 ); // 1010011 == 83

    /*p21.TEGY*/ wire LINE_STROBEa = nand(CNT_000n, CNT_007n, CNT_045n, CNT_083n);

    /*p01.LYHA*/ wire VID_RESET2  = not(b.sys.VID_RESETn);
    /*p01.LYFE*/ wire VID_RESETn2 = not(VID_RESET2);

    /*p21.SONO*/ c.vid.CLK_xBCDExxxa = not(b.vid.CLK_AxxxxFGHb);
    /*p21.SYGU*/ c.vid.LINE_STROBE = tock_pos(a.vid.CLK_xBCDExxxa, b.vid.CLK_xBCDExxxa, VID_RESETn2, b.vid.LINE_STROBE, LINE_STROBEa);

    /*p21.RYNO*/ wire CPGn = or(b.vid.LINE_STROBE, b.vid.LINE_DONE_SYNCa);
    /*p21.POGU*/ c.lcd.CPG = not(CPGn);
  }

  //----------
  // FF41 STAT

  
  //---

  {
    /*p01.TOFU*/ wire VID_RESET3  = not(b.sys.VID_RESETn);
    /*p21.WEGO or */ wire WEGO = or(VID_RESET3, b.vid.RENDER_DONE_SYNC);
    /*p21.XYMU ???*/ c.vid.RENDERING = or(WEGO, b.spr.SCAN_DONE_TRIG); // the trig here doesn't make sense
    /*p29.TEPA not*/ c.spr.RENDERINGn = not(b.vid.RENDERING);
    /*p24.LOBY not*/ c.vid.RENDERINGn = not(b.vid.RENDERING);

    /*p21.XENA*/ wire STORE_MATCHn = not(b.spr.STORE_MATCH);
    /*p21.WODU*/ c.vid.RENDER_DONE = and(STORE_MATCHn, b.vid.X_167);
    /*p21.VOGA*/ c.vid.RENDER_DONE_SYNC = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.vid.X_RST, b.vid.RENDER_DONE_SYNC, b.vid.RENDER_DONE);
  }

  // LCD clock
  // CP = not(or(and(CLKPIPE, or(X_009, WEGO)), FINE_MATCH_TRIG));
  {   
    /*p01.TOFU*/ wire VID_RESET3  = not(b.sys.VID_RESETn);
    /*p21.WEGO*/ wire WEGO  = or(VID_RESET3, b.vid.RENDER_DONE_SYNC);
    /*p21.XAJO*/ wire X_009 = and(b.vid.X0, b.vid.X3);
    /*p21.WUSA*/ wire WUSA  = or(X_009, WEGO);
    /*p21.TOBA*/ wire TOBA  = and(b.vid.CLKPIPE, WUSA);
    /*p21.SEMU*/ wire CPn   = or(TOBA, b.vid.FINE_MATCH_TRIG);
    /*p21.RYPO*/ c.lcd.CP = not(CPn);
  }



  //---


  //----------
  // FF41 STAT stuff

  {
    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.WESY*/ wire WESY_RESET = not(XORE_RESET);

    /*p22.WOFA*/ wire FF41n   = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01n, b.vid.A02n, b.vid.A03n);
    /*p22.VARY*/ wire FF41    = not(FF41n);
    /*p21.SEPA*/ wire FF41_WR = and(b.sys.CPU_WR2, FF41);

    /*p21.RYVE*/ c.vid.CLK_STAT= not(FF41_WR);
    /*p21.ROXE*/ c.vid.INT_HBL_EN = tock_pos(a.vid.CLK_STAT, b.vid.CLK_STAT, WESY_RESET, b.vid.INT_HBL_EN, b.D3);
    /*p21.RUFO*/ c.vid.INT_VBL_EN = tock_pos(a.vid.CLK_STAT, b.vid.CLK_STAT, WESY_RESET, b.vid.INT_VBL_EN, b.D4);
    /*p21.REFE*/ c.vid.INT_OAM_EN = tock_pos(a.vid.CLK_STAT, b.vid.CLK_STAT, WESY_RESET, b.vid.INT_OAM_EN, b.D5);
    /*p21.RUGU*/ c.vid.INT_LYC_EN = tock_pos(a.vid.CLK_STAT, b.vid.CLK_STAT, WESY_RESET, b.vid.INT_LYC_EN, b.D6);

    /*p21.PARU*/ wire VBLANK = not(!b.vid.REG_VBLANK);

    // 11: hblank   - rendering 0, vbl 0, oam 0
    // 10: vblank   - rendering 0, vbl 1, oam 0
    // 01: oam scan - rendering 0, vbl 0, oam 1
    // 00: render   - rendering 1, vbl 0, oam 0
    // so one of these has the wrong polarity

    /*p21.SADU*/ wire STAT_MODE0n = nor(b.vid.RENDERING, VBLANK);
    /*p21.XATY*/ wire STAT_MODE1n = nor(b.vid.RENDERING, b.spr.OAM_ADDR_PARSEn);

    /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);

    /*p01.WESY*/ wire SYS_RESETn6 = not(XORE_RESET);
    /*p21.PAGO*/ wire STAT_LYC_MATCH1 = nor(SYS_RESETn6, FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ wire STAT_LYC_MATCH2 = or(b.vid.LYC_MATCH, STAT_LYC_MATCH1); // this is another of the weird or gates. could be nor?

    /*p21.TOBE*/ wire FF41_RDa = and(b.sys.CPU_RD2, FF41);
    /*p21.VAVE*/ wire FF41_RDb = FF41_RDa; // buffer, not inverter?
    /*p21.TEBY*/ if (FF41_RDa) c.D0 = not(STAT_MODE0n);
    /*p21.WUGA*/ if (FF41_RDa) c.D1 = not(STAT_MODE1n);
    /*p21.SEGO*/ if (FF41_RDa) c.D2 = not(STAT_LYC_MATCH2);
    /*p21.PUZO*/ if (FF41_RDb) c.D3 = b.vid.INT_HBL_EN;
    /*p21.POFO*/ if (FF41_RDb) c.D4 = b.vid.INT_VBL_EN;
    /*p21.SASY*/ if (FF41_RDb) c.D5 = b.vid.INT_OAM_EN;
    /*p21.POTE*/ if (FF41_RDb) c.D6 = b.vid.INT_LYC_EN;

    /*p21.PURE*/ wire LINE_DONEa = not(b.vid.LINE_DONE_SYNCa);
    /*p21.SELA*/ wire LINE_DONEo = not(LINE_DONEa);
    /*p21.TOLU*/ wire INT_VBLn = not(VBLANK);
    /*p21.TAPA*/ wire INT_OAM = and(INT_VBLn, LINE_DONEo);
    /*p21.TARU*/ wire INT_HBL = and(INT_VBLn, b.vid.RENDER_DONE);

    /*p21.SUKO*/ wire INT_STATb = amux4(b.vid.INT_LYC_EN, b.vid.LYC_MATCH,
                                        b.vid.INT_OAM_EN, INT_OAM,
                                        b.vid.INT_VBL_EN, VBLANK, // polarity?
                                        b.vid.INT_HBL_EN, INT_HBL);

    /*p21.TUVA*/ wire INT_STATn = not(INT_STATb);
    /*p21.VOTY*/ c.vid.INT_STAT  = not(INT_STATn);
  }

  //----------

  /*p22.XALY*/ c.vid.ADDR_0x00xxxx = nor(b.A07, b.A05, b.A04);
  /*p22.WUTU*/ c.vid.FF4Xn         = nand(b.sys.ADDR_FFXX, b.A06, b.vid.ADDR_0x00xxxx);
  /*p22.WERO*/ c.vid.FF4X          = not(b.vid.FF4Xn);

  /*p22.XOLA*/ c.vid.A00n = not(b.A00);
  /*p22.XENO*/ c.vid.A01n = not(b.A01);
  /*p22.XUSY*/ c.vid.A02n = not(b.A02);
  /*p22.XERA*/ c.vid.A03n = not(b.A03);

  /*p22.WADO*/ c.vid.A00 = not(b.vid.A00n);
  /*p22.WESA*/ c.vid.A01 = not(b.vid.A01n);
  /*p22.WALO*/ c.vid.A02 = not(b.vid.A02n);
  /*p22.WEPO*/ c.vid.A03 = not(b.vid.A03n);


  //----------
  // FF40 LCDC
  {
    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.XARE*/ wire XARE_RESET = not(XORE_RESET);

    /*p22.WORU*/ c.vid.FF40n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01n, b.vid.A02n, b.vid.A03n);
    /*p22.VOCA*/ c.vid.FF40 = not(b.vid.FF40n);
    /*p23.VYRE*/ c.vid.FF40_RD = and(b.vid.FF40, b.sys.CPU_RD2);
    /*p23.WARU*/ c.vid.FF40_WR = and(b.vid.FF40, b.sys.CPU_WR2);

    /*p23.XUBO*/ c.vid.FF40_WRn = not(b.vid.FF40_WR);
    /*p23.VYXE*/ c.vid.LCDC_BGEN    = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_BGEN,   b.D0);
    /*p23.XYLO*/ c.vid.LCDC_SPEN    = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_SPEN,   b.D1);
    /*p23.XYMO*/ c.vid.LCDC_SPSIZE  = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_SPSIZE, b.D2);
    /*p23.XAFO*/ c.vid.LCDC_BGMAP   = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_BGMAP,  b.D3);
    /*p23.WEXU*/ c.vid.LCDC_BGTILE  = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_BGTILE, b.D4);
    /*p23.WYMO*/ c.vid.LCDC_WINEN   = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_WINEN,  b.D5);
    /*p23.WOKY*/ c.vid.LCDC_WINMAP  = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_WINMAP, b.D6);
    /*p23.XONA*/ c.vid.LCDC_EN      = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, XARE_RESET, b.vid.LCDC_EN,     b.D7);

    /*p23.WYCE*/ c.vid.FF40_RDn = not(b.vid.FF40_RD);
    /*p23.WYPO*/ if (b.vid.FF40_RDn) c.D0 = b.vid.LCDC_BGEN;
    /*p23.XERO*/ if (b.vid.FF40_RDn) c.D1 = b.vid.LCDC_SPEN;
    /*p23.WYJU*/ if (b.vid.FF40_RDn) c.D2 = b.vid.LCDC_SPSIZE;
    /*p23.WUKA*/ if (b.vid.FF40_RDn) c.D3 = b.vid.LCDC_BGMAP;
    /*p23.VOKE*/ if (b.vid.FF40_RDn) c.D4 = b.vid.LCDC_BGTILE;
    /*p23.VATO*/ if (b.vid.FF40_RDn) c.D5 = b.vid.LCDC_WINEN;
    /*p23.VAHA*/ if (b.vid.FF40_RDn) c.D6 = b.vid.LCDC_WINMAP;
    /*p23.XEBU*/ if (b.vid.FF40_RDn) c.D7 = b.vid.LCDC_EN;
  }

  //----------
  // FF4B WX
  {
    /*p22.WAGE*/ c.vid.FF4Bn = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01,  b.vid.A02n, b.vid.A03);
    /*p22.VUMY*/ c.vid.FF4B = not(b.vid.FF4Bn);
    /*p23.WYZE*/ c.vid.WYZE = and(b.sys.CPU_RD2, b.vid.FF4B);
    /*p23.WUZA*/ c.vid.WUZA = and(b.sys.CPU_WR2, b.vid.FF4B);
    /*p23.VYCU*/ c.vid.VYCU = not(b.vid.WYZE);
    /*p23.VOXU*/ c.vid.VOXU = not(b.vid.WUZA);


    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.WALU*/ wire WALU_RESET = not(XORE_RESET);

    /*p23.MYPA*/ c.vid.WX0 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX0, b.D0);
    /*p23.NOFE*/ c.vid.WX1 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX1, b.D1);
    /*p23.NOKE*/ c.vid.WX2 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX2, b.D2);
    /*p23.MEBY*/ c.vid.WX3 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX3, b.D3);
    /*p23.MYPU*/ c.vid.WX4 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX4, b.D4);
    /*p23.MYCE*/ c.vid.WX5 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX5, b.D5); 
    /*p23.MUVO*/ c.vid.WX6 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX6, b.D6);
    /*p23.NUKU*/ c.vid.WX7 = tock_pos(a.vid.VOXU, b.vid.VOXU, WALU_RESET, a.vid.WX7, b.D7);

    /*p23.LOVA*/ if (b.vid.VYCU) c.D0 = b.vid.WX0;
    /*p23.MUKA*/ if (b.vid.VYCU) c.D1 = b.vid.WX1;
    /*p23.MOKO*/ if (b.vid.VYCU) c.D2 = b.vid.WX2;
    /*p23.LOLE*/ if (b.vid.VYCU) c.D3 = b.vid.WX3;
    /*p23.MELE*/ if (b.vid.VYCU) c.D4 = b.vid.WX4;
    /*p23.MUFE*/ if (b.vid.VYCU) c.D5 = b.vid.WX5;
    /*p23.MULY*/ if (b.vid.VYCU) c.D6 = b.vid.WX6;
    /*p23.MARA*/ if (b.vid.VYCU) c.D7 = b.vid.WX7;
  }

  //----------
  // FF4A WY

  {
    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.WALU*/ wire WALU_RESET = not(XORE_RESET);

    /*p22.WYVO*/ c.vid.FF4An = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01,  b.vid.A02n, b.vid.A03);
    /*p22.VYGA*/ c.vid.FF4A = not(b.vid.FF4An);
    /*p23.WAXU*/ c.vid.WAXU = and(b.sys.CPU_RD2, b.vid.FF4A);
    /*p23.WEKO*/ c.vid.WEKO = and(b.sys.CPU_WR2, b.vid.FF4A);

    /*p23.VEFU*/ c.vid.VEFU = not(b.vid.WEKO);
    /*p23.NESO*/ c.vid.WY0 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY0, b.D0);
    /*p23.NYRO*/ c.vid.WY1 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY1, b.D1);
    /*p23.NAGA*/ c.vid.WY2 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY2, b.D2);
    /*p23.MELA*/ c.vid.WY3 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY3, b.D3);
    /*p23.NULO*/ c.vid.WY4 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY4, b.D4);
    /*p23.NENE*/ c.vid.WY5 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY5, b.D5);
    /*p23.NUKA*/ c.vid.WY6 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY6, b.D6);
    /*p23.NAFU*/ c.vid.WY7 = tock_pos(a.vid.VEFU, b.vid.VEFU, WALU_RESET, b.vid.WY7, b.D7);

    /*p23.VOMY*/ c.vid.VOMY = not(b.vid.WAXU);
    /*p23.PUNU*/ if (b.vid.VOMY) c.D0 = b.vid.WY0;
    /*p23.PODA*/ if (b.vid.VOMY) c.D1 = b.vid.WY1;
    /*p23.PYGU*/ if (b.vid.VOMY) c.D2 = b.vid.WY2;
    /*p23.LOKA*/ if (b.vid.VOMY) c.D3 = b.vid.WY3;
    /*p23.MEGA*/ if (b.vid.VOMY) c.D4 = b.vid.WY4;
    /*p23.PELA*/ if (b.vid.VOMY) c.D5 = b.vid.WY5;
    /*p23.POLO*/ if (b.vid.VOMY) c.D6 = b.vid.WY6;
    /*p23.MERA*/ if (b.vid.VOMY) c.D7 = b.vid.WY7;
  }

  //----------
  // FF42 SCY

  {
    /*p22.WEBU*/ c.vid.FF42n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01,  b.vid.A02n, b.vid.A03n);
    /*p22.XARO*/ c.vid.FF42 = not(b.vid.FF42n);

    /*p23.ANYP*/ c.vid.ANYP = and(b.sys.CPU_RD2, b.vid.FF42);
    /*p23.BEDY*/ c.vid.BEDY = and(b.sys.CPU_WR2, b.vid.FF42);
    /*p23.CAVO*/ c.vid.CAVO = not(b.vid.BEDY);
    /*p23.BUWY*/ c.vid.BUWY = not(b.vid.ANYP);

    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p23.GAVE*/ c.vid.SCY0 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY0, b.D0);
    /*p23.FYMO*/ c.vid.SCY1 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY1, b.D1);
    /*p23.FEZU*/ c.vid.SCY2 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY2, b.D2);
    /*p23.FUJO*/ c.vid.SCY3 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY3, b.D3);
    /*p23.DEDE*/ c.vid.SCY4 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY4, b.D4);
    /*p23.FOTY*/ c.vid.SCY5 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY5, b.D5);
    /*p23.FOHA*/ c.vid.SCY6 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY6, b.D6);
    /*p23.FUNY*/ c.vid.SCY7 = tock_pos(a.vid.CAVO, b.vid.CAVO, CUNU_RESET, b.vid.SCY7, b.D7);

    /*p23.WARE*/ if (b.vid.BUWY) c.D0 = b.vid.SCY0;
    /*p23.GOBA*/ if (b.vid.BUWY) c.D1 = b.vid.SCY1;
    /*p23.GONU*/ if (b.vid.BUWY) c.D2 = b.vid.SCY2;
    /*p23.GODO*/ if (b.vid.BUWY) c.D3 = b.vid.SCY3;
    /*p23.CUSA*/ if (b.vid.BUWY) c.D4 = b.vid.SCY4;
    /*p23.GYZO*/ if (b.vid.BUWY) c.D5 = b.vid.SCY5;
    /*p23.GUNE*/ if (b.vid.BUWY) c.D6 = b.vid.SCY6;
    /*p23.GYZA*/ if (b.vid.BUWY) c.D7 = b.vid.SCY7;
  }

  //----------
  // FF43 SCX

  {
    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);

    /*p22.WAVU*/ c.vid.FF43n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01,  b.vid.A02n, b.vid.A03n);
    /*p22.XAVY*/ c.vid.FF43 = not(b.vid.FF43n);
    /*p23.AVOG*/ c.vid.AVOG = and(b.vid.FF43, b.sys.CPU_RD2);
    /*p23.ARUR*/ c.vid.ARUR = and(b.vid.FF43, b.sys.CPU_WR2);
    /*p23.AMUN*/ c.vid.AMUN = not(b.vid.ARUR);
    /*p23.BEBA*/ c.vid.BEBA = not(b.vid.AVOG);

    /*p23.DATY*/ c.vid.SCX0 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX0, b.D0);
    /*p23.DUZU*/ c.vid.SCX1 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX1, b.D1);
    /*p23.CYXU*/ c.vid.SCX2 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX2, b.D2);
    /*p23.GUBO*/ c.vid.SCX3 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX3, b.D3);
    /*p23.BEMY*/ c.vid.SCX4 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX4, b.D4);
    /*p23.CUZY*/ c.vid.SCX5 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX5, b.D5);
    /*p23.CABU*/ c.vid.SCX6 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX6, b.D6);
    /*p23.BAKE*/ c.vid.SCX7 = tock_pos(a.vid.AMUN, b.vid.AMUN, CUNU_RESET, a.vid.SCX7, b.D7);

    /*p23.EDOS*/ if (b.vid.BEBA) c.D0 = b.vid.SCX0;
    /*p23.EKOB*/ if (b.vid.BEBA) c.D1 = b.vid.SCX1;
    /*p23.CUGA*/ if (b.vid.BEBA) c.D2 = b.vid.SCX2;
    /*p23.WONY*/ if (b.vid.BEBA) c.D3 = b.vid.SCX3;
    /*p23.CEDU*/ if (b.vid.BEBA) c.D4 = b.vid.SCX4;
    /*p23.CATA*/ if (b.vid.BEBA) c.D5 = b.vid.SCX5;
    /*p23.DOXE*/ if (b.vid.BEBA) c.D6 = b.vid.SCX6;
    /*p23.CASY*/ if (b.vid.BEBA) c.D7 = b.vid.SCX7;
  }

  //----------
  // FF45 LYC

  {
    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.WESY*/ wire WESY_RESET = not(XORE_RESET);

    /*p22.WETY*/ wire FF45n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01n, b.vid.A02,  b.vid.A03n);
    /*p22.XAYU*/ wire FF45 = not(FF45n);

    /*p23.XUFA*/ wire XUFA = and(b.sys.CPU_WR2, FF45);
    /*p23.WANE*/ c.vid.CLK_LYC = not(XUFA);
    /*p23.SYRY*/ c.vid.LYC0 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC0, b.D0);
    /*p23.VUCE*/ c.vid.LYC1 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC1, b.D1);
    /*p23.SEDY*/ c.vid.LYC2 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC2, b.D2);
    /*p23.SALO*/ c.vid.LYC3 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC3, b.D3);
    /*p23.SOTA*/ c.vid.LYC4 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC4, b.D4);
    /*p23.VAFA*/ c.vid.LYC5 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC5, b.D5);
    /*p23.VEVO*/ c.vid.LYC6 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC6, b.D6);
    /*p23.RAHA*/ c.vid.LYC7 = tock_pos(a.vid.CLK_LYC, b.vid.CLK_LYC, WESY_RESET, a.vid.LYC7, b.D7);

    /*p23.XYLY*/ wire XYLY = and(b.sys.CPU_RD2, FF45);
    /*p23.WEKU*/ wire WEKU = not(XYLY);
    /*p23.RETU*/ if (WEKU) c.D0 = b.vid.LYC0;
    /*p23.VOJO*/ if (WEKU) c.D1 = b.vid.LYC1;
    /*p23.RAZU*/ if (WEKU) c.D2 = b.vid.LYC2;
    /*p23.REDY*/ if (WEKU) c.D3 = b.vid.LYC3;
    /*p23.RACE*/ if (WEKU) c.D4 = b.vid.LYC4;
    /*p23.VAZU*/ if (WEKU) c.D5 = b.vid.LYC5;
    /*p23.VAFE*/ if (WEKU) c.D6 = b.vid.LYC6;
    /*p23.PUFY*/ if (WEKU) c.D7 = b.vid.LYC7;
  }

  //----------
  // FF44 LY

  {
    /*p22.WYLE*/ wire FF44n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01n, b.vid.A02,  b.vid.A03n);
    /*p22.XOGY*/ wire FF44 = not(FF44n);
    /*p23.WAFU*/ wire FF44_RD = and(b.sys.CPU_RD2, FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(b.vid.V0);
    /*p23.XEPO*/ wire LY1n = not(b.vid.V1);
    /*p23.MYFA*/ wire LY2n = not(b.vid.V2);
    /*p23.XUHY*/ wire LY3n = not(b.vid.V3);
    /*p23.WATA*/ wire LY4n = not(b.vid.V4);
    /*p23.XAGA*/ wire LY5n = not(b.vid.V5);
    /*p23.XUCE*/ wire LY6n = not(b.vid.V6);
    /*p23.XOWO*/ wire LY7n = not(b.vid.V7);

    /*p23.VEGA*/ if (FF44_RDn) c.D0 = not(LY0n);
    /*p23.WUVA*/ if (FF44_RDn) c.D1 = not(LY1n);
    /*p23.LYCO*/ if (FF44_RDn) c.D2 = not(LY2n);
    /*p23.WOJY*/ if (FF44_RDn) c.D3 = not(LY3n);
    /*p23.VYNE*/ if (FF44_RDn) c.D4 = not(LY4n);
    /*p23.WAMA*/ if (FF44_RDn) c.D5 = not(LY5n);
    /*p23.WAVO*/ if (FF44_RDn) c.D6 = not(LY6n);
    /*p23.WEZE*/ if (FF44_RDn) c.D7 = not(LY7n);
  }

  //----------
  // more debug stuff

  {
    /*p01.DULA*/ wire DULA_RESET = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.CUNU*/ wire SOTO_RESET = not(DULA_RESET);

    /*p07.UBET*/ wire T1n       = not(b.pin.T1);
    /*p07.UNOR*/ wire MODE_DBG2 = and(b.pin.T2, T1n);
    /*p25.SYCY*/ c.vid.CLK_SOTO = not(MODE_DBG2);
    /*p25.SOTO*/ c.vid.DBG_SOTO = tock_pos(a.vid.CLK_SOTO, b.vid.CLK_SOTO, SOTO_RESET, b.vid.DBG_SOTO, !b.vid.DBG_SOTO);

    /*p25.TUSO*/ wire TUSO = nor(MODE_DBG2, b.sys.BOGA_xBCDEFGH);
    /*p25.SOLE*/ wire SOLE = not(TUSO);
    /*p27.VYPO*/ wire VYPO = not(b.joy.P10_B);

    /*p25.TOVU*/ if (VYPO) c.D0 = SOLE;
    /*p25.SOSA*/ if (VYPO) c.D1 = SOLE;
    /*p25.SEDU*/ if (VYPO) c.D2 = SOLE;
    /*p25.TAXO*/ if (VYPO) c.D3 = SOLE;
    /*p25.TAHY*/ if (VYPO) c.D4 = SOLE;
    /*p25.TESU*/ if (VYPO) c.D5 = SOLE;
    /*p25.TAZU*/ if (VYPO) c.D6 = SOLE;
    /*p25.TEWA*/ if (VYPO) c.D7 = SOLE;
  }

  //----------

  {
    /*p25.TUTO*/ wire DBG_TUTO = and(b.dbg.MODE_DBG2, !b.vid.DBG_SOTO);
    /*p25.RACO*/ wire DBG_TUTOn = not(DBG_TUTO);
    /*p25.TEFY*/ wire MCS_Cn = not(b.pin.MCS_C);
    /*p25.TUCA*/ wire CPU_VRAM_RD  = and (b.vid.ADDR_VRAM, b.sys.ADDR_VALID_ABxxxxxx);
    /*p25.TOLE*/ wire CPU_VRAM_RD2 = mux2(MCS_Cn, CPU_VRAM_RD, DBG_TUTO);
    /*p25.ROPY*/ wire RENDERINGo = not(b.vid.RENDERING);
    /*p25.SERE*/ wire SERE = and(CPU_VRAM_RD2, RENDERINGo);

    /*p04.DECY*/ wire FROM_CPU5n = not(cpu_in.FROM_CPU5);
    /*p04.CATY*/ wire FROM_CPU5  = not(FROM_CPU5n);
    /*p28.MYNU*/ wire CPU_READ_MYSTERYn = nand(b.sys.CPU_RD2, FROM_CPU5);
    /*p28.LEKO*/ wire CPU_READ_MYSTERY  = not(CPU_READ_MYSTERYn);
    /*p25.TYVY*/ wire MD_TO_Dn = nand(SERE, CPU_READ_MYSTERY);
    /*p25.SEBY*/ wire MD_TO_D = not(MD_TO_Dn);

    /*p25.RUGA*/ if (MD_TO_D) c.D0 = /*p25.RERY*/ not(b.MD0);
    /*p25.ROTA*/ if (MD_TO_D) c.D1 = /*p25.RUNA*/ not(b.MD1);
    /*p25.RYBU*/ if (MD_TO_D) c.D2 = /*p25.RONA*/ not(b.MD2);
    /*p25.RAJU*/ if (MD_TO_D) c.D3 = /*p25.RUNO*/ not(b.MD3);
    /*p25.TYJA*/ if (MD_TO_D) c.D4 = /*p25.SANA*/ not(b.MD4);
    /*p25.REXU*/ if (MD_TO_D) c.D5 = /*p25.RORO*/ not(b.MD5);
    /*p25.RUPY*/ if (MD_TO_D) c.D6 = /*p25.RABO*/ not(b.MD6);
    /*p25.TOKU*/ if (MD_TO_D) c.D7 = /*p25.SAME*/ not(b.MD7);
  }

  //----------
  // Window Y match

  {
    /*p27.MYLO*/ wire WX_MATCH0n = xor(b.vid.X0, b.vid.WX0);
    /*p27.PUWU*/ wire WX_MATCH1n = xor(b.vid.X1, b.vid.WX1);
    /*p27.PUHO*/ wire WX_MATCH2n = xor(b.vid.X2, b.vid.WX2);
    /*p27.NYTU*/ wire WX_MATCH3n = xor(b.vid.X3, b.vid.WX3);
    /*p27.NEZO*/ wire WX_MATCH4n = xor(b.vid.X4, b.vid.WX4);
    /*p27.NORY*/ wire WX_MATCH5n = xor(b.vid.X5, b.vid.WX5);
    /*p27.NONO*/ wire WX_MATCH6n = xor(b.vid.X6, b.vid.WX6);
    /*p27.PASE*/ wire WX_MATCH7n = xor(b.vid.X7, b.vid.WX7);

    /*p27.NAZE*/ wire WY_MATCH0n = xor(b.vid.V0, b.vid.WY0);
    /*p27.PEBO*/ wire WY_MATCH1n = xor(b.vid.V1, b.vid.WY1);
    /*p27.POMO*/ wire WY_MATCH2n = xor(b.vid.V2, b.vid.WY2);
    /*p27.NEVU*/ wire WY_MATCH3n = xor(b.vid.V3, b.vid.WY3);
    /*p27.NOJO*/ wire WY_MATCH4n = xor(b.vid.V4, b.vid.WY4);
    /*p27.PAGA*/ wire WY_MATCH5n = xor(b.vid.V5, b.vid.WY5);
    /*p27.PEZO*/ wire WY_MATCH6n = xor(b.vid.V6, b.vid.WY6);
    /*p27.NUPA*/ wire WY_MATCH7n = xor(b.vid.V7, b.vid.WY7);


    // huh, the y matcher runs at 1 mhz but the x matcher runs at 4 mhz
    /*p27.PALO*/ wire WY_MATCH_HIn   = nand(b.vid.LCDC_WINEN, WY_MATCH4n, WY_MATCH5n, WY_MATCH6n, WY_MATCH7n);
    /*p27.NELE*/ wire WY_MATCH_HI    = not(WY_MATCH_HIn);
    /*p27.PAFU*/ wire WY_MATCHn      = nand(WY_MATCH_HI, WY_MATCH0n, WY_MATCH1n, WY_MATCH2n, WY_MATCH3n);
    /*p27.ROGE*/ wire WY_MATCH       = not(WY_MATCHn);
    /*p27.SARY*/ c.vid.WY_MATCH_SYNC = tock_pos(a.vid.CLK_AxxxxFGHb, b.vid.CLK_AxxxxFGHb, b.sys.VID_RESETn, b.vid.WY_MATCH_SYNC, WY_MATCH);

    // polarity or gates wrong
    /*p21.PARU*/ wire VBLANK         = not(!b.vid.REG_VBLANK);

    /*p01.PYRY*/ wire VID_RESET4  = not(b.sys.VID_RESETn);
    /*p27.REPU*/ wire IN_FRAME_Y     = nor(VBLANK, VID_RESET4);   // schematic wrong, this is NOR
    /*p27.REJO*/ wire WIN_CHECK_X    = or(b.vid.WY_MATCH_SYNC, IN_FRAME_Y); // another weird or gate. should be AND?

    /*p27.PUKY*/ wire WX_MATCH_HIn   = nand(WIN_CHECK_X, WX_MATCH4n, WX_MATCH5n, WX_MATCH6n, WX_MATCH7n);
    /*p27.NUFA*/ wire WX_MATCH_HI    = not (WX_MATCH_HIn);
    /*p27.NOGY*/ wire WIN_MATCHn     = nand(WX_MATCH_HI, WX_MATCH0n, WX_MATCH1n, WX_MATCH2n, WX_MATCH3n);
    /*p27.NUKO*/ c.vid.WIN_MATCH     = not (WIN_MATCHn);

    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(b.vid.FINE_CNT0, b.vid.FINE_CNT1, b.vid.FINE_CNT2);
    /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(b.vid.WIN_MATCH, FINE_COUNT_STOPn);

    /*p27.RYFA*/ c.vid.WIN_MATCH_ONSCREEN_SYNC1 = tock_pos(a.vid.SEGU_4M,       b.vid.SEGU_4M,       b.vid.RENDERING, b.vid.WIN_MATCH_ONSCREEN_SYNC1, WIN_MATCH_ONSCREEN);
    /*p27.RENE*/ c.vid.WIN_MATCH_ONSCREEN_SYNC2 = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.vid.RENDERING, b.vid.WIN_MATCH_ONSCREEN_SYNC2, b.vid.WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.SEKO*/ c.vid.WIN_TRIGGER = nor(b.vid.WIN_MATCH_ONSCREEN_SYNC2, !b.vid.WIN_MATCH_ONSCREEN_SYNC1);

    /*p27.ROCO*/ c.vid.ROCO_4M = not(b.vid.SEGU_4M);
    /*p27.PYCO*/ c.vid.WIN_MATCH_SYNC1 = tock_pos(a.vid.ROCO_4M,       b.vid.ROCO_4M,       b.sys.VID_RESETn, b.vid.WIN_MATCH_SYNC1, b.vid.WIN_MATCH);

    /*p27.MEHE*/ c.vid.CLK_xBxDxFxHb = not(b.sys.CLK_AxCxExGx4);
    /*p27.NUNU*/ c.vid.WIN_MATCH_SYNC2 = tock_pos(a.vid.CLK_xBxDxFxHb, b.vid.CLK_xBxDxFxHb, b.sys.VID_RESETn, b.vid.WIN_MATCH_SYNC2, b.vid.WIN_MATCH_SYNC1);
  }

  //----------
  // Fine x matcher for x scroll

  {


    /*p27.SUHA*/ wire FINE_MATCH0n = xor(b.vid.SCX0, b.vid.FINE_CNT0);
    /*p27.SYBY*/ wire FINE_MATCH1n = xor(b.vid.SCX1, b.vid.FINE_CNT1);
    /*p27.SOZU*/ wire FINE_MATCH2n = xor(b.vid.SCX2, b.vid.FINE_CNT2);

    /*p27.PAHA*/ wire PAHA = not(b.vid.RENDERING);
    /*p27.ROXY*/ wire STOP_THE_PIPE = or(PAHA, b.vid.FINE_MATCH_TRIG);
    /*p27.RONE*/ wire FINE_MATCH  = nand(STOP_THE_PIPE, FINE_MATCH0n, FINE_MATCH1n, FINE_MATCH2n);
    /*p27.POHU*/ wire FINE_MATCHn = not(FINE_MATCH);

    /*p24.ROXO*/ c.vid.FINE_MATCH_CLK   = not(b.vid.SEGU_4M);
    /*p27.PUXA*/ c.vid.FINE_MATCH_SYNC1 = tock_pos(a.vid.FINE_MATCH_CLK, b.vid.FINE_MATCH_CLK, b.vid.RENDERING,  b.vid.FINE_MATCH_SYNC1, FINE_MATCHn);

    /*p27.MOXE*/ c.vid.CLK_xBxDxFxHa = not(b.sys.CLK_AxCxExGx4);
    /*p27.NYZE*/ c.vid.FINE_MATCH_SYNC2 = tock_pos(a.vid.CLK_xBxDxFxHa,  b.vid.CLK_xBxDxFxHa,  b.vid.RENDERING,  b.vid.FINE_MATCH_SYNC2, b.vid.FINE_MATCH_SYNC1);

    /*p27.POVA*/ c.vid.FINE_MATCH_TRIG  = and(b.vid.FINE_MATCH_SYNC1, !b.vid.FINE_MATCH_SYNC2);
  }

  {
    /*p25.ROPY*/ wire RENDERINGo = not(b.vid.RENDERING);
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(b.vid.FINE_CNT0, b.vid.FINE_CNT1, b.vid.FINE_CNT2);
    /*p27.PASO*/ wire FINE_RST = nor(b.vid.MAP_X_CLK_STOPn, RENDERINGo);

    /*p27.PECU*/ c.vid.FINE_CLK = nand(b.vid.FINE_MATCH_CLK, FINE_COUNT_STOPn);
    /*p27.RYKU*/ c.vid.FINE_CNT0 = tock_pos( a.vid.FINE_CLK,   b.vid.FINE_CLK,  FINE_RST, b.vid.FINE_CNT0, !b.vid.FINE_CNT0);
    /*p27.ROGA*/ c.vid.FINE_CNT1 = tock_pos(!a.vid.FINE_CNT0, !b.vid.FINE_CNT0, FINE_RST, b.vid.FINE_CNT1, !b.vid.FINE_CNT1);
    /*p27.RUBU*/ c.vid.FINE_CNT2 = tock_pos(!a.vid.FINE_CNT1, !b.vid.FINE_CNT1, FINE_RST, b.vid.FINE_CNT2, !b.vid.FINE_CNT2);
  }


  //----------
  // BG/WIN sequencer

  {
    /*p27.NYFO*/ wire WIN_RST_TRIGn = not(b.vid.WIN_RST_TRIG);
    /*p27.MOSU*/ wire WIN_RST_TRIGb = not(WIN_RST_TRIGn);

    // counts to 5? polarity?
    /*p27.MOCE*/ wire BG_SEQ_5n = nand(b.vid.BG_SEQ_x1x3x5x7, b.vid.BG_SEQ_xxxx4567, b.vid.BG_SEQ_RSTn);
    /*p27.LYRY*/ c.vid.BG_SEQ_5 = not(BG_SEQ_5n);
    /*p27.NYXU*/ c.vid.BG_SEQ_RSTn = nor(b.spr.SCAN_DONE_TRIG, WIN_RST_TRIGb, b.vid.MAP_X_CLK_STOPn);

    /*p27.LEBO*/ c.vid.BG_SEQ_CLK = nand(b.sys.CLK_AxCxExGx4, BG_SEQ_5n);
    /*p27.LAXU*/ c.vid.BG_SEQ_x1x3x5x7 = tock_pos( a.vid.BG_SEQ_CLK,      b.vid.BG_SEQ_CLK,       b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ_x1x3x5x7, !b.vid.BG_SEQ_x1x3x5x7);
    /*p27.MESU*/ c.vid.BG_SEQ_xx23xx67 = tock_pos(!a.vid.BG_SEQ_x1x3x5x7, !b.vid.BG_SEQ_x1x3x5x7, b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ_xx23xx67, !b.vid.BG_SEQ_xx23xx67);
    /*p27.NYVA*/ c.vid.BG_SEQ_xxxx4567 = tock_pos(!a.vid.BG_SEQ_xx23xx67, !b.vid.BG_SEQ_xx23xx67, b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ_xxxx4567, !b.vid.BG_SEQ_xxxx4567);

    /*p27.NAKO*/ wire BG_SEQ_01xx45xx  = not(b.vid.BG_SEQ_xx23xx67);
    /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(b.vid.BG_SEQ_xxxx4567);
    /*p27.NOGU*/ c.vid.BG_SEQ_xx234567 = nand(BG_SEQ_01xx45xx, BG_SEQ_0123xxxx);

    /*p27.LOVY*/ c.vid.BG_SEQ5_SYNC          = tock_pos(a.vid.CLK_xBxDxFxHc,  b.vid.CLK_xBxDxFxHc, b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ5_SYNC,           b.vid.BG_SEQ_5);

    /*p27.LURY*/ wire LURY = and(!c.vid.BG_SEQ5_SYNC, b.vid.RENDERING);
    /*p27.LONY*/ wire LONY = and(LURY, b.vid.BG_SEQ_RSTn);
    /*p27.LUSU*/ wire LONYn = not(LONY);
    /*p27.LENA*/ c.vid.LONYb = not(LONYn);

    /*p24.NAFY*/ wire RENDERING_AND_NOT_WIN_TRIG = nor(WIN_RST_TRIGb, b.vid.RENDERINGn);
    /*p24.NYKA*/ c.vid.BG_SEQ_6 = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, RENDERING_AND_NOT_WIN_TRIG, b.vid.BG_SEQ_6, b.vid.BG_SEQ_5);
    /*p24.PORY*/ c.vid.BG_SEQ_7 = tock_pos(a.vid.CLK_xBxDxFxHc, b.vid.CLK_xBxDxFxHc, RENDERING_AND_NOT_WIN_TRIG, b.vid.BG_SEQ_7, b.vid.BG_SEQ_6);
    /*p24.PYGO*/ c.vid.BG_SEQ_8 = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.vid.RENDERING,            b.vid.BG_SEQ_8, b.vid.BG_SEQ_7);
  }

  {
    /*p27.LYZU*/ c.vid.BG_SEQ_x1x3x5x7_DELAY = tock_pos(a.sys.CLK_AxCxExGx4,  b.sys.CLK_AxCxExGx4, b.vid.RENDERING,   b.vid.BG_SEQ_x1x3x5x7_DELAY,  b.vid.BG_SEQ_x1x3x5x7);

    /*p27.LAXE*/   wire BG_SEQ0n = not(b.vid.BG_SEQ_x1x3x5x7);
    /*p27.MYSO*/ wire BG_SEQ_TRIG_1357 = nor(b.vid.RENDERINGn, BG_SEQ0n, b.vid.BG_SEQ_x1x3x5x7_DELAY);

    /*p27.NOFU*/       wire BG_SEQ_0123xxxx  = not(b.vid.BG_SEQ_xxxx4567);
    /*p27.NYDY*/     wire BG_SEQ_TRIG_3n = nand(BG_SEQ_TRIG_1357, b.vid.BG_SEQ_xx23xx67, BG_SEQ_0123xxxx);
    /*p32.METE*/   wire BG_SEQ_TRIG_3 = not(BG_SEQ_TRIG_3n);
    /*p32.LOMA*/ c.pix.BG_LATCH = not(BG_SEQ_TRIG_3);

    /*p27.NAKO*/         wire BG_SEQ_01xx45xx  = not(b.vid.BG_SEQ_xx23xx67);
    /*p27.MOFU*/       wire MOFU = and(BG_SEQ_TRIG_1357, BG_SEQ_01xx45xx);
    /*p32.LESO*/     wire LESO = not(MOFU);
    /*p32.AJAR*/   wire AJAR = not(LESO);
    /*p32.LABU*/ c.pix.VRAM_TEMP_CLK = not(AJAR);
  }

  //----------

  {
    /*p01.ATAR*/ wire VID_RESET6  = not(b.sys.VID_RESETn);
    /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

    /*p27.XAHY*/ wire NEW_LINEn = not(b.vid.NEW_LINE1);
    /*p27.XOFO*/ wire MAP_X_RST = nand(b.vid.LCDC_WINEN, NEW_LINEn, b.sys.VID_RESETn);
    /*p27.XACO*/ wire MAP_X_RSTn = not(MAP_X_RST);


    /*p27.PORE*/ wire WIN_RSTb = not(c.vid.WAZY);
    /*p27.VETU*/ c.vid.MAP_X_CLK = and(WIN_RSTb, b.vid.MAP_X_CLK_STOPn);
    /*p27.WYKA*/ c.vid.MAP_X0 = tock_pos( a.vid.MAP_X_CLK, b.vid.MAP_X_CLK, MAP_X_RSTn, b.vid.MAP_X0, !b.vid.MAP_X0);
    /*p27.WODY*/ c.vid.MAP_X1 = tock_pos(!a.vid.MAP_X0,    !b.vid.MAP_X0,   MAP_X_RSTn, b.vid.MAP_X1, !b.vid.MAP_X1);
    /*p27.WOBO*/ c.vid.MAP_X2 = tock_pos(!a.vid.MAP_X1,    !b.vid.MAP_X1,   MAP_X_RSTn, b.vid.MAP_X2, !b.vid.MAP_X2);
    /*p27.WYKO*/ c.vid.MAP_X3 = tock_pos(!a.vid.MAP_X2,    !b.vid.MAP_X2,   MAP_X_RSTn, b.vid.MAP_X3, !b.vid.MAP_X3);
    /*p27.XOLO*/ c.vid.MAP_X4 = tock_pos(!a.vid.MAP_X3,    !b.vid.MAP_X3,   MAP_X_RSTn, b.vid.MAP_X4, !b.vid.MAP_X4);

    /*p21.PARU*/ wire VBLANK = not(!b.vid.REG_VBLANK);
    /*p01.PYRY*/ wire VID_RESET4  = not(b.sys.VID_RESETn);
    /*p27.REPU*/ wire Y_RST  = nor(VBLANK, VID_RESET4);   // schematic wrong, this is NOR
    /*p27.SYNY*/ wire Y_RSTn = not(Y_RST);

    /*p27.VYNO*/ c.vid.TILE_Y0 = tock_pos( a.vid.WAZY,      b.vid.WAZY,    Y_RSTn, b.vid.TILE_Y0, !b.vid.TILE_Y0);
    /*p27.VUJO*/ c.vid.TILE_Y1 = tock_pos(!a.vid.TILE_Y0,  !b.vid.TILE_Y0, Y_RSTn,b.vid.TILE_Y1, !b.vid.TILE_Y1);
    /*p27.VYMU*/ c.vid.TILE_Y2 = tock_pos(!a.vid.TILE_Y1,  !b.vid.TILE_Y1, Y_RSTn, b.vid.TILE_Y2, !b.vid.TILE_Y2);
    /*p27.TUFU*/ c.vid.MAP_Y0  = tock_pos(!a.vid.TILE_Y2,  !b.vid.TILE_Y2, Y_RSTn, b.vid.MAP_Y0,  !b.vid.MAP_Y0);
    /*p27.TAXA*/ c.vid.MAP_Y1  = tock_pos(!a.vid.MAP_Y0,   !b.vid.MAP_Y0,  Y_RSTn, b.vid.MAP_Y1,  !b.vid.MAP_Y1);
    /*p27.TOZO*/ c.vid.MAP_Y2  = tock_pos(!a.vid.MAP_Y1,   !b.vid.MAP_Y1,  Y_RSTn, b.vid.MAP_Y2,  !b.vid.MAP_Y2);
    /*p27.TATE*/ c.vid.MAP_Y3  = tock_pos(!a.vid.MAP_Y2,   !b.vid.MAP_Y2,  Y_RSTn, b.vid.MAP_Y3,  !b.vid.MAP_Y3);
    /*p27.TEKE*/ c.vid.MAP_Y4  = tock_pos(!a.vid.MAP_Y3,   !b.vid.MAP_Y3,  Y_RSTn, b.vid.MAP_Y4,  !b.vid.MAP_Y4);
  }

  //----------

  //----------

  {
    /*p27.VYPO*/ wire VYPO = not(b.joy.P10_B);

    //----------
    // vid x/y + scroll x/y

    /*p26.FAFO*/ wire TILE_Y0C  = add_c(b.vid.V0, b.vid.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1C  = add_c(b.vid.V1, b.vid.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2C  = add_c(b.vid.V2, b.vid.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0C   = add_c(b.vid.V3, b.vid.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1C   = add_c(b.vid.V4, b.vid.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2C   = add_c(b.vid.V5, b.vid.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3C   = add_c(b.vid.V6, b.vid.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4C   = add_c(b.vid.V7, b.vid.SCY7, MAP_Y3C);

    /*p26.FAFO*/ wire TILE_Y0S  = add_s(b.vid.V0, b.vid.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1S  = add_s(b.vid.V1, b.vid.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2S  = add_s(b.vid.V2, b.vid.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0S   = add_s(b.vid.V3, b.vid.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1S   = add_s(b.vid.V4, b.vid.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2S   = add_s(b.vid.V5, b.vid.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3S   = add_s(b.vid.V6, b.vid.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4S   = add_s(b.vid.V7, b.vid.SCY7, MAP_Y3C);

    /*p26.ATAD*/ wire TILE_X0C  = add_c(b.vid.X0, b.vid.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1C  = add_c(b.vid.X1, b.vid.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2C  = add_c(b.vid.X2, b.vid.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0C   = add_c(b.vid.X3, b.vid.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1C   = add_c(b.vid.X4, b.vid.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2C   = add_c(b.vid.X5, b.vid.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3C   = add_c(b.vid.X6, b.vid.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4C   = add_c(b.vid.X7, b.vid.SCX7, MAP_X3C);

    /*p26.ATAD*/ wire TILE_X0S  = add_s(b.vid.X0, b.vid.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1S  = add_s(b.vid.X1, b.vid.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2S  = add_s(b.vid.X2, b.vid.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0S   = add_s(b.vid.X3, b.vid.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1S   = add_s(b.vid.X4, b.vid.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2S   = add_s(b.vid.X5, b.vid.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3S   = add_s(b.vid.X6, b.vid.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4S   = add_s(b.vid.X7, b.vid.SCX7, MAP_X3C);

    //----------
    // vram read mux

    {
      /*p25.XANE*/ wire VRAM_LOCK = nor(b.dma.DMA_READ_VRAM, b.vid.RENDERING);
      /*p25.XEDU*/ wire VRAM_LOCKn = not(VRAM_LOCK);

      /*p25.XAKY*/ if (VRAM_LOCKn) c.MA00 = b.A00;
      /*p25.XUXU*/ if (VRAM_LOCKn) c.MA01 = b.A01;
      /*p25.XYNE*/ if (VRAM_LOCKn) c.MA02 = b.A02;
      /*p25.XODY*/ if (VRAM_LOCKn) c.MA03 = b.A03;
      /*p25.XECA*/ if (VRAM_LOCKn) c.MA04 = b.A04;
      /*p25.XOBA*/ if (VRAM_LOCKn) c.MA05 = b.A05;
      /*p25.XOPO*/ if (VRAM_LOCKn) c.MA06 = b.A06;
      /*p25.XYBO*/ if (VRAM_LOCKn) c.MA07 = b.A07;
      /*p25.RYSU*/ if (VRAM_LOCKn) c.MA08 = b.A08;
      /*p25.RESE*/ if (VRAM_LOCKn) c.MA09 = b.A09;
      /*p25.RUSE*/ if (VRAM_LOCKn) c.MA10 = b.A10;
      /*p25.RYNA*/ if (VRAM_LOCKn) c.MA11 = b.A11;
      /*p25.RUMO*/ if (VRAM_LOCKn) c.MA12 = b.A12;
    }

    {
      /*p29.ABON*/ wire SPRITE_READ = not(b.spr.SPRITE_READn);

      /*p29.WUKY*/ wire FLIP_Y = not(b.spr.OAM_A_D6);
      /*p29.XUQU*/ wire SPRITE_AB = not(!b.spr.SPR_DEL1);

      /*p29.WAGO*/ wire WAGO      = xor(FLIP_Y, b.spr.TS_LINE_0);
      /*p29.CYVU*/ wire SPRITE_Y0 = xor(FLIP_Y, b.spr.TS_LINE_1);
      /*p29.BORE*/ wire SPRITE_Y1 = xor(FLIP_Y, b.spr.TS_LINE_2);
      /*p29.BUVY*/ wire SPRITE_Y2 = xor(FLIP_Y, b.spr.TS_LINE_3);

      /*p29.FUFO*/ wire LCDC_SPSIZEn = not(b.vid.LCDC_SPSIZE);
      /*p29.GEJY*/ wire SPRITE_Y3 = amux2(LCDC_SPSIZEn, !b.spr.OAM_B_D0, b.vid.LCDC_SPSIZE,  WAGO);

      /*p29.ABEM*/ if (SPRITE_READ) c.MA00 = SPRITE_AB;
      /*p29.BAXE*/ if (SPRITE_READ) c.MA01 = SPRITE_Y0;
      /*p29.ARAS*/ if (SPRITE_READ) c.MA02 = SPRITE_Y1;
      /*p29.AGAG*/ if (SPRITE_READ) c.MA03 = SPRITE_Y2;
      /*p29.FAMU*/ if (SPRITE_READ) c.MA04 = SPRITE_Y3;
      /*p29.FUGY*/ if (SPRITE_READ) c.MA05 = b.spr.OAM_B_D1;
      /*p29.GAVO*/ if (SPRITE_READ) c.MA06 = b.spr.OAM_B_D2;
      /*p29.WYGA*/ if (SPRITE_READ) c.MA07 = b.spr.OAM_B_D3;
      /*p29.WUNE*/ if (SPRITE_READ) c.MA08 = b.spr.OAM_B_D4;
      /*p29.GOTU*/ if (SPRITE_READ) c.MA09 = b.spr.OAM_B_D5;
      /*p29.GEGU*/ if (SPRITE_READ) c.MA10 = b.spr.OAM_B_D6;
      /*p29.XEHE*/ if (SPRITE_READ) c.MA11 = b.spr.OAM_B_D7;
      /*p29.DYSO*/ if (SPRITE_READ) c.MA12 = b.joy.P10_B;   // sprites always in low half of tile store
    }

    {
      /*p27.PORE*/ wire TILE_Y_CLKn = not(b.vid.WAZY);
      /*p26.AXAD*/ wire TILE_Y_CLK  = not(TILE_Y_CLKn);

      /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(b.vid.BG_SEQ_xxxx4567);
      /*p27.XUHA*/ wire BG_SEQ_xxxx4567b = not(BG_SEQ_0123xxxx);

      // one of these is window

      /*p27.NETA*/ wire TILE_READa = and(b.vid.LONYb, b.vid.BG_SEQ_xx234567);
      /*p25.XUCY*/ wire TILE_READb = nand(TILE_READa, TILE_Y_CLKn);

      /*p26.ASUL*/ wire FETCH_TILEn = and(TILE_Y_CLK, TILE_READa);
      /*p26.BEJE*/ wire FETCH_TILE = not(FETCH_TILEn);

      /*p25.XONU*/ if (TILE_READb) c.MA00 = BG_SEQ_xxxx4567b;
      /*p25.WUDO*/ if (TILE_READb) c.MA01 = b.vid.TILE_Y0;
      /*p25.WAWE*/ if (TILE_READb) c.MA02 = b.vid.TILE_Y1;
      /*p25.WOLU*/ if (TILE_READb) c.MA03 = b.vid.TILE_Y2;

      /*p26.ASUM*/ if (FETCH_TILE) c.MA00 = BG_SEQ_xxxx4567b;
      /*p26.EVAD*/ if (FETCH_TILE) c.MA01 = TILE_Y0S;
      /*p26.DAHU*/ if (FETCH_TILE) c.MA02 = TILE_Y1S;
      /*p26.DODE*/ if (FETCH_TILE) c.MA03 = TILE_Y2S;

      /*p25.VUZA*/ wire TILE_BANK = nor(b.vid.LCDC_BGTILE, b.pix.BG_PIX_B7); // register reused

      /*p25.VAPY*/ if (TILE_READa) c.MA04 = b.pix.BG_PIX_B0; // register reused
      /*p25.SEZU*/ if (TILE_READa) c.MA05 = b.pix.BG_PIX_B1;
      /*p25.VEJY*/ if (TILE_READa) c.MA06 = b.pix.BG_PIX_B2;
      /*p25.RUSA*/ if (TILE_READa) c.MA07 = b.pix.BG_PIX_B3;
      /*p25.ROHA*/ if (TILE_READa) c.MA08 = b.pix.BG_PIX_B4;
      /*p25.RESO*/ if (TILE_READa) c.MA09 = b.pix.BG_PIX_B5;
      /*p25.SUVO*/ if (TILE_READa) c.MA10 = b.pix.BG_PIX_B6;
      /*p25.TOBO*/ if (TILE_READa) c.MA11 = b.pix.BG_PIX_B7;
      /*p25.VUZA*/ if (TILE_READa) c.MA12 = TILE_BANK;

    }

    {
      /*p27.PORE*/ wire BG_OR_WINa = not(b.vid.WAZY);
      /*p26.AXAD*/ wire BG_OR_WINn = not(BG_OR_WINa);

      /*p27.NENY*/ wire BG_SEQ_01xxxxxx = not(b.vid.BG_SEQ_xx234567);
      /*p27.POTU and */ wire POTU = and(b.vid.LONYb, BG_SEQ_01xxxxxx);

      /*p25.XEZE nand*/ wire WIN_MAP_READa = nand(POTU, BG_OR_WINa);
      /*p26.ACEN and */ wire BG_MAP_READa  = and (POTU, BG_OR_WINn);

      /*p25.WUKO*/ wire WIN_MAP_READn = not(WIN_MAP_READa);
      /*p26.BAFY*/ wire BG_MAP_READn  = not(BG_MAP_READa);

      {
        // small tri
        /*p27.XEJA*/ if (!WIN_MAP_READn) c.MA00 = b.vid.MAP_X0;
        /*p27.XAMO*/ if (!WIN_MAP_READn) c.MA01 = b.vid.MAP_X1;
        /*p27.XAHE*/ if (!WIN_MAP_READn) c.MA02 = b.vid.MAP_X2;
        /*p27.XULO*/ if (!WIN_MAP_READn) c.MA03 = b.vid.MAP_X3;
        /*p27.WUJU*/ if (!WIN_MAP_READn) c.MA04 = b.vid.MAP_X4;
        /*p27.VYTO*/ if (!WIN_MAP_READn) c.MA05 = b.vid.MAP_Y0;
        /*p27.VEHA*/ if (!WIN_MAP_READn) c.MA06 = b.vid.MAP_Y1;
        /*p27.VACE*/ if (!WIN_MAP_READn) c.MA07 = b.vid.MAP_Y2;
        /*p27.VOVO*/ if (!WIN_MAP_READn) c.MA08 = b.vid.MAP_Y3;
        /*p27.VULO*/ if (!WIN_MAP_READn) c.MA09 = b.vid.MAP_Y4;
        /*p27.VEVY*/ if (!WIN_MAP_READn) c.MA10 = b.vid.LCDC_WINMAP;
        /*p27.VEZA*/ if (!WIN_MAP_READn) c.MA11 = b.spr.P10_Bn;
        /*p27.VOGU*/ if (!WIN_MAP_READn) c.MA12 = b.spr.P10_Bn;
      }

      {
        // small tri
        /*p26.AXEP*/ if (!BG_MAP_READn) c.MA00 = MAP_X0S;
        /*p26.AFEB*/ if (!BG_MAP_READn) c.MA01 = MAP_X1S;
        /*p26.ALEL*/ if (!BG_MAP_READn) c.MA02 = MAP_X2S;
        /*p26.COLY*/ if (!BG_MAP_READn) c.MA03 = MAP_X3S;
        /*p26.AJAN*/ if (!BG_MAP_READn) c.MA04 = MAP_X4S;
        /*p26.DUHO*/ if (!BG_MAP_READn) c.MA05 = MAP_Y0S;
        /*p26.CASE*/ if (!BG_MAP_READn) c.MA06 = MAP_Y1S;
        /*p26.CYPO*/ if (!BG_MAP_READn) c.MA07 = MAP_Y2S;
        /*p26.CETA*/ if (!BG_MAP_READn) c.MA08 = MAP_Y3S;
        /*p26.DAFE*/ if (!BG_MAP_READn) c.MA09 = MAP_Y4S;
        /*p26.AMUV*/ if (!BG_MAP_READn) c.MA10 = b.vid.LCDC_BGMAP;
        /*p26.COVE*/ if (!BG_MAP_READn) c.MA11 = b.spr.P10_Bn;
        /*p26.COXO*/ if (!BG_MAP_READn) c.MA12 = b.spr.P10_Bn;
      }
    }

    //----------
    // Memory address bus driver pin driver

    /*p25.LEXE*/ c.pin.MA00 = not(/*p25.MYFU*/ not(b.MA00));
    /*p25.LOZU*/ c.pin.MA01 = not(/*p25.MASA*/ not(b.MA01));
    /*p25.LACA*/ c.pin.MA02 = not(/*p25.MYRE*/ not(b.MA02));
    /*p25.LUVO*/ c.pin.MA03 = not(/*p25.MAVU*/ not(b.MA03));
    /*p25.LOLY*/ c.pin.MA04 = not(/*p25.MEPA*/ not(b.MA04));
    /*p25.LALO*/ c.pin.MA05 = not(/*p25.MYSA*/ not(b.MA05));
    /*p25.LEFA*/ c.pin.MA06 = not(/*p25.MEWY*/ not(b.MA06));
    /*p25.LUBY*/ c.pin.MA07 = not(/*p25.MUME*/ not(b.MA07));
    /*p25.TUJY*/ c.pin.MA08 = not(/*p25.VOVA*/ not(b.MA08));
    /*p25.TAGO*/ c.pin.MA09 = not(/*p25.VODE*/ not(b.MA09));
    /*p25.NUVA*/ c.pin.MA10 = not(/*p25.RUKY*/ not(b.MA10));
    /*p25.PEDU*/ c.pin.MA11 = not(/*p25.RUMA*/ not(b.MA11));
    /*p25.PONY*/ c.pin.MA12 = not(/*p25.REHO*/ not(b.MA12));
  }
}

//-----------------------------------------------------------------------------

};