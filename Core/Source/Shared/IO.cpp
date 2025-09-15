#include "IO.h"

#include <filesystem>

#include <fstream>
#include <stdio.h>
#include <iostream>

#include "Path.h"

namespace Core
{
	String IO::readText(String& path)
	{
		std::string result_str = "";
		std::ifstream in(path.std_str(), std::ios::in);

		if (!in.is_open()) {
			return "";
		}

		std::string line;
		while (std::getline(in, line))
		{
			result_str += line + "\n";
		}

		in.close();

		return String(result_str).replace('\r\n', '\n');
	}

	void IO::writeText(String& path, String& text, bool append)
	{
		const std::ios_base::openmode open_mode = append ? (std::ios::out | std::ios::app) : std::ios::out;

		std::ofstream text_file;
		text_file.open(path.std_str(), open_mode);
		text_file << text.std_str();
		text_file.close();
	}

	List<String> IO::getDiskDrives()
	{
		List<String> drives;

#ifdef _WIN32
		for (char drive = 'A'; drive <= 'Z'; ++drive) {
			std::string drive_path = std::string(1, drive) + ":\\";
			if (std::filesystem::exists(drive_path)) {
				drives.add(drive_path);
			}
		}
#else
		std::filesystem::path mount_path("/");
		if (std::filesystem::exists(mount_path)) {
			drives.add(mount_path.string());
		}

		std::vector<std::string> common_mounts = { "/mnt", "/media", "/Volumes" };
		for (const auto& mount : common_mounts) {
			if (std::filesystem::exists(mount) &&
				std::filesystem::is_directory(mount)) {
				drives.add(mount);
			}
		}
#endif

		return drives;
	}
}