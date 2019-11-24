#include "Video.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P21_VideoControl_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)cpu_in;

  /*p27.MOXE*/ c.vid.CLK_xBxDxFxHa = not(b.sys.CLK_AxCxExGx4);
  /*p27.MEHE*/ c.vid.CLK_xBxDxFxHb = not(b.sys.CLK_AxCxExGx4);
  /*p27.MYVO*/ c.vid.CLK_xBxDxFxHc = not(b.sys.CLK_AxCxExGx4);
  /*p29.LAPE*/ c.vid.CLK_xBxDxFxHd = not(b.sys.CLK_AxCxExGx4);
  /*p29.XOTA*/ c.vid.CLK_xBxDxFxHe = not(b.vid.CLK_AxCxExGxb);

  /*p27.TAVA*/ c.vid.CLK_AxCxExGxa = not(b.sys.CLK_xBxDxFxH5);
  /*p29.XYVA*/ c.vid.CLK_AxCxExGxb = not(b.sys.CLK_xBxDxFxH2);
  /*p29.XYFY*/ c.vid.CLK_AxCxExGxc = not(b.vid.CLK_xBxDxFxHe);

  /*p29.WUVU*/ c.vid.CLK_2Ma = tock_pos(a.vid.CLK_xBxDxFxHe, b.vid.CLK_xBxDxFxHe, b.sys.VID_RESETn1, b.vid.CLK_2Ma, !b.vid.CLK_2Ma);
  /*p29.XUPY*/ c.vid.CLK_2Mb = not(!b.vid.CLK_2Ma);

  /*p21.VENA*/ c.vid.CLK_1Ma = tock_pos(!a.vid.CLK_2Ma, !b.vid.CLK_2Ma, b.sys.VID_RESETn1, b.vid.CLK_1Ma, !b.vid.CLK_1Ma);
  /*p21.TALU*/ c.vid.CLK_1Mb = not(!b.vid.CLK_1Ma);

  //----------
  // x counter. this is a little weird, presumably because it can tick at 4 mhz but not always?

  /*p21.TADY*/ c.vid.TADY = nor(b.spr.NEW_LINE, b.sys.VID_RESET3);

  /*p21.RYBO*/ c.vid.RYBO = xor(b.vid.X0, b.vid.X1);
  /*p21.XUKE*/ c.vid.XUKE = and(b.vid.X0, b.vid.X1);

  /*p21.XYLE*/ c.vid.XYLE = and(b.vid.X2, b.vid.XUKE);
  /*p21.XEGY*/ c.vid.XEGY = xor(b.vid.X2, b.vid.XUKE);

  /*p21.XORA*/ c.vid.XORA = xor(b.vid.X3, b.vid.XYLE);

  /*p21.XEHO*/ c.vid.X0 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.TADY, b.vid.X0, !b.vid.X0);
  /*p21.SAVY*/ c.vid.X1 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.TADY, b.vid.X1, b.vid.RYBO);
  /*p21.XODU*/ c.vid.X2 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.TADY, b.vid.X2, b.vid.XEGY);
  /*p21.XYDO*/ c.vid.X3 = tock_pos(a.vid.CLKPIPE, b.vid.CLKPIPE, b.vid.TADY, b.vid.X3, b.vid.XORA);

  /*p21.TOCA*/ c.vid.TOCA = not(b.vid.X3);
  /*p21.SAKE*/ c.vid.SAKE = xor(b.vid.X4, b.vid.X5);
  /*p21.TYBA*/ c.vid.TYBA = and(b.vid.X4, b.vid.X5);

  /*p21.SURY*/ c.vid.SURY = and(b.vid.X6, b.vid.TYBA);
  /*p21.TYGE*/ c.vid.TYGE = xor(b.vid.X6, b.vid.TYBA);

  /*p21.ROKU*/ c.vid.ROKU = xor(b.vid.X7, b.vid.SURY);

  /*p21.TUHU*/ c.vid.X4 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.TADY, b.vid.X4, !b.vid.X4);
  /*p21.TUKY*/ c.vid.X5 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.TADY, b.vid.X5, b.vid.SAKE);
  /*p21.TAKO*/ c.vid.X6 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.TADY, b.vid.X6, b.vid.TYGE);
  /*p21.SYBE*/ c.vid.X7 = tock_pos(a.vid.TOCA, b.vid.TOCA, b.vid.TADY, b.vid.X7, b.vid.ROKU);


  /*p21.ACAM*/ c.vid.X0n = not(b.vid.X0); // polarity?
  /*p21.AZUB*/ c.vid.X1n = not(b.vid.X1);
  /*p21.AMEL*/ c.vid.X2n = not(b.vid.X2);
  /*p21.AHAL*/ c.vid.X3n = not(b.vid.X3);
  /*p21.APUX*/ c.vid.X4n = not(b.vid.X4);
  /*p21.ABEF*/ c.vid.X5n = not(b.vid.X5);
  /*p21.ADAZ*/ c.vid.X6n = not(b.vid.X6);
  /*p21.ASAH*/ c.vid.X7n = not(b.vid.X7);


  //----------
  // LY compare

  /*p21.SYFU*/ c.vid.LY_MATCH7 = xor(b.vid.V7, b.vid.LYC7);
  /*p21.TERY*/ c.vid.LY_MATCH6 = xor(b.vid.V6, b.vid.LYC6);
  /*p21.TUCY*/ c.vid.LY_MATCH5 = xor(b.vid.V5, b.vid.LYC5);
  /*p21.TYKU*/ c.vid.LY_MATCH4 = xor(b.vid.V4, b.vid.LYC4);
  /*p21.RASY*/ c.vid.LY_MATCH3 = xor(b.vid.V3, b.vid.LYC3);
  /*p21.REDA*/ c.vid.LY_MATCH2 = xor(b.vid.V2, b.vid.LYC2);
  /*p21.TYDE*/ c.vid.LY_MATCH1 = xor(b.vid.V1, b.vid.LYC1);
  /*p21.RYME*/ c.vid.LY_MATCH0 = xor(b.vid.V0, b.vid.LYC0);

  /*p21.SOVU*/ c.vid.LY_MATCHA = nor(b.vid.LY_MATCH7, b.vid.LY_MATCH6, b.vid.LY_MATCH5, b.vid.LY_MATCH4);
  /*p21.SUBO*/ c.vid.LY_MATCHB = nor(b.vid.LY_MATCH3, b.vid.LY_MATCH2, b.vid.LY_MATCH1, b.vid.LY_MATCH0);

  /*p21.RAPE*/ c.vid.LY_MATCHn = nand(b.vid.LY_MATCHA, b.vid.LY_MATCHB);
  /*p21.PALY*/ c.vid.LY_MATCH = not(b.vid.LY_MATCHn);
  /*p21.ROPO*/ c.vid.INT_LYC = tock_pos(a.vid.CLK_1Mb, b.vid.CLK_1Mb, b.sys.SYS_RESETn6, b.vid.INT_LYC, b.vid.LY_MATCH);

  //----------
  // line sequencer?

  /*p21.MUDE*/ c.vid.CNT_RSTn = nor(b.vid.LINE_DONEn, b.sys.VID_RESET2);

  /*p21.SAXO*/ c.vid.CNT_0 = tock_pos( a.vid.CLK_1Mb, b.vid.CLK_1Mb, b.vid.CNT_RSTn, c.vid.CNT_0, !c.vid.CNT_0);
  /*p21.TYPO*/ c.vid.CNT_1 = tock_pos(!a.vid.CNT_0,   !b.vid.CNT_0,  b.vid.CNT_RSTn, c.vid.CNT_1, !c.vid.CNT_1);
  /*p21.VYZO*/ c.vid.CNT_2 = tock_pos(!a.vid.CNT_1,   !b.vid.CNT_1,  b.vid.CNT_RSTn, c.vid.CNT_2, !c.vid.CNT_2);
  /*p21.TELU*/ c.vid.CNT_3 = tock_pos(!a.vid.CNT_2,   !b.vid.CNT_2,  b.vid.CNT_RSTn, c.vid.CNT_3, !c.vid.CNT_3);
  /*p21.SUDE*/ c.vid.CNT_4 = tock_pos(!a.vid.CNT_3,   !b.vid.CNT_3,  b.vid.CNT_RSTn, c.vid.CNT_4, !c.vid.CNT_4);
  /*p21.TAHA*/ c.vid.CNT_5 = tock_pos(!a.vid.CNT_4,   !b.vid.CNT_4,  b.vid.CNT_RSTn, c.vid.CNT_5, !c.vid.CNT_5);
  /*p21.TYRY*/ c.vid.CNT_6 = tock_pos(!a.vid.CNT_5,   !b.vid.CNT_5,  b.vid.CNT_RSTn, c.vid.CNT_6, !c.vid.CNT_6);

  /*p21.TOCU*/ c.vid.CNT_0n = not(b.vid.CNT_0);
  /*p21.VEPE*/ c.vid.CNT_1n = not(b.vid.CNT_1);
  /*p21.VUTY*/ c.vid.CNT_2n = not(b.vid.CNT_2);
  /*p21.VATE*/ c.vid.CNT_3n = not(b.vid.CNT_3);
  /*p21.TUDA*/ c.vid.CNT_4n = not(b.vid.CNT_4);
  /*p21.TAFY*/ c.vid.CNT_5n = not(b.vid.CNT_5);
  /*p21.TUJU*/ c.vid.CNT_6n = not(b.vid.CNT_6);






  /*p21.SANU*/ c.vid.CNT_113n = nand(b.vid.CNT_6, b.vid.CNT_5, b.vid.CNT_4, b.vid.CNT_0);

  /*p21.RUTU*/ c.vid.LINE_DONEn = tock_pos(a.vid.SONO_CLK, b.vid.SONO_CLK, b.sys.VID_RESETn2, b.vid.LINE_DONEn, b.vid.CNT_113n);
  /*p21.SONO*/   c.vid.SONO_CLK   = not(b.vid.CLK_1Mb);



  //----------
  // FF41 STAT

  /*p21.NYPE*/ c.vid.LINE_DONE_DELAYn = tock_pos(a.vid.CLK_1Mb, b.vid.CLK_1Mb, b.sys.VID_RESETn2, b.vid.LINE_DONE_DELAYn, b.vid.LINE_DONEn);

  /*p21.XYVO*/ c.vid.LINE_144 = and(b.vid.V4, b.vid.V7);
  /*p21.POPU*/ c.vid.LINE_144_SYNC = tock_pos(a.vid.LINE_DONE_DELAYn, b.vid.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.vid.LINE_144_SYNC, b.vid.LINE_144);



  /*p21.WODU*/ c.vid.RENDER_DONEn = and(b.vid.OAM_SCANn, b.vid.X_167);
  /*p21.XENA*/   c.vid.OAM_SCANn = not(b.spr.OAM_SCAN);
  /*p21.XANO*/   c.vid.X_167 = not(b.vid.X_167n);
  /*p21.XUGU*/     c.vid.X_167n = nand(b.vid.X0, b.vid.X1, b.vid.X2, b.vid.X5, b.vid.X7); // 128 + 32 + 4 + 2 + 1 = 167


  /*p21.PARU*/ c.vid.INT_VBL     = not(!b.vid.LINE_144_SYNC);
  /*p21.TOLU*/ c.vid.INT_VBLn    = not(b.vid.INT_VBL);
  /*p21.VYPU*/ c.vid.INT_VBL_BUF = not(b.vid.INT_VBLn);


  /*p21.TAPA*/ c.vid.INT_OAM = and(b.vid.INT_VBLn, b.vid.LINE_DONEo);
  /*p21.SELA*/   c.vid.LINE_DONEo    = not(b.vid.LINE_DONEa);
  /*p21.PURE*/     c.vid.LINE_DONEa    = not(b.vid.LINE_DONEn);

  /*p21.TARU*/ c.vid.INT_HBL = and(b.vid.INT_VBLn, b.vid.RENDER_DONEn);

  //---

  /*p21.XYMU*/ c.vid.RENDERING = or(b.vid.WEGO, b.spr.SCAN_DONE_TRIG);
  /*p21.WEGO*/   c.vid.WEGO = or(b.sys.VID_RESET3, b.vid.VOGA);
  /*p21.VOGA*/     c.vid.VOGA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.vid.TADY, b.vid.VOGA, b.vid.RENDER_DONEn);

  /*p21.XAJO*/ c.vid.X_009 = and(b.vid.X0, b.vid.X3);


  //---




  /*p21.SUKO*/ c.vid.INT_STATb = amux4(b.vid.INT_LYC_EN, b.vid.INT_LYC,
                                       b.vid.INT_OAM_EN, b.vid.INT_OAM,
                                       b.vid.INT_VBL_EN, b.vid.INT_VBL,
                                       b.vid.INT_HBL_EN, b.vid.INT_HBL);

  /*p21.TUVA*/ c.vid.INT_STATn = not(b.vid.INT_STATb);
  /*p21.VOTY*/ c.vid.INT_STAT  = not(b.vid.INT_STATn);


  //----------
  // FF41 STAT stuff

  /*p21.TOBE*/ c.vid.FF41_RDa = and(b.sys.CPU_RD2, b.vid.FF41);
  /*p21.VAVE*/ c.vid.FF41_RDb = b.vid.FF41_RDa; // buffer, not inverter

                                                // 00: hblank   - rendering 0, vbl 0, oam 0
                                                // 01: vblank   - rendering 0, vbl 1, oam 0
                                                // 10: oam scan - rendering 0, vbl 0, oam 1
                                                // 11: render   - rendering 1, vbl 0, oam 0

                                                // so one of these has the wrong polarity



  /*p21.SADU*/ c.vid.STAT_MODE0n = nor(b.vid.RENDERING, b.vid.INT_VBL);
  /*p21.XATY*/ c.vid.STAT_MODE1n = nor(b.spr.OAM_ADDR_PARSEn, b.vid.RENDERING);

  /*p21.SEPA*/ c.vid.FF41_WR  = and(b.sys.CPU_WR2, b.vid.FF41);
  /*p21.RYJU*/ c.vid.FF41_WRn = not(b.vid.FF41_WR);
  /*p21.RYVE*/ c.vid.FF41_WRo = not(b.vid.FF41_WR);

  /*p21.ROXE*/ c.vid.INT_HBL_EN = tock_pos(a.vid.FF41_WRo, b.vid.FF41_WRo, b.sys.SYS_RESETn6, b.vid.INT_HBL_EN, b.D3);
  /*p21.RUFO*/ c.vid.INT_VBL_EN = tock_pos(a.vid.FF41_WRo, b.vid.FF41_WRo, b.sys.SYS_RESETn6, b.vid.INT_VBL_EN, b.D4);
  /*p21.REFE*/ c.vid.INT_OAM_EN = tock_pos(a.vid.FF41_WRo, b.vid.FF41_WRo, b.sys.SYS_RESETn6, b.vid.INT_OAM_EN, b.D5);
  /*p21.RUGU*/ c.vid.INT_LYC_EN = tock_pos(a.vid.FF41_WRo, b.vid.FF41_WRo, b.sys.SYS_RESETn6, b.vid.INT_LYC_EN, b.D6);

  // weird
  /*p21.PAGO*/ c.vid.STAT_LYC_MATCH1 = nor(b.sys.SYS_RESETn6, b.vid.FF41_WRn);  // schematic wrong, this is NOR
  /*p21.RUPO*/ c.vid.STAT_LYC_MATCH2 = or(b.vid.INT_LYC, b.vid.STAT_LYC_MATCH1); // this is another of the weird or gates. could be nor?

  /*p21.TEBY*/ if (b.vid.FF41_RDa) c.D0 = not(b.vid.STAT_MODE0n);
  /*p21.WUGA*/ if (b.vid.FF41_RDa) c.D1 = not(b.vid.STAT_MODE1n);
  /*p21.SEGO*/ if (b.vid.FF41_RDa) c.D2 = not(b.vid.STAT_LYC_MATCH2);
  /*p21.PUZO*/ if (b.vid.FF41_RDb) c.D3 = b.vid.INT_HBL_EN;
  /*p21.POFO*/ if (b.vid.FF41_RDb) c.D4 = b.vid.INT_VBL_EN;
  /*p21.SASY*/ if (b.vid.FF41_RDb) c.D5 = b.vid.INT_OAM_EN;
  /*p21.POTE*/ if (b.vid.FF41_RDb) c.D6 = b.vid.INT_LYC_EN;

  //----------
  // y counter

  // 128+16+1+2
  /*p21.NOKO*/ c.vid.LINE_147 = and(b.vid.V7, b.vid.V4, b.vid.V0, b.vid.V1);
  /*p21.MYTA*/ c.vid.LINE_147_SYNC = tock_pos(a.vid.LINE_DONE_DELAYn, b.vid.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.vid.LINE_147_SYNC, b.vid.LINE_147);
  /*p21.LAMA*/ c.vid.Y_RSTn = nor(b.vid.LINE_147_SYNC, b.sys.VID_RESET2);

  /*p21.MUWY*/ c.vid.V0 = tock_pos(a.vid.LINE_DONEn, b.vid.LINE_DONEn, b.vid.Y_RSTn, c.vid.V0, !c.vid.V0);
  /*p21.MYRO*/ c.vid.V1 = tock_pos(!a.vid.V0,        !b.vid.V0,        b.vid.Y_RSTn, c.vid.V1, !c.vid.V1);
  /*p21.LEXA*/ c.vid.V2 = tock_pos(!a.vid.V1,        !b.vid.V1,        b.vid.Y_RSTn, c.vid.V2, !c.vid.V2);
  /*p21.LYDO*/ c.vid.V3 = tock_pos(!a.vid.V2,        !b.vid.V2,        b.vid.Y_RSTn, c.vid.V3, !c.vid.V3);
  /*p21.LOVU*/ c.vid.V4 = tock_pos(!a.vid.V3,        !b.vid.V3,        b.vid.Y_RSTn, c.vid.V4, !c.vid.V4);
  /*p21.LEMA*/ c.vid.V5 = tock_pos(!a.vid.V4,        !b.vid.V4,        b.vid.Y_RSTn, c.vid.V5, !c.vid.V5);
  /*p21.MATO*/ c.vid.V6 = tock_pos(!a.vid.V5,        !b.vid.V5,        b.vid.Y_RSTn, c.vid.V6, !c.vid.V6);
  /*p21.LAFO*/ c.vid.V7 = tock_pos(!a.vid.V6,        !b.vid.V6,        b.vid.Y_RSTn, c.vid.V7, !c.vid.V7);

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

  /*p22.WORU*/ c.vid.FF40n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01n, b.vid.A02n, b.vid.A03n);
  /*p22.WOFA*/ c.vid.FF41n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01n, b.vid.A02n, b.vid.A03n);
  /*p22.WEBU*/ c.vid.FF42n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01,  b.vid.A02n, b.vid.A03n);
  /*p22.WAVU*/ c.vid.FF43n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01,  b.vid.A02n, b.vid.A03n);
  /*p22.WYLE*/ c.vid.FF44n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01n, b.vid.A02,  b.vid.A03n);
  /*p22.WETY*/ c.vid.FF45n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01n, b.vid.A02,  b.vid.A03n);
  /*p22.WATE*/ c.vid.FF46n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01,  b.vid.A02,  b.vid.A03n);
  /*p22.WYBO*/ c.vid.FF47n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01,  b.vid.A02,  b.vid.A03n);
  /*p22.WETA*/ c.vid.FF48n = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01n, b.vid.A02n, b.vid.A03);
  /*p22.VAMA*/ c.vid.FF49n = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01n, b.vid.A02n, b.vid.A03);
  /*p22.WYVO*/ c.vid.FF4An = nand(b.vid.FF4X, b.vid.A00n, b.vid.A01,  b.vid.A02n, b.vid.A03);
  /*p22.WAGE*/ c.vid.FF4Bn = nand(b.vid.FF4X, b.vid.A00,  b.vid.A01,  b.vid.A02n, b.vid.A03);

  /*p22.VOCA*/ c.vid.FF40 = not(b.vid.FF40n);
  /*p22.VARY*/ c.vid.FF41 = not(b.vid.FF41n);
  /*p22.XARO*/ c.vid.FF42 = not(b.vid.FF42n);
  /*p22.XAVY*/ c.vid.FF43 = not(b.vid.FF43n);
  /*p22.XOGY*/ c.vid.FF44 = not(b.vid.FF44n);
  /*p22.XAYU*/ c.vid.FF45 = not(b.vid.FF45n);
  /*p22.XEDA*/ c.vid.FF46 = not(b.vid.FF46n);
  /*p22.WERA*/ c.vid.FF47 = not(b.vid.FF47n);
  /*p22.XAYO*/ c.vid.FF48 = not(b.vid.FF48n);
  /*p22.TEGO*/ c.vid.FF49 = not(b.vid.FF49n);
  /*p22.VYGA*/ c.vid.FF4A = not(b.vid.FF4An);
  /*p22.VUMY*/ c.vid.FF4B = not(b.vid.FF4Bn);

  //----------
  // FF40 LCDC
  /*p23.VYRE*/ c.vid.FF40_RD = and(b.vid.FF40, b.sys.CPU_RD2);
  /*p23.WARU*/ c.vid.FF40_WR = and(b.vid.FF40, b.sys.CPU_WR2);
  /*p23.WYCE*/ c.vid.FF40_RDn = not(b.vid.FF40_RD);
  /*p23.XUBO*/ c.vid.FF40_WRn = not(b.vid.FF40_WR);

  /*p23.VYXE*/ c.vid.LCDC_BGEN    = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.LCDC_BGEN,   b.D0);
  /*p23.XYLO*/ c.vid.LCDC_SPEN    = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.LCDC_SPEN,   b.D1);
  /*p23.XYMO*/ c.vid.LCDC_SPSIZE  = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.LCDC_SPSIZE, b.D2);
  /*p23.XAFO*/ c.vid.LCDC_BGMAP   = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.LCDC_BGMAP,  b.D3);
  /*p23.WEXU*/ c.vid.BG_TILE_SEL  = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.BG_TILE_SEL, b.D4);
  /*p23.WYMO*/ c.vid.LCDC_WINEN   = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.LCDC_WINEN,  b.D5);
  /*p23.WOKY*/ c.vid.WIN_MAP_SEL  = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.WIN_MAP_SEL, b.D6);
  /*p23.XONA*/ c.vid.LCDC_EN      = tock_pos(a.vid.FF40_WRn, b.vid.FF40_WRn, b.sys.SYS_RESETn7, b.vid.LCDC_EN,     b.D7);

  /*p23.WYPO*/ if (b.vid.FF40_RDn) c.D0 = b.vid.LCDC_BGEN;
  /*p23.XERO*/ if (b.vid.FF40_RDn) c.D1 = b.vid.LCDC_SPEN;
  /*p23.WYJU*/ if (b.vid.FF40_RDn) c.D2 = b.vid.LCDC_SPSIZE;
  /*p23.WUKA*/ if (b.vid.FF40_RDn) c.D3 = b.vid.LCDC_BGMAP;
  /*p23.VOKE*/ if (b.vid.FF40_RDn) c.D4 = b.vid.BG_TILE_SEL;
  /*p23.VATO*/ if (b.vid.FF40_RDn) c.D5 = b.vid.LCDC_WINEN;
  /*p23.VAHA*/ if (b.vid.FF40_RDn) c.D6 = b.vid.WIN_MAP_SEL;
  /*p23.XEBU*/ if (b.vid.FF40_RDn) c.D7 = b.vid.LCDC_EN;

  //----------
  // FF4B WX
  /*p23.WYZE*/ c.vid.WYZE = and(b.sys.CPU_RD2, b.vid.FF4B);
  /*p23.WUZA*/ c.vid.WUZA = and(b.sys.CPU_WR2, b.vid.FF4B);
  /*p23.VYCU*/ c.vid.VYCU = not(b.vid.WYZE);
  /*p23.VOXU*/ c.vid.VOXU = not(b.vid.WUZA);

  /*p23.MYPA*/ c.vid.WX0 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX0, b.D0);
  /*p23.NOFE*/ c.vid.WX1 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX1, b.D1);
  /*p23.NOKE*/ c.vid.WX2 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX2, b.D2);
  /*p23.MEBY*/ c.vid.WX3 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX3, b.D3);
  /*p23.MYPU*/ c.vid.WX4 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX4, b.D4);
  /*p23.MYCE*/ c.vid.WX5 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX5, b.D5); 
  /*p23.MUVO*/ c.vid.WX6 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX6, b.D6);
  /*p23.NUKU*/ c.vid.WX7 = tock_pos(a.vid.VOXU, b.vid.VOXU, b.sys.SYS_RESETn5, a.vid.WX7, b.D7);

  /*p23.LOVA*/ if (b.vid.VYCU) c.D0 = b.vid.WX0;
  /*p23.MUKA*/ if (b.vid.VYCU) c.D1 = b.vid.WX1;
  /*p23.MOKO*/ if (b.vid.VYCU) c.D2 = b.vid.WX2;
  /*p23.LOLE*/ if (b.vid.VYCU) c.D3 = b.vid.WX3;
  /*p23.MELE*/ if (b.vid.VYCU) c.D4 = b.vid.WX4;
  /*p23.MUFE*/ if (b.vid.VYCU) c.D5 = b.vid.WX5;
  /*p23.MULY*/ if (b.vid.VYCU) c.D6 = b.vid.WX6;
  /*p23.MARA*/ if (b.vid.VYCU) c.D7 = b.vid.WX7;

  //----------
  // FF4A WY
  /*p23.WAXU*/ c.vid.WAXU = and(b.sys.CPU_RD2, b.vid.FF4A);
  /*p23.WEKO*/ c.vid.WEKO = and(b.sys.CPU_WR2, b.vid.FF4A);
  /*p23.VEFU*/ c.vid.VEFU = not(b.vid.WEKO);
  /*p23.VOMY*/ c.vid.VOMY = not(b.vid.WAXU);

  /*p23.NESO*/ c.vid.WY0 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY0, b.D0);
  /*p23.NYRO*/ c.vid.WY1 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY1, b.D1);
  /*p23.NAGA*/ c.vid.WY2 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY2, b.D2);
  /*p23.MELA*/ c.vid.WY3 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY3, b.D3);
  /*p23.NULO*/ c.vid.WY4 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY4, b.D4);
  /*p23.NENE*/ c.vid.WY5 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY5, b.D5);
  /*p23.NUKA*/ c.vid.WY6 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY6, b.D6);
  /*p23.NAFU*/ c.vid.WY7 = tock_pos(a.vid.VEFU, b.vid.VEFU, b.sys.SYS_RESETn5, b.vid.WY7, b.D7);

  /*p23.PUNU*/ if (b.vid.VOMY) c.D0 = b.vid.WY0;
  /*p23.PODA*/ if (b.vid.VOMY) c.D1 = b.vid.WY1;
  /*p23.PYGU*/ if (b.vid.VOMY) c.D2 = b.vid.WY2;
  /*p23.LOKA*/ if (b.vid.VOMY) c.D3 = b.vid.WY3;
  /*p23.MEGA*/ if (b.vid.VOMY) c.D4 = b.vid.WY4;
  /*p23.PELA*/ if (b.vid.VOMY) c.D5 = b.vid.WY5;
  /*p23.POLO*/ if (b.vid.VOMY) c.D6 = b.vid.WY6;
  /*p23.MERA*/ if (b.vid.VOMY) c.D7 = b.vid.WY7;

  //----------
  // FF43 SCX

  /*p23.AVOG*/ c.vid.AVOG = and(b.vid.FF43, b.sys.CPU_RD2);
  /*p23.ARUR*/ c.vid.ARUR = and(b.vid.FF43, b.sys.CPU_WR2);
  /*p23.AMUN*/ c.vid.AMUN = not(b.vid.ARUR);
  /*p23.BEBA*/ c.vid.BEBA = not(b.vid.AVOG);

  /*p23.DATY*/ c.vid.SCX0 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX0, b.D0);
  /*p23.DUZU*/ c.vid.SCX1 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX1, b.D1);
  /*p23.CYXU*/ c.vid.SCX2 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX2, b.D2);
  /*p23.GUBO*/ c.vid.SCX3 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX3, b.D3);
  /*p23.BEMY*/ c.vid.SCX4 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX4, b.D4);
  /*p23.CUZY*/ c.vid.SCX5 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX5, b.D5);
  /*p23.CABU*/ c.vid.SCX6 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX6, b.D6);
  /*p23.BAKE*/ c.vid.SCX7 = tock_pos(a.vid.AMUN, b.vid.AMUN, b.sys.SYS_RESETn2, a.vid.SCX7, b.D7);

  /*p23.EDOS*/ if (b.vid.BEBA) c.D0 = b.vid.SCX0;
  /*p23.EKOB*/ if (b.vid.BEBA) c.D1 = b.vid.SCX1;
  /*p23.CUGA*/ if (b.vid.BEBA) c.D2 = b.vid.SCX2;
  /*p23.WONY*/ if (b.vid.BEBA) c.D3 = b.vid.SCX3;
  /*p23.CEDU*/ if (b.vid.BEBA) c.D4 = b.vid.SCX4;
  /*p23.CATA*/ if (b.vid.BEBA) c.D5 = b.vid.SCX5;
  /*p23.DOXE*/ if (b.vid.BEBA) c.D6 = b.vid.SCX6;
  /*p23.CASY*/ if (b.vid.BEBA) c.D7 = b.vid.SCX7;

  //----------
  // FF42 SCY

  /*p23.ANYP*/ c.vid.ANYP = and(b.sys.CPU_RD2, b.vid.FF42);
  /*p23.BEDY*/ c.vid.BEDY = and(b.sys.CPU_WR2, b.vid.FF42);
  /*p23.CAVO*/ c.vid.CAVO = not(b.vid.BEDY);
  /*p23.BUWY*/ c.vid.BUWY = not(b.vid.ANYP);

  /*p23.GAVE*/ c.vid.SCY0 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY0, b.D0);
  /*p23.FYMO*/ c.vid.SCY1 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY1, b.D1);
  /*p23.FEZU*/ c.vid.SCY2 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY2, b.D2);
  /*p23.FUJO*/ c.vid.SCY3 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY3, b.D3);
  /*p23.DEDE*/ c.vid.SCY4 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY4, b.D4);
  /*p23.FOTY*/ c.vid.SCY5 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY5, b.D5);
  /*p23.FOHA*/ c.vid.SCY6 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY6, b.D6);
  /*p23.FUNY*/ c.vid.SCY7 = tock_pos(a.vid.CAVO, b.vid.CAVO, b.sys.SYS_RESETn2, b.vid.SCY7, b.D7);

  /*p23.WARE*/ if (b.vid.BUWY) c.D0 = b.vid.SCY0;
  /*p23.GOBA*/ if (b.vid.BUWY) c.D1 = b.vid.SCY1;
  /*p23.GONU*/ if (b.vid.BUWY) c.D2 = b.vid.SCY2;
  /*p23.GODO*/ if (b.vid.BUWY) c.D3 = b.vid.SCY3;
  /*p23.CUSA*/ if (b.vid.BUWY) c.D4 = b.vid.SCY4;
  /*p23.GYZO*/ if (b.vid.BUWY) c.D5 = b.vid.SCY5;
  /*p23.GUNE*/ if (b.vid.BUWY) c.D6 = b.vid.SCY6;
  /*p23.GYZA*/ if (b.vid.BUWY) c.D7 = b.vid.SCY7;

  //----------
  // FF45 LYC

  /*p23.XYLY*/ c.vid.XYLY = and(b.sys.CPU_RD2, b.vid.FF45);
  /*p23.XUFA*/ c.vid.XUFA = and(b.sys.CPU_WR2, b.vid.FF45);
  /*p23.WANE*/ c.vid.WANE = not(b.vid.XUFA);
  /*p23.WEKU*/ c.vid.WEKU = not(b.vid.XYLY);

  /*p23.SYRY*/ c.vid.LYC0 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC0, b.D0);
  /*p23.VUCE*/ c.vid.LYC1 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC1, b.D1);
  /*p23.SEDY*/ c.vid.LYC2 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC2, b.D2);
  /*p23.SALO*/ c.vid.LYC3 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC3, b.D3);
  /*p23.SOTA*/ c.vid.LYC4 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC4, b.D4);
  /*p23.VAFA*/ c.vid.LYC5 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC5, b.D5);
  /*p23.VEVO*/ c.vid.LYC6 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC6, b.D6);
  /*p23.RAHA*/ c.vid.LYC7 = tock_pos(a.vid.WANE, b.vid.WANE, b.sys.SYS_RESETn6, a.vid.LYC7, b.D7);

  /*p23.RETU*/ if (b.vid.WEKU) c.D0 = b.vid.LYC0;
  /*p23.VOJO*/ if (b.vid.WEKU) c.D1 = b.vid.LYC1;
  /*p23.RAZU*/ if (b.vid.WEKU) c.D2 = b.vid.LYC2;
  /*p23.REDY*/ if (b.vid.WEKU) c.D3 = b.vid.LYC3;
  /*p23.RACE*/ if (b.vid.WEKU) c.D4 = b.vid.LYC4;
  /*p23.VAZU*/ if (b.vid.WEKU) c.D5 = b.vid.LYC5;
  /*p23.VAFE*/ if (b.vid.WEKU) c.D6 = b.vid.LYC6;
  /*p23.PUFY*/ if (b.vid.WEKU) c.D7 = b.vid.LYC7;

  //----------
  // FF44 LY

  /*p23.WAFU*/ c.vid.FF44_RD = and(b.sys.CPU_RD2, b.vid.FF44);
  /*p23.VARO*/ c.vid.FF44_RDn = not(b.vid.FF44_RD);

  /*p23.WURY*/ c.vid.LY0n = not(b.vid.V0);
  /*p23.XEPO*/ c.vid.LY1n = not(b.vid.V1);
  /*p23.MYFA*/ c.vid.LY2n = not(b.vid.V2);
  /*p23.XUHY*/ c.vid.LY3n = not(b.vid.V3);
  /*p23.WATA*/ c.vid.LY4n = not(b.vid.V4);
  /*p23.XAGA*/ c.vid.LY5n = not(b.vid.V5);
  /*p23.XUCE*/ c.vid.LY6n = not(b.vid.V6);
  /*p23.XOWO*/ c.vid.LY7n = not(b.vid.V7);

  /*p23.VEGA*/ if (b.vid.FF44_RDn) c.D0 = not(b.vid.LY0n);
  /*p23.WUVA*/ if (b.vid.FF44_RDn) c.D1 = not(b.vid.LY1n);
  /*p23.LYCO*/ if (b.vid.FF44_RDn) c.D2 = not(b.vid.LY2n);
  /*p23.WOJY*/ if (b.vid.FF44_RDn) c.D3 = not(b.vid.LY3n);
  /*p23.VYNE*/ if (b.vid.FF44_RDn) c.D4 = not(b.vid.LY4n);
  /*p23.WAMA*/ if (b.vid.FF44_RDn) c.D5 = not(b.vid.LY5n);
  /*p23.WAVO*/ if (b.vid.FF44_RDn) c.D6 = not(b.vid.LY6n);
  /*p23.WEZE*/ if (b.vid.FF44_RDn) c.D7 = not(b.vid.LY7n);

  //----------
  // Main PPU clock, needs untangling

  /*p24.LOBY*/ c.vid.RENDERINGn = not(b.vid.RENDERING);

  /*p24.SACU*/ c.vid.CLKPIPE = nor(b.vid.SEGU_4M, b.vid.FINE_MATCH_TRIG_OR_NOT_RENDERING);


  /*p24.SEGU*/ c.vid.SEGU_4M = not(b.vid.TYFA);
  /*p24.TYFA*/   c.vid.TYFA = and(b.vid.SOCY, b.vid.POKY, b.vid.VYBO);
  /*p24.SOCY*/     c.vid.SOCY = not(b.vid.TOMU);
  /*p24.TOMU*/       c.vid.TOMU = not(b.vid.SYLO);
  /*p24.VYBO*/     c.vid.VYBO = nor(b.spr.OAM_SCAN, b.vid.RENDER_DONEn, b.vid.CLK_xBxDxFxHc);



  /*p24.POKY*/ c.vid.POKY = or(b.vid.PYGO, b.vid.RENDERINGn);
  /*p24.PYGO*/   c.vid.PYGO = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.vid.RENDERING, b.vid.PYGO, b.vid.PORY);
  /*p24.PORY*/     c.vid.PORY = tock_pos(a.vid.CLK_xBxDxFxHc, b.vid.CLK_xBxDxFxHc, b.vid.NAFY, b.vid.PORY, b.vid.NYKA);
  /*p24.NAFY*/       c.vid.NAFY = nor(b.vid.MOSU, b.vid.RENDERINGn);
  /*p24.NYKA*/       c.vid.NYKA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.vid.NAFY, b.vid.NYKA, b.vid.BG_SEQ_5);

  /*p27.ROXY*/ c.vid.FINE_MATCH_TRIG_OR_NOT_RENDERING = or(/*p27.PAHA*/ not(b.vid.RENDERING), b.vid.FINE_MATCH_TRIG);

  //----------
  // Control

  {
    /*p24.KEDY*/ wire LCDC_ENn = not(b.vid.LCDC_EN);

    /*p21.POGU*/ c.chip_out.CPG = not(b.vid.RYNO);
    /*p21.RYNO*/   c.vid.RYNO = or(b.vid.SYGU, b.vid.LINE_DONEn);
    /*p21.SYGU*/     c.vid.SYGU = tock_pos(a.vid.SONO_CLK, b.vid.SONO_CLK, b.sys.VID_RESETn2, b.vid.SYGU, b.vid.TEGY);
    /*p21.TEGY*/       c.vid.TEGY = nand(b.vid.CNT_000n, b.vid.CNT_007n, b.vid.CNT_045n, b.vid.CNT_083n);
    /*p21.VOKU*/         c.vid.CNT_000n = nand(b.vid.CNT_6n, b.vid.CNT_5n, b.vid.CNT_4n, b.vid.CNT_3n, b.vid.CNT_2n, b.vid.CNT_1n, b.vid.CNT_0n); // 0000000 == 0
    /*p21.TOZU*/         c.vid.CNT_007n = nand(b.vid.CNT_6n, b.vid.CNT_5n, b.vid.CNT_4n, b.vid.CNT_3n, b.vid.CNT_2,  b.vid.CNT_1,  b.vid.CNT_0); // 0000111 == 7
    /*p21.TECE*/         c.vid.CNT_045n = nand(b.vid.CNT_6n, b.vid.CNT_5,  b.vid.CNT_4n, b.vid.CNT_3,  b.vid.CNT_2,  b.vid.CNT_1n, b.vid.CNT_0); // 0101101 == 45
    /*p21.TEBO*/         c.vid.CNT_083n = nand(b.vid.CNT_6,  b.vid.CNT_4n, b.vid.CNT_4,  b.vid.CNT_6n, b.vid.CNT_2n, b.vid.CNT_1,  b.vid.CNT_0); // 1010011 == 83

    // Clock
    /*p21.RYPO*/ c.chip_out.CP = not(b.vid.CPn);
    /*p21.SEMU*/   c.vid.CPn = or(b.vid.TOBA, b.vid.FINE_MATCH_TRIG);
    /*p21.TOBA*/     c.vid.TOBA = and(b.vid.CLKPIPE, b.vid.WUSA);
    /*p21.WUSA*/       c.vid.WUSA = or(b.vid.X_009, b.vid.WEGO);

    // Horizontal sync
    /*p24.RUZE*/ c.chip_out.ST = not(b.vid.POFY);
    /*p24.POFY*/   c.vid.POFY = not(b.vid.RUJU);
    /*p24.RUJU*/     c.vid.RUJU = or(b.vid.PAHO, b.sys.VID_RESET3, b.vid.POME);
    /*p24.PAHO*/        c.vid.PAHO = tock_pos(a.vid.FINE_MATCH_CLK, b.vid.FINE_MATCH_CLK, b.vid.RENDERING, b.vid.PAHO, b.vid.X3);
    /*p24.POME*/        c.vid.POME = nor(b.spr.SCAN_DONE_TRIG, b.vid.POFY);


    // Data latch
    /*p24.KYMO*/ c.chip_out.CPL = not(b.vid.CPLn);
    /*p24.KAHE*/   c.vid.CPLn = amux2(b.vid.LCDC_EN, b.vid.KASA, LCDC_ENn, b.vid.UMOB);
    /*p24.KASA*/     c.vid.KASA = not(b.vid.LINE_DONEa);
    /*p24.UMOB*/     c.vid.UMOB = not(b.sys.DIV_06n);

    // Alt signal?
    /*p24.KOFO*/ c.chip_out.FR  = not(b.vid.FRn);
    /*p24.KUPA*/   c.vid.FRn  = amux2(b.vid.LCDC_EN, b.vid.KEBO, LCDC_ENn, b.vid.USEC);
    /*p24.KEBO*/     c.vid.KEBO = not(b.vid.MECO);
    /*p24.MECO*/       c.vid.MECO = not(b.vid.MAGU);
    /*p24.MAGU*/         c.vid.MAGU = xor(b.vid.NAPO, b.vid.LUCA);
    /*p21.NAPO*/           c.vid.NAPO = tock_pos(a.vid.LINE_144_SYNC, b.vid.LINE_144_SYNC, b.sys.VID_RESETn2, b.vid.NAPO, !b.vid.NAPO);
    /*p24.LUCA*/           c.vid.LUCA = tock_pos(a.vid.LINE_DONE,     b.vid.LINE_DONE,     b.sys.VID_RESETn2, b.vid.LUCA, !b.vid.LUCA);
    /*p24.LOFU*/             c.vid.LINE_DONE = not(b.vid.LINE_DONEn);
    /*p24.USEC*/     c.vid.USEC = not(b.sys.DIV_07n);

    // Vertical sync
    /*p24.MURE*/ c.chip_out.S = not(b.vid.LINE_000_SYNCn);
    /*p24.MEDA*/   c.vid.LINE_000_SYNCn = tock_pos(a.vid.LINE_DONE_DELAYn, b.vid.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.vid.LINE_000_SYNCn, b.vid.LINE_000n);
    /*p24.NERU*/     c.vid.LINE_000n = nor(b.vid.V0, b.vid.V1, b.vid.V2, b.vid.V3, b.vid.V4, b.vid.V5, b.vid.V6, b.vid.V7);

    // Unused
    /*p24.LEBE*/ c.vid.LEBE = tock_pos(!a.vid.LUCA, !b.vid.LUCA, b.sys.VID_RESETn2, b.vid.LEBE, !b.vid.LEBE);
  }

  //----------

  // guess
  /*p25.TUCA*/ c.vid.CPU_VRAM_RD  = and (b.vid.ADDR_VRAM, b.sys.CPU_RD_SYNC);
  /*p25.TUJA*/ c.vid.CPU_VRAM_WR  = and (b.vid.ADDR_VRAM, b.sys.CPU_WR_SYNC);
  /*p25.TEGU*/ c.vid.CPU_VRAM_CLK = nand(b.vid.ADDR_VRAM, b.sys.CLK_xxxDxxxx1);

  /*p25.TEFY*/ c.vid.MCS_Cn = not(chip_in.MCS_C);
  /*p25.SUDO*/ c.vid.MWR_Cn = not(chip_in.MWR_C);
  /*p25.TAVY*/ c.vid.MOE_Cn = not(chip_in.MOE_C);

  /*p25.TOLE*/ c.vid.CPU_VRAM_RD2  = mux2(b.vid.MCS_Cn, b.vid.CPU_VRAM_RD , b.vid.DBG_TUTO);
  /*p25.TYJY*/ c.vid.CPU_VRAM_WR2  = mux2(b.vid.MWR_Cn, b.vid.CPU_VRAM_WR , b.vid.DBG_TUTO);
  /*p25.SALE*/ c.vid.CPU_VRAM_CLK2 = mux2(b.vid.MOE_Cn, b.vid.CPU_VRAM_CLK, b.vid.DBG_TUTO);

  /*p25.RUVY*/ c.vid.CPU_VRAM_CLK2n = not(b.vid.CPU_VRAM_CLK2);

  /*p25.SERE*/ c.vid.SERE = and(b.vid.CPU_VRAM_RD2,   b.vid.RENDERINGo);
  /*p25.ROPY*/   c.vid.RENDERINGo = not(b.vid.RENDERING);

  /*p25.SAZO*/ c.vid.MD_OUTd = and(b.vid.CPU_VRAM_CLK2n, b.vid.SERE);
  /*p25.RYJE*/ c.vid.MD_INb  = not(b.vid.MD_OUTd);
  /*p25.REVO*/ c.vid.MD_OUTc = not(b.vid.MD_INb);
  /*p25.ROCY*/ c.vid.MD_OUTe = and(b.vid.MD_OUTd, b.vid.MD_OUTc);
  /*p25.RAHU*/ c.vid.D_TO_MD = not(b.vid.MD_OUTe);

  /*p25.RELA*/ c.vid.MD_OUTb = or(b.vid.MD_OUTc, b.vid.MD_OUTd);
  /*p25.RENA*/ c.vid.MD_IN   = not(b.vid.MD_OUTb);

  /*p25.ROFA*/ c.chip_out.MD0_B = not(b.vid.MD_IN);
  /*p25.ROFA*/ c.chip_out.MD1_B = not(b.vid.MD_IN);
  /*p25.ROFA*/ c.chip_out.MD2_B = not(b.vid.MD_IN);
  /*p25.ROFA*/ c.chip_out.MD3_B = not(b.vid.MD_IN);
  /*p25.ROFA*/ c.chip_out.MD4_B = not(b.vid.MD_IN);
  /*p25.ROFA*/ c.chip_out.MD5_B = not(b.vid.MD_IN);
  /*p25.ROFA*/ c.chip_out.MD6_B = not(b.vid.MD_IN);
  /*p25.ROFA*/ c.chip_out.MD7_B = not(b.vid.MD_IN);





  /*p25.SOKY*/ c.chip_out.MCS_A = not(b.vid.MCS_An);
  /*p25.SETY*/ c.chip_out.MCS_D = not(b.vid.MCS_Dn);
  /*p25.TODE*/   c.vid.MCS_An = and(b.vid.MCS, b.vid.DBG_TUTOn);
  /*p25.SEWO*/   c.vid.MCS_Dn = or(b.vid.MCS, b.vid.DBG_TUTO);
  /*p25.SUTU*/     c.vid.MCS = nor(b.vid.LONYb, b.sys.VRAM_TO_OAMn, b.spr.SPRITE_READn, b.vid.SERE);

  /*p25.SYSY*/ c.chip_out.MWR_A = not(b.vid.MWR_An);
  /*p25.RAGU*/ c.chip_out.MWR_D = not(b.vid.MWR_Dn);
  /*p25.TAXY*/   c.vid.MWR_An = and(b.vid.MWR, b.vid.DBG_TUTOn);
  /*p25.SOFY*/   c.vid.MWR_Dn = or(b.vid.MWR, b.vid.DBG_TUTO);
  /*p25.SOHY*/     c.vid.MWR = nand(b.vid.CPU_VRAM_WR2, b.vid.SERE);

  /*p25.REFO*/ c.chip_out.MOE_A = not(b.vid.MOE_An);
  /*p25.SAHA*/ c.chip_out.MOE_D = not(b.vid.MOE_Dn);
  /*p25.SEMA*/   c.vid.MOE_An    = and(b.vid.MOE, b.vid.DBG_TUTOn);
  /*p25.RUTE*/   c.vid.MOE_Dn    = or(b.vid.MOE, b.vid.DBG_TUTO); // schematic wrong, second input is RACU
  /*p25.RACU*/     c.vid.MOE = and(b.vid.RYLU, b.vid.RAWA, b.vid.MYMA, b.vid.VRAM_TO_OAM);
  /*p25.RYLU*/       c.vid.RYLU = nand(b.vid.CPU_VRAM_CLK2, b.vid.RENDERINGn);
  /*p25.RAWA*/       c.vid.RAWA = not(b.vid.SOHO);
  /*p25.SOHO*/         c.vid.SOHO = and(b.spr.TACU, b.spr.SPRITE_READn);
  /*p27.MYMA*/       c.vid.MYMA = not(b.vid.LONY);
  /*p25.APAM*/       c.vid.VRAM_TO_OAM = not(b.sys.VRAM_TO_OAMn);

  //----------

  /*p25.RODY*/ if (b.vid.MD_IN) c.MD0 = chip_in.MD0_C;
  /*p25.REBA*/ if (b.vid.MD_IN) c.MD1 = chip_in.MD1_C;
  /*p25.RYDO*/ if (b.vid.MD_IN) c.MD2 = chip_in.MD2_C;
  /*p25.REMO*/ if (b.vid.MD_IN) c.MD3 = chip_in.MD3_C;
  /*p25.ROCE*/ if (b.vid.MD_IN) c.MD4 = chip_in.MD4_C;
  /*p25.ROPU*/ if (b.vid.MD_IN) c.MD5 = chip_in.MD5_C;
  /*p25.RETA*/ if (b.vid.MD_IN) c.MD6 = chip_in.MD6_C;
  /*p25.RAKU*/ if (b.vid.MD_IN) c.MD7 = chip_in.MD7_C;

  //----------
  // more debug stuff

  /*p25.SYCY*/ c.vid.MODE_DBG2n = not(b.sys.MODE_DBG2);
  /*p25.SOTO*/ c.vid.DBG_SOTO   = tock_pos(a.vid.MODE_DBG2n, b.vid.MODE_DBG2n, b.sys.SYS_RESETn2, b.vid.DBG_SOTO, !b.vid.DBG_SOTO);
  /*p25.TUTO*/ c.vid.DBG_TUTO   = and(b.sys.MODE_DBG2, !b.vid.DBG_SOTO);
  /*p25.RACO*/ c.vid.DBG_TUTOn  = not(b.vid.DBG_TUTO);

  /*p25.TUSO*/ c.vid.TUSO = nor(b.sys.MODE_DBG2, b.sys.CPU_CLK1n);
  /*p25.SOLE*/ c.vid.SOLE = not(b.vid.TUSO);
  /*p25.????*/ c.vid.P10_Bn = not(chip_in.P10_B);

  /*p25.TOVU*/ if (b.vid.P10_Bn) c.D0 = b.vid.SOLE;
  /*p25.SOSA*/ if (b.vid.P10_Bn) c.D1 = b.vid.SOLE;
  /*p25.SEDU*/ if (b.vid.P10_Bn) c.D2 = b.vid.SOLE;
  /*p25.TAXO*/ if (b.vid.P10_Bn) c.D3 = b.vid.SOLE;
  /*p25.TAHY*/ if (b.vid.P10_Bn) c.D4 = b.vid.SOLE;
  /*p25.TESU*/ if (b.vid.P10_Bn) c.D5 = b.vid.SOLE;
  /*p25.TAZU*/ if (b.vid.P10_Bn) c.D6 = b.vid.SOLE;
  /*p25.TEWA*/ if (b.vid.P10_Bn) c.D7 = b.vid.SOLE;

  //----------

  /*p25.TEME*/ if (b.vid.D_TO_MD) c.MD0 = b.D0; // well this is clearly backwards
  /*p25.TEWU*/ if (b.vid.D_TO_MD) c.MD1 = b.D1;
  /*p25.TYGO*/ if (b.vid.D_TO_MD) c.MD2 = b.D2;
  /*p25.SOTE*/ if (b.vid.D_TO_MD) c.MD3 = b.D3;
  /*p25.SEKE*/ if (b.vid.D_TO_MD) c.MD4 = b.D4;
  /*p25.RUJO*/ if (b.vid.D_TO_MD) c.MD5 = b.D5;
  /*p25.TOFA*/ if (b.vid.D_TO_MD) c.MD6 = b.D6;
  /*p25.SUZA*/ if (b.vid.D_TO_MD) c.MD7 = b.D7;

  /*p25.RURA*/ c.chip_out.MD0_D = not(/*p25.SYNU*/ or(b.vid.D_TO_MD, b.MD0));
  /*p25.RULY*/ c.chip_out.MD1_D = not(/*p25.SYMA*/ or(b.vid.D_TO_MD, b.MD1));
  /*p25.RARE*/ c.chip_out.MD2_D = not(/*p25.ROKO*/ or(b.vid.D_TO_MD, b.MD2));
  /*p25.RODU*/ c.chip_out.MD3_D = not(/*p25.SYBU*/ or(b.vid.D_TO_MD, b.MD3));
  /*p25.RUBE*/ c.chip_out.MD4_D = not(/*p25.SAKO*/ or(b.vid.D_TO_MD, b.MD4));
  /*p25.RUMU*/ c.chip_out.MD5_D = not(/*p25.SEJY*/ or(b.vid.D_TO_MD, b.MD5));
  /*p25.RYTY*/ c.chip_out.MD6_D = not(/*p25.SEDO*/ or(b.vid.D_TO_MD, b.MD6));
  /*p25.RADY*/ c.chip_out.MD7_D = not(/*p25.SAWU*/ or(b.vid.D_TO_MD, b.MD7));

  //----------

  //----------

  /*p25.ROVE*/ c.vid.D_TO_MDn = not(b.vid.D_TO_MD);

  /*p25.REGE*/ c.chip_out.MD0_A = not(/*p25.SEFA*/ and(b.MD0, b.vid.D_TO_MDn));
  /*p25.RYKY*/ c.chip_out.MD1_A = not(/*p25.SOGO*/ and(b.MD1, b.vid.D_TO_MDn));
  /*p25.RAZO*/ c.chip_out.MD2_A = not(/*p25.SEFU*/ and(b.MD2, b.vid.D_TO_MDn));
  /*p25.RADA*/ c.chip_out.MD3_A = not(/*p25.SUNA*/ and(b.MD3, b.vid.D_TO_MDn));
  /*p25.RYRO*/ c.chip_out.MD4_A = not(/*p25.SUMO*/ and(b.MD4, b.vid.D_TO_MDn));
  /*p25.REVU*/ c.chip_out.MD5_A = not(/*p25.SAZU*/ and(b.MD5, b.vid.D_TO_MDn));
  /*p25.REKU*/ c.chip_out.MD6_A = not(/*p25.SAMO*/ and(b.MD6, b.vid.D_TO_MDn));
  /*p25.RYZE*/ c.chip_out.MD7_A = not(/*p25.SUKE*/ and(b.MD7, b.vid.D_TO_MDn));

  //----------

  //----------

  /*p25.TYVY*/ c.vid.MD_TO_Dn = nand(b.vid.SERE, b.spr.CPU_READ_MYSTERY);
  /*p25.SEBY*/ c.vid.MD_TO_D = not(b.vid.MD_TO_Dn);

  /*p25.RUGA*/ if (b.vid.MD_TO_D) c.D0 = /*p25.RERY*/ not(b.MD0);
  /*p25.ROTA*/ if (b.vid.MD_TO_D) c.D1 = /*p25.RUNA*/ not(b.MD1);
  /*p25.RYBU*/ if (b.vid.MD_TO_D) c.D2 = /*p25.RONA*/ not(b.MD2);
  /*p25.RAJU*/ if (b.vid.MD_TO_D) c.D3 = /*p25.RUNO*/ not(b.MD3);
  /*p25.TYJA*/ if (b.vid.MD_TO_D) c.D4 = /*p25.SANA*/ not(b.MD4);
  /*p25.REXU*/ if (b.vid.MD_TO_D) c.D5 = /*p25.RORO*/ not(b.MD5);
  /*p25.RUPY*/ if (b.vid.MD_TO_D) c.D6 = /*p25.RABO*/ not(b.MD6);
  /*p25.TOKU*/ if (b.vid.MD_TO_D) c.D7 = /*p25.SAME*/ not(b.MD7);

  //----------

  /*p27.REPU*/ c.vid.IN_FRAME_Y  = nor(b.vid.INT_VBL, b.sys.VID_RESET4);   // schematic wrong, this is NOR
  /*p27.SYNY*/ c.vid.IN_FRAME_Yn = not(b.vid.IN_FRAME_Y);

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

    // WIN_EN polarity?
    /*p27.PALO*/ wire WY_MATCH_HI  = nand(b.vid.LCDC_WINEN, WY_MATCH4n, WY_MATCH5n, WY_MATCH6n, WY_MATCH7n);
    /*p27.NELE*/ wire WY_MATCH_HIn = not(WY_MATCH_HI);
    /*p27.PAFU*/ wire WY_MATCHn    = nand(WY_MATCH_HIn, WY_MATCH0n, WY_MATCH1n, WY_MATCH2n, WY_MATCH3n);
    /*p27.ROGE*/ wire WY_MATCH     = not(WY_MATCHn);

    // huh, the y matcher runs at 1 mhz but the x matcher runs at 4 mhz
    /*p27.SARY*/ c.vid.WY_MATCH_SYNC = tock_pos(a.vid.CLK_1Mb, b.vid.CLK_1Mb, b.sys.VID_RESETn1, b.vid.WY_MATCH_SYNC, WY_MATCH);

    // polarity or gates wrong
    /*p27.REJO*/ wire WIN_CHECK_X = or(b.vid.WY_MATCH_SYNC, b.vid.IN_FRAME_Y); // another weird or gate. should be AND?

    /*p27.PUKY*/ wire WX_MATCH_HI  = nand(WIN_CHECK_X, WX_MATCH4n, WX_MATCH5n, WX_MATCH6n, WX_MATCH7n);
    /*p27.NUFA*/ wire WX_MATCH_HIn = not (WX_MATCH_HI);
    /*p27.NOGY*/ wire WIN_MATCHn   = nand(WX_MATCH_HIn, WX_MATCH0n, WX_MATCH1n, WX_MATCH2n, WX_MATCH3n);
    /*p27.NUKO*/ c.vid.WIN_MATCH   = not (WIN_MATCHn);
  }

  //----------
  // Fine x matcher for x scroll

  {

    /*p27.ROZE*/ c.vid.FINE_COUNT_STOPn = nand(b.vid.FINE_CNT0, b.vid.FINE_CNT1, b.vid.FINE_CNT2);

    /*p27.SUHA*/ wire FINE_MATCH0 = xor(b.vid.SCX0, b.vid.FINE_CNT0);
    /*p27.SYBY*/ wire FINE_MATCH1 = xor(b.vid.SCX1, b.vid.FINE_CNT1);
    /*p27.SOZU*/ wire FINE_MATCH2 = xor(b.vid.SCX2, b.vid.FINE_CNT2);
    /*p27.RONE*/ wire FINE_MATCH = nand(b.vid.FINE_MATCH_TRIG_OR_NOT_RENDERING, FINE_MATCH0, FINE_MATCH0, FINE_MATCH0);
    /*p27.POHU*/ wire FINE_MATCHn = not(FINE_MATCH);

    /*p24.ROXO*/ c.vid.FINE_MATCH_CLK   = not(b.vid.SEGU_4M);
    /*p27.PUXA*/ c.vid.FINE_MATCH_SYNC1 = tock_pos(a.vid.FINE_MATCH_CLK, b.vid.FINE_MATCH_CLK, b.vid.RENDERING,  b.vid.FINE_MATCH_SYNC1, FINE_MATCHn);
    /*p27.NYZE*/ c.vid.FINE_MATCH_SYNC2 = tock_pos(a.vid.CLK_xBxDxFxHa,  b.vid.CLK_xBxDxFxHa,  b.vid.RENDERING,  b.vid.FINE_MATCH_SYNC2, b.vid.FINE_MATCH_SYNC1);
    /*p27.POVA*/ c.vid.FINE_MATCH_TRIG  = and(b.vid.FINE_MATCH_SYNC1, !b.vid.FINE_MATCH_SYNC2);


    /*p27.PECU*/ c.vid.FINE_CLK = nand(b.vid.FINE_MATCH_CLK, b.vid.FINE_COUNT_STOPn);
    /*p27.PASO*/ wire FINE_RST = nor(b.vid.MAP_X_CLK_STOPn, b.vid.RENDERINGo);
    /*p27.RYKU*/ c.vid.FINE_CNT0 = tock_pos( a.vid.FINE_CLK,   b.vid.FINE_CLK,  FINE_RST, b.vid.FINE_CNT0, !b.vid.FINE_CNT0);
    /*p27.ROGA*/ c.vid.FINE_CNT1 = tock_pos(!a.vid.FINE_CNT0, !b.vid.FINE_CNT0, FINE_RST, b.vid.FINE_CNT1, !b.vid.FINE_CNT1);
    /*p27.RUBU*/ c.vid.FINE_CNT2 = tock_pos(!a.vid.FINE_CNT1, !b.vid.FINE_CNT1, FINE_RST, b.vid.FINE_CNT2, !b.vid.FINE_CNT2);
  
    /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(b.vid.WIN_MATCH, b.vid.FINE_COUNT_STOPn);
    /*p27.RYFA*/ c.vid.WIN_MATCH_ONSCREEN_SYNC1 = tock_pos(a.vid.SEGU_4M, b.vid.SEGU_4M, b.vid.RENDERING, b.vid.WIN_MATCH_ONSCREEN_SYNC1, WIN_MATCH_ONSCREEN);
    /*p27.RENE*/ c.vid.WIN_MATCH_ONSCREEN_SYNC2 = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.vid.RENDERING, b.vid.WIN_MATCH_ONSCREEN_SYNC2, b.vid.WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.SEKO*/ c.vid.WIN_TRIGGER = nor(b.vid.WIN_MATCH_ONSCREEN_SYNC2, !b.vid.WIN_MATCH_ONSCREEN_SYNC1);
  }




  //----------



  // stall for sprite is going to be in here 
  /*p27.TUXY*/ c.vid.TUXY = nand(b.vid.SOVY, b.vid.SYLO);
  /*p27.SOVY*/   c.vid.SOVY = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn1, b.vid.SOVY, b.vid.RYDY);
  /*p27.SYLO*/   c.vid.SYLO = not(b.vid.RYDY);
  /*p27.RYDY*/     c.vid.RYDY = nor(b.vid.PUKU, b.sys.VID_RESET4, b.vid.PORY);
  /*p27.PUKU*/       c.vid.PUKU = nor(b.vid.NUNY, b.vid.RYDY);











  //----------
  // BG/WIN sequencer


  // background _or_ window trigger
  /*p27.PYNU*/ c.vid.TILE_ABb = or(b.vid.WIN_MATCH_SYNC2, b.vid.MAP_X_RST);
  /*p27.NUNU*/   c.vid.WIN_MATCH_SYNC2 = tock_pos(a.vid.CLK_xBxDxFxHb, b.vid.CLK_xBxDxFxHb, b.sys.VID_RESETn1, b.vid.WIN_MATCH_SYNC2, b.vid.WIN_MATCH_SYNC1);
  /*p27.PYCO*/     c.vid.WIN_MATCH_SYNC1 = tock_pos(a.vid.ROCO_4M,       b.vid.ROCO_4M,       b.sys.VID_RESETn1, b.vid.WIN_MATCH_SYNC1, b.vid.WIN_MATCH);
  /*p27.ROCO*/       c.vid.ROCO_4M = not(b.vid.SEGU_4M);
  /*p27.XOFO*/   c.vid.MAP_X_RST = nand(b.vid.LCDC_WINEN, b.vid.NEW_LINEn, b.sys.VID_RESETn1);
  /*p27.XAHY*/     c.vid.NEW_LINEn = not(b.spr.NEW_LINE);

  /*p27.MOSU*/ c.vid.MOSU = not(b.vid.NYFO);
  /*p27.NYFO*/   c.vid.NYFO = not(b.vid.NUNY);
  /*p27.NUNY*/     c.vid.NUNY = and(!b.vid.NOPA, b.vid.TILE_ABb);
  /*p27.NOPA*/       c.vid.NOPA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.sys.VID_RESETn1, b.vid.NOPA, b.vid.TILE_ABb);


  /*p27.LEBO*/ c.vid.BG_SEQ_CLK = nand(b.sys.CLK_AxCxExGx4, b.vid.BG_SEQ_5n);
  /*p27.NYXU*/ c.vid.BG_SEQ_RSTn = nor(b.spr.SCAN_DONE_TRIG, b.vid.MOSU, b.vid.MAP_X_CLK_STOPn);

  /*p27.LAXU*/ c.vid.BG_SEQ_x1x3x5x7 = tock_pos( a.vid.BG_SEQ_CLK,      b.vid.BG_SEQ_CLK,       b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ_x1x3x5x7, !b.vid.BG_SEQ_x1x3x5x7);
  /*p27.MESU*/ c.vid.BG_SEQ_xx23xx67 = tock_pos(!a.vid.BG_SEQ_x1x3x5x7, !b.vid.BG_SEQ_x1x3x5x7, b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ_xx23xx67, !b.vid.BG_SEQ_xx23xx67);
  /*p27.NYVA*/ c.vid.BG_SEQ_xxxx4567 = tock_pos(!a.vid.BG_SEQ_xx23xx67, !b.vid.BG_SEQ_xx23xx67, b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ_xxxx4567, !b.vid.BG_SEQ_xxxx4567);

  /*p27.NAKO*/ c.vid.BG_SEQ_01xx45xx  = not(b.vid.BG_SEQ_xx23xx67);
  /*p27.NOFU*/ c.vid.BG_SEQ_0123xxxx  = not(b.vid.BG_SEQ_xxxx4567);
  /*p27.XUHA*/ c.vid.BG_SEQ_xxxx4567b = not(b.vid.BG_SEQ_0123xxxx);


  /*p27.MOCE*/ c.vid.BG_SEQ_5n = nand(b.vid.BG_SEQ_x1x3x5x7,
                                      b.vid.BG_SEQ_xxxx4567,
                                      b.vid.BG_SEQ_RSTn); // counts to 5? polarity?

  /*p27.LYRY*/ c.vid.BG_SEQ_5 = not(b.vid.BG_SEQ_5n);

  /*p27.NOGU*/ c.vid.BG_SEQ_xx234567 = nand(b.vid.BG_SEQ_01xx45xx,
                                            b.vid.BG_SEQ_0123xxxx);


  /*p27.LOVY*/ c.vid.BG_SEQ5_SYNC = tock_pos(a.vid.CLK_xBxDxFxHc,  b.vid.CLK_xBxDxFxHc, b.vid.BG_SEQ_RSTn, b.vid.BG_SEQ5_SYNC, b.vid.BG_SEQ_5);

  /*p27.LYZU*/ c.vid.BG_SEQ_x1x3x5x7_DELAY = tock_pos(a.sys.CLK_AxCxExGx4,  b.sys.CLK_AxCxExGx4, b.vid.RENDERING, b.vid.BG_SEQ_x1x3x5x7_DELAY,  b.vid.BG_SEQ_x1x3x5x7);

  //----------
  // address output bus

  /*p27.WAZY*/ c.vid.TILE_Y_CLK = not(b.vid.TILE_Y_CLKn);
  /*p27.PORE*/   c.vid.TILE_Y_CLKn = not(b.vid.TILE_ABo);
  /*p27.NOCU*/     c.vid.TILE_ABo    = not(b.vid.TILE_ABb);
  /*p27.VYNO*/ c.vid.TILE_Y0 = tock_pos( a.vid.TILE_Y_CLK, b.vid.TILE_Y_CLK, b.vid.IN_FRAME_Yn, b.vid.TILE_Y0, !b.vid.TILE_Y0);
  /*p27.VUJO*/ c.vid.TILE_Y1 = tock_pos(!a.vid.TILE_Y0,    !b.vid.TILE_Y0,   b.vid.IN_FRAME_Yn, b.vid.TILE_Y1, !b.vid.TILE_Y1);
  /*p27.VYMU*/ c.vid.TILE_Y2 = tock_pos(!a.vid.TILE_Y1,    !b.vid.TILE_Y1,   b.vid.IN_FRAME_Yn, b.vid.TILE_Y2, !b.vid.TILE_Y2);

  /*p27.TEVO*/ c.vid.MAP_X_CLK_STOPn = nor(b.vid.WIN_TRIGGER, b.vid.SUZU, b.vid.TAVE);
  /*p27.SUZU*/   c.vid.SUZU = not(b.vid.TUXY);
  /*p27.TAVE*/   c.vid.TAVE = not(b.vid.SUVU);
  /*p27.SUVU*/     c.vid.SUVU = nand(b.vid.RENDERING, b.vid.ROMO, b.vid.NYKA, b.vid.PORY);
  /*p27.ROMO*/       c.vid.ROMO = not(b.vid.POKY);

  /*p27.VETU*/ c.vid.MAP_X_CLK = and(b.vid.TILE_Y_CLKn, b.vid.MAP_X_CLK_STOPn);
  /*p27.XACO*/ c.vid.MAP_X_RSTn = not(b.vid.MAP_X_RST);

  /*p27.WYKA*/ c.vid.MAP_X0 = tock_pos( a.vid.MAP_X_CLK, b.vid.MAP_X_CLK, b.vid.MAP_X_RSTn, b.vid.MAP_X0, !b.vid.MAP_X0);
  /*p27.WODY*/ c.vid.MAP_X1 = tock_pos(!a.vid.MAP_X0,    !b.vid.MAP_X0,   b.vid.MAP_X_RSTn, b.vid.MAP_X1, !b.vid.MAP_X1);
  /*p27.WOBO*/ c.vid.MAP_X2 = tock_pos(!a.vid.MAP_X1,    !b.vid.MAP_X1,   b.vid.MAP_X_RSTn, b.vid.MAP_X2, !b.vid.MAP_X2);
  /*p27.WYKO*/ c.vid.MAP_X3 = tock_pos(!a.vid.MAP_X2,    !b.vid.MAP_X2,   b.vid.MAP_X_RSTn, b.vid.MAP_X3, !b.vid.MAP_X3);
  /*p27.XOLO*/ c.vid.MAP_X4 = tock_pos(!a.vid.MAP_X3,    !b.vid.MAP_X3,   b.vid.MAP_X_RSTn, b.vid.MAP_X4, !b.vid.MAP_X4);

  /*p27.TUFU*/ c.vid.MAP_Y0 = tock_pos(!a.vid.TILE_Y2,   !b.vid.TILE_Y2, b.vid.IN_FRAME_Yn, b.vid.MAP_Y0, !b.vid.MAP_Y0);
  /*p27.TAXA*/ c.vid.MAP_Y1 = tock_pos(!a.vid.MAP_Y0,    !b.vid.MAP_Y0,  b.vid.IN_FRAME_Yn, b.vid.MAP_Y1, !b.vid.MAP_Y1);
  /*p27.TOZO*/ c.vid.MAP_Y2 = tock_pos(!a.vid.MAP_Y1,    !b.vid.MAP_Y1,  b.vid.IN_FRAME_Yn, b.vid.MAP_Y2, !b.vid.MAP_Y2);
  /*p27.TATE*/ c.vid.MAP_Y3 = tock_pos(!a.vid.MAP_Y2,    !b.vid.MAP_Y2,  b.vid.IN_FRAME_Yn, b.vid.MAP_Y3, !b.vid.MAP_Y3);
  /*p27.TEKE*/ c.vid.MAP_Y4 = tock_pos(!a.vid.MAP_Y3,    !b.vid.MAP_Y3,  b.vid.IN_FRAME_Yn, b.vid.MAP_Y4, !b.vid.MAP_Y4);


  /*p27.VYPO*/ c.vid.P10_Bn = not(chip_in.P10_B);

  /*p25.VUZA*/ c.vid.WIN_TILE_BANK = nor(b.vid.BG_TILE_SEL, b.pix.VRAM_TEMP_D7);

  //----------

  /*p27.TEKY*/ c.vid.TEKY = and(b.spr.OAM_SCAN, b.vid.TUKU, b.vid.BG_SEQ_5, b.vid.SOWO);
  /*p27.TUKU*/   c.vid.TUKU = not(b.vid.TOMU);
  /*p27.SOWO*/   c.vid.SOWO = not(b.vid.TAKA);
  /*p27.TAKA*/     c.vid.TAKA = unk2(b.vid.VEKU, b.vid.SECA_RST);
  /*p27.VEKU*/       c.vid.VEKU = nor(b.spr.WUTY, b.vid.TAVE);
  /*p27.SECA*/       c.vid.SECA_RST = nor(b.vid.TEKY_TRIG, b.sys.VID_RESET5, b.spr.NEW_LINE);
  /*p27.RYCE*/         c.vid.TEKY_TRIG = and(b.vid.TEKY_SYNC1, !b.vid.TEKY_SYNC2);

  /*p27.SOBU*/ c.vid.TEKY_SYNC1 = tock_pos(a.vid.CLK_AxCxExGxa, b.vid.CLK_AxCxExGxa, b.vid.P10_Bn, b.vid.TEKY_SYNC1, b.vid.TEKY);
  /*p27.SUDA*/ c.vid.TEKY_SYNC2 = tock_pos(a.sys.CLK_xBxDxFxH5, b.sys.CLK_xBxDxFxH5, b.vid.P10_Bn, b.vid.TEKY_SYNC2, b.vid.TEKY_SYNC1);

  //----------

  /*p27.LENA*/ c.vid.LONYb = not(b.vid.LONYn);
  /*p27.LUSU*/   c.vid.LONYn = not(b.vid.LONY);
  /*p27.LURY*/     c.vid.LONY = and(c.vid.LURY, b.vid.BG_SEQ_RSTn);
  /*p27.LONY*/       c.vid.LURY = and(!c.vid.BG_SEQ5_SYNC, b.vid.RENDERING);


  /*p27.POTU*/ c.vid.POTU = and(b.vid.LONYb, b.vid.BG_SEQ_01xxxxxx);
  /*p27.NENY*/   c.vid.BG_SEQ_01xxxxxx = not(b.vid.BG_SEQ_xx234567);


  //----------

  {
    //----------
    // vid x/y + scroll x/y

    /*p26.FAFO*/ wire TILE_Y0C = add_c(b.vid.V0, b.vid.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1C = add_c(b.vid.V1, b.vid.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2C = add_c(b.vid.V2, b.vid.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0C  = add_c(b.vid.V3, b.vid.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1C  = add_c(b.vid.V4, b.vid.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2C  = add_c(b.vid.V5, b.vid.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3C  = add_c(b.vid.V6, b.vid.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4C  = add_c(b.vid.V7, b.vid.SCY7, MAP_Y3C);

    /*p26.FAFO*/ wire TILE_Y0S  = add_s(b.vid.V0, b.vid.SCY0, 0);
    /*p26.EMUX*/ wire TILE_Y1S  = add_s(b.vid.V1, b.vid.SCY1, TILE_Y0C);
    /*p26.ECAB*/ wire TILE_Y2S  = add_s(b.vid.V2, b.vid.SCY2, TILE_Y1C);
    /*p26.ETAM*/ wire MAP_Y0S   = add_s(b.vid.V3, b.vid.SCY3, TILE_Y2C);
    /*p26.DOTO*/ wire MAP_Y1S   = add_s(b.vid.V4, b.vid.SCY4, MAP_Y0C);
    /*p26.DABA*/ wire MAP_Y2S   = add_s(b.vid.V5, b.vid.SCY5, MAP_Y1C);
    /*p26.EFYK*/ wire MAP_Y3S   = add_s(b.vid.V6, b.vid.SCY6, MAP_Y2C);
    /*p26.EJOK*/ wire MAP_Y4S   = add_s(b.vid.V7, b.vid.SCY7, MAP_Y3C);

    /*p26.ATAD*/ wire TILE_X0C = add_c(b.vid.X0, b.vid.SCX0, 0);
    /*p26.BEHU*/ wire TILE_X1C = add_c(b.vid.X1, b.vid.SCX1, TILE_X0C);
    /*p26.APYH*/ wire TILE_X2C = add_c(b.vid.X2, b.vid.SCX2, TILE_X1C);
    /*p26.BABE*/ wire MAP_X0C  = add_c(b.vid.X3, b.vid.SCX3, TILE_X2C);
    /*p26.ABOD*/ wire MAP_X1C  = add_c(b.vid.X4, b.vid.SCX4, MAP_X0C);
    /*p26.BEWY*/ wire MAP_X2C  = add_c(b.vid.X5, b.vid.SCX5, MAP_X1C);
    /*p26.BYCA*/ wire MAP_X3C  = add_c(b.vid.X6, b.vid.SCX6, MAP_X2C);
    /*p26.ACUL*/ wire MAP_X4C  = add_c(b.vid.X7, b.vid.SCX7, MAP_X3C);

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

    /*p25.XANE*/ c.vid.A_TO_MAn = nor(b.sys.VRAM_TO_OAMn, b.vid.RENDERING);
    /*p25.XEDU*/ c.vid.A_TO_MA = not(b.vid.A_TO_MAn);

    /*p25.XAKY*/ if (b.vid.A_TO_MA) c.MA00 = b.A00;
    /*p25.XUXU*/ if (b.vid.A_TO_MA) c.MA01 = b.A01;
    /*p25.XYNE*/ if (b.vid.A_TO_MA) c.MA02 = b.A02;
    /*p25.XODY*/ if (b.vid.A_TO_MA) c.MA03 = b.A03;
    /*p25.XECA*/ if (b.vid.A_TO_MA) c.MA04 = b.A04;
    /*p25.XOBA*/ if (b.vid.A_TO_MA) c.MA05 = b.A05;
    /*p25.XOPO*/ if (b.vid.A_TO_MA) c.MA06 = b.A06;
    /*p25.XYBO*/ if (b.vid.A_TO_MA) c.MA07 = b.A07;
    /*p25.RYSU*/ if (b.vid.A_TO_MA) c.MA08 = b.A08;
    /*p25.RESE*/ if (b.vid.A_TO_MA) c.MA09 = b.A09;
    /*p25.RUSE*/ if (b.vid.A_TO_MA) c.MA10 = b.A10;
    /*p25.RYNA*/ if (b.vid.A_TO_MA) c.MA11 = b.A11;
    /*p25.RUMO*/ if (b.vid.A_TO_MA) c.MA12 = b.A12;

    /*p29.ABON*/ c.spr.SPRITE_READ = not(b.spr.SPRITE_READn);
    /*p29.ABEM*/ if (b.spr.SPRITE_READ) c.MA00 = b.spr.SPRITE_AB;
    /*p29.BAXE*/ if (b.spr.SPRITE_READ) c.MA01 = b.spr.SPRITE_Y0;
    /*p29.ARAS*/ if (b.spr.SPRITE_READ) c.MA02 = b.spr.SPRITE_Y1;
    /*p29.AGAG*/ if (b.spr.SPRITE_READ) c.MA03 = b.spr.SPRITE_Y2;
    /*p29.FAMU*/ if (b.spr.SPRITE_READ) c.MA04 = b.spr.GEJY;
    /*p29.FUGY*/ if (b.spr.SPRITE_READ) c.MA05 = b.spr.OAM_B_D1;
    /*p29.GAVO*/ if (b.spr.SPRITE_READ) c.MA06 = b.spr.OAM_B_D2;
    /*p29.WYGA*/ if (b.spr.SPRITE_READ) c.MA07 = b.spr.OAM_B_D3;
    /*p29.WUNE*/ if (b.spr.SPRITE_READ) c.MA08 = b.spr.OAM_B_D4;
    /*p29.GOTU*/ if (b.spr.SPRITE_READ) c.MA09 = b.spr.OAM_B_D5;
    /*p29.GEGU*/ if (b.spr.SPRITE_READ) c.MA10 = b.spr.OAM_B_D6;
    /*p29.XEHE*/ if (b.spr.SPRITE_READ) c.MA11 = b.spr.OAM_B_D7;
    /*p29.DYSO*/ if (b.spr.SPRITE_READ) c.MA12 = chip_in.P10_B;   // sprites always in low half of tile store

    /*p25.XEZE*/ wire WIN_MAP_READn = nand(b.vid.POTU, b.vid.TILE_Y_CLKn);
    /*p25.WUKO*/ wire WIN_MAP_READ = not(WIN_MAP_READn);

    /*p27.XEJA*/ if (WIN_MAP_READ) c.MA00 = b.vid.MAP_X0;
    /*p27.XAMO*/ if (WIN_MAP_READ) c.MA01 = b.vid.MAP_X1;
    /*p27.XAHE*/ if (WIN_MAP_READ) c.MA02 = b.vid.MAP_X2;
    /*p27.XULO*/ if (WIN_MAP_READ) c.MA03 = b.vid.MAP_X3;
    /*p27.WUJU*/ if (WIN_MAP_READ) c.MA04 = b.vid.MAP_X4;
    /*p27.VYTO*/ if (WIN_MAP_READ) c.MA05 = b.vid.MAP_Y0;
    /*p27.VEHA*/ if (WIN_MAP_READ) c.MA06 = b.vid.MAP_Y1;
    /*p27.VACE*/ if (WIN_MAP_READ) c.MA07 = b.vid.MAP_Y2;
    /*p27.VOVO*/ if (WIN_MAP_READ) c.MA08 = b.vid.MAP_Y3;
    /*p27.VULO*/ if (WIN_MAP_READ) c.MA09 = b.vid.MAP_Y4;
    /*p27.VEVY*/ if (WIN_MAP_READ) c.MA10 = not(b.vid.WIN_MAP_SEL);
    /*p27.VEZA*/ if (WIN_MAP_READ) c.MA11 = not(b.vid.P10_Bn);
    /*p27.VOGU*/ if (WIN_MAP_READ) c.MA12 = not(b.vid.P10_Bn);

    /*p27.NETA*/ wire WIN_TILE_READa = and(b.vid.LONYb, b.vid.BG_SEQ_xx234567);
    /*p25.XUCY*/ wire WIN_TILE_READb = nand(WIN_TILE_READa, b.vid.TILE_Y_CLKn);

    /*p25.XONU*/ if (WIN_TILE_READb) c.MA00 = b.vid.BG_SEQ_xxxx4567b;
    /*p25.WUDO*/ if (WIN_TILE_READb) c.MA01 = b.vid.TILE_Y0;
    /*p25.WAWE*/ if (WIN_TILE_READb) c.MA02 = b.vid.TILE_Y1;
    /*p25.WOLU*/ if (WIN_TILE_READb) c.MA03 = b.vid.TILE_Y2;

    /*p25.VAPY*/ if (WIN_TILE_READa) c.MA04 = b.pix.VRAM_TEMP_D0;
    /*p25.SEZU*/ if (WIN_TILE_READa) c.MA05 = b.pix.VRAM_TEMP_D1;
    /*p25.VEJY*/ if (WIN_TILE_READa) c.MA06 = b.pix.VRAM_TEMP_D2;
    /*p25.RUSA*/ if (WIN_TILE_READa) c.MA07 = b.pix.VRAM_TEMP_D3;
    /*p25.ROHA*/ if (WIN_TILE_READa) c.MA08 = b.pix.VRAM_TEMP_D4;
    /*p25.RESO*/ if (WIN_TILE_READa) c.MA09 = b.pix.VRAM_TEMP_D5;
    /*p25.SUVO*/ if (WIN_TILE_READa) c.MA10 = b.pix.VRAM_TEMP_D6;
    /*p25.TOBO*/ if (WIN_TILE_READa) c.MA11 = b.pix.VRAM_TEMP_D7;
    /*p25.VUZA*/ if (WIN_TILE_READa) c.MA12 = b.vid.WIN_TILE_BANK;

    /*p26.AXAD*/ wire TILE_Y_CLK = not(b.vid.TILE_Y_CLKn);
    /*p26.ASUL*/ wire FETCH_TILEn = and(TILE_Y_CLK, WIN_TILE_READa);
    /*p26.BEJE*/ wire FETCH_TILE = not(FETCH_TILEn);

    /*p26.ACEN*/ wire FETCH_MAPn = and(TILE_Y_CLK, b.vid.POTU);
    /*p26.BAFY*/ wire FETCH_MAP  = not(FETCH_MAPn);

    /*p26.ASUM*/ if (FETCH_TILE) c.MA00 = b.vid.BG_SEQ_xxxx4567b;
    /*p26.EVAD*/ if (FETCH_TILE) c.MA01 = TILE_Y0S;
    /*p26.DAHU*/ if (FETCH_TILE) c.MA02 = TILE_Y1S;
    /*p26.DODE*/ if (FETCH_TILE) c.MA03 = TILE_Y2S;

    /*p26.AXEP*/ if (FETCH_MAP) c.MA00 = MAP_X0S;
    /*p26.AFEB*/ if (FETCH_MAP) c.MA01 = MAP_X1S;
    /*p26.ALEL*/ if (FETCH_MAP) c.MA02 = MAP_X2S;
    /*p26.COLY*/ if (FETCH_MAP) c.MA03 = MAP_X3S;
    /*p26.AJAN*/ if (FETCH_MAP) c.MA04 = MAP_X4S;

    /*p26.DUHO*/ if (FETCH_MAP) c.MA05 = MAP_Y0S;
    /*p26.CASE*/ if (FETCH_MAP) c.MA06 = MAP_Y1S;
    /*p26.CYPO*/ if (FETCH_MAP) c.MA07 = MAP_Y2S;
    /*p26.CETA*/ if (FETCH_MAP) c.MA08 = MAP_Y3S;
    /*p26.DAFE*/ if (FETCH_MAP) c.MA09 = MAP_Y4S;

    /*p26.AMUV*/ if (FETCH_MAP) c.MA10 = b.vid.LCDC_BGMAP;
    /*p26.COVE*/ if (FETCH_MAP) c.MA11 = b.spr.P10_Bn;
    /*p26.COXO*/ if (FETCH_MAP) c.MA12 = b.spr.P10_Bn;

    //----------
    // Memory address bus driver pin driver

    /*p25.LEXE*/ c.chip_out.MA00 = not(/*p25.MYFU*/ not(b.MA00));
    /*p25.LOZU*/ c.chip_out.MA01 = not(/*p25.MASA*/ not(b.MA01));
    /*p25.LACA*/ c.chip_out.MA02 = not(/*p25.MYRE*/ not(b.MA02));
    /*p25.LUVO*/ c.chip_out.MA03 = not(/*p25.MAVU*/ not(b.MA03));
    /*p25.LOLY*/ c.chip_out.MA04 = not(/*p25.MEPA*/ not(b.MA04));
    /*p25.LALO*/ c.chip_out.MA05 = not(/*p25.MYSA*/ not(b.MA05));
    /*p25.LEFA*/ c.chip_out.MA06 = not(/*p25.MEWY*/ not(b.MA06));
    /*p25.LUBY*/ c.chip_out.MA07 = not(/*p25.MUME*/ not(b.MA07));
    /*p25.TUJY*/ c.chip_out.MA08 = not(/*p25.VOVA*/ not(b.MA08));
    /*p25.TAGO*/ c.chip_out.MA09 = not(/*p25.VODE*/ not(b.MA09));
    /*p25.NUVA*/ c.chip_out.MA10 = not(/*p25.RUKY*/ not(b.MA10));
    /*p25.PEDU*/ c.chip_out.MA11 = not(/*p25.RUMA*/ not(b.MA11));
    /*p25.PONY*/ c.chip_out.MA12 = not(/*p25.REHO*/ not(b.MA12));
  }
}
