# File Merge and Comparator

A C++ project that compares and merges text files line by line.
Built with object-oriented programming as a Project Based Learning (PBL) assignment.

## Features
- **Compare** two files and see which lines are only in A, only in B, or common,
  with a similarity percentage.
- **Merge** two files in three ways:
  1. Append: all of file A, then all of file B
  2. Smart merge: common lines appear once, unique lines from both are kept
  3. Conflict markers: where the files differ, both versions are kept
     inside `<<<<<<<`, `=======` and `>>>>>>>` markers (like Git)
- Shows a clear error if a file cannot be opened.

## Project structure
    textfile.h     TextFile class: loads, saves and stores the lines of a file
    comparator.h   Comparator class: finds differences using the LCS algorithm
    merger.h       Merger base class and AppendMerger, SmartMerger, ConflictMerger
    main.cpp       Menu-driven program
    a.txt, b.txt   Sample files for testing

## Build and run
    g++ -std=c++17 main.cpp -o filetool
    ./filetool

Enter file names (not file contents) when asked, for example `a.txt` and `b.txt`.

## Example
Comparing `a.txt` and `b.txt`:

    - [A:  2] banana
    + [B:  2] blueberry
    + [B:  5] elderberry

    Summary: 3 same, 1 only in A, 2 only in B | similarity 50.0%

## How the comparison works
It finds the Longest Common Subsequence (LCS) of the two files' lines, the same
idea used by the Unix `diff` tool. Lines in the LCS are common; the rest are
differences.

## OOP concepts used
| Concept | Where |
|---|---|
| Classes, encapsulation, constructors | `TextFile` |
| Static members | `filesLoaded`, `mergesDone` |
| Operator overloading, friend function | `==`, `+`, `<<` in `TextFile` |
| Function overloading | `Comparator::compare()` |
| Inheritance, virtual functions, polymorphism | `Merger` and its three derived classes |
| Dynamic memory | `new` and `delete` in `main.cpp` |
| Exception handling | `try/catch` in `main.cpp` |
| STL and file streams | `vector`, `ifstream`, `ofstream` |
