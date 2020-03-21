#pragma once
#include "Types.h"

struct DMA {

  DMA();

  //----------

  Ack  on_ibus_req(Req ibus_req);

  Req  get_ebus_req(int tcycle);
  Req  get_vbus_req(int tcycle);
  Req  get_obus_req(int tcycle);

  void on_ebus_ack(Ack ibus_ack);
  void on_vbus_ack(Ack vbus_ack);
  void on_obus_ack(Ack obus_ack);

  void tock(int tcycle);
  void dump(std::string& d);

  //----------

  enum DMAMode {
    DMA_NONE,
    DMA_VRAM,
    DMA_IRAM,
    DMA_CART
  };

  DMAMode  dma_mode_x;
  uint8_t  dma_count_x;
  uint16_t dma_source_x;

  DMAMode  dma_mode_a;
  uint8_t  dma_count_a;
  uint16_t dma_source_a;

  DMAMode  dma_mode_b;
  uint8_t  dma_count_b;
  uint8_t  dma_data_b;
};