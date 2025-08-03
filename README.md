# 🖥️ Operating Systems Projects – CPSC XXX

This repository contains four programming projects completed for the Operating Systems course at California State University, Fullerton. The projects explore foundational OS concepts including process creation, memory allocation, and thread synchronization using POSIX threads and custom memory management strategies.

## 🧰 Technologies Used

- C++
- POSIX Threads (pthreads)
- Custom Allocators
- GNU Make
- Linux System Calls

---

## 📁 Project Overviews

### 🔧 Project 1 – Process Creation and Output Streams

Implements small utility programs (`foreach.cpp`, `stderr.cpp`, `thrice.cpp`) to explore process creation, standard output/error management, and stream redirection in Unix-like systems.

📄 `Project 1/README.md`

---

### 🧠 Project 2 – Custom Memory Allocator

Implements a basic memory allocator using a free list (`allocator.cpp`, `allocator.h`, `node.h`) and tests allocation behavior with various data sizes. The allocator reuses previously freed heap blocks but does **not fully coalesce adjacent free blocks**, which may lead to fragmentation. While not production-ready, it illustrates key ideas behind `malloc`/`free` systems and exposes practical tradeoffs in heap memory design.

📄 `Project 2/README.txt`

---

### 📦 Project 3 – File Block Rebuilder

Includes a file block manipulation utility (`rebuild.cpp`, `box.cpp`) designed to reconstruct data from fragmented or partial binary files. Focuses on low-level file I/O operations and memory layout.

📄 `Project 3/README.txt`

---

### 🔄 Project 4 – Thread Synchronization and Producer-Consumer Problems

Implements various multithreading challenges including:

- `producer-consumer.cpp` – Classic bounded buffer problem using semaphores.
- `multiple_producers.cpp` & `multiple_consumers.cpp` – Handling concurrent access across multiple threads.
- `ordering_output.cpp` – Ensuring deterministic print order in multithreaded scenarios.
- `pthread-wrappers.cpp/h` – Utility wrappers around pthreads for cleaner syntax.
- `fizzbuzz.cpp` – Multithreaded variant of the FizzBuzz problem.

📄 `Project 4/README.md`

---

## ⚠️ Note

All code was developed and tested in a Linux environment with `g++` and `make`. Intended for educational purposes only.

---

## 📚 License

MIT License — for academic and personal use.
