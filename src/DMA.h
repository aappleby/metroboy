#pragma once
#include "Types.h"

#include "Sch_Common.h"

struct DMA1 {

  //bool has_ebus_req() const;
  //bool has_vbus_req() const;
  //bool has_obus_req() const;

  void get_ebus_req(Req& r) const;
  void get_vbus_req(Req& r) const;
  void get_obus_req(Req& r) const;

  void on_ebus_ack(const Ack& ebus_ack);
  void on_vbus_ack(const Ack& vbus_ack);

  void tick(int phase, const Req& ibus_req, Ack& ibus_ack) const;
  void tock(int phase, const Req& ibus_req);
  void dump(std::string& d);

  //----------

  enum class Mode {
    NONE = 0,
    VRAM,
    IRAM,
    CART
  };

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

  DMA2();

  void tock(int phase, const Req& req);
  void dump(std::string& d);

  uint16_t addr = 0;

  /*p04.LYXE*/ bool DMA_WR_LATCH;
  /*p04.LUVY*/ bool DMA_RUN_TRIG_d0;
  /*p04.LENE*/ bool DMA_RUN_TRIG_d4;
  /*p04.LOKY*/ bool DMA_RUN_LATCH = 0;
  /*p04.MATU*/ bool DMA_RUN = 0;
  /*p04.MYTE*/ bool DMA_DONE = 0;


};