#pragma once
#include <stdint.h>
#include <glm/glm.hpp>

using namespace glm;

double ease(double a, double b, double delta);

struct Viewport {
  dvec2 min;
  dvec2 max;
  dvec2 screen_size;

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

  dvec2 worldToScreen(dvec2 v) const;
  dvec2 screenToWorld(dvec2 v) const;
  double get_zoom() const;

  Viewport zoom(dvec2 screen_pos, double zoom);
  Viewport pan(dvec2 delta);
  Viewport snap();
};