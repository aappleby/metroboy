#include "GateBoyLib/GateBoyInterrupts.h"

#include "CoreLib/Constants.h"

#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

// Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
// Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
// Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
// Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
// Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

void GateBoy::tock_interrupts_gates(const GateBoyReg& reg_old)
{
  auto cpu_addr = bit_pack(reg.cpu_abus);
  wire FFFF_HIT_ext = cpu_addr == 0xFFFF; 
  wire FFFF_RDn_ext = nand2(reg.cpu_signals.TEDO_CPU_RDp.out_new(), FFFF_HIT_ext);
  wire FFFF_WRn_ext = nand2(reg.cpu_signals.TAPU_CPU_WRp.out_new(), FFFF_HIT_ext);

  reg.reg_ie.IE_D0.dff_r(FFFF_WRn_ext, ~reg.sys_rst.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  reg.reg_ie.IE_D1.dff_r(FFFF_WRn_ext, ~reg.sys_rst.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  reg.reg_ie.IE_D2.dff_r(FFFF_WRn_ext, ~reg.sys_rst.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  reg.reg_ie.IE_D3.dff_r(FFFF_WRn_ext, ~reg.sys_rst.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  reg.reg_ie.IE_D4.dff_r(FFFF_WRn_ext, ~reg.sys_rst.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());

  /*_p21.SEPA*/ wire SEPA_FF41_WRp = and2(CUPA_CPU_WRp(), reg.cpu_abus.VARY_FF41p());
  /*_p21.RYVE*/ wire RYVE_FF41_WRn = not1(SEPA_FF41_WRp);

  /*_p21.ROXE*/ reg.reg_stat.ROXE_STAT_HBI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p21.RUFO*/ reg.reg_stat.RUFO_STAT_VBI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p21.REFE*/ reg.reg_stat.REFE_STAT_OAI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p21.RUGU*/ reg.reg_stat.RUGU_STAT_LYI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());

  /*_p21.TOBE*/ wire TOBE_FF41_RDp = and2(ASOT_CPU_RDp(), reg.cpu_abus.VARY_FF41p());
  /*_p21.VAVE*/ wire VAVE_FF41_RDn = not1(TOBE_FF41_RDp);

  /*#p21.PARU*/ wire PARU_VBLANKp = not1(reg.lcd.POPU_y144p.qn_new());
  /*#p21.SADU*/ wire SADU_STAT_MODE0n = nor2(reg.XYMU_RENDERINGn.qn_new(), PARU_VBLANKp);   // die NOR
  /*#p21.XATY*/ wire XATY_STAT_MODE1n = nor2(reg.ACYL_SCANNINGp.out_new(), reg.XYMU_RENDERINGn.qn_new()); // die NOR

  //probe_wire(24, "POPU VBLANK", lcd.POPU_y144p.qp_new());
  //probe_wire(25, "XYMU RENDER", ~XYMU_RENDERINGn.qp_new());
  //probe_wire(26, "ACYL SCAN",   ACYL_SCANNINGp.out_new());
  //probe_wire(27, "SADU STAT0",  ~SADU_STAT_MODE0n);
  //probe_wire(28, "XATY STAT1",  ~XATY_STAT_MODE1n);
  //probe_wire(29, "RUPO STAT2",  ~RUPO_LYC_MATCHn.qp_new());
  //probe_wire(30, "MUWY LY0",    reg_ly.MUWY_LY0p.qp_new());
  //probe_wire(31, "SYRY LYC0",   reg_lyc.SYRY_LYC0n.qn_new());

  /*#p21.TEBY*/ triwire TEBY_STAT0_TO_CD0 = tri6_pn(TOBE_FF41_RDp, SADU_STAT_MODE0n);
  /*#p21.WUGA*/ triwire WUGA_STAT1_TO_CD1 = tri6_pn(TOBE_FF41_RDp, XATY_STAT_MODE1n);
  /*#p21.SEGO*/ triwire SEGO_STAT2_TO_CD2 = tri6_pn(TOBE_FF41_RDp, reg.int_ctrl.RUPO_LYC_MATCHn.qp_new());
  /*_p21.PUZO*/ triwire PUZO_STAT3_TO_CD3 = tri6_nn(VAVE_FF41_RDn, reg.reg_stat.ROXE_STAT_HBI_ENn.qp_new());
  /*_p21.POFO*/ triwire POFO_STAT4_TO_CD4 = tri6_nn(VAVE_FF41_RDn, reg.reg_stat.RUFO_STAT_VBI_ENn.qp_new());
  /*_p21.SASY*/ triwire SASY_STAT5_TO_CD5 = tri6_nn(VAVE_FF41_RDn, reg.reg_stat.REFE_STAT_OAI_ENn.qp_new());
  /*_p21.POTE*/ triwire POTE_STAT6_TO_CD6 = tri6_nn(VAVE_FF41_RDn, reg.reg_stat.RUGU_STAT_LYI_ENn.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(TEBY_STAT0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(WUGA_STAT1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(SEGO_STAT2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(PUZO_STAT3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(POFO_STAT4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus.BUS_CPU_D05p.tri_bus(SASY_STAT5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus.BUS_CPU_D06p.tri_bus(POTE_STAT6_TO_CD6);

  /*#p21.PURE*/ wire PURE_x113n = not1(reg.lcd.RUTU_x113p.qp_new());
  /*#p21.TOLU*/ wire TOLU_VBLANKn   = not1(PARU_VBLANKp);
  /*#p21.SELA*/ wire SELA_x113p = not1(PURE_x113n);
  /*#p21.TAPA*/ wire TAPA_INT_OAM   = and2(TOLU_VBLANKn, SELA_x113p);
  /*#p21.TARU*/ wire TARU_INT_HBL   = and2(reg.WODU_HBLANKp.out_new(), TOLU_VBLANKn);
  /*#p21.SUKO*/ wire SUKO_INT_STATp = amux4(reg.reg_stat.RUGU_STAT_LYI_ENn.qn_new(), reg.int_ctrl.ROPO_LY_MATCH_SYNCp.qp_new(), reg.reg_stat.REFE_STAT_OAI_ENn.qn_new(), TAPA_INT_OAM, reg.reg_stat.RUFO_STAT_VBI_ENn.qn_new(), PARU_VBLANKp, reg.reg_stat.ROXE_STAT_HBI_ENn.qn_new(), TARU_INT_HBL);

  /*#p21.VYPU*/ wire VYPU_INT_VBLANKp = not1(TOLU_VBLANKn);
  /*#p21.TUVA*/ wire TUVA_INT_STATn   = not1(SUKO_INT_STATp);
  /*#p21.VOTY*/ wire VOTY_INT_STATp   = not1(TUVA_INT_STATn);

  /*#p02.ASOK*/ wire ASOK_INT_JOYp = nand2(reg.joy_int.APUG_JP_GLITCH3.qp_new(), reg.joy_int.BATU_JP_GLITCH0.qp_new());

  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp());
  /*_p07.REFA*/ wire REFA_FF0F_WRn = nand4(reg.cpu_signals.TAPU_CPU_WRp.out_new(), reg.cpu_abus.SYKE_ADDR_HIp(), reg.cpu_abus.SEMY_XX_0000xxxxp(), reg.cpu_abus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND

  /*_p02.LETY*/ wire LETY_INT_VBL_ACKn  = not1(reg.cpu_ack.SIG_CPU_ACK_VBLANK.out_new());
  /*_p02.LEJA*/ wire LEJA_INT_STAT_ACKn = not1(reg.cpu_ack.SIG_CPU_ACK_STAT.out_new());
  /*_p02.LESA*/ wire LESA_INT_TIM_ACKn  = not1(reg.cpu_ack.SIG_CPU_ACK_TIMER.out_new());
  /*_p02.LUFE*/ wire LUFE_INT_SER_ACKn  = not1(reg.cpu_ack.SIG_CPU_ACK_SERIAL.out_new());
  /*_p02.LAMO*/ wire LAMO_INT_JOY_ACKn  = not1(reg.cpu_ack.SIG_CPU_ACK_JOYPAD.out_new());

  /*_p02.ROTU*/ wire ROTU_FF0F_WRp   = not1(REFA_FF0F_WRn);
  /*_p02.MYZU*/ wire MYZU_FF0F_SET0n = nand3(ROTU_FF0F_WRp, LETY_INT_VBL_ACKn,  reg.cpu_dbus.BUS_CPU_D00p.out_new());
  /*_p02.MODY*/ wire MODY_FF0F_SET1n = nand3(ROTU_FF0F_WRp, LEJA_INT_STAT_ACKn, reg.cpu_dbus.BUS_CPU_D01p.out_new());
  /*_p02.PYHU*/ wire PYHU_FF0F_SET2n = nand3(ROTU_FF0F_WRp, LESA_INT_TIM_ACKn,  reg.cpu_dbus.BUS_CPU_D02p.out_new());
  /*_p02.TOME*/ wire TOME_FF0F_SET3n = nand3(ROTU_FF0F_WRp, LUFE_INT_SER_ACKn,  reg.cpu_dbus.BUS_CPU_D03p.out_new());
  /*_p02.TOGA*/ wire TOGA_FF0F_SET4n = nand3(ROTU_FF0F_WRp, LAMO_INT_JOY_ACKn,  reg.cpu_dbus.BUS_CPU_D04p.out_new());

  /*#p02.MUXE*/ wire MUXE_INT0_WRn = or2(reg.cpu_dbus.BUS_CPU_D00p.out_new(), REFA_FF0F_WRn);
  /*_p02.NABE*/ wire NABE_INT1_WRn = or2(reg.cpu_dbus.BUS_CPU_D01p.out_new(), REFA_FF0F_WRn);
  /*_p02.RAKE*/ wire RAKE_INT2_WRn = or2(reg.cpu_dbus.BUS_CPU_D02p.out_new(), REFA_FF0F_WRn);
  /*_p02.SULO*/ wire SULO_INT3_WRn = or2(reg.cpu_dbus.BUS_CPU_D03p.out_new(), REFA_FF0F_WRn);
  /*_p02.SEME*/ wire SEME_INT4_WRn = or2(reg.cpu_dbus.BUS_CPU_D04p.out_new(), REFA_FF0F_WRn);

  /*#p02.LYTA*/ wire LYTA_FF0F_RST0n = and3(MUXE_INT0_WRn, LETY_INT_VBL_ACKn,  ALUR_SYS_RSTn);
  /*_p02.MOVU*/ wire MOVU_FF0F_RST1n = and3(NABE_INT1_WRn, LEJA_INT_STAT_ACKn, ALUR_SYS_RSTn);
  /*_p02.PYGA*/ wire PYGA_FF0F_RST2n = and3(RAKE_INT2_WRn, LESA_INT_TIM_ACKn,  ALUR_SYS_RSTn);
  /*_p02.TUNY*/ wire TUNY_FF0F_RST3n = and3(SULO_INT3_WRn, LUFE_INT_SER_ACKn,  ALUR_SYS_RSTn);
  /*_p02.TYME*/ wire TYME_FF0F_RST4n = and3(SEME_INT4_WRn, LAMO_INT_JOY_ACKn,  ALUR_SYS_RSTn);

  /*_p02.LOPE*/ reg.reg_if.LOPE_FF0F_D0p.dff22(VYPU_INT_VBLANKp,              MYZU_FF0F_SET0n, LYTA_FF0F_RST0n, reg.SIG_VCC.out_new());
  /*_p02.LALU*/ reg.reg_if.LALU_FF0F_D1p.dff22(VOTY_INT_STATp,                MODY_FF0F_SET1n, MOVU_FF0F_RST1n, reg.SIG_VCC.out_new());
  /*_p02.NYBO*/ reg.reg_if.NYBO_FF0F_D2p.dff22(reg.int_ctrl.MOBA_TIMER_OVERFLOWp.qp_new(), PYHU_FF0F_SET2n, PYGA_FF0F_RST2n, reg.SIG_VCC.out_new());
  ///*_p02.UBUL*/ reg_if.UBUL_FF0F_D3p.dff22(serial.CALY_SER_CNT3.qp_new(), TOME_FF0F_SET3n, TUNY_FF0F_RST3n, SIG_VCC.out_new());
  /*_p02.UBUL*/ reg.reg_if.UBUL_FF0F_D3p.dff22(0,                             TOME_FF0F_SET3n, TUNY_FF0F_RST3n, reg.SIG_VCC.out_new());
  /*_p02.ULAK*/ reg.reg_if.ULAK_FF0F_D4p.dff22(ASOK_INT_JOYp,                 TOGA_FF0F_SET4n, TYME_FF0F_RST4n, reg.SIG_VCC.out_new());

  /*_SIG_CPU_INT_VBLANK*/ reg.cpu_int.SIG_CPU_INT_VBLANK.sig_out(reg.reg_if.LOPE_FF0F_D0p.qp_new());
  /*_SIG_CPU_INT_STAT  */ reg.cpu_int.SIG_CPU_INT_STAT  .sig_out(reg.reg_if.LALU_FF0F_D1p.qp_new());
  /*_SIG_CPU_INT_TIMER */ reg.cpu_int.SIG_CPU_INT_TIMER .sig_out(reg.reg_if.NYBO_FF0F_D2p.qp_new());
  /*_SIG_CPU_INT_SERIAL*/ reg.cpu_int.SIG_CPU_INT_SERIAL.sig_out(reg.reg_if.UBUL_FF0F_D3p.qp_new());
  /*_SIG_CPU_INT_JOYPAD*/ reg.cpu_int.SIG_CPU_INT_JOYPAD.sig_out(reg.reg_if.ULAK_FF0F_D4p.qp_new());

  // FIXME where did these tags go?
  triwire tristate_i0 = tri6_nn(FFFF_RDn_ext, reg.reg_ie.IE_D0.qn_new());
  triwire tristate_i1 = tri6_nn(FFFF_RDn_ext, reg.reg_ie.IE_D1.qn_new());
  triwire tristate_i2 = tri6_nn(FFFF_RDn_ext, reg.reg_ie.IE_D2.qn_new());
  triwire tristate_i3 = tri6_nn(FFFF_RDn_ext, reg.reg_ie.IE_D3.qn_new());
  triwire tristate_i4 = tri6_nn(FFFF_RDn_ext, reg.reg_ie.IE_D4.qn_new());

  reg.cpu_dbus.BUS_CPU_D00p.tri_bus(tristate_i0);
  reg.cpu_dbus.BUS_CPU_D01p.tri_bus(tristate_i1);
  reg.cpu_dbus.BUS_CPU_D02p.tri_bus(tristate_i2);
  reg.cpu_dbus.BUS_CPU_D03p.tri_bus(tristate_i3);
  reg.cpu_dbus.BUS_CPU_D04p.tri_bus(tristate_i4);

  /*_p07.ROLO*/ wire ROLO_FF0F_RDn = nand4(reg.cpu_signals.TEDO_CPU_RDp.out_new(), reg.cpu_abus.SYKE_ADDR_HIp(), reg.cpu_abus.SEMY_XX_0000xxxxp(), reg.cpu_abus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND
  /*_p02.POLA*/ wire POLA_FF0F_RDp = not1(ROLO_FF0F_RDn);

  // FIXME why is this latch different from the others? MATY is one of those big yellow latchy things.

  /*_p02.MATY*/ reg.int_latch.MATY_FF0F_L0p.tp_latchp(ROLO_FF0F_RDn, reg.reg_if.LOPE_FF0F_D0p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.MOPO*/ reg.int_latch.MOPO_FF0F_L1p.tp_latchp(ROLO_FF0F_RDn, reg.reg_if.LALU_FF0F_D1p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.PAVY*/ reg.int_latch.PAVY_FF0F_L2p.tp_latchp(ROLO_FF0F_RDn, reg.reg_if.NYBO_FF0F_D2p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.NEJY*/ reg.int_latch.NEJY_FF0F_L3p.tp_latchp(ROLO_FF0F_RDn, reg.reg_if.UBUL_FF0F_D3p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.NUTY*/ reg.int_latch.NUTY_FF0F_L4p.tp_latchp(ROLO_FF0F_RDn, reg.reg_if.ULAK_FF0F_D4p.qp_new()); // OUTPUT ON RUNG 10

  /*#p02.NELA*/ triwire NELA_IF0_TO_CD0 = tri6_pn(POLA_FF0F_RDp, reg.int_latch.MATY_FF0F_L0p.qn_new());
  /*#p02.NABO*/ triwire NABO_IF1_TO_CD1 = tri6_pn(POLA_FF0F_RDp, reg.int_latch.MOPO_FF0F_L1p.qn_new());
  /*#p02.ROVA*/ triwire ROVA_IF2_TO_CD2 = tri6_pn(POLA_FF0F_RDp, reg.int_latch.PAVY_FF0F_L2p.qn_new());
  /*#p02.PADO*/ triwire PADO_IF3_TO_CD3 = tri6_pn(POLA_FF0F_RDp, reg.int_latch.NEJY_FF0F_L3p.qn_new());
  /*#p02.PEGY*/ triwire PEGY_IF4_TO_CD4 = tri6_pn(POLA_FF0F_RDp, reg.int_latch.NUTY_FF0F_L4p.qn_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(NELA_IF0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(NABO_IF1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(ROVA_IF2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(PADO_IF3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(PEGY_IF4_TO_CD4);
}
