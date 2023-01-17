#ifndef LIST_H
#define LIST_H

/* List */
template <typename... TT>
struct List;

template <>
struct List<>
{
    constexpr static int size = 0;
};

template <typename T, typename... TT>
struct List<T, TT...>
{
    typedef T head;
    typedef List<TT...> next;
    constexpr static int size = sizeof...(TT) + 1;
};

/* PrependList */
template <typename T, typename U>
struct PrependList;

template <typename T, typename... U>
struct PrependList<T, List<U...>>
{
    typedef List<T, U...> list;
};

/* GetAtIndex */
template <int N, typename T>
struct GetAtIndex;

template <int N, typename T, typename... TT>
struct GetAtIndex<N, List<T, TT...>>
{
    typedef typename GetAtIndex<N - 1, List<TT...>>::value value;
};

template <typename T, typename... TT>
struct GetAtIndex<0, List<T, TT...>>
{
    typedef T value;
};

/* SetAtIndex */
template <int N, typename U, typename T>
struct SetAtIndex;

template <int N, typename U, typename T, typename... TT>
struct SetAtIndex<N, U, List<T, TT...>>
{
    typedef typename PrependList<T, typename SetAtIndex<N - 1, U, List<TT...>>::list>::list list;
};

template <typename U, typename T, typename... TT>
struct SetAtIndex<0, U, List<T, TT...>>
{
    typedef typename PrependList<U, List<TT...>>::list list;
};

// ========= ReverseList ========= //
template <typename L>
struct ReverseList;

template <typename... TT, typename T>
struct ReverseList<List<TT..., T>>
{
    typedef typename PrependList<T, typename ReverseList<List<TT...>>::list>::list list;
};

template <>
struct ReverseList<List<>>
{
    typedef List<> list;
};

#endif