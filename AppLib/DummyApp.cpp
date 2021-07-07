#include "AppLib/DummyApp.h"
#include "AppLib/GLBase.h"

#include "SDL/include/SDL.h"

//-----------------------------------------------------------------------------

DummyApp::~DummyApp() {
};

const char* DummyApp::app_get_title() {
  return "DummyApp";
}

//-----------------------------------------------------------------------------

void DummyApp::app_init(int _screen_w, int _screen_h) {
  screen_w = _screen_w;
  screen_h = _screen_h;

  grid_painter.init(65536.0f, 65536.0f);
  text_painter.init();
  blitter.init();

  uint32_t pix[] = {
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
  };

  tex = create_texture_u32(4, 4, pix);

  view_control.init(screen_w, screen_h);
}

//-----------------------------------------------------------------------------

void DummyApp::app_close() {
}

//-----------------------------------------------------------------------------

void DummyApp::app_update(double delta) {
  (void)delta;

  int mouse_x = 0, mouse_y = 0;
  //uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
  dvec2 mouse_pos_screen = { mouse_x, mouse_y };

  view_control.begin_frame(screen_w, screen_h);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEWHEEL) {
      printf("wheel\n");
      view_control.on_mouse_wheel((int)mouse_pos_screen.x, (int)mouse_pos_screen.y, double(event.wheel.y) * 0.25);
    }

    if (event.motion.state & SDL_BUTTON_LMASK) {
      printf("xrel %d\n", event.motion.xrel);
      printf("yrel %d\n", event.motion.yrel);

      // Getting garbage xrel/yrel under WSLg
      if (abs(event.motion.xrel) < 1000 && abs(event.motion.yrel) < 1000) {
        view_control.pan(event.motion.xrel, event.motion.yrel);
      }
    }
  }

  view_control.update(delta);
}

//-----------------------------------------------------------------------------

void DummyApp::app_render_frame() {

  auto& view = view_control.view_smooth_snap;

  //Viewport view = Viewport::screenspace(screen_w, screen_h);

  grid_painter.render(view);

  static int count = 0;
  text_painter.dprintf("Hello World %d\n", count++);
  text_painter.render(view, 0, 0);

  blitter.blit(view, tex, 128, 128, 32, 32);
}

//-----------------------------------------------------------------------------

void DummyApp::app_render_ui() {
}

//-----------------------------------------------------------------------------
