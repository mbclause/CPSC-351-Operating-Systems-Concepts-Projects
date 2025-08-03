# CPSC 351 Final Project - Multi-Threaded implimentation of "Fizzbuzz"

Programmers: Michael Clausen, Anh Nguyen, Linh Nguyen

This is a program that impliments the popular "Fizz buzz" game using POSIX threads written in C++. It runs in the Linux environment.

The wrapper code in "p_thread_wrappers.cpp/h" was written by our professor, Kennyt Avery. The majority of the rest of the code is mine.


fizzbuzz.cpp: [Usage: fizzbuzz [NUM]]:<br>
This program takes a number as a command-line parameter to count up from 1: print out "fizz" if the number is divisible by 3, "buzz" if the number is divisible by 5,"fizzbuzz" if the number is divisible by 3 and 5, otherwise print out the number itself.

producer-consumer.cpp [Usage: producer-consumer [NUM] [BUFSIZE]]: <br>
This file impliments the producer/consumer relationship. The producer thread counts up to the specified number, placing each integer in the bounded buffer. The consumer thread removes the integer from the buffer and print the corresponding output value.

multiple_producers.cpp [Usage: multiple_producers [NUM] [BUFSIZE]]:<br> 
In this file, there are two producers, one producing odd values, the other producing even values, and place them in the bounded buffer. The consumer thread removes the integer from the buffer and print the corresponding output value.

multiple_consumers.cpp [Usage: multiple_consumers [NUM] [BUFSIZE]]:<br>
Besides the two producers, this program has four consumer threads, one to print out Fizz, one to print out Buzz, one to print out FizzBuzz and one to print out other integers.

ordering_output.cpp [Usage: ordering_output [NUM] [BUFSIZE]]: <br>
This program also has two producers and four consumers, but they will be scheduled to print the outputs in ascending order. 





