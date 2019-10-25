#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P11_Ch1Regs {
  struct Input {
    bool APU_RESET;
    bool APU_RESETn;

    bool CPU_RD;
    bool CPU_RDn;
    bool CPU_WR;
    bool APU_WR;

    bool ANUJ;
    bool NET03;

    bool FF10;
    bool FF11;
    bool FF12;
    bool FF13;
    bool FF14;

    bool DYFA_1MHZ;
    bool GEXU;
    bool FEKU;
    bool COPE;

    // ff13 load value
    bool HYKA3,JYKA3,HAVO3,EDUL3,FELY3,HOLU3,HYXU3,HOPO3,DYGY3,EVAB3,AXAN3;

    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool FF10_D0n,FF10_D1n,FF10_D2n,FF10_D3n,FF10_D4n,FF10_D5n,FF10_D6n;
    
    bool FF11_D6, FF11_D6n;
    bool FF11_D7, FF11_D7n;

    bool FF14_D6, FF14_D6n;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  reg BANY,ANAZ,BOTU,AVAF,ARAX,ADEK,BANA; // FF10 NR10
  reg CENA,DYCA; // FF11 NR11
  reg JOPU,JENA,JAXO,JATY,JAFY,JUSA,JUZY,JOMA; // FF12 NR12
  reg GAXE,HYFE,JYTY,KYNA,JEMA,HYKE,FEVA,EKOV,EMUS,EVAK,COPU; // FF13 NR13
  reg BOKO; // FF14 NR14

  void tick(const Input& in, Output& out) {
    //----------
    // FF10 NR10

    // schematic image cut off, this could be APU_WR or CPU_WR
    wire CENU = and(in.APU_WR, in.FF10);
    wire BUZE = not(in.FF10);
    wire ATYN = nor(in.CPU_RDn, BUZE);
    wire ASOP = not(ATYN);

    wire BANY_Q = BANY.tock(CENU, in.APU_RESETn, in.D0);
    wire ANAZ_Q = ANAZ.tock(CENU, in.APU_RESETn, in.D2);
    wire BOTU_Q = BOTU.tock(CENU, in.APU_RESETn, in.D6);
    wire AVAF_Q = AVAF.tock(CENU, in.APU_RESETn, in.D3);
    wire ARAX_Q = ARAX.tock(CENU, in.APU_RESETn, in.D1);
    wire ADEK_Q = ADEK.tock(CENU, in.APU_RESETn, in.D4);
    wire BANA_Q = BANA.tock(CENU, in.APU_RESETn, in.D5);

    wire AMYD = not(!BANY_Q);
    wire AZYP = not(!ANAZ_Q);
    wire AWOS = not(!BOTU_Q);
    wire AFOX = not(!AVAF_Q);
    wire ATAX = not(!ARAX_Q);
    wire AVEK = not(!ADEK_Q);
    wire AKUX = not(!BANA_Q);

    out.FF10_D0n = BANY_Q;
    out.FF10_D2n = ANAZ_Q;
    out.FF10_D6n = BOTU_Q;
    out.FF10_D3n = AVAF_Q;
    out.FF10_D1n = ARAX_Q;
    out.FF10_D4n = ADEK_Q;
    out.FF10_D5n = BANA_Q;

    if (ASOP) {
      out.D_OE = true;
      out.D0 = AMYD;
      out.D2 = AZYP;
      out.D6 = AWOS;
      out.D3 = AFOX;
      out.D1 = ATAX;
      out.D4 = AVEK;
      out.D5 = AKUX;
    }

    //----------
    // FF11 NR11

    // BUG APU_WR
    wire COVU = and(in.APU_WR, in.FF11);
    wire CEPO = not(in.APU_RESET);
    wire DAFO = not(COVU);
    wire CENA_Q = CENA.tock(DAFO, CEPO, in.D6);
    wire DYCA_Q = DYCA.tock(DAFO, CEPO, in.D7);

    wire BUWA = not(in.CPU_RDn);
    wire BEXU = nor(BUWA, in.FF11);

    wire BOWO = not(!CENA_Q);
    wire CUDA = not(!DYCA_Q);

    out.FF11_D6  = CENA_Q;
    out.FF11_D6n = !CENA_Q;
    out.FF11_D7  = DYCA_Q;
    out.FF11_D7n = !DYCA_Q;

    if (BEXU) {
      out.D_OE;
      out.D6 = BOWO;
      out.D7 = CUDA;
    }

    //----------
    // FF12 NR12

    wire GAXU = nand(in.FF12, in.APU_WR);
    wire HATO = not(in.APU_RESET);
    wire GAGO = not(in.FF12);
    wire KAGY = not(GAXU);
    wire HOCU = or(GAGO, in.CPU_RDn);

    // Unused on schematic
    (void)KAGY;

    wire JOPU_Q = JOPU.tock(GAXU, HATO, in.D7);
    wire JENA_Q = JENA.tock(GAXU, HATO, in.D6);
    wire JAXO_Q = JAXO.tock(GAXU, HATO, in.D5);
    wire JATY_Q = JATY.tock(GAXU, HATO, in.D4);
    wire JAFY_Q = JAFY.tock(GAXU, HATO, in.D3);

    wire JYSE = not(!JOPU_Q);
    wire HEVE = not(!JENA_Q);
    wire HEWA = not(!JAXO_Q);
    wire HOWU = not(!JATY_Q);
    wire HONO = not(!JAFY_Q);

    if (HOCU) {
      out.D_OE = true;
      out.D7 = JYSE;
      out.D6 = HEVE;
      out.D5 = HEWA;
      out.D4 = HOWU;
      out.D3 = HONO;
    }

    wire HAXE = not(in.FF12);
    wire HAFU = and(in.APU_WR, in.FF12);
    wire HAMY = or(HAXE, in.CPU_RDn);
    wire KYGY = not(HAFU);

    wire JUSA_Q = JUSA.tock(KYGY, HATO, in.D0);
    wire JUZY_Q = JUZY.tock(KYGY, HATO, in.D1);
    wire JOMA_Q = JOMA.tock(KYGY, HATO, in.D2);

    wire JYNE = not(!JUSA_Q);
    wire JACA = not(!JUZY_Q);
    wire JOKU = not(!JOMA_Q);

    if (HAMY) {
      out.D_OE = true;
      out.D0 = JYNE;
      out.D1 = JACA;
      out.D2 = JOKU;
    }

    //----------
    // FF13 NR13

    wire FULO = nor(in.DYFA_1MHZ, in.GEXU);
    wire EPYK = nor(in.FEKU, in.COPE);
    wire GEKU = not(FULO);
    wire FUME = not(EPYK);

    // FIXME daisy chain
    wire GAXE_Q = GAXE.count(GEKU,     FUME, in.HYKA3);
    wire HYFE_Q = HYFE.count(GAXE.q(), FUME, in.JYKA3);
    wire JYTY_Q = JYTY.count(HYFE.q(), FUME, in.HAVO3);
    wire KYNA_Q = KYNA.count(JYTY.q(), FUME, in.EDUL3);

    wire DEGA = not(EPYK);
    wire KYPE = not(KYNA.q());

    wire JEMA_Q = JEMA.count(KYPE,     DEGA, in.FELY3);
    wire HYKE_Q = HYKE.count(JEMA.q(), DEGA, in.HOLU3);
    wire FEVA_Q = FEVA.count(HYKE.q(), DEGA, in.HYXU3);
    wire EKOV_Q = EKOV.count(FEVA.q(), DEGA, in.HOPO3);

    wire DAKO = not(EPYK);
    wire DERU = not(EKOV.q());

    wire EMUS_Q = EMUS.count(DERU,     DAKO, in.DYGY3);
    wire EVAK_Q = EVAK.count(EMUS.q(), DAKO, in.EVAB3);
    wire COPU_Q = COPU.count(EVAK.q(), DAKO, in.AXAN3);

    wire FORU = not(!GAXE_Q);
    wire GEFU = not(!HYFE_Q);
    wire KYVU = not(!JYTY_Q);
    wire KUMO = not(!KYNA_Q);
    wire KARY = not(!JEMA_Q);
    wire GODE = not(!HYKE_Q);
    wire GOJE = not(!FEVA_Q);
    wire FOZE = not(!EKOV_Q);

    wire CEGE = not(in.CPU_RDn);
    wire DAXA = nand(CEGE, in.NET03);
    wire CACA = not(in.FF13);
    wire DYPU = nor(DAXA, CACA);
    wire EVAJ = not(DYPU);

    if (EVAJ) {
      out.D_OE = true;
      out.D0 = FORU;
      out.D1 = GEFU;
      out.D2 = KYVU;
      out.D3 = KUMO;
      out.D4 = KARY;
      out.D5 = GODE;
      out.D6 = GOJE;
      out.D7 = FOZE;
    }

    wire CURE = not(in.FF14);
    wire DUPY = or(CURE, DAXA);

    wire DOPA = not(!EMUS_Q);
    wire DEMU = not(!EVAK_Q);
    wire DEXO = not(!COPU_Q);

    if (DUPY) {
      out.D_OE = true;
      out.D0 = DOPA;
      out.D1 = DEMU;
      out.D2 = DEXO;
    }

    //----------
    // FF14 NR14

    wire BAGE = nand(in.ANUJ, in.FF14);
    wire CAMY = not(in.APU_RESET);
    wire BUDA = not(in.CPU_RD);
    wire BALE = nand(BUDA, in.FF14);
    wire BOKO_Q = BOKO.tock(BAGE, CAMY, in.D6);

    out.FF14_D6  = BOKO_Q;
    out.FF14_D6n = !BOKO_Q;

    wire BYTU = not(!BOKO_Q);
    if (BALE) {
      out.D_OE = true;
      out.D6 = BYTU;
    }
  }
};