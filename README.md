**** Change your Compiler on Sunlab

  You will need to use a newer compiler than the default one on the sunlab
  machines. The current compiler is gcc 4.4.7.
  The assignment contains C++11 features, which won't compile with GCC version 4.4.7.
  In order to switch gcc compiler, you have to load one of the modules already installed
  for gcc:
    execute: 'gcc --version' --> you should see "gcc (GCC) 4.4.7 20120313 (Red Hat 4.4.7-18)";
    execute: 'which gcc' --> you should see "/usr/bin/gcc";
    execute: 'module avail' --> you should be able to see different version of the gcc
    compiler. Your application will require at least gcc-5.4.0.

  In order to load that module
    execute: 'module load gcc-5.4.0'
    execute: 'which gcc' --> you should see "/opt/gcc-5.4.0/bin/gcc";
    execute: 'gcc --version' --> you should see "gcc (GCC) 5.4.0";

  Now you can compile your application. A Makefile has been provided. Given the freedom left
  to each of you in implementing your solution, feel free to modify the Makefile to
  include your features.
  You must change the version of your compiler every time you start a new terminal window.
  Note that not all the distributed source files initially compile.

**** Code -> Debug -> Code
  All the files in the 'src' folder contain a specification of how they are expected
  to be coded. Each student is expected to understand how these source files relate
  with each other. You might find directions not so strict. This has been done
  intentionally to give students freedom to design their own solutions. Don't be
  afraid to explore.

**** Recommendation:
  Please do not overload the sunlab machines with an unreasonable large amount of parallel threads.
  To play with parallelism, you should be satisfied enough to run with 4 to 24 threads. Recall that
  nodes in the sunlab cluster have 8 CPUs (try 'less /proc/cpuinfo'), therefore any configuration
  with a higher number of parallel threads won't produce an actual parallel execution.
  To check the real-time utilization of the machine on sunlab, use the "top" command. To restrict
  the list of processes to those associated with your account only, use "top -u your-sunlab-user".

**** Submission Instructions:

  To submit your solution, use coursesite. You have unlimited submissions until
  the deadline. Only the last uploaded version counts.

**** Face-to-Face Meeting:

  I will review your solution during a one-on-one meeting with you.  It will also
  provide a chance to ask questions about the design choice you made. If the solution
  satisfies the given specification, a maximum of 6 extra points will be given.

**** Due Date:

  This assignment is due at 11:59 PM on September 10, 2021.

**** STEPS
// Step 1
// Define a simplemap_t of types <int,float>
// this map represents a collection of bank accounts:
// each account has a unique ID of type int;
// each account has an amount of fund of type int.

// Step 2
// Populate the entire map with the 'insert' function
// Initialize the map in a way the sum of the amounts of
// all the accounts in the map is 100000

// Step 3
// Define a function "deposit" that selects two random bank accounts
// and an amount. This amount is subtracted from the amount
// of the first account and summed to the amount of the second
// account. In practice, give two accounts B1 and B2, and a value V,
// the function performs B1-=V and B2+=V.
// The execution of the whole function should happen atomically:
// no operation should happen on B1 and B2 (or on the whole map?)
// while the function executes.

// Step 4
// Define a function "balance" that sums the amount of all the
// bank accounts in the map. In order to have a consistent result,
// the execution of this function should happen atomically (coarse grain lock?):
// no other deposit operations should interleave.

// Step 5
// Define a function 'do_work', which has a for-loop that
// iterates for config_t.iters times. In each iteration,
// the function 'deposit' should be called with 95% of the probability;
// otherwise (the rest 5%) the function 'balance' should be called.
// The function 'do_work' should measure 'exec_time_i', which is the
// time needed to perform the entire for-loop. This time will be shared with
// the main thread once the thread executing the 'do_work' joins its execution
// with the main thread.

// Step 6
// The evaluation should be performed in the following way:
// - the main thread creates #threads threads (as defined in config_t)
//   << use std:threds >>
// - each thread executes the function 'do_work' until completion
// - the (main) spawning thread waits for all the threads to be executed
//   << use std::thread::join() >>
//	 and collect all the 'exec_time_i' from each joining thread
//   << consider using std::future for retireving 'exec_time_i' after the thread finishes its do_work>>
// - once all the threads have joined, the function "balance" must be called

// Make sure evey invocation of the "balance" function returns 100000.

// Step 7
// Now configure your application to perform the SAME TOTAL amount
// of iterations just executed, but all done by a single thread.
// Measure the time to perform them and compare with the time
// previously collected.
// Which conclusion can you draw?
// Which optimization can you do to the single-threaded execution in
// order to improve its performance?

// Step 8
// Remove all the items in the map by leveraging the 'remove' function of the map
// Destroy all the allocated resources (if any)
// Execution terminates.
// If you reach this stage happy, then you did a good job!

// Final step: Produce plot
// I expect each submission to include at least one plot in which
// the x-axis is the concurrent threads used {1;2;4;8}
// the y-axis is the application execution time.
// The performance at 1 thread must be the sequential
// application without synchronization primitives

// You might need the following function to print the entire map.
// Attention if you use it while multiple threads are operating

