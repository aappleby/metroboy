#include "P08_ExtCpuBuses.h"
#include "../Schematics.h"
#include "Gameboy.h"

void blah(const Gameboy& b, Gameboy& c) {

  c.p08.ADDR_0000_7FFF = not(b.A15);
  c.p08.ADDR_NOT_VRAM = or(b.A13, b.A14, b.p08.ADDR_0000_7FFF);

  c.p08.TEXO = and(b.cpu.ADDR_VALID, b.p08.ADDR_NOT_VRAM);

  c.p08.LEVO = not(b.p08.TEXO);

  c.p08.LAGU = unk3(b.cpu.CPU_RAW_RD, b.p08.LEVO, b.cpu.CPU_RAW_WR);

  c.p08.LYWE = not(b.p08.LAGU);

  c.p08.MOCA = nor(b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.MEXO = not(b.p01.CPU_WR_SYNC);
  c.p08.NEVY = or(b.p08.MEXO, b.p08.MOCA);
  c.p08.MOTY = or(b.p08.MOCA, b.p08.LYWE);
  c.p08.PUVA = or(b.p08.NEVY, b.p04.DO_DMA);
  c.p08.TYMU = nor(b.p04.DO_DMA, b.p08.MOTY);

  c.p08.MODE_DBG1n = not(b.p07.MODE_DBG1);
  c.p08.A1n = not(b.A01);
  c.p08.LOXO = unk3(b.p08.MODE_DBG1n, b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.LASY = not(b.p08.LOXO);
  c.p08.MATE = not(b.p08.LASY);

  c.p08.SOGY = not(b.A14);
  c.p08.CART_RAM = and(b.A13, b.p08.SOGY, b.A15);

  c.p08.TYNU = unk3(b.A15, b.A14, b.p08.CART_RAM);

  c.p08.TOZA = and(b.p01.CPU_RD_SYNC, 
  b.p08.TYNU,
  b.p07.ADDR_0000_FE00);


  c.p08.CS_A = mux2(b.p04.DMA_A15, b.p08.TOZA, b.p04.DO_DMA); // polarity?
  c.p08.WR_A = nand(b.p08.PUVA, b.p08.MODE_DBG2n1);
  c.p08.WR_D = nor (b.p08.PUVA, b.p07.MODE_DBG2);
  c.p08.RD_A = nand(b.p08.TYMU, b.p08.MODE_DBG2n1);
  c.p08.RD_D = nor (b.p08.TYMU, b.p07.MODE_DBG2);

  c.chip.CS_A = b.p08.CS_A;
  c.chip.WR_A = b.p08.WR_A;
  c.chip.WR_D = b.p08.WR_D;
  c.chip.RD_A = b.p08.RD_A;
  c.chip.RD_D = b.p08.RD_D;
}

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P08_ExtCpuBuses::tick(const Gameboy& /*a*/, const Gameboy& b, Gameboy& c) {

  c.p08.MODE_DBG2n1 = not(b.p07.MODE_DBG2);
  c.p08.MODE_DBG2n2 = not(b.p07.MODE_DBG2);

  //----------
  // center right, generating the external read/write signals to the cart

  c.p08.ADDR_0000_7FFF = not(b.A15);
  c.p08.ADDR_NOT_VRAM = or(b.A13, b.A14, b.p08.ADDR_0000_7FFF);

  // address valid and not vram
  c.p08.TEXO = and(b.cpu.ADDR_VALID, b.p08.ADDR_NOT_VRAM);

  // address_not_valid or addr_vram
  c.p08.LEVO = not(b.p08.TEXO);

  //c.p08.LAGU = unk3(b.cpu.CPU_RAW_RD, b.p08.LEVO, b.cpu.CPU_RAW_WR);

  c.p08.LAGU = or(and(b.cpu.CPU_RAW_RD, b.p08.LEVO), b.cpu.CPU_RAW_WR);

  c.p08.LYWE = not(b.p08.LAGU);

  c.p08.MOCA = nor(b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.MEXO = not(b.p01.CPU_WR_SYNC);
  c.p08.NEVY = or(b.p08.MEXO, b.p08.MOCA);
  c.p08.MOTY = or(b.p08.MOCA, b.p08.LYWE);
  c.p08.PUVA = or(b.p08.NEVY, b.p04.DO_DMA);
  c.p08.TYMU = nor(b.p04.DO_DMA, b.p08.MOTY);

  //----------
  // top center

  c.p08.MODE_DBG1n = not(b.p07.MODE_DBG1);
  c.p08.A1n = not(b.A01);
  c.p08.LOXO = unk3(b.p08.MODE_DBG1n, b.p08.TEXO, b.p07.MODE_DBG1);
  c.p08.LASY = not(b.p08.LOXO);
  c.p08.MATE = not(b.p08.LASY);

  //----------
  // left center, CS and A15 drivers

  c.p08.SOGY = not(b.A14);
  c.p08.CART_RAM = and(b.A13, b.p08.SOGY, b.A15); // selects A000-BFFF, cart ram
  
  
  // this is a guess, it selects addr < 0xC000 || cart_ram, which seems reasonable
  //c.p08.TYNU = unk3(b.A15, b.A14, b.p08.CART_RAM);
  c.p08.TYNU = nor(nand(b.A15, b.A14), b.p08.CART_RAM);
  
  c.p08.TOZA = and(b.p01.CPU_RD_SYNC, 
                   b.p08.TYNU,
                   b.p07.ADDR_0000_FE00);


  c.p08.CS_A = mux2(b.p04.DMA_A15, b.p08.TOZA, b.p04.DO_DMA); // polarity?
  c.p08.WR_A = nand(b.p08.PUVA, b.p08.MODE_DBG2n1);
  c.p08.WR_D = nor (b.p08.PUVA, b.p07.MODE_DBG2);
  c.p08.RD_A = nand(b.p08.TYMU, b.p08.MODE_DBG2n1);
  c.p08.RD_D = nor (b.p08.TYMU, b.p07.MODE_DBG2);

  c.chip.CS_A = b.p08.CS_A;
  c.chip.WR_A = b.p08.WR_A;
  c.chip.WR_D = b.p08.WR_D;
  c.chip.RD_A = b.p08.RD_A;
  c.chip.RD_D = b.p08.RD_D;

  //----------
  // AXX_A/D

  c.p08.SOBY_15 = nor(b.A15, b.p07.READ_BOOTROM);

  c.p08.ADDR_LATCH_00 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_00, b.A00);
  c.p08.ADDR_LATCH_01 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_01, b.A01);
  c.p08.ADDR_LATCH_02 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_02, b.A02);
  c.p08.ADDR_LATCH_03 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_03, b.A03);
  c.p08.ADDR_LATCH_04 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_04, b.A04);
  c.p08.ADDR_LATCH_05 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_05, b.A05);
  c.p08.ADDR_LATCH_06 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_06, b.A06);
  c.p08.ADDR_LATCH_07 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_07, b.A07);
  c.p08.ADDR_LATCH_08 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_08, b.A08);
  c.p08.ADDR_LATCH_09 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_09, b.A09);
  c.p08.ADDR_LATCH_10 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_10, b.A10);
  c.p08.ADDR_LATCH_11 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_11, b.A11);
  c.p08.ADDR_LATCH_12 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_12, b.A12);
  c.p08.ADDR_LATCH_13 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_13, b.A13);
  c.p08.ADDR_LATCH_14 = latch_pos(b.p08.MATE, b.p08.ADDR_LATCH_14, b.A14);
  c.p08.ADDR_LATCH_15 = nand(b.p01.CPU_RD_SYNC, b.p08.SOBY_15);

  c.p08.ADDR_MUX_00 = mux2(b.p04.DMA_A00, b.p08.ADDR_LATCH_00, b.p04.DO_DMA);
  c.p08.ADDR_MUX_01 = mux2(b.p04.DMA_A01, b.p08.ADDR_LATCH_01, b.p04.DO_DMA);
  c.p08.ADDR_MUX_02 = mux2(b.p04.DMA_A02, b.p08.ADDR_LATCH_02, b.p04.DO_DMA);
  c.p08.ADDR_MUX_03 = mux2(b.p04.DMA_A03, b.p08.ADDR_LATCH_03, b.p04.DO_DMA);
  c.p08.ADDR_MUX_04 = mux2(b.p04.DMA_A04, b.p08.ADDR_LATCH_04, b.p04.DO_DMA);
  c.p08.ADDR_MUX_05 = mux2(b.p04.DMA_A05, b.p08.ADDR_LATCH_05, b.p04.DO_DMA);
  c.p08.ADDR_MUX_06 = mux2(b.p04.DMA_A06, b.p08.ADDR_LATCH_06, b.p04.DO_DMA);
  c.p08.ADDR_MUX_07 = mux2(b.p04.DMA_A07, b.p08.ADDR_LATCH_07, b.p04.DO_DMA);
  c.p08.ADDR_MUX_08 = mux2(b.p04.DMA_A08, b.p08.ADDR_LATCH_08, b.p04.DO_DMA);
  c.p08.ADDR_MUX_09 = mux2(b.p04.DMA_A09, b.p08.ADDR_LATCH_09, b.p04.DO_DMA);
  c.p08.ADDR_MUX_10 = mux2(b.p04.DMA_A10, b.p08.ADDR_LATCH_10, b.p04.DO_DMA);
  c.p08.ADDR_MUX_11 = mux2(b.p04.DMA_A11, b.p08.ADDR_LATCH_11, b.p04.DO_DMA);
  c.p08.ADDR_MUX_12 = mux2(b.p04.DMA_A12, b.p08.ADDR_LATCH_12, b.p04.DO_DMA);
  c.p08.ADDR_MUX_13 = mux2(b.p04.DMA_A13, b.p08.ADDR_LATCH_13, b.p04.DO_DMA);
  c.p08.ADDR_MUX_14 = mux2(b.p04.DMA_A14, b.p08.ADDR_LATCH_14, b.p04.DO_DMA);
  c.p08.ADDR_MUX_15 = mux2(b.p04.DMA_A15, b.p08.ADDR_LATCH_15, b.p04.DO_DMA);

  c.p08.PIN_A00_A = nand(b.p08.ADDR_MUX_00, b.p08.MODE_DBG2n1);
  c.p08.PIN_A01_A = nand(b.p08.ADDR_MUX_01, b.p08.MODE_DBG2n1);
  c.p08.PIN_A02_A = nand(b.p08.ADDR_MUX_02, b.p08.MODE_DBG2n1);
  c.p08.PIN_A03_A = nand(b.p08.ADDR_MUX_03, b.p08.MODE_DBG2n1);
  c.p08.PIN_A04_A = nand(b.p08.ADDR_MUX_04, b.p08.MODE_DBG2n1);
  c.p08.PIN_A05_A = nand(b.p08.ADDR_MUX_05, b.p08.MODE_DBG2n1);
  c.p08.PIN_A06_A = nand(b.p08.ADDR_MUX_06, b.p08.MODE_DBG2n1);
  c.p08.PIN_A07_A = nand(b.p08.ADDR_MUX_07, b.p08.MODE_DBG2n1);
  c.p08.PIN_A08_A = nand(b.p08.ADDR_MUX_08, b.p08.MODE_DBG2n1);
  c.p08.PIN_A09_A = nand(b.p08.ADDR_MUX_09, b.p08.MODE_DBG2n1);
  c.p08.PIN_A10_A = nand(b.p08.ADDR_MUX_10, b.p08.MODE_DBG2n1);
  c.p08.PIN_A11_A = nand(b.p08.ADDR_MUX_11, b.p08.MODE_DBG2n1);
  c.p08.PIN_A12_A = nand(b.p08.ADDR_MUX_12, b.p08.MODE_DBG2n1);
  c.p08.PIN_A13_A = nand(b.p08.ADDR_MUX_13, b.p08.MODE_DBG2n1);
  c.p08.PIN_A14_A = nand(b.p08.ADDR_MUX_14, b.p08.MODE_DBG2n1);
  c.p08.PIN_A15_A = nand(b.p08.ADDR_MUX_15, b.p08.MODE_DBG2n2);

  c.p08.PIN_A00_D = nor(b.p08.ADDR_MUX_00, b.p07.MODE_DBG2);
  c.p08.PIN_A01_D = nor(b.p08.ADDR_MUX_01, b.p07.MODE_DBG2);
  c.p08.PIN_A02_D = nor(b.p08.ADDR_MUX_02, b.p07.MODE_DBG2);
  c.p08.PIN_A03_D = nor(b.p08.ADDR_MUX_03, b.p07.MODE_DBG2);
  c.p08.PIN_A04_D = nor(b.p08.ADDR_MUX_04, b.p07.MODE_DBG2);
  c.p08.PIN_A05_D = nor(b.p08.ADDR_MUX_05, b.p07.MODE_DBG2);
  c.p08.PIN_A06_D = nor(b.p08.ADDR_MUX_06, b.p07.MODE_DBG2);
  c.p08.PIN_A07_D = nor(b.p08.ADDR_MUX_07, b.p07.MODE_DBG2);
  c.p08.PIN_A08_D = nor(b.p08.ADDR_MUX_08, b.p07.MODE_DBG2);
  c.p08.PIN_A09_D = nor(b.p08.ADDR_MUX_09, b.p07.MODE_DBG2);
  c.p08.PIN_A10_D = nor(b.p08.ADDR_MUX_10, b.p07.MODE_DBG2);
  c.p08.PIN_A11_D = nor(b.p08.ADDR_MUX_11, b.p07.MODE_DBG2);
  c.p08.PIN_A12_D = nor(b.p08.ADDR_MUX_12, b.p07.MODE_DBG2);
  c.p08.PIN_A13_D = nor(b.p08.ADDR_MUX_13, b.p07.MODE_DBG2);
  c.p08.PIN_A14_D = nor(b.p08.ADDR_MUX_14, b.p07.MODE_DBG2);
  c.p08.PIN_A15_D = nor(b.p08.ADDR_MUX_15, b.p07.MODE_DBG2);

  c.chip.A00_A = b.p08.PIN_A00_A;
  c.chip.A01_A = b.p08.PIN_A01_A;
  c.chip.A02_A = b.p08.PIN_A02_A;
  c.chip.A03_A = b.p08.PIN_A03_A;
  c.chip.A04_A = b.p08.PIN_A04_A;
  c.chip.A05_A = b.p08.PIN_A05_A;
  c.chip.A06_A = b.p08.PIN_A06_A;
  c.chip.A07_A = b.p08.PIN_A07_A;
  c.chip.A08_A = b.p08.PIN_A08_A;
  c.chip.A09_A = b.p08.PIN_A09_A;
  c.chip.A10_A = b.p08.PIN_A10_A;
  c.chip.A11_A = b.p08.PIN_A11_A;
  c.chip.A12_A = b.p08.PIN_A12_A;
  c.chip.A13_A = b.p08.PIN_A13_A;
  c.chip.A14_A = b.p08.PIN_A14_A;
  c.chip.A15_A = b.p08.PIN_A15_A;

  c.chip.A00_D = b.p08.PIN_A00_D;
  c.chip.A01_D = b.p08.PIN_A01_D;
  c.chip.A02_D = b.p08.PIN_A02_D;
  c.chip.A03_D = b.p08.PIN_A03_D;
  c.chip.A04_D = b.p08.PIN_A04_D;
  c.chip.A05_D = b.p08.PIN_A05_D;
  c.chip.A06_D = b.p08.PIN_A06_D;
  c.chip.A07_D = b.p08.PIN_A07_D;
  c.chip.A08_D = b.p08.PIN_A08_D;
  c.chip.A09_D = b.p08.PIN_A09_D;
  c.chip.A10_D = b.p08.PIN_A10_D;
  c.chip.A11_D = b.p08.PIN_A11_D;
  c.chip.A12_D = b.p08.PIN_A12_D;
  c.chip.A13_D = b.p08.PIN_A13_D;
  c.chip.A14_D = b.p08.PIN_A14_D;
  c.chip.A15_D = b.p08.PIN_A15_D;

  //----------
  // Chip data pin output driver.

  /*
  if (DBUS_OUT) {
    D0_B = 1;
    D0_A = not(D0);
    D0_D = not(D0);    
  }
  else {
    D0_B = 0;
    D0_A = 1;
    D0_D = 0;
  }
  */

  c.p08.DBUS_OUTn = mux2(b.p08.CPU_RDn, b.p08.MOTY, b.p07.MODE_DBG2);
  c.p08.DBUS_OUT  = not(b.p08.DBUS_OUTn);

  c.chip.D0_B = b.p08.DBUS_OUT;
  c.chip.D1_B = b.p08.DBUS_OUT;
  c.chip.D2_B = b.p08.DBUS_OUT;
  c.chip.D3_B = b.p08.DBUS_OUT;
  c.chip.D4_B = b.p08.DBUS_OUT;
  c.chip.D5_B = b.p08.DBUS_OUT;
  c.chip.D6_B = b.p08.DBUS_OUT;
  c.chip.D7_B = b.p08.DBUS_OUT;

  // are we driving the inverted byte onto the bus? looks like it...

  c.p08.D0_A  = nand(b.p08.DBUS_OUT,  b.D0);
  c.p08.D1_A  = nand(b.p08.DBUS_OUT,  b.D1);
  c.p08.D2_A  = nand(b.p08.DBUS_OUT,  b.D2);
  c.p08.D3_A  = nand(b.p08.DBUS_OUT,  b.D3);
  c.p08.D4_A  = nand(b.p08.DBUS_OUT,  b.D4);
  c.p08.D5_A  = nand(b.p08.DBUS_OUT,  b.D5);
  c.p08.D6_A  = nand(b.p08.DBUS_OUT,  b.D6);
  c.p08.D7_A  = nand(b.p08.DBUS_OUT,  b.D7);
              
  c.p08.D0_D  = nor (b.p08.DBUS_OUTn, b.D0);
  c.p08.D1_D  = nor (b.p08.DBUS_OUTn, b.D1);
  c.p08.D2_D  = nor (b.p08.DBUS_OUTn, b.D2);
  c.p08.D3_D  = nor (b.p08.DBUS_OUTn, b.D3);
  c.p08.D4_D  = nor (b.p08.DBUS_OUTn, b.D4);
  c.p08.D5_D  = nor (b.p08.DBUS_OUTn, b.D5);
  c.p08.D6_D  = nor (b.p08.DBUS_OUTn, b.D6);
  c.p08.D7_D  = nor (b.p08.DBUS_OUTn, b.D7);

  c.chip.D0_A = b.p08.D0_A;
  c.chip.D1_A = b.p08.D1_A;
  c.chip.D2_A = b.p08.D2_A;
  c.chip.D3_A = b.p08.D3_A;
  c.chip.D4_A = b.p08.D4_A;
  c.chip.D5_A = b.p08.D5_A;
  c.chip.D6_A = b.p08.D6_A;
  c.chip.D7_A = b.p08.D7_A;

  c.chip.D0_D = b.p08.D0_D;
  c.chip.D1_D = b.p08.D1_D;
  c.chip.D2_D = b.p08.D2_D;
  c.chip.D3_D = b.p08.D3_D;
  c.chip.D4_D = b.p08.D4_D;
  c.chip.D5_D = b.p08.D5_D;
  c.chip.D6_D = b.p08.D6_D;
  c.chip.D7_D = b.p08.D7_D;

  //----------
  // Not sure why there's two of these

  c.p08.LAVO = nand(b.cpu.CPU_RAW_RD, b.p08.TEXO, b.cpu.FROM_CPU5);

  c.p08.LATCH_D0 = latch_pos(b.p08.LAVO, b.p08.LATCH_D0, b.chip.D0_C);
  c.p08.LATCH_D1 = latch_pos(b.p08.LAVO, b.p08.LATCH_D1, b.chip.D1_C);
  c.p08.LATCH_D2 = latch_pos(b.p08.LAVO, b.p08.LATCH_D2, b.chip.D2_C);
  c.p08.LATCH_D3 = latch_pos(b.p08.LAVO, b.p08.LATCH_D3, b.chip.D3_C);
  c.p08.LATCH_D4 = latch_pos(b.p08.LAVO, b.p08.LATCH_D4, b.chip.D4_C);
  c.p08.LATCH_D5 = latch_pos(b.p08.LAVO, b.p08.LATCH_D5, b.chip.D5_C);
  c.p08.LATCH_D6 = latch_pos(b.p08.LAVO, b.p08.LATCH_D6, b.chip.D6_C);
  c.p08.LATCH_D7 = latch_pos(b.p08.LAVO, b.p08.LATCH_D7, b.chip.D7_C);

  c.p08.BUS_D0 = b.p08.LATCH_D0;
  c.p08.BUS_D1 = b.p08.LATCH_D1;
  c.p08.BUS_D2 = b.p08.LATCH_D2;
  c.p08.BUS_D3 = b.p08.LATCH_D3;
  c.p08.BUS_D4 = b.p08.LATCH_D4;
  c.p08.BUS_D5 = b.p08.LATCH_D5;
  c.p08.BUS_D6 = b.p08.LATCH_D6;
  c.p08.BUS_D7 = b.p08.LATCH_D7;

  if (b.p08.LAVO) {
    c.D0 = b.p08.BUS_D0;
    c.D1 = b.p08.BUS_D1;
    c.D2 = b.p08.BUS_D2;
    c.D3 = b.p08.BUS_D3;
    c.D4 = b.p08.BUS_D4;
    c.D5 = b.p08.BUS_D5;
    c.D6 = b.p08.BUS_D6;
    c.D7 = b.p08.BUS_D7;
  }

  //----------

  c.p08.D0_Cn = not(b.chip.D0_C);
  c.p08.D1_Cn = not(b.chip.D1_C);
  c.p08.D2_Cn = not(b.chip.D2_C);
  c.p08.D3_Cn = not(b.chip.D3_C);
  c.p08.D4_Cn = not(b.chip.D4_C);
  c.p08.D5_Cn = not(b.chip.D5_C);
  c.p08.D6_Cn = not(b.chip.D6_C);
  c.p08.D7_Cn = not(b.chip.D7_C);

  c.p08.D0_C  = not(b.p08.D0_Cn);
  c.p08.D1_C  = not(b.p08.D1_Cn);
  c.p08.D2_C  = not(b.p08.D2_Cn);
  c.p08.D3_C  = not(b.p08.D3_Cn);
  c.p08.D4_C  = not(b.p08.D4_Cn);
  c.p08.D5_C  = not(b.p08.D5_Cn);
  c.p08.D6_C  = not(b.p08.D6_Cn);
  c.p08.D7_C  = not(b.p08.D7_Cn);

  c.p08.LYRA = nand(b.p07.MODE_DBG2, b.p08.DBUS_OUTn);

  if (b.p08.LYRA) {
    c.D0 = b.p08.D0_C;
    c.D1 = b.p08.D1_C;
    c.D2 = b.p08.D2_C;
    c.D3 = b.p08.D3_C;
    c.D4 = b.p08.D4_C;
    c.D5 = b.p08.D5_C;
    c.D6 = b.p08.D6_C;
    c.D7 = b.p08.D7_C;
  }

#if 0
  //----------
  // if NET01 high, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  c.p08.A00_Cn = not(b.chip.A00_C);
  c.p08.A01_Cn = not(b.chip.A01_C);
  c.p08.A02_Cn = not(b.chip.A02_C);
  c.p08.A03_Cn = not(b.chip.A03_C);
  c.p08.A04_Cn = not(b.chip.A04_C);
  c.p08.A05_Cn = not(b.chip.A05_C);
  c.p08.A06_Cn = not(b.chip.A06_C);
  c.p08.A07_Cn = not(b.chip.A07_C);
  c.p08.A08_Cn = not(b.chip.A08_C);
  c.p08.A09_Cn = not(b.chip.A09_C);
  c.p08.A10_Cn = not(b.chip.A10_C);
  c.p08.A11_Cn = not(b.chip.A11_C);
  c.p08.A12_Cn = not(b.chip.A12_C);
  c.p08.A13_Cn = not(b.chip.A13_C);
  c.p08.A14_Cn = not(b.chip.A14_C);
  c.p08.A15_Cn = not(b.chip.A15_C);

  c.p08.A00_C = not(b.p08.A00_Cn); 
  c.p08.A01_C = not(b.p08.A01_Cn); 
  c.p08.A02_C = not(b.p08.A02_Cn); 
  c.p08.A03_C = not(b.p08.A03_Cn); 
  c.p08.A04_C = not(b.p08.A04_Cn); 
  c.p08.A05_C = not(b.p08.A05_Cn); 
  c.p08.A06_C = not(b.p08.A06_Cn); 
  c.p08.A07_C = not(b.p08.A07_Cn); 
  c.p08.A08_C = not(b.p08.A08_Cn); 
  c.p08.A09_C = not(b.p08.A09_Cn); 
  c.p08.A10_C = not(b.p08.A10_Cn); 
  c.p08.A11_C = not(b.p08.A11_Cn); 
  c.p08.A12_C = not(b.p08.A12_Cn); 
  c.p08.A13_C = not(b.p08.A13_Cn); 
  c.p08.A14_C = not(b.p08.A14_Cn); 
  c.p08.A15_C = not(b.p08.A15_Cn);


  if (b.NET01) {
    c.cpu.A00 = b.p08.A00_C;
    c.cpu.A01 = b.p08.A01_C;
    c.cpu.A02 = b.p08.A02_C;
    c.cpu.A03 = b.p08.A03_C;
    c.cpu.A04 = b.p08.A04_C;
    c.cpu.A05 = b.p08.A05_C;
    c.cpu.A06 = b.p08.A06_C;
    c.cpu.A07 = b.p08.A07_C;
    c.cpu.A08 = b.p08.A08_C;
    c.cpu.A09 = b.p08.A09_C;
    c.cpu.A10 = b.p08.A10_C;
    c.cpu.A11 = b.p08.A11_C;
    c.cpu.A12 = b.p08.A12_C;
    c.cpu.A13 = b.p08.A13_C;
    c.cpu.A14 = b.p08.A14_C;
  }                   

  if (b.p08.MODE_DBG2n2) {
    c.cpu.A15 = b.p08.A15_C;
  }
#endif
}
