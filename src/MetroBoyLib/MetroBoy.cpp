#include "MetroBoyLib/MetroBoy.h"

#include "GameboyLib/Assembler.h"
#include "GameboyLib/Constants.h"
#include "CoreLib/Dumper.h"

#include <assert.h>
#include "imgui/imgui.h"

#pragma warning(disable:4189)
#pragma warning(disable:4244)

//-----------------------------------------------------------------------------

#define MB_DELTA_AB   ((phase_total & 7) == 0)
#define MB_DELTA_BC   ((phase_total & 7) == 1)
#define MB_DELTA_CD   ((phase_total & 7) == 2)
#define MB_DELTA_DE   ((phase_total & 7) == 3)
#define MB_DELTA_EF   ((phase_total & 7) == 4)
#define MB_DELTA_FG   ((phase_total & 7) == 5)
#define MB_DELTA_GH   ((phase_total & 7) == 6)
#define MB_DELTA_HA   ((phase_total & 7) == 7)

//-----------------------------------------------------------------------------

void MetroBoy::reset(uint8_t* new_rom, size_t new_rom_size) {
  check_sentinel();

  gb_cpu.reset();
  cart.set_rom(new_rom, new_rom_size);
  cart.reset();
  ppu.reset();
  oam.reset();
  //spu.reset();
  timer.reset();
  vram.reset();
  joy.reset();
  serial.reset();
  zram.reset();

  boot.disable_bootrom = true;

  phase_total = 0;

  ebus_req.addr = 0x0100;
  ebus_req.data = 0x00;
  ebus_req.read = 1;
  ebus_req.write = 0;

  ebus_ack.addr = 0x0100;
  ebus_ack.data = 0x00;
  ebus_ack.read = 1;

  memset(framebuffer, 4, 160 * 144);
}


void MetroBoy::reset_to_bootrom(uint8_t* new_rom, size_t new_rom_size) {
  check_sentinel();

  // FIXME needs power on reset stuff

  CHECK_P(false);

  /*
  gb_cpu.reset_to_bootrom();
  cart.set_rom(new_rom, new_rom_size);
  cart.reset();
  ppu.reset_to_bootrom();
  oam.reset();
  spu.reset();
  timer.reset();
  vram.reset();
  joy.reset();
  serial.reset();
  zram.reset();

  boot.disable_bootrom = false;

  phase_total = 0;

  ebus_req.addr = 0x0000;
  ebus_req.data = 0x00;
  ebus_req.read = 1;
  ebus_req.write = 0;

  ebus_ack.addr = 0x0000;
  ebus_ack.data = 0x00;
  ebus_ack.read = 1;
  */

  memset(framebuffer, 4, 160 * 144);
}

//-----------------------------------------------------------------------------

void MetroBoy::next_phase() {

  //-----------------------------------
  // gather acks

  if (ibus_req.read) {
    ibus_ack = { 0 };
    ppu.   tick(phase_total, ibus_req, ibus_ack);
    serial.tick(phase_total, ibus_req, ibus_ack);
    joy.tick(phase_total, ibus_req, ibus_ack);
    zram.  tick(phase_total, ibus_req, ibus_ack);
    //spu.   tick(phase_total, ibus_req, ibus_ack);
    boot.  tick(phase_total, ibus_req, ibus_ack);
    timer. tick(phase_total, ibus_req, ibus_ack);
    dma.   tick(phase_total, ibus_req, ibus_ack);
    ints.  tick(phase_total, ibus_req, ibus_ack);
  }

  ebus_ack = { 0 };
  vbus_ack = { 0 };
  obus_ack = { 0 };

  cart.tick(phase_total, ebus_req, ebus_ack);
  vram.tick(phase_total, vbus_req, vbus_ack);
  oam .tick(phase_total, obus_req, obus_ack);

  //----------------------------------------

  bool dma_src_vbus = dma.DMA_RUN_READ && (dma.addr >= ADDR_VRAM_BEGIN) && (dma.addr <= ADDR_VRAM_END);
  bool dma_src_ebus = dma.DMA_RUN_READ && !dma_src_vbus;

  if (dma_src_vbus) dma_data_latch = vbus_ack.data_lo;
  if (dma_src_ebus) dma_data_latch = ebus_ack.data_lo;

  DCHECK_N(ibus_ack.read > 1);
  DCHECK_N(ebus_ack.read > 1);
  DCHECK_N(vbus_ack.read > 1);
  DCHECK_N(obus_ack.read > 1);

  if (MB_DELTA_HA) {
    bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
    bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

    uint8_t bus_data = 0xFF;
    if      (cpu_has_ibus_req) bus_data = ibus_ack.data_lo;
    else if (cpu_has_ebus_req) bus_data = ebus_ack.data_lo;
    else if (cpu_has_vbus_req) bus_data = vbus_ack.data_lo;
    else if (cpu_has_obus_req) bus_data = obus_ack.data_lo;

    // FIXME
    debugbreak();
    //gb_cpu.tock_ab(ints.imask, ints.intf, bus_data);

    /*
    cpu.tock_ack(imask_delay, intf_delay, bus_data);
    cpu.tock_req(imask_delay, intf_delay);

    intf_delay = ints.intf;
    imask_delay = ints.imask;
    */
  }

  timer. tock(phase_total, ibus_req);
  serial.tock(phase_total, ibus_req);
  joy.tock(phase_total, ibus_req);
  boot.  tock(phase_total, ibus_req);
  zram.  tock(phase_total, ibus_req);
  //spu.   tock(phase_total, ibus_req);
  ppu.   tock(phase_total, ibus_req, vbus_ack, obus_ack);
  dma.   tock(phase_total, ibus_req);
  cart.  tock(phase_total, ebus_req);
  vram.  tock(phase_total, vbus_req);
  oam.   tock(phase_total, obus_req);
  ints.  tock(phase_total, ibus_req, gb_cpu.get_int_ack(), ppu.vblank_int, ppu.stat_int, timer.timer_int, /*serial_int*/ 0, joy.get() != 0xFF);

  //----------

  int pix_x = ppu.pix_count - 8;
  int pix_y = ppu.line;

  if (pix_x >= 0 && pix_x < 160 && pix_y >= 0 && pix_y < 144) {
    framebuffer[pix_x + pix_y * 160] = ppu.pix_out;
  }

  //-----------------------------------
  // prioritize reqs

  if (MB_DELTA_AB) {
    cpu_req = gb_cpu.get_bus_req();
    //cpu_req.addr  = gb_cpu.get_bus_addr();
    //cpu_req.data  = gb_cpu.get_bus_data();
    //cpu_req.read  = gb_cpu.get_bus_read();
    //cpu_req.write = gb_cpu.get_bus_write();

    ibus_req = {0};
    ebus_req = {0};
    vbus_req = {0};
    obus_req = {0};

    if (cpu_req.addr >= 0xFF00 && cpu_req.addr <= 0xFFFF) {
      ibus_req = cpu_req;
    }
    else if (cpu_req.addr >= 0xFE00 && cpu_req.addr <= 0xFEFF) {
      obus_req = cpu_req;
    }
    else if (cpu_req.addr >= 0x8000 && cpu_req.addr <= 0x9FFF) {
      vbus_req = cpu_req;
    }
    else {
      ebus_req = cpu_req;
    }
  }

  ppu.get_vbus_req(vbus_req);
  ppu.get_obus_req(obus_req);

  dma_src_vbus = dma.DMA_RUN_READ && (dma.addr >= ADDR_VRAM_BEGIN) && (dma.addr <= ADDR_VRAM_END);
  dma_src_ebus = dma.DMA_RUN_READ && !dma_src_vbus;

  if (dma_src_vbus) {
    vbus_req.addr = dma.addr;
    vbus_req.data = 0;
    vbus_req.read = 1;
    vbus_req.write = 0;
  }

  if (dma_src_ebus) {
    ebus_req.addr = dma.addr;
    ebus_req.data = 0;
    ebus_req.read = 1;
    ebus_req.write = 0;
  }

  if (MB_DELTA_EF && dma.DMA_RUN_WRITE) {
    obus_req.addr = uint16_t(0xFE00 | (dma.addr & 0xFF));
    obus_req.data = dma_data_latch;
    obus_req.read = 0;
    obus_req.write = 1;
  }

  phase_total++;
}

//-----------------------------------------------------------------------------

void MetroBoy::dump_bus(Dumper& d) {
  static const char* phase_names[] = {
    "\002A_______\001",
    "\003_B______\001",
    "\002__C_____\001",
    "\003___D____\001",
    "\002____E___\001",
    "\003_____F__\001",
    "\002______G_\001",
    "\003_______H\001",
  };

  d("\002------------- BUS --------------\001\n");
  d("phase %s\n", phase_names[phase_total & 7]);
  d("tphase %lld\n", phase_total);
  d("tcycle %lld\n", phase_total >> 1);
  d("mcycle %lld\n", phase_total >> 3);
  d("bgb cycle      0x%08x\n", (phase_total / 4) + 0x00B2D5E6);
  d("boot   %d\n", boot.disable_bootrom);
  d("\n");

  //d.dump_req("---IBUS req:    ", ibus_req);
  //d.dump_ack("---IBUS ack:    ", ibus_ack);
  //d.dump_req("---EBUS req:    ", ebus_req);
  //d.dump_ack("---EBUS ack:    ", ebus_ack);
  //d.dump_req("---VBUS req:    ", vbus_req);
  //d.dump_ack("---VBUS ack:    ", vbus_ack);
  //d.dump_req("---OBUS req:    ", obus_req);
  //d.dump_ack("---OBUS ack:    ", obus_ack);
  d("\n");
}

//-----------------------------------------------------------------------------

void MetroBoy::dump_disasm(Dumper& d) {
  d("\002--------------DISASM-----------\001\n");

  uint16_t pc = gb_cpu.get_op_addr();

  Assembler a;
  if (ADDR_CART_ROM_BEGIN <= pc && pc <= ADDR_CART_ROM_END) {
    a.disassemble(cart.cart_rom, 32768,
                  ADDR_CART_ROM_BEGIN, pc,
                  30, d, false);
  }
  else if (ADDR_MAIN_RAM_BEGIN <= pc && pc <= ADDR_MAIN_RAM_END) {
    a.disassemble(cart.main_ram, 8192,
                  ADDR_MAIN_RAM_BEGIN, pc,
                  30, d, false);
  }
  else if (ADDR_ZEROPAGE_BEGIN <= pc && pc <= ADDR_ZEROPAGE_END) {
    a.disassemble(zram.ram, 127,
                  ADDR_ZEROPAGE_BEGIN, pc,
                  30, d, false);
  }
  else {
    d("(bad pc)\n");
  }
  d("\n");
}

//-----------------------------------------------------------------------------
