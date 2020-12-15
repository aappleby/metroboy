#include "GateBoyLib/GateBoyTimer.h"

#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

void GateBoyDiv::tock(
  GateBoyResetDebug& rst,
  GateBoyPhaseClock& pclk,
  GateBoyCpuBus& cpu_bus)
{

  /* p01.TAGY*/ wire _TAGY_FF04_RDp_ext    = and4(cpu_bus.TEDO_CPU_RDp, cpu_bus.RYFO_FF04_FF07p(), cpu_bus.TOLA_A01n(), cpu_bus.TOVY_A00n());
  /* p01.TAPE*/ wire _TAPE_FF04_WRp_clkevn = and4(cpu_bus.TAPU_CPU_WRp, cpu_bus.RYFO_FF04_FF07p(), cpu_bus.TOLA_A01n(), cpu_bus.TOVY_A00n());

  /* p01.UFOL*/ wire _UFOL_DIV_RSTn_evn_ext = nor3(pclk.UCOB_CLKBADp(), rst.PIN_SYS_RST.qp_new(), _TAPE_FF04_WRp_clkevn);

  /* p01.UKUP*/ UKUP_DIV00p.dff17(pclk.BOGA_Axxxxxxx(),     _UFOL_DIV_RSTn_evn_ext, UKUP_DIV00p.qn_any());
  /* p01.UFOR*/ UFOR_DIV01p.dff17(UKUP_DIV00p.qn_new(), _UFOL_DIV_RSTn_evn_ext, UFOR_DIV01p.qn_any());
  /* p01.UNER*/ UNER_DIV02p.dff17(UFOR_DIV01p.qn_new(), _UFOL_DIV_RSTn_evn_ext, UNER_DIV02p.qn_any());
  /*#p01.TERO*/ TERO_DIV03p.dff17(UNER_DIV02p.qn_new(), _UFOL_DIV_RSTn_evn_ext, TERO_DIV03p.qn_any());
  /* p01.UNYK*/ UNYK_DIV04p.dff17(TERO_DIV03p.qn_new(), _UFOL_DIV_RSTn_evn_ext, UNYK_DIV04p.qn_any());
  /* p01.TAMA*/ TAMA_DIV05p.dff17(UNYK_DIV04p.qn_new(), _UFOL_DIV_RSTn_evn_ext, TAMA_DIV05p.qn_any());

  // this is hacked up because we're ignoring the debug reg for the moment
  /* p01.ULUR*/ wire _ULUR_DIV_06_clknew = /*mux2p(FF60_1, _BOGA_Axxxxxxx,*/ TAMA_DIV05p.qn_new() /*)*/;

  /* p01.UGOT*/ UGOT_DIV06p.dff17(_ULUR_DIV_06_clknew,      _UFOL_DIV_RSTn_evn_ext, UGOT_DIV06p.qn_any());
  /* p01.TULU*/ TULU_DIV07p.dff17(UGOT_DIV06p.qn_new(), _UFOL_DIV_RSTn_evn_ext, TULU_DIV07p.qn_any());
  /* p01.TUGO*/ TUGO_DIV08p.dff17(TULU_DIV07p.qn_new(), _UFOL_DIV_RSTn_evn_ext, TUGO_DIV08p.qn_any());
  /* p01.TOFE*/ TOFE_DIV09p.dff17(TUGO_DIV08p.qn_new(), _UFOL_DIV_RSTn_evn_ext, TOFE_DIV09p.qn_any());
  /* p01.TERU*/ TERU_DIV10p.dff17(TOFE_DIV09p.qn_new(), _UFOL_DIV_RSTn_evn_ext, TERU_DIV10p.qn_any());
  /* p01.SOLA*/ SOLA_DIV11p.dff17(TERU_DIV10p.qn_new(), _UFOL_DIV_RSTn_evn_ext, SOLA_DIV11p.qn_any());
  /* p01.SUBU*/ SUBU_DIV12p.dff17(SOLA_DIV11p.qn_new(), _UFOL_DIV_RSTn_evn_ext, SUBU_DIV12p.qn_any());
  /* p01.TEKA*/ TEKA_DIV13p.dff17(SUBU_DIV12p.qn_new(), _UFOL_DIV_RSTn_evn_ext, TEKA_DIV13p.qn_any());
  /* p01.UKET*/ UKET_DIV14p.dff17(TEKA_DIV13p.qn_new(), _UFOL_DIV_RSTn_evn_ext, UKET_DIV14p.qn_any());
  /* p01.UPOF*/ UPOF_DIV15p.dff17(UKET_DIV14p.qn_new(), _UFOL_DIV_RSTn_evn_ext, UPOF_DIV15p.qn_any());

  /* FF04 DIV */
  /* p01.UMEK*/ wire _UMEK_DIV06n_evn = not1(UGOT_DIV06p.qp_new());
  /* p01.UREK*/ wire _UREK_DIV07n_evn = not1(TULU_DIV07p.qp_new());
  /* p01.UTOK*/ wire _UTOK_DIV08n_evn = not1(TUGO_DIV08p.qp_new());
  /* p01.SAPY*/ wire _SAPY_DIV09n_evn = not1(TOFE_DIV09p.qp_new());
  /* p01.UMER*/ wire _UMER_DIV10n_evn = not1(TERU_DIV10p.qp_new());
  /* p01.RAVE*/ wire _RAVE_DIV11n_evn = not1(SOLA_DIV11p.qp_new());
  /* p01.RYSO*/ wire _RYSO_DIV12n_evn = not1(SUBU_DIV12p.qp_new());
  /* p01.UDOR*/ wire _UDOR_DIV13n_evn = not1(TEKA_DIV13p.qp_new());

  /* p01.TAWU*/ cpu_bus.BUS_CPU_D_out[0].tri6_pn(_TAGY_FF04_RDp_ext, _UMEK_DIV06n_evn);
  /* p01.TAKU*/ cpu_bus.BUS_CPU_D_out[1].tri6_pn(_TAGY_FF04_RDp_ext, _UREK_DIV07n_evn);
  /* p01.TEMU*/ cpu_bus.BUS_CPU_D_out[2].tri6_pn(_TAGY_FF04_RDp_ext, _UTOK_DIV08n_evn);
  /* p01.TUSE*/ cpu_bus.BUS_CPU_D_out[3].tri6_pn(_TAGY_FF04_RDp_ext, _SAPY_DIV09n_evn);
  /* p01.UPUG*/ cpu_bus.BUS_CPU_D_out[4].tri6_pn(_TAGY_FF04_RDp_ext, _UMER_DIV10n_evn); // Schematic wrong, UPUG/SEPU driving D5/D4
  /* p01.SEPU*/ cpu_bus.BUS_CPU_D_out[5].tri6_pn(_TAGY_FF04_RDp_ext, _RAVE_DIV11n_evn);
  /* p01.SAWA*/ cpu_bus.BUS_CPU_D_out[6].tri6_pn(_TAGY_FF04_RDp_ext, _RYSO_DIV12n_evn);
  /* p01.TATU*/ cpu_bus.BUS_CPU_D_out[7].tri6_pn(_TAGY_FF04_RDp_ext, _UDOR_DIV13n_evn);
}
