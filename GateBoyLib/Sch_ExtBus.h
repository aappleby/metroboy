#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoy;

//-----------------------------------------------------------------------------

struct ExtBus {
  void reset_cart() {
    ALOR_EXT_ADDR_LATCH_00p.reset(1);
    APUR_EXT_ADDR_LATCH_01p.reset(0);
    ALYR_EXT_ADDR_LATCH_02p.reset(1);
    ARET_EXT_ADDR_LATCH_03p.reset(1);
    AVYS_EXT_ADDR_LATCH_04p.reset(0);
    ATEV_EXT_ADDR_LATCH_05p.reset(0);
    AROS_EXT_ADDR_LATCH_06p.reset(1);
    ARYM_EXT_ADDR_LATCH_07p.reset(0);
    LUNO_EXT_ADDR_LATCH_08p.reset(0);
    LYSA_EXT_ADDR_LATCH_09p.reset(0);
    PATE_EXT_ADDR_LATCH_10p.reset(0);
    LUMY_EXT_ADDR_LATCH_11p.reset(0);
    LOBU_EXT_ADDR_LATCH_12p.reset(0);
    LONU_EXT_ADDR_LATCH_13p.reset(0);
    NYRE_EXT_ADDR_LATCH_14p.reset(0);

    SOMA_EXT_DATA_LATCH_D0n.reset(0);
    RONY_EXT_DATA_LATCH_D1n.reset(0);
    RAXY_EXT_DATA_LATCH_D2n.reset(0);
    SELO_EXT_DATA_LATCH_D3n.reset(0);
    SODY_EXT_DATA_LATCH_D4n.reset(0);
    SAGO_EXT_DATA_LATCH_D5n.reset(0);
    RUPA_EXT_DATA_LATCH_D6n.reset(0);
    SAZY_EXT_DATA_LATCH_D7n.reset(0);
  }

  void reset_boot() {
    ALOR_EXT_ADDR_LATCH_00p.reset(0);
    APUR_EXT_ADDR_LATCH_01p.reset(0);
    ALYR_EXT_ADDR_LATCH_02p.reset(0);
    ARET_EXT_ADDR_LATCH_03p.reset(0);
    AVYS_EXT_ADDR_LATCH_04p.reset(0);
    ATEV_EXT_ADDR_LATCH_05p.reset(0);
    AROS_EXT_ADDR_LATCH_06p.reset(0);
    ARYM_EXT_ADDR_LATCH_07p.reset(0);
    LUNO_EXT_ADDR_LATCH_08p.reset(0);
    LYSA_EXT_ADDR_LATCH_09p.reset(0);
    PATE_EXT_ADDR_LATCH_10p.reset(0);
    LUMY_EXT_ADDR_LATCH_11p.reset(0);
    LOBU_EXT_ADDR_LATCH_12p.reset(0);
    LONU_EXT_ADDR_LATCH_13p.reset(0);
    NYRE_EXT_ADDR_LATCH_14p.reset(0);

    SOMA_EXT_DATA_LATCH_D0n.reset(0);
    RONY_EXT_DATA_LATCH_D1n.reset(0);
    RAXY_EXT_DATA_LATCH_D2n.reset(0);
    SELO_EXT_DATA_LATCH_D3n.reset(0);
    SODY_EXT_DATA_LATCH_D4n.reset(0);
    SAGO_EXT_DATA_LATCH_D5n.reset(0);
    RUPA_EXT_DATA_LATCH_D6n.reset(0);
    SAZY_EXT_DATA_LATCH_D7n.reset(0);
  }

  //-----------------------------------------------------------------------------

  /*p08.ALOR*/ TpLatch ALOR_EXT_ADDR_LATCH_00p;
  /*p08.APUR*/ TpLatch APUR_EXT_ADDR_LATCH_01p;
  /*p08.ALYR*/ TpLatch ALYR_EXT_ADDR_LATCH_02p;
  /*p08.ARET*/ TpLatch ARET_EXT_ADDR_LATCH_03p;
  /*p08.AVYS*/ TpLatch AVYS_EXT_ADDR_LATCH_04p;
  /*p08.ATEV*/ TpLatch ATEV_EXT_ADDR_LATCH_05p;
  /*p08.AROS*/ TpLatch AROS_EXT_ADDR_LATCH_06p;
  /*p08.ARYM*/ TpLatch ARYM_EXT_ADDR_LATCH_07p;
  /*p08.LUNO*/ TpLatch LUNO_EXT_ADDR_LATCH_08p;
  /*p08.LYSA*/ TpLatch LYSA_EXT_ADDR_LATCH_09p;
  /*p08.PATE*/ TpLatch PATE_EXT_ADDR_LATCH_10p;
  /*p08.LUMY*/ TpLatch LUMY_EXT_ADDR_LATCH_11p;
  /*p08.LOBU*/ TpLatch LOBU_EXT_ADDR_LATCH_12p;
  /*p08.LONU*/ TpLatch LONU_EXT_ADDR_LATCH_13p;
  /*p08.NYRE*/ TpLatch NYRE_EXT_ADDR_LATCH_14p;

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_D0n;
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_D1n;
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_D2n;
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_D3n;
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_D4n;
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_D5n;
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_D6n;
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_D7n;
};

//-----------------------------------------------------------------------------
