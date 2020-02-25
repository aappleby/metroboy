#include "Gameboy.h"

#include "Assembler.h"
#include "Constants.h"

#include <assert.h>

extern uint8_t rom_buf[];
extern const uint8_t DMG_ROM_bin[];

//-----------------------------------------------------------------------------

void Gameboy::reset(size_t new_rom_size, uint16_t new_pc) {
  *this = {};

  z80.reset(new_pc);
  mmu.reset(new_rom_size, new_pc);
  ppu.reset(new_pc == 0);
  oam.reset();
  spu.reset();
  timer.reset();
  vram.reset();
  iram.reset();
  buttons.reset();
  serial.reset();
  zram.reset();

  tcycle = -1;
  trace_val = 0;

  dma_mode_x = DMA_NONE;
  dma_count_x = 0;
  dma_source_x = 0x000;

  dma_mode_a = DMA_NONE;
  dma_count_a = 0;
  dma_source_a = 0x000;

  dma_mode_b = DMA_NONE;
  dma_count_b = 0;
  dma_data_b = 0x00;

  intf = 0xE1;
  imask = 0x00;

  sentinel = 0xDEADBEEF;
}

void Gameboy::reset(uint16_t new_pc) {
  reset(mmu.get_rom_size(), new_pc);
}

//-----------------------------------------------------------------------------
// 4 mhz tick/tock

Bus Gameboy::tick() const {
  return gb_to_bus;
}

//-----------------------------------------------------------------------------

void Gameboy::tock() {
  tcycle++;
  const int tphase = tcycle & 3;
  cpu_to_bus = z80.tick();

  const auto iram_out = iram.tick();
  const auto mmu_out = mmu.tick();
  const auto vram_out = vram.tick();
  const auto oam_out = oam.tick();
  const auto ppu_out = ppu.tick(tcycle);


  iram_to_bus = iram_out.iram_to_bus;
  iram_to_dma = iram_out.iram_to_dma;
 
  mmu_to_bus = mmu_out.mmu_to_bus;
  mmu_to_dma = mmu_out.mmu_to_dma;

  pad_to_bus = buttons.tick();
  serial_to_bus  = serial.tick();;
  zram_to_bus    = zram.tick();
  spu_to_bus     = spu.tick();
  timer_to_bus   = timer.to_bus();
  
  ppu_to_bus  = ppu_out.ppu_to_bus;
  ppu_to_vram = ppu_out.ppu_to_vram;
  ppu_to_oam  = ppu_out.ppu_to_oam;

  vram_to_bus = vram_out.vram_to_bus;
  vram_to_dma = vram_out.vram_to_dma;
  vram_to_ppu = vram_out.vram_to_ppu;

  oam_to_bus = oam_out.oam_to_bus;
  oam_to_ppu = oam_out.oam_to_ppu;

  //-----------------------------------
  // DMA state machine

  if (tphase == 0) {
    dma_mode_b = dma_mode_a;
    dma_count_b = dma_count_a;

    dma_mode_a = dma_mode_x;
    dma_count_a = dma_count_x;
    dma_source_a = dma_source_x;

    if (dma_mode_x != DMA_NONE) dma_count_x++;
    if (dma_count_x == 160) dma_mode_x = DMA_NONE;

    if (cpu_to_bus.write) {
      if (cpu_to_bus.addr == ADDR_DMA) {
        if (cpu_to_bus.data <= 0x7F) dma_mode_x = DMA_CART;
        if (0x80 <= cpu_to_bus.data && cpu_to_bus.data <= 0x9F) dma_mode_x = DMA_VRAM;
        if (0xA0 <= cpu_to_bus.data && cpu_to_bus.data <= 0xBF) dma_mode_x = DMA_CART;
        if (0xC0 <= cpu_to_bus.data && cpu_to_bus.data <= 0xFD) dma_mode_x = DMA_IRAM;
        dma_count_x = 0;
        dma_source_x = cpu_to_bus.data;
      }
    }
  }

  //-----------------------------------
  // interrupt stuff

  uint8_t intf_ = intf;
  uint8_t imask_ = imask;

  if (tphase == 0 || tphase == 2) {
    bool fire_int_timer1   = timer.get_interrupt();;
    bool fire_int_buttons1 = buttons.get() != 0xFF;
    //bool fire_int_timer2 = timer_to_bus.overflow;
    //bool fire_int_buttons2 = pad_to_bus.val != 0xFF;

    if (imask & 0x01) z80.unhalt |= ppu_out.vblank1;
    if (imask & 0x02) z80.unhalt |= ppu_out.stat2;
    if (imask & 0x04) z80.unhalt |= fire_int_timer1;
    if (imask & 0x10) z80.unhalt |= fire_int_buttons1;

    if (ppu_out.vblank1)   intf_ |= INT_VBLANK_MASK;
    if (ppu_out.stat1)     intf_ |= INT_STAT_MASK;
    if (fire_int_timer1)   intf_ |= INT_TIMER_MASK;
    if (fire_int_buttons1) intf_ |= INT_JOYPAD_MASK;
  }

  //-----------------------------------
  // Z80 bus mux & tock

  bus_to_cpu = {};

  bus_to_cpu.addr |= mmu_to_bus.addr;
  bus_to_cpu.addr |= vram_to_bus.addr;
  bus_to_cpu.addr |= iram_to_bus.addr;
  bus_to_cpu.addr |= oam_to_bus.addr;

  bus_to_cpu.addr |= ppu_to_bus.addr;
  bus_to_cpu.addr |= pad_to_bus.addr;
  bus_to_cpu.addr |= serial_to_bus.addr;
  bus_to_cpu.addr |= spu_to_bus.addr;
  bus_to_cpu.addr |= timer_to_bus.addr;
  bus_to_cpu.addr |= zram_to_bus.addr;
  bus_to_cpu.addr |= gb_to_bus.addr;

  bus_to_cpu.data |= mmu_to_bus.data;
  bus_to_cpu.data |= vram_to_bus.data;
  bus_to_cpu.data |= iram_to_bus.data;
  bus_to_cpu.data |= oam_to_bus.data;

  bus_to_cpu.data |= ppu_to_bus.data;
  bus_to_cpu.data |= pad_to_bus.data;
  bus_to_cpu.data |= serial_to_bus.data;
  bus_to_cpu.data |= spu_to_bus.data;
  bus_to_cpu.data |= timer_to_bus.data;
  bus_to_cpu.data |= zram_to_bus.data;
  bus_to_cpu.data |= gb_to_bus.data;

  bus_to_cpu.read += mmu_to_bus.read;
  bus_to_cpu.read += vram_to_bus.read;
  bus_to_cpu.read += iram_to_bus.read;
  bus_to_cpu.read += oam_to_bus.read;

  bus_to_cpu.read += ppu_to_bus.read;
  bus_to_cpu.read += pad_to_bus.read;
  bus_to_cpu.read += serial_to_bus.read;
  bus_to_cpu.read += spu_to_bus.read;
  bus_to_cpu.read += timer_to_bus.read;
  bus_to_cpu.read += zram_to_bus.read;
  bus_to_cpu.read += gb_to_bus.read;

  bus_to_cpu.write = false;

  bus_to_cpu.ack = bus_to_cpu.read;

  if (bus_to_cpu.read > 1) {
    printf("BUS COLLISION @ %d\n", tcycle);
  }
  else if (bus_to_cpu.read == 0) {
    //printf("BUS BLOCKED\n");
    bus_to_cpu.data = 0xFF;
  }

  intf_ &= ~z80.get_int_ack();
  z80.tock(tcycle, bus_to_cpu, imask_, intf_);

  //-----------------------------------
  // Peripheral bus mux & tocks

  dma_to_bus = {};
  if (dma_mode_a != DMA_NONE) {
    dma_to_bus = {
      uint16_t((dma_source_a << 8) | dma_count_a),
      0,
      dma_mode_a != DMA_NONE,
      false,
      false,
      true,
      false,
    };
  }

  iram.tock   (tcycle, cpu_to_bus, dma_to_bus);
  mmu.tock    (tcycle, cpu_to_bus, dma_to_bus);
  vram.tock   (tcycle, cpu_to_bus, dma_to_bus, ppu_to_vram);

  dma_to_oam = {
    uint16_t(ADDR_OAM_BEGIN + dma_count_b),
    uint16_t(mmu_to_dma.data | vram_to_dma.data | iram_to_dma.data),
    false,
    true,
    false,
    true,
    false
  };
  if (dma_mode_b == DMA_NONE) dma_to_oam = {};

  oam.tock    (tcycle, cpu_to_bus, dma_to_oam, ppu_to_oam);

  buttons.tock(tcycle, cpu_to_bus);
  serial.tock (tcycle, cpu_to_bus);
  zram.tock   (tcycle, cpu_to_bus);
  spu.tock    (tcycle, cpu_to_bus);
  timer.tock  (tcycle, cpu_to_bus);
  ppu.tock    (tcycle, cpu_to_bus, vram_to_ppu, oam_to_ppu);

  intf = intf_;
  imask = imask_;

  //-----------------------------------
  // Internal read/write for intf/imask

  gb_to_bus = {};
  if (cpu_to_bus.write) {
    if (cpu_to_bus.addr == ADDR_IF) {
      intf_ = (uint8_t)cpu_to_bus.data | 0b11100000;
    }
    if (cpu_to_bus.addr == ADDR_IE) {
      imask_ = (uint8_t)cpu_to_bus.data;
    }
  }
  else if (cpu_to_bus.read) {
    if (cpu_to_bus.addr == ADDR_IF) { 
      // FIXME intf or intf_?
      gb_to_bus.addr = cpu_to_bus.addr;
      gb_to_bus.data = 0b11100000 | intf_;
      gb_to_bus.read = true;
    }
    if (cpu_to_bus.addr == ADDR_IE) {
      // FIXME imask or imask_?
      gb_to_bus.addr = cpu_to_bus.addr;
      gb_to_bus.data = imask_;
      gb_to_bus.read = true;
    }
  }

  //----------

  gb_to_host.x       = ppu_out.x;
  gb_to_host.y       = ppu_out.y;
  gb_to_host.counter = ppu_out.counter;
  gb_to_host.pix     = ppu_out.pix_out;
  gb_to_host.pix_oe  = ppu_out.pix_oe;
  gb_to_host.out_r   = spu.get_r();
  gb_to_host.out_l   = spu.get_l();
  gb_to_host.trace   = ppu_to_vram.addr;
}

//-----------------------------------------------------------------------------

#if 0
{
//return z80.get_state() << 4;

//return z80.get_op();

//return z80.op_ == 0x76 ? 0xFFFFFFFF : 0; // moderately interesting
//return z80.op_ == 0x00 ? 0xFFFFFFFF : 0; // moderately interesting
//return z80.op_ == 0xcb ? 0xFFFFFFFF : 0; // moderately interesting
//return (z80.get_op() & 0b11000000) == 0b10000000 ? 0xFFFFFFFF : 0; // moderately interesting

//return (z80.mem_addr >= ADDR_SPU_BEGIN && z80.mem_addr < ADDR_SPU_END) ? -1 : 0; // sparse

//return ppu.sprite_index << 4; // also pretty cool

//return cpu_to_bus.addr;
trace_val = ppu_out.vram_addr; // this one's pretty cool
                                //return cpu_to_bus.write ? 0xFFFFFFFF : 0x00000000;
}
#endif

//-----------------------------------------------------------------------------

void Gameboy::dump1(std::string& d) {

  sprintf(d, "\002--------------CPU--------------\001\n");
  z80.dump(d);
  sprintf(d, "\n");

  sprintf(d, "\002--------------BUS--------------\001\n");
  sprintf(d, "tcycle         %d\n", tcycle);
  //sprintf(d, "dma_mode_a     %d\n", dma_mode_a);
  sprintf(d, "dma_count_a    %d\n", dma_count_a);
  //sprintf(d, "dma_source_a   0x%04x\n", dma_source_a);
  //sprintf(d, "dma_mode_b     %d\n", dma_mode_b);
  sprintf(d, "dma_count_b    %d\n", dma_count_b);
  //sprintf(d, "dma_data_b     %d\n", dma_data_b);
  sprintf(d, "imask          %s\n", byte_to_bits(imask));
  sprintf(d, "intf           %s\n", byte_to_bits(intf));
  sprintf(d, "\n");

  print_bus(d, "cpu_to_bus",     cpu_to_bus);
  sprintf(d, "\n");
  print_bus(d, "gby_to_bus",     gb_to_bus);
  print_bus(d, "irm_to_bus",     iram_to_bus);
  print_bus(d, "mmu_to_bus",     mmu_to_bus);
  print_bus(d, "pad_to_bus",     pad_to_bus);
  print_bus(d, "ser_to_bus",     serial_to_bus);
  print_bus(d, "zrm_to_bus",     zram_to_bus);
  print_bus(d, "spu_to_bus",     spu_to_bus);
  print_bus(d, "tmr_to_bus",     timer_to_bus);
  print_bus(d, "ppu_to_bus",     ppu_to_bus);
  print_bus(d, "vrm_to_bus",     vram_to_bus);
  print_bus(d, "oam_to_bus",     oam_to_bus);
  print_bus(d, "dma_to_bus",     dma_to_bus);
  sprintf(d, "\n");
  print_bus(d, "bus_to_cpu",     bus_to_cpu);
  sprintf(d, "\n");

  /*
  print_bus(d, "ppu_to_vram",    ppu_to_vram);
  print_bus(d, "vram_to_ppu",    vram_to_ppu);
  sprintf(d, "\n");

  print_bus(d, "ppu_to_oam",     ppu_to_oam);
  print_bus(d, "oam_to_ppu",     oam_to_ppu);
  sprintf(d, "\n");
  */

  //print_bus(d, "bus_to_dma",     bus_to_dma);
  print_bus(d, "dma_to_oam",     dma_to_oam);

  //print_bus(d, "gb_to_bus", gb_to_bus);
  //print_bus(d, "bus_to_cpu", bus_to_cpu);


  /*
  sprintf(d, "\002--------------TIMER------------\001\n");
  timer.dump(d);
  sprintf(d, "\n");
  */

  sprintf(d, "\002--------------MMU--------------\001\n");
  mmu.dump(d);
  sprintf(d, "\n");

  sprintf(d, "\002--------------VRAM-------------\001\n");
  vram.dump(d);
  sprintf(d, "\n");

  sprintf(d, "\002--------------IRAM-------------\001\n");
  iram.dump(d);
  sprintf(d, "\n");

  sprintf(d, "\002--------------OAM--------------\001\n");
  oam.dump(d);
  sprintf(d, "\n");
}

//-----------------------------------------------------------------------------

void Gameboy::dump2(std::string& d) {
  sprintf(d, "\002--------------ZRAM-------------\001\n");
  zram.dump(d);
  sprintf(d, "\n");

  sprintf(d, "\002--------------BUTTONS----------\001\n");
  buttons.dump(d);
  sprintf(d, "\n");

  sprintf(d, "\002--------------SERIAL-----------\001\n");
  serial.dump(d);
}

//-----------------------------------------------------------------------------

void Gameboy::dump3(std::string& d) {
  sprintf(d, "\002--------------DISASM-----------\001\n");

  uint16_t pc = z80.get_pc();
  const uint8_t* segment;
  
  if (ADDR_IRAM_BEGIN <= pc && pc <= ADDR_IRAM_END) {
    segment = iram.get() + (pc - ADDR_IRAM_BEGIN);
  }
  else if (ADDR_ZEROPAGE_BEGIN <= pc && pc <= ADDR_ZEROPAGE_END) {
    segment = zram.get() + (pc - ADDR_ZEROPAGE_BEGIN);
  }
  else if (ADDR_OAM_BEGIN <= pc && pc <= ADDR_OAM_END) {
    segment = oam.get() + (pc - ADDR_OAM_BEGIN);
  }
  else {
    segment = mmu.get_flat_ptr(pc);
  }

  if (segment) {
    Assembler a;
    a.disassemble(segment, 65536, pc, 30, d, false);
    sprintf(d, "\n");
  }
  else {
    sprintf(d, "(bad pc)\n");
  }
}

void Gameboy::dump4(std::string& d) {
  sprintf(d, "\002--------------TIMER------------\001\n");
  timer.dump(d);
  sprintf(d, "\n");
}

//-----------------------------------------------------------------------------