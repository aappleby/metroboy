#include "GateBoyLib/Probe.h"

#include "GateBoyLib/GateBoy.h"

#include <string>
#include <map>
#include <vector>
#include <deque>



Probes::Probes() {
  pass_cursor = 0;

  for (int i = 0; i < channel_count; i++) {
    memset(names[i], 0, 32);
    sprintf_s(names[i], 32, "<probe %02d>", i);
  }

  memset(phase_samples, '_', channel_count * sample_count);
  memset(pass_samples, '_', channel_count * sample_count);
  memset(stable, 0, sample_count);
}

void Probes::probe(int index, const char* signal_name, char s) {
  strcpy_s(names[index], 31, signal_name);
  if (s <= 1) {
    pass_samples[index][pass_cursor] = s + 30; //? '#' : '_';
    phase_samples[index][phase_cursor] = s + 30; //? '#' : '_';
  }
  else {
    pass_samples[index][pass_cursor] = s;
    phase_samples[index][phase_cursor] = s;
  }
}

void Probes::begin_pass(int phase_total) {
  (void)phase_total;
  pass_cursor = (pass_cursor + 1) % sample_count;
}

void Probes::end_pass(bool _stable) {
  stable[pass_cursor] = _stable;
}

void Probes::begin_phase() {
  phase_cursor = (phase_cursor + 1) % sample_count;
}

void Probes::end_phase() {
}

void Probes::dump(Dumper& d, bool draw_passes) {
  for (int y = 0; y < channel_count; y++) {

    d("\001%-24s : ", names[y]);
    if (draw_passes) {
      auto s = pass_samples[y];
      for (int x = 0; x < sample_count; x++) {
        int idx = (pass_cursor + x + 1) % sample_count;
        d.add_char(stable[idx] ? '\001' : '\007');
        d.add_char(s[idx]);
      }
    }
    else {
      auto s = phase_samples[y];
      for (int x = 0; x < sample_count; x++) {
        int idx = (phase_cursor + x + 1) % sample_count;
        d.add_char(s[idx]);
      }
    }
    d.add_char('\001');
    d.add_char('\n');
  }
}

void probe(int index, const char* signal_name, char s) {
  if (GateBoy::current) {
    GateBoy::current->probes.probe(index, signal_name, s);
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