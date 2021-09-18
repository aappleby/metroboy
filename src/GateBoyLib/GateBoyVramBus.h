#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct VramABusLo {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    BUS_VRAM_A00n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A01n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A02n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A03n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A04n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A05n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A06n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A07n.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  void reset_to_cart() {
    BUS_VRAM_A00n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A01n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A02n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A03n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A04n.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_A05n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A06n.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_A07n.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  /*_BUS_VRAM_A00n*/ Bus BUS_VRAM_A00n;
  /*_BUS_VRAM_A01n*/ Bus BUS_VRAM_A01n;
  /*_BUS_VRAM_A02n*/ Bus BUS_VRAM_A02n;
  /*_BUS_VRAM_A03n*/ Bus BUS_VRAM_A03n;
  /*_BUS_VRAM_A04n*/ Bus BUS_VRAM_A04n;
  /*_BUS_VRAM_A05n*/ Bus BUS_VRAM_A05n;
  /*_BUS_VRAM_A06n*/ Bus BUS_VRAM_A06n;
  /*_BUS_VRAM_A07n*/ Bus BUS_VRAM_A07n;
};

struct VramABusHi {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    BUS_VRAM_A08n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A09n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A10n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A11n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A12n.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  void reset_to_cart() {
    BUS_VRAM_A08n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A09n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A10n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A11n.state = BIT_OLD | BIT_DRIVEN | 1;
    BUS_VRAM_A12n.state = BIT_OLD | BIT_DRIVEN | 1;
  }

  /*_BUS_VRAM_A08n*/ Bus BUS_VRAM_A08n;
  /*_BUS_VRAM_A09n*/ Bus BUS_VRAM_A09n;
  /*_BUS_VRAM_A10n*/ Bus BUS_VRAM_A10n;
  /*_BUS_VRAM_A11n*/ Bus BUS_VRAM_A11n;
  /*_BUS_VRAM_A12n*/ Bus BUS_VRAM_A12n;
};

//-----------------------------------------------------------------------------

struct VramDBus {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_cart() {
    BUS_VRAM_D00p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D01p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D02p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D03p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D04p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D05p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D06p.state = BIT_OLD | BIT_DRIVEN | 0;
    BUS_VRAM_D07p.state = BIT_OLD | BIT_DRIVEN | 0;
  }

  /*_BUS_VRAM_D00p*/ Bus BUS_VRAM_D00p;
  /*_BUS_VRAM_D01p*/ Bus BUS_VRAM_D01p;
  /*_BUS_VRAM_D02p*/ Bus BUS_VRAM_D02p;
  /*_BUS_VRAM_D03p*/ Bus BUS_VRAM_D03p;
  /*_BUS_VRAM_D04p*/ Bus BUS_VRAM_D04p;
  /*_BUS_VRAM_D05p*/ Bus BUS_VRAM_D05p;
  /*_BUS_VRAM_D06p*/ Bus BUS_VRAM_D06p;
  /*_BUS_VRAM_D07p*/ Bus BUS_VRAM_D07p;
};

//-----------------------------------------------------------------------------
