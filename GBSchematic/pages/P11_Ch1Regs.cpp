#include "P11_Ch1Regs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P11_Ch1Regs_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // FF10 NR10

  /*p11.CENU*/ c.p11.CENU = and(b.apu.APU_WR, b.apu.ADDR_FF10);
  /*p11.BUZE*/ c.p11.BUZE = not(b.apu.ADDR_FF10);
  /*p11.ATYN*/ c.p11.ATYN = nor(b.apu.CPU_RDn, b.p11.BUZE);
  /*p11.ASOP*/ c.p11.ASOP = not(b.p11.ATYN);

  /*p11.BANY*/ c.p11.NR10_SWEEP_SHIFT_0 = tock_pos(a.p11.CENU, b.p11.CENU, b.apu.APU_RESETn, b.p11.NR10_SWEEP_SHIFT_0, b.D0);
  /*p11.ARAX*/ c.p11.NR10_SWEEP_SHIFT_1 = tock_pos(a.p11.CENU, b.p11.CENU, b.apu.APU_RESETn, b.p11.NR10_SWEEP_SHIFT_1, b.D1);
  /*p11.ANAZ*/ c.p11.NR10_SWEEP_SHIFT_2 = tock_pos(a.p11.CENU, b.p11.CENU, b.apu.APU_RESETn, b.p11.NR10_SWEEP_SHIFT_2, b.D2);
  /*p11.AVAF*/ c.p11.NR10_SWEEP_DIR     = tock_pos(a.p11.CENU, b.p11.CENU, b.apu.APU_RESETn, b.p11.NR10_SWEEP_DIR,     b.D3);
  /*p11.ADEK*/ c.p11.NR10_SWEEP_TIME_0  = tock_pos(a.p11.CENU, b.p11.CENU, b.apu.APU_RESETn, b.p11.NR10_SWEEP_TIME_0,  b.D4);
  /*p11.BANA*/ c.p11.NR10_SWEEP_TIME_1  = tock_pos(a.p11.CENU, b.p11.CENU, b.apu.APU_RESETn, b.p11.NR10_SWEEP_TIME_1,  b.D5);
  /*p11.BOTU*/ c.p11.NR10_SWEEP_TIME_2  = tock_pos(a.p11.CENU, b.p11.CENU, b.apu.APU_RESETn, b.p11.NR10_SWEEP_TIME_2,  b.D6);
  
  /*p11.AMYD*/ if (b.p11.ASOP) c.D0 = b.p11.NR10_SWEEP_SHIFT_0;
  /*p11.ATAX*/ if (b.p11.ASOP) c.D1 = b.p11.NR10_SWEEP_SHIFT_1;
  /*p11.AZYP*/ if (b.p11.ASOP) c.D2 = b.p11.NR10_SWEEP_SHIFT_2;
  /*p11.AFOX*/ if (b.p11.ASOP) c.D3 = b.p11.NR10_SWEEP_DIR;
  /*p11.AVEK*/ if (b.p11.ASOP) c.D4 = b.p11.NR10_SWEEP_TIME_0;
  /*p11.AKUX*/ if (b.p11.ASOP) c.D5 = b.p11.NR10_SWEEP_TIME_1;
  /*p11.AWOS*/ if (b.p11.ASOP) c.D6 = b.p11.NR10_SWEEP_TIME_2;

  //----------
  // FF11 NR11

  /*p11.COVU*/ c.p11.FF11_WR = and(b.apu.APU_WR, b.apu.ADDR_FF11);
  /*p11.DAFO*/ c.p11.FF11_WRn = not(b.p11.FF11_WR);
  /*p11.BUWA*/ c.p11.CPU_RDa = not(b.apu.CPU_RDn);
  /*p11.BEXU*/ c.p11.FF11_RD = nor(b.p11.CPU_RDa, b.apu.ADDR_FF11); // polarity?

  /*p11.CENA*/ c.p11.CH1_DUTY_0 = tock_pos(a.p11.FF11_WRn, b.p11.FF11_WRn, b.p11.CEPO, b.p11.CH1_DUTY_0, b.D6);
  /*p11.DYCA*/ c.p11.CH1_DUTY_1 = tock_pos(a.p11.FF11_WRn, b.p11.FF11_WRn, b.p11.CEPO, b.p11.CH1_DUTY_1, b.D7);

  /*p11.BOWO*/ if (b.p11.FF11_RD) c.D6 = b.p11.CH1_DUTY_0;
  /*p11.CUDA*/ if (b.p11.FF11_RD) c.D7 = b.p11.CH1_DUTY_1;

  //----------
  // FF12 NR12

  /*p11.KAGY*/ c.p11.KAGY = not(b.p11.GAXU); // unused?
  /*p11.HAFU*/ c.p11.HAFU = and(b.apu.APU_WR, b.apu.ADDR_FF12);
  /*p11.KYGY*/ c.p11.KYGY = not(b.p11.HAFU);
  /*p11.GAXU*/ c.p11.GAXU = nand(b.apu.APU_WR, b.apu.ADDR_FF12);
  /*p11.HAXE*/ c.p11.HAXE = not(b.apu.ADDR_FF12);
  /*p11.HAMY*/ c.p11.HAMY = or(b.p11.HAXE, b.apu.CPU_RDn);
  /*p11.GAGO*/ c.p11.GAGO = not(b.apu.ADDR_FF12);
  /*p11.HOCU*/ c.p11.HOCU = or(b.p11.GAGO, b.apu.CPU_RDn);

  /*p11.JUSA*/ c.p11.CH1_SWEEP_0 = tock_pos(a.p11.KYGY, b.p11.KYGY, b.p11.HATO, b.p11.CH1_SWEEP_0, b.D0);
  /*p11.JUZY*/ c.p11.CH1_SWEEP_1 = tock_pos(a.p11.KYGY, b.p11.KYGY, b.p11.HATO, b.p11.CH1_SWEEP_1, b.D1);
  /*p11.JOMA*/ c.p11.CH1_SWEEP_2 = tock_pos(a.p11.KYGY, b.p11.KYGY, b.p11.HATO, b.p11.CH1_SWEEP_2, b.D2);
  /*p11.JAFY*/ c.p11.CH1_ENV_DIR = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.CH1_ENV_DIR, b.D3);
  /*p11.JATY*/ c.p11.CH1_VOL_0   = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.CH1_VOL_0,   b.D4);
  /*p11.JAXO*/ c.p11.CH1_VOL_1   = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.CH1_VOL_1,   b.D5);
  /*p11.JENA*/ c.p11.CH1_VOL_2   = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.CH1_VOL_2,   b.D6);
  /*p11.JOPU*/ c.p11.CH1_VOL_3   = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.CH1_VOL_3,   b.D7);

  /*p11.JYNE*/ if (b.p11.HAMY) c.D0 = b.p11.CH1_SWEEP_0;
  /*p11.JACA*/ if (b.p11.HAMY) c.D1 = b.p11.CH1_SWEEP_1;
  /*p11.JOKU*/ if (b.p11.HAMY) c.D2 = b.p11.CH1_SWEEP_2;
  /*p11.HONO*/ if (b.p11.HOCU) c.D3 = b.p11.CH1_ENV_DIR;
  /*p11.HOWU*/ if (b.p11.HOCU) c.D4 = b.p11.CH1_VOL_0;
  /*p11.HEWA*/ if (b.p11.HOCU) c.D5 = b.p11.CH1_VOL_1;
  /*p11.HEVE*/ if (b.p11.HOCU) c.D6 = b.p11.CH1_VOL_2;
  /*p11.JYSE*/ if (b.p11.HOCU) c.D7 = b.p11.CH1_VOL_3;

  //----------
  // FF13/14 ch1 freq

  /*p11.CACA*/ c.p11.ADDR_FF13n = not(b.apu.ADDR_FF13);

  /*p11.CEGE*/ c.p11.CPU_RDb  = not(b.apu.CPU_RDn);
  /*p11.DAXA*/ c.p11.CPU_RDnb = nand(b.p11.CPU_RDb, b.apu.DBG_APU);
  /*p11.DYPU*/ c.p11.FF13_RDn = nor(b.p11.CPU_RDnb, b.p11.ADDR_FF13n);
  /*p11.EVAJ*/ c.p11.FF13_RDa = not(b.p11.FF13_RDn);

  /*p11.FULO*/ c.p11.CH1_FREQ_CLKn = nor(b.p01.DYFA_1M, b.p13.GEXU);
  /*p11.GEKU*/ c.p11.CH1_FREQ_CLKa = not(b.p11.CH1_FREQ_CLKn);
  /*p11.KYPE*/ c.p11.CH1_FREQ_CLKb = not(b.p11.CH1_FREQ_03);
  /*p11.DERU*/ c.p11.CH1_FREQ_CLKc = not(b.p11.CH1_FREQ_07);

  /*p11.EPYK*/ c.p11.CH1_FREQ_RSTn = nor(b.p13.FEKU, b.p13.CH1_PHASE_CLKna);
  /*p11.FUME*/ c.p11.CH1_FREQ_RSTa = not(b.p11.CH1_FREQ_RSTn);
  /*p11.DEGA*/ c.p11.CH1_FREQ_RSTb = not(b.p11.CH1_FREQ_RSTn);
  /*p11.DAKO*/ c.p11.CH1_FREQ_RSTc = not(b.p11.CH1_FREQ_RSTn);

  /*p11.GAXE*/ c.p11.CH1_FREQ_00 = count_pos(a.p11.CH1_FREQ_CLKa, b.p11.CH1_FREQ_CLKa, b.p11.CH1_FREQ_RSTa, b.p11.CH1_FREQ_00, b.p12.FREQ_SUM_OUT_00);
  /*p11.HYFE*/ c.p11.CH1_FREQ_01 = count_pos(a.p11.CH1_FREQ_00,   b.p11.CH1_FREQ_00,   b.p11.CH1_FREQ_RSTa, b.p11.CH1_FREQ_01, b.p12.FREQ_SUM_OUT_01);
  /*p11.JYTY*/ c.p11.CH1_FREQ_02 = count_pos(a.p11.CH1_FREQ_01,   b.p11.CH1_FREQ_01,   b.p11.CH1_FREQ_RSTa, b.p11.CH1_FREQ_02, b.p12.FREQ_SUM_OUT_02);
  /*p11.KYNA*/ c.p11.CH1_FREQ_03 = count_pos(a.p11.CH1_FREQ_02,   b.p11.CH1_FREQ_02,   b.p11.CH1_FREQ_RSTa, b.p11.CH1_FREQ_03, b.p12.FREQ_SUM_OUT_03);
  /*p11.JEMA*/ c.p11.CH1_FREQ_04 = count_pos(a.p11.CH1_FREQ_CLKb, b.p11.CH1_FREQ_CLKb, b.p11.CH1_FREQ_RSTb, b.p11.CH1_FREQ_04, b.p12.FREQ_SUM_OUT_04);
  /*p11.HYKE*/ c.p11.CH1_FREQ_05 = count_pos(a.p11.CH1_FREQ_04,   b.p11.CH1_FREQ_04,   b.p11.CH1_FREQ_RSTb, b.p11.CH1_FREQ_05, b.p12.FREQ_SUM_OUT_05);
  /*p11.FEVA*/ c.p11.CH1_FREQ_06 = count_pos(a.p11.CH1_FREQ_05,   b.p11.CH1_FREQ_05,   b.p11.CH1_FREQ_RSTb, b.p11.CH1_FREQ_06, b.p12.FREQ_SUM_OUT_06);
  /*p11.EKOV*/ c.p11.CH1_FREQ_07 = count_pos(a.p11.CH1_FREQ_06,   b.p11.CH1_FREQ_06,   b.p11.CH1_FREQ_RSTb, b.p11.CH1_FREQ_07, b.p12.FREQ_SUM_OUT_07);
  /*p11.EMUS*/ c.p11.CH1_FREQ_08 = count_pos(a.p11.CH1_FREQ_CLKc, b.p11.CH1_FREQ_CLKc, b.p11.CH1_FREQ_RSTc, b.p11.CH1_FREQ_08, b.p12.FREQ_SUM_OUT_08);
  /*p11.EVAK*/ c.p11.CH1_FREQ_09 = count_pos(a.p11.CH1_FREQ_08,   b.p11.CH1_FREQ_08,   b.p11.CH1_FREQ_RSTc, b.p11.CH1_FREQ_09, b.p12.FREQ_SUM_OUT_09);
  /*p11.COPU*/ c.p11.CH1_FREQ_10 = count_pos(a.p11.CH1_FREQ_09,   b.p11.CH1_FREQ_09,   b.p11.CH1_FREQ_RSTc, b.p11.CH1_FREQ_10, b.p12.FREQ_SUM_OUT_10);

  /*p11.FORU*/ if (b.p11.FF13_RDa) c.D0 = b.p11.CH1_FREQ_00;
  /*p11.GEFU*/ if (b.p11.FF13_RDa) c.D1 = b.p11.CH1_FREQ_01;
  /*p11.KYVU*/ if (b.p11.FF13_RDa) c.D2 = b.p11.CH1_FREQ_02;
  /*p11.KUMO*/ if (b.p11.FF13_RDa) c.D3 = b.p11.CH1_FREQ_03;
  /*p11.KARY*/ if (b.p11.FF13_RDa) c.D4 = b.p11.CH1_FREQ_04;
  /*p11.GODE*/ if (b.p11.FF13_RDa) c.D5 = b.p11.CH1_FREQ_05;
  /*p11.GOJE*/ if (b.p11.FF13_RDa) c.D6 = b.p11.CH1_FREQ_06;
  /*p11.FOZE*/ if (b.p11.FF13_RDa) c.D7 = b.p11.CH1_FREQ_07;
  /*p11.DOPA*/ if (b.p11.FF14_RDa) c.D0 = b.p11.CH1_FREQ_08;
  /*p11.DEMU*/ if (b.p11.FF14_RDa) c.D1 = b.p11.CH1_FREQ_09;
  /*p11.DEXO*/ if (b.p11.FF14_RDa) c.D2 = b.p11.CH1_FREQ_10;

  //----------
  // FF14 NR14

  /*p11.CURE*/ c.p11.ADDR_FF14n = not(b.apu.ADDR_FF14);
  /*p11.BAGE*/ c.p11.FF14_WRn   = nand(b.ch3.CPU_WR_WEIRD, b.apu.ADDR_FF14); 
  /*p11.BUDA*/ c.p11.CPU_RDna   = not(b.p07.CPU_RD); // not sure which CPU_RD signal this is
  /*p11.DUPY*/ c.p11.FF14_RDa   = or(b.p11.CPU_RDnb, b.p11.ADDR_FF14n); // polarity?
  /*p11.BALE*/ c.p11.FF14_RDb   = nand(b.p11.CPU_RDna, b.apu.ADDR_FF14);


  /*p11.BOKO*/ c.p11.NR14_STOP = tock_pos(a.p11.FF14_WRn, b.p11.FF14_WRn, b.p11.CAMY, c.p11.NR14_STOP, b.D6);

  /*p13.DOGE*/ c.p13.FF14_WRnb = nand(b.apu.APU_WR, b.apu.ADDR_FF14);

  /*p13.DADO*/ c.p13.NR14_START_RST = nor(b.apu.APU_RESET1, b.p13.CH1_RUNNING);
  /*p13.DUPE*/ c.p13.NR14_START = tock_pos(a.p13.FF14_WRnb, b.p13.FF14_WRnb, b.p13.NR14_START_RST, b.p13.NR14_START, b.D7);
  /*p13.EZEC*/ c.p13.CH1_RUNNING = tock_pos(a.p01.CPUCLK_xxxxEFGH9, b.p01.CPUCLK_xxxxEFGH9, b.p13.DUKA, b.p13.CH1_RUNNING, b.p13.NR14_START);


  /*p11.BYTU*/ if (b.p11.FF14_RDb) c.D6 = b.p11.NR14_STOP;
}
