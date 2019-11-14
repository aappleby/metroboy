#include "P11_Ch1Regs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P11_Ch1Regs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // FF10 NR10

  c.p11.CENU = and(b.p10.APU_WR, b.p10.FF10);
  c.p11.BUZE = not(b.p10.FF10);
  c.p11.ATYN = nor(b.p09.CPU_RDn, b.p11.BUZE);
  c.p11.ASOP = not(b.p11.ATYN);

  c.p11.BANY = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.BANY, b.D0);
  c.p11.ANAZ = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.ANAZ, b.D2);
  c.p11.BOTU = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.BOTU, b.D6);
  c.p11.AVAF = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.AVAF, b.D3);
  c.p11.ARAX = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.ARAX, b.D1);
  c.p11.ADEK = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.ADEK, b.D4);
  c.p11.BANA = tock_pos(a.p11.CENU, b.p11.CENU, b.p09.APU_RESETn, b.p11.BANA, b.D5);

  c.p11.AMYD = not(!b.p11.BANY);
  c.p11.AZYP = not(!b.p11.ANAZ);
  c.p11.AWOS = not(!b.p11.BOTU);
  c.p11.AFOX = not(!b.p11.AVAF);
  c.p11.ATAX = not(!b.p11.ARAX);
  c.p11.AVEK = not(!b.p11.ADEK);
  c.p11.AKUX = not(!b.p11.BANA);

  if (b.p11.ASOP) {
    c.D0 = b.p11.AMYD;
    c.D2 = b.p11.AZYP;
    c.D6 = b.p11.AWOS;
    c.D3 = b.p11.AFOX;
    c.D1 = b.p11.ATAX;
    c.D4 = b.p11.AVEK;
    c.D5 = b.p11.AKUX;
  }

  //----------
  // FF11 NR11

  // BUG APU_WR
  c.p11.COVU = and(b.p10.APU_WR, b.p10.FF11);
  c.p11.DAFO = not(b.p11.COVU);
  c.p11.CENA = tock_pos(a.p11.DAFO, b.p11.DAFO, b.p11.CEPO, b.p11.CENA, b.D6);
  c.p11.DYCA = tock_pos(a.p11.DAFO, b.p11.DAFO, b.p11.CEPO, b.p11.DYCA, b.D7);

  c.p11.BUWA = not(b.p09.CPU_RDn);
  c.p11.BEXU = nor(b.p11.BUWA, b.p10.FF11);

  c.p11.BOWO = not(!b.p11.CENA);
  c.p11.CUDA = not(!b.p11.DYCA);

  if (b.p11.BEXU) {
    c.D6 = b.p11.BOWO;
    c.D7 = b.p11.CUDA;
  }

  //----------
  // FF12 NR12

  c.p11.GAXU = nand(b.p10.FF12, b.p10.APU_WR);
  c.p11.GAGO = not(b.p10.FF12);
  c.p11.KAGY = not(b.p11.GAXU);

  c.p11.HAXE = not(b.p10.FF12);
  c.p11.HAFU = and(b.p10.APU_WR, b.p10.FF12);
  c.p11.KYGY = not(b.p11.HAFU);

  c.p11.HAMY = or(b.p11.HAXE, b.p09.CPU_RDn);
  c.p11.HOCU = or(b.p11.GAGO, b.p09.CPU_RDn);

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

  if (b.p11.HAMY) {
    c.D0 = b.p11.JYNE;
    c.D1 = b.p11.JACA;
    c.D2 = b.p11.JOKU;
  }

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
  c.p11.CACA = not(b.p10.FF13);
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

  c.p11.CURE = not(b.p10.FF14);
  c.p11.DUPY = or(b.p11.CURE, b.p11.DAXA);

  c.p11.DOPA = not(!b.p11.EMUS);
  c.p11.DEMU = not(!b.p11.EVAK);
  c.p11.DEXO = not(!b.p11.COPU);

  if (b.p11.DUPY) {
    c.D0 = b.p11.DOPA;
    c.D1 = b.p11.DEMU;
    c.D2 = b.p11.DEXO;
  }

  //----------
  // FF14 NR14

  // why is ANUJ here?

  c.p11.BAGE = nand(b.p16.ANUJ, b.p10.FF14); 
  c.p11.BALE = nand(b.p11.CPU_RDn, b.p10.FF14);
  c.p11.BOKO = tock_pos(a.p11.BAGE, b.p11.BAGE, b.p11.CAMY, c.p11.BOKO, b.D6);

  c.p11.BYTU = not(!b.p11.BOKO);
  if (b.p11.BALE) {
    c.D6 = b.p11.BYTU;
  }
}
