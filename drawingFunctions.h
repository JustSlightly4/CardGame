/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Function/Classes for drawing to the screen are written here
 */
#ifndef DRAWINGFUNCTIONSH
#define DRAWINGFUNCTIONSH
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <iomanip>
#include "functions.h"
#include "raylib.h"
#include "deckofcards.h"
#include "cards.h"
#include "buttons.h"
#include "globals.h"

//Drawing Helper Functions
void DrawGrid(Data &StyleGuide);
void DrawGridDots(Data &StyleGuide);
void DrawFPSOnGrid(Data &StyleGuide);
Rectangle CoordsToRec(Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);

//Simple Drawing Functions
void DrawTextureOnGrid(Texture2D &texture, Rectangle source, Vector2 startCoords, Vector2 endCoords, Color tint, Data &StyleGuide);
void DrawRectangleOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, Data &StyleGuide);
void DrawRectangleLinesOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, int lineThickness, Data &StyleGuide);
void DrawStarOnGrid(Vector2 coords, Data &StyleGuide);

//Complex Drawing Functions
void DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
void DrawTextSOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, Data &StyleGuide, int lineThickness = 0);
float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
float DrawTextSWrappedOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, Data &StyleGuide, int lineThickness = 0);
void DrawCardOnGrid(Deck &deck, int index, Vector2 startCoords, Vector2 endCoords, bool showStats, Data &StyleGuide);
void DrawCardButtonOnGrid(Deck &deck, SingleButtonGroup &buttons, int cardIndex, int buttonIndex, Vector2 startCoords, Vector2 endCoords, bool showStats, Data &StyleGuide);
void DrawButtonOnGrid(SingleButtonGroup &buttons, int index, Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);
void DrawButtonOnGrid(PlusMinusButtonGroup &buttons, int index, string value, Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);
void DrawButtonRowOnGrid(SingleButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);
void DrawCardRowOnGrid(Deck &deck, int cardWidthSegments, Vector2 startCoords, Vector2 endCoords, bool showStats, Data &StyleGuide);
void DrawCardButtonRowOnGrid(Deck &deck, SingleButtonGroup &buttons, int spacing, Vector2 startCoords, Vector2 endCoords, bool showStats, Data &StyleGuide);

#endif
