#pragma once
#include "process.h"
#include "process_generator.h"
#include "fcfs.h"
#include "sjf.h"
#include "priority.h"
#include "rr.h"
#include "mlq.h"
#include <vector>
#include <fstream>
namespace OSSchedulerGUI {
    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    ref class GanttForm : public Form {
    public:
        GanttForm(String^ title, System::Collections::Generic::List<array<int>^>^ data) {
            this->Text = title + " - Gantt Chart";
            this->Size = Drawing::Size(900, 200);
            this->BackColor = Color::White;
            PictureBox^ canvas = gcnew PictureBox();
            canvas->Dock = DockStyle::Fill;
            canvas->BackColor = Color::White;
            canvas->Tag = data;
            canvas->Paint += gcnew PaintEventHandler(this, &GanttForm::onPaint);
            this->Controls->Add(canvas);
        }
    private:
        void onPaint(Object^ sender, PaintEventArgs^ e) {
            PictureBox^ pb = (PictureBox^)sender;
            auto data = (System::Collections::Generic::List<array<int>^>^)pb->Tag;
            Graphics^ g = e->Graphics;
            if (data->Count == 0) return;
            int maxTime = 0;
            for each(array<int> ^ row in data)
                if (row[2] > maxTime) maxTime = row[2];
            int marginL = 60, marginT = 40, barH = 40;
            int chartW = pb->Width - marginL - 20;
            float scale = (float)chartW / maxTime;
            array<Color>^ colors = {
                Color::FromArgb(70,130,200), Color::FromArgb(80,180,100),
                Color::FromArgb(220,100,80), Color::FromArgb(180,100,200),
                Color::FromArgb(220,160,60), Color::FromArgb(60,180,180)
            };
            for each(array<int> ^ row in data) {
                int pid = row[0], start = row[1], finish = row[2];
                int x = marginL + (int)(start * scale);
                int w = (int)((finish - start) * scale);
                Color c = colors[pid % colors->Length];
                g->FillRectangle(gcnew SolidBrush(c), x, marginT, w - 1, barH);
                g->DrawRectangle(Pens::White, x, marginT, w - 1, barH);
                if (w > 20) {
                    String^ label = "P" + pid.ToString();
                    Drawing::Font^ f = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold);
                    SizeF sz = g->MeasureString(label, f);
                    g->DrawString(label, f, Brushes::White, x + (w - sz.Width) / 2, marginT + (barH - sz.Height) / 2);
                }
            }
            Drawing::Font^ tf = gcnew Drawing::Font("Segoe UI", 7);
            for (int t = 0; t <= maxTime; t++) {
                int x = marginL + (int)(t * scale);
                g->DrawLine(Pens::LightGray, x, marginT + barH, x, marginT + barH + 5);
                g->DrawString(t.ToString(), tf, Brushes::Black, x - 3, marginT + barH + 6);
            }
            g->DrawString("CPU", gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold), Brushes::DimGray, 5, marginT + 10);
        }
    };
    public ref class MainForm : public Form {
    public:
        MainForm() {
            this->Text = "OS Scheduler";
            this->Size = Drawing::Size(1100, 820);
            this->BackColor = Color::FromArgb(245, 245, 250);
            Label^ title = gcnew Label();
            title->Text = "OS Scheduler";
            title->Font = gcnew Drawing::Font("Segoe UI", 20, FontStyle::Bold);
            title->ForeColor = Color::FromArgb(30, 60, 140);
            title->Location = Point(420, 12);
            title->AutoSize = true;
            Label^ lblInput = gcnew Label();
            lblInput->Text = "Enter Processes:";
            lblInput->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
            lblInput->Location = Point(15, 55);
            lblInput->AutoSize = true;
            dgvInput = gcnew DataGridView();
            dgvInput->Location = Point(15, 78);
            dgvInput->Size = Drawing::Size(650, 185);
            dgvInput->BackgroundColor = Color::White;
            dgvInput->AllowUserToAddRows = true;
            dgvInput->AllowUserToDeleteRows = true;
            dgvInput->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgvInput->BorderStyle = BorderStyle::FixedSingle;
            dgvInput->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(30, 60, 140);
            dgvInput->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            dgvInput->ColumnHeadersDefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            dgvInput->EnableHeadersVisualStyles = false;
            dgvInput->RowHeadersVisible = false;
            dgvInput->Columns->Add("colID", "ID");
            dgvInput->Columns->Add("colArrival", "Arrival Time");
            dgvInput->Columns->Add("colBurst", "Burst Time");
            dgvInput->Columns->Add("colPriority", "Priority");
            dgvInput->Rows->Add("1", "0", "5", "3");
            dgvInput->Rows->Add("2", "2", "3", "5");
            dgvInput->Rows->Add("3", "4", "8", "2");
            Button^ btnClear = gcnew Button();
            btnClear->Text = "Clear Table";
            btnClear->Location = Point(680, 78);
            btnClear->Size = Drawing::Size(130, 38);
            btnClear->BackColor = Color::FromArgb(210, 70, 70);
            btnClear->ForeColor = Color::White;
            btnClear->FlatStyle = FlatStyle::Flat;
            btnClear->FlatAppearance->BorderSize = 0;
            btnClear->Click += gcnew EventHandler(this, &MainForm::btnClear_Click);
            Button^ btnAddRow = gcnew Button();
            btnAddRow->Text = "+ Add Row";
            btnAddRow->Location = Point(680, 126);
            btnAddRow->Size = Drawing::Size(130, 38);
            btnAddRow->BackColor = Color::FromArgb(60, 160, 80);
            btnAddRow->ForeColor = Color::White;
            btnAddRow->FlatStyle = FlatStyle::Flat;
            btnAddRow->FlatAppearance->BorderSize = 0;
            btnAddRow->Click += gcnew EventHandler(this, &MainForm::btnAddRow_Click);
            btnRun = gcnew Button();
            btnRun->Text = "Run Schedulers";
            btnRun->Location = Point(680, 174);
            btnRun->Size = Drawing::Size(130, 38);
            btnRun->BackColor = Color::FromArgb(30, 90, 200);
            btnRun->ForeColor = Color::White;
            btnRun->FlatStyle = FlatStyle::Flat;
            btnRun->FlatAppearance->BorderSize = 0;
            btnRun->Click += gcnew EventHandler(this, &MainForm::btnRun_Click);
            Label^ lblQuantum = gcnew Label();
            lblQuantum->Text = "RR Quantum:";
            lblQuantum->Location = Point(840, 90);
            lblQuantum->AutoSize = true;
            txtQuantum = gcnew TextBox();
            txtQuantum->Text = "4";
            txtQuantum->Location = Point(940, 87);
            txtQuantum->Size = Drawing::Size(55, 25);
            lblStatus = gcnew Label();
            lblStatus->Text = "";
            lblStatus->ForeColor = Color::FromArgb(60, 150, 60);
            lblStatus->Location = Point(15, 270);
            lblStatus->AutoSize = true;

            TabControl^ tabs = gcnew TabControl();
            tabs->Location = Point(10, 295);
            tabs->Size = Drawing::Size(1065, 480);

            dgvFCFS = makeGrid();
            dgvSJF = makeGrid();
            dgvPriority = makeGrid();
            dgvRR = makeGrid();
            dgvMLQ = makeGrid();

            ganttFCFS = gcnew System::Collections::Generic::List<array<int>^>();
            ganttSJF = gcnew System::Collections::Generic::List<array<int>^>();
            ganttPriority = gcnew System::Collections::Generic::List<array<int>^>();
            ganttRR = gcnew System::Collections::Generic::List<array<int>^>();
            ganttMLQ = gcnew System::Collections::Generic::List<array<int>^>();

            tabs->TabPages->Add(makeTab("FCFS", dgvFCFS, "FCFS", ganttFCFS));
            tabs->TabPages->Add(makeTab("SJF", dgvSJF, "SJF", ganttSJF));
            tabs->TabPages->Add(makeTab("Priority", dgvPriority, "Priority", ganttPriority));
            tabs->TabPages->Add(makeTab("Round Robin", dgvRR, "RR", ganttRR));
            tabs->TabPages->Add(makeTab("MLQ", dgvMLQ, "MLQ", ganttMLQ));

            this->Controls->Add(title);
            this->Controls->Add(lblInput);
            this->Controls->Add(dgvInput);
            this->Controls->Add(btnClear);
            this->Controls->Add(btnAddRow);
            this->Controls->Add(btnRun);
            this->Controls->Add(lblQuantum);
            this->Controls->Add(txtQuantum);
            this->Controls->Add(lblStatus);
            this->Controls->Add(tabs);
        }

    private:
        Button^ btnRun;
        TextBox^ txtQuantum;
        DataGridView^ dgvInput;
        DataGridView^ dgvFCFS, ^ dgvSJF, ^ dgvPriority, ^ dgvRR, ^ dgvMLQ;
        Label^ lblStatus;
        System::Collections::Generic::List<array<int>^>^ ganttFCFS, ^ ganttSJF, ^ ganttPriority, ^ ganttRR, ^ ganttMLQ;

        DataGridView^ makeGrid() {
            DataGridView^ g = gcnew DataGridView();
            g->Dock = DockStyle::Fill;
            g->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            g->ReadOnly = true;
            g->BackgroundColor = Color::White;
            g->ColumnHeadersDefaultCellStyle->BackColor = Color::FromArgb(30, 60, 140);
            g->ColumnHeadersDefaultCellStyle->ForeColor = Color::White;
            g->EnableHeadersVisualStyles = false;
            g->RowHeadersVisible = false;
            return g;
        }

        TabPage^ makeTab(String^ name, DataGridView^ grid, String^ algoName,
            System::Collections::Generic::List<array<int>^>^ ganttData) {
            TabPage^ page = gcnew TabPage(name);
            Panel^ panel = gcnew Panel();
            panel->Dock = DockStyle::Fill;
            Button^ btn = gcnew Button();
            btn->Text = "Show Gantt Chart";
            btn->Dock = DockStyle::Bottom;
            btn->Height = 36;
            btn->BackColor = Color::FromArgb(30, 90, 200);
            btn->ForeColor = Color::White;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->Tag = gcnew array<Object^>(2) { algoName, ganttData };
            btn->Click += gcnew EventHandler(this, &MainForm::btnGantt_Click);
            grid->Dock = DockStyle::Fill;
            panel->Controls->Add(grid);
            panel->Controls->Add(btn);
            page->Controls->Add(panel);
            return page;
        }
        void btnGantt_Click(Object^ sender, EventArgs^ e) {
            Button^ btn = (Button^)sender;
            auto tag = (array<Object^>^)btn->Tag;
            auto data = (System::Collections::Generic::List<array<int>^>^)tag[1];
            if (data->Count == 0) { MessageBox::Show("Run the schedulers first!"); return; }
            (gcnew GanttForm((String^)tag[0], data))->Show();
        }
        std::vector<Process> readFromTable() {
            std::vector<Process> processes;
            for (int i = 0; i < dgvInput->Rows->Count - 1; i++) {
                DataGridViewRow^ row = dgvInput->Rows[i];
                if (row->Cells[0]->Value == nullptr) continue;
                Process p;
                p.id = Int32::Parse(row->Cells[0]->Value->ToString());
                p.arrivalTime = Int32::Parse(row->Cells[1]->Value->ToString());
                p.burstTime = Int32::Parse(row->Cells[2]->Value->ToString());
                p.priority = Int32::Parse(row->Cells[3]->Value->ToString());
                p.remainingTime = p.burstTime;
                p.waitingTime = p.turnaroundTime = 0;
                p.startTime = -1; p.finishTime = 0;
                processes.push_back(p);
            }
            return processes;
        }
        void buildGantt(std::vector<Process>& procs, System::Collections::Generic::List<array<int>^>^ gantt) {
            gantt->Clear();
            for (auto& p : procs)
                gantt->Add(gcnew array<int>(3) { p.id, p.startTime, p.finishTime });
        }
        void fillGrid(DataGridView^ grid, std::vector<Process>& processes) {
            grid->Rows->Clear(); grid->Columns->Clear();
            grid->Columns->Add("ID", "ID");
            grid->Columns->Add("Arrival", "Arrival");
            grid->Columns->Add("Burst", "Burst");
            grid->Columns->Add("Priority", "Priority");
            grid->Columns->Add("Completion", "Completion");
            grid->Columns->Add("Waiting", "Waiting");
            grid->Columns->Add("Turnaround", "Turnaround");
            double wt = 0, tat = 0;
            for (auto& p : processes) {
                grid->Rows->Add(p.id, p.arrivalTime, p.burstTime, p.priority, p.finishTime, p.waitingTime, p.turnaroundTime);
                wt += p.waitingTime; tat += p.turnaroundTime;
            }
            int n = (int)processes.size();
            if (n > 0) {
                int r = grid->Rows->Add("AVG", "", "", "", "", Math::Round(wt / n, 2), Math::Round(tat / n, 2));
                grid->Rows[r]->DefaultCellStyle->BackColor = Color::FromArgb(255, 255, 180);
                grid->Rows[r]->DefaultCellStyle->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            }
        }
        void btnClear_Click(Object^ sender, EventArgs^ e) { dgvInput->Rows->Clear(); lblStatus->Text = "";
        }
        void btnAddRow_Click(Object^ sender, EventArgs^ e) {
            dgvInput->Rows->Add(dgvInput->Rows->Count.ToString(), "0", "1", "1");
        }
        void btnRun_Click(Object^ sender, EventArgs^ e) {
            try {
                std::vector<Process> processes = readFromTable();
                if (processes.empty()) { MessageBox::Show("Please enter at least one process!"); return; }
                int quantum = Int32::Parse(txtQuantum->Text);
                std::vector<Process> p1(processes); runFCFS(p1);        fillGrid(dgvFCFS, p1);     buildGantt(p1, ganttFCFS);
                std::vector<Process> p2(processes); runSJF(p2);         fillGrid(dgvSJF, p2);      buildGantt(p2, ganttSJF);
                std::vector<Process> p3(processes); runPriority(p3);    fillGrid(dgvPriority, p3); buildGantt(p3, ganttPriority);
                std::vector<Process> p4(processes); runRR(p4, quantum); fillGrid(dgvRR, p4);       buildGantt(p4, ganttRR);
                std::vector<Process> p5(processes); runMLQ(p5);         fillGrid(dgvMLQ, p5);      buildGantt(p5, ganttMLQ);
                lblStatus->Text = "Done! Results shown in tabs below.";
            }
            catch (...) { MessageBox::Show("Error! Make sure all cells have valid numbers."); }
        }
    };
}