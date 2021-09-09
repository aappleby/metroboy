#pragma once

#pragma warning(disable:4189) // unused local variable
#pragma warning(disable:4100) // unused param
#pragma warning(disable:4127) // conditional expression is constant

#pragma optimize("", off)

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




// Evaluate all CHECK() statements (basically assert)
constexpr bool config_check        = config_debug | config_release;

// Verify that all registers were flagged as NEW after a logic pass, and set them back to OLD.
constexpr bool config_oldnew_flags = config_debug | config_release;

// Use the extra gate flag bits to check for floating gates and bus collisions.
constexpr bool config_drive_flags  = config_debug | config_release;

// In debug builds we check that tock_slow() is idempotent.
constexpr bool config_idempotence = config_debug;