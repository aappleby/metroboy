#include "P12_Ch1Sweep.h"
#include "../Schematics.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------

void P12_Ch1Sweep::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p12.ARYL*/ c.p12.NR10_SWEEP_DIRn = not(b.p11.NR10_SWEEP_DIR);
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

  /*p12.DOLY*/ c.p12.FREQ_SUM_L_10 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_10, b.p12.FREQ_SUM_OUT_10);
  /*p12.DOFY*/ c.p12.FREQ_SUM_L_09 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_09, b.p12.FREQ_SUM_OUT_09);
  /*p12.DEXE*/ c.p12.FREQ_SUM_L_08 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_08, b.p12.FREQ_SUM_OUT_08);
  /*p12.DELE*/ c.p12.FREQ_SUM_L_07 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_07, b.p12.FREQ_SUM_OUT_07);
  /*p12.EXAP*/ c.p12.FREQ_SUM_L_06 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_06, b.p12.FREQ_SUM_OUT_06);
  /*p12.FAXO*/ c.p12.FREQ_SUM_L_05 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_05, b.p12.FREQ_SUM_OUT_05);
  /*p12.GYME*/ c.p12.FREQ_SUM_L_04 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_04, b.p12.FREQ_SUM_OUT_04);
  /*p12.JYME*/ c.p12.FREQ_SUM_L_03 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_03, b.p12.FREQ_SUM_OUT_03);
  /*p12.KARE*/ c.p12.FREQ_SUM_L_02 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_02, b.p12.FREQ_SUM_OUT_02);
  /*p12.JODE*/ c.p12.FREQ_SUM_L_01 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_01, b.p12.FREQ_SUM_OUT_01);
  /*p12.GALO*/ c.p12.FREQ_SUM_L_00 = tock_pos(a.p13.ADAD, a.p13.ADAD, a.p13.KYLY, b.p12.FREQ_SUM_L_00, b.p12.FREQ_SUM_OUT_00);

  /*p12.DEVA*/ c.p12.FREQ_SUM_R_10 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_10, b.p12.FREQ_SHIFT_FLIP_10);
  /*p12.ETER*/ c.p12.FREQ_SUM_R_09 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_09, b.p12.FREQ_SHIFT_FLIP_09);
  /*p12.DEFA*/ c.p12.FREQ_SUM_R_08 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_08, b.p12.FREQ_SHIFT_FLIP_08);
  /*p12.EDOK*/ c.p12.FREQ_SUM_R_07 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_07, b.p12.FREQ_SHIFT_FLIP_07);
  /*p12.EPYR*/ c.p12.FREQ_SUM_R_06 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_06, b.p12.FREQ_SHIFT_FLIP_06);
  /*p12.GELE*/ c.p12.FREQ_SUM_R_05 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_05, b.p12.FREQ_SHIFT_FLIP_05);
  /*p12.JETE*/ c.p12.FREQ_SUM_R_04 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_04, b.p12.FREQ_SHIFT_FLIP_04);
  /*p12.JAPE*/ c.p12.FREQ_SUM_R_03 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_03, b.p12.FREQ_SHIFT_FLIP_03);
  /*p12.HELE*/ c.p12.FREQ_SUM_R_02 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_02, b.p12.FREQ_SHIFT_FLIP_02);
  /*p12.HOPA*/ c.p12.FREQ_SUM_R_01 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_01, b.p12.FREQ_SHIFT_FLIP_01);
  /*p12.HORA*/ c.p12.FREQ_SUM_R_00 = tock_pos(b.p13.ADAD, b.p13.ADAD, b.p13.KYLY, c.p12.FREQ_SUM_R_00, b.p12.FREQ_SHIFT_FLIP_00);

  /*p12.GUXA*/ wire FREQ_SUM_C_00 = add_c(!b.p12.FREQ_SUM_L_00, !b.p12.FREQ_SUM_R_00, b.p12.NR10_SWEEP_DIRn);
  /*p12.HALU*/ wire FREQ_SUM_C_01 = add_c(!b.p12.FREQ_SUM_L_01, !b.p12.FREQ_SUM_R_01, FREQ_SUM_C_00);
  /*p12.JULE*/ wire FREQ_SUM_C_02 = add_c(!b.p12.FREQ_SUM_L_02, !b.p12.FREQ_SUM_R_02, FREQ_SUM_C_01);
  /*p12.JORY*/ wire FREQ_SUM_C_03 = add_c(!b.p12.FREQ_SUM_L_03, !b.p12.FREQ_SUM_R_03, FREQ_SUM_C_02);
  /*p12.HEXO*/ wire FREQ_SUM_C_04 = add_c(!b.p12.FREQ_SUM_L_04, !b.p12.FREQ_SUM_R_04, FREQ_SUM_C_03);
  /*p12.GEVA*/ wire FREQ_SUM_C_05 = add_c(!b.p12.FREQ_SUM_L_05, !b.p12.FREQ_SUM_R_05, FREQ_SUM_C_04);
  /*p12.FEGO*/ wire FREQ_SUM_C_06 = add_c(!b.p12.FREQ_SUM_L_06, !b.p12.FREQ_SUM_R_06, FREQ_SUM_C_05);
  /*p12.ETEK*/ wire FREQ_SUM_C_07 = add_c(!b.p12.FREQ_SUM_L_07, !b.p12.FREQ_SUM_R_07, FREQ_SUM_C_06);
  /*p12.DYXE*/ wire FREQ_SUM_C_08 = add_c(!b.p12.FREQ_SUM_L_08, !b.p12.FREQ_SUM_R_08, FREQ_SUM_C_07);
  /*p12.DULE*/ wire FREQ_SUM_C_09 = add_c(!b.p12.FREQ_SUM_L_09, !b.p12.FREQ_SUM_R_09, FREQ_SUM_C_08);
  /*p12.CORU*/ wire FREQ_SUM_C_10 = add_c(!b.p12.FREQ_SUM_L_10, !b.p12.FREQ_SUM_R_10, FREQ_SUM_C_09);

  /*p12.GUXA*/ wire FREQ_SUM_S_00 = add_s(!b.p12.FREQ_SUM_L_00, !b.p12.FREQ_SUM_R_00, b.p12.NR10_SWEEP_DIRn);
  /*p12.HALU*/ wire FREQ_SUM_S_01 = add_s(!b.p12.FREQ_SUM_L_01, !b.p12.FREQ_SUM_R_01, FREQ_SUM_C_00);
  /*p12.JULE*/ wire FREQ_SUM_S_02 = add_s(!b.p12.FREQ_SUM_L_02, !b.p12.FREQ_SUM_R_02, FREQ_SUM_C_01);
  /*p12.JORY*/ wire FREQ_SUM_S_03 = add_s(!b.p12.FREQ_SUM_L_03, !b.p12.FREQ_SUM_R_03, FREQ_SUM_C_02);
  /*p12.HEXO*/ wire FREQ_SUM_S_04 = add_s(!b.p12.FREQ_SUM_L_04, !b.p12.FREQ_SUM_R_04, FREQ_SUM_C_03);
  /*p12.GEVA*/ wire FREQ_SUM_S_05 = add_s(!b.p12.FREQ_SUM_L_05, !b.p12.FREQ_SUM_R_05, FREQ_SUM_C_04);
  /*p12.FEGO*/ wire FREQ_SUM_S_06 = add_s(!b.p12.FREQ_SUM_L_06, !b.p12.FREQ_SUM_R_06, FREQ_SUM_C_05);
  /*p12.ETEK*/ wire FREQ_SUM_S_07 = add_s(!b.p12.FREQ_SUM_L_07, !b.p12.FREQ_SUM_R_07, FREQ_SUM_C_06);
  /*p12.DYXE*/ wire FREQ_SUM_S_08 = add_s(!b.p12.FREQ_SUM_L_08, !b.p12.FREQ_SUM_R_08, FREQ_SUM_C_07);
  /*p12.DULE*/ wire FREQ_SUM_S_09 = add_s(!b.p12.FREQ_SUM_L_09, !b.p12.FREQ_SUM_R_09, FREQ_SUM_C_08);
  /*p12.CORU*/ wire FREQ_SUM_S_10 = add_s(!b.p12.FREQ_SUM_L_10, !b.p12.FREQ_SUM_R_10, FREQ_SUM_C_09);

  // this is overflow check
  // the logic here seems wrong, like carry should be negated or something
  /*p12.BYLE*/ c.p12.CH1_SWEEP_MAX = nor(b.p12.NR10_SWEEP_DIRn, FREQ_SUM_C_10);
  /*p12.ATYS*/ c.p12.CH1_SWEEP_STOPn = or(b.p12.CH1_SWEEP_MAX, b.p12.NR10_SWEEP_DIRn);

  // ->P11
  /*p12.AXAN*/ c.p12.FREQ_SUM_OUT_10 = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.FREQ_SUM_SET_10, b.p12.FREQ_SUM_RST_10, c.p12.FREQ_SUM_OUT_10, FREQ_SUM_S_10);
  /*p12.EVAB*/ c.p12.FREQ_SUM_OUT_09 = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.FREQ_SUM_SET_09, b.p12.FREQ_SUM_RST_09, c.p12.FREQ_SUM_OUT_09, FREQ_SUM_S_09);
  /*p12.DYGY*/ c.p12.FREQ_SUM_OUT_08 = srtock_pos(a.p13.BUSO, a.p13.BUSO, b.p12.FREQ_SUM_SET_08, b.p12.FREQ_SUM_RST_08, c.p12.FREQ_SUM_OUT_08, FREQ_SUM_S_08);
  /*p12.HOPO*/ c.p12.FREQ_SUM_OUT_07 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_07, b.p12.FREQ_SUM_RST_07, c.p12.FREQ_SUM_OUT_07, FREQ_SUM_S_07);
  /*p12.HYXU*/ c.p12.FREQ_SUM_OUT_06 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_06, b.p12.FREQ_SUM_RST_06, c.p12.FREQ_SUM_OUT_06, FREQ_SUM_S_06);
  /*p12.HOLU*/ c.p12.FREQ_SUM_OUT_05 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_05, b.p12.FREQ_SUM_RST_05, c.p12.FREQ_SUM_OUT_05, FREQ_SUM_S_05);
  /*p12.FELY*/ c.p12.FREQ_SUM_OUT_04 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_04, b.p12.FREQ_SUM_RST_04, c.p12.FREQ_SUM_OUT_04, FREQ_SUM_S_04);
  /*p12.EDUL*/ c.p12.FREQ_SUM_OUT_03 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_03, b.p12.FREQ_SUM_RST_03, c.p12.FREQ_SUM_OUT_03, FREQ_SUM_S_03);
  /*p12.HAVO*/ c.p12.FREQ_SUM_OUT_02 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_02, b.p12.FREQ_SUM_RST_02, c.p12.FREQ_SUM_OUT_02, FREQ_SUM_S_02);
  /*p12.JYKA*/ c.p12.FREQ_SUM_OUT_01 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_01, b.p12.FREQ_SUM_RST_01, c.p12.FREQ_SUM_OUT_01, FREQ_SUM_S_01);
  /*p12.HYKA*/ c.p12.FREQ_SUM_OUT_00 = srtock_pos(a.p13.BOJE, a.p13.BOJE, b.p12.FREQ_SUM_SET_00, b.p12.FREQ_SUM_RST_00, c.p12.FREQ_SUM_OUT_00, FREQ_SUM_S_00);

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

  /*p12.BOJO*/ c.p12.FREQ_SUM_OUT_10n = not(b.p12.FREQ_SUM_OUT_10);
  /*p12.APAT*/ c.p12.FREQ_SUM_OUT_09n = not(b.p12.FREQ_SUM_OUT_09);
  /*p12.BYRU*/ c.p12.FREQ_SUM_OUT_08n = not(b.p12.FREQ_SUM_OUT_08);
  /*p12.CYKY*/ c.p12.FREQ_SUM_OUT_07n = not(b.p12.FREQ_SUM_OUT_07);
  /*p12.DEBO*/ c.p12.FREQ_SUM_OUT_06n = not(b.p12.FREQ_SUM_OUT_06);
  /*p12.FOHY*/ c.p12.FREQ_SUM_OUT_05n = not(b.p12.FREQ_SUM_OUT_05);
  /*p12.KOVO*/ c.p12.FREQ_SUM_OUT_04n = not(b.p12.FREQ_SUM_OUT_04);
  /*p12.KEKE*/ c.p12.FREQ_SUM_OUT_03n = not(b.p12.FREQ_SUM_OUT_03);
  /*p12.HUNY*/ c.p12.FREQ_SUM_OUT_02n = not(b.p12.FREQ_SUM_OUT_02);
  /*p12.HOXE*/ c.p12.FREQ_SUM_OUT_01n = not(b.p12.FREQ_SUM_OUT_01);
  /*p12.JUTA*/ c.p12.FREQ_SUM_OUT_00n = not(b.p12.FREQ_SUM_OUT_00);

  /*p12.AFYR*/ c.p12.AFYR = and(b.p12.FREQ_SUM_OUT_10n, b.p12.KEDO);
  /*p12.BUVO*/ c.p12.BUVO = and(b.p12.FREQ_SUM_OUT_09n, b.p12.KEDO);
  /*p12.AFUG*/ c.p12.AFUG = and(b.p12.FREQ_SUM_OUT_08n, b.p12.KEDO);
  /*p12.BAPU*/ c.p12.BAPU = and(b.p12.FREQ_SUM_OUT_07n, b.p12.KEDO);
  /*p12.EREG*/ c.p12.EREG = and(b.p12.FREQ_SUM_OUT_06n, b.p12.JUJU);
  /*p12.EVOF*/ c.p12.EVOF = and(b.p12.FREQ_SUM_OUT_05n, b.p12.JUJU);
  /*p12.KEVY*/ c.p12.KEVY = and(b.p12.FREQ_SUM_OUT_04n, b.p12.JUJU);
  /*p12.KAXY*/ c.p12.KAXY = and(b.p12.FREQ_SUM_OUT_03n, b.p12.JUJU);
  /*p12.JEHY*/ c.p12.JEHY = and(b.p12.FREQ_SUM_OUT_02n, b.p12.KAPE);
  /*p12.JOCY*/ c.p12.JOCY = and(b.p12.FREQ_SUM_OUT_01n, b.p12.KAPE);
  /*p12.KOKO*/ c.p12.KOKO = and(b.p12.FREQ_SUM_OUT_00n, b.p12.KAPE);

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

  /*p12.CULU*/ c.p12.FREQ_SHIFT_FLIP_10 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_10);
  /*p12.DOZY*/ c.p12.FREQ_SHIFT_FLIP_09 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_09);
  /*p12.CALE*/ c.p12.FREQ_SHIFT_FLIP_08 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_08);
  /*p12.DYME*/ c.p12.FREQ_SHIFT_FLIP_07 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_07);
  /*p12.FURE*/ c.p12.FREQ_SHIFT_FLIP_06 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_06);
  /*p12.GOLY*/ c.p12.FREQ_SHIFT_FLIP_05 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_05);
  /*p12.KEFE*/ c.p12.FREQ_SHIFT_FLIP_04 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_04);
  /*p12.HEFY*/ c.p12.FREQ_SHIFT_FLIP_03 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_03);
  /*p12.GOPO*/ c.p12.FREQ_SHIFT_FLIP_02 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_02);
  /*p12.GELA*/ c.p12.FREQ_SHIFT_FLIP_01 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_01);
  /*p12.GYLO*/ c.p12.FREQ_SHIFT_FLIP_00 = xor(b.p12.NR10_SWEEP_DIRn, b.p12.FREQ_SHIFT_00);
}
