#include "P14_Ch2Regs.h"
#include "../Schematics.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P14_Ch2Regs::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p14.FOGE = not(b.p09.CPU_RDn);
  c.p14.FAPE = and(b.p14.FOGE, b.p09.NET03);

  //----------
  // FF16 NR21

  c.p14.AGYN = nand(b.p10.APU_WR, b.p10.FF16); // FF16_WR
  c.p14.ASYP = not(b.p14.AGYN);
  c.p14.BENY = nor(b.p14.ASYP, b.p09.APU_RESET, b.p15.ELOX);

  c.p14.BACU = and(b.p10.FF16, b.p10.APU_WR);
  c.p14.BUDU = not(b.p14.BACU);
  c.p14.BAMY = tock_pos(a.p14.BUDU, b.p14.BUDU, b.p09.APU_RESET2n, b.p14.BAMY, b.D7);
  c.p14.BERA = tock_pos(a.p14.BUDU, b.p14.BUDU, b.p09.APU_RESET2n, b.p14.BERA, b.D6);

  c.p14.BYGO = not(b.p09.CPU_RDn);
  c.p14.CORO = nand(b.p10.FF16, b.p14.BYGO);
  c.p14.CEKA = not(!b.p14.BAMY);
  c.p14.CECY = not(!b.p14.BERA);

  if (b.p14.CORO) {
    c.D7 = b.p14.CEKA;
    c.D6 = b.p14.CECY;
  }

  //----------
  // FF17 NR22

  c.p14.ENUF = and(b.p10.FF17, b.p10.APU_WR);
  c.p14.JYBU = not(b.p09.APU_RESET);
  c.p14.FYRY = not(b.p10.FF17);
  c.p14.GURU = or(b.p14.FYRY, b.p09.CPU_RDn);

  c.p14.GURE = not(b.p10.FF17);
  c.p14.GEXA = or(b.p14.GURE, b.p09.CPU_RDn);
  c.p14.GERE = and(b.p10.APU_WR, b.p10.FF17);
  c.p14.JEDE = not(b.p14.GERE);

  c.p14.GATA = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.GATA, b.D4);
  c.p14.FORE = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.FORE, b.D3);
  c.p14.GAGE = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.GAGE, b.D7);
  c.p14.GURA = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.GURA, b.D6);
  c.p14.GUFE = tock_pos(a.p14.ENUF, b.p14.ENUF, b.p14.JYBU, b.p14.GUFE, b.D5);
  c.p14.HAVA = tock_pos(a.p14.JEDE, b.p14.JEDE, b.p14.JYBU, b.p14.HAVA, b.D2);
  c.p14.HORE = tock_pos(a.p14.JEDE, b.p14.JEDE, b.p14.JYBU, b.p14.HORE, b.D1);
  c.p14.HYFU = tock_pos(a.p14.JEDE, b.p14.JEDE, b.p14.JYBU, b.p14.HYFU, b.D0);

  c.p14.HUPE = not(!b.p14.GATA);
  c.p14.GENE = not(!b.p14.FORE);
  c.p14.HYRY = not(!b.p14.GAGE);
  c.p14.HORO = not(!b.p14.GURA);
  c.p14.HERE = not(!b.p14.GUFE);
  c.p14.HAVU = not(!b.p14.HAVA);
  c.p14.HYRE = not(!b.p14.HORE);
  c.p14.HUVU = not(!b.p14.HYFU);

  if (b.p14.GURU) {
    c.D4 = b.p14.HUPE;
    c.D3 = b.p14.GENE;
    c.D7 = b.p14.HYRY;
    c.D6 = b.p14.HORO;
    c.D5 = b.p14.HERE;
  }

  if (b.p14.GEXA) {
    c.D2 = b.p14.HAVU;
    c.D1 = b.p14.HYRE;
    c.D0 = b.p14.HUVU;
  }

  //----------
  // FF18 NR23

  c.p14.DOSA = and(b.p10.FF18, b.p10.APU_WR);
  c.p14.HUDE = not(b.p09.APU_RESET);
  c.p14.ESUR = not(b.p14.DOSA);
  c.p14.EXUC = and(b.p10.FF18, b.p10.APU_WR);
  c.p14.FYXO = not(b.p14.EXUC);
  c.p14.FERY = not(b.p10.DUCE);
  c.p14.GUZA = nor(b.p14.FERY, b.p14.FAPE);
  c.p14.FUTY = not(b.p14.GUZA);

  c.p14.FOFE = tock_pos(a.p14.ESUR, b.p14.ESUR, b.p14.HUDE, b.p14.FOFE, b.D0);
  c.p14.FOVA = tock_pos(a.p14.ESUR, b.p14.ESUR, b.p14.HUDE, b.p14.FOVA, b.D1);
  c.p14.FEDY = tock_pos(a.p14.ESUR, b.p14.ESUR, b.p14.HUDE, b.p14.FEDY, b.D2);
  c.p14.FOME = tock_pos(a.p14.ESUR, b.p14.ESUR, b.p14.HUDE, b.p14.FOME, b.D3);
  c.p14.FORA = tock_pos(a.p14.ESUR, b.p14.ESUR, b.p14.HUDE, b.p14.FORA, b.D4);
  c.p14.GODA = tock_pos(a.p14.FYXO, b.p14.FYXO, b.p14.HUDE, b.p14.GODA, b.D5);
  c.p14.GUMY = tock_pos(a.p14.FYXO, b.p14.FYXO, b.p14.HUDE, b.p14.GUMY, b.D6);
  c.p14.GUPU = tock_pos(a.p14.FYXO, b.p14.FYXO, b.p14.HUDE, b.p14.GUPU, b.D7);

  c.p14.DONE = count_pos(a.p15.DOCA, b.p15.DOCA, b.p15.COGU, b.p14.DONE, b.p14.FOFE);
  c.p14.DYNU = count_pos(a.p14.DONE,    b.p14.DONE,    b.p15.COGU, b.p14.DYNU, b.p14.FOVA);
  c.p14.EZOF = count_pos(a.p14.DYNU,    b.p14.DYNU,    b.p15.COGU, b.p14.EZOF, b.p14.FEDY);
  c.p14.CYVO = count_pos(a.p14.EZOF,    b.p14.EZOF,    b.p15.COGU, b.p14.CYVO, b.p14.FOME);

  c.p14.EDEP = not(!b.p14.CYVO);

  c.p14.FUXO = count_pos(a.p14.EDEP, b.p14.EDEP, b.p15.EROG, b.p14.FUXO, b.p14.FORA);
  c.p14.GANO = count_pos(a.p14.FUXO, b.p14.FUXO, b.p15.EROG, b.p14.GANO, b.p14.GODA);
  c.p14.GOCA = count_pos(a.p14.GANO, b.p14.GANO, b.p15.EROG, b.p14.GOCA, b.p14.GUMY);
  c.p14.GANE = count_pos(a.p14.GOCA, b.p14.GOCA, b.p15.EROG, b.p14.GANE, b.p14.GUPU);

  c.p14.FAVA = not(!b.p14.DONE);
  c.p14.FAJY = not(!b.p14.DYNU);
  c.p14.FEGU = not(!b.p14.EZOF);
  c.p14.FOSE = not(!b.p14.CYVO);
  c.p14.GERO = not(!b.p14.FUXO);
  c.p14.GAKY = not(!b.p14.GANO);
  c.p14.GADU = not(!b.p14.GOCA);
  c.p14.GAZO = not(!b.p14.GANE);

  if (b.p14.FUTY) {
    c.D0 = b.p14.FAVA;
    c.D1 = b.p14.FAJY;
    c.D2 = b.p14.FEGU;
    c.D3 = b.p14.FOSE;
    c.D5 = b.p14.GERO; // wtf, d4 and d5 are switched on the schematic
    c.D4 = b.p14.GAKY;
    c.D6 = b.p14.GADU;
    c.D7 = b.p14.GAZO;
  }

  //----------
  // FF19 NR24

  c.p14.KYPU = not(b.p09.APU_RESET);
  c.p14.JENU = and(b.p10.FF19, b.p10.APU_WR);
  c.p14.KYSA = not(b.p14.JENU);
  c.p14.JUPY = tock_pos(a.p14.KYSA, b.p14.KYSA, b.p14.KYPU, c.p14.JUPY, b.D2);
  c.p14.JANY = tock_pos(a.p14.KYSA, b.p14.KYSA, b.p14.KYPU, c.p14.JANY, b.D1);
  c.p14.JEFU = tock_pos(a.p14.KYSA, b.p14.KYSA, b.p14.KYPU, c.p14.JEFU, b.D0);

  c.p14.DETA = nand(b.p10.APU_WR, b.p10.FF19);
  c.p14.GOTE = not(b.p10.FF19);
  c.p14.ETAP = tock_pos(a.p14.DETA, b.p14.DETA, b.p15.DERA, b.p14.ETAP, b.D7);
  c.p14.HYPO = or(b.p14.GOTE, b.p14.FAPE);

  c.p14.GALA = not(!b.p14.GANE);
  c.p14.HEVY = count_pos(a.p14.GALA, b.p14.GALA, b.p15.GYPA, c.p14.HEVY, b.p14.FF19_D0);
  c.p14.HEPU = count_pos(a.p14.HEVY, b.p14.HEVY, b.p15.GYPA, c.p14.HEPU, b.p14.FF19_D1);
  c.p14.HERO = count_pos(a.p14.HEPU, b.p14.HEPU, b.p15.GYPA, c.p14.HERO, b.p14.FF19_D2);

  c.p14.JEKE = not(!b.p14.HERO);
  c.p14.JARO = not(!b.p14.HEPU);
  c.p14.HUNA = not(!b.p14.HEVY);

  if (b.p14.HYPO) {
    c.D2 = b.p14.JEKE;
    c.D1 = b.p14.JARO;
    c.D0 = b.p14.HUNA;
  }

  c.p14.GADO = not(b.p09.CPU_RDn);
  c.p14.EVYF = nor(b.p16.ANUJ, b.p10.FF19);
  c.p14.FAZO = not(b.p09.APU_RESET);
  c.p14.HUMA = nor(b.p10.FF19, b.p14.GADO);
  c.p14.EMER = tock_pos(a.p14.EVYF, b.p14.EVYF, b.p14.FAZO, b.p14.EMER, b.D6);
  c.p14.GOJY = b.p14.EMER;

  if (b.p14.HUMA) {
    c.D6 = b.p14.GOJY;
  }
}
