#include "PPU.h"

#include "Clocks.h"
#include "CpuBus.h"
#include "ExtBus.h"
#include "MemBus.h"
#include "PPU.h"
#include "AddressDecoder.h"
#include "Sprites.h"
#include "Window.h"
#include "LCD.h"

struct PPU ppu;

//-----------------------------------------------------------------------------

void PPU::tick_videocontrol(const Resets& rst, const AddressDecoder& dec) {

  //----------
  // LY compare

  bool SYFU = xor(ppu.V7, ppu.FF45_D7);
  bool TERY = xor(ppu.V6, ppu.FF45_D6);
  bool TUCY = xor(ppu.V5, ppu.FF45_D5);
  bool TYKU = xor(ppu.V4, ppu.FF45_D4);
  bool RASY = xor(ppu.V3, ppu.FF45_D3);
  bool REDA = xor(ppu.V2, ppu.FF45_D2);
  bool TYDE = xor(ppu.V1, ppu.FF45_D1);
  bool RYME = xor(ppu.V0, ppu.FF45_D0);

  bool SOVU = nor(SYFU, TERY, TUCY, TYKU);
  bool SUBO = nor(RASY, REDA, TYDE, RYME);
  bool RAPE = nand(SOVU, SUBO);
  PALY = not(RAPE);

  //----------
  // top center

  bool WUVU_Q = spr.WUVU.q();
  bool VENA_Q = VENA.flip(!WUVU_Q, rst.RESET_VIDEO);
  bool RUTU_Q = RUTU.q();
  bool SYGU_Q = SYGU.q();

  bool MUDE = nor(RUTU_Q, LYHA); // schematic says RUTU_OUT, but I think this is just RUTU_Q?
  TALU = not(!VENA_Q);

  bool SAXO_Q = SAXO.flip(TALU,    MUDE);
  bool TYPO_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool VYZO_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TELU_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool SUDE_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TAHA_Q = TYPO.flip(!SAXO_Q, MUDE);
  bool TYRY_Q = TYPO.flip(!SAXO_Q, MUDE);


  bool TOCU = not(SAXO_Q);
  bool VEPE = not(TYPO_Q);
  bool VUTY = not(VYZO_Q);
  bool TUJU = not(TELU_Q);
  bool TAFY = not(SUDE_Q);
  bool TUDA = not(TAHA_Q);
  bool VATE = not(TYRY_Q);

  bool VOKU = nand(TUJU,   TAFY,   TUDA,   VATE,   VUTY,   VEPE,   TOCU);
  bool TOZU = nand(TUJU,   TAFY,   TUDA,   VATE,   VYZO_Q, TYPO_Q, SAXO_Q);
  bool TECE = nand(TUJU,   TAHA_Q, TUDA,   TELU_Q, VYZO_Q, VEPE,   SAXO_Q);
  bool TEBO = nand(TYRY_Q, TAFY,   SUDE_Q, VATE,   VUTY,   TYPO_Q, SAXO_Q);

  bool TEGY = nand(VOKU, TOZU, TECE, TEBO);
  bool SANU = nand(TYRY_Q, TAHA_Q, SUDE_Q, SAXO_Q);

  bool SONO = not(TALU);

  bool RYNO = or(SYGU_Q, RUTU_Q);
  bool POGU = not(RYNO);
  ext.PIN_CPG = POGU;

  RUTU.tock(SONO, LYFE, SANU);
  SYGU.tock(SONO, LYFE, TEGY);

  //----------
  // x counter

  bool TADY = nor(spr.ATEJ, lcd.TOFU);

  bool XEHO_Q = ppu.XEHO.q();
  bool SAVY_Q = ppu.SAVY.q();
  bool XODU_Q = ppu.XODU.q();
  bool XYDO_Q = ppu.XYDO.q();

  bool XUKE = and(XEHO_Q, SAVY_Q);
  bool XYLE = and(XODU_Q, XUKE);
  bool RYBO = xor(SAVY_Q, XEHO_Q);
  bool XEGY = xor(XODU_Q, XUKE);
  bool XORA = xor(XYDO_Q, XYLE);

  ppu.XEHO.tock(clk.CLKPIPE, TADY, !XEHO_Q);
  ppu.SAVY.tock(clk.CLKPIPE, TADY, RYBO);
  ppu.XODU.tock(clk.CLKPIPE, TADY, XEGY);
  ppu.XYDO.tock(clk.CLKPIPE, TADY, XORA);

  bool TOCA = not(XYDO_Q);

  bool TUHU_Q = ppu.TUHU.q();
  bool TUKY_Q = ppu.TUKY.q();
  bool TAKO_Q = ppu.TAKO.q();
  bool SYBE_Q = ppu.SYBE.q();

  bool TYBA = and(TUKY_Q, TUHU_Q);
  bool SURY = and(TAKO_Q, TYBA);
  bool SAKE = xor(TUKY_Q, TUHU_Q);
  bool TYGE = xor(TAKO_Q, TYBA);
  bool ROKU = xor(SYBE_Q, SURY);

  ppu.TUHU.tock(TOCA, TADY, !TUHU_Q);
  ppu.TUKY.tock(TOCA, TADY, SAKE);
  ppu.TAKO.tock(TOCA, TADY, TYGE);
  ppu.SYBE.tock(TOCA, TADY, ROKU);

  ppu.ACAM = not(XEHO_Q);
  ppu.AZUB = not(SAVY_Q);
  ppu.AMEL = not(XODU_Q);
  ppu.AHAL = not(XYDO_Q);
  ppu.APUX = not(TUHU_Q);
  ppu.ABEF = not(TUKY_Q);
  ppu.ADAZ = not(TAKO_Q);
  ppu.ASAH = not(SYBE_Q);

  //----------
  // FF41 STAT

  XYVO = and(ppu.V4, ppu.V7);

  bool NYPE_Q = NYPE.tock(TALU,   LYFE, RUTU_Q); // RUTU_OUT on the schematic?
  bool POPU_Q = POPU.tock(NYPE_Q, LYFE, XYVO);
  NAPO.flip(POPU_Q, LYFE);

  bool XUGU = nand(XEHO_Q, SAVY_Q, XODU_Q, TUKY_Q, SYBE_Q);
  bool XENA = not(spr.FEPO);
  bool XANO = not(XUGU);
  bool PARU = not(!POPU_Q);
  INT_VBL = PARU;
  WODU = and(XENA, XANO);

  PURE = not(RUTU_Q);
  SELA = not(PURE);
  bool TOLU = not(PARU);
  bool TAPA = and(SELA, TOLU);
  INT_OAM = TAPA;
  bool TARU = and(TOLU, WODU);
  INT_HBL = TARU;
  bool VYPU = not(TOLU);
  cpu.INT_VBL_BUF = VYPU;

  //---

  bool SEPA = and(cpu.CPU_WR2, dec.FF41);

  bool VOGA_Q = VOGA.tock(clk.CLK2, TADY, WODU);
  bool WEGO = or(lcd.TOFU, VOGA_Q);
  bool XAJO = and(XEHO_Q, XYDO_Q);
  XYMU = unk2(WEGO, spr.AVAP);
  bool WUSA = unk2(XAJO, WEGO);
  bool TOBA = and(clk.CLKPIPE, WUSA);
  bool SADU = nor(XYMU, PARU);
  bool XATY = nor(spr.ACYL, XYMU);
  bool SEMU = or(TOBA, win.POVA);
  bool RYJU = not(SEPA);
  bool RYPO = not(SEMU);
  bool PAGO = or(clk.WESY, RYJU);

  //---

  bool RYVE = not(SEPA);
  bool RUGU_Q = RUGU.tock(RYVE, clk.WESY, mem.D6);
  bool REFE_Q = REFE.tock(RYVE, clk.WESY, mem.D5);
  bool ROPO_Q = ROPO.tock(TALU, clk.WESY, PALY); // this seems odd
  bool RUFO_Q = RUFO.tock(RYVE, clk.WESY, mem.D4);
  bool ROXE_Q = ROXE.tock(RYVE, clk.WESY, mem.D3);

  bool PUZO = not(!ROXE_Q);
  bool SASY = not(!REFE_Q);
  bool POFO = not(!RUFO_Q);
  bool POTE = not(!RUGU_Q);
  bool TEBY = not(SADU); // these two are the STAT mode signal
  bool WUGA = not(XATY);

  bool SUKO = amux4(RUGU_Q, ROPO_Q, REFE_Q, INT_OAM, RUFO_Q, INT_VBL, ROXE_Q, INT_HBL);
  bool TUVA = not(SUKO);
  bool VOTY = not(TUVA);
  bool RUPO = unk2(ROPO_Q, PAGO);
  bool SEGO = not(RUPO);

  // pin output?
  ext.CP = RYPO;
  cpu.INT_STAT = VOTY;

  bool TOBE = and(cpu.CPU_RD2, dec.FF41);
  bool VAVE = TOBE; // buffer, not inverter
  if (TOBE) {
    mem.D0 = TEBY;
    mem.D1 = WUGA;
    mem.D2 = SEGO;
  }
  if (VAVE) {
    mem.D3 = PUZO;
    mem.D4 = POFO;
    mem.D5 = SASY;
    mem.D6 = POTE;
  }

  //----------
  // y counter

  bool NOKO = and(ppu.V7, ppu.V4, ppu.V0, ppu.V1);
  LYHA = not(rst.RESET_VIDEO);
  LYFE = not(LYHA);
  bool MYTA_Q = MYTA.tock(NYPE_Q, LYFE, NOKO);
  bool LAMA = nor(MYTA_Q, LYHA);

  bool MUWY_Q = ppu.MUWY.flip(RUTU_Q,  LAMA);
  bool MYRO_Q = ppu.MYRO.flip(!MUWY_Q, LAMA);
  bool LEXA_Q = ppu.LEXA.flip(!MYRO_Q, LAMA);
  bool LYDO_Q = ppu.LYDO.flip(!LEXA_Q, LAMA);
  bool LOVU_Q = ppu.LOVU.flip(!LYDO_Q, LAMA);
  bool LEMA_Q = ppu.LEMA.flip(!LOVU_Q, LAMA);
  bool MATO_Q = ppu.MATO.flip(!LEMA_Q, LAMA);
  bool LAFO_Q = ppu.LAFO.flip(!MATO_Q, LAMA);

  ppu.V0 = MUWY_Q;
  ppu.V1 = MYRO_Q;
  ppu.V2 = LEXA_Q;
  ppu.V3 = LYDO_Q;
  ppu.V4 = LOVU_Q;
  ppu.V5 = LEMA_Q;
  ppu.V6 = MATO_Q;
  ppu.V7 = LAFO_Q;
}