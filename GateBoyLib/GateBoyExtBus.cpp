#include "GateBoyLib/GateBoyExtBus.h"

#include "GateBoyLib/GateBoy.h"

//----------------------------------------
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

  wire cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp_new();
  wire cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp_new();
  wire cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp_new();
  wire cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp_new();
  wire cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp_new();
  wire cart_rom_a19 = bit(MBC1_MODE.qp_new()) ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp_new();
  wire cart_rom_a20 = bit(MBC1_MODE.qp_new()) ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp_new();

  wire cart_ram_a13 = bit(MBC1_MODE.qp_new()) ? MBC1_BANK_D5.qp_new() : 0;
  wire cart_ram_a14 = bit(MBC1_MODE.qp_new()) ? MBC1_BANK_D6.qp_new() : 0;

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = PIN_79_EXT_RDn.qp_new();
    wire CEn = PIN_01_EXT_A[15].qp_new();

    if (!CEn && !OEn) {
      ext_bus.set_pin_data(cart_rom[rom_addr]);
    }
  }
}
*/

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::copy_cpu_addr_to_addr_latch()
{
  /* p08.LOXO*/ wire _LOXO_HOLDn = and_or3(rst.MULE_MODE_DBG1n(), cpu_bus.TEXO_ADDR_VRAMn(new_bus), rst.UMUT_MODE_DBG1p());
  /* p08.LASY*/ wire _LASY_HOLDp = not1(_LOXO_HOLDn);
  /* p08.MATE*/ wire _MATE_HOLDn = not1(_LASY_HOLDp);
  /* p08.ALOR*/ ext_bus.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A00p.qp_new());
  /* p08.APUR*/ ext_bus.ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A01p.qp_new());
  /* p08.ALYR*/ ext_bus.ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A02p.qp_new());
  /* p08.ARET*/ ext_bus.ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A03p.qp_new());
  /* p08.AVYS*/ ext_bus.ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A04p.qp_new());
  /* p08.ATEV*/ ext_bus.ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A05p.qp_new());
  /* p08.AROS*/ ext_bus.ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A06p.qp_new());
  /* p08.ARYM*/ ext_bus.ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A07p.qp_new());
  /* p08.LUNO*/ ext_bus.ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A08p.qp_new());
  /* p08.LYSA*/ ext_bus.ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A09p.qp_new());
  /* p08.PATE*/ ext_bus.ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A10p.qp_new());
  /* p08.LUMY*/ ext_bus.ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A11p.qp_new());
  /* p08.LOBU*/ ext_bus.ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A12p.qp_new());
  /* p08.LONU*/ ext_bus.ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A13p.qp_new());
  /* p08.NYRE*/ ext_bus.ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.tp_latchn(_MATE_HOLDn, new_bus.BUS_CPU_A14p.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::copy_addr_latch_to_pins()
{
  /* p08.AMET*/ wire _AMET_A00p = mux2p(LUMA_DMA_CARTp(), dma.NAKY_DMA_A00p.qp_new(), ext_bus.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /* p08.ATOL*/ wire _ATOL_A01p = mux2p(LUMA_DMA_CARTp(), dma.PYRO_DMA_A01p.qp_new(), ext_bus.ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /* p08.APOK*/ wire _APOK_A02p = mux2p(LUMA_DMA_CARTp(), dma.NEFY_DMA_A02p.qp_new(), ext_bus.ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /* p08.AMER*/ wire _AMER_A03p = mux2p(LUMA_DMA_CARTp(), dma.MUTY_DMA_A03p.qp_new(), ext_bus.ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /* p08.ATEM*/ wire _ATEM_A04p = mux2p(LUMA_DMA_CARTp(), dma.NYKO_DMA_A04p.qp_new(), ext_bus.ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /* p08.ATOV*/ wire _ATOV_A05p = mux2p(LUMA_DMA_CARTp(), dma.PYLO_DMA_A05p.qp_new(), ext_bus.ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /* p08.ATYR*/ wire _ATYR_A06p = mux2p(LUMA_DMA_CARTp(), dma.NUTO_DMA_A06p.qp_new(), ext_bus.ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(LUMA_DMA_CARTp(), dma.MUGU_DMA_A07p.qp_new(), ext_bus.ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire _MANO_A08p = mux2p(LUMA_DMA_CARTp(), dma.NAFA_DMA_A08n.qn_new(), ext_bus.ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /* p08.MASU*/ wire _MASU_A09p = mux2p(LUMA_DMA_CARTp(), dma.PYNE_DMA_A09n.qn_new(), ext_bus.ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /* p08.PAMY*/ wire _PAMY_A10p = mux2p(LUMA_DMA_CARTp(), dma.PARA_DMA_A10n.qn_new(), ext_bus.ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /* p08.MALE*/ wire _MALE_A11p = mux2p(LUMA_DMA_CARTp(), dma.NYDO_DMA_A11n.qn_new(), ext_bus.ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /* p08.MOJY*/ wire _MOJY_A12p = mux2p(LUMA_DMA_CARTp(), dma.NYGY_DMA_A12n.qn_new(), ext_bus.ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /* p08.MUCE*/ wire _MUCE_A13p = mux2p(LUMA_DMA_CARTp(), dma.PULA_DMA_A13n.qn_new(), ext_bus.ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /* p08.PEGE*/ wire _PEGE_A14p = mux2p(LUMA_DMA_CARTp(), dma.POKU_DMA_A14n.qn_new(), ext_bus.ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

  /* p08.KUPO*/ wire _KUPO = nand2(_AMET_A00p, rst.TOVA_MODE_DBG2n());
  /* p08.CABA*/ wire _CABA = nand2(_ATOL_A01p, rst.TOVA_MODE_DBG2n());
  /* p08.BOKU*/ wire _BOKU = nand2(_APOK_A02p, rst.TOVA_MODE_DBG2n());
  /* p08.BOTY*/ wire _BOTY = nand2(_AMER_A03p, rst.TOVA_MODE_DBG2n());
  /* p08.BYLA*/ wire _BYLA = nand2(_ATEM_A04p, rst.TOVA_MODE_DBG2n());
  /* p08.BADU*/ wire _BADU = nand2(_ATOV_A05p, rst.TOVA_MODE_DBG2n());
  /* p08.CEPU*/ wire _CEPU = nand2(_ATYR_A06p, rst.TOVA_MODE_DBG2n());
  /* p08.DEFY*/ wire _DEFY = nand2(_ASUR_A07p, rst.TOVA_MODE_DBG2n());
  /* p08.MYNY*/ wire _MYNY = nand2(_MANO_A08p, rst.TOVA_MODE_DBG2n());
  /* p08.MUNE*/ wire _MUNE = nand2(_MASU_A09p, rst.TOVA_MODE_DBG2n());
  /* p08.ROXU*/ wire _ROXU = nand2(_PAMY_A10p, rst.TOVA_MODE_DBG2n());
  /* p08.LEPY*/ wire _LEPY = nand2(_MALE_A11p, rst.TOVA_MODE_DBG2n());
  /* p08.LUCE*/ wire _LUCE = nand2(_MOJY_A12p, rst.TOVA_MODE_DBG2n());
  /* p08.LABE*/ wire _LABE = nand2(_MUCE_A13p, rst.TOVA_MODE_DBG2n());
  /* p08.PUHE*/ wire _PUHE = nand2(_PEGE_A14p, rst.TOVA_MODE_DBG2n());

  /* p08.KOTY*/ wire _KOTY = nor2(_AMET_A00p, rst.UNOR_MODE_DBG2p());
  /* p08.COTU*/ wire _COTU = nor2(_ATOL_A01p, rst.UNOR_MODE_DBG2p());
  /* p08.BAJO*/ wire _BAJO = nor2(_APOK_A02p, rst.UNOR_MODE_DBG2p());
  /* p08.BOLA*/ wire _BOLA = nor2(_AMER_A03p, rst.UNOR_MODE_DBG2p());
  /* p08.BEVO*/ wire _BEVO = nor2(_ATEM_A04p, rst.UNOR_MODE_DBG2p());
  /* p08.AJAV*/ wire _AJAV = nor2(_ATOV_A05p, rst.UNOR_MODE_DBG2p());
  /* p08.CYKA*/ wire _CYKA = nor2(_ATYR_A06p, rst.UNOR_MODE_DBG2p());
  /* p08.COLO*/ wire _COLO = nor2(_ASUR_A07p, rst.UNOR_MODE_DBG2p());
  /* p08.MEGO*/ wire _MEGO = nor2(_MANO_A08p, rst.UNOR_MODE_DBG2p());
  /* p08.MENY*/ wire _MENY = nor2(_MASU_A09p, rst.UNOR_MODE_DBG2p());
  /* p08.RORE*/ wire _RORE = nor2(_PAMY_A10p, rst.UNOR_MODE_DBG2p());
  /* p08.LYNY*/ wire _LYNY = nor2(_MALE_A11p, rst.UNOR_MODE_DBG2p());
  /* p08.LOSO*/ wire _LOSO = nor2(_MOJY_A12p, rst.UNOR_MODE_DBG2p());
  /* p08.LEVA*/ wire _LEVA = nor2(_MUCE_A13p, rst.UNOR_MODE_DBG2p());
  /* p08.PAHY*/ wire _PAHY = nor2(_PEGE_A14p, rst.UNOR_MODE_DBG2p());

  /*PIN_01*/ ext_bus.PIN_01_A00.pin_out_hilo(_KUPO, _KOTY);
  /*PIN_02*/ ext_bus.PIN_02_A01.pin_out_hilo(_CABA, _COTU);
  /*PIN_03*/ ext_bus.PIN_03_A02.pin_out_hilo(_BOKU, _BAJO);
  /*PIN_04*/ ext_bus.PIN_04_A03.pin_out_hilo(_BOTY, _BOLA);
  /*PIN_05*/ ext_bus.PIN_05_A04.pin_out_hilo(_BYLA, _BEVO);
  /*PIN_06*/ ext_bus.PIN_06_A05.pin_out_hilo(_BADU, _AJAV);
  /*PIN_07*/ ext_bus.PIN_07_A06.pin_out_hilo(_CEPU, _CYKA);
  /*PIN_08*/ ext_bus.PIN_08_A07.pin_out_hilo(_DEFY, _COLO);
  /*PIN_09*/ ext_bus.PIN_09_A08.pin_out_hilo(_MYNY, _MEGO);
  /*PIN_10*/ ext_bus.PIN_10_A09.pin_out_hilo(_MUNE, _MENY);
  /*PIN_11*/ ext_bus.PIN_11_A10.pin_out_hilo(_ROXU, _RORE);
  /*PIN_12*/ ext_bus.PIN_12_A11.pin_out_hilo(_LEPY, _LYNY);
  /*PIN_13*/ ext_bus.PIN_13_A12.pin_out_hilo(_LUCE, _LOSO);
  /*PIN_14*/ ext_bus.PIN_14_A13.pin_out_hilo(_LABE, _LEVA);
  /*PIN_15*/ ext_bus.PIN_15_A14.pin_out_hilo(_PUHE, _PAHY);

  // A15 is "special"
  /* p08.SOBY*/ wire _SOBY_A15n = nor2 (new_bus.BUS_CPU_A15p.qp_new(), cpu_bus.TUTU_READ_BOOTROMp(new_bus));
  /* p08.SEPY*/ wire _SEPY_A15p = nand2(cpu_bus.ABUZ_EXT_RAM_CS_CLK, _SOBY_A15n);
  /* p08.TAZY*/ wire _TAZY_A15p = mux2p (LUMA_DMA_CARTp(), dma.MARU_DMA_A15n.qn_new(), _SEPY_A15p);
  /* p08.SUZE*/ wire _SUZE_A15n = nand2(_TAZY_A15p, rst.RYCA_MODE_DBG2n());
  /* p08.RULO*/ wire _RULO_A15n = nor2 (_TAZY_A15p, rst.UNOR_MODE_DBG2p());
  /*PIN_16*/ ext_bus.PIN_16_A15.pin_out_hilo(_SUZE_A15n, _RULO_A15n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::copy_pins_to_data_latch()
{
  /* p08.LAVO*/ wire _LAVO_HOLDn = nand3(cpu_bus.SIG_CPU_RDp, cpu_bus.TEXO_ADDR_VRAMn(new_bus), cpu_bus.SIG_CPU_LATCH_EXT);
  /*#p08.SOMA*/ ext_bus.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_17_D00.qp_new());
  /* p08.RONY*/ ext_bus.ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_18_D01.qp_new());
  /* p08.RAXY*/ ext_bus.ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_19_D02.qp_new());
  /* p08.SELO*/ ext_bus.ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_20_D03.qp_new());
  /* p08.SODY*/ ext_bus.ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_21_D04.qp_new());
  /* p08.SAGO*/ ext_bus.ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_22_D05.qp_new());
  /* p08.RUPA*/ ext_bus.ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_23_D06.qp_new());
  /* p08.SAZY*/ ext_bus.ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latchn(_LAVO_HOLDn, ext_bus.PIN_24_D07.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::copy_data_latch_to_cpu_bus()
{
  /* p08.LAVO*/ wire _LAVO_HOLDn = nand3(cpu_bus.SIG_CPU_RDp, cpu_bus.TEXO_ADDR_VRAMn(new_bus), cpu_bus.SIG_CPU_LATCH_EXT);
  /*#p08.RYMA_EL0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
  /* p08.RUVO_EL1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
  /* p08.RYKO_EL2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
  /* p08.TAVO_EL3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
  /* p08.TEPE_EL4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
  /* p08.SAFO_EL5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
  /* p08.SEVU_EL6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
  /* p08.TAJU_EL7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_nn(_LAVO_HOLDn, ext_bus.ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::copy_cpu_data_to_pins()
{
  // FIXME So does this mean that if the CPU writes to the external new_bus during dma, that data
  // will actually end up in oam?

  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(cpu_bus.TEXO_ADDR_VRAMn(new_bus), rst.UMUT_MODE_DBG1p());
  /* p08.LAGU*/ wire _LAGU = and_or3(cpu_bus.SIG_CPU_RDp, cpu_bus.LEVO_ADDR_VRAMn(new_bus), cpu_bus.SIG_CPU_WRp);
  /* p08.LYWE*/ wire _LYWE = not1(_LAGU);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
  /* p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(rst.UNOR_MODE_DBG2p(), cpu_bus.REDU_CPU_RDn(), _MOTY_CPU_EXT_RD);
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

  /*PIN_17*/ ext_bus.PIN_17_D00.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RUXA, _RUNE);
  /*PIN_18*/ ext_bus.PIN_18_D01.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RUJA, _RYPU);
  /*PIN_19*/ ext_bus.PIN_19_D02.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RABY, _SULY);
  /*PIN_20*/ ext_bus.PIN_20_D03.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RERA, _SEZE);
  /*PIN_21*/ ext_bus.PIN_21_D04.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RORY, _RESY);
  /*PIN_22*/ ext_bus.PIN_22_D05.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RYVO, _TAMU);
  /*PIN_23*/ ext_bus.PIN_23_D06.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RAFY, _ROGY);
  /*PIN_24*/ ext_bus.PIN_24_D07.pin_io_out_pull_hilo(_LULA_CBD_TO_EPDp, _RAVU, _RYDA);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::set_ext_control_pins()
{
  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(cpu_bus.TEXO_ADDR_VRAMn(new_bus), rst.UMUT_MODE_DBG1p());

  {
    /* p08.LAGU*/ wire _LAGU = and_or3(cpu_bus.SIG_CPU_RDp, cpu_bus.LEVO_ADDR_VRAMn(new_bus), cpu_bus.SIG_CPU_WRp);
    /* p08.LYWE*/ wire _LYWE = not1(_LAGU);
    /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
    /* p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(LUMA_DMA_CARTp(), _MOTY_CPU_EXT_RD);
    /* p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, rst.TOVA_MODE_DBG2n());
    /* p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, rst.UNOR_MODE_DBG2p());
    /*PIN_79*/ ext_bus.PIN_79_RDn.pin_out_hilo(_UGAC_RD_A, _URUN_RD_D);
  }

  {
    /* p08.NEVY*/ wire _NEVY = or2(cpu_bus.MEXO_CPU_WRn(), _MOCA_DBG_EXT_RD);
    /* p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, LUMA_DMA_CARTp());
    /* p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, rst.TOVA_MODE_DBG2n());
    /* p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, rst.UNOR_MODE_DBG2p());
    /*PIN_78*/ ext_bus.PIN_78_WRn.pin_out_hilo(_UVER_WR_A, _USUF_WR_D);
  }

  {
    /* p08.TOZA*/ wire _TOZA_CS_A = and3(cpu_bus.ABUZ_EXT_RAM_CS_CLK, new_bus.TYNU_A000_FFFFp(), new_bus.TUNA_0000_FDFF());
    /* p08.TYHO*/ wire _TYHO_CS_A = mux2p(LUMA_DMA_CARTp(), dma.MARU_DMA_A15n.qn_new(), _TOZA_CS_A);
    /*PIN_80*/ ext_bus.PIN_80_CSn.pin_out_dp(_TYHO_CS_A);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::read_ext_to_pins() {
  uint16_t addr = (uint16_t)BitBase::pack_ext_new(16, &ext_bus.PIN_01_A00);
  wire data = 0xFF;

  // ROM read
  //wire rom_CEn = PIN_01_ADDR[15].ext_qp_new2();
  //wire rom_OEp = bit(~rom_CEn) && bit(~PIN_79_RDn.ext_qp_new2()) && cart_buf;
  wire rom_OEp = (addr >= 0x0000) && (addr <= 0x7FFF) && bit(~ext_bus.PIN_79_RDn.qp_ext_new());
  if (rom_OEp) data = cart_buf[addr & 0x7FFF];

  // Cart RAM read
  //wire cram_CS1n = PIN_80_CSn.ext_qp_new2();
  //wire cram_CS2p = PIN_01_ADDR[13].ext_qp_new2() && bit(~PIN_01_ADDR[14].ext_qp_new2()) && PIN_01_ADDR[15].ext_qp_new2();
  //wire cram_OEp  = bit(~cram_CS1n) && cram_CS2p && bit(~PIN_79_RDn.ext_qp_new2());
  wire cram_OEp = (addr >= 0xA000) && (addr <= 0xBFFF) && bit(~ext_bus.PIN_79_RDn.qp_ext_new());
  if (cram_OEp) data = cart_ram[addr & 0x1FFF];

  // Internal RAM read
  //wire iram_CS1n = PIN_80_CSn.qp_new();
  //wire iram_CS2p = PIN_01_ADDR[14].qp_new();
  //wire iram_OEp  = bit(~iram_CS1n) && iram_CS2p && bit(~PIN_79_RDn.qp_new());
  wire iram_OEp = (addr >= 0xC000) && (addr <= 0xDFFF) && bit(~ext_bus.PIN_79_RDn.qp_ext_new());
  if (iram_OEp) data = int_ram[addr & 0x1FFF];

  // Echo RAM read
  //wire eram_CS1n = PIN_80_CSn.qp_new();
  //wire eram_CS2p = PIN_01_ADDR[14].qp_new();
  //wire eram_OEp  = bit(~eram_CS1n) && eram_CS2p && bit(~PIN_79_RDn.qp_new());
  wire eram_OEp = (addr >= 0xE000) && (addr <= 0xFFFF) && bit(~ext_bus.PIN_79_RDn.qp_ext_new());
  if (eram_OEp) data = int_ram[addr & 0x1FFF];

  wire ext_OEp = rom_OEp || cram_OEp || iram_OEp || eram_OEp;
  ext_bus.PIN_17_D00.pin_io_in_oedp(ext_OEp, (data >> 0) & 1);
  ext_bus.PIN_18_D01.pin_io_in_oedp(ext_OEp, (data >> 1) & 1);
  ext_bus.PIN_19_D02.pin_io_in_oedp(ext_OEp, (data >> 2) & 1);
  ext_bus.PIN_20_D03.pin_io_in_oedp(ext_OEp, (data >> 3) & 1);
  ext_bus.PIN_21_D04.pin_io_in_oedp(ext_OEp, (data >> 4) & 1);
  ext_bus.PIN_22_D05.pin_io_in_oedp(ext_OEp, (data >> 5) & 1);
  ext_bus.PIN_23_D06.pin_io_in_oedp(ext_OEp, (data >> 6) & 1);
  ext_bus.PIN_24_D07.pin_io_in_oedp(ext_OEp, (data >> 7) & 1);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::write_pins_to_ext() {
  uint16_t addr = (uint16_t)BitBase::pack_ext_new(16, &ext_bus.PIN_01_A00);
  wire data = (wire)BitBase::pack_ext_new(8, &ext_bus.PIN_17_D00);

  // Cart RAM write
  //wire cram_CS1n = PIN_80_CSn.qp_new();
  //wire cram_CS2p = PIN_01_ADDR[13].qp_new() && bit(~PIN_01_ADDR[14].qp_new()) && PIN_01_ADDR[15].qp_new();
  //wire cram_WRp  = bit(~cram_CS1n) && cram_CS2p && bit(~PIN_78_WRn.qp_new());
  wire cram_WRp = (addr >= 0xA000) && (addr <= 0xBFFF) && bit(~ext_bus.PIN_78_WRn.qp_ext_new());
  if (cram_WRp) cart_ram[addr & 0x1FFF] = data;

  // Internal RAM write
  //wire iram_CS1n = PIN_80_CSn.qp_new();
  //wire iram_CS2p = PIN_01_ADDR[14].qp_new();
  //wire iram_WRp  = bit(~iram_CS1n) && iram_CS2p && bit(~PIN_78_WRn.qp_new());
  wire iram_WRp = (addr >= 0xC000) && (addr <= 0xDFFF) && bit(~ext_bus.PIN_78_WRn.qp_ext_new());
  if (iram_WRp) int_ram[addr & 0x1FFF]  = data;

  // Echo RAM write
  //wire eram_CS1n = PIN_80_CSn.qp_new();
  //wire eram_CS2p = PIN_01_ADDR[14].qp_new();
  //wire eram_WRp  = bit(~eram_CS1n) && iram_CS2p && bit(~PIN_78_WRn.qp_new());
  wire eram_WRp = (addr >= 0xE000) && (addr <= 0xFFFF) && bit(~ext_bus.PIN_78_WRn.qp_ext_new());
  if (eram_WRp) int_ram[addr & 0x1FFF]  = data;
}

//------------------------------------------------------------------------------------------------------------------------
