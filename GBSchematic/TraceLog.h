#pragma once
#include "Timer.h"
#include <vector>
#include <functional>

typedef std::function<void(void*)> step_func;

class TraceLog {

  int state_size;

  step_func do_step;
};