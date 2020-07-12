#include "Sch_PPU.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void PpuRegisters::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void PpuRegisters::tock(SchematicTop& top) {

  /*p21.TADY*/ wire _TADY_LINE_START_RST = nor(top.lcd_reg.BYHA_VID_LINE_TRIG_d4(), top.rst_reg.TOFU_VID_RSTp());
  /*p21.WEGO*/ wire _WEGO_LINE_END_RST   = or(top.rst_reg.TOFU_VID_RSTp(), VOGA_RENDER_DONE_SYNC.q());

  /*p24.ROXO*/ wire _ROXO_CLKPIPEp = not(top.SEGU_CLKPIPEn());

  {
    /*p21.VOGA*/ VOGA_RENDER_DONE_SYNC.set(top.clk_reg.ALET_xBxDxFxH(), _TADY_LINE_START_RST, top.WODU_RENDER_DONEp());
  }

  {
    /*p21.XYMU*/ _XYMU_RENDERINGp.nor_latch(top.AVAP_RENDER_START_RST(), _WEGO_LINE_END_RST);
  }

  {
    /*p27.PECU*/ wire _PECU_FINE_CLK = nand(_ROXO_CLKPIPEp, ROZE_FINE_COUNT_7n());
    /*p27.PASO*/ wire _PASO_FINE_RST = nor(top.TEVO_FINE_RSTp(), top.ppu_reg.ROPY_RENDERINGn());
    /*p27.RYKU*/ RYKU_FINE_CNT0.set(_PECU_FINE_CLK,      _PASO_FINE_RST, RYKU_FINE_CNT0.qn());
    /*p27.ROGA*/ ROGA_FINE_CNT1.set(RYKU_FINE_CNT0.qn(), _PASO_FINE_RST, ROGA_FINE_CNT1.qn());
    /*p27.RUBU*/ RUBU_FINE_CNT2.set(ROGA_FINE_CNT1.qn(), _PASO_FINE_RST, RUBU_FINE_CNT2.qn());
  }

  {
    // There's a feedback loop here of sorts

    /*p27.SUHA*/ wire _SUHA_FINE_MATCHp = xnor(top.tile_fetcher.DATY_SCX0(), RYKU_FINE_CNT0.q()); // Arms on the ground side, XNOR
    /*p27.SYBY*/ wire _SYBY_FINE_MATCHp = xnor(top.tile_fetcher.DUZU_SCX1(), ROGA_FINE_CNT1.q());
    /*p27.SOZU*/ wire _SOZU_FINE_MATCHp = xnor(top.tile_fetcher.CYXU_SCX2(), RUBU_FINE_CNT2.q());
    /*p27.RONE*/ wire _RONE_FINE_MATCHn = nand(ROXY_FINE_MATCH_LATCHn.q(), _SUHA_FINE_MATCHp, _SYBY_FINE_MATCHp, _SOZU_FINE_MATCHp);
    /*p27.POHU*/ wire _POHU_FINE_MATCHp = not(_RONE_FINE_MATCHn);

    /*p27.PUXA*/ PUXA_FINE_MATCH_A.set(_ROXO_CLKPIPEp,      _XYMU_RENDERINGp.q(), _POHU_FINE_MATCHp);
    /*p27.NYZE*/ NYZE_FINE_MATCH_B.set(top.clk_reg.MOXE_AxCxExGx(), _XYMU_RENDERINGp.q(), PUXA_FINE_MATCH_A.q());

    /*p27.POVA*/ wire _POVA_FINE_MATCHpe = and(PUXA_FINE_MATCH_A.q(), NYZE_FINE_MATCH_B.qn());
    /*p27.PAHA*/ wire _PAHA_RENDERINGn = not(_XYMU_RENDERINGp.q());
    /*p27.ROXY*/ ROXY_FINE_MATCH_LATCHn.nor_latch(_PAHA_RENDERINGn, _POVA_FINE_MATCHpe);

    /*p21.XAJO*/ wire _XAJO_X_009 = and (XEHO_X0.q(), XYDO_X3.q());
    /*p21.WUSA*/ _WUSA_LCD_CLOCK_GATE.nor_latch(_XAJO_X_009, _WEGO_LINE_END_RST);
    /*p21.TOBA*/ wire _TOBA_LCD_CLOCK = and (top.SACU_CLKPIPEp(), _WUSA_LCD_CLOCK_GATE.q());
    /*p21.SEMU*/ wire _SEMU_LCD_CLOCK = or(_TOBA_LCD_CLOCK, _POVA_FINE_MATCHpe);
    /*p21.RYPO*/ wire _RYPO_LCD_CLOCK = not(_SEMU_LCD_CLOCK);
    top.LCD_PIN_CP.set(_RYPO_LCD_CLOCK);
  }

  {
    // LCD horizontal sync pin latch
    // if AVAP goes high, POFY goes high.
    // if PAHO or TOFU go high, POFY goes low.

    /*p24.PAHO*/ PAHO_X_8_SYNC.set(_ROXO_CLKPIPEp, _XYMU_RENDERINGp.q(), XYDO_X3.q());
    /*p24.RUJU*/ POFY_ST_LATCH.nor_latch(top.AVAP_RENDER_START_RST(), PAHO_X_8_SYNC.q() || top.rst_reg.TOFU_VID_RSTp());
    /*p24.RUZE*/ wire _RUZE_PIN_ST = not(POFY_ST_LATCH.q());
    top.LCD_PIN_ST.set(_RUZE_PIN_ST);
  }

  //----------

  {
    // vid x position, has carry lookahead because this can increment every tcycle
    /*p21.RYBO*/ wire _RYBO = xor(XEHO_X0.q(), SAVY_X1.q());
    /*p21.XUKE*/ wire _XUKE = and(XEHO_X0.q(), SAVY_X1.q());

    /*p21.XYLE*/ wire _XYLE = and(XODU_X2.q(), _XUKE);
    /*p21.XEGY*/ wire _XEGY = xor(XODU_X2.q(), _XUKE);
    /*p21.XORA*/ wire _XORA = xor(XYDO_X3.q(), _XYLE);

    /*p21.SAKE*/ wire _SAKE = xor(TUHU_X4.q(), TUKY_X5.q());
    /*p21.TYBA*/ wire _TYBA = and(TUHU_X4.q(), TUKY_X5.q());
    /*p21.SURY*/ wire _SURY = and(TAKO_X6.q(), _TYBA);
    /*p21.TYGE*/ wire _TYGE = xor(TAKO_X6.q(), _TYBA);
    /*p21.ROKU*/ wire _ROKU = xor(SYBE_X7.q(), _SURY);

    /*p24.TOCA*/ wire _TOCA_CLKPIPE_HI = not(XYDO_X3.q());

    /*p21.XEHO*/ XEHO_X0.set(top.SACU_CLKPIPEp(), _TADY_LINE_START_RST, XEHO_X0.qn());
    /*p21.SAVY*/ SAVY_X1.set(top.SACU_CLKPIPEp(), _TADY_LINE_START_RST, _RYBO);
    /*p21.XODU*/ XODU_X2.set(top.SACU_CLKPIPEp(), _TADY_LINE_START_RST, _XEGY);
    /*p21.XYDO*/ XYDO_X3.set(top.SACU_CLKPIPEp(), _TADY_LINE_START_RST, _XORA);
    /*p21.TUHU*/ TUHU_X4.set(_TOCA_CLKPIPE_HI,    _TADY_LINE_START_RST, TUHU_X4.qn());
    /*p21.TUKY*/ TUKY_X5.set(_TOCA_CLKPIPE_HI,    _TADY_LINE_START_RST, _SAKE);
    /*p21.TAKO*/ TAKO_X6.set(_TOCA_CLKPIPE_HI,    _TADY_LINE_START_RST, _TYGE);
    /*p21.SYBE*/ SYBE_X7.set(_TOCA_CLKPIPE_HI,    _TADY_LINE_START_RST, _ROKU);
  }

  //----------------------------------------
  // Config registers

  // FF41 STAT
  {
    /*p22.WOFA*/ wire _WOFA_FF41n = nand(top.WERO_FF4Xp(), top.WADO_A00p(), top.XENO_A01n(), top.XUSY_A02n(), top.XERA_A03n());
    /*p22.VARY*/ wire _VARY_FF41p = not(_WOFA_FF41n);

    /*p21.TOBE*/ wire _TOBE_FF41_RDp = and (_VARY_FF41p, top.ASOT_CPU_RDp()); // die AND
    /*p21.VAVE*/ wire _VAVE_FF41_RDn = not(_TOBE_FF41_RDp); // die INV

    /*p21.SEPA*/ wire _SEPA_FF41_WRp = and (_VARY_FF41p, top.CUPA_CPU_WRp_xxxxEFGx());
    /*p21.RYVE*/ wire _RYVE_FF41_WRn = not(_SEPA_FF41_WRp);

    /*p21.RYJU*/ wire _RYJU_FF41_WRn = not(_SEPA_FF41_WRp);
    
    /*p21.PAGO*/ wire _PAGO_LYC_MATCH_RST = nor(top.rst_reg.WESY_SYS_RSTn(), _RYJU_FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ RUPO_LYC_MATCH_LATCHn.nor_latch(_PAGO_LYC_MATCH_RST, top.lcd_reg.ROPO_LY_MATCH_SYNCp());

    /*p21.ROXE*/ ROXE_INT_HBL_EN.set(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D0.q());
    /*p21.RUFO*/ RUFO_INT_VBL_EN.set(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D1.q());
    /*p21.REFE*/ REFE_INT_OAM_EN.set(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D2.q());
    /*p21.RUGU*/ RUGU_INT_LYC_EN.set(_RYVE_FF41_WRn, !_RYVE_FF41_WRn, top.rst_reg.WESY_SYS_RSTn(), top.CPU_TRI_D3.q());

    /*p21.XATY*/ wire _XATY_STAT_MODE1n = nor(_XYMU_RENDERINGp.q(), top.ACYL_SCANNINGp()); // die NOR
    /*p21.SADU*/ wire _SADU_STAT_MODE0n = nor(_XYMU_RENDERINGp.q(), top.lcd_reg.PARU_VBLANKp_d4()); // die NOR

    // OK, these tribufs are _slightly_ different - compare SEGO and SASY, second rung.

    // TEBY facing dot
    // WUGA facing dot
    // SEGO facing dot

    /*p21.TEBY*/ top.CPU_TRI_D0.set_tribuf_6p(_TOBE_FF41_RDp, not(_SADU_STAT_MODE0n));
    /*p21.WUGA*/ top.CPU_TRI_D1.set_tribuf_6p(_TOBE_FF41_RDp, not(_XATY_STAT_MODE1n));
    /*p21.SEGO*/ top.CPU_TRI_D2.set_tribuf_6p(_TOBE_FF41_RDp, not(RUPO_LYC_MATCH_LATCHn.q()));

    // PUZO not facing dot
    // POFO not facing dot
    // SASY not facing dot
    // POTE not facing dot

    /*p21.PUZO*/ top.CPU_TRI_D3.set_tribuf_6n(_VAVE_FF41_RDn, ROXE_INT_HBL_EN.q());
    /*p21.POFO*/ top.CPU_TRI_D4.set_tribuf_6n(_VAVE_FF41_RDn, RUFO_INT_VBL_EN.q());
    /*p21.SASY*/ top.CPU_TRI_D5.set_tribuf_6n(_VAVE_FF41_RDn, REFE_INT_OAM_EN.q());
    /*p21.POTE*/ top.CPU_TRI_D6.set_tribuf_6n(_VAVE_FF41_RDn, RUGU_INT_LYC_EN.q());
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
  /*p21.XYMU*/ hash << _XYMU_RENDERINGp.commit();
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
