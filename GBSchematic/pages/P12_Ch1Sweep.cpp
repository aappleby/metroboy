#include "P12_Ch1Sweep.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P12_Ch1Sweep::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p12.ARYL*/ c.p12.ARYL = not(b.p11.CH1_SWEEP_DIR);
  /*p12.KEDO*/ c.p12.KEDO = not(b.p13.KALA);
  /*p12.JUJU*/ c.p12.JUJU = not(b.p13.KALA);
  /*p12.KAPE*/ c.p12.KAPE = not(b.p13.KALA);
  /*p12.FAJA*/ c.p12.FAJA = not(b.p13.EGOR);
  /*p12.CYBE*/ c.p12.CYBE = not(b.p12.EJYB);
  /*p12.BECY*/ c.p12.BECY = not(b.p12.CYBE);
  /*p12.EJYB*/ c.p12.EJYB = not(b.p12.FAJA);

  /*p12.DEPU*/ c.p12.FREQ_WRa = nand(b.p10.APU_WR, b.p10.ADDR_FF13); // polarity?
  /*p12.DEBY*/ c.p12.FREQ_WRb = and(b.p10.APU_WR, b.p10.ADDR_FF14); // polarity?
  /*p12.DYLA*/ c.p12.FREQ_WRc = not(b.p12.FREQ_WRa);

  // axan.set
  /*p12.AFEG*/ c.p12.FREQ_SUM_SET_10 = nand(b.p12.FREQ_WRb, b.D2);
  /*p12.BUDO*/ c.p12.FREQ_SUM_SET_09 = nand(b.p12.FREQ_WRb, b.D1);
  /*p12.BUGU*/ c.p12.FREQ_SUM_SET_08 = nand(b.p12.FREQ_WRb, b.D0);
  /*p12.ETOL*/ c.p12.FREQ_SUM_SET_07 = nand(b.p12.FREQ_WRc, b.D7);
  /*p12.ELER*/ c.p12.FREQ_SUM_SET_06 = nand(b.p12.FREQ_WRc, b.D6);
  /*p12.KYPA*/ c.p12.FREQ_SUM_SET_05 = nand(b.p12.FREQ_WRc, b.D5);
  /*p12.KOVU*/ c.p12.FREQ_SUM_SET_04 = nand(b.p12.FREQ_WRc, b.D4);
  /*p12.GOPE*/ c.p12.FREQ_SUM_SET_03 = nand(b.p12.FREQ_WRc, b.D3);
  /*p12.GOLO*/ c.p12.FREQ_SUM_SET_02 = nand(b.p12.FREQ_WRc, b.D2);
  /*p12.GETA*/ c.p12.FREQ_SUM_SET_01 = nand(b.p12.FREQ_WRc, b.D1);
  /*p12.GYLU*/ c.p12.FREQ_SUM_SET_00 = nand(b.p12.FREQ_WRc, b.D0);

  /*p12.BYFU*/ c.p12.FREQ_D10n = not(b.D2);
  /*p12.BOFU*/ c.p12.FREQ_D09n = not(b.D1);
  /*p12.BYSU*/ c.p12.FREQ_D08n = not(b.D0);
  /*p12.DULO*/ c.p12.FREQ_D07n = not(b.D7);
  /*p12.DYLU*/ c.p12.FREQ_D06n = not(b.D6);
  /*p12.JULO*/ c.p12.FREQ_D05n = not(b.D5);
  /*p12.KOPU*/ c.p12.FREQ_D04n = not(b.D4);
  /*p12.ETUV*/ c.p12.FREQ_D03n = not(b.D3);
  /*p12.FULE*/ c.p12.FREQ_D02n = not(b.D2);
  /*p12.GULU*/ c.p12.FREQ_D01n = not(b.D1);
  /*p12.DEKE*/ c.p12.FREQ_D00n = not(b.D0);

  /*p12.AJUX*/ c.p12.AJUX = and(b.p12.FREQ_WRb, b.p12.FREQ_D10n);
  /*p12.AMAC*/ c.p12.AMAC = and(b.p12.FREQ_WRb, b.p12.FREQ_D09n);
  /*p12.BASO*/ c.p12.BASO = and(b.p12.FREQ_WRb, b.p12.FREQ_D08n);
  /*p12.EMAR*/ c.p12.EMAR = and(b.p12.FREQ_WRc, b.p12.FREQ_D07n);
  /*p12.ETOK*/ c.p12.ETOK = and(b.p12.FREQ_WRc, b.p12.FREQ_D06n);
  /*p12.KYFU*/ c.p12.KYFU = and(b.p12.FREQ_WRc, b.p12.FREQ_D05n);
  /*p12.KAVO*/ c.p12.KAVO = and(b.p12.FREQ_WRc, b.p12.FREQ_D04n);
  /*p12.FEGA*/ c.p12.FEGA = and(b.p12.FREQ_WRc, b.p12.FREQ_D03n);
  /*p12.FOKE*/ c.p12.FOKE = and(b.p12.FREQ_WRc, b.p12.FREQ_D02n);
  /*p12.FOPU*/ c.p12.FOPU = and(b.p12.FREQ_WRc, b.p12.FREQ_D01n);
  /*p12.EJYF*/ c.p12.EJYF = and(b.p12.FREQ_WRc, b.p12.FREQ_D00n);

  /*p12.APAJ*/ c.p12.FREQ_SUM_RST_10 = nor(b.p12.AJUX, b.p09.APU_RESET1);
  /*p12.BOVU*/ c.p12.FREQ_SUM_RST_09 = nor(b.p12.AMAC, b.p09.APU_RESET1);
  /*p12.BOXU*/ c.p12.FREQ_SUM_RST_08 = nor(b.p12.BASO, b.p09.APU_RESET1);
  /*p12.ESEL*/ c.p12.FREQ_SUM_RST_07 = nor(b.p12.EMAR, b.p09.APU_RESET1);
  /*p12.ELUF*/ c.p12.FREQ_SUM_RST_06 = nor(b.p12.ETOK, b.p09.APU_RESET1);
  /*p12.KAJU*/ c.p12.FREQ_SUM_RST_05 = nor(b.p12.KYFU, b.p09.APU_RESET1);
  /*p12.KAPO*/ c.p12.FREQ_SUM_RST_04 = nor(b.p12.KAVO, b.p09.APU_RESET1);
  /*p12.GAMO*/ c.p12.FREQ_SUM_RST_03 = nor(b.p12.FEGA, b.p09.APU_RESET1);
  /*p12.GYFU*/ c.p12.FREQ_SUM_RST_02 = nor(b.p12.FOKE, b.p09.APU_RESET1);
  /*p12.GATO*/ c.p12.FREQ_SUM_RST_01 = nor(b.p12.FOPU, b.p09.APU_RESET1);
  /*p12.EFOR*/ c.p12.FREQ_SUM_RST_00 = nor(b.p12.EJYF, b.p09.APU_RESET1);

  /*p12.DOLY*/ c.p12.DOLY = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DOLY, b.p12.FREQ_SUM_OUT_10);
  /*p12.DOFY*/ c.p12.DOFY = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DOFY, b.p12.FREQ_SUM_OUT_09);
  /*p12.DEXE*/ c.p12.DEXE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DEXE, b.p12.FREQ_SUM_OUT_08);
  /*p12.DELE*/ c.p12.DELE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.DELE, b.p12.FREQ_SUM_OUT_07);
  /*p12.EXAP*/ c.p12.EXAP = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.EXAP, b.p12.FREQ_SUM_OUT_06);
  /*p12.FAXO*/ c.p12.FAXO = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FAXO, b.p12.FREQ_SUM_OUT_05);
  /*p12.GYME*/ c.p12.GYME = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.GYME, b.p12.FREQ_SUM_OUT_04);
  /*p12.JYME*/ c.p12.JYME = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.JYME, b.p12.FREQ_SUM_OUT_03);
  /*p12.KARE*/ c.p12.KARE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.KARE, b.p12.FREQ_SUM_OUT_02);
  /*p12.JODE*/ c.p12.JODE = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.JODE, b.p12.FREQ_SUM_OUT_01);
  /*p12.GALO*/ c.p12.GALO = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.GALO, b.p12.FREQ_SUM_OUT_00);

  /*p12.DEVA*/ c.p12.DEVA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.DEVA, b.p12.FREQ_SHIFT_FLIP_10);
  /*p12.ETER*/ c.p12.ETER = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.ETER, b.p12.FREQ_SHIFT_FLIP_09);
  /*p12.DEFA*/ c.p12.DEFA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.DEFA, b.p12.FREQ_SHIFT_FLIP_08);
  /*p12.EDOK*/ c.p12.EDOK = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.EDOK, b.p12.FREQ_SHIFT_FLIP_07);
  /*p12.EPYR*/ c.p12.EPYR = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.EPYR, b.p12.FREQ_SHIFT_FLIP_06);
  /*p12.GELE*/ c.p12.GELE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.GELE, b.p12.FREQ_SHIFT_FLIP_05);
  /*p12.JETE*/ c.p12.JETE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.JETE, b.p12.FREQ_SHIFT_FLIP_04);
  /*p12.JAPE*/ c.p12.JAPE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.JAPE, b.p12.FREQ_SHIFT_FLIP_03);
  /*p12.HELE*/ c.p12.HELE = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.HELE, b.p12.FREQ_SHIFT_FLIP_02);
  /*p12.HOPA*/ c.p12.HOPA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.HOPA, b.p12.FREQ_SHIFT_FLIP_01);
  /*p12.HORA*/ c.p12.HORA = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.HORA, b.p12.FREQ_SHIFT_FLIP_00);


  // FIXME - this is kinda weird
  /*p12.GUXA*/ wire GUXA_C = add_c(!b.p12.GALO, !b.p12.HORA, b.p12.ARYL);
  /*p12.HALU*/ wire HALU_C = add_c(!b.p12.JODE, !b.p12.HOPA, GUXA_C);
  /*p12.JULE*/ wire JULE_C = add_c(!b.p12.KARE, !b.p12.HELE, HALU_C);
  /*p12.JORY*/ wire JORY_C = add_c(!b.p12.JYME, !b.p12.JAPE, JULE_C);
  /*p12.HEXO*/ wire HEXO_C = add_c(!b.p12.GYME, !b.p12.JETE, JORY_C);
  /*p12.GEVA*/ wire GEVA_C = add_c(!b.p12.FAXO, !b.p12.GELE, HEXO_C);
  /*p12.FEGO*/ wire FEGO_C = add_c(!b.p12.EXAP, !b.p12.EPYR, GEVA_C);
  /*p12.ETEK*/ wire ETEK_C = add_c(!b.p12.DELE, !b.p12.EDOK, FEGO_C);
  /*p12.DYXE*/ wire DYXE_C = add_c(!b.p12.DEXE, !b.p12.DEFA, ETEK_C);
  /*p12.DULE*/ wire DULE_C = add_c(!b.p12.DOFY, !b.p12.ETER, DYXE_C);
  /*p12.CORU*/ wire CORU_C = add_c(!b.p12.DOLY, !b.p12.DEVA, DULE_C);

  /*p12.GUXA*/ wire GUXA_S = add_s(!b.p12.GALO, !b.p12.HORA, b.p12.ARYL);
  /*p12.HALU*/ wire HALU_S = add_s(!b.p12.JODE, !b.p12.HOPA, GUXA_C);
  /*p12.JULE*/ wire JULE_S = add_s(!b.p12.KARE, !b.p12.HELE, HALU_C);
  /*p12.JORY*/ wire JORY_S = add_s(!b.p12.JYME, !b.p12.JAPE, JULE_C);
  /*p12.HEXO*/ wire HEXO_S = add_s(!b.p12.GYME, !b.p12.JETE, JORY_C);
  /*p12.GEVA*/ wire GEVA_S = add_s(!b.p12.FAXO, !b.p12.GELE, HEXO_C);
  /*p12.FEGO*/ wire FEGO_S = add_s(!b.p12.EXAP, !b.p12.EPYR, GEVA_C);
  /*p12.ETEK*/ wire ETEK_S = add_s(!b.p12.DELE, !b.p12.EDOK, FEGO_C);
  /*p12.DYXE*/ wire DYXE_S = add_s(!b.p12.DEXE, !b.p12.DEFA, ETEK_C);
  /*p12.DULE*/ wire DULE_S = add_s(!b.p12.DOFY, !b.p12.ETER, DYXE_C);
  /*p12.CORU*/ wire CORU_S = add_s(!b.p12.DOLY, !b.p12.DEVA, DULE_C);

  // this is overflow/underflow check
  /*p12.BYLE*/ c.p12.BYLE = nor(b.p12.ARYL, CORU_C);
  /*p12.ATYS*/ c.p12.ATYS = or(b.p12.BYLE, b.p12.ARYL);

  // ->P11
  /*p12.AXAN*/ c.p12.FREQ_SUM_OUT_10 = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.FREQ_SUM_SET_10, b.p12.FREQ_SUM_RST_10, c.p12.FREQ_SUM_OUT_10, CORU_S);
  /*p12.EVAB*/ c.p12.FREQ_SUM_OUT_09 = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.FREQ_SUM_SET_09, b.p12.FREQ_SUM_RST_09, c.p12.FREQ_SUM_OUT_09, DULE_S);
  /*p12.DYGY*/ c.p12.FREQ_SUM_OUT_08 = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.FREQ_SUM_SET_08, b.p12.FREQ_SUM_RST_08, c.p12.FREQ_SUM_OUT_08, DYXE_S);
  /*p12.HOPO*/ c.p12.FREQ_SUM_OUT_07 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_07, b.p12.FREQ_SUM_RST_07, c.p12.FREQ_SUM_OUT_07, ETEK_S);
  /*p12.HYXU*/ c.p12.FREQ_SUM_OUT_06 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_06, b.p12.FREQ_SUM_RST_06, c.p12.FREQ_SUM_OUT_06, FEGO_S);
  /*p12.HOLU*/ c.p12.FREQ_SUM_OUT_05 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_05, b.p12.FREQ_SUM_RST_05, c.p12.FREQ_SUM_OUT_05, GEVA_S);
  /*p12.FELY*/ c.p12.FREQ_SUM_OUT_04 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_04, b.p12.FREQ_SUM_RST_04, c.p12.FREQ_SUM_OUT_04, HEXO_S);
  /*p12.EDUL*/ c.p12.FREQ_SUM_OUT_03 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_03, b.p12.FREQ_SUM_RST_03, c.p12.FREQ_SUM_OUT_03, JORY_S);
  /*p12.HAVO*/ c.p12.FREQ_SUM_OUT_02 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_02, b.p12.FREQ_SUM_RST_02, c.p12.FREQ_SUM_OUT_02, JULE_S);
  /*p12.JYKA*/ c.p12.FREQ_SUM_OUT_01 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_01, b.p12.FREQ_SUM_RST_01, c.p12.FREQ_SUM_OUT_01, HALU_S);
  /*p12.HYKA*/ c.p12.FREQ_SUM_OUT_00 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_00, b.p12.FREQ_SUM_RST_00, c.p12.FREQ_SUM_OUT_00, GUXA_S);

  /*p12.BOJO*/ c.p12.FREQ_SUM_10n = not(b.p12.FREQ_SUM_OUT_10);
  /*p12.APAT*/ c.p12.FREQ_SUM_09n = not(b.p12.FREQ_SUM_OUT_09);
  /*p12.BYRU*/ c.p12.FREQ_SUM_08n = not(b.p12.FREQ_SUM_OUT_08);
  /*p12.CYKY*/ c.p12.FREQ_SUM_07n = not(b.p12.FREQ_SUM_OUT_07);
  /*p12.DEBO*/ c.p12.FREQ_SUM_06n = not(b.p12.FREQ_SUM_OUT_06);
  /*p12.FOHY*/ c.p12.FREQ_SUM_05n = not(b.p12.FREQ_SUM_OUT_05);
  /*p12.KOVO*/ c.p12.FREQ_SUM_04n = not(b.p12.FREQ_SUM_OUT_04);
  /*p12.KEKE*/ c.p12.FREQ_SUM_03n = not(b.p12.FREQ_SUM_OUT_03);
  /*p12.HUNY*/ c.p12.FREQ_SUM_02n = not(b.p12.FREQ_SUM_OUT_02);
  /*p12.HOXE*/ c.p12.FREQ_SUM_01n = not(b.p12.FREQ_SUM_OUT_01);
  /*p12.JUTA*/ c.p12.FREQ_SUM_00n = not(b.p12.FREQ_SUM_OUT_00);

  /*p12.AFYR*/ c.p12.AFYR = and(b.p12.KEDO, b.p12.FREQ_SUM_10n);
  /*p12.BUVO*/ c.p12.BUVO = and(b.p12.KEDO, b.p12.FREQ_SUM_09n);
  /*p12.AFUG*/ c.p12.AFUG = and(b.p12.KEDO, b.p12.FREQ_SUM_08n);
  /*p12.BAPU*/ c.p12.BAPU = and(b.p12.KEDO, b.p12.FREQ_SUM_07n);
  /*p12.EREG*/ c.p12.EREG = and(b.p12.JUJU, b.p12.FREQ_SUM_06n);
  /*p12.EVOF*/ c.p12.EVOF = and(b.p12.JUJU, b.p12.FREQ_SUM_05n);
  /*p12.KEVY*/ c.p12.KEVY = and(b.p12.JUJU, b.p12.FREQ_SUM_04n);
  /*p12.KAXY*/ c.p12.KAXY = and(b.p12.JUJU, b.p12.FREQ_SUM_03n);
  /*p12.JEHY*/ c.p12.JEHY = and(b.p12.KAPE, b.p12.FREQ_SUM_02n);
  /*p12.JOCY*/ c.p12.JOCY = and(b.p12.KAPE, b.p12.FREQ_SUM_01n);
  /*p12.KOKO*/ c.p12.KOKO = and(b.p12.KAPE, b.p12.FREQ_SUM_00n);

  /*p12.BEJU*/ c.p12.FREQ_SHIFT_SET_10 = nand(b.p12.FREQ_SUM_OUT_10, b.p12.KEDO);
  /*p12.BESO*/ c.p12.FREQ_SHIFT_SET_09 = nand(b.p12.FREQ_SUM_OUT_09, b.p12.KEDO);
  /*p12.BEGE*/ c.p12.FREQ_SHIFT_SET_08 = nand(b.p12.FREQ_SUM_OUT_08, b.p12.KEDO);
  /*p12.DACE*/ c.p12.FREQ_SHIFT_SET_07 = nand(b.p12.FREQ_SUM_OUT_07, b.p12.KEDO);
  /*p12.EKEM*/ c.p12.FREQ_SHIFT_SET_06 = nand(b.p12.FREQ_SUM_OUT_06, b.p12.JUJU);
  /*p12.GOVO*/ c.p12.FREQ_SHIFT_SET_05 = nand(b.p12.FREQ_SUM_OUT_05, b.p12.JUJU);
  /*p12.KOLA*/ c.p12.FREQ_SHIFT_SET_04 = nand(b.p12.FREQ_SUM_OUT_04, b.p12.JUJU);
  /*p12.KYRY*/ c.p12.FREQ_SHIFT_SET_03 = nand(b.p12.FREQ_SUM_OUT_03, b.p12.JUJU);
  /*p12.HAWY*/ c.p12.FREQ_SHIFT_SET_02 = nand(b.p12.FREQ_SUM_OUT_02, b.p12.KAPE);
  /*p12.HOLA*/ c.p12.FREQ_SHIFT_SET_01 = nand(b.p12.FREQ_SUM_OUT_01, b.p12.KAPE);
  /*p12.HOZU*/ c.p12.FREQ_SHIFT_SET_00 = nand(b.p12.FREQ_SUM_OUT_00, b.p12.KAPE);

  /*p12.AVUF*/ c.p12.FREQ_SHIFT_RST_10 = nor(b.p09.APU_RESET1, b.p12.AFYR);
  /*p12.AFUX*/ c.p12.FREQ_SHIFT_RST_09 = nor(b.p09.APU_RESET1, b.p12.BUVO);
  /*p12.AGOR*/ c.p12.FREQ_SHIFT_RST_08 = nor(b.p09.APU_RESET1, b.p12.AFUG);
  /*p12.BEWO*/ c.p12.FREQ_SHIFT_RST_07 = nor(b.p09.APU_RESET1, b.p12.BAPU);
  /*p12.ENOK*/ c.p12.FREQ_SHIFT_RST_06 = nor(b.p09.APU_RESET1, b.p12.EREG);
  /*p12.EZUK*/ c.p12.FREQ_SHIFT_RST_05 = nor(b.p09.APU_RESET1, b.p12.EVOF);
  /*p12.KYBO*/ c.p12.FREQ_SHIFT_RST_04 = nor(b.p09.APU_RESET1, b.p12.KEVY);
  /*p12.KETO*/ c.p12.FREQ_SHIFT_RST_03 = nor(b.p09.APU_RESET1, b.p12.KAXY);
  /*p12.HYVU*/ c.p12.FREQ_SHIFT_RST_02 = nor(b.p09.APU_RESET1, b.p12.JEHY);
  /*p12.HOBU*/ c.p12.FREQ_SHIFT_RST_01 = nor(b.p09.APU_RESET1, b.p12.JOCY);
  /*p12.JADO*/ c.p12.FREQ_SHIFT_RST_00 = nor(b.p09.APU_RESET1, b.p12.KOKO);

  /*p12.BEKU*/ c.p12.FREQ_SHIFT_10 = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.FREQ_SHIFT_SET_10, b.p12.FREQ_SHIFT_RST_10, b.p12.FREQ_SHIFT_10, b.chip.P10_B);
  /*p12.AGEZ*/ c.p12.FREQ_SHIFT_09 = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.FREQ_SHIFT_SET_09, b.p12.FREQ_SHIFT_RST_09, b.p12.FREQ_SHIFT_09, b.p12.FREQ_SHIFT_10);
  /*p12.ELUX*/ c.p12.FREQ_SHIFT_08 = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.FREQ_SHIFT_SET_08, b.p12.FREQ_SHIFT_RST_08, b.p12.FREQ_SHIFT_08, b.p12.FREQ_SHIFT_09);
  /*p12.EXAC*/ c.p12.FREQ_SHIFT_07 = srtock_pos(a.p12.BECY, b.p12.BECY, b.p12.FREQ_SHIFT_SET_07, b.p12.FREQ_SHIFT_RST_07, b.p12.FREQ_SHIFT_07, b.p12.FREQ_SHIFT_08);
  /*p12.FEDO*/ c.p12.FREQ_SHIFT_06 = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.FREQ_SHIFT_SET_06, b.p12.FREQ_SHIFT_RST_06, b.p12.FREQ_SHIFT_06, b.p12.FREQ_SHIFT_07);
  /*p12.FUDE*/ c.p12.FREQ_SHIFT_05 = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.FREQ_SHIFT_SET_05, b.p12.FREQ_SHIFT_RST_05, b.p12.FREQ_SHIFT_05, b.p12.FREQ_SHIFT_06);
  /*p12.JOTA*/ c.p12.FREQ_SHIFT_04 = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.FREQ_SHIFT_SET_04, b.p12.FREQ_SHIFT_RST_04, b.p12.FREQ_SHIFT_04, b.p12.FREQ_SHIFT_05);
  /*p12.JOLU*/ c.p12.FREQ_SHIFT_03 = srtock_pos(a.p12.EJYB, b.p12.EJYB, b.p12.FREQ_SHIFT_SET_03, b.p12.FREQ_SHIFT_RST_03, b.p12.FREQ_SHIFT_03, b.p12.FREQ_SHIFT_04);
  /*p12.GOGA*/ c.p12.FREQ_SHIFT_02 = srtock_pos(b.p13.EGOR, b.p13.EGOR, b.p12.FREQ_SHIFT_SET_02, b.p12.FREQ_SHIFT_RST_02, b.p12.FREQ_SHIFT_02, b.p12.FREQ_SHIFT_03);
  /*p12.JEFA*/ c.p12.FREQ_SHIFT_01 = srtock_pos(b.p13.EGOR, b.p13.EGOR, b.p12.FREQ_SHIFT_SET_01, b.p12.FREQ_SHIFT_RST_01, b.p12.FREQ_SHIFT_01, b.p12.FREQ_SHIFT_02);
  /*p12.FABU*/ c.p12.FREQ_SHIFT_00 = srtock_pos(b.p13.EGOR, b.p13.EGOR, b.p12.FREQ_SHIFT_SET_00, b.p12.FREQ_SHIFT_RST_00, b.p12.FREQ_SHIFT_00, b.p12.FREQ_SHIFT_01);

  /*p12.CULU*/ c.p12.FREQ_SHIFT_FLIP_10 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_10);
  /*p12.DOZY*/ c.p12.FREQ_SHIFT_FLIP_09 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_09);
  /*p12.CALE*/ c.p12.FREQ_SHIFT_FLIP_08 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_08);
  /*p12.DYME*/ c.p12.FREQ_SHIFT_FLIP_07 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_07);
  /*p12.FURE*/ c.p12.FREQ_SHIFT_FLIP_06 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_06);
  /*p12.GOLY*/ c.p12.FREQ_SHIFT_FLIP_05 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_05);
  /*p12.KEFE*/ c.p12.FREQ_SHIFT_FLIP_04 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_04);
  /*p12.HEFY*/ c.p12.FREQ_SHIFT_FLIP_03 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_03);
  /*p12.GOPO*/ c.p12.FREQ_SHIFT_FLIP_02 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_02);
  /*p12.GELA*/ c.p12.FREQ_SHIFT_FLIP_01 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_01);
  /*p12.GYLO*/ c.p12.FREQ_SHIFT_FLIP_00 = xor(b.p12.ARYL, b.p12.FREQ_SHIFT_00);
}
