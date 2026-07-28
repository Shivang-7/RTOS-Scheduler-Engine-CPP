# Deterministic Real-Time OS (RTOS) Scheduler Engine in C++

A lightweight, deterministic Real-Time Operating System (RTOS) simulation engine built from scratch in modern C++17. Designed to mathematically evaluate, execute, and analyze mission-critical CPU scheduling workloads (e.g., radar tracking, missile guidance, flight telemetry) under strict real-time constraints.

---

## Key Features

- **Schedulability Validation (Liu-Layland Test)**: Automatically computes CPU utilization bounds to verify whether a task set is mathematically guaranteed to meet deadlines before execution.
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
```

## Getting Started

**Build and Run**
```
g++ -std=c++17 -Iinclude src/main.cpp -o RTOS_Engine
./RTOS_Engine
```

## Terminal Simulation Output

When running the simulation driver (main.cpp), the engine executes pre-execution verification followed by cycle-by-cycle scheduler execution and resource synchronization tests:
```
DETERMINISTIC RTOS SCHEDULER EVALUATION ENGINE (C++)

=== Schedulability Report ===
Total CPU Utilization (U): 0.708333
RMS Bound (n=3): 0.779763
Status: PASSED (Guaranteed Schedulable)

========================================================
          STARTING RATE MONOTONIC SIMULATION

Time (t)    Running Task          Status / Preemption
0 - 1       Radar_Scan            Burst Left: 0
1 - 2       Missile_Guidance      Burst Left: 1
2 - 3       Missile_Guidance      Burst Left: 0
3 - 4       Telemetry_Comm        Burst Left: 0
4 - 5       Radar_Scan            Burst Left: 0
5 - 6       [ CPU IDLE ]          ---
...

========================================================
    RESOURCE SYNCHRONIZATION TEST: WITH PIP (INHERITANCE ACTIVE)

Time (t)    Running Task          Effective Priority    Lock & System Log
...
5 - 6       Low_Priority_Log      12                    [LOCK CONFLICT] High_Priority_Radar BLOCKED!
                                                        [PIP ACTIVE] Low_Priority_Log INHERITS priority (Period: 6)
6 - 7       Low_Priority_Log      6                     Executing under boosted priority...
7 - 8       Low_Priority_Log      6                     [UNLOCK] Low_Priority_Log released Shared_Sensor_Bus
                                                        [UNBLOCKED] High_Priority_Radar is now READY!
8 - 9       High_Priority_Radar   6                     Executing...
```

## Architectural Value & System Design Rationale

**Why design a C++ simulation engine instead of using FreeRTOS or RT-Linux?**
While production kernels like FreeRTOS or VxWorks execute pre-compiled code on real hardware, they lack pre-execution mathematical safety verification and multi-algorithm comparative analysis tools.
This engine serves as a system architecture verification tool for mission-critical engineering:
- **Mathematical Safety First**: Verifies whether a workload satisfies Liu-Layland utilization limits prior to physical deployment on satellites or actuators.

- **Side-by-Side Algorithm Evaluation**: Enables comparative evaluation between static (RMS) and dynamic (EDF) scheduling strategies to identify exact CPU utilization limits and context-switching overheads.

- **Whitebox Mutex Diagnostics**: Models resource contention and demonstrates how the Priority Inheritance Protocol eliminates the classic Priority Inversion flaw (notably associated with the 1997 Mars Pathfinder incident).

## Theoretical Foundations

- **Liu-Layland Utilization Bound: $$U = \sum_{i=1}^{n} \frac{C_i}{T_i} \le n(2^{1/n} - 1)$$**
  (Where $C_i$ is execution time and $T_i$ is task period)

- **Priority Inversion & Inheritance**: When $T_{\text{high}}$ blocks on a mutex held by $T_{\text{low}}$, PIP temporarily elevates $T_{\text{low}}$'s priority to $\max(\text{Priority}(T_{\text{high}}))$, preventing intermediate tasks ($T_{\text{medium}}$) from starving $T_{\text{low}}$ and stalling $T_{\text{high}}$.
