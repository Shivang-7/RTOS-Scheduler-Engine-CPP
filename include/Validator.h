// This runs the Liu and Layland Schedulability Test

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "Task.h"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std; 

class Validator {
public:
    static double calculateUtilization(const vector<Task>& tasks) {
        double utilization = 0.0;
        for (const auto& task : tasks) {
            utilization += static_cast<double>(task.burstTime) / task.period;
        }
        return utilization;
    }

    static bool checkRMSBound(const vector<Task>& tasks) {
        size_t n = tasks.size();
        if (n == 0) return true;

        double utilization = calculateUtilization(tasks);
        double upperBound = n * (pow(2.0, 1.0 / n) - 1.0);

        cout << "\n=== Schedulability Report ===" << endl;
        cout << "Total CPU Utilization (U): " << utilization << endl;
        cout << "RMS Bound (n=" << n << "): " << upperBound << endl;

        if (utilization <= upperBound) {
            cout << "Status: PASSED (Guaranteed Schedulable)" << endl;
            return true;
        } else if (utilization <= 1.0) {
            cout << "Status: WARNING (U <= 1.0, but exceeds RMS bound)" << endl;
            return false;
        } else {
            cout << "Status: OVERLOADED (U > 1.0)" << endl;
            return false;
        }
    }
};

#endif // VALIDATOR_H