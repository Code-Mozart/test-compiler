#include "file_reader.h"
#include <fstream>

namespace testc {

	File_Reader_Result read_from_file(const Ref<const File> file)
	{
		std::ifstream ifs((FILE*)file);
        if (ifs.is_open()) {
            string content = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
            size_t count = std::count(content.begin(), content.end(), '\n') + 1;
            return File_Reader_Result{
				Source_Info{ file, count, String_View{ content.data(), content.length() } },
				std::move(content)
			};
        } else {
            throw File_Access_Exception(file);
		}
	}

}
