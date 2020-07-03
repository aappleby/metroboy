#include "Sch_Window.h"
#include "Sch_Top.h"

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

// PYNU has "arms" on the ground side - nor latch
// PYNU00 << NUNU16
// PYNU01 nc
// PYNU02 >> nc
// PYNU03 >> NOCU00, NUNY00, NOPA06
// PYNU04 nc
// PYNU05 << XOFO03

// NUNU high -> PYNU high
// XOFO high -> PYNU low

//------------------------------------------------------------------------------

WindowSignals WindowRegisters::sig(const SchematicTop& gb) const {
  WindowSignals sig;

  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);

  /*p27.NOCU*/ sig.NOCU_WIN_MODEn       = not(PYNU_WIN_MODE_TRIGA.q());
  /*p27.NUNY*/ sig.NUNY_WIN_MODE_TRIGp  = and (PYNU_WIN_MODE_TRIGA, !NOPA_WIN_MODE_TRIGB);
  /*p27.SEKO*/ sig.SEKO_WIN_MATCH_TRIGp = nor(!RYFA_WIN_MATCH_TRIGA, RENE_WIN_MATCH_TRIGB);

  /*p27.SYLO*/ sig.SYLO_WIN_HITn = not(RYDY_WIN_HIT_LATCHp);
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

void WindowRegisters::tick(SchematicTop& gb) {
  auto& cpu_bus = gb.cpu_bus;

  auto cpu_sig = gb.cpu_bus.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto& clk_reg = gb.clk_reg;
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto win_sig = gb.win_reg.sig(gb);
  auto tile_fetcher_sig = gb.tile_fetcher.sig(gb);

  auto& ppu_config = gb.ppu_config;

  /*p27.NOCU*/ wire NOCU_WIN_MODEn = not(PYNU_WIN_MODE_TRIGA.q());
  /*p27.PORE*/ wire PORE_WIN_MODEp = not(NOCU_WIN_MODEn);

  /*p01.PYRY*/ wire PYRY_VID_RSTp = not(rst_sig.XAPO_VID_RSTn);
  /*p27.REPU*/ wire REPU_IN_FRAME_Y = nor(lcd_sig.PARU_VBLANKp, PYRY_VID_RSTp);   // schematic wrong, this is NOR

  /*p27.XOFO*/ wire XOFO_WIN_RSTp = nand(ppu_config.WYMO_LCDC_WINEN, lcd_sig.XAHY_VID_LINE_TRIG_d4n, rst_sig.XAPO_VID_RSTn);
  

  //----------------------------------------

  /*p22.XOLA*/ wire XOLA_A00n = not(cpu_bus.CPU_PIN_A00);
  /*p22.XENO*/ wire XENO_A01n = not(cpu_bus.CPU_PIN_A01);
  /*p22.XUSY*/ wire XUSY_A02n = not(cpu_bus.CPU_PIN_A02);
  /*p22.XERA*/ wire XERA_A03n = not(cpu_bus.CPU_PIN_A03);

  /*p22.WADO*/ wire WADO_A00p = not(XOLA_A00n);
  /*p22.WESA*/ wire WESA_A01p = not(XENO_A01n);
  /*p22.WEPO*/ wire WEPO_A03p = not(XERA_A03n);

  {
    /*p27.NAZE*/ wire _WY_MATCH0 = xnor(lcd_sig.MUWY_Y0, WY0);
    /*p27.PEBO*/ wire _WY_MATCH1 = xnor(lcd_sig.MYRO_Y1, WY1);
    /*p27.POMO*/ wire _WY_MATCH2 = xnor(lcd_sig.LEXA_Y2, WY2);
    /*p27.NEVU*/ wire _WY_MATCH3 = xnor(lcd_sig.LYDO_Y3, WY3);
    /*p27.NOJO*/ wire _WY_MATCH4 = xnor(lcd_sig.LOVU_Y4, WY4);
    /*p27.PAGA*/ wire _WY_MATCH5 = xnor(lcd_sig.LEMA_Y5, WY5);
    /*p27.PEZO*/ wire _WY_MATCH6 = xnor(lcd_sig.MATO_Y6, WY6);
    /*p27.NUPA*/ wire _WY_MATCH7 = xnor(lcd_sig.LAFO_Y7, WY7);

    /*p27.PALO*/ wire _WY_MATCH_HIn = nand(ppu_config.WYMO_LCDC_WINEN, _WY_MATCH4, _WY_MATCH5, _WY_MATCH6, _WY_MATCH7);
    /*p27.NELE*/ wire _WY_MATCH_HI  = not(_WY_MATCH_HIn);
    /*p27.PAFU*/ wire _WY_MATCHn    = nand(_WY_MATCH_HI, _WY_MATCH0, _WY_MATCH1, _WY_MATCH2, _WY_MATCH3);
    /*p27.ROGE*/ wire _WY_MATCHp    = not(_WY_MATCHn);

    /*p21.TALU*/ wire TALU_xBCDExxx = not(clk_reg.VENA_xBCDExxx);
    /*p27.SARY*/ SARY_WIN_MATCH_Y_SYNC.set(TALU_xBCDExxx, rst_sig.XAPO_VID_RSTn, _WY_MATCHp);

    /*p27.MYLO*/ wire _WX_MATCH0 = xnor(ppu_sig.XEHO_X0, WX0);
    /*p27.PUWU*/ wire _WX_MATCH1 = xnor(ppu_sig.SAVY_X1, WX1);
    /*p27.PUHO*/ wire _WX_MATCH2 = xnor(ppu_sig.XODU_X2, WX2);
    /*p27.NYTU*/ wire _WX_MATCH3 = xnor(ppu_sig.XYDO_X3, WX3);
    /*p27.NEZO*/ wire _WX_MATCH4 = xnor(ppu_sig.TUHU_X4, WX4);
    /*p27.NORY*/ wire _WX_MATCH5 = xnor(ppu_sig.TUKY_X5, WX5);
    /*p27.NONO*/ wire _WX_MATCH6 = xnor(ppu_sig.TAKO_X6, WX6);
    /*p27.PASE*/ wire _WX_MATCH7 = xnor(ppu_sig.SYBE_X7, WX7);

    /*p27.PUKY*/ wire _WX_MATCH_HIn = nand(REJO_WY_MATCH_LATCH, _WX_MATCH4, _WX_MATCH5, _WX_MATCH6, _WX_MATCH7);
    /*p27.NUFA*/ wire _WX_MATCH_HI  = not (_WX_MATCH_HIn);
    /*p27.NOGY*/ wire _WX_MATCHn    = nand(_WX_MATCH_HI, _WX_MATCH0, _WX_MATCH1, _WX_MATCH2, _WX_MATCH3);
    /*p27.NUKO*/ wire WX_MATCHp    = not(_WX_MATCHn);

    {
      /*p01.ANOS*/ wire ANOS_AxCxExGx = not(gb.PIN_CLK_IN_xBxDxFxH);
      /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
      /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
      /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
      /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
      /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
      /*p27.MEHE*/ wire MEHE_AxCxExGx = not(ALET_xBxDxFxH);
      /*p27.ROCO*/ wire ROCO_CLKPIPEp = not(ppu_sig.SEGU_CLKPIPEn);
      /*p27.PYCO*/ PYCO_WIN_MATCH_SYNC1.set(ROCO_CLKPIPEp,         rst_sig.XAPO_VID_RSTn, WX_MATCHp);
      /*p27.NUNU*/ NUNU_WIN_MATCH_SYNC2.set(MEHE_AxCxExGx, rst_sig.XAPO_VID_RSTn, PYCO_WIN_MATCH_SYNC1);

      /*p27.PYNU*/ PYNU_WIN_MODE_TRIGA.nor_latch(NUNU_WIN_MATCH_SYNC2, XOFO_WIN_RSTp);
      /*p27.NOPA*/ NOPA_WIN_MODE_TRIGB.set(ALET_xBxDxFxH, rst_sig.XAPO_VID_RSTn, PYNU_WIN_MODE_TRIGA);
      /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and (PYNU_WIN_MODE_TRIGA, !NOPA_WIN_MODE_TRIGB);

      // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BFETCH_DONE_SYNC_DELAY) is RESET.
      ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
      ///*p27.RYDY*/ RYDY = nor(PUKU, rst_reg.VID_RESET4, BFETCH_DONE_SYNC_DELAY);

      /*p27.RYDY*/ RYDY_WIN_HIT_LATCHp.nor_latch(NUNY_WIN_MODE_TRIGp, PYRY_VID_RSTp || tile_fetcher_sig.PORY_FETCH_DONE_Bp);
      /*p27.SOVY*/ SOVY_WIN_HIT_SYNC.set(ALET_xBxDxFxH, rst_sig.XAPO_VID_RSTn, RYDY_WIN_HIT_LATCHp);
    }

    {
      /*p01.ANOS*/ wire ANOS_AxCxExGx = not(gb.PIN_CLK_IN_xBxDxFxH);
      /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
      /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
      /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
      /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
      /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
      /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(WX_MATCHp, ppu_sig.ROZE_FINE_COUNT_STOPn);
      /*p27.RYFA*/ RYFA_WIN_MATCH_TRIGA.set(ppu_sig.SEGU_CLKPIPEn, ppu_sig.XYMU_RENDERINGp, WIN_MATCH_ONSCREEN);
      /*p27.RENE*/ RENE_WIN_MATCH_TRIGB.set(ALET_xBxDxFxH, ppu_sig.XYMU_RENDERINGp, RYFA_WIN_MATCH_TRIGA);
      /*p27.SEKO*/ wire SEKO_WIN_MATCH_TRIGp = nor(!RYFA_WIN_MATCH_TRIGA, RENE_WIN_MATCH_TRIGB);

#if 0

      /*p27.TEVO*/ wire TEVO_FINE_RSTp = nor(SEKO_WIN_MATCH_TRIGp,
                                             and(SOVY_WIN_HIT_SYNC, !RYDY_WIN_HIT_LATCHp),
                                             tile_fetcher_sig.TAVE_PORCH_DONE_TRIGp);

#endif
      /*p27.SYLO*/ wire SYLO_WIN_HITn = not(RYDY_WIN_HIT_LATCHp.q());
      /*p27.TUXY*/ wire TUXY = nand(SOVY_WIN_HIT_SYNC, SYLO_WIN_HITn);
      /*p27.SUZU*/ wire SUZU = not(TUXY);
      /*p27.TEVO*/ wire TEVO_FINE_RSTp = nor(SEKO_WIN_MATCH_TRIGp, SUZU, tile_fetcher_sig.TAVE_PORCH_DONE_TRIGp);

      (void)TEVO_FINE_RSTp;
    }
  }

  /*p27.REJO*/ REJO_WY_MATCH_LATCH.nor_latch(SARY_WIN_MATCH_Y_SYNC, REPU_IN_FRAME_Y);



  // window x coordinate
  {
    // something weird here, PORE doesn't look like a clock

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

    /*p27.VETU*/ wire VETU_WIN_MAP_CLK = and (ppu_sig.TEVO_FINE_RSTp, PORE_WIN_MODEp);
    /*p27.XACO*/ wire XACO_WIN_RSTn = not(XOFO_WIN_RSTp);

    /*p27.WYKA*/ WIN_X3.set(VETU_WIN_MAP_CLK, XACO_WIN_RSTn, !WIN_X3);
    /*p27.WODY*/ WIN_X4.set(!WIN_X3,          XACO_WIN_RSTn, !WIN_X4);
    /*p27.WOBO*/ WIN_X5.set(!WIN_X4,          XACO_WIN_RSTn, !WIN_X5);
    /*p27.WYKO*/ WIN_X6.set(!WIN_X5,          XACO_WIN_RSTn, !WIN_X6);
    /*p27.XOLO*/ WIN_X7.set(!WIN_X6,          XACO_WIN_RSTn, !WIN_X7);
  }

  // window y coordinate
  // every time we leave win mode we increment win_y
  {
    /*p27.SYNY*/ wire SYNY_IN_FRAME_Y = not(REPU_IN_FRAME_Y);
    /*p27.WAZY*/ wire WAZY_WIN_Y_CLK = not(PORE_WIN_MODEp);
    /*p27.VYNO*/ WIN_Y0.set(WAZY_WIN_Y_CLK, SYNY_IN_FRAME_Y, !WIN_Y0);
    /*p27.VUJO*/ WIN_Y1.set(!WIN_Y0,        SYNY_IN_FRAME_Y, !WIN_Y1);
    /*p27.VYMU*/ WIN_Y2.set(!WIN_Y1,        SYNY_IN_FRAME_Y, !WIN_Y2);
    /*p27.TUFU*/ WIN_Y3.set(!WIN_Y2,        SYNY_IN_FRAME_Y, !WIN_Y3);
    /*p27.TAXA*/ WIN_Y4.set(!WIN_Y3,        SYNY_IN_FRAME_Y, !WIN_Y4);
    /*p27.TOZO*/ WIN_Y5.set(!WIN_Y4,        SYNY_IN_FRAME_Y, !WIN_Y5);
    /*p27.TATE*/ WIN_Y6.set(!WIN_Y5,        SYNY_IN_FRAME_Y, !WIN_Y6);
    /*p27.TEKE*/ WIN_Y7.set(!WIN_Y6,        SYNY_IN_FRAME_Y, !WIN_Y7);
  }

  // FF4A
  {
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
    /*p07.TONA*/ wire TONA_A08n = not(cpu_bus.CPU_PIN_A08);
    /*p22.XALY*/ wire XALY_0x00xxxxp = nor(cpu_bus.CPU_PIN_A07, cpu_bus.CPU_PIN_A05, cpu_bus.CPU_PIN_A04);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p22.WUTU*/ wire WUTU_FF40_FF4Fn = nand(SYKE_FF00_FFFFp, cpu_bus.CPU_PIN_A06, XALY_0x00xxxxp);
    /*p22.WERO*/ wire WERO_FF40_FF4Fp = not(WUTU_FF40_FF4Fn);
    /*p22.WYVO*/ wire FF4An = nand(WERO_FF40_FF4Fp, XOLA_A00n, WESA_A01p, XUSY_A02n, WEPO_A03p);
    /*p22.VYGA*/ wire FF4A = not(FF4An);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(cpu_sig.UJYV_CPU_RD);
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.WAXU*/ wire FF4A_RD = and (ASOT_CPU_RD, FF4A);
    /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(cpu_sig.UBAL_CPU_WRp_ABCDExxx);
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.WEKO*/ wire FF4A_WR = and (CUPA_CPU_WR_xxxxxFGH, FF4A);
    /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);

    /*p01.ALUR*/ wire ALUR_RSTn = not(rst_sig.AVOR_RSTp);   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.WALU*/ wire WALU_RSTn = not(XORE_RSTp);
    /*p23.NESO*/ WY0.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D0);
    /*p23.NYRO*/ WY1.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D1);
    /*p23.NAGA*/ WY2.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D2);
    /*p23.MELA*/ WY3.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D3);
    /*p23.NULO*/ WY4.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D4);
    /*p23.NENE*/ WY5.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D5);
    /*p23.NUKA*/ WY6.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D6);
    /*p23.NAFU*/ WY7.set(FF4A_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D7);


    /*p23.PUNU*/ cpu_bus.CPU_TRI_D0.set_tribuf(!FF4A_RDn, WY0.q());
    /*p23.PODA*/ cpu_bus.CPU_TRI_D1.set_tribuf(!FF4A_RDn, WY1.q());
    /*p23.PYGU*/ cpu_bus.CPU_TRI_D2.set_tribuf(!FF4A_RDn, WY2.q());
    /*p23.LOKA*/ cpu_bus.CPU_TRI_D3.set_tribuf(!FF4A_RDn, WY3.q());
    /*p23.MEGA*/ cpu_bus.CPU_TRI_D4.set_tribuf(!FF4A_RDn, WY4.q());
    /*p23.PELA*/ cpu_bus.CPU_TRI_D5.set_tribuf(!FF4A_RDn, WY5.q());
    /*p23.POLO*/ cpu_bus.CPU_TRI_D6.set_tribuf(!FF4A_RDn, WY6.q());
    /*p23.MERA*/ cpu_bus.CPU_TRI_D7.set_tribuf(!FF4A_RDn, WY7.q());
  }

  // FF4B
  {
    /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(cpu_bus.CPU_PIN_A15, cpu_bus.CPU_PIN_A14, cpu_bus.CPU_PIN_A13, cpu_bus.CPU_PIN_A12, cpu_bus.CPU_PIN_A11, cpu_bus.CPU_PIN_A10, cpu_bus.CPU_PIN_A09);
    /*p07.TONA*/ wire TONA_A08n = not(cpu_bus.CPU_PIN_A08);
    /*p22.XALY*/ wire XALY_0x00xxxxp = nor(cpu_bus.CPU_PIN_A07, cpu_bus.CPU_PIN_A05, cpu_bus.CPU_PIN_A04);
    /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
    /*p22.WUTU*/ wire WUTU_FF40_FF4Fn = nand(SYKE_FF00_FFFFp, cpu_bus.CPU_PIN_A06, XALY_0x00xxxxp);
    /*p22.WERO*/ wire WERO_FF40_FF4Fp = not(WUTU_FF40_FF4Fn);
    /*p22.WAGE*/ wire FF4Bn = nand(WERO_FF40_FF4Fp, WADO_A00p, WESA_A01p, XUSY_A02n, WEPO_A03p);
    /*p22.VUMY*/ wire FF4B = not(FF4Bn);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(cpu_sig.UJYV_CPU_RD);
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.WYZE*/ wire FF4B_RD = and (ASOT_CPU_RD, FF4B);
    /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(cpu_sig.UBAL_CPU_WRp_ABCDExxx);
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.WUZA*/ wire FF4B_WR = and (CUPA_CPU_WR_xxxxxFGH, FF4B);
    /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

    /*p01.ALUR*/ wire ALUR_RSTn = not(rst_sig.AVOR_RSTp);   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.WALU*/ wire WALU_RSTn = not(XORE_RSTp);
    /*p23.MYPA*/ WX0.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D0);
    /*p23.NOFE*/ WX1.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D1);
    /*p23.NOKE*/ WX2.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D2);
    /*p23.MEBY*/ WX3.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D3);
    /*p23.MYPU*/ WX4.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D4);
    /*p23.MYCE*/ WX5.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D5);
    /*p23.MUVO*/ WX6.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D6);
    /*p23.NUKU*/ WX7.set(FF4B_WRn, WALU_RSTn, cpu_bus.CPU_TRI_D7);


    /*p23.LOVA*/ cpu_bus.CPU_TRI_D0.set_tribuf(!FF4B_RDn, WX0.q());
    /*p23.MUKA*/ cpu_bus.CPU_TRI_D1.set_tribuf(!FF4B_RDn, WX1.q());
    /*p23.MOKO*/ cpu_bus.CPU_TRI_D2.set_tribuf(!FF4B_RDn, WX2.q());
    /*p23.LOLE*/ cpu_bus.CPU_TRI_D3.set_tribuf(!FF4B_RDn, WX3.q());
    /*p23.MELE*/ cpu_bus.CPU_TRI_D4.set_tribuf(!FF4B_RDn, WX4.q());
    /*p23.MUFE*/ cpu_bus.CPU_TRI_D5.set_tribuf(!FF4B_RDn, WX5.q());
    /*p23.MULY*/ cpu_bus.CPU_TRI_D6.set_tribuf(!FF4B_RDn, WX6.q());
    /*p23.MARA*/ cpu_bus.CPU_TRI_D7.set_tribuf(!FF4B_RDn, WX7.q());
  }
}

//------------------------------------------------------------------------------

SignalHash WindowRegisters::commit() {
  SignalHash hash;
  /*p27.SARY*/ hash << SARY_WIN_MATCH_Y_SYNC.commit_reg();
  /*p27.RYFA*/ hash << RYFA_WIN_MATCH_TRIGA.commit_reg();
  /*p27.RENE*/ hash << RENE_WIN_MATCH_TRIGB.commit_reg();
  /*p27.PYCO*/ hash << PYCO_WIN_MATCH_SYNC1.commit_reg();
  /*p27.NUNU*/ hash << NUNU_WIN_MATCH_SYNC2.commit_reg();
  /*p27.REJO*/ hash << REJO_WY_MATCH_LATCH.commit_latch();
  /*p27.NOPA*/ hash << NOPA_WIN_MODE_TRIGB.commit_reg();
  /*p27.SOVY*/ hash << SOVY_WIN_HIT_SYNC.commit_reg();
  /*p27.PYNU*/ hash << PYNU_WIN_MODE_TRIGA.commit_latch();
  /*p27.RYDY*/ hash << RYDY_WIN_HIT_LATCHp.commit_latch();

  /*p27.WYKA*/ hash << WIN_X3.commit_reg();
  /*p27.WODY*/ hash << WIN_X4.commit_reg();
  /*p27.WOBO*/ hash << WIN_X5.commit_reg();
  /*p27.WYKO*/ hash << WIN_X6.commit_reg();
  /*p27.XOLO*/ hash << WIN_X7.commit_reg();
  /*p27.VYNO*/ hash << WIN_Y0.commit_reg();
  /*p27.VUJO*/ hash << WIN_Y1.commit_reg();
  /*p27.VYMU*/ hash << WIN_Y2.commit_reg();
  /*p27.TUFU*/ hash << WIN_Y3.commit_reg();
  /*p27.TAXA*/ hash << WIN_Y4.commit_reg();
  /*p27.TOZO*/ hash << WIN_Y5.commit_reg();
  /*p27.TATE*/ hash << WIN_Y6.commit_reg();
  /*p27.TEKE*/ hash << WIN_Y7.commit_reg();

  /*p23.MYPA*/ hash << WX0.commit_reg();
  /*p23.NOFE*/ hash << WX1.commit_reg();
  /*p23.NOKE*/ hash << WX2.commit_reg();
  /*p23.MEBY*/ hash << WX3.commit_reg();
  /*p23.MYPU*/ hash << WX4.commit_reg();
  /*p23.MYCE*/ hash << WX5.commit_reg();
  /*p23.MUVO*/ hash << WX6.commit_reg();
  /*p23.NUKU*/ hash << WX7.commit_reg();

  /*p23.NESO*/ hash << WY0.commit_reg();
  /*p23.NYRO*/ hash << WY1.commit_reg();
  /*p23.NAGA*/ hash << WY2.commit_reg();
  /*p23.MELA*/ hash << WY3.commit_reg();
  /*p23.NULO*/ hash << WY4.commit_reg();
  /*p23.NENE*/ hash << WY5.commit_reg();
  /*p23.NUKA*/ hash << WY6.commit_reg();
  /*p23.NAFU*/ hash << WY7.commit_reg();



  return hash;
}

//------------------------------------------------------------------------------

//LY_MATCH_SYNC      .dump(text_painter, "LY_MATCH_SYNC       ");
//LYC_MATCH_LATCHn    .dump(text_painter, "LYC_MATCH_LATCHn     ");
//NOPA_WIN_MODE_SYNC.dump(text_painter, "NOPA_WIN_MODE_SYNC       ");
//SOVY_WIN_HIT_SYNC.dump(text_painter, "SOVY_WIN_HIT_SYNC       ");
//PYNU_WIN_MODE_LATCH.dump(text_painter, "PYNU_WIN_MODE_LATCH      ");
//RYDY_WIN_HIT_LATCHp.dump(text_painter, "RYDY_WIN_HIT_LATCHp      ");
//SARY_WIN_MATCH_Y_SYNC.dump(text_painter, "SARY_WIN_MATCH_Y_SYNC            ");
//RYFA_WIN_MATCH_ONSCREEN_SYNC1.dump(text_painter, "RYFA_WIN_MATCH_ONSCREEN_SYNC1 ");
//RENE_WIN_MATCH_ONSCREEN_SYNC2.dump(text_painter, "RENE_WIN_MATCH_ONSCREEN_SYNC2 ");
//PYCO_WIN_MATCH_SYNC1.dump(text_painter, "PYCO_WIN_MATCH_SYNC1          ");
//NUNU_WIN_MATCH_SYNC2.dump(text_painter, "NUNU_WIN_MATCH_SYNC2          ");
//REJO_WY_MATCH_LATCH.dump(text_painter, "REJO_WY_MATCH_LATCH           ");
//dump(text_painter,      "WY          ", WY0,   WY1,   WY2,   WY3,   WY4,   WY5,   WY6,   WY7);
//dump(text_painter,      "WX          ", WX0,   WX1,   WX2,   WX3,   WX4,   WX5,   WX6,   WX7);
