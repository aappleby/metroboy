#include "GateBoySPU.h"

#include "GateBoyLib/GateBoy.h"

#ifdef SIM_AUDIO

int ch3_audio_out(const GateBoyState& reg_new) {

  /*#p17.DATE*/ wire DATE_WAVE_PLAY_D0 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_any(), reg_new.ch3.CYFO_SAMPLE0p.qn_any(), reg_new.ch3.CUVO_SAMPLE4p.qn_any());
  /*#p17.DAZY*/ wire DAZY_WAVE_PLAY_D1 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_any(), reg_new.ch3.CESY_SAMPLE1p.qn_any(), reg_new.ch3.CEVO_SAMPLE5p.qn_any());
  /*#p17.CUZO*/ wire CUZO_WAVE_PLAY_D2 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_any(), reg_new.ch3.BUDY_SAMPLE2p.qn_any(), reg_new.ch3.BORA_SAMPLE6p.qn_any());
  /*#p17.COPO*/ wire COPO_WAVE_PLAY_D3 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_any(), reg_new.ch3.BEGU_SAMPLE3p.qn_any(), reg_new.ch3.BEPA_SAMPLE7p.qn_any());

  /*#p18.GEMY*/ wire GEMY_WAVE_VOL3 = nor2(reg_new.ch3.HUKY_NR32_VOL0p.qn_any(), reg_new.ch3.HODY_NR32_VOL1p.qn_any());
  /*#p18.GOKA*/ wire GOKA_WAVE_VOL2 = nor2(reg_new.ch3.HUKY_NR32_VOL0p.qn_any(), reg_new.ch3.HODY_NR32_VOL1p.qp_any());
  /*#p18.GEGE*/ wire GEGE_WAVE_VOL4 = nor2(reg_new.ch3.HUKY_NR32_VOL0p.qp_any(), reg_new.ch3.HODY_NR32_VOL1p.qn_any());

  /*#p18.EZAG*/ wire EZAG_WAVE_OUT0 = amux3(DATE_WAVE_PLAY_D0, GEGE_WAVE_VOL4, DAZY_WAVE_PLAY_D1, GOKA_WAVE_VOL2, CUZO_WAVE_PLAY_D2, GEMY_WAVE_VOL3);
  /*#p18.EVUG*/ wire EVUG_WAVE_OUT1 = amux3(DAZY_WAVE_PLAY_D1, GEGE_WAVE_VOL4, CUZO_WAVE_PLAY_D2, GOKA_WAVE_VOL2, COPO_WAVE_PLAY_D3, GEMY_WAVE_VOL3);
  /*#p18.DOKY*/ wire DOKY_WAVE_OUT2 = amux2(CUZO_WAVE_PLAY_D2, GEGE_WAVE_VOL4, COPO_WAVE_PLAY_D3, GOKA_WAVE_VOL2);
  /*#p18.DORE*/ wire DORE_WAVE_OUT3 = and2 (COPO_WAVE_PLAY_D3, GEGE_WAVE_VOL4);

  /*#p18.COKA*/ wire COKA_CH3_ACTIVEp = not1(reg_new.ch3.DAVO_CH3_ACTIVEp.qn_any());
  /*#p18.BARY*/ wire BARY_WAVE_DAC0 = and2(COKA_CH3_ACTIVEp, EZAG_WAVE_OUT0);
  /*#p18.BYKA*/ wire BYKA_WAVE_DAC1 = and2(COKA_CH3_ACTIVEp, EVUG_WAVE_OUT1);
  /*#p18.BOPA*/ wire BOPA_WAVE_DAC2 = and2(COKA_CH3_ACTIVEp, DOKY_WAVE_OUT2);
  /*#p18.BELY*/ wire BELY_WAVE_DAC3 = and2(COKA_CH3_ACTIVEp, DORE_WAVE_OUT3);

  return ((BARY_WAVE_DAC0 & 1) << 0) |
         ((BYKA_WAVE_DAC1 & 1) << 1) |
         ((BOPA_WAVE_DAC2 & 1) << 2) |
         ((BELY_WAVE_DAC3 & 1) << 3);
}

void SpuChannel3::reset_to_cart() {
  KOGA_NR33_FREQ00p.state = 0x1a;
  JOVY_NR33_FREQ01p.state = 0x1a;
  JAXA_NR33_FREQ02p.state = 0x1a;
  JEFE_NR33_FREQ03p.state = 0x1a;
  JYPO_NR33_FREQ04p.state = 0x1a;
  JOVE_NR33_FREQ05p.state = 0x1a;
  KANA_NR33_FREQ06p.state = 0x1a;
  KOGU_NR33_FREQ07p.state = 0x1a;
  JEMO_NR34_FREQ08p.state = 0x1a;
  JETY_NR34_FREQ09p.state = 0x1a;
  JACY_NR34_FREQ10p.state = 0x1a;
  HOTO_NR34_LENENp.state = 0x1a;
  GAVU_NR34_TRIGp.state = 0x1a;
  DAVO_CH3_ACTIVEp.state = 0x1a;
  GOFY_CH3_TRIGn.state = 0x19;
  FOZU_CH3_ACTIVEp.state = 0x18;
  GUXE_NR30_AMP_ENp.state = 0x1a;
  GEVO_NR31_LEN0p.state = 0x1a;
  FORY_NR31_LEN1p.state = 0x18;
  GATU_NR31_LEN2p.state = 0x18;
  GAPO_NR31_LEN3p.state = 0x18;
  GEMO_NR31_LEN4p.state = 0x18;
  FORO_NR31_LEN5p.state = 0x18;
  FAVE_NR31_LEN6p.state = 0x18;
  FYRU_NR31_LEN7p.state =   0x18;
  FEXU_LEN_DONEp.state = 0x1a;
  HUKY_NR32_VOL0p.state = 0x1a;
  HODY_NR32_VOL1p.state = 0x1a;
  GARA_TRIG_D1.state = 0x18;
  GYTA_TRIG_D2.state = 0x18;
  GYRA_TRIG_D3.state = 0x1a;
  KUTU_COUNT00.state = 0x1b;
  KUPE_COUNT01.state = 0x1a;
  KUNU_COUNT02.state = 0x18;
  KEMU_COUNT03.state = 0x18;
  KYGU_COUNT04.state = 0x18;
  KEPA_COUNT05.state = 0x18;
  KAFO_COUNT06.state = 0x18;
  KENO_COUNT07.state = 0x18;
  KEJU_COUNT08.state = 0x18;
  KEZA_COUNT09.state = 0x18;
  JAPU_COUNT10.state = 0x18;
  HUNO_SAMPLE_CLKp.state = 0x1a;
  EFAR_WAVE_IDX0.state = 0x1a;
  ERUS_WAVE_IDX1.state = 0x1a;
  EFUZ_WAVE_IDX2.state = 0x1a;
  EXEL_WAVE_IDX3.state = 0x1a;
  EFAL_WAVE_IDX4.state = 0x1a;
  FETY_WAVE_LOOP.state = 0x1a;
  CYFO_SAMPLE0p.state = 0x1a;
  CESY_SAMPLE1p.state = 0x1a;
  BUDY_SAMPLE2p.state = 0x1a;
  BEGU_SAMPLE3p.state = 0x1a;
  CUVO_SAMPLE4p.state = 0x1a;
  CEVO_SAMPLE5p.state = 0x1a;
  BORA_SAMPLE6p.state = 0x1a;
  BEPA_SAMPLE7p.state = 0x1a;
  FOBA_CH3_TRIGp.state = 0x18;
  GUGU_FREQ_GATE.state = 0x19;
  BUSA_WAVE_CLK_D1.state = 0x18;
  BANO_WAVE_CLK_D2.state = 0x1a;
  AZUS_WAVE_CLK_D3.state = 0x18;
  AZET_WAVE_CLK_D4.state = 0x1a;  
}

void tick_ch3(const GateBoyState& reg_old, GateBoyState& reg_new, uint8_t* wave_ram) {
  {
    /*_p16.GEJO*/ wire GEJO_FF1A_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.EMOR_ADDR_FF1Ap());
    /*_p16.GUCY*/ wire GUCY_FF1A_WRn = not1(GEJO_FF1A_WRp);
    /*_p16.GUXE*/ reg_new.ch3.GUXE_NR30_AMP_ENp.dff9b(GUCY_FF1A_WRn, reg_new.GOVE_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*_p16.HAGA*/ wire HAGA_FF1C_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.GEFO_ADDR_FF1Cp());
    /*_p16.GUZU*/ wire GUZU_FF1C_WRn = not1(HAGA_FF1C_WRp);
    /*_p16.HUKY*/ reg_new.ch3.HUKY_NR32_VOL0p.dff9b(GUZU_FF1C_WRn, reg_new.GURO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p16.HODY*/ reg_new.ch3.HODY_NR32_VOL1p.dff9b(GUZU_FF1C_WRn, reg_new.GURO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  }

  {
    /*_p16.KOTA*/ wire KOTA_FF1D_WRn = nand2(reg_new.cpu_abus.FENY_ADDR_FF1Dp(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*_p16.JAFA*/ wire JAFA_FF1D_WRn = nand2(reg_new.cpu_abus.FENY_ADDR_FF1Dp(), reg_new.cpu_signals.BOGY_CPU_WRp());

    // Clock polarity again
    /*_p16.KYHO*/ wire KYHO_FF1D_WRp = not1(KOTA_FF1D_WRn);
    /*_p16.KULY*/ wire KULY_FF1D_WRp = not1(JAFA_FF1D_WRn);
    /*_p16.KYHO*/ wire KYHO_FF1D_WRn = not1(KYHO_FF1D_WRp);
    /*_p16.KULY*/ wire KULY_FF1D_WRn = not1(KULY_FF1D_WRp);

    /*_p16.KOGA*/ reg_new.ch3.KOGA_NR33_FREQ00p.dff9b(KULY_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p16.JOVY*/ reg_new.ch3.JOVY_NR33_FREQ01p.dff9b(KULY_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p16.JAXA*/ reg_new.ch3.JAXA_NR33_FREQ02p.dff9b(KULY_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p16.JEFE*/ reg_new.ch3.JEFE_NR33_FREQ03p.dff9b(KULY_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p16.JYPO*/ reg_new.ch3.JYPO_NR33_FREQ04p.dff9b(KULY_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p16.JOVE*/ reg_new.ch3.JOVE_NR33_FREQ05p.dff9b(KYHO_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p16.KANA*/ reg_new.ch3.KANA_NR33_FREQ06p.dff9b(KYHO_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p16.KOGU*/ reg_new.ch3.KOGU_NR33_FREQ07p.dff9b(KYHO_FF1D_WRn, reg_new.KUHA_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*#p16.HUDA*/ wire HUDA_FF1E_WRp = and2(reg_new.cpu_abus.DUGO_ADDR_FF1Ep(), reg_new.cpu_signals.BOGY_CPU_WRp());
    /*#p16.JUZO*/ wire JUZO_FF1E_WRn = not1(HUDA_FF1E_WRp);

    /*_p16.JEMO*/ reg_new.ch3.JEMO_NR34_FREQ08p.dff9b(JUZO_FF1E_WRn, reg_new.KOPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p16.JETY*/ reg_new.ch3.JETY_NR34_FREQ09p.dff9b(JUZO_FF1E_WRn, reg_new.KOPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p16.JACY*/ reg_new.ch3.JACY_NR34_FREQ10p.dff9b(JUZO_FF1E_WRn, reg_new.KOPY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  }

  //----------------------------------------

  {
    /*#p18.EZAS*/ wire EZAS_CH3_ACTIVEn_old = not1(reg_old.ch3.FOZU_CH3_ACTIVEp.qp_old());
    /*#p18.DORU*/ wire DORU_CH3_ACTIVEp_old = not1(EZAS_CH3_ACTIVEn_old);
    /*#p18.DAVO*/ reg_new.ch3.DAVO_CH3_ACTIVEp.dff17(reg_new.spu.AJER_AxxDExxH.qp_new(), reg_new.CALU_APU_RSTn_new(), DORU_CH3_ACTIVEp_old);
  }

  {
    /*#p16.FOBA*/ reg_new.ch3.FOBA_CH3_TRIGp.dff17(reg_new.sys_clk.DOVA_ABCDxxxx(), reg_new.GOMA_APU_RSTn_new(), reg_old.ch3.GAVU_NR34_TRIGp.qp_oldB());
    /*_p16.EPYX*/ wire EPYX_FF1E_WRp = nor2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DUGO_ADDR_FF1Ep()); // polarity?
    /*_p16.FAKO*/ wire FAKO_RESTART_RST = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch3.FOBA_CH3_TRIGp.qp_new());
    /*_p16.GAVU*/ reg_new.ch3.GAVU_NR34_TRIGp.dff9b(EPYX_FF1E_WRp, FAKO_RESTART_RST, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*#p16.GYRA*/ reg_new.ch3.GYRA_TRIG_D3.dff17(reg_new.spu.CERY_CLK_2M.qp_new(), reg_new.GAZE_APU_RSTn_new(), reg_old.ch3.GYTA_TRIG_D2.qp_old());

    /*#p16.GYTA*/ reg_new.ch3.GYTA_TRIG_D2.dff17(reg_new.spu.FABO_CLK_xxCDxxGH(),  reg_new.GAZE_APU_RSTn_new(), reg_old.ch3.GARA_TRIG_D1.qp_old());

    /*#p16.FURY*/ wire FURY_TRIG_RSTn = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch3.GYTA_TRIG_D2.qp_new());

    /*#p16.GARA*/ reg_new.ch3.GARA_TRIG_D1.dff17(reg_new.spu.FABO_CLK_xxCDxxGH(),  FURY_TRIG_RSTn,              reg_old.ch3.GOFY_CH3_TRIGn.qn_old());

    /*#p16.GULO*/ wire GULO_CH3_STOPp = not1(FURY_TRIG_RSTn);
    /*_p16.GOFY*/ reg_new.ch3.GOFY_CH3_TRIGn.nor_latch(GULO_CH3_STOPp, reg_new.ch3.FOBA_CH3_TRIGp.qp_new());

    /*_p16.FUVO*/ wire FUVO_FREQ_GATE_SETn = nor2(reg_new.ch3.GUXE_NR30_AMP_ENp.qn_newB(), reg_new.KEBA_APU_RSTp_new());
    /*#p16.FAJU*/ wire FAJU_FREQ_GATE_RSTn = not1(reg_new.ch3.GYRA_TRIG_D3.qp_new());
    /*#p16.GUGU*/ reg_new.ch3.GUGU_FREQ_GATE.nand_latch(FUVO_FREQ_GATE_SETn, FAJU_FREQ_GATE_RSTn);
  }





  //----------------------------------------





  {
    /*#p18.HEFO*/ wire HEFO_FREQ_CLKn = nor2(reg_new.spu.CERY_CLK_2M.qp_new(), reg_new.ch3.GUGU_FREQ_GATE.qp_new());
    /*_p18.JUTY*/ wire JUTY_FREQ_CLKp = not1(HEFO_FREQ_CLKn);

    /*#p18.KUTU*/ reg_new.ch3.KUTU_COUNT00.dff20_clk(JUTY_FREQ_CLKp);
    /*#p18.KUPE*/ reg_new.ch3.KUPE_COUNT01.dff20_clk(reg_new.ch3.KUTU_COUNT00.qp_new());
    /*_p18.KUNU*/ reg_new.ch3.KUNU_COUNT02.dff20_clk(reg_new.ch3.KUPE_COUNT01.qp_new());
    /*_p18.KEMU*/ reg_new.ch3.KEMU_COUNT03.dff20_clk(reg_new.ch3.KUNU_COUNT02.qp_new());

    /*#p18.KYRU*/ wire KYRU_FREQ_CLKp = not1(reg_new.ch3.KEMU_COUNT03.qn_new());
    /*_p18.KYGU*/ reg_new.ch3.KYGU_COUNT04.dff20_clk(KYRU_FREQ_CLKp);
    /*_p18.KEPA*/ reg_new.ch3.KEPA_COUNT05.dff20_clk(reg_new.ch3.KYGU_COUNT04.qp_new());
    /*_p18.KAFO*/ reg_new.ch3.KAFO_COUNT06.dff20_clk(reg_new.ch3.KEPA_COUNT05.qp_new());
    /*_p18.KENO*/ reg_new.ch3.KENO_COUNT07.dff20_clk(reg_new.ch3.KAFO_COUNT06.qp_new());

    /*#p18.KESE*/ wire KESE_FREQ_CLKp = not1(reg_new.ch3.KENO_COUNT07.qn_new());
    /*_p18.KEJU*/ reg_new.ch3.KEJU_COUNT08.dff20_clk(KESE_FREQ_CLKp);
    /*_p18.KEZA*/ reg_new.ch3.KEZA_COUNT09.dff20_clk(reg_new.ch3.KEJU_COUNT08.qp_new());
    /*_p18.JAPU*/ reg_new.ch3.JAPU_COUNT10.dff20_clk(reg_new.ch3.KEZA_COUNT09.qp_new());
  }

  {
    /*#p18.HYFO*/ wire HYFO_FREQ_OVERFLOWp_new = not1(reg_new.ch3.JAPU_COUNT10.qp_new());

    /*#p18.HUPA*/ wire HUPA_SAMPLE_CLK_RSTp_old = and2(reg_old.ch3.HUNO_SAMPLE_CLKp.qp_old(), reg_old.spu.CERY_CLK_2M.qp_old());
    /*#p18.GAFU*/ wire GAFU_old = nor3(reg_old.KEBA_APU_RSTp_old(), reg_old.ch3.GARA_TRIG_D1.qp_old(), HUPA_SAMPLE_CLK_RSTp_old);
    /*#p18.HUNO*/ reg_new.ch3.HUNO_SAMPLE_CLKp.dff17(HYFO_FREQ_OVERFLOWp_new, GAFU_old, reg_old.ch3.HUNO_SAMPLE_CLKp.qn_old());

    /*#p18.HUPA*/ wire HUPA_SAMPLE_CLK_RSTp_new = and2(reg_new.ch3.HUNO_SAMPLE_CLKp.qp_new(), reg_new.spu.CERY_CLK_2M.qp_new());
    /*#p18.GAFU*/ wire GAFU_new = nor3(reg_new.KEBA_APU_RSTp_new(), reg_new.ch3.GARA_TRIG_D1.qp_new(), HUPA_SAMPLE_CLK_RSTp_new);
    /*#p18.HUNO*/ reg_new.ch3.HUNO_SAMPLE_CLKp.dff17_any(HYFO_FREQ_OVERFLOWp_new, GAFU_new, reg_old.ch3.HUNO_SAMPLE_CLKp.qn_old());
  }

  {
    /*#p18.HEMA*/ wire HEMA_SAMPLE_CLKn_new = not1(reg_new.ch3.HUNO_SAMPLE_CLKp.qp_new());
    /*#p18.GASE*/ wire GASE_SAMPLE_CLKp_new = not1(HEMA_SAMPLE_CLKn_new);
    /*#p18.DERO*/ wire DERO_SAMPLE_CLKn_new = not1(GASE_SAMPLE_CLKp_new);

    /*#p18.HERA*/ wire HERA_FREQ_RSTp_new = nor2(GASE_SAMPLE_CLKp_new, reg_new.ch3.GARA_TRIG_D1.qp_new());
    /*_p18.KYKO*/ wire KYKO_FREQ_RSTn_new = not1(HERA_FREQ_RSTp_new);
    /*_p18.JERA*/ wire JERA_FREQ_RSTn_new = not1(HERA_FREQ_RSTp_new);
    /*_p18.KASO*/ wire KASO_FREQ_RSTn_new = not1(HERA_FREQ_RSTp_new);

    /*#p18.HEFO*/ wire HEFO_FREQ_CLKn = nor2(reg_new.spu.CERY_CLK_2M.qp_new(), reg_new.ch3.GUGU_FREQ_GATE.qp_new());
    /*_p18.JUTY*/ wire JUTY_FREQ_CLKp = not1(HEFO_FREQ_CLKn);

    /*#p18.KUTU*/ reg_new.ch3.KUTU_COUNT00.dff20_any(JUTY_FREQ_CLKp,                      KYKO_FREQ_RSTn_new, reg_new.ch3.KOGA_NR33_FREQ00p.qp_newB());
    /*#p18.KUPE*/ reg_new.ch3.KUPE_COUNT01.dff20_any(reg_new.ch3.KUTU_COUNT00.qp_new(),   KYKO_FREQ_RSTn_new, reg_new.ch3.JOVY_NR33_FREQ01p.qp_newB());
    /*_p18.KUNU*/ reg_new.ch3.KUNU_COUNT02.dff20_any(reg_new.ch3.KUPE_COUNT01.qp_new(),   KYKO_FREQ_RSTn_new, reg_new.ch3.JAXA_NR33_FREQ02p.qp_newB());
    /*_p18.KEMU*/ reg_new.ch3.KEMU_COUNT03.dff20_any(reg_new.ch3.KUNU_COUNT02.qp_new(),   KYKO_FREQ_RSTn_new, reg_new.ch3.JEFE_NR33_FREQ03p.qp_newB());

    /*#p18.KYRU*/ wire KYRU_FREQ_CLKp = not1(reg_new.ch3.KEMU_COUNT03.qn_new());
    /*_p18.KYGU*/ reg_new.ch3.KYGU_COUNT04.dff20_any(KYRU_FREQ_CLKp,                      JERA_FREQ_RSTn_new, reg_new.ch3.JYPO_NR33_FREQ04p.qp_newB());
    /*_p18.KEPA*/ reg_new.ch3.KEPA_COUNT05.dff20_any(reg_new.ch3.KYGU_COUNT04.qp_new(),   JERA_FREQ_RSTn_new, reg_new.ch3.JOVE_NR33_FREQ05p.qp_newB());
    /*_p18.KAFO*/ reg_new.ch3.KAFO_COUNT06.dff20_any(reg_new.ch3.KEPA_COUNT05.qp_new(),   JERA_FREQ_RSTn_new, reg_new.ch3.KANA_NR33_FREQ06p.qp_newB());
    /*_p18.KENO*/ reg_new.ch3.KENO_COUNT07.dff20_any(reg_new.ch3.KAFO_COUNT06.qp_new(),   JERA_FREQ_RSTn_new, reg_new.ch3.KOGU_NR33_FREQ07p.qp_newB());

    /*#p18.KESE*/ wire KESE_FREQ_CLKp = not1(reg_new.ch3.KENO_COUNT07.qn_new());
    /*_p18.KEJU*/ reg_new.ch3.KEJU_COUNT08.dff20_any(KESE_FREQ_CLKp,                      KASO_FREQ_RSTn_new, reg_new.ch3.JEMO_NR34_FREQ08p.qp_newB());
    /*_p18.KEZA*/ reg_new.ch3.KEZA_COUNT09.dff20_any(reg_new.ch3.KEJU_COUNT08.qp_new(),   KASO_FREQ_RSTn_new, reg_new.ch3.JETY_NR34_FREQ09p.qp_newB());
    /*_p18.JAPU*/ reg_new.ch3.JAPU_COUNT10.dff20_any(reg_new.ch3.KEZA_COUNT09.qp_new(),   KASO_FREQ_RSTn_new, reg_new.ch3.JACY_NR34_FREQ10p.qp_newB());
  }

  {
    /*#p18.HYFO*/ wire HYFO_FREQ_OVERFLOWp_new = not1(reg_new.ch3.JAPU_COUNT10.qp_new());
    /*#p18.HUNO*/ reg_new.ch3.HUNO_SAMPLE_CLKp.dff17_clk(HYFO_FREQ_OVERFLOWp_new, reg_old.ch3.HUNO_SAMPLE_CLKp.qn_old());
  }

























  {
    /*#p18.HEMA*/ wire HEMA_SAMPLE_CLKn_old = not1(reg_old.ch3.HUNO_SAMPLE_CLKp.qp_old());
    /*#p18.GASE*/ wire GASE_SAMPLE_CLKp_old = not1(HEMA_SAMPLE_CLKn_old);
    /*#p18.DERO*/ wire DERO_SAMPLE_CLKn_old = not1(GASE_SAMPLE_CLKp_old);

    /*#p18.HEMA*/ wire HEMA_SAMPLE_CLKn_new = not1(reg_new.ch3.HUNO_SAMPLE_CLKp.qp_new());
    /*#p18.GASE*/ wire GASE_SAMPLE_CLKp_new = not1(HEMA_SAMPLE_CLKn_new);
    /*#p18.DERO*/ wire DERO_SAMPLE_CLKn_new = not1(GASE_SAMPLE_CLKp_new);


    /*#p18.ETAN*/ wire ETAN_WAVE_RST_old = nor2(reg_old.ch3.GARA_TRIG_D1.qp_old(), reg_old.ch3.FETY_WAVE_LOOP.qp_old());

    /*#p18.EFAR*/ reg_new.ch3.EFAR_WAVE_IDX0.dff17(DERO_SAMPLE_CLKn_new,                ETAN_WAVE_RST_old, reg_old.ch3.EFAR_WAVE_IDX0.qn_old());
    /*#p18.ERUS*/ reg_new.ch3.ERUS_WAVE_IDX1.dff17(reg_new.ch3.EFAR_WAVE_IDX0.qn_new(), ETAN_WAVE_RST_old, reg_old.ch3.ERUS_WAVE_IDX1.qn_old());
    /*#p18.EFUZ*/ reg_new.ch3.EFUZ_WAVE_IDX2.dff17(reg_new.ch3.ERUS_WAVE_IDX1.qn_new(), ETAN_WAVE_RST_old, reg_old.ch3.EFUZ_WAVE_IDX2.qn_old());
    /*#p18.EXEL*/ reg_new.ch3.EXEL_WAVE_IDX3.dff17(reg_new.ch3.EFUZ_WAVE_IDX2.qn_new(), ETAN_WAVE_RST_old, reg_old.ch3.EXEL_WAVE_IDX3.qn_old());
    /*#p18.EFAL*/ reg_new.ch3.EFAL_WAVE_IDX4.dff17(reg_new.ch3.EXEL_WAVE_IDX3.qn_new(), ETAN_WAVE_RST_old, reg_old.ch3.EFAL_WAVE_IDX4.qn_old());


    /*#p18.FOTO*/ wire FOTO_old = and2(reg_old.ch3.FETY_WAVE_LOOP.qp_old(), GASE_SAMPLE_CLKp_old);
    /*#p18.GYRY*/ wire GYRY_LOOP_RST_old = nor3(reg_old.KEBA_APU_RSTp_old(), reg_old.ch3.GARA_TRIG_D1.qp_old(), FOTO_old);
    /*#p18.FETY*/ reg_new.ch3.FETY_WAVE_LOOP.dff17(reg_new.ch3.EFAL_WAVE_IDX4.qn_new(), GYRY_LOOP_RST_old, reg_old.ch3.FETY_WAVE_LOOP.qn_old());

    /*#p18.FOTO*/ wire FOTO_new = and2(reg_new.ch3.FETY_WAVE_LOOP.qp_new(), GASE_SAMPLE_CLKp_new);
    /*#p18.GYRY*/ wire GYRY_LOOP_RST_new = nor3(reg_new.KEBA_APU_RSTp_new(), reg_new.ch3.GARA_TRIG_D1.qp_new(), FOTO_new);
    /*#p18.FETY*/ reg_new.ch3.FETY_WAVE_LOOP.dff17_any(reg_new.ch3.EFAL_WAVE_IDX4.qn_new(), GYRY_LOOP_RST_new, reg_old.ch3.FETY_WAVE_LOOP.qn_old());

    /*#p18.ETAN*/ wire ETAN_WAVE_RST_new = nor2(reg_new.ch3.GARA_TRIG_D1.qp_new(), reg_new.ch3.FETY_WAVE_LOOP.qp_new());

    /*#p18.EFAR*/ reg_new.ch3.EFAR_WAVE_IDX0.dff17_any(DERO_SAMPLE_CLKn_new,                ETAN_WAVE_RST_new, reg_old.ch3.EFAR_WAVE_IDX0.qn_old());
    /*#p18.ERUS*/ reg_new.ch3.ERUS_WAVE_IDX1.dff17_any(reg_new.ch3.EFAR_WAVE_IDX0.qn_new(), ETAN_WAVE_RST_new, reg_old.ch3.ERUS_WAVE_IDX1.qn_old());
    /*#p18.EFUZ*/ reg_new.ch3.EFUZ_WAVE_IDX2.dff17_any(reg_new.ch3.ERUS_WAVE_IDX1.qn_new(), ETAN_WAVE_RST_new, reg_old.ch3.EFUZ_WAVE_IDX2.qn_old());
    /*#p18.EXEL*/ reg_new.ch3.EXEL_WAVE_IDX3.dff17_any(reg_new.ch3.EFUZ_WAVE_IDX2.qn_new(), ETAN_WAVE_RST_new, reg_old.ch3.EXEL_WAVE_IDX3.qn_old());
    /*#p18.EFAL*/ reg_new.ch3.EFAL_WAVE_IDX4.dff17_any(reg_new.ch3.EXEL_WAVE_IDX3.qn_new(), ETAN_WAVE_RST_new, reg_old.ch3.EFAL_WAVE_IDX4.qn_old());







    /*#p17.ARUC*/ wire ARUC_CLK = not1(reg_new.sys_clk.AMUK_xBxDxFxH());
    /*#p17.COZY*/ wire COZY_CLK = not1(reg_new.sys_clk.AMUK_xBxDxFxH());
    /*#p17.BUSA*/ reg_new.ch3.BUSA_WAVE_CLK_D1.dff17(reg_new.sys_clk.AMUK_xBxDxFxH(), reg_new.BAMA_APU_RSTn_new(), GASE_SAMPLE_CLKp_old);
    /*#p17.BANO*/ reg_new.ch3.BANO_WAVE_CLK_D2.dff17(COZY_CLK,                        reg_new.BAMA_APU_RSTn_new(), reg_old.ch3.BUSA_WAVE_CLK_D1.qp_old());
    /*#p17.AZUS*/ reg_new.ch3.AZUS_WAVE_CLK_D3.dff17(reg_new.sys_clk.AMUK_xBxDxFxH(), reg_new.BAMA_APU_RSTn_new(), reg_old.ch3.BANO_WAVE_CLK_D2.qp_old());
    /*_p17.AZET*/ reg_new.ch3.AZET_WAVE_CLK_D4.dff17(ARUC_CLK,                        reg_new.BAMA_APU_RSTn_new(), reg_old.ch3.AZUS_WAVE_CLK_D3.qp_old());
  }

  //----------
  // Wave ram interface

  // WAVE RAM ADDR
  // ---- COKA_CH3_ACTIVEp
  // ---- AGYL_WAVE_A1
  // ---- BOLE_WAVE_A0
  // ---- AXOL_WAVE_A3
  // ---- AFUM_WAVE_A2

  // WAVE RAM CTRL
  // ---- AMYT_WAVE_WRn    this has to be write
  // ---- ALER_WAVE_CSn    Must be CS, it can be set active by cpu write or cpu read
  // ---- ATOK_WAVE_OEn    Must be OE, can be set by cpu read or delayed wave clock

  {
    /*_p18.BOLE*/ wire BOLE_WAVE_A0 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.ERUS_WAVE_IDX1.qp_new(), reg_new.cpu_abus.BUS_CPU_A00p.out_new());
    /*_p18.AGYL*/ wire AGYL_WAVE_A1 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.EFUZ_WAVE_IDX2.qp_new(), reg_new.cpu_abus.BUS_CPU_A01p.out_new());
    /*_p18.AFUM*/ wire AFUM_WAVE_A2 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.EXEL_WAVE_IDX3.qp_new(), reg_new.cpu_abus.BUS_CPU_A02p.out_new());
    /*_p18.AXOL*/ wire AXOL_WAVE_A3 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.EFAL_WAVE_IDX4.qp_new(), reg_new.cpu_abus.BUS_CPU_A03p.out_new());

    /*#p17.BENA*/ wire BENA_CPU_WAVE_RDn = nand2(reg_new.cpu_signals.BOKE_CPU_RDp(), reg_new.cpu_abus.BARO_ADDR_FF3Xp());
    /*#p17.CAZU*/ wire CAZU_CPU_WAVE_RDp = not1(BENA_CPU_WAVE_RDn);

    // wave ram control line 1
    /*_p17.BYZA*/ wire BYZA_WAVE_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.BARO_ADDR_FF3Xp());
    /*#p17.AMYT*/ wire AMYT_WAVE_WRn = not1(BYZA_WAVE_WRp);

    // wave ram control line 2
    /*#p17.BOXO*/ wire BOXO_WAVE_CSn = nor2(reg_new.ch3.AZUS_WAVE_CLK_D3.qp_new(), reg_new.ch3.AZET_WAVE_CLK_D4.qp_new()); // pulse generator
    /*#p17.BORU*/ wire BORU_WAVE_CSp = not1(BOXO_WAVE_CSn);

    /*#p17.BETA*/ wire BETA_WAVE_CSp = or3(BYZA_WAVE_WRp, CAZU_CPU_WAVE_RDp, reg_new.sys_clk.BORY_ABxxxxxH());
    /*#p17.AZOR*/ wire AZOR_WAVE_CSn = not1(BETA_WAVE_CSp);
    /*#p17.BUKU*/ wire BUKU_WAVE_CSp = not1(AZOR_WAVE_CSn);

    /*#p17.ATUR*/ wire ATUR_WAVE_CSp = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), BORU_WAVE_CSp, BUKU_WAVE_CSp);
    /*#p17.ALER*/ wire ALER_WAVE_CSn = not1(ATUR_WAVE_CSp);

    // wave ram control line 3
    /*#p17.BUTU*/ wire BUTU_SAMPLE_CLKp = not1(reg_new.ch3.AZUS_WAVE_CLK_D3.qn_new());
    /*#p18.BENO*/ wire BENO_WAVE_OEp = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), BUTU_SAMPLE_CLKp, CAZU_CPU_WAVE_RDp);
    /*#p18.ATOK*/ wire ATOK_WAVE_OEn = not1(BENO_WAVE_OEp);


    {
      auto wave_addr = (bit(AXOL_WAVE_A3) << 3) | (bit(AFUM_WAVE_A2) << 2) | (bit(AGYL_WAVE_A1) << 1) | (bit(BOLE_WAVE_A0) << 0);

      if (!bit(ALER_WAVE_CSn)) {
        if (!bit(AMYT_WAVE_WRn)) {
          wave_ram[wave_addr] = bit_pack(reg_old.cpu_dbus);
        }
        if (!bit(ATOK_WAVE_OEn)) {
          uint8_t data = wave_ram[wave_addr];

          triwire tri0 = tri_pp(1, bit(data, 0));
          triwire tri1 = tri_pp(1, bit(data, 1));
          triwire tri2 = tri_pp(1, bit(data, 2));
          triwire tri3 = tri_pp(1, bit(data, 3));
          triwire tri4 = tri_pp(1, bit(data, 4));
          triwire tri5 = tri_pp(1, bit(data, 5));
          triwire tri6 = tri_pp(1, bit(data, 6));
          triwire tri7 = tri_pp(1, bit(data, 7));

          reg_new.wave_dbus.BUS_WAVE_D00.tri_bus(tri0);
          reg_new.wave_dbus.BUS_WAVE_D01.tri_bus(tri1);
          reg_new.wave_dbus.BUS_WAVE_D02.tri_bus(tri2);
          reg_new.wave_dbus.BUS_WAVE_D03.tri_bus(tri3);
          reg_new.wave_dbus.BUS_WAVE_D04.tri_bus(tri4);
          reg_new.wave_dbus.BUS_WAVE_D05.tri_bus(tri5);
          reg_new.wave_dbus.BUS_WAVE_D06.tri_bus(tri6);
          reg_new.wave_dbus.BUS_WAVE_D07.tri_bus(tri7);
        }
      }
    }








    // wave ram -> sample reg
    /*#p17.BEKA*/ wire BEKA_SAMPLE_CLKn = not1(BUTU_SAMPLE_CLKp);
    /*#p17.COJU*/ wire COJU_SAMPLE_CLKn = not1(BUTU_SAMPLE_CLKp);
    /*#p17.BAJA*/ wire BAJA_SAMPLE_CLKn = not1(BUTU_SAMPLE_CLKp);
    /*#p17.BUFE*/ wire BUFE_SAMPLE_CLKn = not1(BUTU_SAMPLE_CLKp);

    /*_p17.CYFO*/ reg_new.ch3.CYFO_SAMPLE0p.dff9b(BEKA_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D00.qp_old());
    /*_p17.CESY*/ reg_new.ch3.CESY_SAMPLE1p.dff9b(BUFE_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D01.qp_old());
    /*_p17.BUDY*/ reg_new.ch3.BUDY_SAMPLE2p.dff9b(BAJA_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D02.qp_old());
    /*_p17.BEGU*/ reg_new.ch3.BEGU_SAMPLE3p.dff9b(COJU_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D03.qp_old());
    /*_p17.CUVO*/ reg_new.ch3.CUVO_SAMPLE4p.dff9b(BEKA_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D04.qp_old());
    /*_p17.CEVO*/ reg_new.ch3.CEVO_SAMPLE5p.dff9b(BUFE_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D05.qp_old());
    /*_p17.BORA*/ reg_new.ch3.BORA_SAMPLE6p.dff9b(BAJA_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D06.qp_old());
    /*_p17.BEPA*/ reg_new.ch3.BEPA_SAMPLE7p.dff9b(COJU_SAMPLE_CLKn, reg_new.ACOR_APU_RSTn_new(), reg_old.wave_dbus.BUS_WAVE_D07.qp_old());
  }

  {
    // wave ram -> cpu bus

    /*#p17.BENA*/ wire BENA_CPU_WAVE_RDn = nand2(reg_new.cpu_signals.BOKE_CPU_RDp(), reg_new.cpu_abus.BARO_ADDR_FF3Xp());
    /*#p17.CAZU*/ wire CAZU_CPU_WAVE_RDp = not1(BENA_CPU_WAVE_RDn);

    /*#p17.CUGO*/ wire CUGO_WAVE_D0n = not1(reg_new.wave_dbus.BUS_WAVE_D00.qp_new());
    /*#p17.CEGU*/ wire CEGU_WAVE_D1n = not1(reg_new.wave_dbus.BUS_WAVE_D01.qp_new());
    /*#p17.ATEC*/ wire ATEC_WAVE_D2n = not1(reg_new.wave_dbus.BUS_WAVE_D02.qp_new());
    /*#p17.ADOK*/ wire ADOK_WAVE_D3n = not1(reg_new.wave_dbus.BUS_WAVE_D03.qp_new());
    /*#p17.BACA*/ wire BACA_WAVE_D4n = not1(reg_new.wave_dbus.BUS_WAVE_D04.qp_new());
    /*#p17.BERO*/ wire BERO_WAVE_D5n = not1(reg_new.wave_dbus.BUS_WAVE_D05.qp_new());
    /*#p17.CUTO*/ wire CUTO_WAVE_D6n = not1(reg_new.wave_dbus.BUS_WAVE_D06.qp_new());
    /*#p17.AKAF*/ wire AKAF_WAVE_D7n = not1(reg_new.wave_dbus.BUS_WAVE_D07.qp_new());

    /*#p17.DUGU*/ triwire DUGU = tri6_pn(CAZU_CPU_WAVE_RDp, CUGO_WAVE_D0n);
    /*#p17.DESY*/ triwire DESY = tri6_pn(CAZU_CPU_WAVE_RDp, CEGU_WAVE_D1n);
    /*#p17.BATY*/ triwire BATY = tri6_pn(CAZU_CPU_WAVE_RDp, ATEC_WAVE_D2n);
    /*#p17.BADE*/ triwire BADE = tri6_pn(CAZU_CPU_WAVE_RDp, ADOK_WAVE_D3n);
    /*#p17.BUNE*/ triwire BUNE = tri6_pn(CAZU_CPU_WAVE_RDp, BACA_WAVE_D4n);
    /*#p17.BAVA*/ triwire BAVA = tri6_pn(CAZU_CPU_WAVE_RDp, BERO_WAVE_D5n);
    /*#p17.DESA*/ triwire DESA = tri6_pn(CAZU_CPU_WAVE_RDp, CUTO_WAVE_D6n);
    /*#p17.BEZU*/ triwire BEZU = tri6_pn(CAZU_CPU_WAVE_RDp, AKAF_WAVE_D7n);

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(DUGU);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(DESY);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(BATY);
    reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(BADE);
    reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(BUNE);
    reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(BAVA);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(DESA);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(BEZU);
  }



  /*#p16.DERY*/ wire DERY_FF1B_WRn_new = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DUSA_ADDR_FF1Bp());
  /*#p16.GAJY*/ wire GAJY_FF1B_WRp_new = not1(DERY_FF1B_WRn_new);
  /*#p16.EMUT*/ wire EMUT_FF1B_WRp_new = not1(DERY_FF1B_WRn_new);
  /*#p16.GETO*/ wire GETO_FF1B_WRp_new = not1(DERY_FF1B_WRn_new);

  /*#p16.FOVO*/ wire FOVO_FF1E_WRn_new = nand2(reg_new.ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.DUGO_ADDR_FF1Ep());
  /*_p16.HOTO*/ reg_new.ch3.HOTO_NR34_LENENp.dff9b(FOVO_FF1E_WRn_new, reg_new.HEKY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());






  // using FEXU_old to break the loop
  {
    /*#p18.GEPY*/ wire GEPY_LEN_CLKp_new = nor3(reg_old.ch3.FEXU_LEN_DONEp.qp_old(), reg_new.spu.BUFY_CLK_256n(), reg_new.ch3.HOTO_NR34_LENENp.qn_newB()); // fexu/hoto polarity seems wrong
    /*#p18.GENU*/ wire GENU_LEN_CLKn_new = not1(GEPY_LEN_CLKp_new);

    probe(3, "GAJY_FF1B_WRp_new", GAJY_FF1B_WRp_new);

    /*#p18.GEVO*/ reg_new.ch3.GEVO_NR31_LEN0p.dff20(GENU_LEN_CLKn_new,                    GAJY_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D00p.out_new());
    /*_p18.FORY*/ reg_new.ch3.FORY_NR31_LEN1p.dff20(reg_new.ch3.GEVO_NR31_LEN0p.qp_new(), GAJY_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D01p.out_new());
    /*_p18.GATU*/ reg_new.ch3.GATU_NR31_LEN2p.dff20(reg_new.ch3.FORY_NR31_LEN1p.qp_new(), GAJY_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D02p.out_new());
    /*_p18.GAPO*/ reg_new.ch3.GAPO_NR31_LEN3p.dff20(reg_new.ch3.GATU_NR31_LEN2p.qp_new(), GAJY_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D03p.out_new());

    /*#p18.FALU*/ wire FALU_LEN_CLKp = not1(reg_new.ch3.GAPO_NR31_LEN3p.qn_new());
    /*_p18.GEMO*/ reg_new.ch3.GEMO_NR31_LEN4p.dff20(FALU_LEN_CLKp,                        EMUT_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D04p.out_new());
    /*_p18.FORO*/ reg_new.ch3.FORO_NR31_LEN5p.dff20(reg_new.ch3.GEMO_NR31_LEN4p.qp_new(), EMUT_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D05p.out_new());
    /*_p18.FAVE*/ reg_new.ch3.FAVE_NR31_LEN6p.dff20(reg_new.ch3.FORO_NR31_LEN5p.qp_new(), EMUT_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D06p.out_new());
    /*_p18.FYRU*/ reg_new.ch3.FYRU_NR31_LEN7p.dff20(reg_new.ch3.FAVE_NR31_LEN6p.qp_new(), EMUT_FF1B_WRp_new, reg_new.cpu_dbus.BUS_CPU_D07p.out_new());
  }

  /*#p18.GUDA*/ wire GUDA_LEN_DONE_RSTn = nor3(GETO_FF1B_WRp_new, reg_new.KEBA_APU_RSTp_new(), reg_new.ch3.GARA_TRIG_D1.qp_new());
  /*_p18.FEXU*/ reg_new.ch3.FEXU_LEN_DONEp.dff17(reg_new.ch3.FYRU_NR31_LEN7p.qn_new(), GUDA_LEN_DONE_RSTn, reg_old.ch3.FEXU_LEN_DONEp.qn_old());











  {  
    /*#p18.GEDO*/ wire GEDO_LEN_DONEp = and2(reg_new.ch3.FEXU_LEN_DONEp.qp_new(), reg_new.ch3.HOTO_NR34_LENENp.qp_newB()); // schematic had BUFY instead of FEXU?
    /*#p18.FYGO*/ wire FYGO_STOPp = or3(reg_new.KEBA_APU_RSTp_new(), GEDO_LEN_DONEp, reg_new.ch3.GUXE_NR30_AMP_ENp.qn_newB());
    /*#p18.FOZU*/ reg_new.ch3.FOZU_CH3_ACTIVEp.nor_latch(reg_new.ch3.GARA_TRIG_D1.qp_new(), FYGO_STOPp);
  }











  {
    /*_p16.FASY*/ wire FASY_FF1A_RDn = nand2(reg_new.cpu_abus.EMOR_ADDR_FF1Ap(), reg_new.cpu_signals.GAXO_CPU_RDp());
    /*_p16.FEVO*/ wire FEVO_CH3_AMP_ENn  = not1(reg_new.ch3.GUXE_NR30_AMP_ENp.qp_newB());
    /*_p16.GEKO*/ triwire FEVO_NR30_D07p = tri6_nn(FASY_FF1A_RDn, FEVO_CH3_AMP_ENn);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(FEVO_NR30_D07p);
  }

  {
    /*_p16.HENU*/ wire HENU_FF1C_RDn = nand2(reg_new.cpu_abus.GEFO_ADDR_FF1Cp(), reg_new.cpu_signals.JOTU_CPU_RDp());
    /*_p16.HAMU*/ triwire HAMU_D05p = tri6_nn(HENU_FF1C_RDn, reg_new.ch3.HUKY_NR32_VOL0p.qn_newB());
    /*_p16.HAMU*/ triwire HUCO_D06p = tri6_nn(HENU_FF1C_RDn, reg_new.ch3.HODY_NR32_VOL1p.qn_newB());

    reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(HAMU_D05p);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HUCO_D06p);
  }

  {
    /*_p16.GAWA*/ wire GAWA_FF1E_RDn = nand2(reg_new.cpu_abus.DUGO_ADDR_FF1Ep(), reg_new.cpu_signals.GORY_CPU_RDp()); // polarity?
    /*_p16.HACA*/ triwire HACA_D06p = tri6_nn(GAWA_FF1E_RDn, reg_new.ch3.HOTO_NR34_LENENp.qn_newB());
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HACA_D06p);
  }

#if 0

  {
    /*_p16.HENU*/ wire HENU_FF1C_RDn = nand2(reg_new.cpu_abus.GEFO_ADDR_FF1Cp(), reg_new.cpu_signals.JOTU_CPU_RDp());
    /*_p18.HONY*/ wire HONY_DBG_FF1C_RD = and3(reg_new.EDEK_NR52_DBG_APUp(), reg_new.cpu_signals.JECO_CPU_RDp(), reg_new.cpu_abus.GEFO_ADDR_FF1Cp());
    /*_p18.GENO*/ wire GENO_DBG_FF1C_RDn = not1(HONY_DBG_FF1C_RD);

    if (HENU_FF1C_RDn) set_data(
      /*_p18.FAPY*/ 0,
      /*_p18.FARO*/ 0,
      /*_p18.FOTE*/ 0,
      /*_p18.FANA*/ 0,
      /*_p18.FERA*/ 0,
      /*_p16.HAMU*/ HUKY_NR32_VOL0p,
      /*_p16.HUCO*/ HODY_NR32_VOL1p
    );
  }

  /*_p16.GUTE*/ wire GUTE_DBG_FF1D_RDn = nor2(reg_new.cpu_abus.HOXA_ADDR_FF1Dn(), reg_new.EGAD_CPU_RDn_DBGn());
  /*_p16.HOVO*/ wire HOVO_DBG_FF1D_RD = not1(GUTE_DBG_FF1D_RDn);

  if (HOVO_DBG_FF1D_RD) set_data(
    /*_p16.JOFO*/ KUTU_COUNT00,
    /*_p16.KAFU*/ KUPE_COUNT01,
    /*_p16.KESY*/ KUNU_COUNT02,
    /*_p16.JUDE*/ KEMU_COUNT03,
    /*_p16.JUKE*/ KYGU_COUNT04,
    /*_p16.JEZA*/ KEPA_COUNT05,
    /*_p16.KORA*/ KAFO_COUNT06,
    /*_p16.KAMY*/ KENO_COUNT07
  );

    if (FUVA_FF1E_RDb) set_data(
    /*_p16.FUVA*/ wire FUVA_FF1E_RDn = or2(reg_new.cpu_abus.GUNU_ADDR_FF1En(), reg_new.EGAD_CPU_RDn_DBGn());

      /*_p16.JUVY*/ KEJU_COUNT08,
      /*_p16.JURA*/ KEZA_COUNT09,
      /*_p16.HUFO*/ JAPU_FREQ_10,
      /*_p16.HACA*/ HOTO_NR34_STOP
    );

    /*_p16.GAWA*/ wire GAWA_FF1E_RDn = nand2(reg_new.cpu_abus.DUGO_ADDR_FF1Ep(), reg_new.cpu_signals.GORY_CPU_RDp()); // polarity?
    if (GAWA_FF1E_RDa) set_data(
      /*_p16.HACA*/ HOTO_NR34_STOP
    );
#endif

}

#endif