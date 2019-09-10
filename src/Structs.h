#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using std::string;

struct CollisionBox{

	string text;
	int top;
	int bottom;
	int left;
	int right;
	bool is_folder;

};

#endif