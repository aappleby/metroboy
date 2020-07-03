#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void SchematicTop::tick_everything() {
  clk_reg.tick(*this);
  dbg_reg.tick(*this);
  rst_reg.tick(*this);
  tim_reg.tick(*this);


  CPU_PIN_BOOTp.set(TUTU_BOOTp());


  /*
  dma_reg.tick(*this);
  tim_reg.tick(*this);
  ser_reg.tick(*this);

  joy_reg.tick(rst_sig, clk_reg, cpu_bus, cpu_sig);

  ppu_reg.tick(*this);
  sst_reg.tick(*this);
  lcd_reg.tick(*this);
  pxp_reg.tick(*this);
  //cpu_sig.tick(*this);
  cpu_pins_out.tick(*this);
  vram_pins.tick(*this);
  */

  // FF40 LCDC
  {
    /*p22.XOLA*/ wire XOLA_A00n = not(CPU_PIN_A00);
    /*p22.XENO*/ wire XENO_A01n = not(CPU_PIN_A01);
    /*p22.XUSY*/ wire XUSY_A02n = not(CPU_PIN_A02);
    /*p22.XERA*/ wire XERA_A03n = not(CPU_PIN_A03);
    /*p22.WORU*/ wire WORU_FF40n = nand(WERO_FF40_FF4Fp(), XOLA_A00n, XENO_A01n, XUSY_A02n, XERA_A03n);
    /*p22.VOCA*/ wire VOCA_FF40p = not(WORU_FF40n);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.VYRE*/ wire VYRE_FF40_RDp = and (VOCA_FF40p, ASOT_CPU_RD);
    /*p23.WYCE*/ wire WYCE_FF40_RDn = not(VYRE_FF40_RDp);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.WARU*/ wire WARU_FF40_WRp = and (VOCA_FF40p, CUPA_CPU_WR_xxxxxFGH);
    /*p23.XUBO*/ wire XUBO_FF40_WRn = not(WARU_FF40_WRp);

    /*p23.WYPO*/ CPU_TRI_D0.set_tribuf(WYCE_FF40_RDn, VYXE_LCDC_BGEN);
    /*p23.XERO*/ CPU_TRI_D1.set_tribuf(WYCE_FF40_RDn, XYLO_LCDC_SPEN);
    /*p23.WYJU*/ CPU_TRI_D2.set_tribuf(WYCE_FF40_RDn, XYMO_LCDC_SPSIZE);
    /*p23.WUKA*/ CPU_TRI_D3.set_tribuf(WYCE_FF40_RDn, XAFO_LCDC_BGMAP);
    /*p23.VOKE*/ CPU_TRI_D4.set_tribuf(WYCE_FF40_RDn, WEXU_LCDC_BGTILE);
    /*p23.VATO*/ CPU_TRI_D5.set_tribuf(WYCE_FF40_RDn, WYMO_LCDC_WINEN);
    /*p23.VAHA*/ CPU_TRI_D6.set_tribuf(WYCE_FF40_RDn, WOKY_LCDC_WINMAP);
    /*p23.XEBU*/ CPU_TRI_D7.set_tribuf(WYCE_FF40_RDn, XONA_LCDC_EN);

    /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.XARE*/ wire XARE_RSTn = not(XORE_RSTp);
    /*p23.VYXE*/ VYXE_LCDC_BGEN   .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D0);
    /*p23.XYLO*/ XYLO_LCDC_SPEN   .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D1);
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D2);
    /*p23.XAFO*/ XAFO_LCDC_BGMAP  .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D3);
    /*p23.WEXU*/ WEXU_LCDC_BGTILE .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D4);
    /*p23.WYMO*/ WYMO_LCDC_WINEN  .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D5);
    /*p23.WOKY*/ WOKY_LCDC_WINMAP .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D6);
    /*p23.XONA*/ XONA_LCDC_EN     .set(XUBO_FF40_WRn, XARE_RSTn, CPU_TRI_D7);
  }
}

//---------------------------------------------------------------------------

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

void SchematicTop::tick_vram_pins(SchematicTop& top) {

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
    /*p27.MYMA*/ wire MYMA_BGW_VRAM_RDn = not(top.LONY_BG_READ_VRAM_LATCHp()); // this should be correct
    /*p25.APAM*/ wire APAM_DMA_VRAM_RDn = not(LUFA_DMA_READ_VRAMp);
    /*p25.RACU*/ wire RACU_MOEn   = and (RYLU_DBG_VRAM_RDn, RAWA_SPR_VRAM_RDn, MYMA_BGW_VRAM_RDn, APAM_DMA_VRAM_RDn); // def and
    /*p25.RACO*/ wire RACO_DBG_VRAMn = not(top.TUTO_DBG_VRAMp());
    /*p25.SEMA*/ wire SEMA_MOE_An = and(RACU_MOEn, RACO_DBG_VRAMn);
    /*p25.RUTE*/ wire RUTE_MOE_Dn = or (RACU_MOEn, top.TUTO_DBG_VRAMp()); // schematic wrong, second input is RACU
    /*p25.REFO*/ top.EXT_PIN_MOEn_A.set(not(SEMA_MOE_An));
    /*p25.SAHA*/ top.EXT_PIN_MOEn_D.set(not(RUTE_MOE_Dn));
  }

  {
    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn   = nand(top.MATU_DMA_RUNNINGp(), top.MUDA_DMA_SRC_VRAMp());
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);
    /*p29.ABON*/ wire ABON_SPR_VRAM_RDp1 = not(top.TEXY_SPRITE_READp());
    /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(top.LONY_BG_READ_VRAM_LATCHp());
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
//-----------------------------------------------------------------------------

SignalHash SchematicTop::commit_everything() {
  SignalHash hash;

  hash << VYXE_LCDC_BGEN.commit_reg();
  hash << XYLO_LCDC_SPEN.commit_reg();
  hash << XYMO_LCDC_SPSIZE.commit_reg();
  hash << XAFO_LCDC_BGMAP.commit_reg();
  hash << WEXU_LCDC_BGTILE.commit_reg();
  hash << WYMO_LCDC_WINEN.commit_reg();
  hash << WOKY_LCDC_WINMAP.commit_reg();
  hash << XONA_LCDC_EN.commit_reg();

  /* PIN_50 */ hash << LCD_PIN_LD1.commit_pinout();
  /* PIN_51 */ hash << LCD_PIN_LD0.commit_pinout();
  /* PIN_52 */ hash << LCD_PIN_CPG.commit_pinout();
  /* PIN_53 */ hash << LCD_PIN_CP.commit_pinout();
  /* PIN_54 */ hash << LCD_PIN_ST.commit_pinout();
  /* PIN_55 */ hash << LCD_PIN_CPL.commit_pinout();
  /* PIN_56 */ hash << LCD_PIN_FR.commit_pinout();
  /* PIN_57 */ hash << LCD_PIN_S.commit_pinout();

  /* PIN_01 */ hash << EXT_PIN_A00_C.clear_preset();      // -> KOVA
  /* PIN_02 */ hash << EXT_PIN_A01_C.clear_preset();      // -> CAMU
  /* PIN_03 */ hash << EXT_PIN_A02_C.clear_preset();      // -> BUXU
  /* PIN_04 */ hash << EXT_PIN_A03_C.clear_preset();      // -> BASE
  /* PIN_05 */ hash << EXT_PIN_A04_C.clear_preset();      // -> AFEC
  /* PIN_06 */ hash << EXT_PIN_A05_C.clear_preset();      // -> ABUP
  /* PIN_07 */ hash << EXT_PIN_A06_C.clear_preset();      // -> CYGU
  /* PIN_08 */ hash << EXT_PIN_A07_C.clear_preset();      // -> COGO
  /* PIN_09 */ hash << EXT_PIN_A08_C.clear_preset();      // -> MUJY
  /* PIN_10 */ hash << EXT_PIN_A09_C.clear_preset();      // -> NENA
  /* PIN_11 */ hash << EXT_PIN_A10_C.clear_preset();      // -> SURA
  /* PIN_12 */ hash << EXT_PIN_A11_C.clear_preset();      // -> MADY
  /* PIN_13 */ hash << EXT_PIN_A12_C.clear_preset();      // -> LAHE
  /* PIN_14 */ hash << EXT_PIN_A13_C.clear_preset();      // -> LURA
  /* PIN_15 */ hash << EXT_PIN_A14_C.clear_preset();      // -> PEVO
  /* PIN_16 */ hash << EXT_PIN_A15_C.clear_preset();      // -> RAZA
  /* PIN_71 */ hash << SYS_PIN_RST.clear_preset();
  /* PIN_74 */ hash << SYS_PIN_CLK_GOOD.clear_preset();
  /* PIN_76 */ hash << SYS_PIN_T2.clear_preset();
  /* PIN_77 */ hash << SYS_PIN_T1.clear_preset();
  /* PIN_78 */ hash << EXT_PIN_WRn_C.clear_preset();     // -> UBAL
  /* PIN_79 */ hash << EXT_PIN_RDn_C.clear_preset();     // -> UJYV

  /* PIN_25 */ hash << EXT_PIN_MD7_A.commit_pinout();    // <- RYZE
  /* PIN_25 */ hash << EXT_PIN_MD7_B.commit_pinout();    // <- ROFA
  /* PIN_25 */ hash << EXT_PIN_MD7_C.clear_preset();     // -> RAKU
  /* PIN_25 */ hash << EXT_PIN_MD7_D.commit_pinout();    // <- RADY
  /* PIN_26 */ hash << EXT_PIN_MD6_A.commit_pinout();    // <- REKU
  /* PIN_26 */ hash << EXT_PIN_MD6_B.commit_pinout();    // <- ROFA
  /* PIN_26 */ hash << EXT_PIN_MD6_C.clear_preset();     // -> RETA
  /* PIN_26 */ hash << EXT_PIN_MD6_D.commit_pinout();    // <- RYTY
  /* PIN_27 */ hash << EXT_PIN_MD5_A.commit_pinout();    // <- REVU
  /* PIN_27 */ hash << EXT_PIN_MD5_B.commit_pinout();    // <- ROFA
  /* PIN_27 */ hash << EXT_PIN_MD5_C.clear_preset();     // -> ROPU
  /* PIN_27 */ hash << EXT_PIN_MD5_D.commit_pinout();    // <- RUMU
  /* PIN_28 */ hash << EXT_PIN_MD4_A.commit_pinout();    // <- RYRO
  /* PIN_28 */ hash << EXT_PIN_MD4_B.commit_pinout();    // <- ROFA
  /* PIN_28 */ hash << EXT_PIN_MD4_C.clear_preset();     // -> ROCE
  /* PIN_28 */ hash << EXT_PIN_MD4_D.commit_pinout();    // <- RUBE
  /* PIN_29 */ hash << EXT_PIN_MD3_A.commit_pinout();    // <- RADA
  /* PIN_29 */ hash << EXT_PIN_MD3_B.commit_pinout();    // <- ROFA
  /* PIN_29 */ hash << EXT_PIN_MD3_C.clear_preset();     // -> REMO
  /* PIN_29 */ hash << EXT_PIN_MD3_D.commit_pinout();    // <- RODU
  /* PIN_30 */ hash << EXT_PIN_MD2_A.commit_pinout();    // <- RAZO
  /* PIN_30 */ hash << EXT_PIN_MD2_B.commit_pinout();    // <- ROFA
  /* PIN_30 */ hash << EXT_PIN_MD2_C.clear_preset();     // -> RYDO
  /* PIN_30 */ hash << EXT_PIN_MD2_D.commit_pinout();    // <- RARE
  /* PIN_31 */ hash << EXT_PIN_MD1_A.commit_pinout();    // <- RYKY
  /* PIN_31 */ hash << EXT_PIN_MD1_B.commit_pinout();    // <- ROFA
  /* PIN_31 */ hash << EXT_PIN_MD1_C.clear_preset();     // -> REBA
  /* PIN_31 */ hash << EXT_PIN_MD1_D.commit_pinout();    // <- RULY
                                                         /* PIN_32 */ /*GND*/
  /* PIN_33 */ hash << EXT_PIN_MD0_A.commit_pinout();    // <- REGE
  /* PIN_33 */ hash << EXT_PIN_MD0_B.commit_pinout();    // <- ROFA
  /* PIN_33 */ hash << EXT_PIN_MD0_C.clear_preset();     // -> RODY
  /* PIN_33 */ hash << EXT_PIN_MD0_D.commit_pinout();    // <- RURA
  /* PIN_34 */ hash << EXT_PIN_MA00_AD.commit_pinout();  // <- ECAL
  /* PIN_35 */ hash << EXT_PIN_MA01_AD.commit_pinout();  // <- EGEZ
  /* PIN_36 */ hash << EXT_PIN_MA02_AD.commit_pinout();  // <- FUHE
  /* PIN_37 */ hash << EXT_PIN_MA03_AD.commit_pinout();  // <- FYZY
  /* PIN_38 */ hash << EXT_PIN_MA04_AD.commit_pinout();  // <- DAMU
  /* PIN_39 */ hash << EXT_PIN_MA05_AD.commit_pinout();  // <- DAVA
  /* PIN_40 */ hash << EXT_PIN_MA06_AD.commit_pinout();  // <- ETEG
  /* PIN_41 */ hash << EXT_PIN_MA07_AD.commit_pinout();  // <- EREW
  /* PIN_42 */ hash << EXT_PIN_MA12_AD.commit_pinout();  // <- EXYF
  /* PIN_43 */ hash << EXT_PIN_MCSn_A.commit_pinout();   // <- SOKY
  /* PIN_43 */ hash << EXT_PIN_MCSn_C.clear_preset();     // -> TEFY
  /* PIN_43 */ hash << EXT_PIN_MCSn_D.commit_pinout();   // <- SETY
  /* PIN_44 */ hash << EXT_PIN_MA10_AD.commit_pinout();  // <- ERAF
  /* PIN_45 */ hash << EXT_PIN_MOEn_A.commit_pinout();   // <- REFO
  /* PIN_45 */ hash << EXT_PIN_MOEn_C.clear_preset();     // -> TAVY
  /* PIN_45 */ hash << EXT_PIN_MOEn_D.commit_pinout();   // <- SAHA
  /* PIN_46 */ hash << EXT_PIN_MA11_AD.commit_pinout();  // <- FUSY
  /* PIN_47 */ hash << EXT_PIN_MA09_AD.commit_pinout();  // <- DUVE
  /* PIN_48 */ hash << EXT_PIN_MA08_AD.commit_pinout();  // <- EVAX
  /* PIN_49 */ hash << EXT_PIN_MWRn_A.commit_pinout();   // <- SYSY
  /* PIN_49 */ hash << EXT_PIN_MWRn_C.clear_preset();     // -> SUDO
  /* PIN_49 */ hash << EXT_PIN_MWRn_D.commit_pinout();   // <- RAGU

  /* PIN_17 */ hash << EXT_PIN_D0_A.commit_pinout();      // <- RUXA
  /* PIN_17 */ hash << EXT_PIN_D0_B.commit_pinout();      // <- LULA
  /* PIN_17 */ hash << EXT_PIN_D0_D.commit_pinout();      // <- RUNE
  /* PIN_18 */ hash << EXT_PIN_D1_A.commit_pinout();      // <- RUJA
  /* PIN_18 */ hash << EXT_PIN_D1_B.commit_pinout();      // <- LULA
  /* PIN_18 */ hash << EXT_PIN_D1_D.commit_pinout();      // <- RYPU
  /* PIN_19 */ hash << EXT_PIN_D2_A.commit_pinout();      // <- RABY
  /* PIN_19 */ hash << EXT_PIN_D2_B.commit_pinout();      // <- LULA
  /* PIN_19 */ hash << EXT_PIN_D2_D.commit_pinout();      // <- SULY
  /* PIN_20 */ hash << EXT_PIN_D3_A.commit_pinout();      // <- RERA
  /* PIN_20 */ hash << EXT_PIN_D3_B.commit_pinout();      // <- LULA
  /* PIN_20 */ hash << EXT_PIN_D3_D.commit_pinout();      // <- SEZE
  /* PIN_21 */ hash << EXT_PIN_D4_A.commit_pinout();      // <- RORY
  /* PIN_21 */ hash << EXT_PIN_D4_B.commit_pinout();      // <- LULA
  /* PIN_21 */ hash << EXT_PIN_D4_D.commit_pinout();      // <- RESY
  /* PIN_22 */ hash << EXT_PIN_D5_A.commit_pinout();      // <- RYVO
  /* PIN_22 */ hash << EXT_PIN_D5_B.commit_pinout();      // <- LULA
  /* PIN_22 */ hash << EXT_PIN_D5_D.commit_pinout();      // <- TAMU
  /* PIN_23 */ hash << EXT_PIN_D6_A.commit_pinout();      // <- RAFY
  /* PIN_23 */ hash << EXT_PIN_D6_B.commit_pinout();      // <- LULA
  /* PIN_23 */ hash << EXT_PIN_D6_D.commit_pinout();      // <- ROGY
  /* PIN_24 */ hash << EXT_PIN_D7_A.commit_pinout();      // <- RAVU
  /* PIN_24 */ hash << EXT_PIN_D7_B.commit_pinout();      // <- LULA
  /* PIN_24 */ hash << EXT_PIN_D7_D.commit_pinout();      // <- RYDA

  hash << OAM_PIN_CLK.commit_pinout();
  hash << OAM_PIN_OE.commit_pinout();
  hash << OAM_PIN_WR_A.commit_pinout(); // definitely write
  hash << OAM_PIN_WR_B.commit_pinout(); // definitely write
  hash << OAM_PIN_A0.commit_pinout();
  hash << OAM_PIN_A1.commit_pinout();
  hash << OAM_PIN_A2.commit_pinout();
  hash << OAM_PIN_A3.commit_pinout();
  hash << OAM_PIN_A4.commit_pinout();
  hash << OAM_PIN_A5.commit_pinout();
  hash << OAM_PIN_A6.commit_pinout();
  hash << OAM_PIN_A7.commit_pinout();
  hash << OAM_PIN_DA0.commit_tribuf();
  hash << OAM_PIN_DA1.commit_tribuf();
  hash << OAM_PIN_DA2.commit_tribuf();
  hash << OAM_PIN_DA3.commit_tribuf();
  hash << OAM_PIN_DA4.commit_tribuf();
  hash << OAM_PIN_DA5.commit_tribuf();
  hash << OAM_PIN_DA6.commit_tribuf();
  hash << OAM_PIN_DA7.commit_tribuf();
  hash << OAM_PIN_DB0.commit_tribuf();
  hash << OAM_PIN_DB1.commit_tribuf();
  hash << OAM_PIN_DB2.commit_tribuf();
  hash << OAM_PIN_DB3.commit_tribuf();
  hash << OAM_PIN_DB4.commit_tribuf();
  hash << OAM_PIN_DB5.commit_tribuf();
  hash << OAM_PIN_DB6.commit_tribuf();
  hash << OAM_PIN_DB7.commit_tribuf();

  /* PIN_78 */ hash << EXT_PIN_WRn_A.commit_pinout();    // <- UVER
  /* PIN_78 */ hash << EXT_PIN_WRn_D.commit_pinout();    // <- USUF
  /* PIN_79 */ hash << EXT_PIN_RDn_A.commit_pinout();    // <- UGAC
  /* PIN_79 */ hash << EXT_PIN_RDn_D.commit_pinout();    // <- URUN
  /* PIN_80 */ hash << EXT_PIN_CSn_A.commit_pinout();    // <- TYHO

  /* PIN_01 */ hash << EXT_PIN_A00_A.commit_pinout();     // <- KUPO
  /* PIN_01 */ hash << EXT_PIN_A00_D.commit_pinout();     // <- KOTY
  /* PIN_02 */ hash << EXT_PIN_A01_A.commit_pinout();     // <- CABA
  /* PIN_02 */ hash << EXT_PIN_A01_D.commit_pinout();     // <- COTU
  /* PIN_03 */ hash << EXT_PIN_A02_A.commit_pinout();     // <- BOKU
  /* PIN_03 */ hash << EXT_PIN_A02_D.commit_pinout();     // <- BAJO
  /* PIN_04 */ hash << EXT_PIN_A03_A.commit_pinout();     // <- BOTY
  /* PIN_04 */ hash << EXT_PIN_A03_D.commit_pinout();     // <- BOLA
  /* PIN_05 */ hash << EXT_PIN_A04_A.commit_pinout();     // <- BYLA
  /* PIN_05 */ hash << EXT_PIN_A04_D.commit_pinout();     // <- BEVO
  /* PIN_06 */ hash << EXT_PIN_A05_A.commit_pinout();     // <- BADU
  /* PIN_06 */ hash << EXT_PIN_A05_D.commit_pinout();     // <- AJAV
  /* PIN_07 */ hash << EXT_PIN_A06_A.commit_pinout();     // <- CEPU
  /* PIN_07 */ hash << EXT_PIN_A06_D.commit_pinout();     // <- CYKA
  /* PIN_08 */ hash << EXT_PIN_A07_A.commit_pinout();     // <- DEFY
  /* PIN_08 */ hash << EXT_PIN_A07_D.commit_pinout();     // <- COLO
  /* PIN_09 */ hash << EXT_PIN_A08_A.commit_pinout();     // <- MYNY
  /* PIN_09 */ hash << EXT_PIN_A08_D.commit_pinout();     // <- MEGO
  /* PIN_10 */ hash << EXT_PIN_A09_A.commit_pinout();     // <- MUNE
  /* PIN_10 */ hash << EXT_PIN_A09_D.commit_pinout();     // <- MENY
  /* PIN_11 */ hash << EXT_PIN_A10_A.commit_pinout();     // <- ROXU
  /* PIN_11 */ hash << EXT_PIN_A10_D.commit_pinout();     // <- RORE
  /* PIN_12 */ hash << EXT_PIN_A11_A.commit_pinout();     // <- LEPY
  /* PIN_12 */ hash << EXT_PIN_A11_D.commit_pinout();     // <- LYNY
  /* PIN_13 */ hash << EXT_PIN_A12_A.commit_pinout();     // <- LUCE
  /* PIN_13 */ hash << EXT_PIN_A12_D.commit_pinout();     // <- LOSO
  /* PIN_14 */ hash << EXT_PIN_A13_A.commit_pinout();     // <- LABE
  /* PIN_14 */ hash << EXT_PIN_A13_D.commit_pinout();     // <- LEVA
  /* PIN_15 */ hash << EXT_PIN_A14_A.commit_pinout();     // <- PUHE
  /* PIN_15 */ hash << EXT_PIN_A14_D.commit_pinout();     // <- PAHY
  /* PIN_16 */ hash << EXT_PIN_A15_A.commit_pinout();     // <- SUZE
  /* PIN_16 */ hash << EXT_PIN_A15_D.commit_pinout();     // <- RULO

  /* PIN_58 */ /*VCC*/
  /* PIN_59 */ /*ROUT*/
  /* PIN_60 */ /*LOUT*/
  /* PIN_61 */ /*VIN*/

  /* PIN_62 */ hash << EXT_P15_A.commit_pinout();   // <- CELA
  /* PIN_62 */ hash << EXT_P15_D.commit_pinout();   // <- COFY
  /* PIN_63 */ hash << EXT_P14_A.commit_pinout();   // <- KARU
  /* PIN_63 */ hash << EXT_P14_D.commit_pinout();   // <- KELY
  /* PIN_64 */ hash << EXT_P13_A.commit_pinout();   // <- KORY
  /* PIN_64 */ hash << EXT_P13_B.commit_pinout();   
  /* PIN_64 */ hash << EXT_P13_C.clear_preset();    // -> KERY, P05.KOLO
  /* PIN_64 */ hash << EXT_P13_D.commit_pinout();   // <- KALE
  /* PIN_65 */ hash << EXT_P12_A.commit_pinout();   // <- KYHU
  /* PIN_65 */ hash << EXT_P12_B.commit_pinout();   
  /* PIN_65 */ hash << EXT_P12_C.clear_preset();    // -> KERY, P05.KEJA
  /* PIN_65 */ hash << EXT_P12_D.commit_pinout();   // <- KASY
  /* PIN_66 */ hash << EXT_P11_A.commit_pinout();   // <- KYTO
  /* PIN_66 */ hash << EXT_P11_B.commit_pinout();   
  /* PIN_66 */ hash << EXT_P11_C.clear_preset();    // -> KERY, P05.KAPA
  /* PIN_66 */ hash << EXT_P11_D.commit_pinout();   // <- KABU
  /* PIN_67 */ hash << EXT_P10_A.commit_pinout();   // <- KOLE
  /* PIN_67 */ hash << EXT_P10_B.commit_pinout();   
  /* PIN_67 */ hash << EXT_P10_C.clear_preset();    // -> KERY, KEVU
  /* PIN_67 */ hash << EXT_P10_D.commit_pinout();   // <- KYBU

  //----------

  hash << CPU_PIN_RD.clear_preset();             // PORTA_00: -> UJYV, LAGU, LAVO
  hash << CPU_PIN_WR.clear_preset();             // PORTA_01: -> AREV, LAGU.
  //hash << UNOR_MODE_DBG2.commit_pinout();      // PORTA_02: <- P07.UNOR_MODE_DBG2
  hash << CPU_PIN_SYRO.commit_pinout();          // PORTA_03  <- SYRO
  hash << CPU_PIN_BOOTp.commit_pinout();         // PORTA_04: <- TUTU
  //hash << UMUT_MODE_DBG1.commit_pinout();      // PORTA_05: <- P07.UMUT_MODE_DBG1
  hash << CPU_PIN_ADDR_VALID.clear_preset();     // PORTA_06: -> APAP, TEXO

  hash << CPU_PIN_ACK_VBLANK.clear_preset();     // PORTB_01: -> LETY, vblank int ack
  hash << CPU_PIN_INT_VBLANK.commit_pinout();    // PORTB_03: <- LOPE, vblank int
  hash << CPU_PIN_ACK_STAT.clear_preset();       // PORTB_05: -> LEJA, stat int ack
  hash << CPU_PIN_INT_STAT.commit_pinout();      // PORTB_07: <- LALU, stat int
  hash << CPU_PIN_ACK_TIMER.clear_preset();      // PORTB_09: -> LESA, timer int ack
  hash << CPU_PIN_INT_TIMER.commit_pinout();     // PORTB_11: <- NYBO, timer int
  hash << CPU_PIN_ACK_SERIAL.clear_preset();     // PORTB_13: -> LUFE, serial int ack
  hash << CPU_PIN_INT_SERIAL.commit_pinout();    // PORTB_15: <- UBUL, serial int
  hash << CPU_PIN_ACK_JOYPAD.clear_preset();     // PORTB_17: -> LAMO, joypad int ack
  hash << CPU_PIN_INT_JOYPAD.commit_pinout();    // PORTB_19: <- ULAK, joypad int

  hash << CPU_PIN_WAKE.commit_pinout();          // <- P02.AWOB

  hash << CPU_PIN_CLKREQ.clear_preset();         // PORTC_00: -> ABOL
  hash << CPU_PIN_AFER_RSTp.commit_pinout();     // PORTC_01: <- AFER
  hash << CPU_PIN_EXT_RESET.commit_pinout();     // PORTC_02: <- PIN_RESET directly connected to the pad
  hash << CPU_PIN_EXT_CLKGOOD.commit_pinout();   // PORTC_03: <- CLKIN_A
  hash << CPU_PIN_TABA_RSTp.commit_pinout();     // PORTC_04: <- TABA

  hash << CPU_PIN6.clear_preset();               // PORTD_00: -> LEXY, doesn't do anything
  hash << CPU_PIN_BOWA_AxCDEFGH.commit_pinout(); // PORTD_01: <- BOWA
  hash << CPU_PIN_BEDO_xBxxxxxx.commit_pinout(); // PORTD_02: <- BEDO _____fgh
  hash << CPU_PIN_BEKO_xBCDExxx.commit_pinout(); // PORTD_03: <- BEKO ____efgh connection not indicated on P01
  hash << CPU_PIN_BUDE_AxxxxFGH.commit_pinout(); // PORTD_04: <- BUDE abcd____
  hash << CPU_PIN_BOLO_xBCDEFGx.commit_pinout(); // PORTD_05: <- BOLO
  hash << CPU_PIN5.clear_preset();               // PORTD_06: -> FROM_CPU5
  hash << CPU_PIN_BUKE_ABxxxxxH.commit_pinout(); // PORTD_07: <- BUKE _____f__
  hash << CPU_PIN_BOMA_xBxxxxxx.commit_pinout(); // PORTD_08: <- BOMA _____fgh
  hash << CPU_PIN_BOGA_AxCDEFGH.commit_pinout(); // PORTD_09: <- BOGA abcde___


  hash << CPU_PIN_A00.clear_preset();
  hash << CPU_PIN_A01.clear_preset();
  hash << CPU_PIN_A02.clear_preset();
  hash << CPU_PIN_A03.clear_preset();
  hash << CPU_PIN_A04.clear_preset();
  hash << CPU_PIN_A05.clear_preset();
  hash << CPU_PIN_A06.clear_preset();
  hash << CPU_PIN_A07.clear_preset();
  hash << CPU_PIN_A08.clear_preset();
  hash << CPU_PIN_A09.clear_preset();
  hash << CPU_PIN_A10.clear_preset();
  hash << CPU_PIN_A11.clear_preset();
  hash << CPU_PIN_A12.clear_preset();
  hash << CPU_PIN_A13.clear_preset();
  hash << CPU_PIN_A14.clear_preset();
  hash << CPU_PIN_A15.clear_preset();

  hash << CPU_TRI_D0.commit_tribuf();
  hash << CPU_TRI_D1.commit_tribuf();
  hash << CPU_TRI_D2.commit_tribuf();
  hash << CPU_TRI_D3.commit_tribuf();
  hash << CPU_TRI_D4.commit_tribuf();
  hash << CPU_TRI_D5.commit_tribuf();
  hash << CPU_TRI_D6.commit_tribuf();
  hash << CPU_TRI_D7.commit_tribuf();

  //----------

  hash << VRM_TRI_A00.commit_tribuf();
  hash << VRM_TRI_A01.commit_tribuf();
  hash << VRM_TRI_A02.commit_tribuf();
  hash << VRM_TRI_A03.commit_tribuf();
  hash << VRM_TRI_A04.commit_tribuf();
  hash << VRM_TRI_A05.commit_tribuf();
  hash << VRM_TRI_A06.commit_tribuf();
  hash << VRM_TRI_A07.commit_tribuf();
  hash << VRM_TRI_A08.commit_tribuf();
  hash << VRM_TRI_A09.commit_tribuf();
  hash << VRM_TRI_A10.commit_tribuf();
  hash << VRM_TRI_A11.commit_tribuf();
  hash << VRM_TRI_A12.commit_tribuf();

  hash << VRM_TRI_D0.commit_tribuf();
  hash << VRM_TRI_D1.commit_tribuf();
  hash << VRM_TRI_D2.commit_tribuf();
  hash << VRM_TRI_D3.commit_tribuf();
  hash << VRM_TRI_D4.commit_tribuf();
  hash << VRM_TRI_D5.commit_tribuf();
  hash << VRM_TRI_D6.commit_tribuf();
  hash << VRM_TRI_D7.commit_tribuf();

  //----------
  // Tile fetcher regs

  hash << BG_PIX_A0.commit_reg();
  hash << BG_PIX_A1.commit_reg();
  hash << BG_PIX_A2.commit_reg();
  hash << BG_PIX_A3.commit_reg();
  hash << BG_PIX_A4.commit_reg();
  hash << BG_PIX_A5.commit_reg();
  hash << BG_PIX_A6.commit_reg();
  hash << BG_PIX_A7.commit_reg();

  hash << BG_PIX_B0.commit_reg();
  hash << BG_PIX_B1.commit_reg();
  hash << BG_PIX_B2.commit_reg(); 
  hash << BG_PIX_B3.commit_reg(); 
  hash << BG_PIX_B4.commit_reg(); 
  hash << BG_PIX_B5.commit_reg(); 
  hash << BG_PIX_B6.commit_reg(); 
  hash << BG_PIX_B7.commit_reg();

  //----------
  // Sprite store tribufs

  hash << SPR_TRI_IDX_0.commit_tribuf();
  hash << SPR_TRI_IDX_1.commit_tribuf();
  hash << SPR_TRI_IDX_2.commit_tribuf();
  hash << SPR_TRI_IDX_3.commit_tribuf();
  hash << SPR_TRI_IDX_4.commit_tribuf();
  hash << SPR_TRI_IDX_5.commit_tribuf();

  hash << SPR_TRI_LINE_0.commit_tribuf();
  hash << SPR_TRI_LINE_1.commit_tribuf();
  hash << SPR_TRI_LINE_2.commit_tribuf();
  hash << SPR_TRI_LINE_3.commit_tribuf();

  //----------

  hash << clk_reg.commit();
  hash << dbg_reg.commit();
  hash << rst_reg.commit();
  hash << tim_reg.commit();

#if 0
  hash << lcd_reg.commit();
  hash << pxp_reg.commit();
  hash << sst_reg.commit();
  hash << ppu_reg.commit();
  hash << win_reg.commit();
  hash << lcd_reg.commit();
  hash << ser_reg.commit();

  /* PIN_17 */ hash << EXT_PIN_D0_C.clear_preset();       // -> TOVO,SOMA
  /* PIN_18 */ hash << EXT_PIN_D1_C.clear_preset();       // -> RUZY,RONY
  /* PIN_19 */ hash << EXT_PIN_D2_C.clear_preset();       // -> ROME,RAXY
  /* PIN_20 */ hash << EXT_PIN_D3_C.clear_preset();       // -> SAZA,SELO
  /* PIN_21 */ hash << EXT_PIN_D4_C.clear_preset();       // -> TEHE,SODY
  /* PIN_22 */ hash << EXT_PIN_D5_C.clear_preset();       // -> RATU,SAGO
  /* PIN_23 */ hash << EXT_PIN_D6_C.clear_preset();       // -> SOCA,RUPA
  /* PIN_24 */ hash << EXT_PIN_D7_C.clear_preset();       // -> RYBA,SAZY
#endif

  return hash;
}

//-----------------------------------------------------------------------------
// Debug signals

wire SchematicTop::UMUT_MODE_DBG1p() const {
  /*p07.UVAR*/ wire UVAR_T2n = not(SYS_PIN_T2);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and (SYS_PIN_T1, UVAR_T2n);
  return UMUT_MODE_DBG1p;
}

wire SchematicTop::UNOR_MODE_DBG2p() const {
  /*p07.UBET*/ wire UBET_T1n = not(SYS_PIN_T1);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (SYS_PIN_T2, UBET_T1n); // Must be UNORp, see UJYV/UBAL
  return UNOR_MODE_DBG2p;
}

wire SchematicTop::UPOJ_MODE_PROD() const {
  /*p07.UBET*/ wire UBET_T1n = not(SYS_PIN_T1);
  /*p07.UVAR*/ wire UVAR_T2n = not(SYS_PIN_T2);
  /*p07.UPOJ*/ wire UPOJ_MODE_PROD = nand(UBET_T1n, UVAR_T2n, SYS_PIN_RST);
  return UPOJ_MODE_PROD;
}

wire SchematicTop::TOVA_MODE_DBG2n() const {
  /*p08.TOVA*/ wire TOVA_MODE_DBG2n = not(UNOR_MODE_DBG2p());
  return TOVA_MODE_DBG2n;
}

wire SchematicTop::TUTO_DBG_VRAMp() const {
  /*p25.TUTO*/ wire TUTO_DBG_VRAMp = and (UNOR_MODE_DBG2p(), !dbg_reg.SOTO_DBG);
  return TUTO_DBG_VRAMp;
}

// still not sure what this is...
wire SchematicTop::ABUZ() const {
  /*p??.APAP*/ wire APAP = not(CPU_PIN_ADDR_VALID); // Missing from schematic
  /*p01.AWOD*/ wire AWOD = nor(UNOR_MODE_DBG2p(), APAP);
  /*p01.ABUZ*/ wire ABUZ = not(AWOD);
  return ABUZ;
}

void SchematicTop::preset_t1t2(bool t1, bool t2) {
  SYS_PIN_T1.preset(true, t1);
  SYS_PIN_T2.preset(true, t2);
}

//-----------------------------------------------------------------------------
// Clock signals

wire SchematicTop::AFUR_xBCDExxx() const { return clk_reg.AFUR_xBCDExxx; }
wire SchematicTop::ALEF_xxCDEFxx() const { return clk_reg.ALEF_xxCDEFxx; }
wire SchematicTop::APUK_xxxDEFGx() const { return clk_reg.APUK_xxxDEFGx; }
wire SchematicTop::ADYK_xxxxEFGH() const { return clk_reg.ADYK_xxxxEFGH; }
wire SchematicTop::WUVU_AxxDExxH() const { return clk_reg.WUVU_AxxDExxH; }
wire SchematicTop::VENA_xBCDExxx() const { return clk_reg.VENA_xBCDExxx; }
wire SchematicTop::WOSU_xxCDxxGH() const { return clk_reg.WOSU_xxCDxxGH; }

wire SchematicTop::BYJU_AxCDEFGH() const {
  /*p01.ABOL*/ wire ABOL_CLKREQn  = not(CPU_PIN_CLKREQ);
  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx());
  /*p01.NULE*/ wire NULE_AxxxxFGH = nor(ABOL_CLKREQn,  ATYP_xBCDExxx);
  /*p01.AROV*/ wire AROV_xxxDEFGx = not(!APUK_xxxDEFGx());
  /*p01.BAPY*/ wire BAPY_AxxxxxxH = nor(ABOL_CLKREQn,  AROV_xxxDEFGx, ATYP_xBCDExxx);
  /*p01.AFEP*/ wire AFEP_ABxxxxGH = not(ALEF_xxCDEFxx());
  /*p01.BYRY*/ wire BYRY_xBCDExxx = not(NULE_AxxxxFGH);
  /*p01.BUDE*/ wire BUDE_AxxxxFGH = not(BYRY_xBCDExxx);
  /*p01.BERU*/ wire BERU_xBCDEFGx = not(BAPY_AxxxxxxH);
  /*p01.BUFA*/ wire BUFA_AxxxxxxH = not(BERU_xBCDEFGx);
  /*p01.BOLO*/ wire BOLO_xBCDEFGx = not(BUFA_AxxxxxxH);
  /*p01.BEKO*/ wire BEKO_xBCDExxx = not(BUDE_AxxxxFGH);
  /*p01.BEJA*/ wire BEJA_AxxxxFGH = nand(BOLO_xBCDEFGx, BEKO_xBCDExxx);
  /*p01.BANE*/ wire BANE_xBCDExxx = not(BEJA_AxxxxFGH);
  /*p01.BELO*/ wire BELO_AxxxxFGH = not(BANE_xBCDExxx);
  /*p01.BAZE*/ wire BAZE_xBCDExxx = not(BELO_AxxxxFGH);
  /*p01.BUTO*/ wire BUTO_AxCDEFGH = nand(AFEP_ABxxxxGH, ATYP_xBCDExxx, BAZE_xBCDExxx);
  /*p01.BELE*/ wire BELE_xBxxxxxx = not(BUTO_AxCDEFGH);
  /*p01.ATEZ*/ wire ATEZ_CLKBAD   = not(SYS_PIN_CLK_GOOD);
  /*p01.BYJU*/ wire BYJU_AxCDEFGH = nor(BELE_xBxxxxxx, ATEZ_CLKBAD);
  return BYJU_AxCDEFGH;
}

//-----------------------------------------------------------------------------
// Timer signals

wire SchematicTop::UVYN_DIV_05n() const    { return not(tim_reg.TAMA_DIV_05); }
wire SchematicTop::UMEK_DIV_06n() const    { return not(tim_reg.UGOT_DIV_06); }
wire SchematicTop::UREK_DIV_07n() const    { return not(tim_reg.TULU_DIV_07); }
wire SchematicTop::UPOF_DIV_15() const     { return tim_reg.UPOF_DIV_15; }
wire SchematicTop::MOBA_INT_TIMERp() const { return tim_reg.MOBA_INT_TIMERp; }

//-----------------------------------------------------------------------------
// Reset signals

wire SchematicTop::AVOR_RSTp() const {
  /*p01.AVOR*/ wire AVOR_RSTp = or(rst_reg.AFER_RSTp.q(), rst_reg.ASOL_RST_LATCHp.q());   
  return AVOR_RSTp;
}

wire SchematicTop::XAPO_VID_RSTn() const {
  /*p01.AVOR*/ wire AVOR_RSTp = or(rst_reg.AFER_RSTp.q(), rst_reg.ASOL_RST_LATCHp.q());   
  /*p01.ALUR*/ wire ALUR_RSTn = not(AVOR_RSTp);   // this goes all over the place
  /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
  /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
  /*p01.XEBE*/ wire XEBE_RSTn = not(XORE_RSTp);
  /*p01.XODO*/ wire XODO_VID_RSTp = nand(XEBE_RSTn, XONA_LCDC_EN);
  /*p01.XAPO*/ wire XAPO_VID_RSTn = not(XODO_VID_RSTp);
  return XAPO_VID_RSTn;
}


//-----------------------------------------------------------------------------
// CPU signals

wire SchematicTop::UJYV_CPU_RD() const {
  /*p07.UJYV*/ wire UJYV_CPU_RD = mux2_n(EXT_PIN_RDn_C, CPU_PIN_RD, UNOR_MODE_DBG2p());
  return UJYV_CPU_RD;
}

wire SchematicTop::UBAL_CPU_WRp_ABCDExxx() const {
  /*p01.ATYP*/ wire ATYP_xBCDExxx = not(!AFUR_xBCDExxx());
  /*p01.ADAR*/ wire ADAR_ABCDxxxx = not(ADYK_xxxxEFGH());
  /*p01.AFAS*/ wire AFAS_xxxxxFGH = nor(ADAR_ABCDxxxx, ATYP_xBCDExxx);
  /*p01.AREV*/ wire AREV_CPU_WRn_ABCDExxx = nand(CPU_PIN_WR, AFAS_xxxxxFGH);
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxxFGH    = not(AREV_CPU_WRn_ABCDExxx);
  /*p07.UBAL*/ wire UBAL_CPU_WRp_ABCDExxx = mux2_n(EXT_PIN_WRn_C, APOV_CPU_WRp_xxxxxFGH, UNOR_MODE_DBG2p());
  return UBAL_CPU_WRp_ABCDExxx;
}

int SchematicTop::get_addr() const {
  return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03, CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
    CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11, CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
}

void SchematicTop::preset_addr(bool oe, uint16_t addr) {
  CPU_PIN_A00.preset(oe, addr & 0x0001);
  CPU_PIN_A01.preset(oe, addr & 0x0002);
  CPU_PIN_A02.preset(oe, addr & 0x0004);
  CPU_PIN_A03.preset(oe, addr & 0x0008);
  CPU_PIN_A04.preset(oe, addr & 0x0010);
  CPU_PIN_A05.preset(oe, addr & 0x0020);
  CPU_PIN_A06.preset(oe, addr & 0x0040);
  CPU_PIN_A07.preset(oe, addr & 0x0080);
  CPU_PIN_A08.preset(oe, addr & 0x0100);
  CPU_PIN_A09.preset(oe, addr & 0x0200);
  CPU_PIN_A10.preset(oe, addr & 0x0400);
  CPU_PIN_A11.preset(oe, addr & 0x0800);
  CPU_PIN_A12.preset(oe, addr & 0x1000);
  CPU_PIN_A13.preset(oe, addr & 0x2000);
  CPU_PIN_A14.preset(oe, addr & 0x4000);
  CPU_PIN_A15.preset(oe, addr & 0x8000);
}

int SchematicTop::get_data() const {
  return pack(CPU_TRI_D0, CPU_TRI_D1, CPU_TRI_D2, CPU_TRI_D3, CPU_TRI_D4, CPU_TRI_D5, CPU_TRI_D6, CPU_TRI_D7);
}

void SchematicTop::set_data(bool oe, uint8_t data) {
  CPU_TRI_D0.set_tribuf(oe, data & 0x01);
  CPU_TRI_D1.set_tribuf(oe, data & 0x02);
  CPU_TRI_D2.set_tribuf(oe, data & 0x04);
  CPU_TRI_D3.set_tribuf(oe, data & 0x08);
  CPU_TRI_D4.set_tribuf(oe, data & 0x10);
  CPU_TRI_D5.set_tribuf(oe, data & 0x20);
  CPU_TRI_D6.set_tribuf(oe, data & 0x40);
  CPU_TRI_D7.set_tribuf(oe, data & 0x80);
}

void SchematicTop::preset_rd(bool rd)            { CPU_PIN_RD.preset(true, rd); }
void SchematicTop::preset_wr(bool wr)            { CPU_PIN_WR.preset(true, wr); }
void SchematicTop::preset_addr_valid(bool valid) { CPU_PIN_ADDR_VALID.preset(true, valid); }

//-----------------------------------------------------------------------------
// Bootrom signals

wire SchematicTop::BOOT_BITn() const {
  /*p07.TEPU*/ return bootrom.BOOT_BITn;
}

wire SchematicTop::TUTU_BOOTp() const {
  /*p07.TERA*/ wire TERA_BOOT_BITp  = not(BOOT_BITn());
  /*p07.TULO*/ wire TULO_ADDR_00XXp = nor(CPU_PIN_A15, CPU_PIN_A14, CPU_PIN_A13, CPU_PIN_A12, CPU_PIN_A11, CPU_PIN_A10, CPU_PIN_A09, CPU_PIN_A08);
  /*p07.TUTU*/ wire TUTU_ADDR_BOOTp = and (TERA_BOOT_BITp, TULO_ADDR_00XXp);
  /*p07.TUTU*/ wire TUTU_BOOTp = TUTU_ADDR_BOOTp;
  return TUTU_BOOTp;
}

//-----------------------------------------------------------------------------
// DMA signals

wire SchematicTop::MATU_DMA_RUNNINGp() const {
  /*p04.MATU*/ return dma_reg.MATU_DMA_RUNNINGp;
}

wire SchematicTop::MUDA_DMA_SRC_VRAMp() const {
  /*p04.LEBU*/ wire LEBU_DMA_ADDR_A15n  = not(dma_reg.DMA_A15.q());
  /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = nor(dma_reg.DMA_A13.q(), dma_reg.DMA_A14.q(), LEBU_DMA_ADDR_A15n);
  return MUDA_DMA_SRC_VRAMp;
}

wire SchematicTop::DMA_A00() const { return dma_reg.DMA_A00; }
wire SchematicTop::DMA_A01() const { return dma_reg.DMA_A01; }
wire SchematicTop::DMA_A02() const { return dma_reg.DMA_A02; }
wire SchematicTop::DMA_A03() const { return dma_reg.DMA_A03; }
wire SchematicTop::DMA_A04() const { return dma_reg.DMA_A04; }
wire SchematicTop::DMA_A05() const { return dma_reg.DMA_A05; }
wire SchematicTop::DMA_A06() const { return dma_reg.DMA_A06; }
wire SchematicTop::DMA_A07() const { return dma_reg.DMA_A07; }
wire SchematicTop::DMA_A08() const { return dma_reg.DMA_A08; }
wire SchematicTop::DMA_A09() const { return dma_reg.DMA_A09; }
wire SchematicTop::DMA_A10() const { return dma_reg.DMA_A10; }
wire SchematicTop::DMA_A11() const { return dma_reg.DMA_A11; }
wire SchematicTop::DMA_A12() const { return dma_reg.DMA_A12; }
wire SchematicTop::DMA_A13() const { return dma_reg.DMA_A13; }
wire SchematicTop::DMA_A14() const { return dma_reg.DMA_A14; }
wire SchematicTop::DMA_A15() const { return dma_reg.DMA_A15; }

//------------------------------------------------------------------------------
// Joypad signals

wire SchematicTop::ASOK_INT_JOYPADp() const {
  /*p02.ASOK*/ wire ASOK_INT_JOYPADp = and (joy_reg.JP_GLITCH3, joy_reg.JP_GLITCH0);
  return ASOK_INT_JOYPADp;
}

//------------------------------------------------------------------------------

//void preset(bool EXT_PIN_RDn_A, bool EXT_PIN_CSn_A);

/*
void preset_d(bool oe, uint8_t d) {
PIN_D0_C.preset(oe, d & 0x01);
PIN_D1_C.preset(oe, d & 0x02);
PIN_D2_C.preset(oe, d & 0x04);
PIN_D3_C.preset(oe, d & 0x08);
PIN_D4_C.preset(oe, d & 0x10);
PIN_D5_C.preset(oe, d & 0x20);
PIN_D6_C.preset(oe, d & 0x40);
PIN_D7_C.preset(oe, d & 0x80);
}
*/


#if 0
void ExtPinsOut::preset() {
  EXT_PIN_WRn_C.preset(true, 0);   // -> P07.UBAL
  EXT_PIN_RDn_C.preset(true, 0);   // -> P07.UJYV
  EXT_PIN_A00_C.preset(true, 0);   // -> P08.KOVA
  EXT_PIN_A01_C.preset(true, 0);   // -> P08.CAMU
  EXT_PIN_A02_C.preset(true, 0);   // -> P08.BUXU
  EXT_PIN_A03_C.preset(true, 0);   // -> P08.BASE
  EXT_PIN_A04_C.preset(true, 0);   // -> P08.AFEC
  EXT_PIN_A05_C.preset(true, 0);   // -> P08.ABUP
  EXT_PIN_A06_C.preset(true, 0);   // -> P08.CYGU
  EXT_PIN_A07_C.preset(true, 0);   // -> P08.COGO
  EXT_PIN_A08_C.preset(true, 0);   // -> P08.MUJY
  EXT_PIN_A09_C.preset(true, 0);   // -> P08.NENA
  EXT_PIN_A10_C.preset(true, 0);   // -> P08.SURA
  EXT_PIN_A11_C.preset(true, 0);   // -> P08.MADY
  EXT_PIN_A12_C.preset(true, 0);   // -> P08.LAHE
  EXT_PIN_A13_C.preset(true, 0);   // -> P08.LURA
  EXT_PIN_A14_C.preset(true, 0);   // -> P08.PEVO
  EXT_PIN_A15_C.preset(true, 0);   // -> P08.RAZA

  if (!EXT_PIN_RDn_A && !EXT_PIN_CSn_A) {
    uint16_t ext_addr = get_addr();

    if (0x0000 <= ext_addr && ext_addr <= 0x7FFF) {
      // Cart rom
      //uint8_t d = rom[ext_addr];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xC000 <= ext_addr && ext_addr <= 0xDFFF) {
      // Main ram
      //uint8_t d = ram[ext_addr - 0xC000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else if (0xE000 <= ext_addr && ext_addr <= 0xFFFF) {
      // Echo ram
      //uint8_t d = ram[ext_addr - 0xE000];
      uint8_t d = 0;
      preset_d(true, d);
    }
    else {
      printf("Bad address?\n");
      __debugbreak();
    }
  }
  else {
    preset_d(false, 0);
  }
}
#endif





#if 0
inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& d) {
  dump_pin(text_painter, a.a, d.a);
}

inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& b, const PinIn& c, const PinOut& d) {
  dump_pin(text_painter, a.a, b.a, c.a, d.a);
}

void dump_regs(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf(" ----- EXT_BUS -----\n");

  text_painter.dprintf("ADDR_LATCH ");
  dump2(text_painter, EXT_ADDR_LATCH_14.a);
  dump2(text_painter, EXT_ADDR_LATCH_13.a);
  dump2(text_painter, EXT_ADDR_LATCH_12.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_11.a);
  dump2(text_painter, EXT_ADDR_LATCH_10.a);
  dump2(text_painter, EXT_ADDR_LATCH_09.a);
  dump2(text_painter, EXT_ADDR_LATCH_08.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_07.a);
  dump2(text_painter, EXT_ADDR_LATCH_06.a);
  dump2(text_painter, EXT_ADDR_LATCH_05.a);
  dump2(text_painter, EXT_ADDR_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, EXT_ADDR_LATCH_03.a);
  dump2(text_painter, EXT_ADDR_LATCH_02.a);
  dump2(text_painter, EXT_ADDR_LATCH_01.a);
  dump2(text_painter, EXT_ADDR_LATCH_00.a);
  text_painter.newline();

  text_painter.dprintf("DATA_LATCH ");
  dump2(text_painter, CART_DATA_LATCH_07.a);
  dump2(text_painter, CART_DATA_LATCH_06.a);
  dump2(text_painter, CART_DATA_LATCH_05.a);
  dump2(text_painter, CART_DATA_LATCH_04.a);
  text_painter.add_char(':');
  dump2(text_painter, CART_DATA_LATCH_03.a);
  dump2(text_painter, CART_DATA_LATCH_02.a);
  dump2(text_painter, CART_DATA_LATCH_01.a);
  dump2(text_painter, CART_DATA_LATCH_00.a);
  text_painter.newline();
  */
}

void dump_pins(TextPainter& /*text_painter*/) {
  /*
  text_painter.dprintf("----- EXT_PINS -----\n");

  text_painter.dprintf("PIN_PHI %d\n", PIN_PHI.a.val);
  text_painter.dprintf("WR  %d\n", EXT_PIN_WRn_A.a.val);
  text_painter.dprintf("RD  %d\n", EXT_PIN_RDn_A.a.val);
  text_painter.dprintf("CS  %d\n", EXT_PIN_CSn_A.a.val);

  text_painter.add_text("Axx ");
  dump_pins(text_painter, EXT_PIN_A15_A, EXT_PIN_A15_D);
  dump_pins(text_painter, EXT_PIN_A14_A, EXT_PIN_A14_D);
  dump_pins(text_painter, EXT_PIN_A13_A, EXT_PIN_A13_D);
  dump_pins(text_painter, EXT_PIN_A12_A, EXT_PIN_A12_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A11_A, EXT_PIN_A11_D);
  dump_pins(text_painter, EXT_PIN_A10_A, EXT_PIN_A10_D);
  dump_pins(text_painter, EXT_PIN_A09_A, EXT_PIN_A09_D);
  dump_pins(text_painter, EXT_PIN_A08_A, EXT_PIN_A08_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A07_A, EXT_PIN_A07_D);
  dump_pins(text_painter, EXT_PIN_A06_A, EXT_PIN_A06_D);
  dump_pins(text_painter, EXT_PIN_A05_A, EXT_PIN_A05_D);
  dump_pins(text_painter, EXT_PIN_A04_A, EXT_PIN_A04_D);
  text_painter.add_char(':');
  dump_pins(text_painter, EXT_PIN_A03_A, EXT_PIN_A03_D);
  dump_pins(text_painter, EXT_PIN_A02_A, EXT_PIN_A02_D);
  dump_pins(text_painter, EXT_PIN_A01_A, EXT_PIN_A01_D);
  dump_pins(text_painter, EXT_PIN_A00_A, EXT_PIN_A00_D);
  text_painter.newline();

  text_painter.add_text("Dxx ");
  dump_pins(text_painter, EXT_PIN_D7_A, EXT_PIN_D7_B, PIN_D7_C, EXT_PIN_D7_D);
  dump_pins(text_painter, EXT_PIN_D6_A, EXT_PIN_D6_B, PIN_D6_C, EXT_PIN_D6_D);
  dump_pins(text_painter, EXT_PIN_D5_A, EXT_PIN_D5_B, PIN_D5_C, EXT_PIN_D5_D);
  dump_pins(text_painter, EXT_PIN_D4_A, EXT_PIN_D4_B, PIN_D4_C, EXT_PIN_D4_D);
  dump_pins(text_painter, EXT_PIN_D3_A, EXT_PIN_D3_B, PIN_D3_C, EXT_PIN_D3_D);
  dump_pins(text_painter, EXT_PIN_D2_A, EXT_PIN_D2_B, PIN_D2_C, EXT_PIN_D2_D);
  dump_pins(text_painter, EXT_PIN_D1_A, EXT_PIN_D1_B, PIN_D1_C, EXT_PIN_D1_D);
  dump_pins(text_painter, EXT_PIN_D0_A, EXT_PIN_D0_B, PIN_D0_C, EXT_PIN_D0_D);
  text_painter.newline();
  */
}

uint16_t get_addr() const {
  uint16_t ext_addr = (uint16_t)pack(
    EXT_PIN_A00_A, EXT_PIN_A01_A, EXT_PIN_A02_A, EXT_PIN_A03_A,
    EXT_PIN_A04_A, EXT_PIN_A05_A, EXT_PIN_A06_A, EXT_PIN_A07_A,
    EXT_PIN_A08_A, EXT_PIN_A09_A, EXT_PIN_A10_A, EXT_PIN_A11_A,
    EXT_PIN_A12_A, EXT_PIN_A13_A, EXT_PIN_A14_A, EXT_PIN_A15_A);

  return ext_addr;
}

/*
void dump_pins(TextPainter& text_painter) {
text_painter.dprintf("----- SYS_PINS -----\n");
text_painter.dprintf("PIN_RST      %d\n", PIN_RST.a.val);
text_painter.dprintf("PIN_CLK_GOOD %d\n", PIN_CLK_GOOD.a.val);
text_painter.dprintf("SYS_PIN_CLK_xBxDxFxH   %d\n", SYS_PIN_CLK_xBxDxFxH.a.val);
text_painter.dprintf("SYS_PIN_T1       %d\n", SYS_PIN_T1.a.val);
text_painter.dprintf("SYS_PIN_T2       %d\n", SYS_PIN_T2.a.val);
text_painter.newline();
}
*/

#endif