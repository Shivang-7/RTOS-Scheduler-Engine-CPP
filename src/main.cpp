#include "../include/Task.h"
#include "../include/Validator.h"
#include "../include/RMSScheduler.h"
#include "../include/EDFScheduler.h"
#include "../include/PIPScheduler.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
    cout << "========================================================" << endl;
    cout << "  DETERMINISTIC RTOS SCHEDULER EVALUATION ENGINE (C++)  " << endl;
    cout << "========================================================" << endl;

    // 1. Tactical workload for RMS & EDF
    vector<Task> tacticalWorkload = {
        {"Radar_Scan",        1, 4, 4, 1, 0, TaskState::READY, 0},
        {"Missile_Guidance",  2, 6, 6, 2, 0, TaskState::READY, 0},
        {"Telemetry_Comm",    1, 8, 8, 1, 0, TaskState::READY, 0}
    };

    // Math validation check
    Validator::checkRMSBound(tacticalWorkload);

    // RMS Simulation
    RMSScheduler rmsScheduler(tacticalWorkload, 12);
    rmsScheduler.runSimulation();

    // EDF Simulation
    EDFScheduler edfScheduler(tacticalWorkload, 12);
    edfScheduler.runSimulation();

    // 2. Resource Contention Workload for Priority Inversion & Inheritance Demo
    // Low_Priority_Log locks resource at t=0
    // High_Priority_Radar arrives and needs resource
    // Med_Priority_Worker arrives and threatens to starve Low_Priority_Log (Priority Inversion)
    vector<Task> synchronizationWorkload = {
        {"High_Priority_Radar", 2, 6, 6, 2, 0, TaskState::READY, 0},
        {"Med_Priority_Worker", 3, 8, 8, 3, 0, TaskState::READY, 0},
        {"Low_Priority_Log",    3, 12, 12, 3, 0, TaskState::READY, 0}
    };

    PIPScheduler pipScheduler(synchronizationWorkload, 12, 1);
    
    // Demonstrate Priority Inversion lockup vs PIP solution
    pipScheduler.runSimulationWithPIP(false); // Without PIP (Inversion happens)
    pipScheduler.runSimulationWithPIP(true);  // With PIP (Inheritance solves it)

    return 0;
}