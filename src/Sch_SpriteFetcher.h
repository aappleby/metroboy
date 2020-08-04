#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void tick(const SchematicTop& gb);
  void tock(SchematicTop& gb);
  void dump(Dumper& d) const;

  // sfetch.veku, sst.store*_rstp, pxp.XEPY
  /*p29.WUTY*/ wire WUTY_SPRITE_DONEp() const { return not1(VUSA_SPRITE_DONEn()); }

  // -> oam bus
  /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK() const {
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(_TYFO_SFETCH_S0_D1.qp(), TYTU_SFETCH_S0n());
    return and2(TUVO_PPU_OAM_RDp(), TACU_SPR_SEQ_5_TRIG);
  }

  // -> oam bus
  /*p28.WEFY*/ wire WEFY_SPR_READp() const { return and2(TUVO_PPU_OAM_RDp(), _TYFO_SFETCH_S0_D1.qp()); }

  // -> vram bus
  /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG() const { return nand2(_TYFO_SFETCH_S0_D1.qp(), TYTU_SFETCH_S0n()); }

  // -> vram bus
  /*p29.TEXY*/ wire TEXY_SPR_READ_VRAMp() const {
    /*p29.SAKY*/ wire _SAKY_SFETCH_MYSTERY = nor2(_TULY_SFETCH_S1.qp(), _VONU_SFETCH_S1_D4.qp());
    /*p29.TYSO*/ wire TYSO_SPRITE_READn = or2(_SAKY_SFETCH_MYSTERY, _TEPA_RENDERINGn); // def or
    return not1(TYSO_SPRITE_READn);
  }

  // -> vram bus
  /*p29.RACA*/ wire RACA_LATCH_SPPIXB() const { return and2(_VONU_SFETCH_S1_D4.qp(), SYCU_SFETCH_S0pe()); }

  // -> vram bus
  /*p29.TOPU*/ wire TOPU_LATCH_SPPIXA() const { return and2(_TULY_SFETCH_S1.qp(), SYCU_SFETCH_S0pe()); }

  // -> vram bus
  /*p29.XUQU*/ wire XUQU_SPRITE_AB() const { return not1(!_VONU_SFETCH_S1_D4.qp()); }

private:
  /*p29.SYCU*/ wire SYCU_SFETCH_S0pe() const { return nor3(TYTU_SFETCH_S0n(), _LOBY_RENDERINGn, _TYFO_SFETCH_S0_D1.qp()); }
  /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp() const { return nor3(_TEPA_RENDERINGn, _TULY_SFETCH_S1.qp(), _TESE_SFETCH_S2.qp()); }
  /*p29.TYTU*/ wire TYTU_SFETCH_S0n()     const { return not1(_TOXE_SFETCH_S0.qp()); }
  /*p29.VUSA*/ wire VUSA_SPRITE_DONEn() const {
    /*p29.TYNO*/ wire TYNO = nand3(_TOXE_SFETCH_S0.qp(), _SEBA_SFETCH_S1_D5.qp(), _VONU_SFETCH_S1_D4.qp());
    return or2(_TYFO_SFETCH_S0_D1.qn(), TYNO);
  }

  Sig _TEPA_RENDERINGn;
  Sig _LOBY_RENDERINGn;

  /*p27.TAKA*/ Tri _TAKA_SFETCH_RUNNINGp = TRI_D0NP;
  /*p27.SOBU*/ RegQP _SOBU_SFETCH_REQp = REG_D0C0;
  /*p27.SUDA*/ RegQN _SUDA_SFETCH_REQp = REG_D0C0;

  // When TOXE & TYFO & VONU & SEBA, sprites are loaded into the pipe.

  /*p29.TOXE*/ RegQPN _TOXE_SFETCH_S0 = REG_D0C0;
  /*p29.TYFO*/ RegQPN _TYFO_SFETCH_S0_D1 = REG_D0C0;

  /*p29.TULY*/ RegQPN _TULY_SFETCH_S1 = REG_D0C0;
  /*p29.TESE*/ RegQPN _TESE_SFETCH_S2 = REG_D0C0;

  /*p29.TOBU*/ RegQP  _TOBU_SFETCH_S1_D2 = REG_D0C0;
  /*p29.VONU*/ RegQPN _VONU_SFETCH_S1_D4 = REG_D0C0;
  /*p29.SEBA*/ RegQP  _SEBA_SFETCH_S1_D5 = REG_D0C0;

};


//-----------------------------------------------------------------------------

}; // namespace Schematics