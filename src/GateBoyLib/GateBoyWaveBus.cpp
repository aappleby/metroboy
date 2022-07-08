#include "GateBoyWaveBus.h"

void GateBoyWaveBus::reset_to_cart() {
  memset(this, BIT_OLD | BIT_PULLED | 1, sizeof(*this));
}
