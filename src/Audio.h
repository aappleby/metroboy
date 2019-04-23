#pragma once
#include "Types.h"

struct AudioQueue {
  AudioQueue() : queue(), mux(), cv(), closed(false) {
  }

  std::list<sample_t*> queue;
  std::mutex mux;
  std::condition_variable cv;
  bool closed;

  sample_t* get() {
    std::unique_lock<std::mutex> lock(mux);
    cv.wait(lock, [&] { return closed || !queue.empty(); });
    if (queue.empty()) {
      return nullptr;
    }
    else {
      sample_t* buf = queue.front();
      queue.pop_front();
      return buf;
    }
  }

  void put(sample_t* buf) {
    std::unique_lock<std::mutex> lock(mux);
    queue.push_back(buf);
    cv.notify_one();
  }

  void close() {
    closed = true;
    cv.notify_all();
  }
};

void audio_init();
void audio_stop();
void audio_begin();
void audio_post(sample_t in_l_i, sample_t in_r_i);
void audio_end();
