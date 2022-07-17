#include "GateBoyWaveBus.h"

void GateBoyWaveBus::reset() {
  memset(this, BIT_OLD | BIT_PULLED | 1, sizeof(*this));
}
