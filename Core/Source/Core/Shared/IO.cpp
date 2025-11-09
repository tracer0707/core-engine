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

        if (!in.is_open())
        {
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
} // namespace Core