#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P21_VideoControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // x counter. this is a little weird, presumably because it can tick at 4 mhz but not always?

  c.p21.TADY = nor(b.p28.ATEJ, b.p01.VID_RESET3);

  c.p21.XUKE = and(b.p21.XEHO, b.p21.SAVY);
  c.p21.XYLE = and(b.p21.XODU, b.p21.XUKE);
  c.p21.RYBO = xor(b.p21.SAVY, b.p21.XEHO);
  c.p21.XEGY = xor(b.p21.XODU, b.p21.XUKE);
  c.p21.XORA = xor(b.p21.XYDO, b.p21.XYLE);

  c.p21.XEHO = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.XEHO, !b.p21.XEHO);
  c.p21.SAVY = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.SAVY, b.p21.RYBO);
  c.p21.XODU = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.XODU, b.p21.XEGY);
  c.p21.XYDO = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.XYDO, b.p21.XORA);

  c.p21.TOCA = not(b.p21.XYDO);

  c.p21.TYBA = and(b.p21.TUKY, b.p21.TUHU);
  c.p21.SURY = and(b.p21.TAKO, b.p21.TYBA);
  c.p21.SAKE = xor(b.p21.TUKY, b.p21.TUHU);
  c.p21.TYGE = xor(b.p21.TAKO, b.p21.TYBA);
  c.p21.ROKU = xor(b.p21.SYBE, b.p21.SURY);

  c.p21.TUHU = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.TUHU, !b.p21.TUHU);
  c.p21.TUKY = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.TUKY, b.p21.SAKE);
  c.p21.TAKO = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.TAKO, b.p21.TYGE);
  c.p21.SYBE = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.SYBE, b.p21.ROKU);

  // polarity?
  c.p21.ACAM = not(b.p21.XEHO);
  c.p21.AZUB = not(b.p21.SAVY);
  c.p21.AMEL = not(b.p21.XODU);
  c.p21.AHAL = not(b.p21.XYDO);
  c.p21.APUX = not(b.p21.TUHU);
  c.p21.ABEF = not(b.p21.TUKY);
  c.p21.ADAZ = not(b.p21.TAKO);
  c.p21.ASAH = not(b.p21.SYBE);

  //----------
  // LY compare

  c.p21.SYFU = xor(b.p21.V7, b.p23.FF45_D7);
  c.p21.TERY = xor(b.p21.V6, b.p23.FF45_D6);
  c.p21.TUCY = xor(b.p21.V5, b.p23.FF45_D5);
  c.p21.TYKU = xor(b.p21.V4, b.p23.FF45_D4);
  c.p21.RASY = xor(b.p21.V3, b.p23.FF45_D3);
  c.p21.REDA = xor(b.p21.V2, b.p23.FF45_D2);
  c.p21.TYDE = xor(b.p21.V1, b.p23.FF45_D1);
  c.p21.RYME = xor(b.p21.V0, b.p23.FF45_D0);

  c.p21.SOVU = nor(b.p21.SYFU, b.p21.TERY, b.p21.TUCY, b.p21.TYKU);
  c.p21.SUBO = nor(b.p21.RASY, b.p21.REDA, b.p21.TYDE, b.p21.RYME);
  c.p21.RAPE = nand(b.p21.SOVU, b.p21.SUBO);
  c.p21.PALY = not(b.p21.RAPE);

  //----------
  // top center

  c.p21.MUDE = nor(b.p21.RUTU, b.p01.VID_RESET2); // schematic says RUTU_OUT, but I think this is just RUTU?

  c.p21.SAXO = tock_pos( a.p21.TALU,  b.p21.TALU, b.p21.MUDE, c.p21.SAXO, !c.p21.SAXO);
  c.p21.TYPO = tock_pos(!a.p21.SAXO, !b.p21.SAXO, b.p21.MUDE, c.p21.TYPO, !c.p21.TYPO);
  c.p21.VYZO = tock_pos(!a.p21.TYPO, !b.p21.TYPO, b.p21.MUDE, c.p21.VYZO, !c.p21.VYZO);
  c.p21.TELU = tock_pos(!a.p21.VYZO, !b.p21.VYZO, b.p21.MUDE, c.p21.TELU, !c.p21.TELU);
  c.p21.SUDE = tock_pos(!a.p21.TELU, !b.p21.TELU, b.p21.MUDE, c.p21.SUDE, !c.p21.SUDE);
  c.p21.TAHA = tock_pos(!a.p21.SUDE, !b.p21.SUDE, b.p21.MUDE, c.p21.TAHA, !c.p21.TAHA);
  c.p21.TYRY = tock_pos(!a.p21.TAHA, !b.p21.TAHA, b.p21.MUDE, c.p21.TYRY, !c.p21.TYRY);

  c.p21.TOCU = not(b.p21.SAXO);
  c.p21.VEPE = not(b.p21.TYPO);
  c.p21.VUTY = not(b.p21.VYZO);
  c.p21.TUJU = not(b.p21.TELU);
  c.p21.TAFY = not(b.p21.SUDE);
  c.p21.TUDA = not(b.p21.TAHA);
  c.p21.VATE = not(b.p21.TYRY);

  c.p21.VOKU = nand(b.p21.TUJU, b.p21.TAFY, b.p21.TUDA, b.p21.VATE, b.p21.VUTY, b.p21.VEPE, b.p21.TOCU);
  c.p21.TOZU = nand(b.p21.TUJU, b.p21.TAFY, b.p21.TUDA, b.p21.VATE, b.p21.VYZO, b.p21.TYPO, b.p21.SAXO);
  c.p21.TECE = nand(b.p21.TUJU, b.p21.TAHA, b.p21.TUDA, b.p21.TELU, b.p21.VYZO, b.p21.VEPE, b.p21.SAXO);
  c.p21.TEBO = nand(b.p21.TYRY, b.p21.TAFY, b.p21.SUDE, b.p21.VATE, b.p21.VUTY, b.p21.TYPO, b.p21.SAXO);
  c.p21.TEGY = nand(b.p21.VOKU, b.p21.TOZU, b.p21.TECE, b.p21.TEBO);

  c.p21.SANU = nand(b.p21.TYRY, b.p21.TAHA, b.p21.SUDE, b.p21.SAXO);
  c.p21.RUTU = tock_pos(a.p21.SONO, b.p21.SONO, b.p01.VID_RESETn2, b.p21.RUTU, b.p21.SANU);
  c.p21.SYGU = tock_pos(a.p21.SONO, b.p21.SONO, b.p01.VID_RESETn2, b.p21.SYGU, b.p21.TEGY);
  c.p21.RYNO = or(b.p21.SYGU, b.p21.RUTU);
  c.p21.POGU = not(b.p21.RYNO);

  //----------
  // FF41 STAT

  c.p21.XYVO = and(b.p21.V4, b.p21.V7);

  c.p21.NYPE = tock_pos(a.p21.TALU, b.p21.TALU, b.p01.VID_RESETn2, b.p21.NYPE, b.p21.RUTU);
  c.p21.POPU = tock_pos(a.p21.NYPE, b.p21.NYPE, b.p01.VID_RESETn2, b.p21.POPU, b.p21.XYVO);
  c.p21.NAPO = tock_pos(a.p21.POPU, b.p21.POPU, b.p01.VID_RESETn2, b.p21.NAPO, !b.p21.NAPO);

  c.p21.XUGU = nand(b.p21.XEHO, b.p21.SAVY, b.p21.XODU, b.p21.TUKY, b.p21.SYBE);
  c.p21.XENA = not(b.p29.FEPO);
  c.p21.XANO = not(b.p21.XUGU);
  c.p21.PARU = not(!b.p21.POPU);
  c.p21.WODU = and(b.p21.XENA, b.p21.XANO);

  c.p21.PURE = not(b.p21.RUTU);
  c.p21.SELA = not(b.p21.PURE);
  c.p21.TOLU = not(b.p21.PARU);
  c.p21.TAPA = and(b.p21.SELA, b.p21.TOLU);
  c.p21.TARU = and(b.p21.TOLU, b.p21.WODU);
  c.p21.VYPU = not(b.p21.TOLU);

  //---

  c.p21.SEPA = and(b.p07.CPU_WR2, b.p22.FF41);

  c.p21.VOGA = tock_pos(a.p01.CLK_AxCxExGx4, b.p01.CLK_AxCxExGx4, b.p21.TADY, b.p21.VOGA, b.p21.WODU);
  c.p21.WEGO = or(b.p01.VID_RESET3, b.p21.VOGA);
  c.p21.XAJO = and(b.p21.XEHO, b.p21.XYDO);
  c.p21.XYMU = or(b.p21.WEGO, b.p29.AVAP);
  c.p21.WUSA = or(b.p21.XAJO, b.p21.WEGO);
  c.p21.TOBA = and(b.p24.CLKPIPE, b.p21.WUSA);
  c.p21.SADU = nor(b.p21.XYMU, b.p21.PARU);
  c.p21.XATY = nor(b.p28.ACYL, b.p21.XYMU);
  c.p21.SEMU = or(b.p21.TOBA, b.p27.POVA);
  c.p21.RYJU = not(b.p21.SEPA);
  c.p21.RYPO = not(b.p21.SEMU);
  c.p21.PAGO = or(b.p01.SYS_RESETn6, b.p21.RYJU);

  //---

  c.p21.RYVE = not(b.p21.SEPA);
  c.p21.RUGU = tock_pos(a.p21.RYVE, b.p21.RYVE, b.p01.SYS_RESETn6, b.p21.RUGU, b.D6);
  c.p21.REFE = tock_pos(a.p21.RYVE, b.p21.RYVE, b.p01.SYS_RESETn6, b.p21.REFE, b.D5);
  c.p21.RUFO = tock_pos(a.p21.RYVE, b.p21.RYVE, b.p01.SYS_RESETn6, b.p21.RUFO, b.D4);
  c.p21.ROXE = tock_pos(a.p21.RYVE, b.p21.RYVE, b.p01.SYS_RESETn6, b.p21.ROXE, b.D3);

  c.p21.ROPO = tock_pos(a.p21.TALU, b.p21.TALU, b.p01.SYS_RESETn6, b.p21.ROPO, b.p21.PALY); // this seems odd

  c.p21.PUZO = not(!b.p21.ROXE);
  c.p21.SASY = not(!b.p21.REFE);
  c.p21.POFO = not(!b.p21.RUFO);
  c.p21.POTE = not(!b.p21.RUGU);
  c.p21.TEBY = not(b.p21.SADU); // these two are the STAT mode signal
  c.p21.WUGA = not(b.p21.XATY);

  c.p21.SUKO = amux4(b.p21.RUGU, b.p21.ROPO, b.p21.REFE, b.p21.INT_OAM, b.p21.RUFO, b.p21.INT_VBL, b.p21.ROXE, b.p21.INT_HBL);
  c.p21.TUVA = not(b.p21.SUKO);
  c.p21.VOTY = not(b.p21.TUVA);
  c.p21.RUPO = unk2(b.p21.ROPO, b.p21.PAGO);
  c.p21.SEGO = not(b.p21.RUPO);

  c.p21.TOBE = and(b.p07.CPU_RD2, b.p22.FF41);
  c.p21.VAVE = b.p21.TOBE; // buffer, not inverter

  if (b.p21.TOBE) {
    c.D0 = b.p21.TEBY;
    c.D1 = b.p21.WUGA;
    c.D2 = b.p21.SEGO;
  }
  if (b.p21.VAVE) {
    c.D3 = b.p21.PUZO;
    c.D4 = b.p21.POFO;
    c.D5 = b.p21.SASY;
    c.D6 = b.p21.POTE;
  }

  //----------
  // y counter

  c.p21.NOKO = and(b.p21.V7, b.p21.V4, b.p21.V0, b.p21.V1);
  c.p21.MYTA = tock_pos(a.p21.NYPE, b.p21.NYPE, b.p01.VID_RESETn2, b.p21.MYTA, b.p21.NOKO);
  c.p21.LAMA = nor(b.p21.MYTA, b.p01.VID_RESET2);

  c.p21.MUWY = tock_pos(a.p21.RUTU,  b.p21.RUTU,  b.p21.LAMA, c.p21.MUWY, !c.p21.MUWY);
  c.p21.MYRO = tock_pos(!a.p21.MUWY, !b.p21.MUWY, b.p21.LAMA, c.p21.MYRO, !c.p21.MYRO);
  c.p21.LEXA = tock_pos(!a.p21.MYRO, !b.p21.MYRO, b.p21.LAMA, c.p21.LEXA, !c.p21.LEXA);
  c.p21.LYDO = tock_pos(!a.p21.LEXA, !b.p21.LEXA, b.p21.LAMA, c.p21.LYDO, !c.p21.LYDO);
  c.p21.LOVU = tock_pos(!a.p21.LYDO, !b.p21.LYDO, b.p21.LAMA, c.p21.LOVU, !c.p21.LOVU);
  c.p21.LEMA = tock_pos(!a.p21.LOVU, !b.p21.LOVU, b.p21.LAMA, c.p21.LEMA, !c.p21.LEMA);
  c.p21.MATO = tock_pos(!a.p21.LEMA, !b.p21.LEMA, b.p21.LAMA, c.p21.MATO, !c.p21.MATO);
  c.p21.LAFO = tock_pos(!a.p21.MATO, !b.p21.MATO, b.p21.LAMA, c.p21.LAFO, !c.p21.LAFO);
}