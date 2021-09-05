#pragma once

#pragma warning(disable:4189) // unused local variable

//#pragma warning(disable : 4127)

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
constexpr bool config_check        = config_debug;

// Verify that all registers were flagged as NEW after a logic pass, and set them back to OLD.
constexpr bool config_oldnew_flags = config_debug | config_release;

// Use the extra gate flag bits to check for floating gates and bus collisions.
constexpr bool config_drive_flags  = config_debug | config_release;

// Start the sim in logic mode by default.
constexpr bool config_default_logic_mode = config_fastmode;

// Hash the GB state and use it to verify that our gate simulation is idempotent between clock ticks.
constexpr bool config_hash = config_debug | config_regression;

// If we want to check flags, we have to "commit" the state once we've updated it.
constexpr bool config_commit = config_oldnew_flags | config_drive_flags;

// In debug builds we check that tock_slow() is idempotent.
constexpr bool config_idempotent = config_debug;

// Debug probes disabled in fast mode.
constexpr bool config_probes = !config_fastmode;


/*
#if 0


//#define YES_LOGIC_VS_GATES



// Logic-vs-gates mode - turn all the checks off except the hash
#i f def YES_LOGIC_VS_GATES

#d e fine NO_CHECK
#d e fine YES_HASH
#d e fine NO_OLDNEW_FLAGS
#d e fine NO_DRIVE_FLAGS

#e l se

#i fdef _DEBUG


#d efine YES_CHECK

#d efine YES_HASH

// Verify that all registers were flagged as NEW after a logic pass, and set them back to OLD.
#d efine USE_OLDNEW_FLAGS

// Use the extra gate flag bits to check for floating gates and bus collisions.
#d efine USE_DRIVE_FLAGS

#e lif FAST_MODE

#d efine NO_CHECK
#d efine NO_HASH
#d efine NO_OLDNEW_FLAGS
#d efine NO_DRIVE_FLAGS

#e lse


#d efine NO_CHECK
#d efine NO_HASH
#d efine USE_OLDNEW_FLAGS
#d efine USE_DRIVE_FLAGS

#e ndif

#e ndif

#endif
*/