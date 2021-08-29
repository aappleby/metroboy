#include "GateBoyLib/Probe.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

#include "GateBoyLib/Gates.h"

#pragma warning(disable: 4996)

thread_local Probes* thread_probes = nullptr;

Probes::Probes() {
  reset_to_cart();
}

void Probes::reset_to_cart() {
  phase_cursor = 0;
  for (int i = 0; i < channel_count; i++) {
    memset(names[i], 0, 32);
    sprintf(names[i], "<probe %02d>", i);
  }

  memset(phase_samples, '_', channel_count * sample_count);
  memset(phase_tags, '_', sample_count);
}

void Probes::probe_wire(int index, const char* signal_name, char s) {
  strcpy(names[index], signal_name);
  if (s <= 1) {
    phase_samples[index][phase_cursor] = s + 30; //? '#' : '_';
  }
  else {
    phase_samples[index][phase_cursor] = s;
  }
}

void Probes::begin_pass(int phase_tag) {
  CHECK_P(thread_probes == nullptr);
  thread_probes = this;
  phase_cursor = (phase_cursor + 1) % sample_count;
  phase_tags[phase_cursor] = phase_tag;
}

void Probes::end_pass() {
  CHECK_P(thread_probes == this);
  thread_probes = nullptr;
}

void Probes::dump(Dumper& d) {
  for (int y = 0; y < channel_count; y++) {

    d("\001%02d %-16s : ", y, names[y]);
    auto s = phase_samples[y];
    for (int x = 0; x < sample_count; x++) {
      int idx = (phase_cursor + x + 1) % sample_count;

      d.add_char(phase_tags[idx] == 0 ? '\004' : '\001');
      d.add_char(s[idx]);
    }
    d.add_char('\001');
    d.add_char('\n');

    //if ((y & 7) == 7) d.add_char('\n');
  }
}

void probe_wire(int index, const char* signal_name, wire s) {
  if (thread_probes) {
    thread_probes->probe_wire(index, signal_name, s & 1);
  }
  else {
    //printf("<no probes for current thread>\n");
  }
}

void probe_char(int index, const char* signal_name, char s) {
  if (thread_probes) {
    thread_probes->probe_wire(index, signal_name, s);
  }
  else {
    //printf("<no probes for current thread>\n");
  }
}
