This repo is the home of MetroBoy, GateBoy, and Plait. Metroboy is currently broken, don't use it.

GateBoy

GateBoy is a gate-level simulation of the original Game Boy hardware that was reverse-engineered from die shots of the original DMG-0? chip.

I owe a _huge_ amount of thanks to [Furrtek](https://github.com/furrtek) for his original [schematics](https://github.com/furrtek/DMG-CPU-Inside) that served as a [Rosetta Stone](https://en.wikipedia.org/wiki/Rosetta_Stone) for getting the whole translation started. I've noted in the codebase where I found errors in the schematics - some have been reported back to Furrtek but there are still a lot of discrepancies.


How is this simulation connected to the Furrtek schematics?

 - Every gate in the Furrtek schematics has a corresponding line in the GateBoy source code. Lines are tagged like this - `/*#p08.ASUR*/` - this means that gate ASUR is on page 8 of the schematics, and the '#' indicates that I've manually traced the gate to verify that the schematic is correct.


Is GateBoy a perfect simulation of a Game Boy?

 - Actually no, for complicated reasons. The Game Boy chip has a handful of logic gates that operate [independently of the master clock](https://en.wikipedia.org/wiki/Asynchronous_circuit) and whose exact behavior depends on things like [gate delay](https://en.wikipedia.org/wiki/Propagation_delay). These gates create [glitches](https://en.wikipedia.org/wiki/Glitch) that depend heavily on the physical placement of the gates, the silicon process used to make them, and other weird things like temperature and voltage.

 For example, there's a glitch in the external address bus logic that causes internal bus addresses like 0xFF20 to appear on the external bus even though the logic should prevent that. Due to input delays, gate LOXO (page 8 in the schematic) can produce a glitch pulse that causes latch ALOR


Why is GateBoy so slow?
GateBoy simulates every logic gate on the DMG chip.