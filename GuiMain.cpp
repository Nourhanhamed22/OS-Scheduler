#include "MainForm.h"
#include <windows.h>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew OSSchedulerGUI::MainForm());
    return 0;
}