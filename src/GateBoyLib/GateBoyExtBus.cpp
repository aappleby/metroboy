#include "GateBoyLib/GateBoyExtBus.h"

#include "CoreLib/Log.h"
#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

uint32_t cart_rom_addr_mask(const blob& cart_blob) {
  switch (cart_blob[0x0148]) {
  case 0:  return 0x00007FFF; // 32K
  case 1:  return 0x0000FFFF; // 64K
  case 2:  return 0x0001FFFF; // 128K
  case 3:  return 0x0003FFFF; // 256K
  case 4:  return 0x0007FFFF; // 512K
  case 5:  return 0x000FFFFF; // 1M
  case 6:  return 0x001FFFFF; // 2M
  case 7:  return 0x003FFFFF; // 4M
  case 8:  return 0x007FFFFF; // 8M
  default: return 0x00007FFF;
  }
}

uint32_t cart_ram_addr_mask(const blob& cart_blob) {
  // these masks are only for mbc1
  switch (cart_blob[0x0149]) {
  case 0:  return 0x00000000;
  case 1:  return 0x000007FF;
  case 2:  return 0x00001FFF;
  case 3:  return 0x00007FFF;
  default: return 0x00000000;
  }
}

bool cart_has_mbc1(const blob& cart_blob) {
  switch (cart_blob[0x0147]) {
  case 0x00: return 0;
  case 0x01: return 1;
  case 0x02: return 1;
  case 0x03: return 1;
  case 0x08: return 0;
  case 0x09: return 0;
  case 0x19: return 1; // MBC5
  case 0x1A: return 1; // MBC5+RAM
  case 0x1B: return 1; // MBC5+RAM+BATTERY
  case 0x1C: return 1; // MBC5+RUMBLE
  case 0x1D: return 1; // MBC5+RUMBLE+RAM
  case 0x1E: return 1; // MBC5+RUMBLE+RAM+BATTERY
  default: {
    LOG_R("Bad cart type! 0x%02x\n", cart_blob[0x0147]);
    return false;
  }
  }
}

bool cart_has_ram(const blob& cart_blob) {
  switch (cart_blob[0x0147]) {
  case 0x00: return 0;
  case 0x01: return 0;
  case 0x02: return 1;
  case 0x03: return 1;
  case 0x08: return 1;
  case 0x09: return 1;
  case 0x19: return 0; // MBC5
  case 0x1A: return 1; // MBC5+RAM
  case 0x1B: return 1; // MBC5+RAM+BATTERY
  case 0x1C: return 0; // MBC5+RUMBLE
  case 0x1D: return 1; // MBC5+RUMBLE+RAM
  case 0x1E: return 1; // MBC5+RUMBLE+RAM+BATTERY
  default: {
    LOG_R("Bad cart type! 0x%02x\n", cart_blob[0x0147]);
    return 0;
  }
  }
}


uint8_t GateBoy::read_flat_addr(const blob& cart_blob, int addr) const {
  if (!bit0(gb_state.cpu_signals.TEPU_BOOT_BITn.state) && addr >= 0x0000 && addr < 0x0100) {
    return DMG_ROM_blob[addr];
  }
  else if (addr >= 0x0000 && addr < 0x4000) {
    return cart_blob[addr & 0x7FFF];

    // FIXME why were we mucking with bank 0?
    /*
    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit(gb_state.ext_mbc.MBC1_MODE.out_old());
      bool mbc1_ram_en = bit(gb_state.ext_mbc.MBC1_RAM_EN.out_old());
      uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&gb_state.ext_mbc.MBC1_BANK5, 2) : 0;
      uint32_t mbc1_rom0_addr = ((addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & cart_rom_addr_mask(cart_blob);
      CHECK_P(mbc1_rom0_addr < cart_blob.size());
      return &cart_blob[mbc1_rom0_addr];
    }
    else {
      return &cart_blob[addr & 0x7FFF];
    }
    */
  }
  else if (addr >= 0x4000 && addr < 0x8000) {
    if (cart_has_mbc1(cart_blob)) {
      uint32_t mbc1_rom1_bank = bit_pack(&gb_state.ext_mbc.MBC1_BANK0, 7);
      if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
      uint32_t mbc1_rom1_addr = ((addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & cart_rom_addr_mask(cart_blob);
      CHECK_P(mbc1_rom1_addr < cart_blob.size());
      return cart_blob[mbc1_rom1_addr];
    }
    else {
      return cart_blob[addr & 0x7FFF];
    }
  }
  else if (addr >= 0x8000 && addr < 0xA000) {
    return mem.vid_ram[addr - 0x8000];
  }
  else if (addr >= 0xA000 && addr < 0xC000) {
    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit0(gb_state.ext_mbc.MBC1_MODE.out_old());
      bool mbc1_ram_en = bit0(gb_state.ext_mbc.MBC1_RAM_EN.out_old());
      uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&gb_state.ext_mbc.MBC1_BANK5, 2) : 0;
      if (mbc1_mode == 0) mbc1_ram_bank = 0;
      uint32_t mbc1_ram_addr = ((addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);
      CHECK_P(mbc1_ram_addr < 32768);
      return mem.cart_ram[mbc1_ram_addr];
    }
    else {
      return mem.cart_ram[addr & 0x1FFF];
    }
  }
  else if (addr >= 0xC000 && addr < 0xE000) {
    return mem.int_ram[addr & 0x1FFF];
  }
  else if (addr >= 0xE000 && addr < 0xFE00) {
    return mem.int_ram[addr & 0x1FFF];
  }
  else if (addr >= 0xFE00 && addr < 0xFF00) {
    return mem.oam_ram[addr & 0x00FF];
  }
  else if (addr >= 0xFF80 && addr < 0xFFFF) {
    return mem.zero_ram[addr & 0x007F];
  }
  else {
    printf("read_flat_addr : bad addr 0x%08x @ phase %llu\n", addr, (long long unsigned int)sys.gb_phase_total);
    //debugbreak();
    return 0;
  }
}

void GateBoy::get_flat_blob(const blob& cart_blob, int addr, int size, blob& out) const {
  if (addr + size >= 0xFFFF) {
    size = 0xFFFF - addr;
  }

  out.resize(size);
  for (int i = 0; i < size; i++) {
    out[i] = read_flat_addr(cart_blob, addr + i);
  }
}


//-----------------------------------------------------------------------------

void GateBoy::tock_ext_gates(const GateBoyState& reg_old, const blob& cart_blob)
{
  auto& reg_new = gb_state;

  /*_p08.MOCA*/ wire MOCA_DBG_EXT_RD_new = nor2(reg_new.TEXO_ADDR_VRAMn_new(), pins.sys.UMUT_MODE_DBG1p_new());

  /*#p04.LEBU*/ wire LEBU_DMA_A15n_new  = not1(reg_new.reg_dma.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire MUDA_DMA_VRAMp_new = nor3(reg_new.reg_dma.PULA_DMA_A13n.qn_new(), reg_new.reg_dma.POKU_DMA_A14n.qn_new(), LEBU_DMA_A15n_new);
  /*_p04.LOGO*/ wire LOGO_DMA_VRAMn_new = not1(MUDA_DMA_VRAMp_new);
  /*_p04.MORY*/ wire MORY_DMA_CARTn_new = nand2(reg_new.MATU_DMA_RUNNINGp_odd.qp_new(), LOGO_DMA_VRAMn_new);
  /*_p04.LUMA*/ wire LUMA_DMA_CARTp_new = not1(MORY_DMA_CARTn_new);

  {
    /*_p08.LAGU*/ wire LAGU_new = and_or3(reg_new.cpu_signals.SIG_IN_CPU_RDp.out_new(), reg_new.LEVO_ADDR_VRAMn_new(), reg_new.cpu_signals.SIG_IN_CPU_WRp.out_new());
    /*_p08.LYWE*/ wire LYWE_new = not1(LAGU_new);
    /*_p08.MOTY*/ wire MOTY_CPU_EXT_RD_new = or2(MOCA_DBG_EXT_RD_new, LYWE_new);
    /*_p08.TYMU*/ wire TYMU_EXT_RDn_new = nor2(LUMA_DMA_CARTp_new, MOTY_CPU_EXT_RD_new);
    /*_p08.UGAC*/ wire UGAC_RD_A_new = nand2(TYMU_EXT_RDn_new, pins.sys.TOVA_MODE_DBG2n_new());
    /*_p08.URUN*/ wire URUN_RD_D_new = nor2 (TYMU_EXT_RDn_new, pins.sys.UNOR_MODE_DBG2p_new());
    /*_PIN_79*/ pins.ctrl.PIN_79_RDn.pin_out(UGAC_RD_A_new, URUN_RD_D_new);
  }

  {
    /*_p08.NEVY*/ wire NEVY_new = or2(reg_new.cpu_signals.MEXO_CPU_WRn_new(), MOCA_DBG_EXT_RD_new);
    /*_p08.PUVA*/ wire PUVA_EXT_WRn_new = or2(NEVY_new, LUMA_DMA_CARTp_new);
    /*_p08.UVER*/ wire UVER_WR_A_new = nand2(PUVA_EXT_WRn_new, pins.sys.TOVA_MODE_DBG2n_new());
    /*_p08.USUF*/ wire USUF_WR_D_new = nor2 (PUVA_EXT_WRn_new, pins.sys.UNOR_MODE_DBG2p_new());
    /*_PIN_78*/ pins.ctrl.PIN_78_WRn.pin_out(UVER_WR_A_new, USUF_WR_D_new);
  }

  {
    /*_p08.TOZA*/ wire TOZA_CS_A_new = and3(reg_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new(), reg_new.cpu_abus.TYNU_A000_FFFFp_new(), reg_new.cpu_abus.TUNA_0000_FDFF_new());
    /*_p08.TYHO*/ wire TYHO_CS_A_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.MARU_DMA_A15n.qn_new(), TOZA_CS_A_new);
    /*_PIN_80*/ pins.ctrl.PIN_80_CSn.pin_out(TYHO_CS_A_new, TYHO_CS_A_new);
  }

  /*_p08.LOXO*/ wire LOXO_HOLDn_new = and_or3(pins.sys.MULE_MODE_DBG1n_new(), reg_new.TEXO_ADDR_VRAMn_new(), pins.sys.UMUT_MODE_DBG1p_new());
  /*_p08.LASY*/ wire LASY_HOLDp_new = not1(LOXO_HOLDn_new);
  /*_p08.MATE*/ wire MATE_HOLDn_new = not1(LASY_HOLDp_new);
  /*_p08.ALOR*/ reg_new.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A00p.out_new());
  /*_p08.APUR*/ reg_new.ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A01p.out_new());
  /*_p08.ALYR*/ reg_new.ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A02p.out_new());
  /*_p08.ARET*/ reg_new.ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A03p.out_new());
  /*_p08.AVYS*/ reg_new.ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A04p.out_new());
  /*_p08.ATEV*/ reg_new.ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A05p.out_new());
  /*_p08.AROS*/ reg_new.ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A06p.out_new());
  /*_p08.ARYM*/ reg_new.ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A07p.out_new());
  /*_p08.LUNO*/ reg_new.ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A08p.out_new());
  /*_p08.LYSA*/ reg_new.ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A09p.out_new());
  /*_p08.PATE*/ reg_new.ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A10p.out_new());
  /*_p08.LUMY*/ reg_new.ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A11p.out_new());
  /*_p08.LOBU*/ reg_new.ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A12p.out_new());
  /*_p08.LONU*/ reg_new.ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A13p.out_new());
  /*_p08.NYRE*/ reg_new.ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.tp_latchn(MATE_HOLDn_new, reg_new.cpu_abus.BUS_CPU_A14p.out_new());


  /*_p08.AMET*/ wire AMET_A00p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.NAKY_DMA_A00p_odd.qp_new(), reg_new.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /*_p08.ATOL*/ wire ATOL_A01p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.PYRO_DMA_A01p_odd.qp_new(), reg_new.ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /*_p08.APOK*/ wire APOK_A02p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.NEFY_DMA_A02p_odd.qp_new(), reg_new.ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /*_p08.AMER*/ wire AMER_A03p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.MUTY_DMA_A03p_odd.qp_new(), reg_new.ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /*_p08.ATEM*/ wire ATEM_A04p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.NYKO_DMA_A04p_odd.qp_new(), reg_new.ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /*_p08.ATOV*/ wire ATOV_A05p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.PYLO_DMA_A05p_odd.qp_new(), reg_new.ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /*_p08.ATYR*/ wire ATYR_A06p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.NUTO_DMA_A06p_odd.qp_new(), reg_new.ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire ASUR_A07p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.dma_lo.MUGU_DMA_A07p_odd.qp_new(), reg_new.ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire MANO_A08p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.NAFA_DMA_A08n.qn_new(),    reg_new.ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /*_p08.MASU*/ wire MASU_A09p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.PYNE_DMA_A09n.qn_new(),    reg_new.ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /*_p08.PAMY*/ wire PAMY_A10p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.PARA_DMA_A10n.qn_new(),    reg_new.ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /*_p08.MALE*/ wire MALE_A11p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.NYDO_DMA_A11n.qn_new(),    reg_new.ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /*_p08.MOJY*/ wire MOJY_A12p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.NYGY_DMA_A12n.qn_new(),    reg_new.ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /*_p08.MUCE*/ wire MUCE_A13p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.PULA_DMA_A13n.qn_new(),    reg_new.ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /*_p08.PEGE*/ wire PEGE_A14p_new = mux2p(LUMA_DMA_CARTp_new, reg_new.reg_dma.POKU_DMA_A14n.qn_new(),    reg_new.ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

  /*_p08.KUPO*/ wire KUPO_new = nand2(AMET_A00p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.CABA*/ wire CABA_new = nand2(ATOL_A01p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.BOKU*/ wire BOKU_new = nand2(APOK_A02p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.BOTY*/ wire BOTY_new = nand2(AMER_A03p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.BYLA*/ wire BYLA_new = nand2(ATEM_A04p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.BADU*/ wire BADU_new = nand2(ATOV_A05p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.CEPU*/ wire CEPU_new = nand2(ATYR_A06p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.DEFY*/ wire DEFY_new = nand2(ASUR_A07p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.MYNY*/ wire MYNY_new = nand2(MANO_A08p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.MUNE*/ wire MUNE_new = nand2(MASU_A09p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.ROXU*/ wire ROXU_new = nand2(PAMY_A10p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.LEPY*/ wire LEPY_new = nand2(MALE_A11p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.LUCE*/ wire LUCE_new = nand2(MOJY_A12p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.LABE*/ wire LABE_new = nand2(MUCE_A13p_new, pins.sys.TOVA_MODE_DBG2n_new());
  /*_p08.PUHE*/ wire PUHE_new = nand2(PEGE_A14p_new, pins.sys.TOVA_MODE_DBG2n_new());

  /*_p08.KOTY*/ wire KOTY_new = nor2(AMET_A00p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.COTU*/ wire COTU_new = nor2(ATOL_A01p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.BAJO*/ wire BAJO_new = nor2(APOK_A02p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.BOLA*/ wire BOLA_new = nor2(AMER_A03p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.BEVO*/ wire BEVO_new = nor2(ATEM_A04p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.AJAV*/ wire AJAV_new = nor2(ATOV_A05p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.CYKA*/ wire CYKA_new = nor2(ATYR_A06p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.COLO*/ wire COLO_new = nor2(ASUR_A07p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.MEGO*/ wire MEGO_new = nor2(MANO_A08p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.MENY*/ wire MENY_new = nor2(MASU_A09p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.RORE*/ wire RORE_new = nor2(PAMY_A10p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.LYNY*/ wire LYNY_new = nor2(MALE_A11p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.LOSO*/ wire LOSO_new = nor2(MOJY_A12p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.LEVA*/ wire LEVA_new = nor2(MUCE_A13p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_p08.PAHY*/ wire PAHY_new = nor2(PEGE_A14p_new, pins.sys.UNOR_MODE_DBG2p_new());

  /*_PIN_01*/ pins.abus_lo.PIN_01_A00.pin_out(KUPO_new, KOTY_new);
  /*_PIN_02*/ pins.abus_lo.PIN_02_A01.pin_out(CABA_new, COTU_new);
  /*_PIN_03*/ pins.abus_lo.PIN_03_A02.pin_out(BOKU_new, BAJO_new);
  /*_PIN_04*/ pins.abus_lo.PIN_04_A03.pin_out(BOTY_new, BOLA_new);
  /*_PIN_05*/ pins.abus_lo.PIN_05_A04.pin_out(BYLA_new, BEVO_new);
  /*_PIN_06*/ pins.abus_lo.PIN_06_A05.pin_out(BADU_new, AJAV_new);
  /*_PIN_07*/ pins.abus_lo.PIN_07_A06.pin_out(CEPU_new, CYKA_new);
  /*_PIN_08*/ pins.abus_lo.PIN_08_A07.pin_out(DEFY_new, COLO_new);
  /*_PIN_09*/ pins.abus_hi.PIN_09_A08.pin_out(MYNY_new, MEGO_new);
  /*_PIN_10*/ pins.abus_hi.PIN_10_A09.pin_out(MUNE_new, MENY_new);
  /*_PIN_11*/ pins.abus_hi.PIN_11_A10.pin_out(ROXU_new, RORE_new);
  /*_PIN_12*/ pins.abus_hi.PIN_12_A11.pin_out(LEPY_new, LYNY_new);
  /*_PIN_13*/ pins.abus_hi.PIN_13_A12.pin_out(LUCE_new, LOSO_new);
  /*_PIN_14*/ pins.abus_hi.PIN_14_A13.pin_out(LABE_new, LEVA_new);
  /*_PIN_15*/ pins.abus_hi.PIN_15_A14.pin_out(PUHE_new, PAHY_new);

  // A15 is "special"
  /*_p07.TERA*/ wire TERA_BOOT_BITp_new = not1(reg_new.cpu_signals.TEPU_BOOT_BITn.qp_new());
  /*_p07.TUTU*/ wire TUTU_READ_BOOTROMp_new = and2(TERA_BOOT_BITp_new, reg_new.cpu_abus.TULO_ADDR_BOOTROMp_new());
  /*_p08.SOBY*/ wire SOBY_A15n_new = nor2 (reg_new.cpu_abus.BUS_CPU_A15p.out_new(), TUTU_READ_BOOTROMp_new);
  /*_p08.SEPY*/ wire SEPY_A15p_new = nand2(reg_new.cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new(), SOBY_A15n_new);
  /*_p08.TAZY*/ wire TAZY_A15p_new = mux2p (LUMA_DMA_CARTp_new, reg_new.reg_dma.MARU_DMA_A15n.qn_new(), SEPY_A15p_new);
  /*_p08.SUZE*/ wire SUZE_A15n_new = nand2(TAZY_A15p_new, pins.sys.RYCA_MODE_DBG2n_new());
  /*_p08.RULO*/ wire RULO_A15n_new = nor2 (TAZY_A15p_new, pins.sys.UNOR_MODE_DBG2p_new());
  /*_PIN_16*/ pins.abus_hi.PIN_16_A15.pin_out(SUZE_A15n_new, RULO_A15n_new);

  // FIXME So does this mean that if the CPU writes to the external bus during dma, that data_out
  // will actually end up in oam?

  /*_p08.LAGU*/ wire LAGU_new = and_or3(reg_new.cpu_signals.SIG_IN_CPU_RDp.out_new(), reg_new.LEVO_ADDR_VRAMn_new(), reg_new.cpu_signals.SIG_IN_CPU_WRp.out_new());
  /*_p08.LYWE*/ wire LYWE_new = not1(LAGU_new);
  /*_p08.MOTY*/ wire MOTY_CPU_EXT_RD_new = or2(MOCA_DBG_EXT_RD_new, LYWE_new);
  /*_p08.RORU*/ wire RORU_CBD_TO_EPDn_new = mux2p(pins.sys.UNOR_MODE_DBG2p_new(), reg_new.cpu_signals.REDU_CPU_RDn_new(), MOTY_CPU_EXT_RD_new);
  /*_p08.LULA*/ wire LULA_CBD_TO_EPDp_new = not1(RORU_CBD_TO_EPDn_new);

  /*_p25.RUXA*/ wire RUXA_new = nand2(reg_new.cpu_dbus.BUS_CPU_D00p.out_new(), LULA_CBD_TO_EPDp_new);
  /*_p25.RUJA*/ wire RUJA_new = nand2(reg_new.cpu_dbus.BUS_CPU_D01p.out_new(), LULA_CBD_TO_EPDp_new);
  /*_p25.RABY*/ wire RABY_new = nand2(reg_new.cpu_dbus.BUS_CPU_D02p.out_new(), LULA_CBD_TO_EPDp_new);
  /*_p25.RERA*/ wire RERA_new = nand2(reg_new.cpu_dbus.BUS_CPU_D03p.out_new(), LULA_CBD_TO_EPDp_new);
  /*_p25.RORY*/ wire RORY_new = nand2(reg_new.cpu_dbus.BUS_CPU_D04p.out_new(), LULA_CBD_TO_EPDp_new);
  /*_p25.RYVO*/ wire RYVO_new = nand2(reg_new.cpu_dbus.BUS_CPU_D05p.out_new(), LULA_CBD_TO_EPDp_new);
  /*_p25.RAFY*/ wire RAFY_new = nand2(reg_new.cpu_dbus.BUS_CPU_D06p.out_new(), LULA_CBD_TO_EPDp_new);
  /*_p25.RAVU*/ wire RAVU_new = nand2(reg_new.cpu_dbus.BUS_CPU_D07p.out_new(), LULA_CBD_TO_EPDp_new);

  /*_p08.RUNE*/ wire RUNE_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D00p.out_new(), RORU_CBD_TO_EPDn_new);
  /*_p08.RYPU*/ wire RYPU_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D01p.out_new(), RORU_CBD_TO_EPDn_new);
  /*_p08.SULY*/ wire SULY_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D02p.out_new(), RORU_CBD_TO_EPDn_new);
  /*_p08.SEZE*/ wire SEZE_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D03p.out_new(), RORU_CBD_TO_EPDn_new);
  /*_p08.RESY*/ wire RESY_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D04p.out_new(), RORU_CBD_TO_EPDn_new);
  /*_p08.TAMU*/ wire TAMU_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D05p.out_new(), RORU_CBD_TO_EPDn_new);
  /*_p08.ROGY*/ wire ROGY_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D06p.out_new(), RORU_CBD_TO_EPDn_new);
  /*_p08.RYDA*/ wire RYDA_new = nor2 (reg_new.cpu_dbus.BUS_CPU_D07p.out_new(), RORU_CBD_TO_EPDn_new);

  //----------------------------------------
  // region 0 = rom 0
  // region 1 = rom 0
  // region 2 = rom 1
  // region 3 = rom 1
  // region 4 = vram
  // region 5 = cram
  // region 6 = iram
  // region 7 = eram

  auto ext_addr = bit_pack_inv(pins.abus_lo) | (bit_pack_inv(pins.abus_hi) << 8);
  const int region = ext_addr >> 13;

  //----------------------------------------

  bool EXT_rd_en = false;
  uint8_t data_in = 0xFF;

  if (bit0(~pins.ctrl.PIN_79_RDn.qp_ext_new())) {

    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit0(reg_old.ext_mbc.MBC1_MODE.out_old());
      bool mbc1_ram_en = bit0(reg_old.ext_mbc.MBC1_RAM_EN.out_old());

      if (region == 0 || region == 1) {
        uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&reg_new.ext_mbc.MBC1_BANK5, 2) : 0;
        uint32_t mbc1_rom0_addr = ((ext_addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & cart_rom_addr_mask(cart_blob);
        if (mbc1_rom0_addr >= cart_blob.size()) debugbreak();

        EXT_rd_en = true;
        data_in = cart_blob[mbc1_rom0_addr];
      }
      else if (region == 2 || region == 3) {
        uint32_t mbc1_rom1_bank = bit_pack(&reg_new.ext_mbc.MBC1_BANK0, 7);
        if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
        uint32_t mbc1_rom1_addr = ((ext_addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & cart_rom_addr_mask(cart_blob);
        if (mbc1_rom1_addr >= cart_blob.size()) debugbreak();

        EXT_rd_en = true;
        data_in = cart_blob[mbc1_rom1_addr];
      }
      else if (region == 5 && cart_has_ram(cart_blob)) {
        uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&reg_new.ext_mbc.MBC1_BANK5, 2) : 0;
        if (mbc1_mode == 0) mbc1_ram_bank = 0;
        uint32_t mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);
        if (mbc1_ram_addr >= 32768) debugbreak();

        EXT_rd_en = mbc1_ram_en;
        data_in = mem.cart_ram[mbc1_ram_addr];
      }
    }
    else {
      if (region == 0 || region == 1) {
        EXT_rd_en = true;
        data_in = cart_blob[ext_addr & cart_rom_addr_mask(cart_blob)];
      }
      else if (region == 2 || region == 3) {
        EXT_rd_en = true;
        data_in = cart_blob[ext_addr & cart_rom_addr_mask(cart_blob)];
      }
    }

    if (region == 6 || region == 7) {
      EXT_rd_en = true;
      data_in = mem.int_ram[ext_addr & 0x1FFF];
    }
  }

  wire EXT_data_in0_new = bit(data_in, 0);
  wire EXT_data_in1_new = bit(data_in, 1);
  wire EXT_data_in2_new = bit(data_in, 2);
  wire EXT_data_in3_new = bit(data_in, 3);
  wire EXT_data_in4_new = bit(data_in, 4);
  wire EXT_data_in5_new = bit(data_in, 5);
  wire EXT_data_in6_new = bit(data_in, 6);
  wire EXT_data_in7_new = bit(data_in, 7);


  /*_PIN_17*/ pins.dbus.PIN_17_D00.pin_io(LULA_CBD_TO_EPDp_new, RUXA_new, RUNE_new, EXT_rd_en, EXT_data_in0_new);
  /*_PIN_18*/ pins.dbus.PIN_18_D01.pin_io(LULA_CBD_TO_EPDp_new, RUJA_new, RYPU_new, EXT_rd_en, EXT_data_in1_new);
  /*_PIN_19*/ pins.dbus.PIN_19_D02.pin_io(LULA_CBD_TO_EPDp_new, RABY_new, SULY_new, EXT_rd_en, EXT_data_in2_new);
  /*_PIN_20*/ pins.dbus.PIN_20_D03.pin_io(LULA_CBD_TO_EPDp_new, RERA_new, SEZE_new, EXT_rd_en, EXT_data_in3_new);
  /*_PIN_21*/ pins.dbus.PIN_21_D04.pin_io(LULA_CBD_TO_EPDp_new, RORY_new, RESY_new, EXT_rd_en, EXT_data_in4_new);
  /*_PIN_22*/ pins.dbus.PIN_22_D05.pin_io(LULA_CBD_TO_EPDp_new, RYVO_new, TAMU_new, EXT_rd_en, EXT_data_in5_new);
  /*_PIN_23*/ pins.dbus.PIN_23_D06.pin_io(LULA_CBD_TO_EPDp_new, RAFY_new, ROGY_new, EXT_rd_en, EXT_data_in6_new);
  /*_PIN_24*/ pins.dbus.PIN_24_D07.pin_io(LULA_CBD_TO_EPDp_new, RAVU_new, RYDA_new, EXT_rd_en, EXT_data_in7_new);

  //----------------------------------------

  reg_new.ext_mbc.MBC1_RAM_EN.hold();
  reg_new.ext_mbc.MBC1_MODE.hold();
  reg_new.ext_mbc.MBC1_BANK0.hold();
  reg_new.ext_mbc.MBC1_BANK1.hold();
  reg_new.ext_mbc.MBC1_BANK2.hold();
  reg_new.ext_mbc.MBC1_BANK3.hold();
  reg_new.ext_mbc.MBC1_BANK4.hold();
  reg_new.ext_mbc.MBC1_BANK5.hold();
  reg_new.ext_mbc.MBC1_BANK6.hold();

  uint8_t data_out = 0;
  data_out |= bit0(pins.dbus.PIN_17_D00.qp_ext_new()) << 0;
  data_out |= bit0(pins.dbus.PIN_18_D01.qp_ext_new()) << 1;
  data_out |= bit0(pins.dbus.PIN_19_D02.qp_ext_new()) << 2;
  data_out |= bit0(pins.dbus.PIN_20_D03.qp_ext_new()) << 3;
  data_out |= bit0(pins.dbus.PIN_21_D04.qp_ext_new()) << 4;
  data_out |= bit0(pins.dbus.PIN_22_D05.qp_ext_new()) << 5;
  data_out |= bit0(pins.dbus.PIN_23_D06.qp_ext_new()) << 6;
  data_out |= bit0(pins.dbus.PIN_24_D07.qp_ext_new()) << 7;

  if (!bit0(pins.ctrl.PIN_78_WRn.qp_ext_new())) {
    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit0(reg_new.ext_mbc.MBC1_MODE.out_new());
      bool mbc1_ram_en = bit0(reg_new.ext_mbc.MBC1_RAM_EN.out_new());

      if (region == 0 && 1) {
        reg_new.ext_mbc.MBC1_RAM_EN <<= bit0((data_out & 0x0F) == 0x0A);
      }
      else if (region == 1 && 1) {
        reg_new.ext_mbc.MBC1_BANK0 <<= bit(data_out, 0);
        reg_new.ext_mbc.MBC1_BANK1 <<= bit(data_out, 1);
        reg_new.ext_mbc.MBC1_BANK2 <<= bit(data_out, 2);
        reg_new.ext_mbc.MBC1_BANK3 <<= bit(data_out, 3);
        reg_new.ext_mbc.MBC1_BANK4 <<= bit(data_out, 4);
      }
      else if (region == 2 && 1) {
        reg_new.ext_mbc.MBC1_BANK5 <<= bit(data_out, 0);
        reg_new.ext_mbc.MBC1_BANK6 <<= bit(data_out, 1);
      }
      else if (region == 3 && 1) {
        reg_new.ext_mbc.MBC1_MODE <<= (data_out & 1);
      }
      else if (region == 5 && cart_has_ram(cart_blob) && 1 && mbc1_ram_en) {
        uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&reg_new.ext_mbc.MBC1_BANK5, 2) : 0;
        if (mbc1_mode == 0) mbc1_ram_bank = 0;
        uint32_t mbc1_ram_addr = ((ext_addr & 0x1FFF) | (mbc1_ram_bank << 13)) & cart_ram_addr_mask(cart_blob);
        if (mbc1_ram_addr >= 32768) debugbreak();

        mem.cart_ram[mbc1_ram_addr & cart_ram_addr_mask(cart_blob)] = data_out;
      }
      else if (region == 5 && cart_has_ram(cart_blob) && 0) {
        mem.cart_ram[ext_addr & cart_ram_addr_mask(cart_blob)] = data_out;
      }
      else if (region == 6 || region == 7) {
        mem.int_ram[ext_addr & 0x1FFF]  = data_out;
      }
    }
    else {
      if (region == 6 || region == 7) {
        mem.int_ram[ext_addr & 0x1FFF]  = data_out;
      }
    }

  }

  //----------------------------------------

  /*_p08.LAVO*/ wire LAVO_HOLDn_new = nand3(reg_new.cpu_signals.SIG_IN_CPU_RDp.out_new(), reg_new.TEXO_ADDR_VRAMn_new(), reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.out_new());

  /*#p08.SOMA*/ reg_new.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_17_D00.qp_int_new());
  /*_p08.RONY*/ reg_new.ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_18_D01.qp_int_new());
  /*_p08.RAXY*/ reg_new.ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_19_D02.qp_int_new());
  /*_p08.SELO*/ reg_new.ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_20_D03.qp_int_new());
  /*_p08.SODY*/ reg_new.ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_21_D04.qp_int_new());
  /*_p08.SAGO*/ reg_new.ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_22_D05.qp_int_new());
  /*_p08.RUPA*/ reg_new.ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_23_D06.qp_int_new());
  /*_p08.SAZY*/ reg_new.ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latchn(LAVO_HOLDn_new, pins.dbus.PIN_24_D07.qp_int_new());

  /*#p08.RYMA*/ triwire RYMA_EL0_TO_CD0_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
  /*_p08.RUVO*/ triwire RUVO_EL1_TO_CD1_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
  /*_p08.RYKO*/ triwire RYKO_EL2_TO_CD2_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
  /*_p08.TAVO*/ triwire TAVO_EL3_TO_CD3_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
  /*_p08.TEPE*/ triwire TEPE_EL4_TO_CD4_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
  /*_p08.SAFO*/ triwire SAFO_EL5_TO_CD5_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
  /*_p08.SEVU*/ triwire SEVU_EL6_TO_CD6_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
  /*_p08.TAJU*/ triwire TAJU_EL7_TO_CD7_new = tri6_nn(LAVO_HOLDn_new, reg_new.ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(RYMA_EL0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(RUVO_EL1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(RYKO_EL2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(TAVO_EL3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(TEPE_EL4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(SAFO_EL5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(SEVU_EL6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(TAJU_EL7_TO_CD7_new);
}

//-----------------------------------------------------------------------------

void ExtDataLatch::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

void ExtDataLatch::reset_to_bootrom() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

void ExtDataLatch::reset_to_cart() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

//-----------------------------------------------------------------------------

void ExtAddrLatch::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x0000);
}

void ExtAddrLatch::reset_to_bootrom() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x0000);
}

void ExtAddrLatch::reset_to_cart() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x004D);
}

//-----------------------------------------------------------------------------

void GateBoyMBC::reset_to_poweron() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

void GateBoyMBC::reset_to_bootrom() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

void GateBoyMBC::reset_to_cart() {
  bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00);
}

//-----------------------------------------------------------------------------
