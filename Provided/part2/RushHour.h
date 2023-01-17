#ifndef RUSH_HOUR
#define RUSH_HOUR

#include "MoveVehicle.h"
#include "Printer.h"
#include <iostream>
// ========== GET_CAR_LINE ========== //
template <CellType CT, int Index, typename L>
struct getCarLine;

template <CellType CT, int Index, typename T, typename... TT>
struct getCarLine<CT, Index, List<T, TT...>>
{
    constexpr static int idx = ConditionalInteger<T::type == CT, Index, getCarLine<CT, Index + 1, List<TT...>>::idx>::value;
};

template <CellType CT, int Index>
struct getCarLine<CT, Index, List<>>
{
    constexpr static int idx = -1;
};

// ========== GET_CAR ========== //
template <int RowIndex, CellType CT, typename L>
struct getCar;

template <int RowIndex, CellType CT, typename T, typename... TT>
struct getCar<RowIndex, CT, List<T, TT...>>
{
private:
    // static_assert((RowIndex < sizeof...(TT)), "red car isn't found");
    constexpr static int colNum = getCarLine<CT, 0, T>::idx;

public:
    constexpr static int idx = ConditionalInteger<colNum != -1, colNum, getCar<RowIndex + 1, CT, List<TT...>>::idx>::value;
    constexpr static int row_idx = ConditionalInteger<colNum != -1, RowIndex, getCar<RowIndex + 1, CT, List<TT...>>::row_idx>::value;
};

template <int RowIndex, CellType CT>
struct getCar<RowIndex, CT, List<>>
{
    constexpr static int idx = -1;
    constexpr static int row_idx = -1;
};

// ========== CHECK_WIN ========== //
template <typename T>
struct CheckWin;

template <typename... TT>
struct CheckWin<GameBoard<List<TT...>>>
{
private:
    constexpr static int colNum = getCar<0, CellType::X, List<TT...>>::idx;
    typedef typename GetAtIndex<getCar<0, CellType::X, List<TT...>>::row_idx, List<TT...>>::value row_list;
    constexpr static int lastRedCarIndex = GetLastIndexOfCar<colNum, CellType::X, row_list>::idx;

public:
    constexpr static bool result = isLegalMove<GameBoard<List<TT...>>::width - lastRedCarIndex - 1, colNum, row_list>::islegal;
};

// ========== CHECK_SOLUTION ========== //
template <typename T, typename U>
struct CheckSolution;

template <typename... TT, typename U, typename... UU>
struct CheckSolution<GameBoard<List<TT...>>, List<U, UU...>>
{
private:
    constexpr static int colNum = getCar<0, U::type, List<TT...>>::idx;
    constexpr static int rowNum = getCar<0, U::type, List<TT...>>::row_idx;
    typedef typename MoveVehicle<GameBoard<List<TT...>>, rowNum, colNum, U::direction, U::amount>::board board;

public:
    constexpr static bool result = CheckSolution<board, UU...>::result;
};

template <typename... TT>
struct CheckSolution<GameBoard<List<TT...>>, List<>>
{
    constexpr static bool result = CheckWin<GameBoard<List<TT...>>>::result;
};
#endif