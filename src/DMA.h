#pragma once
#include "Types.h"

struct DMA {

  DMA();

  //----------

  void ibus_req(const Req& ibus_req);
  void ibus_ack(Ack& ibus_ack);

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
    NONE,
    VRAM,
    IRAM,
    CART
  };

  Ack      ack;

  Mode     mode_x;
  uint8_t  count_x;
  uint16_t source_x;

  Mode     mode_a;
  uint8_t  count_a;
  uint16_t source_a;

  Mode     mode_b;
  uint8_t  count_b;
  uint8_t  data_b;
};