#pragma once
#include "glm/glm/glm.hpp"

using namespace glm;

double ease(double a, double b, double delta);

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

  Viewport zoom(dvec2 screen_pos, double zoom);
  Viewport pan(dvec2 delta);
  Viewport snap();
  Viewport ease(Viewport target, double delta);

  static Viewport screenspace(int screen_w, int screen_h) {
    return {
      {0,0},
      {screen_w, screen_h},
      {screen_w, screen_h}
    };
  }
};

struct ViewController {

  void init(int screen_w, int screen_h) {
    view_screen = Viewport::screenspace(screen_w, screen_h);
    view_target = view_screen;
    view_smooth = view_screen;
    view_snap   = view_screen;
  }

  void begin_frame(int screen_w, int screen_h) {
    view_screen = Viewport::screenspace(screen_w, screen_h);
  }

  void on_mouse_wheel(int mouse_x, int mouse_y, double wheel) {
    view_target = view_target.zoom({mouse_x, mouse_y}, wheel);
  }

  void pan(double dx, double dy) {
    view_target = view_target.pan({dx, dy});
  }

  void update(double delta) {
    Viewport snapped = view_target.snap();
    view_smooth = view_smooth.ease(view_target, delta);
    view_snap = view_snap.ease(snapped, delta);

    if (view_snap == snapped) {
      view_target = view_snap;
      view_smooth = view_snap;
    }
  }

  void pop_view() {
    view_target = view_screen;
  }

  Viewport view_screen;
  Viewport view_target;
  Viewport view_smooth;
  Viewport view_snap;
};