#pragma once
#include <stdint.h>

#if 0

struct LogicBoyReset {
  void poweron() {
    memset(this, 0, sizeof(*this));
  }

  void reset() {
    TUBO_WAITINGp  = 0;
    ASOL_POR_DONEn = 0;
    AFER_SYS_RSTp  = 0;
    SOTO_DBG_VRAMp = 0;

    SIG_CPU_EXT_CLKGOOD = 1;
    SIG_CPU_EXT_RESETp  = 0;
    SIG_CPU_STARTp      = 0;
    SIG_CPU_INT_RESETp  = 0;
  }

  uint8_t TUBO_WAITINGp;
  uint8_t ASOL_POR_DONEn;
  uint8_t AFER_SYS_RSTp;
  uint8_t SOTO_DBG_VRAMp;

  uint8_t SIG_CPU_EXT_CLKGOOD;
  uint8_t SIG_CPU_EXT_RESETp;
  uint8_t SIG_CPU_STARTp;
  uint8_t SIG_CPU_INT_RESETp;
};

#endif