#include "Sch_CpuBus.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void CpuBus::set_cpu_req(Req req) {

  _CPU_PIN_RDp.set(req.read);
  _CPU_PIN_WRp.set(req.write);

  _CPU_PIN5.set(0);
  _CPU_PIN6.set(0);

  // this probably isn't right
  _CPU_PIN_ADDR_VALID.set(req.read || req.write);

  CPU_PIN_A00.set(req.addr & 0x0001);
  CPU_PIN_A01.set(req.addr & 0x0002);
  CPU_PIN_A02.set(req.addr & 0x0004);
  CPU_PIN_A03.set(req.addr & 0x0008);
  CPU_PIN_A04.set(req.addr & 0x0010);
  CPU_PIN_A05.set(req.addr & 0x0020);
  CPU_PIN_A06.set(req.addr & 0x0040);
  CPU_PIN_A07.set(req.addr & 0x0080);
  CPU_PIN_A08.set(req.addr & 0x0100);
  CPU_PIN_A09.set(req.addr & 0x0200);
  CPU_PIN_A10.set(req.addr & 0x0400);
  CPU_PIN_A11.set(req.addr & 0x0800);
  CPU_PIN_A12.set(req.addr & 0x1000);
  CPU_PIN_A13.set(req.addr & 0x2000);
  CPU_PIN_A14.set(req.addr & 0x4000);
  CPU_PIN_A15.set(req.addr & 0x8000);

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
}

//------------------------------------------------------------------------------

CpuBus::CpuBus() {
  CPU_TRI_D0.preset_a(0);
  CPU_TRI_D1.preset_a(0);
  CPU_TRI_D2.preset_a(0);
  CPU_TRI_D3.preset_a(0);
  CPU_TRI_D4.preset_a(0);
  CPU_TRI_D5.preset_a(0);
  CPU_TRI_D6.preset_a(0);
  CPU_TRI_D7.preset_a(0);
}

//------------------------------------------------------------------------------

void CpuBus::tock(const SchematicTop& top) {

  /*p04.MAKA*/ _MAKA_FROM_CPU5_SYNC = ff17(top.clk_reg.ZEME_AxCxExGx(), top.clk_reg.CUNU_SYS_RSTn(), _CPU_PIN5);

  _CPU_PIN_ADDR_HI.set(SYRO_FE00_FFFFp());
  _CPU_PIN_BOOTp.set(top.TUTU_ADDR_BOOTp());


}

//------------------------------------------------------------------------------

SignalHash CpuBus::commit() {
  SignalHash hash;

  hash << CPU_TRI_D0.commit();
  hash << CPU_TRI_D1.commit();
  hash << CPU_TRI_D2.commit();
  hash << CPU_TRI_D3.commit();
  hash << CPU_TRI_D4.commit();
  hash << CPU_TRI_D5.commit();
  hash << CPU_TRI_D6.commit();
  hash << CPU_TRI_D7.commit();

  //----------------------------------------
  // SOC-to-CPU

  hash << _CPU_PIN_BOOTp.commit();         // PORTA_04: <- TUTU
  hash << _CPU_PIN_ADDR_HI.commit();       // PORTA_03: <- SYRO

  //----------------------------------------
  // CPU-to-SOC

  hash << _CPU_PIN6.commit_input();               // PORTD_00: -> LEXY, doesn't do anything
  hash << _CPU_PIN5.commit_input();               // PORTD_06: -> FROM_CPU5

  hash << _MAKA_FROM_CPU5_SYNC.commit();

  hash << _CPU_PIN_RDp.commit_input();            // PORTA_00: -> UJYV, LAGU, LAVO
  hash << _CPU_PIN_WRp.commit_input();            // PORTA_01: -> AREV, LAGU.
  hash << _CPU_PIN_ADDR_VALID.commit_input();     // PORTA_06: -> APAP, TEXO

  hash << CPU_PIN_A00.commit_input();
  hash << CPU_PIN_A01.commit_input();
  hash << CPU_PIN_A02.commit_input();
  hash << CPU_PIN_A03.commit_input();
  hash << CPU_PIN_A04.commit_input();
  hash << CPU_PIN_A05.commit_input();
  hash << CPU_PIN_A06.commit_input();
  hash << CPU_PIN_A07.commit_input();
  hash << CPU_PIN_A08.commit_input();
  hash << CPU_PIN_A09.commit_input();
  hash << CPU_PIN_A10.commit_input();
  hash << CPU_PIN_A11.commit_input();
  hash << CPU_PIN_A12.commit_input();
  hash << CPU_PIN_A13.commit_input();
  hash << CPU_PIN_A14.commit_input();
  hash << CPU_PIN_A15.commit_input();

  return hash;
}

//------------------------------------------------------------------------------