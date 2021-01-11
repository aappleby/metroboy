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
  GHOST_REGION,
  PLACE_ANCHOR,
  PAN_VIEW,
  MENU_OPTION,
  SPLIT_CELL,
  MERGE_CELL,
};

//-----------------------------------------------------------------------------

class PlaitApp : public App {
public:
  PlaitApp();
  ~PlaitApp() override;

  void on_key_up();

  const char* app_get_title() override;
  void app_init(int screen_w, int screen_h) override;
  void app_close() override;

  void begin_frame(int screen_w, int screen_h) override;
  void app_update(double delta) override;
  void app_render_frame() override;
  void app_render_ui() override;

  typedef std::function<void(PlaitCell*)> NodeGroupCallback;
  typedef std::function<void(PlaitNode*)>      NodeCallback;

  void apply_region_node (dvec2 corner_a, dvec2 corner_b, NodeCallback callback);

  void select_region(dvec2 corner_a, dvec2 corner_b);
  void lock_region(dvec2 corner_a, dvec2 corner_b);
  void select_node(PlaitNode* node);

  void commit_selection();
  void revert_selection();
  void clear_selection();

  void paint_node(PlaitNode* node);
  PlaitNode* pick_node(dvec2 pos, bool ignore_selected, bool ignore_clicked, bool ignore_hovered);

  void  draw_node(PlaitNode* node);
  void  draw_edge(PlaitTrace* node);

  void event_select_tool  (SDL_Event event);
  void event_imgui        (SDL_Event event);
  void event_drag_nodes   (SDL_Event event);
  void event_select_region(SDL_Event event);
  void event_lock_region  (SDL_Event event);
  void event_unlock_region(SDL_Event event);
  void event_ghost_region (SDL_Event event);
  void event_split_cell   (SDL_Event event);
  void event_merge_cell   (SDL_Event event);
  void event_place_anchor (SDL_Event event);
  void event_pan_view     (SDL_Event event);
  void event_menu_option  (SDL_Event event);

  //----------------------------------------

  ViewController view_control;

  ToolMode current_tool = ToolMode::NONE;

  Plait plait;
  DieDB die_db;
  std::map<std::string, uint32_t> node_type_to_color;

  GridPainter grid_painter;
  LinePainter edge_painter;
  LinePainter outline_painter;
  TextPainter text_painter;
  BoxPainter  box_painter;
  BoxPainter  port_painter;
  Blitter     blitter;
  int tex = 0;

  uint32_t mouse_buttons = 0;
  dvec2 mouse_pos_screen = {0,0};
  dvec2 mouse_pos_world  = {0,0};
  dvec2 click_pos_screen = {0,0};
  dvec2 click_pos_world  = {0,0};

  int frame_count = 0;

  bool show_edges = true;
  bool show_anchors = true;

  //std::set<NodeGroup*> group_selection;
  std::set<PlaitNode*>      node_selection;

  PlaitNode*      clicked_node = nullptr;
  //NodeGroup* clicked_group = nullptr;
  dvec2      clicked_offset = {0,0};

  //NodeGroup* hovered_group = nullptr;
  PlaitNode*      hovered_node = nullptr;
};

//-----------------------------------------------------------------------------