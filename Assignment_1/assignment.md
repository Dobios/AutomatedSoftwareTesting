# Assignment 1 - Code Coverage

## Overview

This assigment is designed to put coverage metrics into practice 
by introducing you to `gcov`. The assignment will guide you through
1. compiling C/C++ code to support code coverage
2. using gcov to extract code coverage metrics
3. building a test suite to achieve full coverage on a few sample programs
4. analyzing test suite coverage on a real-world project

## Description

We will start by working on a few simple toy programs to familiarize yourself with
`gcov`. The instructions are designed for gcc running on a common Linux distribution. 
However, it should be possible to complete the assignment on another OS and compiler such
as clang on macOS with some minor modifications to the compilation flags.

### Introduction to gcov

Gcov is a source code coverage analysis and profiling tool included in the GNU Compiler
Collection. In order to use it, you will need to compile your code with some special flags.
We will start exploring this with a hello world program that trivially achieves 100% 
coverage in its only execution path. Go into the `toy0` directory, where you will see a
single `hello.cpp` file. Compile it as follows.
```
$ g++ --coverage -g -O0 hello.cpp
```
The `--coverage` flag is equivalent to `-fprofile-arcs -ftest-coverage`, which will add 
additional instructions to keep track of statement execution for gcov to analyze. Adding
debug information and disabling optimizations using `-g -O0` helps map instructions back to 
lines of code. 

Once compiled, you should see a `hello.gcno` notes file, which stores a mapping of
basic blocks to source code. Once this is done, we need to run the executable, at which point 
profiling and coverage data will be generated and written to a `hello.gcda` data file. 
Note that you may need to remove existing data files if you compile and run the binary again.
```
$ ./a.out
Hello, gcov!
```
At this point, we can invoke `gcov` to read the notes and data file to give us a coverage report. 
You can do this by invoking it on the source file you're interested in.
The `-r` flag makes sure only relative imports are considered in the coverage, which excludes
standard library files such as `<iostream>`. The output should look like this.
```
$ gcov -r hello.cpp
File 'hello.cpp'
Lines executed:100.00% of 3
Creating 'hello.cpp.gcov'
```

This will create a `hello.cpp.gcov` output file. Make sure to include it in your submission.
Congratulations, you've just analyzed your first program using `gcov`!

## Toy Programs

The following subtasks all follow the same format: in the respective `toy<nr>` directories, you 
will find a `test<nr>.cpp` file with an incomplete test suite for a function or class that is 
contained in the `<program>.cpp` and `<program>.h` files. Your task is to extend the set of 
tests in `test<nr>.cpp` to achieve 100% coverage in `<program>.cpp`. Make sure to include 
`test<nr>.cpp` and the resulting `.gnco` file in your submission.

### FizzBuzz

Have a look at `test1.cpp` in the `toy1` directory. You'll notice a simple test function,
an array of test cases and a main function to run them. You should add exactly two test cases to
achieve 100% line coverage in `fizzbuzz.cpp`. You are free to modify the existing one if you like, 
it is only there to demonstrate how to use the framework. There are also a few questions for you 
to answer. Invoking `gcov` works in a very similar way to before. 
Initially, it should look like this.
```
$ g++ --coverage -g -O0 *.cpp
$ ./a.out
Passed 1/1 test cases!
$ gcov -r fizzbuzz.cpp
File 'fizzbuzz.cpp'
Lines executed:66.67% of 9
Creating 'fizzbuzz.cpp.gcov'
```

### Linked List and Binary Tree

The next two tasks are structured identically. In each case, you are given a rough implementation
of a common data structure. There are no questions to answer here, but there is more code 
to cover including some branching, which will make it a bit more difficult.
You are free to either make `testcase0` more complex or add more test cases
to achieve 100% line coverage. Note that the semantics of passing the test
case don't matter here. A simple `return true` at the end is fine.

Since it will be harder to reason about which parts of the code aren't covered yet, you may find
the information in the `.gcov` output file useful. Alternatively, `-t` will print this information
directly to stdout. Optionally, you can also use [gcovr](https://gcovr.com/en/stable/) to 
generate an interactive report of your current coverage including branches
using `gcovr --html-details <filename>.html`. There might be compatibility problems with some
browsers like Safari.

### Primality Test

Finally, you are given an optimized version of a trial division test to check for prime numbers.
There aren't many lines of code, but quite a few conditions. Consequently, we change our
metric here. We want the test suite to achieve 100% <b>branch</b> coverage. 
More specifically, we want each branch to be taken at least once. We can get `gcov` to 
report this with its `-b` flag. Initially, the output should look like this.
```
$ gcov -r -b prime.cpp
File 'prime.cpp'
Lines executed:33.33% of 9
Branches executed:12.50% of 16
Taken at least once:6.25% of 16
No calls
Creating 'prime.cpp.gcov'
```
Note that branches with complex conditions (including ||, &&) are compiled down to 
multiple branches with short-circuit evaluation. This means that one test case each to 
have the entire condition evaluate to true and false respectively is not sufficient for each
branch to be taken once. Also note that this is not condition coverage
since short-circuit evaluation causes dependent evaluation of sub-conditions.


## Coverage in the Real World

As the final task of the assignment, we will use the knowledge obtained so far 
and apply it to an actual open-source project. As a first step, 
clone the [Z3 repository](https://github.com/Z3Prover/z3) from GitHub.
The Z3 Theorem Prover is an SMT solver targeted at problems in software verification 
and program analysis. Its test suite therefore essentially consists of meta tests, 
which makes their comprehensiveness all the more critical. If you haven't done so already, 
you will want to install `gcovr` at this point to take the
collection and summarization of `gcov` data files across subdirectories off your hands. 
Z3 uses the Ninja build system, which you also need to install.
```
$ sudo apt install gcovr ninja-build
```
You can build Z3 with the following commands. In particular, have a look at the cmake
configuration. Just as before, we use the `--coverage` flag to get the 
compiler to generate the necessary instructions and files for coverage tracking.
```
$ cd z3
$ mkdir build 
$ cd build
$ CXXFLAGS=--coverage LDFLAGS=-lgcov cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=./install -G "Ninja" ../
$ ninja
```
At this point, try to invoke `gcovr` without any arguments to make sure the note
files have been generated correctly. You should see a list of files and 0% coverage.
If the output looks like this, something went wrong, and you should try recompiling from scratch.
```
$ gcovr
------------------------------------------------------------------------------
                           GCC Code Coverage Report
Directory: .
------------------------------------------------------------------------------
File                                       Lines    Exec  Cover   Missing
------------------------------------------------------------------------------
------------------------------------------------------------------------------
TOTAL                                          0       0    --%
------------------------------------------------------------------------------
# no bueno
```
Z3 has two separate test suites. One of them is contained within the Z3 repo itself.
They can also be built using ninja. Running them will take a few minutes, maybe even an hour 
depending on the speed of your machine.
```
$ ninja test-z3
$ ninja install
$ ./test-z3 -a
```
Once done, let `gcovr` collect the coverage information and export it to an XML file.
With the `-s` flag, you get a brief summary of line and branch coverage after the export 
is done. We also use `--gcov-ignore-parse-errors` to generate the XML even if there is 
something wrong with one of the data files. You should see around 10% line coverage in your report.
```
$ gcovr --gcov-ignore-parse-errors --xml test-z3.xml --xml-pretty -s
```
Next, we will run Z3 on sample code for a few of its bindings. Since these snippets
are designed to showcase how to use the bindings, they cover a good range
of functionality and should be able to increase coverage.
```
$ ninja c_example
$ ninja cpp_example
$ ninja c_maxsat_example
$ examples/c_example_build_dir/c_example
$ examples/cpp_example_build_dir/cpp_example
$ examples/c_maxsat_example_build_dir/c_maxsat_example ../examples/maxsat/ex.smt
```
Again, use gcovr to export the coverage information to XML. Then,
exit out of the build directory to run the final suite of tests.
```
$ gcovr --gcov-ignore-parse-errors --xml examples.xml --xml-pretty -s
$ cd ..
```
Clone the [z3test repo](https://github.com/z3prover/z3test). The scripts are configured to use 
clang, so just for this part, you will need to install in case it's not on your system already.
```
$ sudo apt install clang
```
There are two types of tests here. Some of them are regression tests, containing bugs from 
previous versions to prevent them from reappearing. Z3test also contains dedicated coverage
tests. Run them using the respective Python scripts.
```
$ python z3test/scripts/test_benchmarks.py build/z3 z3test/regressions/smt2
$ python z3test/scripts/test_coverage_tests.py build/install z3test/coverage/cpp  
```
Export the coverage information one last time. It should've improved significantly
and be between 40% and 50% at this point.
```
$ gcovr --gcov-ignore-parse-errors --xml z3test.xml --xml-pretty -s
```
The main takeaway from this assignment should be that measuring code coverage is feasible in 
practice. The hard part is writing tests that get anywhere close to covering the entirety
of a project's codebase. You may have already had to invest a bit of time to think of
every eventuality in some of the toy programs. This only gets more difficult in the real
world as you can see from the results on Z3.

## Submission and Grading

You can achieve a total of 100 points for this assignment. Please submit your
files as follows, with one directory per subtask in a zip file. If this
structure is ignored, we may deduct up to 20 points from your total. 
Note the achievable points in parentheses next to each subtask.

- \<matriculation_number\>-\<firstname\>-\<lastname\>.zip
  - toy0 (10 pts)
    - hello.cpp.gcov
  - toy1 (15 pts)
    - test1.cpp
    - fizzbuzz.cpp.gcov
  - toy2 (15 pts)
    - test2.cpp
    - BinaryTree.cpp.gcov
  - toy3 (15 pts)
    - test3.cpp
    - LinkedList.cpp.gcov
  - toy4 (20 pts)
    - test4.cpp
    - prime.cpp.gcov
  - z3 (25 pts)
    - test-z3.xml
    - examples.xml
    - z3test.xml
