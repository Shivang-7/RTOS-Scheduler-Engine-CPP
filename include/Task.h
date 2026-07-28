#ifndef TASK_H
#define TASK_H

#include <string>
using namespace std;

enum class TaskState {
    READY,
    RUNNING,
    BLOCKED,
    COMPLETED
};

struct Task {
    string id;          // e.g., "Radar_Scan", "Missile_Guidance"
    int burstTime;           // Computation time required per period (C)
    int period;              // Time interval after which task restarts (T)
    int absoluteDeadline;    // Deadline by which task must finish (D)
    
    // Dynamic execution tracking variables
    int remainingBurst;      // Computation remaining in current period
    int nextReleaseTime;     // Next tick when task becomes READY again
    TaskState state;         // Operational state
    int totalMissedDeadlines;// Diagnostic counter
};

#endif // TASK_H