#ifndef MOVE_VEHICLE_H
#define MOVE_VEHICLE_H

#include "CellType.h"
#include "Direction.h"
#include "GameBoard.h"
#include "List.h"
#include "Utilities.h"
#include "TransposeList.h"

// ========= MOVE ========= //
template <CellType CT, Direction Dir, int Amount>
struct Move
{
    static_assert(CT == CellType::EMPTY, "cell type can't be empty!");
    constexpr static CellType type = CT;
    constexpr static Direction direction = Dir;
    constexpr static int amount = Amount;
};

/*template <Direction Dir, int Amount>
struct Move<EMPTY, Dir, Amount>
{
    static_assert(false, "cell type can't be empty!");
};*/

// ========= IS_VALID_DIRECTION ========= //

template <Direction D>
struct isValidDirection
{
    constexpr static bool value = (D == LEFT || D == RIGHT);
};

// ========= Get_Last_Index_Of_Car ========= //

template <int I, CellType CT, typename L>
struct GetLastIndexOfCar;

template <int I, CellType CT, typename T, typename... TT>
struct GetLastIndexOfCar<I, CT, List<T, TT...>>
{
    // Find last BoardCell in which car appears.
    constexpr static int idx = ConditionalInteger<CT == T::type, GetLastIndexOfCar<(I + 1), CT, List<TT...>>::idx, I - 1>::value;
};

template <int I, CellType CT>
struct GetLastIndexOfCar<I, CT, List<>>
{
    constexpr static int idx = -1;
};

// ========= IS_LEGAL_MOVE ========= //

template <int Amount, int Index, typename L>
struct isLegalMove;

template <int Amount, int Index, typename... TT>
struct isLegalMove<Amount, Index, List<TT...>>
{
    constexpr static bool islegal = ConditionalInteger<(GetAtIndex<Index, List<TT...>>::value::type == EMPTY), isLegalMove<Amount - 1, Index + 1, List<TT...>>::islegal, false>::value;
};

template <int Index, typename... TT>
struct isLegalMove<0, Index, List<TT...>>
{
    constexpr static bool islegal = true;
};

// ========= MOVE_CAR ========= //

template <int RBound, int LBound, int Amount, typename L>
struct MoveCar;

template <int RBound, int LBound, int Amount, typename... TT>
struct MoveCar<RBound, LBound, Amount, List<TT...>>
{
private:
    typedef typename SetAtIndex<RBound + 1, typename GetAtIndex<RBound, List<TT...>>::value, List<TT...>>::list temp_list;
    typedef typename SetAtIndex<LBound, BoardCell<EMPTY, RIGHT, 0>, temp_list>::list final_list;

public:
    typedef typename MoveCar<RBound + 1, LBound + 1, Amount - 1, final_list>::list list;
};

template <int RBound, int LBound, typename... TT>
struct MoveCar<RBound, LBound, 0, List<TT...>>
{
    typedef List<TT...> list;
};

// ========= MOVE_VEHICLE ========= //

template <typename GB, int R, int C, Direction Dir, int A>
struct MoveVehicle;

template <typename L, int R, int C, Direction Dir, int A>
struct MoveVehicle<GameBoard<L>, R, C, Dir, A>
{
    static_assert((R < GameBoard<L>::length && R >= 0), "Row is out of bounds");
    static_assert((C < GameBoard<L>::width) && C >= 0, "Coloum is out of bounds");
    static_assert((GetAtIndex<C, GetAtIndex<R, L>>::value::type != EMPTY), "Cell is Empty");
    static_assert((isValidDirection<GetAtIndex<C, GetAtIndex<R, L>>::value::direction>::value), "invalid direction");

private:
    typedef typename GetAtIndex<R, L>::value rowList;
    typedef typename ReverseList<rowList>::list reverse_list;
    constexpr static int rowSize = rowList::size;
    constexpr static int lastRight = GetLastIndexOfCar<C, GetAtIndex<C, GetAtIndex<R, L>>::value::type, rowList>::idx;
    constexpr static int lastLeft = rowSize - 1 - GetLastIndexOfCar<(rowSize - C - 1), GetAtIndex<C, rowList>::value::type, reverse_list>::idx;
    static_assert(ConditionalInteger<Dir == RIGHT, isLegalMove<A, lastRight + 1, rowList>::islegal, isLegalMove<A, rowSize - lastLeft - 1, reverse_list>::islegal>::value, "Another car in path");

public:
    typedef typename Conditional<Dir == RIGHT, GameBoard<typename SetAtIndex<R, typename MoveCar<lastRight, lastLeft, A, rowList>::list, L>::list>, GameBoard<typename SetAtIndex<R, typename MoveCar<rowSize - lastLeft - 1, rowSize - lastRight - 1, A, reverse_list>::list, L>::list>>::value board;
};

template <typename L, int R, int C, int A>
struct MoveVehicle<GameBoard<L>, R, C, UP, A>
{
    typedef GameBoard<typename Transpose<typename MoveVehicle<GameBoard<typename Transpose<L>::matrix>, R, C, LEFT, A>::board::board>::matrix> board;
};

template <typename L, int R, int C, int A>
struct MoveVehicle<GameBoard<L>, R, C, DOWN, A>
{
    typedef GameBoard<typename Transpose<typename MoveVehicle<GameBoard<typename Transpose<L>::matrix>, R, C, RIGHT, A>::board::board>::matrix> board;
};

#endif