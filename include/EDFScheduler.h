#ifndef EDF_SCHEDULER_H
#define EDF_SCHEDULER_H

#include "Task.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

class EDFScheduler {
private:
    vector<Task> tasks;
    int simulationTime;

public:
    EDFScheduler(const vector<Task>& initialTasks, int totalDuration)
        : tasks(initialTasks), simulationTime(totalDuration) {}

    void runSimulation() {
        cout << "\n========================================================" << endl;
        cout << "      STARTING EARLIEST DEADLINE FIRST (EDF) SIMULATION   " << endl;
        cout << "========================================================" << endl;

        cout << left << setw(10) << "Time (t)"
                  << setw(22) << "Running Task"
                  << setw(20) << "Current Deadline" << endl;
        cout << "--------------------------------------------------------" << endl;

        for (int t = 0; t < simulationTime; ++t) {
            // 1. Check for task arrivals / new instances at tick 't'
            for (auto& task : tasks) {
                if (t % task.period == 0) {
                    // Check if previous instance missed deadline
                    if (task.remainingBurst > 0 && t > 0) {
                        task.totalMissedDeadlines++;
                        cout << "  [!] DEADLINE MISS: Task " << task.id 
                                  << " failed at t=" << t << endl;
                    }
                    // Reset burst and update dynamic absolute deadline for current instance
                    task.remainingBurst = task.burstTime;
                    task.nextReleaseTime = t;
                    task.absoluteDeadline = t + task.period; // Dynamic Deadline = Release Time + Period
                    task.state = TaskState::READY;
                }
            }

            // 2. Select the READY task with the EARLIEST absolute deadline
            Task* highestPriorityTask = nullptr;
            int earliestDeadline = INT_MAX;

            for (auto& task : tasks) {
                if (task.remainingBurst > 0) {
                    if (task.absoluteDeadline < earliestDeadline) {
                        earliestDeadline = task.absoluteDeadline;
                        highestPriorityTask = &task;
                    }
                }
            }

            // 3. Execute the selected task
            if (highestPriorityTask != nullptr) {
                highestPriorityTask->state = TaskState::RUNNING;
                cout << left << setw(10) << (to_string(t) + " - " + to_string(t+1))
                          << setw(22) << highestPriorityTask->id
                          << "Deadline t=" << highestPriorityTask->absoluteDeadline 
                          << " (Left: " << (highestPriorityTask->remainingBurst - 1) << ")" << endl;

                highestPriorityTask->remainingBurst--;
                if (highestPriorityTask->remainingBurst == 0) {
                    highestPriorityTask->state = TaskState::COMPLETED;
                }
            } else {
                cout << left << setw(10) << (to_string(t) + " - " + to_string(t+1))
                          << setw(22) << "[ CPU IDLE ]"
                          << "---" << endl;
            }
        }

        // Summary
        cout << "\n========================================================" << endl;
        cout << "            EDF SIMULATION DIAGNOSTIC SUMMARY           " << endl;
        cout << "========================================================" << endl;
        for (const auto& task : tasks) {
            cout << "Task: " << left << setw(20) << task.id
                      << " | Period: " << setw(4) << task.period
                      << " | Missed Deadlines: " << task.totalMissedDeadlines << endl;
        }
    }
};

#endif // EDF_SCHEDULER_H