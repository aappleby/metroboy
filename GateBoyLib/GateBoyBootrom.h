#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoyBootrom {
  void reset_cart();

  void tock(wire BUS_CPU_A[16],
            wire BUS_CPU_D[8],
            wire UMUT_MODE_DBG1p_ext,
            wire AVOR_SYS_RSTp,
            wire TEDO_CPU_RDp,
            wire TAPU_CPU_WRp,
            uint8_t* boot_buf,
            BusOut BUS_CPU_D_out[8]);

  wire TUTU_READ_BOOTROMp_new(wire BUS_CPU_A[16]);

  /*p07.TEPU*/ DFF17 BOOT_BITn_h;
};

