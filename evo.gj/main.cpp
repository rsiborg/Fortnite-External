#include <Windows.h>
#include <iostream>
#include "render/render.h"
#include "cheat/esp/esp.h"

using namespace std;

void rndmTitle()
{
	constexpr int length = 25;
	const auto characters = TEXT("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	TCHAR title[length + 1]{};

	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 55 + 1];
	}

	SetConsoleTitle(title);
}
struct slowly_printing_string {
	std::string data;
	long int delay;
};
std::ostream& operator<<(std::ostream& out, const slowly_printing_string& s) {
	for (const auto& c : s.data) {
		out << c << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(s.delay));
	}
	return out;
}

void load_driver() {
menu_:
	int choice;
	system("color 3");
	rndmTitle();
	system("cls");
	printf("Welcome to Evo.gj. Made by Ritz, Updated by Droused\n\n");
	printf("Please select an option below: \n");
	printf((" [1] Load Cheat\n\n > "));
	std::cin >> choice;
	switch (choice)
	{
	case 1:
		break;
	default:
		break;
	}
}
auto main() -> int //memez
{

	printf("Loading...");

	load_driver();

	driver::find_driver();
	ProcId = driver::find_process((L"FortniteClient-Win64-Shipping.exe"));
	if (ProcId == 0) {
		system("cls");
		printf("\n\nFortnite was not found. Please make sure it's open!\n\n");
		system("pause");
	}
	else {

		BaseId = driver::find_image();
		system("cls");
		if (Debug::PrintPointers) {
			Util::PrintPtr("ProcessId: ", ProcId);
			Util::PrintPtr("BaseId: ", BaseId);
		}

		if (BaseId == 0 || BaseId == NULL) {
			std::cout << "\nDriver was not mapped" << std::endl;
			std::system("pause");
		}

		else {
			Render::CreateOverlay();
			Render::DirectXInit();
			Render::MainLoop();
		}
	}

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	return 0;
}
