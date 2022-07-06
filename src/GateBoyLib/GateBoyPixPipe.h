#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct WinControl {
  void reset_to_cart();

  /*_p27.NUKO*/ Gate NUKO_WX_MATCHp_odd; // old used
  /*_p27.ROGE*/ Gate ROGE_WY_MATCHp_odd; // old used
  /*_p27.PYNU*/ NorLatch PYNU_WIN_MODE_Ap_odd;      // AxxxxxGx
  /*_p27.PUKU*/ Gate PUKU_WIN_HITn_odd;             // xxCxxxGx
  /*_p27.RYDY*/ Gate RYDY_WIN_HITp_odd;             // xxCxxxGx
  /*_p27.SOVY*/ DFF17 SOVY_WIN_HITp_evn;            // xxxDxxxH
  /*_p27.NOPA*/ DFF17 NOPA_WIN_MODE_Bp_evn;         // xBxDxFxH
  /*_p27.PYCO*/ DFF17 PYCO_WIN_MATCHp_evn;          // xxxDxxxH
  /*_p27.NUNU*/ DFF17 NUNU_WIN_MATCHp_odd;          // xxxxExGx
  /*_p27.REJO*/ NorLatch REJO_WY_MATCH_LATCHp_odd;  // xxCxxxxx
  /*_p27.SARY*/ DFF17 SARY_WY_MATCHp_odd;           // xxCxxxxx
  /*_p27.RYFA*/ DFF17 RYFA_WIN_FETCHn_A_evn;        // AxCxExGx
  /*_p27.RENE*/ DFF17 RENE_WIN_FETCHn_B_evn;        // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct FineCount {
  void reset_to_cart();

  /*#p27.RYKU*/ DFF17 RYKU_FINE_CNT0_odd;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*#p27.ROGA*/ DFF17 ROGA_FINE_CNT1_odd;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
  /*#p27.RUBU*/ DFF17 RUBU_FINE_CNT2_odd;                // ABCDEFGH Ticks on even clocks, reset on odd clocks.
};

struct FineScroll {
  void reset_to_cart();

  /*#p27.PUXA*/ DFF17 PUXA_SCX_FINE_MATCH_evn;        // xxxxxFxH
  /*#p27.NYZE*/ DFF17 NYZE_SCX_FINE_MATCH_odd;        // AxxxxxGx
  /*#p27.ROXY*/ NorLatch ROXY_FINE_SCROLL_DONEn_evn;    // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct MaskPipe {
  void reset_to_cart();

  /*_p26.VEZO*/ DFF22 VEZO_MASK_PIPE_0; // AxCxExGx
  /*_p26.WURU*/ DFF22 WURU_MASK_PIPE_1; // AxCxExGx
  /*_p26.VOSA*/ DFF22 VOSA_MASK_PIPE_2; // AxCxExGx
  /*_p26.WYFU*/ DFF22 WYFU_MASK_PIPE_3; // AxCxExGx
  /*_p26.XETE*/ DFF22 XETE_MASK_PIPE_4; // AxCxExGx
  /*_p26.WODA*/ DFF22 WODA_MASK_PIPE_5; // AxCxExGx
  /*_p26.VUMO*/ DFF22 VUMO_MASK_PIPE_6; // AxCxExGx
  /*_p26.VAVA*/ DFF22 VAVA_MASK_PIPE_7; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct BgwPipeA {
  void reset_to_cart();

  /*_p32.MYDE*/ DFF22 MYDE_BGW_PIPE_A0; // AxCxExGx
  /*_p32.NOZO*/ DFF22 NOZO_BGW_PIPE_A1; // AxCxExGx
  /*_p32.MOJU*/ DFF22 MOJU_BGW_PIPE_A2; // AxCxExGx
  /*_p32.MACU*/ DFF22 MACU_BGW_PIPE_A3; // AxCxExGx
  /*_p32.NEPO*/ DFF22 NEPO_BGW_PIPE_A4; // AxCxExGx
  /*_p32.MODU*/ DFF22 MODU_BGW_PIPE_A5; // AxCxExGx
  /*_p32.NEDA*/ DFF22 NEDA_BGW_PIPE_A6; // AxCxExGx
  /*_p32.PYBO*/ DFF22 PYBO_BGW_PIPE_A7; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct BgwPipeB {
  void reset_to_cart();

  /*_p32.TOMY*/ DFF22 TOMY_BGW_PIPE_B0; // AxCxExGx
  /*_p32.TACA*/ DFF22 TACA_BGW_PIPE_B1; // AxCxExGx
  /*_p32.SADY*/ DFF22 SADY_BGW_PIPE_B2; // AxCxExGx
  /*_p32.RYSA*/ DFF22 RYSA_BGW_PIPE_B3; // AxCxExGx
  /*_p32.SOBO*/ DFF22 SOBO_BGW_PIPE_B4; // AxCxExGx
  /*_p32.SETU*/ DFF22 SETU_BGW_PIPE_B5; // AxCxExGx
  /*_p32.RALU*/ DFF22 RALU_BGW_PIPE_B6; // AxCxExGx
  /*_p32.SOHU*/ DFF22 SOHU_BGW_PIPE_B7; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct SprPipeA {
  void reset_to_cart();

  /*_p33.NURO*/ DFF22 NURO_SPR_PIPE_A0; // AxCxExGx
  /*_p33.MASO*/ DFF22 MASO_SPR_PIPE_A1; // AxCxExGx
  /*_p33.LEFE*/ DFF22 LEFE_SPR_PIPE_A2; // AxCxExGx
  /*_p33.LESU*/ DFF22 LESU_SPR_PIPE_A3; // AxCxExGx
  /*_p33.WYHO*/ DFF22 WYHO_SPR_PIPE_A4; // AxCxExGx
  /*_p33.WORA*/ DFF22 WORA_SPR_PIPE_A5; // AxCxExGx
  /*_p33.VAFO*/ DFF22 VAFO_SPR_PIPE_A6; // AxCxExGx
  /*_p33.WUFY*/ DFF22 WUFY_SPR_PIPE_A7; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct SprPipeB {
  void reset_to_cart();

  /*_p33.NYLU*/ DFF22 NYLU_SPR_PIPE_B0; // AxCxExGx
  /*_p33.PEFU*/ DFF22 PEFU_SPR_PIPE_B1; // AxCxExGx
  /*_p33.NATY*/ DFF22 NATY_SPR_PIPE_B2; // AxCxExGx
  /*_p33.PYJO*/ DFF22 PYJO_SPR_PIPE_B3; // AxCxExGx
  /*_p33.VARE*/ DFF22 VARE_SPR_PIPE_B4; // AxCxExGx
  /*_p33.WEBA*/ DFF22 WEBA_SPR_PIPE_B5; // AxCxExGx
  /*_p33.VANU*/ DFF22 VANU_SPR_PIPE_B6; // AxCxExGx
  /*_p33.VUPY*/ DFF22 VUPY_SPR_PIPE_B7; // AxCxExGx
};

//-----------------------------------------------------------------------------

struct PalPipe {
  void reset_to_cart();

  /*_p34.RUGO*/ DFF22 RUGO_PAL_PIPE_D0;
  /*_p34.SATA*/ DFF22 SATA_PAL_PIPE_D1;
  /*_p34.ROSA*/ DFF22 ROSA_PAL_PIPE_D2;
  /*_p34.SOMY*/ DFF22 SOMY_PAL_PIPE_D3;
  /*_p34.PALU*/ DFF22 PALU_PAL_PIPE_D4;
  /*_p34.NUKE*/ DFF22 NUKE_PAL_PIPE_D5;
  /*_p34.MODA*/ DFF22 MODA_PAL_PIPE_D6;
  /*_p34.LYME*/ DFF22 LYME_PAL_PIPE_D7;
};

//-----------------------------------------------------------------------------
