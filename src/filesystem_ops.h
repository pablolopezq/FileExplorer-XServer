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
string getFileName(const string& s);