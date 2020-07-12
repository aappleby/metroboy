#include "Sandbox.h"
#include "Console.h"

#include "Cells.h"

#include <stdio.h>
#include <stdarg.h>

#pragma warning(disable:4100)

//void print_at(int x, int y, int color, const char* format, ...);

//void print_signal(int x, int y, const char* name, SignalState s) {
//  print_at(x, y, 0b1111, "%s %d\n", name, s.val);
//} 

//------------------------------------------------------------------------------

struct Sandbox {

  Sandbox() {
  }

  void tick() {
  }

  SignalHash commit() {
    SignalHash hash;
    return hash;
  }

  void dump(SignalHash hash, int passes) {
    /*
    auto dec = [](SignalState s) {
      if (s.error) return "ERR";
      if (s.hiz)   return "HIZ";
      if (s.clk)   return s.val ? "^1^" : "^0^";
      else         return s.val ? ".1." : ".0.";
    };

    printf("0x%016llx %2d   %s  %s  %s  %s  %s  ", hash.h, passes, dec(SYS_PIN_CLK_A), dec(SYS_PIN_CLK_B), dec(SYS_PIN_T1), dec(SYS_PIN_T2), dec(SYS_PIN_RST));
    printf("%s ", dec(TUBO.a));
    printf("%d ", get_div());

    printf("\n");
    */
  }
};

//------------------------------------------------------------------------------

void phase(Sandbox& sandbox) {
  SignalHash hash;
  int pass = 0;
  for (pass = 0; pass < 100; pass++) {
    sandbox.tick();
    SignalHash new_hash = sandbox.commit();
    if (new_hash.h == hash.h) break;
    hash = new_hash;
  }

  sandbox.dump(hash, pass);
}

void tphase(Sandbox& sandbox) {
  //sandbox.SYS_PIN_CLK_B.set(0);
  phase(sandbox);
  //sandbox.SYS_PIN_CLK_B.set(1);
  phase(sandbox);
}

void tphase_rst(Sandbox& sandbox) {
  //sandbox.SYS_PIN_RSTp.set(0);
  tphase(sandbox);
}

void tphase_run(Sandbox& sandbox) {
  //sandbox.SYS_PIN_RSTp.set(1);
  tphase(sandbox);
}

//------------------------------------------------------------------------------

int main() {

  Console con;
  con.init();

  while(1) {
    con.begin_frame();

    for (int y = 0; y < 20; y++) {
      for (int x = 0; x < 20; x++) {
        int sx = x + ((con.frame / 10) & 0x1F);
        int sy = y + ((con.frame / 10) & 0x1F);
        con.plot(sx, sy, 0b1100, 'R');
      }
    }

    con.print_at(con.mouse_x, con.mouse_y, 0b1111, "X");
    con.print_at(30, 30, 0b1110, "Hello World");

    con.end_frame();
  }
 
  return 0;


  /*
  Sandbox sandbox;

  SignalHash hash;
  sandbox.dump(hash, 0);

  tphase_rst(sandbox);

  sandbox.SYS_PIN_CLK_A.set(1);

  for (int i = 0; i < 20; i++) {
    tphase_run(sandbox);
    printf("x\n");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  */

  /*
  int x = 0;
  while(1) {
    printf("\033[2J");
    print_signal(0, 0, "SYS_PIN_CLK_A", VAL);
    print_signal(0, 1, "SYS_PIN_CLK_B", HIZ);
    print_signal(0, 2, "SYS_PIN_CLK_C", CLK);
    print_signal(0, 3, "SYS_PIN_CLK_D", SET);
    print_signal(0, 4, "SYS_PIN_CLK_E", RST);
    print_signal(0, 5, "SYS_PIN_CLK_F", ERROR);
    printf("char %d\n", x);
    printf("\n");
  }
  */

  /*
  while(1) {
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
    printf("\033[38;2;%d;%d;%dm", 0x88, 0x88, 0xFF);
    printf("top line?");
    printf("\033[m");
    //printf("\n");
    //printf("next line\n");
  }
  */
}

//------------------------------------------------------------------------------