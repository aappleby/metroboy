#include "GateBoyLib/GateBoyInterrupts.h"

#include "CoreLib/Constants.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

// Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
// Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
// Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
// Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
// Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

void GateBoy::tock_interrupts()
{
  uint16_t cpu_addr = (uint16_t)pack_new(16, (BitBase*)&new_bus.BUS_CPU_A00p);
  wire FFFF_HIT_ext = cpu_addr == 0xFFFF;
  wire FFFF_RDn_ext = nand2(cpu_signals.TEDO_CPU_RDp.out_new(), FFFF_HIT_ext);
  wire FFFF_WRn_ext = nand2(cpu_signals.TAPU_CPU_WRp.out_new(), FFFF_HIT_ext);

  interrupts.IE_D0.dff_r(FFFF_WRn_ext, ~rst.PIN_71_RST.qp_int_new(), old_bus.BUS_CPU_D00p.out_old());
  interrupts.IE_D1.dff_r(FFFF_WRn_ext, ~rst.PIN_71_RST.qp_int_new(), old_bus.BUS_CPU_D01p.out_old());
  interrupts.IE_D2.dff_r(FFFF_WRn_ext, ~rst.PIN_71_RST.qp_int_new(), old_bus.BUS_CPU_D02p.out_old());
  interrupts.IE_D3.dff_r(FFFF_WRn_ext, ~rst.PIN_71_RST.qp_int_new(), old_bus.BUS_CPU_D03p.out_old());
  interrupts.IE_D4.dff_r(FFFF_WRn_ext, ~rst.PIN_71_RST.qp_int_new(), old_bus.BUS_CPU_D04p.out_old());

  /* p21.SEPA*/ wire SEPA_FF41_WRp = and2(CUPA_CPU_WRp(), new_bus.VARY_FF41p());
  /* p21.RYVE*/ wire RYVE_FF41_WRn = not1(SEPA_FF41_WRp);

  /* p21.ROXE*/ reg_stat.ROXE_STAT_HBI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D03p.out_old());
  /* p21.RUFO*/ reg_stat.RUFO_STAT_VBI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D04p.out_old());
  /* p21.REFE*/ reg_stat.REFE_STAT_OAI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D05p.out_old());
  /* p21.RUGU*/ reg_stat.RUGU_STAT_LYI_ENn.dff9(RYVE_FF41_WRn, WESY_SYS_RSTn(), old_bus.BUS_CPU_D06p.out_old());

  /* p21.TOBE*/ wire TOBE_FF41_RDp = and2(ASOT_CPU_RDp(), new_bus.VARY_FF41p());
  /* p21.VAVE*/ wire VAVE_FF41_RDn = not1(TOBE_FF41_RDp);

  /*#p21.PARU*/ wire PARU_VBLANKp = not1(lcd.POPU_y144p.qn_new());
  /*#p21.SADU*/ wire SADU_STAT_MODE0n = nor2(XYMU_RENDERINGn.qn_new(), PARU_VBLANKp);   // die NOR
  /*#p21.XATY*/ wire XATY_STAT_MODE1n = nor2(sprite_scanner.ACYL_SCANNINGp.out_new(), XYMU_RENDERINGn.qn_new()); // die NOR

  probe_wire(24, "POPU VBLANK", lcd.POPU_y144p.qp_new());
  probe_wire(25, "XYMU RENDER", ~XYMU_RENDERINGn.qp_new());
  probe_wire(26, "ACYL SCAN",   sprite_scanner.ACYL_SCANNINGp.out_new());
  probe_wire(27, "SADU STAT0",  ~SADU_STAT_MODE0n);
  probe_wire(28, "XATY STAT1",  ~XATY_STAT_MODE1n);
  probe_wire(29, "RUPO STAT2",  ~reg_stat.RUPO_LYC_MATCHn.qp_new());
  probe_wire(30, "MUWY LY0",    reg_ly.MUWY_LY0p.qp_new());
  probe_wire(31, "SYRY LYC0",   reg_lyc.SYRY_LYC0n.qn_new());

  /*#p21.TEBY*/ triwire TEBY_STAT0_TO_CD0 = tri6_pn(TOBE_FF41_RDp, SADU_STAT_MODE0n);
  /*#p21.WUGA*/ triwire WUGA_STAT1_TO_CD1 = tri6_pn(TOBE_FF41_RDp, XATY_STAT_MODE1n);
  /*#p21.SEGO*/ triwire SEGO_STAT2_TO_CD2 = tri6_pn(TOBE_FF41_RDp, reg_stat.RUPO_LYC_MATCHn.qp_new());
  /* p21.PUZO*/ triwire PUZO_STAT3_TO_CD3 = tri6_nn(VAVE_FF41_RDn, reg_stat.ROXE_STAT_HBI_ENn.qp_new());
  /* p21.POFO*/ triwire POFO_STAT4_TO_CD4 = tri6_nn(VAVE_FF41_RDn, reg_stat.RUFO_STAT_VBI_ENn.qp_new());
  /* p21.SASY*/ triwire SASY_STAT5_TO_CD5 = tri6_nn(VAVE_FF41_RDn, reg_stat.REFE_STAT_OAI_ENn.qp_new());
  /* p21.POTE*/ triwire POTE_STAT6_TO_CD6 = tri6_nn(VAVE_FF41_RDn, reg_stat.RUGU_STAT_LYI_ENn.qp_new());

  /* BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(TEBY_STAT0_TO_CD0);
  /* BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(WUGA_STAT1_TO_CD1);
  /* BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(SEGO_STAT2_TO_CD2);
  /* BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(PUZO_STAT3_TO_CD3);
  /* BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(POFO_STAT4_TO_CD4);
  /* BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(SASY_STAT5_TO_CD5);
  /* BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(POTE_STAT6_TO_CD6);

  /*#p21.PURE*/ wire PURE_x113n = not1(lcd.RUTU_x113p.qp_new());
  /*#p21.TOLU*/ wire TOLU_VBLANKn   = not1(PARU_VBLANKp);
  /*#p21.SELA*/ wire SELA_x113p = not1(PURE_x113n);
  /*#p21.TAPA*/ wire TAPA_INT_OAM   = and2(TOLU_VBLANKn, SELA_x113p);
  /*#p21.TARU*/ wire TARU_INT_HBL   = and2(WODU_HBLANKp.out_new(), TOLU_VBLANKn);
  /*#p21.SUKO*/ wire SUKO_INT_STATp = amux4(reg_stat.RUGU_STAT_LYI_ENn.qn_new(), reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new(), reg_stat.REFE_STAT_OAI_ENn.qn_new(), TAPA_INT_OAM, reg_stat.RUFO_STAT_VBI_ENn.qn_new(), PARU_VBLANKp, reg_stat.ROXE_STAT_HBI_ENn.qn_new(), TARU_INT_HBL);

  /*#p21.VYPU*/ wire VYPU_INT_VBLANKp = not1(TOLU_VBLANKn);
  /*#p21.TUVA*/ wire TUVA_INT_STATn   = not1(SUKO_INT_STATp);
  /*#p21.VOTY*/ wire VOTY_INT_STATp   = not1(TUVA_INT_STATn);

  /*#p02.ASOK*/ wire ASOK_INT_JOYp = nand2(joy.APUG_JP_GLITCH3.qp_new(), joy.BATU_JP_GLITCH0.qp_new());

  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp());
  /* p07.REFA*/ wire REFA_FF0F_WRn = nand4(cpu_signals.TAPU_CPU_WRp.out_new(), new_bus.SYKE_ADDR_HIp(), new_bus.SEMY_XX_0000xxxxp(), new_bus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND

  /* p02.LETY*/ wire LETY_INT_VBL_ACKn  = not1(interrupts.SIG_CPU_ACK_VBLANK.out_new());
  /* p02.LEJA*/ wire LEJA_INT_STAT_ACKn = not1(interrupts.SIG_CPU_ACK_STAT.out_new());
  /* p02.LESA*/ wire LESA_INT_TIM_ACKn  = not1(interrupts.SIG_CPU_ACK_TIMER.out_new());
  /* p02.LUFE*/ wire LUFE_INT_SER_ACKn  = not1(interrupts.SIG_CPU_ACK_SERIAL.out_new());
  /* p02.LAMO*/ wire LAMO_INT_JOY_ACKn  = not1(interrupts.SIG_CPU_ACK_JOYPAD.out_new());

  /* p02.ROTU*/ wire ROTU_FF0F_WRp   = not1(REFA_FF0F_WRn);
  /* p02.MYZU*/ wire MYZU_FF0F_SET0n = nand3(ROTU_FF0F_WRp, LETY_INT_VBL_ACKn,  new_bus.BUS_CPU_D00p.out_new());
  /* p02.MODY*/ wire MODY_FF0F_SET1n = nand3(ROTU_FF0F_WRp, LEJA_INT_STAT_ACKn, new_bus.BUS_CPU_D01p.out_new());
  /* p02.PYHU*/ wire PYHU_FF0F_SET2n = nand3(ROTU_FF0F_WRp, LESA_INT_TIM_ACKn,  new_bus.BUS_CPU_D02p.out_new());
  /* p02.TOME*/ wire TOME_FF0F_SET3n = nand3(ROTU_FF0F_WRp, LUFE_INT_SER_ACKn,  new_bus.BUS_CPU_D03p.out_new());
  /* p02.TOGA*/ wire TOGA_FF0F_SET4n = nand3(ROTU_FF0F_WRp, LAMO_INT_JOY_ACKn,  new_bus.BUS_CPU_D04p.out_new());

  /*#p02.MUXE*/ wire MUXE_INT0_WRn = or2(new_bus.BUS_CPU_D00p.out_new(), REFA_FF0F_WRn);
  /* p02.NABE*/ wire NABE_INT1_WRn = or2(new_bus.BUS_CPU_D01p.out_new(), REFA_FF0F_WRn);
  /* p02.RAKE*/ wire RAKE_INT2_WRn = or2(new_bus.BUS_CPU_D02p.out_new(), REFA_FF0F_WRn);
  /* p02.SULO*/ wire SULO_INT3_WRn = or2(new_bus.BUS_CPU_D03p.out_new(), REFA_FF0F_WRn);
  /* p02.SEME*/ wire SEME_INT4_WRn = or2(new_bus.BUS_CPU_D04p.out_new(), REFA_FF0F_WRn);

  /*#p02.LYTA*/ wire LYTA_FF0F_RST0n = and3(MUXE_INT0_WRn, LETY_INT_VBL_ACKn,  ALUR_SYS_RSTn);
  /* p02.MOVU*/ wire MOVU_FF0F_RST1n = and3(NABE_INT1_WRn, LEJA_INT_STAT_ACKn, ALUR_SYS_RSTn);
  /* p02.PYGA*/ wire PYGA_FF0F_RST2n = and3(RAKE_INT2_WRn, LESA_INT_TIM_ACKn,  ALUR_SYS_RSTn);
  /* p02.TUNY*/ wire TUNY_FF0F_RST3n = and3(SULO_INT3_WRn, LUFE_INT_SER_ACKn,  ALUR_SYS_RSTn);
  /* p02.TYME*/ wire TYME_FF0F_RST4n = and3(SEME_INT4_WRn, LAMO_INT_JOY_ACKn,  ALUR_SYS_RSTn);

  /* p02.LOPE*/ interrupts.LOPE_FF0F_D0p.dff22(VYPU_INT_VBLANKp,                    MYZU_FF0F_SET0n, LYTA_FF0F_RST0n, SIG_VCC.out_new());
  /* p02.LALU*/ interrupts.LALU_FF0F_D1p.dff22(VOTY_INT_STATp,                      MODY_FF0F_SET1n, MOVU_FF0F_RST1n, SIG_VCC.out_new());
  /* p02.NYBO*/ interrupts.NYBO_FF0F_D2p.dff22(timer.MOBA_TIMER_OVERFLOWp.qp_new(), PYHU_FF0F_SET2n, PYGA_FF0F_RST2n, SIG_VCC.out_new());
  /* p02.UBUL*/ interrupts.UBUL_FF0F_D3p.dff22(serial.CALY_SER_CNT3.qp_new(),       TOME_FF0F_SET3n, TUNY_FF0F_RST3n, SIG_VCC.out_new());
  /* p02.ULAK*/ interrupts.ULAK_FF0F_D4p.dff22(ASOK_INT_JOYp,                       TOGA_FF0F_SET4n, TYME_FF0F_RST4n, SIG_VCC.out_new());

  /*SIG_CPU_INT_VBLANK*/ interrupts.SIG_CPU_INT_VBLANK.sig_out(interrupts.LOPE_FF0F_D0p.qp_new());
  /*SIG_CPU_INT_STAT  */ interrupts.SIG_CPU_INT_STAT  .sig_out(interrupts.LALU_FF0F_D1p.qp_new());
  /*SIG_CPU_INT_TIMER */ interrupts.SIG_CPU_INT_TIMER .sig_out(interrupts.NYBO_FF0F_D2p.qp_new());
  /*SIG_CPU_INT_SERIAL*/ interrupts.SIG_CPU_INT_SERIAL.sig_out(interrupts.UBUL_FF0F_D3p.qp_new());
  /*SIG_CPU_INT_JOYPAD*/ interrupts.SIG_CPU_INT_JOYPAD.sig_out(interrupts.ULAK_FF0F_D4p.qp_new());

  // FIXME where did these tags go?
  triwire tristate_i0 = tri6_nn(FFFF_RDn_ext, interrupts.IE_D0.qn_new());
  triwire tristate_i1 = tri6_nn(FFFF_RDn_ext, interrupts.IE_D1.qn_new());
  triwire tristate_i2 = tri6_nn(FFFF_RDn_ext, interrupts.IE_D2.qn_new());
  triwire tristate_i3 = tri6_nn(FFFF_RDn_ext, interrupts.IE_D3.qn_new());
  triwire tristate_i4 = tri6_nn(FFFF_RDn_ext, interrupts.IE_D4.qn_new());

  new_bus.BUS_CPU_D00p.tri_bus(tristate_i0);
  new_bus.BUS_CPU_D01p.tri_bus(tristate_i1);
  new_bus.BUS_CPU_D02p.tri_bus(tristate_i2);
  new_bus.BUS_CPU_D03p.tri_bus(tristate_i3);
  new_bus.BUS_CPU_D04p.tri_bus(tristate_i4);

  /* p07.ROLO*/ wire ROLO_FF0F_RDn = nand4(cpu_signals.TEDO_CPU_RDp.out_new(), new_bus.SYKE_ADDR_HIp(), new_bus.SEMY_XX_0000xxxxp(), new_bus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND
  /* p02.POLA*/ wire POLA_FF0F_RDp = not1(ROLO_FF0F_RDn);

  // FIXME why is this latch different from the others? MATY is one of those big yellow latchy things.

  /* p02.MATY*/ interrupts.MATY_FF0F_L0p.tp_latchp(ROLO_FF0F_RDn, interrupts.LOPE_FF0F_D0p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.MOPO*/ interrupts.MOPO_FF0F_L1p.tp_latchp(ROLO_FF0F_RDn, interrupts.LALU_FF0F_D1p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.PAVY*/ interrupts.PAVY_FF0F_L2p.tp_latchp(ROLO_FF0F_RDn, interrupts.NYBO_FF0F_D2p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NEJY*/ interrupts.NEJY_FF0F_L3p.tp_latchp(ROLO_FF0F_RDn, interrupts.UBUL_FF0F_D3p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NUTY*/ interrupts.NUTY_FF0F_L4p.tp_latchp(ROLO_FF0F_RDn, interrupts.ULAK_FF0F_D4p.qp_new()); // OUTPUT ON RUNG 10

  /*#p02.NELA_IF0_TO_CD0*/ triwire NELA_IF0_TO_CD0 = tri6_pn(POLA_FF0F_RDp, interrupts.MATY_FF0F_L0p.qn_new());
  /*#p02.NABO_IF1_TO_CD1*/ triwire NABO_IF1_TO_CD1 = tri6_pn(POLA_FF0F_RDp, interrupts.MOPO_FF0F_L1p.qn_new());
  /*#p02.ROVA_IF2_TO_CD2*/ triwire ROVA_IF2_TO_CD2 = tri6_pn(POLA_FF0F_RDp, interrupts.PAVY_FF0F_L2p.qn_new());
  /*#p02.PADO_IF3_TO_CD3*/ triwire PADO_IF3_TO_CD3 = tri6_pn(POLA_FF0F_RDp, interrupts.NEJY_FF0F_L3p.qn_new());
  /*#p02.PEGY_IF4_TO_CD4*/ triwire PEGY_IF4_TO_CD4 = tri6_pn(POLA_FF0F_RDp, interrupts.NUTY_FF0F_L4p.qn_new());

  /* BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(NELA_IF0_TO_CD0);
  /* BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(NABO_IF1_TO_CD1);
  /* BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(ROVA_IF2_TO_CD2);
  /* BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(PADO_IF3_TO_CD3);
  /* BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(PEGY_IF4_TO_CD4);
}

//------------------------------------------------------------------------------------------------------------------------
