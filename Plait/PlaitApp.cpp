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

//#include <windows.h>

using namespace std;

static uint32_t event_mousepos;

static std::map<ToolMode, std::string> tool_to_string = {
  {ToolMode::NONE,            "NONE"},
  {ToolMode::IMGUI,           "IMGUI"},
  {ToolMode::DRAG_NODE,       "DRAG_NODE"},
  {ToolMode::SELECT_REGION,   "SELECT_REGION"},
  {ToolMode::LOCK_REGION,     "LOCK_REGION"},
  {ToolMode::UNLOCK_REGION,   "UNLOCK_REGION"},
  {ToolMode::PLACE_ANCHOR,    "PLACE_ANCHOR"},
  {ToolMode::PAN_VIEW,        "PAN_VIEW"},
  {ToolMode::MENU_OPTION,     "MENU_OPTION"},
  {ToolMode::HIGHLIGHT_HOVER, "HIGHLIGHT_HOVER"},
};

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  int ret = 0;

  PlaitApp* app = new PlaitApp();

  //app->cell_db.parse_dir("GateBoyLib");
  printf("Loading gameboy.cell_db.json\n");
  app->cell_db.load_json("gameboy.cell_db.json");

  printf("Loading gameboy.plait.json\n");
  app->plait.load_json("gameboy.plait.json", app->cell_db);


  AppHost* app_host = new AppHost(app);
  ret = app_host->app_main(argc, argv);
  delete app;

  return ret;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

PlaitApp::~PlaitApp() {
};

const char* PlaitApp::app_get_title() {
  return "PlaitApp";
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_init(int screen_w, int screen_h) {
  view_control.init(screen_w, screen_h);

  event_mousepos = SDL_RegisterEvents(1);


#if 1
  auto& n2c = node_type_to_color;

  //n2c[""]      = 0xFF008000;

  n2c["not1"]  = COL_MID_GREY;

  n2c["and2"]  = COL_MID_RED;
  n2c["and3"]  = COL_MID_RED;
  n2c["and4"]  = COL_MID_RED;

  n2c["nand2"] = COL_MID_RED - COL_HINT1;
  n2c["nand3"] = COL_MID_RED - COL_HINT1;
  n2c["nand4"] = COL_MID_RED - COL_HINT1;
  n2c["nand5"] = COL_MID_RED - COL_HINT1;
  n2c["nand6"] = COL_MID_RED - COL_HINT1;
  n2c["nand7"] = COL_MID_RED - COL_HINT1;

  n2c["or2"]   = COL_MID_BLUE;
  n2c["or3"]   = COL_MID_BLUE;
  n2c["or4"]   = COL_MID_BLUE;

  n2c["nor2"]  = COL_MID_BLUE - COL_HINT1;
  n2c["nor3"]  = COL_MID_BLUE - COL_HINT1;
  n2c["nor4"]  = COL_MID_BLUE - COL_HINT1;
  n2c["nor5"]  = COL_MID_BLUE - COL_HINT1;
  n2c["nor6"]  = COL_MID_BLUE - COL_HINT1;
  n2c["nor7"]  = COL_MID_BLUE - COL_HINT1;
  n2c["nor8"]  = COL_MID_BLUE - COL_HINT1;

  n2c["and_or3"]     = COL_MID_MAGENTA;
  n2c["or_and3"]     = COL_MID_MAGENTA;
  n2c["not_or_and3"] = COL_MID_MAGENTA - COL_HINT1;

  n2c["add_s"] = COL_AZURE;
  n2c["add_c"] = COL_AZURE;

  n2c["xor2"]  = COL_MINT;
  n2c["xnor2"] = COL_MINT;

  n2c["mux2n"] = COL_ROSE - COL_HINT1;
  n2c["mux2p"] = COL_ROSE + COL_HINT1;
  n2c["amux2"] = COL_ROSE;
  n2c["amux4"] = COL_ROSE;
#endif

  check_gl_error();
  box_painter.init();
  check_gl_error();
  port_painter.init();
  check_gl_error();
  grid_painter.init();
  check_gl_error();
  edge_painter.init();
  check_gl_error();
  outline_painter.init();
  check_gl_error();
  text_painter.init();
  check_gl_error();
  blitter.init();
  check_gl_error();

  uint32_t pix[] = {
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFFFF00FF, 0xFFFF00FF, 0xFF00FFFF,
    0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF, 0xFF00FFFF,
  };

  tex = create_texture_u32(4, 4, pix);

  for (auto& [tag, node] : plait.node_map) {
    node->color = 0x00000000;

    auto cell_type = node->get_cell()->cell_type;
    if (cell_type == CellType::PIN_IN)  node->color = COL_PALE_RED;
    if (cell_type == CellType::PIN_OUT) node->color = COL_PALE_GREEN;
    if (cell_type == CellType::PIN_IO)  node->color = COL_PALE_YELLOW;

    if (cell_type == CellType::SIG_IN)  node->color = COL_MID_RED;
    if (cell_type == CellType::SIG_OUT) node->color = COL_MID_GREEN;

    if (cell_type == CellType::BUS)     node->color = COL_DARK_GREY;
    if (cell_type == CellType::DFF)     node->color = COL_DARK_YELLOW;
    if (cell_type == CellType::LATCH)   node->color = COL_ORANGE;
    if (cell_type == CellType::TRIBUF)  node->color = COL_MID_YELLOW;

    if (cell_type == CellType::LOGIC) {
      auto it = node_type_to_color.find(node->gate());
      if (it != node_type_to_color.end()) {
        node->color = (*it).second;
      }
      else {
        printf("Could not pick a color for %s\n", node->gate());
      }
    }
  }

  printf("Init done %f\n", timestamp());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_close() {
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

Node* PlaitApp::pick_node(dvec2 _mouse_pos, bool ignore_selected, bool ignore_clicked, bool ignore_hovered) {
  (void)ignore_selected;
  (void)ignore_clicked;
  (void)ignore_hovered;

  for (auto& [tag, node] : plait.node_map) {
    if ((node == clicked_node) && ignore_clicked) continue;

    dvec2 node_pos = node->get_pos_abs_new();

    int width = 128;
    int height = 64;

    if (_mouse_pos.x >= node_pos.x &&
        _mouse_pos.y >= node_pos.y &&
        _mouse_pos.x <= node_pos.x + width &&
        _mouse_pos.y <= node_pos.y + height) {
      //printf("hit node\n");
      return node;
    }
  }

  return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::apply_region(dvec2 corner_a, dvec2 corner_b, NodeCallback callback) {

  dvec2 rmin = min(corner_a, corner_b);
  dvec2 rmax = max(corner_a, corner_b);

  const dvec2 node_size = {128,64};

  for (auto& [tag, node] : plait.node_map) {
    dvec2 nmin = node->get_pos_abs_new();
    dvec2 nmax = node->get_pos_abs_new() + node_size;

    if (nmin.x < rmin.x) continue;
    if (nmin.y < rmin.y) continue;
    if (nmax.x > rmax.x) continue;
    if (nmax.y > rmax.y) continue;

    callback(node);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::select_region(dvec2 corner_a, dvec2 corner_b) {
  printf("Selection region ");
  auto callback = [this](Node* node) {
    printf("%s ", node->name());
    node->selected = true;
    selection.insert(node);
  };

  apply_region(corner_a, corner_b, callback);
  printf("\n");
}

void PlaitApp::lock_region(dvec2 corner_a, dvec2 corner_b) {
  printf("Locking region ");
  auto callback = [this](Node* node) {
    printf("%s ", node->name());
    node->locked = true;
  };

  apply_region(corner_a, corner_b, callback);
  printf("\n");
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::select_node(Node* node) {
  printf("Selecting %s\n", node->name());
  node->selected = true;
  selection.insert(node);
}

void PlaitApp::commit_selection() {
  if (selection.empty()) return;

  printf("Committing ");
  for (auto node : selection) {
    printf("%s ", node->name());
    node->commit_pos();
  }
  printf("\n");
}

void PlaitApp::revert_selection() {
  if (selection.empty()) return;

  printf("Reverting ");
  for (auto node : selection) {
    printf("%s ", node->name());
    node->revert_pos();
  }
  printf("\n");
}

void PlaitApp::clear_selection() {
  if (selection.empty()) return;

  printf("Unselecting ");
  for (auto node : selection) {
    printf("%s ", node->name());
    node->selected = false;
  }
  selection.clear();
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

void spring_nodes(Node* a, Node* b) {
  if (a->ghost || b->ghost) return;
  if (a->floating || b->floating) return;

  dvec2 offset = b->get_pos_abs_new() - a->get_pos_abs_new();
  double dist = length(offset);
  offset = normalize(offset);

  if (dist < 1) return;

  double push_force = remap_clamp(dist, 10, 256, -256, 0);
  double pull_force = remap_clamp(dist, 1024, 16384, 0, 16384);

  a->spring_force += offset * push_force;
  a->spring_force += offset * pull_force;

  b->spring_force += offset * -push_force;
  b->spring_force += offset * -pull_force;
}

void spring_nodes2(Node* a, Node* b) {
  if (a->ghost || b->ghost) return;
  if (a->floating || b->floating) return;

  dvec2 offset = b->get_pos_abs_new() - a->get_pos_abs_new();
  double dist = length(offset);
  offset = normalize(offset);

  if (dist < 1) return;

  double push_force = remap_clamp(dist,   0,  96, -512,   0);
  double pull_force = remap_clamp(dist,  96, 192,    0, 512);

  a->spring_force += offset * push_force;
  a->spring_force += offset * pull_force;

  b->spring_force += offset * -push_force;
  b->spring_force += offset * -pull_force;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::begin_frame(int screen_w, int screen_h) {
    view_control.begin_frame(screen_w, screen_h);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitApp::on_mouse_down(uint32_t mouse_buttons_posedge) {
  (void)mouse_buttons_posedge;

#if 0
  if (mouse_buttons_posedge & SDL_BUTTON_LMASK) {
    click_start_screen = mouse_pos_screen;
    click_start_world = view_control.view_snap.screenToWorld(click_start_screen);

    clicked_node = pick_node(click_start_world, /*ignore_selected*/ false, /*ignore_clicked*/ false, /*ignore_hovered*/ false);

    if (keys_old[SDL_SCANCODE_D]) {
      if (!clicked_node) {
        printf("D-clicked on nothing\n");
      }
      else if (clicked_node->selected) {
        printf("D-clicked on selected node %s\n", clicked_node->name());
        clicked_node->toggle_locked();
      }
      else {
        printf("D-clicked on unselected node %s\n", clicked_node->name());
        clicked_node->toggle_locked();
      }
    }
    if (keys_old[SDL_SCANCODE_LCTRL]) {
      if (!clicked_node) {
        printf("Control-clicked on nothing\n");
      }
      else if (clicked_node->selected) {
        printf("Control-clicked on selected node %s\n", clicked_node->name());
      }
      else {
        printf("Control-clicked on unselected node %s\n", clicked_node->name());
        select_node(clicked_node);
      }
    }
    else if (keys_old[SDL_SCANCODE_F]) {
      if (!clicked_node) {
        printf("F-clicked on nothing\n");
      }
      else if (clicked_node->selected) {
        printf("F-clicked on selected node %s\n", clicked_node->name());
      }
      else {
        printf("F-clicked on unselected node %s\n", clicked_node->name());
        // quick anchor, clicked node becomes anchor arg
        commit_selection();
        clear_selection();
        select_node(clicked_node);
      }
    }
    else {
      if (!clicked_node) {
        printf("Clicked on nothing\n");
      }
      else if (clicked_node->selected) {
        printf("Clicked on selected node %s\n", clicked_node->name());
        commit_selection();
      }
      else {
        printf("Clicked on unselected node %s\n", clicked_node->name());
        // clicked on unselected node, quick select it.
        commit_selection();
        clear_selection();
        select_node(clicked_node);
      }
    }
  }
#endif
  return false;
}

bool PlaitApp::on_mouse_up(uint32_t mouse_buttons_negedge) {
  (void)mouse_buttons_negedge;

#if 0
  if (mouse_buttons_negedge & SDL_BUTTON_LMASK) {
    click_end_screen = mouse_pos_screen;
    click_end_world = view.screenToWorld(click_end_screen);

    auto end_node = pick_node(click_end_world, /*ignore_selected*/ false, /*ignore_clicked*/ false, /*ignore_hovered*/ false);
    bool was_drag = length(click_end_screen - click_start_screen) > 3;

    if (keys_old[SDL_SCANCODE_D]) {
      if (was_drag) {
        lock_region(click_start_world, click_end_world);
      }
    }
    if (keys_old[SDL_SCANCODE_F]) {
      if (end_node) {
        if (selection.size()) {
          printf("Linking selection ");
          for (auto selected_node : selection) printf("%s ", selected_node->name());
          printf("to %s\n", end_node->name());
          for (auto selected_node : selection) selected_node->set_anchor(nullptr);
          for (auto selected_node : selection) selected_node->set_anchor(end_node);
        }
      }
      else {
        if (selection.size()) {
          printf("Unlinking selection ");
          for (auto selected_node : selection) printf("%s ", selected_node->name());
          printf("\n");
          for (auto selected_node : selection) selected_node->set_anchor(nullptr);
        }
        if (clicked_node) {
          printf("Unlinking clicked node %s\n", clicked_node->name());
          clicked_node->set_anchor(nullptr);
        }
        commit_selection();
        clear_selection();
      }
    }
    else if (keys_old[SDL_SCANCODE_LCTRL] && keys_new[SDL_SCANCODE_LCTRL]) {
      if (was_drag) {
        commit_selection();
        clear_selection();
        select_region(click_start_world, click_end_world);
      }
    }
    else if (clicked_node) {
    }
    else if (!clicked_node) {
      if (!was_drag) {
        commit_selection();
        clear_selection();
      }
    }

    clicked_node = nullptr;
  }
#endif
  return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::on_key_up() {
#if 0
  if (keys_posedge[SDL_SCANCODE_Q]) {
    for (auto node : selection) node->toggle_ghost();
  }
  else if (keys_posedge[SDL_SCANCODE_E]) show_edges = !show_edges;
  else if (keys_posedge[SDL_SCANCODE_A]) show_anchors = !show_anchors;
  else if (keys_posedge[SDL_SCANCODE_RETURN]) {
    commit_selection();
    clear_selection();
  }
  else if (keys_posedge[SDL_SCANCODE_ESCAPE]) {
    revert_selection();
    clear_selection();
  }
  else if ((keys_posedge[SDL_SCANCODE_S]) && (keys_old[SDL_SCANCODE_LCTRL])) {
    //printf("Saving gameboy.cell_db.json\n");
    //cell_db.save_json("gameboy.cell_db.json");
    //plait.save("gameboy.plait");
    plait.save_json("gameboy.plait.json");
  }
  else if ((keys_posedge[SDL_SCANCODE_L]) && (keys_old[SDL_SCANCODE_LCTRL])) {
    //printf("Loading gameboy.cell_db.json\n");
    //cell_db.load_json("gameboy.cell_db.json");
    printf("Loading gameboy.plait\n");
    plait.load_json("gameboy.plait", cell_db);
  }
#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitApp::event_to_tool(SDL_Event event) {
  (void)event;

  //----------------------------------------

  if (current_tool == ToolMode::MENU_OPTION) {
    switch(event.type) {
    case SDL_KEYDOWN: {
      int key = event.key.keysym.scancode;
      if (key == SDL_SCANCODE_S) {
        printf("Saving plait\n");
        plait.save_json("gameboy.plait.json");
      }
      return true;
    }
    case SDL_KEYUP: {
      int key = event.key.keysym.scancode;
      if (key == SDL_SCANCODE_LALT) {
        printf("Leaving MENU_OPTION\n");
        current_tool = ToolMode::NONE;
        return true;
      }
    }
    }
  }

  //----------------------------------------

  if (current_tool == ToolMode::SELECT_REGION) {
    switch(event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("event_to_tool(SELECT_REGION, SDL_MOUSEBUTTONDOWN)\n");
        sel_active = true;
        sel_begin = mouse_pos_world;
        sel_end = mouse_pos_world;
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      sel_end = mouse_pos_world;
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      sel_end = mouse_pos_world;

      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("event_to_tool(SELECT_REGION, SDL_MOUSEBUTTONUP)\n");
        sel_active = false;
        select_region(sel_begin, sel_end);
      }
      break;
    }
    case SDL_KEYUP: {
      int key = event.key.keysym.scancode;
      if (key == SDL_SCANCODE_LCTRL) {
        printf("Leaving lock region\n");
        current_tool = ToolMode::NONE;
        return true;
      }
    }
    }
    return true;
  }

  //----------------------------------------

  if (current_tool == ToolMode::LOCK_REGION) {
    switch(event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("event_to_tool(LOCK_REGION, SDL_MOUSEBUTTONDOWN)\n");
        sel_active = true;
        sel_begin = mouse_pos_world;
        sel_end = mouse_pos_world;
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      sel_end = mouse_pos_world;
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      sel_end = mouse_pos_world;

      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("event_to_tool(LOCK_REGION, SDL_MOUSEBUTTONUP)\n");
        sel_active = false;
        lock_region(sel_begin, sel_end);
      }
      break;
    }
    case SDL_KEYUP: {
      int key = event.key.keysym.scancode;
      if (key == SDL_SCANCODE_D) {
        printf("Leaving select region\n");
        current_tool = ToolMode::NONE;
        return true;
      }
    }
    }
    return true;
  }

  //----------------------------------------

  if (current_tool == ToolMode::UNLOCK_REGION) {
    switch(event.type) {
    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("event_to_tool(UNLOCK_REGION, SDL_MOUSEBUTTONDOWN)\n");
        sel_active = true;
        sel_begin = mouse_pos_world;
        sel_end = mouse_pos_world;
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      sel_end = mouse_pos_world;
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      sel_end = mouse_pos_world;

      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("event_to_tool(UNLOCK_REGION, SDL_MOUSEBUTTONUP)\n");
        sel_active = false;
        auto callback = [this](Node* node) {
          node->locked = false;
        };
        apply_region(sel_begin, sel_end, callback);
      }
      break;
    }
    case SDL_KEYUP: {
      int key = event.key.keysym.scancode;
      if (key == SDL_SCANCODE_F) {
        printf("Leaving unlock region\n");
        current_tool = ToolMode::NONE;
        return true;
      }
    }
    }
    return true;
  }

  //----------------------------------------

  if (current_tool == ToolMode::PAN_VIEW) {
    switch(event.type) {
    case SDL_MOUSEMOTION: {
      if (event.motion.state & SDL_BUTTON_LMASK) {
        view_control.pan(event.motion.xrel, event.motion.yrel);
        return true;
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("Leaving PAN_VIEW\n");
        current_tool = ToolMode::NONE;
        return true;
      }
      break;
    }
    }
  }

  //----------------------------------------

  if (current_tool == ToolMode::DRAG_NODE) {
    switch(event.type) {
    case SDL_MOUSEMOTION: {
      if (event.motion.state & SDL_BUTTON_LMASK) {
        dvec2 pos_abs_new = mouse_pos_world + clicked_offset;
        pos_abs_new.x = round(pos_abs_new.x / 16) * 16.0;
        pos_abs_new.y = round(pos_abs_new.y / 16) * 16.0;
        dvec2 delta = pos_abs_new - clicked_node->get_pos_abs_new();
        for (auto node : selection) {
          node->move(delta);
        }

        return true;
      }
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      if (event.button.button & SDL_BUTTON_LMASK) {
        printf("Leaving DRAG_NODE\n");
        current_tool = ToolMode::NONE;
        //clicked_node->commit_pos();
        clicked_node = nullptr;
        return true;
      }
      break;
    }
    }
  }

  //----------------------------------------

#if 0
  int mouse_x = 0, mouse_y = 0;
  uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

  if (mouse_buttons & SDL_BUTTON_LMASK) {
    if (keys_new[SDL_SCANCODE_D]     ) return true;
    if (keys_new[SDL_SCANCODE_F]     ) return true;
    if (keys_new[SDL_SCANCODE_LCTRL] ) return true;
  }

  if (clicked_node != nullptr) return true;

  if (!selection.empty()) return true;
#endif

  return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitApp::event_to_imgui(SDL_Event event) {
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

  return io.WantCaptureKeyboard || io.WantCaptureMouse;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitApp::event_to_view_control(SDL_Event event) {
  (void)event;

#if 0
  switch (event.type) {
    case SDL_MOUSEWHEEL: {
      int mouse_x = 0, mouse_y = 0;
      SDL_GetMouseState(&mouse_x, &mouse_y);
      view_control.on_mouse_wheel(mouse_x, mouse_y, double(event.wheel.y) * 0.25);
      return true;
      break;
    }
    case SDL_KEYDOWN: {
      if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
        view_control.pop_view();
        return true;
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      //printf("event_to_view_control(SDL_MOUSEMOTION)\n");
      if (event.motion.state & SDL_BUTTON_LMASK) {
        view_control.pan(event.motion.xrel, event.motion.yrel);
        return true;
      }
      break;
    }
  }
#endif
  return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitApp::event_to_ui(SDL_Event event) {
  (void)event;

#if 0
  if (event.type == event_mousepos) {
    printf("mousepos event\n");

    int mouse_x = 0, mouse_y = 0;
    uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    mouse_pos_screen = {mouse_x, mouse_y};
    mouse_pos_world = view_control.view_snap.screenToWorld(mouse_pos_screen);

    mouse_buttons_old = mouse_buttons_new;
    mouse_buttons_new = mouse_buttons;
    mouse_buttons_posedge = ~mouse_buttons_old &  mouse_buttons_new;
    mouse_buttons_negedge =  mouse_buttons_old & ~mouse_buttons_new;

    if (mouse_buttons_posedge) printf("mouse_posedge 0x%08x\n", mouse_buttons_posedge);
    if (mouse_buttons_negedge) printf("mouse_negedge 0x%08x\n", mouse_buttons_negedge);

    hovered_node = pick_node(mouse_pos_world, /*ignore_selected*/ false, /*ignore_clicked*/ true, /*ignore_hovered*/ false);
  }
#endif

  switch(event.type) {
  case SDL_MOUSEWHEEL: {
    int mouse_x = 0, mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    view_control.on_mouse_wheel(mouse_x, mouse_y, double(event.wheel.y) * 0.25);
    return true;
    break;
  }


  case SDL_KEYDOWN: {
    int key = event.key.keysym.scancode;

    if (key == SDL_SCANCODE_LALT) {
      printf("Entering MENU_OPTION\n");
      current_tool = ToolMode::MENU_OPTION;
      return true;
    }
    if (key == SDL_SCANCODE_LCTRL) {
      printf("Entering SELECT_REGION\n");
      current_tool = ToolMode::SELECT_REGION;
      return true;
    }
    else if (key == SDL_SCANCODE_D) {
      printf("Entering LOCK_REGION\n");
      current_tool = ToolMode::LOCK_REGION;
      return true;
    }
    else if (key == SDL_SCANCODE_F) {
      printf("Entering UNLOCK_REGION\n");
      current_tool = ToolMode::UNLOCK_REGION;
      return true;
    }
    else if (key == SDL_SCANCODE_ESCAPE) {
      if (selection.size()) {
        printf("Reverting selection\n");
        revert_selection();
        clear_selection();
      }
      else {
        //view_control.pop_view();
      }
      return true;
    }
    break;

  }


  case SDL_MOUSEBUTTONDOWN: {
    if (event.button.button & SDL_BUTTON_LMASK) {
      clicked_node = pick_node(mouse_pos_world, /*ignore_selected*/ false, /*ignore_clicked*/ true, /*ignore_hovered*/ false);
      if (clicked_node) {
        clicked_pos_abs = clicked_node->get_pos_abs_new();
        clicked_offset  = clicked_pos_abs - mouse_pos_world;

        if (clicked_node->selected) {
        }
        else {
          commit_selection();
          clear_selection();
        }

        printf("entering DRAG_NODE\n");
        current_tool = ToolMode::DRAG_NODE;
        select_node(clicked_node);
        drag_begin = mouse_pos_world;

        return true;
      }
      else {
        printf("entering PAN_VIEW\n");
        commit_selection();
        clear_selection();
        current_tool = ToolMode::PAN_VIEW;
        return true;
      }
    }
    break;
  }
  }

  if (event.type == event_mousepos) {
    //printf("event_to_ui(event_mousepos)\n");

    hovered_node = pick_node(mouse_pos_world, /*ignore_selected*/ false, /*ignore_clicked*/ true, /*ignore_hovered*/ false);
  }

  return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_update(double delta_time) {
  (void)delta_time;

  {
    int mouse_x = 0, mouse_y = 0;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    mouse_pos_screen = {mouse_x, mouse_y};
    mouse_pos_world = view_control.view_snap.screenToWorld(mouse_pos_screen);
  }

  {
    ImGuiIO& io = ImGui::GetIO();
    int mouse_x = 0, mouse_y = 0;
    const auto mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
    io.MouseDown[0] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
    io.MouseDown[1] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    io.MousePos = { (float)mouse_x, (float)mouse_y };
  }

  hovered_node = nullptr;

  {
    int mouse_x = 0, mouse_y = 0;
    uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = event_mousepos;
    event.user.code = mouse_buttons;
    event.user.data1 = reinterpret_cast<void*>((int64_t)mouse_x);
    event.user.data2 = reinterpret_cast<void*>((int64_t)mouse_y);
    SDL_PushEvent(&event);
  }

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event_to_tool(event)) continue;
    if (event_to_imgui(event)) continue;
    if (event_to_view_control(event)) continue;
    if (event_to_ui(event)) continue;
  }

  view_control.update(delta_time);

#if 0
  {
    {
      int count = 0;
      const uint8_t* keys = SDL_GetKeyboardState(&count);

      for (int i = 0; i < count; i++) {
        keys_old[i] = keys_new[i];
        keys_new[i] = keys[i];
        keys_posedge[i] = ~keys_old[i] &  keys_new[i];
        keys_negedge[i] =  keys_old[i] & ~keys_new[i];
      }
    }

    on_mouse_down(mouse_buttons_posedge);
    on_mouse_up(mouse_buttons_negedge);
    on_key_up();
  }

  //----------------------------------------

  if (keys_new[SDL_SCANCODE_LCTRL]) {
  }
  else if (keys_new[SDL_SCANCODE_F]) {
  }
  else if (mouse_buttons_new & SDL_BUTTON_LMASK && clicked_node) {
    for (auto selected_node : selection) {
      if (selected_node->root_anchor_is_selected()) continue;

      dvec2 new_pos = selected_node->get_pos_old() + (mouse_pos_world - click_start_world);
      new_pos.x = round(new_pos.x / 16) * 16.0;
      new_pos.y = round(new_pos.y / 16) * 16.0;
      selected_node->set_pos_new(new_pos);
    }
  }

#endif

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

  /*
  {
    ImGuiIO& io = ImGui::GetIO();
    printf("capture %d %d\n", io.WantCaptureKeyboard, io.WantCaptureMouse);
  }
  */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::draw_node(Node* node) {
  dvec2 node_pos_old = node->get_pos_abs_old();
  dvec2 node_pos_new = node->get_pos_abs_new();

  const dvec2 node_size = {128,64};
  const dvec2 port_size = {4,4};

  size_t port_in_count = node->prev.size();
  size_t port_out_count = 1;

  //----------------------------------------

  // Node outline
  {
    uint32_t color = 0xFF808080;
    if (node->selected) color = 0xFFCCCCCC;
    if (node->floating) color = 0xFF0000FF;

    outline_painter.push_box(node_pos_new, node_pos_new + node_size, color);
  }

  // Node fill
  if (!node->ghost) {
    box_painter.push_corner_size(
      node_pos_new + dvec2(4,4),
      node_size - dvec2(8,8),
      node == hovered_node ? node->color + COL_HINT3 : node->color);
  }

  // Node text
  text_painter.add_text_at(node->name(), float(node_pos_new.x + 8), float(node_pos_new.y + 8));
  text_painter.add_text_at(node->gate(), float(node_pos_new.x + 8), float(node_pos_new.y + 24));
  if (node->locked) text_painter.add_text_at("LOCKED", float(node_pos_new.x + 8), float(node_pos_new.y + 40));

  // Node input port(s)
  {
    double stride = (node_size.y) / (port_in_count + 1);

    for (size_t i = 0; i < port_in_count; i++) {
      dvec2 port_pos = node_pos_new + dvec2(0, stride * (i + 1));
      port_painter.push_center_size(port_pos, port_size, 0xCC008000);
    }
  }

  // Node output port(s)
  {
    double stride = (node_size.y) / (port_out_count + 1);

    for (size_t i = 0; i < port_out_count; i++) {
      dvec2 port_pos = node_pos_new + dvec2(node_size.x, stride * (i + 1));
      port_painter.push_center_size(port_pos, port_size, 0xCC000080);
    }
  }

  // Edges from previous node(s)
  if (show_edges && !node->ghost) {
    double stride = (node_size.y) / (port_in_count + 1);

    for (size_t i = 0; i < port_in_count; i++) {
      auto prev = node->prev[i];
      if (prev == nullptr) continue;
      if (prev->ghost) continue;

      auto prev_pos_new = prev->get_pos_abs_new();

      // Highlight "backwards" edges in red.
      bool edge_backwards = prev_pos_new.x > node_pos_new.x;

      uint32_t color_a = edge_backwards ? 0xFF0000FF : 0x40FFFFFF;
      uint32_t color_b = edge_backwards ? 0xFF0000FF : 0x4044FF44;

      // Make edges connected to selected nodes opaque.
      if (node->selected || prev->selected) {
        if (edge_backwards) {
          color_a = 0xFF8080FF;
          color_b = 0xFF8080FF;
        }
        else {
          color_a |= 0xFF000000;
          color_b |= 0xFF000000;
        }
      }

      dvec2 port_prev = prev_pos_new + dvec2(node_size.x, node_size.y / 2);
      dvec2 port_next = node_pos_new + dvec2(0, stride * (i + 1));

      edge_painter.push(port_prev, color_a, port_next, color_b);
    }
  }

  // Anchor edge
  if (show_anchors && node->anchored()) {
    dvec2 center_a = node->get_anchor()->get_pos_abs_new() + node_size / 2.0;
    dvec2 center_b = node->get_pos_abs_new() + node_size / 2.0;
    edge_painter.push(center_a, 0x80FFFFFF, center_b, 0x80FF4040);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_frame() {

  //int mouse_x = 0, mouse_y = 0;
  //uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
  text_painter.bg_col = {0.0,0.0,0.0,0.3};
  const dvec2 node_size = {128,64};

  //----------------------------------------
  // Grid layer

  grid_painter.render(view_control.view_snap);

  //----------------------------------------
  // Node layer

  // Node shadows
  for (auto node : selection) {
    dvec2 node_pos_old = node->get_pos_abs_old();
    outline_painter.push_box(node_pos_old, node_pos_old + node_size, 0xFF404040);
  }
  outline_painter.render(view_control.view_snap, 0, 0, 1);

  for (auto& [tag, node] : plait.node_map) {
    if (!node->selected) draw_node(node);
  }

  outline_painter.render(view_control.view_snap, 0, 0, 1);
  box_painter.render(view_control.view_snap, 0, 0, 1);
  port_painter.render(view_control.view_snap, 0, 0, 1);
  edge_painter.render(view_control.view_snap, 0, 0, 1);
  text_painter.render(view_control.view_snap, 0, 0, 1);


  for (auto node : selection) {
    draw_node(node);
  }

  outline_painter.render(view_control.view_snap, 0, 0, 1);
  box_painter.render(view_control.view_snap, 0, 0, 1);
  port_painter.render(view_control.view_snap, 0, 0, 1);
  edge_painter.render(view_control.view_snap, 0, 0, 1);
  text_painter.render(view_control.view_snap, 0, 0, 1);

  //----------------------------------------
  // UI layer

#if 0
  // Draw in-progress anchor edges

  if (keys_new[SDL_SCANCODE_F] && clicked_node) {
    if (selection.size()) {
      for (auto selected_node : selection) {
        edge_painter.push(mouse_pos_world, 0xFFFFFFFF, selected_node->get_pos_new() + node_size * 0.5, 0xFFFF8080);
      }
    }

    edge_painter.push(mouse_pos_world, 0xFFFFFFFF, clicked_node->get_pos_new() + node_size * 0.5, 0xFFFF8080);
  }
#endif

  // Draw selection rect

  /*
  uint32_t selection_rect_color = 0x00000000;

  if (keys_new[SDL_SCANCODE_D])      selection_rect_color = 0xCC80FFFF;
  if (keys_new[SDL_SCANCODE_LCTRL])  selection_rect_color = 0xCCFFFFFF;
  */

  if (current_tool == ToolMode::SELECT_REGION && sel_active) {
    outline_painter.push_box(sel_begin, sel_end, 0xFFFFFFFF);
  }

  if (current_tool == ToolMode::LOCK_REGION && sel_active) {
    outline_painter.push_box(sel_begin, sel_end, 0xFF00FFFF);
  }

  if (current_tool == ToolMode::UNLOCK_REGION && sel_active) {
    outline_painter.push_box(sel_begin, sel_end, 0xFFFFFF00);
  }

  outline_painter.render(view_control.view_snap, 0, 0, 1);
  box_painter.render(view_control.view_snap, 0, 0, 1);
  port_painter.render(view_control.view_snap, 0, 0, 1);
  edge_painter.render(view_control.view_snap, 0, 0, 1);
  text_painter.render(view_control.view_snap, 0, 0, 1);

  frame_count++;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_ui() {

  // Draw selection info
  {
    StringDumper d;
    d("Tool mode %s\n", tool_to_string[current_tool].c_str());
    d("Selected nodes : ");
    for (auto selected_node : selection) d("%s ", selected_node->name());
    d("\n");
    d("Clicked node : %s\n", clicked_node ? clicked_node->name() : "<none>");
    d("Hovered node : %s\n", hovered_node ? hovered_node->name() : "<none>");
    text_painter.add_text_at(d.c_str(), 0, 0);
  }

  text_painter.render(view_control.view_screen, 0, 0, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
