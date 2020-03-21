#include "ZRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  *this = {};

  int c = 0;
  ram[c++] = 'h';
  ram[c++] = 'e';
  ram[c++] = 'l';
  ram[c++] = 'l';
  ram[c++] = 'o';
  ram[c++] = ' ';
  ram[c++] = 'w';
  ram[c++] = 'o';
  ram[c++] = 'r';
  ram[c++] = 'l';
  ram[c++] = 'd';
  ram[c++] = 8;
  ram[c++] = 8;
  ram[c++] = 8;
  ram[c++] = 8;
  ram[c++] = 8;
  ram[c++] = 'h';
  ram[c++] = 'e';
  ram[c++] = 'l';
  ram[c++] = 'l';
  ram[c++] = 'o';
  ram[c++] = ' ';
  ram[c++] = 'w';
  ram[c++] = 'o';
  ram[c++] = 'r';
  ram[c++] = 'l';
  ram[c++] = 'd';
  ram[c++] = 8;
  ram[c++] = 8;
  ram[c++] = 8;
  ram[c++] = 8;
  ram[c++] = 8;
}

//-----------------------------------------------------------------------------

Ack ZRAM::on_ibus_req(Req ibus_req) {
  bool zram_hit = ((ibus_req.addr & 0xFF80) == 0xFF80) && (ibus_req.addr != 0xFFFF);
  if (!zram_hit) return {};

  if (ibus_req.write) {
    ram[ibus_req.addr & 0x007F] = (uint8_t)ibus_req.data;
  }

  return {
    .addr  = ibus_req.addr,
    .data  = uint16_t(ibus_req.read ? ram[ibus_req.addr & 0x007F] : 0),
    .read  = ibus_req.read,
    .write = ibus_req.write,
  };
}

//-----------------------------------------------------------------------------

void ZRAM::dump(std::string& d) {
  sprintf(d, "\002---------------ZRAM-------------\001\n");
  for (int y = 0; y < 16; y++) {
    sprintf(d, "%04x: ", y * 8 + ADDR_ZEROPAGE_BEGIN);
    for (int x = 0; x < 8; x++) {
      uint8_t b = ram[x + y * 8];
      uint8_t l = (b >> 0) & 0x0F;
      uint8_t h = (b >> 4) & 0x0F;

      d.push_back(h > 9 ? '7' + h : '0' + h);
      d.push_back(l > 9 ? '7' + l : '0' + l);
      d.push_back(' ');
    }
    d.push_back('\n');
  }
}

//-----------------------------------------------------------------------------
