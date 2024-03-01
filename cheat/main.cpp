#include "memory.h"
#include <thread>
#include <iostream>
#include <cstdlib>
#include <clocale>

namespace offsets
{
	constexpr auto localPlayer = 0x1730118;
	constexpr auto flags = 0x3D4;
	constexpr auto forceJump = 0x17294A0;
}

int main(int argc, char* argv[])
{
	const auto em = Memory("cs2.exe");
	const int Standing = 65665;
	const int pjump = 65537;
	const int mjump = 256;
	int pid = em.pid;
	setlocale(LC_ALL, "Russian");
	if (pid == 0) {
		std::cout << "start cs2 please!\nзапусти кс 2 \n";
		while (pid == 0) {
			Sleep(200);
			Memory em = Memory("cs2.exe");
			pid = em.pid;
		}
		std::cout << "found!\nигра нейдена\n";
		std::cout << "press numpad 7 to restart and enable bhop, del to off. \nнажми 7 на нумпаде чтобы закрыть это окно и врубить бхоп. Когда нужно будет вырубить бхоп нажми del. Убедись что нум лок включен. Также выруби когда закончишь играть!\n";
		std::string command = "start ";
		while (true) {
			if (GetAsyncKeyState(VK_NUMPAD7)) {
				command += argv[0];
				system(command.c_str());

				exit(0);
			}
		}

	}
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	const auto mem = Memory("cs2.exe");
	const auto client = mem.GetModuleAddress("client.dll");
	while (true)
	{
		// we don't need this running a billion times per second :)
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(client + offsets::localPlayer);
		const unsigned int mflags = mem.Read<std::uint32_t>(localPlayer + offsets::flags);
		const bool space = GetAsyncKeyState(VK_SPACE);
		const auto force_jump = mem.Read<DWORD>(client + offsets::forceJump);

		// bhop
		if (space) {
			if (mflags == Standing and force_jump == 256) {
				Sleep(1);
				mem.Write<uintptr_t>(client + offsets::forceJump, pjump);
				
			}
			else {
				mem.Write<uintptr_t>(client + offsets::forceJump, mjump);
			}
			Sleep(23);
		}
		if (GetAsyncKeyState(VK_DELETE)) {
			exit(EXIT_SUCCESS);
		}
	}
}
