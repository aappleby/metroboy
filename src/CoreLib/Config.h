#pragma once

// Regression test the LogicBoy implementation against GateBoy
//#define YES_LOGIC_VS_GATES



// Logic-vs-gates mode - turn all the checks off except the hash
#ifdef YES_LOGIC_VS_GATES

#define NO_CHECK
#define YES_HASH
#define NO_OLDNEW_FLAGS
#define NO_DRIVE_FLAGS

#else


#ifdef _DEBUG

// Debug builds - painfully, ungodly slow.

// Evaluate all CHECK() statements (basically assert)
#define YES_CHECK

// Hash the GB state and use it to verify that our gate simulation is idempotent between clock ticks.
#define YES_HASH

// Verify that all registers were flagged as NEW after a logic pass, and set them back to OLD.
#define USE_OLDNEW_FLAGS

// Use the extra gate flag bits to check for floating gates and bus collisions.
#define USE_DRIVE_FLAGS

#elif FAST_MODE

// As-fast-as-possible build - turn all error checking off.
#define NO_CHECK
#define NO_HASH
#define NO_OLDNEW_FLAGS
#define NO_DRIVE_FLAGS

#else

// Running tests in optimized mode build - don't bother with CHECK()s or hash
// tests, but do use all the gate bits as they get folded into the memory dump.

#define NO_CHECK
#define NO_HASH
#define USE_OLDNEW_FLAGS
#define USE_DRIVE_FLAGS

#endif

#endif
