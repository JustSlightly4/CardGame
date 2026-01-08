/*
 * Eric Ryan Montgomery
 * 11/24/2025
 * For CardGameUI
 * UIDrawer Class to replace functional functions
 */

#ifndef UIDRAWERH
#define UIDRAWERH
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <iomanip>
#include "raylib.h"
#include "Deck.h"
#include "Card.h"
#include "Buttons.h"
#include "Globals.h"
#include "StateVariables.h"

 class UIDrawer {
    public:
      //Mutable
      Vector2 screenDimensions;
      float maxScroll;
      float starRotation;
      float widthSegment;
      float heightSegment;
      float scrollOffset;

      //Font Iterator
      std::vector<NamedFont>::iterator currentFont;

      // Constant bases
      static constexpr Vector2 origin = {0, 0};
      static constexpr Rectangle cardSource = {0, 0, 100, 150};
      static constexpr Rectangle buttonSource = {0, 0, 200, 100};
      static constexpr int segments = 64;
      static constexpr Color textColor = BLACK;
      static constexpr Color background = RAYWHITE;
      static constexpr Color starColor = GOLD;
      static constexpr int starRadius = 20;
      static constexpr int starLineThickness = 7;
      static constexpr float scrollSpeed = 50.0f;

      // Drawing Constants
      static constexpr Vector2 REC_START = {1, 53};
      static constexpr Vector2 REC_END = {63, 63};
      static constexpr Color REC_COLOR = BLACK;
      static constexpr Vector2 REC_BTN_START = {2, 54};
      static constexpr Vector2 REC_BTN_END = {62, 62};

      //Alignemnt Variables
      enum AlignmentX {
         CENTERX = 0,
         RIGHTX = 1,
         LEFTX = 2,
      };

      enum AlignmentY {
         CENTERY = 0,
         DOWNY = 1,
         UPY = 2,
      };

      typedef struct ALIGNMENT {
         AlignmentX x;
         AlignmentY y;
         bool reduceTextSize = true;
      } Alignment;

      //Create/Update/Destory Functions
      UIDrawer();
      void Update();

      //Base Drawing Functions
      void DrawGrid();
      void DrawGridDots();
      void DrawFPSOnGrid();
      Rectangle CoordsToRec(Vector2 startCoords, Vector2 endCoords);
      void scrollLogic();
      void DrawTextureOnGrid(Texture2D &texture, Rectangle source, Vector2 startCoords, Vector2 endCoords, Color tint);
      void DrawRectangleOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint);
      void DrawRectangleLinesOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, int lineThickness);
      void DrawStarOnGrid(Vector2 coords);
      void DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
      void DrawTextSOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness = 0);
      float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness = 0);
      float DrawTextSWrappedOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness = 0);

      //Card/DeckofCards/Button Drawing Functions
      float GetCardSourceX(Card *card);
      Rectangle GetCardSourceRec(Card *card);
      void DrawCardOnGrid(Deck &deck, int index, Vector2 startCoords, Vector2 endCoords, bool showStats);
      void DrawCardButtonOnGrid(Deck &deck, SingleButtonGroup &buttons, int cardIndex, int buttonIndex, Vector2 startCoords, Vector2 endCoords, bool showStats);
      void DrawButtonOnGrid(SingleButtonGroup &buttons, int index, Vector2 startCoords, Vector2 endCoords);
      void DrawButtonOnGrid(PlusMinusButtonGroup &buttons, int index, string value, Vector2 startCoords, Vector2 endCoords);
      void DrawButtonOnGrid(DoubleButtonGroup &buttons, int index, Vector2 startCoords, Vector2 endCoords);
      void DrawButtonRowOnGrid(SingleButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords, int amountOfButtons = 7);
      void DrawButtonColumnOnGrid(PlusMinusButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords);
      void DrawButtonColumnOnGrid(DoubleButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords);
      void DrawCardRowOnGrid(Deck &deck, int cardWidthSegments, Vector2 startCoords, Vector2 endCoords, bool showStats);
      void DrawCardButtonRowOnGrid(Deck &deck, SingleButtonGroup &buttons, int spacing, Vector2 startCoords, Vector2 endCoords, bool showStats);

 };
 #endif