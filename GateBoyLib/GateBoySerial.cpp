#include "GateBoyLib/GateBoySerial.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Serial

void GateBoySerial::tock(
  wire BUS_CPU_A[16],
  wire BUS_CPU_D[8],
  wire ALUR_SYS_RSTn,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,
  wire TAMA_DIV05p,
  BusOut BUS_CPU_D_out[8])
{

  bool DAWA_SER_clknew;
  {
    /* p01.UVYN*/ wire _UVYN_DIV_05n_next = not1(TAMA_DIV05p);
    /* p06.UWAM*/ wire _UWAM_FF02_WRn_clk = nand4(TAPU_CPU_WRp, SANO_FF00_FF03p_ext(BUS_CPU_A),  BUS_CPU_A[ 1], TOVY_A00n_ext(BUS_CPU_A));
    /* p06.COTY*/ COTY_SER_CLK .dff17(_UVYN_DIV_05n_next, _UWAM_FF02_WRn_clk, COTY_SER_CLK.qn_old());
    /* p06.CULY*/ CULY_XFER_DIR.dff17(_UWAM_FF02_WRn_clk, ALUR_SYS_RSTn, BUS_CPU_D[0]);

    /* p06.CARO*/ wire _CARO_SER_RST_new = and2(_UWAM_FF02_WRn_clk, ALUR_SYS_RSTn);
    /* p06.CAFA*/ CAFA_SER_CNT0.RSTn(_CARO_SER_RST_new);
    /* p06.CYLO*/ CYLO_SER_CNT1.RSTn(_CARO_SER_RST_new);
    /* p06.CYDE*/ CYDE_SER_CNT2.RSTn(_CARO_SER_RST_new);
    /* p06.CALY*/ CALY_SER_CNT3.RSTn(_CARO_SER_RST_new);

    /* p06.COBA*/ wire _COBA_SER_CNT3n_new  = not1(CALY_SER_CNT3.qp_new());
    /* p06.CABY*/ wire _CABY_XFER_RESET_new = and2(_COBA_SER_CNT3n_new, ALUR_SYS_RSTn);
    /* p06.ETAF*/ ETAF_SER_RUNNING.dff17(_UWAM_FF02_WRn_clk, _CABY_XFER_RESET_new, BUS_CPU_D[7]);

    /* p06.CAVE*/ wire _CAVE_SER_clknew = mux2n(CULY_XFER_DIR.qp_new(), COTY_SER_CLK.qp_new(), PIN_SCK.qn_new());
    /* p06.DAWA*/ wire _DAWA_SER_clknew = or2(_CAVE_SER_clknew, ETAF_SER_RUNNING.qn_new());

    /* p06.CAFA*/ CAFA_SER_CNT0.dff17(_DAWA_SER_clknew,               _CARO_SER_RST_new, CAFA_SER_CNT0.qn_new());
    /* p06.CYLO*/ CYLO_SER_CNT1.dff17(CAFA_SER_CNT0.qn_new(), _CARO_SER_RST_new, CYLO_SER_CNT1.qn_new());
    /* p06.CYDE*/ CYDE_SER_CNT2.dff17(CYLO_SER_CNT1.qn_new(), _CARO_SER_RST_new, CYDE_SER_CNT2.qn_new());
    /* p06.CALY*/ CALY_SER_CNT3.dff17(CYDE_SER_CNT2.qn_new(), _CARO_SER_RST_new, CALY_SER_CNT3.qn_new());

    DAWA_SER_clknew =_DAWA_SER_clknew;
  }

  {
    /* p06.EDYL*/ wire _EDYL_SER_CLK_new = not1(DAWA_SER_clknew);
    /* p06.ELYS*/ ELYS_SER_OUT  .dff17(_EDYL_SER_CLK_new, ALUR_SYS_RSTn, EDER_SER_DATA7.qp_old());
    /* p05.KENA*/ wire _KENA = /*mux2n(KUKO_DBG_FF00_D6,*/ ELYS_SER_OUT.qp_new() /*, FF60_0)*/; // FIXME hacking out debug stuff

    PIN_SOUT.pin_out(_KENA, _KENA);
  }

  {
    /* p06.KEXU*/ wire _KEXU_new = nand2(COTY_SER_CLK.qp_new(), CULY_XFER_DIR.qp_new());
    /* p06.JAGO*/ wire _JAGO_new = not1(CULY_XFER_DIR.qp_new());
    /* p06.KUJO*/ wire _KUJO_new = nor2(COTY_SER_CLK.qp_new(), _JAGO_new);

    PIN_SCK.pin_out(CULY_XFER_DIR.qp_new(), _KEXU_new, _KUJO_new);
  }

  {
    /* p06.URYS*/ wire _URYS_FF01_WRn_clk   = nand4(TAPU_CPU_WRp, SANO_FF00_FF03p_ext(BUS_CPU_A), TOLA_A01n_ext(BUS_CPU_A), BUS_CPU_A[ 0]);

    /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[0], ALUR_SYS_RSTn);
    /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[1], ALUR_SYS_RSTn);
    /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[2], ALUR_SYS_RSTn);
    /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[3], ALUR_SYS_RSTn);
    /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[4], ALUR_SYS_RSTn);
    /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[5], ALUR_SYS_RSTn);
    /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[6], ALUR_SYS_RSTn);
    /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn_new = or_and3(_URYS_FF01_WRn_clk, BUS_CPU_D[7], ALUR_SYS_RSTn);

    /* p06.DAKU*/ wire _DAKU_FF01_WRp_clk = not1(_URYS_FF01_WRn_clk);
    /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn_new = nand2(BUS_CPU_D[0], _DAKU_FF01_WRp_clk);
    /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn_new = nand2(BUS_CPU_D[1], _DAKU_FF01_WRp_clk);
    /* p06.DELA*/ wire _DELA_SER_DATA2_SETn_new = nand2(BUS_CPU_D[2], _DAKU_FF01_WRp_clk);
    /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn_new = nand2(BUS_CPU_D[3], _DAKU_FF01_WRp_clk);
    /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn_new = nand2(BUS_CPU_D[4], _DAKU_FF01_WRp_clk);
    /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn_new = nand2(BUS_CPU_D[5], _DAKU_FF01_WRp_clk);
    /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn_new = nand2(BUS_CPU_D[6], _DAKU_FF01_WRp_clk);
    /* p06.EFEF*/ wire _EFEL_SER_DATA7_SETn_new = nand2(BUS_CPU_D[7], _DAKU_FF01_WRp_clk);

    /* p06.EDYL*/ wire _EDYL_SER_CLK_new = not1(DAWA_SER_clknew);
    /* p06.EPYT*/ wire _EPYT_SER_CLK_new = not1(_EDYL_SER_CLK_new);
    /* p06.DEHO*/ wire _DEHO_SER_CLK_new = not1(_EPYT_SER_CLK_new);
    /* p06.DAWE*/ wire _DAWE_SER_CLK_new = not1(_DEHO_SER_CLK_new);

    /* p06.CAGE*/ wire _CAGE_SER_IN_new  = not1(PIN_SIN.qp_new());

    /* p06.EDER*/ EDER_SER_DATA7.dff22(_EPYT_SER_CLK_new, _EFEL_SER_DATA7_SETn_new, _EGUV_SER_DATA7_RSTn_new, EROD_SER_DATA6.qp_old());
    /* p06.EROD*/ EROD_SER_DATA6.dff22(_EPYT_SER_CLK_new, _EDEL_SER_DATA6_SETn_new, _EFAK_SER_DATA6_RSTn_new, EJAB_SER_DATA5.qp_old());
    /* p06.EJAB*/ EJAB_SER_DATA5.dff22(_EPYT_SER_CLK_new, _ELOK_SER_DATA5_SETn_new, _EHUJ_SER_DATA5_RSTn_new, DOVU_SER_DATA4.qp_old());
    /* p06.DOVU*/ DOVU_SER_DATA4.dff22(_EPYT_SER_CLK_new, _DOLA_SER_DATA4_SETn_new, _DYLY_SER_DATA4_RSTn_new, DOJO_SER_DATA3.qp_old());
    /* p06.DOJO*/ DOJO_SER_DATA3.dff22(_DAWE_SER_CLK_new, _DYGE_SER_DATA3_SETn_new, _DAJU_SER_DATA3_RSTn_new, DYRA_SER_DATA2.qp_old());
    /* p06.DYRA*/ DYRA_SER_DATA2.dff22(_DAWE_SER_CLK_new, _DELA_SER_DATA2_SETn_new, _DYBO_SER_DATA2_RSTn_new, DEGU_SER_DATA1.qp_old());
    /* p06.DEGU*/ DEGU_SER_DATA1.dff22(_DAWE_SER_CLK_new, _DOCU_SER_DATA1_SETn_new, _DUMO_SER_DATA1_RSTn_new, CUBA_SER_DATA0.qp_old());
    /* p06.CUBA*/ CUBA_SER_DATA0.dff22(_DAWE_SER_CLK_new, _CUFU_SER_DATA0_SETn_new, _COHY_SER_DATA0_RSTn_new, _CAGE_SER_IN_new);
  }

  {
    /* p06.UFEG*/ wire _UFEG_FF01_RDp_ext   =  and4(TEDO_CPU_RDp, SANO_FF00_FF03p_ext(BUS_CPU_A),  TOLA_A01n_ext(BUS_CPU_A), BUS_CPU_A[ 0]);
    /* p06.UCOM*/ wire _UCOM_FF02_RDp_ext   =  and4(TEDO_CPU_RDp, SANO_FF00_FF03p_ext(BUS_CPU_A),  BUS_CPU_A[ 1],            TOVY_A00n_ext(BUS_CPU_A));

    /* FF01 SER */
    /*#p06.CUGY*/ BUS_CPU_D_out[0].tri6_pn(_UFEG_FF01_RDp_ext, CUBA_SER_DATA0.qn_new());
    /* p06.DUDE*/ BUS_CPU_D_out[1].tri6_pn(_UFEG_FF01_RDp_ext, DEGU_SER_DATA1.qn_new());
    /* p06.DETU*/ BUS_CPU_D_out[2].tri6_pn(_UFEG_FF01_RDp_ext, DYRA_SER_DATA2.qn_new());
    /* p06.DASO*/ BUS_CPU_D_out[3].tri6_pn(_UFEG_FF01_RDp_ext, DOJO_SER_DATA3.qn_new());
    /* p06.DAME*/ BUS_CPU_D_out[4].tri6_pn(_UFEG_FF01_RDp_ext, DOVU_SER_DATA4.qn_new());
    /* p06.EVOK*/ BUS_CPU_D_out[5].tri6_pn(_UFEG_FF01_RDp_ext, EJAB_SER_DATA5.qn_new());
    /* p06.EFAB*/ BUS_CPU_D_out[6].tri6_pn(_UFEG_FF01_RDp_ext, EROD_SER_DATA6.qn_new());
    /*#p06.ETAK*/ BUS_CPU_D_out[7].tri6_pn(_UFEG_FF01_RDp_ext, EDER_SER_DATA7.qn_new());

    /* FF02 SER */
    /* p06.CORE*/ BUS_CPU_D_out[0].tri6_pn(_UCOM_FF02_RDp_ext, CULY_XFER_DIR.qn_new());
    /* p06.ELUV*/ BUS_CPU_D_out[7].tri6_pn(_UCOM_FF02_RDp_ext, ETAF_SER_RUNNING.qn_new());
  }
}

