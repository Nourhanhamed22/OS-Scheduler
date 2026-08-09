#include <iostream>
#include <fstream>
#include <vector>
#include "process.h"
#include "process_generator.h"
#include "fcfs.h"
#include "sjf.h"
#include "priority.h"
#include "rr.h"
#include "mlq.h"
using namespace std;

int main() {
    cout << "Generating processes from input.txt...\n";
    generateProcesses();

    ifstream inFile("output.txt");
    if (!inFile) {
        cout << "Error: output.txt not found!\n";
        return 1;
    }

    int n;
    inFile >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        inFile >> processes[i].id
            >> processes[i].arrivalTime
            >> processes[i].burstTime
            >> processes[i].priority;
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
        processes[i].startTime = -1;
        processes[i].finishTime = 0;
    }
    inFile.close();

    cout << "\n========== Loaded Processes ==========\n";
    cout << "ID\tArrival\tBurst\tPriority\n";
    for (auto& p : processes)
        cout << p.id << "\t" << p.arrivalTime << "\t"
        << p.burstTime << "\t" << p.priority << "\n";

    cout << "\n========== Scheduling Results ==========\n";

    runFCFS(processes);
    runSJF(processes);
    runPriority(processes);
    runMLQ(processes);

    int quantum;
    cout << "\nEnter Round Robin Quantum: ";
    cin >> quantum;
    runRR(processes, quantum);

    cout << "\n========== Done ==========\n";
    return 0;
}