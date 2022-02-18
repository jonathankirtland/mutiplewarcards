// megawar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <iomanip>
using namespace std;

/*
allows for construction of cards
*/
class Card
{
public:

	enum Rank { Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };
	enum Suit { Hearts, Diamonds, Spades, Clubs };

	Card()
	{
	}
	Card(const Rank rank, const Suit suit) { myRank = rank; mySuit = suit; };
	~Card()
	{
	}
	int getRank()
	{
		return myRank;
	}

	void display()
	{
		switch (myRank)
		{
		case Card::Ace:
			cout << "A of " << displaySuits();
			break;
		case Card::Jack:
			cout << "J of " << displaySuits();
			break;
		case Card::Queen:
			cout << "Q of " << displaySuits();
			break;
		case Card::King:
			cout << "K of " << displaySuits();
			break;
		default:
			cout << myRank << " of " << displaySuits();
			break;
		}
	}

private:
	Suit mySuit;
	Rank myRank;

	//enum names annoying
	string displaySuits()
	{
		switch (mySuit)
		{
		case Hearts:
			return "Hearts";
			break;
		case Diamonds:
			return "Diamonds";
			break;
		case Spades:
			return "Spades";
			break;
		case Clubs:
			return "CLubs";
			break;
		}
	}
};

/*
parent class for card storage
*/
class CardPile
{
public: vector<Card> pile;

public:
	virtual void addCard(Card c)
	{
		pile.insert(pile.begin(), c);
	}
	virtual Card removeCard()
	{
		Card temp = pile[pile.size() - 1];
		pile.pop_back();
		return temp;
	}
};

/*
class for storing cards
*/
class Deck : public CardPile
{
public:
	Deck()
	{
		populate();
	}
	void populate()
	{
		for (int i = 1; i < 14; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Card card(static_cast<Card::Rank>(i), static_cast<Card::Suit>(j));
				addCard(card);
			}
		}
	}

	Card deal()
	{
		return removeCard();
	}

	/*
	actually random shuffling algo
	*/
	void shuffle()
	{
		srand(2);

		for (int i = 0; i < pile.size(); i++)
		{
			int r = i + (rand() % (pile.size() - i));

			std::swap(pile[i], pile[r]);
		}
	}

	void displayDeck()
	{
		for (auto it = pile.begin(); it != pile.end(); ++it)
		{
			(*it).display();
			cout << endl;
		}
	}
};

/*
allows for the use of multiple decks
*/
class MegaDeck : public Deck
{
public:
	//create n amount of decks
	MegaDeck(int n)
	{
		//i is 1 because deck calls populate by default
		for (int i = 1; i < n; i++)
		{
			populate();
		}
	}
	MegaDeck()
	{
	}
};

/*
a card container class
*/
class LostandFound : public CardPile
{
public:
	LostandFound()
	{
	}

	vector<Card> dump()
	{
		return pile;
	}
};

/*
a card container class
*/
class WarPile : public CardPile
{
public:
	WarPile()
	{
	}
	vector<Card> dump()
	{
		return pile;
	}
};

/*
the player class which also stores cards
*/
class Player : public CardPile
{
public:
	int battles, won;
	Player()
	{
		fierceness = 0;
		battles = 0;
		won = 0;
	}

	//calculates the fierceness of a players hand
	void calcFierce()
	{
		int sum = 0;
		for (auto it = pile.begin(); it != pile.end(); it++)
		{
			sum += (*it).getRank();
		}
		fierceness = 1.0 * sum / pile.size();

		if (fierceness != fierceness)
		{
			fierceness = 0.00;
		}
	}

	//return the fierceness
	double getFierce()
	{
		return fierceness;
	}

	//used to display the player
	void display()
	{
		int width = 10;
		cout << left
			<< setw(width) << "Fierceness : "
			<< setw(width) << setprecision(3) << fierceness
			<< setw(width) << " Cards = "
			<< setw(width) << pile.size()
			<< setw(width) << " Battles: "
			<< setw(10) << battles
			<< setw(width) << " Won: "
			<< setw(width) << won
			<< endl;
	}

private:
	double fierceness;
};

/*
finds and returns the index o0f the max element in an integer vector
*/
int isMax(vector<int> vect)
{
	//find the max element
	vector<int>::iterator i = max_element(vect.begin(), vect.end());

	//return the index
	int r = i - vect.begin();
	return r;
}

/*
checks if there is a war
*/
bool isWar(vector<int> vect)
{
	int count = std::count(vect.begin(), vect.end(), vect[isMax(vect)]);

	if (count > 1)
	{
		return true;
	}
	return false;
}

/*
checks if there is a winner and returns the index of that winner
*/
int isWinner(vector<Player> vect, int decksize, int leftover)
{
	for (int i = 0; i < vect.size(); i++)
	{
		if (vect[i].pile.size() == decksize - leftover)
		{
			return i;
		}
	}
	return -1;
}

int main()
{
	vector<Player> playerList;

	vector<int> cardvals;
	WarPile war;
	LostandFound lf;
	int playercount;
	MegaDeck decks;
	int decksize;
	int leftover;
	int battles = 0;

	cout << "How many players" << endl;
	cin >> playercount;

	for (int i = 0; i < playercount; i++)
	{
		Player p = Player();
		playerList.push_back(p);
	}

	cout << "How many decks" << endl;
	int numdecks;
	cin >> numdecks;
	decks = MegaDeck(numdecks);

	decksize = decks.pile.size();

	//shuffle
	decks.shuffle();

	//assign cards to players
	while (decks.pile.size() >= playercount)
	{
		for (int i = 0; i < playerList.size(); i++)
		{
			Card temp = decks.removeCard();
			playerList[i].addCard(temp);
		}
		leftover = decks.pile.size();
	}

	//while there isnt a winner
	while (isWinner(playerList, decksize, leftover) == -1)
	{
		cout << endl << "Battle " << ++battles << endl;

		for (int i = 0; i < playerList.size(); i++)
		{
			playerList[i].calcFierce();
			cout << "Player " << i + 1 << ": ";
			playerList[i].display();
			//makes sure players have cards to play
			if (playerList[i].pile.size() != 0)
			{
				//increase player battles
				playerList[i].battles++;

				//insert player card into cardval vector and add to warpile
				Card c = playerList[i].removeCard();
				cardvals.push_back(c.getRank());
				war.addCard(c);
			}
			else
			{
				//if a player is out add 0 to the cardvals
				cardvals.push_back(0);
			}
		}
		while (isWar(cardvals))
		{
			//for all players
			for (int i = 0; i < playerList.size(); i++)
			{
				//check to make sure we dont go out of bounds
				if (playerList[i].pile.size() != 0)
				{
					//makes it easier
					int psize = playerList[i].pile.size(), msize = playerList[isMax(cardvals)].pile.size();

					int max = isMax(cardvals);
					//check if player has war card so players who dont have it dont participate
					if (playerList[i].pile[psize - 1].getRank() == playerList[max].pile[msize - 1].getRank())
					{
						//if player is in the war, but doesnt have enough cards for war
						if (playerList[i].pile.size() < 4)
						{
							//add hand to lost and found
							lf.pile.insert(lf.pile.end(), playerList[i].pile.begin(), playerList[i].pile.end());

							//empty hand because player loses
							playerList[i].pile.clear();
							cardvals[i] = 0;
						}
						else
						{
							//for loop wouldnt work for some odd reason
							war.addCard(playerList[i].removeCard());
							war.addCard(playerList[i].removeCard());
							war.addCard(playerList[i].removeCard());

							//fourth card is played and added to cardvals and warpile
							Card c = playerList[i].removeCard();
							cardvals[i] = c.getRank();
							war.addCard(c);

							//using current cardvals check for double war
						}
					}
				}
				else
				{
					cardvals[i] = 0;
				}
			}
		}

		//get the index of the max card, which will be the winning card
		int ind = isMax(cardvals);

		//add war pile
		playerList[ind].pile.insert(playerList[ind].pile.begin(), war.pile.begin(), war.pile.end());
		//add lost and found pile
		playerList[ind].pile.insert(playerList[ind].pile.begin(), lf.pile.begin(), lf.pile.end());
		//increment battles won
		playerList[ind].won++;

		//clear all the piles for next battle
		lf.pile.clear();
		war.pile.clear();
		cardvals.clear();
	}

	cout << endl << "Battle " << ++battles << endl;

	for (int i = 0; i < playerList.size(); i++)
	{
		playerList[i].calcFierce();
		cout << "Player " << i + 1 << ": ";
		playerList[i].display();
	}

	cout << endl << "Player " << isWinner(playerList, decksize, leftover) + 1 << " wins" << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file