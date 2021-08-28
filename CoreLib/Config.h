#pragma once

#ifdef _DEBUG

// Debug builds -

// Evaluate all CHECK() statements (basically assert)
#define YES_CHECK

// Hash the GB state and use it to verify that our gate simulation is idempotent between clock ticks.
#define YES_HASH

// Verify that all registers were flagged as NEW after a logic pass, and set them back to OLD.
#define YES_COMMIT

// Use the extra gate flag bits to check for old/new violations, floating gates, and bus collisions.
#define YES_GATE_BITS

#elif FAST_MODE

// As-fast-as-possible build - turn all error checking off.

#define NO_CHECK
#define NO_HASH
#define NO_COMMIT
#define NO_GATE_BITS

#else

// Running tests build - don't bother with CHECK()s or hash tests, but do use all
// the gate bits as they get folded into the memory dump.

#define NO_CHECK
#define NO_HASH
#define YES_COMMIT
#define YES_GATE_BITS

#endif
