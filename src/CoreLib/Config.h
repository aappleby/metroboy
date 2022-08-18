#pragma once

#pragma warning(disable:4189) // unused local variable
#pragma warning(disable:4100) // unused param
#pragma warning(disable:4127) // conditional expression is constant
#pragma warning(disable:4201) // anonymous struct/union

#define SIM_AUDIO

#if 1
// Debug builds - painfully, ungodly slow.
#ifdef CONFIG_DEBUG
constexpr bool config_debug = true;
#else
constexpr bool config_debug = false;
#endif

// Regression test the LogicBoy implementation against GateBoy
#ifdef CONFIG_REGRESSION
constexpr bool config_regression = true;
#else
constexpr bool config_regression = false;
#endif

// Running tests in optimized mode build - don't bother with CHECK()s or hash
// tests, but do use all the gate bits as they get folded into the memory dump.
#ifdef CONFIG_RELEASE
constexpr bool config_release = true;
#else
constexpr bool config_release = false;
#endif

// As-fast-as-possible build - turn all error checking off.
#ifdef CONFIG_FASTMODE
constexpr bool config_fastmode = true;
#else
constexpr bool config_fastmode = false;
#endif

static_assert(((int)config_debug + (int)config_regression + (int)config_release + (int)config_fastmode) == 1, "Bad build config");
#endif

#if 0
constexpr bool config_debug = true;
constexpr bool config_regression = false;
constexpr bool config_release = false;
constexpr bool config_fastmode = false;
#endif

// debug      - use flags, dchecks, idempotence checks
// release    - use flags, no dchecks
// regression - no flags, no dchecks, regression checks against release
// fast       - no flags, no dchecks

// Evaluate all DCHECK() statements (basically assert)
constexpr bool config_dcheck      = config_debug;
constexpr bool config_use_flags   = config_debug || config_release;
constexpr bool config_check_flags = config_debug;

// In debug builds we check that tock_slow() is idempotent.
constexpr bool config_idempotence  = config_debug;
//constexpr bool config_idempotence  = false;

static_assert((config_check_flags && config_use_flags) || !config_check_flags, "Bad config_use/check_flags");