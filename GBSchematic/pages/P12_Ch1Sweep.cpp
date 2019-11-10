#include "P12_Ch1Sweep.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P12_Ch1Sweep::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P12_Ch1Sweep pa = ga.p12;
  const P12_Ch1Sweep pb = gb.p12;
  P12_Ch1Sweep pc = gc.p12;

  pc.DEPU = nand(gb.p10.APU_WR, pb.FF13); // BUG - APU_WR
  pc.DEBY = and(gb.p10.APU_WR, pb.FF14);
  pc.DYLA = not(pb.DEPU);

  pc.BYFU = not(gb.D2);
  pc.BOFU = not(gb.D1);
  pc.BYSU = not(gb.D0);
  pc.DULO = not(gb.D7);
  pc.DYLU = not(gb.D6);
  pc.JULO = not(gb.D5);
  pc.KOPU = not(gb.D4);
  pc.ETUV = not(gb.D3);
  pc.FULE = not(gb.D2);
  pc.GULU = not(gb.D1);
  pc.DEKE = not(gb.D0);

  pc.AFEG = nand(gb.D2, pb.DEBY);
  pc.BUDO = nand(gb.D1, pb.DEBY);
  pc.BUGU = nand(gb.D0, pb.DEBY);
  pc.ETOL = nand(gb.D7, pb.DYLA);
  pc.ELER = nand(gb.D6, pb.DYLA);
  pc.KYPA = nand(gb.D5, pb.DYLA);
  pc.KOVU = nand(gb.D4, pb.DYLA);
  pc.GOPE = nand(gb.D3, pb.DYLA);
  pc.GOLO = nand(gb.D2, pb.DYLA);
  pc.GETA = nand(gb.D1, pb.DYLA);
  pc.GYLU = nand(gb.D0, pb.DYLA);

  pc.AJUX = and(pb.DEBY, pb.BYFU);
  pc.AMAC = and(pb.DEBY, pb.BOFU);
  pc.BASO = and(pb.DEBY, pb.BYSU);
  pc.EMAR = and(pb.DYLA, pb.DULO);
  pc.ETOK = and(pb.DYLA, pb.DYLU);
  pc.KYFU = and(pb.DYLA, pb.JULO);
  pc.KAVO = and(pb.DYLA, pb.KOPU);
  pc.FEGA = and(pb.DYLA, pb.ETUV);
  pc.FOKE = and(pb.DYLA, pb.FULE);
  pc.FOPU = and(pb.DYLA, pb.GULU);
  pc.EJYF = and(pb.DYLA, pb.DEKE);

  pc.APAJ = nor(pb.AJUX, gb.p09.APU_RESET);
  pc.BOVU = nor(pb.AMAC, gb.p09.APU_RESET);
  pc.BOXU = nor(pb.BASO, gb.p09.APU_RESET);
  pc.ESEL = nor(pb.EMAR, gb.p09.APU_RESET);
  pc.ELUF = nor(pb.ETOK, gb.p09.APU_RESET);
  pc.KAJU = nor(pb.KYFU, gb.p09.APU_RESET);
  pc.KAPO = nor(pb.KAVO, gb.p09.APU_RESET);
  pc.GAMO = nor(pb.FEGA, gb.p09.APU_RESET);
  pc.GYFU = nor(pb.FOKE, gb.p09.APU_RESET);
  pc.GATO = nor(pb.FOPU, gb.p09.APU_RESET);
  pc.EFOR = nor(pb.EJYF, gb.p09.APU_RESET);

  pc.ARYL = not(pb.FF10_D3n);

  wire GUXA_C = add_c(!pb.GALO, !pb.HORA, pb.ARYL);
  wire HALU_C = add_c(!pb.JODE, !pb.HOPA, GUXA_C);
  wire JULE_C = add_c(!pb.KARE, !pb.HELE, HALU_C);
  wire JORY_C = add_c(!pb.JYME, !pb.JAPE, JULE_C);
  wire HEXO_C = add_c(!pb.GYME, !pb.JETE, JORY_C);
  wire GEVA_C = add_c(!pb.FAXO, !pb.GELE, HEXO_C);
  wire FEGO_C = add_c(!pb.EXAP, !pb.EPYR, GEVA_C);
  wire ETEK_C = add_c(!pb.DELE, !pb.EDOK, FEGO_C);
  wire DYXE_C = add_c(!pb.DEXE, !pb.DEFA, ETEK_C);
  wire DULE_C = add_c(!pb.DOFY, !pb.ETER, DYXE_C);
  wire CORU_C = add_c(!pb.DOLY, !pb.DEVA, DULE_C);

  wire GUXA_S = add_s(!pb.GALO, !pb.HORA, pb.ARYL);
  wire HALU_S = add_s(!pb.JODE, !pb.HOPA, GUXA_C);
  wire JULE_S = add_s(!pb.KARE, !pb.HELE, HALU_C);
  wire JORY_S = add_s(!pb.JYME, !pb.JAPE, JULE_C);
  wire HEXO_S = add_s(!pb.GYME, !pb.JETE, JORY_C);
  wire GEVA_S = add_s(!pb.FAXO, !pb.GELE, HEXO_C);
  wire FEGO_S = add_s(!pb.EXAP, !pb.EPYR, GEVA_C);
  wire ETEK_S = add_s(!pb.DELE, !pb.EDOK, FEGO_C);
  wire DYXE_S = add_s(!pb.DEXE, !pb.DEFA, ETEK_C);
  wire DULE_S = add_s(!pb.DOFY, !pb.ETER, DYXE_C);
  wire CORU_S = add_s(!pb.DOLY, !pb.DEVA, DULE_C);

  pc.AXAN = srtock_pos(pa.BUSO, pb.BUSO, pb.AFEG, pb.APAJ, pc.AXAN, CORU_S);
  pc.EVAB = srtock_pos(pa.BUSO, pb.BUSO, pb.BUDO, pb.BOVU, pc.EVAB, DULE_S);
  pc.DYGY = srtock_pos(pa.BUSO, pb.BUSO, pb.BUGU, pb.BOXU, pc.DYGY, DYXE_S);
  pc.HOPO = srtock_pos(pa.BOJE, pb.BOJE, pb.ETOL, pb.ESEL, pc.HOPO, ETEK_S);
  pc.HYXU = srtock_pos(pa.BOJE, pb.BOJE, pb.ELER, pb.ELUF, pc.HYXU, FEGO_S);
  pc.HOLU = srtock_pos(pa.BOJE, pb.BOJE, pb.KYPA, pb.KAJU, pc.HOLU, GEVA_S);
  pc.FELY = srtock_pos(pa.BOJE, pb.BOJE, pb.KOVU, pb.KAPO, pc.FELY, HEXO_S);
  pc.EDUL = srtock_pos(pa.BOJE, pb.BOJE, pb.GOPE, pb.GAMO, pc.EDUL, JORY_S);
  pc.HAVO = srtock_pos(pa.BOJE, pb.BOJE, pb.GOLO, pb.GYFU, pc.HAVO, JULE_S);
  pc.JYKA = srtock_pos(pa.BOJE, pb.BOJE, pb.GETA, pb.GATO, pc.JYKA, HALU_S);
  pc.HYKA = srtock_pos(pa.BOJE, pb.BOJE, pb.GYLU, pb.EFOR, pc.HYKA, GUXA_S);


  pc.DOLY = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.DOLY, pb.AXAN);
  pc.DOFY = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.DOFY, pb.EVAB);
  pc.DEXE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.DEXE, pb.DYGY);
  pc.DELE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.DELE, pb.HOPO);
  pc.EXAP = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.EXAP, pb.HYXU);
  pc.FAXO = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.FAXO, pb.HOLU);
  pc.GYME = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.GYME, pb.FELY);
  pc.JYME = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.JYME, pb.EDUL);
  pc.KARE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.KARE, pb.HAVO);
  pc.JODE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.JODE, pb.JYKA);
  pc.GALO = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pb.GALO, pb.HYKA);

  pc.BOJO = not(pb.AXAN);
  pc.APAT = not(pb.EVAB);
  pc.BYRU = not(pb.DYGY);
  pc.CYKY = not(pb.HOPO);
  pc.DEBO = not(pb.HYXU);
  pc.FOHY = not(pb.HOLU);
  pc.KOVO = not(pb.FELY);
  pc.KEKE = not(pb.EDUL);
  pc.HUNY = not(pb.HAVO);
  pc.HOXE = not(pb.JYKA);
  pc.JUTA = not(pb.HYKA);

  pc.KEDO = not(pb.KALA);
  pc.JUJU = not(pb.KALA);
  pc.KAPE = not(pb.KALA);

  pc.AFYR = and(pb.KEDO, pb.BOJO);
  pc.BUVO = and(pb.KEDO, pb.APAT);
  pc.AFUG = and(pb.KEDO, pb.BYRU);
  pc.BAPU = and(pb.KEDO, pb.CYKY);
  pc.EREG = and(pb.JUJU, pb.DEBO);
  pc.EVOF = and(pb.JUJU, pb.FOHY);
  pc.KEVY = and(pb.JUJU, pb.KOVO);
  pc.KAXY = and(pb.JUJU, pb.KEKE);
  pc.JEHY = and(pb.KAPE, pb.HUNY);
  pc.JOCY = and(pb.KAPE, pb.HOXE);
  pc.KOKO = and(pb.KAPE, pb.JUTA);

  pc.BEJU = nand(pb.AXAN, pb.KEDO);
  pc.BESO = nand(pb.EVAB, pb.KEDO);
  pc.BEGE = nand(pb.DYGY, pb.KEDO);
  pc.DACE = nand(pb.HOPO, pb.KEDO);
  pc.EKEM = nand(pb.HYXU, pb.JUJU);
  pc.GOVO = nand(pb.HOLU, pb.JUJU);
  pc.KOLA = nand(pb.FELY, pb.JUJU);
  pc.KYRY = nand(pb.EDUL, pb.JUJU);
  pc.HAWY = nand(pb.HAVO, pb.KAPE);
  pc.HOLA = nand(pb.JYKA, pb.KAPE);
  pc.HOZU = nand(pb.HYKA, pb.KAPE);

  pc.AVUF = nor(gb.p09.APU_RESET, pb.AFYR);
  pc.AFUX = nor(gb.p09.APU_RESET, pb.BUVO);
  pc.AGOR = nor(gb.p09.APU_RESET, pb.AFUG);
  pc.BEWO = nor(gb.p09.APU_RESET, pb.BAPU);
  pc.ENOK = nor(gb.p09.APU_RESET, pb.EREG);
  pc.EZUK = nor(gb.p09.APU_RESET, pb.EVOF);
  pc.KYBO = nor(gb.p09.APU_RESET, pb.KEVY);
  pc.KETO = nor(gb.p09.APU_RESET, pb.KAXY);
  pc.HYVU = nor(gb.p09.APU_RESET, pb.JEHY);
  pc.HOBU = nor(gb.p09.APU_RESET, pb.JOCY);
  pc.JADO = nor(gb.p09.APU_RESET, pb.KOKO);

  pc.FAJA = not(pb.EGOR);
  pc.EJYB = not(pb.FAJA);
  pc.CYBE = not(pb.EJYB);
  pc.BECY = not(pb.CYBE);

  pc.BEKU = srtock_pos(pa.BECY, pb.BECY, pb.BEJU, pb.AVUF, pb.BEKU, gb.chip.P10_B);
  pc.AGEZ = srtock_pos(pa.BECY, pb.BECY, pb.BESO, pb.AFUX, pb.AGEZ, pb.BEKU);
  pc.ELUX = srtock_pos(pa.BECY, pb.BECY, pb.BEGE, pb.AGOR, pb.ELUX, pb.AGEZ);
  pc.EXAC = srtock_pos(pa.BECY, pb.BECY, pb.DACE, pb.BEWO, pb.EXAC, pb.ELUX);
  pc.FEDO = srtock_pos(pa.EJYB, pb.EJYB, pb.EKEM, pb.ENOK, pb.FEDO, pb.EXAC);
  pc.FUDE = srtock_pos(pa.EJYB, pb.EJYB, pb.GOVO, pb.EZUK, pb.FUDE, pb.FEDO);
  pc.JOTA = srtock_pos(pa.EJYB, pb.EJYB, pb.KOLA, pb.KYBO, pb.JOTA, pb.FUDE);
  pc.JOLU = srtock_pos(pa.EJYB, pb.EJYB, pb.KYRY, pb.KETO, pb.JOLU, pb.JOTA);
  pc.GOGA = srtock_pos(pa.EGOR, pb.EGOR, pb.HAWY, pb.HYVU, pb.GOGA, pb.JOLU);
  pc.JEFA = srtock_pos(pa.EGOR, pb.EGOR, pb.HOLA, pb.HOBU, pb.JEFA, pb.GOGA);
  pc.FABU = srtock_pos(pa.EGOR, pb.EGOR, pb.HOZU, pb.JADO, pb.FABU, pb.JEFA);

  pc.CULU = xor(pb.ARYL, pb.BEKU);
  pc.DOZY = xor(pb.ARYL, pb.AGEZ);
  pc.CALE = xor(pb.ARYL, pb.ELUX);
  pc.DYME = xor(pb.ARYL, pb.EXAC);
  pc.FURE = xor(pb.ARYL, pb.FEDO);
  pc.GOLY = xor(pb.ARYL, pb.FUDE);
  pc.KEFE = xor(pb.ARYL, pb.JOTA);
  pc.HEFY = xor(pb.ARYL, pb.JOLU);
  pc.GOPO = xor(pb.ARYL, pb.GOGA);
  pc.GELA = xor(pb.ARYL, pb.JEFA);
  pc.GYLO = xor(pb.ARYL, pb.FABU);

  pc.DEVA = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.DEVA, pb.CULU);
  pc.ETER = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.ETER, pb.DOZY);
  pc.DEFA = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.DEFA, pb.CALE);
  pc.EDOK = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.EDOK, pb.DYME);
  pc.EPYR = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.EPYR, pb.FURE);
  pc.GELE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.GELE, pb.GOLY);
  pc.JETE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.JETE, pb.KEFE);
  pc.JAPE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.JAPE, pb.HEFY);
  pc.HELE = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.HELE, pb.GOPO);
  pc.HOPA = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.HOPA, pb.GELA);
  pc.HORA = tock_pos(pa.ADAD, pb.ADAD, pb.KYLY, pc.HORA, pb.GYLO);

  pc.BYLE = nor(pb.ARYL, CORU_C);
  pc.ATYS = or(pb.BYLE, pb.ARYL);
}
