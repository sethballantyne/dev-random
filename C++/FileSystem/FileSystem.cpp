#include <queue>
#include <set>
#include <Windows.h>
#include <iostream>
#include "FileSystem.h"
using namespace std;

int FileSystem::GetFiles(wstring path, vector<wstring>& directories, vector<wstring>& files)
{
	set<wchar_t> dirSeperators = { '\\', '//' };
	bool endsWithDirSeperator = false;

	// path doesn't end with a directory seperator and there's no room to append one 
	// along with the * to specify we want everything in the directory.
	if(dirSeperators.find(path.back()) == dirSeperators.end())
	{
		if(path.length() > MAX_PATH - 3)
		{
			return FS_ERROR_PATH_TOO_LONG;
		}
	}
	else
	{
		endsWithDirSeperator = true;
	}

	WIN32_FIND_DATA findData;

	if(!endsWithDirSeperator)
	{
		path += L"\\";
	}

	HANDLE hFind = FindFirstFile((wstring(path) + L"*").c_str(), &findData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		return FS_ERROR_INVALID_PATH;
	}

	do
	{
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			
			wstring dirName(findData.cFileName);

			// ignore these fuckers, with .., we'll end up processing the entire filesystem.
			// (which was a fun bug to discover)
			if(dirName != L"." && dirName != L"..")                
			{
				//wcout << "DIR: " << wstring(findData.cFileName) << endl;
				wstring finalPath = path + wstring(findData.cFileName);
				directories.push_back(finalPath);
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
		return FS_ERROR_GENERAL_B0RK;
	}

	FindClose(hFind);

	return 0;
}

int FileSystem::GetFiles(std::wstring path, vector<wstring>& destVector)
{
	queue<wstring> pathQueue;
	pathQueue.push(path);

	vector<wstring> directories;
	vector<wstring> files;

	while(!pathQueue.empty())
	{
		
		int retval = GetFiles(pathQueue.front(), directories, files);
		if(retval != 0)
		{
			return retval;
		}

		for(auto dir : directories)
		{
			pathQueue.push(dir);
		}

		directories.clear();
		pathQueue.pop();
	}

	destVector = files;
	return 0;
}