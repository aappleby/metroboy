#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P21_VideoControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p27.MOXE*/ c.p21.CLK_xBxDxFxHa = not(b.sys.CLK_AxCxExGx4);
  /*p27.MEHE*/ c.p21.CLK_xBxDxFxHb = not(b.sys.CLK_AxCxExGx4);
  /*p27.MYVO*/ c.p21.CLK_xBxDxFxHc = not(b.sys.CLK_AxCxExGx4);
  /*p29.LAPE*/ c.p21.CLK_xBxDxFxHd = not(b.sys.CLK_AxCxExGx4);
  /*p29.XOTA*/ c.p21.CLK_xBxDxFxHe = not(b.p21.CLK_AxCxExGxb);

  /*p27.TAVA*/ c.p21.CLK_AxCxExGxa = not(b.sys.CLK_xBxDxFxH5);
  /*p29.XYVA*/ c.p21.CLK_AxCxExGxb = not(b.sys.CLK_xBxDxFxH2);
  /*p29.XYFY*/ c.p21.CLK_AxCxExGxc = not(b.p21.CLK_xBxDxFxHe);

  /*p29.WUVU*/ c.p21.CLK_2Ma = tock_pos(a.p21.CLK_xBxDxFxHe, b.p21.CLK_xBxDxFxHe, b.sys.VID_RESETn1, b.p21.CLK_2Ma, !b.p21.CLK_2Ma);
  /*p29.XUPY*/ c.p21.CLK_2Mb = not(!b.p21.CLK_2Ma);

  /*p21.VENA*/ c.p21.CLK_1Ma = tock_pos(!a.p21.CLK_2Ma, !b.p21.CLK_2Ma, b.sys.VID_RESETn1, b.p21.CLK_1Ma, !b.p21.CLK_1Ma);
  /*p21.TALU*/ c.p21.CLK_1Mb = not(!b.p21.CLK_1Ma);

  //----------
  // x counter. this is a little weird, presumably because it can tick at 4 mhz but not always?

  /*p21.TADY*/ c.p21.TADY = nor(b.spr.NEW_LINE, b.sys.VID_RESET3);

  /*p21.RYBO*/ c.p21.RYBO = xor(b.p21.X0, b.p21.X1);
  /*p21.XUKE*/ c.p21.XUKE = and(b.p21.X0, b.p21.X1);

  /*p21.XYLE*/ c.p21.XYLE = and(b.p21.X2, b.p21.XUKE);
  /*p21.XEGY*/ c.p21.XEGY = xor(b.p21.X2, b.p21.XUKE);

  /*p21.XORA*/ c.p21.XORA = xor(b.p21.X3, b.p21.XYLE);

  /*p21.XEHO*/ c.p21.X0 = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.X0, !b.p21.X0);
  /*p21.SAVY*/ c.p21.X1 = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.X1, b.p21.RYBO);
  /*p21.XODU*/ c.p21.X2 = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.X2, b.p21.XEGY);
  /*p21.XYDO*/ c.p21.X3 = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.X3, b.p21.XORA);

  /*p21.TOCA*/ c.p21.TOCA = not(b.p21.X3);
  /*p21.SAKE*/ c.p21.SAKE = xor(b.p21.X4, b.p21.X5);
  /*p21.TYBA*/ c.p21.TYBA = and(b.p21.X4, b.p21.X5);

  /*p21.SURY*/ c.p21.SURY = and(b.p21.X6, b.p21.TYBA);
  /*p21.TYGE*/ c.p21.TYGE = xor(b.p21.X6, b.p21.TYBA);

  /*p21.ROKU*/ c.p21.ROKU = xor(b.p21.X7, b.p21.SURY);

  /*p21.TUHU*/ c.p21.X4 = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.X4, !b.p21.X4);
  /*p21.TUKY*/ c.p21.X5 = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.X5, b.p21.SAKE);
  /*p21.TAKO*/ c.p21.X6 = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.X6, b.p21.TYGE);
  /*p21.SYBE*/ c.p21.X7 = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.X7, b.p21.ROKU);


  /*p21.ACAM*/ c.p21.X0n = not(b.p21.X0); // polarity?
  /*p21.AZUB*/ c.p21.X1n = not(b.p21.X1);
  /*p21.AMEL*/ c.p21.X2n = not(b.p21.X2);
  /*p21.AHAL*/ c.p21.X3n = not(b.p21.X3);
  /*p21.APUX*/ c.p21.X4n = not(b.p21.X4);
  /*p21.ABEF*/ c.p21.X5n = not(b.p21.X5);
  /*p21.ADAZ*/ c.p21.X6n = not(b.p21.X6);
  /*p21.ASAH*/ c.p21.X7n = not(b.p21.X7);


  //----------
  // LY compare

  /*p21.SYFU*/ c.p21.LY_MATCH7 = xor(b.p21.V7, b.p23.LYC7);
  /*p21.TERY*/ c.p21.LY_MATCH6 = xor(b.p21.V6, b.p23.LYC6);
  /*p21.TUCY*/ c.p21.LY_MATCH5 = xor(b.p21.V5, b.p23.LYC5);
  /*p21.TYKU*/ c.p21.LY_MATCH4 = xor(b.p21.V4, b.p23.LYC4);
  /*p21.RASY*/ c.p21.LY_MATCH3 = xor(b.p21.V3, b.p23.LYC3);
  /*p21.REDA*/ c.p21.LY_MATCH2 = xor(b.p21.V2, b.p23.LYC2);
  /*p21.TYDE*/ c.p21.LY_MATCH1 = xor(b.p21.V1, b.p23.LYC1);
  /*p21.RYME*/ c.p21.LY_MATCH0 = xor(b.p21.V0, b.p23.LYC0);

  /*p21.SOVU*/ c.p21.LY_MATCHA = nor(b.p21.LY_MATCH7, b.p21.LY_MATCH6, b.p21.LY_MATCH5, b.p21.LY_MATCH4);
  /*p21.SUBO*/ c.p21.LY_MATCHB = nor(b.p21.LY_MATCH3, b.p21.LY_MATCH2, b.p21.LY_MATCH1, b.p21.LY_MATCH0);

  /*p21.RAPE*/ c.p21.LY_MATCHn = nand(b.p21.LY_MATCHA, b.p21.LY_MATCHB);
  /*p21.PALY*/ c.p21.LY_MATCH = not(b.p21.LY_MATCHn);
  /*p21.ROPO*/ c.p21.INT_LYC = tock_pos(a.p21.CLK_1Mb, b.p21.CLK_1Mb, b.sys.SYS_RESETn6, b.p21.INT_LYC, b.p21.LY_MATCH);

  //----------
  // line sequencer?

  /*p21.MUDE*/ c.p21.CNT_RSTn = nor(b.p21.LINE_DONEn, b.sys.VID_RESET2);

  /*p21.SAXO*/ c.p21.CNT_0 = tock_pos( a.p21.CLK_1Mb, b.p21.CLK_1Mb, b.p21.CNT_RSTn, c.p21.CNT_0, !c.p21.CNT_0);
  /*p21.TYPO*/ c.p21.CNT_1 = tock_pos(!a.p21.CNT_0,   !b.p21.CNT_0,  b.p21.CNT_RSTn, c.p21.CNT_1, !c.p21.CNT_1);
  /*p21.VYZO*/ c.p21.CNT_2 = tock_pos(!a.p21.CNT_1,   !b.p21.CNT_1,  b.p21.CNT_RSTn, c.p21.CNT_2, !c.p21.CNT_2);
  /*p21.TELU*/ c.p21.CNT_3 = tock_pos(!a.p21.CNT_2,   !b.p21.CNT_2,  b.p21.CNT_RSTn, c.p21.CNT_3, !c.p21.CNT_3);
  /*p21.SUDE*/ c.p21.CNT_4 = tock_pos(!a.p21.CNT_3,   !b.p21.CNT_3,  b.p21.CNT_RSTn, c.p21.CNT_4, !c.p21.CNT_4);
  /*p21.TAHA*/ c.p21.CNT_5 = tock_pos(!a.p21.CNT_4,   !b.p21.CNT_4,  b.p21.CNT_RSTn, c.p21.CNT_5, !c.p21.CNT_5);
  /*p21.TYRY*/ c.p21.CNT_6 = tock_pos(!a.p21.CNT_5,   !b.p21.CNT_5,  b.p21.CNT_RSTn, c.p21.CNT_6, !c.p21.CNT_6);

  /*p21.TOCU*/ c.p21.CNT_0n = not(b.p21.CNT_0);
  /*p21.VEPE*/ c.p21.CNT_1n = not(b.p21.CNT_1);
  /*p21.VUTY*/ c.p21.CNT_2n = not(b.p21.CNT_2);
  /*p21.VATE*/ c.p21.CNT_3n = not(b.p21.CNT_3);
  /*p21.TUDA*/ c.p21.CNT_4n = not(b.p21.CNT_4);
  /*p21.TAFY*/ c.p21.CNT_5n = not(b.p21.CNT_5);
  /*p21.TUJU*/ c.p21.CNT_6n = not(b.p21.CNT_6);






  /*p21.SANU*/ c.p21.CNT_113n = nand(b.p21.CNT_6, b.p21.CNT_5, b.p21.CNT_4, b.p21.CNT_0);

  /*p21.RUTU*/ c.p21.LINE_DONEn = tock_pos(a.p21.SONO_CLK, b.p21.SONO_CLK, b.sys.VID_RESETn2, b.p21.LINE_DONEn, b.p21.CNT_113n);
  /*p21.SONO*/   c.p21.SONO_CLK   = not(b.p21.CLK_1Mb);



  //----------
  // FF41 STAT

  /*p21.NYPE*/ c.p21.LINE_DONE_DELAYn = tock_pos(a.p21.CLK_1Mb, b.p21.CLK_1Mb, b.sys.VID_RESETn2, b.p21.LINE_DONE_DELAYn, b.p21.LINE_DONEn);

  /*p21.XYVO*/ c.p21.LINE_144 = and(b.p21.V4, b.p21.V7);
  /*p21.POPU*/ c.p21.LINE_144_SYNC = tock_pos(a.p21.LINE_DONE_DELAYn, b.p21.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.p21.LINE_144_SYNC, b.p21.LINE_144);



  /*p21.WODU*/ c.p21.RENDER_DONEn = and(b.p21.OAM_SCANn, b.p21.X_167);
  /*p21.XENA*/   c.p21.OAM_SCANn = not(b.spr.OAM_SCAN);
  /*p21.XANO*/   c.p21.X_167 = not(b.p21.X_167n);
  /*p21.XUGU*/     c.p21.X_167n = nand(b.p21.X0, b.p21.X1, b.p21.X2, b.p21.X5, b.p21.X7); // 128 + 32 + 4 + 2 + 1 = 167


  /*p21.PARU*/ c.p21.INT_VBL     = not(!b.p21.LINE_144_SYNC);
  /*p21.TOLU*/ c.p21.INT_VBLn    = not(b.p21.INT_VBL);
  /*p21.VYPU*/ c.p21.INT_VBL_BUF = not(b.p21.INT_VBLn);


  /*p21.TAPA*/ c.p21.INT_OAM = and(b.p21.INT_VBLn, b.p21.LINE_DONEo);
  /*p21.SELA*/   c.p21.LINE_DONEo    = not(b.p21.LINE_DONEa);
  /*p21.PURE*/     c.p21.LINE_DONEa    = not(b.p21.LINE_DONEn);

  /*p21.TARU*/ c.p21.INT_HBL = and(b.p21.INT_VBLn, b.p21.RENDER_DONEn);

  //---

  /*p21.XYMU*/ c.p21.RENDERING = or(b.p21.WEGO, b.spr.SCAN_DONE_TRIG);
  /*p21.WEGO*/   c.p21.WEGO = or(b.sys.VID_RESET3, b.p21.VOGA);
  /*p21.VOGA*/     c.p21.VOGA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p21.TADY, b.p21.VOGA, b.p21.RENDER_DONEn);

  /*p21.XAJO*/ c.p21.X_009 = and(b.p21.X0, b.p21.X3);


  //---




  /*p21.SUKO*/ c.p21.INT_STATb = amux4(b.p21.INT_LYC_EN, b.p21.INT_LYC,
                                       b.p21.INT_OAM_EN, b.p21.INT_OAM,
                                       b.p21.INT_VBL_EN, b.p21.INT_VBL,
                                       b.p21.INT_HBL_EN, b.p21.INT_HBL);

  /*p21.TUVA*/ c.p21.INT_STATn = not(b.p21.INT_STATb);
  /*p21.VOTY*/ c.p21.INT_STAT  = not(b.p21.INT_STATn);


  //----------
  // FF41 STAT stuff

  /*p21.TOBE*/ c.p21.FF41_RDa = and(b.sys.CPU_RD2, b.p22.FF41);
  /*p21.VAVE*/ c.p21.FF41_RDb = b.p21.FF41_RDa; // buffer, not inverter

                                                // 00: hblank   - rendering 0, vbl 0, oam 0
                                                // 01: vblank   - rendering 0, vbl 1, oam 0
                                                // 10: oam scan - rendering 0, vbl 0, oam 1
                                                // 11: render   - rendering 1, vbl 0, oam 0

                                                // so one of these has the wrong polarity



  /*p21.SADU*/ c.p21.STAT_MODE0n = nor(b.p21.RENDERING, b.p21.INT_VBL);
  /*p21.XATY*/ c.p21.STAT_MODE1n = nor(b.spr.OAM_ADDR_PARSEn, b.p21.RENDERING);

  /*p21.SEPA*/ c.p21.FF41_WR  = and(b.sys.CPU_WR2, b.p22.FF41);
  /*p21.RYJU*/ c.p21.FF41_WRn = not(b.p21.FF41_WR);
  /*p21.RYVE*/ c.p21.FF41_WRo = not(b.p21.FF41_WR);

  /*p21.ROXE*/ c.p21.INT_HBL_EN = tock_pos(a.p21.FF41_WRo, b.p21.FF41_WRo, b.sys.SYS_RESETn6, b.p21.INT_HBL_EN, b.D3);
  /*p21.RUFO*/ c.p21.INT_VBL_EN = tock_pos(a.p21.FF41_WRo, b.p21.FF41_WRo, b.sys.SYS_RESETn6, b.p21.INT_VBL_EN, b.D4);
  /*p21.REFE*/ c.p21.INT_OAM_EN = tock_pos(a.p21.FF41_WRo, b.p21.FF41_WRo, b.sys.SYS_RESETn6, b.p21.INT_OAM_EN, b.D5);
  /*p21.RUGU*/ c.p21.INT_LYC_EN = tock_pos(a.p21.FF41_WRo, b.p21.FF41_WRo, b.sys.SYS_RESETn6, b.p21.INT_LYC_EN, b.D6);

  // weird
  /*p21.PAGO*/ c.p21.STAT_LYC_MATCH1 = nor(b.sys.SYS_RESETn6, b.p21.FF41_WRn);  // schematic wrong, this is NOR
  /*p21.RUPO*/ c.p21.STAT_LYC_MATCH2 = or(b.p21.INT_LYC, b.p21.STAT_LYC_MATCH1); // this is another of the weird or gates. could be nor?

  /*p21.TEBY*/ if (b.p21.FF41_RDa) c.D0 = not(b.p21.STAT_MODE0n);
  /*p21.WUGA*/ if (b.p21.FF41_RDa) c.D1 = not(b.p21.STAT_MODE1n);
  /*p21.SEGO*/ if (b.p21.FF41_RDa) c.D2 = not(b.p21.STAT_LYC_MATCH2);
  /*p21.PUZO*/ if (b.p21.FF41_RDb) c.D3 = b.p21.INT_HBL_EN;
  /*p21.POFO*/ if (b.p21.FF41_RDb) c.D4 = b.p21.INT_VBL_EN;
  /*p21.SASY*/ if (b.p21.FF41_RDb) c.D5 = b.p21.INT_OAM_EN;
  /*p21.POTE*/ if (b.p21.FF41_RDb) c.D6 = b.p21.INT_LYC_EN;

  //----------
  // y counter

  // 128+16+1+2
  /*p21.NOKO*/ c.p21.LINE_147 = and(b.p21.V7, b.p21.V4, b.p21.V0, b.p21.V1);
  /*p21.MYTA*/ c.p21.LINE_147_SYNC = tock_pos(a.p21.LINE_DONE_DELAYn, b.p21.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.p21.LINE_147_SYNC, b.p21.LINE_147);
  /*p21.LAMA*/ c.p21.Y_RSTn = nor(b.p21.LINE_147_SYNC, b.sys.VID_RESET2);

  /*p21.MUWY*/ c.p21.V0 = tock_pos(a.p21.LINE_DONEn, b.p21.LINE_DONEn, b.p21.Y_RSTn, c.p21.V0, !c.p21.V0);
  /*p21.MYRO*/ c.p21.V1 = tock_pos(!a.p21.V0,        !b.p21.V0,        b.p21.Y_RSTn, c.p21.V1, !c.p21.V1);
  /*p21.LEXA*/ c.p21.V2 = tock_pos(!a.p21.V1,        !b.p21.V1,        b.p21.Y_RSTn, c.p21.V2, !c.p21.V2);
  /*p21.LYDO*/ c.p21.V3 = tock_pos(!a.p21.V2,        !b.p21.V2,        b.p21.Y_RSTn, c.p21.V3, !c.p21.V3);
  /*p21.LOVU*/ c.p21.V4 = tock_pos(!a.p21.V3,        !b.p21.V3,        b.p21.Y_RSTn, c.p21.V4, !c.p21.V4);
  /*p21.LEMA*/ c.p21.V5 = tock_pos(!a.p21.V4,        !b.p21.V4,        b.p21.Y_RSTn, c.p21.V5, !c.p21.V5);
  /*p21.MATO*/ c.p21.V6 = tock_pos(!a.p21.V5,        !b.p21.V5,        b.p21.Y_RSTn, c.p21.V6, !c.p21.V6);
  /*p21.LAFO*/ c.p21.V7 = tock_pos(!a.p21.V6,        !b.p21.V6,        b.p21.Y_RSTn, c.p21.V7, !c.p21.V7);
}

//-----------------------------------------------------------------------------

void P22_PpuDecode_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {
  /*p22.XALY*/ c.p22.ADDR_0x00xxxx = nor(b.A07, b.A05, b.A04);
  /*p22.WUTU*/ c.p22.FF4Xn         = nand(b.sys.ADDR_FFXX, b.A06, b.p22.ADDR_0x00xxxx);
  /*p22.WERO*/ c.p22.FF4X          = not(b.p22.FF4Xn);

  /*p22.XOLA*/ c.p22.A00n = not(b.A00);
  /*p22.XENO*/ c.p22.A01n = not(b.A01);
  /*p22.XUSY*/ c.p22.A02n = not(b.A02);
  /*p22.XERA*/ c.p22.A03n = not(b.A03);

  /*p22.WADO*/ c.p22.A00 = not(b.p22.A00n);
  /*p22.WESA*/ c.p22.A01 = not(b.p22.A01n);
  /*p22.WALO*/ c.p22.A02 = not(b.p22.A02n);
  /*p22.WEPO*/ c.p22.A03 = not(b.p22.A03n);

  /*p22.WORU*/ c.p22.FF40n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01n, b.p22.A02n, b.p22.A03n);
  /*p22.WOFA*/ c.p22.FF41n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01n, b.p22.A02n, b.p22.A03n);
  /*p22.WEBU*/ c.p22.FF42n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01,  b.p22.A02n, b.p22.A03n);
  /*p22.WAVU*/ c.p22.FF43n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01,  b.p22.A02n, b.p22.A03n);
  /*p22.WYLE*/ c.p22.FF44n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01n, b.p22.A02,  b.p22.A03n);
  /*p22.WETY*/ c.p22.FF45n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01n, b.p22.A02,  b.p22.A03n);
  /*p22.WATE*/ c.p22.FF46n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01,  b.p22.A02,  b.p22.A03n);
  /*p22.WYBO*/ c.p22.FF47n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01,  b.p22.A02,  b.p22.A03n);
  /*p22.WETA*/ c.p22.FF48n = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01n, b.p22.A02n, b.p22.A03);
  /*p22.VAMA*/ c.p22.FF49n = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01n, b.p22.A02n, b.p22.A03);
  /*p22.WYVO*/ c.p22.FF4An = nand(b.p22.FF4X, b.p22.A00n, b.p22.A01,  b.p22.A02n, b.p22.A03);
  /*p22.WAGE*/ c.p22.FF4Bn = nand(b.p22.FF4X, b.p22.A00,  b.p22.A01,  b.p22.A02n, b.p22.A03);

  /*p22.VOCA*/ c.p22.FF40 = not(b.p22.FF40n);
  /*p22.VARY*/ c.p22.FF41 = not(b.p22.FF41n);
  /*p22.XARO*/ c.p22.FF42 = not(b.p22.FF42n);
  /*p22.XAVY*/ c.p22.FF43 = not(b.p22.FF43n);
  /*p22.XOGY*/ c.p22.FF44 = not(b.p22.FF44n);
  /*p22.XAYU*/ c.p22.FF45 = not(b.p22.FF45n);
  /*p22.XEDA*/ c.p22.FF46 = not(b.p22.FF46n);
  /*p22.WERA*/ c.p22.FF47 = not(b.p22.FF47n);
  /*p22.XAYO*/ c.p22.FF48 = not(b.p22.FF48n);
  /*p22.TEGO*/ c.p22.FF49 = not(b.p22.FF49n);
  /*p22.VYGA*/ c.p22.FF4A = not(b.p22.FF4An);
  /*p22.VUMY*/ c.p22.FF4B = not(b.p22.FF4Bn);
}

//-----------------------------------------------------------------------------

void P23_VideoRegs_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  // FF40 LCDC
  /*p23.VYRE*/ c.p23.FF40_RD = and(b.p22.FF40, b.sys.CPU_RD2);
  /*p23.WARU*/ c.p23.FF40_WR = and(b.p22.FF40, b.sys.CPU_WR2);
  /*p23.WYCE*/ c.p23.FF40_RDn = not(b.p23.FF40_RD);
  /*p23.XUBO*/ c.p23.FF40_WRn = not(b.p23.FF40_WR);

  /*p23.VYXE*/ c.p23.LCDC_BGEN    = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.LCDC_BGEN,   b.D0);
  /*p23.XYLO*/ c.p23.LCDC_SPEN    = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.LCDC_SPEN,   b.D1);
  /*p23.XYMO*/ c.p23.LCDC_SPSIZE  = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.LCDC_SPSIZE, b.D2);
  /*p23.XAFO*/ c.p23.LCDC_BGMAP   = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.LCDC_BGMAP,  b.D3);
  /*p23.WEXU*/ c.p23.BG_TILE_SEL  = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.BG_TILE_SEL, b.D4);
  /*p23.WYMO*/ c.p23.LCDC_WINEN   = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.LCDC_WINEN,  b.D5);
  /*p23.WOKY*/ c.p23.WIN_MAP_SEL  = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.WIN_MAP_SEL, b.D6);
  /*p23.XONA*/ c.p23.LCDC_EN      = tock_pos(a.p23.FF40_WRn, b.p23.FF40_WRn, b.sys.SYS_RESETn7, b.p23.LCDC_EN,     b.D7);

  /*p23.WYPO*/ if (b.p23.FF40_RDn) c.D0 = b.p23.LCDC_BGEN;
  /*p23.XERO*/ if (b.p23.FF40_RDn) c.D1 = b.p23.LCDC_SPEN;
  /*p23.WYJU*/ if (b.p23.FF40_RDn) c.D2 = b.p23.LCDC_SPSIZE;
  /*p23.WUKA*/ if (b.p23.FF40_RDn) c.D3 = b.p23.LCDC_BGMAP;
  /*p23.VOKE*/ if (b.p23.FF40_RDn) c.D4 = b.p23.BG_TILE_SEL;
  /*p23.VATO*/ if (b.p23.FF40_RDn) c.D5 = b.p23.LCDC_WINEN;
  /*p23.VAHA*/ if (b.p23.FF40_RDn) c.D6 = b.p23.WIN_MAP_SEL;
  /*p23.XEBU*/ if (b.p23.FF40_RDn) c.D7 = b.p23.LCDC_EN;

  // FF4B WX
  /*p23.WYZE*/ c.p23.WYZE = and(b.sys.CPU_RD2, b.p22.FF4B);
  /*p23.WUZA*/ c.p23.WUZA = and(b.sys.CPU_WR2, b.p22.FF4B);
  /*p23.VYCU*/ c.p23.VYCU = not(b.p23.WYZE);
  /*p23.VOXU*/ c.p23.VOXU = not(b.p23.WUZA);

  /*p23.MYPA*/ c.p23.WX0 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX0, b.D0);
  /*p23.NOFE*/ c.p23.WX1 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX1, b.D1);
  /*p23.NOKE*/ c.p23.WX2 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX2, b.D2);
  /*p23.MEBY*/ c.p23.WX3 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX3, b.D3);
  /*p23.MYPU*/ c.p23.WX4 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX4, b.D4);
  /*p23.MYCE*/ c.p23.WX5 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX5, b.D5); 
  /*p23.MUVO*/ c.p23.WX6 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX6, b.D6);
  /*p23.NUKU*/ c.p23.WX7 = tock_pos(a.p23.VOXU, b.p23.VOXU, b.sys.SYS_RESETn5, a.p23.WX7, b.D7);

  /*p23.LOVA*/ if (b.p23.VYCU) c.D0 = b.p23.WX0;
  /*p23.MUKA*/ if (b.p23.VYCU) c.D1 = b.p23.WX1;
  /*p23.MOKO*/ if (b.p23.VYCU) c.D2 = b.p23.WX2;
  /*p23.LOLE*/ if (b.p23.VYCU) c.D3 = b.p23.WX3;
  /*p23.MELE*/ if (b.p23.VYCU) c.D4 = b.p23.WX4;
  /*p23.MUFE*/ if (b.p23.VYCU) c.D5 = b.p23.WX5;
  /*p23.MULY*/ if (b.p23.VYCU) c.D6 = b.p23.WX6;
  /*p23.MARA*/ if (b.p23.VYCU) c.D7 = b.p23.WX7;

  // FF4A WY
  /*p23.WAXU*/ c.p23.WAXU = and(b.sys.CPU_RD2, b.p22.FF4A);
  /*p23.WEKO*/ c.p23.WEKO = and(b.sys.CPU_WR2, b.p22.FF4A);
  /*p23.VEFU*/ c.p23.VEFU = not(b.p23.WEKO);
  /*p23.VOMY*/ c.p23.VOMY = not(b.p23.WAXU);

  /*p23.NESO*/ c.p23.WY0 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY0, b.D0);
  /*p23.NYRO*/ c.p23.WY1 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY1, b.D1);
  /*p23.NAGA*/ c.p23.WY2 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY2, b.D2);
  /*p23.MELA*/ c.p23.WY3 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY3, b.D3);
  /*p23.NULO*/ c.p23.WY4 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY4, b.D4);
  /*p23.NENE*/ c.p23.WY5 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY5, b.D5);
  /*p23.NUKA*/ c.p23.WY6 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY6, b.D6);
  /*p23.NAFU*/ c.p23.WY7 = tock_pos(a.p23.VEFU, b.p23.VEFU, b.sys.SYS_RESETn5, b.p23.WY7, b.D7);

  /*p23.PUNU*/ if (b.p23.VOMY) c.D0 = b.p23.WY0;
  /*p23.PODA*/ if (b.p23.VOMY) c.D1 = b.p23.WY1;
  /*p23.PYGU*/ if (b.p23.VOMY) c.D2 = b.p23.WY2;
  /*p23.LOKA*/ if (b.p23.VOMY) c.D3 = b.p23.WY3;
  /*p23.MEGA*/ if (b.p23.VOMY) c.D4 = b.p23.WY4;
  /*p23.PELA*/ if (b.p23.VOMY) c.D5 = b.p23.WY5;
  /*p23.POLO*/ if (b.p23.VOMY) c.D6 = b.p23.WY6;
  /*p23.MERA*/ if (b.p23.VOMY) c.D7 = b.p23.WY7;

  // FF43 SCX
  /*p23.AVOG*/ c.p23.AVOG = and(b.p22.FF43, b.sys.CPU_RD2);
  /*p23.ARUR*/ c.p23.ARUR = and(b.p22.FF43, b.sys.CPU_WR2);
  /*p23.AMUN*/ c.p23.AMUN = not(b.p23.ARUR);
  /*p23.BEBA*/ c.p23.BEBA = not(b.p23.AVOG);

  /*p23.DATY*/ c.p23.SCX0 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX0, b.D0);
  /*p23.DUZU*/ c.p23.SCX1 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX1, b.D1);
  /*p23.CYXU*/ c.p23.SCX2 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX2, b.D2);
  /*p23.GUBO*/ c.p23.SCX3 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX3, b.D3);
  /*p23.BEMY*/ c.p23.SCX4 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX4, b.D4);
  /*p23.CUZY*/ c.p23.SCX5 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX5, b.D5);
  /*p23.CABU*/ c.p23.SCX6 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX6, b.D6);
  /*p23.BAKE*/ c.p23.SCX7 = tock_pos(a.p23.AMUN, b.p23.AMUN, b.sys.SYS_RESETn2, a.p23.SCX7, b.D7);

  /*p23.EDOS*/ if (b.p23.BEBA) c.D0 = b.p23.SCX0;
  /*p23.EKOB*/ if (b.p23.BEBA) c.D1 = b.p23.SCX1;
  /*p23.CUGA*/ if (b.p23.BEBA) c.D2 = b.p23.SCX2;
  /*p23.WONY*/ if (b.p23.BEBA) c.D3 = b.p23.SCX3;
  /*p23.CEDU*/ if (b.p23.BEBA) c.D4 = b.p23.SCX4;
  /*p23.CATA*/ if (b.p23.BEBA) c.D5 = b.p23.SCX5;
  /*p23.DOXE*/ if (b.p23.BEBA) c.D6 = b.p23.SCX6;
  /*p23.CASY*/ if (b.p23.BEBA) c.D7 = b.p23.SCX7;

  // FF42 SCY
  /*p23.ANYP*/ c.p23.ANYP = and(b.sys.CPU_RD2, b.p22.FF42);
  /*p23.BEDY*/ c.p23.BEDY = and(b.sys.CPU_WR2, b.p22.FF42);
  /*p23.CAVO*/ c.p23.CAVO = not(b.p23.BEDY);
  /*p23.BUWY*/ c.p23.BUWY = not(b.p23.ANYP);

  /*p23.GAVE*/ c.p23.SCY0 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY0, b.D0);
  /*p23.FYMO*/ c.p23.SCY1 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY1, b.D1);
  /*p23.FEZU*/ c.p23.SCY2 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY2, b.D2);
  /*p23.FUJO*/ c.p23.SCY3 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY3, b.D3);
  /*p23.DEDE*/ c.p23.SCY4 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY4, b.D4);
  /*p23.FOTY*/ c.p23.SCY5 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY5, b.D5);
  /*p23.FOHA*/ c.p23.SCY6 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY6, b.D6);
  /*p23.FUNY*/ c.p23.SCY7 = tock_pos(a.p23.CAVO, b.p23.CAVO, b.sys.SYS_RESETn2, b.p23.SCY7, b.D7);

  /*p23.WARE*/ if (b.p23.BUWY) c.D0 = b.p23.SCY0;
  /*p23.GOBA*/ if (b.p23.BUWY) c.D1 = b.p23.SCY1;
  /*p23.GONU*/ if (b.p23.BUWY) c.D2 = b.p23.SCY2;
  /*p23.GODO*/ if (b.p23.BUWY) c.D3 = b.p23.SCY3;
  /*p23.CUSA*/ if (b.p23.BUWY) c.D4 = b.p23.SCY4;
  /*p23.GYZO*/ if (b.p23.BUWY) c.D5 = b.p23.SCY5;
  /*p23.GUNE*/ if (b.p23.BUWY) c.D6 = b.p23.SCY6;
  /*p23.GYZA*/ if (b.p23.BUWY) c.D7 = b.p23.SCY7;

  // FF45 LYC
  /*p23.XYLY*/ c.p23.XYLY = and(b.sys.CPU_RD2, b.p22.FF45);
  /*p23.XUFA*/ c.p23.XUFA = and(b.sys.CPU_WR2, b.p22.FF45);
  /*p23.WANE*/ c.p23.WANE = not(b.p23.XUFA);
  /*p23.WEKU*/ c.p23.WEKU = not(b.p23.XYLY);

  /*p23.SYRY*/ c.p23.LYC0 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC0, b.D0);
  /*p23.VUCE*/ c.p23.LYC1 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC1, b.D1);
  /*p23.SEDY*/ c.p23.LYC2 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC2, b.D2);
  /*p23.SALO*/ c.p23.LYC3 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC3, b.D3);
  /*p23.SOTA*/ c.p23.LYC4 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC4, b.D4);
  /*p23.VAFA*/ c.p23.LYC5 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC5, b.D5);
  /*p23.VEVO*/ c.p23.LYC6 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC6, b.D6);
  /*p23.RAHA*/ c.p23.LYC7 = tock_pos(a.p23.WANE, b.p23.WANE, b.sys.SYS_RESETn6, a.p23.LYC7, b.D7);

  /*p23.RETU*/ if (b.p23.WEKU) c.D0 = b.p23.LYC0;
  /*p23.VOJO*/ if (b.p23.WEKU) c.D1 = b.p23.LYC1;
  /*p23.RAZU*/ if (b.p23.WEKU) c.D2 = b.p23.LYC2;
  /*p23.REDY*/ if (b.p23.WEKU) c.D3 = b.p23.LYC3;
  /*p23.RACE*/ if (b.p23.WEKU) c.D4 = b.p23.LYC4;
  /*p23.VAZU*/ if (b.p23.WEKU) c.D5 = b.p23.LYC5;
  /*p23.VAFE*/ if (b.p23.WEKU) c.D6 = b.p23.LYC6;
  /*p23.PUFY*/ if (b.p23.WEKU) c.D7 = b.p23.LYC7;

  // FF44 LY
  /*p23.WAFU*/ c.p23.FF44_RD = and(b.sys.CPU_RD2, b.p22.FF44);
  /*p23.VARO*/ c.p23.FF44_RDn = not(b.p23.FF44_RD);

  /*p23.WURY*/ c.p23.LY0n = not(b.p21.V0);
  /*p23.XEPO*/ c.p23.LY1n = not(b.p21.V1);
  /*p23.MYFA*/ c.p23.LY2n = not(b.p21.V2);
  /*p23.XUHY*/ c.p23.LY3n = not(b.p21.V3);
  /*p23.WATA*/ c.p23.LY4n = not(b.p21.V4);
  /*p23.XAGA*/ c.p23.LY5n = not(b.p21.V5);
  /*p23.XUCE*/ c.p23.LY6n = not(b.p21.V6);
  /*p23.XOWO*/ c.p23.LY7n = not(b.p21.V7);

  /*p23.VEGA*/ if (b.p23.FF44_RDn) c.D0 = not(b.p23.LY0n);
  /*p23.WUVA*/ if (b.p23.FF44_RDn) c.D1 = not(b.p23.LY1n);
  /*p23.LYCO*/ if (b.p23.FF44_RDn) c.D2 = not(b.p23.LY2n);
  /*p23.WOJY*/ if (b.p23.FF44_RDn) c.D3 = not(b.p23.LY3n);
  /*p23.VYNE*/ if (b.p23.FF44_RDn) c.D4 = not(b.p23.LY4n);
  /*p23.WAMA*/ if (b.p23.FF44_RDn) c.D5 = not(b.p23.LY5n);
  /*p23.WAVO*/ if (b.p23.FF44_RDn) c.D6 = not(b.p23.LY6n);
  /*p23.WEZE*/ if (b.p23.FF44_RDn) c.D7 = not(b.p23.LY7n);
}

//-----------------------------------------------------------------------------

void P24_LcdControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // Main PPU clock, needs untangling

  /*p24.LOBY*/ c.p24.RENDERINGn = not(b.p21.RENDERING);

  /*p24.SACU*/ c.p24.CLKPIPE = nor(b.p24.SEGU_4M, b.p27.FINE_MATCH_TRIG_OR_NOT_RENDERING);


  /*p24.SEGU*/ c.p24.SEGU_4M = not(b.p24.TYFA);
  /*p24.TYFA*/   c.p24.TYFA = and(b.p24.SOCY, b.p24.POKY, b.p24.VYBO);
  /*p24.SOCY*/     c.p24.SOCY = not(b.p24.TOMU);
  /*p24.TOMU*/       c.p24.TOMU = not(b.p27.SYLO);
  /*p24.VYBO*/     c.p24.VYBO = nor(b.spr.OAM_SCAN, b.p21.RENDER_DONEn, b.p21.CLK_xBxDxFxHc);



  /*p24.POKY*/ c.p24.POKY = or(b.p24.PYGO, b.p24.RENDERINGn);
  /*p24.PYGO*/   c.p24.PYGO = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p21.RENDERING, b.p24.PYGO, b.p24.PORY);
  /*p24.PORY*/     c.p24.PORY = tock_pos(a.p21.CLK_xBxDxFxHc, b.p21.CLK_xBxDxFxHc, b.p24.NAFY, b.p24.PORY, b.p24.NYKA);
  /*p24.NAFY*/       c.p24.NAFY = nor(b.p27.MOSU, b.p24.RENDERINGn);
  /*p24.NYKA*/       c.p24.NYKA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p24.NAFY, b.p24.NYKA, b.p27.BG_SEQ_5);



  /*p27.ROXY*/ c.p27.FINE_MATCH_TRIG_OR_NOT_RENDERING = or(b.p27.RENDERINGo, b.p27.FINE_MATCH_TRIG);
  /*p27.PAHA*/   c.p27.RENDERINGo = not(b.p21.RENDERING);

  /*p24.ROXO*/ c.p24.FINE_MATCH_CLK   = not(b.p24.SEGU_4M);
  /*p27.PUXA*/ c.p27.FINE_MATCH_SYNC1 = tock_pos(a.p24.FINE_MATCH_CLK, b.p24.FINE_MATCH_CLK, b.p21.RENDERING,  b.p27.FINE_MATCH_SYNC1, b.p27.FINE_MATCHn);
  /*p27.NYZE*/ c.p27.FINE_MATCH_SYNC2 = tock_pos(a.p21.CLK_xBxDxFxHa,  b.p21.CLK_xBxDxFxHa,  b.p21.RENDERING,  b.p27.FINE_MATCH_SYNC2, b.p27.FINE_MATCH_SYNC1);
  /*p27.POVA*/ c.p27.FINE_MATCH_TRIG  = and(b.p27.FINE_MATCH_SYNC1, !b.p27.FINE_MATCH_SYNC2);



  //----------
  // Control

  /*p21.POGU*/ c.chip.CPG = not(b.p21.RYNO);
  /*p21.RYNO*/   c.p21.RYNO = or(b.p21.SYGU, b.p21.LINE_DONEn);
  /*p21.SYGU*/     c.p21.SYGU = tock_pos(a.p21.SONO_CLK, b.p21.SONO_CLK, b.sys.VID_RESETn2, b.p21.SYGU, b.p21.TEGY);
  /*p21.TEGY*/       c.p21.TEGY = nand(b.p21.CNT_000n, b.p21.CNT_007n, b.p21.CNT_045n, b.p21.CNT_083n);
  /*p21.VOKU*/         c.p21.CNT_000n = nand(b.p21.CNT_6n, b.p21.CNT_5n, b.p21.CNT_4n, b.p21.CNT_3n, b.p21.CNT_2n, b.p21.CNT_1n, b.p21.CNT_0n); // 0000000 == 0
  /*p21.TOZU*/         c.p21.CNT_007n = nand(b.p21.CNT_6n, b.p21.CNT_5n, b.p21.CNT_4n, b.p21.CNT_3n, b.p21.CNT_2,  b.p21.CNT_1,  b.p21.CNT_0); // 0000111 == 7
  /*p21.TECE*/         c.p21.CNT_045n = nand(b.p21.CNT_6n, b.p21.CNT_5,  b.p21.CNT_4n, b.p21.CNT_3,  b.p21.CNT_2,  b.p21.CNT_1n, b.p21.CNT_0); // 0101101 == 45
  /*p21.TEBO*/         c.p21.CNT_083n = nand(b.p21.CNT_6,  b.p21.CNT_4n, b.p21.CNT_4,  b.p21.CNT_6n, b.p21.CNT_2n, b.p21.CNT_1,  b.p21.CNT_0); // 1010011 == 83

                                                                                                                                               // Clock
  /*p21.RYPO*/ c.p21.CP = not(b.p21.CPn);
  /*p21.SEMU*/   c.p21.CPn = or(b.p21.TOBA, b.p27.FINE_MATCH_TRIG);
  /*p21.TOBA*/     c.p21.TOBA = and(b.p24.CLKPIPE, b.p21.WUSA);
  /*p21.WUSA*/       c.p21.WUSA = or(b.p21.X_009, b.p21.WEGO);

  // Horizontal sync
  /*p24.RUZE*/ c.chip.ST = not(b.p24.POFY);
  /*p24.POFY*/   c.p24.POFY = not(b.p24.RUJU);
  /*p24.RUJU*/     c.p24.RUJU = or(b.p24.PAHO, b.sys.VID_RESET3, b.p24.POME);
  /*p24.PAHO*/        c.p24.PAHO = tock_pos(a.p24.FINE_MATCH_CLK, b.p24.FINE_MATCH_CLK, b.p21.RENDERING, b.p24.PAHO, b.p21.X3);
  /*p24.POME*/        c.p24.POME = nor(b.spr.SCAN_DONE_TRIG, b.p24.POFY);


  // Data latch
  /*p24.KYMO*/ c.chip.CPL = not(b.p24.CPLn);
  /*p24.KAHE*/   c.p24.CPLn = amux2(b.p23.LCDC_EN, b.p24.KASA, b.p24.LCDC_ENn, b.p24.UMOB);
  /*p24.KASA*/     c.p24.KASA = not(b.p21.LINE_DONEa);
  /*p24.UMOB*/     c.p24.UMOB = not(b.sys.DIV_06n);

  // Alt signal?
  /*p24.KOFO*/ c.chip.FR  = not(b.p24.FRn);
  /*p24.KUPA*/   c.p24.FRn  = amux2(b.p23.LCDC_EN, b.p24.KEBO, b.p24.LCDC_ENn, b.p24.USEC);
  /*p24.KEBO*/     c.p24.KEBO = not(b.p24.MECO);
  /*p24.MECO*/       c.p24.MECO = not(b.p24.MAGU);
  /*p24.MAGU*/         c.p24.MAGU = xor(b.p21.NAPO, b.p24.LUCA);
  /*p21.NAPO*/           c.p21.NAPO = tock_pos(a.p21.LINE_144_SYNC, b.p21.LINE_144_SYNC, b.sys.VID_RESETn2, b.p21.NAPO, !b.p21.NAPO);
  /*p24.LUCA*/           c.p24.LUCA = tock_pos(a.p24.LINE_DONE,     b.p24.LINE_DONE,     b.sys.VID_RESETn2, b.p24.LUCA, !b.p24.LUCA);
  /*p24.LOFU*/             c.p24.LINE_DONE = not(b.p21.LINE_DONEn);
  /*p24.USEC*/     c.p24.USEC = not(b.sys.DIV_07n);

  // Vertical sync
  /*p24.MURE*/ c.chip.S = not(b.p24.LINE_000_SYNCn);
  /*p24.MEDA*/   c.p24.LINE_000_SYNCn = tock_pos(a.p21.LINE_DONE_DELAYn, b.p21.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.p24.LINE_000_SYNCn, b.p24.LINE_000n);
  /*p24.NERU*/     c.p24.LINE_000n = nor(b.p21.V0, b.p21.V1, b.p21.V2, b.p21.V3, b.p21.V4, b.p21.V5, b.p21.V6, b.p21.V7);

  /*p24.KEDY*/ c.p24.LCDC_ENn = not(b.p23.LCDC_EN);


  // Unused
  /*p24.LEBE*/ c.p24.LEBE = tock_pos(!a.p24.LUCA, !b.p24.LUCA, b.sys.VID_RESETn2, b.p24.LEBE, !b.p24.LEBE);
}

//-----------------------------------------------------------------------------

void P25_VramInterface_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // MAXX pin driver

  /*p25.MYFU*/ c.p25.MA00n = not(b.MA00);
  /*p25.MASA*/ c.p25.MA01n = not(b.MA01);
  /*p25.MYRE*/ c.p25.MA02n = not(b.MA02);
  /*p25.MAVU*/ c.p25.MA03n = not(b.MA03);
  /*p25.MEPA*/ c.p25.MA04n = not(b.MA04);
  /*p25.MYSA*/ c.p25.MA05n = not(b.MA05);
  /*p25.MEWY*/ c.p25.MA06n = not(b.MA06);
  /*p25.MUME*/ c.p25.MA07n = not(b.MA07);
  /*p25.VOVA*/ c.p25.MA08n = not(b.MA08);
  /*p25.VODE*/ c.p25.MA09n = not(b.MA09);
  /*p25.RUKY*/ c.p25.MA10n = not(b.MA10);
  /*p25.RUMA*/ c.p25.MA11n = not(b.MA11);
  /*p25.REHO*/ c.p25.MA12n = not(b.MA12);

  /*p25.LEXE*/ c.p25.MA00b = not(c.p25.MA00n);
  /*p25.LOZU*/ c.p25.MA01b = not(c.p25.MA01n);
  /*p25.LACA*/ c.p25.MA02b = not(c.p25.MA02n);
  /*p25.LUVO*/ c.p25.MA03b = not(c.p25.MA03n);
  /*p25.LOLY*/ c.p25.MA04b = not(c.p25.MA04n);
  /*p25.LALO*/ c.p25.MA05b = not(c.p25.MA05n);
  /*p25.LEFA*/ c.p25.MA06b = not(c.p25.MA06n);
  /*p25.LUBY*/ c.p25.MA07b = not(c.p25.MA07n);
  /*p25.TUJY*/ c.p25.MA08b = not(c.p25.MA08n);
  /*p25.TAGO*/ c.p25.MA09b = not(c.p25.MA09n);
  /*p25.NUVA*/ c.p25.MA10b = not(c.p25.MA10n);
  /*p25.PEDU*/ c.p25.MA11b = not(c.p25.MA11n);
  /*p25.PONY*/ c.p25.MA12b = not(c.p25.MA12n);

  c.chip.MA00 = c.p25.MA00b;
  c.chip.MA01 = c.p25.MA01b;
  c.chip.MA02 = c.p25.MA02b;
  c.chip.MA03 = c.p25.MA03b;
  c.chip.MA04 = c.p25.MA04b;
  c.chip.MA05 = c.p25.MA05b;
  c.chip.MA06 = c.p25.MA06b;
  c.chip.MA07 = c.p25.MA07b;
  c.chip.MA08 = c.p25.MA08b;
  c.chip.MA09 = c.p25.MA09b;
  c.chip.MA10 = c.p25.MA10b;
  c.chip.MA11 = c.p25.MA11b;
  c.chip.MA12 = c.p25.MA12b;

  //----------

  /*p25.SYRO*/ c.p25.ADDR_FE00_FFFF = not(b.sys.ADDR_0000_FE00);
  /*p25.TEFA*/ c.p25.TEFA = nor(b.p25.ADDR_FE00_FFFF, b.sys.ADDR_VALID_AND_NOT_VRAM);\

    // guess
    /*p25.SOSE*/ c.p25.ADDR_VRAM = and(b.A15, b.p25.TEFA);

  // guess
  /*p25.TUCA*/ c.p25.CPU_VRAM_RD  = and (b.p25.ADDR_VRAM, b.sys.CPU_RD_SYNC);
  /*p25.TUJA*/ c.p25.CPU_VRAM_WR  = and (b.p25.ADDR_VRAM, b.sys.CPU_WR_SYNC);
  /*p25.TEGU*/ c.p25.CPU_VRAM_CLK = nand(b.p25.ADDR_VRAM, b.sys.CLK_xxxDxxxx1);

  /*p25.TEFY*/ c.p25.MCS_Cn = not(b.chip.MCS_C);
  /*p25.SUDO*/ c.p25.MWR_Cn = not(b.chip.MWR_C);
  /*p25.TAVY*/ c.p25.MOE_Cn = not(b.chip.MOE_C);

  /*p25.TOLE*/ c.p25.CPU_VRAM_RD2  = mux2(b.p25.MCS_Cn, b.p25.CPU_VRAM_RD , b.p25.DBG_TUTO);
  /*p25.TYJY*/ c.p25.CPU_VRAM_WR2  = mux2(b.p25.MWR_Cn, b.p25.CPU_VRAM_WR , b.p25.DBG_TUTO);
  /*p25.SALE*/ c.p25.CPU_VRAM_CLK2 = mux2(b.p25.MOE_Cn, b.p25.CPU_VRAM_CLK, b.p25.DBG_TUTO);

  /*p25.RUVY*/ c.p25.CPU_VRAM_CLK2n = not(b.p25.CPU_VRAM_CLK2);

  /*p25.SERE*/ c.p25.SERE = and(b.p25.CPU_VRAM_RD2,   b.p25.RENDERINGn);
  /*p25.ROPY*/   c.p25.RENDERINGn = not(b.p21.RENDERING);

  /*p25.SAZO*/ c.p25.MD_OUTd = and(b.p25.CPU_VRAM_CLK2n, b.p25.SERE);
  /*p25.RYJE*/ c.p25.MD_INb  = not(b.p25.MD_OUTd);
  /*p25.REVO*/ c.p25.MD_OUTc = not(b.p25.MD_INb);
  /*p25.ROCY*/ c.p25.MD_OUTe = and(b.p25.MD_OUTd, b.p25.MD_OUTc);
  /*p25.RAHU*/ c.p25.D_TO_MD = not(b.p25.MD_OUTe);

  /*p25.RELA*/ c.p25.MD_OUTb = or(b.p25.MD_OUTc, b.p25.MD_OUTd);
  /*p25.RENA*/ c.p25.MD_IN   = not(b.p25.MD_OUTb);
  /*p25.ROFA*/ c.p25.MD_OUT  = not(b.p25.MD_IN);

  c.chip.MD0_B = b.p25.MD_OUT;
  c.chip.MD1_B = b.p25.MD_OUT;
  c.chip.MD2_B = b.p25.MD_OUT;
  c.chip.MD3_B = b.p25.MD_OUT;
  c.chip.MD4_B = b.p25.MD_OUT;
  c.chip.MD5_B = b.p25.MD_OUT;
  c.chip.MD6_B = b.p25.MD_OUT;
  c.chip.MD7_B = b.p25.MD_OUT;





  /*p25.SOKY*/ c.chip.MCS_A = not(b.p25.MCS_An);
  /*p25.SETY*/ c.chip.MCS_D = not(b.p25.MCS_Dn);
  /*p25.TODE*/   c.p25.MCS_An = and(b.p25.MCS, b.p25.DBG_TUTOn);
  /*p25.SEWO*/   c.p25.MCS_Dn = or(b.p25.MCS, b.p25.DBG_TUTO);
  /*p25.SUTU*/     c.p25.MCS = nor(b.p27.LONYb, b.sys.VRAM_TO_OAMn, b.spr.TEXY, b.p25.SERE);

  /*p25.SYSY*/ c.chip.MWR_A = not(b.p25.MWR_An);
  /*p25.RAGU*/ c.chip.MWR_D = not(b.p25.MWR_Dn);
  /*p25.TAXY*/   c.p25.MWR_An = and(b.p25.MWR, b.p25.DBG_TUTOn);
  /*p25.SOFY*/   c.p25.MWR_Dn = or(b.p25.MWR, b.p25.DBG_TUTO);
  /*p25.SOHY*/     c.p25.MWR = nand(b.p25.CPU_VRAM_WR2, b.p25.SERE);

  /*p25.REFO*/ c.chip.MOE_A = not(b.p25.MOE_An);
  /*p25.SAHA*/ c.chip.MOE_D = not(b.p25.MOE_Dn);
  /*p25.SEMA*/   c.p25.MOE_An    = and(b.p25.MOE, b.p25.DBG_TUTOn);
  /*p25.RUTE*/   c.p25.MOE_Dn    = or(b.p25.MOE, b.p25.DBG_TUTO); // schematic wrong, second input is RACU
  /*p25.RACU*/     c.p25.MOE = and(b.p25.RYLU, b.p25.RAWA, b.p27.MYMA, b.p25.VRAM_TO_OAM);
  /*p25.RYLU*/       c.p25.RYLU = nand(b.p25.CPU_VRAM_CLK2, b.p25.RENDERINGn);
  /*p25.RAWA*/       c.p25.RAWA = not(b.p25.SOHO);
  /*p25.SOHO*/         c.p25.SOHO = and(b.spr.TACU, b.spr.TEXY);
  /*p27.MYMA*/       c.p27.MYMA = not(b.p27.LONY);
  /*p25.APAM*/       c.p25.VRAM_TO_OAM = not(b.sys.VRAM_TO_OAMn);

  //----------

  /*p25.RODY*/ if (b.p25.MD_IN) c.MD0 = b.chip.MD0_C;
  /*p25.REBA*/ if (b.p25.MD_IN) c.MD1 = b.chip.MD1_C;
  /*p25.RYDO*/ if (b.p25.MD_IN) c.MD2 = b.chip.MD2_C;
  /*p25.REMO*/ if (b.p25.MD_IN) c.MD3 = b.chip.MD3_C;
  /*p25.ROCE*/ if (b.p25.MD_IN) c.MD4 = b.chip.MD4_C;
  /*p25.ROPU*/ if (b.p25.MD_IN) c.MD5 = b.chip.MD5_C;
  /*p25.RETA*/ if (b.p25.MD_IN) c.MD6 = b.chip.MD6_C;
  /*p25.RAKU*/ if (b.p25.MD_IN) c.MD7 = b.chip.MD7_C;

  //----------
  // more debug stuff

  /*p25.SYCY*/ c.p25.MODE_DBG2n = not(b.sys.MODE_DBG2);
  /*p25.SOTO*/ c.p25.DBG_SOTO   = tock_pos(a.p25.MODE_DBG2n, b.p25.MODE_DBG2n, b.sys.SYS_RESETn2, b.p25.DBG_SOTO, !b.p25.DBG_SOTO);
  /*p25.TUTO*/ c.p25.DBG_TUTO   = and(b.sys.MODE_DBG2, !b.p25.DBG_SOTO);
  /*p25.RACO*/ c.p25.DBG_TUTOn  = not(b.p25.DBG_TUTO);

  /*p25.TUSO*/ c.p25.TUSO = nor(b.sys.MODE_DBG2, b.sys.CPU_CLK1n);
  /*p25.SOLE*/ c.p25.SOLE = not(b.p25.TUSO);
  /*p25.????*/ c.p25.P10_Bn = not(b.chip.P10_B);

  /*p25.TOVU*/ if (b.p25.P10_Bn) c.D0 = b.p25.SOLE;
  /*p25.SOSA*/ if (b.p25.P10_Bn) c.D1 = b.p25.SOLE;
  /*p25.SEDU*/ if (b.p25.P10_Bn) c.D2 = b.p25.SOLE;
  /*p25.TAXO*/ if (b.p25.P10_Bn) c.D3 = b.p25.SOLE;
  /*p25.TAHY*/ if (b.p25.P10_Bn) c.D4 = b.p25.SOLE;
  /*p25.TESU*/ if (b.p25.P10_Bn) c.D5 = b.p25.SOLE;
  /*p25.TAZU*/ if (b.p25.P10_Bn) c.D6 = b.p25.SOLE;
  /*p25.TEWA*/ if (b.p25.P10_Bn) c.D7 = b.p25.SOLE;

  //----------

  /*p25.TEME*/ if (b.p25.D_TO_MD) c.MD0 = b.D0; // well this is clearly backwards
  /*p25.TEWU*/ if (b.p25.D_TO_MD) c.MD1 = b.D1;
  /*p25.TYGO*/ if (b.p25.D_TO_MD) c.MD2 = b.D2;
  /*p25.SOTE*/ if (b.p25.D_TO_MD) c.MD3 = b.D3;
  /*p25.SEKE*/ if (b.p25.D_TO_MD) c.MD4 = b.D4;
  /*p25.RUJO*/ if (b.p25.D_TO_MD) c.MD5 = b.D5;
  /*p25.TOFA*/ if (b.p25.D_TO_MD) c.MD6 = b.D6;
  /*p25.SUZA*/ if (b.p25.D_TO_MD) c.MD7 = b.D7;

  /*p25.SYNU*/ c.p25.SYNU = or(b.p25.D_TO_MD, b.MD0);
  /*p25.SYMA*/ c.p25.SYMA = or(b.p25.D_TO_MD, b.MD1);
  /*p25.ROKO*/ c.p25.ROKO = or(b.p25.D_TO_MD, b.MD2);
  /*p25.SYBU*/ c.p25.SYBU = or(b.p25.D_TO_MD, b.MD3);
  /*p25.SAKO*/ c.p25.SAKO = or(b.p25.D_TO_MD, b.MD4);
  /*p25.SEJY*/ c.p25.SEJY = or(b.p25.D_TO_MD, b.MD5);
  /*p25.SEDO*/ c.p25.SEDO = or(b.p25.D_TO_MD, b.MD6);
  /*p25.SAWU*/ c.p25.SAWU = or(b.p25.D_TO_MD, b.MD7);

  /*p25.RURA*/ c.p25.RURA = not(b.p25.SYNU);
  /*p25.RULY*/ c.p25.RULY = not(b.p25.SYMA);
  /*p25.RARE*/ c.p25.RARE = not(b.p25.ROKO);
  /*p25.RODU*/ c.p25.RODU = not(b.p25.SYBU);
  /*p25.RUBE*/ c.p25.RUBE = not(b.p25.SAKO);
  /*p25.RUMU*/ c.p25.RUMU = not(b.p25.SEJY);
  /*p25.RYTY*/ c.p25.RYTY = not(b.p25.SEDO);
  /*p25.RADY*/ c.p25.RADY = not(b.p25.SAWU);

  c.chip.MD0_D = b.p25.RURA;
  c.chip.MD1_D = b.p25.RULY;
  c.chip.MD2_D = b.p25.RARE;
  c.chip.MD3_D = b.p25.RODU;
  c.chip.MD4_D = b.p25.RUBE;
  c.chip.MD5_D = b.p25.RUMU;
  c.chip.MD6_D = b.p25.RYTY;
  c.chip.MD7_D = b.p25.RADY;

  //----------

  /*p25.XANE*/ c.p25.A_TO_MAn = nor(b.sys.VRAM_TO_OAMn, b.p21.RENDERING);
  /*p25.XEDU*/ c.p25.A_TO_MA = not(b.p25.A_TO_MAn);

  /*p25.XAKY*/ if (b.p25.A_TO_MA) c.chip.MA00 = b.A00;
  /*p25.XUXU*/ if (b.p25.A_TO_MA) c.chip.MA01 = b.A01;
  /*p25.XYNE*/ if (b.p25.A_TO_MA) c.chip.MA02 = b.A02;
  /*p25.XODY*/ if (b.p25.A_TO_MA) c.chip.MA03 = b.A03;
  /*p25.XECA*/ if (b.p25.A_TO_MA) c.chip.MA04 = b.A04;
  /*p25.XOBA*/ if (b.p25.A_TO_MA) c.chip.MA05 = b.A05;
  /*p25.XOPO*/ if (b.p25.A_TO_MA) c.chip.MA06 = b.A06;
  /*p25.XYBO*/ if (b.p25.A_TO_MA) c.chip.MA07 = b.A07;
  /*p25.RYSU*/ if (b.p25.A_TO_MA) c.chip.MA08 = b.A08;
  /*p25.RESE*/ if (b.p25.A_TO_MA) c.chip.MA09 = b.A09;
  /*p25.RUSE*/ if (b.p25.A_TO_MA) c.chip.MA10 = b.A10;
  /*p25.RYNA*/ if (b.p25.A_TO_MA) c.chip.MA11 = b.A11;
  /*p25.RUMO*/ if (b.p25.A_TO_MA) c.chip.MA12 = b.A12;

  //----------

  /*p25.ROVE*/ c.p25.D_TO_MDn = not(b.p25.D_TO_MD);

  /*p25.SEFA*/ c.p25.SEFA_00 = and(b.MD0, b.p25.D_TO_MDn);
  /*p25.SOGO*/ c.p25.SOGO_01 = and(b.MD1, b.p25.D_TO_MDn);
  /*p25.SEFU*/ c.p25.SEFU_02 = and(b.MD2, b.p25.D_TO_MDn);
  /*p25.SUNA*/ c.p25.SUNA_03 = and(b.MD3, b.p25.D_TO_MDn);
  /*p25.SUMO*/ c.p25.SUMO_04 = and(b.MD4, b.p25.D_TO_MDn);
  /*p25.SAZU*/ c.p25.SAZU_05 = and(b.MD5, b.p25.D_TO_MDn);
  /*p25.SAMO*/ c.p25.SAMO_06 = and(b.MD6, b.p25.D_TO_MDn);
  /*p25.SUKE*/ c.p25.SUKE_07 = and(b.MD7, b.p25.D_TO_MDn);

  /*p25.REGE*/ c.p25.REGE_00 = not(b.p25.SEFA_00);
  /*p25.RYKY*/ c.p25.RYKY_01 = not(b.p25.SOGO_01);
  /*p25.RAZO*/ c.p25.RAZO_02 = not(b.p25.SEFU_02);
  /*p25.RADA*/ c.p25.RADA_03 = not(b.p25.SUNA_03);
  /*p25.RYRO*/ c.p25.RYRO_04 = not(b.p25.SUMO_04);
  /*p25.REVU*/ c.p25.REVU_05 = not(b.p25.SAZU_05);
  /*p25.REKU*/ c.p25.REKU_06 = not(b.p25.SAMO_06);
  /*p25.RYZE*/ c.p25.RYZE_07 = not(b.p25.SUKE_07);

  c.chip.MD0_A = b.p25.REGE_00;
  c.chip.MD1_A = b.p25.RYKY_01;
  c.chip.MD2_A = b.p25.RAZO_02;
  c.chip.MD3_A = b.p25.RADA_03;
  c.chip.MD4_A = b.p25.RYRO_04;
  c.chip.MD5_A = b.p25.REVU_05;
  c.chip.MD6_A = b.p25.REKU_06;
  c.chip.MD7_A = b.p25.RYZE_07;

  //----------

  /*p25.RALO*/ c.p25.D0n = not(b.chip.D0_C);
  /*p25.TUNE*/ c.p25.D1n = not(b.chip.D1_C);
  /*p25.SERA*/ c.p25.D2n = not(b.chip.D2_C);
  /*p25.TENU*/ c.p25.D3n = not(b.chip.D3_C);
  /*p25.SYSA*/ c.p25.D4n = not(b.chip.D4_C);
  /*p25.SUGY*/ c.p25.D5n = not(b.chip.D5_C);
  /*p25.TUBE*/ c.p25.D6n = not(b.chip.D6_C);
  /*p25.SYZO*/ c.p25.D7n = not(b.chip.D7_C);

  //----------

  /*p25.TYVY*/ c.p25.MD_TO_Dn = nand(b.p25.SERE, b.spr.CPU_READ_MYSTERY);
  /*p25.SEBY*/ c.p25.MD_TO_D = not(b.p25.MD_TO_Dn);

  /*p25.RERY*/ c.p25.RERY_00 = not(b.MD0);
  /*p25.RUNA*/ c.p25.RUNA_01 = not(b.MD1);
  /*p25.RONA*/ c.p25.RONA_02 = not(b.MD2);
  /*p25.RUNO*/ c.p25.RUNO_03 = not(b.MD3);
  /*p25.SANA*/ c.p25.SANA_04 = not(b.MD4);
  /*p25.RORO*/ c.p25.RORO_05 = not(b.MD5);
  /*p25.RABO*/ c.p25.RABO_06 = not(b.MD6);
  /*p25.SAME*/ c.p25.SAME_07 = not(b.MD7);

  /*p25.RUGA*/ if (b.p25.MD_TO_D) c.D0 = b.p25.RERY_00;
  /*p25.ROTA*/ if (b.p25.MD_TO_D) c.D1 = b.p25.RUNA_01;
  /*p25.RYBU*/ if (b.p25.MD_TO_D) c.D2 = b.p25.RONA_02;
  /*p25.RAJU*/ if (b.p25.MD_TO_D) c.D3 = b.p25.RUNO_03;
  /*p25.TYJA*/ if (b.p25.MD_TO_D) c.D4 = b.p25.SANA_04;
  /*p25.REXU*/ if (b.p25.MD_TO_D) c.D5 = b.p25.RORO_05;
  /*p25.RUPY*/ if (b.p25.MD_TO_D) c.D6 = b.p25.RABO_06;
  /*p25.TOKU*/ if (b.p25.MD_TO_D) c.D7 = b.p25.SAME_07;
}

//-----------------------------------------------------------------------------

void P26_Background_tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {


  //----------
  // y + scy;

  /*p26.FAFO*/ c.p26.TILE_Y0C = add_c(b.p21.V0, b.p23.SCY0, 0);
  /*p26.EMUX*/ c.p26.TILE_Y1C = add_c(b.p21.V1, b.p23.SCY1, b.p26.TILE_Y0C);
  /*p26.ECAB*/ c.p26.TILE_Y2C = add_c(b.p21.V2, b.p23.SCY2, b.p26.TILE_Y1C);
  /*p26.ETAM*/ c.p26.MAP_Y0C  = add_c(b.p21.V3, b.p23.SCY3, b.p26.TILE_Y2C);
  /*p26.DOTO*/ c.p26.MAP_Y1C  = add_c(b.p21.V4, b.p23.SCY4, b.p26.MAP_Y0C);
  /*p26.DABA*/ c.p26.MAP_Y2C  = add_c(b.p21.V5, b.p23.SCY5, b.p26.MAP_Y1C);
  /*p26.EFYK*/ c.p26.MAP_Y3C  = add_c(b.p21.V6, b.p23.SCY6, b.p26.MAP_Y2C);
  /*p26.EJOK*/ c.p26.MAP_Y4C  = add_c(b.p21.V7, b.p23.SCY7, b.p26.MAP_Y3C);

  /*p26.FAFO*/ c.p26.TILE_Y0  = add_s(b.p21.V0, b.p23.SCY0, 0);
  /*p26.EMUX*/ c.p26.TILE_Y1  = add_s(b.p21.V1, b.p23.SCY1, b.p26.TILE_Y0C);
  /*p26.ECAB*/ c.p26.TILE_Y2  = add_s(b.p21.V2, b.p23.SCY2, b.p26.TILE_Y1C);
  /*p26.ETAM*/ c.p26.MAP_Y0   = add_s(b.p21.V3, b.p23.SCY3, b.p26.TILE_Y2C);
  /*p26.DOTO*/ c.p26.MAP_Y1   = add_s(b.p21.V4, b.p23.SCY4, b.p26.MAP_Y0C);
  /*p26.DABA*/ c.p26.MAP_Y2   = add_s(b.p21.V5, b.p23.SCY5, b.p26.MAP_Y1C);
  /*p26.EFYK*/ c.p26.MAP_Y3   = add_s(b.p21.V6, b.p23.SCY6, b.p26.MAP_Y2C);
  /*p26.EJOK*/ c.p26.MAP_Y4   = add_s(b.p21.V7, b.p23.SCY7, b.p26.MAP_Y3C);

  /*p26.AXAD*/ c.p26.AXAD = not(b.p27.TILE_Y_CLKn);
  /*p26.ASUL*/ c.p26.FETCH_TILEn = and(b.p26.AXAD, b.p27.WIN_TILE_READa);
  /*p26.BEJE*/ c.p26.FETCH_TILE = not(b.p26.FETCH_TILEn);

  /*p26.ASUM*/ if (b.p26.FETCH_TILE) c.MA00 = b.p27.BG_SEQ_xxxx4567b;
  /*p26.EVAD*/ if (b.p26.FETCH_TILE) c.MA01 = b.p26.TILE_Y0;
  /*p26.DAHU*/ if (b.p26.FETCH_TILE) c.MA02 = b.p26.TILE_Y1;
  /*p26.DODE*/ if (b.p26.FETCH_TILE) c.MA03 = b.p26.TILE_Y2;

  //----------
  // x + scx

  /*p26.ATAD*/ c.p26.TILE_X0C = add_c(b.p21.X0, b.p23.SCX0, 0);
  /*p26.BEHU*/ c.p26.TILE_X1C = add_c(b.p21.X1, b.p23.SCX1, b.p26.TILE_X0C);
  /*p26.APYH*/ c.p26.TILE_X2C = add_c(b.p21.X2, b.p23.SCX2, b.p26.TILE_X1C);
  /*p26.BABE*/ c.p26.MAP_X0C  = add_c(b.p21.X3, b.p23.SCX3, b.p26.TILE_X2C);
  /*p26.ABOD*/ c.p26.MAP_X1C  = add_c(b.p21.X4, b.p23.SCX4, b.p26.MAP_X0C);
  /*p26.BEWY*/ c.p26.MAP_X2C  = add_c(b.p21.X5, b.p23.SCX5, b.p26.MAP_X1C);
  /*p26.BYCA*/ c.p26.MAP_X3C  = add_c(b.p21.X6, b.p23.SCX6, b.p26.MAP_X2C);
  /*p26.ACUL*/ c.p26.MAP_X4C  = add_c(b.p21.X7, b.p23.SCX7, b.p26.MAP_X3C);

  /*p26.ATAD*/ c.p26.TILE_X0  = add_s(b.p21.X0, b.p23.SCX0, 0);
  /*p26.BEHU*/ c.p26.TILE_X1  = add_s(b.p21.X1, b.p23.SCX1, b.p26.TILE_X0C);
  /*p26.APYH*/ c.p26.TILE_X2  = add_s(b.p21.X2, b.p23.SCX2, b.p26.TILE_X1C);
  /*p26.BABE*/ c.p26.MAP_X0   = add_s(b.p21.X3, b.p23.SCX3, b.p26.TILE_X2C);
  /*p26.ABOD*/ c.p26.MAP_X1   = add_s(b.p21.X4, b.p23.SCX4, b.p26.MAP_X0C);
  /*p26.BEWY*/ c.p26.MAP_X2   = add_s(b.p21.X5, b.p23.SCX5, b.p26.MAP_X1C);
  /*p26.BYCA*/ c.p26.MAP_X3   = add_s(b.p21.X6, b.p23.SCX6, b.p26.MAP_X2C);
  /*p26.ACUL*/ c.p26.MAP_X4   = add_s(b.p21.X7, b.p23.SCX7, b.p26.MAP_X3C);

  //----------
  // map fetch

  /*p26.ACEN*/ c.p26.FETCH_MAPn = and(b.p26.AXAD, b.p27.POTU);
  /*p26.BAFY*/ c.p26.FETCH_MAP  = not(b.p26.FETCH_MAPn);

  /*p26.AXEP*/ if (b.p26.FETCH_MAP) c.MA00 = b.p26.MAP_X0;
  /*p26.AFEB*/ if (b.p26.FETCH_MAP) c.MA01 = b.p26.MAP_X1;
  /*p26.ALEL*/ if (b.p26.FETCH_MAP) c.MA02 = b.p26.MAP_X2;
  /*p26.COLY*/ if (b.p26.FETCH_MAP) c.MA03 = b.p26.MAP_X3;
  /*p26.AJAN*/ if (b.p26.FETCH_MAP) c.MA04 = b.p26.MAP_X4;

  /*p26.DUHO*/ if (b.p26.FETCH_MAP) c.MA05 = b.p26.MAP_Y0;
  /*p26.CASE*/ if (b.p26.FETCH_MAP) c.MA06 = b.p26.MAP_Y1;
  /*p26.CYPO*/ if (b.p26.FETCH_MAP) c.MA07 = b.p26.MAP_Y2;
  /*p26.CETA*/ if (b.p26.FETCH_MAP) c.MA08 = b.p26.MAP_Y3;
  /*p26.DAFE*/ if (b.p26.FETCH_MAP) c.MA09 = b.p26.MAP_Y4;

  /*p26.AMUV*/ if (b.p26.FETCH_MAP) c.MA10 = b.p23.LCDC_BGMAP;
  /*p26.COVE*/ if (b.p26.FETCH_MAP) c.MA11 = b.spr.P10_Bn;
  /*p26.COXO*/ if (b.p26.FETCH_MAP) c.MA12 = b.spr.P10_Bn;
}

//-----------------------------------------------------------------------------

void P27_WindowMapLookup_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p27.REPU*/ c.p27.IN_FRAME_Y  = nor(b.p21.INT_VBL, b.sys.VID_RESET4);   // schematic wrong, this is NOR
  /*p27.SYNY*/ c.p27.IN_FRAME_Yn = not(b.p27.IN_FRAME_Y);

  //----------
  // Window Y match

  /*p27.MYLO*/ c.p27.WX_MATCH0n = xor(b.p21.X0, b.p23.WX0);
  /*p27.PUWU*/ c.p27.WX_MATCH1n = xor(b.p21.X1, b.p23.WX1);
  /*p27.PUHO*/ c.p27.WX_MATCH2n = xor(b.p21.X2, b.p23.WX2);
  /*p27.NYTU*/ c.p27.WX_MATCH3n = xor(b.p21.X3, b.p23.WX3);
  /*p27.NEZO*/ c.p27.WX_MATCH4n = xor(b.p21.X4, b.p23.WX4);
  /*p27.NORY*/ c.p27.WX_MATCH5n = xor(b.p21.X5, b.p23.WX5);
  /*p27.NONO*/ c.p27.WX_MATCH6n = xor(b.p21.X6, b.p23.WX6);
  /*p27.PASE*/ c.p27.WX_MATCH7n = xor(b.p21.X7, b.p23.WX7);

  /*p27.NAZE*/ c.p27.WY_MATCH0n = xor(b.p21.V0, b.p23.WY0);
  /*p27.PEBO*/ c.p27.WY_MATCH1n = xor(b.p21.V1, b.p23.WY1);
  /*p27.POMO*/ c.p27.WY_MATCH2n = xor(b.p21.V2, b.p23.WY2);
  /*p27.NEVU*/ c.p27.WY_MATCH3n = xor(b.p21.V3, b.p23.WY3);
  /*p27.NOJO*/ c.p27.WY_MATCH4n = xor(b.p21.V4, b.p23.WY4);
  /*p27.PAGA*/ c.p27.WY_MATCH5n = xor(b.p21.V5, b.p23.WY5);
  /*p27.PEZO*/ c.p27.WY_MATCH6n = xor(b.p21.V6, b.p23.WY6);
  /*p27.NUPA*/ c.p27.WY_MATCH7n = xor(b.p21.V7, b.p23.WY7);

  // WIN_EN polarity?
  /*p27.PALO*/ c.p27.WY_MATCH_HI  = nand(b.p23.LCDC_WINEN, b.p27.WY_MATCH4n, b.p27.WY_MATCH5n, b.p27.WY_MATCH6n, b.p27.WY_MATCH7n);
  /*p27.NELE*/ c.p27.WY_MATCH_HIn = not(b.p27.WY_MATCH_HI);
  /*p27.PAFU*/ c.p27.WY_MATCHn    = nand(b.p27.WY_MATCH_HIn, b.p27.WY_MATCH0n, b.p27.WY_MATCH1n, b.p27.WY_MATCH2n, b.p27.WY_MATCH3n);
  /*p27.ROGE*/ c.p27.WY_MATCH     = not(b.p27.WY_MATCHn);

  /*p27.SARY*/ c.p27.WY_MATCH_SYNC = tock_pos(a.p21.CLK_1Mb, b.p21.CLK_1Mb, b.sys.VID_RESETn1, b.p27.WY_MATCH_SYNC, b.p27.WY_MATCH);

  //----------
  // Window X match

  // polarity or gates wrong
  /*p27.REJO*/ c.p27.WIN_CHECK_X = or(b.p27.WY_MATCH_SYNC, b.p27.IN_FRAME_Y); // another weird or gate. should be AND?

  /*p27.PUKY*/ c.p27.WX_MATCH_HI  = nand(b.p27.WIN_CHECK_X, b.p27.WX_MATCH4n, b.p27.WX_MATCH5n, b.p27.WX_MATCH6n, b.p27.WX_MATCH7n);
  /*p27.NUFA*/ c.p27.WX_MATCH_HIn = not (b.p27.WX_MATCH_HI);
  /*p27.NOGY*/ c.p27.WIN_MATCHn   = nand(b.p27.WX_MATCH_HIn, b.p27.WX_MATCH0n, b.p27.WX_MATCH1n, b.p27.WX_MATCH2n, b.p27.WX_MATCH3n);
  /*p27.NUKO*/ c.p27.WIN_MATCH    = not (b.p27.WIN_MATCHn);

  //----------
  // Fine x matcher for x scroll

  /*p27.PASO*/ c.p27.FINE_RST = nor(b.p27.MAP_X_CLK_STOPn, b.p27.RENDERINGo);

  /*p27.RYKU*/ c.p27.FINE_CNT0 = tock_pos( a.p27.FINE_CLK,   b.p27.FINE_CLK,  b.p27.FINE_RST, b.p27.FINE_CNT0, !b.p27.FINE_CNT0);
  /*p27.ROGA*/ c.p27.FINE_CNT1 = tock_pos(!a.p27.FINE_CNT0, !b.p27.FINE_CNT0, b.p27.FINE_RST, b.p27.FINE_CNT1, !b.p27.FINE_CNT1);
  /*p27.RUBU*/ c.p27.FINE_CNT2 = tock_pos(!a.p27.FINE_CNT1, !b.p27.FINE_CNT1, b.p27.FINE_RST, b.p27.FINE_CNT2, !b.p27.FINE_CNT2);
  /*p27.PECU*/   c.p27.FINE_CLK = nand(b.p24.FINE_MATCH_CLK, b.p27.FINE_COUNT_STOPn);

  /*p27.ROZE*/ c.p27.FINE_COUNT_STOPn = nand(b.p27.FINE_CNT0, b.p27.FINE_CNT1, b.p27.FINE_CNT2);

  /*p27.POHU*/ c.p27.FINE_MATCHn = not(b.p27.FINE_MATCH);
  /*p27.RONE*/   c.p27.FINE_MATCH = nand(b.p27.FINE_MATCH_TRIG_OR_NOT_RENDERING, b.p27.FINE_MATCH0, b.p27.FINE_MATCH0, b.p27.FINE_MATCH0);
  /*p27.SUHA*/     c.p27.FINE_MATCH0 = xor(b.p23.SCX0, b.p27.FINE_CNT0);
  /*p27.SYBY*/     c.p27.FINE_MATCH1 = xor(b.p23.SCX1, b.p27.FINE_CNT1);
  /*p27.SOZU*/     c.p27.FINE_MATCH2 = xor(b.p23.SCX2, b.p27.FINE_CNT2);




  //----------


  /*p27.PANY*/ c.p27.WIN_MATCH_ONSCREEN = nor(b.p27.WIN_MATCH, b.p27.FINE_COUNT_STOPn);
  /*p27.RYFA*/ c.p27.WIN_MATCH_ONSCREEN_SYNC1 = tock_pos(a.p24.SEGU_4M, b.p24.SEGU_4M, b.p21.RENDERING, b.p27.WIN_MATCH_ONSCREEN_SYNC1, b.p27.WIN_MATCH_ONSCREEN);
  /*p27.RENE*/ c.p27.WIN_MATCH_ONSCREEN_SYNC2 = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p21.RENDERING, b.p27.WIN_MATCH_ONSCREEN_SYNC2, b.p27.WIN_MATCH_ONSCREEN_SYNC1);
  /*p27.SEKO*/ c.p27.WIN_TRIGGER = nor(b.p27.WIN_MATCH_ONSCREEN_SYNC2, !b.p27.WIN_MATCH_ONSCREEN_SYNC1);


  // stall for sprite is going to be in here 
  /*p27.TUXY*/ c.p27.TUXY = nand(b.p27.SOVY, b.p27.SYLO);
  /*p27.SOVY*/   c.p27.SOVY = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn1, b.p27.SOVY, b.p27.RYDY);
  /*p27.SYLO*/   c.p27.SYLO = not(b.p27.RYDY);
  /*p27.RYDY*/     c.p27.RYDY = nor(b.p27.PUKU, b.sys.VID_RESET4, b.p24.PORY);
  /*p27.PUKU*/       c.p27.PUKU = nor(b.p27.NUNY, b.p27.RYDY);











  //----------
  // BG/WIN sequencer


  // background _or_ window trigger
  /*p27.PYNU*/ c.p27.TILE_ABb = or(b.p27.WIN_MATCH_SYNC2, b.p27.MAP_X_RST);
  /*p27.NUNU*/   c.p27.WIN_MATCH_SYNC2 = tock_pos(a.p21.CLK_xBxDxFxHb, b.p21.CLK_xBxDxFxHb, b.sys.VID_RESETn1, b.p27.WIN_MATCH_SYNC2, b.p27.WIN_MATCH_SYNC1);
  /*p27.PYCO*/     c.p27.WIN_MATCH_SYNC1 = tock_pos(a.p27.ROCO_4M,       b.p27.ROCO_4M,       b.sys.VID_RESETn1, b.p27.WIN_MATCH_SYNC1, b.p27.WIN_MATCH);
  /*p27.ROCO*/       c.p27.ROCO_4M = not(b.p24.SEGU_4M);
  /*p27.XOFO*/   c.p27.MAP_X_RST = nand(b.p23.LCDC_WINEN, b.p27.NEW_LINEn, b.sys.VID_RESETn1);
  /*p27.XAHY*/     c.p27.NEW_LINEn = not(b.spr.NEW_LINE);

  /*p27.MOSU*/ c.p27.MOSU = not(b.p27.NYFO);
  /*p27.NYFO*/   c.p27.NYFO = not(b.p27.NUNY);
  /*p27.NUNY*/     c.p27.NUNY = and(!b.p27.NOPA, b.p27.TILE_ABb);
  /*p27.NOPA*/       c.p27.NOPA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn1, b.p27.NOPA, b.p27.TILE_ABb);


  /*p27.LEBO*/ c.p27.BG_SEQ_CLK = nand(b.sys.CLK_AxCxExGx4, b.p27.BG_SEQ_5n);
  /*p27.NYXU*/ c.p27.BG_SEQ_RSTn = nor(b.spr.SCAN_DONE_TRIG, b.p27.MOSU, b.p27.MAP_X_CLK_STOPn);

  /*p27.LAXU*/ c.p27.BG_SEQ_x1x3x5x7 = tock_pos( a.p27.BG_SEQ_CLK,      b.p27.BG_SEQ_CLK,       b.p27.BG_SEQ_RSTn, b.p27.BG_SEQ_x1x3x5x7, !b.p27.BG_SEQ_x1x3x5x7);
  /*p27.MESU*/ c.p27.BG_SEQ_xx23xx67 = tock_pos(!a.p27.BG_SEQ_x1x3x5x7, !b.p27.BG_SEQ_x1x3x5x7, b.p27.BG_SEQ_RSTn, b.p27.BG_SEQ_xx23xx67, !b.p27.BG_SEQ_xx23xx67);
  /*p27.NYVA*/ c.p27.BG_SEQ_xxxx4567 = tock_pos(!a.p27.BG_SEQ_xx23xx67, !b.p27.BG_SEQ_xx23xx67, b.p27.BG_SEQ_RSTn, b.p27.BG_SEQ_xxxx4567, !b.p27.BG_SEQ_xxxx4567);

  /*p27.NAKO*/ c.p27.BG_SEQ_01xx45xx  = not(b.p27.BG_SEQ_xx23xx67);
  /*p27.NOFU*/ c.p27.BG_SEQ_0123xxxx  = not(b.p27.BG_SEQ_xxxx4567);
  /*p27.XUHA*/ c.p27.BG_SEQ_xxxx4567b = not(b.p27.BG_SEQ_0123xxxx);


  /*p27.MOCE*/ c.p27.BG_SEQ_5n = nand(b.p27.BG_SEQ_x1x3x5x7,
                                      b.p27.BG_SEQ_xxxx4567,
                                      b.p27.BG_SEQ_RSTn); // counts to 5? polarity?

  /*p27.LYRY*/ c.p27.BG_SEQ_5 = not(b.p27.BG_SEQ_5n);

  /*p27.NOGU*/ c.p27.BG_SEQ_xx234567 = nand(b.p27.BG_SEQ_01xx45xx,
                                            b.p27.BG_SEQ_0123xxxx);


  /*p27.LOVY*/ c.p27.BG_SEQ5_SYNC = tock_pos(a.p21.CLK_xBxDxFxHc,  b.p21.CLK_xBxDxFxHc, b.p27.BG_SEQ_RSTn, b.p27.BG_SEQ5_SYNC, b.p27.BG_SEQ_5);

  /*p27.LYZU*/ c.p27.BG_SEQ_x1x3x5x7_DELAY = tock_pos(a.sys.CLK_AxCxExGx4,  b.sys.CLK_AxCxExGx4, b.p21.RENDERING, b.p27.BG_SEQ_x1x3x5x7_DELAY,  b.p27.BG_SEQ_x1x3x5x7);

  //----------
  // address output bus

  /*p27.WAZY*/ c.p27.TILE_Y_CLK = not(b.p27.TILE_Y_CLKn);
  /*p27.PORE*/   c.p27.TILE_Y_CLKn = not(b.p27.TILE_ABo);
  /*p27.NOCU*/     c.p27.TILE_ABo    = not(b.p27.TILE_ABb);
  /*p27.VYNO*/ c.p27.TILE_Y0 = tock_pos( a.p27.TILE_Y_CLK, b.p27.TILE_Y_CLK, b.p27.IN_FRAME_Yn, b.p27.TILE_Y0, !b.p27.TILE_Y0);
  /*p27.VUJO*/ c.p27.TILE_Y1 = tock_pos(!a.p27.TILE_Y0,    !b.p27.TILE_Y0,   b.p27.IN_FRAME_Yn, b.p27.TILE_Y1, !b.p27.TILE_Y1);
  /*p27.VYMU*/ c.p27.TILE_Y2 = tock_pos(!a.p27.TILE_Y1,    !b.p27.TILE_Y1,   b.p27.IN_FRAME_Yn, b.p27.TILE_Y2, !b.p27.TILE_Y2);

  /*p27.TEVO*/ c.p27.MAP_X_CLK_STOPn = nor(b.p27.WIN_TRIGGER, b.p27.SUZU, b.p27.TAVE);
  /*p27.SUZU*/   c.p27.SUZU = not(b.p27.TUXY);
  /*p27.TAVE*/   c.p27.TAVE = not(b.p27.SUVU);
  /*p27.SUVU*/     c.p27.SUVU = nand(b.p21.RENDERING, b.p27.ROMO, b.p24.NYKA, b.p24.PORY);
  /*p27.ROMO*/       c.p27.ROMO = not(b.p24.POKY);

  /*p27.VETU*/ c.p27.MAP_X_CLK = and(b.p27.TILE_Y_CLKn, b.p27.MAP_X_CLK_STOPn);
  /*p27.XACO*/ c.p27.MAP_X_RSTn = not(b.p27.MAP_X_RST);

  /*p27.WYKA*/ c.p27.MAP_X0 = tock_pos( a.p27.MAP_X_CLK, b.p27.MAP_X_CLK, b.p27.MAP_X_RSTn, b.p27.MAP_X0, !b.p27.MAP_X0);
  /*p27.WODY*/ c.p27.MAP_X1 = tock_pos(!a.p27.MAP_X0,    !b.p27.MAP_X0,   b.p27.MAP_X_RSTn, b.p27.MAP_X1, !b.p27.MAP_X1);
  /*p27.WOBO*/ c.p27.MAP_X2 = tock_pos(!a.p27.MAP_X1,    !b.p27.MAP_X1,   b.p27.MAP_X_RSTn, b.p27.MAP_X2, !b.p27.MAP_X2);
  /*p27.WYKO*/ c.p27.MAP_X3 = tock_pos(!a.p27.MAP_X2,    !b.p27.MAP_X2,   b.p27.MAP_X_RSTn, b.p27.MAP_X3, !b.p27.MAP_X3);
  /*p27.XOLO*/ c.p27.MAP_X4 = tock_pos(!a.p27.MAP_X3,    !b.p27.MAP_X3,   b.p27.MAP_X_RSTn, b.p27.MAP_X4, !b.p27.MAP_X4);

  /*p27.TUFU*/ c.p27.MAP_Y0 = tock_pos(!a.p27.TILE_Y2,   !b.p27.TILE_Y2, b.p27.IN_FRAME_Yn, b.p27.MAP_Y0, !b.p27.MAP_Y0);
  /*p27.TAXA*/ c.p27.MAP_Y1 = tock_pos(!a.p27.MAP_Y0,    !b.p27.MAP_Y0,  b.p27.IN_FRAME_Yn, b.p27.MAP_Y1, !b.p27.MAP_Y1);
  /*p27.TOZO*/ c.p27.MAP_Y2 = tock_pos(!a.p27.MAP_Y1,    !b.p27.MAP_Y1,  b.p27.IN_FRAME_Yn, b.p27.MAP_Y2, !b.p27.MAP_Y2);
  /*p27.TATE*/ c.p27.MAP_Y3 = tock_pos(!a.p27.MAP_Y2,    !b.p27.MAP_Y2,  b.p27.IN_FRAME_Yn, b.p27.MAP_Y3, !b.p27.MAP_Y3);
  /*p27.TEKE*/ c.p27.MAP_Y4 = tock_pos(!a.p27.MAP_Y3,    !b.p27.MAP_Y3,  b.p27.IN_FRAME_Yn, b.p27.MAP_Y4, !b.p27.MAP_Y4);


  /*p27.VYPO*/ c.p27.P10_Bn = not(b.chip.P10_B);

  /*p25.VUZA*/ c.p25.WIN_TILE_BANK = nor(b.p23.BG_TILE_SEL, b.pix.VRAM_TEMP_D7);

  //----------

  /*p27.TEKY*/ c.p27.TEKY = and(b.spr.OAM_SCAN, b.p27.TUKU, b.p27.BG_SEQ_5, b.p27.SOWO);
  /*p27.TUKU*/   c.p27.TUKU = not(b.p24.TOMU);
  /*p27.SOWO*/   c.p27.SOWO = not(b.p27.TAKA);
  /*p27.TAKA*/     c.p27.TAKA = unk2(b.p27.VEKU, b.p27.SECA_RST);
  /*p27.VEKU*/       c.p27.VEKU = nor(b.spr.WUTY, b.p27.TAVE);
  /*p27.SECA*/       c.p27.SECA_RST = nor(b.p27.TEKY_TRIG, b.sys.VID_RESET5, b.spr.NEW_LINE);
  /*p27.RYCE*/         c.p27.TEKY_TRIG = and(b.p27.TEKY_SYNC1, !b.p27.TEKY_SYNC2);

  /*p27.SOBU*/ c.p27.TEKY_SYNC1 = tock_pos(a.p21.CLK_AxCxExGxa, b.p21.CLK_AxCxExGxa, b.p27.P10_Bn, b.p27.TEKY_SYNC1, b.p27.TEKY);
  /*p27.SUDA*/ c.p27.TEKY_SYNC2 = tock_pos(a.sys.CLK_xBxDxFxH5, b.sys.CLK_xBxDxFxH5, b.p27.P10_Bn, b.p27.TEKY_SYNC2, b.p27.TEKY_SYNC1);

  //----------

  /*p27.LENA*/ c.p27.LONYb = not(b.p27.LONYn);
  /*p27.LUSU*/   c.p27.LONYn = not(b.p27.LONY);
  /*p27.LURY*/     c.p27.LONY = and(c.p27.LURY, b.p27.BG_SEQ_RSTn);
  /*p27.LONY*/       c.p27.LURY = and(!c.p27.BG_SEQ5_SYNC, b.p21.RENDERING);

  // WIN_MAP_READ = and (!BG_SEQ5_SYNC, b.p21.RENDERING, BG_SEQ_RSTn, BG_SEQ_01xxxxxx, TILE_Y_CLKn);
  /*p25.WUKO*/ c.p25.WIN_MAP_READ = not(b.p25.WIN_MAP_READn);
  /*p25.XEZE*/   c.p25.WIN_MAP_READn = nand(b.p27.POTU, b.p27.TILE_Y_CLKn);
  /*p27.POTU*/     c.p27.POTU = and(b.p27.LONYb, b.p27.BG_SEQ_01xxxxxx);
  /*p27.NENY*/       c.p27.BG_SEQ_01xxxxxx = not(b.p27.BG_SEQ_xx234567);

  // WIN_TILE_READ = nand(!BG_SEQ5_SYNC, b.p21.RENDERING, BG_SEQ_RSTn, BG_SEQ_xx234567, TILE_Y_CLKn);
  /*p25.XUCY*/ c.p25.WIN_TILE_READb = nand(b.p27.WIN_TILE_READa, b.p27.TILE_Y_CLKn);
  /*p27.NETA*/ c.p27.WIN_TILE_READa = and(b.p27.LONYb, b.p27.BG_SEQ_xx234567);

  /*p27.XEJA*/ if (b.p25.WIN_MAP_READ) c.MA00 = b.p27.MAP_X0;
  /*p27.XAMO*/ if (b.p25.WIN_MAP_READ) c.MA01 = b.p27.MAP_X1;
  /*p27.XAHE*/ if (b.p25.WIN_MAP_READ) c.MA02 = b.p27.MAP_X2;
  /*p27.XULO*/ if (b.p25.WIN_MAP_READ) c.MA03 = b.p27.MAP_X3;
  /*p27.WUJU*/ if (b.p25.WIN_MAP_READ) c.MA04 = b.p27.MAP_X4;
  /*p27.VYTO*/ if (b.p25.WIN_MAP_READ) c.MA05 = b.p27.MAP_Y0;
  /*p27.VEHA*/ if (b.p25.WIN_MAP_READ) c.MA06 = b.p27.MAP_Y1;
  /*p27.VACE*/ if (b.p25.WIN_MAP_READ) c.MA07 = b.p27.MAP_Y2;
  /*p27.VOVO*/ if (b.p25.WIN_MAP_READ) c.MA08 = b.p27.MAP_Y3;
  /*p27.VULO*/ if (b.p25.WIN_MAP_READ) c.MA09 = b.p27.MAP_Y4;
  /*p27.VEVY*/ if (b.p25.WIN_MAP_READ) c.MA10 = not(b.p23.WIN_MAP_SEL);
  /*p27.VEZA*/ if (b.p25.WIN_MAP_READ) c.MA11 = not(b.p27.P10_Bn);
  /*p27.VOGU*/ if (b.p25.WIN_MAP_READ) c.MA12 = not(b.p27.P10_Bn);

  /*p25.XONU*/ if (b.p25.WIN_TILE_READb) c.MA00 = b.p27.BG_SEQ_xxxx4567b;
  /*p25.WUDO*/ if (b.p25.WIN_TILE_READb) c.MA01 = b.p27.TILE_Y0;
  /*p25.WAWE*/ if (b.p25.WIN_TILE_READb) c.MA02 = b.p27.TILE_Y1;
  /*p25.WOLU*/ if (b.p25.WIN_TILE_READb) c.MA03 = b.p27.TILE_Y2;
  /*p25.VAPY*/ if (b.p27.WIN_TILE_READa) c.MA04 = b.pix.VRAM_TEMP_D0;
  /*p25.SEZU*/ if (b.p27.WIN_TILE_READa) c.MA05 = b.pix.VRAM_TEMP_D1;
  /*p25.VEJY*/ if (b.p27.WIN_TILE_READa) c.MA06 = b.pix.VRAM_TEMP_D2;
  /*p25.RUSA*/ if (b.p27.WIN_TILE_READa) c.MA07 = b.pix.VRAM_TEMP_D3;
  /*p25.ROHA*/ if (b.p27.WIN_TILE_READa) c.MA08 = b.pix.VRAM_TEMP_D4;
  /*p25.RESO*/ if (b.p27.WIN_TILE_READa) c.MA09 = b.pix.VRAM_TEMP_D5;
  /*p25.SUVO*/ if (b.p27.WIN_TILE_READa) c.MA10 = b.pix.VRAM_TEMP_D6;
  /*p25.TOBO*/ if (b.p27.WIN_TILE_READa) c.MA11 = b.pix.VRAM_TEMP_D7;
  /*p25.VUZA*/ if (b.p27.WIN_TILE_READa) c.MA12 = b.p25.WIN_TILE_BANK;
}
