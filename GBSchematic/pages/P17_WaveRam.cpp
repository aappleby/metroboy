#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P17_WaveRam {
  struct Input {
    bool AMUK_4MHZ;
    bool APU_RESET;

    bool CPU_RD;
    bool CPU_RDn;
    bool APU_WR;

    bool FF3X;
    bool BUKE;
    bool GASE;
    bool CH3_ACTIVE;
    bool EFAR_Q;

    bool WAVE_RD_D0;
    bool WAVE_RD_D1;
    bool WAVE_RD_D2;
    bool WAVE_RD_D3;
    bool WAVE_RD_D4;
    bool WAVE_RD_D5;
    bool WAVE_RD_D6;
    bool WAVE_RD_D7;
  };

  struct Output {
    bool CYBO_4MHZ;

    bool WAVE_RAM_CTRL1;
    bool WAVE_RAM_RDn;
    bool WAVE_RAM_WR;
    bool WAVE_RAM_WRn;

    bool WAVE_PLAY_D0;
    bool WAVE_PLAY_D1;
    bool WAVE_PLAY_D2;
    bool WAVE_PLAY_D3;
  
    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // Wave ram sample temp storage
  reg CYFO,CESY,BUDY,BEGU,CUVO,CEVO,BORA,BEPA;

  // 4 bit shift register
  reg BUSA,BANO,AZUS,AZET;

  void tick(const Input& in, Output& out) {

    //----------

    wire BOKE = not(in.CPU_RDn);
    wire BENA = nand(BOKE, in.FF3X);
    wire BYZA = and(in.APU_WR, in.FF3X);
    wire CAZU = not(BENA);
    wire AMYT = not(BYZA);

    out.WAVE_RAM_WR = BYZA;
    out.WAVE_RAM_RDn = CAZU;
    out.WAVE_RAM_WRn = AMYT;

    //----------

    wire BAMA = not(in.APU_RESET);
    wire ARUC = not(in.AMUK_4MHZ);
    wire CYBO = not(in.AMUK_4MHZ);
    wire COZY = not(in.AMUK_4MHZ);

    out.CYBO_4MHZ = CYBO;

    wire ABUR = not(in.BUKE);
    wire BORY = not(ABUR);
    wire BETA = or(out.WAVE_RAM_WR, out.WAVE_RAM_RDn, BORY);
    wire AZOR = not(BETA);
    wire BUKU = not(AZOR);

    // Schematic makes this look like a clock divider, but it's a 4 bit shift register.
    wire BUSA_Q = BUSA.tock(in.AMUK_4MHZ, BAMA, in.GASE);
    wire BANO_Q = BANO.tock(COZY,         BAMA, BUSA_Q);
    wire AZUS_Q = AZUS.tock(in.AMUK_4MHZ, BAMA, BANO_Q);
    wire AZET_Q = AZET.tock(ARUC,         BAMA, AZUS_Q);

    wire BUTU = not(AZUS_Q);
    // this net name is wrong, this is not a divided clock
    wire BUTU_512KHZ = BUTU;

    wire BOXO = nor(AZUS_Q, AZET_Q);
    wire BORU = not(BOXO);

    wire ATUR = mux2(BORU,BUKU,in.CH3_ACTIVE);
    wire ALER = not(ATUR);
    out.WAVE_RAM_CTRL1 = ALER;


    wire BEKA = not(BUTU_512KHZ);
    wire COJU = not(BUTU_512KHZ);
    wire BAJA = not(BUTU_512KHZ);
    wire BUFE = not(BUTU_512KHZ);

    // unused on schematic
    (void)BEKA;
    (void)COJU;
    (void)BAJA;
    (void)BUFE;

    //----------

    wire AKAF = not(in.WAVE_RD_D7);
    wire CUTO = not(in.WAVE_RD_D6);
    wire BERO = not(in.WAVE_RD_D5);
    wire BACA = not(in.WAVE_RD_D4);
    wire ADOK = not(in.WAVE_RD_D3);
    wire ATEC = not(in.WAVE_RD_D2);
    wire CEGU = not(in.WAVE_RD_D1);
    wire CUGO = not(in.WAVE_RD_D0);

    wire BEZU = not(AKAF);
    wire DESA = not(CUTO);
    wire BAVA = not(BERO);
    wire BUNE = not(BACA);
    wire BADE = not(ADOK);
    wire BATY = not(ATEC);
    wire DESY = not(CEGU);
    wire DUGU = not(CUGO);

    // uhhhh polarity? this is probably wrong...
    if (out.WAVE_RAM_RDn) {
      out.D_OE= true;
      out.D7 = BEZU;
      out.D6 = DESA;
      out.D5 = BAVA;
      out.D4 = BUNE;
      out.D3 = BADE;
      out.D2 = BATY;
      out.D1 = DESY;
      out.D0 = DUGU;
    }

    wire ACOR = not(in.APU_RESET);

    wire BEPA_Q = BEPA.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D7);
    wire BORA_Q = BORA.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D6);
    wire CEVO_Q = CEVO.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D5);
    wire CUVO_Q = CUVO.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D4);
    wire BEGU_Q = BEGU.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D3);
    wire BUDY_Q = BUDY.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D2);
    wire CESY_Q = CESY.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D1);
    wire CYFO_Q = CYFO.tock(BUTU_512KHZ, ACOR, in.WAVE_RD_D0);

    // why are these using the inverted output of the ff?
    wire COPO = mux2(!BEGU_Q, !BEPA_Q, in.EFAR_Q);
    wire CUZO = mux2(!BUDY_Q, !BORA_Q, in.EFAR_Q);
    wire DAZY = mux2(!CESY_Q, !CEVO_Q, in.EFAR_Q);
    wire DATE = mux2(!CYFO_Q, !CUVO_Q, in.EFAR_Q);

    out.WAVE_PLAY_D3 = COPO;
    out.WAVE_PLAY_D2 = CUZO;
    out.WAVE_PLAY_D1 = DAZY;
    out.WAVE_PLAY_D0 = DATE;
  }

};