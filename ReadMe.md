# A C++ library to generate Sudoku levels on the fly
This was written as a DLL and used within a Game Maker Language GUI App.
sudoku.dll(c) 2005 Amiya Patanaik.
DLL written in visual c++ .net 2005.

* creator: Amiya Patanaik. 
* Ideas: Taken from sudoku python script by Arel Cordero
* date: Nov 30, 2005

**description**: On the fly generation of sudoku.  The sudoku is generated as a string.
Example: If the generated sudoku is...

|   | 9 |   | 1 |   | 4 | 5 | 8 |   |
|---|---|---|---|---|---|---|---|---|
|   | 2 |   | 3 |   |   |   | 9 | 7 |
| 3 | 1 |   |   | 9 |   |   |   |   |
| 9 |   | 3 |   |   |   |   | 7 |   |
| 8 |   |   |   |   |   |   |   | 4 |
|   | 5 |   |   |   |   | 2 |   | 8 |
|   |   |   |   |   | 8 |   | 2 | 1 |
| 6 | 7 |   |   |   |   | 5 | 4 |   |
|   | 8 | 4 | 9 |   |   | 1 | 5 |   |

**It will be encoded as...**
0,9,0,1,0,4,5,8,0,0,2,0,3,0,........,5,4,0,0,8,4,9,0,0,1,5,0
              
## arguments: 
* argument0: String that will hold the generated sudoku.
This string is passed by reference. So it must be large enough to hold the sudoku-that is atleast 85 characters long.
* argument1: Max tries-The maximum number of tries the computer                   will execute to generate the desired sudoku. If the desired one is very difficult one then specifying a large 'max try' may cause the computer to hang!!
* argument2: With cells-No of visible cells in the genarated sudoku. A solved sudoku has 81 cells. An average level sudoku has 35 cells. Generating a sudoku with less than 25 cells is very difficult and may not be possible even when Max tries = 10000 !! it took 8 minutes to process on my system :<>
* Returns: 1 if process is succesful.
              
## remarks: 
In case the dll is unable to generate the desired sudoku even after               no. of tries = Max tries it simply returns the best sudoku generated.              Take care that the dll file and gm6/gb file are in the same folder. Beware passing invalid arguments may cause problems or in rare cases a general protection fault!!
