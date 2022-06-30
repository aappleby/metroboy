#pragma once
#include "GateBoyLib/Regs.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoyCpuSignals {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  wire AJAS_CPU_RDn_old      () const;
  wire DYKY_CPU_WRn_old      () const;

  wire ASOT_CPU_RDp_old      () const;
  wire CUPA_CPU_WRp_old      () const;
  wire MYNU_CPU_RDn_old      () const;
  wire LEKO_CPU_RDp_old      () const;
  wire REDU_CPU_RDn_old      () const;
  wire MEXO_CPU_WRn_old      () const;

  wire DECY_LATCH_EXTn_old   () const;
  wire CATY_LATCH_EXTp_old   () const;
  wire BOFE_LATCH_EXTn_old   () const;

  wire AJAS_CPU_RDn_new      () const;
  wire DYKY_CPU_WRn_new      () const;

  wire ASOT_CPU_RDp_new      () const;
  wire CUPA_CPU_WRp_new      () const;
  wire MYNU_CPU_RDn_new      () const;
  wire LEKO_CPU_RDp_new      () const;
  wire REDU_CPU_RDn_new      () const;
  wire MEXO_CPU_WRn_new      () const;

  wire DECY_LATCH_EXTn_new   () const;
  wire CATY_LATCH_EXTp_new   () const;
  wire BOFE_LATCH_EXTn_new   () const;

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn() const { return not1(TEDO_CPU_RDp.qp_new()); }
  /*#p19.BYLO*/ wire BYLO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.GAXO*/ wire GAXO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p20.COSA*/ wire COSA_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p14.FOGE*/ wire FOGE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p16.DOVO*/ wire DOVO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p11.CEGE*/ wire CEGE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#pXX.GADO*/ wire GADO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p17.BOKE*/ wire BOKE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p14.BYGO*/ wire BYGO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.CETO*/ wire CETO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.KAZO*/ wire KAZO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.CURU*/ wire CURU_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.KYDU*/ wire KYDU_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p16.JOTU*/ wire JOTU_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p18.JECO*/ wire JECO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p16.GORY*/ wire GORY_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p19.KAGE*/ wire KAGE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p11.BUDA*/ wire BUDA_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p11.BUWA*/ wire BUWA_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }

  /*_p10.BAFU*/ wire BAFU_CPU_WRn() const { return not1(TAPU_CPU_WRp.qp_new()); }
  /*_p10.BOGY*/ wire BOGY_CPU_WRp() const { return not1(BAFU_CPU_WRn()); }

  //----------------------------------------

  Gate ABUZ_EXT_RAM_CS_CLK;

  /*_SIG_IN_CPU_RDp     */  SigIn  SIG_IN_CPU_RDp;           // top right port PORTA_00: -> LAGU, LAVO, TEDO
  /*_SIG_IN_CPU_WRp     */  SigIn  SIG_IN_CPU_WRp;           // top right port PORTA_01: ->
  /*_SIG_IN_CPU_EXT_BUSp*/  SigIn  SIG_IN_CPU_EXT_BUSp;      // top right port PORTA_06: -> TEXO, APAP
  /*_SIG_IN_CPU_LATCH_EXT*/ SigIn  SIG_IN_CPU_DBUS_FREE;     // top left port PORTD_06: -> ANUJ, DECY, LAVO, MUZU

  /*_SIG_CPU_UNOR_DBG*/  SigOut SIG_CPU_UNOR_DBG;      // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  /*_SIG_CPU_ADDR_HIp*/  SigOut SIG_CPU_ADDR_HIp;      // top right port PORTA_03: <- P25.SYRO_FE00_FFFFp
  /*_SIG_CPU_UMUT_DBG*/  SigOut SIG_CPU_UMUT_DBG;      // top right port PORTA_05: <- P07.UMUT_MODE_DBG1
  /*_SIG_CPU_BOOTp*/     SigOut SIG_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  /*_SIG_BOOT_CSp*/      SigOut SIG_BOOT_CSp;          // cs for bootrom

  /*_p07.TEPU*/ DFF17 TEPU_BOOT_BITn;

  Gate TEDO_CPU_RDp;
  Gate APOV_CPU_WRp;
  Gate TAPU_CPU_WRp;

  ///*_SIG_CPU_6*/         SigOut SIG_CPU_6;             // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? Drives an unwired pad.
};

//-----------------------------------------------------------------------------

struct GateBoyCpuABus {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();
  void set_addr(uint16_t bus_addr_new);

  //----------

  wire TOVY_A00n_old         () const;
  wire TOLA_A01n_old         () const;
  wire XOLA_A00n_old         () const;
  wire XENO_A01n_old         () const;
  wire XUSY_A02n_old         () const;
  wire XERA_A03n_old         () const;
  wire TONA_A08n_old         () const;
  wire SORE_A15n_old         () const;
  wire SEFY_A02n_old         () const;
  wire SOGY_A14n_old         () const;

  wire WADO_A00p_old         () const;
  wire WESA_A01p_old         () const;
  wire WALO_A02p_old         () const;
  wire WEPO_A03p_old         () const;

  wire WUTU_ADDR_PPUn_old    () const;
  wire WERO_ADDR_PPUp_old    () const;

  wire TUNA_0000_FDFF_old    () const;
  wire SYKE_ADDR_HIp_old     () const;

  wire XALY_0x00xxxx_old     () const;
  wire SARE_XX00_XX07p_old   () const;
  wire RYFO_FF04_FF07p_old   () const;

  wire WATE_FF46n_old        () const;
  wire XEDA_FF46p_old        () const;

  //----------

  wire TUNA_0000_FDFF_new    () const;
  wire RYCU_FE00_FFFF_new    () const;
  wire SYRO_FE00_FFFF_new    () const;

  wire TOVY_A00n_new         () const;
  wire TOLA_A01n_new         () const;
  wire XOLA_A00n_new         () const;
  wire XENO_A01n_new         () const;
  wire XUSY_A02n_new         () const;
  wire XERA_A03n_new         () const;
  wire TONA_A08n_new         () const;
  wire SORE_A15n_new         () const;
  wire SEFY_A02n_new         () const;
  wire SOGY_A14n_new         () const;

  wire WADO_A00p_new         () const;
  wire WESA_A01p_new         () const;
  wire WALO_A02p_new         () const;
  wire WEPO_A03p_new         () const;

  wire TYRO_XX_0x0x0000p_new () const;
  wire TUFA_XX_x1x1xxxxp_new () const;

  wire SYKE_ADDR_HIp_new     () const;
  wire SOHA_ADDR_HIn_new     () const;

  wire XALY_0x00xxxx_new     () const;
  wire SARE_XX00_XX07p_new   () const;
  wire RYFO_FF04_FF07p_new   () const;

  wire SAPA_XX_xxxx1111p_new () const;
  wire SEMY_XX_0000xxxxp_new () const;
  wire SANO_FF00_FF03p_new   () const;
  wire TUMA_A000_BFFFp_new   () const;
  wire TYNU_A000_FFFFp_new   () const;
  wire ADAH_FE00_FEFFn_new   () const;

  wire TULO_ADDR_BOOTROMp_new() const;

  wire ROPE_ADDR_OAMn_new    () const;
  wire SARO_ADDR_OAMp_new    () const;

  wire WORU_FF40n_new        () const;
  wire WOFA_FF41n_new        () const;
  wire WEBU_FF42n_new        () const;
  wire WAVU_FF43n_new        () const;
  wire WYLE_FF44n_new        () const;
  wire WETY_FF45n_new        () const;
  wire WATE_FF46n_new        () const;
  wire WYBO_FF47n_new        () const;
  wire WETA_FF48n_new        () const;
  wire VAMA_FF49n_new        () const;
  wire WYVO_FF4An_new        () const;
  wire WAGE_FF4Bn_new        () const;

  wire VOCA_FF40p_new        () const;
  wire VARY_FF41p_new        () const;
  wire XARO_FF42p_new        () const;
  wire XAVY_FF43p_new        () const;
  wire XOGY_FF44p_new        () const;
  wire XAYU_FF45p_new        () const;
  wire XEDA_FF46p_new        () const;
  wire WERA_FF47p_new        () const;
  wire XAYO_FF48p_new        () const;
  wire TEGO_FF49p_new        () const;
  wire VYGA_FF4Ap_new        () const;
  wire VUMY_FF4Bp_new        () const;

  wire WUTU_ADDR_PPUn_new    () const;
  wire WERO_ADDR_PPUp_new    () const;

  wire TEVY_ADDR_VRAMn_new   () const;

  //----------

  /*_p10.DYTE*/ wire DYTE_A00n() const { return not1(BUS_CPU_A00p.out_new()); }
  /*_p10.AFOB*/ wire AFOB_A01n() const { return not1(BUS_CPU_A01p.out_new()); }
  /*_p10.ABUB*/ wire ABUB_A02n() const { return not1(BUS_CPU_A02p.out_new()); }
  /*_p10.ACOL*/ wire ACOL_A03n() const { return not1(BUS_CPU_A03p.out_new()); }
  /*#p10.ATUP*/ wire ATUP_A04n() const { return not1(BUS_CPU_A04p.out_new()); }
  /*#p10.BOXY*/ wire BOXY_A05n() const { return not1(BUS_CPU_A05p.out_new()); }
  /*#p10.ASAD*/ wire ASAD_A06n() const { return not1(BUS_CPU_A06p.out_new()); }
  /*#p10.AVUN*/ wire AVUN_A07n() const { return not1(BUS_CPU_A07p.out_new()); }

  /*_p10.DOSO*/ wire DOSO_A00p() const { return not1(DYTE_A00n()); }
  /*_p10.DUPA*/ wire DUPA_A01p() const { return not1(AFOB_A01n()); }
  /*_p10.DENO*/ wire DENO_A02p() const { return not1(ABUB_A02n()); }
  /*_p10.DUCE*/ wire DUCE_A03p() const { return not1(ACOL_A03n()); }

  /*_p10.ATEG*/ wire ATEG_ADDR_XX1Xn() const { return or4(BUS_CPU_A07p.out_new(), BUS_CPU_A06p.out_new(), BUS_CPU_A05p.out_new(), ATUP_A04n()); }
  /*_p10.AWET*/ wire AWET_ADDR_XX2Xn() const { return or4(BUS_CPU_A07p.out_new(), BUS_CPU_A06p.out_new(), BOXY_A05n(), BUS_CPU_A04p.out_new()); }
  /*#p10.ACOM*/ wire ACOM_ADDR_XX3Xn() const { return nand4(AVUN_A07n(), ASAD_A06n(), BUS_CPU_A05p.out_new(), BUS_CPU_A04p.out_new()); }

  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn() const { return not1(SYKE_ADDR_HIp_old()); }

  /*_p10.BUNO*/ wire BUNO_ADDR_FF1Xp() const { return nor2(BAKO_ADDR_FFXXn(), ATEG_ADDR_XX1Xn()); }
  /*_p10.BEZY*/ wire BEZY_ADDR_FF2Xn() const { return  or2(AWET_ADDR_XX2Xn(), BAKO_ADDR_FFXXn()); }
  /*#p10.BARO*/ wire BARO_ADDR_FF3Xp() const { return nor2(ACOM_ADDR_XX3Xn(), BAKO_ADDR_FFXXn()); }

  /*_p10.BANU*/ wire BANU_ADDR_FF1Xn() const { return not1(BUNO_ADDR_FF1Xp()); }
  /*_p10.CONA*/ wire CONA_ADDR_FF2Xp() const { return not1(BEZY_ADDR_FF2Xn()); }

  //----------

  /*#p10.DUPO*/ wire DUPO_ADDR_0000n() const { return nand4(ACOL_A03n(), ABUB_A02n(), AFOB_A01n(), DYTE_A00n()); }
  /*#p10.DUNO*/ wire DUNO_ADDR_0001n() const { return nand4(ACOL_A03n(), ABUB_A02n(), AFOB_A01n(), DOSO_A00p()); }
  /*#p10.DAMY*/ wire DAMY_ADDR_0010n() const { return nand4(ACOL_A03n(), ABUB_A02n(), DUPA_A01p(), DYTE_A00n()); }
  /*#p10.ETUF*/ wire ETUF_ADDR_0011n() const { return nand4(ACOL_A03n(), ABUB_A02n(), DUPA_A01p(), DOSO_A00p()); }
  /*#p10.ESOT*/ wire ESOT_ADDR_0100n() const { return nand4(ACOL_A03n(), DENO_A02p(), AFOB_A01n(), DYTE_A00n()); }

  /*_p10.DAZA*/ wire DAZA_ADDR_0110n() const { return nand4(ACOL_A03n(), DENO_A02p(), DUPA_A01p(), DYTE_A00n()); }
  /*_p10.DUVU*/ wire DUVU_ADDR_0111n() const { return nand4(ACOL_A03n(), DENO_A02p(), DUPA_A01p(), DOSO_A00p()); }
  /*_p10.DAFY*/ wire DAFY_ADDR_1000n() const { return nand4(DUCE_A03p(), ABUB_A02n(), AFOB_A01n(), DYTE_A00n()); }
  /*_p10.DEJY*/ wire DEJY_ADDR_1001n() const { return nand4(DUCE_A03p(), ABUB_A02n(), AFOB_A01n(), DOSO_A00p()); }

  /*_p10.EXAT*/ wire EXAT_ADDR_1010n() const { return nand4(DUCE_A03p(), ABUB_A02n(), DUPA_A01p(), DYTE_A00n()); }
  /*_p10.EMAX*/ wire EMAX_ADDR_1011n() const { return nand4(DOSO_A00p(), DUPA_A01p(), ABUB_A02n(), DUCE_A03p()); }
  /*_p10.GANY*/ wire GANY_ADDR_1100n() const { return nand4(DUCE_A03p(), DENO_A02p(), AFOB_A01n(), DYTE_A00n()); }
  /*_p10.EMOS*/ wire EMOS_ADDR_1101n() const { return nand4(DOSO_A00p(), AFOB_A01n(), DENO_A02p(), DUCE_A03p()); }
  /*_p10.EGEN*/ wire EGEN_ADDR_1110n() const { return nand4(DUCE_A03p(), DENO_A02p(), DUPA_A01p(), DYTE_A00n()); }

  /*#p10.DONA*/ wire DONA_ADDR_0000n() const { return nand4(DYTE_A00n(), AFOB_A01n(), ABUB_A02n(), ACOL_A03n()); }
  /*#p10.DEWA*/ wire DEWA_ADDR_0001n() const { return nand4(DOSO_A00p(), AFOB_A01n(), ABUB_A02n(), ACOL_A03n()); }
  /*#p10.DOFA*/ wire DOFA_ADDR_0010p() const { return  and4(ACOL_A03n(), ABUB_A02n(), DUPA_A01p(), DYTE_A00n()); }
  /*#p10.DUFE*/ wire DUFE_ADDR_0011n() const { return nand4(DOSO_A00p(), DUPA_A01p(), ABUB_A02n(), ACOL_A03n()); }

  /*_p10.DATU*/ wire DATU_ADDR_0100n() const { return nand4(ACOL_A03n(), DENO_A02p(), AFOB_A01n(), DYTE_A00n()); }
  /*_p10.DURA*/ wire DURA_ADDR_0101n() const { return nand4(ACOL_A03n(), DENO_A02p(), AFOB_A01n(), DOSO_A00p()); }
  /*#p10.EKAG*/ wire EKAG_ADDR_0110p() const { return  and4(ACOL_A03n(), DENO_A02p(), DUPA_A01p(), DYTE_A00n()); }

  /*#p10.DYVA*/ wire DYVA_ADDR_FF10p() const { return nor2(BANU_ADDR_FF1Xn(), DUPO_ADDR_0000n()); }
  /*#p10.CAXE*/ wire CAXE_ADDR_FF11p() const { return nor2(BANU_ADDR_FF1Xn(), DUNO_ADDR_0001n()); }
  /*#p10.EDAF*/ wire EDAF_ADDR_FF12p() const { return nor2(BANU_ADDR_FF1Xn(), DAMY_ADDR_0010n()); }
  /*#p10.DECO*/ wire DECO_ADDR_FF13p() const { return nor2(BANU_ADDR_FF1Xn(), ETUF_ADDR_0011n()); }
  /*#p10.DUJA*/ wire DUJA_ADDR_FF14p() const { return nor2(BANU_ADDR_FF1Xn(), ESOT_ADDR_0100n()); }

  /*_p10.COVY*/ wire COVY_ADDR_FF16p() const { return nor2(BANU_ADDR_FF1Xn(), DAZA_ADDR_0110n()); }
  /*_p10.DUTU*/ wire DUTU_ADDR_FF17p() const { return nor2(BANU_ADDR_FF1Xn(), DUVU_ADDR_0111n()); }
  /*_p10.DARA*/ wire DARA_ADDR_FF18p() const { return nor2(BANU_ADDR_FF1Xn(), DAFY_ADDR_1000n()); }
  /*_p10.DOZA*/ wire DOZA_ADDR_FF19p() const { return nor2(BANU_ADDR_FF1Xn(), DEJY_ADDR_1001n()); }

  /*_p10.EMOR*/ wire EMOR_ADDR_FF1Ap() const { return nor2(BANU_ADDR_FF1Xn(), EXAT_ADDR_1010n()); }
  /*_p10.DUSA*/ wire DUSA_ADDR_FF1Bp() const { return nor2(BANU_ADDR_FF1Xn(), EMAX_ADDR_1011n()); }
  /*_p10.GEFO*/ wire GEFO_ADDR_FF1Cp() const { return nor2(BANU_ADDR_FF1Xn(), GANY_ADDR_1100n()); }
  /*_p10.FENY*/ wire FENY_ADDR_FF1Dp() const { return nor2(BANU_ADDR_FF1Xn(), EMOS_ADDR_1101n()); }
  /*_p10.DUGO*/ wire DUGO_ADDR_FF1Ep() const { return nor2(BANU_ADDR_FF1Xn(), EGEN_ADDR_1110n()); }

  /*#p10.DANU*/ wire DANU_ADDR_FF20p() const { return nor2(DONA_ADDR_0000n(), BEZY_ADDR_FF2Xn()); }
  /*#p10.COVO*/ wire COVO_ADDR_FF21p() const { return nor2(DEWA_ADDR_0001n(), BEZY_ADDR_FF2Xn()); }
  /*#p10.EKEZ*/ wire EKEZ_ADDR_FF22p() const { return and2(CONA_ADDR_FF2Xp(), DOFA_ADDR_0010p()); }
  /*#p10.CUGE*/ wire CUGE_ADDR_FF23p() const { return nor2(DUFE_ADDR_0011n(), BEZY_ADDR_FF2Xn()); }

  /*_p10.CAFY*/ wire CAFY_ADDR_FF24p() const { return nor2(BEZY_ADDR_FF2Xn(), DATU_ADDR_0100n()); }
  /*_p10.CORA*/ wire CORA_ADDR_FF25p() const { return nor2(BEZY_ADDR_FF2Xn(), DURA_ADDR_0101n()); }

  /*#p11.BUZE*/ wire BUZE_ADDR_FF10n() const { return not1(DYVA_ADDR_FF10p()); }
  /*#p11.HAXE*/ wire HAXE_ADDR_FF12n() const { return not1(EDAF_ADDR_FF12p()); }
  /*#p11.GAGO*/ wire GAGO_ADDR_FF12n() const { return not1(EDAF_ADDR_FF12p()); }
  /*#p11.CACA*/ wire CACA_ADDR_FF13n() const { return not1(DECO_ADDR_FF13p()); }
  /*#p11.CURE*/ wire CURE_ADDR_FF14n() const { return not1(DUJA_ADDR_FF14p()); }
  /*_p16.HOXA*/ wire HOXA_ADDR_FF1Dn() const { return not1(FENY_ADDR_FF1Dp()); }
  /*_p16.GUNU*/ wire GUNU_ADDR_FF1En() const { return not1(DUGO_ADDR_FF1Ep()); }
  /*_p09.BYMA*/ wire BYMA_ADDR_FF24n() const { return not1(CAFY_ADDR_FF24p()); }
  /*_p09.GEPA*/ wire GEPA_ADDR_FF25n() const { return not1(CORA_ADDR_FF25p()); }


  /*_BUS_CPU_A00p*/ Bus BUS_CPU_A00p;
  /*_BUS_CPU_A01p*/ Bus BUS_CPU_A01p;
  /*_BUS_CPU_A02p*/ Bus BUS_CPU_A02p;
  /*_BUS_CPU_A03p*/ Bus BUS_CPU_A03p;
  /*_BUS_CPU_A04p*/ Bus BUS_CPU_A04p;
  /*_BUS_CPU_A05p*/ Bus BUS_CPU_A05p;
  /*_BUS_CPU_A06p*/ Bus BUS_CPU_A06p;
  /*_BUS_CPU_A07p*/ Bus BUS_CPU_A07p;
  /*_BUS_CPU_A08p*/ Bus BUS_CPU_A08p;
  /*_BUS_CPU_A09p*/ Bus BUS_CPU_A09p;
  /*_BUS_CPU_A10p*/ Bus BUS_CPU_A10p;
  /*_BUS_CPU_A11p*/ Bus BUS_CPU_A11p;
  /*_BUS_CPU_A12p*/ Bus BUS_CPU_A12p;
  /*_BUS_CPU_A13p*/ Bus BUS_CPU_A13p;
  /*_BUS_CPU_A14p*/ Bus BUS_CPU_A14p;
  /*_BUS_CPU_A15p*/ Bus BUS_CPU_A15p;
};

//---------------------------------------------------------------------------------------------------------------------

struct GateBoyCpuDBus {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();
  void set_data(bool OEp, uint8_t data);

  /*_BUS_CPU_D00p*/ Bus BUS_CPU_D00p;
  /*_BUS_CPU_D01p*/ Bus BUS_CPU_D01p;
  /*_BUS_CPU_D02p*/ Bus BUS_CPU_D02p;
  /*_BUS_CPU_D03p*/ Bus BUS_CPU_D03p;
  /*_BUS_CPU_D04p*/ Bus BUS_CPU_D04p;
  /*_BUS_CPU_D05p*/ Bus BUS_CPU_D05p;
  /*_BUS_CPU_D06p*/ Bus BUS_CPU_D06p;
  /*_BUS_CPU_D07p*/ Bus BUS_CPU_D07p;
};

//---------------------------------------------------------------------------------------------------------------------
