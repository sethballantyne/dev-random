#pragma once
#include <vector>
#include <string>

#define FS_ERROR_PATH_TOO_LONG 1
#define FS_ERROR_INVALID_PATH  2
#define FS_ERROR_GENERAL_B0RK  3

class FileSystem
{
private:
	int GetFiles(std::wstring path, std::vector<std::wstring>& directories, std::vector<std::wstring>& files);
public:
	FileSystem() = default;
	~FileSystem() = default;

	// don't need these fuckers (yet)
	FileSystem(const FileSystem&) = delete;			
	FileSystem& operator=(const FileSystem&) = delete;

	// Gets all the files within the specified path -- including subdirectories -- and stores the absolute path
	// of each file (including filenames) in 'files'.
	int GetFiles(std::wstring path, std::vector<std::wstring>& files);
};