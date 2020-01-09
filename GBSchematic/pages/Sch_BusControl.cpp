#include "Sch_BusControl.h"

#include "Sch_CpuPins.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Debug.h"
#include "Sch_Decoder.h"
#include "Sch_DMA.h"
#include "Sch_System.h"
#include "Sch_Video.h"
#include "Sch_Vram.h"

namespace Schematics {

/*
0b000xxxxx_xxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxx_xxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxx_xxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxx_xxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxx_xxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxx_xxxxxxxx - 0xA000 to 0xBFFF (cart ram)
0b110xxxxx_xxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxx_xxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)

0b11111110_00000000 - 0xFE00 - OAM begin
0b11111110_10011111 - 0xFE9F - OAM end

0b11111111_00000000 - 0xFF00 - IO begin
0b11111111_01111111 - 0xFF7F - IO end

0b11111111_10000000 - 0xFF80 - Zeropage begin
0b11111111_11111110 - 0xFFFE - Zeropage end
0b11111111_11111111 - 0xFFFF - Interrupt enable
*/

//-----------------------------------------------------------------------------

BusSignals BusSignals::tick(const SysSignals& sys_sig,
                            const ClkSignals& clk_sig,
                            BusTristates& bus_tri) {
  BusSignals bus_sig = {};

  /*p01.AREV*/ wire AREV = nand(sys_sig.CPU_RAW_WR, clk_sig.AFAS_xxxxEFGx);
  /*p01.APOV*/ wire CPU_WR_xxxxEFGx = not(AREV);

  /*p07.UJYV*/ wire CPU_RD_MUX = mux2n(sys_sig.PIN_RD_C, sys_sig.CPU_RAW_RD, sys_sig.MODE_DBG2);
  /*p07.UBAL*/ wire CPU_WR_MUX = mux2n(sys_sig.PIN_WR_C, CPU_WR_xxxxEFGx, sys_sig.MODE_DBG2);

  /*p07.TEDO*/ wire TEDO_CPURD  = not(CPU_RD_MUX);
  /*p07.TAPU*/ wire TAPU_CPUWR  = not(CPU_WR_MUX);
    
  /*p07.AJAS*/ wire CPU_RDn = not(TEDO_CPURD);
  /*p07.DYKY*/ wire CPU_WRn = not(TAPU_CPUWR);

  /*p07.ASOT*/ wire ASOT_CPURD = not(CPU_RDn);
  /*p07.CUPA*/ wire CUPA_CPUWR = not(CPU_WRn);

  /*p07.TEDO*/ bus_sig.TEDO_CPURD = TEDO_CPURD;
  /*p07.TAPU*/ bus_sig.TAPU_CPUWR = TAPU_CPUWR;
  /*p07.ASOT*/ bus_sig.ASOT_CPURD = ASOT_CPURD;
  /*p07.CUPA*/ bus_sig.CUPA_CPUWR = CUPA_CPUWR;

  /*p08.SORE*/ wire ADDR_0000_7FFF = not(bus_tri.A15());
  /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(bus_tri.A13(), bus_tri.A14(), ADDR_0000_7FFF);
  /*p08.TEXO*/ wire ADDR_VALID_AND_NOT_VRAM = and(sys_sig.CPU_ADDR_VALID, ADDR_NOT_VRAM);
  /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(ADDR_VALID_AND_NOT_VRAM);
  /*p08.TEXO*/ bus_sig.ADDR_VALID_AND_NOT_VRAM = ADDR_VALID_AND_NOT_VRAM;
  /*p08.LEVO*/ bus_sig.ADDR_VALID_AND_NOT_VRAMn = ADDR_VALID_AND_NOT_VRAMn;

  /*p08.MOCA*/ wire DBG_EXT_RDn = nor(ADDR_VALID_AND_NOT_VRAM, sys_sig.MODE_DBG1);
  /*p08.LAGU*/ wire LAGU = or(and(sys_sig.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), sys_sig.CPU_RAW_WR);
  /*p08.LYWE*/ wire LYWE = not(LAGU);

  /*p08.REDU*/ wire CPU_RDo = not(TEDO_CPURD);
  /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
  /*p08.RORU*/ wire CBUS_TO_CEXTn = mux2(CPU_RDo, CPU_EXT_RD, sys_sig.MODE_DBG2);
  /*p08.LULA*/ wire CBUS_TO_CEXT  = not(CBUS_TO_CEXTn);
  /*p08.RORU*/ bus_sig.CBUS_TO_CEXTn = CBUS_TO_CEXTn;
  /*p08.LULA*/ bus_sig.CBUS_TO_CEXT  = CBUS_TO_CEXT;

  return bus_sig;
}

//-----------------------------------------------------------------------------

};



















