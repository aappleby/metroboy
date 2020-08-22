#include "AppLib/AppHost.h"

#include "GateBoyApp/GateBoyApp.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)

void run_microtests();

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  App* app = new GateBoyApp();
  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}
