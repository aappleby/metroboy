#include "AppLib/DummyApp.h"
#include "AppLib/GLBase.h"

//-----------------------------------------------------------------------------

DummyApp::~DummyApp() {
};

const char* DummyApp::app_get_title() {
  return "DummyApp";
}

//-----------------------------------------------------------------------------

void DummyApp::app_init() {
  grid_painter.init();
  text_painter.init();
  blitter.init();

  uint32_t pix[] = {
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
  };

  tex = create_texture_u32(4, 4, pix);
}

//-----------------------------------------------------------------------------

void DummyApp::app_close() {
}

//-----------------------------------------------------------------------------

void DummyApp::app_update(Viewport view, double delta) {
  (void)view;
  (void)delta;
}

//-----------------------------------------------------------------------------

void DummyApp::app_render_frame(Viewport view) {
  grid_painter.render(view);

  static int count = 0;
  text_painter.dprintf("Hello World %d\n", count++);
  text_painter.render(view, 0, 0, 1);

  blitter.blit(view, tex, 128, 128, 32, 32);
}

//-----------------------------------------------------------------------------

void DummyApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
