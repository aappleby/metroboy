#include "Sch_BusMux.h"

#include "Sch_Top.h"

using namespace Schematics;

/*
0b000xxxxx_xxxxxxxx - 0x0000 to 0x1FFF (cart rom)
0b001xxxxx_xxxxxxxx - 0x2000 to 0x3FFF (cart rom)
0b010xxxxx_xxxxxxxx - 0x4000 to 0x5FFF (cart rom banked)
0b011xxxxx_xxxxxxxx - 0x6000 to 0x7FFF (cart rom banked)
0b100xxxxx_xxxxxxxx - 0x8000 to 0x9FFF (vram)
0b101xxxxx_xxxxxxxx - 0xA000 to 0xBFFF (cart ram)
0b110xxxxx_xxxxxxxx - 0xC000 to 0xDFFF (internal ram)
0b111xxxxx_xxxxxxxx - 0xE000 to 0xFFFF (echo ram, oam, high ram, io)
0b11111110_00000000 - 0xFE00 - OAM begin
0b11111110_10011111 - 0xFE9F - OAM end
0b11111111_00000000 - 0xFF00 - IO begin
0b11111111_01111111 - 0xFF7F - IO end
0b11111111_10000000 - 0xFF80 - Zeropage begin
0b11111111_11111110 - 0xFFFE - Zeropage end
0b11111111_11111111 - 0xFFFF - Interrupt enable
*/

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

// Schematic wrong, AVER is def nand
// AVER01 << ACYL03
// AVER02 << XYSO02
// AVER03 >> BYCU03

// VAPE is def and
// VAPE01 << TACU03
// VAPE02 << TUVO04
// VAPE03 nc


// Possible schematic error - CUFE doesn't make sense as or(and()), only as and(or())

// 4-rung whereas the or(and()) were 5 rung?
// Arm on left (gnd) side
// CUFE01 << SARO03
// CUFE02 << MATU17
// CUFE03 >> BYCU01
// CUFE04 << MOPA03

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

//------------------------------------------------------------------------------

// cpu can read oam when there's no parsing, rendering, or dma
// so byte 0 is the palette number? something wrong here...
// this is 8 sets of 4 tribuffers feeding into an inverter, which we'll model as an amux4n
// except the inputs are negated or something?

// NAXY01 << UVYT02
// NAXY02 << MAKA17
// NAXY03 >> POWU02

// schematic says naxy = nor(maka, luvy), but wrong
// naxy = nor(uvyt, maka)

//------------------------------------------------------------------------------

void BusMux::tick(SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void BusMux::tock(SchematicTop& top) {

  // Address decoder
  /*p07.RYCU*/ wire RYCU_0000_FDFFn = not(top.TUNA_0000_FDFFp());
  /*p07.SOHA*/ wire SOHA_FF00_FFFFn = not(top.SYKE_FF00_FFFFp());
  /*p07.ROPE*/ wire ROPE_FE00_FEFFn = nand(RYCU_0000_FDFFn, SOHA_FF00_FFFFn);
  /*p07.SARO*/ wire SARO_FE00_FEFFp = not(ROPE_FE00_FEFFn);
  /*p28.ADAH*/ wire ADAH_FE00_FEFFn = not(SARO_FE00_FEFFp);

  /*p08.SORE*/ wire SORE_0000_7FFFp = not(top.CPU_PIN_A15);
  /*p08.TEVY*/ wire TEVY_8000_9FFFn = or(top.CPU_PIN_A13, top.CPU_PIN_A14, SORE_0000_7FFFp);
  /*p08.TEXO*/ wire TEXO_8000_9FFFn = and (top.CPU_PIN_ADDR_VALID, TEVY_8000_9FFFn);
  /*p08.LEVO*/ wire LEVO_8000_9FFFp = not(TEXO_8000_9FFFn);

  // CPU read/write

  /*p04.DECY*/ wire DECY_FROM_CPU5n = not(top.CPU_PIN5);
  /*p04.CATY*/ wire CATY_FROM_CPU5p = not(DECY_FROM_CPU5n);
  /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(top.ASOT_CPU_RDp(), CATY_FROM_CPU5p);
  /*p28.LEKO*/ wire LEKO_CPU_RDp = not(MYNU_CPU_RDn);

  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx = not(top.AREV_CPU_WRn_ABCDxxxH());
  /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH = not(APOV_CPU_WRp_xxxxEFGx);

  {
    /*p04.MAKA*/ MAKA_FROM_CPU5_SYNC.set(top.ZEME_AxCxExGx(), top.CUNU_SYS_RSTn(), top.CPU_PIN5);
  }

  {
    // CPU controls OAM bus if there's no scanning, rendering, or dmaing.
    /*p28.ASAM*/ wire ASAM_CPU_OAM_RDn  = or (top.ACYL_SCANNINGp(), top.XYMU_RENDERINGp(), top.MATU_DMA_RUNNINGp());
    /*p28.GARO*/ wire GARO_A0n = not(top.CPU_PIN_A00);
    /*p28.WACU*/ wire WACU_A1n = not(top.CPU_PIN_A01);
    /*p28.GOSE*/ wire GOSE_A2n = not(top.CPU_PIN_A02);
    /*p28.WAPE*/ wire WAPE_A3n = not(top.CPU_PIN_A03);
    /*p28.FEVU*/ wire FEVU_A4n = not(top.CPU_PIN_A04);
    /*p28.GERA*/ wire GERA_A5n = not(top.CPU_PIN_A05);
    /*p28.WAXA*/ wire WAXA_A6n = not(top.CPU_PIN_A06);
    /*p28.FOBY*/ wire FOBY_A7n = not(top.CPU_PIN_A07);

    // PPU controls OAM bus if it's rendering and there's no DMA running.
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(top.MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY     = and (BOGE_DMA_RUNNINGn, top.XYMU_RENDERINGp());
    /*p28.BETE*/ wire BETE_PPU_OAM_RDn  = not(AJON_OAM_BUSY);

    /*p28.GYBU*/ wire GYBU_IDX_0n = not(top.SPR_TRI_INDX_0.q());
    /*p28.GYKA*/ wire GYKA_IDX_1n = not(top.SPR_TRI_INDX_1.q());
    /*p28.FABY*/ wire FABY_IDX_2n = not(top.SPR_TRI_INDX_2.q());
    /*p28.FACO*/ wire FACO_IDX_3n = not(top.SPR_TRI_INDX_3.q());
    /*p28.FUGU*/ wire FUGU_IDX_4n = not(top.SPR_TRI_INDX_4.q());
    /*p28.FYKE*/ wire FYKE_IDX_5n = not(top.SPR_TRI_INDX_5.q());

    // Scanner controls OAM address if it's running (collision w/ DMA?)
    /*p28.APAR*/ wire APAR_SCAN_OAM_RDn  = not(top.ACYL_SCANNINGp());
    /*p28.GUSE*/ wire GUSE_SCAN0n = not(top.YFEL_SCAN0());
    /*p28.GEMA*/ wire GEMA_SCAN1n = not(top.WEWY_SCAN1());
    /*p28.FUTO*/ wire FUTO_SCAN2n = not(top.GOSO_SCAN2());
    /*p28.FAKU*/ wire FAKU_SCAN3n = not(top.ELYN_SCAN3());
    /*p28.GAMA*/ wire GAMA_SCAN4n = not(top.FAHA_SCAN4());
    /*p28.GOBY*/ wire GOBY_SCAN5n = not(top.FONY_SCAN5());

    // DMA controls OAM address if it's running.
    /*p04.DUGA*/ wire DUGA_DMA_OAM_RDn  = not(top.MATU_DMA_RUNNINGp());
    /*p28.FODO*/ wire FODO_DMA_A00n = not(top.DMA_A00());
    /*p28.FESA*/ wire FESA_DMA_A01n = not(top.DMA_A01());
    /*p28.FAGO*/ wire FAGO_DMA_A02n = not(top.DMA_A02());
    /*p28.FYKY*/ wire FYKY_DMA_A03n = not(top.DMA_A03());
    /*p28.ELUG*/ wire ELUG_DMA_A04n = not(top.DMA_A04());
    /*p28.EDOL*/ wire EDOL_DMA_A05n = not(top.DMA_A05());
    /*p28.FYDU*/ wire FYDU_DMA_A06n = not(top.DMA_A06());
    /*p28.FETU*/ wire FETU_DMA_A07n = not(top.DMA_A07());

    /*p28.GECA*/ wire GECA_GND = top.WEFE_GND;
    /*p28.WYDU*/ wire WYDU_GND = top.WEFE_GND;

    /*p28.WUWE*/ wire WUWE_GND = top.GND;
    /*p28.GEFY*/ wire GEFY_GND = top.GND;

    // Hacky model of weird tribus
    /*p28.GEKA*/ GEKA_OAM_A0p = not((GARO_A0n & !ASAM_CPU_OAM_RDn) | (GECA_GND    & !BETE_PPU_OAM_RDn) | (GEFY_GND    & !APAR_SCAN_OAM_RDn) | (FODO_DMA_A00n & !DUGA_DMA_OAM_RDn));
    /*p28.ZYFO*/ ZYFO_OAM_A1p = not((WACU_A1n & !ASAM_CPU_OAM_RDn) | (WYDU_GND    & !BETE_PPU_OAM_RDn) | (WUWE_GND    & !APAR_SCAN_OAM_RDn) | (FESA_DMA_A01n & !DUGA_DMA_OAM_RDn));
    /*p28.YFOT*/ YFOT_OAM_A2p = not((GOSE_A2n & !ASAM_CPU_OAM_RDn) | (GYBU_IDX_0n & !BETE_PPU_OAM_RDn) | (GUSE_SCAN0n & !APAR_SCAN_OAM_RDn) | (FAGO_DMA_A02n & !DUGA_DMA_OAM_RDn));
    /*p28.YFOC*/ YFOC_OAM_A3p = not((WAPE_A3n & !ASAM_CPU_OAM_RDn) | (GYKA_IDX_1n & !BETE_PPU_OAM_RDn) | (GEMA_SCAN1n & !APAR_SCAN_OAM_RDn) | (FYKY_DMA_A03n & !DUGA_DMA_OAM_RDn));
    /*p28.YVOM*/ YVOM_OAM_A4p = not((FEVU_A4n & !ASAM_CPU_OAM_RDn) | (FABY_IDX_2n & !BETE_PPU_OAM_RDn) | (FUTO_SCAN2n & !APAR_SCAN_OAM_RDn) | (ELUG_DMA_A04n & !DUGA_DMA_OAM_RDn));
    /*p28.YMEV*/ YMEV_OAM_A5p = not((GERA_A5n & !ASAM_CPU_OAM_RDn) | (FACO_IDX_3n & !BETE_PPU_OAM_RDn) | (FAKU_SCAN3n & !APAR_SCAN_OAM_RDn) | (EDOL_DMA_A05n & !DUGA_DMA_OAM_RDn));
    /*p28.XEMU*/ XEMU_OAM_A6p = not((WAXA_A6n & !ASAM_CPU_OAM_RDn) | (FUGU_IDX_4n & !BETE_PPU_OAM_RDn) | (GAMA_SCAN4n & !APAR_SCAN_OAM_RDn) | (FYDU_DMA_A06n & !DUGA_DMA_OAM_RDn));
    /*p28.YZET*/ YZET_OAM_A7p = not((FOBY_A7n & !ASAM_CPU_OAM_RDn) | (FYKE_IDX_5n & !BETE_PPU_OAM_RDn) | (GOBY_SCAN5n & !APAR_SCAN_OAM_RDn) | (FETU_DMA_A07n & !DUGA_DMA_OAM_RDn));

    OAM_PIN_A0.set(GEKA_OAM_A0p);
    OAM_PIN_A1.set(ZYFO_OAM_A1p);
    OAM_PIN_A2.set(YFOT_OAM_A2p);
    OAM_PIN_A3.set(YFOC_OAM_A3p);
    OAM_PIN_A4.set(YVOM_OAM_A4p);
    OAM_PIN_A5.set(YMEV_OAM_A5p);
    OAM_PIN_A6.set(XEMU_OAM_A6p);
    OAM_PIN_A7.set(YZET_OAM_A7p);
  }

  {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(top.MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, top.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(top.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), AJON_OAM_BUSY); // def nor
    /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p04.NAXY*/ wire NAXY_DMA_OAM_WENp = nor(top.UVYT_ABCDxxxx(), MAKA_FROM_CPU5_SYNC.q()); // def nor
    /*p04.POWU*/ wire POWU_DMA_OAM_WRp = and (top.MATU_DMA_RUNNINGp(), NAXY_DMA_OAM_WENp); // def and
    /*p04.WYJA*/ wire WYJA_OAM_WRp = or (and (AMAB_OAM_LOCKn, top.CUPA_CPU_WRp_xxxxEFGx()), POWU_DMA_OAM_WRp);
    /*p28.WAFO*/ wire WAFO_OAM_A0n = not(top.GEKA_OAM_A0p()); // def not
    /*p28.YNYC*/ wire YNYC_OAM_B_WRn = and (WYJA_OAM_WRp, WAFO_OAM_A0n); // def and
    /*p28.YLYC*/ wire YLYC_OAM_A_WRn = and (WYJA_OAM_WRp, top.GEKA_OAM_A0p()); // def and
    /*p28.ZONE*/ wire ZONE = not(YLYC_OAM_A_WRn);
    /*p28.ZOFE*/ wire ZOFE = not(YNYC_OAM_B_WRn);
    OAM_PIN_WR_A.set(ZONE);
    OAM_PIN_WR_B.set(ZOFE);
  }

  // Internal latch -> CPU bus
  {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(top.MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, top.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma

    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(top.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), AJON_OAM_BUSY); // def nor
    /*p28.WAFO*/ wire WAFO_OAM_A0n = not(GEKA_OAM_A0p); // def not
    /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and

    // FIXME polarity
    /*p28.WUKU*/ wire WUKU_OAM_A_CPU_RD  = and (LEKO_CPU_RDp, AMAB_OAM_LOCKn, GEKA_OAM_A0p);
    /*p28.GUKO*/ wire GUKO_OAM_B_CPU_RD  = and (LEKO_CPU_RDp, AMAB_OAM_LOCKn, WAFO_OAM_A0n);
    /*p28.WEWU*/ wire WEWU_OAM_A_CPU_RDn = not(WUKU_OAM_A_CPU_RD);
    /*p28.WUME*/ wire WUME_OAM_B_CPU_RDn = not(GUKO_OAM_B_CPU_RD);

    /*p31.XACA*/ top.CPU_TRI_D0.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, XYKY_LATCH_OAM_A0.q());
    /*p31.XAGU*/ top.CPU_TRI_D1.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, YRUM_LATCH_OAM_A1.q());
    /*p31.XEPU*/ top.CPU_TRI_D2.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, YSEX_LATCH_OAM_A2.q());
    /*p31.XYGU*/ top.CPU_TRI_D3.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, YVEL_LATCH_OAM_A3.q());
    /*p31.XUNA*/ top.CPU_TRI_D4.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, WYNO_LATCH_OAM_A4.q());
    /*p31.DEVE*/ top.CPU_TRI_D5.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, CYRA_LATCH_OAM_A5.q());
    /*p31.ZEHA*/ top.CPU_TRI_D6.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, ZUVE_LATCH_OAM_A6.q());
    /*p31.FYRA*/ top.CPU_TRI_D7.set_tribuf_10n(WEWU_OAM_A_CPU_RDn, ECED_LATCH_OAM_A7.q());

    /*p29.YFAP*/ top.CPU_TRI_D0.set_tribuf_10n(WUME_OAM_B_CPU_RDn, YDYV_LATCH_OAM_B0.q());
    /*p29.XELE*/ top.CPU_TRI_D1.set_tribuf_10n(WUME_OAM_B_CPU_RDn, YCEB_LATCH_OAM_B1.q());
    /*p29.YPON*/ top.CPU_TRI_D2.set_tribuf_10n(WUME_OAM_B_CPU_RDn, ZUCA_LATCH_OAM_B2.q());
    /*p29.XUVO*/ top.CPU_TRI_D3.set_tribuf_10n(WUME_OAM_B_CPU_RDn, WONE_LATCH_OAM_B3.q());
    /*p29.ZYSA*/ top.CPU_TRI_D4.set_tribuf_10n(WUME_OAM_B_CPU_RDn, ZAXE_LATCH_OAM_B4.q());
    /*p29.YWEG*/ top.CPU_TRI_D5.set_tribuf_10n(WUME_OAM_B_CPU_RDn, XAFU_LATCH_OAM_B5.q());
    /*p29.XABU*/ top.CPU_TRI_D6.set_tribuf_10n(WUME_OAM_B_CPU_RDn, YSES_LATCH_OAM_B6.q());
    /*p29.YTUX*/ top.CPU_TRI_D7.set_tribuf_10n(WUME_OAM_B_CPU_RDn, ZECA_LATCH_OAM_B7.q());
  }

  // DMA vram -> oam
  {
    /*p04.MATU*/ wire MATU_DMA_RUNNINGp = top.MATU_DMA_RUNNINGp();
    /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = top.MUDA_DMA_SRC_VRAMp();

    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn = nand(MATU_DMA_RUNNINGp, MUDA_DMA_SRC_VRAMp);
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);
    /*p28.AZAR*/ wire AZAR_DMA_READ_VRAMn = not(LUFA_DMA_READ_VRAMp);

    /*p28.WUZU*/ OAM_PIN_DA0.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D0);
    /*p28.AXER*/ OAM_PIN_DA1.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D1);
    /*p28.ASOX*/ OAM_PIN_DA2.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D2);
    /*p28.CETU*/ OAM_PIN_DA3.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D3);
    /*p28.ARYN*/ OAM_PIN_DA4.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D4);
    /*p28.ACOT*/ OAM_PIN_DA5.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D5);
    /*p28.CUJE*/ OAM_PIN_DA6.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D6);
    /*p28.ATER*/ OAM_PIN_DA7.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D7);
    /*p28.WOWA*/ OAM_PIN_DB0.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D0);
    /*p28.AVEB*/ OAM_PIN_DB1.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D1);
    /*p28.AMUH*/ OAM_PIN_DB2.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D2);
    /*p28.COFO*/ OAM_PIN_DB3.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D3);
    /*p28.AZOZ*/ OAM_PIN_DB4.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D4);
    /*p28.AGYK*/ OAM_PIN_DB5.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D5);
    /*p28.BUSE*/ OAM_PIN_DB6.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D6);
    /*p28.ANUM*/ OAM_PIN_DB7.set_tribuf_6n(AZAR_DMA_READ_VRAMn, top.VRM_TRI_D7);
  }

  // DMA ext -> oam
  {
    /*p04.MATU*/ wire MATU_DMA_RUNNINGp = top.MATU_DMA_RUNNINGp();
    /*p04.MUDA*/ wire MUDA_DMA_SRC_VRAMp = top.MUDA_DMA_SRC_VRAMp();

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(MUDA_DMA_SRC_VRAMp);
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(MATU_DMA_RUNNINGp, LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);
    /*p25.CEDE*/ wire CEDE_DMA_READ_CARTn = not(LUMA_DMA_READ_CARTp);

    // FIXME data polarity?
    // FIXME Hax to prevent reading hiz

    if (!CEDE_DMA_READ_CARTn) {
      /*p25.RALO*/ wire RALO = not(top.EXT_PIN_D0_C);
      /*p25.TUNE*/ wire TUNE = not(top.EXT_PIN_D1_C);
      /*p25.SERA*/ wire SERA = not(top.EXT_PIN_D2_C);
      /*p25.TENU*/ wire TENU = not(top.EXT_PIN_D3_C);
      /*p25.SYSA*/ wire SYSA = not(top.EXT_PIN_D4_C);
      /*p25.SUGY*/ wire SUGY = not(top.EXT_PIN_D5_C);
      /*p25.TUBE*/ wire TUBE = not(top.EXT_PIN_D6_C);
      /*p25.SYZO*/ wire SYZO = not(top.EXT_PIN_D7_C);

      /*p25.WEJO*/ OAM_PIN_DA0.set_tribuf_6n(CEDE_DMA_READ_CARTn, RALO);
      /*p25.BUBO*/ OAM_PIN_DA1.set_tribuf_6n(CEDE_DMA_READ_CARTn, TUNE);
      /*p25.BETU*/ OAM_PIN_DA2.set_tribuf_6n(CEDE_DMA_READ_CARTn, SERA);
      /*p25.CYME*/ OAM_PIN_DA3.set_tribuf_6n(CEDE_DMA_READ_CARTn, TENU);
      /*p25.BAXU*/ OAM_PIN_DA4.set_tribuf_6n(CEDE_DMA_READ_CARTn, SYSA);
      /*p25.BUHU*/ OAM_PIN_DA5.set_tribuf_6n(CEDE_DMA_READ_CARTn, SUGY);
      /*p25.BYNY*/ OAM_PIN_DA6.set_tribuf_6n(CEDE_DMA_READ_CARTn, TUBE);
      /*p25.BYPY*/ OAM_PIN_DA7.set_tribuf_6n(CEDE_DMA_READ_CARTn, SYZO);
    
      /*p25.WASA*/ OAM_PIN_DB0.set_tribuf_6n(CEDE_DMA_READ_CARTn, RALO);
      /*p25.BOMO*/ OAM_PIN_DB1.set_tribuf_6n(CEDE_DMA_READ_CARTn, TUNE);
      /*p25.BASA*/ OAM_PIN_DB2.set_tribuf_6n(CEDE_DMA_READ_CARTn, SERA);
      /*p25.CAKO*/ OAM_PIN_DB3.set_tribuf_6n(CEDE_DMA_READ_CARTn, TENU);
      /*p25.BUMA*/ OAM_PIN_DB4.set_tribuf_6n(CEDE_DMA_READ_CARTn, SYSA);
      /*p25.BUPY*/ OAM_PIN_DB5.set_tribuf_6n(CEDE_DMA_READ_CARTn, SUGY);
      /*p25.BASY*/ OAM_PIN_DB6.set_tribuf_6n(CEDE_DMA_READ_CARTn, TUBE);
      /*p25.BAPE*/ OAM_PIN_DB7.set_tribuf_6n(CEDE_DMA_READ_CARTn, SYZO);
    }
    else {
      /*p25.WEJO*/ OAM_PIN_DA0.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BUBO*/ OAM_PIN_DA1.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BETU*/ OAM_PIN_DA2.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.CYME*/ OAM_PIN_DA3.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BAXU*/ OAM_PIN_DA4.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BUHU*/ OAM_PIN_DA5.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BYNY*/ OAM_PIN_DA6.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BYPY*/ OAM_PIN_DA7.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
    
      /*p25.WASA*/ OAM_PIN_DB0.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BOMO*/ OAM_PIN_DB1.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BASA*/ OAM_PIN_DB2.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.CAKO*/ OAM_PIN_DB3.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BUMA*/ OAM_PIN_DB4.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BUPY*/ OAM_PIN_DB5.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BASY*/ OAM_PIN_DB6.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
      /*p25.BAPE*/ OAM_PIN_DB7.set_tribuf_6n(CEDE_DMA_READ_CARTn, 1);
    }
  }

  // CPU bus -> oam
  {
    wire MATU_DMA_RUNNINGp = top.MATU_DMA_RUNNINGp();
    wire XYMU_RENDERINGp = top.XYMU_RENDERINGp();
    wire ACYL_SCANNINGp = top.ACYL_SCANNINGp();
    wire CUPA_CPU_WRp_xxxxEFGx = top.CUPA_CPU_WRp_xxxxEFGx();
    wire XYNY_ABCDxxxx = top.XYNY_ABCDxxxx();

    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(MATU_DMA_RUNNINGp);
    /*p28.AJON*/ wire AJON_OAM_BUSY = and (BOGE_DMA_RUNNINGn, XYMU_RENDERINGp); // def AND. ppu can read oam when there's rendering but no dma
    /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor(MATU_DMA_RUNNINGp, ACYL_SCANNINGp, AJON_OAM_BUSY); // def nor

    /*p28.XUTO*/ wire XUTO_CPU_OAM_WRp = and (SARO_FE00_FEFFp, CUPA_CPU_WRp_xxxxEFGx);

    // wtf is this OR about?
    /*p28.WUJE*/ wire WUJE_CPU_OAM_WRp = or (XYNY_ABCDxxxx, XUTO_CPU_OAM_WRp);
    /*p28.XUPA*/ wire XUPA_CPU_OAM_WRn = not(WUJE_CPU_OAM_WRp);

    /*p28.AMAB*/ wire AMAB_OAM_LOCKn = and (SARO_FE00_FEFFp, AJUJ_OAM_BUSYn); // def and
    /*p28.APAG*/ wire APAG_CPUD_TO_OAMDp = amux2(XUPA_CPU_OAM_WRn, AMAB_OAM_LOCKn, AJUJ_OAM_BUSYn, ADAH_FE00_FEFFn);
    /*p28.AZUL*/ wire AZUL_CPUD_TO_OAMDn = not(APAG_CPUD_TO_OAMDp);

    /*p28.ZAXA*/ OAM_PIN_DA0.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D0);
    /*p28.ZAKY*/ OAM_PIN_DA1.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D1);
    /*p28.WULE*/ OAM_PIN_DA2.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D2);
    /*p28.ZOZO*/ OAM_PIN_DA3.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D3);
    /*p28.ZUFO*/ OAM_PIN_DA4.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D4);
    /*p28.ZATO*/ OAM_PIN_DA5.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D5);
    /*p28.YVUC*/ OAM_PIN_DA6.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D6);
    /*p28.ZUFE*/ OAM_PIN_DA7.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D7);

    /*p28.ZAMY*/ OAM_PIN_DB0.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D0);
    /*p28.ZOPU*/ OAM_PIN_DB1.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D1);
    /*p28.WYKY*/ OAM_PIN_DB2.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D2);
    /*p28.ZAJA*/ OAM_PIN_DB3.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D3);
    /*p28.ZUGA*/ OAM_PIN_DB4.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D4);
    /*p28.ZUMO*/ OAM_PIN_DB5.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D5);
    /*p28.XYTO*/ OAM_PIN_DB6.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D6);
    /*p28.ZYFA*/ OAM_PIN_DB7.set_tribuf_6n(AZUL_CPUD_TO_OAMDn, top.CPU_TRI_D7);
  }

  // OAM data -> internal latch
  {
#if 0
    /*p28.WEFY*/ wire WEFY_SPR_READp     = and(XYMU_RENDERINGp, !TULY_SFETCH_S1, !TESE_SFETCH_S2, TYFO_SFETCH_S0_D1);
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and(XYMU_RENDERINGp, !TULY_SFETCH_S1, !TESE_SFETCH_S2, or(!TYFO_SFETCH_S0_D1, TOXE_SFETCH_S0));
#endif

    /*p29.XOCE*/ wire XOCE_ABxxEFxx = not(top.WOSU_xBCxxFGx());
    /*p28.AJEP*/ wire AJEP_SCAN_OAM_LATCHn = nand(top.ACYL_SCANNINGp(), XOCE_ABxxEFxx); // schematic wrong, is def nand
    /*p28.XUJA*/ wire XUJA_SPR_OAM_LATCHn  = not(top.WEFY_SPR_READp());
    /*p28.BOTA*/ wire BOTA_CPU_OAM_LATCHn  = nand(DECY_FROM_CPU5n, SARO_FE00_FEFFp, top.ASOT_CPU_RDp()); // Schematic wrong, this is NAND

    /*p28.ASYT*/ wire ASYT_OAM_LATCHn      = and(AJEP_SCAN_OAM_LATCHn, XUJA_SPR_OAM_LATCHn, BOTA_CPU_OAM_LATCHn); // def and
    /*p28.BODE*/ wire BODE_OAM_LATCHp      = not(ASYT_OAM_LATCHn);

    // ASYT = and(AJEP, XUJA, BOTA)
    // BODE = not(ASYT)

    // FIXME polarity

    /*p31.XYKY*/ XYKY_LATCH_OAM_A0.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA0);
    /*p31.YRUM*/ YRUM_LATCH_OAM_A1.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA1);
    /*p31.YSEX*/ YSEX_LATCH_OAM_A2.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA2);
    /*p31.YVEL*/ YVEL_LATCH_OAM_A3.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA3);
    /*p31.WYNO*/ WYNO_LATCH_OAM_A4.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA4);
    /*p31.CYRA*/ CYRA_LATCH_OAM_A5.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA5);
    /*p31.ZUVE*/ ZUVE_LATCH_OAM_A6.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA6);
    /*p31.ECED*/ ECED_LATCH_OAM_A7.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DA7);

    /*p29.YDYV*/ YDYV_LATCH_OAM_B0.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB0);
    /*p29.YCEB*/ YCEB_LATCH_OAM_B1.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB1);
    /*p29.ZUCA*/ ZUCA_LATCH_OAM_B2.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB2);
    /*p29.WONE*/ WONE_LATCH_OAM_B3.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB3);
    /*p29.ZAXE*/ ZAXE_LATCH_OAM_B4.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB4);
    /*p29.XAFU*/ XAFU_LATCH_OAM_B5.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB5);
    /*p29.YSES*/ YSES_LATCH_OAM_B6.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB6);
    /*p29.ZECA*/ ZECA_LATCH_OAM_B7.tp_latch(BODE_OAM_LATCHp, OAM_PIN_DB7);

    /*p28.YVAL*/ wire YVAL_OAM_LATCHn  = not(BODE_OAM_LATCHp);
    /*p28.YRYV*/ wire YRYU_OAM_LATCHp  = not(YVAL_OAM_LATCHn);
    /*p28.ZODO*/ wire ZODO_OAM_LATCHn  = not(YRYU_OAM_LATCHp);
    OAM_PIN_OE.set(ZODO_OAM_LATCHn);

    /*p29.WOJO*/ wire WOJO_xxxDxxxH = nor(!top.WUVU_xxCDxxGH(), !top.WOSU_xBCxxFGx());
    /*p29.XYSO*/ wire XYSO_ABCxDEFx = not(WOJO_xxxDxxxH);
    /*p25.AVER*/ wire AVER_SCAN_OAM_CLK = nand(top.ACYL_SCANNINGp(), XYSO_ABCxDEFx); 
    /*p25.XUJY*/ wire XUJY_PPU_OAM_CLK  = not(top.VAPE_FETCH_OAM_CLK());
    /*p25.CUFE*/ wire CUFE_DMA_OAM_CLK  = and (or (SARO_FE00_FEFFp, top.MATU_DMA_RUNNINGp()), top.MOPA_xxxxEFGH());
    /*p25.BYCU*/ wire BYCU_OAM_CLK      = nand(AVER_SCAN_OAM_CLK, XUJY_PPU_OAM_CLK, CUFE_DMA_OAM_CLK);
    /*p25.COTA*/ wire COTA_OAM_CLKn     = not(BYCU_OAM_CLK);

    OAM_PIN_CLK.set(COTA_OAM_CLKn);

    // REG8_01 << CLKp
    // REG8_02 << D
    // REG8_03 nc
    // REG8_04 << CLKn
    // REG8_05 nc
    // REG8_06 nc
    // REG8_07 >> Q
    // REG8_08 >> Qn

    // YLOR = reg8(XEGA, XYKY, !XEGA)

    // Internal latch -> sprite fetcher/matcher

    /*p31.XEGA*/ wire XEGA_OAM_CLKp = not(COTA_OAM_CLKn);
    /*p31.YLOR*/ YLOR_SPRITE_X0.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), XYKY_LATCH_OAM_A0.q());
    /*p31.ZYTY*/ ZYTY_SPRITE_X1.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), YRUM_LATCH_OAM_A1.q());
    /*p31.ZYVE*/ ZYVE_SPRITE_X2.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), YSEX_LATCH_OAM_A2.q());
    /*p31.ZEZY*/ ZEZY_SPRITE_X3.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), YVEL_LATCH_OAM_A3.q());
    /*p31.GOMO*/ GOMO_SPRITE_X4.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), WYNO_LATCH_OAM_A4.q());
    /*p31.BAXO*/ BAXO_SPRITE_X5.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), CYRA_LATCH_OAM_A5.q());
    /*p31.YZOS*/ YZOS_SPRITE_X6.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), ZUVE_LATCH_OAM_A6.q());
    /*p31.DEPO*/ DEPO_SPRITE_X7.set(XEGA_OAM_CLKp, not(XEGA_OAM_CLKp), ECED_LATCH_OAM_A7.q());

    /*p29.YWOK*/ wire YWOK_OAM_CLKn = not(COTA_OAM_CLKn);
    /*p29.XUSO*/ XUSO_SPRITE_Y0.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), YDYV_LATCH_OAM_B0.q());
    /*p29.XEGU*/ XEGU_SPRITE_Y1.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), YCEB_LATCH_OAM_B1.q());
    /*p29.YJEX*/ YJEX_SPRITE_Y2.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), ZUCA_LATCH_OAM_B2.q());
    /*p29.XYJU*/ XYJU_SPRITE_Y3.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), WONE_LATCH_OAM_B3.q());
    /*p29.YBOG*/ YBOG_SPRITE_Y4.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), ZAXE_LATCH_OAM_B4.q());
    /*p29.WYSO*/ WYSO_SPRITE_Y5.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), XAFU_LATCH_OAM_B5.q());
    /*p29.XOTE*/ XOTE_SPRITE_Y6.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), YSES_LATCH_OAM_B6.q());
    /*p29.YZAB*/ YZAB_SPRITE_Y7.set(YWOK_OAM_CLKn, not(YWOK_OAM_CLKn), ZECA_LATCH_OAM_B7.q());
  }

  // CPU bus -> VRAM bus
  {
    /*p04.MUHO*/ wire MUHO_DMA_READ_VRAMn = nand(top.MATU_DMA_RUNNINGp(), top.MUDA_DMA_SRC_VRAMp()); // def nand
    /*p04.LUFA*/ wire LUFA_DMA_READ_VRAMp = not(MUHO_DMA_READ_VRAMn);

    /*p25.XANE*/ wire XANE_VRAM_UNLOCKEDp = nor(LUFA_DMA_READ_VRAMp, top.XYMU_RENDERINGp()); // def nor
    /*p25.XEDU*/ wire XEDU_VRAM_UNLOCKEDn = not(XANE_VRAM_UNLOCKEDp);

    // FIXME polarity

    /*p25.XAKY*/ top.VRM_TRI_A00.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A00);
    /*p25.XUXU*/ top.VRM_TRI_A01.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A01);
    /*p25.XYNE*/ top.VRM_TRI_A02.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A02);
    /*p25.XODY*/ top.VRM_TRI_A03.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A03);
    /*p25.XECA*/ top.VRM_TRI_A04.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A04);
    /*p25.XOBA*/ top.VRM_TRI_A05.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A05);
    /*p25.XOPO*/ top.VRM_TRI_A06.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A06);
    /*p25.XYBO*/ top.VRM_TRI_A07.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A07);
    /*p25.RYSU*/ top.VRM_TRI_A08.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A08);
    /*p25.RESE*/ top.VRM_TRI_A09.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A09);
    /*p25.RUSE*/ top.VRM_TRI_A10.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A10);
    /*p25.RYNA*/ top.VRM_TRI_A11.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A11);
    /*p25.RUMO*/ top.VRM_TRI_A12.set_tribuf_6n(XEDU_VRAM_UNLOCKEDn, top.CPU_PIN_A12);
  }

  // CPU address pins -> address latch
  {
    /*p08.MULE*/ wire MULE_MODE_DBG1n = not(top.UMUT_MODE_DBG1p());
    /*p08.LOXO*/ wire LOXO_LATCH_CPU_ADDRp = or (and (MULE_MODE_DBG1n, TEXO_8000_9FFFn), top.UMUT_MODE_DBG1p());
    /*p08.LASY*/ wire LASY_LATCH_CPU_ADDRn = not(LOXO_LATCH_CPU_ADDRp);
    /*p08.MATE*/ wire MATE_LATCH_CPU_ADDRp = not(LASY_LATCH_CPU_ADDRn);

    /*p08.ALOR*/ CPU_ADDR_LATCH_00.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A00);
    /*p08.APUR*/ CPU_ADDR_LATCH_01.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A01);
    /*p08.ALYR*/ CPU_ADDR_LATCH_02.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A02);
    /*p08.ARET*/ CPU_ADDR_LATCH_03.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A03);
    /*p08.AVYS*/ CPU_ADDR_LATCH_04.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A04);
    /*p08.ATEV*/ CPU_ADDR_LATCH_05.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A05);
    /*p08.AROS*/ CPU_ADDR_LATCH_06.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A06);
    /*p08.ARYM*/ CPU_ADDR_LATCH_07.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A07);
    /*p08.LUNO*/ CPU_ADDR_LATCH_08.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A08);
    /*p08.LYSA*/ CPU_ADDR_LATCH_09.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A09);
    /*p08.PATE*/ CPU_ADDR_LATCH_10.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A10);
    /*p08.LUMY*/ CPU_ADDR_LATCH_11.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A11);
    /*p08.LOBU*/ CPU_ADDR_LATCH_12.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A12);
    /*p08.LONU*/ CPU_ADDR_LATCH_13.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A13);
    /*p08.NYRE*/ CPU_ADDR_LATCH_14.tp_latch(MATE_LATCH_CPU_ADDRp, top.CPU_PIN_A14);
  }

  // DMA address / CPU address latch -> ext addr pins
  {
    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(top.MUDA_DMA_SRC_VRAMp());
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(top.MATU_DMA_RUNNINGp(), LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.AMET*/ wire EXT_ADDR_00 = mux2_p(top.DMA_A00(), CPU_ADDR_LATCH_00.q(), LUMA_DMA_READ_CARTp);
    /*p08.ATOL*/ wire EXT_ADDR_01 = mux2_p(top.DMA_A01(), CPU_ADDR_LATCH_01.q(), LUMA_DMA_READ_CARTp);
    /*p08.APOK*/ wire EXT_ADDR_02 = mux2_p(top.DMA_A02(), CPU_ADDR_LATCH_02.q(), LUMA_DMA_READ_CARTp);
    /*p08.AMER*/ wire EXT_ADDR_03 = mux2_p(top.DMA_A03(), CPU_ADDR_LATCH_03.q(), LUMA_DMA_READ_CARTp);
    /*p08.ATEM*/ wire EXT_ADDR_04 = mux2_p(top.DMA_A04(), CPU_ADDR_LATCH_04.q(), LUMA_DMA_READ_CARTp);
    /*p08.ATOV*/ wire EXT_ADDR_05 = mux2_p(top.DMA_A05(), CPU_ADDR_LATCH_05.q(), LUMA_DMA_READ_CARTp);
    /*p08.ATYR*/ wire EXT_ADDR_06 = mux2_p(top.DMA_A06(), CPU_ADDR_LATCH_06.q(), LUMA_DMA_READ_CARTp);
    /*p08.ASUR*/ wire EXT_ADDR_07 = mux2_p(top.DMA_A07(), CPU_ADDR_LATCH_07.q(), LUMA_DMA_READ_CARTp);
    /*p08.MANO*/ wire EXT_ADDR_08 = mux2_p(top.DMA_A08(), CPU_ADDR_LATCH_08.q(), LUMA_DMA_READ_CARTp);
    /*p08.MASU*/ wire EXT_ADDR_09 = mux2_p(top.DMA_A09(), CPU_ADDR_LATCH_09.q(), LUMA_DMA_READ_CARTp);
    /*p08.PAMY*/ wire EXT_ADDR_10 = mux2_p(top.DMA_A10(), CPU_ADDR_LATCH_10.q(), LUMA_DMA_READ_CARTp);
    /*p08.MALE*/ wire EXT_ADDR_11 = mux2_p(top.DMA_A11(), CPU_ADDR_LATCH_11.q(), LUMA_DMA_READ_CARTp);
    /*p08.MOJY*/ wire EXT_ADDR_12 = mux2_p(top.DMA_A12(), CPU_ADDR_LATCH_12.q(), LUMA_DMA_READ_CARTp);
    /*p08.MUCE*/ wire EXT_ADDR_13 = mux2_p(top.DMA_A13(), CPU_ADDR_LATCH_13.q(), LUMA_DMA_READ_CARTp);
    /*p08.PEGE*/ wire EXT_ADDR_14 = mux2_p(top.DMA_A14(), CPU_ADDR_LATCH_14.q(), LUMA_DMA_READ_CARTp);

    /*p08.KUPO*/ top.EXT_PIN_A00_A.set(nand(EXT_ADDR_00, top.TOVA_MODE_DBG2n()));
    /*p08.CABA*/ top.EXT_PIN_A01_A.set(nand(EXT_ADDR_01, top.TOVA_MODE_DBG2n()));
    /*p08.BOKU*/ top.EXT_PIN_A02_A.set(nand(EXT_ADDR_02, top.TOVA_MODE_DBG2n()));
    /*p08.BOTY*/ top.EXT_PIN_A03_A.set(nand(EXT_ADDR_03, top.TOVA_MODE_DBG2n()));
    /*p08.BYLA*/ top.EXT_PIN_A04_A.set(nand(EXT_ADDR_04, top.TOVA_MODE_DBG2n()));
    /*p08.BADU*/ top.EXT_PIN_A05_A.set(nand(EXT_ADDR_05, top.TOVA_MODE_DBG2n()));
    /*p08.CEPU*/ top.EXT_PIN_A06_A.set(nand(EXT_ADDR_06, top.TOVA_MODE_DBG2n()));
    /*p08.DEFY*/ top.EXT_PIN_A07_A.set(nand(EXT_ADDR_07, top.TOVA_MODE_DBG2n()));
    /*p08.MYNY*/ top.EXT_PIN_A08_A.set(nand(EXT_ADDR_08, top.TOVA_MODE_DBG2n()));
    /*p08.MUNE*/ top.EXT_PIN_A09_A.set(nand(EXT_ADDR_09, top.TOVA_MODE_DBG2n()));
    /*p08.ROXU*/ top.EXT_PIN_A10_A.set(nand(EXT_ADDR_10, top.TOVA_MODE_DBG2n()));
    /*p08.LEPY*/ top.EXT_PIN_A11_A.set(nand(EXT_ADDR_11, top.TOVA_MODE_DBG2n()));
    /*p08.LUCE*/ top.EXT_PIN_A12_A.set(nand(EXT_ADDR_12, top.TOVA_MODE_DBG2n()));
    /*p08.LABE*/ top.EXT_PIN_A13_A.set(nand(EXT_ADDR_13, top.TOVA_MODE_DBG2n()));
    /*p08.PUHE*/ top.EXT_PIN_A14_A.set(nand(EXT_ADDR_14, top.TOVA_MODE_DBG2n()));

    /*p08.KOTY*/ top.EXT_PIN_A00_D.set(nor (EXT_ADDR_00, top.UNOR_MODE_DBG2p()));
    /*p08.COTU*/ top.EXT_PIN_A01_D.set(nor (EXT_ADDR_01, top.UNOR_MODE_DBG2p()));
    /*p08.BAJO*/ top.EXT_PIN_A02_D.set(nor (EXT_ADDR_02, top.UNOR_MODE_DBG2p()));
    /*p08.BOLA*/ top.EXT_PIN_A03_D.set(nor (EXT_ADDR_03, top.UNOR_MODE_DBG2p()));
    /*p08.BEVO*/ top.EXT_PIN_A04_D.set(nor (EXT_ADDR_04, top.UNOR_MODE_DBG2p()));
    /*p08.AJAV*/ top.EXT_PIN_A05_D.set(nor (EXT_ADDR_05, top.UNOR_MODE_DBG2p()));
    /*p08.CYKA*/ top.EXT_PIN_A06_D.set(nor (EXT_ADDR_06, top.UNOR_MODE_DBG2p()));
    /*p08.COLO*/ top.EXT_PIN_A07_D.set(nor (EXT_ADDR_07, top.UNOR_MODE_DBG2p()));
    /*p08.MEGO*/ top.EXT_PIN_A08_D.set(nor (EXT_ADDR_08, top.UNOR_MODE_DBG2p()));
    /*p08.MENY*/ top.EXT_PIN_A09_D.set(nor (EXT_ADDR_09, top.UNOR_MODE_DBG2p()));
    /*p08.RORE*/ top.EXT_PIN_A10_D.set(nor (EXT_ADDR_10, top.UNOR_MODE_DBG2p()));
    /*p08.LYNY*/ top.EXT_PIN_A11_D.set(nor (EXT_ADDR_11, top.UNOR_MODE_DBG2p()));
    /*p08.LOSO*/ top.EXT_PIN_A12_D.set(nor (EXT_ADDR_12, top.UNOR_MODE_DBG2p()));
    /*p08.LEVA*/ top.EXT_PIN_A13_D.set(nor (EXT_ADDR_13, top.UNOR_MODE_DBG2p()));
    /*p08.PAHY*/ top.EXT_PIN_A14_D.set(nor (EXT_ADDR_14, top.UNOR_MODE_DBG2p()));
  }

  // CPU data bus -> external data bus
  {
    // So does this mean that if the CPU writes to the external bus during dma, that data
    // will actually end up in oam?

    /*p08.LAGU*/ wire LAGU = or(and(top.CPU_PIN_RDp, LEVO_8000_9FFFp), top.CPU_PIN_WRp);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor(TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p());
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or(MOCA_DBG_EXT_RD, LYWE);

    /*p08.REDU*/ wire _REDU_CPU_RDn = not(top.TEDO_CPU_RDp());
    /*p08.RORU*/ wire _RORU_IBUS_TO_EBUSn = mux2_p(_REDU_CPU_RDn, MOTY_CPU_EXT_RD, top.UNOR_MODE_DBG2p());
    /*p08.LULA*/ wire _LULA_IBUS_TO_EBUSp = not(_RORU_IBUS_TO_EBUSn);
    top.EXT_PIN_D0_B.set(_LULA_IBUS_TO_EBUSp);
    top.EXT_PIN_D1_B.set(_LULA_IBUS_TO_EBUSp);
    top.EXT_PIN_D2_B.set(_LULA_IBUS_TO_EBUSp);
    top.EXT_PIN_D3_B.set(_LULA_IBUS_TO_EBUSp);
    top.EXT_PIN_D4_B.set(_LULA_IBUS_TO_EBUSp);
    top.EXT_PIN_D5_B.set(_LULA_IBUS_TO_EBUSp);
    top.EXT_PIN_D6_B.set(_LULA_IBUS_TO_EBUSp);
    top.EXT_PIN_D7_B.set(_LULA_IBUS_TO_EBUSp);

    /*p25.RUXA*/ top.EXT_PIN_D0_A.set(nand(top.CPU_TRI_D0.q(), _LULA_IBUS_TO_EBUSp));
    /*p25.RUJA*/ top.EXT_PIN_D1_A.set(nand(top.CPU_TRI_D1.q(), _LULA_IBUS_TO_EBUSp));
    /*p25.RABY*/ top.EXT_PIN_D2_A.set(nand(top.CPU_TRI_D2.q(), _LULA_IBUS_TO_EBUSp));
    /*p25.RERA*/ top.EXT_PIN_D3_A.set(nand(top.CPU_TRI_D3.q(), _LULA_IBUS_TO_EBUSp));
    /*p25.RORY*/ top.EXT_PIN_D4_A.set(nand(top.CPU_TRI_D4.q(), _LULA_IBUS_TO_EBUSp));
    /*p25.RYVO*/ top.EXT_PIN_D5_A.set(nand(top.CPU_TRI_D5.q(), _LULA_IBUS_TO_EBUSp));
    /*p25.RAFY*/ top.EXT_PIN_D6_A.set(nand(top.CPU_TRI_D6.q(), _LULA_IBUS_TO_EBUSp));
    /*p25.RAVU*/ top.EXT_PIN_D7_A.set(nand(top.CPU_TRI_D7.q(), _LULA_IBUS_TO_EBUSp));

    /*p08.RUNE*/ top.EXT_PIN_D0_D.set(nor (top.CPU_TRI_D0.q(), _RORU_IBUS_TO_EBUSn));
    /*p08.RYPU*/ top.EXT_PIN_D1_D.set(nor (top.CPU_TRI_D1.q(), _RORU_IBUS_TO_EBUSn));
    /*p08.SULY*/ top.EXT_PIN_D2_D.set(nor (top.CPU_TRI_D2.q(), _RORU_IBUS_TO_EBUSn));
    /*p08.SEZE*/ top.EXT_PIN_D3_D.set(nor (top.CPU_TRI_D3.q(), _RORU_IBUS_TO_EBUSn));
    /*p08.RESY*/ top.EXT_PIN_D4_D.set(nor (top.CPU_TRI_D4.q(), _RORU_IBUS_TO_EBUSn));
    /*p08.TAMU*/ top.EXT_PIN_D5_D.set(nor (top.CPU_TRI_D5.q(), _RORU_IBUS_TO_EBUSn));
    /*p08.ROGY*/ top.EXT_PIN_D6_D.set(nor (top.CPU_TRI_D6.q(), _RORU_IBUS_TO_EBUSn));
    /*p08.RYDA*/ top.EXT_PIN_D7_D.set(nor (top.CPU_TRI_D7.q(), _RORU_IBUS_TO_EBUSn));
  }

  // External data bus -> CPU data bus
  {
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

    /*p08.LAVO*/ wire LAVO_LATCH_CPU_DATAn = nand(top.CPU_PIN_RDp, TEXO_8000_9FFFn, top.CPU_PIN5);

    /*p08.SOMA*/ SOMA_EXT_DATA_LATCH_00.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D0_C);
    /*p08.RONY*/ RONY_EXT_DATA_LATCH_01.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D1_C);
    /*p08.RAXY*/ RAXY_EXT_DATA_LATCH_02.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D2_C);
    /*p08.SELO*/ SELO_EXT_DATA_LATCH_03.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D3_C);
    /*p08.SODY*/ SODY_EXT_DATA_LATCH_04.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D4_C);
    /*p08.SAGO*/ SAGO_EXT_DATA_LATCH_05.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D5_C);
    /*p08.RUPA*/ RUPA_EXT_DATA_LATCH_06.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D6_C);
    /*p08.SAZY*/ SAZY_EXT_DATA_LATCH_07.tp_latch(LAVO_LATCH_CPU_DATAn, top.EXT_PIN_D7_C);

    /*p08.RYMA*/ top.CPU_TRI_D0.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, SOMA_EXT_DATA_LATCH_00.q());
    /*p08.RUVO*/ top.CPU_TRI_D1.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, RONY_EXT_DATA_LATCH_01.q());
    /*p08.RYKO*/ top.CPU_TRI_D2.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, RAXY_EXT_DATA_LATCH_02.q());
    /*p08.TAVO*/ top.CPU_TRI_D3.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, SELO_EXT_DATA_LATCH_03.q());
    /*p08.TEPE*/ top.CPU_TRI_D4.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, SODY_EXT_DATA_LATCH_04.q());
    /*p08.SAFO*/ top.CPU_TRI_D5.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, SAGO_EXT_DATA_LATCH_05.q());
    /*p08.SEVU*/ top.CPU_TRI_D6.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, RUPA_EXT_DATA_LATCH_06.q());
    /*p08.TAJU*/ top.CPU_TRI_D7.set_tribuf_6n(LAVO_LATCH_CPU_DATAn, SAZY_EXT_DATA_LATCH_07.q());
  }

  {
    // This whole section is screwy
    // Where did AGUT go?
    // Where did AJAX go?

    // AJAX = not(ATYP)
    // AGUT = nor(AROV, AJAX)
    // AGUT looks unused

    /*p??.APAP*/ wire APAP_ADDR_VALIDp = not(top.CPU_PIN_ADDR_VALID); // Missing from schematic
    /*p01.AWOD*/ wire AWOD_ADDR_VALIDn = nor(top.UNOR_MODE_DBG2p(), APAP_ADDR_VALIDp);
    /*p01.ABUZ*/ wire ABUZ_ADDR_VALIDp = not(AWOD_ADDR_VALIDn);

    /*p07.TERA*/ wire TERA_BOOT_BITp  = not(top.BOOT_BITn());
    /*p07.TULO*/ wire TULO_ADDR_00XXp = nor(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09, top.CPU_PIN_A08);
    /*p07.TUTU*/ wire TUTU_ADDR_BOOTp = and (TERA_BOOT_BITp, TULO_ADDR_00XXp);

    /*p08.SOBY*/ wire SOBY_A15n = nor(top.CPU_PIN_A15, TUTU_ADDR_BOOTp);
    /*p08.SEPY*/ wire SEPY_A15p = nand(ABUZ_ADDR_VALIDp, SOBY_A15n);

    /*p04.LOGO*/ wire LOGO_DMA_SRC_VRAMn  = not(top.MUDA_DMA_SRC_VRAMp());
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(top.MATU_DMA_RUNNINGp(), LOGO_DMA_SRC_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.TAZY*/ wire TAZY_A15p = mux2_p(top.DMA_A15(), SEPY_A15p, LUMA_DMA_READ_CARTp);

    /*p08.RYCA*/ wire RYCA_MODE_DBG2n = not(top.UNOR_MODE_DBG2p());
    /*p08.SUZE*/ top.EXT_PIN_A15_A.set(nand(TAZY_A15p, RYCA_MODE_DBG2n));
    /*p08.RULO*/ top.EXT_PIN_A15_D.set(nor (TAZY_A15p, top.UNOR_MODE_DBG2p()));
  }

  {
    /*p08.LAGU*/ wire LAGU = or(and(top.CPU_PIN_RDp, LEVO_8000_9FFFp), top.CPU_PIN_WRp);
    /*p08.LYWE*/ wire LYWE = not(LAGU);
    /*p08.MOCA*/ wire MOCA_DBG_EXT_RD = nor(TEXO_8000_9FFFn, top.UMUT_MODE_DBG1p());
    /*p08.MOTY*/ wire MOTY_CPU_EXT_RD = or(MOCA_DBG_EXT_RD, LYWE);

    /*p04.LOGO*/ wire LOGO_DMA_VRAMn      = not(top.MUDA_DMA_SRC_VRAMp());
    /*p04.MORY*/ wire MORY_DMA_READ_CARTn = nand(top.MATU_DMA_RUNNINGp(), LOGO_DMA_VRAMn);
    /*p04.LUMA*/ wire LUMA_DMA_READ_CARTp = not(MORY_DMA_READ_CARTn);

    /*p08.NEVY*/ wire NEVY = or(MEXO_CPU_WRn_ABCDxxxH, MOCA_DBG_EXT_RD);
    /*p08.TYMU*/ wire TYMU_RD_OUTn = nor(LUMA_DMA_READ_CARTp, MOTY_CPU_EXT_RD);
    /*p08.PUVA*/ wire PUVA_WR_OUTn = or(NEVY, LUMA_DMA_READ_CARTp);

    /*p08.UGAC*/ wire _UGAC_RDp_A = nand(TYMU_RD_OUTn, top.TOVA_MODE_DBG2n());
    /*p08.URUN*/ wire _URUN_RDp_D = nor (TYMU_RD_OUTn, top.UNOR_MODE_DBG2p());
    /* PIN_79 */ top.EXT_PIN_RDn_A.set(_UGAC_RDp_A);
    /* PIN_79 */ top.EXT_PIN_RDn_D.set(_URUN_RDp_D);

    /*p08.UVER*/ wire _UVER_WRp_A = nand(PUVA_WR_OUTn, top.TOVA_MODE_DBG2n());
    /*p08.USUF*/ wire _USUF_WRp_D = nor (PUVA_WR_OUTn, top.UNOR_MODE_DBG2p());
    /* PIN_78 */ top.EXT_PIN_WRn_A.set(_UVER_WRp_A);
    /* PIN_78 */ top.EXT_PIN_WRn_D.set(_USUF_WRp_D);

    // if TYNU is and(or()) things don't make sense.
    ///*p08.TYNU*/ wire TYNU_ADDR_RAM = and(ADDR >= 0x4000, TUMA_CART_RAM);

    // Die trace:
    // TOZA = and(TYNU, ABUZ, TUNA);
    // TYHO = mux2_p(LUMA, MARU.QN?, TOZA);

    // TOZA = address valid, address ram, address not highmem
    // The A15 in the other half of the mux is weird.

    /*p08.SOGY*/ wire _SOGY_A14n = not(top.CPU_PIN_A14);
    /*p08.TUMA*/ wire _TUMA_CART_RAM = and(top.CPU_PIN_A13, _SOGY_A14n, top.CPU_PIN_A15);

    // TYNU 5-rung
    // TYNU01

    // still not sure what this is...
    /*p??.APAP*/ wire APAP = not(top.CPU_PIN_ADDR_VALID); // Missing from schematic
    /*p01.AWOD*/ wire AWOD = nor(top.UNOR_MODE_DBG2p(), APAP);
    /*p01.ABUZ*/ wire ABUZ = not(AWOD);

    /*p08.TYNU*/ wire _TYNU_ADDR_RAM = or(and(top.CPU_PIN_A15, top.CPU_PIN_A14), _TUMA_CART_RAM);
    /*p08.TOZA*/ wire _TOZA = and(ABUZ, _TYNU_ADDR_RAM, top.TUNA_0000_FDFFp()); // suggests ABUZp

    /*p08.TYHO*/ wire TYHO_CS_A = mux2_p(top.DMA_A15(), _TOZA, LUMA_DMA_READ_CARTp); // ABxxxxxx

    /* PIN_80 */ top.EXT_PIN_CSn_A.set(TYHO_CS_A);
  }
}

//------------------------------------------------------------------------------

SignalHash BusMux::commit(SchematicTop& top) {
  SignalHash hash;

  hash << MAKA_FROM_CPU5_SYNC.commit();

  hash << CPU_ADDR_LATCH_00.commit();
  hash << CPU_ADDR_LATCH_01.commit();
  hash << CPU_ADDR_LATCH_02.commit();
  hash << CPU_ADDR_LATCH_03.commit();
  hash << CPU_ADDR_LATCH_04.commit();
  hash << CPU_ADDR_LATCH_05.commit();
  hash << CPU_ADDR_LATCH_06.commit();
  hash << CPU_ADDR_LATCH_07.commit();
  hash << CPU_ADDR_LATCH_08.commit();
  hash << CPU_ADDR_LATCH_09.commit();
  hash << CPU_ADDR_LATCH_10.commit();
  hash << CPU_ADDR_LATCH_11.commit();
  hash << CPU_ADDR_LATCH_12.commit();
  hash << CPU_ADDR_LATCH_13.commit();
  hash << CPU_ADDR_LATCH_14.commit();

  hash << SOMA_EXT_DATA_LATCH_00.commit();
  hash << RONY_EXT_DATA_LATCH_01.commit();
  hash << RAXY_EXT_DATA_LATCH_02.commit();
  hash << SELO_EXT_DATA_LATCH_03.commit();
  hash << SODY_EXT_DATA_LATCH_04.commit();
  hash << SAGO_EXT_DATA_LATCH_05.commit();
  hash << RUPA_EXT_DATA_LATCH_06.commit();
  hash << SAZY_EXT_DATA_LATCH_07.commit();

  hash << YLOR_SPRITE_X0.commit();
  hash << ZYTY_SPRITE_X1.commit();
  hash << ZYVE_SPRITE_X2.commit();
  hash << ZEZY_SPRITE_X3.commit();
  hash << GOMO_SPRITE_X4.commit();
  hash << BAXO_SPRITE_X5.commit();
  hash << YZOS_SPRITE_X6.commit();
  hash << DEPO_SPRITE_X7.commit();

  hash << XUSO_SPRITE_Y0.commit();
  hash << XEGU_SPRITE_Y1.commit();
  hash << YJEX_SPRITE_Y2.commit();
  hash << XYJU_SPRITE_Y3.commit();
  hash << YBOG_SPRITE_Y4.commit();
  hash << WYSO_SPRITE_Y5.commit();
  hash << XOTE_SPRITE_Y6.commit();
  hash << YZAB_SPRITE_Y7.commit();

  //-----------------------------------------------------------------------------
  // OAM bus

  hash << OAM_PIN_CLK.commit();
  hash << OAM_PIN_OE.commit();
  hash << OAM_PIN_WR_A.commit();
  hash << OAM_PIN_WR_B.commit();

  hash << OAM_PIN_A0.commit();
  hash << OAM_PIN_A1.commit();
  hash << OAM_PIN_A2.commit();
  hash << OAM_PIN_A3.commit();
  hash << OAM_PIN_A4.commit();
  hash << OAM_PIN_A5.commit();
  hash << OAM_PIN_A6.commit();
  hash << OAM_PIN_A7.commit();

  hash << OAM_PIN_DA0.commit();
  hash << OAM_PIN_DA1.commit();
  hash << OAM_PIN_DA2.commit();
  hash << OAM_PIN_DA3.commit();
  hash << OAM_PIN_DA4.commit();
  hash << OAM_PIN_DA5.commit();
  hash << OAM_PIN_DA6.commit();
  hash << OAM_PIN_DA7.commit();
  hash << OAM_PIN_DB0.commit();
  hash << OAM_PIN_DB1.commit();
  hash << OAM_PIN_DB2.commit();
  hash << OAM_PIN_DB3.commit();
  hash << OAM_PIN_DB4.commit();
  hash << OAM_PIN_DB5.commit();
  hash << OAM_PIN_DB6.commit();
  hash << OAM_PIN_DB7.commit();

  hash << GEKA_OAM_A0p.commit();
  hash << ZYFO_OAM_A1p.commit();
  hash << YFOT_OAM_A2p.commit();
  hash << YFOC_OAM_A3p.commit();
  hash << YVOM_OAM_A4p.commit();
  hash << YMEV_OAM_A5p.commit();
  hash << XEMU_OAM_A6p.commit();
  hash << YZET_OAM_A7p.commit();

  hash << XYKY_LATCH_OAM_A0.commit();
  hash << YRUM_LATCH_OAM_A1.commit();
  hash << YSEX_LATCH_OAM_A2.commit();
  hash << YVEL_LATCH_OAM_A3.commit();
  hash << WYNO_LATCH_OAM_A4.commit();
  hash << CYRA_LATCH_OAM_A5.commit();
  hash << ZUVE_LATCH_OAM_A6.commit();
  hash << ECED_LATCH_OAM_A7.commit();

  hash << YDYV_LATCH_OAM_B0.commit();
  hash << YCEB_LATCH_OAM_B1.commit();
  hash << ZUCA_LATCH_OAM_B2.commit();
  hash << WONE_LATCH_OAM_B3.commit();
  hash << ZAXE_LATCH_OAM_B4.commit();
  hash << XAFU_LATCH_OAM_B5.commit();
  hash << YSES_LATCH_OAM_B6.commit();
  hash << ZECA_LATCH_OAM_B7.commit();

  //-----------------------------------------------------------------------------

  hash << top.EXT_PIN_RDn_A.commit();    // PIN_79 <- UGAC
  hash << top.EXT_PIN_RDn_D.commit();    // PIN_79 <- URUN
  hash << top.EXT_PIN_WRn_A.commit();    // PIN_78 <- UVER
  hash << top.EXT_PIN_WRn_D.commit();    // PIN_78 <- USUF
  hash << top.EXT_PIN_CSn_A.commit();    // PIN_80 <- TYHO

  hash << top.EXT_PIN_A00_A.commit();    // PIN_01 <- KUPO
  hash << top.EXT_PIN_A00_D.commit();    // PIN_01 <- KOTY
  hash << top.EXT_PIN_A01_A.commit();    // PIN_02 <- CABA
  hash << top.EXT_PIN_A01_D.commit();    // PIN_02 <- COTU
  hash << top.EXT_PIN_A02_A.commit();    // PIN_03 <- BOKU
  hash << top.EXT_PIN_A02_D.commit();    // PIN_03 <- BAJO
  hash << top.EXT_PIN_A03_A.commit();    // PIN_04 <- BOTY
  hash << top.EXT_PIN_A03_D.commit();    // PIN_04 <- BOLA
  hash << top.EXT_PIN_A04_A.commit();    // PIN_05 <- BYLA
  hash << top.EXT_PIN_A04_D.commit();    // PIN_05 <- BEVO
  hash << top.EXT_PIN_A05_A.commit();    // PIN_06 <- BADU
  hash << top.EXT_PIN_A05_D.commit();    // PIN_06 <- AJAV
  hash << top.EXT_PIN_A06_A.commit();    // PIN_07 <- CEPU
  hash << top.EXT_PIN_A06_D.commit();    // PIN_07 <- CYKA
  hash << top.EXT_PIN_A07_A.commit();    // PIN_08 <- DEFY
  hash << top.EXT_PIN_A07_D.commit();    // PIN_08 <- COLO
  hash << top.EXT_PIN_A08_A.commit();    // PIN_09 <- MYNY
  hash << top.EXT_PIN_A08_D.commit();    // PIN_09 <- MEGO
  hash << top.EXT_PIN_A09_A.commit();    // PIN_10 <- MUNE
  hash << top.EXT_PIN_A09_D.commit();    // PIN_10 <- MENY
  hash << top.EXT_PIN_A10_A.commit();    // PIN_11 <- ROXU
  hash << top.EXT_PIN_A10_D.commit();    // PIN_11 <- RORE
  hash << top.EXT_PIN_A11_A.commit();    // PIN_12 <- LEPY
  hash << top.EXT_PIN_A11_D.commit();    // PIN_12 <- LYNY
  hash << top.EXT_PIN_A12_A.commit();    // PIN_13 <- LUCE
  hash << top.EXT_PIN_A12_D.commit();    // PIN_13 <- LOSO
  hash << top.EXT_PIN_A13_A.commit();    // PIN_14 <- LABE
  hash << top.EXT_PIN_A13_D.commit();    // PIN_14 <- LEVA
  hash << top.EXT_PIN_A14_A.commit();    // PIN_15 <- PUHE
  hash << top.EXT_PIN_A14_D.commit();    // PIN_15 <- PAHY
  hash << top.EXT_PIN_A15_A.commit();    // PIN_16 <- SUZE
  hash << top.EXT_PIN_A15_D.commit();    // PIN_16 <- RULO

  hash << top.EXT_PIN_D0_A.commit();     // PIN_17 <- RUXA
  hash << top.EXT_PIN_D0_B.commit();     // PIN_17 <- LULA
  hash << top.EXT_PIN_D0_D.commit();     // PIN_17 <- RUNE
  hash << top.EXT_PIN_D1_A.commit();     // PIN_18 <- RUJA
  hash << top.EXT_PIN_D1_B.commit();     // PIN_18 <- LULA
  hash << top.EXT_PIN_D1_D.commit();     // PIN_18 <- RYPU
  hash << top.EXT_PIN_D2_A.commit();     // PIN_19 <- RABY
  hash << top.EXT_PIN_D2_B.commit();     // PIN_19 <- LULA
  hash << top.EXT_PIN_D2_D.commit();     // PIN_19 <- SULY
  hash << top.EXT_PIN_D3_A.commit();     // PIN_20 <- RERA
  hash << top.EXT_PIN_D3_B.commit();     // PIN_20 <- LULA
  hash << top.EXT_PIN_D3_D.commit();     // PIN_20 <- SEZE
  hash << top.EXT_PIN_D4_A.commit();     // PIN_21 <- RORY
  hash << top.EXT_PIN_D4_B.commit();     // PIN_21 <- LULA
  hash << top.EXT_PIN_D4_D.commit();     // PIN_21 <- RESY
  hash << top.EXT_PIN_D5_A.commit();     // PIN_22 <- RYVO
  hash << top.EXT_PIN_D5_B.commit();     // PIN_22 <- LULA
  hash << top.EXT_PIN_D5_D.commit();     // PIN_22 <- TAMU
  hash << top.EXT_PIN_D6_A.commit();     // PIN_23 <- RAFY
  hash << top.EXT_PIN_D6_B.commit();     // PIN_23 <- LULA
  hash << top.EXT_PIN_D6_D.commit();     // PIN_23 <- ROGY
  hash << top.EXT_PIN_D7_A.commit();     // PIN_24 <- RAVU
  hash << top.EXT_PIN_D7_B.commit();     // PIN_24 <- LULA
  hash << top.EXT_PIN_D7_D.commit();     // PIN_24 <- RYDA

  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_pins(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_PINS -----\n");
  text_painter.dprintf("PIN_CLK %d\n", PIN_CLK.a.val);
  text_painter.dprintf("PIN_OE    %d\n", PIN_OE.a.val);
  text_painter.dprintf("EXT_PIN_WRn_A  %d\n", EXT_PIN_WRn_A.a.val);
  text_painter.dprintf("PIN_WR_B  %d\n", PIN_WR_B.a.val);

  text_painter.add_text("Axx   ");
  dump2(text_painter, PIN_A7.a);
  dump2(text_painter, PIN_A6.a);
  dump2(text_painter, PIN_A5.a);
  dump2(text_painter, PIN_A4.a);
  dump2(text_painter, PIN_A3.a);
  dump2(text_painter, PIN_A2.a);
  dump2(text_painter, PIN_A1.a);
  dump2(text_painter, PIN_A0.a);
  text_painter.newline();

  text_painter.add_text("A_Dx  ");
  dump2(text_painter, PIN_DA7.a);
  dump2(text_painter, PIN_DA6.a);
  dump2(text_painter, PIN_DA5.a);
  dump2(text_painter, PIN_DA4.a);
  dump2(text_painter, PIN_DA3.a);
  dump2(text_painter, PIN_DA2.a);
  dump2(text_painter, PIN_DA1.a);
  dump2(text_painter, PIN_DA0.a);
  text_painter.newline();

  text_painter.add_text("B_Dx  ");
  dump2(text_painter, PIN_DB7.a);
  dump2(text_painter, PIN_DB6.a);
  dump2(text_painter, PIN_DB5.a);
  dump2(text_painter, PIN_DB4.a);
  dump2(text_painter, PIN_DB3.a);
  dump2(text_painter, PIN_DB2.a);
  dump2(text_painter, PIN_DB1.a);
  dump2(text_painter, PIN_DB0.a);
  text_painter.newline();

  text_painter.newline();
}

void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- OAM_REG -----\n");
  text_painter.dprintf("LATCH_A 0x%02x\n", pack(XYKY_LATCH_OAM_A0.q(), YRUM_LATCH_OAM_A1.q(), YSEX_LATCH_OAM_A2.q(), YVEL_LATCH_OAM_A3.q(), WYNO_LATCH_OAM_A4.q(), CYRA_LATCH_OAM_A5.q(), ZUVE_LATCH_OAM_A6.q(), ECED_LATCH_OAM_A7.q()));
  text_painter.dprintf("LATCH_B 0x%02x\n", pack(YDYV_LATCH_OAM_B0.q(), YCEB_LATCH_OAM_B1.q(), ZUCA_LATCH_OAM_B2.q(), WONE_LATCH_OAM_B3.q(), ZAXE_LATCH_OAM_B4.q(), XAFU_LATCH_OAM_B5.q(), YSES_LATCH_OAM_B6.q(), ZECA_LATCH_OAM_B7.q()));
  text_painter.dprintf("REG_A   0x%02x\n", pack(YLOR_SPRITE_X0.q(), ZYTY_SPRITE_X1.q(), ZYVE_SPRITE_X2.q(), ZEZY_SPRITE_X3.q(), GOMO_SPRITE_X4.q(), BAXO_SPRITE_X5.q(), YZOS_SPRITE_X6.q(), DEPO_SPRITE_X7.q()));
  text_painter.dprintf("REG_B   0x%02x\n", pack(XUSO_SPRITE_Y0.q(), XEGU_SPRITE_Y1.q(), YJEX_SPRITE_Y2.q(), XYJU_SPRITE_Y3.q(), YBOG_SPRITE_Y4.q(), WYSO_SPRITE_Y5.q(), XOTE_SPRITE_Y6.q(), YZAB_SPRITE_Y7.q()));
  text_painter.newline();
}

#endif