Game Boy CPU Instruction Behavior Test
--------------------------------------
This ROM tests the behavior of all CPU instructions except STOP and the
11 illegal opcodes. The tests are fairly thorough, running instructions
with boundary data and verifying both the result and that other
registers are not modified. Instructions which perform the same
operation on different registers are each tested just as thoroughly, in
case an emulator implements each independently. Some sub-tests take half
minute to complete.

Failed instructions are listed as

	[CB] opcode

Some errors cannot of course be diagnosed properly, since the test
framework itself relies on basic instruction behavior being correct.


Internal operation
------------------
The main tests use a framework that runs each instruction in a loop,
varying the register values on input and examining them on output.
Rather than keep a table of correct values, it simply calculates a
CRC-32 checksum of all the output, then compares this with the correct
value. Instructions are divided into several groups, each with a
different set of input values suited for their behavior; for example,
the bit test instructions are fed $01, $02, $04 ... $40, $80, to ensure
each bit is handled properly, while the arithmetic instructions are fed
$01, $0F, $10, $7F, $FF, to exercise carry and half-carry. A few
instructions require a custom test due to their uniqueness.


Multi-ROM
---------
In the main directory is a single ROM which runs all the tests. It
prints a test's number, runs the test, then "ok" if it passes, otherwise
a failure code. Once all tests have completed it either reports that all
tests passed, or prints the number of failed tests. Finally, it makes
several beeps. If a test fails, it can be run on its own by finding the
corresponding ROM in individual/.

Ths compact format on screen is to avoid having the results scroll off
the top, so the test can be started and allowed to run without having to
constantly monitor the display. 

Currently there is no well-defined way for an emulator test rig to
programatically find the result of the test; contact me if you're trying
to do completely automated testing of your emulator. One simple approach
is to take a screenshot after all tests have run, or even just a
checksum of one, and compare this with a previous run.


Failure codes
-------------
Failed tests may print a failure code, and also short description of the
problem. For more information about a failure code, look in the
corresponding source file in source/; the point in the code where
"set_test n" occurs is where that failure code will be generated.
Failure code 1 is a general failure of the test; any further information
will be printed.

Note that once a sub-test fails, no further tests for that file are run.


Console output
--------------
Information is printed on screen in a way that needs only minimum LCD
support, and won't hang if LCD output isn't supported at all.
Specifically, while polling LY to wait for vblank, it will time out if
it takes too long, so LY always reading back as the same value won't
hang the test. It's also OK if scrolling isn't supported; in this case,
text will appear starting at the top of the screen.

Everything printed on screen is also sent to the game link port by
writing the character to SB, then writing $81 to SC. This is useful for
tests which print lots of information that scrolls off screen.


Source code
-----------
Source code is included for all tests, in source/. It can be used to
build the individual test ROMs. Code for the multi test isn't included
due to the complexity of putting everything together.

Code is written for the wla-dx assembler. To assemble a particular test,
execute

	wla -o "source_filename.s" test.o
	wlalink linkfile test.gb

Test code uses a common shell framework contained in common/.


Internal framework operation
----------------------------
Tests use a common framework for setting things up, reporting results,
and ending. All files first include "shell.inc", which sets up the ROM
header and shell code, and includes other commonly-used modules.

One oddity is that test code is first copied to internal RAM at $D000,
then executed there. This allows self-modification, and ensures the code
is executed the same way it is on my devcart, which doesn't have a
rewritable ROM as most do.

Some macros are used to simplify common tasks:

	Macro               Behavior
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	wreg addr,data      Writes data to addr using LDH
	lda  addr           Loads byte from addr into A using LDH
	sta  addr           Stores A at addr using LDH
	delay n             Delays n cycles, where NOP = 1 cycle
	delay_msec n        Delays n milliseconds
	set_test n,"Cause"  Sets failure code and optional string

Routines and macros are documented where they are defined.

-- 
Shay Green <gblargg@gmail.com>
