#include "GateBoySPU.h"

#include "GateBoy.h"

#ifdef SIM_AUDIO

#define DELTA_AB ((phase_new & 7) == 1)
#define DELTA_BC ((phase_new & 7) == 2)
#define DELTA_CD ((phase_new & 7) == 3)
#define DELTA_DE ((phase_new & 7) == 4)
#define DELTA_EF ((phase_new & 7) == 5)
#define DELTA_FG ((phase_new & 7) == 6)
#define DELTA_GH ((phase_new & 7) == 7)
#define DELTA_HA ((phase_new & 7) == 0)

sample_t ch4_audio_out_fast(const SpuChannel4& ch4) {

  ///*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  ///*_p20.DYRY*/ wire DYRY_DBG = and2(ch4.CUNY_NR44_LEN_ENp.qp_old(), EDEK_NR52_DBG_APUp);
  /*_p20.DYRY*/ wire DYRY_DBG = 0;

  /*_p20.GAME*/ wire GAME_LFSR_OUT = and2(ch4.GENA_CH4_ACTIVEp.qp_old(), ch4.HEZU_LFSR_15.qp_old());

  // ignoring debug thing on the audio out here
  wire FEME_LFSR_CLKp_new = 0;

  /*_p20.EZUL*/ wire EZUL_CH4_BIT_MUX = mux2p(DYRY_DBG, FEME_LFSR_CLKp_new, GAME_LFSR_OUT);

  ///*_p20.CEPY*/ wire CEPY_NR44_STOPn = not1(ch4.CUNY_NR44_LEN_ENp.qp_old());
  ///*_p20.COTE*/ wire COTE_DBG_CH4_MUTE = and2(CEPY_NR44_STOPn, EDEK_NR52_DBG_APUp);
  /*_p20.COTE*/ wire COTE_DBG_CH4_MUTE = 0;
  /*_p20.DATO*/ wire DATO_CH4_RAW_BIT = or2(EZUL_CH4_BIT_MUX, COTE_DBG_CH4_MUTE);

  /*#p20.AKOF*/ wire AKOF_CH4_DAC0 = and2(ch4.FEKO_CH4_VOL0.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.BYZY*/ wire BYZY_CH4_DAC1 = and2(ch4.FATY_CH4_VOL1.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.APYR*/ wire APYR_CH4_DAC2 = and2(ch4.FERU_CH4_VOL2.qp_old(), DATO_CH4_RAW_BIT);
  /*#p20.BOZA*/ wire BOZA_CH4_DAC3 = and2(ch4.FYRO_CH4_VOL3.qp_old(), DATO_CH4_RAW_BIT);

  return ((AKOF_CH4_DAC0 & 1) << 0) |
    ((BYZY_CH4_DAC1 & 1) << 1) |
    ((APYR_CH4_DAC2 & 1) << 2) |
    ((BOZA_CH4_DAC3 & 1) << 3);
}

//------------------------------------------------------------------------------

void tick_ch4_fast(
  uint64_t phase_new,
  int64_t spu_phase_new,

  const GateBoyCpuDBus& cpu_dbus_old,
  const GateBoySPU& spu_old,
  const SpuChannel4& ch4_old,
  const GateBoyWaveBus& wave_dbus_old,
  const GateBoyCpuABus& cpu_abus_new,

  GateBoyCpuDBus& cpu_dbus_new,
  GateBoySPU& spu_new,
  SpuChannel4& ch4_new,
  GateBoyWaveBus& wave_dbus_new,

  wire AFER_SYS_RSTp,
  wire ASOL_POR_DONEn,
  wire SIG_CPU_CLKREQ,
  wire AVET_AxCxExGx,
  wire AFUR_ABCDxxxx_qn,
  wire APUK_xxCDEFxx_qn,
  wire ALEF_xBCDExxx,
  wire SIG_IN_CPU_RDp,
  wire SIG_IN_CPU_WRp,
  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,
  wire SIG_IN_CPU_DBUS_FREE,

  uint8_t* wave_ram
)
{
  auto addr = bit_pack(cpu_abus_new);
  auto dbus_old = bit_pack(cpu_dbus_old);
  bool apu_rst = bit(or3(AFER_SYS_RSTp, ASOL_POR_DONEn, spu_new.HADA_NR52_ALL_SOUND_ON.qn_any()));

  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = and2(AFUR_ABCDxxxx_qn, SIG_CPU_CLKREQ);
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx = not1(BUDE_xxxxEFGH);


  /*#p01.HAMA*/ wire HAMA_CLK_512K = not1(spu_new.JESO_CLK_512K.qp_any());
  /*_p01.BURE*/ wire BURE_CLK_512 = not1(spu_new.BARA_CLK_512.qp_any());
  /*#p01.FYNE*/ wire FYNE_CLK_512 = not1(BURE_CLK_512);
  /*_p01.GALE*/ wire GALE_CLK_512 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_any(), HAMA_CLK_512K, FYNE_CLK_512);
  /*_p01.GEXY*/ wire GEXY_CLK_512 = not1(GALE_CLK_512);
  /*_p01.HORU*/ wire HORU_CLK_512 = not1(GEXY_CLK_512);

  /*_p01.CULO*/ wire CULO_CLK_256 = not1(spu_new.CARU_CLK_256.qp_any());
  /*_p01.BEZE*/ wire BEZE_CLK_256 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_any(), HAMA_CLK_512K, CULO_CLK_256);
  /*_p01.COFU*/ wire COFU_CLK_256 = not1(BEZE_CLK_256);
  /*_p01.BUFY*/ wire BUFY_CLK_256 = not1(COFU_CLK_256);

  /*_p01.APEF*/ wire APEF_CLK_128 = not1(spu_new.BYLU_CLK_128.qp_any());
  /*_p01.BULE*/ wire BULE_CLK_128 = mux2p(spu_new.FERO_NR52_DBG_APUp.qp_any(), HAMA_CLK_512K, APEF_CLK_128);
  /*_p01.BARU*/ wire BARU_CLK_128 = not1(BULE_CLK_128);
  /*_p01.BYFE*/ wire BYFE_CLK_128 = not1(BARU_CLK_128);

  auto nr42 = bit_pack(&ch4_new.EMOK_NR42_ENV_DELAY0p, 8);
  auto nr43 = bit_pack(&ch4_new.JARE_NR43_DIV0p, 8);

  if (apu_rst) {
    nr42 = 0;
    nr43 = 0;
    ch4_new.GYSU_CH4_TRIG.dff17(DOVA_ABCDxxxx, 0, ch4_old.HOGA_NR44_TRIGp.qp_any());
    ch4_new.GONE_CH4_TRIGp.dff17_clk(HAMA_CLK_512K, ch4_old.HAZO_CH4_TRIGn.qn_any());
    ch4_new.GORA_CH4_TRIGp.dff17(HAMA_CLK_512K, 0, ch4_old.GONE_CH4_TRIGp.qp_any());
    ch4_new.GATY_CH4_TRIGp.dff17(HAMA_CLK_512K, 0, ch4_old.GORA_CH4_TRIGp.qp_any());
    ch4_new.GONE_CH4_TRIGp.dff17_rst(0);
    ch4_new.HAZO_CH4_TRIGn.nor_latch(1, 0);
    ch4_new.CUNY_NR44_LEN_ENp.state = 0;
    ch4_new.HOGA_NR44_TRIGp.state = 0;
    if (bit(ch4_new.GYSU_CH4_TRIG.qp_any())) {
      ch4_new.HOGA_NR44_TRIGp.state = 0;
    }
  }
  else {
    ch4_new.GYSU_CH4_TRIG.dff17(DOVA_ABCDxxxx, 1, ch4_old.HOGA_NR44_TRIGp.qp_any());
    ch4_new.GONE_CH4_TRIGp.dff17_clk(HAMA_CLK_512K, ch4_old.HAZO_CH4_TRIGn.qn_any());
    ch4_new.GORA_CH4_TRIGp.dff17(HAMA_CLK_512K, 1, ch4_old.GONE_CH4_TRIGp.qp_any());
    ch4_new.GATY_CH4_TRIGp.dff17(HAMA_CLK_512K, 1, ch4_old.GORA_CH4_TRIGp.qp_any());
    ch4_new.GONE_CH4_TRIGp.dff17_rst(~ch4_new.GORA_CH4_TRIGp.qp_any());
    ch4_new.HAZO_CH4_TRIGn.nor_latch(ch4_new.GORA_CH4_TRIGp.qp_any(), ch4_new.GYSU_CH4_TRIG.qp_any());

    if (DELTA_GH && SIG_IN_CPU_WRp && SIG_IN_CPU_DBUS_FREE) {
      if (addr == 0xFF21) nr42 = dbus_old;
      if (addr == 0xFF22) nr43 = dbus_old;
      if (addr == 0xFF23) {
        ch4_new.CUNY_NR44_LEN_ENp.state = cpu_dbus_old.BUS_CPU_D06p.qp_any();
        ch4_new.HOGA_NR44_TRIGp.state = cpu_dbus_old.BUS_CPU_D07p.qp_any();
      }
    }
    if (bit(ch4_new.GYSU_CH4_TRIG.qp_any())) {
      ch4_new.HOGA_NR44_TRIGp.state = 0;
    }
  }

  bit_unpack(&ch4_new.EMOK_NR42_ENV_DELAY0p, 8, nr42);
  bit_unpack(&ch4_new.JARE_NR43_DIV0p,       8, nr43);

  wire GUNY_FREQ_GATE_RSTn_new = nor2(apu_rst, ch4_new.GONE_CH4_TRIGp.qp_any());

  auto div_old = bit_pack(&ch4_old.JYCO_DIV0, 3);

  if (DELTA_AB) {
    ch4_new.GARY_FREQ_GATEp.state = div_old == 7;
  }

  if (!bit(GUNY_FREQ_GATE_RSTn_new)) {
    ch4_new.GARY_FREQ_GATEp.state = 0;
  }




  auto freq = bit_pack(&ch4_new.CEXO_FREQ_00, 14);

  if (DELTA_EF && bit(ch4_new.GARY_FREQ_GATEp.qp_any())) {
    freq++;
  }

  if (apu_rst) {
    freq = 0;
  }

  bit_unpack(&ch4_new.CEXO_FREQ_00, 14, freq);

  uint8_t lfsr_clk_old = 0;
  switch(bit_pack(&ch4_old.FETA_NR43_FREQ0p, 4)) {
  case 0:  lfsr_clk_old = ch4_old.CEXO_FREQ_00.qp_any(); break;
  case 1:  lfsr_clk_old = ch4_old.DEKO_FREQ_01.qp_any(); break;
  case 2:  lfsr_clk_old = ch4_old.EZEF_FREQ_02.qp_any(); break;
  case 3:  lfsr_clk_old = ch4_old.EPOR_FREQ_03.qp_any(); break;
  case 4:  lfsr_clk_old = ch4_old.DURE_FREQ_04.qp_any(); break;
  case 5:  lfsr_clk_old = ch4_old.DALE_FREQ_05.qp_any(); break;
  case 6:  lfsr_clk_old = ch4_old.DOKE_FREQ_06.qp_any(); break;
  case 7:  lfsr_clk_old = ch4_old.DEMO_FREQ_07.qp_any(); break;
  case 8:  lfsr_clk_old = ch4_old.DOSE_FREQ_08.qp_any(); break;
  case 9:  lfsr_clk_old = ch4_old.DETE_FREQ_09.qp_any(); break;
  case 10: lfsr_clk_old = ch4_old.ERUT_FREQ_10.qp_any(); break;
  case 11: lfsr_clk_old = ch4_old.DOTA_FREQ_11.qp_any(); break;
  case 12: lfsr_clk_old = ch4_old.DERE_FREQ_12.qp_any(); break;
  case 13: lfsr_clk_old = ch4_old.ESEP_FREQ_13.qp_any(); break;
  case 14: lfsr_clk_old = 0; break;
  case 15: lfsr_clk_old = 0; break;
  }

  uint8_t lfsr_clk_new = 0;
  switch(bit_pack(&ch4_new.FETA_NR43_FREQ0p, 4)) {
  case 0:  lfsr_clk_new = ch4_new.CEXO_FREQ_00.qp_any(); break;
  case 1:  lfsr_clk_new = ch4_new.DEKO_FREQ_01.qp_any(); break;
  case 2:  lfsr_clk_new = ch4_new.EZEF_FREQ_02.qp_any(); break;
  case 3:  lfsr_clk_new = ch4_new.EPOR_FREQ_03.qp_any(); break;
  case 4:  lfsr_clk_new = ch4_new.DURE_FREQ_04.qp_any(); break;
  case 5:  lfsr_clk_new = ch4_new.DALE_FREQ_05.qp_any(); break;
  case 6:  lfsr_clk_new = ch4_new.DOKE_FREQ_06.qp_any(); break;
  case 7:  lfsr_clk_new = ch4_new.DEMO_FREQ_07.qp_any(); break;
  case 8:  lfsr_clk_new = ch4_new.DOSE_FREQ_08.qp_any(); break;
  case 9:  lfsr_clk_new = ch4_new.DETE_FREQ_09.qp_any(); break;
  case 10: lfsr_clk_new = ch4_new.ERUT_FREQ_10.qp_any(); break;
  case 11: lfsr_clk_new = ch4_new.DOTA_FREQ_11.qp_any(); break;
  case 12: lfsr_clk_new = ch4_new.DERE_FREQ_12.qp_any(); break;
  case 13: lfsr_clk_new = ch4_new.ESEP_FREQ_13.qp_any(); break;
  case 14: lfsr_clk_new = 0; break;
  case 15: lfsr_clk_new = 0; break;
  }


  {
    /*#p19.CUWA*/ wire CUWA_LEN_CLKa = or3(ch4_old.FUGO_CH4_LEN_DONEp.qp_any(), BUFY_CLK_256, ch4_new.CUNY_NR44_LEN_ENp.qn_any());

    /*#p19.DANO*/ ch4_new.DANO_NR41_LEN0p.dff20(CUWA_LEN_CLKa,                    and2(TAPU_CPU_WRp, addr == 0xFF20), cpu_dbus_new.BUS_CPU_D00p.qp_any());
    /*#p19.FAVY*/ ch4_new.FAVY_NR41_LEN1p.dff20(ch4_new.DANO_NR41_LEN0p.qp_any(), and2(TAPU_CPU_WRp, addr == 0xFF20), cpu_dbus_new.BUS_CPU_D01p.qp_any());
    /*#p19.DENA*/ ch4_new.DENA_NR41_LEN2p.dff20(ch4_new.FAVY_NR41_LEN1p.qp_any(), and2(TAPU_CPU_WRp, addr == 0xFF20), cpu_dbus_new.BUS_CPU_D02p.qp_any());
    /*#p19.CEDO*/ ch4_new.CEDO_NR41_LEN3p.dff20(ch4_new.DENA_NR41_LEN2p.qp_any(), and2(TAPU_CPU_WRp, addr == 0xFF20), cpu_dbus_new.BUS_CPU_D03p.qp_any());
    /*#p19.FYLO*/ ch4_new.FYLO_NR41_LEN4p.dff20(ch4_new.CEDO_NR41_LEN3p.qp_any(), and2(TAPU_CPU_WRp, addr == 0xFF20), cpu_dbus_new.BUS_CPU_D04p.qp_any());
    /*#p19.EDOP*/ ch4_new.EDOP_NR41_LEN5p.dff20(ch4_new.FYLO_NR41_LEN4p.qp_any(), and2(TAPU_CPU_WRp, addr == 0xFF20), cpu_dbus_new.BUS_CPU_D05p.qp_any());

    if (DELTA_DE && SIG_IN_CPU_WRp && addr == 0xFF20) {
    }

    /*#p19.GAPY*/ wire GAPY_LEN_DONE_RSTn = nor3(and2(TAPU_CPU_WRp, addr == 0xFF20), apu_rst, ch4_new.GONE_CH4_TRIGp.qp_any());
    /*#p19.FUGO*/ ch4_new.FUGO_CH4_LEN_DONEp.dff17(ch4_new.EDOP_NR41_LEN5p.qn_any(), GAPY_LEN_DONE_RSTn, ch4_old.FUGO_CH4_LEN_DONEp.qn_any());

    wire CUWA_LEN_CLKa2 = or3(ch4_new.FUGO_CH4_LEN_DONEp.qp_any(), BUFY_CLK_256, ch4_new.CUNY_NR44_LEN_ENp.qn_any());

    /*#p19.DANO*/ ch4_new.DANO_NR41_LEN0p.dff20_any(CUWA_LEN_CLKa2, and2(TAPU_CPU_WRp, addr == 0xFF20), cpu_dbus_new.BUS_CPU_D00p.qp_any());
  }

  wire GEVY_CH4_AMP_ENn = nor5(
    ch4_new.GEKY_NR42_ENV_DIRp.qp_any(),
    ch4_new.GARU_NR42_ENV0p.qp_any(),
    ch4_new.GOKY_NR42_ENV1p.qp_any(),
    ch4_new.GOZO_NR42_ENV2p.qp_any(),
    ch4_new.GEDU_NR42_ENV3p.qp_any());


  {
   
    if (bit(ch4_new.GONE_CH4_TRIGp.qp_any())) {
      ch4_new.GENA_CH4_ACTIVEp.state = 1;
    }

    wire EFOT_CH4_STOPp = and2(ch4_new.CUNY_NR44_LEN_ENp.qp_any(), ch4_new.FUGO_CH4_LEN_DONEp.qp_any());
    if (bit(or3(apu_rst, EFOT_CH4_STOPp, GEVY_CH4_AMP_ENn))) {
      ch4_new.GENA_CH4_ACTIVEp.state = 0;
    }
  }

  {
    /*#p20.JERY*/ ch4_new.JERY_DIV_GATE.nand_latch(nor2(GEVY_CH4_AMP_ENn, apu_rst), not1(ch4_new.GATY_CH4_TRIGp.qp_any()));
  }

  {
    /*#p20.KANU*/ wire KANU_DIV_CLKa = or2(ch4_new.JERY_DIV_GATE.qp_any(), spu_new.JESO_CLK_512K.qp_any());
    /*#p20.HUCE*/ wire HUCE_DIV_LOADp = or2(ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GARY_FREQ_GATEp.qp_any());

    /*#p20.JYCO*/ ch4_new.JYCO_DIV0.dff20(KANU_DIV_CLKa,              HUCE_DIV_LOADp, ch4_new.JARE_NR43_DIV0p.qn_any());
    /*#p20.JYRE*/ ch4_new.JYRE_DIV1.dff20(ch4_new.JYCO_DIV0.qp_any(), HUCE_DIV_LOADp, ch4_new.JERO_NR43_DIV1p.qn_any());
    /*_p20.JYFU*/ ch4_new.JYFU_DIV2.dff20(ch4_new.JYRE_DIV1.qp_any(), HUCE_DIV_LOADp, ch4_new.JAKY_NR43_DIV2p.qn_any());
  }


  //----------
  // The actual LFSR

  {
    /*#p20.HURA*/ wire HURA_LFSR_IN_old = xnor2(ch4_old.HEZU_LFSR_15.qp_any(), ch4_old.HYRO_LFSR_14.qp_any());

    if (or2(ch4_new.GONE_CH4_TRIGp.qp_any(), apu_rst) & 1) {
      /*#p20.JOTO*/ ch4_new.JOTO_LFSR_00.dff17(not1(lfsr_clk_new), 0, HURA_LFSR_IN_old);
      /*#p20.KOMU*/ ch4_new.KOMU_LFSR_01.dff17(lfsr_clk_new,       0, ch4_old.JOTO_LFSR_00.qp_any());
      /*#p20.KETU*/ ch4_new.KETU_LFSR_02.dff17(lfsr_clk_new,       0, ch4_old.KOMU_LFSR_01.qp_any());
      /*#p20.KUTA*/ ch4_new.KUTA_LFSR_03.dff17(lfsr_clk_new,       0, ch4_old.KETU_LFSR_02.qp_any());
      /*#p20.KUZY*/ ch4_new.KUZY_LFSR_04.dff17(lfsr_clk_new,       0, ch4_old.KUTA_LFSR_03.qp_any());
      /*#p20.KYWY*/ ch4_new.KYWY_LFSR_05.dff17(lfsr_clk_new,       0, ch4_old.KUZY_LFSR_04.qp_any());
      /*#p20.JAJU*/ ch4_new.JAJU_LFSR_06.dff17(lfsr_clk_new,       0, ch4_old.KYWY_LFSR_05.qp_any());
      /*#p20.HAPE*/ ch4_new.HAPE_LFSR_07.dff17(lfsr_clk_new,       0, ch4_old.JAJU_LFSR_06.qp_any());
      /*#p20.JUXE*/ ch4_new.JUXE_LFSR_08.dff17(lfsr_clk_new,       0, ch4_old.HAPE_LFSR_07.qp_any());

      /*#p20.KAVU*/ wire KAVU_LFSR_FB_old = amux2(ch4_old.JOTO_LFSR_00.qp_any(), ch4_old.JAMY_NR43_MODEp.qp_any(), ch4_old.JAMY_NR43_MODEp.qn_any(), ch4_old.JUXE_LFSR_08.qp_any());
      /*#p20.JEPE*/ ch4_new.JEPE_LFSR_09.dff17(lfsr_clk_new,       0, KAVU_LFSR_FB_old);
      /*#p20.JAVO*/ ch4_new.JAVO_LFSR_10.dff17(lfsr_clk_new,       0, ch4_old.JEPE_LFSR_09.qp_any());
      /*#p20.HEPA*/ ch4_new.HEPA_LFSR_11.dff17(lfsr_clk_new,       0, ch4_old.JAVO_LFSR_10.qp_any());
      /*#p20.HORY*/ ch4_new.HORY_LFSR_12.dff17(lfsr_clk_new,       0, ch4_old.HEPA_LFSR_11.qp_any());
      /*#p20.HENO*/ ch4_new.HENO_LFSR_13.dff17(lfsr_clk_new,       0, ch4_old.HORY_LFSR_12.qp_any());
      /*#p20.HYRO*/ ch4_new.HYRO_LFSR_14.dff17(lfsr_clk_new,       0, ch4_old.HENO_LFSR_13.qp_any());
      /*#p20.HEZU*/ ch4_new.HEZU_LFSR_15.dff17(lfsr_clk_new,       0, ch4_old.HYRO_LFSR_14.qp_any());
    } else {

      auto lfsr = bit_pack(&ch4_new.JOTO_LFSR_00, 16);
      /*

      if (posedge(lfsr_clk_old, lfsr_clk_new)) {
        if (bit(ch4_old.JAMY_NR43_MODEp.qp_any())) {
          lfsr = (lfsr << 1) | bit(HURA_LFSR_IN_old);

          lfsr &= 0b1111110111111111;
          lfsr |= bit(HURA_LFSR_IN_old) << 9;

          bit_unpack(&ch4_new.JOTO_LFSR_00, 16, lfsr);
        }
        else {
          lfsr = (lfsr << 1) | bit(HURA_LFSR_IN_old);

          bit_unpack(&ch4_new.JOTO_LFSR_00, 16, lfsr);
        }
      }
      */


      if (bit(ch4_old.JAMY_NR43_MODEp.qp_any())) {
        /*#p20.JOTO*/ ch4_new.JOTO_LFSR_00.dff17(not1(lfsr_clk_new), 1, bit(HURA_LFSR_IN_old));
        /*#p20.KOMU*/ ch4_new.KOMU_LFSR_01.dff17(lfsr_clk_new,       1, ch4_old.JOTO_LFSR_00.qp_any());
        /*#p20.KETU*/ ch4_new.KETU_LFSR_02.dff17(lfsr_clk_new,       1, ch4_old.KOMU_LFSR_01.qp_any());
        /*#p20.KUTA*/ ch4_new.KUTA_LFSR_03.dff17(lfsr_clk_new,       1, ch4_old.KETU_LFSR_02.qp_any());
        /*#p20.KUZY*/ ch4_new.KUZY_LFSR_04.dff17(lfsr_clk_new,       1, ch4_old.KUTA_LFSR_03.qp_any());
        /*#p20.KYWY*/ ch4_new.KYWY_LFSR_05.dff17(lfsr_clk_new,       1, ch4_old.KUZY_LFSR_04.qp_any());
        /*#p20.JAJU*/ ch4_new.JAJU_LFSR_06.dff17(lfsr_clk_new,       1, ch4_old.KYWY_LFSR_05.qp_any());
        /*#p20.HAPE*/ ch4_new.HAPE_LFSR_07.dff17(lfsr_clk_new,       1, ch4_old.JAJU_LFSR_06.qp_any());
        /*#p20.JUXE*/ ch4_new.JUXE_LFSR_08.dff17(lfsr_clk_new,       1, ch4_old.HAPE_LFSR_07.qp_any());

        /*#p20.JEPE*/ ch4_new.JEPE_LFSR_09.dff17(lfsr_clk_new,       1, ch4_old.JOTO_LFSR_00.qp_any());
      }
      else {
        /*#p20.JOTO*/ ch4_new.JOTO_LFSR_00.dff17(not1(lfsr_clk_new), 1, bit(HURA_LFSR_IN_old));
        /*#p20.KOMU*/ ch4_new.KOMU_LFSR_01.dff17(lfsr_clk_new,       1, ch4_old.JOTO_LFSR_00.qp_any());
        /*#p20.KETU*/ ch4_new.KETU_LFSR_02.dff17(lfsr_clk_new,       1, ch4_old.KOMU_LFSR_01.qp_any());
        /*#p20.KUTA*/ ch4_new.KUTA_LFSR_03.dff17(lfsr_clk_new,       1, ch4_old.KETU_LFSR_02.qp_any());
        /*#p20.KUZY*/ ch4_new.KUZY_LFSR_04.dff17(lfsr_clk_new,       1, ch4_old.KUTA_LFSR_03.qp_any());
        /*#p20.KYWY*/ ch4_new.KYWY_LFSR_05.dff17(lfsr_clk_new,       1, ch4_old.KUZY_LFSR_04.qp_any());
        /*#p20.JAJU*/ ch4_new.JAJU_LFSR_06.dff17(lfsr_clk_new,       1, ch4_old.KYWY_LFSR_05.qp_any());
        /*#p20.HAPE*/ ch4_new.HAPE_LFSR_07.dff17(lfsr_clk_new,       1, ch4_old.JAJU_LFSR_06.qp_any());
        /*#p20.JUXE*/ ch4_new.JUXE_LFSR_08.dff17(lfsr_clk_new,       1, ch4_old.HAPE_LFSR_07.qp_any());
        /*#p20.JEPE*/ ch4_new.JEPE_LFSR_09.dff17(lfsr_clk_new,       1, ch4_old.JUXE_LFSR_08.qp_any());
      }

      if (posedge(lfsr_clk_old, lfsr_clk_new)) {
        ch4_new.JAVO_LFSR_10.state = ch4_old.JEPE_LFSR_09.state;
        ch4_new.HEPA_LFSR_11.state = ch4_old.JAVO_LFSR_10.state;
        ch4_new.HORY_LFSR_12.state = ch4_old.HEPA_LFSR_11.state;
        ch4_new.HENO_LFSR_13.state = ch4_old.HORY_LFSR_12.state;
        ch4_new.HYRO_LFSR_14.state = ch4_old.HENO_LFSR_13.state;
        ch4_new.HEZU_LFSR_15.state = ch4_old.HYRO_LFSR_14.state;
      }
    }

  }

  //----------
  // Env

  auto env_timer_old = bit_pack(&ch4_old.CUNA_ENV_TIMER0n, 3) ^ 7;
  auto env_delay_new = bit_pack(&ch4_new.EMOK_NR42_ENV_DELAY0p, 3);

  {

    ch4_new.FOSY_ENV_CLKp.dff17_clk(HORU_CLK_512, env_timer_old != 7);

    if (bit(and2(~HORU_CLK_512, ch4_new.FOSY_ENV_CLKp.qp_any())) || (env_delay_new == 0) || bit(ch4_new.GONE_CH4_TRIGp.qp_any()) || apu_rst) {
      ch4_new.FOSY_ENV_CLKp.state &= ~1;
    }
  }

  {
    /*#p20.ABEL*/ ch4_new.ABEL_CLK_64.dff17(not1(BYFE_CLK_128), !apu_rst, ch4_old.ABEL_CLK_64.qn_any());

    /*#p20.DAPY*/ wire DAPY_ENV_TIMER_LOADp_new = or2(ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.FOSY_ENV_CLKp.qp_any());

    /*#p20.CUNA*/ ch4_new.CUNA_ENV_TIMER0n.dff20(ch4_new.ABEL_CLK_64.qp_any(),      DAPY_ENV_TIMER_LOADp_new, ch4_new.EMOK_NR42_ENV_DELAY0p.qn_any());
    /*#p20.COFE*/ ch4_new.COFE_ENV_TIMER1n.dff20(ch4_new.CUNA_ENV_TIMER0n.qp_any(), DAPY_ENV_TIMER_LOADp_new, ch4_new.ETYJ_NR42_ENV_DELAY1p.qn_any());
    /*#p20.DOGO*/ ch4_new.DOGO_ENV_TIMER2n.dff20(ch4_new.COFE_ENV_TIMER1n.qp_any(), DAPY_ENV_TIMER_LOADp_new, ch4_new.EZYK_NR42_ENV_DELAY2p.qn_any());
  }


  {
    auto vol_old = bit_pack(&ch4_old.FEKO_CH4_VOL0, 4);

    if (bit(ch4_old.GEKY_NR42_ENV_DIRp.qp_any())) {
      ch4_new.FYNO_VOL_MAXp.dff17(ch4_new.FOSY_ENV_CLKp.qp_any(), 1, vol_old == 15);
    } else {
      ch4_new.FYNO_VOL_MAXp.dff17(ch4_new.FOSY_ENV_CLKp.qp_any(), 1, vol_old == 0);
    }


    if (bit(or2(ch4_new.GONE_CH4_TRIGp.qp_any(), apu_rst))) {
      ch4_new.FYNO_VOL_MAXp.state &= ~1;
    }
  }


  if (bit(ch4_new.FYNO_VOL_MAXp.qp_any())) {
    ch4_new.EROX_ENV_RUNNINGn.state = 1;
  }
  if (bit(or2(apu_rst, ch4_new.GONE_CH4_TRIGp.qp_any()))) {
    ch4_new.EROX_ENV_RUNNINGn.state = 0;
  }


  /*#p20.FELO*/ wire FELO_ENV_CLK = or3(ch4_new.FOSY_ENV_CLKp.qp_any(), env_delay_new == 0, ch4_new.EROX_ENV_RUNNINGn.qp_any());

  if (bit(ch4_new.GEKY_NR42_ENV_DIRp.qp_any())) {
    /*#p20.FEKO*/ ch4_new.FEKO_CH4_VOL0.dff20(FELO_ENV_CLK,                   ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GARU_NR42_ENV0p.qp_any());
    /*#p20.FATY*/ ch4_new.FATY_CH4_VOL1.dff20(ch4_new.FEKO_CH4_VOL0.qp_any(), ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GOKY_NR42_ENV1p.qp_any());
    /*#p20.FERU*/ ch4_new.FERU_CH4_VOL2.dff20(ch4_new.FATY_CH4_VOL1.qp_any(), ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GOZO_NR42_ENV2p.qp_any());
    /*#p20.FYRO*/ ch4_new.FYRO_CH4_VOL3.dff20(ch4_new.FERU_CH4_VOL2.qp_any(), ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GEDU_NR42_ENV3p.qp_any());
  }
  else {
    /*#p20.FEKO*/ ch4_new.FEKO_CH4_VOL0.dff20(FELO_ENV_CLK,                   ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GARU_NR42_ENV0p.qp_any());
    /*#p20.FATY*/ ch4_new.FATY_CH4_VOL1.dff20(ch4_new.FEKO_CH4_VOL0.qn_any(), ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GOKY_NR42_ENV1p.qp_any());
    /*#p20.FERU*/ ch4_new.FERU_CH4_VOL2.dff20(ch4_new.FATY_CH4_VOL1.qn_any(), ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GOZO_NR42_ENV2p.qp_any());
    /*#p20.FYRO*/ ch4_new.FYRO_CH4_VOL3.dff20(ch4_new.FERU_CH4_VOL2.qn_any(), ch4_new.GONE_CH4_TRIGp.qp_any(), ch4_new.GEDU_NR42_ENV3p.qp_any());
  }

  if (SIG_IN_CPU_RDp) {
    if (addr == 0xFF21) bit_unpack(cpu_dbus_new, bit_pack(&ch4_new.EMOK_NR42_ENV_DELAY0p, 8));
    if (addr == 0xFF22) bit_unpack(cpu_dbus_new, bit_pack(&ch4_new.JARE_NR43_DIV0p, 8));
    if (addr == 0xFF23) cpu_dbus_new.BUS_CPU_D06p.state = ch4_new.CUNY_NR44_LEN_ENp.qp_any();
  }
}

#endif
