#include "MyForm.h"
#include <Windows.h>


using namespace Kursach;

[STAThread] //Задаем текущий поток как поток как поток однопоточного контейнера (STA), чтобы вызовы OLE стали возможны
//без этого будет ошибка с потоками (которая возникает при автозаполнении в comboBox)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm);
}