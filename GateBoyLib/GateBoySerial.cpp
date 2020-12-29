#include "GateBoyLib/GateBoySerial.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyCpuBus.h"
#include "GateBoyLib/GateBoyTimer.h"

// DFF17_01 SC
// DFF17_02 << CLKp
// DFF17_03 SC
// DFF17_04 --
// DFF17_05 --
// DFF17_06 << RSTn  // must be RSTn, see WUVU/VENA/WOSU
// DFF17_07 << D
// DFF17_08 --
// DFF17_09 SC
// DFF17_10 --
// DFF17_11 --
// DFF17_12 SC
// DFF17_13 << RSTn
// DFF17_14 --
// DFF17_15 --
// DFF17_16 >> QN   _MUST_ be QN - see TERO
// DFF17_17 >> Q    _MUST_ be Q  - see TERO

// sck_a <= KEXU
// sck_b <= CULY17
// sck_c => CAVE
// sck_d <= KUJO

//------------------------------------------------------------------------------------------------------------------------

void GateBoySerial::read_sb(GateBoyCpuBus& cpu_bus)
{
  /* p06.UFEG*/ wire _UFEG_FF01_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.TOLA_A01n(), cpu_bus.BUS_CPU_A00p.qp_new());
  /*#p06.CUGY*/ cpu_bus.BUS_CPU_D00p.tri6_pn(_UFEG_FF01_RDp, CUBA_SER_DATA0.qn_new());
  /* p06.DUDE*/ cpu_bus.BUS_CPU_D01p.tri6_pn(_UFEG_FF01_RDp, DEGU_SER_DATA1.qn_new());
  /* p06.DETU*/ cpu_bus.BUS_CPU_D02p.tri6_pn(_UFEG_FF01_RDp, DYRA_SER_DATA2.qn_new());
  /* p06.DASO*/ cpu_bus.BUS_CPU_D03p.tri6_pn(_UFEG_FF01_RDp, DOJO_SER_DATA3.qn_new());
  /* p06.DAME*/ cpu_bus.BUS_CPU_D04p.tri6_pn(_UFEG_FF01_RDp, DOVU_SER_DATA4.qn_new());
  /* p06.EVOK*/ cpu_bus.BUS_CPU_D05p.tri6_pn(_UFEG_FF01_RDp, EJAB_SER_DATA5.qn_new());
  /* p06.EFAB*/ cpu_bus.BUS_CPU_D06p.tri6_pn(_UFEG_FF01_RDp, EROD_SER_DATA6.qn_new());
  /*#p06.ETAK*/ cpu_bus.BUS_CPU_D07p.tri6_pn(_UFEG_FF01_RDp, EDER_SER_DATA7.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySerial::read_sc(GateBoyCpuBus& cpu_bus)
{
  /* p06.UCOM*/ wire _UCOM_FF02_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.BUS_CPU_A01p.qp_new(), cpu_bus.TOVY_A00n());
  /* p06.CORE*/ cpu_bus.BUS_CPU_D00p.tri6_pn(_UCOM_FF02_RDp, CULY_SER_DIR.qn_new());
  /* p06.ELUV*/ cpu_bus.BUS_CPU_D07p.tri6_pn(_UCOM_FF02_RDp, ETAF_SER_RUNNING.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoySerial::tock1(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, GateBoyDiv& div)
{
  PIN68_SCK.reset_for_pass();
  PIN70_SOUT.reset_for_pass();

  /*#p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(cpu_bus.TOVY_A00n(), cpu_bus.BUS_CPU_A01p.qp_new(), cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.SANO_FF00_FF03p());
  /*#p06.CULY*/ CULY_SER_DIR.dff17(_UWAM_FF02_WRn, rst.ALUR_SYS_RSTn(), cpu_bus.BUS_CPU_D00p.qp_old());

  /*#p01.UVYN*/ wire _UVYN_DIV05n = not1(div.TAMA_DIV05p.qp_new());
  /*#p06.COTY*/ COTY_SER_CLK.dff17(_UVYN_DIV05n, _UWAM_FF02_WRn, COTY_SER_CLK.qn_old()); // schematic wrong, clock is UVYN

  //----------------------------------------
  // Feedback loop

  for (int rep = 0; rep < 2; rep++) {
    /*#p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(CULY_SER_DIR.qp_new(), COTY_SER_CLK.qp_new(), PIN68_SCK.qn_any());
    /*#p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ETAF_SER_RUNNING.qn_any());

    {
      /* p06.CARO*/ wire _CARO_SER_RSTn = and2(_UWAM_FF02_WRn, rst.ALUR_SYS_RSTn());
      /*#p06.CAFA*/ CAFA_SER_CNT0.dff17_any(_DAWA_SER_CLK,           _CARO_SER_RSTn, CAFA_SER_CNT0.qn_any());
      /*#p06.CYLO*/ CYLO_SER_CNT1.dff17_any(CAFA_SER_CNT0.qn_any(), _CARO_SER_RSTn, CYLO_SER_CNT1.qn_any());
      /*#p06.CYDE*/ CYDE_SER_CNT2.dff17_any(CYLO_SER_CNT1.qn_any(), _CARO_SER_RSTn, CYDE_SER_CNT2.qn_any());
      /*#p06.CALY*/ CALY_SER_CNT3.dff17_any(CYDE_SER_CNT2.qn_any(), _CARO_SER_RSTn, CALY_SER_CNT3.qn_any());
    }

    {
      /*#p06.COBA*/ wire _COBA_SER_CNT3n = not1(CALY_SER_CNT3.qp_any());
      /*#p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, rst.ALUR_SYS_RSTn());
      /*#p06.ETAF*/ ETAF_SER_RUNNING.dff17_any(_UWAM_FF02_WRn, _CABY_XFER_RESET, cpu_bus.BUS_CPU_D07p.qp_old());
    }

    {
      /*#p06.KEXU*/ wire _KEXU = nand2(_DAWA_SER_CLK, CULY_SER_DIR.qp_new()); // schematic wrong, first input is DAWA
      /*#p06.JAGO*/ wire _JAGO =  not1(CULY_SER_DIR.qp_new());
      /*#p06.KUJO*/ wire _KUJO =  nor2(_JAGO, _DAWA_SER_CLK); // schematic wrong

      /*PIN68*/ PIN68_SCK.pin_out_pull_hilo_any(CULY_SER_DIR.qp_new(), _KEXU, _KUJO);
      PIN68_SCK.pin_in_oedp_any(0, 1);
    }
  }

  //----------------------------------------

  {
    /*#p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(CULY_SER_DIR.qp_new(), COTY_SER_CLK.qp_new(), PIN68_SCK.int_qp_new());
    /*#p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ETAF_SER_RUNNING.qn_new());
    /*#p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
    /*#p06.ELYS*/ ELYS_SER_OUT  .dff17(_EDYL_SER_CLK, rst.ALUR_SYS_RSTn(), EDER_SER_DATA7.qp_old());
    ///* p05.KENA*/ wire _KENA = mux2n(KUKO_DBG_FF00_D6, ELYS_SER_OUT.qp_new(), FF60_0); // FIXME hacking out debug stuff
    /*PIN70*/ PIN70_SOUT.pin_out_dp(ELYS_SER_OUT.qp_new());
  }

}



void GateBoySerial::tock2(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus) {
  PIN69_SIN.reset_for_pass();

  /*#p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(CULY_SER_DIR.qp_new(), COTY_SER_CLK.qp_new(), PIN68_SCK.int_qp_new());
  /*#p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, ETAF_SER_RUNNING.qn_new());
  /*#p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  // this pin has 4 wire2s attached, but they're not traced
  PIN69_SIN.pin_in_dp(1);

  /* p06.URYS*/ wire _URYS_FF01_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.SANO_FF00_FF03p(), cpu_bus.TOLA_A01n(),   cpu_bus.BUS_CPU_A00p.qp_new());
  /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D00p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D01p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D02p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D03p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D04p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D05p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D06p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, cpu_bus.BUS_CPU_D07p.qp_new(), rst.ALUR_SYS_RSTn());

  /* p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(cpu_bus.BUS_CPU_D00p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(cpu_bus.BUS_CPU_D01p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(cpu_bus.BUS_CPU_D02p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(cpu_bus.BUS_CPU_D03p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(cpu_bus.BUS_CPU_D04p.qp_new(), _DAKU_FF01_WRp);
  /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(cpu_bus.BUS_CPU_D05p.qp_new(), _DAKU_FF01_WRp);
  /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(cpu_bus.BUS_CPU_D06p.qp_new(), _DAKU_FF01_WRp);
  /* p06.EFEF*/ wire _EFEF_SER_DATA7_SETn = nand2(cpu_bus.BUS_CPU_D07p.qp_new(), _DAKU_FF01_WRp);

  /* p06.CAGE*/ wire _CAGE_SER_IN_new  = not1(PIN69_SIN.int_qp_new());
  /* p06.EDER*/ EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEF_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, EROD_SER_DATA6.qp_old());
  /* p06.EROD*/ EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, EJAB_SER_DATA5.qp_old());
  /* p06.EJAB*/ EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, DOVU_SER_DATA4.qp_old());
  /* p06.DOVU*/ DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, DOJO_SER_DATA3.qp_old());
  /* p06.DOJO*/ DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, DYRA_SER_DATA2.qp_old());
  /* p06.DYRA*/ DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, DEGU_SER_DATA1.qp_old());
  /* p06.DEGU*/ DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, CUBA_SER_DATA0.qp_old());
  /* p06.CUBA*/ CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SER_IN_new);
}

//------------------------------------------------------------------------------------------------------------------------
