#include "GateBoyLib/GateBoySerial.h"

#include "GateBoyLib/GateBoy.h"

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

void GateBoy::reg_sb_read()
{
  /* p06.UFEG*/ wire _UFEG_FF01_RDp =  and4(cpu_bus.TEDO_CPU_RDp, new_bus.SANO_FF00_FF03p(), new_bus.TOLA_A01n(), new_bus.BUS_CPU_A00p.qp_new());
  /*#p06.CUGY_SD0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_UFEG_FF01_RDp, serial.CUBA_SER_DATA0.qn_new());
  /* p06.DUDE_SD1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_pn(_UFEG_FF01_RDp, serial.DEGU_SER_DATA1.qn_new());
  /* p06.DETU_SD2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_pn(_UFEG_FF01_RDp, serial.DYRA_SER_DATA2.qn_new());
  /* p06.DASO_SD3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_pn(_UFEG_FF01_RDp, serial.DOJO_SER_DATA3.qn_new());
  /* p06.DAME_SD4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_pn(_UFEG_FF01_RDp, serial.DOVU_SER_DATA4.qn_new());
  /* p06.EVOK_SD5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_pn(_UFEG_FF01_RDp, serial.EJAB_SER_DATA5.qn_new());
  /* p06.EFAB_SD6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_pn(_UFEG_FF01_RDp, serial.EROD_SER_DATA6.qn_new());
  /*#p06.ETAK_SD7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_pn(_UFEG_FF01_RDp, serial.EDER_SER_DATA7.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::reg_sc_read()
{
  /* p06.UCOM*/ wire _UCOM_FF02_RDp =  and4(cpu_bus.TEDO_CPU_RDp, new_bus.SANO_FF00_FF03p(), new_bus.BUS_CPU_A01p.qp_new(), new_bus.TOVY_A00n());
  /* p06.CORE_SER0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_UCOM_FF02_RDp, serial.CULY_SER_DIR.qn_new());
  /* p06.ELUV_SER1_TO_CD1*/ new_bus.BUS_CPU_D07p.tri6_pn(_UCOM_FF02_RDp, serial.ETAF_SER_RUNNING.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::serial_tock1()
{
  serial.PIN_68_SCK.reset_for_pass();
  serial.PIN_70_SOUT.reset_for_pass();

  /*#p06.UWAM*/ wire _UWAM_FF02_WRn = nand4(new_bus.TOVY_A00n(), new_bus.BUS_CPU_A01p.qp_new(), cpu_bus.TAPU_CPU_WRp, new_bus.SANO_FF00_FF03p());
  /*#p06.CULY*/ serial.CULY_SER_DIR.dff17(_UWAM_FF02_WRn, rst.ALUR_SYS_RSTn(), new_bus.BUS_CPU_D00p.qp_old());

  /*#p01.UVYN*/ wire _UVYN_DIV05n = not1(div.TAMA_DIV05p.qp_new());
  /*#p06.COTY*/ serial.COTY_SER_CLK.dff17(_UVYN_DIV05n, _UWAM_FF02_WRn, serial.COTY_SER_CLK.qn_old()); // schematic wrong, clock is UVYN

  //----------------------------------------
  // Feedback loop

  for (int rep = 0; rep < 2; rep++) {
    // FIXME check PIN_68 polarity
    /*#p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(serial.CULY_SER_DIR.qp_new(), serial.COTY_SER_CLK.qp_new(), serial.PIN_68_SCK.qp_any());
    /*#p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, serial.ETAF_SER_RUNNING.qn_any());

    {
      /* p06.CARO*/ wire _CARO_SER_RSTn = and2(_UWAM_FF02_WRn, rst.ALUR_SYS_RSTn());
      /*#p06.CAFA*/ serial.CAFA_SER_CNT0.dff17_any(_DAWA_SER_CLK,                 _CARO_SER_RSTn, serial.CAFA_SER_CNT0.qn_any());
      /*#p06.CYLO*/ serial.CYLO_SER_CNT1.dff17_any(serial.CAFA_SER_CNT0.qn_any(), _CARO_SER_RSTn, serial.CYLO_SER_CNT1.qn_any());
      /*#p06.CYDE*/ serial.CYDE_SER_CNT2.dff17_any(serial.CYLO_SER_CNT1.qn_any(), _CARO_SER_RSTn, serial.CYDE_SER_CNT2.qn_any());
      /*#p06.CALY*/ serial.CALY_SER_CNT3.dff17_any(serial.CYDE_SER_CNT2.qn_any(), _CARO_SER_RSTn, serial.CALY_SER_CNT3.qn_any());
    }

    {
      /*#p06.COBA*/ wire _COBA_SER_CNT3n = not1(serial.CALY_SER_CNT3.qp_any());
      /*#p06.CABY*/ wire _CABY_XFER_RESET = and2(_COBA_SER_CNT3n, rst.ALUR_SYS_RSTn());
      /*#p06.ETAF*/ serial.ETAF_SER_RUNNING.dff17_any(_UWAM_FF02_WRn, _CABY_XFER_RESET, new_bus.BUS_CPU_D07p.qp_old());
    }

    {
      /*#p06.KEXU*/ wire _KEXU = nand2(_DAWA_SER_CLK, serial.CULY_SER_DIR.qp_new()); // schematic wrong, first input is DAWA
      /*#p06.JAGO*/ wire _JAGO =  not1(serial.CULY_SER_DIR.qp_new());
      /*#p06.KUJO*/ wire _KUJO =  nor2(_JAGO, _DAWA_SER_CLK); // schematic wrong

      /*PIN_68*/ serial.PIN_68_SCK.pin_io_out_pull_hilo_any(serial.CULY_SER_DIR.qp_new(), _KEXU, _KUJO);
      serial.PIN_68_SCK.pin_io_in_oedp_any(0, 1);
    }
  }

  //----------------------------------------

  {
    /*#p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(serial.CULY_SER_DIR.qp_new(), serial.COTY_SER_CLK.qp_new(), serial.PIN_68_SCK.qp_new());
    /*#p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, serial.ETAF_SER_RUNNING.qn_new());
    /*#p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
    /*#p06.ELYS*/ serial.ELYS_SER_OUT  .dff17(_EDYL_SER_CLK, rst.ALUR_SYS_RSTn(), serial.EDER_SER_DATA7.qp_old());
    ///* p05.KENA*/ wire _KENA = mux2n(KUKO_DBG_FF00_D6, ELYS_SER_OUT.qp_new(), FF60_0); // FIXME hacking out debug stuff
    /*PIN_70*/ serial.PIN_70_SOUT.pin_out_dp(serial.ELYS_SER_OUT.qp_new());
  }

}



void GateBoy::serial_tock2() {
  serial.PIN_69_SIN.reset_for_pass();

  /*#p06.CAVE*/ wire _CAVE_SER_CLK = mux2n(serial.CULY_SER_DIR.qp_new(), serial.COTY_SER_CLK.qp_new(), serial.PIN_68_SCK.qp_new());
  /*#p06.DAWA*/ wire _DAWA_SER_CLK = or2(_CAVE_SER_CLK, serial.ETAF_SER_RUNNING.qn_new());
  /*#p06.EDYL*/ wire _EDYL_SER_CLK = not1(_DAWA_SER_CLK);
  /* p06.EPYT*/ wire _EPYT_SER_CLK = not1(_EDYL_SER_CLK);
  /* p06.DEHO*/ wire _DEHO_SER_CLK = not1(_EPYT_SER_CLK);
  /* p06.DAWE*/ wire _DAWE_SER_CLK = not1(_DEHO_SER_CLK);

  // this pin has 4 wire2s attached, but they're not traced
  serial.PIN_69_SIN.pin_in_dp(1);

  /* p06.URYS*/ wire _URYS_FF01_WRn = nand4(cpu_bus.TAPU_CPU_WRp, new_bus.SANO_FF00_FF03p(), new_bus.TOLA_A01n(),   new_bus.BUS_CPU_A00p.qp_new());
  /* p06.COHY*/ wire _COHY_SER_DATA0_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D00p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DUMO*/ wire _DUMO_SER_DATA1_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D01p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DYBO*/ wire _DYBO_SER_DATA2_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D02p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DAJU*/ wire _DAJU_SER_DATA3_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D03p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.DYLY*/ wire _DYLY_SER_DATA4_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D04p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EHUJ*/ wire _EHUJ_SER_DATA5_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D05p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EFAK*/ wire _EFAK_SER_DATA6_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D06p.qp_new(), rst.ALUR_SYS_RSTn());
  /* p06.EGUV*/ wire _EGUV_SER_DATA7_RSTn = or_and3(_URYS_FF01_WRn, new_bus.BUS_CPU_D07p.qp_new(), rst.ALUR_SYS_RSTn());

  /* p06.DAKU*/ wire _DAKU_FF01_WRp = not1(_URYS_FF01_WRn);
  /* p06.CUFU*/ wire _CUFU_SER_DATA0_SETn = nand2(new_bus.BUS_CPU_D00p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DOCU*/ wire _DOCU_SER_DATA1_SETn = nand2(new_bus.BUS_CPU_D01p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DELA*/ wire _DELA_SER_DATA2_SETn = nand2(new_bus.BUS_CPU_D02p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DYGE*/ wire _DYGE_SER_DATA3_SETn = nand2(new_bus.BUS_CPU_D03p.qp_new(), _DAKU_FF01_WRp);
  /* p06.DOLA*/ wire _DOLA_SER_DATA4_SETn = nand2(new_bus.BUS_CPU_D04p.qp_new(), _DAKU_FF01_WRp);
  /* p06.ELOK*/ wire _ELOK_SER_DATA5_SETn = nand2(new_bus.BUS_CPU_D05p.qp_new(), _DAKU_FF01_WRp);
  /* p06.EDEL*/ wire _EDEL_SER_DATA6_SETn = nand2(new_bus.BUS_CPU_D06p.qp_new(), _DAKU_FF01_WRp);
  /* p06.EFEF*/ wire _EFEF_SER_DATA7_SETn = nand2(new_bus.BUS_CPU_D07p.qp_new(), _DAKU_FF01_WRp);

  /* p06.CAGE*/ wire _CAGE_SER_IN_new  = not1(serial.PIN_69_SIN.qp_new());
  /* p06.EDER*/ serial.EDER_SER_DATA7.dff22(_EPYT_SER_CLK, _EFEF_SER_DATA7_SETn, _EGUV_SER_DATA7_RSTn, serial.EROD_SER_DATA6.qp_old());
  /* p06.EROD*/ serial.EROD_SER_DATA6.dff22(_EPYT_SER_CLK, _EDEL_SER_DATA6_SETn, _EFAK_SER_DATA6_RSTn, serial.EJAB_SER_DATA5.qp_old());
  /* p06.EJAB*/ serial.EJAB_SER_DATA5.dff22(_EPYT_SER_CLK, _ELOK_SER_DATA5_SETn, _EHUJ_SER_DATA5_RSTn, serial.DOVU_SER_DATA4.qp_old());
  /* p06.DOVU*/ serial.DOVU_SER_DATA4.dff22(_EPYT_SER_CLK, _DOLA_SER_DATA4_SETn, _DYLY_SER_DATA4_RSTn, serial.DOJO_SER_DATA3.qp_old());
  /* p06.DOJO*/ serial.DOJO_SER_DATA3.dff22(_DAWE_SER_CLK, _DYGE_SER_DATA3_SETn, _DAJU_SER_DATA3_RSTn, serial.DYRA_SER_DATA2.qp_old());
  /* p06.DYRA*/ serial.DYRA_SER_DATA2.dff22(_DAWE_SER_CLK, _DELA_SER_DATA2_SETn, _DYBO_SER_DATA2_RSTn, serial.DEGU_SER_DATA1.qp_old());
  /* p06.DEGU*/ serial.DEGU_SER_DATA1.dff22(_DAWE_SER_CLK, _DOCU_SER_DATA1_SETn, _DUMO_SER_DATA1_RSTn, serial.CUBA_SER_DATA0.qp_old());
  /* p06.CUBA*/ serial.CUBA_SER_DATA0.dff22(_DAWE_SER_CLK, _CUFU_SER_DATA0_SETn, _COHY_SER_DATA0_RSTn, _CAGE_SER_IN_new);
}

//------------------------------------------------------------------------------------------------------------------------
