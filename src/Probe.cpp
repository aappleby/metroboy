#include "Probe.h"

#include <string>
#include <set>
#include <map>

std::map<std::string, wire> all_probes;

void probe(const char* signal_name, wire s) {
  all_probes[signal_name] = s;
}

void dump_probes(Dumper& d) {
  d("---------- PROBES ----------\n");
  for (const auto& v : all_probes) {
    d("%-24s : %d\n", v.first.c_str(), v.second);
  }
  d("\n");
}