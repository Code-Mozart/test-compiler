#pragma once

#include "../src/basic/types.h"
#include "../src/basic/macros.h"
#include "../src/basic/exception.h"
#include "../src/common/location.h"
#include "../src/source_info.h"
#include "../src/file_handler.h"
#include "../src/common/compiler_message.h"
#include "../src/syntax/token.h"
#include "../src/syntax/lexer.h"
#include "../src/syntax/ast.h"
#include "../src/syntax/parser.h"
#include "../src/semantic/keyword.h"
#include "../src/semantic/operator.h"
#include "../src/semantic/symbol_table.h"
#include "../src/generator/instruction.h"
#include "../src/generator/generator.h"
#include "../src/generator/instruction_writer.h"
