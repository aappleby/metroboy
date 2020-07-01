#include "Sch_Window.h"
#include "TestGB.h"

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
  WindowSignals sig;

  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);

  /*p27.NOCU*/ sig.NOCU_WIN_MODEn = not(PYNU_WIN_MODE_LATCH.q());
  /*p27.PORE*/ sig.PORE_WIN_MODE = not(sig.NOCU_WIN_MODEn);
  /*p27.NUNY*/ sig.NUNY_WIN_MODE_TRIGp = and (PYNU_WIN_MODE_LATCH, !NOPA_WIN_MODE_SYNC);
  /*p27.NYFO*/ sig.NYFO_WIN_MODE_TRIGn = not(sig.NUNY_WIN_MODE_TRIGp);
  /*p27.MOSU*/ sig.MOSU_WIN_MODE_TRIGp = not(sig.NYFO_WIN_MODE_TRIGn);

  /*p27.SYLO*/ sig.SYLO_WIN_HITn = not(RYDY_WIN_HIT_LATCH.q());
  /*p24.TOMU*/ sig.TOMU_WIN_HITp = not(sig.SYLO_WIN_HITn);
  /*p24.SOCY*/ sig.SOCY_WIN_HITn = not(sig.TOMU_WIN_HITp);
  /*p27.TUKU*/ sig.TUKU_WIN_HITn = not(sig.TOMU_WIN_HITp);
  /*p27.XOFO*/ sig.XOFO_WIN_RSTp = nand(ppu_sig.WYMO_LCDC_WINEN, lcd_sig.XAHY_VID_LINE_TRIG_d4n, rst_sig.XAPO_VID_RSTn);

  /*p27.SEKO*/ sig.SEKO_WIN_TRIGGER = nor(RENE_WIN_MATCH_ONSCREEN_SYNC2, !RYFA_WIN_MATCH_ONSCREEN_SYNC1);

  /*p27.TUXY*/ wire TUXY = nand(SOVY_WIN_HIT_SYNC, sig.SYLO_WIN_HITn);
  /*p27.SUZU*/ sig.SUZU = not(TUXY);

  sig.WIN_X3 = WIN_X3;
  sig.WIN_X4 = WIN_X4;
  sig.WIN_X5 = WIN_X5;
  sig.WIN_X6 = WIN_X6;
  sig.WIN_X7 = WIN_X7;

  sig.WIN_Y0 = WIN_Y0;
  sig.WIN_Y1 = WIN_Y1;
  sig.WIN_Y2 = WIN_Y2;
  sig.WIN_Y3 = WIN_Y3;
  sig.WIN_Y4 = WIN_Y4;
  sig.WIN_Y5 = WIN_Y5;
  sig.WIN_Y6 = WIN_Y6;
  sig.WIN_Y7 = WIN_Y7;

  return sig;
}

//------------------------------------------------------------------------------

void WindowRegisters::tick(TestGB& gb) {
  auto& cpu_bus = gb.cpu_bus;

  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto win_sig = sig(gb);
  auto tile_fetcher_sig = gb.tile_fetcher.sig(gb);

  /*p27.REPU*/ wire REPU_IN_FRAME_Y = nor(lcd_sig.PARU_VBLANKp, rst_sig.PYRY_VID_RSTp);   // schematic wrong, this is NOR

  //----------------------------------------

  {
    /*p27.NAZE*/ wire _WY_MATCH0 = xnor(lcd_sig.MUWY_Y0, WY0);
    /*p27.PEBO*/ wire _WY_MATCH1 = xnor(lcd_sig.MYRO_Y1, WY1);
    /*p27.POMO*/ wire _WY_MATCH2 = xnor(lcd_sig.LEXA_Y2, WY2);
    /*p27.NEVU*/ wire _WY_MATCH3 = xnor(lcd_sig.LYDO_Y3, WY3);
    /*p27.NOJO*/ wire _WY_MATCH4 = xnor(lcd_sig.LOVU_Y4, WY4);
    /*p27.PAGA*/ wire _WY_MATCH5 = xnor(lcd_sig.LEMA_Y5, WY5);
    /*p27.PEZO*/ wire _WY_MATCH6 = xnor(lcd_sig.MATO_Y6, WY6);
    /*p27.NUPA*/ wire _WY_MATCH7 = xnor(lcd_sig.LAFO_Y7, WY7);

    /*p27.PALO*/ wire _WY_MATCH_HIn = nand(ppu_sig.WYMO_LCDC_WINEN, _WY_MATCH4, _WY_MATCH5, _WY_MATCH6, _WY_MATCH7);
    /*p27.NELE*/ wire _WY_MATCH_HI = not(_WY_MATCH_HIn);
    /*p27.PAFU*/ wire _WY_MATCHn = nand(_WY_MATCH_HI, _WY_MATCH0, _WY_MATCH1, _WY_MATCH2, _WY_MATCH3);
    /*p27.ROGE*/ wire _WY_MATCHp = not(_WY_MATCHn);

    /*p27.SARY*/ SARY_WIN_MATCH_Y_SYNC.set(clk_sig.TALU_xBCDExxx, rst_sig.XAPO_VID_RSTn, _WY_MATCHp);

    /*p27.MYLO*/ wire _WX_MATCH0 = xnor(ppu_sig.XEHO_X0, WX0);
    /*p27.PUWU*/ wire _WX_MATCH1 = xnor(ppu_sig.SAVY_X1, WX1);
    /*p27.PUHO*/ wire _WX_MATCH2 = xnor(ppu_sig.XODU_X2, WX2);
    /*p27.NYTU*/ wire _WX_MATCH3 = xnor(ppu_sig.XYDO_X3, WX3);
    /*p27.NEZO*/ wire _WX_MATCH4 = xnor(ppu_sig.TUHU_X4, WX4);
    /*p27.NORY*/ wire _WX_MATCH5 = xnor(ppu_sig.TUKY_X5, WX5);
    /*p27.NONO*/ wire _WX_MATCH6 = xnor(ppu_sig.TAKO_X6, WX6);
    /*p27.PASE*/ wire _WX_MATCH7 = xnor(ppu_sig.SYBE_X7, WX7);

    /*p27.PUKY*/ wire _WX_MATCH_HIn = nand(REJO_WY_MATCH_LATCH, _WX_MATCH4, _WX_MATCH5, _WX_MATCH6, _WX_MATCH7);
    /*p27.NUFA*/ wire _WX_MATCH_HI = not (_WX_MATCH_HIn);
    /*p27.NOGY*/ wire _WX_MATCHn = nand(_WX_MATCH_HI, _WX_MATCH0, _WX_MATCH1, _WX_MATCH2, _WX_MATCH3);
    /*p27.NUKO*/ wire _WX_MATCHp = not(_WX_MATCHn);

    /*p27.PYCO*/ PYCO_WIN_MATCH_SYNC1.set(ppu_sig.ROCO_AxCxExGx, rst_sig.XAPO_VID_RSTn, _WX_MATCHp);
    /*p27.NUNU*/ NUNU_WIN_MATCH_SYNC2.set(clk_sig.MEHE_AxCxExGx, rst_sig.XAPO_VID_RSTn, PYCO_WIN_MATCH_SYNC1);

    /*p27.PANY*/ wire _WIN_MATCH_ONSCREEN = nor(_WX_MATCHp, ppu_sig.ROZE_FINE_COUNT_STOPn);
    /*p27.RYFA*/ RYFA_WIN_MATCH_ONSCREEN_SYNC1.set(ppu_sig.SEGU_xBxDxFxH, ppu_sig.XYMU_RENDERINGp, _WIN_MATCH_ONSCREEN);
    /*p27.RENE*/ RENE_WIN_MATCH_ONSCREEN_SYNC2.set(clk_sig.ALET_xBxDxFxH, ppu_sig.XYMU_RENDERINGp, RYFA_WIN_MATCH_ONSCREEN_SYNC1);
  }

  /*p27.REJO*/ REJO_WY_MATCH_LATCH.nor_latch(SARY_WIN_MATCH_Y_SYNC, REPU_IN_FRAME_Y);

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
    
  /*p27.RYDY*/ RYDY_WIN_HIT_LATCH.nor_latch(win_sig.NUNY_WIN_MODE_TRIGp, rst_sig.PYRY_VID_RSTp || tile_fetcher_sig.PORY_BFETCH_DONE_SYNC_DELAY);

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

    /*p27.WYKA*/ WIN_X3.set(_VETU_X_CLK, _XACO_WIN_RSTn,  WIN_X3.qn());
    /*p27.WODY*/ WIN_X4.set( WIN_X3.qn(), _XACO_WIN_RSTn, WIN_X4.qn());
    /*p27.WOBO*/ WIN_X5.set(!WIN_X4,      _XACO_WIN_RSTn, WIN_X5.qn());
    /*p27.WYKO*/ WIN_X6.set(!WIN_X5,      _XACO_WIN_RSTn, WIN_X6.qn());
    /*p27.XOLO*/ WIN_X7.set(!WIN_X6,      _XACO_WIN_RSTn, WIN_X7.qn());
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

  // FF4A
  {
    /*p22.WYVO*/ wire FF4An = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.XOLA_A00n, cpu_sig.WESA_A01p, cpu_sig.XUSY_A02n, cpu_sig.WEPO_A03p);
    /*p22.VYGA*/ wire FF4A = not(FF4An);

    /*p23.WAXU*/ wire FF4A_RD = and (cpu_sig.ASOT_CPU_RD, FF4A);
    /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);

    /*p23.WEKO*/ wire FF4A_WR = and (cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF4A);
    /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);

    /*p23.NESO*/ WY0.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D0);
    /*p23.NYRO*/ WY1.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D1);
    /*p23.NAGA*/ WY2.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D2);
    /*p23.MELA*/ WY3.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D3);
    /*p23.NULO*/ WY4.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D4);
    /*p23.NENE*/ WY5.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D5);
    /*p23.NUKA*/ WY6.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D6);
    /*p23.NAFU*/ WY7.set(FF4A_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D7);


    /*p23.PUNU*/ cpu_bus.TRI_D0.set_tribuf(!FF4A_RDn, WY0.q());
    /*p23.PODA*/ cpu_bus.TRI_D1.set_tribuf(!FF4A_RDn, WY1.q());
    /*p23.PYGU*/ cpu_bus.TRI_D2.set_tribuf(!FF4A_RDn, WY2.q());
    /*p23.LOKA*/ cpu_bus.TRI_D3.set_tribuf(!FF4A_RDn, WY3.q());
    /*p23.MEGA*/ cpu_bus.TRI_D4.set_tribuf(!FF4A_RDn, WY4.q());
    /*p23.PELA*/ cpu_bus.TRI_D5.set_tribuf(!FF4A_RDn, WY5.q());
    /*p23.POLO*/ cpu_bus.TRI_D6.set_tribuf(!FF4A_RDn, WY6.q());
    /*p23.MERA*/ cpu_bus.TRI_D7.set_tribuf(!FF4A_RDn, WY7.q());
  }

  // FF4B
  {
    /*p22.WAGE*/ wire FF4Bn = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.WADO_A00p, cpu_sig.WESA_A01p, cpu_sig.XUSY_A02n, cpu_sig.WEPO_A03p);
    /*p22.VUMY*/ wire FF4B = not(FF4Bn);

    /*p23.WYZE*/ wire FF4B_RD = and (cpu_sig.ASOT_CPU_RD, FF4B);
    /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);

    /*p23.WUZA*/ wire FF4B_WR = and (cpu_sig.CUPA_CPU_WR_xxxxxFGH, FF4B);
    /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

    /*p23.MYPA*/ WX0.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D0);
    /*p23.NOFE*/ WX1.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D1);
    /*p23.NOKE*/ WX2.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D2);
    /*p23.MEBY*/ WX3.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D3);
    /*p23.MYPU*/ WX4.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D4);
    /*p23.MYCE*/ WX5.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D5);
    /*p23.MUVO*/ WX6.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D6);
    /*p23.NUKU*/ WX7.set(FF4B_WRn, rst_sig.WALU_RSTn, cpu_bus.TRI_D7);


    /*p23.LOVA*/ cpu_bus.TRI_D0.set_tribuf(!FF4B_RDn, WX0.q());
    /*p23.MUKA*/ cpu_bus.TRI_D1.set_tribuf(!FF4B_RDn, WX1.q());
    /*p23.MOKO*/ cpu_bus.TRI_D2.set_tribuf(!FF4B_RDn, WX2.q());
    /*p23.LOLE*/ cpu_bus.TRI_D3.set_tribuf(!FF4B_RDn, WX3.q());
    /*p23.MELE*/ cpu_bus.TRI_D4.set_tribuf(!FF4B_RDn, WX4.q());
    /*p23.MUFE*/ cpu_bus.TRI_D5.set_tribuf(!FF4B_RDn, WX5.q());
    /*p23.MULY*/ cpu_bus.TRI_D6.set_tribuf(!FF4B_RDn, WX6.q());
    /*p23.MARA*/ cpu_bus.TRI_D7.set_tribuf(!FF4B_RDn, WX7.q());
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

  /*p23.MYPA*/ changed |= WX0.commit_reg();
  /*p23.NOFE*/ changed |= WX1.commit_reg();
  /*p23.NOKE*/ changed |= WX2.commit_reg();
  /*p23.MEBY*/ changed |= WX3.commit_reg();
  /*p23.MYPU*/ changed |= WX4.commit_reg();
  /*p23.MYCE*/ changed |= WX5.commit_reg();
  /*p23.MUVO*/ changed |= WX6.commit_reg();
  /*p23.NUKU*/ changed |= WX7.commit_reg();

  /*p23.NESO*/ changed |= WY0.commit_reg();
  /*p23.NYRO*/ changed |= WY1.commit_reg();
  /*p23.NAGA*/ changed |= WY2.commit_reg();
  /*p23.MELA*/ changed |= WY3.commit_reg();
  /*p23.NULO*/ changed |= WY4.commit_reg();
  /*p23.NENE*/ changed |= WY5.commit_reg();
  /*p23.NUKA*/ changed |= WY6.commit_reg();
  /*p23.NAFU*/ changed |= WY7.commit_reg();



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
//dump(text_painter,      "WY          ", WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7);
//dump(text_painter,      "WX          ", WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7);
