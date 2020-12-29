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

#include <windows.h>

using namespace std;

//-----------------------------------------------------------------------------

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  int ret = 0;

  PlaitApp* app = new PlaitApp();

  app->cell_db.parse_dir("GateBoyLib");

  AppHost* app_host = new AppHost(app);
  ret = app_host->app_main(argc, argv);
  delete app;

  return ret;
}

//-----------------------------------------------------------------------------

PlaitApp::~PlaitApp() {
};

const char* PlaitApp::app_get_title() {
  return "PlaitApp";
}

//-----------------------------------------------------------------------------

void PlaitApp::app_init() {
  auto& n2c = node_type_to_color;

  n2c[""]      = 0xFF008000;

  n2c["not1"]  = 0xFF808080;

  n2c["and2"]  = 0xFF000080;
  n2c["and3"]  = 0xFF000080;
  n2c["and4"]  = 0xFF000080;
  n2c["nand2"] = 0xFF000080;
  n2c["nand3"] = 0xFF000080;
  n2c["nand4"] = 0xFF000080;
  n2c["nand5"] = 0xFF000080;
  n2c["nand6"] = 0xFF000080;
  n2c["nand7"] = 0xFF000080;

  n2c["or2"]   = 0xFF800000;
  n2c["or3"]   = 0xFF800000;
  n2c["or4"]   = 0xFF800000;

  n2c["and_or3"] = 0xFF800080;
  n2c["or_and3"] = 0xFF800080;
  n2c["not_or_and3"] = 0xFF800080;

  n2c["add_s"] = 0xFF008000;
  n2c["add_c"] = 0xFF008000;

  n2c["nor2"]  = 0xFF800000;
  n2c["nor3"]  = 0xFF800000;
  n2c["nor4"]  = 0xFF800000;
  n2c["nor5"]  = 0xFF800000;
  n2c["nor6"]  = 0xFF800000;
  n2c["nor7"]  = 0xFF800000;
  n2c["nor8"]  = 0xFF800000;

  n2c["xor2"]  = 0xFF808000;
  n2c["xnor2"] = 0xFF808000;

  n2c["mux2n"] = 0xFF004080;
  n2c["mux2p"] = 0xFF004080;
  n2c["amux2"] = 0xFF004080;
  n2c["amux4"] = 0xFF004080;

  n2c["dff9" ] = 0xFF004040;
  n2c["dff22"] = 0xFF004040;
  n2c["dff17"] = 0xFF004040;
  n2c["dff20"] = 0xFF004040;
  n2c["dff17_any"] = 0xFF004040;
  n2c["dff13"] = 0xFF004040;
  n2c["dff8n"] = 0xFF004040;
  n2c["dff11"] = 0xFF004040;
  n2c["dff8p"] = 0xFF004040;

  n2c["tri10_np"] = 0xFF008080;
  n2c["tri6_pn"]  = 0xFF008080;
  n2c["tri6_nn"]  = 0xFF008080;

  n2c["nand_latch"] = 0xFF0060B0;
  n2c["nor_latch" ] = 0xFF0060B0;
  n2c["tp_latchn" ] = 0xFF0060B0;
  n2c["tp_latchp" ] = 0xFF0060B0;

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

  keyboard_state = SDL_GetKeyboardState(nullptr);

  int cursor_x = 0;
  int cursor_y = 0;

  for (auto& [tag, cell] : cell_db.cell_map) {
    auto node = new Node();
    cell->node = node;
    node->cell = cell;
  }

  for (auto& [tag, cell] : cell_db.cell_map) {
    auto node = (Node*)cell->node;

    cursor_x = rand() * 2 - 32768;
    cursor_y = rand() * 2 - 32768;

    node->set_pos_old({cursor_x, cursor_y});
    node->set_pos_new({cursor_x, cursor_y});
    node->color = 0xFFFF00FF;

    auto it = node_type_to_color.find(cell->gate);
    if (it != node_type_to_color.end()) node->color = (*it).second;

    for (auto& port : cell->args) {
      if (cell_db.has_cell(port.tag)) {
        auto prev_cell = cell_db.get_cell(port.tag);
        auto prev_node = (Node*)prev_cell->node;
        if (prev_node) {
          node->prev.push_back(prev_node);
          prev_node->next.push_back(node);
        }
      }
    }

    plait.nodes.push_back(node);

    cursor_x += 128 + 16;
    if (cursor_x >= 4096) {
      cursor_x = 0;
      cursor_y += 80;
    }
  }


#if 0
  Node test_nodes[] = {
    { { 256,  256},  { 256,  256}, "+====\\\node|REPU >\node+====/"},
    { { 20,  20},  { 20,  20}, "SARY"},
    { { 30,  30},  { 30,  30}, "REJO"},
    { { 40,  40},  { 40,  40}, "XOFO"},
    { { 50,  50},  { 50,  50}, "NUNU"},
    { { 60,  60},  { 60,  60}, "PYNU"},
    { { 70,  70},  { 70,  70}, "NOPA"},
    { { 80,  80},  { 80,  80}, "NAFY"},
    { { 90,  90},  { 90,  90}, "PYGO"},
    { {128, 128},  {128, 128}, "PORY"},
  };

  for (int i = 0; i < 10; i++) {
    Node* new_node = new Node();
    new_node->name = test_nodes[i].name;
    new_node->pos_old = test_nodes[i].pos_old;
    new_node->pos_new = test_nodes[i].pos_new;

    plait.nodes.push_back(new_node);
    plait.names[new_node->name] = new_node;
  }
#endif

  // Hook up next pointers
  /*
  for (auto next : plait.nodes) {
    for (auto prev : next->prev) {
      prev->next.push_back(next);
    }
  }
  */

  // Mark levels
  /*
  deque<Node*> queue;
  for (auto& node : plait.nodes) {
    node->mark = 0;
    if (node->prev.empty()) {
      node->rank = 0;
      plait.roots.insert(node);
      queue.push_back(node);
    }
    else {
      node->rank = -1;
    }
  }

  plait.update_rank();
  */

  // Initial placement
  /*
  vector<int> cursor_y(100, 0);

  for (auto& node : plait.nodes) {
    if (node->rank < 0) __debugbreak();

    node->pos_old.x = node->rank * 128;
    node->pos_old.y = cursor_y[node->rank] * 128;
    cursor_y[node->rank]++;
  }

  for (auto& node : plait.nodes) node->pos_new = node->pos_old;
  */

  printf("Done %f\node", timestamp());
}

//-----------------------------------------------------------------------------

Node* PlaitApp::pick_node(dvec2 _mouse_pos) {
  for (auto n : plait.nodes) {
    if (n == hit_node) continue;
    dvec2 node_pos = n->get_pos_old();

    int width = 128;
    int height = 64;

    if (_mouse_pos.x >= node_pos.x &&
        _mouse_pos.y >= node_pos.y &&
        _mouse_pos.x <= node_pos.x + width &&
        _mouse_pos.y <= node_pos.y + height) {
      return n;
    }
  }

  return nullptr;
}

//-----------------------------------------------------------------------------

void PlaitApp::app_close() {
}

//-----------------------------------------------------------------------------

void PlaitApp::app_update(Viewport view, double delta_time) {
  (void)view;
  (void)delta_time;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (keyboard_state[SDL_SCANCODE_LSHIFT]) continue;

    switch (event.type) {
    case SDL_KEYDOWN: {
      int key = event.key.keysym.scancode;
      if (key == SDL_SCANCODE_E) show_edges = !show_edges;
      if (key == SDL_SCANCODE_A) show_anchors = !show_anchors;
      break;
    }

    case SDL_MOUSEBUTTONDOWN: {
      if (event.button.clicks == 1) {
        click_start = view.screenToWorld({event.button.x, event.button.y});
      }
      hit_node = pick_node(click_start);
      break;
    }

    case SDL_MOUSEBUTTONUP: {
      click_end = view.screenToWorld({event.button.x, event.button.y});
      if (hit_node) {
        if (keyboard_state[SDL_SCANCODE_F]) {
          if (hover_node) {
            printf("Linking %s to %s\n", hit_node->cell->tag.c_str(), hover_node->cell->tag.c_str());
            hit_node->set_anchor(hover_node);
          }
          else {
            printf("Unlinking %s\n", hit_node->cell->tag.c_str());
            hit_node->set_anchor(nullptr);
          }
        }
        hit_node->set_pos_old(hit_node->get_pos_new());
        hit_node = nullptr;
      }

      if (event.button.clicks == 2) {
        auto node = pick_node(click_end);
        if (node) {
          node->locked = !node->locked;
        }
      }
      break;
    }

    case SDL_MOUSEMOTION: {
      mouse_pos = view.screenToWorld({event.motion.x, event.motion.y});

      hover_node = pick_node(mouse_pos);

      if (hit_node) {
        if (keyboard_state[SDL_SCANCODE_F]) {
        }
        else {
          dvec2 new_pos = hit_node->get_pos_old() + (mouse_pos - click_start);
          new_pos.x = round(new_pos.x / 16) * 16.0;
          new_pos.y = round(new_pos.y / 16) * 16.0;

          hit_node->set_pos_new(new_pos);
        }
      }

      break;
    }
    }
  }

  // Pull nodes towards their parents

  for (auto node : plait.nodes) {
    if (node == hit_node) continue;
    if (node->locked) continue;
    if (node->anchored()) continue;
    dvec2 d = {0,0};
    int active_springs = 0;

    for (auto prev : node->prev) {
      if (prev->anchored_to(node)) continue;
      dvec2 offset = prev->get_pos_new() - node->get_pos_new();
      if (length(offset) < 512) continue;
      active_springs++;
      d += offset;
    }

    if (active_springs) {
      d /= double(active_springs);
      node->set_pos_new(node->get_pos_old() + d * 0.01);
    }
    node->commit_pos();
  }

#if 0
  // Push nodes away from their siblings
  for (auto node : plait.nodes) {
    if (node == hit_node) continue;
    if (node->locked) continue;

    size_t sib_count = node->prev.size();

    for (size_t i = 0; i < sib_count; i++) {
      auto sib_a = node->prev[(i - 1 + sib_count) % sib_count];
      auto sib_b = node->prev[(i + 1) % sib_count];

      dvec2 delta_a = sib_a->pos_new - node->pos_new;
      dvec2 delta_b = sib_b->pos_new - node->pos_new;
    }
  }
#endif

}

//-----------------------------------------------------------------------------

void PlaitApp::app_render_frame(Viewport view) {
  grid_painter.render(view);

  text_painter.bg_col = {0,0.5,0,0.5};

  const dvec2 node_size = {128,64};
  const dvec2 port_size = {4,4};

  for (auto node : plait.nodes) {
    dvec2 node_pos = node->get_pos_new();

    outline_painter.push_box(node_pos, node_pos + node_size, 0xFF808080);

    box_painter.push_corner_size(
      node_pos + dvec2(4,4),
      node_size - dvec2(8,8),
      node == hover_node ? 0xFF00FF00 : node->color);

    text_painter.add_text_at(node->cell->tag.c_str(),  int(node_pos.x + 8), int(node_pos.y + 8));
    text_painter.add_text_at(node->cell->gate.c_str(), int(node_pos.x + 8), int(node_pos.y + 24));
    if (node->locked) text_painter.add_text_at("LOCKED", int(node_pos.x + 8), int(node_pos.y + 40));

    if (hit_node && keyboard_state[SDL_SCANCODE_F]) {
      edge_painter.push(mouse_pos, 0xFFFFFF, hit_node->get_pos_new() + node_size * 0.5, 0xFFFF8080);
    }

#if 1
    // input port(s)
    {
      const int port_in_count = int(node->prev.size());
      auto gap = (node_size.y - (port_size.y * port_in_count)) / float(port_in_count + 1);
      auto stride = gap + port_size.y;

      for (size_t i = 0; i < port_in_count; i++) {
        dvec2 port_pos = node->get_pos_new() + dvec2(0, gap + stride * i);
        port_painter.push_center_size(port_pos, port_size, 0x80808080);
      }
    }
#endif

#if 1
    // output port(s)
    {
      const int port_out_count = 1;
      auto gap = (node_size.y - (port_size.y * port_out_count)) / float(port_out_count + 1);
      auto stride = gap + port_size.y;

      for (size_t i = 0; i < port_out_count; i++) {
        dvec2 port_pos = node_pos + dvec2(node_size.x, gap + stride * i);
        port_painter.push_center_size(port_pos, port_size, 0x80008000);
      }
    }
#endif

#if 1
    // edges from previous node(s)
    if (show_edges) {
      const int port_in_count = int(node->prev.size());
      auto gap = (node_size.y - (port_size.y * port_in_count)) / float(port_in_count + 1);
      auto stride = gap + port_size.y;

      for (size_t i = 0; i < port_in_count; i++) {
        auto prev = node->prev[i];
        if (prev == nullptr) continue;

        dvec2 port_prev = prev->get_pos_new() + dvec2(node_size.x, node_size.y / 2);
        dvec2 port_next = node->get_pos_new() + dvec2(0, gap + stride * i);

        edge_painter.push(port_prev, 0x800000FF, port_next, 0x8000FF00);
      }
    }
#endif

    // anchor edge
    if (show_anchors && node->get_anchor()) {
      dvec2 center_a = node->get_anchor()->get_pos_new() + node_size / 2.0;
      dvec2 center_b = node->get_pos_new() + node_size / 2.0;
      edge_painter.push(center_a, 0x80FFFFFF, center_b, 0x80FF4040);

    }
  }

  port_painter.render(view, 0, 0, 1);
  edge_painter.render(view, 0, 0, 1);
  box_painter.render(view, 0, 0, 1);
  text_painter.render(view, 0, 0, 1);
  outline_painter.render(view, 0, 0, 1);
}

//-----------------------------------------------------------------------------

void PlaitApp::app_render_ui(Viewport view) {
  (void)view;
}

//-----------------------------------------------------------------------------
