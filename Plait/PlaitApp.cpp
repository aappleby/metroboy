#include "Plait/PlaitApp.h"
#include "AppLib/GLBase.h"

#include "AppLib/AppHost.h"
#include "AppLib/GLBase.h"

#include "Plait/CellDB.h"

#define SDL_MAIN_HANDLED
#ifdef _MSC_VER
#include "SDL/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <deque>
#include <algorithm>
#include <filesystem>

//#include <windows.h>

using namespace std;

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  int ret = 0;

  PlaitApp* app = new PlaitApp();

  //app->cell_db.parse_dir("GateBoyLib");
  printf("Loading gameboy.cell_db.json");
  app->cell_db.load_json("gameboy.cell_db.json");

  app->plait.load("gameboy.plait", app->cell_db);


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

void PlaitApp::app_init() {
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

  keyboard_count = 0;
  keyboard_state = SDL_GetKeyboardState(&keyboard_count);
  old_keys.resize(keyboard_count, 0);
  new_keys.resize(keyboard_count, 0);

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

    dvec2 node_pos = node->get_pos_new();

    int width = 128;
    int height = 64;

    if (_mouse_pos.x >= node_pos.x &&
        _mouse_pos.y >= node_pos.y &&
        _mouse_pos.x <= node_pos.x + width &&
        _mouse_pos.y <= node_pos.y + height) {
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
    dvec2 nmin = node->get_pos_new();
    dvec2 nmax = node->get_pos_new() + node_size;

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
  dvec2 offset = b->get_pos_new() - a->get_pos_new();
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
  dvec2 offset = b->get_pos_new() - a->get_pos_new();
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

void PlaitApp::app_update(Viewport view, double delta_time) {
  (void)view;
  (void)delta_time;

  int mouse_x = 0, mouse_y = 0;
  uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
  mouse_pos_screen = {mouse_x, mouse_y};
  mouse_pos_world = view.screenToWorld(mouse_pos_screen);
  hovered_node = pick_node(mouse_pos_world, /*ignore_selected*/ false, /*ignore_clicked*/ true, /*ignore_hovered*/ false);

  //----------------------------------------

  SDL_Event event;
  while (SDL_PollEvent(&event)) switch (event.type) {

    //----------------------------------------

    case SDL_KEYDOWN: {
      int key = event.key.keysym.scancode;
      int mod = event.key.keysym.mod;

      if (key == SDL_SCANCODE_E) show_edges = !show_edges;
      if (key == SDL_SCANCODE_A) show_anchors = !show_anchors;
      if (key == SDL_SCANCODE_RETURN) {
        commit_selection();
        clear_selection();
      }
      if (key == SDL_SCANCODE_ESCAPE) {
        revert_selection();
        clear_selection();
      }

      if ((key == SDL_SCANCODE_S) && (mod & KMOD_CTRL)) {
        //printf("Saving gameboy.cell_db.json\n");
        //cell_db.save_json("gameboy.cell_db.json");
        //plait.save("gameboy.plait");
        plait.save_json("gameboy.plait.json");
      }

      if ((key == SDL_SCANCODE_L) && (mod & KMOD_CTRL)) {
        //printf("Loading gameboy.cell_db.json\n");
        //cell_db.load_json("gameboy.cell_db.json");
        printf("Loading gameboy.plait\n");
        plait.load("gameboy.plait", cell_db);
      }

      break;
    }

    //----------------------------------------

    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.clicks != 1) {
        break;
      }

      //printf("Click start at %d %d\n", event.button.x, event.button.y);

      memcpy(old_keys.data(), keyboard_state, keyboard_count);
      memset(new_keys.data(), 0, keyboard_count);

      printf("\n");
      click_start_screen = {event.button.x, event.button.y};
      click_start_world = view.screenToWorld(click_start_screen);
      clicked_node = pick_node(click_start_world, /*ignore_selected*/ false, /*ignore_clicked*/ false, /*ignore_hovered*/ false);

      if (old_keys[SDL_SCANCODE_D]) {
        if (!clicked_node) {
          printf("D-clicked on nothing\n");
        }
        else if (clicked_node->selected) {
          printf("D-clicked on selected node %s\n", clicked_node->name());
          clicked_node->toggle_lock();
        }
        else {
          printf("D-clicked on unselected node %s\n", clicked_node->name());
          clicked_node->toggle_lock();
        }
      }
      if (old_keys[SDL_SCANCODE_LCTRL]) {
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
      else if (old_keys[SDL_SCANCODE_F]) {
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

      break;
    }

    //----------------------------------------

    case SDL_MOUSEBUTTONUP: {
      //printf("Click end at %d %d\n", event.button.x, event.button.y);

      memcpy(new_keys.data(), keyboard_state, keyboard_count);
      click_end_screen = {event.button.x, event.button.y};
      click_end_world = view.screenToWorld(click_end_screen);
      auto end_node = pick_node(click_end_world, /*ignore_selected*/ false, /*ignore_clicked*/ false, /*ignore_hovered*/ false);

      bool was_drag = length(click_end_screen - click_start_screen) > 3;

      if (old_keys[SDL_SCANCODE_D]) {
        if (was_drag) {
          lock_region(click_start_world, click_end_world);
        }
      }
      if (old_keys[SDL_SCANCODE_F]) {
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
      else if (old_keys[SDL_SCANCODE_LCTRL] && new_keys[SDL_SCANCODE_LCTRL]) {
        if (was_drag) {
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

      memset(old_keys.data(), 0, keyboard_count);
      clicked_node = nullptr;
      break;
    }

    //----------------------------------------

    case SDL_MOUSEMOTION: {

      if (keyboard_state[SDL_SCANCODE_LCTRL]) {
      }
      else if (keyboard_state[SDL_SCANCODE_F]) {
      }
      else if (mouse_buttons & SDL_BUTTON_LMASK && clicked_node) {
        for (auto selected_node : selection) {
          dvec2 new_pos = selected_node->get_pos_old() + (mouse_pos_world - click_start_world);
          new_pos.x = round(new_pos.x / 16) * 16.0;
          new_pos.y = round(new_pos.y / 16) * 16.0;
          selected_node->set_pos_new(new_pos);
        }
      }

      break;
    }

    //----------------------------------------

  } // while (SDL_PollEvent(&event)) switch (event.type)

  //----------------------------------------
  // Pull nodes towards their parents/children

#if 1
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

#if 1
  for (auto& [tag, node] : plait.node_map) {
    if (node->selected || node->locked || node->anchored()) {
    }
    else {
      node->set_pos_new(node->get_pos_old() + node->spring_force * 0.01);
      node->commit_pos();
    }
    node->spring_force = {0,0};
  }
#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::draw_node(Node* node) {
  dvec2 node_pos_new = node->get_pos_new();
  const dvec2 node_size = {128,64};
  const dvec2 port_size = {4,4};

  outline_painter.push_box(node_pos_new, node_pos_new + node_size, node->selected ? 0xFFCCCCCC : 0xFF808080);

  box_painter.push_corner_size(
    node_pos_new + dvec2(4,4),
    node_size - dvec2(8,8),
    node == hovered_node ? node->color + COL_HINT3 : node->color);

  text_painter.add_text_at(node->name(), float(node_pos_new.x + 8), float(node_pos_new.y + 8));
  text_painter.add_text_at(node->gate(), float(node_pos_new.x + 8), float(node_pos_new.y + 24));
  if (node->locked) text_painter.add_text_at("LOCKED", float(node_pos_new.x + 8), float(node_pos_new.y + 40));

  // input port(s)
  {
    size_t port_in_count = node->prev.size();
    double stride = (node_size.y) / (port_in_count + 1);

    for (size_t i = 0; i < port_in_count; i++) {
      dvec2 port_pos = node_pos_new + dvec2(0, stride * (i + 1));
      port_painter.push_center_size(port_pos, port_size, 0xCC008000);
    }
  }

  // output port(s)
  {
    size_t port_out_count = 1;
    double stride = (node_size.y) / (port_out_count + 1);

    for (size_t i = 0; i < port_out_count; i++) {
      dvec2 port_pos = node_pos_new + dvec2(node_size.x, stride * (i + 1));
      port_painter.push_center_size(port_pos, port_size, 0xCC000080);
    }
  }

  // edges from previous node(s)
  if (show_edges) {
    size_t port_in_count = node->prev.size();
    double stride = (node_size.y) / (port_in_count + 1);

    for (size_t i = 0; i < port_in_count; i++) {
      auto prev = node->prev[i];
      if (prev == nullptr) continue;

      uint32_t color_a = (node->selected || prev->selected) ? 0xFF8888FF : 0x800000FF;
      uint32_t color_b = (node->selected || prev->selected) ? 0xFF88FF88 : 0x8000FF00;

      dvec2 port_prev = prev->get_pos_new() + dvec2(node_size.x, node_size.y / 2);
      dvec2 port_next = node->get_pos_new() + dvec2(0, stride * (i + 1));

      edge_painter.push(port_prev, color_a, port_next, color_b);
    }
  }

  // anchor edge
  if (show_anchors && node->get_anchor()) {
    dvec2 center_a = node->get_anchor()->get_pos_new() + node_size / 2.0;
    dvec2 center_b = node->get_pos_new() + node_size / 2.0;
    edge_painter.push(center_a, 0x80FFFFFF, center_b, 0x80FF4040);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_frame(Viewport view_world) {
  int mouse_x = 0, mouse_y = 0;
  uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
  text_painter.bg_col = {0.0,0.0,0.0,0.3};
  const dvec2 node_size = {128,64};

  //----------------------------------------
  // Grid layer

  grid_painter.render(view_world);

  //----------------------------------------
  // Selected node shadows

  for (auto node : selection) {
    dvec2 node_pos_old = node->get_pos_old();
    outline_painter.push_box(node_pos_old, node_pos_old + node_size, 0xFF404040);
  }
  outline_painter.render(view_world, 0, 0, 1);

  //----------------------------------------
  // Node layer

  for (auto& [tag, node] : plait.node_map) {
    draw_node(node);
  }

  port_painter.render(view_world, 0, 0, 1);
  edge_painter.render(view_world, 0, 0, 1);
  box_painter.render(view_world, 0, 0, 1);
  text_painter.render(view_world, 0, 0, 1);
  outline_painter.render(view_world, 0, 0, 1);

  //----------------------------------------
  // UI layer

  // Draw in-progress anchor edges

  if (keyboard_state[SDL_SCANCODE_F] && clicked_node) {
    if (selection.size()) {
      for (auto selected_node : selection) {
        edge_painter.push(mouse_pos_world, 0xFFFFFFFF, selected_node->get_pos_new() + node_size * 0.5, 0xFFFF8080);
      }
    }

    if (clicked_node) {
      edge_painter.push(mouse_pos_world, 0xFFFFFFFF, clicked_node->get_pos_new() + node_size * 0.5, 0xFFFF8080);
    }
  }

  // Draw selection rect

  uint32_t selection_rect_color = 0x00000000;

  if (old_keys[SDL_SCANCODE_D]      && keyboard_state[SDL_SCANCODE_D])      selection_rect_color = 0xCC80FFFF;
  if (old_keys[SDL_SCANCODE_LCTRL]  && keyboard_state[SDL_SCANCODE_LCTRL])  selection_rect_color = 0xCCFFFFFF;
  if (old_keys[SDL_SCANCODE_LSHIFT] && keyboard_state[SDL_SCANCODE_LSHIFT]) selection_rect_color = 0xCC8080FF;

  if (selection_rect_color && (mouse_buttons & SDL_BUTTON_LMASK)) {
    outline_painter.push_box(
      click_start_world,
      view_world.screenToWorld({mouse_x, mouse_y}),
      selection_rect_color);
  }

  port_painter.render(view_world, 0, 0, 1);
  edge_painter.render(view_world, 0, 0, 1);
  box_painter.render(view_world, 0, 0, 1);
  text_painter.render(view_world, 0, 0, 1);
  outline_painter.render(view_world, 0, 0, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void PlaitApp::app_render_ui(Viewport view_screen) {
  // Draw selection info
  {
    StringDumper d;
    d("Selected nodes : ");
    for (auto selected_node : selection) d("%s ", selected_node->name());
    d("\n");
    d("Clicked node : %s\n", clicked_node ? clicked_node->name() : "<none>");
    d("Hovered node : %s\n", hovered_node ? hovered_node->name() : "<none>");
    text_painter.add_text_at(d.c_str(), 0, 0);
  }

  text_painter.render(view_screen, 0, 0, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitApp::is_mouse_locked() const {
  int mouse_x = 0, mouse_y = 0;
  uint32_t mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

  if (mouse_buttons & SDL_BUTTON_LMASK) {
    if (old_keys[SDL_SCANCODE_D]     ) return true;
    if (old_keys[SDL_SCANCODE_F]     ) return true;
    if (old_keys[SDL_SCANCODE_LCTRL] ) return true;
    if (old_keys[SDL_SCANCODE_LSHIFT]) return true;
    if (old_keys[SDL_SCANCODE_LALT]  ) return true;
    if (keyboard_state[SDL_SCANCODE_D]     ) return true;
    if (keyboard_state[SDL_SCANCODE_F]     ) return true;
    if (keyboard_state[SDL_SCANCODE_LCTRL] ) return true;
    if (keyboard_state[SDL_SCANCODE_LSHIFT]) return true;
    if (keyboard_state[SDL_SCANCODE_LALT]  ) return true;
  }

  return clicked_node != nullptr;
}

//--------------------------------------------------------------------------------

bool PlaitApp::is_keyboard_locked() const {
  return !selection.empty();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
