/// plait_noparse

#include "GateBoyLib/Sch_Channel3.h"

//-----------------------------------------------------------------------------

#if 0

void P16_Ch3Regs_tick(const ClkSignals& clk_reg,
                      const Gameboy& a,
                      const Gameboy& b,
                      Gameboy& next) {

  //----------
  // FF1A NR30 - Channel 3 Sound on/off (R/W)

  {
    /*_p10.EXAT*/ wire ADDR_1010an = nand2(b.apu.ADDR_1xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*_p10.EMOR*/ wire ADDR_FF1A  = nor4(b.apu.ADDR_FF1Xn, ADDR_1010an);

    /*_p16.GOVE*/ wire APU_RESETo = not1(b.apu.APU_RESET1);
    /*_p16.GEJO*/ wire FF1A_WR = and2(b.apu.APU_WR, ADDR_FF1A);
    /*_p16.GUCY*/ next.ch3.FF1A_WRn = not1(FF1A_WR);
    /*_p16.GUXE*/ next.ch3.CH3_AMP_ENna = tock_pos(a.ch3.FF1A_WRn, b.ch3.FF1A_WRn, APU_RESETo, b.ch3.CH3_AMP_ENna, b.bus.TS_D0());

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p09.GAXO*/ wire CPU_RDd = not1(CPU_RDn);
    /*_p16.FASY*/ wire FF1A_RD = nand2(ADDR_FF1A, CPU_RDd);
    /*_p16.FEVO*/ wire CH3_AMP_ENa  = not1(!b.ch3.CH3_AMP_ENna);

    if (FF1A_RD) next.bus.set_data(
      /*_p16.GEKO*/ CH3_AMP_ENa
    );
  }

  //----------
  // FF1B - NR31 - Channel 3 Sound Length

  {
    /*_p18.GEPY*/ wire GEPY = nor4(next.ch3.LEN_DONE, b.apu.CLK_256a, !b.ch3.NR34_STOP);
    /*_p18.GENU*/ next.ch3.GENU_CLK = not1(GEPY);
    /*_p18.FALU*/ next.ch3.FALU_CLK = not1(b.ch3.CH3_LEN3);

    /*_p10.EMAX*/ wire ADDR_1011an = nand2(b.apu.ADDR_1xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx1);
    /*_p10.DUSA*/ wire ADDR_FF1B  = nor4(b.apu.ADDR_FF1Xn, ADDR_1011an);

    /*_p16.DERY*/ wire FF1B_WR = nand2(b.apu.APU_WR, ADDR_FF1B);
    /*_p16.EMUT*/ wire EMUT = not1(FF1B_WR);
    /*_p16.GAJY*/ wire GAJY = not1(FF1B_WR);

    /*_p18.GEVO*/ next.ch3.CH3_LEN0 = count_pos(a.ch3.GENU_CLK, b.ch3.GENU_CLK, GAJY, b.ch3.CH3_LEN0, b.bus.TS_D0());
    /*_p18.FORY*/ next.ch3.CH3_LEN1 = count_pos(a.ch3.CH3_LEN0, b.ch3.CH3_LEN0, GAJY, b.ch3.CH3_LEN1, b.bus.TS_D1());
    /*_p18.GATU*/ next.ch3.CH3_LEN2 = count_pos(a.ch3.CH3_LEN1, b.ch3.CH3_LEN1, GAJY, b.ch3.CH3_LEN2, b.bus.TS_D2());
    /*_p18.GAPO*/ next.ch3.CH3_LEN3 = count_pos(a.ch3.CH3_LEN2, b.ch3.CH3_LEN2, GAJY, b.ch3.CH3_LEN3, b.bus.TS_D3());
    /*_p18.GEMO*/ next.ch3.CH3_LEN4 = count_pos(a.ch3.FALU_CLK, b.ch3.FALU_CLK, EMUT, b.ch3.CH3_LEN4, b.bus.TS_D4());
    /*_p18.FORO*/ next.ch3.CH3_LEN5 = count_pos(a.ch3.CH3_LEN4, b.ch3.CH3_LEN4, EMUT, b.ch3.CH3_LEN5, b.bus.TS_D5());
    /*_p18.FAVE*/ next.ch3.CH3_LEN6 = count_pos(a.ch3.CH3_LEN5, b.ch3.CH3_LEN5, EMUT, b.ch3.CH3_LEN6, b.bus.TS_D6());
    /*_p18.FYRU*/ next.ch3.CH3_LEN7 = count_pos(a.ch3.CH3_LEN6, b.ch3.CH3_LEN6, EMUT, b.ch3.CH3_LEN7, b.bus.TS_D7());

    /*_p16.GETO*/ wire GETO = not1(FF1B_WR);
    /*_p18.GUDA*/ wire GUDA = nor4(GETO, b.apu.APU_RESET1, b.ch3.GARA);
    /*_p18.FEXU*/ next.ch3.LEN_DONE = tock_pos(!a.ch3.CH3_LEN7, !b.ch3.CH3_LEN7, GUDA, b.ch3.LEN_DONE, !b.ch3.LEN_DONE);
  }

  //----------
  // FF1C NR32 - Channel 3 Select output level (R/W)

  {
    /*_p10.GANY*/ wire ADDR_1100an = nand2(b.apu.ADDR_1xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*_p10.GEFO*/ wire ADDR_FF1C  = nor4(b.apu.ADDR_FF1Xn, ADDR_1100an);

    /*_p16.GURO*/ wire APU_RESETn = not1(b.apu.APU_RESET1);

    /*_p16.HAGA*/ wire FF1C_WR = and2(b.apu.APU_WR, ADDR_FF1C);

    /*_p16.GUZU*/ next.ch3.FF1C_WRn = not1(FF1C_WR);
    /*_p16.HUKY*/ next.ch3.NR32_VOL0 = tock_pos(a.ch3.FF1C_WRn, b.ch3.FF1C_WRn, APU_RESETn, b.ch3.NR32_VOL0, b.bus.TS_D0());
    /*_p16.HODY*/ next.ch3.NR32_VOL1 = tock_pos(a.ch3.FF1C_WRn, b.ch3.FF1C_WRn, APU_RESETn, b.ch3.NR32_VOL1, b.bus.TS_D1());

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p16.JOTU*/ wire CPU_RDb = not1(CPU_RDn);
    /*_p16.HENU*/ wire FF1C_RD = nand2(ADDR_FF1C, CPU_RDb);

    /*_p18.JECO*/ wire DBG_CPU_RDb = not1(CPU_RDn);
    /*_p18.HONY*/ wire DBG_FF1C_RD = and2(b.apu.NR52_DBG_APU, DBG_CPU_RDb, ADDR_FF1C);
    /*_p18.GENO*/ wire DBG_FF1C_RDn = not1(DBG_FF1C_RD);

#if 0
    // blah
    /*_p18.FAPY*/ if (DBG_FF1C_RDn) next.\1() = b.ch3.WAVE_IDX0;
    /*_p18.FARO*/ if (DBG_FF1C_RDn) next.\1() = b.ch3.WAVE_IDX1;
    /*_p18.FOTE*/ if (DBG_FF1C_RDn) next.\1() = b.ch3.WAVE_IDX2;
    /*_p18.FANA*/ if (DBG_FF1C_RDn) next.\1() = b.ch3.WAVE_IDX3;
    /*_p18.FERA*/ if (DBG_FF1C_RDn) next.\1() = b.ch3.WAVE_IDX4;
    /*_p16.HAMU*/ if (FF1C_RD)      next.\1() = b.ch3.NR32_VOL0;
    /*_p16.HUCO*/ if (FF1C_RD)      next.\1() = b.ch3.NR32_VOL1;
#endif
    if (FF1C_RD) next.bus.set_data(
      /*_p18.FAPY*/ 0,
      /*_p18.FARO*/ 0,
      /*_p18.FOTE*/ 0,
      /*_p18.FANA*/ 0,
      /*_p18.FERA*/ 0,
      /*_p16.HAMU*/ b.ch3.NR32_VOL0,
      /*_p16.HUCO*/ b.ch3.NR32_VOL1
    );
  }

  //----------
  // FF1D - NR33 - Channel 3 Frequency's lower data (W)

  {
    /*_p10.EMOS*/ wire ADDR_1101an = nand2(b.apu.ADDR_1xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1);
    /*_p10.FENY*/ wire ADDR_FF1D  = nor4(b.apu.ADDR_FF1Xn, ADDR_1101an);
    /*_p16.HOXA*/ wire ADDR_FF1Dn = not1(ADDR_FF1D);

    /*_p16.KOTA*/ wire FF1D_WRn = nand2(ADDR_FF1D, b.apu.APU_WR);
    /*_p16.JAFA*/ wire FF1D_WRo = nand2(ADDR_FF1D, b.apu.APU_WR);

    /*_p16.KYHO*/ next.ch3.FF1D_WRa = not1(FF1D_WRn);
    /*_p16.KULY*/ next.ch3.FF1D_WRb = not1(FF1D_WRo);
    /*_p16.KUHA*/ next.ch3.APU_RESETq = not1(b.apu.APU_RESET1);

    /*_p16.KOGA*/ next.ch3.NR33_FREQ0 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ0, b.bus.TS_D0());
    /*_p16.JOVY*/ next.ch3.NR33_FREQ1 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ1, b.bus.TS_D1());
    /*_p16.JAXA*/ next.ch3.NR33_FREQ2 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ2, b.bus.TS_D2());
    /*_p16.JEFE*/ next.ch3.NR33_FREQ3 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ3, b.bus.TS_D3());
    /*_p16.JYPO*/ next.ch3.NR33_FREQ4 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ4, b.bus.TS_D4());
    /*_p16.JOVE*/ next.ch3.NR33_FREQ5 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ5, b.bus.TS_D5());
    /*_p16.KANA*/ next.ch3.NR33_FREQ6 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ6, b.bus.TS_D6());
    /*_p16.KOGU*/ next.ch3.NR33_FREQ7 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ7, b.bus.TS_D7());

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p16.DOVO*/ wire ASOT_CPURD = not1(CPU_RDn);
    /*_p16.EGAD*/ wire DBG_CPU_RD = nand2(b.apu.NR52_DBG_APU, ASOT_CPURD);
    /*_p16.GUTE*/ wire DBG_FF1D_RDn = nor4(ADDR_FF1Dn, DBG_CPU_RD);
    /*_p16.HOVO*/ wire DBG_FF1D_RD = not1(DBG_FF1D_RDn);

    if (DBG_FF1D_RD) next.bus.set_data(
      /*_p16.JOFO*/ b.ch3.FREQ_00,
      /*_p16.KAFU*/ b.ch3.FREQ_01,
      /*_p16.KESY*/ b.ch3.FREQ_02,
      /*_p16.JUDE*/ b.ch3.FREQ_03,
      /*_p16.JUKE*/ b.ch3.FREQ_04,
      /*_p16.JEZA*/ b.ch3.FREQ_05,
      /*_p16.KORA*/ b.ch3.FREQ_06,
      /*_p16.KAMY*/ b.ch3.FREQ_07
    );
  }

  //----------
  // FF1E - NR34 - Channel 3 Frequency's higher data (R/W)

  {
    /*_p10.EGEN*/ wire ADDR_1110an = nand2(b.apu.ADDR_1xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*_p10.DUGO*/ wire ADDR_FF1E  = nor4(b.apu.ADDR_FF1Xn, ADDR_1110an);

    /*_p16.HUDA*/ wire FF1E_WRn = nand2(ADDR_FF1E, b.apu.APU_WR); // polarity?

    /*_p16.KOPY*/ wire APU_RESETs  = not1(b.apu.APU_RESET1);
    /*_p16.HEKY*/ wire APU_RESETr  = not1(b.apu.APU_RESET1);
    /*_p16.FAKO*/ wire RESTART_RST = nor4(b.apu.APU_RESET1, b.ch3.CH3_RESTART_SYNC);

    /*_p16.JUZO*/ next.ch3.FF1E_WR = not1(FF1E_WRn);
    /*_p16.ANUJ*/ wire CPU_WR_WEIRD = and2(b.cpu.SIG_CPU_HOLD_MEM, b.apu.APU_WR);
    /*_p16.FOVO*/ next.ch3.FF1E_WRo = nand2(CPU_WR_WEIRD, ADDR_FF1E);
    /*_p16.EPYX*/ next.ch3.FF1E_WRp = nor4(b.apu.APU_WR, ADDR_FF1E); // polarity?

    /*_p16.JEMO*/ next.ch3.NR34_FREQ8  = tock_pos(a.ch3.FF1E_WR,  b.ch3.FF1E_WR,  APU_RESETs,  b.ch3.NR34_FREQ8,  b.bus.TS_D0());
    /*_p16.JETY*/ next.ch3.NR34_FREQ9  = tock_pos(a.ch3.FF1E_WR,  b.ch3.FF1E_WR,  APU_RESETs,  b.ch3.NR34_FREQ9,  b.bus.TS_D1());
    /*_p16.JACY*/ next.ch3.NR34_FREQ10 = tock_pos(a.ch3.FF1E_WR,  b.ch3.FF1E_WR,  APU_RESETs,  b.ch3.NR34_FREQ10, b.bus.TS_D2());
    /*_p16.HOTO*/ next.ch3.NR34_STOP   = tock_pos(a.ch3.FF1E_WRo, b.ch3.FF1E_WRo, APU_RESETr,  b.ch3.NR34_STOP,   b.bus.TS_D3());
    /*_p16.GAVU*/ next.ch3.NR34_START  = tock_pos(a.ch3.FF1E_WRp, b.ch3.FF1E_WRp, RESTART_RST, b.ch3.NR34_START,  b.bus.TS_D4());

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p16.DOVO*/ wire ASOT_CPURD = not1(CPU_RDn);
    /*_p16.EGAD*/ wire DBG_CPU_RD = nand2(b.apu.NR52_DBG_APU, ASOT_CPURD);

    /*_p16.GUNU*/ wire GUNU = not1(ADDR_FF1E);
    /*_p16.FUVA*/ wire FF14_RDb = or2(GUNU, DBG_CPU_RD);

    if (FF14_RDb) next.bus.set_data(
      /*_p16.JUVY*/ b.ch3.FREQ_08,
      /*_p16.JURA*/ b.ch3.FREQ_09,
      /*_p16.HUFO*/ b.ch3.FREQ_10,
      /*_p16.HACA*/ b.ch3.NR34_STOP
    );

#if 0
    // blah
    /*_p16.JUVY*/ if (FF14_RDb) next.\1() = b.ch3.FREQ_08;
    /*_p16.JURA*/ if (FF14_RDb) next.\1() = b.ch3.FREQ_09;
    /*_p16.HUFO*/ if (FF14_RDb) next.\1() = b.ch3.FREQ_10;

    /*_p16.GORY*/ wire GORY = not1(CPU_RDn); // polarity?
    /*_p16.GAWA*/ wire FF14_RDa = nand2(ADDR_FF1E, GORY); // polarity?

    if (FF14_RDa) next.bus.set_data(
      /*_p16.HACA*/ b.ch3.NR34_STOP
    );
#endif
  }

  //----------
  // Frequency counter

  {
    /*_p16.FURY*/ wire APU_RESETn = nor4(b.ch3.GYTA, b.apu.APU_RESET1);
    /*_p16.GULO*/ wire GULO = not1(APU_RESETn);

    // weird latch?
    /*_p16.GOFY*/ wire GOFY = or2(GULO, b.ch3.CH3_RESTART_SYNC);

    /*_p16.GAZE*/ wire APU_RESETo = not1(b.apu.APU_RESET1);

    /*_p16.FABO*/ next.ch3.CLK_xxCDxxGH = not1(b.apu.CERY);
    /*_p16.GARA*/ next.ch3.GARA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  APU_RESETn, b.ch3.GARA, GOFY);
    /*_p16.GYTA*/ next.ch3.GYTA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  APU_RESETo, b.ch3.GYTA, b.ch3.GARA);
    /*_p16.GYRA*/ next.ch3.GYRA = tock_pos(a.apu.CERY, b.apu.CERY, APU_RESETo, b.ch3.GYRA, b.ch3.GYTA);
  }

  {
    /*_p16.FAJU*/ wire FAJU = not1(b.ch3.GYRA);
    /*_p16.FUVO*/ wire FUVO = nor4(b.ch3.CH3_AMP_ENna, b.apu.APU_RESET1);

    // weird latch?
    /*_p16.GUGU*/ wire FREQ_CLK_STOP = or2(FUVO, FAJU);
    /*_p18.HEFO*/ wire FREQ_CLKn = nor4(b.apu.CERY, FREQ_CLK_STOP);

    /*_p18.HEMA*/ wire WAVE_CLKb = not1(b.ch3.WAVE_CLKo);
    /*_p18.GASE*/ wire WAVE_CLKn = not1(WAVE_CLKb);
    /*_p18.HERA*/ wire FREQ_RST  = nor4(WAVE_CLKn, b.ch3.GARA);
    /*_p18.KYKO*/ wire FREQ_RSTn = not1(FREQ_RST);
    /*_p18.JERA*/ wire FREQ_RSTo = not1(FREQ_RST);
    /*_p18.KASO*/ wire FREQ_RSTp = not1(FREQ_RST);

    /*_p18.JUTY*/ next.ch3.FREQ_CLKa = not1(FREQ_CLKn);
    /*_p18.KYRU*/ next.ch3.FREQ_CLKb = not1(!b.ch3.FREQ_03);
    /*_p18.KESE*/ next.ch3.FREQ_CLKc = not1(!b.ch3.FREQ_07);

    /*_p18.KUTU*/ next.ch3.FREQ_00 = count_pos(a.ch3.FREQ_CLKa, b.ch3.FREQ_CLKa, FREQ_RSTn, b.ch3.FREQ_00, b.ch3.NR33_FREQ0);
    /*_p18.KUPE*/ next.ch3.FREQ_01 = count_pos(a.ch3.FREQ_00,   b.ch3.FREQ_00,   FREQ_RSTn, b.ch3.FREQ_01, b.ch3.NR33_FREQ1);
    /*_p18.KUNU*/ next.ch3.FREQ_02 = count_pos(a.ch3.FREQ_01,   b.ch3.FREQ_01,   FREQ_RSTn, b.ch3.FREQ_02, b.ch3.NR33_FREQ2);
    /*_p18.KEMU*/ next.ch3.FREQ_03 = count_pos(a.ch3.FREQ_02,   b.ch3.FREQ_02,   FREQ_RSTn, b.ch3.FREQ_03, b.ch3.NR33_FREQ3);
    /*_p18.KYGU*/ next.ch3.FREQ_04 = count_pos(a.ch3.FREQ_CLKb, b.ch3.FREQ_CLKb, FREQ_RSTo, b.ch3.FREQ_04, b.ch3.NR33_FREQ4);
    /*_p18.KEPA*/ next.ch3.FREQ_05 = count_pos(a.ch3.FREQ_04,   b.ch3.FREQ_04,   FREQ_RSTo, b.ch3.FREQ_05, b.ch3.NR33_FREQ5);
    /*_p18.KAFO*/ next.ch3.FREQ_06 = count_pos(a.ch3.FREQ_05,   b.ch3.FREQ_05,   FREQ_RSTo, b.ch3.FREQ_06, b.ch3.NR33_FREQ6);
    /*_p18.KENO*/ next.ch3.FREQ_07 = count_pos(a.ch3.FREQ_06,   b.ch3.FREQ_06,   FREQ_RSTo, b.ch3.FREQ_07, b.ch3.NR33_FREQ7);
    /*_p18.KEJU*/ next.ch3.FREQ_08 = count_pos(a.ch3.FREQ_CLKc, b.ch3.FREQ_CLKc, FREQ_RSTp, b.ch3.FREQ_08, b.ch3.NR34_FREQ8);
    /*_p18.KEZA*/ next.ch3.FREQ_09 = count_pos(a.ch3.FREQ_08,   b.ch3.FREQ_08,   FREQ_RSTp, b.ch3.FREQ_09, b.ch3.NR34_FREQ9);
    /*_p18.JAPU*/ next.ch3.FREQ_10 = count_pos(a.ch3.FREQ_09,   b.ch3.FREQ_09,   FREQ_RSTp, b.ch3.FREQ_10, b.ch3.NR34_FREQ10);
  }

  {
    /*_p18.HUPA*/ wire HUPA = and2(b.ch3.WAVE_CLKo, b.apu.CERY);
    /*_p18.GAFU*/ wire GAFU = nor4(b.apu.APU_RESET1, b.ch3.GARA, HUPA);

    /*_p18.JYFO*/ next.ch3.JYFO_CLK  = not1(b.ch3.FREQ_10);
    /*_p18.HUNO*/ next.ch3.WAVE_CLKo = tock_pos(a.ch3.JYFO_CLK, b.ch3.JYFO_CLK, GAFU, b.ch3.WAVE_CLKo, !b.ch3.WAVE_CLKo);

    /*_p18.HEMA*/ wire WAVE_CLKb = not1(b.ch3.WAVE_CLKo);
    /*_p18.GASE*/ wire WAVE_CLKn = not1(WAVE_CLKb);

    /*_p18.FOTO*/ wire FOTO = and2(b.ch3.WAVE_LOOP, WAVE_CLKn);
    /*_p18.ETAN*/ wire WAVE_RST = or2(b.ch3.GARA, b.ch3.WAVE_LOOP);
    /*_p18.GYRY*/ wire LOOP_RST = nor4(b.apu.APU_RESET1, b.ch3.GARA, FOTO);

    /*_p18.DERO*/ next.ch3.WAVE_CLK  = not1(WAVE_CLKn);
    /*_p18.EFAR*/ next.ch3.WAVE_IDX0 = tock_pos( a.ch3.WAVE_CLK,   b.ch3.WAVE_CLK,  WAVE_RST, b.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0);
    /*_p18.ERUS*/ next.ch3.WAVE_IDX1 = tock_pos(!a.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0, WAVE_RST, b.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1);
    /*_p18.EFUZ*/ next.ch3.WAVE_IDX2 = tock_pos(!a.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1, WAVE_RST, b.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2);
    /*_p18.EXEL*/ next.ch3.WAVE_IDX3 = tock_pos(!a.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2, WAVE_RST, b.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3);
    /*_p18.EFAL*/ next.ch3.WAVE_IDX4 = tock_pos(!a.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3, WAVE_RST, b.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4);
    /*_p18.FETY*/ next.ch3.WAVE_LOOP = tock_pos(!a.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4, LOOP_RST, b.ch3.WAVE_LOOP, !b.ch3.WAVE_LOOP);
  }




  //----------
  // Wave ram control signals

  {
    /*_p16.GOMA*/ wire APU_RESETn = not1(b.apu.APU_RESET1);
    ///*_p16.FOBA*/ next.ch3.CH3_RESTART_SYNC = tock_pos(a.clk_reg.DOVA_ABCDxxxx, b.clk_reg.DOVA_ABCDxxxx, APU_RESETn, b.ch3.CH3_RESTART_SYNC, b.ch3.NR34_START);
  }

  {
    /*_p18.GEDO*/ next.ch3.GEDO = and2(b.apu.CLK_256a, b.ch3.NR34_STOP);
    /*_p18.FYGO*/ next.ch3.FYGO = or2(b.apu.APU_RESET1, b.ch3.GEDO, b.ch3.CH3_AMP_ENna);
    /*_p18.FOZU*/ next.ch3.FOZU = or2(b.ch3.GARA, b.ch3.FYGO);
    /*_p18.EZAS*/ next.ch3.EZAS = not1(b.ch3.FOZU);
    /*_p18.DORU*/ next.ch3.DORU = not1(b.ch3.EZAS);

    /*_p18.CALU*/ wire APU_RESETn = not1(b.apu.APU_RESET1);
    /*_p18.DAVO*/ next.ch3.CH3_ACTIVEo = tock_pos(a.apu.AJER_2M, b.apu.AJER_2M, APU_RESETn, b.ch3.CH3_ACTIVEo, b.ch3.DORU);
  }

  {
    /*_p17.BAMA*/ wire APU_RESETn = not1(b.apu.APU_RESET1);
    /*_p17.COZY*/ next.ch3.COZY_CLK = not1(clk_reg.AMUK_xBxDxFxH);
    /*_p17.ARUC*/ next.ch3.ARUC_CLK = not1(clk_reg.AMUK_xBxDxFxH);

    /*_p18.HEMA*/ wire WAVE_CLKb = not1(b.ch3.WAVE_CLKo);
    /*_p18.GASE*/ wire WAVE_CLKn = not1(WAVE_CLKb);
    ///*_p17.BUSA*/ next.ch3.BUSA        = tock_pos(clk.AMUK_xBxDxFxH, clk.AMUK_xBxDxFxH, APU_RESETn, b.ch3.BUSA,        WAVE_CLKn); // ?
    /*_p17.BANO*/ next.ch3.BANO        = tock_pos(a.ch3.COZY_CLK,      b.ch3.COZY_CLK,      APU_RESETn, b.ch3.BANO,        b.ch3.BUSA);
    ///*_p17.AZUS*/ next.ch3.SAMPLE_CLKn = tock_pos(clk.AMUK_xBxDxFxH, clk.AMUK_xBxDxFxH, APU_RESETn, b.ch3.SAMPLE_CLKn, b.ch3.BANO);
    /*_p17.AZET*/ next.ch3.AZET        = tock_pos(a.ch3.ARUC_CLK,      b.ch3.ARUC_CLK,      APU_RESETn, b.ch3.AZET,        b.ch3.SAMPLE_CLKn);
  }

  {

    /*_p10.ASAD*/ wire A06n = not1(b.bus.BUS_CPU_A06());
    /*_p10.AVUN*/ wire A07n = not1(b.bus.BUS_CPU_A07());
    /*_p10.ACOM*/ wire ADDR_XX3Xn = nand2(A07n, A06n, b.bus.BUS_CPU_A05(), b.bus.BUS_CPU_A04());

    /*_p10.BARO*/ wire ADDR_FF3X  = nor4(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);

    /*_p17.BYZA*/ wire WAVE_WR = and2(b.apu.APU_WR, ADDR_FF3X);

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p17.BOKE*/ wire CPU_RDa = not1(CPU_RDn);
    /*_p17.BENA*/ wire CPU_WAVE_RD  = nand2(CPU_RDa, ADDR_FF3X);
    /*_p17.CAZU*/ wire CPU_WAVE_RDn = not1(CPU_WAVE_RD);

    /*_p17.ABUR*/ wire ABUR_xxCDEFGx = not1(clk_reg.BUKE_ABxxxxxH);
    /*_p17.BORY*/ wire BORY_ABxxxxxH = not1(ABUR_xxCDEFGx);
    /*_p17.BETA*/ wire BETA = or2(WAVE_WR, CPU_WAVE_RDn, BORY_ABxxxxxH);

    /*_p17.BOXO*/ wire BOXO = nor4(b.ch3.SAMPLE_CLKn, b.ch3.AZET);
    /*_p17.AZOR*/ wire AZOR = not1(BETA);

    /*_p17.BORU*/ wire BORU = not1(BOXO);
    /*_p17.BUKU*/ wire BUKU = not1(AZOR);
    /*_p18.COKA*/ wire CH3_ACTIVE = not1(!b.ch3.CH3_ACTIVEo);

    /*_p17.ATUR*/   wire WAVE_RAM_CTRL1n = mux2(BORU, BUKU, CH3_ACTIVE);
    /*_p17.ALER*/ next.ch3.WAVE_RAM_CTRL1 = not1(WAVE_RAM_CTRL1n);
  }

  {
    /*_p10.ASAD*/ wire A06n = not1(b.bus.BUS_CPU_A06());
    /*_p10.AVUN*/ wire A07n = not1(b.bus.BUS_CPU_A07());
    /*_p10.ACOM*/ wire ADDR_XX3Xn = nand2(A07n, A06n, b.bus.BUS_CPU_A05(), b.bus.BUS_CPU_A04());

    /*_p10.BARO*/ wire ADDR_FF3X  = nor4(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);
    /*_p17.BUTU*/ wire SAMPLE_CLK = not1(b.ch3.SAMPLE_CLKn);
    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p17.BOKE*/ wire CPU_RDa = not1(CPU_RDn);
    /*_p17.BENA*/ wire CPU_WAVE_RD  = nand2(CPU_RDa, ADDR_FF3X);
    /*_p17.CAZU*/ wire CPU_WAVE_RDn = not1(CPU_WAVE_RD);

    /*_p18.COKA*/ wire CH3_ACTIVE = not1(!b.ch3.CH3_ACTIVEo);

    /*_p18.BENO*/ wire WAVE_RAM_CTRL3n = mux2(SAMPLE_CLK, CPU_WAVE_RDn, CH3_ACTIVE);
    /*_p18.ATOK*/ next.ch3.WAVE_RAM_CTRL3 = not1(WAVE_RAM_CTRL3n);
  }

  {
    /*_p10.ASAD*/ wire A06n = not1(b.bus.BUS_CPU_A06());
    /*_p10.AVUN*/ wire A07n = not1(b.bus.BUS_CPU_A07());
    /*_p10.ACOM*/ wire ADDR_XX3Xn = nand2(A07n, A06n, b.bus.BUS_CPU_A05(), b.bus.BUS_CPU_A04());

    /*_p10.BARO*/ wire ADDR_FF3X  = nor4(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);
    /*_p17.BYZA*/ wire WAVE_WR = and2(b.apu.APU_WR, ADDR_FF3X);
    /*_p17.AMYT*/ next.ch3.WAVE_WRn = not1(WAVE_WR);
  }











  //----------
  {
    /*_p17.ACOR*/ wire APU_RESETv = not1(b.apu.APU_RESET1);
    /*_p17.BUTU*/ wire SAMPLE_CLK = not1(b.ch3.SAMPLE_CLKn);

    /*_p17.BEKA*/ next.ch3.SAMPLE_CLKa = not1(SAMPLE_CLK);
    /*_p17.COJU*/ next.ch3.SAMPLE_CLKb = not1(SAMPLE_CLK);
    /*_p17.BAJA*/ next.ch3.SAMPLE_CLKc = not1(SAMPLE_CLK);
    /*_p17.BUFE*/ next.ch3.SAMPLE_CLKd = not1(SAMPLE_CLK);

    /*_p17.CYFO*/ next.ch3.SAMPLE_0 = tock_pos(a.ch3.SAMPLE_CLKa, b.ch3.SAMPLE_CLKa, APU_RESETv, b.ch3.SAMPLE_0, b.ch3.WAVE_D0);
    /*_p17.CESY*/ next.ch3.SAMPLE_1 = tock_pos(a.ch3.SAMPLE_CLKd, b.ch3.SAMPLE_CLKd, APU_RESETv, b.ch3.SAMPLE_1, b.ch3.WAVE_D1);
    /*_p17.BUDY*/ next.ch3.SAMPLE_2 = tock_pos(a.ch3.SAMPLE_CLKc, b.ch3.SAMPLE_CLKc, APU_RESETv, b.ch3.SAMPLE_2, b.ch3.WAVE_D2);
    /*_p17.BEGU*/ next.ch3.SAMPLE_3 = tock_pos(a.ch3.SAMPLE_CLKb, b.ch3.SAMPLE_CLKb, APU_RESETv, b.ch3.SAMPLE_3, b.ch3.WAVE_D3);
    /*_p17.CUVO*/ next.ch3.SAMPLE_4 = tock_pos(a.ch3.SAMPLE_CLKa, b.ch3.SAMPLE_CLKa, APU_RESETv, b.ch3.SAMPLE_4, b.ch3.WAVE_D4);
    /*_p17.CEVO*/ next.ch3.SAMPLE_5 = tock_pos(a.ch3.SAMPLE_CLKd, b.ch3.SAMPLE_CLKd, APU_RESETv, b.ch3.SAMPLE_5, b.ch3.WAVE_D5);
    /*_p17.BORA*/ next.ch3.SAMPLE_6 = tock_pos(a.ch3.SAMPLE_CLKc, b.ch3.SAMPLE_CLKc, APU_RESETv, b.ch3.SAMPLE_6, b.ch3.WAVE_D6);
    /*_p17.BEPA*/ next.ch3.SAMPLE_7 = tock_pos(a.ch3.SAMPLE_CLKb, b.ch3.SAMPLE_CLKb, APU_RESETv, b.ch3.SAMPLE_7, b.ch3.WAVE_D7);

    /*_p17.CUGO*/ wire WAVE_D0n = not1(b.ch3.WAVE_D0);
    /*_p17.CEGU*/ wire WAVE_D1n = not1(b.ch3.WAVE_D1);
    /*_p17.ATEC*/ wire WAVE_D2n = not1(b.ch3.WAVE_D2);
    /*_p17.ADOK*/ wire WAVE_D3n = not1(b.ch3.WAVE_D3);
    /*_p17.BACA*/ wire WAVE_D4n = not1(b.ch3.WAVE_D4);
    /*_p17.BERO*/ wire WAVE_D5n = not1(b.ch3.WAVE_D5);
    /*_p17.CUTO*/ wire WAVE_D6n = not1(b.ch3.WAVE_D6);
    /*_p17.AKAF*/ wire WAVE_D7n = not1(b.ch3.WAVE_D7);

    /*_p10.ASAD*/ wire A06n = not1(b.bus.BUS_CPU_A06());
    /*_p10.AVUN*/ wire A07n = not1(b.bus.BUS_CPU_A07());
    /*_p10.ACOM*/ wire ADDR_XX3Xn = nand2(A07n, A06n, b.bus.BUS_CPU_A05(), b.bus.BUS_CPU_A04());
    /*_p10.BARO*/ wire ADDR_FF3X  = nor4(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);

    /*_p09.AGUZ*/ wire CPU_RDn = not1(b.ctl.TEDO_CPURD);
    /*_p17.BOKE*/ wire CPU_RDa = not1(CPU_RDn);
    /*_p17.BENA*/ wire CPU_WAVE_RD  = nand2(CPU_RDa, ADDR_FF3X);
    /*_p17.CAZU*/ wire CPU_WAVE_RDn = not1(CPU_WAVE_RD);

    if (!CPU_WAVE_RDn) next.bus.set_data(
      /*_p17.DUGU*/ WAVE_D0n,
      /*_p17.DESY*/ WAVE_D1n,
      /*_p17.BATY*/ WAVE_D2n,
      /*_p17.BADE*/ WAVE_D3n,
      /*_p17.BUNE*/ WAVE_D4n,
      /*_p17.BAVA*/ WAVE_D5n,
      /*_p17.DESA*/ WAVE_D6n,
      /*_p17.BEZU*/ WAVE_D7n
    );
  }

  //----------
  // Output

  {
    /*_p18.COKA*/ wire CH3_ACTIVE = not1(!b.ch3.CH3_ACTIVEo);

    /*_p17.DATE*/ wire WAVE_PLAY_D0 = mux2n(!b.ch3.SAMPLE_0, !b.ch3.SAMPLE_4, b.ch3.WAVE_IDX0);
    /*_p17.DAZY*/ wire WAVE_PLAY_D1 = mux2n(!b.ch3.SAMPLE_1, !b.ch3.SAMPLE_5, b.ch3.WAVE_IDX0);
    /*_p17.COPO*/ wire WAVE_PLAY_D3 = mux2n(!b.ch3.SAMPLE_2, !b.ch3.SAMPLE_6, b.ch3.WAVE_IDX0);
    /*_p17.CUZO*/ wire WAVE_PLAY_D2 = mux2n(!b.ch3.SAMPLE_3, !b.ch3.SAMPLE_7, b.ch3.WAVE_IDX0);

    /*_p18.GEMY*/ wire WAVE_VOL3 = nor4(!b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);
    /*_p18.GOKA*/ wire WAVE_VOL2 = nor4(!b.ch3.NR32_VOL0,  b.ch3.NR32_VOL0);
    /*_p18.GEGE*/ wire WAVE_VOL4 = nor4( b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);

    /*_p18.EZAG*/ wire WAVE_OUT0 = amux3(WAVE_PLAY_D0, WAVE_VOL4, WAVE_PLAY_D1, WAVE_VOL2, WAVE_PLAY_D2, WAVE_VOL3);
    /*_p18.EVUG*/ wire WAVE_OUT1 = amux3(WAVE_PLAY_D1, WAVE_VOL4, WAVE_PLAY_D2, WAVE_VOL2, WAVE_PLAY_D3, WAVE_VOL3);
    /*_p18.DOKY*/ wire WAVE_OUT2 = amux2(WAVE_VOL4, WAVE_PLAY_D2, WAVE_PLAY_D3, WAVE_VOL3);
    /*_p18.DORE*/ wire WAVE_OUT3 = and2(WAVE_PLAY_D3, WAVE_VOL4);

    /*_p18.BARY*/ next.ch3.WAVE_DAC0 = and2(CH3_ACTIVE, WAVE_OUT0);
    /*_p18.BYKA*/ next.ch3.WAVE_DAC1 = and2(CH3_ACTIVE, WAVE_OUT1);
    /*_p18.BOPA*/ next.ch3.WAVE_DAC2 = and2(CH3_ACTIVE, WAVE_OUT2);
    /*_p18.BELY*/ next.ch3.WAVE_DAC3 = and2(CH3_ACTIVE, WAVE_OUT3);

    /*_p18.BOLE*/ next.ch3.WAVE_A0 = mux2(b.ch3.WAVE_IDX1, b.bus.BUS_CPU_A00(), CH3_ACTIVE);
    /*_p18.AGYL*/ next.ch3.WAVE_A1 = mux2(b.ch3.WAVE_IDX2, b.bus.BUS_CPU_A01(), CH3_ACTIVE);
    /*_p18.AFUM*/ next.ch3.WAVE_A2 = mux2(b.ch3.WAVE_IDX3, b.bus.BUS_CPU_A02(), CH3_ACTIVE);
    /*_p18.AXOL*/ next.ch3.WAVE_A3 = mux2(b.ch3.WAVE_IDX4, b.bus.BUS_CPU_A03(), CH3_ACTIVE);
  }
}

#endif

//-----------------------------------------------------------------------------
