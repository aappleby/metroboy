// RISC-V SiMPLE SV -- combined text/data memory bus
// BSD 3-Clause License
// (c) 2017-2019, Arthur Matos, Marcus Vinicius Lamar, Universidade de Brasília,
//                Marek Materzok, University of Wrocław

#include "../../../src/uart_test/metron_tools.h"
#include "../../synth/config.h"
#include "constants.h"
#include "example_data_memory.h"
#include "example_text_memory.h"

struct example_memory_bus {
  logic<32> i_address;
  logic<32> o_read_data;
  logic<32> i_write_data;
  logic<4>  i_byte_enable;
  logic<1>  i_read_enable;
  logic<1>  i_write_enable;

  example_data_memory data_memory;
  example_text_memory text_memory;

  void tick() {
    data_memory.tick(
      bx<DATA_BITS-2>(i_address),
      i_write_enable && i_address >= DATA_BEGIN && i_address <= DATA_END,
      i_byte_enable,
      i_write_data);
  }

  void tock() {
    data_memory.tock(bx<DATA_BITS-2>(i_address));
    text_memory.tock(bx<TEXT_BITS-2>(i_address));

    o_read_data = b32(DONTCARE);
    if (i_read_enable) {
      if (i_address >= TEXT_BEGIN && i_address <= TEXT_END) o_read_data = text_memory.o_q;
      if (i_address >= DATA_BEGIN && i_address <= DATA_END) o_read_data = data_memory.o_q;
    }
  }
};
