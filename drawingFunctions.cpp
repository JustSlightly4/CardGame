/*
 * Eric Ryan Montgomery
 * 03/17/2025
 * For CardGameUI
 * Function/Classes that are not part of the cards/deck are written here
 */
 
#include "drawingFunctions.h"

void DrawBasicCardStats(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide) {
	//Draw Card Details
	Rectangle belowCard = {pos.x, pos.y + (StyleGuide.cardTextureSize.y * size) + (StyleGuide.margin * (size-1)), StyleGuide.cardTextureSize.x * size, StyleGuide.fontSize/2.0f};
	DrawTextS(Deck->at(index)->GetName(), belowCard, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){CENTERX, CENTERY});
	belowCard.y += (StyleGuide.fontSize * size * 0.5);
	DrawTextS("P: " + to_string(Deck->at(index)->GetPower()) + ", MP: " + to_string(Deck->at(index)->GetMagicalPower()), belowCard, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){CENTERX, CENTERY});
	belowCard.y += (StyleGuide.fontSize * size * 0.5);
	DrawTextS("Health: " + to_string(Deck->at(index)->GetHealth()) + "/" + to_string(Deck->at(index)->GetHealthT()), belowCard, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){CENTERX, CENTERY});
}

void DrawTextS(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation) {
	if (fontSize <= 0) fontSize = 40;
	
	//Draw Test
	if (orientation.x == CENTERX) { //Centered
		dest.x = (dest.x + (dest.width/2)) - (((MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1.0f)).x)/2);
	} else if (orientation.x == RIGHTX) { //Right
		dest.x = dest.x + dest.width - ((MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1.0f)).x);
	} else { //Left
		//Do nothing
	}
	
	if (orientation.y == CENTERY) { //Centered
		dest.y = (dest.y + (dest.height/2)) - (((MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1.0f)).y)/2);
	} else if (orientation.y == DOWNY) { //Down
		dest.y = (dest.y + (dest.height)) - (((MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1.0f)).y)/2);
	} else { //UP
		//Do nothing
	}
	
	DrawTextEx(GetFontDefault(), text.c_str(), (Vector2){dest.x, dest.y}, fontSize, 1.0f, tint); // Draw text using font and additional parameters
}

float DrawTextSWrapped(string text, Rectangle dest, Color tint, float fontSize, Alignment orientation) {

    float totalHeight;
    float modifiedFontSize = fontSize * 1.2;
    vector<string> lines;
    do {
		lines.clear();
		string currentLine;
		istringstream words(text);
		string word;
		modifiedFontSize /= 1.2;
		while (words >> word) {
			string testLine = currentLine.empty() ? word : currentLine + " " + word;
			Vector2 textSize = MeasureTextEx(GetFontDefault(), testLine.c_str(), modifiedFontSize, 1.0f);
			
			if (textSize.x <= dest.width) {
				currentLine = testLine;
			} else {
				lines.push_back(currentLine);
				currentLine = word;
			}
		}
		
		if (!currentLine.empty()) {
			lines.push_back(currentLine);
		}
		
		totalHeight = (lines.size()-1) * modifiedFontSize;
	} while(totalHeight >= (dest.height) && orientation.reduceTextSize == true);
	
    if (lines.size() < 1) {
		//cout << "Error: No text in function DrawTextSWrapped" << endl;
		return 0;
	} else if (lines.size() < 2) {
		DrawTextS(lines[0], dest, tint, modifiedFontSize, (Alignment){orientation.x, orientation.y});
	} else {
		for (size_t i = 0; i < lines.size(); ++i) {
			if (orientation.y == CENTERY) DrawTextS(lines[i], (Rectangle){dest.x, dest.y - (totalHeight/2) + (i * modifiedFontSize), dest.width, dest.height}, tint, modifiedFontSize, (Alignment){orientation.x, orientation.y});
			if (orientation.y == UPY || orientation.y == DOWNY) {
				DrawTextS(lines[i], dest, tint, modifiedFontSize, (Alignment){orientation.x, orientation.y});
				dest.y += modifiedFontSize;
			}
		}
	}
	
	if (totalHeight <= 0) return modifiedFontSize;
	return (modifiedFontSize * lines.size());
}

void DrawButtonLine(SingleButtonGroup &buttons, Data &StyleGuide) {
	DrawRectangleRec(StyleGuide.recDimensions, BLACK);
	float startOffSet = (StyleGuide.screenDimensions.x - (buttons.GetSize() * StyleGuide.buttonTextureSize.x))/2;
	for (int i = 0; i < buttons.GetSize(); ++i) {
		DrawButton(buttons, i, (Vector2){StyleGuide.buttonTextureSize.x * i + startOffSet, StyleGuide.screenDimensions.y - StyleGuide.recDimensions.height/2.0f - StyleGuide.margin - StyleGuide.buttonTextureSize.y/2.0f}, StyleGuide);
	}
}

void DrawSettingsButtons(PlusMinusButtonGroup &buttons, Data &StyleGuide) {
	Rectangle textDest;
	textDest = DrawButton(buttons, 0, (Vector2){StyleGuide.screenDimensions.x/2.0f - (StyleGuide.buttonTextureSize.x * 3.0f)/2.0f, (StyleGuide.buttonTextureSize.y * ((2.0f * 0) + 1.0f)) + StyleGuide.margin}, StyleGuide);
	DrawTextSWrapped(to_string(StyleGuide.numCards), textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	textDest = DrawButton(buttons, 1, (Vector2){StyleGuide.screenDimensions.x/2.0f - (StyleGuide.buttonTextureSize.x * 3.0f)/2.0f, (StyleGuide.buttonTextureSize.y * ((2.0f * 1) + 1.0f)) + StyleGuide.margin}, StyleGuide);
	DrawTextSWrapped(to_string(StyleGuide.deckStrength), textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	textDest = DrawButton(buttons, 2, (Vector2){StyleGuide.screenDimensions.x/2.0f - (StyleGuide.buttonTextureSize.x * 3.0f)/2.0f, (StyleGuide.buttonTextureSize.y * ((2.0f * 2) + 1.0f)) + StyleGuide.margin}, StyleGuide);
	if (StyleGuide.deck2AI == true) DrawTextSWrapped("true", textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	if (StyleGuide.deck2AI == false) DrawTextSWrapped("false", textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
}

void DrawRules(float &scrollOffset, Data &StyleGuide) {
	vector<string> *gamerules = new vector<string> {
		"Game Rules:",
		"1: Each player gets a user defined amount of cards referred to as a deck.",
		"2: Before play begins, each deck is shuffled and hidden from the player.",
		"3: Each player then takes the first and last card from their deck and puts "
			"those cards into play. The players first card is considered their main card "
			"and the players last card is considered their support card.",
		"4: A players main card is always the card that is taking damage or debuffs from "
			"the opponent. The support card can not be targeted by the opponent.",
		"5: A randomly selected player goes first and has the "
			"option to attack, cast a spell, swap their main card out for their support card, charge up "
			"their card, use their flask, or print the gamerules.",
		"6: If a card's health or power falls below one, the round is ended and the "
			"winning player recieves a point. Both main cards currently in play are then "
			"discarded and each player's next card is played as their main. All support cards remain in "
			"play upon a rounds end. The player who lost the last round gets to go first on the next round.",
		"7: Upon attacking, the card in play has a 5% chance to either land a "
			"critical hit for 20% more damage or miss their attack entirely. A cards base damage is always that "
			"cards amount of physical power.", 
		"8: Upon casting a spell, the card in play will cast their chosen spell at the opponent for various effects. "
			"In general, the spells power will be a random value between one and the cards magical power.",
		"9: Upon swapping, the player swaps their main card out for their support card/ "
			"Swapping can only be done twice per round and can not be "
			"done on the last round of the game.",
		"10: Upon charging up, the players card receives an increase to their health and "
			"power. If a card powers up twice, the card uses their special ability.",
		"11: Upon using the flask, the players card will be fully healed. The "
			"flask does not use up the players turn and they may take another action. "
			"However, the flask may only be used twice per game.",
		"12: Upon printing the gamerules, the gamerules will be displayed on screen. "
			"Printing the gamerules does not use up the players turn and they may take "
			"another action.",
	};
	DrawStringVector(*gamerules, scrollOffset, StyleGuide);
	delete gamerules;
}

void DrawSkills(float &scrollOffset, Data &StyleGuide) {
	vector<string> *skills = new vector<string> {
		"Card Abilities:",
		"-Pure Mimicry: If the user's color is the same as the opponents color or if either card is colored white, the user copies the opponents card exactly.",
		"-Discolored Mimicry: If the user's colors is not the same as the opponents color and neither card is colored white, the user copies the opponents card as if it were newly created.",
		"-Heavy Handed: Increases the users power.",
		"-Faithful: Increases users critical hit chance.",
		"-Fast Hand: Gives the user an extra action.",
		"-Healthy Mind: Increases the users health.",
		"-Terror: Decreases the opponents power.",
		"-Resistant: Increases the users physicalResistance.",
		"-Magic Immunity: The user becomes completely immune to all magical attacks",
		"-Holy Presence: Charges up the next card in the users deck by one if the next card is not already charged.",
		"-Evil Presence: Charges down the next card in the opponents deck by one if the next card is not already charged.",
		"-Accurate: Increases the users critical hit chance and decreases the users miss chance.",
		"-Inner Gate: The user can only either critical hit or miss an attack.",
		"-Necromancy: Revives the most powerful fallen card in the users deck and puts that card into play. If there are no fallen cards in the users deck then the user gains power instead.",
		"-Divine Help: The gods strike the users opponent.",
		"-Nullify: If the opponent has not used their ability yet, the user will prevent the opponent from ever using their ability. Otherwise, the user will attempt to reverse the oppponents ability.",
		"-Made in Heaven: The user will reset the opponents charge back to zero. Does not affect already used abilities.",
		"-Chaos: The user will use a random ability.",
		"-Strategic Fire: The user will pick an ability depending on various game factors.",
		" ",
		" ",
		"Card Spells:",
		"-Force: Attack the opponent for a random value between one and the users magical power",
		"-Drain: Attacks the opponent power for a random value between one and the users magical power divided by two",
		"-Weaken: Reduces the opponents resistance for a random percentage between one and the user magical power",
		"-Heal: Heals the caster for a random value between one and the users magical power.",
	};
	DrawStringVector(*skills, scrollOffset, StyleGuide);
	delete skills;
}

void DrawStringVector(const vector<string> &sentences, const float &scrollOffset, Data &StyleGuide) {
    float heightIncrease = 0;
	for (size_t i = 0; i < sentences.size(); ++i) {
		heightIncrease += DrawTextSWrapped(sentences[i], (Rectangle){StyleGuide.margin, StyleGuide.margin + heightIncrease - scrollOffset, StyleGuide.screenDimensions.x - (StyleGuide.margin * 2), StyleGuide.fontSize}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){LEFTX, UPY, false});
		heightIncrease += StyleGuide.sentenceSpacing;
	}
	StyleGuide.maxScroll = heightIncrease - StyleGuide.screenDimensions.y + StyleGuide.recDimensions.height + (StyleGuide.margin * 3);
	if (StyleGuide.maxScroll < 0) StyleGuide.maxScroll = 0;
}

//Prints all cards out in a line
void DrawCardLine(deck *Deck, SingleButtonGroup &buttons, Vector2 startPos, int spacing, Data &StyleGuide) {
	Vector2 pos = startPos;
	//Rectangle buttonDest = {pos.x, pos.y, StyleGuide.cardTextureSize.x, StyleGuide.cardTextureSize.y};
	for (int i = 0; i < Deck->GetCardAmt(); ++i) {
		pos.x = (startPos.x + (StyleGuide.cardTextureSize.x * i)) + (spacing * i);
		//buttonDest.x = pos.x;
		DrawCardButton(i, Deck, buttons, pos, 1, StyleGuide);
		//buttons[i].SetBounds(buttonDest);
		//DrawTexturePro(*buttons.GetTexture(), (Rectangle){0, 0, StyleGuide.cardTextureSize.x, StyleGuide.cardTextureSize.y}, buttonDest, StyleGuide.origin, 0, buttons[i].GetColor());
	}
}

void DrawCard(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide) {
	
	//Draw Card Texture		
	DrawTexturePro(*Deck->GetTexture(), (Rectangle){GetCardSourceX(Deck->at(index), StyleGuide), 
			0, StyleGuide.baseCardTextureSize.x, StyleGuide.baseCardTextureSize.y}, 
			(Rectangle){pos.x, pos.y, StyleGuide.cardTextureSize.x * size, StyleGuide.cardTextureSize.y * size}, 
			StyleGuide.origin, 0, Deck->at(index)->GetColorRaylib());
	
	//Draw Card Details
	DrawBasicCardStats(index, Deck, pos, size, StyleGuide);
}

void DrawCardWithStats(int index, deck *Deck, Vector2 pos, float size, Data &StyleGuide) {
	
	Rectangle cardDest = {pos.x, pos.y, StyleGuide.cardTextureSize.x * size, StyleGuide.cardTextureSize.y * size};
	//Draw Card Texture		
	DrawTexturePro(*Deck->GetTexture(), (Rectangle){GetCardSourceX(Deck->at(index), StyleGuide), 
			0, StyleGuide.baseCardTextureSize.x, StyleGuide.baseCardTextureSize.y}, 
			cardDest, StyleGuide.origin, 0, Deck->at(index)->GetColorRaylib());
	
	//Draw Card Details
	ostringstream ss;
	Rectangle textDest = {cardDest.x + (StyleGuide.cardTextureSize.x * size) + StyleGuide.margin, cardDest.y, cardDest.width * 2, cardDest.height};
	textDest.y += DrawTextSWrapped(Deck->at(index)->GetName(), textDest, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){LEFTX, UPY});
	textDest.y += DrawTextSWrapped("Power: " + to_string(Deck->at(index)->GetPower()), textDest, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){LEFTX, UPY});
	textDest.y += DrawTextSWrapped("Magical Power: " + to_string(Deck->at(index)->GetMagicalPower()), textDest, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){LEFTX, UPY});
	textDest.y += DrawTextSWrapped("Health: " + to_string(Deck->at(index)->GetHealth()) + "/" + to_string(Deck->at(index)->GetHealthT()), textDest, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){LEFTX, UPY});
	ss << "Physical Resistance: " << setprecision(2) << Deck->at(index)->GetPhysicalResistance();
	textDest.y += DrawTextSWrapped((ss.str()).c_str(), textDest, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){LEFTX, UPY});
	ss.str(""); ss << "Magical Resistance: " << setprecision(2) << Deck->at(index)->GetMagicalResistance();
	textDest.y += DrawTextSWrapped((ss.str()).c_str(), textDest, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){LEFTX, UPY});
	textDest.y += DrawTextSWrapped("Ability: " + Deck->at(index)->GetAbilityStr(), textDest, StyleGuide.textColor, (StyleGuide.fontSize/2) * size, (Alignment){LEFTX, UPY});
}

void DrawCardButton(int index, deck *Deck, SingleButtonGroup &buttons, Vector2 pos, float size, Data &StyleGuide) {
	
	buttons[index].SetBounds((Rectangle){pos.x, pos.y, StyleGuide.cardTextureSize.x, StyleGuide.cardTextureSize.y});
	//Draw Card Texture
	
	Color cardColor;
	if (buttons[index].GetState() == 0) { //Card color when it's not highlighted
		cardColor = Deck->at(index)->GetColorRaylib();
	} else { //Card color when it is highlighted
		cardColor = buttons[index].GetColor();
	}
	DrawTexturePro(
		*Deck->GetTexture(), 
		(Rectangle){GetCardSourceX(Deck->at(index), StyleGuide), 0, 100, 150}, 
		(Rectangle){pos.x, pos.y, StyleGuide.cardTextureSize.x * size, StyleGuide.cardTextureSize.y * size}, 
		StyleGuide.origin, 0, cardColor
	);
	
	//Draw Card Details
	DrawBasicCardStats(index, Deck, pos, size, StyleGuide);
}

void DrawGame(deck *deck1, deck *deck2, GameVars &gameVars, Data &StyleGuide, Flags &flags) {
	int size = 2;
	
	//Draw Game Info
	Rectangle roundDest = {StyleGuide.recDimensions.height + (StyleGuide.margin * 2), StyleGuide.margin, StyleGuide.screenDimensions.x - (StyleGuide.recDimensions.height * 2) - (StyleGuide.margin * 4), StyleGuide.fontSize};
	Rectangle turnDest = {roundDest.x, roundDest.y + StyleGuide.fontSize, roundDest.width, roundDest.height};
	Rectangle whoDest = {roundDest.x, (roundDest.y + turnDest.y)/2, roundDest.width, roundDest.height};
	DrawTextS("Round: " + to_string(gameVars.round+1), roundDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	DrawTextS("Turn: " + to_string(gameVars.turn+1), turnDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	if (gameVars.playerInPlay == 0) DrawTextS("Player 1 Turn", whoDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){LEFTX, CENTERY});
	if (gameVars.playerInPlay == 1) DrawTextS("Player 2 Turn", whoDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){RIGHTX, CENTERY});
	
	
	//Draw both Cards currently in play
	float offSet = StyleGuide.cardTextureSize.x/2;
	Vector2 player1MainDest = {StyleGuide.screenDimensions.x/2 - (2 *(StyleGuide.cardTextureSize.x * size)) - offSet, StyleGuide.screenDimensions.y/2 - StyleGuide.cardTextureSize.y/2 - (StyleGuide.recDimensions.height * 2)};
	Vector2 player2MainDest = {StyleGuide.screenDimensions.x/2 + ((3 * (StyleGuide.cardTextureSize.x * size))/2) - offSet, StyleGuide.screenDimensions.y/2 - StyleGuide.cardTextureSize.y/2 - (StyleGuide.recDimensions.height * 2)};
	DrawCard(gameVars.round, deck1, player1MainDest, size, StyleGuide);
	DrawCard(gameVars.round, deck2, player2MainDest, size, StyleGuide);
	
	//Draws the card that the player is able to swap out with
	Vector2 player1SupportDest = {StyleGuide.screenDimensions.x/2.0f - (StyleGuide.cardTextureSize.x * size/2) - offSet, StyleGuide.screenDimensions.y/2 - (StyleGuide.recDimensions.height)};
	Vector2 player2SupportDest = {StyleGuide.screenDimensions.x/2.0f + (StyleGuide.cardTextureSize.x * size/2) - offSet, StyleGuide.screenDimensions.y/2 - (StyleGuide.recDimensions.height)};
	if (gameVars.round < deck1->GetCardAmt() - 1) {
		DrawCard(deck1->GetCardAmt() - 1, deck1, player1SupportDest, size/2, StyleGuide);
		DrawCard(deck1->GetCardAmt() - 1, deck2, player2SupportDest, size/2, StyleGuide);
	}
	
	//Draws the dialog box
	Rectangle outerBounds = {StyleGuide.margin, StyleGuide.screenDimensions.y - StyleGuide.recDimensions.height * 2 - StyleGuide.margin * 2, StyleGuide.recDimensions.width, StyleGuide.recDimensions.height};
	Rectangle innerBounds = {outerBounds.x + StyleGuide.margin, outerBounds.y + (StyleGuide.margin * 2), outerBounds.width - (StyleGuide.margin * 2), outerBounds.height - (StyleGuide.margin * 4)};
	DrawRectangleLinesEx(outerBounds, StyleGuide.margin, StyleGuide.textColor);
	DrawTextSWrapped(gameVars.dialog, innerBounds, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	
	//Draws the scoring columns
	Rectangle leftColumn = {StyleGuide.margin, StyleGuide.margin, StyleGuide.recDimensions.height, StyleGuide.screenDimensions.y - StyleGuide.recDimensions.height - outerBounds.height - (StyleGuide.margin*4)};
	Rectangle rightColumn = {StyleGuide.screenDimensions.x - leftColumn.width - StyleGuide.margin, StyleGuide.margin, StyleGuide.recDimensions.height, StyleGuide.screenDimensions.y - StyleGuide.recDimensions.height - outerBounds.height - (StyleGuide.margin*4)};
	DrawRectangleLinesEx(leftColumn, StyleGuide.margin, StyleGuide.textColor); 
	DrawRectangleLinesEx(rightColumn, StyleGuide.margin, StyleGuide.textColor);
	
	Rectangle leftMeter = {leftColumn.x + StyleGuide.margin, leftColumn.y + StyleGuide.margin + leftColumn.height - ((leftColumn.height/StyleGuide.numCards) * gameVars.player1Score), 
		leftColumn.width - (StyleGuide.margin * 2), ((leftColumn.height/StyleGuide.numCards) * gameVars.player1Score) - (StyleGuide.margin * 2)};
	Rectangle rightMeter = {rightColumn.x + StyleGuide.margin, rightColumn.y + StyleGuide.margin + rightColumn.height - ((rightColumn.height/StyleGuide.numCards) * gameVars.player2Score), 
		leftColumn.width - (StyleGuide.margin * 2), ((rightColumn.height/StyleGuide.numCards) * gameVars.player2Score) - (StyleGuide.margin * 2)};
	DrawRectangleRec(leftMeter, RED);
	DrawRectangleRec(rightMeter, BLUE);
	
	//Draw Star on who is playing
	if (gameVars.playerInPlay == PLAYER1) {
		if (gameVars.currCardRole == C_MAIN) DrawPolyLinesEx(player1MainDest, 4, StyleGuide.starRadius, StyleGuide.starRotation, StyleGuide.starLineThickness, GOLD);
		else DrawPolyLinesEx(player1SupportDest, 4, StyleGuide.starRadius, StyleGuide.starRotation, StyleGuide.starLineThickness, GOLD);
	} else {
		if (gameVars.currCardRole == C_MAIN) DrawPolyLinesEx(player2MainDest + (Vector2){StyleGuide.cardTextureSize.x * size, 0}, 4, StyleGuide.starRadius, StyleGuide.starRotation, StyleGuide.starLineThickness, GOLD);
		else DrawPolyLinesEx(player2SupportDest + (Vector2){StyleGuide.cardTextureSize.x, 0}, 4, StyleGuide.starRadius, StyleGuide.starRotation, StyleGuide.starLineThickness, GOLD); 
	}
}

void DrawButton(SingleButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide) {
	Rectangle dest;
	dest.x = position.x;
	dest.y = position.y;
	dest.width = StyleGuide.buttonTextureSize.x;
	dest.height = StyleGuide.buttonTextureSize.y;
	buttons[index].SetBounds(dest);
	DrawTexturePro(*buttons.GetTexture(), StyleGuide.buttonSource, dest, (Vector2){0, 0}, 0, buttons[index].GetColor()); // Draw a part of a texture defined by a rectangle with 'pro' parameters
	DrawTextSWrapped(buttons[index].GetLabel(), dest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
}

Rectangle DrawButton(PlusMinusButtonGroup &buttons, int index, Vector2 position, Data &StyleGuide) {
	buttons[index].SetBounds(0, (Rectangle){position.x, position.y, StyleGuide.buttonTextureSize.x, StyleGuide.buttonTextureSize.y});
	Rectangle middleDest = {position.x + StyleGuide.buttonTextureSize.x, position.y, StyleGuide.buttonTextureSize.x, StyleGuide.buttonTextureSize.y};
	buttons[index].SetBounds(1, (Rectangle){position.x + (StyleGuide.buttonTextureSize.x * 2), position.y, StyleGuide.buttonTextureSize.x, StyleGuide.buttonTextureSize.y});
	
	//Draw left most button
	DrawTexturePro(*buttons.GetTexture(), StyleGuide.buttonSource, buttons[index].GetBounds(0), (Vector2){0, 0}, 0, buttons[index].GetColor(0)); // Draw a part of a texture defined by a rectangle with 'pro' parameters
	DrawTextSWrapped(string(1, buttons[index].GetSymbolLabel(0)), buttons[index].GetBounds(0), StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	
	//Draw middle button that displays text
	DrawTexturePro(*buttons.GetTexture(), StyleGuide.buttonSource, middleDest, (Vector2){0, 0}, 0, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' 
	
	//Draw right most button
	DrawTexturePro(*buttons.GetTexture(), StyleGuide.buttonSource, buttons[index].GetBounds(1), (Vector2){0, 0}, 0, buttons[index].GetColor(1)); // Draw a part of a texture defined by a rectangle with 'pro' parameters
	DrawTextSWrapped(string(1, buttons[index].GetSymbolLabel(1)), buttons[index].GetBounds(1), StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	
	//Draw Text above all three buttons to describe what these buttons do
	DrawTextS(buttons[index].GetLabel(), (Rectangle){middleDest.x, middleDest.y - StyleGuide.fontSize * 2, middleDest.width, middleDest.height}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	return middleDest;
}

void DrawButtonTopLabel(SingleButtonGroup &buttons, int index, Vector2 position, string variable, Data &StyleGuide) {
	Rectangle dest;
	dest.x = position.x;
	dest.y = position.y;
	dest.width = StyleGuide.buttonTextureSize.x;
	dest.height = StyleGuide.buttonTextureSize.y;
	buttons[index].SetBounds(dest);
	DrawTexturePro(*buttons.GetTexture(), StyleGuide.buttonSource, dest, (Vector2){0, 0}, 0, buttons[index].GetColor()); // Draw a part of a texture defined by a rectangle with 'pro' parameters
	DrawTextSWrapped(buttons[index].GetLabel(), (Rectangle){dest.x, dest.y - StyleGuide.buttonTextureSize.y, dest.width, dest.height}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	DrawTextSWrapped(variable, (Rectangle){dest.x, dest.y, dest.width, dest.height}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
}

void DrawCardEditScreen(PlusMinusButtonGroup &buttons, CardEditVars &cardEditVars, deck *Deck, deck *dummyDeck, Data &StyleGuide) {
	//Dummy deck acts as temp card so that the user can disregard the changes if they want to
	//Feature has not been implemented fully yet
	int size = 2;
	int i = cardEditVars.cardClickedOn;
	DrawCardWithStats(i, dummyDeck, (Vector2){StyleGuide.margin + (StyleGuide.cardTextureSize.x * size * 0.5f), (StyleGuide.screenDimensions.y - (StyleGuide.cardTextureSize.y * size))/2 - StyleGuide.recDimensions.height - (StyleGuide.margin * 2)}, size, StyleGuide);
	Vector2 dest = {((3 * StyleGuide.screenDimensions.x)/4) - (((StyleGuide.buttonTextureSize.x * 3)/2)), (1 * (StyleGuide.screenDimensions.y - StyleGuide.recDimensions.height - (StyleGuide.margin * 2) - (StyleGuide.buttonTextureSize.y/2) - (StyleGuide.fontSize * 2))/4)};
	
	//Card Control Buttons
	Rectangle textDest;
	textDest = DrawButton(buttons, 0, dest, StyleGuide);
	DrawTextSWrapped(dummyDeck->at(i)->GetColorStr(), textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	textDest = DrawButton(buttons, 1, (Vector2){dest.x, dest.y * 2}, StyleGuide);
	DrawTextSWrapped(dummyDeck->at(i)->GetAttributeStr(), textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	textDest = DrawButton(buttons, 2, (Vector2){dest.x, dest.y * 3}, StyleGuide);
	DrawTextSWrapped(to_string(dummyDeck->at(i)->GetNumber()), textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	textDest = DrawButton(buttons, 3, (Vector2){dest.x, dest.y * 4}, StyleGuide);
	DrawTextSWrapped(dummyDeck->at(i)->GetSpellStr(), textDest, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
	
	//Point amount up top
	DrawTextS("Points Left " + to_string(cardEditVars.remainingPoints) + "/" + to_string(Deck->GetTotalPoints()), (Rectangle){0, StyleGuide.margin, StyleGuide.screenDimensions.x, StyleGuide.fontSize}, StyleGuide.textColor, StyleGuide.fontSize, (Alignment){CENTERX, CENTERY});
}
