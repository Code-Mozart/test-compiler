#pragma once

#include "util/Util.h"

struct FileInfo {
	string filename;
	string content;
	size_t lines;
	size_t offet = 0;
};
