#include "OOP5_Part2_TestHeader.h"
#include "GameBoard.h"
#include "RushHour.h"

void testSolved1()
{
    typedef GameBoard<List<
        List<BoardCell<X, RIGHT, 2>, BoardCell<X, LEFT, 2>, BoardCell<EMPTY, RIGHT, 8>>,
        List<BoardCell<EMPTY, RIGHT, 0>, BoardCell<EMPTY, RIGHT, 0>, BoardCell<A, RIGHT, 1>>>>
        gameBoard;

    Printer<gameBoard>::print(std::cout);
    // static_assert(CheckWin<gameBoard>::result, "Fail");
}

int main()
{
    testSolved1();
    return 0;
}
