#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <inttypes.h>

#pragma warning(disable:4996)

#include "uart_top.h"
#include "uart_hello.h"
#include "uart_tx.h"
#include "uart_rx.h"

class VcdDump {
public:

  typedef std::unordered_map<std::string, std::string> tag_to_string;
  typedef std::unordered_map<std::string, uint64_t> tag_to_value;

  VcdDump(const char* filename) {
    file = fopen(filename, "wb");
    time = 0;
  }

  ~VcdDump() {
    fclose(file);
    file = nullptr;
  }

  void set_time(uint64_t _time) {
    time = _time;
    fprintf(file, "#%" PRIu64 "\n", time);
  };

  void advance(uint64_t delta) {
    time += delta;
    fprintf(file, "#%" PRIu64 "\n", time);
  };

  void set_value(const char* key, uint64_t value, int width) {
    auto it = field_state.find(key);
    if ((it == field_state.end()) || (value != (*it).second)) {
      if (width == 1) {
        assert((value & ~1) == 0);
        fprintf(file, "%" PRIu64 "%s\n", value & 1, key);
      }
      else {
        fprintf(file, "b");
        for (int i = (int)width - 1; i >= 0; i--) {
          fprintf(file, "%d", int(value >> i) & 1);
        }
        fprintf(file, " %s\n", key);
      }
    }
    field_state[key] = value;
  }

  FILE* file = nullptr;
  uint64_t time = 0;
  tag_to_string field_path;
  tag_to_value field_state;
};

//----------------------------------------

inline void dump_tx_header() {
  printf("[tx cyc bit buf serial cts idle] ");
}

template<int cycles_per_bit>
inline void dump_tx(uart_tx<cycles_per_bit>& tx) {
  printf("[   %-3d %-3d %03x %-6d %-3d %-4d] ", tx.cycle, tx.cursor, tx.buffer, tx.o_serial, tx.o_cts, tx.o_idle);
}

template<int clocks_per_bit>
inline void dump_tx_vcd_header(VcdDump& d) {
  fprintf(d.file, "$scope module tx $end\n");
  fprintf(d.file, "$var wire 5  tx_cycle    cycle    $end\n", uart_tx<clocks_per_bit>::cycle_bits);
  fprintf(d.file, "$var wire 5  tx_cursor   cursor   $end\n", uart_tx<clocks_per_bit>::cursor_bits);
  fprintf(d.file, "$var wire 9  tx_buf      buf      $end\n");
  fprintf(d.file, "$var wire 1  tx_o_cts    o_cts    $end\n");
  fprintf(d.file, "$var wire 1  tx_o_idle   o_idle   $end\n");
  fprintf(d.file, "$var wire 1  tx_o_serial o_serial $end\n");
  fprintf(d.file, "$upscope $end\n");
}

template<int cycles_per_bit>
inline void dump_tx_value(VcdDump& d, uart_tx<cycles_per_bit>& tx) {
  d.set_value("tx_cycle", tx.cycle, 5);
  d.set_value("tx_cursor", tx.cursor, 5);
  d.set_value("tx_buf", tx.buffer, 9);

  d.set_value("tx_o_serial", tx.o_serial, 1);
  d.set_value("tx_o_cts", tx.o_cts, 1);
  d.set_value("tx_o_idle", tx.o_idle, 1);
}

//----------------------------------------

inline void dump_rx_header() {
  printf("[rx cyc cursor o_data o_valid o_sum] ");
}

template<int clocks_per_bit>
inline void dump_rx(uart_rx<clocks_per_bit>& rx) {
  printf("[   %-3d %-3d %-6x %-7d %05x] ", rx.cycle, rx.cursor, rx.o_data, rx.o_valid, rx.o_sum);
}

template<int clocks_per_bit>
inline void dump_rx_vcd_header(VcdDump& d) {
  fprintf(d.file, "$scope module rx $end\n");
  fprintf(d.file, "$var wire %d  rx_cycle   cycle   $end\n", uart_rx<clocks_per_bit>::cycle_bits);
  fprintf(d.file, "$var wire %d  rx_cursor  cursor  $end\n", uart_rx<clocks_per_bit>::cursor_bits);
  fprintf(d.file, "$var wire 8   rx_data    data    $end\n");
  fprintf(d.file, "$var wire 32  rx_sum     sum     $end\n");
  fprintf(d.file, "$var wire 8   rx_o_data  o_data  $end\n");
  fprintf(d.file, "$var wire 1   rx_o_valid o_valid $end\n");
  fprintf(d.file, "$var wire 32  rx_o_sum   o_sum   $end\n");
  fprintf(d.file, "$upscope $end\n");
}

template<int clocks_per_bit>
inline void dump_rx_value(VcdDump& d, uart_rx<clocks_per_bit>& rx) {
  d.set_value("rx_cycle", rx.cycle, uart_rx<clocks_per_bit>::cycle_bits);
  d.set_value("rx_cursor", rx.cursor, uart_rx<clocks_per_bit>::cursor_bits);
  d.set_value("rx_data", rx.buffer, 8);
  d.set_value("rx_sum", rx.sum, 32);
  d.set_value("rx_o_data", rx.o_data, 8);
  d.set_value("rx_o_valid", rx.o_valid, 1);
  d.set_value("rx_o_sum", rx.o_sum, 32);
}

//----------------------------------------

inline void dump_hello_header() {
  printf("[hlo state cursor data req done] ");
}

template<int clocks_per_bit>
inline void dump_hello(uart_hello& hello) {
  printf("[    %-5d %-6d %04x %-3d %-4d] ", hello.state, hello.cursor, hello.o_data, hello.o_req, hello.o_done);
}

inline void dump_hello_vcd_header(VcdDump& d) {
  fprintf(d.file, "$scope module hello $end\n");
  fprintf(d.file, "$var wire 2 hello_state  state  $end\n");
  fprintf(d.file, "$var wire 9 hello_cursor cursor $end\n");
  fprintf(d.file, "$var wire 8 hello_o_data o_data $end\n");
  fprintf(d.file, "$var wire 1 hello_o_req  o_req  $end\n");
  fprintf(d.file, "$var wire 1 hello_o_done o_done $end\n");
  fprintf(d.file, "$upscope $end\n");
}

inline void dump_hello_value(VcdDump& d, uart_hello& hello) {
  d.set_value("hello_state", hello.state, 2);
  d.set_value("hello_cursor", hello.cursor, 9);
  d.set_value("hello_o_data", hello.o_data, 8);
  d.set_value("hello_o_req", hello.o_req, 1);
  d.set_value("hello_o_done", hello.o_done, 1);
}

//----------------------------------------

inline void dump_top_header() {
  printf("[top data valid done sum ] ");
  dump_hello_header();
  dump_tx_header();
  dump_rx_header();
}

template<int cycles_per_bit>
inline void dump_top(uart_top<cycles_per_bit>& top) {
  printf("[    %04x %-5d %-4d %04x] ", top.o_data, top.o_valid, top.o_done, top.o_sum);
  dump_hello(top.hello);
  dump_tx(top.tx);
  dump_rx(top.rx);
}

inline void dump_top_vcd_header(VcdDump& d) {
  fprintf(d.file, "$var wire 8  top_o_data  o_data $end\n");
  fprintf(d.file, "$var wire 1  top_o_valid o_valid $end\n");
  fprintf(d.file, "$var wire 1  top_o_done  o_done $end\n");
  fprintf(d.file, "$var wire 32 top_o_sum   o_sum $end\n");
}

template<int cycles_per_bit>
inline void dump_top_value(VcdDump& d, uart_top<cycles_per_bit>& top) {
  d.set_value("top_o_data", top.o_data, 8);
  d.set_value("top_o_valid", top.o_valid, 1);
  d.set_value("top_o_done", top.o_done, 1);
  d.set_value("top_o_sum", top.o_sum, 32);
  dump_hello_value(d, top.hello);
  dump_tx_value(d, top.tx);
  dump_rx_value(d, top.rx);
}

//----------------------------------------
