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

void tick_ch1_fast(
  const GateBoyCpuDBus& cpu_dbus_old,
  const GateBoySPU& spu_old,
  const SpuChannel1& ch1_old,
  const GateBoyCpuABus& cpu_abus_new,

  GateBoyCpuDBus& cpu_dbus_new,
  GateBoySPU& spu_new,
  SpuChannel1& ch1_new,

  wire AFER_SYS_RSTp,
  wire ASOL_POR_DONEn,
  wire SIG_CPU_CLKREQ,
  wire AFUR_ABCDxxxx_qn,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,
  wire SIG_IN_CPU_DBUS_FREE
);

void tick_ch2_fast(const GateBoyState& reg_old, GateBoyState& reg_new);
void tick_ch3_fast(const GateBoyState& reg_old, GateBoyState& reg_new, uint8_t* wave_ram);
void tick_ch4_fast(const GateBoyState& reg_old, GateBoyState& reg_new);

sample_t spu_audio_out_r(const GateBoyState& reg_new) {
  auto& spu_new = reg_new.spu;
  sample_t mix = 0;

  if (spu_new.ANEV_NR51_RCH1_ENp.state & 1) mix += ch1_audio_out(reg_new.ch1);
  if (spu_new.BOGU_NR51_RCH2_ENp.state & 1) mix += ch2_audio_out(reg_new.ch2);
  if (spu_new.BAFO_NR51_RCH3_ENp.state & 1) mix += ch3_audio_out(reg_new.ch3);
  if (spu_new.ATUF_NR51_RCH4_ENp.state & 1) mix += ch4_audio_out(reg_new.ch4);


  sample_t vol_r = ((spu_new.BYRE_NR50_VOL_R0.state & 1) << 0) |
                   ((spu_new.BUMO_NR50_VOL_R1.state & 1) << 1) |
                   ((spu_new.COZU_NR50_VOL_R2.state & 1) << 2);
  vol_r += 1;

  sample_t result = mix * vol_r;

  return result;
}

sample_t spu_audio_out_l(const GateBoyState& reg_new) {
  auto& spu_new = reg_new.spu;
  sample_t mix = 0;

  if (spu_new.BUME_NR51_LCH1_ENp.state & 1) mix += ch1_audio_out(reg_new.ch1);
  if (spu_new.BOFA_NR51_LCH2_ENp.state & 1) mix += ch2_audio_out(reg_new.ch2);
  if (spu_new.BEFO_NR51_LCH3_ENp.state & 1) mix += ch3_audio_out(reg_new.ch3);
  if (spu_new.BEPU_NR51_LCH4_ENp.state & 1) mix += ch4_audio_out(reg_new.ch4);

  sample_t vol_l = ((spu_new.APEG_NR50_VOL_L0.state & 1) << 0) |
                   ((spu_new.BYGA_NR50_VOL_L1.state & 1) << 1) |
                   ((spu_new.AGER_NR50_VOL_L2.state & 1) << 2);
  vol_l += 1;

  return mix * vol_l;
}

//-----------------------------------------------------------------------------

void GateBoySPU::reset() {
  CEMO_xBCDExxx.state   = BIT_OLD | BIT_DRIVEN;
  ATEP_AxxDExxH.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  CERY_AxxDExxH.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  ATYK_AxxDExxH.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  AVOK_xBCDExxx.state   = BIT_OLD | BIT_DRIVEN;
  AJER_AxxDExxH.state   = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | BIT_DATA;
  JESO_CLK_512K.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  
  BARA_CLK_512.state  = 0x1b;
  CARU_CLK_256.state  = 0x19;
  BYLU_CLK_128.state  = 0x18;

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

  auto& spu_old = reg_old.spu;
  auto& spu_new = reg_new.spu;
  auto& abus_new = reg_new.cpu_abus;
  auto& dbus_old = reg_old.cpu_dbus;

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(reg_new.cpu_signals.TEDO_CPU_RDp.qp_new());
  /*_p10.BAFU*/ wire BAFU_CPU_WRn = not1(reg_new.cpu_signals.TAPU_CPU_WRp.qp_new());
  /*_p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn);

  memset(&reg_new.wave_dbus, BIT_NEW | BIT_PULLED | 1, sizeof(reg_new.wave_dbus));
  
  /*_p10.DYTE*/ wire DYTE_A00n = not1(abus_new.BUS_CPU_A00p.out_new());
  /*_p10.AFOB*/ wire AFOB_A01n = not1(abus_new.BUS_CPU_A01p.out_new());
  /*_p10.ABUB*/ wire ABUB_A02n = not1(abus_new.BUS_CPU_A02p.out_new());
  /*_p10.ACOL*/ wire ACOL_A03n = not1(abus_new.BUS_CPU_A03p.out_new());
  /*#p10.BOXY*/ wire BOXY_A05n = not1(abus_new.BUS_CPU_A05p.out_new());
  /*_p10.DOSO*/ wire DOSO_A00p = not1(DYTE_A00n);
  /*_p10.DUPA*/ wire DUPA_A01p = not1(AFOB_A01n);
  /*_p10.DENO*/ wire DENO_A02p = not1(ABUB_A02n);

  /*_p10.AWET*/ wire AWET_ADDR_XX2Xn = or4(abus_new.BUS_CPU_A07p.out_new(), abus_new.BUS_CPU_A06p.out_new(), BOXY_A05n, abus_new.BUS_CPU_A04p.out_new());
  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn = not1(abus_new.SYKE_ADDR_HIp_new());
  /*_p10.BEZY*/ wire BEZY_ADDR_FF2Xn = or2(AWET_ADDR_XX2Xn, BAKO_ADDR_FFXXn);
  /*_p10.CONA*/ wire CONA_ADDR_FF2Xp = not1(BEZY_ADDR_FF2Xn);

  /*_p10.DATU*/ wire DATU_ADDR_0100n = nand4(ACOL_A03n, DENO_A02p, AFOB_A01n, DYTE_A00n);
  /*_p10.DURA*/ wire DURA_ADDR_0101n = nand4(ACOL_A03n, DENO_A02p, AFOB_A01n, DOSO_A00p);
  /*#p10.EKAG*/ wire EKAG_ADDR_0110p =  and4(ACOL_A03n, DENO_A02p, DUPA_A01p, DYTE_A00n);

  // The APU reset register _must_ be ticked first.

  /*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp, EKAG_ADDR_0110p); // was this wrong on the schematic?
  /*#p09.HAWU*/ wire HAWU_NR52_WRn = nand2(DOXY_ADDR_FF26p, BOGY_CPU_WRp);
  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*_p09.HAPO*/ wire HAPO_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*_p09.GUFO*/ wire GUFO_SYS_RSTn = not1(HAPO_SYS_RSTp);
  /*#p09.HADA*/ spu_new.HADA_NR52_ALL_SOUND_ON.dff17(HAWU_NR52_WRn, GUFO_SYS_RSTn, dbus_old.BUS_CPU_D07p.out_old()); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.
  /*_p09.JYRO*/ wire JYRO_APU_RSTp = or2(HAPO_SYS_RSTp, reg_new.spu.HADA_NR52_ALL_SOUND_ON.qn_new());
  /*_p09.KUBY*/ wire KUBY_APU_RSTn = not1(JYRO_APU_RSTp);
  /*_p09.KEBA*/ wire KEBA_APU_RSTp = not1(KUBY_APU_RSTn);
  /*_p09.KEPY*/ wire KEPY_APU_RSTn = not1(JYRO_APU_RSTp);

  /*#p09.ETUC*/ wire ETUC_NR52_WRp = and2(BOGY_CPU_WRp, DOXY_ADDR_FF26p);
  /*#p09.FOKU*/ wire FOKU_NR52_WRn = not1(ETUC_NR52_WRp);
  /*#p09.EFOP*/ wire EFOP_NR52_DBG_APUp_old = and2(dbus_old.BUS_CPU_D04p.out_old(), reg_old.cpu_signals.SIG_CPU_UNOR_DBG.qp_old());
  /*#p09.FERO*/ spu_new.FERO_NR52_DBG_APUp.dff9(FOKU_NR52_WRn, KEPY_APU_RSTn, EFOP_NR52_DBG_APUp_old);


  //----------
  // SPU clock dividers

  /*_p01.BOPO*/ wire BOPO_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*_p09.KAME*/ wire KAME_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*_p01.BELA*/ wire BELA_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*_p09.ATYV*/ wire ATYV_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p09.AGUR*/ wire AGUR_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p15.BUWE*/ wire BUWE_APU_RSTn_new = not1(KEBA_APU_RSTp);

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(reg_new.sys_clk.AVET_AxCxExGx.out_new());
  /*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*_p01.ATAG*/ wire ATAG_AxCxExGx = not1(AZOF_AxCxExGx);
  /*_p01.AMUK*/ wire AMUK_xBxDxFxH = not1(ATAG_AxCxExGx);
  /*_p15.AZEG*/ wire AZEG_AxCxExGx = not1(AMUK_xBxDxFxH);
  /*_p01.APUV*/ wire APUV_AxCxExGx = not1(AMUK_xBxDxFxH);
  /*_p01.CYBO*/ wire CYBO_AxCxExGx = not1(AMUK_xBxDxFxH);
  /*_p01.ARYF*/ wire ARYF_AxCxExGx = not1(AMUK_xBxDxFxH);

  /*_p01.CERY*/ spu_new.CERY_AxxDExxH.dff17(CYBO_AxCxExGx, BELA_APU_RSTn_new, spu_old.CERY_AxxDExxH.qn_old());
  /*_p01.ATYK*/ spu_new.ATYK_AxxDExxH.dff17(ARYF_AxCxExGx, BOPO_APU_RSTn_new, spu_old.ATYK_AxxDExxH.qn_old());
  /*_p15.ATEP*/ spu_new.ATEP_AxxDExxH.dff17(AZEG_AxCxExGx, BUWE_APU_RSTn_new, spu_old.ATEP_AxxDExxH.qn_old());
  /*_p09.AJER*/ spu_new.AJER_AxxDExxH.dff17(APUV_AxCxExGx, ATYV_APU_RSTn_new, spu_old.AJER_AxxDExxH.qn_old());

  /*#p01.BATA*/ wire BATA_xBCxxFGx = not1(reg_new.spu.AJER_AxxDExxH.qp_new());
  /*#p09.CALO*/ reg_new.ch1.CALO_xBCDExxx.dff17(BATA_xBCxxFGx, AGUR_APU_RSTn_new, reg_old.ch1.CALO_xBCDExxx.qn_old());

  /*_p01.AVOK*/ spu_new.AVOK_xBCDExxx.dff17(spu_new.ATYK_AxxDExxH.qn_new(),  BOPO_APU_RSTn_new, spu_old.AVOK_xBCDExxx.qn_old());
  /*_p01.BAVU*/ wire BAVU_AxxxxFGH = not1(reg_new.spu.AVOK_xBCDExxx.qp_new());
  /*_p01.JESO*/ spu_new.JESO_CLK_512K.dff17(BAVU_AxxxxFGH, KAME_APU_RSTn_new, spu_old.JESO_CLK_512K.qn_old());


  /*#p15.BUFO*/ wire BUFO = not1(spu_new.ATEP_AxxDExxH.qp_new());
  /*#p15.BYHO*/ wire BYHO_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p15.CEMO*/ spu_new.CEMO_xBCDExxx.dff17(BUFO, BYHO_APU_RSTn_new, spu_old.CEMO_xBCDExxx.qn_old());

  //----------
  // Low-speed clocks are picked up from DIV

  /*_p01.COKE*/ wire COKE_AxxDExxH = not1(spu_new.AJER_AxxDExxH.qn_new());
  /*_p01.UMER*/ wire UMER_DIV10n_old = not1(reg_old.reg_div.TERU_DIV10p.qp_old());

  // FIXME speed up spu clocks for debugging
  //wire UMER_DIV10n_old = not1(reg_old.reg_div.TULU_DIV07p.qp_old());

  /*_p01.ATUS*/ wire ATUS_APU_RSTn_new = not1(KEBA_APU_RSTp);


  /*_p01.BARA*/ spu_new.BARA_CLK_512.dff17(COKE_AxxDExxH,                 ATUS_APU_RSTn_new, UMER_DIV10n_old);
  /*_p01.BURE*/ wire BURE_CLK_512_new = not1(reg_new.spu.BARA_CLK_512.qp_new());

  /*_p01.CARU*/ spu_new.CARU_CLK_256.dff17(BURE_CLK_512_new,              ATUS_APU_RSTn_new, spu_old.CARU_CLK_256.qn_old());
  /*_p01.BYLU*/ spu_new.BYLU_CLK_128.dff17(spu_new.CARU_CLK_256.qn_new(), ATUS_APU_RSTn_new, spu_old.BYLU_CLK_128.qn_old());

  tick_ch1(reg_old, reg_new);
  tick_ch2(reg_old, reg_new);
  tick_ch3(reg_old, reg_new, wave_ram);
  tick_ch4(reg_old, reg_new);

  {
    /*_p10.CAFY*/ wire CAFY_ADDR_FF24p = nor2(BEZY_ADDR_FF2Xn, DATU_ADDR_0100n);
    /*_p09.BOSU*/ wire BOSU_NR50_WRn = nand2(CAFY_ADDR_FF24p, BOGY_CPU_WRp);
    /*_p09.BAXY*/ wire BAXY_NR50_WRp = not1(BOSU_NR50_WRn);
    /*_p09.BOWE*/ wire BOWE_NR50_WRp = not1(BOSU_NR50_WRn);
    /*_p09.BUBU*/ wire BUBU_NR50_WRn = not1(BAXY_NR50_WRp);
    /*_p09.ATAF*/ wire ATAF_NR50_WRn = not1(BOWE_NR50_WRp);

    /*_p09.APEG*/ spu_new.APEG_NR50_VOL_L0   .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D00p.out_old());
    /*_p09.BYGA*/ spu_new.BYGA_NR50_VOL_L1   .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D01p.out_old());
    /*_p09.AGER*/ spu_new.AGER_NR50_VOL_L2   .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D02p.out_old());
    /*_p09.APOS*/ spu_new.APOS_NR50_VIN_TO_L .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D03p.out_old());
    /*_p09.BYRE*/ spu_new.BYRE_NR50_VOL_R0   .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D04p.out_old());
    /*_p09.BUMO*/ spu_new.BUMO_NR50_VOL_R1   .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D05p.out_old());
    /*_p09.COZU*/ spu_new.COZU_NR50_VOL_R2   .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D06p.out_old());
    /*_p09.BEDU*/ spu_new.BEDU_NR50_VIN_TO_R .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D07p.out_old());
  }

  {
    // The die trace for NR51 has the clkp/clkn backwards compared to the other dff9s.

    /*_p10.CORA*/ wire CORA_ADDR_FF25p = nor2(BEZY_ADDR_FF2Xn, DURA_ADDR_0101n);
    /*#p09.BUPO*/ wire BUPO_NR51_WRn = nand2(CORA_ADDR_FF25p, BOGY_CPU_WRp);
    /*#p09.BONO*/ wire BONO_NR51_WRp = not1(BUPO_NR51_WRn);
    /*#p09.BYFA*/ wire BYFA_NR51_WRp = not1(BUPO_NR51_WRn);
    /*#p09.BONO*/ wire BONO_NR51_WRn = not1(BONO_NR51_WRp);
    /*#p09.BYFA*/ wire BYFA_NR51_WRn = not1(BYFA_NR51_WRp);

    /*_p09.ANEV*/ spu_new.ANEV_NR51_RCH1_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D00p.out_old());
    /*_p09.BOGU*/ spu_new.BOGU_NR51_RCH2_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D01p.out_old());
    /*_p09.BAFO*/ spu_new.BAFO_NR51_RCH3_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D02p.out_old());
    /*_p09.ATUF*/ spu_new.ATUF_NR51_RCH4_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D03p.out_old());
    /*_p09.BUME*/ spu_new.BUME_NR51_LCH1_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D04p.out_old());
    /*_p09.BOFA*/ spu_new.BOFA_NR51_LCH2_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D05p.out_old());
    /*_p09.BEFO*/ spu_new.BEFO_NR51_LCH3_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D06p.out_old());
    /*_p09.BEPU*/ spu_new.BEPU_NR51_LCH4_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D07p.out_old());
  }

  {
    ///*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp, EKAG_ADDR_0110p); // was this wrong on the schematic?
    /*#p09.BOPY*/ wire BOPY_NR52_WRn = nand2(BOGY_CPU_WRp, DOXY_ADDR_FF26p);
    /*#p09.BOWY*/ spu_new.BOWY_NR52_DBG_SWEEP.dff17(BOPY_NR52_WRn, KEPY_APU_RSTn, dbus_old.BUS_CPU_D05p.out_old());
  }

  {
    /*_p10.CAFY*/ wire CAFY_ADDR_FF24p = nor2(BEZY_ADDR_FF2Xn, DATU_ADDR_0100n);
    /*_p09.BYMA*/ wire BYMA_ADDR_FF24n = not1(CAFY_ADDR_FF24p);
    /*_p09.BEFU*/ wire BEFU_NR50_RDp = nor2(AGUZ_CPU_RDn, BYMA_ADDR_FF24n);
    /*_p09.ADAK*/ wire ADAK_NR50_RDn = not1(BEFU_NR50_RDp);

    /*_p09.AKOD*/ triwire AKOD_D00 = tri6_nn(ADAK_NR50_RDn, spu_new.APEG_NR50_VOL_L0.qn_new());
    /*_p09.AWED*/ triwire AWED_D01 = tri6_nn(ADAK_NR50_RDn, spu_new.BYGA_NR50_VOL_L1.qn_new());
    /*_p09.AVUD*/ triwire AVUD_D02 = tri6_nn(ADAK_NR50_RDn, spu_new.AGER_NR50_VOL_L2.qn_new());
    /*_p09.AXEM*/ triwire AXEM_D03 = tri6_nn(ADAK_NR50_RDn, spu_new.APOS_NR50_VIN_TO_L.qn_new());
    /*_p09.AMAD*/ triwire AMAD_D04 = tri6_nn(ADAK_NR50_RDn, spu_new.BYRE_NR50_VOL_R0.qn_new());
    /*_p09.ARUX*/ triwire ARUX_D05 = tri6_nn(ADAK_NR50_RDn, spu_new.BUMO_NR50_VOL_R1.qn_new());
    /*_p09.BOCY*/ triwire BOCY_D06 = tri6_nn(ADAK_NR50_RDn, spu_new.COZU_NR50_VOL_R2.qn_new());
    /*_p09.ATUM*/ triwire ATUR_D07 = tri6_nn(ADAK_NR50_RDn, spu_new.BEDU_NR50_VIN_TO_R.qn_new());

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
    /*_p10.CORA*/ wire CORA_ADDR_FF25p = nor2(BEZY_ADDR_FF2Xn, DURA_ADDR_0101n);
    /*_p09.GEPA*/ wire GEPA_ADDR_FF25n = not1(CORA_ADDR_FF25p);
    /*_p09.HEFA*/ wire HEFA_NR51_RDp = nor2(GEPA_ADDR_FF25n, AGUZ_CPU_RDn); // outline wrong color in die
    /*_p09.GUMU*/ wire GUMU_NR51_RDn = not1(HEFA_NR51_RDp);

    /*_p09.BUZU*/ triwire BUZU_D0 = tri6_nn(GUMU_NR51_RDn, spu_new.ANEV_NR51_RCH1_ENp.qn_new()); 
    /*_p09.CAPU*/ triwire CAPU_D1 = tri6_nn(GUMU_NR51_RDn, spu_new.BOGU_NR51_RCH2_ENp.qn_new()); 
    /*_p09.CAGA*/ triwire CAGA_D2 = tri6_nn(GUMU_NR51_RDn, spu_new.BAFO_NR51_RCH3_ENp.qn_new()); 
    /*_p09.BOCA*/ triwire BOCA_D3 = tri6_nn(GUMU_NR51_RDn, spu_new.ATUF_NR51_RCH4_ENp.qn_new()); 
    /*_p09.CAVU*/ triwire CAVU_D4 = tri6_nn(GUMU_NR51_RDn, spu_new.BUME_NR51_LCH1_ENp.qn_new()); 
    /*_p09.CUDU*/ triwire CUDU_D5 = tri6_nn(GUMU_NR51_RDn, spu_new.BOFA_NR51_LCH2_ENp.qn_new()); 
    /*_p09.CADA*/ triwire CADA_D6 = tri6_nn(GUMU_NR51_RDn, spu_new.BEFO_NR51_LCH3_ENp.qn_new()); 
    /*_p09.CERE*/ triwire CERE_D7 = tri6_nn(GUMU_NR51_RDn, spu_new.BEPU_NR51_LCH4_ENp.qn_new()); 

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
    /*_p09.CURU*/ wire CURU_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.KYDU*/ wire KYDU_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.KAZO*/ wire KAZO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.CETO*/ wire CETO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.GAXO*/ wire GAXO_CPU_RDp = not1(AGUZ_CPU_RDn);

    ///*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp, EKAG_ADDR_0110p); // was this wrong on the schematic?
    /*#p09.DOLE*/ wire DOLE_NR52_RDn = nand2(DOXY_ADDR_FF26p, CETO_CPU_RDp);
    /*#p09.DURU*/ wire DURU_NR52_RDn = nand2(DOXY_ADDR_FF26p, CURU_CPU_RDp);
    /*#p09.FEWA*/ wire FEWA_NR52_RDn = nand2(DOXY_ADDR_FF26p, GAXO_CPU_RDp);
    /*#p09.KAMU*/ wire KAMU_NR52_RDn = nand2(DOXY_ADDR_FF26p, KAZO_CPU_RDp);
    /*#p09.JURE*/ wire JURE_NR52_RDn = nand2(KYDU_CPU_RDp, DOXY_ADDR_FF26p);

    /*#p13.CARA*/ wire CARA_CH1_ACTIVEn = not1(reg_new.ch1.CYTO_CH1_ACTIVEp.qp_new());
    /*#p15.DEFU*/ wire DEFU_CH2_ACTIVEn = not1(reg_new.ch2.DANE_CH2_ACTIVEp.qp_new());
    /*#p18.COKA*/ wire COKA_CH3_ACTIVEp = not1(reg_new.ch3.DAVO_CH3_ACTIVEp.qn_any());
    /*#p18.ERED*/ wire ERED_CH3_ACTIVEn = not1(COKA_CH3_ACTIVEp);
    /*#p20.JUWA*/ wire JUWA_CH4_ACTIVEn = not1(reg_new.ch4.GENA_CH4_ACTIVEp.qp_new());

    /*#p09.COTO*/ triwire COTO_D0 = tri6_nn(DOLE_NR52_RDn, CARA_CH1_ACTIVEn);
    /*#p09.EFUS*/ triwire EFUS_D1 = tri6_nn(DURU_NR52_RDn, DEFU_CH2_ACTIVEn);
    /*_p09.FATE*/ triwire FATE_D2 = tri6_nn(FEWA_NR52_RDn, ERED_CH3_ACTIVEn);
    /*_p09.KOGE*/ triwire KOGE_D3 = tri6_nn(KAMU_NR52_RDn, JUWA_CH4_ACTIVEn);
    /*_p09.HOPE*/ triwire HOPE_D7 = tri6_nn(JURE_NR52_RDn, spu_new.HADA_NR52_ALL_SOUND_ON.qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(COTO_D0);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(EFUS_D1);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(FATE_D2);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(KOGE_D3);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(HOPE_D7);
  }
}




















//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


















void tick_spu_fast(
  const GateBoyCpuABus& cpu_abus_new,
  const GateBoyCpuDBus& cpu_dbus_old,
  const GateBoySPU& spu_old,
  const SpuChannel1& ch1_old,
  const SpuChannel2& ch2_old,
  const SpuChannel3& ch3_old,
  const SpuChannel4& ch4_old,

  GateBoyCpuDBus& cpu_dbus_new,
  GateBoyWaveBus& wave_dbus_new,
  GateBoySPU& spu_new,
  SpuChannel1& ch1_new,
  SpuChannel2& ch2_new,
  SpuChannel3& ch3_new,
  SpuChannel4& ch4_new,

  wire SIG_CPU_CLKREQ,
  wire SIG_IN_CPU_DBUS_FREE,
  wire SIG_CPU_UNOR_DBG_old,
  wire TERU_DIV10p_old,

  wire AFER_SYS_RSTp,
  wire ASOL_POR_DONEn,

  wire AVET_AxCxExGx,
  wire AFUR_ABCDxxxx_qn,

  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  uint8_t* wave_ram
)
{
  auto AVOR_SYS_RSTp = or2(AFER_SYS_RSTp, ASOL_POR_DONEn);

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(TEDO_CPU_RDp);
  /*_p10.BAFU*/ wire BAFU_CPU_WRn = not1(TAPU_CPU_WRp);
  /*_p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn);

  memset(&wave_dbus_new, BIT_NEW | BIT_PULLED | 1, sizeof(wave_dbus_new));

  /*_p10.DYTE*/ wire DYTE_A00n = not1(cpu_abus_new.BUS_CPU_A00p.out_any());
  /*_p10.AFOB*/ wire AFOB_A01n = not1(cpu_abus_new.BUS_CPU_A01p.out_any());
  /*_p10.ABUB*/ wire ABUB_A02n = not1(cpu_abus_new.BUS_CPU_A02p.out_any());
  /*_p10.ACOL*/ wire ACOL_A03n = not1(cpu_abus_new.BUS_CPU_A03p.out_any());
  /*#p10.BOXY*/ wire BOXY_A05n = not1(cpu_abus_new.BUS_CPU_A05p.out_any());
  /*_p10.DOSO*/ wire DOSO_A00p = not1(DYTE_A00n);
  /*_p10.DUPA*/ wire DUPA_A01p = not1(AFOB_A01n);
  /*_p10.DENO*/ wire DENO_A02p = not1(ABUB_A02n);

  /*_p10.AWET*/ wire AWET_ADDR_XX2Xn = or4(cpu_abus_new.BUS_CPU_A07p.out_any(), cpu_abus_new.BUS_CPU_A06p.out_any(), BOXY_A05n, cpu_abus_new.BUS_CPU_A04p.out_any());
  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn = not1(cpu_abus_new.SYKE_ADDR_HIp_any());
  /*_p10.BEZY*/ wire BEZY_ADDR_FF2Xn = or2(AWET_ADDR_XX2Xn, BAKO_ADDR_FFXXn);
  /*_p10.CONA*/ wire CONA_ADDR_FF2Xp = not1(BEZY_ADDR_FF2Xn);

  /*_p10.DATU*/ wire DATU_ADDR_0100n = nand4(ACOL_A03n, DENO_A02p, AFOB_A01n, DYTE_A00n);
  /*_p10.DURA*/ wire DURA_ADDR_0101n = nand4(ACOL_A03n, DENO_A02p, AFOB_A01n, DOSO_A00p);
  /*#p10.EKAG*/ wire EKAG_ADDR_0110p = and4(ACOL_A03n, DENO_A02p, DUPA_A01p, DYTE_A00n);

  // The APU reset register _must_ be ticked first.

  /*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp, EKAG_ADDR_0110p); // was this wrong on the schematic?
  /*#p09.HAWU*/ wire HAWU_NR52_WRn = nand2(DOXY_ADDR_FF26p, BOGY_CPU_WRp);
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*_p09.HAPO*/ wire HAPO_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*_p09.GUFO*/ wire GUFO_SYS_RSTn = not1(HAPO_SYS_RSTp);
  /*#p09.HADA*/ spu_new.HADA_NR52_ALL_SOUND_ON.dff17(HAWU_NR52_WRn, GUFO_SYS_RSTn, cpu_dbus_old.BUS_CPU_D07p.out_any()); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.
  /*_p09.JYRO*/ wire JYRO_APU_RSTp = or2(HAPO_SYS_RSTp, spu_new.HADA_NR52_ALL_SOUND_ON.qn_any());
  /*_p09.KUBY*/ wire KUBY_APU_RSTn = not1(JYRO_APU_RSTp);
  /*_p09.KEBA*/ wire KEBA_APU_RSTp = not1(KUBY_APU_RSTn);
  /*_p09.KEPY*/ wire KEPY_APU_RSTn = not1(JYRO_APU_RSTp);

  /*#p09.ETUC*/ wire ETUC_NR52_WRp = and2(BOGY_CPU_WRp, DOXY_ADDR_FF26p);
  /*#p09.FOKU*/ wire FOKU_NR52_WRn = not1(ETUC_NR52_WRp);
  /*#p09.EFOP*/ wire EFOP_NR52_DBG_APUp_old = and2(cpu_dbus_old.BUS_CPU_D04p.out_any(), SIG_CPU_UNOR_DBG_old);
  /*#p09.FERO*/ spu_new.FERO_NR52_DBG_APUp.dff9(FOKU_NR52_WRn, KEPY_APU_RSTn, EFOP_NR52_DBG_APUp_old);

  //----------
  // SPU clock dividers

  /*_p01.BOPO*/ wire BOPO_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*_p09.KAME*/ wire KAME_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*_p01.BELA*/ wire BELA_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*_p09.ATYV*/ wire ATYV_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p09.AGUR*/ wire AGUR_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p15.BUWE*/ wire BUWE_APU_RSTn_new = not1(KEBA_APU_RSTp);

  /*_p01.ATAL*/ wire ATAL_xBxDxFxH = not1(AVET_AxCxExGx);
  /*_p01.AZOF*/ wire AZOF_AxCxExGx = not1(ATAL_xBxDxFxH);
  /*_p01.ATAG*/ wire ATAG_AxCxExGx = not1(AZOF_AxCxExGx);
  /*_p01.AMUK*/ wire AMUK_xBxDxFxH = not1(ATAG_AxCxExGx);
  /*_p15.AZEG*/ wire AZEG_AxCxExGx = not1(AMUK_xBxDxFxH);
  /*_p01.APUV*/ wire APUV_AxCxExGx = not1(AMUK_xBxDxFxH);
  /*_p01.CYBO*/ wire CYBO_AxCxExGx = not1(AMUK_xBxDxFxH);
  /*_p01.ARYF*/ wire ARYF_AxCxExGx = not1(AMUK_xBxDxFxH);

  /*_p01.CERY*/ spu_new.CERY_AxxDExxH.dff17(CYBO_AxCxExGx, BELA_APU_RSTn_new, spu_old.CERY_AxxDExxH.qn_any());
  /*_p01.ATYK*/ spu_new.ATYK_AxxDExxH.dff17(ARYF_AxCxExGx, BOPO_APU_RSTn_new, spu_old.ATYK_AxxDExxH.qn_any());
  /*_p15.ATEP*/ spu_new.ATEP_AxxDExxH.dff17(AZEG_AxCxExGx, BUWE_APU_RSTn_new, spu_old.ATEP_AxxDExxH.qn_any());
  /*_p09.AJER*/ spu_new.AJER_AxxDExxH.dff17(APUV_AxCxExGx, ATYV_APU_RSTn_new, spu_old.AJER_AxxDExxH.qn_any());

  /*#p01.BATA*/ wire BATA_xBCxxFGx = not1(spu_new.AJER_AxxDExxH.qn_any());
  /*#p09.CALO*/ ch1_new.CALO_xBCDExxx.dff17(BATA_xBCxxFGx, AGUR_APU_RSTn_new, ch1_old.CALO_xBCDExxx.qn_any());

  /*_p01.AVOK*/ spu_new.AVOK_xBCDExxx.dff17(spu_new.ATYK_AxxDExxH.qn_any(), BOPO_APU_RSTn_new, spu_old.AVOK_xBCDExxx.qn_any());
  /*_p01.BAVU*/ wire BAVU_AxxxxFGH = not1(spu_new.AVOK_xBCDExxx.qp_any());
  /*_p01.JESO*/ spu_new.JESO_CLK_512K.dff17(BAVU_AxxxxFGH, KAME_APU_RSTn_new, spu_old.JESO_CLK_512K.qn_any());


  /*#p15.BUFO*/ wire BUFO = not1(spu_new.ATEP_AxxDExxH.qp_any());
  /*#p15.BYHO*/ wire BYHO_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p15.CEMO*/ spu_new.CEMO_xBCDExxx.dff17(BUFO, BYHO_APU_RSTn_new, spu_old.CEMO_xBCDExxx.qn_any());

  //----------
  // Low-speed clocks are picked up from DIV

  /*_p01.COKE*/ wire COKE_AxxDExxH = not1(spu_new.AJER_AxxDExxH.qn_any());
  /*_p01.UMER*/ wire UMER_DIV10n_old = not1(TERU_DIV10p_old);

  // FIXME speed up spu clocks for debugging
  //wire UMER_DIV10n_old = not1(reg_old.reg_div.TULU_DIV07p.qp_any());

  /*_p01.ATUS*/ wire ATUS_APU_RSTn_new = not1(KEBA_APU_RSTp);


  /*_p01.BARA*/ spu_new.BARA_CLK_512.dff17(COKE_AxxDExxH, ATUS_APU_RSTn_new, UMER_DIV10n_old);
  /*_p01.BURE*/ wire BURE_CLK_512_new = not1(spu_new.BARA_CLK_512.qp_any());

  /*_p01.CARU*/ spu_new.CARU_CLK_256.dff17(BURE_CLK_512_new, ATUS_APU_RSTn_new, spu_old.CARU_CLK_256.qn_any());
  /*_p01.BYLU*/ spu_new.BYLU_CLK_128.dff17(spu_new.CARU_CLK_256.qn_any(), ATUS_APU_RSTn_new, spu_old.BYLU_CLK_128.qn_any());


  tick_ch1_fast(
    cpu_dbus_old,
    spu_old,
    ch1_old,

    cpu_abus_new,
    cpu_dbus_new,
    spu_new,
    ch1_new,

    AFER_SYS_RSTp,
    ASOL_POR_DONEn,
    SIG_CPU_CLKREQ,
    AFUR_ABCDxxxx_qn,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    SIG_IN_CPU_DBUS_FREE
  );


  /*
  tick_ch1_fast(_reg_old, _reg_new);
  tick_ch2_fast(_reg_old, _reg_new);
  tick_ch3_fast(_reg_old, _reg_new, wave_ram);
  tick_ch4_fast(_reg_old, _reg_new);
  */

  {
    /*_p10.CAFY*/ wire CAFY_ADDR_FF24p = nor2(BEZY_ADDR_FF2Xn, DATU_ADDR_0100n);
    /*_p09.BOSU*/ wire BOSU_NR50_WRn = nand2(CAFY_ADDR_FF24p, BOGY_CPU_WRp);
    /*_p09.BAXY*/ wire BAXY_NR50_WRp = not1(BOSU_NR50_WRn);
    /*_p09.BOWE*/ wire BOWE_NR50_WRp = not1(BOSU_NR50_WRn);
    /*_p09.BUBU*/ wire BUBU_NR50_WRn = not1(BAXY_NR50_WRp);
    /*_p09.ATAF*/ wire ATAF_NR50_WRn = not1(BOWE_NR50_WRp);

    /*_p09.APEG*/ spu_new.APEG_NR50_VOL_L0  .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D00p.out_any());
    /*_p09.BYGA*/ spu_new.BYGA_NR50_VOL_L1  .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D01p.out_any());
    /*_p09.AGER*/ spu_new.AGER_NR50_VOL_L2  .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D02p.out_any());
    /*_p09.APOS*/ spu_new.APOS_NR50_VIN_TO_L.dff9(ATAF_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D03p.out_any());
    /*_p09.BYRE*/ spu_new.BYRE_NR50_VOL_R0  .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D04p.out_any());
    /*_p09.BUMO*/ spu_new.BUMO_NR50_VOL_R1  .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D05p.out_any());
    /*_p09.COZU*/ spu_new.COZU_NR50_VOL_R2  .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D06p.out_any());
    /*_p09.BEDU*/ spu_new.BEDU_NR50_VIN_TO_R.dff9(BUBU_NR50_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D07p.out_any());
  }

  {
    // The die trace for NR51 has the clkp/clkn backwards compared to the other dff9s.

    /*_p10.CORA*/ wire CORA_ADDR_FF25p = nor2(BEZY_ADDR_FF2Xn, DURA_ADDR_0101n);
    /*#p09.BUPO*/ wire BUPO_NR51_WRn = nand2(CORA_ADDR_FF25p, BOGY_CPU_WRp);
    /*#p09.BONO*/ wire BONO_NR51_WRp = not1(BUPO_NR51_WRn);
    /*#p09.BYFA*/ wire BYFA_NR51_WRp = not1(BUPO_NR51_WRn);
    /*#p09.BONO*/ wire BONO_NR51_WRn = not1(BONO_NR51_WRp);
    /*#p09.BYFA*/ wire BYFA_NR51_WRn = not1(BYFA_NR51_WRp);

    /*_p09.ANEV*/ spu_new.ANEV_NR51_RCH1_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D00p.out_any());
    /*_p09.BOGU*/ spu_new.BOGU_NR51_RCH2_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D01p.out_any());
    /*_p09.BAFO*/ spu_new.BAFO_NR51_RCH3_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D02p.out_any());
    /*_p09.ATUF*/ spu_new.ATUF_NR51_RCH4_ENp.dff9(BONO_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D03p.out_any());
    /*_p09.BUME*/ spu_new.BUME_NR51_LCH1_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D04p.out_any());
    /*_p09.BOFA*/ spu_new.BOFA_NR51_LCH2_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D05p.out_any());
    /*_p09.BEFO*/ spu_new.BEFO_NR51_LCH3_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D06p.out_any());
    /*_p09.BEPU*/ spu_new.BEPU_NR51_LCH4_ENp.dff9(BYFA_NR51_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D07p.out_any());
  }

  {
    ///*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp, EKAG_ADDR_0110p); // was this wrong on the schematic?
    /*#p09.BOPY*/ wire BOPY_NR52_WRn = nand2(BOGY_CPU_WRp, DOXY_ADDR_FF26p);
    /*#p09.BOWY*/ spu_new.BOWY_NR52_DBG_SWEEP.dff17(BOPY_NR52_WRn, KEPY_APU_RSTn, cpu_dbus_old.BUS_CPU_D05p.out_any());
  }

  {
    /*_p10.CAFY*/ wire CAFY_ADDR_FF24p = nor2(BEZY_ADDR_FF2Xn, DATU_ADDR_0100n);
    /*_p09.BYMA*/ wire BYMA_ADDR_FF24n = not1(CAFY_ADDR_FF24p);
    /*_p09.BEFU*/ wire BEFU_NR50_RDp = nor2(AGUZ_CPU_RDn, BYMA_ADDR_FF24n);
    /*_p09.ADAK*/ wire ADAK_NR50_RDn = not1(BEFU_NR50_RDp);

    /*_p09.AKOD*/ triwire AKOD_D00 = tri6_nn(ADAK_NR50_RDn, spu_new.APEG_NR50_VOL_L0.qn_any());
    /*_p09.AWED*/ triwire AWED_D01 = tri6_nn(ADAK_NR50_RDn, spu_new.BYGA_NR50_VOL_L1.qn_any());
    /*_p09.AVUD*/ triwire AVUD_D02 = tri6_nn(ADAK_NR50_RDn, spu_new.AGER_NR50_VOL_L2.qn_any());
    /*_p09.AXEM*/ triwire AXEM_D03 = tri6_nn(ADAK_NR50_RDn, spu_new.APOS_NR50_VIN_TO_L.qn_any());
    /*_p09.AMAD*/ triwire AMAD_D04 = tri6_nn(ADAK_NR50_RDn, spu_new.BYRE_NR50_VOL_R0.qn_any());
    /*_p09.ARUX*/ triwire ARUX_D05 = tri6_nn(ADAK_NR50_RDn, spu_new.BUMO_NR50_VOL_R1.qn_any());
    /*_p09.BOCY*/ triwire BOCY_D06 = tri6_nn(ADAK_NR50_RDn, spu_new.COZU_NR50_VOL_R2.qn_any());
    /*_p09.ATUM*/ triwire ATUR_D07 = tri6_nn(ADAK_NR50_RDn, spu_new.BEDU_NR50_VIN_TO_R.qn_any());

    /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(AKOD_D00);
    /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(AWED_D01);
    /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(AVUD_D02);
    /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(AXEM_D03);
    /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(AMAD_D04);
    /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(ARUX_D05);
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(BOCY_D06);
    /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(ATUR_D07);
  }

  {
    /*_p10.CORA*/ wire CORA_ADDR_FF25p = nor2(BEZY_ADDR_FF2Xn, DURA_ADDR_0101n);
    /*_p09.GEPA*/ wire GEPA_ADDR_FF25n = not1(CORA_ADDR_FF25p);
    /*_p09.HEFA*/ wire HEFA_NR51_RDp = nor2(GEPA_ADDR_FF25n, AGUZ_CPU_RDn); // outline wrong color in die
    /*_p09.GUMU*/ wire GUMU_NR51_RDn = not1(HEFA_NR51_RDp);

    /*_p09.BUZU*/ triwire BUZU_D0 = tri6_nn(GUMU_NR51_RDn, spu_new.ANEV_NR51_RCH1_ENp.qn_any());
    /*_p09.CAPU*/ triwire CAPU_D1 = tri6_nn(GUMU_NR51_RDn, spu_new.BOGU_NR51_RCH2_ENp.qn_any());
    /*_p09.CAGA*/ triwire CAGA_D2 = tri6_nn(GUMU_NR51_RDn, spu_new.BAFO_NR51_RCH3_ENp.qn_any());
    /*_p09.BOCA*/ triwire BOCA_D3 = tri6_nn(GUMU_NR51_RDn, spu_new.ATUF_NR51_RCH4_ENp.qn_any());
    /*_p09.CAVU*/ triwire CAVU_D4 = tri6_nn(GUMU_NR51_RDn, spu_new.BUME_NR51_LCH1_ENp.qn_any());
    /*_p09.CUDU*/ triwire CUDU_D5 = tri6_nn(GUMU_NR51_RDn, spu_new.BOFA_NR51_LCH2_ENp.qn_any());
    /*_p09.CADA*/ triwire CADA_D6 = tri6_nn(GUMU_NR51_RDn, spu_new.BEFO_NR51_LCH3_ENp.qn_any());
    /*_p09.CERE*/ triwire CERE_D7 = tri6_nn(GUMU_NR51_RDn, spu_new.BEPU_NR51_LCH4_ENp.qn_any());

    /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(BUZU_D0);
    /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(CAPU_D1);
    /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(CAGA_D2);
    /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(BOCA_D3);
    /*_BUS_CPU_D04p*/ cpu_dbus_new.BUS_CPU_D04p.tri_bus(CAVU_D4);
    /*_BUS_CPU_D05p*/ cpu_dbus_new.BUS_CPU_D05p.tri_bus(CUDU_D5);
    /*_BUS_CPU_D06p*/ cpu_dbus_new.BUS_CPU_D06p.tri_bus(CADA_D6);
    /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(CERE_D7);
  }

  {
    /*_p09.CURU*/ wire CURU_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.KYDU*/ wire KYDU_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.KAZO*/ wire KAZO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.CETO*/ wire CETO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p09.GAXO*/ wire GAXO_CPU_RDp = not1(AGUZ_CPU_RDn);

    ///*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp, EKAG_ADDR_0110p); // was this wrong on the schematic?
    /*#p09.DOLE*/ wire DOLE_NR52_RDn = nand2(DOXY_ADDR_FF26p, CETO_CPU_RDp);
    /*#p09.DURU*/ wire DURU_NR52_RDn = nand2(DOXY_ADDR_FF26p, CURU_CPU_RDp);
    /*#p09.FEWA*/ wire FEWA_NR52_RDn = nand2(DOXY_ADDR_FF26p, GAXO_CPU_RDp);
    /*#p09.KAMU*/ wire KAMU_NR52_RDn = nand2(DOXY_ADDR_FF26p, KAZO_CPU_RDp);
    /*#p09.JURE*/ wire JURE_NR52_RDn = nand2(KYDU_CPU_RDp, DOXY_ADDR_FF26p);

    /*#p13.CARA*/ wire CARA_CH1_ACTIVEn = not1(ch1_new.CYTO_CH1_ACTIVEp.qp_any());
    /*#p15.DEFU*/ wire DEFU_CH2_ACTIVEn = not1(ch2_new.DANE_CH2_ACTIVEp.qp_any());
    /*#p18.COKA*/ wire COKA_CH3_ACTIVEp = not1(ch3_new.DAVO_CH3_ACTIVEp.qn_any());
    /*#p18.ERED*/ wire ERED_CH3_ACTIVEn = not1(COKA_CH3_ACTIVEp);
    /*#p20.JUWA*/ wire JUWA_CH4_ACTIVEn = not1(ch4_new.GENA_CH4_ACTIVEp.qp_any());

    /*#p09.COTO*/ triwire COTO_D0 = tri6_nn(DOLE_NR52_RDn, CARA_CH1_ACTIVEn);
    /*#p09.EFUS*/ triwire EFUS_D1 = tri6_nn(DURU_NR52_RDn, DEFU_CH2_ACTIVEn);
    /*_p09.FATE*/ triwire FATE_D2 = tri6_nn(FEWA_NR52_RDn, ERED_CH3_ACTIVEn);
    /*_p09.KOGE*/ triwire KOGE_D3 = tri6_nn(KAMU_NR52_RDn, JUWA_CH4_ACTIVEn);
    /*_p09.HOPE*/ triwire HOPE_D7 = tri6_nn(JURE_NR52_RDn, spu_new.HADA_NR52_ALL_SOUND_ON.qn_any());

    /*_BUS_CPU_D00p*/ cpu_dbus_new.BUS_CPU_D00p.tri_bus(COTO_D0);
    /*_BUS_CPU_D01p*/ cpu_dbus_new.BUS_CPU_D01p.tri_bus(EFUS_D1);
    /*_BUS_CPU_D02p*/ cpu_dbus_new.BUS_CPU_D02p.tri_bus(FATE_D2);
    /*_BUS_CPU_D03p*/ cpu_dbus_new.BUS_CPU_D03p.tri_bus(KOGE_D3);
    /*_BUS_CPU_D07p*/ cpu_dbus_new.BUS_CPU_D07p.tri_bus(HOPE_D7);
  }
}

#endif
