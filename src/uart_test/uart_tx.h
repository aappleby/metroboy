#include "metron.h"

//==============================================================================

template<int cycles_per_bit = 4>
struct uart_tx : public Module {

  //----------------------------------------

  uint8_t i_data;
  bool    i_req;

  bool    o_serial;
  bool    o_cts;
  bool    o_idle;

  //----------------------------------------

  static constexpr int timer_bits = clog2(cycles_per_bit);

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  static constexpr int extra_stop_bits = 7;

  int tx_cycle;
  int tx_bit;
  uint16_t tx_buf;

  //----------------------------------------

  void initial() {
  }

  //----------------------------------------

  void reset() {
    tx_cycle = 0;
    tx_bit   = 0;
    tx_buf   = 0x1FF;
  }

  //----------------------------------------

  void tick() {
    o_serial = tx_buf & 1;
    o_cts    = ((tx_bit <= extra_stop_bits) && (tx_cycle == 0)) || (tx_bit < extra_stop_bits);
    o_idle   = (tx_bit == 0) && (tx_cycle == 0);
  }

  //----------------------------------------

  void tock() {
    if (tx_bit <= extra_stop_bits && tx_cycle == 0 && i_req) {
      // Transmit start
      tx_cycle = cycles_per_bit - 1;
      tx_bit   = 10 + extra_stop_bits - 1;
      tx_buf   = (i_data << 1) | 0;
    } else if (tx_cycle != 0) {
      // Bit delay
      tx_cycle = tx_cycle - 1;
      tx_bit   = tx_bit;
      tx_buf   = tx_buf;
    } else if (tx_bit != 0) {
      // Bit delay done, switch to next bit.
      tx_cycle = cycles_per_bit - 1;
      tx_bit   = tx_bit - 1;
      tx_buf   = (tx_buf >> 1) | 0x100;
    }
  }






















  void dump_header() {
    printf("[tx cyc bit buf serial cts idle] ");
  }

  void dump() {
    printf("[   %-3d %-3d %03x %-6d %-3d %-4d] ", tx_cycle, tx_bit, tx_buf, o_serial, o_cts, o_idle);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module tx $end\n");
    fprintf(d.file, "$var wire 5  tx_cycle    cycle $end\n");
    fprintf(d.file, "$var wire 5  tx_bit      bit $end\n");
    fprintf(d.file, "$var wire 9  tx_buf      buf $end\n");
    fprintf(d.file, "$var wire 1  tx_o_cts    o_cts $end\n");
    fprintf(d.file, "$var wire 1  tx_o_idle   o_idle $end\n");
    fprintf(d.file, "$var wire 1  tx_o_serial o_serial $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("tx_cycle", tx_cycle, 5);
    d.set_value("tx_bit",   tx_bit, 5);
    d.set_value("tx_buf",   tx_buf, 9);

    d.set_value("tx_o_serial", o_serial, 1);
    d.set_value("tx_o_cts",    o_cts, 1);
    d.set_value("tx_o_idle",   o_idle, 1);
  }
};

//==============================================================================
