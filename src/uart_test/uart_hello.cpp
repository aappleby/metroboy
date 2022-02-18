#include "uart_hello.h"

static void uart_hello_test() {
  uart_hello t;
  t.init();
  t.tick(false, 0, 0);
  t.tock(false);
}