#include "IO.h"

#ifdef _WIN32
#include "../Classes/dirent.h"
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

#include <filesystem>

#include <fstream>
#include <stdio.h>
#include <iostream>

#include "Path.h"

namespace Core
{
	const UString IO::readText(const UString& path)
	{
		std::string result_str = "";
		std::string path_str = String::toStdString(path);
		std::ifstream in(path_str, std::ios::in);

		if (!in.is_open()) {
			return "";
		}

		std::string line;
		while (std::getline(in, line))
		{
			result_str += line + "\n";
		}

		in.close();

		UString result = String::fromStdString(result_str);
		result = result.findAndReplace('\r\n', '\n');

		return result;
	}

	void IO::writeText(const UString& path, const UString& text, bool append)
	{
		const std::string path_str = String::toStdString(path);
		const std::string text_str = String::toStdString(text);

		const std::ios_base::openmode open_mode = append ? (std::ios::out | std::ios::app) : std::ios::out;

		std::ofstream text_file;
		text_file.open(path_str, open_mode);
		text_file << text_str;
		text_file.close();
	}

	const bool IO::pathExists(const UString& path)
	{
		std::string path_str = String::toStdString(path);

#ifdef _WIN32
		struct _stat buffer;
		return (_stat(path_str.c_str(), &buffer) == 0);
#else
		struct stat buffer;
		return (stat(path_str.c_str(), &buffer) == 0);
#endif
	}

	const bool IO::fileExists(const UString& path)
	{
		return pathExists(path) && !Path::isDir(path);
	}

	const bool IO::dirExists(const UString& path)
	{
		return pathExists(path) && Path::isDir(path);
	}

	void IO::fileCopy(const UString& from, const UString& to)
	{
		const std::string from_str = String::toStdString(from);
		const std::string to_str = String::toStdString(from);

		std::ifstream src(from_str, std::ios::binary);
		std::ofstream dst(to_str, std::ios::binary);

		dst << src.rdbuf();
	}

	void IO::dirCopy(const UString& from, const UString& to, bool recursive)
	{
		const std::string from_str = String::toStdString(from);
		const std::string to_str = String::toStdString(from);

		const std::filesystem::copy_options options = recursive ?
			(std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing) :
			std::filesystem::copy_options::overwrite_existing;

		std::filesystem::copy(std::filesystem::path(from_str), std::filesystem::path(to_str), options);
	}

	void IO::fileRename(const UString& from, const UString& to)
	{
		const std::string from_str = String::toStdString(from);
		const std::string to_str = String::toStdString(from);

		std::error_code err;
		std::filesystem::rename(from_str, to_str, err);

		//TODO: обработка ошибок
	}

	void IO::createDir(const UString& path, const bool& recursive)
	{
		std::string path_str = String::toStdString(path);

#ifdef _WIN32
		if (!recursive)
		{
			std::wstring stemp = std::wstring(path_str.begin(), path_str.end());
			LPCWSTR sw = stemp.c_str();
			CreateDirectory(sw, NULL);
		}
		else
		{
			std::filesystem::create_directories(path_str);
		}
#else
		if (!recursive)
		{
			mkdir(path_str.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
		else
		{
			std::filesystem::create_directories(path_str);
		}
#endif
	}

	void IO::fileDelete(const UString& path)
	{
		std::string path_str = String::toStdString(path);
		std::remove(path_str.c_str());
	}

	void IO::dirDelete(const UString& path, const bool& recursive)
	{
		std::string path_str = String::toStdString(path);

		if (std::filesystem::exists(path_str))
		{
			if (recursive)
				std::filesystem::remove_all(path_str);
			else
				std::filesystem::remove(path_str);
		}
	}
}