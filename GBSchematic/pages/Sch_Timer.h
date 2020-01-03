#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TimerSignals {
  /*p01.UMEK*/ bool DIV_06n;
  /*p01.UREK*/ bool DIV_07n;
  /*p01.UTOK*/ bool DIV_08n;
  /*p01.SAPY*/ bool DIV_09n;
  /*p01.UMER*/ bool DIV_10n;
  /*p01.RAVE*/ bool DIV_11n;
  /*p01.RYSO*/ bool DIV_12n;
  /*p01.UDOR*/ bool DIV_13n;

  /*p03.MOBA*/ bool INT_TIMER;
};

//-----------------------------------------------------------------------------

struct Timer {

  void pwron();
  void reset();
  void commit();

  TimerSignals tick(const SysSignals& sys_sig,
                    const ClkSignals& clk_sig,
                    const RstSignals& rst_sig,
                    const BusSignals& bus_sig,
                    const Decoder&    dec_sig,
                    BusTristates&     bus_tri);

  TimerSignals signals() const;

  int get_div() const {
    return pack(DIV_00, DIV_01, DIV_02, DIV_03, DIV_04, DIV_05, DIV_06, DIV_07,
                DIV_08, DIV_09, DIV_10, DIV_11, DIV_12, DIV_13, DIV_14, DIV_15);
  }

  void set_div(uint16_t x) {
    DIV_00.set2(x & 0x0001); DIV_01.set2(x & 0x0002); DIV_02.set2(x & 0x0004); DIV_03.set2(x & 0x0008);
    DIV_04.set2(x & 0x0010); DIV_05.set2(x & 0x0020); DIV_06.set2(x & 0x0040); DIV_07.set2(x & 0x0080);
    DIV_08.set2(x & 0x0100); DIV_09.set2(x & 0x0200); DIV_10.set2(x & 0x0400); DIV_11.set2(x & 0x0800);
    DIV_12.set2(x & 0x1000); DIV_13.set2(x & 0x2000); DIV_14.set2(x & 0x4000); DIV_15.set2(x & 0x8000);
  }

  int get_tima() const {
    return pack(TIMA_0.v(), TIMA_1.v(), TIMA_2.v(), TIMA_3.v(),
                TIMA_4.v(), TIMA_5.v(), TIMA_6.v(), TIMA_7.v());
  }

  void set_tima(uint8_t x) {
    TIMA_0.set2(x & 0x01); TIMA_1.set2(x & 0x02); TIMA_2.set2(x & 0x04); TIMA_3.set2(x & 0x08);
    TIMA_4.set2(x & 0x10); TIMA_5.set2(x & 0x20); TIMA_6.set2(x & 0x40); TIMA_7.set2(x & 0x80);
  }

  bool get_tima_max()  { return TIMA_MAX; }
  bool get_int_timer() { return INT_TIMER; }

  int get_tma() const {
    return pack(TMA_0, TMA_1, TMA_2, TMA_3, TMA_4, TMA_5, TMA_6, TMA_7);
  }

  void set_tma(uint8_t x) {
    TMA_0.set2(x & 0x01); TMA_1.set2(x & 0x02); TMA_2.set2(x & 0x04); TMA_3.set2(x & 0x08);
    TMA_4.set2(x & 0x10); TMA_5.set2(x & 0x20); TMA_6.set2(x & 0x40); TMA_7.set2(x & 0x80);
  }

  int get_tac() const {
    return pack(TAC_0, TAC_1, TAC_2);
  }

  void set_tac(uint8_t x) {
    TAC_0.set2(x & 1);
    TAC_1.set2(x & 2);
    TAC_2.set2(x & 4);
  }

//private:

  void tickDIV(const SysSignals&   sys_sig,
               const ClkSignals&   clk_sig,
               const BusSignals&   bus_sig,
               const Decoder&      dec_sig,
               const BusTristates& bus_tri);

  void tickTIMA(const SysSignals&   sys_sig,
                const ClkSignals&   clk_sig,
                const RstSignals&   rst_sig,
                const BusSignals&   bus_sig,
                const Decoder&      dec_sig,
                const BusTristates& bus_tri);

  void tickTMA(const RstSignals&   rst_sig,
               const BusSignals&   bus_sig,
               const Decoder&      dec_sig,
               const BusTristates& bus_tri);

  void tickTAC(const RstSignals&   rst_sig,
               const BusSignals&   bus_sig,
               const Decoder&      dec_sig,
               const BusTristates& bus_tri);

  void tickBusRead(const BusSignals& bus_sig,
                   const Decoder&    dec_sig,
                   BusTristates&     bus_tri) const;

  //----------
  // FF04 DIV

  /*p01.UKUP*/ Reg2 DIV_00;
  /*p01.UFOR*/ Reg2 DIV_01;
  /*p01.UNER*/ Reg2 DIV_02;
  /*p01.TERO*/ Reg2 DIV_03;
  /*p01.UNYK*/ Reg2 DIV_04;
  /*p01.TAMA*/ Reg2 DIV_05;
  /*p01.UGOT*/ Reg2 DIV_06;
  /*p01.TULU*/ Reg2 DIV_07;
  /*p01.TUGO*/ Reg2 DIV_08;
  /*p01.TOFE*/ Reg2 DIV_09;
  /*p01.TERU*/ Reg2 DIV_10;
  /*p01.SOLA*/ Reg2 DIV_11;
  /*p01.SUBU*/ Reg2 DIV_12;
  /*p01.TEKA*/ Reg2 DIV_13;
  /*p01.UKET*/ Reg2 DIV_14;
  /*p01.UPOF*/ Reg2 DIV_15;

  //----------
  // FF05 TIMA

  /*p03.REGA*/ Counter TIMA_0;
  /*p03.POVY*/ Counter TIMA_1;
  /*p03.PERU*/ Counter TIMA_2;
  /*p03.RATE*/ Counter TIMA_3;
  /*p03.RUBY*/ Counter TIMA_4;
  /*p03.RAGE*/ Counter TIMA_5;
  /*p03.PEDA*/ Counter TIMA_6;
  /*p03.NUGA*/ Counter TIMA_7;

  /*p03.NYDU*/ Reg2 TIMA_MAX;
  /*p03.MOBA*/ Reg2 INT_TIMER;

  //----------
  // FF06 TMA

  /*p03.SABU*/ Reg2 TMA_0;
  /*p03.NYKE*/ Reg2 TMA_1;
  /*p03.MURU*/ Reg2 TMA_2;
  /*p03.TYVA*/ Reg2 TMA_3;
  /*p03.TYRU*/ Reg2 TMA_4;
  /*p03.SUFY*/ Reg2 TMA_5;
  /*p03.PETO*/ Reg2 TMA_6;
  /*p03.SETA*/ Reg2 TMA_7;

  //----------
  // FF07 TAC

  /*p03.SOPU*/ Reg2 TAC_0;
  /*p03.SAMY*/ Reg2 TAC_1;
  /*p03.SABO*/ Reg2 TAC_2;
};

//-----------------------------------------------------------------------------

};