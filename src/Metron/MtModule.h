#pragma once

#include <string>
#include <vector>
#include <functional>

#include "tree_sitter/api.h"
#include "MtIterator.h"

typedef std::function<void(MtHandle)> NodeVisitor;
typedef std::function<void(MtHandle parent, MtHandle child)> NodeVisitor2;
typedef std::vector<uint8_t> blob;

//------------------------------------------------------------------------------

struct MtModule {

  std::string input_filename;
  std::string output_filename;

  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;

  blob src_blob;
  TSTree* tree = nullptr;
  const char* source = nullptr;
  const char* source_end = nullptr;
  TSNode root;

  std::vector<TSNode> moduleparams;

  std::vector<TSNode> inputs;
  std::vector<TSNode> outputs;

  std::vector<TSNode> localparams;
  std::vector<TSNode> fields;
  std::vector<TSNode> submodules;

  TSNode node_init;
  TSNode node_tick;
  TSNode node_tock;

  std::vector<TSNode> tasks;
  std::vector<TSNode> functions;

  TSNode module_template = { 0 };
  TSNode module_class = { 0 };
  TSNode module_param_list = { 0 };
  std::string module_name;

  MtModule();
  ~MtModule();
  void load(const std::string& input_filename, const std::string& output_filename);

  void dump_node(TSNode n, int index = 0, int field = -1, int depth = 0);
  void dump_tree(TSNode n, int index, int field, int depth, int maxdepth);
  void dump_tree(TSNode n, int maxdepth) { dump_tree(n, 0, -1, 0, maxdepth); }
  void dump_tree(TSNode n) { dump_tree(n, 0, -1, 0, 255); }

  void visit_tree(MtHandle n, NodeVisitor cv);
  void visit_tree2(MtHandle parent, NodeVisitor2 cv);

  // Text handling
  const char* start(TSNode n);
  const char* end(TSNode n);
  std::string body(TSNode n);
  bool match(TSNode n, const char* s);

  std::string node_to_name(TSNode n);
  std::string node_to_type(TSNode n);

  // Field introspection
  bool field_is_primitive(TSNode n);
  bool field_is_module(TSNode n);
  bool field_is_static(TSNode n);
  bool field_is_const(TSNode n);
  bool field_is_param(TSNode n);
  bool field_is_input(TSNode n);
  bool field_is_output(TSNode n);

  MtHandle get_field_by_id(TSNode id);
  MtHandle get_task_by_id(TSNode id);
  MtHandle get_function_by_id(TSNode id);

  // Scanner
  void find_module();
  void collect_moduleparams();
  void collect_fields();
};

//------------------------------------------------------------------------------
