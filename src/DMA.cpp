#include "DMA.h"
#include "Constants.h"
#include <assert.h>

#if 0
//-----------------------------------------------------------------------------

void DMA1::get_ebus_req(Req& r) const {
  if ((mode_a != Mode::NONE) && ((source_a >> 5) != 4)) {
    r.addr  = uint16_t((source_a << 8) | count_a);
    r.data  = 0;
    r.read  = 1;
    r.write = 0;
  }
}

//----------------------------------------

void DMA1::get_vbus_req(Req& r) const {
  if ((mode_a != Mode::NONE) && ((source_a >> 5) == 4)) {
    r.addr  = uint16_t((source_a << 8) | count_a);
    r.data  = 0;
    r.read  = 1;
    r.write = 0;
  }
}

//----------------------------------------

void DMA1::get_obus_req(Req& r) const {
  if (mode_b != Mode::NONE) {
    r.addr  = uint16_t(ADDR_OAM_BEGIN + count_b);
    r.data  = data_b;
    r.read  = 0;
    r.write = 1;
  }
}

//-----------------------------------------------------------------------------
#endif

void DMA2::tick(const Req& req, Ack& ack) {
  if (req.read && req.addr == 0xFF46) {
    ack.addr = req.addr;
    ack.data = uint8_t(addr >> 8);
    ack.read++;
  }
}

void DMA2::tock(int phase, const Req& req) {


  /*p04.LAVY*/ bool DMA_WR = (req.addr == 0xFF46) && req.write && (PHASE_F || PHASE_G || PHASE_H);
  /*p04.LOKO*/ bool DMA_RST = DMA_RUN_TRIG_d4;

  if (PHASE_B) {
    // something wrong here, inverting this until we figure it out.
    ///*p04.LUPA*/ bool LUPA = nor(DMA_WR, DMA_WR_LATCH);
    /*p04.LUPA*/ bool LUPA = DMA_WR || DMA_WR_LATCH;
    /*p04.LUVY*/ DMA_RUN_TRIG_d0 = LUPA;
    /*p04.MATU*/ DMA_RUN_WRITE = DMA_RUN_READ;
    if (DMA_RUN_READ) addr++;
  }

  if (PHASE_F) {
    /*p04.LENE*/ DMA_RUN_TRIG_d4 = DMA_RUN_TRIG_d0;
    /*p04.MYTE*/ DMA_DONE = (addr & 0xFF) == 159;
    if (req.write && req.addr == 0xFF46) {
      addr = (req.data << 8);
    }
  }

  if (DMA_WR)  DMA_WR_LATCH = 1;
  if (DMA_RST) DMA_WR_LATCH = 0;

  if (DMA_RUN_TRIG_d4) DMA_RUN_READ = 1;
  if (DMA_DONE)        DMA_RUN_READ = 0;

  if (DMA_RST) {
    DMA_DONE = 0;
    addr = addr & 0xFF00;
  }
}

void DMA2::dump(std::string& d) {
  sprintf(d, "\002--------------DMA2--------------\001\n");

  sprintf(d, "DMA_WR_LATCH    %d\n", (bool)DMA_WR_LATCH);
  sprintf(d, "DMA_RUN_TRIG_d0 %d\n", (bool)DMA_RUN_TRIG_d0);
  sprintf(d, "DMA_RUN_TRIG_d4 %d\n", (bool)DMA_RUN_TRIG_d4);
  sprintf(d, "DMA_RUN_READ    %d\n", (bool)DMA_RUN_READ);
  sprintf(d, "DMA_RUN_WRITE   %d\n", (bool)DMA_RUN_WRITE);
  sprintf(d, "DMA_DONE        %d\n", (bool)DMA_DONE);
  sprintf(d, "addr            0x%04x\n", addr);
}