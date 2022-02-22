#include <stdio.h>
#include <assert.h>
#include <stdint.h>

//#include "verilated_vcd_c.h"
//#include "../obj/DUT.h"
//#include "obj_dir/DUT.h"

#include "out/uart/Vuart_top.h"
#include "out/uart/Vuart_top_uart_top.h"
#include "out/uart/Vuart_top_uart_hello.h"
#include "out/uart/Vuart_top_uart_tx__C3.h"
#include "out/uart/Vuart_top_uart_rx__C3.h"

#include "uart/uart_top.h"

//------------------------------------------------------------------------------

void dump(Vuart_top& vtop) {
  auto top = vtop.uart_top;

  printf("v: ");
  //printf("top %03d ", top->counter);
  printf("hello i_cts=%d i_idle=%d o_data=x%02x o_req=%d o_done=%d s=%d data=x%02x cursor=%d",
    top->hello->i_cts,
    top->hello->i_idle,
    top->hello->o_data,
    top->hello->o_req,
    top->hello->o_done,
    top->hello->s,
    top->hello->data,
    top->hello->cursor);

  //printf("tx %03d %03d %03d ", top->tx->cycle, top->tx->cursor, top->tx->buffer);
  //printf("rx %03d %03d %03d 0x%08x", top->rx->cycle, top->rx->cursor, top->rx->buffer, top->rx->sum);

  printf("\n");

  //state s;
  //logic<cursor_bits> cursor;
  //logic<8> data;


}

void dump(uart_top<3>& top) {

  printf("m: ");

  printf("hello i_cts=%d i_idle=%d o_data=x%02x o_req=%d o_done=%d s=%d data=x%02x cursor=%d",
    (int)top.tx.o_cts,     //top.hello.i_cts,
    (int)top.tx.o_idle,    //top.hello.i_idle,
    (int)top.hello.o_data,
    (int)top.hello.o_req,
    (int)top.hello.o_done,
    (int)top.hello.s,
    (int)top.hello.data,
    (int)top.hello.cursor);



  //printf("m: top %03d hello %03d %03d %03d ", top.counter, top.hello.s, (int)top.hello.cursor, (int)top.hello.data);
  //printf("m: top %03d hello %03d %03d %03d ", top.counter, top.hello.s, (int)top.hello.cursor, (int)top.hello.data);
  //printf("tx %03d %03d %03d ", (int)top.tx.cycle, (int)top.tx.cursor, (int)top.tx.buffer);
  //printf("rx %03d %03d %03d 0x%08x", (int)top.rx.cycle, (int)top.rx.cursor, (int)top.rx.buffer, (uint32_t)top.rx.sum);

  printf("\n");

  //state s;
  //logic<cursor_bits> cursor;
  //logic<8> data;


}
//------------------------------------------------------------------------------

int test_metron(int argc, char** argv) {
  printf("Metron simulation:\n");
  printf("================================================================================\n");

  const int cycles_per_bit = 3;
  uart_top<cycles_per_bit> top;
  top.init();
  top.tick(0);
  top.tock(0);

  for (int cycle = 0; cycle < 20000; cycle++) {
    bool old_valid = top.o_valid;
    top.tick(1);
    top.tock(1);
    if (!old_valid && top.o_valid) printf("%c", (uint8_t)top.o_data);
    
    if (top.o_done) {
      printf("\n");
      printf("================================================================================\n");
      printf("%d\n", cycle);
      if (top.o_sum == 0x0000b764) {
        //printf("All tests pass.\n");
        return 0;
      }
    }
  }
  return -1;
}

//------------------------------------------------------------------------------

void test_lockstep(int argc, char** argv) {
  printf("Metron+Verilator lockstep simulation:\n");
  printf("================================================================================\n");

  {
    printf("\n");
    printf("verilated:\n");
    // Verilated Metron sim
    Vuart_top vuart_top;
    dump(vuart_top);

    // Reset cycle
    vuart_top.clk = 0;
    vuart_top.rst_n = 1;
    vuart_top.eval();
    vuart_top.rst_n = 0;
    vuart_top.eval();
    vuart_top.rst_n = 1;
    vuart_top.eval();
    printf("reset: ");
    dump(vuart_top);

    // Cycles
    for (int i = 0; i < 30; i++) {
      vuart_top.clk = 1;
      vuart_top.eval();
      vuart_top.clk = 0;
      vuart_top.eval();
      printf("c%04d: ", i);
      dump(vuart_top);
    }
  }


  // Metron sim
  {
    printf("\n");
    printf("metron:\n");
    const int cycles_per_bit = 3;
    uart_top<cycles_per_bit> muart_top;
    memset(&muart_top, 0, sizeof(muart_top));

    muart_top.init();

    // Reset cycle
    muart_top.tick(0);
    muart_top.tock(0);
    printf("reset: ");
    dump(muart_top);

    // Cycles
    for (int i = 0; i < 30; i++) {
      muart_top.tick(1);
      muart_top.tock(1);
      printf("c%04d: ", i);
      dump(muart_top);
    }
  }


#if 0
  for (int cycle = 0; cycle < 20000; cycle++) {

    bool old_valid = vuart_top.o_valid;

    vuart_top.clk = 0;
    vuart_top.rst_n = 1;
    vuart_top.eval();

    vuart_top.clk = 1;
    vuart_top.rst_n = 1;
    vuart_top.eval();

    //muart_top.tick(1);
    //muart_top.tock(1);
    //
    //if (muart_top.o_serial != vuart_top.o_serial ||
    //    muart_top.o_data   != vuart_top.o_data ||
    //    muart_top.o_valid  != vuart_top.o_valid ||
    //    muart_top.o_done   != vuart_top.o_done ||
    //    muart_top.o_sum    != vuart_top.o_sum) {
    //  printf("Mismatch @ %d\n", cycle);
    //  return;
    //}

    if (!old_valid && vuart_top.o_valid) printf("%c", (uint8_t)vuart_top.o_data);
    
    if (vuart_top.o_done) {
      printf("\n");
      printf("================================================================================\n");
      if (vuart_top.o_sum == 0x0000b764) printf("All tests pass.\n");
      break;
    }
  }
#endif
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  //test_metron(argc, argv);
  test_lockstep(argc, argv);
  return 0;
}

//------------------------------------------------------------------------------
