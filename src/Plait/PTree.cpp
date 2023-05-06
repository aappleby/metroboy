#include "Plait/PTree.h"
#include "metrolib/core/Types.h"
#include "metrolib/core/File.h"
#include <regex>

using namespace std;

extern "C" {
  extern const TSLanguage *tree_sitter_cpp();
}

PTree::PTree(const char* filename) {
  load_blob(filename, src_blob);

  auto parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_cpp());
  tree = ts_parser_parse_string(parser, NULL, (const char*)src_blob.data(), (uint32_t)src_blob.size());
  ts_parser_delete(parser);
}

PTree::~PTree() {
  ts_tree_delete(tree);
}

int count_descendants(PNode node) {
  int count = node.child_count();
  int total = count;
  for (int i = 0; i < count; i++) {
    total += count_descendants(node.child(i));
  }
  return total;
}

int PTree::count_nodes() const {
  return count_descendants(root());
}

int PTree::count_comments() const {
  std::deque<PNode> queue;
  queue.push_back(root());

  int total = 0;

  while(!queue.empty()) {
    PNode node = queue.back();
    queue.pop_back();

    if (node.is_comment()) {
      std::string s = node.body(source());

      smatch match;
      static regex rx_extract_tag(R"(^\/\*(.*?)\*\/)");
      if (regex_search(s, match, rx_extract_tag)) {
        //printf("%s\n", s.c_str());
        total++;
      }
      else {
        printf("%s\n", s.c_str());
      }

    }

    node.enqueue_children(queue);
  }

  return total;
}
