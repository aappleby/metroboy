#include "AppBase.h"

#include "gateboy_main.h"
#include "metroboy_main.h"

#pragma warning(disable:4100)
#pragma warning(disable:4189)

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  
  MetroBoyApp* app = new MetroBoyApp();
  //GateboyMain* app = new GateboyMain();

  int ret = app->app_main(argc, argv);
  delete app;
  return ret;
}