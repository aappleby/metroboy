#include "GateBoyLib/GateBoyExtBus.h"

#include "GateBoyLib/GateBoy.h"

//----------------------------------------
// FIXME - implement MBC1

// ROM read
//wire rom_CEn = PIN_01_ADDR[15].ext_qp_new2();
//wire rom_OEp = bit(~rom_CEn) && bit(~PIN_79_RDn.ext_qp_new2()) && cart_buf;
// Cart RAM read
//wire cram_CS1n = PIN_80_CSn.ext_qp_new2();
//wire cram_CS2p = PIN_01_ADDR[13].ext_qp_new2() && bit(~PIN_01_ADDR[14].ext_qp_new2()) && PIN_01_ADDR[15].ext_qp_new2();
//wire cram_OEp  = bit(~cram_CS1n) && cram_CS2p && bit(~PIN_79_RDn.ext_qp_new2());
// Internal RAM read
//wire iram_CS1n = PIN_80_CSn.qp_new();
//wire iram_CS2p = PIN_01_ADDR[14].qp_new();
//wire iram_OEp  = bit(~iram_CS1n) && iram_CS2p && bit(~PIN_79_RDn.qp_new());
// Echo RAM read
//wire eram_CS1n = PIN_80_CSn.qp_new();
//wire eram_CS2p = PIN_01_ADDR[14].qp_new();
//wire eram_OEp  = bit(~eram_CS1n) && eram_CS2p && bit(~PIN_79_RDn.qp_new());

// Cart RAM write
//wire cram_CS1n = PIN_80_CSn.qp_new();
//wire cram_CS2p = PIN_01_ADDR[13].qp_new() && bit(~PIN_01_ADDR[14].qp_new()) && PIN_01_ADDR[15].qp_new();
//wire cram_WRp  = bit(~cram_CS1n) && cram_CS2p && bit(~PIN_78_WRn.qp_new());
// Internal RAM write
//wire iram_CS1n = PIN_80_CSn.qp_new();
//wire iram_CS2p = PIN_01_ADDR[14].qp_new();
//wire iram_WRp  = bit(~iram_CS1n) && iram_CS2p && bit(~PIN_78_WRn.qp_new());
// Echo RAM write
//wire eram_CS1n = PIN_80_CSn.qp_new();
//wire eram_CS2p = PIN_01_ADDR[14].qp_new();
//wire eram_WRp  = bit(~eram_CS1n) && iram_CS2p && bit(~PIN_78_WRn.qp_new());


// 0000-3FFF - ROM Bank 00 (Read Only) This area always contains the first 16KBytes of the cartridge ROM.
// 4000-7FFF - ROM Bank 01-7F (Read Only) This area may contain any of the further 16KByte banks of the ROM, allowing to address up to 125 ROM Banks (almost 2MByte). As described below, bank numbers 20h, 40h, and 60h cannot be used, resulting in the odd amount of 125 banks.
// A000-BFFF - RAM Bank 00-03, if any (Read/Write) This area is used to address external RAM in the cartridge (if any). External RAM is often battery buffered, allowing to store game positions or high score tables, even if the gameboy is turned off, or if the cartridge is removed from the gameboy. Available RAM sizes are: 2KByte (at A000-A7FF), 8KByte (at A000-BFFF), and 32KByte (in form of four 8K banks at A000-BFFF).

// 0000-1FFF - RAM Enable (Write Only)   00h  Disable RAM (default)   ?Ah  Enable RAM Practically any value with 0Ah in the lower 4 bits enables RAM, and any other value disables RAM.
// 2000-3FFF - ROM Bank Number (Write Only)
// Writing to this address space selects the lower 5 bits of the ROM Bank Number (in range 01-1Fh).
// When 00h is written, the MBC translates that to bank 01h also.
// That doesn't harm so far, because ROM Bank 00h can be always directly accessed by reading from 0000-3FFF.
// But (when using the register below to specify the upper ROM Bank bits), the same happens for Bank 20h, 40h, and 60h.
// Any attempt to address these ROM Banks will select Bank 21h, 41h, and 61h instead.

// 4000-5FFF - RAM Bank Number - or - Upper Bits of ROM Bank Number (Write Only) This 2bit register can be used to select a RAM Bank in range from 00-03h, or to specify the upper two bits (Bit 5-6) of the ROM Bank number, depending on the current ROM/RAM Mode. (See below.)
// 6000-7FFF - ROM/RAM Mode Select (Write Only)  00h = ROM Banking Mode (up to 8KByte RAM, 2MByte ROM) (default)   01h = RAM Banking Mode (up to 32KByte RAM, 512KByte ROM)

/*
{

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = PIN_79_EXT_RDn.qp_new();
    wire CEn = PIN_01_EXT_A[15].qp_new();

    if (!CEn && !OEn) {
      ext_pins.set_pin_data(cart_rom[rom_addr]);
    }
  }
}
*/

/*
void MetroBoyCart::tick(int phase_total, const Req& req, Ack& ack) const {
  (void)phase_total;

  const int region = req.addr >> 13;
  const uint16_t ram_addr = req.addr & 0x1FFF;
  const uint16_t rom_addr = req.addr & 0x7FFF;

  if (req.read) {
    if (region == 2 || region == 3) {
      int rom_bank = ((bank_latch2 << 5) | bank_latch1);
      if (bank_latch1 == 0) rom_bank |= 0b00000001;
      rom_bank &= (rom_bank_count - 1);
      ack.data_lo = cart_rom[(rom_bank << 14) | (rom_addr & 0x3FFF)];
    }
    else if (region == 5) {
      // cart_ram
      if (ram_enable && ram_bank_count) {
        int ram_bank = mode ? bank_latch2 : 0;
        ram_bank &= (ram_bank_count - 1);
        if (ram_bank_count == 0) ram_bank = 0;
        ack.data_lo = cart_ram[(ram_bank << 13) | ram_addr];
      }
    }
  }
}
*/

//------------------------------------------------------------------------------------------------------------------------

#pragma warning(disable:4189) // unused local

void GateBoy::tock_ext()
{
  ext_pins.PIN_80_CSn.reset_for_pass();
  ext_pins.PIN_79_RDn.reset_for_pass();
  ext_pins.PIN_78_WRn.reset_for_pass();

  ext_pins.PIN_01_A00.reset_for_pass();
  ext_pins.PIN_02_A01.reset_for_pass();
  ext_pins.PIN_03_A02.reset_for_pass();
  ext_pins.PIN_04_A03.reset_for_pass();
  ext_pins.PIN_05_A04.reset_for_pass();
  ext_pins.PIN_06_A05.reset_for_pass();
  ext_pins.PIN_07_A06.reset_for_pass();
  ext_pins.PIN_08_A07.reset_for_pass();
  ext_pins.PIN_09_A08.reset_for_pass();
  ext_pins.PIN_10_A09.reset_for_pass();
  ext_pins.PIN_11_A10.reset_for_pass();
  ext_pins.PIN_12_A11.reset_for_pass();
  ext_pins.PIN_13_A12.reset_for_pass();
  ext_pins.PIN_14_A13.reset_for_pass();
  ext_pins.PIN_15_A14.reset_for_pass();
  ext_pins.PIN_16_A15.reset_for_pass();

  ext_pins.PIN_17_D00.reset_for_pass();
  ext_pins.PIN_18_D01.reset_for_pass();
  ext_pins.PIN_19_D02.reset_for_pass();
  ext_pins.PIN_20_D03.reset_for_pass();
  ext_pins.PIN_21_D04.reset_for_pass();
  ext_pins.PIN_22_D05.reset_for_pass();
  ext_pins.PIN_23_D06.reset_for_pass();
  ext_pins.PIN_24_D07.reset_for_pass();

  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(TEXO_ADDR_VRAMn(), UMUT_MODE_DBG1p());

  /*#p04.LEBU*/ wire _LEBU_DMA_A15n  = not1(dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire _MUDA_DMA_VRAMp = nor3(dma.PULA_DMA_A13n.qn_new(), dma.POKU_DMA_A14n.qn_new(), _LEBU_DMA_A15n);
  /* p04.LOGO*/ wire _LOGO_DMA_VRAMn = not1(_MUDA_DMA_VRAMp);
  /* p04.MORY*/ wire _MORY_DMA_CARTn = nand2(dma.MATU_DMA_RUNNINGp.qp_new(), _LOGO_DMA_VRAMn);
  /* p04.LUMA*/ wire _LUMA_DMA_CARTp = not1(_MORY_DMA_CARTn);

  {
    /* p08.LAGU*/ wire _LAGU = and_or3(cpu_signals.SIG_CPU_RDp.qp_new(), LEVO_ADDR_VRAMn(), cpu_signals.SIG_CPU_WRp.qp_new());
    /* p08.LYWE*/ wire _LYWE = not1(_LAGU);
    /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
    /* p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(_LUMA_DMA_CARTp, _MOTY_CPU_EXT_RD);
    /* p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, TOVA_MODE_DBG2n());
    /* p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, UNOR_MODE_DBG2p());
    /*PIN_79*/ ext_pins.PIN_79_RDn.pin_out_hilo(_UGAC_RD_A, _URUN_RD_D);
  }

  {
    /* p08.NEVY*/ wire _NEVY = or2(MEXO_CPU_WRn(), _MOCA_DBG_EXT_RD);
    /* p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, _LUMA_DMA_CARTp);
    /* p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, TOVA_MODE_DBG2n());
    /* p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, UNOR_MODE_DBG2p());
    /*PIN_78*/ ext_pins.PIN_78_WRn.pin_out_hilo(_UVER_WR_A, _USUF_WR_D);
  }

  {
    /* p08.TOZA*/ wire _TOZA_CS_A = and3(cpu_signals.ABUZ_EXT_RAM_CS_CLK.qp_new(), new_bus.TYNU_A000_FFFFp(), new_bus.TUNA_0000_FDFF());
    /* p08.TYHO*/ wire _TYHO_CS_A = mux2p(_LUMA_DMA_CARTp, dma.MARU_DMA_A15n.qn_new(), _TOZA_CS_A);
    /*PIN_80*/ ext_pins.PIN_80_CSn.pin_out_dp(_TYHO_CS_A);
  }

  /* p08.LOXO*/ wire _LOXO_HOLDn = and_or3(MULE_MODE_DBG1n(), TEXO_ADDR_VRAMn(), UMUT_MODE_DBG1p());
  /* p08.LASY*/ wire _LASY_HOLDp = not1(_LOXO_HOLDn);
  /* p08.MATE*/ wire _MATE_HOLDn = not1(_LASY_HOLDp);
  /* p08.ALOR*/ ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A00p.qp_new());
  /* p08.APUR*/ ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A01p.qp_new());
  /* p08.ALYR*/ ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A02p.qp_new());
  /* p08.ARET*/ ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A03p.qp_new());
  /* p08.AVYS*/ ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A04p.qp_new());
  /* p08.ATEV*/ ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A05p.qp_new());
  /* p08.AROS*/ ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A06p.qp_new());
  /* p08.ARYM*/ ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A07p.qp_new());
  /* p08.LUNO*/ ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A08p.qp_new());
  /* p08.LYSA*/ ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A09p.qp_new());
  /* p08.PATE*/ ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A10p.qp_new());
  /* p08.LUMY*/ ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A11p.qp_new());
  /* p08.LOBU*/ ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A12p.qp_new());
  /* p08.LONU*/ ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A13p.qp_new());
  /* p08.NYRE*/ ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A14p.qp_new());


  /* p08.AMET*/ wire _AMET_A00p = mux2p(_LUMA_DMA_CARTp, dma.NAKY_DMA_A00p.qp_new(), ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /* p08.ATOL*/ wire _ATOL_A01p = mux2p(_LUMA_DMA_CARTp, dma.PYRO_DMA_A01p.qp_new(), ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /* p08.APOK*/ wire _APOK_A02p = mux2p(_LUMA_DMA_CARTp, dma.NEFY_DMA_A02p.qp_new(), ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /* p08.AMER*/ wire _AMER_A03p = mux2p(_LUMA_DMA_CARTp, dma.MUTY_DMA_A03p.qp_new(), ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /* p08.ATEM*/ wire _ATEM_A04p = mux2p(_LUMA_DMA_CARTp, dma.NYKO_DMA_A04p.qp_new(), ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /* p08.ATOV*/ wire _ATOV_A05p = mux2p(_LUMA_DMA_CARTp, dma.PYLO_DMA_A05p.qp_new(), ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /* p08.ATYR*/ wire _ATYR_A06p = mux2p(_LUMA_DMA_CARTp, dma.NUTO_DMA_A06p.qp_new(), ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(_LUMA_DMA_CARTp, dma.MUGU_DMA_A07p.qp_new(), ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire _MANO_A08p = mux2p(_LUMA_DMA_CARTp, dma.NAFA_DMA_A08n.qn_new(), ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /* p08.MASU*/ wire _MASU_A09p = mux2p(_LUMA_DMA_CARTp, dma.PYNE_DMA_A09n.qn_new(), ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /* p08.PAMY*/ wire _PAMY_A10p = mux2p(_LUMA_DMA_CARTp, dma.PARA_DMA_A10n.qn_new(), ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /* p08.MALE*/ wire _MALE_A11p = mux2p(_LUMA_DMA_CARTp, dma.NYDO_DMA_A11n.qn_new(), ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /* p08.MOJY*/ wire _MOJY_A12p = mux2p(_LUMA_DMA_CARTp, dma.NYGY_DMA_A12n.qn_new(), ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /* p08.MUCE*/ wire _MUCE_A13p = mux2p(_LUMA_DMA_CARTp, dma.PULA_DMA_A13n.qn_new(), ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /* p08.PEGE*/ wire _PEGE_A14p = mux2p(_LUMA_DMA_CARTp, dma.POKU_DMA_A14n.qn_new(), ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

  /* p08.KUPO*/ wire _KUPO = nand2(_AMET_A00p, TOVA_MODE_DBG2n());
  /* p08.CABA*/ wire _CABA = nand2(_ATOL_A01p, TOVA_MODE_DBG2n());
  /* p08.BOKU*/ wire _BOKU = nand2(_APOK_A02p, TOVA_MODE_DBG2n());
  /* p08.BOTY*/ wire _BOTY = nand2(_AMER_A03p, TOVA_MODE_DBG2n());
  /* p08.BYLA*/ wire _BYLA = nand2(_ATEM_A04p, TOVA_MODE_DBG2n());
  /* p08.BADU*/ wire _BADU = nand2(_ATOV_A05p, TOVA_MODE_DBG2n());
  /* p08.CEPU*/ wire _CEPU = nand2(_ATYR_A06p, TOVA_MODE_DBG2n());
  /* p08.DEFY*/ wire _DEFY = nand2(_ASUR_A07p, TOVA_MODE_DBG2n());
  /* p08.MYNY*/ wire _MYNY = nand2(_MANO_A08p, TOVA_MODE_DBG2n());
  /* p08.MUNE*/ wire _MUNE = nand2(_MASU_A09p, TOVA_MODE_DBG2n());
  /* p08.ROXU*/ wire _ROXU = nand2(_PAMY_A10p, TOVA_MODE_DBG2n());
  /* p08.LEPY*/ wire _LEPY = nand2(_MALE_A11p, TOVA_MODE_DBG2n());
  /* p08.LUCE*/ wire _LUCE = nand2(_MOJY_A12p, TOVA_MODE_DBG2n());
  /* p08.LABE*/ wire _LABE = nand2(_MUCE_A13p, TOVA_MODE_DBG2n());
  /* p08.PUHE*/ wire _PUHE = nand2(_PEGE_A14p, TOVA_MODE_DBG2n());

  /* p08.KOTY*/ wire _KOTY = nor2(_AMET_A00p, UNOR_MODE_DBG2p());
  /* p08.COTU*/ wire _COTU = nor2(_ATOL_A01p, UNOR_MODE_DBG2p());
  /* p08.BAJO*/ wire _BAJO = nor2(_APOK_A02p, UNOR_MODE_DBG2p());
  /* p08.BOLA*/ wire _BOLA = nor2(_AMER_A03p, UNOR_MODE_DBG2p());
  /* p08.BEVO*/ wire _BEVO = nor2(_ATEM_A04p, UNOR_MODE_DBG2p());
  /* p08.AJAV*/ wire _AJAV = nor2(_ATOV_A05p, UNOR_MODE_DBG2p());
  /* p08.CYKA*/ wire _CYKA = nor2(_ATYR_A06p, UNOR_MODE_DBG2p());
  /* p08.COLO*/ wire _COLO = nor2(_ASUR_A07p, UNOR_MODE_DBG2p());
  /* p08.MEGO*/ wire _MEGO = nor2(_MANO_A08p, UNOR_MODE_DBG2p());
  /* p08.MENY*/ wire _MENY = nor2(_MASU_A09p, UNOR_MODE_DBG2p());
  /* p08.RORE*/ wire _RORE = nor2(_PAMY_A10p, UNOR_MODE_DBG2p());
  /* p08.LYNY*/ wire _LYNY = nor2(_MALE_A11p, UNOR_MODE_DBG2p());
  /* p08.LOSO*/ wire _LOSO = nor2(_MOJY_A12p, UNOR_MODE_DBG2p());
  /* p08.LEVA*/ wire _LEVA = nor2(_MUCE_A13p, UNOR_MODE_DBG2p());
  /* p08.PAHY*/ wire _PAHY = nor2(_PEGE_A14p, UNOR_MODE_DBG2p());

  /*PIN_01*/ ext_pins.PIN_01_A00.pin_out_hilo(_KUPO, _KOTY);
  /*PIN_02*/ ext_pins.PIN_02_A01.pin_out_hilo(_CABA, _COTU);
  /*PIN_03*/ ext_pins.PIN_03_A02.pin_out_hilo(_BOKU, _BAJO);
  /*PIN_04*/ ext_pins.PIN_04_A03.pin_out_hilo(_BOTY, _BOLA);
  /*PIN_05*/ ext_pins.PIN_05_A04.pin_out_hilo(_BYLA, _BEVO);
  /*PIN_06*/ ext_pins.PIN_06_A05.pin_out_hilo(_BADU, _AJAV);
  /*PIN_07*/ ext_pins.PIN_07_A06.pin_out_hilo(_CEPU, _CYKA);
  /*PIN_08*/ ext_pins.PIN_08_A07.pin_out_hilo(_DEFY, _COLO);
  /*PIN_09*/ ext_pins.PIN_09_A08.pin_out_hilo(_MYNY, _MEGO);
  /*PIN_10*/ ext_pins.PIN_10_A09.pin_out_hilo(_MUNE, _MENY);
  /*PIN_11*/ ext_pins.PIN_11_A10.pin_out_hilo(_ROXU, _RORE);
  /*PIN_12*/ ext_pins.PIN_12_A11.pin_out_hilo(_LEPY, _LYNY);
  /*PIN_13*/ ext_pins.PIN_13_A12.pin_out_hilo(_LUCE, _LOSO);
  /*PIN_14*/ ext_pins.PIN_14_A13.pin_out_hilo(_LABE, _LEVA);
  /*PIN_15*/ ext_pins.PIN_15_A14.pin_out_hilo(_PUHE, _PAHY);

  // A15 is "special"
  /* p07.TERA*/ wire TERA_BOOT_BITp = not1(cpu_signals.TEPU_BOOT_BITn_h.qp_new());
  /* p07.TUTU*/ wire TUTU_READ_BOOTROMp = and2(TERA_BOOT_BITp, new_bus.TULO_ADDR_BOOTROMp());
  /* p08.SOBY*/ wire _SOBY_A15n = nor2 (new_bus.BUS_CPU_A15p.qp_new(), TUTU_READ_BOOTROMp);
  /* p08.SEPY*/ wire _SEPY_A15p = nand2(cpu_signals.ABUZ_EXT_RAM_CS_CLK.qp_new(), _SOBY_A15n);
  /* p08.TAZY*/ wire _TAZY_A15p = mux2p (_LUMA_DMA_CARTp, dma.MARU_DMA_A15n.qn_new(), _SEPY_A15p);
  /* p08.SUZE*/ wire _SUZE_A15n = nand2(_TAZY_A15p, RYCA_MODE_DBG2n());
  /* p08.RULO*/ wire _RULO_A15n = nor2 (_TAZY_A15p, UNOR_MODE_DBG2p());
  /*PIN_16*/ ext_pins.PIN_16_A15.pin_out_hilo(_SUZE_A15n, _RULO_A15n);

  // FIXME So does this mean that if the CPU writes to the external bus during dma, that data_in
  // will actually end up in oam?

  /* p08.LAGU*/ wire _LAGU = and_or3(cpu_signals.SIG_CPU_RDp.qp_new(), LEVO_ADDR_VRAMn(), cpu_signals.SIG_CPU_WRp.qp_new());
  /* p08.LYWE*/ wire _LYWE = not1(_LAGU);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
  /* p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(UNOR_MODE_DBG2p(), REDU_CPU_RDn(), _MOTY_CPU_EXT_RD);
  /* p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

  /* p25.RUXA*/ wire _RUXA = nand2(new_bus.BUS_CPU_D00p.qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RUJA*/ wire _RUJA = nand2(new_bus.BUS_CPU_D01p.qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RABY*/ wire _RABY = nand2(new_bus.BUS_CPU_D02p.qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RERA*/ wire _RERA = nand2(new_bus.BUS_CPU_D03p.qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RORY*/ wire _RORY = nand2(new_bus.BUS_CPU_D04p.qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RYVO*/ wire _RYVO = nand2(new_bus.BUS_CPU_D05p.qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RAFY*/ wire _RAFY = nand2(new_bus.BUS_CPU_D06p.qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RAVU*/ wire _RAVU = nand2(new_bus.BUS_CPU_D07p.qp_new(), _LULA_CBD_TO_EPDp);

  /* p08.RUNE*/ wire _RUNE = nor2 (new_bus.BUS_CPU_D00p.qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RYPU*/ wire _RYPU = nor2 (new_bus.BUS_CPU_D01p.qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.SULY*/ wire _SULY = nor2 (new_bus.BUS_CPU_D02p.qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.SEZE*/ wire _SEZE = nor2 (new_bus.BUS_CPU_D03p.qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RESY*/ wire _RESY = nor2 (new_bus.BUS_CPU_D04p.qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.TAMU*/ wire _TAMU = nor2 (new_bus.BUS_CPU_D05p.qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.ROGY*/ wire _ROGY = nor2 (new_bus.BUS_CPU_D06p.qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RYDA*/ wire _RYDA = nor2 (new_bus.BUS_CPU_D07p.qp_new(), _RORU_CBD_TO_EPDn);

  /*PIN_17*/ ext_pins.PIN_17_D00.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RUXA, _RUNE);
  /*PIN_18*/ ext_pins.PIN_18_D01.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RUJA, _RYPU);
  /*PIN_19*/ ext_pins.PIN_19_D02.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RABY, _SULY);
  /*PIN_20*/ ext_pins.PIN_20_D03.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RERA, _SEZE);
  /*PIN_21*/ ext_pins.PIN_21_D04.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RORY, _RESY);
  /*PIN_22*/ ext_pins.PIN_22_D05.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RYVO, _TAMU);
  /*PIN_23*/ ext_pins.PIN_23_D06.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RAFY, _ROGY);
  /*PIN_24*/ ext_pins.PIN_24_D07.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RAVU, _RYDA);

  ext_pins.MBC1_RAM_EN.hold();
  ext_pins.MBC1_MODE.hold();
  ext_pins.MBC1_BANK0.hold();
  ext_pins.MBC1_BANK1.hold();
  ext_pins.MBC1_BANK2.hold();
  ext_pins.MBC1_BANK3.hold();
  ext_pins.MBC1_BANK4.hold();
  ext_pins.MBC1_BANK5.hold();
  ext_pins.MBC1_BANK6.hold();

  //----------------------------------------
  // region 0 = rom 0
  // region 1 = rom 0
  // region 2 = rom 1
  // region 3 = rom 1
  // region 4 = vram
  // region 5 = cram
  // region 6 = iram
  // region 7 = eram

  uint16_t addr = (uint16_t)BitBase::pack_ext_new(16, &ext_pins.PIN_01_A00);
  const int region = addr >> 13;

  bool mbc1_ram_en = bit(ext_pins.MBC1_RAM_EN.qp_new());
  bool mbc1_mode   = bit(ext_pins.MBC1_MODE.qp_new());

  uint32_t mbc1_rom_bank = mbc1_mode ? BitBase::pack_new(5, &ext_pins.MBC1_BANK0) : BitBase::pack_new(7, &ext_pins.MBC1_BANK0);
  if ((mbc1_rom_bank & 0x1F) == 0) mbc1_rom_bank |= 1;

  uint32_t mbc1_ram_bank = mbc1_mode ? BitBase::pack_new(2, &ext_pins.MBC1_BANK5) : 0;
  if (mbc1_mode == 0) mbc1_ram_bank = 0;

  uint32_t mbc1_rom_addr = ((addr & 0x3FFF) | (mbc1_rom_bank << 14)) & cart_rom_addr_mask;
  uint32_t mbc1_ram_addr = ((addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask;


  if (mbc1_rom_addr >= cart_size) __debugbreak();
  if (mbc1_ram_addr >= 32768)     __debugbreak();

  //----------------------------------------

  bool rd_en = false;
  uint8_t data_out = 0;

  if (bit(~ext_pins.PIN_79_RDn.qp_ext_new())) {
    if (region == 0 || region == 1) {
      rd_en = 1;
      data_out = cart_buf[addr];
    }
    else if ((region == 2 || region == 3) && cart_has_mbc1) {
      rd_en = 1;
      data_out = cart_buf[mbc1_rom_addr];
    }
    else if ((region == 2 || region == 3) && !cart_has_mbc1) {
      rd_en = 1;
      data_out = cart_buf[addr & cart_rom_addr_mask];
    }
    else if (region == 5 && cart_has_ram && cart_has_mbc1) {
      rd_en = mbc1_ram_en;
      data_out = cart_ram[mbc1_ram_addr & cart_ram_addr_mask];
    }
    else if (region == 5 && cart_has_ram && !cart_has_mbc1) {
      rd_en = 1;
      data_out = cart_ram[addr & cart_ram_addr_mask];
    }
    else if (region == 6 || region == 7) {
      rd_en = 1;
      data_out = int_ram[addr & 0x1FFF];
    }
  }

  ext_pins.PIN_17_D00.pin_io_in_oedp(rd_en, (data_out >> 0) & 1);
  ext_pins.PIN_18_D01.pin_io_in_oedp(rd_en, (data_out >> 1) & 1);
  ext_pins.PIN_19_D02.pin_io_in_oedp(rd_en, (data_out >> 2) & 1);
  ext_pins.PIN_20_D03.pin_io_in_oedp(rd_en, (data_out >> 3) & 1);
  ext_pins.PIN_21_D04.pin_io_in_oedp(rd_en, (data_out >> 4) & 1);
  ext_pins.PIN_22_D05.pin_io_in_oedp(rd_en, (data_out >> 5) & 1);
  ext_pins.PIN_23_D06.pin_io_in_oedp(rd_en, (data_out >> 6) & 1);
  ext_pins.PIN_24_D07.pin_io_in_oedp(rd_en, (data_out >> 7) & 1);

  //----------------------------------------

  uint8_t data_in  = (uint8_t)BitBase::pack_ext_new(8, &ext_pins.PIN_17_D00);

  if (bit(~ext_pins.PIN_78_WRn.qp_ext_new())) {
    if (region == 0 && cart_has_mbc1) {
      ext_pins.MBC1_RAM_EN = bit((data_in & 0x0F) == 0x0A);
    }
    else if (region == 1 && cart_has_mbc1) {
      ext_pins.MBC1_BANK0 = bit(data_in, 0);
      ext_pins.MBC1_BANK1 = bit(data_in, 1);
      ext_pins.MBC1_BANK2 = bit(data_in, 2);
      ext_pins.MBC1_BANK3 = bit(data_in, 3);
      ext_pins.MBC1_BANK4 = bit(data_in, 4);
    }
    else if (region == 2 && cart_has_mbc1) {
      ext_pins.MBC1_BANK5 = bit(data_in, 0);
      ext_pins.MBC1_BANK6 = bit(data_in, 1);
    }
    else if (region == 3 && cart_has_mbc1) {
      ext_pins.MBC1_MODE = (data_in & 1);
    }
    else if (region == 5 && cart_has_ram && cart_has_mbc1 && mbc1_ram_en) {
      cart_ram[mbc1_ram_addr & cart_ram_addr_mask] = data_in;
    }
    else if (region == 5 && cart_has_ram && !cart_has_mbc1) {
      cart_ram[addr & cart_ram_addr_mask] = data_in;
    }
    else if (region == 6 || region == 7) {
      int_ram[addr & 0x1FFF]  = data_in;
    }
  }

  //----------------------------------------

  /* p08.LAVO*/ wire _LAVO_HOLDn = nand3(cpu_signals.SIG_CPU_RDp.qp_new(), TEXO_ADDR_VRAMn(), cpu_signals.SIG_CPU_LATCH_EXT.qp_new());

  /*#p08.SOMA*/ ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_17_D00.qp_new());
  /* p08.RONY*/ ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_18_D01.qp_new());
  /* p08.RAXY*/ ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_19_D02.qp_new());
  /* p08.SELO*/ ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_20_D03.qp_new());
  /* p08.SODY*/ ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_21_D04.qp_new());
  /* p08.SAGO*/ ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_22_D05.qp_new());
  /* p08.RUPA*/ ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_23_D06.qp_new());
  /* p08.SAZY*/ ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latchn(_LAVO_HOLDn, ext_pins.PIN_24_D07.qp_new());
  /*#p08.RYMA_EL0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_LAVO_HOLDn, ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
  /* p08.RUVO_EL1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_LAVO_HOLDn, ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
  /* p08.RYKO_EL2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_LAVO_HOLDn, ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
  /* p08.TAVO_EL3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_LAVO_HOLDn, ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
  /* p08.TEPE_EL4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_LAVO_HOLDn, ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
  /* p08.SAFO_EL5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_LAVO_HOLDn, ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
  /* p08.SEVU_EL6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_LAVO_HOLDn, ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
  /* p08.TAJU_EL7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_LAVO_HOLDn, ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------
