#pragma once
#include <string>

const std::string& GetFilename();
const std::string& GetPluginsDir();
const std::string& GetFilenameWithoutExt();

void errmsg(const std::string& msg);