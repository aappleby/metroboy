#pragma once
#include "glm/glm/glm.hpp"

using namespace glm;

double timestamp();

double ease(double a, double b, double delta);

double ppw_to_zoom(double ppw);
double zoom_to_ppw(double ppw);

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct Viewport {

  static Viewport from_center_zoom(dvec2 center, double zoom) {
    return Viewport{
      center,
      zoom
    };
  }

  double view_zoom() const {
    return _view_zoom;
  }

  double scale_world_to_screen() const {
    return zoom_to_ppw(_view_zoom);
  }

  double scale_screen_to_world() const {
    return 1.0 / zoom_to_ppw(_view_zoom);
  }

  dvec2 screen_min(dvec2 screen_size) const {
    return _world_center - screen_size * scale_screen_to_world() * 0.5;
  }

  dvec2 screen_max(dvec2 screen_size) const {
    return _world_center + screen_size * scale_screen_to_world() * 0.5;
  };

  dvec2 world_center() const {
    return _world_center;
  }

  bool operator ==(const Viewport& b) const {
    return (_world_center == b._world_center) && (_view_zoom == b._view_zoom);
  }

  void translate_x(double d) {
    _world_center.x += d;
  }

  void translate_y(double d) {
    _world_center.y += d;
  }

  dvec2 world_to_screen(dvec2 v, dvec2 screen_size) const;
  dvec2 screen_to_world(dvec2 v, dvec2 screen_size) const;

  Viewport center_on(dvec2 c);
  Viewport zoom(dvec2 screen_pos, dvec2 screen_size, double zoom);
  Viewport pan(dvec2 delta);
  Viewport snap();
  Viewport ease(Viewport target, double delta);

  static Viewport screenspace(dvec2 screen_size) {
    return {
      screen_size * 0.5,
      0.0
    };
  }

  dvec2 _world_center = {};
  double _view_zoom = 0;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct ViewController {

  void init(dvec2 screen_size) {
    auto view_screen = Viewport::screenspace(screen_size);
    view_target      = view_screen;
    view_target_snap = view_screen;
    view_smooth      = view_screen;
    view_smooth_snap = view_screen;
  }

  void update(double delta) {
    view_smooth      = view_smooth.ease(view_target, delta);
    view_smooth_snap = view_smooth_snap.ease(view_target_snap, delta);



    if (view_target.world_center().x      > 32768 &&
        view_target_snap.world_center().x > 32768 &&
        view_smooth.world_center().x      > 32768 &&
        view_smooth_snap.world_center().x > 32768)
    {
      view_target.translate_x(-65536);
      view_target_snap.translate_x(-65536);
      view_smooth.translate_x(-65536);
      view_smooth_snap.translate_x(-65536);
    }

    if (view_target.world_center().x      < -32768 &&
        view_target_snap.world_center().x < -32768 &&
        view_smooth.world_center().x      < -32768 &&
        view_smooth_snap.world_center().x < -32768)
    {
      view_target.translate_x(65536);
      view_target_snap.translate_x(65536);
      view_smooth.translate_x(65536);
      view_smooth_snap.translate_x(65536);
    }

    if (view_target.world_center().y      > 32768 &&
        view_target_snap.world_center().y > 32768 &&
        view_smooth.world_center().y      > 32768 &&
        view_smooth_snap.world_center().y > 32768)
    {
      view_target.translate_y(-65536);
      view_target_snap.translate_y(-65536);
      view_smooth.translate_y(-65536);
      view_smooth_snap.translate_y(-65536);
    }

    if (view_target.world_center().y      < -32768 &&
        view_target_snap.world_center().y < -32768 &&
        view_smooth.world_center().y      < -32768 &&
        view_smooth_snap.world_center().y < -32768)
    {
      view_target.translate_y(65536);
      view_target_snap.translate_y(65536);
      view_smooth.translate_y(65536);
      view_smooth_snap.translate_y(65536);
    }
  }

  void on_mouse_wheel(dvec2 mouse_pos, dvec2 screen_size, double wheel) {
    view_target = view_target.zoom(mouse_pos, screen_size, wheel);
    view_target_snap = view_target.snap();
  }

  void pan(dvec2 delta_pos) {
    view_target = view_target.pan(delta_pos);
    view_target_snap = view_target.snap();
  }

  void center_on(dvec2 c) {
    view_target = view_target.center_on(c);
    view_target_snap = view_target.snap();
  }

  Viewport view_target;
  Viewport view_target_snap;

  Viewport view_smooth;
  Viewport view_smooth_snap;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
