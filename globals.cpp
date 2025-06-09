/*
 * Eric Ryan Montgomery
 * 03/17/2025
 * For CardGameUI
 * Functions for deck/cards without the use of raylib are written here
 */
#include "globals.h"

//List of colors and attributes
vector<colors> cols { //Colors are worth 0-5 points
	C_WHITE, //0
	C_GREEN, //1
	C_BLUE, //2
	C_YELLOW, //3
	C_RED, //4
	C_ORANGE, //5
};
vector<attributes> atts { //atts are worth 0-5 points
	C_MIMIC, //0
	C_DEX, //1
	C_INT, //2
	C_STR, //3
	C_FTH, //4
	C_ARC, //5
};
vector<spells> spellList { //All spells are free for now
	FORCE,
	DRAIN,
	WEAKEN,
	HEAL,
};
vector<actions> actionsList {
	ATTACK,
	SWAP,
	CHARGE,
	FLASK,
	CASTSPELL,
};
unordered_map<colors, colors> advantage = {
	{ C_WHITE, C_YELLOW },   
	{ C_GREEN, C_RED },  
	{ C_BLUE, C_ORANGE },  
	{ C_YELLOW, C_WHITE }, 
	{ C_RED, C_GREEN },
	{ C_ORANGE, C_BLUE } 
};

Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
	return (Vector2){lhs.x + rhs.x, lhs.y + rhs.y};
}

// Free function to simulate overloading *
Rectangle operator*(const Rectangle& lhs, const int& rhs) {
    return (Rectangle){lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs};
}

// Free function to simulate overloading *
Rectangle operator*(const Rectangle& lhs, const float& rhs) {
    return (Rectangle){lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs};
}

// Free function to simulate overloading *
Vector2 operator*(const Vector2& lhs, const int& rhs) {
    return (Vector2){lhs.x * rhs, lhs.y * rhs};
}

// Free function to simulate overloading *
Vector2 operator*(const Vector2& lhs, const float& rhs) {
    return (Vector2){lhs.x * rhs, lhs.y * rhs};
}

// Free function to simulate overloading ==
bool operator==(const Vector2& lhs, const Vector2& rhs) {
    if (lhs.x == rhs.x && lhs.y == rhs.y) return true;
    return false;
}

// Free function to simulate overloading !=
bool operator!=(const Vector2& lhs, const Vector2& rhs) {
    if (lhs == rhs) return false;
    return true;
}
