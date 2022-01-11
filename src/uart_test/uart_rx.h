#pragma once
#include "metron.h"

//==============================================================================

template<int cycles_per_bit = 4>
struct uart_rx {

  bool     i_serial;

  uint8_t  o_data;
  bool     o_valid;
  uint32_t o_sum;

  //----------------------------------------
  /*verilator public_module*/

  static const int cycle_bits  = clog2(cycles_per_bit);
  static const int cycle_max   = cycles_per_bit - 1;
  static const int cursor_max  = 9;
  static const int cursor_bits = clog2(cursor_max);

  int cycle;
  int cursor;
  uint8_t buffer;
  uint32_t sum;

  //----------------------------------------

  void tick(bool rst_n) {
    o_data  = buffer;
    o_valid = cursor == 1;
    o_sum   = sum;
  }

  //----------------------------------------

  void tock(bool rst_n) {
    if (!rst_n) {
      cycle = 0;
      cursor = 0;
      buffer = 0;
      sum = 0;
    }
    else {
      if (cycle != 0) {
        cycle = cycle - 1;
      }
      else if (cursor != 0) {
        uint8_t temp;

        temp = (i_serial << 7) | (buffer >> 1);
        if (cursor - 1 == 1) sum = sum + temp;

        cycle  = cycle_max;
        cursor = cursor - 1;
        buffer = temp;
      }
      else if (i_serial == 0) {
        cycle = cycle_max;
        cursor = cursor_max;
      }
    }
  }

  //----------------------------------------
















































  void dump_header() {
    printf("[rx cyc cursor o_data o_valid o_sum] ");
  }

  void dump() {
    printf("[   %-3d %-3d %-6x %-7d %05x] ", cycle, cursor, o_data, o_valid, o_sum);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module rx $end\n");
    fprintf(d.file, "$var wire %d  rx_cycle   cycle   $end\n",  cycle_bits);
    fprintf(d.file, "$var wire %d  rx_cursor  cursor  $end\n", cursor_bits);
    fprintf(d.file, "$var wire 8   rx_data    data    $end\n");
    fprintf(d.file, "$var wire 32  rx_sum     sum     $end\n");
    fprintf(d.file, "$var wire 8   rx_o_data  o_data  $end\n");
    fprintf(d.file, "$var wire 1   rx_o_valid o_valid $end\n");
    fprintf(d.file, "$var wire 32  rx_o_sum   o_sum   $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("rx_cycle",   cycle,   cycle_bits);
    d.set_value("rx_cursor",  cursor,  cursor_bits);
    d.set_value("rx_data",    buffer,    8);
    d.set_value("rx_sum",     sum,     32);
    d.set_value("rx_o_data",  o_data,  8);
    d.set_value("rx_o_valid", o_valid, 1);
    d.set_value("rx_o_sum",   o_sum,   32);
  }
};

//==============================================================================
