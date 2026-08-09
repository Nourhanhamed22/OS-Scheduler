#include "mlq.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
static void calc(Process& p, int currentTime) {
    p.finishTime = currentTime;
    p.turnaroundTime = p.finishTime - p.arrivalTime;
    p.waitingTime = p.turnaroundTime - p.burstTime;
}
void runMLQ(std::vector<Process>& processes, int quantum) {
    vector<Process*> q1, q2, q3;

    for (auto& p : processes)
        (p.priority >= 7 ? q1 : p.priority >= 4 ? q2 : q3).push_back(&p);
    auto cmp = [](Process* a, Process* b) { return a->arrivalTime < b->arrivalTime; };
    sort(q1.begin(), q1.end(), cmp);
    sort(q2.begin(), q2.end(), cmp);
    sort(q3.begin(), q3.end(), cmp);
    int t = 0;
    queue<Process*> rq;
    vector<bool> inQ(q1.size(), false);
    for (auto p : q1) p->remainingTime = p->burstTime;
    if (!q1.empty()) { t = q1[0]->arrivalTime; rq.push(q1[0]); inQ[0] = true; }
    while (!rq.empty()) {
        auto cur = rq.front(); rq.pop();
        if (cur->startTime == -1) cur->startTime = max(t, cur->arrivalTime);
        int run = min(quantum, cur->remainingTime);
        t += run;
        cur->remainingTime -= run;

        for (int i = 0; i < (int)q1.size(); i++)
            if (!inQ[i] && q1[i]->arrivalTime <= t)
                rq.push(q1[i]), inQ[i] = true;

        if (cur->remainingTime == 0) calc(*cur, t);
        else rq.push(cur);

        if (rq.empty())
            for (int i = 0; i < (int)q1.size(); i++)
                if (!inQ[i]) { t = max(t, q1[i]->arrivalTime); rq.push(q1[i]); inQ[i] = true; break; }
    }

    for (auto p : q2) { t = max(t, p->arrivalTime); p->startTime = t; t += p->burstTime; calc(*p, t); }

    vector<bool> done(q3.size(), false);
    for (int doneCnt = 0; doneCnt < (int)q3.size(); ) {
        int idx = -1;
        for (int i = 0; i < (int)q3.size(); i++)
            if (!done[i] && q3[i]->arrivalTime <= t)
                if (idx == -1 || q3[i]->burstTime < q3[idx]->burstTime) idx = i;
        if (idx == -1) { t++; continue; }
        q3[idx]->startTime = t;
        t += q3[idx]->burstTime;
        calc(*q3[idx], t);
        done[idx] = true;
        doneCnt++;
    }
}