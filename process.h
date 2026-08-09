#ifndef PROCESS_H
#define PROCESS_H

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;

    int waitingTime = 0;
    int turnaroundTime = 0;
    int remainingTime = 0;
    int startTime = -1;
    int finishTime = 0;
};

#endif
