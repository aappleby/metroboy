    // TERO_01 sc
    // TERO_02 << UNER_16
    // TERO_03 sc
    // TERO_04 nc
    // TERO_05 nc
    // TERO_06 << UFOL_04
    // TERO_07 << TERO_16
    // TERO_08 nc
    // TERO_09 sc
    // TERO_10 nc
    // TERO_11 nc
    // TERO_12 sc
    // TERO_13 << UFOL_04
    // TERO_14 nc
    // TERO_15 nc
    // TERO_16 >> TERO_07, UNYK_02
    // TERO_17 >> UVYR_01

    // XYKY_01 << BODE
    // XYKY_02 nc
    // XYKY_03 << OAM_D0
    // XYKY_04 nc
    // XYKY_05 nc
    // XYKY_06 nc
    // XYKY_07 nc
    // XYKY_08 >> YLOR
    // XYKY_09 nc
    // XYKY_10 >> XACA


  // CORE := tri(UCOM, CULY_QN)
  // ELUV := tri(UCOM, ETAF_QN)
  // CAVE01 << CULY17 (xfer dir)
  // CAVE02 << COTY17 (mux 1 = int clk)
  // CAVE03 << SCK_C  (mux 0 = ext clk)
  // CAVE04 nc
  // CAVE05 >> DAWA01

  // LOPE_01 SC
    // LOPE_02 NC
    // LOPE_03 NC
    // LOPE_04 NC
    // LOPE_05 SC
    // LOPE_06 SC
    // LOPE_07 << D    PESU_GND 
    // LOPE_08 NC
    // LOPE_09 SC
    // LOPE_10 NC
    // LOPE_11 SC
    // LOPE_12 NC
    // LOPE_13 NC
    // LOPE_14 << SETn MYZU_04
    // LOPE_15 >> Qn
    // LOPE_16 >> Q    PIN_CPU_INT_VBLANK, MATY_03
    // LOPE_17 << RSTn LYTA_05
    // LOPE_18 NC
    // LOPE_19 SC
    // LOPE_20 SC
    // LOPE_21 SC
    // LOPE_22 << CLKp VYPU_03

    // TEME_01 << BUS_CPU_D0p
    // TEME_02 nc
    // TEME_03 nc
    // TEME_04 nc
    // TEME_05 << RAHU
    // TEME_06 << BUS_CPU_D0p
    // TEME_07 nc
    // TEME_08 nc 
    // TEME_09 >> BUS_VRAM_D0p
    // TEME_10 nc

    // YFAP_01 << YDYV_10
    // YFAP_02
    // YFAP_03
    // YFAP_04
    // YFAP_05 << WUME
    // YFAP_06 << YDYV_10
    // YFAP_07
    // YFAP_08
    // YFAP_09 >> BUS_CPU_D0p
    // YFAP_10


    // REGA_01 >> POVY_20 Qn
    // REGA_02 nc
    // REGA_03 << PUXY_03 D
    // REGA_04 << MEXU_04 LOADp
    // REGA_05 nc
    // REGA_06 sc
    // REGA_07 nc
    // REGA_08 nc
    // REGA_09 nc
    // REGA_10 nc
    // REGA_11 sc
    // REGA_12 nc
    // REGA_13 nc
    // REGA_14 << MEXU_04 LOADp
    // REGA_15 nc
    // REGA_16 << PUXY_03 D
    // REGA_17 >> SOKU_04 Q
    // REGA_18 sc
    // REGA_19 sc
    // REGA_20 << SOGU_03 CLKp

    // POVY_01 >> PERU_20 Qn
    // POVY_02 nc
    // POVY_03 << NERO_03 D
    // POVY_04 << MEXU_04 LOADp
    // POVY_05 nc
    // POVY_06 sc
    // POVY_07 nc
    // POVY_08 nc
    // POVY_09 nc
    // POVY_10 nc
    // POVY_11 sc
    // POVY_12 nc
    // POVY_13 nc
    // POVY_14 << MEXU_04 LOADp
    // POVY_15 nc
    // POVY_16 << NERO_03 D
    // POVY_17 >> RACY_04 Q
    // POVY_18 sc
    // POVY_19 sc
    // POVY_20 << REGA_01 CLKp

    // NUGA_01 >> NYDU_07 Qn
    // NUGA_02 nc
    // NUGA_03 << PAGU_03 D
    // NUGA_04 << MEXU_04 LOADp
    // NUGA_05 nc
    // NUGA_06 sc
    // NUGA_07 nc
    // NUGA_08 nc
    // NUGA_09 nc
    // NUGA_10 nc
    // NUGA_11 sc
    // NUGA_12 nc
    // NUGA_13 nc
    // NUGA_14 << MEXU_04 LOADp
    // NUGA_15 nc
    // NUGA_16 << PAGU_03 D
    // NUGA_17 >> PUSO_04 Q
    // NUGA_18 sc
    // NUGA_19 sc
    // NUGA_20 << PEDA_01 CLKp

    // NYDU_01 SC
    // NYDU_02 << BOGA_05 CLKp
    // NYDU_03 SC
    // NYDU_04 NC
    // NYDU_05 NC
    // NYDU_06 << MUGY_02 RSTn
    // NYDU_07 << NUGA_01 D
    // NYDU_08 NC
    // NYDU_09 SC
    // NYDU_10 NC
    // NYDU_11 NC
    // NYDU_12 SC
    // NYDU_13 << MUGY_02 RSTn
    // NYDU_14 NC
    // NYDU_15 NC
    // NYDU_16 >> MERY_02 QN
    // NYDU_17 >> NC      Q


// AFUR_01 NC
    // AFUR_02 << !ATAL_03: CLKp
    // AFUR_03 << ADYK_09 : D
    // AFUR_04 << ATAL_03 : CLKn
    // AFUR_05 NC
    // AFUR_06 << UPOJ_04 : RSTp
    // AFUR_07 NC
    // AFUR_08 >> ALEF_03 : Qn?
    // AFUR_09 >> ATYP_01 : Q?

    // ALEF_01 NC
    // ALEF_02 << ATAL_03 : CLKp
    // ALEF_03 << AFUR_08 : D
    // ALEF_04 << !ATAL_03: CLKn
    // ALEF_05 NC
    // ALEF_06 << UPOJ_04 : RSTp
    // ALEF_07 NC
    // ALEF_08 >> APUK_03, AFEP_01 : Qn?
    // ALEF_09 >> NC      : Q?

    // APUK_01 NC
    // APUK_02 << !ATAL_03: CLKp
    // APUK_03 << ALEF_08 : D
    // APUK_04 << ATAL_03 : CLKn
    // APUK_05 NC
    // APUK_06 << UPOJ_04 : RSTp
    // APUK_07 NC
    // APUK_08 >> ADYK_03 : Qn?
    // APUK_09 >> AROV_01 : Q?

    // ADYK_01 NC
    // ADYK_02 << ATAL_03 : CLKp
    // ADYK_03 << APUK_08 : D
    // ADYK_04 << !ATAL_03: CLKn
    // ADYK_05 NC
    // ADYK_06 << UPOJ_04 : RSTp
    // ADYK_07 NC
    // ADYK_08 >> ADAR_01 : Qn?
    // ADYK_09 >> AFUR_03 : Q?

    // adyk = dff(atal, upoj, apuk.qn())



    // NAFA_01 << LORU_02 : CLKp
// NAFA_02 << CPU_D0  : D
// NAFA_03 nc
// NAFA_04 << PYSU_02 : CLKn
// NAFA_05 nc
// NAFA_06 nc
// NAFA_07 >> EVAX_04 : Qn? (tribuf to BUS_VRAM_A08p)
// NAFA_08 >> POLY_04 : Q?  (tribuf to BUS_CPU_D0p)


// LOKY := nand(LARA, LENE_QN);
// LARA := nand(LOKY, MYTE_QN, CUNU);

#if 0

// LAVY00 << CUPA04
// LAVY01 << XEDA02
// LAVY02 nc
// LAVY03 >> LORU00

// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// LAVY = and2(FF46, CUPA)
// NAVO = nand6(dma addr)
// NOLO = not1(NAVO)
// LOKO = nand2(CUNU, !LENE) // check neg output?
// LUPA = nor2(LAVY, LYXE)
// LUVY = state
// MATU = state
// LENE = state
// MYTE_DMA_DONE = state
// LYXE = latch(LOKO, LAVY)
// LARA = nand3(LOKY, CUNU, !MYTE_DMA_DONE)
// LOKY = nand2(LARA, !LENE)
// META = and2(UVYT, LOKY)
// LORU = not1(LAVY)

// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// LUPA00 << LAVY
// LUPA01 << LYXE
// LUPA02 >> LUVY

// LOGO = not1(MUDA);
// MORY = nand2(MATU17, LOGO) MATU17 must be Q
// LUMA = not1(MORY);

// Die trace:
// LEBU = not1(MARU06)
// MUDA = nor4(PULA06, POKU06, LEBU);

// if rung 6 of MARU/PULA/POKU was QN:
// MUDA = and2(A13, A14, !A15);
// would select last quarter of ROM, which doesn't make sense
// so rung 6 of MARU must be Q.

// schematic incorrect.
// lyxe - weird gate - lavy, loko
// lupa - nor4 - lavy, lyxe

//----------------------------------------

// Arms on ground, NOR latch
// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// if LOKO02 goes high, LYXE02 goes high.
// if LAVY03 goes high, LYXE02 goes low.

#endif

  // If AVOR_RSTp was 1 in run mode
  // then ALUR_RSTn = 0
  // then MULO_TIMA_RST = 1
  // then PUXY/NERO = 0 -> stuck
  // Therefore AVOR_RSTp = 0 in run mode
  // Therefore ASOL|AFER = 0 in run mode

    // FUFO := not(XYMO_QN)
    // WUKY := not(YZOS_QP)


    // XUQU := not(VONU_QN);
    // GEJY := amux2(XUSO_Q, FUFO, XYMO_QN, WAGO)

    // ABEM := tri(ABON, XUQU) >> VRAM_A00
    // BAXE := tri(ABON, CYVU) >> VRAM_A01
    // ARAS := tri(ABON, BORE) >> VRAM_A02
    // AGAG := tri(ABON, BUVY) >> VRAM_A03
    // FAMU := tri(ABON, GEJY) >> VRAM_A04
    // FUGY := tri(ABON, XEGU_Q) >> VRAM_A05

// TEGU_01 << SOSE
// TEGU_02 << PIN_CPU_WR
// TEGU_03 >> SALE_03

// APAR := not1(ACYL);
// DUGA := not1(MATU_17);
// ASAM := or2(ACYL, XYMU, MATU_17);
// BETE := not1(AJON);

// BOGE := not1(MATU);
// ACYL := and2(BOGE, BESU);
// AJON := and2(XYMU, BOGE)

// DEGE := not(ERUC)
// DABY := not(ENEF)
// DABU := not(FECO)
// GYSA := not(GYKY)

// Schematic has the line tribufs out of order

// CUCU := tri(FEPO, DEGE) >> SPR_TRI_L0
// CUCA := tri(FEPO, DABY) >> SPR_TRI_L1
// CEGA := tri(FEPO, DABU) >> SPR_TRI_L2
// WENU := tri(FEPO, GYSA) >> SPR_TRI_L3
// XADU := dff(WUDA, CYKE, WEFE, YFOT);
// WUZY := tri(BUZA, XADU)

// WUKU := and2(LEKO, AMAB, GEKA);
// WEWU := not1(WUKU);
// XACA := tri(XYKY, WEWU);

// GEKA := not1(OAM_TRI_A0n);
// WAFO := not1(GEKA);

// GUKO := and2(WAFO, AMAB, LEKO);
// WUME := not1(GUKO);

// YLOR_01 << XEGA_01 : CLKp
// YLOR_02 << XYKY_08 : D
// YLOR_03 nc
// YLOR_04 << COMP    : CLKn
// YLOR_05 nc
// YLOR_06 nc
// YLOR_07 >> nc      : Qn
// YLOR_08 >> ZAGO_01 : Q


#if   0
    POWU_DMA_OAM_WRp     = and2(MATU_DMA_RUNNINGp, xxxxEFGH, !PIN_CPU_HOLD_MEM);
    WYJA_OAM_WRp         = or (and(FE00_FEFFp, AJUJ_OAM_BUSYn, PIN_CPU_WRp, AFAS_xxxxEFGx), POWU_DMA_OAM_WRp);
    PIN_OAM_WRn_A = nand2(WYJA_OAM_WRp, !OAM_TRI_A0n);
    PIN_OAM_WRn_B = nand2(WYJA_OAM_WRp,  OAM_TRI_A0n);

#endif

#if 0

    SCAN_OAM_LATCH = and2(ACYL_SCANNINGp, XOCE_AxxDExxH());
    CPU_OAM_LATCH  = and2(PIN_CPU_RDp, !PIN_CPU_HOLD_MEM, FE00_FEFFp)
    PIN_OAM_OE = !or(SCAN_OAM_LATCH, SPR_READp, BOTA_CPU_OAM_LATCH);

#endif

#if 0
    // cpu data driven to bus on EFGH during write

PIN_EXT_RD           = and2(              PIN_CPU_WRp,  PIN_CPU_ADDR_EXT, !ADDR_VRAM);
PIN_EXT_WR           = and2(              PIN_CPU_WRp,  PIN_CPU_ADDR_EXT, !ADDR_VRAM, AFAS_xxxxEFGx);
PIN_EXT_CS           = and2(                           !PIN_CPU_ADDR_EXT, A000_FFFF,  TUNA_0000_FDFFp);
MATE_LATCH_CPU_ADDRp = and2(                            PIN_CPU_ADDR_EXT, !ADDR_VRAM);
LATCH_CPU_DATA       = and2(PIN_CPU_RDp,                PIN_CPU_ADDR_EXT, !ADDR_VRAM, PIN_CPU_DV);
IBUS_TO_EBUSp        = and2(              PIN_CPU_WRp,  PIN_CPU_ADDR_EXT, !ADDR_VRAM);
PIN_EXT_A15_A        = and2(                           !PIN_CPU_ADDR_EXT, !A15);
PIN_EXT_A15_D        = and2(                           !PIN_CPU_ADDR_EXT, !A15);

#endif

#if 0

    LAVO is NAND3

    LAVO_01 << PIN_CPU_RD
    LAVO_02 << TEXO_04
    LAVO_03 << PIN_CPU_DV
    LAVO_04 >>

    // SOMA = latch(LAVO, PIN_EXT_D0n_C)
    // RYMA = tri(LAVO, SOMA_08)
#endif

#if 0

    SORE = not1(A15);
    TEVY = or2(A13, A14, SORE);
    TEXO = and2(PIN_CPU_ADDR_EXT, TEVY);
    MOCA = nor4(TEXO, UMUT)
    NEVY = or2(MEXO, MOCA);
    LAGU = or2(and(PIN_CPU_RD, LEVO), PIN_CPU_WR);
    LYWE = not1(LAGU)
    MOTY = or2(MOCA, LYWE)

    UJYV is MUX5
    UJYV_01 << UNOR_04
    UJYV_02 << RD_C
    UJYV_03 << PIN_CPU_RD
    UJYV_04 nc
    UJYV_05 >> TEDO_01

    TEDO is NOT4
    TEDO_01 << UJYV_05
    TEDO_02 nc
    TEDO_03 nc
    TEDO_04 >> TEDA_02, TUBY_02, UFEG_02, REDU_01, SORA_02, *

    REDU is NOT
    REDU_01 << TEDO_04
    REDU_02 >> RORU_02

    RORU is MUX6
    RORU_01 << UNOR_04
    RORU_02 << REDU_02
    RORU_03 << MOTY_04
    RORU_04 nc
    RORU_05 nc 
    RORU_06 >> LULA_01, *

    LULA is NOT
    LULA_01 << RORU_06
    LULA_02 >> D_B

    RUXA is NAND
    RUXA_01 << CPU_D0
    RUXA_02 << LULA_02
    RUXA_03 >> D0_A


    UJYV = mux5n(UNOR, RD_C, PIN_CPU_RD);
    TEDO = not1(UJYV)
    REDU = not1(TEDO)
    RORU = mux6p(UNOR, REDU, MOTY);
    LULA = not1(RORU)
    RUXA = nand2(CPU_D0, LULA);
    D0_A = RUXA;
    

    // if RORU selects MOTY:

    D0_A = or2(!CPU_D0, !PIN_CPU_WR, ADDR_VRAM, !PIN_CPU_ADDR_EXT);


    LULA_CBD_TO_EPDp = PIN_CPU_WRp;
    if (ADDR_VRAM) LULA_CBD_TO_EPDp = 0;
    if (!PIN_CPU_ADDR_EXT) LULA_CBD_TO_EPDp = 0;

    D*_B = and2(PIN_CPU_WRp, PIN_CPU_ADDR_EXT, !ADDR_VRAM);

#endif


#if 0

    PIN_EXT_A15_A = and2(xxCDEFGH, PIN_CPU_ADDR_EXT, !A15);
    PIN_EXT_A15_D = and2(xxCDEFGH, PIN_CPU_ADDR_EXT, !A15);

#endif

#if 0
    PIN_EXT_A00n_A = not1(tp_latch(and(PIN_CPU_ADDR_EXT, !ADDR_VRAM), BUS_CPU_A00));

#endif


#if 0
    AGUT is (probably) and2(OR())
    AGUT_01 << AROV
    AGUT_02 << AJAX
    AGUT_03 nc
    AGUT_04 << PIN_CPU_ADDR_EXT
    AGUT_05 >> AWOD_02

    AWOD is NOR
    AWOD_01 << UNOR
    AWOD_02 << AGUT_05
    AWOD_03 >> ABUZ_01

    ABUZ is NOT4
    ABUZ_01 << AWOD_03
    ABUZ_02 nc
    ABUZ_03 nc
    ABUZ_04 >> *

    CS_A = and2(xxCDEFGH, PIN_CPU_ADDR_EXT, A000_FDFF);

#endif

#if 0

    AREV is NAND2
    AREV_01 << AFAS_03
    AREV_02 << PIN_CPU_WR
    AREV_03 >> APOV_01

    APOV is NOT4
    APOV_01 << AREV_03
    APOV_02 nc
    APOV_03 nc
    APOV_04 >> MEXO_01

    MEXO is NOT2
    MEXO_01 << APOV_04
    MEXO_02 >> NEVY_01

    SORE is NOT
    SORE_01 << A15
    SORE_02 >> TEVY_03

    TEVY is OR3
    TEVY_01 << A13
    TEVY_02 << A14
    TEVY_03 << SORE_02
    TEVY_04 nc
    TEVY_05 >> TEXO_02

    TEXO is AND2
    TEXO_01 << top right cpu port pin 7 
    TEXO_02 << TEVY_05
    TEXO_03 nc
    TEXO_04 >> LAVO_02, MOCA_01, LEVO_01, LOXO_02

    MOCA is NOR2
    MOCA_01 << TEXO_04
    MOCA_02 << UMUT_04
    MOCA_03 >> NEVY_02, MOTY_01

    NEVY is OR2
    NEVY_01 << MEXO_02
    NEVY_02 << MOCA_03
    NEVY_03 nc
    NEVY_04 >> PUVA_01

    PUVA is OR2
    PUVA_01 << NEVY_04
    PUVA_02 << LUMA_02
    PUVA_03 nc
    PUVA_04 >> USUF_02, UVER_01

    UVER is NAND2
    UVER_01 << PUVA_04
    UVER_02 << TOVA_02
    UVER_03 >> WR_A

    USUF is NOR2
    USUF_01 << UNOR_04
    USUF_02 << PUVA_04
    USUF_03 >> WR_D

    AREV = nand2(AFAS, PIN_CPU_WR);
    APOV = not1(AREV);
    MEXO = not1(APOV);
    SORE = not1(A15);
    TEVY = or2(A13, A14, SORE);
    TEXO = and2(PIN_CPU_ADDR_EXT, TEVY);
    MOCA = nor4(TEXO, UMUT)
    NEVY = or2(MEXO, MOCA);
    PUVA = or2(NEVY, LUMA)
    UVER = nand2(PUVA, TOVA);
    USUF = nor4(UNOR, PUVA);
    WR_A = UVER;
    WR_D = USUF;


    PIN_EXT_WR_A = and2(AFAS_xxxxEFGx, PIN_CPU_WR, PIN_CPU_ADDR_EXT, !ADDR_VRAM);

#endif


#if 0

    SORE is NOT
    SORE_01 << A15
    SORE_02 >> TEVY_03

    TEVY is OR3
    TEVY_01 << A13
    TEVY_02 << A14
    TEVY_03 << SORE_02
    TEVY_04 nc
    TEVY_05 >> TEXO_02

    TEXO is AND2
    TEXO_01 << top right cpu port pin 7 
    TEXO_02 << TEVY_05
    TEXO_03 nc
    TEXO_04 >> LAVO_02, MOCA_01, LEVO_01, LOXO_02

    MOCA is NOR2
    MOCA_01 << TEXO_04
    MOCA_02 << UMUT_04
    MOCA_03 >> NEVY_02, MOTY_01

    LEVO is NOT
    LEVO_01 << TEXO_04
    LEVO_02 >> LAGU_02

    LAGU is (probably) or2(AND())
    LAGU_01 << PIN_CPU_RD
    LAGU_02 << LEVO_02
    LAGU_03 nc
    LAGU_04 << PIN_CPU_WR
    LAGU_05 >> LYWE_01

    LYWE is NOT
    LYWE_01 << LAGU_05
    LYWE_02 >> MOTY_02

    MOTY is OR
    MOTY_01 << MOCA_03
    MOTY_02 << LYWE_02
    MOTY_03 nc
    MOTY_04 >> TYMU_02, RORU_03

    TYMU is NOR2
    TYMU_01 << LUMA_02
    TYMU_02 << MOTY_03
    TYMU_03 >> UGAC_01, URUN_02

    UGAC is NAND2
    UGAC_01 << TYMU_03
    UGAC_02 << TOVA_02
    UGAC_03 >> RD_A

    URUN is NOR2
    URUN_01 << UNOR_04
    URUN_02 << TYMU_03
    URUN_03 >> RD_D

    SORE = not1(A15);
    TEVY = or2(A13, A14, SORE);
    TEXO = and2(PIN_CPU_ADDR_EXT, TEVY);
    MOCA = nor4(TEXO, UMUT)
    LEVO = not1(TEXO)
    LAGU = or2(and(PIN_CPU_RD, LEVO), PIN_CPU_WR);
    LYWE = not1(LAGU)
    MOTY = or2(MOCA, LYWE)
    TYMU = nor4(LUMA, MOTY)
    UGAC = nand2(TYMU, TOVA)
    URUN = nor4(UNOR, TYMU)
    RD_A = UGAC
    RD_D = URUN

    // so we're always reading _unless_ we have a valid write to not-vram
    PIN_EXT_RD_A = or2(!PIN_CPU_WR, !PIN_CPU_ADDR_EXT, ADDR_VRAM);

#endif



#if 0
  // these are all "normal operating mode" w/o rendering or dma 

  PIN_OAM_OE     = nand2(PIN_CPU_RDp, !PIN_CPU_HOLD_MEM,                     ADDR_OAM);
  OPD_TO_OBD     = nand2(PIN_CPU_RDp, !PIN_CPU_HOLD_MEM,                     ADDR_OAM);
  OBD_TO_OBL     =  and2(PIN_CPU_RDp, !PIN_CPU_HOLD_MEM,                     ADDR_OAM);
  OBL_TO_CBD     =  and2(PIN_CPU_RDp,  PIN_CPU_HOLD_MEM,                     ADDR_OAM);

  EPD_TO_CBD     =  and2(PIN_CPU_RDp,  PIN_CPU_HOLD_MEM,  PIN_CPU_ADDR_EXT, !ADDR_VRAM);
  PIN_EXT_RD     = nand2(PIN_CPU_WRp,                     PIN_CPU_ADDR_EXT, !ADDR_VRAM);
  PIN_EXT_WR     =  and2(PIN_CPU_WRp,                     PIN_CPU_ADDR_EXT, !ADDR_VRAM,   xxxxEFGx);
  CBD_TO_EPD     =  and2(PIN_CPU_WRp,                     PIN_CPU_ADDR_EXT, !ADDR_VRAM);
  CBA_TO_EPA     =  and2(                                 PIN_CPU_ADDR_EXT, !ADDR_VRAM);

  PIN_EXT_CS     =  and2(                                !PIN_CPU_ADDR_EXT,  ADDR_ERAM,   xxCDEFGH);

  PIN_EXT_A15_A  =  !or(                                 PIN_CPU_ADDR_EXT, A15);
  PIN_EXT_A15_D  =  !or(                                 PIN_CPU_ADDR_EXT, A15);

  PIN_OAM_WRn    = nand2(PIN_CPU_WRp,                                        ADDR_OAM,    xxxxEFGx);
  TIMA_LOADp     =  and2(PIN_CPU_WRp, !PIN_CPU_HOLD_MEM,                     FF05,        xxxxEFGx);


  VBD_TO_CBD     =  and2(PIN_CPU_RDp,  PIN_CPU_HOLD_MEM, !PIN_CPU_ADDR_EXT,  ADDR_VRAM);
  PIN_VRAM_WR    = nand2(PIN_CPU_WRp,                    !PIN_CPU_ADDR_EXT,  ADDR_VRAM,   xxxxEFGx);
  PIN_VRAM_CS    = nand2(                                !PIN_CPU_ADDR_EXT,  ADDR_VRAM);
  PIN_VRAM_OE    = nand2(PIN_CPU_WRp,                                        ADDR_VRAM);


  CBD_TO_VPD     =  and2(PIN_CPU_WRp,                    !PIN_CPU_ADDR_EXT,  ADDR_VRAM);


  PIN_EXT_A00n_A = not1(tp_latch(and(PIN_CPU_ADDR_EXT, !ADDR_VRAM), BUS_CPU_A00));


  /*read  rom */ PIN_CPU_HOLD_MEM = true;  PIN_CPU_ADDR_EXT = true;
  /*write rom */ PIN_CPU_HOLD_MEM = dc;    PIN_CPU_ADDR_EXT = true;

  /*read  vram*/ PIN_CPU_HOLD_MEM = true;  PIN_CPU_ADDR_EXT = false;
  /*write vram*/ PIN_CPU_HOLD_MEM = dc;    PIN_CPU_ADDR_EXT = false;

  /*read  cram*/ PIN_CPU_HOLD_MEM = true;  PIN_CPU_ADDR_EXT = true;
  /*write cram*/ PIN_CPU_HOLD_MEM = dc;    PIN_CPU_ADDR_EXT = true;

  /*read  eram*/ PIN_CPU_HOLD_MEM = true;  PIN_CPU_ADDR_EXT = true;
  /*write eram*/ PIN_CPU_HOLD_MEM = dc;    PIN_CPU_ADDR_EXT = true;

  /*read  oam */ PIN_CPU_HOLD_MEM = true;  PIN_CPU_ADDR_EXT = false;
  /*write oam */ PIN_CPU_HOLD_MEM = dc;    PIN_CPU_ADDR_EXT = false;

  /*read  hram*/ PIN_CPU_HOLD_MEM = dc;    PIN_CPU_ADDR_EXT = dc;
  /*write hram*/ PIN_CPU_HOLD_MEM = dc;    PIN_CPU_ADDR_EXT = false;

#endif



// VYPO -> VEZA VOGU VEZO TYFO SUDA_06 SOBU (RAWU POZO ...)

// XEPE_01 nc
    // XEPE_02 << FUXU_02  (clk)
    // XEPE_03 << ZAGO_02  (d)
    // XEPE_04 << COMP_CLK (clkn)
    // XEPE_05 nc
    // XEPE_06 << DYNA02   (rst)
    // XEPE_07 nc
    // XEPE_08 >> ZOGY_02  (qn)
    // XEPE_09 >> nc


    
    // ATYP := not(AFUR_09);
  // AJAX := not(ATYP_02);
  // AROV := not(APUK_09);



// MAKA_01 sc
    // MAKA_02 << ZEME
    // MAKA_03 sc
    // MAKA_04 NC
    // MAKA_05 NC
    // MAKA_06 << CUNU
    // MAKA_07 << CATY
    // MAKA_08 NC
    // MAKA_09 sc
    // MAKA_10 NC
    // MAKA_11 NC
    // MAKA_12 sc
    // MAKA_13 << CUNU
    // MAKA_14 NC
    // MAKA_15 NC
    // MAKA_16 >> QN NC
    // MAKA_17 >> NAXY_02



// XECU01 nc
// XECU02 << WEFE02
// XECU03 << YZET02 (oam a7)
// XECU04 nc
// XECU05 << WUDA03
// XECU06 ==
// XECU07 ==
// XECU08 << CYKE01???? - is this a dual-edge ff? I guess that would make it latch the address every tcycle?
// XECU09 << WEFE02
// XECU10 ==
// XECU11 nc
// XECU12 >> WEZA04
// XECU13 >> nc

// BUZA04 = and2(CENO16, XYMU03)

// WEZA01 << BUZA04
// WEZA02 ==
// WEZA03 ==
// WEZA04 << XECU12
// WEZA05 nc
// WEZA06 


// Die trace:
// VETU = and2(TEVO, PORE);
// XACO = not1(XOFO)
// WYKA =

// WYKA01 <> WYKA12
// WYKA02 << VETU
// WYKA03 <> WYKA09
// WYKA04 nc
// WYKA05 nc
// WYKA06 << XACO
// WYKA07 << WYKA16
// WYKA08 nc
// WYKA09 <> WYKA03
// WYKA10 nc
// WYKA11 nc
// WYKA12 <> WYKA01
// WYKA13 << XACO
// WYKA14 nc
// WYKA15 nc
// WYKA16 >> WYKA07, WODY02
// WYKA17 >> XEJA

// TYNO_01 << TOXE_17
// TYNO_02 << SEBA_17
// TYNO_03 << VONU_17
// TYNO_04 >> VUSA_02

// VUSA_01 << TYFO_16
// VUSA_02 << TYNO_04
// VUSA_03 nc
// VUSA_04 >>

// stat read
// hblank  = stat 0 = 00
// vblank  = stat 1 = 01
// oamscan = stat 2 = 10
// render  = stat 3 = 11

// RUPO arms on ground side, nor4 latch
// RUPO00 << ROPO16
// RUPO01 nc
// RUPO02 >> SEGO03
// RUPO03 >> nc
// RUPO04 nc
// RUPO05 << PAGO03

// when PAGO03 goes high, RUPO02 goes high
// when ROPO16 goes high, RUPO02 goes low.


// Clock polarity wrong? Check regs on die.

// BFETCH_000 - LABU_TILE_DB_CLKp = 1;
// BFETCH_001 - LABU_TILE_DB_CLKp = or2(!vid_reg.RENDERING_LATCHp.qp(), vid_reg.BFETCH_S0_DELAY.qp())
// BFETCH_010 - LABU_TILE_DB_CLKp = 1;
// BFETCH_011 - LABU_TILE_DB_CLKp = 1;
// BFETCH_100 - LABU_TILE_DB_CLKp = 1;
// BFETCH_101 - LABU_TILE_DB_CLKp = or2(!vid_reg.RENDERING_LATCHp.qp(), vid_reg.BFETCH_S0_DELAY.qp())

// BFETCH_000 - LOMA_BG_LATCHn = 1;
// BFETCH_001 - LOMA_BG_LATCHn = 1;
// BFETCH_010 - LOMA_BG_LATCHn = 1;
// BFETCH_011 - LOMA_BG_LATCHn = or2(!vid_reg.RENDERING_LATCHp.qp(), vid_reg.BFETCH_S0_DELAY.qp());
// BFETCH_100 - LOMA_BG_LATCHn = 1;
// BFETCH_101 - LOMA_BG_LATCHn = 1;

// LEGU_01 << LOMA_02    CLKp
// LEGU_02 << VRM_BUS_D0 D
// LEGU_03 nc
// LEGU_04 << COMP_CLK   CLKn
// LEGU_05 nc
// LEGU_06 nc
// LEGU_07 >> LUHE_01, LAKY_02 Q
// LEGU_08 >> NC         Qn



// BEBU_01 << DOBA_17
// BEBU_02 << BALU_02
// BEBU_03 << BYBA_16
// BEBU_04 nc
// BEBU_05 >> AVAP_01

// Arms on ground, nor4 latch
// BESU01 << CATU17
// BESU02 nc
// BESU03 >> nc
// BESU04 >> ACYL02
// BESU05 nc
// BESU06 << ASEN04

// When CATU goes high, BESU goes high
// When ASEN goes high, BESU goes low.


// if TYNU is and2(or()) things don't make sense.
///*p08.TYNU*/ wire TYNU_ADDR_RAM = and2(ADDR >= 0x4000, TUMA_CART_RAM);

// Die trace:
// TOZA = and2(TYNU, ABUZ, TUNA);
// TYHO = mux2_p(LUMA, MARU.QN?, TOZA);

// TOZA = address valid, address ram, address not highmem
// The A15 in the other half of the mux is weird.

// SOMA01 << LAVO04
// SOMA02 nc
// SOMA03 << D0_C
// SOMA04 nc
// SOMA05 nc
// SOMA06 nc
// SOMA07 >> RYMA04
// SOMA08 nc
// SOMA09 == nc

/*
0b000xxxxx_xxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxx_xxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxx_xxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxx_xxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxx_xxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxx_xxxxxxxx - 0xA000 to 0xBFFF (cart ram)
0b110xxxxx_xxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxx_xxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)
0b11111110_00000000 - 0xFE00 - OAM begin
0b11111110_10011111 - 0xFE9F - OAM end
0b11111111_00000000 - 0xFF00 - IO begin
0b11111111_01111111 - 0xFF7F - IO end
0b11111111_10000000 - 0xFF80 - Zeropage begin
0b11111111_11111110 - 0xFFFE - Zeropage end
0b11111111_11111111 - 0xFFFF - Interrupt enable
*/

// MORY00 << MATU17
// MORY01
// MORY02 
// -> MORY reads DMA_RUNNING.qp

// arm on vcc side
// LOXO01 << MULE02
// LOXO02 << TEXO04
// LOXO03 nc
// LOXO04 << UMUT04
// LOXO05 >> LASY01

// Schematic wrong, AVER is def nand2
// AVER01 << ACYL03
// AVER02 << XYSO02
// AVER03 >> BYCU03

// VAPE is def and
// VAPE01 << TACU03
// VAPE02 << TUVO04
// VAPE03 nc


// Possible schematic error - CUFE doesn't make sense as or2(and()), only as and2(or())

// 4-rung whereas the or2(and()) were 5 rung?
// Arm on left (gnd) side
// CUFE01 << SARO03
// CUFE02 << MATU17
// CUFE03 >> BYCU01
// CUFE04 << MOPA03

// Schematic very wrong
// TUVO01 << TEPA02
// TUVO02 << TULY17
// TUVO03 << TESE17
// TUVO04 >> VAPE02

// WEFY01
// WEFY02
// WEFY03
// WEFY04

// AJEP def nand2
// XUJA def not
// BOTA def nand2
// ASYT def and
// BODE def not
// YVAL def not
// YRYV def not
// ZODO def not

//------------------------------------------------------------------------------

// cpu can read oam when there's no parsing, rendering, or dma
// so byte 0 is the palette number? something wrong here...
// this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
// except the inputs are negated or something?

// NAXY01 << UVYT02
// NAXY02 << MAKA17
// NAXY03 >> POWU02

// schematic says naxy = nor4(maka, luvy), but wrong
// naxy = nor4(uvyt, maka)


// NOR latch
// POKY00 << PYGO
// POKY01 nc
// POKY02 >> nc
// POKY03 >> ROMO00, others
// POKY04 nc
// POKY05 << LOBY01
// If PYGO goes high, POKY03 goes high
// if LOBY goes high, POKY03 goes low.
// POKY.nor_latch(PYGO, LOBY)

// SOBU_01 SC
// SOBU_02 << TAVA_02 CLKp
// SOBU_03 SC
// SOBU_04 NC
// SOBU_05 NC
// SOBU_06 << VYPO    RSTn 
// SOBU_07 << TEKY_06 D
// SOBU_08 NC
// SOBU_09 SC
// SOBU_10 NC
// SOBU_11 NC
// SOBU_12 SC
// SOBU_13 << VYPO    RSTn
// SOBU_14 NC
// SOBU_15 NC
// SOBU_16 >> NC      QN
// SOBU_17 >> SUDA_07 Q

// TAKA has arms on the VCC side - nand2 latch
// TAKA01 << VEKU02
// TAKA02 nc
// TAKA03 >> nc
// TAKA04 >> SOWO00
// TAKA05 nc
// TAKA06 << SECA03
// if SECA03 goes low, TAKA04 goes high
// if VEKU02 goes low, TAKA04 goes low

// VAPE04 >> XUJY01
// SAKY = nor4(TULY17, VONU17)
// TEPA = not1(XYMU)
// TYSO = or2(SAKY, TEPA)
// TEXY = not1(TYSO)

// TEVY box color wrong on die trace, but schematic correct.

// Die trace:
// SORE = not1(A15)
// TEVY = or2(A13, A13, SORE) // A13 line not fully drawn
// TEXO = and2(ADDR_VALIDx?, TEVY)

// UJYV01 << UNOR04
// UJYV02 << (mux 1) RD_C
// UJYV03 << (mux 0) CPU_RAW_RD
// UJYV04 nc
// UJYV05 >> TEDO01

// UBAL1 << UNOR04
// UBAL2 << (mux1) << WR_C
// UBAL3 << (mux0) << APOV04
// UBAL4 nc
// UBAL5 >>

// Die trace

// BYHA = weirdgate
// BYHA01 << ANEL17
// BYHA02 << ABAF02
// BYHA03 xx CATU17 (crossover?)
// BYHA04 << ABEZ02
// BYHA05 >> ATEJ01

// ATEJ03 = not1(BYHA05)
// ANOM03 = nor4(ATEJ03, ATAR02)
// BALU02 = not1(ANOM03)
// BEBU05 = or2(DOBA17, BALU02, BYBA16)
// AVAP03 = not1(BEBU05)
// NYXU04 = nor4(AVAP03, MOSU03, TEVO05)

// Die trace:

// WUSA arms on the ground side, nor4 latch
// WUSA00 << XAJO03
// WUSA01 nc
// WUSA02 >> nc
// WUSA03 >> TOBA00
// WUSA04 nc 
// WUSA05 << WEGO03

// When XAJO03 goes high, WUSA03 goes high.
// When WEGO03 goes high, WUSA03 goes low.

// XEHO01 <> XEHO12
// XEHO02 << SACU04
// XEHO03 <> XEHO09
// XEHO04 nc
// XEHO05 nc
// XEHO06 << XYMU03
// XEHO07 << XEHO16 // so it's a counter
// XEHO08 nc
// XEHO09 <> XEHO03
// XEHO10 nc
// XEHO11 ?? RUMA01
// XEHO12 <> XEHO01
// XEHO13 << XYMU03
// XEHO14 nc
// XEHO15 nc
// XEHO16
// XEHO17 >> XAJO01

// ROXY = NOR latch
// ROXY00 << PAHA
// ROXY01 nc
// ROXY02 >> nc
// ROXY03 >> RONE00
// ROXY04 nc
// ROXY05 << POVA

// If PAHA goes high, ROXY03 goes high.
// If POVA goes high, ROXY03 goes low

// LURY01 << LOVY15 (next to bottom rung)
// LURY02 << XYMU02
// LURY03 nc
// LURY04 >> LONY01

// LONY has "arms" on the VCC side - different from the other latches?
// Probably a NAND latch instead of NOR
// LONY01 << LURY03
// LONY02 nc
// LONY03 == nc
// LONY04 >> LUSU01, MYMA01
// LONY05 nc
// LONY06 << NYXU03

// if LURY goes low, LONY goes low
// if NYXU goes low, LONY goes high

// XYMU has "arms" on the ground side
// XYMU00 << WEGO03
// XYMU01 nc
// XYMU02 >> bunch of stuff
// XYMU03 >> nc
// XYMU04 nc
// XYMU05 << AVAP02

// if AVAP02 goes high, XYMU02 goes high.
// if WEGO03 goes high, XYMU02 goes low.


// Die trace:
// VYBO = nor4(FEPO04, WODU04, MYVO02)
// TYFA = and2(SOCY02, POKY04, VYBO04)
// SEGU = not1(TYFA05) // 5 rung inverter, because fanout?
// ROXO = not1(SEGU05)

// NEFO_01 << LOMA_02    (clk)
// NEFO_02 << BUS_CPU_D7p     (d)
// NEFO_03 nc
// NEFO_04 << COMP_CLOCK (clkn)
// NEFO_05 nc
// NEFO_06 nc
// NEFO_07 >> NAJA_02    (Q)
// NEFO_08 nc

// ASOL_01 << AFAR_03
// ASOL_02 nc
// ASOL_03 >> AVOR_02
// ASOL_04 >> nc
// ASOL_05 nc
// ASOL_06 << SYS_PIN_RST

// AFAR_03 high = ASOL_03 low
// SYS_PIN_RST high = ASOL_03 high

// AFER_01 nc
// AFER_02 << UPOJ_04 (RSTp?)
// AFER_03 << ASOL_03 (D)
// AFER_04 nc
// AFER_05 << BOGA_07 (CLKp?)
// AFER_06 nc
// AFER_07 nc
// AFER_08 << BOMA_07 (CLKn?)
// AFER_09 << UPOJ_04 (RSTp?)
// AFER_10 nc
// AFER_11 nc
// AFER_12 >> nc
// AFER_13 >> AVOR_01

// Latch w/ arms on the ground side, output on the top rung - nor4 latch with inverted output
    
// UCOB_01 << SYS_PIN_CLK_GOOD
// UCOB_02 >> UPYF_02, UFOL_01

// UPYF_01 << SYS_PIN_RST
// UPYF_02 << UCOB_02
// UPYF_03 nc
// UPYF_04 >> TUBO_06

// TUBO_00 << CLKREQ
// TUBO_01 nc
// TUBO_02 >> UNUT 
// TUBO_03 == nc
// TUBO_04 nc
// TUBO_05 << UPYF

// UPYF high, TUBO_02 high
// CLKREQ high, TUBO_02 low


// UNUT_01 << TUBO_03
// UNUT_02 << UPOF_17 (Q)
// UNUT_03 nc
// UNUT_04 >> TABA_03

// TABA_01 << UNOR_04 
// TABA_02 << UMUT_04
// TABA_03 << UNUT_04
// TABA_04 nc
// TABA_05 >> ALYP_01
// TABA = or2(UNOR, UMUT, UNUT)

// ALYP_01 << TABA_05
// ALYP_02 >> AFAR_02

// AFAR_01 << SYS_PIN_RSTn
// AFAR_02 << ALYP_02
// AFAR_03 >> ASOL_01

// ASOL_01 << AFAR_03
// ASOL_02 nc
// ASOL_03 >> AVOR_02
// ASOL_04 >> nc
// ASOL_05 nc
// ASOL_06 << SYS_PIN_RSTn

// UPOJ_01 << UBET_02
// UPOJ_02 << UVAR_02
// UPOJ_03 << SYS_PIN_RST
// UPOJ_04 >> AFER ADYK APUK ALEF AFUR
// UPOJ = nand2(!T1, !T2, RSTn);

// AFER_01 nc
// AFER_02 << UPOJ_04 (RSTp?)
// AFER_03 << ASOL_03 (D)
// AFER_04 nc
// AFER_05 << BOGA_07 (CLKp?)
// AFER_06 nc
// AFER_07 nc
// AFER_08 << BOMA_07 (CLKn?)
// AFER_09 << UPOJ_04 (RSTp?)
// AFER_10 nc
// AFER_11 nc
// AFER_12 >> nc
// AFER_13 >> AVOR_01

// AVOR_01 << AFER_13
// AVOR_02 << ASOL_03
// AVOR_03 >> ALUR_01

// TUBO_00 << CLKREQ
// TUBO_01 nc
// TUBO_02 >> UNUT 
// TUBO_03 == nc
// TUBO_04 nc
// TUBO_05 << UPYF

// CLKREQ high, TUBO_02 low
// UPYF high,   TUBO_02 high


// UPYF = or2(SYS_PIN_RST ,UCOB)
// UPYF_01 << SYS_PIN_RST
// UPYF_02 << UCOB_02
// UPYF_03 nc
// UPYF_04 >> TUBO_06


// UCOB = not1(SYS_PIN_CLK_A)
// UCOB_01 << SYS_PIN_CLK_A
// UCOB_02 >> UPYF_02, UFOL_01


// SYS_PIN_RST >> UPOJ, UPYF, AFAR, ASOL, UFOL



// UCOB = not1(SYS_PIN_CLK_A)
// UCOB_01 << SYS_PIN_CLK_A

// UFOL_01 << UCOB_02
// UFOL_02 << SYS_PIN_RST
// UFOL_03 << TAPE_06
// UFOL_04 >> (div resets)

// UKUP_01 sc
// UKUP_02 << BOGA_05 CLKp
// UKUP_03 sc
// UKUP_04 NC
// UKUP_05 NC
// UKUP_06 << UFOL_04 RSTn
// UKUP_07 << UKUP_16 D
// UKUP_08 NC
// UKUP_09 sc
// UKUP_10 NC
// UKUP_11 NC
// UKUP_12 sc
// UKUP_13 << UFOL_04  RSTn
// UKUP_14 NC
// UKUP_15 NC
// UKUP_16 >> UFOR_02, UKUP_16  QN   _MUST_ be QN - see DIV
// UKUP_17 >> nc                Q    _MUST_ be Q  - see DIV

// So this makes no sense. SYS_PIN_RSTn high -> DIV always reset. But SYS_PIN_RSTn is high in run mode.
// UCOB_CLKBAD low in run mode 

// If TAPE is going to do anything here, UCOB and RST _must_ be low



// UCOB = not1(SYS_PIN_CLK_A)
// UCOB_01 << SYS_PIN_CLK_A

// UFOL_01 << UCOB_02
// UFOL_02 << SYS_PIN_RST
// UFOL_03 << TAPE_06
// UFOL_04 >> (div resets)

// UKUP_01 sc
// UKUP_02 << BOGA_05 CLKp
// UKUP_03 sc
// UKUP_04 NC
// UKUP_05 NC
// UKUP_06 << UFOL_04 RSTn
// UKUP_07 << UKUP_16 D
// UKUP_08 NC
// UKUP_09 sc
// UKUP_10 NC
// UKUP_11 NC
// UKUP_12 sc
// UKUP_13 << UFOL_04  RSTn
// UKUP_14 NC
// UKUP_15 NC
// UKUP_16 >> UFOR_02, UKUP_16  QN   _MUST_ be QN - see DIV
// UKUP_17 >> nc                Q    _MUST_ be Q  - see DIV

// So this makes no sense. SYS_PIN_RSTn high -> DIV always reset. But SYS_PIN_RSTn is high in run mode.
// UCOB_CLKBAD low in run mode 

// If TAPE is going to do anything here, UCOB and RST _must_ be low

