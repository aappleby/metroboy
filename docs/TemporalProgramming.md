# Time is a programming paradigm


## TL;DR

A recurrence is a mathematical function that defines a sequence of values {x0, x1, x2...} given a starting value x0 and a recurrence relation "x' = f(x)" that defines how to compute each element in the sequence from the previous values.

Temporal programming is a paradigm in which programs are expressed solely as recurrences. In the above definition, "x" represents the entire state of the program and "f(x)" computes the next state of the program as a function of the previous state.
 
 The ideas behind temporal programming have been around for decades, but "pure" temporal programming languages still don't exist yet. Once they do, they should allow us to do some very interesting things.

## Some Background, And A Caveat

####  So what is "Temporal Programming"?
Temporal programming is a generalization of the exceedingly common "new_state = f(old_state)" pattern that appears in...

We call a program "temporal" if it relies on the ability to atomically change "old" state into "new" state.

A temporal program advances from state to state like a ticking clock, with each new state being a pure function of the previous one.

## Aren't you just talking about...

OK, so it's not exactly any of those things. Still, your definition of "temporal programming" sounds rather simplistic - why call it a programming paradigm?
    Think about the lack of loops in some functional languages - at first that limitation seems like a annoyance, but once you've gained familiarity with recursion and map/reduce it starts to feel less annoying and more elegant. Going back to a procedural langauge, you might then find yourself annoyed at having to write loops again for something that would be a one-liner in your preferred functional language.

A software developer who tries to read a large chunk of Verilog for the first time is in for a very, very steep learning curve - and it's not a syntax issue. Verilog _looks_ superficially like C but it doesn't _run_ like C, and the explanation that's often given to new devs is something along the lines of "Hardware just doesn't work like that". Which is true, but it's missing the point - The learning curve isn't caused by the language, it's caused by thinking about the problem using the wrong programming paradigm.

OK, this is starting to sound somewhat interesting. Surely all of this has been researched thoroughly by now?
    Absolutely, but not to the extent you might think.
    The ideas have been floating around for years in various obscure programming languages, research papers, and talks
    <reference list>

So what can you do with temporal programming?


Open Questions

References

This list is not at all comprehensive, and I keep finding new variations on these ideas as I stumble across new keywords to plug into Google Scholar. If you know of a project or language that seems to be related to temporal programming, email me and I'll add them to this list.

    Esterel
    Signal
    Plaid
    FRP
    Verilog/VHDL
    TLA+
    Guarded Atomic Actions
    Functional Reactive Programming
    Synchronous Programming Languages
    Communicating Sequential Processes
    
































# Time is a programming paradigm.

Changing “old” state into “new” state atomically is a programming paradigm.

## What defines a programming paradigm?

### It should provide a concise but abstract conceptual model

A good programming paradigm should be explainable in a few sentences and should provide an answer to the general question of “What is programming?” without referring to concrete details like language, syntax, or implementation.

For imperative programs, that would be Turing machines. For functional programming, lambda calculus. For temporal programming, recurrence relations

### It should be practical and useful

A good programming paradigm should be demonstrable using the tools we already have, even if those tools aren’t yet

### It should extend our ability to think about programming

If it’s worth calling a paradigm, it should be profound enough that it gives us new ways to think about and solve programming problems. It should add “new tools to the toolbox” that don’t exist in other paradigms.

    x' = f(x)

“Structural” and “Procedural” programming extended imperative programming by providing ways to simplify programs by clarifying control flow and allowing programs to be split into collections of sub-procedures. “Object-Oriented” programming extended that further by describing programs as collections of “things” that can perform “actions” at a higher level of abstraction.

Functional programming gave us an alternate definition of programming, one based on applying and composing functions (link to wikipedia) 

Temporal programming can be seen as a sort of “imperative programming rotated 90 degrees through time” - instead of modeling a program as a blob of data we call “state” that is incrementally modified by a sequence of actions over time, temporal programs consist of sequences of “states” placed at discrete points in time, each computed from the previous state in a single atomic action.

If it sounds like temporal programming should by definition be “easy”, I can assure you that it is not. A seasoned C programmer might be able to comprehend isolated snippets of Verilog, but going from there to understanding something like a DDR memory controller is a _huge_ mental leap.

This is part of why you hear FPGA & ASIC developers say “Writing code in Verilog is _not_ like writing Quicksort - everything happens at once!” - true, but missing the point slightly. Quicksort makes intuitive sense in the imperative and functional paradigms, but attempting to write Quicksort in Verilog will quickly prove futile - while you may be able to produce something Quicksort-esque the result will almost certainly not be usable in actual hardware, as actual hardware has no notion of “recursion”.

Similarly, while you could write a memory controller in an imperative language that “bit-bangs” the control signals to a DDR chip, your implementation will be fundamentally and radically limited by the host processor - generating and responding to dozens of control signals and data wires within a handful of nanoseconds just isn’t something modern CPUs are designed for.

This isn’t to say that temporal ideas aren’t useful in imperative or functional languages, or vice versa - for example, having a mechanism to coordinate atomic state changes across objects can be hugely useful in implementing simulations. Similarly, being able to model a Verilog module’s sub-circuits as collections of pure functions can greatly improve the maintainability of the codebase.

## The "new_state = f(old_state)" pattern

The pattern appears all over the place.

## Translating C to Verilog?

If you’re a veteran C programmer, I can virtually guarantee that you will not be able to write functional Verilog systems of any significant size until you sit down and internalize some temporal programming concepts.

Translating a C program into Verilog will almost never work, for the same reason that you can’t translate a chocolate chip cookie recipe into a chocolate chip cookie factory - the former is a set of instructions that a complex actor can perform to produce cookies in series, the latter is a collection of machines and conveyor belts that produce cookies in parallel.

It’s not impossible, however - it just requires that the C program be written in a more temporal style, one that will probably feel cumbersome and alien to procedural programmers at first.

In order to “write” a cookie factory in C, we need to describe our cookie recipe differently. In our factory, all the machines are running all the time.

We can imagine an initial version of our cookie factory that just replicates the steps in the recipe using a sequence of robots, each performing one step of the recipe. Robot A puts butter and sugar in a bowl and hands it off to robot B. Robot B creams the butter and sugar together and passes the bowl to robot C, etcetera etcetera.

This will work to a degree, but it doesn’t scale up well. 

 Instead of “cream together the butter and sugar”, we need to define a butter pipeline and a sugar pipeline 

We need to define a “cookie pipeline”.

For our butter pipeline, we want to turn on the pipe when we have bowls to fill, fill all the bowls with the right amount of butter as fast as possible, and then turn off the pipe. Same for sugar.




Right now our cookie factory is set up specifically for chocolate chip cookies, but as responsible factory owners we probably want to be able to produce other types of cookies as well. It would be nice if we could generalize our cookie factory to be able to handle oatmeal raisin cookies, snickerdoodles, and white chocolate macadamia nut cookies (one of my favorites).



Our fully-automated cookie factory consists of:

Various tubes or conveyors containing raw ingredients
Dispensing mechanisms that can place measured amounts of those ingredients into mixing containers



Some signals in our factory are “fast” - our dough dispenser might need a “dispense” signal that lasts exactly 0.3 seconds in order for our cookies to all be the same size.

Some signals are “slow” - cookies need to be removed from the oven when they’re done, but a few seconds delay isn’t disastrous.

Some signals only change infrequently, if ever - we might make chocolate chip cookies for a week, then oatmeal cookies for a week, then back to chocolate chip.


As you might have realized by now, while the “pipeline” is relatively simple, all the “control” logic is a pain to wire up and has to change every time we want to use a new recipe. It would make more sense if we could wire up all the “slow” control signals to a central computer and leave the machines on the factory floor solely responsible for the heavy lifting (and mixing, and baking) that needs to be parallelized.

And so in practice that’s what most factories/circuits/etcetera do - they define a “factory” full of machines and the connections to those machines, provide a way for an external “controller” to change the settings on the machines and start/stop them remotely, and provide feedback to the controller on the current state of each machine.




This analogy may sound ridiculous, but it’s actually not that far off conceptually from how real hardware works for things like AI accelerator chips.




Having sketched out our cookie factory, let’s try describing it using temporal programming style.

If the oven is on and the timer has beeped,
Turn the oven off


Even though these steps are written as a numbered list, they do _not_ 
Each of these steps is an _atomic_ action. We can’t say “stir for 5 minutes”, we have to say “if the ingredients are in the mixer and the timer is at 0, start the mixer. If the mixer is running and the timer reads 300 seconds, turn the mixer off and signal the next machine”.
Every action has to “flip a switch”



### If we want a language that is “parallel by default”, then lexical ordering _can’t_ be the sole arbiter of execution order.





You’re just redefining reactive programming / functional reactive programming.
    “A := b + c” continuously assigns to a

Temporal Programming - A new name for an old programming paradigm
What do state machines, functional reactive programming, and chip design have in common?
They model computation as atomic transitions from “old” states to “new” states.

If you ask a programmer to give examples of “programming paradigms”, the two most common answers you’ll usually receive are “Procedural Programming” and “Functional Programming”. 

What do fractals, state machines, functional reactive programming, and chip design have in common?

For the purposes of this essay, we will say that they all compute “new” state from “old” state. 

x’ = f(x)

for( int i=0; i<512; i++ ) {
  float x2 = x*x - y*y + cx;
  float y2 = 2.0*x*y + cy;
  x = x2;
  y = y2;
}


for( int i=0; i<512; i++ ) {
  x@ = x*x - y*y + cx;
  y@ = 2.0*x*y + cy;
}

When we’re writing C, we implicitly assume that assigning to a variable changes the value of that variable before the next statement is executed. This can make 

## Some Background, And A Big Caveat

I’ve been a professional programmer for a bit over 25 years now. In that time I’ve worked at pretty much all levels of both the hardware and software tech stacks, from WebGL to embedded assembly code to PCB design and bringup. About 10 years ago I started thinking about a question I couldn’t find a good answer to - “What would it look like for time itself to be a first-class entity in a programming language?”

Since I am not a computer scientist, I wasn’t quite sure where to start looking for an answer. I skimmed all over the Wikipedia pages for programming languages and paradigms, browsed techy subreddits and various other forums, wrote a few toy programming languages that didn’t go anywhere, and eventually decided that I should learn Verilog and try building things for a FPGA. That led to me writing and testing a few CPU cores, which led to me looking into FPGA-based game console emulators, which led to me writing MetroBoy and GateBoy and LogicBoy and all the other Game Boy stuff that’s up on Github. At some point I realized that I wanted a tool to translate some of that code into Verilog, so I wrote Metron and built a live online demo and tutorial for it (also on Github).

And all that, in turn, led to me rethinking my original question. What if the right way to get “time” into a programming language wasn’t to try and cram it into the syntax or type system, but to define it as a sort of design pattern? We keep seeing the “atomically change old state into new state” pattern repeating across a lot of different programming contexts, so what if a programming language made that pattern easier or clearer to express? Surely someone had already done that, coined a term for it, and written a bunch of papers about it? After more Wikipedia and Scholar searches I still didn’t have a solid answer - many articles seemed to describe things _like_ what I was thinking of, but none of them generalized their ideas into something I could point at and say “Yes, that is the umbrella term for describing how time interacts with a programming language that I’ve been looking for”.

So I’m left with coining a term, something more elegant than “atomically change old state into new state” that can apply equally well to both a game’s state-machine-based AI and a Verilog barrel shifter. In honor of Leslie Lamport’s paper “The Temporal Logic of Actions”, I propose we call it “Temporal Programming”.

Again, I am not a computer scientist. It’s quite possible that what I’m describing is an old research topic hidden in some corner of the internet that somehow I’m unable to find on Google Scholar. I’m not “inventing” a programming paradigm here, just putting a label on something that already exists - if there’s already a term that fits better than “Temporal Programming”, then we should use it instead.

## Wait a second - the phrase “atomically change old state into new state” is so generic as to be meaningless. Of _course_ we change old state into new state, we do it all the time. That isn’t a paradigm, it’s just regular programming.

### Surely this has all been investigated already - why haven’t I ever heard of temporal programming before?

There have been a small number of languages that model something similar to temporal programming; you can find them in Wikipedia under the entry “Synchronous Programming Languages”.

Most of the hardware description languages (HDLs) used to design logic for FPGAs and chips also contain aspects of temporal programming - in particular the non-blocking assignment operator in Verilog “x <= y” is equivalent to “x@ = y”.

 - Leslie lamport’s temporal logic of actions
 - “Discrete synchronous programming”
 - Esterel has some uncanny similarities
    - “The language version is that of the Esterel v5 system, version v5 91. This new language version extends the previous version v5 21 by the addition of new pre operators, which makes it possible to access the previous status and value of a signal.”
“Esterel v4 was much better than Esterel v3 since it avoided state space explosion. However, it required generated circuits to be acyclic. Although this condition is standard in hardware or data-flow systems design, it turned out to be too restrictive for Esterel.”
 - Lustre - “pre p: Returns the previous value of p”
 - Signal
- “synchronous languages”
- https://en.wikipedia.org/wiki/Synchronous_programming_language
 - "the synchronous abstraction"
 - “Guarded atomic actions”
 - “An implementation can execute several rules concurrently in a clock cycle, provided some sequential execution of those rules can reproduce the behavior of the concurrent execution.”
 - “http://www.averest.org/pdf/QuartzPresentation.pdf”
 - Communicating sequential processes (CSP)
 - https://hackage.haskell.org/package/atom


## Can we go over the whole “atomic change old state into new state” in more detail?

### Atomic: 

From the code’s point of view, a program’s state can never be “partially modified”. A temporal programming language must enforce that every line of code sees the same consistent view of the universe during evaluation, even if 

This has some knock-on effects: A temporal programming language compiler _must_ be able to prove that all possible evaluation graphs contain at most one assignment 

### Change:

Unlike pure functional programming languages, state _does_ change. Unlike most imperative programming languages, “change” can include things like type changes - it’s perfectly valid to say that X’s old type is “apple” and its new type is “orange”.

Flip a coin and lock/unlock the door example

### Old/New:
A temporal program’s operations must form a directed acyclic graph - the “old” state is always read-only and “new” state must always be computed from the old state plus external inputs. Because the “direction” of computation is always explicit, the lexical ordering of statements in a block has no effect on the computation. You can read the new value of X “lexically before” you’ve assigned it, as long as you don’t create a feedback cycle. Branch statements don’t “change control flow”, they select or deselect blocks of statements for evaluation. Similarly, “calling” a function doesn’t immediately execute it, it just marks the statements in that function as belonging to the current moment’s evaluation graph.

### State
A temporal program’s state must persist even if no changes are currently being evaluated. “State” can refer to both data stored in memory and data implicitly stored in the type system (UnlockedDoor/LockedDoor). 

The phrase “compute new state from old state” sounds so generic that it’s more like a design pattern than a paradigm. Why call it a paradigm?

From the software world viewpoint it does seem to It seems much broader than a pattern, and saying that the entire FPGA/ASIC industry follows a “design pattern” sounds incongruous.

Temporal programming is not just about “computing new state from old state”, it’s about how that computation is expressed in the language - does the compiler enforce that all state changes are atomic?


## Aren’t you just talking about…

### State machines?

Yes and no. State machines can be considered to be a subset of temporal programs where one element of the program’s state (plus any external inputs) controls changes to the rest of the state. 

### Destructuring assignment?
### Transactional memory?
### Reactive Programming?
### Guarded atomic actions?

## I can already do what you’re talking about in {language} using {feature}, so why coin a new term?

Trivial way to model this program in C
auto old_state = state;
state = f(old_state);


### Imperative vs. Functional vs. Temporal

If we are going to declare “temporal” programming to be something distinct from imperative and functional programming, we need some sort of orthogonal-ish classification to distinguish between them.

 - Imperative - State changes incrementally
 - Functional - State does not change
 - Temporal - State changes atomically

This is not a great classification, but it’s a start.


### Branch Vs. Mux

Let’s look at two trivial C examples:

int result;
if (rand() > 0) {
  result = expensive_computation_a(input);
}
else {
  result = expensive_computation_b(input);
}

int result = rand() > 0 ? expensive_computation_a(input) : expensive_computation_b(input);


A software programmer looks at these examples and sees branching control flow - first we generate a random number, then we evaluate either computation A or B but never both. 

A hardware programmer sees a mux - first we evaluate _both_ computation A and B, and _then_ we look at the result of rand() to see which one to assign to “result”. To a hardware programmer the “expensive_computation” functions are _things_ - the circuit doing the computation occupies physical space on the chip, so if we want to do both A and B then we need room on the chip for both of them even if we only rarely use one.


“Phi” functions



## So what would a “pure” temporal programming language look like?

The _entire_ program state is X
State changes atomically
Type can change atomically
Code can run backwards if F is invertible
Order of expressions is mostly irrelevant. Temporal programs can be “flattened” into a list of “field’ = f(state)” expressions
In a temporal programming language, the physical structure of the code (call trees, etc) is less relevant. Statements in a block are order-invariant. Branches select blocks for evaluation but there is no “program counter” in the usual sense

A ring oscillator can’t be modeled directly in temporal programming as it has no clock and is not a pure function
What could a temporal programming language do that {lang} can’t?


## What can we express in a temporal language that we can't express in a procedural or functional language

“Is it possible to get to a state with property X from the current state?”
Does property X hold in all possible states?
Tla+ type proof stuff

Speculative execution and runahead
    Some game console emulators improve responsiveness and input latency by implementing “runahead” - given an “old” emulation state, they compute every possible “next” state that could be reached by the user pressing some combination of input buttons. Once the user input is known the emulator can immediately switch to the selected pre-computed state, display the corresponding game image, and then begin computing the next set of possible states.
    This is not right https://docs.libretro.com/guides/runahead/


## Temporal Programming and Concurrency
“Concurrency” is irrelevant at the language level. 
“Concurrent programming would be much simpler if the nondeterminism were controlled in some way, so that it is not visible to the programmer.” - Van Roy
“For example, if each of two threads assigns a variable cell to a different value, then each of the two values can be observed:”
This cannot be expressed in a temporal programming language - by definition a variable can have only one future
“concurrency naturally implies nondeterminism.”
No it doesn’t, not in this context








## Open Questions
 - What do statements like “x@-2 = y@+3” even mean?
 - A sufficiently smart compiler doesn’t need to keep as many intermediate copies
 - Large programs are expression graphs that can be incrementally evaluated in parallel by a “swarm” of worker bees
 - What would a processor designed specifically for temporal programs look like? Something in between a CPU and a FPGA?
 - “Programming Paradigms for Dummies: What Every Programmer Should Know”
 - “discrete synchronous programming”
 - In this paradigm, a program waits for input events, does internal calculations, and emits output events
 - “Waits” is meaningless in TPL
 - Doesn’t discuss internal state change
 - “output events from one subcomponent are instantaneously available as input events in other subcomponents”
     - Well, not exactly. 











Destructuring assignment: Sure, you can use “(a, b) = (b + 1, a + 1)” in some languages to atomically-ish assign multiple variables, but that doesn’t 













 - So what's this "temporal" programming thing, anyway?

 Well, it's my attempt to define a style of programming that's in between "procedural" programming and "functional" programming.

 At a low level it's a pretty simple idea, though it has some interesting repercussions.

 Let's look at a trivial program

 int a = 10;
 int b = 20;
 a = b + 1;
 b = a + 1;
 printf("a = %d, b = %d\n", a, b);
 
 In a procedural language, this will print "a = 21, b = 22". If we flip the order of assignments, we get 

 int a = 10;
 int b = 20;
 b = a + 1;
 a = b + 1;
 printf("a = %d, b = %d\n", a, b);

 we now get "a = 12, b = 11". 

 If we could somehow simultaneously evaluate the two assignments, we might expect to see "a = 21, b = 11".

 But we can't express simultaneous anything in C, it's always sequential. So let's introduce some new notation

 a' = b + 1;
 b' = a + 1;

 The tick mark indicates the _next_ value of a variable, no tick indicates the _previous_ value of a variable. This may look familiar if you've played around with Mandelbrot sets, which are defined by the recurrence relation "z' = z^2 + c".

 There's some ambiguity here, however - if I call a function that writes to a variable "foo'", when does that write actually take effect? If I read from "foo" after calling that function and then I print the value of "foo", does it print the old or new value of foo? To answer that question, we're going to need some more notation:

 int a = 10;
 int b = 20;
 temporal {
    a' = b + 1;
    b' = a + 1;
 }
 printf("a = %d, b = %d\n", a, b);

Inside a temporal block, only ticked variables may be written to. Reading from a variable always gives the _old_ value. After the temporal block, reading from a variable gives the _new_ value.

It's perfectly legal to read from a ticked variable in a temporal block, provided that doing so doesn't create an evaluation loop. For example

 int a = 10;
 int b = 20;
 temporal {
    a' = b' + 1;
    b' = a + 1;
 }
 printf("a = %d, b = %d\n", a, b);

will print "a = 12, b = 11".

However, 

 int a = 10;
 int b = 20;
 temporal {
    a' = b' + 1;
    b' = a' + 1;
 }
 printf("a = %d, b = %d\n", a, b);

will not compile - in order to evaulate a' we need to know b', and in order to evaluate b' we need to know a'.