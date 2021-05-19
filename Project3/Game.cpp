// Game.cpp

#include "provided.h"
#include <iostream>
#include <string>
using namespace std;

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black);
    ~GameImpl();
    GameImpl &operator=(const GameImpl& rhs);
    GameImpl(const GameImpl& other);
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
private:
    Scaffold* m_grid;
    int m_N;
    Player* Player1;
    Player* Player2;
    Player* currentPlayer;
};

/*
 Construct a Game to be played with the indicated players on a scaffold of the indicated size. The red player always moves first. The goal of the game is for a player to get N of their checkers in a row.
 */
GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_grid = new Scaffold(nColumns, nLevels);
    m_N = N;
    Player1 = red;
    Player2 = black;
    currentPlayer = Player1;
}

GameImpl::~GameImpl() {
    delete m_grid;
}

GameImpl&GameImpl::operator=(const GameImpl& rhs) {
    delete m_grid;
    m_grid = new Scaffold(rhs.m_grid->cols(), rhs.m_grid->levels());
    m_N = rhs.m_N;
    Player1 = rhs.Player1;
    Player2 = rhs.Player2;
    currentPlayer = rhs.currentPlayer;
    return *this;
}
GameImpl::GameImpl(const GameImpl& other) {
    m_grid = new Scaffold(other.m_grid->cols(), other.m_grid->levels());
    m_N = other.m_N;
    Player1 = other.Player1;
    Player2 = other.Player2;
    currentPlayer = other.currentPlayer;
}

/*
 If the game isn't over (i.e., more moves are possible), return false and do not change winner. Otherwise, set winner to RED, BLACK, or TIE_GAME, reflecting the outcome of the game, and return true.
 */
bool GameImpl::completed(int& winner) const
{
    if (m_grid->numberEmpty() == 0) {
        winner = TIE_GAME;
        return true;
    }
    for (int i = 1; i <= m_grid->cols(); i++) {
        for (int j = 1; j <= m_grid->levels(); j++) {
            if (m_grid->checkerAt(i, j) != VACANT) {
                int checker = m_grid->checkerAt(i, j);
                int counter = 1;
                // check left side
                int col = i-1;
                int level = j;
                while (col > 0 && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    col--;
                }
                // check right side
                col = i+1;
                while (col <= m_grid->cols() && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    col++;
                }
                if (counter >= m_N) {
                    winner = checker;
                    return true;
                }
                // reset counter, check other directions
                counter = 1;
                // checking verticals
                col = i;
                level = j - 1;
                // check downwards
                while (level > 0 && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    level--;
                }
                // check upwards
                level = j + 1;
                while ( level <= m_grid->levels() && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    level++;
                }
                if (counter >= m_N) {
                    winner = checker;
                    return true;
                }
                // reset counter, check other directions
                counter = 1;
                // check diagonal top left to bottom right
                // check top left
                level = j-1;
                col = i-1;
                while ( col > 0 && level > 0 && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    level--;
                    col--;
                }
                // check bottom right
                level = j + 1;
                col = i + 1;
                while ( level <= m_grid->levels() && col <= m_grid->cols() && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    level++;
                    col++;
                }
                if (counter >= m_N) {
                    winner = checker;
                    return true;
                }
                // reset counter, check other directions
                counter = 1;
                // check diagnonal top right to bottom left
                // check top right
                level = j-1;
                col = i+1;
                while ( level > 0 && col <= m_grid->cols() && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    level--;
                    col++;
                }
                // check bottom left
                level = j+1;
                col = i-1;
                while ( level <= m_grid->levels() && col > 0 && m_grid->checkerAt(col, level) == checker) {
                    counter++;
                    level++;
                    col--;
                }
                if (counter >= m_N) {
                    winner = checker;
                    return true;
                }
            }
        }
    }
    return false;
}

/*
 If the game is over, return false. Otherwise, make a move for the player whose turn it is (so that it becomes the other player's turn) and return true.
 */
bool GameImpl::takeTurn()
{
    int winner;
    if (completed(winner)) {
        return false;
    }
    if (currentPlayer == Player1) {
        cout << "It is " << Player1->name()  << "'s turn" << endl;
        m_grid->makeMove(Player1->chooseMove(*m_grid, m_N, RED), RED);
        currentPlayer = Player2;
    }
    else if (currentPlayer == Player2) {
        cout << "It is " << Player2->name()  << "'s turn" << endl;
        m_grid->makeMove(Player2->chooseMove(*m_grid, m_N, BLACK), BLACK);
        currentPlayer = Player1;
    }
    return true;
}

/*
 Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore member function for input streams is useful here.) Announce the winner at the end of the game.
 */
void GameImpl::play()
{
    m_grid->display();
    if (Player1 == nullptr || Player2 == nullptr) {
        return;
    }
    int winner;
    while(!completed(winner)) {
        takeTurn();
        m_grid->display();
        cout << "Press ENTER to continue.";
        cin.ignore(10000,'\n');
    }
    m_grid->display();
    if (winner == RED) {
        cout << "The winner is red" << endl;
    }
    else if (winner == BLACK) {
        cout << "The winner is black" << endl;
    }
    else if (winner == TIE_GAME) {
        cout << "Tie game" << endl;
    }
    else {
        cerr << "Error somewhere" << endl;
    }
}

/*
 In the Game's scaffold, if there is a red checker at the indicated column and level, return RED; if there's a black checker there, return BLACK; otherwise, return VACANT. This function exists so that we and you can more easily test your program; a real client would never use it.
 */
int GameImpl::checkerAt(int c, int r) const
{
    return m_grid->checkerAt(c, r);
} 

//******************** Game functions *******************************

//  These functions simply delegate to GameImpl's functions.
//  You probably don't want to change any of this code. 

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
