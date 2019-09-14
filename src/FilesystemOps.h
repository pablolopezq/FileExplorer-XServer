#ifndef FILESYSTEM_OPS_H
#define FILESYSTEM_OPS_H

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Structs.h"

using std::string;

namespace FileOps{
    void get_entries(string full_path, std::vector<Entry> &entries);
    void delete_file(string full_path);
    void delete_folder(string full_path);
    void create_file(string full_path);
    void create_folder(string full_path);
    string get_file_name(const string& s);
}

#endif