#include "Sch_VRAM.h"

#include "TestGB.h"

using namespace Schematics;

// Die trace:
// TEFA = nor(SYRO, TEXO);         OK
// SOSE = and( A15, TEFA);         OK
// TUCA = and(SOSE, ABUZ);         OK
// TEGU = nand(SOSE, CPU_RAW_WR);  OK
// TAVY = not(MOE_C);              OK
// TEFY = not(MCS_C);              OK

// TUTO = and(TOVA, SOTO);         OK
// ROPY = not(XYMU);               OK

// TOLE = mux2(TUTO, TEFY, TUCA)   OK
// SERE = and(TOLE, ROPY)          OK
// SALE = mux2(TUTO, TAVY, TEGU)   OK
// RUVY = not(SALE);               OK
// SAZO = and(SERE, RUVY)          OK
// RYJE = not(SAZO)                OK
// REVO = not(RYJE)                OK
// ROCY = and(SAZO, REVO)          OK
// RAHU = not(ROCY)                OK, big inverter
// ROVE = not(RAHU)                OK
// RELA = or(REVO, SAZO)          OK
// RENA = not(RELA)                OK

// XAKY 6-rung tribuf, inputs on 1 and 4
// XAKY01
// XAKY02 nc
// XAKY03 nc
// XAKY04
// XAKY05 nc
// XAKY06

//---------------------------------------------------------------------------

void VramPins::tick(TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto win_sig = gb.win_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  auto& ppu_reg = gb.ppu_reg;

  auto& vram_bus = gb.vram_bus;

  auto& cpu_bus = gb.cpu_bus;

  /*p25.XANE*/ wire _XANE_VRAM_LOCKn = nor(dma_sig.LUFA_DMA_READ_VRAMp, ppu_sig.XYMU_RENDERINGp); // def nor
  
  //---------------------------------------------------------------------------

  /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(ppu_reg.LONY_BG_READ_VRAM_LATCHp.q());
  /*p27.LENA*/ wire LENA_BGW_VRAM_RD = not(LUSU_BGW_VRAM_RDn);
  /*p29.ABON*/ wire ABON_SPR_VRAM_RDp1 = not(ppu_sig.TEXY_SPRITE_READ);

  // How does DMA/PPU lock come into play here? Does it?
  // PPU locks via RENDERING_LATCH, dma by...? well it doesn't have to lock since it's just reading.
  // so really if the address is muxed right, we only need to block writes while vram's locked.
  // oh there's also some locking down in the pin driver...

  //----------

  /*p25.TAVY*/ wire TAVY_MOE_Cn = not(PIN_MOEn_C);
  /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(adr_sig.SOSE_8000_9FFFp, cpu_bus.PIN_CPU_RAW_WR); // Schematic wrong, second input is CPU_RAW_WR
  /*p25.SALE*/ wire SALE_DBG_VRAM_RDb = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, dbg_sig.TUTO_DBG_VRAM);
  /*p25.RUVY*/ wire RUVY_VRAM_WR = not(SALE_DBG_VRAM_RDb);
  /*p25.SAZO*/ wire SAZO_VRAM_RD = and (RUVY_VRAM_WR, ppu_sig.SERE_VRAM_RD);
  /*p25.RYJE*/ wire RYJE_VRAM_RDn = not(SAZO_VRAM_RD);
  /*p25.REVO*/ wire REVO_VRAM_RDp = not(RYJE_VRAM_RDn);
  /*p25.ROCY*/ wire ROCY_VBUS_TRISTATEn = and (REVO_VRAM_RDp, SAZO_VRAM_RD);
  /*p25.RAHU*/ wire RAHU_VBUS_TRISTATEp = not(ROCY_VBUS_TRISTATEn);
  /*p25.ROVE*/ wire ROVE_VBUS_TRISTATEn = not(RAHU_VBUS_TRISTATEp);

  /*p25.RELA*/ wire RELA_VRAM_DRIVEp = or (REVO_VRAM_RDp, SAZO_VRAM_RD);
  /*p25.RENA*/ wire RENA_VRAM_DRIVEn = not(RELA_VRAM_DRIVEp);

  {
    /*p25.REGE*/ PIN_MD0_A.set(not(/*p25.SEFA*/ and(vram_bus.TS_MD0, ROVE_VBUS_TRISTATEn)));
    /*p25.RYKY*/ PIN_MD1_A.set(not(/*p25.SOGO*/ and(vram_bus.TS_MD1, ROVE_VBUS_TRISTATEn)));
    /*p25.RAZO*/ PIN_MD2_A.set(not(/*p25.SEFU*/ and(vram_bus.TS_MD2, ROVE_VBUS_TRISTATEn)));
    /*p25.RADA*/ PIN_MD3_A.set(not(/*p25.SUNA*/ and(vram_bus.TS_MD3, ROVE_VBUS_TRISTATEn)));
    /*p25.RYRO*/ PIN_MD4_A.set(not(/*p25.SUMO*/ and(vram_bus.TS_MD4, ROVE_VBUS_TRISTATEn)));
    /*p25.REVU*/ PIN_MD5_A.set(not(/*p25.SAZU*/ and(vram_bus.TS_MD5, ROVE_VBUS_TRISTATEn)));
    /*p25.REKU*/ PIN_MD6_A.set(not(/*p25.SAMO*/ and(vram_bus.TS_MD6, ROVE_VBUS_TRISTATEn)));
    /*p25.RYZE*/ PIN_MD7_A.set(not(/*p25.SUKE*/ and(vram_bus.TS_MD7, ROVE_VBUS_TRISTATEn)));

    /*p25.RURA*/ PIN_MD0_D.set(not(/*p25.SYNU*/ or(vram_bus.TS_MD0, RAHU_VBUS_TRISTATEp)));
    /*p25.RULY*/ PIN_MD1_D.set(not(/*p25.SYMA*/ or(vram_bus.TS_MD1, RAHU_VBUS_TRISTATEp)));
    /*p25.RARE*/ PIN_MD2_D.set(not(/*p25.ROKO*/ or(vram_bus.TS_MD2, RAHU_VBUS_TRISTATEp)));
    /*p25.RODU*/ PIN_MD3_D.set(not(/*p25.SYBU*/ or(vram_bus.TS_MD3, RAHU_VBUS_TRISTATEp)));
    /*p25.RUBE*/ PIN_MD4_D.set(not(/*p25.SAKO*/ or(vram_bus.TS_MD4, RAHU_VBUS_TRISTATEp)));
    /*p25.RUMU*/ PIN_MD5_D.set(not(/*p25.SEJY*/ or(vram_bus.TS_MD5, RAHU_VBUS_TRISTATEp)));
    /*p25.RYTY*/ PIN_MD6_D.set(not(/*p25.SEDO*/ or(vram_bus.TS_MD6, RAHU_VBUS_TRISTATEp)));
    /*p25.RADY*/ PIN_MD7_D.set(not(/*p25.SAWU*/ or(vram_bus.TS_MD7, RAHU_VBUS_TRISTATEp)));
  }

  {

    /*p25.ROFA*/ PIN_MD0_B.set(not(RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ PIN_MD1_B.set(not(RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ PIN_MD2_B.set(not(RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ PIN_MD3_B.set(not(RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ PIN_MD4_B.set(not(RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ PIN_MD5_B.set(not(RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ PIN_MD6_B.set(not(RENA_VRAM_DRIVEn));
    /*p25.ROFA*/ PIN_MD7_B.set(not(RENA_VRAM_DRIVEn));
  }

  // Die trace
  // MYMA = not(LONY);
  // APAM = not(LUFA);
  // RACU = and(RYLU, RAWA, MYMA, APAM);

  {
    /*p25.TUJA*/ wire _TUJA_CPU_VRAM_WR = and(adr_sig.SOSE_8000_9FFFp, cpu_sig.APOV_CPU_WR_xxxxxFGH);
    /*p25.SUDO*/ wire _SUDO_MWR_Cn = not(PIN_MWRn_C);
    /*p25.TYJY*/ wire _TYJY_DBG_VRAM_WR = mux2_p(_SUDO_MWR_Cn, _TUJA_CPU_VRAM_WR, dbg_sig.TUTO_DBG_VRAM);
    /*p25.SOHY*/ wire _SOHY_MWR = nand(_TYJY_DBG_VRAM_WR, ppu_sig.SERE_VRAM_RD);
    /*p25.RACO*/ wire _RACO_DBG_VRAMn = not(dbg_sig.TUTO_DBG_VRAM);
    /*p25.TAXY*/ wire _TAXY_MWR_An = and(_SOHY_MWR, _RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire _SOFY_MWR_Dn = or(_SOHY_MWR, dbg_sig.TUTO_DBG_VRAM);
    /*p25.RYLU*/ wire _RYLU_DBG_VRAM_RDn = nand(SALE_DBG_VRAM_RDb, _XANE_VRAM_LOCKn);
    /*p25.SOHO*/ wire _SOHO_SPR_VRAM_RDp2 = and(ppu_sig.TACU_SPR_SEQ_5_TRIG, ABON_SPR_VRAM_RDp1);
    /*p25.RAWA*/ wire _RAWA_SPR_VRAM_RDn1 = not(_SOHO_SPR_VRAM_RDp2);
    /*p25.APAM*/ wire _APAM_DMA_VRAM_RDn2 = not(dma_sig.LUFA_DMA_READ_VRAMp);
    /*p27.MYMA*/ wire _MYMA_BGW_VRAM_RDn = not(ppu_reg.LONY_BG_READ_VRAM_LATCHp.q()); // this should be correct
    /*p25.RACU*/ wire _RACU_MOEn = and(_RYLU_DBG_VRAM_RDn, _RAWA_SPR_VRAM_RDn1, _MYMA_BGW_VRAM_RDn, _APAM_DMA_VRAM_RDn2); // def and
    /*p25.SUTU*/ wire _SUTU_READ_VRAMn = nor(LENA_BGW_VRAM_RD, dma_sig.LUFA_DMA_READ_VRAMp, ABON_SPR_VRAM_RDp1, ppu_sig.SERE_VRAM_RD);
    /*p25.SEMA*/ wire _SEMA_MOE_An = and (_RACU_MOEn, _RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire _RUTE_MOE_Dn = or(dbg_sig.TUTO_DBG_VRAM, _RACU_MOEn); // schematic wrong, second input is RACU
    /*p25.TODE*/ wire _TODE_MCS_An = and(_SUTU_READ_VRAMn, _RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire _SEWO_MCS_Dn = or(_SUTU_READ_VRAMn, dbg_sig.TUTO_DBG_VRAM);

    /*p25.SYSY*/ PIN_MWRn_A.set(not(_TAXY_MWR_An));
    /*p25.RAGU*/ PIN_MWRn_D.set(not(_SOFY_MWR_Dn));
    /*p25.REFO*/ PIN_MOEn_A.set(not(_SEMA_MOE_An));
    /*p25.SAHA*/ PIN_MOEn_D.set(not(_RUTE_MOE_Dn));
    /*p25.SOKY*/ PIN_MCSn_A.set(not(_TODE_MCS_An));
    /*p25.SETY*/ PIN_MCSn_D.set(not(_SEWO_MCS_Dn));
  }
}