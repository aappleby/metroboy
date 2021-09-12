#include "GateBoyLib/GateBoyExtBus.h"

#include "CoreLib/Log.h"
#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

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


//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_ext_gates(const blob& cart_blob)
{
  /*_p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor2(TEXO_ADDR_VRAMn(), UMUT_MODE_DBG1p());

  /*#p04.LEBU*/ wire LEBU_DMA_A15n  = not1(reg.dma_hi.MARU_DMA_A15n.qn_new());
  /*#p04.MUDA*/ wire MUDA_DMA_VRAMp = nor3(reg.dma_hi.PULA_DMA_A13n.qn_new(), reg.dma_hi.POKU_DMA_A14n.qn_new(), LEBU_DMA_A15n);
  /*_p04.LOGO*/ wire LOGO_DMA_VRAMn = not1(MUDA_DMA_VRAMp);
  /*_p04.MORY*/ wire MORY_DMA_CARTn = nand2(reg.dma_ctrl.MATU_DMA_RUNNINGp.qp_new(), LOGO_DMA_VRAMn);
  /*_p04.LUMA*/ wire LUMA_DMA_CARTp = not1(MORY_DMA_CARTn);

  {
    /*_p08.LAGU*/ wire LAGU = and_or3(reg.cpu_signals.SIG_IN_CPU_RDp.out_new(), LEVO_ADDR_VRAMn(), reg.cpu_signals.SIG_IN_CPU_WRp.out_new());
    /*_p08.LYWE*/ wire LYWE = not1(LAGU);
    /*_p08.MOTY*/ wire MOTY_CPU_EXT_RD = or2(MOCA_DBG_EXT_RD, LYWE);
    /*_p08.TYMU*/ wire TYMU_EXT_RDn = nor2(LUMA_DMA_CARTp, MOTY_CPU_EXT_RD);
    /*_p08.UGAC*/ wire UGAC_RD_A = nand2(TYMU_EXT_RDn, TOVA_MODE_DBG2n());
    /*_p08.URUN*/ wire URUN_RD_D = nor2 (TYMU_EXT_RDn, UNOR_MODE_DBG2p());
    /*_PIN_79*/ reg.ext_ctrl.PIN_79_RDn.pin_out(UGAC_RD_A, URUN_RD_D);
  }

  {
    /*_p08.NEVY*/ wire NEVY = or2(MEXO_CPU_WRn(), MOCA_DBG_EXT_RD);
    /*_p08.PUVA*/ wire PUVA_EXT_WRn = or2(NEVY, LUMA_DMA_CARTp);
    /*_p08.UVER*/ wire UVER_WR_A = nand2(PUVA_EXT_WRn, TOVA_MODE_DBG2n());
    /*_p08.USUF*/ wire USUF_WR_D = nor2 (PUVA_EXT_WRn, UNOR_MODE_DBG2p());
    /*_PIN_78*/ reg.ext_ctrl.PIN_78_WRn.pin_out(UVER_WR_A, USUF_WR_D);
  }

  {
    /*_p08.TOZA*/ wire TOZA_CS_A = and3(reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new(), reg.cpu_abus_new.TYNU_A000_FFFFp(), reg.cpu_abus_new.TUNA_0000_FDFF());
    /*_p08.TYHO*/ wire TYHO_CS_A = mux2p(LUMA_DMA_CARTp, reg.dma_hi.MARU_DMA_A15n.qn_new(), TOZA_CS_A);
    /*_PIN_80*/ reg.ext_ctrl.PIN_80_CSn.pin_out(TYHO_CS_A, TYHO_CS_A);
  }

  /*_p08.LOXO*/ wire LOXO_HOLDn = and_or3(MULE_MODE_DBG1n(), TEXO_ADDR_VRAMn(), UMUT_MODE_DBG1p());
  /*_p08.LASY*/ wire LASY_HOLDp = not1(LOXO_HOLDn);
  /*_p08.MATE*/ wire MATE_HOLDn = not1(LASY_HOLDp);
  /*_p08.ALOR*/ reg.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A00p.out_new());
  /*_p08.APUR*/ reg.ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A01p.out_new());
  /*_p08.ALYR*/ reg.ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A02p.out_new());
  /*_p08.ARET*/ reg.ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A03p.out_new());
  /*_p08.AVYS*/ reg.ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A04p.out_new());
  /*_p08.ATEV*/ reg.ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A05p.out_new());
  /*_p08.AROS*/ reg.ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A06p.out_new());
  /*_p08.ARYM*/ reg.ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A07p.out_new());
  /*_p08.LUNO*/ reg.ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A08p.out_new());
  /*_p08.LYSA*/ reg.ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A09p.out_new());
  /*_p08.PATE*/ reg.ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A10p.out_new());
  /*_p08.LUMY*/ reg.ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A11p.out_new());
  /*_p08.LOBU*/ reg.ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A12p.out_new());
  /*_p08.LONU*/ reg.ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A13p.out_new());
  /*_p08.NYRE*/ reg.ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.tp_latchn(MATE_HOLDn, reg.cpu_abus_new.BUS_CPU_A14p.out_new());


  /*_p08.AMET*/ wire AMET_A00p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.NAKY_DMA_A00p.qp_new(), reg.ext_addr_latch.ALOR_EXT_ADDR_LATCH_00p.qp_new());
  /*_p08.ATOL*/ wire ATOL_A01p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.PYRO_DMA_A01p.qp_new(), reg.ext_addr_latch.APUR_EXT_ADDR_LATCH_01p.qp_new());
  /*_p08.APOK*/ wire APOK_A02p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.NEFY_DMA_A02p.qp_new(), reg.ext_addr_latch.ALYR_EXT_ADDR_LATCH_02p.qp_new());
  /*_p08.AMER*/ wire AMER_A03p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.MUTY_DMA_A03p.qp_new(), reg.ext_addr_latch.ARET_EXT_ADDR_LATCH_03p.qp_new());
  /*_p08.ATEM*/ wire ATEM_A04p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.NYKO_DMA_A04p.qp_new(), reg.ext_addr_latch.AVYS_EXT_ADDR_LATCH_04p.qp_new());
  /*_p08.ATOV*/ wire ATOV_A05p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.PYLO_DMA_A05p.qp_new(), reg.ext_addr_latch.ATEV_EXT_ADDR_LATCH_05p.qp_new());
  /*_p08.ATYR*/ wire ATYR_A06p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.NUTO_DMA_A06p.qp_new(), reg.ext_addr_latch.AROS_EXT_ADDR_LATCH_06p.qp_new());
  /*#p08.ASUR*/ wire ASUR_A07p = mux2p(LUMA_DMA_CARTp, reg.dma_lo.MUGU_DMA_A07p.qp_new(), reg.ext_addr_latch.ARYM_EXT_ADDR_LATCH_07p.qp_new());
  /*#p08.MANO*/ wire MANO_A08p = mux2p(LUMA_DMA_CARTp, reg.dma_hi.NAFA_DMA_A08n.qn_new(), reg.ext_addr_latch.LUNO_EXT_ADDR_LATCH_08p.qp_new());
  /*_p08.MASU*/ wire MASU_A09p = mux2p(LUMA_DMA_CARTp, reg.dma_hi.PYNE_DMA_A09n.qn_new(), reg.ext_addr_latch.LYSA_EXT_ADDR_LATCH_09p.qp_new());
  /*_p08.PAMY*/ wire PAMY_A10p = mux2p(LUMA_DMA_CARTp, reg.dma_hi.PARA_DMA_A10n.qn_new(), reg.ext_addr_latch.PATE_EXT_ADDR_LATCH_10p.qp_new());
  /*_p08.MALE*/ wire MALE_A11p = mux2p(LUMA_DMA_CARTp, reg.dma_hi.NYDO_DMA_A11n.qn_new(), reg.ext_addr_latch.LUMY_EXT_ADDR_LATCH_11p.qp_new());
  /*_p08.MOJY*/ wire MOJY_A12p = mux2p(LUMA_DMA_CARTp, reg.dma_hi.NYGY_DMA_A12n.qn_new(), reg.ext_addr_latch.LOBU_EXT_ADDR_LATCH_12p.qp_new());
  /*_p08.MUCE*/ wire MUCE_A13p = mux2p(LUMA_DMA_CARTp, reg.dma_hi.PULA_DMA_A13n.qn_new(), reg.ext_addr_latch.LONU_EXT_ADDR_LATCH_13p.qp_new());
  /*_p08.PEGE*/ wire PEGE_A14p = mux2p(LUMA_DMA_CARTp, reg.dma_hi.POKU_DMA_A14n.qn_new(), reg.ext_addr_latch.NYRE_EXT_ADDR_LATCH_14p.qp_new());

  /*_p08.KUPO*/ wire KUPO = nand2(AMET_A00p, TOVA_MODE_DBG2n());
  /*_p08.CABA*/ wire CABA = nand2(ATOL_A01p, TOVA_MODE_DBG2n());
  /*_p08.BOKU*/ wire BOKU = nand2(APOK_A02p, TOVA_MODE_DBG2n());
  /*_p08.BOTY*/ wire BOTY = nand2(AMER_A03p, TOVA_MODE_DBG2n());
  /*_p08.BYLA*/ wire BYLA = nand2(ATEM_A04p, TOVA_MODE_DBG2n());
  /*_p08.BADU*/ wire BADU = nand2(ATOV_A05p, TOVA_MODE_DBG2n());
  /*_p08.CEPU*/ wire CEPU = nand2(ATYR_A06p, TOVA_MODE_DBG2n());
  /*_p08.DEFY*/ wire DEFY = nand2(ASUR_A07p, TOVA_MODE_DBG2n());
  /*_p08.MYNY*/ wire MYNY = nand2(MANO_A08p, TOVA_MODE_DBG2n());
  /*_p08.MUNE*/ wire MUNE = nand2(MASU_A09p, TOVA_MODE_DBG2n());
  /*_p08.ROXU*/ wire ROXU = nand2(PAMY_A10p, TOVA_MODE_DBG2n());
  /*_p08.LEPY*/ wire LEPY = nand2(MALE_A11p, TOVA_MODE_DBG2n());
  /*_p08.LUCE*/ wire LUCE = nand2(MOJY_A12p, TOVA_MODE_DBG2n());
  /*_p08.LABE*/ wire LABE = nand2(MUCE_A13p, TOVA_MODE_DBG2n());
  /*_p08.PUHE*/ wire PUHE = nand2(PEGE_A14p, TOVA_MODE_DBG2n());

  /*_p08.KOTY*/ wire KOTY = nor2(AMET_A00p, UNOR_MODE_DBG2p());
  /*_p08.COTU*/ wire COTU = nor2(ATOL_A01p, UNOR_MODE_DBG2p());
  /*_p08.BAJO*/ wire BAJO = nor2(APOK_A02p, UNOR_MODE_DBG2p());
  /*_p08.BOLA*/ wire BOLA = nor2(AMER_A03p, UNOR_MODE_DBG2p());
  /*_p08.BEVO*/ wire BEVO = nor2(ATEM_A04p, UNOR_MODE_DBG2p());
  /*_p08.AJAV*/ wire AJAV = nor2(ATOV_A05p, UNOR_MODE_DBG2p());
  /*_p08.CYKA*/ wire CYKA = nor2(ATYR_A06p, UNOR_MODE_DBG2p());
  /*_p08.COLO*/ wire COLO = nor2(ASUR_A07p, UNOR_MODE_DBG2p());
  /*_p08.MEGO*/ wire MEGO = nor2(MANO_A08p, UNOR_MODE_DBG2p());
  /*_p08.MENY*/ wire MENY = nor2(MASU_A09p, UNOR_MODE_DBG2p());
  /*_p08.RORE*/ wire RORE = nor2(PAMY_A10p, UNOR_MODE_DBG2p());
  /*_p08.LYNY*/ wire LYNY = nor2(MALE_A11p, UNOR_MODE_DBG2p());
  /*_p08.LOSO*/ wire LOSO = nor2(MOJY_A12p, UNOR_MODE_DBG2p());
  /*_p08.LEVA*/ wire LEVA = nor2(MUCE_A13p, UNOR_MODE_DBG2p());
  /*_p08.PAHY*/ wire PAHY = nor2(PEGE_A14p, UNOR_MODE_DBG2p());

  /*_PIN_01*/ reg.ext_abus.PIN_01_A00.pin_out(KUPO, KOTY);
  /*_PIN_02*/ reg.ext_abus.PIN_02_A01.pin_out(CABA, COTU);
  /*_PIN_03*/ reg.ext_abus.PIN_03_A02.pin_out(BOKU, BAJO);
  /*_PIN_04*/ reg.ext_abus.PIN_04_A03.pin_out(BOTY, BOLA);
  /*_PIN_05*/ reg.ext_abus.PIN_05_A04.pin_out(BYLA, BEVO);
  /*_PIN_06*/ reg.ext_abus.PIN_06_A05.pin_out(BADU, AJAV);
  /*_PIN_07*/ reg.ext_abus.PIN_07_A06.pin_out(CEPU, CYKA);
  /*_PIN_08*/ reg.ext_abus.PIN_08_A07.pin_out(DEFY, COLO);
  /*_PIN_09*/ reg.ext_abus.PIN_09_A08.pin_out(MYNY, MEGO);
  /*_PIN_10*/ reg.ext_abus.PIN_10_A09.pin_out(MUNE, MENY);
  /*_PIN_11*/ reg.ext_abus.PIN_11_A10.pin_out(ROXU, RORE);
  /*_PIN_12*/ reg.ext_abus.PIN_12_A11.pin_out(LEPY, LYNY);
  /*_PIN_13*/ reg.ext_abus.PIN_13_A12.pin_out(LUCE, LOSO);
  /*_PIN_14*/ reg.ext_abus.PIN_14_A13.pin_out(LABE, LEVA);
  /*_PIN_15*/ reg.ext_abus.PIN_15_A14.pin_out(PUHE, PAHY);

  // A15 is "special"
  /*_p07.TERA*/ wire TERA_BOOT_BITp = not1(reg.cpu_signals.TEPU_BOOT_BITn.qp_new());
  /*_p07.TUTU*/ wire TUTU_READ_BOOTROMp = and2(TERA_BOOT_BITp, reg.cpu_abus_new.TULO_ADDR_BOOTROMp());
  /*_p08.SOBY*/ wire SOBY_A15n = nor2 (reg.cpu_abus_new.BUS_CPU_A15p.out_new(), TUTU_READ_BOOTROMp);
  /*_p08.SEPY*/ wire SEPY_A15p = nand2(reg.cpu_signals.ABUZ_EXT_RAM_CS_CLK.out_new(), SOBY_A15n);
  /*_p08.TAZY*/ wire TAZY_A15p = mux2p (LUMA_DMA_CARTp, reg.dma_hi.MARU_DMA_A15n.qn_new(), SEPY_A15p);
  /*_p08.SUZE*/ wire SUZE_A15n = nand2(TAZY_A15p, RYCA_MODE_DBG2n());
  /*_p08.RULO*/ wire RULO_A15n = nor2 (TAZY_A15p, UNOR_MODE_DBG2p());
  /*_PIN_16*/ reg.ext_abus.PIN_16_A15.pin_out(SUZE_A15n, RULO_A15n);

  // FIXME So does this mean that if the CPU writes to the external bus during dma, that data_out
  // will actually end up in oam?

  /*_p08.LAGU*/ wire LAGU = and_or3(reg.cpu_signals.SIG_IN_CPU_RDp.out_new(), LEVO_ADDR_VRAMn(), reg.cpu_signals.SIG_IN_CPU_WRp.out_new());
  /*_p08.LYWE*/ wire LYWE = not1(LAGU);
  /*_p08.MOTY*/ wire MOTY_CPU_EXT_RD = or2(MOCA_DBG_EXT_RD, LYWE);
  /*_p08.RORU*/ wire RORU_CBD_TO_EPDn = mux2p(UNOR_MODE_DBG2p(), REDU_CPU_RDn(), MOTY_CPU_EXT_RD);
  /*_p08.LULA*/ wire LULA_CBD_TO_EPDp = not1(RORU_CBD_TO_EPDn);

  /*_p25.RUXA*/ wire RUXA = nand2(reg.cpu_dbus_new.BUS_CPU_D00p.out_new(), LULA_CBD_TO_EPDp);
  /*_p25.RUJA*/ wire RUJA = nand2(reg.cpu_dbus_new.BUS_CPU_D01p.out_new(), LULA_CBD_TO_EPDp);
  /*_p25.RABY*/ wire RABY = nand2(reg.cpu_dbus_new.BUS_CPU_D02p.out_new(), LULA_CBD_TO_EPDp);
  /*_p25.RERA*/ wire RERA = nand2(reg.cpu_dbus_new.BUS_CPU_D03p.out_new(), LULA_CBD_TO_EPDp);
  /*_p25.RORY*/ wire RORY = nand2(reg.cpu_dbus_new.BUS_CPU_D04p.out_new(), LULA_CBD_TO_EPDp);
  /*_p25.RYVO*/ wire RYVO = nand2(reg.cpu_dbus_new.BUS_CPU_D05p.out_new(), LULA_CBD_TO_EPDp);
  /*_p25.RAFY*/ wire RAFY = nand2(reg.cpu_dbus_new.BUS_CPU_D06p.out_new(), LULA_CBD_TO_EPDp);
  /*_p25.RAVU*/ wire RAVU = nand2(reg.cpu_dbus_new.BUS_CPU_D07p.out_new(), LULA_CBD_TO_EPDp);

  /*_p08.RUNE*/ wire RUNE = nor2 (reg.cpu_dbus_new.BUS_CPU_D00p.out_new(), RORU_CBD_TO_EPDn);
  /*_p08.RYPU*/ wire RYPU = nor2 (reg.cpu_dbus_new.BUS_CPU_D01p.out_new(), RORU_CBD_TO_EPDn);
  /*_p08.SULY*/ wire SULY = nor2 (reg.cpu_dbus_new.BUS_CPU_D02p.out_new(), RORU_CBD_TO_EPDn);
  /*_p08.SEZE*/ wire SEZE = nor2 (reg.cpu_dbus_new.BUS_CPU_D03p.out_new(), RORU_CBD_TO_EPDn);
  /*_p08.RESY*/ wire RESY = nor2 (reg.cpu_dbus_new.BUS_CPU_D04p.out_new(), RORU_CBD_TO_EPDn);
  /*_p08.TAMU*/ wire TAMU = nor2 (reg.cpu_dbus_new.BUS_CPU_D05p.out_new(), RORU_CBD_TO_EPDn);
  /*_p08.ROGY*/ wire ROGY = nor2 (reg.cpu_dbus_new.BUS_CPU_D06p.out_new(), RORU_CBD_TO_EPDn);
  /*_p08.RYDA*/ wire RYDA = nor2 (reg.cpu_dbus_new.BUS_CPU_D07p.out_new(), RORU_CBD_TO_EPDn);

  //----------------------------------------
  // region 0 = rom 0
  // region 1 = rom 0
  // region 2 = rom 1
  // region 3 = rom 1
  // region 4 = vram
  // region 5 = cram
  // region 6 = iram
  // region 7 = eram

  auto ext_addr = bit_pack_inv(reg.ext_abus);
  const int region = ext_addr >> 13;

  //----------------------------------------

  bool EXT_rd_en = false;
  uint8_t data_in = 0xFF;

  if (bit(~reg.ext_ctrl.PIN_79_RDn.qp_ext_new())) {

    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit(reg.ext_mbc.MBC1_MODE.out_old());
      bool mbc1_ram_en = bit(reg.ext_mbc.MBC1_RAM_EN.out_old());

      if (region == 0 || region == 1) {
        uint32_t mbc1_rom0_bank = mbc1_mode ? bit_pack(&reg.ext_mbc.MBC1_BANK5, 2) : 0;
        uint32_t mbc1_rom0_addr = ((ext_addr & 0x3FFF) | (mbc1_rom0_bank << 19)) & cart_rom_addr_mask(cart_blob);
        if (mbc1_rom0_addr >= cart_blob.size()) debugbreak();

        EXT_rd_en = true;
        data_in = cart_blob[mbc1_rom0_addr];
      }
      else if (region == 2 || region == 3) {
        uint32_t mbc1_rom1_bank = bit_pack(&reg.ext_mbc.MBC1_BANK0, 7);
        if ((mbc1_rom1_bank & 0x1F) == 0) mbc1_rom1_bank |= 1;
        uint32_t mbc1_rom1_addr = ((ext_addr & 0x3FFF) | (mbc1_rom1_bank << 14)) & cart_rom_addr_mask(cart_blob);
        if (mbc1_rom1_addr >= cart_blob.size()) debugbreak();

        EXT_rd_en = true;
        data_in = cart_blob[mbc1_rom1_addr];
      }
      else if (region == 5 && cart_has_ram(cart_blob)) {
        uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&reg.ext_mbc.MBC1_BANK5, 2) : 0;
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

  wire EXT_data_in0 = get_bit(data_in, 0);
  wire EXT_data_in1 = get_bit(data_in, 1);
  wire EXT_data_in2 = get_bit(data_in, 2);
  wire EXT_data_in3 = get_bit(data_in, 3);
  wire EXT_data_in4 = get_bit(data_in, 4);
  wire EXT_data_in5 = get_bit(data_in, 5);
  wire EXT_data_in6 = get_bit(data_in, 6);
  wire EXT_data_in7 = get_bit(data_in, 7);


  /*_PIN_17*/ reg.ext_dbus.PIN_17_D00.pin_io(LULA_CBD_TO_EPDp, RUXA, RUNE, EXT_rd_en, EXT_data_in0);
  /*_PIN_18*/ reg.ext_dbus.PIN_18_D01.pin_io(LULA_CBD_TO_EPDp, RUJA, RYPU, EXT_rd_en, EXT_data_in1);
  /*_PIN_19*/ reg.ext_dbus.PIN_19_D02.pin_io(LULA_CBD_TO_EPDp, RABY, SULY, EXT_rd_en, EXT_data_in2);
  /*_PIN_20*/ reg.ext_dbus.PIN_20_D03.pin_io(LULA_CBD_TO_EPDp, RERA, SEZE, EXT_rd_en, EXT_data_in3);
  /*_PIN_21*/ reg.ext_dbus.PIN_21_D04.pin_io(LULA_CBD_TO_EPDp, RORY, RESY, EXT_rd_en, EXT_data_in4);
  /*_PIN_22*/ reg.ext_dbus.PIN_22_D05.pin_io(LULA_CBD_TO_EPDp, RYVO, TAMU, EXT_rd_en, EXT_data_in5);
  /*_PIN_23*/ reg.ext_dbus.PIN_23_D06.pin_io(LULA_CBD_TO_EPDp, RAFY, ROGY, EXT_rd_en, EXT_data_in6);
  /*_PIN_24*/ reg.ext_dbus.PIN_24_D07.pin_io(LULA_CBD_TO_EPDp, RAVU, RYDA, EXT_rd_en, EXT_data_in7);

  //----------------------------------------

  reg.ext_mbc.MBC1_RAM_EN.hold();
  reg.ext_mbc.MBC1_MODE.hold();
  reg.ext_mbc.MBC1_BANK0.hold();
  reg.ext_mbc.MBC1_BANK1.hold();
  reg.ext_mbc.MBC1_BANK2.hold();
  reg.ext_mbc.MBC1_BANK3.hold();
  reg.ext_mbc.MBC1_BANK4.hold();
  reg.ext_mbc.MBC1_BANK5.hold();
  reg.ext_mbc.MBC1_BANK6.hold();

  uint8_t data_out = 0;
  data_out |= bit(reg.ext_dbus.PIN_17_D00.qp_ext_new()) << 0;
  data_out |= bit(reg.ext_dbus.PIN_18_D01.qp_ext_new()) << 1;
  data_out |= bit(reg.ext_dbus.PIN_19_D02.qp_ext_new()) << 2;
  data_out |= bit(reg.ext_dbus.PIN_20_D03.qp_ext_new()) << 3;
  data_out |= bit(reg.ext_dbus.PIN_21_D04.qp_ext_new()) << 4;
  data_out |= bit(reg.ext_dbus.PIN_22_D05.qp_ext_new()) << 5;
  data_out |= bit(reg.ext_dbus.PIN_23_D06.qp_ext_new()) << 6;
  data_out |= bit(reg.ext_dbus.PIN_24_D07.qp_ext_new()) << 7;

  if (!bit(reg.ext_ctrl.PIN_78_WRn.qp_ext_new())) {
    if (cart_has_mbc1(cart_blob)) {
      bool mbc1_mode   = bit(reg.ext_mbc.MBC1_MODE.out_new());
      bool mbc1_ram_en = bit(reg.ext_mbc.MBC1_RAM_EN.out_new());

      if (region == 0 && 1) {
        reg.ext_mbc.MBC1_RAM_EN = bit((data_out & 0x0F) == 0x0A);
      }
      else if (region == 1 && 1) {
        reg.ext_mbc.MBC1_BANK0 = get_bit(data_out, 0);
        reg.ext_mbc.MBC1_BANK1 = get_bit(data_out, 1);
        reg.ext_mbc.MBC1_BANK2 = get_bit(data_out, 2);
        reg.ext_mbc.MBC1_BANK3 = get_bit(data_out, 3);
        reg.ext_mbc.MBC1_BANK4 = get_bit(data_out, 4);
      }
      else if (region == 2 && 1) {
        reg.ext_mbc.MBC1_BANK5 = get_bit(data_out, 0);
        reg.ext_mbc.MBC1_BANK6 = get_bit(data_out, 1);
      }
      else if (region == 3 && 1) {
        reg.ext_mbc.MBC1_MODE = (data_out & 1);
      }
      else if (region == 5 && cart_has_ram(cart_blob) && 1 && mbc1_ram_en) {
        uint32_t mbc1_ram_bank = mbc1_mode ? bit_pack(&reg.ext_mbc.MBC1_BANK5, 2) : 0;
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

  /*_p08.LAVO*/ wire LAVO_HOLDn = nand3(reg.cpu_signals.SIG_IN_CPU_RDp.out_new(), TEXO_ADDR_VRAMn(), reg.cpu_signals.SIG_IN_CPU_LATCH_EXT.out_new());

  //probe_wire(1, "LAVO",  LAVO_HOLDn);
  //probe_wire(2, "RD",    cpu_signals.SIG_IN_CPU_RDp.qp_new());
  //probe_wire(3, "TEXO",  TEXO_ADDR_VRAMn());
  //probe_wire(4, "LATCH", cpu_signals.SIG_IN_CPU_LATCH_EXT.qp_new());

  //probe_char( 8, "PIN_17 ext", ext_pins.PIN_17_D00.cp_ext());
  //probe_char( 9, "PIN_18 ext", ext_pins.PIN_18_D01.cp_ext());
  //probe_char(10, "PIN_19 ext", ext_pins.PIN_19_D02.cp_ext());
  //probe_char(11, "PIN_20 ext", ext_pins.PIN_20_D03.cp_ext());
  //probe_char(12, "PIN_21 ext", ext_pins.PIN_21_D04.cp_ext());
  //probe_char(13, "PIN_22 ext", ext_pins.PIN_22_D05.cp_ext());
  //probe_char(14, "PIN_23 ext", ext_pins.PIN_23_D06.cp_ext());
  //probe_char(15, "PIN_24 ext", ext_pins.PIN_24_D07.cp_ext());

  /*#p08.SOMA*/ reg.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_17_D00.qp_int_new());
  /*_p08.RONY*/ reg.ext_data_latch.RONY_EXT_DATA_LATCH_D1n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_18_D01.qp_int_new());
  /*_p08.RAXY*/ reg.ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_19_D02.qp_int_new());
  /*_p08.SELO*/ reg.ext_data_latch.SELO_EXT_DATA_LATCH_D3n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_20_D03.qp_int_new());
  /*_p08.SODY*/ reg.ext_data_latch.SODY_EXT_DATA_LATCH_D4n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_21_D04.qp_int_new());
  /*_p08.SAGO*/ reg.ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_22_D05.qp_int_new());
  /*_p08.RUPA*/ reg.ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_23_D06.qp_int_new());
  /*_p08.SAZY*/ reg.ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.tp_latchn(LAVO_HOLDn, reg.ext_dbus.PIN_24_D07.qp_int_new());

  //probe_wire(24, "SOMA", ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
  //probe_wire(25, "RONY", ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
  //probe_wire(26, "RAXY", ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
  //probe_wire(27, "SELO", ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
  //probe_wire(28, "SODY", ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
  //probe_wire(29, "SAGO", ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
  //probe_wire(30, "RUPA", ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
  //probe_wire(31, "SAZY", ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());

  /*#p08.RYMA*/ triwire RYMA_EL0_TO_CD0 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.SOMA_EXT_DATA_LATCH_D0n.qp_new());
  /*_p08.RUVO*/ triwire RUVO_EL1_TO_CD1 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.RONY_EXT_DATA_LATCH_D1n.qp_new());
  /*_p08.RYKO*/ triwire RYKO_EL2_TO_CD2 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.RAXY_EXT_DATA_LATCH_D2n.qp_new());
  /*_p08.TAVO*/ triwire TAVO_EL3_TO_CD3 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.SELO_EXT_DATA_LATCH_D3n.qp_new());
  /*_p08.TEPE*/ triwire TEPE_EL4_TO_CD4 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.SODY_EXT_DATA_LATCH_D4n.qp_new());
  /*_p08.SAFO*/ triwire SAFO_EL5_TO_CD5 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.SAGO_EXT_DATA_LATCH_D5n.qp_new());
  /*_p08.SEVU*/ triwire SEVU_EL6_TO_CD6 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.RUPA_EXT_DATA_LATCH_D6n.qp_new());
  /*_p08.TAJU*/ triwire TAJU_EL7_TO_CD7 = tri6_nn(LAVO_HOLDn, reg.ext_data_latch.SAZY_EXT_DATA_LATCH_D7n.qp_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus_new.BUS_CPU_D00p.tri_bus(RYMA_EL0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus_new.BUS_CPU_D01p.tri_bus(RUVO_EL1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus_new.BUS_CPU_D02p.tri_bus(RYKO_EL2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus_new.BUS_CPU_D03p.tri_bus(TAVO_EL3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus_new.BUS_CPU_D04p.tri_bus(TEPE_EL4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus_new.BUS_CPU_D05p.tri_bus(SAFO_EL5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus_new.BUS_CPU_D06p.tri_bus(SEVU_EL6_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus_new.BUS_CPU_D07p.tri_bus(TAJU_EL7_TO_CD7);
}
