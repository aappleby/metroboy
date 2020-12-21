#include "GateBoyLib/Probe.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

thread_local Probes* thread_probes = nullptr;

Probes::Probes() {
  reset_to_cart();
}

void Probes::reset_to_cart() {
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

void Probes::begin_pass(int pass_count) {
  CHECK_P(thread_probes == nullptr);
  thread_probes = this;
  pass_cursor = (pass_cursor + 1) % sample_count;
  if (pass_count == 0) {
    phase_cursor = (phase_cursor + 1) % sample_count;
  }
}

void Probes::end_pass(bool _stable) {
  CHECK_P(thread_probes == this);
  stable[pass_cursor] = _stable;
  thread_probes = nullptr;
}

void Probes::dump(Dumper& d, bool draw_passes) {
  for (int y = 0; y < channel_count; y++) {

    d("\001%-16s : ", names[y]);
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
  if (thread_probes) {
    thread_probes->probe(index, signal_name, s);
  }
  else {
    //printf("<no probes for current thread>\n");
  }
}
