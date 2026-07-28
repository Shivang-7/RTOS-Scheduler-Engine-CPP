//Represents a shared hardware resource (like a Shared Sensor Bus or Memory Buffer) protected by a mutex lock.

#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
using namespace std;

struct Resource {
    string id;            // e.g., "Radar_Data_Bus"
    bool isLocked;             // True if locked by a task
    string lockedByTaskId; // ID of the task currently holding the lock

    Resource(string name) : id(name), isLocked(false), lockedByTaskId("") {}
};

#endif // RESOURCE_H