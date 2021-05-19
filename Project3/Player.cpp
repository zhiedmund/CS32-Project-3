// Player.cpp

#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <queue>
using namespace std;

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
private:
    int determineBestMove(Scaffold&s, int N, int color, int depth, int& col);
    bool hasWon(const Scaffold& s, int N, int col, int level);
    vector<int> nextAvailableLevel;
    AlarmClock* ac;
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if(s.numberEmpty() == 0) {
        return 0;
    }
    cout << "Choose a column to play" << endl;
    int column = 0;
    cin >> column;
    cin.ignore(10000, '\n');
    if (column > s.cols()) {
        return 0; // invalid column number!
    }
    // checks if there is at least one vacant spot in that column
    if (s.checkerAt(column, s.levels()) == VACANT) {
       return column;
    }
    else {
       return 0;
    }
    /*
     A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid move), and returning that choice. You may assume that the user will enter an integer when prompted for a column number, although it might not be within the range of valid column numbers. (In other words, we won't test HumanPlayer by providing input that is not an integer when an integer is required. The techniques for dealing with the issue completely correctly are a distraction to this project.
     */
    return column;
}

/*
 A BadPlayer is a computer player that chooses an arbitrary valid column and returns that choice. "Arbitrary" can be what you like (e.g., always the leftmost non-full column, or always the column with the fewest checkers, or a randomly selected non-full column, or whatever) provided the move is legal. The point of this class is to have an easy-to-implement class that at least plays legally.
 */
int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if(s.numberEmpty() == 0) {
        return 0;
    }
    // i is the column
    for (int i = 1; i <= s.cols(); i++) {
        for (int j = s.levels(); j > 0; j--) {
            if (s.checkerAt(i, j) == VACANT) {
                return i;
            }
        }
    }
    return 0;  //  This is not always correct; it's just here to compile
}


bool SmartPlayerImpl::hasWon(const Scaffold& s, int N, int col, int level) { // returns who wins
//    int color = VACANT; // color who just made the last move
//    int i, j; // temporary variables
//    int level; // level of the grid we are checking, always start at 1, the top
//    for (level = s.levels(); level >= 1; level--) {
//        if (s.checkerAt(col, level) != VACANT) {
//            // topmost piece in that column
//            color = s.checkerAt(col, level);
//            break;
//        }
//    }
    int color = s.checkerAt(col, level); // color at location we are checking
    int counter = 1; // how many are in a row is at least 1
    // check the horizontals
    int j = col - 1;
    int i = level;
    // check left side
    while ( j > 0 && s.checkerAt(j, i) == color) {
        counter++;
        j--;
    }
    // checking the right side
    j = col + 1;
    while ( j <= s.cols() && s.checkerAt(j, i) == color) {
        counter++;
        j++;
    }
    if (counter >= N) {
        return true;
    }
    // reset counter, check other directions
    counter = 1;
    // checking verticals
    j = col;
    i = level - 1;
    // check downwards
    while ( i > 0 && s.checkerAt(j, i) == color) {
        counter++;
        i--;
    }
    // check upwards
    i = level + 1;
    while ( i <= s.levels() && s.checkerAt(j, i) == color) {
        counter++;
        i++;
    }
    if (counter >= N) {
        return true;
    }
    // reset counter, check other directions
    counter = 1;
    // check diagonal top left to bottom right
    // check top left
    i = level-1;
    j = col-1;
    while ( i > 0 && j > 0 && s.checkerAt(j, i) == color) {
        counter++;
        i--;
        j--;
    }
    // check bottom right
    i = level + 1;
    j = col + 1;
    while ( i <= s.levels() && j <= s.cols() && s.checkerAt(j, i) == color) {
        counter++;
        i++;
        j++;
    }
    if (counter >= N) {
        return true;
    }
    // reset counter, check other directions
    counter = 1;
    // check diagnonal top right to bottom left
    // check top right
    i = level-1;
    j = col+1;
    while ( i > 0 && j <= s.cols() && s.checkerAt(j, i) == color) {
        counter++;
        i--;
        j++;
    }
    // check bottom left
    i = level+1;
    j = col-1;
    while ( i <= s.levels() && j >0 && s.checkerAt(j, i) == color) {
        counter++;
        i++;
        j--;
    }
    if (counter >= N) {
        return true;
    }
    return false;
}

int SmartPlayerImpl::determineBestMove(Scaffold& s, int N, int color, int depth, int& col) {
    // keeps track of ratings
//    vector<int> ratings(s.cols());
    int rating;
    // searches through each column
    // most pos & neg ratings
    int mostPositive = -BIGNUMBER;
    for (int i = 1; i <= s.cols(); i++) {
        // if there are vacant spots in the column
        if (nextAvailableLevel[i-1] <= s.levels() && !ac->timedOut()) {
            // make a move
            s.makeMove(i, color);
            if (hasWon(s, N, i, nextAvailableLevel[i-1])) {
//                ratings[i-1] = +(BIGNUMBER - depth);
                rating = +(BIGNUMBER - depth);
            }
            else {
                if (s.numberEmpty() == 0) {
//                    ratings[i-1] = 0; // tie is rating of 0
                    rating = 0;
                }
                else {
                    // negative because we changed color
                    nextAvailableLevel[i-1]++;
//                    ratings[i-1] = -(determineBestMove(s, N, (color == RED) ? BLACK : RED, depth + 1, col));
                    rating = -(determineBestMove(s, N, (color == RED) ? BLACK : RED, depth + 1, col));
                    nextAvailableLevel[i-1]--;
                }
            }
            s.undoMove();
            if (rating > mostPositive) {
                mostPositive = rating;
                col = i;
            }
        }
        if(ac->timedOut()) {
            return mostPositive;
        }
//        else {
//            ratings[i-1] = -BIGNUMBER;
//            rating = -BIGNUMBER;
//        }
    }
   
//    for (int i = 0; i < ratings.size(); i++) {
//        if (ratings[i] == -BIGNUMBER) {
//            continue;
//        }
//        if (ratings[i] > mostPositive) {
//            mostPositive = ratings[i];
//            col = i+1; // column that wins game fastest
//        }
//        else if (mostPositive == 0 && tieGameCol == 0) { // have not found a tie game yet
//            if (ratings[i] == 0) {
//                col = i+1; // tie game
//                tieGameCol = col;
//            }
//            else if (ratings[i] > leastNegative) {
//                leastNegative = ratings[i];
//                col = i+1; // least negative rating is column that delays the game the longest
//            }
//        }
//    }
//    if (depth == 1)  {
//        for (int i = 0;  i < ratings.size(); i++) {
//            cout << ratings[i] << " ";
//        }
//        cout << endl;
//    }
//    if (mostPositive > 0) {
//        return mostPositive;
//    }
//    else if (tieGameCol != 0) {
//        return 0;
//    }
//    else if (leastNegative < 0 ) {
//        return leastNegative;
//    }
//    else {
//        return 0;
//    }
    return mostPositive;
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    // sets the next available level in each column to be the lowest one in each column
    ac = new AlarmClock(9000);
    nextAvailableLevel.resize(s.cols());
    int j;
    for (int i = 0; i < nextAvailableLevel.size(); i++) {
        nextAvailableLevel[i] = s.levels() + 1; // none available
        for (j = 0; j < s.levels(); j++) {
            if (s.checkerAt(i+1, j+1) == VACANT) {
                nextAvailableLevel[i] = j+1;
                break;
            }
        }
    }
    Scaffold temp(s);
    int col = 1; // to be returned
    determineBestMove(temp, N, color, 1, col); // rating of a pathway
    if(ac->timedOut()) {
        cout << "Timed out!" << endl;
    }
    return col;
    /*
     determineBestHumanMove():
     The determineBestHumanMove function iterates through all possible moves that the human can make in response to the last computer trial move.
     For each possible move the human could make (remember, this is a simulated move that the computer is trying out, not a real human player move), the function will:
     Make the move (updating the scaffold appropriately with the new checker)
     Use the rating function to rate the resulting scaffold after the move has been made (to see if the human would have just won, it's a tie, etc.).
     If the rating function indicates that the user just won or the move resulted in a tie, then remember the result of this move (e.g., store each evaluation in a collection for later). Otherwise, call the determineBestComputerMove function (shown above) on the current scaffold and get its return value. Then record the return value of determineBestComputerMove (e.g., store each evaluation in a collection for later).
     Undo the current trial move (removing the checker from the scaffold appropriately)
     The determineBestHumanMove function will then chose the move that results in the scaffold with the minimum value (i.e. a value of −1, if possible, indicating a win for the human player). If there is more than one possible move with the highest rating, then the function can choose whichever of these moves is most convenient. Notice that while the computer function always wants to choose the move with the highest value, the human function wants to choose the move with the lowest value, which indicates a win for the human, rather than the computer.
     The function then returns two numbers: (a) one indicating which move would likely be made on behalf of the human, and (b) a number (1, 0, or −1) that indicates the worst possible scaffold score (from the computer's perspective) that the suggested move will eventually result in (since that's what the player would try to do).
     */
    
    return 0;  //  This is not always correct; it's just here to compile
}

//******************** Player derived class functions *************************

//  These functions simply delegate to the Impl classes' functions.
//  You probably don't want to change any of this code. 

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}
 
HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}
 
int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
