#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void __cdecl _tmain()
{
	char* path = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist";
	key(path);
}

void __cdecl key(char* path)
{
	HKEY hTestKey;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT(path), 0, KEY_READ, &hTestKey) == ERROR_SUCCESS)
	{
		QueryKey(path, 0);
	}

	RegCloseKey(hTestKey);
}

void QueryKey(char *path, const int root)
{
	TCHAR achKey[MAX_KEY_LENGTH]; // buffer for subkey name
	DWORD cbName; // size of name string
	TCHAR achClass[MAX_PATH] = TEXT(""); // buffer for class name
	DWORD cchClassName = MAX_PATH; // size of class string
	DWORD cSubKeys = 0; // number of subkeys
	DWORD cbMaxSubKey; // longest subkey size
	DWORD cchMaxClass; // longest class string
	DWORD cValues; // number of values for key
	DWORD cchMaxValue; // longest value name
	DWORD cbMaxValueData; // longest value data
	DWORD cbSecurityDescriptor; // size of security descriptor
	FILETIME ftLastWriteTime; // last write time


	HKEY hKey;
	RegOpenKeyEx(HKEY_CURRENT_USER, TEXT(path), 0, KEY_READ, &hKey);



	DWORD i, retCode;

	TCHAR achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	char *subpath;
	int j;

	// Get the class name and the value count.
	retCode = RegQueryInfoKey(
		hKey, // key handle
		achClass, // buffer for class name
		&cchClassName, // size of class string
		NULL, // reserved
		&cSubKeys, // number of subkeys
		&cbMaxSubKey, // longest subkey size
		&cchMaxClass, // longest class string
		&cValues, // number of values for this key
		&cchMaxValue, // longest value name
		&cbMaxValueData, // longest value data
		&cbSecurityDescriptor, // security descriptor
		&ftLastWriteTime); // last write time

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
			if (retCode == ERROR_SUCCESS)
			{
				for (j = 0; i < root; i++)
				{
					if (i % 2 == 0 || i == 0)
						printf("%c", 179);
					else
						printf(" ");

				}
				printf("%c%c%s\n", 195, 196, achKey);

				subpath = malloc(strlen(path) + 2 + strlen(achKey));

				strcpy(subpath, path);
				strcat(subpath, "\\");
				strcat(subpath, achKey);

				QueryKey(subpath, root + 2);
			}
		}
	}

	// Enumerate the key values.

	if (cValues)
	{
		for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
		{
			cchValue = MAX_VALUE_NAME;
			achValue[0] = '\0';
			retCode = RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL);

			if (retCode == ERROR_SUCCESS)
			{
				for (j = 0; i < root; i++)
				{
					if (i % 2 == 0 || i == 0)
						printf("%c", 179);
					else
						printf(" ");

				}
				printf("%c%c%s\n", 195, 196, achValue);
			}
		}
	}
}
