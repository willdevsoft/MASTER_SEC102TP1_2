// SEC102 TP1 Exercice2 by Wilfried Poisson
//
// Objectif: Enumération de processus avec les API WIN32
// V0.2: début du TP avec l'utilisation de l'API win32 
// Voir fichier README dans GitHub
//Enumération des processus PID


#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <string.h>

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

int PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	DWORD cbNeeded; //rajout
	unsigned int i; //rajout
	HMODULE hMods[1024]; //rajout
	DWORD dwFileSize2;
	LARGE_INTEGER filesize2;

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return 1;
	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	// Print the process name and identifier.

	_tprintf(TEXT("Processus name: %s  (PID: %u)\n"), szProcessName, processID);

	// rajout module name and handle value

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				// Print the module name and handle value.

				_tprintf(TEXT("\t Module name: %s    Handle value: (0x%08X)\n"), szModName, hMods[i]);
			}
// Rajout taille du module
			DWORD dwFileSize;
			DWORD dwFileType;
			HANDLE hFile = CreateFile(szModName, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
			// Get the file size...
			dwFileSize = GetFileSize(hFile, NULL);
			printf(" size: %d bytes\n", dwFileSize);
		}
	}


	// Fin rajout --------------------
	// Release the handle to the process.

	CloseHandle(hProcess);
}





int main(void)
{
	// Get the list of process identifiers.

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}


	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			PrintProcessNameAndID(aProcesses[i]);
		}
	}

	return 0;
}