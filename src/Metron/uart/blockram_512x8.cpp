#include "metron.h"

//=============================================================================
// tiny one-read-port + one-write-port memory to simulate block ram

struct blockram_512x8
/*
#(parameter INIT_FILE = "")
(
  input logic clk,

  input logic[8:0]  raddr,
  output logic[7:0] rdata,

  input logic       write_en,
  input logic[8:0]  waddr,
  input logic[7:0]  wdata
);
*/
{
  static const int size = 512;

  uint8_t memory[size];
  uint8_t rdata;

  void initial(uint8_t* src, int size) {
    assert(size == 512);
    memcpy(memory, src, size);
    rdata = 0xFF;
  }

  void tick(uint8_t& rdata) {
    rdata = this->rdata;
  }

  void tock(int raddr, bool write_en, int waddr, uint8_t wdata) {
    rdata = memory[raddr];
    if (write_en) memory[waddr] = wdata;
  }
};
