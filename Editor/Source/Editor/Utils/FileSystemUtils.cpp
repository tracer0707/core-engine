#include "FileSystemUtils.h"

#include <filesystem>
#include <iostream>

#include <Core/Shared/Path.h>

#include "../Controls/TreeView.h"
#include "../Controls/TreeNode.h"

namespace Editor
{
	void FileSystemUtils::fsToTreeView(Core::String path, TreeView* treeView, TreeNode* rootNode, bool addFiles)
	{
        std::string _path = path.std_str();
        auto fs_path = std::filesystem::path(path.std_str());

        TreeNode* _node = treeView->createNode();

        if (rootNode != nullptr)
        {
            _node->setText(fs_path.filename().generic_string());
            _node->setStringTag(0, fs_path.generic_string());
        }
        else
        {
            _node->setText(_path);
            _node->setStringTag(0, path.replace('\\', '/'));
        }

        if (rootNode != nullptr)
            rootNode->addControl(_node);
        else
            treeView->addControl(_node);

        if (std::filesystem::is_directory(_path))
        {
            _node->setAlwaysShowOpenArrow(true);
            _node->setOnOpen([_path, treeView, _node, addFiles](bool opened)
            {
                if (opened)
                {
                    try
                    {
                        Core::List<std::filesystem::path> fs;
                        for (const auto& entry : std::filesystem::directory_iterator(_path, std::filesystem::directory_options::skip_permission_denied))
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

                        for (const auto& entry : fs)
                        {
                            Core::String path = entry.generic_string();
                            if (Core::Path::isHiddenOrSystem(path)) continue;
                            if (!addFiles && !std::filesystem::is_directory(entry)) continue;

                            fsToTreeView(path, treeView, _node, addFiles);
                        }
                    }
                    catch (const std::filesystem::filesystem_error& e)
                    {
                        std::cerr << "Error accessing " << _path << ": " << e.what() << std::endl;
                    }
                }
                else
                {
                    _node->clear();
                }
            });
        }
	}
}