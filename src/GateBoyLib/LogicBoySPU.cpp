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

#define DELTA_AB (phase_new == 1)
#define DELTA_BC (phase_new == 2)
#define DELTA_CD (phase_new == 3)
#define DELTA_DE (phase_new == 4)
#define DELTA_EF (phase_new == 5)
#define DELTA_FG (phase_new == 6)
#define DELTA_GH (phase_new == 7)
#define DELTA_HA (phase_new == 0)

//------------------------------------------------------------------------------

void tick_spu_fast(
  uint64_t phase_new,

  const GateBoyCpuABus& cpu_abus_new,
  const GateBoyCpuDBus& cpu_dbus_old,
  const GateBoyWaveBus& wave_dbus_old,
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
  wire ALEF_xBCDExxx,
  wire APUK_xxCDEFxx_qn,

  wire SIG_IN_CPU_RDp,
  wire SIG_IN_CPU_WRp,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  uint8_t* wave_ram
)
{
 
  auto addr = bit_pack(cpu_abus_new);
  auto dbus_old = bit_pack(cpu_dbus_old);

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
  /*#p10.EKAG*/ wire EKAG_ADDR_0110p =  and4(ACOL_A03n, DENO_A02p, DUPA_A01p, DYTE_A00n);

  // The APU reset register _must_ be ticked first.

  /*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp, EKAG_ADDR_0110p); // was this wrong on the schematic?
  /*#p09.HAWU*/ wire HAWU_NR52_WRn = nand2(DOXY_ADDR_FF26p, BOGY_CPU_WRp);
  /*#p01.AVOR*/ wire AVOR_SYS_RSTp = or2(AFER_SYS_RSTp, ASOL_POR_DONEn);
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*_p09.HAPO*/ wire HAPO_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*#p09.HADA*/ spu_new.HADA_NR52_ALL_SOUND_ON.dff17(HAWU_NR52_WRn, nor2(AFER_SYS_RSTp, ASOL_POR_DONEn), cpu_dbus_old.BUS_CPU_D07p.out_any()); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.

  /*_p09.JYRO*/ wire apu_rst = bit(or3(AFER_SYS_RSTp, ASOL_POR_DONEn, spu_new.HADA_NR52_ALL_SOUND_ON.qn_any()));

  /*_p09.JYRO*/ wire JYRO_APU_RSTp = or2(HAPO_SYS_RSTp, spu_new.HADA_NR52_ALL_SOUND_ON.qn_any());
  /*_p09.KUBY*/ wire KUBY_APU_RSTn = not1(JYRO_APU_RSTp);
  /*_p09.KEBA*/ wire KEBA_APU_RSTp = not1(KUBY_APU_RSTn);
  /*_p09.KEPY*/ wire KEPY_APU_RSTn = not1(JYRO_APU_RSTp);

  /*#p09.ETUC*/ wire ETUC_NR52_WRp = and2(BOGY_CPU_WRp, DOXY_ADDR_FF26p);
  /*#p09.FOKU*/ wire FOKU_NR52_WRn = not1(ETUC_NR52_WRp);
  /*#p09.EFOP*/ wire EFOP_NR52_DBG_APUp_old = and2(cpu_dbus_old.BUS_CPU_D04p.out_any(), SIG_CPU_UNOR_DBG_old);
  /*#p09.FERO*/ spu_new.FERO_NR52_DBG_APUp.dff9(FOKU_NR52_WRn, !apu_rst, EFOP_NR52_DBG_APUp_old);


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

  /*_p01.CERY*/ spu_new.CERY_AxxDExxH.dff17(CYBO_AxCxExGx, !apu_rst, spu_old.CERY_AxxDExxH.qn_any());
  /*_p01.ATYK*/ spu_new.ATYK_AxxDExxH.dff17(ARYF_AxCxExGx, !apu_rst, spu_old.ATYK_AxxDExxH.qn_any());
  /*_p15.ATEP*/ spu_new.ATEP_AxxDExxH.dff17(AZEG_AxCxExGx, !apu_rst, spu_old.ATEP_AxxDExxH.qn_any());
  /*_p09.AJER*/ spu_new.AJER_AxxDExxH.dff17(APUV_AxCxExGx, !apu_rst, spu_old.AJER_AxxDExxH.qn_any());

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
    phase_new,
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

    SIG_IN_CPU_RDp,
    SIG_IN_CPU_WRp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,

    SIG_IN_CPU_DBUS_FREE
  );

  tick_ch2_fast(
    phase_new,
    cpu_dbus_old,
    spu_old,
    ch2_old,
    cpu_abus_new,

    cpu_dbus_new,
    spu_new,
    ch2_new,

    AFER_SYS_RSTp,
    ASOL_POR_DONEn,
    SIG_CPU_CLKREQ,
    AFUR_ABCDxxxx_qn,
    SIG_IN_CPU_RDp,
    SIG_IN_CPU_WRp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    SIG_IN_CPU_DBUS_FREE
  );

  tick_ch3_fast(
    phase_new,
    cpu_dbus_old,
    spu_old,
    ch3_old,
    wave_dbus_old,
    cpu_abus_new,

    cpu_dbus_new,
    spu_new,
    ch3_new,
    wave_dbus_new,

    AFER_SYS_RSTp,
    ASOL_POR_DONEn,
    SIG_CPU_CLKREQ,
    AVET_AxCxExGx,
    AFUR_ABCDxxxx_qn,
    APUK_xxCDEFxx_qn,
    ALEF_xBCDExxx,
    SIG_IN_CPU_RDp,
    SIG_IN_CPU_WRp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    SIG_IN_CPU_DBUS_FREE,

    wave_ram
  );

  tick_ch4_fast(
    phase_new,
    cpu_dbus_old,
    spu_old,
    ch4_old,
    wave_dbus_old,

    cpu_abus_new,
    cpu_dbus_new,
    spu_new,
    ch4_new,
    wave_dbus_new,

    AFER_SYS_RSTp,
    ASOL_POR_DONEn,
    SIG_CPU_CLKREQ,
    AVET_AxCxExGx,
    AFUR_ABCDxxxx_qn,
    APUK_xxCDEFxx_qn,
    ALEF_xBCDExxx,
    SIG_IN_CPU_RDp,
    SIG_IN_CPU_WRp,
    TEDO_CPU_RDp,
    TAPU_CPU_WRp,
    SIG_IN_CPU_DBUS_FREE,

    wave_ram
  );

  if (apu_rst) {
    bit_unpack(&spu_new.APEG_NR50_VOL_L0, 8, 0);
    bit_unpack(&spu_new.ANEV_NR51_RCH1_ENp, 8, 0);
    spu_new.BOWY_NR52_DBG_SWEEP.state = 0;
  }
  else {
    if (DELTA_GH && SIG_IN_CPU_WRp && SIG_IN_CPU_DBUS_FREE) {
      if (addr == 0xFF24) bit_unpack(&spu_new.APEG_NR50_VOL_L0, 8, dbus_old);
      if (addr == 0xFF25) bit_unpack(&spu_new.ANEV_NR51_RCH1_ENp, 8, dbus_old);
      if (addr == 0xFF26) spu_new.BOWY_NR52_DBG_SWEEP.state = bit(dbus_old, 5);
    }
  }

  {
    if (addr == 0xFF24 && SIG_IN_CPU_RDp) bit_unpack(cpu_dbus_new, bit_pack(&spu_new.APEG_NR50_VOL_L0, 8));
    if (addr == 0xFF25 && SIG_IN_CPU_RDp) bit_unpack(cpu_dbus_new, bit_pack(&spu_new.ANEV_NR51_RCH1_ENp, 8));
    
    if (addr == 0xFF26 && SIG_IN_CPU_RDp) {
      auto dbus_new = pack(
        ch1_new.CYTO_CH1_ACTIVEp.qp_any(),
        ch2_new.DANE_CH2_ACTIVEp.qp_any(),
        ch3_new.DAVO_CH3_ACTIVEp.qp_any(),
        ch4_new.GENA_CH4_ACTIVEp.qp_any(),
        1, 1, 1,
        spu_new.HADA_NR52_ALL_SOUND_ON.qp_any()
      );

      bit_unpack(&cpu_dbus_new, 8, dbus_new);
    }
  }
}

#endif
