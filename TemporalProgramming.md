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