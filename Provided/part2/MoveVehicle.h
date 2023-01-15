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
    constexpr static CellType type = CT;
    constexpr static Direction direction = Dir;
    constexpr static int amount = Amount;
};

template <Direction Dir, int Amount>
struct Move<EMPTY, Dir, Amount>
{
    static_assert(false, "cell type can't be empty!");
};

// ========= MOVE_VEHICLE ========= //
template <Direction D1>
struct isValidDirection
{
    constexpr static bool value = (D1 == LEFT || D1 == RIGHT);
};

template <typename GB, int R, int C, Direction Dir, int A>
struct MoveVehicle
{
};

template <typename L, int R, int C, Direction Dir, int A>
struct MoveVehicle<GameBoard<L>, R, C, Dir, A>
{
    static_assert((R < GameBoard<L>::length), "Row is out of bounds");
    static_assert((C < GameBoard<L>::width), "Coloum is out of bounds");
    static_assert((GetAtIndex<C, GetAtIndex<R, L>>::value::type != EMPTY), "Cell is Empty");
    static_assert((isValidDirection<GetAtIndex<C, GetAtIndex<R, L>>::value::direction>::value), "invalid direction");
    
};

#endif