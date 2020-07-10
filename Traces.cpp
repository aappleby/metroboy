// UNUT_01 << TUBO_03
// UNUT_02 << UPOF_17 (Q)
// UNUT_03 nc
// UNUT_04 >> TABA_03

// TABA_01 << UNOR_04 
// TABA_02 << UMUT_04
// TABA_03 << UNUT_04
// TABA_04 nc
// TABA_05 >> ALYP_01
// TABA = or(UNOR, UMUT, UNUT)

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
// UPOJ = nand(!T1, !T2, RSTn);

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


// UPYF = or(SYS_PIN_RST ,UCOB)
// UPYF_01 << SYS_PIN_RST
// UPYF_02 << UCOB_02
// UPYF_03 nc
// UPYF_04 >> TUBO_06


// UCOB = not(SYS_PIN_CLK_A)
// UCOB_01 << SYS_PIN_CLK_A
// UCOB_02 >> UPYF_02, UFOL_01


// SYS_PIN_RST >> UPOJ, UPYF, AFAR, ASOL, UFOL



// UCOB = not(SYS_PIN_CLK_A)
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



// UCOB = not(SYS_PIN_CLK_A)
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

