#include "P12_Ch1Sweep.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P12_Ch1Sweep::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  c.p12.DEPU = nand(b.p10.APU_WR, b.p10.ADDR_FF13); // BUG - APU_WR
  c.p12.DEBY = and(b.p10.APU_WR, b.p10.ADDR_FF14);
  c.p12.DYLA = not(b.p12.DEPU);

  c.p12.BYFU = not(b.D2);
  c.p12.BOFU = not(b.D1);
  c.p12.BYSU = not(b.D0);
  c.p12.DULO = not(b.D7);
  c.p12.DYLU = not(b.D6);
  c.p12.JULO = not(b.D5);
  c.p12.KOPU = not(b.D4);
  c.p12.ETUV = not(b.D3);
  c.p12.FULE = not(b.D2);
  c.p12.GULU = not(b.D1);
  c.p12.DEKE = not(b.D0);

  c.p12.AFEG = nand(b.D2, b.p12.DEBY);
  c.p12.BUDO = nand(b.D1, b.p12.DEBY);
  c.p12.BUGU = nand(b.D0, b.p12.DEBY);
  c.p12.ETOL = nand(b.D7, b.p12.DYLA);
  c.p12.ELER = nand(b.D6, b.p12.DYLA);
  c.p12.KYPA = nand(b.D5, b.p12.DYLA);
  c.p12.KOVU = nand(b.D4, b.p12.DYLA);
  c.p12.GOPE = nand(b.D3, b.p12.DYLA);
  c.p12.GOLO = nand(b.D2, b.p12.DYLA);
  c.p12.GETA = nand(b.D1, b.p12.DYLA);
  c.p12.GYLU = nand(b.D0, b.p12.DYLA);

  c.p12.AJUX = and(b.p12.DEBY, b.p12.BYFU);
  c.p12.AMAC = and(b.p12.DEBY, b.p12.BOFU);
  c.p12.BASO = and(b.p12.DEBY, b.p12.BYSU);
  c.p12.EMAR = and(b.p12.DYLA, b.p12.DULO);
  c.p12.ETOK = and(b.p12.DYLA, b.p12.DYLU);
  c.p12.KYFU = and(b.p12.DYLA, b.p12.JULO);
  c.p12.KAVO = and(b.p12.DYLA, b.p12.KOPU);
  c.p12.FEGA = and(b.p12.DYLA, b.p12.ETUV);
  c.p12.FOKE = and(b.p12.DYLA, b.p12.FULE);
  c.p12.FOPU = and(b.p12.DYLA, b.p12.GULU);
  c.p12.EJYF = and(b.p12.DYLA, b.p12.DEKE);

  c.p12.APAJ = nor(b.p12.AJUX, b.p09.APU_RESET1);
  c.p12.BOVU = nor(b.p12.AMAC, b.p09.APU_RESET1);
  c.p12.BOXU = nor(b.p12.BASO, b.p09.APU_RESET1);
  c.p12.ESEL = nor(b.p12.EMAR, b.p09.APU_RESET1);
  c.p12.ELUF = nor(b.p12.ETOK, b.p09.APU_RESET1);
  c.p12.KAJU = nor(b.p12.KYFU, b.p09.APU_RESET1);
  c.p12.KAPO = nor(b.p12.KAVO, b.p09.APU_RESET1);
  c.p12.GAMO = nor(b.p12.FEGA, b.p09.APU_RESET1);
  c.p12.GYFU = nor(b.p12.FOKE, b.p09.APU_RESET1);
  c.p12.GATO = nor(b.p12.FOPU, b.p09.APU_RESET1);
  c.p12.EFOR = nor(b.p12.EJYF, b.p09.APU_RESET1);

  c.p12.ARYL = not(b.p11.CH1_SWEEP_DIR);

  // FIXME - this is kinda weird
  wire GUXA_C = add_c(!b.p12.GALO, !b.p12.HORA, b.p12.ARYL);
  wire HALU_C = add_c(!b.p12.JODE, !b.p12.HOPA, GUXA_C);
  wire JULE_C = add_c(!b.p12.KARE, !b.p12.HELE, HALU_C);
  wire JORY_C = add_c(!b.p12.JYME, !b.p12.JAPE, JULE_C);
  wire HEXO_C = add_c(!b.p12.GYME, !b.p12.JETE, JORY_C);
  wire GEVA_C = add_c(!b.p12.FAXO, !b.p12.GELE, HEXO_C);
  wire FEGO_C = add_c(!b.p12.EXAP, !b.p12.EPYR, GEVA_C);
  wire ETEK_C = add_c(!b.p12.DELE, !b.p12.EDOK, FEGO_C);
  wire DYXE_C = add_c(!b.p12.DEXE, !b.p12.DEFA, ETEK_C);
  wire DULE_C = add_c(!b.p12.DOFY, !b.p12.ETER, DYXE_C);
  wire CORU_C = add_c(!b.p12.DOLY, !b.p12.DEVA, DULE_C);

  wire GUXA_S = add_s(!b.p12.GALO, !b.p12.HORA, b.p12.ARYL);
  wire HALU_S = add_s(!b.p12.JODE, !b.p12.HOPA, GUXA_C);
  wire JULE_S = add_s(!b.p12.KARE, !b.p12.HELE, HALU_C);
  wire JORY_S = add_s(!b.p12.JYME, !b.p12.JAPE, JULE_C);
  wire HEXO_S = add_s(!b.p12.GYME, !b.p12.JETE, JORY_C);
  wire GEVA_S = add_s(!b.p12.FAXO, !b.p12.GELE, HEXO_C);
  wire FEGO_S = add_s(!b.p12.EXAP, !b.p12.EPYR, GEVA_C);
  wire ETEK_S = add_s(!b.p12.DELE, !b.p12.EDOK, FEGO_C);
  wire DYXE_S = add_s(!b.p12.DEXE, !b.p12.DEFA, ETEK_C);
  wire DULE_S = add_s(!b.p12.DOFY, !b.p12.ETER, DYXE_C);
  wire CORU_S = add_s(!b.p12.DOLY, !b.p12.DEVA, DULE_C);

  c.p12.AXAN = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.AFEG, b.p12.APAJ, c.p12.AXAN, CORU_S);
  c.p12.EVAB = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.BUDO, b.p12.BOVU, c.p12.EVAB, DULE_S);
  c.p12.DYGY = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.BUGU, b.p12.BOXU, c.p12.DYGY, DYXE_S);
  c.p12.HOPO = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.ETOL, b.p12.ESEL, c.p12.HOPO, ETEK_S);
  c.p12.HYXU = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.ELER, b.p12.ELUF, c.p12.HYXU, FEGO_S);
  c.p12.HOLU = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.KYPA, b.p12.KAJU, c.p12.HOLU, GEVA_S);
  c.p12.FELY = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.KOVU, b.p12.KAPO, c.p12.FELY, HEXO_S);
  c.p12.EDUL = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.GOPE, b.p12.GAMO, c.p12.EDUL, JORY_S);
  c.p12.HAVO = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.GOLO, b.p12.GYFU, c.p12.HAVO, JULE_S);
  c.p12.JYKA = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.GETA, b.p12.GATO, c.p12.JYKA, HALU_S);
  c.p12.HYKA = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.GYLU, b.p12.EFOR, c.p12.HYKA, GUXA_S);

  c.p12.DOLY = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DOLY, b.p12.AXAN);
  c.p12.DOFY = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DOFY, b.p12.EVAB);
  c.p12.DEXE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DEXE, b.p12.DYGY);
  c.p12.DELE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DELE, b.p12.HOPO);
  c.p12.EXAP = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.EXAP, b.p12.HYXU);
  c.p12.FAXO = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FAXO, b.p12.HOLU);
  c.p12.GYME = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.GYME, b.p12.FELY);
  c.p12.JYME = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.JYME, b.p12.EDUL);
  c.p12.KARE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.KARE, b.p12.HAVO);
  c.p12.JODE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.JODE, b.p12.JYKA);
  c.p12.GALO = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.GALO, b.p12.HYKA);

  c.p12.BOJO = not(b.p12.AXAN);
  c.p12.APAT = not(b.p12.EVAB);
  c.p12.BYRU = not(b.p12.DYGY);
  c.p12.CYKY = not(b.p12.HOPO);
  c.p12.DEBO = not(b.p12.HYXU);
  c.p12.FOHY = not(b.p12.HOLU);
  c.p12.KOVO = not(b.p12.FELY);
  c.p12.KEKE = not(b.p12.EDUL);
  c.p12.HUNY = not(b.p12.HAVO);
  c.p12.HOXE = not(b.p12.JYKA);
  c.p12.JUTA = not(b.p12.HYKA);

  c.p12.KEDO = not(b.p13.KALA);
  c.p12.JUJU = not(b.p13.KALA);
  c.p12.KAPE = not(b.p13.KALA);

  c.p12.AFYR = and(b.p12.KEDO, b.p12.BOJO);
  c.p12.BUVO = and(b.p12.KEDO, b.p12.APAT);
  c.p12.AFUG = and(b.p12.KEDO, b.p12.BYRU);
  c.p12.BAPU = and(b.p12.KEDO, b.p12.CYKY);
  c.p12.EREG = and(b.p12.JUJU, b.p12.DEBO);
  c.p12.EVOF = and(b.p12.JUJU, b.p12.FOHY);
  c.p12.KEVY = and(b.p12.JUJU, b.p12.KOVO);
  c.p12.KAXY = and(b.p12.JUJU, b.p12.KEKE);
  c.p12.JEHY = and(b.p12.KAPE, b.p12.HUNY);
  c.p12.JOCY = and(b.p12.KAPE, b.p12.HOXE);
  c.p12.KOKO = and(b.p12.KAPE, b.p12.JUTA);

  c.p12.BEJU = nand(b.p12.AXAN, b.p12.KEDO);
  c.p12.BESO = nand(b.p12.EVAB, b.p12.KEDO);
  c.p12.BEGE = nand(b.p12.DYGY, b.p12.KEDO);
  c.p12.DACE = nand(b.p12.HOPO, b.p12.KEDO);
  c.p12.EKEM = nand(b.p12.HYXU, b.p12.JUJU);
  c.p12.GOVO = nand(b.p12.HOLU, b.p12.JUJU);
  c.p12.KOLA = nand(b.p12.FELY, b.p12.JUJU);
  c.p12.KYRY = nand(b.p12.EDUL, b.p12.JUJU);
  c.p12.HAWY = nand(b.p12.HAVO, b.p12.KAPE);
  c.p12.HOLA = nand(b.p12.JYKA, b.p12.KAPE);
  c.p12.HOZU = nand(b.p12.HYKA, b.p12.KAPE);

  c.p12.AVUF = nor(b.p09.APU_RESET1, b.p12.AFYR);
  c.p12.AFUX = nor(b.p09.APU_RESET1, b.p12.BUVO);
  c.p12.AGOR = nor(b.p09.APU_RESET1, b.p12.AFUG);
  c.p12.BEWO = nor(b.p09.APU_RESET1, b.p12.BAPU);
  c.p12.ENOK = nor(b.p09.APU_RESET1, b.p12.EREG);
  c.p12.EZUK = nor(b.p09.APU_RESET1, b.p12.EVOF);
  c.p12.KYBO = nor(b.p09.APU_RESET1, b.p12.KEVY);
  c.p12.KETO = nor(b.p09.APU_RESET1, b.p12.KAXY);
  c.p12.HYVU = nor(b.p09.APU_RESET1, b.p12.JEHY);
  c.p12.HOBU = nor(b.p09.APU_RESET1, b.p12.JOCY);
  c.p12.JADO = nor(b.p09.APU_RESET1, b.p12.KOKO);

  c.p12.FAJA = not(b.p13.EGOR);
  c.p12.EJYB = not(b.p12.FAJA);
  c.p12.CYBE = not(b.p12.EJYB);
  c.p12.BECY = not(b.p12.CYBE);

  c.p12.BEKU = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.BEJU, b.p12.AVUF, b.p12.BEKU, b.chip.P10_B);
  c.p12.AGEZ = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.BESO, b.p12.AFUX, b.p12.AGEZ, b.p12.BEKU);
  c.p12.ELUX = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.BEGE, b.p12.AGOR, b.p12.ELUX, b.p12.AGEZ);
  c.p12.EXAC = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.DACE, b.p12.BEWO, b.p12.EXAC, b.p12.ELUX);
  c.p12.FEDO = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.EKEM, b.p12.ENOK, b.p12.FEDO, b.p12.EXAC);
  c.p12.FUDE = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.GOVO, b.p12.EZUK, b.p12.FUDE, b.p12.FEDO);
  c.p12.JOTA = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.KOLA, b.p12.KYBO, b.p12.JOTA, b.p12.FUDE);
  c.p12.JOLU = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.KYRY, b.p12.KETO, b.p12.JOLU, b.p12.JOTA);
  c.p12.GOGA = srtock_pos(b.p13.EGOR, b.p13.EGOR, b.p12.HAWY, b.p12.HYVU, b.p12.GOGA, b.p12.JOLU);
  c.p12.JEFA = srtock_pos(b.p13.EGOR, b.p13.EGOR, b.p12.HOLA, b.p12.HOBU, b.p12.JEFA, b.p12.GOGA);
  c.p12.FABU = srtock_pos(b.p13.EGOR, b.p13.EGOR, b.p12.HOZU, b.p12.JADO, b.p12.FABU, b.p12.JEFA);

  c.p12.CULU = xor(b.p12.ARYL, b.p12.BEKU);
  c.p12.DOZY = xor(b.p12.ARYL, b.p12.AGEZ);
  c.p12.CALE = xor(b.p12.ARYL, b.p12.ELUX);
  c.p12.DYME = xor(b.p12.ARYL, b.p12.EXAC);
  c.p12.FURE = xor(b.p12.ARYL, b.p12.FEDO);
  c.p12.GOLY = xor(b.p12.ARYL, b.p12.FUDE);
  c.p12.KEFE = xor(b.p12.ARYL, b.p12.JOTA);
  c.p12.HEFY = xor(b.p12.ARYL, b.p12.JOLU);
  c.p12.GOPO = xor(b.p12.ARYL, b.p12.GOGA);
  c.p12.GELA = xor(b.p12.ARYL, b.p12.JEFA);
  c.p12.GYLO = xor(b.p12.ARYL, b.p12.FABU);

  c.p12.DEVA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.DEVA, b.p12.CULU);
  c.p12.ETER = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.ETER, b.p12.DOZY);
  c.p12.DEFA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.DEFA, b.p12.CALE);
  c.p12.EDOK = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.EDOK, b.p12.DYME);
  c.p12.EPYR = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.EPYR, b.p12.FURE);
  c.p12.GELE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.GELE, b.p12.GOLY);
  c.p12.JETE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.JETE, b.p12.KEFE);
  c.p12.JAPE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.JAPE, b.p12.HEFY);
  c.p12.HELE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.HELE, b.p12.GOPO);
  c.p12.HOPA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.HOPA, b.p12.GELA);
  c.p12.HORA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.HORA, b.p12.GYLO);

  c.p12.BYLE = nor(b.p12.ARYL, CORU_C);
  c.p12.ATYS = or(b.p12.BYLE, b.p12.ARYL);
}
