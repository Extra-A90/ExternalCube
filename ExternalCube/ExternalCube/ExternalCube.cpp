// ExternalCube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <locale>

#include "proc.h"
#include "mem.h"

#include "ConsoleCol.h"

using namespace std;

bool DebugMode = false;
bool GodMode = false;

std::vector<std::string> Split(string str, char Delim) {
	std::vector<std::string> Args;
	std::stringstream ss(str);
	std::string Arg;
	while (std::getline(ss, Arg, Delim)) {
		Args.push_back(Arg);
	}
	return Args;
}

int FindPointer(int offset, HANDLE pHandle, int baseaddr, int offsets[])
{
	int Address = baseaddr;
	int total = offset;
	for (int i = 0; i < total; i++) //Loop trough the offsets
	{
		cout << "New Address: " << hex << Address << endl;
		ReadProcessMemory(pHandle, (LPCVOID)Address, &Address, 4, NULL);
		Address += offsets[i];
	}
	return Address;
}

wstring widen(const string& str)
{
	wostringstream wstm;
	const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}

string narrow(const wstring& str)
{
	ostringstream stm;

	// Incorrect code from the link
	// const ctype<char>& ctfacet = use_facet<ctype<char>>(stm.getloc());

	// Correct code.
	const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(stm.getloc());

	for (size_t i = 0; i < str.size(); ++i)
		stm << ctfacet.narrow(str[i], 0);
	return stm.str();
}


std::string tolower(std::string str)
{
	std::string retn("");
	for (size_t i = 0; i < str.length(); i++)
	{
		int ascii = (int)str.at(i);
		if (ascii >= (int)'a' && ascii <= (int)'z')
			retn += (char)ascii;
		else
			retn += (char)(ascii + ((int)'a' - (int)'A'));
	}
	return retn;
}

void ExecuteCommand(string cmd, DWORD ProcIdA, uintptr_t ModuleBaseA, HANDLE hProcessA, uintptr_t PlayerStructureA) {

	DWORD ProcId = ProcIdA;
	uintptr_t ModuleBase = ModuleBaseA;
	HANDLE hProcess = hProcessA;
	uintptr_t PlayerStructure = PlayerStructureA;

	std::vector<std::string> In = Split(cmd, ' ');
	if (In.size() != 0) {
		if (tolower(In.at(0)) == "cmds") {
			cout << iaqua << "" << endl;
			cout << "----------------------------------" << endl;
			cout << "        Character Commands        " << endl;
			cout << "----------------------------------" << endl;
			cout << white << "sethealth [#] - Sets your current health to #" << endl;
			cout << "godmode [t/f] - Sets your health to a big number and prevents it from changing" << endl;
			cout << red << "" << endl;
			cout << "----------------------------------" << endl;
			cout << "          Weapon Commands         " << endl;
			cout << "----------------------------------" << endl;
			cout << white << "primaryammo [#] - Sets your primary gun (Shotgun, Carbine, etc) ammo to #" << endl;
			cout << "pistolammo [#] - Sets your secondary gun (Pistol, etc) ammo to #" << endl;
			cout << "primaryreserve [#] - Sets your primary gun (Shotgun, Carbine, etc) reserve ammo to #" << endl;
			cout << "pistolreserve [#] - Sets your secondary gun (Pistol, etc) reserve ammo to #" << endl;
			cout << red << "" << endl;
			cout << "----------------------------------" << endl;
			cout << "           Map Commands           " << endl;
			cout << "----------------------------------" << endl;
			cout << white << "mapname - Returns the name of the map you are currently playing on." << endl;
			cout << red << "" << endl;
			cout << "----------------------------------" << endl;
			cout << "          Online Commands         " << endl;
			cout << "----------------------------------" << endl;
			cout << white << "aimbot [t/f] - Automatically locks onto the other teams head for you." << endl;
			cout << "esp [t/f] - Puts a box around players and bots to see them through walls, etc." << endl;
			cout << "triggerbot [t/f] - Automatically fires your gun for you when your mouse is on another player or bot." << endl;
			cout << red << "" << endl;
			cout << "----------------------------------" << endl;
			cout << "          Cheat Commands         " << endl;
			cout << "----------------------------------" << endl;
			cout << white << "debugmode [t/f] - Used for debugging the cheat, such as changes the cheat makes." << endl;
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "help") {
			cout << iaqua << "" << endl;
			cout << "----------------------------------" << endl;
			cout << "        Online Information        " << endl;
			cout << "----------------------------------" << endl;
			cout << white << "This cheat can be used for online multiplayer purposes. The only commands that will work online are the commands under 'Online Commands' (when you type 'cmds')" << endl;
			cout << "Setting your health to 9999999 when playing multiplayer will not work, as the server handles it. If you are playing with bots though, setting your health works." << endl;
			cout << "Remember, our cheat is FULLY undetected. You will not get banned unless a staff member of that server bans you (If they even exist)." << endl;
			cout << red << "" << endl;
			cout << "----------------------------------" << endl;
			cout << "       General Information        " << endl;
			cout << "----------------------------------" << endl;
			cout << white << "This section will tell you what certain things in the 'cmds' output mean." << endl;
			cout << "# = Number. You must put a number when running this command." << endl;
			cout << "t/f = True or false. You must put 't' or 'f'. This is used to usually turn the cheats on, or off." << endl;
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "debugmode") {
			string truefalse = tolower(In.at(1));

			if (truefalse == "t" || truefalse == "true") {
				DebugMode = true;
			}

			if (truefalse == "f" || truefalse == "false") {
				DebugMode = false;
			}

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] Debug mode is now on. All output that is yellow will usually be output from Debug Mode. To turn this off, type 'debugmode t/f' (you can also do 'debugmode true/false')" << endl;
			}
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "sethealth") {
			ReadProcessMemory(hProcess, (BYTE*)ModuleBase + 0x10F4F4, &PlayerStructure, sizeof(PlayerStructure), nullptr);

			uintptr_t Health_Addr = PlayerStructure + 0xF8;
			int HealthValue = 0;
			int NewHealthValue = stoi(In.at(1));

			//string InAString_NewHealthValue = tolower(In.at(1));
			//stringstream StringToInt(InAString_NewHealthValue);

			//StringToInt >> NewHealthValue;

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] Converted string to int, current value of int (Input):" << NewHealthValue << endl;
			}

			if (DebugMode == true) { //We dont need the "== true" part, I know noah, I just like doing it just in case :) Feel free to change this, also if you read this please DM me on discord telling me you did, thanks! SO DO IT PLZ! Tyvm.
				cout << iyellow << "[DEBUG] The health address is currently: 0x" << hex << Health_Addr << endl;
				cout << iyellow << "[DEBUG] The base address is currently: 0x" << hex << PlayerStructure << endl;
			}

			ReadProcessMemory(hProcess, (BYTE*)Health_Addr, &HealthValue, sizeof(HealthValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] The current Health is: " << dec << HealthValue << endl;
			}

			WriteProcessMemory(hProcess, (BYTE*)Health_Addr, &NewHealthValue, sizeof(NewHealthValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] Wrote to the address, it should now be: " << dec << HealthValue << endl;
			}
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "godmode" || tolower(In.at(0)) == "gmode") {
			if (In.at(1) == "t" || In.at(1) == "true") {
				uintptr_t Health_Addr = PlayerStructure + 0xF8;
				int HealthValue = 0;
				int NewHealthValue = 2147483647;

				ReadProcessMemory(hProcess, (BYTE*)Health_Addr, &HealthValue, sizeof(HealthValue), nullptr);

				if (DebugMode == true) {
					cout << iyellow << "[DEBUG] The current value of Health is: " << dec << HealthValue << endl;
				}

				WriteProcessMemory(hProcess, (BYTE*)Health_Addr, &NewHealthValue, sizeof(NewHealthValue), nullptr);

				if (DebugMode == true) {
					cout << iyellow << "[DEBUG] Did WriteProcessMemory. The current value of health (hopefully changed) is: " << dec << HealthValue << endl;
				}
			}

			if (In.at(1) == "f" || In.at(1) == "false") {
				uintptr_t Health_Addr = PlayerStructure + 0xF8;
				int HealthValue = 0;
				int NewHealthValue = 100;

				ReadProcessMemory(hProcess, (BYTE*)Health_Addr, &HealthValue, sizeof(HealthValue), nullptr);

				if (DebugMode == true) {
					cout << iyellow << "[DEBUG] The current value of Health is: " << dec << HealthValue << endl;
				}

				WriteProcessMemory(hProcess, (BYTE*)Health_Addr, &NewHealthValue, sizeof(NewHealthValue), nullptr);

				if (DebugMode == true) {
					cout << iyellow << "[DEBUG] Did WriteProcessMemory. The current value of health (hopefully changed) is: " << dec << HealthValue << endl;
				}
			}
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "primaryammo") {
			uintptr_t Primary_Addr = PlayerStructure + 0x148;
			int PrimaryValue = 0;
			int NewPrimaryValue = stoi(In.at(1));

			ReadProcessMemory(hProcess, (BYTE*)Primary_Addr, &PrimaryValue, sizeof(PrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] The current value of Primary Ammo is: " << dec << PrimaryValue << endl;
			}

			WriteProcessMemory(hProcess, (BYTE*)Primary_Addr, &NewPrimaryValue, sizeof(NewPrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] Did WriteProcessMemory. The current value of primary ammo (hopefully changed) is: " << dec << PrimaryValue << endl;
			}
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "pistolammo") {
			uintptr_t Primary_Addr = PlayerStructure + 0x13C;
			int PrimaryValue = 0;
			int NewPrimaryValue = stoi(In.at(1));

			ReadProcessMemory(hProcess, (BYTE*)Primary_Addr, &PrimaryValue, sizeof(PrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] The current value of Pistol Ammo is: " << dec << PrimaryValue << endl;
			}

			WriteProcessMemory(hProcess, (BYTE*)Primary_Addr, &NewPrimaryValue, sizeof(NewPrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] Did WriteProcessMemory. The current value of pistol ammo (hopefully changed) is: " << dec << PrimaryValue << endl;
			}
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "primaryreserve") {
			uintptr_t Primary_Addr = PlayerStructure + 0x120;
			int PrimaryValue = 0;
			int NewPrimaryValue = stoi(In.at(1));

			ReadProcessMemory(hProcess, (BYTE*)Primary_Addr, &PrimaryValue, sizeof(PrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] The current value of Primary Reserve Ammo is: " << dec << PrimaryValue << endl;
			}

			WriteProcessMemory(hProcess, (BYTE*)Primary_Addr, &NewPrimaryValue, sizeof(NewPrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] Did WriteProcessMemory. The current value of primary reserve ammo (hopefully changed) is: " << dec << PrimaryValue << endl;
			}
			cout << white << "" << endl;
		}

		if (tolower(In.at(0)) == "pistolreserve") {
			uintptr_t Primary_Addr = PlayerStructure + 0x114;
			int PrimaryValue = 0;
			int NewPrimaryValue = stoi(In.at(1));

			ReadProcessMemory(hProcess, (BYTE*)Primary_Addr, &PrimaryValue, sizeof(PrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] The current value of Pistol Reserve Ammo is: " << dec << PrimaryValue << endl;
			}

			WriteProcessMemory(hProcess, (BYTE*)Primary_Addr, &NewPrimaryValue, sizeof(NewPrimaryValue), nullptr);

			if (DebugMode == true) {
				cout << iyellow << "[DEBUG] Did WriteProcessMemory. The current value of pistol reserve ammo (hopefully changed) is: " << dec << PrimaryValue << endl;
			}
			cout << white << "" << endl;
		}
	}
}

int main()
{
	cout << white << "Getting Proccess ID for ac_client.exe... ";
	DWORD ProcId = GetProcId(L"ac_client.exe");
	if (ProcId) {
		cout << igreen << "Success!" << endl;

		//GetModuleBaseAddress

		cout << white << "Getting Module Base for ac_client.exe (" << ProcId << ")... ";
		uintptr_t ModuleBase = GetModuleBaseAddress(ProcId, L"ac_client.exe");
		cout << igreen << "Success!" << endl;

		//Get Handle to game

		cout << white << "Getting a handle to the game... ";
		HANDLE hProcess = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcId);
		cout << igreen << "Success!" << endl;

		DWORD dwExit = 0;
		while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {

			//Resolve base address of pointer chain

			cout << white << "Getting Player/Entity Structure... ";
			uintptr_t PlayerStructure = 0;

			ReadProcessMemory(hProcess, (BYTE*)ModuleBase + 0x10F4F4, &PlayerStructure, sizeof(PlayerStructure), nullptr);
			cout << igreen << "Success!" << endl;

			cout << white << "Starting input option... ";
			string i;
			cout << igreen << "Success!" << endl;

			cout << iaqua << "Finished set-up! Type 'cmds' for a list of commands, or 'help' if you need to know what certain things mean!" << endl;
			cout << white << "" << endl;

			while (true) {
				getline(cin, i);
				ExecuteCommand(i, ProcId, ModuleBase, hProcess, PlayerStructure);
			}
		}
	}
	else {
		cout << ired << "Failed! Please make sure the game is running, or contact support!" << endl;
	}

	getchar();
	return 0;
}

/*

    //Get ProcId of game
    DWORD ProcId = GetProcId(L"ac_client.exe");

    //GetModuleBaseAddress

    uintptr_t ModuleBase = GetModuleBaseAddress(ProcId, L"ac_client.exe");

    //Get Handle to game

    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, ProcId);

    //Resolve base address of pointer chain

    uintptr_t PlayerStructure = 0;

    ReadProcessMemory(hProcess, (BYTE*)ModuleBase + 0x109B74, &PlayerStructure, sizeof(PlayerStructure), nullptr);

    //Resolve our ammo pointer chain

    //vector<unsigned int> Offsets = { 0x0, 0x278 };

    uintptr_t Health_Addr = PlayerStructure + 0xF8;

    cout << "The DMA Addy is: 0x" << hex << Health_Addr << endl;

    //Read Ammo Value

    int HealthValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)Health_Addr, &HealthValue, sizeof(HealthValue), nullptr);
    cout << "Current ammo is: " << dec << HealthValue << endl;

    //Write to it

    int NewHealth = 9999;
    WriteProcessMemory(hProcess, (BYTE*)Health_Addr, &NewHealth, sizeof(NewHealth), nullptr);

    //Read out again
    ReadProcessMemory(hProcess, (BYTE*)Health_Addr, &HealthValue, sizeof(HealthValue), nullptr);

    cout << "Set ammo to: " << dec << HealthValue << endl;

    getchar();
    return 0;

*/