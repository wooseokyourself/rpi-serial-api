#ifndef __COMMON__
#define __COMMON__

#include <unistd.h>
#include <string>
#include <fstream>
#include <iterator>
#include <cstring>
#include <vector>
#include <iostream>

void extractFileBytes (std::string& outBytes, const std::string filePath);

#endif