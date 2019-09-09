#ifndef FILESYSTEM_OPS_H
#define FILESYSTEM_OPS_H value

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

void get_directories(string full_path, tree<string> &tr);
string get_file_name(const string& s);

#endif