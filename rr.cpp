#include "rr.h"
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

void runRR(std::vector<Process>& processes, int quantum) {
    for (auto& p : processes)
        p.remainingTime = p.burstTime;

    sort(processes.begin(), processes.end(), [](Process a, Process b) {
        return a.arrivalTime != b.arrivalTime ?
            a.arrivalTime < b.arrivalTime : a.id < b.id;
        });

    queue<Process*> readyQueue;
    int currentTime = 0, completed = 0;
    int n = processes.size();
    vector<bool> inQueue(n, false);

    readyQueue.push(&processes[0]);
    inQueue[0] = true;

    while (completed < n) {
        if (readyQueue.empty()) {
            currentTime++;
            for (int i = 0; i < n; i++)
                if (!inQueue[i] && processes[i].arrivalTime <= currentTime)
                    readyQueue.push(&processes[i]), inQueue[i] = true;
            continue;
        }

        Process* current = readyQueue.front();
        readyQueue.pop();

        if (current->remainingTime == current->burstTime)
            current->startTime = currentTime;

        int runTime = min(quantum, (int)current->remainingTime);
        currentTime += runTime;
        current->remainingTime -= runTime;

        for (int i = 0; i < n; i++)
            if (!inQueue[i] && processes[i].arrivalTime <= currentTime)
                readyQueue.push(&processes[i]), inQueue[i] = true;

        if (current->remainingTime == 0) {
            current->finishTime = currentTime;
            current->turnaroundTime = current->finishTime - current->arrivalTime;
            current->waitingTime = current->turnaroundTime - current->burstTime;
            completed++;
        }
        else {
            readyQueue.push(current);
        }
    }
}