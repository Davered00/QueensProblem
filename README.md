# QueensProblem

What is it?
-------------
This program enumerates and lists the solutions for the n-Queens Problem for n from 1-12. For those unfamiliar, the n-Queens problem considers a chessboard of size nxn and first asks how many ways can one place n queen chess pieces such that none of the queens can capture another queen in one move. As a follow-up problem, one can also ask how many ways are there, under the restriction that solutions that differ only by rotations or reflection are equivalent.

For more information on the problem, see https://en.wikipedia.org/wiki/Eight_queens_puzzle

Documentation
-------------
This particular implementation of the n-Queens problem in C++11 first uses a simple backtracking algorithm where solutions are generated row-wise, one queen at a time. As each queen is placed on the board, the program checks if the problem's condition is violated, and if so, moves the queen it put down to the next available spot. If there is no next available spot, the queen is removed, and the previous queen is moved instead. Whenever all the queens are placed, the orientation is added to the solution queue and the program proceeds from there as if the condition were violated. This is repeated until there are no more available spots for the first queen to move.

To solve the follow-up problem, I generate the seven other arrangements of the each item in the list, compare them to every other item later in the list, and remove any matches. For the added challenge (but less efficiency, of course) in this step, I decided to use a queue to store the data instead of a doubly-linked list.

By default, only the trimmed orientations are printed. To print them all, run the program with the [trim] flag of 0. Ex.: ./queens 4 0


