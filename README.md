# Ball Game
## Rules

A Ball Game is played with M balls on a square board. The board is divided into NxN unit squares, and M of those unit squares contain holes. Balls and holes are numbered from 1 to M. The goal of the Ball game is to roll each ball into the hole that has the same number. 

A game board may contain walls. Each wall is one unit long and stands between two adjacent unit squares. Two squares are considered adjacent if and only if they share a side. 

At the beginning of the game, all balls are placed on the board, each in a different square. A “move” consists of slightly lifting a side of the game board. Then all balls on the board roll downward toward the opposite side, each one rolling until it meets a wall or drops into an empty hole, or until the next square is already occupied by another ball. Balls roll subject to the following restrictions:

- Balls cannot jump over walls, other balls, or empty holes.
- Balls cannot leave the board. (The edge of the board is a wall.)
- A unit square can contain at most a single ball at any one time.
- When a ball moves into a square with a hole, the ball drops into that hole. The hole is then filled, and other balls can subsequently roll over the hole. A ball in a hole can never leave that hole. 

The game is over when each ball has dropped into a hole with the corresponding number. 

The figure below illustrates a solution for a game played on a 4x4 board with three blue balls, three holes and a wall. The solution has five moves: lift the east side, lift the north side, lift the south side, lift the west side, lift the north side.

## Input 
The input file (**input.txt**) contains several test cases. The first line of each test case contains three numbers: the size N (2 ≤ N ≤ 40) of the board, the number M (M > 0) of balls, and the number W of walls.  

Each of the following 2M lines contains two integers. The first integer is a row location and the second is a column location. The first M of those lines represent the locations of the balls, where ball#1 is on the first line, ball#2 on the second, and so on. The last M of those lines represent the locations of the holes, with the location of hole#1 coming first, hole#2 coming second, and so on.  

Finally, the next W lines represent the wall locations. Each of those lines contains four integers: the first pair are the row and column of the square on one side of the wall and the second pair are the row and column of the square on the other side of the wall.  

Rows and columns are numbered 0..N-1. 

The input file ends with a line containing three zeroes. 

## Output
For each test case, will be printed the case number (beginning with 1), the minimal number of moves to win the game and the sequence of the moves (‘N’ to lift the north side, ‘E’ to lift the east side, ‘S’ to lift the south side, ‘W’ to lift the west side).  

If the game cannot be won, will be print the word “impossible”.