/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Button Classes/Functions are written here
 */
 
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <memory>
#include "raylib.h"
#include "globals.h"
using namespace std;
 
#ifndef BUTTONSH
#define BUTTONSH

class PlusMinusButton {
	protected:
		int buttonState[2];
		bool buttonAction[2];
		bool functionality[2];
		char symbolLabels[2];
		Rectangle bounds[2];
		Color tint[2];
		string label;
	public:
		PlusMinusButton(string &text);
		void SetButtonState(int index, int state);
		void SetButtonAction(int index, bool action);
		void SetBounds(int index, Rectangle dest);
		Rectangle GetBounds(int index);
		bool GetAction(int index);
		bool GetAllActions();
		int GetState(int index);
		Color GetColor(int index);
		void SetLabel(string text);
		string GetLabel();
		char GetSymbolLabel(int index);
		void SetFunctionality(int index, bool b);
		bool GetFunctionality(int index);
};

class PlusMinusButtonGroup {
	protected:
		std::shared_ptr<Texture2D> buttonTexture;
		int size;
		vector<PlusMinusButton> buttons;
		Data &StyleGuide;
	public:
		PlusMinusButtonGroup(shared_ptr<Texture2D>& texture, Data &data);
		void AddButton(string label);
		int GetSize();
		PlusMinusButton& operator[](int index);
		void AnimationLogic(Vector2 &mousePoint);
		auto begin() { return buttons.begin(); }
		auto end() { return buttons.end(); }
		shared_ptr<Texture2D>& GetTexture();
};

class SingleButton {
	protected:
		int buttonState; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
		bool buttonAction; // Button action should be activated
		bool functionality; //If true the button can be clicked
		Rectangle bounds;
		Color tint;
		string label;
	public:
		SingleButton(string &text);
		void SetButtonState(int state);
		void SetButtonAction(bool action);
		void SetBounds(Rectangle dest);
		Rectangle GetBounds();
		bool GetAction();
		int GetState();
		Color GetColor();
		void SetLabel(string text);
		string GetLabel();
		void SetFunctionality(bool b);
		bool GetFunctionality();
};

class SingleButtonGroup {
	protected:
		std::shared_ptr<Texture2D> buttonTexture;
		int size;
		vector<SingleButton> buttons;
		Data &StyleGuide;
	public:
		SingleButtonGroup(shared_ptr<Texture2D>& texture, Data &data);
		void AddButton(string label);
		void ClearAllButtons();
		int GetSize();
		SingleButton& operator[](int index);
		void AnimationLogic(Vector2 &mousePoint);
		void SetFunctionality(bool b, int start, int end);
		auto begin() { return buttons.begin(); }
		auto end() { return buttons.end(); }
		shared_ptr<Texture2D>& GetTexture();
};

#endif
