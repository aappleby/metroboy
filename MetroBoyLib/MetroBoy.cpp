#include "MetroBoyLib/MetroBoy.h"

#include "CoreLib/Assembler.h"
#include "CoreLib/Constants.h"

#include <assert.h>
#include "imgui/imgui.h"

#pragma warning(disable:4189)
#pragma warning(disable:4244)

//-----------------------------------------------------------------------------

void MetroBoy::reset(uint16_t new_pc, uint8_t* new_rom, size_t new_rom_size) {
  check_sentinel();

  z80.reset(new_pc);
  cart.set_rom(new_rom, new_rom_size);
  cart.reset();
  ppu.reset(new_pc == 0);
  oam.reset();
  spu.reset();
  timer.reset();
  vram.reset();
  joypad.reset();
  serial.reset();
  zram.reset();

  boot.disable_bootrom = new_pc != 0x0000;

  phase_total = 0;

  ebus_req.addr = new_pc;
  ebus_req.data = 0x00;
  ebus_req.read = 1;
  ebus_req.write = 0;

  ebus_ack.addr = new_pc;
  ebus_ack.data = 0x00;
  ebus_ack.read = 1;

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
    joypad.tick(phase_total, ibus_req, ibus_ack);
    zram.  tick(phase_total, ibus_req, ibus_ack);
    spu.   tick(phase_total, ibus_req, ibus_ack);
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

  CHECK_N(ibus_ack.read > 1);
  CHECK_N(ebus_ack.read > 1);
  CHECK_N(vbus_ack.read > 1);
  CHECK_N(obus_ack.read > 1);

  if (DELTA_HA) {
    bool cpu_has_ibus_req = cpu_req.addr >= ADDR_IOBUS_BEGIN;
    bool cpu_has_vbus_req = cpu_req.addr >= ADDR_VRAM_BEGIN && cpu_req.addr <= ADDR_VRAM_END;
    bool cpu_has_obus_req = cpu_req.addr >= ADDR_OAM_BEGIN  && cpu_req.addr <= ADDR_OAM_END;
    bool cpu_has_ebus_req = !cpu_has_vbus_req && !cpu_has_obus_req && !cpu_has_ibus_req;

    uint8_t bus_data = 0xFF;
    if      (cpu_has_ibus_req) bus_data = ibus_ack.data_lo;
    else if (cpu_has_ebus_req) bus_data = ebus_ack.data_lo;
    else if (cpu_has_vbus_req) bus_data = vbus_ack.data_lo;
    else if (cpu_has_obus_req) bus_data = obus_ack.data_lo;

    z80.tock_ha(ints.imask, ints.intf, ints.imask, ints.intf, bus_data);

    /*
    z80.tock_ack(imask_delay, intf_delay, bus_data);
    z80.tock_req(imask_delay, intf_delay);

    intf_delay = ints.intf;
    imask_delay = ints.imask;
    */
  }

  timer. tock(phase_total, ibus_req);
  serial.tock(phase_total, ibus_req);
  joypad.tock(phase_total, ibus_req);
  boot.  tock(phase_total, ibus_req);
  zram.  tock(phase_total, ibus_req);
  spu.   tock(phase_total, ibus_req);
  ppu.   tock(phase_total, ibus_req, vbus_ack, obus_ack);
  dma.   tock(phase_total, ibus_req);
  cart.  tock(phase_total, ebus_req);
  vram.  tock(phase_total, vbus_req);
  oam.   tock(phase_total, obus_req);
  ints.  tock(phase_total, ibus_req, z80.int_ack, ppu.vblank_int, ppu.stat_int, timer.timer_int, /*serial_int*/ 0, joypad.get() != 0xFF);

  //----------

  int pix_x = ppu.pix_count - 8;
  int pix_y = ppu.line;

  if (pix_x >= 0 && pix_x < 160 && pix_y >= 0 && pix_y < 144) {
    framebuffer[pix_x + pix_y * 160] = ppu.pix_out;
  }

  //-----------------------------------
  // prioritize reqs

  if (DELTA_AB) {
    cpu_req = z80.bus_req;

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

    // hackkkk
    /*
    if (cpu_req.write && cpu_req.addr >= 0x8000 && cpu_req.addr <= 0x9FFF) {
      vram.ram[cpu_req.addr - 0x8000] = cpu_req.data;
    }
    */
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

  if (DELTA_EF && dma.DMA_RUN_WRITE) {
    obus_req.addr = uint16_t(0xFE00 | (dma.addr & 0xFF));
    obus_req.data = dma_data_latch;
    obus_req.read = 0;
    obus_req.write = 1;
  }

  phase_total++;
}

//-----------------------------------------------------------------------------

void MetroBoy::dump_bus(Dumper& d) {
  const char* phases[] = {
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
  d("phase %s\n", phases[phase_total & 7]);
  d("tphase %lld\n", phase_total);
  d("tcycle %lld\n", phase_total >> 1);
  d("mcycle %lld\n", phase_total >> 3);
  d("bgb cycle      0x%08x\n", (phase_total / 4) + 0x00B2D5E6);
  d("boot   %d\n", boot.disable_bootrom);
  d("\n");

  d("---IBUS req:    "); dump_req(d, ibus_req);
  d("---IBUS ack:    "); dump_ack(d, ibus_ack);
  d("---EBUS req:    "); dump_req(d, ebus_req);
  d("---EBUS ack:    "); dump_ack(d, ebus_ack);
  d("---VBUS req:    "); dump_req(d, vbus_req);
  d("---VBUS ack:    "); dump_ack(d, vbus_ack);
  d("---OBUS req:    "); dump_req(d, obus_req);
  d("---OBUS ack:    "); dump_ack(d, obus_ack);
  d("\n");
}

//-----------------------------------------------------------------------------

void MetroBoy::dump_disasm(Dumper& d) {
  d("\002--------------DISASM-----------\001\n");

  uint16_t pc = z80.op_addr;

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