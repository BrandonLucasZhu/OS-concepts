## Part1:

1: To run part 1, simply type the command below.
./p1

### Discussion for part1:

After running the test results, the time at which each child process executes and computes the calculation of each row at different orders everytime the program is compiled since it is running concurrently and the time at which determining which process finishes first is abstract. Example of the output is shown below.

Child Process 0: working with row number 0 for Matrix Q
Child Process 3: working with row number 3 for Matrix Q
Child Process 1: working with row number 1 for Matrix Q
Child Process 2: working with row number 2 for Matrix Q
Matrix M:
| 1 2 3 4 |
| 5 6 7 8 |
| 4 3 2 1 |
| 8 7 6 5 |
Matrix N:
| 1 3 5 7 |
| 2 4 6 8 |
| 7 3 5 7 |
| 8 6 4 2 |
Matrix Q
| 58 44 48 52 |
| 130 108 128 148 |
| 32 36 52 68 |
| 104 100 132 164 |

1. To run part 2, run the following command below

   ./p2

This part will prompt the user to select the order of the matrix to be multiplied. Select either 2, 4 or 6. The number of processes he/she would like to run will then be prompted. The user must input the value 1,2, or 4 in order to run the program properly.

Discussion for part2:

In this portion of the lab, an addition of noting the time at which each process is completed. In addition, the program will state whether it is running 1, 2 or 4 processes. The example outputs are shown below.

For one process output:

Child Process One: working with row number 0 for Matrix Q
Child Process One: working with row number 1 for Matrix Q
Child Process One: working with row number 2 for Matrix Q
Child Process One: working with row number 3 for Matrix Q

Elapsed Time: 475 micro sec
Matrix M
| 1 2 3 4 |
| 5 6 7 8 |
| 4 3 2 1 |
| 8 7 6 5 |
Matrix: N
| 1 3 5 7 |
| 2 4 6 8 |
| 7 3 5 7 |
| 8 6 4 2 |
Matrix Q
| 58 44 48 52 |
| 130 108 128 148 |
| 32 36 52 68 |
| 104 100 132 164 |

For two processes output example:

Child Process One: working with row number 0 for Matrix Q
Child Process One: working with row number 1 for Matrix Q

Child Process One elapsed Time: 270 micro sec
Child Process Two: working with row number 2 for Matrix Q
Child Process Two: working with row number 3 for Matrix Q

Child Process Two elapsed Time: 160 micro sec
Matrix M
| 1 2 3 4 |
| 5 6 7 8 |
| 4 3 2 1 |
| 8 7 6 5 |
Matrix: N
| 1 3 5 7 |
| 2 4 6 8 |
| 7 3 5 7 |
| 8 6 4 2 |
Matrix Q
| 58 44 48 52 |
| 130 108 128 148 |
| 32 36 52 68 |
| 104 100 132 164 |

For all four processes example:

Child Process 0: working with row number 0 for Matrix Q
Child Process 0 elapsed Time: 201 micro sec
Child Process 3: working with row number 3 for Matrix Q
Child Process 3 elapsed Time: 143 micro sec
Child Process 2: working with row number 2 for Matrix Q
Child Process 2 elapsed Time: 259 micro sec
Child Process 1: working with row number 1 for Matrix Q
Child Process 1 elapsed Time: 517 micro sec
Matrix M
| 1 2 3 4 |
| 5 6 7 8 |
| 4 3 2 1 |
| 8 7 6 5 |
Matrix: N
| 1 3 5 7 |
| 2 4 6 8 |
| 7 3 5 7 |
| 8 6 4 2 |
Matrix Q
| 58 44 48 52 |
| 130 108 128 148 |
| 32 36 52 68 |
| 104 100 132 164 |

Additional Test Cases: 6x6 Matrix Multiplication 2 processes.
Child Process One: working with row number 0 for Matrix Q
Child Process One: working with row number 1 for Matrix Q

Child Process One elapsed Time: 406 micro sec
Child Process Two: working with row number 2 for Matrix Q
Child Process Two: working with row number 3 for Matrix Q
Child Process Two: working with row number 4 for Matrix Q
Child Process Two: working with row number 5 for Matrix Q

Child Process Two elapsed Time: 327 micro sec
Matrix M
| 1 2 3 4 5 6 |
| 5 6 5 6 7 8 |
| 7 8 5 6 4 3 |
| 4 3 2 1 5 6 |
| 5 6 8 7 6 5 |
| 6 5 5 6 8 7 |
Matrix: N
| 1 3 5 7 1 8 |
| 1 8 2 4 6 8 |
| 6 8 9 3 7 3 |
| 7 3 5 7 1 3 |
| 1 3 8 6 4 2 |
| 4 2 2 3 3 2 |
Matrix Q
| 78 82 108 100 76 67 |
| 122 158 184 182 134 151 |
| 103 161 164 171 121 167 |
| 55 82 101 101 75 87 |
| 134 176 202 183 143 155 |
| 119 154 193 188 130 151 |

Because of how the 4 processes were coded, a 2x2 matrix and a 6x6 matrix will be
will have the equivalent amount of processes that a 2x2 and 6x6 matrix have in rows.
As such, 6 processes will be used for a 6x6 matrix and 2 processes will be used for a
2x2 matrix.
