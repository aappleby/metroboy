#pragma once

#include <string>
#include <vector>
#include <functional>
#include <set>

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
  MtHandle root;

  std::vector<MtHandle> moduleparams;

  std::vector<MtHandle> inputs;
  std::vector<MtHandle> outputs;

  std::vector<MtHandle> localparams;
  std::vector<MtHandle> fields;
  std::vector<MtHandle> submodules;

  MtHandle node_init;
  MtHandle node_tick;
  MtHandle node_tock;

  std::vector<MtHandle> tasks;
  std::vector<MtHandle> functions;

  MtHandle module_template;
  MtHandle module_class;
  MtHandle module_param_list;
  std::string module_name;

  MtModule();
  ~MtModule();
  void load(const std::string& input_filename, const std::string& output_filename);

  void print_error(MtHandle n, const char* fmt, ...);

  void dump_node(MtHandle n, int index, int depth);
  void dump_tree(MtHandle n, int index, int depth, int maxdepth);
  void dump_tree(MtHandle n) { dump_tree(n, 0, 0, 255); }

  void visit_tree(MtHandle n, NodeVisitor cv);
  void visit_tree2(MtHandle parent, NodeVisitor2 cv);

  MtHandle get_by_id(std::vector<MtHandle>& handles, MtHandle id);

  MtHandle get_field_by_id(MtHandle id)    { return get_by_id(fields, id); }
  MtHandle get_task_by_id(MtHandle id)     { return get_by_id(tasks, id); }
  MtHandle get_function_by_id(MtHandle id) { return get_by_id(functions, id);}
  MtHandle get_input_by_id(MtHandle id)    { return get_by_id(inputs, id); }
  MtHandle get_output_by_id(MtHandle id)   { return get_by_id(outputs, id); }

  // Scanner
  void find_module();
  void collect_moduleparams();
  void collect_fields();

  void check_dirty_tick(MtHandle n);
  void check_dirty_tock(MtHandle n);
  void check_dirty_read(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth);
  void check_dirty_write(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth);
  void check_dirty_dispatch(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth);
  void check_dirty_assign(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth);
  void check_dirty_if(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth);
  void check_dirty_call(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth);
  void check_dirty_switch(MtHandle n, bool is_seq, std::set<MtHandle>& dirty_fields, int depth);
};

//------------------------------------------------------------------------------
