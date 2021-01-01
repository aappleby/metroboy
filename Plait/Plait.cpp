#include "Plait/Plait.h"

#include <iostream>
#include <fstream>

#if 0
void Node::dump(Dumper& d) {
  d("Node : \"%s\"\n", cell->name.c_str());
  d("Tag  : \"%s\"\n", cell->tag_comment.c_str());
  d("Func : \"%s\"\n", cell->func.c_str());
  d("Tail : \"%s\"\n", cell->tail.c_str());
  for(auto p : prev) {
    d("Arg  : \"%s\"\n", p->cell->name.c_str());
  }
  d("\n");
}
#endif

//-----------------------------------------------------------------------------

#if 0
Node* Plait::get_or_create_node(const std::string& name) {
  Node* new_node = get_node(name);
  if (new_node) return new_node;

  new_node = new Node();
  new_node->name = name;

  nodes.push_back(new_node);
  names.insert({name, new_node});

  return new_node;
}

Node* Plait::get_node(const std::string& name) {
  auto it = names.find(name);
  if (it == names.end()) {
    return nullptr;
  }
  else {
    return (*it).second;
  }
}
#endif

//-----------------------------------------------------------------------------

/*
bool has_tag(const plait::CellDB& cell_db, const std::string& tag) {
  for (auto& cell : cell_db.cells()) {
    if (cell.tag() == tag) return true;
  }
  return false;
}
*/


void Plait::save(const char* filename) {
  printf("Saving plait\n");
  std::ofstream out(filename);

  /*
  for (auto node : nodes) {
    out << "Hello World" << std::endl;
  }
  */

  /*
  int rank = 0;
  int mark = 0;

  bool locked = 0;
  bool selected = 0;

  uint32_t color = 0xFFFF00FF;

  std::vector<Node*> prev;
  std::vector<Node*> next;

  dvec2 spring_force = {0,0};

  const Cell* get_cell()            { return cell; }
  void        set_cell(Cell* _cell) { cell = _cell; }

  Cell* cell = nullptr;
  Node* anchor = nullptr;
  dvec2 offset_old = {0,0};
  dvec2 offset_new = {0,0};
  */

  out << nodes.size() << std::endl;

  for (auto node : nodes) {
    out << node->tag() << std::endl;
    out << node->locked << std::endl;
    out << (node->anchor ? node->anchor->tag() : "<no_anchor>") << std::endl;
    out << node->offset_old.x << std::endl;
    out << node->offset_old.y << std::endl;
  }
}

void Plait::load(const char* filename) {
  printf("Loading plait\n");
  std::ifstream lines(filename);

  int node_count;
  lines >> node_count;
  printf("Node count %d\n", node_count);

  std::map<std::string, Node*> node_map;
  for(auto node : nodes) node_map[node->tag()] = node;

  for (int i = 0; i < node_count; i++) {
    std::string tag;
    bool locked;
    std::string anchor_tag;
    dvec2 offset;

    lines >> tag;
    lines >> locked;
    lines >> anchor_tag;
    lines >> offset.x;
    lines >> offset.y;

    //printf("Tag: %s, offset {%f,%f}\n", tag.c_str(), offset_old.x, offset_old.y);

    auto it = node_map.find(tag);
    if (it != node_map.end()) {
      (*it).second->offset_old = offset;
      (*it).second->offset_new = offset;
      (*it).second->commit_pos();
      (*it).second->locked = locked;
    }
    else {
      printf("Did not recognize tag %s\n", tag.c_str());
    }

    if (anchor_tag != "<no_anchor>") {
      auto it2 = node_map.find(anchor_tag);
      if (it2 != node_map.end()) {
        (*it).second->anchor = (*it2).second;
      }
      else {
        printf("Did not recognize anchor tag %s\n", tag.c_str());
      }
    }
  }
}
