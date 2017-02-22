//
// Created by tyler on 2/21/17.
//

#ifndef YAFEL_SEQUENCE_FUNCTIONS_HPP
#define YAFEL_SEQUENCE_FUNCTIONS_HPP

#include "sequences.hpp"

YAFEL_NAMESPACE_OPEN


// Sequence sum
constexpr int sum(sequence<>) { return 0; }

template<int S, int ...SS>
constexpr int sum(sequence<S, SS...>) { return S + sum(sequence<SS...>()); }



// Concatenate sequences
template<int ...PP, int ...SS>
constexpr auto seq_cat(sequence<PP...>, sequence<SS...>)
{
    return sequence<PP..., SS...>();
}

YAFEL_NAMESPACE_CLOSE

#endif //YAFEL_SEQUENCE_FUNCTIONS_HPP