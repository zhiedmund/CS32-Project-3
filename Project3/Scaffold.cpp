// Scaffold.cpp
#include <iostream>
#include "provided.h"
#include <stack>

using namespace std;

class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels); 
    ~ScaffoldImpl();
    ScaffoldImpl &operator=(const ScaffoldImpl& rhs);
    ScaffoldImpl(const ScaffoldImpl& other);
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();
private:
    int m_col;
    int m_levels;
    int** m_grid;
    int m_empty;
    stack<int> colStack;
    stack<int> rowStack;
};
/*
 Construct a Scaffold with the indicated number of columns and levels. If either is not positive, you may terminate the program after writing a message to cerr.
 */
ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
    if (nColumns < 1 ||  nLevels < 1 ) {
        cerr << "Invalid Dimensions" << endl;
        exit(1);
    }
    m_col = nColumns;
    m_levels = nLevels;
    // initializes dynamically allocated 2D array
    m_grid = new int*[m_levels];
    for (int i = 0; i < m_levels; i++) {
        m_grid[i] = new int[m_col];
    }
    for (int i = 0; i < m_levels; i++) {
        for (int j = 0; j < m_col; j++) {
            m_grid[i][j] = VACANT;
        }
    }
    m_empty = m_col * m_levels;
}

ScaffoldImpl::ScaffoldImpl(const ScaffoldImpl& other) {
    m_col = other.m_col;
    m_levels = other.m_levels;
    m_empty = other.m_empty;
    colStack = other.colStack;
    rowStack = other.rowStack;
    m_grid = new int*[m_levels];
    for (int i = 0; i < m_levels; i++) {
        m_grid[i] = new int[m_col];
    }
    for (int i = 0; i < m_levels; i++) {
        for (int j = 0; j < m_col; j++) {
            m_grid[i][j] = other.m_grid[i][j];
        }
    }
}

ScaffoldImpl::~ScaffoldImpl() {
    for (int i = 0; i < m_levels; i++) {
        delete [] m_grid[i];
    }
    delete [] m_grid;
}


ScaffoldImpl&ScaffoldImpl::operator=(const ScaffoldImpl& rhs) {
    // deletes what we currently have
    for (int i = 0; i < m_levels; i++) {
        delete [] m_grid[i];
    }
    delete [] m_grid;
    // allocates a new array
    m_col = rhs.m_col;
    m_levels = rhs.m_levels;
    m_empty = rhs.m_empty;
    colStack = rhs.colStack;
    rowStack = rhs.rowStack;
    m_grid = new int*[m_levels];
    for (int i = 0; i < m_levels; i++) {
        m_grid[i] = new int[m_col];
    }
    for (int i = 0; i < m_levels; i++) {
        for (int j = 0; j < m_col; j++) {
            m_grid[i][j] = rhs.m_grid[i][j];
        }
    }
    return *this;
}


int ScaffoldImpl::cols() const
{
    return m_col;
}

int ScaffoldImpl::levels() const
{
    return m_levels;
}

int ScaffoldImpl::numberEmpty() const
{
    return m_empty;
}

/*
 If there is a red checker at the indicated column and level, return RED; if there's a black checker there, return BLACK; otherwise, return VACANT.
 */
int ScaffoldImpl::checkerAt(int column, int level) const
{
    if (level > m_levels || column > m_col || level < 1 || column < 1) {
        cerr << "Out of bounds" << endl;
        return VACANT; // out of bounds error
    }
    if (m_grid[level-1][column-1] == RED) {
        return RED;
    }
    else if (m_grid[level-1][column-1] == BLACK) {
        return BLACK;
    }
    else {
        return VACANT;  //  This is not always correct; it's just here to compile
    }
    
}
   
void ScaffoldImpl::display() const
{
    for(int i = m_levels-1; i >= 0; i--) {
        cout << '|';
        for (int j = 0; j < m_col; j++) {
            if (m_grid[i][j] == RED) {
                cout << 'R' << '|';
            }
            else if (m_grid[i][j] == BLACK) {
                cout << 'B' << '|';
            }
            else {
                cout << ' ' << '|';
            }
        }
        cout << endl;
    }
    for(int k = 0; k < m_col; k++) {
        cout << '+' << '-';
    }
    cout << '+' << endl;
}

//If the first parameter is a valid column number with at least one vacant position in that column, and if color is RED or BLACK, drop a checker of the appropriate color into that column and return true. Otherwise, do nothing and return false.
bool ScaffoldImpl::makeMove(int column, int color)
{
    // check validity
    if (column > m_col && column <= 0 && (color != RED && color != BLACK)) {
        return false;
    }
    int j = column-1;
    for(int i = 0; i < m_levels; i++) {
        // edge case done in for loop
        if (m_grid[i][j] == VACANT) {
            m_grid[i][j] = color;
            m_empty--;
            colStack.push(column);
            rowStack.push(i+1);
            return true;
        }
    }
    // grid is full
    return false;
}

int ScaffoldImpl::undoMove()
{
    //Undo the most recently made move that has not already been undone (i.e., remove the checker in the scaffold that was most recently added), and return the number of the column from which that checker was removed. If there are no checkers in the scaffold, do nothing and return 0.
    if (m_empty == m_col * m_levels) {
        return 0;
    }
    if (colStack.empty()) {
        return 0;
    }
    int colToBeRemoved = colStack.top();
    int rowToBeRemoved = rowStack.top();
    m_grid[rowToBeRemoved-1][colToBeRemoved-1] = VACANT;
    colStack.pop();
    rowStack.pop();
    return colToBeRemoved;
}

//******************** Scaffold functions *******************************

//  These functions simply delegate to ScaffoldImpl's functions.
//  You probably don't want to change any of this code. 

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
