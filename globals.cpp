/*
 * Eric Ryan Montgomery
 * 09/21/2025
 * For CardGameUI
 * Global Variables/Structs/Class are written here
 */
#include "globals.h"

string gamerules = "Game Rules:"
		"\n\n1: Each player gets a user defined amount of cards referred to as a Deck."
		"\n\n2: Before play begins, each Deck is shuffled and hidden from the player."
		"\n\n3: Each player then takes the first and last Card from their Deck and puts "
			"those cards into play. The players first Card is considered their main Card "
			"and the players last Card is considered their support Card."
		"\n\n4: A players main Card is always the Card that is taking damage or debuffs from "
			"the opponent. The support Card can not be targeted by the opponent."
		"\n\n5: A randomly selected player goes first and has the "
			"option to attack, cast a spell, swap their main Card out for their support Card, charge up "
			"their Card, use their flask, or print the gamerules."
		"\n\n6: If a Card's health or power falls below one, the round is ended and the "
			"winning player recieves a point. Both main cards currently in play are then "
			"discarded and each player's next Card is played as their main. All support cards remain in "
			"play upon a rounds end. The player who lost the last round gets to go first on the next round."
		"\n\n7: Upon attacking, the Card in play has a 5% chance to either land a "
			"critical hit for 20% more damage or miss their attack entirely. A cards base damage is always that "
			"cards amount of physical power."
		"\n\n8: Upon casting a spell, the Card in play will cast their chosen spell at the opponent for various effects. "
			"In general, the spells power will be a random value between one and the cards magical power."
		"\n\n9: Upon swapping, the player swaps their main Card out for their support Card/ "
			"Swapping can only be done twice per round and can not be "
			"done on the last round of the game."
		"\n\n10: Upon charging up, the players Card receives an increase to their health and "
			"power. If a Card powers up twice, the Card uses their special ability."
		"\n\n11: Upon using the flask, the players Card will be fully healed. The "
			"flask does not use up the players turn and they may take another action. "
			"However, the flask may only be used twice per game."
		"\n\n12: Upon printing the gamerules, the gamerules will be displayed on screen. "
			"Printing the gamerules does not use up the players turn and they may take "
			"another action.";
			
string skills = "card Abilities:"
		"\n\n-Pure Mimicry: If the user's color is the same as the opponents color or if either Card is colored white, the user copies the opponents Card exactly."
		"\n\n-Discolored Mimicry: If the user's colors is not the same as the opponents color and neither Card is colored white, the user copies the opponents Card as if it were newly created."
		"\n\n-Heavy Handed: Increases the users power."
		"\n\n-Faithful: Increases users critical hit chance."
		"\n\n-Fast Hand: Gives the user an extra action."
		"\n\n-Healthy Mind: Increases the users health."
		"\n\n-Terror: Decreases the opponents power."
		"\n\n-Resistant: Increases the users physicalResistance."
		"\n\n-Magic Immunity: The user becomes completely immune to all magical attacks"
		"\n\n-Holy Presence: Charges up the next Card in the users Deck by one if the next Card is not already charged."
		"\n\n-Evil Presence: Charges down the next Card in the opponents Deck by one if the next Card is not already charged."
		"\n\n-Accurate: Increases the users critical hit chance and decreases the users miss chance."
		"\n\n-Inner Gate: The user can only either critical hit or miss an attack."
		"\n\n-Necromancy: Revives the most powerful fallen Card in the users Deck and puts that Card into play. If there are no fallen cards in the users Deck then the user gains power instead."
		"\n\n-Divine Help: The gods strike the users opponent."
		"\n\n-Nullify: If the opponent has not used their ability yet, the user will prevent the opponent from ever using their ability. Otherwise, the user will attempt to reverse the oppponents ability."
		"\n\n-Made in Heaven: The user will reset the opponents charge back to zero. Does not affect already used abilities."
		"\n\n-Chaos: The user will use a random ability."
		"\n\n-Strategic Fire: The user will pick an ability depending on various game factors."
		"\n\n\nCard Spells:"
		"\n\n-Force: Attack the opponent for a random value between one and the users magical power"
		"\n\n-Drain: Attacks the opponent power for a random value between one and the users magical power divided by two"
		"\n\n-Weaken: Reduces the opponents resistance for a random percentage between one and the user magical power"
		"\n\n-Heal: Heals the caster for a random value between one and the users magical power.";

Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
	return (Vector2){lhs.x + rhs.x, lhs.y + rhs.y};
}

// Free function to simulate overloading *
Rectangle operator*(const Rectangle& lhs, const int& rhs) {
    return (Rectangle){lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs};
}

// Free function to simulate overloading *
Rectangle operator*(const Rectangle& lhs, const float& rhs) {
    return (Rectangle){lhs.x * rhs, lhs.y * rhs, lhs.width * rhs, lhs.height * rhs};
}

// Free function to simulate overloading *
Vector2 operator*(const Vector2& lhs, const int& rhs) {
    return (Vector2){lhs.x * rhs, lhs.y * rhs};
}

// Free function to simulate overloading *
Vector2 operator*(const Vector2& lhs, const float& rhs) {
    return (Vector2){lhs.x * rhs, lhs.y * rhs};
}

// Free function to simulate overloading ==
bool operator==(const Vector2& lhs, const Vector2& rhs) {
    if (lhs.x == rhs.x && lhs.y == rhs.y) return true;
    return false;
}

// Free function to simulate overloading !=
bool operator!=(const Vector2& lhs, const Vector2& rhs) {
    if (lhs == rhs) return false;
    return true;
}
