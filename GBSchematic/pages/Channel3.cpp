#include "../Schematics.h"
#include "Gameboy.h"

void P16_Ch3Regs_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)chip_in;
  (void)cpu_in;

  //----------
  // FF1A NR30 - Channel 3 Sound on/off (R/W)

  /*p16.FEVO*/ c.ch3.CH3_AMP_ENa = not(!b.ch3.CH3_AMP_ENna);
  /*p16.GUXE*/   c.ch3.CH3_AMP_ENna = tock_pos(a.ch3.FF1A_WRn, b.ch3.FF1A_WRn, b.ch3.APU_RESETn, b.ch3.CH3_AMP_ENna, b.D7);
  /*p16.GEJO*/     c.ch3.FF1A_WR = and(b.apu.APU_WR, b.apu.ADDR_FF1A);
  /*p16.GUCY*/     c.ch3.FF1A_WRn = not(b.ch3.FF1A_WR);
  /*p16.GOVE*/     c.ch3.APU_RESETn = not(b.apu.APU_RESET1);

  /*p16.GEKO*/ if (b.ch3.FF1A_RD) c.D7 = c.ch3.CH3_AMP_ENa;
  /*p16.FASY*/   c.ch3.FF1A_RD = nand(b.apu.ADDR_FF1A, b.apu.CPU_RDd);

  //----------
  // FF1B - NR31 - Channel 3 Sound Length

  /*p18.GEVO*/ c.ch3.CH3_LEN0 = count_pos(a.ch3.GENU,     b.ch3.GENU,     b.ch3.GAJY, b.ch3.CH3_LEN0, b.D0);
  /*p18.FORY*/ c.ch3.CH3_LEN1 = count_pos(a.ch3.CH3_LEN0, b.ch3.CH3_LEN0, b.ch3.GAJY, b.ch3.CH3_LEN1, b.D1);
  /*p18.GATU*/ c.ch3.CH3_LEN2 = count_pos(a.ch3.CH3_LEN1, b.ch3.CH3_LEN1, b.ch3.GAJY, b.ch3.CH3_LEN2, b.D2);
  /*p18.GAPO*/ c.ch3.CH3_LEN3 = count_pos(a.ch3.CH3_LEN2, b.ch3.CH3_LEN2, b.ch3.GAJY, b.ch3.CH3_LEN3, b.D3);
  /*p18.GEMO*/ c.ch3.CH3_LEN4 = count_pos(a.ch3.FALU,     b.ch3.FALU,     b.ch3.EMUT, b.ch3.CH3_LEN4, b.D4);
  /*p18.FORO*/ c.ch3.CH3_LEN5 = count_pos(a.ch3.CH3_LEN4, b.ch3.CH3_LEN4, b.ch3.EMUT, b.ch3.CH3_LEN5, b.D5);
  /*p18.FAVE*/ c.ch3.CH3_LEN6 = count_pos(a.ch3.CH3_LEN5, b.ch3.CH3_LEN5, b.ch3.EMUT, b.ch3.CH3_LEN6, b.D6);
  /*p18.FYRU*/ c.ch3.CH3_LEN7 = count_pos(a.ch3.CH3_LEN6, b.ch3.CH3_LEN6, b.ch3.EMUT, b.ch3.CH3_LEN7, b.D7);
  /*p18.FALU*/   c.ch3.FALU = not(b.ch3.CH3_LEN3);
  /*p16.GAJY*/   c.ch3.GAJY = not(b.ch3.FF1B_WR);
  /*p16.EMUT*/   c.ch3.EMUT = not(b.ch3.FF1B_WR);
  /*p18.GENU*/   c.ch3.GENU = not(b.ch3.GEPY);
  /*p18.GEPY*/     c.ch3.GEPY = nor(c.ch3.LEN_DONE, b.sys.CLK_256a, !b.ch3.NR34_STOP);
  /*p18.FEXU*/       c.ch3.LEN_DONE = tock_pos(!a.ch3.CH3_LEN7, !b.ch3.CH3_LEN7, b.ch3.GUDA, b.ch3.LEN_DONE, !b.ch3.LEN_DONE);
  /*p18.GUDA*/         c.ch3.GUDA = nor(b.ch3.GETO, b.apu.APU_RESET1, b.ch3.GARA);
  /*p16.GETO*/           c.ch3.GETO = not(b.ch3.FF1B_WR);
  /*p16.DERY*/             c.ch3.FF1B_WR = nand(b.apu.APU_WR, b.apu.ADDR_FF1B);

  //----------
  // FF1C NR32 - Channel 3 Select output level (R/W)

  /*p16.HUKY*/ c.ch3.NR32_VOL0 = tock_pos(a.ch3.FF1C_WRn, b.ch3.FF1C_WRn, b.ch3.APU_RESETt, b.ch3.NR32_VOL0, b.D5);
  /*p16.HODY*/ c.ch3.NR32_VOL1 = tock_pos(a.ch3.FF1C_WRn, b.ch3.FF1C_WRn, b.ch3.APU_RESETt, b.ch3.NR32_VOL1, b.D6);
  /*p16.GUZU*/   c.ch3.FF1C_WRn = not(b.ch3.FF1C_WR);
  /*p16.HAGA*/     c.ch3.FF1C_WR = and(b.apu.APU_WR, b.apu.ADDR_FF1C);
  /*p16.GURO*/   c.ch3.APU_RESETt = not(b.apu.APU_RESET1);

  /*p18.FAPY*/ if (b.ch3.DBG_FF1C_RDn) c.D0 = b.ch3.WAVE_IDX0;
  /*p18.FARO*/ if (b.ch3.DBG_FF1C_RDn) c.D1 = b.ch3.WAVE_IDX1;
  /*p18.FOTE*/ if (b.ch3.DBG_FF1C_RDn) c.D2 = b.ch3.WAVE_IDX2;
  /*p18.FANA*/ if (b.ch3.DBG_FF1C_RDn) c.D3 = b.ch3.WAVE_IDX3;
  /*p18.FERA*/ if (b.ch3.DBG_FF1C_RDn) c.D4 = b.ch3.WAVE_IDX4;
  /*p18.GENO*/   c.ch3.DBG_FF1C_RDn = not(b.ch3.DBG_FF1C_RD);
  /*p18.HONY*/     c.ch3.DBG_FF1C_RD = and(b.apu.DBG_APU, b.ch3.DBG_CPU_RDb, b.apu.ADDR_FF1C);
  /*p18.JECO*/       c.ch3.DBG_CPU_RDb = not(b.apu.CPU_RDn);
  /*p16.HAMU*/ if (b.ch3.FF1C_RD) c.D5 = b.ch3.NR32_VOL0;
  /*p16.HUCO*/ if (b.ch3.FF1C_RD) c.D6 = b.ch3.NR32_VOL1;
  /*p16.HENU*/   c.ch3.FF1C_RD = nand(b.apu.ADDR_FF1C, b.ch3.CPU_RDb);
  /*p16.JOTU*/     c.ch3.CPU_RDb = not(b.apu.CPU_RDn);

  //----------
  // FF1D - NR33 - Channel 3 Frequency's lower data (W)
  
  /*p16.KOGA*/ c.ch3.NR33_FREQ0 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ0, b.D0);
  /*p16.JOVY*/ c.ch3.NR33_FREQ1 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ1, b.D1);
  /*p16.JAXA*/ c.ch3.NR33_FREQ2 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ2, b.D2);
  /*p16.JEFE*/ c.ch3.NR33_FREQ3 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ3, b.D3);
  /*p16.JYPO*/ c.ch3.NR33_FREQ4 = tock_pos(a.ch3.FF1D_WRb, b.ch3.FF1D_WRb, b.ch3.APU_RESETq, b.ch3.NR33_FREQ4, b.D4);
  /*p16.JOVE*/ c.ch3.NR33_FREQ5 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ5, b.D5);
  /*p16.KANA*/ c.ch3.NR33_FREQ6 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ6, b.D6);
  /*p16.KOGU*/ c.ch3.NR33_FREQ7 = tock_pos(a.ch3.FF1D_WRa, b.ch3.FF1D_WRa, b.ch3.APU_RESETq, b.ch3.NR33_FREQ7, b.D7);
  /*p16.KOTA*/   c.ch3.FF1D_WRn = nand(b.apu.ADDR_FF1D, b.apu.APU_WR);
  /*p16.KYHO*/     c.ch3.FF1D_WRa = not(b.ch3.FF1D_WRn);
  /*p16.JAFA*/   c.ch3.FF1D_WRo = nand(b.apu.ADDR_FF1D, b.apu.APU_WR);
  /*p16.KULY*/     c.ch3.FF1D_WRb = not(b.ch3.FF1D_WRo);
  /*p16.KUHA*/   c.ch3.APU_RESETq = not(b.apu.APU_RESET1);

  /*p16.JOFO*/ if (b.ch3.DBG_FF1D_RD) c.D0 = b.ch3.FREQ_00;
  /*p16.KAFU*/ if (b.ch3.DBG_FF1D_RD) c.D1 = b.ch3.FREQ_01;
  /*p16.KESY*/ if (b.ch3.DBG_FF1D_RD) c.D2 = b.ch3.FREQ_02;
  /*p16.JUDE*/ if (b.ch3.DBG_FF1D_RD) c.D3 = b.ch3.FREQ_03;
  /*p16.JUKE*/ if (b.ch3.DBG_FF1D_RD) c.D4 = b.ch3.FREQ_04;
  /*p16.JEZA*/ if (b.ch3.DBG_FF1D_RD) c.D5 = b.ch3.FREQ_05;
  /*p16.KORA*/ if (b.ch3.DBG_FF1D_RD) c.D6 = b.ch3.FREQ_06;
  /*p16.KAMY*/ if (b.ch3.DBG_FF1D_RD) c.D7 = b.ch3.FREQ_07;
  /*p16.HOVO*/   c.ch3.DBG_FF1D_RD = not(b.ch3.DBG_FF1D_RDn);
  /*p16.GUTE*/     c.ch3.DBG_FF1D_RDn = nor(b.ch3.ADDR_FF1Dn, b.ch3.DBG_CPU_RD);
  /*p16.HOXA*/       c.ch3.ADDR_FF1Dn = not(b.apu.ADDR_FF1D);
  /*p16.EGAD*/       c.ch3.DBG_CPU_RD = nand(b.apu.DBG_APU, b.ch3.CPU_RDn);
  /*p16.DOVO*/         c.ch3.CPU_RDn = not(b.apu.CPU_RDn);

  //----------
  // FF1E - NR34 - Channel 3 Frequency's higher data (R/W)

  /*p16.JEMO*/ c.ch3.NR34_FREQ8  = tock_pos(a.ch3.FF1E_WR, b.ch3.FF1E_WR,   b.ch3.APU_RESETs,  b.ch3.NR34_FREQ8,  b.D0);
  /*p16.JETY*/ c.ch3.NR34_FREQ9  = tock_pos(a.ch3.FF1E_WR, b.ch3.FF1E_WR,   b.ch3.APU_RESETs,  b.ch3.NR34_FREQ9,  b.D1);
  /*p16.JACY*/ c.ch3.NR34_FREQ10 = tock_pos(a.ch3.FF1E_WR, b.ch3.FF1E_WR,   b.ch3.APU_RESETs,  b.ch3.NR34_FREQ10, b.D2);
  /*p16.HOTO*/ c.ch3.NR34_STOP   = tock_pos(a.ch3.FF1E_WRo, b.ch3.FF1E_WRo, b.ch3.APU_RESETr,  b.ch3.NR34_STOP, b.D6);
  /*p16.GAVU*/ c.ch3.NR34_START  = tock_pos(a.ch3.FF1E_WRp, b.ch3.FF1E_WRp, b.ch3.RESTART_RST, b.ch3.NR34_START, b.D7);
  /*p16.JUZO*/   c.ch3.FF1E_WR = not(b.ch3.FF1E_WRn);
  /*p16.HUDA*/     c.ch3.FF1E_WRn = nand(b.apu.ADDR_FF1E, b.apu.APU_WR); // polarity?
  /*p16.FOVO*/   c.ch3.FF1E_WRo = nand(b.apu.CPU_WR_WEIRD, b.apu.ADDR_FF1E);
  /*p16.EPYX*/   c.ch3.FF1E_WRp = nor(b.apu.APU_WR, b.apu.ADDR_FF1E); // polarity?
  /*p16.KOPY*/   c.ch3.APU_RESETs = not(b.apu.APU_RESET1);
  /*p16.HEKY*/   c.ch3.APU_RESETr = not(b.apu.APU_RESET1);
  /*p16.FAKO*/   c.ch3.RESTART_RST = nor(b.apu.APU_RESET1, b.ch3.CH3_RESTART_SYNC);

  /*p16.JUVY*/ if (b.ch3.FF14_RDb) c.D0 = b.ch3.FREQ_08;
  /*p16.JURA*/ if (b.ch3.FF14_RDb) c.D1 = b.ch3.FREQ_09;
  /*p16.HUFO*/ if (b.ch3.FF14_RDb) c.D2 = b.ch3.FREQ_10;
  /*p16.HACA*/ if (b.ch3.FF14_RDa) c.D6 = b.ch3.NR34_STOP;
  /*p16.GAWA*/   c.ch3.FF14_RDa = nand(b.apu.ADDR_FF1E, b.ch3.GORY); // polarity?
  /*p16.GORY*/     c.ch3.GORY = not(b.apu.CPU_RDn); // polarity?
  /*p16.FUVA*/   c.ch3.FF14_RDb = or(b.ch3.GUNU, b.ch3.DBG_CPU_RD);
  /*p16.GUNU*/     c.ch3.GUNU = not(b.apu.ADDR_FF1E);

  //----------
  // Frequency counter

  /*p18.KUTU*/ c.ch3.FREQ_00 = count_pos(a.ch3.FREQ_CLK,  b.ch3.FREQ_CLK,  b.ch3.FREQ_RSTn, b.ch3.FREQ_00, b.ch3.NR33_FREQ0);
  /*p18.KUPE*/ c.ch3.FREQ_01 = count_pos(a.ch3.FREQ_00,   b.ch3.FREQ_00,   b.ch3.FREQ_RSTn, b.ch3.FREQ_01, b.ch3.NR33_FREQ1);
  /*p18.KUNU*/ c.ch3.FREQ_02 = count_pos(a.ch3.FREQ_01,   b.ch3.FREQ_01,   b.ch3.FREQ_RSTn, b.ch3.FREQ_02, b.ch3.NR33_FREQ2);
  /*p18.KEMU*/ c.ch3.FREQ_03 = count_pos(a.ch3.FREQ_02,   b.ch3.FREQ_02,   b.ch3.FREQ_RSTn, b.ch3.FREQ_03, b.ch3.NR33_FREQ3);
  /*p18.KYGU*/ c.ch3.FREQ_04 = count_pos(a.ch3.FREQ_CLKb, b.ch3.FREQ_CLKb, b.ch3.FREQ_RSTo, b.ch3.FREQ_04, b.ch3.NR33_FREQ4);
  /*p18.KEPA*/ c.ch3.FREQ_05 = count_pos(a.ch3.FREQ_04,   b.ch3.FREQ_04,   b.ch3.FREQ_RSTo, b.ch3.FREQ_05, b.ch3.NR33_FREQ5);
  /*p18.KAFO*/ c.ch3.FREQ_06 = count_pos(a.ch3.FREQ_05,   b.ch3.FREQ_05,   b.ch3.FREQ_RSTo, b.ch3.FREQ_06, b.ch3.NR33_FREQ6);
  /*p18.KENO*/ c.ch3.FREQ_07 = count_pos(a.ch3.FREQ_06,   b.ch3.FREQ_06,   b.ch3.FREQ_RSTo, b.ch3.FREQ_07, b.ch3.NR33_FREQ7);
  /*p18.KEJU*/ c.ch3.FREQ_08 = count_pos(a.ch3.FREQ_CLKc, b.ch3.FREQ_CLKc, b.ch3.FREQ_RSTp, b.ch3.FREQ_08, b.ch3.NR34_FREQ8);
  /*p18.KEZA*/ c.ch3.FREQ_09 = count_pos(a.ch3.FREQ_08,   b.ch3.FREQ_08,   b.ch3.FREQ_RSTp, b.ch3.FREQ_09, b.ch3.NR34_FREQ9);
  /*p18.JAPU*/ c.ch3.FREQ_10 = count_pos(a.ch3.FREQ_09,   b.ch3.FREQ_09,   b.ch3.FREQ_RSTp, b.ch3.FREQ_10, b.ch3.NR34_FREQ10);
  /*p18.JUTY*/   c.ch3.FREQ_CLK = not(b.ch3.FREQ_CLKn);
  /*p18.HEFO*/     c.ch3.FREQ_CLKn = nor(b.sys.CLK_ABxxEFxx1, b.ch3.FREQ_CLK_STOP);
  /*p16.GUGU*/       c.ch3.FREQ_CLK_STOP = or(b.ch3.FUVO, b.ch3.FAJU);
  /*p16.FUVO*/         c.ch3.FUVO = nor(b.ch3.CH3_AMP_ENna, b.apu.APU_RESET1);
  /*p16.FAJU*/         c.ch3.FAJU = not(b.ch3.GYRA);
  /*p18.KYRU*/   c.ch3.FREQ_CLKb = not(!b.ch3.FREQ_03);
  /*p18.KESE*/   c.ch3.FREQ_CLKc = not(!b.ch3.FREQ_07);
  /*p18.KYKO*/   c.ch3.FREQ_RSTn = not(b.ch3.FREQ_RST);
  /*p18.JERA*/   c.ch3.FREQ_RSTo = not(b.ch3.FREQ_RST);
  /*p18.KASO*/   c.ch3.FREQ_RSTp = not(b.ch3.FREQ_RST);
  /*p18.HERA*/     c.ch3.FREQ_RST = nor(b.ch3.WAVE_CLKn, b.ch3.GARA);

  /*p18.GASE*/ c.ch3.WAVE_CLKn = not(b.ch3.WAVE_CLKb);
  /*p18.HEMA*/   c.ch3.WAVE_CLKb = not(b.ch3.WAVE_CLKo);
  /*p18.HUNO*/     c.ch3.WAVE_CLKo = tock_pos(a.ch3.JYFO, b.ch3.JYFO, b.ch3.GAFU, b.ch3.WAVE_CLKo, !b.ch3.WAVE_CLKo);
  /*p18.JYFO*/       c.ch3.JYFO = not(b.ch3.FREQ_10);
  /*p18.GAFU*/       c.ch3.GAFU = nor(b.apu.APU_RESET1, b.ch3.GARA, b.ch3.HUPA);
  /*p18.HUPA*/         c.ch3.HUPA = and(b.ch3.WAVE_CLKo, b.sys.CLK_ABxxEFxx1);

  /*p18.EFAR*/ c.ch3.WAVE_IDX0 = tock_pos( a.ch3.WAVE_CLK,   b.ch3.WAVE_CLK,  b.ch3.WAVE_RST, b.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0);
  /*p18.ERUS*/ c.ch3.WAVE_IDX1 = tock_pos(!a.ch3.WAVE_IDX0, !b.ch3.WAVE_IDX0, b.ch3.WAVE_RST, b.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1);
  /*p18.EFUZ*/ c.ch3.WAVE_IDX2 = tock_pos(!a.ch3.WAVE_IDX1, !b.ch3.WAVE_IDX1, b.ch3.WAVE_RST, b.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2);
  /*p18.EXEL*/ c.ch3.WAVE_IDX3 = tock_pos(!a.ch3.WAVE_IDX2, !b.ch3.WAVE_IDX2, b.ch3.WAVE_RST, b.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3);
  /*p18.EFAL*/ c.ch3.WAVE_IDX4 = tock_pos(!a.ch3.WAVE_IDX3, !b.ch3.WAVE_IDX3, b.ch3.WAVE_RST, b.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4);
  /*p18.DERO*/   c.ch3.WAVE_CLK  = not(b.ch3.WAVE_CLKn);
  /*p18.ETAN*/   c.ch3.WAVE_RST = or(b.ch3.GARA, b.ch3.WAVE_LOOP);
  /*p18.FETY*/     c.ch3.WAVE_LOOP = tock_pos(!a.ch3.WAVE_IDX4, !b.ch3.WAVE_IDX4, b.ch3.GYRY, b.ch3.WAVE_LOOP, !b.ch3.WAVE_LOOP);
  /*p18.GYRY*/       c.ch3.GYRY = nor(b.apu.APU_RESET1, b.ch3.GARA, b.ch3.FOTO);
  /*p18.FOTO*/         c.ch3.FOTO = and(b.ch3.WAVE_LOOP, b.ch3.WAVE_CLKn);




  //----------
  // Wave ram control signals

  /*p16.GYRA*/ c.ch3.GYRA = tock_pos(a.sys.CLK_ABxxEFxx1, b.sys.CLK_ABxxEFxx1, b.ch3.APU_RESETo, b.ch3.GYRA, b.ch3.GYTA);
  /*p16.GYTA*/ c.ch3.GYTA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  b.ch3.APU_RESETo, b.ch3.GYTA, b.ch3.GARA);
  /*p16.GARA*/ c.ch3.GARA = tock_pos(a.ch3.CLK_xxCDxxGH,  b.ch3.CLK_xxCDxxGH,  b.ch3.FURY,       b.ch3.GARA, b.ch3.GOFY);
  /*p16.FABO*/   c.ch3.CLK_xxCDxxGH = not(b.sys.CLK_ABxxEFxx1);
  /*p16.GAZE*/   c.ch3.APU_RESETo = not(b.apu.APU_RESET1);
  /*p16.GOFY*/   c.ch3.GOFY = or(b.ch3.GULO, b.ch3.CH3_RESTART_SYNC);
  /*p16.GULO*/     c.ch3.GULO = not(b.ch3.FURY);
  /*p16.FURY*/       c.ch3.FURY = nor(b.ch3.GYTA, b.apu.APU_RESET1);
  /*p16.FOBA*/     c.ch3.CH3_RESTART_SYNC = tock_pos(a.sys.CPUCLK_xxxxEFGH9, b.sys.CPUCLK_xxxxEFGH9, b.ch3.APU_RESETp, b.ch3.CH3_RESTART_SYNC, b.ch3.NR34_START);
  /*p16.GOMA*/       c.ch3.APU_RESETp = not(b.apu.APU_RESET1);


  /*p18.COKA*/ c.ch3.CH3_ACTIVE = not(!b.ch3.CH3_ACTIVEo);
  /*p18.DAVO*/   c.ch3.CH3_ACTIVEo = tock_pos(a.sys.AJER_2M, b.sys.AJER_2M, b.ch3.APU_RESETw, b.ch3.CH3_ACTIVEo, b.ch3.DORU);
  /*p18.CALU*/     c.ch3.APU_RESETw = not(b.apu.APU_RESET1);
  /*p18.DORU*/     c.ch3.DORU = not(b.ch3.EZAS);
  /*p18.EZAS*/       c.ch3.EZAS = not(b.ch3.FOZU);
  /*p18.FOZU*/         c.ch3.FOZU = or(b.ch3.GARA, b.ch3.FYGO);
  /*p18.FYGO*/           c.ch3.FYGO = or(b.apu.APU_RESET1, b.ch3.GEDO, b.ch3.CH3_AMP_ENna);
  /*p18.GEDO*/             c.ch3.GEDO = and(b.sys.CLK_256a, b.ch3.NR34_STOP);

  /*p17.AZET*/ c.ch3.AZET = tock_pos(a.ch3.ARUC, b.ch3.ARUC, b.ch3.APU_RESETu, b.ch3.AZET, b.ch3.SAMPLE_CLKn);
  /*p17.ARUC*/   c.ch3.ARUC = not(b.sys.CLK_xBxDxFxH1);
  /*p17.AZUS*/   c.ch3.SAMPLE_CLKn = tock_pos(a.sys.CLK_xBxDxFxH1, b.sys.CLK_xBxDxFxH1, b.ch3.APU_RESETu, b.ch3.SAMPLE_CLKn, b.ch3.BANO);
  /*p17.BANO*/     c.ch3.BANO = tock_pos(a.ch3.COZY, b.ch3.COZY, b.ch3.APU_RESETu, b.ch3.BANO, b.ch3.BUSA);
  /*p17.COZY*/       c.ch3.COZY = not(b.sys.CLK_xBxDxFxH1);
  /*p17.BUSA*/       c.ch3.BUSA = tock_pos(a.sys.CLK_xBxDxFxH1, b.sys.CLK_xBxDxFxH1, b.ch3.APU_RESETu, b.ch3.BUSA, b.ch3.WAVE_CLKn); // ?
  /*p17.BAMA*/         c.ch3.APU_RESETu = not(b.apu.APU_RESET1);

  /*p17.ALER*/ c.WAVE_RAM_CTRL1 = not(b.ch3.WAVE_RAM_CTRL1n);
  /*p17.ATUR*/   c.ch3.WAVE_RAM_CTRL1n = mux2(b.ch3.BORU, b.ch3.BUKU, b.ch3.CH3_ACTIVE);
  /*p17.BORU*/     c.ch3.BORU = not(b.ch3.BOXO);
  /*p17.BOXO*/       c.ch3.BOXO = nor(b.ch3.SAMPLE_CLKn, b.ch3.AZET);
  /*p17.BUKU*/     c.ch3.BUKU = not(b.ch3.AZOR);
  /*p17.AZOR*/       c.ch3.AZOR = not(b.ch3.BETA);
  /*p17.BETA*/         c.ch3.BETA = or(b.ch3.WAVE_WR, b.ch3.CPU_WAVE_RDn, b.ch3.BORY);
  /*p17.BYZA*/           c.ch3.WAVE_WR = and(b.apu.APU_WR, b.apu.ADDR_FF3X);
  /*p17.BORY*/           c.ch3.BORY = not(b.ch3.ABUR);
  /*p17.ABUR*/             c.ch3.ABUR = not(b.sys.CPUCLK_xxxxxFxx1);

  /*p17.AMYT*/ c.WAVE_WRn = not(b.ch3.WAVE_WR);

  // block cpu access to wave ram while ch3 playing?
  /*p18.ATOK*/ c.WAVE_RAM_CTRL3 = not(b.ch3.WAVE_RAM_CTRL3n);
  /*p18.BENO*/   c.ch3.WAVE_RAM_CTRL3n = mux2(b.ch3.SAMPLE_CLK, b.ch3.CPU_WAVE_RDn, b.ch3.CH3_ACTIVE);
  /*p17.BUTU*/     c.ch3.SAMPLE_CLK = not(b.ch3.SAMPLE_CLKn);
  /*p17.CAZU*/     c.ch3.CPU_WAVE_RDn = not(b.ch3.CPU_WAVE_RD);
  /*p17.BENA*/       c.ch3.CPU_WAVE_RD  = nand(b.ch3.CPU_RDa, b.apu.ADDR_FF3X);
  /*p17.BOKE*/         c.ch3.CPU_RDa = not(b.apu.CPU_RDn);











  //----------
  // Wave read

  /*p17.CUGO*/ c.ch3.WAVE_D0n = not(b.WAVE_D0);
  /*p17.CEGU*/ c.ch3.WAVE_D1n = not(b.WAVE_D1);
  /*p17.ATEC*/ c.ch3.WAVE_D2n = not(b.WAVE_D2);
  /*p17.ADOK*/ c.ch3.WAVE_D3n = not(b.WAVE_D3);
  /*p17.BACA*/ c.ch3.WAVE_D4n = not(b.WAVE_D4);
  /*p17.BERO*/ c.ch3.WAVE_D5n = not(b.WAVE_D5);
  /*p17.CUTO*/ c.ch3.WAVE_D6n = not(b.WAVE_D6);
  /*p17.AKAF*/ c.ch3.WAVE_D7n = not(b.WAVE_D7);

  // uhhhh polarity? this is probably wrong...
  /*p17.DUGU*/ if (b.ch3.CPU_WAVE_RDn) c.D7 = b.ch3.WAVE_D0n;
  /*p17.DESY*/ if (b.ch3.CPU_WAVE_RDn) c.D6 = b.ch3.WAVE_D1n;
  /*p17.BATY*/ if (b.ch3.CPU_WAVE_RDn) c.D5 = b.ch3.WAVE_D2n;
  /*p17.BADE*/ if (b.ch3.CPU_WAVE_RDn) c.D4 = b.ch3.WAVE_D3n;
  /*p17.BUNE*/ if (b.ch3.CPU_WAVE_RDn) c.D3 = b.ch3.WAVE_D4n;
  /*p17.BAVA*/ if (b.ch3.CPU_WAVE_RDn) c.D2 = b.ch3.WAVE_D5n;
  /*p17.DESA*/ if (b.ch3.CPU_WAVE_RDn) c.D1 = b.ch3.WAVE_D6n;
  /*p17.BEZU*/ if (b.ch3.CPU_WAVE_RDn) c.D0 = b.ch3.WAVE_D7n;

  /*p17.CYFO*/ c.ch3.SAMPLE_0 = tock_pos(a.ch3.SAMPLE_CLKa, b.ch3.SAMPLE_CLKa, b.ch3.APU_RESETv, b.ch3.SAMPLE_0, b.WAVE_D0);
  /*p17.CESY*/ c.ch3.SAMPLE_1 = tock_pos(a.ch3.SAMPLE_CLKd, b.ch3.SAMPLE_CLKd, b.ch3.APU_RESETv, b.ch3.SAMPLE_1, b.WAVE_D1);
  /*p17.BUDY*/ c.ch3.SAMPLE_2 = tock_pos(a.ch3.SAMPLE_CLKc, b.ch3.SAMPLE_CLKc, b.ch3.APU_RESETv, b.ch3.SAMPLE_2, b.WAVE_D2);
  /*p17.BEGU*/ c.ch3.SAMPLE_3 = tock_pos(a.ch3.SAMPLE_CLKb, b.ch3.SAMPLE_CLKb, b.ch3.APU_RESETv, b.ch3.SAMPLE_3, b.WAVE_D3);
  /*p17.CUVO*/ c.ch3.SAMPLE_4 = tock_pos(a.ch3.SAMPLE_CLKa, b.ch3.SAMPLE_CLKa, b.ch3.APU_RESETv, b.ch3.SAMPLE_4, b.WAVE_D4);
  /*p17.CEVO*/ c.ch3.SAMPLE_5 = tock_pos(a.ch3.SAMPLE_CLKd, b.ch3.SAMPLE_CLKd, b.ch3.APU_RESETv, b.ch3.SAMPLE_5, b.WAVE_D5);
  /*p17.BORA*/ c.ch3.SAMPLE_6 = tock_pos(a.ch3.SAMPLE_CLKc, b.ch3.SAMPLE_CLKc, b.ch3.APU_RESETv, b.ch3.SAMPLE_6, b.WAVE_D6);
  /*p17.BEPA*/ c.ch3.SAMPLE_7 = tock_pos(a.ch3.SAMPLE_CLKb, b.ch3.SAMPLE_CLKb, b.ch3.APU_RESETv, b.ch3.SAMPLE_7, b.WAVE_D7);
  /*p17.BEKA*/   c.ch3.SAMPLE_CLKa = not(b.ch3.SAMPLE_CLK);
  /*p17.BUFE*/   c.ch3.SAMPLE_CLKd = not(b.ch3.SAMPLE_CLK);
  /*p17.BAJA*/   c.ch3.SAMPLE_CLKc = not(b.ch3.SAMPLE_CLK);
  /*p17.COJU*/   c.ch3.SAMPLE_CLKb = not(b.ch3.SAMPLE_CLK);
  /*p17.ACOR*/   c.ch3.APU_RESETv = not(b.apu.APU_RESET1);

  // where's the write to the wave bus from the cpu bus?

  //----------
  // Output

  /*p17.DATE*/ c.ch3.WAVE_PLAY_D0 = mux2n(!b.ch3.SAMPLE_0, !b.ch3.SAMPLE_4, b.ch3.WAVE_IDX0);
  /*p17.DAZY*/ c.ch3.WAVE_PLAY_D1 = mux2n(!b.ch3.SAMPLE_1, !b.ch3.SAMPLE_5, b.ch3.WAVE_IDX0);
  /*p17.COPO*/ c.ch3.WAVE_PLAY_D3 = mux2n(!b.ch3.SAMPLE_2, !b.ch3.SAMPLE_6, b.ch3.WAVE_IDX0);
  /*p17.CUZO*/ c.ch3.WAVE_PLAY_D2 = mux2n(!b.ch3.SAMPLE_3, !b.ch3.SAMPLE_7, b.ch3.WAVE_IDX0);

  /*p18.GEMY*/ c.ch3.WAVE_VOL3 = nor(!b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);
  /*p18.GOKA*/ c.ch3.WAVE_VOL2 = nor(!b.ch3.NR32_VOL0,  b.ch3.NR32_VOL0);
  /*p18.GEGE*/ c.ch3.WAVE_VOL4 = nor( b.ch3.NR32_VOL0, !b.ch3.NR32_VOL0);

  /*p18.EZAG*/ c.ch3.WAVE_OUT0 = amux3(b.ch3.WAVE_PLAY_D0, b.ch3.WAVE_VOL4, b.ch3.WAVE_PLAY_D1, b.ch3.WAVE_VOL2, b.ch3.WAVE_PLAY_D2, b.ch3.WAVE_VOL3);
  /*p18.EVUG*/ c.ch3.WAVE_OUT1 = amux3(b.ch3.WAVE_PLAY_D1, b.ch3.WAVE_VOL4, b.ch3.WAVE_PLAY_D2, b.ch3.WAVE_VOL2, b.ch3.WAVE_PLAY_D3, b.ch3.WAVE_VOL3);
  /*p18.DOKY*/ c.ch3.WAVE_OUT2 = amux2(b.ch3.WAVE_VOL4, b.ch3.WAVE_PLAY_D2, b.ch3.WAVE_PLAY_D3, b.ch3.WAVE_VOL3);
  /*p18.DORE*/ c.ch3.WAVE_OUT3 = and(b.ch3.WAVE_PLAY_D3, b.ch3.WAVE_VOL4);

  /*p18.BARY*/ c.WAVE_DAC0 = and(b.ch3.CH3_ACTIVE, b.ch3.WAVE_OUT0);
  /*p18.BYKA*/ c.WAVE_DAC1 = and(b.ch3.CH3_ACTIVE, b.ch3.WAVE_OUT1);
  /*p18.BOPA*/ c.WAVE_DAC2 = and(b.ch3.CH3_ACTIVE, b.ch3.WAVE_OUT2);
  /*p18.BELY*/ c.WAVE_DAC3 = and(b.ch3.CH3_ACTIVE, b.ch3.WAVE_OUT3);

  /*p18.BOLE*/ c.WAVE_A0 = mux2(b.ch3.WAVE_IDX1, b.A00, b.ch3.CH3_ACTIVE);
  /*p18.AGYL*/ c.WAVE_A1 = mux2(b.ch3.WAVE_IDX2, b.A01, b.ch3.CH3_ACTIVE);
  /*p18.AFUM*/ c.WAVE_A2 = mux2(b.ch3.WAVE_IDX3, b.A02, b.ch3.CH3_ACTIVE);
  /*p18.AXOL*/ c.WAVE_A3 = mux2(b.ch3.WAVE_IDX4, b.A03, b.ch3.CH3_ACTIVE);

}
