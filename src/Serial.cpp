#include "Serial.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void Serial::reset() {
  *this = {};
  sb = 0x00;
  sc = 0x7E;
}

//-----------------------------------------------------------------------------

Bus Serial::tick() const {
  return serial_to_bus;
}

void Serial::tock(int tcycle_, Bus bus_to_serial_) {
  const int tphase = tcycle_ & 3;
  if (tphase != 0) return;

  tcycle = tcycle_;
  bus_to_serial = bus_to_serial_;
  serial_to_bus = {};
  if ((bus_to_serial.addr == ADDR_SB) || (bus_to_serial.addr == ADDR_SC)) {
    serial_to_bus = bus_to_serial;
    serial_to_bus.ack = true;
  }

  if (bus_to_serial.write) {
    if (bus_to_serial.addr == ADDR_SB) sb = (uint8_t)bus_to_serial.data;
    if (bus_to_serial.addr == ADDR_SC) sc = (uint8_t)bus_to_serial.data | 0b01111110;
  }
  else if (bus_to_serial.read) {
    if (bus_to_serial.addr == ADDR_SB) serial_to_bus.data = sb;
    if (bus_to_serial.addr == ADDR_SC) serial_to_bus.data = sc;
  }
}

//-----------------------------------------------------------------------------

void Serial::dump(std::string& d) {
  print_bus(d, "bus_to_serial", bus_to_serial);
  print_bus(d, "serial_to_bus", serial_to_bus);
}

//-----------------------------------------------------------------------------
