#include "priority.h"
#include <iostream>
#include <vector>
using namespace std;

void runPriority(std::vector<Process>& processes) {
    int n = processes.size();
    int time = 0, completed = 0;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrivalTime <= time) {
                if (idx == -1 ||
                    processes[i].priority > processes[idx].priority ||
                    (processes[i].priority == processes[idx].priority &&
                        processes[i].id < processes[idx].id))
                    idx = i;
            }
        }
        if (idx == -1) { time++; continue; }

        processes[idx].startTime = time;
        time += processes[idx].burstTime;
        processes[idx].finishTime = time;
        processes[idx].turnaroundTime = processes[idx].finishTime - processes[idx].arrivalTime;
        processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        done[idx] = true;
        completed++;
    }
}