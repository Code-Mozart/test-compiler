#include "file_handler.h"
#include <fstream>

namespace testc {

	File_Read_Result read_from_file(const Ref<const File> file)
	{
		std::ifstream ifs(file->c_str());
        if (ifs.is_open()) {
            string content = string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
            size_t count = std::count(content.begin(), content.end(), '\n') + 1;
			ifs.close();
            return File_Read_Result{
				true, {},
				Source_Info{ file, count, String_View{ content.data(), content.length() } },
				std::move(content)
			};
        }
		else {
            return File_Read_Result{
				false,
				{ raise_error("Failed to read source file '" + file->string() + "'") },
				Source_Info{ file, 0, String_View{} },
				string()
			};
		}
	}
	
	File_Write_Result write_to_file(const Ref<const File> file, const string& content) {
		std::ofstream ofs(file->c_str());
		if (ofs.is_open()) {
			ofs << content;
			ofs.close();
			return File_Write_Result{ true, {} };
		}
		else {
			return File_Write_Result{
				false,
				{ raise_error("Failed to write file '" + file->string() + "'") }
			};
		}
	}

}
