#include <queue>
#include <Windows.h>
#include "FileSystem.h"
using namespace std;

void FileSystem::GetFiles(wstring path, vector<wstring>& directories, vector<wstring>& files)
{
	if(path.length() > MAX_PATH - 3)
	{
		throw exception();
	}

	WIN32_FIND_DATA findData;
	wstring temp = path + L"\\*";
	HANDLE hFind = FindFirstFile(temp.c_str(), &findData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		//MessageBox(NULL, temp.c_str(), L"HELLO!", MB_OK|MB_ICONEXCLAMATION);
		//wcout << temp.c_str() << endl;
		// error DisplayErrorBox(TEXT("FINDFIRSTFILE"))
	}

	do
	{
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wstring dirName(findData.cFileName);
			if(dirName != L"." && dirName != L"..")
			{
				if(path.back() != '\\')
				{
					path += L"\\";
				}
				//wcout << "DIR: " << wstring(findData.cFileName) << endl;
				directories.push_back(path + wstring(findData.cFileName) + L"\\");
			}
		}
		else
		{
			files.push_back(path + wstring(findData.cFileName));
		}
	} while(FindNextFile(hFind, &findData) != 0);

	DWORD error = GetLastError();

	if(error != ERROR_NO_MORE_FILES)
	{
		throw exception();

		// error DisplayErrorBox(TEXT("FindFirstFile"))
	}

	FindClose(hFind);
}

vector<wstring> FileSystem::GetFiles(std::wstring path)
{
	queue<wstring> pathQueue;
	pathQueue.push(path);

	vector<wstring> directories;
	vector<wstring> files;

	while(!pathQueue.empty())
	{
		GetFiles(pathQueue.front(), directories, files);

		for(auto dir : directories)
		{
			pathQueue.push(dir);
		}

		directories.clear();
		pathQueue.pop();
	}

	return files;
}