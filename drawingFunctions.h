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
void DrawGrid();
void DrawGridDots();
void DrawFPSOnGrid();
Rectangle CoordsToRec(Vector2 startCoords, Vector2 endCoords);

//Simple Drawing Functions
void DrawTextureOnGrid(Texture2D &texture, Rectangle source, Vector2 startCoords, Vector2 endCoords, Color tint);
void DrawRectangleOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint);
void DrawRectangleLinesOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, int lineThickness);
void DrawStarOnGrid(Vector2 coords);

//Complex Drawing Functions
void DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
void DrawTextSOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness = 0);
float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
float DrawTextSWrappedOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness = 0);
void DrawCardOnGrid(Deck &deck, int index, Vector2 startCoords, Vector2 endCoords, bool showStats);
void DrawCardButtonOnGrid(Deck &deck, SingleButtonGroup &buttons, int cardIndex, int buttonIndex, Vector2 startCoords, Vector2 endCoords, bool showStats);
void DrawButtonOnGrid(SingleButtonGroup &buttons, int index, Vector2 startCoords, Vector2 endCoords);
void DrawButtonOnGrid(PlusMinusButtonGroup &buttons, int index, string value, Vector2 startCoords, Vector2 endCoords);
void DrawButtonRowOnGrid(SingleButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords);
void DrawButtonColumnOnGrid(PlusMinusButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords);
void DrawCardRowOnGrid(Deck &deck, int cardWidthSegments, Vector2 startCoords, Vector2 endCoords, bool showStats);
void DrawCardButtonRowOnGrid(Deck &deck, SingleButtonGroup &buttons, int spacing, Vector2 startCoords, Vector2 endCoords, bool showStats);

#endif
