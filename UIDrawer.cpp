/*
 * Eric Ryan Montgomery
 * 11/24/2025
 * For CardGameUI
 * UIDrawer Class to replace functional functions
 */
 
#include "UIDrawer.h"

UIDrawer::UIDrawer() {
	screenDimensions = {(float)GetScreenWidth(), (float)GetScreenHeight()};
	starRotation = 0.0f;
	maxScroll = 0.0f;
	scrollOffset = 0.0f;
	widthSegment = screenDimensions.x/segments;
	heightSegment = screenDimensions.y/segments;
}

void UIDrawer::Update() {
	// Update screen dimensions
	screenDimensions.x = GetScreenWidth();
	screenDimensions.y = GetScreenHeight();

	// Star rotation animation
	starRotation += 20 * GetFrameTime();
	if (starRotation > 360.0f) starRotation = 0.0f;
	
	widthSegment = screenDimensions.x/segments;
	heightSegment = screenDimensions.y/segments;

	//Update Scroll Logic
	this->scrollLogic();
}

void UIDrawer::scrollLogic() {
	if (this->maxScroll < 0) this->maxScroll = 0;
	if (IsKeyDown(KEY_DOWN) || GetMouseWheelMove() < 0) scrollOffset += this->scrollSpeed;
	if (IsKeyDown(KEY_UP) || GetMouseWheelMove() > 0) scrollOffset -= this->scrollSpeed;
	if (scrollOffset < 0) scrollOffset = 0;
	if (scrollOffset >= this->maxScroll) scrollOffset = this->maxScroll;
}

//Draws grid across screen for debugging purposes
void UIDrawer::DrawGrid() {
	//void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color); 
	//Split screen into 32 by 32 squares
	Vector2 linePos = {0, 0};
	for (int i = 0; i < this->segments + 1; ++i) {
		DrawLineEx({linePos.x, 0}, {linePos.x, this->screenDimensions.y}, 1, BLACK);
		DrawLineEx({0, linePos.y}, {this->screenDimensions.x, linePos.y}, 1, BLACK);
		linePos.x = i * this->widthSegment;
		linePos.y = i * this->heightSegment;
	}
}

//Draws dots across screen for debugging purposes
void UIDrawer::DrawGridDots() {
	//void DrawCircleV(Vector2 center, float radius, Color color)
	for (int i = 0; i < this->segments + 1; ++i) {
		for (int j = 0; j < this->segments + 1; ++j) {
			DrawCircleV({i * this->widthSegment, j * this->heightSegment}, 3, RED);
		}
	}
}

//Draws the FPS on the top left of the screen
void UIDrawer::DrawFPSOnGrid() {
	string FPS = "FPS: " + to_string(GetFPS());
	DrawTextSOnGrid(FPS, {0, 0}, {3, 1}, {LEFTX, UPY});
}

inline Rectangle UIDrawer::CoordsToRec(Vector2 startCoords, Vector2 endCoords) {
	return (Rectangle){startCoords.x * this->widthSegment, startCoords.y * this->heightSegment, 
		(endCoords.x - startCoords.x) * this->widthSegment, 
		(endCoords.y - startCoords.y) * this->heightSegment};
}

//Draws a texture on a grid
void UIDrawer::DrawTextureOnGrid(Texture2D &texture, Rectangle source, Vector2 startCoords, Vector2 endCoords, Color tint) {
	DrawTexturePro(texture, source, 
	CoordsToRec(startCoords, endCoords), 
	this->origin, 0.0f, tint);
}

//Draws a rectangle on a grid
void UIDrawer::DrawRectangleOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint) {
	DrawRectangleRec(CoordsToRec(startCoords, endCoords), tint);
}

//Draws Rectangle Lines on a grid
void UIDrawer::DrawRectangleLinesOnGrid(Vector2 startCoords, Vector2 endCoords, Color tint, int lineThickness) {
	DrawRectangleLinesEx(CoordsToRec(startCoords, endCoords), 
		lineThickness, tint);
}

//Draws a rotating star on the grid
void UIDrawer::DrawStarOnGrid(Vector2 coords) {
	DrawPolyLinesEx({coords.x * this->widthSegment, coords.y * this->heightSegment}, 4, this->starRadius, this->starRotation, this->starLineThickness, GOLD);
}

//-------------------------

//Draw Test Super
void UIDrawer::DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness) {
	
	//Draw Test
	if (orientation.x == CENTERX) { //Centered
		dest.x = (dest.x + (dest.width/2)) - (((MeasureTextEx(this->currentFont->font, text.c_str(), fontSize, 1.0f)).x)/2);
	} else if (orientation.x == RIGHTX) { //Right
		dest.x = dest.x + dest.width - ((MeasureTextEx(this->currentFont->font, text.c_str(), fontSize, 1.0f)).x) - lineThickness;
	} else { //Left
		dest.x = dest.x + lineThickness;
	}
	
	if (orientation.y == CENTERY) { //Centered
		dest.y = (dest.y + (dest.height/2)) - (((MeasureTextEx(this->currentFont->font, text.c_str(), fontSize, 1.0f)).y)/2);
	} else if (orientation.y == DOWNY) { //Down
		dest.y = (dest.y + (dest.height)) - ((MeasureTextEx(this->currentFont->font, text.c_str(), fontSize, 1.0f)).y) - lineThickness;
	} else { //UP
		dest.y = dest.y + lineThickness;
	}
	
	DrawTextEx(this->currentFont->font, text.c_str(), (Vector2){dest.x, dest.y}, fontSize, 1.0f, tint); // Draw text using font and additional parameters
}

//DrawTextS but on a grid
void UIDrawer::DrawTextSOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness) {
	DrawTextS(text, CoordsToRec(startCoords, endCoords), 
		this->textColor, this->currentFont->fontSize, orientation, lineThickness);
}

//Draw Text Super Wrapped on the x-axis
float UIDrawer::DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation, int lineThickness) {

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
			wordWidth = MeasureTextEx(this->currentFont->font, (words[i] + " ").c_str(), fontSize, 1.0f).x;
			sum += wordWidth;
			if (sum >= dest.width && sum > wordWidth) {
				newLineQueue.push(i); //Marking this word as needing to start on a newline
				sum = MeasureTextEx(this->currentFont->font, (words[i]).c_str(), fontSize, 1.0f).x;
			}
		} else if (i != words.size()-1) {
			words[i] = "";
			newLineQueue.push(i);
			sum = 0;
		} else {
			words[i] = "";
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
	float lineHeight = MeasureTextEx(this->currentFont->font, "Ay", fontSize, 1.0f).y;
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
		dest.y += MeasureTextEx(this->currentFont->font, "Ay", fontSize, 1.0f).y;
	}
	
	return blockHeight;

}

//DrawTextSWrapped but on a grid
float UIDrawer::DrawTextSWrappedOnGrid(string text, Vector2 startCoords, Vector2 endCoords, Alignment orientation, int lineThickness) {
	return DrawTextSWrapped(text, {startCoords.x * this->widthSegment, startCoords.y * this->heightSegment, 
		(endCoords.x - startCoords.x) * this->widthSegment, 
		(endCoords.y - startCoords.y) * this->heightSegment}, 
		this->textColor, this->currentFont->fontSize, orientation, lineThickness);
}

//Gets the correct Card source texture data based on its attribute
float UIDrawer::GetCardSourceX(Card *card) {
	switch(card->GetAttribute()) {
		case(Card::C_MIMIC):
			return cardSource.width * 4;
		case(Card::C_STR):
			return cardSource.width * 0;
		case(Card::C_FTH):
			return cardSource.width * 2;
		case(Card::C_DEX):
			return cardSource.width * 1;
		case(Card::C_INT):
			return cardSource.width * 3;
		case(Card::C_ARC):
			return cardSource.width * 5;
		default:
			return cardSource.width * 4;
	}
}

//Gets the correct Card source texture data based on its attribute
//and returns it as a rectangle
Rectangle UIDrawer::GetCardSourceRec(Card *card) {
	switch(card->GetAttribute()) {
		case(Card::C_MIMIC):
			return {cardSource.width * 4, 0, cardSource.width, cardSource.height};
		case(Card::C_STR):
			return {cardSource.width * 0, 0, cardSource.width, cardSource.height};
		case(Card::C_FTH):
			return {cardSource.width * 2, 0, cardSource.width, cardSource.height};
		case(Card::C_DEX):
			return {cardSource.width * 1, 0, cardSource.width, cardSource.height};
		case(Card::C_INT):
			return {cardSource.width * 3, 0, cardSource.width, cardSource.height};
		case(Card::C_ARC):
			return {cardSource.width * 5, 0, cardSource.width, cardSource.height};
		default:
			return {cardSource.width * 4, 0, cardSource.width, cardSource.height};
	}
}

//Draws a single Card on the grid
void UIDrawer::DrawCardOnGrid(Deck &deck, int index, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	DrawTexturePro(*deck.GetTexture(), 
	GetCardSourceRec(deck[index]),
	CoordsToRec(startCoords, endCoords),
	this->origin, 0.0f, deck[index]->GetColorRaylib());
	
	if (showStats) {
		DrawTextSOnGrid(deck[index]->GetName(), {startCoords.x, endCoords.y}, {endCoords.x, endCoords.y + 2}, {CENTERX, CENTERY});
		DrawTextSOnGrid("P: " + to_string(deck[index]->GetPower()) + ", M: " + to_string(deck[index]->GetMagicalPower()), {startCoords.x, endCoords.y + 2}, {endCoords.x, endCoords.y + 4}, {CENTERX, CENTERY});
		DrawTextSOnGrid("Health: " + to_string(deck[index]->GetHealth()) + "/" + to_string(deck[index]->GetHealthT()), {startCoords.x, endCoords.y + 4}, {endCoords.x, endCoords.y + 6}, {CENTERX, CENTERY});
	}
}

//Draws a single Card button on the grid
void UIDrawer::DrawCardButtonOnGrid(Deck &deck, SingleButtonGroup &buttons, int cardIndex, int buttonIndex, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	DrawCardOnGrid(deck, cardIndex, startCoords, endCoords, showStats);
	DrawButtonOnGrid(buttons, buttonIndex, startCoords, endCoords);
}

//Draws a single button on the grid
void UIDrawer::DrawButtonOnGrid(SingleButtonGroup &buttons, int index, Vector2 startCoords, Vector2 endCoords) {
	if (index < 0 || index > buttons.GetSize()-1) return;
	Rectangle buttonDest = CoordsToRec(startCoords, endCoords);
	buttons[index].SetBounds(buttonDest);
	DrawTextureOnGrid(*buttons.GetTexture(), this->buttonSource, startCoords, endCoords, WHITE);
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
void UIDrawer::DrawButtonOnGrid(PlusMinusButtonGroup &buttons, int index, string value, Vector2 startCoords, Vector2 endCoords) {
	if (index < 0 || index > buttons.GetSize()-1) return;
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
	DrawTexturePro(*buttons.GetTexture(), this->buttonSource, buttonDest[0], this->origin, 0.0f, WHITE);
	DrawTexturePro(*buttons.GetTexture(), this->buttonSource, buttonDest[1], this->origin, 0.0f, WHITE);
	DrawTexturePro(*buttons.GetTexture(), this->buttonSource, buttonDest[2], this->origin, 0.0f, WHITE);
	
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
	DrawTextSWrapped(string(1, buttons[index].GetSymbolLabel(0)), buttonDest[0], this->textColor, this->currentFont->fontSize, (Alignment){CENTERX, CENTERY});
	DrawTextSWrapped(value, buttonDest[1], this->textColor, this->currentFont->fontSize, (Alignment){CENTERX, CENTERY});
	DrawTextSWrapped(string(1, buttons[index].GetSymbolLabel(1)), buttonDest[2], this->textColor, this->currentFont->fontSize, (Alignment){CENTERX, CENTERY});
}

//Draws a horizontal row of buttons on the grid
void UIDrawer::DrawButtonRowOnGrid(SingleButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords, int amountOfButtons) {
	float buttonWidth = (endCoords.x - startCoords.x)/(min(buttons.GetSize(), amountOfButtons));
	int amountRows = ceil((float)buttons.GetSize() / (float)amountOfButtons);
	float buttonHeight = (endCoords.y - startCoords.y)/amountRows;

	int buttonIndex = 0; //So that we know which button we're indexing
	for (int i = 0; i < amountRows; ++i){ //The row
		for (int j = 0; j < amountOfButtons; ++j) { //the column
			if (buttonIndex >= buttons.GetSize()) return;
			DrawButtonOnGrid(buttons, buttonIndex, 
			{startCoords.x + (j * buttonWidth), startCoords.y + (i * buttonHeight)}, 
			{startCoords.x + (j * buttonWidth) + buttonWidth, startCoords.y + (i * buttonHeight) + buttonHeight});
			++buttonIndex; //Advance the buttonIndex
		}
	}
}

void UIDrawer::DrawButtonColumnOnGrid(PlusMinusButtonGroup &buttons, Vector2 startCoords, Vector2 endCoords) {
	float buttonHeight = (endCoords.y - startCoords.y - (buttons.GetSize() * 3))/buttons.GetSize();
	for (int i = 0; i < buttons.GetSize(); ++i) {
		DrawButtonOnGrid(buttons, i, buttons[i].GetLabel(), 
		{startCoords.x, startCoords.y + (i * buttonHeight) + (i * 3)}, 
		{endCoords.x, startCoords.y + (i * buttonHeight) + buttonHeight + (i * 3)});
	}
}

//Draws the whole Deck of cards in a row
void UIDrawer::DrawCardRowOnGrid(Deck &deck, int spacing, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	float cardWidth = ((endCoords.x - startCoords.x) - (spacing * (deck.size() - 1)))/deck.size();
	float xValue;
	for (int i = 0; i < deck.size(); ++i) {
		xValue = startCoords.x + (i * cardWidth) + (i * spacing);
		DrawCardOnGrid(deck, i, {xValue, startCoords.y}, {xValue + cardWidth, endCoords.y}, showStats);
	}
}

//Draws the whole Deck of cards in a row
void UIDrawer::DrawCardButtonRowOnGrid(Deck &deck, SingleButtonGroup &buttons, int cardWidthSegments, Vector2 startCoords, Vector2 endCoords, bool showStats) {
	float cardWidth = cardWidthSegments;
	float spacing = ((endCoords.x - startCoords.x) - (cardWidth * deck.size()))/(deck.size() - 1);
	float xValue;
	for (int i = 0; i < deck.size(); ++i) {
		xValue = startCoords.x + (i * cardWidth) + (i * spacing);
		DrawCardButtonOnGrid(deck, buttons, i, i, {xValue, startCoords.y}, {xValue + cardWidth, endCoords.y}, showStats);
	}
}