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

