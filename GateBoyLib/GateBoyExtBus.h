#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyDMA;
struct GateBoyResetDebug;

//-----------------------------------------------------------------------------

struct ExtDataLatch {
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
  void addr_latch_to_pins(
    const GateBoyResetDebug& rst,
    const GateBoyDMA& dma,
    const ExtAddrLatch& ext_addr_latch,
    Signal BUS_CPU_A[16],
    wire ABUZ_xxCDEFGH,
    wire TUTU_READ_BOOTROMp
  );

  void pins_to_data_latch(
    wire PIN_CPU_RDp,
    wire PIN_CPU_LATCH_EXT,
    wire TEXO_ADDR_VRAMn,
    ExtDataLatch& ext_data_latch);

  void data_latch_to_cpu_bus(
    const ExtDataLatch& ext_data_latch,
    wire PIN_CPU_RDp,
    wire PIN_CPU_LATCH_EXT,
    wire TEXO_ADDR_VRAMn,
    BusOut BUS_CPU_D_out[8]
  );

  void cpu_data_to_pins(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus);

  void set_pins(
    const GateBoyResetDebug& rst,
    const GateBoyCpuBus& cpu_bus,
    const GateBoyDMA& dma,
    wire ABUZ_xxCDEFGH);

  void cart_to_pins(const uint8_t* cart_buf, uint8_t* cart_ram, uint8_t* ext_ram);

  PinOut PIN_EXT_CSn;    // PIN_80 // CS changes on phase C if addr in [A000,FDFF]
  PinOut PIN_EXT_RDn;    // PIN_79 // RDn idles low, goes high on phase B for an external write
  PinOut PIN_EXT_WRn;    // PIN_78 // WRn idles high, goes low during EFG if there's a write
  PinOut PIN_EXT_A[16];
  PinIO  PIN_EXT_D[8];
};

//-----------------------------------------------------------------------------
