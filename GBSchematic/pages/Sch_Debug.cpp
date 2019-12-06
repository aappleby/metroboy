#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void Debug_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  const System& pa = a.sys;
  const System& pb = b.sys;
  System& pc = c.sys;

  //----------
  // weird debug things, probably not right

  /*p05.AXYN*/ wire AXYN_xBCDEFGH = not(pb.BEDO_Axxxxxxx);
  /*p05.ADYR*/ wire ADYR_Axxxxxxx = not(AXYN_xBCDEFGH);
  /*p05.APYS*/ wire APYS_xBCDEFGH = nor(pb.MODE_DBG2, ADYR_Axxxxxxx);
  /*p05.AFOP*/ wire AFOP_Axxxxxxx = not(APYS_xBCDEFGH);
  /*p07.LECO*/ wire LECO_xBCDEFGH = nor(pb.BEDO_Axxxxxxx, pb.MODE_DBG2);

  /*p05.ANOC*/ if (AFOP_Axxxxxxx) c.D0 = not(chip_in.P10_B);
  /*p05.ATAJ*/ if (AFOP_Axxxxxxx) c.D1 = not(chip_in.P10_B);
  /*p05.AJEC*/ if (AFOP_Axxxxxxx) c.D2 = not(chip_in.P10_B);
  /*p05.ASUZ*/ if (AFOP_Axxxxxxx) c.D3 = not(chip_in.P10_B);
  /*p05.BENU*/ if (AFOP_Axxxxxxx) c.D4 = not(chip_in.P10_B);
  /*p05.AKAJ*/ if (AFOP_Axxxxxxx) c.D5 = not(chip_in.P10_B);
  /*p05.ARAR*/ if (AFOP_Axxxxxxx) c.D6 = not(chip_in.P10_B);
  /*p05.BEDA*/ if (AFOP_Axxxxxxx) c.D7 = not(chip_in.P10_B);

  /*p07.ROMY*/ if (LECO_xBCDEFGH) c.D0 = chip_in.P10_B;
  /*p07.RYNE*/ if (LECO_xBCDEFGH) c.D1 = chip_in.P10_B;
  /*p07.REJY*/ if (LECO_xBCDEFGH) c.D2 = chip_in.P10_B;
  /*p07.RASE*/ if (LECO_xBCDEFGH) c.D3 = chip_in.P10_B;
  /*p07.REKA*/ if (LECO_xBCDEFGH) c.D4 = chip_in.P10_B;
  /*p07.ROWE*/ if (LECO_xBCDEFGH) c.D5 = chip_in.P10_B;
  /*p07.RYKE*/ if (LECO_xBCDEFGH) c.D6 = chip_in.P10_B;
  /*p07.RARU*/ if (LECO_xBCDEFGH) c.D7 = chip_in.P10_B;

  /*p08.TOVA*/ pc.MODE_DBG2n1 = not(pb.MODE_DBG2);
  /*p08.RYCA*/ pc.MODE_DBG2n2 = not(pb.MODE_DBG2);
  /*p08.MULE*/ pc.MODE_DBG1o = not(pb.MODE_DBG1);

  // FF60 debug reg

  /*p07.APER*/ pc.FF60_WRn = nand(pb.MODE_DEBUG, b.A05, b.A06, pb.CPU_WR, pb.ADDR_111111110xx00000);
  /*p07.BURO*/ pc.FF60_0   = tock_pos(pa.FF60_WRn, pb.FF60_WRn, pb.SYS_RESETn1, pb.FF60_0, b.D0);
  /*p07.AMUT*/ pc.FF60_1   = tock_pos(pa.FF60_WRn, pb.FF60_WRn, pb.SYS_RESETn1, pb.FF60_1, b.D1);

  /*p05.JEVA*/ wire FF60_0o = not(pb.FF60_0);

  /*p05.KORE*/ pc.P05_NC0 = nand(pb.DBG_FF00_D7, pb.FF60_0);
  /*p05.KYWE*/ pc.P05_NC1 = nor (pb.DBG_FF00_D7, FF60_0o);

  /*p08.LYRA*/ wire DBG_D_RD = nand(pb.MODE_DBG2, pb.DBUS_OUTn); // polarity?

  /*p08.TUTY*/ if (DBG_D_RD) c.D0 = not(/*p08.TOVO*/ not(chip_in.D0_C));
  /*p08.SYWA*/ if (DBG_D_RD) c.D1 = not(/*p08.RUZY*/ not(chip_in.D1_C));
  /*p08.SUGU*/ if (DBG_D_RD) c.D2 = not(/*p08.ROME*/ not(chip_in.D2_C));
  /*p08.TAWO*/ if (DBG_D_RD) c.D3 = not(/*p08.SAZA*/ not(chip_in.D3_C));
  /*p08.TUTE*/ if (DBG_D_RD) c.D4 = not(/*p08.TEHE*/ not(chip_in.D4_C));
  /*p08.SAJO*/ if (DBG_D_RD) c.D5 = not(/*p08.RATU*/ not(chip_in.D5_C));
  /*p08.TEMY*/ if (DBG_D_RD) c.D6 = not(/*p08.SOCA*/ not(chip_in.D6_C));
  /*p08.ROPA*/ if (DBG_D_RD) c.D7 = not(/*p08.RYBA*/ not(chip_in.D7_C));

#if 0
  //----------
  // if NET01 high, drive external address bus onto internal address
  // these may be backwards, probably don't want to drive external address onto bus normally...

  /*p08.KOVA*/ pc.A00_Cn = not(b.chip.A00_C);
  /*p08.CAMU*/ pc.A01_Cn = not(b.chip.A01_C);
  /*p08.BUXU*/ pc.A02_Cn = not(b.chip.A02_C);
  /*p08.BASE*/ pc.A03_Cn = not(b.chip.A03_C);
  /*p08.AFEC*/ pc.A04_Cn = not(b.chip.A04_C);
  /*p08.ABUP*/ pc.A05_Cn = not(b.chip.A05_C);
  /*p08.CYGU*/ pc.A06_Cn = not(b.chip.A06_C);
  /*p08.COGO*/ pc.A07_Cn = not(b.chip.A07_C);
  /*p08.MUJY*/ pc.A08_Cn = not(b.chip.A08_C);
  /*p08.NENA*/ pc.A09_Cn = not(b.chip.A09_C);
  /*p08.SURA*/ pc.A10_Cn = not(b.chip.A10_C);
  /*p08.MADY*/ pc.A11_Cn = not(b.chip.A11_C);
  /*p08.LAHE*/ pc.A12_Cn = not(b.chip.A12_C);
  /*p08.LURA*/ pc.A13_Cn = not(b.chip.A13_C);
  /*p08.PEVO*/ pc.A14_Cn = not(b.chip.A14_C);
  /*p08.RAZA*/ pc.A15_Cn = not(b.chip.A15_C);

  /*p08.KEJO*/ pc.A00_C = not(pb.A00_Cn); 
  /*p08.BYXE*/ pc.A01_C = not(pb.A01_Cn); 
  /*p08.AKAN*/ pc.A02_C = not(pb.A02_Cn); 
  /*p08.ANAR*/ pc.A03_C = not(pb.A03_Cn); 
  /*p08.AZUV*/ pc.A04_C = not(pb.A04_Cn); 
  /*p08.AJOV*/ pc.A05_C = not(pb.A05_Cn); 
  /*p08.BYNE*/ pc.A06_C = not(pb.A06_Cn); 
  /*p08.BYNA*/ pc.A07_C = not(pb.A07_Cn); 
  /*p08.LOFA*/ pc.A08_C = not(pb.A08_Cn); 
  /*p08.MAPU*/ pc.A09_C = not(pb.A09_Cn); 
  /*p08.RALA*/ pc.A10_C = not(pb.A10_Cn); 
  /*p08.LORA*/ pc.A11_C = not(pb.A11_Cn); 
  /*p08.LYNA*/ pc.A12_C = not(pb.A12_Cn); 
  /*p08.LEFY*/ pc.A13_C = not(pb.A13_Cn); 
  /*p08.NEFE*/ pc.A14_C = not(pb.A14_Cn); 
  /*p08.SYZU*/ pc.A15_C = not(pb.A15_Cn);


  if (b.MODE_DBG2n1) c.cpu.A00 = pb.A00_C;
  if (b.MODE_DBG2n1) c.cpu.A01 = pb.A01_C;
  if (b.MODE_DBG2n1) c.cpu.A02 = pb.A02_C;
  if (b.MODE_DBG2n1) c.cpu.A03 = pb.A03_C;
  if (b.MODE_DBG2n1) c.cpu.A04 = pb.A04_C;
  if (b.MODE_DBG2n1) c.cpu.A05 = pb.A05_C;
  if (b.MODE_DBG2n1) c.cpu.A06 = pb.A06_C;
  if (b.MODE_DBG2n1) c.cpu.A07 = pb.A07_C;
  if (b.MODE_DBG2n1) c.cpu.A08 = pb.A08_C;
  if (b.MODE_DBG2n1) c.cpu.A09 = pb.A09_C;
  if (b.MODE_DBG2n1) c.cpu.A10 = pb.A10_C;
  if (b.MODE_DBG2n1) c.cpu.A11 = pb.A11_C;
  if (b.MODE_DBG2n1) c.cpu.A12 = pb.A12_C;
  if (b.MODE_DBG2n1) c.cpu.A13 = pb.A13_C;
  if (b.MODE_DBG2n1) c.cpu.A14 = pb.A14_C;

  if (pb.MODE_DBG2n2) {
    c.cpu.A15 = pb.A15_C;
  }
#endif
}

//-----------------------------------------------------------------------------

};