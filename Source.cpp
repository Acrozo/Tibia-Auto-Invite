#include <iostream>
#include <windows.h>
#include <string.h>
#include <fstream>
#include <chrono>

#pragma warning(disable : 4996)

using namespace std;
using namespace std::chrono;

LPCSTR TibiaWindowName;

char TibiaNameInput[255];
char AletaSioString[] = "aleta sio";

char *ctime_no_newline;

ifstream file;
bool IsPlaying = false;
POINT P;

size_t StoreTibiaNameInputSize;

string CharacterName, TibiaWindowNameFinal, SecondaryTibiaNameInput;
int mousex, mousey;

void Color(int color = 0x07)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void GetLocalTime()
{
	time_t tm = time(NULL);
	ctime_no_newline = strtok(ctime(&tm), "\n"); // removing the endl or \n from the time output
	Color(0x06);
	cout << ctime_no_newline;
}


void ReadPlayerNamesFromFile()
{    // Constantly reading the file, to check for a new name;

	file.open("Players.txt");

	if (file.fail())
		cout << "Failed to open file" << endl;

	file >> TibiaNameInput;
	file.close();
}

void SayAletaSio()
{
	for (int i = 0; i <= strlen(AletaSioString); i++)
	{
		if (AletaSioString[i] == NULL)
			break;
		else
			PostMessage(FindWindow(NULL, TibiaWindowName), WM_CHAR, AletaSioString[i], 0); // types out the name of the player letter by letter
	}
	PostMessage(FindWindow(NULL, TibiaWindowName), WM_CHAR, VK_RETURN, 0);
	Sleep(200);
}

void TypePlayerName()
{
	SecondaryTibiaNameInput = TibiaNameInput;
	StoreTibiaNameInputSize = strlen(TibiaNameInput); //storing an auxiliar number for later comparisons
	for (int i = 0; i <= strlen(TibiaNameInput); i++)
	{
		if (TibiaNameInput[i] == NULL)
			break;
		else
			PostMessage(FindWindow(NULL, TibiaWindowName), WM_CHAR, TibiaNameInput[i], 0); // types out the name of the player letter by letter

	}
}

void ClickOkTibiaClient()
{
	// mousey ALWAYS -25 for Tibia client ?? I dont know why, it just wont click normally.
	PostMessage(FindWindow(NULL, TibiaWindowName), WM_LBUTTONDOWN, 0, MAKELPARAM(mousex, mousey - 25));
	PostMessage(FindWindow(NULL, TibiaWindowName), WM_LBUTTONUP, 0, MAKELPARAM(mousex, mousey - 25));
}

void RemoveKickPlayer()
{
	if (IsPlaying == true)
	{
		if (strlen(TibiaNameInput) == 0)
		{
			SayAletaSio();
			Sleep(100);
			for (int i = 0; i <= StoreTibiaNameInputSize; i++)
				PostMessage(FindWindow(NULL, TibiaWindowName), WM_CHAR, VK_BACK, 0);      // Delete old player name

			Sleep(100);
			ClickOkTibiaClient();
			IsPlaying = false;
			GetLocalTime();
			cout << "   " << SecondaryTibiaNameInput << " has been kicked" << endl;
		}
	}
	Sleep(300);
}

void InvitePlayer()
{
	if (IsPlaying != true)
	{
		if (strlen(TibiaNameInput) != 0)
		{
			SayAletaSio();
			Sleep(100);
			TypePlayerName();
			Sleep(100);
			ClickOkTibiaClient();
			GetLocalTime();
			cout << "   " << SecondaryTibiaNameInput << " has been invited" << endl;
			IsPlaying = true;
		}
	}
	Sleep(300);
}

void SetCursorPosition()
{
	Color(0x09);
	cout << "Press 'insert' or 'ins' once you know your Aleta Sio 'OK' button location and input the values" << endl;
	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT))
			break;
		GetCursorPos(&P);
		cout << P.x << endl << P.y << "\n" << endl;
		Sleep(1500);
	}
	cout << "Please input your 'OK' button location values" << endl;
	cin >> mousex >> mousey;
	system("cls");
}

int main()
{
	SetConsoleTitle("Tibia Auto Inviter");

	Color(0x04);
	cout << "TO MAKE THIS PROGRAM WORKS MAKE SURE TO RUN IT AS ADMINISTRATOR" << endl;

	Color(0x0A);
	cout << "Please input your character name: ";
	cin >> CharacterName;

	TibiaWindowNameFinal = "Tibia - " + CharacterName;
	TibiaWindowName = TibiaWindowNameFinal.c_str();

	system("cls");

	SetCursorPosition();

	while (true)
	{
		ReadPlayerNamesFromFile();
		InvitePlayer();
		RemoveKickPlayer();
	}

	return 0;
}