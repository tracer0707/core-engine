#include "FileSystemUtils.h"

#include <iostream>

#include <Core/Shared/Path.h>

#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"

namespace Editor
{
	Core::List<Core::String> FileSystemUtils::getDiskDrives()
	{
		Core::List<Core::String> drives;

#ifdef _WIN32
		for (char drive = 'A'; drive <= 'Z'; ++drive)
		{
			std::string drive_path = std::string(1, drive) + ":\\";
			if (std::filesystem::exists(drive_path))
			{
				drives.add(Core::String(drive_path).replace('\\', '/'));
			}
		}
#else
		std::filesystem::path mount_path("/");
		if (std::filesystem::exists(mount_path))
		{
			drives.add(mount_path.string());
		}

		std::vector<std::string> common_mounts = {"/mnt", "/media", "/Volumes"};
		for (const auto& mount : common_mounts)
		{
			if (std::filesystem::exists(mount) && std::filesystem::is_directory(mount))
			{
				drives.add(mount);
			}
		}
#endif

		return drives;
	}

	Core::List<std::filesystem::path> FileSystemUtils::getPathEntries(Core::String path)
	{
		Core::List<std::filesystem::path> fs;
		for (const auto& entry : std::filesystem::directory_iterator(path.std_str(), std::filesystem::directory_options::skip_permission_denied))
		{
			fs.add(entry.path());
		}

		fs.sort([](std::filesystem::path& a, std::filesystem::path& b) -> bool {
			bool isDirA = std::filesystem::is_directory(a);
			bool isDirB = std::filesystem::is_directory(b);

			std::string _a = a.generic_string();
			std::string _b = b.generic_string();

			std::transform(_a.begin(), _a.end(), _a.begin(), [](unsigned char c) { return std::tolower(c); });
			std::transform(_b.begin(), _b.end(), _b.begin(), [](unsigned char c) { return std::tolower(c); });

			if (isDirA && isDirB) return _a < _b;
			if (isDirA) return 1;
			if (isDirB) return 0;

			return _a < _b;
		});

		return fs;
	}

	void FileSystemUtils::enumerateFiles(const std::filesystem::path& root, Core::List<std::filesystem::path>& out)
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
		{
			Core::String _p = entry.path().generic_string();
			if (entry.is_regular_file() && !Core::Path::isHiddenOrSystem(_p))
			{
				_p = entry.path().lexically_relative(root).generic_string();
				out.add(_p.std_str());
			}
		}
	}

	void FileSystemUtils::fsToTreeView(Core::String path, TreeView* treeView, TreeNode* rootNode, bool addFiles, bool showRootNode)
	{
		std::string _path = path.std_str();

		if (!showRootNode)
		{
			Core::List<std::filesystem::path> fs = getPathEntries(_path);
			for (const auto& entry : fs)
			{
				Core::String path = entry.generic_string();
				if (Core::Path::isHiddenOrSystem(path)) continue;
				if (!addFiles && !std::filesystem::is_directory(entry)) continue;

				fsToTreeView(path, treeView, nullptr, addFiles, true);
			}
			return;
		}

		auto fs_path = std::filesystem::path(path.std_str());

		TreeNode* _node = treeView->createNode();

		if (fs_path.has_filename())
			_node->setText(fs_path.filename().generic_string());
		else
			_node->setText(_path);

		_node->setStringTag(0, fs_path.generic_string());

		if (rootNode != nullptr)
			rootNode->addControl(_node);
		else
			treeView->addControl(_node);

		if (std::filesystem::is_directory(_path))
		{
			Core::List<std::filesystem::path> fs = getPathEntries(_path);

			if (addFiles)
			{
				_node->setAlwaysShowOpenArrow(fs.count() > 0);
			}
			else
			{
				bool hasDir = false;
				for (const auto& entry : fs)
				{
					if (std::filesystem::is_directory(entry))
					{
						hasDir = true;
						break;
					}
				}
				_node->setAlwaysShowOpenArrow(hasDir);
			}

			_node->setOnOpen([_path, treeView, _node, addFiles, fs](bool opened) {
				if (opened)
				{
					for (const auto& entry : fs)
					{
						Core::String path = entry.generic_string();
						if (Core::Path::isHiddenOrSystem(path)) continue;
						if (!addFiles && !std::filesystem::is_directory(entry)) continue;

						fsToTreeView(path, treeView, _node, addFiles, true);
					}
				}
				else
				{
					_node->clear();
				}
			});
		}
	}
} // namespace Editor