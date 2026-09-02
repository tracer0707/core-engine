#include "FileSystemUtils.h"

#include <iostream>

#include <Core/Shared/Path.h>

#include "../Editor/Controls/TreeView.h"
#include "../Editor/Controls/TreeNode.h"
#include "../Shared/IconsForkAwesome.h"
#include "../Shared/Tags.h"

namespace fs = std::filesystem;

namespace Editor
{
	Core::List<fs::path> FileSystemUtils::getDiskDrives()
	{
		Core::List<fs::path> drives;

#ifdef _WIN32
		for (char drive = 'A'; drive <= 'Z'; ++drive)
		{
			fs::path drive_path = std::string(1, drive) + ":\\";
			if (fs::exists(drive_path))
			{
				drives.add(drive_path);
			}
		}
#else
		fs::path mount_path("/");
		if (fs::exists(mount_path))
		{
			drives.add(mount_path);
		}

		std::vector<fs::path> common_mounts = {fs::path("/mnt"), fs::path("/media"), fs::path("/Volumes")};
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

	Core::List<fs::path> FileSystemUtils::getPathEntries(const fs::path& path)
	{
		Core::List<fs::path> fs;
		for (const auto& entry : fs::directory_iterator(path, fs::directory_options::skip_permission_denied))
		{
			fs.add(entry.path());
		}

		fs.sort([](fs::path& a, fs::path& b) -> bool {
			bool isDirA = fs::is_directory(a);
			bool isDirB = fs::is_directory(b);

			std::string _a = Core::Path::toUtf8(a).std_str();
			std::string _b = Core::Path::toUtf8(b).std_str();

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
			if (entry.is_regular_file() && !Core::Path::isHiddenOrSystem(entry.path()))
			{
				out.add(fs::relative(entry.path(), root));
			}
		}
	}

	void FileSystemUtils::fsToTreeView(const fs::path& path, TreeView* treeView, TreeNode* rootNode, bool addFiles, bool showRootNode, bool lazyLoad)
	{
		Core::String _path = Core::Path::toUtf8(path);

		if (!showRootNode)
		{
			Core::List<fs::path> fs = getPathEntries(path);
			for (const auto& entry : fs)
			{
				if (Core::Path::isHiddenOrSystem(entry)) continue;
				if (!addFiles && !fs::is_directory(entry)) continue;

				fsToTreeView(entry, treeView, nullptr, addFiles, true, lazyLoad);
			}
			return;
		}

		auto fs_path = path;

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

			_node->setText(icon + " " + Core::Path::toUtf8(fs_path.filename()).std_str());
		}
		else
		{
			_node->setText(_path);
		}

		_node->setStringTag(TAG_FULL_PATH, Core::Path::toUtf8(fs_path));

		if (rootNode != nullptr)
			rootNode->addControl(_node);
		else
			treeView->addControl(_node);

		if (fs::is_directory(fs_path))
		{
			Core::List<fs::path> fs = getPathEntries(fs_path);

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
							if (Core::Path::isHiddenOrSystem(entry)) continue;
							if (!addFiles && !fs::is_directory(entry)) continue;

							fsToTreeView(entry, treeView, _node, addFiles, true, lazyLoad);
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
					if (Core::Path::isHiddenOrSystem(entry)) continue;
					if (!addFiles && !fs::is_directory(entry)) continue;

					fsToTreeView(entry, treeView, _node, addFiles, true, lazyLoad);
				}
			}
		}
	}
} // namespace Editor