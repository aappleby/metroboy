#pragma once
#include "Types.h"

struct DMA {

  DMA();

  //----------

  bool on_ibus_req(int tcycle, const Req& ibus_req, Ack& ibus_ack);

  bool has_ebus_req(int tcycle);
  bool has_vbus_req(int tcycle);
  bool has_obus_req(int tcycle);

  Req  get_ebus_req(int tcycle);
  Req  get_vbus_req(int tcycle);
  Req  get_obus_req(int tcycle);

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