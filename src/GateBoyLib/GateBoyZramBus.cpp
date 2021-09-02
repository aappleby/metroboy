#include "GateBoyLib/GateBoyZramBus.h"

#include "GateBoyLib/GateBoy.h"

// ZRAM control signals are
// clk_reg.SIG_CPU_BUKE_AxxxxxGH
// TEDO_CPU_RDp();
// TAPU_CPU_WRp_xxxxEFGx()
// SYKE_FF00_FFFFp
// and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_zram_gates()
{
  uint16_t addr = (uint16_t)pack_new(16, (BitBase*)&new_bus.BUS_CPU_A00p);
  wire CSp = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (bit(zram_bus.clk_old.out_old() & ~cpu_signals.TAPU_CPU_WRp.out_new() & CSp)) {
    zero_ram[addr & 0x007F] = (uint8_t)pack_old(8, (BitBase*)&old_bus.BUS_CPU_D00p);
  }
  zram_bus.clk_old = cpu_signals.TAPU_CPU_WRp.out_new();

  uint8_t data = zero_ram[addr & 0x007F];

  triwire tri0 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 0));
  triwire tri1 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 1));
  triwire tri2 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 2));
  triwire tri3 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 3));
  triwire tri4 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 4));
  triwire tri5 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 5));
  triwire tri6 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 6));
  triwire tri7 = tri_pp(CSp && bit(cpu_signals.TEDO_CPU_RDp.out_new()), bit(data, 7));

  new_bus.BUS_CPU_D00p.tri_bus(tri0);
  new_bus.BUS_CPU_D01p.tri_bus(tri1);
  new_bus.BUS_CPU_D02p.tri_bus(tri2);
  new_bus.BUS_CPU_D03p.tri_bus(tri3);
  new_bus.BUS_CPU_D04p.tri_bus(tri4);
  new_bus.BUS_CPU_D05p.tri_bus(tri5);
  new_bus.BUS_CPU_D06p.tri_bus(tri6);
  new_bus.BUS_CPU_D07p.tri_bus(tri7);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_zram_logic()
{
  uint16_t addr = (uint16_t)pack_new(16, (BitBase*)&new_bus.BUS_CPU_A00p);
  wire CSp = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (bit(zram_bus.clk_old.state & ~cpu_signals.TAPU_CPU_WRp.state & CSp)) {
    zero_ram[addr & 0x007F] = (uint8_t)pack_old(8, (BitBase*)&old_bus.BUS_CPU_D00p);
  }
  zram_bus.clk_old = cpu_signals.TAPU_CPU_WRp.state;

  uint8_t data = zero_ram[addr & 0x007F];

  if (CSp && bit(cpu_signals.TEDO_CPU_RDp.state)) {
    triwire tri0 = tri_pp(1, bit(data, 0));
    triwire tri1 = tri_pp(1, bit(data, 1));
    triwire tri2 = tri_pp(1, bit(data, 2));
    triwire tri3 = tri_pp(1, bit(data, 3));
    triwire tri4 = tri_pp(1, bit(data, 4));
    triwire tri5 = tri_pp(1, bit(data, 5));
    triwire tri6 = tri_pp(1, bit(data, 6));
    triwire tri7 = tri_pp(1, bit(data, 7));

    new_bus.BUS_CPU_D00p.tri_bus(tri0);
    new_bus.BUS_CPU_D01p.tri_bus(tri1);
    new_bus.BUS_CPU_D02p.tri_bus(tri2);
    new_bus.BUS_CPU_D03p.tri_bus(tri3);
    new_bus.BUS_CPU_D04p.tri_bus(tri4);
    new_bus.BUS_CPU_D05p.tri_bus(tri5);
    new_bus.BUS_CPU_D06p.tri_bus(tri6);
    new_bus.BUS_CPU_D07p.tri_bus(tri7);
  }
}

//------------------------------------------------------------------------------------------------------------------------
