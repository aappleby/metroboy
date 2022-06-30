#include "GateBoySPU.h"

#include "GateBoyLib/GateBoyState.h"

void GateBoySPU::tick_ch3(const GateBoyState& reg_old, GateBoyState& reg_new) {

  /*#p17.BENA*/ wire BENA_CPU_WAVE_RDp = nand2(reg_new.cpu_signals.BOKE_CPU_RDp(), reg_new.cpu_abus.BARO_ADDR_FF3Xp());
  /*#p17.CAZU*/ wire CAZU_CPU_WAVE_RDn = not1(BENA_CPU_WAVE_RDp);

  {
    /*#p16.FURY*/ wire FURY_TRIG_RSTn = nor2(reg_new.KEBA_APU_RSTp(), reg_new.ch3.GYTA_TRIG_D2.qp_new());
    /*#p16.GULO*/ wire GULO_CH3_STOPp = not1(FURY_TRIG_RSTn);
    /*_p16.GOFY*/ reg_new.ch3.GOFY_CH3_TRIGn.nor_latch(GULO_CH3_STOPp, reg_new.ch3.FOBA_CH3_TRIGp.qp_new());

    /*#p16.GARA*/ reg_new.ch3.GARA_TRIG_D1.dff17(FABO_CLK_xxCDxxGH(),      FURY_TRIG_RSTn,  reg_new.ch3.GOFY_CH3_TRIGn.qn_new());
    /*#p16.GYTA*/ reg_new.ch3.GYTA_TRIG_D2.dff17(FABO_CLK_xxCDxxGH(),      reg_new.GAZE_APU_RSTn(), reg_new.ch3.GARA_TRIG_D1.qp_new());
    /*#p16.GYRA*/ reg_new.ch3.GYRA_TRIG_D3.dff17(CERY_CLK_2M.qp_new(), reg_new.GAZE_APU_RSTn(), reg_new.ch3.GYTA_TRIG_D2.qp_new());
  }

  {
    /*#p18.HYFO*/ wire HYFO_FREQ_OVERFLOWp = not1(reg_new.ch3.JAPU_FREQ_10.qp_new());
    /*#p18.HUPA*/ wire HUPA_SAMPLE_CLK_RSTp = and2(reg_new.ch3.HUNO_SAMPLE_CLKp.qp_new(), CERY_CLK_2M.qp_new());
    /*#p18.GAFU*/ wire GAFU = nor3(reg_new.KEBA_APU_RSTp(), reg_new.ch3.GARA_TRIG_D1.qp_new(), HUPA_SAMPLE_CLK_RSTp);
    /*#p18.HUNO*/ reg_new.ch3.HUNO_SAMPLE_CLKp.dff17(HYFO_FREQ_OVERFLOWp, GAFU, reg_new.ch3.HUNO_SAMPLE_CLKp.qn_old());
    /*#p18.HEMA*/ wire HEMA_SAMPLE_CLKn = not1(reg_new.ch3.HUNO_SAMPLE_CLKp.qp_new());
    /*#p18.GASE*/ wire GASE_SAMPLE_CLKp = not1(HEMA_SAMPLE_CLKn);
    /*#p18.DERO*/ wire DERO_SAMPLE_CLKn = not1(GASE_SAMPLE_CLKp);

    /*_p16.FUVO*/ wire FUVO_FREQ_GATE_SETn = nor2(reg_new.ch3.GUXE_CH3_AMP_ENn.qp_new(), reg_new.KEBA_APU_RSTp());
    /*#p16.FAJU*/ wire FAJU_FREQ_GATE_RSTn = not1(reg_new.ch3.GYRA_TRIG_D3.qp_new());
    /*#p16.GUGU*/ reg_new.ch3.GUGU_FREQ_GATE.nand_latch(FUVO_FREQ_GATE_SETn, FAJU_FREQ_GATE_RSTn);

    /*#p18.HERA*/ wire HERA_FREQ_RSTp = nor2(GASE_SAMPLE_CLKp, reg_new.ch3.GARA_TRIG_D1.qp_new());
    /*_p18.KYKO*/ wire KYKO_FREQ_RSTn = not1(HERA_FREQ_RSTp);
    /*_p18.JERA*/ wire JERA_FREQ_RSTn = not1(HERA_FREQ_RSTp);
    /*_p18.KASO*/ wire KASO_FREQ_RSTn = not1(HERA_FREQ_RSTp);

    /*#p18.HEFO*/ wire HEFO_FREQ_CLKn = nor2(CERY_CLK_2M.qp_new(), reg_new.ch3.GUGU_FREQ_GATE.qp_new());
    /*_p18.JUTY*/ wire JUTY_FREQ_CLKp = not1(HEFO_FREQ_CLKn);
    /*#p18.KUTU*/ reg_new.ch3.KUTU_FREQ_00.dff20(JUTY_FREQ_CLKp,              KYKO_FREQ_RSTn, reg_new.ch3.KOGA_NR33_FREQ00.qn_old());
    /*#p18.KUPE*/ reg_new.ch3.KUPE_FREQ_01.dff20(reg_new.ch3.KUTU_FREQ_00.qp_new(),   KYKO_FREQ_RSTn, reg_new.ch3.JOVY_NR33_FREQ01.qn_old());
    /*_p18.KUNU*/ reg_new.ch3.KUNU_FREQ_02.dff20(reg_new.ch3.KUPE_FREQ_01.qp_new(),   KYKO_FREQ_RSTn, reg_new.ch3.JAXA_NR33_FREQ02.qn_old());
    /*_p18.KEMU*/ reg_new.ch3.KEMU_FREQ_03.dff20(reg_new.ch3.KUNU_FREQ_02.qp_new(),   KYKO_FREQ_RSTn, reg_new.ch3.JEFE_NR33_FREQ03.qn_old());

    /*#p18.KYRU*/ wire KYRU_FREQ_CLKp = not1(reg_new.ch3.KEMU_FREQ_03.qn_new());
    /*_p18.KYGU*/ reg_new.ch3.KYGU_FREQ_04.dff20(KYRU_FREQ_CLKp,          JERA_FREQ_RSTn, reg_new.ch3.JYPO_NR33_FREQ04.qn_old());
    /*_p18.KEPA*/ reg_new.ch3.KEPA_FREQ_05.dff20(reg_new.ch3.KYGU_FREQ_04.qp_new(),   JERA_FREQ_RSTn, reg_new.ch3.JOVE_NR33_FREQ05.qn_old());
    /*_p18.KAFO*/ reg_new.ch3.KAFO_FREQ_06.dff20(reg_new.ch3.KEPA_FREQ_05.qp_new(),   JERA_FREQ_RSTn, reg_new.ch3.KANA_NR33_FREQ06.qn_old());
    /*_p18.KENO*/ reg_new.ch3.KENO_FREQ_07.dff20(reg_new.ch3.KAFO_FREQ_06.qp_new(),   JERA_FREQ_RSTn, reg_new.ch3.KOGU_NR33_FREQ07.qn_old());

    /*#p18.KESE*/ wire KESE_FREQ_CLKp = not1(reg_new.ch3.KENO_FREQ_07.qn_new());
    /*_p18.KEJU*/ reg_new.ch3.KEJU_FREQ_08.dff20(KESE_FREQ_CLKp,          KASO_FREQ_RSTn, reg_new.ch3.JEMO_NR34_FREQ08.qn_old());
    /*_p18.KEZA*/ reg_new.ch3.KEZA_FREQ_09.dff20(reg_new.ch3.KEJU_FREQ_08.qp_new(),   KASO_FREQ_RSTn, reg_new.ch3.JETY_NR34_FREQ09.qn_old());
    /*_p18.JAPU*/ reg_new.ch3.JAPU_FREQ_10.dff20(reg_new.ch3.KEZA_FREQ_09.qp_new(),   KASO_FREQ_RSTn, reg_new.ch3.JACY_NR34_FREQ10.qn_old());

    /*#p18.ETAN*/ wire ETAN_WAVE_RST = or2(reg_new.ch3.GARA_TRIG_D1.qp_new(), reg_new.ch3.FETY_WAVE_LOOP.qp_new());

    /*#p18.EFAR*/ reg_new.ch3.EFAR_WAVE_IDX0.dff17(DERO_SAMPLE_CLKn,            ETAN_WAVE_RST, reg_new.ch3.EFAR_WAVE_IDX0.qn_old());
    /*#p18.ERUS*/ reg_new.ch3.ERUS_WAVE_IDX1.dff17(reg_new.ch3.EFAR_WAVE_IDX0.qn_new(), ETAN_WAVE_RST, reg_new.ch3.ERUS_WAVE_IDX1.qn_old());
    /*#p18.EFUZ*/ reg_new.ch3.EFUZ_WAVE_IDX2.dff17(reg_new.ch3.ERUS_WAVE_IDX1.qn_new(), ETAN_WAVE_RST, reg_new.ch3.EFUZ_WAVE_IDX2.qn_old());
    /*#p18.EXEL*/ reg_new.ch3.EXEL_WAVE_IDX3.dff17(reg_new.ch3.EFUZ_WAVE_IDX2.qn_new(), ETAN_WAVE_RST, reg_new.ch3.EXEL_WAVE_IDX3.qn_old());
    /*#p18.EFAL*/ reg_new.ch3.EFAL_WAVE_IDX4.dff17(reg_new.ch3.EXEL_WAVE_IDX3.qn_new(), ETAN_WAVE_RST, reg_new.ch3.EFAL_WAVE_IDX4.qn_old());
   
    /*#p18.FOTO*/ wire FOTO = and2(reg_new.ch3.FETY_WAVE_LOOP.qp_new(), GASE_SAMPLE_CLKp);
    /*#p18.GYRY*/ wire GYRY_LOOP_RST = nor3(reg_new.KEBA_APU_RSTp(), reg_new.ch3.GARA_TRIG_D1.qp_new(), FOTO);
    /*#p18.FETY*/ reg_new.ch3.FETY_WAVE_LOOP.dff17(reg_new.ch3.EFAL_WAVE_IDX4.qn_new(), GYRY_LOOP_RST, reg_new.ch3.FETY_WAVE_LOOP.qn_old());

    // Wave read pulse generator and read reg clock
    /*#p17.ARUC*/ wire ARUC_CLK = not1(reg_new.sys_clk.AMUK_xBxDxFxH());
    /*#p17.COZY*/ wire COZY_CLK = not1(reg_new.sys_clk.AMUK_xBxDxFxH());
    /*#p17.BUSA*/ reg_new.ch3.BUSA_WAVE_CLK_D1.dff17(reg_new.sys_clk.AMUK_xBxDxFxH(), reg_new.BAMA_APU_RSTn(), GASE_SAMPLE_CLKp);
    /*#p17.BANO*/ reg_new.ch3.BANO_WAVE_CLK_D2.dff17(COZY_CLK,                        reg_new.BAMA_APU_RSTn(), reg_new.ch3.BUSA_WAVE_CLK_D1.qp_old());
    /*#p17.AZUS*/ reg_new.ch3.AZUS_WAVE_CLK_D3.dff17(reg_new.sys_clk.AMUK_xBxDxFxH(), reg_new.BAMA_APU_RSTn(), reg_new.ch3.BANO_WAVE_CLK_D2.qp_old());
    /*_p17.AZET*/ reg_new.ch3.AZET_WAVE_CLK_D4.dff17(ARUC_CLK,                        reg_new.BAMA_APU_RSTn(), reg_new.ch3.AZUS_WAVE_CLK_D3.qp_old());
  }

  //----------
  // Channel active signal

  {  
    /*#p18.GEDO*/ wire GEDO_LEN_DONEp = and2(reg_new.ch3.FEXU_LEN_DONEp.qp_new(), reg_new.ch3.HOTO_NR34_LENEN.qn_new()); // schematic had BUFY instead of FEXU?
    /*#p18.FYGO*/ wire FYGO_STOPp = or3(reg_new.KEBA_APU_RSTp(), GEDO_LEN_DONEp, reg_new.ch3.GUXE_CH3_AMP_ENn.qp_new());
    /*#p18.FOZU*/ reg_new.ch3.FOZU_CH3_ACTIVEp.nor_latch(reg_new.ch3.GARA_TRIG_D1.qp_new(), FYGO_STOPp);

    /*#p18.EZAS*/ wire EZAS_CH3_ACTIVEn = not1(reg_new.ch3.FOZU_CH3_ACTIVEp.qp_new());
    /*#p18.DORU*/ wire DORU_CH3_ACTIVEp = not1(EZAS_CH3_ACTIVEn);
    /*#p18.DAVO*/ reg_new.ch3.DAVO_CH3_ACTIVEp.dff17(AJER_CLK_2M.qp_new(), reg_new.CALU_APU_RSTn(), DORU_CH3_ACTIVEp);
  }

  //----------
  // Wave ram control signals

  {
    // wave ram control line 1
    /*_p17.BYZA*/ wire BYZA_WAVE_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.BARO_ADDR_FF3Xp());
    /*#p17.AMYT*/ wire AMYT_WAVE_WRn = not1(BYZA_WAVE_WRp); // to wave ram?

    /*#p17.BOXO*/ wire BOXO_TRIG = nor2(reg_new.ch3.AZUS_WAVE_CLK_D3.qp_new(), reg_new.ch3.AZET_WAVE_CLK_D4.qp_new()); // pulse generator
    /*#p17.BORU*/ wire BORU_TRIG = not1(BOXO_TRIG);

    /*#p17.BETA*/ wire BETA_WAVE_RAM_CS = or3(BYZA_WAVE_WRp, CAZU_CPU_WAVE_RDn, reg_new.sys_clk.BORY_ABxxxxxH());
    /*#p17.AZOR*/ wire AZOR_WAVE_RAM_CS = not1(BETA_WAVE_RAM_CS);
    /*#p17.BUKU*/ wire BUKU_WAVE_RAM_CS = not1(AZOR_WAVE_RAM_CS);

    /*#p17.ATUR*/ wire ATUR_WAVE_RAM_CS = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), BORU_TRIG, BUKU_WAVE_RAM_CS);  // to wave ram? CS?
    /*#p17.ALER*/ wire ALER_WAVE_RAM_CS = not1(ATUR_WAVE_RAM_CS);
  }

  //----------
  // Wave ram address

  {
    /*_p18.BOLE*/ wire BOLE_WAVE_A0 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.ERUS_WAVE_IDX1.qp_new(), reg_new.cpu_abus.BUS_CPU_A00p.out_new());
    /*_p18.AGYL*/ wire AGYL_WAVE_A1 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.EFUZ_WAVE_IDX2.qp_new(), reg_new.cpu_abus.BUS_CPU_A01p.out_new());
    /*_p18.AFUM*/ wire AFUM_WAVE_A2 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.EXEL_WAVE_IDX3.qp_new(), reg_new.cpu_abus.BUS_CPU_A02p.out_new());
    /*_p18.AXOL*/ wire AXOL_WAVE_A3 = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), reg_new.ch3.EFAL_WAVE_IDX4.qp_new(), reg_new.cpu_abus.BUS_CPU_A03p.out_new());
  }

  // wave ram -> sample register
  {
    /*#p17.BUTU*/ wire BUTU_SAMPLE_CLKn = not1(reg_new.ch3.AZUS_WAVE_CLK_D3.qn_new());
    /*#p17.BEKA*/ wire BEKA_SAMPLE_CLKp = not1(BUTU_SAMPLE_CLKn);
    /*#p17.COJU*/ wire COJU_SAMPLE_CLKp = not1(BUTU_SAMPLE_CLKn);
    /*#p17.BAJA*/ wire BAJA_SAMPLE_CLKp = not1(BUTU_SAMPLE_CLKn);
    /*#p17.BUFE*/ wire BUFE_SAMPLE_CLKp = not1(BUTU_SAMPLE_CLKn);

    /*_p17.CYFO*/ reg_new.ch3.CYFO_SAMPLE_0.dff9(BEKA_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D00.qp_old());
    /*_p17.CESY*/ reg_new.ch3.CESY_SAMPLE_1.dff9(BUFE_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D01.qp_old());
    /*_p17.BUDY*/ reg_new.ch3.BUDY_SAMPLE_2.dff9(BAJA_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D02.qp_old());
    /*_p17.BEGU*/ reg_new.ch3.BEGU_SAMPLE_3.dff9(COJU_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D03.qp_old());
    /*_p17.CUVO*/ reg_new.ch3.CUVO_SAMPLE_4.dff9(BEKA_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D04.qp_old());
    /*_p17.CEVO*/ reg_new.ch3.CEVO_SAMPLE_5.dff9(BUFE_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D05.qp_old());
    /*_p17.BORA*/ reg_new.ch3.BORA_SAMPLE_6.dff9(BAJA_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D06.qp_old());
    /*_p17.BEPA*/ reg_new.ch3.BEPA_SAMPLE_7.dff9(COJU_SAMPLE_CLKp, reg_new.ACOR_APU_RSTn(), reg_old.wave_bus.BUS_WAVE_D07.qp_old());

    // wave ram control line 3
    /*#p18.BENO*/ wire BENO_WAVE_RAM_CTRL3n = mux2p(reg_new.ch3.COKA_CH3_ACTIVEp(), BUTU_SAMPLE_CLKn, CAZU_CPU_WAVE_RDn); // to wave ram?
    /*#p18.ATOK*/ wire ATOK_WAVE_RAM_CTRL3p = not1(BENO_WAVE_RAM_CTRL3n); // to wave ram?
  }

  // wave ram -> cpu bus
  {
    /*#p17.CUGO*/ wire CUGO_WAVE_D0n = not1(reg_new.wave_bus.BUS_WAVE_D00.qp_new());
    /*#p17.CEGU*/ wire CEGU_WAVE_D1n = not1(reg_new.wave_bus.BUS_WAVE_D01.qp_new());
    /*#p17.ATEC*/ wire ATEC_WAVE_D2n = not1(reg_new.wave_bus.BUS_WAVE_D02.qp_new());
    /*#p17.ADOK*/ wire ADOK_WAVE_D3n = not1(reg_new.wave_bus.BUS_WAVE_D03.qp_new());
    /*#p17.BACA*/ wire BACA_WAVE_D4n = not1(reg_new.wave_bus.BUS_WAVE_D04.qp_new());
    /*#p17.BERO*/ wire BERO_WAVE_D5n = not1(reg_new.wave_bus.BUS_WAVE_D05.qp_new());
    /*#p17.CUTO*/ wire CUTO_WAVE_D6n = not1(reg_new.wave_bus.BUS_WAVE_D06.qp_new());
    /*#p17.AKAF*/ wire AKAF_WAVE_D7n = not1(reg_new.wave_bus.BUS_WAVE_D07.qp_new());

    /*#p17.DUGU*/ triwire DUGU = tri6_pn(CAZU_CPU_WAVE_RDn, CUGO_WAVE_D0n);
    /*#p17.DESY*/ triwire DESY = tri6_pn(CAZU_CPU_WAVE_RDn, CEGU_WAVE_D1n);
    /*#p17.BATY*/ triwire BATY = tri6_pn(CAZU_CPU_WAVE_RDn, ATEC_WAVE_D2n);
    /*#p17.BADE*/ triwire BADE = tri6_pn(CAZU_CPU_WAVE_RDn, ADOK_WAVE_D3n);
    /*#p17.BUNE*/ triwire BUNE = tri6_pn(CAZU_CPU_WAVE_RDn, BACA_WAVE_D4n);
    /*#p17.BAVA*/ triwire BAVA = tri6_pn(CAZU_CPU_WAVE_RDn, BERO_WAVE_D5n);
    /*#p17.DESA*/ triwire DESA = tri6_pn(CAZU_CPU_WAVE_RDn, CUTO_WAVE_D6n);
    /*#p17.BEZU*/ triwire BEZU = tri6_pn(CAZU_CPU_WAVE_RDn, AKAF_WAVE_D7n);

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(DUGU);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(DESY);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(BATY);
    reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(BADE);
    reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(BUNE);
    reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(BAVA);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(DESA);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(BEZU);
  }
}

//-----------------------------------------------------------------------------

void GateBoySPU::tick_nr30(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*_p16.GEJO*/ wire GEJO_FF1A_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.EMOR_ADDR_FF1Ap());
  /*_p16.GUCY*/ wire GUCY_FF1A_WRn = not1(GEJO_FF1A_WRp);
  /*_p16.GUXE*/ reg_new.ch3.GUXE_CH3_AMP_ENn.dff9(GUCY_FF1A_WRn, reg_new.GOVE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());

  /*_p16.FASY*/ wire FASY_FF1A_RD = nand2(reg_new.cpu_abus.EMOR_ADDR_FF1Ap(), reg_new.cpu_signals.GAXO_CPU_RDp());
  /*_p16.FEVO*/ wire FEVO_CH3_AMP_ENa  = not1(reg_new.ch3.GUXE_CH3_AMP_ENn.qn_new());

#if 0
  if (FASY_FF1A_RD) set_data(
    /*_p16.GEKO*/ FEVO_CH3_AMP_ENa
  );
#endif
}

//-----------------------------------------------------------------------------

void GateBoySPU::tick_nr31(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*_p16.DERY*/ wire DERY_FF1B_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DUSA_ADDR_FF1Bp());

  // fexu/hoto polarity seems wrong
  /*#p18.GEPY*/ wire GEPY_LEN_CLKp = nor3(reg_new.ch3.FEXU_LEN_DONEp.qp_new(), BUFY_CLK_256n(), reg_new.ch3.HOTO_NR34_LENEN.qp_new());
  /*#p18.GENU*/ wire GENU_LEN_CLKn = not1(GEPY_LEN_CLKp);
  /*_p16.GAJY*/ wire GAJY_LOADp = not1(DERY_FF1B_WRn);
  /*_p18.GEVO*/ reg_new.ch3.GEVO_CH3_LEN0p.dff20(GENU_LEN_CLKn,               GAJY_LOADp, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p18.FORY*/ reg_new.ch3.FORY_CH3_LEN1p.dff20(reg_new.ch3.GEVO_CH3_LEN0p.qp_new(), GAJY_LOADp, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p18.GATU*/ reg_new.ch3.GATU_CH3_LEN2p.dff20(reg_new.ch3.FORY_CH3_LEN1p.qp_new(), GAJY_LOADp, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p18.GAPO*/ reg_new.ch3.GAPO_CH3_LEN3p.dff20(reg_new.ch3.GATU_CH3_LEN2p.qp_new(), GAJY_LOADp, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());

  /*#p18.FALU*/ wire FALU_LEN_CLKp = not1(reg_new.ch3.GAPO_CH3_LEN3p.qn_new());
  /*_p16.EMUT*/ wire EMUT_LOADp = not1(DERY_FF1B_WRn);
  /*_p18.GEMO*/ reg_new.ch3.GEMO_CH3_LEN4p.dff20(FALU_LEN_CLKp,               EMUT_LOADp, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p18.FORO*/ reg_new.ch3.FORO_CH3_LEN5p.dff20(reg_new.ch3.GEMO_CH3_LEN4p.qp_new(), EMUT_LOADp, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p18.FAVE*/ reg_new.ch3.FAVE_CH3_LEN6p.dff20(reg_new.ch3.FORO_CH3_LEN5p.qp_new(), EMUT_LOADp, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p18.FYRU*/ reg_new.ch3.FYRU_CH3_LEN7p.dff20(reg_new.ch3.FAVE_CH3_LEN6p.qp_new(), EMUT_LOADp, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*#p16.GETO*/ wire GETO_FF1B_WRp = not1(DERY_FF1B_WRn);
  /*#p18.GUDA*/ wire GUDA_LEN_DONE_RSTn = nor3(GETO_FF1B_WRp, reg_new.KEBA_APU_RSTp(), reg_new.ch3.GARA_TRIG_D1.qp_new());
  /*_p18.FEXU*/ reg_new.ch3.FEXU_LEN_DONEp.dff17(reg_new.ch3.FYRU_CH3_LEN7p.qn_new(), GUDA_LEN_DONE_RSTn, reg_new.ch3.FEXU_LEN_DONEp.qn_old());
}

//-----------------------------------------------------------------------------

void GateBoySPU::tick_nr32(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*_p16.HAGA*/ wire HAGA_FF1C_WR = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.GEFO_ADDR_FF1Cp());

  /*_p16.GUZU*/ wire GUZU_FF1C_WRn = not1(HAGA_FF1C_WR);
  /*_p16.HUKY*/ reg_new.ch3.HUKY_NR32_VOL0.dff9(GUZU_FF1C_WRn, reg_new.GURO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p16.HODY*/ reg_new.ch3.HODY_NR32_VOL1.dff9(GUZU_FF1C_WRn, reg_new.GURO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());

  /*_p16.HENU*/ wire HENU_FF1C_RD = nand2(reg_new.cpu_abus.GEFO_ADDR_FF1Cp(), reg_new.cpu_signals.JOTU_CPU_RDp());

  /*_p18.HONY*/ wire HONY_DBG_FF1C_RD = and3(reg_new.EDEK_NR52_DBG_APUp(), reg_new.cpu_signals.JECO_CPU_RDp(), reg_new.cpu_abus.GEFO_ADDR_FF1Cp());
  /*_p18.GENO*/ wire GENO_DBG_FF1C_RDn = not1(HONY_DBG_FF1C_RD);

#if 0
  if (HENU_FF1C_RD) set_data(
    /*_p18.FAPY*/ 0,
    /*_p18.FARO*/ 0,
    /*_p18.FOTE*/ 0,
    /*_p18.FANA*/ 0,
    /*_p18.FERA*/ 0,
    /*_p16.HAMU*/ HUKY_NR32_VOL0,
    /*_p16.HUCO*/ HODY_NR32_VOL1
  );
#endif
}

//-----------------------------------------------------------------------------

void GateBoySPU::tick_nr33(const GateBoyState& reg_old, GateBoyState& reg_new) {

  /*_p16.KOTA*/ wire KOTA_FF1D_WRn = nand2(reg_new.cpu_abus.FENY_ADDR_FF1Dp(), reg_new.cpu_signals.BOGY_CPU_WRp());
  /*_p16.JAFA*/ wire JAFA_FF1D_WRo = nand2(reg_new.cpu_abus.FENY_ADDR_FF1Dp(), reg_new.cpu_signals.BOGY_CPU_WRp());

  /*_p16.KYHO*/ wire KYHO_FF1D_WRa = not1(KOTA_FF1D_WRn);
  /*_p16.KULY*/ wire KULY_FF1D_WRb = not1(JAFA_FF1D_WRo);

  /*_p16.KOGA*/ reg_new.ch3.KOGA_NR33_FREQ00.dff9(KULY_FF1D_WRb, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p16.JOVY*/ reg_new.ch3.JOVY_NR33_FREQ01.dff9(KULY_FF1D_WRb, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p16.JAXA*/ reg_new.ch3.JAXA_NR33_FREQ02.dff9(KULY_FF1D_WRb, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p16.JEFE*/ reg_new.ch3.JEFE_NR33_FREQ03.dff9(KULY_FF1D_WRb, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p16.JYPO*/ reg_new.ch3.JYPO_NR33_FREQ04.dff9(KULY_FF1D_WRb, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p16.JOVE*/ reg_new.ch3.JOVE_NR33_FREQ05.dff9(KYHO_FF1D_WRa, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p16.KANA*/ reg_new.ch3.KANA_NR33_FREQ06.dff9(KYHO_FF1D_WRa, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p16.KOGU*/ reg_new.ch3.KOGU_NR33_FREQ07.dff9(KYHO_FF1D_WRa, reg_new.KUHA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p16.GUTE*/ wire GUTE_DBG_FF1D_RDn = nor2(reg_new.cpu_abus.HOXA_ADDR_FF1Dn(), reg_new.EGAD_CPU_RDn_DBGn());
  /*_p16.HOVO*/ wire HOVO_DBG_FF1D_RD = not1(GUTE_DBG_FF1D_RDn);

#if 0
  if (HOVO_DBG_FF1D_RD) set_data(
    /*_p16.JOFO*/ KUTU_FREQ_00,
    /*_p16.KAFU*/ KUPE_FREQ_01,
    /*_p16.KESY*/ KUNU_FREQ_02,
    /*_p16.JUDE*/ KEMU_FREQ_03,
    /*_p16.JUKE*/ KYGU_FREQ_04,
    /*_p16.JEZA*/ KEPA_FREQ_05,
    /*_p16.KORA*/ KAFO_FREQ_06,
    /*_p16.KAMY*/ KENO_FREQ_07
  );
#endif
}

//-----------------------------------------------------------------------------

void GateBoySPU::tick_nr34(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*#p16.HUDA*/ wire HUDA_FF1E_WRp = and2(reg_new.cpu_abus.DUGO_ADDR_FF1Ep(), reg_new.cpu_signals.BOGY_CPU_WRp());
  /*#p16.JUZO*/ wire JUZO_FF1E_WRn = not1(HUDA_FF1E_WRp);

  /*_p16.JEMO*/ reg_new.ch3.JEMO_NR34_FREQ08.dff9 (JUZO_FF1E_WRn, reg_new.KOPY_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p16.JETY*/ reg_new.ch3.JETY_NR34_FREQ09.dff9 (JUZO_FF1E_WRn, reg_new.KOPY_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p16.JACY*/ reg_new.ch3.JACY_NR34_FREQ10.dff9 (JUZO_FF1E_WRn, reg_new.KOPY_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());

  /*#p16.FOVO*/ wire FOVO_FF1E_WRn = nand2(reg_new.ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.DUGO_ADDR_FF1Ep());
  /*_p16.HOTO*/ reg_new.ch3.HOTO_NR34_LENEN.dff9  (FOVO_FF1E_WRn, reg_new.HEKY_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());

  /*#p16.FOBA*/ reg_new.ch3.FOBA_CH3_TRIGp.dff17(reg_new.sys_clk.DOVA_ABCDxxxx(), reg_new.GOMA_APU_RSTn(), reg_new.ch3.GAVU_NR34_TRIG.qn_old());

  /*_p16.EPYX*/ wire EPYX_FF1E_WRp = nor2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DUGO_ADDR_FF1Ep()); // polarity?
  /*_p16.FAKO*/ wire FAKO_RESTART_RST = nor2(reg_new.KEBA_APU_RSTp(), reg_new.ch3.FOBA_CH3_TRIGp.qp_new());
  /*_p16.GAVU*/ reg_new.ch3.GAVU_NR34_TRIG.dff9   (EPYX_FF1E_WRp, FAKO_RESTART_RST, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  {
    /*_p16.FUVA*/ wire FUVA_FF1E_RDn = or2(reg_new.cpu_abus.GUNU_ADDR_FF1En(), reg_new.EGAD_CPU_RDn_DBGn());

  #if 0
    if (FUVA_FF1E_RDb) set_data(
      /*_p16.JUVY*/ KEJU_FREQ_08,
      /*_p16.JURA*/ KEZA_FREQ_09,
      /*_p16.HUFO*/ JAPU_FREQ_10,
      /*_p16.HACA*/ HOTO_NR34_STOP
    );
  #endif

    /*_p16.GAWA*/ wire GAWA_FF1E_RDn = nand2(reg_new.cpu_abus.DUGO_ADDR_FF1Ep(), reg_new.cpu_signals.GORY_CPU_RDp()); // polarity?

  #if 0
    if (GAWA_FF1E_RDa) set_data(
      /*_p16.HACA*/ HOTO_NR34_STOP
    );
  #endif
  }
}

//-----------------------------------------------------------------------------

uint32_t tick_out(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*#p17.DATE*/ wire DATE_WAVE_PLAY_D0 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_new(), reg_new.ch3.CYFO_SAMPLE_0.qp_new(), reg_new.ch3.CUVO_SAMPLE_4.qp_new());
  /*#p17.DAZY*/ wire DAZY_WAVE_PLAY_D1 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_new(), reg_new.ch3.CESY_SAMPLE_1.qp_new(), reg_new.ch3.CEVO_SAMPLE_5.qp_new());
  /*#p17.CUZO*/ wire CUZO_WAVE_PLAY_D2 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_new(), reg_new.ch3.BUDY_SAMPLE_2.qp_new(), reg_new.ch3.BORA_SAMPLE_6.qp_new());
  /*#p17.COPO*/ wire COPO_WAVE_PLAY_D3 = mux2n(reg_new.ch3.EFAR_WAVE_IDX0.qp_new(), reg_new.ch3.BEGU_SAMPLE_3.qp_new(), reg_new.ch3.BEPA_SAMPLE_7.qp_new());

  /*#p18.GEMY*/ wire GEMY_WAVE_VOL3 = nor2(reg_new.ch3.HUKY_NR32_VOL0.qp_new(), reg_new.ch3.HODY_NR32_VOL1.qp_new());
  /*#p18.GOKA*/ wire GOKA_WAVE_VOL2 = nor2(reg_new.ch3.HUKY_NR32_VOL0.qp_new(), reg_new.ch3.HODY_NR32_VOL1.qn_new());
  /*#p18.GEGE*/ wire GEGE_WAVE_VOL4 = nor2(reg_new.ch3.HUKY_NR32_VOL0.qn_new(), reg_new.ch3.HODY_NR32_VOL1.qp_new());

  /*#p18.EZAG*/ wire EZAG_WAVE_OUT0 = amux3(DATE_WAVE_PLAY_D0, GEGE_WAVE_VOL4, DAZY_WAVE_PLAY_D1, GOKA_WAVE_VOL2, CUZO_WAVE_PLAY_D2, GEMY_WAVE_VOL3);
  /*#p18.EVUG*/ wire EVUG_WAVE_OUT1 = amux3(DAZY_WAVE_PLAY_D1, GEGE_WAVE_VOL4, CUZO_WAVE_PLAY_D2, GOKA_WAVE_VOL2, COPO_WAVE_PLAY_D3, GEMY_WAVE_VOL3);
  /*#p18.DOKY*/ wire DOKY_WAVE_OUT2 = amux2(CUZO_WAVE_PLAY_D2, GEGE_WAVE_VOL4, COPO_WAVE_PLAY_D3, GOKA_WAVE_VOL2);
  /*#p18.DORE*/ wire DORE_WAVE_OUT3 = and2 (COPO_WAVE_PLAY_D3, GEGE_WAVE_VOL4);

  // these go straight to the dac
  /*#p18.BARY*/ wire BARY_WAVE_DAC0 = and2(reg_new.ch3.COKA_CH3_ACTIVEp(), EZAG_WAVE_OUT0);
  /*#p18.BYKA*/ wire BYKA_WAVE_DAC1 = and2(reg_new.ch3.COKA_CH3_ACTIVEp(), EVUG_WAVE_OUT1);
  /*#p18.BOPA*/ wire BOPA_WAVE_DAC2 = and2(reg_new.ch3.COKA_CH3_ACTIVEp(), DOKY_WAVE_OUT2);
  /*#p18.BELY*/ wire BELY_WAVE_DAC3 = and2(reg_new.ch3.COKA_CH3_ACTIVEp(), DORE_WAVE_OUT3);

  return (BARY_WAVE_DAC0 << 0) | (BYKA_WAVE_DAC1 << 1) | (BOPA_WAVE_DAC2 << 2) | (BELY_WAVE_DAC3 << 3);
}










