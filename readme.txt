# Authors:
# Ayush Kumar (170195)
# Deepesh Kumar Lall (170236)



To use this SUDOKU+ Solver & Generator,

1. Make sure that minisat is installed on your system and accessible by command line with the command 'minisat'.
2. Compile solver.cpp : "g++ -std=c++11 solver.cpp".
3. Run the binary created : "./a.out".
4. Enter the full filename (e.g. sudokufile.txt) which contains the unsolved Sudoku+ at the prompt that appears. Note that this file should be in the same directory as solver.cpp.
5. The solved Sudoku+ is published in sudokusol.txt file.
6. The SAT encoding of the given Sudoku+ is also published in sudokusat.txt file.
7. The program also automatically generates a new Sudoku+ and publishes it in newsudoku.txt file.
8. All files are created in the same directory.
9. The solved and generated sudoku is also printed on the command line interface.


