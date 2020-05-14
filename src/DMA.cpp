#include "DMA.h"
#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

DMA::DMA() {
  ack = {0};

  mode_x = Mode::NONE;
  count_x = 0;
  source_x = 0x000;

  mode_a = Mode::NONE;
  count_a = 0;
  source_a = 0x000;

  mode_b = Mode::NONE;
  count_b = 0;
  data_b = 0x00;
}

//-----------------------------------------------------------------------------

/*
bool DMA::has_ebus_req() const {
  return (mode_a != Mode::NONE) && ((source_a >> 5) != 4);
}

//----------------------------------------

bool DMA::has_vbus_req() const {
  return (mode_a != Mode::NONE) && ((source_a >> 5) == 4);
}

//----------------------------------------

bool DMA::has_obus_req() const {
  return (mode_b != Mode::NONE);
}
*/

//-----------------------------------------------------------------------------

void DMA::get_ebus_req(Req& r) const {
  r.addr  = uint16_t((source_a << 8) | count_a);
  r.data  = 0;
  r.read  = (mode_a != Mode::NONE) && ((source_a >> 5) != 4);
  r.write = 0;
}

//----------------------------------------

void DMA::get_vbus_req(Req& r) const {
  r.addr  = uint16_t((source_a << 8) | count_a);
  r.data  = 0;
  r.read  = (mode_a != Mode::NONE) && ((source_a >> 5) == 4);
  r.write = 0;
}

//----------------------------------------

void DMA::get_obus_req(Req& r) const {
  r.addr  = uint16_t(ADDR_OAM_BEGIN + count_b);
  r.data  = data_b;
  r.read  = 0;
  r.write = (mode_b != Mode::NONE);
}

//----------------------------------------

void DMA::on_ebus_ack(const Ack& ebus_ack) {
  uint16_t src_addr = uint16_t((source_a << 8) | count_a);
  if (ebus_ack.read == 1 && ebus_ack.addr == src_addr) {
    data_b = uint8_t(ebus_ack.data);
  }
}

//----------------------------------------

void DMA::on_vbus_ack(const Ack& vbus_ack) {
  uint16_t src_addr = uint16_t((source_a << 8) | count_a);
  if (vbus_ack.read == 1 && vbus_ack.addr == src_addr) {
    data_b = uint8_t(vbus_ack.data);
  }
}

//----------------------------------------

void DMA::on_obus_ack(const Ack& /*obus_ack*/) {
  // nothing to do here?
}

//-----------------------------------------------------------------------------

void DMA::ibus_req(const Req& ibus_req) {
  ack = {0};
  if (ibus_req.addr != ADDR_DMA) return;

  if (ibus_req.write) {
    //printf("%08d DMA write 0x%02x\n", tcycle, ibus_req.data);

    if (ibus_req.data <= 0x7F) mode_x = Mode::CART;
    if (0x80 <= ibus_req.data && ibus_req.data <= 0x9F) mode_x = Mode::VRAM;
    if (0xA0 <= ibus_req.data && ibus_req.data <= 0xBF) mode_x = Mode::CART;
    if (0xC0 <= ibus_req.data && ibus_req.data <= 0xFD) mode_x = Mode::IRAM;
    count_x = 0;
    source_x = ibus_req.data;

    ack = {
      .addr  = ibus_req.addr,
      .data  = ibus_req.data,
      .read  = 0,
      .write = 1,
    };
  }

  if (ibus_req.read) {
    ack = {
      .addr  = ibus_req.addr,
      .data  = source_x,
      .read  = 1,
      .write = 0,
    };
  }
}

void DMA::ibus_ack(Ack& ibus_ack) {
  ibus_ack.addr  += ack.addr;
  ibus_ack.data  += ack.data;
  ibus_ack.read  += ack.read;
  ibus_ack.write += ack.write;
}

//-----------------------------------------------------------------------------

void DMA::tock(int tcycle) {
  //if (mode_x != Mode::NONE) __debugbreak();
  //if (mode_a != Mode::NONE) __debugbreak();
  //if (mode_b != Mode::NONE) __debugbreak();

  int tphase = tcycle & 3;
  if (tphase == 0) {
    mode_b = mode_a;
    count_b = count_a;

    mode_a = mode_x;
    count_a = count_x;
    source_a = source_x;

    if (mode_x != Mode::NONE) count_x++;
    if (count_x == 160) mode_x = Mode::NONE;
  }
}

//-----------------------------------------------------------------------------

void DMA::dump(std::string& d) {
  sprintf(d, "\002--------------DMA--------------\001\n");
 
  sprintf(d, "mode_x      %d\n", mode_x);
  sprintf(d, "count_x     %d\n", count_x);
  sprintf(d, "source_x    0x%04x\n", source_x);
  sprintf(d, "\n");
  sprintf(d, "mode_a      %d\n", mode_a);
  sprintf(d, "count_a     %d\n", count_a);
  sprintf(d, "source_a    0x%04x\n", source_a);
  sprintf(d, "\n");
  sprintf(d, "mode_b      %d\n", mode_b);
  sprintf(d, "count_b     %d\n", count_b);
  sprintf(d, "data_b      %d\n", data_b);
}

//-----------------------------------------------------------------------------
