#pragma once
#include "metron.h"

//==============================================================================

template<int clocks_per_bit = 4>
struct uart_rx : public Module {

  //----------------------------------------

  bool     i_serial;

  uint8_t  o_data;
  bool     o_valid;
  uint32_t o_sum;

  //----------------------------------------

  static constexpr int timer_bits = clog2(clocks_per_bit);
  int cycle;
  int bit;
  uint8_t data;
  uint32_t sum;

  //----------------------------------------

  void reset() {
    cycle = 0;
    bit = 0;
    data = 0;
    sum = 0;
  }

  //----------------------------------------

  void tick() {
    o_data  = data;
    o_valid = bit == 1;
    o_sum   = sum;
  }

  //----------------------------------------

  void tock() {
    if (cycle != 0) {
      cycle = cycle - 1;
    }
    else if (bit != 0) {
      cycle = (clocks_per_bit - 1);
      bit = bit - 1;
      data = (i_serial << 7) | (data >> 1);
      if (bit == 1) sum += data;
    }
    else if (i_serial == 0) {
      cycle = (clocks_per_bit - 1);
      bit = 9;
    }
  }

  //----------------------------------------

  void dump_header() {
    printf("[rx cyc bit o_data o_valid o_sum] ");
  }

  void dump() {
    printf("[   %-3d %-3d %-6x %-7d %05x] ", cycle, bit, o_data, o_valid, o_sum);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module rx $end\n");
    fprintf(d.file, "$var wire 2  rx_cycle   cycle $end\n");
    fprintf(d.file, "$var wire 4  rx_bit     bit $end\n");
    fprintf(d.file, "$var wire 8  rx_o_data  o_data $end\n");
    fprintf(d.file, "$var wire 1  rx_o_valid o_valid $end\n");
    fprintf(d.file, "$var wire 32 rx_o_sum   o_sum $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_width(VcdDump& d) {
    d.set_width("rx_cycle", clog2(clocks_per_bit));
    d.set_width("rx_bit",   clog2(9));
    //d.set_width("rx_data",  8);
    //d.set_width("rx_sum",   32);

    d.set_width("rx_o_data",  8);
    d.set_width("rx_o_valid", 1);
    d.set_width("rx_o_sum",   32);
  }

  void dump_value(VcdDump& d) {
    d.set_value("rx_cycle", cycle);
    d.set_value("rx_bit",   bit);
    //d.set_value("rx_data",  data);
    //d.set_value("rx_sum",   sum);

    d.set_value("rx_o_data",  o_data);
    d.set_value("rx_o_valid", o_valid);
    d.set_value("rx_o_sum",   o_sum);
  }
};

//==============================================================================
