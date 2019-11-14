#include "P24_LcdControl.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P24_LcdControl::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p24.LOBY = not(b.p21.XYMU);
  c.p24.NAFY = nor(b.p27.MOSU, b.p24.LOBY);
  c.p24.NYKA = tock_pos(a.p01.CLK_AxCxExGx4, b.p01.CLK_AxCxExGx4, b.p24.NAFY, b.p24.NYKA, b.p27.LYRY);
  c.p24.PORY = tock_pos(a.p27.MYVO,          b.p27.MYVO,          b.p24.NAFY, b.p24.PORY, b.p24.NYKA);
  c.p24.PYGO = tock_pos(a.p01.CLK_AxCxExGx4, b.p01.CLK_AxCxExGx4, b.p21.XYMU, b.p24.PYGO, b.p24.PORY);
  c.p24.TOMU = not(b.p27.SYLO);
  c.p24.POKY = unk2(b.p24.PYGO, b.p24.LOBY);
  c.p24.SOCY = not(b.p24.TOMU);
  c.p24.VYBO = nor(b.p29.FEPO, b.p21.WODU, b.p27.MYVO);
  c.p24.TYFA = and(b.p24.SOCY, b.p24.POKY, b.p24.VYBO);
  c.p24.SEGU = not(b.p24.TYFA);
  c.p24.SACU = nor(b.p24.SEGU, b.p27.ROXY);
  c.p24.ROXO = not(b.p24.SEGU);
  c.p24.PAHO = tock_pos(a.p24.ROXO, b.p24.ROXO, b.p21.XYMU, b.p24.PAHO, b.p21.XYDO);

  c.p24.POME = nor(b.p29.AVAP, b.p24.POFY);
  c.p24.RUJU = or(b.p24.PAHO, b.p01.VID_RESET3, b.p24.POME);
  c.p24.POFY = not(b.p24.RUJU);
  c.p24.RUZE = not(b.p24.POFY);

  c.chip.ST = b.p24.RUZE;

  //----------
  // Vertical sync

  c.p24.NERU = nor(b.p21.V0, b.p21.V1, b.p21.V2, b.p21.V3, b.p21.V4, b.p21.V5, b.p21.V6, b.p21.V7);
  c.p24.MEDA = tock_pos(a.p21.NYPE, b.p21.NYPE, b.p01.VID_RESETn2, b.p24.MEDA, b.p24.NERU);
  c.p24.MURE = not(b.p24.MEDA);

  c.chip.S = b.p24.MURE;

  //----------

  c.p24.LOFU = not(b.p21.RUTU);
  c.p24.LUCA = tock_pos(a.p24.LOFU, b.p24.LOFU, b.p01.VID_RESETn2, b.p24.LUCA, !b.p24.LUCA);
  c.p24.MAGU = xor(b.p21.NAPO, b.p24.LUCA);
  c.p24.LEBE = tock_pos(!a.p24.LUCA, !b.p24.LUCA, b.p01.VID_RESETn2, b.p24.LEBE, !b.p24.LEBE);
  c.p24.MECO = not(b.p24.MAGU);
  c.p24.KEBO = not(b.p24.MECO);
  c.p24.KASA = not(b.p21.PURE);
  c.p24.UMOB = not(b.p01.DIV_06n);
  c.p24.USEC = not(b.p01.DIV_07n);
  c.p24.LCD_OFF = not(b.p23.LCD_ON);

  c.p24.KAHE = amux2(b.p23.LCD_ON, b.p24.KASA, b.p24.LCD_OFF, b.p24.UMOB);
  c.p24.KUPA = amux2(b.p23.LCD_ON, b.p24.KEBO, b.p24.LCD_OFF, b.p24.USEC);
  c.p24.KYMO = not(b.p24.KAHE);
  c.p24.KOFO = not(b.p24.KUPA);

  c.chip.CPL = b.p24.KYMO;
  c.chip.FR = b.p24.KOFO;
}