#include "GateBoyLib/GateBoyZramBus.h"

#include "GateBoyLib/GateBoyCpuBus.h"

// ZRAM control signals are
// clk_reg.SIG_CPU_BUKE_AxxxxxGH
// TEDO_CPU_RDp();
// TAPU_CPU_WRp_xxxxEFGx()
// _SYKE_FF00_FFFFp
// and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

//------------------------------------------------------------------------------------------------------------------------

void GateBoyZramBus::read(GateBoyCpuBus& cpu_bus, uint8_t* zero_ram)
{
  uint16_t addr = (uint16_t)BitBase::pack_new(16, cpu_bus.BUS_CPU_A);
  wire CSp = (addr >= 0xFF80) && (addr <= 0xFFFE);

  uint8_t data = zero_ram[addr & 0x007F];
  cpu_bus.BUS_CPU_D[0].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x01));
  cpu_bus.BUS_CPU_D[1].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x02));
  cpu_bus.BUS_CPU_D[2].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x04));
  cpu_bus.BUS_CPU_D[3].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x08));
  cpu_bus.BUS_CPU_D[4].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x10));
  cpu_bus.BUS_CPU_D[5].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x20));
  cpu_bus.BUS_CPU_D[6].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x40));
  cpu_bus.BUS_CPU_D[7].tri(CSp && cpu_bus.TEDO_CPU_RDp.qp_new(), wire(data & 0x80));
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyZramBus::write(GateBoyCpuBus& cpu_bus, uint8_t* zero_ram)
{
  uint16_t addr = (uint16_t)BitBase::pack_new(16, cpu_bus.BUS_CPU_A);
  wire CSp = (addr >= 0xFF80) && (addr <= 0xFFFE);

  wire clk_new = !cpu_bus.TAPU_CPU_WRp.qp_new();
  if (!clk_old.qp_old() && clk_new && CSp) {
    zero_ram[addr & 0x007F] = (uint8_t)BitBase::pack_old(8, cpu_bus.BUS_CPU_D);
  }
  clk_old.set_new(clk_new);
}

//------------------------------------------------------------------------------------------------------------------------