#include "GateBoyLib/GateBoySerial.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_serial()
{
  /*#p06.UWAM*/ wire UWAM_FF02_WRn = nand4(new_bus.TOVY_A00n(), new_bus.BUS_CPU_A01p.out_new(), cpu_signals.TAPU_CPU_WRp.out_new(), new_bus.SANO_FF00_FF03p());
  /*#p06.CULY*/ serial.CULY_SER_DIR.dff17(UWAM_FF02_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D00p.out_old());

  /*#p01.UVYN*/ wire UVYN_DIV05n = not1(div.TAMA_DIV05p.qp_new());
  /*#p06.COTY*/ serial.COTY_SER_CLK.dff17(UVYN_DIV05n, UWAM_FF02_WRn, serial.COTY_SER_CLK.qn_old());

  //----------------------------------------
  // Feedback loop

  for (int rep = 0; rep < 2; rep++) {
    // FIXME check PIN_68 polarity
    /*#p06.CAVE*/ wire CAVE_SER_CLK = mux2n(serial.CULY_SER_DIR.qp_new(), serial.COTY_SER_CLK.qp_new(), serial.PIN_68_SCK.qp_int_any());
    /*#p06.DAWA*/ wire DAWA_SER_CLK = or2(CAVE_SER_CLK, serial.ETAF_SER_RUN.qn_any());

    {
      /*_p06.CARO*/ wire CARO_SER_RSTn = and2(UWAM_FF02_WRn, ALUR_SYS_RSTn());
      /*#p06.CAFA*/ serial.CAFA_SER_CNT0.dff17_any(DAWA_SER_CLK,                 CARO_SER_RSTn, serial.CAFA_SER_CNT0.qn_any());
      /*#p06.CYLO*/ serial.CYLO_SER_CNT1.dff17_any(serial.CAFA_SER_CNT0.qn_any(), CARO_SER_RSTn, serial.CYLO_SER_CNT1.qn_any());
      /*#p06.CYDE*/ serial.CYDE_SER_CNT2.dff17_any(serial.CYLO_SER_CNT1.qn_any(), CARO_SER_RSTn, serial.CYDE_SER_CNT2.qn_any());
      /*#p06.CALY*/ serial.CALY_SER_CNT3.dff17_any(serial.CYDE_SER_CNT2.qn_any(), CARO_SER_RSTn, serial.CALY_SER_CNT3.qn_any());
    }

    {
      /*#p06.COBA*/ wire COBA_SER_CNT3n = not1(serial.CALY_SER_CNT3.qp_any());
      /*#p06.CABY*/ wire CABY_XFER_RESET = and2(COBA_SER_CNT3n, ALUR_SYS_RSTn());
      /*#p06.ETAF*/ serial.ETAF_SER_RUN.dff17_any(UWAM_FF02_WRn, CABY_XFER_RESET, old_bus.BUS_CPU_D07p.out_old());
    }

    {
      /*#p06.KEXU*/ wire KEXU = nand2(DAWA_SER_CLK, serial.CULY_SER_DIR.qp_new()); // schematic wrong, first input is DAWA
      /*#p06.JAGO*/ wire JAGO =  not1(serial.CULY_SER_DIR.qp_new());
      /*#p06.KUJO*/ wire KUJO =  nor2(JAGO, DAWA_SER_CLK); // schematic wrong

      wire EXT_ser_oe = 0;
      wire EXT_ser_dp = 1;

      /*_PIN_68*/ serial.PIN_68_SCK.pin_io_any(serial.CULY_SER_DIR.qp_new(), KEXU, KUJO, EXT_ser_oe, EXT_ser_dp);
    }
  }

  //----------------------------------------

  {
    /*#p06.CAVE*/ wire CAVE_SER_CLK = mux2n(serial.CULY_SER_DIR.qp_new(), serial.COTY_SER_CLK.qp_new(), serial.PIN_68_SCK.qp_int_new());
    /*#p06.DAWA*/ wire DAWA_SER_CLK = or2(CAVE_SER_CLK, serial.ETAF_SER_RUN.qn_new());
    /*#p06.EDYL*/ wire EDYL_SER_CLK = not1(DAWA_SER_CLK);
    /*#p06.ELYS*/ serial.ELYS_SER_OUT  .dff17(EDYL_SER_CLK, ALUR_SYS_RSTn(), serial.EDER_SER_DATA7.qp_old());
    ///*_p05.KENA*/ wire KENA = mux2n(KUKO_DBG_FF00_D6, ELYS_SER_OUT.qp_new(), FF60_0); // FIXME hacking out debug stuff
    /*_PIN_70*/ serial.PIN_70_SOUT.pin_out(serial.ELYS_SER_OUT.qp_new(), serial.ELYS_SER_OUT.qp_new());
  }

  /*#p06.CAVE*/ wire CAVE_SER_CLK = mux2n(serial.CULY_SER_DIR.qp_new(), serial.COTY_SER_CLK.qp_new(), serial.PIN_68_SCK.qp_int_new());
  /*#p06.DAWA*/ wire DAWA_SER_CLK = or2(CAVE_SER_CLK, serial.ETAF_SER_RUN.qn_new());
  /*#p06.EDYL*/ wire EDYL_SER_CLK = not1(DAWA_SER_CLK);
  /*_p06.EPYT*/ wire EPYT_SER_CLK = not1(EDYL_SER_CLK);
  /*_p06.DEHO*/ wire DEHO_SER_CLK = not1(EPYT_SER_CLK);
  /*_p06.DAWE*/ wire DAWE_SER_CLK = not1(DEHO_SER_CLK);

  // this pin has 4 wires attached, but they're not traced
  wire EXT_ser_sin = 1;
  /*_PIN_69*/ serial.PIN_69_SIN.pin_in(EXT_ser_sin);

  /*_p06.URYS*/ wire URYS_FF01_WRn = nand4(cpu_signals.TAPU_CPU_WRp.out_new(), new_bus.SANO_FF00_FF03p(), new_bus.TOLA_A01n(),   new_bus.BUS_CPU_A00p.out_new());
  /*_p06.COHY*/ wire COHY_SER_DATA0_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D00p.out_new(), ALUR_SYS_RSTn());
  /*_p06.DUMO*/ wire DUMO_SER_DATA1_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D01p.out_new(), ALUR_SYS_RSTn());
  /*_p06.DYBO*/ wire DYBO_SER_DATA2_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D02p.out_new(), ALUR_SYS_RSTn());
  /*_p06.DAJU*/ wire DAJU_SER_DATA3_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D03p.out_new(), ALUR_SYS_RSTn());
  /*_p06.DYLY*/ wire DYLY_SER_DATA4_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D04p.out_new(), ALUR_SYS_RSTn());
  /*_p06.EHUJ*/ wire EHUJ_SER_DATA5_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D05p.out_new(), ALUR_SYS_RSTn());
  /*_p06.EFAK*/ wire EFAK_SER_DATA6_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D06p.out_new(), ALUR_SYS_RSTn());
  /*_p06.EGUV*/ wire EGUV_SER_DATA7_RSTn = or_and3(URYS_FF01_WRn, new_bus.BUS_CPU_D07p.out_new(), ALUR_SYS_RSTn());

  /*_p06.DAKU*/ wire DAKU_FF01_WRp = not1(URYS_FF01_WRn);
  /*_p06.CUFU*/ wire CUFU_SER_DATA0_SETn = nand2(new_bus.BUS_CPU_D00p.out_new(), DAKU_FF01_WRp);
  /*_p06.DOCU*/ wire DOCU_SER_DATA1_SETn = nand2(new_bus.BUS_CPU_D01p.out_new(), DAKU_FF01_WRp);
  /*_p06.DELA*/ wire DELA_SER_DATA2_SETn = nand2(new_bus.BUS_CPU_D02p.out_new(), DAKU_FF01_WRp);
  /*_p06.DYGE*/ wire DYGE_SER_DATA3_SETn = nand2(new_bus.BUS_CPU_D03p.out_new(), DAKU_FF01_WRp);
  /*_p06.DOLA*/ wire DOLA_SER_DATA4_SETn = nand2(new_bus.BUS_CPU_D04p.out_new(), DAKU_FF01_WRp);
  /*_p06.ELOK*/ wire ELOK_SER_DATA5_SETn = nand2(new_bus.BUS_CPU_D05p.out_new(), DAKU_FF01_WRp);
  /*_p06.EDEL*/ wire EDEL_SER_DATA6_SETn = nand2(new_bus.BUS_CPU_D06p.out_new(), DAKU_FF01_WRp);
  /*_p06.EFEF*/ wire EFEF_SER_DATA7_SETn = nand2(new_bus.BUS_CPU_D07p.out_new(), DAKU_FF01_WRp);

  /*_p06.CAGE*/ wire CAGE_SER_IN_new  = not1(serial.PIN_69_SIN.qp_int_new());
  /*_p06.EDER*/ serial.EDER_SER_DATA7.dff22(EPYT_SER_CLK, EFEF_SER_DATA7_SETn, EGUV_SER_DATA7_RSTn, serial.EROD_SER_DATA6.qp_old());
  /*_p06.EROD*/ serial.EROD_SER_DATA6.dff22(EPYT_SER_CLK, EDEL_SER_DATA6_SETn, EFAK_SER_DATA6_RSTn, serial.EJAB_SER_DATA5.qp_old());
  /*_p06.EJAB*/ serial.EJAB_SER_DATA5.dff22(EPYT_SER_CLK, ELOK_SER_DATA5_SETn, EHUJ_SER_DATA5_RSTn, serial.DOVU_SER_DATA4.qp_old());
  /*_p06.DOVU*/ serial.DOVU_SER_DATA4.dff22(EPYT_SER_CLK, DOLA_SER_DATA4_SETn, DYLY_SER_DATA4_RSTn, serial.DOJO_SER_DATA3.qp_old());
  /*_p06.DOJO*/ serial.DOJO_SER_DATA3.dff22(DAWE_SER_CLK, DYGE_SER_DATA3_SETn, DAJU_SER_DATA3_RSTn, serial.DYRA_SER_DATA2.qp_old());
  /*_p06.DYRA*/ serial.DYRA_SER_DATA2.dff22(DAWE_SER_CLK, DELA_SER_DATA2_SETn, DYBO_SER_DATA2_RSTn, serial.DEGU_SER_DATA1.qp_old());
  /*_p06.DEGU*/ serial.DEGU_SER_DATA1.dff22(DAWE_SER_CLK, DOCU_SER_DATA1_SETn, DUMO_SER_DATA1_RSTn, serial.CUBA_SER_DATA0.qp_old());
  /*_p06.CUBA*/ serial.CUBA_SER_DATA0.dff22(DAWE_SER_CLK, CUFU_SER_DATA0_SETn, COHY_SER_DATA0_RSTn, CAGE_SER_IN_new);

  /*_p06.UFEG*/ wire UFEG_FF01_RDp =  and4(cpu_signals.TEDO_CPU_RDp.out_new(), new_bus.SANO_FF00_FF03p(), new_bus.TOLA_A01n(), new_bus.BUS_CPU_A00p.out_new());

  /*#p06.CUGY*/ triwire CUGY_SD0_TO_CD0 = tri6_pn(UFEG_FF01_RDp, serial.CUBA_SER_DATA0.qn_new());
  /*_p06.DUDE*/ triwire DUDE_SD1_TO_CD1 = tri6_pn(UFEG_FF01_RDp, serial.DEGU_SER_DATA1.qn_new());
  /*_p06.DETU*/ triwire DETU_SD2_TO_CD2 = tri6_pn(UFEG_FF01_RDp, serial.DYRA_SER_DATA2.qn_new());
  /*_p06.DASO*/ triwire DASO_SD3_TO_CD3 = tri6_pn(UFEG_FF01_RDp, serial.DOJO_SER_DATA3.qn_new());
  /*_p06.DAME*/ triwire DAME_SD4_TO_CD4 = tri6_pn(UFEG_FF01_RDp, serial.DOVU_SER_DATA4.qn_new());
  /*_p06.EVOK*/ triwire EVOK_SD5_TO_CD5 = tri6_pn(UFEG_FF01_RDp, serial.EJAB_SER_DATA5.qn_new());
  /*_p06.EFAB*/ triwire EFAB_SD6_TO_CD6 = tri6_pn(UFEG_FF01_RDp, serial.EROD_SER_DATA6.qn_new());
  /*#p06.ETAK*/ triwire ETAK_SD7_TO_CD7 = tri6_pn(UFEG_FF01_RDp, serial.EDER_SER_DATA7.qn_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(CUGY_SD0_TO_CD0);
  /*_BUS_CPU_D01p*/ new_bus.BUS_CPU_D01p.tri_bus(DUDE_SD1_TO_CD1);
  /*_BUS_CPU_D02p*/ new_bus.BUS_CPU_D02p.tri_bus(DETU_SD2_TO_CD2);
  /*_BUS_CPU_D03p*/ new_bus.BUS_CPU_D03p.tri_bus(DASO_SD3_TO_CD3);
  /*_BUS_CPU_D04p*/ new_bus.BUS_CPU_D04p.tri_bus(DAME_SD4_TO_CD4);
  /*_BUS_CPU_D05p*/ new_bus.BUS_CPU_D05p.tri_bus(EVOK_SD5_TO_CD5);
  /*_BUS_CPU_D06p*/ new_bus.BUS_CPU_D06p.tri_bus(EFAB_SD6_TO_CD6);
  /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(ETAK_SD7_TO_CD7);

  /*_p06.UCOM*/ wire UCOM_FF02_RDp =  and4(cpu_signals.TEDO_CPU_RDp.out_new(), new_bus.SANO_FF00_FF03p(), new_bus.BUS_CPU_A01p.out_new(), new_bus.TOVY_A00n());
  /*_p06.CORE*/ triwire CORE_SER0_TO_CD0 = tri6_pn(UCOM_FF02_RDp, serial.CULY_SER_DIR.qn_new());
  /*_p06.ELUV*/ triwire ELUV_SER1_TO_CD1 = tri6_pn(UCOM_FF02_RDp, serial.ETAF_SER_RUN.qn_new());

  /*_BUS_CPU_D00p*/ new_bus.BUS_CPU_D00p.tri_bus(CORE_SER0_TO_CD0);
  /*_BUS_CPU_D07p*/ new_bus.BUS_CPU_D07p.tri_bus(ELUV_SER1_TO_CD1);
}

//------------------------------------------------------------------------------------------------------------------------
