#pragma once

#include "basic/include.h"
#include "common/location.h"

namespace testc {

    enum class Operation {
        Stop,

        Push,
        Pop,
        Load,
        Store,

        Jump,
        JumpIf0,
        JumpIfNot0,

        Call,
        Return,

        Add,
        Subtract,
        Multiply,
        Divide,
        Modulo,

        Less_Than,

        Console_Out
    };

    struct Instruction {
        Location location;
        Operation operation;
        short argument;
    };

    constexpr bool has_parameter(Operation op);
    const string& as_string(Operation op);
    const string& to_string(Operation op);
    string as_string(const Instruction& instr);
    string to_string(const Instruction& instr);

}
