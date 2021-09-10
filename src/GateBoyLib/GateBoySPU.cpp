/// plait_noparse

#include "GateBoyLib/GateBoySPU.h"
#include "GateBoyLib/GateBoy.h"

#pragma warning(disable : 4189)

//-----------------------------------------------------------------------------

void GateBoy::tock_spu_gates() {
#if 0
  /*#p09.HAPO*/ wire HAPO_SYS_RESETp = not1(ALUR_SYS_RSTn());
  /*#p09.GUFO*/ wire GUFO_SYS_RESETn = not1(HAPO_SYS_RESETp);

  /*#p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(cpu_signals.TEDO_CPU_RDp.out_new());
  /*#p09.CETO*/ wire CETO_CPU_RDp = not1(AGUZ_CPU_RDn);
  /*#p09.KAZO*/ wire KAZO_CPU_RDp = not1(AGUZ_CPU_RDn);
  /*#p09.CURU*/ wire CURU_CPU_RDp = not1(AGUZ_CPU_RDn);
  /*#p09.GAXO*/ wire GAXO_CPU_RDp = not1(AGUZ_CPU_RDn);
  /*#p09.KYDU*/ wire KYDU_CPU_RDp = not1(AGUZ_CPU_RDn);


  /*#p10.BAFU*/ wire BAFU_CPU_WRn = not1(cpu_signals.TAPU_CPU_WRp.out_new());
  /*#p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn); // biiiig not

  /*#p07.BAKO*/ wire BAKO_ADDR_FFXXn1 = not1(cpu_abus_new.SYKE_ADDR_HIp());

  /*#p10.ATUP*/ wire ATUP_ADDR_A04n = not1(cpu_abus_new.BUS_CPU_A04p.out_any());
  /*#p10.BOXY*/ wire BOXY_ADDR_A05n = not1(cpu_abus_new.BUS_CPU_A05p.out_any());
  /*#p10.ASAD*/ wire ASAD_ADDR_A06n = not1(cpu_abus_new.BUS_CPU_A06p.out_any());
  /*#p10.AVUN*/ wire AVUN_ADDR_A07n = not1(cpu_abus_new.BUS_CPU_A07p.out_any());

  /*#p10.ATEG*/ wire ATEG_ADDR_XX1Xn = or4(ATUP_ADDR_A04n, cpu_abus_new.BUS_CPU_A05p.out_new(), cpu_abus_new.BUS_CPU_A06p.out_new(), cpu_abus_new.BUS_CPU_A07p.out_new());
  /*#p10.AWET*/ wire AWET_ADDR_XX2Xn = or4(cpu_abus_new.BUS_CPU_A04p.out_new(), BOXY_ADDR_A05n, cpu_abus_new.BUS_CPU_A06p.out_new(), cpu_abus_new.BUS_CPU_A07p.out_new());

  /*#p10.BUNO*/ wire BUNO_ADDR_FF1X  = nor2(BAKO_ADDR_FFXXn1, ATEG_ADDR_XX1Xn);
  /*#p10.BANU*/ wire BANU_ADDR_FF1Xn = not1(BUNO_ADDR_FF1X);
  /*#p10.BEZY*/ wire BEZY_ADDR_FF2Xn = or2(AWET_ADDR_XX2Xn, BAKO_ADDR_FFXXn1);

  /*#p10.DYTE*/ wire DYTE_ADDR_xxx0  = not1(cpu_abus_new.BUS_CPU_A00p.out_new());
  /*#p10.AFOB*/ wire AFOB_ADDR_xx0x  = not1(cpu_abus_new.BUS_CPU_A01p.out_new());
  /*#p10.ABUB*/ wire ABUB_ADDR_x0xx  = not1(cpu_abus_new.BUS_CPU_A02p.out_new());
  /*#p10.ACOL*/ wire ACOL_ADDR_0xxx  = not1(cpu_abus_new.BUS_CPU_A03p.out_new());

  /*#p10.DOSO*/ wire DOSO_ADDR_xxx1  = not1(DYTE_ADDR_xxx0);
  /*#p10.DUPA*/ wire DUPA_ADDR_xx1x  = not1(AFOB_ADDR_xx0x);
  /*#p10.DENO*/ wire DENO_ADDR_x1xx  = not1(ABUB_ADDR_x0xx);
  /*#p10.DUCE*/ wire DUCE_ADDR_1xxx  = not1(ACOL_ADDR_0xxx);

  /*#p10.DATU*/ wire DATU_ADDR_0100n = nand4(DYTE_ADDR_xxx0, AFOB_ADDR_xx0x, DENO_ADDR_x1xx, ACOL_ADDR_0xxx);
  /*#p10.EKAG*/ wire EKAG_ADDR_0110p = and4 (ACOL_ADDR_0xxx, DENO_ADDR_x1xx, DUPA_ADDR_xx1x, DYTE_ADDR_xxx0);
  /*#p10.CONA*/ wire CONA_ADDR_FF2X  = not1(BEZY_ADDR_FF2Xn);

#if 0
    ///*_p09.EDEK* / next.apu.NR52_DBG_APU       = not1(!b.apu.NR52_DBG_APUn);
    ///*_p18.COKA* / wire CH3_ACTIVE = not1(!b.ch3.CH3_ACTIVEo);
    ///*_p18.ERED* / wire CH3_ACTIVEn = not1(CH3_ACTIVE);
    ///*_p20.JUWA* / wire CH4_ACTIVEn = not1(b.ch4.CH4_ACTIVE);

    if (NR52_RDna) next.bus.set_data(
      ///*_p09.COTO* / not1(CH1_ACTIVEn),
      ///*_p09.EFUS* / not1(CH2_ACTIVEn),
      ///*_p09.FATE* / not1(CH3_ACTIVEn),
      ///*_p09.KOGE* / not1(CH4_ACTIVEn),
      ///*_p09.HOPE* / b.apu.NR52_ALL_SOUND_ON
    );
#endif

  /*#p10.DOXY*/ wire DOXY_ADDR_FF26 = and2 (CONA_ADDR_FF2X,  EKAG_ADDR_0110p);

  /*#p09.ETUC*/ wire ETUC_NR52_WRp = and2 (BOGY_CPU_WRp, DOXY_ADDR_FF26);
  /*#p09.HAWU*/ wire HAWU_NR52_WRn = nand2(DOXY_ADDR_FF26, BOGY_CPU_WRp);
  /*#p09.BOPY*/ wire BOPY_NR52_WRn = nand2(BOGY_CPU_WRp, DOXY_ADDR_FF26);
  /*#p09.FOKU*/ wire FOKU_NR52_WRn = not1(ETUC_NR52_WRp);

  /*#p09.EFOP*/ wire EFOP_DBG_APUp  = and2(cpu_dbus_old.BUS_CPU_D04p.out_old(), UNOR_MODE_DBG2p());

  /*#p09.HADA*/ reg_NR52.HADA_ALL_SOUND_ONp.dff17(HAWU_NR52_WRn, GUFO_SYS_RESETn, cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*#p09.JYRO*/ wire JYRO_APU_RSTp = or2(HAPO_SYS_RESETp, reg_NR52.HADA_ALL_SOUND_ONp.qn_new());
  /*#p09.KEPY*/ wire KEPY_APU_RESETn = not1(JYRO_APU_RSTp);
  /*#p09.KUBY*/ wire KUBY_APU_RESETn = not1(JYRO_APU_RSTp);
  /*#p09.FERO*/ reg_NR52.FERO_DBG_APUp     .dff9 (FOKU_NR52_WRn, KEPY_APU_RESETn, EFOP_DBG_APUp);
  /*#p09.BOWY*/ reg_NR52.BOWY_DBG_SWEEPp   .dff17(BOPY_NR52_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D05p.out_old());

  //-----------------------------------------------------------------------------

  /*#p09.DOLE*/ wire DOLE_NR52_RDn = nand2(DOXY_ADDR_FF26, CETO_CPU_RDp);
  /*#p09.KAMU*/ wire KAMU_NR52_RDn = nand2(DOXY_ADDR_FF26, KAZO_CPU_RDp);
  /*#p09.DURU*/ wire DURU_NR52_RDn = nand2(DOXY_ADDR_FF26, CURU_CPU_RDp);
  /*#p09.FEWA*/ wire FEWA_NR52_RDn = nand2(DOXY_ADDR_FF26, GAXO_CPU_RDp);
  /*#p09.JURE*/ wire JURE_NR52_RDn = nand2(KYDU_CPU_RDp, DOXY_ADDR_FF26);

  wire CARA_CH1_ACTIVE = 0; // FIXME stub input d
  wire DEFU_CH2_ACTIVE = 0; // FIXME stub input d
  wire ERED_CH3_ACTIVE = 0; // FIXME stub input d
  wire JUWA_CH4_ACTIVE = 0; // FIXME stub input d

  /*#p23.COTO*/ triwire COTO_NR52_TO_CD0 = tri6_nn(DOLE_NR52_RDn, CARA_CH1_ACTIVE);
  /*#p23.EFUS*/ triwire EFUS_NR52_TO_CD1 = tri6_nn(DURU_NR52_RDn, DEFU_CH2_ACTIVE);
  /*#p23.FATE*/ triwire FATE_NR52_TO_CD2 = tri6_nn(FEWA_NR52_RDn, ERED_CH3_ACTIVE);
  /*#p23.KOGE*/ triwire KOGE_NR52_TO_CD3 = tri6_nn(KAMU_NR52_RDn, JUWA_CH4_ACTIVE);
  /*#p23.HOPE*/ triwire HOPE_NR52_TO_CD7 = tri6_nn(JURE_NR52_RDn, reg_NR52.HADA_ALL_SOUND_ONp.qn_new());

  /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(COTO_NR52_TO_CD0);
  /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(EFUS_NR52_TO_CD1);
  /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(FATE_NR52_TO_CD2);
  /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(KOGE_NR52_TO_CD3);
  /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(HOPE_NR52_TO_CD7);

  //-----------------------------------------------------------------------------

  /*#p10.CAFY*/ wire CAFY_ADDR_FF24p = nor2(DATU_ADDR_0100n, BEZY_ADDR_FF2Xn);
  /*#p09.BYMA*/ wire BYMA_ADDR_FF24n = not1(CAFY_ADDR_FF24p);
  /*#p09.BOSU*/ wire BOSU_NR50_WRn = nand2(CAFY_ADDR_FF24p, BOGY_CPU_WRp);
  /*#p09.BAXY*/ wire BAXY_NR50_WRp = not1(BOSU_NR50_WRn);
  /*#p09.BOWE*/ wire BOWE_NR50_WRp = not1(BOSU_NR50_WRn);
  /*#p09.BUBU*/ wire BUBU_NR50_WRn = not1(BAXY_NR50_WRp);
  /*#p09.ATAF*/ wire ATAF_NR50_WRn = not1(BOWE_NR50_WRp);

  /*#p09.APEG*/ reg_NR50.APEG_VOL_L0  .dff9(ATAF_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*#p09.BYGA*/ reg_NR50.BYGA_VOL_L1  .dff9(ATAF_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*#p09.AGER*/ reg_NR50.AGER_VOL_L2  .dff9(ATAF_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*#p09.APOS*/ reg_NR50.APOS_VIN_TO_L.dff9(ATAF_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*#p09.BYRE*/ reg_NR50.BYRE_VOL_R0  .dff9(BUBU_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*#p09.BUMO*/ reg_NR50.BUMO_VOL_R1  .dff9(BUBU_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*#p09.COZU*/ reg_NR50.COZU_VOL_R2  .dff9(BUBU_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*#p09.BEDU*/ reg_NR50.BEDU_VIN_TO_R.dff9(BUBU_NR50_WRn, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*#p09.BEFU*/ wire BEFU_NR50_RDp = nor2(AGUZ_CPU_RDn, BYMA_ADDR_FF24n);
  /*#p09.ADAK*/ wire ADAK_NR50_RDn = not1(BEFU_NR50_RDp);

  /*#p23.AKOD*/ triwire AKOD_NR50_TO_CD0 = tri6_nn(ADAK_NR50_RDn, reg_NR50.APEG_VOL_L0.qp_new());
  /*#p23.AWED*/ triwire AWED_NR50_TO_CD1 = tri6_nn(ADAK_NR50_RDn, reg_NR50.BYGA_VOL_L1.qp_new());
  /*#p23.AVUD*/ triwire AVUD_NR50_TO_CD2 = tri6_nn(ADAK_NR50_RDn, reg_NR50.AGER_VOL_L2.qp_new());
  /*#p23.AXEM*/ triwire AXEM_NR50_TO_CD3 = tri6_nn(ADAK_NR50_RDn, reg_NR50.APOS_VIN_TO_L.qp_new());
  /*#p23.AMAD*/ triwire AMAD_NR50_TO_CD4 = tri6_nn(ADAK_NR50_RDn, reg_NR50.BYRE_VOL_R0.qp_new());
  /*#p23.ARUX*/ triwire ARUX_NR50_TO_CD5 = tri6_nn(ADAK_NR50_RDn, reg_NR50.BUMO_VOL_R1.qp_new());
  /*#p23.BOCY*/ triwire BOCY_NR50_TO_CD6 = tri6_nn(ADAK_NR50_RDn, reg_NR50.COZU_VOL_R2.qp_new());
  /*#p23.ATUM*/ triwire ATUM_NR50_TO_CD7 = tri6_nn(ADAK_NR50_RDn, reg_NR50.BEDU_VIN_TO_R.qp_new());

  /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(AKOD_NR50_TO_CD0);
  /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(AWED_NR50_TO_CD1);
  /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(AVUD_NR50_TO_CD2);
  /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(AXEM_NR50_TO_CD3);
  /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(AMAD_NR50_TO_CD4);
  /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(ARUX_NR50_TO_CD5);
  /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(BOCY_NR50_TO_CD6);
  /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(ATUM_NR50_TO_CD7);

  //-----------------------------------------------------------------------------


  /*_p10.DURA*/ wire DURA_ADDR_0101n = nand4(ACOL_ADDR_0xxx, DENO_ADDR_x1xx, AFOB_ADDR_xx0x, DOSO_ADDR_xxx1);
  /*_p10.CORA*/ wire CORA_ADDR_FF25p = nor2(BEZY_ADDR_FF2Xn, DURA_ADDR_0101n);
  /*_p09.GEPA*/ wire GEPA_ADDR_FF25n = not1(CORA_ADDR_FF25p);
  /*_p09.BUPO*/ wire BUPO_NR51_WRn = nand2(CORA_ADDR_FF25p, BOGY_CPU_WRp);

  /*_p09.BONO*/ wire BONO_NR51_WRp = not1(BUPO_NR51_WRn);
  /*_p09.BYFA*/ wire BYFA_NR51_WRp = not1(BUPO_NR51_WRn);

  wire COMP_CLOCK = not(BONO_NR51_WRp);

  /*_p09.ANEV*/ reg_NR51.ANEV_NR51_0.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p09.BOGU*/ reg_NR51.BOGU_NR51_1.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p09.BAFO*/ reg_NR51.BAFO_NR51_2.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*_p09.ATUF*/ reg_NR51.ATUF_NR51_3.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*_p09.BUME*/ reg_NR51.BUME_NR51_4.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*_p09.BOFA*/ reg_NR51.BOFA_NR51_5.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*_p09.BEFO*/ reg_NR51.BEFO_NR51_6.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*_p09.BEPU*/ reg_NR51.BEPU_NR51_7.dff9(COMP_CLOCK, KEPY_APU_RESETn, cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*_p09.HEFA*/ wire HEFA_NR51_RDp = nor2(GEPA_ADDR_FF25n, AGUZ_CPU_RDn);
  /*_p09.GUMU*/ wire GUMU_NR51_RDn = not1(HEFA_NR51_RDp);

  /*_p09.CAPU*/ triwire CAPU_NR51_TO_CD0 = tri6_nn(GUMU_NR51_RDn, reg_NR51.ANEV_NR51_0.qp_new());
  /*_p09.CAGA*/ triwire CAGA_NR51_TO_CD1 = tri6_nn(GUMU_NR51_RDn, reg_NR51.BOGU_NR51_1.qp_new());
  /*_p09.BOCA*/ triwire BOCA_NR51_TO_CD2 = tri6_nn(GUMU_NR51_RDn, reg_NR51.BAFO_NR51_2.qp_new());
  /*_p09.BUZU*/ triwire BUZU_NR51_TO_CD3 = tri6_nn(GUMU_NR51_RDn, reg_NR51.ATUF_NR51_3.qp_new());
  /*_p09.CERE*/ triwire CERE_NR51_TO_CD4 = tri6_nn(GUMU_NR51_RDn, reg_NR51.BUME_NR51_4.qp_new());
  /*_p09.CADA*/ triwire CADA_NR51_TO_CD5 = tri6_nn(GUMU_NR51_RDn, reg_NR51.BOFA_NR51_5.qp_new());
  /*_p09.CAVU*/ triwire CAVU_NR51_TO_CD6 = tri6_nn(GUMU_NR51_RDn, reg_NR51.BEFO_NR51_6.qp_new());
  /*_p09.CUDU*/ triwire CUDU_NR51_TO_CD7 = tri6_nn(GUMU_NR51_RDn, reg_NR51.BEPU_NR51_7.qp_new());

  /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(CAPU_NR51_TO_CD0);
  /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(CAGA_NR51_TO_CD1);
  /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(BOCA_NR51_TO_CD2);
  /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(BUZU_NR51_TO_CD3);
  /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(CERE_NR51_TO_CD4);
  /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(CADA_NR51_TO_CD5);
  /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(CAVU_NR51_TO_CD6);
  /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(CUDU_NR51_TO_CD7);
#endif
}

//-----------------------------------------------------------------------------

void GateBoy::tock_spu_logic() {
#if 0
  auto cpu_addr_new = pack(cpu_abus_new);
  auto cpu_rd_new = bit(cpu_signals.SIG_IN_CPU_RDp);
  auto cpu_wr_new = bit(cpu_signals.SIG_IN_CPU_WRp);

  // cpu_signals.APOV_CPU_WRp = CLK_xxxxEFGx && cpu_wr_new;

#if 0
    //next.apu.NR52_DBG_APU       = not1(!b.apu.NR52_DBG_APUn);
    //wire CH3_ACTIVE = not1(!b.ch3.CH3_ACTIVEo);
    //wire CH3_ACTIVEn = not1(CH3_ACTIVE);
    //wire CH4_ACTIVEn = not1(b.ch4.CH4_ACTIVE);

    if (NR52_RDna) next.bus.set_data(
      // not1(CH1_ACTIVEn),
      // not1(CH2_ACTIVEn),
      // not1(CH3_ACTIVEn),
      // not1(CH4_ACTIVEn),
      // b.apu.NR52_ALL_SOUND_ON
    );
#endif

  if (cpu_wr_new && (cpu_addr_new == 0xFF26) && DELTA_GH) {
    reg_NR52.HADA_ALL_SOUND_ONp.state = cpu_dbus_old.BUS_CPU_D07p.state;
    reg_NR52.FERO_DBG_APUp     .state = 0;
    reg_NR52.BOWY_DBG_SWEEPp   .state = cpu_dbus_old.BUS_CPU_D05p.state;
  }

  bool spu_rst = bit(~reg_NR52.HADA_ALL_SOUND_ONp.state);

  if (spu_rst) {
    reg_NR52.FERO_DBG_APUp.state = 0;
    reg_NR52.BOWY_DBG_SWEEPp.state = 0;
  }

  //-----------------------------------------------------------------------------

  wire CARA_CH1_ACTIVE = 0; // FIXME stub input d
  wire DEFU_CH2_ACTIVE = 0; // FIXME stub input d
  wire ERED_CH3_ACTIVE = 0; // FIXME stub input d
  wire JUWA_CH4_ACTIVE = 0; // FIXME stub input d

  if (cpu_rd_new && (cpu_addr_new == 0xFF26)) {
    cpu_dbus_new.BUS_CPU_D00p.state = CARA_CH1_ACTIVE;
    cpu_dbus_new.BUS_CPU_D01p.state = DEFU_CH2_ACTIVE;
    cpu_dbus_new.BUS_CPU_D02p.state = ERED_CH3_ACTIVE;
    cpu_dbus_new.BUS_CPU_D03p.state = JUWA_CH4_ACTIVE;
    cpu_dbus_new.BUS_CPU_D07p.state = reg_NR52.HADA_ALL_SOUND_ONp.state;
  }

  //-----------------------------------------------------------------------------

  if (cpu_wr_new && (cpu_addr_new == 0xFF24) && DELTA_GH) {
    unpack2(reg_NR50, pack(cpu_dbus_new));
  }

  if (spu_rst) {
    clear(reg_NR50);
  }

  if (cpu_rd_new && (cpu_addr_new == 0xFF24)) {
    unpack2(cpu_dbus_new, pack(reg_NR50));
  }

  //-----------------------------------------------------------------------------

  if (cpu_wr_new && (cpu_addr_new == 0xFF25) && DELTA_GH) {
    unpack2(reg_NR51, pack(cpu_dbus_new));
  }

  if (spu_rst) clear(reg_NR51);

  if (cpu_rd_new && (cpu_addr_new == 0xFF25)) {
    unpack2(cpu_dbus_new, pack(reg_NR51));
  }
#endif
}

//-----------------------------------------------------------------------------
