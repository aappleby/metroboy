#include "MetroBoyLib/MetroBoyDMA.h"
#include "CoreLib/Constants.h"
#include <assert.h>
#include <stdio.h>

#if 0

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

#endif

//-----------------------------------------------------------------------------
// FIXME the phases are totally off here

void MetroBoyDMA::tick(int phase_total, const Req& req, Ack& ack) {
  (void)phase_total;

  if (req.read && req.addr == 0xFF46) {
    ack.addr = req.addr;
    ack.data_lo = uint8_t(addr >> 8);
    ack.read++;
  }
}

//-----------------------------------------------------------------------------

void MetroBoyDMA::tock(int phase_total, const Req& req) {
  bool DMA_WR = (req.addr == 0xFF46) && req.write && (MB_DELTA_FG || MB_DELTA_GH || MB_DELTA_HA);
  bool DMA_RST = DMA_RUN_TRIG_d4;

  if (MB_DELTA_BC) {
    // something wrong here, inverting this until we figure it out.
    // bool LUPA = nor4(DMA_WR, DMA_WR_LATCH);
    bool LUPA = DMA_WR || DMA_WR_LATCH;
    DMA_RUN_TRIG_d0 = LUPA;
    DMA_RUN_WRITE = DMA_RUN_READ;
    if (DMA_RUN_READ) addr++;
  }

  if (MB_DELTA_FG) {
    /*_p04.LENE*/ DMA_RUN_TRIG_d4 = DMA_RUN_TRIG_d0;
    /*_p04.MYTE*/ DMA_DONE = (addr & 0xFF) == 159;
    if (req.write && req.addr == 0xFF46) {
      addr = (req.data_lo << 8);
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

//-----------------------------------------------------------------------------

void MetroBoyDMA::dump(Dumper& d) const {
  d("\002--------------DMA2--------------\001\n");

  d("DMA_WR_LATCH    %d\n", (bool)DMA_WR_LATCH);
  d("DMA_RUN_TRIG_d0 %d\n", (bool)DMA_RUN_TRIG_d0);
  d("DMA_RUN_TRIG_d4 %d\n", (bool)DMA_RUN_TRIG_d4);
  d("DMA_RUN_READ    %d\n", (bool)DMA_RUN_READ);
  d("DMA_RUN_WRITE   %d\n", (bool)DMA_RUN_WRITE);
  d("DMA_DONE        %d\n", (bool)DMA_DONE);
  d("addr            0x%04x\n", addr);
}

//-----------------------------------------------------------------------------
