#include "Sch_PPU.h"

#include "Sch_Top.h"

using namespace Schematics;

// Die trace
// NUNY04 = and(PYNU04, NOPA16)
// NYFO02 = not(NUNY04)
// MOSU03 = not(NYFO02) // 3 rung inverter
// ABAF02 = not(CATU17)

// BYHA = weirdgate
// BYHA01 << ANEL17
// BYHA02 << ABAF02
// BYHA03 xx CATU17 (crossover?)
// BYHA04 << ABEZ02
// BYHA05 >> ATEJ01

// ATEJ03 = not(BYHA05)
// ANOM03 = nor(ATEJ03, ATAR02)
// BALU02 = not(ANOM03)
// BEBU05 = or(DOBA17, BALU02, BYBA16)
// AVAP03 = not(BEBU05)
// NYXU04 = nor(AVAP03, MOSU03, TEVO05)

// Die trace:

// WUSA arms on the ground side, nor latch
// WUSA00 << XAJO03
// WUSA01 nc
// WUSA02 >> nc
// WUSA03 >> TOBA00
// WUSA04 nc 
// WUSA05 << WEGO03

// When XAJO03 goes high, WUSA03 goes high.
// When WEGO03 goes high, WUSA03 goes low.

// XEHO01 <> XEHO12
// XEHO02 << SACU04
// XEHO03 <> XEHO09
// XEHO04 nc
// XEHO05 nc
// XEHO06 << XYMU03
// XEHO07 << XEHO16 // so it's a counter
// XEHO08 nc
// XEHO09 <> XEHO03
// XEHO10 nc
// XEHO11 ?? RUMA01
// XEHO12 <> XEHO01
// XEHO13 << XYMU03
// XEHO14 nc
// XEHO15 nc
// XEHO16
// XEHO17 >> XAJO01

// ROXY = NOR latch
// ROXY00 << PAHA
// ROXY01 nc
// ROXY02 >> nc
// ROXY03 >> RONE00
// ROXY04 nc
// ROXY05 << POVA

// If PAHA goes high, ROXY03 goes high.
// If POVA goes high, ROXY03 goes low

// LURY01 << LOVY15 (next to bottom rung)
// LURY02 << XYMU02
// LURY03 nc
// LURY04 >> LONY01

// LONY has "arms" on the VCC side - different from the other latches?
// Probably a NAND latch instead of NOR
// LONY01 << LURY03
// LONY02 nc
// LONY03 == nc
// LONY04 >> LUSU01, MYMA01
// LONY05 nc
// LONY06 << NYXU03

// if LURY goes low, LONY goes low
// if NYXU goes low, LONY goes high

// XYMU has "arms" on the ground side
// XYMU00 << WEGO03
// XYMU01 nc
// XYMU02 >> bunch of stuff
// XYMU03 >> nc
// XYMU04 nc
// XYMU05 << AVAP02

// if AVAP02 goes high, XYMU02 goes high.
// if WEGO03 goes high, XYMU02 goes low.

// TAKA has arms on the VCC side - nand latch
// TAKA01 << VEKU02
// TAKA02 nc
// TAKA03 >> nc
// TAKA04 >> SOWO00
// TAKA05 nc
// TAKA06 << SECA03
// if SECA03 goes low, TAKA04 goes high
// if VEKU02 goes low, TAKA04 goes low

//------------------------------------------------------------------------------

void PpuRegisters::tick(SchematicTop& top) {

  /*p22.XOLA*/ wire XOLA_A00n = not(top.CPU_PIN_A00);
  /*p22.XENO*/ wire XENO_A01n = not(top.CPU_PIN_A01);
  /*p22.XUSY*/ wire XUSY_A02n = not(top.CPU_PIN_A02);
  /*p22.XERA*/ wire XERA_A03n = not(top.CPU_PIN_A03);

  /*p22.WADO*/ wire WADO_A00p = not(XOLA_A00n);

  {
    /*p01.TOFU*/ wire TOFU_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p21.WEGO*/ wire _WEGO_RST_LATCH = or(TOFU_VID_RSTp, VOGA_RENDER_DONE_SYNC);
    /*p21.XYMU*/ XYMU_RENDERINGp.nor_latch(top.AVAP_SCAN_DONE_TRIGp(), _WEGO_RST_LATCH);
  }


  {
    
    /*p01.TOFU*/ wire TOFU_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p21.WEGO*/ wire _WEGO_RST_LATCH = or(TOFU_VID_RSTp, VOGA_RENDER_DONE_SYNC);
    /*p27.POVA*/ wire _POVA_FINE_MATCH_SETp = and (PUXA_FINE_MATCH_Ap, !NYZE_FINE_MATCH_Bp);
    // XAJO04 = and(XEHO17, XYDO17);
    /*p21.XAJO*/ wire _XAJO_X_009 = and (XEHO_X0.q(), XYDO_X3.q());
    /*p21.WUSA*/ WUSA_CPEN_LATCH.nor_latch(_XAJO_X_009, _WEGO_RST_LATCH);
    /*p21.TOBA*/ wire _TOBA = and (top.SACU_CLKPIPEp(), WUSA_CPEN_LATCH);
    /*p21.SEMU*/ wire _SEMU_LCD_CPn = or(_TOBA, _POVA_FINE_MATCH_SETp);
    /*p21.RYPO*/ wire _RYPO_LCD_CP = not(_SEMU_LCD_CPn);
    top.LCD_PIN_CP.set(_RYPO_LCD_CP);
  }

  {
    /*p24.ROXO*/ wire ROXO_CLKPIPEp = not(top.SEGU_CLKPIPEn());
    /*p27.PECU*/ wire PECU_FINE_CLK = nand(ROXO_CLKPIPEp, top.ROZE_FINE_COUNT_STOPn());
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(XYMU_RENDERINGp);
    /*p27.PASO*/ wire PASO_FINE_RST = nor(top.TEVO_FINE_RSTp(), ROPY_RENDERINGn);
    /*p27.RYKU*/ RYKU_FINE_CNT0.set(PECU_FINE_CLK,   PASO_FINE_RST, !RYKU_FINE_CNT0);
    /*p27.ROGA*/ ROGA_FINE_CNT1.set(!RYKU_FINE_CNT0, PASO_FINE_RST, !ROGA_FINE_CNT1);
    /*p27.RUBU*/ RUBU_FINE_CNT2.set(!ROGA_FINE_CNT1, PASO_FINE_RST, !RUBU_FINE_CNT2);
  }

  {
    /*p24.ROXO*/ wire ROXO_CLKPIPEp = not(top.SEGU_CLKPIPEn());
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.SYS_PIN_CLK_xBxDxFxH);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
    /*p27.MOXE*/ wire MOXE_AxCxExGx = not(ALET_xBxDxFxH);

    /*p27.SUHA*/ wire SUHA_FINE_MATCH0p = xnor(top.DATY_SCX0(), RYKU_FINE_CNT0); // Arms on the ground side, XNOR
    /*p27.SYBY*/ wire SYBY_FINE_MATCH1p = xnor(top.DUZU_SCX1(), ROGA_FINE_CNT1);
    /*p27.SOZU*/ wire SOZU_FINE_MATCH2p = xnor(top.CYXU_SCX2(), RUBU_FINE_CNT2);
    /*p27.RONE*/ wire RONE_FINE_MATCHn = nand(ROXY_FINE_MATCH_LATCHn, SUHA_FINE_MATCH0p, SYBY_FINE_MATCH1p, SOZU_FINE_MATCH2p);
    /*p27.POHU*/ wire POHU_FINE_MATCHp = not(RONE_FINE_MATCHn);
    /*p27.PUXA*/ PUXA_FINE_MATCH_Ap.set(ROXO_CLKPIPEp, XYMU_RENDERINGp, POHU_FINE_MATCHp);
    /*p27.NYZE*/ NYZE_FINE_MATCH_Bp.set(MOXE_AxCxExGx, XYMU_RENDERINGp, PUXA_FINE_MATCH_Ap);

    /*p27.POVA*/ wire POVA_FINE_MATCH_SETp = and (PUXA_FINE_MATCH_Ap, !NYZE_FINE_MATCH_Bp);
    /*p27.PAHA*/ wire PAHA_FINE_MATCH_RSTp = not(XYMU_RENDERINGp);
    /*p27.ROXY*/ ROXY_FINE_MATCH_LATCHn.nor_latch(PAHA_FINE_MATCH_RSTp, POVA_FINE_MATCH_SETp);
  }
  {
    // LCD horizontal sync pin latch
    // if AVAP goes high, POFY goes high.
    // if PAHO or TOFU go high, POFY goes low.

    /*p01.TOFU*/ wire TOFU_VID_RSTp = not(top.XAPO_VID_RSTn());

    /*p24.ROXO*/ wire ROXO_CLKPIPEp = not(top.SEGU_CLKPIPEn());
    /*p24.PAHO*/ PAHO_X_8_SYNC.set(ROXO_CLKPIPEp, top.XYMU_RENDERINGp(), XYDO_X3);
    /*p24.RUJU*/ POFY_ST_LATCH.nor_latch(top.AVAP_SCAN_DONE_TRIGp(), PAHO_X_8_SYNC || TOFU_VID_RSTp);
    /*p24.RUZE*/ wire RUZE_PIN_ST = not(POFY_ST_LATCH);
    top.LCD_PIN_ST.set(RUZE_PIN_ST);
  }

  //----------

  {
    // vid x position, has carry lookahead because this increments every tcycle
    /*p21.RYBO*/ wire RYBO = xor(XEHO_X0, SAVY_X1);
    /*p21.XUKE*/ wire XUKE = and(XEHO_X0, SAVY_X1);

    /*p21.XYLE*/ wire XYLE = and(XODU_X2, XUKE);
    /*p21.XEGY*/ wire XEGY = xor(XODU_X2, XUKE);
    /*p21.XORA*/ wire XORA = xor(XYDO_X3, XYLE);

    /*p21.SAKE*/ wire SAKE = xor(TUHU_X4, TUKY_X5);
    /*p21.TYBA*/ wire TYBA = and(TUHU_X4, TUKY_X5);
    /*p21.SURY*/ wire SURY = and(TAKO_X6, TYBA);
    /*p21.TYGE*/ wire TYGE = xor(TAKO_X6, TYBA);
    /*p21.ROKU*/ wire ROKU = xor(SYBE_X7, SURY);

    
    /*p01.TOFU*/ wire TOFU_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p21.TADY*/ wire TADY_X_RSTn = nor(top.BYHA_VID_LINE_TRIG_d4n(), TOFU_VID_RSTp);

    /*p24.TOCA*/ wire TOCA_CLKPIPE_HI = not(XYDO_X3);

    /*p21.XEHO*/ XEHO_X0.set(top.SACU_CLKPIPEp(), TADY_X_RSTn, !XEHO_X0);
    /*p21.SAVY*/ SAVY_X1.set(top.SACU_CLKPIPEp(), TADY_X_RSTn, RYBO);
    /*p21.XODU*/ XODU_X2.set(top.SACU_CLKPIPEp(), TADY_X_RSTn, XEGY);
    /*p21.XYDO*/ XYDO_X3.set(top.SACU_CLKPIPEp(), TADY_X_RSTn, XORA);
    /*p21.TUHU*/ TUHU_X4.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, !TUHU_X4);
    /*p21.TUKY*/ TUKY_X5.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, SAKE);
    /*p21.TAKO*/ TAKO_X6.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, TYGE);
    /*p21.SYBE*/ SYBE_X7.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, ROKU);
  }

  {
    
    /*p01.TOFU*/ wire TOFU_VID_RSTp = not(top.XAPO_VID_RSTn());
    /*p21.TADY*/ wire TADY_X_RST = nor(top.BYHA_VID_LINE_TRIG_d4n(), TOFU_VID_RSTp);
    // having this reset connected to both RENDER_DONE_SYNC and x seems odd
    /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.SYS_PIN_CLK_xBxDxFxH);
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
    /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
    /*p21.VOGA*/ VOGA_RENDER_DONE_SYNC.set(ALET_xBxDxFxH, TADY_X_RST, top.WODU_RENDER_DONEp());
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

    /*p22.WOFA*/ wire WOFA_FF41n = nand(top.WERO_FF40_FF4Fp(), WADO_A00p, XENO_A01n, XUSY_A02n, XERA_A03n);
    /*p22.VARY*/ wire VARY_FF41p = not(WOFA_FF41n);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p21.TOBE*/ wire TOBE_FF41_RDa = and (ASOT_CPU_RD, VARY_FF41p); // die AND
    /*p21.VAVE*/ wire VAVE_FF41_RDb = not(TOBE_FF41_RDa); // die INV

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p21.SEPA*/ wire SEPA_FF41_WRp = and (CUPA_CPU_WR_xxxxxFGH, VARY_FF41p);
    /*p21.RYVE*/ wire RYVE_FF41_WRn = not(SEPA_FF41_WRp);

    /*p21.RYJU*/ wire RYJU_FF41_WRn = not(SEPA_FF41_WRp);
    
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.WESY*/ wire WESY_RSTn = not(XORE_RSTp);
    /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(WESY_RSTn, RYJU_FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ RUPO_LYC_MATCH_LATCHn.nor_latch(PAGO_LYC_MATCH_RST, top.ROPO_LY_MATCH_SYNCp());

    /*p21.ROXE*/ ROXE_INT_HBL_EN.set(RYVE_FF41_WRn, WESY_RSTn, top.CPU_TRI_D0);
    /*p21.RUFO*/ RUFO_INT_VBL_EN.set(RYVE_FF41_WRn, WESY_RSTn, top.CPU_TRI_D1);
    /*p21.REFE*/ REFE_INT_OAM_EN.set(RYVE_FF41_WRn, WESY_RSTn, top.CPU_TRI_D2);
    /*p21.RUGU*/ RUGU_INT_LYC_EN.set(RYVE_FF41_WRn, WESY_RSTn, top.CPU_TRI_D3);

    /*p21.PARU*/ wire PARU_IN_VBLANK = not(!top.POPU_VBLANK_d4());
    /*p21.XATY*/ wire XATY_STAT_MODE1n = nor(XYMU_RENDERINGp, top.ACYL_SCANNINGp()); // die NOR
    /*p21.SADU*/ wire SADU_STAT_MODE0n = nor(XYMU_RENDERINGp, PARU_IN_VBLANK); // die NOR

    // OK, these tribufs are _slightly_ different - compare SEGO and SASY, second rung.
    /*p21.TEBY*/ top.CPU_TRI_D0.set_tribuf(TOBE_FF41_RDa, not(SADU_STAT_MODE0n));
    /*p21.WUGA*/ top.CPU_TRI_D1.set_tribuf(TOBE_FF41_RDa, not(XATY_STAT_MODE1n));
    /*p21.SEGO*/ top.CPU_TRI_D2.set_tribuf(TOBE_FF41_RDa, not(RUPO_LYC_MATCH_LATCHn.q()));
    /*p21.PUZO*/ top.CPU_TRI_D3.set_tribuf(not(VAVE_FF41_RDb), ROXE_INT_HBL_EN.q());
    /*p21.POFO*/ top.CPU_TRI_D4.set_tribuf(not(VAVE_FF41_RDb), RUFO_INT_VBL_EN.q());
    /*p21.SASY*/ top.CPU_TRI_D5.set_tribuf(not(VAVE_FF41_RDb), REFE_INT_OAM_EN.q());
    /*p21.POTE*/ top.CPU_TRI_D6.set_tribuf(not(VAVE_FF41_RDb), RUGU_INT_LYC_EN.q());
  }
}

//------------------------------------------------------------------------------

SignalHash PpuRegisters::commit() {
  SignalHash hash;
  /*p??.ROXY*/ hash << ROXY_FINE_MATCH_LATCHn.commit_latch();
  /*p??.PUXA*/ hash << PUXA_FINE_MATCH_Ap.commit_reg();
  /*p27.NYZE*/ hash << NYZE_FINE_MATCH_Bp.commit_reg();
  /*p27.RYKU*/ hash << RYKU_FINE_CNT0.commit_reg();
  /*p27.ROGA*/ hash << ROGA_FINE_CNT1.commit_reg();
  /*p27.RUBU*/ hash << RUBU_FINE_CNT2.commit_reg();
  /*p21.XEHO*/ hash << XEHO_X0.commit_reg();
  /*p21.SAVY*/ hash << SAVY_X1.commit_reg();
  /*p21.XODU*/ hash << XODU_X2.commit_reg();
  /*p21.XYDO*/ hash << XYDO_X3.commit_reg();
  /*p21.TUHU*/ hash << TUHU_X4.commit_reg();
  /*p21.TUKY*/ hash << TUKY_X5.commit_reg();
  /*p21.TAKO*/ hash << TAKO_X6.commit_reg();
  /*p21.SYBE*/ hash << SYBE_X7.commit_reg();
  /*p21.XYMU*/ hash << XYMU_RENDERINGp.commit_latch();
  /*p21.VOGA*/ hash << VOGA_RENDER_DONE_SYNC.commit_reg();

  /*p21.RUPO*/ hash << RUPO_LYC_MATCH_LATCHn.commit_latch();


  /*p24.PAHO*/ hash << PAHO_X_8_SYNC.commit_reg();
  /*p24.RUJU*/ hash << POFY_ST_LATCH.commit_latch(); // nor latch with p24.RUJU, p24.POME
  /*p21.WUSA*/ hash << WUSA_CPEN_LATCH.commit_latch();

  // FF41 - STAT
  /*p21.ROXE*/ hash << ROXE_INT_HBL_EN.commit_reg();
  /*p21.RUFO*/ hash << RUFO_INT_VBL_EN.commit_reg();
  /*p21.REFE*/ hash << REFE_INT_OAM_EN.commit_reg();
  /*p21.RUGU*/ hash << RUGU_INT_LYC_EN.commit_reg();

  return hash;
}

//------------------------------------------------------------------------------
