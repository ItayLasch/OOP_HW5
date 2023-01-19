#include "OOP5_Part2_TestHeader.h"
#include "GameBoard.h"
#include "RushHour.h"
#include "Printer.h"
void testSolved1()
{
    typedef GameBoard<List<
        List<BoardCell<X, RIGHT, 2>, BoardCell<X, LEFT, 2>, BoardCell<EMPTY, RIGHT, 8>>,
        List<BoardCell<EMPTY, RIGHT, 0>, BoardCell<EMPTY, RIGHT, 0>, BoardCell<A, UP, 1>>>>
        gameBoard;

    Printer<gameBoard>::print(std::cout);
    typedef MoveVehicle<gameBoard, 0, 0, RIGHT, 1>::board b1;
    typedef MoveVehicle<gameBoard, 1, 2, UP, 1>::board b2;

    printf("print B1:\n");
    Printer<b1>::print(std::cout);
    printf("print B2:\n");
    Printer<b2>::print(std::cout);

    typedef MoveVehicle<b1, 0, 2, LEFT, 1>::board b3;
    typedef MoveVehicle<b2, 0, 2, DOWN, 1>::board b4;

    printf("print B3:\n");
    Printer<b3>::print(std::cout);
    printf("print B4:\n");
    Printer<b4>::print(std::cout);
    static_assert(CheckWin<gameBoard>::result, "Fail");
    Printer<Transpose<gameBoard::board>::matrix>::print(std::cout);
}

int main()
{
    testSolved1();
    return 0;
}
