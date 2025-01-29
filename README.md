# Compare Performance: std::vector vs. std::list

This repository contains a C++ program that compares the performance of two Standard Library containers:
- `std::vector<int>`
- `std::list<int>`

for the following operations:

1. **Sorted insertion** of N randomly generated integers.
2. **Random removal** of N elements by index.

The results are reported in microseconds.

---

## Table of Contents

- [Overview](#overview)
- [Why Does `std::vector` Often Outperform `std::list`?](#why-does-stdvector-often-outperform-stdlist)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [Example Output](#example-output)
- [License](#license)

---

## Overview

The core idea is to insert N elements in sorted order, then remove them one by one at random positions. Specifically:

- **Insertion (Vector)**: We generate a random integer, use `std::lower_bound` to find the position in O(log N), then call `insert`, which can be O(N) due to shifting elements.
- **Removal (Vector)**: We pick a random index in O(1) and erase, which again can be O(N) due to shifting.

- **Insertion (List)**: We search for the insertion position by walking the list in O(N), then insert in O(1) once found.
- **Removal (List)**: We pick a random index in O(1), but advancing the iterator to that index is O(N); erasing a node in a linked list is O(1).

The code times how long each container takes for both operations combined.

---

## Why Does `std::vector` Often Outperform `std::list`?

On paper, one might expect:
- `std::vector` insertions to be **O(N)** (element shifting),
- `std::list` insertions to be **O(1)** (just re-link pointers).

However, in practice, **`std::vector` can be faster**, sometimes significantly, because of:

1. **Contiguous Memory / Cache Locality**  
   `std::vector` stores its elements contiguously. Modern CPUs fetch data in cache lines (64 bytes on many systems). Sequential access and predictable memory patterns allow CPUs to preload data efficiently, minimizing cache misses.

2. **Branch Prediction & Prefetching**  
   With contiguous memory, CPU hardware prefetchers can anticipate future memory accesses. Linked lists, by contrast, require pointer chasing to traverse nodes scattered around the heap, causing more cache misses.

3. **Fewer Allocations**  
   A `std::list` allocates a new node for each element, introducing additional overhead for memory allocation and deallocation. A `std::vector` does fewer (though larger) allocations overall.

4. **Iterators**  
   Random access in `std::vector` is O(1) by index, while random access in `std::list` is O(N). Even though removing an element from a list is O(1) once the iterator is known, finding the right position (especially for a “random index”) can dominate overall time.

**Summary**: In real workloads, the CPU-level optimizations for contiguous data often outweigh the theoretical advantages of a linked list’s constant-time inserts/erases at known positions.

---

## Build Instructions

1. **Clone** this repository:
   ```bash
   git clone https://github.com/zgrigoryan/vector-vs-list-performance.git
   cd CompareVectorList
