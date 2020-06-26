#include "Sch_OAM.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

// ACYL04 = and(BOGE01, BESU04)

void OamRegisters::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_pins);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  auto& bus_reg = gb.bus_reg;
  auto& cpu_pins = gb.cpu_pins;
  auto& dma_reg = gb.dma_reg;
  auto& ext_pins = gb.ext_pins;
  auto& sst_reg = gb.sst_reg;
  auto& ppu_reg = gb.ppu_reg;
  auto& dbg_reg = gb.dbg_reg;
  auto& oam_pins = gb.oam_pins;
  auto& oam_reg = gb.oam_reg;

  //----------------------------------------

  // internal address bus to external address bus
  {
    // MORY00 << MATU17
    // MORY01
    // MORY02 
    // -> MORY reads DMA_RUNNING.q

    // arm on vcc side
    // LOXO01 << MULE02
    // LOXO02 << TEXO04
    // LOXO03 nc
    // LOXO04 << UMUT04
    // LOXO05 >> LASY01

    // Address bus to address pins mux
    /*p08.MULE*/ wire _MULE_MODE_DBG1 = not(dbg_sig.UMUT_MODE_DBG1);
    /*p08.LOXO*/ wire _LOXO = or(and(_MULE_MODE_DBG1, adr_sig.TEXO_8000_9FFFn), dbg_sig.UMUT_MODE_DBG1);
    /*p08.LASY*/ wire _LASY = not(_LOXO);
    /*p08.MATE*/ wire _MATE_LATCH_INT_ADDR = not(_LASY);

#if 0

    // or(and)
    if (dbg_sig.UMUT_MODE_DBG1) {
      /*p08.MATE*/ wire _MATE_LATCH_INT_ADDR = 1;
    }
    else {
      /*p08.MATE*/ wire _MATE_LATCH_INT_ADDR = adr_sig.TEXO_8000_9FFFn;
    }

    // and(or)
    if (dbg_sig.UMUT_MODE_DBG1) {
      /*p08.MATE*/ wire _MATE_LATCH_INT_ADDR = adr_sig.TEXO_8000_9FFFn;
    }
    else {
      /*p08.MATE*/ wire _MATE_LATCH_INT_ADDR = 0;
    }

#endif

    // Pass gate?
    /*p08.ALOR*/ bus_reg.CPU_ADDR_LATCH_00.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A00);
    /*p08.APUR*/ bus_reg.CPU_ADDR_LATCH_01.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A01);
    /*p08.ALYR*/ bus_reg.CPU_ADDR_LATCH_02.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A02);
    /*p08.ARET*/ bus_reg.CPU_ADDR_LATCH_03.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A03);
    /*p08.AVYS*/ bus_reg.CPU_ADDR_LATCH_04.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A04);
    /*p08.ATEV*/ bus_reg.CPU_ADDR_LATCH_05.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A05);
    /*p08.AROS*/ bus_reg.CPU_ADDR_LATCH_06.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A06);
    /*p08.ARYM*/ bus_reg.CPU_ADDR_LATCH_07.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A07);
    /*p08.LUNO*/ bus_reg.CPU_ADDR_LATCH_08.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A08);
    /*p08.LYSA*/ bus_reg.CPU_ADDR_LATCH_09.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A09);
    /*p08.PATE*/ bus_reg.CPU_ADDR_LATCH_10.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A10);
    /*p08.LUMY*/ bus_reg.CPU_ADDR_LATCH_11.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A11);
    /*p08.LOBU*/ bus_reg.CPU_ADDR_LATCH_12.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A12);
    /*p08.LONU*/ bus_reg.CPU_ADDR_LATCH_13.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A13);
    /*p08.NYRE*/ bus_reg.CPU_ADDR_LATCH_14.tp_latch(_MATE_LATCH_INT_ADDR, cpu_pins.A14);

    /*p08.AMET*/ wire EXT_ADDR_00 = mux2_p(dma_reg.DMA_A00.q(), bus_reg.CPU_ADDR_LATCH_00, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ wire EXT_ADDR_01 = mux2_p(dma_reg.DMA_A01.q(), bus_reg.CPU_ADDR_LATCH_01, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ wire EXT_ADDR_02 = mux2_p(dma_reg.DMA_A02.q(), bus_reg.CPU_ADDR_LATCH_02, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ wire EXT_ADDR_03 = mux2_p(dma_reg.DMA_A03.q(), bus_reg.CPU_ADDR_LATCH_03, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ wire EXT_ADDR_04 = mux2_p(dma_reg.DMA_A04.q(), bus_reg.CPU_ADDR_LATCH_04, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ wire EXT_ADDR_05 = mux2_p(dma_reg.DMA_A05.q(), bus_reg.CPU_ADDR_LATCH_05, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ wire EXT_ADDR_06 = mux2_p(dma_reg.DMA_A06.q(), bus_reg.CPU_ADDR_LATCH_06, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ wire EXT_ADDR_07 = mux2_p(dma_reg.DMA_A07.q(), bus_reg.CPU_ADDR_LATCH_07, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ wire EXT_ADDR_08 = mux2_p(dma_reg.DMA_A08.q(), bus_reg.CPU_ADDR_LATCH_08, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ wire EXT_ADDR_09 = mux2_p(dma_reg.DMA_A09.q(), bus_reg.CPU_ADDR_LATCH_09, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ wire EXT_ADDR_10 = mux2_p(dma_reg.DMA_A10.q(), bus_reg.CPU_ADDR_LATCH_10, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ wire EXT_ADDR_11 = mux2_p(dma_reg.DMA_A11.q(), bus_reg.CPU_ADDR_LATCH_11, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ wire EXT_ADDR_12 = mux2_p(dma_reg.DMA_A12.q(), bus_reg.CPU_ADDR_LATCH_12, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ wire EXT_ADDR_13 = mux2_p(dma_reg.DMA_A13.q(), bus_reg.CPU_ADDR_LATCH_13, dma_sig.LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ wire EXT_ADDR_14 = mux2_p(dma_reg.DMA_A14.q(), bus_reg.CPU_ADDR_LATCH_14, dma_sig.LUMA_DMA_READ_CARTp);

    /*p08.KUPO*/ ext_pins.A00_A.set(nand(EXT_ADDR_00, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CABA*/ ext_pins.A01_A.set(nand(EXT_ADDR_01, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOKU*/ ext_pins.A02_A.set(nand(EXT_ADDR_02, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BOTY*/ ext_pins.A03_A.set(nand(EXT_ADDR_03, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BYLA*/ ext_pins.A04_A.set(nand(EXT_ADDR_04, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.BADU*/ ext_pins.A05_A.set(nand(EXT_ADDR_05, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.CEPU*/ ext_pins.A06_A.set(nand(EXT_ADDR_06, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.DEFY*/ ext_pins.A07_A.set(nand(EXT_ADDR_07, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MYNY*/ ext_pins.A08_A.set(nand(EXT_ADDR_08, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.MUNE*/ ext_pins.A09_A.set(nand(EXT_ADDR_09, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.ROXU*/ ext_pins.A10_A.set(nand(EXT_ADDR_10, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LEPY*/ ext_pins.A11_A.set(nand(EXT_ADDR_11, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LUCE*/ ext_pins.A12_A.set(nand(EXT_ADDR_12, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.LABE*/ ext_pins.A13_A.set(nand(EXT_ADDR_13, dbg_sig.TOVA_MODE_DBG2n));
    /*p08.PUHE*/ ext_pins.A14_A.set(nand(EXT_ADDR_14, dbg_sig.TOVA_MODE_DBG2n));

    /*p08.KOTY*/ ext_pins.A00_D.set(nor(EXT_ADDR_00, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COTU*/ ext_pins.A01_D.set(nor(EXT_ADDR_01, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BAJO*/ ext_pins.A02_D.set(nor(EXT_ADDR_02, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BOLA*/ ext_pins.A03_D.set(nor(EXT_ADDR_03, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.BEVO*/ ext_pins.A04_D.set(nor(EXT_ADDR_04, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.AJAV*/ ext_pins.A05_D.set(nor(EXT_ADDR_05, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.CYKA*/ ext_pins.A06_D.set(nor(EXT_ADDR_06, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.COLO*/ ext_pins.A07_D.set(nor(EXT_ADDR_07, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MEGO*/ ext_pins.A08_D.set(nor(EXT_ADDR_08, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.MENY*/ ext_pins.A09_D.set(nor(EXT_ADDR_09, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.RORE*/ ext_pins.A10_D.set(nor(EXT_ADDR_10, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LYNY*/ ext_pins.A11_D.set(nor(EXT_ADDR_11, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LOSO*/ ext_pins.A12_D.set(nor(EXT_ADDR_12, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.LEVA*/ ext_pins.A13_D.set(nor(EXT_ADDR_13, dbg_sig.UNOR_MODE_DBG2p));
    /*p08.PAHY*/ ext_pins.A14_D.set(nor(EXT_ADDR_14, dbg_sig.UNOR_MODE_DBG2p));
  }

  // EXT -> CPU
  // external data bus to internal data bus
  {
    /*p08.LAVO*/ wire _LAVO_LATCH_EXT_DATA = nand(cpu_pins.CPU_RAW_RD, adr_sig.TEXO_8000_9FFFn, cpu_pins.FROM_CPU5);

    // SOMA01 << LAVO04
    // SOMA02 nc
    // SOMA03 << D0_C
    // SOMA04 nc
    // SOMA05 nc
    // SOMA06 nc
    // SOMA07 >> RYMA04
    // SOMA08 nc
    // SOMA09 == nc

    // Is this actually like a pass gate? We already know the latch cells, and this is bigger than those.

    /*p08.SOMA*/ bus_reg.CART_DATA_LATCH_00.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D0_C);
    /*p08.RONY*/ bus_reg.CART_DATA_LATCH_01.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D1_C);
    /*p08.RAXY*/ bus_reg.CART_DATA_LATCH_02.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D2_C);
    /*p08.SELO*/ bus_reg.CART_DATA_LATCH_03.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D3_C);
    /*p08.SODY*/ bus_reg.CART_DATA_LATCH_04.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D4_C);
    /*p08.SAGO*/ bus_reg.CART_DATA_LATCH_05.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D5_C);
    /*p08.RUPA*/ bus_reg.CART_DATA_LATCH_06.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D6_C);
    /*p08.SAZY*/ bus_reg.CART_DATA_LATCH_07.tp_latch(_LAVO_LATCH_EXT_DATA, ext_pins.D7_C);

    /*p08.RYMA*/ cpu_pins.D0.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_00);
    /*p08.RUVO*/ cpu_pins.D1.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_01);
    /*p08.RYKO*/ cpu_pins.D2.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_02);
    /*p08.TAVO*/ cpu_pins.D3.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_03);
    /*p08.TEPE*/ cpu_pins.D4.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_04);
    /*p08.SAFO*/ cpu_pins.D5.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_05);
    /*p08.SEVU*/ cpu_pins.D6.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_06);
    /*p08.TAJU*/ cpu_pins.D7.set_tribuf(!_LAVO_LATCH_EXT_DATA, bus_reg.CART_DATA_LATCH_07);
  }




  {
    // cpu can read oam when there's no parsing, rendering, or dma
    // so byte 0 is the palette number? something wrong here...
    // this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
    // except the inputs are negated or something?

    /*p28.GARO*/ wire _GARO_A0n = not(cpu_pins.A00);
    /*p28.WACU*/ wire _WACU_A1n = not(cpu_pins.A01);
    /*p28.GOSE*/ wire _GOSE_A2n = not(cpu_pins.A02);
    /*p28.WAPE*/ wire _WAPE_A3n = not(cpu_pins.A03);
    /*p28.FEVU*/ wire _FEVU_A4n = not(cpu_pins.A04);
    /*p28.GERA*/ wire _GERA_A5n = not(cpu_pins.A05);
    /*p28.WAXA*/ wire _WAXA_A6n = not(cpu_pins.A06);
    /*p28.FOBY*/ wire _FOBY_A7n = not(cpu_pins.A07);

    /*p28.GYBU*/ wire _GYBU_IDX_0n = not(sst_reg.WUZY_TS_IDX_0);
    /*p28.GYKA*/ wire _GYKA_IDX_1n = not(sst_reg.WYSE_TS_IDX_1);
    /*p28.FABY*/ wire _FABY_IDX_2n = not(sst_reg.ZYSU_TS_IDX_2);
    /*p28.FACO*/ wire _FACO_IDX_3n = not(sst_reg.WYDA_TS_IDX_3);
    /*p28.FUGU*/ wire _FUGU_IDX_4n = not(sst_reg.WUCO_TS_IDX_4);
    /*p28.FYKE*/ wire _FYKE_IDX_5n = not(sst_reg.WEZA_TS_IDX_5);

    /*p28.GUSE*/ wire _GUSE_SCAN0n = not(sst_reg.SCAN0.q());
    /*p28.GEMA*/ wire _GEMA_SCAN1n = not(sst_reg.SCAN1.q());
    /*p28.FUTO*/ wire _FUTO_SCAN2n = not(sst_reg.SCAN2.q());
    /*p28.FAKU*/ wire _FAKU_SCAN3n = not(sst_reg.SCAN3.q());
    /*p28.GAMA*/ wire _GAMA_SCAN4n = not(sst_reg.SCAN4.q());
    /*p28.GOBY*/ wire _GOBY_SCAN5n = not(sst_reg.SCAN5.q());

    /*p28.FETU*/ wire _FETU_DMA_A07n = not(dma_reg.DMA_A07.q());
    /*p28.FYDU*/ wire _FYDU_DMA_A06n = not(dma_reg.DMA_A06.q());
    /*p28.EDOL*/ wire _EDOL_DMA_A05n = not(dma_reg.DMA_A05.q());
    /*p28.ELUG*/ wire _ELUG_DMA_A04n = not(dma_reg.DMA_A04.q());
    /*p28.FYKY*/ wire _FYKY_DMA_A03n = not(dma_reg.DMA_A03.q());
    /*p28.FAGO*/ wire _FAGO_DMA_A02n = not(dma_reg.DMA_A02.q());
    /*p28.FESA*/ wire _FESA_DMA_A01n = not(dma_reg.DMA_A01.q());
    /*p28.FODO*/ wire _FODO_DMA_A00n = not(dma_reg.DMA_A00.q());

    /*p28.AJON*/ wire _AJON_PPU_USE_OAM2 = and (dma_sig.BOGE_DMA_RUNNINGn, ppu_reg.XYMU_RENDERINGp.q()); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire _AJUJ_OAM_BUSYn = nor(dma_reg.MATU_DMA_OAM_WRp.q(), ppu_sig.ACYL_PPU_USE_OAM1p, _AJON_PPU_USE_OAM2); // def nor
    /*p28.AMAB*/ wire _AMAB_OAM_LOCKn = and (adr_sig.SARO_FE00_FEFFp, _AJUJ_OAM_BUSYn); // def and

    /*p28.ASAM*/ wire _ASAM_CPU_OAM_RDn = or(ppu_sig.ACYL_PPU_USE_OAM1p, ppu_reg.XYMU_RENDERINGp.q(), dma_reg.MATU_DMA_OAM_WRp.q());
    /*p28.BETE*/ wire _BETE_PPU_OAM_RDn = not(_AJON_PPU_USE_OAM2);
    /*p28.APAR*/ wire _APAR_PPU_OAM_RDn = not(ppu_sig.ACYL_PPU_USE_OAM1p);
    /*p04.DUGA*/ wire _DUGA_DMA_OAM_RDn = not(dma_reg.MATU_DMA_OAM_WRp.q()); // so if dma happens during oam parse, both drive the address line - bus conflict?

    /*p28.GEKA*/ wire _GEKA_OAM_A0p = not((_GARO_A0n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.GECA_P10_Bp  & !_BETE_PPU_OAM_RDn) | (dbg_sig.GEFY_P10_Bn & !_APAR_PPU_OAM_RDn) | (_FODO_DMA_A00n & !_DUGA_DMA_OAM_RDn));
    /*p28.ZYFO*/ wire _ZYFO_OAM_A1p = not((_WACU_A1n & !_ASAM_CPU_OAM_RDn) | (dbg_sig.WYDU_P10_Bp  & !_BETE_PPU_OAM_RDn) | (dbg_sig.WUWE_P10_Bn & !_APAR_PPU_OAM_RDn) | (_FESA_DMA_A01n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOT*/ wire _YFOT_OAM_A2p = not((_GOSE_A2n & !_ASAM_CPU_OAM_RDn) | (_GYBU_IDX_0n & !_BETE_PPU_OAM_RDn) | (_GUSE_SCAN0n & !_APAR_PPU_OAM_RDn) | (_FAGO_DMA_A02n & !_DUGA_DMA_OAM_RDn));
    /*p28.YFOC*/ wire _YFOC_OAM_A3p = not((_WAPE_A3n & !_ASAM_CPU_OAM_RDn) | (_GYKA_IDX_1n & !_BETE_PPU_OAM_RDn) | (_GEMA_SCAN1n & !_APAR_PPU_OAM_RDn) | (_FYKY_DMA_A03n & !_DUGA_DMA_OAM_RDn));
    /*p28.YVOM*/ wire _YVOM_OAM_A4p = not((_FEVU_A4n & !_ASAM_CPU_OAM_RDn) | (_FABY_IDX_2n & !_BETE_PPU_OAM_RDn) | (_FUTO_SCAN2n & !_APAR_PPU_OAM_RDn) | (_ELUG_DMA_A04n & !_DUGA_DMA_OAM_RDn));
    /*p28.YMEV*/ wire _YMEV_OAM_A5p = not((_GERA_A5n & !_ASAM_CPU_OAM_RDn) | (_FACO_IDX_3n & !_BETE_PPU_OAM_RDn) | (_FAKU_SCAN3n & !_APAR_PPU_OAM_RDn) | (_EDOL_DMA_A05n & !_DUGA_DMA_OAM_RDn));
    /*p28.XEMU*/ wire _XEMU_OAM_A6p = not((_WAXA_A6n & !_ASAM_CPU_OAM_RDn) | (_FUGU_IDX_4n & !_BETE_PPU_OAM_RDn) | (_GAMA_SCAN4n & !_APAR_PPU_OAM_RDn) | (_FYDU_DMA_A06n & !_DUGA_DMA_OAM_RDn));
    /*p28.YZET*/ wire _YZET_OAM_A7p = not((_FOBY_A7n & !_ASAM_CPU_OAM_RDn) | (_FYKE_IDX_5n & !_BETE_PPU_OAM_RDn) | (_GOBY_SCAN5n & !_APAR_PPU_OAM_RDn) | (_FETU_DMA_A07n & !_DUGA_DMA_OAM_RDn));

    /*p28.GEKA*/ oam_pins.A0.set(_GEKA_OAM_A0p);
    /*p28.ZYFO*/ oam_pins.A1.set(_ZYFO_OAM_A1p);
    /*p28.YFOT*/ oam_pins.A2.set(_YFOT_OAM_A2p);
    /*p28.YFOC*/ oam_pins.A3.set(_YFOC_OAM_A3p);
    /*p28.YVOM*/ oam_pins.A4.set(_YVOM_OAM_A4p);
    /*p28.YMEV*/ oam_pins.A5.set(_YMEV_OAM_A5p);
    /*p28.XEMU*/ oam_pins.A6.set(_XEMU_OAM_A6p);
    /*p28.YZET*/ oam_pins.A7.set(_YZET_OAM_A7p);

    /*p04.NAXY*/ wire _NAXY_DMA_OAM_WENp = nor(dbg_reg.FROM_CPU5_SYNC.q(), dma_reg.LUVY_DMA_TRIG_d0.q()); // def nor
    /*p04.POWU*/ wire _POWU_DMA_OAM_WRp  = and(dma_reg.MATU_DMA_OAM_WRp.q(), _NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire _WYJA_OAM_WRp      = or(and(_AMAB_OAM_LOCKn, cpu_sig.CUPA_CPU_WR_xxxxxFGH), _POWU_DMA_OAM_WRp);

    /*p28.WAFO*/ wire _WAFO_OAM_A0n      = not(_GEKA_OAM_A0p); // def not
    /*p28.YNYC*/ wire _YNYC_OAM_B_WRn    = and(_WYJA_OAM_WRp, _WAFO_OAM_A0n); // def and
    /*p28.YLYC*/ wire _YLYC_OAM_A_WRn    = and(_WYJA_OAM_WRp, _GEKA_OAM_A0p); // def and

    /*p28.ZONE*/ wire _ZONE = not(_YLYC_OAM_A_WRn);
    /*p28.ZOFE*/ wire _ZOFE = not(_YNYC_OAM_B_WRn);

    oam_pins.WR_A.set(_ZONE);
    oam_pins.WR_B.set(_ZOFE);

    /*p04.CATY*/ wire _CATY_FROM_CPU5 = not(cpu_sig.DECY_FROM_CPU5n);
    /*p28.MYNU*/ wire _MYNU_CPU_READ_MYSTERYn = nand(cpu_sig.ASOT_CPU_RD, _CATY_FROM_CPU5);
    /*p28.LEKO*/ wire _LEKO_CPU_READ_MYSTERY = not(_MYNU_CPU_READ_MYSTERYn);

    /*p28.WUKU*/ wire _WUKU_OAM_A_CPU_RD = and (_LEKO_CPU_READ_MYSTERY, _AMAB_OAM_LOCKn, oam_pins.A0);
    /*p28.GUKO*/ wire _GUKO_OAM_B_CPU_RD = and (_LEKO_CPU_READ_MYSTERY, _AMAB_OAM_LOCKn, _WAFO_OAM_A0n);
    /*p28.WEWU*/ wire _WEWU_OAM_A_CPU_RDn = not(_WUKU_OAM_A_CPU_RD);
    /*p28.WUME*/ wire _WUME_OAM_B_CPU_RDn = not(_GUKO_OAM_B_CPU_RD);

    /*p31.XACA*/ cpu_pins.D0.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A0);
    /*p31.XAGU*/ cpu_pins.D1.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A1);
    /*p31.XEPU*/ cpu_pins.D2.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A2);
    /*p31.XYGU*/ cpu_pins.D3.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A3);
    /*p31.XUNA*/ cpu_pins.D4.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A4);
    /*p31.DEVE*/ cpu_pins.D5.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A5);
    /*p31.ZEHA*/ cpu_pins.D6.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A6);
    /*p31.FYRA*/ cpu_pins.D7.set_tribuf(!_WEWU_OAM_A_CPU_RDn, oam_reg.LATCH_OAM_A7);

    /*p29.YFAP*/ cpu_pins.D0.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B0); // big tri
    /*p29.XELE*/ cpu_pins.D1.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B1);
    /*p29.YPON*/ cpu_pins.D2.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B2);
    /*p29.XUVO*/ cpu_pins.D3.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B3);
    /*p29.ZYSA*/ cpu_pins.D4.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B4);
    /*p29.YWEG*/ cpu_pins.D5.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B5);
    /*p29.XABU*/ cpu_pins.D6.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B6);
    /*p29.YTUX*/ cpu_pins.D7.set_tribuf(!_WUME_OAM_B_CPU_RDn, oam_reg.LATCH_OAM_B7);

    /*p25.TYVY*/ wire _TYVY_MD_TO_Dn = nand(ppu_sig.SERE_VRAM_RD, _LEKO_CPU_READ_MYSTERY);
    /*p25.SEBY*/ wire _SEBY_MD_TO_D = not(_TYVY_MD_TO_Dn);
    /*p25.RUGA*/ /*p25.RERY*/ cpu_pins.D0.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD0));
    /*p25.ROTA*/ /*p25.RUNA*/ cpu_pins.D1.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD1));
    /*p25.RYBU*/ /*p25.RONA*/ cpu_pins.D2.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD2));
    /*p25.RAJU*/ /*p25.RUNO*/ cpu_pins.D3.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD3));
    /*p25.TYJA*/ /*p25.SANA*/ cpu_pins.D4.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD4));
    /*p25.REXU*/ /*p25.RORO*/ cpu_pins.D5.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD5));
    /*p25.RUPY*/ /*p25.RABO*/ cpu_pins.D6.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD6));
    /*p25.TOKU*/ /*p25.SAME*/ cpu_pins.D7.set_tribuf(_SEBY_MD_TO_D, !not(ppu_reg.MD7));

    /*p28.XUTO*/ wire _XUTO_CPU_OAM_WR = and(adr_sig.SARO_FE00_FEFFp, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p28.WUJE*/ wire _WUJE_CPU_OAM_WR = or(clk_sig.XYNY_xBCDExxx, _XUTO_CPU_OAM_WR);
    /*p28.XUPA*/ wire _XUPA_CPU_OAM_WR = not(_WUJE_CPU_OAM_WR);
    /*p28.ADAH*/ wire _ADAH_ADDR_OAMn  = not(adr_sig.SARO_FE00_FEFFp);
    /*p28.APAG*/ wire _APAG_D_TO_OAMDn = amux2(_XUPA_CPU_OAM_WR, _AMAB_OAM_LOCKn, _AJUJ_OAM_BUSYn, _ADAH_ADDR_OAMn);
    /*p28.AZUL*/ wire _AZUL_D_TO_OAMD  = not(_APAG_D_TO_OAMDn);

    /*p28.ZAXA*/ oam_pins.A_D0.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D0);
    /*p28.ZAKY*/ oam_pins.A_D1.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D1);
    /*p28.WULE*/ oam_pins.A_D2.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D2);
    /*p28.ZOZO*/ oam_pins.A_D3.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D3);
    /*p28.ZUFO*/ oam_pins.A_D4.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D4);
    /*p28.ZATO*/ oam_pins.A_D5.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D5);
    /*p28.YVUC*/ oam_pins.A_D6.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D6);
    /*p28.ZUFE*/ oam_pins.A_D7.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D7);

    /*p28.ZAMY*/ oam_pins.B_D0.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D0); // small tri
    /*p28.ZOPU*/ oam_pins.B_D1.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D1);
    /*p28.WYKY*/ oam_pins.B_D2.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D2);
    /*p28.ZAJA*/ oam_pins.B_D3.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D3);
    /*p28.ZUGA*/ oam_pins.B_D4.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D4);
    /*p28.ZUMO*/ oam_pins.B_D5.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D5);
    /*p28.XYTO*/ oam_pins.B_D6.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D6);
    /*p28.ZYFA*/ oam_pins.B_D7.set_tribuf(_AZUL_D_TO_OAMD, cpu_pins.D7);
  }

  {
    /*p28.AZAR*/ wire _AZAR_DMA_READ_VRAMn = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p28.WUZU*/ oam_pins.A_D0.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD0);
    /*p28.AXER*/ oam_pins.A_D1.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD1);
    /*p28.ASOX*/ oam_pins.A_D2.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD2);
    /*p28.CETU*/ oam_pins.A_D3.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD3);
    /*p28.ARYN*/ oam_pins.A_D4.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD4);
    /*p28.ACOT*/ oam_pins.A_D5.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD5);
    /*p28.CUJE*/ oam_pins.A_D6.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD6);
    /*p28.ATER*/ oam_pins.A_D7.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD7);
    /*p28.WOWA*/ oam_pins.B_D0.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD0);
    /*p28.AVEB*/ oam_pins.B_D1.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD1);
    /*p28.AMUH*/ oam_pins.B_D2.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD2);
    /*p28.COFO*/ oam_pins.B_D3.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD3);
    /*p28.AZOZ*/ oam_pins.B_D4.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD4);
    /*p28.AGYK*/ oam_pins.B_D5.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD5);
    /*p28.BUSE*/ oam_pins.B_D6.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD6);
    /*p28.ANUM*/ oam_pins.B_D7.set_tribuf(_AZAR_DMA_READ_VRAMn, ppu_reg.MD7);
  }

  {
    /*p25.WEJO*/ oam_pins.A_D0.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_pins.D0_C)));
    /*p25.BUBO*/ oam_pins.A_D1.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_pins.D1_C)));
    /*p25.BETU*/ oam_pins.A_D2.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_pins.D2_C)));
    /*p25.CYME*/ oam_pins.A_D3.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_pins.D3_C)));
    /*p25.BAXU*/ oam_pins.A_D4.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_pins.D4_C)));
    /*p25.BUHU*/ oam_pins.A_D5.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_pins.D5_C)));
    /*p25.BYNY*/ oam_pins.A_D6.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_pins.D6_C)));
    /*p25.BYPY*/ oam_pins.A_D7.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_pins.D7_C)));
    /*p25.WASA*/ oam_pins.B_D0.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.RALO*/ not(ext_pins.D0_C)));
    /*p25.BOMO*/ oam_pins.B_D1.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUNE*/ not(ext_pins.D1_C)));
    /*p25.BASA*/ oam_pins.B_D2.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SERA*/ not(ext_pins.D2_C)));
    /*p25.CAKO*/ oam_pins.B_D3.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TENU*/ not(ext_pins.D3_C)));
    /*p25.BUMA*/ oam_pins.B_D4.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYSA*/ not(ext_pins.D4_C)));
    /*p25.BUPY*/ oam_pins.B_D5.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SUGY*/ not(ext_pins.D5_C)));
    /*p25.BASY*/ oam_pins.B_D6.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.TUBE*/ not(ext_pins.D6_C)));
    /*p25.BAPE*/ oam_pins.B_D7.set_tribuf(dma_sig.MORY_DMA_READ_CARTn, not(/*p25.SYZO*/ not(ext_pins.D7_C)));
  }

  {
    // Schematic very wrong
    // TUVO01 << TEPA02
    // TUVO02 << TULY17
    // TUVO03 << TESE17
    // TUVO04 >> VAPE02

    // WEFY01
    // WEFY02
    // WEFY03
    // WEFY04

    // AJEP def nand
    // XUJA def not
    // BOTA def nand
    // ASYT def and
    // BODE def not
    // YVAL def not
    // YRYV def not
    // ZODO def not

    /*p29.TUVO*/ wire _TUVO_PPU_OAM_RDp = nor(ppu_sig.TEPA_RENDERINGn, ppu_reg.TULY_SFETCH_S1.q(), ppu_reg.TESE_SFETCH_S2.q());

    /*p28.WEFY*/ wire _WEFY_SPR_READp = and(_TUVO_PPU_OAM_RDp, ppu_reg.TYFO_SFETCH_S0_D1.q());
    /*p28.AJEP*/ wire _AJEP = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XOCE_ABxxEFxx); // schematic wrong, is def nand
    /*p28.XUJA*/ wire _XUJA_SPR_READn = not(_WEFY_SPR_READp);
    /*p28.BOTA*/ wire _BOTA_CPU_RD_OAMn = nand(cpu_sig.DECY_FROM_CPU5n, adr_sig.SARO_FE00_FEFFp, cpu_sig.ASOT_CPU_RD); // Schematic wrong, this is NAND
    /*p28.ASYT*/ wire _ASYT_OAM_LATCH = and(_AJEP, _XUJA_SPR_READn, _BOTA_CPU_RD_OAMn);
    /*p28.BODE*/ wire _BODE_OAM_LATCH = not(_ASYT_OAM_LATCH); // to the tribus receiver below

    {
      /*p28.YVAL*/ wire _YVAL_OAM_LATCHo = not(_BODE_OAM_LATCH);
      /*p28.YRYV*/ wire _YRYU_OAM_LATCHb = not(_YVAL_OAM_LATCHo);
      /*p28.ZODO*/ wire _ZODO_OAM_OE = not(_YRYU_OAM_LATCHb);
      oam_pins.ZODO_OE.set(_ZODO_OAM_OE);
    }

    /*p31.XYKY*/ oam_reg.LATCH_OAM_A0.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D0);
    /*p31.YRUM*/ oam_reg.LATCH_OAM_A1.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D1);
    /*p31.YSEX*/ oam_reg.LATCH_OAM_A2.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D2);
    /*p31.YVEL*/ oam_reg.LATCH_OAM_A3.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D3);
    /*p31.WYNO*/ oam_reg.LATCH_OAM_A4.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D4);
    /*p31.CYRA*/ oam_reg.LATCH_OAM_A5.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D5);
    /*p31.ZUVE*/ oam_reg.LATCH_OAM_A6.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D6);
    /*p31.ECED*/ oam_reg.LATCH_OAM_A7.tp_latch(_BODE_OAM_LATCH, oam_pins.A_D7);

    /*p29.YDYV*/ oam_reg.LATCH_OAM_B0.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D0);
    /*p29.YCEB*/ oam_reg.LATCH_OAM_B1.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D1);
    /*p29.ZUCA*/ oam_reg.LATCH_OAM_B2.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D2);
    /*p29.WONE*/ oam_reg.LATCH_OAM_B3.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D3);
    /*p29.ZAXE*/ oam_reg.LATCH_OAM_B4.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D4);
    /*p29.XAFU*/ oam_reg.LATCH_OAM_B5.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D5);
    /*p29.YSES*/ oam_reg.LATCH_OAM_B6.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D6);
    /*p29.ZECA*/ oam_reg.LATCH_OAM_B7.tp_latch(_BODE_OAM_LATCH, oam_pins.B_D7);

    // Schematic wrong, AVER is def nand
    // AVER01 << ACYL03
    // AVER02 << XYSO02
    // AVER03 >> BYCU03

    // VAPE is def and
    // VAPE01 << TACU03
    // VAPE02 << TUVO04
    // VAPE03 nc
    // VAPE04 >> XUJY01

    /*p25.AVER*/ wire _AVER = nand(ppu_sig.ACYL_PPU_USE_OAM1p, clk_sig.XYSO_ABCxDEFx); 
    /*p25.VAPE*/ wire _VAPE = and (_TUVO_PPU_OAM_RDp, ppu_sig.TACU_SPR_SEQ_5_TRIG);
    /*p25.XUJY*/ wire _XUJY = not(_VAPE);

    // Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())

    // 4-rung whereas the or(and()) were 5 rung?
    // Arm on left (gnd) side
    // CUFE01 << SARO03
    // CUFE02 << MATU17
    // CUFE03 >> BYCU01
    // CUFE04 << MOPA03

    /*p25.CUFE*/ wire _CUFE_OAM_WR = and (or (adr_sig.SARO_FE00_FEFFp, dma_reg.MATU_DMA_OAM_WRp.q()), clk_sig.MOPA_AxxxxFGH);

    /*p25.BYCU*/ wire _BYCU_OAM_CLK = nand(_AVER, _XUJY, _CUFE_OAM_WR); // schematic wrong, this is NAND... but that doesn't make sense?
    /*p25.COTA*/ wire _COTA_OAM_CLK = not(_BYCU_OAM_CLK);
    oam_pins.COTA_CLK.set(_COTA_OAM_CLK);

    /*p31.XEGA*/ wire XEGA_OAM_CLKn = not(_COTA_OAM_CLK);
    /*p31.YLOR*/ oam_reg.YLOR_SPRITE_X0.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A0);
    /*p31.ZYTY*/ oam_reg.ZYTY_SPRITE_X1.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A1);
    /*p31.ZYVE*/ oam_reg.ZYVE_SPRITE_X2.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A2);
    /*p31.ZEZY*/ oam_reg.ZEZY_SPRITE_X3.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A3);
    /*p31.GOMO*/ oam_reg.GOMO_SPRITE_X4.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A4);
    /*p31.BAXO*/ oam_reg.BAXO_SPRITE_X5.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A5);
    /*p31.YZOS*/ oam_reg.YZOS_SPRITE_X6.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A6);
    /*p31.DEPO*/ oam_reg.DEPO_SPRITE_X7.set(XEGA_OAM_CLKn, oam_reg.LATCH_OAM_A7);

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not(_COTA_OAM_CLK);
    /*p29.XUSO*/ oam_reg.XUSO_SPRITE_Y0.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B0);
    /*p29.XEGU*/ oam_reg.XEGU_SPRITE_Y1.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B1);
    /*p29.YJEX*/ oam_reg.YJEX_SPRITE_Y2.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B2);
    /*p29.XYJU*/ oam_reg.XYJU_SPRITE_Y3.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B3);
    /*p29.YBOG*/ oam_reg.YBOG_SPRITE_Y4.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B4);
    /*p29.WYSO*/ oam_reg.WYSO_SPRITE_Y5.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B5);
    /*p29.XOTE*/ oam_reg.XOTE_SPRITE_Y6.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B6);
    /*p29.YZAB*/ oam_reg.YZAB_SPRITE_Y7.set(YWOK_OAM_CLKn, oam_reg.LATCH_OAM_B7);
  }
}
