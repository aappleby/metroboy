#include "P13_Channel1.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P13_Channel1::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // Waveform counter

  c.p13.CALA = not(b.p11.CH1_FREQ_10);
  c.p13.COMY = tock_pos(a.p13.CALA, b.p13.CALA, b.p13.DYRU, b.p13.COMY, !b.p13.COMY);
  
  c.p13.DOKA = and(b.p13.COMY, b.p01.DYFA_1M);
  c.p13.DYRU = nor(b.p09.APU_RESET1, b.p13.FEKU, b.p13.DOKA);
  
  c.p13.CYTE = not(b.p13.COMY);
  c.p13.COPE = not(b.p13.CYTE);
  c.p13.CH1_PHASE_CLKa = not(b.p13.COPE);

  c.p13.CH1_PHASE_0 = tock_pos( a.p13.CH1_PHASE_CLKa,  b.p13.CH1_PHASE_CLKa, b.p11.CEPO, c.p13.CH1_PHASE_0, !c.p13.CH1_PHASE_0);
  c.p13.CH1_PHASE_1 = tock_pos(!a.p13.CH1_PHASE_0,   !b.p13.CH1_PHASE_0,   b.p11.CEPO, c.p13.CH1_PHASE_1, !c.p13.CH1_PHASE_1);
  c.p13.CH1_PHASE_2 = tock_pos(!a.p13.CH1_PHASE_1,   !b.p13.CH1_PHASE_1,   b.p11.CEPO, c.p13.CH1_PHASE_2, !c.p13.CH1_PHASE_2);

  // ENEK    = 00000011
  // EZOZ    = 00000010
  // PHASE_2 = 00001111
  // CODO    = 11111100

  c.p13.DUVO = not(b.p13.CH1_PHASE_0);
  c.p13.ENEK = and(b.p13.DUVO, b.p13.EZOZ);
  c.p13.EZOZ = and(b.p13.CH1_PHASE_1, b.p13.CH1_PHASE_2);
  c.p13.CODO = not(b.p13.EZOZ);

  c.p13.DUTY_0 = nor( b.p11.CH1_DUTY_0,  b.p11.CH1_DUTY_1);
  c.p13.DUTY_1 = nor(!b.p11.CH1_DUTY_0,  b.p11.CH1_DUTY_1);
  c.p13.DUTY_2 = nor( b.p11.CH1_DUTY_0, !b.p11.CH1_DUTY_1);
  c.p13.DUTY_3 = nor(!b.p11.CH1_DUTY_0, !b.p11.CH1_DUTY_1);

  c.p13.CH1_BIT = amux4(b.p13.ENEK,        b.p13.DUTY_0,
                        b.p13.EZOZ,        b.p13.DUTY_1,
                        b.p13.CH1_PHASE_2, b.p13.DUTY_2,
                        b.p13.CODO,        b.p13.DUTY_3);

  //----------
  // Length timer

  c.p13.FF11_WRna = nand(b.p10.APU_WR, b.p10.ADDR_FF11);
  c.p13.FF11_WRa = not(b.p13.FF11_WRna);
  c.p13.FF11_WRb = not(b.p13.FF11_WRna);
  c.p13.FF11_WRc = not(b.p13.FF11_WRna);
  c.p13.CORY = nor(b.p13.FEKU, b.p09.APU_RESET1, b.p13.FF11_WRb);



  c.p13.CH1_LEN_CLKn = nor(b.p01.BUFY_256, !b.p11.NR14_STOP, b.p13.CH1_LEN_DONE); // use_len polarity?
  c.p13.CH1_LEN_CLK = not(b.p13.CH1_LEN_CLKn);

  c.p13.NR11_LEN0 = count_pos(a.p13.CH1_LEN_CLK, b.p13.CH1_LEN_CLK, b.p13.FF11_WRc, b.p13.NR11_LEN0, b.D0);
  c.p13.NR11_LEN1 = count_pos(a.p13.NR11_LEN0,   b.p13.NR11_LEN0,   b.p13.FF11_WRc, b.p13.NR11_LEN1, b.D1);
  c.p13.NR11_LEN2 = count_pos(a.p13.NR11_LEN1,   b.p13.NR11_LEN1,   b.p13.FF11_WRc, b.p13.NR11_LEN2, b.D2);
  c.p13.NR11_LEN3 = count_pos(a.p13.NR11_LEN2,   b.p13.NR11_LEN2,   b.p13.FF11_WRc, b.p13.NR11_LEN3, b.D3);
  c.p13.NR11_LEN3n = not(!b.p13.NR11_LEN3);
  c.p13.NR11_LEN4 = count_pos(a.p13.NR11_LEN3n,  b.p13.NR11_LEN3n,  b.p13.FF11_WRa, b.p13.NR11_LEN4, b.D4);
  c.p13.NR11_LEN5 = count_pos(a.p13.NR11_LEN4,   b.p13.NR11_LEN4,   b.p13.FF11_WRa, b.p13.NR11_LEN5, b.D5);

  c.p13.CH1_LEN_DONE = tock_pos(!a.p13.NR11_LEN5, !b.p13.NR11_LEN5, b.p13.CORY, c.p13.CH1_LEN_DONE, !c.p13.CH1_LEN_DONE);

  c.p13.BONE = not(b.p12.ATYS);
  c.p13.CH1_AMP_ENn = nor(b.p11.CH1_ENV_DIR, b.p11.CH1_VOL_0, b.p11.CH1_VOL_1, b.p11.CH1_VOL_2, b.p11.CH1_VOL_3);

  c.p13.CYFA = and(b.p13.CH1_LEN_DONE, b.p11.NR14_STOP);
  c.p13.BERY = or(b.p13.BONE, b.p09.APU_RESET1, b.p13.CYFA, b.p13.CH1_AMP_ENn);

  //----------
  // EG timer

  c.p13.KAZA = nor(b.p13.FEKU, b.p13.KOZY);
  c.p13.SWEEP_RST = not(b.p13.KAZA);

  c.p13.CLK_128n = not(b.p01.CLK_128);
  
  c.p13.CLK_64a  = tock_pos(a.p13.CLK_128n, b.p13.CLK_128n, b.p13.KADO, b.p13.CLK_64a, !b.p13.CLK_64a);
  c.p13.CLK_64nb = not(b.p13.CLK_64a);
  c.p13.CLK_64b  = not(b.p13.CLK_64nb);

  c.p13.SWEEP_CNT0 = count_pos(a.p13.CLK_64b,    b.p13.CLK_64b,    b.p13.SWEEP_RST, b.p13.SWEEP_CNT0, !b.p11.CH1_SWEEP_0);
  c.p13.SWEEP_CNT1 = count_pos(a.p13.SWEEP_CNT0, b.p13.SWEEP_CNT0, b.p13.SWEEP_RST, b.p13.SWEEP_CNT1, !b.p11.CH1_SWEEP_1);
  c.p13.SWEEP_CNT2 = count_pos(a.p13.SWEEP_CNT1, b.p13.SWEEP_CNT1, b.p13.SWEEP_RST, b.p13.SWEEP_CNT2, !b.p11.CH1_SWEEP_2);

  
  c.p13.CH1_NO_SWEEP = nor(b.p11.CH1_SWEEP_0, b.p11.CH1_SWEEP_1, b.p11.CH1_SWEEP_2);

  c.p13.KOTE = and(b.p13.SWEEP_CNT0, b.p13.SWEEP_CNT1, b.p13.SWEEP_CNT2);

  c.p13.KORO = nor(b.p13.KUKU, b.p13.CH1_NO_SWEEP);

  c.p13.KOZY = tock_pos(a.p01.HORU_512, b.p01.HORU_512, b.p13.KORO, b.p13.KOZY, b.p13.KOTE);
  c.p13.KYNO = tock_pos(a.p13.KOZY,     b.p13.KOZY,     b.p13.KORU, b.p13.KYNO, b.p13.JADE);


  c.p13.KURY = not(b.p13.KOZY);
  c.p13.KUKU = nor(b.p01.CPUCLK_REQn, b.p13.KURY);

  c.p13.FEKU = tock_pos(a.p01.DYFA_1M, b.p01.DYFA_1M, b.p13.EGET, b.p13.FEKU, b.p13.FYFO);
  c.p13.FARE = tock_pos(a.p01.DYFA_1M, b.p01.DYFA_1M, b.p13.ERUM, b.p13.FARE, b.p13.FEKU);
  c.p13.FYTE = tock_pos(a.p01.DYFA_1M, b.p01.DYFA_1M, b.p13.ERUM, b.p13.FYTE, b.p13.FARE);

  c.p13.FEMY = nor(b.p13.CH1_AMP_ENn, b.p09.APU_RESET1);
  c.p13.GEPU = not(b.p13.FYTE);
  c.p13.GEXU = unk2(b.p13.FEMY, b.p13.GEPU);

  c.p13.EGET = nor(b.p09.APU_RESET1, b.p13.FARE);
  c.p13.GEFE = not(b.p13.EGET);
  c.p13.FYFO = or(b.p13.GEFE, b.p13.CH1_RUNNING); // unk2





  c.p13.KEKO = or(b.p09.APU_RESET1, b.p13.FEKU);
  c.p13.KABA = or(b.p09.APU_RESET1, b.p13.FEKU);
  c.p13.KYLY = not(b.p13.KABA);

  c.p13.KEZU = or(b.p13.KYNO, b.p13.KEKO); // unk2
  c.p13.KAKE = and(b.p13.KOZY, b.p13.CH1_NO_SWEEP, b.p13.KEZU);

  c.p13.CH1_ACTIVE = or(b.p13.FEKU, b.p13.BERY); // unk2
  c.p13.CH1_ACTIVEn = not(b.p13.CH1_ACTIVE);

  c.p13.DUWO = tock_pos(a.p13.COPE, b.p13.COPE, b.p11.CEPO, b.p13.DUWO, b.p13.CH1_BIT);
  c.p13.COWE = and(b.p13.CH1_ACTIVE, b.p13.DUWO);
  c.p13.BOTO = or(b.p13.COWE, b.p09.NET03);

  // weird things are going on with the reg clocks and muxes... probably broken

  c.p13.HESU = amux2(b.p11.CH1_ENV_DIR, b.p13.HOKO, !b.p13.HOKO, !b.p11.CH1_ENV_DIR);
  c.p13.HETO = amux2(b.p11.CH1_ENV_DIR, b.p13.HEMY, !b.p13.HEMY, !b.p11.CH1_ENV_DIR);
  c.p13.HYTO = amux2(b.p11.CH1_ENV_DIR, b.p13.HAFO, !b.p13.HAFO, !b.p11.CH1_ENV_DIR);
  c.p13.JUFY = amux2(b.p11.CH1_ENV_DIR, b.p13.KAKE,  b.p13.KAKE, !b.p11.CH1_ENV_DIR);

  c.p13.HEVO = count_pos(a.p13.HESU, b.p13.HESU, b.p13.FEKU, b.p13.HEVO, b.p11.CH1_VOL_0);
  c.p13.HOKO = count_pos(a.p13.HETO, b.p13.HETO, b.p13.FEKU, b.p13.HOKO, b.p11.CH1_VOL_1);
  c.p13.HEMY = count_pos(a.p13.HYTO, b.p13.HYTO, b.p13.FEKU, b.p13.HEMY, b.p11.CH1_VOL_2);
  c.p13.HAFO = count_pos(a.p13.JUFY, b.p13.JUFY, b.p13.FEKU, b.p13.HAFO, b.p11.CH1_VOL_3);

  c.p13.CH1_OUT3 = and(b.p13.HEVO, b.p13.BOTO);
  c.p13.CH1_OUT2 = and(b.p13.HOKO, b.p13.BOTO);
  c.p13.CH1_OUT1 = and(b.p13.HEMY, b.p13.BOTO);
  c.p13.CH1_OUT0 = and(b.p13.HAFO, b.p13.BOTO);

  c.p13.HUFU = nand(b.p11.CH1_ENV_DIR, b.p13.HAFO, b.p13.HEMY, b.p13.HOKO, b.p13.HEVO);
  c.p13.HANO = nor(b.p11.CH1_ENV_DIR,  b.p13.HAFO, b.p13.HEMY, b.p13.HOKO, b.p13.HEVO);
  c.p13.HAKE = not(b.p13.HUFU);
  c.p13.JADE = or(b.p13.HAKE, b.p13.HANO);
  c.p13.KORU = nor(b.p13.FEKU, b.p09.APU_RESET1);


  //----------
  // Sweep timer

  c.p13.DAFA = nor(b.p13.BEXA, b.p13.FEKU);
  c.p13.CYMU = not(b.p13.DAFA);
  c.p13.BAVE = and(b.p11.CH1_SWEEP_TIME_0, b.p11.CH1_SWEEP_TIME_1, b.p11.CH1_SWEEP_TIME_2);

  c.p13.CUPO = count_pos(a.p09.CATE, b.p09.CATE, b.p13.CYMU, b.p13.CUPO, b.p11.CH1_SWEEP_TIME_0);
  c.p13.CYPU = count_pos(a.p13.CUPO, b.p13.CUPO, b.p13.CYMU, b.p13.CYPU, b.p11.CH1_SWEEP_TIME_1);
  c.p13.CAXY = count_pos(a.p13.CYPU, b.p13.CYPU, b.p13.CYMU, b.p13.CAXY, b.p11.CH1_SWEEP_TIME_2);

  c.p13.BURY = nor(b.p13.BAVE, b.p09.APU_RESET1);
  c.p13.COZE = and(b.p13.CAXY, b.p13.CYPU, b.p13.CUPO);

  c.p13.BEXA = tock_pos(a.p01.AJER_2M, b.p01.AJER_2M, b.p13.BURY, b.p13.BEXA, b.p13.COZE);

  //----------
  // Sweep shift counter

  c.p13.DACU = nor(b.p13.FEKU, b.p13.BEXA);
  c.p13.CYLU = not(b.p13.DACU);
  c.p13.BUGE = nand(b.p11.CH1_SWEEP_SHIFT_0, b.p11.CH1_SWEEP_SHIFT_1, b.p11.CH1_SWEEP_SHIFT_2);
  c.p13.CELE = not(b.p13.BUGE);

  c.p13.ADAD = not(!b.p13.BYTE);
  c.p13.EPUK = nor(b.p13.ADAD, b.p09.APU_RESET1);
  c.p13.EVOL = nor(b.p13.BEXA, b.p13.FYTE);
  c.p13.FEMU = unk2(b.p13.EPUK, b.p13.EVOL);
  c.p13.EGYP = nor(b.p01.DYFA_1M, b.p13.FEMU);
  c.p13.DODY = nor(b.p13.EGYP, b.p13.CELE);
  c.p13.EGOR = and(b.p19.CH4_LEN_3b, b.p13.DODY);
  c.p13.DAPU = not(b.p13.EGOR);

  c.p13.COPA = count_pos(a.p13.DAPU, b.p13.DAPU, b.p13.CYLU, b.p13.COPA, b.p11.CH1_SWEEP_SHIFT_0);
  c.p13.CAJA = count_pos(a.p13.COPA, b.p13.COPA, b.p13.CYLU, b.p13.CAJA, b.p11.CH1_SWEEP_SHIFT_1);
  c.p13.BYRA = count_pos(a.p13.CAJA, b.p13.CAJA, b.p13.CYLU, b.p13.BYRA, b.p11.CH1_SWEEP_SHIFT_2);

  c.p13.COPY = and(b.p13.COPA, b.p13.CAJA, b.p13.BYRA);
  c.p13.ATAT = nor(b.p09.APU_RESET1, b.p13.BEXA);

  c.p13.BYTE = tock_pos(a.p01.AJER_2M, b.p01.AJER_2M, b.p13.ATAT, b.p13.BYTE, b.p13.COPY);

  c.p13.ATUV = and(b.p13.BEXA, b.p12.ATYS);
  c.p13.BOJE = and(b.p13.ATUV, b.p13.BUGE);
  c.p13.BUSO = or(b.p13.BUGE, b.p12.ATYS, b.p13.BEXA);
  c.p13.KALA = nor(b.p13.BEXA, b.p13.FEKU);
}
