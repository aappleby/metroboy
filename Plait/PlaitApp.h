#pragma once

#include "AppLib/App.h"
#include "AppLib/BoxPainter.h"
#include "AppLib/GridPainter.h"
#include "AppLib/LinePainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/Blitter.h"

#include "CoreLib/Debug.h"

#include "Plait/Plait.h"
#include "Plait/CellDB.h"

#include <functional>

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

  bool is_mouse_locked() const override;
  bool is_keyboard_locked() const override;

  typedef std::function<void(Node*)> NodeCallback;

  void apply_region(dvec2 corner_a, dvec2 corner_b, NodeCallback callback);

  void select_region(dvec2 corner_a, dvec2 corner_b);
  void lock_region(dvec2 corner_a, dvec2 corner_b);
  void select_node(Node* node);

  void commit_selection();
  void revert_selection();
  void clear_selection();

  Node* pick_node(dvec2 pos, bool ignore_selected, bool ignore_clicked, bool ignore_hovered);
  void  draw_node(Node* node);

  //----------------------------------------

  Plait plait;
  CellDB cell_db;
  std::map<std::string, uint32_t> node_type_to_color;

  GridPainter grid_painter;
  LinePainter edge_painter;
  LinePainter outline_painter;
  TextPainter text_painter;
  BoxPainter  box_painter;
  BoxPainter  port_painter;
  Blitter     blitter;
  int tex = 0;

  const uint8_t* keyboard_state = nullptr;
  int keyboard_count = 0;

  dvec2 click_start_screen = {0,0};
  dvec2 click_end_screen = {0,0};
  dvec2 mouse_pos_screen = {0,0};

  dvec2 click_start_world = {0,0};
  dvec2 click_end_world = {0,0};
  dvec2 mouse_pos_world = {0,0};

  bool show_edges = true;
  bool show_anchors = true;

  dvec2 sel_min = {0,0};
  dvec2 sel_max = {0,0};

  Node* clicked_node = nullptr;
  Node* hovered_node = nullptr;

  std::set<Node*> selection;

  std::vector<uint8_t> old_keys;
  std::vector<uint8_t> new_keys;
};

//-----------------------------------------------------------------------------