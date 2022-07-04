#pragma once

#include "basic/include.h"
#include "common/location.h"

namespace testc {

    // @later: introduce a large enum with all errors/warnings and resolve them
    // in an as_string(...) function

    enum class Severity {
        Info, Warning, Error
    };

    // An compiler output message. If compilation fails a appropriate compiler
    // message is generated that describes the error and has a pointer to the
    // location in the source code where the error occured.
    struct Compiler_Message {
        Severity severity;
        Location location;
        string text;
    };

    const string& as_string(Severity sev);
    const string& to_string(Severity sev);
    string as_string(const Compiler_Message& msg);

}
