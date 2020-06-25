#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

#pragma warning(disable:4100) // unreferenced formal parameter

using namespace Schematics;

/*
0b000xxxxx_xxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxx_xxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxx_xxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxx_xxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxx_xxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxx_xxxxxxxx - 0xA000 to 0xBFFF (cart ram)

0b110xxxxx_xxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxx_xxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)

0b11111110_00000000 - 0xFE00 - OAM begin
0b11111110_10011111 - 0xFE9F - OAM end

0b11111111_00000000 - 0xFF00 - IO begin
0b11111111_01111111 - 0xFF7F - IO end

0b11111111_10000000 - 0xFF80 - Zeropage begin
0b11111111_11111110 - 0xFFFE - Zeropage end
0b11111111_11111111 - 0xFFFF - Interrupt enable
*/

TestGB gb;

//-----------------------------------------------------------------------------

#if 0

//Die trace:
ABOL = not(CLKREQ)
ATEZ = not(CLKIN_A)

ADAR = not(ADYK.qn)
ATYP = not(AFUR.q);
AROV = not(APUK.qn);
AFEP = not(ALEF.qn);

AFAS = nor(ADAR, ATYP)
AREV = nand(AFAS, CPU_RAW_WR)
APOV = not(AREV) // 4 rung inverter


APAP = not(CPU_ADDR_VALID)
AWOD = nor(UNOR, APAP)
ABUZ = not(AWOD)

AGUT = ? ? ? (AROV, AJAX) - gate unused ?
APAP = not(CPU_ADDR_VALID)
AJAX = not(ATYP)
ALUR = not(AVOR) // 3 rung inverter
AVOR = or (AFER, ASOL)

AFER = reg, gap above it or something.starting at the first connected rung

// Because this has BOGA and BOMA both as inputs and two UPOJ inputs, maybe
// it's supposed to be a clock-crossing synchronizer?

AFER00 << UPOJ
AFER01 << ASOL
AFER02 nc
AFER03 << BOGA
AFER04 nc
AFER05 nc
AFER06 << BOMA
AFER06 << UPOJ
AFER07 nc
AFER08 nc
AFER09 >> nc     // QN
AFER10 >> AVOR   // Q

ASOL = nor_latch(<< AFAR, nc, >> AVOR, nc, nc, << PIN_RST) - output inverted

AFAR = nor(PIN_RST, ALYP)
ALYP = not(TABA)
ADAR = not(ADYK)


// Hax

/*p01.ADYK*/ clk_reg.PHAZ_xxxxEFGH
/*p01.AFUR*/ clk_reg.PHAZ_xBCDExxx

ABOL = not(CLKREQ)
ATEZ = not(CLKIN_A)
APOV = and(ADYK_xxxxEFGH, AFUR_xBCDExxx, CPU_RAW_WR) // cpu write is _only_ on E?

ABUZ = or(UNOR, !CPU_ADDR_VALID)

AGUT = ? ? ? (AROV, AJAX) - gate unused ?
AJAX = AFUR
ALUR = nor(AFER_RST, ASOL_RST_LATCH)



#endif

void TestGB::tick_everything() {
  //----------------------------------------
  // sch_system

  ///*p01.ARYS*/ wire ARYS_xBxDxFxH = not(ext_pins.CLK); // ignoring the deglitcher here
  ///*p01.AVET*/ wire AVET_AxCxExGx = ext_pins.CLK;

  /*p01.UNUT*/ wire TIMEOUT = and(rst_reg.TUBO_CLKREQn_LATCH, tim_reg.UPOF_DIV_15);
  /*p01.TABA*/ wire TABA_RST = or (UNOR_MODE_DBG2n(), UMUT_MODE_DBG1(), TIMEOUT);
  /*p01.ALYP*/ wire ALYP_RSTn = not(TABA_RST);
  /*p01.AFAR*/ wire AFAR_RST = nor(ALYP_RSTn, sys_pins.RST);

  // ASOL has arms on the ground side, output on the top rung - nor latch with inverted output
  /*p01.ASOL*/ rst_reg.ASOL_RST_LATCHp.nor_latch(AFAR_RST, sys_pins.RST); // Schematic wrong, this is a latch.

  //----------------------------------------
  // sch_clocks

  ///*p01.ATAG*/ wire ATAG_xBxDxFxH = not(AZOF_AxCxExGx);
  ///*p01.AMUK*/ wire AMUK_AxCxExGx = not(ATAG_xBxDxFxH); // apu master 4m clock, but apu isn't hooked up yet
  ///*p01.DOVA*/ wire DOVA_xBCDExxx = not(BUDE_AxxxxFGH); // and then this goes to channel 1

  {
    /*p01.AFUR*/ clk_reg.PHAZ_xBCDExxx.set_duo(ATAL_xBxDxFxH(), UPOJ_MODE_PRODn(), !clk_reg.PHAZ_xxxxEFGH.a);
    /*p01.ALEF*/ clk_reg.PHAZ_xxCDEFxx.set_duo(ATAL_xBxDxFxH(), UPOJ_MODE_PRODn(), clk_reg.PHAZ_xBCDExxx.a);
    /*p01.APUK*/ clk_reg.PHAZ_xxxDEFGx.set_duo(ATAL_xBxDxFxH(), UPOJ_MODE_PRODn(), clk_reg.PHAZ_xxCDEFxx.a);
    /*p01.ADYK*/ clk_reg.PHAZ_xxxxEFGH.set_duo(ATAL_xBxDxFxH(), UPOJ_MODE_PRODn(), clk_reg.PHAZ_xxxDEFGx.a);
  }

  {
    /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(gb.ANOS_AxCxExGx());
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH); // dma, sprite store
    wire FROM_CPU5 = gb.cpu_pins.FROM_CPU5;
    /*p04.MAKA*/ FROM_CPU5_SYNC.set(ZEME_AxCxExGx, CUNU_RSTn(), FROM_CPU5);
  }


#if 0
  // if rung 6 of AFUR/ALEF/APUK/ADYK is QN and not Q...

  ADYK = !APUK
  APUK = !ALEF
  ALEF = !AFUR
  AFUR = ADYK

  ADYK APUK ALEF AFUR
  0    0    0    0
  1    1    1    0
  0    0    1    1
  1    0    0    0
  1    1    1    0

  // yeah doesn't work or make sense.
#endif


  {
    // wave ram write clock
    /*p17.ABUR*/ wire ABUR_xxCDEFGx = not(BUKE_ABxxxxxH());
    /*p17.BORY*/ wire BORY_ABxxxxxH = not(ABUR_xxCDEFGx);
    wave_pins.BORY_ABxxxxxH.set(BORY_ABxxxxxH);
  }

  //----------------------------------------
  // sch_resets

  {

    /*p01.BOMA*/ wire BOMA_xBxxxxxx = not(BOGA_AxCDEFGH());

    /*p01.ASOL*/ wire ASOL_RST = or (AFAR_RST, sys_pins.RST);
    /*p01.AFER*/ rst_reg.RESET_REGp.set(BOMA_xBxxxxxx, UPOJ_MODE_PRODn(), ASOL_RST);
    ///*p01.AVOR*/ wire AVOR_RST = or (rst_reg.RESET_REGp, ASOL_RST);
  }


  {
    // Latch w/ arms on the ground side, output on the top rung - nor latch with inverted output
    // TUBO00 << cpu_pins.CLKREQ
    // TUBO01 nc
    // TUBO02 >> UNUT 
    // TUBO03 == nc
    // TUBO04 nc
    // TUBO05 << UPYF
    /*p01.UCOB*/ wire UCOB_CLKBAD = not(sys_pins.CLK_GOOD);
    /*p01.UPYF*/ wire UPYF = or (sys_pins.RST, UCOB_CLKBAD);
    /*p01.TUBO*/ rst_reg.TUBO_CLKREQn_LATCH.nor_latch(cpu_pins.CLKREQ, UPYF);
  }

  //----------------------------------------
  // sch_clocks

  {
    /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH());
    /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
    /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH); // dma, sprite store
    /*p29.XYVA*/ wire XYVA_AxCxExGx = not(ZEME_AxCxExGx);
    /*p29.XOTA*/ wire XOTA_xBxDxFxH = not(XYVA_AxCxExGx);
    /*p29.XYFY*/ wire XYFY_AxCxExGx = not(XOTA_xBxDxFxH);
    /*p29.WUVU*/ vck_reg.WUVU_AxxDExxH.set(XOTA_xBxDxFxH, XAPO_VID_RSTn(), !vck_reg.WUVU_AxxDExxH);
    /*p21.VENA*/ vck_reg.VENA_xBCDExxx.set(!vck_reg.WUVU_AxxDExxH, XAPO_VID_RSTn(), !vck_reg.VENA_xBCDExxx);
    /*p29.WOSU*/ vck_reg.WOSU_xxCDxxGH.set(XYFY_AxCxExGx, XAPO_VID_RSTn(), !vck_reg.WUVU_AxxDExxH);
  }

  {
    /*p07.UBET*/ wire UBET_T1n       = not(sys_pins.T1);
    /*p07.UNOR*/ wire UNOR_MODE_DBG2 = and(sys_pins.T2, UBET_T1n);
    /*p25.SYCY*/ wire SYCY_DBG_CLOCK = not(UNOR_MODE_DBG2);
    /*p25.SOTO*/ bus_reg.SOTO_DBG.set(SYCY_DBG_CLOCK, CUNU_RSTn(), !bus_reg.SOTO_DBG);
  }

  //----------------------------------------

  dma_reg.tick(*this);
  tick_timer();
  tick_serial(cpu_pins);
  tick_joypad();
  tick_ppu();
  sst_reg.tick(*this);
  tick_lcd();

  tick_pixpipe();
  tick_cpu_interrupts();

  tick_vram_addr();

  tick_bootrom();
  tick_cart_data();
  tick_cart_pins();

  tick_oam();
}

//-----------------------------------------------------------------------------
