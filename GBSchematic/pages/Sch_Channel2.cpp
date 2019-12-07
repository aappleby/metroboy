#include "Sch_Channel2.h"
#include "Sch_Gameboy.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P14_Ch2Regs_tick(const ChipIn& chip_in, const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {
  (void)chip_in;
  (void)cpu_in;

  /*p15.AZEG*/ c.ch2.AZEG = not(b.apu.CLK_xBxDxFxH1);

  /*p14.HUDE*/ c.ch2.APU_RESETn1 = not(b.apu.APU_RESET1);

  /*p14.JYBU*/ c.ch2.JYBU = not(b.apu.APU_RESET1);
  /*p14.KYPU*/ c.ch2.KYPU = not(b.apu.APU_RESET1);
  /*p14.FAZO*/ c.ch2.FAZO = not(b.apu.APU_RESET1);
  /*p15.KATY*/ c.ch2.KATY = not(b.apu.APU_RESET1);
  /*p15.CYWU*/ c.ch2.CYWU = not(b.apu.APU_RESET1);
  /*p15.CEXE*/ c.ch2.CEXE = not(b.apu.APU_RESET1);
  /*p15.BUWE*/ c.ch2.BUWE = not(b.apu.APU_RESET1);

  //----------
  // FF16 NR21

  {
    /*p10.DAZA*/ wire ADDR_0110an = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx0);
    /*p10.COVY*/ wire ADDR_FF16  = nor(b.apu.ADDR_FF1Xn, ADDR_0110an);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p14.BYGO*/ c.ch2.BYGO = not(CPU_RDn);
    /*p14.CORO*/ c.ch2.CORO = nand(ADDR_FF16, b.ch2.BYGO);
    /*p14.AGYN*/ c.ch2.FF16_WR = nand(b.apu.APU_WR, ADDR_FF16); // FF16_WR
    /*p14.ASYP*/ c.ch2.ASYP = not(b.ch2.FF16_WR);
    /*p14.BENY*/ c.ch2.BENY = nor(b.ch2.ASYP, b.apu.APU_RESET1, b.ch2.ELOX);
    /*p14.BACU*/ c.ch2.BACU = and(ADDR_FF16, b.apu.APU_WR);
    /*p14.BUDU*/ c.ch2.BUDU = not(b.ch2.BACU);

    {
      /*p09.AFAT*/ wire APU_RESETn = not(b.apu.APU_RESET1);
      /*p14.BERA*/ c.ch2.NR21_DUTY0 = tock_pos(a.ch2.BUDU, b.ch2.BUDU, APU_RESETn, b.ch2.NR21_DUTY0, b.D6);
      /*p14.BAMY*/ c.ch2.NR21_DUTY1 = tock_pos(a.ch2.BUDU, b.ch2.BUDU, APU_RESETn, b.ch2.NR21_DUTY1, b.D7);
    }

    /*p14.CEKA*/ if (b.ch2.CORO) c.D7 = b.ch2.NR21_DUTY0;
    /*p14.CECY*/ if (b.ch2.CORO) c.D6 = b.ch2.NR21_DUTY1;
  }

  //----------
  // FF17 NR22

  {
    /*p10.DUVU*/ wire ADDR_0111an = nand(b.apu.ADDR_0xxx, b.apu.ADDR_x1xx, b.apu.ADDR_xx1x, b.apu.ADDR_xxx1); 
    /*p10.DUTU*/ wire ADDR_FF17  = nor(b.apu.ADDR_FF1Xn, ADDR_0111an);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p14.ENUF*/ c.ch2.ENUF = and(ADDR_FF17, b.apu.APU_WR);
    /*p14.FYRY*/ c.ch2.FYRY = not(ADDR_FF17);
    /*p14.GURU*/ c.ch2.GURU = or(b.ch2.FYRY, CPU_RDn);
    /*p14.GURE*/ c.ch2.GURE = not(ADDR_FF17);
    /*p14.GEXA*/ c.ch2.GEXA = or(b.ch2.GURE, CPU_RDn);
    /*p14.GERE*/ c.ch2.GERE = and(b.apu.APU_WR, ADDR_FF17);
    /*p14.JEDE*/ c.ch2.JEDE = not(b.ch2.GERE);

    /*p14.HYFU*/ c.ch2.NR22_ENV_TIMER0 = tock_pos(a.ch2.JEDE, b.ch2.JEDE, b.ch2.JYBU, b.ch2.NR22_ENV_TIMER0, b.D0);
    /*p14.HAVA*/ c.ch2.NR22_ENV_TIMER1 = tock_pos(a.ch2.JEDE, b.ch2.JEDE, b.ch2.JYBU, b.ch2.NR22_ENV_TIMER1, b.D1);
    /*p14.HORE*/ c.ch2.NR22_ENV_TIMER2 = tock_pos(a.ch2.JEDE, b.ch2.JEDE, b.ch2.JYBU, b.ch2.NR22_ENV_TIMER2, b.D2);
    /*p14.FORE*/ c.ch2.FF17_D3 = tock_pos(a.ch2.ENUF, b.ch2.ENUF, b.ch2.JYBU, b.ch2.FF17_D3, b.D3);
    /*p14.GATA*/ c.ch2.FF17_D4 = tock_pos(a.ch2.ENUF, b.ch2.ENUF, b.ch2.JYBU, b.ch2.FF17_D4, b.D4);
    /*p14.GUFE*/ c.ch2.FF17_D5 = tock_pos(a.ch2.ENUF, b.ch2.ENUF, b.ch2.JYBU, b.ch2.FF17_D5, b.D5);
    /*p14.GURA*/ c.ch2.FF17_D6 = tock_pos(a.ch2.ENUF, b.ch2.ENUF, b.ch2.JYBU, b.ch2.FF17_D6, b.D6);
    /*p14.GAGE*/ c.ch2.FF17_D7 = tock_pos(a.ch2.ENUF, b.ch2.ENUF, b.ch2.JYBU, b.ch2.FF17_D7, b.D7);

    /*p14.HUVU*/ if (b.ch2.GEXA) c.D0 = b.ch2.NR22_ENV_TIMER0;
    /*p14.HYRE*/ if (b.ch2.GEXA) c.D1 = b.ch2.NR22_ENV_TIMER1;
    /*p14.HAVU*/ if (b.ch2.GEXA) c.D2 = b.ch2.NR22_ENV_TIMER2;
    /*p14.GENE*/ if (b.ch2.GURU) c.D3 = b.ch2.FF17_D3;
    /*p14.HUPE*/ if (b.ch2.GURU) c.D4 = b.ch2.FF17_D4;
    /*p14.HERE*/ if (b.ch2.GURU) c.D5 = b.ch2.FF17_D5;
    /*p14.HORO*/ if (b.ch2.GURU) c.D6 = b.ch2.FF17_D6;
    /*p14.HYRY*/ if (b.ch2.GURU) c.D7 = b.ch2.FF17_D7;
  }

  //----------
  // FF18 NR23

  {

    /*p10.DAFY*/ wire ADDR_1000an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx0);
    /*p10.DARA*/ wire ADDR_FF18  = nor(b.apu.ADDR_FF1Xn, ADDR_1000an);

    /*p10.DEJY*/ wire ADDR_1001an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.DOZA*/ wire ADDR_FF19  = nor(b.apu.ADDR_FF1Xn, ADDR_1001an);

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    /*p14.FOGE*/ c.ch2.FOGE = not(CPU_RDn);
    /*p14.FAPE*/ c.ch2.FAPE = and(b.ch2.FOGE, b.apu.NR52_DBG_APU);

    /*p14.GOTE*/ c.ch2.GOTE = not(ADDR_FF19);
    /*p14.HYPO*/ c.ch2.HYPO = or(b.ch2.GOTE, b.ch2.FAPE);

    /*p14.DOSA*/ c.ch2.NR23_WR1 = and(ADDR_FF18, b.apu.APU_WR);
    /*p14.EXUC*/ c.ch2.NR23_WR2 = and(ADDR_FF18, b.apu.APU_WR);

    /*p14.ESUR*/ c.ch2.NR23_WRn1 = not(b.ch2.NR23_WR1);
    /*p14.FYXO*/ c.ch2.NR23_WRn2 = not(b.ch2.NR23_WR2);

    /*p14.FERY*/ c.ch2.FERY = not(b.apu.ADDR_1xxx);
    /*p14.GUZA*/ c.ch2.GUZA = nor(b.ch2.FERY, b.ch2.FAPE);
    /*p14.FUTY*/ c.ch2.FUTY = not(b.ch2.GUZA);

    /*p14.FOFE*/ c.ch2.NR23_FREQ0 = tock_pos(a.ch2.NR23_WRn1, b.ch2.NR23_WRn1, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ0, b.D0);
    /*p14.FOVA*/ c.ch2.NR23_FREQ1 = tock_pos(a.ch2.NR23_WRn1, b.ch2.NR23_WRn1, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ1, b.D1);
    /*p14.FEDY*/ c.ch2.NR23_FREQ2 = tock_pos(a.ch2.NR23_WRn1, b.ch2.NR23_WRn1, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ2, b.D2);
    /*p14.FOME*/ c.ch2.NR23_FREQ3 = tock_pos(a.ch2.NR23_WRn1, b.ch2.NR23_WRn1, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ3, b.D3);
    /*p14.FORA*/ c.ch2.NR23_FREQ4 = tock_pos(a.ch2.NR23_WRn1, b.ch2.NR23_WRn1, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ4, b.D4);
    /*p14.GODA*/ c.ch2.NR23_FREQ5 = tock_pos(a.ch2.NR23_WRn2, b.ch2.NR23_WRn2, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ5, b.D5);
    /*p14.GUMY*/ c.ch2.NR23_FREQ6 = tock_pos(a.ch2.NR23_WRn2, b.ch2.NR23_WRn2, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ6, b.D6);
    /*p14.GUPU*/ c.ch2.NR23_FREQ7 = tock_pos(a.ch2.NR23_WRn2, b.ch2.NR23_WRn2, b.ch2.APU_RESETn1, b.ch2.NR23_FREQ7, b.D7);

    /*p14.FAVA*/ if (b.ch2.FUTY) c.D0 = b.ch2.CH2_FREQ_00;
    /*p14.FAJY*/ if (b.ch2.FUTY) c.D1 = b.ch2.CH2_FREQ_01;
    /*p14.FEGU*/ if (b.ch2.FUTY) c.D2 = b.ch2.CH2_FREQ_02;
    /*p14.FOSE*/ if (b.ch2.FUTY) c.D3 = b.ch2.CH2_FREQ_03;
    /*p14.GERO*/ if (b.ch2.FUTY) c.D5 = b.ch2.CH2_FREQ_04; // d4 and d5 are switched on the schematic
    /*p14.GAKY*/ if (b.ch2.FUTY) c.D4 = b.ch2.CH2_FREQ_05;
    /*p14.GADU*/ if (b.ch2.FUTY) c.D6 = b.ch2.CH2_FREQ_06;
    /*p14.GAZO*/ if (b.ch2.FUTY) c.D7 = b.ch2.CH2_FREQ_07;
  }

  //----------
  // FF19 NR24

  {
    /*p10.DEJY*/ wire ADDR_1001an = nand(b.apu.ADDR_1xxx, b.apu.ADDR_x0xx, b.apu.ADDR_xx0x, b.apu.ADDR_xxx1); 
    /*p10.DOZA*/ wire ADDR_FF19  = nor(b.apu.ADDR_FF1Xn, ADDR_1001an);

    c.ch2.JENU = and(ADDR_FF19, b.apu.APU_WR);
    c.ch2.KYSA = not(b.ch2.JENU);
    c.ch2.NR24_FREQ8  = tock_pos(a.ch2.KYSA, b.ch2.KYSA, b.ch2.KYPU, b.ch2.NR24_FREQ8,  b.D0);
    c.ch2.NR24_FREQ9  = tock_pos(a.ch2.KYSA, b.ch2.KYSA, b.ch2.KYPU, b.ch2.NR24_FREQ9,  b.D1);
    c.ch2.NR24_FREQ10 = tock_pos(a.ch2.KYSA, b.ch2.KYSA, b.ch2.KYPU, b.ch2.NR24_FREQ10, b.D2);
    c.ch2.NR24_STOP   = tock_pos(a.ch2.EVYF, b.ch2.EVYF, b.ch2.FAZO, b.ch2.NR24_STOP, b.D6);
    c.ch2.NR24_START  = tock_pos(a.ch2.DETA, b.ch2.DETA, b.ch2.DERA, b.ch2.NR24_START, b.D7);

    c.ch2.DETA = nand(b.apu.APU_WR, ADDR_FF19);

    /*p14.HUNA*/ if (b.ch2.HYPO) c.D0 = b.ch2.CH2_FREQ_08;
    /*p14.JARO*/ if (b.ch2.HYPO) c.D1 = b.ch2.CH2_FREQ_09;
    /*p14.JEKE*/ if (b.ch2.HYPO) c.D2 = b.ch2.CH2_FREQ_10;

    if (b.ch2.HUMA) c.D6 = b.ch2.GOJY;

    /*p09.AGUZ*/ wire CPU_RDn = not(b.sys.CPU_RD);
    c.ch2.GADO = not(CPU_RDn);
    c.ch2.EVYF = nor(b.apu.CPU_WR_WEIRD, ADDR_FF19);
    c.ch2.HUMA = nor(ADDR_FF19, b.ch2.GADO);
  }
  


  //----------
  // reset thing

  /*p15.ELOX*/ c.ch2.ELOX = tock_pos(a.ch2.CEMO, b.ch2.CEMO, b.ch2.DOXA, b.ch2.ELOX, b.ch2.DALA);
  /*p15.DORY*/ c.ch2.DORY = tock_pos(a.ch2.CEMO, b.ch2.CEMO, b.ch2.CEXE, b.ch2.DORY, b.ch2.ELOX);
  /*p15.DOXA*/ c.ch2.DOXA = or(b.apu.APU_RESET1, b.ch2.DORY);

  //----------

  /*p15.BUFO*/ c.ch2.BUFO = not(b.ch2.ATEP);
  /*p15.BODO*/ c.ch2.BODO = not(b.ch2.CAZA);
  /*p15.CEMO*/ c.ch2.CEMO = tock_pos(a.ch2.BUFO, b.ch2.BUFO, b.ch2.BYHO, b.ch2.CEMO, !b.ch2.CEMO);
  /*p15.BUTA*/ c.ch2.BUTA = or(b.ch2.BODO, b.ch2.ARES);
  /*p15.CAMA*/ c.ch2.CAMA = nor(b.ch2.CEMO, b.ch2.BUTA);
  /*p15.DOCA*/ c.ch2.DOCA = not(b.ch2.CAMA);

  /*p15.ETUK*/ c.ch2.ETUK = not(b.ch2.GYKO);
  /*p15.DAVU*/ c.ch2.DAVU = not(b.ch2.ETUK);

  /*p15.DUJU*/ c.ch2.DUJU = nor(b.ch2.DAVU, b.ch2.ELOX);
  /*p15.COGU*/ c.ch2.COGU = not(b.ch2.DUJU);
  /*p15.EROG*/ c.ch2.EROG = not(b.ch2.DUJU);
  /*p15.GYPA*/ c.ch2.GYPA = not(b.ch2.DUJU);

  /*p14.DONE*/ c.ch2.CH2_FREQ_00 = count_pos(a.ch2.DOCA,         b.ch2.DOCA,         b.ch2.COGU, b.ch2.CH2_FREQ_00, b.ch2.NR23_FREQ0);
  /*p14.DYNU*/ c.ch2.CH2_FREQ_01 = count_pos(a.ch2.CH2_FREQ_00,  b.ch2.CH2_FREQ_00,  b.ch2.COGU, b.ch2.CH2_FREQ_01, b.ch2.NR23_FREQ1);
  /*p14.EZOF*/ c.ch2.CH2_FREQ_02 = count_pos(a.ch2.CH2_FREQ_01,  b.ch2.CH2_FREQ_01,  b.ch2.COGU, b.ch2.CH2_FREQ_02, b.ch2.NR23_FREQ2);
  /*p14.CYVO*/ c.ch2.CH2_FREQ_03 = count_pos(a.ch2.CH2_FREQ_02,  b.ch2.CH2_FREQ_02,  b.ch2.COGU, b.ch2.CH2_FREQ_03, b.ch2.NR23_FREQ3);
  /*p14.EDEP*/ c.ch2.CH2_FREQ_03b = not(!b.ch2.CH2_FREQ_03);
  /*p14.FUXO*/ c.ch2.CH2_FREQ_04 = count_pos(a.ch2.CH2_FREQ_03b, b.ch2.CH2_FREQ_03b, b.ch2.EROG, b.ch2.CH2_FREQ_04, b.ch2.NR23_FREQ4);
  /*p14.GANO*/ c.ch2.CH2_FREQ_05 = count_pos(a.ch2.CH2_FREQ_04,  b.ch2.CH2_FREQ_04,  b.ch2.EROG, b.ch2.CH2_FREQ_05, b.ch2.NR23_FREQ5);
  /*p14.GOCA*/ c.ch2.CH2_FREQ_06 = count_pos(a.ch2.CH2_FREQ_05,  b.ch2.CH2_FREQ_05,  b.ch2.EROG, b.ch2.CH2_FREQ_06, b.ch2.NR23_FREQ6);
  /*p14.GANE*/ c.ch2.CH2_FREQ_07 = count_pos(a.ch2.CH2_FREQ_06,  b.ch2.CH2_FREQ_06,  b.ch2.EROG, b.ch2.CH2_FREQ_07, b.ch2.NR23_FREQ7);
  /*p14.GALA*/ c.ch2.CH2_FREQ_07b = not(!b.ch2.CH2_FREQ_07);
  /*p14.HEVY*/ c.ch2.CH2_FREQ_08 = count_pos(a.ch2.CH2_FREQ_07b, b.ch2.CH2_FREQ_07b, b.ch2.GYPA, c.ch2.CH2_FREQ_08, b.ch2.NR24_FREQ8);
  /*p14.HEPU*/ c.ch2.CH2_FREQ_09 = count_pos(a.ch2.CH2_FREQ_08,  b.ch2.CH2_FREQ_08,  b.ch2.GYPA, c.ch2.CH2_FREQ_09, b.ch2.NR24_FREQ9);
  /*p14.HERO*/ c.ch2.CH2_FREQ_10 = count_pos(a.ch2.CH2_FREQ_09,  b.ch2.CH2_FREQ_09,  b.ch2.GYPA, c.ch2.CH2_FREQ_10, b.ch2.NR24_FREQ10);

  /*p15.FUJY*/ c.ch2.FUJY = and(b.ch2.GYKO, b.ch2.CEMO);
  /*p15.GALU*/ c.ch2.GALU = not(b.ch2.CH2_FREQ_10);
  /*p15.GYRE*/ c.ch2.GYRE = nor(b.apu.APU_RESET1, b.ch2.ELOX, b.ch2.FUJY);
  /*p15.GYKO*/ c.ch2.GYKO = tock_pos(a.ch2.GALU, b.ch2.GALU, b.ch2.GYRE, b.ch2.GYKO, !b.ch2.GYKO);

  //----------

  /*p15.HOTA*/ c.ch2.HOTA = not(b.apu.CLK_128a);
  /*p15.JYNA*/ c.ch2.JYNA = tock_pos(a.ch2.HOTA, b.ch2.HOTA, b.ch2.KATY, b.ch2.JYNA, !b.ch2.JYNA);
  /*p15.KYLO*/ c.ch2.KYLO = not(b.ch2.JYNA);
  /*p15.KENE*/ c.ch2.KENE = not(b.ch2.KYLO);
  /*p15.JORE*/ c.ch2.JORE = count_pos(a.ch2.KENE, b.ch2.KENE, b.ch2.JAKE, b.ch2.JORE, !b.ch2.NR22_ENV_TIMER0);
  /*p15.JONA*/ c.ch2.JONA = count_pos(a.ch2.JORE, b.ch2.JORE, b.ch2.JAKE, b.ch2.JONA, !b.ch2.NR22_ENV_TIMER1);
  /*p15.JEVY*/ c.ch2.JEVY = count_pos(a.ch2.JONA, b.ch2.JONA, b.ch2.JAKE, b.ch2.JEVY, !b.ch2.NR22_ENV_TIMER2);
  /*p15.KYVO*/ c.ch2.KYVO = and(!b.ch2.JORE, b.ch2.JONA, b.ch2.JEVY);
  /*p15.JOPA*/ c.ch2.JOPA = tock_pos(a.apu.CLK_512a, b.apu.CLK_512a, b.ch2.HAFE, b.ch2.JOPA, b.ch2.KYVO);
  /*p15.HEPO*/ c.ch2.HEPO = tock_pos(a.ch2.JOPA,     b.ch2.JOPA,     b.ch2.HYPA, b.ch2.HEPO, b.ch2.GUFY);
  /*p15.DOPE*/ c.ch2.DOPE = tock_pos(a.sys.PHASE_ABCDxxxx7c, b.sys.PHASE_ABCDxxxx7c, b.ch2.CYWU, b.ch2.DOPE, b.ch2.NR24_START);
  /*p15.DERA*/ c.ch2.DERA = nor(b.apu.APU_RESET1, b.ch2.DOPE);
  /*p15.DALA*/ c.ch2.DALA = or(b.ch2.CELO, b.ch2.DOPE);
  /*p15.CELO*/ c.ch2.CELO = not(b.ch2.DOXA);
  /*p15.HYLE*/ c.ch2.HYLE = or(b.apu.APU_RESET1, b.ch2.ELOX);
  /*p15.HYPA*/ c.ch2.HYPA = nor(b.ch2.ELOX, b.apu.APU_RESET1);
  /*p15.HYLY*/ c.ch2.HYLY = nor(b.ch2.ELOX, b.ch2.JOPA);
  /*p15.JAKE*/ c.ch2.JAKE = not(b.ch2.HYLY);
  /*p15.JEME*/ c.ch2.JEME = or(b.ch2.HEPO, b.ch2.HYLE);
  /*p15.ATEP*/ c.ch2.ATEP = tock_pos(a.ch2.AZEG, b.ch2.AZEG, b.ch2.BUWE, b.ch2.ATEP, !b.ch2.ATEP);
  /*p15.CAZA*/ c.ch2.CAZA = tock_pos(a.ch2.CEMO, b.ch2.CEMO, b.ch2.CEXE, b.ch2.CAZA, b.ch2.DORY);
  /*p15.BYHO*/ c.ch2.BYHO = not(b.ch2.BUWE);

  /*p15.DEME*/ c.ch2.DEME = or(b.ch2.CYRE, b.apu.CLK_256a, b.ch2.NR24_STOP);
  /*p15.DORA*/ c.ch2.DORA = and(b.ch2.CYRE, b.ch2.NR24_STOP);
  /*p15.FUTE*/ c.ch2.CH2_AMP_ENn = nor(b.ch2.FF17_D3,b.ch2.FF17_D4,b.ch2.FF17_D5,b.ch2.FF17_D6,b.ch2.FF17_D7);
  /*p15.DYRO*/ c.ch2.DYRO = not(b.ch2.DEME);
  /*p15.ESYK*/ c.ch2.ESYK = or(b.apu.APU_RESET1, b.ch2.DORA, b.ch2.CH2_AMP_ENn);
  /*p15.ARES*/ c.ch2.ARES = nand(b.apu.APU_RESET1, b.ch2.CH2_AMP_ENn);
  /*p15.DANE*/ c.ch2.DANE = or(b.ch2.ELOX, b.ch2.ESYK);
  /*p15.DEFU*/ c.ch2.CH2_ACTIVEn = not(b.ch2.DANE);
  /*p15.BYMO*/ c.ch2.BYMO = not(b.ch2.FF16_WR);
  /*p15.AGET*/ c.ch2.AGET = not(b.ch2.FF16_WR);
  
  
  /*p15.ERYC*/ c.ch2.ERYC_00 = count_pos(a.ch2.DYRO,     b.ch2.DYRO,     b.ch2.BYMO, b.ch2.ERYC_00, b.D0);
  /*p15.CERA*/ c.ch2.CERA_01 = count_pos(a.ch2.ERYC_00,  b.ch2.ERYC_00,  b.ch2.BYMO, b.ch2.CERA_01, b.D1);
  /*p15.CONU*/ c.ch2.CONU_02 = count_pos(a.ch2.CERA_01,  b.ch2.CERA_01,  b.ch2.BYMO, b.ch2.CONU_02, b.D2);
  /*p15.CAME*/ c.ch2.CAME_03 = count_pos(a.ch2.CONU_02,  b.ch2.CONU_02,  b.ch2.BYMO, b.ch2.CAME_03, b.D3);
  /*p15.BUKO*/ c.ch2.BUKO    = not(!b.ch2.CAME_03);
  /*p15.BUVA*/ c.ch2.BUVA_04 = count_pos(a.ch2.BUKO,     b.ch2.BUKO,     b.ch2.AGET, b.ch2.BUVA_04, b.D4);
  /*p15.AKYD*/ c.ch2.AKYD_05 = count_pos(a.ch2.BUVA_04,  b.ch2.BUVA_04,  b.ch2.AGET, b.ch2.AKYD_05, b.D5);
  /*p15.CYRE*/ c.ch2.CYRE    = tock_pos(!a.ch2.AKYD_05,  !b.ch2.AKYD_05, b.ch2.BENY, b.ch2.CYRE,    !b.ch2.CYRE);

  /*p15.GADE*/ c.ch2.GADE = not(b.ch2.JOPA);
  /*p15.HOLY*/ c.ch2.HOLY = nor(b.apu.CLK_512a, b.ch2.GADE);
  /*p15.JUPU*/ c.ch2.JUPU = nor(b.ch2.NR22_ENV_TIMER0, b.ch2.NR22_ENV_TIMER1, b.ch2.NR22_ENV_TIMER2);
  /*p15.HOFO*/ c.ch2.HOFO = or(b.ch2.JOPA, b.ch2.JUPU, b.ch2.JEME);
  /*p15.HAFE*/ c.ch2.HAFE = or(b.ch2.HOLY, b.ch2.ELOX, b.apu.APU_RESET1);

  {
    /*p15.CULE*/ c.ch2.CULE = not(b.ch2.DAVU);
    /*p09.AFAT*/ wire APU_RESETn = not(b.apu.APU_RESET1);
    /*p15.CANO*/ c.ch2.CANO_00 = tock_pos(a.ch2.CULE,     b.ch2.CULE,     APU_RESETn, b.ch2.CANO_00, !b.ch2.CANO_00);
    /*p15.CAGY*/ c.ch2.CAGY_01 = tock_pos(!a.ch2.CANO_00, !b.ch2.CANO_00, APU_RESETn, b.ch2.CAGY_01, !b.ch2.CAGY_01);
    /*p15.DYVE*/ c.ch2.DYVE_02 = tock_pos(!a.ch2.CAGY_01, !b.ch2.CAGY_01, APU_RESETn, b.ch2.DYVE_02, !b.ch2.DYVE_02);
  }

  /*p15.DYMU*/ c.ch2.DYMU = and(b.ch2.DYVE_02, b.ch2.CAGY_01);
  /*p15.DUGE*/ c.ch2.DUGE = not(b.ch2.CANO_00);
  /*p15.DARE*/ c.ch2.DARE = not(b.ch2.DYMU);
  /*p15.EGOG*/ c.ch2.EGOG = and(b.ch2.DYMU, b.ch2.DUGE);

  /*p15.DOMO*/ c.ch2.DOMO = nor( b.ch2.NR21_DUTY0,  b.ch2.NR21_DUTY1);
  /*p15.DYTA*/ c.ch2.DYTA = nor(!b.ch2.NR21_DUTY0,  b.ch2.NR21_DUTY1);
  /*p15.DOJU*/ c.ch2.DOJU = nor( b.ch2.NR21_DUTY0, !b.ch2.NR21_DUTY1);
  /*p15.DOVE*/ c.ch2.DOVE = nor(!b.ch2.NR21_DUTY0, !b.ch2.NR21_DUTY1);
  /*p15.EXES*/ c.ch2.EXES = amux4(b.ch2.EGOG,    b.ch2.DOMO,
                                  b.ch2.DYMU,    b.ch2.DYTA,
                                  b.ch2.DYVE_02, b.ch2.DOJU,
                                  b.ch2.DARE,    b.ch2.DOVE);



  {
    /*p09.AFAT*/ wire APU_RESETn = not(b.apu.APU_RESET1);
    /*p15.DOME*/ c.ch2.DOME = tock_pos(a.ch2.DAVU, b.ch2.DAVU, APU_RESETn, b.ch2.DOME, b.ch2.EXES);
  }
  /*p15.CYSE*/ c.ch2.CYSE = and(b.ch2.DANE, b.ch2.DOME);
  /*p15.BONU*/ c.ch2.BONU = or(b.ch2.CYSE, b.apu.NR52_DBG_APU);
  /*p15.FOPY*/ c.ch2.FOPY = amux2(b.ch2.FOMY, b.ch2.FF17_D3, !b.ch2.FOMY, !b.ch2.FF17_D3);
  /*p15.ETUP*/ c.ch2.ETUP = amux2(b.ch2.FETE, b.ch2.FF17_D3, !b.ch2.FETE, !b.ch2.FF17_D3);
  /*p15.FARU*/ c.ch2.FARU = amux2(b.ch2.FENO, b.ch2.FF17_D3, !b.ch2.FENO, !b.ch2.FF17_D3);
  /*p15.GAFA*/ c.ch2.GAFA = amux2(b.ch2.HOFO, b.ch2.FF17_D3, b.ch2.HOFO,  !b.ch2.FF17_D3);
  /*p15.FENA*/ c.ch2.FENA = count_pos(a.ch2.FOPY, b.ch2.FOPY, b.ch2.ELOX, b.ch2.FENA, b.ch2.FF17_D7);
  /*p15.FOMY*/ c.ch2.FOMY = count_pos(a.ch2.ETUP, b.ch2.ETUP, b.ch2.ELOX, b.ch2.FOMY, b.ch2.FF17_D6);
  /*p15.FETE*/ c.ch2.FETE = count_pos(a.ch2.FARU, b.ch2.FARU, b.ch2.ELOX, b.ch2.FETE, b.ch2.FF17_D5);
  /*p15.FENO*/ c.ch2.FENO = count_pos(a.ch2.GAFA, b.ch2.GAFA, b.ch2.ELOX, b.ch2.FENO, b.ch2.FF17_D4);
  /*p15.EMYR*/ c.ch2.EMYR = nor (b.ch2.FF17_D3, b.ch2.FENO, b.ch2.FETE, b.ch2.FOMY, b.ch2.FENA);
  /*p15.ERAT*/ c.ch2.ERAT = nand(b.ch2.FF17_D3, b.ch2.FENO, b.ch2.FETE, b.ch2.FOMY, b.ch2.FENA);
  /*p15.FYRE*/ c.ch2.FYRE = not(b.ch2.ERAT);
  /*p15.GUFY*/ c.ch2.GUFY = or(b.ch2.EMYR, b.ch2.FYRE);
  /*p15.ANYV*/ c.ch2.ANYV = and(b.ch2.FETE, b.ch2.BONU);
  /*p15.ANAN*/ c.ch2.ANAN = and(b.ch2.FENO, b.ch2.BONU);
  /*p15.AMOV*/ c.ch2.AMOV = and(b.ch2.FENA, b.ch2.BONU);
  /*p15.ASOG*/ c.ch2.ASOG = and(b.ch2.FOMY, b.ch2.BONU);
}

//-----------------------------------------------------------------------------

};