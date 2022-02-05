#pragma once
#include "Platform.h"

#include "MtNode.h"

//------------------------------------------------------------------------------

struct MtModule {

  MtModule();
  ~MtModule();

  void load(const std::string& input_filename, const std::string& output_filename);
  void print_error(MtNode n, const char* fmt, ...);

  // Identifier lookup

  MtNode get_by_id(std::vector<MtNode>& handles, MtNode id);
  MtNode get_field_by_id(MtNode id)    { return get_by_id(fields, id); }
  MtNode get_task_by_id(MtNode id)     { return get_by_id(tasks, id); }
  MtNode get_function_by_id(MtNode id) { return get_by_id(functions, id);}
  MtNode get_input_by_id(MtNode id)    { return get_by_id(inputs, id); }
  MtNode get_output_by_id(MtNode id)   { return get_by_id(outputs, id); }

  // Scanner
  
  void find_module();
  void collect_moduleparams();
  void collect_fields();

  // Rule checker

  void check_dirty_tick(MtNode n);
  void check_dirty_tock(MtNode n);
  void check_dirty_read(MtNode n, bool is_seq, std::set<MtNode>& dirty_fields, int depth);
  void check_dirty_write(MtNode n, bool is_seq, std::set<MtNode>& dirty_fields, int depth);
  void check_dirty_dispatch(MtNode n, bool is_seq, std::set<MtNode>& dirty_fields, int depth);
  void check_dirty_assign(MtNode n, bool is_seq, std::set<MtNode>& dirty_fields, int depth);
  void check_dirty_if(MtNode n, bool is_seq, std::set<MtNode>& dirty_fields, int depth);
  void check_dirty_call(MtNode n, bool is_seq, std::set<MtNode>& dirty_fields, int depth);
  void check_dirty_switch(MtNode n, bool is_seq, std::set<MtNode>& dirty_fields, int depth);

  //----------

  MtModLibrary* lib;
  std::string input_filename;
  std::string output_filename;
  FILE* out_file;

  blob src_blob;
  const char* source = nullptr;
  const char* source_end = nullptr;
  const TSLanguage* lang = nullptr;
  TSParser* parser = nullptr;
  TSTree* tree = nullptr;

  std::string mod_name;

  MtNode mod_root;
  MtNode mod_init;
  MtNode mod_tick;
  MtNode mod_tock;
  MtNode mod_template;
  MtNode mod_class;
  MtNode mod_param_list;

  std::vector<MtNode> modparams;
  std::vector<MtNode> inputs;
  std::vector<MtNode> outputs;
  std::vector<MtNode> localparams;
  std::vector<MtNode> fields;
  std::vector<MtNode> tasks;
  std::vector<MtNode> functions;
  std::vector<MtNode> submodules;
};

//------------------------------------------------------------------------------
