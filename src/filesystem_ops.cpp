#include <vector>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "filesystem_ops.h"

using namespace std;

string getFileName(const string& s) {

   char sep = '/';

   size_t i = s.rfind(sep, s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}


void get_directories(string full_path, tree<string> &tr){

	vector<char *> dirs;

	tree<string>::iterator root = tr.begin();
	tree<string>::iterator loc;

	//tr.append_child(root, full_path);

	loc = find(tr.begin(), tr.end(), getFileName(full_path));
	
	DIR *dp;
  struct dirent *dirp;

	if((dp = opendir(full_path.c_str())) == NULL) {
        cout << "Error opening " << full_path << endl;
    }

    while ((dirp = readdir(dp))) {
        string str(dirp->d_name);
        
        if(dirp->d_type == 4){
        	if(str == ".." || str == "." || str[0] == '.')
        		continue;

        	// cout << dirp->d_name << endl;
        	// cout << "DIR\n";
        	//dirs.push_back(dirp->d_name);
        	
        	tr.append_child(loc, str);
        	get_directories(full_path + "/" + str, tr);
        }
    }

    closedir(dp);
}