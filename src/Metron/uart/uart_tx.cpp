#include "uart_tx.h"

static void uart_tx_test() {
  uart_tx t;
  t.tick(false, 0, 0);
  t.tock(false);
}