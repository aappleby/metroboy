#include "Sch_Interrupts.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void InterruptRegisters::tick(SchematicTop& top) {
  Signal _ROLO_FF0F_RDn;
  Signal _REFA_FF0F_WRn;
  {
    /*p07.SEMY*/ wire _SEMY_ADDR_XX0X = nor(top.CPU_PIN_A07, top.CPU_PIN_A06, top.CPU_PIN_A05, top.CPU_PIN_A04);
    /*p07.SAPA*/ wire _SAPA_ADDR_XXXF = and (top.CPU_PIN_A00, top.CPU_PIN_A01, top.CPU_PIN_A02, top.CPU_PIN_A03);

    /*p07.ROLO*/ _ROLO_FF0F_RDn = nand(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, top.SYKE_FF00_FFFFp(), top.TEDO_CPU_RDp());          // schematic wrong, is NAND
    /*p07.REFA*/ _REFA_FF0F_WRn = nand(_SEMY_ADDR_XX0X, _SAPA_ADDR_XXXF, top.SYKE_FF00_FFFFp(), top.TAPU_CPU_WRp_xxxxEFGx()); // schematic wrong, is NAND
  }

  // FF0F INT
  {
    // pass gates? does look like a transparent latch here...
    // order wrong?

    // MATY is connected to cpu int pin...CPU_PIN_INT_VBLANK?
    // schematic says LOPE also connected to CPU_PIN_INT_VBLANK?

    /*p02.MATY*/ MATY_FF0F_L0.tp_latch(_ROLO_FF0F_RDn, LOPE_FF0F_0.q()); // OUTPUT ON RUNG 10
    /*p02.NEJY*/ NEJY_FF0F_L1.tp_latch(_ROLO_FF0F_RDn, UBUL_FF0F_3.q()); // OUTPUT ON RUNG 10
    /*p02.NUTY*/ NUTY_FF0F_L2.tp_latch(_ROLO_FF0F_RDn, ULAK_FF0F_4.q()); // OUTPUT ON RUNG 10
    /*p02.MOPO*/ MOPO_FF0F_L3.tp_latch(_ROLO_FF0F_RDn, LALU_FF0F_1.q()); // OUTPUT ON RUNG 10
    /*p02.PAVY*/ PAVY_FF0F_L4.tp_latch(_ROLO_FF0F_RDn, NYBO_FF0F_2.q()); // OUTPUT ON RUNG 10

    /*p02.POLA*/ wire _POLA_FF0F_RD  = not(_ROLO_FF0F_RDn);
    /*p02.NELA*/ top.CPU_TRI_D0.set_tribuf_6p(_POLA_FF0F_RD, MATY_FF0F_L0.q());
    /*p02.NABO*/ top.CPU_TRI_D1.set_tribuf_6p(_POLA_FF0F_RD, NEJY_FF0F_L1.q());
    /*p02.ROVA*/ top.CPU_TRI_D2.set_tribuf_6p(_POLA_FF0F_RD, NUTY_FF0F_L2.q());
    /*p02.PADO*/ top.CPU_TRI_D3.set_tribuf_6p(_POLA_FF0F_RD, MOPO_FF0F_L3.q());
    /*p02.PEGY*/ top.CPU_TRI_D4.set_tribuf_6p(_POLA_FF0F_RD, PAVY_FF0F_L4.q());
  }

  {
    // Bit 0 : V-Blank  Interrupt Request(INT 40h)  (1=Request)
    // Bit 1 : LCD STAT Interrupt Request(INT 48h)  (1=Request)
    // Bit 2 : Timer    Interrupt Request(INT 50h)  (1=Request)
    // Bit 3 : Serial   Interrupt Request(INT 58h)  (1=Request)
    // Bit 4 : Joypad   Interrupt Request(INT 60h)  (1=Request)

    /*p02.ROTU*/ wire _ROTU_FF0F_WRp = not(_REFA_FF0F_WRn);

    /*p21.TOLU*/ wire TOLU_VBLANKn = not(top.PARU_VBLANKp_d4());

    /*p21.VYPU*/ wire VYPU_VBLANKp       = not(TOLU_VBLANKn);      // int 0 source
    /*p21.VOTY*/ wire VOTY_INT_STATp     = top.VOTY_INT_STATp();   // int 1 source
    /*p03.MOBA*/ wire MOBA_INT_TIMERp    = top.MOBA_INT_TIMERp();  // int 2 source
    /*p06.CALY*/ wire CALY_INT_SERIALp   = top.CALY_INT_SERIALp(); // int 3 source
    /*p02.ASOK*/ wire ASOK_INT_JOYPADp   = top.ASOK_INT_JOYPADp(); // int 4 source

    /*p02.LETY*/ wire LETY_INT_VBL_ACKn  = not(top.CPU_PIN_ACK_VBLANK);
    /*p02.LEJA*/ wire LEJA_INT_STAT_ACKn = not(top.CPU_PIN_ACK_STAT);
    /*p02.LESA*/ wire LESA_INT_TIM_ACKn  = not(top.CPU_PIN_ACK_TIMER);
    /*p02.LUFE*/ wire LUFE_INT_SER_ACKn  = not(top.CPU_PIN_ACK_SERIAL);
    /*p02.LAMO*/ wire LAMO_INT_JOY_ACKn  = not(top.CPU_PIN_ACK_JOYPAD);

    /*p02.MUXE*/ wire MUXE_INT0_WRn      = or (top.CPU_TRI_D0, _REFA_FF0F_WRn);
    /*p02.NABE*/ wire NABE               = or (top.CPU_TRI_D1, _REFA_FF0F_WRn);
    /*p02.RAKE*/ wire RAKE               = or (top.CPU_TRI_D2, _REFA_FF0F_WRn);
    /*p02.SULO*/ wire SULO               = or (top.CPU_TRI_D3, _REFA_FF0F_WRn);
    /*p02.SEME*/ wire SEME               = or (top.CPU_TRI_D4, _REFA_FF0F_WRn);

    /*p02.MYZU*/ wire MYZU_FF0F_SET0n    = nand(_ROTU_FF0F_WRp, LETY_INT_VBL_ACKn,  top.CPU_TRI_D0);
    /*p02.MODY*/ wire MODY_FF0F_SET1n    = nand(_ROTU_FF0F_WRp, LEJA_INT_STAT_ACKn, top.CPU_TRI_D1);
    /*p02.PYHU*/ wire PYHU_FF0F_SET2n    = nand(_ROTU_FF0F_WRp, LESA_INT_TIM_ACKn,  top.CPU_TRI_D2);
    /*p02.TOME*/ wire TOME_FF0F_SET3n    = nand(_ROTU_FF0F_WRp, LUFE_INT_SER_ACKn,  top.CPU_TRI_D3);
    /*p02.TOGA*/ wire TOGA_FF0F_SET4n    = nand(_ROTU_FF0F_WRp, LAMO_INT_JOY_ACKn,  top.CPU_TRI_D4);

    /*p02.LYTA*/ wire LYTA_FF0F_RST0n    = and (MUXE_INT0_WRn, LETY_INT_VBL_ACKn,  top.ALUR_SYS_RSTn());
    /*p02.MOVU*/ wire MOVU_FF0F_RST1n    = and (NABE,          LEJA_INT_STAT_ACKn, top.ALUR_SYS_RSTn());
    /*p02.PYGA*/ wire PYGA_FF0F_RST2n    = and (RAKE,          LESA_INT_TIM_ACKn,  top.ALUR_SYS_RSTn());
    /*p02.TUNY*/ wire TUNY_FF0F_RST3n    = and (SULO,          LUFE_INT_SER_ACKn,  top.ALUR_SYS_RSTn());
    /*p02.TYME*/ wire TYME_FF0F_RST4n    = and (SEME,          LAMO_INT_JOY_ACKn,  top.ALUR_SYS_RSTn());


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

    /*p02.LOPE*/ LOPE_FF0F_0.set(VYPU_VBLANKp,     MYZU_FF0F_SET0n, LYTA_FF0F_RST0n, top.PESU_GND);
    /*p02.LALU*/ LALU_FF0F_1.set(VOTY_INT_STATp,   MODY_FF0F_SET1n, MOVU_FF0F_RST1n, top.PESU_GND);
    /*p02.NYBO*/ NYBO_FF0F_2.set(MOBA_INT_TIMERp,  PYHU_FF0F_SET2n, PYGA_FF0F_RST2n, top.PESU_GND);
    /*p02.UBUL*/ UBUL_FF0F_3.set(CALY_INT_SERIALp, TOME_FF0F_SET3n, TUNY_FF0F_RST3n, top.PESU_GND);
    /*p02.ULAK*/ ULAK_FF0F_4.set(ASOK_INT_JOYPADp, TOGA_FF0F_SET4n, TYME_FF0F_RST4n, top.PESU_GND);

    CPU_PIN_INT_VBLANK.set(LOPE_FF0F_0.q());
    CPU_PIN_INT_STAT  .set(LALU_FF0F_1.q());
    CPU_PIN_INT_TIMER .set(NYBO_FF0F_2.q());
    CPU_PIN_INT_SERIAL.set(UBUL_FF0F_3.q());
    CPU_PIN_INT_JOYPAD.set(ULAK_FF0F_4.q());
  }
}

//------------------------------------------------------------------------------

SignalHash InterruptRegisters::commit() {
  SignalHash hash;
  hash << LOPE_FF0F_0.commit();
  hash << UBUL_FF0F_3.commit();
  hash << ULAK_FF0F_4.commit();
  hash << LALU_FF0F_1.commit();
  hash << NYBO_FF0F_2.commit();
  hash << MATY_FF0F_L0.commit();
  hash << NEJY_FF0F_L1.commit();
  hash << NUTY_FF0F_L2.commit();
  hash << MOPO_FF0F_L3.commit();
  hash << PAVY_FF0F_L4.commit();

  hash << CPU_PIN_INT_VBLANK.commit();    // PORTB_03: <- LOPE, vblank int
  hash << CPU_PIN_INT_STAT.commit();      // PORTB_07: <- LALU, stat int
  hash << CPU_PIN_INT_TIMER.commit();     // PORTB_11: <- NYBO, timer int
  hash << CPU_PIN_INT_SERIAL.commit();    // PORTB_15: <- UBUL, serial int
  hash << CPU_PIN_INT_JOYPAD.commit();    // PORTB_19: <- ULAK, joypad int

  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf(" ----- INT REG -----\n");
  LOPE_FF0F_0.dump(text_painter, "LOPE_FF0F_0  ");
  UBUL_FF0F_3.dump(text_painter, "UBUL_FF0F_3  ");
  ULAK_FF0F_4.dump(text_painter, "ULAK_FF0F_4  ");
  LALU_FF0F_1.dump(text_painter, "LALU_FF0F_1  ");
  NYBO_FF0F_2.dump(text_painter, "NYBO_FF0F_2  ");
  MATY_FF0F_L0.dump(text_painter, "MATY_FF0F_L0 ");
  NEJY_FF0F_L1.dump(text_painter, "NEJY_FF0F_L1 ");
  NUTY_FF0F_L2.dump(text_painter, "NUTY_FF0F_L2 ");
  MOPO_FF0F_L3.dump(text_painter, "MOPO_FF0F_L3 ");
  PAVY_FF0F_L4.dump(text_painter, "PAVY_FF0F_L4 ");
  text_painter.newline();
}
#endif