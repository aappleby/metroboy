#include "AppHost.h"

#include "MetroBoyApp.h"
#include "GateBoyApp.h"
#include "DummyApp.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)

void run_microtests();

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

#if 0
  printf("Running reset fuzz test in slow mode\n");
  GateBoy gateboy1;
  gateboy1.fuzz_reset_sequence(false);
  printf("\n");

  printf("Running reset fuzz test in fast mode\n");
  GateBoy gateboy2;
  gateboy2.fuzz_reset_sequence(true);
  printf("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    printf("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    printf("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS TOTAL HASH ----------\n");
  }
#endif


#if 0

  printf("Running benchmark in slow mode\n");
  GateBoy gateboy1;
  gateboy1.run_benchmark(false);
  printf("\n");

  printf("Running benchmark in fast mode\n");
  GateBoy gateboy2;
  gateboy2.run_benchmark(true);
  printf("\n");

  if (gateboy1.phase_hash != gateboy2.phase_hash) {
    printf("XXXXXXXXXX FAIL PHASE HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS PHASE HASH ----------\n");
  }

  if (gateboy1.total_hash != gateboy2.total_hash) {
    printf("XXXXXXXXXX FAIL TOTAL HASH XXXXXXXXXX\n");
  }
  else {
    printf("---------- PASS TOTAL HASH ----------\n");
  }
#endif

#if 1
  App* app = new GateBoyApp();
  //App* app = new DummyApp();
  //App* app = new MetroBoyApp();

  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
#endif
}