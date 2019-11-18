#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P21_VideoControl_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p27.MOXE*/ c.p21.MOXE = not(b.sys.CLK_AxCxExGx4);
  /*p27.MEHE*/ c.p21.MEHE = not(b.sys.CLK_AxCxExGx4);
  /*p27.MYVO*/ c.p21.MYVO = not(b.sys.CLK_AxCxExGx4);
  /*p27.TAVA*/ c.p21.TAVA = not(b.sys.CLK_xBxDxFxH5);

  /*p29.LAPE*/ c.p29.LAPE = not(b.sys.CLK_AxCxExGx4);
  /*p29.XYVA*/ c.p29.XYVA = not(b.sys.CLK_xBxDxFxH2);
  /*p29.XOTA*/ c.p29.XOTA = not(b.p29.XYVA);
  /*p29.XYFY*/ c.p29.XYFY = not(b.p29.XOTA);

  /*p29.WUVU*/ c.p29.WUVU = tock_pos(a.p29.XOTA, b.p29.XOTA, b.sys.VID_RESETn1, b.p29.WUVU, !b.p29.WUVU);
  /*p29.XUPY*/ c.p29.XUPY = not(!b.p29.WUVU);

  /*p21.VENA*/ c.p21.VENA = tock_pos(!a.p29.WUVU, !b.p29.WUVU, b.sys.VID_RESETn1, b.p21.VENA, !b.p21.VENA);
  /*p21.TALU*/ c.p21.TALU = not(!b.p21.VENA);
  /*p21.SONO*/ c.p21.SONO = not(b.p21.TALU);

  //----------
  // x counter. this is a little weird, presumably because it can tick at 4 mhz but not always?

  /*p21.TADY*/ c.p21.TADY = nor(b.p28.ATEJ, b.sys.VID_RESET3);
  /*p21.XUKE*/ c.p21.XUKE = and(b.p21.XEHO, b.p21.SAVY);
  /*p21.XYLE*/ c.p21.XYLE = and(b.p21.XODU, b.p21.XUKE);
  /*p21.RYBO*/ c.p21.RYBO = xor(b.p21.SAVY, b.p21.XEHO);
  /*p21.XEGY*/ c.p21.XEGY = xor(b.p21.XODU, b.p21.XUKE);
  /*p21.XORA*/ c.p21.XORA = xor(b.p21.XYDO, b.p21.XYLE);

  /*p21.XEHO*/ c.p21.XEHO = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.XEHO, !b.p21.XEHO);
  /*p21.SAVY*/ c.p21.SAVY = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.SAVY, b.p21.RYBO);
  /*p21.XODU*/ c.p21.XODU = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.XODU, b.p21.XEGY);
  /*p21.XYDO*/ c.p21.XYDO = tock_pos(a.p24.CLKPIPE, b.p24.CLKPIPE, b.p21.TADY, b.p21.XYDO, b.p21.XORA);

  /*p21.TOCA*/ c.p21.TOCA = not(b.p21.XYDO);
  /*p21.TYBA*/ c.p21.TYBA = and(b.p21.TUKY, b.p21.TUHU);
  /*p21.SURY*/ c.p21.SURY = and(b.p21.TAKO, b.p21.TYBA);
  /*p21.SAKE*/ c.p21.SAKE = xor(b.p21.TUKY, b.p21.TUHU);
  /*p21.TYGE*/ c.p21.TYGE = xor(b.p21.TAKO, b.p21.TYBA);
  /*p21.ROKU*/ c.p21.ROKU = xor(b.p21.SYBE, b.p21.SURY);

  /*p21.TUHU*/ c.p21.TUHU = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.TUHU, !b.p21.TUHU);
  /*p21.TUKY*/ c.p21.TUKY = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.TUKY, b.p21.SAKE);
  /*p21.TAKO*/ c.p21.TAKO = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.TAKO, b.p21.TYGE);
  /*p21.SYBE*/ c.p21.SYBE = tock_pos(a.p21.TOCA, b.p21.TOCA, b.p21.TADY, b.p21.SYBE, b.p21.ROKU);

  /*p21.ACAM*/ c.p21.ACAM = not(b.p21.XEHO); // polarity?
  /*p21.AZUB*/ c.p21.AZUB = not(b.p21.SAVY);
  /*p21.AMEL*/ c.p21.AMEL = not(b.p21.XODU);
  /*p21.AHAL*/ c.p21.AHAL = not(b.p21.XYDO);
  /*p21.APUX*/ c.p21.APUX = not(b.p21.TUHU);
  /*p21.ABEF*/ c.p21.ABEF = not(b.p21.TUKY);
  /*p21.ADAZ*/ c.p21.ADAZ = not(b.p21.TAKO);
  /*p21.ASAH*/ c.p21.ASAH = not(b.p21.SYBE);

  //----------
  // LY compare

  /*p21.SYFU*/ c.p21.SYFU = xor(b.p21.V7, b.p23.FF45_D7);
  /*p21.TERY*/ c.p21.TERY = xor(b.p21.V6, b.p23.FF45_D6);
  /*p21.TUCY*/ c.p21.TUCY = xor(b.p21.V5, b.p23.FF45_D5);
  /*p21.TYKU*/ c.p21.TYKU = xor(b.p21.V4, b.p23.FF45_D4);
  /*p21.RASY*/ c.p21.RASY = xor(b.p21.V3, b.p23.FF45_D3);
  /*p21.REDA*/ c.p21.REDA = xor(b.p21.V2, b.p23.FF45_D2);
  /*p21.TYDE*/ c.p21.TYDE = xor(b.p21.V1, b.p23.FF45_D1);
  /*p21.RYME*/ c.p21.RYME = xor(b.p21.V0, b.p23.FF45_D0);

  /*p21.SOVU*/ c.p21.SOVU = nor(b.p21.SYFU, b.p21.TERY, b.p21.TUCY, b.p21.TYKU);
  /*p21.SUBO*/ c.p21.SUBO = nor(b.p21.RASY, b.p21.REDA, b.p21.TYDE, b.p21.RYME);
  /*p21.RAPE*/ c.p21.RAPE = nand(b.p21.SOVU, b.p21.SUBO);
  /*p21.PALY*/ c.p21.PALY = not(b.p21.RAPE);

  //----------
  // top center

  /*p21.MUDE*/ c.p21.MUDE = nor(b.p21.RUTU, b.sys.VID_RESET2); // schematic says RUTU_OUT, but I think this is just RUTU?
  /*p21.SAXO*/ c.p21.SAXO = tock_pos( a.p21.TALU,  b.p21.TALU, b.p21.MUDE, c.p21.SAXO, !c.p21.SAXO);
  /*p21.TYPO*/ c.p21.TYPO = tock_pos(!a.p21.SAXO, !b.p21.SAXO, b.p21.MUDE, c.p21.TYPO, !c.p21.TYPO);
  /*p21.VYZO*/ c.p21.VYZO = tock_pos(!a.p21.TYPO, !b.p21.TYPO, b.p21.MUDE, c.p21.VYZO, !c.p21.VYZO);
  /*p21.TELU*/ c.p21.TELU = tock_pos(!a.p21.VYZO, !b.p21.VYZO, b.p21.MUDE, c.p21.TELU, !c.p21.TELU);
  /*p21.SUDE*/ c.p21.SUDE = tock_pos(!a.p21.TELU, !b.p21.TELU, b.p21.MUDE, c.p21.SUDE, !c.p21.SUDE);
  /*p21.TAHA*/ c.p21.TAHA = tock_pos(!a.p21.SUDE, !b.p21.SUDE, b.p21.MUDE, c.p21.TAHA, !c.p21.TAHA);
  /*p21.TYRY*/ c.p21.TYRY = tock_pos(!a.p21.TAHA, !b.p21.TAHA, b.p21.MUDE, c.p21.TYRY, !c.p21.TYRY);

  /*p21.TOCU*/ c.p21.TOCU = not(b.p21.SAXO);
  /*p21.VEPE*/ c.p21.VEPE = not(b.p21.TYPO);
  /*p21.VUTY*/ c.p21.VUTY = not(b.p21.VYZO);
  /*p21.TUJU*/ c.p21.TUJU = not(b.p21.TELU);
  /*p21.TAFY*/ c.p21.TAFY = not(b.p21.SUDE);
  /*p21.TUDA*/ c.p21.TUDA = not(b.p21.TAHA);
  /*p21.VATE*/ c.p21.VATE = not(b.p21.TYRY);

  /*p21.VOKU*/ c.p21.VOKU = nand(b.p21.TUJU, b.p21.TAFY, b.p21.TUDA, b.p21.VATE, b.p21.VUTY, b.p21.VEPE, b.p21.TOCU);
  /*p21.TOZU*/ c.p21.TOZU = nand(b.p21.TUJU, b.p21.TAFY, b.p21.TUDA, b.p21.VATE, b.p21.VYZO, b.p21.TYPO, b.p21.SAXO);
  /*p21.TECE*/ c.p21.TECE = nand(b.p21.TUJU, b.p21.TAHA, b.p21.TUDA, b.p21.TELU, b.p21.VYZO, b.p21.VEPE, b.p21.SAXO);
  /*p21.TEBO*/ c.p21.TEBO = nand(b.p21.TYRY, b.p21.TAFY, b.p21.SUDE, b.p21.VATE, b.p21.VUTY, b.p21.TYPO, b.p21.SAXO);

  /*p21.TEGY*/ c.p21.TEGY = nand(b.p21.VOKU, b.p21.TOZU, b.p21.TECE, b.p21.TEBO);
  /*p21.SANU*/ c.p21.SANU = nand(b.p21.TYRY, b.p21.TAHA, b.p21.SUDE, b.p21.SAXO);

  /*p21.RUTU*/ c.p21.RUTU = tock_pos(a.p21.SONO, b.p21.SONO, b.sys.VID_RESETn2, b.p21.RUTU, b.p21.SANU);
  /*p21.SYGU*/ c.p21.SYGU = tock_pos(a.p21.SONO, b.p21.SONO, b.sys.VID_RESETn2, b.p21.SYGU, b.p21.TEGY);

  /*p21.RYNO*/ c.p21.RYNO = or(b.p21.SYGU, b.p21.RUTU);
  /*p21.POGU*/ c.p21.PIN_CPG = not(b.p21.RYNO);

  //----------
  // FF41 STAT

  /*p21.NYPE*/ c.p21.NYPE = tock_pos(a.p21.TALU, b.p21.TALU, b.sys.VID_RESETn2, b.p21.NYPE, b.p21.RUTU);

  /*p21.XYVO*/ c.p21.LINE_144 = and(b.p21.V4, b.p21.V7);
  /*p21.POPU*/ c.p21.LINE_144_SYNC = tock_pos(a.p21.NYPE,          b.p21.NYPE,          b.sys.VID_RESETn2, b.p21.LINE_144_SYNC, b.p21.LINE_144);

  // why do we have a ff listening to line144?
  /*p21.NAPO*/ c.p21.NAPO          = tock_pos(a.p21.LINE_144_SYNC, b.p21.LINE_144_SYNC, b.sys.VID_RESETn2, b.p21.NAPO,          !b.p21.NAPO);

  /*p21.XUGU*/ c.p21.XUGU = nand(b.p21.XEHO, b.p21.SAVY, b.p21.XODU, b.p21.TUKY, b.p21.SYBE);

  /*p21.XENA*/ c.p21.XENA = not(b.p29.FEPO);
  /*p21.XANO*/ c.p21.XANO = not(b.p21.XUGU);

  /*p21.WODU*/ c.p21.WODU = and(b.p21.XENA, b.p21.XANO);

  /*p21.PURE*/ c.p21.PURE = not(b.p21.RUTU);
  /*p21.SELA*/ c.p21.SELA = not(b.p21.PURE);

  /*p21.PARU*/ c.p21.INT_VBL = not(!b.p21.LINE_144_SYNC);
  /*p21.TOLU*/ c.p21.TOLU = not(b.p21.INT_VBL);
  /*p21.VYPU*/ c.p21.INT_VBL_BUF = not(b.p21.TOLU);

  /*p21.TAPA*/ c.p21.INT_OAM = and(b.p21.SELA, b.p21.TOLU);
  /*p21.TARU*/ c.p21.INT_HBL = and(b.p21.TOLU, b.p21.WODU);

  //---

  /*p21.SEPA*/ c.p21.SEPA = and(b.sys.CPU_WR2, b.p22.FF41);
  /*p21.VOGA*/ c.p21.VOGA = tock_pos(a.sys.CLK_AxCxExGx4, b.sys.CLK_AxCxExGx4, b.p21.TADY, b.p21.VOGA, b.p21.WODU);
  /*p21.WEGO*/ c.p21.WEGO = or(b.sys.VID_RESET3, b.p21.VOGA);
  /*p21.XAJO*/ c.p21.XAJO = and(b.p21.XEHO, b.p21.XYDO);
  /*p21.XYMU*/ c.p21.XYMU = or(b.p21.WEGO, b.p29.AVAP);
  /*p21.WUSA*/ c.p21.WUSA = or(b.p21.XAJO, b.p21.WEGO);
  /*p21.TOBA*/ c.p21.TOBA = and(b.p24.CLKPIPE, b.p21.WUSA);
  /*p21.SEMU*/ c.p21.SEMU = or(b.p21.TOBA, b.p27.POVA);
  /*p21.RYJU*/ c.p21.RYJU = not(b.p21.SEPA);
  /*p21.RYPO*/ c.p21.CP = not(b.p21.SEMU);

  //---

  /*p21.RYVE*/ c.p21.RYVE = not(b.p21.SEPA);


  /*p21.ROXE*/ c.p21.INT_HBL_EN = tock_pos(a.p21.RYVE, b.p21.RYVE, b.sys.SYS_RESETn6, b.p21.INT_HBL_EN, b.D3);
  /*p21.RUFO*/ c.p21.INT_VBL_EN = tock_pos(a.p21.RYVE, b.p21.RYVE, b.sys.SYS_RESETn6, b.p21.INT_VBL_EN, b.D4);
  /*p21.REFE*/ c.p21.INT_OAM_EN = tock_pos(a.p21.RYVE, b.p21.RYVE, b.sys.SYS_RESETn6, b.p21.INT_OAM_EN, b.D5);
  /*p21.RUGU*/ c.p21.INT_LYC_EN = tock_pos(a.p21.RYVE, b.p21.RYVE, b.sys.SYS_RESETn6, b.p21.INT_LYC_EN, b.D6);

  /*p21.ROPO*/ c.p21.INT_LYC = tock_pos(a.p21.TALU, b.p21.TALU, b.sys.SYS_RESETn6, b.p21.INT_LYC, b.p21.PALY); // this seems odd



  /*p21.SUKO*/ c.p21.INT_STATb = amux4(b.p21.INT_LYC_EN, b.p21.INT_LYC,
                                       b.p21.INT_OAM_EN, b.p21.INT_OAM,
                                       b.p21.INT_VBL_EN, b.p21.INT_VBL,
                                       b.p21.INT_HBL_EN, b.p21.INT_HBL);

  /*p21.TUVA*/ c.p21.INT_STATn = not(b.p21.INT_STATb);
  /*p21.VOTY*/ c.p21.INT_STAT  = not(b.p21.INT_STATn);

  /*p21.PAGO*/ c.p21.PAGO = or(b.sys.SYS_RESETn6, b.p21.RYJU);

  
  //----------
  // FF41 STAT stuff

  /*p21.TOBE*/ c.p21.FF41_RDa = and(b.sys.CPU_RD2, b.p22.FF41);
  /*p21.VAVE*/ c.p21.FF41_RDb = b.p21.FF41_RDa; // buffer, not inverter

  /*p21.SADU*/ c.p21.STAT_MODE0     = nor(b.p21.XYMU, b.p21.INT_VBL);
  /*p21.XATY*/ c.p21.STAT_MODE1     = nor(b.p28.ACYL, b.p21.XYMU);
  /*p21.RUPO*/ c.p21.STAT_LYC_MATCH = unk2(b.p21.INT_LYC, b.p21.PAGO);

  /*p21.TEBY*/ if (b.p21.FF41_RDa) c.D0 = not(b.p21.STAT_MODE0);
  /*p21.WUGA*/ if (b.p21.FF41_RDa) c.D1 = not(b.p21.STAT_MODE1);
  /*p21.SEGO*/ if (b.p21.FF41_RDa) c.D2 = not(b.p21.STAT_LYC_MATCH);
  /*p21.PUZO*/ if (b.p21.FF41_RDb) c.D3 = b.p21.INT_HBL_EN;
  /*p21.POFO*/ if (b.p21.FF41_RDb) c.D4 = b.p21.INT_VBL_EN;
  /*p21.SASY*/ if (b.p21.FF41_RDb) c.D5 = b.p21.INT_OAM_EN;
  /*p21.POTE*/ if (b.p21.FF41_RDb) c.D6 = b.p21.INT_LYC_EN;

  //----------
  // y counter

  /*p21.NOKO*/ c.p21.NOKO = and(b.p21.V7, b.p21.V4, b.p21.V0, b.p21.V1);
  /*p21.MYTA*/ c.p21.MYTA = tock_pos(a.p21.NYPE, b.p21.NYPE, b.sys.VID_RESETn2, b.p21.MYTA, b.p21.NOKO);
  /*p21.LAMA*/ c.p21.LAMA = nor(b.p21.MYTA, b.sys.VID_RESET2);

  /*p21.MUWY*/ c.p21.V0 = tock_pos(a.p21.RUTU, b.p21.RUTU, b.p21.LAMA, c.p21.V0, !c.p21.V0);
  /*p21.MYRO*/ c.p21.V1 = tock_pos(!a.p21.V0,  !b.p21.V0,  b.p21.LAMA, c.p21.V1, !c.p21.V1);
  /*p21.LEXA*/ c.p21.V2 = tock_pos(!a.p21.V1,  !b.p21.V1,  b.p21.LAMA, c.p21.V2, !c.p21.V2);
  /*p21.LYDO*/ c.p21.V3 = tock_pos(!a.p21.V2,  !b.p21.V2,  b.p21.LAMA, c.p21.V3, !c.p21.V3);
  /*p21.LOVU*/ c.p21.V4 = tock_pos(!a.p21.V3,  !b.p21.V3,  b.p21.LAMA, c.p21.V4, !c.p21.V4);
  /*p21.LEMA*/ c.p21.V5 = tock_pos(!a.p21.V4,  !b.p21.V4,  b.p21.LAMA, c.p21.V5, !c.p21.V5);
  /*p21.MATO*/ c.p21.V6 = tock_pos(!a.p21.V5,  !b.p21.V5,  b.p21.LAMA, c.p21.V6, !c.p21.V6);
  /*p21.LAFO*/ c.p21.V7 = tock_pos(!a.p21.V6,  !b.p21.V6,  b.p21.LAMA, c.p21.V7, !c.p21.V7);
}