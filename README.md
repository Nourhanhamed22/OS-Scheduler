# OS Scheduler Simulator

A C++ simulation of classic CPU scheduling algorithms, with both a **console interface** and a **Windows Forms (C++/CLI) GUI** featuring interactive Gantt charts.

## Overview

This project implements and compares five CPU scheduling algorithms commonly taught in operating systems courses:

- **FCFS** — First-Come, First-Served
- **SJF** — Shortest Job First (non-preemptive)
- **Priority Scheduling** (non-preemptive, higher number = higher priority)
- **Round Robin (RR)** — preemptive, configurable time quantum
- **MLQ** — Multi-Level Queue (3 levels: high priority uses Round Robin, mid priority uses FCFS, low priority uses SJF)

For each algorithm, the program computes per-process **start time**, **finish time**, **waiting time**, and **turnaround time**, along with the averages across all processes.

## Features

- Random process generation from statistical parameters (normal/Poisson distributions)
- Manual process entry via an editable data grid (GUI mode)
- Side-by-side results tables for all five algorithms
- Visual **Gantt chart** rendering per algorithm (GUI mode)
- Configurable Round Robin time quantum

## Project Structure

```
├── main.cpp                 # Console entry point — runs all algorithms sequentially
├── GuiMain.cpp               # WinForms GUI entry point
├── MainForm.h / MainForm.cpp # Main GUI window, results grids, Gantt chart rendering
├── MainForm.resx             # Form resources
├── process.h                 # Process struct definition
├── process_generator.h/.cpp  # Random process generator (reads input.txt → output.txt)
├── fcfs.h / fcfs.cpp          # First-Come, First-Served
├── sjf.h / sjf.cpp            # Shortest Job First
├── priority.h / priority.cpp  # Priority Scheduling
├── rr.h / rr.cpp               # Round Robin
├── mlq.h / mlq.cpp             # Multi-Level Queue
├── input.txt                  # Parameters for random process generation
└── output.txt                 # Generated processes (auto-created)
```

## The `Process` Model

```cpp
struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime, turnaroundTime, remainingTime;
    int startTime, finishTime;
};
```

## Getting Started

### Prerequisites

- A C++ compiler (C++11 or later) for the **console** version
- Windows + Visual Studio with **.NET / C++/CLI (Windows Forms)** support for the **GUI** version

### Console Version

1. Build `main.cpp` together with `process_generator.cpp`, `fcfs.cpp`, `sjf.cpp`, `priority.cpp`, `rr.cpp`, and `mlq.cpp`.
2. Ensure `input.txt` is in the working directory (see format below).
3. Run the executable. It will:
   - Generate a randomized process set into `output.txt`
   - Load and display the processes
   - Run FCFS, SJF, Priority, and MLQ automatically
   - Prompt for a Round Robin quantum, then run RR

### GUI Version

1. Open the project in Visual Studio as a **CLR (Windows Forms) Console/Application** project.
2. Build with `GuiMain.cpp` as the entry point.
3. Run the app:
   - Enter processes directly in the input grid (or edit the default sample rows)
   - Set the Round Robin quantum
   - Click **Run Schedulers** to populate results for all five algorithms in separate tabs
   - Click **Show Gantt Chart** on any tab to view a visual timeline of that algorithm's execution order

## Input Format (`input.txt`)

Used only by the random process generator (console mode):

```
<numProcesses>
<arrivalMean> <arrivalStdDev>
<burstMean> <burstStdDev>
<priorityLambda>
```

Example:
```
10
8.5 1.4
10 5.3
7.9
```

This generates 10 processes with arrival times drawn from a normal distribution (mean 8.5, std dev 1.4), burst times from a normal distribution (mean 10, std dev 5.3), and priorities from a Poisson distribution (λ = 7.9).

## Output Format (`output.txt`)

Auto-generated process list, one process per line:

```
<numProcesses>
<id> <arrivalTime> <burstTime> <priority>
...
```

## Scheduling Notes

- **FCFS**: processes run strictly in arrival order (ties broken by ID).
- **SJF**: non-preemptive; among available processes, the shortest burst time runs next (ties broken by ID).
- **Priority**: non-preemptive; higher `priority` value runs first (ties broken by ID).
- **Round Robin**: preemptive; processes cycle through a ready queue with a fixed time quantum.
- **MLQ**: processes are split into three queues by priority —
  - Priority ≥ 7 → Round Robin
  - Priority 4–6 → FCFS
  - Priority ≤ 3 → SJF

## License

This project is provided as-is for educational purposes.
