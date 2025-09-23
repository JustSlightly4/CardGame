/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Function/Classes for drawing to the screen are written here
 */

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
#ifndef DRAWINGFUNCTIONSH
#define DRAWINGFUNCTIONSH

//New Grid Based Drawing Functions
void DrawGrid(Data &StyleGuide);
void DrawGridDots(Data &StyleGuide);
Rectangle CoordsToRec(Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);
void DrawTextureOnGrid(Texture2D &texture, Rectangle source, Vector2 startCoords, Vector2 endCoords, Color tint, Data &StyleGuide);
void DrawRectangleOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, Data &StyleGuide);
void DrawRectangleLinesOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, int lineThickness, Data &StyleGuide);
void DrawTextSOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, Data &StyleGuide, int lineThickness = 0);
void DrawTextSWrappedOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, Data &StyleGuide, int lineThickness = 0);
void DrawFPSOnGrid(Data &StyleGuide);
void DrawCardOnGrid(int index, deck &Deck, Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);
void DrawCardWithBasicStatsOnGrid(int index, deck &Deck, Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);
void DrawButtonOnGrid(SingleButtonGroup &buttons, int index, Vector2 startCoords, Vector2 endCoords, Data &StyleGuide);

//Older Absolute Pixel Drawing Functions 
void DrawBasicCardStats(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide);
void DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
void DrawButtonLine(SingleButtonGroup &buttons, Data &StyleGuide);
void DrawSettingsButtons(PlusMinusButtonGroup &buttons, Data &StyleGuide);
void DrawRules(float &scrollOffset, Data &StyleGuide);
void DrawSkills(float &scrollOffset, Data &StyleGuide);
void DrawStringVector(const vector<string> &sentences, const float &scrollOffset, Data &StyleGuide);
void DrawCardLine(deck *Deck, SingleButtonGroup &buttons, Vector2 startPos, int spacing, Data &StyleGuide);
void DrawCard(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide);
void DrawCardWithStats(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide);
void DrawCardButton(int index, deck *Deck, SingleButtonGroup &buttons, Vector2 pos, float size, Data &StyleGuide);
void DrawGame(deck *deck1, deck *deck2, GameVars &gameVars, Data &StyleGuide, Flags &flags);
void DrawButton(SingleButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide);
Rectangle DrawButton(PlusMinusButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide);
void DrawButtonTopLabel(SingleButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide);
void DrawCardEditScreen(PlusMinusButtonGroup &buttons, CardEditVars &cardEditVars, deck *Deck, deck *dummyDeck, Data &StyleGuide);

#endif
