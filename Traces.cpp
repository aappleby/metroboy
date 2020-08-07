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
// POKY = nor_latch(PYGO, LOBY)

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
// NEFO_02 << CPU_BUS_D7     (d)
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

