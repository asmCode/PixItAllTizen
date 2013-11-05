#pragma once

#include <string>
#include <vector>
#include <fstream>

class Path
{
private:
	std::string file_path;
	std::string file_name;
	std::string file_ext;

public:
	Path(std::string path)
	{
		if (path.empty())
			return;

		int strLen;
		int lastSlash = path.find_last_of("/");

		// splitting path and filename + ext
		strLen = path.size();
		if (lastSlash != -1)
		{
			file_path = path.substr(0, lastSlash + 1);

			if (lastSlash + 1 != strLen)
				file_name = path.substr(lastSlash + 1, -1);
		}
		else
			file_name = path;

		// splitting filename and ext
		strLen = file_name.size();
		int lastDot = file_name.find_last_of(".");		
		if (lastDot != -1 && lastDot != 0)
		{
			if (lastDot + 1 != strLen)
				file_ext = file_name.substr(lastDot + 1, -1);

			file_name = file_name.substr(0, lastDot);
		}
	}

	std::string GetPath()
	{
		return file_path;
	}

	std::string GetExt()
	{
		return file_ext;
	}

	std::string GetFilename()
	{
		return file_name;
	}

	std::string GetFilenameExt()
	{
		if (!file_ext.empty())
			return file_name + "." + GetExt();
		else
			return file_name;
	}

	std::string GetFullPath()
	{
		return file_path + GetFilenameExt();
	}

	static bool IsFileExists(const char *fileName);

	// ads slash at the end of the string, if required
	static void FixFoldername(std::string &folderName)
	{
		if (folderName.find_last_of("/") == folderName.size() - 1)
			return;

		folderName += "/";
	}

	static void GetAllFiles(std::vector<std::string> &files,
							const std::string &path,
							const std::string &filter);
};
