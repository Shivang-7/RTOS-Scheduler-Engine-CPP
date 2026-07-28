# Deterministic Real-Time OS (RTOS) Scheduler Engine in C++

A lightweight, deterministic Real-Time Operating System (RTOS) simulation engine built from scratch in modern C++17. Designed to mathematically evaluate, execute, and analyze mission-critical CPU scheduling workloads (e.g., radar tracking, missile guidance, flight telemetry) under strict real-time constraints.

---

## Key Features

- **Schedulability Validation (Liu-Layland Test)**: Automatically computes CPU utilization bounds ($U = \sum \frac{C_i}{T_i} \le n(2^{1/n} - 1)$) to verify whether a task set is mathematically guaranteed to meet deadlines before execution.
- **Static Priority Scheduling (RMS)**: Implements Rate Monotonic Scheduling where shorter task periods receive higher static priorities.
- **Dynamic Priority Scheduling (EDF)**: Implements Earliest Deadline First where priorities are evaluated dynamically based on absolute deadline proximity.
- **Priority Inheritance Protocol (PIP)**: Prevents Priority Inversion and task starvation by dynamically boosting the priority of resource-holding low-priority tasks.
- **Hardware Overhead Simulation**: Tracks context-switching overhead and register save/restore state penalties.

---

## Project Structure

```plaintext
RTOS_Scheduler/
├── include/
│   ├── Task.h          # Task Control Block (TCB) definitions
│   ├── Validator.h     # Liu-Layland Schedulability Bound engine
│   ├── RMSScheduler.h  # Rate Monotonic Scheduler engine
│   ├── EDFScheduler.h  # Earliest Deadline First Scheduler engine
│   ├── Resource.h      # Mutex & Shared Resource abstraction
│   └── PIPScheduler.h  # Priority Inheritance Protocol Scheduler
├── src/
│   └── main.cpp        # Driver script executing tactical simulations
├── CMakeLists.txt      # Cross-platform build configuration
└── README.md