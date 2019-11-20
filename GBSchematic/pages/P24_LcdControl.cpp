#include "P24_LcdControl.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P24_LcdControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p24.LOBY*/ c.p24.LOBY = not(b.p21.XYMU);
  /*p24.NAFY*/ c.p24.NAFY = nor(b.p27.MOSU, b.p24.LOBY);
  /*p24.NYKA*/ c.p24.NYKA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p24.NAFY, b.p24.NYKA, b.p27.LYRY);
  /*p24.PORY*/ c.p24.PORY = tock_pos(a.p21.CLK_xBxDxFxHc, b.p21.CLK_xBxDxFxHc, b.p24.NAFY, b.p24.PORY, b.p24.NYKA);
  /*p24.PYGO*/ c.p24.PYGO = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p21.XYMU, b.p24.PYGO, b.p24.PORY);
  /*p24.TOMU*/ c.p24.TOMU = not(b.p27.SYLO);
  /*p24.POKY*/ c.p24.POKY = unk2(b.p24.PYGO, b.p24.LOBY);
  /*p24.SOCY*/ c.p24.SOCY = not(b.p24.TOMU);
  /*p24.VYBO*/ c.p24.VYBO = nor(b.p29.OAM_SCAN, b.p21.RENDER_DONEn, b.p21.CLK_xBxDxFxHc);
  /*p24.TYFA*/ c.p24.TYFA = and(b.p24.SOCY, b.p24.POKY, b.p24.VYBO);
  /*p24.SEGU*/ c.p24.SEGU = not(b.p24.TYFA);
  /*p24.SACU*/ c.p24.CLKPIPE = nor(b.p24.SEGU, b.p27.ROXY);
  /*p24.ROXO*/ c.p24.ROXO = not(b.p24.SEGU);
  /*p24.PAHO*/ c.p24.PAHO = tock_pos(a.p24.ROXO, b.p24.ROXO, b.p21.XYMU, b.p24.PAHO, b.p21.X3);

  /*p24.POME*/ c.p24.POME = nor(b.p29.AVAP, b.p24.POFY);
  /*p24.RUJU*/ c.p24.RUJU = or(b.p24.PAHO, b.sys.VID_RESET3, b.p24.POME);
  /*p24.POFY*/ c.p24.POFY = not(b.p24.RUJU);
  /*p24.RUZE*/ c.p24.RUZE = not(b.p24.POFY);

  c.chip.ST = b.p24.RUZE;

  //----------
  // Vertical sync

  /*p24.NERU*/ c.p24.LINE_000n = nor(b.p21.V0, b.p21.V1, b.p21.V2, b.p21.V3, b.p21.V4, b.p21.V5, b.p21.V6, b.p21.V7);
  /*p24.MEDA*/ c.p24.LINE_000_SYNCn = tock_pos(a.p21.LINE_DONE_DELAYn, b.p21.LINE_DONE_DELAYn, b.sys.VID_RESETn2, b.p24.LINE_000_SYNCn, b.p24.LINE_000n);
  /*p24.MURE*/ c.p24.LINE_000_SYNC = not(b.p24.LINE_000_SYNCn);

  c.chip.S = b.p24.LINE_000_SYNC;

  //----------

  /*p24.LOFU*/ c.p24.LOFU = not(b.p21.LINE_DONEn);
  /*p24.LUCA*/ c.p24.LUCA = tock_pos(a.p24.LOFU, b.p24.LOFU, b.sys.VID_RESETn2, b.p24.LUCA, !b.p24.LUCA);
  /*p24.MAGU*/ c.p24.MAGU = xor(b.p21.NAPO, b.p24.LUCA);
  /*p24.LEBE*/ c.p24.LEBE = tock_pos(!a.p24.LUCA, !b.p24.LUCA, b.sys.VID_RESETn2, b.p24.LEBE, !b.p24.LEBE);
  /*p24.MECO*/ c.p24.MECO = not(b.p24.MAGU);
  /*p24.KEBO*/ c.p24.KEBO = not(b.p24.MECO);
  /*p24.USEC*/ c.p24.USEC = not(b.sys.DIV_07n);
  /*p24.KEDY*/ c.p24.LCDC_ENn = not(b.p23.LCDC_EN);

  /*p24.KASA*/ c.p24.KASA = not(b.p21.LINE_DONEa);
  /*p24.UMOB*/ c.p24.UMOB = not(b.sys.DIV_06n);

  /*p24.KAHE*/ c.p24.CPLn = amux2(b.p23.LCDC_EN, b.p24.KASA, b.p24.LCDC_ENn, b.p24.UMOB);
  /*p24.KUPA*/ c.p24.FRn  = amux2(b.p23.LCDC_EN, b.p24.KEBO, b.p24.LCDC_ENn, b.p24.USEC);
  /*p24.KYMO*/ c.p24.CPL  = not(b.p24.CPLn);
  /*p24.KOFO*/ c.p24.FR   = not(b.p24.FRn);

  c.chip.CPL = b.p24.CPL;
  c.chip.FR  = b.p24.FR;
}