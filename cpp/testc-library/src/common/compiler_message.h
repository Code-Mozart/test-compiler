#pragma once

#include "util/util.h"
#include "common/location.h"

namespace testc {

    enum class Severity {
        Info, Warning, Error
    };

    struct Compiler_Message {
        Severity severity;
        Location location;
        string text;
    };

}
