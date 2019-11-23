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