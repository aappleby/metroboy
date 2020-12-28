#pragma once

#include "AppLib/App.h"
#include "AppLib/BoxPainter.h"
#include "AppLib/GridPainter.h"
#include "AppLib/LinePainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/Blitter.h"

#include "CoreLib/Debug.h"


//-----------------------------------------------------------------------------

class PlaitApp : public App {
public:
  ~PlaitApp() override;

  const char* app_get_title() override;
  void app_init() override;
  void app_close() override;
  void app_update(Viewport view, double delta) override;
  void app_render_frame(Viewport view) override;
  void app_render_ui(Viewport view) override;

  bool is_mouse_locked() const override { return mouse_locked; }

  //Node* pick_node(dvec2 pos);

  //----------------------------------------

  //Plait plait;

  GridPainter grid_painter;
  LinePainter edge_painter;
  LinePainter outline_painter;
  TextPainter text_painter;
  BoxPainter  box_painter;
  BoxPainter  port_painter;
  Blitter     blitter;
  int tex = 0;

  const uint8_t* keyboard_state = nullptr;

  dvec2 drag_start;
  dvec2 drag_end;
  dvec2 mouse_pos;
  bool mouse_locked;

  int mouse_buttons;
  bool dragging;
  //Node* hit_node;
};

//-----------------------------------------------------------------------------