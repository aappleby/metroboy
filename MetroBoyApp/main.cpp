#include "AppLib/AppHost.h"

#include "MetroBoyApp/MetroBoyApp.h"
#include "AppLib/DummyApp.h"

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

  //App* app = new GateBoyApp();
  //App* app = new DummyApp();
  App* app = new MetroBoyApp();

  AppHost* app_host = new AppHost(app);
  int ret = app_host->app_main(argc, argv);
  delete app;
  return ret;
}