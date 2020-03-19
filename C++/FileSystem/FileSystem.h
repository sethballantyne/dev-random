#pragma once
#include <vector>
#include <string>

#define FS_ERROR_PATH_TOO_LONG 1
#define FS_ERROR_INVALID_PATH  2
#define FS_ERROR_GENERAL_B0RK  3

class FileSystem
{
private:
	static int GetFiles(std::wstring path, std::vector<std::wstring>& directories, std::vector<std::wstring>& files);
public:
	FileSystem() = default;
	~FileSystem() = default;

	// don't need these fuckers (yet)
	FileSystem(const FileSystem&) = delete;					// copy ctor
	FileSystem(const FileSystem&&) = delete;                // move ctor
	FileSystem& operator=(const FileSystem&) = delete;      // copy assignment operator
	FileSystem& operator=(const FileSystem&&) = delete;     // move assignment operator

	// Gets all the files within the specified path -- including subdirectories -- and stores the absolute path
	// of each file (including filenames) in 'files'.
	static int GetFiles(std::wstring path, std::vector<std::wstring>& files);
};