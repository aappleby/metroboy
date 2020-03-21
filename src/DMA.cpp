#include "DMA.h"
#include "Constants.h"

//-----------------------------------------------------------------------------

DMA::DMA() {
  dma_mode_x = DMA_NONE;
  dma_count_x = 0;
  dma_source_x = 0x000;

  dma_mode_a = DMA_NONE;
  dma_count_a = 0;
  dma_source_a = 0x000;

  dma_mode_b = DMA_NONE;
  dma_count_b = 0;
  dma_data_b = 0x00;
}

//-----------------------------------------------------------------------------

Req DMA::get_ebus_req(int tcycle) {
  int region = dma_source_a >> 5;
  if (dma_mode_a == DMA_NONE || region == 4) return {};

  uint16_t dma_src_addr = uint16_t((dma_source_a << 8) | dma_count_a);

  return {
    .phase = tcycle & 3,
    .addr  = dma_src_addr,
    .data  = 0,
    .read  = 1,
    .write = 0,
  };
}

//----------------------------------------

Req DMA::get_vbus_req(int tcycle) {
  int region = dma_source_a >> 5;
  if (dma_mode_a == DMA_NONE || region != 4) return {};

  uint16_t dma_src_addr = uint16_t((dma_source_a << 8) | dma_count_a);

  return {
    .phase = tcycle & 3,
    .addr  = dma_src_addr,
    .data  = 0,
    .read  = 1,
    .write = 0,
  };
}

//----------------------------------------

Req DMA::get_obus_req(int tcycle) {
  if (dma_mode_b == DMA_NONE) return {};

  uint16_t dma_dst_addr = uint16_t(ADDR_OAM_BEGIN + dma_count_b);

  return {
    .phase = tcycle & 3,
    .addr  = dma_dst_addr,
    .data  = dma_data_b,
    .read  = 0,
    .write = 1,
  };
}

//-----------------------------------------------------------------------------

Ack DMA::on_ibus_req(Req ibus_req) {
  if (ibus_req.addr != ADDR_DMA) return {};

  if (ibus_req.phase == 0 && ibus_req.write) {
    if (ibus_req.data <= 0x7F) dma_mode_x = DMA_CART;
    if (0x80 <= ibus_req.data && ibus_req.data <= 0x9F) dma_mode_x = DMA_VRAM;
    if (0xA0 <= ibus_req.data && ibus_req.data <= 0xBF) dma_mode_x = DMA_CART;
    if (0xC0 <= ibus_req.data && ibus_req.data <= 0xFD) dma_mode_x = DMA_IRAM;
    dma_count_x = 0;
    dma_source_x = ibus_req.data;

    return {
      .phase = ibus_req.phase,
      .addr  = ibus_req.addr,
      .data  = ibus_req.data,
      .read  = 0,
      .write = 1,
    };
  }

  if (ibus_req.read) {
    return {
      .phase = ibus_req.phase,
      .addr  = ibus_req.addr,
      .data  = dma_source_x,
      .read  = 1,
      .write = 0,
    };
  }

  return {};
}

//----------------------------------------

void DMA::on_ebus_ack(Ack ebus_ack) {
  uint16_t dma_src_addr = uint16_t((dma_source_a << 8) | dma_count_a);
  if (ebus_ack.read == 1 && ebus_ack.addr == dma_src_addr) {
    dma_data_b = uint8_t(ebus_ack.data);
  }
}

//----------------------------------------

void DMA::on_vbus_ack(Ack vbus_ack) {
  uint16_t dma_src_addr = uint16_t((dma_source_a << 8) | dma_count_a);
  if (vbus_ack.read == 1 && vbus_ack.addr == dma_src_addr) {
    dma_data_b = uint8_t(vbus_ack.data);
  }
}

//----------------------------------------

void DMA::on_obus_ack(Ack /*obus_ack*/) {
  // nothing to do here?
}

//-----------------------------------------------------------------------------

void DMA::tock(int tcycle) {
  int tphase = tcycle & 3;
  if (tphase == 0) {
    dma_mode_b = dma_mode_a;
    dma_count_b = dma_count_a;

    dma_mode_a = dma_mode_x;
    dma_count_a = dma_count_x;
    dma_source_a = dma_source_x;

    if (dma_mode_x != DMA_NONE) dma_count_x++;
    if (dma_count_x == 160) dma_mode_x = DMA_NONE;
  }
}

//-----------------------------------------------------------------------------

void DMA::dump(std::string& d) {
  sprintf(d, "\002--------------BUS--------------\001\n");
  //sprintf(d, "tcycle         %d\n", tcycle);
 
  sprintf(d, "dma_mode_x     %d\n", dma_mode_x);
  sprintf(d, "dma_count_x    %d\n", dma_count_x);
  sprintf(d, "dma_source_x   0x%04x\n", dma_source_x);
  sprintf(d, "\n");
  sprintf(d, "dma_mode_a     %d\n", dma_mode_a);
  sprintf(d, "dma_count_a    %d\n", dma_count_a);
  sprintf(d, "dma_source_a   0x%04x\n", dma_source_a);
  sprintf(d, "\n");
  sprintf(d, "dma_mode_b     %d\n", dma_mode_b);
  sprintf(d, "dma_count_b    %d\n", dma_count_b);
  sprintf(d, "dma_data_b     %d\n", dma_data_b);
}

//-----------------------------------------------------------------------------
