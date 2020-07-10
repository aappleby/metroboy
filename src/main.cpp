#include "AppBase.h"

#include "metroboy_main.h"
#include "GateBoy.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)

void run_microtests();

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

  return GateBoy::main(argc, argv);

  /*
  int ret = 0;
  MetroBoyApp* app = new MetroBoyApp();
  ret = app->app_main(argc, argv);
  delete app;
  return ret;
  */
}