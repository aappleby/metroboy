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

void DummyApp::app_init(int screen_w, int screen_h) {
  dvec2 screen_size(screen_w, screen_h);

  grid_painter.init(65536.0f, 65536.0f);
  text_painter.init();
  blitter.init();

  uint32_t pix[] = {
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
  };

  tex = create_texture_u32(4, 4, pix, false);

  view_control.init(screen_size);
}

//-----------------------------------------------------------------------------

void DummyApp::app_close() {
}

//-----------------------------------------------------------------------------

void DummyApp::app_update(dvec2 screen_size, double delta) {
  (void)delta;

  int mouse_x = 0, mouse_y = 0;
  dvec2 mouse_pos_screen(mouse_x, mouse_y);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEWHEEL) {
      //printf("wheel\n");
      view_control.on_mouse_wheel(mouse_pos_screen, screen_size, double(event.wheel.y) * 0.25);
    }

    if (event.motion.state & SDL_BUTTON_LMASK) {
      //printf("xrel %d\n", event.motion.xrel);
      //printf("yrel %d\n", event.motion.yrel);

      // Getting garbage xrel/yrel under WSLg
      if (abs(event.motion.xrel) < 1000 && abs(event.motion.yrel) < 1000) {
        view_control.pan(dvec2(event.motion.xrel, event.motion.yrel));
      }
    }
  }

  view_control.update(delta);
}

//-----------------------------------------------------------------------------

void DummyApp::app_render_frame(dvec2 screen_size, double delta) {
  (void)delta;

  auto& view = view_control.view_smooth_snap;

  grid_painter.render(view, screen_size);

  static int count = 0;
  text_painter.dprintf("Hello World %d\n", count++);
  text_painter.render(view, screen_size, 0, 0);

  blitter.blit(view, screen_size, tex, 128, 128, 32, 32);
}

//-----------------------------------------------------------------------------

void DummyApp::app_render_ui(dvec2 screen_size, double delta) {
  (void)screen_size;
  (void)delta;
}

//-----------------------------------------------------------------------------
