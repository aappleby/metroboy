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
  int64_t& spu_phase_new,

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

  memset(&wave_dbus_new, BIT_NEW | BIT_PULLED | 1, sizeof(wave_dbus_new));

  if (bit(or2(AFER_SYS_RSTp, ASOL_POR_DONEn))) {
    spu_new.HADA_NR52_ALL_SOUND_ON.state = 0;
  }
  else {
    if (DELTA_GH && SIG_IN_CPU_WRp && SIG_IN_CPU_DBUS_FREE) {
      if (addr == 0xFF26) {
        spu_new.HADA_NR52_ALL_SOUND_ON.state = cpu_dbus_old.BUS_CPU_D07p.out_any();
      }
    }
  }

  bool apu_rst = bit(or3(AFER_SYS_RSTp, ASOL_POR_DONEn, spu_new.HADA_NR52_ALL_SOUND_ON.qn_any()));

  if (apu_rst) {
    spu_phase_new = -1;
  }
  else {
    spu_phase_new++;
  }

  //----------
  // SPU clock dividers

  /*_p01.CERY*/ spu_new.CERY_AxxDExxH.dff17(not1(AVET_AxCxExGx), !apu_rst, spu_old.CERY_AxxDExxH.qn_any());
  /*_p01.ATYK*/ spu_new.ATYK_AxxDExxH.dff17(not1(AVET_AxCxExGx), !apu_rst, spu_old.ATYK_AxxDExxH.qn_any());
  /*_p15.ATEP*/ spu_new.ATEP_AxxDExxH.dff17(not1(AVET_AxCxExGx), !apu_rst, spu_old.ATEP_AxxDExxH.qn_any());
  /*_p09.AJER*/ spu_new.AJER_AxxDExxH.dff17(not1(AVET_AxCxExGx), !apu_rst, spu_old.AJER_AxxDExxH.qn_any());

  /*#p01.AVOK*/ spu_new.AVOK_xBCDExxx.dff17( spu_new.ATYK_AxxDExxH.qn_any(), !apu_rst, spu_old.AVOK_xBCDExxx.qn_any());
  /*#p01.JESO*/ spu_new.JESO_CLK_512K.dff17(~spu_new.AVOK_xBCDExxx.qp_any(), !apu_rst, spu_old.JESO_CLK_512K.qn_any());
  /*#p15.CEMO*/ spu_new.CEMO_xBCDExxx.dff17(~spu_new.ATEP_AxxDExxH.qp_any(), !apu_rst, spu_old.CEMO_xBCDExxx.qn_any());
  /*_p01.BARA*/ spu_new.BARA_CLK_512 .dff17( spu_new.AJER_AxxDExxH.qp_any(), !apu_rst, not1(TERU_DIV10p_old));
  /*_p01.CARU*/ spu_new.CARU_CLK_256 .dff17(~spu_new.BARA_CLK_512.qp_any(),  !apu_rst, spu_old.CARU_CLK_256.qn_any());
  /*_p01.BYLU*/ spu_new.BYLU_CLK_128 .dff17( spu_new.CARU_CLK_256.qn_any(),  !apu_rst, spu_old.BYLU_CLK_128.qn_any());


  tick_ch1_fast(
    phase_new,
    spu_phase_new,

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
    spu_phase_new,

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
    spu_phase_new,

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
    spu_phase_new,

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
    spu_new.FERO_NR52_DBG_APUp.state = 0;
  }
  else {
    if (DELTA_GH && SIG_IN_CPU_WRp && SIG_IN_CPU_DBUS_FREE) {
      if (addr == 0xFF24) bit_unpack(&spu_new.APEG_NR50_VOL_L0, 8, dbus_old);
      if (addr == 0xFF25) bit_unpack(&spu_new.ANEV_NR51_RCH1_ENp, 8, dbus_old);
      if (addr == 0xFF26) {
        spu_new.FERO_NR52_DBG_APUp.state = bit(and2(cpu_dbus_old.BUS_CPU_D04p.out_any(), SIG_CPU_UNOR_DBG_old));
        spu_new.BOWY_NR52_DBG_SWEEP.state = bit(dbus_old, 5);
      }
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
