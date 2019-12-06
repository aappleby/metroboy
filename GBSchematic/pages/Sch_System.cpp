#include "Sch_System.h"
#include "Sch_Gameboy.h"

#pragma warning(disable : 4189)
#pragma warning(disable : 4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void System_tick(const CpuIn& cpu_in, const ChipIn& chip_in, const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // Debug mode enable signals

  /*p07.UBET*/ c.sys.T1n        = not(chip_in.T1);
  /*p07.UVAR*/ c.sys.T2n        = not(chip_in.T2);
  /*p07.UMUT*/ c.sys.MODE_DBG1  = and(chip_in.T1, b.sys.T2n);
  /*p07.YAZA*/ c.sys.MODE_DBG1n = not(b.sys.MODE_DBG1);
  /*p07.UNOR*/ c.sys.MODE_DBG2  = and(chip_in.T2, b.sys.T1n);
  /*p07.UPOJ*/ c.sys.MODE_PROD  = nand(b.sys.T1n, b.sys.T2n, chip_in.RST);
  /*p07.APET*/ c.sys.MODE_DEBUG = or(b.sys.MODE_DBG1, b.sys.MODE_DBG2);

  //----------
  // CPU reset

  /*p01.UCOB*/ c.sys.CLK_BAD1    = not(chip_in.CLKIN_A);
  /*p01.ATEZ*/ c.sys.CLK_BAD2    = not(chip_in.CLKIN_A);

  /*p01.ABOL*/ c.sys.CPUCLK_REQn = not(cpu_in.CPUCLK_REQ);
  /*p01.BUTY*/ c.sys.CPUCLK_REQ  = not(b.sys.CPUCLK_REQn);

  {
    /*p01.TUBO*/ wire NO_CLOCK  = or(b.sys.CPUCLK_REQn, /*p01.UPYF*/ or(chip_in.RST, b.sys.CLK_BAD1));
    /*p01.UNUT*/ wire TIMEOUT   = and(NO_CLOCK, b.sys.DIV_15);
    /*p01.TABA*/ wire CPU_RESET   = or(b.sys.MODE_DBG2, b.sys.MODE_DBG1, TIMEOUT);
    /*p01.ALYP*/ wire CPU_RESETn  = not(CPU_RESET);


    /*p01.ASOL*/ wire RESET_IN  = or (/*p01.AFAR*/ nor(CPU_RESETn, chip_in.RST), chip_in.RST);
    /*p01.BOMA*/ c.sys.RESET_CLK = not(a.sys.BOGA_xBCDEFGH);
    /*p01.AFER*/ c.sys.RESET_REG = tock_pos(a.sys.RESET_CLK, b.sys.RESET_CLK, b.sys.MODE_PROD, b.sys.RESET_REG, RESET_IN);

    /*p01.AVOR*/ wire AVOR_RESET = or(b.sys.RESET_REG, RESET_IN);
    /*p01.ALUR*/ c.sys.SYS_RESETn1 = not(AVOR_RESET);  // this goes all over the place

    // PORTD_07 = RESET_CLK
    c.cpu_out.CPU_RESET = CPU_RESET;
  }

  //----------
  // Clock input and deglitcher

  /*p01.ARYS*/ c.sys.CLK_AxCxExGx5  = not(chip_in.CLKIN_B);
  /*p01.ANOS*/ c.sys.CLK_AxCxExGx6  = nand(chip_in.CLKIN_B, b.sys.CLK_xBxDxFxH3);
  /*p01.AVET*/ c.sys.CLK_xBxDxFxH3  = nand(b.sys.CLK_AxCxExGx6, b.sys.CLK_AxCxExGx5);

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  /*p01.ATAL*/ c.sys.CLK_AxCxExGx3  = not(b.sys.CLK_xBxDxFxH3); // apu, phase generator
  /*p01.AFUR*/ c.sys.PHASE_ABCDxxxx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.PHASE_ABCDxxxx1, !b.sys.PHASE_xxxDEFGx1);
  /*p01.ALEF*/ c.sys.PHASE_xBCDExxx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.PHASE_xBCDExxx1,  b.sys.PHASE_ABCDxxxx1);
  /*p01.APUK*/ c.sys.PHASE_xxCDEFxx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.PHASE_xxCDEFxx1,  b.sys.PHASE_xBCDExxx1);
  /*p01.ADYK*/ c.sys.PHASE_xxxDEFGx1 = tock_duo(a.sys.CLK_AxCxExGx3, b.sys.CLK_AxCxExGx3, b.sys.MODE_PROD, b.sys.PHASE_xxxDEFGx1,  b.sys.PHASE_xxCDEFxx1);

  {
    /*p01.ATYP*/ wire ATYP_ABCDxxxx = not(!b.sys.PHASE_ABCDxxxx1);  
    /*p01.ADAR*/ wire ADAR_ABCxxxxH = not( b.sys.PHASE_xxxDEFGx1);
    /*p01.AFAS*/ c.sys.PHASE_xxxxEFGx3 = nor(ADAR_ABCxxxxH, ATYP_ABCDxxxx);
  }

  {
    /*p01.AFEP*/ wire AFEP_AxxxxFGH = not( b.sys.PHASE_xBCDExxx1);
    /*p01.AROV*/ wire AROV_xxCDEFxx = not(!b.sys.PHASE_xxCDEFxx1);
    /*p01.BUGO*/ wire BUGO_xBCDExxx = not(AFEP_AxxxxFGH);
    /*p01.BATE*/ wire BATE_AxxxxxGH = nor(b.sys.CPUCLK_REQn, BUGO_xBCDExxx, AROV_xxCDEFxx);
    /*p01.BASU*/ wire BASU_xBCDEFxx = not(BATE_AxxxxxGH);
    /*p01.BUKE*/ c.sys.PHASE_AxxxxxGH6 = not(BASU_xBCDEFxx);
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

  {
    /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
    /*p07.RYCU*/ wire ADDR_FE00_FFFF = not(ADDR_0000_FE00);
    /*p07.SOHA*/ wire ADDR_FFXXn2 = not(b.sys.ADDR_FFXX);
    /*p07.ROPE*/ wire ADDR_FEXXn = nand(ADDR_FE00_FFFF, ADDR_FFXXn2);
    /*p07.SARO*/ c.sys.ADDR_OAM = not(ADDR_FEXXn);
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
      /*p01.AREV*/ wire AREV = nand(cpu_in.CPU_RAW_WR, b.sys.PHASE_xxxxEFGx3);
      /*p01.APOV*/ c.sys.CPU_WR_xxxxEFGx  = not(AREV);
    }
    
    {
      /*p07.UJYV*/ wire CPU_RD_MUX   = mux2n(chip_in.RD_C, cpu_in.CPU_RAW_RD, b.sys.MODE_DBG2);
      /*p07.TEDO*/ c.sys.CPU_RD  = not(CPU_RD_MUX);
    }
    
    {
      /*p07.UBAL*/ wire CPU_WR_MUX   = mux2n(chip_in.WR_C, b.sys.CPU_WR_xxxxEFGx,   b.sys.MODE_DBG2);
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
      /*p01.AWOD*/ wire AWOD = or(b.sys.MODE_DBG2, AGUT_xxCDEFGH);
      /*p01.ABUZ*/ c.sys.ADDR_VALID_ABxxxxxx = not(AWOD);
      /*p08.SOGY*/ wire SOGY = not(b.A14);
      /*p08.TUMA*/ wire A000_BFFF = and(b.A13, SOGY, b.A15);
      /*p08.TYNU*/ wire A000_FFFF = or(and(b.A15, b.A14), A000_BFFF);
      /*p07.TUNA*/ wire ADDR_0000_FE00 = nand(b.A15, b.A14, b.A13, b.A12, b.A11, b.A10, b.A09);
      /*p08.TOZA*/ wire A000_FDFF_ABxxxxxx = and(b.sys.ADDR_VALID_ABxxxxxx, A000_FFFF, ADDR_0000_FE00);
      /*p08.TYHO*/ c.chip_out.CS_A = mux2(b.sys.DMA_A15, A000_FDFF_ABxxxxxx, b.sys.DO_DMA); // ABxxxxxx
    }

    {
      /*p08.SORE*/ wire SORE = not(b.A15);
      /*p08.TEVY*/ wire ADDR_NOT_VRAM = or(b.A13, b.A14, SORE);
      /*p08.TEXO*/ c.sys.ADDR_VALID_AND_NOT_VRAM = and(cpu_in.ADDR_VALID, ADDR_NOT_VRAM);
    }


    {
      /*p08.MOCA*/ wire DBG_EXT_RDn = nor(b.sys.ADDR_VALID_AND_NOT_VRAM, b.sys.MODE_DBG1);
      /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(b.sys.ADDR_VALID_AND_NOT_VRAM);
      /*p08.LAGU*/ wire LAGU = or(and(cpu_in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), cpu_in.CPU_RAW_WR);
      /*p08.LYWE*/ wire LYWE = not(LAGU);
      /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
      /*p08.TYMU*/ wire RD_OUT = nor(b.sys.DO_DMA, CPU_EXT_RD);
      /*p08.UGAC*/ c.chip_out.RD_A = nand(RD_OUT, b.sys.MODE_DBG2n1);
      /*p08.URUN*/ c.chip_out.RD_D = nor (RD_OUT, b.sys.MODE_DBG2);
    }

    {
      /*p08.MOCA*/ wire DBG_EXT_RDn = nor(b.sys.ADDR_VALID_AND_NOT_VRAM, b.sys.MODE_DBG1);
      /*p08.MEXO*/ wire MEXO_ABCDxxxH = not(b.sys.CPU_WR_xxxxEFGx);
      /*p08.NEVY*/ wire NEVY = or(MEXO_ABCDxxxH, DBG_EXT_RDn);
      /*p08.PUVA*/ wire WR_OUT = or(NEVY, b.sys.DO_DMA);
      /*p08.UVER*/ c.chip_out.WR_A = nand(WR_OUT, b.sys.MODE_DBG2n1);
      /*p08.USUF*/ c.chip_out.WR_D = nor (WR_OUT, b.sys.MODE_DBG2);
    }

    {
      /*p08.MOCA*/ wire DBG_EXT_RDn = nor(b.sys.ADDR_VALID_AND_NOT_VRAM, b.sys.MODE_DBG1);
      /*p08.LEVO*/ wire ADDR_VALID_AND_NOT_VRAMn = not(b.sys.ADDR_VALID_AND_NOT_VRAM);
      /*p08.LAGU*/ wire LAGU = or(and(cpu_in.CPU_RAW_RD, ADDR_VALID_AND_NOT_VRAMn), cpu_in.CPU_RAW_WR);
      /*p08.LYWE*/ wire LYWE = not(LAGU);
      /*p08.MOTY*/ wire CPU_EXT_RD = or(DBG_EXT_RDn, LYWE);
      /*p08.REDU*/ wire CPU_RDo = not(b.sys.CPU_RD);
      /*p08.RORU*/ c.sys.DBUS_OUTn = mux2(CPU_RDo, CPU_EXT_RD, b.sys.MODE_DBG2);
      /*p08.LULA*/ c.sys.DBUS_OUT  = not(b.sys.DBUS_OUTn);
    }

    {
      // addr >= FF80 and not XXFF
      /*p07.WALE*/ wire ADDR_x1111111n = nand(b.A00, b.A01, b.A02, b.A03, b.A04, b.A05, b.A06);
      /*p07.WOLY*/ wire WOLY = nand(b.sys.ADDR_FFXX, b.A07, ADDR_x1111111n);
      /*p07.WUTA*/ c.HRAM_CS = not(WOLY);
    }
  }
}

//-----------------------------------------------------------------------------

};