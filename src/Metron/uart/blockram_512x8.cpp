#include "metron.h"

//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

struct blockram_512x8
{
  static const int size = 512;

  uint8_t memory[size];
  uint8_t data;

  void initial(uint8_t* src, int size) {
    assert(size == 512);
    memcpy(memory, src, size);
  }

  void reset() {
    data = 0x00;
  }

  void tick(uint8_t& o_data) {
    o_data = data;
  }

  void tock(int addr) {
    data = memory[addr];
  }
};
