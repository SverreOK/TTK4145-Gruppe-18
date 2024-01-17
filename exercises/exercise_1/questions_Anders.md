Exercise 1 - Theory questions
-----------------------------

### Concepts

What is the difference between *concurrency* and *parallelism*?
> concurrency is when you have more than one task running in overlapping time periods. but they dont have to be running simultaniously, two tasks could be interchangebly be excecuting code as handled by a scheduler. Parallelism is when two task run simultaniously excecuting code at the same time, on multiple cores/threads.

What is the difference between a *race condition* and a *data race*? 
> A race condition is when two or more processes are running over eachother and the outcome depends on when each of the processes end up running what code, this can be quite random depending on code excecution timing and scheduler behaviour.

A data race is a spesific type of race condition where multiple processes are accessing the same memory at the same time. This will mean that they will be trying to change the same variable without knowing what the other process is doing and the result can be random as controlled by a scheduler/depending on which finishes first/ends last.
 
*Very* roughly - what does a *scheduler* do, and how does it do it?
> A scheduler is what controls which threads are excecuting code and running and when it does so.


### Engineering

Why would we use multiple threads? What kinds of problems do threads solve?
> You will generally use multiple threads to be able to handle many different processes running. It also allows for multiprocessing/parallelism which can speed up excecution time.

Some languages support "fibers" (sometimes called "green threads") or "coroutines"? What are they, and why would we rather use them over threads?
> fibres/green threads, are threads which are scheduled/handled by the program itself instead of the OS' scheduler. it has the potential to be more optimized and efficient than the OS' scheduler, but isnt multiprocessed meaning not truly concurrent.

Coroutines are simmilar to fibres as they allow a program to decide when multiple "threads" are able to run. But it differs in that these "threads"/routines communicate/cooporate on when to yield to other processes and let them run. These are also more lightweight than the normal OS' run threads/scheduler.

Does creating concurrent programs make the programmer's life easier? Harder? Maybe both?
> depending on the requirements of the task at hand it can be either harder or easier. If there are strict performance requirements then a concurrent program can have a much better potential to fulfill this and run fast. But this will generally be at the cost of more difficult architecture of a program, generally a single core program will be much less prone to bugs, and when there are they can often be easier to find and solve compared to a concurrent program.

What do you think is best - *shared variables* or *message passing*?
> i am a message passing enjoyer. It seems like once set up makes for a more robust framework to create software around. (although the "simplicity" of shared variables for low level programming is nice for ease of implementation)


