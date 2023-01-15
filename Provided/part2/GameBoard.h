#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "List.h"
#include "BoardCell.h"

template <typename L>
struct GameBoard
{
    typedef L board;
    constexpr static int width = board::head::size;
    constexpr static length = board::size;
};

#endif