#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Pins_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  /*p25.RURA*/ c.chip_out.MD0_D = not(/*p25.SYNU*/ or(b.vid.D_TO_MD, b.MD0));
  /*p25.RULY*/ c.chip_out.MD1_D = not(/*p25.SYMA*/ or(b.vid.D_TO_MD, b.MD1));
  /*p25.RARE*/ c.chip_out.MD2_D = not(/*p25.ROKO*/ or(b.vid.D_TO_MD, b.MD2));
  /*p25.RODU*/ c.chip_out.MD3_D = not(/*p25.SYBU*/ or(b.vid.D_TO_MD, b.MD3));
  /*p25.RUBE*/ c.chip_out.MD4_D = not(/*p25.SAKO*/ or(b.vid.D_TO_MD, b.MD4));
  /*p25.RUMU*/ c.chip_out.MD5_D = not(/*p25.SEJY*/ or(b.vid.D_TO_MD, b.MD5));
  /*p25.RYTY*/ c.chip_out.MD6_D = not(/*p25.SEDO*/ or(b.vid.D_TO_MD, b.MD6));
  /*p25.RADY*/ c.chip_out.MD7_D = not(/*p25.SAWU*/ or(b.vid.D_TO_MD, b.MD7));

  //----------

  //----------

  /*p25.ROVE*/ c.vid.D_TO_MDn = not(b.vid.D_TO_MD);

  /*p25.REGE*/ c.chip_out.MD0_A = not(/*p25.SEFA*/ and(b.MD0, b.vid.D_TO_MDn));
  /*p25.RYKY*/ c.chip_out.MD1_A = not(/*p25.SOGO*/ and(b.MD1, b.vid.D_TO_MDn));
  /*p25.RAZO*/ c.chip_out.MD2_A = not(/*p25.SEFU*/ and(b.MD2, b.vid.D_TO_MDn));
  /*p25.RADA*/ c.chip_out.MD3_A = not(/*p25.SUNA*/ and(b.MD3, b.vid.D_TO_MDn));
  /*p25.RYRO*/ c.chip_out.MD4_A = not(/*p25.SUMO*/ and(b.MD4, b.vid.D_TO_MDn));
  /*p25.REVU*/ c.chip_out.MD5_A = not(/*p25.SAZU*/ and(b.MD5, b.vid.D_TO_MDn));
  /*p25.REKU*/ c.chip_out.MD6_A = not(/*p25.SAMO*/ and(b.MD6, b.vid.D_TO_MDn));
  /*p25.RYZE*/ c.chip_out.MD7_A = not(/*p25.SUKE*/ and(b.MD7, b.vid.D_TO_MDn));

  //----------
  // Address pin driver

  /*p08.LOXO*/ c.sys.ADDR_LATCHb = or(and(b.sys.MODE_DBG1o, b.sys.ADDR_VALID_AND_NOT_VRAM), b.sys.MODE_DBG1);
  /*p08.MATE*/ c.sys.ADDR_LATCH  = not(/*p08.LASY*/ not(b.sys.ADDR_LATCHb));

  /*p08.ALOR*/ c.sys.ADDR_LATCH_00 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_00, b.A00);
  /*p08.APUR*/ c.sys.ADDR_LATCH_01 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_01, b.A01);
  /*p08.ALYR*/ c.sys.ADDR_LATCH_02 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_02, b.A02);
  /*p08.ARET*/ c.sys.ADDR_LATCH_03 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_03, b.A03);
  /*p08.AVYS*/ c.sys.ADDR_LATCH_04 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_04, b.A04);
  /*p08.ATEV*/ c.sys.ADDR_LATCH_05 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_05, b.A05);
  /*p08.AROS*/ c.sys.ADDR_LATCH_06 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_06, b.A06);
  /*p08.ARYM*/ c.sys.ADDR_LATCH_07 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_07, b.A07);
  /*p08.LUNO*/ c.sys.ADDR_LATCH_08 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_08, b.A08);
  /*p08.LYSA*/ c.sys.ADDR_LATCH_09 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_09, b.A09);
  /*p08.PATE*/ c.sys.ADDR_LATCH_10 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_10, b.A10);
  /*p08.LUMY*/ c.sys.ADDR_LATCH_11 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_11, b.A11);
  /*p08.LOBU*/ c.sys.ADDR_LATCH_12 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_12, b.A12);
  /*p08.LONU*/ c.sys.ADDR_LATCH_13 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_13, b.A13);
  /*p08.NYRE*/ c.sys.ADDR_LATCH_14 = latch_pos(b.sys.ADDR_LATCH, b.sys.ADDR_LATCH_14, b.A14);

  // something wrong here...
  /*p08.SOBY*/ wire SOBY = nor(b.A15, b.sys.ADDR_BOOT);
  /*p08.SEPY*/ c.sys.ADDR_LATCH_15 = nand(b.sys.ADDR_VALID_ABxxxxxx, SOBY); // wat?

  /*p08.AMET*/ c.sys.ADDR_MUX_00 = mux2(b.sys.DMA_A00, b.sys.ADDR_LATCH_00, b.sys.DO_DMA);
  /*p08.ATOL*/ c.sys.ADDR_MUX_01 = mux2(b.sys.DMA_A01, b.sys.ADDR_LATCH_01, b.sys.DO_DMA);
  /*p08.APOK*/ c.sys.ADDR_MUX_02 = mux2(b.sys.DMA_A02, b.sys.ADDR_LATCH_02, b.sys.DO_DMA);
  /*p08.AMER*/ c.sys.ADDR_MUX_03 = mux2(b.sys.DMA_A03, b.sys.ADDR_LATCH_03, b.sys.DO_DMA);
  /*p08.ATEM*/ c.sys.ADDR_MUX_04 = mux2(b.sys.DMA_A04, b.sys.ADDR_LATCH_04, b.sys.DO_DMA);
  /*p08.ATOV*/ c.sys.ADDR_MUX_05 = mux2(b.sys.DMA_A05, b.sys.ADDR_LATCH_05, b.sys.DO_DMA);
  /*p08.ATYR*/ c.sys.ADDR_MUX_06 = mux2(b.sys.DMA_A06, b.sys.ADDR_LATCH_06, b.sys.DO_DMA);
  /*p08.ASUR*/ c.sys.ADDR_MUX_07 = mux2(b.sys.DMA_A07, b.sys.ADDR_LATCH_07, b.sys.DO_DMA);
  /*p08.MANO*/ c.sys.ADDR_MUX_08 = mux2(b.sys.DMA_A08, b.sys.ADDR_LATCH_08, b.sys.DO_DMA);
  /*p08.MASU*/ c.sys.ADDR_MUX_09 = mux2(b.sys.DMA_A09, b.sys.ADDR_LATCH_09, b.sys.DO_DMA);
  /*p08.PAMY*/ c.sys.ADDR_MUX_10 = mux2(b.sys.DMA_A10, b.sys.ADDR_LATCH_10, b.sys.DO_DMA);
  /*p08.MALE*/ c.sys.ADDR_MUX_11 = mux2(b.sys.DMA_A11, b.sys.ADDR_LATCH_11, b.sys.DO_DMA);
  /*p08.MOJY*/ c.sys.ADDR_MUX_12 = mux2(b.sys.DMA_A12, b.sys.ADDR_LATCH_12, b.sys.DO_DMA);
  /*p08.MUCE*/ c.sys.ADDR_MUX_13 = mux2(b.sys.DMA_A13, b.sys.ADDR_LATCH_13, b.sys.DO_DMA);
  /*p08.PEGE*/ c.sys.ADDR_MUX_14 = mux2(b.sys.DMA_A14, b.sys.ADDR_LATCH_14, b.sys.DO_DMA);
  /*p08.TAZY*/ c.sys.ADDR_MUX_15 = mux2(b.sys.DMA_A15, b.sys.ADDR_LATCH_15, b.sys.DO_DMA);

  /*p08.KUPO*/ c.chip_out.A00_A = nand(b.sys.ADDR_MUX_00, b.sys.MODE_DBG2n1);
  /*p08.CABA*/ c.chip_out.A01_A = nand(b.sys.ADDR_MUX_01, b.sys.MODE_DBG2n1);
  /*p08.BOKU*/ c.chip_out.A02_A = nand(b.sys.ADDR_MUX_02, b.sys.MODE_DBG2n1);
  /*p08.BOTY*/ c.chip_out.A03_A = nand(b.sys.ADDR_MUX_03, b.sys.MODE_DBG2n1);
  /*p08.BYLA*/ c.chip_out.A04_A = nand(b.sys.ADDR_MUX_04, b.sys.MODE_DBG2n1);
  /*p08.BADU*/ c.chip_out.A05_A = nand(b.sys.ADDR_MUX_05, b.sys.MODE_DBG2n1);
  /*p08.CEPU*/ c.chip_out.A06_A = nand(b.sys.ADDR_MUX_06, b.sys.MODE_DBG2n1);
  /*p08.DEFY*/ c.chip_out.A07_A = nand(b.sys.ADDR_MUX_07, b.sys.MODE_DBG2n1);
  /*p08.MYNY*/ c.chip_out.A08_A = nand(b.sys.ADDR_MUX_08, b.sys.MODE_DBG2n1);
  /*p08.MUNE*/ c.chip_out.A09_A = nand(b.sys.ADDR_MUX_09, b.sys.MODE_DBG2n1);
  /*p08.ROXU*/ c.chip_out.A10_A = nand(b.sys.ADDR_MUX_10, b.sys.MODE_DBG2n1);
  /*p08.LEPY*/ c.chip_out.A11_A = nand(b.sys.ADDR_MUX_11, b.sys.MODE_DBG2n1);
  /*p08.LUCE*/ c.chip_out.A12_A = nand(b.sys.ADDR_MUX_12, b.sys.MODE_DBG2n1);
  /*p08.LABE*/ c.chip_out.A13_A = nand(b.sys.ADDR_MUX_13, b.sys.MODE_DBG2n1);
  /*p08.PUHE*/ c.chip_out.A14_A = nand(b.sys.ADDR_MUX_14, b.sys.MODE_DBG2n1);
  /*p08.SUZE*/ c.chip_out.A15_A = nand(b.sys.ADDR_MUX_15, b.sys.MODE_DBG2n2);

  /*p08.KOTY*/ c.chip_out.A00_D = nor(b.sys.ADDR_MUX_00, b.sys.MODE_DBG2);
  /*p08.COTU*/ c.chip_out.A01_D = nor(b.sys.ADDR_MUX_01, b.sys.MODE_DBG2);
  /*p08.BAJO*/ c.chip_out.A02_D = nor(b.sys.ADDR_MUX_02, b.sys.MODE_DBG2);
  /*p08.BOLA*/ c.chip_out.A03_D = nor(b.sys.ADDR_MUX_03, b.sys.MODE_DBG2);
  /*p08.BEVO*/ c.chip_out.A04_D = nor(b.sys.ADDR_MUX_04, b.sys.MODE_DBG2);
  /*p08.AJAV*/ c.chip_out.A05_D = nor(b.sys.ADDR_MUX_05, b.sys.MODE_DBG2);
  /*p08.CYKA*/ c.chip_out.A06_D = nor(b.sys.ADDR_MUX_06, b.sys.MODE_DBG2);
  /*p08.COLO*/ c.chip_out.A07_D = nor(b.sys.ADDR_MUX_07, b.sys.MODE_DBG2);
  /*p08.MEGO*/ c.chip_out.A08_D = nor(b.sys.ADDR_MUX_08, b.sys.MODE_DBG2);
  /*p08.MENY*/ c.chip_out.A09_D = nor(b.sys.ADDR_MUX_09, b.sys.MODE_DBG2);
  /*p08.RORE*/ c.chip_out.A10_D = nor(b.sys.ADDR_MUX_10, b.sys.MODE_DBG2);
  /*p08.LYNY*/ c.chip_out.A11_D = nor(b.sys.ADDR_MUX_11, b.sys.MODE_DBG2);
  /*p08.LOSO*/ c.chip_out.A12_D = nor(b.sys.ADDR_MUX_12, b.sys.MODE_DBG2);
  /*p08.LEVA*/ c.chip_out.A13_D = nor(b.sys.ADDR_MUX_13, b.sys.MODE_DBG2);
  /*p08.PAHY*/ c.chip_out.A14_D = nor(b.sys.ADDR_MUX_14, b.sys.MODE_DBG2);
  /*p08.RULO*/ c.chip_out.A15_D = nor(b.sys.ADDR_MUX_15, b.sys.MODE_DBG2);

  //----------
  // Data pin driver.

  /*p08.LULA*/ c.chip_out.D0_B = b.sys.DBUS_OUT;
  /*p08.LULA*/ c.chip_out.D1_B = b.sys.DBUS_OUT;
  /*p08.LULA*/ c.chip_out.D2_B = b.sys.DBUS_OUT;
  /*p08.LULA*/ c.chip_out.D3_B = b.sys.DBUS_OUT;
  /*p08.LULA*/ c.chip_out.D4_B = b.sys.DBUS_OUT;
  /*p08.LULA*/ c.chip_out.D5_B = b.sys.DBUS_OUT;
  /*p08.LULA*/ c.chip_out.D6_B = b.sys.DBUS_OUT;
  /*p08.LULA*/ c.chip_out.D7_B = b.sys.DBUS_OUT;

  /*p25.RUXA*/ c.chip_out.D0_A = nand(b.D0, b.sys.DBUS_OUT);
  /*p25.RUJA*/ c.chip_out.D1_A = nand(b.D1, b.sys.DBUS_OUT);
  /*p25.RABY*/ c.chip_out.D2_A = nand(b.D2, b.sys.DBUS_OUT);
  /*p25.RERA*/ c.chip_out.D3_A = nand(b.D3, b.sys.DBUS_OUT);
  /*p25.RORY*/ c.chip_out.D4_A = nand(b.D4, b.sys.DBUS_OUT);
  /*p25.RYVO*/ c.chip_out.D5_A = nand(b.D5, b.sys.DBUS_OUT);
  /*p25.RAFY*/ c.chip_out.D7_A = nand(b.D6, b.sys.DBUS_OUT);
  /*p25.RAVU*/ c.chip_out.D6_A = nand(b.D7, b.sys.DBUS_OUT);

  /*p08.RUNE*/ c.chip_out.D0_D = nor (b.D0, b.sys.DBUS_OUTn);
  /*p08.RYPU*/ c.chip_out.D1_D = nor (b.D1, b.sys.DBUS_OUTn);
  /*p08.SULY*/ c.chip_out.D2_D = nor (b.D2, b.sys.DBUS_OUTn);
  /*p08.SEZE*/ c.chip_out.D3_D = nor (b.D3, b.sys.DBUS_OUTn);
  /*p08.RESY*/ c.chip_out.D4_D = nor (b.D4, b.sys.DBUS_OUTn);
  /*p08.TAMU*/ c.chip_out.D5_D = nor (b.D5, b.sys.DBUS_OUTn);
  /*p08.ROGY*/ c.chip_out.D6_D = nor (b.D6, b.sys.DBUS_OUTn);
  /*p08.RYDA*/ c.chip_out.D7_D = nor (b.D7, b.sys.DBUS_OUTn);

  /*p08.LAVO*/ c.sys.LATCH_DX_C = nand(cpu_in.CPU_RAW_RD, b.sys.ADDR_VALID_AND_NOT_VRAM, cpu_in.FROM_CPU5); // polarity?

  /*p08.SOMA*/ c.sys.LATCH_D0_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D0_C, chip_in.D0_C);
  /*p08.RONY*/ c.sys.LATCH_D1_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D1_C, chip_in.D1_C);
  /*p08.RAXY*/ c.sys.LATCH_D2_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D2_C, chip_in.D2_C);
  /*p08.SELO*/ c.sys.LATCH_D3_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D3_C, chip_in.D3_C);
  /*p08.SODY*/ c.sys.LATCH_D4_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D4_C, chip_in.D4_C);
  /*p08.SAGO*/ c.sys.LATCH_D5_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D5_C, chip_in.D5_C);
  /*p08.RUPA*/ c.sys.LATCH_D6_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D6_C, chip_in.D6_C);
  /*p08.SAZY*/ c.sys.LATCH_D7_C = latch_pos(b.sys.LATCH_DX_C, b.sys.LATCH_D7_C, chip_in.D7_C);

  /*p08.RYMA*/ if (b.sys.LATCH_DX_C) c.D0 = b.sys.LATCH_D0_C;
  /*p08.RUVO*/ if (b.sys.LATCH_DX_C) c.D1 = b.sys.LATCH_D1_C;
  /*p08.RYKO*/ if (b.sys.LATCH_DX_C) c.D2 = b.sys.LATCH_D2_C;
  /*p08.TAVO*/ if (b.sys.LATCH_DX_C) c.D3 = b.sys.LATCH_D3_C;
  /*p08.TEPE*/ if (b.sys.LATCH_DX_C) c.D4 = b.sys.LATCH_D4_C;
  /*p08.SAFO*/ if (b.sys.LATCH_DX_C) c.D5 = b.sys.LATCH_D5_C;
  /*p08.SEVU*/ if (b.sys.LATCH_DX_C) c.D6 = b.sys.LATCH_D6_C;
  /*p08.TAJU*/ if (b.sys.LATCH_DX_C) c.D7 = b.sys.LATCH_D7_C;
}

//-----------------------------------------------------------------------------

};