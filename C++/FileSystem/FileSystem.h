#pragma once
#include <vector>
#include <string>

class FileSystem
{
private:
	void GetFiles(std::wstring path, std::vector<std::wstring>& directories, std::vector<std::wstring>& files);
public:
	FileSystem() = default;
	std::vector<std::wstring> GetFiles(std::wstring path);
};