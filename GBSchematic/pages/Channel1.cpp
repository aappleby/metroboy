#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P11_Ch1Regs_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p11.CEPO*/ c.ch1.CEPO = not(b.apu.APU_RESET1);
  /*p11.HATO*/ c.ch1.HATO = not(b.apu.APU_RESET1);
  /*p11.CAMY*/ c.ch1.CAMY = not(b.apu.APU_RESET1);
  /*p13.KADO*/ c.ch1.KADO = not(b.apu.APU_RESET1);
  /*p13.ERUM*/ c.ch1.ERUM = not(b.apu.APU_RESET1);
  /*p13.DUKA*/ c.ch1.DUKA = not(b.apu.APU_RESET1);

  //----------
  // FF10 NR10

  /*p11.CENU*/ c.ch1.CENU = and(b.apu.APU_WR, b.apu.ADDR_FF10);
  /*p11.BUZE*/ c.ch1.BUZE = not(b.apu.ADDR_FF10);
  /*p11.ATYN*/ c.ch1.ATYN = nor(b.apu.CPU_RDn, b.ch1.BUZE);
  /*p11.ASOP*/ c.ch1.ASOP = not(b.ch1.ATYN);

  /*p11.BANY*/ c.ch1.NR10_SWEEP_SHIFT_0 = tock_pos(a.ch1.CENU, b.ch1.CENU, b.apu.APU_RESETn, b.ch1.NR10_SWEEP_SHIFT_0, b.D0);
  /*p11.ARAX*/ c.ch1.NR10_SWEEP_SHIFT_1 = tock_pos(a.ch1.CENU, b.ch1.CENU, b.apu.APU_RESETn, b.ch1.NR10_SWEEP_SHIFT_1, b.D1);
  /*p11.ANAZ*/ c.ch1.NR10_SWEEP_SHIFT_2 = tock_pos(a.ch1.CENU, b.ch1.CENU, b.apu.APU_RESETn, b.ch1.NR10_SWEEP_SHIFT_2, b.D2);
  /*p11.AVAF*/ c.ch1.NR10_SWEEP_DIR     = tock_pos(a.ch1.CENU, b.ch1.CENU, b.apu.APU_RESETn, b.ch1.NR10_SWEEP_DIR,     b.D3);
  /*p11.ADEK*/ c.ch1.NR10_SWEEP_TIME_0  = tock_pos(a.ch1.CENU, b.ch1.CENU, b.apu.APU_RESETn, b.ch1.NR10_SWEEP_TIME_0,  b.D4);
  /*p11.BANA*/ c.ch1.NR10_SWEEP_TIME_1  = tock_pos(a.ch1.CENU, b.ch1.CENU, b.apu.APU_RESETn, b.ch1.NR10_SWEEP_TIME_1,  b.D5);
  /*p11.BOTU*/ c.ch1.NR10_SWEEP_TIME_2  = tock_pos(a.ch1.CENU, b.ch1.CENU, b.apu.APU_RESETn, b.ch1.NR10_SWEEP_TIME_2,  b.D6);

  /*p11.AMYD*/ if (b.ch1.ASOP) c.D0 = b.ch1.NR10_SWEEP_SHIFT_0;
  /*p11.ATAX*/ if (b.ch1.ASOP) c.D1 = b.ch1.NR10_SWEEP_SHIFT_1;
  /*p11.AZYP*/ if (b.ch1.ASOP) c.D2 = b.ch1.NR10_SWEEP_SHIFT_2;
  /*p11.AFOX*/ if (b.ch1.ASOP) c.D3 = b.ch1.NR10_SWEEP_DIR;
  /*p11.AVEK*/ if (b.ch1.ASOP) c.D4 = b.ch1.NR10_SWEEP_TIME_0;
  /*p11.AKUX*/ if (b.ch1.ASOP) c.D5 = b.ch1.NR10_SWEEP_TIME_1;
  /*p11.AWOS*/ if (b.ch1.ASOP) c.D6 = b.ch1.NR10_SWEEP_TIME_2;

  //----------
  // FF11 NR11

  /*p11.COVU*/ c.ch1.FF11_WR = and(b.apu.APU_WR, b.apu.ADDR_FF11);
  /*p11.DAFO*/ c.ch1.FF11_WRn = not(b.ch1.FF11_WR);
  /*p11.BUWA*/ c.ch1.CPU_RDa = not(b.apu.CPU_RDn);
  /*p11.BEXU*/ c.ch1.FF11_RD = nor(b.ch1.CPU_RDa, b.apu.ADDR_FF11); // polarity?

  /*p11.CENA*/ c.ch1.CH1_DUTY_0 = tock_pos(a.ch1.FF11_WRn, b.ch1.FF11_WRn, b.ch1.CEPO, b.ch1.CH1_DUTY_0, b.D6);
  /*p11.DYCA*/ c.ch1.CH1_DUTY_1 = tock_pos(a.ch1.FF11_WRn, b.ch1.FF11_WRn, b.ch1.CEPO, b.ch1.CH1_DUTY_1, b.D7);

  /*p11.BOWO*/ if (b.ch1.FF11_RD) c.D6 = b.ch1.CH1_DUTY_0;
  /*p11.CUDA*/ if (b.ch1.FF11_RD) c.D7 = b.ch1.CH1_DUTY_1;

  //----------
  // FF12 NR12

  /*p11.KAGY*/ c.ch1.KAGY = not(b.ch1.GAXU); // unused?
  /*p11.HAFU*/ c.ch1.HAFU = and(b.apu.APU_WR, b.apu.ADDR_FF12);
  /*p11.KYGY*/ c.ch1.KYGY = not(b.ch1.HAFU);
  /*p11.GAXU*/ c.ch1.GAXU = nand(b.apu.APU_WR, b.apu.ADDR_FF12);
  /*p11.HAXE*/ c.ch1.HAXE = not(b.apu.ADDR_FF12);
  /*p11.HAMY*/ c.ch1.HAMY = or(b.ch1.HAXE, b.apu.CPU_RDn);
  /*p11.GAGO*/ c.ch1.GAGO = not(b.apu.ADDR_FF12);
  /*p11.HOCU*/ c.ch1.HOCU = or(b.ch1.GAGO, b.apu.CPU_RDn);

  /*p11.JUSA*/ c.ch1.CH1_SWEEP_0 = tock_pos(a.ch1.KYGY, b.ch1.KYGY, b.ch1.HATO, b.ch1.CH1_SWEEP_0, b.D0);
  /*p11.JUZY*/ c.ch1.CH1_SWEEP_1 = tock_pos(a.ch1.KYGY, b.ch1.KYGY, b.ch1.HATO, b.ch1.CH1_SWEEP_1, b.D1);
  /*p11.JOMA*/ c.ch1.CH1_SWEEP_2 = tock_pos(a.ch1.KYGY, b.ch1.KYGY, b.ch1.HATO, b.ch1.CH1_SWEEP_2, b.D2);
  /*p11.JAFY*/ c.ch1.CH1_ENV_DIR = tock_pos(a.ch1.GAXU, b.ch1.GAXU, b.ch1.HATO, b.ch1.CH1_ENV_DIR, b.D3);
  /*p11.JATY*/ c.ch1.CH1_VOL_0   = tock_pos(a.ch1.GAXU, b.ch1.GAXU, b.ch1.HATO, b.ch1.CH1_VOL_0,   b.D4);
  /*p11.JAXO*/ c.ch1.CH1_VOL_1   = tock_pos(a.ch1.GAXU, b.ch1.GAXU, b.ch1.HATO, b.ch1.CH1_VOL_1,   b.D5);
  /*p11.JENA*/ c.ch1.CH1_VOL_2   = tock_pos(a.ch1.GAXU, b.ch1.GAXU, b.ch1.HATO, b.ch1.CH1_VOL_2,   b.D6);
  /*p11.JOPU*/ c.ch1.CH1_VOL_3   = tock_pos(a.ch1.GAXU, b.ch1.GAXU, b.ch1.HATO, b.ch1.CH1_VOL_3,   b.D7);

  /*p11.JYNE*/ if (b.ch1.HAMY) c.D0 = b.ch1.CH1_SWEEP_0;
  /*p11.JACA*/ if (b.ch1.HAMY) c.D1 = b.ch1.CH1_SWEEP_1;
  /*p11.JOKU*/ if (b.ch1.HAMY) c.D2 = b.ch1.CH1_SWEEP_2;
  /*p11.HONO*/ if (b.ch1.HOCU) c.D3 = b.ch1.CH1_ENV_DIR;
  /*p11.HOWU*/ if (b.ch1.HOCU) c.D4 = b.ch1.CH1_VOL_0;
  /*p11.HEWA*/ if (b.ch1.HOCU) c.D5 = b.ch1.CH1_VOL_1;
  /*p11.HEVE*/ if (b.ch1.HOCU) c.D6 = b.ch1.CH1_VOL_2;
  /*p11.JYSE*/ if (b.ch1.HOCU) c.D7 = b.ch1.CH1_VOL_3;

  //----------
  // FF13/14 ch1 freq

  /*p11.CACA*/ c.ch1.ADDR_FF13n = not(b.apu.ADDR_FF13);

  /*p11.CEGE*/ c.ch1.CPU_RDb  = not(b.apu.CPU_RDn);
  /*p11.DAXA*/ c.ch1.CPU_RDnb = nand(b.ch1.CPU_RDb, b.apu.DBG_APU);
  /*p11.DYPU*/ c.ch1.FF13_RDn = nor(b.ch1.CPU_RDnb, b.ch1.ADDR_FF13n);
  /*p11.EVAJ*/ c.ch1.FF13_RDa = not(b.ch1.FF13_RDn);

  /*p11.FULO*/ c.ch1.CH1_FREQ_CLKn = nor(b.sys.DYFA_1M, b.ch1.GEXU);
  /*p11.GEKU*/ c.ch1.CH1_FREQ_CLKa = not(b.ch1.CH1_FREQ_CLKn);
  /*p11.KYPE*/ c.ch1.CH1_FREQ_CLKb = not(b.ch1.CH1_FREQ_03);
  /*p11.DERU*/ c.ch1.CH1_FREQ_CLKc = not(b.ch1.CH1_FREQ_07);

  /*p11.EPYK*/ c.ch1.CH1_FREQ_RSTn = nor(b.ch1.FEKU, b.ch1.CH1_PHASE_CLKna);
  /*p11.FUME*/ c.ch1.CH1_FREQ_RSTa = not(b.ch1.CH1_FREQ_RSTn);
  /*p11.DEGA*/ c.ch1.CH1_FREQ_RSTb = not(b.ch1.CH1_FREQ_RSTn);
  /*p11.DAKO*/ c.ch1.CH1_FREQ_RSTc = not(b.ch1.CH1_FREQ_RSTn);

  /*p11.GAXE*/ c.ch1.CH1_FREQ_00 = count_pos(a.ch1.CH1_FREQ_CLKa, b.ch1.CH1_FREQ_CLKa, b.ch1.CH1_FREQ_RSTa, b.ch1.CH1_FREQ_00, b.ch1.FREQ_SUM_OUT_00);
  /*p11.HYFE*/ c.ch1.CH1_FREQ_01 = count_pos(a.ch1.CH1_FREQ_00,   b.ch1.CH1_FREQ_00,   b.ch1.CH1_FREQ_RSTa, b.ch1.CH1_FREQ_01, b.ch1.FREQ_SUM_OUT_01);
  /*p11.JYTY*/ c.ch1.CH1_FREQ_02 = count_pos(a.ch1.CH1_FREQ_01,   b.ch1.CH1_FREQ_01,   b.ch1.CH1_FREQ_RSTa, b.ch1.CH1_FREQ_02, b.ch1.FREQ_SUM_OUT_02);
  /*p11.KYNA*/ c.ch1.CH1_FREQ_03 = count_pos(a.ch1.CH1_FREQ_02,   b.ch1.CH1_FREQ_02,   b.ch1.CH1_FREQ_RSTa, b.ch1.CH1_FREQ_03, b.ch1.FREQ_SUM_OUT_03);
  /*p11.JEMA*/ c.ch1.CH1_FREQ_04 = count_pos(a.ch1.CH1_FREQ_CLKb, b.ch1.CH1_FREQ_CLKb, b.ch1.CH1_FREQ_RSTb, b.ch1.CH1_FREQ_04, b.ch1.FREQ_SUM_OUT_04);
  /*p11.HYKE*/ c.ch1.CH1_FREQ_05 = count_pos(a.ch1.CH1_FREQ_04,   b.ch1.CH1_FREQ_04,   b.ch1.CH1_FREQ_RSTb, b.ch1.CH1_FREQ_05, b.ch1.FREQ_SUM_OUT_05);
  /*p11.FEVA*/ c.ch1.CH1_FREQ_06 = count_pos(a.ch1.CH1_FREQ_05,   b.ch1.CH1_FREQ_05,   b.ch1.CH1_FREQ_RSTb, b.ch1.CH1_FREQ_06, b.ch1.FREQ_SUM_OUT_06);
  /*p11.EKOV*/ c.ch1.CH1_FREQ_07 = count_pos(a.ch1.CH1_FREQ_06,   b.ch1.CH1_FREQ_06,   b.ch1.CH1_FREQ_RSTb, b.ch1.CH1_FREQ_07, b.ch1.FREQ_SUM_OUT_07);
  /*p11.EMUS*/ c.ch1.CH1_FREQ_08 = count_pos(a.ch1.CH1_FREQ_CLKc, b.ch1.CH1_FREQ_CLKc, b.ch1.CH1_FREQ_RSTc, b.ch1.CH1_FREQ_08, b.ch1.FREQ_SUM_OUT_08);
  /*p11.EVAK*/ c.ch1.CH1_FREQ_09 = count_pos(a.ch1.CH1_FREQ_08,   b.ch1.CH1_FREQ_08,   b.ch1.CH1_FREQ_RSTc, b.ch1.CH1_FREQ_09, b.ch1.FREQ_SUM_OUT_09);
  /*p11.COPU*/ c.ch1.CH1_FREQ_10 = count_pos(a.ch1.CH1_FREQ_09,   b.ch1.CH1_FREQ_09,   b.ch1.CH1_FREQ_RSTc, b.ch1.CH1_FREQ_10, b.ch1.FREQ_SUM_OUT_10);

  /*p11.FORU*/ if (b.ch1.FF13_RDa) c.D0 = b.ch1.CH1_FREQ_00;
  /*p11.GEFU*/ if (b.ch1.FF13_RDa) c.D1 = b.ch1.CH1_FREQ_01;
  /*p11.KYVU*/ if (b.ch1.FF13_RDa) c.D2 = b.ch1.CH1_FREQ_02;
  /*p11.KUMO*/ if (b.ch1.FF13_RDa) c.D3 = b.ch1.CH1_FREQ_03;
  /*p11.KARY*/ if (b.ch1.FF13_RDa) c.D4 = b.ch1.CH1_FREQ_04;
  /*p11.GODE*/ if (b.ch1.FF13_RDa) c.D5 = b.ch1.CH1_FREQ_05;
  /*p11.GOJE*/ if (b.ch1.FF13_RDa) c.D6 = b.ch1.CH1_FREQ_06;
  /*p11.FOZE*/ if (b.ch1.FF13_RDa) c.D7 = b.ch1.CH1_FREQ_07;
  /*p11.DOPA*/ if (b.ch1.FF14_RDa) c.D0 = b.ch1.CH1_FREQ_08;
  /*p11.DEMU*/ if (b.ch1.FF14_RDa) c.D1 = b.ch1.CH1_FREQ_09;
  /*p11.DEXO*/ if (b.ch1.FF14_RDa) c.D2 = b.ch1.CH1_FREQ_10;

  //----------
  // FF14 NR14

  /*p11.CURE*/ c.ch1.ADDR_FF14n = not(b.apu.ADDR_FF14);
  /*p11.BAGE*/ c.ch1.FF14_WRn   = nand(b.apu.CPU_WR_WEIRD, b.apu.ADDR_FF14); 
  /*p11.BUDA*/ c.ch1.CPU_RDna   = not(b.sys.CPU_RD); // not sure which CPU_RD signal this is
  /*p11.DUPY*/ c.ch1.FF14_RDa   = or(b.ch1.CPU_RDnb, b.ch1.ADDR_FF14n); // polarity?
  /*p11.BALE*/ c.ch1.FF14_RDb   = nand(b.ch1.CPU_RDna, b.apu.ADDR_FF14);


  /*p11.BOKO*/ c.ch1.NR14_STOP = tock_pos(a.ch1.FF14_WRn, b.ch1.FF14_WRn, b.ch1.CAMY, c.ch1.NR14_STOP, b.D6);

  /*p13.DOGE*/ c.ch1.FF14_WRnb = nand(b.apu.APU_WR, b.apu.ADDR_FF14);

  /*p13.DADO*/ c.ch1.NR14_START_RST = nor(b.apu.APU_RESET1, b.ch1.CH1_RUNNING);
  /*p13.DUPE*/ c.ch1.NR14_START = tock_pos(a.ch1.FF14_WRnb, b.ch1.FF14_WRnb, b.ch1.NR14_START_RST, b.ch1.NR14_START, b.D7);
  /*p13.EZEC*/ c.ch1.CH1_RUNNING = tock_pos(a.sys.CPUCLK_xxxxEFGH9, b.sys.CPUCLK_xxxxEFGH9, b.ch1.DUKA, b.ch1.CH1_RUNNING, b.ch1.NR14_START);


  /*p11.BYTU*/ if (b.ch1.FF14_RDb) c.D6 = b.ch1.NR14_STOP;

  //----------

  /*p12.ARYL*/ c.ch1.NR10_SWEEP_DIRn = not(b.ch1.NR10_SWEEP_DIR);
  /*p12.KEDO*/ c.ch1.KEDO = not(b.ch1.KALA);
  /*p12.JUJU*/ c.ch1.JUJU = not(b.ch1.KALA);
  /*p12.KAPE*/ c.ch1.KAPE = not(b.ch1.KALA);
  /*p12.FAJA*/ c.ch1.FAJA = not(b.ch1.EGOR);
  /*p12.CYBE*/ c.ch1.CYBE = not(b.ch1.EJYB);
  /*p12.BECY*/ c.ch1.BECY = not(b.ch1.CYBE);
  /*p12.EJYB*/ c.ch1.EJYB = not(b.ch1.FAJA);

  /*p12.DEPU*/ c.ch1.FREQ_WRa = nand(b.apu.APU_WR, b.apu.ADDR_FF13); // polarity?
  /*p12.DEBY*/ c.ch1.FREQ_WRb = and(b.apu.APU_WR, b.apu.ADDR_FF14); // polarity?
  /*p12.DYLA*/ c.ch1.FREQ_WRc = not(b.ch1.FREQ_WRa);

  // axan.set
  /*p12.AFEG*/ c.ch1.FREQ_SUM_SET_10 = nand(b.ch1.FREQ_WRb, b.D2);
  /*p12.BUDO*/ c.ch1.FREQ_SUM_SET_09 = nand(b.ch1.FREQ_WRb, b.D1);
  /*p12.BUGU*/ c.ch1.FREQ_SUM_SET_08 = nand(b.ch1.FREQ_WRb, b.D0);
  /*p12.ETOL*/ c.ch1.FREQ_SUM_SET_07 = nand(b.ch1.FREQ_WRc, b.D7);
  /*p12.ELER*/ c.ch1.FREQ_SUM_SET_06 = nand(b.ch1.FREQ_WRc, b.D6);
  /*p12.KYPA*/ c.ch1.FREQ_SUM_SET_05 = nand(b.ch1.FREQ_WRc, b.D5);
  /*p12.KOVU*/ c.ch1.FREQ_SUM_SET_04 = nand(b.ch1.FREQ_WRc, b.D4);
  /*p12.GOPE*/ c.ch1.FREQ_SUM_SET_03 = nand(b.ch1.FREQ_WRc, b.D3);
  /*p12.GOLO*/ c.ch1.FREQ_SUM_SET_02 = nand(b.ch1.FREQ_WRc, b.D2);
  /*p12.GETA*/ c.ch1.FREQ_SUM_SET_01 = nand(b.ch1.FREQ_WRc, b.D1);
  /*p12.GYLU*/ c.ch1.FREQ_SUM_SET_00 = nand(b.ch1.FREQ_WRc, b.D0);

  /*p12.BYFU*/ c.ch1.FREQ_D10n = not(b.D2);
  /*p12.BOFU*/ c.ch1.FREQ_D09n = not(b.D1);
  /*p12.BYSU*/ c.ch1.FREQ_D08n = not(b.D0);
  /*p12.DULO*/ c.ch1.FREQ_D07n = not(b.D7);
  /*p12.DYLU*/ c.ch1.FREQ_D06n = not(b.D6);
  /*p12.JULO*/ c.ch1.FREQ_D05n = not(b.D5);
  /*p12.KOPU*/ c.ch1.FREQ_D04n = not(b.D4);
  /*p12.ETUV*/ c.ch1.FREQ_D03n = not(b.D3);
  /*p12.FULE*/ c.ch1.FREQ_D02n = not(b.D2);
  /*p12.GULU*/ c.ch1.FREQ_D01n = not(b.D1);
  /*p12.DEKE*/ c.ch1.FREQ_D00n = not(b.D0);

  /*p12.AJUX*/ c.ch1.AJUX = and(b.ch1.FREQ_WRb, b.ch1.FREQ_D10n);
  /*p12.AMAC*/ c.ch1.AMAC = and(b.ch1.FREQ_WRb, b.ch1.FREQ_D09n);
  /*p12.BASO*/ c.ch1.BASO = and(b.ch1.FREQ_WRb, b.ch1.FREQ_D08n);
  /*p12.EMAR*/ c.ch1.EMAR = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D07n);
  /*p12.ETOK*/ c.ch1.ETOK = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D06n);
  /*p12.KYFU*/ c.ch1.KYFU = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D05n);
  /*p12.KAVO*/ c.ch1.KAVO = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D04n);
  /*p12.FEGA*/ c.ch1.FEGA = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D03n);
  /*p12.FOKE*/ c.ch1.FOKE = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D02n);
  /*p12.FOPU*/ c.ch1.FOPU = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D01n);
  /*p12.EJYF*/ c.ch1.EJYF = and(b.ch1.FREQ_WRc, b.ch1.FREQ_D00n);

  /*p12.APAJ*/ c.ch1.FREQ_SUM_RST_10 = nor(b.ch1.AJUX, b.apu.APU_RESET1);
  /*p12.BOVU*/ c.ch1.FREQ_SUM_RST_09 = nor(b.ch1.AMAC, b.apu.APU_RESET1);
  /*p12.BOXU*/ c.ch1.FREQ_SUM_RST_08 = nor(b.ch1.BASO, b.apu.APU_RESET1);
  /*p12.ESEL*/ c.ch1.FREQ_SUM_RST_07 = nor(b.ch1.EMAR, b.apu.APU_RESET1);
  /*p12.ELUF*/ c.ch1.FREQ_SUM_RST_06 = nor(b.ch1.ETOK, b.apu.APU_RESET1);
  /*p12.KAJU*/ c.ch1.FREQ_SUM_RST_05 = nor(b.ch1.KYFU, b.apu.APU_RESET1);
  /*p12.KAPO*/ c.ch1.FREQ_SUM_RST_04 = nor(b.ch1.KAVO, b.apu.APU_RESET1);
  /*p12.GAMO*/ c.ch1.FREQ_SUM_RST_03 = nor(b.ch1.FEGA, b.apu.APU_RESET1);
  /*p12.GYFU*/ c.ch1.FREQ_SUM_RST_02 = nor(b.ch1.FOKE, b.apu.APU_RESET1);
  /*p12.GATO*/ c.ch1.FREQ_SUM_RST_01 = nor(b.ch1.FOPU, b.apu.APU_RESET1);
  /*p12.EFOR*/ c.ch1.FREQ_SUM_RST_00 = nor(b.ch1.EJYF, b.apu.APU_RESET1);

  /*p12.DOLY*/ c.ch1.FREQ_SUM_L_10 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_10, b.ch1.FREQ_SUM_OUT_10);
  /*p12.DOFY*/ c.ch1.FREQ_SUM_L_09 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_09, b.ch1.FREQ_SUM_OUT_09);
  /*p12.DEXE*/ c.ch1.FREQ_SUM_L_08 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_08, b.ch1.FREQ_SUM_OUT_08);
  /*p12.DELE*/ c.ch1.FREQ_SUM_L_07 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_07, b.ch1.FREQ_SUM_OUT_07);
  /*p12.EXAP*/ c.ch1.FREQ_SUM_L_06 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_06, b.ch1.FREQ_SUM_OUT_06);
  /*p12.FAXO*/ c.ch1.FREQ_SUM_L_05 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_05, b.ch1.FREQ_SUM_OUT_05);
  /*p12.GYME*/ c.ch1.FREQ_SUM_L_04 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_04, b.ch1.FREQ_SUM_OUT_04);
  /*p12.JYME*/ c.ch1.FREQ_SUM_L_03 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_03, b.ch1.FREQ_SUM_OUT_03);
  /*p12.KARE*/ c.ch1.FREQ_SUM_L_02 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_02, b.ch1.FREQ_SUM_OUT_02);
  /*p12.JODE*/ c.ch1.FREQ_SUM_L_01 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_01, b.ch1.FREQ_SUM_OUT_01);
  /*p12.GALO*/ c.ch1.FREQ_SUM_L_00 = tock_pos(a.ch1.ADAD, a.ch1.ADAD, a.ch1.KYLY, b.ch1.FREQ_SUM_L_00, b.ch1.FREQ_SUM_OUT_00);

  /*p12.DEVA*/ c.ch1.FREQ_SUM_R_10 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_10, b.ch1.FREQ_SHIFT_FLIP_10);
  /*p12.ETER*/ c.ch1.FREQ_SUM_R_09 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_09, b.ch1.FREQ_SHIFT_FLIP_09);
  /*p12.DEFA*/ c.ch1.FREQ_SUM_R_08 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_08, b.ch1.FREQ_SHIFT_FLIP_08);
  /*p12.EDOK*/ c.ch1.FREQ_SUM_R_07 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_07, b.ch1.FREQ_SHIFT_FLIP_07);
  /*p12.EPYR*/ c.ch1.FREQ_SUM_R_06 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_06, b.ch1.FREQ_SHIFT_FLIP_06);
  /*p12.GELE*/ c.ch1.FREQ_SUM_R_05 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_05, b.ch1.FREQ_SHIFT_FLIP_05);
  /*p12.JETE*/ c.ch1.FREQ_SUM_R_04 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_04, b.ch1.FREQ_SHIFT_FLIP_04);
  /*p12.JAPE*/ c.ch1.FREQ_SUM_R_03 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_03, b.ch1.FREQ_SHIFT_FLIP_03);
  /*p12.HELE*/ c.ch1.FREQ_SUM_R_02 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_02, b.ch1.FREQ_SHIFT_FLIP_02);
  /*p12.HOPA*/ c.ch1.FREQ_SUM_R_01 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_01, b.ch1.FREQ_SHIFT_FLIP_01);
  /*p12.HORA*/ c.ch1.FREQ_SUM_R_00 = tock_pos(b.ch1.ADAD, b.ch1.ADAD, b.ch1.KYLY, c.ch1.FREQ_SUM_R_00, b.ch1.FREQ_SHIFT_FLIP_00);

  /*p12.GUXA*/ wire FREQ_SUM_C_00 = add_c(!b.ch1.FREQ_SUM_L_00, !b.ch1.FREQ_SUM_R_00, b.ch1.NR10_SWEEP_DIRn);
  /*p12.HALU*/ wire FREQ_SUM_C_01 = add_c(!b.ch1.FREQ_SUM_L_01, !b.ch1.FREQ_SUM_R_01, FREQ_SUM_C_00);
  /*p12.JULE*/ wire FREQ_SUM_C_02 = add_c(!b.ch1.FREQ_SUM_L_02, !b.ch1.FREQ_SUM_R_02, FREQ_SUM_C_01);
  /*p12.JORY*/ wire FREQ_SUM_C_03 = add_c(!b.ch1.FREQ_SUM_L_03, !b.ch1.FREQ_SUM_R_03, FREQ_SUM_C_02);
  /*p12.HEXO*/ wire FREQ_SUM_C_04 = add_c(!b.ch1.FREQ_SUM_L_04, !b.ch1.FREQ_SUM_R_04, FREQ_SUM_C_03);
  /*p12.GEVA*/ wire FREQ_SUM_C_05 = add_c(!b.ch1.FREQ_SUM_L_05, !b.ch1.FREQ_SUM_R_05, FREQ_SUM_C_04);
  /*p12.FEGO*/ wire FREQ_SUM_C_06 = add_c(!b.ch1.FREQ_SUM_L_06, !b.ch1.FREQ_SUM_R_06, FREQ_SUM_C_05);
  /*p12.ETEK*/ wire FREQ_SUM_C_07 = add_c(!b.ch1.FREQ_SUM_L_07, !b.ch1.FREQ_SUM_R_07, FREQ_SUM_C_06);
  /*p12.DYXE*/ wire FREQ_SUM_C_08 = add_c(!b.ch1.FREQ_SUM_L_08, !b.ch1.FREQ_SUM_R_08, FREQ_SUM_C_07);
  /*p12.DULE*/ wire FREQ_SUM_C_09 = add_c(!b.ch1.FREQ_SUM_L_09, !b.ch1.FREQ_SUM_R_09, FREQ_SUM_C_08);
  /*p12.CORU*/ wire FREQ_SUM_C_10 = add_c(!b.ch1.FREQ_SUM_L_10, !b.ch1.FREQ_SUM_R_10, FREQ_SUM_C_09);

  /*p12.GUXA*/ wire FREQ_SUM_S_00 = add_s(!b.ch1.FREQ_SUM_L_00, !b.ch1.FREQ_SUM_R_00, b.ch1.NR10_SWEEP_DIRn);
  /*p12.HALU*/ wire FREQ_SUM_S_01 = add_s(!b.ch1.FREQ_SUM_L_01, !b.ch1.FREQ_SUM_R_01, FREQ_SUM_C_00);
  /*p12.JULE*/ wire FREQ_SUM_S_02 = add_s(!b.ch1.FREQ_SUM_L_02, !b.ch1.FREQ_SUM_R_02, FREQ_SUM_C_01);
  /*p12.JORY*/ wire FREQ_SUM_S_03 = add_s(!b.ch1.FREQ_SUM_L_03, !b.ch1.FREQ_SUM_R_03, FREQ_SUM_C_02);
  /*p12.HEXO*/ wire FREQ_SUM_S_04 = add_s(!b.ch1.FREQ_SUM_L_04, !b.ch1.FREQ_SUM_R_04, FREQ_SUM_C_03);
  /*p12.GEVA*/ wire FREQ_SUM_S_05 = add_s(!b.ch1.FREQ_SUM_L_05, !b.ch1.FREQ_SUM_R_05, FREQ_SUM_C_04);
  /*p12.FEGO*/ wire FREQ_SUM_S_06 = add_s(!b.ch1.FREQ_SUM_L_06, !b.ch1.FREQ_SUM_R_06, FREQ_SUM_C_05);
  /*p12.ETEK*/ wire FREQ_SUM_S_07 = add_s(!b.ch1.FREQ_SUM_L_07, !b.ch1.FREQ_SUM_R_07, FREQ_SUM_C_06);
  /*p12.DYXE*/ wire FREQ_SUM_S_08 = add_s(!b.ch1.FREQ_SUM_L_08, !b.ch1.FREQ_SUM_R_08, FREQ_SUM_C_07);
  /*p12.DULE*/ wire FREQ_SUM_S_09 = add_s(!b.ch1.FREQ_SUM_L_09, !b.ch1.FREQ_SUM_R_09, FREQ_SUM_C_08);
  /*p12.CORU*/ wire FREQ_SUM_S_10 = add_s(!b.ch1.FREQ_SUM_L_10, !b.ch1.FREQ_SUM_R_10, FREQ_SUM_C_09);

  // this is overflow check
  // the logic here seems wrong, like carry should be negated or something
  /*p12.BYLE*/ c.ch1.CH1_SWEEP_MAX = nor(b.ch1.NR10_SWEEP_DIRn, FREQ_SUM_C_10);
  /*p12.ATYS*/ c.ch1.CH1_SWEEP_STOPn = or(b.ch1.CH1_SWEEP_MAX, b.ch1.NR10_SWEEP_DIRn);

  // ->P11
  /*p12.AXAN*/ c.ch1.FREQ_SUM_OUT_10 = srtock_pos(a.ch1.BUSO, a.ch1.BUSO, b.ch1.FREQ_SUM_SET_10, b.ch1.FREQ_SUM_RST_10, c.ch1.FREQ_SUM_OUT_10, FREQ_SUM_S_10);
  /*p12.EVAB*/ c.ch1.FREQ_SUM_OUT_09 = srtock_pos(a.ch1.BUSO, a.ch1.BUSO, b.ch1.FREQ_SUM_SET_09, b.ch1.FREQ_SUM_RST_09, c.ch1.FREQ_SUM_OUT_09, FREQ_SUM_S_09);
  /*p12.DYGY*/ c.ch1.FREQ_SUM_OUT_08 = srtock_pos(a.ch1.BUSO, a.ch1.BUSO, b.ch1.FREQ_SUM_SET_08, b.ch1.FREQ_SUM_RST_08, c.ch1.FREQ_SUM_OUT_08, FREQ_SUM_S_08);
  /*p12.HOPO*/ c.ch1.FREQ_SUM_OUT_07 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_07, b.ch1.FREQ_SUM_RST_07, c.ch1.FREQ_SUM_OUT_07, FREQ_SUM_S_07);
  /*p12.HYXU*/ c.ch1.FREQ_SUM_OUT_06 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_06, b.ch1.FREQ_SUM_RST_06, c.ch1.FREQ_SUM_OUT_06, FREQ_SUM_S_06);
  /*p12.HOLU*/ c.ch1.FREQ_SUM_OUT_05 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_05, b.ch1.FREQ_SUM_RST_05, c.ch1.FREQ_SUM_OUT_05, FREQ_SUM_S_05);
  /*p12.FELY*/ c.ch1.FREQ_SUM_OUT_04 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_04, b.ch1.FREQ_SUM_RST_04, c.ch1.FREQ_SUM_OUT_04, FREQ_SUM_S_04);
  /*p12.EDUL*/ c.ch1.FREQ_SUM_OUT_03 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_03, b.ch1.FREQ_SUM_RST_03, c.ch1.FREQ_SUM_OUT_03, FREQ_SUM_S_03);
  /*p12.HAVO*/ c.ch1.FREQ_SUM_OUT_02 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_02, b.ch1.FREQ_SUM_RST_02, c.ch1.FREQ_SUM_OUT_02, FREQ_SUM_S_02);
  /*p12.JYKA*/ c.ch1.FREQ_SUM_OUT_01 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_01, b.ch1.FREQ_SUM_RST_01, c.ch1.FREQ_SUM_OUT_01, FREQ_SUM_S_01);
  /*p12.HYKA*/ c.ch1.FREQ_SUM_OUT_00 = srtock_pos(a.ch1.BOJE, a.ch1.BOJE, b.ch1.FREQ_SUM_SET_00, b.ch1.FREQ_SUM_RST_00, c.ch1.FREQ_SUM_OUT_00, FREQ_SUM_S_00);

  /*p12.BEJU*/ c.ch1.FREQ_SHIFT_SET_10 = nand(b.ch1.FREQ_SUM_OUT_10, b.ch1.KEDO);
  /*p12.BESO*/ c.ch1.FREQ_SHIFT_SET_09 = nand(b.ch1.FREQ_SUM_OUT_09, b.ch1.KEDO);
  /*p12.BEGE*/ c.ch1.FREQ_SHIFT_SET_08 = nand(b.ch1.FREQ_SUM_OUT_08, b.ch1.KEDO);
  /*p12.DACE*/ c.ch1.FREQ_SHIFT_SET_07 = nand(b.ch1.FREQ_SUM_OUT_07, b.ch1.KEDO);
  /*p12.EKEM*/ c.ch1.FREQ_SHIFT_SET_06 = nand(b.ch1.FREQ_SUM_OUT_06, b.ch1.JUJU);
  /*p12.GOVO*/ c.ch1.FREQ_SHIFT_SET_05 = nand(b.ch1.FREQ_SUM_OUT_05, b.ch1.JUJU);
  /*p12.KOLA*/ c.ch1.FREQ_SHIFT_SET_04 = nand(b.ch1.FREQ_SUM_OUT_04, b.ch1.JUJU);
  /*p12.KYRY*/ c.ch1.FREQ_SHIFT_SET_03 = nand(b.ch1.FREQ_SUM_OUT_03, b.ch1.JUJU);
  /*p12.HAWY*/ c.ch1.FREQ_SHIFT_SET_02 = nand(b.ch1.FREQ_SUM_OUT_02, b.ch1.KAPE);
  /*p12.HOLA*/ c.ch1.FREQ_SHIFT_SET_01 = nand(b.ch1.FREQ_SUM_OUT_01, b.ch1.KAPE);
  /*p12.HOZU*/ c.ch1.FREQ_SHIFT_SET_00 = nand(b.ch1.FREQ_SUM_OUT_00, b.ch1.KAPE);

  /*p12.BOJO*/ c.ch1.FREQ_SUM_OUT_10n = not(b.ch1.FREQ_SUM_OUT_10);
  /*p12.APAT*/ c.ch1.FREQ_SUM_OUT_09n = not(b.ch1.FREQ_SUM_OUT_09);
  /*p12.BYRU*/ c.ch1.FREQ_SUM_OUT_08n = not(b.ch1.FREQ_SUM_OUT_08);
  /*p12.CYKY*/ c.ch1.FREQ_SUM_OUT_07n = not(b.ch1.FREQ_SUM_OUT_07);
  /*p12.DEBO*/ c.ch1.FREQ_SUM_OUT_06n = not(b.ch1.FREQ_SUM_OUT_06);
  /*p12.FOHY*/ c.ch1.FREQ_SUM_OUT_05n = not(b.ch1.FREQ_SUM_OUT_05);
  /*p12.KOVO*/ c.ch1.FREQ_SUM_OUT_04n = not(b.ch1.FREQ_SUM_OUT_04);
  /*p12.KEKE*/ c.ch1.FREQ_SUM_OUT_03n = not(b.ch1.FREQ_SUM_OUT_03);
  /*p12.HUNY*/ c.ch1.FREQ_SUM_OUT_02n = not(b.ch1.FREQ_SUM_OUT_02);
  /*p12.HOXE*/ c.ch1.FREQ_SUM_OUT_01n = not(b.ch1.FREQ_SUM_OUT_01);
  /*p12.JUTA*/ c.ch1.FREQ_SUM_OUT_00n = not(b.ch1.FREQ_SUM_OUT_00);

  /*p12.AFYR*/ c.ch1.AFYR = and(b.ch1.FREQ_SUM_OUT_10n, b.ch1.KEDO);
  /*p12.BUVO*/ c.ch1.BUVO = and(b.ch1.FREQ_SUM_OUT_09n, b.ch1.KEDO);
  /*p12.AFUG*/ c.ch1.AFUG = and(b.ch1.FREQ_SUM_OUT_08n, b.ch1.KEDO);
  /*p12.BAPU*/ c.ch1.BAPU = and(b.ch1.FREQ_SUM_OUT_07n, b.ch1.KEDO);
  /*p12.EREG*/ c.ch1.EREG = and(b.ch1.FREQ_SUM_OUT_06n, b.ch1.JUJU);
  /*p12.EVOF*/ c.ch1.EVOF = and(b.ch1.FREQ_SUM_OUT_05n, b.ch1.JUJU);
  /*p12.KEVY*/ c.ch1.KEVY = and(b.ch1.FREQ_SUM_OUT_04n, b.ch1.JUJU);
  /*p12.KAXY*/ c.ch1.KAXY = and(b.ch1.FREQ_SUM_OUT_03n, b.ch1.JUJU);
  /*p12.JEHY*/ c.ch1.JEHY = and(b.ch1.FREQ_SUM_OUT_02n, b.ch1.KAPE);
  /*p12.JOCY*/ c.ch1.JOCY = and(b.ch1.FREQ_SUM_OUT_01n, b.ch1.KAPE);
  /*p12.KOKO*/ c.ch1.KOKO = and(b.ch1.FREQ_SUM_OUT_00n, b.ch1.KAPE);

  /*p12.AVUF*/ c.ch1.FREQ_SHIFT_RST_10 = nor(b.apu.APU_RESET1, b.ch1.AFYR);
  /*p12.AFUX*/ c.ch1.FREQ_SHIFT_RST_09 = nor(b.apu.APU_RESET1, b.ch1.BUVO);
  /*p12.AGOR*/ c.ch1.FREQ_SHIFT_RST_08 = nor(b.apu.APU_RESET1, b.ch1.AFUG);
  /*p12.BEWO*/ c.ch1.FREQ_SHIFT_RST_07 = nor(b.apu.APU_RESET1, b.ch1.BAPU);
  /*p12.ENOK*/ c.ch1.FREQ_SHIFT_RST_06 = nor(b.apu.APU_RESET1, b.ch1.EREG);
  /*p12.EZUK*/ c.ch1.FREQ_SHIFT_RST_05 = nor(b.apu.APU_RESET1, b.ch1.EVOF);
  /*p12.KYBO*/ c.ch1.FREQ_SHIFT_RST_04 = nor(b.apu.APU_RESET1, b.ch1.KEVY);
  /*p12.KETO*/ c.ch1.FREQ_SHIFT_RST_03 = nor(b.apu.APU_RESET1, b.ch1.KAXY);
  /*p12.HYVU*/ c.ch1.FREQ_SHIFT_RST_02 = nor(b.apu.APU_RESET1, b.ch1.JEHY);
  /*p12.HOBU*/ c.ch1.FREQ_SHIFT_RST_01 = nor(b.apu.APU_RESET1, b.ch1.JOCY);
  /*p12.JADO*/ c.ch1.FREQ_SHIFT_RST_00 = nor(b.apu.APU_RESET1, b.ch1.KOKO);

  /*p12.BEKU*/ c.ch1.FREQ_SHIFT_10 = srtock_pos(a.ch1.BECY, b.ch1.BECY, b.ch1.FREQ_SHIFT_SET_10, b.ch1.FREQ_SHIFT_RST_10, b.ch1.FREQ_SHIFT_10, b.chip.P10_B);
  /*p12.AGEZ*/ c.ch1.FREQ_SHIFT_09 = srtock_pos(a.ch1.BECY, b.ch1.BECY, b.ch1.FREQ_SHIFT_SET_09, b.ch1.FREQ_SHIFT_RST_09, b.ch1.FREQ_SHIFT_09, b.ch1.FREQ_SHIFT_10);
  /*p12.ELUX*/ c.ch1.FREQ_SHIFT_08 = srtock_pos(a.ch1.BECY, b.ch1.BECY, b.ch1.FREQ_SHIFT_SET_08, b.ch1.FREQ_SHIFT_RST_08, b.ch1.FREQ_SHIFT_08, b.ch1.FREQ_SHIFT_09);
  /*p12.EXAC*/ c.ch1.FREQ_SHIFT_07 = srtock_pos(a.ch1.BECY, b.ch1.BECY, b.ch1.FREQ_SHIFT_SET_07, b.ch1.FREQ_SHIFT_RST_07, b.ch1.FREQ_SHIFT_07, b.ch1.FREQ_SHIFT_08);
  /*p12.FEDO*/ c.ch1.FREQ_SHIFT_06 = srtock_pos(a.ch1.EJYB, b.ch1.EJYB, b.ch1.FREQ_SHIFT_SET_06, b.ch1.FREQ_SHIFT_RST_06, b.ch1.FREQ_SHIFT_06, b.ch1.FREQ_SHIFT_07);
  /*p12.FUDE*/ c.ch1.FREQ_SHIFT_05 = srtock_pos(a.ch1.EJYB, b.ch1.EJYB, b.ch1.FREQ_SHIFT_SET_05, b.ch1.FREQ_SHIFT_RST_05, b.ch1.FREQ_SHIFT_05, b.ch1.FREQ_SHIFT_06);
  /*p12.JOTA*/ c.ch1.FREQ_SHIFT_04 = srtock_pos(a.ch1.EJYB, b.ch1.EJYB, b.ch1.FREQ_SHIFT_SET_04, b.ch1.FREQ_SHIFT_RST_04, b.ch1.FREQ_SHIFT_04, b.ch1.FREQ_SHIFT_05);
  /*p12.JOLU*/ c.ch1.FREQ_SHIFT_03 = srtock_pos(a.ch1.EJYB, b.ch1.EJYB, b.ch1.FREQ_SHIFT_SET_03, b.ch1.FREQ_SHIFT_RST_03, b.ch1.FREQ_SHIFT_03, b.ch1.FREQ_SHIFT_04);
  /*p12.GOGA*/ c.ch1.FREQ_SHIFT_02 = srtock_pos(b.ch1.EGOR, b.ch1.EGOR, b.ch1.FREQ_SHIFT_SET_02, b.ch1.FREQ_SHIFT_RST_02, b.ch1.FREQ_SHIFT_02, b.ch1.FREQ_SHIFT_03);
  /*p12.JEFA*/ c.ch1.FREQ_SHIFT_01 = srtock_pos(b.ch1.EGOR, b.ch1.EGOR, b.ch1.FREQ_SHIFT_SET_01, b.ch1.FREQ_SHIFT_RST_01, b.ch1.FREQ_SHIFT_01, b.ch1.FREQ_SHIFT_02);
  /*p12.FABU*/ c.ch1.FREQ_SHIFT_00 = srtock_pos(b.ch1.EGOR, b.ch1.EGOR, b.ch1.FREQ_SHIFT_SET_00, b.ch1.FREQ_SHIFT_RST_00, b.ch1.FREQ_SHIFT_00, b.ch1.FREQ_SHIFT_01);

  /*p12.CULU*/ c.ch1.FREQ_SHIFT_FLIP_10 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_10);
  /*p12.DOZY*/ c.ch1.FREQ_SHIFT_FLIP_09 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_09);
  /*p12.CALE*/ c.ch1.FREQ_SHIFT_FLIP_08 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_08);
  /*p12.DYME*/ c.ch1.FREQ_SHIFT_FLIP_07 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_07);
  /*p12.FURE*/ c.ch1.FREQ_SHIFT_FLIP_06 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_06);
  /*p12.GOLY*/ c.ch1.FREQ_SHIFT_FLIP_05 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_05);
  /*p12.KEFE*/ c.ch1.FREQ_SHIFT_FLIP_04 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_04);
  /*p12.HEFY*/ c.ch1.FREQ_SHIFT_FLIP_03 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_03);
  /*p12.GOPO*/ c.ch1.FREQ_SHIFT_FLIP_02 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_02);
  /*p12.GELA*/ c.ch1.FREQ_SHIFT_FLIP_01 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_01);
  /*p12.GYLO*/ c.ch1.FREQ_SHIFT_FLIP_00 = xor(b.ch1.NR10_SWEEP_DIRn, b.ch1.FREQ_SHIFT_00);

  //----------
  // Waveform counter

  c.ch1.CALA = not(b.ch1.CH1_FREQ_10);
  c.ch1.CH1_PHASE_CLKnb = tock_pos(a.ch1.CALA, b.ch1.CALA, b.ch1.DYRU, b.ch1.CH1_PHASE_CLKnb, !b.ch1.CH1_PHASE_CLKnb);
  
  c.ch1.DOKA = and(b.ch1.CH1_PHASE_CLKnb, b.sys.DYFA_1M);
  c.ch1.DYRU = nor(b.apu.APU_RESET1, b.ch1.FEKU, b.ch1.DOKA);
  
  c.ch1.CH1_PHASE_CLKb = not(b.ch1.CH1_PHASE_CLKnb);
  c.ch1.CH1_PHASE_CLKna = not(b.ch1.CH1_PHASE_CLKb);
  c.ch1.CH1_PHASE_CLKa = not(b.ch1.CH1_PHASE_CLKna);

  c.ch1.CH1_PHASE_0 = tock_pos( a.ch1.CH1_PHASE_CLKa,  b.ch1.CH1_PHASE_CLKa, b.ch1.CEPO, c.ch1.CH1_PHASE_0, !c.ch1.CH1_PHASE_0);
  c.ch1.CH1_PHASE_1 = tock_pos(!a.ch1.CH1_PHASE_0,   !b.ch1.CH1_PHASE_0,   b.ch1.CEPO, c.ch1.CH1_PHASE_1, !c.ch1.CH1_PHASE_1);
  c.ch1.CH1_PHASE_2 = tock_pos(!a.ch1.CH1_PHASE_1,   !b.ch1.CH1_PHASE_1,   b.ch1.CEPO, c.ch1.CH1_PHASE_2, !c.ch1.CH1_PHASE_2);

  // ENEK    = 00000011
  // EZOZ    = 00000010
  // PHASE_2 = 00001111
  // CODO    = 11111100

  c.ch1.DUVO = not(b.ch1.CH1_PHASE_0);
  c.ch1.ENEK = and(b.ch1.DUVO, b.ch1.EZOZ);
  c.ch1.EZOZ = and(b.ch1.CH1_PHASE_1, b.ch1.CH1_PHASE_2);
  c.ch1.CODO = not(b.ch1.EZOZ);

  c.ch1.DUTY_0 = nor( b.ch1.CH1_DUTY_0,  b.ch1.CH1_DUTY_1);
  c.ch1.DUTY_1 = nor(!b.ch1.CH1_DUTY_0,  b.ch1.CH1_DUTY_1);
  c.ch1.DUTY_2 = nor( b.ch1.CH1_DUTY_0, !b.ch1.CH1_DUTY_1);
  c.ch1.DUTY_3 = nor(!b.ch1.CH1_DUTY_0, !b.ch1.CH1_DUTY_1);

  c.ch1.CH1_RAW_BIT = amux4(b.ch1.ENEK,        b.ch1.DUTY_0,
                            b.ch1.EZOZ,        b.ch1.DUTY_1,
                            b.ch1.CH1_PHASE_2, b.ch1.DUTY_2,
                            b.ch1.CODO,        b.ch1.DUTY_3);

  //----------
  // Length timer

  c.ch1.FF11_WRna = nand(b.apu.APU_WR, b.apu.ADDR_FF11);
  c.ch1.FF11_WRa = not(b.ch1.FF11_WRna);
  c.ch1.FF11_WRb = not(b.ch1.FF11_WRna);
  c.ch1.FF11_WRc = not(b.ch1.FF11_WRna);
  c.ch1.CORY = nor(b.ch1.FEKU, b.apu.APU_RESET1, b.ch1.FF11_WRb);



  c.ch1.CH1_LEN_CLKn = nor(b.sys.CLK_256a, !b.ch1.NR14_STOP, b.ch1.CH1_LEN_DONE); // use_len polarity?
  c.ch1.CH1_LEN_CLK = not(b.ch1.CH1_LEN_CLKn);

  c.ch1.NR11_LEN0 = count_pos(a.ch1.CH1_LEN_CLK, b.ch1.CH1_LEN_CLK, b.ch1.FF11_WRc, b.ch1.NR11_LEN0, b.D0);
  c.ch1.NR11_LEN1 = count_pos(a.ch1.NR11_LEN0,   b.ch1.NR11_LEN0,   b.ch1.FF11_WRc, b.ch1.NR11_LEN1, b.D1);
  c.ch1.NR11_LEN2 = count_pos(a.ch1.NR11_LEN1,   b.ch1.NR11_LEN1,   b.ch1.FF11_WRc, b.ch1.NR11_LEN2, b.D2);
  c.ch1.NR11_LEN3 = count_pos(a.ch1.NR11_LEN2,   b.ch1.NR11_LEN2,   b.ch1.FF11_WRc, b.ch1.NR11_LEN3, b.D3);
  c.ch1.NR11_LEN3n = not(!b.ch1.NR11_LEN3);
  c.ch1.NR11_LEN4 = count_pos(a.ch1.NR11_LEN3n,  b.ch1.NR11_LEN3n,  b.ch1.FF11_WRa, b.ch1.NR11_LEN4, b.D4);
  c.ch1.NR11_LEN5 = count_pos(a.ch1.NR11_LEN4,   b.ch1.NR11_LEN4,   b.ch1.FF11_WRa, b.ch1.NR11_LEN5, b.D5);

  c.ch1.CH1_LEN_DONE = tock_pos(!a.ch1.NR11_LEN5, !b.ch1.NR11_LEN5, b.ch1.CORY, c.ch1.CH1_LEN_DONE, !c.ch1.CH1_LEN_DONE);

  c.ch1.CH1_SWEEP_STOP = not(b.ch1.CH1_SWEEP_STOPn);
  c.ch1.CH1_AMP_ENn = nor(b.ch1.CH1_ENV_DIR, b.ch1.CH1_VOL_0, b.ch1.CH1_VOL_1, b.ch1.CH1_VOL_2, b.ch1.CH1_VOL_3);

  /*p13.CYFA*/ c.ch1.CH1_LEN_STOP = and(b.ch1.CH1_LEN_DONE, b.ch1.NR14_STOP);
  /*p13.BERY*/ c.ch1.CH1_STOP = or(b.ch1.CH1_SWEEP_STOP, b.apu.APU_RESET1, b.ch1.CH1_LEN_STOP, b.ch1.CH1_AMP_ENn);

  //----------
  // EG timer

  c.ch1.KAZA = nor(b.ch1.FEKU, b.ch1.KOZY);
  c.ch1.SWEEP_RST = not(b.ch1.KAZA);

  c.ch1.CLK_128n = not(b.sys.CLK_128a);
  
  c.ch1.CLK_64a  = tock_pos(a.ch1.CLK_128n, b.ch1.CLK_128n, b.ch1.KADO, b.ch1.CLK_64a, !b.ch1.CLK_64a);
  c.ch1.CLK_64nb = not(b.ch1.CLK_64a);
  c.ch1.CLK_64b  = not(b.ch1.CLK_64nb);

  c.ch1.SWEEP_CNT0 = count_pos(a.ch1.CLK_64b,    b.ch1.CLK_64b,    b.ch1.SWEEP_RST, b.ch1.SWEEP_CNT0, !b.ch1.CH1_SWEEP_0);
  c.ch1.SWEEP_CNT1 = count_pos(a.ch1.SWEEP_CNT0, b.ch1.SWEEP_CNT0, b.ch1.SWEEP_RST, b.ch1.SWEEP_CNT1, !b.ch1.CH1_SWEEP_1);
  c.ch1.SWEEP_CNT2 = count_pos(a.ch1.SWEEP_CNT1, b.ch1.SWEEP_CNT1, b.ch1.SWEEP_RST, b.ch1.SWEEP_CNT2, !b.ch1.CH1_SWEEP_2);

  
  c.ch1.CH1_NO_SWEEP = nor(b.ch1.CH1_SWEEP_0, b.ch1.CH1_SWEEP_1, b.ch1.CH1_SWEEP_2);

  c.ch1.KOTE = and(b.ch1.SWEEP_CNT0, b.ch1.SWEEP_CNT1, b.ch1.SWEEP_CNT2);

  c.ch1.KORO = nor(b.ch1.KUKU, b.ch1.CH1_NO_SWEEP);

  c.ch1.KOZY = tock_pos(a.sys.CLK_512a, b.sys.CLK_512a, b.ch1.KORO, b.ch1.KOZY, b.ch1.KOTE);
  c.ch1.KYNO = tock_pos(a.ch1.KOZY,     b.ch1.KOZY,     b.ch1.KORU, b.ch1.KYNO, b.ch1.CH1_ENV_MAX);


  c.ch1.KURY = not(b.ch1.KOZY);
  c.ch1.KUKU = nor(b.sys.CPUCLK_REQn, b.ch1.KURY);

  c.ch1.FEKU = tock_pos(a.sys.DYFA_1M, b.sys.DYFA_1M, b.ch1.EGET, b.ch1.FEKU, b.ch1.FYFO);
  c.ch1.FARE = tock_pos(a.sys.DYFA_1M, b.sys.DYFA_1M, b.ch1.ERUM, b.ch1.FARE, b.ch1.FEKU);
  c.ch1.FYTE = tock_pos(a.sys.DYFA_1M, b.sys.DYFA_1M, b.ch1.ERUM, b.ch1.FYTE, b.ch1.FARE);

  c.ch1.KORU = nor(b.ch1.FEKU, b.apu.APU_RESET1);

  c.ch1.FEMY = nor(b.ch1.CH1_AMP_ENn, b.apu.APU_RESET1);
  c.ch1.GEPU = not(b.ch1.FYTE);
  c.ch1.GEXU = unk2(b.ch1.FEMY, b.ch1.GEPU);

  c.ch1.EGET = nor(b.apu.APU_RESET1, b.ch1.FARE);
  c.ch1.GEFE = not(b.ch1.EGET);
  c.ch1.FYFO = or(b.ch1.GEFE, b.ch1.CH1_RUNNING); // unk2





  /*p13.KEKO*/ c.ch1.KEKO = or(b.apu.APU_RESET1, b.ch1.FEKU);
  /*p13.KABA*/ c.ch1.KABA = or(b.apu.APU_RESET1, b.ch1.FEKU);
  /*p13.KYLY*/ c.ch1.KYLY = not(b.ch1.KABA);
  /*p13.KEZU*/ c.ch1.KEZU = or(b.ch1.KYNO, b.ch1.KEKO); // unk2
  /*p13.KAKE*/ c.ch1.KAKE = and(b.ch1.KOZY, b.ch1.CH1_NO_SWEEP, b.ch1.KEZU);

  // these are probably flipped or something
  c.ch1.CH1_ACTIVE = or(b.ch1.FEKU, b.ch1.CH1_STOP);
  c.ch1.CH1_ACTIVEn = not(b.ch1.CH1_ACTIVE);

  /*p13.DUWO*/ c.ch1.DUWO = tock_pos(a.ch1.CH1_PHASE_CLKna, b.ch1.CH1_PHASE_CLKna, b.ch1.CEPO, b.ch1.DUWO, b.ch1.CH1_RAW_BIT);
  /*p13.COWE*/ c.ch1.COWE = and(b.ch1.CH1_ACTIVE, b.ch1.DUWO);
  c.ch1.CH1_BIT = or(b.ch1.COWE, b.apu.DBG_APU);

  // I'm not entirely sure how this works...

  /*p13.HESU*/ c.ch1.CH1_ENV_DELTA0 = amux2(b.ch1.CH1_ENV_DIR, b.ch1.CH1_ENV1, !b.ch1.CH1_ENV1, !b.ch1.CH1_ENV_DIR);
  /*p13.HETO*/ c.ch1.CH1_ENV_DELTA1 = amux2(b.ch1.CH1_ENV_DIR, b.ch1.CH1_ENV2, !b.ch1.CH1_ENV2, !b.ch1.CH1_ENV_DIR);
  /*p13.HYTO*/ c.ch1.CH1_ENV_DELTA2 = amux2(b.ch1.CH1_ENV_DIR, b.ch1.CH1_ENV3, !b.ch1.CH1_ENV3, !b.ch1.CH1_ENV_DIR);
  /*p13.JUFY*/ c.ch1.CH1_ENV_DELTA3 = amux2(b.ch1.CH1_ENV_DIR, b.ch1.KAKE,  b.ch1.KAKE, !b.ch1.CH1_ENV_DIR);

  /*p13.HEVO*/ c.ch1.CH1_ENV0 = count_pos(a.ch1.CH1_ENV_DELTA0, b.ch1.CH1_ENV_DELTA0, b.ch1.FEKU, b.ch1.CH1_ENV0, b.ch1.CH1_VOL_0);
  /*p13.HOKO*/ c.ch1.CH1_ENV1 = count_pos(a.ch1.CH1_ENV_DELTA1, b.ch1.CH1_ENV_DELTA1, b.ch1.FEKU, b.ch1.CH1_ENV1, b.ch1.CH1_VOL_1);
  /*p13.HEMY*/ c.ch1.CH1_ENV2 = count_pos(a.ch1.CH1_ENV_DELTA2, b.ch1.CH1_ENV_DELTA2, b.ch1.FEKU, b.ch1.CH1_ENV2, b.ch1.CH1_VOL_2);
  /*p13.HAFO*/ c.ch1.CH1_ENV3 = count_pos(a.ch1.CH1_ENV_DELTA3, b.ch1.CH1_ENV_DELTA3, b.ch1.FEKU, b.ch1.CH1_ENV3, b.ch1.CH1_VOL_3);

  /*p13.AMOP*/ c.ch1.CH1_OUT0 = and(b.ch1.CH1_ENV0, b.ch1.CH1_BIT);
  /*p13.ASON*/ c.ch1.CH1_OUT1 = and(b.ch1.CH1_ENV1, b.ch1.CH1_BIT);
  /*p13.AGOF*/ c.ch1.CH1_OUT2 = and(b.ch1.CH1_ENV2, b.ch1.CH1_BIT);
  /*p13.ACEG*/ c.ch1.CH1_OUT3 = and(b.ch1.CH1_ENV3, b.ch1.CH1_BIT);

  /*p13.HUFU*/ c.ch1.CH1_ENV_TOPn = nand(b.ch1.CH1_ENV_DIR, b.ch1.CH1_ENV0, b.ch1.CH1_ENV1, b.ch1.CH1_ENV2, b.ch1.CH1_ENV3);
  /*p13.HANO*/ c.ch1.CH1_ENV_BOT = nor(b.ch1.CH1_ENV_DIR,  b.ch1.CH1_ENV0, b.ch1.CH1_ENV1, b.ch1.CH1_ENV2, b.ch1.CH1_ENV3);
  /*p13.HAKE*/ c.ch1.CH1_ENV_TOP = not(b.ch1.CH1_ENV_TOPn);
  /*p13.JADE*/ c.ch1.CH1_ENV_MAX = or(b.ch1.CH1_ENV_TOP, b.ch1.CH1_ENV_BOT);

  //----------
  // Sweep timer

  /*p13.DAFA*/ c.ch1.DAFA = nor(b.ch1.BEXA, b.ch1.FEKU);
  /*p13.CYMU*/ c.ch1.CYMU = not(b.ch1.DAFA);
  /*p13.BAVE*/ c.ch1.BAVE = and(b.ch1.NR10_SWEEP_TIME_0, b.ch1.NR10_SWEEP_TIME_1, b.ch1.NR10_SWEEP_TIME_2);

  // this is some debug thing
  /*p09.BAZA*/ c.apu.DBG_SWEEP_CLK = tock_pos(a.sys.AJER_2M, b.sys.AJER_2M, b.apu.APU_RESETn3, b.apu.DBG_SWEEP_CLK, b.apu.DBG_SWEEP);
  /*p09.CELY*/ c.apu.CELY = mux2(b.apu.DBG_SWEEP_CLK, b.sys.CLK_128a, b.apu.DBG_APU);
  /*p09.CONE*/ c.apu.CONE = not(b.apu.CELY);
  /*p09.CATE*/ c.apu.CATE = not(b.apu.CONE);
  /*p13.CUPO*/ c.ch1.CUPO = count_pos(a.apu.CATE, b.apu.CATE, b.ch1.CYMU, b.ch1.CUPO, b.ch1.NR10_SWEEP_TIME_0);
  /*p13.CYPU*/ c.ch1.CYPU = count_pos(a.ch1.CUPO, b.ch1.CUPO, b.ch1.CYMU, b.ch1.CYPU, b.ch1.NR10_SWEEP_TIME_1);
  /*p13.CAXY*/ c.ch1.CAXY = count_pos(a.ch1.CYPU, b.ch1.CYPU, b.ch1.CYMU, b.ch1.CAXY, b.ch1.NR10_SWEEP_TIME_2);
  /*p13.BURY*/ c.ch1.BURY = nor(b.ch1.BAVE, b.apu.APU_RESET1);
  /*p13.COZE*/ c.ch1.COZE = and(b.ch1.CAXY, b.ch1.CYPU, b.ch1.CUPO);
  /*p13.BEXA*/ c.ch1.BEXA = tock_pos(a.sys.AJER_2M, b.sys.AJER_2M, b.ch1.BURY, b.ch1.BEXA, b.ch1.COZE);

  //----------
  // Sweep shift counter

  /*p13.DACU*/ c.ch1.DACU = nor(b.ch1.FEKU, b.ch1.BEXA);
  /*p13.CYLU*/ c.ch1.CYLU = not(b.ch1.DACU);
  /*p13.BUGE*/ c.ch1.BUGE = nand(b.ch1.NR10_SWEEP_SHIFT_0, b.ch1.NR10_SWEEP_SHIFT_1, b.ch1.NR10_SWEEP_SHIFT_2);
  /*p13.CELE*/ c.ch1.CELE = not(b.ch1.BUGE);
  /*p13.ADAD*/ c.ch1.ADAD = not(!b.ch1.BYTE);
  /*p13.EPUK*/ c.ch1.EPUK = nor(b.ch1.ADAD, b.apu.APU_RESET1);
  /*p13.EVOL*/ c.ch1.EVOL = nor(b.ch1.BEXA, b.ch1.FYTE);
  /*p13.FEMU*/ c.ch1.FEMU = unk2(b.ch1.EPUK, b.ch1.EVOL);
  /*p13.EGYP*/ c.ch1.EGYP = nor(b.sys.DYFA_1M, b.ch1.FEMU);
  /*p13.DODY*/ c.ch1.DODY = nor(b.ch1.EGYP, b.ch1.CELE);
  /*p13.EGOR*/ c.ch1.EGOR = and(b.ch4.NR41_LEN3b, b.ch1.DODY); // wtf? probably debug something
  /*p13.DAPU*/ c.ch1.DAPU = not(b.ch1.EGOR);
  /*p13.COPA*/ c.ch1.COPA = count_pos(a.ch1.DAPU, b.ch1.DAPU, b.ch1.CYLU, b.ch1.COPA, b.ch1.NR10_SWEEP_SHIFT_0);
  /*p13.CAJA*/ c.ch1.CAJA = count_pos(a.ch1.COPA, b.ch1.COPA, b.ch1.CYLU, b.ch1.CAJA, b.ch1.NR10_SWEEP_SHIFT_1);
  /*p13.BYRA*/ c.ch1.BYRA = count_pos(a.ch1.CAJA, b.ch1.CAJA, b.ch1.CYLU, b.ch1.BYRA, b.ch1.NR10_SWEEP_SHIFT_2);
  /*p13.COPY*/ c.ch1.COPY = and(b.ch1.COPA, b.ch1.CAJA, b.ch1.BYRA);
  /*p13.ATAT*/ c.ch1.ATAT = nor(b.apu.APU_RESET1, b.ch1.BEXA);
  /*p13.BYTE*/ c.ch1.BYTE = tock_pos(a.sys.AJER_2M, b.sys.AJER_2M, b.ch1.ATAT, b.ch1.BYTE, b.ch1.COPY);
  /*p13.ATUV*/ c.ch1.ATUV = and(b.ch1.BEXA, b.ch1.CH1_SWEEP_STOPn);
  /*p13.BOJE*/ c.ch1.BOJE = and(b.ch1.ATUV, b.ch1.BUGE);
  /*p13.BUSO*/ c.ch1.BUSO = or(b.ch1.BUGE, b.ch1.CH1_SWEEP_STOPn, b.ch1.BEXA);
  /*p13.KALA*/ c.ch1.KALA = nor(b.ch1.BEXA, b.ch1.FEKU);
}
