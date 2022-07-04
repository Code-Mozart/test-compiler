#pragma once

#include "basic/include.h"
#include "common/location.h"

namespace testc {
    
    struct Source_Info {
        const Ref<const File> filepath = No_File;
        const size_t lines = 0;
        const String_View source_string = String_View::Empty;
    };

    string to_string(const Source_Info& info);

}
