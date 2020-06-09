#pragma once
#include "Types.h"

#include "Sch_Common.h"

struct DMA {
  void tock_req(const Req& ibus_req);
  void tick_ack(Ack& ibus_ack) const;

  //bool has_ebus_req() const;
  //bool has_vbus_req() const;
  //bool has_obus_req() const;

  void get_ebus_req(Req& r) const;
  void get_vbus_req(Req& r) const;
  void get_obus_req(Req& r) const;

  void on_ebus_ack(const Ack& ibus_ack);
  void on_vbus_ack(const Ack& vbus_ack);
  void on_obus_ack(const Ack& obus_ack);

  void tock(int tcycle);
  void dump(std::string& d);

  //----------

  enum class Mode {
    NONE = 0,
    VRAM,
    IRAM,
    CART
  };

  Ack      ack;

  Mode     mode_x = Mode::NONE;
  uint8_t  count_x = 0;
  uint16_t source_x = 0;

  Mode     mode_a = Mode::NONE;
  uint8_t  count_a = 0;
  uint16_t source_a = 0;

  Mode     mode_b = Mode::NONE;
  uint8_t  count_b = 0;
  uint8_t  data_b = 0;
};


struct DMA2 {

  void tock(int phase, const Req& req);
  void dump(std::string& d);

  int count = 0;

  /*p07.CUPA*/ bool CUPA = 0;
  /*p04.LAVY*/ bool LAVY = 0;
  /*p04.NAVO*/ bool NAVO = 0;
  /*p04.NOLO*/ bool NOLO = 0;
  /*p04.LOKO*/ bool LOKO = 0;
  /*p04.LUPA*/ bool LUPA = 0;
  /*p04.META*/ bool META = 0;
  /*p04.LORU*/ bool LORU = 0;

  /*p04.MATU*/ Schematics::Reg3 MATU;
  /*p04.MYTE*/ Schematics::Reg3 MYTE;
  /*p04.LUVY*/ Schematics::Reg3 LUVY;
  /*p04.LENE*/ Schematics::Reg3 LENE;
  /*p04.LYXE*/ Schematics::Latch3 LYXE;
  /*p04.LARA*/ Schematics::Gate LARA;
  /*p04.LOKY*/ Schematics::Gate LOKY;

  uint8_t addr_hi = 0;
  uint8_t addr_lo = 0;

};