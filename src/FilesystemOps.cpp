#include <vector>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fcntl.h>

#include "FilesystemOps.h"

using std::vector;
using std::cout;
using std::endl;

string FileOps::get_file_name(const string& s) {

   char sep = '/';

   size_t i = s.rfind(sep, s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}


void FileOps::get_entries(string full_path, std::vector<Entry> &entries){

	// tree<string>::iterator root = tr.begin();
	// tree<string>::iterator loc;

	//tr.append_child(root, full_path);

	// loc = find(tr.begin(), tr.end(), get_file_name(full_path));
	
	DIR *dp;
  struct dirent *dirp;

	if((dp = opendir(full_path.c_str())) == NULL) {
      cout << "Error opening " << full_path << endl;
  }
  cout << "Opened " << full_path << endl;

    while ((dirp = readdir(dp))) {
        string name(dirp->d_name);
        string full_name(full_path + "/" + name);
        
        if(dirp->d_type == 4 || dirp->d_type == 8){
        	if(name == ".." || name == "." || name[0] == '.')
        		continue;

        Entry entry;
        entry.name = name;
        entry.full_path = full_name;
        entry.type = dirp->d_type == 4 ? Entry_Type::FOLDER : Entry_Type::FILE;

        entries.push_back(entry);

        cout << "Pushed " << entry.name << "into vector\n";

        	//dirs.push_back(dirp->d_name);
        	
        	// tr.append_child(loc, full_name);
          // if(dirp->d_type == 4)
          //   get_directories(full_path + "/" + name1, tr);
        }
    }

    cout << "Folder vector size is " << entries.size() << endl;

    closedir(dp);
}

void FileOps::delete_file(string full_path){
  
  if(remove(full_path.c_str()) != 0){
    cout << "File deletion failed\n";
  }
  else{
    cout << "File deleted successfully\n";
  }
}

void FileOps::delete_folder(string full_path){

    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
    }
    dir = opendir(full_path.c_str());
    if (dir == NULL) {
        perror("Error opendir()");
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", full_path, entry->d_name);
            if (entry->d_type == DT_DIR) {
                delete_folder(path);
            }

            /*
             * Here, the actual deletion must be done.  Beacuse this is
             * quite a dangerous thing to do, and this program is not very
             * well tested, we are just printing as if we are deleting.
             */
            printf("(not really) Deleting: %s\n", path);
            /*
             * When you are finished testing this and feel you are ready to do the real
             * deleting, use this: remove*STUB*(path);
             * (see "man 3 remove")
             * Please note that I DONT TAKE RESPONSIBILITY for data you delete with this!
             */
        }

    }
    closedir(dir);

    /*
     * Now the directory is emtpy, finally delete the directory itself. (Just
     * printing here, see above) 
     */
    printf("(not really) Deleting: %s\n", full_path);

}

void FileOps::create_folder(string full_path){
	if (mkdir(full_path.c_str(), 775) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else
        cout << "Directory created" << endl; 
}

void FileOps::create_file(string full_path){
  if (creat(full_path.c_str(), 775) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else
        cout << "Directory created" << endl; 
}