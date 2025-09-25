/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Button Classes/Functions are written here
 */
 
#include "buttons.h"

//Start PlusMinusButton Class------------------------------------
PlusMinusButton::PlusMinusButton(string &text) {
	label = text;
	symbolLabels[0] = '+';
	symbolLabels[1] = '-';
	for (int i = 0; i < 2; ++i) {
		buttonState[i] = 0;
		buttonAction[i] = false;
		functionality[i] = true;
		tint[i] = WHITE;
	}
}

void PlusMinusButton::SetButtonState(int index, int state) {
	if (state < 0 || state > 2) return;
	if (index < 0 || index > 1) return;
	buttonState[index] = state;
	switch(state) {
		case 0:
			tint[index] = WHITE;
			break;
		case 1:
			tint[index] = LIGHTGRAY;
			break;
		case 2:
			tint[index] = DARKGRAY;
			break;
		default:
			tint[index] = WHITE;
			break;
	}
}

void PlusMinusButton::SetButtonAction(int index, bool action) {
	if (index < 0 || index > 1) return;
	buttonAction[index] = action;
}

void PlusMinusButton::SetBounds(int index, Rectangle dest) {
	if (index < 0 || index > 1) return;
	bounds[index] = dest;
}

Rectangle PlusMinusButton::GetBounds(int index) {
	if (index < 0 || index > 1) return (Rectangle){0, 0, 0, 0};
	return bounds[index];
}

bool PlusMinusButton::GetAction(int index) {
	if (index < 0 || index > 1) return false;
	return buttonAction[index];
}

bool PlusMinusButton::GetAllActions() {
	if (buttonAction[0] == true || buttonAction[1] == true) return true;
	return false;
}

int PlusMinusButton::GetState(int index) {
	if (index < 0 || index > 1) return 0;
	return buttonState[index];
}

Color PlusMinusButton::GetColor(int index) {
	if (index < 0 || index > 1) return WHITE;
	return tint[index];
}

void PlusMinusButton::SetLabel(string text) {
	label = text;
}

string PlusMinusButton::GetLabel() {
	return label;
}

char PlusMinusButton::GetSymbolLabel(int index) {
	return symbolLabels[index];
}

void PlusMinusButton::SetFunctionality(int index, bool b) {
	functionality[index] = b;
	if (functionality[index] == false) {
		SetButtonState(index, 2);
	}
}
bool PlusMinusButton::GetFunctionality(int index) {
	return functionality[index];
}
//End PlusMinusButton Class--------------------------------------


//Start PlusMinusButtonGroup Class-------------------------------
PlusMinusButtonGroup::PlusMinusButtonGroup(shared_ptr<Texture2D>& texture, Data &data) : buttonTexture(texture), StyleGuide(data) {
	size = 0;
}

void PlusMinusButtonGroup::AddButton(string label) {
	buttons.push_back(PlusMinusButton(label));
	++size;
}

void PlusMinusButtonGroup::AnimationLogic(Vector2 &mousePoint) {
	for (int j = 0; j < 2; ++j) { //Outer for loops makes sure both buttons are handled
		//resets all setup buttons back to false
		for (int i = 0; i < size; i++) {
			buttons[i].SetButtonAction(j, false);
		}
		
		//Allows buttons to have animations
		for (int i = 0; i < size; ++i) {
			if (buttons[i].GetFunctionality(j) == true) {
				if (CheckCollisionPointRec(mousePoint, buttons[i].GetBounds(j)))
				{
					if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttons[i].SetButtonState(j, 2);
					else buttons[i].SetButtonState(j, 1);
					if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttons[i].SetButtonAction(j, true);
				}
				else buttons[i].SetButtonState(j, 0);
			}
		}
	}
}

int PlusMinusButtonGroup::GetSize() {
	return size;
}

PlusMinusButton& PlusMinusButtonGroup::operator[](int index) {
	return buttons[index];
}

std::shared_ptr<Texture2D>& PlusMinusButtonGroup::GetTexture() {
	return buttonTexture; // Doesn't increase reference count
}
//End PlusMinusButtonGroup Class---------------------------------



//Start SingleButton Class---------------------------------------
SingleButton::SingleButton(string &text) {
	label = text;
	buttonState = 0;
	buttonAction = false;
	functionality = true;
	tint = WHITE;
}

void SingleButton::SetButtonState(int state) {
	if (state < 0 || state > 2) return;
	buttonState = state;
}

void SingleButton::SetButtonAction(bool action) {
	buttonAction = action;
}

void SingleButton::SetBounds(Rectangle dest) {
	bounds = dest;
}

Rectangle SingleButton::GetBounds() {
	return bounds;
}

bool SingleButton::GetAction() {
	return buttonAction;
}

int SingleButton::GetState() {
	return buttonState;
}

Color SingleButton::GetColor() {
	return tint;
}

void SingleButton::SetLabel(string text) {
	label = text;
}

string SingleButton::GetLabel() {
	return label;
}

void SingleButton::SetFunctionality(bool b) {
	functionality = b;
	if (functionality == false) {
		SetButtonState(2);
	}
}

bool SingleButton::GetFunctionality() {
	return functionality;
}
//End SingleButton Class-----------------------------------------



//Start SingleButtonGroup Class----------------------------------
SingleButtonGroup::SingleButtonGroup(shared_ptr<Texture2D>& texture, Data &data) : buttonTexture(texture), StyleGuide(data) {
	size = 0;
}

void SingleButtonGroup::AddButton(string label) {
	buttons.push_back(SingleButton(label));
	++size;
}

//Return size of button array
int SingleButtonGroup::GetSize() {
	return size;
}

//Overload [] operator
SingleButton& SingleButtonGroup::operator[](int index) {
	return buttons[index];
}

//Sets Button Functionality on a range on start(inclusive) to end(non-inclusive);
void SingleButtonGroup::SetFunctionality(bool b, int start, int end) {
	if (size == 0 || start > end || start < 0 || end > size - 1) {
		cout << "Indexing failure in function SetFunctionality!" << endl;
		return;
	}
	for (int i = start; i < end+1; ++i) {
		buttons[i].SetFunctionality(b);
	}
}

//Provides the animation logic for the button group
void SingleButtonGroup::AnimationLogic(Vector2 &mousePoint) {
	//resets all setup buttons back to false
	for (int i = 0; i < size; i++) {
		buttons[i].SetButtonAction(false);
	}
	
	//Allows buttons to have animations and functionality
	for (int i = 0; i < size; ++i) {
		if (buttons[i].GetFunctionality() == true) {
			if (CheckCollisionPointRec(mousePoint, buttons[i].GetBounds()))
			{
				//Sets Color
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) buttons[i].SetButtonState(2);
				else buttons[i].SetButtonState(1);
				
				//Make the button have a flag saying it clicked
				if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) buttons[i].SetButtonAction(true);
			}
			else buttons[i].SetButtonState(0); //State Determines color
		}
	}
}

void SingleButtonGroup::ClearAllButtons() {
	buttons.clear();
	size = 0;
}

shared_ptr<Texture2D>& SingleButtonGroup::GetTexture() {
	return buttonTexture; // Doesn't increase reference count
}
//End SingleButtonGroup Class----------------------------------
