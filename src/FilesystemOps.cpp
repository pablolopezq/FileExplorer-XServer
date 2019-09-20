#include <vector>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <fcntl.h>
#include <errno.h>

#include "FilesystemOps.h"

using std::vector;
using std::cout;
using std::endl;

int cp(const char *to, const char *from){
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_from = open(from, O_RDONLY);
    if (fd_from < 0)
        return -1;

    fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;

    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);

        /* Success! */
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);

    errno = saved_errno;
    return -1;
}

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
    std::string path;

    dir = opendir(full_path.c_str());
    if (dir == NULL) {
        perror("Error opendir()");
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {

            path = full_path + "/" + entry->d_name;

            if (entry->d_type == DT_DIR) {
                delete_folder(path);
            }

            /*
             * Here, the actual deletion must be done.  Beacuse this is
             * quite a dangerous thing to do, and this program is not very
             * well tested, we are just printing as if we are deleting.
             */
            // printf("(not really) Deleting: %s\n", path);
            cout << "(not really) Deleting " << path << endl;
            remove(path.c_str());
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
    cout << "(not really) Deleting " << full_path << endl;
    
    if(remove(full_path.c_str()) != 0){
      cout << "Folder deletion failed\n";
    }
    else{
      cout << "Folder deleted successfully\n";
    }

}

void FileOps::create_folder(string full_path){
	if (mkdir(full_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else
        cout << "Directory created" << endl; 
}

void FileOps::create_file(string full_path){
  if (creat(full_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else
        cout << "File created" << endl; 
}

void FileOps::create_symlink(string src, string dest){
  if (symlink(src.c_str(),dest.c_str()) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else
        cout << "SymLink created" << endl; 
}

void FileOps::create_hardlink(string src, string dest){
  if (link(src.c_str(),dest.c_str()) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else
        cout << "Link created" << endl; 
}

void FileOps::delete_link(string full_path){
  if (unlink(full_path.c_str()) == -1) 
        cout << "Error :  " << strerror(errno) << endl;
    else
        cout << "SymLink Deleted" << endl; 
}

void FileOps::copy_file(string src, string dest){
  if (cp(src.c_str(), dest.c_str()) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else
        cout << "File Deleted" << endl; 
}

void FileOps::move_file(string src, string dest){
  if (cp(src.c_str(), dest.c_str()) == -1) 
        cout << "Error :  " << strerror(errno) << endl; 
    else{
        cout << "File Deleted" << endl; 
        delete_file(src);
    }
}

void FileOps::copy_dir(string src, string dest){
    string command = "cp " + src + " " + dest;
    system(command.c_str());
}

void FileOps::move_dir(string src, string dest){
    string command = "cp " + src + " " + dest;
    system(command.c_str());
    delete_folder(src);
}