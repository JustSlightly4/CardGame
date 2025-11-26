/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Button Classes/Functions are written here
 */
#ifndef BUTTONSH
#define BUTTONSH
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <functional>
#include <memory>
#include "raylib.h"
#include "Globals.h"
using namespace std;

class PlusMinusButton {
	public:
		PlusMinusButton(string title, string label);
		void SetButtonState(int index, int state);
		void SetButtonAction(int index, bool action);
		void SetBounds(int index, Rectangle dest);
		Rectangle GetBounds(int index);
		bool GetAction(int index);
		bool GetAllActions();
		int GetState(int index);
		Color GetColor(int index);
		void SetTitle(string title);
		string GetTitle();
		void SetLabel(string label);
		string GetLabel();
		char GetSymbolLabel(int index);
		void SetFunctionality(int index, bool b);
		bool GetFunctionality(int index);

	protected:
		int buttonState[2];
		bool buttonAction[2];
		bool functionality[2];
		char symbolLabels[2];
		Rectangle bounds[2];
		Color tint[2];
		string title;
		string label;
};

class PlusMinusButtonGroup {
	public:
		PlusMinusButtonGroup(shared_ptr<Texture2D>& texture);
		void AddButton(string label, string buttonLabel);
		int GetSize();
		PlusMinusButton& operator[](int index);
		PlusMinusButton& operator[](const string& title);
		void AnimationLogic(Vector2 &mousePoint);
		auto begin() { return buttons.begin(); }
		auto end() { return buttons.end(); }
		shared_ptr<Texture2D>& GetTexture();

	protected:
		std::shared_ptr<Texture2D> buttonTexture;
		int size;
		vector<PlusMinusButton> buttons;
		unordered_map<string, int> buttonsMap;
};

class SingleButton {
	public:
		SingleButton(string label);
		void SetButtonState(int state);
		void SetButtonAction(bool action);
		void SetBounds(Rectangle dest);
		Rectangle GetBounds();
		bool GetAction();
		int GetState();
		Color GetColor();
		void SetLabel(string label);
		string GetLabel();
		void SetFunctionality(bool b);
		bool GetFunctionality();
	protected:
		int buttonState; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
		bool buttonAction; // Button action should be activated
		bool functionality; //If true the button can be clicked
		Rectangle bounds;
		Color tint;
		string label;
};

class SingleButtonGroup {
	public:
		SingleButtonGroup(shared_ptr<Texture2D>& texture);
		void AddButton(string label);
		void ClearAllButtons();
		int GetSize();
		SingleButton& operator[](int index);
		SingleButton& operator[](const string& label);
		void AnimationLogic(Vector2 &mousePoint);
		void SetFunctionality(bool b, int start, int end);
		auto begin() { return buttons.begin(); }
		auto end() { return buttons.end(); }
		shared_ptr<Texture2D>& GetTexture();
	protected:
		std::shared_ptr<Texture2D> buttonTexture;
		int size;
		vector<SingleButton> buttons;
		unordered_map<string, int> buttonsMap;
};

#endif
