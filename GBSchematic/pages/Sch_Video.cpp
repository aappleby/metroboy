#include "Sch_Video.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct VideoIn {
  bool ADDR_VALID;
  bool NEW_LINE1;
  bool SPRITE_DONE;
  bool STORE_MATCH;
  bool SCAN_DONE_TRIG;
  bool CPU_RD2;
  bool CPU_WR2;
  bool REG_VBLANK;
  bool OAM_ADDR_PARSEn;
  bool LINE_ENDo;
};

void P21_VideoControl_tick(const VideoIn& in,
                           const Bus& bus_in,
                           const Pins& pins,
                           const LCD& lcd,
                           const Registers& regs,
                           const Clocks& clocks,
                           const Resets& resets,
                           const Decoder& dec,
                           const Debug& dbg,
                           const Video& prev,
                           Video& next,
                           Bus& bus_out) {

  /*p27.VYPO*/ wire VYPO = not(pins.P10_B);

  /*p01.PYRY*/ wire VID_RESET4  = not(resets.VID_RESETn);
  /*p01.ATAR*/ wire VID_RESET6  = not(resets.VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

  {
    /*p08.SORE*/ wire SORE = not(bus_in.A15);
    /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(bus_in.A13, bus_in.A14, SORE);
    /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(in.ADDR_VALID, ADDR_NOT_VRAM);
    /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(bus_in.A15, bus_in.A14, bus_in.A13, bus_in.A12, bus_in.A11, bus_in.A10, bus_in.A09);
    /*p25.SYRO*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FE00);
    /*p25.TEFA*/ wire TEFA = nor(ADDR_FE00_FFFF, ADDR_VALID_AND_NOT_VRAM);
    /*p25.SOSE*/ next.ADDR_VRAM = and(bus_in.A15, TEFA);
  }

  //----------

#if 0
  /*p27.MYVO*/ next.MYVO_xBxDxFxH = not(clocks.ALET_AxCxExGx);
  /*p29.LAPE*/ next.LAPE_xBxDxFxH = not(clocks.ALET_AxCxExGx);

  {
    /*p01.ATAL*/ wire ATAL_AxCxExGx  = not(clocks.ROOTCLK_xBxDxFxH);
    /*p01.AZOF*/ wire AZOF_xBxDxFxH  = not(ATAL_AxCxExGx);
    /*p01.ZAXY*/ wire ZAXY_AxCxExGx  = not(AZOF_xBxDxFxH);
    /*p01.ZEME*/ wire ZEME_xBxDxFxH  = not(ZAXY_AxCxExGx);
    /*p29.XYVA*/ wire XYVA_AxCxExGx  = not(ZEME_xBxDxFxH);
    /*p29.XOTA*/ next.XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
    /*p29.WUVU*/ next.WUVU_AxxDExxH = tock_pos( a.vid.XOTA_xBxDxFxH,  prev.XOTA_xBxDxFxH, prev.rst.VID_RESETn, prev.WUVU_AxxDExxH, !prev.WUVU_AxxDExxH);
    /*p21.VENA*/ next.VENA_AxxxxFGH = tock_pos(!a.vid.WUVU_AxxDExxH, !prev.WUVU_AxxDExxH, prev.rst.VID_RESETn, prev.VENA_AxxxxFGH, !prev.VENA_AxxxxFGH);
  }
#endif

  //----------
  // x counter. this is a little weird, presumably because it can tick at 4 mhz but not always?

  {
    /*p01.TOFU*/ wire VID_RESET3  = not(resets.VID_RESETn);
    /*p21.TADY*/ next.X_RST = nor(in.NEW_LINE1, VID_RESET3);

    /*p21.RYBO*/ wire RYBO = xor(prev.X0, prev.X1);
    /*p21.XUKE*/ wire XUKE = and(prev.X0, prev.X1);

    /*p21.XYLE*/ wire XYLE = and(prev.X2, XUKE);
    /*p21.XEGY*/ wire XEGY = xor(prev.X2, XUKE);
    /*p21.XORA*/ wire XORA = xor(prev.X3, XYLE);

    /*p21.XEHO*/ next.X0.tock(prev.CLKPIPE, prev.X_RST, !prev.X0);
    /*p21.SAVY*/ next.X1.tock(prev.CLKPIPE, prev.X_RST, RYBO);
    /*p21.XODU*/ next.X2.tock(prev.CLKPIPE, prev.X_RST, XEGY);
    /*p21.XYDO*/ next.X3.tock(prev.CLKPIPE, prev.X_RST, XORA);

    /*p21.SAKE*/ wire SAKE = xor(prev.X4, prev.X5);
    /*p21.TYBA*/ wire TYBA = and(prev.X4, prev.X5);
    /*p21.SURY*/ wire SURY = and(prev.X6, TYBA);
    /*p21.TYGE*/ wire TYGE = xor(prev.X6, TYBA);
    /*p21.ROKU*/ wire ROKU = xor(prev.X7, SURY);

    /*p21.TOCA*/ next.TOCA = not(prev.X3);
    /*p21.TUHU*/ next.X4.tock(prev.TOCA, prev.X_RST, !prev.X4);
    /*p21.TUKY*/ next.X5.tock(prev.TOCA, prev.X_RST, SAKE);
    /*p21.TAKO*/ next.X6.tock(prev.TOCA, prev.X_RST, TYGE);
    /*p21.SYBE*/ next.X7.tock(prev.TOCA, prev.X_RST, ROKU);

    /*p21.XUGU*/ next.X_167n = nand(prev.X0, prev.X1, prev.X2, prev.X5, prev.X7); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ next.X_167 = not(prev.X_167n);
  }

  //----------
  // LY compare

  {
    /*p01.DULA*/ wire DULA_RESET = not(resets.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.WESY*/ wire SYS_RESETn6 = not(XORE_RESET); // video

    /*p21.SYFU*/ wire LY_MATCH7 = xor(lcd.V7, regs.LYC7);
    /*p21.TERY*/ wire LY_MATCH6 = xor(lcd.V6, regs.LYC6);
    /*p21.TUCY*/ wire LY_MATCH5 = xor(lcd.V5, regs.LYC5);
    /*p21.TYKU*/ wire LY_MATCH4 = xor(lcd.V4, regs.LYC4);
    /*p21.RASY*/ wire LY_MATCH3 = xor(lcd.V3, regs.LYC3);
    /*p21.REDA*/ wire LY_MATCH2 = xor(lcd.V2, regs.LYC2);
    /*p21.TYDE*/ wire LY_MATCH1 = xor(lcd.V1, regs.LYC1);
    /*p21.RYME*/ wire LY_MATCH0 = xor(lcd.V0, regs.LYC0);

    /*p21.SOVU*/ wire LY_MATCHA = nor(LY_MATCH7, LY_MATCH6, LY_MATCH5, LY_MATCH4);
    /*p21.SUBO*/ wire LY_MATCHB = nor(LY_MATCH3, LY_MATCH2, LY_MATCH1, LY_MATCH0);

    /*p21.RAPE*/ wire LY_MATCHn = nand(LY_MATCHA, LY_MATCHB);
    /*p21.PALY*/ wire LY_MATCHa = not(LY_MATCHn);

    /*p21.ROPO*/ next.LYC_MATCH.tock(clocks.TALU_AxxxxFGH, SYS_RESETn6, LY_MATCHa);
  }

  //----------

  {

    /*p27.XAHY*/ wire NEW_LINEn = not(in.NEW_LINE1);
    /*p27.XOFO*/ wire MAP_X_RST = nand(regs.LCDC_WINEN, NEW_LINEn, resets.VID_RESETn);
    /*p27.PYNU*/ next.WIN_RST = or(prev.WIN_MATCH_SYNC2, MAP_X_RST);
    /*p27.NOPA*/ next.WIN_RST_SYNC.tock(clocks.ALET_AxCxExGx, resets.VID_RESETn, prev.WIN_RST);

    /*p27.NUNY*/ next.WIN_RST_TRIG = and(prev.WIN_RST, !prev.WIN_RST_SYNC);

    // PUKU/RYDY form a NOR latch. WIN_RST_TRIG is SET, (VID_RESET | BG_SEQ_7) is RESET.
    /*p27.PUKU*/ next.PUKU = nor(prev.RYDY, prev.WIN_RST_TRIG);
    /*p27.RYDY*/ next.RYDY = nor(prev.PUKU, VID_RESET4, prev.BG_SEQ_7);
    /*p27.SOVY*/ next.REG_SOVY.tock(clocks.ALET_AxCxExGx, resets.VID_RESETn, prev.RYDY);
  }

  {
    /*p24.POKY*/ wire POKY = or(prev.BG_SEQ_8, prev.RENDERINGn);
    /*p27.ROMO*/ wire ROMO = not(POKY);
    /*p27.SYLO*/ wire SYLO = not(prev.RYDY);

    /*p27.SUVU*/   wire SUVU = nand(prev.RENDERING, ROMO, prev.BG_SEQ_6, prev.BG_SEQ_7);
    /*p27.TAVE*/ wire TAVE = not(SUVU);

    /*p27.TUXY*/     wire TUXY = nand(prev.REG_SOVY, SYLO);
    /*p27.SUZU*/   wire SUZU = not(TUXY);
    /*p27.TEVO*/ next.MAP_X_CLK_STOPn = nor(prev.WIN_TRIGGER, SUZU, TAVE);

    /*p01.ROSY*/ wire VID_RESET5  = not(resets.VID_RESETn);

    /*p24.TOMU*/     wire TOMU = not(SYLO);
    /*p27.TUKU*/   wire TUKU = not(TOMU);
    /*p27.VEKU*/       wire VEKU = nor(in.SPRITE_DONE, TAVE);
    /*p27.RYCE*/         wire SPRITE_TRIG = and(prev.TEKY_SYNC1, !prev.TEKY_SYNC2);
    /*p27.SECA*/       wire SPR_SEQ_RST = nor(SPRITE_TRIG, VID_RESET5, in.NEW_LINE1);
    /*p27.TAKA*/     wire TAKA = unk2(VEKU, SPR_SEQ_RST);
    /*p27.SOWO*/   wire SOWO = not(TAKA);
    /*p27.TEKY*/ wire TEKY = and(in.STORE_MATCH, TUKU, prev.BG_SEQ_5, SOWO);
    /*p27.SOBU*/ next.TEKY_SYNC1.tock(clocks.TAVA_AxCxExGx, VYPO, TEKY);
    /*p27.SUDA*/ next.TEKY_SYNC2.tock(clocks.LAPE_xBxDxFxH, VYPO, prev.TEKY_SYNC1);

    /*p24.SOCY*/ wire SOCY = not(TOMU);
    /*p24.VYBO*/ wire VYBO = nor(in.STORE_MATCH, prev.RENDER_DONE, clocks.MYVO_xBxDxFxH);
    /*p24.TYFA*/ wire TYFA = and(SOCY, POKY, VYBO);

    /*p24.SEGU*/ next.SEGU_4M = not(TYFA);
    /*p24.ROXO*/ next.ROXO_4M = not(prev.SEGU_4M);
  }

  {
    /*p27.PAHA*/ wire PAHA = not(prev.RENDERING);
    /*p27.ROXY*/ wire STOP_THE_PIPE = or(PAHA, prev.FINE_MATCH_TRIG);
    /*p24.SACU*/ next.CLKPIPE = nor(prev.SEGU_4M, STOP_THE_PIPE);
  }


  //----------
  // FF41 STAT

  
  //---

  {
    /*p01.TOFU*/ wire VID_RESET3  = not(resets.VID_RESETn);
    /*p21.WEGO or */ wire WEGO = or(VID_RESET3, prev.RENDER_DONE_SYNC);
    /*p21.XYMU ???*/ next.RENDERING = or(WEGO, in.SCAN_DONE_TRIG); // the trig here doesn't make sense
    
    /*p24.LOBY not*/ next.RENDERINGn = not(prev.RENDERING);

    /*p21.XENA*/ wire STORE_MATCHn = not(in.STORE_MATCH);
    /*p21.WODU*/ next.RENDER_DONE = and(STORE_MATCHn, prev.X_167);
    /*p21.VOGA*/ next.RENDER_DONE_SYNC.tock(clocks.ALET_AxCxExGx, prev.X_RST, prev.RENDER_DONE);
  }


  //---


  //----------
  // FF41 STAT stuff

  {
    /*p01.DULA*/ wire DULA_RESET = not(resets.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);
    /*p01.WESY*/ wire WESY_RESET = not(XORE_RESET);

    /*p22.WOFA*/ wire FF41n   = nand(prev.FF4X, prev.WADO_A00,  prev.A01n, prev.A02n, prev.A03n);
    /*p22.VARY*/ wire FF41    = not(FF41n);
    /*p21.SEPA*/ wire FF41_WR = and(in.CPU_WR2, FF41);

    /*p21.RYVE*/ wire CLK_STAT= not(FF41_WR);
    /*p21.ROXE*/ next.INT_HBL_EN.tock(CLK_STAT, WESY_RESET, bus_in.D3);
    /*p21.RUFO*/ next.INT_VBL_EN.tock(CLK_STAT, WESY_RESET, bus_in.D4);
    /*p21.REFE*/ next.INT_OAM_EN.tock(CLK_STAT, WESY_RESET, bus_in.D5);
    /*p21.RUGU*/ next.INT_LYC_EN.tock(CLK_STAT, WESY_RESET, bus_in.D6);

    /*p21.PARU*/ wire VBLANK = not(!in.REG_VBLANK);

    // 11: hblank   - rendering 0, vbl 0, oam 0
    // 10: vblank   - rendering 0, vbl 1, oam 0
    // 01: oam scan - rendering 0, vbl 0, oam 1
    // 00: render   - rendering 1, vbl 0, oam 0
    // so one of these has the wrong polarity

    /*p21.SADU*/ wire STAT_MODE0n = nor(prev.RENDERING, VBLANK);
    /*p21.XATY*/ wire STAT_MODE1n = nor(prev.RENDERING, in.OAM_ADDR_PARSEn);

    /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);

    /*p01.WESY*/ wire SYS_RESETn6 = not(XORE_RESET);
    /*p21.PAGO*/ wire STAT_LYC_MATCH1 = nor(SYS_RESETn6, FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ wire STAT_LYC_MATCH2 = or(prev.LYC_MATCH, STAT_LYC_MATCH1); // this is another of the weird or gates. could be nor?

    /*p21.TOBE*/ wire FF41_RDa = and(in.CPU_RD2, FF41);
    /*p21.VAVE*/ wire FF41_RDb = FF41_RDa; // buffer, not inverter?
    /*p21.TEBY*/ if (FF41_RDa) bus_out.D0 = not(STAT_MODE0n);
    /*p21.WUGA*/ if (FF41_RDa) bus_out.D1 = not(STAT_MODE1n);
    /*p21.SEGO*/ if (FF41_RDa) bus_out.D2 = not(STAT_LYC_MATCH2);
    /*p21.PUZO*/ if (FF41_RDb) bus_out.D3 = prev.INT_HBL_EN;
    /*p21.POFO*/ if (FF41_RDb) bus_out.D4 = prev.INT_VBL_EN;
    /*p21.SASY*/ if (FF41_RDb) bus_out.D5 = prev.INT_OAM_EN;
    /*p21.POTE*/ if (FF41_RDb) bus_out.D6 = prev.INT_LYC_EN;

    /*p21.PURE*/ wire LINE_DONEa = not(in.LINE_ENDo);
    /*p21.SELA*/ wire LINE_DONEo = not(LINE_DONEa);
    /*p21.TOLU*/ wire INT_VBLn = not(VBLANK);
    /*p21.TAPA*/ wire INT_OAM = and(INT_VBLn, LINE_DONEo);
    /*p21.TARU*/ wire INT_HBL = and(INT_VBLn, prev.RENDER_DONE);

    /*p21.SUKO*/ wire INT_STATb = amux4(prev.INT_LYC_EN, prev.LYC_MATCH,
                                        prev.INT_OAM_EN, INT_OAM,
                                        prev.INT_VBL_EN, VBLANK, // polarity?
                                        prev.INT_HBL_EN, INT_HBL);

    /*p21.TUVA*/ wire INT_STATn = not(INT_STATb);
    /*p21.VOTY*/ next.INT_STAT  = not(INT_STATn);
  }

  //----------

  /*p22.XALY*/ next.ADDR_0x00xxxx = nor(bus_in.A07, bus_in.A05, bus_in.A04);
  /*p22.WUTU*/ next.FF4Xn         = nand(dec.ADDR_FFXX, bus_in.A06, prev.ADDR_0x00xxxx);
  /*p22.WERO*/ next.FF4X          = not(prev.FF4Xn);

  /*p22.XOLA*/ next.A00n = not(bus_in.A00);
  /*p22.XENO*/ next.A01n = not(bus_in.A01);
  /*p22.XUSY*/ next.A02n = not(bus_in.A02);
  /*p22.XERA*/ next.A03n = not(bus_in.A03);

  /*p22.WADO*/ next.WADO_A00 = not(prev.A00n);
  /*p22.WESA*/ next.WESA_A01 = not(prev.A01n);
  /*p22.WALO*/ next.WALO_A02 = not(prev.A02n);
  /*p22.WEPO*/ next.WEPO_A03 = not(prev.A03n);



  //----------
  // FF44 LY

  {
    /*p22.WYLE*/ wire FF44n = nand(prev.FF4X, prev.A00n, prev.A01n, prev.WALO_A02,  prev.A03n);
    /*p22.XOGY*/ wire FF44 = not(FF44n);
    /*p23.WAFU*/ wire FF44_RD = and(in.CPU_RD2, FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(lcd.V0);
    /*p23.XEPO*/ wire LY1n = not(lcd.V1);
    /*p23.MYFA*/ wire LY2n = not(lcd.V2);
    /*p23.XUHY*/ wire LY3n = not(lcd.V3);
    /*p23.WATA*/ wire LY4n = not(lcd.V4);
    /*p23.XAGA*/ wire LY5n = not(lcd.V5);
    /*p23.XUCE*/ wire LY6n = not(lcd.V6);
    /*p23.XOWO*/ wire LY7n = not(lcd.V7);

    /*p23.VEGA*/ if (FF44_RDn) bus_out.D0 = not(LY0n);
    /*p23.WUVA*/ if (FF44_RDn) bus_out.D1 = not(LY1n);
    /*p23.LYCO*/ if (FF44_RDn) bus_out.D2 = not(LY2n);
    /*p23.WOJY*/ if (FF44_RDn) bus_out.D3 = not(LY3n);
    /*p23.VYNE*/ if (FF44_RDn) bus_out.D4 = not(LY4n);
    /*p23.WAMA*/ if (FF44_RDn) bus_out.D5 = not(LY5n);
    /*p23.WAVO*/ if (FF44_RDn) bus_out.D6 = not(LY6n);
    /*p23.WEZE*/ if (FF44_RDn) bus_out.D7 = not(LY7n);
  }

  //----------
  // more debug stuff

  {
    /*p01.DULA*/ wire DULA_RESET = not(resets.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET = not(CUNU_RESET);

    /*p25.TUSO*/ wire TUSO = nor(dbg.MODE_DBG2, clocks.BOGA_xBCDEFGH);
    /*p25.SOLE*/ wire SOLE = not(TUSO);

    /*p25.TOVU*/ if (VYPO) bus_out.D0 = SOLE;
    /*p25.SOSA*/ if (VYPO) bus_out.D1 = SOLE;
    /*p25.SEDU*/ if (VYPO) bus_out.D2 = SOLE;
    /*p25.TAXO*/ if (VYPO) bus_out.D3 = SOLE;
    /*p25.TAHY*/ if (VYPO) bus_out.D4 = SOLE;
    /*p25.TESU*/ if (VYPO) bus_out.D5 = SOLE;
    /*p25.TAZU*/ if (VYPO) bus_out.D6 = SOLE;
    /*p25.TEWA*/ if (VYPO) bus_out.D7 = SOLE;
  }

  //----------

  //----------
  // Window Y match

  {
    /*p27.MYLO*/ wire WX_MATCH0n = xor(prev.X0, regs.WX0);
    /*p27.PUWU*/ wire WX_MATCH1n = xor(prev.X1, regs.WX1);
    /*p27.PUHO*/ wire WX_MATCH2n = xor(prev.X2, regs.WX2);
    /*p27.NYTU*/ wire WX_MATCH3n = xor(prev.X3, regs.WX3);
    /*p27.NEZO*/ wire WX_MATCH4n = xor(prev.X4, regs.WX4);
    /*p27.NORY*/ wire WX_MATCH5n = xor(prev.X5, regs.WX5);
    /*p27.NONO*/ wire WX_MATCH6n = xor(prev.X6, regs.WX6);
    /*p27.PASE*/ wire WX_MATCH7n = xor(prev.X7, regs.WX7);

    /*p27.NAZE*/ wire WY_MATCH0n = xor(lcd.V0, regs.WY0);
    /*p27.PEBO*/ wire WY_MATCH1n = xor(lcd.V1, regs.WY1);
    /*p27.POMO*/ wire WY_MATCH2n = xor(lcd.V2, regs.WY2);
    /*p27.NEVU*/ wire WY_MATCH3n = xor(lcd.V3, regs.WY3);
    /*p27.NOJO*/ wire WY_MATCH4n = xor(lcd.V4, regs.WY4);
    /*p27.PAGA*/ wire WY_MATCH5n = xor(lcd.V5, regs.WY5);
    /*p27.PEZO*/ wire WY_MATCH6n = xor(lcd.V6, regs.WY6);
    /*p27.NUPA*/ wire WY_MATCH7n = xor(lcd.V7, regs.WY7);


    // huh, the y matcher runs at 1 mhz but the x matcher runs at 4 mhz
    /*p27.PALO*/ wire WY_MATCH_HIn   = nand(regs.LCDC_WINEN, WY_MATCH4n, WY_MATCH5n, WY_MATCH6n, WY_MATCH7n);
    /*p27.NELE*/ wire WY_MATCH_HI    = not(WY_MATCH_HIn);
    /*p27.PAFU*/ wire WY_MATCHn      = nand(WY_MATCH_HI, WY_MATCH0n, WY_MATCH1n, WY_MATCH2n, WY_MATCH3n);
    /*p27.ROGE*/ wire WY_MATCH       = not(WY_MATCHn);
    /*p27.SARY*/ next.WY_MATCH_SYNC.tock(clocks.TALU_AxxxxFGH, resets.VID_RESETn, WY_MATCH);

    // polarity or gates wrong
    /*p21.PARU*/ wire VBLANK         = not(!lcd.REG_VBLANK);

    /*p27.REPU*/ wire IN_FRAME_Y     = nor(VBLANK, VID_RESET4);   // schematic wrong, this is NOR
    /*p27.REJO*/ wire WIN_CHECK_X    = or(prev.WY_MATCH_SYNC, IN_FRAME_Y); // another weird or gate. should be AND?

    /*p27.PUKY*/ wire WX_MATCH_HIn   = nand(WIN_CHECK_X, WX_MATCH4n, WX_MATCH5n, WX_MATCH6n, WX_MATCH7n);
    /*p27.NUFA*/ wire WX_MATCH_HI    = not (WX_MATCH_HIn);
    /*p27.NOGY*/ wire WIN_MATCHn     = nand(WX_MATCH_HI, WX_MATCH0n, WX_MATCH1n, WX_MATCH2n, WX_MATCH3n);
    /*p27.NUKO*/ next.WIN_MATCH     = not (WIN_MATCHn);

    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(prev.FINE_CNT0, prev.FINE_CNT1, prev.FINE_CNT2);
    /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(prev.WIN_MATCH, FINE_COUNT_STOPn);

    /*p27.RYFA*/ next.WIN_MATCH_ONSCREEN_SYNC1.tock(prev.SEGU_4M,            prev.RENDERING, WIN_MATCH_ONSCREEN);
    /*p27.RENE*/ next.WIN_MATCH_ONSCREEN_SYNC2.tock(clocks.ALET_AxCxExGx, prev.RENDERING, prev.WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.SEKO*/ next.WIN_TRIGGER = nor(prev.WIN_MATCH_ONSCREEN_SYNC2, !prev.WIN_MATCH_ONSCREEN_SYNC1);

    /*p27.ROCO*/ next.ROCO_4M = not(prev.SEGU_4M);
    /*p27.PYCO*/ next.WIN_MATCH_SYNC1.tock(prev.ROCO_4M,            resets.VID_RESETn, prev.WIN_MATCH);
    /*p27.NUNU*/ next.WIN_MATCH_SYNC2.tock(clocks.MEHE_xBxDxFxH, resets.VID_RESETn, prev.WIN_MATCH_SYNC1);
  }

  //----------
  // Fine x matcher for x scroll

  {


    /*p27.SUHA*/ wire FINE_MATCH0n = xor(regs.SCX0, prev.FINE_CNT0);
    /*p27.SYBY*/ wire FINE_MATCH1n = xor(regs.SCX1, prev.FINE_CNT1);
    /*p27.SOZU*/ wire FINE_MATCH2n = xor(regs.SCX2, prev.FINE_CNT2);

    /*p27.PAHA*/ wire PAHA = not(prev.RENDERING);
    /*p27.ROXY*/ wire STOP_THE_PIPE = or(PAHA, prev.FINE_MATCH_TRIG);
    /*p27.RONE*/ wire FINE_MATCH  = nand(STOP_THE_PIPE, FINE_MATCH0n, FINE_MATCH1n, FINE_MATCH2n);
    /*p27.POHU*/ wire FINE_MATCHn = not(FINE_MATCH);

    /*p27.PUXA*/ next.FINE_MATCH_SYNC1.tock(prev.ROXO_4M, prev.RENDERING, FINE_MATCHn);

    /*p27.NYZE*/ next.FINE_MATCH_SYNC2.tock(clocks.MOXE_xBxDxFxH, prev.RENDERING, prev.FINE_MATCH_SYNC1);

    /*p27.POVA*/ next.FINE_MATCH_TRIG  = and(prev.FINE_MATCH_SYNC1, !prev.FINE_MATCH_SYNC2);
  }

  {
    /*p25.ROPY*/ wire RENDERINGo = not(prev.RENDERING);
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(prev.FINE_CNT0, prev.FINE_CNT1, prev.FINE_CNT2);
    /*p27.PASO*/ wire FINE_RST = nor(prev.MAP_X_CLK_STOPn, RENDERINGo);

    /*p27.PECU*/ wire FINE_CLK = nand(prev.ROXO_4M, FINE_COUNT_STOPn);
    /*p27.RYKU*/ next.FINE_CNT0.tock( FINE_CLK,       FINE_RST, !prev.FINE_CNT0);
    /*p27.ROGA*/ next.FINE_CNT1.tock(!prev.FINE_CNT0, FINE_RST, !prev.FINE_CNT1);
    /*p27.RUBU*/ next.FINE_CNT2.tock(!prev.FINE_CNT1, FINE_RST, !prev.FINE_CNT2);
  }


  //----------
  // BG/WIN sequencer

  {
    /*p27.NYFO*/ wire WIN_RST_TRIGn = not(prev.WIN_RST_TRIG);
    /*p27.MOSU*/ wire WIN_RST_TRIGb = not(WIN_RST_TRIGn);

    // counts to 5? polarity?
    /*p27.MOCE*/ wire BG_SEQ_5n = nand(prev.BG_SEQ_x1x3x5x7, prev.BG_SEQ_xxxx4567, prev.BG_SEQ_RSTn);
    /*p27.LYRY*/ next.BG_SEQ_5 = not(BG_SEQ_5n);
    /*p27.NYXU*/ next.BG_SEQ_RSTn = nor(in.SCAN_DONE_TRIG, WIN_RST_TRIGb, prev.MAP_X_CLK_STOPn);

    /*p27.LEBO*/ wire BG_SEQ_CLK = nand(clocks.ALET_AxCxExGx, BG_SEQ_5n);
    /*p27.LAXU*/ next.BG_SEQ_x1x3x5x7.tock( BG_SEQ_CLK,           prev.BG_SEQ_RSTn, !prev.BG_SEQ_x1x3x5x7);
    /*p27.MESU*/ next.BG_SEQ_xx23xx67.tock(!prev.BG_SEQ_x1x3x5x7, prev.BG_SEQ_RSTn, !prev.BG_SEQ_xx23xx67);
    /*p27.NYVA*/ next.BG_SEQ_xxxx4567.tock(!prev.BG_SEQ_xx23xx67, prev.BG_SEQ_RSTn, !prev.BG_SEQ_xxxx4567);

    /*p27.NAKO*/ wire BG_SEQ_01xx45xx  = not(prev.BG_SEQ_xx23xx67);
    /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(prev.BG_SEQ_xxxx4567);
    /*p27.NOGU*/ next.BG_SEQ_xx234567 = nand(BG_SEQ_01xx45xx, BG_SEQ_0123xxxx);

    /*p27.LOVY*/ next.BG_SEQ5_SYNC.tock(clocks.MYVO_xBxDxFxH, prev.BG_SEQ_RSTn, prev.BG_SEQ_5);

    /*p27.LURY*/ wire LURY = and(!prev.BG_SEQ5_SYNC, prev.RENDERING);
    /*p27.LONY*/ wire LONY = and(LURY, prev.BG_SEQ_RSTn);
    /*p27.LUSU*/ wire LONYn = not(LONY);
    /*p27.LENA*/ next.LONYb = not(LONYn);

    /*p24.NAFY*/ wire RENDERING_AND_NOT_WIN_TRIG = nor(WIN_RST_TRIGb, prev.RENDERINGn);
    /*p24.NYKA*/ next.BG_SEQ_6.tock(clocks.ALET_AxCxExGx, RENDERING_AND_NOT_WIN_TRIG, prev.BG_SEQ_5);
    /*p24.PORY*/ next.BG_SEQ_7.tock(clocks.MYVO_xBxDxFxH, RENDERING_AND_NOT_WIN_TRIG, prev.BG_SEQ_6);
    /*p24.PYGO*/ next.BG_SEQ_8.tock(clocks.ALET_AxCxExGx, prev.RENDERING,             prev.BG_SEQ_7);
    /*p27.LYZU*/ next.BG_SEQ_x1x3x5x7_DELAY.tock(clocks.ALET_AxCxExGx, prev.RENDERING, prev.BG_SEQ_x1x3x5x7);
  }



  //----------

  {

    /*p27.XAHY*/ wire NEW_LINEn = not(lcd.NEW_LINE1);
    /*p27.XOFO*/ wire MAP_X_RST = nand(regs.LCDC_WINEN, NEW_LINEn, resets.VID_RESETn);
    /*p27.XACO*/ wire MAP_X_RSTn = not(MAP_X_RST);


    /*p27.NOCU*/ wire NOCU = not(next.WIN_RST);
    /*p27.PORE*/ wire PORE = not(NOCU);
    /*p27.WAZY*/ wire WAZY= not(PORE);
    /*p27.PORE*/ wire WIN_RSTb = not(WAZY);
    /*p27.VETU*/ wire MAP_X_CLK = and(WIN_RSTb, prev.MAP_X_CLK_STOPn);
    /*p27.WYKA*/ next.MAP_X0.tock(MAP_X_CLK,    MAP_X_RSTn, !prev.MAP_X0);
    /*p27.WODY*/ next.MAP_X1.tock(!prev.MAP_X0, MAP_X_RSTn, !prev.MAP_X1);
    /*p27.WOBO*/ next.MAP_X2.tock(!prev.MAP_X1, MAP_X_RSTn, !prev.MAP_X2);
    /*p27.WYKO*/ next.MAP_X3.tock(!prev.MAP_X2, MAP_X_RSTn, !prev.MAP_X3);
    /*p27.XOLO*/ next.MAP_X4.tock(!prev.MAP_X3, MAP_X_RSTn, !prev.MAP_X4);

    /*p21.PARU*/ wire VBLANK = not(!lcd.REG_VBLANK);
    /*p27.REPU*/ wire Y_RST  = nor(VBLANK, VID_RESET4);   // schematic wrong, this is NOR
    /*p27.SYNY*/ wire Y_RSTn = not(Y_RST);

    /*p27.VYNO*/ next.TILE_Y0.tock( WAZY,         Y_RSTn, !prev.TILE_Y0);
    /*p27.VUJO*/ next.TILE_Y1.tock(!prev.TILE_Y0, Y_RSTn, !prev.TILE_Y1);
    /*p27.VYMU*/ next.TILE_Y2.tock(!prev.TILE_Y1, Y_RSTn, !prev.TILE_Y2);
    /*p27.TUFU*/ next.MAP_Y0.tock(!prev.TILE_Y2,  Y_RSTn, !prev.MAP_Y0);
    /*p27.TAXA*/ next.MAP_Y1.tock(!prev.MAP_Y0,   Y_RSTn, !prev.MAP_Y1);
    /*p27.TOZO*/ next.MAP_Y2.tock(!prev.MAP_Y1,   Y_RSTn, !prev.MAP_Y2);
    /*p27.TATE*/ next.MAP_Y3.tock(!prev.MAP_Y2,   Y_RSTn, !prev.MAP_Y3);
    /*p27.TEKE*/ next.MAP_Y4.tock(!prev.MAP_Y3,   Y_RSTn, !prev.MAP_Y4);
  }

  //----------

}

//-----------------------------------------------------------------------------

};