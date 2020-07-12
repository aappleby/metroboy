// NOR latch
// POKY00 << PYGO
// POKY01 nc
// POKY02 >> nc
// POKY03 >> ROMO00, others
// POKY04 nc
// POKY05 << LOBY01
// If PYGO goes high, POKY03 goes high
// if LOBY goes high, POKY03 goes low.

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

// TAKA has arms on the VCC side - nand latch
// TAKA01 << VEKU02
// TAKA02 nc
// TAKA03 >> nc
// TAKA04 >> SOWO00
// TAKA05 nc
// TAKA06 << SECA03
// if SECA03 goes low, TAKA04 goes high
// if VEKU02 goes low, TAKA04 goes low

// VAPE04 >> XUJY01
// SAKY = nor(TULY17, VONU17)
// TEPA = not(XYMU)
// TYSO = or(SAKY, TEPA)
// TEXY = not(TYSO)

// TEVY box color wrong on die trace, but schematic correct.

// Die trace:
// SORE = not(A15)
// TEVY = or(A13, A13, SORE) // A13 line not fully drawn
// TEXO = and(ADDR_VALIDx?, TEVY)

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

// ATEJ03 = not(BYHA05)
// ANOM03 = nor(ATEJ03, ATAR02)
// BALU02 = not(ANOM03)
// BEBU05 = or(DOBA17, BALU02, BYBA16)
// AVAP03 = not(BEBU05)
// NYXU04 = nor(AVAP03, MOSU03, TEVO05)

// Die trace:

// WUSA arms on the ground side, nor latch
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
// VYBO = nor(FEPO04, WODU04, MYVO02)
// TYFA = and(SOCY02, POKY04, VYBO04)
// SEGU = not(TYFA05) // 5 rung inverter, because fanout?
// ROXO = not(SEGU05)

// NEFO_01 << LOMA_02    (clk)
// NEFO_02 << CPU_TRI_D7     (d)
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

// Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
    
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

