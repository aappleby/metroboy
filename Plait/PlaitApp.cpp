#include "Plait/PlaitApp.h"
#include "AppLib/GLBase.h"

#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#include "Plait/CellDB.h"

#include "imgui/imgui.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <deque>
#include <algorithm>
#include <filesystem>
#include <sstream>

//#include <windows.h>

using namespace std;

//const dvec2 node_size = {128,64};
const dvec2 node_size = {64,64};
const dvec2 node_half_size = {32,32};
const dvec2 port_size = {4,4};

const int world_min = -1;
const int world_max = 1;
//const int world_min = 0;
//const int world_max = 0;

static std::map<ToolMode, std::string> tool_to_string = {
  {ToolMode::NONE,            "NONE"},
  {ToolMode::IMGUI,           "IMGUI"},

  {ToolMode::DRAG_NODE,       "DRAG_NODE"},
  {ToolMode::DRAG_LABEL,      "DRAG_LABEL"},

  {ToolMode::SELECT_REGION,   "SELECT_REGION"},
  {ToolMode::GHOST_REGION,    "GHOST_REGION"},

  {ToolMode::CREATE_ROOT,     "CREATE_ROOT"},
  {ToolMode::CREATE_LEAF,     "CREATE_LEAF"},
  {ToolMode::DELETE_NODE,     "DELETE_NODE"},
  {ToolMode::LINK_NODE,       "LINK_NODE"},

  {ToolMode::PAN_VIEW,        "PAN_VIEW"},
  {ToolMode::MENU_OPTION,     "MENU_OPTION"},
};

dvec2 wrap(dvec2 d) {
  while(d.x >  32768) d.x -= 65536;
  while(d.x < -32768) d.x += 65536;
  while(d.y >  32768) d.y -= 65536;
  while(d.y < -32768) d.y += 65536;
  return d;
}

dvec2 wrap_towards(dvec2 origin, dvec2 d) {
  while((d.x - origin.x) >  32768) d.x -= 65536;
  while((d.x - origin.x) < -32768) d.x += 65536;
  while((d.y - origin.y) >  32768) d.y -= 65536;
  while((d.y - origin.y) < -32768) d.y += 65536;
  return d;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  int ret = 0;

  const char* die_db_file = "gameboy.die_db.json";
  (void)die_db_file;

  PlaitApp* app = new PlaitApp();

#if 1
  printf("Parsing gateboy source\n");
  app->die_db.parse_dir("GateBoyLib");
  printf("Done\n\n");

  {
    printf("Saving gameboy.die_db.json\n");
    //std::ostringstream stream_out;
    //app->die_db.save_json(stream_out);
    app->die_db.save_json(die_db_file);
    printf("Done\n\n");

    printf("Loading gameboy.die_db.json\n");
    app->die_db.clear();
    //std::istringstream stream_in(stream_out.str());
    //app->die_db.load_json(stream_in);
    app->die_db.load_json(die_db_file);
    printf("Done\n\n");
  }

#else
  printf("Loading gameboy.die_db.json\n");
  app->die_db.clear();
  app->die_db.load_json("gameboy.die_db.json");
  printf("Done\n\n");
#endif


#if 0
  {
    printf("Loading gameboy.plait.json\n");
    {
      nlohmann::json jroot;
      std::ifstream("gameboy.plait.json") >> jroot;
      app->plait.from_json(jroot, app->die_db);
    }
    printf("Done\n\n");


    printf("Saving gameboy.plait_hax.json\n");
    {
      nlohmann::json jroot;
      app->plait.to_json(jroot);
      std::ofstream("gameboy.plait_hax.json") << jroot.dump(2);
    }
    printf("Done\n\n");

    printf("Loading gameboy.plait_hax.json\n");
    {
      app->plait.clear();
      nlohmann::json jroot;
      std::ifstream("gameboy.plait_hax.json") >> jroot;
      app->plait.from_json(jroot, app->die_db);
    }
    printf("Done\n\n");
  }
#endif

#if 1

  printf("Loading gameboy.plait.json\n");
  {
    nlohmann::json jroot;
    std::ifstream("gameboy.plait.json") >> jroot;
    app->plait.from_json(jroot, app->die_db);
  }
  printf("Done\n\n");


  size_t total_nodes = 0;
  for (auto& [tag, plait_cell] : app->plait.cell_map) {
    total_nodes++;
    total_nodes += plait_cell->leaf_nodes.size();
  }
  printf("Total cells %zd\n", app->plait.cell_map.size());
  printf("Total nodes %zd\n", total_nodes);
  printf("Total edges %zd\n", app->plait.traces.size());

  AppHost* app_host = new AppHost(app);
  ret = app_host->app_main(argc, argv);
  delete app;
#endif

  return ret;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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
      printf("Could not pick a color for %s\n", node->plait_cell->gate());
    }
  }

  //node->color = 0x00FFFFFF;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_init(int screen_w, int screen_h) {
  view_control.init(screen_w, screen_h);

  box_painter.init();
  port_painter.init();
  grid_painter.init();
  edge_painter.init();
  outline_painter.init();
  text_painter.init();
  ui_text_painter.init();
  blitter.init();

  check_gl_error();

  uint32_t pix[] = {
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
  };

  tex = create_texture_u32(4, 4, pix);

  for (auto& [tag, cell] : plait.cell_map) {
    paint_node(cell->core_node);
    for (auto& [name, root] : cell->root_nodes) {
      paint_node(root);
    }
    for (auto& [name, leaf] : cell->leaf_nodes) {
      paint_node(leaf);
    }
  }

  printf("Init done %f\n", timestamp());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_close() {
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitApp::hit_node(dvec2 _mouse_pos, PlaitNode* node) {
  dvec2 node_pos = node->get_pos_new();

  double min_x = node_pos.x;
  double min_y = node_pos.y;
  double max_x = node_pos.x + node_size.x;
  double max_y = node_pos.y + node_size.y;

  while(_mouse_pos.x - min_x >  32768) _mouse_pos.x -= 65536;
  while(_mouse_pos.x - max_x < -32768) _mouse_pos.x += 65536;

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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

void PlaitApp::apply_region_node(dvec2 corner_a, dvec2 corner_b, NodeCallback callback) {
  for (auto& [tag, plait_cell] : plait.cell_map) {
    if (contains_node(corner_a, corner_b, plait_cell->core_node)) {
      callback(plait_cell->core_node);
    }
    for (auto& [name, root] : plait_cell->root_nodes) {
      if (contains_node(corner_a, corner_b, root)) {
        callback(root);
      }
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      if (contains_node(corner_a, corner_b, leaf)) {
        callback(leaf);
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::select_region(dvec2 corner_a, dvec2 corner_b) {
  printf("Selection region ");
  auto callback = [this](PlaitNode* node) {
    printf("%s ", node->plait_cell->name());
    node->select();
    node_selection.insert(node);
  };

  apply_region_node(corner_a, corner_b, callback);
  printf("\n");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::select_node(PlaitNode* node) {
  printf("Selecting %s\n", node->plait_cell->name());
  node->select();
  node_selection.insert(node);
}

void PlaitApp::commit_selection() {
  if (node_selection.empty()) return;

  printf("Committing ");
  for (auto node : node_selection) {
    printf("%s ", node->plait_cell->name());
    node->commit_pos();
  }
  printf("\n");
}

void PlaitApp::revert_selection() {
  if (node_selection.empty()) return;

  printf("Reverting ");
  for (auto node : node_selection) {
    printf("%s ", node->plait_cell->name());
    node->revert_pos();
  }
  printf("\n");
}

void PlaitApp::clear_selection() {
  if (node_selection.empty()) return;

  printf("Unselecting ");
  for (auto node : node_selection) {
    printf("%s ", node->plait_cell->name());
    node->deselect();
  }
  node_selection.clear();
  printf("\n");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::begin_frame(int screen_w, int screen_h) {
  view_control.begin_frame(screen_w, screen_h);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::event_menu_option(SDL_Event event) {
  switch(event.type) {
  case SDL_KEYDOWN: {
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_S) {
      const char* filename = "gameboy.plait.json";
      printf("Saving plait %s\n", filename);
      commit_selection();
      clear_selection();

      nlohmann::json jroot;
      plait.to_json(jroot);
      std::ofstream(filename) << jroot.dump(2);
    }
    break;
  }
  case SDL_KEYUP: {
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_LALT) {
      current_tool = ToolMode::NONE;
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
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_LCTRL) {
      current_tool = ToolMode::NONE;
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
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_Q) {
      current_tool = ToolMode::NONE;
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
      view_control.pan(event.motion.xrel, event.motion.yrel);
    }
    break;
  }
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      if (!was_drag) {
        commit_selection();
        clear_selection();
      }
      current_tool = ToolMode::NONE;
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
      pos_abs_new.x = round(pos_abs_new.x / 16) * 16.0;
      pos_abs_new.y = round(pos_abs_new.y / 16) * 16.0;
      dvec2 delta = pos_abs_new - clicked_node->get_pos_new();
      for (auto node : node_selection) node->move(delta);
    }
    break;
  }
  case SDL_MOUSEBUTTONUP: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      current_tool = ToolMode::NONE;
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
      if (clicked_label) {
        clicked_label->pos_old = clicked_label->pos_new;
      }
      current_tool = ToolMode::NONE;
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

  if (!io.WantCaptureKeyboard && !io.WantCaptureMouse) {
    current_tool = ToolMode::NONE;
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
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_Z) {
      current_tool = ToolMode::NONE;
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
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_X) {
      current_tool = ToolMode::NONE;
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
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_C) {
      current_tool = ToolMode::NONE;
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
    int key = event.key.keysym.scancode;
    if (key == SDL_SCANCODE_V) {
      current_tool = ToolMode::NONE;
    }
    break;
  }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::event_select_tool(SDL_Event event) {
  if (event.type == SDL_KEYDOWN) {
    int key = event.key.keysym.scancode;
    ToolMode new_tool = ToolMode::NONE;

    if (key == SDL_SCANCODE_E) show_edges = !show_edges;

    if (key == SDL_SCANCODE_Q)     new_tool = ToolMode::GHOST_REGION;
    if (key == SDL_SCANCODE_Z)     new_tool = ToolMode::CREATE_ROOT;
    if (key == SDL_SCANCODE_X)     new_tool = ToolMode::CREATE_LEAF;
    if (key == SDL_SCANCODE_C)     new_tool = ToolMode::LINK_NODE;
    if (key == SDL_SCANCODE_V)     new_tool = ToolMode::DELETE_NODE;
    if (key == SDL_SCANCODE_LALT)  new_tool = ToolMode::MENU_OPTION;
    if (key == SDL_SCANCODE_LCTRL) new_tool = ToolMode::SELECT_REGION;

    if (new_tool != ToolMode::NONE) {
      current_tool = new_tool;
    }

    if (key == SDL_SCANCODE_ESCAPE && node_selection.size()) {
      printf("Reverting selection\n");
      revert_selection();
      clear_selection();
    }

    if (key == SDL_SCANCODE_RETURN && node_selection.size()) {
      printf("Commiting selection\n");
      commit_selection();
      clear_selection();
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
    else if (new_clicked_label) {
      clicked_label = new_clicked_label;
      clicked_label_offset = clicked_label->pos_new - mouse_pos_world;
      current_tool = ToolMode::DRAG_LABEL;
    }
    else {
      clicked_label = nullptr;
      //commit_selection();
      //clear_selection();
      current_tool = ToolMode::PAN_VIEW;
    }
  }
  else {
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
      current_tool = ToolMode::IMGUI;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_update(double delta_time) {
  auto& view = view_control.view_smooth_snap;

  time_delta = delta_time;

  double time_start = timestamp();

  {
    int mouse_x = 0, mouse_y = 0;
    mouse_buttons    = SDL_GetMouseState(&mouse_x, &mouse_y);
    mouse_pos_screen = {mouse_x, mouse_y};
    mouse_pos_world  = view.screenToWorld(mouse_pos_screen);
    mouse_pos_wrap = mouse_pos_world;
    while(mouse_pos_wrap.x >  32768) mouse_pos_wrap.x -= 65536;
    while(mouse_pos_wrap.x < -32768) mouse_pos_wrap.x += 65536;
  }

  {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    io.MouseDown[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    io.MousePos = { (float)mouse_pos_screen.x, (float)mouse_pos_screen.y };
  }

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONDOWN && (event.button.button & SDL_BUTTON_LMASK)) {
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
    }


    switch(current_tool) {
    case ToolMode::NONE:           event_select_tool(event); break;
    case ToolMode::IMGUI:          event_imgui(event); break;
    case ToolMode::DRAG_NODE:      event_drag_nodes(event); break;
    case ToolMode::DRAG_LABEL:     event_drag_label(event); break;
    case ToolMode::SELECT_REGION:  event_select_region(event); break;
    case ToolMode::GHOST_REGION:   event_ghost_region(event); break;
    case ToolMode::CREATE_ROOT:    event_create_root(event); break;
    case ToolMode::CREATE_LEAF:    event_create_leaf(event); break;
    case ToolMode::LINK_NODE:      event_link_node(event); break;
    case ToolMode::DELETE_NODE:    event_delete_node(event); break;
    case ToolMode::PAN_VIEW:       event_pan_view(event); break;
    case ToolMode::MENU_OPTION:    event_menu_option(event); break;
    default: {
      printf("Bad tool!\n");
      __debugbreak();
      break;
    }
    }

    if(event.type == SDL_MOUSEWHEEL) {
      view_control.on_mouse_wheel((int)mouse_pos_screen.x, (int)mouse_pos_screen.y, double(event.wheel.y) * 0.25);
    }

    if (event.type == SDL_MOUSEBUTTONUP && (event.button.button & SDL_BUTTON_LMASK)) {
      clicked_node = nullptr;
    }
  }

  if (current_tool != ToolMode::IMGUI) {
    hovered_node = pick_node(mouse_pos_world);
  }
  else {
    hovered_node = nullptr;
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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
void PlaitApp::draw_node_outline(PlaitNode* node) {
  dvec2 node_pos_old = node->get_pos_old();
  dvec2 node_pos_new = node->get_pos_new();

  //----------------------------------------

  if (node->plait_cell->selected_node_count) {
    uint32_t color = node->selected() ? 0xFFFFFFFF : 0xFF88CCFF;
    box_painter.push_corner_size(node_pos_new, node_size, color);
    box_painter.push_corner_size(node_pos_new, node_size, 0xFF000000);
  }
  else {
    uint32_t color = COL_DARK_GREY;
    if (node->plait_cell->leaf_nodes.size()) {
      color = (node->name == "core") ? 0xFF408040 : 0xFF804040;
    }

    box_painter.push_corner_size(node_pos_new, node_size, color);
    box_painter.push_corner_size(node_pos_new, node_size, 0xFF000000);
  }
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::draw_node_fill(PlaitNode* node, uint32_t color) {
  if (!node->ghosted) {
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

static char buf[256];

void PlaitApp::draw_node_text(PlaitNode* node) {
  dvec2 node_pos_new = node->get_pos_new();
  text_painter.add_text_at_simple(node->plait_cell->name(), float(node_pos_new.x), float(node_pos_new.y));
  //text_painter.add_text_at_simple(node->plait_cell->gate(), float(node_pos_new.x + 8), float(node_pos_new.y + 24));

  //sprintf_s(buf, 256, "%d : %d", (int)node->plait_cell->root_nodes.size(), (int)node->plait_cell->leaf_nodes.size());
  //text_painter.add_text_at(buf, float(node_pos_new.x + 8), float(node_pos_new.y + 40));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::draw_edge(PlaitTrace* edge) {
  auto output_node = edge->output_node;
  auto input_node  = edge->input_node;

  //if (!output_node->visible && !input_node->visible) return;

  if (output_node->ghosted || input_node->ghosted) return;

  auto output_pos_new = output_node->get_pos_new();
  auto input_pos_new  = input_node->get_pos_new();

  /*
  if (edge->input_node->plait_cell->die_cell->cell_type == DieCellType::DFF) {
    auto& input_port_name = edge->input_node->plait_cell->die_cell->input_ports[edge->input_port_index];
    if (input_port_name[0] == 'D') {
      input_pos_new.x += 65536;
    }
  }
  */

  auto delta = wrap(input_pos_new - output_pos_new);
  input_pos_new = output_pos_new + delta;


  // Highlight "backwards" edges in red.
  bool edge_backwards = delta.x < 0;
  uint32_t output_color = edge_backwards ? 0xFF0000FF : 0x40FFFFFF;
  uint32_t input_color = edge_backwards ? 0xFF0000FF : 0x4044FF44;

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_frame() {

  auto& view = view_control.view_smooth_snap;

  double time_start = timestamp();

  box_painter.reset();
  port_painter.reset();
  edge_painter.reset();
  text_painter.reset();

  //----------------------------------------
  // Grid layer

  grid_painter.render(view);

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
      box_painter.push_corner_size(node->get_pos_new() - dvec2(3,3), node_size + dvec2(6,6), 0xFFFFFFFF);
    }
  }

  //----------
  // Node fills

  for (auto& [tag, plait_cell] : plait.cell_map) {
    uint32_t color = plait_cell->core_node->color;

    if (plait_cell->root_nodes.empty() && plait_cell->leaf_nodes.empty()) {
      color = (color & 0xFF000000) | ((color & 0x00FEFEFE) >> 1);
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

  //----------

#if 1
  // Branches
  for (auto& [tag, plait_cell] : plait.cell_map) {
    //if (plait_cell->die_cell->cell_type == DieCellType::TRIBUF) continue;
    //if (plait_cell->die_cell->cell_type == DieCellType::BUS) continue;
    //if (plait_cell->die_cell->cell_type == DieCellType::DFF) continue;
    //if (plait_cell->die_cell->cell_type == DieCellType::PIN_IO) continue;
    if (plait_cell->core_node->ghosted) continue;

    bool wrap_ok = false;
    if (plait_cell->die_cell->cell_type == DieCellType::BUS)    wrap_ok = true;
    if (plait_cell->die_cell->cell_type == DieCellType::DFF)    wrap_ok = true;
    if (plait_cell->die_cell->cell_type == DieCellType::PIN_IO) wrap_ok = true;

    bool always_draw = false;
    //if (plait_cell->die_cell->cell_type == DieCellType::LATCH) always_draw = true;
    //if (plait_cell->die_cell->cell_type == DieCellType::LOGIC) always_draw = true;
    //if (plait_cell->die_cell->cell_type == DieCellType::ADDER) always_draw = true;

    for (auto& [name, root] : plait_cell->root_nodes) {
      if (root->ghosted) continue;
      auto core_pos = plait_cell->core_node->pos_new;
      auto root_pos = wrap_towards(core_pos, root->pos_new);
      bool backwards = (root_pos.x > core_pos.x) && !wrap_ok;
      uint32_t color_a = backwards ? 0xFF0000FF : 0xFFFFFF00;
      uint32_t color_b = backwards ? 0xFF0000FF : 0xFFFF00FF;

      if ((plait_cell->core_node->selected() ^ root->selected()) || backwards || always_draw) {
        edge_painter.push(core_pos + node_half_size,
                          color_a,
                          root_pos + node_half_size,
                          color_b);
      }
    }
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      if (leaf->ghosted) continue;
      auto core_pos = plait_cell->core_node->pos_new;
      auto leaf_pos = wrap_towards(core_pos, leaf->pos_new);
      bool backwards = (leaf_pos.x < core_pos.x) && !wrap_ok;
      uint32_t color_a = backwards ? 0xFF0000FF : 0xFFFFFF00;
      uint32_t color_b = backwards ? 0xFF0000FF : 0xFFFF00FF;

      if ((plait_cell->core_node->selected() ^ leaf->selected()) || backwards || always_draw) {
        edge_painter.push(core_pos + node_half_size,
                          color_a,
                          leaf_pos + node_half_size,
                          color_b);
      }
    }
  }

  /*
  // Cores with only one branch
  for (auto& [tag, plait_cell] : plait.cell_map) {
    if (plait_cell->core_node->ghosted) continue;
    if (plait_cell->die_cell->cell_type == DieCellType::DFF) continue;
    if (plait_cell->die_cell->cell_type == DieCellType::BUS) continue;
    if (plait_cell->die_cell->cell_type == DieCellType::PIN_IO) continue;
    if (plait_cell->die_cell->cell_type == DieCellType::TRIBUF) continue;

    if (plait_cell->leaf_nodes.size() != 1 || plait_cell->die_cell->fanout != 1) continue;

    //for (auto& [name, root] : plait_cell->root_nodes) {
    //  auto core_pos = plait_cell->core_node->pos_new + dvec2(64, 32);
    //  auto root_pos = root->pos_new + dvec2(64, 32);
    //  bool backwards = (root_pos.x > core_pos.x);
    //  uint32_t color_a = backwards ? 0xFF0000FF : 0xFF00FF00;
    //  uint32_t color_b = backwards ? 0xFF0000FF : 0xFFFF0000;
    //
    //  edge_painter.push(core_pos, color_a, root_pos, color_b);
    //}
    for (auto& [name, leaf] : plait_cell->leaf_nodes) {
      auto core_pos = plait_cell->core_node->pos_new + dvec2(64, 32);
      auto leaf_pos = leaf->pos_new + dvec2(64, 32);
      bool backwards = (leaf_pos.x < core_pos.x);
      uint32_t color_a = backwards ? 0xFF0000FF : 0xFF00FF00;
      uint32_t color_b = backwards ? 0xFF0000FF : 0xFFFF0000;

      edge_painter.push(core_pos, color_a, leaf_pos, color_b);
    }
  }
  */

#endif


  //----------------------------------------
  // Labels

  for (auto& label : plait.labels) {
    uint32_t color = (label == clicked_label) ? COL_VIOLET : COL_PALE_GREY;
    color |= 0xFF000000;
    text_painter.add_text_at(label->text.c_str(), color, label->pos_new.x, label->pos_new.y, label->scale);
  }

  //----------
  // Dump stuff to GPU

  box_painter.update_buf();
  port_painter.update_buf();
  edge_painter.update_buf();
  text_painter.update_buf();

  for (int i = world_min; i <= world_max; i++) {
    box_painter .render_at(view, 65536 * i, 0, 1);
    port_painter.render_at(view, 65536 * i, 0, 1);
    edge_painter.render_at(view, 65536 * i, 0, 1);
    text_painter.render_at(view, 65536 * i, 0);
  }

  //----------------------------------------
  // Selection rect

  if (mouse_buttons & SDL_BUTTON_LMASK) {
    uint32_t sel_color = 0x00000000;
    if (current_tool == ToolMode::SELECT_REGION) sel_color = 0xFFFFFFFF;
    if (current_tool == ToolMode::GHOST_REGION)  sel_color = 0xFFFF0040;

    if (sel_color) {
      outline_painter.push_box(click_pos_wrap, mouse_pos_wrap, sel_color);
      outline_painter.render(view, 0, 0, 1);
    }
  }

  //----------------------------------------
  // Done

  frame_count++;

  time_render = timestamp() - time_start;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_ui() {
#if 1

  // Draw selection info

  double time_start = timestamp();
  ui_text_painter.reset();

  {
    static StringDumper d;

    d.clear();

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

    ui_text_painter.add_text_at(d.c_str(), 0, 0);
  }

  ui_text_painter.render(view_control.view_screen, 0, 0);
  time_ui = timestamp() - time_start;

  {
    ImGui::Begin("Label Editor");
    static char str0[128] = "Label Text Here";

    if (clicked_label) {
      strcpy_s(str0, 128, clicked_label->text.c_str());
    }

    if (ImGui::InputText("text", str0, IM_ARRAYSIZE(str0))) {
      if (clicked_label) {
        clicked_label->text = str0;
      }
    }

    if (ImGui::Button("Create label")) {
      printf("Creating label %s\n", str0);
      PlaitLabel* label = new PlaitLabel {
        str0,
        view_control.view_smooth_snap.center(),
        view_control.view_smooth_snap.center(),
        16.0
      };
      plait.labels.push_back(label);
    }

    static char node_tag[128];
    ImGui::InputText("tag", node_tag, IM_ARRAYSIZE(node_tag));
    if (ImGui::Button("Find Tag")) {
      printf("find tag\n");
      for (auto& [tag, plait_cell] : plait.cell_map) {
        if (tag == node_tag) {
          view_control.center_on(plait_cell->core_node->pos_old + node_size * 0.5);
        }
      }

    }


    ImGui::End();
  }

#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
