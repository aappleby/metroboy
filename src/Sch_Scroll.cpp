#include "Sch_Scroll.h"

#include "TestGB.h"
#include "Sch_Timer.h"

using namespace Schematics;

//------------------------------------------------------------------------------

ScrollSignals ScrollRegisters::sig(const TestGB& gb) const {
  ScrollSignals sig = {0};

  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto ppu_sig = gb.ppu_reg.sig(gb);

  //----------------------------------------

  sig.DATY_SCX0 = DATY_SCX0;
  sig.DUZU_SCX1 = DUZU_SCX1;
  sig.CYXU_SCX2 = CYXU_SCX2;

  /*p26.FAFO*/ wire TILE_Y0S = add_s(lcd_sig.MUWY_Y0, GAVE_SCY0.q(), 0);
  /*p26.FAFO*/ wire TILE_Y0C = add_c(lcd_sig.MUWY_Y0, GAVE_SCY0.q(), 0);
  /*p26.EMUX*/ wire TILE_Y1S = add_s(lcd_sig.MYRO_Y1, FYMO_SCY1.q(), TILE_Y0C);
  /*p26.EMUX*/ wire TILE_Y1C = add_c(lcd_sig.MYRO_Y1, FYMO_SCY1.q(), TILE_Y0C);
  /*p26.ECAB*/ wire TILE_Y2S = add_s(lcd_sig.LEXA_Y2, FEZU_SCY2.q(), TILE_Y1C);
  /*p26.ECAB*/ wire TILE_Y2C = add_c(lcd_sig.LEXA_Y2, FEZU_SCY2.q(), TILE_Y1C);
  /*p26.ETAM*/ wire MAP_Y0S  = add_s(lcd_sig.LYDO_Y3, FUJO_SCY3.q(), TILE_Y2C);
  /*p26.ETAM*/ wire MAP_Y0C  = add_c(lcd_sig.LYDO_Y3, FUJO_SCY3.q(), TILE_Y2C);
  /*p26.DOTO*/ wire MAP_Y1S  = add_s(lcd_sig.LOVU_Y4, DEDE_SCY4.q(), MAP_Y0C);
  /*p26.DOTO*/ wire MAP_Y1C  = add_c(lcd_sig.LOVU_Y4, DEDE_SCY4.q(), MAP_Y0C);
  /*p26.DABA*/ wire MAP_Y2S  = add_s(lcd_sig.LEMA_Y5, FOTY_SCY5.q(), MAP_Y1C);
  /*p26.DABA*/ wire MAP_Y2C  = add_c(lcd_sig.LEMA_Y5, FOTY_SCY5.q(), MAP_Y1C);
  /*p26.EFYK*/ wire MAP_Y3S  = add_s(lcd_sig.MATO_Y6, FOHA_SCY6.q(), MAP_Y2C);
  /*p26.EFYK*/ wire MAP_Y3C  = add_c(lcd_sig.MATO_Y6, FOHA_SCY6.q(), MAP_Y2C);
  /*p26.EJOK*/ wire MAP_Y4S  = add_s(lcd_sig.LAFO_Y7, FUNY_SCY7.q(), MAP_Y3C);
  /*p26.EJOK*/ wire MAP_Y4C  = add_c(lcd_sig.LAFO_Y7, FUNY_SCY7.q(), MAP_Y3C);

  sig.FAFO_TILE_Y0S = TILE_Y0S;
  sig.EMUX_TILE_Y1S = TILE_Y1S;
  sig.ECAB_TILE_Y2S = TILE_Y2S;
  sig.ETAM_MAP_Y0S  = MAP_Y0S;
  sig.DOTO_MAP_Y1S  = MAP_Y1S;
  sig.DABA_MAP_Y2S  = MAP_Y2S;
  sig.EFYK_MAP_Y3S  = MAP_Y3S;
  sig.EJOK_MAP_Y4S  = MAP_Y4S;

  (void)MAP_Y4C;

  /*p26.ATAD*/ wire TILE_X0S = add_s(ppu_sig.XEHO_X0, DATY_SCX0.q(), 0);
  /*p26.ATAD*/ wire TILE_X0C = add_c(ppu_sig.XEHO_X0, DATY_SCX0.q(), 0);
  /*p26.BEHU*/ wire TILE_X1S = add_s(ppu_sig.SAVY_X1, DUZU_SCX1.q(), TILE_X0C);
  /*p26.BEHU*/ wire TILE_X1C = add_c(ppu_sig.SAVY_X1, DUZU_SCX1.q(), TILE_X0C);
  /*p26.APYH*/ wire TILE_X2S = add_s(ppu_sig.XODU_X2, CYXU_SCX2.q(), TILE_X1C);
  /*p26.APYH*/ wire TILE_X2C = add_c(ppu_sig.XODU_X2, CYXU_SCX2.q(), TILE_X1C);
  /*p26.BABE*/ wire MAP_X0S  = add_s(ppu_sig.XYDO_X3, GUBO_SCX3.q(), TILE_X2C);
  /*p26.BABE*/ wire MAP_X0C  = add_c(ppu_sig.XYDO_X3, GUBO_SCX3.q(), TILE_X2C);
  /*p26.ABOD*/ wire MAP_X1S  = add_s(ppu_sig.TUHU_X4, BEMY_SCX4.q(), MAP_X0C);
  /*p26.ABOD*/ wire MAP_X1C  = add_c(ppu_sig.TUHU_X4, BEMY_SCX4.q(), MAP_X0C);
  /*p26.BEWY*/ wire MAP_X2S  = add_s(ppu_sig.TUKY_X5, CUZY_SCX5.q(), MAP_X1C);
  /*p26.BEWY*/ wire MAP_X2C  = add_c(ppu_sig.TUKY_X5, CUZY_SCX5.q(), MAP_X1C);
  /*p26.BYCA*/ wire MAP_X3S  = add_s(ppu_sig.TAKO_X6, CABU_SCX6.q(), MAP_X2C);
  /*p26.BYCA*/ wire MAP_X3C  = add_c(ppu_sig.TAKO_X6, CABU_SCX6.q(), MAP_X2C);
  /*p26.ACUL*/ wire MAP_X4S  = add_s(ppu_sig.SYBE_X7, BAKE_SCX7.q(), MAP_X3C);
  /*p26.ACUL*/ wire MAP_X4C  = add_c(ppu_sig.SYBE_X7, BAKE_SCX7.q(), MAP_X3C);

  sig.BABE_MAP_X0S = MAP_X0S;
  sig.ABOD_MAP_X1S = MAP_X1S;
  sig.BEWY_MAP_X2S = MAP_X2S;
  sig.BYCA_MAP_X3S = MAP_X3S;
  sig.ACUL_MAP_X4S = MAP_X4S;

  (void)TILE_X0S;
  (void)TILE_X1S;
  (void)TILE_X2S;
  (void)MAP_X4C;

  return sig;
}

//------------------------------------------------------------------------------

void ScrollRegisters::tick(TestGB& gb) {
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_bus.sig(gb);

  auto& cpu_bus = gb.cpu_bus;

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

bool ScrollRegisters::commit() {
  bool changed = false;

  /*p23.GAVE*/ changed |= GAVE_SCY0.commit_reg();
  /*p23.FYMO*/ changed |= FYMO_SCY1.commit_reg();
  /*p23.FEZU*/ changed |= FEZU_SCY2.commit_reg();
  /*p23.FUJO*/ changed |= FUJO_SCY3.commit_reg();
  /*p23.DEDE*/ changed |= DEDE_SCY4.commit_reg();
  /*p23.FOTY*/ changed |= FOTY_SCY5.commit_reg();
  /*p23.FOHA*/ changed |= FOHA_SCY6.commit_reg();
  /*p23.FUNY*/ changed |= FUNY_SCY7.commit_reg();
  /*p23.DATY*/ changed |= DATY_SCX0.commit_reg();
  /*p23.DUZU*/ changed |= DUZU_SCX1.commit_reg();
  /*p23.CYXU*/ changed |= CYXU_SCX2.commit_reg();
  /*p23.GUBO*/ changed |= GUBO_SCX3.commit_reg();
  /*p23.BEMY*/ changed |= BEMY_SCX4.commit_reg();
  /*p23.CUZY*/ changed |= CUZY_SCX5.commit_reg();
  /*p23.CABU*/ changed |= CABU_SCX6.commit_reg();
  /*p23.BAKE*/ changed |= BAKE_SCX7.commit_reg();
  
  return changed;
}

//------------------------------------------------------------------------------

//dump(text_painter,      "SCY         ", SCY0,  SCY1,  SCY2,  SCY3,  SCY4,  SCY5,  SCY6,  SCY7);
//dump(text_painter,      "SCX         ", SCX0,  SCX1,  SCX2,  SCX3,  SCX4,  SCX5,  SCX6,  SCX7);
