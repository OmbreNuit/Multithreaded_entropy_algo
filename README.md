# Multithreaded Incremental Entropy Algorithm
This is a multithreaded version of the incremental entropy algorithm proposed by Dr. Rincon in the paper "Entropy-based scheduling performance in real-time multiprocessor systerms"

Article found at <https://ieeexplore.ieee.org/abstract/document/10089704>

This program takes a input string representing the tasks executed in a processor, and calculates the entropy of each scheduling instant using the incremental entropy algorithm proposed by Dr. Rincon.

For example, given the following string:
```
A 2 B 4 C 3 A 7 
```

When A, B, and C represent the tasks executed in the processor, and 2, 4, 3, and 7, represent the time instants that the tasks are excuted in the processor. The task identifier is represented by one character, and the task execution time is represented by a positive integer value.

Given the previous example, the scheduling time instants are 0, 2, 6, and 9, and the entropy for scheduling time instants are:
```
H(0) = 0, H(2) = 0.918295, H(6) = 1.530493, and H(9) = 1.419736
```
The output of the program:
```
CPU 1
Task scheduling information: A(2), B(4), C(3), A(7)
Entropy for CPU 1
0.00 0.92 1.53 1.42

CPU 2
Task scheduling information: B(3), A(3), C(3), A(1), B(1), C(1)
Entropy for CPU 2
0.00 1.00 1.58 1.57 1.57 1.58
```
Uses multiple POSIX threads to implement the solution
