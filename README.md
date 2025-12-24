# IPC Shared Memory Client

A high-performance C++ console application utilizing **Boost.Interprocess** for low-latency communication using only a in-memory region for communication
---

## 🛠 Features
* **Boost.Interprocess:** Uses `managed_shared_memory` for fast data exchange.
* **Dual-Thread Architecture:** * **Producer:** Manages user input via condition variable.
    * **Consumer:** Processes memory updates via Condition Variables.
* **Resource Management:** Automated cleanup of shared memory segments on exit.
**WARNING: Currently the Control-C interrupt causing the program to crush! Only by typing "exit" or "quit" is working as expected
---


### Thread Logic:
| Thread | Responsibility | Exit Strategy |
| :--- | :--- | :--- |
| **Main** | Reads `stdin`, initialize the name of user and spawns the 2 other threads | 
| **Producer** | Reads `stdin` and writes to SHM | 
| **Consumer** | Reads from SHM | Uses `wait()` and waits for condition to wake up|

---

## 🚀 Getting Started

### Prerequisites
* **Boost Libraries:** `sudo apt-get install libboost-all-dev`
* **Compiler:** C++17 or higher
* **Build Tool:** makefile 

### Build Instructions
Go to the root folder and execute the makefile with :
```bash
make .
```
