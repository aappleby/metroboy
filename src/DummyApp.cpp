#include "DummyApp.h"
#include "GLBase.h"

//-----------------------------------------------------------------------------

DummyApp::~DummyApp() {
};

const char* DummyApp::get_title() {
  return "DummyApp";
}

//-----------------------------------------------------------------------------

void DummyApp::init() {
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

void DummyApp::close() {
}

//-----------------------------------------------------------------------------

void DummyApp::update(double delta) {
  (void)delta;
}

//-----------------------------------------------------------------------------

void DummyApp::render_frame(Viewport view) {
  (void)view;
  grid_painter.render(view);

  static int count = 0;
  text_painter.dprintf("Hello World %d\n", count++);
  text_painter.render(view, 0, 0, 1);

  blitter.blit(view, tex, 128, 128, 32, 32);
}

//-----------------------------------------------------------------------------

void DummyApp::render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
