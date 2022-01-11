#include "metron.h"

//==============================================================================

template<int cycles_per_bit = 4>
struct uart_tx {

  //----------------------------------------

  uint8_t i_data;
  bool    i_req;

  bool    o_serial;
  bool    o_cts;
  bool    o_idle;

  //----------------------------------------
  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages

  static const int extra_stop_bits = 7;

  static const int cycle_bits = clog2(cycles_per_bit);
  static const int cycle_max  = cycles_per_bit - 1;

  static const int cursor_bits = clog2(10 + extra_stop_bits);
  static const int cursor_max  = 10 + extra_stop_bits - 1;

  int cycle;
  int cursor;
  uint16_t buffer;

  //----------------------------------------

  void tick(bool rst_n) {
    o_serial = buffer & 1;
    o_cts    = ((cursor <= extra_stop_bits) && (cycle == 0)) || (cursor < extra_stop_bits);
    o_idle   = (cursor == 0) && (cycle == 0);
  }

  //----------------------------------------

  void tock(bool rst_n) {
    if (!rst_n) {
      cycle  = 0;
      cursor = 0;
      buffer = 0x1FF;
    } else {
      if (cursor <= extra_stop_bits && cycle == 0 && i_req) {
        // Transmit start
        cycle  = cycles_per_bit - 1;
        cursor = 10 + extra_stop_bits - 1;
        buffer = (i_data << 1) | 0;
      }
      else if (cycle != 0) {
        // Bit delay
        cycle  = cycle - 1;
        cursor = cursor;
        buffer = buffer;
      }
      else if (cursor != 0) {
        // Bit delay done, switch to next bit.
        cycle  = cycles_per_bit - 1;
        cursor = cursor - 1;
        buffer = (buffer >> 1) | 0x100;
      }
    }
  }

  //----------------------------------------





















  void dump_header() {
    printf("[tx cyc bit buf serial cts idle] ");
  }

  void dump() {
    printf("[   %-3d %-3d %03x %-6d %-3d %-4d] ", cycle, cursor, buffer, o_serial, o_cts, o_idle);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module tx $end\n");
    fprintf(d.file, "$var wire 5  tx_cycle    cycle    $end\n");
    fprintf(d.file, "$var wire 5  tx_cursor   cursor   $end\n");
    fprintf(d.file, "$var wire 9  tx_buf      buf      $end\n");
    fprintf(d.file, "$var wire 1  tx_o_cts    o_cts    $end\n");
    fprintf(d.file, "$var wire 1  tx_o_idle   o_idle   $end\n");
    fprintf(d.file, "$var wire 1  tx_o_serial o_serial $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("tx_cycle",    cycle, 5);
    d.set_value("tx_cursor",   cursor, 5);
    d.set_value("tx_buf",      buffer, 9);

    d.set_value("tx_o_serial", o_serial, 1);
    d.set_value("tx_o_cts",    o_cts, 1);
    d.set_value("tx_o_idle",   o_idle, 1);
  }
};

//==============================================================================
