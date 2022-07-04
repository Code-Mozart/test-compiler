#include "location.h"
#include "util/string_util.h"

namespace testc {

	string as_string(const Location& loc) {
        if (loc.filepath == No_File) {
            return "";
        }
        else if (loc.line == No_Line) {
            return loc.filepath;
        }
        else if (loc.pos == No_Pos) {
            return loc.filepath + ":" + loc.line;
        }
        else {
            return loc.filepath + ":" + loc.line + ":" + loc.pos;
        }
    }

}
