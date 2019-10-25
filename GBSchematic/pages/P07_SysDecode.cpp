#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P7_Sys_Decode {
  struct Input {
    bool BEDO;
    bool P10_B;
    bool FFXX;
    bool RESET2;
    bool RESET;
    bool CPU_WR_RAW;
    bool ANAP;
    bool NET02; // FIXME this should be T1nT2
    bool WR_IN;
    bool CPU_RD_SYNC;
    bool CPU_RAW_RD;
    bool RD_B;
    bool FROM_CPU6;

    bool T1;
    bool T2;

    bool A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool CPU_RD;
    bool CPU_WR;
    bool CPU_RD2;
    bool CPU_WR2;

    bool PIN_NC;

    bool BOOT_CS;
    bool HRAM_CS;
    bool T1nT2n;
    bool T1nT2;
    bool T1T2n;

    bool FF60_D0;
    bool FF60_D1;

    bool FFXX;
    bool FFXXn;
    bool SARO;
    bool FEXXFFXXn;
    bool FF0F_RD;
    bool FF0F_WR;

    bool BOOTROM_A1A0;
    bool BOOTROM_A1A0n;
    bool BOOTROM_A1nA0;
    bool BOOTROM_A1nA0n;
    bool BOOTROM_A2n;
    bool BOOTROM_A3n;

    bool BOOTROM_A5AA4;
    bool BOOTROM_A5A4n;
    bool BOOTROM_A5nA4;
    bool BOOTROM_A5nA4n;
    bool BOOTROM_A6n;
    bool BOOTROM_A7n;

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF50 - the "enable bootrom" register
  reg TEPU;

  // FF60 - secret debug register
  reg AMUT,BURO;

  void tick(const Input& in, Output& out) {

    wire UBET = not(in.T1);
    wire UVAR = not(in.T2);
    wire UPOJ = nand(UBET, UVAR, in.RESET);
    wire UNOR = and(in.T2, UBET);
    wire UMUT = and(in.T1, UVAR);
    out.T1nT2n= UPOJ;
    out.T1nT2 = UNOR;
    out.T1T2n = UMUT;

    //----------

    bool UBAL = mux2(in.WR_IN, in.CPU_RD_SYNC, out.T1nT2);
    bool UJYV = mux2(in.RD_B, in.CPU_RAW_RD, out.T1nT2);
    bool TAPU = not(UBAL);
    bool TEDO = not(UJYV);

    out.CPU_WR = TAPU;
    out.CPU_RD = TEDO;

    bool DYKY = not(TAPU);
    bool AJAS = not(TEDO);

    bool CUPA = not(DYKY);
    bool ASOT = not(AJAS);

    out.CPU_WR2 = CUPA;
    out.CPU_RD2 = ASOT;

    bool LEXY = not(in.FROM_CPU6);
    out.PIN_NC = LEXY;

    //----------

    bool TYRO = nor(in.A7, in.A5, in.A3, in.A2, in.A1, in.A0);
    bool TUFA = and(in.A4, in.A6);
    bool TUGE = nand(TYRO, TUFA, in.FFXX, out.CPU_WR);
    bool TEPU_Q = TEPU.q();
    bool SATO = or(in.D0, TEPU_Q);
    TEPU.tock(TUGE, in.RESET2, SATO);
    bool TERA = not(TEPU_Q);
    bool TULO = nor(in.A15, in.A14, in.A13, in.A12, in.A11, in.A10, in.A9, in.A8);
    bool TUTU = and(TERA, TULO);
    
    // these two cells are weirdly off by themselves next to wave ram
    bool YAZA = not(out.T1T2n);
    bool YULA = and(YAZA, TUTU, out.CPU_RD);

    bool ZORO = nor(in.A15, in.A14, in.A13, in.A12);
    bool ZADU = nor(in.A11, in.A10, in.A9, in.A8);
    bool ZUFA = and(ZORO, ZADU);
    bool ZADO = nand(YULA, ZUFA);
    bool ZERY = not(ZADO);

    out.BOOT_CS = ZERY;

    bool TEXE = and(out.CPU_RD, in.FFXX, TUFA, TYRO);
    bool SYPU = not(TEPU_Q);
    if (TEXE) {
      out.D_OE = true;
      out.D0 = SYPU;
    }
    //----------

    bool SEMY = nor(in.A7, in.A6, in.A5, in.A4);
    bool SAPA = and(in.A0, in.A1, in.A2, in.A3);
    bool ROLO = and(SEMY, SAPA, in.FFXX, out.CPU_RD);
    bool REFA = and(SEMY, SAPA, in.FFXX, in.CPU_WR_RAW);

    out.FF0F_RD = ROLO;
    out.FF0F_WR = REFA;

    //----------

    bool WALE = nand(in.A0, in.A1, in.A2, in.A3, in.A4, in.A5, in.A6);
    bool WOLY = nand(WALE, in.A7, in.FFXX);
    bool WUTA = not(WOLY);

    out.HRAM_CS = WUTA;

    //----------

    bool LECO = nor(in.BEDO, out.T1nT2);

    bool RARU = not(in.P10_B);
    bool ROWE = not(in.P10_B);
    bool RYKE = not(in.P10_B);
    bool RYNE = not(in.P10_B);
    bool RASE = not(in.P10_B);
    bool REJY = not(in.P10_B);
    bool REKA = not(in.P10_B);
    bool ROMY = not(in.P10_B);

    if (LECO) {
      out.D_OE = true;
      out.D0 = ROMY;
      out.D1 = RYNE;
      out.D2 = REJY;
      out.D3 = RASE;
      out.D4 = REKA;
      out.D5 = ROWE;
      out.D6 = RYKE;
      out.D7 = RARU;
    }

    //----------

    bool TONA = not(in.A8);
    bool TUNA = nand(in.A15, in.A14, in.A13, in.A12, in.A11, in.A10, in.A9);
    out.FEXXFFXXn = TUNA;
    bool SYKE = nor(TONA, TUNA);
    bool RYCU = not(TUNA);
    bool SOHA = not(in.FFXX);
    bool ROPE = nand(RYCU, SOHA);
    out.FFXX = SYKE;
    bool BAKO = not(SYKE);
    out.FFXXn = BAKO;
    out.SARO = not(ROPE);

    //----------
    // bootrom address generation

    bool ZYRA = not(in.A7);
    bool ZAGE = not(in.A6);
    bool ZABU = not(in.A3);
    bool ZOKE = not(in.A2);

    out.BOOTROM_A7n = ZYRA;
    out.BOOTROM_A6n = ZAGE;
    out.BOOTROM_A3n = ZABU;
    out.BOOTROM_A2n = ZOKE;

    bool ZERA = not(in.A5);
    bool ZUFY = not(in.A4);
    bool ZYKY = and(ZERA, ZUFY);
    bool ZYGA = and(ZERA, in.A4);
    bool ZOVY = and(in.A5, ZUFY);
    bool ZUKO = and(in.A5, in.A4);

    out.BOOTROM_A5nA4n = ZYKY;
    out.BOOTROM_A5nA4 = ZYGA;
    out.BOOTROM_A5A4n = ZOVY;
    out.BOOTROM_A5AA4 = ZUKO;

    bool ZUVY = not(in.A1);
    bool ZYBA = not(in.A0);
    bool ZOLE = and(ZUVY, ZYBA);
    bool ZAJE = and(ZUVY, in.A0);
    bool ZUBU = and(ZYBA, in.A1);
    bool ZAPY = and(in.A1, in.A0);

    bool ZETE = not(ZOLE);
    bool ZEFU = not(ZAJE);
    bool ZYRO = not(ZUBU);
    bool ZAPA = not(ZAPY);

    out.BOOTROM_A1nA0n = ZETE;
    out.BOOTROM_A1nA0 = ZEFU;
    out.BOOTROM_A1A0n = ZYRO;
    out.BOOTROM_A1A0 = ZAPA;

    //----------

    bool APET = or(in.NET02, out.T1T2n);
    bool APER = nand(APET, in.A5, in.A6, out.CPU_WR, in.ANAP);
    bool AMUT_Q = AMUT.tock(APER, in.RESET2, in.D1);
    bool BURO_Q = BURO.tock(APER, in.RESET2, in.D0);

    out.FF60_D1= AMUT_Q;
    out.FF60_D0= BURO_Q;
  }};
