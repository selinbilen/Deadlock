# Deadlock
You will develop a simulation for a deadlock detection algorithm (implement the version given in the seventh course slides on page 3 and 4). It will evaluate the given state of the processes and resources to decide whether a deadlock exists or not. It will also print the free vector, the number of the deadlocked processes and their process ids. Lastly if a deadlock exists, the program will determine which processes to be terminated in order to resolve the deadlock situation. You will implement the “terminate deadlocked processes one by one until deadlock no longer exists” strategy with “select the one which has the least number of allocated resources” policy.

Input
Program input is a text file that represents "Request" and "Allocation" matrixes as well as the resource vector. First line of the file contains integer value for the total number of processes(n) and the total number of resources(m), respectively. After that, next n line is “Request” matrix where each line represents the requests of a single process for resources. Similarly, next n line is "Allocation" matrix. After that, the last line in the document is the resource vector. Resource values are separated by tab(\t) characters.


Output
The expected program outputs for the sample inputs are given below.

>./program input1.txt

DEADLOCK DETECTED!
Free vector: 1, 1, 0, 1, 1
2 processes in deadlock: 0, 1

Resolving deadlock:
Process 1 is terminated.
Terminating process 1.
Free vector after the termination: 2, 2, 1, 1, 1
Deadlock resolved.

>./program input2.txt

NO DEADLOCK!

>./program input3.txt

DEADLOCK DETECTED!
Free vector: 3, 3, 3, 0, 3, 1, 3
4 processes in deadlock: 2, 3, 4, 5

Resolving deadlock:
Process 3 is terminated.
Free vector after the termination: 3, 3, 3, 0, 4, 1, 3
Process 5 is terminated.
Free vector after the termination:
