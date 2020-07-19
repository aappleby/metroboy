#include "Sch_CpuBus.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void CpuBus::set_cpu_req(Req req) {

  _CPU_PIN_RDp.hold(req.read);
  _CPU_PIN_WRp.hold(req.write);

  _CPU_PIN5.hold(0);
  _CPU_PIN6.hold(0);

  // this probably isn't right
  _CPU_PIN_ADDR_VALID.hold(req.read || req.write);

  CPU_PIN_A00.hold(req.addr & 0x0001);
  CPU_PIN_A01.hold(req.addr & 0x0002);
  CPU_PIN_A02.hold(req.addr & 0x0004);
  CPU_PIN_A03.hold(req.addr & 0x0008);
  CPU_PIN_A04.hold(req.addr & 0x0010);
  CPU_PIN_A05.hold(req.addr & 0x0020);
  CPU_PIN_A06.hold(req.addr & 0x0040);
  CPU_PIN_A07.hold(req.addr & 0x0080);
  CPU_PIN_A08.hold(req.addr & 0x0100);
  CPU_PIN_A09.hold(req.addr & 0x0200);
  CPU_PIN_A10.hold(req.addr & 0x0400);
  CPU_PIN_A11.hold(req.addr & 0x0800);
  CPU_PIN_A12.hold(req.addr & 0x1000);
  CPU_PIN_A13.hold(req.addr & 0x2000);
  CPU_PIN_A14.hold(req.addr & 0x4000);
  CPU_PIN_A15.hold(req.addr & 0x8000);

  /*
  if (req.write) {
    CPU_TRI_D0.preset_a(req.data_lo & 0x01);
    CPU_TRI_D1.preset_a(req.data_lo & 0x02);
    CPU_TRI_D2.preset_a(req.data_lo & 0x04);
    CPU_TRI_D3.preset_a(req.data_lo & 0x08);
    CPU_TRI_D4.preset_a(req.data_lo & 0x10);
    CPU_TRI_D5.preset_a(req.data_lo & 0x20);
    CPU_TRI_D6.preset_a(req.data_lo & 0x40);
    CPU_TRI_D7.preset_a(req.data_lo & 0x80);

    CPU_TRI_D0.preset_b(req.data_lo & 0x01);
    CPU_TRI_D1.preset_b(req.data_lo & 0x02);
    CPU_TRI_D2.preset_b(req.data_lo & 0x04);
    CPU_TRI_D3.preset_b(req.data_lo & 0x08);
    CPU_TRI_D4.preset_b(req.data_lo & 0x10);
    CPU_TRI_D5.preset_b(req.data_lo & 0x20);
    CPU_TRI_D6.preset_b(req.data_lo & 0x40);
    CPU_TRI_D7.preset_b(req.data_lo & 0x80);
  }
  */
}

//------------------------------------------------------------------------------

CpuBus::CpuBus() {
  /*
  CPU_TRI_D0.preset_a(0);
  CPU_TRI_D1.preset_a(0);
  CPU_TRI_D2.preset_a(0);
  CPU_TRI_D3.preset_a(0);
  CPU_TRI_D4.preset_a(0);
  CPU_TRI_D5.preset_a(0);
  CPU_TRI_D6.preset_a(0);
  CPU_TRI_D7.preset_a(0);
  */
}

//------------------------------------------------------------------------------

void CpuBus::tock(const SchematicTop& top) {

  /*p04.MAKA*/ _MAKA_FROM_CPU5_SYNC = ff17_r2(top.clk_reg.ZEME_AxCxExGx(), top.clk_reg.CUNU_SYS_RSTn(), _CPU_PIN5);

  _CPU_PIN_ADDR_HI = SYRO_FE00_FFFFp();
  _CPU_PIN_BOOTp = top.TUTU_ADDR_BOOTp();


}

//------------------------------------------------------------------------------

SignalHash CpuBus::commit() {
  CPU_TRI_D0.commit();
  CPU_TRI_D1.commit();
  CPU_TRI_D2.commit();
  CPU_TRI_D3.commit();
  CPU_TRI_D4.commit();
  CPU_TRI_D5.commit();
  CPU_TRI_D6.commit();
  CPU_TRI_D7.commit();

  //----------------------------------------
  // SOC-to-CPU

  _CPU_PIN_BOOTp.commit();         // PORTA_04: <- TUTU
  _CPU_PIN_ADDR_HI.commit();       // PORTA_03: <- SYRO

  //----------------------------------------
  // CPU-to-SOC

  _CPU_PIN6.commit();               // PORTD_00: -> LEXY, doesn't do anything
  _CPU_PIN5.commit();               // PORTD_06: -> FROM_CPU5

  _MAKA_FROM_CPU5_SYNC.commit();

  _CPU_PIN_RDp.commit();            // PORTA_00: -> UJYV, LAGU, LAVO
  _CPU_PIN_WRp.commit();            // PORTA_01: -> AREV, LAGU.
  _CPU_PIN_ADDR_VALID.commit();     // PORTA_06: -> APAP, TEXO

  CPU_PIN_A00.commit();
  CPU_PIN_A01.commit();
  CPU_PIN_A02.commit();
  CPU_PIN_A03.commit();
  CPU_PIN_A04.commit();
  CPU_PIN_A05.commit();
  CPU_PIN_A06.commit();
  CPU_PIN_A07.commit();
  CPU_PIN_A08.commit();
  CPU_PIN_A09.commit();
  CPU_PIN_A10.commit();
  CPU_PIN_A11.commit();
  CPU_PIN_A12.commit();
  CPU_PIN_A13.commit();
  CPU_PIN_A14.commit();
  CPU_PIN_A15.commit();

  return {SignalHash::hash_blob(this, sizeof(*this))};
}

//------------------------------------------------------------------------------