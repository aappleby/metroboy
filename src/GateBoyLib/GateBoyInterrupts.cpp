#include "GateBoyLib/GateBoyInterrupts.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"
#include "GateBoyLib/Utils.h"

//-----------------------------------------------------------------------------

// Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
// Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
// Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
// Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
// Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

void GateBoy::tock_interrupts_gates(const GateBoyState& reg_old)
{
  auto& reg_new = gb_state;

  auto cpu_addr_new = bit_pack(reg_new.cpu_abus);
  wire FFFF_HIT_ext_new = cpu_addr_new == 0xFFFF; 
  wire FFFF_RDn_ext_new = nand2(reg_new.cpu_signals.TEDO_CPU_RDp.out_new(), FFFF_HIT_ext_new);
  wire FFFF_WRn_ext_new = nand2(reg_new.cpu_signals.TAPU_CPU_WRp.out_new(), FFFF_HIT_ext_new);

  reg_new.reg_ie.IE_D0.dff_r(FFFF_WRn_ext_new, ~pins.sys.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  reg_new.reg_ie.IE_D1.dff_r(FFFF_WRn_ext_new, ~pins.sys.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  reg_new.reg_ie.IE_D2.dff_r(FFFF_WRn_ext_new, ~pins.sys.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  reg_new.reg_ie.IE_D3.dff_r(FFFF_WRn_ext_new, ~pins.sys.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  reg_new.reg_ie.IE_D4.dff_r(FFFF_WRn_ext_new, ~pins.sys.PIN_71_RST.qp_int_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());

  /*_p21.SEPA*/ wire SEPA_FF41_WRp_new = and2(reg_new.cpu_signals.CUPA_CPU_WRp_new(), reg_new.cpu_abus.VARY_FF41p_new());
  /*_p21.RYVE*/ wire RYVE_FF41_WRn_new = not1(SEPA_FF41_WRp_new);

  /*_p21.ROXE*/ reg_new.reg_stat.ROXE_STAT_HBI_ENn.dff9(RYVE_FF41_WRn_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p21.RUFO*/ reg_new.reg_stat.RUFO_STAT_VBI_ENn.dff9(RYVE_FF41_WRn_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p21.REFE*/ reg_new.reg_stat.REFE_STAT_OAI_ENn.dff9(RYVE_FF41_WRn_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p21.RUGU*/ reg_new.reg_stat.RUGU_STAT_LYI_ENn.dff9(RYVE_FF41_WRn_new, reg_new.sys_rst.WESY_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());

  /*_p21.TOBE*/ wire TOBE_FF41_RDp_new = and2(reg_new.cpu_signals.ASOT_CPU_RDp_new(), reg_new.cpu_abus.VARY_FF41p_new());
  /*_p21.VAVE*/ wire VAVE_FF41_RDn_new = not1(TOBE_FF41_RDp_new);

  /*#p21.PARU*/ wire PARU_VBLANKp_odd_new = not1(reg_new.lcd.POPU_VBLANKp_odd.qn_new());
  /*#p21.SADU*/ wire SADU_STAT_MODE0n_new = nor2(reg_new.XYMU_RENDERING_LATCHn.qn_new(), PARU_VBLANKp_odd_new);   // die NOR
  /*#p21.XATY*/ wire XATY_STAT_MODE1n_new = nor2(reg_new.ACYL_SCANNINGp_odd.out_new(), reg_new.XYMU_RENDERING_LATCHn.qn_new()); // die NOR

  /*#p21.TEBY*/ triwire TEBY_STAT0_TO_CD0_new = tri6_pn(TOBE_FF41_RDp_new, SADU_STAT_MODE0n_new);
  /*#p21.WUGA*/ triwire WUGA_STAT1_TO_CD1_new = tri6_pn(TOBE_FF41_RDp_new, XATY_STAT_MODE1n_new);
  /*#p21.SEGO*/ triwire SEGO_STAT2_TO_CD2_new = tri6_pn(TOBE_FF41_RDp_new, reg_new.int_ctrl.RUPO_LYC_MATCHn.qp_new());
  /*_p21.PUZO*/ triwire PUZO_STAT3_TO_CD3_new = tri6_nn(VAVE_FF41_RDn_new, reg_new.reg_stat.ROXE_STAT_HBI_ENn.qp_new());
  /*_p21.POFO*/ triwire POFO_STAT4_TO_CD4_new = tri6_nn(VAVE_FF41_RDn_new, reg_new.reg_stat.RUFO_STAT_VBI_ENn.qp_new());
  /*_p21.SASY*/ triwire SASY_STAT5_TO_CD5_new = tri6_nn(VAVE_FF41_RDn_new, reg_new.reg_stat.REFE_STAT_OAI_ENn.qp_new());
  /*_p21.POTE*/ triwire POTE_STAT6_TO_CD6_new = tri6_nn(VAVE_FF41_RDn_new, reg_new.reg_stat.RUGU_STAT_LYI_ENn.qp_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(TEBY_STAT0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(WUGA_STAT1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(SEGO_STAT2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(PUZO_STAT3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(POFO_STAT4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(SASY_STAT5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(POTE_STAT6_TO_CD6_new);

  /*#p21.PURE*/ wire PURE_LINE_ENDn_new = not1(reg_new.lcd.RUTU_LINE_ENDp_odd.qp_new());
  /*#p21.TOLU*/ wire TOLU_VBLANKn_new   = not1(PARU_VBLANKp_odd_new);
  /*#p21.SELA*/ wire SELA_LINE_ENDp_new = not1(PURE_LINE_ENDn_new);
  /*#p21.TAPA*/ wire TAPA_INT_OAM_new   = and2(TOLU_VBLANKn_new, SELA_LINE_ENDp_new);
  /*#p21.TARU*/ wire TARU_INT_HBL_new   = and2(reg_new.WODU_HBLANK_GATEp_odd.out_new(), TOLU_VBLANKn_new);
  /*#p21.SUKO*/ wire SUKO_INT_STATp_new = amux4(
    reg_new.reg_stat.RUGU_STAT_LYI_ENn.qn_new(), reg_new.int_ctrl.ROPO_LY_MATCH_SYNCp.qp_new(),
    reg_new.reg_stat.REFE_STAT_OAI_ENn.qn_new(), TAPA_INT_OAM_new,
    reg_new.reg_stat.RUFO_STAT_VBI_ENn.qn_new(), PARU_VBLANKp_odd_new,
    reg_new.reg_stat.ROXE_STAT_HBI_ENn.qn_new(), TARU_INT_HBL_new);

  /*#p21.VYPU*/ wire VYPU_INT_VBLANKp_new = not1(TOLU_VBLANKn_new);
  /*#p21.TUVA*/ wire TUVA_INT_STATn_new   = not1(SUKO_INT_STATp_new);
  /*#p21.VOTY*/ wire VOTY_INT_STATp_new   = not1(TUVA_INT_STATn_new);

  /*#p02.ASOK*/ wire ASOK_INT_JOYp_new = nand2(reg_new.joy_int.APUG_JP_GLITCH3.qp_new(), reg_new.joy_int.BATU_JP_GLITCH0.qp_new());

  /*#p01.ALUR*/ wire ALUR_SYS_RSTn_new = not1(reg_new.sys_rst.AVOR_SYS_RSTp_new());
  /*_p07.REFA*/ wire REFA_FF0F_WRn_new = nand4(
    reg_new.cpu_signals.TAPU_CPU_WRp.out_new(),
    reg_new.cpu_abus.SYKE_ADDR_HIp_new(),
    reg_new.cpu_abus.SEMY_XX_0000xxxxp_new(),
    reg_new.cpu_abus.SAPA_XX_xxxx1111p_new()); // schematic wrong, is NAND

  /*_p02.LETY*/ wire LETY_INT_VBL_ACKn_new  = not1(reg_new.cpu_ack.SIG_CPU_ACK_VBLANK.out_new());
  /*_p02.LEJA*/ wire LEJA_INT_STAT_ACKn_new = not1(reg_new.cpu_ack.SIG_CPU_ACK_STAT.out_new());
  /*_p02.LESA*/ wire LESA_INT_TIM_ACKn_new  = not1(reg_new.cpu_ack.SIG_CPU_ACK_TIMER.out_new());
  /*_p02.LUFE*/ wire LUFE_INT_SER_ACKn_new  = not1(reg_new.cpu_ack.SIG_CPU_ACK_SERIAL.out_new());
  /*_p02.LAMO*/ wire LAMO_INT_JOY_ACKn_new  = not1(reg_new.cpu_ack.SIG_CPU_ACK_JOYPAD.out_new());

  /*_p02.ROTU*/ wire ROTU_FF0F_WRp   = not1(REFA_FF0F_WRn_new);
  /*_p02.MYZU*/ wire MYZU_FF0F_SET0n = nand3(ROTU_FF0F_WRp, LETY_INT_VBL_ACKn_new,  reg_new.cpu_dbus.BUS_CPU_D00p.out_new());
  /*_p02.MODY*/ wire MODY_FF0F_SET1n = nand3(ROTU_FF0F_WRp, LEJA_INT_STAT_ACKn_new, reg_new.cpu_dbus.BUS_CPU_D01p.out_new());
  /*_p02.PYHU*/ wire PYHU_FF0F_SET2n = nand3(ROTU_FF0F_WRp, LESA_INT_TIM_ACKn_new,  reg_new.cpu_dbus.BUS_CPU_D02p.out_new());
  /*_p02.TOME*/ wire TOME_FF0F_SET3n = nand3(ROTU_FF0F_WRp, LUFE_INT_SER_ACKn_new,  reg_new.cpu_dbus.BUS_CPU_D03p.out_new());
  /*_p02.TOGA*/ wire TOGA_FF0F_SET4n = nand3(ROTU_FF0F_WRp, LAMO_INT_JOY_ACKn_new,  reg_new.cpu_dbus.BUS_CPU_D04p.out_new());

  /*#p02.MUXE*/ wire MUXE_INT0_WRn_new = or2(reg_new.cpu_dbus.BUS_CPU_D00p.out_new(), REFA_FF0F_WRn_new);
  /*_p02.NABE*/ wire NABE_INT1_WRn_new = or2(reg_new.cpu_dbus.BUS_CPU_D01p.out_new(), REFA_FF0F_WRn_new);
  /*_p02.RAKE*/ wire RAKE_INT2_WRn_new = or2(reg_new.cpu_dbus.BUS_CPU_D02p.out_new(), REFA_FF0F_WRn_new);
  /*_p02.SULO*/ wire SULO_INT3_WRn_new = or2(reg_new.cpu_dbus.BUS_CPU_D03p.out_new(), REFA_FF0F_WRn_new);
  /*_p02.SEME*/ wire SEME_INT4_WRn_new = or2(reg_new.cpu_dbus.BUS_CPU_D04p.out_new(), REFA_FF0F_WRn_new);

  /*#p02.LYTA*/ wire LYTA_FF0F_RST0n_new = and3(MUXE_INT0_WRn_new, LETY_INT_VBL_ACKn_new,  ALUR_SYS_RSTn_new);
  /*_p02.MOVU*/ wire MOVU_FF0F_RST1n_new = and3(NABE_INT1_WRn_new, LEJA_INT_STAT_ACKn_new, ALUR_SYS_RSTn_new);
  /*_p02.PYGA*/ wire PYGA_FF0F_RST2n_new = and3(RAKE_INT2_WRn_new, LESA_INT_TIM_ACKn_new,  ALUR_SYS_RSTn_new);
  /*_p02.TUNY*/ wire TUNY_FF0F_RST3n_new = and3(SULO_INT3_WRn_new, LUFE_INT_SER_ACKn_new,  ALUR_SYS_RSTn_new);
  /*_p02.TYME*/ wire TYME_FF0F_RST4n_new = and3(SEME_INT4_WRn_new, LAMO_INT_JOY_ACKn_new,  ALUR_SYS_RSTn_new);

  /*_p02.LOPE*/ reg_new.reg_if.LOPE_FF0F_D0p.dff22(VYPU_INT_VBLANKp_new,                           MYZU_FF0F_SET0n, LYTA_FF0F_RST0n_new, reg_old.SIG_VCC.out_old());
  /*_p02.LALU*/ reg_new.reg_if.LALU_FF0F_D1p.dff22(VOTY_INT_STATp_new,                             MODY_FF0F_SET1n, MOVU_FF0F_RST1n_new, reg_old.SIG_VCC.out_old());
  /*_p02.NYBO*/ reg_new.reg_if.NYBO_FF0F_D2p.dff22(reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp.qp_new(), PYHU_FF0F_SET2n, PYGA_FF0F_RST2n_new, reg_old.SIG_VCC.out_old());

  ///*_p02.UBUL*/ reg_if.UBUL_FF0F_D3p.dff22(serial.CALY_SER_CNT3.qp_new(), TOME_FF0F_SET3n, TUNY_FF0F_RST3n, SIG_VCC.out_new());

  /*_p02.UBUL*/ reg_new.reg_if.UBUL_FF0F_D3p.dff22(0,                 TOME_FF0F_SET3n, TUNY_FF0F_RST3n_new, reg_old.SIG_VCC.out_old());
  /*_p02.ULAK*/ reg_new.reg_if.ULAK_FF0F_D4p.dff22(ASOK_INT_JOYp_new, TOGA_FF0F_SET4n, TYME_FF0F_RST4n_new, reg_old.SIG_VCC.out_old());

  /*_SIG_CPU_INT_VBLANK*/ reg_new.cpu_int.SIG_CPU_INT_VBLANK.sig_out(reg_new.reg_if.LOPE_FF0F_D0p.qp_new());
  /*_SIG_CPU_INT_STAT  */ reg_new.cpu_int.SIG_CPU_INT_STAT  .sig_out(reg_new.reg_if.LALU_FF0F_D1p.qp_new());
  /*_SIG_CPU_INT_TIMER */ reg_new.cpu_int.SIG_CPU_INT_TIMER .sig_out(reg_new.reg_if.NYBO_FF0F_D2p.qp_new());
  /*_SIG_CPU_INT_SERIAL*/ reg_new.cpu_int.SIG_CPU_INT_SERIAL.sig_out(reg_new.reg_if.UBUL_FF0F_D3p.qp_new());
  /*_SIG_CPU_INT_JOYPAD*/ reg_new.cpu_int.SIG_CPU_INT_JOYPAD.sig_out(reg_new.reg_if.ULAK_FF0F_D4p.qp_new());

  // FIXME where did these tags go?
  triwire tristate_i0_new = tri6_nn(FFFF_RDn_ext_new, reg_new.reg_ie.IE_D0.qn_new());
  triwire tristate_i1_new = tri6_nn(FFFF_RDn_ext_new, reg_new.reg_ie.IE_D1.qn_new());
  triwire tristate_i2_new = tri6_nn(FFFF_RDn_ext_new, reg_new.reg_ie.IE_D2.qn_new());
  triwire tristate_i3_new = tri6_nn(FFFF_RDn_ext_new, reg_new.reg_ie.IE_D3.qn_new());
  triwire tristate_i4_new = tri6_nn(FFFF_RDn_ext_new, reg_new.reg_ie.IE_D4.qn_new());

  reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(tristate_i0_new);
  reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(tristate_i1_new);
  reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(tristate_i2_new);
  reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(tristate_i3_new);
  reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(tristate_i4_new);

  /*_p07.ROLO*/ wire ROLO_FF0F_RDn = nand4(
    reg_new.cpu_signals.TEDO_CPU_RDp.out_new(),
    reg_new.cpu_abus.SYKE_ADDR_HIp_new(),
    reg_new.cpu_abus.SEMY_XX_0000xxxxp_new(),
    reg_new.cpu_abus.SAPA_XX_xxxx1111p_new()); // schematic wrong, is NAND
  /*_p02.POLA*/ wire POLA_FF0F_RDp = not1(ROLO_FF0F_RDn);

  // FIXME why is this latch different from the others? MATY is one of those big yellow latchy things.

  /*_p02.MATY*/ reg_new.int_latch.MATY_FF0F_L0p.tp_latchp(ROLO_FF0F_RDn, reg_new.reg_if.LOPE_FF0F_D0p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.MOPO*/ reg_new.int_latch.MOPO_FF0F_L1p.tp_latchp(ROLO_FF0F_RDn, reg_new.reg_if.LALU_FF0F_D1p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.PAVY*/ reg_new.int_latch.PAVY_FF0F_L2p.tp_latchp(ROLO_FF0F_RDn, reg_new.reg_if.NYBO_FF0F_D2p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.NEJY*/ reg_new.int_latch.NEJY_FF0F_L3p.tp_latchp(ROLO_FF0F_RDn, reg_new.reg_if.UBUL_FF0F_D3p.qp_new()); // OUTPUT ON RUNG 10
  /*_p02.NUTY*/ reg_new.int_latch.NUTY_FF0F_L4p.tp_latchp(ROLO_FF0F_RDn, reg_new.reg_if.ULAK_FF0F_D4p.qp_new()); // OUTPUT ON RUNG 10

  /*#p02.NELA*/ triwire NELA_IF0_TO_CD0_new = tri6_pn(POLA_FF0F_RDp, reg_new.int_latch.MATY_FF0F_L0p.qn_new());
  /*#p02.NABO*/ triwire NABO_IF1_TO_CD1_new = tri6_pn(POLA_FF0F_RDp, reg_new.int_latch.MOPO_FF0F_L1p.qn_new());
  /*#p02.ROVA*/ triwire ROVA_IF2_TO_CD2_new = tri6_pn(POLA_FF0F_RDp, reg_new.int_latch.PAVY_FF0F_L2p.qn_new());
  /*#p02.PADO*/ triwire PADO_IF3_TO_CD3_new = tri6_pn(POLA_FF0F_RDp, reg_new.int_latch.NEJY_FF0F_L3p.qn_new());
  /*#p02.PEGY*/ triwire PEGY_IF4_TO_CD4_new = tri6_pn(POLA_FF0F_RDp, reg_new.int_latch.NUTY_FF0F_L4p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(NELA_IF0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(NABO_IF1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(ROVA_IF2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(PADO_IF3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(PEGY_IF4_TO_CD4_new);
}

//-----------------------------------------------------------------------------

void RegIF::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void RegIF::reset_to_bootrom() {
  LOPE_FF0F_D0p.state = 0b00011000;
  LALU_FF0F_D1p.state = 0b00011000;
  NYBO_FF0F_D2p.state = 0b00011000;
  UBUL_FF0F_D3p.state = 0b00011000;
  ULAK_FF0F_D4p.state = 0b00011010;
}

void RegIF::reset_to_cart() {
  LOPE_FF0F_D0p.state = 0b00011011;
  LALU_FF0F_D1p.state = 0b00011000;
  NYBO_FF0F_D2p.state = 0b00011000;
  UBUL_FF0F_D3p.state = 0b00011000;
  ULAK_FF0F_D4p.state = 0b00011000;
}

//-----------------------------------------------------------------------------
// This is technically in the CPU, but we're going to implement it here for now.

void RegIE::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void RegIE::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

void RegIE::reset_to_cart() {
  IE_D0.state = 0b00011010;
  IE_D1.state = 0b00011010;
  IE_D2.state = 0b00011010;
  IE_D3.state = 0b00011010;
  IE_D4.state = 0b00011010;
}

//-----------------------------------------------------------------------------

void InterruptLatch::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void InterruptLatch::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void InterruptLatch::reset_to_cart() {
  MATY_FF0F_L0p.state = 0b00011000;
  MOPO_FF0F_L1p.state = 0b00011000;
  PAVY_FF0F_L2p.state = 0b00011000;
  NEJY_FF0F_L3p.state = 0b00011000;
  NUTY_FF0F_L4p.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void CpuInt::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void CpuInt::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void CpuInt::reset_to_cart() {
  SIG_CPU_INT_VBLANK.state = 0b00011001;
  SIG_CPU_INT_STAT.state   = 0b00011000;
  SIG_CPU_INT_TIMER.state  = 0b00011000;
  SIG_CPU_INT_SERIAL.state = 0b00011000;
  SIG_CPU_INT_JOYPAD.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void CpuAck::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void CpuAck::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void CpuAck::reset_to_cart() {
  SIG_CPU_ACK_VBLANK.state = 0b00011000;
  SIG_CPU_ACK_STAT.state   = 0b00011000;
  SIG_CPU_ACK_TIMER.state  = 0b00011000;
  SIG_CPU_ACK_SERIAL.state = 0b00011000;
  SIG_CPU_ACK_JOYPAD.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void InterruptControl::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void InterruptControl::reset_to_bootrom() {
  AWOB_WAKE_CPU.state        = 0b00011001;
  SIG_CPU_WAKE.state         = 0b00011001;
  NYDU_TIMA7p_DELAY.state    = 0b00011000;
  MOBA_TIMER_OVERFLOWp.state = 0b00011000;
  RUPO_LYC_MATCHn.state      = 0b00011001;
  ROPO_LY_MATCH_SYNCp.state  = 0b00011000;
}

void InterruptControl::reset_to_cart() {
  AWOB_WAKE_CPU.state        = 0b00011001;
  SIG_CPU_WAKE.state         = 0b00011001;
  NYDU_TIMA7p_DELAY.state    = 0b00011000;
  MOBA_TIMER_OVERFLOWp.state = 0b00011000;
  RUPO_LYC_MATCHn.state      = 0b00011000;
  ROPO_LY_MATCH_SYNCp.state  = 0b00011001;
}

//-----------------------------------------------------------------------------

