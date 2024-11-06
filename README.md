# NachOS Project

## Overview
This project involves the development and extension of **NachOS**,an instructional operating system framework designed for operating systems courses. Using **NachOS**, we implement key components necessary for operating system functionality, including **thread management**, **CPU scheduling**, and **file systems**.

The main objective is to gain hands-on experience with OS concepts by building foundational operating system components, which can be extended to real-world scenarios.

## Project Components
The project is divided into several key modules, each focusing on a specific aspect of operating system functionality:

1. **Thread Management and Synchronization**
   - Implement the basic thread API, enabling creation, termination, and synchronization of threads.
   - Implement synchronization primitives such as **semaphores**, **locks**, and **condition variables**.

2. **CPU Scheduling**
   - Develop and implement various CPU scheduling algorithms, such as **First-Come, First-Served (FCFS)**, **Round Robin**, and **Priority Scheduling**.
   - Support preemptive and non-preemptive scheduling policies.
   - Integrate the scheduling policies to handle multi-threaded tasks efficiently.

3. **Process Management**
   - Extend Nachos to handle process creation, termination, and inter-process communication.
   - Implement features for memory management and virtual memory, such as paging and swapping.
   
4. **File System**
   - Build a file system capable of managing files and directories.
   - Implement file operations such as creation, deletion, reading, and writing.
   - Support disk scheduling for improved access speed.

## Key Features
- **Multi-threaded Execution:** Supports creating and managing multiple threads, enabling concurrency.
- **Customizable CPU Scheduler:** Various scheduling algorithms can be selected and tested to understand their impact on system performance.
- **Memory Management:** Simulate page tables and implement basic virtual memory techniques.
- **File System Operations:** Basic file management, including support for directories and persistent storage.

## System Requirements
- **C++ Compiler**: Nachos is primarily written in **C++**. Ensure you have a compatible compiler (e.g., **g++**).
- **Unix-based OS**: Nachos runs best on Unix-like operating systems such as Linux and macOS.
- **Java Runtime**: Certain versions of Nachos require Java for some testing utilities.
