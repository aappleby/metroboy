#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P6_SerialLink {
  struct Input {
    bool CLK_16k;
    
    bool SCK_IN;
    bool SCK_DIR;
    bool SIN_IN;

    bool RESET2;

    bool CPU_WR;
    bool CPU_RD;

    bool FFXX;
    bool TOVY_A0n;
    bool TOLA_A1n;

    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool A00_07;
    bool INT_SERIAL;
    bool SER_OUT;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF01 SB
  reg CUBA,DEGU,DYRA,DOJO,DOVU,EJAB,EROD,EDER;

  // FF02 SC
  reg ETAF, CULY;

  // clock divider
  reg COTY;

  // output reg
  reg ELYS;

  // counter that triggers INT_SERIAL
  reg CAFA,CYLO,CYDE,CALY;

  void tick(const Input& in, Output& out) {

    wire SARE = nor(in.A7, in.A7, in.A7, in.A4, in.A3);
    out.A00_07 = SARE;
    wire SEFY = not(in.A2);
    wire SANO = and(SARE, SEFY, in.FFXX);

    wire CALY_Q = CALY.q();
    wire COBA = not(CALY_Q);
    wire CABY = and(COBA, in.RESET2);

    wire UWAM = nand(in.TOVY_A0n, in.A1, in.CPU_WR, SANO);
    wire COTY_Q = COTY.flip(in.CLK_16k, UWAM);
    wire ETAF_Q = ETAF.tock(UWAM, CABY, in.D7);
    wire CULY_Q = CULY.tock(UWAM, in.RESET2, in.D0);
    wire CARO = and(UWAM, in.RESET2);

    wire CAVE = mux2(COTY_Q, in.SCK_IN, CULY_Q);
    wire DAWA = or(CAVE, !ETAF_Q);
    wire ELUV = not(!ETAF_Q);
    wire CORE = not(!CULY_Q);

    wire JAGO = not(in.SCK_DIR);
    wire KEXU = nand(DAWA, in.SCK_DIR);
    wire KUJO = nor(JAGO, DAWA);
    (void)KUJO;
    (void)KEXU;
    wire EDYL = not(DAWA);
    wire SER_TICKn = EDYL;

    // FIXME daisy chain
    wire CAFA_Q = CAFA.flip(DAWA, CARO);
    wire CYLO_Q = CYLO.flip(!CAFA_Q, CARO);
    wire CYDE_Q = CYDE.flip(!CYLO_Q, CARO);
    CALY.flip(!CYDE_Q, CARO);
    out.INT_SERIAL = CALY_Q;

    wire UCOM = nand(SANO, in.CPU_RD, in.A1, in.TOVY_A0n);
    if (UCOM) {
      out.D_OE = true;
      out.D0 = CORE;
      out.D7 = ELUV;
    }

    //----------
    // FF01 SB

    wire URYS = nand(SANO, in.CPU_WR, in.TOLA_A1n, in.A0);
    wire COHY = unk3(URYS, in.D0, in.RESET2);
    wire DUMO = unk3(URYS, in.D1, in.RESET2);
    wire DYBO = unk3(URYS, in.D2, in.RESET2);
    wire DAJU = unk3(URYS, in.D3, in.RESET2);
    wire DYLY = unk3(URYS, in.D4, in.RESET2);
    wire EHUJ = unk3(URYS, in.D5, in.RESET2);
    wire EFAK = unk3(URYS, in.D6, in.RESET2);
    wire EGUV = unk3(URYS, in.D7, in.RESET2);

    wire DAKU = not(URYS);
    wire CUFU = nand(in.D0, DAKU);
    wire DOCU = nand(in.D1, DAKU);
    wire DELA = nand(in.D2, DAKU);
    wire DYGE = nand(in.D3, DAKU);
    wire DOLA = nand(in.D4, DAKU);
    wire ELOK = nand(in.D5, DAKU);
    wire EDEL = nand(in.D6, DAKU);
    wire EFEF = nand(in.D7, DAKU);

    wire EPYT = not(SER_TICKn);
    wire DEHO = not(EPYT);
    wire DAWE = not(DEHO);
    wire CAGE = not(in.SIN_IN);
    wire CUBA_Q = CUBA.srtock(DAWE, CUFU, COHY, CAGE);
    wire DEGU_Q = DEGU.srtock(DAWE, DOCU, DUMO, CUBA_Q);
    wire DYRA_Q = DYRA.srtock(DAWE, DELA, DYBO, DEGU_Q);
    wire DOJO_Q = DOJO.srtock(DAWE, DYGE, DAJU, DYRA_Q);
    wire DOVU_Q = DOVU.srtock(EPYT, DOLA, DYLY, DOJO_Q);
    wire EJAB_Q = EJAB.srtock(EPYT, ELOK, EHUJ, DOVU_Q);
    wire EROD_Q = EROD.srtock(EPYT, EDEL, EFAK, EJAB_Q);
    wire EDER_Q = EDER.srtock(EPYT, EFEF, EGUV, EROD_Q);

    wire CUGY = not(!CUBA_Q);
    wire DUDE = not(!DEGU_Q);
    wire DETU = not(!DYRA_Q);
    wire DASO = not(!DOJO_Q);
    wire DAME = not(!DOVU_Q);
    wire EVOK = not(!EJAB_Q);
    wire EFAB = not(!EROD_Q);
    wire ETAK = not(!EDER_Q);

    wire UFEG = and(SANO, in.CPU_RD, in.TOLA_A1n, in.A0);
    if (UFEG) {
      out.D_OE = true;
      out.D0 = CUGY;
      out.D1 = DUDE;
      out.D2 = DETU;
      out.D3 = DASO;
      out.D4 = DAME;
      out.D5 = EVOK;
      out.D6 = EFAB;
      out.D7 = ETAK;
    }

    //----------

    wire ELYS_Q = ELYS.tock(SER_TICKn, in.RESET2, EDER_Q);
    out.SER_OUT = ELYS_Q;
  }
};
