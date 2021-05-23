#include "services/TaskService.hpp"

#include <iostream>
#include <chrono>

// just a simple wrapper
inline uint64_t getCurrTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

TaskService::TaskService(ObjectPtr r): Service(r) {
    srvType = TASKSRV;
    std::cout << "TaskService Loaded!" << std::endl;
}

void TaskService::pushTask(TaskCallback callBk, void* userData, uint64_t delta, bool timer) {
    uint64_t currTime = getCurrTime();
    tasks.push({callBk, userData, currTime+delta, delta, timer});
}

void TaskService::tick() {
    uint64_t currTime = getCurrTime();
    Task task;

    // since the queue is ordered, the task at the top should be the next task in time
    //std::cout << "next event : " << tasks.top().event << " <= " << currTime << std::endl;
    while((task = tasks.top()).event <= currTime) {
        // remove the task from the queue
        tasks.pop();

        // run the task
        task.callback(task.userData, currTime, currTime-task.event+task.delta);

        // if its a timer, reschedule it
        if (task.timer) {
            task.event = currTime+task.delta;
            tasks.push(task);
        }
    }
}