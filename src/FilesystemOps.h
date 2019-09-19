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
    void create_symlink(string src, string dest);
    void create_hardlink(string src, string dest);
    void delete_link(string full_path);
    void copy_file(string src, string dest);
    void move_file(string src, string dest);
    void copy_dir(string src, string dest);
    void move_dir(string src, string dest);
    string get_file_name(const string& s);

}

#endif