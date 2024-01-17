Exercise 1 - Theory questions
-----------------------------

### Concepts

What is the difference between *concurrency* and *parallelism*?
> Concurrency is when multiple threads start, run and stop in an overlapping fashion, for example several threads running on a single core and yielding to each other when not doing anything important. Parallelism is when several threads run at the same time in parallel, a multicore system would be an example of this.

What is the difference between a *race condition* and a *data race*? 
> A data race is when several threads race to access a mutable object, a race condition is when the order of completion of threads affects the correctness of a program. Data races can cause race conditions.
 
*Very* roughly - what does a *scheduler* do, and how does it do it?
> A scheduler decides which threads get to run at which time. It does this roughly by going through a queue and either waiting for the thread running to yield or by interrupting it (pre-emptive scheduling) to allow the next thread in line to run.


### Engineering

Why would we use multiple threads? What kinds of problems do threads solve?
> One very compelling advantage of using multiple threads is that it can allow your program to do something useful while for example waiting for something. For multicore systems it also allows for parallelism, which can also increase the speed of the program.

Some languages support "fibers" (sometimes called "green threads") or "coroutines"? What are they, and why would we rather use them over threads?
> Fibers can be explained as threads where the execution path is only interrupted when it yields, in contrast to normal threads which may be pre-emptively interrupted. They are useful because of their increased data integrity and less expensive context switches. 

Does creating concurrent programs make the programmer's life easier? Harder? Maybe both?
> Using threads can, on one hand, make the code easier to organize and understand, but on the other hand it can introduce a number of issues like deadlocks and race conditions if the programmer is not careful. I would say a bit of both.

What do you think is best - *shared variables* or *message passing*?
> I think it depends. Message passing sounds safer and easier to conceptualize in my opinion, so at this moment I think it is what I would prefer.