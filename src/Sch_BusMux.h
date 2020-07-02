#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct BusMuxSignals {
  /*p28.GEKA*/ Signal GEKA_OAM_A0p;
  /*p28.ZYFO*/ Signal ZYFO_OAM_A1p;
  /*p28.YFOT*/ Signal YFOT_OAM_A2p;
  /*p28.YFOC*/ Signal YFOC_OAM_A3p;
  /*p28.YVOM*/ Signal YVOM_OAM_A4p;
  /*p28.YMEV*/ Signal YMEV_OAM_A5p;
  /*p28.XEMU*/ Signal XEMU_OAM_A6p;
  /*p28.YZET*/ Signal YZET_OAM_A7p;

  /*p28.APAG*/ Signal APAG_CPUD_TO_OAMDp;  // to oam bus, drives cpu bus data onto oam bus data
  /*p25.SAZO*/ Signal SAZO_VRAM_RD;        // to vram pins
  /*p04.WYJA*/ Signal WYJA_OAM_WRp;        // to oam, controls PIN_OAM_WR*
  /*p28.AMAB*/ Signal AMAB_OAM_LOCKn;      // to oam, controls driving OAMD to CPUD
  /*p25.COTA*/ Signal COTA_OAM_CLK;        // to oam, controls PIN_CLK
  /*p28.ASYT*/ Signal ASYT_OAM_LATCH;      // to oam, controls PIN_OE and OAMD to OAML
  /*p25.TYVY*/ Signal TYVY_VRAMD_TO_CPUDp; // to busmux.tick, controls VRAMD to CPUD

  /*p08.TYHO*/ Signal TYHO_CS_A;

  /*p08.TYMU*/ Signal TYMU_RD_OUTn;
  /*p08.PUVA*/ Signal PUVA_WR_OUTn;
};

//-----------------------------------------------------------------------------

struct BusMux {
  BusMuxSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit(void);

private:

  //----------
  // Cart pins

  /* PIN_79 */ PinOut PIN_RD_A;   // <- P08.UGAC
  /* PIN_79 */ PinOut PIN_RD_D;   // <- P08.URUN
  /* PIN_78 */ PinOut PIN_WR_A;   // <- P08.UVER
  /* PIN_78 */ PinOut PIN_WR_D;   // <- P08.USUF
  /* PIN_80 */ PinOut PIN_CS_A;   // <- P08.TYHO
                                  
  //----------
  // Address pins

  /* PIN_01 */ PinOut PIN_A00_A;   // <- P08.KUPO
  /* PIN_01 */ PinOut PIN_A00_D;   // <- P08.KOTY
  /* PIN_02 */ PinOut PIN_A01_A;   // <- P08.CABA
  /* PIN_02 */ PinOut PIN_A01_D;   // <- P08.COTU
  /* PIN_03 */ PinOut PIN_A02_A;   // <- P08.BOKU
  /* PIN_03 */ PinOut PIN_A02_D;   // <- P08.BAJO
  /* PIN_04 */ PinOut PIN_A03_A;   // <- P08.BOTY
  /* PIN_04 */ PinOut PIN_A03_D;   // <- P08.BOLA
  /* PIN_05 */ PinOut PIN_A04_A;   // <- P08.BYLA
  /* PIN_05 */ PinOut PIN_A04_D;   // <- P08.BEVO
  /* PIN_06 */ PinOut PIN_A05_A;   // <- P08.BADU
  /* PIN_06 */ PinOut PIN_A05_D;   // <- P08.AJAV
  /* PIN_07 */ PinOut PIN_A06_A;   // <- P08.CEPU
  /* PIN_07 */ PinOut PIN_A06_D;   // <- P08.CYKA
  /* PIN_08 */ PinOut PIN_A07_A;   // <- P08.DEFY
  /* PIN_08 */ PinOut PIN_A07_D;   // <- P08.COLO
  /* PIN_09 */ PinOut PIN_A08_A;   // <- P08.MYNY
  /* PIN_09 */ PinOut PIN_A08_D;   // <- P08.MEGO
  /* PIN_10 */ PinOut PIN_A09_A;   // <- P08.MUNE
  /* PIN_10 */ PinOut PIN_A09_D;   // <- P08.MENY
  /* PIN_11 */ PinOut PIN_A10_A;   // <- P08.ROXU
  /* PIN_11 */ PinOut PIN_A10_D;   // <- P08.RORE
  /* PIN_12 */ PinOut PIN_A11_A;   // <- P08.LEPY
  /* PIN_12 */ PinOut PIN_A11_D;   // <- P08.LYNY
  /* PIN_13 */ PinOut PIN_A12_A;   // <- P08.LUCE
  /* PIN_13 */ PinOut PIN_A12_D;   // <- P08.LOSO
  /* PIN_14 */ PinOut PIN_A13_A;   // <- P08.LABE
  /* PIN_14 */ PinOut PIN_A13_D;   // <- P08.LEVA
  /* PIN_15 */ PinOut PIN_A14_A;   // <- P08.PUHE
  /* PIN_15 */ PinOut PIN_A14_D;   // <- P08.PAHY
  /* PIN_16 */ PinOut PIN_A15_A;   // <- P08.SUZE
  /* PIN_16 */ PinOut PIN_A15_D;   // <- P08.RULO
};

//-----------------------------------------------------------------------------

}; // namespace Schematics