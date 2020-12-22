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
  /* p07.ROLO*/ wire2 _ROLO_FF0F_RDn = nand4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.SYKE_ADDR_HIp(), cpu_bus.SEMY_XX_0000xxxxp(), cpu_bus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND
  /* p02.POLA*/ wire2 _POLA_FF0F_RDp = not1(_ROLO_FF0F_RDn);

  // FIXME this inversion fixes a bunch of tests...
  // MATY is one of those big yellow latchy things
  /* p02.MATY*/ MATY_FF0F_L0p.tp_latch(!_ROLO_FF0F_RDn, LOPE_FF0F_D0p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.MOPO*/ MOPO_FF0F_L1p.tp_latch(!_ROLO_FF0F_RDn, LALU_FF0F_D1p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.PAVY*/ PAVY_FF0F_L2p.tp_latch(!_ROLO_FF0F_RDn, NYBO_FF0F_D2p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NEJY*/ NEJY_FF0F_L3p.tp_latch(!_ROLO_FF0F_RDn, UBUL_FF0F_D3p.qp_new()); // OUTPUT ON RUNG 10
  /* p02.NUTY*/ NUTY_FF0F_L4p.tp_latch(!_ROLO_FF0F_RDn, ULAK_FF0F_D4p.qp_new()); // OUTPUT ON RUNG 10

  /*#p02.NELA*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_POLA_FF0F_RDp, MATY_FF0F_L0p.qn_new());
  /*#p02.NABO*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_POLA_FF0F_RDp, MOPO_FF0F_L1p.qn_new());
  /*#p02.ROVA*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_POLA_FF0F_RDp, PAVY_FF0F_L2p.qn_new());
  /*#p02.PADO*/ cpu_bus.BUS_CPU_D[3].tri6_pn(_POLA_FF0F_RDp, NEJY_FF0F_L3p.qn_new());
  /*#p02.PEGY*/ cpu_bus.BUS_CPU_D[4].tri6_pn(_POLA_FF0F_RDp, NUTY_FF0F_L4p.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyInterrupts::read_ie(GateBoyCpuBus& cpu_bus)
{
  uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, cpu_bus.BUS_CPU_A);
  wire2 FFFF_HIT_ext = cpu_addr == 0xFFFF;
  wire2 FFFF_RDn_ext = nand2(cpu_bus.TEDO_CPU_RDp.qp_new(), FFFF_HIT_ext);

  cpu_bus.BUS_CPU_D[0].tri6_nn(FFFF_RDn_ext, IE_D0.qn_new());
  cpu_bus.BUS_CPU_D[1].tri6_nn(FFFF_RDn_ext, IE_D1.qn_new());
  cpu_bus.BUS_CPU_D[2].tri6_nn(FFFF_RDn_ext, IE_D2.qn_new());
  cpu_bus.BUS_CPU_D[3].tri6_nn(FFFF_RDn_ext, IE_D3.qn_new());
  cpu_bus.BUS_CPU_D[4].tri6_nn(FFFF_RDn_ext, IE_D4.qn_new());
}

void GateBoyInterrupts::write_ie(const GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  uint16_t cpu_addr = (uint16_t)BitBase::pack_new(16, cpu_bus.BUS_CPU_A);
  wire2 FFFF_HIT_ext = cpu_addr == 0xFFFF;
  wire2 FFFF_WRn_ext = nand2(cpu_bus.TAPU_CPU_WRp.qp_new(), FFFF_HIT_ext);

  IE_D0.dff(FFFF_WRn_ext, 1, !rst.PIN71_RST.int_qp_new(), cpu_bus.BUS_CPU_D[0].qp_old());
  IE_D1.dff(FFFF_WRn_ext, 1, !rst.PIN71_RST.int_qp_new(), cpu_bus.BUS_CPU_D[1].qp_old());
  IE_D2.dff(FFFF_WRn_ext, 1, !rst.PIN71_RST.int_qp_new(), cpu_bus.BUS_CPU_D[2].qp_old());
  IE_D3.dff(FFFF_WRn_ext, 1, !rst.PIN71_RST.int_qp_new(), cpu_bus.BUS_CPU_D[3].qp_old());
  IE_D4.dff(FFFF_WRn_ext, 1, !rst.PIN71_RST.int_qp_new(), cpu_bus.BUS_CPU_D[4].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyInterrupts::tock(
  const GateBoyResetDebug& rst,
  GateBoyCpuBus& cpu_bus,
  const GateBoyJoypad& joypad,
  const RegStat& reg_stat,
  const RegLYC& reg_lyc,
  const GateBoySerial& serial,
  wire2 PARU_VBLANKp,
  wire2 PURE_LINE_ENDn,
  wire2 MOBA_TIMER_OVERFLOWp,
  wire2 WODU_HBLANKp)
{
  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  /*#p21.TOLU*/ wire2 _TOLU_VBLANKn   = not1(PARU_VBLANKp);
  /*#p21.SELA*/ wire2 _SELA_LINE_ENDp = not1(PURE_LINE_ENDn);
  /*#p21.TAPA*/ wire2 _TAPA_INT_OAM   = and2(_TOLU_VBLANKn, _SELA_LINE_ENDp);
  /*#p21.TARU*/ wire2 _TARU_INT_HBL   = and2(WODU_HBLANKp, _TOLU_VBLANKn);
  /*#p21.SUKO*/ wire2 _SUKO_INT_STATp = amux4(reg_stat.RUGU_STAT_LYI_ENn.qn_new(), reg_lyc.ROPO_LY_MATCH_SYNCp.qp_new(),
                                             reg_stat.REFE_STAT_OAI_ENn.qn_new(), _TAPA_INT_OAM,
                                             reg_stat.RUFO_STAT_VBI_ENn.qn_new(), PARU_VBLANKp, // polarity?
                                             reg_stat.ROXE_STAT_HBI_ENn.qn_new(), _TARU_INT_HBL);

  /*#p21.VYPU*/ wire2 _VYPU_INT_VBLANKp = not1(_TOLU_VBLANKn);
  /*#p21.TUVA*/ wire2 _TUVA_INT_STATn   = not1(_SUKO_INT_STATp);
  /*#p21.VOTY*/ wire2 _VOTY_INT_STATp   = not1(_TUVA_INT_STATn);


  /*#p01.ALUR*/ wire2 _ALUR_SYS_RSTn = not1(rst.AVOR_SYS_RSTp());
  /* p07.REFA*/ wire2 _REFA_FF0F_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.SYKE_ADDR_HIp(), cpu_bus.SEMY_XX_0000xxxxp(), cpu_bus.SAPA_XX_xxxx1111p()); // schematic wrong, is NAND

  // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
  // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
  // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
  // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
  // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

  /* p02.LETY*/ wire2 _LETY_INT_VBL_ACKn  = not1(SIG_CPU_ACK_VBLANK.qp_new());
  /* p02.LEJA*/ wire2 _LEJA_INT_STAT_ACKn = not1(SIG_CPU_ACK_STAT.qp_new());
  /* p02.LESA*/ wire2 _LESA_INT_TIM_ACKn  = not1(SIG_CPU_ACK_TIMER.qp_new());
  /* p02.LUFE*/ wire2 _LUFE_INT_SER_ACKn  = not1(SIG_CPU_ACK_SERIAL.qp_new());
  /* p02.LAMO*/ wire2 _LAMO_INT_JOY_ACKn  = not1(SIG_CPU_ACK_JOYPAD.qp_new());

  /* p02.ROTU*/ wire2 _ROTU_FF0F_WRp   = not1(_REFA_FF0F_WRn);
  /* p02.MYZU*/ wire2 _MYZU_FF0F_SET0n = nand3(_ROTU_FF0F_WRp, _LETY_INT_VBL_ACKn,  cpu_bus.BUS_CPU_D[0].qp_new());
  /* p02.MODY*/ wire2 _MODY_FF0F_SET1n = nand3(_ROTU_FF0F_WRp, _LEJA_INT_STAT_ACKn, cpu_bus.BUS_CPU_D[1].qp_new());
  /* p02.PYHU*/ wire2 _PYHU_FF0F_SET2n = nand3(_ROTU_FF0F_WRp, _LESA_INT_TIM_ACKn,  cpu_bus.BUS_CPU_D[2].qp_new());
  /* p02.TOME*/ wire2 _TOME_FF0F_SET3n = nand3(_ROTU_FF0F_WRp, _LUFE_INT_SER_ACKn,  cpu_bus.BUS_CPU_D[3].qp_new());
  /* p02.TOGA*/ wire2 _TOGA_FF0F_SET4n = nand3(_ROTU_FF0F_WRp, _LAMO_INT_JOY_ACKn,  cpu_bus.BUS_CPU_D[4].qp_new());

  /*#p02.MUXE*/ wire2 _MUXE_INT0_WRn = or2(cpu_bus.BUS_CPU_D[0].qp_new(), _REFA_FF0F_WRn);
  /* p02.NABE*/ wire2 _NABE_INT1_WRn = or2(cpu_bus.BUS_CPU_D[1].qp_new(), _REFA_FF0F_WRn);
  /* p02.RAKE*/ wire2 _RAKE_INT2_WRn = or2(cpu_bus.BUS_CPU_D[2].qp_new(), _REFA_FF0F_WRn);
  /* p02.SULO*/ wire2 _SULO_INT3_WRn = or2(cpu_bus.BUS_CPU_D[3].qp_new(), _REFA_FF0F_WRn);
  /* p02.SEME*/ wire2 _SEME_INT4_WRn = or2(cpu_bus.BUS_CPU_D[4].qp_new(), _REFA_FF0F_WRn);

  /*#p02.LYTA*/ wire2 _LYTA_FF0F_RST0n = and3(_MUXE_INT0_WRn, _LETY_INT_VBL_ACKn,  _ALUR_SYS_RSTn);
  /* p02.MOVU*/ wire2 _MOVU_FF0F_RST1n = and3(_NABE_INT1_WRn, _LEJA_INT_STAT_ACKn, _ALUR_SYS_RSTn);
  /* p02.PYGA*/ wire2 _PYGA_FF0F_RST2n = and3(_RAKE_INT2_WRn, _LESA_INT_TIM_ACKn,  _ALUR_SYS_RSTn);
  /* p02.TUNY*/ wire2 _TUNY_FF0F_RST3n = and3(_SULO_INT3_WRn, _LUFE_INT_SER_ACKn,  _ALUR_SYS_RSTn);
  /* p02.TYME*/ wire2 _TYME_FF0F_RST4n = and3(_SEME_INT4_WRn, _LAMO_INT_JOY_ACKn,  _ALUR_SYS_RSTn);

  wire2 _PESU_VCC = 1;
  /* p02.LOPE*/ LOPE_FF0F_D0p.dff22(_VYPU_INT_VBLANKp,             _MYZU_FF0F_SET0n, _LYTA_FF0F_RST0n, _PESU_VCC);
  /* p02.LALU*/ LALU_FF0F_D1p.dff22(_VOTY_INT_STATp,               _MODY_FF0F_SET1n, _MOVU_FF0F_RST1n, _PESU_VCC);
  /* p02.NYBO*/ NYBO_FF0F_D2p.dff22(MOBA_TIMER_OVERFLOWp,          _PYHU_FF0F_SET2n, _PYGA_FF0F_RST2n, _PESU_VCC);
  /* p02.UBUL*/ UBUL_FF0F_D3p.dff22(serial.CALY_SER_CNT3.qp_new(), _TOME_FF0F_SET3n, _TUNY_FF0F_RST3n, _PESU_VCC);
  /* p02.ULAK*/ ULAK_FF0F_D4p.dff22(joypad.ASOK_INT_JOYp(),        _TOGA_FF0F_SET4n, _TYME_FF0F_RST4n, _PESU_VCC);

  SIG_CPU_INT_VBLANK.set(LOPE_FF0F_D0p.qp_new());
  SIG_CPU_INT_STAT  .set(LALU_FF0F_D1p.qp_new());
  SIG_CPU_INT_TIMER .set(NYBO_FF0F_D2p.qp_new());
  SIG_CPU_INT_SERIAL.set(UBUL_FF0F_D3p.qp_new());
  SIG_CPU_INT_JOYPAD.set(ULAK_FF0F_D4p.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------
