#include "AppHost.h"

#include "MetroBoyApp.h"
#include "GateBoyApp.h"
#include "DummyApp.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)

void run_microtests();

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

  //return GateBoy::main(argc, argv);

  App* app = new DummyApp();

  int ret = 0;
  AppHost* app_host = new AppHost(app);
  ret = app_host->app_main(argc, argv);
  delete app;
  return ret;

  /*
  int ret = 0;
  MetroBoyApp* app = new MetroBoyApp();
  ret = app->app_main(argc, argv);
  delete app;
  return ret;
  */
}