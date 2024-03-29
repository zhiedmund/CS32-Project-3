// main.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

void doScaffoldTests()
{
    Scaffold s1(3, 2);
    s1.makeMove(2, RED);
    s1.makeMove(1, BLACK);
    s1.makeMove(1, RED);
    assert(s1.numberEmpty() == 3); // 3
    s1.display();
    //  |R| | |
    //  |B|R| |
    //  +-+-+-+
    s1.undoMove();
    s1.display();
    //  | | | |
    //  |B|R| |
    //  +-+-+-+
    s1.undoMove();
    s1.display();
    //  | | | |
    //  | |R| |
    //  +-+-+-+
    
    Scaffold s(3, 2);
    assert(s.cols() == 3  &&  s.levels() == 2  && s.numberEmpty() == 6);
    assert(s.makeMove(1, RED));
    assert(s.makeMove(1, BLACK));
    assert(!s.makeMove(1, RED));
    assert(s.numberEmpty() == 4);
    assert(s.checkerAt(1, 1) == RED  &&  s.checkerAt(1, 2) == BLACK);
    assert(s.checkerAt(2, 1) == VACANT);
    s.display();
}

void doPlayerTests()
{
    HumanPlayer hp("Marge");
    assert(hp.name() == "Marge"  &&  hp.isInteractive());
    BadPlayer bp("Homer");
    assert(bp.name() == "Homer"  &&  !bp.isInteractive());
    SmartPlayer sp("Lisa");
    assert(sp.name() == "Lisa"  &&  !sp.isInteractive());
    Scaffold s(3, 2);
    s.makeMove(1, RED);
    s.makeMove(1, BLACK);
    cout << "=========" << endl;
    int n = hp.chooseMove(s, 3, RED);
    cout << "=========" << endl;
    assert(n == 2  ||  n == 3);
    n = bp.chooseMove(s, 3, RED);
    assert(n == 2  ||  n == 3);
    n = sp.chooseMove(s, 3, RED);
    assert(n == 2  ||  n == 3);
}

void doGameTests()
{
    BadPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Game g(2, 2, 2, &bp1, &bp2);
    int winner;
    assert(!g.completed(winner));
    g.takeTurn();  // Red's first move
    assert(!g.completed(winner)  &&
                (g.checkerAt(1, 1) == RED  ||  g.checkerAt(2, 1) == RED));
    g.takeTurn();  // Black's first move
    assert(!g.completed(winner));
    g.takeTurn();  // Red's second move; Red must win
    assert(g.completed(winner)  &&  winner == RED);
}

void playerTests2() {
    Scaffold s1(3, 5);
    SmartPlayer sp1("Computer");
    cout << "Timer start" << endl;
    sp1.chooseMove(s1, 5, RED);
    cout << "Done" << endl;
    Scaffold s2(4, 4);
    sp1.chooseMove(s2, 3, RED);
    cout << "Done" << endl;
    Scaffold s3(5, 3);
    sp1.chooseMove(s3, 2, RED);
    cout << "Done" << endl;
    Scaffold s4(5, 2);
    sp1.chooseMove(s4, 5, RED);
    cout << "Done" << endl;
    Scaffold s5(6, 2);
    sp1.chooseMove(s5, 3, RED);
    cout << "Done" << endl;
    Scaffold s6(10, 2);
    sp1.chooseMove(s6, 2, RED);
    cout << "Done" << endl;
    Scaffold s7(10, 1);
    sp1.chooseMove(s7, 4, RED);
    cout << "Done" << endl;
}

void dynamicAllocationTests() {
    Scaffold s1(4,4); //tests constructor
    s1.makeMove(1, RED);
    Scaffold s2(1,1);
    s2 = s1; //tests assignment operator
    assert(s1.numberEmpty() == s2.numberEmpty());
    Scaffold s3(s2);
    assert(s1.numberEmpty() == 15);
    assert(s3.numberEmpty() == 15);  //checks copy constructor for single node
}

void chooseMoveTest() {
    Scaffold s(7,6);
    SmartPlayer sp("Computer");
    cout << "Timer start" << endl;
    sp.chooseMove(s, 4, RED);
    cout << "Timer end" << endl;
}

int main()
{
    doScaffoldTests();
    doPlayerTests();
    doGameTests();
    playerTests2();
    dynamicAllocationTests();
    chooseMoveTest();


    cerr << "Passed all tests" << endl;
//	string name;
//	cout << "What's your name?  ";
//	getline(cin, name);
//    HumanPlayer hp(name);
//	SmartPlayer sp("Computer");
//	Game g1(4, 3, 3, &hp, &sp);
//	g1.play();
//	cout << endl << "This time I'll go first." << endl;
//	Game g2(4, 3, 3, &sp, &hp);
//	g2.play();
}

    
