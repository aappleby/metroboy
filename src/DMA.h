#pragma once
#include "Types.h"

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