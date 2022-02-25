#include "uart_rx.h"

static void uart_rx_test() {
  uart_rx<> test_uart_rx;
  //test_uart_rx.init();
  test_uart_rx.tick(false, 0);
  test_uart_rx.tock(false);
}