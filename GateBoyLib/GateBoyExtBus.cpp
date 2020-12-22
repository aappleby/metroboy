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

  wire2 cart_rom_a14 = bank_0 ? 1 : MBC1_BANK_D0.qp_new();
  wire2 cart_rom_a15 = bank_0 ? 0 : MBC1_BANK_D1.qp_new();
  wire2 cart_rom_a16 = bank_0 ? 0 : MBC1_BANK_D2.qp_new();
  wire2 cart_rom_a17 = bank_0 ? 0 : MBC1_BANK_D3.qp_new();
  wire2 cart_rom_a18 = bank_0 ? 0 : MBC1_BANK_D4.qp_new();
  wire2 cart_rom_a19 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D5.qp_new();
  wire2 cart_rom_a20 = MBC1_MODE.qp_new() ? 0 : bank_0 ? 0 : MBC1_BANK_D6.qp_new();

  wire2 cart_ram_a13 = MBC1_MODE.qp_new() ? MBC1_BANK_D5.qp_new() : 0;
  wire2 cart_ram_a14 = MBC1_MODE.qp_new() ? MBC1_BANK_D6.qp_new() : 0;

  // ROM read
  {
    uint16_t rom_addr = ext_addr & 0x7FFF;
    wire2 OEn = PIN79_EXT_RDn.qp_new();
    wire2 CEn = PIN01_EXT_A[15].qp_new();

    if (!CEn && !OEn) {
      ext_bus.set_pin_data(cart_rom[rom_addr]);
    }
  }
}
*/

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::copy_cpu_addr_to_addr_latch(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus)
{
  /* p08.LOXO*/ wire2 _LOXO_HOLDn = and_or3(rst.MULE_MODE_DBG1n(), cpu_bus.TEXO_ADDR_VRAMn(), rst.UMUT_MODE_DBG1p());
  /* p08.LASY*/ wire2 _LASY_HOLDp = not1(_LOXO_HOLDn);
  /* p08.MATE*/ wire2 _MATE_HOLDn = not1(_LASY_HOLDp);
  /* p08.ALOR*/ ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 0].qp_new());
  /* p08.APUR*/ ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 1].qp_new());
  /* p08.ALYR*/ ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 2].qp_new());
  /* p08.ARET*/ ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 3].qp_new());
  /* p08.AVYS*/ ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 4].qp_new());
  /* p08.ATEV*/ ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 5].qp_new());
  /* p08.AROS*/ ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 6].qp_new());
  /* p08.ARYM*/ ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 7].qp_new());
  /* p08.LUNO*/ ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 8].qp_new());
  /* p08.LYSA*/ ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[ 9].qp_new());
  /* p08.PATE*/ ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[10].qp_new());
  /* p08.LUMY*/ ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[11].qp_new());
  /* p08.LOBU*/ ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[12].qp_new());
  /* p08.LONU*/ ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[13].qp_new());
  /* p08.NYRE*/ ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn, cpu_bus.BUS_CPU_A[14].qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::copy_addr_latch_to_pins(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus, const GateBoyDMA& dma, wire2 ABUZ_EXT_RAM_CS_CLK)
{
  /* p08.AMET*/ wire2 _AMET_A00p = mux2p(dma.LUMA_DMA_CARTp(), dma.NAKY_DMA_A00p.qp_new(), ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /* p08.ATOL*/ wire2 _ATOL_A01p = mux2p(dma.LUMA_DMA_CARTp(), dma.PYRO_DMA_A01p.qp_new(), ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /* p08.APOK*/ wire2 _APOK_A02p = mux2p(dma.LUMA_DMA_CARTp(), dma.NEFY_DMA_A02p.qp_new(), ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /* p08.AMER*/ wire2 _AMER_A03p = mux2p(dma.LUMA_DMA_CARTp(), dma.MUTY_DMA_A03p.qp_new(), ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /* p08.ATEM*/ wire2 _ATEM_A04p = mux2p(dma.LUMA_DMA_CARTp(), dma.NYKO_DMA_A04p.qp_new(), ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /* p08.ATOV*/ wire2 _ATOV_A05p = mux2p(dma.LUMA_DMA_CARTp(), dma.PYLO_DMA_A05p.qp_new(), ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /* p08.ATYR*/ wire2 _ATYR_A06p = mux2p(dma.LUMA_DMA_CARTp(), dma.NUTO_DMA_A06p.qp_new(), ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire2 _ASUR_A07p = mux2p(dma.LUMA_DMA_CARTp(), dma.MUGU_DMA_A07p.qp_new(), ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire2 _MANO_A08p = mux2p(dma.LUMA_DMA_CARTp(), dma.NAFA_DMA_A08n.qn_new(), ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /* p08.MASU*/ wire2 _MASU_A09p = mux2p(dma.LUMA_DMA_CARTp(), dma.PYNE_DMA_A09n.qn_new(), ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /* p08.PAMY*/ wire2 _PAMY_A10p = mux2p(dma.LUMA_DMA_CARTp(), dma.PARA_DMA_A10n.qn_new(), ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /* p08.MALE*/ wire2 _MALE_A11p = mux2p(dma.LUMA_DMA_CARTp(), dma.NYDO_DMA_A11n.qn_new(), ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /* p08.MOJY*/ wire2 _MOJY_A12p = mux2p(dma.LUMA_DMA_CARTp(), dma.NYGY_DMA_A12n.qn_new(), ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /* p08.MUCE*/ wire2 _MUCE_A13p = mux2p(dma.LUMA_DMA_CARTp(), dma.PULA_DMA_A13n.qn_new(), ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /* p08.PEGE*/ wire2 _PEGE_A14p = mux2p(dma.LUMA_DMA_CARTp(), dma.POKU_DMA_A14n.qn_new(), ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

  /* p08.KUPO*/ wire2 _KUPO = nand2(_AMET_A00p, rst.TOVA_MODE_DBG2n());
  /* p08.CABA*/ wire2 _CABA = nand2(_ATOL_A01p, rst.TOVA_MODE_DBG2n());
  /* p08.BOKU*/ wire2 _BOKU = nand2(_APOK_A02p, rst.TOVA_MODE_DBG2n());
  /* p08.BOTY*/ wire2 _BOTY = nand2(_AMER_A03p, rst.TOVA_MODE_DBG2n());
  /* p08.BYLA*/ wire2 _BYLA = nand2(_ATEM_A04p, rst.TOVA_MODE_DBG2n());
  /* p08.BADU*/ wire2 _BADU = nand2(_ATOV_A05p, rst.TOVA_MODE_DBG2n());
  /* p08.CEPU*/ wire2 _CEPU = nand2(_ATYR_A06p, rst.TOVA_MODE_DBG2n());
  /* p08.DEFY*/ wire2 _DEFY = nand2(_ASUR_A07p, rst.TOVA_MODE_DBG2n());
  /* p08.MYNY*/ wire2 _MYNY = nand2(_MANO_A08p, rst.TOVA_MODE_DBG2n());
  /* p08.MUNE*/ wire2 _MUNE = nand2(_MASU_A09p, rst.TOVA_MODE_DBG2n());
  /* p08.ROXU*/ wire2 _ROXU = nand2(_PAMY_A10p, rst.TOVA_MODE_DBG2n());
  /* p08.LEPY*/ wire2 _LEPY = nand2(_MALE_A11p, rst.TOVA_MODE_DBG2n());
  /* p08.LUCE*/ wire2 _LUCE = nand2(_MOJY_A12p, rst.TOVA_MODE_DBG2n());
  /* p08.LABE*/ wire2 _LABE = nand2(_MUCE_A13p, rst.TOVA_MODE_DBG2n());
  /* p08.PUHE*/ wire2 _PUHE = nand2(_PEGE_A14p, rst.TOVA_MODE_DBG2n());

  /* p08.KOTY*/ wire2 _KOTY = nor2 (_AMET_A00p, rst.UNOR_MODE_DBG2p());
  /* p08.COTU*/ wire2 _COTU = nor2 (_ATOL_A01p, rst.UNOR_MODE_DBG2p());
  /* p08.BAJO*/ wire2 _BAJO = nor2 (_APOK_A02p, rst.UNOR_MODE_DBG2p());
  /* p08.BOLA*/ wire2 _BOLA = nor2 (_AMER_A03p, rst.UNOR_MODE_DBG2p());
  /* p08.BEVO*/ wire2 _BEVO = nor2 (_ATEM_A04p, rst.UNOR_MODE_DBG2p());
  /* p08.AJAV*/ wire2 _AJAV = nor2 (_ATOV_A05p, rst.UNOR_MODE_DBG2p());
  /* p08.CYKA*/ wire2 _CYKA = nor2 (_ATYR_A06p, rst.UNOR_MODE_DBG2p());
  /* p08.COLO*/ wire2 _COLO = nor2 (_ASUR_A07p, rst.UNOR_MODE_DBG2p());
  /* p08.MEGO*/ wire2 _MEGO = nor2 (_MANO_A08p, rst.UNOR_MODE_DBG2p());
  /* p08.MENY*/ wire2 _MENY = nor2 (_MASU_A09p, rst.UNOR_MODE_DBG2p());
  /* p08.RORE*/ wire2 _RORE = nor2 (_PAMY_A10p, rst.UNOR_MODE_DBG2p());
  /* p08.LYNY*/ wire2 _LYNY = nor2 (_MALE_A11p, rst.UNOR_MODE_DBG2p());
  /* p08.LOSO*/ wire2 _LOSO = nor2 (_MOJY_A12p, rst.UNOR_MODE_DBG2p());
  /* p08.LEVA*/ wire2 _LEVA = nor2 (_MUCE_A13p, rst.UNOR_MODE_DBG2p());
  /* p08.PAHY*/ wire2 _PAHY = nor2 (_PEGE_A14p, rst.UNOR_MODE_DBG2p());

  PIN01_ADDR[ 0].pin_out_hilo(_KUPO, _KOTY);
  PIN01_ADDR[ 1].pin_out_hilo(_CABA, _COTU);
  PIN01_ADDR[ 2].pin_out_hilo(_BOKU, _BAJO);
  PIN01_ADDR[ 3].pin_out_hilo(_BOTY, _BOLA);
  PIN01_ADDR[ 4].pin_out_hilo(_BYLA, _BEVO);
  PIN01_ADDR[ 5].pin_out_hilo(_BADU, _AJAV);
  PIN01_ADDR[ 6].pin_out_hilo(_CEPU, _CYKA);
  PIN01_ADDR[ 7].pin_out_hilo(_DEFY, _COLO);
  PIN01_ADDR[ 8].pin_out_hilo(_MYNY, _MEGO);
  PIN01_ADDR[ 9].pin_out_hilo(_MUNE, _MENY);
  PIN01_ADDR[10].pin_out_hilo(_ROXU, _RORE);
  PIN01_ADDR[11].pin_out_hilo(_LEPY, _LYNY);
  PIN01_ADDR[12].pin_out_hilo(_LUCE, _LOSO);
  PIN01_ADDR[13].pin_out_hilo(_LABE, _LEVA);
  PIN01_ADDR[14].pin_out_hilo(_PUHE, _PAHY);

  // A15 is "special"
  /* p08.SOBY*/ wire2 _SOBY_A15n = nor2(cpu_bus.BUS_CPU_A[15].qp_new(), cpu_bus.TUTU_READ_BOOTROMp());
  /* p08.SEPY*/ wire2 _SEPY_A15p = nand2(ABUZ_EXT_RAM_CS_CLK, _SOBY_A15n);
  /* p08.TAZY*/ wire2 _TAZY_A15p = mux2p(dma.LUMA_DMA_CARTp(), dma.MARU_DMA_A15n.qn_new(), _SEPY_A15p);
  /* p08.SUZE*/ wire2 _SUZE_A15n = nand2(_TAZY_A15p, rst.RYCA_MODE_DBG2n());
  /* p08.RULO*/ wire2 _RULO_A15n = nor2 (_TAZY_A15p, rst.UNOR_MODE_DBG2p());
  PIN01_ADDR[15].pin_out_hilo(_SUZE_A15n, _RULO_A15n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::copy_pins_to_data_latch(const GateBoyCpuBus& cpu_bus)
{
  /* p08.LAVO*/ wire2 _LAVO_HOLDn = nand3(cpu_bus.SIG_CPU_RDp.qp_new(), cpu_bus.TEXO_ADDR_VRAMn(), cpu_bus.SIG_CPU_LATCH_EXT.qp_new());
  /*#p08.SOMA*/ ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn, PIN17_DATA[0].int_qp_new());
  /* p08.RONY*/ ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn, PIN17_DATA[1].int_qp_new());
  /* p08.RAXY*/ ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn, PIN17_DATA[2].int_qp_new());
  /* p08.SELO*/ ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn, PIN17_DATA[3].int_qp_new());
  /* p08.SODY*/ ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn, PIN17_DATA[4].int_qp_new());
  /* p08.SAGO*/ ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn, PIN17_DATA[5].int_qp_new());
  /* p08.RUPA*/ ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn, PIN17_DATA[6].int_qp_new());
  /* p08.SAZY*/ ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn, PIN17_DATA[7].int_qp_new());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::copy_data_latch_to_cpu_bus(GateBoyCpuBus& cpu_bus)
{
  /* p08.LAVO*/ wire2 _LAVO_HOLDn = nand3(cpu_bus.SIG_CPU_RDp.qp_new(), cpu_bus.TEXO_ADDR_VRAMn(), cpu_bus.SIG_CPU_LATCH_EXT.qp_new());
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

void GateBoyExtBus::copy_cpu_data_to_pins(const GateBoyResetDebug& rst, const GateBoyCpuBus& cpu_bus)
{
  // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
  // will actually end up in oam?

  /* p08.MOCA*/ wire2 _MOCA_DBG_EXT_RD = nor2(cpu_bus.TEXO_ADDR_VRAMn(), rst.UMUT_MODE_DBG1p());
  /* p08.LAGU*/ wire2 _LAGU = and_or3(cpu_bus.SIG_CPU_RDp.qp_new(), cpu_bus.LEVO_ADDR_VRAMn(), cpu_bus.SIG_CPU_WRp.qp_new());
  /* p08.LYWE*/ wire2 _LYWE = not1(_LAGU);
  /* p08.MOTY*/ wire2 _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
  /* p08.RORU*/ wire2 _RORU_CBD_TO_EPDn = mux2p(rst.UNOR_MODE_DBG2p(), cpu_bus.REDU_CPU_RDn(), _MOTY_CPU_EXT_RD);
  /* p08.LULA*/ wire2 _LULA_CBD_TO_EPDp = not1(_RORU_CBD_TO_EPDn);

  /* p25.RUXA*/ wire2 _RUXA = nand2(cpu_bus.BUS_CPU_D[0].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RUJA*/ wire2 _RUJA = nand2(cpu_bus.BUS_CPU_D[1].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RABY*/ wire2 _RABY = nand2(cpu_bus.BUS_CPU_D[2].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RERA*/ wire2 _RERA = nand2(cpu_bus.BUS_CPU_D[3].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RORY*/ wire2 _RORY = nand2(cpu_bus.BUS_CPU_D[4].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RYVO*/ wire2 _RYVO = nand2(cpu_bus.BUS_CPU_D[5].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RAFY*/ wire2 _RAFY = nand2(cpu_bus.BUS_CPU_D[6].qp_new(), _LULA_CBD_TO_EPDp);
  /* p25.RAVU*/ wire2 _RAVU = nand2(cpu_bus.BUS_CPU_D[7].qp_new(), _LULA_CBD_TO_EPDp);

  /* p08.RUNE*/ wire2 _RUNE = nor2 (cpu_bus.BUS_CPU_D[0].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RYPU*/ wire2 _RYPU = nor2 (cpu_bus.BUS_CPU_D[1].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.SULY*/ wire2 _SULY = nor2 (cpu_bus.BUS_CPU_D[2].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.SEZE*/ wire2 _SEZE = nor2 (cpu_bus.BUS_CPU_D[3].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RESY*/ wire2 _RESY = nor2 (cpu_bus.BUS_CPU_D[4].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.TAMU*/ wire2 _TAMU = nor2 (cpu_bus.BUS_CPU_D[5].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.ROGY*/ wire2 _ROGY = nor2 (cpu_bus.BUS_CPU_D[6].qp_new(), _RORU_CBD_TO_EPDn);
  /* p08.RYDA*/ wire2 _RYDA = nor2 (cpu_bus.BUS_CPU_D[7].qp_new(), _RORU_CBD_TO_EPDn);

  PIN17_DATA[0].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RUXA, _RUNE);
  PIN17_DATA[1].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RUJA, _RYPU);
  PIN17_DATA[2].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RABY, _SULY);
  PIN17_DATA[3].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RERA, _SEZE);
  PIN17_DATA[4].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RORY, _RESY);
  PIN17_DATA[5].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RYVO, _TAMU);
  PIN17_DATA[6].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RAFY, _ROGY);
  PIN17_DATA[7].pin_out_pull_hilo(_LULA_CBD_TO_EPDp, _RAVU, _RYDA);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::set_control_pins(
  const GateBoyResetDebug& rst,
  const GateBoyCpuBus& cpu_bus,
  const GateBoyDMA& dma,
  wire2 ABUZ_EXT_RAM_CS_CLK)
{
  /* p08.MOCA*/ wire2 _MOCA_DBG_EXT_RD = nor2(cpu_bus.TEXO_ADDR_VRAMn(), rst.UMUT_MODE_DBG1p());

  {
    /* p08.LAGU*/ wire2 _LAGU = and_or3(cpu_bus.SIG_CPU_RDp.qp_new(), cpu_bus.LEVO_ADDR_VRAMn(), cpu_bus.SIG_CPU_WRp.qp_new());
    /* p08.LYWE*/ wire2 _LYWE = not1(_LAGU);
    /* p08.MOTY*/ wire2 _MOTY_CPU_EXT_RD = or2(_MOCA_DBG_EXT_RD, _LYWE);
    /* p08.TYMU*/ wire2 _TYMU_EXT_RDn = nor2(dma.LUMA_DMA_CARTp(), _MOTY_CPU_EXT_RD);
    /* p08.UGAC*/ wire2 _UGAC_RD_A = nand2(_TYMU_EXT_RDn, rst.TOVA_MODE_DBG2n());
    /* p08.URUN*/ wire2 _URUN_RD_D = nor2 (_TYMU_EXT_RDn, rst.UNOR_MODE_DBG2p());
    PIN79_RDn.pin_out_hilo(_UGAC_RD_A, _URUN_RD_D);
  }

  {
    /* p08.NEVY*/ wire2 _NEVY = or2(cpu_bus.MEXO_CPU_WRn(), _MOCA_DBG_EXT_RD);
    /* p08.PUVA*/ wire2 _PUVA_EXT_WRn = or2(_NEVY, dma.LUMA_DMA_CARTp());
    /* p08.UVER*/ wire2 _UVER_WR_A = nand2(_PUVA_EXT_WRn, rst.TOVA_MODE_DBG2n());
    /* p08.USUF*/ wire2 _USUF_WR_D = nor2 (_PUVA_EXT_WRn, rst.UNOR_MODE_DBG2p());
    PIN78_WRn.pin_out_hilo(_UVER_WR_A, _USUF_WR_D);
  }

  {
    /* p08.TOZA*/ wire2 _TOZA_CS_A = and3(ABUZ_EXT_RAM_CS_CLK, cpu_bus.TYNU_A000_FFFFp(), cpu_bus.TUNA_0000_FDFF());
    /* p08.TYHO*/ wire2 _TYHO_CS_A = mux2p(dma.LUMA_DMA_CARTp(), dma.MARU_DMA_A15n.qn_new(), _TOZA_CS_A);
    PIN80_CSn.pin_out_dp(_TYHO_CS_A);
  }
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::read_ext_to_pins(const uint8_t* cart_buf, const uint8_t* cart_ram, const uint8_t* int_ram) {
  uint16_t addr = (uint16_t)BitBase::pack_ext_new(16, &PIN01_ADDR[ 0]);
  wire2 data = 0xFF;

  // ROM read
  //wire2 rom_CEn = PIN01_ADDR[15].ext_qp_new();
  //wire2 rom_OEp = !rom_CEn && !PIN79_RDn.ext_qp_new() && cart_buf;
  wire2 rom_OEp = (addr >= 0x0000) && (addr <= 0x7FFF) && !PIN79_RDn.ext_qp_new();
  if (rom_OEp) data = cart_buf[addr & 0x7FFF];

  // Cart RAM read
  //wire2 cram_CS1n = PIN80_CSn.ext_qp_new();
  //wire2 cram_CS2p = PIN01_ADDR[13].ext_qp_new() && !PIN01_ADDR[14].ext_qp_new() && PIN01_ADDR[15].ext_qp_new();
  //wire2 cram_OEp  = !cram_CS1n && cram_CS2p && !PIN79_RDn.ext_qp_new();
  wire2 cram_OEp = (addr >= 0xA000) && (addr <= 0xBFFF) && !PIN79_RDn.ext_qp_new();
  if (cram_OEp) data = cart_ram[addr & 0x1FFF];

  // Internal RAM read
  //wire2 iram_CS1n = PIN80_CSn.qp_new();
  //wire2 iram_CS2p = PIN01_ADDR[14].qp_new();
  //wire2 iram_OEp  = !iram_CS1n && iram_CS2p && !PIN79_RDn.qp_new();
  wire2 iram_OEp = (addr >= 0xC000) && (addr <= 0xDFFF) && !PIN79_RDn.ext_qp_new();
  if (iram_OEp) data = int_ram[addr & 0x1FFF];

  // Echo RAM read
  //wire2 eram_CS1n = PIN80_CSn.qp_new();
  //wire2 eram_CS2p = PIN01_ADDR[14].qp_new();
  //wire2 eram_OEp  = !eram_CS1n && eram_CS2p && !PIN79_RDn.qp_new();
  wire2 eram_OEp = (addr >= 0xE000) && (addr <= 0xFFFF) && !PIN79_RDn.ext_qp_new();
  if (eram_OEp) data = int_ram[addr & 0x1FFF];

  wire2 ext_OEp = rom_OEp || cram_OEp || iram_OEp || eram_OEp;
  PIN17_DATA[0].pin_in_oedp(ext_OEp, (data >> 0) & 1);
  PIN17_DATA[1].pin_in_oedp(ext_OEp, (data >> 1) & 1);
  PIN17_DATA[2].pin_in_oedp(ext_OEp, (data >> 2) & 1);
  PIN17_DATA[3].pin_in_oedp(ext_OEp, (data >> 3) & 1);
  PIN17_DATA[4].pin_in_oedp(ext_OEp, (data >> 4) & 1);
  PIN17_DATA[5].pin_in_oedp(ext_OEp, (data >> 5) & 1);
  PIN17_DATA[6].pin_in_oedp(ext_OEp, (data >> 6) & 1);
  PIN17_DATA[7].pin_in_oedp(ext_OEp, (data >> 7) & 1);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyExtBus::write_pins_to_ext(uint8_t* cart_ram, uint8_t* int_ram) {
  uint16_t addr = (uint16_t)BitBase::pack_ext_new(16, &PIN01_ADDR[ 0]);
  wire2 data = (wire2)BitBase::pack_ext_new(8, &PIN17_DATA[0]);

  // Cart RAM write
  //wire2 cram_CS1n = PIN80_CSn.qp_new();
  //wire2 cram_CS2p = PIN01_ADDR[13].qp_new() && !PIN01_ADDR[14].qp_new() && PIN01_ADDR[15].qp_new();
  //wire2 cram_WRp  = !cram_CS1n && cram_CS2p && !PIN78_WRn.qp_new();
  wire2 cram_WRp = (addr >= 0xA000) && (addr <= 0xBFFF) && !PIN78_WRn.ext_qp_new();
  if (cram_WRp) cart_ram[addr & 0x1FFF] = data;

  // Internal RAM write
  //wire2 iram_CS1n = PIN80_CSn.qp_new();
  //wire2 iram_CS2p = PIN01_ADDR[14].qp_new();
  //wire2 iram_WRp  = !iram_CS1n && iram_CS2p && !PIN78_WRn.qp_new();
  wire2 iram_WRp = (addr >= 0xC000) && (addr <= 0xDFFF) && !PIN78_WRn.ext_qp_new();
  if (iram_WRp) int_ram[addr & 0x1FFF]  = data;

  // Echo RAM write
  //wire2 eram_CS1n = PIN80_CSn.qp_new();
  //wire2 eram_CS2p = PIN01_ADDR[14].qp_new();
  //wire2 eram_WRp  = !eram_CS1n && iram_CS2p && !PIN78_WRn.qp_new();
  wire2 eram_WRp = (addr >= 0xE000) && (addr <= 0xFFFF) && !PIN78_WRn.ext_qp_new();
  if (eram_WRp) int_ram[addr & 0x1FFF]  = data;
}

//------------------------------------------------------------------------------------------------------------------------
