#include "System.h"

//-----------------------------------------------------------------------------

void System::reset() {
  RESET_REG.reset();
  BOOT_BIT.reset();

  CLK_ABCDxxxx.reset();
  CLK_xBCDExxx.reset();
  CLK_xxCDEFxx.reset();
  CLK_xxxDEFGx.reset();
  ATYK.reset();
  AVOK.reset();
  CLK_512K.reset();
  AJER_2M.reset();
  CLK_512.reset();
  CLK_256.reset();
  CLK_128.reset();

  DIV_00.reset();
  DIV_01.reset();
  DIV_02.reset();
  DIV_03.reset();
  DIV_04.reset();
  DIV_05.reset();
  DIV_06.reset();
  DIV_07.reset();
  DIV_08.reset();
  DIV_09.reset();
  DIV_10.reset();
  DIV_11.reset();
  DIV_12.reset();
  DIV_13.reset();
  DIV_14.reset();
  DIV_15.reset();

  TIMA_0.reset();
  TIMA_1.reset();
  TIMA_2.reset();
  TIMA_3.reset();
  TIMA_4.reset();
  TIMA_5.reset();
  TIMA_6.reset();
  TIMA_7.reset();
  TMA_0.reset();
  TMA_1.reset();
  TMA_2.reset();
  TMA_3.reset();
  TMA_4.reset();
  TMA_5.reset();
  TMA_6.reset();
  TMA_7.reset();
  TAC_0.reset();
  TAC_1.reset();
  TAC_2.reset();
  TIMA_MAX.reset();
  INT_TIMER.reset();

  DMA_RSTn2.reset();
  DMA_RSTn1.reset();
  DMA_DONE_SYNC.reset();
  DMA_RUNNING_SYNC.reset();
  DMA_A00.reset();
  DMA_A01.reset();
  DMA_A02.reset();
  DMA_A03.reset();
  DMA_A04.reset();
  DMA_A05.reset();
  DMA_A06.reset();
  DMA_A07.reset();
  DMA_A08.reset();
  DMA_A09.reset();
  DMA_A10.reset();
  DMA_A11.reset();
  DMA_A12.reset();
  DMA_A13.reset();
  DMA_A14.reset();
  DMA_A15.reset();

  XFER_START.reset();
  XFER_DIR.reset();
  SER_CLK.reset();
  SER_CNT0.reset();
  SER_CNT1.reset();
  SER_CNT2.reset();
  SER_CNT3.reset();
  SER_DATA0.reset();
  SER_DATA1.reset();
  SER_DATA2.reset();
  SER_DATA3.reset();
  SER_DATA4.reset();
  SER_DATA5.reset();
  SER_DATA6.reset();
  SER_DATA7.reset();
  SER_OUT.reset();

  JOYP_RA.reset();
  JOYP_LB.reset();
  JOYP_UC.reset();
  JOYP_DS.reset();
  KELY.reset();
  COFY.reset();
  DBG_FF00_D6.reset();
  DBG_FF00_D7.reset();
  JP_GLITCH0.reset();
  JP_GLITCH1.reset();
  JP_GLITCH2.reset();
  JP_GLITCH3.reset();
}

//-----------------------------------------------------------------------------

void System::tick(Gameboy& gb, bool LCDC_EN, bool APU_RESET1, bool DBG_APUn, bool FF46) {

  //----------
  // CPU reset

  /*p01.ASOL*/ RESET_IN  = gb.chip_in.RST;
  /*p01.AVOR*/ SYS_RESET = RESET_REG || RESET_IN;
  /*p01.XODO*/ VID_RESET = SYS_RESET || !LCDC_EN;

  //----------
  // Clock tree

  /*p01.ARYS*/ CLK_AxCxExGx = !gb.chip_in.CLKIN_B;
  /*p01.ANOS*/ CLK_xBxDxFxH =  gb.chip_in.CLKIN_B;

  //----------
  // Phase generator.

  /*p01.ATYP*/ CLK_xxxxEFGH = _not(CLK_ABCDxxxx);
  /*p01.AFEP*/ CLK_AxxxxFGH = _not(CLK_xBCDExxx);
  /*p01.AROV*/ CLK_ABxxxxGH = _not(CLK_xxCDEFxx);
  /*p01.ADAR*/ CLK_ABCxxxxH = _not(CLK_xxxDEFGx);

  /*p01.AFAS*/ CLK_xxxDxxxx = _nor(CLK_ABCxxxxH, CLK_xxxxEFGH);
  /*p01.BELE*/ CLK_xxxxxFGH = _and(CLK_AxxxxFGH, CLK_xxxxEFGH);

  //----------
  // CPU clocks

  /*p01.BAPY*/ CPUCLK_xxDExxxx = _nor(CLK_ABxxxxGH, CLK_xxxxEFGH);
  /*p01.NULE*/ CPUCLK_ABCDxxxx = _not(CLK_xxxxEFGH);
  /*p01.BATE*/ CPUCLK_xxxxxFxx = _nor(CLK_xBCDExxx, CLK_ABxxxxGH);
  /*p01.BERU*/ CPUCLK_ABxxEFGH = _not(CPUCLK_xxDExxxx);
  /*p01.BYRY*/ CPUCLK_xxxxEFGH = _not(CPUCLK_ABCDxxxx);

  /*p01.BOGA*/ DIV_CLK = !CLK_xxxxxFGH;
  
  //----------
  // APU clocks

  /*p01.GALE*/ CLK_512b = mux2(CLK_512K, /*p01.FYNE*/ CLK_512, DBG_APUn); // dbg_apu polarity?
  /*p01.BEZE*/ CLK_256b = mux2(CLK_512K, /*p01.CULO*/ CLK_256, DBG_APUn); // dbg_apu polarity?
  /*p01.BULE*/ CLK_128b = mux2(CLK_512K, /*p01.APEF*/ CLK_128, DBG_APUn); // dbg_apu polarity?

  // these are used by all the channels
  /*p01.HORU*/ CLK_512a = CLK_512b;
  /*p01.BUFY*/ CLK_256a = CLK_256b;
  /*p01.BYFE*/ CLK_128a = CLK_128b;

  //----------
  // P02
  
  /*p02.KERY*/ wire ANY_BUTTON = _or(gb.chip_in.P13_C, gb.chip_in.P12_C, gb.chip_in.P11_C, gb.chip_in.P10_C);
  /*p02.AWOB*/ TO_CPU2.latch(DIV_CLK, ANY_BUTTON);
  /*p02.ASOK*/ INT_JP = _and(JP_GLITCH3, JP_GLITCH0);

  //----------

  // This needs to be updated after the clock tree fixes in Sch_System.cpp

  wire CLK_ABCDxxGH = _or(CLK_ABCDxxxx, CLK_ABxxxxGH);
  /*p01.ABUZ*/ wire ADDR_VALID_SYNCn  = _nand(CLK_ABCDxxGH, gb.cpu_in.ADDR_VALID);

  // Seems wrong.


  /*p07.TEDO*/ CPU_RD   = gb.cpu_in.CPU_RAW_RD;
  /*p07.TAPU*/ CPU_WR   = _and(gb.cpu_in.CPU_RAW_WR, CLK_xxxDxxxx);

  wire ADDR_VRAM = _and(!gb.bus.A13, !gb.bus.A14, gb.bus.A15);
  wire ADDR_VALID_AND_NOT_VRAM = _and(gb.cpu_in.ADDR_VALID, !ADDR_VRAM);

  // this seems wrong, CPU_RAW_RD cancels out. could be right tho...
  ///*p08.LULA*/ DBUS_OUT = _and(ADDR_VALID_AND_NOT_VRAM, _or(_and(gb.cpu_in.CPU_RAW_RD, !ADDR_VALID_AND_NOT_VRAM), gb.cpu_in.CPU_RAW_WR));

  if (ADDR_VALID_AND_NOT_VRAM) {
    /*p08.LULA*/ DBUS_OUT = gb.cpu_in.CPU_RAW_WR;
  }
  else {
    /*p08.LULA*/ DBUS_OUT = 0;
  }

#if 0
  // TYNU
  // 0xC000+
  // 0xA000-BFFF
  // TYNU = addr >= 0xA000?
  /*p08.TYNU*/ wire TYNU     = _or(_and(gb.bus.A15, gb.bus.A14), _and(gb.bus.A13, !gb.bus.A14, gb.bus.A15)); // not sure this is right

  bool ADDR_A000_FDFF = (gb.bus.A >= 0xA000) && (gb.bus.A <= 0xFDFF);

  /*p08.TOZA*/ wire CART_CS  = _and(ADDR_VALID_SYNC, ADDR_A000_FDFF);


  /*p08.TYMU*/ wire CART_RD  = _nor(DMA_ADDR_EXT, !DBUS_OUT);
  /*p08.PUVA*/ wire CART_WR  = _or (DMA_ADDR_EXT, !CPU_WR, _not(ADDR_VALID_AND_NOT_VRAM));

  /*p08.TYHO*/ gb.chip_out.CS_A = mux2(DMA_A15, CART_CS, DMA_ADDR_EXT); // polarity?
  /*p08.UGAC*/ gb.chip_out.RD_A = !CART_RD;
  /*p08.URUN*/ gb.chip_out.RD_D = !CART_RD;
  /*p08.UVER*/ gb.chip_out.WR_A = !CART_WR;
  /*p08.USUF*/ gb.chip_out.WR_D = !CART_WR;
#endif

  //----------
  // this chunk seems reasonable now, but the chip_out.CS is bogus

  wire DMA_SRC_VRAM = _and(DMA_A15, !DMA_A14, !DMA_A13);
  wire DMA_ADDR_EXT = _and(DMA_RUNNING_SYNC, !DMA_SRC_VRAM);
  wire VRAM_TO_OAM  = _and(DMA_RUNNING_SYNC,  DMA_SRC_VRAM);
  
  {
    gb.chip_out.WR_A = 0;
    gb.chip_out.WR_D = 0;
    gb.chip_out.RD_A = 1;
    gb.chip_out.RD_D = 1;

    if (!DMA_ADDR_EXT && !ADDR_VRAM && gb.cpu_in.ADDR_VALID && gb.cpu_in.CPU_RAW_WR) {
      gb.chip_out.WR_A = CLK_xxxDxxxx;
      gb.chip_out.WR_D = CLK_xxxDxxxx;
      gb.chip_out.RD_A = 0;
      gb.chip_out.RD_D = 0;
    }
  }

  //----------
  // FF0F IF
  
  {
#if 0
    /*p07.REFA*/ wire FF0F_WR  = CPU_WR && gb.bus.A == 0xFF0F;

    /*p02.LUFE*/ wire INT_STAT_ACK = _not(gb.cpu_in.FROM_CPU7);
    /*p02.LEJA*/ wire INT_SER_ACK  = _not(gb.cpu_in.FROM_CPU8);
    /*p02.LETY*/ wire INT_VBL_ACK  = _not(gb.cpu_in.FROM_CPU9);
    /*p02.LESA*/ wire INT_JOY_ACK  = _not(gb.cpu_in.FROM_CPU10);
    /*p02.LAMO*/ wire INT_TIM_ACK  = _not(gb.cpu_in.FROM_CPU11);

    /*p02.MYZU*/ wire FF0F_SET0 = _nand(FF0F_WR, INT_VBL_ACK,  gb.bus.D0);
    /*p02.MODY*/ wire FF0F_SET1 = _nand(FF0F_WR, INT_SER_ACK,  gb.bus.D1);
    /*p02.PYHU*/ wire FF0F_SET2 = _nand(FF0F_WR, INT_JOY_ACK,  gb.bus.D2);
    /*p02.TOME*/ wire FF0F_SET3 = _nand(FF0F_WR, INT_STAT_ACK, gb.bus.D3);
    /*p02.TOGA*/ wire FF0F_SET4 = _nand(FF0F_WR, INT_TIM_ACK,  gb.bus.D4);

    /*p02.LYTA*/ wire FF0F_RST0 = _and(_or(gb.bus.D0, !FF0F_WR), INT_VBL_ACK,  !SYS_RESET);
    /*p02.MOVU*/ wire FF0F_RST1 = _and(_or(gb.bus.D1, !FF0F_WR), INT_SER_ACK,  !SYS_RESET);
    /*p02.PYGA*/ wire FF0F_RST2 = _and(_or(gb.bus.D2, !FF0F_WR), INT_JOY_ACK,  !SYS_RESET);
    /*p02.TUNY*/ wire FF0F_RST3 = _and(_or(gb.bus.D3, !FF0F_WR), INT_STAT_ACK, !SYS_RESET);
    /*p02.TYME*/ wire FF0F_RST4 = _and(_or(gb.bus.D4, !FF0F_WR), INT_TIM_ACK,  !SYS_RESET);

    /*p02.PESU*/ wire FF0F_IN = not(chip_in.P10_B);

    /*p02.LOPE*/ FF0F_0.tock(b.vid.INT_VBL_BUF, FF0F_SET0, FF0F_RST0, FF0F_IN);
    /*p02.UBUL*/ FF0F_1.tock(SER_CNT3,          FF0F_SET1, FF0F_RST1, FF0F_IN);
    /*p02.ULAK*/ FF0F_2.tock(INT_JP,            FF0F_SET2, FF0F_RST2, FF0F_IN);
    /*p02.LALU*/ FF0F_3.tock(b.vid.INT_STAT,    FF0F_SET3, FF0F_RST3, FF0F_IN);
    /*p02.NYBO*/ FF0F_4.tock(INT_TIMER,         FF0F_SET4, FF0F_RST4, FF0F_IN);

    wire FF0F_RD = CPU_RD && gb.bus.A == 0xFF0F;

    /*p02.MATY*/ FF0F_L0.latch(!FF0F_RD, FF0F_0);
    /*p02.NEJY*/ FF0F_L1.latch(!FF0F_RD, FF0F_1);
    /*p02.NUTY*/ FF0F_L2.latch(!FF0F_RD, FF0F_2);
    /*p02.MOPO*/ FF0F_L3.latch(!FF0F_RD, FF0F_3);
    /*p02.PAVY*/ FF0F_L4.latch(!FF0F_RD, FF0F_4);

    /*p02.NELA*/ if (FF0F_RD) gb.bus.D0 = FF0F_L0;
    /*p02.NABO*/ if (FF0F_RD) gb.bus.D1 = FF0F_L1;
    /*p02.ROVA*/ if (FF0F_RD) gb.bus.D2 = FF0F_L2;
    /*p02.PADO*/ if (FF0F_RD) gb.bus.D3 = FF0F_L3;
    /*p02.PEGY*/ if (FF0F_RD) gb.bus.D4 = FF0F_L4;
#endif
  }


  {
    wire FF00_RD = CPU_RD && gb.bus.A == 0xFF00;

    /*p05.KEVU*/ JOYP_L0.latch(!FF00_RD, gb.chip_in.P10_C);
    /*p05.KAPA*/ JOYP_L1.latch(!FF00_RD, gb.chip_in.P11_C);
    /*p05.KEJA*/ JOYP_L2.latch(!FF00_RD, gb.chip_in.P12_C);
    /*p05.KOLO*/ JOYP_L3.latch(!FF00_RD, gb.chip_in.P13_C);
  }
  //----------
  // Address pin driver

  {
    /*p08.LOXO*/ wire ADDR_LATCH = ADDR_VALID_AND_NOT_VRAM;
    /*p08.ALOR*/ ADDR_LATCH_00.latch(ADDR_LATCH, gb.bus.A00);
    /*p08.APUR*/ ADDR_LATCH_01.latch(ADDR_LATCH, gb.bus.A01);
    /*p08.ALYR*/ ADDR_LATCH_02.latch(ADDR_LATCH, gb.bus.A02);
    /*p08.ARET*/ ADDR_LATCH_03.latch(ADDR_LATCH, gb.bus.A03);
    /*p08.AVYS*/ ADDR_LATCH_04.latch(ADDR_LATCH, gb.bus.A04);
    /*p08.ATEV*/ ADDR_LATCH_05.latch(ADDR_LATCH, gb.bus.A05);
    /*p08.AROS*/ ADDR_LATCH_06.latch(ADDR_LATCH, gb.bus.A06);
    /*p08.ARYM*/ ADDR_LATCH_07.latch(ADDR_LATCH, gb.bus.A07);
    /*p08.LUNO*/ ADDR_LATCH_08.latch(ADDR_LATCH, gb.bus.A08);
    /*p08.LYSA*/ ADDR_LATCH_09.latch(ADDR_LATCH, gb.bus.A09);
    /*p08.PATE*/ ADDR_LATCH_10.latch(ADDR_LATCH, gb.bus.A10);
    /*p08.LUMY*/ ADDR_LATCH_11.latch(ADDR_LATCH, gb.bus.A11);
    /*p08.LOBU*/ ADDR_LATCH_12.latch(ADDR_LATCH, gb.bus.A12);
    /*p08.LONU*/ ADDR_LATCH_13.latch(ADDR_LATCH, gb.bus.A13);
    /*p08.NYRE*/ ADDR_LATCH_14.latch(ADDR_LATCH, gb.bus.A14);

    // this is super weird
    /*p07.TUTU*/ wire ADDR_BOOT  = _and(!BOOT_BIT, gb.bus.A <= 0xFF);
    /*p08.SEPY*/ ADDR_LATCH_15 = _or(!ADDR_VALID_SYNCn, gb.bus.A15, ADDR_BOOT);


    /*p08.AMET*/ wire ADDR_MUX_00 = mux2(DMA_A00, ADDR_LATCH_00, DMA_ADDR_EXT);
    /*p08.ATOL*/ wire ADDR_MUX_01 = mux2(DMA_A01, ADDR_LATCH_01, DMA_ADDR_EXT);
    /*p08.APOK*/ wire ADDR_MUX_02 = mux2(DMA_A02, ADDR_LATCH_02, DMA_ADDR_EXT);
    /*p08.AMER*/ wire ADDR_MUX_03 = mux2(DMA_A03, ADDR_LATCH_03, DMA_ADDR_EXT);
    /*p08.ATEM*/ wire ADDR_MUX_04 = mux2(DMA_A04, ADDR_LATCH_04, DMA_ADDR_EXT);
    /*p08.ATOV*/ wire ADDR_MUX_05 = mux2(DMA_A05, ADDR_LATCH_05, DMA_ADDR_EXT);
    /*p08.ATYR*/ wire ADDR_MUX_06 = mux2(DMA_A06, ADDR_LATCH_06, DMA_ADDR_EXT);
    /*p08.ASUR*/ wire ADDR_MUX_07 = mux2(DMA_A07, ADDR_LATCH_07, DMA_ADDR_EXT);
    /*p08.MANO*/ wire ADDR_MUX_08 = mux2(DMA_A08, ADDR_LATCH_08, DMA_ADDR_EXT);
    /*p08.MASU*/ wire ADDR_MUX_09 = mux2(DMA_A09, ADDR_LATCH_09, DMA_ADDR_EXT);
    /*p08.PAMY*/ wire ADDR_MUX_10 = mux2(DMA_A10, ADDR_LATCH_10, DMA_ADDR_EXT);
    /*p08.MALE*/ wire ADDR_MUX_11 = mux2(DMA_A11, ADDR_LATCH_11, DMA_ADDR_EXT);
    /*p08.MOJY*/ wire ADDR_MUX_12 = mux2(DMA_A12, ADDR_LATCH_12, DMA_ADDR_EXT);
    /*p08.MUCE*/ wire ADDR_MUX_13 = mux2(DMA_A13, ADDR_LATCH_13, DMA_ADDR_EXT);
    /*p08.PEGE*/ wire ADDR_MUX_14 = mux2(DMA_A14, ADDR_LATCH_14, DMA_ADDR_EXT);
    /*p08.TAZY*/ wire ADDR_MUX_15 = mux2(DMA_A15, ADDR_LATCH_15, DMA_ADDR_EXT);

    /*p08.KUPO*/ gb.chip_out.A00_A = _not(ADDR_MUX_00);
    /*p08.CABA*/ gb.chip_out.A01_A = _not(ADDR_MUX_01);
    /*p08.BOKU*/ gb.chip_out.A02_A = _not(ADDR_MUX_02);
    /*p08.BOTY*/ gb.chip_out.A03_A = _not(ADDR_MUX_03);
    /*p08.BYLA*/ gb.chip_out.A04_A = _not(ADDR_MUX_04);
    /*p08.BADU*/ gb.chip_out.A05_A = _not(ADDR_MUX_05);
    /*p08.CEPU*/ gb.chip_out.A06_A = _not(ADDR_MUX_06);
    /*p08.DEFY*/ gb.chip_out.A07_A = _not(ADDR_MUX_07);
    /*p08.MYNY*/ gb.chip_out.A08_A = _not(ADDR_MUX_08);
    /*p08.MUNE*/ gb.chip_out.A09_A = _not(ADDR_MUX_09);
    /*p08.ROXU*/ gb.chip_out.A10_A = _not(ADDR_MUX_10);
    /*p08.LEPY*/ gb.chip_out.A11_A = _not(ADDR_MUX_11);
    /*p08.LUCE*/ gb.chip_out.A12_A = _not(ADDR_MUX_12);
    /*p08.LABE*/ gb.chip_out.A13_A = _not(ADDR_MUX_13);
    /*p08.PUHE*/ gb.chip_out.A14_A = _not(ADDR_MUX_14);
    /*p08.SUZE*/ gb.chip_out.A15_A = _not(ADDR_MUX_15);

    /*p08.KOTY*/ gb.chip_out.A00_D = _not(ADDR_MUX_00);
    /*p08.COTU*/ gb.chip_out.A01_D = _not(ADDR_MUX_01);
    /*p08.BAJO*/ gb.chip_out.A02_D = _not(ADDR_MUX_02);
    /*p08.BOLA*/ gb.chip_out.A03_D = _not(ADDR_MUX_03);
    /*p08.BEVO*/ gb.chip_out.A04_D = _not(ADDR_MUX_04);
    /*p08.AJAV*/ gb.chip_out.A05_D = _not(ADDR_MUX_05);
    /*p08.CYKA*/ gb.chip_out.A06_D = _not(ADDR_MUX_06);
    /*p08.COLO*/ gb.chip_out.A07_D = _not(ADDR_MUX_07);
    /*p08.MEGO*/ gb.chip_out.A08_D = _not(ADDR_MUX_08);
    /*p08.MENY*/ gb.chip_out.A09_D = _not(ADDR_MUX_09);
    /*p08.RORE*/ gb.chip_out.A10_D = _not(ADDR_MUX_10);
    /*p08.LYNY*/ gb.chip_out.A11_D = _not(ADDR_MUX_11);
    /*p08.LOSO*/ gb.chip_out.A12_D = _not(ADDR_MUX_12);
    /*p08.LEVA*/ gb.chip_out.A13_D = _not(ADDR_MUX_13);
    /*p08.PAHY*/ gb.chip_out.A14_D = _not(ADDR_MUX_14);
    /*p08.RULO*/ gb.chip_out.A15_D = _not(ADDR_MUX_15);

    /*p04.ECAL*/ if (VRAM_TO_OAM) gb.chip_out.MA00 = DMA_A00;
    /*p04.EGEZ*/ if (VRAM_TO_OAM) gb.chip_out.MA01 = DMA_A01;
    /*p04.FUHE*/ if (VRAM_TO_OAM) gb.chip_out.MA02 = DMA_A02;
    /*p04.FYZY*/ if (VRAM_TO_OAM) gb.chip_out.MA03 = DMA_A03;
    /*p04.DAMU*/ if (VRAM_TO_OAM) gb.chip_out.MA04 = DMA_A04;
    /*p04.DAVA*/ if (VRAM_TO_OAM) gb.chip_out.MA05 = DMA_A05;
    /*p04.ETEG*/ if (VRAM_TO_OAM) gb.chip_out.MA06 = DMA_A06;
    /*p04.EREW*/ if (VRAM_TO_OAM) gb.chip_out.MA07 = DMA_A07;
    /*p04.EVAX*/ if (VRAM_TO_OAM) gb.chip_out.MA08 = DMA_A08;
    /*p04.DUVE*/ if (VRAM_TO_OAM) gb.chip_out.MA09 = DMA_A09;
    /*p04.ERAF*/ if (VRAM_TO_OAM) gb.chip_out.MA10 = DMA_A10;
    /*p04.FUSY*/ if (VRAM_TO_OAM) gb.chip_out.MA11 = DMA_A11;
    /*p04.EXYF*/ if (VRAM_TO_OAM) gb.chip_out.MA12 = DMA_A12;
  }

  //----------
  // External data pin driver.

  {
    /*p08.LULA*/ gb.chip_out.D0_B = DBUS_OUT;
    /*p08.LULA*/ gb.chip_out.D1_B = DBUS_OUT;
    /*p08.LULA*/ gb.chip_out.D2_B = DBUS_OUT;
    /*p08.LULA*/ gb.chip_out.D3_B = DBUS_OUT;
    /*p08.LULA*/ gb.chip_out.D4_B = DBUS_OUT;
    /*p08.LULA*/ gb.chip_out.D5_B = DBUS_OUT;
    /*p08.LULA*/ gb.chip_out.D6_B = DBUS_OUT;
    /*p08.LULA*/ gb.chip_out.D7_B = DBUS_OUT;

    /*p25.RUXA*/ gb.chip_out.D0_A = _nand(gb.bus.D0,  DBUS_OUT);
    /*p25.RUJA*/ gb.chip_out.D1_A = _nand(gb.bus.D1,  DBUS_OUT);
    /*p25.RABY*/ gb.chip_out.D2_A = _nand(gb.bus.D2,  DBUS_OUT);
    /*p25.RERA*/ gb.chip_out.D3_A = _nand(gb.bus.D3,  DBUS_OUT);
    /*p25.RORY*/ gb.chip_out.D4_A = _nand(gb.bus.D4,  DBUS_OUT);
    /*p25.RYVO*/ gb.chip_out.D5_A = _nand(gb.bus.D5,  DBUS_OUT);
    /*p25.RAFY*/ gb.chip_out.D7_A = _nand(gb.bus.D6,  DBUS_OUT);
    /*p25.RAVU*/ gb.chip_out.D6_A = _nand(gb.bus.D7,  DBUS_OUT);

    /*p08.RUNE*/ gb.chip_out.D0_D = _nor (gb.bus.D0, !DBUS_OUT);
    /*p08.RYPU*/ gb.chip_out.D1_D = _nor (gb.bus.D1, !DBUS_OUT);
    /*p08.SULY*/ gb.chip_out.D2_D = _nor (gb.bus.D2, !DBUS_OUT);
    /*p08.SEZE*/ gb.chip_out.D3_D = _nor (gb.bus.D3, !DBUS_OUT);
    /*p08.RESY*/ gb.chip_out.D4_D = _nor (gb.bus.D4, !DBUS_OUT);
    /*p08.TAMU*/ gb.chip_out.D5_D = _nor (gb.bus.D5, !DBUS_OUT);
    /*p08.ROGY*/ gb.chip_out.D6_D = _nor (gb.bus.D6, !DBUS_OUT);
    /*p08.RYDA*/ gb.chip_out.D7_D = _nor (gb.bus.D7, !DBUS_OUT);

    // def nand
    /*p08.LAVO*/ wire CART_RD = _and(gb.cpu_in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAM, gb.cpu_in.FROM_CPU5); // polarity?

    /*p08.RYMA*/ if (CART_RD) gb.bus.D0 = gb.chip_in.D0_C;
    /*p08.RUVO*/ if (CART_RD) gb.bus.D1 = gb.chip_in.D1_C;
    /*p08.RYKO*/ if (CART_RD) gb.bus.D2 = gb.chip_in.D2_C;
    /*p08.TAVO*/ if (CART_RD) gb.bus.D3 = gb.chip_in.D3_C;
    /*p08.TEPE*/ if (CART_RD) gb.bus.D4 = gb.chip_in.D4_C;
    /*p08.SAFO*/ if (CART_RD) gb.bus.D5 = gb.chip_in.D5_C;
    /*p08.SEVU*/ if (CART_RD) gb.bus.D6 = gb.chip_in.D6_C;
    /*p08.TAJU*/ if (CART_RD) gb.bus.D7 = gb.chip_in.D7_C;
  }

  //----------
  // Internal data bus driver

  /*p07.WUTA*/ gb.HRAM_CS = gb.bus.A >= 0xFF80 && gb.bus.A < 0xFFFF;
  /*p07.ZERY*/ gb.BOOT_CS = CPU_RD && !BOOT_BIT && (gb.bus.A <= 0xFF);

  /*p05.KENA*/ gb.chip_out.SOUT  = SER_OUT;
  /*p06.CULY*/ gb.chip_out.SCK_B = XFER_DIR;
  /*p06.KEXU*/ gb.chip_out.SCK_A = _nand(SER_CLK,  XFER_DIR);
  /*p06.KUJO*/ gb.chip_out.SCK_D = _nor (SER_CLK, !XFER_DIR);

  wire FF00_RD = CPU_RD && gb.bus.A == 0xFF00;
  wire FF01_RD = CPU_RD && gb.bus.A == 0xFF01;
  wire FF02_RD = CPU_RD && gb.bus.A == 0xFF02;
  wire FF05_RD = CPU_RD && gb.bus.A == 0xFF05;
  wire FF07_RD = CPU_RD && gb.bus.A == 0xFF07;
  wire FF46_RD = CPU_RD && gb.bus.A == 0xFF46;
  wire FF50_RD = CPU_RD && gb.bus.A == 0xFF50;

  if (FF00_RD) gb.bus.D0 = JOYP_L0;
  if (FF00_RD) gb.bus.D1 = JOYP_L1;
  if (FF00_RD) gb.bus.D2 = JOYP_L2;
  if (FF00_RD) gb.bus.D3 = JOYP_L3;
  if (FF00_RD) gb.bus.D4 = KELY;
  if (FF00_RD) gb.bus.D5 = COFY;

  if (FF01_RD) gb.bus.D0 = SER_DATA0;
  if (FF01_RD) gb.bus.D1 = SER_DATA1;
  if (FF01_RD) gb.bus.D2 = SER_DATA2;
  if (FF01_RD) gb.bus.D3 = SER_DATA3;
  if (FF01_RD) gb.bus.D4 = SER_DATA4;
  if (FF01_RD) gb.bus.D5 = SER_DATA5;
  if (FF01_RD) gb.bus.D6 = SER_DATA6;
  if (FF01_RD) gb.bus.D7 = SER_DATA7;

  if (FF02_RD) gb.bus.D0 = XFER_DIR;
  if (FF02_RD) gb.bus.D7 = XFER_START;

  if (FF07_RD) gb.bus.D2 = TAC_0;
  if (FF07_RD) gb.bus.D1 = TAC_1;
  if (FF07_RD) gb.bus.D0 = TAC_2;

  if (FF50_RD) gb.bus.D0 = BOOT_BIT;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



void System::tock(Gameboy& gb, bool APU_RESET1) {

  /*p07.UPOJ*/ wire MODE_PROD = _nand(!gb.chip_in.T1, !gb.chip_in.T2, gb.chip_in.RST);
  /*p01.AFER*/ RESET_REG.tock(!DIV_CLK, MODE_PROD, RESET_IN);

  //----------
  // Clock phase generator

  {
    bool oldCLK_xxxDEFGx = CLK_xxxDEFGx;

    // These registers tick on _BOTH_EDGES_ of the master clock.
    /*p01.ADYK*/ CLK_xxxDEFGx.tock(CLK_AxCxExGx, MODE_PROD, CLK_xxCDEFxx);
    /*p01.APUK*/ CLK_xxCDEFxx.tock(CLK_AxCxExGx, MODE_PROD, CLK_xBCDExxx);
    /*p01.ALEF*/ CLK_xBCDExxx.tock(CLK_AxCxExGx, MODE_PROD, CLK_ABCDxxxx);
    /*p01.AFUR*/ CLK_ABCDxxxx.tock(CLK_AxCxExGx, MODE_PROD, !oldCLK_xxxDEFGx);
  }

  //----------
  // Clock dividers

  /*p01.BYLU*/ CLK_128.tock  (!CLK_256,     !APU_RESET1, !CLK_128);
  /*p01.CARU*/ CLK_256.tock  (!CLK_512,     !APU_RESET1, !CLK_256);
  /*p01.BARA*/ CLK_512.tock  (!AJER_2M,     !APU_RESET1, !DIV_10);

  /*p01.JESO*/ CLK_512K.tock (!AVOK,        !APU_RESET1, !CLK_512K);
  /*p01.AVOK*/ AVOK.tock     (ATYK,         !APU_RESET1, !AVOK);
  /*p01.ATYK*/ ATYK.tock     (CLK_AxCxExGx, !APU_RESET1, !ATYK);
  /*p09.AJER*/ AJER_2M.tock  (CLK_AxCxExGx, !APU_RESET1, !AJER_2M);

  //----------
  // Serial port

  {
    /*p06.DAKU*/ wire FF01_WR = CPU_WR && gb.bus.A == 0xFF01;
    /*p06.UWAM*/ wire FF02_WR = CPU_WR && gb.bus.A == 0xFF02;

    /*p06.CAVE*/ wire SER_CLK_MUXn = mux2n(SER_CLK, gb.chip_in.SCK_C, XFER_DIR);
    /*p06.DAWA*/ wire SER_CLK1 = _or(SER_CLK_MUXn, !XFER_START); // this must stop the clock or something when the transmit's done

    /*p06.CARO*/ wire SER_RST = FF02_WR || SYS_RESET;
    /*p06.CABY*/ wire XFER_RESET = SER_CNT3 || SYS_RESET;

    /*p06.ETAF*/ XFER_START.tock(!FF02_WR,  !XFER_RESET, gb.bus.D7);
    /*p06.CULY*/ XFER_DIR.tock  (!FF02_WR,  !SYS_RESET,  gb.bus.D0);
    /*p06.COTY*/ SER_CLK.tock   (!DIV_05,   !FF02_WR,    !SER_CLK);
    /*p06.ELYS*/ SER_OUT.tock   (!SER_CLK1, !SYS_RESET,  SER_DATA7);
    /*p06.CALY*/ SER_CNT3.tock  (!SER_CNT2, !SER_RST,    !SER_CNT3);
    /*p06.CYDE*/ SER_CNT2.tock  (!SER_CNT1, !SER_RST,    !SER_CNT2);
    /*p06.CYLO*/ SER_CNT1.tock  (!SER_CNT0, !SER_RST,    !SER_CNT1);
    /*p06.CAFA*/ SER_CNT0.tock  ( SER_CLK1, !SER_RST,    !SER_CNT0);
  
    /*p06.CUFU*/ wire SER_DATA0_SETn = _nand(gb.bus.D0, FF01_WR);
    /*p06.DOCU*/ wire SER_DATA1_SETn = _nand(gb.bus.D1, FF01_WR);
    /*p06.DELA*/ wire SER_DATA2_SETn = _nand(gb.bus.D2, FF01_WR);
    /*p06.DYGE*/ wire SER_DATA3_SETn = _nand(gb.bus.D3, FF01_WR);
    /*p06.DOLA*/ wire SER_DATA4_SETn = _nand(gb.bus.D4, FF01_WR);
    /*p06.ELOK*/ wire SER_DATA5_SETn = _nand(gb.bus.D5, FF01_WR);
    /*p06.EDEL*/ wire SER_DATA6_SETn = _nand(gb.bus.D6, FF01_WR);
    /*p06.EFEF*/ wire SER_DATA7_SETn = _nand(gb.bus.D7, FF01_WR);

    /*p06.COHY*/ wire SER_DATA0_RSTn = _or(_and(!FF01_WR, gb.bus.D0), !SYS_RESET);
    /*p06.DUMO*/ wire SER_DATA1_RSTn = _or(_and(!FF01_WR, gb.bus.D1), !SYS_RESET);
    /*p06.DYBO*/ wire SER_DATA2_RSTn = _or(_and(!FF01_WR, gb.bus.D2), !SYS_RESET);
    /*p06.DAJU*/ wire SER_DATA3_RSTn = _or(_and(!FF01_WR, gb.bus.D3), !SYS_RESET);
    /*p06.DYLY*/ wire SER_DATA4_RSTn = _or(_and(!FF01_WR, gb.bus.D4), !SYS_RESET);
    /*p06.EHUJ*/ wire SER_DATA5_RSTn = _or(_and(!FF01_WR, gb.bus.D5), !SYS_RESET);
    /*p06.EFAK*/ wire SER_DATA6_RSTn = _or(_and(!FF01_WR, gb.bus.D6), !SYS_RESET);
    /*p06.EGUV*/ wire SER_DATA7_RSTn = _or(_and(!FF01_WR, gb.bus.D7), !SYS_RESET);

    /*p06.EDER*/ SER_DATA7.tock(SER_CLK1, SER_DATA7_SETn, SER_DATA7_RSTn, SER_DATA6);
    /*p06.EROD*/ SER_DATA6.tock(SER_CLK1, SER_DATA6_SETn, SER_DATA6_RSTn, SER_DATA5);
    /*p06.EJAB*/ SER_DATA5.tock(SER_CLK1, SER_DATA5_SETn, SER_DATA5_RSTn, SER_DATA4);
    /*p06.DOVU*/ SER_DATA4.tock(SER_CLK1, SER_DATA4_SETn, SER_DATA4_RSTn, SER_DATA3);
    /*p06.DOJO*/ SER_DATA3.tock(SER_CLK1, SER_DATA3_SETn, SER_DATA3_RSTn, SER_DATA2);
    /*p06.DYRA*/ SER_DATA2.tock(SER_CLK1, SER_DATA2_SETn, SER_DATA2_RSTn, SER_DATA1);
    /*p06.DEGU*/ SER_DATA1.tock(SER_CLK1, SER_DATA1_SETn, SER_DATA1_RSTn, SER_DATA0);
    /*p06.CUBA*/ SER_DATA0.tock(SER_CLK1, SER_DATA0_SETn, SER_DATA0_RSTn, !gb.chip_in.SIN_C);
  }


  //----------
  // Boot bit

  {
    /*p07.TUGE*/ wire BOOT_BIT_WR = CPU_WR && gb.bus.A == 0xFF50;
    /*p07.TEPU*/ BOOT_BIT.tock(!BOOT_BIT_WR, !SYS_RESET, gb.bus.D0 || BOOT_BIT);
  }

  //----------
  // Timer regs

  {
    wire FF05_WR = CPU_WR && gb.bus.A == 0xFF05;
    wire FF06_WR = CPU_WR && gb.bus.A == 0xFF06;
    wire FF07_WR = CPU_WR && gb.bus.A == 0xFF07;

    wire TIMA_CLK = (TAC_1 ? (TAC_0 ? DIV_05 : DIV_03) : (TAC_0 ? DIV_01 : DIV_07)) && TAC_2;

    /*p03.SOPU*/ TAC_0.tock(!FF07_WR, !SYS_RESET, gb.bus.D0);
    /*p03.SAMY*/ TAC_1.tock(!FF07_WR, !SYS_RESET, gb.bus.D1);
    /*p03.SABO*/ TAC_2.tock(!FF07_WR, !SYS_RESET, gb.bus.D2);

    // FROM_CPU5 makes no sense here
    /*p03.MEXU*/ wire TIMA_LOAD = _or(_and(!gb.cpu_in.FROM_CPU5, FF05_WR), SYS_RESET, INT_TIMER); // from_cpu5 polarity?
    /*p03.MERY*/ wire INT_TIMER_IN = _nor(!TIMA_MAX, TIMA_7);

    /*p03.NYDU*/ TIMA_MAX.tock (DIV_CLK, !TIMA_LOAD, TIMA_7);
    /*p03.MOBA*/ INT_TIMER.tock(DIV_CLK, !SYS_RESET, INT_TIMER_IN);

    /*p03.PUXY*/ wire TIMA_LD_0 = _nor(SYS_RESET, mux2n(TMA_0, gb.bus.D0, !FF05_WR));
    /*p03.NERO*/ wire TIMA_LD_1 = _nor(SYS_RESET, mux2n(TMA_1, gb.bus.D1, !FF05_WR));
    /*p03.NADA*/ wire TIMA_LD_2 = _nor(SYS_RESET, mux2n(TMA_2, gb.bus.D2, !FF05_WR));
    /*p03.REPA*/ wire TIMA_LD_3 = _nor(SYS_RESET, mux2n(TMA_3, gb.bus.D3, !FF05_WR));
    /*p03.ROLU*/ wire TIMA_LD_4 = _nor(SYS_RESET, mux2n(TMA_4, gb.bus.D4, !FF05_WR));
    /*p03.RUGY*/ wire TIMA_LD_5 = _nor(SYS_RESET, mux2n(TMA_5, gb.bus.D5, !FF05_WR));
    /*p03.PYMA*/ wire TIMA_LD_6 = _nor(SYS_RESET, mux2n(TMA_6, gb.bus.D6, !FF05_WR));
    /*p03.PAGU*/ wire TIMA_LD_7 = _nor(SYS_RESET, mux2n(TMA_7, gb.bus.D7, !FF05_WR));

    /*p03.NUGA*/ TIMA_7.tock(TIMA_6,   TIMA_LOAD, TIMA_LD_7);
    /*p03.PEDA*/ TIMA_6.tock(TIMA_5,   TIMA_LOAD, TIMA_LD_6);
    /*p03.RAGE*/ TIMA_5.tock(TIMA_4,   TIMA_LOAD, TIMA_LD_5);
    /*p03.RUBY*/ TIMA_4.tock(TIMA_3,   TIMA_LOAD, TIMA_LD_4);
    /*p03.RATE*/ TIMA_3.tock(TIMA_2,   TIMA_LOAD, TIMA_LD_3);
    /*p03.PERU*/ TIMA_2.tock(TIMA_1,   TIMA_LOAD, TIMA_LD_2);
    /*p03.POVY*/ TIMA_1.tock(TIMA_0,   TIMA_LOAD, TIMA_LD_1);
    /*p03.REGA*/ TIMA_0.tock(TIMA_CLK, TIMA_LOAD, TIMA_LD_0);

    /*p03.SABU*/ TMA_0.tock(!FF06_WR, !SYS_RESET, gb.bus.D0);
    /*p03.NYKE*/ TMA_1.tock(!FF06_WR, !SYS_RESET, gb.bus.D1);
    /*p03.MURU*/ TMA_2.tock(!FF06_WR, !SYS_RESET, gb.bus.D2);
    /*p03.TYVA*/ TMA_3.tock(!FF06_WR, !SYS_RESET, gb.bus.D3);
    /*p03.TYRU*/ TMA_4.tock(!FF06_WR, !SYS_RESET, gb.bus.D4);
    /*p03.SUFY*/ TMA_5.tock(!FF06_WR, !SYS_RESET, gb.bus.D5);
    /*p03.PETO*/ TMA_6.tock(!FF06_WR, !SYS_RESET, gb.bus.D6);
    /*p03.SETA*/ TMA_7.tock(!FF06_WR, !SYS_RESET, gb.bus.D7);
  }

  //----------
  // Joypad regs

  {
    wire FF00_WR = CPU_WR && gb.bus.A == 0xFF00;

    /*p02.KERY*/ wire ANY_BUTTON = _or(gb.chip_in.P13_C, gb.chip_in.P12_C, gb.chip_in.P11_C, gb.chip_in.P10_C);

    /*p02.APUG*/ JP_GLITCH3.tock(DIV_CLK, !SYS_RESET, JP_GLITCH2);
    /*p02.AGEM*/ JP_GLITCH2.tock(DIV_CLK, !SYS_RESET, JP_GLITCH1);
    /*p02.ACEF*/ JP_GLITCH1.tock(DIV_CLK, !SYS_RESET, JP_GLITCH0);
    /*p02.BATU*/ JP_GLITCH0.tock(DIV_CLK, !SYS_RESET, ANY_BUTTON);

    /*p05.JUTE*/ JOYP_RA    .tock(!FF00_WR, !SYS_RESET, gb.bus.D0);
    /*p05.KECY*/ JOYP_LB    .tock(!FF00_WR, !SYS_RESET, gb.bus.D1);
    /*p05.JALE*/ JOYP_UC    .tock(!FF00_WR, !SYS_RESET, gb.bus.D2);
    /*p05.KYME*/ JOYP_DS    .tock(!FF00_WR, !SYS_RESET, gb.bus.D3);
    /*p05.KELY*/ KELY       .tock(!FF00_WR, !SYS_RESET, gb.bus.D4);
    /*p05.COFY*/ COFY       .tock(!FF00_WR, !SYS_RESET, gb.bus.D5);
    /*p05.KUKO*/ DBG_FF00_D6.tock(!FF00_WR, !SYS_RESET, gb.bus.D6);
    /*p05.KERU*/ DBG_FF00_D7.tock(!FF00_WR, !SYS_RESET, gb.bus.D7);
  }

  //----------
  // DMA regs

  {
    wire FF46_WR = CPU_WR && gb.bus.A == 0xFF46;

    // probably wrong, schematic looks incorrect
    /*p04.LOKO*/ wire DMA_RST = _or(SYS_RESET, DMA_RSTn2);

    /*p04.LOKY*/ wire DMA_RUNNING = !DMA_DONE_SYNC && !SYS_RESET && !DMA_RSTn2;

    /*p04.LENE*/ DMA_RSTn2.tock(CPUCLK_ABCDxxxx, !SYS_RESET, DMA_RSTn1);
    /*p04.LUVY*/ DMA_RSTn1.tock(CPUCLK_xxxxEFGH, !SYS_RESET, _and(!FF46_WR, !DMA_RST));

    /*p04.MATU*/ DMA_RUNNING_SYNC.tock(CPUCLK_xxxxEFGH, !SYS_RESET, DMA_RUNNING);

    /*p04.NAVO*/ wire DMA_DONE = _and(DMA_A00, DMA_A01, DMA_A02, DMA_A03, DMA_A04, DMA_A07); // 128+16+8+4+2+1 = 159, this must be "dma done"
    /*p04.MYTE*/ DMA_DONE_SYNC.tock   (CPUCLK_ABCDxxxx, !DMA_RST,   DMA_DONE);

    /*p04.META*/ wire DMA_CLK = _and(CPUCLK_xxxxEFGH, DMA_RUNNING);

    /*p04.MUGU*/ DMA_A07.tock(!DMA_A06, !DMA_RST, !DMA_A07);
    /*p04.NUTO*/ DMA_A06.tock(!DMA_A05, !DMA_RST, !DMA_A06);
    /*p04.PYLO*/ DMA_A05.tock(!DMA_A04, !DMA_RST, !DMA_A05);
    /*p04.NYKO*/ DMA_A04.tock(!DMA_A03, !DMA_RST, !DMA_A04);
    /*p04.MUTY*/ DMA_A03.tock(!DMA_A02, !DMA_RST, !DMA_A03);
    /*p04.NEFY*/ DMA_A02.tock(!DMA_A01, !DMA_RST, !DMA_A02);
    /*p04.PYRO*/ DMA_A01.tock(!DMA_A00, !DMA_RST, !DMA_A01);
    /*p04.NAKY*/ DMA_A00.tock( DMA_CLK, !DMA_RST, !DMA_A00);

    /*p04.NAFA*/ DMA_A08.tock(!FF46_WR, 0, gb.bus.D0);
    /*p04.PYNE*/ DMA_A09.tock(!FF46_WR, 0, gb.bus.D1);
    /*p04.PARA*/ DMA_A10.tock(!FF46_WR, 0, gb.bus.D2);
    /*p04.NYDO*/ DMA_A11.tock(!FF46_WR, 0, gb.bus.D3);
    /*p04.NYGY*/ DMA_A12.tock(!FF46_WR, 0, gb.bus.D4);
    /*p04.PULA*/ DMA_A13.tock(!FF46_WR, 0, gb.bus.D5);
    /*p04.POKU*/ DMA_A14.tock(!FF46_WR, 0, gb.bus.D6);
    /*p04.MARU*/ DMA_A15.tock(!FF46_WR, 0, gb.bus.D7);
  }

  //----------
  // FF04 DIV

  {
    wire DIV_RST = gb.chip_in.RST || (CPU_WR && gb.bus.A == 0xFF04);

    /*p01.UPOF*/ DIV_15.tock(!DIV_14, !DIV_RST, !DIV_15);
    /*p01.UKET*/ DIV_14.tock(!DIV_13, !DIV_RST, !DIV_14);
    /*p01.TEKA*/ DIV_13.tock(!DIV_12, !DIV_RST, !DIV_13);
    /*p01.SUBU*/ DIV_12.tock(!DIV_11, !DIV_RST, !DIV_12);
    /*p01.SOLA*/ DIV_11.tock(!DIV_10, !DIV_RST, !DIV_11);
    /*p01.TERU*/ DIV_10.tock(!DIV_09, !DIV_RST, !DIV_10);
    /*p01.TOFE*/ DIV_09.tock(!DIV_08, !DIV_RST, !DIV_09);
    /*p01.TUGO*/ DIV_08.tock(!DIV_07, !DIV_RST, !DIV_08);
    /*p01.TULU*/ DIV_07.tock(!DIV_06, !DIV_RST, !DIV_07);
    /*p01.UGOT*/ DIV_06.tock(!DIV_05, !DIV_RST, !DIV_06);
    /*p01.TAMA*/ DIV_05.tock(!DIV_04, !DIV_RST, !DIV_05);
    /*p01.UNYK*/ DIV_04.tock(!DIV_03, !DIV_RST, !DIV_04);
    /*p01.TERO*/ DIV_03.tock(!DIV_02, !DIV_RST, !DIV_03);
    /*p01.UNER*/ DIV_02.tock(!DIV_01, !DIV_RST, !DIV_02);
    /*p01.UFOR*/ DIV_01.tock(!DIV_00, !DIV_RST, !DIV_01);
    /*p01.UKUP*/ DIV_00.tock(DIV_CLK, !DIV_RST, !DIV_00);
  }
}

//-----------------------------------------------------------------------------
