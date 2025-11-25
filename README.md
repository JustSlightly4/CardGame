Eric Ryan Montgomery  
11/25/25  
Card Game Documentation  

# Table of Contents
- [Introduction](#introduction)
- [Design Goals](#design-goals)
- [CardGameUI](#cardgameui)
- [Cards](#cards)
- [Decks](#decks)
- [RegularGame](#regulargame)
- [UIDrawer](#uidrawer)
- [Buttons](#buttons)
- [StateVariables](#statevariables)
- [Globals](#globals)
- [Coding Conventions](#coding-conventions)
- [TODO](#todo)

# Introduction
This documentation is meant to provide a simple, high-level view of the project. I want to do a more in-depth explanation, but as a solo developer working on a passion project, I simply do not have the time. The following text should cover every file created for the project and provide general rules and conventions for writing code to ensure it is consistent, readable, and maintainable for the foreseeable future.

I started this project in the summer of 2024 to practice writing C code while taking an introduction to computer programming course. I did not plan anything and have just been following a simple build-and-fix model. The project started in C using the terminal as the interface, but eventually, Raylib was added to build a GUI, and later, the code was ported to C++. It's not a particularly good project, and many inconsistencies appear because the language, design goals, and my programming skills have changed over the course of building it. However, I will continue to develop and add more to this project whenever I get the time.

The project is currently dubbed "Card Game" because I have never been able to come up with a better name. The purpose is for it to be a card game that's different from the standard 52-card style games that most people know how to play. It is mainly inspired by games with heavy RPG elements. 

# Design Goals
As stated in the introduction, the project's design goals have changed over time. Currently, the biggest design goal is to make the program as "externally" modular as possible. For example, the UIDrawer class should be able to be ripped out and used in another project alongside Raylib with minimal code changes. The same should apply to cards, decks, and buttons as well.

The second most important design goal is to be as "internally" modular as possible. However, this mainly applies to the UIDrawer and buttons classes. For example, the UIDrawer class and Raylib should be able to be taken out and replaced with another GUI library or engine with minimal changes to the rest of the code. Another point to keep in mind here is that only the main and graphics sections of the code should rely on Raylib so that this goal may be preserved.

The third design goal is modularity over efficiency. Inspired by the Linux kernel, I would like to build a layered system rather than a monolithic one. This goal will help preserve the previous two, and efficiency should not be a large concern since the project is intended to stay small. 

# CardGameUI
CardGameUI acts as the main file for the program. It is split up into four subsequent sections:  
1. Intialization - In this section all the needed variables, buttons, classes, and the window itself is created and intialized.
2. Logic - The logic section is where all functions that do not draw anything to the screen run. Variables and classes are updated here each frame. The logic for buttons inputs, and the game are here.
3. Drawing - The drawing section is where all functions that draw to the screen go. Drawing should only be done using the custom made UIDrawer class.
4. De-Initialization - The de-initialization section is where all variables that need de-initialization are deleted to prevent memory leaks. The main window is closed here.

# Cards
The card class represents a single card. Each card must have a color, an attribute, and a power level. These three main stats determine all other stats except for a card's spell. Each card can have any spell the user desires; however, the power of that spell is determined by a card's magical power, which in turn is determined by the three main stats.

A card is largely useless on its own. To function within a game, a card must be part of a deck.

# Decks
The deck class represents one or more cards. However, a point system is in place to provide a limit on the number of cards within a player's deck. The point limit is defined as:  
`number of cards in the deck * the average amount of points a card should have.`  

Cards do not need to be created before making a deck, though it is allowed. The deck class has built-in functionality to create its own cards.

# RegularGame
The RegularGame class handles all functionality and the state of the current game in progress. When I talk about the game here, I am not referring to the program as a whole but only the section where the cards battle each other. 

# UIDrawer
The UIDrawer class handles all functionality that relates to drawing to the screen. It is IMPORTANT to note that the regular Raylib functions should never be used outside this class, as the UIDrawer class operates on a grid to keep the window and all textures consistent.

# Buttons
Both button classes handle the buttons' functionality, but not what the buttons actually do. The button classes only update the variables for whether the buttons are hovered over, clicked, or currently on screen. What each button actually does/modifies is handled in the code's logic section, either in main or in other functions.

# StateVariables
The state variables file contains multiple classes that hold the state of some screen or object. For example, the editcard screen relies on a state class to track which card is being edited and the edits being made.

# Globals
The globals file contains variables or functions that, for some reason, need to be global. Currently, this includes large, unchanging strings and overloaded operators for Raylib.

# Coding Conventions
- Variables: camelCase
```
int myInteger;
string myStringForMyProgram;
```

- Functions: UpperCase  
```
void MyFunction() {}
int MySecondFunction() {}
```

- Classes: UpperCase, public, then protected. Classes in the .h file should only have variables and headers within them whereas, the .cpp should have all the actual code.

```
//Vector3D.h
class Vector3D {
    public:
        Vector3D::Vector3D(int i, int j, int k);
        void PrintComponents();
    protected:
        int x;
        int y;
        int z;
}
```

```
//Vector3D.cpp
Vector3D::Vector3D(int i, int j, int k) {
    ...
}

void Vector3D::PrintComponents() {
    ...
}
```

# TODO
1. Update all code to follow general guidelines
2. Continue updating and rewriting the new UIDrawer and RegularGame classes
3. Add status effects and more spells
4. Give the orange cards abilities other than mimic
5. Make the game local multiplayer