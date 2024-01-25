
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <ctime>

using namespace std;


string cards[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
int cardVals[13] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };
bool hasAce = false;

int drawCard();
void blackJackCheck(int, int);
int cardCount(bool, int, int);
void oddsCheck(bool, int);

int main() {
    int c1, c2, cVal1, cVal2, handTotal;
    string userAnswer;
    srand((unsigned)time(0));

    c1 = drawCard();       //drawcard function gives element number
    cout << "Your cards: " << cards[c1] << "  ";
    cVal1 = cardVals[c1];     //from location of string array, set coresponding value from int array
    if (cVal1 == 11)       //check for Ace (set to 11 to begin)
        hasAce = true;   //will be used to evaluate "soft" hands
    cardVals[c1] = 0;
    c2 = drawCard();
    cout << cards[c2] << endl << endl;
    cVal2 = cardVals[c2];
    if (cVal2 == 11)
        hasAce = true;  //only one Ace in usable cards, so no further check necessary
    cardVals[c2] = 0;

    if (hasAce)        //check for Blackjack (21 = automatic win)
        blackJackCheck(cVal1, cVal2);
    handTotal = cardCount(hasAce, cVal1, cVal2);
    oddsCheck(hasAce, handTotal);

    cout << "\n\nWould you like to draw another card?  (press \"y\" for yes, any other key for no) ";
    cin >> userAnswer;
    cout << endl << endl;
    if (userAnswer == "y" || userAnswer == "Y") {
        c1 = drawCard();
        cout << "You drew a " << cards[c1] << endl;
        cVal1 = cardVals[c1];
        if (cVal1 == 11)
            hasAce = true;
        cardVals[c1] = 0;
        handTotal = cardCount(hasAce, cVal1, handTotal);
    }
    else {
        cout << "Stand on " << handTotal << endl;
    }

}

int drawCard() {          //this will take mod to return value from 0-12  
    int val = rand() % 13;         //corresponds to predefined suit and value arrays
    if (cardVals[val] != 0)
        return val;
    else drawCard();      //if card already used, value is 0...so redraw
}
void blackJackCheck(int a, int b) {    //blackjack check only happens on original 2 cards
    if (a + b == 21)            //if bool hasAce is true, only have to check for 10 value
        cout << "BLACKJACK!" << endl;
}
int cardCount(bool ace, int a, int b) {
    int total = a + b;

    if (total == 21) {
        cout << "YOU WIN!  total = 21" << endl;
        cout << "     (you cannot improve this hand!)";
    }
    else if (total > 21) {
        if (!ace) {
            cout << "BUST!    total = " << total << endl;
        }
        else {
            total = total - 10;
            ace = false;
            hasAce = false;              //flip flag on Ace (can't be 11 without busting now)
            cardCount(hasAce, total, 0);
        }
    }
    else {
        if (!ace)
            cout << "Your total is: " << total << endl;
        else
            cout << "Your total is: " << total - 10 << " or " << total << endl;
    }
    return total;
}

void oddsCheck(bool a, int t) {
    int cardTot = 0;
    int advantageCard = 0;
    int disadvantageCard = 0;
    int doubAdv = 0;
    double goodOdds = 0.00;
    double badOdds = 0.00;
    double propOdds = 0.00;

    if (!a) {
        for (int j = 0; j < 13; j++) {
            if (cardVals[j] != 0) {
                cardTot += 1;
                if (cardVals[j] == 11) {
                    if ((t + cardVals[j] < 22) || (t + (cardVals[j] - 10) < 22))
                        advantageCard += 1;
                    else
                        disadvantageCard += 1;
                }
                else {
                    if (t + cardVals[j] < 22)
                        advantageCard += 1;
                    else
                        disadvantageCard += 1;
                }
            }
        }
        goodOdds = static_cast<double>(advantageCard) / (double)(1.0 * cardTot);
        badOdds = static_cast<double>(disadvantageCard) / (double)(1.0 * cardTot);

        cout << "Odds of drawing a card that will bust your hand: ";
        cout << fixed << setprecision(2) << (badOdds * 100) << "%" << endl;
        cout << "Odds of drawing a card that will not bust your hand: ";
        cout << fixed << setprecision(2) << (goodOdds * 100) << "%" << endl;
    }
    else {
        for (int j = 0; j < 13; j++) {
            if (cardVals[j] != 0) {
                cardTot += 1;
                if ((t + cardVals[j] < 22) || (t - 10 + cardVals[j] < 22))  //guaranteed not to bust with soft hand, but this algorithm will
                    advantageCard += 1;                                       //be helpful if program was expanded for 2 hands      
                if (t + cardVals[j] < 22 || ((t - 10) + cardVals[j]) >= t)
                    doubAdv += 1;
            }
        }
        disadvantageCard = cardTot - advantageCard;
        goodOdds = static_cast<double>(advantageCard) / (double)(1.0 * cardTot);
        badOdds = static_cast<double>(disadvantageCard) / (double)(1.0 * cardTot);
        propOdds = static_cast<double>(doubAdv) / (double)(1.0 * cardTot);


        cout << "Odds of drawing a card that will bust your hand: ";
        cout << fixed << setprecision(2) << (badOdds * 100) << "%" << endl;
        cout << "Odds of drawing a card that will not bust your hand: ";
        cout << fixed << setprecision(2) << (goodOdds * 100) << "%" << endl;
        cout << "Odds of drawing a card that will increase your hand (or remain same): ";
        cout << fixed << setprecision(2) << (propOdds * 100) << "%" << endl;
    }
}
