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

struct CollisionBox{

	string text;
	int top;
	int bottom;
	int left;
	int right;
	ButtonType type;

};

#endif