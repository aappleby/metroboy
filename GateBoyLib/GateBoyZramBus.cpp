#include "GateBoyLib/GateBoyZramBus.h"

#include "GateBoyLib/GateBoy.h"

// ZRAM control signals are
// clk_reg.SIG_CPU_BUKE_AxxxxxGH
// TEDO_CPU_RDp();
// TAPU_CPU_WRp_xxxxEFGx()
// _SYKE_FF00_FFFFp
// and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::read_zram()
{
  uint16_t addr = (uint16_t)BitBase::pack_new(16, &new_bus.BUS_CPU_A00p);
  wire CSp = (addr >= 0xFF80) && (addr <= 0xFFFE);

  uint8_t data = zero_ram[addr & 0x007F];
  new_bus.BUS_CPU_D00p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 0));
  new_bus.BUS_CPU_D01p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 1));
  new_bus.BUS_CPU_D02p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 2));
  new_bus.BUS_CPU_D03p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 3));
  new_bus.BUS_CPU_D04p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 4));
  new_bus.BUS_CPU_D05p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 5));
  new_bus.BUS_CPU_D06p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 6));
  new_bus.BUS_CPU_D07p.tri(CSp && bit(cpu_bus.TEDO_CPU_RDp.qp_new()), bit(data, 7));
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::write_zram()
{
  uint16_t addr = (uint16_t)BitBase::pack_new(16, &new_bus.BUS_CPU_A00p);
  wire CSp = (addr >= 0xFF80) && (addr <= 0xFFFE);

  wire clk_new = bit(~cpu_bus.TAPU_CPU_WRp.qp_new());
  if (bit(~zram_bus.clk_old.qp_old()) && clk_new && CSp) {
    zero_ram[addr & 0x007F] = (uint8_t)BitBase::pack_old(8, &old_bus.BUS_CPU_D00p);
  }
  zram_bus.clk_old = clk_new;
}

//------------------------------------------------------------------------------------------------------------------------
