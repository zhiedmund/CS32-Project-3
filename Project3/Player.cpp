// Player.cpp

#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

const int BIGNUMBER = 999999;

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
    int determineBestMove(AlarmClock &ac, Scaffold&s, int N, int color, int depth, int& col);
    bool hasWon(const Scaffold& s, int N, int col, int level);
    vector<int> nextAvailableLevel;
};

/*
 A HumanPlayer chooses its move by prompting a person running the program for a move (reprompting if necessary until the person enters a valid move), and returning that choice. You may assume that the user will enter an integer when prompted for a column number, although it might not be within the range of valid column numbers. (In other words, we won't test HumanPlayer by providing input that is not an integer when an integer is required. The techniques for dealing with the issue completely correctly are a distraction to this project.
 */
int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if(s.numberEmpty() == 0) {
        return 0;
    }
    if (N > s.cols() && N > s.levels()) {
        cerr << "Invalid N" << endl;
        exit(1);
    }
    if (N < 1) {
        cerr << "Invalid N" << endl;
        exit(1);
    }
    if (color != RED && color != BLACK) {
        cerr << "Invalid Color" << endl;
        exit(1);
    }
    cout << "Choose a column to play" << endl;
    bool valid = false;
    int column = 0;
    while (!valid) {
        cin >> column;
        cin.ignore(10000, '\n');
        if (column > s.cols() || column < 1 || s.checkerAt(column, s.levels()) != VACANT) { // if input is out of bounds or there is no available position in the column
            cout << "Invalid column, please try again" << endl;
        }
        else {
            valid = true;
        }
    }
    return column;
}

/*
 A BadPlayer is a computer player that chooses an arbitrary valid column and returns that choice. "Arbitrary" can be what you like (e.g., always the leftmost non-full column, or always the column with the fewest checkers, or a randomly selected non-full column, or whatever) provided the move is legal. The point of this class is to have an easy-to-implement class that at least plays legally.
 */
int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if(s.numberEmpty() == 0) {
        return 0; // board is full, no move to be made
    }
    if (N > s.cols() && N > s.levels()) {
        cerr << "Invalid N" << endl;
        exit(1);
    }
    if (N < 1) {
        cerr << "Invalid N" << endl;
        exit(1);
    }
    if (color != RED && color != BLACK) {
        cerr << "Invalid Color" << endl;
        exit(1);
    }
    // i is the column
    for (int i = 1; i <= s.cols(); i++) {
        for (int j = s.levels(); j > 0; j--) {
            if (s.checkerAt(i, j) == VACANT) {
                // returns the leftmost non-full column
                return i;
            }
        }
    }
    return 0; // no move to be made
}


bool SmartPlayerImpl::hasWon(const Scaffold& s, int N, int col, int level) { // checks who wins
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
    return false; // no N in a row found
}

int SmartPlayerImpl::determineBestMove(AlarmClock& ac, Scaffold& s, int N, int color, int depth, int& col) {
    // keeps track of ratings
    int rating;
    int mostPositive = -BIGNUMBER;
    // searches through each column
    for (int i = 1; i <= s.cols(); i++) {
        // if there are vacant spots in the column
        if (nextAvailableLevel[i-1] <= s.levels() && !ac.timedOut()) {
            // make a move
            s.makeMove(i, color);
            if (hasWon(s, N, i, nextAvailableLevel[i-1])) {
                rating = +(BIGNUMBER - depth);
            }
            else {
                if (s.numberEmpty() == 0) {
                    rating = 0; // tie
                }
                else {
                    // negative because we changed color
                    nextAvailableLevel[i-1]++;
                    rating = -(determineBestMove(ac, s, N, (color == RED) ? BLACK : RED, depth + 1, col));
                    nextAvailableLevel[i-1]--;
                }
            }
            // undo steps
            s.undoMove();
            // if we found a better outcome, replace rating and column with rating and column from better outcome
            if (rating > mostPositive) {
                mostPositive = rating;
                col = i;
            }
        }
        // if time runs out, return what we have
        if(ac.timedOut()) {
            return mostPositive;
        }
    }
    // return what we have
    return mostPositive;
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    // starts alarm clock
    AlarmClock ac(9900);
    // validation
    if (N > s.cols() && N > s.levels()) {
        cerr << "Invalid N" << endl;
        exit(1);
    }
    if (N < 1) {
        cerr << "Invalid N" << endl;
        exit(1);
    }
    if (color != RED && color != BLACK) {
        cerr << "Invalid Color" << endl;
        exit(1);
    }
    // sets the next available level in each column to be the lowest one in each column
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
    // creates a copy to analyze
    Scaffold temp(s);
    int col = 1; // to be returned
    determineBestMove(ac, temp, N, color, 1, col); // rating of a pathway
    if(ac.timedOut()) {
        cerr << "Timed out!" << endl;
    }
    return col;
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
