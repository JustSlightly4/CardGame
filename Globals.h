/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Global Variables/Structs/Class are written here
 */
 
#ifndef GLOBALSH
#define GLOBALSH
#include <iostream>
#include <string>
#include <vector>
#include "raylib.h"
#include "Card.h"
#include "Deck.h"

using namespace std;

Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Rectangle operator*(const Rectangle& lhs, const int& rhs);
Rectangle operator*(const Rectangle& lhs, const float& rhs);
Vector2 operator*(const Vector2& lhs, const int& rhs);
Vector2 operator*(const Vector2& lhs, const float& rhs);
bool operator==(const Vector2& lhs, const Vector2& rhs);
bool operator!=(const Vector2& lhs, const Vector2& rhs);

extern string gamerules;
extern string skills;

#endif
