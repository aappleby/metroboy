#include "Sch_Interrupts.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void InterruptRegisters::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void InterruptRegisters::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  /*p07.SEMY*/ wire _SEMY_ADDR_XX0X = nor(cpu_bus.CPU_BUS_A07, cpu_bus.CPU_BUS_A06, cpu_bus.CPU_BUS_A05, cpu_bus.CPU_BUS_A04);
  /*p07.SAPA*/ wire _SAPA_ADDR_XXXF = and (cpu_bus.CPU_BUS_A00, cpu_bus.CPU_BUS_A01, cpu_bus.CPU_BUS_A02, cpu_bus.CPU_BUS_A03);

  /*p07.ROLO*/ wire _ROLO_FF0F_RDn = nand(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, cpu_bus.SYKE_FF00_FFFFp(), top.TEDO_CPU_RDp());          // schematic wrong, is NAND
  /*p07.REFA*/ wire _REFA_FF0F_WRn = nand(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, cpu_bus.SYKE_FF00_FFFFp(), top.TAPU_CPU_WRp_xxxxEFGx()); // schematic wrong, is NAND

  // FIXME FIXME FIXME order wrong!!!

  // FF0F INT
  {
    // pass gates? does look like a transparent latch here...

    // MATY is connected to cpu int pin...CPU_PIN_INT_VBLANK?
    // schematic says LOPE also connected to CPU_PIN_INT_VBLANK?

    /*p02.MATY*/ MATY_FF0F_L0 = tp_latch_r2(_ROLO_FF0F_RDn, LOPE_FF0F_0.q()); // OUTPUT ON RUNG 10
    /*p02.NEJY*/ NEJY_FF0F_L1 = tp_latch_r2(_ROLO_FF0F_RDn, UBUL_FF0F_3.q()); // OUTPUT ON RUNG 10
    /*p02.NUTY*/ NUTY_FF0F_L2 = tp_latch_r2(_ROLO_FF0F_RDn, ULAK_FF0F_4.q()); // OUTPUT ON RUNG 10
    /*p02.MOPO*/ MOPO_FF0F_L3 = tp_latch_r2(_ROLO_FF0F_RDn, LALU_FF0F_1.q()); // OUTPUT ON RUNG 10
    /*p02.PAVY*/ PAVY_FF0F_L4 = tp_latch_r2(_ROLO_FF0F_RDn, NYBO_FF0F_2.q()); // OUTPUT ON RUNG 10

    /*p02.POLA*/ wire _POLA_FF0F_RD  = not(_ROLO_FF0F_RDn);
    /*p02.NELA*/ cpu_bus.CPU_BUS_D0 = tribuf_6p_r2(_POLA_FF0F_RD, MATY_FF0F_L0.q());
    /*p02.NABO*/ cpu_bus.CPU_BUS_D1 = tribuf_6p_r2(_POLA_FF0F_RD, NEJY_FF0F_L1.q());
    /*p02.ROVA*/ cpu_bus.CPU_BUS_D2 = tribuf_6p_r2(_POLA_FF0F_RD, NUTY_FF0F_L2.q());
    /*p02.PADO*/ cpu_bus.CPU_BUS_D3 = tribuf_6p_r2(_POLA_FF0F_RD, MOPO_FF0F_L3.q());
    /*p02.PEGY*/ cpu_bus.CPU_BUS_D4 = tribuf_6p_r2(_POLA_FF0F_RD, PAVY_FF0F_L4.q());
  }

  {
    // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
    // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
    // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
    // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
    // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

    /*p02.ROTU*/ wire _ROTU_FF0F_WRp = not(_REFA_FF0F_WRn);

    /*p21.TOLU*/ wire _TOLU_VBLANKn       = not(top.lcd_reg.PARU_VBLANKp_d4());
    /*p21.VYPU*/ wire _VYPU_VBLANKp       = not(_TOLU_VBLANKn);     

    /*p02.LETY*/ wire _LETY_INT_VBL_ACKn  = not(CPU_PIN_ACK_VBLANK);
    /*p02.LEJA*/ wire _LEJA_INT_STAT_ACKn = not(CPU_PIN_ACK_STAT);
    /*p02.LESA*/ wire _LESA_INT_TIM_ACKn  = not(CPU_PIN_ACK_TIMER);
    /*p02.LUFE*/ wire _LUFE_INT_SER_ACKn  = not(CPU_PIN_ACK_SERIAL);
    /*p02.LAMO*/ wire _LAMO_INT_JOY_ACKn  = not(CPU_PIN_ACK_JOYPAD);

    /*p02.MYZU*/ wire _MYZU_FF0F_SET0n    = nand(_ROTU_FF0F_WRp, _LETY_INT_VBL_ACKn,  cpu_bus.CPU_BUS_D0.q());
    /*p02.MODY*/ wire _MODY_FF0F_SET1n    = nand(_ROTU_FF0F_WRp, _LEJA_INT_STAT_ACKn, cpu_bus.CPU_BUS_D1.q());
    /*p02.PYHU*/ wire _PYHU_FF0F_SET2n    = nand(_ROTU_FF0F_WRp, _LESA_INT_TIM_ACKn,  cpu_bus.CPU_BUS_D2.q());
    /*p02.TOME*/ wire _TOME_FF0F_SET3n    = nand(_ROTU_FF0F_WRp, _LUFE_INT_SER_ACKn,  cpu_bus.CPU_BUS_D3.q());
    /*p02.TOGA*/ wire _TOGA_FF0F_SET4n    = nand(_ROTU_FF0F_WRp, _LAMO_INT_JOY_ACKn,  cpu_bus.CPU_BUS_D4.q());

    /*p02.MUXE*/ wire _MUXE_INT0_WRn = or (cpu_bus.CPU_BUS_D0.q(), _REFA_FF0F_WRn);
    /*p02.NABE*/ wire _NABE_INT1_WRn = or (cpu_bus.CPU_BUS_D1.q(), _REFA_FF0F_WRn);
    /*p02.RAKE*/ wire _RAKE_INT2_WRn = or (cpu_bus.CPU_BUS_D2.q(), _REFA_FF0F_WRn);
    /*p02.SULO*/ wire _SULO_INT3_WRn = or (cpu_bus.CPU_BUS_D3.q(), _REFA_FF0F_WRn);
    /*p02.SEME*/ wire _SEME_INT4_WRn = or (cpu_bus.CPU_BUS_D4.q(), _REFA_FF0F_WRn);

    /*p02.LYTA*/ wire _LYTA_FF0F_RST0n = and (_MUXE_INT0_WRn, _LETY_INT_VBL_ACKn,  top.clk_reg.ALUR_SYS_RSTn());
    /*p02.MOVU*/ wire _MOVU_FF0F_RST1n = and (_NABE_INT1_WRn, _LEJA_INT_STAT_ACKn, top.clk_reg.ALUR_SYS_RSTn());
    /*p02.PYGA*/ wire _PYGA_FF0F_RST2n = and (_RAKE_INT2_WRn, _LESA_INT_TIM_ACKn,  top.clk_reg.ALUR_SYS_RSTn());
    /*p02.TUNY*/ wire _TUNY_FF0F_RST3n = and (_SULO_INT3_WRn, _LUFE_INT_SER_ACKn,  top.clk_reg.ALUR_SYS_RSTn());
    /*p02.TYME*/ wire _TYME_FF0F_RST4n = and (_SEME_INT4_WRn, _LAMO_INT_JOY_ACKn,  top.clk_reg.ALUR_SYS_RSTn());

    /*p21.TAPA*/ wire TAPA_INT_OAM = and (_TOLU_VBLANKn, top.lcd_reg.SELA_LINE_ENDp());
    /*p21.TARU*/ wire TARU_INT_HBL = and (_TOLU_VBLANKn, top.pix_pipe.WODU_RENDER_DONEp());
    /*p21.SUKO*/ wire SUKO_INT_STATb = amux4(top.pix_pipe.RUGU_INT_LYC_EN.q(), top.lcd_reg.ROPO_LY_MATCH_SYNCp(),
                                             top.pix_pipe.REFE_INT_OAM_EN.q(), TAPA_INT_OAM,
                                             top.pix_pipe.RUFO_INT_VBL_EN.q(), top.lcd_reg.PARU_VBLANKp_d4(), // polarity?
                                             top.pix_pipe.ROXE_INT_HBL_EN.q(), TARU_INT_HBL);
    /*p21.TUVA*/ wire TUVA_INT_STATn = not(SUKO_INT_STATb);
    /*p21.VOTY*/ wire VOTY_INT_STATp = not(TUVA_INT_STATn);


    // LOPE_01 SC
    // LOPE_02 NC
    // LOPE_03 NC
    // LOPE_04 NC
    // LOPE_05 SC
    // LOPE_06 SC
    // LOPE_07 << D    PESU_GND 
    // LOPE_08 NC
    // LOPE_09 SC
    // LOPE_10 NC
    // LOPE_11 SC
    // LOPE_12 NC
    // LOPE_13 NC
    // LOPE_14 << SETn MYZU_04
    // LOPE_15 >> Qn
    // LOPE_16 >> Q    CPU_PIN_INT_VBLANK, MATY_03
    // LOPE_17 << RSTn LYTA_05
    // LOPE_18 NC
    // LOPE_19 SC
    // LOPE_20 SC
    // LOPE_21 SC
    // LOPE_22 << CLKp VYPU_03

    wire PESU_GND = 0;
    /*p02.LOPE*/ LOPE_FF0F_0 = ff22_r2(_VYPU_VBLANKp,     _MYZU_FF0F_SET0n, _LYTA_FF0F_RST0n, PESU_GND);
    /*p02.LALU*/ LALU_FF0F_1 = ff22_r2(VOTY_INT_STATp,   _MODY_FF0F_SET1n, _MOVU_FF0F_RST1n, PESU_GND);
    /*p02.NYBO*/ NYBO_FF0F_2 = ff22_r2(top.tim_reg.MOBA_INT_TIMERp(),  _PYHU_FF0F_SET2n, _PYGA_FF0F_RST2n, PESU_GND);
    /*p02.UBUL*/ UBUL_FF0F_3 = ff22_r2(top.ser_reg.CALY_SER_INTp(), _TOME_FF0F_SET3n, _TUNY_FF0F_RST3n, PESU_GND);
    /*p02.ULAK*/ ULAK_FF0F_4 = ff22_r2(top.joypad.ASOK_INT_JOYPADp(), _TOGA_FF0F_SET4n, _TYME_FF0F_RST4n, PESU_GND);

    CPU_PIN_INT_VBLANK = LOPE_FF0F_0.q();
    CPU_PIN_INT_STAT   = LALU_FF0F_1.q();
    CPU_PIN_INT_TIMER  = NYBO_FF0F_2.q();
    CPU_PIN_INT_SERIAL = UBUL_FF0F_3.q();
    CPU_PIN_INT_JOYPAD = ULAK_FF0F_4.q();
  }
}

//------------------------------------------------------------------------------
