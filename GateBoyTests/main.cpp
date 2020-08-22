#include "GateBoyTests/GateBoyTests.h"

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {

#if 1
  GateBoyTests t;
  return t.test_main(argc, argv);
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