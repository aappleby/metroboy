#include "Sch_Sprites.h"
#include "Sch_Gameboy.h"
#include "Sch_SpriteStore.h"
#include "Sch_Video.h"
#include "Sch_System.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void SpriteRegisters::reset() {
#if 0
  //P10_Bn = false;
  //OAM_ADDR_PARSE = false;
  STORE_SPRITE_IDXn.val = false; STORE_SPRITE_IDXn.clk = false;

  SCAN0.val = true;  SCAN0.clk = true;
  SCAN1.val = true;  SCAN1.clk = false;
  SCAN2.val = true;  SCAN2.clk = false;
  SCAN3.val = false; SCAN3.clk = false;
  SCAN4.val = false; SCAN4.clk = true;
  SCAN5.val = true;  SCAN5.clk = true;

  SCAN_DONE_d0 = true;

  SPRITE_IDX0.val = false; SPRITE_IDX0.clk = false;
  SPRITE_IDX1.val = false; SPRITE_IDX1.clk = false;
  SPRITE_IDX2.val = false; SPRITE_IDX2.clk = false;
  SPRITE_IDX3.val = false; SPRITE_IDX3.clk = false;
  SPRITE_IDX4.val = false; SPRITE_IDX4.clk = false;
  SPRITE_IDX5.val = false; SPRITE_IDX5.clk = false;

  SCAN_DONE_d4.val = true; SCAN_DONE_d4.clk = false;
  SCAN_DONE_d5.val = true; SCAN_DONE_d5.clk = true;
  SCAN_DONE_d0_TRIG = false;

  SPR_SEQ0.val = false; SPR_SEQ0.clk = false;
  SPR_SEQ1.val = false; SPR_SEQ1.clk = false;
  SPR_SEQ2.val = false; SPR_SEQ2.clk = false;

  SEQ_xx23xx.val  = false; SEQ_xx23xx.clk  = false;
  SEQ_xxx34xn.val = false; SEQ_xxx34xn.clk = false;
  SEQ_xxxx45n.val = false; SEQ_xxxx45n.clk = false;
  SEQ_B0d.val = false; SEQ_B0d.clk = false;

  //SPRITE_PIX_LATCH_B = false;
  //SPRITE_PIX_LATCH_A = false;
  //SPRITE_DONE = false;
  //STORE_EN = false;
  //MATCH_EN = false;
  //SPRITE_READ = false;
#endif
}

uint32_t SpriteRegisters::index() const {
  return (SCAN0 << 0) | (SCAN1 << 1) | (SCAN2 << 2) | (SCAN3 << 3) | (SCAN4 << 4) | (SCAN5 << 5);
}

//-----------------------------------------------------------------------------

bool SpriteRegisters::MATCH_EN(const VidRegisters& vid_reg, const VidConfig& vid_reg2) const {
  /*p29.CEHA*/ wire CEHA = not(!STORE_SPRITE_IDXn);
  /*p29.BYJO*/ wire BYJO = not(CEHA);
  /*p29.AZEM*/ wire AZEM = and(BYJO, vid_reg.XYMU_RENDERING_LATCH);
  /*p29.AROR*/ wire MATCH_EN = and(AZEM, vid_reg2.LCDC_SPEN);
  return MATCH_EN;
}

//-----------------------------------------------------------------------------

void SpriteRegisters::sprite_seq(const SysSignals& sys_sig,
                                 const ClkSignals& clk_sig,
                                 const RstSignals& rst_sig,
                                 const VidSignals& vid_sig,
                                 const VidRegisters& vid_reg,
                                 const LcdSignals& lcd_sig) {
  /*p01.ROSY*/ wire VID_RESET5 = not(rst_sig.VID_RESETn);
  /*p01.LAPE*/ wire LAPE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.TAVA*/ wire TAVA_AxCxExGx = not(LAPE_xBxDxFxH);

  /*p29.TAME*/ wire SEQ_5n = nand(SPR_SEQ2, SPR_SEQ0);

  ///*p29.TOMA*/ SEQ_CLK = or(AxCxExGx, SEQ_5);

  /*p29.TOMA*/ wire TOMA_AxCxExGx = nand(LAPE_xBxDxFxH, SEQ_5n);
  /*p27.SECA*/ wire SPR_SEQ_RST = nor(vid_sig.RYCE_SPRITE_FETCH_TRIG, VID_RESET5, lcd_sig.BYHA_VID_LINE_TRIG_d4n);
  /*p29.TOXE*/ SPR_SEQ0.set(TOMA_AxCxExGx, SPR_SEQ_RST, !SPR_SEQ0);
  /*p29.TULY*/ SPR_SEQ1.set(!SPR_SEQ0,     SPR_SEQ_RST, !SPR_SEQ1);
  /*p29.TESE*/ SPR_SEQ2.set(!SPR_SEQ1,     SPR_SEQ_RST, !SPR_SEQ2);

  /*p29.TYFO*/ SEQ_B0d    .set(LAPE_xBxDxFxH, sys_sig.P10_Bn,               SPR_SEQ0);
  /*p29.TOBU*/ SEQ_xx23xx .set(TAVA_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, SPR_SEQ1);    // note input is seq 1 not 2
  /*p29.VONU*/ SEQ_xxx34xn.set(TAVA_AxCxExGx, vid_reg.XYMU_RENDERING_LATCH, SEQ_xx23xx);
  /*p29.SEBA*/ SEQ_xxxx45n.set(LAPE_xBxDxFxH, vid_reg.XYMU_RENDERING_LATCH, SEQ_xxx34xn); // is this clock wrong?
}

//-----------------------------------------------------------------------------

SpriteSignals SpriteRegisters::tick(const ClkSignals& clk_sig,
                                    const SysSignals& sys_sig,
                                    const VclkSignals& vid_clk,
                                    const RstSignals& rst_sig,
                                    const DmaRegisters& dma_reg,
                                    const LcdSignals& lcd_sig,
                                    const LcdRegisters& lcd_reg,
                                    const VidConfig& vid_cfg,
                                    const VidRegisters& vid_reg,
                                    const OamRegisters& oam_reg,
                                    const OamPins& oam_pins,
                                    SpriteTristate& sil_tri,
                                    bool STORE_MATCH) {

  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIG = scanner(clk_sig, vid_clk, rst_sig, lcd_sig);

  /*p01.ROSY*/ wire VID_RESET5 = not(rst_sig.VID_RESETn);
  /*p01.ATAR*/ wire VID_RESET6 = not(rst_sig.VID_RESETn);
  /*p01.ABEZ*/ wire VID_RESETn3 = not(VID_RESET6);

  /*p28.ASEN*/ wire ASEN = or(VID_RESET6, AVAP_SCAN_DONE_d0_TRIG);
  /*p28.BESU*/ wire BESU = or(lcd_sig.CATU_VID_LINE_d4, ASEN);


  /*p28.BOGE*/ wire DMA_RW_ENn = not(dma_reg.REG_DMA_RW_EN);
  /*p28.ACYL*/ wire ACYL_OAM_ADDR_PARSE = and(DMA_RW_ENn, BESU);


  /*p29.AROR*/ wire MATCH_EN = this->MATCH_EN(vid_reg, vid_cfg);

  //----------
  // Sequencer
  
  {
    /*p29.TYTU*/ wire SPR_SEQ0n = not(SPR_SEQ0);
    /*p24.LOBY*/ wire LOBY_RENDERINGn = not(vid_reg.XYMU_RENDERING_LATCH);
    /*p29.SYCU*/ wire SYCU = nor(SPR_SEQ0n, LOBY_RENDERINGn, SEQ_B0d);

    /*p29.TOPU*/ wire SPRITE_PIX_LATCH_Ao = and(SPR_SEQ1, SYCU);
    /*p29.VYWA*/ wire SPRITE_PIX_LATCH_Ab = not(SPRITE_PIX_LATCH_Ao);
    /*p29.WENY*/ wire SPRITE_PIX_LATCH_An = not(SPRITE_PIX_LATCH_Ab);
    /*p29.XADO*/ wire SPRITE_PIX_LATCH_A  = not(SPRITE_PIX_LATCH_An);

    /*p29.RACA*/ wire SPRITE_PIX_LATCH_Bo = and(SEQ_xxx34xn, SYCU);
    /*p29.PEBY*/ wire SPRITE_PIX_LATCH_Bb = not(SPRITE_PIX_LATCH_Bo);
    /*p29.NYBE*/ wire SPRITE_PIX_LATCH_Bn = not(SPRITE_PIX_LATCH_Bb);
    /*p29.PUCO*/ wire SPRITE_PIX_LATCH_B  = not(SPRITE_PIX_LATCH_Bn);

    /*p29.TYNO*/ wire TYNO = nand(SPR_SEQ0, SEQ_xxxx45n, SEQ_xxx34xn);
    /*p29.VUSA*/ wire VUSA = or(!SEQ_B0d, TYNO);
    /*p29.WUTY*/ wire WUTY_SPRITE_DONE = not(VUSA);
  }

  {
    /*p29.SAKY*/ wire SAKY = nor(SPR_SEQ1, SEQ_xxx34xn);
    /*p29.TEPA*/ wire RENDERINGn = not(vid_reg.XYMU_RENDERING_LATCH);
    /*p29.TYSO*/ wire SPRITE_READq = or(SAKY, RENDERINGn); // seems wrong
    /*p29.TEXY*/ wire SPRITE_READ = not(SPRITE_READq);
  }
    
  ///*p29.XONO*/ wire FLIP_X = and(oam.OAM_A_D5, SPRITE_READn);

  //----------
  // Sprite y comparator

  SpriteDelta spr_delta = storeMatchY(sys_sig, lcd_reg, oam_reg, vid_cfg);

  /*p29.CEHA*/ wire CEHA = not(!STORE_SPRITE_IDXn);
  /*p29.CARE*/ wire STORE_ENn = or(vid_clk.XOCE_ABxxEFxx, CEHA, spr_delta.SPR_MATCH_Y);
  /*p29.DYTY*/ wire STORE_EN = not(STORE_ENn);

  /*p28.WEFE*/ wire WEFE = not(sys_sig.PIN_P10_B);
  /*p30.CYKE*/ wire CYKE = not(vid_clk.XUPY_xBCxxFGx);
  /*p30.WUDA*/ wire WUDA = not(CYKE);
  /*p30.XADU*/ SPRITE_IDX0.set(WUDA, WEFE, oam_pins.A2);
  /*p30.XEDY*/ SPRITE_IDX1.set(WUDA, WEFE, oam_pins.A3);
  /*p30.ZUZE*/ SPRITE_IDX2.set(WUDA, WEFE, oam_pins.A4);
  /*p30.XOBE*/ SPRITE_IDX3.set(WUDA, WEFE, oam_pins.A5);
  /*p30.YDUF*/ SPRITE_IDX4.set(WUDA, WEFE, oam_pins.A6);
  /*p30.XECU*/ SPRITE_IDX5.set(WUDA, WEFE, oam_pins.A7);

  /*p29.CENO*/ STORE_SPRITE_IDXn.set(vid_clk.XUPY_xBCxxFGx, VID_RESETn3, BESU);
  /*p29.BUZA*/ wire STORE_SPRITE_IDX = and(!STORE_SPRITE_IDXn, vid_reg.XYMU_RENDERING_LATCH);

  /*p30.WENU*/ if (STORE_MATCH)      sil_tri.TS_LINE_0 = spr_delta.SPRITE_DELTA1; // order wrong
  /*p30.CUCU*/ if (STORE_MATCH)      sil_tri.TS_LINE_1 = spr_delta.SPRITE_DELTA2;
  /*p30.CUCA*/ if (STORE_MATCH)      sil_tri.TS_LINE_2 = spr_delta.SPRITE_DELTA0;
  /*p30.CEGA*/ if (STORE_MATCH)      sil_tri.TS_LINE_3 = spr_delta.SPRITE_DELTA3;
  /*p30.WEZA*/ if (STORE_SPRITE_IDX) sil_tri.TS_IDX_0  = SPRITE_IDX5; // order wrong
  /*p30.WUCO*/ if (STORE_SPRITE_IDX) sil_tri.TS_IDX_1  = SPRITE_IDX4;
  /*p30.WYDA*/ if (STORE_SPRITE_IDX) sil_tri.TS_IDX_2  = SPRITE_IDX3;
  /*p30.ZYSU*/ if (STORE_SPRITE_IDX) sil_tri.TS_IDX_3  = SPRITE_IDX2;
  /*p30.WYSE*/ if (STORE_SPRITE_IDX) sil_tri.TS_IDX_4  = SPRITE_IDX1;
  /*p30.WUZY*/ if (STORE_SPRITE_IDX) sil_tri.TS_IDX_5  = SPRITE_IDX0;

  return {
  };
}

//-----------------------------------------------------------------------------

SpriteDelta SpriteRegisters::storeMatchY(const SysSignals& sys_sig,
                                         const LcdRegisters& lcd_reg,
                                         const OamRegisters& oam_reg,
                                         const VidConfig& vid_reg2) {
  /*p29.EBOS*/ wire V0n = not(lcd_reg.Y0);
  /*p29.DASA*/ wire V1n = not(lcd_reg.Y1);
  /*p29.FUKY*/ wire V2n = not(lcd_reg.Y2);
  /*p29.FUVE*/ wire V3n = not(lcd_reg.Y3);
  /*p29.FEPU*/ wire V4n = not(lcd_reg.Y4);
  /*p29.FOFA*/ wire V5n = not(lcd_reg.Y5);
  /*p29.FEMO*/ wire V6n = not(lcd_reg.Y6);
  /*p29.GUSU*/ wire V7n = not(lcd_reg.Y7);

  /*p29.ERUC*/ wire YDIFF0   = add_c(V0n, !oam_reg.REG_OAM_B0, sys_sig.PIN_P10_B);
  /*p29.ERUC*/ wire YDIFF_C0 = add_s(V0n, !oam_reg.REG_OAM_B0, sys_sig.PIN_P10_B);
  /*p29.ENEF*/ wire YDIFF1   = add_s(V1n, !oam_reg.REG_OAM_B1, YDIFF_C0);
  /*p29.ENEF*/ wire YDIFF_C1 = add_c(V1n, !oam_reg.REG_OAM_B1, YDIFF_C0);
  /*p29.FECO*/ wire YDIFF2   = add_s(V2n, !oam_reg.REG_OAM_B2, YDIFF_C1);
  /*p29.FECO*/ wire YDIFF_C2 = add_c(V2n, !oam_reg.REG_OAM_B2, YDIFF_C1);
  /*p29.GYKY*/ wire YDIFF3   = add_s(V3n, !oam_reg.REG_OAM_B3, YDIFF_C2);
  /*p29.GYKY*/ wire YDIFF_C3 = add_c(V3n, !oam_reg.REG_OAM_B3, YDIFF_C2);
  /*p29.GOPU*/ wire YDIFF4   = add_s(V4n, !oam_reg.REG_OAM_B4, YDIFF_C3);
  /*p29.GOPU*/ wire YDIFF_C4 = add_c(V4n, !oam_reg.REG_OAM_B4, YDIFF_C3);
  /*p29.FUWA*/ wire YDIFF5   = add_s(V5n, !oam_reg.REG_OAM_B5, YDIFF_C4);
  /*p29.FUWA*/ wire YDIFF_C5 = add_c(V5n, !oam_reg.REG_OAM_B5, YDIFF_C4);
  /*p29.GOJU*/ wire YDIFF6   = add_s(V6n, !oam_reg.REG_OAM_B6, YDIFF_C5);
  /*p29.GOJU*/ wire YDIFF_C6 = add_c(V6n, !oam_reg.REG_OAM_B6, YDIFF_C5);
  /*p29.WUHU*/ wire YDIFF7   = add_s(V7n, !oam_reg.REG_OAM_B7, YDIFF_C6);
  /*p29.WUHU*/ wire YDIFF_C7 = add_c(V7n, !oam_reg.REG_OAM_B7, YDIFF_C6);

  /*p29.DEGE*/ wire SPRITE_DELTA0 = not(YDIFF0);
  /*p29.DABY*/ wire SPRITE_DELTA1 = not(YDIFF1);
  /*p29.DABU*/ wire SPRITE_DELTA2 = not(YDIFF2);
  /*p29.GYSA*/ wire SPRITE_DELTA3 = not(YDIFF3);
  /*p29.GACE*/ wire SPRITE_DELTA4 = not(YDIFF4);
  /*p29.GUVU*/ wire SPRITE_DELTA5 = not(YDIFF5);
  /*p29.GYDA*/ wire SPRITE_DELTA6 = not(YDIFF6);
  /*p29.GEWY*/ wire SPRITE_DELTA7 = not(YDIFF7);

  /*p29.GOVU*/ wire GOVU = or(YDIFF3, vid_reg2.LCDC_SPSIZE);
  /*p29.WOTA*/ wire SPR_MATCH_Yn = nand(SPRITE_DELTA4, SPRITE_DELTA5, SPRITE_DELTA6, SPRITE_DELTA7, YDIFF_C7, GOVU);
  /*p29.GESE*/ wire SPR_MATCH_Y = not(SPR_MATCH_Yn);

  return {
    /*p29.GESE*/ SPR_MATCH_Y,
    /*p29.DEGE*/ SPRITE_DELTA0,
    /*p29.DABY*/ SPRITE_DELTA1,
    /*p29.DABU*/ SPRITE_DELTA2,
    /*p29.GYSA*/ SPRITE_DELTA3,
    /*p29.GACE*/ SPRITE_DELTA4,
    /*p29.GUVU*/ SPRITE_DELTA5,
    /*p29.GYDA*/ SPRITE_DELTA6,
    /*p29.GEWY*/ SPRITE_DELTA7,
  };
}

//-----------------------------------------------------------------------------

wire SpriteRegisters::scanner(const ClkSignals& clk_sig,
                              const VclkSignals& vid_clk,
                              const RstSignals& rst_sig,
                              const LcdSignals& lcd_sig) {

  // something wrong here
  // ANOM = nor(ATEJ, ATAR);

  /*p01.ATAR*/ wire VID_RESET6 = not(rst_sig.VID_RESETn);
  /*p28.ANOM*/ wire SCAN_RSTn = nor(lcd_sig.ATEJ_VID_LINE_TRIG_d4, VID_RESET6);
  /*p29.BALU*/ wire SCAN_RSTa = not(SCAN_RSTn);
  /*p29.BAGY*/ wire SCAN_RSTo = not(SCAN_RSTa);

  /*p28.FETO*/ wire SCAN_DONE_d0 = and(SCAN0, SCAN1, SCAN2, SCAN5); // 32 + 4 + 2 + 1 = 39
  /*p28.GAVA*/ wire SCAN_CLK = or(SCAN_DONE_d0, vid_clk.XUPY_xBCxxFGx);
  /*p28.YFEL*/ SCAN0.set(SCAN_CLK, SCAN_RSTn, !SCAN0);
  /*p28.WEWY*/ SCAN1.set(!SCAN0,   SCAN_RSTn, !SCAN1);
  /*p28.GOSO*/ SCAN2.set(!SCAN1,   SCAN_RSTn, !SCAN2);
  /*p28.ELYN*/ SCAN3.set(!SCAN2,   SCAN_RSTn, !SCAN3);
  /*p28.FAHA*/ SCAN4.set(!SCAN3,   SCAN_RSTn, !SCAN4);
  /*p28.FONY*/ SCAN5.set(!SCAN4,   SCAN_RSTn, !SCAN5);

  // the second clock here doesn't match the usual synchronizer pattern, but it matches the die.
  /*p29.BYBA*/ SCAN_DONE_d4.set(vid_clk.XUPY_xBCxxFGx, SCAN_RSTo, SCAN_DONE_d0);
  /*p29.DOBA*/ SCAN_DONE_d5.set(clk_sig.ALET_AxCxExGx, SCAN_RSTo, SCAN_DONE_d4);

  // which means this trigger is exactly 1 phase long
  /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or(SCAN_RSTa, SCAN_DONE_d5, !SCAN_DONE_d4);
  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIG = not(SCAN_DONE_d0_TRIGn);
  return AVAP_SCAN_DONE_d0_TRIG;
}

//-----------------------------------------------------------------------------

};