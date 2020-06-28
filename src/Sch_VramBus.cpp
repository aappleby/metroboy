#include "Sch_VramBus.h"

#include "TestGB.h"

using namespace Schematics;

void VramBus::tick(TestGB& /*gb*/) {
}

bool VramBus::commit() {
  bool changed = false;
  changed |= TS_MA00.commit_tribuf();
  changed |= TS_MA01.commit_tribuf();
  changed |= TS_MA02.commit_tribuf();
  changed |= TS_MA03.commit_tribuf();
  changed |= TS_MA04.commit_tribuf();
  changed |= TS_MA05.commit_tribuf();
  changed |= TS_MA06.commit_tribuf();
  changed |= TS_MA07.commit_tribuf();
  changed |= TS_MA08.commit_tribuf();
  changed |= TS_MA09.commit_tribuf();
  changed |= TS_MA10.commit_tribuf();
  changed |= TS_MA11.commit_tribuf();
  changed |= TS_MA12.commit_tribuf();
  changed |= TS_MD0.commit_tribuf();
  changed |= TS_MD1.commit_tribuf();
  changed |= TS_MD2.commit_tribuf();
  changed |= TS_MD3.commit_tribuf();
  changed |= TS_MD4.commit_tribuf();
  changed |= TS_MD5.commit_tribuf();
  changed |= TS_MD6.commit_tribuf();
  changed |= TS_MD7.commit_tribuf();
  return changed;
}