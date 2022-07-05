#include "GateBoyWaveBus.h"

void GateBoyWaveBus::reset_to_bootrom() {
  memset(this, BIT_OLD | BIT_DRIVEN | 1, sizeof(*this));
}

void GateBoyWaveBus::reset_to_cart() {
  memset(this, BIT_OLD | BIT_DRIVEN | 1, sizeof(*this));
}
