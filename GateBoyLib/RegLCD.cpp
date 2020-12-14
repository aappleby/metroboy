#include "RegLCD.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void LcdRegisters::tock(
  wire XODO_VID_RSTp_new_h,
  wire XUPY_ABxxEFxx_clk_evn,
  const RegLX& reg_lx,
  const RegLY& reg_ly)
{
  /* p01.XAPO*/ wire _XAPO_VID_RSTn_new_evn = not1(XODO_VID_RSTp_new_h);
  /* p01.LYHA*/ wire _LYHA_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
  /* p01.LYFE*/ wire _LYFE_VID_RSTn_new_evn = not1(_LYHA_VID_RSTp_new_evn);
  /*#p01.ATAR*/ wire _ATAR_VID_RSTp_new_evn = not1(_XAPO_VID_RSTn_new_evn);
  /*#p01.ABEZ*/ wire _ABEZ_VID_RSTn_new_evn = not1(_ATAR_VID_RSTp_new_evn);

  /*#p28.AWOH*/ wire _AWOH_xxCDxxGH_clknew = not1(XUPY_ABxxEFxx_clk_evn);

  /*#p21.XYVO*/ wire _XYVO_y144p_old_evn = and2(reg_ly.LOVU_LY4p_evn.qp_old(), reg_ly.LAFO_LY7p_evn.qp_old()); // 128 + 16 = 144
  /*#p21.POPU*/ POPU_VBLANKp_evn.dff17(reg_lx.NYPE_x113p_c.qp_new(), _LYFE_VID_RSTn_new_evn, _XYVO_y144p_old_evn);

  /*#p21.PURE*/ wire _PURE_LINE_ENDn_old_evn = not1(reg_lx.RUTU_x113p_g.qp_old());
  /*#p21.SELA*/ wire _SELA_LINE_P908p_old_evn = not1(_PURE_LINE_ENDn_old_evn);
  /*#p29.ALES*/ wire _ALES_y144n_old_evn = not1(_XYVO_y144p_old_evn);
  /*#p29.ABOV*/ wire _ABOV_LINE_P908p_old_evn = and2(_SELA_LINE_P908p_old_evn, _ALES_y144n_old_evn);

  /*#p28.ANEL*/ ANEL_LINE_P002p_c.dff17(_AWOH_xxCDxxGH_clknew,  _ABEZ_VID_RSTn_new_evn,  CATU_LINE_P000p_a.qp_old());
  /*#p29.CATU*/ CATU_LINE_P000p_a.dff17( XUPY_ABxxEFxx_clk_evn,  _ABEZ_VID_RSTn_new_evn, _ABOV_LINE_P908p_old_evn);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------