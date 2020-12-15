#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyDMA;
struct GateBoyResetDebug;

//-----------------------------------------------------------------------------

struct ExtDataLatch {
  void reset_cart() {
    SOMA_EXT_DATA_LATCH_D0n.reset(REG_D1C0);
    RONY_EXT_DATA_LATCH_D1n.reset(REG_D0C0);
    RAXY_EXT_DATA_LATCH_D2n.reset(REG_D1C0);
    SELO_EXT_DATA_LATCH_D3n.reset(REG_D0C0);
    SODY_EXT_DATA_LATCH_D4n.reset(REG_D1C0);
    SAGO_EXT_DATA_LATCH_D5n.reset(REG_D1C0);
    RUPA_EXT_DATA_LATCH_D6n.reset(REG_D1C0);
    SAZY_EXT_DATA_LATCH_D7n.reset(REG_D1C0);
  }

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_D0n; // AxCxExxx
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_D1n; // AxCxExxx
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_D2n; // AxCxExxx
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_D3n; // AxCxExxx
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_D4n; // AxCxExxx
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_D5n; // AxCxExxx
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_D6n; // AxCxExxx
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_D7n; // AxCxExxx
};

struct ExtAddrLatch {
  void reset_cart() {
    ALOR_EXT_ADDR_LATCH_00p.reset(REG_D1C0);
    APUR_EXT_ADDR_LATCH_01p.reset(REG_D0C0);
    ALYR_EXT_ADDR_LATCH_02p.reset(REG_D1C0);
    ARET_EXT_ADDR_LATCH_03p.reset(REG_D1C0);
    AVYS_EXT_ADDR_LATCH_04p.reset(REG_D0C0);
    ATEV_EXT_ADDR_LATCH_05p.reset(REG_D0C0);
    AROS_EXT_ADDR_LATCH_06p.reset(REG_D1C0);
    ARYM_EXT_ADDR_LATCH_07p.reset(REG_D0C0);
    LUNO_EXT_ADDR_LATCH_08p.reset(REG_D0C0);
    LYSA_EXT_ADDR_LATCH_09p.reset(REG_D0C0);
    PATE_EXT_ADDR_LATCH_10p.reset(REG_D0C0);
    LUMY_EXT_ADDR_LATCH_11p.reset(REG_D0C0);
    LOBU_EXT_ADDR_LATCH_12p.reset(REG_D0C0);
    LONU_EXT_ADDR_LATCH_13p.reset(REG_D0C0);
    NYRE_EXT_ADDR_LATCH_14p.reset(REG_D0C0);
  }

  void tock(const GateBoyResetDebug& rst, Signal BUS_CPU_A[16], wire TEXO_ADDR_VRAMn_ext);

  /*p08.ALOR*/ TpLatch ALOR_EXT_ADDR_LATCH_00p; // xBxxxxxx
  /*p08.APUR*/ TpLatch APUR_EXT_ADDR_LATCH_01p; // xBxxxxxx
  /*p08.ALYR*/ TpLatch ALYR_EXT_ADDR_LATCH_02p; // xBxxxxxx
  /*p08.ARET*/ TpLatch ARET_EXT_ADDR_LATCH_03p; // xBxxxxxx
  /*p08.AVYS*/ TpLatch AVYS_EXT_ADDR_LATCH_04p; // xBxxxxxx
  /*p08.ATEV*/ TpLatch ATEV_EXT_ADDR_LATCH_05p; // xBxxxxxx
  /*p08.AROS*/ TpLatch AROS_EXT_ADDR_LATCH_06p; // xBxxxxxx
  /*p08.ARYM*/ TpLatch ARYM_EXT_ADDR_LATCH_07p; // xBxxxxxx
  /*p08.LUNO*/ TpLatch LUNO_EXT_ADDR_LATCH_08p; // ABxxxxxx
  /*p08.LYSA*/ TpLatch LYSA_EXT_ADDR_LATCH_09p; // ABxxxxxx
  /*p08.PATE*/ TpLatch PATE_EXT_ADDR_LATCH_10p; // ABxxxxxx
  /*p08.LUMY*/ TpLatch LUMY_EXT_ADDR_LATCH_11p; // ABxxxxxx
  /*p08.LOBU*/ TpLatch LOBU_EXT_ADDR_LATCH_12p; // ABxxxxxx
  /*p08.LONU*/ TpLatch LONU_EXT_ADDR_LATCH_13p; // ABxxxxxx
  /*p08.NYRE*/ TpLatch NYRE_EXT_ADDR_LATCH_14p; // ABxxxxxx
};

struct GateBoyExtBus {
  void reset_cart() {
    PIN_EXT_A[ 0].reset(REG_D1C0);
    PIN_EXT_A[ 1].reset(REG_D0C0);
    PIN_EXT_A[ 2].reset(REG_D1C0);
    PIN_EXT_A[ 3].reset(REG_D1C0);
    PIN_EXT_A[ 4].reset(REG_D0C0);
    PIN_EXT_A[ 5].reset(REG_D0C0);
    PIN_EXT_A[ 6].reset(REG_D1C0);
    PIN_EXT_A[ 7].reset(REG_D0C0);
    PIN_EXT_A[ 8].reset(REG_D0C0);
    PIN_EXT_A[ 9].reset(REG_D0C0);
    PIN_EXT_A[10].reset(REG_D0C0);
    PIN_EXT_A[11].reset(REG_D0C0);
    PIN_EXT_A[12].reset(REG_D0C0);
    PIN_EXT_A[13].reset(REG_D0C0);
    PIN_EXT_A[14].reset(REG_D0C0);
    PIN_EXT_A[15].reset(REG_D1C0);

    PIN_EXT_D[0].reset(REG_D0C0);
    PIN_EXT_D[1].reset(REG_D1C0);
    PIN_EXT_D[2].reset(REG_D0C0);
    PIN_EXT_D[3].reset(REG_D1C0);
    PIN_EXT_D[4].reset(REG_D0C0);
    PIN_EXT_D[5].reset(REG_D0C0);
    PIN_EXT_D[6].reset(REG_D0C0);
    PIN_EXT_D[7].reset(REG_D0C0);
  }

  void addr_latch_to_pins(
    const GateBoyResetDebug& rst,
    const GateBoyCpuBus& cpu_bus,
    const GateBoyDMA& dma,
    const ExtAddrLatch& ext_addr_latch,
    wire ABUZ_EXT_RAM_CS_CLK,
    wire TUTU_READ_BOOTROMp
  );

  void pins_to_data_latch(const GateBoyCpuBus& cpu_bus, ExtDataLatch& ext_data_latch);
  void data_latch_to_cpu_bus(GateBoyCpuBus& cpu_bus, const ExtDataLatch& ext_data_latch);
  void cpu_data_to_pins(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus);

  void set_pins(
    const GateBoyResetDebug& rst,
    const GateBoyCpuBus& cpu_bus,
    const GateBoyDMA& dma,
    wire ABUZ_EXT_RAM_CS_CLK);

  void cart_to_pins(const uint8_t* cart_buf, uint8_t* cart_ram, uint8_t* ext_ram);

  PinOut PIN_EXT_CSn;    // PIN_80 // CS changes on phase C if addr in [A000,FDFF]
  PinOut PIN_EXT_RDn;    // PIN_79 // RDn idles low, goes high on phase B for an external write
  PinOut PIN_EXT_WRn;    // PIN_78 // WRn idles high, goes low during EFG if there's a write
  PinOut PIN_EXT_A[16];
  PinIO  PIN_EXT_D[8];
};

//-----------------------------------------------------------------------------
