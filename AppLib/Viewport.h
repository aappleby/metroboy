#pragma once
#include "glm/glm/glm.hpp"

using namespace glm;

double timestamp();

double ease(double a, double b, double delta);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Viewport {
  dvec2 min = {};
  dvec2 max = {};
  dvec2 screen_size = {};

  bool operator ==(const Viewport& b) const {
    return (min == b.min) && (max == b.max) && (screen_size == b.screen_size);
  }

  double ox() const { return (min.x + max.x) * 0.5; }
  double oy() const { return (min.y + max.y) * 0.5; }
  double mx() const { return min.x; }
  double my() const { return min.y; }
  double dx() const { return (max.x - min.x); }
  double dy() const { return (max.y - min.y); }
  double sx() const { return screen_size.x; }
  double sy() const { return screen_size.y; }

  dvec2 center() {
    return (max + min) * 0.5;
  }

  dvec2 worldToScreen(dvec2 v) const;
  dvec2 screenToWorld(dvec2 v) const;

  dvec2 deltaScreenToWorld(dvec2 delta) const;

  double get_zoom() const;

  Viewport center_on(dvec2 c);
  Viewport zoom(dvec2 screen_pos, double zoom);
  Viewport pan(dvec2 delta);
  Viewport snap();
  Viewport ease(Viewport target, double delta);

  static Viewport screenspace(int screen_w, int screen_h) {
    return {
      {0,0},
      {screen_w, screen_h},
      {screen_w, screen_h},
    };
  }
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct ViewController {

  void init(int screen_w, int screen_h) {
    view_screen = Viewport::screenspace(screen_w, screen_h);

    view_target      = view_screen;
    view_target_snap = view_screen;
    view_smooth      = view_screen;
    view_smooth_snap = view_screen;
  }

  void begin_frame(int screen_w, int screen_h) {
    view_screen = Viewport::screenspace(screen_w, screen_h);
  }

  void on_mouse_wheel(int mouse_x, int mouse_y, double wheel) {
    view_target = view_target.zoom({mouse_x, mouse_y}, wheel);
    view_target_snap = view_target.snap();
  }

  void pan(double dx, double dy) {
    view_target = view_target.pan({dx, dy});
    view_target_snap = view_target.snap();
  }

  void center_on(dvec2 c) {
    view_target = view_target.center_on(c);
    view_target_snap = view_target.snap();
  }

  void update(double delta) {
    view_smooth      = view_smooth.ease(view_target, delta);
    view_smooth_snap = view_smooth_snap.ease(view_target_snap, delta);

    if (view_target.ox()      > 32768 &&
        view_target_snap.ox() > 32768 &&
        view_smooth.ox()      > 32768 &&
        view_smooth_snap.ox() > 32768)
    {
      view_target.min.x      -= 65536;
      view_target.max.x      -= 65536;
      view_target_snap.min.x -= 65536;
      view_target_snap.max.x -= 65536;
      view_smooth.min.x      -= 65536;
      view_smooth.max.x      -= 65536;
      view_smooth_snap.min.x -= 65536;
      view_smooth_snap.max.x -= 65536;
    }

    if (view_target.ox()      < -32768 &&
        view_target_snap.ox() < -32768 &&
        view_smooth.ox()      < -32768 &&
        view_smooth_snap.ox() < -32768)
    {
      view_target.min.x      += 65536;
      view_target.max.x      += 65536;
      view_target_snap.min.x += 65536;
      view_target_snap.max.x += 65536;
      view_smooth.min.x      += 65536;
      view_smooth.max.x      += 65536;
      view_smooth_snap.min.x += 65536;
      view_smooth_snap.max.x += 65536;
    }

    if (view_target.oy()      > 32768 &&
        view_target_snap.oy() > 32768 &&
        view_smooth.oy()      > 32768 &&
        view_smooth_snap.oy() > 32768)
    {
      view_target.min.y      -= 65536;
      view_target.max.y      -= 65536;
      view_target_snap.min.y -= 65536;
      view_target_snap.max.y -= 65536;
      view_smooth.min.y      -= 65536;
      view_smooth.max.y      -= 65536;
      view_smooth_snap.min.y -= 65536;
      view_smooth_snap.max.y -= 65536;
    }

    if (view_target.oy()      < -32768 &&
        view_target_snap.oy() < -32768 &&
        view_smooth.oy()      < -32768 &&
        view_smooth_snap.oy() < -32768)
    {
      view_target.min.y      += 65536;
      view_target.max.y      += 65536;
      view_target_snap.min.y += 65536;
      view_target_snap.max.y += 65536;
      view_smooth.min.y      += 65536;
      view_smooth.max.y      += 65536;
      view_smooth_snap.min.y += 65536;
      view_smooth_snap.max.y += 65536;
    }
  }

  void pop_view() {
    view_target = view_screen;
  }

  Viewport view_screen;
  Viewport view_target;
  Viewport view_target_snap;

  Viewport view_smooth;
  Viewport view_smooth_snap;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
