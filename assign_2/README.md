# Compile and run:

- Enter the directory of where the files are. Simply type the two commands separately to run and compile the program.

```
  gcc STATS.c -o stats

  ./stats
```

After running these commands. A prompt will be displayed showing whether the user wants to run in debug mode or not. This is shown below.

    Do you want to enter debug mode y/n ?

The user is required to enter just the character 'y' or 'n' to determine whether they want to run the program in debug mode or not.

Enter five unique values individually (press enter after entering each value). Give it a couple seconds after entering parameter 5 and the resultant organized array will be produced.

This is an example input of 5 parameters and the output results.

    Example output with debug mode below:

    Do you want to enter debug mode y/n ?
    y
    Please input 5 distinct values
    Parameter 1:434
    Parameter 2:576
    Parameter 3:76
    Parameter 4:1
    Parameter 5:3
    Process 1: 434 swapped with 576
    Process 3: 76 did not swap with 1
    Process 4: 1 swapped with 3
    Process 2: 434 did not swap with 76
    Process 3: 76 did not swap with 3
    Process 1: 576 did not swap with 434
    [576][434][76][3][1]


    Example output without debug mode below:

    Do you want to enter debug mode y/n ?
    n
    Please input 5 distinct values
    Parameter 1:43
    Parameter 2:65
    Parameter 3:314
    Parameter 4:657
    Parameter 5:8
    [657][314][65][43][8]

## Description of the algorithm:

As stated in the guideline, each process handles two index's to perform a swap method. Each process will have have a continous loop until the entire array within shared memory is organized in decreasing order i.e. 8,6,4,3,2. Within each process case, each semaphore will have a "wait" and "signal" portion of the code to allow each process to enter the critical section to determine whether a swap is required. In this case like in lab 4, the signal in lab 4 is to prevent race conditions from happening. The swap method is a function that swaps values of the target array indexes of each process. The concept of using bubble sort was used which is to continuosly swap until the desired order is reached. In this design, a three semaphores was used to create the process synchronization using waits which are "sleeps". Process 1 and 4 manipulate the first and last 2 index's array. Thus, they can run concurrently. Process 2 and process 3 share the same index's for swap manipulation so they cannot run concurrently. The purpose of this algorithm is to implement three semaphores to allow certain processes run concurrently without having the same array index that they control.
