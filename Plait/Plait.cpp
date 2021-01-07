#include "Plait/Plait.h"

#include <iostream>
#include <fstream>

#include "json/single_include/nlohmann/json.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

bool PlaitNode::anchored_to(PlaitNode* target) {
  for (auto cursor = anchor; cursor; cursor = cursor->anchor) {
    if (cursor == target) return true;
  }
  return false;
}

void PlaitNode::set_anchor(PlaitNode* new_anchor) {
  CHECK_P(new_anchor == nullptr || pinned);
  CHECK_P(new_anchor == nullptr || new_anchor->pinned);

  // Ignore invalid anchors, or anchors that would create a loop.
  if (this == new_anchor) return;
  if (anchor == new_anchor) return;
  if (new_anchor && new_anchor->anchored_to(this)) return;

  // Unlink from old anchor.
  if (anchor) {
    pos_rel += anchor->get_pos_abs_old();
    anchor = nullptr;
  }

  // Link to new anchor.
  if (new_anchor) {
    pos_rel -= new_anchor->get_pos_abs_old();
    anchor = new_anchor;
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

#if 0
  void update_rank1() {
    mark = 1;
    rank = 0;
    for(auto p : prev) {
      if (p->mark) continue;
      if (p->rank == -1) p->update_rank1();
      if (p->rank >= rank) {
        rank = p->rank + 1;
      }
    }
    mark = 0;
  }

  void update_rank2() {
    if (prev.empty() && next.empty()) {
      rank = 0;
    }
    else if (prev.empty()) {
      int min_next = 1000000;
      for (auto n : next) if (n->rank < min_next) min_next = n->rank;
      rank = min_next - 1;
    }
    else if (next.empty()) {
      int max_prev = -1;
      for (auto p : prev) if (p->rank > max_prev) max_prev = p->rank;
      rank = max_prev + 1;
    }
    else {
      int max_prev = -1;
      for (auto p : prev) if (p->rank > max_prev) max_prev = p->rank;
      int min_next = 1000000;
      for (auto n : next) if (n->rank < min_next) min_next = n->rank;

      rank = (max_prev + min_next) / 2;
    }
  }
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::save_json(const char* filename) {
  printf("Saving plait %s\n", filename);
  using namespace nlohmann;

  json root;

  for (auto& [tag, group] : tag_to_group) {
    auto& jnode = root[tag];
    group->nodes[0]->commit_pos();
    jnode["locked"]     = group->nodes[0]->locked;
    jnode["pos_rel_x"]  = group->nodes[0]->pos_rel.x;
    jnode["pos_rel_y"]  = group->nodes[0]->pos_rel.y;
    jnode["pos_abs_x"]  = group->nodes[0]->pos_abs.x;
    jnode["pos_abs_y"]  = group->nodes[0]->pos_abs.y;
    if (group->nodes[0]->anchor) jnode["anchor_tag"] = group->nodes[0]->anchor->group->cell->tag;
  }

  std::ofstream(filename) << root.dump(2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

void Plait::load_json(const char* filename, CellDB& cell_db) {
  printf("Loading plait %s\n", filename);
  using namespace nlohmann;

  CHECK_P(tag_to_group.empty());

  json root;
  std::ifstream(filename) >> root;

  for (auto& it : root.items()) {
    auto& tag = it.key();
    auto& jnode = it.value();

    auto cell = cell_db.tag_to_cell[tag];
    if (cell == nullptr) {
      printf("Did not recognize cell tag %s\n", tag.c_str());
      continue;
    }

    auto group = new PlaitCell(cell);
    tag_to_group[tag] = group;

    auto node = group->add_node();
    node->locked    = jnode.value("locked", false);
    node->pos_rel.x = jnode.value("pos_rel_x", 0.0);
    node->pos_rel.y = jnode.value("pos_rel_y", 0.0);
    node->pos_abs.x = jnode.value("pos_abs_x", 0.0);
    node->pos_abs.y = jnode.value("pos_abs_y", 0.0);
  }

  // Check for missing tags
  for (auto& [tag, cell] : cell_db.tag_to_cell) {
    if (tag_to_group.count(tag) == 0) {
      printf("Did not load node for tag \"%s\", creating placeholder\n", tag.c_str());
      {
        auto group = new PlaitCell(cell);
        tag_to_group[tag] = group;
        group->add_node();
      }
    }
  }

  // Connect anchors
  for (auto& [tag, jnode] : root.items()) {
    if (!jnode["anchor_tag"].is_string()) continue;
    const auto& anchor_tag = jnode["anchor_tag"].get<std::string>();

    if (tag == anchor_tag) {
      printf("node %s anchored to itself?\n", tag.c_str());
    }

    // FIXME nodes[0]
    auto group = tag_to_group[tag];
    auto anchor_group = tag_to_group[anchor_tag];

    if (group && anchor_group) {
      group->nodes[0]->anchor = anchor_group->nodes[0];
    }
    else if (group == nullptr) {
      printf("bad tag %s\n", tag.c_str());
    }
    else if (anchor_group == nullptr) {
      printf("bad anchor tag %s\n", anchor_tag.c_str());
    }
  }

  // Connect ports
  for (auto& [tag, group] : tag_to_group) {
    auto node = group->nodes[0];
    auto arg_count = group->cell->args.size();

    for (auto i = 0; i < arg_count; i++) {
      auto& arg = group->cell->args[i];
      auto prev_group = tag_to_group[arg.tag];
      auto prev_node = prev_group->nodes[0];

      if (prev_group == nullptr) {
        printf("Did not recognize arg tag %s\n", tag.c_str());
      }
      else {
        group->prev_group.push_back(prev_group);
        node->prev_node.push_back(prev_node);
        node->prev_port.push_back(arg.port);
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
