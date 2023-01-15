#ifndef BOARD_CELL_H
#define BOARD_CELL_H

#include "CellType.h"
#include "Direction.h"

template <CellType CT, Direction D, int L>
struct BoardCell
{
    constexpr static CellType type = CT;
    constexpr static Direction direction = D;
    constexpr static int length = L;
};


#endif