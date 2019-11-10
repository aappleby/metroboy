#include "P13_Channel1.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P13_Channel1::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P13_Channel1 pa = ga.p13;
  const P13_Channel1 pb = gb.p13;
  P13_Channel1 pc = gc.p13;

  //----------
  // Waveform counter

  pc.DAJO = not(pb.COPE);

  pc.ESUT = tock_pos(pa.DAJO,  pb.DAJO,  pb.CEPO, pc.ESUT, !pc.ESUT);
  pc.EROS = tock_pos(!pa.ESUT, !pb.ESUT, pb.CEPO, pc.EROS, !pc.EROS);
  pc.DAPE = tock_pos(!pa.EROS, !pb.EROS, pb.CEPO, pc.DAPE, !pc.DAPE);

  pc.DUVO = not(pb.ESUT);
  pc.EZOZ = and(pb.EROS, pb.DAPE);
  pc.ENEK = and(pb.DUVO, pb.EZOZ);
  pc.CODO = not(pb.EZOZ);
  pc.COSO = nor(pb.FF11_D6,  pb.FF11_D7);
  pc.CAVA = nor(pb.FF11_D6n, pb.FF11_D7);
  pc.CEVU = nor(pb.FF11_D6,  pb.FF11_D7n);
  pc.CAXO = nor(pb.FF11_D6n, pb.FF11_D7n);

  pc.DUNA = amux4(pb.ENEK, pb.COSO, pb.EZOZ, pb.CAVA, pb.DAPE, pb.CEVU, pb.CODO, pb.CAXO);

  //----------
  // Length timer

  pc.BORO = nand(gb.p10.APU_WR, pb.FF11);
  pc.BEPE = not(pb.BORO);
  pc.BOKA = not(pb.BORO);
  pc.BUGY = not(pb.BORO);
  pc.CORY = nor(pb.FEKU, gb.p09.APU_RESET, pb.BOKA);

  pc.CUSO = not(!pb.CUNO);

  // FIXME daisy chain
  pc.CURA = count_pos(pa.CUSO, pb.CUSO, pb.BEPE, pb.CURA, gb.D4);
  pc.ERAM = count_pos(pa.CURA, pb.CUSO, pb.BEPE, pb.ERAM, gb.D5);
  pc.CERO = tock_pos(!pa.ERAM, !pb.ERAM, pb.CORY, pc.CERO, !pc.CERO);

  pc.CAPY = nor(pb.FF14_D6n, gb.p01.BUFY_256, pb.CERO);
  pc.CYFA = and(pb.CERO, pb.FF14_D6);
  pc.CANU = not(pb.CAPY);

  pc.BACY = count_pos(pa.CANU, pb.CANU, pb.BUGY, pb.BACY, gb.D0);
  pc.CAVY = count_pos(pa.BACY, pb.BACY, pb.BUGY, pb.CAVY, gb.D1);
  pc.BOVY = count_pos(pa.CAVY, pb.CAVY, pb.BUGY, pb.BOVY, gb.D2);
  pc.CUNO = count_pos(pa.BOVY, pb.BOVY, pb.BUGY, pb.CUNO, gb.D3);

  pc.BONE = not(pb.ATYS);
  pc.HOCA = nor(pb.FF12_D3, pb.FF12_D4, pb.FF12_D5, pb.FF12_D6, pb.FF12_D7);
  pc.FEMY = nor(pb.HOCA, gb.p09.APU_RESET);
  pc.BERY = or(pb.BONE, gb.p09.APU_RESET, pb.CYFA, pb.HOCA);
  pc.GEPU = not(pb.FYTE);
  pc.GEXU = unk2(pb.FEMY, pb.GEPU);

  //----------
  // EG timer

  pc.KAZA = nor(pb.FEKU, pb.KOZY);
  pc.KUXU = not(pb.KAZA);

  pc.JONE = not(gb.p01.BYFE_128);
  pc.KADO = not(gb.p09.APU_RESET);
  pc.KALY = tock_pos(pa.JONE, pb.JONE, pb.KADO, pb.KALY, !pb.KALY);
  pc.KERE = not(pb.KALY);
  pc.JOLA = not(pb.KERE);

  pc.JOVA = count_pos(pa.JOLA, pb.JOLA, pb.KUXU, pb.JOVA, pb.FF12_D0n);
  pc.KENU = count_pos(pa.JOVA, pb.JOVA, pb.KUXU, pb.KENU, pb.FF12_D1n);
  pc.KERA = count_pos(pa.KENU, pb.KENU, pb.KUXU, pb.KERA, pb.FF12_D2n);

  pc.KOTE = and(pb.JOVA, pb.KENU, pb.KERA);
  pc.KURY = not(pb.KOZY);
  pc.KUKU = nor(gb.ABOL, pb.KURY);
  pc.KOMA = nor(pb.FF12_D0, pb.FF12_D1, pb.FF12_D2);
  pc.KORO = nor(pb.KUKU, pb.KOMA);
  pc.KOZY = tock_pos(ga.p01.HORU_512, gb.p01.HORU_512, pb.KORO, pb.KOZY, pb.KOTE);

  pc.ERUM = not(gb.p09.APU_RESET);
  pc.FARE = tock_pos(ga.p09.DYFA_1M, gb.p09.DYFA_1M, pb.ERUM, pb.FARE, pb.FEKU);
  pc.FYTE = tock_pos(ga.p09.DYFA_1M, gb.p09.DYFA_1M, pb.ERUM, pb.FYTE, pb.FARE);
  pc.EGET = nor(gb.p09.APU_RESET, pb.FARE);
  pc.GEFE = not(pb.EGET);

  pc.DOGE = nand(gb.p10.APU_WR, pb.FF14_D6); // BUG - APU_WR
  pc.DADO = nor(gb.p09.APU_RESET, pb.EZEC);
  pc.DUPE = tock_pos(pa.DOGE, pb.DOGE, pb.DADO, pb.DUPE, gb.D7);
  pc.DUKA = not(gb.p09.APU_RESET);
  pc.EZEC = tock_pos(ga.p01.PHIn, gb.p01.PHIn, pb.DUKA, pb.EZEC, pb.DUPE);

  pc.FYFO = or(pb.GEFE, pb.EZEC); // unk2
  pc.FEKU = tock_pos(ga.p09.DYFA_1M, gb.p09.DYFA_1M, pb.EGET, pb.FEKU, pb.FYFO);
  pc.KEKO = or(gb.p09.APU_RESET, pb.FEKU);
  pc.KABA = or(gb.p09.APU_RESET, pb.FEKU);
  pc.KYLY = not(pb.KABA);

  pc.KYNO = pb.KYNO;
  pc.KEZU = or(pb.KYNO, pb.KEKO); // unk2
  pc.KAKE = and(pb.KOZY, pb.KOMA, pb.KEZU);

  pc.CYTO = or(pb.FEKU, pb.BERY); // unk2
  pc.DUWO = tock_pos(pa.COPE, pb.COPE, pb.CEPO, pb.DUWO, pb.CH1_BIT);
  pc.CARA = not(pb.CYTO);
  pc.COWE = and(pb.CYTO, pb.DUWO);
  pc.BOTO = or(pb.COWE, gb.p09.NET03);

  // weird things are going on with the reg clocks and muxes... probably broken

  pc.HESU = amux2(pb.FF12_D3, pb.HOKO, !pb.HOKO, pb.FF12_D3n);
  pc.HETO = amux2(pb.FF12_D3, pb.HEMY, !pb.HEMY, pb.FF12_D3n);
  pc.HYTO = amux2(pb.FF12_D3, pb.HAFO, !pb.HAFO, pb.FF12_D3n);
  pc.JUFY = amux2(pb.FF12_D3, pb.KAKE,  pb.KAKE, pb.FF12_D3n);

  pc.HEVO = count_pos(pa.HESU, pb.HESU, pb.FEKU, pb.HEVO, pb.FF12_D7);
  pc.HOKO = count_pos(pa.HETO, pb.HETO, pb.FEKU, pb.HOKO, pb.FF12_D6);
  pc.HEMY = count_pos(pa.HYTO, pb.HYTO, pb.FEKU, pb.HEMY, pb.FF12_D5);
  pc.HAFO = count_pos(pa.JUFY, pb.JUFY, pb.FEKU, pb.HAFO, pb.FF12_D4);

  pc.ACEG = and(pb.HEVO, pb.BOTO);
  pc.AGOF = and(pb.HOKO, pb.BOTO);
  pc.ASON = and(pb.HEMY, pb.BOTO);
  pc.AMOP = and(pb.HAFO, pb.BOTO);

  pc.HUFU = nand(pb.FF12_D3, pb.HAFO, pb.HEMY, pb.HOKO, pb.HEVO);
  pc.HANO = nor(pb.FF12_D3,  pb.HAFO, pb.HEMY, pb.HOKO, pb.HEVO);
  pc.HAKE = not(pb.HUFU);
  pc.JADE = or(pb.HAKE, pb.HANO);
  pc.KORU = nor(pb.FEKU, gb.p09.APU_RESET);

  pc.KYNO = tock_pos(pa.KOZY, pb.KOZY, pb.KORU, pb.KYNO, pb.JADE);

  //----------
  // little thing left side

  pc.CYTE = not(pb.COMY);
  pc.COPE = not(pb.CYTE);
  pc.DOKA = and(pb.COMY, gb.p09.DYFA_1M);
  pc.CALA = not(pb.COPU_COUT);
  pc.DYRU = nor(gb.p09.APU_RESET, pb.FEKU, pb.DOKA);
  pc.COMY = tock_pos(pa.CALA, pb.CALA, pb.DYRU, pb.COMY, !pb.COMY);

  //----------
  // Sweep timer

  pc.DAFA = nor(pb.BEXA, pb.FEKU);
  pc.CYMU = not(pb.DAFA);
  pc.BAVE = and(pb.FF10_D4n, pb.FF10_D5n, pb.FF10_D6n);

  pc.CUPO = count_pos(pa.CATE, pb.CATE, pb.CYMU, pb.CUPO, pb.FF10_D4n);
  pc.CYPU = count_pos(pa.CUPO, pb.CUPO, pb.CYMU, pb.CYPU, pb.FF10_D5n);
  pc.CAXY = count_pos(pa.CYPU, pb.CYPU, pb.CYMU, pb.CAXY, pb.FF10_D6n);

  pc.BURY = nor(pb.BAVE, gb.p09.APU_RESET);
  pc.COZE = and(pb.CAXY, pb.CYPU, pb.CUPO);

  pc.BEXA = tock_pos(ga.p09.AJER_2M, gb.p09.AJER_2M, pb.BURY, pb.BEXA, pb.COZE);

  //----------
  // Sweep shift counter

  pc.DACU = nor(pb.FEKU, pb.BEXA);
  pc.CYLU = not(pb.DACU);
  pc.BUGE = nand(pb.FF10_D2n, pb.FF10_D1n, pb.FF10_D0n);
  pc.CELE = not(pb.BUGE);

  pc.ADAD = not(!pb.BYTE);
  pc.EPUK = nor(pb.ADAD, gb.p09.APU_RESET);
  pc.EVOL = nor(pb.BEXA, pb.FYTE);
  pc.FEMU = unk2(pb.EPUK, pb.EVOL);
  pc.EGYP = nor(gb.p09.DYFA_1M, pb.FEMU);
  pc.DODY = nor(pb.EGYP, pb.CELE);
  pc.EGOR = and(pb.DOPU_OUT, pb.DODY);
  pc.DAPU = not(pb.EGOR);

  pc.COPA = count_pos(pa.DAPU, pb.DAPU, pb.CYLU, pb.COPA, pb.FF10_D0n);
  pc.CAJA = count_pos(pa.COPA, pb.COPA, pb.CYLU, pb.CAJA, pb.FF10_D1n);
  pc.BYRA = count_pos(pa.CAJA, pb.CAJA, pb.CYLU, pb.BYRA, pb.FF10_D2n);

  pc.COPY = and(pb.COPA, pb.CAJA, pb.BYRA);
  pc.ATAT = nor(gb.p09.APU_RESET, pb.BEXA);

  pc.BYTE = tock_pos(ga.p09.AJER_2M, gb.p09.AJER_2M, pb.ATAT, pb.BYTE, pb.COPY);

  pc.ATUV = and(pb.BEXA, pb.ATYS);
  pc.BOJE = and(pb.ATUV, pb.BUGE);
  pc.BUSO = or(pb.BUGE, pb.ATYS, pb.BEXA);
  pc.KALA = nor(pb.BEXA, pb.FEKU);
}
