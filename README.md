# Sudoku-Solver

A simple Sudoku+ Solver and Generator using MiniSAT. Built as assignment for Course CS202 : Logic for Colmputer Science. Sudoku+ is an extended version of a normal Sudoku with the extra constraint that not only each row, each column and each 3 x 3 box, but also both main diagonals must contain distinct numbers from 1 to 9.

Larger Sudoku+ sizes can be solved as well with this sudoku-solver as long as the size is a perfect square and not too large to be limited by the computation speed of your PC.

## Dependencies
You must have [MiniSAT](http://minisat.se/) installed on your system and accessible through command line. MiniSAT is an open-source [SAT Solver](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem) and is available for free. OSX users can install is easily with [Homebrew](https://brew.sh/).
```bash
brew install minisat
```

## Build and Run
To run this sudoku+ solver, follw these steps :

1. Make sure that minisat is installed on your system and accessible by command line with the command 'minisat'.
2. Compile solver.cpp : ``g++ -std=c++11 solver.cpp ~o sudokusolver``
3. Run the binary created : ``sudokusolver``
4. Enter the full filename (e.g. sudokufile.txt) which contains the unsolved Sudoku+ at the prompt that appears. Note that this file should be in the same directory as solver.cpp.
5. The solved Sudoku+ is published in sudokusol.txt file.
6. The SAT encoding of the given Sudoku+ is also published in sudokusat.txt file.
7. The program also automatically generates a new Sudoku+ and publishes it in newsudoku.txt file.
8. All files are created in the same directory.
9. The solved and generated sudoku is also printed on the command line interface.

