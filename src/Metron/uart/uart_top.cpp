#include "uart_top.h"

static void uart_top_test() {
  uart_top t;
  t.init();
  t.tick(false);
  t.tock();
}