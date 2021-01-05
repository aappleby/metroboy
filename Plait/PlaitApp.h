#pragma once

#include "AppLib/App.h"
#include "AppLib/BoxPainter.h"
#include "AppLib/GridPainter.h"
#include "AppLib/LinePainter.h"
#include "AppLib/TextPainter.h"
#include "AppLib/Blitter.h"
#include "AppLib/Viewport.h"

#include "CoreLib/Debug.h"

#include "Plait/Plait.h"
#include "Plait/CellDB.h"

#include <functional>

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

enum class ToolMode {
  NONE,
  IMGUI,
  DRAG_NODE,
  SELECT_REGION,
  LOCK_REGION,
  UNLOCK_REGION,
  PLACE_ANCHOR,
  PAN_VIEW,
  MENU_OPTION,
  HIGHLIGHT_HOVER,
};

//-----------------------------------------------------------------------------

class PlaitApp : public App {
public:
  ~PlaitApp() override;

  void on_key_up();

  const char* app_get_title() override;
  void app_init(int screen_w, int screen_h) override;
  void app_close() override;

  void begin_frame(int screen_w, int screen_h) override;
  void app_update(double delta) override;
  void app_render_frame() override;
  void app_render_ui() override;

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

  bool event_to_tool(SDL_Event event);
  bool event_to_imgui(SDL_Event event);
  bool event_to_view_control(SDL_Event event);
  bool event_to_ui(SDL_Event event);

  bool on_mouse_down(uint32_t mouse_buttons_posedge);
  bool on_mouse_up(uint32_t mouse_buttons_negedge);

  //----------------------------------------

  ViewController view_control;

  ToolMode current_tool = ToolMode::NONE;

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

  dvec2 mouse_pos_screen = {0,0};
  dvec2 mouse_pos_world = {0,0};

  int frame_count = 0;

  //uint32_t mouse_buttons_old = 0;
  //uint32_t mouse_buttons_new = 0;
  //uint32_t mouse_buttons_posedge = 0;
  //uint32_t mouse_buttons_negedge = 0;

  //const uint8_t* keyboard_state = nullptr;
  //int keyboard_count = 0;

  //dvec2 click_start_screen = {0,0};
  //dvec2 click_end_screen = {0,0};

  //dvec2 click_start_world = {0,0};
  //dvec2 click_end_world = {0,0};

  bool show_edges = true;
  bool show_anchors = true;

  bool sel_active = false;
  dvec2 sel_begin = {0,0};
  dvec2 sel_end   = {0,0};
  std::set<Node*> selection;

  Node* clicked_node = nullptr;
  dvec2 clicked_offset = {0,0};
  dvec2 clicked_pos_abs = {0,0};

  Node* hovered_node = nullptr;

  dvec2 drag_begin = {0,0};
  dvec2 drag_end = {0,0};


  //std::vector<uint8_t> keys_old;
  //std::vector<uint8_t> keys_new;
  //std::vector<uint8_t> keys_posedge;
  //std::vector<uint8_t> keys_negedge;
};

//-----------------------------------------------------------------------------