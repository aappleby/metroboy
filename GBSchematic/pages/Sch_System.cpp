#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void System_tick(const CpuIn& cpu_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // CPU reset

  /*p01.UCOB*/ c.sys.CLK_BAD1    = not(b.pin.CLKIN_A);
  /*p01.ATEZ*/ c.sys.CLK_BAD2    = not(b.pin.CLKIN_A);

  /*p01.ABOL*/ c.sys.CPUCLK_REQn = not(cpu_in.CPUCLK_REQ);
  /*p01.BUTY*/ c.sys.CPUCLK_REQ  = not(b.sys.CPUCLK_REQn);

  {
    /*p01.TUBO*/ wire NO_CLOCK  = or(b.sys.CPUCLK_REQn, /*p01.UPYF*/ or(b.pin.RST, b.sys.CLK_BAD1));
    /*p01.UNUT*/ wire TIMEOUT   = and(NO_CLOCK, b.tim.DIV_15);
    /*p01.TABA*/ wire CPU_RESET   = or(b.dbg.MODE_DBG2, b.dbg.MODE_DBG1, TIMEOUT);

    // PORTD_07 = RESET_CLK
    c.cpu_out.CPU_RESET = CPU_RESET;

    /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);
    /*p01.ASOL*/ wire RESET_IN  = or (/*p01.AFAR*/ nor(CPU_RESETn, b.pin.RST), b.pin.RST);
    /*p01.BOMA*/ c.sys.RESET_CLK = not(a.sys.BOGA_xBCDEFGH);

    /*p01.AFER*/ c.sys.RESET_REG = tock_pos(a.sys.RESET_CLK, b.sys.RESET_CLK, b.dbg.MODE_PROD, b.sys.RESET_REG, RESET_IN);

    // master system reset
    /*p01.AVOR*/ wire AVOR_RESET  = or(b.sys.RESET_REG, RESET_IN);
    /*p01.ALUR*/ c.sys.SYS_RESETn = not(AVOR_RESET);  // this goes all over the place

    // master video reset
    /*p01.DULA*/ wire DULA_RESET  = not(b.sys.SYS_RESETn);
    /*p01.CUNU*/ wire CUNU_RESET  = not(DULA_RESET);
    /*p01.XORE*/ wire XORE_RESET  = not(CUNU_RESET);
    /*p01.XEBE*/ wire XEBE_RESET  = not(XORE_RESET);
    /*p01.XODO*/ wire XODO_RESET  = nand(XEBE_RESET, b.vid.LCDC_EN);
    /*p01.XAPO*/ c.sys.VID_RESETn = not(XODO_RESET);
  }


  //----------
  // Clock input and deglitcher

  {
    /*p01.ARYS*/ wire CLKIN_Bn  = not(b.pin.CLKIN_B);
    /*p01.ANOS*/ c.sys.ROOTCLK_AxCxExGx  = nand(b.pin.CLKIN_B, b.sys.ROOTCLK_xBxDxFxH);
    /*p01.AVET*/ c.sys.ROOTCLK_xBxDxFxH  = nand(b.sys.ROOTCLK_AxCxExGx, CLKIN_Bn);
  }

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  {
    /*p01.ATAL*/ c.sys.PHASE_CLK = not(b.sys.ROOTCLK_xBxDxFxH); // apu, phase generator
    /*p01.AFUR*/ c.sys.PHASE_ABCDxxxx1 = tock_duo(a.sys.PHASE_CLK, b.sys.PHASE_CLK, b.dbg.MODE_PROD, b.sys.PHASE_ABCDxxxx1, !b.sys.PHASE_xxxDEFGx1);
    /*p01.ALEF*/ c.sys.PHASE_xBCDExxx1 = tock_duo(a.sys.PHASE_CLK, b.sys.PHASE_CLK, b.dbg.MODE_PROD, b.sys.PHASE_xBCDExxx1,  b.sys.PHASE_ABCDxxxx1);
    /*p01.APUK*/ c.sys.PHASE_xxCDEFxx1 = tock_duo(a.sys.PHASE_CLK, b.sys.PHASE_CLK, b.dbg.MODE_PROD, b.sys.PHASE_xxCDEFxx1,  b.sys.PHASE_xBCDExxx1);
    /*p01.ADYK*/ c.sys.PHASE_xxxDEFGx1 = tock_duo(a.sys.PHASE_CLK, b.sys.PHASE_CLK, b.dbg.MODE_PROD, b.sys.PHASE_xxxDEFGx1,  b.sys.PHASE_xxCDEFxx1);
  }

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.sys.PHASE_ABCDxxxx1);  
    /*p01.ADAR*/ wire ADAR_ABCxxxxH = not( b.sys.PHASE_xxxDEFGx1);
    /*p01.AFAS*/ c.sys.PHASE_xxxxEFGx3 = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);
  }

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.sys.PHASE_ABCDxxxx1);  
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(b.sys.CPUCLK_REQn, ATYP_ABCDxxxx);
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
    /*p01.UVYT*/ c.sys.PHASE_ABCDxxxx7b = not(BUDE_xxxxEFGH); // dma
    /*p04.MOPA*/ c.sys.PHASE_xxxxEFGH8b = not(b.sys.PHASE_ABCDxxxx7b); // -> sprites, dma
  }

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.sys.PHASE_ABCDxxxx1);  
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(b.sys.CPUCLK_REQn, ATYP_ABCDxxxx);
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
    /*p01.DOVA*/ c.sys.PHASE_ABCDxxxx7c = not(BUDE_xxxxEFGH); // apu
  }

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.sys.PHASE_ABCDxxxx1);  
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( b.sys.PHASE_xBCDExxx1);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(!b.sys.PHASE_xxCDEFxx1);

    /*p01.BAPY*/ wire BAPY_xxxxxxGH = nor(b.sys.CPUCLK_REQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
    /*p01.NULE*/ wire NULE_xxxxEFGH = nor(b.sys.CPUCLK_REQn, ATYP_ABCDxxxx);
    /*p01.BERU*/ wire BERU_ABCDEFxx = not(BAPY_xxxxxxGH);
    /*p01.BYRY*/ wire BYRY_ABCDxxxx = not(NULE_xxxxEFGH);
    /*p01.BUFA*/ wire BUFA_xxxxxxGH = not(BERU_ABCDEFxx);
    /*p01.BOLO*/ wire BOLO_ABCDEFxx = not(BUFA_xxxxxxGH);
    /*p01.BUDE*/ wire BUDE_xxxxEFGH = not(BYRY_ABCDxxxx);
    /*p01.BEKO*/ wire BEKO_ABCDxxxx = not(BUDE_xxxxEFGH);
    /*p01.BEJA*/ wire BEJA_xxxxEFGH = nand(BOLO_ABCDEFxx, BEKO_ABCDxxxx);
    /*p01.BANE*/ wire BANE_ABCDxxxx = not(BEJA_xxxxEFGH);
    /*p01.BELO*/ wire BELO_xxxxEFGH = not(BANE_ABCDxxxx);
    /*p01.BAZE*/ wire BAZE_ABCDxxxx = not(BELO_xxxxEFGH);
    /*p01.BUTO*/ wire BUTO_xBCDEFGH = nand(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
    /*p01.BELE*/ wire BELE_Axxxxxxx = not(BUTO_xBCDEFGH);
    /*p01.BYJU*/ wire BYJU_xBCDEFGH = nor(BELE_Axxxxxxx, b.sys.CLK_BAD2);
    /*p01.BALY*/ wire BALY_Axxxxxxx = not(BYJU_xBCDEFGH);

    /*p01.BOGA*/ c.sys.BOGA_xBCDEFGH = not(BALY_Axxxxxxx); // joypad, timer, reset
    /*p01.BUVU*/ wire BUVU_Axxxxxxx = and(b.sys.CPUCLK_REQ, BALY_Axxxxxxx);
    /*p01.BYXO*/ wire BYXO_xBCDEFGH = not(BUVU_Axxxxxxx);
    /*p01.BEDO*/ c.sys.BEDO_Axxxxxxx = not(BYXO_xBCDEFGH); // debug

    /*p01.BOWA*/ wire BOWA_xBCDEFGH = not(b.sys.BEDO_Axxxxxxx);
    c.cpu_out.CLK_xBCDEFGH = BOWA_xBCDEFGH;
  }

  //----------
  // decoders


  {
    /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
    /*p07.TONA*/ wire ADDR_08n = not(b.A08);
    /*p07.SYKE*/ c.sys.ADDR_FFXX = nor(ADDR_0000_FE00, ADDR_08n);
  }

  /*
  0b000xxxxxxxxxxxxx - 0x0000 to 0x1FFF (cart rom)
  0b001xxxxxxxxxxxxx - 0x2000 to 0x3FFF (cart rom)
  0b010xxxxxxxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
  0b011xxxxxxxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
  0b100xxxxxxxxxxxxx - 0x8000 to 0x9FFF (vram)
  0b101xxxxxxxxxxxxx - 0xA000 to 0xBFFF (cart ram)
  0b110xxxxxxxxxxxxx - 0xC000 to 0xDFFF (internal ram)
  0b111xxxxxxxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)
  */

  //----------
  // Cart select/read/write signals
  {
    {
    }

    // debug override of CPU_RD/CPU_WR
    
    {
      /*p07.UJYV*/ wire CPU_RD_MUX   = mux2n(b.pin.RD_C, cpu_in.CPU_RAW_RD, b.dbg.MODE_DBG2);
      /*p07.TEDO*/ c.sys.CPU_RD  = not(CPU_RD_MUX);
    }
    
    {
      /*p01.AREV*/ wire AREV = nand(cpu_in.CPU_RAW_WR, b.sys.PHASE_xxxxEFGx3);
      /*p01.APOV*/ wire CPU_WR_xxxxEFGx  = not(AREV);
      /*p07.UBAL*/ wire CPU_WR_MUX   = mux2n(b.pin.WR_C, CPU_WR_xxxxEFGx,   b.dbg.MODE_DBG2);
      /*p07.TAPU*/ c.sys.CPU_WR  = not(CPU_WR_MUX);
    }
    
    {
      /*p07.AJAS*/ wire CPU_RDn = not(b.sys.CPU_RD);
      /*p07.ASOT*/ c.sys.CPU_RD2 = not(CPU_RDn);
    }
    
    {
      /*p07.DYKY*/ wire CPU_WRn = not(b.sys.CPU_WR);
      /*p07.CUPA*/ c.sys.CPU_WR2 = not(CPU_WRn);
    }

    {
      /*p01.ATYP*/ wire PHASE_ABCDxxxx2 = not(!b.sys.PHASE_ABCDxxxx1);  
      /*p01.AROV*/ wire PHASE_xxCDEFxx2 = not(!b.sys.PHASE_xxCDEFxx1);
      /*p01.AJAX*/ wire PHASE_xxxxEFGH3 = not(PHASE_ABCDxxxx2);
      /*p01.AGUT*/ wire AGUT_xxCDEFGH = and(or(PHASE_xxxxEFGH3, PHASE_xxCDEFxx2), cpu_in.ADDR_VALID);
      /*p01.AWOD*/ wire AWOD = or(b.dbg.MODE_DBG2, AGUT_xxCDEFGH);
      /*p01.ABUZ*/ c.sys.ADDR_VALID_ABxxxxxx = not(AWOD);
    }
  }
}

//-----------------------------------------------------------------------------

};