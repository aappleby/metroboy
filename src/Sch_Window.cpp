#include "Sch_Window.h"
#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void WindowRegisters::tick(SchematicTop& top) {

  /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.SYS_PIN_CLK_xBxDxFxH);
  /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
  /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
  /*p27.MEHE*/ wire MEHE_AxCxExGx = not(ALET_xBxDxFxH);

  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(top.BYHA_VID_LINE_TRIG_d4n());
  /*p27.XAHY*/ wire XAHY_VID_LINE_TRIG_d4n = not(ATEJ_VID_LINE_TRIG_d4p);
  /*p27.XOFO*/ wire XOFO_WIN_RSTp = nand(top.WYMO_LCDC_WINEN, XAHY_VID_LINE_TRIG_d4n, top.XAPO_VID_RSTn());

  /*p01.PYRY*/ wire PYRY_VID_RSTp = not(top.XAPO_VID_RSTn());

  /*p21.TALU*/ wire TALU_xBCDExxx = not(top.VENA_xBCDExxx());
  /*p21.PARU*/ wire PARU_VBLANKp = not(!top.POPU_VBLANK_d4());
  /*p27.REPU*/ wire REPU_IN_FRAME_Y = nor(PARU_VBLANKp, PYRY_VID_RSTp);   // schematic wrong, this is NOR
  /*p27.SYNY*/ wire SYNY_IN_FRAME_Y = not(REPU_IN_FRAME_Y);

  {
    /*p27.SARY*/ _SARY_WY_MATCH.set(TALU_xBCDExxx, top.XAPO_VID_RSTn(), ROGE_WY_MATCHp(top));
    /*p27.REJO*/ _REJO_WY_MATCH_LATCH.nor_latch(_SARY_WY_MATCH, REPU_IN_FRAME_Y);
  }

  {
    // This trigger fires on the pixel _at_ WX
    /*p27.ROCO*/ wire _ROCO_CLKPIPEp = not(top.SEGU_CLKPIPEn());
    /*p27.PYCO*/ _PYCO_WIN_MATCH_SYNC1.set(_ROCO_CLKPIPEp, top.XAPO_VID_RSTn(), NUKO_WX_MATCHp(top));
    /*p27.NUNU*/ _NUNU_WIN_MATCH_SYNC2.set(MEHE_AxCxExGx, top.XAPO_VID_RSTn(), _PYCO_WIN_MATCH_SYNC1);

    /*p27.PYNU*/ PYNU_WIN_MODE_TRIGA.nor_latch(_NUNU_WIN_MATCH_SYNC2, XOFO_WIN_RSTp);
    /*p27.NOPA*/ NOPA_WIN_MODE_TRIGB.set(ALET_xBxDxFxH, top.XAPO_VID_RSTn(), PYNU_WIN_MODE_TRIGA);
  }

  {
    // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BFETCH_DONE_SYNC_DELAY) is RESET.
    ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
    ///*p27.RYDY*/ RYDY = nor(PUKU, rst_reg.VID_RESET4, BFETCH_DONE_SYNC_DELAY);

    /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and (PYNU_WIN_MODE_TRIGA, !NOPA_WIN_MODE_TRIGB);
    /*p27.RYDY*/ RYDY_WIN_HIT_LATCHp.nor_latch(NUNY_WIN_MODE_TRIGp, PYRY_VID_RSTp || top.PORY_FETCH_DONE_Bp());
    /*p27.SOVY*/ SOVY_WIN_HIT_SYNC.set(ALET_xBxDxFxH, top.XAPO_VID_RSTn(), RYDY_WIN_HIT_LATCHp);
  }

  {
    // This trigger fires on the pixel _after_ WX
    /*p27.PANY*/ wire PANY_WX_MATCHn = nor(NUKO_WX_MATCHp(top), top.ROZE_FINE_COUNT_7n());
    /*p27.RYFA*/ _RYFA_WX_PASS_TRIGA.set(top.SEGU_CLKPIPEn(), top.XYMU_RENDERINGp(), PANY_WX_MATCHn);
    /*p27.RENE*/ _RENE_WX_PASS_TRIGB.set(ALET_xBxDxFxH,       top.XYMU_RENDERINGp(), _RYFA_WX_PASS_TRIGA);
    /*p27.SEKO*/ SEKO_WIN_PASS_TRIGp = nor(!_RYFA_WX_PASS_TRIGA, _RENE_WX_PASS_TRIGB);
  }

  // window x coordinate
  {
    // something weird here, PORE doesn't look like a clock

    /*p27.NOCU*/ wire NOCU_WIN_MODEn = not(PYNU_WIN_MODE_TRIGA.q());
    /*p27.PORE*/ wire PORE_WIN_MODEp = not(NOCU_WIN_MODEn);
    /*p27.VETU*/ wire VETU_WIN_MAP_CLK = and (top.TEVO_FINE_RSTp(), PORE_WIN_MODEp);
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
    /*p27.NOCU*/ wire NOCU_WIN_MODEn = not(PYNU_WIN_MODE_TRIGA.q());
    /*p27.PORE*/ wire PORE_WIN_MODEp = not(NOCU_WIN_MODEn);
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
    /*p22.XOLA*/ wire XOLA_A00n = not(top.CPU_PIN_A00);
    /*p22.XENO*/ wire XENO_A01n = not(top.CPU_PIN_A01);
    /*p22.XUSY*/ wire XUSY_A02n = not(top.CPU_PIN_A02);
    /*p22.XERA*/ wire XERA_A03n = not(top.CPU_PIN_A03);

    /*p22.WESA*/ wire WESA_A01p = not(XENO_A01n);
    /*p22.WEPO*/ wire WEPO_A03p = not(XERA_A03n);

    /*p22.WYVO*/ wire FF4An = nand(top.WERO_FF40_FF4Fp(), XOLA_A00n, WESA_A01p, XUSY_A02n, WEPO_A03p);
    /*p22.VYGA*/ wire FF4A = not(FF4An);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.WAXU*/ wire FF4A_RD = and (ASOT_CPU_RD, FF4A);
    /*p23.VOMY*/ wire FF4A_RDn = not(FF4A_RD);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.WEKO*/ wire FF4A_WR = and (CUPA_CPU_WR_xxxxxFGH, FF4A);
    /*p23.VEFU*/ wire FF4A_WRn = not(FF4A_WR);

    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.WALU*/ wire WALU_RSTn = not(XORE_RSTp);
    /*p23.NESO*/ WY0.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D0);
    /*p23.NYRO*/ WY1.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D1);
    /*p23.NAGA*/ WY2.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D2);
    /*p23.MELA*/ WY3.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D3);
    /*p23.NULO*/ WY4.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D4);
    /*p23.NENE*/ WY5.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D5);
    /*p23.NUKA*/ WY6.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D6);
    /*p23.NAFU*/ WY7.set(FF4A_WRn, WALU_RSTn, top.CPU_TRI_D7);


    /*p23.PUNU*/ top.CPU_TRI_D0.set_tribuf(!FF4A_RDn, WY0.q());
    /*p23.PODA*/ top.CPU_TRI_D1.set_tribuf(!FF4A_RDn, WY1.q());
    /*p23.PYGU*/ top.CPU_TRI_D2.set_tribuf(!FF4A_RDn, WY2.q());
    /*p23.LOKA*/ top.CPU_TRI_D3.set_tribuf(!FF4A_RDn, WY3.q());
    /*p23.MEGA*/ top.CPU_TRI_D4.set_tribuf(!FF4A_RDn, WY4.q());
    /*p23.PELA*/ top.CPU_TRI_D5.set_tribuf(!FF4A_RDn, WY5.q());
    /*p23.POLO*/ top.CPU_TRI_D6.set_tribuf(!FF4A_RDn, WY6.q());
    /*p23.MERA*/ top.CPU_TRI_D7.set_tribuf(!FF4A_RDn, WY7.q());
  }

  // FF4B
  {
    /*p22.XOLA*/ wire XOLA_A00n = not(top.CPU_PIN_A00);
    /*p22.XENO*/ wire XENO_A01n = not(top.CPU_PIN_A01);
    /*p22.XUSY*/ wire XUSY_A02n = not(top.CPU_PIN_A02);
    /*p22.XERA*/ wire XERA_A03n = not(top.CPU_PIN_A03);

    /*p22.WADO*/ wire WADO_A00p = not(XOLA_A00n);
    /*p22.WESA*/ wire WESA_A01p = not(XENO_A01n);
    /*p22.WEPO*/ wire WEPO_A03p = not(XERA_A03n);

    /*p22.WAGE*/ wire FF4Bn = nand(top.WERO_FF40_FF4Fp(), WADO_A00p, WESA_A01p, XUSY_A02n, WEPO_A03p);
    /*p22.VUMY*/ wire FF4B = not(FF4Bn);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.WYZE*/ wire FF4B_RD = and (ASOT_CPU_RD, FF4B);
    /*p23.VYCU*/ wire FF4B_RDn = not(FF4B_RD);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.WUZA*/ wire FF4B_WR = and (CUPA_CPU_WR_xxxxxFGH, FF4B);
    /*p23.VOXU*/ wire FF4B_WRn = not(FF4B_WR);

    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.WALU*/ wire WALU_RSTn = not(XORE_RSTp);
    /*p23.MYPA*/ WX0.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D0);
    /*p23.NOFE*/ WX1.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D1);
    /*p23.NOKE*/ WX2.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D2);
    /*p23.MEBY*/ WX3.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D3);
    /*p23.MYPU*/ WX4.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D4);
    /*p23.MYCE*/ WX5.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D5);
    /*p23.MUVO*/ WX6.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D6);
    /*p23.NUKU*/ WX7.set(FF4B_WRn, WALU_RSTn, top.CPU_TRI_D7);


    /*p23.LOVA*/ top.CPU_TRI_D0.set_tribuf(!FF4B_RDn, WX0.q());
    /*p23.MUKA*/ top.CPU_TRI_D1.set_tribuf(!FF4B_RDn, WX1.q());
    /*p23.MOKO*/ top.CPU_TRI_D2.set_tribuf(!FF4B_RDn, WX2.q());
    /*p23.LOLE*/ top.CPU_TRI_D3.set_tribuf(!FF4B_RDn, WX3.q());
    /*p23.MELE*/ top.CPU_TRI_D4.set_tribuf(!FF4B_RDn, WX4.q());
    /*p23.MUFE*/ top.CPU_TRI_D5.set_tribuf(!FF4B_RDn, WX5.q());
    /*p23.MULY*/ top.CPU_TRI_D6.set_tribuf(!FF4B_RDn, WX6.q());
    /*p23.MARA*/ top.CPU_TRI_D7.set_tribuf(!FF4B_RDn, WX7.q());
  }
}

//------------------------------------------------------------------------------

wire WindowRegisters::ROGE_WY_MATCHp(const SchematicTop& top) const {
  /*p27.NAZE*/ wire _WY_MATCH0 = xnor(top.MUWY_Y0(), WY0);
  /*p27.PEBO*/ wire _WY_MATCH1 = xnor(top.MYRO_Y1(), WY1);
  /*p27.POMO*/ wire _WY_MATCH2 = xnor(top.LEXA_Y2(), WY2);
  /*p27.NEVU*/ wire _WY_MATCH3 = xnor(top.LYDO_Y3(), WY3);
  /*p27.NOJO*/ wire _WY_MATCH4 = xnor(top.LOVU_Y4(), WY4);
  /*p27.PAGA*/ wire _WY_MATCH5 = xnor(top.LEMA_Y5(), WY5);
  /*p27.PEZO*/ wire _WY_MATCH6 = xnor(top.MATO_Y6(), WY6);
  /*p27.NUPA*/ wire _WY_MATCH7 = xnor(top.LAFO_Y7(), WY7);

  /*p27.PALO*/ wire _WY_MATCH_HIn  = nand(top.WYMO_LCDC_WINEN, _WY_MATCH4, _WY_MATCH5, _WY_MATCH6, _WY_MATCH7);
  /*p27.NELE*/ wire _WY_MATCH_HI   = not(_WY_MATCH_HIn);
  /*p27.PAFU*/ wire _WY_MATCHn     = nand(_WY_MATCH_HI, _WY_MATCH0, _WY_MATCH1, _WY_MATCH2, _WY_MATCH3);
  /*p27.ROGE*/ wire ROGE_WY_MATCHp = not(_WY_MATCHn);
  return ROGE_WY_MATCHp;
}

wire WindowRegisters::NUKO_WX_MATCHp(const SchematicTop& top) const {
  /*p27.MYLO*/ wire _WX_MATCH0 = xnor(top.XEHO_X0(), WX0);
  /*p27.PUWU*/ wire _WX_MATCH1 = xnor(top.SAVY_X1(), WX1);
  /*p27.PUHO*/ wire _WX_MATCH2 = xnor(top.XODU_X2(), WX2);
  /*p27.NYTU*/ wire _WX_MATCH3 = xnor(top.XYDO_X3(), WX3);
  /*p27.NEZO*/ wire _WX_MATCH4 = xnor(top.TUHU_X4(), WX4);
  /*p27.NORY*/ wire _WX_MATCH5 = xnor(top.TUKY_X5(), WX5);
  /*p27.NONO*/ wire _WX_MATCH6 = xnor(top.TAKO_X6(), WX6);
  /*p27.PASE*/ wire _WX_MATCH7 = xnor(top.SYBE_X7(), WX7);

  /*p27.PUKY*/ wire _WX_MATCH_HIn  = nand(_REJO_WY_MATCH_LATCH, _WX_MATCH4, _WX_MATCH5, _WX_MATCH6, _WX_MATCH7);
  /*p27.NUFA*/ wire _WX_MATCH_HI   = not (_WX_MATCH_HIn);
  /*p27.NOGY*/ wire _WX_MATCHn     = nand(_WX_MATCH_HI, _WX_MATCH0, _WX_MATCH1, _WX_MATCH2, _WX_MATCH3);
  /*p27.NUKO*/ wire NUKO_WX_MATCHp = not(_WX_MATCHn);
  return NUKO_WX_MATCHp;
}

//------------------------------------------------------------------------------

SignalHash WindowRegisters::commit() {
  SignalHash hash;
  /*p27.SARY*/ hash << _SARY_WY_MATCH.commit_reg();
  /*p27.RYFA*/ hash << _RYFA_WX_PASS_TRIGA.commit_reg();
  /*p27.RENE*/ hash << _RENE_WX_PASS_TRIGB.commit_reg();
  /*p27.PYCO*/ hash << _PYCO_WIN_MATCH_SYNC1.commit_reg();
  /*p27.NUNU*/ hash << _NUNU_WIN_MATCH_SYNC2.commit_reg();
  /*p27.REJO*/ hash << _REJO_WY_MATCH_LATCH.commit_latch();
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

