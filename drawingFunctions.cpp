/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Function/Classes for drawing to the screen are written here
 */
 
#include "drawingFunctions.h"

//Draws grid across screen for debugging purposes
void DrawGrid() {
	//void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color); 
	//Split screen into 32 by 32 squares
	Vector2 linePos = {0, 0};
	for (int i = 0; i < styleGuide.segments + 1; ++i) {
		DrawLineEx({linePos.x, 0}, {linePos.x, styleGuide.screenDimensions.y}, 1, BLACK);
		DrawLineEx({0, linePos.y}, {styleGuide.screenDimensions.x, linePos.y}, 1, BLACK);
		linePos.x = i * styleGuide.widthSegment;
		linePos.y = i * styleGuide.heightSegment;
	}
}

//Draws dots across screen for debugging purposes
void DrawGridDots() {
	//void DrawCircleV(Vector2 center, float radius, Color color)
	for (int i = 0; i < styleGuide.segments + 1; ++i) {
		for (int j = 0; j < styleGuide.segments + 1; ++j) {
			DrawCircleV({i * styleGuide.widthSegment, j * styleGuide.heightSegment}, 3, RED);
		}
	}
}

//Draws the FPS on the top left of the screen
void DrawFPSOnGrid() {
	string FPS = "FPS: " + to_string(GetFPS());
	DrawTextSOnGrid(FPS, {0, 0}, {3, 1}, {LEFTX, UPY});
}

inline Rectangle CoordsToRec(Vector2 startCoords, Vector2 endCoords) {
	return (Rectangle){startCoords.x * styleGuide.widthSegment, startCoords.y * styleGuide.heightSegment, 
		(endCoords.x - startCoords.x) * styleGuide.widthSegment, 
		(endCoords.y - startCoords.y) * styleGuide.heightSegment};
}

//Draws a texture on a grid
void DrawTextureOnGrid(Texture2D &texture, Rectangle source, Vector2 startCoords, Vector2 endCoords, Color tint) {
	DrawTexturePro(texture, source, 
	CoordsToRec(startCoords, endCoords), 
	styleGuide.origin, 0.0f, tint);
}

//Draws a rectangle on a grid
void DrawRectangleOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint) {
	DrawRectangleRec(CoordsToRec(startCoords, endCoords), tint);
}

//Draws Rectangle Lines on a grid
void DrawRectangleLinesOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, int lineThickness) {
	DrawRectangleLinesEx(CoordsToRec(startCoords, endCoords), 
		lineThickness, tint);
}

//Draws a rotating star on the grid
void DrawStarOnGrid(Vector2 coords) {
	DrawPolyLinesEx({coords.x * styleGuide.widthSegment, coords.y * styleGuide.heightSegment}, 4, styleGuide.starRadius, styleGuide.starRotation, styleGuide.starLineThickness, GOLD);
}

//-------------------------

//Draw Test Super
void DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness) {
	
	//Draw Test
	if (orientation.x == CENTERX) { //Centered
		dest.x = (dest.x + (dest.width/2)) - (((MeasureTextEx(styleGuide.currentFont->font, text.c_str(), fontSize, 1.0f)).x)/2);
	} else if (orientation.x == RIGHTX) { //Right
		dest.x = dest.x + dest.width - ((MeasureTextEx(styleGuide.currentFont->font, text.c_str(), fontSize, 1.0f)).x) - lineThickness;
	} else { //Left
		dest.x = dest.x + lineThickness;
	}
	
	if (orientation.y == CENTERY) { //Centered
		dest.y = (dest.y + (dest.height/2)) - (((MeasureTextEx(styleGuide.currentFont->font, text.c_str(), fontSize, 1.0f)).y)/2);
	} else if (orientation.y == DOWNY) { //Down
		dest.y = (dest.y + (dest.height)) - ((MeasureTextEx(styleGuide.currentFont->font, text.c_str(), fontSize, 1.0f)).y) - lineThickness;
	} else { //UP
		dest.y = dest.y + lineThickness;
	}
	
	DrawTextEx(styleGuide.currentFont->font, text.c_str(), (Vector2){dest.x, dest.y}, fontSize, 1.0f, tint); // Draw text using font and additional parameters
}

//DrawTextS but on a grid
void DrawTextSOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness) {
	DrawTextS(text, CoordsToRec(startCoords, endCoords), 
		styleGuide.textColor, styleGuide.currentFont->fontSize, orientation, lineThickness);
}

//Draw Text Super Wrapped on the x-axis
float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness) {

	//Splits text up into its individual words
    vector<string> words;
    stringstream ss(text);
    string tempWord;
	
	while (ss.peek() == '\n') { //Captures \n characters at the beginning of the text
        ss.get();              // consume this newline
        words.push_back("\n"); // record it
    }
    while (ss >> tempWord) {
        words.push_back(tempWord);
        while (ss.peek() == '\n') { //Captures \n characters after current word
			ss.get();              // consume this newline
			words.push_back("\n"); // record it
		}
    }
    
    //Decides which words need to be on a new line and indexes them
    queue<int> newLineQueue;
    float sum = 0;
    float wordWidth = 0;
    /*Add the width of each word in words until it is greater than the dest width
     * then push the index of the word that went over onto the queue to be
     * used later.
     */
    for (int i = 0; i < words.size(); ++i) {
		if (words[i] != "\n") {
			wordWidth = MeasureTextEx(styleGuide.currentFont->font, (words[i] + " ").c_str(), fontSize, 1.0f).x;
			sum += wordWidth;
			if (sum >= dest.width && sum > wordWidth) {
				newLineQueue.push(i); //Marking this word as needing to start on a newline
				sum = MeasureTextEx(styleGuide.currentFont->font, (words[i]).c_str(), fontSize, 1.0f).x;
			}
		} else {
			words[i] = "";
			newLineQueue.push(i);
			sum = 0;
		}
	}
	
	//Pushes words.size() to the end of the queue so the last line isn't forgotten
	newLineQueue.push(words.size());
	
	/*The queue now marks where the new lines should begin
	 * so this sections combines the words into the lines
	 */
	/*Now newLineQueue.size() == number of lines needed*/
	int totalNumLines = newLineQueue.size();
	vector<string> lines;
	int j = 0;
	for (int i = 0; i < totalNumLines; ++i) {
		lines.push_back("");
		while (j < newLineQueue.front()) {
			if (!lines[i].empty()) lines[i] += " ";
			lines[i] += words[j];
			++j;
		}
		newLineQueue.pop();
	}
	
	//Finally this section puts the words to the screen
	float lineHeight = MeasureTextEx(styleGuide.currentFont->font, "Ay", fontSize, 1.0f).y;
	float blockHeight = lines.size() * lineHeight;

	// Align the entire block vertically once
	if (orientation.y == CENTERY) { //CENTERY
		dest.y = (dest.y + dest.height/2) - blockHeight/2 - lineThickness; //LineThickness will need to be subtracted to counter DrawTextS() UPY
	} else if (orientation.y == DOWNY) { //DOWNY
		dest.y = (dest.y + dest.height) - blockHeight - (lineThickness * 2); ////LineThickness will need to be subtracted twice to counter DrawTextS() UPY
	} else { // UPY
		dest.y = dest.y; //Do not apply lineThickness it will be applied later in DrawTextS() using UPY
	}
	for (int i = 0; i < lines.size(); ++i) {
		DrawTextS(lines[i], dest, tint, fontSize, (Alignment){orientation.x, UPY}, lineThickness);
		dest.y += MeasureTextEx(styleGuide.currentFont->font, "Ay", fontSize, 1.0f).y;
	}
	
	return blockHeight;

}

//DrawTextSWrapped but on a grid
float DrawTextSWrappedOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness) {
	return DrawTextSWrapped(text, {startCoords.x * styleGuide.widthSegment, startCoords.y * styleGuide.heightSegment, 
		(endCoords.x - startCoords.x) * styleGuide.widthSegment, 
		(endCoords.y - startCoords.y) * styleGuide.heightSegment}, 
		styleGuide.textColor, styleGuide.currentFont->fontSize, orientation, lineThickness);
}

//Draws a single Card on the grid
void DrawCardOnGrid(Deck &deck, int index, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	DrawTexturePro(*deck.GetTexture(), 
	GetCardSourceRec(deck[index]),
	CoordsToRec(startCoords, endCoords),
	styleGuide.origin, 0.0f, deck[index]->GetColorRaylib());
	
	if (showStats) {
		DrawTextSOnGrid(deck[index]->GetName(), {startCoords.x, endCoords.y}, {endCoords.x, endCoords.y + 2}, {CENTERX, CENTERY});
		DrawTextSOnGrid("P: " + to_string(deck[index]->GetPower()) + ", M: " + to_string(deck[index]->GetMagicalPower()), {startCoords.x, endCoords.y + 2}, {endCoords.x, endCoords.y + 4}, {CENTERX, CENTERY});
		DrawTextSOnGrid("Health: " + to_string(deck[index]->GetHealth()) + "/" + to_string(deck[index]->GetHealthT()), {startCoords.x, endCoords.y + 4}, {endCoords.x, endCoords.y + 6}, {CENTERX, CENTERY});
	}
}

//Draws a single Card button on the grid
void DrawCardButtonOnGrid(Deck &deck, SingleButtonGroup &buttons, int cardIndex, int buttonIndex, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	DrawCardOnGrid(deck, cardIndex, startCoords, endCoords, showStats);
	DrawButtonOnGrid(buttons, buttonIndex, startCoords, endCoords);
}

//Draws a single button on the grid
void DrawButtonOnGrid(SingleButtonGroup &buttons, int index, Vector2 startCoords, Vector2 endCoords) {
	Rectangle buttonDest = CoordsToRec(startCoords, endCoords);
	buttons[index].SetBounds(buttonDest);
	DrawTextureOnGrid(*buttons.GetTexture(), styleGuide.buttonSource, startCoords, endCoords, WHITE);
	switch(buttons[index].GetState()) {
		case 1://Hovered GRAY (Color){ 130, 130, 130, 100 }
			DrawRectangleRec(buttonDest, (Color){ 130, 130, 130, 100 });
			break;
		case 2: //Clicked DARKGRAY (Color){ 80, 80, 80, 100 }
			DrawRectangleRec(buttonDest, (Color){ 80, 80, 80, 100 });
			break;
		default: //Neither hover nor clicked
			break;
	}
	DrawTextSWrappedOnGrid(buttons[index].GetLabel(), startCoords, endCoords, (Alignment){CENTERX, CENTERY});
}

//Draws a PlusMinus button on the grid
void DrawButtonOnGrid(PlusMinusButtonGroup &buttons, int index, string value, Vector2 startCoords, Vector2 endCoords) {
	//Calculations for a single buttons width and all three buttons destinations on screen
	float singleButtonWidth = (endCoords.x - startCoords.x)/3;
	vector<Rectangle> buttonDest(3);
	buttonDest[0] = CoordsToRec(startCoords, {startCoords.x + singleButtonWidth, endCoords.y});
	buttonDest[1] = CoordsToRec({startCoords.x + singleButtonWidth, startCoords.y}, {startCoords.x + (singleButtonWidth * 2), endCoords.y});
	buttonDest[2] = CoordsToRec({startCoords.x + (singleButtonWidth * 2), startCoords.y}, {startCoords.x + (singleButtonWidth * 3), endCoords.y});
	
	//Setting the bounds on button 1 and 2
	buttons[index].SetBounds(0, buttonDest[0]);
	buttons[index].SetBounds(1, buttonDest[2]);
	
	//Drawing all three buttons
	DrawTexturePro(*buttons.GetTexture(), styleGuide.buttonSource, buttonDest[0], styleGuide.origin, 0.0f, WHITE);
	DrawTexturePro(*buttons.GetTexture(), styleGuide.buttonSource, buttonDest[1], styleGuide.origin, 0.0f, WHITE);
	DrawTexturePro(*buttons.GetTexture(), styleGuide.buttonSource, buttonDest[2], styleGuide.origin, 0.0f, WHITE);
	
	//Draws grey rectangle on the left most button if needed
	switch(buttons[index].GetState(0)) {
		case 1://Hovered GRAY (Color){ 130, 130, 130, 100 }
			DrawRectangleRec(buttonDest[0], (Color){ 130, 130, 130, 100 });
			break;
		case 2: //Clicked DARKGRAY (Color){ 80, 80, 80, 100 }
			DrawRectangleRec(buttonDest[0], (Color){ 80, 80, 80, 100 });
			break;
		default: //Neither hover nor clicked
			break;
	}
	
	//Draws grey rectangle on the right most button if needed
	switch(buttons[index].GetState(1)) {
		case 1://Hovered GRAY (Color){ 130, 130, 130, 100 }
			DrawRectangleRec(buttonDest[2], (Color){ 130, 130, 130, 100 });
			break;
		case 2: //Clicked DARKGRAY (Color){ 80, 80, 80, 100 }
			DrawRectangleRec(buttonDest[2], (Color){ 80, 80, 80, 100 });
			break;
		default: //Neither hover nor clicked
			break;
	}
	
	//The title above all three buttons
	DrawTextSOnGrid(buttons[index].GetTitle(), {startCoords.x, startCoords.y - 2}, {endCoords.x, startCoords.y}, (Alignment){CENTERX, CENTERY});
	
	//All three buttons values
	DrawTextSWrapped(string(1, buttons[index].GetSymbolLabel(0)), buttonDest[0], styleGuide.textColor, styleGuide.currentFont->fontSize, (Alignment){CENTERX, CENTERY});
	DrawTextSWrapped(value, buttonDest[1], styleGuide.textColor, styleGuide.currentFont->fontSize, (Alignment){CENTERX, CENTERY});
	DrawTextSWrapped(string(1, buttons[index].GetSymbolLabel(1)), buttonDest[2], styleGuide.textColor, styleGuide.currentFont->fontSize, (Alignment){CENTERX, CENTERY});
}

//Draws a horizontal row of buttons on the grid
void DrawButtonRowOnGrid(SingleButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords) {
	float buttonWidth = (endCoords.x - startCoords.x)/buttons.GetSize();
	for (int i = 0; i < buttons.GetSize(); ++i) {
		DrawButtonOnGrid(buttons, i, 
		{startCoords.x + (i * buttonWidth), startCoords.y}, 
		{startCoords.x + (i * buttonWidth) + buttonWidth, endCoords.y});
	}
}

void DrawButtonColumnOnGrid(PlusMinusButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords) {
	float buttonHeight = (endCoords.y - startCoords.y - (buttons.GetSize() * 3))/buttons.GetSize();
	for (int i = 0; i < buttons.GetSize(); ++i) {
		DrawButtonOnGrid(buttons, i, buttons[i].GetLabel(), 
		{startCoords.x, startCoords.y + (i * buttonHeight) + (i * 3)}, 
		{endCoords.x, startCoords.y + (i * buttonHeight) + buttonHeight + (i * 3)});
	}
}

//Draws the whole Deck of cards in a row
void DrawCardRowOnGrid(Deck &deck, int spacing, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	float cardWidth = ((endCoords.x - startCoords.x) - (spacing * (deck.size() - 1)))/deck.size();
	float xValue;
	for (int i = 0; i < deck.size(); ++i) {
		xValue = startCoords.x + (i * cardWidth) + (i * spacing);
		DrawCardOnGrid(deck, i, {xValue, startCoords.y}, {xValue + cardWidth, endCoords.y}, showStats);
	}
}

//Draws the whole Deck of cards in a row
void DrawCardButtonRowOnGrid(Deck &deck, SingleButtonGroup &buttons, int cardWidthSegments, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	float cardWidth = cardWidthSegments;
	float spacing = ((endCoords.x - startCoords.x) - (cardWidth * deck.size()))/(deck.size() - 1);
	float xValue;
	for (int i = 0; i < deck.size(); ++i) {
		xValue = startCoords.x + (i * cardWidth) + (i * spacing);
		DrawCardButtonOnGrid(deck, buttons, i, i, {xValue, startCoords.y}, {xValue + cardWidth, endCoords.y}, showStats);
	}
}
