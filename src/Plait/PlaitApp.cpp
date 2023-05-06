#include "Plait/PlaitApp.h"

#include "metrolib/appbase/AppHost.h"
#include "metrolib/appbase/DummyApp.h"
#include "metrolib/appbase/GLBase.h"
#include "metrolib/appbase/GLBase.h"
#include "metrolib/core/Constants.h"
#include "metrolib/core/Utils.h"
#include "imgui/imgui.h"
#include "Plait/CellDB.h"

#include <algorithm>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#pragma warning(disable:4996)

using namespace std;

const dvec2 node_size = {64,64};
const dvec2 node_half_size = {32,32};
const dvec2 port_size = {4,4};

const double world_width  = 65536.0;
const double world_height = 16384.0;

const double half_world_width  = world_width / 2.0;
const double half_world_height = world_height / 2.0;

const int world_min = -1;
const int world_max = 1;

static std::map<ToolMode, std::string> tool_to_string = {
  {ToolMode::NONE,            "NONE"},

  {ToolMode::DRAG_NODE,       "DRAG_NODE"},
  {ToolMode::DRAG_LABEL,      "DRAG_LABEL"},

  {ToolMode::DRAG_FRAME,      "DRAG_FRAME"},

  {ToolMode::SELECT_REGION,   "SELECT_REGION"},
  {ToolMode::GHOST_REGION,    "GHOST_REGION"},

  {ToolMode::TOGGLE_OLD,      "TOGGLE_OLD"},
  {ToolMode::TOGGLE_GLOBAL,   "TOGGLE_GLOBAL"},

  {ToolMode::CREATE_ROOT,     "CREATE_ROOT"},
  {ToolMode::CREATE_LEAF,     "CREATE_LEAF"},
  {ToolMode::DELETE_NODE,     "DELETE_NODE"},
  {ToolMode::LINK_NODE,       "LINK_NODE"},

  {ToolMode::PAN_VIEW,        "PAN_VIEW"},
  {ToolMode::MENU_OPTION,     "MENU_OPTION"},
};

dvec2 wrap(dvec2 d) {
  while(d.x >  half_world_width) d.x -= world_width;
  while(d.x < -half_world_width) d.x += world_width;
  return d;
}

dvec2 wrap_towards(dvec2 origin, dvec2 d) {
  while((d.x - origin.x) >  half_world_width) d.x -= world_width;
  while((d.x - origin.x) < -half_world_width) d.x += world_width;
  return d;
}

//---------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  int ret = 0;

  PlaitApp* app = new PlaitApp();
  (void)app;

  LOG_B("Loading gameboy.die_db.json\n");
  app->die_db.clear();
  app->die_db.load_json("gameboy.die_db.json");

#if 0
  LOG_B("Parsing gateboy source\n");
  app->die_db.clear();
  app->die_db.parse_dir("src\\GateBoyLib");
  //app->die_db.save_json("gameboy.die_db.json");
#endif
  LOG_B("Total cells %d\n", (int)app->die_db.cell_map.size());
  LOG_B("Total edges %d\n", (int)app->die_db.traces.size());
  LOG_B("Done\n");


#if 1
  LOG_B("Loading gameboy.plait.json\n");
  nlohmann::json jroot;
  std::ifstream("gameboy.plait.json") >> jroot;
  app->plait.from_json(jroot, app->die_db);

  size_t total_nodes = 0;
  for (auto& [tag, plait_cell] : app->plait.cell_map) {
    total_nodes++;
    total_nodes += plait_cell->leaf_nodes.size();
  }
  LOG_B("Total cells %zd\n", app->plait.cell_map.size());
  LOG_B("Total nodes %zd\n", total_nodes);
  LOG_B("Total edges %zd\n", app->plait.traces.size());
  LOG_B("Done\n");
#endif

  AppHost* app_host = new AppHost(app);
  ret = app_host->app_main(argc, argv);
  delete app;

  return ret;
}

//---------------------------------------------------------------------------------------------------------------------

enum gate_pix_type {
  GP_BUF  = 0x00000000,
  GP_NOT  = 0x01000000,
  GP_AND  = 0x02000000,
  GP_NAND = 0x03000000,
  GP_OR   = 0x04000000,
  GP_NOR  = 0x05000000,
  GP_XOR  = 0x06000000,
  GP_XNOR = 0x07000000,

  GP_PIO  = 0x08000000,
  GP_POUT = 0x09000000,
  GP_PIN  = 0x0A000000,
  GP_ADD  = 0x0B000000,
  GP_NOL  = 0x0C000000,
  GP_NAL  = 0x0D000000,
  GP_OAN  = 0x0E000000,
  GP_AO   = 0x0F000000,

  GP_OA   = 0x10000000,
  GP_BUS  = 0x11000000,
  GP_TRI  = 0x12000000,
  GP_DFFP = 0x13000000,
  GP_DFFN = 0x14000000,
  GP_MUXP = 0x15000000,
  GP_MUXN = 0x16000000
};

PlaitApp::PlaitApp() {
  auto& n2c = node_type_to_color;

  n2c["not1"]  = COL_MID_GREY | GP_NOT;

  n2c["and2"]  = COL_PALE_RED | GP_AND;
  n2c["and3"]  = COL_PALE_RED | GP_AND;
  n2c["and4"]  = COL_PALE_RED | GP_AND;

  n2c["nand2"] = COL_PALE_RED | GP_NAND;
  n2c["nand3"] = COL_PALE_RED | GP_NAND;
  n2c["nand4"] = COL_PALE_RED | GP_NAND;
  n2c["nand5"] = COL_PALE_RED | GP_NAND;
  n2c["nand6"] = COL_PALE_RED | GP_NAND;
  n2c["nand7"] = COL_PALE_RED | GP_NAND;

  n2c["or2"]   = COL_PALE_BLUE | GP_OR;
  n2c["or3"]   = COL_PALE_BLUE | GP_OR;
  n2c["or4"]   = COL_PALE_BLUE | GP_OR;

  n2c["nor2"]  = COL_PALE_BLUE | GP_NOR;
  n2c["nor3"]  = COL_PALE_BLUE | GP_NOR;
  n2c["nor4"]  = COL_PALE_BLUE | GP_NOR;
  n2c["nor5"]  = COL_PALE_BLUE | GP_NOR;
  n2c["nor6"]  = COL_PALE_BLUE | GP_NOR;
  n2c["nor7"]  = COL_PALE_BLUE | GP_NOR;
  n2c["nor8"]  = COL_PALE_BLUE | GP_NOR;

  n2c["and_or3"]     = COL_MID_MAGENTA | GP_AO;
  n2c["or_and3"]     = COL_MID_MAGENTA | GP_OA;
  n2c["not_or_and3"] = COL_MID_MAGENTA | GP_OAN;

  n2c["xor2"]  = COL_MINT | GP_XOR;
  n2c["xnor2"] = COL_MINT | GP_XNOR;

  n2c["mux2n"] = COL_ROSE | GP_MUXN;
  n2c["mux2p"] = COL_ROSE | GP_MUXP;
  n2c["amux2"] = COL_ROSE | GP_MUXP;
  n2c["amux4"] = COL_ROSE | GP_MUXP;
}

PlaitApp::~PlaitApp() {
}

const char* PlaitApp::app_get_title() {
  return "PlaitApp";
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::paint_node(PlaitNode* node) {
  node->color = 0xFFFF00FF;

  auto cell_type = node->plait_cell->die_cell->cell_type;
  if (cell_type == DieCellType::PIN_IN)  node->color = COL_PALE_RED | GP_PIN;
  if (cell_type == DieCellType::PIN_OUT) node->color = COL_PALE_GREEN | GP_POUT;
  if (cell_type == DieCellType::PIN_IO)  node->color = COL_PALE_YELLOW | GP_PIO;

  if (cell_type == DieCellType::SIG_IN)  node->color = COL_MID_RED | GP_PIN;
  if (cell_type == DieCellType::SIG_OUT) node->color = COL_MID_GREEN | GP_POUT;

  if (cell_type == DieCellType::BUS)     node->color = COL_PALE_GREEN | GP_BUS;
  if (cell_type == DieCellType::DFF)     node->color = COL_PALE_BROWN | GP_DFFP;
  if (cell_type == DieCellType::LATCH)   node->color = COL_ORANGE | GP_NAL;
  if (cell_type == DieCellType::TRIBUF)  node->color = COL_MID_YELLOW | GP_TRI;
  if (cell_type == DieCellType::ADDER)   node->color = COL_MID_TEAL | GP_ADD;

  if (cell_type == DieCellType::LOGIC) {
    auto it = node_type_to_color.find(node->plait_cell->gate());
    if (it != node_type_to_color.end()) {
      node->color = (*it).second;
    }
    else {
      LOG_R("Could not pick a color for %s\n", node->plait_cell->gate());
    }
  }

  //node->color = 0x00FFFFFF;
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_init(int screen_w, int screen_h) {
  dvec2 screen_size(screen_w, screen_h);
  view_control.init(screen_size);

  box_painter.init();
  port_painter.init();
  grid_painter.init(world_width, world_height);
  edge_painter.init();
  outline_painter.init();
  text_painter.init();
  ui_text_painter.init();
  blitter.init();
  dump_painter.init_ascii();

  check_gl_error();

  for (auto& [tag, cell] : plait.cell_map) {
    paint_node(cell->core_node);
    for (auto& [name, root] : cell->root_nodes) {
      paint_node(root);
    }
    for (auto& [name, leaf] : cell->leaf_nodes) {
      paint_node(leaf);
    }
  }

  LOG_B("Init done %f\n", timestamp());
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_close() {
}

//---------------------------------------------------------------------------------------------------------------------

bool PlaitApp::hit_node(dvec2 _mouse_pos, PlaitNode* node) {
  dvec2 node_pos = node->get_pos_new();

  double min_x = node_pos.x;
  double min_y = node_pos.y;
  double max_x = node_pos.x + node_size.x;
  double max_y = node_pos.y + node_size.y;

  while(_mouse_pos.x - min_x >  half_world_width) _mouse_pos.x -= world_width;
  while(_mouse_pos.x - max_x < -half_world_width) _mouse_pos.x += world_width;

  if (_mouse_pos.x >= min_x &&
      _mouse_pos.y >= min_y &&
      _mouse_pos.x <= max_x &&
      _mouse_pos.y <= max_y) {
    return true;
  }
  else {
    return false;
  }
}

PlaitNode* PlaitApp::pick_node(dvec2 _mouse_pos_world) {
  for (auto& [tag, plait_cell] : plait.cell_map) {

    if (hit_node(_mouse_pos_world, plait_cell->core_node)) return plait_cell->core_node;
    for (auto& [name, root] : plait_cell->root_nodes) {
      if (hit_node(_mouse_pos_world, root)) return root;
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      if (hit_node(_mouse_pos_world, leaf)) return leaf;
    }
  }

  return nullptr;
}

PlaitLabel* PlaitApp::pick_label(dvec2 _mouse_pos) {
  for (auto label : plait.labels) {
    dvec2 min = label->pos_new;
    dvec2 max = label->pos_new + dvec2(label->text.size() * 6 * label->scale, 12 * label->scale);
    if (_mouse_pos.x >= min.x &&
        _mouse_pos.y >= min.y &&
        _mouse_pos.x <= max.x &&
        _mouse_pos.y <= max.y) {
      return label;
    }
  }
  return nullptr;
}

PlaitFrame* PlaitApp::pick_frame(dvec2 _mouse_pos) {
  for (auto frame : plait.frames) {
    dvec2 min = frame->pos;
    dvec2 glyph_size(6, 12);

    dvec2 max = min + dvec2(frame->size) * glyph_size * (double)frame->text_scale;
    if (_mouse_pos.x >= min.x &&
      _mouse_pos.y >= min.y &&
      _mouse_pos.x <= max.x &&
      _mouse_pos.y <= max.y) {
      return frame;
    }
  }
  return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------

bool PlaitApp::contains_node(dvec2 corner_a, dvec2 corner_b, PlaitNode* node) {
  dvec2 rmin = min(corner_a, corner_b);
  dvec2 rmax = max(corner_a, corner_b);

  dvec2 nmin = node->get_pos_new();
  dvec2 nmax = node->get_pos_new() + node_size;

  if (nmin.x < rmin.x) return false;
  if (nmin.y < rmin.y) return false;
  if (nmax.x > rmax.x) return false;
  if (nmax.y > rmax.y) return false;
  return true;
}

int PlaitApp::apply_region_node(dvec2 corner_a, dvec2 corner_b, NodeCallback callback) {
  int hit = 0;
  for (auto& [tag, plait_cell] : plait.cell_map) {
    if (contains_node(corner_a, corner_b, plait_cell->core_node)) {
      callback(plait_cell->core_node);
      hit++;
    }
    for (auto& [name, root] : plait_cell->root_nodes) {
      if (contains_node(corner_a, corner_b, root)) {
        callback(root);
        hit++;
      }
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      if (contains_node(corner_a, corner_b, leaf)) {
        callback(leaf);
        hit++;
      }
    }
  }

  // If we just clicked a thing instead of click-drag-selecting, fire the callback for only
  // the clicked node.
  if (hit == 0) {
    if (clicked_node) {
      callback(clicked_node);
    }
  }

  return hit;
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::select_region(dvec2 corner_a, dvec2 corner_b) {
  //LOG_B("Selection region ");
  auto callback = [this](PlaitNode* node) {
    //LOG_B("%s ", node->plait_cell->name());
    node->select();
    node_selection.insert(node);
  };

  apply_region_node(corner_a, corner_b, callback);
  //LOG_B("\n");
}

void PlaitApp::select_node(PlaitNode* node) {
  //LOG_B("Selecting %s\n", node->plait_cell->name());
  node->select();
  node_selection.insert(node);
}

void PlaitApp::commit_selection() {
  if (node_selection.empty()) return;

  //printf("Committing ");
  for (auto node : node_selection) {
    //LOG_B("%s ", node->plait_cell->name());
    node->commit_pos();
  }
  //LOG_B("\n");
}

void PlaitApp::revert_selection() {
  if (node_selection.empty()) return;

  //LOG_B("Reverting ");
  for (auto node : node_selection) {
    //LOG_B("%s ", node->plait_cell->name());
    node->revert_pos();
  }
  //LOG_B("\n");
}

void PlaitApp::clear_selection() {
  if (node_selection.empty()) return;

  //LOG_B("Unselecting ");
  for (auto node : node_selection) {
    //LOG_B("%s ", node->plait_cell->name());
    node->deselect();
  }
  node_selection.clear();
  //LOG_B("\n");
}

//---------------------------------------------------------------------------------------------------------------------

double remap(double x, double a1, double a2, double b1, double b2) {
  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

double remap_clamp(double x, double a1, double a2, double b1, double b2) {
  if (x < a1) x = a1;
  if (x > a2) x = a2;

  x = (x - a1) / (a2 - a1);
  x = x * (b2 - b1) + b1;
  return x;
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::event_menu_option(SDL_Event event) {
  switch(event.type) {
  case SDL_KEYDOWN: {
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_S) {
      const char* filename = "gameboy.plait.json";
      LOG_B("Saving plait %s\n", filename);
      commit_selection();
      clear_selection();

      nlohmann::json jroot;
      plait.to_json(jroot);
      std::ofstream(filename) << jroot.dump(2);
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_LALT) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_select_region(SDL_Event event) {
  bool was_drag = length(click_pos_screen - mouse_pos_screen) > 3;

  switch(event.type) {
  case SDL_MOUSEBUTTONDOWN:
    if (clicked_node) {
      select_node(clicked_node);
    }
    break;
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      if (was_drag) {
        commit_selection();
        clear_selection();
        select_region(click_pos_wrap, mouse_pos_world);
      }
      else {
        if (clicked_node) {
          select_node(clicked_node);
        }
      }
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_LCTRL) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_ghost_region(SDL_Event event) {
  switch(event.type) {
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      auto callback = [this](PlaitNode* node) {
        node->toggle_ghosted();
      };
      apply_region_node(click_pos_wrap, mouse_pos_world, callback);
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_toggle_old(SDL_Event event) {
  switch (event.type) {
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      auto callback = [this](PlaitNode* node) {
        node->toggle_old();
      };
      apply_region_node(click_pos_wrap, mouse_pos_world, callback);
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_F) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_toggle_global(SDL_Event event) {
  switch (event.type) {
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      auto callback = [this](PlaitNode* node) {
        node->toggle_global();
      };
      apply_region_node(click_pos_wrap, mouse_pos_world, callback);
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_G) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_pan_view(SDL_Event event) {
  bool was_drag = length(click_pos_screen - mouse_pos_screen) > 3;

  switch(event.type) {
  case SDL_MOUSEMOTION: {
    if (event.motion.state & SDL_BUTTON_LMASK) {
      view_control.pan(dvec2(event.motion.xrel, event.motion.yrel));
    }
    break;
  }
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      if (!was_drag) {
        commit_selection();
        clear_selection();
      }
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_drag_nodes(SDL_Event event) {
  switch(event.type) {
  case SDL_MOUSEMOTION: {
    if (event.motion.state & SDL_BUTTON_LMASK) {
      dvec2 pos_abs_new = (mouse_pos_world - click_pos_world) + click_pos_wrap + clicked_node_offset;
      pos_abs_new.x = round(pos_abs_new.x / 96) * 96.0;
      pos_abs_new.y = round(pos_abs_new.y / 48) * 48.0;
      dvec2 delta = pos_abs_new - clicked_node->get_pos_new();
      for (auto node : node_selection) node->move(delta);
    }
    break;
  }
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_drag_label(SDL_Event event) {
  switch(event.type) {
  case SDL_MOUSEMOTION: {
    if (event.motion.state & SDL_BUTTON_LMASK) {
      dvec2 pos_abs_new = mouse_pos_wrap + clicked_label_offset;
      pos_abs_new.x = round(pos_abs_new.x / 16) * 16.0;
      pos_abs_new.y = round(pos_abs_new.y / 16) * 16.0;
      clicked_label->pos_old = pos_abs_new;
      clicked_label->pos_new = pos_abs_new;
    }
    break;
  }
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_drag_frame(SDL_Event event) {
  assert(clicked_frame);

  switch (event.type) {
  case SDL_MOUSEMOTION: {
    if (event.motion.state & SDL_BUTTON_LMASK) {
      dvec2 pos_abs_new = mouse_pos_wrap + clicked_frame_offset;
      pos_abs_new.x = round(pos_abs_new.x / 16) * 16.0;
      pos_abs_new.y = round(pos_abs_new.y / 16) * 16.0;
      clicked_frame->pos = pos_abs_new;
    }
    break;
  }
  case SDL_KEYDOWN: {
    break;
  }

  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_imgui(SDL_Event event) {
  ImGuiIO& io = ImGui::GetIO();

  switch (event.type) {
  case SDL_TEXTINPUT: {
    io.AddInputCharactersUTF8(event.text.text);
    break;
  }
  case SDL_KEYDOWN:
  case SDL_KEYUP: {
    int key = event.key.keysym.scancode;
    IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
    io.KeysDown[key] = (event.type == SDL_KEYDOWN);
    io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
    io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
    io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
    io.KeySuper = false;
    break;
  }
  case SDL_MOUSEWHEEL: {
    io.MouseWheelH += event.wheel.x;
    io.MouseWheel += event.wheel.y;
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_create_root(SDL_Event event) {
  switch(event.type) {
  case SDL_MOUSEBUTTONDOWN: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      commit_selection();
      clear_selection();
      if (clicked_node) {
        plait.spawn_root_node(clicked_node);
        clicked_node = nullptr;
      }
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_Z) {
      release_tool();
    }
    break;
  }
  }
}


void PlaitApp::event_create_leaf(SDL_Event event) {
  switch(event.type) {
  case SDL_MOUSEBUTTONDOWN: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      commit_selection();
      clear_selection();
      if (clicked_node) {
        plait.spawn_leaf_node(clicked_node);
        clicked_node = nullptr;
      }
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_X) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_link_node(SDL_Event event) {
  switch(event.type) {
  case SDL_MOUSEBUTTONDOWN: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      if (clicked_node) {
        for (auto leaf_node : node_selection) {
          plait.link_nodes(leaf_node, clicked_node);
        }
      }
      clicked_node = nullptr;
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_C) {
      release_tool();
    }
    break;
  }
  }
}

//--------------------------------------------------------------------------------

void PlaitApp::event_delete_node(SDL_Event event) {
  switch(event.type) {
  case SDL_MOUSEBUTTONDOWN: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      commit_selection();
      clear_selection();
      if (clicked_node) {
        if (clicked_node->name != "core") {
          plait.delete_node(clicked_node);
        }
        clicked_node = nullptr;
      }
    }
    break;
  }
  case SDL_KEYUP: {
    if (event.key.keysym.scancode == SDL_SCANCODE_V) {
      release_tool();
    }
    break;
  }
  }
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::event_select_tool(SDL_Event event) {
  if (event.type == SDL_KEYDOWN) {
    int key = event.key.keysym.scancode;
    ToolMode new_tool = ToolMode::NONE;

    if (key == SDL_SCANCODE_E) show_edges = !show_edges;

    if (key == SDL_SCANCODE_F)     new_tool = ToolMode::TOGGLE_OLD;
    if (key == SDL_SCANCODE_G)     new_tool = ToolMode::TOGGLE_GLOBAL;
    if (key == SDL_SCANCODE_Q)     new_tool = ToolMode::GHOST_REGION;
    if (key == SDL_SCANCODE_Z)     new_tool = ToolMode::CREATE_ROOT;
    if (key == SDL_SCANCODE_X)     new_tool = ToolMode::CREATE_LEAF;
    if (key == SDL_SCANCODE_C)     new_tool = ToolMode::LINK_NODE;
    if (key == SDL_SCANCODE_V)     new_tool = ToolMode::DELETE_NODE;
    if (key == SDL_SCANCODE_LALT)  new_tool = ToolMode::MENU_OPTION;
    if (key == SDL_SCANCODE_LCTRL) new_tool = ToolMode::SELECT_REGION;

    if (key == SDL_SCANCODE_ESCAPE && node_selection.size()) {
      //LOG_B("Reverting selection\n");
      revert_selection();
      clear_selection();
    }

    if (key == SDL_SCANCODE_RETURN && node_selection.size()) {
      //LOG_B("Commiting selection\n");
      commit_selection();
      clear_selection();
    }

    if (new_tool != ToolMode::NONE) {
      current_tool = new_tool;
    }
  }
  else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button & SDL_BUTTON_LMASK) {
    auto new_clicked_label = pick_label(mouse_pos_world);

    if (clicked_node) {
      clicked_node_offset = wrap(clicked_node->get_pos_new() - mouse_pos_world);

      if (!clicked_node->selected()) {
        commit_selection();
        clear_selection();
        select_node(clicked_node);
      }
      current_tool = ToolMode::DRAG_NODE;
    }
    else if (clicked_frame) {
      if (event.button.clicks == 2) {
        selected_frame = clicked_frame;
      }
      else {
        clicked_frame_offset = wrap(clicked_frame->pos - mouse_pos_world);
        current_tool = ToolMode::DRAG_FRAME;
      }
    }
    else if (new_clicked_label) {
      clicked_label = new_clicked_label;
      clicked_label_offset = clicked_label->pos_new - mouse_pos_world;
      current_tool = ToolMode::DRAG_LABEL;
    }
    else {
      clicked_label = nullptr;
      current_tool = ToolMode::PAN_VIEW;
    }
  }
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_update(dvec2 screen_size, double delta_time) {
  (void)delta_time;

  auto& view = view_control.view_smooth_snap;

  time_delta = delta_time;

  double time_start = timestamp();

  int mouse_x = 0, mouse_y = 0;
  mouse_buttons    = SDL_GetMouseState(&mouse_x, &mouse_y);
  mouse_pos_screen = {mouse_x, mouse_y};
  mouse_pos_world  = view.screen_to_world(mouse_pos_screen, screen_size);
  mouse_pos_wrap = mouse_pos_world;
  while(mouse_pos_wrap.x >  half_world_width) mouse_pos_wrap.x -= world_width;
  while(mouse_pos_wrap.x < -half_world_width) mouse_pos_wrap.x += world_width;

  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
  io.MouseDown[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
  io.MouseDown[2] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  io.MousePos = { (float)mouse_pos_screen.x, (float)mouse_pos_screen.y };
  bool imgui_override = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow) || io.WantCaptureKeyboard || io.WantCaptureMouse;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    // If ImGui wants input, it gets all the SDL events.
    if (imgui_override) {
      event_imgui(event);
      continue;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button & SDL_BUTTON_LMASK)) {
      clicked_frame = pick_frame(mouse_pos_world);
      clicked_node = pick_node(mouse_pos_world);
      click_pos_screen = mouse_pos_screen;
      click_pos_world  = mouse_pos_world;
      click_pos_wrap   = mouse_pos_wrap;
    }

    if (event.type == SDL_KEYDOWN) {
      int key = event.key.keysym.scancode;

      if (key == SDL_SCANCODE_DELETE) {
        if (clicked_label) {
          plait.labels.erase(std::find(plait.labels.begin(), plait.labels.end(), clicked_label));
          delete clicked_label;
          clicked_label = nullptr;
        }
      }

      if (key == SDL_SCANCODE_F1) {
        show_help = !show_help;
      }
    }


    switch(current_tool) {
    case ToolMode::NONE:           event_select_tool(event); break;
    case ToolMode::DRAG_NODE:      event_drag_nodes(event); break;
    case ToolMode::DRAG_LABEL:     event_drag_label(event); break;
    case ToolMode::DRAG_FRAME:     event_drag_frame(event); break;
    case ToolMode::SELECT_REGION:  event_select_region(event); break;
    case ToolMode::GHOST_REGION:   event_ghost_region(event); break;
    case ToolMode::TOGGLE_OLD:     event_toggle_old(event); break;
    case ToolMode::TOGGLE_GLOBAL:  event_toggle_global(event); break;
    case ToolMode::CREATE_ROOT:    event_create_root(event); break;
    case ToolMode::CREATE_LEAF:    event_create_leaf(event); break;
    case ToolMode::LINK_NODE:      event_link_node(event); break;
    case ToolMode::DELETE_NODE:    event_delete_node(event); break;
    case ToolMode::PAN_VIEW:       event_pan_view(event); break;
    case ToolMode::MENU_OPTION:    event_menu_option(event); break;
    default: {
      LOG_R("Bad tool!\n");
      debugbreak();
      break;
    }
    }

    if(event.type == SDL_MOUSEWHEEL) {
      view_control.on_mouse_wheel(mouse_pos_screen, screen_size, double(event.wheel.y) * 0.25);
    }

    if (event.type == SDL_MOUSEBUTTONUP && (event.button.button & SDL_BUTTON_LMASK)) {
      clicked_frame = nullptr;
      clicked_node = nullptr;
    }
  }

  if (imgui_override) {
    hovered_node = nullptr;
    hovered_frame = nullptr;
  }
  else {
    hovered_node = pick_node(mouse_pos_world);
    hovered_frame = pick_frame(mouse_pos_world);
  }

  view_control.update(delta_time);

  //----------------------------------------
  // Pull nodes towards their parents/children

#if 0
  for (auto& [tag, node] : plait.node_map) {
    for (auto next : node->next) {
      spring_nodes(node, next);
    }

    if (node->get_cell() && node->get_cell()->cell_type == CellType::BUS) {
      if (node->prev.size() >= 2) {
        for (size_t i = 0; i < node->prev.size() - 1; i++) {
          spring_nodes2(node->prev[i + 0], node->prev[i + 1]);
        }
      }
    }
    else {
      if (node->next.size() >= 2) {
        for (size_t i = 0; i < node->next.size() - 1; i++) {
          spring_nodes2(node->next[i + 0], node->next[i + 1]);
        }
      }
    }
  }
#endif

#if 0
  // Push nodes away from their siblings
  for (auto node : plait.nodes) {
    if (node->prev.size() >= 2) {
      for (size_t i = 0; i < node->prev.size() - 1; i++) {
        Node* a = node->prev[i + 0];
        Node* b = node->prev[i + 1];

        if (a->selected) continue;
        if (a->locked) continue;
        if (a->anchored()) continue;

        dvec2 offset = a->get_pos_new() - b->get_pos_new();
        if (length(offset) < 256) {
          a->set_pos_new(a->get_pos_old() + offset * 0.01);
          a->commit_pos();
        }
      }
    }

    if (node->next.size() >= 2) {
      for (size_t i = 0; i < node->next.size() - 1; i++) {
        Node* a = node->next[i + 0];
        Node* b = node->next[i + 1];

        if (a->selected) continue;
        if (a->locked) continue;
        if (a->anchored()) continue;

        dvec2 offset = a->get_pos_new() - b->get_pos_new();
        if (length(offset) < 256) {
          a->set_pos_new(a->get_pos_old() + offset * 0.01);
          a->commit_pos();
        }
      }
    }
  }
#endif

  //----------------------------------------
  // Apply accumulated spring forces

#if 0
  for (auto& [tag, node] : plait.node_map) {
    if (node->selected || node->locked || node->anchored()) {
    }
    else {
      node->move(node->spring_force * 0.01);
      node->commit_pos();
    }
    node->spring_force = {0,0};
  }
#endif

  time_update = timestamp() - time_start;
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::draw_node_fill(PlaitNode* node, uint32_t color) {
  if (!node->ghosted) {
    /*
    bool bad_node = false;

    if (node->is_leaf() && node->plait_cell->die_cell->fanout == 1 && !node->old) {
      bad_node = true;
    }
    if (node->is_root() && node->plait_cell->die_cell->input_ports.size() == 1) {
      bad_node = true;
    }

    if (bad_node) {
      bad_node_count++;
      color = (frame_count & 16) ? 0xFFFF00FF : 0xFF00FFFF;
    }
    */

    if (node->old) {
      color = (color & 0xFF000000) | ((color & 0x00FEFEFE) >> 1);
    }
    box_painter.push_corner_size(node->get_pos_new(), node_size, color);
  }
}

//----------------------------------------

void PlaitApp::draw_node_ports(PlaitNode* node) {

  dvec2 node_pos_new = node->get_pos_new();

  // Node input port(s)

  size_t prev_port_count = node->plait_cell->die_cell->input_ports.size();
  double prev_stride = (node_size.y) / (prev_port_count + 1);

  for (size_t i = 0; i < prev_port_count; i++) {
    dvec2 port_pos = node_pos_new + dvec2(2, prev_stride * (i + 1));
    port_painter.push_center_size(port_pos, port_size, 0x00008000);
  }

  // Node output port(s)

  size_t next_port_count = node->plait_cell->die_cell->output_ports.size();
  double next_stride = (node_size.y) / (next_port_count + 1);

  for (size_t i = 0; i < next_port_count; i++) {
    dvec2 port_pos = node_pos_new + dvec2(node_size.x - 2, next_stride * (i + 1));
    port_painter.push_center_size(port_pos, port_size, 0x00000080);
  }
}

//----------------------------------------

void PlaitApp::draw_node_text(PlaitNode* node) {
  dvec2 node_pos_new = node->get_pos_new();
  text_painter.add_text_at_simple(node->plait_cell->name(), float(node_pos_new.x), float(node_pos_new.y));
  //text_painter.add_text_at_simple(node->plait_cell->gate(), float(node_pos_new.x + 8), float(node_pos_new.y + 24));
  //sprintf_s(buf, 256, "%d : %d", (int)node->plait_cell->root_nodes.size(), (int)node->plait_cell->leaf_nodes.size());
  //text_painter.add_text_at(buf, float(node_pos_new.x + 8), float(node_pos_new.y + 40));
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::draw_edge(PlaitTrace* edge) {
  auto output_node = edge->output_node;
  auto input_node  = edge->input_node;

  //if (!output_node->visible && !input_node->visible) return;

  if (output_node->ghosted || input_node->ghosted) return;

  auto output_pos_new = output_node->get_pos_new();
  auto input_pos_new  = input_node->get_pos_new();

  //auto delta = wrap(input_pos_new - output_pos_new);
  auto delta = input_pos_new - output_pos_new;
  input_pos_new = output_pos_new + delta;


  // Highlight "backwards" edges in red.
  bool edge_backwards = delta.x < 0;
  uint32_t output_color = edge_backwards ? 0xFF0000FF : 0x8044FF44;
  uint32_t input_color  = edge_backwards ? 0xFF0000FF : 0x8044FF44;

  // Make edges connected to selected nodes opaque.
  if (output_node->plait_cell->selected() || input_node->plait_cell->selected()) {
    if (edge_backwards) {
      output_color = 0xFF8080FF;
      input_color = 0xFF8080FF;
    }
    else {
      output_color |= 0xFF000000;
      input_color |= 0xFF000000;
    }
  }

  // Old nodes can drive other old nodes and DFF D ports.
  // New nodes can only drive other new nodes, except DFF D ports.

  auto& cell_type = edge->input_node->plait_cell->die_cell->cell_type;
  auto& input_port_name = edge->input_node->plait_cell->die_cell->input_ports[edge->input_port_index];

  bool input_is_logic = (cell_type == DieCellType::ADDER) || (cell_type == DieCellType::LOGIC);
  bool input_is_dff   = cell_type == DieCellType::DFF;
  bool input_is_dff_d = input_is_dff && (input_port_name[0] == 'D');

  bool bad_edge = false;

  // Old node -> old node = input must be a logic cell
  if ( edge->output_node->old &&  edge->input_node->old) bad_edge = !input_is_logic;

  // Old node -> new node = input must be dff d port
  if ( edge->output_node->old && !edge->input_node->old) bad_edge = !input_is_dff_d;

  // New node -> old node = always bad
  if (!edge->output_node->old &&  edge->input_node->old) bad_edge = true;

  // New node -> new node = input must _not_ be dff d port
  if (!edge->output_node->old && !edge->input_node->old) bad_edge = input_is_dff_d;

  if (bad_edge) {
    input_color  = (frame_count & 16) ? 0xFFFF00FF : 0xFF00FFFF;
    output_color = (frame_count & 16) ? 0xFFFF00FF : 0xFF00FFFF;
    bad_edge_count++;
  }


  size_t output_port_count = output_node->plait_cell->die_cell->output_ports.size();
  size_t input_port_count  = input_node->plait_cell->die_cell->input_ports.size();

  double output_stride = (node_size.y) / (output_port_count + 1);
  double input_stride  = (node_size.y) / (input_port_count + 1);

  dvec2 output_port_pos = output_pos_new + dvec2(node_size.x, output_stride * (edge->output_port_index + 1));
  dvec2 input_port_pos  = input_pos_new  + dvec2(0,           input_stride * (edge->input_port_index + 1));


  edge_painter.push(output_port_pos,
                    output_color,
                    input_port_pos,
                    input_color);
}

//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_frame(dvec2 screen_size, double delta) {
  (void)delta;

  auto& view = view_control.view_smooth_snap;

  double time_start = timestamp();

  bad_node_count = 0;
  bad_edge_count = 0;
  bad_branch_count = 0;

  //----------------------------------------
  // Grid layer

  grid_painter.render(view, screen_size);

  //----------------------------------------
  // Text blocks

  for (auto f : plait.frames) {
    const char* cursor = f->text.c_str();

    auto grid_bytes = f->size.x * f->size.y;
    text_grid.resize(grid_bytes);
    memset(text_grid.data(), 0, grid_bytes);

    int cursor_x2 = 0;
    int cursor_y2 = 0;
    while (*cursor) {
      char c = *cursor++;
      if (c == '\n') {
        cursor_x2 = 0;
        cursor_y2++;
      }
      else if (c == '\t') {
        while (cursor_x2 % 2) cursor_x2++;
      }
      else {
        if (cursor_x2 < f->size.x && cursor_y2 < f->size.y) {
          text_grid[cursor_x2 + cursor_y2 * f->size.x] = c;
        }
        cursor_x2++;
      }
    }
    dump_painter.dump(view, screen_size, f->pos.x, f->pos.y, f->text_scale, f->text_scale, f->size.x, f->size.y, f->color, (uint8_t*)text_grid.data());
  }

  //----------------------------------------
  // Visibility

  /*
  for (auto& [tag, plait_cell] : plait.cell_map) {
    plait_cell->core_node->update_visibility(view);
    for (auto& [name, root] : plait_cell->root_nodes) {
      root->update_visibility(view);
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      leaf->update_visibility(view);
    }
  }
  */

  //----------------------------------------
  // Node layer

  if (node_selection.size()) {
    // Selected node shadows
    for (auto node: node_selection) {
      box_painter.push_corner_size(node->get_pos_old(), node_size, 0xFF000000);
    }

    // Selected node highlights
    for (auto node: node_selection) {
      box_painter.push_corner_size(node->get_pos_new() - dvec2(6,6), node_size + dvec2(12,12), 0xFFFFFFFF);
    }
  }
  box_painter.render(view, screen_size, 0, 0, 1);

  //----------
  // Node fills

  for (auto& [tag, plait_cell] : plait.cell_map) {
    auto core = plait_cell->core_node;
    uint32_t color = core->color;

    if (core->is_global()) {
      box_painter.push_corner_size(core->get_pos_new() - dvec2(3, 3), node_size + dvec2(6, 6), 0xFF00FFFF);
      for (auto& [name, root] : plait_cell->root_nodes) {
        box_painter.push_corner_size(root->get_pos_new() - dvec2(3, 3), node_size + dvec2(6, 6), 0xFF00BBFF);
      }
      for (auto& [name, leaf] : plait_cell->leaf_nodes) {
        box_painter.push_corner_size(leaf->get_pos_new() - dvec2(3, 3), node_size + dvec2(6, 6), 0xFF00FF88);
      }
    }

    draw_node_fill(plait_cell->core_node, color);
    draw_node_text(plait_cell->core_node);
    draw_node_ports(plait_cell->core_node);
    for (auto& [name, root] : plait_cell->root_nodes) {
      draw_node_fill(root, color);
      draw_node_text(root);
      draw_node_ports(root);
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      draw_node_fill(leaf, color);
      draw_node_text(leaf);
      draw_node_ports(leaf);
    }
  }

  if (show_edges) {
    //for (auto& [trace_key, trace] : plait.trace_map_old) {
    for (auto& plait_trace : plait.traces) {
      draw_edge(plait_trace);
    }
  }

  box_painter.render(view, screen_size, 0, 0, 1);
  port_painter.render(view, screen_size, 0, 0, 1);
  text_painter.render(view, screen_size, 0, 0);
  edge_painter.render(view, screen_size, 0, 0, 1);

  //----------

  // Branches

  for (auto& [tag, plait_cell] : plait.cell_map) {
    auto core = plait_cell->core_node;
    auto die_cell = plait_cell->die_cell;

    auto core_pos = core->pos_new;

    bool no_branches = false;
    //no_branches |= die_cell->cell_type == DieCellType::BUS;
    //no_branches |= die_cell->cell_type == DieCellType::SIG_IN;
    //no_branches |= die_cell->cell_type == DieCellType::SIG_OUT;
    //no_branches |= die_cell->cell_type == DieCellType::PIN_IN;
    //no_branches |= die_cell->cell_type == DieCellType::PIN_OUT;
    //no_branches |= die_cell->cell_type == DieCellType::PIN_IO;
    no_branches |= core->is_global();

    if (hovered_node && hovered_node->plait_cell == plait_cell) no_branches = false;

    bool wrap_ok = false;
    if (die_cell->cell_type == DieCellType::BUS)    wrap_ok = true;
    if (die_cell->cell_type == DieCellType::PIN_IO) wrap_ok = true;

    double max_root_x = -1e10;
    double min_leaf_x =  1e10;

    for (auto& [name, root] : plait_cell->root_nodes) {
      if (root->pos_new.x > max_root_x) max_root_x = root->pos_new.x;
    }

    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      if (leaf->pos_new.x < min_leaf_x) min_leaf_x = leaf->pos_new.x;
    }

    // Draw all branches for selected nodes that aren't buses
    if (!no_branches) {
      uint32_t color = plait_cell->selected_node_count ? 0xFFFFFF00 : 0x60FFFF00;
      if (hovered_node && hovered_node->plait_cell == plait_cell) color = 0xFFFFFF00;

      for (auto& [name, root] : plait_cell->root_nodes) {
        if (root->ghosted) continue;
        if (root->old && !plait_cell->selected_node_count && (root != hovered_node)) continue;
        auto root_pos = root->pos_new;
        edge_painter.push(core_pos + node_half_size, color, root_pos + node_half_size, color);
      }
      for (auto& [name, leaf] : plait_cell->leaf_nodes) {
        if (leaf->ghosted) continue;
        if (leaf->old && !plait_cell->selected_node_count && (leaf != hovered_node)) continue;
        auto leaf_pos = leaf->pos_new;
        edge_painter.push(core_pos + node_half_size, color, leaf_pos + node_half_size, color);
      }
    }


    // Draw bad root node branches
    for (auto& [name, root] : plait_cell->root_nodes) {
      if (root->ghosted) continue;
      auto root_pos = root->pos_new;
      bool bad_branch = false;
      if (root_pos.x > min_leaf_x) bad_branch = true;
      if (!plait_cell->core_node->old && root_pos.x > core_pos.x) bad_branch = true;
      if (wrap_ok) bad_branch = false;
      if (root->old) bad_branch = false;
      if (!bad_branch) continue;
      bad_branch_count++;
      edge_painter.push(core_pos + node_half_size, 0x8000FFFF, root_pos + node_half_size, 0x8000FFFF);
    }

    // Draw bad leaf node branches
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      if (leaf->ghosted) continue;
      auto leaf_pos = leaf->pos_new;
      bool bad_branch = false;
      if (leaf_pos.x < max_root_x) bad_branch = true;
      if (!plait_cell->core_node->old && leaf_pos.x < core_pos.x) bad_branch = true;
      if (wrap_ok) bad_branch = false;
      if (leaf->old) bad_branch = false;
      if (!bad_branch) continue;

      bad_branch_count++;
      edge_painter.push(core_pos + node_half_size, 0x8000FFFF, leaf_pos + node_half_size, 0x8000FFFF);
    }
  }

  edge_painter.render(view, screen_size, 0, 0, 1);


  //----------------------------------------
  // Labels

  for (auto label : plait.labels) {
    uint32_t color = (label == clicked_label) ? COL_VIOLET : COL_PALE_GREY;
    color |= 0xFF000000;
    text_painter.add_text_at(label->text.c_str(), color, label->pos_new.x, label->pos_new.y, label->scale);
  }
  text_painter.render(view, screen_size, 0, 0);

  //----------------------------------------
  // Selection rect

  if (mouse_buttons & SDL_BUTTON_LMASK) {
    uint32_t sel_color = 0x00000000;
    if (current_tool == ToolMode::SELECT_REGION) sel_color = 0xFFFFFFFF;
    if (current_tool == ToolMode::GHOST_REGION)  sel_color = 0xFFFF0080;
    if (current_tool == ToolMode::TOGGLE_GLOBAL) sel_color = 0xFF00FFFF;
    if (current_tool == ToolMode::TOGGLE_OLD)    sel_color = 0xFF888888;

    if (sel_color) {
      outline_painter.push_box(click_pos_wrap, mouse_pos_wrap, sel_color);
      outline_painter.render(view, screen_size, 0, 0, 1);
    }
  }

  //----------------------------------------
  // Done

  frame_count++;

  time_render = timestamp() - time_start;
}

//---------------------------------------------------------------------------------------------------------------------

static const char* help_text = R"(========== Legend ==========

Graph edges and "root" nodes are generated from the parsed source and cannot be created or destroyed.
Root nodes can be cloned to move their input/output edges elsewhere in the graph.

Logic cells:
  Blue    : OR/NOR
  Pink    : AND/NAND
  Green   : XOR/XNOR
  Teal    : 1-Bit Adders
  Grey    : NOT
  Magenta : Mux, And+Or, Or+And, Nor+And

Storage cells:
  Orange  : Latches
  Brown   : Flip-flops

Communication cells:
  Red     : Input signals (from the CPU)
  Pink    : Input Pins
  Yellow  : Tri-state bus drivers, IO pins
  Green   : Buses, Output pins, Output signals

Node modifiers:
  No highlight     : Local node
  Orange highlight : Input clone of a global node
  Yellow highlight : Global root node.
  Green highlight  : Output clone of a global node.
  Dimmed           : An "old" signal from the previous cycle
  Solid black      : Hidden "ghost" node.

Edge colors:
  Green  : Links between local nodes.
  Teal   : Links between local clones.
  Yellow : Node clones are "backwards"
  Red    : Node edges are "backwards"
  Blink  : Invalid connection between "old" and "new" node

Flip-flop node input ports placement:
  | Clock              Non-inverting output |
  | Set (optional)                          |
  | Reset (optional)   Inverting output     |
  | Data                                    |

Flip-flop data input ports _must_ be connected to an _old_ node.

========== Controls ==========

Click+drag the background grid to pan the view, mousewheel to zoom the view

Hold a key and click or click-drag to:
  No key : Move selected nodes or text boxes
  Control: Select nodes
  F      : Toggle "old" state
  G      : Toggle "global" state
  Q      : Toggle "ghost" state

Hold a key and click a node to:
  Z : Create a new "input" clone
  X : Create a new "output" clone
  C : Link all selected nodes to it if possible
  V : Delete it.

Other keys:
  E      : Toggle edge visibility
  Alt+S  : Save graph
  Escape : Unselect nodes and return them to their previous location
  Enter  : Unselect nodes and keep them in their new location

Broken stuff:
  Double-clicking a text box will open an editor, but controls aren't finished yet.
)";


//---------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_ui(dvec2 screen_size, double delta) {
  (void)delta;

  double time_start = timestamp();

  if (!show_help) {
    ui_text_painter.bg_col = vec4(0, 0, 0, 0.3);

    static StringDumper d;

    d.clear();

    d("\002========== PRESS F1 FOR HELP ==========\001\n");
    d("World zoom       : %f\n", view_control.view_smooth.view_zoom());
    d("World ppw        : %f\n", view_control.view_smooth.scale_world_to_screen());
    d("World scale      : %f\n", view_control.view_smooth.scale_screen_to_world());

    d("Bad node count   : %d\n", bad_node_count);
    d("Bad edge count   : %d\n", bad_edge_count);
    d("Bad branch count : %d\n", bad_branch_count);
    d("Mouse screen x : %f\n", mouse_pos_screen.x);
    d("Mouse screen y : %f\n", mouse_pos_screen.y);
    d("Mouse world x  : %f\n", mouse_pos_world.x);
    d("Mouse world y  : %f\n", mouse_pos_world.y);
    d("Mouse wrap x   : %f\n", mouse_pos_wrap.x);
    d("Mouse wrap y   : %f\n", mouse_pos_wrap.y);
    d("\n");
    d("Click screen x : %f\n", click_pos_screen.x);
    d("Click screen y : %f\n", click_pos_screen.y);
    d("Click world x  : %f\n", click_pos_world.x);
    d("Click world y  : %f\n", click_pos_world.y);
    d("Click wrap x   : %f\n", click_pos_wrap.x);
    d("Click wrap y   : %f\n", click_pos_wrap.y);
    d("Click offset x : %f\n", clicked_node_offset.x);
    d("Click offset y : %f\n", clicked_node_offset.y);
    d("\n");

    d("Tool mode %s\n", tool_to_string[current_tool].c_str());
    d("Selected nodes : ");
    for (auto selected_node : node_selection) d("%s ", selected_node->plait_cell->name());
    d("\n");

    if (clicked_node) {
      d("Clicked node  : %s %f %f \n", clicked_node->plait_cell->name(), clicked_node->pos_new.x, clicked_node->pos_new.y);
    }
    else {
      d("Clicked node  : \n");
    }

    if (hovered_node) {
      d("Hovered node  : %s %f %f\n", hovered_node->plait_cell->name(), hovered_node->pos_new.x, hovered_node->pos_new.y);
    }
    else {
      d("Hovered node  : \n");
    }

    if (hovered_frame) {
      d("Hovered frame  : %s %f %f\n", hovered_frame->title.c_str(), hovered_frame->pos.x, hovered_frame->pos.y);
    }
    else {
      d("Hovered frame  : \n");
    }

    if (clicked_frame) {
      d("Clicked frame  : %s %f %f\n", clicked_frame->title.c_str(), clicked_frame->pos.x, clicked_frame->pos.y);
    }
    else {
      d("Clicked frame  : \n");
    }

    if (selected_frame) {
      d("Selected frame  : %s %f %f\n", selected_frame->title.c_str(), selected_frame->pos.x, selected_frame->pos.y);
    }
    else {
      d("Selected frame  : \n");
    }


    d("\n");

    d("Clicked label : %s\n", clicked_label ? clicked_label->text.c_str() : "<none>");
    d("Delta time        : %f\n", 1000.0 * time_delta);
    d("Update time       : %f\n", 1000.0 * time_update);
    d("Graph render time : %f\n", 1000.0 * time_render);
    d("UI render time    : %f\n", 1000.0 * time_ui);

    if (hovered_node) {
      hovered_node->plait_cell->dump(d);
      hovered_node->dump(d);
    }

    ui_text_painter.add_text_at(d.c_str(), 8, 8);
  }
  else {
    ui_text_painter.bg_col = vec4(0, 0, 0, 0.8);
    ui_text_painter.add_text_at(help_text, 8, 8);
  }

  ui_text_painter.render(Viewport::screenspace(screen_size), screen_size, 0, 0);

  time_ui = timestamp() - time_start;

  if (selected_frame) {
    auto f = selected_frame;

    ImGui::OpenPopup("Frame Editor");
    if (ImGui::BeginPopupModal("Frame Editor", NULL, 0))
    {
      static ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackResize;

      auto string_resize = [](ImGuiInputTextCallbackData* data) {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
          std::string* str = (std::string*)data->UserData;
          str->resize(data->BufSize);
          data->Buf = str->data();
        }
        return 0;
      };

      if (ImGui::InputText("title", f->title.data(), f->title.size(), flags, string_resize, &f->title)) {
      }
      if (ImGui::InputTextMultiline("text", f->text.data(), f->text.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags, string_resize, &f->text)) {
      }
      ImGui::SliderInt("size x", &f->size.x, 1, 128);
      ImGui::SliderInt("size y", &f->size.y, 1, 128);
      ImGui::SliderInt("scale",  &f->text_scale, 1, 32);
      ImGui::ColorEdit4("Background", (float*)&f->color);


      if (ImGui::Button("Close")) {
        ImGui::CloseCurrentPopup();
        selected_frame = nullptr;
      }
      ImGui::EndPopup();
    }
  }

  if (0) {
    ImGui::Begin("Label Editor");

    /*
    static char str0[128] = "Label Text Here";

    if (clicked_label) {
      strcpy(str0, clicked_label->text.c_str());
    }
    */

    /*
    if (ImGui::InputText("text", str0, IM_ARRAYSIZE(str0))) {
      if (clicked_label) {
        clicked_label->text = str0;
      }
    }
    */

    /*
    if (ImGui::Button("Create label")) {
      //LOG_B("Creating label %s\n", str0);
      PlaitLabel* label = new PlaitLabel {
        str0,
        view_control.view_smooth_snap.world_center(),
        view_control.view_smooth_snap.world_center(),
        16.0
      };
      plait.labels.push_back(label);
    }
    */

    //static char node_tag[128];

    //ImGui::InputText("tag", node_tag, IM_ARRAYSIZE(node_tag));

    /*
    if (ImGui::Button("Find Tag")) {
      //LOG_B("find tag\n");
      for (auto& [tag, plait_cell] : plait.cell_map) {
        if (tag == node_tag) {
          view_control.center_on(plait_cell->core_node->pos_old + node_size * 0.5);
        }
      }
    }
    */

    ImGui::End();
  }
}

//---------------------------------------------------------------------------------------------------------------------
