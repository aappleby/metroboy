#include "P11_Ch1Regs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P11_Ch1Regs_tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // FF10 NR10

  c.p11.BUZE = not(b.p10.ADDR_FF10);
  c.p11.ATYN = nor(b.p09.CPU_RDn, b.p11.BUZE);
  c.p11.ASOP = not(b.p11.ATYN);

  c.p11.CENU = and(b.p10.APU_WR, b.p10.ADDR_FF10);
  c.p11.CH1_SWEEP_SHIFT_0 = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.CH1_SWEEP_SHIFT_0, b.D0);
  c.p11.CH1_SWEEP_SHIFT_1 = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.CH1_SWEEP_SHIFT_1, b.D1);
  c.p11.CH1_SWEEP_SHIFT_2 = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.CH1_SWEEP_SHIFT_2, b.D2);
  c.p11.CH1_SWEEP_DIR     = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.CH1_SWEEP_DIR,     b.D3);
  c.p11.CH1_SWEEP_TIME_0  = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.CH1_SWEEP_TIME_0,  b.D4);
  c.p11.CH1_SWEEP_TIME_1  = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.CH1_SWEEP_TIME_1,  b.D5);
  c.p11.CH1_SWEEP_TIME_2  = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.CH1_SWEEP_TIME_2,  b.D6);
  
  c.p11.FF10_D0 = not(!b.p11.CH1_SWEEP_SHIFT_0);
  c.p11.FF10_D1 = not(!b.p11.CH1_SWEEP_SHIFT_1);
  c.p11.FF10_D2 = not(!b.p11.CH1_SWEEP_SHIFT_2);
  c.p11.FF10_D3 = not(!b.p11.CH1_SWEEP_DIR);
  c.p11.FF10_D4 = not(!b.p11.CH1_SWEEP_TIME_0);
  c.p11.FF10_D5 = not(!b.p11.CH1_SWEEP_TIME_1);
  c.p11.FF10_D6 = not(!b.p11.CH1_SWEEP_TIME_2);

  if (b.p11.ASOP) {
    c.D0 = b.p11.FF10_D0;
    c.D1 = b.p11.FF10_D1;
    c.D2 = b.p11.FF10_D2;
    c.D3 = b.p11.FF10_D3;
    c.D4 = b.p11.FF10_D4;
    c.D5 = b.p11.FF10_D5;
    c.D6 = b.p11.FF10_D6;
  }

  //----------
  // FF11 NR11

  /*p11.COVU*/ c.p11.FF11_WR = and(b.p10.APU_WR, b.p10.ADDR_FF11);
  /*p11.DAFO*/ c.p11.FF11_WRn = not(b.p11.FF11_WR);

  /*p11.CENA*/ c.p11.CH1_DUTY_0 = tock_pos(a.p11.FF11_WRn, b.p11.FF11_WRn, b.p11.CEPO, b.p11.CH1_DUTY_0, b.D6);
  /*p11.DYCA*/ c.p11.CH1_DUTY_1 = tock_pos(a.p11.FF11_WRn, b.p11.FF11_WRn, b.p11.CEPO, b.p11.CH1_DUTY_1, b.D7);

  /*p11.BUWA*/ c.p11.CPU_RD2 = not(b.p09.CPU_RDn);
  /*p11.BEXU*/ c.p11.FF11_RD = nor(b.p11.CPU_RD2, b.p10.ADDR_FF11); // polarity?

  /*p11.BOWO*/ c.p11.FF11_D6 = not(!b.p11.CH1_DUTY_0);
  /*p11.CUDA*/ c.p11.FF11_D7 = not(!b.p11.CH1_DUTY_1);

  if (b.p11.FF11_RD) {
    c.D6 = b.p11.FF11_D6;
    c.D7 = b.p11.FF11_D7;
  }

  //----------
  // FF12 NR12

  c.p11.KAGY = not(b.p11.GAXU); // unused?

  c.p11.HAFU = and(b.p10.APU_WR, b.p10.ADDR_FF12);
  c.p11.KYGY = not(b.p11.HAFU);
  c.p11.GAXU = nand(b.p10.APU_WR, b.p10.ADDR_FF12);
  c.p11.FF12_D0 = tock_pos(a.p11.KYGY, b.p11.KYGY, b.p11.HATO, b.p11.FF12_D0, b.D0);
  c.p11.FF12_D1 = tock_pos(a.p11.KYGY, b.p11.KYGY, b.p11.HATO, b.p11.FF12_D1, b.D1);
  c.p11.FF12_D2 = tock_pos(a.p11.KYGY, b.p11.KYGY, b.p11.HATO, b.p11.FF12_D2, b.D2);
  c.p11.FF12_D3 = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.FF12_D3, b.D3);
  c.p11.FF12_D4 = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.FF12_D4, b.D4);
  c.p11.FF12_D5 = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.FF12_D5, b.D5);
  c.p11.FF12_D6 = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.FF12_D6, b.D6);
  c.p11.FF12_D7 = tock_pos(a.p11.GAXU, b.p11.GAXU, b.p11.HATO, b.p11.FF12_D7, b.D7);

  c.p11.JYNE = not(!b.p11.FF12_D0);
  c.p11.JACA = not(!b.p11.FF12_D1);
  c.p11.JOKU = not(!b.p11.FF12_D2);
  c.p11.HONO = not(!b.p11.FF12_D3);
  c.p11.HOWU = not(!b.p11.FF12_D4);
  c.p11.HEWA = not(!b.p11.FF12_D5);
  c.p11.HEVE = not(!b.p11.FF12_D6);
  c.p11.JYSE = not(!b.p11.FF12_D7);

  c.p11.HAXE = not(b.p10.ADDR_FF12);
  c.p11.HAMY = or(b.p11.HAXE, b.p09.CPU_RDn);
  if (b.p11.HAMY) {
    c.D0 = b.p11.JYNE;
    c.D1 = b.p11.JACA;
    c.D2 = b.p11.JOKU;
  }

  c.p11.GAGO = not(b.p10.ADDR_FF12);
  c.p11.HOCU = or(b.p11.GAGO, b.p09.CPU_RDn);
  if (b.p11.HOCU) {
    c.D3 = b.p11.HONO;
    c.D4 = b.p11.HOWU;
    c.D5 = b.p11.HEWA;
    c.D6 = b.p11.HEVE;
    c.D7 = b.p11.JYSE;
  }

  //----------
  // FF13 NR13

  c.p11.FULO = nor(b.p01.DYFA_1M, b.p13.GEXU);
  c.p11.EPYK = nor(b.p13.FEKU, b.p13.COPE);
  c.p11.GEKU = not(b.p11.FULO);
  c.p11.FUME = not(b.p11.EPYK);

  c.p11.CH1_FREQ_00 = count_pos(a.p11.GEKU, b.p11.GEKU, b.p11.FUME, b.p11.CH1_FREQ_00, b.p12.HYKA);
  c.p11.CH1_FREQ_01 = count_pos(a.p11.GAXE, b.p11.GAXE, b.p11.FUME, b.p11.CH1_FREQ_01, b.p12.JYKA);
  c.p11.CH1_FREQ_02 = count_pos(a.p11.HYFE, b.p11.HYFE, b.p11.FUME, b.p11.CH1_FREQ_02, b.p12.HAVO);
  c.p11.CH1_FREQ_03 = count_pos(a.p11.JYTY, b.p11.JYTY, b.p11.FUME, b.p11.CH1_FREQ_03, b.p12.EDUL);

  c.p11.DEGA = not(b.p11.EPYK);
  c.p11.KYPE = not(b.p11.KYNA);

  c.p11.JEMA = count_pos(a.p11.KYPE, b.p11.KYPE, b.p11.DEGA, b.p11.JEMA, b.p12.FELY);
  c.p11.HYKE = count_pos(a.p11.JEMA, b.p11.JEMA, b.p11.DEGA, b.p11.HYKE, b.p12.HOLU);
  c.p11.FEVA = count_pos(a.p11.HYKE, b.p11.HYKE, b.p11.DEGA, b.p11.FEVA, b.p12.HYXU);
  c.p11.EKOV = count_pos(a.p11.FEVA, b.p11.FEVA, b.p11.DEGA, b.p11.EKOV, b.p12.HOPO);

  c.p11.DAKO = not(b.p11.EPYK);
  c.p11.DERU = not(b.p11.EKOV);

  c.p11.EMUS = count_pos(a.p11.DERU, b.p11.DERU, b.p11.DAKO, b.p11.EMUS, b.p12.DYGY);
  c.p11.EVAK = count_pos(a.p11.EMUS, b.p11.EMUS, b.p11.DAKO, b.p11.EVAK, b.p12.EVAB);
  c.p11.COPU = count_pos(a.p11.EVAK, b.p11.EVAK, b.p11.DAKO, b.p11.COPU, b.p12.AXAN);

  c.p11.FORU = not(!b.p11.GAXE);
  c.p11.GEFU = not(!b.p11.HYFE);
  c.p11.KYVU = not(!b.p11.JYTY);
  c.p11.KUMO = not(!b.p11.KYNA);
  c.p11.KARY = not(!b.p11.JEMA);
  c.p11.GODE = not(!b.p11.HYKE);
  c.p11.GOJE = not(!b.p11.FEVA);
  c.p11.FOZE = not(!b.p11.EKOV);

  c.p11.CEGE = not(b.p09.CPU_RDn);
  c.p11.DAXA = nand(b.p11.CEGE, b.p09.NET03);

  c.p11.CACA = not(b.p10.ADDR_FF13);
  c.p11.DYPU = nor(b.p11.DAXA, b.p11.CACA);
  c.p11.EVAJ = not(b.p11.DYPU);
  if (b.p11.EVAJ) {
    c.D0 = b.p11.FORU;
    c.D1 = b.p11.GEFU;
    c.D2 = b.p11.KYVU;
    c.D3 = b.p11.KUMO;
    c.D4 = b.p11.KARY;
    c.D5 = b.p11.GODE;
    c.D6 = b.p11.GOJE;
    c.D7 = b.p11.FOZE;
  }

  c.p11.DOPA = not(!b.p11.EMUS);
  c.p11.DEMU = not(!b.p11.EVAK);
  c.p11.DEXO = not(!b.p11.COPU);

  c.p11.CURE = not(b.p10.ADDR_FF14);
  c.p11.DUPY = or(b.p11.CURE, b.p11.DAXA);
  if (b.p11.DUPY) {
    c.D0 = b.p11.DOPA;
    c.D1 = b.p11.DEMU;
    c.D2 = b.p11.DEXO;
  }

  //----------
  // FF14 NR14

  c.p11.BAGE = nand(b.p16.CPU_WR_WEIRD, b.p10.ADDR_FF14); 
  c.p11.CH1_USE_LENGTH = tock_pos(a.p11.BAGE, b.p11.BAGE, b.p11.CAMY, c.p11.CH1_USE_LENGTH, b.D6);

  c.p11.FF14_D6 = not(!b.p11.CH1_USE_LENGTH);

  c.p11.FF14_RD = nand(b.p11.CPU_RDn, b.p10.ADDR_FF14);
  if (b.p11.FF14_RD) {
    c.D6 = b.p11.FF14_D6;
  }
}
