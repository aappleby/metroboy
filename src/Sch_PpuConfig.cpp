#include "Sch_PpuConfig.h"

#include "TestGB.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void PpuConfig::tick(TestGB& gb) {
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_bus.sig(gb);

  auto& cpu_bus = gb.cpu_bus;

  // FF40 LCDC
  {
    /*p22.WORU*/ wire WORU_FF40n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.XOLA_A00n, cpu_sig.XENO_A01n, cpu_sig.XUSY_A02n, cpu_sig.XERA_A03n);
    /*p22.VOCA*/ wire VOCA_FF40p = not(WORU_FF40n);

    /*p23.VYRE*/ wire VYRE_FF40_RDp = and (VOCA_FF40p, cpu_sig.ASOT_CPU_RD);
    /*p23.WYCE*/ wire WYCE_FF40_RDn = not(VYRE_FF40_RDp);

    /*p23.WARU*/ wire WARU_FF40_WRp = and (VOCA_FF40p, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p23.XUBO*/ wire XUBO_FF40_WRn = not(WARU_FF40_WRp);

    /*p23.WYPO*/ cpu_bus.TRI_D0.set_tribuf(WYCE_FF40_RDn, VYXE_LCDC_BGEN);
    /*p23.XERO*/ cpu_bus.TRI_D1.set_tribuf(WYCE_FF40_RDn, XYLO_LCDC_SPEN);
    /*p23.WYJU*/ cpu_bus.TRI_D2.set_tribuf(WYCE_FF40_RDn, XYMO_LCDC_SPSIZE);
    /*p23.WUKA*/ cpu_bus.TRI_D3.set_tribuf(WYCE_FF40_RDn, XAFO_LCDC_BGMAP);
    /*p23.VOKE*/ cpu_bus.TRI_D4.set_tribuf(WYCE_FF40_RDn, WEXU_LCDC_BGTILE);
    /*p23.VATO*/ cpu_bus.TRI_D5.set_tribuf(WYCE_FF40_RDn, WYMO_LCDC_WINEN);
    /*p23.VAHA*/ cpu_bus.TRI_D6.set_tribuf(WYCE_FF40_RDn, WOKY_LCDC_WINMAP);
    /*p23.XEBU*/ cpu_bus.TRI_D7.set_tribuf(WYCE_FF40_RDn, XONA_LCDC_EN);

    /*p01.XARE*/ wire XARE_RESETn = not(rst_sig.XORE_RSTp);
    /*p23.VYXE*/ VYXE_LCDC_BGEN   .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D0);
    /*p23.XYLO*/ XYLO_LCDC_SPEN   .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D1);
    /*p23.XYMO*/ XYMO_LCDC_SPSIZE .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D2);
    /*p23.XAFO*/ XAFO_LCDC_BGMAP  .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D3);
    /*p23.WEXU*/ WEXU_LCDC_BGTILE .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D4);
    /*p23.WYMO*/ WYMO_LCDC_WINEN  .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D5);
    /*p23.WOKY*/ WOKY_LCDC_WINMAP .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D6);
    /*p23.XONA*/ XONA_LCDC_EN     .set(XUBO_FF40_WRn, XARE_RESETn, cpu_bus.TRI_D7);
  }

  // FF42 SCY
  {
    /*p22.WEBU*/ wire WEBU_FF42n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.XOLA_A00n, cpu_sig.WESA_A01p, cpu_sig.XUSY_A02n, cpu_sig.XERA_A03n);
    /*p22.XARO*/ wire XARO_FF42p = not(WEBU_FF42n);

    /*p23.ANYP*/ wire ANYP_FF42_RDp = and(XARO_FF42p, cpu_sig.ASOT_CPU_RD);
    /*p23.BUWY*/ wire BUWY_FF42_RDn = not(ANYP_FF42_RDp);

    /*p23.BEDY*/ wire BEDY_FF42_WRp = and(XARO_FF42p, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p23.CAVO*/ wire CAVO_FF42_WRn = not(BEDY_FF42_WRp);

    /*p23.GAVE*/ GAVE_SCY0.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D0);
    /*p23.FYMO*/ FYMO_SCY1.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D1);
    /*p23.FEZU*/ FEZU_SCY2.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D2);
    /*p23.FUJO*/ FUJO_SCY3.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D3);
    /*p23.DEDE*/ DEDE_SCY4.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D4);
    /*p23.FOTY*/ FOTY_SCY5.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D5);
    /*p23.FOHA*/ FOHA_SCY6.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D6);
    /*p23.FUNY*/ FUNY_SCY7.set(CAVO_FF42_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D7);

    /*p23.WARE*/ cpu_bus.TRI_D0.set_tribuf(BUWY_FF42_RDn, GAVE_SCY0.q());
    /*p23.GOBA*/ cpu_bus.TRI_D1.set_tribuf(BUWY_FF42_RDn, FYMO_SCY1.q());
    /*p23.GONU*/ cpu_bus.TRI_D2.set_tribuf(BUWY_FF42_RDn, FEZU_SCY2.q());
    /*p23.GODO*/ cpu_bus.TRI_D3.set_tribuf(BUWY_FF42_RDn, FUJO_SCY3.q());
    /*p23.CUSA*/ cpu_bus.TRI_D4.set_tribuf(BUWY_FF42_RDn, DEDE_SCY4.q());
    /*p23.GYZO*/ cpu_bus.TRI_D5.set_tribuf(BUWY_FF42_RDn, FOTY_SCY5.q());
    /*p23.GUNE*/ cpu_bus.TRI_D6.set_tribuf(BUWY_FF42_RDn, FOHA_SCY6.q());
    /*p23.GYZA*/ cpu_bus.TRI_D7.set_tribuf(BUWY_FF42_RDn, FUNY_SCY7.q());
  }

  // FF43 SCX
  {
    /*p22.WAVU*/ wire WAVU_FF43n = nand(cpu_sig.WERO_FF40_FF4Fp, cpu_sig.WADO_A00p, cpu_sig.WESA_A01p, cpu_sig.XUSY_A02n, cpu_sig.XERA_A03n);
    /*p22.XAVY*/ wire XAVY_FF43p = not(WAVU_FF43n);

    /*p23.AVOG*/ wire AVOG_FF43_RDp = and (XAVY_FF43p, cpu_sig.ASOT_CPU_RD);
    /*p23.BEBA*/ wire BEBA_FF43_RDn = not(AVOG_FF43_RDp);

    /*p23.ARUR*/ wire ARUR_FF43_WRp = and (XAVY_FF43p, cpu_sig.CUPA_CPU_WR_xxxxxFGH);
    /*p23.AMUN*/ wire AMUN_FF43_WRn = not(ARUR_FF43_WRp);

    /*p23.DATY*/ DATY_SCX0.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D0);
    /*p23.DUZU*/ DUZU_SCX1.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D1);
    /*p23.CYXU*/ CYXU_SCX2.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D2);
    /*p23.GUBO*/ GUBO_SCX3.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D3);
    /*p23.BEMY*/ BEMY_SCX4.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D4);
    /*p23.CUZY*/ CUZY_SCX5.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D5);
    /*p23.CABU*/ CABU_SCX6.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D6);
    /*p23.BAKE*/ BAKE_SCX7.set(AMUN_FF43_WRn, rst_sig.CUNU_RSTn, cpu_bus.TRI_D7);

    /*p23.EDOS*/ cpu_bus.TRI_D0.set_tribuf(!BEBA_FF43_RDn, DATY_SCX0.q());
    /*p23.EKOB*/ cpu_bus.TRI_D1.set_tribuf(!BEBA_FF43_RDn, DUZU_SCX1.q());
    /*p23.CUGA*/ cpu_bus.TRI_D2.set_tribuf(!BEBA_FF43_RDn, CYXU_SCX2.q());
    /*p23.WONY*/ cpu_bus.TRI_D3.set_tribuf(!BEBA_FF43_RDn, GUBO_SCX3.q());
    /*p23.CEDU*/ cpu_bus.TRI_D4.set_tribuf(!BEBA_FF43_RDn, BEMY_SCX4.q());
    /*p23.CATA*/ cpu_bus.TRI_D5.set_tribuf(!BEBA_FF43_RDn, CUZY_SCX5.q());
    /*p23.DOXE*/ cpu_bus.TRI_D6.set_tribuf(!BEBA_FF43_RDn, CABU_SCX6.q());
    /*p23.CASY*/ cpu_bus.TRI_D7.set_tribuf(!BEBA_FF43_RDn, BAKE_SCX7.q());
  }
}

//------------------------------------------------------------------------------

bool PpuConfig::commit() {
  bool changed = false;

  changed |= VYXE_LCDC_BGEN.commit_reg();
  changed |= XYLO_LCDC_SPEN.commit_reg();
  changed |= XYMO_LCDC_SPSIZE.commit_reg();
  changed |= XAFO_LCDC_BGMAP.commit_reg();
  changed |= WEXU_LCDC_BGTILE.commit_reg();
  changed |= WYMO_LCDC_WINEN.commit_reg();
  changed |= WOKY_LCDC_WINMAP.commit_reg();
  changed |= XONA_LCDC_EN.commit_reg();

  changed |= GAVE_SCY0.commit_reg();
  changed |= FYMO_SCY1.commit_reg();
  changed |= FEZU_SCY2.commit_reg();
  changed |= FUJO_SCY3.commit_reg();
  changed |= DEDE_SCY4.commit_reg();
  changed |= FOTY_SCY5.commit_reg();
  changed |= FOHA_SCY6.commit_reg();
  changed |= FUNY_SCY7.commit_reg();

  changed |= DATY_SCX0.commit_reg();
  changed |= DUZU_SCX1.commit_reg();
  changed |= CYXU_SCX2.commit_reg();
  changed |= GUBO_SCX3.commit_reg();
  changed |= BEMY_SCX4.commit_reg();
  changed |= CUZY_SCX5.commit_reg();
  changed |= CABU_SCX6.commit_reg();
  changed |= BAKE_SCX7.commit_reg();


  return changed;
}


//------------------------------------------------------------------------------
