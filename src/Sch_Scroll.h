#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct ScrollSignals {
  // for fine match
  /*p23.DATY*/ bool DATY_SCX0;
  /*p23.DUZU*/ bool DUZU_SCX1;
  /*p23.CYXU*/ bool CYXU_SCX2;

  /*p26.FAFO*/ bool FAFO_TILE_Y0S;
  /*p26.EMUX*/ bool EMUX_TILE_Y1S;
  /*p26.ECAB*/ bool ECAB_TILE_Y2S;
  /*p26.ETAM*/ bool ETAM_MAP_Y0S;
  /*p26.DOTO*/ bool DOTO_MAP_Y1S;
  /*p26.DABA*/ bool DABA_MAP_Y2S;
  /*p26.EFYK*/ bool EFYK_MAP_Y3S;
  /*p26.EJOK*/ bool EJOK_MAP_Y4S;

  /*p26.BABE*/ bool BABE_MAP_X0S;
  /*p26.ABOD*/ bool ABOD_MAP_X1S;
  /*p26.BEWY*/ bool BEWY_MAP_X2S;
  /*p26.BYCA*/ bool BYCA_MAP_X3S;
  /*p26.ACUL*/ bool ACUL_MAP_X4S;
};

struct ScrollRegisters {

  ScrollRegisters() {
    DATY_SCX0.a.val = DATY_SCX0.b.val = 1;
  }

  ScrollSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  int get_scy()  const { return pack(GAVE_SCY0, FYMO_SCY1, FEZU_SCY2, FUJO_SCY3, DEDE_SCY4, FOTY_SCY5, FOHA_SCY6, FUNY_SCY7); }
  int get_scx()  const { return pack(DATY_SCX0, DUZU_SCX1, CYXU_SCX2, GUBO_SCX3, BEMY_SCX4, CUZY_SCX5, CABU_SCX6, BAKE_SCX7); }

private:

  // FF42 - SCY
  /*p23.GAVE*/ Reg GAVE_SCY0;
  /*p23.FYMO*/ Reg FYMO_SCY1;
  /*p23.FEZU*/ Reg FEZU_SCY2;
  /*p23.FUJO*/ Reg FUJO_SCY3;
  /*p23.DEDE*/ Reg DEDE_SCY4;
  /*p23.FOTY*/ Reg FOTY_SCY5;
  /*p23.FOHA*/ Reg FOHA_SCY6;
  /*p23.FUNY*/ Reg FUNY_SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg DATY_SCX0;
  /*p23.DUZU*/ Reg DUZU_SCX1;
  /*p23.CYXU*/ Reg CYXU_SCX2;
  /*p23.GUBO*/ Reg GUBO_SCX3;
  /*p23.BEMY*/ Reg BEMY_SCX4;
  /*p23.CUZY*/ Reg CUZY_SCX5;
  /*p23.CABU*/ Reg CABU_SCX6;
  /*p23.BAKE*/ Reg BAKE_SCX7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics