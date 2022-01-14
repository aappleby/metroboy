#include "metron_top.h"


//==============================================================================

class uart_tx {
public:

  //----------------------------------------

  logic<8> i_data;
  logic<1> i_req;

  logic<1> o_serial;
  logic<1> o_cts;
  logic<1> o_idle;


  /*verilator public_module*/

  //----------------------------------------
  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages

  static const int cycles_per_bit = 3;
  static const int extra_stop_bits = 7;

  static const int cycle_bits = clog2(cycles_per_bit);
  static const int cycle_max = cycles_per_bit - 1;

  static const int cursor_bits = clog2(10 + extra_stop_bits);
  static const int cursor_max = 10 + extra_stop_bits - 1;

  logic<cycle_bits>  cycle;
  logic<cursor_bits> cursor;
  logic<9>           buffer;

  //----------------------------------------

  void tick(bool rst_n) {
    o_serial = buffer & 1;  
    o_cts = ((cursor == extra_stop_bits) && (cycle == 0)) || (cursor < extra_stop_bits);
    o_idle = (cursor == 0) && (cycle == 0);
  }

  //----------------------------------------

  void tock(bool rst_n) {
    if (!rst_n) {
      cycle = 0;
      cursor = 0;
      buffer = 0x1FF;
    } else {
      if (cursor <= extra_stop_bits && cycle == 0 && i_req) {
        // Transmit start
        cycle = cycle_max;
        cursor = cursor_max;
        buffer = i_data << 1;
      }
      else if (cycle != 0) {
        // Bit delay
        cycle = cycle - 1;
        cursor = cursor;
        buffer = buffer;
      }
      else if (cursor != 0) {
        // Bit delay done, switch to next bit.
        cycle = cycle_max;
        cursor = cursor - 1;
        buffer = (buffer >> 1) | 0x100;
      }
    }
  }

  //----------------------------------------

};

//==============================================================================
