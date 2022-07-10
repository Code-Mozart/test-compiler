#pragma once

#include "basic/include.h"

namespace testc {

    struct Instruction;

    // Writes all instructions into a single string with one instruction per line.
    // This is the format expected by the Testc Virtual Machine. 
    string write_instructions(const List<Ref<Instruction>>& instructions);

}
