#ifndef UTILITIES_H
#define UTILITIES_H

template <bool exp, typename T1, typename T2>
struct Conditional
{
};

template <typename T1, typename T2>
struct Conditional<true, T1, T2>
{
    typedef T1 value;
};

template <typename T1, typename T2>
struct Conditional<false, T1, T2>
{
    typedef T2 value;
};

template <bool exp, int N1, int N2>
struct ConditionalInteger
{
};

template <int N1, int N2>
struct ConditionalInteger<true, N1, N2>
{
    constexpr static int value = N1;
};

template <int N1, int N2>
struct ConditionalInteger<false, N1, N2>
{
    constexpr static int value = N2;
};

#endif