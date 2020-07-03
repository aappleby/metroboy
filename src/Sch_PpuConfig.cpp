#include "Sch_PpuConfig.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void PpuConfig::tick(SchematicTop& top) {
  
  /*p22.XOLA*/ wire XOLA_A00n = not(top.CPU_PIN_A00);
  /*p22.XENO*/ wire XENO_A01n = not(top.CPU_PIN_A01);
  /*p22.XUSY*/ wire XUSY_A02n = not(top.CPU_PIN_A02);
  /*p22.XERA*/ wire XERA_A03n = not(top.CPU_PIN_A03);
  /*p22.WADO*/ wire WADO_A00p = not(XOLA_A00n);
  /*p22.WESA*/ wire WESA_A01p = not(XENO_A01n);

  // FF40 LCDC
  {
    /*p22.WORU*/ wire WORU_FF40n = nand(top.WERO_FF40_FF4Fp(), XOLA_A00n, XENO_A01n, XUSY_A02n, XERA_A03n);
    /*p22.VOCA*/ wire VOCA_FF40p = not(WORU_FF40n);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.VYRE*/ wire VYRE_FF40_RDp = and (VOCA_FF40p, ASOT_CPU_RD);
    /*p23.WYCE*/ wire WYCE_FF40_RDn = not(VYRE_FF40_RDp);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.WARU*/ wire WARU_FF40_WRp = and (VOCA_FF40p, CUPA_CPU_WR_xxxxxFGH);
    /*p23.XUBO*/ wire XUBO_FF40_WRn = not(WARU_FF40_WRp);

    /*p23.WYPO*/ top.CPU_TRI_D0.set_tribuf(WYCE_FF40_RDn, VYXE_LCDC_BGEN);
    /*p23.XERO*/ top.CPU_TRI_D1.set_tribuf(WYCE_FF40_RDn, XYLO_LCDC_SPEN);
    /*p23.WYJU*/ top.CPU_TRI_D2.set_tribuf(WYCE_FF40_RDn, XYMO_LCDC_SPSIZE);
    /*p23.WUKA*/ top.CPU_TRI_D3.set_tribuf(WYCE_FF40_RDn, XAFO_LCDC_BGMAP);
    /*p23.VOKE*/ top.CPU_TRI_D4.set_tribuf(WYCE_FF40_RDn, WEXU_LCDC_BGTILE);
    /*p23.VATO*/ top.CPU_TRI_D5.set_tribuf(WYCE_FF40_RDn, WYMO_LCDC_WINEN);
    /*p23.VAHA*/ top.CPU_TRI_D6.set_tribuf(WYCE_FF40_RDn, WOKY_LCDC_WINMAP);
    /*p23.XEBU*/ top.CPU_TRI_D7.set_tribuf(WYCE_FF40_RDn, XONA_LCDC_EN);

    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p01.XORE*/ wire XORE_RSTp = not(CUNU_RSTn);
    /*p01.XARE*/ wire XARE_RSTn = not(XORE_RSTp);
    /*p23.VYXE*/ VYXE_LCDC_BGEN   .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D0);
    /*p23.XYLO*/ XYLO_LCDC_SPEN   .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D1);
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D2);
    /*p23.XAFO*/ XAFO_LCDC_BGMAP  .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D3);
    /*p23.WEXU*/ WEXU_LCDC_BGTILE .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D4);
    /*p23.WYMO*/ WYMO_LCDC_WINEN  .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D5);
    /*p23.WOKY*/ WOKY_LCDC_WINMAP .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D6);
    /*p23.XONA*/ XONA_LCDC_EN     .set(XUBO_FF40_WRn, XARE_RSTn, top.CPU_TRI_D7);
  }

  // FF42 SCY
  {
    /*p22.WEBU*/ wire WEBU_FF42n = nand(top.WERO_FF40_FF4Fp(), XOLA_A00n, WESA_A01p, XUSY_A02n, XERA_A03n);
    /*p22.XARO*/ wire XARO_FF42p = not(WEBU_FF42n);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.ANYP*/ wire ANYP_FF42_RDp = and(XARO_FF42p, ASOT_CPU_RD);
    /*p23.BUWY*/ wire BUWY_FF42_RDn = not(ANYP_FF42_RDp);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.BEDY*/ wire BEDY_FF42_WRp = and(XARO_FF42p, CUPA_CPU_WR_xxxxxFGH);
    /*p23.CAVO*/ wire CAVO_FF42_WRn = not(BEDY_FF42_WRp);

    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p23.GAVE*/ GAVE_SCY0.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D0);
    /*p23.FYMO*/ FYMO_SCY1.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D1);
    /*p23.FEZU*/ FEZU_SCY2.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D2);
    /*p23.FUJO*/ FUJO_SCY3.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D3);
    /*p23.DEDE*/ DEDE_SCY4.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D4);
    /*p23.FOTY*/ FOTY_SCY5.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D5);
    /*p23.FOHA*/ FOHA_SCY6.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D6);
    /*p23.FUNY*/ FUNY_SCY7.set(CAVO_FF42_WRn, CUNU_RSTn, top.CPU_TRI_D7);

    /*p23.WARE*/ top.CPU_TRI_D0.set_tribuf(BUWY_FF42_RDn, GAVE_SCY0.q());
    /*p23.GOBA*/ top.CPU_TRI_D1.set_tribuf(BUWY_FF42_RDn, FYMO_SCY1.q());
    /*p23.GONU*/ top.CPU_TRI_D2.set_tribuf(BUWY_FF42_RDn, FEZU_SCY2.q());
    /*p23.GODO*/ top.CPU_TRI_D3.set_tribuf(BUWY_FF42_RDn, FUJO_SCY3.q());
    /*p23.CUSA*/ top.CPU_TRI_D4.set_tribuf(BUWY_FF42_RDn, DEDE_SCY4.q());
    /*p23.GYZO*/ top.CPU_TRI_D5.set_tribuf(BUWY_FF42_RDn, FOTY_SCY5.q());
    /*p23.GUNE*/ top.CPU_TRI_D6.set_tribuf(BUWY_FF42_RDn, FOHA_SCY6.q());
    /*p23.GYZA*/ top.CPU_TRI_D7.set_tribuf(BUWY_FF42_RDn, FUNY_SCY7.q());
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire WAVU_FF43n = nand(top.WERO_FF40_FF4Fp(), WADO_A00p, WESA_A01p, XUSY_A02n, XERA_A03n);
    /*p22.XAVY*/ wire XAVY_FF43p = not(WAVU_FF43n);

    /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
    /*p07.AJAS*/ wire AJAS_BUS_RD = not(TEDO_CPU_RD);
    /*p07.ASOT*/ wire ASOT_CPU_RD = not(AJAS_BUS_RD);
    /*p23.AVOG*/ wire AVOG_FF43_RDp = and (XAVY_FF43p, ASOT_CPU_RD);
    /*p23.BEBA*/ wire BEBA_FF43_RDn = not(AVOG_FF43_RDp);

    /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());
    /*p07.DYKY*/ wire DYKY_CPU_WR_ABCDExxx = not(TAPU_CPU_WR_xxxxxFGH);
    /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH = not(DYKY_CPU_WR_ABCDExxx);
    /*p23.ARUR*/ wire ARUR_FF43_WRp = and (XAVY_FF43p, CUPA_CPU_WR_xxxxxFGH);
    /*p23.AMUN*/ wire AMUN_FF43_WRn = not(ARUR_FF43_WRp);

    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
    /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
    /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
    /*p23.DATY*/ DATY_SCX0.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D0);
    /*p23.DUZU*/ DUZU_SCX1.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D1);
    /*p23.CYXU*/ CYXU_SCX2.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D2);
    /*p23.GUBO*/ GUBO_SCX3.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D3);
    /*p23.BEMY*/ BEMY_SCX4.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D4);
    /*p23.CUZY*/ CUZY_SCX5.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D5);
    /*p23.CABU*/ CABU_SCX6.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D6);
    /*p23.BAKE*/ BAKE_SCX7.set(AMUN_FF43_WRn, CUNU_RSTn, top.CPU_TRI_D7);

    /*p23.EDOS*/ top.CPU_TRI_D0.set_tribuf(!BEBA_FF43_RDn, DATY_SCX0.q());
    /*p23.EKOB*/ top.CPU_TRI_D1.set_tribuf(!BEBA_FF43_RDn, DUZU_SCX1.q());
    /*p23.CUGA*/ top.CPU_TRI_D2.set_tribuf(!BEBA_FF43_RDn, CYXU_SCX2.q());
    /*p23.WONY*/ top.CPU_TRI_D3.set_tribuf(!BEBA_FF43_RDn, GUBO_SCX3.q());
    /*p23.CEDU*/ top.CPU_TRI_D4.set_tribuf(!BEBA_FF43_RDn, BEMY_SCX4.q());
    /*p23.CATA*/ top.CPU_TRI_D5.set_tribuf(!BEBA_FF43_RDn, CUZY_SCX5.q());
    /*p23.DOXE*/ top.CPU_TRI_D6.set_tribuf(!BEBA_FF43_RDn, CABU_SCX6.q());
    /*p23.CASY*/ top.CPU_TRI_D7.set_tribuf(!BEBA_FF43_RDn, BAKE_SCX7.q());
  }
}

//------------------------------------------------------------------------------

SignalHash PpuConfig::commit() {
  SignalHash hash;

  hash << VYXE_LCDC_BGEN.commit_reg();
  hash << XYLO_LCDC_SPEN.commit_reg();
  hash << XYMO_LCDC_SPSIZE.commit_reg();
  hash << XAFO_LCDC_BGMAP.commit_reg();
  hash << WEXU_LCDC_BGTILE.commit_reg();
  hash << WYMO_LCDC_WINEN.commit_reg();
  hash << WOKY_LCDC_WINMAP.commit_reg();
  hash << XONA_LCDC_EN.commit_reg();

  hash << GAVE_SCY0.commit_reg();
  hash << FYMO_SCY1.commit_reg();
  hash << FEZU_SCY2.commit_reg();
  hash << FUJO_SCY3.commit_reg();
  hash << DEDE_SCY4.commit_reg();
  hash << FOTY_SCY5.commit_reg();
  hash << FOHA_SCY6.commit_reg();
  hash << FUNY_SCY7.commit_reg();

  hash << DATY_SCX0.commit_reg();
  hash << DUZU_SCX1.commit_reg();
  hash << CYXU_SCX2.commit_reg();
  hash << GUBO_SCX3.commit_reg();
  hash << BEMY_SCX4.commit_reg();
  hash << CUZY_SCX5.commit_reg();
  hash << CABU_SCX6.commit_reg();
  hash << BAKE_SCX7.commit_reg();


  return hash;
}


//------------------------------------------------------------------------------
