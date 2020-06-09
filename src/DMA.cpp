#include "DMA.h"
#include "Constants.h"
#include <assert.h>

#include "Schematics.h"
using namespace Schematics;

//-----------------------------------------------------------------------------

void DMA1::get_ebus_req(Req& r) const {
  r.addr  = uint16_t((source_a << 8) | count_a);
  r.data  = 0;
  r.read  = (mode_a != Mode::NONE) && ((source_a >> 5) != 4);
  r.write = 0;
}

//----------------------------------------

void DMA1::get_vbus_req(Req& r) const {
  r.addr  = uint16_t((source_a << 8) | count_a);
  r.data  = 0;
  r.read  = (mode_a != Mode::NONE) && ((source_a >> 5) == 4);
  r.write = 0;
}

//----------------------------------------

void DMA1::get_obus_req(Req& r) const {
  r.addr  = uint16_t(ADDR_OAM_BEGIN + count_b);
  r.data  = data_b;
  r.read  = 0;
  r.write = (mode_b != Mode::NONE);
}

//----------------------------------------

void DMA1::on_ebus_ack(const Ack& ebus_ack) {
  uint16_t src_addr = uint16_t((source_a << 8) | count_a);
  if (ebus_ack.read == 1 && ebus_ack.addr == src_addr) {
    data_b = uint8_t(ebus_ack.data);
  }
}

//----------------------------------------

void DMA1::on_vbus_ack(const Ack& vbus_ack) {
  uint16_t src_addr = uint16_t((source_a << 8) | count_a);
  if (vbus_ack.read == 1 && vbus_ack.addr == src_addr) {
    data_b = uint8_t(vbus_ack.data);
  }
}

//----------------------------------------

void DMA1::on_obus_ack(const Ack& /*obus_ack*/) {
  // nothing to do here?
}

//-----------------------------------------------------------------------------

void DMA1::tick(int phase, const Req& req, Ack& ack) const {
  if (req.read && req.addr == ADDR_DMA) {
    ack.addr  += req.addr;
    ack.data  += source_x;
    ack.read  += req.read;
    ack.write += req.write;
  }
}

//-----------------------------------------------------------------------------

void DMA1::tock(int phase, const Req& ibus_req) {

  if (PHASE_B) {
    mode_b = mode_a;
    count_b = count_a;

    mode_a = mode_x;
    count_a = count_x;
    source_a = source_x;

    if (mode_x != Mode::NONE) count_x++;
    if (count_x == 160) mode_x = Mode::NONE;
  }

  if (PHASE_F && ibus_req.write && ibus_req.addr == ADDR_DMA) {
    //printf("%08d DMA write 0x%02x\n", tcycle, ibus_req.data);

    if (ibus_req.data <= 0x7F) mode_x = Mode::CART;
    if (0x80 <= ibus_req.data && ibus_req.data <= 0x9F) mode_x = Mode::VRAM;
    if (0xA0 <= ibus_req.data && ibus_req.data <= 0xBF) mode_x = Mode::CART;
    if (0xC0 <= ibus_req.data && ibus_req.data <= 0xFD) mode_x = Mode::IRAM;
    count_x = 0;
    source_x = ibus_req.data;
  }
}

//-----------------------------------------------------------------------------

void DMA1::dump(std::string& d) {
  sprintf(d, "\002--------------DMA--------------\001\n");
 
  sprintf(d, "mode_x      %d\n", mode_x);
  sprintf(d, "count_x     %d\n", count_x);
  sprintf(d, "source_x    0x%04x\n", source_x);
  sprintf(d, "\n");
  sprintf(d, "mode_a      %d\n", mode_a);
  sprintf(d, "count_a     %d\n", count_a);
  sprintf(d, "source_a    0x%04x\n", source_a);
  sprintf(d, "\n");
  sprintf(d, "mode_b      %d\n", mode_b);
  sprintf(d, "count_b     %d\n", count_b);
  sprintf(d, "data_b      %d\n", data_b);
}

//-----------------------------------------------------------------------------


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

/*p28.AMAB*/ wire AMAB = and(ADDR_OAM, !DMA_RUN, !ACYL_OAM_ADDR_PARSE, OAM_ADDR_RENDER);
/*p04.POWU*/ wire POWU = and(DMA_RUN, !FROM_CPU5_SYNC, !LUVY);
/*p04.WYJA*/ wire WYJA    = or(and(AMAB, CUPA_BUS_WR_xxxxxFGH), POWU);
/*p28.ZONE*/ ZONE.set(nand(WYJA,  oam_pins.A0));
/*p28.ZOFE*/ ZOFE.set(nand(WYJA, !oam_pins.A0));
/*p01.UVYT*/ UVYT = or(ABOL_CLKREQn, clk_reg.PHAZ_xBCDExxx);
/*p04.MOPA*/ MOPA = !UVYT;
/*p01.APOV*/ APOV = and(cpu_pins.CPU_RAW_WR, clk_reg.PHAZ_xxxxEFGH, !clk_reg.PHAZ_xBCDExxx);
/*p07.CUPA*/ CUPA = mux2(ext_pins.WR_C, APOV, UNOR_MODE_DBG2);
/*p22.XEDA*/ FF46 = cpu_pins.get_addr() == 0xFF46;

//----------

/*p04.LAVY*/ LAVY = and(FF46, CUPA);
/*p04.NAVO*/ NAVO = nand(DMA_A00, DMA_A01, DMA_A02, DMA_A03, DMA_A04, DMA_A07);
/*p04.NOLO*/ NOLO = not(NAVO);
/*p04.LOKO*/ LOKO = nand(CUNU, !LENE);
/*p04.LUPA*/ LUPA = nor(LAVY, LYXE);
/*p04.LUVY*/ LUVY.set(UVYT, CUNU, LUPA);
/*p04.MATU*/ MATU.set(UVYT, CUNU, LOKY);
/*p04.LENE*/ LENE.set(MOPA, CUNU, LUVY);
/*p04.MYTE*/ MYTE.set(MOPA, !LOKO,  NOLO);
/*p04.LYXE*/ LYXE.sr_latch(LOKO, LAVY);
/*p04.LARA*/ LARA = nand(LOKY, CUNU, !MYTE);
/*p04.LOKY*/ LOKY = nand(LARA, !LENE);
/*p04.META*/ META = and(UVYT, LOKY);
/*p04.LORU*/ LORU = not(LAVY);

if (PHASE_B) {
  /*p04.NAKY*/ DMA_A00.set(META, !LOKO, !DMA_A00);
  /*p04.PYRO*/ DMA_A01.set(!DMA_A00, !LOKO, !DMA_A01);
  /*p04.NEFY*/ DMA_A02.set(!DMA_A01, !LOKO, !DMA_A02);
  /*p04.MUTY*/ DMA_A03.set(!DMA_A02, !LOKO, !DMA_A03);
  /*p04.NYKO*/ DMA_A04.set(!DMA_A03, !LOKO, !DMA_A04);
  /*p04.PYLO*/ DMA_A05.set(!DMA_A04, !LOKO, !DMA_A05);
  /*p04.NUTO*/ DMA_A06.set(!DMA_A05, !LOKO, !DMA_A06);
  /*p04.MUGU*/ DMA_A07.set(!DMA_A06, !LOKO, !DMA_A07);
}

if (PHASE_F) {
  // these look like dffs? 

  /*p04.NAFA*/ DMA_A08.set(LORU, cpu_pins.D0);
  /*p04.PYNE*/ DMA_A09.set(LORU, cpu_pins.D1);
  /*p04.PARA*/ DMA_A10.set(LORU, cpu_pins.D2);
  /*p04.NYDO*/ DMA_A11.set(LORU, cpu_pins.D3);
  /*p04.NYGY*/ DMA_A12.set(LORU, cpu_pins.D4);
  /*p04.PULA*/ DMA_A13.set(LORU, cpu_pins.D5);
  /*p04.POKU*/ DMA_A14.set(LORU, cpu_pins.D6);
  /*p04.MARU*/ DMA_A15.set(LORU, cpu_pins.D7);
}

#endif

// LYXE00 << LAVY03
// LYXE01 nc
// LYXE02 >> LUPA01
// LYXE03 >> nc
// LYXE04 nc
// LYXE05 << LOKO02

// LUPA00 << LAVY
// LUPA01 << LYXE
// LUPA02 >> LUVY

DMA2::DMA2() {
}

void DMA2::tock(int phase, const Req& req) {


  /*p04.LAVY*/ bool DMA_WR = (req.addr == 0xFF46) && req.write && (PHASE_F || PHASE_G || PHASE_H);
  /*p04.LOKO*/ bool DMA_RST = DMA_RUN_TRIG_d4;

  if (PHASE_B) {
    // something wrong here, inverting this until we figure it out.
    ///*p04.LUPA*/ bool LUPA = nor(DMA_WR, DMA_WR_LATCH);
    /*p04.LUPA*/ bool LUPA = or(DMA_WR, DMA_WR_LATCH);
    /*p04.LUVY*/ DMA_RUN_TRIG_d0 = LUPA;
    /*p04.MATU*/ DMA_RUN = DMA_RUN_LATCH;
    if (DMA_RUN_LATCH) addr++;
  }

  if (PHASE_F) {
    /*p04.LENE*/ DMA_RUN_TRIG_d4 = DMA_RUN_TRIG_d0;
    /*p04.MYTE*/ DMA_DONE = (addr & 0xFF) == 159;
    if (req.write && req.addr == 0xFF46) addr = (addr & 0xFF00) | (req.data << 8);
  }

  if (DMA_WR)  DMA_WR_LATCH = 1;
  if (DMA_RST) DMA_WR_LATCH = 0;

  if (DMA_RUN_TRIG_d4) DMA_RUN_LATCH = 1;
  if (DMA_DONE)        DMA_RUN_LATCH = 0;

  if (DMA_RST) {
    DMA_DONE = 0;
    addr = addr & 0xFF00;
  }

#if 0

  // polarity of xymu seems wrong here
  /*p28.AJUJ*/ wire OAM_BUSYn = and(!DMA_RUNNING, !ACYL_OAM_ADDR_PARSE, vid_reg.XYMU_RENDERING_LATCH);

  /*p04.POWU*/ wire DMA_WRITE_OAM = and(dma_reg.DMA_RUNNING, !dma_reg.FROM_CPU5_SYNC, !DMA_RUN_TRIG_d0);
  /*p04.WYJA*/ wire WYJA_OAM_WR = or(and(ADDR_OAM, OAM_BUSYn, CUPA_BUS_WR_xxxxxFGH), DMA_WRITE_OAM);

  /*p28.ZONE*/ oam_pins.WR_A.set(nand(WYJA_OAM_WR, oam_pins.A0));
  /*p28.ZOFE*/ oam_pins.WR_B.set(nand(WYJA_OAM_WR, not(oam_pins.A0)));
#endif


#if 0
  /*p04.MUDA*/ wire DMA_VRAM  = and(!dma_reg.DMA_A13, !dma_reg.DMA_A14, dma_reg.DMA_A15);
  /*p04.LUMA*/ wire LUMA_DMA_READ_CART = and(dma_reg.DMA_RUNNING, !DMA_VRAM);
#endif
}

void DMA2::dump(std::string& d) {
  sprintf(d, "\002--------------DMA2--------------\001\n");

  sprintf(d, "DMA_WR_LATCH    %d\n", (bool)DMA_WR_LATCH);
  sprintf(d, "DMA_RUN_TRIG_d0 %d\n", (bool)DMA_RUN_TRIG_d0);
  sprintf(d, "DMA_RUN_TRIG_d4 %d\n", (bool)DMA_RUN_TRIG_d4);
  sprintf(d, "DMA_RUN_LATCH   %d\n", (bool)DMA_RUN_LATCH);
  sprintf(d, "DMA_RUN         %d\n", (bool)DMA_RUN);
  sprintf(d, "DMA_DONE        %d\n", (bool)DMA_DONE);
  sprintf(d, "addr            0x%04x\n", addr);
}