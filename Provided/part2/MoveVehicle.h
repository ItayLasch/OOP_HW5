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
    static_assert(CT != CellType::EMPTY, "cell type can't be empty!");
    constexpr static CellType type = CT;
    constexpr static Direction direction = Dir;
    constexpr static int amount = Amount;
};

// ========= IS_VALID_DIRECTION ========= //

template <Direction D1, Direction D2>
struct isValidDirection
{
    constexpr static bool value = ConditionalInteger<(D1 == RIGHT || D1 == LEFT), (D2 == RIGHT || D2 == LEFT), (D2 == UP || D2 == DOWN)>::value;
};

// ========= Get_Last_Index_Of_Car ========= //

template <int I, CellType CT, typename L>
struct GetLastIndexOfCarAux;

template <int I, CellType CT, typename T, typename... TT>
struct GetLastIndexOfCarAux<I, CT, List<T, TT...>>
{
    // Find last BoardCell in which car appears.
    constexpr static int idx = ConditionalInteger<CT == T::type, GetLastIndexOfCarAux<(I + 1), CT, List<TT...>>::idx, I - 1>::value;
};

template <int I, CellType CT>
struct GetLastIndexOfCarAux<I, CT, List<>>
{
    constexpr static int idx = I - 1;
};

template <int I, int Index, CellType CT, typename L>
struct GetLastIndexOfCar;

template <int I, int Index, CellType CT, typename T, typename... TT>
struct GetLastIndexOfCar<I, Index, CT, List<T, TT...>>
{
    constexpr static int idx = GetLastIndexOfCar<I - 1, Index, CT, List<TT...>>::idx;
};

template <int Index, CellType CT, typename T, typename... TT>
struct GetLastIndexOfCar<0, Index, CT, List<T, TT...>>
{
    constexpr static int idx = GetLastIndexOfCarAux<Index, CT, List<T, TT...>>::idx;
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
struct MoveVehicleAux;

template <typename L, int R, int C, int A>
struct MoveVehicleAux<GameBoard<L>, R, C, RIGHT, A>
{
private:
    typedef typename GetAtIndex<R, L>::value rowList;
    typedef typename ReverseList<rowList>::list reverse_list;
    constexpr static CellType car_type = GetAtIndex<C, rowList>::value::type;
    constexpr static int rowSize = rowList::size;
    constexpr static int lastRight = GetLastIndexOfCar<C, C, GetAtIndex<C, typename GetAtIndex<R, L>::value>::value::type, rowList>::idx;
    constexpr static int lastLeft = rowSize - 1 - GetLastIndexOfCar<(rowSize - C - 1), (rowSize - C - 1), car_type, reverse_list>::idx;
    static_assert(isLegalMove<A, lastRight + 1, rowList>::islegal, "Another car in path");

public:
    typedef GameBoard<typename SetAtIndex<R, typename MoveCar<lastRight, lastLeft, A, rowList>::list, L>::list> board;
};

template <typename L, int R, int C, int A>
struct MoveVehicleAux<GameBoard<L>, R, C, LEFT, A>
{
private:
    typedef typename GetAtIndex<R, L>::value rowList;
    typedef typename ReverseList<rowList>::list reverse_list;
    constexpr static int rowSize = rowList::size;
    typedef typename SetAtIndex<R, reverse_list, L>::list templist;

    typedef typename MoveVehicleAux<GameBoard<templist>, R, (rowSize - C - 1), RIGHT, A>::board::board reversed_updated_board;
    typedef typename GetAtIndex<R, reversed_updated_board>::value reversed_rowList;
    typedef typename ReverseList<reversed_rowList>::list updated_list;
    typedef typename SetAtIndex<R, updated_list, reversed_updated_board>::list newlist;

public:
    typedef GameBoard<newlist> board;
};

template <typename L, int R, int C, int A>
struct MoveVehicleAux<GameBoard<L>, R, C, UP, A>
{
    typedef GameBoard<typename Transpose<typename MoveVehicleAux<GameBoard<typename Transpose<L>::matrix>, C, R, LEFT, A>::board::board>::matrix> board;
};

template <typename L, int R, int C, int A>
struct MoveVehicleAux<GameBoard<L>, R, C, DOWN, A>
{
    typedef GameBoard<typename Transpose<typename MoveVehicleAux<GameBoard<typename Transpose<L>::matrix>, C, R, RIGHT, A>::board::board>::matrix> board;
};

template <typename GB, int R, int C, Direction Dir, int A>
struct MoveVehicle;

template <typename L, int R, int C, Direction Dir, int A>
struct MoveVehicle<GameBoard<L>, R, C, Dir, A>
{
    static_assert((R < GameBoard<L>::length && R >= 0), "Row is out of bounds");
    static_assert((C < GameBoard<L>::width) && C >= 0, "Coloum is out of bounds");
    static_assert((GetAtIndex<C, typename GetAtIndex<R, L>::value>::value::type != EMPTY), "Cell is Empty");
    static_assert((isValidDirection<GetAtIndex<C, typename GetAtIndex<R, L>::value>::value::direction, Dir>::value), "invalid direction");

public:
    typedef typename MoveVehicleAux<GameBoard<L>, R, C, Dir, A>::board board;
};
#endif