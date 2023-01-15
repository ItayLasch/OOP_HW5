#ifndef LIST_H
#define LIST_H

template<typename... TT>
struct List{};

template <>
struct List<>
{
    constexpr static int size = 0;
};

template <typename T, typename... TT>
struct List
{
    typedef T head;
    typedef List<TT...> next;
    constexpr static int size = sizeof...(TT) + 1;
};

// ========= PrependList ========= //

template<typename T, typename U>
struct PrepentList{};

template <typename T, typename... U>
struct PrepentList<T, List<U...>>
{
    typedef List<T,U...> list;
};

// ========= GetAtIndex ========= //
template<int N, typename T>
struct GetAtIndex{};

template<int N, typename... TT>
struct GetAtIndex<N,List<TT...>>
{
    typedef typename GetAtIndex<N - 1, List<TT...>::tail>::value value;
};

template <typename... TT>
struct GetAtIndex<0, List<TT...>>
{
    typedef List<TT...>::head value;
};

// ========= SetAtIndex ========= //
template <int N,typename U ,typename T>
struct SetAtIndex
{
};

template <int N, typename U,typename T ,typename... TT>
struct SetAtIndex<N, U, List<T,TT...>>
{
    typedef typename PrepentList<List<T, typename SetAtIndex<N - 1, U, List<TT...>>::list>>::list list;
};

template <typename U,typename T, typename... TT>
struct SetAtIndex<0, U,List<T,TT...>>
{
    typedef typename List<U,TT...> list;
};
#endif