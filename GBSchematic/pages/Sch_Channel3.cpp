#include "Sch_Channel3.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void P16_Ch3Regs_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)chip_in;
  (void)cpu_in;

  //----------
  // FF1A NR30 - Channel 3 Sound on/off (R/W)

  {
    /*p10.EXAT*/ wire ADDR_1010an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*p10.EMOR*/ wire ADDR_FF1A  = nor(b.apu.ADDR_FF1Xn, ADDR_1010an);

    /*p16.GOVE*/ wire APU_RESETo = not(b.apu.APU_RESET1);
    /*p16.GEJO*/ wire FF1A_WR = and(b.apu.APU_WR, ADDR_FF1A);
    /*p16.GUCY*/ c.ch3.FF1A_WRn = not(FF1A_WR);
    /*p16.GUXE*/ c.ch3.CH3_AMP_ENna = tock_pos(a.ch3.FF1A_WRn, b.ch3.FF1A_WRn, APU_RESETo, b.ch3.CH3_AMP_ENna, b.D7);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p09.GAXO*/ wire CPU_RDd = not(CPU_RDn);
    /*p16.FASY*/ wire FF1A_RD = nand(ADDR_FF1A, CPU_RDd);
    /*p16.FEVO*/ wire CH3_AMP_ENa  = not(!b.ch3.CH3_AMP_ENna);
    /*p16.GEKO*/ if (FF1A_RD) c.D7 = CH3_AMP_ENa;
  }

  //----------
  // FF1B - NR31 - Channel 3 Sound Length

  {
    /*p18.GEPY*/ wire GEPY = nor(c.ch3.LEN_DONE, b.apu.CLK_256a, !b.ch3.NR34_STOP);
    /*p18.GENU*/ c.ch3.GENU_CLK = not(GEPY);
    /*p18.FALU*/ c.ch3.FALU_CLK = not(b.ch3.CH3_LEN3);

    /*p10.EMAX*/ wire ADDR_1011an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx1); 
    /*p10.DUSA*/ wire ADDR_FF1B  = nor(b.apu.ADDR_FF1Xn, ADDR_1011an);

    /*p16.DERY*/ wire FF1B_WR = nand(b.apu.APU_WR, ADDR_FF1B);
    /*p16.EMUT*/ wire EMUT = not(FF1B_WR);
    /*p16.GAJY*/ wire GAJY = not(FF1B_WR);

    /*p18.GEVO*/ c.ch3.CH3_LEN0 = count_pos(a.ch3.GENU_CLK, b.ch3.GENU_CLK, GAJY, b.ch3.CH3_LEN0, b.D0);
    /*p18.FORY*/ c.ch3.CH3_LEN1 = count_pos(a.ch3.CH3_LEN0, b.ch3.CH3_LEN0, GAJY, b.ch3.CH3_LEN1, b.D1);
    /*p18.GATU*/ c.ch3.CH3_LEN2 = count_pos(a.ch3.CH3_LEN1, b.ch3.CH3_LEN1, GAJY, b.ch3.CH3_LEN2, b.D2);
    /*p18.GAPO*/ c.ch3.CH3_LEN3 = count_pos(a.ch3.CH3_LEN2, b.ch3.CH3_LEN2, GAJY, b.ch3.CH3_LEN3, b.D3);
    /*p18.GEMO*/ c.ch3.CH3_LEN4 = count_pos(a.ch3.FALU_CLK, b.ch3.FALU_CLK, EMUT, b.ch3.CH3_LEN4, b.D4);
    /*p18.FORO*/ c.ch3.CH3_LEN5 = count_pos(a.ch3.CH3_LEN4, b.ch3.CH3_LEN4, EMUT, b.ch3.CH3_LEN5, b.D5);
    /*p18.FAVE*/ c.ch3.CH3_LEN6 = count_pos(a.ch3.CH3_LEN5, b.ch3.CH3_LEN5, EMUT, b.ch3.CH3_LEN6, b.D6);
    /*p18.FYRU*/ c.ch3.CH3_LEN7 = count_pos(a.ch3.CH3_LEN6, b.ch3.CH3_LEN6, EMUT, b.ch3.CH3_LEN7, b.D7);

    /*p16.GETO*/ wire GETO = not(FF1B_WR);
    /*p18.GUDA*/ wire GUDA = nor(GETO, b.apu.APU_RESET1, b.ch3.GARA);
    /*p18.FEXU*/ c.ch3.LEN_DONE = tock_pos(!a.ch3.CH3_LEN7, !b.ch3.CH3_LEN7, GUDA, b.ch3.LEN_DONE, !b.ch3.LEN_DONE);
  }

  //----------
  // FF1C NR32 - Channel 3 Select output level (R/W)

  {
    /*p10.GANY*/ wire ADDR_1100an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.GEFO*/ wire ADDR_FF1C  = nor(b.apu.ADDR_FF1Xn, ADDR_1100an);

    /*p16.GURO*/ wire APU_RESETn = not(b.apu.APU_RESET1);

    /*p16.HAGA*/ wire FF1C_WR = and(b.apu.APU_WR, ADDR_FF1C);

    /*p16.GUZU*/ c.ch3.FF1C_WRn = not(FF1C_WR);
    /*p16.HUKY*/ c.ch3.NR32_VOL0 = tock_pos(a.ch3.FF1C_WRn, b.ch3.FF1C_WRn, APU_RESETn, b.ch3.NR32_VOL0, b.D5);
    /*p16.HODY*/ c.ch3.NR32_VOL1 = tock_pos(a.ch3.FF1C_WRn, b.ch3.FF1C_WRn, APU_RESETn, b.ch3.NR32_VOL1, b.D6);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p16.JOTU*/ wire CPU_RDb = not(CPU_RDn);
    /*p16.HENU*/ wire FF1C_RD = nand(ADDR_FF1C, CPU_RDb);

    /*p18.JECO*/ wire DBG_CPU_RDb = not(CPU_RDn);
    /*p18.HONY*/ wire DBG_FF1C_RD = and(b.apu.NR52_DBG_APU, DBG_CPU_RDb, ADDR_FF1C);
    /*p18.GENO*/ wire DBG_FF1C_RDn = not(DBG_FF1C_RD);

    /*p18.FAPY*/ if (DBG_FF1C_RDn) c.D0 = b.ch3.WAVE_IDX0;
    /*p18.FARO*/ if (DBG_FF1C_RDn) c.D1 = b.ch3.WAVE_IDX1;
    /*p18.FOTE*/ if (DBG_FF1C_RDn) c.D2 = b.ch3.WAVE_IDX2;
    /*p18.FANA*/ if (DBG_FF1C_RDn) c.D3 = b.ch3.WAVE_IDX3;
    /*p18.FERA*/ if (DBG_FF1C_RDn) c.D4 = b.ch3.WAVE_IDX4;
    /*p16.HAMU*/ if (FF1C_RD)      c.D5 = b.ch3.NR32_VOL0;
    /*p16.HUCO*/ if (FF1C_RD)      c.D6 = b.ch3.NR32_VOL1;
  }

  //----------
  // FF1D - NR33 - Channel 3 Frequency's lower data (W)
  
  {
    /*p10.EMOS*/ wire ADDR_1101an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.FENY*/ wire ADDR_FF1D  = nor(b.apu.ADDR_FF1Xn, ADDR_1101an);
    /*p16.HOXA*/ wire ADDR_FF1Dn = not(ADDR_FF1D);

    /*p16.KOTA*/ wire FF1D_WRn = nand(ADDR_FF1D, b.apu.APU_WR);
    /*p16.JAFA*/ wire FF1D_WRo = nand(ADDR_FF1D, b.apu.APU_WR);

    /*p16.KYHO*/ c.ch3.FF1D_WRa = not(FF1D_WRn);
    /*p16.KULY*/ c.ch3.FF1D_WRb = not(FF1D_WRo);
    /*p16.KUHA*/ c.ch3.APU_RESETq = not(b.apu.APU_RESET1);

    /*p16.KOGA*/ c.ch3.NR33_FREQ0 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ0, b.D0);
    /*p16.JOVY*/ c.ch3.NR33_FREQ1 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ1, b.D1);
    /*p16.JAXA*/ c.ch3.NR33_FREQ2 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ2, b.D2);
    /*p16.JEFE*/ c.ch3.NR33_FREQ3 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ3, b.D3);
    /*p16.JYPO*/ c.ch3.NR33_FREQ4 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ4, b.D4);
    /*p16.JOVE*/ c.ch3.NR33_FREQ5 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ5, b.D5);
    /*p16.KANA*/ c.ch3.NR33_FREQ6 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ6, b.D6);
    /*p16.KOGU*/ c.ch3.NR33_FREQ7 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ7, b.D7);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p16.DOVO*/ wire CPU_RD2 = not(CPU_RDn);
    /*p16.EGAD*/ wire DBG_CPU_RD = nand(b.apu.NR52_DBG_APU, CPU_RD2);
    /*p16.GUTE*/ wire DBG_FF1D_RDn = nor(ADDR_FF1Dn, DBG_CPU_RD);
    /*p16.HOVO*/ wire DBG_FF1D_RD = not(DBG_FF1D_RDn);

    /*p16.JOFO*/ if (DBG_FF1D_RD) c.D0 = b.ch3.FREQ_00;
    /*p16.KAFU*/ if (DBG_FF1D_RD) c.D1 = b.ch3.FREQ_01;
    /*p16.KESY*/ if (DBG_FF1D_RD) c.D2 = b.ch3.FREQ_02;
    /*p16.JUDE*/ if (DBG_FF1D_RD) c.D3 = b.ch3.FREQ_03;
    /*p16.JUKE*/ if (DBG_FF1D_RD) c.D4 = b.ch3.FREQ_04;
    /*p16.JEZA*/ if (DBG_FF1D_RD) c.D5 = b.ch3.FREQ_05;
    /*p16.KORA*/ if (DBG_FF1D_RD) c.D6 = b.ch3.FREQ_06;
    /*p16.KAMY*/ if (DBG_FF1D_RD) c.D7 = b.ch3.FREQ_07;
  }

  //----------
  // FF1E - NR34 - Channel 3 Frequency's higher data (R/W)

  {
    /*p10.EGEN*/ wire ADDR_1110an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*p10.DUGO*/ wire ADDR_FF1E  = nor(b.apu.ADDR_FF1Xn, ADDR_1110an);

    /*p16.HUDA*/ wire FF1E_WRn = nand(ADDR_FF1E, b.apu.APU_WR); // polarity?

    /*p16.KOPY*/ wire APU_RESETs  = not(b.apu.APU_RESET1);
    /*p16.HEKY*/ wire APU_RESETr  = not(b.apu.APU_RESET1);
    /*p16.FAKO*/ wire RESTART_RST = nor(b.apu.APU_RESET1, b.ch3.CH3_RESTART_SYNC);

    /*p16.JUZO*/ c.ch3.FF1E_WR = not(FF1E_WRn);
    /*p16.ANUJ*/ wire CPU_WR_WEIRD = and(cpu_in.FROM_CPU5, b.apu.APU_WR);
    /*p16.FOVO*/ c.ch3.FF1E_WRo = nand(CPU_WR_WEIRD, ADDR_FF1E);
    /*p16.EPYX*/ c.ch3.FF1E_WRp = nor(b.apu.APU_WR, ADDR_FF1E); // polarity?

    /*p16.JEMO*/ c.ch3.NR34_FREQ8  = tock_pos(a.ch3.FF1E_WR,  b.ch3.FF1E_WR,  APU_RESETs,  b.ch3.NR34_FREQ8,  b.D0);
    /*p16.JETY*/ c.ch3.NR34_FREQ9  = tock_pos(a.ch3.FF1E_WR,  b.ch3.FF1E_WR,  APU_RESETs,  b.ch3.NR34_FREQ9,  b.D1);
    /*p16.JACY*/ c.ch3.NR34_FREQ10 = tock_pos(a.ch3.FF1E_WR,  b.ch3.FF1E_WR,  APU_RESETs,  b.ch3.NR34_FREQ10, b.D2);
    /*p16.HOTO*/ c.ch3.NR34_STOP   = tock_pos(a.ch3.FF1E_WRo, b.ch3.FF1E_WRo, APU_RESETr,  b.ch3.NR34_STOP,   b.D6);
    /*p16.GAVU*/ c.ch3.NR34_START  = tock_pos(a.ch3.FF1E_WRp, b.ch3.FF1E_WRp, RESTART_RST, b.ch3.NR34_START,  b.D7);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p16.DOVO*/ wire CPU_RD2 = not(CPU_RDn);
    /*p16.EGAD*/ wire DBG_CPU_RD = nand(b.apu.NR52_DBG_APU, CPU_RD2);

    /*p16.GUNU*/ wire GUNU = not(ADDR_FF1E);
    /*p16.FUVA*/ wire FF14_RDb = or(GUNU, DBG_CPU_RD);
    /*p16.JUVY*/ if (FF14_RDb) c.D0 = b.ch3.FREQ_08;
    /*p16.JURA*/ if (FF14_RDb) c.D1 = b.ch3.FREQ_09;
    /*p16.HUFO*/ if (FF14_RDb) c.D2 = b.ch3.FREQ_10;

    /*p16.GORY*/ wire GORY = not(CPU_RDn); // polarity?
    /*p16.GAWA*/ wire FF14_RDa = nand(ADDR_FF1E, GORY); // polarity?
    /*p16.HACA*/ if (FF14_RDa) c.D6 = b.ch3.NR34_STOP;
  }

  //----------
  // Frequency counter

  {
    /*p16.FURY*/ wire APU_RESETn = nor(b.ch3.GYTA, b.apu.APU_RESET1);
    /*p16.GULO*/ wire GULO = not(APU_RESETn);
    /*p16.GOFY*/ wire GOFY = or(GULO, b.ch3.CH3_RESTART_SYNC);

    /*p16.GAZE*/ wire APU_RESETo = not(b.apu.APU_RESET1);

    /*p16.FABO*/ c.ch3.CLK_xxCDxxGH = not(b.apu.CLK_ABxxEFxx1);
    /*p16.GARA*/ c.ch3.GARA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  APU_RESETn, b.ch3.GARA, GOFY);
    /*p16.GYTA*/ c.ch3.GYTA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  APU_RESETo, b.ch3.GYTA, b.ch3.GARA);
    /*p16.GYRA*/ c.ch3.GYRA = tock_pos(a.apu.CLK_ABxxEFxx1, b.apu.CLK_ABxxEFxx1, APU_RESETo, b.ch3.GYRA, b.ch3.GYTA);
  }

  {
    /*p16.FAJU*/ wire FAJU = not(b.ch3.GYRA);
    /*p16.FUVO*/ wire FUVO = nor(b.ch3.CH3_AMP_ENna, b.apu.APU_RESET1);
    /*p16.GUGU*/ wire FREQ_CLK_STOP = or(FUVO, FAJU);
    /*p18.HEFO*/ wire FREQ_CLKn = nor(b.apu.CLK_ABxxEFxx1, FREQ_CLK_STOP);

    /*p18.HEMA*/ wire WAVE_CLKb = not(b.ch3.WAVE_CLKo);
    /*p18.GASE*/ wire WAVE_CLKn = not(WAVE_CLKb);
    /*p18.HERA*/ wire FREQ_RST  = nor(WAVE_CLKn, b.ch3.GARA);
    /*p18.KYKO*/ wire FREQ_RSTn = not(FREQ_RST);
    /*p18.JERA*/ wire FREQ_RSTo = not(FREQ_RST);
    /*p18.KASO*/ wire FREQ_RSTp = not(FREQ_RST);
    
    /*p18.JUTY*/ c.ch3.FREQ_CLKa = not(FREQ_CLKn);
    /*p18.KYRU*/ c.ch3.FREQ_CLKb = not(!b.ch3.FREQ_03);
    /*p18.KESE*/ c.ch3.FREQ_CLKc = not(!b.ch3.FREQ_07);

    /*p18.KUTU*/ c.ch3.FREQ_00 = count_pos(a.ch3.FREQ_CLKa, b.ch3.FREQ_CLKa, FREQ_RSTn, b.ch3.FREQ_00, b.ch3.NR33_FREQ0);
    /*p18.KUPE*/ c.ch3.FREQ_01 = count_pos(a.ch3.FREQ_00,   b.ch3.FREQ_00,   FREQ_RSTn, b.ch3.FREQ_01, b.ch3.NR33_FREQ1);
    /*p18.KUNU*/ c.ch3.FREQ_02 = count_pos(a.ch3.FREQ_01,   b.ch3.FREQ_01,   FREQ_RSTn, b.ch3.FREQ_02, b.ch3.NR33_FREQ2);
    /*p18.KEMU*/ c.ch3.FREQ_03 = count_pos(a.ch3.FREQ_02,   b.ch3.FREQ_02,   FREQ_RSTn, b.ch3.FREQ_03, b.ch3.NR33_FREQ3);
    /*p18.KYGU*/ c.ch3.FREQ_04 = count_pos(a.ch3.FREQ_CLKb, b.ch3.FREQ_CLKb, FREQ_RSTo, b.ch3.FREQ_04, b.ch3.NR33_FREQ4);
    /*p18.KEPA*/ c.ch3.FREQ_05 = count_pos(a.ch3.FREQ_04,   b.ch3.FREQ_04,   FREQ_RSTo, b.ch3.FREQ_05, b.ch3.NR33_FREQ5);
    /*p18.KAFO*/ c.ch3.FREQ_06 = count_pos(a.ch3.FREQ_05,   b.ch3.FREQ_05,   FREQ_RSTo, b.ch3.FREQ_06, b.ch3.NR33_FREQ6);
    /*p18.KENO*/ c.ch3.FREQ_07 = count_pos(a.ch3.FREQ_06,   b.ch3.FREQ_06,   FREQ_RSTo, b.ch3.FREQ_07, b.ch3.NR33_FREQ7);
    /*p18.KEJU*/ c.ch3.FREQ_08 = count_pos(a.ch3.FREQ_CLKc, b.ch3.FREQ_CLKc, FREQ_RSTp, b.ch3.FREQ_08, b.ch3.NR34_FREQ8);
    /*p18.KEZA*/ c.ch3.FREQ_09 = count_pos(a.ch3.FREQ_08,   b.ch3.FREQ_08,   FREQ_RSTp, b.ch3.FREQ_09, b.ch3.NR34_FREQ9);
    /*p18.JAPU*/ c.ch3.FREQ_10 = count_pos(a.ch3.FREQ_09,   b.ch3.FREQ_09,   FREQ_RSTp, b.ch3.FREQ_10, b.ch3.NR34_FREQ10);
  }

  {
    /*p18.HUPA*/ wire HUPA = and(b.ch3.WAVE_CLKo, b.apu.CLK_ABxxEFxx1);
    /*p18.GAFU*/ wire GAFU = nor(b.apu.APU_RESET1, b.ch3.GARA, HUPA);
    
    /*p18.JYFO*/ c.ch3.JYFO_CLK  = not(b.ch3.FREQ_10);
    /*p18.HUNO*/ c.ch3.WAVE_CLKo = tock_pos(a.ch3.JYFO_CLK, b.ch3.JYFO_CLK, GAFU, b.ch3.WAVE_CLKo, !b.ch3.WAVE_CLKo);

    /*p18.HEMA*/ wire WAVE_CLKb = not(b.ch3.WAVE_CLKo);
    /*p18.GASE*/ wire WAVE_CLKn = not(WAVE_CLKb);

    /*p18.FOTO*/ wire FOTO = and(b.ch3.WAVE_LOOP, WAVE_CLKn);
    /*p18.ETAN*/ wire WAVE_RST = or(b.ch3.GARA, b.ch3.WAVE_LOOP);
    /*p18.GYRY*/ wire LOOP_RST = nor(b.apu.APU_RESET1, b.ch3.GARA, FOTO);

    /*p18.DERO*/ c.ch3.WAVE_CLK  = not(WAVE_CLKn);
    /*p18.EFAR*/ c.ch3.WAVE_IDX0 = tock_pos( a.ch3.WAVE_CLK,   b.ch3.WAVE_CLK,  WAVE_RST, b.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0);
    /*p18.ERUS*/ c.ch3.WAVE_IDX1 = tock_pos(!a.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0, WAVE_RST, b.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1);
    /*p18.EFUZ*/ c.ch3.WAVE_IDX2 = tock_pos(!a.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1, WAVE_RST, b.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2);
    /*p18.EXEL*/ c.ch3.WAVE_IDX3 = tock_pos(!a.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2, WAVE_RST, b.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3);
    /*p18.EFAL*/ c.ch3.WAVE_IDX4 = tock_pos(!a.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3, WAVE_RST, b.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4);
    /*p18.FETY*/ c.ch3.WAVE_LOOP = tock_pos(!a.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4, LOOP_RST, b.ch3.WAVE_LOOP, !b.ch3.WAVE_LOOP);
  }




  //----------
  // Wave ram control signals

  {
    /*p16.GOMA*/ wire APU_RESETn = not(b.apu.APU_RESET1);
    /*p16.FOBA*/ c.ch3.CH3_RESTART_SYNC = tock_pos(a.sys.PHASE_ABCDxxxx7c, b.sys.PHASE_ABCDxxxx7c, APU_RESETn, b.ch3.CH3_RESTART_SYNC, b.ch3.NR34_START);
  }

  {
    /*p18.GEDO*/ c.ch3.GEDO = and(b.apu.CLK_256a, b.ch3.NR34_STOP);
    /*p18.FYGO*/ c.ch3.FYGO = or(b.apu.APU_RESET1, b.ch3.GEDO, b.ch3.CH3_AMP_ENna);
    /*p18.FOZU*/ c.ch3.FOZU = or(b.ch3.GARA, b.ch3.FYGO);
    /*p18.EZAS*/ c.ch3.EZAS = not(b.ch3.FOZU);
    /*p18.DORU*/ c.ch3.DORU = not(b.ch3.EZAS);

    /*p18.CALU*/ wire APU_RESETn = not(b.apu.APU_RESET1);
    /*p18.DAVO*/ c.ch3.CH3_ACTIVEo = tock_pos(a.apu.AJER_2M, b.apu.AJER_2M, APU_RESETn, b.ch3.CH3_ACTIVEo, b.ch3.DORU);
  }

  {
    /*p17.BAMA*/ wire APU_RESETn = not(b.apu.APU_RESET1);
    /*p17.COZY*/ c.ch3.COZY_CLK = not(b.apu.CLK_xBxDxFxH1);
    /*p17.ARUC*/ c.ch3.ARUC_CLK = not(b.apu.CLK_xBxDxFxH1);

    /*p18.HEMA*/ wire WAVE_CLKb = not(b.ch3.WAVE_CLKo);
    /*p18.GASE*/ wire WAVE_CLKn = not(WAVE_CLKb);
    /*p17.BUSA*/ c.ch3.BUSA        = tock_pos(a.apu.CLK_xBxDxFxH1, b.apu.CLK_xBxDxFxH1, APU_RESETn, b.ch3.BUSA,        WAVE_CLKn); // ?
    /*p17.BANO*/ c.ch3.BANO        = tock_pos(a.ch3.COZY_CLK,      b.ch3.COZY_CLK,      APU_RESETn, b.ch3.BANO,        b.ch3.BUSA);
    /*p17.AZUS*/ c.ch3.SAMPLE_CLKn = tock_pos(a.apu.CLK_xBxDxFxH1, b.apu.CLK_xBxDxFxH1, APU_RESETn, b.ch3.SAMPLE_CLKn, b.ch3.BANO);
    /*p17.AZET*/ c.ch3.AZET        = tock_pos(a.ch3.ARUC_CLK,      b.ch3.ARUC_CLK,      APU_RESETn, b.ch3.AZET,        b.ch3.SAMPLE_CLKn);
  }

  {
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( b.sys.PHASE_xBCDExxx1);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(!b.sys.PHASE_xxCDEFxx1);
    /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_AxxxxxGH = nor(b.sys.CPUCLK_REQn, BUGO_xBCDExxx, AROV_xxCDEFxx);
    /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
    /*p01.BUKE*/ wire BUKE_AxxxxxGH = not(BASU_xBCDEFxx);
    /*p17.ABUR*/ wire ABUR_xBCDEFxx = not(BUKE_AxxxxxGH);
    /*p17.BORY*/ wire BORY_AxxxxxGH = not(ABUR_xBCDEFxx);

    /*p10.ASAD*/ wire A06n = not(b.A06);
    /*p10.AVUN*/ wire A07n = not(b.A07);
    /*p10.ACOM*/ wire ADDR_XX3Xn = nand(A07n, A06n, b.A05, b.A04);

    /*p10.BARO*/ wire ADDR_FF3X  = nor(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);

    /*p17.BYZA*/ wire WAVE_WR = and(b.apu.APU_WR, ADDR_FF3X);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p17.BOKE*/ wire CPU_RDa = not(CPU_RDn);
    /*p17.BENA*/ wire CPU_WAVE_RD  = nand(CPU_RDa, ADDR_FF3X);
    /*p17.CAZU*/ wire CPU_WAVE_RDn = not(CPU_WAVE_RD);
    /*p17.BETA*/ wire BETA = or(WAVE_WR, CPU_WAVE_RDn, BORY_AxxxxxGH);

    /*p17.BOXO*/ wire BOXO = nor(b.ch3.SAMPLE_CLKn, b.ch3.AZET);
    /*p17.AZOR*/ wire AZOR = not(BETA);

    /*p17.BORU*/ wire BORU = not(BOXO);
    /*p17.BUKU*/ wire BUKU = not(AZOR);
    /*p18.COKA*/ wire CH3_ACTIVE = not(!b.ch3.CH3_ACTIVEo);

    /*p17.ATUR*/   wire WAVE_RAM_CTRL1n = mux2(BORU, BUKU, CH3_ACTIVE);
    /*p17.ALER*/ c.ch3.WAVE_RAM_CTRL1 = not(WAVE_RAM_CTRL1n);
  }

  {
    /*p10.ASAD*/ wire A06n = not(b.A06);
    /*p10.AVUN*/ wire A07n = not(b.A07);
    /*p10.ACOM*/ wire ADDR_XX3Xn = nand(A07n, A06n, b.A05, b.A04);

    /*p10.BARO*/ wire ADDR_FF3X  = nor(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);
    /*p17.BUTU*/ wire SAMPLE_CLK = not(b.ch3.SAMPLE_CLKn);
    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p17.BOKE*/ wire CPU_RDa = not(CPU_RDn);
    /*p17.BENA*/ wire CPU_WAVE_RD  = nand(CPU_RDa, ADDR_FF3X);
    /*p17.CAZU*/ wire CPU_WAVE_RDn = not(CPU_WAVE_RD);

    /*p18.COKA*/ wire CH3_ACTIVE = not(!b.ch3.CH3_ACTIVEo);

    /*p18.BENO*/ wire WAVE_RAM_CTRL3n = mux2(SAMPLE_CLK, CPU_WAVE_RDn, CH3_ACTIVE);
    /*p18.ATOK*/ c.ch3.WAVE_RAM_CTRL3 = not(WAVE_RAM_CTRL3n);  
  }

  {
    /*p10.ASAD*/ wire A06n = not(b.A06);
    /*p10.AVUN*/ wire A07n = not(b.A07);
    /*p10.ACOM*/ wire ADDR_XX3Xn = nand(A07n, A06n, b.A05, b.A04);

    /*p10.BARO*/ wire ADDR_FF3X  = nor(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);
    /*p17.BYZA*/ wire WAVE_WR = and(b.apu.APU_WR, ADDR_FF3X);
    /*p17.AMYT*/ c.ch3.WAVE_WRn = not(WAVE_WR);
  }











  //----------
  {
    /*p17.ACOR*/ wire APU_RESETv = not(b.apu.APU_RESET1);
    /*p17.BUTU*/ wire SAMPLE_CLK = not(b.ch3.SAMPLE_CLKn);

    /*p17.BEKA*/ c.ch3.SAMPLE_CLKa = not(SAMPLE_CLK);
    /*p17.COJU*/ c.ch3.SAMPLE_CLKb = not(SAMPLE_CLK);
    /*p17.BAJA*/ c.ch3.SAMPLE_CLKc = not(SAMPLE_CLK);
    /*p17.BUFE*/ c.ch3.SAMPLE_CLKd = not(SAMPLE_CLK);

    /*p17.CYFO*/ c.ch3.SAMPLE_0 = tock_pos(a.ch3.SAMPLE_CLKa, b.ch3.SAMPLE_CLKa, APU_RESETv, b.ch3.SAMPLE_0, b.ch3.WAVE_D0);
    /*p17.CESY*/ c.ch3.SAMPLE_1 = tock_pos(a.ch3.SAMPLE_CLKd, b.ch3.SAMPLE_CLKd, APU_RESETv, b.ch3.SAMPLE_1, b.ch3.WAVE_D1);
    /*p17.BUDY*/ c.ch3.SAMPLE_2 = tock_pos(a.ch3.SAMPLE_CLKc, b.ch3.SAMPLE_CLKc, APU_RESETv, b.ch3.SAMPLE_2, b.ch3.WAVE_D2);
    /*p17.BEGU*/ c.ch3.SAMPLE_3 = tock_pos(a.ch3.SAMPLE_CLKb, b.ch3.SAMPLE_CLKb, APU_RESETv, b.ch3.SAMPLE_3, b.ch3.WAVE_D3);
    /*p17.CUVO*/ c.ch3.SAMPLE_4 = tock_pos(a.ch3.SAMPLE_CLKa, b.ch3.SAMPLE_CLKa, APU_RESETv, b.ch3.SAMPLE_4, b.ch3.WAVE_D4);
    /*p17.CEVO*/ c.ch3.SAMPLE_5 = tock_pos(a.ch3.SAMPLE_CLKd, b.ch3.SAMPLE_CLKd, APU_RESETv, b.ch3.SAMPLE_5, b.ch3.WAVE_D5);
    /*p17.BORA*/ c.ch3.SAMPLE_6 = tock_pos(a.ch3.SAMPLE_CLKc, b.ch3.SAMPLE_CLKc, APU_RESETv, b.ch3.SAMPLE_6, b.ch3.WAVE_D6);
    /*p17.BEPA*/ c.ch3.SAMPLE_7 = tock_pos(a.ch3.SAMPLE_CLKb, b.ch3.SAMPLE_CLKb, APU_RESETv, b.ch3.SAMPLE_7, b.ch3.WAVE_D7);

    /*p17.CUGO*/ wire WAVE_D0n = not(b.ch3.WAVE_D0);
    /*p17.CEGU*/ wire WAVE_D1n = not(b.ch3.WAVE_D1);
    /*p17.ATEC*/ wire WAVE_D2n = not(b.ch3.WAVE_D2);
    /*p17.ADOK*/ wire WAVE_D3n = not(b.ch3.WAVE_D3);
    /*p17.BACA*/ wire WAVE_D4n = not(b.ch3.WAVE_D4);
    /*p17.BERO*/ wire WAVE_D5n = not(b.ch3.WAVE_D5);
    /*p17.CUTO*/ wire WAVE_D6n = not(b.ch3.WAVE_D6);
    /*p17.AKAF*/ wire WAVE_D7n = not(b.ch3.WAVE_D7);

    /*p10.ASAD*/ wire A06n = not(b.A06);
    /*p10.AVUN*/ wire A07n = not(b.A07);
    /*p10.ACOM*/ wire ADDR_XX3Xn = nand(A07n, A06n, b.A05, b.A04);
    /*p10.BARO*/ wire ADDR_FF3X  = nor(ADDR_XX3Xn, b.apu.ADDR_FFXXn1);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p17.BOKE*/ wire CPU_RDa = not(CPU_RDn);
    /*p17.BENA*/ wire CPU_WAVE_RD  = nand(CPU_RDa, ADDR_FF3X);
    /*p17.CAZU*/ wire CPU_WAVE_RDn = not(CPU_WAVE_RD);
    /*p17.DUGU*/ if (CPU_WAVE_RDn) c.D7 = WAVE_D0n;
    /*p17.DESY*/ if (CPU_WAVE_RDn) c.D6 = WAVE_D1n;
    /*p17.BATY*/ if (CPU_WAVE_RDn) c.D5 = WAVE_D2n;
    /*p17.BADE*/ if (CPU_WAVE_RDn) c.D4 = WAVE_D3n;
    /*p17.BUNE*/ if (CPU_WAVE_RDn) c.D3 = WAVE_D4n;
    /*p17.BAVA*/ if (CPU_WAVE_RDn) c.D2 = WAVE_D5n;
    /*p17.DESA*/ if (CPU_WAVE_RDn) c.D1 = WAVE_D6n;
    /*p17.BEZU*/ if (CPU_WAVE_RDn) c.D0 = WAVE_D7n;
  }

  //----------
  // Output

  {
    /*p18.COKA*/ wire CH3_ACTIVE = not(!b.ch3.CH3_ACTIVEo);

    /*p17.DATE*/ wire WAVE_PLAY_D0 = mux2n(!b.ch3.SAMPLE_0, !b.ch3.SAMPLE_4, b.ch3.WAVE_IDX0);
    /*p17.DAZY*/ wire WAVE_PLAY_D1 = mux2n(!b.ch3.SAMPLE_1, !b.ch3.SAMPLE_5, b.ch3.WAVE_IDX0);
    /*p17.COPO*/ wire WAVE_PLAY_D3 = mux2n(!b.ch3.SAMPLE_2, !b.ch3.SAMPLE_6, b.ch3.WAVE_IDX0);
    /*p17.CUZO*/ wire WAVE_PLAY_D2 = mux2n(!b.ch3.SAMPLE_3, !b.ch3.SAMPLE_7, b.ch3.WAVE_IDX0);

    /*p18.GEMY*/ wire WAVE_VOL3 = nor(!b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);
    /*p18.GOKA*/ wire WAVE_VOL2 = nor(!b.ch3.NR32_VOL0,  b.ch3.NR32_VOL0);
    /*p18.GEGE*/ wire WAVE_VOL4 = nor( b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);

    /*p18.EZAG*/ wire WAVE_OUT0 = amux3(WAVE_PLAY_D0, WAVE_VOL4, WAVE_PLAY_D1, WAVE_VOL2, WAVE_PLAY_D2, WAVE_VOL3);
    /*p18.EVUG*/ wire WAVE_OUT1 = amux3(WAVE_PLAY_D1, WAVE_VOL4, WAVE_PLAY_D2, WAVE_VOL2, WAVE_PLAY_D3, WAVE_VOL3);
    /*p18.DOKY*/ wire WAVE_OUT2 = amux2(WAVE_VOL4, WAVE_PLAY_D2, WAVE_PLAY_D3, WAVE_VOL3);
    /*p18.DORE*/ wire WAVE_OUT3 = and(WAVE_PLAY_D3, WAVE_VOL4);

    /*p18.BARY*/ c.ch3.WAVE_DAC0 = and(CH3_ACTIVE, WAVE_OUT0);
    /*p18.BYKA*/ c.ch3.WAVE_DAC1 = and(CH3_ACTIVE, WAVE_OUT1);
    /*p18.BOPA*/ c.ch3.WAVE_DAC2 = and(CH3_ACTIVE, WAVE_OUT2);
    /*p18.BELY*/ c.ch3.WAVE_DAC3 = and(CH3_ACTIVE, WAVE_OUT3);

    /*p18.BOLE*/ c.ch3.WAVE_A0 = mux2(b.ch3.WAVE_IDX1, b.A00, CH3_ACTIVE);
    /*p18.AGYL*/ c.ch3.WAVE_A1 = mux2(b.ch3.WAVE_IDX2, b.A01, CH3_ACTIVE);
    /*p18.AFUM*/ c.ch3.WAVE_A2 = mux2(b.ch3.WAVE_IDX3, b.A02, CH3_ACTIVE);
    /*p18.AXOL*/ c.ch3.WAVE_A3 = mux2(b.ch3.WAVE_IDX4, b.A03, CH3_ACTIVE);
  }
}

//-----------------------------------------------------------------------------

};