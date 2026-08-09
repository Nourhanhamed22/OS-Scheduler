#include "fcfs.h"
#include <iostream>
#include <algorithm>

void runFCFS(std::vector<Process>& processes) {
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        if (a.arrivalTime == b.arrivalTime) return a.id < b.id;
        return a.arrivalTime < b.arrivalTime;
        });
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime)
            currentTime = p.arrivalTime;
        p.startTime = currentTime;
        p.finishTime = currentTime + p.burstTime;
        p.waitingTime = p.startTime - p.arrivalTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        currentTime = p.finishTime;
    }
}