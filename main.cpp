#include <iostream>
#include <map>
#include <random>
#include <vector>
#include <algorithm>

using CardValue = int;
using CardSuit = std::string;
using Card = std::pair<CardValue, CardSuit>;

using Deck = std::vector<Card>;
using Hand = std::vector<Card>;

const std::map<CardValue, std::string> CARD_VALUES = {
        {1,  "Ace"},
        {2,  "Two"},
        {3,  "Three"},
        {4,  "Four"},
        {5,  "Five"},
        {6,  "Six"},
        {7,  "Seven"},
        {8,  "Eight"},
        {9,  "Nine"},
        {10, "Ten"},
        {11, "Jack"},
        {12, "Queen"},
        {13, "King"}
};

const std::vector<CardSuit> CARD_SUITS = {
        "Hearts",
        "Diamonds",
        "Clubs",
        "Spades"
};

Deck createDeck()
{
    Deck deck;
    for (const auto &suit: CARD_SUITS)
    {
        for (const auto &[value, name]: CARD_VALUES)
        {
            deck.push_back(std::make_pair(value, suit));
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    return deck;
}

int getCardValue(const Card &card)
{
    const auto &[value, suit] = card;
    if (value >= 10)
    {
        return 10;
    } else if (value == 1)
    {
        return 11;
    } else
    {
        return value;
    }
}

int getHandValue(const Hand &hand)
{
    int total = 0;
    int aces = 0;
    for (const auto &card: hand)
    {
        const auto &[value, suit] = card;
        if (value == 1)
        {
            aces++;
        }
        total += getCardValue(card);
    }
    while (total > 21 && aces > 0)
    {
        total -= 10;
        aces--;
    }
    return total;
}

Hand dealCards(Deck &deck, int numCards)
{
    Hand hand;
    for (int i = 0; i < numCards; i++)
    {
        hand.push_back(deck.back());
        deck.pop_back();
    }
    return hand;
}

void printCard(const Card &card)
{
    const auto &[value, suit] = card;
    std::cout << CARD_VALUES.at(value) << " of " << suit << std::endl;
}

void printHand(const Hand &hand, bool isDealer = false)
{
    std::cout << (isDealer ? "Dealer's hand: " : "Your hand: ") << std::endl;
    for (size_t i = 0; i < hand.size(); i++)
    {
        if (isDealer && i == 0)
        {
            std::cout << "Unknown card" << std::endl;
        } else
        {
            printCard(hand[i]);
        }
    }
    std::cout << "Total value: " << getHandValue(hand) << std::endl;
}

bool shouldDealerHit(const Hand &hand)
{
    return getHandValue(hand) < 16;
}

bool shouldPlayerHit()
{
    std::cout << "Do you want to hit or stand? (h/s) ";
    char choice;
    std::cin >> choice;
    while (choice != 'h' && choice != 's')
    {
        std::cout << "Invalid choice. Please enter h or s: ";
        std::cin >> choice;
    }
    return choice == 'h';
}

void playGame()
{
    Deck deck = createDeck();
    Hand playerHand = dealCards(deck, 2);
    Hand dealerHand = dealCards(deck, 2);

    std::cout << "Welcome to Blackjack!" << std::endl;

    while (true)
    {
        printHand(playerHand);

        if (getHandValue(playerHand) > 21)
        {
            std::cout << "Bust! You lose." << std::endl;
            return;
        }

        if (!shouldPlayerHit())
        {
            break;
        }

        playerHand.push_back(deck.back());
        deck.pop_back();
    }

    while (shouldDealerHit(dealerHand))
    {
        dealerHand.push_back(deck.back());
        deck.pop_back();
    }

    std::cout << std::endl;
    printHand(playerHand);
    printHand(dealerHand, true);

    int playerValue = getHandValue(playerHand);
    int dealerValue = getHandValue(dealerHand);

    if (dealerValue > 21)
    {
        std::cout << "Dealer busts! You win." << std::endl;
    } else if (playerValue > dealerValue)
    {
        std::cout << "You win!" << std::endl;
    } else if (dealerValue > playerValue)
    {
        std::cout << "Dealer wins. You lose." << std::endl;
    } else
    {
        std::cout << "It's a tie!" << std::endl;
    }
}

int main()
{
    playGame();
    return 0;
}

