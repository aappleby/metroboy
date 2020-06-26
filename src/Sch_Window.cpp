#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

// Die trace:

// REJO00 << SARY
// REJO01 nc
// REJO02 >> nc
// REJO03 >> PUKY
// REJO04 nc
// REJO05 << REPU

// SARY high -> REJO high
// REPU high -> REJO low

// PARU02 = not(POPU16)
// PYRY02 = not(XAPO03)

// REPU04 = nor(PARU02, PYRY02)

//------------------------------------------------------------------------------

WindowSignals WindowRegisters::sig(const TestGB& gb) const {
  auto rst_sig = gb.rst_reg.sig(gb);
  auto& cfg_reg = gb.cfg_reg;
  auto& win_reg = gb.win_reg;

  auto lcd_sig = gb.lcd_reg.sig(gb);

  /*p27.NOCU*/ wire NOCU_WIN_MODEn = not(PYNU_WIN_MODE_LATCH.q());
  /*p27.PORE*/ wire PORE_WIN_MODE = not(NOCU_WIN_MODEn);
  /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and (PYNU_WIN_MODE_LATCH, !win_reg.NOPA_WIN_MODE_SYNC);
  /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WIN_MODE_TRIGp);
  /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not(NYFO_WIN_MODE_TRIGn);

  /*p27.SYLO*/ wire SYLO_WIN_HITn = not(RYDY_WIN_HIT_LATCH.q());
  /*p24.TOMU*/ wire TOMU_WIN_HITp = not(SYLO_WIN_HITn);
  /*p24.SOCY*/ wire SOCY_WIN_HITn = not(TOMU_WIN_HITp);
  /*p27.TUKU*/ wire TUKU_WIN_HITn = not(TOMU_WIN_HITp);
  /*p27.XOFO*/ wire XOFO_WIN_RSTp = nand(cfg_reg.LCDC_WINEN.q(), lcd_sig.XAHY_VID_LINE_TRIG_d4n, rst_sig.XAPO_VID_RSTn);

  /*p27.SEKO*/ wire SEKO_WIN_TRIGGER = nor(RENE_WIN_MATCH_ONSCREEN_SYNC2, !RYFA_WIN_MATCH_ONSCREEN_SYNC1);

  /*p27.TUXY*/ wire TUXY = nand(win_reg.SOVY_WIN_HIT_SYNC, SYLO_WIN_HITn);
  /*p27.SUZU*/ wire SUZU = not(TUXY);

  return {
    .NOCU_WIN_MODEn = NOCU_WIN_MODEn,
    .PORE_WIN_MODE = PORE_WIN_MODE,
    .NUNY_WIN_MODE_TRIGp = NUNY_WIN_MODE_TRIGp,
    .NYFO_WIN_MODE_TRIGn = NYFO_WIN_MODE_TRIGn,
    .MOSU_WIN_MODE_TRIGp = MOSU_WIN_MODE_TRIGp,

    .SYLO_WIN_HITn = SYLO_WIN_HITn,
    .TOMU_WIN_HITp = TOMU_WIN_HITp,
    .SOCY_WIN_HITn = SOCY_WIN_HITn,
    .TUKU_WIN_HITn = TUKU_WIN_HITn,
    .XOFO_WIN_RSTp = XOFO_WIN_RSTp,

    .SEKO_WIN_TRIGGER = SEKO_WIN_TRIGGER,

    .SUZU = SUZU,
  };
}

//------------------------------------------------------------------------------

void WindowRegisters::tick(TestGB& gb) {
  auto rst_sig = gb.rst_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto sst_sig = gb.sst_reg.sig(gb);
  auto win_sig = sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  /*p21.PARU*/ wire PARU_IN_VBLANK = not(!gb.lcd_reg.POPU_IN_VBLANK_d4);
  /*p27.REPU*/ wire REPU_IN_FRAME_Y = nor(PARU_IN_VBLANK, rst_sig.PYRY_VID_RSTp);   // schematic wrong, this is NOR

  //----------------------------------------

  /*p27.NAZE*/ wire _WY_MATCH0 = xnor(gb.lcd_reg.MUWY_Y0, gb.cfg_reg.WY0);
  /*p27.PEBO*/ wire _WY_MATCH1 = xnor(gb.lcd_reg.MYRO_Y1, gb.cfg_reg.WY1);
  /*p27.POMO*/ wire _WY_MATCH2 = xnor(gb.lcd_reg.LEXA_Y2, gb.cfg_reg.WY2);
  /*p27.NEVU*/ wire _WY_MATCH3 = xnor(gb.lcd_reg.LYDO_Y3, gb.cfg_reg.WY3);
  /*p27.NOJO*/ wire _WY_MATCH4 = xnor(gb.lcd_reg.LOVU_Y4, gb.cfg_reg.WY4);
  /*p27.PAGA*/ wire _WY_MATCH5 = xnor(gb.lcd_reg.LEMA_Y5, gb.cfg_reg.WY5);
  /*p27.PEZO*/ wire _WY_MATCH6 = xnor(gb.lcd_reg.MATO_Y6, gb.cfg_reg.WY6);
  /*p27.NUPA*/ wire _WY_MATCH7 = xnor(gb.lcd_reg.LAFO_Y7, gb.cfg_reg.WY7);

  /*p27.PALO*/ wire _WY_MATCH_HIn = nand(gb.cfg_reg.LCDC_WINEN, _WY_MATCH4, _WY_MATCH5, _WY_MATCH6, _WY_MATCH7);
  /*p27.NELE*/ wire _WY_MATCH_HI = not(_WY_MATCH_HIn);
  /*p27.PAFU*/ wire _WY_MATCHn = nand(_WY_MATCH_HI, _WY_MATCH0, _WY_MATCH1, _WY_MATCH2, _WY_MATCH3);
  /*p27.ROGE*/ wire _WY_MATCHp = not(_WY_MATCHn);

  /*p27.MYLO*/ wire _WX_MATCH0 = xnor(gb.ppu_reg.XEHO_X0, gb.cfg_reg.WX0);
  /*p27.PUWU*/ wire _WX_MATCH1 = xnor(gb.ppu_reg.SAVY_X1, gb.cfg_reg.WX1);
  /*p27.PUHO*/ wire _WX_MATCH2 = xnor(gb.ppu_reg.XODU_X2, gb.cfg_reg.WX2);
  /*p27.NYTU*/ wire _WX_MATCH3 = xnor(gb.ppu_reg.XYDO_X3, gb.cfg_reg.WX3);
  /*p27.NEZO*/ wire _WX_MATCH4 = xnor(gb.ppu_reg.TUHU_X4, gb.cfg_reg.WX4);
  /*p27.NORY*/ wire _WX_MATCH5 = xnor(gb.ppu_reg.TUKY_X5, gb.cfg_reg.WX5);
  /*p27.NONO*/ wire _WX_MATCH6 = xnor(gb.ppu_reg.TAKO_X6, gb.cfg_reg.WX6);
  /*p27.PASE*/ wire _WX_MATCH7 = xnor(gb.ppu_reg.SYBE_X7, gb.cfg_reg.WX7);

  /*p27.PUKY*/ wire _WX_MATCH_HIn = nand(REJO_WY_MATCH_LATCH, _WX_MATCH4, _WX_MATCH5, _WX_MATCH6, _WX_MATCH7);
  /*p27.NUFA*/ wire _WX_MATCH_HI = not (_WX_MATCH_HIn);
  /*p27.NOGY*/ wire _WX_MATCHn = nand(_WX_MATCH_HI, _WX_MATCH0, _WX_MATCH1, _WX_MATCH2, _WX_MATCH3);
  /*p27.NUKO*/ wire _WX_MATCHp = not(_WX_MATCHn);

  /*p27.SARY*/ SARY_WIN_MATCH_Y_SYNC.set(clk_sig.TALU_xBCDExxx, rst_sig.XAPO_VID_RSTn, _WY_MATCHp);
  /*p27.REJO*/ REJO_WY_MATCH_LATCH.nor_latch(SARY_WIN_MATCH_Y_SYNC, REPU_IN_FRAME_Y);

  /*p27.PANY*/ wire _WIN_MATCH_ONSCREEN = nor(_WX_MATCHp, ppu_sig.ROZE_FINE_COUNT_STOPn);

  /*p27.RYFA*/ RYFA_WIN_MATCH_ONSCREEN_SYNC1.set(ppu_sig.SEGU_xBxDxFxH, gb.ppu_reg.XYMU_RENDERINGp, _WIN_MATCH_ONSCREEN);
  /*p27.RENE*/ RENE_WIN_MATCH_ONSCREEN_SYNC2.set(clk_sig.ALET_xBxDxFxH, gb.ppu_reg.XYMU_RENDERINGp, RYFA_WIN_MATCH_ONSCREEN_SYNC1);

  /*p27.PYCO*/ PYCO_WIN_MATCH_SYNC1.set(ppu_sig.ROCO_AxCxExGx, rst_sig.XAPO_VID_RSTn, _WX_MATCHp);
  /*p27.NUNU*/ NUNU_WIN_MATCH_SYNC2.set(clk_sig.MEHE_AxCxExGx, rst_sig.XAPO_VID_RSTn, PYCO_WIN_MATCH_SYNC1);

  // PYNU has "arms" on the ground side - nor latch
  // PYNU00 << NUNU16
  // PYNU01 nc
  // PYNU02 >> nc
  // PYNU03 >> NOCU00, NUNY00, NOPA06
  // PYNU04 nc
  // PYNU05 << XOFO03

  // NUNU high -> PYNU high
  // XOFO high -> PYNU low

  /*p27.PYNU*/ PYNU_WIN_MODE_LATCH.nor_latch(NUNU_WIN_MATCH_SYNC2, win_sig.XOFO_WIN_RSTp);
  /*p27.NOPA*/ NOPA_WIN_MODE_SYNC.set(clk_sig.ALET_xBxDxFxH, rst_sig.XAPO_VID_RSTn, PYNU_WIN_MODE_LATCH);

  // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BFETCH_DONE_SYNC_DELAY) is RESET.
  ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
  ///*p27.RYDY*/ RYDY = nor(PUKU, rst_reg.VID_RESET4, BFETCH_DONE_SYNC_DELAY);
    
  /*p27.RYDY*/ RYDY_WIN_HIT_LATCH.nor_latch(win_sig.NUNY_WIN_MODE_TRIGp, rst_sig.PYRY_VID_RSTp || gb.ppu_reg.PORY_BFETCH_DONE_SYNC_DELAY);

  /*p27.SOVY*/ SOVY_WIN_HIT_SYNC.set(clk_sig.ALET_xBxDxFxH, rst_sig.XAPO_VID_RSTn, RYDY_WIN_HIT_LATCH);

  // window x coordinate
  {
    // something weird here, this doesn't look like a clock

    // Die trace:
    // VETU = and(TEVO, PORE);
    // XACO = not(XOFO)
    // WYKA =

    // WYKA01 <> WYKA12
    // WYKA02 << VETU
    // WYKA03 <> WYKA09
    // WYKA04 nc
    // WYKA05 nc
    // WYKA06 << XACO
    // WYKA07 << WYKA16
    // WYKA08 nc
    // WYKA09 <> WYKA03
    // WYKA10 nc
    // WYKA11 nc
    // WYKA12 <> WYKA01
    // WYKA13 << XACO
    // WYKA14 nc
    // WYKA15 nc
    // WYKA16 >> WYKA07, WODY02
    // WYKA17 >> XEJA

    /*p27.VETU*/ wire _VETU_X_CLK = and (ppu_sig.TEVO_CLK_STOPn, win_sig.PORE_WIN_MODE);
    /*p27.XACO*/ wire _XACO_WIN_RSTn = not(win_sig.XOFO_WIN_RSTp);

    /*p27.WYKA*/ WIN_X3.set(_VETU_X_CLK, _XACO_WIN_RSTn, WIN_X3.qn());
    /*p27.WODY*/ WIN_X4.set( WIN_X3.qn(), _XACO_WIN_RSTn, !WIN_X4);
    /*p27.WOBO*/ WIN_X5.set(!WIN_X4,      _XACO_WIN_RSTn, !WIN_X5);
    /*p27.WYKO*/ WIN_X6.set(!WIN_X5,      _XACO_WIN_RSTn, !WIN_X6);
    /*p27.XOLO*/ WIN_X7.set(!WIN_X6,      _XACO_WIN_RSTn, !WIN_X7);
  }

  // window y coordinate
  // every time we leave win mode we increment win_y
  {
    /*p27.SYNY*/ wire _SYNY_IN_FRAME_Yn = not(REPU_IN_FRAME_Y);
    /*p27.WAZY*/ wire _WAZY_WIN_Y_CLK = not(win_sig.PORE_WIN_MODE);
    /*p27.VYNO*/ WIN_Y0.set(_WAZY_WIN_Y_CLK, _SYNY_IN_FRAME_Yn, !WIN_Y0);
    /*p27.VUJO*/ WIN_Y1.set(!WIN_Y0, _SYNY_IN_FRAME_Yn, !WIN_Y1);
    /*p27.VYMU*/ WIN_Y2.set(!WIN_Y1, _SYNY_IN_FRAME_Yn, !WIN_Y2);
    /*p27.TUFU*/ WIN_Y3.set(!WIN_Y2, _SYNY_IN_FRAME_Yn, !WIN_Y3);
    /*p27.TAXA*/ WIN_Y4.set(!WIN_Y3, _SYNY_IN_FRAME_Yn, !WIN_Y4);
    /*p27.TOZO*/ WIN_Y5.set(!WIN_Y4, _SYNY_IN_FRAME_Yn, !WIN_Y5);
    /*p27.TATE*/ WIN_Y6.set(!WIN_Y5, _SYNY_IN_FRAME_Yn, !WIN_Y6);
    /*p27.TEKE*/ WIN_Y7.set(!WIN_Y6, _SYNY_IN_FRAME_Yn, !WIN_Y7);
  }

}

//------------------------------------------------------------------------------

bool WindowRegisters::commit() {
  bool changed = false;
  /*p27.SARY*/ changed |= SARY_WIN_MATCH_Y_SYNC.commit_reg();
  /*p27.RYFA*/ changed |= RYFA_WIN_MATCH_ONSCREEN_SYNC1.commit_reg();
  /*p27.RENE*/ changed |= RENE_WIN_MATCH_ONSCREEN_SYNC2.commit_reg();
  /*p27.PYCO*/ changed |= PYCO_WIN_MATCH_SYNC1.commit_reg();
  /*p27.NUNU*/ changed |= NUNU_WIN_MATCH_SYNC2.commit_reg();
  /*p27.REJO*/ changed |= REJO_WY_MATCH_LATCH.commit_latch();
  /*p27.NOPA*/ changed |= NOPA_WIN_MODE_SYNC.commit_reg();
  /*p27.SOVY*/ changed |= SOVY_WIN_HIT_SYNC.commit_reg();
  /*p27.PYNU*/ changed |= PYNU_WIN_MODE_LATCH.commit_latch();
  /*p27.RYDY*/ changed |= RYDY_WIN_HIT_LATCH.commit_latch();

  /*p27.WYKA*/ changed |= WIN_X3.commit_reg();
  /*p27.WODY*/ changed |= WIN_X4.commit_reg();
  /*p27.WOBO*/ changed |= WIN_X5.commit_reg();
  /*p27.WYKO*/ changed |= WIN_X6.commit_reg();
  /*p27.XOLO*/ changed |= WIN_X7.commit_reg();
  /*p27.VYNO*/ changed |= WIN_Y0.commit_reg();
  /*p27.VUJO*/ changed |= WIN_Y1.commit_reg();
  /*p27.VYMU*/ changed |= WIN_Y2.commit_reg();
  /*p27.TUFU*/ changed |= WIN_Y3.commit_reg();
  /*p27.TAXA*/ changed |= WIN_Y4.commit_reg();
  /*p27.TOZO*/ changed |= WIN_Y5.commit_reg();
  /*p27.TATE*/ changed |= WIN_Y6.commit_reg();
  /*p27.TEKE*/ changed |= WIN_Y7.commit_reg();

  return changed;
}

//------------------------------------------------------------------------------

//LY_MATCH_SYNC      .dump(text_painter, "LY_MATCH_SYNC       ");
//LYC_MATCH_LATCHn    .dump(text_painter, "LYC_MATCH_LATCHn     ");
//NOPA_WIN_MODE_SYNC.dump(text_painter, "NOPA_WIN_MODE_SYNC       ");
//SOVY_WIN_HIT_SYNC.dump(text_painter, "SOVY_WIN_HIT_SYNC       ");
//PYNU_WIN_MODE_LATCH.dump(text_painter, "PYNU_WIN_MODE_LATCH      ");
//RYDY_WIN_HIT_LATCH.dump(text_painter, "RYDY_WIN_HIT_LATCH      ");
//SARY_WIN_MATCH_Y_SYNC.dump(text_painter, "SARY_WIN_MATCH_Y_SYNC            ");
//RYFA_WIN_MATCH_ONSCREEN_SYNC1.dump(text_painter, "RYFA_WIN_MATCH_ONSCREEN_SYNC1 ");
//RENE_WIN_MATCH_ONSCREEN_SYNC2.dump(text_painter, "RENE_WIN_MATCH_ONSCREEN_SYNC2 ");
//PYCO_WIN_MATCH_SYNC1.dump(text_painter, "PYCO_WIN_MATCH_SYNC1          ");
//NUNU_WIN_MATCH_SYNC2.dump(text_painter, "NUNU_WIN_MATCH_SYNC2          ");
//REJO_WY_MATCH_LATCH.dump(text_painter, "REJO_WY_MATCH_LATCH           ");
