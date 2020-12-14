#include "GateBoyLib/GateBoyExtBus.h"

#include "GateBoyLib/GateBoyDMA.h"
#include "GateBoyLib/GateBoyResetDebug.h"

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

void ExtAddrLatch::tock(const GateBoyResetDebug& rstdbg, Signal BUS_CPU_A[16], wire TEXO_ADDR_VRAMn_ext) {
  /* p08.LOXO*/ wire _LOXO_HOLDn_new = and_or3(rstdbg.MULE_MODE_DBG1n_ext(), TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
  /* p08.LASY*/ wire _LASY_HOLDp_new = not1(_LOXO_HOLDn_new);
  /* p08.MATE*/ wire _MATE_HOLDn_new = not1(_LASY_HOLDp_new);
  /* p08.ALOR*/ ALOR_EXT_ADDR_LATCH_00p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 0]);
  /* p08.APUR*/ APUR_EXT_ADDR_LATCH_01p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 1]);
  /* p08.ALYR*/ ALYR_EXT_ADDR_LATCH_02p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 2]);
  /* p08.ARET*/ ARET_EXT_ADDR_LATCH_03p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 3]);
  /* p08.AVYS*/ AVYS_EXT_ADDR_LATCH_04p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 4]);
  /* p08.ATEV*/ ATEV_EXT_ADDR_LATCH_05p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 5]);
  /* p08.AROS*/ AROS_EXT_ADDR_LATCH_06p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 6]);
  /* p08.ARYM*/ ARYM_EXT_ADDR_LATCH_07p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 7]);
  /* p08.LUNO*/ LUNO_EXT_ADDR_LATCH_08p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 8]);
  /* p08.LYSA*/ LYSA_EXT_ADDR_LATCH_09p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[ 9]);
  /* p08.PATE*/ PATE_EXT_ADDR_LATCH_10p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[10]);
  /* p08.LUMY*/ LUMY_EXT_ADDR_LATCH_11p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[11]);
  /* p08.LOBU*/ LOBU_EXT_ADDR_LATCH_12p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[12]);
  /* p08.LONU*/ LONU_EXT_ADDR_LATCH_13p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[13]);
  /* p08.NYRE*/ NYRE_EXT_ADDR_LATCH_14p.tp_latch(_MATE_HOLDn_new, BUS_CPU_A[14]);
}

void GateBoyExtBus::addr_latch_to_pins(
  const GateBoyResetDebug& rstdbg,
  const GateBoyDMA& dma,
  const ExtAddrLatch& ext_addr_latch,
  Signal BUS_CPU_A[16],
  wire ABUZ_xxCDEFGH_clk_evn,
  wire TUTU_READ_BOOTROMp_new
)
{
  /* p08.AMET*/ wire _AMET_A00p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NAKY_DMA_A00p_evn.qp_new(), ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /* p08.ATOL*/ wire _ATOL_A01p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PYRO_DMA_A01p_evn.qp_new(), ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /* p08.APOK*/ wire _APOK_A02p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NEFY_DMA_A02p_evn.qp_new(), ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /* p08.AMER*/ wire _AMER_A03p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MUTY_DMA_A03p_evn.qp_new(), ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /* p08.ATEM*/ wire _ATEM_A04p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NYKO_DMA_A04p_evn.qp_new(), ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /* p08.ATOV*/ wire _ATOV_A05p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PYLO_DMA_A05p_evn.qp_new(), ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /* p08.ATYR*/ wire _ATYR_A06p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NUTO_DMA_A06p_evn.qp_new(), ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire _ASUR_A07p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MUGU_DMA_A07p_evn.qp_new(), ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire _MANO_A08p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NAFA_DMA_A08n_h.qn_new(),   ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /* p08.MASU*/ wire _MASU_A09p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PYNE_DMA_A09n_h.qn_new(),   ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /* p08.PAMY*/ wire _PAMY_A10p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PARA_DMA_A10n_h.qn_new(),   ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /* p08.MALE*/ wire _MALE_A11p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NYDO_DMA_A11n_h.qn_new(),   ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /* p08.MOJY*/ wire _MOJY_A12p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.NYGY_DMA_A12n_h.qn_new(),   ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /* p08.MUCE*/ wire _MUCE_A13p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.PULA_DMA_A13n_h.qn_new(),   ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /* p08.PEGE*/ wire _PEGE_A14p_new_evn = mux2p(dma.LUMA_DMA_CARTp_new(), dma.POKU_DMA_A14n_h.qn_new(),   ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

  /* p08.KUPO*/ wire _KUPO_new_evn = nand2(_AMET_A00p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.CABA*/ wire _CABA_new_evn = nand2(_ATOL_A01p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BOKU*/ wire _BOKU_new_evn = nand2(_APOK_A02p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BOTY*/ wire _BOTY_new_evn = nand2(_AMER_A03p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BYLA*/ wire _BYLA_new_evn = nand2(_ATEM_A04p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.BADU*/ wire _BADU_new_evn = nand2(_ATOV_A05p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.CEPU*/ wire _CEPU_new_evn = nand2(_ATYR_A06p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.DEFY*/ wire _DEFY_new_evn = nand2(_ASUR_A07p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.MYNY*/ wire _MYNY_new_evn = nand2(_MANO_A08p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.MUNE*/ wire _MUNE_new_evn = nand2(_MASU_A09p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.ROXU*/ wire _ROXU_new_evn = nand2(_PAMY_A10p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.LEPY*/ wire _LEPY_new_evn = nand2(_MALE_A11p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.LUCE*/ wire _LUCE_new_evn = nand2(_MOJY_A12p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.LABE*/ wire _LABE_new_evn = nand2(_MUCE_A13p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.PUHE*/ wire _PUHE_new_evn = nand2(_PEGE_A14p_new_evn, rstdbg.TOVA_MODE_DBG2n_ext());

  /* p08.KOTY*/ wire _KOTY_new_evn = nor2 (_AMET_A00p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.COTU*/ wire _COTU_new_evn = nor2 (_ATOL_A01p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.BAJO*/ wire _BAJO_new_evn = nor2 (_APOK_A02p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.BOLA*/ wire _BOLA_new_evn = nor2 (_AMER_A03p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.BEVO*/ wire _BEVO_new_evn = nor2 (_ATEM_A04p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.AJAV*/ wire _AJAV_new_evn = nor2 (_ATOV_A05p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.CYKA*/ wire _CYKA_new_evn = nor2 (_ATYR_A06p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.COLO*/ wire _COLO_new_evn = nor2 (_ASUR_A07p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.MEGO*/ wire _MEGO_new_evn = nor2 (_MANO_A08p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.MENY*/ wire _MENY_new_evn = nor2 (_MASU_A09p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.RORE*/ wire _RORE_new_evn = nor2 (_PAMY_A10p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.LYNY*/ wire _LYNY_new_evn = nor2 (_MALE_A11p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.LOSO*/ wire _LOSO_new_evn = nor2 (_MOJY_A12p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.LEVA*/ wire _LEVA_new_evn = nor2 (_MUCE_A13p_new_evn, rstdbg.UNOR_MODE_DBG2p());
  /* p08.PAHY*/ wire _PAHY_new_evn = nor2 (_PEGE_A14p_new_evn, rstdbg.UNOR_MODE_DBG2p());

  PIN_EXT_A[ 0].pin_out(_KUPO_new_evn, _KOTY_new_evn);
  PIN_EXT_A[ 1].pin_out(_CABA_new_evn, _COTU_new_evn);
  PIN_EXT_A[ 2].pin_out(_BOKU_new_evn, _BAJO_new_evn);
  PIN_EXT_A[ 3].pin_out(_BOTY_new_evn, _BOLA_new_evn);
  PIN_EXT_A[ 4].pin_out(_BYLA_new_evn, _BEVO_new_evn);
  PIN_EXT_A[ 5].pin_out(_BADU_new_evn, _AJAV_new_evn);
  PIN_EXT_A[ 6].pin_out(_CEPU_new_evn, _CYKA_new_evn);
  PIN_EXT_A[ 7].pin_out(_DEFY_new_evn, _COLO_new_evn);
  PIN_EXT_A[ 8].pin_out(_MYNY_new_evn, _MEGO_new_evn);
  PIN_EXT_A[ 9].pin_out(_MUNE_new_evn, _MENY_new_evn);
  PIN_EXT_A[10].pin_out(_ROXU_new_evn, _RORE_new_evn);
  PIN_EXT_A[11].pin_out(_LEPY_new_evn, _LYNY_new_evn);
  PIN_EXT_A[12].pin_out(_LUCE_new_evn, _LOSO_new_evn);
  PIN_EXT_A[13].pin_out(_LABE_new_evn, _LEVA_new_evn);
  PIN_EXT_A[14].pin_out(_PUHE_new_evn, _PAHY_new_evn);

  // A15 is "special"
  /* p08.SOBY*/ wire _SOBY_A15n_new = nor2(BUS_CPU_A[15], TUTU_READ_BOOTROMp_new);
  /* p08.SEPY*/ wire _SEPY_A15p_ABxxxxxx_clknew = nand2(ABUZ_xxCDEFGH_clk_evn, _SOBY_A15n_new);
  /* p08.TAZY*/ wire _TAZY_A15p_new = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MARU_DMA_A15n_h.qn_new(), _SEPY_A15p_ABxxxxxx_clknew);
  /* p08.SUZE*/ wire _SUZE_PIN_EXT_A15n_new = nand2(_TAZY_A15p_new, rstdbg.RYCA_MODE_DBG2n_ext());
  /* p08.RULO*/ wire _RULO_PIN_EXT_A15n_new = nor2 (_TAZY_A15p_new, rstdbg.UNOR_MODE_DBG2p());
  PIN_EXT_A[15].pin_out(_SUZE_PIN_EXT_A15n_new, _RULO_PIN_EXT_A15n_new);
}

void GateBoyExtBus::pins_to_data_latch(
  wire PIN_CPU_RDp,
  wire PIN_CPU_LATCH_EXT,
  wire TEXO_ADDR_VRAMn_ext,
  ExtDataLatch& ext_data_latch)
{
  // External data bus -> latch -> CPU data bus
  /* p08.LAVO*/ wire _LAVO_HOLDn_new = nand3(PIN_CPU_RDp, TEXO_ADDR_VRAMn_ext, PIN_CPU_LATCH_EXT);
  /*#p08.SOMA*/ ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[0].qn_new());
  /* p08.RONY*/ ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[1].qn_new());
  /* p08.RAXY*/ ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[2].qn_new());
  /* p08.SELO*/ ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[3].qn_new());
  /* p08.SODY*/ ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[4].qn_new());
  /* p08.SAGO*/ ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[5].qn_new());
  /* p08.RUPA*/ ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[6].qn_new());
  /* p08.SAZY*/ ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latch(_LAVO_HOLDn_new, PIN_EXT_D[7].qn_new());
}

void GateBoyExtBus::data_latch_to_cpu_bus(
  const ExtDataLatch& ext_data_latch,
  wire PIN_CPU_RDp,
  wire PIN_CPU_LATCH_EXT,
  wire TEXO_ADDR_VRAMn_ext,
  BusOut BUS_CPU_D_out[8])
{
  // External data bus -> latch -> CPU data bus
  /* p08.LAVO*/ wire _LAVO_HOLDn_new = nand3(PIN_CPU_RDp, TEXO_ADDR_VRAMn_ext, PIN_CPU_LATCH_EXT);
  /*#p08.RYMA*/ BUS_CPU_D_out[0].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
  /* p08.RUVO*/ BUS_CPU_D_out[1].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
  /* p08.RYKO*/ BUS_CPU_D_out[2].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
  /* p08.TAVO*/ BUS_CPU_D_out[3].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
  /* p08.TEPE*/ BUS_CPU_D_out[4].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
  /* p08.SAFO*/ BUS_CPU_D_out[5].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
  /* p08.SEVU*/ BUS_CPU_D_out[6].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
  /* p08.TAJU*/ BUS_CPU_D_out[7].tri6_nn(_LAVO_HOLDn_new, ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());
}

void GateBoyExtBus::cpu_data_to_pins(
  const GateBoyResetDebug& rstdbg,
  Signal BUS_CPU_D[8],
  wire PIN_CPU_RDp,
  wire PIN_CPU_WRp,
  wire TEDO_CPU_RDp,
  wire TEXO_ADDR_VRAMn)
{
  // FIXME So does this mean that if the CPU writes to the external bus during dma, that data
  // will actually end up in oam?

  /* p08.REDU*/ wire _REDU_CPU_RDn_ext = not1(TEDO_CPU_RDp);
  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn, rstdbg.UMUT_MODE_DBG1p_ext());
  /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn);
  /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp, _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp);
  /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
  /* p08.RORU*/ wire _RORU_CBD_TO_EPDn_new = mux2p(rstdbg.UNOR_MODE_DBG2p(), _REDU_CPU_RDn_ext, _MOTY_CPU_EXT_RD_ext);
  /* p08.LULA*/ wire _LULA_CBD_TO_EPDp_new = not1(_RORU_CBD_TO_EPDn_new);

  /* p25.RUXA*/ wire _RUXA_new = nand2(BUS_CPU_D[0], _LULA_CBD_TO_EPDp_new);
  /* p25.RUJA*/ wire _RUJA_new = nand2(BUS_CPU_D[1], _LULA_CBD_TO_EPDp_new);
  /* p25.RABY*/ wire _RABY_new = nand2(BUS_CPU_D[2], _LULA_CBD_TO_EPDp_new);
  /* p25.RERA*/ wire _RERA_new = nand2(BUS_CPU_D[3], _LULA_CBD_TO_EPDp_new);
  /* p25.RORY*/ wire _RORY_new = nand2(BUS_CPU_D[4], _LULA_CBD_TO_EPDp_new);
  /* p25.RYVO*/ wire _RYVO_new = nand2(BUS_CPU_D[5], _LULA_CBD_TO_EPDp_new);
  /* p25.RAFY*/ wire _RAFY_new = nand2(BUS_CPU_D[6], _LULA_CBD_TO_EPDp_new);
  /* p25.RAVU*/ wire _RAVU_new = nand2(BUS_CPU_D[7], _LULA_CBD_TO_EPDp_new);

  /* p08.RUNE*/ wire _RUNE_new = nor2 (BUS_CPU_D[0], _RORU_CBD_TO_EPDn_new);
  /* p08.RYPU*/ wire _RYPU_new = nor2 (BUS_CPU_D[1], _RORU_CBD_TO_EPDn_new);
  /* p08.SULY*/ wire _SULY_new = nor2 (BUS_CPU_D[2], _RORU_CBD_TO_EPDn_new);
  /* p08.SEZE*/ wire _SEZE_new = nor2 (BUS_CPU_D[3], _RORU_CBD_TO_EPDn_new);
  /* p08.RESY*/ wire _RESY_new = nor2 (BUS_CPU_D[4], _RORU_CBD_TO_EPDn_new);
  /* p08.TAMU*/ wire _TAMU_new = nor2 (BUS_CPU_D[5], _RORU_CBD_TO_EPDn_new);
  /* p08.ROGY*/ wire _ROGY_new = nor2 (BUS_CPU_D[6], _RORU_CBD_TO_EPDn_new);
  /* p08.RYDA*/ wire _RYDA_new = nor2 (BUS_CPU_D[7], _RORU_CBD_TO_EPDn_new);

  PIN_EXT_D[0].pin_out(_LULA_CBD_TO_EPDp_new, _RUXA_new, _RUNE_new);
  PIN_EXT_D[1].pin_out(_LULA_CBD_TO_EPDp_new, _RUJA_new, _RYPU_new);
  PIN_EXT_D[2].pin_out(_LULA_CBD_TO_EPDp_new, _RABY_new, _SULY_new);
  PIN_EXT_D[3].pin_out(_LULA_CBD_TO_EPDp_new, _RERA_new, _SEZE_new);
  PIN_EXT_D[4].pin_out(_LULA_CBD_TO_EPDp_new, _RORY_new, _RESY_new);
  PIN_EXT_D[5].pin_out(_LULA_CBD_TO_EPDp_new, _RYVO_new, _TAMU_new);
  PIN_EXT_D[6].pin_out(_LULA_CBD_TO_EPDp_new, _RAFY_new, _ROGY_new);
  PIN_EXT_D[7].pin_out(_LULA_CBD_TO_EPDp_new, _RAVU_new, _RYDA_new);
}

void GateBoyExtBus::set_pins(
  const GateBoyResetDebug& rstdbg,
  const GateBoyDMA& dma,
  Signal BUS_CPU_A[16],
  wire PIN_CPU_RDp,
  wire PIN_CPU_WRp,
  wire ABUZ_xxCDEFGH_clk_evn,
  wire TEXO_ADDR_VRAMn_ext,
  wire APOV_CPU_WRp_clkevn)
{
  /* p08.MOCA*/ wire _MOCA_DBG_EXT_RD_ext = nor2(TEXO_ADDR_VRAMn_ext, rstdbg.UMUT_MODE_DBG1p_ext());
  /* p08.LEVO*/ wire _LEVO_ADDR_INT_OR_ADDR_VRAM_ext = not1(TEXO_ADDR_VRAMn_ext);
  /* p08.LAGU*/ wire _LAGU_ext = and_or3(PIN_CPU_RDp, _LEVO_ADDR_INT_OR_ADDR_VRAM_ext, PIN_CPU_WRp);
  /* p08.LYWE*/ wire _LYWE_ext = not1(_LAGU_ext);
  /* p08.MOTY*/ wire _MOTY_CPU_EXT_RD_ext = or2(_MOCA_DBG_EXT_RD_ext, _LYWE_ext);
  /* p08.TYMU*/ wire _TYMU_EXT_RDn_new = nor2(dma.LUMA_DMA_CARTp_new(), _MOTY_CPU_EXT_RD_ext);
  /* p08.UGAC*/ wire _UGAC_RD_A_new = nand2(_TYMU_EXT_RDn_new, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.URUN*/ wire _URUN_RD_D_new = nor2 (_TYMU_EXT_RDn_new, rstdbg.UNOR_MODE_DBG2p());
  PIN_EXT_RDn.pin_out(_UGAC_RD_A_new, _URUN_RD_D_new);

  /* p08.MEXO*/ wire _MEXO_CPU_WRn_new = not1(APOV_CPU_WRp_clkevn);
  /* p08.NEVY*/ wire _NEVY_new = or2(_MEXO_CPU_WRn_new, _MOCA_DBG_EXT_RD_ext);
  /* p08.PUVA*/ wire _PUVA_EXT_WRn_new = or2(_NEVY_new, dma.LUMA_DMA_CARTp_new());
  /* p08.UVER*/ wire _UVER_WR_A_new = nand2(_PUVA_EXT_WRn_new, rstdbg.TOVA_MODE_DBG2n_ext());
  /* p08.USUF*/ wire _USUF_WR_D_new = nor2 (_PUVA_EXT_WRn_new, rstdbg.UNOR_MODE_DBG2p());
  PIN_EXT_WRn.pin_out(_UVER_WR_A_new, _USUF_WR_D_new);

  /* p08.TOZA*/ wire _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk = and3(ABUZ_xxCDEFGH_clk_evn, TYNU_A000_FFFFp_ext(BUS_CPU_A), TUNA_0000_FDFF_ext(BUS_CPU_A));
  /* p08.TYHO*/ wire _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew = mux2p(dma.LUMA_DMA_CARTp_new(), dma.MARU_DMA_A15n_h.qn_new(), _TOZA_PIN_EXT_CS_A_xxCDEFGH_clk);
  PIN_EXT_CSn.pin_out(_TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew, _TYHO_PIN_EXT_CS_A_xxCDEFGH_clknew);
}

// The actual "cart" stuff.

void GateBoyExtBus::cart_to_pins(const uint8_t* cart_buf, uint8_t* cart_ram, uint8_t* ext_ram) {
  uint16_t ext_addr_latch = pack_u16p(16, &PIN_EXT_A[ 0]);
  uint8_t ext_data_latch = pack_u8p(8, &PIN_EXT_D[0]);

  // ROM read
  uint16_t rom_addr = ext_addr_latch & 0x7FFF;
  wire rom_CEn_new = PIN_EXT_A[15].qp_new();
  wire rom_OEp_new = !rom_CEn_new && !PIN_EXT_RDn.qp_new() && cart_buf;
  if (rom_OEp_new)  ext_data_latch = cart_buf[rom_addr];

  // Ext RAM read
  uint16_t eram_addr = (ext_addr_latch & 0x1FFF);
  wire eram_CE1n_new = PIN_EXT_CSn.qp_new();
  wire eram_CE2_new  = PIN_EXT_A[14].qp_new();
  wire eram_WRp_new  = !eram_CE1n_new && eram_CE2_new && !PIN_EXT_WRn.qp_new();
  wire eram_OEp_new  = !eram_CE1n_new && eram_CE2_new && !eram_WRp_new && !PIN_EXT_RDn.qp_new();
  if (eram_WRp_new) ext_ram[eram_addr]  = ext_data_latch;
  if (eram_OEp_new) ext_data_latch = ext_ram[eram_addr];

  // Cart RAM read
  uint16_t cram_addr = (ext_addr_latch & 0x1FFF);
  wire cram_CS1n_new = PIN_EXT_CSn.qp_new();
  wire cram_CS2_new  = PIN_EXT_A[13].qp_new() && !PIN_EXT_A[14].qp_new() && PIN_EXT_A[15].qp_new();
  wire cram_WRp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_WRn.qp_new();
  wire cram_OEp_new  = !cram_CS1n_new && cram_CS2_new && !PIN_EXT_RDn.qp_new();

  if (cram_WRp_new) cart_ram[cram_addr] = ext_data_latch;
  if (cram_OEp_new) ext_data_latch = cart_ram[cram_addr];

  wire ext_OEp = rom_OEp_new || eram_OEp_new || cram_OEp_new;
  PIN_EXT_D[0].pin_in(ext_OEp, wire(ext_data_latch & 0x01));
  PIN_EXT_D[1].pin_in(ext_OEp, wire(ext_data_latch & 0x02));
  PIN_EXT_D[2].pin_in(ext_OEp, wire(ext_data_latch & 0x04));
  PIN_EXT_D[3].pin_in(ext_OEp, wire(ext_data_latch & 0x08));
  PIN_EXT_D[4].pin_in(ext_OEp, wire(ext_data_latch & 0x10));
  PIN_EXT_D[5].pin_in(ext_OEp, wire(ext_data_latch & 0x20));
  PIN_EXT_D[6].pin_in(ext_OEp, wire(ext_data_latch & 0x40));
  PIN_EXT_D[7].pin_in(ext_OEp, wire(ext_data_latch & 0x80));
}