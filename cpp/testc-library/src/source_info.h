#pragma once

#include "util/util.h"
#include "common/location.h"

namespace testc {
    
    struct Source_Info {
        const String_View filepath = No_File;
        const size_t lines = 0;
        const String_View source_string = String_View::Empty;
    };

}
