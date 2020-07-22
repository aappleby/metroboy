#include "AppHost.h"

#include "MetroBoyApp.h"
#include "GateBoyApp.h"
#include "DummyApp.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)

void run_microtests();

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

#if 1

  printf("Running benchmark in slow mode\n");
  GateBoy gateboy1;
  gateboy1.run_benchmark(false);
  printf("\n");

  printf("Running benchmark in fast mode\n");
  GateBoy gateboy2;
  gateboy2.run_benchmark(true);
  printf("\n");

  if (gateboy1.phase_hash_bits != gateboy2.phase_hash_bits) {
    printf("XXXXXXXXXX FAIL PHASE BITS  XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS PHASE BITS  ----------\n");
  }

  if (gateboy1.phase_hash_regs != gateboy2.phase_hash_regs) {
    printf("XXXXXXXXXX FAIL PHASE REGS  XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS PHASE REGS  ----------\n");
  }

  if (gateboy1.phase_hash_bytes != gateboy2.phase_hash_bytes) {
    printf("XXXXXXXXXX FAIL PHASE BYTES XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS PHASE BYTES ----------\n");
  }



  if (gateboy1.combined_hash_bits != gateboy2.combined_hash_bits) {
    printf("XXXXXXXXXX FAIL COMBINED BITS  XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS COMBINED BITS  ----------\n");
  }

  if (gateboy1.combined_hash_regs != gateboy2.combined_hash_regs) {
    printf("XXXXXXXXXX FAIL COMBINED REGS  XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS COMBINED REGS  ----------\n");
  }

  if (gateboy1.combined_hash_bytes != gateboy2.combined_hash_bytes) {
    printf("XXXXXXXXXX FAIL COMBINED BYTES XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS COMBINED BYTES ----------\n");
  }

#else
  App* app = new GateBoyApp();
  //App* app = new DummyApp();
  //App* app = new MetroBoyApp();

  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
#endif
}