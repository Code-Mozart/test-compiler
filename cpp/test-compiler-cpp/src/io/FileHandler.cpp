#include "FileHandler.h"

#include <fstream>
#include "util/Util.h"

namespace FileHandler {

    FileInfo Read(const string& path)
    {
        std::ifstream ifs(path);
        if (ifs.is_open()) {
            string content = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
            size_t count = std::count(content.begin(), content.end(), '\n') + 1;
            return FileInfo{ path, content, count };
        } else {
            throw FileAccessException(path);
        }
    }

    void Write(const string& content, const string& path)
    {
        std::ofstream ofs(path);
        if (ofs.is_open()) {
            ofs << content;
        }
        else {
            throw FileAccessException(path);
        }
    }

    string RemoveExtension(const string& filename)
    {
        if (filename == "." || filename == "..")
            return filename;

        size_t pos = filename.find_last_of(".");
        if (pos != std::string::npos)
            return filename.substr(0, pos);

        return filename;
    }

}
