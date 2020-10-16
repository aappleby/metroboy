#include "GateBoyLib/Sch_Top.h"
#include <memory.h>
#include <stdio.h>

#include "GateBoyLib/Probe.h"

using namespace Schematics;

#pragma warning(disable:4100)

//-----------------------------------------------------------------------------

void SchematicTop::tick_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  const auto& top = *this;

  dma_reg.tick();

  {
    /*p07.UBET*/ wire UBET_T1p = not1(T1n);
    /*p07.UVAR*/ wire UVAR_T2p = not1(T2n);
    /*p07.UMUT*/ UMUT_MODE_DBG1p = and2(T1n, UVAR_T2p);
    /*p07.UNOR*/ UNOR_MODE_DBG2p = and2(T2n, UBET_T1p);
    /*p07.UPOJ*/ UPOJ_MODE_PRODn = nand3(UBET_T1p, UVAR_T2p, RST);

    /*p25.TUTO*/ TUTO_DBG_VRAMp = and2(UNOR_MODE_DBG2p, SOTO_DBG_VRAM.qn16());
  }

  {
    // This is def broken, but how do I fix it?

    /*p??.APAP*/ wire APAP_AVn = not1(cpu_bus.PIN_CPU_ADDR_EXTp.qp()); // Missing from schematic
    /*p01.AWOD*/ wire AWOD_AVp = nor2(UNOR_MODE_DBG2p, APAP_AVn);
    /*p01.ABUZ*/ ABUZ_AVn = not1(AWOD_AVp);

    /*#p01.AGUT*/ wire AGUT_xxCDEFGH = or_and3(top.clk_reg.AROV_xxCDEFxx.qp(), top.clk_reg.AJAX_xxxxEFGH.qp(), top.cpu_bus.PIN_CPU_ADDR_EXTp.qp());
    /*#p01.AWOD*/ wire AWOD_ABxxxxxx = nor2(top.UNOR_MODE_DBG2p, AGUT_xxCDEFGH);
    /*#p01.ABUZ*/ ABUZ_xxCDEFGH = not1(AWOD_ABxxxxxx);
  }

  {
    /* p07.UJYV*/ wire UJYV_CPU_RDn = mux2n(UNOR_MODE_DBG2p, ext_bus.PIN_EXT_RDn.qn(), cpu_bus.PIN_CPU_RDp.qp());
    /* p07.TEDO*/ TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    /* p07.AJAS*/ wire AJAS_CPU_RDn = not1(TEDO_CPU_RDp);
    /* p07.ASOT*/ wire ASOT_CPU_RDp = not1(AJAS_CPU_RDn);
    /* p04.DECY*/ wire DECY_LATCH_EXTn = not1(cpu_bus.PIN_CPU_LATCH_EXT.qp());
    /* p04.CATY*/ wire CATY_LATCH_EXTp = not1(DECY_LATCH_EXTn);
    /* p28.MYNU*/ wire MYNU_CPU_RDn = nand2(ASOT_CPU_RDp, CATY_LATCH_EXTp);
    /* p28.LEKO*/ LEKO_CPU_RDp = not1(MYNU_CPU_RDn);
  }

  clk_reg.tick_slow(CLK, CLKGOOD, CPUREADY, *this);

  sprite_scanner.tick(*this);
  sprite_store.tick(*this);

  lcd_reg.tick(*this);

  pix_pipe.tick(*this);

  {
    /*p27.ROMO*/ wire ROMO_PRELOAD_DONEn = not1(tile_fetcher.POKY_PRELOAD_LATCHp.qp04());
    /*p27.SUVU*/ wire SUVU_PRELOAD_DONE_TRIGn = nand4(pix_pipe.XYMU_RENDERINGn.qn03(),
                                                      ROMO_PRELOAD_DONEn,
                                                      tile_fetcher.NYKA_FETCH_DONE_P11.qp17(),
                                                      tile_fetcher.PORY_FETCH_DONE_P12.qp17());
    // -> sprite fetcher, top.TEVO
    /*p27.TAVE*/ TAVE_PRELOAD_DONE_TRIGp = not1(SUVU_PRELOAD_DONE_TRIGn);

    /*p27.SYLO*/ wire SYLO_WIN_HITn = not1(pix_pipe.RYDY);
    /*p27.TUXY*/ wire TUXY_WIN_FIRST_TILE_NE = nand2(SYLO_WIN_HITn, pix_pipe.SOVY_WIN_FIRST_TILE_B.qp17());
    /*p27.SUZU*/ wire SUZU_WIN_FIRST_TILEne = not1(TUXY_WIN_FIRST_TILE_NE);

    // -> ppu.PASO, window.VETU, top.NYXU_TILE_FETCHER_RSTn
    /*p27.TEVO*/ TEVO_FETCH_TRIGp = or3(pix_pipe.SEKO_WIN_TILE_TRIG,
                                        SUZU_WIN_FIRST_TILEne,
                                        TAVE_PRELOAD_DONE_TRIGp); // Schematic wrong, this is OR
  }

  {
    /*#p01.ATAR*/ wire ATAR_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
    /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(lcd_reg.ATEJ_LINE_TRIGp, ATAR_VID_RSTp);
    /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_B.qp17(), BALU_LINE_RSTp, sprite_scanner.BYBA_SCAN_DONE_A.qn16());
    /*#p29.AVAP*/ AVAP_RENDER_START_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
  }

  {
    /*p27.NYFO*/ wire NYFO_WIN_FETCH_TRIGn = not1(pix_pipe.NUNY_WX_MATCH_TRIGp);
    /*p27.MOSU*/ wire MOSU_WIN_FETCH_TRIGp = not1(NYFO_WIN_FETCH_TRIGn);

    // pxp.loze, pxp.luxa, tile.lony/lovy/laxu/mesu/nyva/moce
    // low on phase 0 of bg tile fetch
    /*p27.NYXU*/ NYXU_FETCH_TRIGn = nor3(AVAP_RENDER_START_TRIGp, MOSU_WIN_FETCH_TRIGp, TEVO_FETCH_TRIGp);
  }

  {
    /*p01.AFAS*/ wire AFAS_xxxxEFGx = nor2(clk_reg.ADAR_ABCxxxxH, clk_reg.ATYP_ABCDxxxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDxxxH = nand2(cpu_bus.PIN_CPU_WRp.qp(), AFAS_xxxxEFGx);
    /*p01.APOV*/ APOV_CPU_WRp_xxxxEFGx = not1(AREV_CPU_WRn_ABCDxxxH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCDxxxH = mux2n(UNOR_MODE_DBG2p, ext_bus.PIN_EXT_WRn.qn(), APOV_CPU_WRp_xxxxEFGx);
    /*p07.TAPU*/ TAPU_CPU_WRp_xxxxEFGx = not1(UBAL_CPU_WRn_ABCDxxxH); // boot.TUGE, int.REFA, joy.ATOZ, ser.URYS/UWAM, timer.TAPE/TOPE/TYJU/SARA, top.DYKY
  }

  {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not1(bootrom.BOOT_BITn.qp17());
    /*p07.TUTU*/ TUTU_ADDR_BOOTp = and2(TERA_BOOT_BITp, cpu_bus.TULO_ADDR_00XXp());
  }

  tim_reg.tick(*this);
  joypad.tick(*this);
  tile_fetcher.tick(*this);
  sprite_fetcher.tick(*this);

  oam_bus.tick(*this);
}

//------------------------------------------------------------------------------

void SchematicTop::tock_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY) {
  const auto& top = *this;

  {
    /*#p25.SYCY*/ wire SYCY_DBG_CLOCKn = not1(UNOR_MODE_DBG2p);
    /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(top.clk_reg.ALUR_SYS_RSTn);
    /*#p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);
    /*#p25.SOTO*/ SOTO_DBG_VRAM.dff17(SYCY_DBG_CLOCKn, CUNU_SYS_RSTn, SOTO_DBG_VRAM.qn16());
  }

  clk_reg.tock_clk_slow(RST, CLK, CLKGOOD, CPUREADY, *this);
  clk_reg.tock_rst_slow(RST, CLKGOOD, CPUREADY, *this);
  clk_reg.tock_dbg_slow(*this);
  clk_reg.tock_vid_slow(CLK, *this);

  tim_reg.tock(RST, *this, cpu_bus);
  bootrom.tock(*this, cpu_bus);

  dma_reg.tock(*this, cpu_bus);


  ser_reg.tock(*this, cpu_bus);
  joypad.tock(*this, cpu_bus);
  sprite_scanner.tock(*this);



  lcd_reg.tock(*this, cpu_bus);

  sprite_store.tock(*this);


  pix_pipe.tock(*this, cpu_bus);
  sprite_fetcher.tock(*this);
  tile_fetcher.tock(*this);
  int_reg.tock(*this, cpu_bus);

  cpu_bus.PIN_CPU_ADDR_HIp.set(cpu_bus.SYRO_FE00_FFFFp());
  cpu_bus.PIN_CPU_BOOTp.set(TUTU_ADDR_BOOTp);


  ext_bus.tock(*this);

  oam_bus.tock(*this);
  vram_bus.tock(*this);

  {
    // IE is technically in the CPU, but we're going to implement it here for now.
    wire FFFF_HIT = cpu_bus.get_bus_addr() == 0xFFFF;

    //if (FFFF_HIT) printf("FFFF_HIT\n");

    wire FFFF_WRn = nand2(TAPU_CPU_WRp_xxxxEFGx, FFFF_HIT);

    IE_D0.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D0p.qp());
    IE_D1.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D1p.qp());
    IE_D2.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D2p.qp());
    IE_D3.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D3p.qp());
    IE_D4.dff(FFFF_WRn, !RST, cpu_bus.BUS_CPU_D4p.qp());

    wire FFFF_RDn = nand2(TEDO_CPU_RDp, FFFF_HIT);

    cpu_bus.BUS_CPU_D0p.tri_6nn(FFFF_RDn, IE_D0.qn());
    cpu_bus.BUS_CPU_D1p.tri_6nn(FFFF_RDn, IE_D1.qn());
    cpu_bus.BUS_CPU_D2p.tri_6nn(FFFF_RDn, IE_D2.qn());
    cpu_bus.BUS_CPU_D3p.tri_6nn(FFFF_RDn, IE_D3.qn());
    cpu_bus.BUS_CPU_D4p.tri_6nn(FFFF_RDn, IE_D4.qn());
  }

  lcd_pix_lo.nor_latch(PIN_LCD_DATA0.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());
  lcd_pix_hi.nor_latch(PIN_LCD_DATA1.qp(), PIN_LCD_CLOCK.qp() | PIN_LCD_HSYNC.qp());

  for (int i = 0; i < 159; i++) {
    lcd_pipe_lo[i].dff(PIN_LCD_CLOCK.qp(), lcd_pipe_lo[i + 1].qp());
    lcd_pipe_hi[i].dff(PIN_LCD_CLOCK.qp(), lcd_pipe_hi[i + 1].qp());
    lcd_line_lo[i].dff(PIN_LCD_LATCH.qp(), lcd_pipe_lo[i + 1].qp());
    lcd_line_hi[i].dff(PIN_LCD_LATCH.qp(), lcd_pipe_hi[i + 1].qp());
  }

  lcd_pipe_lo[159].dff(PIN_LCD_CLOCK.qp(), lcd_pix_lo.qp04());
  lcd_pipe_hi[159].dff(PIN_LCD_CLOCK.qp(), lcd_pix_hi.qp04());
  lcd_line_lo[159].dff(PIN_LCD_LATCH.qp(), lcd_pix_lo.qp04());
  lcd_line_hi[159].dff(PIN_LCD_LATCH.qp(), lcd_pix_hi.qp04());
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_ext_bus(wire RST, uint8_t* cart_rom, uint8_t* cart_ram, uint8_t* ext_ram) {
  if (RST) return;

  uint16_t ext_addr = ext_bus.get_pin_addr();

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = ext_bus.PIN_EXT_RDn.qp();
    wire CEn = ext_bus.PIN_EXT_A15p.qp();

    if (!CEn && !OEn) {
      if (cart_rom) {
        ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
      else {
        printf("no cart loaded!\n");
      }
    }
  }

  // Ext RAM read/write (also echo RAM)
  {
    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = ext_bus.PIN_EXT_WRn.qp();
    wire CE1n = ext_bus.PIN_EXT_CSn.qp();
    wire CE2  = ext_bus.PIN_EXT_A14p.qp();
    wire OEn  = ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CE1n && CE2 && !WRn) {
      ext_ram[ram_addr] = ext_bus.get_pin_data();
    }

    // Read
    if (!CE1n && CE2 && WRn && !OEn) {
      ext_bus.set_pin_data(ext_ram[ram_addr]);
    }
  }

  // Cart RAM read/write
  {
    // A000-BFFF
    // 0b101xxxxxxxxxxxxx

    uint16_t ram_addr = (ext_addr & 0x1FFF);

    wire WRn  = ext_bus.PIN_EXT_WRn.qp();
    wire CS1n = ext_bus.PIN_EXT_CSn.qp();
    wire CS2  = ext_bus.PIN_EXT_A13p.qp() && !ext_bus.PIN_EXT_A14p.qp() && ext_bus.PIN_EXT_A15p.qp();
    wire OEn = ext_bus.PIN_EXT_RDn.qp();

    // Write
    if (!CS1n && CS2 && !WRn) {
      cart_ram[ram_addr] = ext_bus.get_pin_data();
    }

    // Read
    if (!CS1n && CS2 && !OEn) {
      ext_bus.set_pin_data(cart_ram[ram_addr]);
    }
  }

  // FIXME - implement MBC1

  // 0000-3FFF - ROM Bank 00 (Read Only) This area always contains the first 16KBytes of the cartridge ROM.
  // 4000-7FFF - ROM Bank 01-7F (Read Only) This area may contain any of the further 16KByte banks of the ROM, allowing to address up to 125 ROM Banks (almost 2MByte). As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
  // A000-BFFF - RAM Bank 00-03, if any (Read/Write) This area is used to address external RAM in the cartridge (if any). External RAM is often battery buffered, allowing to store game positions or high score tables, even if the gameboy is turned off, or if the cartridge is removed from the gameboy. Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).

  // 0000-1FFF - RAM Enable (Write Only)   00h  Disable RAM (default)   ?Ah  Enable RAM Practically any value with 0Ah in the lower 4 bits enables RAM, and any other value disables RAM.
  // 2000-3FFF - ROM Bank Number (Write Only) Writing to this address space selects the lower 5 bits of the ROM Bank Number (in range 01-1Fh). When 00h is written, the MBC translates that to bank 01h also. That doesn't harm so far, because ROM Bank 00h can be always directly accessed by reading from 0000-3FFF.
  // But (when using the register below to specify the upper ROM Bank bits), the same happens for Bank 20h, 40h, and 60h. Any attempt to address these ROM Banks will select Bank 21h, 41h, and 61h instead.
  // 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only) This 2bit register can be used to select a RAM Bank in range from 00-03h, or to specify the upper two bits (Bit 5-6) of the ROM Bank number, depending on the current ROM/RAM Mode. (See below.)
  // 6000-7FFF - ROM/RAM Mode Select (Write Only)  00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)   01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)

  // MBC1_RAM_EN

  // MBC1_BANK_D0
  // MBC1_BANK_D1
  // MBC1_BANK_D2
  // MBC1_BANK_D3
  // MBC1_BANK_D4
  // MBC1_BANK_D5
  // MBC1_BANK_D6

  // MBC1_MODE

  /*
  {

    bool bank_0 = nor(MBC1_BANK_D0, MBC1_BANK_D1, MBC1_BANK_D2, MBC1_BANK_D3, MBC1_BANK_D4);

    wire cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp();
    wire cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp();
    wire cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp();
    wire cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp();
    wire cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp();
    wire cart_rom_a19 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp();
    wire cart_rom_a20 = MBC1_MODE.qp() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp();

    wire cart_ram_a13 = MBC1_MODE.qp() ? MBC1_BANK_D5.qp() : 0;
    wire cart_ram_a14 = MBC1_MODE.qp() ? MBC1_BANK_D6.qp() : 0;

    // ROM read
    {
      uint16_t rom_addr = ext_addr & 0x7FFF;
      wire OEn = top.ext_bus.PIN_EXT_RDn.qp();
      wire CEn = top.ext_bus.PIN_EXT_A15p.qp();

      if (!CEn && !OEn) {
        top.ext_bus.set_pin_data(cart_rom[rom_addr]);
      }
    }
  }
  */
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_vram_bus(wire RST, uint8_t* vid_ram) {
  if (RST) return;

  int vram_addr = vram_bus.get_pin_addr();
  uint8_t& vram_data = vid_ram[vram_addr];

  // We're getting a fake write on the first phase because PIN_VRAM_WRn resets to 0...
  // ignore it if we're in reset

  if (!RST) {
    if (!vram_bus.PIN_VRAM_WRn.qp()) {
      vram_data = (uint8_t)vram_bus.get_pin_data();
    }

    if (!vram_bus.PIN_VRAM_OEn.qp()) {
      vram_bus.set_pin_data_in(vram_data);
    }
    else {
      vram_bus.set_pin_data_z();
    }
  }
}

//-----------------------------------------------------------------------------
// FIXME This should be using PIN_OAM_CLK (which might actually be PIN_OAM_CSn?)

void SchematicTop::tock_oam_bus(wire RST, uint8_t* oam_ram) {
  if (RST) return;

  uint16_t oam_addr = oam_bus.get_oam_pin_addr();
  uint8_t& oam_data_a = oam_ram[(oam_addr << 1) + 0];
  uint8_t& oam_data_b = oam_ram[(oam_addr << 1) + 1];

  if (!oam_bus.PIN_OAM_WR_A.qp()) oam_data_a = oam_bus.get_oam_pin_data_a();
  if (!oam_bus.PIN_OAM_WR_B.qp()) oam_data_b = oam_bus.get_oam_pin_data_b();

  if (!oam_bus.PIN_OAM_OE.qp()) oam_bus.set_pin_data_a(oam_data_a);
  if (!oam_bus.PIN_OAM_OE.qp()) oam_bus.set_pin_data_b(oam_data_b);
}

//-----------------------------------------------------------------------------

void SchematicTop::tock_zram_bus(wire RST, uint8_t* zero_ram) {
  if (RST) return;

  // ZRAM control signals are

  // top.clk_reg.PIN_CPU_BUKE_AxxxxxGH
  // top.TEDO_CPU_RDp();
  // top.TAPU_CPU_WRp_xxxxEFGx()
  // top.cpu_bus.SYKE_FF00_FFFFp()

  // and there's somes gates WUTA/WOLY/WALE that do the check for FFXX && !FFFF

  int addr = cpu_bus.get_bus_addr();
  bool hit_zram = (addr >= 0xFF80) && (addr <= 0xFFFE);

  if (hit_zram) {
    uint8_t& data = zero_ram[addr & 0x007F];
    if (TAPU_CPU_WRp_xxxxEFGx) data = cpu_bus.get_bus_data();
    if (TEDO_CPU_RDp) cpu_bus.set_data(data);
  }
}

//------------------------------------------------------------------------------

#if 0
// Debug stuff I disabled

/*p07.APET*/ wire APET_MODE_DBG = or2(top.clk_reg.UMUT_MODE_DBG1p(), UNOR_MODE_DBG2p); // suggests UMUTp
/*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A05, BUS_CPU_A06, TAPU_CPUWR, ADDR_111111110xx00000);

//----------
// weird debug things, probably not right

/*p05.AXYN*/ wire AXYN_xBCDEFGH = not1(clk_reg.BEDO_Axxxxxxx);
/*p05.ADYR*/ wire ADYR_Axxxxxxx = not1(AXYN_xBCDEFGH);
/*p05.APYS*/ wire APYS_xBCDEFGH = nor4(sys_sig.MODE_DBG2, ADYR_Axxxxxxx);
/*p05.AFOP*/ wire AFOP_Axxxxxxx = not1(APYS_xBCDEFGH);
/*p07.LECO*/ wire LECO_xBCDEFGH = nor4(clk_reg.BEDO_Axxxxxxx, sys_sig.MODE_DBG2);

if (AFOP_Axxxxxxx) set_data(
  /*p05.ANOC*/ not1(top.GND),
  /*p05.ATAJ*/ not1(top.GND),
  /*p05.AJEC*/ not1(top.GND),
  /*p05.ASUZ*/ not1(top.GND),
  /*p05.BENU*/ not1(top.GND),
  /*p05.AKAJ*/ not1(top.GND),
  /*p05.ARAR*/ not1(top.GND),
  /*p05.BEDA*/ not1(top.GND)
);

if (LECO_xBCDEFGH) set_data(
  /*p07.ROMY*/ top.GND,
  /*p07.RYNE*/ top.GND,
  /*p07.REJY*/ top.GND,
  /*p07.RASE*/ top.GND,
  /*p07.REKA*/ top.GND,
  /*p07.ROWE*/ top.GND,
  /*p07.RYKE*/ top.GND,
  /*p07.RARU*/ top.GND
);



//----------
// more debug stuff

/*p25.TUSO*/ wire TUSO = nor4(MODE_DBG2, dff20.PIN_CPU_BOGA_xBCDEFGH);
/*p25.SOLE*/ wire SOLE = not1(TUSO);

if (top.VYPO_GND) bus_out.set_data(
  /*p25.TOVU*/ SOLE,
  /*p25.SOSA*/ SOLE,
  /*p25.SEDU*/ SOLE,
  /*p25.TAXO*/ SOLE,
  /*p25.TAHY*/ SOLE,
  /*p25.TESU*/ SOLE,
  /*p25.TAZU*/ SOLE,
  /*p25.TEWA*/ SOLE
);

///*p05.KORE*/ wire P05_NC0 = nand2(KERU_DBG_FF00_D7, FF60_0);
///*p05.KYWE*/ wire P05_NC1 = nor4 (KERU_DBG_FF00_D7, FF60_0o);

/*p08.LYRA*/ wire DBG_D_RDn = nand2(sys_sig.MODE_DBG2, bus_sig.CBUS_TO_CEXTn);
/*p08.TUTY*/ if (DBG_D_RDn) BUS_CPU_D0p = not1(/*p08.TOVO*/ not1(pins.PIN_D0_C));
/*p08.SYWA*/ if (DBG_D_RDn) BUS_CPU_D1p = not1(/*p08.RUZY*/ not1(pins.PIN_D1_C));
/*p08.SUGU*/ if (DBG_D_RDn) BUS_CPU_D2p = not1(/*p08.ROME*/ not1(pins.PIN_D2_C));
/*p08.TAWO*/ if (DBG_D_RDn) BUS_CPU_D3p = not1(/*p08.SAZA*/ not1(pins.PIN_D3_C));
/*p08.TUTE*/ if (DBG_D_RDn) BUS_CPU_D4p = not1(/*p08.TEHE*/ not1(pins.PIN_D4_C));
/*p08.SAJO*/ if (DBG_D_RDn) BUS_CPU_D5p = not1(/*p08.RATU*/ not1(pins.PIN_D5_C));
/*p08.TEMY*/ if (DBG_D_RDn) BUS_CPU_D6p = not1(/*p08.SOCA*/ not1(pins.PIN_D6_C));
/*p08.ROPA*/ if (DBG_D_RDn) BUS_CPU_D7p = not1(/*p08.RYBA*/ not1(pins.PIN_D7_C));
#endif

// hack, not correct
#if 0
{
  // FF60 debug state
  /*p07.APET*/ wire APET_MODE_DBG = or2(sys_sig.MODE_DBG1, sys_sig.MODE_DBG2);
  /*p07.APER*/ wire FF60_WRn = nand2(APET_MODE_DBG, BUS_CPU_A05, BUS_CPU_A06, bus_sig.TAPU_CPUWR, dec_sig.ADDR_111111110xx00000);

  /*p05.KURA*/ wire FF60_0n = not1(BURO_FF60_0);
  /*p05.JEVA*/ wire FF60_0o = not1(BURO_FF60_0);
  /*p07.BURO*/ BURO_FF60_0 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D0p);
  /*p07.AMUT*/ AMUT_FF60_1 = ff9(FF60_WRn, rst_sig.SYS_RESETn, BUS_CPU_D1p);

  ///*p05.KURA*/ wire FF60_0n = not1(FF60);
  ///*p05.JEVA*/ wire FF60_0o = not1(FF60);
}
#endif


#if 0
// so the address bus is technically a tribuf, but we're going to ignore
// this debug circuit for now.
{
  // If we're in debug mode 2, drive external address bus onto internal address

  /*#p08.KOVA*/ wire KOVA_A00p = not1(PIN_EXT_A00p.qn());
  /* p08.CAMU*/ wire CAMU_A01p = not1(PIN_EXT_A01p.qn());
  /* p08.BUXU*/ wire BUXU_A02p = not1(PIN_EXT_A02p.qn());
  /* p08.BASE*/ wire BASE_A03p = not1(PIN_EXT_A03p.qn());
  /* p08.AFEC*/ wire AFEC_A04p = not1(PIN_EXT_A04p.qn());
  /* p08.ABUP*/ wire ABUP_A05p = not1(PIN_EXT_A05p.qn());
  /* p08.CYGU*/ wire CYGU_A06p = not1(PIN_EXT_A06p.qn());
  /* p08.COGO*/ wire COGO_A07p = not1(PIN_EXT_A07p.qn());
  /* p08.MUJY*/ wire MUJY_A08p = not1(PIN_EXT_A08p.qn());
  /* p08.NENA*/ wire NENA_A09p = not1(PIN_EXT_A09p.qn());
  /* p08.SURA*/ wire SURA_A10p = not1(PIN_EXT_A10p.qn());
  /* p08.MADY*/ wire MADY_A11p = not1(PIN_EXT_A11p.qn());
  /* p08.LAHE*/ wire LAHE_A12p = not1(PIN_EXT_A12p.qn());
  /* p08.LURA*/ wire LURA_A13p = not1(PIN_EXT_A13p.qn());
  /* p08.PEVO*/ wire PEVO_A14p = not1(PIN_EXT_A14p.qn());
  /* p08.RAZA*/ wire RAZA_A15p = not1(PIN_EXT_A15p.qn());

  // KEJO_01 << KOVA_02
  // KEJO_02
  // KEJO_03
  // KEJO_04 << TOVA_02
  // KEJO_05 << KOVA_02
  // KEJO_06
  // KEJO_07
  // KEJO_08
  // KEJO_09 >> BUS_CPU_A00p
  // KEJO_10

  /* p08.KEJO*/ top.cpu_bus.BUS_CPU_A00 = tribuf_10np(TOVA_MODE_DBG2n, KOVA_A00p);
  /* p08.BYXE*/ top.cpu_bus.BUS_CPU_A01 = tribuf_10np(TOVA_MODE_DBG2n, CAMU_A01p);
  /* p08.AKAN*/ top.cpu_bus.BUS_CPU_A02 = tribuf_10np(TOVA_MODE_DBG2n, BUXU_A02p);
  /* p08.ANAR*/ top.cpu_bus.BUS_CPU_A03 = tribuf_10np(TOVA_MODE_DBG2n, BASE_A03p);
  /* p08.AZUV*/ top.cpu_bus.BUS_CPU_A04 = tribuf_10np(TOVA_MODE_DBG2n, AFEC_A04p);
  /* p08.AJOV*/ top.cpu_bus.BUS_CPU_A05 = tribuf_10np(TOVA_MODE_DBG2n, ABUP_A05p);
  /* p08.BYNE*/ top.cpu_bus.BUS_CPU_A06 = tribuf_10np(TOVA_MODE_DBG2n, CYGU_A06p);
  /* p08.BYNA*/ top.cpu_bus.BUS_CPU_A07 = tribuf_10np(TOVA_MODE_DBG2n, COGO_A07p);
  /* p08.LOFA*/ top.cpu_bus.BUS_CPU_A08 = tribuf_10np(TOVA_MODE_DBG2n, MUJY_A08p);
  /* p08.MAPU*/ top.cpu_bus.BUS_CPU_A09 = tribuf_10np(TOVA_MODE_DBG2n, NENA_A09p);
  /* p08.RALA*/ top.cpu_bus.BUS_CPU_A10 = tribuf_10np(TOVA_MODE_DBG2n, SURA_A10p);
  /* p08.LORA*/ top.cpu_bus.BUS_CPU_A11 = tribuf_10np(TOVA_MODE_DBG2n, MADY_A11p);
  /* p08.LYNA*/ top.cpu_bus.BUS_CPU_A12 = tribuf_10np(TOVA_MODE_DBG2n, LAHE_A12p);
  /* p08.LEFY*/ top.cpu_bus.BUS_CPU_A13 = tribuf_10np(TOVA_MODE_DBG2n, LURA_A13p);
  /* p08.NEFE*/ top.cpu_bus.BUS_CPU_A14 = tribuf_10np(TOVA_MODE_DBG2n, PEVO_A14p);
  /* p08.SYZU*/ top.cpu_bus.BUS_CPU_A15 = tribuf_10np(TOVA_MODE_DBG2n, RAZA_A15p);
}
#endif

//-----------------------------------------------------------------------------
