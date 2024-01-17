Exercise 1 - Theory questions
-----------------------------

### Concepts

What is the difference between *concurrency* and *parallelism*?
> Concurrency is when threads are run intertwined and parallellism is when threads are run at the same time in parallell.

What is the difference between a *race condition* and a *data race*? 
> A race condition occurs when the behaviour of the program depends on timing of events. When multiple threads access the same variable, the final outcome will depend on the timing or order of execution. A data race occurs when you have 2 or more different instructions from different threads accessing a shared variable, where at least one is a write instruction.
 
*Very* roughly - what does a *scheduler* do, and how does it do it?
> A scheduler decides which threads should run. Threads often have a priority, which the scheduler takes into consideration.


### Engineering

Why would we use multiple threads? What kinds of problems do threads solve?
> It allows us to build concurrent programs, which enables a core to run different operations simultaneously. Programs get more modular as well. If using threads and something important happens that needs to be handled, the OS can prioritize the thread that handles it.

Some languages support "fibers" (sometimes called "green threads") or "coroutines"? What are they, and why would we rather use them over threads?
> Coroutines support concurrency without parallellism. This isn't that "realtime", but there is however no need for mutexes and semaphores, etc. For simple operations such as I/O one would probably use them over threads.

Does creating concurrent programs make the programmer's life easier? Harder? Maybe both?
> I think it creates the life easier for programmers working on the same project and for a single programmer as well. However there might be some unpredictable behaviour that shows up when using threads instead of a superloop. This makes it harder as well.

What do you think is best - *shared variables* or *message passing*?
> Personally more inclined to global variables with mutex, as that is what I have used previously. However I can see that message passing is more suited towards systems requring more safety and scalability.