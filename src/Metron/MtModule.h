#pragma once

#include <string>
#include <vector>
#include <functional>

#include "Utils.h"
#include "tree_sitter/api.h"
#include "../Plait/TreeSymbols.h"
#include "MtIterator.h"

typedef std::function<void(TSNode)> NodeVisitor;
typedef std::function<void(TSNode parent, TSNode child)> NodeVisitor2;

//------------------------------------------------------------------------------

struct Module {

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

  TSNode module_template = { 0 };
  TSNode module_class = { 0 };
  TSNode module_param_list = { 0 };
  std::string module_name;

  Module();
  ~Module();
  void load(const std::string& input_filename, const std::string& output_filename);
  void dump_node(TSNode n, int index = 0, int field = -1, int depth = 0);
  void dump_tree(TSNode n, int index = 0, int field = -1, int depth = 0, int maxdepth = 255);

  void visit_children(TSNode n, NodeVisitor cv);
  void visit_tree(TSNode n, NodeVisitor cv);

  void visit_tree2(TSNode parent, NodeVisitor2 cv);
  const char* start(TSNode n);
  const char* end(TSNode n);
  std::string body(TSNode n);
  bool match(TSNode n, const char* s);
  std::string class_to_name(TSNode n);
  std::string decl_to_name(TSNode decl);
  std::string field_to_name(TSNode n);
  bool field_is_primitive(TSNode n);
  bool field_is_module(TSNode n);
  bool field_is_static(TSNode n);
  bool field_is_const(TSNode n);
  bool field_is_param(TSNode n);
  bool field_is_input(TSNode n);
  bool field_is_output(TSNode n);

  void find_module();
  void collect_moduleparams();
  void collect_fields();
};






