# Multithreaded Incremental Entropy Algorithm
This is a multithreaded version of the incremental entropy algorithm proposed by Dr. Rincon in the paper "Entropy-based scheduling performance in real-time multiprocessor systerms"

Article found at <https://ieeexplore.ieee.org/abstract/document/10089704>

This program takes a input string representing the tasks executed in a processor, and calculates the entropy of each scheduling instant using the incremental entropy algorithm proposed by Dr. Rincon.

For example, given the following string:
''' A 2 B 4 C 3 A 7 '''

When A, B, and C represent the tasks 
