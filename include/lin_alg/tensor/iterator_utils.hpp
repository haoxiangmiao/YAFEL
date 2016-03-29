#ifndef _YAFEL_ITERATOR_UTILS
#define _YAFEL_ITERATOR_UTILS

#include "yafel_globals.hpp"
#include <tuple>

YAFEL_NAMESPACE_OPEN

// Sequence generator
template <int ...S>
struct seq{};

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...> {};

template<int ...S>
struct gens<0, S...> {
  typedef seq<S...> type;
};


// Tuple-type generator
template<int N, typename ...Args>
struct type_gen : type_gen<N-1, unsigned, Args...> {};

template<typename ...Args>
struct type_gen<0, Args...> {
  typedef std::tuple<Args...> type;
};

// zeros generator
template<int ...S>
struct zseq{};

template<int N, int ...S>
struct zeroGen : zeroGen<N-1, 0, S...> {};

template<int ...S>
struct zeroGen<0,S...> {
  typedef zseq<S...> type;
};

YAFEL_NAMESPACE_CLOSE

#endif
