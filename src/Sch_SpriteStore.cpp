#include "Sch_SpriteStore.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

// XECU01 nc
// XECU02 << WEFE02
// XECU03 << YZET02 (oam a7)
// XECU04 nc
// XECU05 << WUDA03
// XECU06 ==
// XECU07 ==
// XECU08 << CYKE01???? - is this a dual-edge ff? I guess that would make it latch the address every tcycle?
// XECU09 << WEFE02
// XECU10 ==
// XECU11 nc
// XECU12 >> WEZA04
// XECU13 >> nc

// BUZA04 = and(CENO16, XYMU03)

// WEZA01 << BUZA04
// WEZA02 ==
// WEZA03 ==
// WEZA04 << XECU12
// WEZA05 nc
// WEZA06 

// Arms on ground, nor latch
// BESU01 << CATU17
// BESU02 nc
// BESU03 >> nc
// BESU04 >> ACYL02
// BESU05 nc
// BESU06 << ASEN04

// When CATU goes high, BESU goes high
// When ASEN goes high, BESU goes low.

SpriteStoreSignals SpriteStoreRegisters::sig(const TestGB& gb) const {
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);

  /*p23.XYLO*/ wire LCDC_SPEN = gb.cfg_reg.LCDC_SPEN.q();
  /*p21.XYMU*/ wire XYMU_RENDERINGp = gb.ppu_reg.XYMU_RENDERINGp;

  /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(lcd_sig.ATEJ_VID_LINE_TRIG_d4p, rst_sig.ATAR_VID_RSTp);
  /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
  /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or (BALU_SCAN_RST, SCAN_DONE_d5.q(), !SCAN_DONE_d4.q());
  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIGp = not(SCAN_DONE_d0_TRIGn);

  /*p21.ACAM*/ wire X0n = not(gb.ppu_reg.SAXO_X0);
  /*p21.AZUB*/ wire X1n = not(gb.ppu_reg.TYPO_X1);
  /*p21.AMEL*/ wire X2n = not(gb.ppu_reg.VYZO_X2);
  /*p21.AHAL*/ wire X3n = not(gb.ppu_reg.TELU_X3);
  /*p21.APUX*/ wire X4n = not(gb.ppu_reg.SUDE_X4);
  /*p21.ABEF*/ wire X5n = not(gb.ppu_reg.TAHA_X5);
  /*p21.ADAZ*/ wire X6n = not(gb.ppu_reg.TYRY_X6);
  /*p21.ASAH*/ wire X7n = not(gb.ppu_reg.SYBE_X7);

  /*p31.ZOGY*/ wire STORE0_MATCH0n = xor (STORE0_X0, X0n);
  /*p31.ZEBA*/ wire STORE0_MATCH1n = xor (STORE0_X1, X1n);
  /*p31.ZAHA*/ wire STORE0_MATCH2n = xor (STORE0_X2, X2n);
  /*p31.ZOKY*/ wire STORE0_MATCH3n = xor (STORE0_X3, X3n);
  /*p31.WOJU*/ wire STORE0_MATCH4n = xor (STORE0_X4, X4n);
  /*p31.YFUN*/ wire STORE0_MATCH5n = xor (STORE0_X5, X5n);
  /*p31.WYZA*/ wire STORE0_MATCH6n = xor (STORE0_X6, X6n);
  /*p31.YPUK*/ wire STORE0_MATCH7n = xor (STORE0_X7, X7n);

  /*p31.EDYM*/ wire STORE1_MATCH0n = xor (STORE1_X0, X0n);
  /*p31.EMYB*/ wire STORE1_MATCH1n = xor (STORE1_X1, X1n);
  /*p31.EBEF*/ wire STORE1_MATCH2n = xor (STORE1_X2, X2n);
  /*p31.EWOK*/ wire STORE1_MATCH3n = xor (STORE1_X3, X3n);
  /*p31.COLA*/ wire STORE1_MATCH4n = xor (STORE1_X4, X4n);
  /*p31.BOBA*/ wire STORE1_MATCH5n = xor (STORE1_X5, X5n);
  /*p31.COLU*/ wire STORE1_MATCH6n = xor (STORE1_X6, X6n);
  /*p31.BAHU*/ wire STORE1_MATCH7n = xor (STORE1_X7, X7n);

  /*p31.FUZU*/ wire STORE2_MATCH0n = xor (STORE2_X0, X0n);
  /*p31.FESO*/ wire STORE2_MATCH1n = xor (STORE2_X1, X1n);
  /*p31.FOKY*/ wire STORE2_MATCH2n = xor (STORE2_X2, X2n);
  /*p31.FYVA*/ wire STORE2_MATCH3n = xor (STORE2_X3, X3n);
  /*p31.CEKO*/ wire STORE2_MATCH4n = xor (STORE2_X4, X4n);
  /*p31.DETY*/ wire STORE2_MATCH5n = xor (STORE2_X5, X5n);
  /*p31.DOZO*/ wire STORE2_MATCH6n = xor (STORE2_X6, X6n);
  /*p31.CONY*/ wire STORE2_MATCH7n = xor (STORE2_X7, X7n);

  /*p31.YHOK*/ wire STORE3_MATCH0n = xor (STORE3_X0, X0n);
  /*p31.YCAH*/ wire STORE3_MATCH1n = xor (STORE3_X1, X1n);
  /*p31.YDAJ*/ wire STORE3_MATCH2n = xor (STORE3_X2, X2n);
  /*p31.YVUZ*/ wire STORE3_MATCH3n = xor (STORE3_X3, X3n);
  /*p31.YVAP*/ wire STORE3_MATCH4n = xor (STORE3_X4, X4n);
  /*p31.XENY*/ wire STORE3_MATCH5n = xor (STORE3_X5, X5n);
  /*p31.XAVU*/ wire STORE3_MATCH6n = xor (STORE3_X6, X6n);
  /*p31.XEVA*/ wire STORE3_MATCH7n = xor (STORE3_X7, X7n);

  /*p31.XEJU*/ wire STORE4_MATCH0n = xor (STORE4_X0, X0n);
  /*p31.ZATE*/ wire STORE4_MATCH1n = xor (STORE4_X1, X1n);
  /*p31.ZAKU*/ wire STORE4_MATCH2n = xor (STORE4_X2, X2n);
  /*p31.YBOX*/ wire STORE4_MATCH3n = xor (STORE4_X3, X3n);
  /*p31.ZYKU*/ wire STORE4_MATCH4n = xor (STORE4_X4, X4n);
  /*p31.ZYPU*/ wire STORE4_MATCH5n = xor (STORE4_X5, X5n);
  /*p31.XAHA*/ wire STORE4_MATCH6n = xor (STORE4_X6, X6n);
  /*p31.ZEFE*/ wire STORE4_MATCH7n = xor (STORE4_X7, X7n);

  /*p31.GUZO*/ wire STORE5_MATCH0n = xor (STORE5_X0, X0n);
  /*p31.GOLA*/ wire STORE5_MATCH1n = xor (STORE5_X1, X1n);
  /*p31.GEVE*/ wire STORE5_MATCH2n = xor (STORE5_X2, X2n);
  /*p31.GUDE*/ wire STORE5_MATCH3n = xor (STORE5_X3, X3n);
  /*p31.BAZY*/ wire STORE5_MATCH4n = xor (STORE5_X4, X4n);
  /*p31.CYLE*/ wire STORE5_MATCH5n = xor (STORE5_X5, X5n);
  /*p31.CEVA*/ wire STORE5_MATCH6n = xor (STORE5_X6, X6n);
  /*p31.BUMY*/ wire STORE5_MATCH7n = xor (STORE5_X7, X7n);

  /*p31.XOSU*/ wire STORE6_MATCH0n = xor (STORE6_X0, X0n);
  /*p31.ZUVU*/ wire STORE6_MATCH1n = xor (STORE6_X1, X1n);
  /*p31.XUCO*/ wire STORE6_MATCH2n = xor (STORE6_X2, X2n);
  /*p31.ZULO*/ wire STORE6_MATCH3n = xor (STORE6_X3, X3n);
  /*p31.ZARE*/ wire STORE6_MATCH4n = xor (STORE6_X4, X4n);
  /*p31.ZEMU*/ wire STORE6_MATCH5n = xor (STORE6_X5, X5n);
  /*p31.ZYGO*/ wire STORE6_MATCH6n = xor (STORE6_X6, X6n);
  /*p31.ZUZY*/ wire STORE6_MATCH7n = xor (STORE6_X7, X7n);

  /*p31.DUSE*/ wire STORE7_MATCH0n = xor (STORE7_X0, X0n);
  /*p31.DAGU*/ wire STORE7_MATCH1n = xor (STORE7_X1, X1n);
  /*p31.DYZE*/ wire STORE7_MATCH2n = xor (STORE7_X2, X2n);
  /*p31.DESO*/ wire STORE7_MATCH3n = xor (STORE7_X3, X3n);
  /*p31.EJOT*/ wire STORE7_MATCH4n = xor (STORE7_X4, X4n);
  /*p31.ESAJ*/ wire STORE7_MATCH5n = xor (STORE7_X5, X5n);
  /*p31.DUCU*/ wire STORE7_MATCH6n = xor (STORE7_X6, X6n);
  /*p31.EWUD*/ wire STORE7_MATCH7n = xor (STORE7_X7, X7n);

  /*p31.DUZE*/ wire STORE8_MATCH0n = xor (STORE8_X0, X4n);
  /*p31.DAGA*/ wire STORE8_MATCH1n = xor (STORE8_X1, X5n);
  /*p31.DAWU*/ wire STORE8_MATCH2n = xor (STORE8_X2, X6n);
  /*p31.EJAW*/ wire STORE8_MATCH3n = xor (STORE8_X3, X7n);
  /*p31.GOHO*/ wire STORE8_MATCH4n = xor (STORE8_X4, X0n);
  /*p31.GASU*/ wire STORE8_MATCH5n = xor (STORE8_X5, X1n);
  /*p31.GABU*/ wire STORE8_MATCH6n = xor (STORE8_X6, X2n);
  /*p31.GAFE*/ wire STORE8_MATCH7n = xor (STORE8_X7, X3n);

  /*p31.YMAM*/ wire STORE9_MATCH0n = xor (STORE9_X0, X0n);
  /*p31.YTYP*/ wire STORE9_MATCH1n = xor (STORE9_X1, X1n);
  /*p31.YFOP*/ wire STORE9_MATCH2n = xor (STORE9_X2, X2n);
  /*p31.YVAC*/ wire STORE9_MATCH3n = xor (STORE9_X3, X3n);
  /*p31.ZYWU*/ wire STORE9_MATCH4n = xor (STORE9_X4, X4n);
  /*p31.ZUZA*/ wire STORE9_MATCH5n = xor (STORE9_X5, X5n);
  /*p31.ZEJO*/ wire STORE9_MATCH6n = xor (STORE9_X6, X6n);
  /*p31.ZEDA*/ wire STORE9_MATCH7n = xor (STORE9_X7, X7n);

  /*p31.ZAKO*/ wire STORE0_MATCHA = nor(STORE0_MATCH0n, STORE0_MATCH1n, STORE0_MATCH2n, STORE0_MATCH3n);
  /*p31.XEBA*/ wire STORE0_MATCHB = nor(STORE0_MATCH4n, STORE0_MATCH5n, STORE0_MATCH6n, STORE0_MATCH7n);
  /*p31.CYVY*/ wire STORE1_MATCHB = nor(STORE1_MATCH4n, STORE1_MATCH5n, STORE1_MATCH6n, STORE1_MATCH7n);
  /*p31.EWAM*/ wire STORE1_MATCHA = nor(STORE1_MATCH0n, STORE1_MATCH1n, STORE1_MATCH2n, STORE1_MATCH3n);
  /*p31.CEHU*/ wire STORE2_MATCHA = nor(STORE2_MATCH4n, STORE2_MATCH5n, STORE2_MATCH6n, STORE2_MATCH7n);
  /*p31.EKES*/ wire STORE2_MATCHB = nor(STORE2_MATCH0n, STORE2_MATCH1n, STORE2_MATCH2n, STORE2_MATCH3n);
  /*p31.ZURE*/ wire STORE3_MATCHA = nor(STORE3_MATCH0n, STORE3_MATCH1n, STORE3_MATCH2n, STORE3_MATCH3n);
  /*p31.YWOS*/ wire STORE3_MATCHB = nor(STORE3_MATCH4n, STORE3_MATCH5n, STORE3_MATCH6n, STORE3_MATCH7n);
  /*p31.YKOK*/ wire STORE4_MATCHA = nor(STORE4_MATCH0n, STORE4_MATCH1n, STORE4_MATCH2n, STORE4_MATCH3n);
  /*p31.YNAZ*/ wire STORE4_MATCHB = nor(STORE4_MATCH4n, STORE4_MATCH5n, STORE4_MATCH6n, STORE4_MATCH7n);
  /*p31.COGY*/ wire STORE5_MATCHA = nor(STORE5_MATCH4n, STORE5_MATCH5n, STORE5_MATCH6n, STORE5_MATCH7n);
  /*p31.FYMA*/ wire STORE5_MATCHB = nor(STORE5_MATCH0n, STORE5_MATCH1n, STORE5_MATCH2n, STORE5_MATCH3n);
  /*p31.YWAP*/ wire STORE6_MATCHA = nor(STORE6_MATCH4n, STORE6_MATCH5n, STORE6_MATCH6n, STORE6_MATCH7n);
  /*p31.YDOT*/ wire STORE6_MATCHB = nor(STORE6_MATCH0n, STORE6_MATCH1n, STORE6_MATCH2n, STORE6_MATCH3n);
  /*p31.CYCO*/ wire STORE7_MATCHA = nor(STORE7_MATCH0n, STORE7_MATCH1n, STORE7_MATCH2n, STORE7_MATCH3n);
  /*p31.DAJE*/ wire STORE7_MATCHB = nor(STORE7_MATCH4n, STORE7_MATCH5n, STORE7_MATCH6n, STORE7_MATCH7n);
  /*p31.DAMA*/ wire STORE8_MATCHA = nor(STORE8_MATCH0n, STORE8_MATCH1n, STORE8_MATCH2n, STORE8_MATCH3n);
  /*p31.FEHA*/ wire STORE8_MATCHB = nor(STORE8_MATCH4n, STORE8_MATCH5n, STORE8_MATCH6n, STORE8_MATCH7n);
  /*p31.YLEV*/ wire STORE9_MATCHA = nor(STORE9_MATCH0n, STORE9_MATCH1n, STORE9_MATCH2n, STORE9_MATCH3n);
  /*p31.YTUB*/ wire STORE9_MATCHB = nor(STORE9_MATCH4n, STORE9_MATCH5n, STORE9_MATCH6n, STORE9_MATCH7n);

  /*p29.CEHA*/ wire CEHA_SCANNINGp = not(!CENO_SCANNINGp);
  /*p29.BYJO*/ wire BYJO_SCANNINGn = not(CEHA_SCANNINGp);
  /*p29.AZEM*/ wire AZEM_RENDERINGp = and (BYJO_SCANNINGn, XYMU_RENDERINGp);
  /*p29.AROR*/ wire AROR_MATCH_ENp = and (AZEM_RENDERINGp, LCDC_SPEN);

  /*p29.YDUG*/ wire STORE0_MATCHn = nand(AROR_MATCH_ENp, STORE0_MATCHB, STORE0_MATCHA);
  /*p29.DYDU*/ wire STORE1_MATCHn = nand(AROR_MATCH_ENp, STORE1_MATCHA, STORE1_MATCHB);
  /*p29.DEGO*/ wire STORE2_MATCHn = nand(AROR_MATCH_ENp, STORE2_MATCHB, STORE2_MATCHA);
  /*p29.YLOZ*/ wire STORE3_MATCHn = nand(AROR_MATCH_ENp, STORE3_MATCHA, STORE3_MATCHB);
  /*p29.XAGE*/ wire STORE4_MATCHn = nand(AROR_MATCH_ENp, STORE4_MATCHB, STORE4_MATCHA);
  /*p29.EGOM*/ wire STORE5_MATCHn = nand(AROR_MATCH_ENp, STORE5_MATCHB, STORE5_MATCHA);
  /*p29.YBEZ*/ wire STORE6_MATCHn = nand(AROR_MATCH_ENp, STORE6_MATCHB, STORE6_MATCHA);
  /*p29.DYKA*/ wire STORE7_MATCHn = nand(AROR_MATCH_ENp, STORE7_MATCHA, STORE7_MATCHB);
  /*p29.EFYL*/ wire STORE8_MATCHn = nand(AROR_MATCH_ENp, STORE8_MATCHB, STORE8_MATCHA);
  /*p29.YGEM*/ wire STORE9_MATCHn = nand(AROR_MATCH_ENp, STORE9_MATCHA, STORE9_MATCHB);

  /*p29.FEFY*/ wire FEFY = nand(STORE4_MATCHn, STORE3_MATCHn, STORE2_MATCHn, STORE1_MATCHn, STORE0_MATCHn);
  /*p29.FOVE*/ wire FOVE = nand(STORE9_MATCHn, STORE8_MATCHn, STORE7_MATCHn, STORE6_MATCHn, STORE5_MATCHn);
  /*p29.FEPO*/ wire FEPO_STORE_MATCHp = or (FEFY, FOVE);

  return {
    .FEPO_STORE_MATCHp = FEPO_STORE_MATCHp,

    .STORE0_MATCHn = STORE0_MATCHn,
    .STORE1_MATCHn = STORE1_MATCHn,
    .STORE2_MATCHn = STORE2_MATCHn,
    .STORE3_MATCHn = STORE3_MATCHn,
    .STORE4_MATCHn = STORE4_MATCHn,
    .STORE5_MATCHn = STORE5_MATCHn,
    .STORE6_MATCHn = STORE6_MATCHn,
    .STORE7_MATCHn = STORE7_MATCHn,
    .STORE8_MATCHn = STORE8_MATCHn,
    .STORE9_MATCHn = STORE9_MATCHn,

    .AVAP_SCAN_DONE_d0_TRIGp = AVAP_SCAN_DONE_d0_TRIGp,
  };
}

void SpriteStoreRegisters::tick(const TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto sst_sig = sig(gb);

  /*p23.XYMO*/ wire LCDC_SPSIZE  = gb.cfg_reg.LCDC_SPSIZE.q();

  /*p21.XYMU*/ wire XYMU_RENDERINGp = gb.ppu_reg.XYMU_RENDERINGp;

  /*p29.CATU*/ wire CATU_VID_LINE_d4 = gb.lcd_reg.CATU_VID_LINE_d4.q();
  /*p28.ANEL*/ wire ANEL_VID_LINE_d6 = gb.lcd_reg.ANEL_VID_LINE_d6.q();

  /*p28.ABAF*/ wire ABAF_VID_LINE_d4n      = not(CATU_VID_LINE_d4);
  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (ANEL_VID_LINE_d6, ABAF_VID_LINE_d4n), rst_sig.ABEZ_VID_RSTn);
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n);
  /*p28.ABAK*/ wire ABAK_VID_LINE_TRIG_d4p = or (ATEJ_VID_LINE_TRIG_d4p, rst_sig.AMYG_VID_RSTp);
  /*p28.BYVA*/ wire BYVA_VID_LINE_TRIG_d4n = not(ABAK_VID_LINE_TRIG_d4p);
  /*p29.DYBA*/ wire DYBA_VID_LINE_TRIG_d4p = not(BYVA_VID_LINE_TRIG_d4n);

  /*p28.YFOT*/ wire YFOT_OAM_A2p = gb.oam_pins.A2;
  /*p28.YFOC*/ wire YFOC_OAM_A3p = gb.oam_pins.A3;
  /*p28.YVOM*/ wire YVOM_OAM_A4p = gb.oam_pins.A4;
  /*p28.YMEV*/ wire YMEV_OAM_A5p = gb.oam_pins.A5;
  /*p28.XEMU*/ wire XEMU_OAM_A6p = gb.oam_pins.A6;
  /*p28.YZET*/ wire YZET_OAM_A7p = gb.oam_pins.A7;

  //----------------------------------------
  // Current LCD coordinate

  /*p29.EBOS*/ wire Y0n = not(gb.lcd_reg.MUWY_Y0);
  /*p29.DASA*/ wire Y1n = not(gb.lcd_reg.MYRO_Y1);
  /*p29.FUKY*/ wire Y2n = not(gb.lcd_reg.LEXA_Y2);
  /*p29.FUVE*/ wire Y3n = not(gb.lcd_reg.LYDO_Y3);
  /*p29.FEPU*/ wire Y4n = not(gb.lcd_reg.LOVU_Y4);
  /*p29.FOFA*/ wire Y5n = not(gb.lcd_reg.LEMA_Y5);
  /*p29.FEMO*/ wire Y6n = not(gb.lcd_reg.MATO_Y6);
  /*p29.GUSU*/ wire Y7n = not(gb.lcd_reg.LAFO_Y7);

  //----------------------------------------
  // Current sprite X coordinate & sprite Y delta.

  // Sprite store X coordinate comes from REG_OAM_A during scan.
  // Sprite store Y coordinate comes from REG_OAM_B during scan.

  /*p31.ZAGO*/ wire ZAGO_SPRITE_X0 = not(!gb.oam_reg.REG_OAM_A0);
  /*p31.ZOCY*/ wire ZOCY_SPRITE_X1 = not(!gb.oam_reg.REG_OAM_A1);
  /*p31.YPUR*/ wire YPUR_SPRITE_X2 = not(!gb.oam_reg.REG_OAM_A2);
  /*p31.YVOK*/ wire YVOK_SPRITE_X3 = not(!gb.oam_reg.REG_OAM_A3);
  /*p31.COSE*/ wire COSE_SPRITE_X4 = not(!gb.oam_reg.REG_OAM_A4);
  /*p31.AROP*/ wire AROP_SPRITE_X5 = not(!gb.oam_reg.REG_OAM_A5);
  /*p31.XATU*/ wire XATU_SPRITE_X6 = not(!gb.oam_reg.REG_OAM_A6);
  /*p31.BADY*/ wire BADY_SPRITE_X7 = not(!gb.oam_reg.REG_OAM_A7);

  /*p29.ERUC*/ wire YDIFF_S0 = add_c(Y0n, !gb.oam_reg.REG_OAM_B0, gb.joy_pin.P10_B); // are these really connected directly to the pin?
  /*p29.ERUC*/ wire YDIFF_C0 = add_s(Y0n, !gb.oam_reg.REG_OAM_B0, gb.joy_pin.P10_B);
  /*p29.ENEF*/ wire YDIFF_S1 = add_s(Y1n, !gb.oam_reg.REG_OAM_B1, YDIFF_C0);
  /*p29.ENEF*/ wire YDIFF_C1 = add_c(Y1n, !gb.oam_reg.REG_OAM_B1, YDIFF_C0);
  /*p29.FECO*/ wire YDIFF_S2 = add_s(Y2n, !gb.oam_reg.REG_OAM_B2, YDIFF_C1);
  /*p29.FECO*/ wire YDIFF_C2 = add_c(Y2n, !gb.oam_reg.REG_OAM_B2, YDIFF_C1);
  /*p29.GYKY*/ wire YDIFF_S3 = add_s(Y3n, !gb.oam_reg.REG_OAM_B3, YDIFF_C2);
  /*p29.GYKY*/ wire YDIFF_C3 = add_c(Y3n, !gb.oam_reg.REG_OAM_B3, YDIFF_C2);
  /*p29.GOPU*/ wire YDIFF_S4 = add_s(Y4n, !gb.oam_reg.REG_OAM_B4, YDIFF_C3);
  /*p29.GOPU*/ wire YDIFF_C4 = add_c(Y4n, !gb.oam_reg.REG_OAM_B4, YDIFF_C3);
  /*p29.FUWA*/ wire YDIFF_S5 = add_s(Y5n, !gb.oam_reg.REG_OAM_B5, YDIFF_C4);
  /*p29.FUWA*/ wire YDIFF_C5 = add_c(Y5n, !gb.oam_reg.REG_OAM_B5, YDIFF_C4);
  /*p29.GOJU*/ wire YDIFF_S6 = add_s(Y6n, !gb.oam_reg.REG_OAM_B6, YDIFF_C5);
  /*p29.GOJU*/ wire YDIFF_C6 = add_c(Y6n, !gb.oam_reg.REG_OAM_B6, YDIFF_C5);
  /*p29.WUHU*/ wire YDIFF_S7 = add_s(Y7n, !gb.oam_reg.REG_OAM_B7, YDIFF_C6);
  /*p29.WUHU*/ wire YDIFF_C7 = add_c(Y7n, !gb.oam_reg.REG_OAM_B7, YDIFF_C6);

  /*p29.DEGE*/ wire SPRITE_DELTA0 = not(YDIFF_S0);
  /*p29.DABY*/ wire SPRITE_DELTA1 = not(YDIFF_S1);
  /*p29.DABU*/ wire SPRITE_DELTA2 = not(YDIFF_S2);
  /*p29.GYSA*/ wire SPRITE_DELTA3 = not(YDIFF_S3);
  /*p29.GACE*/ wire SPRITE_DELTA4 = not(YDIFF_S4);
  /*p29.GUVU*/ wire SPRITE_DELTA5 = not(YDIFF_S5);
  /*p29.GYDA*/ wire SPRITE_DELTA6 = not(YDIFF_S6);
  /*p29.GEWY*/ wire SPRITE_DELTA7 = not(YDIFF_S7);

  /*p29.GOVU*/ wire GOVU_SPSIZE_MATCH = or (YDIFF_S3, LCDC_SPSIZE);
  /*p29.WOTA*/ wire WOTA_SCAN_MATCH_Yn = nand(SPRITE_DELTA4, SPRITE_DELTA5, SPRITE_DELTA6, SPRITE_DELTA7, YDIFF_C7, GOVU_SPSIZE_MATCH);
  /*p29.GESE*/ wire GESE_SCAN_MATCH_Y = not(WOTA_SCAN_MATCH_Yn);

  //----------------------------------------
  // Sprite scan trigger & reset. Why it resets both before and after the scan I do not know.

  /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4p, rst_sig.ATAR_VID_RSTp);
  /*p29.BALU*/ wire BALU_SCAN_RSTp = not(ANOM_SCAN_RSTn);
  /*p29.BAGY*/ wire BAGY_SCAN_RSTn = not(BALU_SCAN_RSTp);

  /*p28.FETO*/ wire FETO_SCAN_DONE_d0 = and (SCAN0, SCAN1, SCAN2, SCAN5); // 32 + 4 + 2 + 1 = 39
  /*p29.BYBA*/ SCAN_DONE_d4.set(clk_sig.XUPY_xBCxxFGx, BAGY_SCAN_RSTn, FETO_SCAN_DONE_d0);
  /*p29.DOBA*/ SCAN_DONE_d5.set(clk_sig.ALET_xBxDxFxH, BAGY_SCAN_RSTn, SCAN_DONE_d4);

  /*p29.BEBU*/ wire BEBU_SCAN_RSTn = or (BALU_SCAN_RSTp, SCAN_DONE_d5.q(), !SCAN_DONE_d4.q());
  /*p29.AVAP*/ wire AVAP_SCAN_RSTp = not(BEBU_SCAN_RSTn);
  /*p28.ASEN*/ wire ASEN_SCAN_RSTp = or (rst_sig.ATAR_VID_RSTp, AVAP_SCAN_RSTp);

  /*p28.BESU*/ BESU_SCANNINGp.nor_latch(CATU_VID_LINE_d4, ASEN_SCAN_RSTp);
  /*p29.CENO*/ CENO_SCANNINGp.set(clk_sig.XUPY_xBCxxFGx, rst_sig.ABEZ_VID_RSTn, BESU_SCANNINGp);
  /*p29.CEHA*/ wire CEHA_SCANNINGp = not(!CENO_SCANNINGp);

  //----------------------------------------
  // Sprite scan counter
  // Sprite scan takes 160 phases, 4 phases per sprite.

  /*p28.GAVA*/ wire SCAN_CLK = or (FETO_SCAN_DONE_d0, clk_sig.XUPY_xBCxxFGx);
  /*p28.YFEL*/ SCAN0.set(SCAN_CLK,       ANOM_SCAN_RSTn, !SCAN0);
  /*p28.WEWY*/ SCAN1.set(!SCAN0, ANOM_SCAN_RSTn, !SCAN1);
  /*p28.GOSO*/ SCAN2.set(!SCAN1, ANOM_SCAN_RSTn, !SCAN2);
  /*p28.ELYN*/ SCAN3.set(!SCAN2, ANOM_SCAN_RSTn, !SCAN3);
  /*p28.FAHA*/ SCAN4.set(!SCAN3, ANOM_SCAN_RSTn, !SCAN4);
  /*p28.FONY*/ SCAN5.set(!SCAN4, ANOM_SCAN_RSTn, !SCAN5);

  //----------------------------------------
  // Sprite store counter.
  // Polarities here are weird and clocks are getting derived weirdly.
  // The store clock comes in through XUPY_xBCxxFGx here.
  // The sprite count clock stops ticking once we have 10 sprites.

  /*p29.CARE*/ wire CARE_STORE_ENp = or (clk_sig.XOCE_ABxxEFxx, CEHA_SCANNINGp, GESE_SCAN_MATCH_Y);
  /*p29.DYTY*/ wire DYTY_STORE_ENn = not(CARE_STORE_ENp);
  /*p29.DEZY*/ DEZY_CLKp.set(clk_sig.ZEME_AxCxExGx, rst_sig.XAPO_VID_RSTn, DYTY_STORE_ENn);

  /*p29.BAKY*/ wire BAKY_SPRITES_FULL = and(SPRITE_COUNT1, SPRITE_COUNT3);
  /*p29.CAKE*/ wire CAKE_CLKp = or (BAKY_SPRITES_FULL, DEZY_CLKp);
  /*p28.AZYB*/ wire AZYB_RSTn = not(BYHA_VID_LINE_TRIG_d4n);
  /*p29.BESE*/ SPRITE_COUNT0.set(CAKE_CLKp,             AZYB_RSTn, !SPRITE_COUNT0);
  /*p29.CUXY*/ SPRITE_COUNT1.set(SPRITE_COUNT0, AZYB_RSTn, !SPRITE_COUNT1);
  /*p29.BEGO*/ SPRITE_COUNT2.set(SPRITE_COUNT1, AZYB_RSTn, !SPRITE_COUNT2);
  /*p29.DYBE*/ SPRITE_COUNT3.set(SPRITE_COUNT2, AZYB_RSTn, !SPRITE_COUNT3);

  //----------------------------------------
  // Sprite store matcher.

  //----------------------------------------
  // Sprite store input driver.
  // Sprite store index comes from the OAM address bus.

  /*p30.XADU*/ XADU_SPRITE_IDX0.set(clk_sig.WUDA_xBCxxFGx, dbg_sig.WEFE_P10_Bn, YFOT_OAM_A2p);
  /*p30.XEDY*/ XEDY_SPRITE_IDX1.set(clk_sig.WUDA_xBCxxFGx, dbg_sig.WEFE_P10_Bn, YFOC_OAM_A3p);
  /*p30.ZUZE*/ ZUZE_SPRITE_IDX2.set(clk_sig.WUDA_xBCxxFGx, dbg_sig.WEFE_P10_Bn, YVOM_OAM_A4p);
  /*p30.XOBE*/ XOBE_SPRITE_IDX3.set(clk_sig.WUDA_xBCxxFGx, dbg_sig.WEFE_P10_Bn, YMEV_OAM_A5p);
  /*p30.YDUF*/ YDUF_SPRITE_IDX4.set(clk_sig.WUDA_xBCxxFGx, dbg_sig.WEFE_P10_Bn, XEMU_OAM_A6p);
  /*p30.XECU*/ XECU_SPRITE_IDX5.set(clk_sig.WUDA_xBCxxFGx, dbg_sig.WEFE_P10_Bn, YZET_OAM_A7p);

  // polarity seems wrong or something
  /*p29.BUZA*/ wire BUZA_STORE_SPRITE_IDX = and (!CENO_SCANNINGp, XYMU_RENDERINGp);

  /*p30.WUZY*/ WUZY_TS_IDX_0.set_tribuf(BUZA_STORE_SPRITE_IDX, XADU_SPRITE_IDX0);
  /*p30.WYSE*/ WYSE_TS_IDX_1.set_tribuf(BUZA_STORE_SPRITE_IDX, XEDY_SPRITE_IDX1);
  /*p30.ZYSU*/ ZYSU_TS_IDX_2.set_tribuf(BUZA_STORE_SPRITE_IDX, ZUZE_SPRITE_IDX2);
  /*p30.WYDA*/ WYDA_TS_IDX_3.set_tribuf(BUZA_STORE_SPRITE_IDX, XOBE_SPRITE_IDX3);
  /*p30.WUCO*/ WUCO_TS_IDX_4.set_tribuf(BUZA_STORE_SPRITE_IDX, YDUF_SPRITE_IDX4);
  /*p30.WEZA*/ WEZA_TS_IDX_5.set_tribuf(BUZA_STORE_SPRITE_IDX, XECU_SPRITE_IDX5);

  // FEPO_STORE_MATCHp here is weird, I guess it's just an easy signal to use to mux the bus?
  /*p30.WENU*/ WENU_TS_LINE_0.set_tribuf(sst_sig.FEPO_STORE_MATCHp, SPRITE_DELTA0);
  /*p30.CUCU*/ CUCU_TS_LINE_1.set_tribuf(sst_sig.FEPO_STORE_MATCHp, SPRITE_DELTA1);
  /*p30.CUCA*/ CUCA_TS_LINE_2.set_tribuf(sst_sig.FEPO_STORE_MATCHp, SPRITE_DELTA2);
  /*p30.CEGA*/ CEGA_TS_LINE_3.set_tribuf(sst_sig.FEPO_STORE_MATCHp, SPRITE_DELTA3);

  //----------------------------------------
  // Sprite store getter

  /*p29.WEFU*/ wire STORE0_MATCH = not(sst_sig.STORE0_MATCHn);
  /*p29.GAJA*/ wire STORE1_MATCH = not(sst_sig.STORE1_MATCHn);
  /*p29.GUPO*/ wire STORE2_MATCH = not(sst_sig.STORE2_MATCHn);
  /*p29.WEBO*/ wire STORE3_MATCH = not(sst_sig.STORE3_MATCHn);
  /*p29.WUNA*/ wire STORE4_MATCH = not(sst_sig.STORE4_MATCHn);
  /*p29.GABA*/ wire STORE5_MATCH = not(sst_sig.STORE5_MATCHn);
  /*p29.WASE*/ wire STORE6_MATCH = not(sst_sig.STORE6_MATCHn);
  /*p29.GYTE*/ wire STORE7_MATCH = not(sst_sig.STORE7_MATCHn);
  /*p29.GEKE*/ wire STORE8_MATCH = not(sst_sig.STORE8_MATCHn);

  // Priority encoder so we fetch the first sprite that matches

  /*p29.GEZE*/ wire STORE0_MATCH_OUT = or (STORE0_MATCH, gb.joy_pin.P10_B);
  /*p29.FUMA*/ wire STORE1_MATCH_OUT = or (STORE1_MATCH, STORE0_MATCH_OUT);
  /*p29.GEDE*/ wire STORE2_MATCH_OUT = or (STORE2_MATCH, STORE1_MATCH_OUT);
  /*p29.WUTO*/ wire STORE3_MATCH_OUT = or (STORE3_MATCH, STORE2_MATCH_OUT);
  /*p29.XYLA*/ wire STORE4_MATCH_OUT = or (STORE4_MATCH, STORE3_MATCH_OUT);
  /*p29.WEJA*/ wire STORE5_MATCH_OUT = or (STORE5_MATCH, STORE4_MATCH_OUT);
  /*p29.WYLA*/ wire STORE6_MATCH_OUT = or (STORE6_MATCH, STORE5_MATCH_OUT);
  /*p29.FAVO*/ wire STORE7_MATCH_OUT = or (STORE7_MATCH, STORE6_MATCH_OUT);
  /*p29.GYGA*/ wire STORE8_MATCH_OUT = or (STORE8_MATCH, STORE7_MATCH_OUT);

  /*p29.GUVA*/ wire GUVA_SPRITE0_GETp = nor(sst_sig.STORE0_MATCHn, gb.joy_pin.P10_B);
  /*p29.ENUT*/ wire ENUT_SPRITE1_GETp = nor(sst_sig.STORE1_MATCHn, STORE0_MATCH_OUT);
  /*p29.EMOL*/ wire EMOL_SPRITE2_GETp = nor(sst_sig.STORE2_MATCHn, STORE1_MATCH_OUT);
  /*p29.GYFY*/ wire GYFY_SPRITE3_GETp = nor(sst_sig.STORE3_MATCHn, STORE2_MATCH_OUT);
  /*p29.GONO*/ wire GONO_SPRITE4_GETp = nor(sst_sig.STORE4_MATCHn, STORE3_MATCH_OUT);
  /*p29.GEGA*/ wire GEGA_SPRITE5_GETp = nor(sst_sig.STORE5_MATCHn, STORE4_MATCH_OUT);
  /*p29.XOJA*/ wire XOJA_SPRITE6_GETp = nor(sst_sig.STORE6_MATCHn, STORE5_MATCH_OUT);
  /*p29.GUTU*/ wire GUTU_SPRITE7_GETp = nor(sst_sig.STORE7_MATCHn, STORE6_MATCH_OUT);
  /*p29.FOXA*/ wire FOXA_SPRITE8_GETp = nor(sst_sig.STORE8_MATCHn, STORE7_MATCH_OUT);
  /*p29.GUZE*/ wire GUZE_SPRITE9_GETp = nor(sst_sig.STORE9_MATCHn, STORE8_MATCH_OUT);

  /*p29.FURO*/ wire FURO_SPRITE0_GETn = not(GUVA_SPRITE0_GETp);
  /*p29.DYDO*/ wire DYDO_SPRITE1_GETn = not(ENUT_SPRITE1_GETp);
  /*p29.FAME*/ wire FAME_SPRITE2_GETn = not(EMOL_SPRITE2_GETp);
  /*p29.GYMA*/ wire GYMA_SPRITE3_GETn = not(GYFY_SPRITE3_GETp);
  /*p29.GOWO*/ wire GOWO_SPRITE4_GETn = not(GONO_SPRITE4_GETp);
  /*p29.GYGY*/ wire GYGY_SPRITE5_GETn = not(GEGA_SPRITE5_GETp);
  /*p29.XYME*/ wire XYME_SPRITE6_GETn = not(XOJA_SPRITE6_GETp);
  /*p29.GUGY*/ wire GUGY_SPRITE7_GETn = not(GUTU_SPRITE7_GETp);
  /*p29.DENY*/ wire DENY_SPRITE8_GETn = not(FOXA_SPRITE8_GETp);
  /*p29.FADO*/ wire FADO_SPRITE9_GETn = not(GUZE_SPRITE9_GETp);

  /*p30.ZEDY*/ WUZY_TS_IDX_0.set_tribuf(!FURO_SPRITE0_GETn, YWAK_STORE0_IDX0);
  /*p30.ZUMU*/ WYSE_TS_IDX_1.set_tribuf(!FURO_SPRITE0_GETn, ZONY_STORE0_IDX1);
  /*p30.WOKO*/ ZYSU_TS_IDX_2.set_tribuf(!FURO_SPRITE0_GETn, WYTE_STORE0_IDX2);
  /*p30.ZAVE*/ WYDA_TS_IDX_3.set_tribuf(!FURO_SPRITE0_GETn, YZEP_STORE0_IDX3);
  /*p30.ZECE*/ WUCO_TS_IDX_4.set_tribuf(!FURO_SPRITE0_GETn, YSOK_STORE0_IDX4);
  /*p30.ZETU*/ WEZA_TS_IDX_5.set_tribuf(!FURO_SPRITE0_GETn, YGUS_STORE0_IDX5);
  /*p30.GOFO*/ WENU_TS_LINE_0.set_tribuf(!FURO_SPRITE0_GETn, STORE0_LINE0);
  /*p30.WEHE*/ CUCU_TS_LINE_1.set_tribuf(!FURO_SPRITE0_GETn, STORE0_LINE1);
  /*p30.AJAL*/ CUCA_TS_LINE_2.set_tribuf(!FURO_SPRITE0_GETn, STORE0_LINE2);
  /*p30.BUKY*/ CEGA_TS_LINE_3.set_tribuf(!FURO_SPRITE0_GETn, STORE0_LINE3);

  /*p30.CUBO*/ WUZY_TS_IDX_0.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_IDX5);
  /*p30.CELU*/ WYSE_TS_IDX_1.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_IDX4);
  /*p30.CEGY*/ ZYSU_TS_IDX_2.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_IDX3);
  /*p30.BETY*/ WYDA_TS_IDX_3.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_IDX2);
  /*p30.CYBY*/ WUCO_TS_IDX_4.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_IDX1);
  /*p30.BEMO*/ WEZA_TS_IDX_5.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_IDX0);
  /*p30.BEFE*/ WENU_TS_LINE_0.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_LINE0);
  /*p30.BYRO*/ CUCU_TS_LINE_1.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_LINE1);
  /*p30.BACO*/ CUCA_TS_LINE_2.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_LINE2);
  /*p30.AHUM*/ CEGA_TS_LINE_3.set_tribuf(!DYDO_SPRITE1_GETn, STORE1_LINE3);

  /*p30.AXEC*/ WEZA_TS_IDX_5.set_tribuf(!FAME_SPRITE2_GETn, STORE2_IDX0);
  /*p30.CYRO*/ WUCO_TS_IDX_4.set_tribuf(!FAME_SPRITE2_GETn, STORE2_IDX1);
  /*p30.CUVU*/ WYDA_TS_IDX_3.set_tribuf(!FAME_SPRITE2_GETn, STORE2_IDX2);
  /*p30.APON*/ ZYSU_TS_IDX_2.set_tribuf(!FAME_SPRITE2_GETn, STORE2_IDX3);
  /*p30.AFOZ*/ WYSE_TS_IDX_1.set_tribuf(!FAME_SPRITE2_GETn, STORE2_IDX4);
  /*p30.CUBE*/ WUZY_TS_IDX_0.set_tribuf(!FAME_SPRITE2_GETn, STORE2_IDX5);
  /*p30.ZABY*/ WENU_TS_LINE_0.set_tribuf(!FAME_SPRITE2_GETn, STORE2_LINE0);
  /*p30.ZUKE*/ CUCU_TS_LINE_1.set_tribuf(!FAME_SPRITE2_GETn, STORE2_LINE1);
  /*p30.WUXE*/ CUCA_TS_LINE_2.set_tribuf(!FAME_SPRITE2_GETn, STORE2_LINE2);
  /*p30.WERE*/ CEGA_TS_LINE_3.set_tribuf(!FAME_SPRITE2_GETn, STORE2_LINE3);

  /*p30.DEZU*/ WEZA_TS_IDX_5.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_IDX0);
  /*p30.EFUD*/ WUCO_TS_IDX_4.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_IDX1);
  /*p30.DONY*/ WYDA_TS_IDX_3.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_IDX2);
  /*p30.DOWA*/ ZYSU_TS_IDX_2.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_IDX3);
  /*p30.DYGO*/ WYSE_TS_IDX_1.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_IDX4);
  /*p30.ENAP*/ WUZY_TS_IDX_0.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_IDX5);
  /*p30.ZYPO*/ WENU_TS_LINE_0.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_LINE0);
  /*p30.ZEXE*/ CUCU_TS_LINE_1.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_LINE1);
  /*p30.YJEM*/ CUCA_TS_LINE_2.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_LINE2);
  /*p30.YWAV*/ CEGA_TS_LINE_3.set_tribuf(!GYMA_SPRITE3_GETn, STORE3_LINE4);

  /*p30.WAJA*/ WEZA_TS_IDX_5.set_tribuf(!GOWO_SPRITE4_GETn, XYNU);
  /*p30.WOXY*/ WUCO_TS_IDX_4.set_tribuf(!GOWO_SPRITE4_GETn, XEGE);
  /*p30.XYRE*/ WYDA_TS_IDX_3.set_tribuf(!GOWO_SPRITE4_GETn, XABO);
  /*p30.WERU*/ ZYSU_TS_IDX_2.set_tribuf(!GOWO_SPRITE4_GETn, WANU);
  /*p30.WEPY*/ WYSE_TS_IDX_1.set_tribuf(!GOWO_SPRITE4_GETn, XEFE);
  /*p30.WUXU*/ WUZY_TS_IDX_0.set_tribuf(!GOWO_SPRITE4_GETn, XAVE);
  /*p30.BYDO*/ WENU_TS_LINE_0.set_tribuf(!GOWO_SPRITE4_GETn, CUMU);
  /*p30.BUCE*/ CUCU_TS_LINE_1.set_tribuf(!GOWO_SPRITE4_GETn, CAPO);
  /*p30.BOVE*/ CUCA_TS_LINE_2.set_tribuf(!GOWO_SPRITE4_GETn, CONO);
  /*p30.BEVY*/ CEGA_TS_LINE_3.set_tribuf(!GOWO_SPRITE4_GETn, CAJU);

  /*p30.DALO*/ WEZA_TS_IDX_5.set_tribuf(!GYGY_SPRITE5_GETn, EKAP);
  /*p30.DALY*/ WUCO_TS_IDX_4.set_tribuf(!GYGY_SPRITE5_GETn, ETAV);
  /*p30.DUZA*/ WYDA_TS_IDX_3.set_tribuf(!GYGY_SPRITE5_GETn, EBEX);
  /*p30.WAGA*/ ZYSU_TS_IDX_2.set_tribuf(!GYGY_SPRITE5_GETn, GORU);
  /*p30.DYNY*/ WYSE_TS_IDX_1.set_tribuf(!GYGY_SPRITE5_GETn, ETYM);
  /*p30.DOBO*/ WUZY_TS_IDX_0.set_tribuf(!GYGY_SPRITE5_GETn, EKOP);
  /*p30.AWAT*/ WENU_TS_LINE_0.set_tribuf(!GYGY_SPRITE5_GETn, ANED);
  /*p30.BACE*/ CUCU_TS_LINE_1.set_tribuf(!GYGY_SPRITE5_GETn, ACEP);
  /*p30.BODU*/ CUCA_TS_LINE_2.set_tribuf(!GYGY_SPRITE5_GETn, ABUX);
  /*p30.BUJA*/ CEGA_TS_LINE_3.set_tribuf(!GYGY_SPRITE5_GETn, ABEG);

  /*p30.WOCY*/ WEZA_TS_IDX_5.set_tribuf(!XYME_SPRITE6_GETn, GECU);
  /*p30.ELYC*/ WUCO_TS_IDX_4.set_tribuf(!XYME_SPRITE6_GETn, FOXY);
  /*p30.WABO*/ WYDA_TS_IDX_3.set_tribuf(!XYME_SPRITE6_GETn, GOHU);
  /*p30.EZOC*/ ZYSU_TS_IDX_2.set_tribuf(!XYME_SPRITE6_GETn, FOGO);
  /*p30.WYWY*/ WYSE_TS_IDX_1.set_tribuf(!XYME_SPRITE6_GETn, GACY);
  /*p30.WATO*/ WUZY_TS_IDX_0.set_tribuf(!XYME_SPRITE6_GETn, GABO);
  /*p30.ZUDO*/ WENU_TS_LINE_0.set_tribuf(!XYME_SPRITE6_GETn, ZUBE);
  /*p30.YBUK*/ CUCU_TS_LINE_1.set_tribuf(!XYME_SPRITE6_GETn, ZUMY);
  /*p30.ZYTO*/ CUCA_TS_LINE_2.set_tribuf(!XYME_SPRITE6_GETn, ZEXO);
  /*p30.YKOZ*/ CEGA_TS_LINE_3.set_tribuf(!XYME_SPRITE6_GETn, ZAFU);

  /*p30.EVYT*/ WEZA_TS_IDX_5.set_tribuf(!GUGY_SPRITE7_GETn, FUZO);
  /*p30.WABA*/ WUCO_TS_IDX_4.set_tribuf(!GUGY_SPRITE7_GETn, GESY);
  /*p30.ETAD*/ WYDA_TS_IDX_3.set_tribuf(!GUGY_SPRITE7_GETn, FYSU);
  /*p30.ELEP*/ ZYSU_TS_IDX_2.set_tribuf(!GUGY_SPRITE7_GETn, FEFA);
  /*p30.WYGO*/ WYSE_TS_IDX_1.set_tribuf(!GUGY_SPRITE7_GETn, GYNO);
  /*p30.WAKO*/ WUZY_TS_IDX_0.set_tribuf(!GUGY_SPRITE7_GETn, GULE);
  /*p30.WANA*/ WENU_TS_LINE_0.set_tribuf(!GUGY_SPRITE7_GETn, XYGO);
  /*p30.WAXE*/ CUCU_TS_LINE_1.set_tribuf(!GUGY_SPRITE7_GETn, XYNA);
  /*p30.WABU*/ CUCA_TS_LINE_2.set_tribuf(!GUGY_SPRITE7_GETn, XAKU);
  /*p30.YPOZ*/ CEGA_TS_LINE_3.set_tribuf(!GUGY_SPRITE7_GETn, YGUM);

  /*p30.ADYB*/ WEZA_TS_IDX_5.set_tribuf(!DENY_SPRITE8_GETn, STORE8_IDX5);
  /*p30.APOB*/ WUCO_TS_IDX_4.set_tribuf(!DENY_SPRITE8_GETn, STORE8_IDX4);
  /*p30.APYV*/ WYDA_TS_IDX_3.set_tribuf(!DENY_SPRITE8_GETn, STORE8_IDX3);
  /*p30.AFEN*/ ZYSU_TS_IDX_2.set_tribuf(!DENY_SPRITE8_GETn, STORE8_IDX2);
  /*p30.AKYH*/ WYSE_TS_IDX_1.set_tribuf(!DENY_SPRITE8_GETn, STORE8_IDX1);
  /*p30.APOC*/ WUZY_TS_IDX_0.set_tribuf(!DENY_SPRITE8_GETn, STORE8_IDX0);
  /*p30.BUJY*/ WENU_TS_LINE_0.set_tribuf(!DENY_SPRITE8_GETn, STORE8_LINE0);
  /*p30.BOSO*/ CUCU_TS_LINE_1.set_tribuf(!DENY_SPRITE8_GETn, STORE8_LINE1);
  /*p30.AHAC*/ CUCA_TS_LINE_2.set_tribuf(!DENY_SPRITE8_GETn, STORE8_LINE2);
  /*p30.BAZU*/ CEGA_TS_LINE_3.set_tribuf(!DENY_SPRITE8_GETn, STORE8_LINE3);

  /*p30.YHAL*/ WEZA_TS_IDX_5.set_tribuf(!FADO_SPRITE9_GETn, STORE9_IDX0);
  /*p30.YRAD*/ WUCO_TS_IDX_4.set_tribuf(!FADO_SPRITE9_GETn, STORE9_IDX1);
  /*p30.XYRA*/ WYDA_TS_IDX_3.set_tribuf(!FADO_SPRITE9_GETn, STORE9_IDX2);
  /*p30.YNEV*/ ZYSU_TS_IDX_2.set_tribuf(!FADO_SPRITE9_GETn, STORE9_IDX3);
  /*p30.ZOJY*/ WYSE_TS_IDX_1.set_tribuf(!FADO_SPRITE9_GETn, STORE9_IDX4);
  /*p30.ZARO*/ WUZY_TS_IDX_0.set_tribuf(!FADO_SPRITE9_GETn, STORE9_IDX5);
  /*p30.CAWO*/ WENU_TS_LINE_0.set_tribuf(!FADO_SPRITE9_GETn, STORE9_LINE0);
  /*p30.BYME*/ CUCU_TS_LINE_1.set_tribuf(!FADO_SPRITE9_GETn, STORE9_LINE1);
  /*p30.COHO*/ CUCA_TS_LINE_2.set_tribuf(!FADO_SPRITE9_GETn, STORE9_LINE2);
  /*p30.GATE*/ CEGA_TS_LINE_3.set_tribuf(!FADO_SPRITE9_GETn, STORE9_LINE3);

  //----------------------------------------
  // Sprite store setter

  /*p29.EBOJ*/ EBOJ_STORE0_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, GUVA_SPRITE0_GETp);
  /*p29.CEDY*/ CEDY_STORE1_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, ENUT_SPRITE1_GETp);
  /*p29.EGAV*/ EGAV_STORE2_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, EMOL_SPRITE2_GETp);
  /*p29.GOTA*/ GOTA_STORE3_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, GYFY_SPRITE3_GETp);
  /*p29.XUDY*/ XUDY_STORE4_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, GONO_SPRITE4_GETp);
  /*p29.WAFY*/ WAFY_STORE5_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, GEGA_SPRITE5_GETp);
  /*p29.WOMY*/ WOMY_STORE6_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, XOJA_SPRITE6_GETp);
  /*p29.WAPO*/ WAPO_STORE7_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, GUTU_SPRITE7_GETp);
  /*p29.EXUQ*/ EXUQ_STORE8_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, FOXA_SPRITE8_GETp);
  /*p29.FONO*/ FONO_STORE9_RSTp.set(ppu_sig.WUTY_SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, GUZE_SPRITE9_GETp);
    
  /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or (DYBA_VID_LINE_TRIG_d4p, EBOJ_STORE0_RSTp);
  /*p29.EFEV*/ wire EFEV_STORE1_RSTp = or (DYBA_VID_LINE_TRIG_d4p, CEDY_STORE1_RSTp);
  /*p29.FOKO*/ wire FOKO_STORE2_RSTp = or (DYBA_VID_LINE_TRIG_d4p, EGAV_STORE2_RSTp);
  /*p29.GAKE*/ wire GAKE_STORE3_RSTp = or (DYBA_VID_LINE_TRIG_d4p, GOTA_STORE3_RSTp);
  /*p29.WOHU*/ wire WOHU_STORE4_RSTp = or (DYBA_VID_LINE_TRIG_d4p, XUDY_STORE4_RSTp);
  /*p29.FEVE*/ wire FEVE_STORE5_RSTp = or (DYBA_VID_LINE_TRIG_d4p, WAFY_STORE5_RSTp);
  /*p29.WACY*/ wire WACY_STORE6_RSTp = or (DYBA_VID_LINE_TRIG_d4p, WOMY_STORE6_RSTp);
  /*p29.GUKY*/ wire GUKY_STORE7_RSTp = or (DYBA_VID_LINE_TRIG_d4p, WAPO_STORE7_RSTp);
  /*p29.GORO*/ wire GORO_STORE8_RSTp = or (DYBA_VID_LINE_TRIG_d4p, EXUQ_STORE8_RSTp);
  /*p29.DUBU*/ wire DUBU_STORE9_RSTp = or (DYBA_VID_LINE_TRIG_d4p, FONO_STORE9_RSTp);

  /*p29.DYNA*/ wire DYNA_STORE0_RSTn = not(DYWE_STORE0_RSTp);
  /*p29.DOKU*/ wire DOKU_STORE1_RSTn = not(EFEV_STORE1_RSTp);
  /*p29.GAMY*/ wire GAMY_STORE2_RSTn = not(FOKO_STORE2_RSTp);
  /*p29.WUPA*/ wire WUPA_STORE3_RSTn = not(GAKE_STORE3_RSTp);
  /*p29.WUNU*/ wire WUNU_STORE4_RSTn = not(WOHU_STORE4_RSTp);
  /*p29.EJAD*/ wire EJAD_STORE5_RSTn = not(FEVE_STORE5_RSTp);
  /*p29.XAHO*/ wire XAHO_STORE6_RSTn = not(WACY_STORE6_RSTp);
  /*p29.GAFY*/ wire GAFY_STORE7_RSTn = not(GUKY_STORE7_RSTp);
  /*p29.WUZO*/ wire WUZO_STORE8_RSTn = not(GORO_STORE8_RSTp);
  /*p29.DOSY*/ wire DOSY_STORE9_RSTn = not(DUBU_STORE9_RSTp);

  /*p29.EDEN*/ wire EDEN_SPRITE_COUNT0n = not(SPRITE_COUNT0);
  /*p29.CYPY*/ wire CYPY_SPRITE_COUNT1n = not(SPRITE_COUNT1);
  /*p29.CAPE*/ wire CAPE_SPRITE_COUNT2n = not(SPRITE_COUNT2);
  /*p29.CAXU*/ wire CAXU_SPRITE_COUNT3n = not(SPRITE_COUNT3);

  /*p29.FYCU*/ wire FYCU_SPRITE_COUNT0p = not(EDEN_SPRITE_COUNT0n);
  /*p29.FONE*/ wire FONE_SPRITE_COUNT1p = not(CYPY_SPRITE_COUNT1n);
  /*p29.EKUD*/ wire EKUD_SPRITE_COUNT2p = not(CAPE_SPRITE_COUNT2n);
  /*p29.ELYG*/ wire ELYG_SPRITE_COUNT3p = not(CAXU_SPRITE_COUNT3n);

  /*p29.DEZO*/ wire DEZO_STORE0_SELn = nand(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*p29.CUVA*/ wire CUVA_STORE1_SELn = nand(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*p29.GEBU*/ wire GEBU_STORE2_SELn = nand(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*p29.FOCO*/ wire FOCO_STORE3_SELn = nand(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
  /*p29.CUPE*/ wire CUPE_STORE4_SELn = nand(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*p29.CUGU*/ wire CUGU_STORE5_SELn = nand(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*p29.WOMU*/ wire WOMU_STORE6_SELn = nand(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*p29.GUNA*/ wire GUNA_STORE7_SELn = nand(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
  /*p29.DEWY*/ wire DEWY_STORE8_SELn = nand(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);
  /*p29.DOGU*/ wire DOGU_STORE9_SELn = nand(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);

  // Sprite stores latch their input when their SELn signal goes _high_
  /*p29.CEMY*/ wire CEMY_STORE0_CLKp = or (DYTY_STORE_ENn, DEZO_STORE0_SELn);
  /*p29.BYBY*/ wire BYBY_STORE1_CLKp = or (DYTY_STORE_ENn, CUVA_STORE1_SELn);
  /*p29.WYXO*/ wire WYXO_STORE2_CLKp = or (DYTY_STORE_ENn, GEBU_STORE2_SELn);
  /*p29.GUVE*/ wire GUVE_STORE3_CLKp = or (DYTY_STORE_ENn, FOCO_STORE3_SELn);
  /*p29.CECU*/ wire CECU_STORE4_CLKp = or (DYTY_STORE_ENn, CUPE_STORE4_SELn);
  /*p29.CADO*/ wire CADO_STORE5_CLKp = or (DYTY_STORE_ENn, CUGU_STORE5_SELn);
  /*p29.XUJO*/ wire XUJO_STORE6_CLKp = or (DYTY_STORE_ENn, WOMU_STORE6_SELn);
  /*p29.GAPE*/ wire GAPE_STORE7_CLKp = or (DYTY_STORE_ENn, GUNA_STORE7_SELn);
  /*p29.CAHO*/ wire CAHO_STORE8_CLKp = or (DYTY_STORE_ENn, DEWY_STORE8_SELn);
  /*p29.CATO*/ wire CATO_STORE9_CLKp = or (DYTY_STORE_ENn, DOGU_STORE9_SELn);

  /*p29.DYHU*/ wire DYHU_STORE0_CLKn = not(CEMY_STORE0_CLKp);
  /*p29.BUCO*/ wire BUCO_STORE1_CLKn = not(BYBY_STORE1_CLKp);
  /*p29.GYFO*/ wire GYFO_STORE2_CLKn = not(WYXO_STORE2_CLKp);
  /*p29.GUSA*/ wire GUSA_STORE3_CLKn = not(GUVE_STORE3_CLKp);
  /*p29.DUKE*/ wire DUKE_STORE4_CLKn = not(CECU_STORE4_CLKp);
  /*p29.BEDE*/ wire BEDE_STORE5_CLKn = not(CADO_STORE5_CLKp);
  /*p29.WEKA*/ wire WEKA_STORE6_CLKn = not(XUJO_STORE6_CLKp);
  /*p29.GYVO*/ wire GYVO_STORE7_CLKn = not(GAPE_STORE7_CLKp);
  /*p29.BUKA*/ wire BUKA_STORE8_CLKn = not(CAHO_STORE8_CLKp);
  /*p29.DECU*/ wire DECU_STORE9_CLKn = not(CATO_STORE9_CLKp);

  // 10 sprite stores

  /*p29.GENY*/ wire GENY_STORE0_CLKp = not(DYHU_STORE0_CLKn);
  /*p29.ENOB*/ wire ENOB_STORE0_CLKp = not(DYHU_STORE0_CLKn);
  /*p29.FUXU*/ wire FUXU_STORE0_CLKp = not(DYHU_STORE0_CLKn);
  /*p30.YWAK*/ YWAK_STORE0_IDX0.set(GENY_STORE0_CLKp, WUZY_TS_IDX_0);
  /*p30.ZONY*/ ZONY_STORE0_IDX1.set(GENY_STORE0_CLKp, WYSE_TS_IDX_1);
  /*p30.WYTE*/ WYTE_STORE0_IDX2.set(GENY_STORE0_CLKp, ZYSU_TS_IDX_2);
  /*p30.YZEP*/ YZEP_STORE0_IDX3.set(GENY_STORE0_CLKp, WYDA_TS_IDX_3);
  /*p30.YSOK*/ YSOK_STORE0_IDX4.set(GENY_STORE0_CLKp, WUCO_TS_IDX_4);
  /*p30.YGUS*/ YGUS_STORE0_IDX5.set(GENY_STORE0_CLKp, WEZA_TS_IDX_5);
  /*p30.FYHY*/ STORE0_LINE0.set(ENOB_STORE0_CLKp, WENU_TS_LINE_0);
  /*p30.GYHO*/ STORE0_LINE1.set(ENOB_STORE0_CLKp, CUCU_TS_LINE_1);
  /*p30.BOZU*/ STORE0_LINE2.set(ENOB_STORE0_CLKp, CUCA_TS_LINE_2);
  /*p30.CUFO*/ STORE0_LINE3.set(ENOB_STORE0_CLKp, CEGA_TS_LINE_3);
  /*p31.XEPE*/ STORE0_X0   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_SPRITE_X0);
  /*p31.YLAH*/ STORE0_X1   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZOCY_SPRITE_X1);
  /*p31.ZOLA*/ STORE0_X2   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YPUR_SPRITE_X2);
  /*p31.ZULU*/ STORE0_X3   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YVOK_SPRITE_X3);
  /*p31.WELO*/ STORE0_X4   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, COSE_SPRITE_X4);
  /*p31.XUNY*/ STORE0_X5   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, AROP_SPRITE_X5);
  /*p31.WOTE*/ STORE0_X6   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, XATU_SPRITE_X6);
  /*p31.XAKO*/ STORE0_X7   .set(FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, BADY_SPRITE_X7);

  /*p29.BYVY*/ wire BYVU_STORE1_CLKp = not(BUCO_STORE1_CLKn);
  /*p29.AHOF*/ wire AHOF_STORE1_CLKp = not(BUCO_STORE1_CLKn);
  /*p29.ASYS*/ wire ASYS_STORE1_CLKp = not(BUCO_STORE1_CLKn);
  /*p30.CAJY*/ STORE1_IDX0 .set(BYVU_STORE1_CLKp, WEZA_TS_IDX_5);
  /*p30.CUZA*/ STORE1_IDX1 .set(BYVU_STORE1_CLKp, WUCO_TS_IDX_4);
  /*p30.COMA*/ STORE1_IDX2 .set(BYVU_STORE1_CLKp, WYDA_TS_IDX_3);
  /*p30.CUFA*/ STORE1_IDX3 .set(BYVU_STORE1_CLKp, ZYSU_TS_IDX_2);
  /*p30.CEBO*/ STORE1_IDX4 .set(BYVU_STORE1_CLKp, WYSE_TS_IDX_1);
  /*p30.CADU*/ STORE1_IDX5 .set(BYVU_STORE1_CLKp, WUZY_TS_IDX_0);
  /*p30.ABUG*/ STORE1_LINE0.set(AHOF_STORE1_CLKp, WENU_TS_LINE_0);
  /*p30.AMES*/ STORE1_LINE1.set(AHOF_STORE1_CLKp, CUCU_TS_LINE_1);
  /*p30.ABOP*/ STORE1_LINE2.set(AHOF_STORE1_CLKp, CUCA_TS_LINE_2);
  /*p30.AROF*/ STORE1_LINE3.set(AHOF_STORE1_CLKp, CEGA_TS_LINE_3);
  /*p31.DANY*/ STORE1_X0   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZAGO_SPRITE_X0);
  /*p31.DUKO*/ STORE1_X1   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZOCY_SPRITE_X1);
  /*p31.DESU*/ STORE1_X2   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YPUR_SPRITE_X2);
  /*p31.DAZO*/ STORE1_X3   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YVOK_SPRITE_X3);
  /*p31.DAKE*/ STORE1_X4   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, COSE_SPRITE_X4);
  /*p31.CESO*/ STORE1_X5   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, AROP_SPRITE_X5);
  /*p31.DYFU*/ STORE1_X6   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, XATU_SPRITE_X6);
  /*p31.CUSY*/ STORE1_X7   .set(ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, BADY_SPRITE_X7);

  /*p29.BUZY*/ wire BUZY_STORE2_CLKp = not(GYFO_STORE2_CLKn);
  /*p29.FUKE*/ wire FUKE_STORE2_CLKp = not(GYFO_STORE2_CLKn);
  /*p29.CACU*/ wire CACU_STORE2_CLKp = not(GYFO_STORE2_CLKn);
  /*p30.BOXA*/ STORE2_IDX0.set(BUZY_STORE2_CLKp, WEZA_TS_IDX_5);
  /*p30.BUNA*/ STORE2_IDX1.set(BUZY_STORE2_CLKp, WUCO_TS_IDX_4);
  /*p30.BULU*/ STORE2_IDX2.set(BUZY_STORE2_CLKp, WYDA_TS_IDX_3);
  /*p30.BECA*/ STORE2_IDX3.set(BUZY_STORE2_CLKp, ZYSU_TS_IDX_2);
  /*p30.BYHU*/ STORE2_IDX4.set(BUZY_STORE2_CLKp, WYSE_TS_IDX_1);
  /*p30.BUHE*/ STORE2_IDX5.set(BUZY_STORE2_CLKp, WUZY_TS_IDX_0);
  /*p30.YKUK*/ STORE2_LINE0.set(FUKE_STORE2_CLKp, WENU_TS_LINE_0);
  /*p30.YLOV*/ STORE2_LINE1.set(FUKE_STORE2_CLKp, CUCU_TS_LINE_1);
  /*p30.XAZY*/ STORE2_LINE2.set(FUKE_STORE2_CLKp, CUCA_TS_LINE_2);
  /*p30.XOSY*/ STORE2_LINE3.set(FUKE_STORE2_CLKp, CEGA_TS_LINE_3);
  /*p31.FOKA*/ STORE2_X0.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZAGO_SPRITE_X0);
  /*p31.FYTY*/ STORE2_X1.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZOCY_SPRITE_X1);
  /*p31.FUBY*/ STORE2_X2.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YPUR_SPRITE_X2);
  /*p31.GOXU*/ STORE2_X3.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YVOK_SPRITE_X3);
  /*p31.DUHY*/ STORE2_X4.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, COSE_SPRITE_X4);
  /*p31.EJUF*/ STORE2_X5.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, AROP_SPRITE_X5);
  /*p31.ENOR*/ STORE2_X6.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, XATU_SPRITE_X6);
  /*p31.DEPY*/ STORE2_X7.set(CACU_STORE2_CLKp, GAMY_STORE2_RSTn, BADY_SPRITE_X7);

  /*p29.FEKA*/ wire FEKA = not(GUSA_STORE3_CLKn);
  /*p29.XYHA*/ wire XYHA = not(GUSA_STORE3_CLKn);
  /*p29.YFAG*/ wire YFAG = not(GUSA_STORE3_CLKn);
  /*p30.DAFU*/ STORE3_IDX0.set(FEKA, WEZA_TS_IDX_5);
  /*p30.DEBA*/ STORE3_IDX1.set(FEKA, WUCO_TS_IDX_4);
  /*p30.DUHA*/ STORE3_IDX2.set(FEKA, WYDA_TS_IDX_3);
  /*p30.DUNY*/ STORE3_IDX3.set(FEKA, ZYSU_TS_IDX_2);
  /*p30.DESE*/ STORE3_IDX4.set(FEKA, WYSE_TS_IDX_1);
  /*p30.DEVY*/ STORE3_IDX5.set(FEKA, WUZY_TS_IDX_0);
  /*p30.ZURY*/ STORE3_LINE0.set(XYHA, WENU_TS_LINE_0);
  /*p30.ZURO*/ STORE3_LINE1.set(XYHA, CUCU_TS_LINE_1);
  /*p30.ZENE*/ STORE3_LINE2.set(XYHA, CUCA_TS_LINE_2);
  /*p30.ZYLU*/ STORE3_LINE4.set(XYHA, CEGA_TS_LINE_3);
  /*p31.XOLY*/ STORE3_X0.set(YFAG, WUPA_STORE3_RSTn, ZAGO_SPRITE_X0);
  /*p31.XYBA*/ STORE3_X1.set(YFAG, WUPA_STORE3_RSTn, ZOCY_SPRITE_X1);
  /*p31.XABE*/ STORE3_X2.set(YFAG, WUPA_STORE3_RSTn, YPUR_SPRITE_X2);
  /*p31.XEKA*/ STORE3_X3.set(YFAG, WUPA_STORE3_RSTn, YVOK_SPRITE_X3);
  /*p31.XOMY*/ STORE3_X4.set(YFAG, WUPA_STORE3_RSTn, COSE_SPRITE_X4);
  /*p31.WUHA*/ STORE3_X5.set(YFAG, WUPA_STORE3_RSTn, AROP_SPRITE_X5);
  /*p31.WYNA*/ STORE3_X6.set(YFAG, WUPA_STORE3_RSTn, XATU_SPRITE_X6);
  /*p31.WECO*/ STORE3_X7.set(YFAG, WUPA_STORE3_RSTn, BADY_SPRITE_X7);

  /*p29.WOFO*/ wire WOFO = not(DUKE_STORE4_CLKn);
  /*p29.WYLU*/ wire WYLU = not(DUKE_STORE4_CLKn);
  /*p29.EWOT*/ wire EWOT = not(DUKE_STORE4_CLKn);
  /*p31.WEDU*/ STORE4_X0.set(WUNU_STORE4_RSTn, WOFO, ZAGO_SPRITE_X0);
  /*p31.YGAJ*/ STORE4_X1.set(WUNU_STORE4_RSTn, WOFO, ZOCY_SPRITE_X1);
  /*p31.ZYJO*/ STORE4_X2.set(WUNU_STORE4_RSTn, WOFO, YPUR_SPRITE_X2);
  /*p31.XURY*/ STORE4_X3.set(WUNU_STORE4_RSTn, WOFO, YVOK_SPRITE_X3);
  /*p31.YBED*/ STORE4_X4.set(WUNU_STORE4_RSTn, WOFO, COSE_SPRITE_X4);
  /*p31.ZALA*/ STORE4_X5.set(WUNU_STORE4_RSTn, WOFO, AROP_SPRITE_X5);
  /*p31.WYDE*/ STORE4_X6.set(WUNU_STORE4_RSTn, WOFO, XATU_SPRITE_X6);
  /*p31.XEPA*/ STORE4_X7.set(WUNU_STORE4_RSTn, WOFO, BADY_SPRITE_X7);
  /*p30.XYNU*/ XYNU.set(WYLU, WEZA_TS_IDX_5);
  /*p30.XEGE*/ XEGE.set(WYLU, WUCO_TS_IDX_4);
  /*p30.XABO*/ XABO.set(WYLU, WYDA_TS_IDX_3);
  /*p30.WANU*/ WANU.set(WYLU, ZYSU_TS_IDX_2);
  /*p30.XEFE*/ XEFE.set(WYLU, WYSE_TS_IDX_1);
  /*p30.XAVE*/ XAVE.set(WYLU, WUZY_TS_IDX_0);
  /*p30.CUMU*/ CUMU.set(EWOT, WENU_TS_LINE_0);
  /*p30.CAPO*/ CAPO.set(EWOT, CUCU_TS_LINE_1);
  /*p30.CONO*/ CONO.set(EWOT, CUCA_TS_LINE_2);
  /*p30.CAJU*/ CAJU.set(EWOT, CEGA_TS_LINE_3);

  /*p29.CYLA*/ wire CYLA = not(BEDE_STORE5_CLKn);
  /*p29.DYMO*/ wire DYMO = not(BEDE_STORE5_CLKn);
  /*p29.BUCY*/ wire BUCY = not(BEDE_STORE5_CLKn);
  /*p31.FUSA*/ STORE5_X0.set(CYLA, EJAD_STORE5_RSTn, ZAGO_SPRITE_X0);
  /*p31.FAXA*/ STORE5_X1.set(CYLA, EJAD_STORE5_RSTn, ZOCY_SPRITE_X1);
  /*p31.FOZY*/ STORE5_X2.set(CYLA, EJAD_STORE5_RSTn, YPUR_SPRITE_X2);
  /*p31.FESY*/ STORE5_X3.set(CYLA, EJAD_STORE5_RSTn, YVOK_SPRITE_X3);
  /*p31.CYWE*/ STORE5_X4.set(CYLA, EJAD_STORE5_RSTn, COSE_SPRITE_X4);
  /*p31.DYBY*/ STORE5_X5.set(CYLA, EJAD_STORE5_RSTn, AROP_SPRITE_X5);
  /*p31.DURY*/ STORE5_X6.set(CYLA, EJAD_STORE5_RSTn, XATU_SPRITE_X6);
  /*p31.CUVY*/ STORE5_X7.set(CYLA, EJAD_STORE5_RSTn, BADY_SPRITE_X7);
  /*p30.EKAP*/ EKAP.set(DYMO, WEZA_TS_IDX_5);
  /*p30.ETAV*/ ETAV.set(DYMO, WUCO_TS_IDX_4);
  /*p30.EBEX*/ EBEX.set(DYMO, WYDA_TS_IDX_3);
  /*p30.GORU*/ GORU.set(DYMO, ZYSU_TS_IDX_2);
  /*p30.ETYM*/ ETYM.set(DYMO, WYSE_TS_IDX_1);
  /*p30.EKOP*/ EKOP.set(DYMO, WUZY_TS_IDX_0);
  /*p30.ANED*/ ANED.set(BUCY, WENU_TS_LINE_0);
  /*p30.ACEP*/ ACEP.set(BUCY, CUCU_TS_LINE_1);
  /*p30.ABUX*/ ABUX.set(BUCY, CUCA_TS_LINE_2);
  /*p30.ABEG*/ ABEG.set(BUCY, CEGA_TS_LINE_3);

  /*p29.ZAPE*/ wire ZAPE = not(WEKA_STORE6_CLKn);
  /*p29.WUSE*/ wire WUSE = not(WEKA_STORE6_CLKn);
  /*p29.ZURU*/ wire ZURU = not(WEKA_STORE6_CLKn);
  /*p31.YCOL*/ STORE6_X0.set(ZAPE, XAHO_STORE6_RSTn, ZAGO_SPRITE_X0);
  /*p31.YRAC*/ STORE6_X1.set(ZAPE, XAHO_STORE6_RSTn, ZOCY_SPRITE_X1);
  /*p31.YMEM*/ STORE6_X2.set(ZAPE, XAHO_STORE6_RSTn, YPUR_SPRITE_X2);
  /*p31.YVAG*/ STORE6_X3.set(ZAPE, XAHO_STORE6_RSTn, YVOK_SPRITE_X3);
  /*p31.ZOLY*/ STORE6_X4.set(ZAPE, XAHO_STORE6_RSTn, COSE_SPRITE_X4);
  /*p31.ZOGO*/ STORE6_X5.set(ZAPE, XAHO_STORE6_RSTn, AROP_SPRITE_X5);
  /*p31.ZECU*/ STORE6_X6.set(ZAPE, XAHO_STORE6_RSTn, XATU_SPRITE_X6);
  /*p31.ZESA*/ STORE6_X7.set(ZAPE, XAHO_STORE6_RSTn, BADY_SPRITE_X7);
  /*p30.GECU*/ GECU.set(WUSE, WEZA_TS_IDX_5);
  /*p30.FOXY*/ FOXY.set(WUSE, WUCO_TS_IDX_4);
  /*p30.GOHU*/ GOHU.set(WUSE, WYDA_TS_IDX_3);
  /*p30.FOGO*/ FOGO.set(WUSE, ZYSU_TS_IDX_2);
  /*p30.GACY*/ GACY.set(WUSE, WYSE_TS_IDX_1);
  /*p30.GABO*/ GABO.set(WUSE, WUZY_TS_IDX_0);
  /*p30.ZUBE*/ ZUBE.set(ZURU, WENU_TS_LINE_0);
  /*p30.ZUMY*/ ZUMY.set(ZURU, CUCU_TS_LINE_1);
  /*p30.ZEXO*/ ZEXO.set(ZURU, CUCA_TS_LINE_2);
  /*p30.ZAFU*/ ZAFU.set(ZURU, CEGA_TS_LINE_3);

  /*p29.GECY*/ wire GECY = not(GYVO_STORE7_CLKn);
  /*p29.FEFO*/ wire FEFO = not(GYVO_STORE7_CLKn);
  /*p29.WABE*/ wire WABE = not(GYVO_STORE7_CLKn);
  /*p31.ERAZ*/ STORE7_X0.set(GECY, GAFY_STORE7_RSTn, ZAGO_SPRITE_X0);
  /*p31.EPUM*/ STORE7_X1.set(GECY, GAFY_STORE7_RSTn, ZOCY_SPRITE_X1);
  /*p31.EROL*/ STORE7_X2.set(GECY, GAFY_STORE7_RSTn, YPUR_SPRITE_X2);
  /*p31.EHYN*/ STORE7_X3.set(GECY, GAFY_STORE7_RSTn, YVOK_SPRITE_X3);
  /*p31.FAZU*/ STORE7_X4.set(GECY, GAFY_STORE7_RSTn, COSE_SPRITE_X4);
  /*p31.FAXE*/ STORE7_X5.set(GECY, GAFY_STORE7_RSTn, AROP_SPRITE_X5);
  /*p31.EXUK*/ STORE7_X6.set(GECY, GAFY_STORE7_RSTn, XATU_SPRITE_X6);
  /*p31.FEDE*/ STORE7_X7.set(GECY, GAFY_STORE7_RSTn, BADY_SPRITE_X7);
  /*p30.FUZO*/ FUZO.set(FEFO, WEZA_TS_IDX_5);
  /*p30.GESY*/ GESY.set(FEFO, WUCO_TS_IDX_4);
  /*p30.FYSU*/ FYSU.set(FEFO, WYDA_TS_IDX_3);
  /*p30.FEFA*/ FEFA.set(FEFO, ZYSU_TS_IDX_2);
  /*p30.GYNO*/ GYNO.set(FEFO, WYSE_TS_IDX_1);
  /*p30.GULE*/ GULE.set(FEFO, WUZY_TS_IDX_0);
  /*p30.XYGO*/ XYGO.set(WABE, WENU_TS_LINE_0);
  /*p30.XYNA*/ XYNA.set(WABE, CUCU_TS_LINE_1);
  /*p30.XAKU*/ XAKU.set(WABE, CUCA_TS_LINE_2);
  /*p30.YGUM*/ YGUM.set(WABE, CEGA_TS_LINE_3);

  /*p29.CEXU*/ wire CEXU = not(BUKA_STORE8_CLKn);
  /*p29.AKOL*/ wire AKOL = not(BUKA_STORE8_CLKn);
  /*p29.BYMY*/ wire BYMY = not(BUKA_STORE8_CLKn);
  /*p31.GAVY*/ STORE8_X4.set(CEXU, WUZO_STORE8_RSTn, ZAGO_SPRITE_X0);
  /*p31.GYPU*/ STORE8_X5.set(CEXU, WUZO_STORE8_RSTn, ZOCY_SPRITE_X1);
  /*p31.GADY*/ STORE8_X6.set(CEXU, WUZO_STORE8_RSTn, YPUR_SPRITE_X2);
  /*p31.GAZA*/ STORE8_X7.set(CEXU, WUZO_STORE8_RSTn, YVOK_SPRITE_X3);
  /*p31.EZUF*/ STORE8_X0.set(CEXU, WUZO_STORE8_RSTn, COSE_SPRITE_X4);
  /*p31.ENAD*/ STORE8_X1.set(CEXU, WUZO_STORE8_RSTn, AROP_SPRITE_X5);
  /*p31.EBOW*/ STORE8_X2.set(CEXU, WUZO_STORE8_RSTn, XATU_SPRITE_X6);
  /*p31.FYCA*/ STORE8_X3.set(CEXU, WUZO_STORE8_RSTn, BADY_SPRITE_X7);
  /*p30.AXUV*/ STORE8_IDX5.set(AKOL, WEZA_TS_IDX_5);
  /*p30.BADA*/ STORE8_IDX4.set(AKOL, WUCO_TS_IDX_4);
  /*p30.APEV*/ STORE8_IDX3.set(AKOL, WYDA_TS_IDX_3);
  /*p30.BADO*/ STORE8_IDX2.set(AKOL, ZYSU_TS_IDX_2);
  /*p30.BEXY*/ STORE8_IDX1.set(AKOL, WYSE_TS_IDX_1);
  /*p30.BYHE*/ STORE8_IDX0.set(AKOL, WUZY_TS_IDX_0);
  /*p30.AFYM*/ STORE8_LINE0.set(BYMY, WENU_TS_LINE_0);
  /*p30.AZAP*/ STORE8_LINE1.set(BYMY, CUCU_TS_LINE_1);
  /*p30.AFUT*/ STORE8_LINE2.set(BYMY, CUCA_TS_LINE_2);
  /*p30.AFYX*/ STORE8_LINE3.set(BYMY, CEGA_TS_LINE_3);

  /*p29.WEME*/ wire STORE9_CLKa = not(DECU_STORE9_CLKn);
  /*p29.WUFA*/ wire STORE9_CLKb = not(DECU_STORE9_CLKn);
  /*p29.FAKA*/ wire STORE9_CLKc = not(DECU_STORE9_CLKn);
  /*p31.XUVY*/ STORE9_X0.set(STORE9_CLKa, DOSY_STORE9_RSTn, ZAGO_SPRITE_X0);
  /*p31.XERE*/ STORE9_X1.set(STORE9_CLKa, DOSY_STORE9_RSTn, ZOCY_SPRITE_X1);
  /*p31.XUZO*/ STORE9_X2.set(STORE9_CLKa, DOSY_STORE9_RSTn, YPUR_SPRITE_X2);
  /*p31.XEXA*/ STORE9_X3.set(STORE9_CLKa, DOSY_STORE9_RSTn, YVOK_SPRITE_X3);
  /*p31.YPOD*/ STORE9_X4.set(STORE9_CLKa, DOSY_STORE9_RSTn, COSE_SPRITE_X4);
  /*p31.YROP*/ STORE9_X5.set(STORE9_CLKa, DOSY_STORE9_RSTn, AROP_SPRITE_X5);
  /*p31.YNEP*/ STORE9_X6.set(STORE9_CLKa, DOSY_STORE9_RSTn, XATU_SPRITE_X6);
  /*p31.YZOF*/ STORE9_X7.set(STORE9_CLKa, DOSY_STORE9_RSTn, BADY_SPRITE_X7);
  /*p30.XUFO*/ STORE9_IDX0.set(STORE9_CLKb, WEZA_TS_IDX_5);
  /*p30.XUTE*/ STORE9_IDX1.set(STORE9_CLKb, WUCO_TS_IDX_4);
  /*p30.XOTU*/ STORE9_IDX2.set(STORE9_CLKb, WYDA_TS_IDX_3);
  /*p30.XYFE*/ STORE9_IDX3.set(STORE9_CLKb, ZYSU_TS_IDX_2);
  /*p30.YZOR*/ STORE9_IDX4.set(STORE9_CLKb, WYSE_TS_IDX_1);
  /*p30.YBER*/ STORE9_IDX5.set(STORE9_CLKb, WUZY_TS_IDX_0);
  /*p30.DEWU*/ STORE9_LINE0.set(STORE9_CLKc, WENU_TS_LINE_0);
  /*p30.CANA*/ STORE9_LINE1.set(STORE9_CLKc, CUCU_TS_LINE_1);
  /*p30.DYSY*/ STORE9_LINE2.set(STORE9_CLKc, CUCA_TS_LINE_2);
  /*p30.FOFO*/ STORE9_LINE3.set(STORE9_CLKc, CEGA_TS_LINE_3);
}

bool SpriteStoreRegisters::commit() {
  bool changed = false;
  /*p28.BESU*/ changed |= BESU_SCANNINGp.commit_latch();
  /*p29.CENO*/ changed |= CENO_SCANNINGp.commit_reg();
  /*p28.YFEL*/ changed |= SCAN0.commit_reg();
  /*p28.WEWY*/ changed |= SCAN1.commit_reg();
  /*p28.GOSO*/ changed |= SCAN2.commit_reg();
  /*p28.ELYN*/ changed |= SCAN3.commit_reg();
  /*p28.FAHA*/ changed |= SCAN4.commit_reg();
  /*p28.FONY*/ changed |= SCAN5.commit_reg();
  /*p30.XADU*/ changed |= XADU_SPRITE_IDX0.commit_reg();
  /*p30.XEDY*/ changed |= XEDY_SPRITE_IDX1.commit_reg();
  /*p30.ZUZE*/ changed |= ZUZE_SPRITE_IDX2.commit_reg();
  /*p30.XOBE*/ changed |= XOBE_SPRITE_IDX3.commit_reg();
  /*p30.YDUF*/ changed |= YDUF_SPRITE_IDX4.commit_reg();
  /*p30.XECU*/ changed |= XECU_SPRITE_IDX5.commit_reg();
  /*p29.BYBA*/ changed |= SCAN_DONE_d4.commit_reg();
  /*p29.DOBA*/ changed |= SCAN_DONE_d5.commit_reg();
  /*p30.WUZY*/ changed |= WUZY_TS_IDX_0.commit_tribuf();
  /*p30.WYSE*/ changed |= WYSE_TS_IDX_1.commit_tribuf();
  /*p30.ZYSU*/ changed |= ZYSU_TS_IDX_2.commit_tribuf();
  /*p30.WYDA*/ changed |= WYDA_TS_IDX_3.commit_tribuf();
  /*p30.WUCO*/ changed |= WUCO_TS_IDX_4.commit_tribuf();
  /*p30.WEZA*/ changed |= WEZA_TS_IDX_5.commit_tribuf();
  /*p30.WENU*/ changed |= WENU_TS_LINE_0.commit_tribuf();
  /*p30.CUCU*/ changed |= CUCU_TS_LINE_1.commit_tribuf();
  /*p30.CUCA*/ changed |= CUCA_TS_LINE_2.commit_tribuf();
  /*p30.CEGA*/ changed |= CEGA_TS_LINE_3.commit_tribuf();
  /*p29.DEZY*/ changed |= DEZY_CLKp.commit_reg();
  /*p29.BESE*/ changed |= SPRITE_COUNT0.commit_reg();
  /*p29.CUXY*/ changed |= SPRITE_COUNT1.commit_reg();
  /*p29.BEGO*/ changed |= SPRITE_COUNT2.commit_reg();
  /*p29.DYBE*/ changed |= SPRITE_COUNT3.commit_reg();
  /*p29.EBOJ*/ changed |= EBOJ_STORE0_RSTp.commit_reg();
  /*p30.YGUS*/ changed |= YGUS_STORE0_IDX5.commit_reg();
  /*p30.YSOK*/ changed |= YSOK_STORE0_IDX4.commit_reg();
  /*p30.YZEP*/ changed |= YZEP_STORE0_IDX3.commit_reg();
  /*p30.WYTE*/ changed |= WYTE_STORE0_IDX2.commit_reg();
  /*p30.ZONY*/ changed |= ZONY_STORE0_IDX1.commit_reg();
  /*p30.YWAK*/ changed |= YWAK_STORE0_IDX0.commit_reg();
  /*p30.FYHY*/ changed |= STORE0_LINE0.commit_reg();
  /*p30.GYHO*/ changed |= STORE0_LINE1.commit_reg();
  /*p30.BOZU*/ changed |= STORE0_LINE2.commit_reg();
  /*p30.CUFO*/ changed |= STORE0_LINE3.commit_reg();
  /*p31.XEPE*/ changed |= STORE0_X0.commit_reg();
  /*p31.YLAH*/ changed |= STORE0_X1.commit_reg();
  /*p31.ZOLA*/ changed |= STORE0_X2.commit_reg();
  /*p31.ZULU*/ changed |= STORE0_X3.commit_reg();
  /*p31.WELO*/ changed |= STORE0_X4.commit_reg();
  /*p31.XUNY*/ changed |= STORE0_X5.commit_reg();
  /*p31.WOTE*/ changed |= STORE0_X6.commit_reg();
  /*p31.XAKO*/ changed |= STORE0_X7.commit_reg();
  /*p29.CEDY*/ changed |= CEDY_STORE1_RSTp.commit_reg();
  /*p30.CAJY*/ changed |= STORE1_IDX0.commit_reg();
  /*p30.CUZA*/ changed |= STORE1_IDX1.commit_reg();
  /*p30.COMA*/ changed |= STORE1_IDX2.commit_reg();
  /*p30.CUFA*/ changed |= STORE1_IDX3.commit_reg();
  /*p30.CEBO*/ changed |= STORE1_IDX4.commit_reg();
  /*p30.CADU*/ changed |= STORE1_IDX5.commit_reg();
  /*p30.ABUG*/ changed |= STORE1_LINE0.commit_reg();
  /*p30.AMES*/ changed |= STORE1_LINE1.commit_reg();
  /*p30.ABOP*/ changed |= STORE1_LINE2.commit_reg();
  /*p30.AROF*/ changed |= STORE1_LINE3.commit_reg();
  /*p31.DANY*/ changed |= STORE1_X0.commit_reg();
  /*p31.DUKO*/ changed |= STORE1_X1.commit_reg();
  /*p31.DESU*/ changed |= STORE1_X2.commit_reg();
  /*p31.DAZO*/ changed |= STORE1_X3.commit_reg();
  /*p31.DAKE*/ changed |= STORE1_X4.commit_reg();
  /*p31.CESO*/ changed |= STORE1_X5.commit_reg();
  /*p31.DYFU*/ changed |= STORE1_X6.commit_reg();
  /*p31.CUSY*/ changed |= STORE1_X7.commit_reg();
  /*p29.EGAV*/ changed |= EGAV_STORE2_RSTp.commit_reg();
  /*p30.BOXA*/ changed |= STORE2_IDX0.commit_reg();
  /*p30.BUNA*/ changed |= STORE2_IDX1.commit_reg();
  /*p30.BULU*/ changed |= STORE2_IDX2.commit_reg();
  /*p30.BECA*/ changed |= STORE2_IDX3.commit_reg();
  /*p30.BYHU*/ changed |= STORE2_IDX4.commit_reg();
  /*p30.BUHE*/ changed |= STORE2_IDX5.commit_reg();
  /*p30.YKUK*/ changed |= STORE2_LINE0.commit_reg();
  /*p30.YLOV*/ changed |= STORE2_LINE1.commit_reg();
  /*p30.XAZY*/ changed |= STORE2_LINE2.commit_reg();
  /*p30.XOSY*/ changed |= STORE2_LINE3.commit_reg();
  /*p31.FOKA*/ changed |= STORE2_X0.commit_reg();
  /*p31.FYTY*/ changed |= STORE2_X1.commit_reg();
  /*p31.FUBY*/ changed |= STORE2_X2.commit_reg();
  /*p31.GOXU*/ changed |= STORE2_X3.commit_reg();
  /*p31.DUHY*/ changed |= STORE2_X4.commit_reg();
  /*p31.EJUF*/ changed |= STORE2_X5.commit_reg();
  /*p31.ENOR*/ changed |= STORE2_X6.commit_reg();
  /*p31.DEPY*/ changed |= STORE2_X7.commit_reg();
  /*p29.GOTA*/ changed |= GOTA_STORE3_RSTp.commit_reg();
  /*p30.DAFU*/ changed |= STORE3_IDX0.commit_reg();
  /*p30.DEBA*/ changed |= STORE3_IDX1.commit_reg();
  /*p30.DUHA*/ changed |= STORE3_IDX2.commit_reg();
  /*p30.DUNY*/ changed |= STORE3_IDX3.commit_reg();
  /*p30.DESE*/ changed |= STORE3_IDX4.commit_reg();
  /*p30.DEVY*/ changed |= STORE3_IDX5.commit_reg();
  /*p30.ZURY*/ changed |= STORE3_LINE0.commit_reg();
  /*p30.ZURO*/ changed |= STORE3_LINE1.commit_reg();
  /*p30.ZENE*/ changed |= STORE3_LINE2.commit_reg();
  /*p30.ZYLU*/ changed |= STORE3_LINE4.commit_reg();
  /*p31.XOLY*/ changed |= STORE3_X0.commit_reg();
  /*p31.XYBA*/ changed |= STORE3_X1.commit_reg();
  /*p31.XABE*/ changed |= STORE3_X2.commit_reg();
  /*p31.XEKA*/ changed |= STORE3_X3.commit_reg();
  /*p31.XOMY*/ changed |= STORE3_X4.commit_reg();
  /*p31.WUHA*/ changed |= STORE3_X5.commit_reg();
  /*p31.WYNA*/ changed |= STORE3_X6.commit_reg();
  /*p31.WECO*/ changed |= STORE3_X7.commit_reg();
  /*p29.XUDY*/ changed |= XUDY_STORE4_RSTp.commit_reg();
  /*p31.WEDU*/ changed |= STORE4_X0.commit_reg();
  /*p31.YGAJ*/ changed |= STORE4_X1.commit_reg();
  /*p31.ZYJO*/ changed |= STORE4_X2.commit_reg();
  /*p31.XURY*/ changed |= STORE4_X3.commit_reg();
  /*p31.YBED*/ changed |= STORE4_X4.commit_reg();
  /*p31.ZALA*/ changed |= STORE4_X5.commit_reg();
  /*p31.WYDE*/ changed |= STORE4_X6.commit_reg();
  /*p31.XEPA*/ changed |= STORE4_X7.commit_reg();
  /*p30.XYNU*/ changed |= XYNU.commit_reg();
  /*p30.XEGE*/ changed |= XEGE.commit_reg();
  /*p30.XABO*/ changed |= XABO.commit_reg();
  /*p30.WANU*/ changed |= WANU.commit_reg();
  /*p30.XEFE*/ changed |= XEFE.commit_reg();
  /*p30.XAVE*/ changed |= XAVE.commit_reg();
  /*p30.CUMU*/ changed |= CUMU.commit_reg();
  /*p30.CAPO*/ changed |= CAPO.commit_reg();
  /*p30.CONO*/ changed |= CONO.commit_reg();
  /*p30.CAJU*/ changed |= CAJU.commit_reg();
  /*p29.WAFY*/ changed |= WAFY_STORE5_RSTp.commit_reg();
  /*p31.FUSA*/ changed |= STORE5_X0.commit_reg();
  /*p31.FAXA*/ changed |= STORE5_X1.commit_reg();
  /*p31.FOZY*/ changed |= STORE5_X2.commit_reg();
  /*p31.FESY*/ changed |= STORE5_X3.commit_reg();
  /*p31.CYWE*/ changed |= STORE5_X4.commit_reg();
  /*p31.DYBY*/ changed |= STORE5_X5.commit_reg();
  /*p31.DURY*/ changed |= STORE5_X6.commit_reg();
  /*p31.CUVY*/ changed |= STORE5_X7.commit_reg();
  /*p30.EKAP*/ changed |= EKAP.commit_reg();
  /*p30.ETAV*/ changed |= ETAV.commit_reg();
  /*p30.EBEX*/ changed |= EBEX.commit_reg();
  /*p30.GORU*/ changed |= GORU.commit_reg();
  /*p30.ETYM*/ changed |= ETYM.commit_reg();
  /*p30.EKOP*/ changed |= EKOP.commit_reg();
  /*p30.ANED*/ changed |= ANED.commit_reg();
  /*p30.ACEP*/ changed |= ACEP.commit_reg();
  /*p30.ABUX*/ changed |= ABUX.commit_reg();
  /*p30.ABEG*/ changed |= ABEG.commit_reg();
  /*p29.WOMY*/ changed |= WOMY_STORE6_RSTp.commit_reg();
  /*p31.YCOL*/ changed |= STORE6_X0.commit_reg();
  /*p31.YRAC*/ changed |= STORE6_X1.commit_reg();
  /*p31.YMEM*/ changed |= STORE6_X2.commit_reg();
  /*p31.YVAG*/ changed |= STORE6_X3.commit_reg();
  /*p31.ZOLY*/ changed |= STORE6_X4.commit_reg();
  /*p31.ZOGO*/ changed |= STORE6_X5.commit_reg();
  /*p31.ZECU*/ changed |= STORE6_X6.commit_reg();
  /*p31.ZESA*/ changed |= STORE6_X7.commit_reg();
  /*p30.GECU*/ changed |= GECU.commit_reg();
  /*p30.FOXY*/ changed |= FOXY.commit_reg();
  /*p30.GOHU*/ changed |= GOHU.commit_reg();
  /*p30.FOGO*/ changed |= FOGO.commit_reg();
  /*p30.GACY*/ changed |= GACY.commit_reg();
  /*p30.GABO*/ changed |= GABO.commit_reg();
  /*p30.ZUBE*/ changed |= ZUBE.commit_reg();
  /*p30.ZUMY*/ changed |= ZUMY.commit_reg();
  /*p30.ZEXO*/ changed |= ZEXO.commit_reg();
  /*p30.ZAFU*/ changed |= ZAFU.commit_reg();
  /*p29.WAPO*/ changed |= WAPO_STORE7_RSTp.commit_reg();
  /*p31.ERAZ*/ changed |= STORE7_X0.commit_reg();
  /*p31.EPUM*/ changed |= STORE7_X1.commit_reg();
  /*p31.EROL*/ changed |= STORE7_X2.commit_reg();
  /*p31.EHYN*/ changed |= STORE7_X3.commit_reg();
  /*p31.FAZU*/ changed |= STORE7_X4.commit_reg();
  /*p31.FAXE*/ changed |= STORE7_X5.commit_reg();
  /*p31.EXUK*/ changed |= STORE7_X6.commit_reg();
  /*p31.FEDE*/ changed |= STORE7_X7.commit_reg();
  /*p30.FUZO*/ changed |= FUZO.commit_reg();
  /*p30.GESY*/ changed |= GESY.commit_reg();
  /*p30.FYSU*/ changed |= FYSU.commit_reg();
  /*p30.FEFA*/ changed |= FEFA.commit_reg();
  /*p30.GYNO*/ changed |= GYNO.commit_reg();
  /*p30.GULE*/ changed |= GULE.commit_reg();
  /*p30.XYGO*/ changed |= XYGO.commit_reg();
  /*p30.XYNA*/ changed |= XYNA.commit_reg();
  /*p30.XAKU*/ changed |= XAKU.commit_reg();
  /*p30.YGUM*/ changed |= YGUM.commit_reg();
  /*p29.EXUQ*/ changed |= EXUQ_STORE8_RSTp.commit_reg();
  /*p31.GAVY*/ changed |= STORE8_X4.commit_reg();
  /*p31.GYPU*/ changed |= STORE8_X5.commit_reg();
  /*p31.GADY*/ changed |= STORE8_X6.commit_reg();
  /*p31.GAZA*/ changed |= STORE8_X7.commit_reg();
  /*p31.EZUF*/ changed |= STORE8_X0.commit_reg();
  /*p31.ENAD*/ changed |= STORE8_X1.commit_reg();
  /*p31.EBOW*/ changed |= STORE8_X2.commit_reg();
  /*p31.FYCA*/ changed |= STORE8_X3.commit_reg();
  /*p30.AXUV*/ changed |= STORE8_IDX5.commit_reg();
  /*p30.BADA*/ changed |= STORE8_IDX4.commit_reg();
  /*p30.APEV*/ changed |= STORE8_IDX3.commit_reg();
  /*p30.BADO*/ changed |= STORE8_IDX2.commit_reg();
  /*p30.BEXY*/ changed |= STORE8_IDX1.commit_reg();
  /*p30.BYHE*/ changed |= STORE8_IDX0.commit_reg();
  /*p30.AFYM*/ changed |= STORE8_LINE0.commit_reg();
  /*p30.AZAP*/ changed |= STORE8_LINE1.commit_reg();
  /*p30.AFUT*/ changed |= STORE8_LINE2.commit_reg();
  /*p30.AFYX*/ changed |= STORE8_LINE3.commit_reg();
  /*p29.FONO*/ changed |= FONO_STORE9_RSTp.commit_reg();
  /*p31.XUVY*/ changed |= STORE9_X0.commit_reg();
  /*p31.XERE*/ changed |= STORE9_X1.commit_reg();
  /*p31.XUZO*/ changed |= STORE9_X2.commit_reg();
  /*p31.XEXA*/ changed |= STORE9_X3.commit_reg();
  /*p31.YPOD*/ changed |= STORE9_X4.commit_reg();
  /*p31.YROP*/ changed |= STORE9_X5.commit_reg();
  /*p31.YNEP*/ changed |= STORE9_X6.commit_reg();
  /*p31.YZOF*/ changed |= STORE9_X7.commit_reg();
  /*p30.XUFO*/ changed |= STORE9_IDX0.commit_reg();
  /*p30.XUTE*/ changed |= STORE9_IDX1.commit_reg();
  /*p30.XOTU*/ changed |= STORE9_IDX2.commit_reg();
  /*p30.XYFE*/ changed |= STORE9_IDX3.commit_reg();
  /*p30.YZOR*/ changed |= STORE9_IDX4.commit_reg();
  /*p30.YBER*/ changed |= STORE9_IDX5.commit_reg();
  /*p30.DEWU*/ changed |= STORE9_LINE0.commit_reg();
  /*p30.CANA*/ changed |= STORE9_LINE1.commit_reg();
  /*p30.DYSY*/ changed |= STORE9_LINE2.commit_reg();
  /*p30.FOFO*/ changed |= STORE9_LINE3.commit_reg();
  return changed;
}