#include "Viewport.h"
#include <math.h>

#pragma warning(disable:4100)
#pragma warning(disable:4189)

double ease(double a, double b, double delta) {
  if (a == b) return a;
  double t = 1.0 - pow(0.1, delta / 70.0);
  double c = a + (b - a) * t;
  return c == a ? b : c;
}

dvec2 Viewport::worldToScreen(dvec2 v) const {
  double x = screen_size.x * (v.x - min.x) / (max.x - min.x);
  double y = screen_size.y * (v.y - min.y) / (max.y - min.y);
  return {x, y};
}

dvec2 Viewport::screenToWorld(dvec2 v) const {
  double x = (v.x / screen_size.x) * (max.x - min.x) + min.x;
  double y = (v.y / screen_size.y) * (max.y - min.y) + min.y;
  return {x, y};
}

double Viewport::get_zoom() const {
  double w = screen_size.x / (max.x - min.x);
  double z = log2(w);
  return z;
}

#if 0

nx = screen_pos.x / screen_size.x;

aw = a.max.x - a.min.x;
bw = b.max.x - b.min.x;

awx = nx * aw + a.min.x;
bwx = nx * bw + b.min.x;
bw = aw * delta


nx * aw + a.min.x = nx * aw * delta + b.min.x;

b.min.x = nx * aw + a.min.x - nx * aw * delta
b.max.x = b.min.x + bw;


#endif

Viewport Viewport::zoom_in_on(dvec2 screen_pos, double scale) {
  Viewport& a = *this;

  double nx = screen_pos.x / screen_size.x;
  double ny = screen_pos.y / screen_size.y;

  double aw = a.max.x - a.min.x;
  double ah = a.max.y - a.min.y;

  Viewport b;
  b.screen_size = a.screen_size;

  b.min.x = a.min.x + nx * aw * (1 - scale);
  b.max.x = b.min.x + aw * scale;

  b.min.y = a.min.y + ny * ah * (1 - scale);
  b.max.y = b.min.y + ah * scale;

  return b;
}

Viewport Viewport::from_origin_zoom(double fb_width, double fb_height, double ox, double oy, double zoom) {

  double w = fb_width / exp2(zoom);
  double h = fb_height / exp2(zoom);

  Viewport v;
  v.min.x = ox - w * 0.5;
  v.min.y = oy - h * 0.5;
  v.max.x = ox + w * 0.5;
  v.max.y = oy + h * 0.5;
  v.screen_size.x = fb_width;
  v.screen_size.y = fb_height;

  return v;
}

//-----------------------------------------------------------------------------

static struct Autotest {
  Autotest() {
    Viewport a;
    a.min = dvec2(0,0);
    a.max = dvec2(100,100);
    a.screen_size = dvec2(1920,1080);

    dvec2 s(1920,0);

    Viewport b = a.zoom_in_on(s, 0.5);

    printf("b.min.x %f\n", b.min.x);
    printf("b.min.y %f\n", b.min.y);
    printf("b.max.x %f\n", b.max.x);
    printf("b.max.y %f\n", b.max.y);
  }
} autotest;