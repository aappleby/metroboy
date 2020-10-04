#include "GateBoyLib/Probe.h"

#include <string>
#include <map>

std::map<std::string, wire> all_probes;

void probe(const char* signal_name, wire s) {
  all_probes[signal_name] = s;
}

void dump_probes(Dumper& d) {
  if (all_probes.size()) {
    d("\002===== Probes =====\001\n");
    for (const auto& v : all_probes) {
      d("%-24s : %d\n", v.first.c_str(), v.second);
    }
    d("\n");
  }
}

void clear_probes() {
  all_probes.clear();
}