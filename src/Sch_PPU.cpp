#include "Sch_PPU.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void PpuRegisters::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void PpuRegisters::tock(SchematicTop& top) {

  /*p21.TADY*/ wire TADY_LINE_START_RST = nor(top.BYHA_VID_LINE_TRIG_d4(), top.TOFU_VID_RSTp());
  /*p21.WEGO*/ wire WEGO_LINE_END_RST   = or(top.TOFU_VID_RSTp(), VOGA_RENDER_DONE_SYNC.q());

  /*p24.SEGU*/ wire SEGU_CLKPIPEn = top.SEGU_CLKPIPEn();
  /*p24.ROXO*/ wire ROXO_CLKPIPEp = not(SEGU_CLKPIPEn);
  /*p24.SACU*/ wire SACU_CLKPIPEp = nor(SEGU_CLKPIPEn, ROXY_FINE_MATCH_LATCHn.q());

  {
    /*p21.VOGA*/ VOGA_RENDER_DONE_SYNC.set(top.ALET_xBxDxFxH(), TADY_LINE_START_RST, top.WODU_RENDER_DONEp());
  }

  {
    /*p21.XYMU*/ XYMU_RENDERINGp.nor_latch(top.AVAP_RENDER_START_RST(), WEGO_LINE_END_RST);
  }

  {
    /*p27.ROZE*/ wire ROZE_FINE_COUNT_7n = nand(RYKU_FINE_CNT0.q(), ROGA_FINE_CNT1.q(), RUBU_FINE_CNT2.q());
    /*p27.PECU*/ wire PECU_FINE_CLK = nand(ROXO_CLKPIPEp, ROZE_FINE_COUNT_7n);
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(XYMU_RENDERINGp.q());
    /*p27.PASO*/ wire PASO_FINE_RST = nor(top.TEVO_FINE_RSTp(), ROPY_RENDERINGn);
    /*p27.RYKU*/ RYKU_FINE_CNT0.set(PECU_FINE_CLK,       PASO_FINE_RST, RYKU_FINE_CNT0.qn());
    /*p27.ROGA*/ ROGA_FINE_CNT1.set(RYKU_FINE_CNT0.qn(), PASO_FINE_RST, ROGA_FINE_CNT1.qn());
    /*p27.RUBU*/ RUBU_FINE_CNT2.set(ROGA_FINE_CNT1.qn(), PASO_FINE_RST, RUBU_FINE_CNT2.qn());
  }

  {
    // There's a feedback loop here of sorts

    /*p27.SUHA*/ wire _SUHA_FINE_MATCHp = xnor(top.DATY_SCX0(), RYKU_FINE_CNT0.q()); // Arms on the ground side, XNOR
    /*p27.SYBY*/ wire _SYBY_FINE_MATCHp = xnor(top.DUZU_SCX1(), ROGA_FINE_CNT1.q());
    /*p27.SOZU*/ wire _SOZU_FINE_MATCHp = xnor(top.CYXU_SCX2(), RUBU_FINE_CNT2.q());
    /*p27.RONE*/ wire _RONE_FINE_MATCHn = nand(ROXY_FINE_MATCH_LATCHn.q(), _SUHA_FINE_MATCHp, _SYBY_FINE_MATCHp, _SOZU_FINE_MATCHp);
    /*p27.POHU*/ wire _POHU_FINE_MATCHp = not(_RONE_FINE_MATCHn);

    /*p27.PUXA*/ PUXA_FINE_MATCH_A.set(ROXO_CLKPIPEp,       XYMU_RENDERINGp.q(), _POHU_FINE_MATCHp);
    /*p27.NYZE*/ NYZE_FINE_MATCH_B.set(top.MOXE_AxCxExGx(), XYMU_RENDERINGp.q(), PUXA_FINE_MATCH_A.q());

    /*p27.POVA*/ wire POVA_FINE_MATCHpe = and(PUXA_FINE_MATCH_A.q(), NYZE_FINE_MATCH_B.qn());
    /*p27.PAHA*/ wire _PAHA_RENDERINGn = not(XYMU_RENDERINGp.q());
    /*p27.ROXY*/ ROXY_FINE_MATCH_LATCHn.nor_latch(_PAHA_RENDERINGn, POVA_FINE_MATCHpe);

    /*p21.XAJO*/ wire _XAJO_X_009 = and (XEHO_X0.q(), XYDO_X3.q());
    /*p21.WUSA*/ _WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009, WEGO_LINE_END_RST);
    /*p21.TOBA*/ wire _TOBA_LCD_CLOCK = and (SACU_CLKPIPEp, _WUSA_LCD_CLOCK_GATE.q());
    /*p21.SEMU*/ wire _SEMU_LCD_CLOCK = or(_TOBA_LCD_CLOCK, POVA_FINE_MATCHpe);
    /*p21.RYPO*/ wire _RYPO_LCD_CLOCK = not(_SEMU_LCD_CLOCK);
    top.LCD_PIN_CP.set(_RYPO_LCD_CLOCK);
  }

  {
    // LCD horizontal sync pin latch
    // if AVAP goes high, POFY goes high.
    // if PAHO or TOFU go high, POFY goes low.

    /*p24.PAHO*/ PAHO_X_8_SYNC.set(ROXO_CLKPIPEp, top.XYMU_RENDERINGp(), XYDO_X3.q());
    /*p24.RUJU*/ POFY_ST_LATCH.nor_latch(top.AVAP_RENDER_START_RST(), PAHO_X_8_SYNC.q() || top.TOFU_VID_RSTp());
    /*p24.RUZE*/ wire RUZE_PIN_ST = not(POFY_ST_LATCH.q());
    top.LCD_PIN_ST.set(RUZE_PIN_ST);
  }

  //----------

  {
    // vid x position, has carry lookahead because this increments every tcycle
    /*p21.RYBO*/ wire RYBO = xor(XEHO_X0.q(), SAVY_X1.q());
    /*p21.XUKE*/ wire XUKE = and(XEHO_X0.q(), SAVY_X1.q());

    /*p21.XYLE*/ wire XYLE = and(XODU_X2.q(), XUKE);
    /*p21.XEGY*/ wire XEGY = xor(XODU_X2.q(), XUKE);
    /*p21.XORA*/ wire XORA = xor(XYDO_X3.q(), XYLE);

    /*p21.SAKE*/ wire SAKE = xor(TUHU_X4.q(), TUKY_X5.q());
    /*p21.TYBA*/ wire TYBA = and(TUHU_X4.q(), TUKY_X5.q());
    /*p21.SURY*/ wire SURY = and(TAKO_X6.q(), TYBA);
    /*p21.TYGE*/ wire TYGE = xor(TAKO_X6.q(), TYBA);
    /*p21.ROKU*/ wire ROKU = xor(SYBE_X7.q(), SURY);

    
    /*p24.TOCA*/ wire TOCA_CLKPIPE_HI = not(XYDO_X3.q());

    /*p21.XEHO*/ XEHO_X0.set(SACU_CLKPIPEp,   TADY_LINE_START_RST, XEHO_X0.qn());
    /*p21.SAVY*/ SAVY_X1.set(SACU_CLKPIPEp,   TADY_LINE_START_RST, RYBO);
    /*p21.XODU*/ XODU_X2.set(SACU_CLKPIPEp,   TADY_LINE_START_RST, XEGY);
    /*p21.XYDO*/ XYDO_X3.set(SACU_CLKPIPEp,   TADY_LINE_START_RST, XORA);
    /*p21.TUHU*/ TUHU_X4.set(TOCA_CLKPIPE_HI, TADY_LINE_START_RST, TUHU_X4.qn());
    /*p21.TUKY*/ TUKY_X5.set(TOCA_CLKPIPE_HI, TADY_LINE_START_RST, SAKE);
    /*p21.TAKO*/ TAKO_X6.set(TOCA_CLKPIPE_HI, TADY_LINE_START_RST, TYGE);
    /*p21.SYBE*/ SYBE_X7.set(TOCA_CLKPIPE_HI, TADY_LINE_START_RST, ROKU);
  }

  //----------------------------------------
  // Config registers

  // FF41 STAT
  {
    // stat read
    // hblank  = stat 0 = 00
    // vblank  = stat 1 = 01
    // oamscan = stat 2 = 10
    // render  = stat 3 = 11

    // RUPO arms on ground side, nor latch
    // RUPO00 << ROPO16
    // RUPO01 nc
    // RUPO02 >> SEGO03
    // RUPO03 >> nc
    // RUPO04 nc
    // RUPO05 << PAGO03

    // when PAGO03 goes high, RUPO02 goes high
    // when ROPO16 goes high, RUPO02 goes low.

    /*p22.WOFA*/ wire WOFA_FF41n = nand(top.WERO_FF4Xp(), top.WADO_A00p(), top.XENO_A01n(), top.XUSY_A02n(), top.XERA_A03n());
    /*p22.VARY*/ wire VARY_FF41p = not(WOFA_FF41n);

    /*p21.TOBE*/ wire TOBE_FF41_RDp = and (VARY_FF41p, top.ASOT_CPU_RDp()); // die AND
    /*p21.VAVE*/ wire VAVE_FF41_RDn = not(TOBE_FF41_RDp); // die INV

    /*p21.SEPA*/ wire SEPA_FF41_WRp = and (VARY_FF41p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p21.RYVE*/ wire RYVE_FF41_WRn = not(SEPA_FF41_WRp);

    /*p21.RYJU*/ wire RYJU_FF41_WRn = not(SEPA_FF41_WRp);
    
    /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(top.WESY_SYS_RSTn(), RYJU_FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ RUPO_LYC_MATCH_LATCHn.nor_latch(PAGO_LYC_MATCH_RST, top.ROPO_LY_MATCH_SYNCp());

    /*p21.ROXE*/ ROXE_INT_HBL_EN.set(RYVE_FF41_WRn, !RYVE_FF41_WRn, top.WESY_SYS_RSTn(), top.CPU_TRI_D0.q());
    /*p21.RUFO*/ RUFO_INT_VBL_EN.set(RYVE_FF41_WRn, !RYVE_FF41_WRn, top.WESY_SYS_RSTn(), top.CPU_TRI_D1.q());
    /*p21.REFE*/ REFE_INT_OAM_EN.set(RYVE_FF41_WRn, !RYVE_FF41_WRn, top.WESY_SYS_RSTn(), top.CPU_TRI_D2.q());
    /*p21.RUGU*/ RUGU_INT_LYC_EN.set(RYVE_FF41_WRn, !RYVE_FF41_WRn, top.WESY_SYS_RSTn(), top.CPU_TRI_D3.q());

    /*p21.XATY*/ wire XATY_STAT_MODE1n = nor(XYMU_RENDERINGp.q(), top.ACYL_SCANNINGp()); // die NOR
    /*p21.SADU*/ wire SADU_STAT_MODE0n = nor(XYMU_RENDERINGp.q(), top.PARU_VBLANKp_d4()); // die NOR

    // OK, these tribufs are _slightly_ different - compare SEGO and SASY, second rung.

    // TEBY facing dot
    // WUGA facing dot
    // SEGO facing dot

    /*p21.TEBY*/ top.CPU_TRI_D0.set_tribuf_6p(TOBE_FF41_RDp, not(SADU_STAT_MODE0n));
    /*p21.WUGA*/ top.CPU_TRI_D1.set_tribuf_6p(TOBE_FF41_RDp, not(XATY_STAT_MODE1n));
    /*p21.SEGO*/ top.CPU_TRI_D2.set_tribuf_6p(TOBE_FF41_RDp, not(RUPO_LYC_MATCH_LATCHn.q()));

    // PUZO not facing dot
    // POFO not facing dot
    // SASY not facing dot
    // POTE not facing dot

    /*p21.PUZO*/ top.CPU_TRI_D3.set_tribuf_6n(VAVE_FF41_RDn, ROXE_INT_HBL_EN.q());
    /*p21.POFO*/ top.CPU_TRI_D4.set_tribuf_6n(VAVE_FF41_RDn, RUFO_INT_VBL_EN.q());
    /*p21.SASY*/ top.CPU_TRI_D5.set_tribuf_6n(VAVE_FF41_RDn, REFE_INT_OAM_EN.q());
    /*p21.POTE*/ top.CPU_TRI_D6.set_tribuf_6n(VAVE_FF41_RDn, RUGU_INT_LYC_EN.q());
  }
}

//------------------------------------------------------------------------------

SignalHash PpuRegisters::commit(SchematicTop& top) {
  SignalHash hash;
  /*p??.ROXY*/ hash << ROXY_FINE_MATCH_LATCHn.commit();
  /*p??.PUXA*/ hash << PUXA_FINE_MATCH_A.commit();
  /*p27.NYZE*/ hash << NYZE_FINE_MATCH_B.commit();
  /*p27.RYKU*/ hash << RYKU_FINE_CNT0.commit();
  /*p27.ROGA*/ hash << ROGA_FINE_CNT1.commit();
  /*p27.RUBU*/ hash << RUBU_FINE_CNT2.commit();
  /*p21.XEHO*/ hash << XEHO_X0.commit();
  /*p21.SAVY*/ hash << SAVY_X1.commit();
  /*p21.XODU*/ hash << XODU_X2.commit();
  /*p21.XYDO*/ hash << XYDO_X3.commit();
  /*p21.TUHU*/ hash << TUHU_X4.commit();
  /*p21.TUKY*/ hash << TUKY_X5.commit();
  /*p21.TAKO*/ hash << TAKO_X6.commit();
  /*p21.SYBE*/ hash << SYBE_X7.commit();
  /*p21.XYMU*/ hash << XYMU_RENDERINGp.commit();
  /*p21.VOGA*/ hash << VOGA_RENDER_DONE_SYNC.commit();

  /*p21.RUPO*/ hash << RUPO_LYC_MATCH_LATCHn.commit();


  /*p24.PAHO*/ hash << PAHO_X_8_SYNC.commit();
  /*p24.RUJU*/ hash << POFY_ST_LATCH.commit(); // nor latch with p24.RUJU, p24.POME
  /*p21.WUSA*/ hash << _WUSA_LCD_CLOCK_GATE.commit();

  // FF41 - STAT
  /*p21.ROXE*/ hash << ROXE_INT_HBL_EN.commit();
  /*p21.RUFO*/ hash << RUFO_INT_VBL_EN.commit();
  /*p21.REFE*/ hash << REFE_INT_OAM_EN.commit();
  /*p21.RUGU*/ hash << RUGU_INT_LYC_EN.commit();

  hash << top.LCD_PIN_CP.commit();
  hash << top.LCD_PIN_ST.commit();

  return hash;
}

//------------------------------------------------------------------------------
