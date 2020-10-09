#include "GateBoyLib/Probe.h"

#include "GateBoyLib/GateBoy.h"

#include <string>
#include <map>
#include <vector>
#include <deque>



Probes::Probes() {
  cursor = 0;

  for (int i = 0; i < channel_count; i++) {
    memset(names[i], 0, 32);
    sprintf_s(names[i], sample_count, "<probe %02d>", i);
  }

  memset(samples, '_', channel_count * sample_count);
}

void Probes::probe(int index, const char* signal_name, char s) {
  strcpy_s(names[index], 31, signal_name);
  if (s <= 1) {
    samples[index][cursor] = s + 30; //? '#' : '_';
  }
  else {
    samples[index][cursor] = s;
  }
}

void Probes::begin_pass() {
  cursor = (cursor + 1) % sample_count;
}
void Probes::end_pass() {
}

void Probes::dump(Dumper& d) {
  for (int y = 0; y < channel_count; y++) {
    d("%-24s : ", names[y]);
    auto s = samples[y];
    for (int x = 0; x < sample_count; x++) {
      d.add_char(s[(cursor + x + 1) % sample_count]);
    }
    d.add_char('\n');
  }
}

void probe(int index, const char* signal_name, char s) {
  if (GateBoy::current) {
    GateBoy::current->probes.probe(index, signal_name, s);
  }
}

void dump_probes(Dumper& d) {
  if (GateBoy::current) {
    GateBoy::current->probes.dump(d);
  }
}

#if 0
std::map<std::string, std::deque<wire>> all_probes;

void probe(int index, const char* signal_name, wire s) {
  (void)index;
  auto& samples = all_probes[signal_name];
  while(samples.size() >= 30) samples.pop_front();
  samples.push_back(s);
}

void dump_probes(Dumper& d) {
  if (all_probes.size()) {
    d("\002===== Probes =====\001\n");
    for (const auto& v : all_probes) {
      d("%-24s : ", v.first.c_str());
      for (const auto& s : v.second) {
        d("%c", s ? '#' : '_');
      }
      d("\n");
    }
    d("\n");
  }
}

void clear_probes() {
  all_probes.clear();
}
#endif