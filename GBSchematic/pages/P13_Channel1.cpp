#include "P13_Channel1.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P13_Channel1::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // Waveform counter

  c.p13.DAJO = not(b.p13.COPE);

  c.p13.ESUT = tock_pos(a.p13.DAJO,  b.p13.DAJO,  b.p11.CEPO, c.p13.ESUT, !c.p13.ESUT);
  c.p13.EROS = tock_pos(!a.p13.ESUT, !b.p13.ESUT, b.p11.CEPO, c.p13.EROS, !c.p13.EROS);
  c.p13.DAPE = tock_pos(!a.p13.EROS, !b.p13.EROS, b.p11.CEPO, c.p13.DAPE, !c.p13.DAPE);

  c.p13.DUVO = not(b.p13.ESUT);
  c.p13.EZOZ = and(b.p13.EROS, b.p13.DAPE);
  c.p13.ENEK = and(b.p13.DUVO, b.p13.EZOZ);
  c.p13.CODO = not(b.p13.EZOZ);
  c.p13.COSO = nor( b.p11.FF11_D6,  b.p11.FF11_D7);
  c.p13.CAVA = nor(!b.p11.FF11_D6,  b.p11.FF11_D7);
  c.p13.CEVU = nor( b.p11.FF11_D6, !b.p11.FF11_D7);
  c.p13.CAXO = nor(!b.p11.FF11_D6, !b.p11.FF11_D7);

  c.p13.DUNA = amux4(b.p13.ENEK, b.p13.COSO, b.p13.EZOZ, b.p13.CAVA, b.p13.DAPE, b.p13.CEVU, b.p13.CODO, b.p13.CAXO);

  //----------
  // Length timer

  c.p13.BORO = nand(b.p10.APU_WR, b.p10.FF11);
  c.p13.BEPE = not(b.p13.BORO);
  c.p13.BOKA = not(b.p13.BORO);
  c.p13.BUGY = not(b.p13.BORO);
  c.p13.CORY = nor(b.p13.FEKU, b.p09.APU_RESET1, b.p13.BOKA);

  c.p13.CUSO = not(!b.p13.CUNO);

  c.p13.CURA = count_pos(a.p13.CUSO, b.p13.CUSO, b.p13.BEPE, b.p13.CURA, b.D4);
  c.p13.ERAM = count_pos(a.p13.CURA, b.p13.CUSO, b.p13.BEPE, b.p13.ERAM, b.D5);
  c.p13.CERO = tock_pos(!a.p13.ERAM, !b.p13.ERAM, b.p13.CORY, c.p13.CERO, !c.p13.CERO);

  c.p13.CAPY = nor(!b.p11.FF14_D6, b.p01.BUFY_256, b.p13.CERO);
  c.p13.CYFA = and(b.p13.CERO, b.p11.FF14_D6);
  c.p13.CANU = not(b.p13.CAPY);

  c.p13.BACY = count_pos(a.p13.CANU, b.p13.CANU, b.p13.BUGY, b.p13.BACY, b.D0);
  c.p13.CAVY = count_pos(a.p13.BACY, b.p13.BACY, b.p13.BUGY, b.p13.CAVY, b.D1);
  c.p13.BOVY = count_pos(a.p13.CAVY, b.p13.CAVY, b.p13.BUGY, b.p13.BOVY, b.D2);
  c.p13.CUNO = count_pos(a.p13.BOVY, b.p13.BOVY, b.p13.BUGY, b.p13.CUNO, b.D3);

  c.p13.BONE = not(b.p12.ATYS);
  c.p13.HOCA = nor(b.p11.CH1_ENV_DIR, b.p11.FF12_D4, b.p11.FF12_D5, b.p11.FF12_D6, b.p11.FF12_D7);
  c.p13.FEMY = nor(b.p13.HOCA, b.p09.APU_RESET1);
  c.p13.BERY = or(b.p13.BONE, b.p09.APU_RESET1, b.p13.CYFA, b.p13.HOCA);
  c.p13.GEPU = not(b.p13.FYTE);
  c.p13.GEXU = unk2(b.p13.FEMY, b.p13.GEPU);

  //----------
  // EG timer

  c.p13.KAZA = nor(b.p13.FEKU, b.p13.KOZY);
  c.p13.KUXU = not(b.p13.KAZA);

  c.p13.JONE = not(b.p01.BYFE_128);
  c.p13.KADO = not(b.p09.APU_RESET1);
  c.p13.KALY = tock_pos(a.p13.JONE, b.p13.JONE, b.p13.KADO, b.p13.KALY, !b.p13.KALY);
  c.p13.KERE = not(b.p13.KALY);
  c.p13.JOLA = not(b.p13.KERE);

  c.p13.JOVA = count_pos(a.p13.JOLA, b.p13.JOLA, b.p13.KUXU, b.p13.JOVA, !b.p11.FF12_D0);
  c.p13.KENU = count_pos(a.p13.JOVA, b.p13.JOVA, b.p13.KUXU, b.p13.KENU, !b.p11.FF12_D1);
  c.p13.KERA = count_pos(a.p13.KENU, b.p13.KENU, b.p13.KUXU, b.p13.KERA, !b.p11.FF12_D2);

  c.p13.KOTE = and(b.p13.JOVA, b.p13.KENU, b.p13.KERA);
  c.p13.KURY = not(b.p13.KOZY);
  c.p13.KUKU = nor(b.p01.ABOL, b.p13.KURY);
  c.p13.KOMA = nor(b.p11.FF12_D0, b.p11.FF12_D1, b.p11.FF12_D2);
  c.p13.KORO = nor(b.p13.KUKU, b.p13.KOMA);
  c.p13.KOZY = tock_pos(a.p01.HORU_512, b.p01.HORU_512, b.p13.KORO, b.p13.KOZY, b.p13.KOTE);

  c.p13.ERUM = not(b.p09.APU_RESET1);
  c.p13.FARE = tock_pos(a.p09.DYFA_1M, b.p09.DYFA_1M, b.p13.ERUM, b.p13.FARE, b.p13.FEKU);
  c.p13.FYTE = tock_pos(a.p09.DYFA_1M, b.p09.DYFA_1M, b.p13.ERUM, b.p13.FYTE, b.p13.FARE);
  c.p13.EGET = nor(b.p09.APU_RESET1, b.p13.FARE);
  c.p13.GEFE = not(b.p13.EGET);

  c.p13.DOGE = nand(b.p10.APU_WR, b.p11.FF14_D6); // BUG - APU_WR
  c.p13.DADO = nor(b.p09.APU_RESET1, b.p13.EZEC);
  c.p13.DUPE = tock_pos(a.p13.DOGE, b.p13.DOGE, b.p13.DADO, b.p13.DUPE, b.D7);
  c.p13.DUKA = not(b.p09.APU_RESET1);
  c.p13.EZEC = tock_pos(a.p01.PHIn, b.p01.PHIn, b.p13.DUKA, b.p13.EZEC, b.p13.DUPE);

  c.p13.FYFO = or(b.p13.GEFE, b.p13.EZEC); // unk2
  c.p13.FEKU = tock_pos(a.p09.DYFA_1M, b.p09.DYFA_1M, b.p13.EGET, b.p13.FEKU, b.p13.FYFO);
  c.p13.KEKO = or(b.p09.APU_RESET1, b.p13.FEKU);
  c.p13.KABA = or(b.p09.APU_RESET1, b.p13.FEKU);
  c.p13.KYLY = not(b.p13.KABA);

  c.p13.KYNO = b.p13.KYNO;
  c.p13.KEZU = or(b.p13.KYNO, b.p13.KEKO); // unk2
  c.p13.KAKE = and(b.p13.KOZY, b.p13.KOMA, b.p13.KEZU);

  c.p13.CYTO = or(b.p13.FEKU, b.p13.BERY); // unk2
  c.p13.DUWO = tock_pos(a.p13.COPE, b.p13.COPE, b.p11.CEPO, b.p13.DUWO, b.p13.CH1_BIT);
  c.p13.CARA = not(b.p13.CYTO);
  c.p13.COWE = and(b.p13.CYTO, b.p13.DUWO);
  c.p13.BOTO = or(b.p13.COWE, b.p09.NET03);

  // weird things are going on with the reg clocks and muxes... probably broken

  c.p13.HESU = amux2(b.p11.CH1_ENV_DIR, b.p13.HOKO, !b.p13.HOKO, !b.p11.CH1_ENV_DIR);
  c.p13.HETO = amux2(b.p11.CH1_ENV_DIR, b.p13.HEMY, !b.p13.HEMY, !b.p11.CH1_ENV_DIR);
  c.p13.HYTO = amux2(b.p11.CH1_ENV_DIR, b.p13.HAFO, !b.p13.HAFO, !b.p11.CH1_ENV_DIR);
  c.p13.JUFY = amux2(b.p11.CH1_ENV_DIR, b.p13.KAKE,  b.p13.KAKE, !b.p11.CH1_ENV_DIR);

  c.p13.HEVO = count_pos(a.p13.HESU, b.p13.HESU, b.p13.FEKU, b.p13.HEVO, b.p11.FF12_D7);
  c.p13.HOKO = count_pos(a.p13.HETO, b.p13.HETO, b.p13.FEKU, b.p13.HOKO, b.p11.FF12_D6);
  c.p13.HEMY = count_pos(a.p13.HYTO, b.p13.HYTO, b.p13.FEKU, b.p13.HEMY, b.p11.FF12_D5);
  c.p13.HAFO = count_pos(a.p13.JUFY, b.p13.JUFY, b.p13.FEKU, b.p13.HAFO, b.p11.FF12_D4);

  c.p13.ACEG = and(b.p13.HEVO, b.p13.BOTO);
  c.p13.AGOF = and(b.p13.HOKO, b.p13.BOTO);
  c.p13.ASON = and(b.p13.HEMY, b.p13.BOTO);
  c.p13.AMOP = and(b.p13.HAFO, b.p13.BOTO);

  c.p13.HUFU = nand(b.p11.CH1_ENV_DIR, b.p13.HAFO, b.p13.HEMY, b.p13.HOKO, b.p13.HEVO);
  c.p13.HANO = nor(b.p11.CH1_ENV_DIR,  b.p13.HAFO, b.p13.HEMY, b.p13.HOKO, b.p13.HEVO);
  c.p13.HAKE = not(b.p13.HUFU);
  c.p13.JADE = or(b.p13.HAKE, b.p13.HANO);
  c.p13.KORU = nor(b.p13.FEKU, b.p09.APU_RESET1);

  c.p13.KYNO = tock_pos(a.p13.KOZY, b.p13.KOZY, b.p13.KORU, b.p13.KYNO, b.p13.JADE);

  //----------
  // little thing left side

  c.p13.CYTE = not(b.p13.COMY);
  c.p13.COPE = not(b.p13.CYTE);
  c.p13.DOKA = and(b.p13.COMY, b.p09.DYFA_1M);
  c.p13.CALA = not(b.p11.COPU); // not sure about this, says COPU_COUT (carry out) on schematic...
  c.p13.DYRU = nor(b.p09.APU_RESET1, b.p13.FEKU, b.p13.DOKA);
  c.p13.COMY = tock_pos(a.p13.CALA, b.p13.CALA, b.p13.DYRU, b.p13.COMY, !b.p13.COMY);

  //----------
  // Sweep timer

  c.p13.DAFA = nor(b.p13.BEXA, b.p13.FEKU);
  c.p13.CYMU = not(b.p13.DAFA);
  c.p13.BAVE = and(b.p11.FF10_D4n, b.p11.FF10_D5n, b.p11.FF10_D6n);

  c.p13.CUPO = count_pos(a.p09.CATE, b.p09.CATE, b.p13.CYMU, b.p13.CUPO, b.p11.FF10_D4n);
  c.p13.CYPU = count_pos(a.p13.CUPO, b.p13.CUPO, b.p13.CYMU, b.p13.CYPU, b.p11.FF10_D5n);
  c.p13.CAXY = count_pos(a.p13.CYPU, b.p13.CYPU, b.p13.CYMU, b.p13.CAXY, b.p11.FF10_D6n);

  c.p13.BURY = nor(b.p13.BAVE, b.p09.APU_RESET1);
  c.p13.COZE = and(b.p13.CAXY, b.p13.CYPU, b.p13.CUPO);

  c.p13.BEXA = tock_pos(a.p09.AJER_2M, b.p09.AJER_2M, b.p13.BURY, b.p13.BEXA, b.p13.COZE);

  //----------
  // Sweep shift counter

  c.p13.DACU = nor(b.p13.FEKU, b.p13.BEXA);
  c.p13.CYLU = not(b.p13.DACU);
  c.p13.BUGE = nand(b.p11.FF10_D2n, b.p11.FF10_D1n, b.p11.FF10_D0n);
  c.p13.CELE = not(b.p13.BUGE);

  c.p13.ADAD = not(!b.p13.BYTE);
  c.p13.EPUK = nor(b.p13.ADAD, b.p09.APU_RESET1);
  c.p13.EVOL = nor(b.p13.BEXA, b.p13.FYTE);
  c.p13.FEMU = unk2(b.p13.EPUK, b.p13.EVOL);
  c.p13.EGYP = nor(b.p09.DYFA_1M, b.p13.FEMU);
  c.p13.DODY = nor(b.p13.EGYP, b.p13.CELE);
  c.p13.EGOR = and(b.p19.DOPU, b.p13.DODY);
  c.p13.DAPU = not(b.p13.EGOR);

  c.p13.COPA = count_pos(a.p13.DAPU, b.p13.DAPU, b.p13.CYLU, b.p13.COPA, b.p11.FF10_D0n);
  c.p13.CAJA = count_pos(a.p13.COPA, b.p13.COPA, b.p13.CYLU, b.p13.CAJA, b.p11.FF10_D1n);
  c.p13.BYRA = count_pos(a.p13.CAJA, b.p13.CAJA, b.p13.CYLU, b.p13.BYRA, b.p11.FF10_D2n);

  c.p13.COPY = and(b.p13.COPA, b.p13.CAJA, b.p13.BYRA);
  c.p13.ATAT = nor(b.p09.APU_RESET1, b.p13.BEXA);

  c.p13.BYTE = tock_pos(a.p09.AJER_2M, b.p09.AJER_2M, b.p13.ATAT, b.p13.BYTE, b.p13.COPY);

  c.p13.ATUV = and(b.p13.BEXA, b.p12.ATYS);
  c.p13.BOJE = and(b.p13.ATUV, b.p13.BUGE);
  c.p13.BUSO = or(b.p13.BUGE, b.p12.ATYS, b.p13.BEXA);
  c.p13.KALA = nor(b.p13.BEXA, b.p13.FEKU);
}
