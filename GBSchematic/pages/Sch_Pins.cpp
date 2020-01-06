#include "Sch_Pins.h"

#include "Sch_DMA.h"
#include "Sch_Debug.h"
#include "Sch_Clocks.h"
#include "Sch_BusControl.h"
#include "Sch_CpuSignals.h"
#include "Sch_Decoder.h"
#include "Sch_Vram.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

VramPins VramPins::tick(const BusSignals& bus_sig,
                        const VramBus& vram_bus,
                        const DebugSignals& dbg_sig,
                        bool MCS_C, bool MOE_C, bool MWR_C,
                        uint8_t MD_C) {


  /*p25.TODE*/ wire MCS_An = and(vram_bus.MCS, dbg_sig.DBG_VRAMn);
  /*p25.SEWO*/ wire MCS_Dn = or (vram_bus.MCS, dbg_sig.DBG_VRAM);
  /*p25.SOKY*/ wire MCS_A = not(MCS_An);
  /*p25.SETY*/ wire MCS_D = not(MCS_Dn);

  /*p25.SEMA*/ wire MOE_An = and(vram_bus.MOEn, dbg_sig.DBG_VRAMn);
  /*p25.RUTE*/ wire MOE_Dn = or (vram_bus.MOEn, dbg_sig.DBG_VRAM); // schematic wrong, second input is RACU
  /*p25.REFO*/ wire MOE_A = not(MOE_An);
  /*p25.SAHA*/ wire MOE_D = not(MOE_Dn);

  /*p25.TAXY*/ wire MWR_An = and(vram_bus.MWR, dbg_sig.DBG_VRAMn);
  /*p25.SOFY*/ wire MWR_Dn = or (vram_bus.MWR, dbg_sig.DBG_VRAM);
  /*p25.SYSY*/ wire MWR_A = not(MWR_An);
  /*p25.RAGU*/ wire MWR_D = not(MWR_Dn);

  /*p25.LEXE*/ wire MA00_D = not(/*p25.MYFU*/ not(vram_bus.MA00));
  /*p25.LOZU*/ wire MA01_D = not(/*p25.MASA*/ not(vram_bus.MA01));
  /*p25.LACA*/ wire MA02_D = not(/*p25.MYRE*/ not(vram_bus.MA02));
  /*p25.LUVO*/ wire MA03_D = not(/*p25.MAVU*/ not(vram_bus.MA03));
  /*p25.LOLY*/ wire MA04_D = not(/*p25.MEPA*/ not(vram_bus.MA04));
  /*p25.LALO*/ wire MA05_D = not(/*p25.MYSA*/ not(vram_bus.MA05));
  /*p25.LEFA*/ wire MA06_D = not(/*p25.MEWY*/ not(vram_bus.MA06));
  /*p25.LUBY*/ wire MA07_D = not(/*p25.MUME*/ not(vram_bus.MA07));
  /*p25.TUJY*/ wire MA08_D = not(/*p25.VOVA*/ not(vram_bus.MA08));
  /*p25.TAGO*/ wire MA09_D = not(/*p25.VODE*/ not(vram_bus.MA09));
  /*p25.NUVA*/ wire MA10_D = not(/*p25.RUKY*/ not(vram_bus.MA10));
  /*p25.PEDU*/ wire MA11_D = not(/*p25.RUMA*/ not(vram_bus.MA11));
  /*p25.PONY*/ wire MA12_D = not(/*p25.REHO*/ not(vram_bus.MA12));

  /*p25.ROVE*/ wire CBUS_TO_VBUS = not(bus_sig.CBUS_TO_VBUSn);
  /*p25.REGE*/ wire MD0_A = not(/*p25.SEFA*/ and(vram_bus.MD0, CBUS_TO_VBUS));
  /*p25.RYKY*/ wire MD1_A = not(/*p25.SOGO*/ and(vram_bus.MD1, CBUS_TO_VBUS));
  /*p25.RAZO*/ wire MD2_A = not(/*p25.SEFU*/ and(vram_bus.MD2, CBUS_TO_VBUS));
  /*p25.RADA*/ wire MD3_A = not(/*p25.SUNA*/ and(vram_bus.MD3, CBUS_TO_VBUS));
  /*p25.RYRO*/ wire MD4_A = not(/*p25.SUMO*/ and(vram_bus.MD4, CBUS_TO_VBUS));
  /*p25.REVU*/ wire MD5_A = not(/*p25.SAZU*/ and(vram_bus.MD5, CBUS_TO_VBUS));
  /*p25.REKU*/ wire MD6_A = not(/*p25.SAMO*/ and(vram_bus.MD6, CBUS_TO_VBUS));
  /*p25.RYZE*/ wire MD7_A = not(/*p25.SUKE*/ and(vram_bus.MD7, CBUS_TO_VBUS));

  /*p25.ROFA*/ wire MD0_B = not(bus_sig.VEXT_TO_VBUSn);
  /*p25.ROFA*/ wire MD1_B = not(bus_sig.VEXT_TO_VBUSn);
  /*p25.ROFA*/ wire MD2_B = not(bus_sig.VEXT_TO_VBUSn);
  /*p25.ROFA*/ wire MD3_B = not(bus_sig.VEXT_TO_VBUSn);
  /*p25.ROFA*/ wire MD4_B = not(bus_sig.VEXT_TO_VBUSn);
  /*p25.ROFA*/ wire MD5_B = not(bus_sig.VEXT_TO_VBUSn);
  /*p25.ROFA*/ wire MD6_B = not(bus_sig.VEXT_TO_VBUSn);
  /*p25.ROFA*/ wire MD7_B = not(bus_sig.VEXT_TO_VBUSn);

  /*p25.RURA*/ wire MD0_D = not(/*p25.SYNU*/ or(vram_bus.MD0, bus_sig.CBUS_TO_VBUSn));
  /*p25.RULY*/ wire MD1_D = not(/*p25.SYMA*/ or(vram_bus.MD1, bus_sig.CBUS_TO_VBUSn));
  /*p25.RARE*/ wire MD2_D = not(/*p25.ROKO*/ or(vram_bus.MD2, bus_sig.CBUS_TO_VBUSn));
  /*p25.RODU*/ wire MD3_D = not(/*p25.SYBU*/ or(vram_bus.MD3, bus_sig.CBUS_TO_VBUSn));
  /*p25.RUBE*/ wire MD4_D = not(/*p25.SAKO*/ or(vram_bus.MD4, bus_sig.CBUS_TO_VBUSn));
  /*p25.RUMU*/ wire MD5_D = not(/*p25.SEJY*/ or(vram_bus.MD5, bus_sig.CBUS_TO_VBUSn));
  /*p25.RYTY*/ wire MD6_D = not(/*p25.SEDO*/ or(vram_bus.MD6, bus_sig.CBUS_TO_VBUSn));
  /*p25.RADY*/ wire MD7_D = not(/*p25.SAWU*/ or(vram_bus.MD7, bus_sig.CBUS_TO_VBUSn));

  return {
    /* PIN_43 */ .MCS_A  = MCS_A ,
    /* PIN_43 */ .MCS_C  = MCS_C ,
    /* PIN_43 */ .MCS_D  = MCS_D ,
    /* PIN_45 */ .MOE_A  = MOE_A ,
    /* PIN_45 */ .MOE_C  = MOE_C ,
    /* PIN_45 */ .MOE_D  = MOE_D ,
    /* PIN_49 */ .MWR_A  = MWR_A ,
    /* PIN_49 */ .MWR_C  = MWR_C ,
    /* PIN_49 */ .MWR_D  = MWR_D ,
    /* PIN_34 */ .MA00_D = MA00_D,
    /* PIN_35 */ .MA01_D = MA01_D,
    /* PIN_36 */ .MA02_D = MA02_D,
    /* PIN_37 */ .MA03_D = MA03_D,
    /* PIN_38 */ .MA04_D = MA04_D,
    /* PIN_39 */ .MA05_D = MA05_D,
    /* PIN_40 */ .MA06_D = MA06_D,
    /* PIN_41 */ .MA07_D = MA07_D,
    /* PIN_42 */ .MA12_D = MA12_D,
    /* PIN_44 */ .MA10_D = MA10_D,
    /* PIN_46 */ .MA11_D = MA11_D,
    /* PIN_47 */ .MA09_D = MA09_D,
    /* PIN_48 */ .MA08_D = MA08_D,
    /* PIN_33 */ .MD0_A  = MD0_A ,
    /* PIN_33 */ .MD0_B  = MD0_B ,
    /* PIN_33 */ .MD0_C  = bool(MD_C & 0x01),
    /* PIN_33 */ .MD0_D  = MD0_D ,
    /* PIN_31 */ .MD1_A  = MD1_A ,
    /* PIN_31 */ .MD1_B  = MD1_B ,
    /* PIN_31 */ .MD1_C  = bool(MD_C & 0x02),
    /* PIN_31 */ .MD1_D  = MD1_D ,
    /* PIN_30 */ .MD2_A  = MD2_A ,
    /* PIN_30 */ .MD2_B  = MD2_B ,
    /* PIN_30 */ .MD2_C  = bool(MD_C & 0x04),
    /* PIN_30 */ .MD2_D  = MD2_D ,
    /* PIN_29 */ .MD3_A  = MD3_A ,
    /* PIN_29 */ .MD3_B  = MD3_B ,
    /* PIN_29 */ .MD3_C  = bool(MD_C & 0x08),
    /* PIN_29 */ .MD3_D  = MD3_D ,
    /* PIN_28 */ .MD4_A  = MD4_A ,
    /* PIN_28 */ .MD4_B  = MD4_B ,
    /* PIN_28 */ .MD4_C  = bool(MD_C & 0x10),
    /* PIN_28 */ .MD4_D  = MD4_D ,
    /* PIN_27 */ .MD5_A  = MD5_A ,
    /* PIN_27 */ .MD5_B  = MD5_B ,
    /* PIN_27 */ .MD5_C  = bool(MD_C & 0x20),
    /* PIN_27 */ .MD5_D  = MD5_D ,
    /* PIN_26 */ .MD6_A  = MD6_A ,
    /* PIN_26 */ .MD6_B  = MD6_B ,
    /* PIN_26 */ .MD6_C  = bool(MD_C & 0x40),
    /* PIN_26 */ .MD6_D  = MD6_D ,
    /* PIN_25 */ .MD7_A  = MD7_A ,
    /* PIN_25 */ .MD7_B  = MD7_B ,
    /* PIN_25 */ .MD7_C  = bool(MD_C & 0x80),
    /* PIN_25 */ .MD7_D  = MD7_D ,
  };
}

//-----------------------------------------------------------------------------

void CartPins::tick(const SysSignals& sys_sig,
                    const BusSignals& bus_sig,
                    const DebugSignals& dbg_sig,
                    const ClkSignals& clk_sig,
                    const DecoderSignals& dec_sig,
                    const DmaSignals& dma_sig,
                    BusTristates& bus_tri,
                    CartPins& cart_pins) {


  /*p08.SOGY*/ wire SOGY = not(bus_tri.A14());
  /*p08.TUMA*/ wire A000_BFFF = and(bus_tri.A13(), SOGY, bus_tri.A15());
  /*p08.TYNU*/ wire A000_FFFF = or(and(bus_tri.A15(), bus_tri.A14()), A000_BFFF);
  /*p08.TOZA*/ wire A000_FDFF_ABxxxxxx = and(dec_sig.ADDR_VALID_xBCxxxxx, A000_FFFF, dec_sig.ADDR_0000_FE00);
  /*p08.TYHO*/ wire CSn_A = mux2(dma_sig.DMA_A15, A000_FDFF_ABxxxxxx, dma_sig.DMA_READ_CART); // ABxxxxxx

  /*p08.MOCA*/ wire DBG_EXT_RDn = nor(bus_sig.ADDR_VALID_AND_NOT_VRAM, dbg_sig.MODE_DBG1);
  /*p08.LAGU*/ wire LAGU = or(and(sys_sig.CPU_RAW_RD, bus_sig.ADDR_VALID_AND_NOT_VRAMn), sys_sig.CPU_RAW_WR);
  /*p08.LYWE*/ wire LYWE = not(LAGU);
  /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
  /*p08.TYMU*/ wire RD_OUT = nor(dma_sig.DMA_READ_CART, CPU_EXT_RD);

  /*p08.UGAC*/ wire RDn_A = nand(RD_OUT, dbg_sig.MODE_DBG2n1);
  /*p08.URUN*/ wire RDn_D = nor (RD_OUT, dbg_sig.MODE_DBG2);

  /*p01.AREV*/ wire AREV = nand(sys_sig.CPU_RAW_WR, clk_sig.AFAS_xxxxEFGx);
  /*p01.APOV*/ wire CPU_WR_xxxxEFGx  = not(AREV);
  /*p08.MEXO*/ wire MEXO_ABCDxxxH = not(CPU_WR_xxxxEFGx);
  /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDxxxH, DBG_EXT_RDn);
  /*p08.PUVA*/ wire WR_OUT = or(NEVY, dma_sig.DMA_READ_CART);

  /*p08.UVER*/ wire WRn_A = nand(WR_OUT, dbg_sig.MODE_DBG2n1);
  /*p08.USUF*/ wire WRn_D = nor (WR_OUT, dbg_sig.MODE_DBG2);

  //----------

  /* PIN_75 */ cart_pins.PHI   = clk_sig.BUDE_xxxxEFGH;
  /* PIN_78 */ cart_pins.WRn_A = WRn_A;
  /* PIN_78 */ cart_pins.WRn_D = WRn_D;
  /* PIN_79 */ cart_pins.RDn_A = RDn_A;
  /* PIN_79 */ cart_pins.RDn_D = RDn_D;
  /* PIN_80 */ cart_pins.CSn_A = CSn_A;

  //----------
  // Address pin driver

  /*p08.KUPO*/ cart_pins.A00_A = nand(bus_sig.ADDR_OUT_00, dbg_sig.MODE_DBG2n1);
  /*p08.CABA*/ cart_pins.A01_A = nand(bus_sig.ADDR_OUT_01, dbg_sig.MODE_DBG2n1);
  /*p08.BOKU*/ cart_pins.A02_A = nand(bus_sig.ADDR_OUT_02, dbg_sig.MODE_DBG2n1);
  /*p08.BOTY*/ cart_pins.A03_A = nand(bus_sig.ADDR_OUT_03, dbg_sig.MODE_DBG2n1);
  /*p08.BYLA*/ cart_pins.A04_A = nand(bus_sig.ADDR_OUT_04, dbg_sig.MODE_DBG2n1);
  /*p08.BADU*/ cart_pins.A05_A = nand(bus_sig.ADDR_OUT_05, dbg_sig.MODE_DBG2n1);
  /*p08.CEPU*/ cart_pins.A06_A = nand(bus_sig.ADDR_OUT_06, dbg_sig.MODE_DBG2n1);
  /*p08.DEFY*/ cart_pins.A07_A = nand(bus_sig.ADDR_OUT_07, dbg_sig.MODE_DBG2n1);
  /*p08.MYNY*/ cart_pins.A08_A = nand(bus_sig.ADDR_OUT_08, dbg_sig.MODE_DBG2n1);
  /*p08.MUNE*/ cart_pins.A09_A = nand(bus_sig.ADDR_OUT_09, dbg_sig.MODE_DBG2n1);
  /*p08.ROXU*/ cart_pins.A10_A = nand(bus_sig.ADDR_OUT_10, dbg_sig.MODE_DBG2n1);
  /*p08.LEPY*/ cart_pins.A11_A = nand(bus_sig.ADDR_OUT_11, dbg_sig.MODE_DBG2n1);
  /*p08.LUCE*/ cart_pins.A12_A = nand(bus_sig.ADDR_OUT_12, dbg_sig.MODE_DBG2n1);
  /*p08.LABE*/ cart_pins.A13_A = nand(bus_sig.ADDR_OUT_13, dbg_sig.MODE_DBG2n1);
  /*p08.PUHE*/ cart_pins.A14_A = nand(bus_sig.ADDR_OUT_14, dbg_sig.MODE_DBG2n1);
  /*p08.SUZE*/ cart_pins.A15_A = nand(bus_sig.ADDR_OUT_15, dbg_sig.MODE_DBG2n2);

  /*p08.KOTY*/ cart_pins.A00_D = nor (bus_sig.ADDR_OUT_00, dbg_sig.MODE_DBG2);
  /*p08.COTU*/ cart_pins.A01_D = nor (bus_sig.ADDR_OUT_01, dbg_sig.MODE_DBG2);
  /*p08.BAJO*/ cart_pins.A02_D = nor (bus_sig.ADDR_OUT_02, dbg_sig.MODE_DBG2);
  /*p08.BOLA*/ cart_pins.A03_D = nor (bus_sig.ADDR_OUT_03, dbg_sig.MODE_DBG2);
  /*p08.BEVO*/ cart_pins.A04_D = nor (bus_sig.ADDR_OUT_04, dbg_sig.MODE_DBG2);
  /*p08.AJAV*/ cart_pins.A05_D = nor (bus_sig.ADDR_OUT_05, dbg_sig.MODE_DBG2);
  /*p08.CYKA*/ cart_pins.A06_D = nor (bus_sig.ADDR_OUT_06, dbg_sig.MODE_DBG2);
  /*p08.COLO*/ cart_pins.A07_D = nor (bus_sig.ADDR_OUT_07, dbg_sig.MODE_DBG2);
  /*p08.MEGO*/ cart_pins.A08_D = nor (bus_sig.ADDR_OUT_08, dbg_sig.MODE_DBG2);
  /*p08.MENY*/ cart_pins.A09_D = nor (bus_sig.ADDR_OUT_09, dbg_sig.MODE_DBG2);
  /*p08.RORE*/ cart_pins.A10_D = nor (bus_sig.ADDR_OUT_10, dbg_sig.MODE_DBG2);
  /*p08.LYNY*/ cart_pins.A11_D = nor (bus_sig.ADDR_OUT_11, dbg_sig.MODE_DBG2);
  /*p08.LOSO*/ cart_pins.A12_D = nor (bus_sig.ADDR_OUT_12, dbg_sig.MODE_DBG2);
  /*p08.LEVA*/ cart_pins.A13_D = nor (bus_sig.ADDR_OUT_13, dbg_sig.MODE_DBG2);
  /*p08.PAHY*/ cart_pins.A14_D = nor (bus_sig.ADDR_OUT_14, dbg_sig.MODE_DBG2);
  /*p08.RULO*/ cart_pins.A15_D = nor (bus_sig.ADDR_OUT_15, dbg_sig.MODE_DBG2);

  //----------
  // Data pin driver.

  /*p08.LULA*/ cart_pins.D0_B = bus_sig.CBUS_TO_CEXT;
  /*p08.LULA*/ cart_pins.D1_B = bus_sig.CBUS_TO_CEXT;
  /*p08.LULA*/ cart_pins.D2_B = bus_sig.CBUS_TO_CEXT;
  /*p08.LULA*/ cart_pins.D3_B = bus_sig.CBUS_TO_CEXT;
  /*p08.LULA*/ cart_pins.D4_B = bus_sig.CBUS_TO_CEXT;
  /*p08.LULA*/ cart_pins.D5_B = bus_sig.CBUS_TO_CEXT;
  /*p08.LULA*/ cart_pins.D6_B = bus_sig.CBUS_TO_CEXT;
  /*p08.LULA*/ cart_pins.D7_B = bus_sig.CBUS_TO_CEXT;

  /*p25.RUXA*/ cart_pins.D0_A = nand(bus_tri.D0(), bus_sig.CBUS_TO_CEXT);
  /*p25.RUJA*/ cart_pins.D1_A = nand(bus_tri.D1(), bus_sig.CBUS_TO_CEXT);
  /*p25.RABY*/ cart_pins.D2_A = nand(bus_tri.D2(), bus_sig.CBUS_TO_CEXT);
  /*p25.RERA*/ cart_pins.D3_A = nand(bus_tri.D3(), bus_sig.CBUS_TO_CEXT);
  /*p25.RORY*/ cart_pins.D4_A = nand(bus_tri.D4(), bus_sig.CBUS_TO_CEXT);
  /*p25.RYVO*/ cart_pins.D5_A = nand(bus_tri.D5(), bus_sig.CBUS_TO_CEXT);
  /*p25.RAFY*/ cart_pins.D7_A = nand(bus_tri.D6(), bus_sig.CBUS_TO_CEXT);
  /*p25.RAVU*/ cart_pins.D6_A = nand(bus_tri.D7(), bus_sig.CBUS_TO_CEXT);

  /*p08.RUNE*/ cart_pins.D0_D = nor (bus_tri.D0(), bus_sig.CBUS_TO_CEXTn);
  /*p08.RYPU*/ cart_pins.D1_D = nor (bus_tri.D1(), bus_sig.CBUS_TO_CEXTn);
  /*p08.SULY*/ cart_pins.D2_D = nor (bus_tri.D2(), bus_sig.CBUS_TO_CEXTn);
  /*p08.SEZE*/ cart_pins.D3_D = nor (bus_tri.D3(), bus_sig.CBUS_TO_CEXTn);
  /*p08.RESY*/ cart_pins.D4_D = nor (bus_tri.D4(), bus_sig.CBUS_TO_CEXTn);
  /*p08.TAMU*/ cart_pins.D5_D = nor (bus_tri.D5(), bus_sig.CBUS_TO_CEXTn);
  /*p08.ROGY*/ cart_pins.D6_D = nor (bus_tri.D6(), bus_sig.CBUS_TO_CEXTn);
  /*p08.RYDA*/ cart_pins.D7_D = nor (bus_tri.D7(), bus_sig.CBUS_TO_CEXTn);
}

//-----------------------------------------------------------------------------

};