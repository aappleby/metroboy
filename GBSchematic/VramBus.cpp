#include "VramBus.h"

#include "ExtCpuBuses_sch.h"
#include "MemBus.h"
#include "OAM.h"
#include "ExtBus.h"
#include "PPU.h"
#include "Debug.h"
#include "CpuBus.h"
#include "AddressDecoder.h"
#include "System.h"
#include "Clocks.h"
#include "DmaBus.h"
#include "Sprites.h"
#include "Window.h"

//-----------------------------------------------------------------------------
// 25_VRAM_INTERFACE.png

void Vram::tick(OAM& oam, const AddressDecoder& dec, const Window& win, Vram& vram, const ExtCpuBuses_sch& ext_sch, MemBus& mem) {
  bool SOTO_Q = SOTO.q();

  bool RYVO = nand(mem.D5, ext_sch.LULA);
  bool RERA = nand(mem.D3, ext_sch.LULA);
  bool RABY = nand(mem.D2, ext_sch.LULA);
  bool RORY = nand(mem.D4, ext_sch.LULA);
  bool RUJA = nand(mem.D1, ext_sch.LULA);
  bool RAVU = nand(mem.D7, ext_sch.LULA);
  bool RAFY = nand(mem.D6, ext_sch.LULA);
  bool RUXA = nand(mem.D0, ext_sch.LULA);

  // why is this writing to the _A part of the data bus but not the _D?
  ext.D5_A = RYVO;
  ext.D3_A = RERA;
  ext.D2_A = RABY;
  ext.D4_A = RORY;
  ext.D1_A = RUJA;
  ext.D7_A = RAVU;
  ext.D6_A = RAFY;
  ext.D0_A = RUXA;

  // odd...
  bool SOSE = and(mem.A15, nor(not(dec.FEXXFFXXn), ext_sch.TEXO));
  bool RAWA = not(and(spr.TACU, spr.TEXY));

  bool SYCY = not(dbg.NET02);
  SOTO.flip(SYCY, 0);

  bool TUCA = and(SOSE, clk.ABUZ);
  bool TUJA = and(SOSE, cpu.CPU_RD_SYNC);
  bool TEGU = and(SOSE, clk.AFAS);
  bool TAVY = not(ext.MOE_IN);
  bool TUTO = and(dbg.NET02, !SOTO_Q);
  bool SUDO = not(ext.MWR_IN);
  bool TEFY = not(ext.MCS_IN);

  bool SALE = mux2(TUTO, TAVY, TEGU);
  bool TYJY = mux2(TUTO, SUDO, TUJA);
  bool TOLE = mux2(TUTO, TEFY, TUCA);

  bool ROPY = not(ppu.XYMU);
  bool RYLU = nand(SALE, ROPY);

  bool APAM = not(dma.VRAM_TO_OAM);
  bool SOHY = nand(TYJY, and(TOLE, ROPY));

  bool SUTU = nor(win.LENA, dma.VRAM_TO_OAM, spr.TEXY, and(TOLE, ROPY));
  bool SAZO = and(not(SALE), and(TOLE, ROPY));
  bool RACU = and(RYLU, RAWA, win.MYMA, APAM);
  bool RACO = not(TUTO);

  bool RUTE = or(TUTO, RACO); // wat? or of a signal with its own inverse...
  bool SEWO = or(TUTO, SUTU);
  bool TODE = and(SUTU, RACO);
  bool SEMA = and(RACU, RACO);
  bool TAXY = and(SOHY, RACO);
  bool SOFY = or(TUTO, SOHY);

  ext.MCS_A = not(TODE);
  ext.MCS_D = not(SEWO);
  ext.MWR_A = not(TAXY);
  ext.MWR_D = not(SOFY);
  ext.MOE_A = not(SEMA);
  ext.MOE_D = not(RUTE);

  // there's a combi circuit here that delays ROCY relative to RELA, but I'm ignoring it.

  ext.MD_B = SAZO; // something about vram pins...

  if (not(SAZO)) {
    MD0 = ext.MD0_IN;
    MD1 = ext.MD1_IN;
    MD2 = ext.MD2_IN;
    MD3 = ext.MD3_IN;
    MD4 = ext.MD4_IN;
    MD5 = ext.MD5_IN;
    MD6 = ext.MD6_IN;
    MD7 = ext.MD7_IN;
  }

  // wat?
  if (not(ext.P10_B)) {
    bool TUSO = nor(dbg.NET02, clk.BEDO);
    mem.D4 = TUSO;
    mem.D5 = TUSO;
    mem.D3 = TUSO;
    mem.D0 = TUSO;
    mem.D6 = TUSO;
    mem.D7 = TUSO;
    mem.D1 = TUSO;
    mem.D2 = TUSO;
  }

  if (not(SAZO)) {
    MD0 = mem.D0;
    MD1 = mem.D1;
    MD2 = mem.D2;
    MD3 = mem.D3;
    MD4 = mem.D4;
    MD5 = mem.D5;
    MD6 = mem.D6;
    MD7 = mem.D7;
  }

  // seems like there's an extra NOT in here...
  ext.MD0_OUT = not(or(not(SAZO), vram.MD0));
  ext.MD1_OUT = not(or(not(SAZO), vram.MD1));
  ext.MD2_OUT = not(or(not(SAZO), vram.MD2));
  ext.MD3_OUT = not(or(not(SAZO), vram.MD3));
  ext.MD4_OUT = not(or(not(SAZO), vram.MD4));
  ext.MD5_OUT = not(or(not(SAZO), vram.MD5));
  ext.MD6_OUT = not(or(not(SAZO), vram.MD6));
  ext.MD7_OUT = not(or(not(SAZO), vram.MD7));

  if (or(dma.VRAM_TO_OAM, ppu.XYMU)) {
    MA0  = mem.A0;
    MA1  = mem.A1;
    MA2  = mem.A2;
    MA3  = mem.A3;
    MA4  = mem.A4;
    MA5  = mem.A5;
    MA6  = mem.A6;
    MA7  = mem.A7;
    MA8  = mem.A8;
    MA9  = mem.A9;
    MA10 = mem.A10;
    MA11 = mem.A11;
    MA12 = mem.A12;
  }

  if (nand(dbg.T1nT2, ext_sch.RORU)) {
    mem.D0 = ext.D0_IN;
    mem.D1 = ext.D1_IN;
    mem.D2 = ext.D2_IN;
    mem.D3 = ext.D3_IN;
    mem.D4 = ext.D4_IN;
    mem.D5 = ext.D5_IN;
    mem.D6 = ext.D6_IN;
    mem.D7 = ext.D7_IN;
  }

  ext.MD0_A = not(and(vram.MD0, SAZO));
  ext.MD1_A = not(and(vram.MD1, SAZO));
  ext.MD2_A = not(and(vram.MD2, SAZO));
  ext.MD3_A = not(and(vram.MD3, SAZO));
  ext.MD4_A = not(and(vram.MD4, SAZO));
  ext.MD5_A = not(and(vram.MD5, SAZO));
  ext.MD6_A = not(and(vram.MD6, SAZO));
  ext.MD7_A = not(and(vram.MD7, SAZO));

  if (not(dma.LUMA)) {
    oam.OAM_A_D0 = ext.D0_IN;
    oam.OAM_A_D1 = ext.D1_IN;
    oam.OAM_A_D2 = ext.D2_IN;
    oam.OAM_A_D3 = ext.D3_IN;
    oam.OAM_A_D4 = ext.D4_IN;
    oam.OAM_A_D5 = ext.D5_IN;
    oam.OAM_A_D6 = ext.D6_IN;
    oam.OAM_A_D7 = ext.D7_IN;

    oam.OAM_B_D0 = ext.D0_IN;
    oam.OAM_B_D1 = ext.D1_IN;
    oam.OAM_B_D2 = ext.D2_IN;
    oam.OAM_B_D3 = ext.D3_IN;
    oam.OAM_B_D4 = ext.D4_IN;
    oam.OAM_B_D5 = ext.D5_IN;
    oam.OAM_B_D6 = ext.D6_IN;
    oam.OAM_B_D7 = ext.D7_IN;
  }

  if (and(and(TOLE, ROPY), spr.LEKO)) {
    mem.D0 = vram.MD0;
    mem.D1 = vram.MD1;
    mem.D2 = vram.MD2;
    mem.D3 = vram.MD3;
    mem.D4 = vram.MD4;
    mem.D5 = vram.MD5;
    mem.D6 = vram.MD6;
    mem.D7 = vram.MD7;
  }

  if (nand(win.NETA, win.PORE)) {
    // FIXME - should this negation be here? probably not
    vram.MA0 = not(win.XUHA);
    vram.MA1 = not(win.VYNO.q());
    vram.MA2 = not(win.VUJO.q());
    vram.MA3 = not(win.VYMU.q());
  }

  if (win.NETA) {
    // same thing with the negations
    vram.MA4  = not(ppu.RAWU.q());;
    vram.MA5  = not(ppu.POZO.q());;
    vram.MA6  = not(ppu.PYZO.q());;
    vram.MA7  = not(ppu.POXA.q());;
    vram.MA8  = not(ppu.PULO.q());;
    vram.MA9  = not(ppu.POJU.q());;
    vram.MA10 = not(ppu.POWY.q());;
    vram.MA11 = not(ppu.PYJU.q());;
    vram.MA12 = or(ppu.LCDC_BGTILE, ppu.PYJU.q());;
  }
}