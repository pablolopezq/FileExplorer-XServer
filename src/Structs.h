#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using std::string;

enum class ButtonType{
	FOLDER_NAV,
	CREATE_FOLDER,
	DELETE_FOLDER,
	CREATE_FILE,
	DELETE_FILE,
	BACK
};

enum class Entry_Type{
	FOLDER,
	FILE
};

struct CollisionBox{

	string text;
	int top;
	int bottom;
	int left;
	int right;
	ButtonType type;

	bool check_collision(int x, int y){
		if(x >= left && x <= right && y >= top && y <= bottom){
			return true;
		} 
		return false;
	}

};

struct Entry{

	string full_path;
	string name;
	Entry_Type type;
	CollisionBox boundaries;
};

#endif