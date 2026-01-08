#include "FileSystemUtils.h"

#include <iostream>

#include <Core/Shared/Path.h>

#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"
#include "../Shared/IconsForkAwesome.h"

namespace fs = std::filesystem;

namespace Editor
{
	Core::List<Core::String> FileSystemUtils::getDiskDrives()
	{
		Core::List<Core::String> drives;

#ifdef _WIN32
		for (char drive = 'A'; drive <= 'Z'; ++drive)
		{
			std::string drive_path = std::string(1, drive) + ":\\";
			if (fs::exists(drive_path))
			{
				drives.add(Core::String(drive_path).replace('\\', '/'));
			}
		}
#else
		fs::path mount_path("/");
		if (fs::exists(mount_path))
		{
			drives.add(mount_path.string());
		}

		std::vector<std::string> common_mounts = {"/mnt", "/media", "/Volumes"};
		for (const auto& mount : common_mounts)
		{
			if (fs::exists(mount) && fs::is_directory(mount))
			{
				drives.add(mount);
			}
		}
#endif

		return drives;
	}

	Core::List<fs::path> FileSystemUtils::getPathEntries(Core::String path)
	{
		Core::List<fs::path> fs;
		for (const auto& entry : fs::directory_iterator(path.std_str(), fs::directory_options::skip_permission_denied))
		{
			fs.add(entry.path());
		}

		fs.sort([](fs::path& a, fs::path& b) -> bool {
			bool isDirA = fs::is_directory(a);
			bool isDirB = fs::is_directory(b);

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

	void FileSystemUtils::enumerateFiles(const fs::path& root, Core::List<fs::path>& out)
	{
		for (const auto& entry : fs::recursive_directory_iterator(root))
		{
			Core::String _p = entry.path().generic_string();
			if (entry.is_regular_file() && !Core::Path::isHiddenOrSystem(_p))
			{
				_p = Core::Path::relative(entry.path().generic_string(), root.generic_string());
				out.add(_p.std_str());
			}
		}
	}

	void FileSystemUtils::fsToTreeView(Core::String path, TreeView* treeView, TreeNode* rootNode, bool addFiles, bool showRootNode, bool lazyLoad)
	{
		std::string _path = path.std_str();

		if (!showRootNode)
		{
			Core::List<fs::path> fs = getPathEntries(_path);
			for (const auto& entry : fs)
			{
				Core::String path = entry.generic_string();
				if (Core::Path::isHiddenOrSystem(path)) continue;
				if (!addFiles && !fs::is_directory(entry)) continue;

				fsToTreeView(path, treeView, nullptr, addFiles, true, lazyLoad);
			}
			return;
		}

		auto fs_path = fs::path(path.std_str());

		TreeNode* _node = treeView->createNode();

		if (fs_path.has_filename())
		{
			std::string icon;
			if (fs::is_directory(fs_path))
			{
				icon = ICON_FK_FOLDER;
			}
			else
			{
				icon = ICON_FK_FILE;
			}

			_node->setText(icon + " " + fs_path.filename().generic_string());
		}
		else
		{
			_node->setText(_path);
		}

		_node->setStringTag(0, fs_path.generic_string());

		if (rootNode != nullptr)
			rootNode->addControl(_node);
		else
			treeView->addControl(_node);

		if (fs::is_directory(_path))
		{
			Core::List<fs::path> fs = getPathEntries(_path);

			if (addFiles)
			{
				_node->setAlwaysShowOpenArrow(fs.count() > 0);
			}
			else
			{
				bool hasDir = false;
				for (const auto& entry : fs)
				{
					if (fs::is_directory(entry))
					{
						hasDir = true;
						break;
					}
				}
				_node->setAlwaysShowOpenArrow(hasDir);
			}

			if (lazyLoad)
			{
				_node->setOnOpen([_path, treeView, _node, addFiles, fs, lazyLoad](bool opened) {
					if (opened)
					{
						for (const auto& entry : fs)
						{
							Core::String path = entry.generic_string();
							if (Core::Path::isHiddenOrSystem(path)) continue;
							if (!addFiles && !fs::is_directory(entry)) continue;

							fsToTreeView(path, treeView, _node, addFiles, true, lazyLoad);
						}
					}
					else
					{
						_node->clear();
					}
				});
			}
			else
			{
				for (const auto& entry : fs)
				{
					Core::String path = entry.generic_string();
					if (Core::Path::isHiddenOrSystem(path)) continue;
					if (!addFiles && !fs::is_directory(entry)) continue;

					fsToTreeView(path, treeView, _node, addFiles, true, lazyLoad);
				}
			}
		}
	}
} // namespace Editor