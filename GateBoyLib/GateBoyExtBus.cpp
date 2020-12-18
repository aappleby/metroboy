#include "GateBoyLib/GateBoyExtBus.h"

#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyCpuBus.h"

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
  wire cart_rom_a19 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp_new();
  wire cart_rom_a20 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp_new();

  wire cart_ram_a13 = MBC1_MODE.qp_new() ? MBC1_BANK_D5.qp_new() : 0;
  wire cart_ram_a14 = MBC1_MODE.qp_new() ? MBC1_BANK_D6.qp_new() : 0;

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire OEn = PIN_EXT_RDn.qp_new();
    wire CEn = PIN_EXT_A[15].qp_new();

    if (!CEn && !OEn) {
      ext_bus.set_pin_data(cart_rom[rom_addr]);
    }
  }
}
*/

//------------------------------------------------------------------------------------------------------------------------

void ExtAddrLatch::tock(const GateBoyResetDebug& rst, Signal BUS_CPU_A[16], wire TEXO_ADDR_VRAMn_ext) {
  /* p08.LOXO*/ wire _LOXO_HOLDn = and_or3(rst.MULE_MODE_DBG1n(), TEXO_ADDR_VRAMn_ext, rst.UMUT_MODE_DBG1p());
  /* p08.LASY*/ wire _LASY_HOLDp = not1(_LOXO_HOLDn);
  /* p08.MATE*/ wire _MATE_HOLDn = not1(_LASY_HOLDp);
  /* p08.ALOR*/ ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 0].qp_new());
  /* p08.APUR*/ APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 1].qp_new());
  /* p08.ALYR*/ ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 2].qp_new());
  /* p08.ARET*/ ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 3].qp_new());
  /* p08.AVYS*/ AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 4].qp_new());
  /* p08.ATEV*/ ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 5].qp_new());
  /* p08.AROS*/ AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 6].qp_new());
  /* p08.ARYM*/ ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 7].qp_new());
  /* p08.LUNO*/ LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 8].qp_new());
  /* p08.LYSA*/ LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn, BUS_CPU_A[ 9].qp_new());
  /* p08.PATE*/ PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn, BUS_CPU_A[10].qp_new());
  /* p08.LUMY*/ LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn, BUS_CPU_A[11].qp_new());
  /* p08.LOBU*/ LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn, BUS_CPU_A[12].qp_new());
  /* p08.LONU*/ LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn, BUS_CPU_A[13].qp_new());
  /* p08.NYRE*/ NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn, BUS_CPU_A[14].qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::addr_latch_to_pins(
  const GateBoyResetDebug& rst,
  const GateBoyCpuBus& cpu_bus,
  const GateBoyDMA& dma,
  wire ABUZ_EXT_RAM_CS_CLK,
  wire TUTU_READ_BOOTROMp
)
{
  /* p08.AMET*/ wire _AMET_A00p = mux2p(dma.LUMA_DMA_CARTp(), dma.NAKY_DMA_A00p.qp_new(), ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /* p08.ATOL*/ wire _ATOL_A01p = mux2p(dma.LUMA_DMA_CARTp(), dma.PYRO_DMA_A01p.qp_new(), ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /* p08.APOK*/ wire _APOK_A02p = mux2p(dma.LUMA_DMA_CARTp(), dma.NEFY_DMA_A02p.qp_new(), ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /* p08.AMER*/ wire _AMER_A03p = mux2p(dma.LUMA_DMA_CARTp(), dma.MUTY_DMA_A03p.qp_new(), ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /* p08.ATEM*/ wire _ATEM_A04p = mux2p(dma.LUMA_DMA_CARTp(), dma.NYKO_DMA_A04p.qp_new(), ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /* p08.ATOV*/ wire _ATOV_A05p = mux2p(dma.LUMA_DMA_CARTp(), dma.PYLO_DMA_A05p.qp_new(), ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /* p08.ATYR*/ wire _ATYR_A06p = mux2p(dma.LUMA_DMA_CARTp(), dma.NUTO_DMA_A06p.qp_new(), ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire _ASUR_A07p = mux2p(dma.LUMA_DMA_CARTp(), dma.MUGU_DMA_A07p.qp_new(), ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire _MANO_A08p = mux2p(dma.LUMA_DMA_CARTp(), dma.NAFA_DMA_A08n.qn_new(), ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /* p08.MASU*/ wire _MASU_A09p = mux2p(dma.LUMA_DMA_CARTp(), dma.PYNE_DMA_A09n.qn_new(), ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /* p08.PAMY*/ wire _PAMY_A10p = mux2p(dma.LUMA_DMA_CARTp(), dma.PARA_DMA_A10n.qn_new(), ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /* p08.MALE*/ wire _MALE_A11p = mux2p(dma.LUMA_DMA_CARTp(), dma.NYDO_DMA_A11n.qn_new(), ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /* p08.MOJY*/ wire _MOJY_A12p = mux2p(dma.LUMA_DMA_CARTp(), dma.NYGY_DMA_A12n.qn_new(), ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /* p08.MUCE*/ wire _MUCE_A13p = mux2p(dma.LUMA_DMA_CARTp(), dma.PULA_DMA_A13n.qn_new(), ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /* p08.PEGE*/ wire _PEGE_A14p = mux2p(dma.LUMA_DMA_CARTp(), dma.POKU_DMA_A14n.qn_new(), ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

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

  /* p08.KOTY*/ wire _KOTY = nor2 (_AMET_A00p, rst.UNOR_MODE_DBG2p());
  /* p08.COTU*/ wire _COTU = nor2 (_ATOL_A01p, rst.UNOR_MODE_DBG2p());
  /* p08.BAJO*/ wire _BAJO = nor2 (_APOK_A02p, rst.UNOR_MODE_DBG2p());
  /* p08.BOLA*/ wire _BOLA = nor2 (_AMER_A03p, rst.UNOR_MODE_DBG2p());
  /* p08.BEVO*/ wire _BEVO = nor2 (_ATEM_A04p, rst.UNOR_MODE_DBG2p());
  /* p08.AJAV*/ wire _AJAV = nor2 (_ATOV_A05p, rst.UNOR_MODE_DBG2p());
  /* p08.CYKA*/ wire _CYKA = nor2 (_ATYR_A06p, rst.UNOR_MODE_DBG2p());
  /* p08.COLO*/ wire _COLO = nor2 (_ASUR_A07p, rst.UNOR_MODE_DBG2p());
  /* p08.MEGO*/ wire _MEGO = nor2 (_MANO_A08p, rst.UNOR_MODE_DBG2p());
  /* p08.MENY*/ wire _MENY = nor2 (_MASU_A09p, rst.UNOR_MODE_DBG2p());
  /* p08.RORE*/ wire _RORE = nor2 (_PAMY_A10p, rst.UNOR_MODE_DBG2p());
  /* p08.LYNY*/ wire _LYNY = nor2 (_MALE_A11p, rst.UNOR_MODE_DBG2p());
  /* p08.LOSO*/ wire _LOSO = nor2 (_MOJY_A12p, rst.UNOR_MODE_DBG2p());
  /* p08.LEVA*/ wire _LEVA = nor2 (_MUCE_A13p, rst.UNOR_MODE_DBG2p());
  /* p08.PAHY*/ wire _PAHY = nor2 (_PEGE_A14p, rst.UNOR_MODE_DBG2p());

  PIN_EXT_A[ 0].pin_out(_KUPO, _KOTY);
  PIN_EXT_A[ 1].pin_out(_CABA, _COTU);
  PIN_EXT_A[ 2].pin_out(_BOKU, _BAJO);
  PIN_EXT_A[ 3].pin_out(_BOTY, _BOLA);
  PIN_EXT_A[ 4].pin_out(_BYLA, _BEVO);
  PIN_EXT_A[ 5].pin_out(_BADU, _AJAV);
  PIN_EXT_A[ 6].pin_out(_CEPU, _CYKA);
  PIN_EXT_A[ 7].pin_out(_DEFY, _COLO);
  PIN_EXT_A[ 8].pin_out(_MYNY, _MEGO);
  PIN_EXT_A[ 9].pin_out(_MUNE, _MENY);
  PIN_EXT_A[10].pin_out(_ROXU, _RORE);
  PIN_EXT_A[11].pin_out(_LEPY, _LYNY);
  PIN_EXT_A[12].pin_out(_LUCE, _LOSO);
  PIN_EXT_A[13].pin_out(_LABE, _LEVA);
  PIN_EXT_A[14].pin_out(_PUHE, _PAHY);

  // A15 is "special"
  /* p08.SOBY*/ wire _SOBY_A15n = nor2(cpu_bus.BUS_CPU_A[15].qp_new(), TUTU_READ_BOOTROMp);
  /* p08.SEPY*/ wire _SEPY_A15p = nand2(ABUZ_EXT_RAM_CS_CLK, _SOBY_A15n);
  /* p08.TAZY*/ wire _TAZY_A15p = mux2p(dma.LUMA_DMA_CARTp(), dma.MARU_DMA_A15n.qn_new(), _SEPY_A15p);
  /* p08.SUZE*/ wire _SUZE_A15n = nand2(_TAZY_A15p, rst.RYCA_MODE_DBG2n());
  /* p08.RULO*/ wire _RULO_A15n = nor2 (_TAZY_A15p, rst.UNOR_MODE_DBG2p());
  PIN_EXT_A[15].pin_out(_SUZE_A15n, _RULO_A15n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::pins_to_data_latch(const GateBoyCpuBus& cpu_bus)
{
  /* p08.LAVO*/ wire _LAVO_HOLDn = nand3(cpu_bus.PIN_CPU_RDp.qp_new(), cpu_bus.TEXO_ADDR_VRAMn(), cpu_bus.PIN_CPU_LATCH_EXT.qp_new());
  /*#p08.SOMA*/ ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[0].qn_new());
  /* p08.RONY*/ ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[1].qn_new());
  /* p08.RAXY*/ ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[2].qn_new());
  /* p08.SELO*/ ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[3].qn_new());
  /* p08.SODY*/ ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[4].qn_new());
  /* p08.SAGO*/ ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[5].qn_new());
  /* p08.RUPA*/ ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[6].qn_new());
  /* p08.SAZY*/ ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn, PIN_EXT_D[7].qn_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::data_latch_to_cpu_bus(GateBoyCpuBus& cpu_bus)
{
  /* p08.LAVO*/ wire _LAVO_HOLDn = nand3(cpu_bus.PIN_CPU_RDp.qp_new(), cpu_bus.TEXO_ADDR_VRAMn(), cpu_bus.PIN_CPU_LATCH_EXT.qp_new());
  /*#p08.RYMA*/ cpu_bus.BUS_CPU_D[0].tri6_nn(_LAVO_HOLDn, ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
  /* p08.RUVO*/ cpu_bus.BUS_CPU_D[1].tri6_nn(_LAVO_HOLDn, ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
  /* p08.RYKO*/ cpu_bus.BUS_CPU_D[2].tri6_nn(_LAVO_HOLDn, ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
  /* p08.TAVO*/ cpu_bus.BUS_CPU_D[3].tri6_nn(_LAVO_HOLDn, ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
  /* p08.TEPE*/ cpu_bus.BUS_CPU_D[4].tri6_nn(_LAVO_HOLDn, ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
  /* p08.SAFO*/ cpu_bus.BUS_CPU_D[5].tri6_nn(_LAVO_HOLDn, ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
  /* p08.SEVU*/ cpu_bus.BUS_CPU_D[6].tri6_nn(_LAVO_HOLDn, ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
  /* p08.TAJU*/ cpu_bus.BUS_CPU_D[7].tri6_nn(_LAVO_HOLDn, ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::cpu_data_to_pins(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus)
{
  // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
  // will actually end up in oam?

  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(cpu_bus.TEXO_ADDR_VRAMn(), rst.UMUT_MODE_DBG1p());
  /* p08.LAGU*/ wire _LAGU = and_or3(cpu_bus.PIN_CPU_RDp.qp_new(), cpu_bus.LEVO_ADDR_VRAMn(), cpu_bus.PIN_CPU_WRp.qp_new());
  /* p08.LYWE*/ wire _LYWE = not1(_LAGU);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
  /* p08.RORU*/ wire _RORU_CBD_TO_EPDn = mux2p(rst.UNOR_MODE_DBG2p(), cpu_bus.REDU_CPU_RDn(), _MOTY_CPU_EXT_RD);
  /* p08.LULA*/ wire _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

  /* p25.RUXA*/ wire _RUXA = nand2(cpu_bus.BUS_CPU_D[0].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RUJA*/ wire _RUJA = nand2(cpu_bus.BUS_CPU_D[1].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RABY*/ wire _RABY = nand2(cpu_bus.BUS_CPU_D[2].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RERA*/ wire _RERA = nand2(cpu_bus.BUS_CPU_D[3].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RORY*/ wire _RORY = nand2(cpu_bus.BUS_CPU_D[4].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RYVO*/ wire _RYVO = nand2(cpu_bus.BUS_CPU_D[5].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RAFY*/ wire _RAFY = nand2(cpu_bus.BUS_CPU_D[6].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RAVU*/ wire _RAVU = nand2(cpu_bus.BUS_CPU_D[7].qp_new(), _LULA_CBD_TO_EPDp);

  /* p08.RUNE*/ wire _RUNE = nor2 (cpu_bus.BUS_CPU_D[0].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RYPU*/ wire _RYPU = nor2 (cpu_bus.BUS_CPU_D[1].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.SULY*/ wire _SULY = nor2 (cpu_bus.BUS_CPU_D[2].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.SEZE*/ wire _SEZE = nor2 (cpu_bus.BUS_CPU_D[3].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RESY*/ wire _RESY = nor2 (cpu_bus.BUS_CPU_D[4].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.TAMU*/ wire _TAMU = nor2 (cpu_bus.BUS_CPU_D[5].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.ROGY*/ wire _ROGY = nor2 (cpu_bus.BUS_CPU_D[6].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RYDA*/ wire _RYDA = nor2 (cpu_bus.BUS_CPU_D[7].qp_new(), _RORU_CBD_TO_EPDn);

  PIN_EXT_D[0].pin_out(_LULA_CBD_TO_EPDp, _RUXA, _RUNE);
  PIN_EXT_D[1].pin_out(_LULA_CBD_TO_EPDp, _RUJA, _RYPU);
  PIN_EXT_D[2].pin_out(_LULA_CBD_TO_EPDp, _RABY, _SULY);
  PIN_EXT_D[3].pin_out(_LULA_CBD_TO_EPDp, _RERA, _SEZE);
  PIN_EXT_D[4].pin_out(_LULA_CBD_TO_EPDp, _RORY, _RESY);
  PIN_EXT_D[5].pin_out(_LULA_CBD_TO_EPDp, _RYVO, _TAMU);
  PIN_EXT_D[6].pin_out(_LULA_CBD_TO_EPDp, _RAFY, _ROGY);
  PIN_EXT_D[7].pin_out(_LULA_CBD_TO_EPDp, _RAVU, _RYDA);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::set_pins(
  const GateBoyResetDebug& rst,
  const GateBoyCpuBus& cpu_bus,
  const GateBoyDMA& dma,
  wire ABUZ_EXT_RAM_CS_CLK)
{
  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD = nor2(cpu_bus.TEXO_ADDR_VRAMn(), rst.UMUT_MODE_DBG1p());
  /* p08.LAGU*/ wire _LAGU = and_or3(cpu_bus.PIN_CPU_RDp.qp_new(), cpu_bus.LEVO_ADDR_VRAMn(), cpu_bus.PIN_CPU_WRp.qp_new());
  /* p08.LYWE*/ wire _LYWE = not1(_LAGU);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
  /* p08.TYMU*/ wire _TYMU_EXT_RDn = nor2(dma.LUMA_DMA_CARTp(), _MOTY_CPU_EXT_RD);
  /* p08.UGAC*/ wire _UGAC_RD_A = nand2(_TYMU_EXT_RDn, rst.TOVA_MODE_DBG2n());
  /* p08.URUN*/ wire _URUN_RD_D = nor2 (_TYMU_EXT_RDn, rst.UNOR_MODE_DBG2p());
  PIN_EXT_RDn.pin_out(_UGAC_RD_A, _URUN_RD_D);

  /* p08.NEVY*/ wire _NEVY = or2(cpu_bus.MEXO_CPU_WRn(), _MOCA_DBG_EXT_RD);
  /* p08.PUVA*/ wire _PUVA_EXT_WRn = or2(_NEVY, dma.LUMA_DMA_CARTp());
  /* p08.UVER*/ wire _UVER_WR_A = nand2(_PUVA_EXT_WRn, rst.TOVA_MODE_DBG2n());
  /* p08.USUF*/ wire _USUF_WR_D = nor2 (_PUVA_EXT_WRn, rst.UNOR_MODE_DBG2p());
  PIN_EXT_WRn.pin_out(_UVER_WR_A, _USUF_WR_D);

  /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A = and3(ABUZ_EXT_RAM_CS_CLK, cpu_bus.TYNU_A000_FFFFp(), cpu_bus.TUNA_0000_FDFF());
  /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A = mux2p(dma.LUMA_DMA_CARTp(), dma.MARU_DMA_A15n.qn_new(), _TOZA_PIN_EXT_CS_A);
  PIN_EXT_CSn.pin_out(_TYHO_PIN_EXT_CS_A, _TYHO_PIN_EXT_CS_A);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::cart_to_pins(const uint8_t* cart_buf, uint8_t* cart_ram, uint8_t* ext_ram) {
  uint16_t _ext_addr_latch = pack_u16p(16, &PIN_EXT_A[ 0]);
  uint8_t _ext_data_latch = pack_u8p(8, &PIN_EXT_D[0]);

  // ROM read
  uint16_t rom_addr = _ext_addr_latch & 0x7FFF;
  wire rom_CEn_new = PIN_EXT_A[15].qp_new();
  wire rom_OEp_new = !rom_CEn_new && !PIN_EXT_RDn.qp_new() && cart_buf;
  if (rom_OEp_new)  _ext_data_latch = cart_buf[rom_addr];

  // Ext RAM read
  uint16_t eram_addr = (_ext_addr_latch & 0x1FFF);
  wire eram_CE1n_new = PIN_EXT_CSn.qp_new();
  wire eram_CE2_new  = PIN_EXT_A[14].qp_new();
  wire eram_WRp_new  = !eram_CE1n_new && eram_CE2_new && !PIN_EXT_WRn.qp_new();
  wire eram_OEp_new  = !eram_CE1n_new && eram_CE2_new && !eram_WRp_new && !PIN_EXT_RDn.qp_new();
  if (eram_WRp_new) ext_ram[eram_addr]  = _ext_data_latch;
  if (eram_OEp_new) _ext_data_latch = ext_ram[eram_addr];

  // Cart RAM read
  uint16_t cram_addr = (_ext_addr_latch & 0x1FFF);
  wire cram_CS1n_new = PIN_EXT_CSn.qp_new();
  wire cram_CS2_new  = PIN_EXT_A[13].qp_new() && !PIN_EXT_A[14].qp_new() && PIN_EXT_A[15].qp_new();
  wire cram_WRp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_WRn.qp_new();
  wire cram_OEp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_RDn.qp_new();

  if (cram_WRp_new) cart_ram[cram_addr] = _ext_data_latch;
  if (cram_OEp_new) _ext_data_latch = cart_ram[cram_addr];

  wire ext_OEp = rom_OEp_new || eram_OEp_new || cram_OEp_new;
  PIN_EXT_D[0].pin_out(ext_OEp, wire(_ext_data_latch & 0x01));
  PIN_EXT_D[1].pin_out(ext_OEp, wire(_ext_data_latch & 0x02));
  PIN_EXT_D[2].pin_out(ext_OEp, wire(_ext_data_latch & 0x04));
  PIN_EXT_D[3].pin_out(ext_OEp, wire(_ext_data_latch & 0x08));
  PIN_EXT_D[4].pin_out(ext_OEp, wire(_ext_data_latch & 0x10));
  PIN_EXT_D[5].pin_out(ext_OEp, wire(_ext_data_latch & 0x20));
  PIN_EXT_D[6].pin_out(ext_OEp, wire(_ext_data_latch & 0x40));
  PIN_EXT_D[7].pin_out(ext_OEp, wire(_ext_data_latch & 0x80));
}

//------------------------------------------------------------------------------------------------------------------------
