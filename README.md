# sync-locks-c

A C-based project implementing synchronization primitives and thread coordination mechanisms at the user level.

## Overview

This project includes 6 standalone modules, each demonstrating a different low-level synchronization concept:

- `tas_semaphore` – Binary semaphore using Test-And-Set spinlock  
- `ticketlock_semaphore` – Semaphore implemented with the Ticket Lock algorithm  
- `condition_variable` – Implementation of wait/signal/broadcast condition variable  
- `read_write_lock` – Fair reader-writer lock allowing concurrent reads or exclusive writes  
- `thread_local_storage` – TLS implementation using a static array and thread ID mapping  
- `producer_consumer` – Multi-threaded producer-consumer pipeline with a shared queue and condition variable  

Each module is self-contained and includes a `main.c` file for testing and demonstration.

## Technologies

- **Language:** C (C23 standard)  
- **Platform:** Ubuntu 24.04 via Dev Container (Docker)  
- **Build tools:** GCC 13, Make (optional)  
- **Constraints:** No use of `pthread`, `malloc`, or system libraries unless explicitly allowed

## Structure

Each directory contains its own `.c`, `.h`, and `main.c` files.

## Notes

This project demonstrates practical implementations of synchronization primitives, designed to deepen understanding of multithreading, atomic operations, and mutual exclusion in C.  
Each module was built from scratch to simulate low-level thread coordination without relying on system libraries.


