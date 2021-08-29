#pragma once
#include "glm/glm/glm.hpp"

using namespace glm;

double timestamp();

double ease(double a, double b, double delta);

double ppw_to_zoom(double ppw);
double zoom_to_ppw(double ppw);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Viewport {

  static Viewport from_min_max_screen(dvec2 world_min, dvec2 world_max, dvec2 screen_size) {
    double ppw = screen_size.x / (world_max.x - world_min.x);
    double zoom = ppw_to_zoom(ppw);

    return Viewport {
      screen_size,
      (world_min + world_max) * 0.5,
      zoom
    };
  }

  static Viewport from_center_zoom_screen(dvec2 center, double zoom, dvec2 screen_size) {
    dvec2 world_size = screen_size / zoom_to_ppw(zoom);
    return Viewport{
      screen_size,
      center,
      zoom
    };
  }

  double scale_world_to_screen() const {
    return zoom_to_ppw(_view_zoom);
  }

  double scale_screen_to_world() const {
    return 1.0 / zoom_to_ppw(_view_zoom);
  }

  double view_zoom() const {
    return _view_zoom;
  }

  double view_ppw() const {
    return zoom_to_ppw(_view_zoom);
  }

  double view_scale() const {
    return 1.0 / zoom_to_ppw(_view_zoom);
  }

  dvec2 world_min() const {
    return _world_center - world_size() * 0.5;
  }

  dvec2 world_max() const {
    return _world_center + world_size() * 0.5;
  };

  dvec2 world_center() const {
    return _world_center;
  }

  dvec2 world_size() const {
    return _screen_size / zoom_to_ppw(_view_zoom);
  }

  dvec2 screen_size() const {
    return _screen_size;
  }

  dvec2 screen_center() const {
    return _screen_size * 0.5;
  }

  bool operator ==(const Viewport& b) const {
    return (_world_center == b._world_center) && (_view_zoom == b._view_zoom) && (_screen_size == b._screen_size);
  }

  void translate_x(double d) {
    _world_center.x += d;
  }

  void translate_y(double d) {
    _world_center.y += d;
  }

  double ox() const { return _world_center.x; }
  double oy() const { return _world_center.y; }
  double mx() const { return world_min().x; }
  double my() const { return world_min().y; }
  double dx() const { return world_size().x; }
  double dy() const { return world_size().y; }
  double sx() const { return _screen_size.x; }
  double sy() const { return _screen_size.y; }

  dvec2 worldToScreen(dvec2 v) const;
  dvec2 screenToWorld(dvec2 v) const;

  dvec2 deltaScreenToWorld(dvec2 delta) const;

  Viewport center_on(dvec2 c);
  Viewport zoom(dvec2 screen_pos, double zoom);
  Viewport pan(dvec2 delta);
  Viewport snap();
  Viewport ease(Viewport target, double delta);

  static Viewport screenspace(int screen_w, int screen_h) {
    return {
      {screen_w, screen_h},
      {screen_w * 0.5, screen_h * 0.5},
      0.0
    };
  }

  dvec2 _screen_size = {};
  dvec2 _world_center = {};
  double _view_zoom = 0;
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
      view_target.translate_x(-65536);
      view_target_snap.translate_x(-65536);
      view_smooth.translate_x(-65536);
      view_smooth_snap.translate_x(-65536);
    }

    if (view_target.ox()      < -32768 &&
        view_target_snap.ox() < -32768 &&
        view_smooth.ox()      < -32768 &&
        view_smooth_snap.ox() < -32768)
    {
      view_target.translate_x(65536);
      view_target_snap.translate_x(65536);
      view_smooth.translate_x(65536);
      view_smooth_snap.translate_x(65536);
    }

    if (view_target.oy()      > 32768 &&
        view_target_snap.oy() > 32768 &&
        view_smooth.oy()      > 32768 &&
        view_smooth_snap.oy() > 32768)
    {
      view_target.translate_y(-65536);
      view_target_snap.translate_y(-65536);
      view_smooth.translate_y(-65536);
      view_smooth_snap.translate_y(-65536);
    }

    if (view_target.oy()      < -32768 &&
        view_target_snap.oy() < -32768 &&
        view_smooth.oy()      < -32768 &&
        view_smooth_snap.oy() < -32768)
    {
      view_target.translate_y(65536);
      view_target_snap.translate_y(65536);
      view_smooth.translate_y(65536);
      view_smooth_snap.translate_y(65536);
    }
  }

  void pop_view() {
    view_target = view_screen;
    view_target_snap = view_screen;
  }

  Viewport view_screen;
  Viewport view_target;
  Viewport view_target_snap;

  Viewport view_smooth;
  Viewport view_smooth_snap;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
