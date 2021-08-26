#include "AppLib/Viewport.h"
#include <math.h>

//-----------------------------------------------------------------------------

double ema_coeff1(double fc, double fs) {
  double c = cos(2.0 * 3.14159265358979 * fc / fs);
  double a = c - 1 + sqrt(c * (c - 4) + 3);
  return a;
}

double ease(double a, double b, double delta) {
  if (a == b) return b;
  double t = 1.0 - pow(0.1, delta / 0.08);
  double c = a + (b - a) * t;
  return ((float)c == (float)a) ? b : c;
}

dvec2 ease(dvec2 a, dvec2 b, double delta) {
  return {
    ease(a.x, b.x, delta),
    ease(a.y, b.y, delta),
  };
}

Viewport ease(Viewport a, Viewport b, double delta) {
  return Viewport::from_min_max_screen(ease(a.world_min(), b.world_min(), delta),
                                       ease(a.world_max(), b.world_max(), delta),
                                       a.screen_size());
}

#pragma warning(disable : 4201)
union double_funtimes {
  double value;
  struct {
    uint64_t mantissa : 52;
    uint64_t exponent : 11;
    uint64_t sign     : 1;
  };
};

double ppw_to_zoom(double ppw) {
  double_funtimes f;
  f.value = ppw;
  return double(int(f.exponent) - 1023) + f.mantissa / exp2(52);
}

double zoom_to_ppw(double zoom) {
  double zoom_i = floor(zoom);
  double zoom_f = zoom - zoom_i;
  double_funtimes f;
  f.mantissa = uint64_t(zoom_f * exp2(52));
  f.exponent = uint64_t(zoom_i + 1023);
  f.sign = 0;
  return f.value;
}

//-----------------------------------------------------------------------------

dvec2 Viewport::worldToScreen(dvec2 v) const {
  return (v - world_center()) * scale_world_to_screen() + screen_center();
}

dvec2 Viewport::screenToWorld(dvec2 v) const {
  return (v - screen_center()) * scale_screen_to_world() + world_center();
}

dvec2 Viewport::deltaScreenToWorld(dvec2 delta) const {
  return delta * scale_screen_to_world();
}

//-----------------------------------------------------------------------------

Viewport Viewport::center_on(dvec2 c) {
  return Viewport::from_center_zoom_screen(c, 3.0, screen_size());
}

//-----------------------------------------------------------------------------

Viewport Viewport::zoom(dvec2 screen_pos, double zoom) {
  auto screen_delta = screen_pos - 0.5 * screen_size();

  auto new_center =
    world_center()
    + screen_delta / (zoom_to_ppw(view_zoom()))
    - screen_delta / (zoom_to_ppw(view_zoom() + zoom));

  auto new_zoom = view_zoom() + zoom;
  return Viewport::from_center_zoom_screen(new_center, new_zoom, screen_size());
}

//-----------------------------------------------------------------------------

Viewport Viewport::pan(dvec2 screen_delta) {
  dvec2 world_delta = screen_delta * scale_screen_to_world();
  return Viewport::from_center_zoom_screen(world_center() - world_delta, view_zoom(), screen_size());
}

//-----------------------------------------------------------------------------

Viewport Viewport::snap() {
  double zoom1 = view_zoom();
  double zoom2 = round(zoom1 * 4.0) / 4.0;
  double ppw2  = zoom_to_ppw(zoom2);

  dvec2 old_center = world_center();
  dvec2 new_center = { round(old_center.x * ppw2) / ppw2, round(old_center.y * ppw2) / ppw2 };

  return Viewport::from_center_zoom_screen(new_center, zoom2, screen_size());
}

//-----------------------------------------------------------------------------

Viewport Viewport::ease(Viewport target, double delta) {
  return ::ease(*this, target, delta);
}

//-----------------------------------------------------------------------------
