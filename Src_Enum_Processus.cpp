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
#include "HeaderEnumProcessus.h"

using namespace std;

int main(void)
{
	// Get the list of process identifiers.

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}

	// Calcul le nombre de process identifiés
	cProcesses = cbNeeded / sizeof(DWORD);
	// Affiche le nom et l'identifiant pour chaque process.

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			PrintProcessNameAndID(aProcesses[i]);
		}
	}

	return 0;
}



// Fonction permettant d'afficher:
// nom du Process/id/Chemin/Taille


int PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	DWORD cbNeeded; //rajout
	unsigned int i; //rajout
	HMODULE hMods[1024]; //rajout
	DWORD dwFileSize2;
	LARGE_INTEGER filesize2;

	// Récupére un handle du process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess)
		return 1;

	// Si valeur du handle non nul alors
	// On récupére le nom du process.
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

	// Affiche le nom du process et son identifiant PID.

	_tprintf(TEXT("Processus name: %s  (PID: %u)\n"), szProcessName, processID);

	// Récuperation du nom du module et de la valeur du handle

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			DWORD dwModuleSize;

			HANDLE hModule = CreateFile(szModName, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
			// Récupére la taille du module
			dwModuleSize = GetFileSize(hModule, NULL);
			// Récupére le chemin complet du module (fichier).

			if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
			// Affiche le nom du module et la valeur du handle.

				_tprintf(TEXT("\t Module name: %s    Handle value: (0x%08X)  size: %d bytes\n"), szModName, hMods[i], dwModuleSize);
			}

		}
	}
	CloseHandle(hProcess);
}



