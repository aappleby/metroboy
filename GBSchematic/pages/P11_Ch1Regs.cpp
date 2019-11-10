#include "P11_Ch1Regs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P11_Ch1Regs::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P11_Ch1Regs pa = ga.p11;
  const P11_Ch1Regs pb = gb.p11;
  P11_Ch1Regs pc = gc.p11;

  //----------
  // FF10 NR10

  // schematic image cut off, this could be APU_WR or CPU_WR
  pc.CENU = and(gb.p10.APU_WR, pb.FF10);
  pc.BUZE = not(pb.FF10);
  pc.ATYN = nor(gb.p09.CPU_RDn, pb.BUZE);
  pc.ASOP = not(pb.ATYN);

  pc.BANY = tock_pos(pa.CENU, pb.CENU, gb.p09.APU_RESETn, pb.BANY, gb.D0);
  pc.ANAZ = tock_pos(pa.CENU, pb.CENU, gb.p09.APU_RESETn, pb.ANAZ, gb.D2);
  pc.BOTU = tock_pos(pa.CENU, pb.CENU, gb.p09.APU_RESETn, pb.BOTU, gb.D6);
  pc.AVAF = tock_pos(pa.CENU, pb.CENU, gb.p09.APU_RESETn, pb.AVAF, gb.D3);
  pc.ARAX = tock_pos(pa.CENU, pb.CENU, gb.p09.APU_RESETn, pb.ARAX, gb.D1);
  pc.ADEK = tock_pos(pa.CENU, pb.CENU, gb.p09.APU_RESETn, pb.ADEK, gb.D4);
  pc.BANA = tock_pos(pa.CENU, pb.CENU, gb.p09.APU_RESETn, pb.BANA, gb.D5);

  pc.AMYD = not(!pb.BANY);
  pc.AZYP = not(!pb.ANAZ);
  pc.AWOS = not(!pb.BOTU);
  pc.AFOX = not(!pb.AVAF);
  pc.ATAX = not(!pb.ARAX);
  pc.AVEK = not(!pb.ADEK);
  pc.AKUX = not(!pb.BANA);

  pc.FF10_D0n = pb.BANY;
  pc.FF10_D1n = pb.ARAX;
  pc.FF10_D2n = pb.ANAZ;
  pc.FF10_D3n = pb.AVAF;
  pc.FF10_D4n = pb.ADEK;
  pc.FF10_D5n = pb.BANA;
  pc.FF10_D6n = pb.BOTU;

  if (pb.ASOP) {
    gc.D0 = pb.AMYD;
    gc.D2 = pb.AZYP;
    gc.D6 = pb.AWOS;
    gc.D3 = pb.AFOX;
    gc.D1 = pb.ATAX;
    gc.D4 = pb.AVEK;
    gc.D5 = pb.AKUX;
  }

  //----------
  // FF11 NR11

  // BUG APU_WR
  pc.COVU = and(gb.p10.APU_WR, pb.FF11);
  pc.CEPO = not(gb.p09.APU_RESET);
  pc.DAFO = not(pb.COVU);
  pc.CENA = tock_pos(pa.DAFO, pb.DAFO, pb.CEPO, pb.CENA, gb.D6);
  pc.DYCA = tock_pos(pa.DAFO, pb.DAFO, pb.CEPO, pb.DYCA, gb.D7);

  pc.BUWA = not(gb.p09.CPU_RDn);
  pc.BEXU = nor(pb.BUWA, pb.FF11);

  pc.BOWO = not(!pb.CENA);
  pc.CUDA = not(!pb.DYCA);

  pc.FF11_D6  = pb.CENA;
  pc.FF11_D6n = !pb.CENA;
  pc.FF11_D7  = pb.DYCA;
  pc.FF11_D7n = !pb.DYCA;

  if (pb.BEXU) {
    gc.D6 = pb.BOWO;
    gc.D7 = pb.CUDA;
  }

  //----------
  // FF12 NR12

  pc.GAXU = nand(pb.FF12, gb.p10.APU_WR);
  pc.HATO = not(gb.p09.APU_RESET);
  pc.GAGO = not(pb.FF12);
  pc.KAGY = not(pb.GAXU);
  pc.HOCU = or(pb.GAGO, gb.p09.CPU_RDn);

  pc.JOPU = tock_pos(pa.GAXU, pb.GAXU, pb.HATO, pb.JOPU, gb.D7);
  pc.JENA = tock_pos(pa.GAXU, pb.GAXU, pb.HATO, pb.JENA, gb.D6);
  pc.JAXO = tock_pos(pa.GAXU, pb.GAXU, pb.HATO, pb.JAXO, gb.D5);
  pc.JATY = tock_pos(pa.GAXU, pb.GAXU, pb.HATO, pb.JATY, gb.D4);
  pc.JAFY = tock_pos(pa.GAXU, pb.GAXU, pb.HATO, pb.JAFY, gb.D3);

  pc.JYSE = not(!pb.JOPU);
  pc.HEVE = not(!pb.JENA);
  pc.HEWA = not(!pb.JAXO);
  pc.HOWU = not(!pb.JATY);
  pc.HONO = not(!pb.JAFY);

  if (pb.HOCU) {
    gc.D7 = pb.JYSE;
    gc.D6 = pb.HEVE;
    gc.D5 = pb.HEWA;
    gc.D4 = pb.HOWU;
    gc.D3 = pb.HONO;
  }

  pc.HAXE = not(pb.FF12);
  pc.HAFU = and(gb.p10.APU_WR, pb.FF12);
  pc.HAMY = or(pb.HAXE, gb.p09.CPU_RDn);
  pc.KYGY = not(pb.HAFU);

  pc.JUSA = tock_pos(pa.KYGY, pb.KYGY, pb.HATO, pb.JUSA, gb.D0);
  pc.JUZY = tock_pos(pa.KYGY, pb.KYGY, pb.HATO, pb.JUZY, gb.D1);
  pc.JOMA = tock_pos(pa.KYGY, pb.KYGY, pb.HATO, pb.JOMA, gb.D2);

  pc.JYNE = not(!pb.JUSA);
  pc.JACA = not(!pb.JUZY);
  pc.JOKU = not(!pb.JOMA);

  if (pb.HAMY) {
    gc.D0 = pb.JYNE;
    gc.D1 = pb.JACA;
    gc.D2 = pb.JOKU;
  }

  //----------
  // FF13 NR13

  pc.FULO = nor(gb.p09.DYFA_1M, pb.GEXU);
  pc.EPYK = nor(pb.FEKU, pb.COPE);
  pc.GEKU = not(pb.FULO);
  pc.FUME = not(pb.EPYK);

  // FIXME daisy chain
  pc.GAXE = count_pos(pa.GEKU, pb.GEKU, pb.FUME, pb.GAXE, pb.HYKA);
  pc.HYFE = count_pos(pa.GAXE, pb.GAXE, pb.FUME, pb.HYFE, pb.JYKA);
  pc.JYTY = count_pos(pa.HYFE, pb.HYFE, pb.FUME, pb.JYTY, pb.HAVO);
  pc.KYNA = count_pos(pa.JYTY, pb.JYTY, pb.FUME, pb.KYNA, pb.EDUL);

  pc.DEGA = not(pb.EPYK);
  pc.KYPE = not(pb.KYNA);

  pc.JEMA = count_pos(pa.KYPE, pb.KYPE, pb.DEGA, pb.JEMA, pb.FELY);
  pc.HYKE = count_pos(pa.JEMA, pb.JEMA, pb.DEGA, pb.HYKE, pb.HOLU);
  pc.FEVA = count_pos(pa.HYKE, pb.HYKE, pb.DEGA, pb.FEVA, pb.HYXU);
  pc.EKOV = count_pos(pa.FEVA, pb.FEVA, pb.DEGA, pb.EKOV, pb.HOPO);

  pc.DAKO = not(pb.EPYK);
  pc.DERU = not(pb.EKOV);

  pc.EMUS = count_pos(pa.DERU, pb.DERU, pb.DAKO, pb.EMUS, pb.DYGY);
  pc.EVAK = count_pos(pa.EMUS, pb.EMUS, pb.DAKO, pb.EVAK, pb.EVAB);
  pc.COPU = count_pos(pa.EVAK, pb.EVAK, pb.DAKO, pb.COPU, pb.AXAN);

  pc.FORU = not(!pb.GAXE);
  pc.GEFU = not(!pb.HYFE);
  pc.KYVU = not(!pb.JYTY);
  pc.KUMO = not(!pb.KYNA);
  pc.KARY = not(!pb.JEMA);
  pc.GODE = not(!pb.HYKE);
  pc.GOJE = not(!pb.FEVA);
  pc.FOZE = not(!pb.EKOV);

  pc.CEGE = not(gb.p09.CPU_RDn);
  pc.DAXA = nand(pb.CEGE, gb.p09.NET03);
  pc.CACA = not(pb.FF13);
  pc.DYPU = nor(pb.DAXA, pb.CACA);
  pc.EVAJ = not(pb.DYPU);

  if (pb.EVAJ) {
    gc.D0 = pb.FORU;
    gc.D1 = pb.GEFU;
    gc.D2 = pb.KYVU;
    gc.D3 = pb.KUMO;
    gc.D4 = pb.KARY;
    gc.D5 = pb.GODE;
    gc.D6 = pb.GOJE;
    gc.D7 = pb.FOZE;
  }

  pc.CURE = not(pb.FF14);
  pc.DUPY = or(pb.CURE, pb.DAXA);

  pc.DOPA = not(!pb.EMUS);
  pc.DEMU = not(!pb.EVAK);
  pc.DEXO = not(!pb.COPU);

  if (pb.DUPY) {
    gc.D0 = pb.DOPA;
    gc.D1 = pb.DEMU;
    gc.D2 = pb.DEXO;
  }

  //----------
  // FF14 NR14

  // why is ANUJ here?

  pc.BAGE = nand(gb.p16.ANUJ, pb.FF14); 
  pc.CAMY = not(gb.p09.APU_RESET);
  pc.BUDA = not(gb.p07.CPU_RD);
  pc.BALE = nand(pb.BUDA, pb.FF14);
  pc.BOKO = tock_pos(pa.BAGE, pb.BAGE, pb.CAMY, pc.BOKO, gb.D6);

  pc.FF14_D6  = pb.BOKO;
  pc.FF14_D6n = !pb.BOKO;

  pc.BYTU = not(!pb.BOKO);
  if (pb.BALE) {
    gc.D6 = pb.BYTU;
  }
}
