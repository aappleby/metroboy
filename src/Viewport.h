#pragma once
#include <stdint.h>
#include <glm/glm.hpp>

using namespace glm;

double ease(double a, double b, double delta);

struct Viewport {
  dvec2 min;
  dvec2 max;
  dvec2 screen_size;

  double ox() const { return (min.x + max.x) * 0.5; }
  double oy() const { return (min.y + max.y) * 0.5; }
  double dx() const { return (max.x - min.x); }
  double dy() const { return (max.y - min.y); }

  dvec2 worldToScreen(dvec2 v) const;
  dvec2 screenToWorld(dvec2 v) const;
  double get_zoom() const;
  Viewport zoom_in_on(dvec2 screen_pos, double scale);

  static Viewport from_origin_zoom(double fb_width, double fb_height, double ox, double oy, double zoom);
};