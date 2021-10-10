CSE 375/475 Assignment #2 (20 points)
Fall 2021

Introduction:

  The purpose of this assignment is to parallelize an existing sequential program.
  Intel Threading Building Blocks (TBB) is an option, but you might find other
  approaches such as using the OpenMP, OpenCilk, CilkPlus, or OpenCL to parallelize
  a sequential program. If you want to explore a toolchain that is different
  from those listed above, please find my approval first.

  The provided program is a serial implementation of the well-known k-means algorithm
  for clustering. It's a simple implementation, which is borrowed from this repository:

                    https://github.com/marcoscastro/kmeans

  with few customizations I made. Two datasets are also included to test your application.
  The provided synthetic datasets might not represent the best candidate to show the
  effectiveness of your parallel code. By searching online you can find many repositories
  for k-means datasets that can be tested. For example, a known repository is available at:

				http://archive.ics.uci.edu/ml/index.php

  The selection of the dataset is up to you. Each of the available dataset has particular
  characteristics. Since only you can know where parallelization can be exploited in your
  code, select your dataset accordingly.
  Different datasets might have different formats; consider adapting them to fit your
  application (or change the way your application reads an input dataset). Do not forget
  that your sequential application should also be tested under the same dataset for
  comparison.

  Your goal is to get the best speedup you can achieve over the provided program by
  exploiting parallelism and other techniques discussed in class. Keep in mind that
  performance improvement might also come from using advanced instructions, such as SIMD,
  fine tuning important runtime parameters (e.g., number of threads, size of structures),
  redesign part of the solution to break code dependency and increase parallelism.

  Final note is about concurrency. If you discover that concurrency can help achieving
  a better speedup, feel free to explore it ONLY AFTER you parallelized your code without
  flow or data dependency (meaning without race conditions). It would be great to see
  a comparison between the sequential, the parallel code, and the concurrent code. 

  ***** Few advises on how to implement parallelism and get high performance *****

  The first step is to get familiar with the K-means problem and the algorithmic solution.
  The provided application implements the Lloyd's algorithm and is made by multiple steps.
  The textbook has a chapter explaining k-means and provides snippets of code to increase
  understanding of how to provide parallelism.
  The second step is to analyze the provided application and understand which parts
  can be parallelized because they do not include data and flow dependencies.
  Third step is to focus on those parts that contain dependency and design an alternative
  approach to achieve the same goal but in a way parallelism can be exploited.
  In the forth step you should care about optimizations to increase performance further.
  K-means uses the Euclidean distance calculation to understand if a point belongs to a
  certain cluster. SIMD instructions might help in this case, especially when the
  the dataset has many dimensions.
  
  ***** Evaluation *****

  It is REQUIRED to use the machines available at sunlab.cse.lehigh.edu. An evaluation
  performed on your own laptop will NOT BE considered sufficient, unless a specific
  authorization has been granted by me.

  In order to achieve speedup, it is important to check that the machine you are logged
  is not overcommitted. The sunlab machines can run 8 threads simultaneously
  ("less /proc/cpuinfo"), therefore if some other user is running along with you,
  you might not see the expected performance improvement.
  Use "top" (or alternative ways) to check the commitment of the machine you are using.
  If overcommitted, move to another machine. Remember that the "users" command shows
  the users currently logged in the machine.

  The deadline for submission is Friday October 8, 2021 at 12PM (noon).
  There is enough time to allow each of you to study/design/develop/debug.
  Don't waste time!

  The submission should be done through coursesite.lehigh.edu and should consist of:
  - the source code of your assignment;
  - a document containing at least three performance plots including the sequential application
  vs your parallel version. Decide which parameter must be varied in order to produce the
  three plots.
  
  I will grade assignments during a one-on-one meeting with you.
  You must be able to explain all the decisions make in your submission.


Step 0: Get the Code

  The code is attached to the coursesite assignment.

Step 1: Change your Compiler

  You will need to use a newer compiler than the default one on the sunlab
  machines. The current compiler is gcc 4.4.7.
  The assignment contains C++11 features, which won't compile with GCC version 4.4.7.
  In order to switch gcc compiler, you have to load one of the modules already installed
  for gcc:
    execute: 'gcc --version' --> you should see "gcc (GCC) 4.4.7 20120313 (Red Hat 4.4.7-18)";
    execute: 'which gcc' --> you should see "/usr/bin/gcc";
    execute: 'module avail' --> you should be able to see different version of the gcc
    compiler. You application will require at least gcc-5.4.0.
    In order to load that module
    execute: 'module load gcc-5.4.0'
    execute: 'which gcc' --> you should see "/opt/gcc-5.4.0/bin/gcc";
    execute: 'gcc --version' --> you should see "gcc (GCC) 5.4.0";

  Now you can compile your application. You must change the version of your compiler every time
  you start a new terminal window. Note that not all the distributed source files initially compile.


Step 2: Install and link the desired toolchain (e.g., TBB, CilkPlus, ...).

  If you decide to go for Intel TBB, the following steps should be sufficient to install
  TBB using the provided binary files. Instructions are not for the latest version;
  if you want to go with that, just apply the same instructions to a newer binary.

  Download from here: https://github.com/01org/tbb/releases

	---> wget https://github.com/01org/tbb/releases/download/2018_U1/tbb2018_20170919oss_lin.tgz
	---> tar -xvzf tbb2018_20170919oss_lin.tgz (-- takes a couple of minutes)
	---> cd tbb2018_20170919oss/bin
	---> source tbbvars.sh intel64 linux auto_tbbroot

  The last command has to be executed every time you open a new terminal.

  Note that, if you decide to build (and not just installing from binary) your toolchain,
  your should be careful in not exceeding your space quota. If additional space is needed,
  please find alternative solutions or contact me.


Step 3: Test the provided serial k-means solution.
  The provided application loads the whole dataset at the beginning of the execution.
  Once compiled (and the binary "kmeans" is produced), I use the following command line:
  		cat datasets/XXXX.txt | ./kmeans 
  Feel free to change it if you prefer another way.


Step 4: Implement a parallel version of the given serial k-means solution.

