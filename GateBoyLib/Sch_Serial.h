#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct SerialRegisters {
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d) const;

  int get_data() const {
    return pack_p(EDER_SER_DATA7.qp(), EROD_SER_DATA6.qp(), EJAB_SER_DATA5.qp(), DOVU_SER_DATA4.qp(),
                DOJO_SER_DATA3.qp(), DYRA_SER_DATA2.qp(), DEGU_SER_DATA1.qp(), CUBA_SER_DATA0.qp());
  }

//private:

  /*p06.ETAF*/ RegQN  ETAF_XFER_START    = REG_D0C0;
  /*p06.CULY*/ RegQPN CULY_XFER_DIR      = REG_D0C0;
                                   
  /*p06.COTY*/ RegQPN COTY_SER_CLK       = REG_D0C0;
                                   
  /*p06.CAFA*/ RegQN CAFA_SER_CNT0      = REG_D0C0;
  /*p06.CYLO*/ RegQN CYLO_SER_CNT1      = REG_D0C0;
  /*p06.CYDE*/ RegQN CYDE_SER_CNT2      = REG_D0C0;
                                   
  /*p06.CUBA*/ Reg CUBA_SER_DATA0     = REG_D0C0;
  /*p06.DEGU*/ Reg DEGU_SER_DATA1     = REG_D0C0;
  /*p06.DYRA*/ Reg DYRA_SER_DATA2     = REG_D0C0;
  /*p06.DOJO*/ Reg DOJO_SER_DATA3     = REG_D0C0;
  /*p06.DOVU*/ Reg DOVU_SER_DATA4     = REG_D0C0;
  /*p06.EJAB*/ Reg EJAB_SER_DATA5     = REG_D0C0;
  /*p06.EROD*/ Reg EROD_SER_DATA6     = REG_D0C0;
  /*p06.EDER*/ Reg EDER_SER_DATA7     = REG_D0C0;
                                   
  /*p06.ELYS*/ RegQP ELYS_SER_OUT       = REG_D0C0;

  /*p06.CALY*/ RegQPN CALY_SER_INTp = REG_D0C0;
  //----------
  // Serial pins

  /* PIN_68 */ Tri SCK_A = TRI_HZNP;   // <- P06.KEXU
  /* PIN_68 */ Tri SCK_B = TRI_HZNP;   // <- P06.CULY
  /* PIN_68 */ Tri SCK_C = TRI_D0NP;   // -> P06.CAVE
  /* PIN_68 */ Tri SCK_D = TRI_HZNP;   // <- P06.KUJO

  ///* PIN_69 */ Tri SIN_A = TRI_HZNP;   // nc?
  ///* PIN_69 */ Tri SIN_B = TRI_HZNP;   // nc?
  /* PIN_69 */ Tri SIN_Cn = TRI_D0NP;   // -> P06.CAGE
  ///* PIN_69 */ Tri SIN_D = TRI_HZNP;   // nc?

  /* PIN_70 */ Tri SOUTn  = TRI_HZNP;   // <- P05.KENA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics