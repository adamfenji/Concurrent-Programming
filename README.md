## 📘 **Project Overview**  
This repository documents my work from **CS3331: Concurrent Computing**, a systems-level computer science course I completed at **Michigan Technological University** in **Spring 2025**. The class was taught by **Professor Jean Mayo** and focused on developing robust **multithreaded** and **multiprocess** software systems in **C and C++** using **UNIX primitives** and the **ThreadMentor** library.

## 📂 **Projects & Concepts Implemented**

| #  | **Topic**                      | **Description** |
|----|-------------------------------|-----------------|
| 1  | [**Process Forking & Concurrency**](1.%20Process%20Forking%20%26%20Concurrency) | Created a **C program** that launches **3 concurrent processes** to calculate a **Fibonacci number**, estimate **ellipse area** using **Monte Carlo methods**, and simulate a **pinball game** using `fork()`, `wait()`, and `exit()` |
| 2  |[**Concurrent Merge Sort**](2.%20Concurrent%20Merge%20Sort)     | Built a **concurrent merge sort** using **UNIX shared memory** and `execvp()`. Implemented a **binary merge** strategy where each element is positioned using a **modified binary search** |
| 3  |[**Even-Odd Sorting with Threads**](3.%20Even-Odd%20Sorting%20with%20Threads) | Implemented **concurrent even-odd sort** using the **ThreadMentor library** in **C++**. Created and destroyed `n/2` threads per iteration to perform **compare-and-swap** operations |
| 4  | [**Semaphores: Party Room Synchronization**](4.%20Semaphores%20-%20Party%20Sync) | Simulated **landlord-student interactions** using **ThreadMentor monitors** and **semaphores**. Modeled a **social room** with **bounded capacity** and conditions under which **parties are broken up** |
| 5  | [**Monitors: Santa Claus Concurrency Problem**](5.%20Monitors%20-%20Santa%20Claus) | Designed a **Hoare-style monitor** to synchronize **Santa, elves, and reindeer threads**. Prioritized **toy deliveries** over answering **elf questions**, and implemented **Santa’s retirement** after `t` deliveries |

## 🧠 **Key Learnings**
- Gained deep understanding of **concurrency challenges** such as **deadlocks**, **race conditions**, and **starvation**  
- Practiced using **low-level system calls** (`fork`, `wait`, `execvp`) and **shared memory** (`shmget`, `shmat`) in **C**  
- Designed **synchronization strategies** using **monitors**, **mutexes**, and **condition variables**  
- Developed correct and verifiable **concurrent algorithms** using **ThreadMentor in C++**  
- Implemented **classic synchronization problems** (e.g., **Santa Claus**, **bounded buffer**, **even-odd sort**) in a **multi-threaded** setting  
- Learned to use **structured logging** and **indentation strategies** to trace **interleaved output** from concurrent threads  
