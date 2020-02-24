#include "AppBase.h"
#include "metroboy_main.h"


int main(int argc, char** argv) {

  MetroBoyApp* app = new MetroBoyApp();
  int ret = app->main(argc, argv);
  delete app;

  //return gateboy_main(argc, argv);

  return ret;
}