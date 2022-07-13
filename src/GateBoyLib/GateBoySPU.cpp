#include "GateBoySPU.h"

#include "GateBoyLib/GateBoy.h"

#ifndef SIM_AUDIO

int spu_audio_out_r(GateBoyState& reg_new) {
  return 0;
}

int spu_audio_out_l(GateBoyState& reg_new) {
  return 0;
}

#endif

#ifdef SIM_AUDIO

void tick_ch1(const GateBoyState& reg_old, GateBoyState& reg_new);
void tick_ch2(const GateBoyState& reg_old, GateBoyState& reg_new);
void tick_ch3(const GateBoyState& reg_old, GateBoyState& reg_new, uint8_t* wave_ram);
void tick_ch4(const GateBoyState& reg_old, GateBoyState& reg_new);

int spu_audio_out_r(GateBoyState& reg_new) {

  int mix = 0;

  if (reg_new.spu.ANEV_NR51_RCH1_ENp.state & 1) mix += ch1_audio_out(reg_new);
  if (reg_new.spu.BOGU_NR51_RCH2_ENp.state & 1) mix += ch2_audio_out(reg_new);
  if (reg_new.spu.BAFO_NR51_RCH3_ENp.state & 1) mix += ch3_audio_out(reg_new);
  if (reg_new.spu.ATUF_NR51_RCH4_ENp.state & 1) mix += ch4_audio_out(reg_new);


  int vol_r = ((reg_new.spu.BYRE_NR50_VOL_R0.state & 1) << 0) |
              ((reg_new.spu.BUMO_NR50_VOL_R1.state & 1) << 1) |
              ((reg_new.spu.COZU_NR50_VOL_R2.state & 1) << 2);

  vol_r += 1;

  return mix * vol_r;
}

int spu_audio_out_l(GateBoyState& reg_new) {

  int mix = 0;

  if (reg_new.spu.BUME_NR51_LCH1_ENp.state & 1) mix += ch1_audio_out(reg_new);
  if (reg_new.spu.BOFA_NR51_LCH2_ENp.state & 1) mix += ch2_audio_out(reg_new);
  if (reg_new.spu.BEFO_NR51_LCH3_ENp.state & 1) mix += ch3_audio_out(reg_new);
  if (reg_new.spu.BEPU_NR51_LCH4_ENp.state & 1) mix += ch4_audio_out(reg_new);

  int vol_l = ((reg_new.spu.APEG_NR50_VOL_L0.state & 1) << 0) |
              ((reg_new.spu.BYGA_NR50_VOL_L1.state & 1) << 1) |
              ((reg_new.spu.AGER_NR50_VOL_L2.state & 1) << 2);

  vol_l += 1;

  return mix * vol_l;
}

//-----------------------------------------------------------------------------

void GateBoySPU::reset_to_cart() {
  CEMO_xBCDExxx.state   = BIT_OLD | BIT_DRIVEN;
  ATEP_AxxDExxH.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  CERY_CLK_2M.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  ATYK_AxxDExxH.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  AVOK_xBCDExxx.state   = BIT_OLD | BIT_DRIVEN;
  AJER_AxxDExxH.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  JESO_CLK_512K.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  BARA_CLK_512.state  = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  CARU_CLK_256.state  = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  BYLU_CLK_128.state  = BIT_OLD | BIT_DRIVEN;

  APEG_NR50_VOL_L0.state = 0x1b;
  BYGA_NR50_VOL_L1.state = 0x1b;
  AGER_NR50_VOL_L2.state = 0x1b;
  APOS_NR50_VIN_TO_L.state = 0x1a;
  BYRE_NR50_VOL_R0.state = 0x1b;
  BUMO_NR50_VOL_R1.state = 0x1b;
  COZU_NR50_VOL_R2.state = 0x1b;
  BEDU_NR50_VIN_TO_R.state = 0x1a;

  ANEV_NR51_RCH1_ENp.state = 0x1b;
  BOGU_NR51_RCH2_ENp.state = 0x1b;
  BAFO_NR51_RCH3_ENp.state = 0x1a;
  ATUF_NR51_RCH4_ENp.state = 0x1a;
  BUME_NR51_LCH1_ENp.state = 0x1b;
  BOFA_NR51_LCH2_ENp.state = 0x1b;
  BEFO_NR51_LCH3_ENp.state = 0x1b;
  BEPU_NR51_LCH4_ENp.state = 0x1b;

  FERO_NR52_DBG_APUp.state = 0x1a;
  BOWY_NR52_DBG_SWEEP.state = 0x1a;
  HADA_NR52_ALL_SOUND_ON.state = 0x1b;
}

//-----------------------------------------------------------------------------

void tick_spu(const GateBoyState& reg_old, GateBoyState& reg_new, uint8_t* wave_ram) {

  memset(&reg_new.wave_dbus, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.wave_dbus));

  // The APU reset register _must_ be ticked first.
  {
    /*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(reg_new.cpu_abus.CONA_ADDR_FF2Xp(), reg_new.cpu_abus.EKAG_ADDR_0110p()); // was this wrong on the schematic?
    /*#p09.HAWU*/ wire HAWU_NR52_WRn = nand2(DOXY_ADDR_FF26p, reg_new.cpu_signals.BOGY_CPU_WRp());
    /*#p09.HADA*/ reg_new.spu.HADA_NR52_ALL_SOUND_ON.dff17(HAWU_NR52_WRn, reg_new.GUFO_SYS_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old()); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.

    /*#p09.ETUC*/ wire ETUC_NR52_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), DOXY_ADDR_FF26p);
    /*#p09.FOKU*/ wire FOKU_NR52_WRn = not1(ETUC_NR52_WRp);
    /*#p09.EFOP*/ wire EFOP_NR52_DBG_APUp_old = and2(reg_old.cpu_dbus.BUS_CPU_D04p.out_old(), reg_old.cpu_signals.SIG_CPU_UNOR_DBG.qp_old());
    /*#p09.FERO*/ reg_new.spu.FERO_NR52_DBG_APUp.dff9b(FOKU_NR52_WRn, reg_new.KEPY_APU_RSTn_new(), EFOP_NR52_DBG_APUp_old);
  }

  //probe(0, "KEPY_APU_RSTn", reg_new.KEPY_APU_RSTn_new());

  //----------
  // SPU clock dividers

  /*_p01.CERY*/ reg_new.spu.CERY_CLK_2M.dff17(
    reg_new.sys_clk.CYBO_AxCxExGx(),
    reg_new.BELA_APU_RSTn_new(),
    reg_old.spu.CERY_CLK_2M.qn_old()
    );
  /*_p01.ATYK*/ reg_new.spu.ATYK_AxxDExxH.dff17(reg_new.sys_clk.ARYF_AxCxExGx(),  reg_new.BOPO_APU_RSTn_new(), reg_old.spu.ATYK_AxxDExxH.qn_old());
  /*_p09.AJER*/ reg_new.spu.AJER_AxxDExxH.dff17(reg_new.sys_clk.APUV_AxCxExGx(),  reg_new.ATYV_APU_RSTn_new(), reg_old.spu.AJER_AxxDExxH.qn_old());
  /*_p01.AVOK*/ reg_new.spu.AVOK_xBCDExxx.dff17(reg_new.spu.ATYK_AxxDExxH.qn_new(), reg_new.BOPO_APU_RSTn_new(), reg_old.spu.AVOK_xBCDExxx.qn_old());

  /*#p09.CALO*/ reg_new.ch1.CALO_xBCDExxx.dff17(reg_new.spu.BATA_CLK_2M(), reg_new.AGUR_APU_RSTn_new(), reg_old.ch1.CALO_xBCDExxx.qn_old());

  /*_p01.JESO*/ reg_new.spu.JESO_CLK_512K.dff17(reg_new.spu.BAVU_CLK_1M(), reg_new.KAME_APU_RSTn_new(), reg_old.spu.JESO_CLK_512K.qn_old());

  /*_p15.ATEP*/ reg_new.spu.ATEP_AxxDExxH.dff17(reg_new.sys_clk.AZEG_AxCxExGx(), reg_new.BUWE_APU_RSTn_new(), reg_old.spu.ATEP_AxxDExxH.qn_old());

  /*#p15.BUFO*/ wire BUFO = not1(reg_new.spu.ATEP_AxxDExxH.qp_new());
  /*#p15.CEMO*/ reg_new.spu.CEMO_xBCDExxx.dff17(BUFO, reg_new.BYHO_APU_RSTn_new(), reg_old.spu.CEMO_xBCDExxx.qn_old());

  //----------
  // Low-speed clocks are picked up from DIV

  /*_p01.COKE*/ wire COKE_AxxDExxH = not1(reg_new.spu.AJER_AxxDExxH.qn_new());
  ///*_p01.UMER*/ wire UMER_DIV10n_old = not1(reg_old.reg_div.TERU_DIV10p.qp_old());

  // FIXME speed up spu clocks for debugging
  wire UMER_DIV10n_old = not1(reg_old.reg_div.UNYK_DIV04p.qp_old());

  /*_p01.BARA*/ reg_new.spu.BARA_CLK_512.dff17(COKE_AxxDExxH,                     reg_new.ATUS_APU_RSTn_new(), UMER_DIV10n_old);
  /*_p01.CARU*/ reg_new.spu.CARU_CLK_256.dff17(reg_new.spu.BURE_CLK_512_new(),    reg_new.ATUS_APU_RSTn_new(), reg_old.spu.CARU_CLK_256.qn_old());
  /*_p01.BYLU*/ reg_new.spu.BYLU_CLK_128.dff17(reg_new.spu.CARU_CLK_256.qn_new(), reg_new.ATUS_APU_RSTn_new(), reg_old.spu.BYLU_CLK_128.qn_old());

  tick_ch1(reg_old, reg_new);
  tick_ch2(reg_old, reg_new);
  tick_ch3(reg_old, reg_new, wave_ram);
  tick_ch4(reg_old, reg_new);

  {
    /*_p09.BOSU*/ wire BOSU_NR50_WRn = nand2(reg_new.cpu_abus.CAFY_ADDR_FF24p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*_p09.BAXY*/ wire BAXY_NR50_WRp = not1(BOSU_NR50_WRn);
    /*_p09.BOWE*/ wire BOWE_NR50_WRp = not1(BOSU_NR50_WRn);
    /*_p09.BUBU*/ wire BUBU_NR50_WRn = not1(BAXY_NR50_WRp);
    /*_p09.ATAF*/ wire ATAF_NR50_WRn = not1(BOWE_NR50_WRp);

    // APEG01 NC
    // APEG02 ATAF
    // APEG03 D0
    // APEG04 BOWE
    // APEG05 NC
    // APEG06 KEPY
    // APEG07
    // APEG08
    // APEG09

    /*_p09.APEG*/ reg_new.spu.APEG_NR50_VOL_L0   .dff9b(ATAF_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p09.BYGA*/ reg_new.spu.BYGA_NR50_VOL_L1   .dff9b(ATAF_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p09.AGER*/ reg_new.spu.AGER_NR50_VOL_L2   .dff9b(ATAF_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p09.APOS*/ reg_new.spu.APOS_NR50_VIN_TO_L .dff9b(ATAF_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p09.BYRE*/ reg_new.spu.BYRE_NR50_VOL_R0   .dff9b(BUBU_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p09.BUMO*/ reg_new.spu.BUMO_NR50_VOL_R1   .dff9b(BUBU_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p09.COZU*/ reg_new.spu.COZU_NR50_VOL_R2   .dff9b(BUBU_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p09.BEDU*/ reg_new.spu.BEDU_NR50_VIN_TO_R .dff9b(BUBU_NR50_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    // The die trace for NR51 has the clkp/clkn backwards compared to the other dff9s.

    /*#p09.BUPO*/ wire BUPO_NR51_WRn = nand2(reg_new.cpu_abus.CORA_ADDR_FF25p(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*#p09.BONO*/ wire BONO_NR51_WRp = not1(BUPO_NR51_WRn);
    /*#p09.BYFA*/ wire BYFA_NR51_WRp = not1(BUPO_NR51_WRn);
    /*#p09.BONO*/ wire BONO_NR51_WRn = not1(BONO_NR51_WRp);
    /*#p09.BYFA*/ wire BYFA_NR51_WRn = not1(BYFA_NR51_WRp);

    // ANEV01 nc
    // ANEV02 !BONO
    // ANEV03 D0
    // ANEV04 BONO
    // ANEV05 NC
    // ANEV06 KEPY
    // ANEV07
    // ANEV08
    // ANEV09

    /*_p09.ANEV*/ reg_new.spu.ANEV_NR51_RCH1_ENp.dff9b(BONO_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p09.BOGU*/ reg_new.spu.BOGU_NR51_RCH2_ENp.dff9b(BONO_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p09.BAFO*/ reg_new.spu.BAFO_NR51_RCH3_ENp.dff9b(BONO_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p09.ATUF*/ reg_new.spu.ATUF_NR51_RCH4_ENp.dff9b(BONO_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p09.BUME*/ reg_new.spu.BUME_NR51_LCH1_ENp.dff9b(BYFA_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p09.BOFA*/ reg_new.spu.BOFA_NR51_LCH2_ENp.dff9b(BYFA_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p09.BEFO*/ reg_new.spu.BEFO_NR51_LCH3_ENp.dff9b(BYFA_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p09.BEPU*/ reg_new.spu.BEPU_NR51_LCH4_ENp.dff9b(BYFA_NR51_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(reg_new.cpu_abus.CONA_ADDR_FF2Xp(), reg_new.cpu_abus.EKAG_ADDR_0110p()); // was this wrong on the schematic?
    /*#p09.BOPY*/ wire BOPY_NR52_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), DOXY_ADDR_FF26p);
    /*#p09.BOWY*/ reg_new.spu.BOWY_NR52_DBG_SWEEP   .dff17(BOPY_NR52_WRn, reg_new.KEPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  }










  {
    /*_p09.BEFU*/ wire BEFU_NR50_RDp = nor2(reg_new.cpu_signals.AGUZ_CPU_RDn(), reg_new.cpu_abus.BYMA_ADDR_FF24n());
    /*_p09.ADAK*/ wire ADAK_NR50_RDn = not1(BEFU_NR50_RDp);

    /*_p09.AKOD*/ triwire AKOD_D00 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.APEG_NR50_VOL_L0.qn_newB());
    /*_p09.AWED*/ triwire AWED_D01 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.BYGA_NR50_VOL_L1.qn_newB());
    /*_p09.AVUD*/ triwire AVUD_D02 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.AGER_NR50_VOL_L2.qn_newB());
    /*_p09.AXEM*/ triwire AXEM_D03 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.APOS_NR50_VIN_TO_L.qn_newB());
    /*_p09.AMAD*/ triwire AMAD_D04 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.BYRE_NR50_VOL_R0.qn_newB());
    /*_p09.ARUX*/ triwire ARUX_D05 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.BUMO_NR50_VOL_R1.qn_newB());
    /*_p09.BOCY*/ triwire BOCY_D06 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.COZU_NR50_VOL_R2.qn_newB());
    /*_p09.ATUM*/ triwire ATUR_D07 = tri6_nn(ADAK_NR50_RDn, reg_new.spu.BEDU_NR50_VIN_TO_R.qn_newB());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(AKOD_D00);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(AWED_D01);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(AVUD_D02);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(AXEM_D03);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(AMAD_D04);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(ARUX_D05);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(BOCY_D06);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(ATUR_D07);
  }

  {
    /*_p09.HEFA*/ wire HEFA_NR51_RDp = nor2(reg_new.cpu_abus.GEPA_ADDR_FF25n(), reg_new.cpu_signals.AGUZ_CPU_RDn()); // outline wrong color in die
    /*_p09.GUMU*/ wire GUMU_NR51_RDn = not1(HEFA_NR51_RDp);

    /*_p09.BUZU*/ triwire BUZU_D0 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.ANEV_NR51_RCH1_ENp.qn_newB()); 
    /*_p09.CAPU*/ triwire CAPU_D1 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.BOGU_NR51_RCH2_ENp.qn_newB()); 
    /*_p09.CAGA*/ triwire CAGA_D2 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.BAFO_NR51_RCH3_ENp.qn_newB()); 
    /*_p09.BOCA*/ triwire BOCA_D3 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.ATUF_NR51_RCH4_ENp.qn_newB()); 
    /*_p09.CAVU*/ triwire CAVU_D4 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.BUME_NR51_LCH1_ENp.qn_newB()); 
    /*_p09.CUDU*/ triwire CUDU_D5 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.BOFA_NR51_LCH2_ENp.qn_newB()); 
    /*_p09.CADA*/ triwire CADA_D6 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.BEFO_NR51_LCH3_ENp.qn_newB()); 
    /*_p09.CERE*/ triwire CERE_D7 = tri6_nn(GUMU_NR51_RDn, reg_new.spu.BEPU_NR51_LCH4_ENp.qn_newB()); 

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(BUZU_D0);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(CAPU_D1);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(CAGA_D2);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(BOCA_D3);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(CAVU_D4);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(CUDU_D5);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(CADA_D6);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(CERE_D7);
  }

  {
    /*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(reg_new.cpu_abus.CONA_ADDR_FF2Xp(), reg_new.cpu_abus.EKAG_ADDR_0110p()); // was this wrong on the schematic?
    /*#p09.DOLE*/ wire DOLE_NR52_RDn = nand2(DOXY_ADDR_FF26p, reg_new.cpu_signals.CETO_CPU_RDp());
    /*#p09.DURU*/ wire DURU_NR52_RDn = nand2(DOXY_ADDR_FF26p, reg_new.cpu_signals.CURU_CPU_RDp());
    /*#p09.FEWA*/ wire FEWA_NR52_RDn = nand2(DOXY_ADDR_FF26p, reg_new.cpu_signals.GAXO_CPU_RDp());
    /*#p09.KAMU*/ wire KAMU_NR52_RDn = nand2(DOXY_ADDR_FF26p, reg_new.cpu_signals.KAZO_CPU_RDp());
    /*#p09.JURE*/ wire JURE_NR52_RDn = nand2(reg_new.cpu_signals.KYDU_CPU_RDp(), DOXY_ADDR_FF26p);

    /*#p13.CARA*/ wire CARA_CH1_ACTIVEn = not1(reg_new.ch1.CYTO_CH1_ACTIVEp.qp_new());
    /*#p15.DEFU*/ wire DEFU_CH2_ACTIVEn = not1(reg_new.ch2.DANE_CH2_ACTIVEp.qp_new());
    /*#p18.ERED*/ wire ERED_CH3_ACTIVEn = not1(reg_new.ch3.COKA_CH3_ACTIVEp());
    /*#p20.JUWA*/ wire JUWA_CH4_ACTIVEn = not1(reg_new.ch4.GENA_CH4_ACTIVEp.qp_new());

    /*#p09.COTO*/ triwire COTO_D0 = tri6_nn(DOLE_NR52_RDn, CARA_CH1_ACTIVEn);
    /*#p09.EFUS*/ triwire EFUS_D1 = tri6_nn(DURU_NR52_RDn, DEFU_CH2_ACTIVEn);
    /*_p09.FATE*/ triwire FATE_D2 = tri6_nn(FEWA_NR52_RDn, ERED_CH3_ACTIVEn);
    /*_p09.KOGE*/ triwire KOGE_D3 = tri6_nn(KAMU_NR52_RDn, JUWA_CH4_ACTIVEn);
    /*_p09.HOPE*/ triwire HOPE_D7 = tri6_nn(JURE_NR52_RDn, reg_new.spu.HADA_NR52_ALL_SOUND_ON.qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(COTO_D0);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(EFUS_D1);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(FATE_D2);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(KOGE_D3);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(HOPE_D7);
  }
}

#endif