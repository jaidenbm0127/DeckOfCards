#include <iostream>
#include <string>
#include <vector> 
#include "rngs.h"
#include "rngs.c" 
#include <algorithm>
#include <map>
 
using namespace std;
 
 
/**********************************\
 **  These two enums are for       *
 **  holding the rank and suit     *
 **     (obviously)                *
\**********************************/ 
 
enum Rank{
    Ace,
    Two,
    Three,
    Four,
    Five, 
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    LastRank
};
 
enum Suit{
    Spades,
    Hearts,
    Diamonds,
    Clubs,
    LastSuit
};
 
 
// These char * arrays are used in printing the Card so that switch statements arent needed to be hard coded. Since the enums are
// continuous you can pretty much use the Rank/Suit as an index and use these two lists as reference for printing.
static const char * RankStrings[] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
static const char * SuitStrings[] = { "Spades", "Hearts", "Diamonds", "Clubs" };
 
/****************************************\
 **   Card class. Uses the enum to keep  *
 **   track of Suit/Rank, and the card   *
 **   is able to be printed for testing  *
 **   purposes. And some getters         *
\****************************************/ 
 
class Card{
    private:
 
        Suit suit;
        Rank rank;
    
    public:
 
        Card(Suit cardSuit, Rank cardRank){
            suit = cardSuit;
            rank = cardRank;
        }
 
        Suit getSuit(){
            return suit;
        }
 
        Rank getRank(){
            return rank;
        }
 
        // Utilizes the char * array instead of hardcoding a switch statement.
        void printCard(){
            cout << RankStrings[rank] << " of " << SuitStrings[suit];
        }
};
 
/********************************************************************************************\
 **   The bulk of the program is the deck class. There is a lot that is going on here.       *
 **   For one, the deck contains the deck of cards, the hand, the hand size, iterations      *
 **   and specialSum selected. The hand could've been its own class, but putting             *
 **   the deck of cards and hand vector into one class makes things easier, as you can pop   *
 **   the deck and add to hand while being able to automatically refill the deck.            *
\********************************************************************************************/ 
 
class Deck{
 
    private: 
        vector<Card> deckOfCards;
 
        vector<Card> hand;
 
        int handSize;
 
        double iterations;
 
        int specialSum;
 
    public:
 
        // Constructor
        Deck(int h, int i, int s){
            vector<Card> deck;
 
            for(Suit i = Spades; i < LastSuit; i = Suit(i+1)){
                for(Rank j = Ace; j < LastRank; j = Rank(j+1)){
                    Suit suit = i;
                    Rank rank = j;
 
                    Card tempCard(suit, rank);
 
                    deck.push_back(tempCard);
                    
                }
            }
 
            deckOfCards = deck;
 
            handSize = h;
            iterations = i;
            specialSum = s;
        }
 
        // Does the same thing the constructor does, but is necessary for making new hands once the deck runs out of cards.
        void makeNewDeck(){
            vector<Card> deck;
 
            for(Suit i = Spades; i < LastSuit; i = Suit(i+1)){
                for(Rank j = Ace; j < LastRank; j = Rank(j+1)){
                    Suit suit = i;
                    Rank rank = j;
 
                    Card tempCard(suit, rank);
 
                    deck.push_back(tempCard);
                    
                }
            }
 
            deckOfCards = deck;
        }
 
        void printDeck(){
            for(Card card : deckOfCards){
                card.printCard();
                cout << endl;
            }
        }
 
 
        // Uses our Random() to iterate 52 times through deck and swap items to truly randomize deck. 
        void shuffle(){
            for(int i = 0; i < 52; i++){
 
                int r = i + int(Random() * (52 - i));
 
                iter_swap(deckOfCards.begin() + r, deckOfCards.begin() + i);
 
            }
        }
 
 
        // Uses recursion to make a completely self fulfilling dealHand function. Since my method of dealing cards
        // Pops them off of the deck to add to the hand, the first if statement makes sure that the deck of cards has 
        // enough cards to fill the hand. If it doesnt, it calls the makeNewDeck function, shuffles, and calls
        // dealHand recursively.
 
        // If the hand is empty, it pushes back the 5/7 cards at the back of the deck, then pops those cards from
        // the deck so they're not used again.
 
        // If hand is not empty, it empties the hand vector and calls dealHand recursively again.
        void dealHand(){
 
            if(deckOfCards.size() < handSize){
                makeNewDeck();
 
                shuffle();
 
                dealHand();
            }
 
            if(hand.empty()){
                for(int i = 0; i < handSize; i++){
                    hand.push_back(deckOfCards.back());
                    deckOfCards.pop_back();
                }
            }
            else{
                for(int i = 0; i < handSize; i++)
                    hand.pop_back();
                dealHand();
            }
        }
 
        void printHand(){
            for(Card card : hand){
                card.printCard();
                cout << endl;
            }
        }
 
        // Uses the ranks vector to check if there is only a 3 of a kind (NOT 4 of a kind)
        bool threeOfAKind(){
            vector<int> ranks(13);
 
            for(Card card : hand){
                ranks.at(card.getRank())++;
            }
 
            for(int i : ranks){
                if(i >= 4)
                    return false;
                else if(i == 3)
                    return true;
            }
 
            return false;
        }
 
 
        // Uses the ranks vector to see if there's 4 of one Rank in the hand. 
        bool fourOfAKind(){
            vector<int> ranks(13);
 
            for(Card card : hand){
                ranks.at(card.getRank())++;
            }
 
            for(int i : ranks){
                if(i == 4)
                    return true;
            }
 
            return false;
        }
 
        // Probably the most complex function in regards to hand types. Basically uses a vector of the ranks
        // to first check whether it found 3 of the same number. If it did, it marks that as true and restarts
        // the search through the vector to see if it found 2 or more of the same number. If both are true,
        // it returns true. 
        bool fullHouse(){
            vector<int> ranks(13);
 
            bool foundThreeOfSameRank = false;
            bool foundTwoOfSameRank = false; 
 
            for(Card card : hand){
                ranks.at(card.getRank())++;
            }
 
            for(int i = 0; i < ranks.size(); i++){
                if(ranks.at(i) >= 3 && !foundThreeOfSameRank){
                    foundThreeOfSameRank = true; 
                    ranks.erase(ranks.begin() + i);
                    i = 0;
                    continue;
                }
                else if(ranks.at(i) >= 2 && foundThreeOfSameRank)
                    foundTwoOfSameRank = true;
            }
 
            if(foundThreeOfSameRank && foundTwoOfSameRank)
                return true;
            else
                return false;
        }
 
 
        // Similar to the four of a kind but with suits instead. 
        bool flush(){
            vector<int> suits(4);
 
            for(Card card : hand){
                suits.at(card.getSuit())++;
            }
 
            for(int i : suits){
                if(i >= 5)
                    return true;
            }
 
            return false;
        }
 
        // Checks each Rank in the ranks vector and sees if there is at least one of the next four cards in 
        // front of it. Stops at 8 to make sure that it doesnt get an index out of bounds error.
        bool straight(){
            vector<int> ranks(13);
 
            for(Card card : hand){
                ranks.at(card.getRank())++;
            }
 
            for(int i = 0; i < ranks.size(); i++){
               if(ranks.at(i) >= 1){
                   if(i <= 8){
                        if(ranks.at(i+1) >=1 && ranks.at(i+2) >=1 && ranks.at(i+3) >=1 && ranks.at(i+4) >=1)
                            return true;
                   }
               }
            }
 
            return false;
        }
        
        // Just adds up all the cards by quantity of each rank. Then checks if runningTotal equals specialSum.
        bool sum(){
            vector<int> ranks(13);
 
            int runningTotal;
 
            for(Card card : hand){
                ranks.at(card.getRank())++;
            }
 
            for(int i = 0; i < ranks.size(); i++){
                runningTotal += (i+1)*ranks.at(i);
            }
 
            if(runningTotal == specialSum)
                return true;
 
            return false;
        }
 
        void printStatistics(vector<int> counts){
            cout << "POKER HAND PROBABILITY WITH " << handSize << " CARDS AND " << iterations << " ITERATIONS" << endl;
            cout << "POKER HANDS  " << "EXPECTED %  " <<   "ACTUAL %" << endl;
            cout << "-----------  " << "---------  " << "--------" << endl;
            cout << "4 OF A KIND  " << "0.02401%   " << counts.at(1)/iterations*100 << "%" << endl;
            cout << "3 OF A KIND  " << "2.1128%    " << counts.at(0)/iterations*100 << "%" << endl;
            cout << "FULL HOUSE   " << "0.1441%    " << counts.at(2)/iterations*100 << "%" << endl;
            cout << "FLUSH        " << "0.1965%    " << counts.at(3)/iterations*100 << "%" << endl;
            cout << "STRAIGHT     " << "0.3925%    " << counts.at(4)/iterations*100 << "%" << endl;
            cout << "SUM " << specialSum << "       " << "DO MATH    " << counts.at(5)/iterations*100 << "%" << endl;
        }
 
 
        // Checks each hand for each hand type for a number of hands equal to the iterations entered.
        void processHands(){
            vector<int> counts(6);
            for(int i = 0; i < iterations; i++){
                dealHand();
                if(threeOfAKind()){
                    counts.at(0)++;
                } 
                if(fourOfAKind()){
                    counts.at(1)++;
                }
                if(fullHouse()){
                    counts.at(2)++;
                }
                if(flush()){
                    counts.at(3)++;
                }
                if(straight()){
                    counts.at(4)++;
                }
                if(sum()){
                    counts.at(5)++;
                }
            }
            printStatistics(counts);
        }
};
 
 
int main(int argc,char* argv[]){
 
    Deck deck(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));
 
    deck.shuffle();
 
    deck.processHands();
 
}
