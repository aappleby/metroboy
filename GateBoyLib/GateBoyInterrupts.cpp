#include "GateBoyLib/GateBoyInterrupts.h"

#include "CoreLib/Constants.h"

#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyJoypad.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyPixPipe.h"
#include "GateBoyLib/GateBoyLCD.h"
#include "GateBoyLib/GateBoySerial.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyInterrupts::read_intf(GateBoyCpuBus& cpu_bus) {
  /* p07.ROLO*/ wire _ROLO_FF0F_RDn = nand4(cpu_bus.TEDO_CPU_RDp, cpu_bus.SYKE_ADDR_HIp(), cpu_bus.SEMY_XX_0000xxxxp(), cpu_bus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND
  /* p02.POLA*/ wire _POLA_FF0F_RDp = not1(_ROLO_FF0F_RDn);

  // FIXME why is this latch different from the others? MATY is one of those big yellow latchy things.

  /* p02.MATY*/ MATY_FF0F_L0p.tp_latchp(_ROLO_FF0F_RDn, LOPE_FF0F_D0p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.MOPO*/ MOPO_FF0F_L1p.tp_latchp(_ROLO_FF0F_RDn, LALU_FF0F_D1p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.PAVY*/ PAVY_FF0F_L2p.tp_latchp(_ROLO_FF0F_RDn, NYBO_FF0F_D2p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NEJY*/ NEJY_FF0F_L3p.tp_latchp(_ROLO_FF0F_RDn, UBUL_FF0F_D3p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NUTY*/ NUTY_FF0F_L4p.tp_latchp(_ROLO_FF0F_RDn, ULAK_FF0F_D4p.qp_new()); // OUTPUT ON RUNG 10

  /*#p02.NELA*/ cpu_bus.BUS_CPU_D00p.tri6_pn(_POLA_FF0F_RDp, MATY_FF0F_L0p.qn_new());
  /*#p02.NABO*/ cpu_bus.BUS_CPU_D01p.tri6_pn(_POLA_FF0F_RDp, MOPO_FF0F_L1p.qn_new());
  /*#p02.ROVA*/ cpu_bus.BUS_CPU_D02p.tri6_pn(_POLA_FF0F_RDp, PAVY_FF0F_L2p.qn_new());
  /*#p02.PADO*/ cpu_bus.BUS_CPU_D03p.tri6_pn(_POLA_FF0F_RDp, NEJY_FF0F_L3p.qn_new());
  /*#p02.PEGY*/ cpu_bus.BUS_CPU_D04p.tri6_pn(_POLA_FF0F_RDp, NUTY_FF0F_L4p.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyInterrupts::read_ie(GateBoyCpuBus& cpu_bus)
{
  uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, &cpu_bus.BUS_CPU_A00p);
  wire FFFF_HIT_ext = cpu_addr == 0xFFFF;
  wire FFFF_RDn_ext = nand2(cpu_bus.TEDO_CPU_RDp, FFFF_HIT_ext);

  cpu_bus.BUS_CPU_D00p.tri6_nn(FFFF_RDn_ext, IE_D0.qn_new());
  cpu_bus.BUS_CPU_D01p.tri6_nn(FFFF_RDn_ext, IE_D1.qn_new());
  cpu_bus.BUS_CPU_D02p.tri6_nn(FFFF_RDn_ext, IE_D2.qn_new());
  cpu_bus.BUS_CPU_D03p.tri6_nn(FFFF_RDn_ext, IE_D3.qn_new());
  cpu_bus.BUS_CPU_D04p.tri6_nn(FFFF_RDn_ext, IE_D4.qn_new());
}

void GateBoyInterrupts::write_ie(const GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, &cpu_bus.BUS_CPU_A00p);
  wire FFFF_HIT_ext = cpu_addr == 0xFFFF;
  wire FFFF_WRn_ext = nand2(cpu_bus.TAPU_CPU_WRp, FFFF_HIT_ext);

  IE_D0.dff_r(FFFF_WRn_ext, rst.PIN_71_RST.qn_new(), cpu_bus.BUS_CPU_D00p.qp_old());
  IE_D1.dff_r(FFFF_WRn_ext, rst.PIN_71_RST.qn_new(), cpu_bus.BUS_CPU_D01p.qp_old());
  IE_D2.dff_r(FFFF_WRn_ext, rst.PIN_71_RST.qn_new(), cpu_bus.BUS_CPU_D02p.qp_old());
  IE_D3.dff_r(FFFF_WRn_ext, rst.PIN_71_RST.qn_new(), cpu_bus.BUS_CPU_D03p.qp_old());
  IE_D4.dff_r(FFFF_WRn_ext, rst.PIN_71_RST.qn_new(), cpu_bus.BUS_CPU_D04p.qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyInterrupts::tock(
  const GateBoyResetDebug& rst,
  GateBoyCpuBus& cpu_bus,
  const GateBoyJoypad& joypad,
  const RegStat& reg_stat,
  const RegLYC& reg_lyc,
  const GateBoySerial& serial,
  wire PARU_VBLANKp,
  wire PURE_LINE_ENDn,
  DFF17 MOBA_TIMER_OVERFLOWp,
  wire WODU_HBLANKp)
{
  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  /*#p21.TOLU*/ wire _TOLU_VBLANKn   = not1(PARU_VBLANKp);
  /*#p21.SELA*/ wire _SELA_LINE_P908p = not1(PURE_LINE_ENDn);
  /*#p21.TAPA*/ wire _TAPA_INT_OAM   = and2(_TOLU_VBLANKn, _SELA_LINE_P908p);
  /*#p21.TARU*/ wire _TARU_INT_HBL   = and2(WODU_HBLANKp, _TOLU_VBLANKn);
  // polarity?
  /*#p21.SUKO*/ wire _SUKO_INT_STATp = amux4(reg_stat.RUGU_STAT_LYI_ENn.qn_new(), reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new(), reg_stat.REFE_STAT_OAI_ENn.qn_new(), _TAPA_INT_OAM, reg_stat.RUFO_STAT_VBI_ENn.qn_new(), PARU_VBLANKp, reg_stat.ROXE_STAT_HBI_ENn.qn_new(), _TARU_INT_HBL);

  /*#p21.VYPU*/ wire _VYPU_INT_VBLANKp = not1(_TOLU_VBLANKn);
  /*#p21.TUVA*/ wire _TUVA_INT_STATn   = not1(_SUKO_INT_STATp);
  /*#p21.VOTY*/ wire _VOTY_INT_STATp   = not1(_TUVA_INT_STATn);


  /*#p01.ALUR*/ wire _ALUR_SYS_RSTn = not1(rst.AVOR_SYS_RSTp());
  /* p07.REFA*/ wire _REFA_FF0F_WRn = nand4(cpu_bus.TAPU_CPU_WRp, cpu_bus.SYKE_ADDR_HIp(), cpu_bus.SEMY_XX_0000xxxxp(), cpu_bus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND

  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  /* p02.LETY*/ wire _LETY_INT_VBL_ACKn  = not1(SIG_CPU_ACK_VBLANK);
  /* p02.LEJA*/ wire _LEJA_INT_STAT_ACKn = not1(SIG_CPU_ACK_STAT);
  /* p02.LESA*/ wire _LESA_INT_TIM_ACKn  = not1(SIG_CPU_ACK_TIMER);
  /* p02.LUFE*/ wire _LUFE_INT_SER_ACKn  = not1(SIG_CPU_ACK_SERIAL);
  /* p02.LAMO*/ wire _LAMO_INT_JOY_ACKn  = not1(SIG_CPU_ACK_JOYPAD);

  /* p02.ROTU*/ wire _ROTU_FF0F_WRp   = not1(_REFA_FF0F_WRn);
  /* p02.MYZU*/ wire _MYZU_FF0F_SET0n = nand3(_ROTU_FF0F_WRp, _LETY_INT_VBL_ACKn,  cpu_bus.BUS_CPU_D00p.qp_new());
  /* p02.MODY*/ wire _MODY_FF0F_SET1n = nand3(_ROTU_FF0F_WRp, _LEJA_INT_STAT_ACKn, cpu_bus.BUS_CPU_D01p.qp_new());
  /* p02.PYHU*/ wire _PYHU_FF0F_SET2n = nand3(_ROTU_FF0F_WRp, _LESA_INT_TIM_ACKn,  cpu_bus.BUS_CPU_D02p.qp_new());
  /* p02.TOME*/ wire _TOME_FF0F_SET3n = nand3(_ROTU_FF0F_WRp, _LUFE_INT_SER_ACKn,  cpu_bus.BUS_CPU_D03p.qp_new());
  /* p02.TOGA*/ wire _TOGA_FF0F_SET4n = nand3(_ROTU_FF0F_WRp, _LAMO_INT_JOY_ACKn,  cpu_bus.BUS_CPU_D04p.qp_new());

  /*#p02.MUXE*/ wire _MUXE_INT0_WRn = or2(cpu_bus.BUS_CPU_D00p.qp_new(), _REFA_FF0F_WRn);
  /* p02.NABE*/ wire _NABE_INT1_WRn = or2(cpu_bus.BUS_CPU_D01p.qp_new(), _REFA_FF0F_WRn);
  /* p02.RAKE*/ wire _RAKE_INT2_WRn = or2(cpu_bus.BUS_CPU_D02p.qp_new(), _REFA_FF0F_WRn);
  /* p02.SULO*/ wire _SULO_INT3_WRn = or2(cpu_bus.BUS_CPU_D03p.qp_new(), _REFA_FF0F_WRn);
  /* p02.SEME*/ wire _SEME_INT4_WRn = or2(cpu_bus.BUS_CPU_D04p.qp_new(), _REFA_FF0F_WRn);

  /*#p02.LYTA*/ wire _LYTA_FF0F_RST0n = and3(_MUXE_INT0_WRn, _LETY_INT_VBL_ACKn,  _ALUR_SYS_RSTn);
  /* p02.MOVU*/ wire _MOVU_FF0F_RST1n = and3(_NABE_INT1_WRn, _LEJA_INT_STAT_ACKn, _ALUR_SYS_RSTn);
  /* p02.PYGA*/ wire _PYGA_FF0F_RST2n = and3(_RAKE_INT2_WRn, _LESA_INT_TIM_ACKn,  _ALUR_SYS_RSTn);
  /* p02.TUNY*/ wire _TUNY_FF0F_RST3n = and3(_SULO_INT3_WRn, _LUFE_INT_SER_ACKn,  _ALUR_SYS_RSTn);
  /* p02.TYME*/ wire _TYME_FF0F_RST4n = and3(_SEME_INT4_WRn, _LAMO_INT_JOY_ACKn,  _ALUR_SYS_RSTn);

  SigIn SIG_VCC = 1;
  /* p02.LOPE*/ LOPE_FF0F_D0p.dff22(_VYPU_INT_VBLANKp,             _MYZU_FF0F_SET0n, _LYTA_FF0F_RST0n, SIG_VCC);
  /* p02.LALU*/ LALU_FF0F_D1p.dff22(_VOTY_INT_STATp,               _MODY_FF0F_SET1n, _MOVU_FF0F_RST1n, SIG_VCC);
  /* p02.NYBO*/ NYBO_FF0F_D2p.dff22(MOBA_TIMER_OVERFLOWp.qp_new(), _PYHU_FF0F_SET2n, _PYGA_FF0F_RST2n, SIG_VCC);
  /* p02.UBUL*/ UBUL_FF0F_D3p.dff22(serial.CALY_SER_CNT3.qp_new(), _TOME_FF0F_SET3n, _TUNY_FF0F_RST3n, SIG_VCC);
  /* p02.ULAK*/ ULAK_FF0F_D4p.dff22(joypad.ASOK_INT_JOYp(),        _TOGA_FF0F_SET4n, _TYME_FF0F_RST4n, SIG_VCC);

  /*SIG_CPU_INT_VBLANK*/ SIG_CPU_INT_VBLANK.sig_out(LOPE_FF0F_D0p.qp_new());
  /*SIG_CPU_INT_STAT  */ SIG_CPU_INT_STAT  .sig_out(LALU_FF0F_D1p.qp_new());
  /*SIG_CPU_INT_TIMER */ SIG_CPU_INT_TIMER .sig_out(NYBO_FF0F_D2p.qp_new());
  /*SIG_CPU_INT_SERIAL*/ SIG_CPU_INT_SERIAL.sig_out(UBUL_FF0F_D3p.qp_new());
  /*SIG_CPU_INT_JOYPAD*/ SIG_CPU_INT_JOYPAD.sig_out(ULAK_FF0F_D4p.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------
