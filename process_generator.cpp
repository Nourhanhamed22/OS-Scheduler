#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include "process_generator.h"
#include "process.h"
using namespace std;

void generateProcesses() {
    ifstream inFile("input.txt");
    if (!inFile) {
        cout << "Error: input.txt not found! Make sure it's in the project folder." << endl;
        return;
    }

    int numProcesses;
    double arrMean, arrStd, burstMean, burstStd, prioLambda;

    inFile >> numProcesses;
    inFile >> arrMean >> arrStd;
    inFile >> burstMean >> burstStd;
    inFile >> prioLambda;
    inFile.close();

    default_random_engine generator;
    normal_distribution<double> arrivalDist(arrMean, arrStd);
    normal_distribution<double> burstDist(burstMean, burstStd);
    poisson_distribution<int> priorityDist(prioLambda);

    ofstream outFile("output.txt");
    if (!outFile) {
        cout << "Error: cannot create output.txt!" << endl;
        return;
    }

    outFile << numProcesses << endl;

    for (int i = 1; i <= numProcesses; ++i) {
        int arrival = max(0, (int)round(arrivalDist(generator)));
        int burst = max(1, (int)round(burstDist(generator)));
        int priority = priorityDist(generator);
        outFile << i << " " << arrival << " " << burst << " " << priority << endl;
    }

    outFile.close();
    cout << "Success! output.txt has been created." << endl;
}