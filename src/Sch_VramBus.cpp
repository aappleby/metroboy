#include "Sch_VramBus.h"

#include "TestGB.h"

using namespace Schematics;

void VramBus::tick(TestGB& /*gb*/) {
}

bool VramBus::commit() {
  bool changed = false;
  changed |= TRI_A00.commit_tribuf();
  changed |= TRI_A01.commit_tribuf();
  changed |= TRI_A02.commit_tribuf();
  changed |= TRI_A03.commit_tribuf();
  changed |= TRI_A04.commit_tribuf();
  changed |= TRI_A05.commit_tribuf();
  changed |= TRI_A06.commit_tribuf();
  changed |= TRI_A07.commit_tribuf();
  changed |= TRI_A08.commit_tribuf();
  changed |= TRI_A09.commit_tribuf();
  changed |= TRI_A10.commit_tribuf();
  changed |= TRI_A11.commit_tribuf();
  changed |= TRI_A12.commit_tribuf();

  changed |= TRI_D0.commit_tribuf();
  changed |= TRI_D1.commit_tribuf();
  changed |= TRI_D2.commit_tribuf();
  changed |= TRI_D3.commit_tribuf();
  changed |= TRI_D4.commit_tribuf();
  changed |= TRI_D5.commit_tribuf();
  changed |= TRI_D6.commit_tribuf();
  changed |= TRI_D7.commit_tribuf();
  return changed;
}