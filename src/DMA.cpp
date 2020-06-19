#include "DMA.h"
#include "Constants.h"
#include <assert.h>

#include "Schematics.h"
using namespace Schematics;

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

#if 0

// LAVY00 << CUPA04
// LAVY01 << XEDA02
// LAVY02 nc
// LAVY03 >> LORU00

// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// LAVY = and2(FF46, CUPA)
// NAVO = nand6(dma addr)
// NOLO = not(NAVO)
// LOKO = nand2(CUNU, !LENE) // check neg output?
// LUPA = nor2(LAVY, LYXE)
// LUVY = reg
// MATU = reg
// LENE = reg
// MYTE = reg
// LYXE = latch(LOKO, LAVY)
// LARA = nand3(LOKY, CUNU, !MYTE)
// LOKY = nand2(LARA, !LENE)
// META = and2(UVYT, LOKY)
// LORU = not(LAVY)

// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// LUPA00 << LAVY
// LUPA01 << LYXE
// LUPA02 >> LUVY

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
    /*p04.LUPA*/ bool LUPA = or(DMA_WR, DMA_WR_LATCH);
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

#if 0

  // polarity of xymu seems wrong here
  /*p28.AJUJ*/ wire OAM_BUSYn = and(!DMA_RUNNINGp, !ACYL_OAM_ADDR_PARSE, vid_reg.RENDERING_LATCH);

  /*p04.POWU*/ wire DMA_WRITE_OAM = and(dma_reg.DMA_RUNNINGp, !dma_reg.FROM_CPU5_SYNC, !DMA_RUN_TRIG_d0);
  /*p04.WYJA*/ wire WYJA_OAM_WR = or(and(ADDR_OAM, OAM_BUSYn, CUPA_CPU_WR_xxxxxFGH), DMA_WRITE_OAM);

  /*p28.ZONE*/ oam_pins.WR_A.set(nand(WYJA_OAM_WR, oam_pins.A0));
  /*p28.ZOFE*/ oam_pins.WR_B.set(nand(WYJA_OAM_WR, not(oam_pins.A0)));
#endif


#if 0
  /*p04.MUDA*/ wire DMA_VRAM  = and(!dma_reg.DMA_A13, !dma_reg.DMA_A14, dma_reg.DMA_A15);
  /*p04.LUMA*/ wire LUMA_DMA_READ_CART = and(dma_reg.DMA_RUNNINGp, !DMA_VRAM);
#endif
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