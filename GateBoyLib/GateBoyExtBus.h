#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyDMA;
struct GateBoyResetDebug;

//------------------------------------------------------------------------------------------------------------------------

struct ExtDataLatch {
  void reset_to_cart() {
    SOMA_EXT_DATA_LATCH_D0n.reset(0);
    RONY_EXT_DATA_LATCH_D1n.reset(0);
    RAXY_EXT_DATA_LATCH_D2n.reset(0);
    SELO_EXT_DATA_LATCH_D3n.reset(0);
    SODY_EXT_DATA_LATCH_D4n.reset(0);
    SAGO_EXT_DATA_LATCH_D5n.reset(0);
    RUPA_EXT_DATA_LATCH_D6n.reset(0);
    SAZY_EXT_DATA_LATCH_D7n.reset(0);
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

//------------------------------------------------------------------------------------------------------------------------

struct ExtAddrLatch {
  void reset_to_cart() {
    ALOR_EXT_ADDR_LATCH_00p.reset(1);
    APUR_EXT_ADDR_LATCH_01p.reset(0);
    ALYR_EXT_ADDR_LATCH_02p.reset(1);
    ARET_EXT_ADDR_LATCH_03p.reset(1);
    AVYS_EXT_ADDR_LATCH_04p.reset(0);
    ATEV_EXT_ADDR_LATCH_05p.reset(0);
    AROS_EXT_ADDR_LATCH_06p.reset(1);
    ARYM_EXT_ADDR_LATCH_07p.reset(0);
    LUNO_EXT_ADDR_LATCH_08p.reset(0);
    LYSA_EXT_ADDR_LATCH_09p.reset(0);
    PATE_EXT_ADDR_LATCH_10p.reset(0);
    LUMY_EXT_ADDR_LATCH_11p.reset(0);
    LOBU_EXT_ADDR_LATCH_12p.reset(0);
    LONU_EXT_ADDR_LATCH_13p.reset(0);
    NYRE_EXT_ADDR_LATCH_14p.reset(0);
  }

  void copy_cpu_addr_to_addr_latch(const GateBoyResetDebug& rst, Signal BUS_CPU_A[16], wire TEXO_ADDR_VRAMn_ext);

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

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyExtBus {
  void reset_to_cart() {
    ext_addr_latch.reset_to_cart();
    ext_data_latch.reset_to_cart();

    PIN01_A00.reset(0);
    PIN02_A01.reset(1);
    PIN03_A02.reset(0);
    PIN04_A03.reset(0);
    PIN05_A04.reset(1);
    PIN06_A05.reset(1);
    PIN07_A06.reset(0);
    PIN08_A07.reset(1);
    PIN09_A08.reset(1);
    PIN10_A09.reset(1);
    PIN11_A10.reset(1);
    PIN12_A11.reset(1);
    PIN13_A12.reset(1);
    PIN14_A13.reset(1);
    PIN15_A14.reset(1);
    PIN16_A15.reset(0);

    PIN17_D00.reset(0);
    PIN18_D01.reset(0);
    PIN19_D02.reset(0);
    PIN20_D03.reset(0);
    PIN21_D04.reset(0);
    PIN22_D05.reset(0);
    PIN23_D06.reset(0);
    PIN24_D07.reset(0);
  }

  void copy_addr_latch_to_pins(
    const GateBoyResetDebug& rst,
    const GateBoyCpuBus& cpu_bus,
    const GateBoyDMA& dma,
    wire ABUZ_EXT_RAM_CS_CLK
  );

  void copy_pins_to_data_latch(const GateBoyCpuBus& cpu_bus);
  void copy_data_latch_to_cpu_bus(GateBoyCpuBus& cpu_bus);
  void copy_cpu_data_to_pins(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus);

  void set_control_pins(
    const GateBoyResetDebug& rst,
    const GateBoyCpuBus& cpu_bus,
    const GateBoyDMA& dma,
    wire ABUZ_EXT_RAM_CS_CLK);

  void copy_cpu_addr_to_addr_latch(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus);
  void read_ext_to_pins (const uint8_t* cart_buf, const uint8_t* cart_ram, const uint8_t* ext_ram);
  void write_pins_to_ext(uint8_t* cart_ram, uint8_t* ext_ram);

  void dump(Dumper& d) {
    d.dump_slice2n("PIN01_ADDR : ", &PIN01_A00, 16);
    d.dump_slice2n("PIN17_DATA : ", &PIN17_D00, 8);
    d.dump_bitn   ("PIN80_CSn  : ", PIN80_CSn.state);
    d.dump_bitn   ("PIN79_RDn  : ", PIN79_RDn.state);
    d.dump_bitn   ("PIN78_WRn  : ", PIN78_WRn.state);
    d.dump_slice2p("ADDR LATCH : ", &ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p, 15);
    d.dump_slice2n("DATA LATCH : ", &ext_data_latch.SOMA_EXT_DATA_LATCH_D0n, 8);
  }

  PinOut PIN01_A00;
  PinOut PIN02_A01;
  PinOut PIN03_A02;
  PinOut PIN04_A03;
  PinOut PIN05_A04;
  PinOut PIN06_A05;
  PinOut PIN07_A06;
  PinOut PIN08_A07;
  PinOut PIN09_A08;
  PinOut PIN10_A09;
  PinOut PIN11_A10;
  PinOut PIN12_A11;
  PinOut PIN13_A12;
  PinOut PIN14_A13;
  PinOut PIN15_A14;
  PinOut PIN16_A15;

  PinIO  PIN17_D00;
  PinIO  PIN18_D01;
  PinIO  PIN19_D02;
  PinIO  PIN20_D03;
  PinIO  PIN21_D04;
  PinIO  PIN22_D05;
  PinIO  PIN23_D06;
  PinIO  PIN24_D07;

  PinOut PIN80_CSn;      // CS changes on phase C if addr in [A000,FDFF]
  PinOut PIN79_RDn;      // RDn idles low, goes high on phase B for an external write
  PinOut PIN78_WRn;      // WRn idles high, goes low during EFG if there's a write
  ExtDataLatch ext_data_latch;
  ExtAddrLatch ext_addr_latch;

  // pin17 a << RUXA
  // pin17 b << LULA
  // pin17 c >> TOVO,RALO,SOMA
  // pin17 d << RUNE
};

//------------------------------------------------------------------------------------------------------------------------
