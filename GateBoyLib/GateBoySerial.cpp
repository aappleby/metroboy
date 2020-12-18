#include "GateBoyLib/GateBoySerial.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyTimer.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoySerial::read(GateBoyCpuBus& cpu_bus)
{
  /* p06.UFEG*/ wire _UFEG_FF01_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.TOLA_A01n(),        cpu_bus.BUS_CPU_A[ 0].qp());
  /* p06.UCOM*/ wire _UCOM_FF02_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.BUS_CPU_A[ 1].qp(), cpu_bus.TOVY_A00n());

  /* FF01 SER */
  /*#p06.CUGY*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_UFEG_FF01_RDp, CUBA_SER_DATA0.qn_new());
  /* p06.DUDE*/ cpu_bus.BUS_CPU_D[1].tri6_pn(_UFEG_FF01_RDp, DEGU_SER_DATA1.qn_new());
  /* p06.DETU*/ cpu_bus.BUS_CPU_D[2].tri6_pn(_UFEG_FF01_RDp, DYRA_SER_DATA2.qn_new());
  /* p06.DASO*/ cpu_bus.BUS_CPU_D[3].tri6_pn(_UFEG_FF01_RDp, DOJO_SER_DATA3.qn_new());
  /* p06.DAME*/ cpu_bus.BUS_CPU_D[4].tri6_pn(_UFEG_FF01_RDp, DOVU_SER_DATA4.qn_new());
  /* p06.EVOK*/ cpu_bus.BUS_CPU_D[5].tri6_pn(_UFEG_FF01_RDp, EJAB_SER_DATA5.qn_new());
  /* p06.EFAB*/ cpu_bus.BUS_CPU_D[6].tri6_pn(_UFEG_FF01_RDp, EROD_SER_DATA6.qn_new());
  /*#p06.ETAK*/ cpu_bus.BUS_CPU_D[7].tri6_pn(_UFEG_FF01_RDp, EDER_SER_DATA7.qn_new());

  /* FF02 SER */
  /* p06.CORE*/ cpu_bus.BUS_CPU_D[0].tri6_pn(_UCOM_FF02_RDp, CULY_SER_DIR.qn_new());
  /* p06.ELUV*/ cpu_bus.BUS_CPU_D[7].tri6_pn(_UCOM_FF02_RDp, ETAF_SER_RUNNING.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySerial::write_old(GateBoyCpuBus& cpu_bus)
{
  /* p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.BUS_CPU_A[ 1].qp(), cpu_bus.TOVY_A00n());

  /* p06.CULY*/ CULY_SER_DIR    .dff17_clk(_UWAM_FF02_WRn, cpu_bus.BUS_CPU_D[0].qp_old());
  /* p06.ETAF*/ ETAF_SER_RUNNING.dff17_clk(_UWAM_FF02_WRn, cpu_bus.BUS_CPU_D[7].qp_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySerial::tock2(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, GateBoyDiv& div)
{
  {
    /* p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.BUS_CPU_A[ 1].qp(), cpu_bus.TOVY_A00n());
    /* p06.COTY*/ COTY_SER_CLK.dff17(div.TAMA_DIV05p.qp_new(), _UWAM_FF02_WRn, COTY_SER_CLK.qn_old());
  }

  {
    /* p06.KEXU*/ wire _KEXU_new = nand2(COTY_SER_CLK.qp_new(), CULY_SER_DIR.qp_new());
    /* p06.JAGO*/ wire _JAGO_new =  not1(CULY_SER_DIR.qp_new());
    /* p06.KUJO*/ wire _KUJO_new =  nor2(COTY_SER_CLK.qp_new(), _JAGO_new);

    PIN_SCK.pin_out(CULY_SER_DIR.qp_new(), _KEXU_new, _KUJO_new);
  }

  {
    /* p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(CULY_SER_DIR.qp_new(), COTY_SER_CLK.qp_new(), PIN_SCK.qn_new());
    /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ETAF_SER_RUNNING.qn_new());
    /* p06.EDYL*/ wire _EDYL_SER_CLK_new = not1(_DAWA_SER_CLK);
    /* p06.ELYS*/ ELYS_SER_OUT  .dff17(_EDYL_SER_CLK_new, rst.ALUR_SYS_RSTn(), EDER_SER_DATA7.qp_old());
    /* p05.KENA*/ wire _KENA = /*mux2n(KUKO_DBG_FF00_D6,*/ ELYS_SER_OUT.qp_new() /*, FF60_0)*/; // FIXME hacking out debug stuff

    PIN_SOUT.pin_out(_KENA, _KENA);
  }

  {
    /* p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.BUS_CPU_A[ 1].qp_new(), cpu_bus.TOVY_A00n());
    /* p06.CARO*/ wire _CARO_SER_RSTn = and2(_UWAM_FF02_WRn, rst.ALUR_SYS_RSTn());
    /* p06.CAFA*/ CAFA_SER_CNT0.RSTn(_CARO_SER_RSTn);
    /* p06.CYLO*/ CYLO_SER_CNT1.RSTn(_CARO_SER_RSTn);
    /* p06.CYDE*/ CYDE_SER_CNT2.RSTn(_CARO_SER_RSTn);
    /* p06.CALY*/ CALY_SER_CNT3.RSTn(_CARO_SER_RSTn);

    /* p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(CULY_SER_DIR.qp_new(), COTY_SER_CLK.qp_new(), PIN_SCK.qn_new());
    /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ETAF_SER_RUNNING.qn_new());

    /* p06.CAFA*/ CAFA_SER_CNT0.dff17(_DAWA_SER_CLK,          _CARO_SER_RSTn, CAFA_SER_CNT0.qn_new());
    /* p06.CYLO*/ CYLO_SER_CNT1.dff17(CAFA_SER_CNT0.qn_new(), _CARO_SER_RSTn, CYLO_SER_CNT1.qn_new());
    /* p06.CYDE*/ CYDE_SER_CNT2.dff17(CYLO_SER_CNT1.qn_new(), _CARO_SER_RSTn, CYDE_SER_CNT2.qn_new());
    /* p06.CALY*/ CALY_SER_CNT3.dff17(CYDE_SER_CNT2.qn_new(), _CARO_SER_RSTn, CALY_SER_CNT3.qn_new());
  }

  {
    /* p06.COBA*/ wire _COBA_SER_CNT3n = not1(CALY_SER_CNT3.qp_new());
    /* p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, rst.ALUR_SYS_RSTn());

    /* p06.CULY*/ CULY_SER_DIR    .dff17_rst(rst.ALUR_SYS_RSTn());
    /* p06.ETAF*/ ETAF_SER_RUNNING.dff17_rst(_CABY_XFER_RESET);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySerial::write_new(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus)
{
  /* p06.URYS*/ wire _URYS_FF01_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.TOLA_A01n(),   cpu_bus.BUS_CPU_A[ 0].qp());
  /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[0].qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[1].qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[2].qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[3].qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[4].qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[5].qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[6].qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D[7].qp_new(), rst.ALUR_SYS_RSTn());

  /* p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(cpu_bus.BUS_CPU_D[0].qp_new(), _DAKU_FF01_WRp);
  /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(cpu_bus.BUS_CPU_D[1].qp_new(), _DAKU_FF01_WRp);
  /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(cpu_bus.BUS_CPU_D[2].qp_new(), _DAKU_FF01_WRp);
  /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(cpu_bus.BUS_CPU_D[3].qp_new(), _DAKU_FF01_WRp);
  /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(cpu_bus.BUS_CPU_D[4].qp_new(), _DAKU_FF01_WRp);
  /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(cpu_bus.BUS_CPU_D[5].qp_new(), _DAKU_FF01_WRp);
  /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(cpu_bus.BUS_CPU_D[6].qp_new(), _DAKU_FF01_WRp);
  /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn = nand2(cpu_bus.BUS_CPU_D[7].qp_new(), _DAKU_FF01_WRp);

  /* p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(CULY_SER_DIR.qp_new(), COTY_SER_CLK.qp_new(), PIN_SCK.qn_new());
  /* p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ETAF_SER_RUNNING.qn_new());
  /* p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  /* p06.CAGE*/ wire _CAGE_SER_IN_new  = not1(PIN_SIN.qp_new());
  /* p06.EDER*/ EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEL_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, EROD_SER_DATA6.qp_old());
  /* p06.EROD*/ EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, EJAB_SER_DATA5.qp_old());
  /* p06.EJAB*/ EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, DOVU_SER_DATA4.qp_old());
  /* p06.DOVU*/ DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, DOJO_SER_DATA3.qp_old());
  /* p06.DOJO*/ DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, DYRA_SER_DATA2.qp_old());
  /* p06.DYRA*/ DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, DEGU_SER_DATA1.qp_old());
  /* p06.DEGU*/ DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, CUBA_SER_DATA0.qp_old());
  /* p06.CUBA*/ CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SER_IN_new);
}

//------------------------------------------------------------------------------------------------------------------------
