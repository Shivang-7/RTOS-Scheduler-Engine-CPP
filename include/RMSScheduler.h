#ifndef RMS_SCHEDULER_H
#define RMS_SCHEDULER_H

#include "Task.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;

class RMSScheduler {
private:
    vector<Task> tasks;
    int simulationTime;

public:
    RMSScheduler(const vector<Task>& initialTasks, int totalDuration)
        : tasks(initialTasks), simulationTime(totalDuration) {
        
        // In RMS, priority is static and inversely proportional to period.
        // Sort tasks by period ascending (smallest period = highest priority)
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.period < b.period;
        });
    }

    void runSimulation() {
        cout << "\n========================================================" << endl;
        cout << "          STARTING RATE MONOTONIC SIMULATION           " << endl;
        cout << "========================================================" << endl;

        cout << left << setw(10) << "Time (t)"
                  << setw(22) << "Running Task"
                  << setw(20) << "Status / Preemption" << endl;
        cout << "--------------------------------------------------------" << endl;

        for (int t = 0; t < simulationTime; ++t) {
            // 1. Check for task arrivals / period restarts at tick 't'
            for (auto& task : tasks) {
                if (t % task.period == 0) {
                    // Check if previous instance missed its deadline
                    if (task.remainingBurst > 0 && t > 0) {
                        task.totalMissedDeadlines++;
                        cout << "  [!] DEADLINE MISS: Task " << task.id 
                                  << " failed at t=" << t << endl;
                    }
                    // Reset burst for the new period
                    task.remainingBurst = task.burstTime;
                    task.nextReleaseTime = t;
                    task.state = TaskState::READY;
                }
            }

            // 2. Select the ready task with highest priority (smallest period)
            Task* currentTask = nullptr;
            for (auto& task : tasks) {
                if (task.remainingBurst > 0) {
                    currentTask = &task;
                    break; // Since 'tasks' is sorted by period, first match is highest priority!
                }
            }

            // 3. Execute the selected task for 1 tick
            if (currentTask != nullptr) {
                currentTask->state = TaskState::RUNNING;
                cout << left << setw(10) << (to_string(t) + " - " + to_string(t+1))
                          << setw(22) << currentTask->id
                          << "Burst Left: " << (currentTask->remainingBurst - 1) << endl;

                currentTask->remainingBurst--;
                if (currentTask->remainingBurst == 0) {
                    currentTask->state = TaskState::COMPLETED;
                }
            } else {
                cout << left << setw(10) << (to_string(t) + " - " + to_string(t+1))
                          << setw(22) << "[ CPU IDLE ]"
                          << "---" << endl;
            }
        }

        // Print final diagnostic summary
        cout << "\n========================================================" << endl;
        cout << "               SIMULATION DIAGNOSTIC SUMMARY            " << endl;
        cout << "========================================================" << endl;
        for (const auto& task : tasks) {
            cout << "Task: " << left << setw(20) << task.id
                      << " | Period: " << setw(4) << task.period
                      << " | Missed Deadlines: " << task.totalMissedDeadlines << endl;
        }
    }
};

#endif // RMS_SCHEDULER_H