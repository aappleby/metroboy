#include "Sch_VramPins.h"

#include "Sch_Top.h"

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

// How does DMA/PPU lock come into play here? Does it?
// PPU locks via RENDERING_LATCH, dma by...? well it doesn't have to lock since it's just reading.
// so really if the address is muxed right, we only need to block writes while vram's locked.
// oh there's also some locking down in the pin driver...

//---------------------------------------------------------------------------

void VramPins::tick(SchematicTop& top) {

  {
    /*p25.LEXE*/ top.EXT_PIN_MA00_AD.set(not(/*p25.MYFU*/ not(top.VRM_TRI_A00)));
    /*p25.LOZU*/ top.EXT_PIN_MA01_AD.set(not(/*p25.MASA*/ not(top.VRM_TRI_A01)));
    /*p25.LACA*/ top.EXT_PIN_MA02_AD.set(not(/*p25.MYRE*/ not(top.VRM_TRI_A02)));
    /*p25.LUVO*/ top.EXT_PIN_MA03_AD.set(not(/*p25.MAVU*/ not(top.VRM_TRI_A03)));
    /*p25.LOLY*/ top.EXT_PIN_MA04_AD.set(not(/*p25.MEPA*/ not(top.VRM_TRI_A04)));
    /*p25.LALO*/ top.EXT_PIN_MA05_AD.set(not(/*p25.MYSA*/ not(top.VRM_TRI_A05)));
    /*p25.LEFA*/ top.EXT_PIN_MA06_AD.set(not(/*p25.MEWY*/ not(top.VRM_TRI_A06)));
    /*p25.LUBY*/ top.EXT_PIN_MA07_AD.set(not(/*p25.MUME*/ not(top.VRM_TRI_A07)));
    /*p25.TUJY*/ top.EXT_PIN_MA08_AD.set(not(/*p25.VOVA*/ not(top.VRM_TRI_A08)));
    /*p25.TAGO*/ top.EXT_PIN_MA09_AD.set(not(/*p25.VODE*/ not(top.VRM_TRI_A09)));
    /*p25.NUVA*/ top.EXT_PIN_MA10_AD.set(not(/*p25.RUKY*/ not(top.VRM_TRI_A10)));
    /*p25.PEDU*/ top.EXT_PIN_MA11_AD.set(not(/*p25.RUMA*/ not(top.VRM_TRI_A11)));
    /*p25.PONY*/ top.EXT_PIN_MA12_AD.set(not(/*p25.REHO*/ not(top.VRM_TRI_A12)));
  }

  {
    auto ppu_sig = top.ppu_reg.sig(top);

    /*p25.TAVY*/ wire TAVY_MOE_Cn = not(top.EXT_PIN_MOEn_C);
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09);
    /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp);
    /*p08.SORE*/ wire SORE_0000_7FFFp = not(top.CPU_PIN_A15);
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(top.CPU_PIN_A13, top.CPU_PIN_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (top.CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
    /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor(SYRO_FE00_FFFFp, TEXO_8000_9FFFn);
    /*p25.SOSE*/ wire SOSE_8000_9FFFp = and (top.CPU_PIN_A15, TEFA_8000_9FFFp);
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(SOSE_8000_9FFFp, top.CPU_PIN_WR); // Schematic wrong, second input is CPU_RAW_WR
    /*p25.SALE*/ wire SALE_DBG_VRAM_RDb = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, top.TUTO_DBG_VRAMp());
    /*p25.RUVY*/ wire RUVY_VRAM_WR = not(SALE_DBG_VRAM_RDb);
    /*p25.SAZO*/ wire SAZO_VRAM_RD = and (RUVY_VRAM_WR, top.SERE_VRAM_RD());

    /*p25.RYJE*/ wire RYJE_VRAM_RDn = not(SAZO_VRAM_RD);
    /*p25.REVO*/ wire REVO_VRAM_RDp = not(RYJE_VRAM_RDn);
    /*p25.ROCY*/ wire ROCY_VBUS_TRISTATEn = and (REVO_VRAM_RDp, SAZO_VRAM_RD);

    /*p25.RAHU*/ wire RAHU_VBUS_TRISTATEp = not(ROCY_VBUS_TRISTATEn);
    /*p25.SYNU*/ wire SYNU = or (top.VRM_TRI_D0, RAHU_VBUS_TRISTATEp);
    /*p25.SYMA*/ wire SYMA = or (top.VRM_TRI_D1, RAHU_VBUS_TRISTATEp);
    /*p25.ROKO*/ wire ROKO = or (top.VRM_TRI_D2, RAHU_VBUS_TRISTATEp);
    /*p25.SYBU*/ wire SYBU = or (top.VRM_TRI_D3, RAHU_VBUS_TRISTATEp);
    /*p25.SAKO*/ wire SAKO = or (top.VRM_TRI_D4, RAHU_VBUS_TRISTATEp);
    /*p25.SEJY*/ wire SEJY = or (top.VRM_TRI_D5, RAHU_VBUS_TRISTATEp);
    /*p25.SEDO*/ wire SEDO = or (top.VRM_TRI_D6, RAHU_VBUS_TRISTATEp);
    /*p25.SAWU*/ wire SAWU = or (top.VRM_TRI_D7, RAHU_VBUS_TRISTATEp);

    /*p25.ROVE*/ wire ROVE_VBUS_TRISTATEn = not(RAHU_VBUS_TRISTATEp);
    /*p25.SEFA*/ wire SEFA = and(top.VRM_TRI_D0, ROVE_VBUS_TRISTATEn);
    /*p25.SOGO*/ wire SOGO = and(top.VRM_TRI_D1, ROVE_VBUS_TRISTATEn);
    /*p25.SEFU*/ wire SEFU = and(top.VRM_TRI_D2, ROVE_VBUS_TRISTATEn);
    /*p25.SUNA*/ wire SUNA = and(top.VRM_TRI_D3, ROVE_VBUS_TRISTATEn);
    /*p25.SUMO*/ wire SUMO = and(top.VRM_TRI_D4, ROVE_VBUS_TRISTATEn);
    /*p25.SAZU*/ wire SAZU = and(top.VRM_TRI_D5, ROVE_VBUS_TRISTATEn);
    /*p25.SAMO*/ wire SAMO = and(top.VRM_TRI_D6, ROVE_VBUS_TRISTATEn);
    /*p25.SUKE*/ wire SUKE = and(top.VRM_TRI_D7, ROVE_VBUS_TRISTATEn);

    /*p25.REGE*/ wire REGE = not(SEFA);
    /*p25.RYKY*/ wire RYKY = not(SOGO);
    /*p25.RAZO*/ wire RAZO = not(SEFU);
    /*p25.RADA*/ wire RADA = not(SUNA);
    /*p25.RYRO*/ wire RYRO = not(SUMO);
    /*p25.REVU*/ wire REVU = not(SAZU);
    /*p25.REKU*/ wire REKU = not(SAMO);
    /*p25.RYZE*/ wire RYZE = not(SUKE);

    /*p25.RURA*/ wire RURA = not(SYNU);
    /*p25.RULY*/ wire RULY = not(SYMA);
    /*p25.RARE*/ wire RARE = not(ROKO);
    /*p25.RODU*/ wire RODU = not(SYBU);
    /*p25.RUBE*/ wire RUBE = not(SAKO);
    /*p25.RUMU*/ wire RUMU = not(SEJY);
    /*p25.RYTY*/ wire RYTY = not(SEDO);
    /*p25.RADY*/ wire RADY = not(SAWU);

    top.EXT_PIN_MD0_A.set(REGE);
    top.EXT_PIN_MD1_A.set(RYKY);
    top.EXT_PIN_MD2_A.set(RAZO);
    top.EXT_PIN_MD3_A.set(RADA);
    top.EXT_PIN_MD4_A.set(RYRO);
    top.EXT_PIN_MD5_A.set(REVU);
    top.EXT_PIN_MD6_A.set(REKU);
    top.EXT_PIN_MD7_A.set(RYZE);

    top.EXT_PIN_MD0_D.set(RURA);
    top.EXT_PIN_MD1_D.set(RULY);
    top.EXT_PIN_MD2_D.set(RARE);
    top.EXT_PIN_MD3_D.set(RODU);
    top.EXT_PIN_MD4_D.set(RUBE);
    top.EXT_PIN_MD5_D.set(RUMU);
    top.EXT_PIN_MD6_D.set(RYTY);
    top.EXT_PIN_MD7_D.set(RADY);

    /*p25.TEME*/ top.VRM_TRI_D0.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D0);
    /*p25.TEWU*/ top.VRM_TRI_D1.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D1);
    /*p25.TYGO*/ top.VRM_TRI_D2.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D2);
    /*p25.SOTE*/ top.VRM_TRI_D3.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D3);
    /*p25.SEKE*/ top.VRM_TRI_D4.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D4);
    /*p25.RUJO*/ top.VRM_TRI_D5.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D5);
    /*p25.TOFA*/ top.VRM_TRI_D6.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D6);
    /*p25.SUZA*/ top.VRM_TRI_D7.set_tribuf(RAHU_VBUS_TRISTATEp, top.CPU_TRI_D7); // 10-rung

    /*p25.RELA*/ wire RELA_MD_OEp = or (REVO_VRAM_RDp, SAZO_VRAM_RD);
    /*p25.RENA*/ wire RENA_MD_OEn = not(RELA_MD_OEp);
    /*p25.ROFA*/ wire ROFA_MD_OEp = not(RENA_MD_OEn);

    /*p25.ROFA*/ top.EXT_PIN_MD0_B.set(ROFA_MD_OEp);
    /*p25.ROFA*/ top.EXT_PIN_MD1_B.set(ROFA_MD_OEp);
    /*p25.ROFA*/ top.EXT_PIN_MD2_B.set(ROFA_MD_OEp);
    /*p25.ROFA*/ top.EXT_PIN_MD3_B.set(ROFA_MD_OEp);
    /*p25.ROFA*/ top.EXT_PIN_MD4_B.set(ROFA_MD_OEp);
    /*p25.ROFA*/ top.EXT_PIN_MD5_B.set(ROFA_MD_OEp);
    /*p25.ROFA*/ top.EXT_PIN_MD6_B.set(ROFA_MD_OEp);
    /*p25.ROFA*/ top.EXT_PIN_MD7_B.set(ROFA_MD_OEp);

    /*p25.RODY*/ top.VRM_TRI_D0.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD0_C);
    /*p25.REBA*/ top.VRM_TRI_D1.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD1_C);
    /*p25.RYDO*/ top.VRM_TRI_D2.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD2_C);
    /*p25.REMO*/ top.VRM_TRI_D3.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD3_C);
    /*p25.ROCE*/ top.VRM_TRI_D4.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD4_C);
    /*p25.ROPU*/ top.VRM_TRI_D5.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD5_C);
    /*p25.RETA*/ top.VRM_TRI_D6.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD6_C);
    /*p25.RAKU*/ top.VRM_TRI_D7.set_tribuf(RENA_MD_OEn, top.EXT_PIN_MD7_C);
  }

  // Die trace
  // MYMA = not(LONY);
  // APAM = not(LUFA);
  // RACU = and(RYLU, RAWA, MYMA, APAM);

  {
    auto ppu_sig = top.ppu_reg.sig(top);
    
    /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!top.AFUR_xBCDExxx());
    /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(top.ADYK_xxxxEFGH());
    /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(top.CPU_PIN_WR, AFAS_xxxxxFGH);
    /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH = not(AREV_CPU_WRn_ABCDExxx);

    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09);
    /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp);
    /*p08.SORE*/ wire SORE_0000_7FFFp = not(top.CPU_PIN_A15);
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(top.CPU_PIN_A13, top.CPU_PIN_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (top.CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
    /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor(SYRO_FE00_FFFFp, TEXO_8000_9FFFn);
    /*p25.SOSE*/ wire SOSE_8000_9FFFp = and (top.CPU_PIN_A15, TEFA_8000_9FFFp);
    /*p25.TUJA*/ wire TUJA_CPU_VRAM_WR = and(SOSE_8000_9FFFp, APOV_CPU_WR_xxxxxFGH);
    /*p25.SUDO*/ wire SUDO_MWR_Cn = not(top.EXT_PIN_MWRn_C);
    /*p25.TYJY*/ wire TYJY_DBG_VRAM_WR = mux2_p(SUDO_MWR_Cn, TUJA_CPU_VRAM_WR, top.TUTO_DBG_VRAMp());
    /*p25.SOHY*/ wire SOHY_MWR    = nand(TYJY_DBG_VRAM_WR, top.SERE_VRAM_RD());

    /*p25.RACO*/ wire RACO_DBG_VRAMn = not(top.TUTO_DBG_VRAMp());
    /*p25.TAXY*/ wire TAXY_MWR_An = and(SOHY_MWR, RACO_DBG_VRAMn);
    /*p25.SOFY*/ wire SOFY_MWR_Dn = or (SOHY_MWR, top.TUTO_DBG_VRAMp());
    /*p25.SYSY*/ top.EXT_PIN_MWRn_A.set(not(TAXY_MWR_An));
    /*p25.RAGU*/ top.EXT_PIN_MWRn_D.set(not(SOFY_MWR_Dn));
  }

  {
    auto tile_fetcher_sig = top.tile_fetcher.sig(top);
    auto ppu_sig = top.ppu_reg.sig(top);
    

    /*p25.TAVY*/ wire TAVY_MOE_Cn = not(top.EXT_PIN_MOEn_C);

    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09);
    /*p25.SYRO*/ wire SYRO_FE00_FFFFp = not(TUNA_0000_FDFFp);
    /*p08.SORE*/ wire SORE_0000_7FFFp = not(top.CPU_PIN_A15);
    /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(top.CPU_PIN_A13, top.CPU_PIN_A14, SORE_0000_7FFFp);
    /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (top.CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
    /*p25.TEFA*/ wire TEFA_8000_9FFFp = nor(SYRO_FE00_FFFFp, TEXO_8000_9FFFn);
    /*p25.SOSE*/ wire SOSE_8000_9FFFp = and (top.CPU_PIN_A15, TEFA_8000_9FFFp);
    /*p25.TEGU*/ wire TEGU_CPU_VRAM_WRn = nand(SOSE_8000_9FFFp, top.CPU_PIN_WR); // Schematic wrong, second input is CPU_RAW_WR
    /*p25.SALE*/ wire SALE_VRAM_WRn = mux2_p(TAVY_MOE_Cn, TEGU_CPU_VRAM_WRn, top.TUTO_DBG_VRAMp());
    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn   = nand(top.MATU_DMA_RUNNINGp(), top.MUDA_DMA_SRC_VRAMp());
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);      /*p25.XANE*/ wire XANE_VRAM_LOCKn = nor(LUFA_DMA_READ_VRAMp, top.XYMU_RENDERINGp()); // def nor
    /*p25.RYLU*/ wire RYLU_DBG_VRAM_RDn = nand(SALE_VRAM_WRn, XANE_VRAM_LOCKn);
    /*p25.RAWA*/ wire RAWA_SPR_VRAM_RDn = not(top.SOHO_SPR_VRAM_RDp());
    /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not(tile_fetcher_sig.LONY_BG_READ_VRAM_LATCHp); // this should be correct
    /*p25.APAM*/ wire APAM_DMA_VRAM_RDn = not(LUFA_DMA_READ_VRAMp);
    /*p25.RACU*/ wire RACU_MOEn   = and (RYLU_DBG_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAM_RDn); // def and
    /*p25.RACO*/ wire RACO_DBG_VRAMn = not(top.TUTO_DBG_VRAMp());
    /*p25.SEMA*/ wire SEMA_MOE_An = and(RACU_MOEn, RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire RUTE_MOE_Dn = or (RACU_MOEn, top.TUTO_DBG_VRAMp()); // schematic wrong, second input is RACU
    /*p25.REFO*/ top.EXT_PIN_MOEn_A.set(not(SEMA_MOE_An));
    /*p25.SAHA*/ top.EXT_PIN_MOEn_D.set(not(RUTE_MOE_Dn));
  }

  {
    auto tile_fetcher_sig = top.tile_fetcher.sig(top);
    auto ppu_sig = top.ppu_reg.sig(top);
    

    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn   = nand(top.MATU_DMA_RUNNINGp(), top.MUDA_DMA_SRC_VRAMp());
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);
    /*p29.ABON*/ wire ABON_SPR_VRAM_RDp1 = not(top.TEXY_SPRITE_READp());
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(tile_fetcher_sig.LONY_BG_READ_VRAM_LATCHp);
    /*p27.LENA*/ wire LENA_BGW_VRAM_RD = not(LUSU_BGW_VRAM_RDn);
    /*p25.SUTU*/ wire SUTU_MCSn = nor(LENA_BGW_VRAM_RD, LUFA_DMA_READ_VRAMp, ABON_SPR_VRAM_RDp1, top.SERE_VRAM_RD());
    /*p25.RACO*/ wire RACO_DBG_VRAMn = not(top.TUTO_DBG_VRAMp());
    /*p25.TODE*/ wire TODE_MCS_An = and(SUTU_MCSn, RACO_DBG_VRAMn);
    /*p25.SEWO*/ wire SEWO_MCS_Dn = or (SUTU_MCSn, top.TUTO_DBG_VRAMp());
    /*p25.SOKY*/ top.EXT_PIN_MCSn_A.set(not(TODE_MCS_An));
    /*p25.SETY*/ top.EXT_PIN_MCSn_D.set(not(SEWO_MCS_Dn));
  }
}

#if 0
void dump(TextPainter& text_painter) {
  text_painter.dprintf("----- VRAM_PINS -----\n");

  text_painter.dprintf("MCS  %d:x:%d:%d\n", EXT_PIN_MCSn_A.prev().val, EXT_PIN_MCSn_C.prev().val, EXT_PIN_MCSn_D.prev().val);
  text_painter.dprintf("MOE  %d:x:%d:%d\n", EXT_PIN_MOEn_A.prev().val, EXT_PIN_MOEn_C.prev().val, EXT_PIN_MOEn_D.prev().val);
  text_painter.dprintf("MWR  %d:x:%d:%d\n", EXT_PIN_MWRn_A.prev().val, EXT_PIN_MWRn_C.prev().val, EXT_PIN_MWRn_D.prev().val);
  text_painter.dprintf("MAxx 0x%04x\n", pack(EXT_PIN_MA00_AD, EXT_PIN_MA01_AD, EXT_PIN_MA02_AD, EXT_PIN_MA03_AD, EXT_PIN_MA04_AD, EXT_PIN_MA05_AD, EXT_PIN_MA06_AD,
    EXT_PIN_MA07_AD, EXT_PIN_MA08_AD, EXT_PIN_MA09_AD, EXT_PIN_MA10_AD, EXT_PIN_MA11_AD, EXT_PIN_MA12_AD));

  text_painter.dprintf("MDx_A 0x%02x\n", pack(EXT_PIN_MD0_A, EXT_PIN_MD1_A, EXT_PIN_MD2_A, EXT_PIN_MD3_A, EXT_PIN_MD4_A, EXT_PIN_MD5_A, EXT_PIN_MD6_A, EXT_PIN_MD7_A));
  text_painter.dprintf("MDx_B 0x%02x\n", pack(EXT_PIN_MD0_B, EXT_PIN_MD1_B, EXT_PIN_MD2_B, EXT_PIN_MD3_B, EXT_PIN_MD4_B, EXT_PIN_MD5_B, EXT_PIN_MD6_B, EXT_PIN_MD7_B));
  text_painter.dprintf("MDx_C 0x%02x\n", pack(EXT_PIN_MD0_C, EXT_PIN_MD1_C, EXT_PIN_MD2_C, EXT_PIN_MD3_C, EXT_PIN_MD4_C, EXT_PIN_MD5_C, EXT_PIN_MD6_C, EXT_PIN_MD7_C));
  text_painter.dprintf("MDx_D 0x%02x\n", pack(EXT_PIN_MD0_D, EXT_PIN_MD1_D, EXT_PIN_MD2_D, EXT_PIN_MD3_D, EXT_PIN_MD4_D, EXT_PIN_MD5_D, EXT_PIN_MD6_D, EXT_PIN_MD7_D));
  text_painter.newline();
}
#endif