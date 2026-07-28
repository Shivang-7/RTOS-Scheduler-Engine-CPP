#ifndef PIP_SCHEDULER_H
#define PIP_SCHEDULER_H

#include "Task.h"
#include "Resource.h"
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

class PIPScheduler {
private:
    vector<Task> tasks;
    Resource sharedBus;
    int simulationTime;
    int contextSwitchOverhead;

public:
    PIPScheduler(const vector<Task>& initialTasks, int duration, int csPenalty = 1)
        : tasks(initialTasks), sharedBus("Shared_Sensor_Bus"), simulationTime(duration), contextSwitchOverhead(csPenalty) {}

    void runSimulationWithPIP(bool enablePIP) {
        cout << "\n========================================================" << endl;
        cout << "   RESOURCE SYNCHRONIZATION TEST: " 
                  << (enablePIP ? "WITH PIP (INHERITANCE ACTIVE)" : "WITHOUT PIP (PRIORITY INVERSION TRAP)") << endl;
        cout << "========================================================" << endl;

        cout << left << setw(10) << "Time (t)"
                  << setw(22) << "Running Task"
                  << setw(18) << "Effective Priority"
                  << setw(25) << "Lock & System Log" << endl;
        cout << "--------------------------------------------------------" << endl;

        // Reset state for clean simulation run
        for (auto& task : tasks) {
            task.remainingBurst = 0;
            task.state = TaskState::COMPLETED;
        }
        sharedBus.isLocked = false;
        sharedBus.lockedByTaskId = "";

        string lastRunningTaskId = "";
        int totalContextSwitches = 0;

        for (int t = 0; t < simulationTime; ++t) {
            // 1. Task releases at specific arrival times for this scenario
            // Low_Priority_Log arrives at t=0
            // Med_Priority_Worker arrives at t=2
            // High_Priority_Radar arrives at t=2
            for (auto& task : tasks) {
                if (t % task.period == 0) {
                    task.remainingBurst = task.burstTime;
                    task.state = TaskState::READY;
                }
            }

            // 2. Identify task holding the lock
            Task* lockHolder = nullptr;
            if (sharedBus.isLocked) {
                for (auto& task : tasks) {
                    if (task.id == sharedBus.lockedByTaskId) {
                        lockHolder = &task;
                        break;
                    }
                }
            }

            // 3. Find highest priority waiting/ready task
            // Priority order: High_Priority_Radar (Period 4) > Med_Priority_Worker (Period 8) > Low_Priority_Log (Period 12)
            Task* highestReadyTask = nullptr;
            for (auto& task : tasks) {
                if (task.remainingBurst > 0 && task.state != TaskState::BLOCKED) {
                    if (highestReadyTask == nullptr || task.period < highestReadyTask->period) {
                        highestReadyTask = &task;
                    }
                }
            }

            // 4. Check if High_Priority_Radar wants the lock held by Low_Priority_Log
            Task* highTask = nullptr;
            for (auto& task : tasks) {
                if (task.id == "High_Priority_Radar") highTask = &task;
            }

            if (highTask != nullptr && highTask->remainingBurst > 0 && sharedBus.isLocked && sharedBus.lockedByTaskId != highTask->id) {
                highTask->state = TaskState::BLOCKED;
                cout << "  [LOCK CONFLICT] " << highTask->id << " BLOCKED waiting for " << sharedBus.id << "!" << endl;
            }

            // 5. Apply Priority Inheritance Protocol (PIP)
            Task* taskToRun = nullptr;

            if (enablePIP && lockHolder != nullptr && lockHolder->remainingBurst > 0 && highTask != nullptr && highTask->state == TaskState::BLOCKED) {
                // Lock holder inherits High_Priority_Radar's priority!
                taskToRun = lockHolder;
                cout << "  [PIP ACTIVE] " << lockHolder->id << " INHERITS priority of " << highTask->id 
                          << " (Boosted Priority Period: " << highTask->period << ")" << endl;
            } else {
                // Pick highest priority READY task
                taskToRun = highestReadyTask;
            }

            // 6. Execute selected task
            if (taskToRun != nullptr) {
                if (lastRunningTaskId != "" && lastRunningTaskId != taskToRun->id) {
                    totalContextSwitches++;
                }
                lastRunningTaskId = taskToRun->id;

                taskToRun->state = TaskState::RUNNING;

                // Acquire lock if Low_Priority_Log starts executing
                if (taskToRun->id == "Low_Priority_Log" && !sharedBus.isLocked && taskToRun->remainingBurst == taskToRun->burstTime) {
                    sharedBus.isLocked = true;
                    sharedBus.lockedByTaskId = taskToRun->id;
                    cout << "  [LOCK ACQUIRED] " << taskToRun->id << " locked " << sharedBus.id << endl;
                }

                int effectivePriority = (enablePIP && lockHolder == taskToRun && highTask != nullptr && highTask->state == TaskState::BLOCKED) 
                                        ? highTask->period 
                                        : taskToRun->period;

                cout << left << setw(10) << (to_string(t) + " - " + to_string(t+1))
                          << setw(22) << taskToRun->id
                          << setw(18) << effectivePriority
                          << "Burst Left: " << (taskToRun->remainingBurst - 1) << endl;

                taskToRun->remainingBurst--;

                // Unlock resource if lockHolder completes or releases
                if (taskToRun->remainingBurst == 0) {
                    taskToRun->state = TaskState::COMPLETED;
                    if (sharedBus.lockedByTaskId == taskToRun->id) {
                        sharedBus.isLocked = false;
                        sharedBus.lockedByTaskId = "";
                        cout << "  [UNLOCK] " << taskToRun->id << " released " << sharedBus.id << endl;
                        
                        // Unblock waiting high-priority task
                        if (highTask != nullptr && highTask->state == TaskState::BLOCKED) {
                            highTask->state = TaskState::READY;
                            cout << "  [UNBLOCKED] " << highTask->id << " is now READY!" << endl;
                        }
                    }
                }
            } else {
                cout << left << setw(10) << (to_string(t) + " - " + to_string(t+1))
                          << setw(22) << "[ CPU IDLE ]"
                          << setw(18) << "---"
                          << "---" << endl;
            }
        }

        cout << "Total Context Switches Recorded: " << totalContextSwitches << endl;
        cout << "Hardware Context-Switch Penalty Overhead: " 
                  << (totalContextSwitches * contextSwitchOverhead) << " ms" << endl;
    }
};

#endif // PIP_SCHEDULER_H