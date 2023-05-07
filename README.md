# This repo is the home of MetroBoy, GateBoy, and LogicBoy (yes, rather too many projects for one repo but they're very tightly interconnected)

Update Dec 2021 - I've merged the "LogicBoy" branch into master. LogicBoy is a translation of GateBoy into more standard C++ idioms, while maintaining bit-identical behavior with the reference GateBoy model. It currently runs at 60+ fps instead of 6 fps in "fast" mode, with a few more optimization passes still left to go.

## GateBoy

![GateBoy screenshot](images/gateboy.png "GateBoy screenshot")

GateBoy is a **gate-level simulation** of the original Game Boy hardware that was [**reverse-engineered from die shots of the original DMG-01 chip**](https://siliconpr0n.org/map/nintendo/dmg-cpu-b/mz_mit20x/#x=9744&y=8000&z=3). It includes all the standard cells on the chip but not the CPU - it's made of custom logic and is a bit too blurry for me to decipher. GateBoy's CPU is instead my current best guess at how it might be implemented given the constraints implied by the rest of the chip.

Precompiled builds with test ROMS are here - https://github.com/aappleby/MetroBoy/releases/tag/GateBoy_v0.1.1

GateBoy runs at around 6 to 8 frames per second in "fast mode" on a modern 4-ish ghz processor. That's quite horrible compared to an emulator, but pretty impressive for something that's simulating a few billion gates per second on a single core.

I owe a **huge** amount of thanks to [Furrtek](https://github.com/furrtek) for his original [die traces](https://github.com/furrtek/DMG-CPU-Inside/blob/master/preview.png) and [schematics](https://github.com/furrtek/DMG-CPU-Inside) that served as a [Rosetta Stone](https://en.wikipedia.org/wiki/Rosetta_Stone) for getting the whole translation started. I've noted in the codebase where I found errors in the schematics - some have been reported back to Furrtek but there are still a lot of discrepancies.

Big thanks are also owed to [Gekkio](https://github.com/gekkio) for his [Mooneye emulator](https://github.com/Gekkio/mooneye-gb) and tests that helped bootstrap Gateboy, and for the flash cart he designed that I used to build many many additional tests.

## GateBoy FAQ

- How is this simulation connected to the Furrtek schematics?
  - Furrtek assigned every gate on the die and in the schematic a 4-character code like "ASUR" or "BALY". Each of those gates has a corresponding line in the GateBoy source code. Lines in the source are tagged like this - `/*#p08.ASUR*/` - this means that gate ASUR (which happens to be a 2-mux) is on page 8 of the schematics, and the '#' indicates that I've manually traced the gate to verify that the schematic is correct.
  - Here's a chunk of the unmodified die shot with ASUR in the middle - <img src="images/ASUR_context1.png" alt="drawing" width="100%"/>
  - Here's the same chunk with Furrtek's annotations - <img src="images/ASUR_context2.png" alt="drawing" width="100%"/>
  - And here's a closeup showing the three inputs coming into the top "rungs" of the cell, and the output at the bottom - <img src="images/ASUR_traced.png" alt="drawing" width="100%"/>
  - which corresponds to this ASUR in Furrtek's schematic - <img src="images/ASUR_schematic.png" alt="drawing" width="100%"/>
  - which in turn gets translated to this ASUR in GateBoy's code - <img src="images/ASUR_code.png" alt="drawing" width="100%"/>
  - Repeat that a few thousand times, spend a year-ish debugging, and you get GateBoy. To give you a sense of scale, here's the whole die with a red dot covering ASUR - there are currently 2674 active cells in GateBoy, and another thousand-ish in the audio hardware that aren't being simulated - <img src="images/ASUR_die.png" alt="drawing" width="100%"/>

- How is this simulation tested?
  - GateBoy has a fairly comprehensive test suite that runs all of [the Mooneye tests](https://github.com/Gekkio/mooneye-gb/tree/master/tests), as well as a large suite of "micro-tests" that execute in a small number of cycles.
  - GateBoy can also do automated render tests (used for [Mealybug's test suite](https://github.com/mattcurrie/mealybug-tearoom-tests)), but those are currently disabled.
  - There are probably a few plain old code bugs remaining as well. Right now one of the early screens in Zelda is doing something funny with the grass tiles...

- Is GateBoy a perfect simulation of a Game Boy?
  - Actually no, for complicated reasons. The Game Boy chip has a handful of logic gates that operate [independently of the master clock](https://en.wikipedia.org/wiki/Asynchronous_circuit) and whose exact behavior depends on things like [gate delay](https://en.wikipedia.org/wiki/Propagation_delay), the physical distance from the gate to its inputs/outputs, the silicon process used to make them, and (in very strange circumstances) the temperature of the silicon and the voltage applied to the chip. These asynchronous gates can create [glitches](https://en.wikipedia.org/wiki/Glitch) that do strange things which can't be predicted just by looking at a schematic of the chip.
  - For example, there's a glitch in the external address bus logic that causes internal bus addresses like `0xFF20` to appear on the external bus even though the logic should prevent that. Due to gate delays, not all of the inputs to gate `LOXO` (a combined AND-OR gate on page 8 in Furrtek's schematics) arrive at the same time. The differences in arrival time cause `LOXO` to produce a glitch pulse that in turn causes latch `ALOR` to make a copy of one bit of the internal bus address. `ALOR` then drives that bit onto the external bus (through a few more gates) where it can be seen with an oscilloscope or logic analyzer.

- Wait, if glitches don't show up in the schematics then how did you figure that one out?
  - In this case we can deduce what's going on because we can see the side-effect of the glitch on the external bus and there's not that many possible ways that address signal could've gotten there.
  - Other internal glitches are harder to figure out because they don't affect external circuits - they just show up as "something does not match the simulation". There are probably 4-5 glitches that need to be tracked down somehow before the simulation is "perfect", but I'm not going to block the release of GateBoy until I find them.

- Why is GateBoy so slow?
  - GateBoy simulates every logic gate on the DMG chip, one gate at a time. Adding two 8-bit values isn't simulated as "a = b + c;", it's simulated as eight 1-bit adders and eight 1-bit registers and all the control logic that goes along with it.
  - In debug builds, all gates also includes a bunch of error checking to verify that gates aren't read before they're updated, that buses aren't floating, that the simulation always stabilizes, and other things like that.
  - GateBoy also simulates every clock _phase_, not just individual clock cycles. While you may have read that the Game Boy runs at 1 megahertz, this is not quite correct. The 4.19 megahertz clock crystal feeds a set of gates `AFUR+ALEF+APUK+ADYK` that produce four 1 mhz clocks that are out of phase with each other. Those clocks are then combined by additional logic to create sub-clocks of various patterns and frequencies whose edges can lie on either the positive or negative edges of the 4.19 mhz master clock. So, it's more accurate to say that the Game Boy has a 1-megahertz, 8-phase clock. In GateBoy we give each phase a letter (A through H) and all sub-clocks have a suffix like this - `BALY_xBCDEFGH` - which indicates that the clock generated by gate BALY is high on phases B through H.
  - Even with heroic optimization and all the error checking turned off in "fast mode", we still only hit 6-8 fps on a modern CPU.

- Why is GateBoy so fast?
  - Aha, now we're asking interesting questions. Simulating hardware in software is usually 1000's of times slower than realtime - GateBoy is "only" 8x slower than realtime. How does it do that?
  - GateBoy is designed so that most of the simulated gates optimize down to a single instruction or less after the compiler's optimization pass - one "and" gate turns into one "and" instruction, chains of "not" gates get optimized out, etcetera.
  - Normally this would require a huge amount of simulation infrastructure to ensure that the simulation doesn't diverge from the "real world" circuit. GateBoy doesn't do this. Instead, GateBoy does all its error checking by adding additional flags to each wire and register and verifying (in debug builds) that every gate is evaluated in the correct order so that the result is the same as if every gate was evaluated in parallel. There are a few workarounds to deal with asynchronous logic, but they are minor.
  - The flags are positioned so that they don't interfere with the usual one-instruction-per-gate operations, and in "fast mode" builds the flags are disabled and everything optimizes down quite tightly.

- Wouldn't it be even faster to write this in Verilog and then simulate it in Verilator or something?
  - You would think so, and I have translated small portions of GateBoy into Verilog and simulated them in Verilator just to prove that GateBoy's simulation strategy does produce correct results. However, the Verilated code is still around 5-10x slower than GateBoy compiled in "fast mode".

# How do I build and run GateBoy?

All the code is cross-platform and has been tested under Windows 10, Windows 11, Debian, Ubuntu, and WSL-G. Clone the repo and don't forget to do "git submodule init" and "git submodule update" to pull down the support libraries (SDL2, glm, imgui, and json).

- On Windows, open MetroBoy.sln in Visual Studio Community 2019 and build and run as ususal. If you get a "SDL2.dll not found" error, you can either install SDL2 globally, manually copy SDL2.dll into the same folder as the executables, or change "Working Directory" in Project Properties -> Configuration Properties -> Debugging to "$(SolutionDir)".

- On Linux, make sure you have libsdl2-dev installed and run "ninja" from the repo root. You'll get a set of "fast mode" binaries in bin/, which should be launched from the repo root as they make assumptions about directories and such.

- Test roms are symlinked to [GBMicrotests](https://github.com/aappleby/GBMicrotests).
- To run all tests roms, run bin/GateBoyTests from root. The tests should take a second or two, and there should be 20 (known) failures.
- Running other tests (mooneye, mealybug, etc) requires additional setup that I haven't automated yet. :/

# What happened to MetroBoy?

MetroBoy is/was a higher level Game Boy emulator that I wrote from scratch before Furrtek released his schematics. I got it to the point where it passed virtually all the available tests, and then hacked it up in various ways to help get GateBoy working. MetroBoy is currently broken, don't use it. It will be coming back eventually.

# So what comes after GateBoy?

The next step after this is LogicBoy, which will be a simulation that's equivalent at the register level to GateBoy but expresses the logic in more conventional C instead of and/or/not/etc. gates.

## And after that?

MetroBoy will be rewritten so that its externally visible behavior exactly matches LogicBoy.

## That's a lot of stuff. What's the overarching point of all this, anyhow?

GateBoy, LogicBoy, and MetroBoy exist to give me a starting point for working on Metron, which is my long-term project to build a set of programming tools that can bridge between the C/C++ universe used by software and the Verilog/VHDL universe used by hardware. Eventually there will be a single codebase that, using some custom tools, can be translated directly to C++ and run on a PC or that can be translated to SystemVerilog and run on a FPGA.
