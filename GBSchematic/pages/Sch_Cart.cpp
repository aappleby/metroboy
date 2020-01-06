#include "Sch_Cart.h"
#include "Sch_BusControl.h"
#include "Sch_Pins.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void Cart::pwron() {
}

void Cart::reset() {
}

void Cart::phase_begin() {
}

void Cart::phase_end() {
}

void Cart::pass_begin() {
}

bool Cart::pass_end() {
  return false;
}

void Cart::tick(CartPins& cart_pins) {

  if (!cart_pins.RDn_A && !cart_pins.CSn_A) {
    uint16_t cart_addr = (uint16_t)pack(cart_pins.A00_A, cart_pins.A01_A, cart_pins.A02_A, cart_pins.A03_A,
                                        cart_pins.A04_A, cart_pins.A05_A, cart_pins.A06_A, cart_pins.A07_A,
                                        cart_pins.A08_A, cart_pins.A09_A, cart_pins.A10_A, cart_pins.A11_A,
                                        cart_pins.A12_A, cart_pins.A13_A, cart_pins.A14_A, cart_pins.A15_A);

    if (cart_addr < 0x8000) {
      uint8_t d = rom[cart_addr];
      cart_pins.D0_C = d & 0x01;
      cart_pins.D1_C = d & 0x02;
      cart_pins.D2_C = d & 0x04;
      cart_pins.D3_C = d & 0x08;
      cart_pins.D4_C = d & 0x10;
      cart_pins.D5_C = d & 0x20;
      cart_pins.D6_C = d & 0x40;
      cart_pins.D7_C = d & 0x80;
    }

    // move to ExtRam later
    if (0xC000 <= cart_addr && cart_addr <= 0xDFFF) {
      uint8_t d = ram[cart_addr - 0xC000];
      cart_pins.D0_C = d & 0x01;
      cart_pins.D1_C = d & 0x02;
      cart_pins.D2_C = d & 0x04;
      cart_pins.D3_C = d & 0x08;
      cart_pins.D4_C = d & 0x10;
      cart_pins.D5_C = d & 0x20;
      cart_pins.D6_C = d & 0x40;
      cart_pins.D7_C = d & 0x80;
    }
  }
}


//-----------------------------------------------------------------------------

};